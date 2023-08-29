#ifndef _VFS_FATFS_DEVICE_H
#define _VFS_FATFS_DEVICE_H

#include "ff.h"
#include "diskio.h"

typedef struct {
	DSTATUS(*disk_initialize)(void);                       /*!< Initialize Disk Drive                     */
	DSTATUS(*disk_deinitialize)(void);                       /*!< Deinitialize Disk Drive                     */
	DSTATUS(*disk_status)(void);                           /*!< Get Disk Status                           */
	DRESULT(*disk_read)(BYTE *, DWORD, UINT);              /*!< Read Sector(s)                            */
#if _USE_WRITE == 1
	DRESULT(*disk_write)(const BYTE *, DWORD, UINT);       /*!< Write Sector(s) when _USE_WRITE = 0       */
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
	DRESULT(*disk_ioctl)(BYTE, void *);                    /*!< I/O control operation when _USE_IOCTL = 1 */
#endif /* _USE_IOCTL == 1 */
	unsigned char	*TAG;
	unsigned char drv_num;
} ll_diskio_drv;

typedef struct {
	ll_diskio_drv	*drv[_VOLUMES];
	unsigned int            nbr;
} ff_disk_drv;

extern ff_disk_drv  disk;
extern ll_diskio_drv SD_disk_Driver;
extern ll_diskio_drv FLASH_disk_Driver;

typedef struct fatfs_flash_param_s {
	int drv_num;
	char drv[4];
	FATFS fs;
} fatfs_flash_params_t;

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

int fatfs_flash_init(void);
int fatfs_flash_close(void);
int fatfs_flash_get_param(fatfs_flash_params_t *param);
int usb_flash_init(void);
int usb_flash_deinit(void);
int usb_flash_getcapacity(uint32_t *sector_count);
int usb_flash_readblocks(uint32_t sector, uint8_t *data, uint32_t count);
int usb_flash_writeblocks(uint32_t sector, const uint8_t *data, uint32_t count);

int FATFS_RegisterDiskDriver(ll_diskio_drv *disk_drv);
int FATFS_UnRegisterDiskDriver(unsigned char drv_num);
int FATFS_getDrivernum(const char *TAG);

#define FATFS_DISK_USB	                0
#define FATFS_DISK_SD 	                0
#define FATFS_DISK_FLASH 	            1

#endif