#include "platform_opts.h"
#include "vfs.h"
#include "vfs_fatfs.h"
#if FATFS_DISK_SD
#include "platform_stdlib.h"
#include "ff.h"
#include "ameba_sd.h"

static fatfs_sd_params_t fatfs_sd_param;
static uint8_t fatfs_sd_init_done = 0;
static FIL     fatfs_sd_file;
static unsigned char *fatfs_sd_buf = NULL;
static uint32_t fatfs_sd_buf_size;
static uint32_t fatfs_sd_buf_pos;

int fatfs_sd_is_inited(void)
{
	return fatfs_sd_init_done;
}

int fatfs_sd_close(void)
{
	if (fatfs_sd_init_done) {
		if (f_mount(NULL, fatfs_sd_param.drv, 1) != FR_OK) {
			printf("FATFS unmount logical drive fail.\n");
		}

		if (FATFS_UnRegisterDiskDriver(fatfs_sd_param.drv_num)) {
			printf("Unregister disk driver from FATFS fail.\n");
		}

		//sdio_deinit_host(psdioh_adapter);
		//deinit_combine(psdioh_adapter);
		fatfs_sd_init_done = 0;
	}
	return 0;
}

int fatfs_sd_init(void)
{
	int ret = 0;

	if (!fatfs_sd_init_done) {
		fatfs_sd_init_done = 1;
		int Fatfs_ok = 0;
		FRESULT res;
		// Register disk driver to Fatfs
		printf("Register disk driver to Fatfs.\n\r");
		fatfs_sd_param.drv_num = FATFS_RegisterDiskDriver(&SD_disk_Driver);

		if (fatfs_sd_param.drv_num < 0) {
			printf("Rigester disk driver to FATFS fail.\n\r");
		} else {
			Fatfs_ok = 1;
			fatfs_sd_param.drv[0] = fatfs_sd_param.drv_num + '0';
			fatfs_sd_param.drv[1] = ':';
			fatfs_sd_param.drv[2] = '/';
			fatfs_sd_param.drv[3] = 0;
		}
		if (!Fatfs_ok) {
			ret = -1;
			goto fatfs_init_err;
		}

		res = f_mount(&fatfs_sd_param.fs, fatfs_sd_param.drv, 1);
		if (res) {
			printf("f_mount error here, please insert SD card or use f_mkfs to format SD card to FAT32");
			//res = f_mkfs(fatfs_sd_param.drv,0,0);
			if (f_mount(&fatfs_sd_param.fs, fatfs_sd_param.drv, 0) != FR_OK) {
				printf("FATFS mount logical drive on sd card fail.\n\r");
				ret = -2;
				goto fatfs_init_err;
			} else {
				fatfs_sd_init_done = 1;
			}
		} else {
			fatfs_sd_init_done = 1;
		}
	} else {

	}
	return 0;

fatfs_init_err:
	fatfs_sd_close();
	return ret;
}

int fatfs_sd_get_param(fatfs_sd_params_t *param)
{
	if (fatfs_sd_init_done)	{
		memcpy(param, &fatfs_sd_param, sizeof(fatfs_sd_params_t));
		return 0;
	} else {
		memset(param, 0, sizeof(fatfs_sd_params_t));
		return -1;
	}
}

int fatfs_sd_open_file(char *filename)
{
	if (fatfs_sd_init_done)	{
		int res;
		char path[64];

		strcpy(path, fatfs_sd_param.drv);
		sprintf(&path[strlen(path)], "%s", filename);

		res = f_open(&fatfs_sd_file, path, FA_OPEN_ALWAYS | FA_WRITE);
		if (res) {
			printf("open file (%s) fail. res = %d\n\r", filename, res);
			return -1;
		}
		return 0;
	} else {
		return -2;
	}
}

int fatfs_sd_close_file(void)
{
	int res;
	res = f_close(&fatfs_sd_file);
	if (res) {
		printf("close file fail.\n\r");
		return -1;
	}

	return 0;
}

void fatfs_sd_write(char *buf, uint32_t len)
{
	int res = 0;
	uint32_t bw;
	int offset = 0;

	//printf("fatfs_sd_write length= %d\n\r",len);

	while (len > 0) {
		if (fatfs_sd_buf_pos + len >= fatfs_sd_buf_size) {
			memcpy(fatfs_sd_buf + fatfs_sd_buf_pos, buf + offset, fatfs_sd_buf_size - fatfs_sd_buf_pos);

			res = f_write(&fatfs_sd_file, fatfs_sd_buf, fatfs_sd_buf_size, &bw);
			if (res) {
				printf("Write error (%d)\n\r", res);
				f_lseek(&fatfs_sd_file, 0);
			}
			//printf("Write %d bytes.\n\r", bw);
			//vTaskDelay(1);
			offset += fatfs_sd_buf_size - fatfs_sd_buf_pos;
			len -= fatfs_sd_buf_size - fatfs_sd_buf_pos;
			fatfs_sd_buf_pos = 0;
		} else {
			memcpy(fatfs_sd_buf + fatfs_sd_buf_pos, buf + offset, len);
			fatfs_sd_buf_pos = fatfs_sd_buf_pos + len;
			len = 0;
		}
	}
}

void fatfs_sd_flush_buf(void)
{
	int res = 0;
	if (fatfs_sd_buf_pos != 0) {
		uint32_t bw;
		//printf("flush %d bytes before close file\n\r",fatfs_sd_buf_pos);
		res = f_write(&fatfs_sd_file, fatfs_sd_buf, fatfs_sd_buf_pos, &bw);
		if (res) {
			printf("Write error (%d)\n\r", res);
			f_lseek(&fatfs_sd_file, 0);
		}
		fatfs_sd_buf_pos = 0;
	}
}

void fatfs_sd_free_write_buf(void)
{
	if (fatfs_sd_buf) {
		free(fatfs_sd_buf);
	}
}

int fatfs_sd_create_write_buf(uint32_t buf_size)
{
	if (buf_size == 0) {
		printf("ERROR: buf_size can't be 0\n\r");
		return -1;
	}
	fatfs_sd_free_write_buf();
	fatfs_sd_buf = (unsigned char *)malloc(buf_size);
	if (fatfs_sd_buf == NULL) {
		printf("allocate fatfs_sd_buf fail\r\n");
		return -2;
	}
	memset(fatfs_sd_buf, 0, buf_size);
	fatfs_sd_buf_size = buf_size;
	fatfs_sd_buf_pos = 0;
	return 0;
}

FRESULT scan_files(
	char *path        /* Start node to be scanned (also used as work area) */
)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	char *fn;   /* This function is assuming non-Unicode cfg. */
	char cur_path[512] = {0};
	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		sprintf(cur_path, path);
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) {
				break;    /* Break on error or end of dir */
			}
			if (fno.fname[0] == '.') {
				continue;    /* Ignore dot entry */
			}

			fn = fno.fname;
			if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				sprintf(&cur_path[strlen(path)], "/%s", fn);
				scan_files(cur_path);
			} else {                                       /* It is a file. */
				printf("%s/%s\r\n", path, fn);
			}
		}
	}
	f_closedir(&dir);
	return res;
}

int fatfs_get_free_space(void)
{
	FATFS *pfs = NULL;
	DWORD fre_clust;
	unsigned char  res;
	float sector_size = 0;
	float free_size = 0;
	sector_size = 512;

	if (fatfs_sd_init_done) {
		res = f_getfree(fatfs_sd_param.drv, &fre_clust, &pfs);
		if (res) {
			return res;
		} else {
			//TOT_SIZE = (pfs->n_fatent - 2) * pfs->csize/2; //
			free_size = (fre_clust * pfs->csize) * (sector_size / 1024 / 1024); //
		}
	}

	return (int)free_size;//MB
}

long long int fatfs_get_free_space_byte(void)
{
	FATFS *pfs = NULL;
	DWORD fre_clust;
	long long int fre_clust_temp, csize_temp;
	long long int free_size = 0;
	unsigned char  res;
	long long int sector_size = 0;

	sector_size = 512;

	if (fatfs_sd_init_done) {
		res = f_getfree(fatfs_sd_param.drv, &fre_clust, &pfs);
		if (res) {
			return res;
		} else {
			//TOT_SIZE = (pfs->n_fatent - 2) * pfs->csize/2; //
			fre_clust_temp = fre_clust;
			csize_temp     = pfs->csize;
			free_size = (fre_clust_temp * csize_temp) * sector_size; //*(sector_size); //
		}
	}

	return free_size;//Byte
}

void fatfs_list_files(void)
{
	char buff[256] = {0};
	char	logical_drv[4] = {0}; /* root diretor */
	logical_drv[0] = fatfs_sd_param.drv_num + '0';
	logical_drv[1] = ':';
	logical_drv[2] = '/';
	logical_drv[3] = 0;
	if (fatfs_sd_init_done) {
		strcpy(buff, logical_drv);
		scan_files(buff);
	}
}

/*For usb operation.........................................*/
int usb_sd_init(void)
{
	SD_RESULT ret;
	ret = SD_Init();
	if (ret != SD_OK) {
		return 1;
	}
	return ret;
}
int usb_sd_deinit(void)
{
	SD_RESULT ret;
	ret = SD_DeInit();
	if (ret != SD_OK) {
		return 1;
	}
	return ret;
}
int usb_sd_getcapacity(uint32_t *sector_count)
{
	SD_RESULT ret;
	ret = SD_GetCapacity(sector_count);
	if (ret != SD_OK) {
		return 1;
	}
	return 0;
}
int usb_sd_readblocks(uint32_t sector, uint8_t *data, uint32_t count)
{
	SD_RESULT ret;
	ret = SD_ReadBlocks(sector, data, count);
	if (ret != SD_OK) {
		return 1;
	}
	return 0;
}
int usb_sd_writeblocks(uint32_t sector, const uint8_t *data, uint32_t count)
{
	SD_RESULT ret;
	ret = SD_WriteBlocks(sector, data, count);
	if (ret != SD_OK) {
		return 1;
	}
	return 0;
}
/*For usb operation.........................................*/
#endif //FATFS_DISK_SD