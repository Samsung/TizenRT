/*
 *  Routines to associate Flash driver with FatFs
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "integer.h"
#include "stdint.h"
#include <disk_if/inc/flash_fatfs.h>
#include "device_lock.h"
#include "osdep_service.h"
#include "platform_opts.h"

#if defined(FATFS_DISK_FLASH) && (FATFS_DISK_FLASH == 1U)

#include "flash_api.h" // Flash interface

#define FLASH_BLOCK_SIZE	512		// not passing any
#define SECTOR_SIZE_FLASH	512
#define SECTOR_NUM 8

#ifndef FLASH_APP_BASE
u32 FLASH_APP_BASE;
#endif
u32 FLASH_SECTOR_COUNT;

flash_t		flash;

DRESULT interpret_flash_result(int out)
{
	DRESULT res;
	if (out) {
		res = RES_OK;
	} else {
		res = RES_ERROR;
	}
	return res;
}

DSTATUS FLASH_disk_status(void)
{
	DRESULT res;
	res = RES_OK;
	return res;
}

DSTATUS FLASH_disk_initialize(void)
{
	DRESULT res;
#if defined(CONFIG_PLATFORM_8735B)
	//spic_deinit(&flash.hal_spic_adaptor);
	flash_init(&flash);
#endif
	res = RES_OK;
	return res;
}

DSTATUS FLASH_disk_deinitialize(void)
{
	DRESULT res;
	res = RES_OK;
	return res;
}
#if defined(SUPPORT_USB_FLASH_MASSSTORAGE)
#include "ftl_common_api.h"
#ifdef FLASH_SECTOR_COUNT
#undef FLASH_SECTOR_COUNT
#define FLASH_SECTOR_COUNT 2048 //512*2048
#endif
#ifdef SECTOR_SIZE_FLASH
#undef SECTOR_SIZE_FLASH
#define SECTOR_SIZE_FLASH 512 //The usb mass storage need to setup 512 byte.(TODO support other sector size)
#endif
/* Read sector(s) --------------------------------------------*/
DRESULT FLASH_disk_read(BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = 0;
	ftl_common_read(FLASH_APP_BASE + sector * SECTOR_SIZE_FLASH, buff, count * SECTOR_SIZE_FLASH);
	return res;
}
/* Write sector(s) --------------------------------------------*/
DRESULT FLASH_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = 0;
	ftl_common_write(FLASH_APP_BASE + sector * SECTOR_SIZE_FLASH, buff, count * SECTOR_SIZE_FLASH);
	return res;
}
#else
/* Read sector(s) --------------------------------------------*/
DRESULT FLASH_disk_read(BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res;
	char retry_cnt = 0;
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	do {
		res = interpret_flash_result(flash_stream_read(&flash, FLASH_APP_BASE + sector * SECTOR_SIZE_FLASH, count * SECTOR_SIZE_FLASH, (uint8_t *) buff));
		if (++retry_cnt >= 3) {
			break;
		}
	} while (res != RES_OK);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	return res;
}

/* Write sector(s) --------------------------------------------*/
#if _USE_WRITE == 1
DRESULT FLASH_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = RES_OK;
	u8 sector_index = sector % SECTOR_NUM;
	u8 *flash_sector_buffer = (u8 *)rtw_malloc(4096);

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	//deal with fisrt flash sector
	flash_stream_read(&flash, FLASH_APP_BASE + (sector / SECTOR_NUM) * 4096, 4096, flash_sector_buffer);
	rtw_memcpy(flash_sector_buffer + (sector_index * SECTOR_SIZE_FLASH), (BYTE *)buff,
			   ((count + sector_index <= SECTOR_NUM) ? count : (u32)(SECTOR_NUM - sector_index))*SECTOR_SIZE_FLASH);
	flash_erase_sector(&flash, FLASH_APP_BASE + (sector / SECTOR_NUM) * 4096);
	flash_stream_write(&flash, FLASH_APP_BASE + (sector / SECTOR_NUM) * 4096, 4096, flash_sector_buffer);
	rtw_free(flash_sector_buffer);
	flash_sector_buffer = NULL;
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	return res;
}
#endif
#endif
/* IOCTL sector(s) --------------------------------------------*/
#if _USE_IOCTL == 1
DRESULT FLASH_disk_ioctl(BYTE cmd, void *buff)
{
	DRESULT res = RES_ERROR;
	// FLASH_RESULT result;

	switch (cmd) {
	/* Generic command (used by FatFs) */

	/* Make sure that no pending write process in the physical drive */
	case CTRL_SYNC:		/* Flush disk cache (for write functions) */
		res = RES_OK;
		break;
	case GET_SECTOR_COUNT:	/* Get media size (for only f_mkfs()) */
		*(DWORD *)buff = FLASH_SECTOR_COUNT;
		res = RES_OK;
		break;
	/* for case _MAX_SS != _MIN_SS */
	case GET_SECTOR_SIZE:	/* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
		*(WORD *)buff = SECTOR_SIZE_FLASH;	//4096 or 2048 or 1024
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:	/* Get erase block size (for only f_mkfs()) */
		*(DWORD *)buff = FLASH_BLOCK_SIZE;
		res = RES_ERROR;
		break;
	case CTRL_ERASE_SECTOR:/* Force erased a block of sectors (for only _USE_ERASE) */
		res = RES_OK;
		break;
#if 0
	/* MMC/SDC specific ioctl command */

	case MMC_GET_TYPE:	/* Get card type */
		res = RES_OK;
		break;
	case MMC_GET_CSD:	/* Get CSD */
		res = RES_OK;
		break;
	case MMC_GET_CID:	/* Get CID */
		res = RES_OK;
		break;
	case MMC_GET_OCR:	/* Get OCR */
		res = RES_OK;
		break;
	case MMC_GET_SDSTAT:/* Get SD status */
		res = RES_OK;
		break;
#endif
	default:
		res = RES_PARERR;
		break;
	}
	return res;
}
#endif

ll_diskio_drv FLASH_disk_Driver = {
	.disk_initialize = FLASH_disk_initialize,
	.disk_status = FLASH_disk_status,
	.disk_read = FLASH_disk_read,
	.disk_deinitialize = FLASH_disk_deinitialize,
#if _USE_WRITE == 1
	.disk_write = FLASH_disk_write,
#endif
#if _USE_IOCTL == 1
	.disk_ioctl = FLASH_disk_ioctl,
#endif
	.TAG	= (unsigned char *)"FLASH"
};
#endif