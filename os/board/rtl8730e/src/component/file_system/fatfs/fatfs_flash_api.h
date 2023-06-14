#ifndef _FATFS_FLASH_API_H
#define _FATFS_FLASH_API_H
#include "ff.h"

typedef struct fatfs_flash_param_s {
	int drv_num;
	char drv[4];
	FATFS fs;
} fatfs_flash_params_t;

int fatfs_flash_init(void);
int fatfs_flash_close(void);
int fatfs_flash_get_param(fatfs_flash_params_t *param);
int usb_flash_init(void);
int usb_flash_deinit(void);
int usb_flash_getcapacity(uint32_t *sector_count);
int usb_flash_readblocks(uint32_t sector, uint8_t *data, uint32_t count);
int usb_flash_writeblocks(uint32_t sector, const uint8_t *data, uint32_t count);
#endif //_FATFS_FLASH_API_H