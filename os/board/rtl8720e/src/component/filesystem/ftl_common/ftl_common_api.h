#ifndef FTL_COMMON_API_H
#define FTL_COMMON_API_H
#include "platform_opts.h"
#if CONFIG_FTL_EN
#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal.h"
#include "log_service.h"
#include "flash_api.h"
#include "log_service.h"
#include "ftl_nand_api.h"
#include "device_lock.h"
#include "sys_api.h"
#include "hal_sys_ctrl.h"
#include "flash_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FTL_NOR_FLASH    0X00
#define FTL_NAND_FLASH   0X01
#define NOR_FLASH_SECTOR 4096

int ftl_common_read(unsigned int address, unsigned char *buf, unsigned int size);
int ftl_common_write(unsigned int address, unsigned char *buf, unsigned int size);
int ftl_common_erase(unsigned int address);
int ftl_common_info(int *type, int *page_size, int *block_size, int *block_cnt);
int ftl_erase_sector(unsigned int address);
#ifdef __cplusplus
}
#endif
#endif
#endif /* FTL_COMMON_API_H */