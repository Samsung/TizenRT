#ifndef FTL_NAND_API_H
#define FTL_NAND_API_H
#include "platform_opts.h"
#if CONFIG_FTL_EN
#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal.h"
#include "log_service.h"
#include "snand_api.h"
#include "freertos_service.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bbt_t {
	u16 start_tag;             //bad block chip
	u16 bad_block;          //bad block,from v2r bb
	u16 remap_block;        //remap block, in RBA area
	u16 end_tag;             //remap block chip
} bbt;

typedef struct nand_info_t {
	int blk_cnt;
	int page_per_blk;
	int page_size;
	int spare_size;
} nand_info;

typedef struct {
	int blk_cnt;
	int page_per_blk;
	int page_size;
	int spare_size;
	bbt *pbbt;
	unsigned int pbbt_size;
	unsigned char *pbuf;//To read the page size;
	unsigned char *pbuf_read;//To read the page size;
	unsigned char *block_buf;
	int bbmt_cnt; //Total conut
	int bbmt_offset; //offset
	int bbmt_index; // index
	int rba_cnt;
	int rba_offset;// The start rba offset, It IS FIXED
	int rba_percent;
	int rba_index;//Rba block index
	int rba_table_index;//Rba table index
	//_mutex ftl_mutex;
	int (*nand_write_cb)(int page, int size, unsigned char *buf);
	int (*nand_read_cb)(int page, int size, unsigned char *buf);
	int (*nand_erase_cb)(int page);
	int (*nand_unlock_cb)(void);
} bbm_info_attr;

#define FW_RBA_PERCENT 5
#define FW_INFO_BLOCK_INDEX 0  //located at the first block
#define FW_RBA_PERCENT   5     //Reserve the 5% block as reserved block 
#define FW_BAD_BLOCK_TABLE_CNT  4//Reserve 4 block for bad block table

#define BBM_START_TAG 0X4242 //SS
#define BBM_END_TAG   0X6262 //EE
#define BBM_REMAP_TAG 0X5252 //RR
#define BBM_BAD_TAG 0X5454   //DD

#define PAGE_PER_BLK_INDEX 0x04
#define PAGE_SIZE_INDEX    0x08
#define SPARE_SIZE_INDEX   0x0c

#define BBM_RET_OK    0
#define BBM_RET_FAIL  -1

#define FTL_MAX_RETRY 5

int ftl_write_nand(int block, int page, unsigned char *buf, int size, int offset);
int ftl_erase_nand(int block);
int ftl_read_nand(int block, int page, unsigned char *buf, int size, int offset);
int ftl_init_nand(void);
int ftl_get_nand_info(nand_info *value);
void ftl_info_bbm_dump(void);
void ftl_info_dump(void);
void ftl_bad_block_scan(void);
#if CONFIG_NAND_CB_TEST
void ftl_test_set_make_incorrect_data(bool value);
void ftl_scan_and_mark_bad_block(uint32_t start, uint32_t end);
#endif

#ifdef __cplusplus
}
#endif
#endif
#endif /* FTL_NAND_API_H */