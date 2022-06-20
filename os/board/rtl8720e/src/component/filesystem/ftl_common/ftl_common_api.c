#include "platform_opts.h"
#if CONFIG_FTL_EN
#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal.h"
#include "log_service.h"
#include "snand_api.h"
#include "log_service.h"
#include "ftl_nand_api.h"
#include "device_lock.h"
#include "sys_api.h"
#include "hal_sys_ctrl.h"
#include "ftl_common_api.h"
_mutex *ftl_mutex = NULL;
_mutex _ftl_mutex;
_mutex *snand_mutex = NULL;
_mutex _snand_mutex;
void snand_resource_lock(void)
{
	if (snand_mutex == NULL) {
		snand_mutex = &_snand_mutex;
		rtw_mutex_init(snand_mutex);
		rtw_mutex_get(snand_mutex);
	} else {
		rtw_mutex_get(snand_mutex);
	}
}

void snand_resource_unlock(void)
{
	if (snand_mutex == NULL) {
		snand_mutex = &_snand_mutex;
		rtw_mutex_init(snand_mutex);
		rtw_mutex_put(snand_mutex);
	} else {
		rtw_mutex_put(snand_mutex);
	}
}
void ftl_lock(void)
{
	if (ftl_mutex == NULL) {
		ftl_mutex = &_ftl_mutex;
		rtw_mutex_init(ftl_mutex);
		rtw_mutex_get(ftl_mutex);
	} else {
		rtw_mutex_get(ftl_mutex);
	}
}

void ftl_unlock(void)
{
	if (ftl_mutex == NULL) {
		ftl_mutex = &_ftl_mutex;
		rtw_mutex_init(ftl_mutex);
		rtw_mutex_put(ftl_mutex);
	} else {
		rtw_mutex_put(ftl_mutex);
	}
}

int _ftl_common_read(unsigned int address, unsigned char *buf, unsigned int size, int offset)
{
	int ret = 0;
	if (sys_get_boot_sel() == FTL_NAND_FLASH) {
		int block, page = 0;
		nand_info info;
		ret = ftl_init_nand();
		if (ret < 0) {
			printf("Init nand flash fail\r\n");
			return -1;
		}
		ftl_get_nand_info(&info);
		if ((address % info.page_size) != 0 || (size + offset) > info.page_size) {
			printf("It don't alignment page size %x %d\r\n", address, info.page_size);
			return -1;
		}
		block = address / (info.page_size * info.page_per_blk);
		page = ((address) / info.page_size) % info.page_per_blk;
		ret = ftl_read_nand(block, page, buf, size, offset);
	} else {
		ret = ftl_init_nor();
		if (ret < 0) {
			printf("Init nor flash fail\r\n");
		}
		if ((address % NOR_FLASH_SECTOR) != 0 || (size + offset) > NOR_FLASH_SECTOR) {
			printf("It don't alignment block size %x 4096\r\n", address);
			return -1;
		}
		ret = ftl_read_nor(address, buf, size, offset);
	}
	return ret;
}

int ftl_common_read(unsigned int address, unsigned char *buf, unsigned int size)
{
	int i = 0;
	int pgae_size = 0;
	int size_after = 0;
	int address_after = 0;
	int ret = 0;
	int offset = 0;

	int type, page_size, block_size, block_cnt = 0;

	ret =  ftl_common_info(&type, &page_size, &block_size, &block_cnt);
	if (ret < 0) {
		printf("Init flash fail\r\n");
		return ret;
	}
	ftl_lock();
	if (address % page_size) {
		if ((address % page_size) + size <= page_size) {
			ret = _ftl_common_read(address - (address % page_size), buf, size, address % page_size);
		} else {
			offset = address % page_size;
			size_after = size - (page_size - offset);
			address_after = address + (page_size - offset);
			ret = _ftl_common_read(address - offset, buf, page_size - offset, offset); //
			for (i = 0; i < (size_after / page_size); i++) {
				ret = _ftl_common_read(address_after + i * page_size, (buf + (page_size - offset)) + i * page_size, page_size, 0);
			}
			if (size_after % page_size) {
				ret = _ftl_common_read(address_after + i * page_size, (buf + (page_size - offset)) + i * page_size, size_after % page_size, 0);
			}
		}
	} else {
		for (i = 0; i < size / page_size; i++) {
			ret = _ftl_common_read(address + i * page_size, buf + i * page_size, page_size, 0);
		}
		if (size % page_size) {
			ret = _ftl_common_read(address + i * page_size, buf + i * page_size, size % page_size, 0);
		}
	}
	ftl_unlock();
	return ret;
}

int _ftl_common_write(unsigned int address, unsigned char *buf, unsigned int size, int offset)
{
	int ret = 0;
	if (sys_get_boot_sel() == FTL_NAND_FLASH) {
		int block, page = 0;
		nand_info info;
		ret = ftl_init_nand();
		if (ret < 0) {
			printf("Init nand flash fail\r\n");
			return -1;
		}
		ftl_get_nand_info(&info);
		if ((address % info.page_size) != 0 || (size + offset) > info.page_size) {
			printf("It don't alignment page size %x %d\r\n", address, info.page_size);
			return -1;
		}
		block = address / (info.page_size * info.page_per_blk);
		page = ((address) / info.page_size) % info.page_per_blk;
		ret = ftl_write_nand(block, page, buf, size, offset);
	} else {
		ret = ftl_init_nor();
		if (ret < 0) {
			printf("Init nor flash fail\r\n");
			return -1;
		}
		if ((address % NOR_FLASH_SECTOR) != 0 || (size + offset) > NOR_FLASH_SECTOR) {
			printf("It don't alignment block size %x 4096\r\n", address);
			return -1;
		}
		ret = ftl_write_nor(address, buf, size, offset);
	}
	return ret;
}

int ftl_common_write(unsigned int address, unsigned char *buf, unsigned int size)
{
	int i = 0;
	int pgae_size = 0;
	int size_after = 0;
	int address_after = 0;
	int ret = 0;
	int offset = 0;

	int type, page_size, block_size, block_cnt = 0;

	ret =  ftl_common_info(&type, &page_size, &block_size, &block_cnt);
	if (ret < 0) {
		printf("Init flash fail\r\n");
		return ret;
	}
	ftl_lock();
	if (address % page_size) {
		if ((address % page_size) + size <= page_size) {
			ret = _ftl_common_write(address - (address % page_size), buf, size, address % page_size);
		} else {
			offset = address % page_size;
			size_after = size - (page_size - offset);
			address_after = address + (page_size - offset);
			ret = _ftl_common_write(address - offset, buf, page_size - offset, offset); //
			for (i = 0; i < (size_after / page_size); i++) {
				ret = _ftl_common_write(address_after + i * page_size, (buf + (page_size - offset)) + i * page_size, page_size, 0);
			}
			if (size_after % page_size) {
				ret = _ftl_common_write(address_after + i * page_size, (buf + (page_size - offset)) + i * page_size, size_after % page_size, 0);
			}
		}
	} else {
		for (i = 0; i < size / page_size; i++) {
			ret = _ftl_common_write(address + i * page_size, buf + i * page_size, page_size, 0);
		}
		if (size % page_size) {
			ret = _ftl_common_write(address + i * page_size, buf + i * page_size, size % page_size, 0);
		}
	}
	ftl_unlock();
	return ret;
}

int ftl_common_erase(unsigned int address)
{
	int ret = 0;
	ftl_lock();
	if (sys_get_boot_sel() == FTL_NAND_FLASH) {
		int block, page = 0;
		nand_info info;
		ret = ftl_init_nand();
		if (ret < 0) {
			printf("Init nand flash fail\r\n");
			goto EXIT;
		}
		ftl_get_nand_info(&info);
		if ((address % (info.page_size * info.page_per_blk)) != 0) {
			printf("It don't alignment block size %x %d\r\n", address, info.page_size * info.page_per_blk);
			goto EXIT;
		}
		block = address / (info.page_size * info.page_per_blk);
		ret = ftl_erase_nand(block);
	} else {
		ret = ftl_init_nor();
		if (ret < 0) {
			printf("Init nor flash fail\r\n");
			goto EXIT;
		}
		if ((address % NOR_FLASH_SECTOR) != 0) {
			printf("It don't alignment sector size %x 4096\r\n", address);
			goto EXIT;
		}
		ret = ftl_erase_nor(address);
	}
EXIT:
	ftl_unlock();
	return ret;
}

int ftl_erase_sector(unsigned int address)//For 4k sector
{
	int ret = 0;

	if (sys_get_boot_sel() == FTL_NAND_FLASH) {
		int block, page = 0;
		unsigned char *buf = NULL;
		nand_info info;
		ftl_lock();
		ret = ftl_init_nand();
		ftl_unlock();
		if (ret < 0) {
			printf("Init nand flash fail\r\n");
			goto EXIT;
		}
		ftl_get_nand_info(&info);
		if ((address % NOR_FLASH_SECTOR) != 0) {
			printf("It don't alignment block size %x %d\r\n", address, NOR_FLASH_SECTOR);
			goto EXIT;
		}
		buf = malloc(NOR_FLASH_SECTOR);
		if (buf == NULL) {
			printf("It can't be allocated buffer\r\n");
			goto EXIT;
		}
		memset(buf, 0xff, NOR_FLASH_SECTOR);
		ret = ftl_common_write(address, buf, NOR_FLASH_SECTOR);
		if (buf) {
			free(buf);
		}
	} else {
		ftl_lock();
		ret = ftl_init_nor();
		ftl_unlock();
		if (ret < 0) {
			printf("Init nor flash fail\r\n");
			goto EXIT;
		}
		if ((address % NOR_FLASH_SECTOR) != 0) {
			printf("It don't alignment sector size %x 4096\r\n", address);
			goto EXIT;
		}
		ftl_lock();
		ret = ftl_erase_nor(address);
		ftl_unlock();
	}
EXIT:
	return ret;
}

int ftl_common_info(int *type, int *page_size, int *block_size, int *block_cnt)
{
	int ret = 0;
	ftl_lock();
	if (sys_get_boot_sel() == FTL_NAND_FLASH) {
		int block, page = 0;
		nand_info info;
		ret = ftl_init_nand();
		if (ret < 0) {
			printf("Init nand flash fail\r\n");
			goto EXIT;
		}
		ftl_get_nand_info(&info);
		*type = FTL_NAND_FLASH;
		*page_size = info.page_size;
		*block_size = info.page_size * info.page_per_blk;
		*block_cnt = info.blk_cnt;
	} else {
		ret = ftl_init_nor();
		if (ret < 0) {
			printf("Init nor flash fail\r\n");
			goto EXIT;
		}
		*type = FTL_NOR_FLASH;
		*page_size = NOR_FLASH_SECTOR;
		*block_size = 0;
		*block_cnt = 0;
	}
EXIT:
	ftl_unlock();
	return ret;
}
#endif