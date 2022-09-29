#include "platform_opts.h"
#if CONFIG_FTL_EN
#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal.h"
#include "log_service.h"
#include <platform_opts_bt.h>
#include "snand_api.h"
#include "log_service.h"
#include "ftl_nand_api.h"
#include "device_lock.h"
#include "sys_api.h"
#include "hal_sys_ctrl.h"

#define NAND_BLOCK_COUNT 	1024
#define NAND_PAGE_PER_BLOCK 64
#define NAND_PAGE_SIZE 		2048
#define NAND_SPARE_SIZE 	64

#if (CONFIG_FTL_VERIFY || CONFIG_NAND_CB_TEST)
#define CONIG_FTL_TEST_CNT 0x10000
static int make_incorrect_data = 0;
void ftl_test_set_make_incorrect_data(bool value)
{
	make_incorrect_data = value;
}
#endif

bbm_info_attr *bbm_info = NULL;

int bbm_find_table_index(bbm_info_attr *bbm)
{
	for (int i = 1; i <= bbm->pbbt_size; i++) {
		if (bbm->pbbt[i].bad_block == 0xffff) {
			bbm->rba_table_index = i;
			//printf("bbm->rba_table_index %d\r\n",bbm->rba_table_index);
			return 0;
		}
	}
	return -1;
}

int nand_read_cb(int page, int size, unsigned char *buf)
{
	snand_t obj;
	int ret = 0;
	ret = snand_page_read(&obj, page, size, buf);
	return ret;
}
int nand_write_cb(int page, int size, unsigned char *buf)
{
	snand_t obj;
	int ret = 0;
	ret = snand_page_write(&obj, page, size, buf);
	return ret;
}

int nand_erase_cb(int page)
{
	snand_t obj;
	int ret = 0;
	ret = snand_erase_block(&obj, page);
	return ret;
}

void nand_unlock_cb(void)
{
	snand_global_unlock();
}



bool bbm_nand_is_free(bbm_info_attr *bbm, unsigned int block, unsigned int page)
{
	int ret = 0;
	ret = bbm_info->nand_read_cb(block * bbm->page_per_blk + page, bbm->page_size, bbm->pbuf_read);
	if (ret < 0) {
		goto EXIT;
	}
	for (int i = 0 ; i < bbm->page_size; i++) {
		if (bbm->pbuf_read[i] != 0xff) {
			//printf("The content is not empty, do the erase step\r\n");
			goto EXIT;
		}
	}
	//printf("The block is free\r\n");
	return true;
EXIT:
	return false;
}


int bbm_nand_write_direct(bbm_info_attr *bbm, unsigned int block, unsigned int page, unsigned char *buf, int size, int offset)
{
	int ret = 0;

	memset(bbm->pbuf, 0xff, bbm->page_size);
	memset(bbm->pbuf_read, 0xff, bbm->page_size);
	memcpy(bbm->pbuf + offset, buf, size);
	if (memcmp(bbm->pbuf_read, bbm->pbuf, bbm->page_size) == 0) { //It don't need to write
		return ret;
	}

	ret = bbm_info->nand_write_cb(block * bbm->page_per_blk + page, bbm->page_size, bbm->pbuf);
	if (ret < 0) {
		goto EXIT;
	}
	ret = bbm_info->nand_read_cb(block * bbm->page_per_blk + page, bbm->page_size, bbm->pbuf_read);
	if (ret < 0) {
		goto EXIT;
	}
#if (CONFIG_FTL_VERIFY || CONFIG_NAND_CB_TEST)
	if (!page && make_incorrect_data) {
		*(bbm->pbuf_read) = 0x87;
		make_incorrect_data = 0;
	}
#endif
	if (memcmp(bbm->pbuf, bbm->pbuf_read, bbm->page_size) != 0) {
		ret = -1;
		goto EXIT;
	}
	return ret;
EXIT:
	return ret;
}

int bbm_nand_read_block(bbm_info_attr *bbm, int block)
{
	int ret = 0;
	int i = 0;
	for (i = 0; i < bbm->page_per_blk; i++) {
		ret = bbm_info->nand_read_cb(block * bbm->page_per_blk + i, bbm->page_size, bbm->block_buf + i * bbm->page_size);
	}
	return ret;
}

int bbm_nand_write_block(bbm_info_attr *bbm, int block) //This will be erased
{
	int ret = 0;
	int i = 0;
	for (i = 0; i < bbm->page_per_blk; i++) {
		memset(bbm->pbuf, 0xff, bbm->page_size); //Check whether the data need to write to nand flash
		if (memcmp(bbm->pbuf, bbm->block_buf + i * bbm->page_size, bbm->page_size) != 0) {
			ret = bbm_nand_write_direct(bbm, block, i, bbm->block_buf + i * bbm->page_size, bbm->page_size, 0);
			if (ret < 0) {
				return ret;
			}
		}
	}
	return ret;
}


int bbm_nand_write(bbm_info_attr *bbm, unsigned int block, unsigned int page, unsigned char *buf, int size, int offset)
{
	int ret = 0;
	if (size > bbm->page_size || page >= bbm->page_per_blk) {
		printf("The size %d is large than page size %d\r\n", size, bbm->page_size);
		goto EXIT;
	}

	if (bbm_nand_is_free(bbm, block, page) == false) {
		ret = bbm_nand_read_block(bbm, block);
		memcpy(bbm->block_buf + bbm->page_size * page + offset, buf, size);
		ret = bbm_info->nand_erase_cb((block) * bbm->page_per_blk);
		ret = bbm_nand_write_block(bbm, block);
	} else {
		ret = bbm_nand_write_direct(bbm, block, page, buf, size, offset);
		if (ret < 0) { //If it meet the erro recover the data
			printf("The block %d is bad page %d\r\n", block, page);
			bbm_nand_read_block(bbm, block);
			memcpy(bbm->block_buf + bbm->page_size * page, buf, size);
		}
	}
	if (ret < 0) {
		bbm_mark_bad_block(bbm, block);
	}
	return ret;
EXIT:
	return ret;
}

bool bbm_is_bad_block(bbm_info_attr *bbm, int block)
{
	int i = 0;
	bool ret = false;
	for (i = 1; i < bbm->pbbt_size; i++) { //Total number block
		if (bbm->pbbt[i].bad_block == 0xffff) {
			//printf("NAND_GOOD_BLOCK %d\r\n",block);
			ret = false;
			break;
		} else {
			if (bbm->pbbt[i].bad_block == block) {
				//printf("NAND_BAD_BLOCK %d\r\n",block);
				ret = true;
				break;
			}
		}
	}
	return ret;
}

bool bbm_is_remap_block(bbm_info_attr *bbm, int block)
{
	int i = 0;
	bool ret = false;
	for (i = 1; i < bbm->pbbt_size; i++) { //Total number block
		if (bbm->pbbt[i].remap_block == 0xffff && bbm->pbbt[i].bad_block == 0xffff) {
			//printf("NAND_GOOD_REMAP_BLOCK %d\r\n",block);
			ret = false;
			break;
		} else {
			if (bbm->pbbt[i].remap_block == block) {
				//printf("NAND_RESERVE_BLOCK %d\r\n",block);
				ret = true;
				break;
			}
		}
	}
	return ret;
}

int bbm_find_bad_block_index(bbm_info_attr *bbm, int block, int *bad_block_index) //Find the bad block index
{
	int i = 0;
	int ret = BBM_RET_FAIL;
	for (i = 1; i < bbm->pbbt_size; i++) {
		if (bbm->pbbt[i].bad_block == 0xffff) {
			return BBM_RET_FAIL;
		} else {
			if (bbm->pbbt[i].bad_block == block) {
				*bad_block_index = i;
				return BBM_RET_OK;
			}
		}
	}
	return ret;
}

int bbm_find_free_block(bbm_info_attr *bbm, int *remab_block)
{
	int i = 0;
	int ret = BBM_RET_FAIL;
	for (i = 0; i < bbm->rba_cnt; i++) { //Search the range from reserve block
		if (bbm_is_bad_block(bbm, i + bbm->rba_offset) == false && bbm_is_remap_block(bbm, i + bbm->rba_offset) == false) {
			bbm->rba_index = i + bbm->rba_offset;
			*remab_block = bbm->rba_index;
			//printf("Find the block %d\r\n",*remab_block);
			return BBM_RET_OK;
		}
	}
	return ret; //It can't find the free block
}

int bbm_find_remap_block(bbm_info_attr *bbm, int block, int *remab_block)
{
	int i = 0;
	int ret = BBM_RET_FAIL;
	int rba = 0;
	for (i = 1; i < bbm->pbbt_size; i++) {
		if (bbm->pbbt[i].bad_block == block) {
			if (bbm->pbbt[i].remap_block == 0xffff) {
				ret = bbm_find_free_block(bbm, &rba);
				if (ret == BBM_RET_OK) {
					bbm->pbbt[i].remap_block = rba;
					*remab_block = bbm->pbbt[i].remap_block;
					break;
				} else {
					ret = BBM_RET_FAIL;
					break;
				}
			} else {
				if (bbm_is_bad_block(bbm, bbm->pbbt[i].remap_block) == false) {
					ret = BBM_RET_OK;
					*remab_block = bbm->pbbt[i].remap_block;
					break;
				} else {
					ret =  bbm_find_free_block(bbm, &rba);
					if (ret == BBM_RET_OK) {
						bbm->pbbt[i].remap_block = rba;
						*remab_block = bbm->pbbt[i].remap_block;
					}
					break;
				}
			}
		}
	}
	return ret; //It can't find the free block
}

int bbm_check_status(bbm_info_attr *bbm)
{
	int count = 0;
	for (int i = 0; i < bbm->pbbt_size; i++) {
		if (bbm->pbbt[i].start_tag == BBM_START_TAG) {
			if (bbm->pbbt[i].end_tag == BBM_END_TAG) {
				count++;
			} else {
				printf("The bbm table is broken\r\n");
				return -1;
			}
		}
	}
	printf("BBM count %d\r\n", count);
	return count;
}

int bbm_check_is_init(bbm_info_attr *bbm)
{
	int i = 0;
	int tag = 0;
	int ret = 0;
	bbt bbt_value;
	for (i = 0; i < bbm->bbmt_cnt ; i++) {
		ret = bbm_info->nand_read_cb((i + bbm->bbmt_offset) * bbm->page_per_blk, bbm->page_size + bbm->spare_size, bbm->pbuf);
		if (ret == 0) {
			if (bbm->pbuf[bbm->page_size] == 0xff) {
				memcpy(&bbt_value, bbm->pbuf, sizeof(bbt));
				if (bbt_value.start_tag == BBM_START_TAG && bbt_value.end_tag == BBM_END_TAG) {
					memcpy(bbm->pbbt, bbm->pbuf, bbm->page_size);
					ret = bbm_check_status(bbm);
					if (ret < 0) {
						printf("The bbm is not correct\r\n");
					} else {
						printf("The bbm is already init\r\n");
						tag = 1;
						break;
					}
				} else {
					printf("It bbm is not init %d\r\n", i + bbm->bbmt_offset);
					printf("Start tag %x End tag %x\r\n", bbt_value.start_tag, bbt_value.end_tag);
					tag = 0;
					break;
				}
			}
		}
	}
	return tag;
}

int bbm_mark_tag_block(bbm_info_attr *bbm, int block)//The block is broken don't care the result
{
	int ret = 0;
	memset(bbm->pbuf, 0xff, bbm->page_size + bbm->spare_size);
	bbm->pbuf[bbm->page_size] = 0x01; //Add the tag at the spare space.
	ret = bbm_info->nand_erase_cb((block) * bbm->page_per_blk);
	ret = bbm_info->nand_write_cb(block * bbm->page_per_blk, bbm->page_size + bbm->spare_size, bbm->pbuf);
	return ret;
}

int bbm_mark_bad_block(bbm_info_attr *bbm_info, int block)
{
	int bad_block_index = 0;
	int ret = 0;
	ret = bbm_find_bad_block_index(bbm_info, block, &bad_block_index);
	if (ret == BBM_RET_FAIL) {
		bbm_find_table_index(bbm_info);
		bbm_info->pbbt[bbm_info->rba_table_index].start_tag = BBM_START_TAG;       /* "BB" */
		bbm_info->pbbt[bbm_info->rba_table_index].bad_block = block;
		bbm_info->pbbt[bbm_info->rba_table_index].end_tag = BBM_END_TAG;
		printf("bad_block %d rba_table_index %d\r\n", block, bbm_info->rba_table_index);
		ret = BBM_RET_OK;
		bbm_mark_tag_block(bbm_info, block);
	}
	return ret;
}

int ftl_write_nand(int block, int page, unsigned char *buf, int size, int offset)
{
	int ret = 0;
	int remap_block = 0;
	int i = 0;
	int write_block = 0;
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return -1;
	}
	bbm_info_attr *bbm = bbm_info;
	if (size + offset > bbm->page_size || block >= bbm->blk_cnt) {
		printf("The max page is %d block cnt %d\r\n", bbm->page_size, bbm->blk_cnt);
		return -1;
	}
	if (block >= bbm->rba_offset) {
		printf("The block is reserved %d\r\n", block);
		return -1;
	}

	if (bbm_is_bad_block(bbm, block)) {
		ret = bbm_find_remap_block(bbm, block, &remap_block); //Use the remap block
		if (!ret) {
			ret = bbm_nand_write(bbm, remap_block, page, buf, size, offset);
			//printf("Remap write block %d\r\n",remap_block);
		}
	} else {
		ret = bbm_nand_write(bbm, block, page, buf, size, offset);
	}

	if (ret < 0) {
		for (i = 0; i < FTL_MAX_RETRY; i++) {
			if (bbm_is_bad_block(bbm, block)) {
				ret = bbm_find_remap_block(bbm, block, &remap_block); //Use the remap block
				if (!ret) {
					ret = bbm_info->nand_erase_cb((remap_block) * bbm->page_per_blk);
					ret = bbm_nand_write_block(bbm, remap_block);
				} else {
					printf("can not find remap block\n");
				}
			}
			if (ret >= 0) {
				break;
			} else {
				bbm_mark_bad_block(bbm, remap_block);
				printf("Recover the data to other block %d\r\n", remap_block);
			}
		}
		ret = bbm_update_table(bbm);//Update the bad blcoj result
	}

	return ret;
}

#if CONFIG_NAND_CB_TEST
void ftl_scan_and_mark_bad_block(uint32_t start, uint32_t end)
{
	int ret = 0;

	for (int i = start; i < end; i++) {
		ret = bbm_info->nand_erase_cb(i * bbm_info->page_per_blk);
		if (ret & (i < bbm_info->bbmt_index)) {
			bbm_mark_tag_block(bbm_info, i);
		}
	}
}
#endif

#if CONFIG_LITTLE_WEAR_LEVELING_VERIFY
uint32_t wlcount[512];
#endif
int ftl_erase_nand(int block)
{
	int ret = 0;
	int i = 0;
	int remap_block = 0;
#if CONFIG_LITTLE_WEAR_LEVELING_VERIFY
	wlcount[block - 512]++;
#endif
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return -1;
	}
	bbm_info_attr *bbm = bbm_info;
	if (block >= bbm->rba_offset) {
		printf("The block is reserved %d\r\n", block);
		return -1;
	}

	for (i = 0; i < FTL_MAX_RETRY; i++) {
		if (bbm_is_bad_block(bbm, block)) {
			ret = bbm_find_remap_block(bbm, block, &remap_block); //Use the remap block
			ret = bbm->nand_erase_cb((remap_block) * bbm->page_per_blk);
			//printf("Remap erase block %d\r\n", remap_block);
			if (ret < 0) {
				printf("bad block happen %d\r\n", block);
			}
		} else {
			ret = bbm->nand_erase_cb((block) * bbm->page_per_blk);
			if (ret < 0) {
				printf("bad block happen %d\r\n", block);
			}
		}
		if (ret == 0) {
			break;
		}
	}

	return ret;
}

int ftl_read_nand(int block, int page, unsigned char *buf, int size, int offset)
{
	int ret = 0;
	int remap_block = 0;
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return -1;
	}
	bbm_info_attr *bbm = bbm_info;
	if (size + offset > bbm->page_size || block >= bbm->blk_cnt) {
		printf("The max page is %d block cnt %d\r\n", bbm->page_size, bbm->blk_cnt);
		return -1;
	}
	if (block >= bbm->rba_offset) {
		printf("The block is reserved %d\r\n", block);
		return -1;
	}

	if (bbm_is_bad_block(bbm, block)) {
		ret = bbm_find_remap_block(bbm, block, &remap_block); //Use the remap block
		ret = bbm_info->nand_read_cb((remap_block) * bbm->page_per_blk + page, bbm->page_size + bbm->spare_size, bbm->pbuf);
		//printf("Remap read block %d\r\n", remap_block);
	} else {
		ret = bbm_info->nand_read_cb((block) * bbm->page_per_blk + page, bbm->page_size + bbm->spare_size, bbm->pbuf);
	}
	memcpy(buf, bbm->pbuf + offset, size);

	return ret;
}

int bbm_update_table(bbm_info_attr *bbm)
{
	int count = 0;
	int i = 0;
	int ret = 0;
	for (i = 0; i < FW_BAD_BLOCK_TABLE_CNT; i++) {
		if (bbm_is_bad_block(bbm, bbm->bbmt_offset + i) == false) {
			ret = bbm_nand_write(bbm, bbm->bbmt_offset + i, 0, bbm->pbbt, bbm->page_size, 0); //Update the bad blcoj result
			if (ret >= 0) {
				count += 1;
			}
			if (count == 2) {
				break;
			}
		}
	}
	if (ret < 0) {
		printf("The bbm table is fail\r\n");
	}
	return ret;
}

int bbm_create_nand(bbm_info_attr *bbm)//For the first time
{
	int i = 0;
	int ret = 0;
	bbm->pbbt[0].start_tag = BBM_START_TAG;
	bbm->pbbt[0].bad_block = BBM_BAD_TAG;
	bbm->pbbt[0].end_tag = BBM_END_TAG;
	bbm->pbbt[0].remap_block = BBM_REMAP_TAG;
	bbm->rba_table_index = 1;//bbm->blk_cnt-1-FW_BAD_BLOCK_TABLE_CNT;//1024-1-4=1019
	for (i = 0; i < bbm->blk_cnt; i++) { //First time it need to scan the bad block, it need cost time to finish the procedure
		ret = bbm_info->nand_read_cb(i * bbm->page_per_blk, bbm->page_size + bbm->spare_size, bbm->pbuf);
		if (bbm->pbuf[bbm->page_size] != 0xff) {
			bbm->pbbt[bbm->rba_table_index].start_tag = BBM_START_TAG;       /* "BB" */
			bbm->pbbt[bbm->rba_table_index].bad_block = i;
			bbm->pbbt[bbm->rba_table_index].end_tag = BBM_END_TAG;       /* "bb" */
			//printf("%d %x %x\r\n", bbm->rba_table_index, i, bbm->pbbt[bbm->rba_table_index].remap_block);
			bbm->rba_table_index++;
		}
	}
	ret = bbm_update_table(bbm);
	if (ret < 0) {
		printf("Create the table fail\r\n");
	} else {
		printf("bbm_create_nand %d\r\n", bbm->bbmt_index);
	}
	return ret;
}

#if CONFIG_FTL_VERIFY
int ftl_nand_verify_func(bbm_info_attr *bbm)
{
	int i = 0;
	int j = 0;
	uint32_t k = 0;
	int ret = 0;
	unsigned char *wdata;
	int tick = 0;

	printf("start verify ftl...\n");
	for (k = 0; k < CONIG_FTL_TEST_CNT; k++) {
		printf("\ntest no.%d\r\n", k);
		printf("\nclean bad block mark\n");
		for (i = 512; i < bbm_info->bbmt_offset; i++) {
			bbm_info->nand_erase_cb(i * bbm_info->page_per_blk);
		}
		printf("\nerase bad block table\r\n");
		ftl_erase_bbm_table();
		bbm_check_is_init(bbm);
		printf("\ncreate bad block table\n");
		bbm_create_nand(bbm);
		printf("\ndump bad block table\n");
		ftl_info_bbm_dump();
		wdata = malloc(bbm->page_size);
		memset(wdata, 0xa5, bbm->page_size);
		for (i = 512; i < bbm_info->rba_offset; i++) {
			ret = ftl_erase_nand(i);
			tick = xTaskGetTickCount();
			if ((tick & 0xFF) < 10) {
				make_incorrect_data = 1;
			}
			for (j = 0; j < bbm->page_per_blk; j++) {
				ret = ftl_write_nand(i, j, wdata, bbm->page_size, 0);
			}
		}

		free(wdata);
		printf("\ndump bad block table\n");
		ftl_info_bbm_dump();
	}
	printf("bbt function test finish\n");
	while (1);

	return ret;
}
#endif

void ftl_info_dump(void)
{
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return;
	}
	printf("bbm_info->blk_cnt %d\r\n", bbm_info->blk_cnt);
	printf("bbm_info->page_size %d\r\n", bbm_info->page_size);
	printf("bbm_info->page_per_blk %d\r\n", bbm_info->page_per_blk);
	printf("bbm_info->spare_size %d\r\n", bbm_info->spare_size);
	printf("bbm_info->rba_cnt %d\r\n", bbm_info->rba_cnt);
	printf("bbm_info->rba_index %d\r\n", bbm_info->rba_index);
	printf("bbm_info->rba_offset %d\r\n", bbm_info->rba_offset);
	printf("bbm_info->rba_table_index %d\r\n", bbm_info->rba_table_index);
	printf("bbm_info->rba_percent %d\r\n", bbm_info->rba_percent);
	printf("bbm_info->bbmt_cnt %d\r\n", bbm_info->bbmt_cnt);
	printf("bbm_info->bbmt_index %d\r\n", bbm_info->bbmt_index);
	printf("bbm_info->bbmt_offset %d\r\n", bbm_info->bbmt_offset);
	printf("bbm_info->bbmt_size %d\r\n", bbm_info->pbbt_size);
}

void ftl_info_bbm_dump(void)
{
	int i = 0;
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return;
	}
	bbm_info_attr *bbm = bbm_info;
	for (i = 0; i < bbm->pbbt_size; i++) {
		if (bbm->pbbt[i].bad_block != 0xffff) {
			printf("bbm index %d %x %d %x %d\r\n", i, bbm->pbbt[i].start_tag, bbm->pbbt[i].bad_block, bbm->pbbt[i].end_tag, bbm->pbbt[i].remap_block);
		}
	}
}

void ftl_bad_block_scan(void)
{
	int ret = 0;
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return;
	}
	printf("Bad block scan!\n\r");
	for (int i = 0; i < bbm_info->blk_cnt; i++) {
		ret = bbm_info->nand_read_cb(i * bbm_info->page_per_blk, bbm_info->page_size + bbm_info->spare_size, bbm_info->pbuf);
		if (bbm_info->pbuf[bbm_info->page_size] != 0xff) {
			printf("Block %d is bad block!\n\r", i);
		}
	}
}

int ftl_get_nand_info(nand_info *value)
{
	int ret = 0;
	if (ftl_init_nand() < 0) {
		printf("ftl_init_nand fail\r\n");
		return -1;
	}
	value->blk_cnt = bbm_info->blk_cnt;
	value->page_per_blk = bbm_info->page_per_blk;
	value->page_size = bbm_info->page_size;
	value->spare_size = bbm_info->spare_size;
	return ret;
}

void ftl_erase_bbm_table(void)
{
	int count = 0;
	int i = 0;
	int ret = 0;
	bbt bbt_value;
	for (i = 0; i < FW_BAD_BLOCK_TABLE_CNT; i++) {
		ret = bbm_info->nand_read_cb((i + bbm_info->bbmt_offset) * bbm_info->page_per_blk, bbm_info->page_size + bbm_info->spare_size, bbm_info->pbuf);
		if (bbm_info->pbuf[bbm_info->page_size] == 0xff) {
			memcpy(&bbt_value, bbm_info->pbuf, sizeof(bbt));
			if (bbt_value.start_tag == BBM_START_TAG && bbt_value.end_tag == BBM_END_TAG) {
				ret = bbm_info->nand_erase_cb((i + bbm_info->bbmt_offset) * bbm_info->page_per_blk);
			}
		}
	}
	memset(bbm_info->pbbt, 0xff, bbm_info->page_size);
	return ret;
}

int ftl_init_nand(void)
{
	int ret = 0;
	unsigned char fw_info[64] __attribute__((aligned(32)));
	if (sys_get_boot_sel() != 1) {
		printf("It is not nand flash\r\n");
	}
	if (bbm_info == NULL) {
		bbm_info = (bbm_info_attr *)malloc(sizeof(bbm_info_attr));
		if (bbm_info == NULL) {
			printf("It can't allocate the memory %s %d\r\n", __FUNCTION__, __LINE__);
			goto EXIT;
		}
		memset(bbm_info, 0, sizeof(bbm_info_attr));
		bbm_info->nand_read_cb  = nand_read_cb;
		bbm_info->nand_write_cb = nand_write_cb;
		bbm_info->nand_erase_cb  = nand_erase_cb;
		bbm_info->nand_unlock_cb = nand_unlock_cb;

		bbm_info->blk_cnt = NAND_BLOCK_COUNT;
		bbm_info->page_per_blk = NAND_PAGE_PER_BLOCK;
		bbm_info->page_size = NAND_PAGE_SIZE;
		bbm_info->spare_size = NAND_SPARE_SIZE;

		bbm_info->rba_percent = FW_RBA_PERCENT;
		bbm_info->bbmt_cnt = FW_BAD_BLOCK_TABLE_CNT;
		bbm_info->bbmt_offset = bbm_info->blk_cnt - bbm_info->bbmt_cnt; //1024 - 4
		bbm_info->bbmt_index = bbm_info->bbmt_offset;
		bbm_info->rba_cnt = (bbm_info->blk_cnt * bbm_info->rba_percent / 100);
		bbm_info->rba_offset = bbm_info->blk_cnt - (bbm_info->rba_cnt) - bbm_info->bbmt_cnt;
		bbm_info->rba_index = bbm_info->rba_offset;
		bbm_info->pbuf = (unsigned char *)malloc(bbm_info->page_size + bbm_info->spare_size);
		if (bbm_info->pbuf == NULL) {
			printf("It can't allocate the memory %s %d\r\n", __FUNCTION__, __LINE__);
			goto EXIT;
		}
		bbm_info->pbuf_read = (unsigned char *)malloc(bbm_info->page_size + bbm_info->spare_size);
		if (bbm_info->pbuf_read == NULL) {
			printf("It can't allocate the memory %s %d\r\n", __FUNCTION__, __LINE__);
			goto EXIT;
		}
		bbm_info->pbbt_size = (bbm_info->page_size) / sizeof(bbt);
		bbm_info->pbbt = (bbt *)malloc(bbm_info->page_size);
		if (bbm_info->pbbt == NULL) {
			printf("It can't allocate the memory %s %d\r\n", __FUNCTION__, __LINE__);
			goto EXIT;
		}
		bbm_info->rba_table_index = 1; //The default value
		bbm_info->block_buf = (unsigned char *)malloc(bbm_info->page_size * bbm_info->page_per_blk);
		if (bbm_info->block_buf == NULL) {
			printf("It can't allocate the memory %s %d\r\n", __FUNCTION__, __LINE__);
			goto EXIT;
		}
		memset(bbm_info->pbbt, 0xff, bbm_info->page_size);
		//ftl_info_dump();
		bbm_info->nand_unlock_cb();
		ret = bbm_check_is_init(bbm_info);
		if (ret == 0) {
			ret = bbm_create_nand(bbm_info);
		}
		//ftl_info_bbm_dump();
#if CONFIG_FTL_VERIFY
		ftl_nand_verify_func(bbm_info);
#endif
	} else {
		//printf("It is already init\r\n");
	}
	return ret;
EXIT:
	if (bbm_info) {
		if (bbm_info->pbuf) {
			free(bbm_info->pbuf);
		}
		if (bbm_info->pbuf_read) {
			free(bbm_info->pbuf_read);
		}
		if (bbm_info->pbbt) {
			free(bbm_info->pbbt);
		}
		if (bbm_info->block_buf) {
			free(bbm_info->block_buf);
		}
		free(bbm_info);
		bbm_info = NULL;
	}
	return -1;
}
#endif