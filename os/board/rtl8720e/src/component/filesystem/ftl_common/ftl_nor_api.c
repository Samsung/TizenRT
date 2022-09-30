#include "platform_opts.h"
#if CONFIG_FTL_EN
#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal.h"
#include "log_service.h"
#include <platform_opts_bt.h>
#include "flash_api.h"
#include "log_service.h"
#include "ftl_nand_api.h"
#include "device_lock.h"
#include "sys_api.h"
#include "hal_sys_ctrl.h"
#include "flash_api.h"
#include "ftl_nor_api.h"
#include "freertos_service.h"

nor_info_attr *nor_info = NULL;

int nor_read_cb(int address, unsigned char *buf, int size)
{
	flash_t flash;
	int ret = 0;
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	ret = flash_stream_read(&flash, address, size, buf);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	return ret;
}

int nor_write_cb(int address, unsigned char *buf, int size)
{
	flash_t flash;
	int ret = 0;
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	dcache_clean_invalidate_by_addr((uint32_t *)buf, size);
	ret = flash_stream_write(&flash, address, size, buf);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	return ret;
}

int nor_erase_cb(int address)
{
	flash_t flash;
	int ret = 0;
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_erase_sector(&flash, address);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	return ret;
}

int ftl_write_nor(int address, unsigned char *buf, int size, int offset)
{
	int ret = 0;
	if ((address % 4096) != 0 || (size + offset) > nor_info->page_size) {
		return -1;
	}
	ret = nor_info->nor_read_cb(address, nor_info->pbuf, nor_info->page_size);
	ret = nor_info->nor_erase_cb(address);
	memcpy(nor_info->pbuf + offset, buf, size);
	ret = nor_info->nor_write_cb(address, nor_info->pbuf, nor_info->page_size);
	return ret;
}

int ftl_erase_nor(int address)
{
	int ret = 0;
	if ((address % NOR_FLASH_SECTOR) != 0) {
		return -1;
	}
	ret = nor_info->nor_erase_cb(address);
	return ret;
}

int ftl_read_nor(unsigned int address, unsigned char *buf, int size, int offset)
{
	int ret = 0;
	if ((address % NOR_FLASH_SECTOR) != 0 || (size + offset) > nor_info->page_size) {
		return -1;
	}
	ret = nor_info->nor_read_cb(address, nor_info->pbuf, nor_info->page_size);
	memcpy(buf, nor_info->pbuf + offset, size);
	return ret;
}

int ftl_init_nor(void)
{
	int ret = 0;
	if (nor_info == NULL) {
		nor_info = malloc(sizeof(nor_info_attr));
		if (nor_info == NULL) {
			ret = -1;
			goto EXIT;
		}
		memset(nor_info, 0x00, sizeof(nor_info_attr));
		nor_info->pbuf = malloc(NOR_FLASH_SECTOR);
		if (nor_info->pbuf == NULL) {
			ret = -1;
			goto EXIT;
		}
		nor_info->page_size = NOR_FLASH_SECTOR;
		nor_info->nor_write_cb = nor_write_cb;
		nor_info->nor_read_cb = nor_read_cb;
		nor_info->nor_erase_cb = nor_erase_cb;
		//rtw_mutex_init (&nor_info->ftl_mutex);
	} else {
		//printf("It has already init\r\n");
	}
	return ret;
EXIT:
	if (nor_info) {
		if (nor_info->pbuf) {
			free(nor_info->pbuf);
		}
		free(nor_info);
		nor_info = NULL;
	}
	return ret;
}
#endif