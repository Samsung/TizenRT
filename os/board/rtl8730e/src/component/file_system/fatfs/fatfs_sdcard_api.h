#ifndef _FATFS_SDCARD_API_H
#define _FATFS_SDCARD_API_H
#include "ff.h"

typedef struct fatfs_sd_param_s {
	int drv_num;
	char drv[4];
	FATFS fs;
} fatfs_sd_params_t;

int fatfs_sd_is_inited(void);
int fatfs_sd_init(void);
int fatfs_sd_close(void);
int fatfs_sd_get_param(fatfs_sd_params_t *param);

int fatfs_sd_open_file(char *filename);
int fatfs_sd_close_file(void);
int fatfs_sd_create_write_buf(uint32_t buf_size);
void fatfs_sd_free_write_buf(void);
void fatfs_sd_write(char *buf, uint32_t len);
void fatfs_sd_flush_buf(void);
int fatfs_get_free_space(void);
long long int fatfs_get_free_space_byte(void);
void fatfs_list_files(void);
void sd_reset_procedure(int reason);
void sd_gpio_init(void);
void sd_gpio_deinit(void);
void sd_gpio_power_reset(void);
void sd_gpio_power_off(void);
void sd_gpio_power_on(void);

int usb_sd_init(void);
int usb_sd_deinit(void);
int usb_sd_getcapacity(uint32_t *sector_count);
int usb_sd_readblocks(uint32_t sector, uint8_t *data, uint32_t count);
int usb_sd_writeblocks(uint32_t sector, const uint8_t *data, uint32_t count);

#endif //_FATFS_SDCARD_API_H
