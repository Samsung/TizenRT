#ifndef FTL_NOR_API_H
#define FTL_NOR_API_H
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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned char *pbuf;//To read the page size;
	unsigned int page_size;
	//_mutex ftl_mutex;
	int (*nor_write_cb)(int address, unsigned char *buf, int size);
	int (*nor_read_cb)(int address, unsigned char *buf, int size);
	int (*nor_erase_cb)(int address);
} nor_info_attr;
#define NOR_FLASH_SECTOR 4096
int ftl_init_nor(void);
int ftl_read_nor(unsigned int address, unsigned char *buf, int size, int offset);
int ftl_erase_nor(int address);
int ftl_write_nor(int address, unsigned char *buf, int size, int offset);

#ifdef __cplusplus
}
#endif
#endif
#endif /* FTL_NOR_API_H */