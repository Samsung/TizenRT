#ifndef _FLASH_FATFS_H_
#define _FLASH_FATFS_H_
#include "fatfs_ext/inc/ff_driver.h"
#if defined(CONFIG_PLATFORM_8735B)
//#define SUPPORT_USB_FLASH_MASSSTORAGE //Enable the marco to support flash mass storage
#endif
extern ll_diskio_drv FLASH_disk_Driver;
#endif
