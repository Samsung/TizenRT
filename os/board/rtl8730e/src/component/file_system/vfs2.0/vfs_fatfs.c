#include <stdio.h>
#include "vfs.h"
#include "vfs_fatfs.h"
#include "ff.h"
#include "time.h"
#include "osdep_service.h"

ff_disk_drv  disk = {0};

// return drv_num assigned
int FATFS_RegisterDiskDriver(ll_diskio_drv *drv)
{
	unsigned char drv_num = -1;

	if (disk.nbr < _VOLUMES) {
		drv->drv_num = disk.nbr;	// record driver number for a specific disk
		disk.drv[disk.nbr] = drv;
		disk.nbr++;
		drv_num = drv->drv_num;
	}
	printf("FATFS Register: disk driver %d\n\r", drv_num);
	return drv_num;
}

int FATFS_UnRegisterDiskDriver(unsigned char drv_num)
{
	int index;

	if (disk.nbr >= 1) {
		for (index = 0; index < _VOLUMES; index++) {
			if (disk.drv[index]) {
				if (disk.drv[index]->drv_num == drv_num) {
					disk.drv[index] = 0;
					disk.nbr--;
					return 0;
				}
			}
		}
		return -1; // fail
	}
	return -1; // no disk driver registered
}


/**
  * @brief  Gets number of linked drivers to the FatFs module.
  * @param  None
  * @retval Number of attached drivers.
  */
int FATFS_getDrivernum(const char *TAG)
{
	ll_diskio_drv *drv;
	unsigned int index;

	for (index = 0; index < disk.nbr; index++) {
		drv = disk.drv[index];
		if (!strcmp((char const *)drv->TAG, (char const *)TAG)) {
			return drv->drv_num;
		}
	}
	return -1;
}


int fatfs_get_interface(int interface)
{
	int drv_id = 0;
	if (interface == VFS_INF_SD) {
		drv_id = FATFS_getDrivernum("SD");
	} else if (interface == VFS_INF_RAM) {
		drv_id = FATFS_getDrivernum("RAM");
	} else if (interface == VFS_INF_FLASH) {
		drv_id = FATFS_getDrivernum("FLASH");
	} else {
		return -1;
	}
	return drv_id;
}

int fatfs_open(const char *filename, const char *mode, vfs_file *finfo)
{
	FIL *fil = rtw_malloc(sizeof(FIL));
	uint8_t mode_mapping = 0;
	FRESULT res = FR_OK;
	if (fil == NULL) {
		return -1;
	}

	if (strchr(mode, 'r')) {
		mode_mapping |= FA_READ;
	}
	if (strchr(mode, 'w')) {
		if (strchr(mode, 'x')) {
			mode_mapping |= (FA_CREATE_NEW | FA_WRITE);
		} else {
			mode_mapping |= (FA_CREATE_ALWAYS | FA_WRITE);
		}
	}
	if (strchr(mode, 'a')) {
		mode_mapping |= (FA_OPEN_APPEND | FA_WRITE);
	}
	if (strchr(mode, '+')) {
		mode_mapping |= (FA_READ | FA_WRITE);
	}

	res = f_open(fil, filename, mode_mapping);
	if (res != 0) {
		rtw_free(fil);
		return -1;
	}
	finfo->file = (void *)fil;
	return res;
}

int fatfs_read(unsigned char *buf, unsigned int size, unsigned int count, vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	size_t br;
	FRESULT res = f_read(fil, buf, size * count, (UINT *)&br);
	if (res > 0) {
		return -1;
	}
	return br / size;
}

int fatfs_write(unsigned char *buf, unsigned int size, unsigned int count, vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	size_t bw;
	FRESULT res = f_write(fil, buf, size * count, (UINT *)&bw);
	if (res > 0) {
		return -1;
	}
	return bw / size;
}

int fatfs_close(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	FRESULT res = f_close(fil);
	rtw_free(fil);
	if (res > 0) {
		return -1;
	}
	return 0;
}

int fatfs_seek(long int offset, int origin, vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	int size = f_size(fil);
	int curr = f_tell(fil);
	FRESULT res = FR_INT_ERR;
	switch (origin) {
	case SEEK_SET:
		res = f_lseek(fil, offset);
		break;
	case SEEK_CUR:
		res = f_lseek(fil, curr + offset);
		break;
	case SEEK_END:
		res = f_lseek(fil, size - offset);
		break;
	}
	if (res > 0) {
		return -1;
	}
	return 0;
}

void fatfs_rewind(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	f_lseek(fil, 0);
}

int fatfs_fgetops(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	int value = 0;
	value = f_tell(fil);
	return value;
}

int fatfs_fsetops(unsigned int offset, vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	int value = 0;
	value = f_lseek(fil, offset);
	return value;
}

int fatfs_fflush(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	FRESULT res = f_sync(fil);
	return -res;
}

int fatfs_remove(const char *name)
{
	FRESULT res = f_unlink(name);
	return -res;
}

int fatfs_rename(const char *old_name, const char *new_name)
{
	FRESULT res = f_rename(old_name, new_name);
	return -res;
}

int fatfs_feof(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	return f_eof(fil);
}

int fatfs_ferror(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	return f_error(fil);
}

int fatfs_ftell(vfs_file *finfo)
{
	FIL *fil = (FIL *)finfo->file;
	return f_tell(fil);
}

int fatfs_ftruncate(vfs_file *finfo, off_t length)
{
	FIL *fil = (FIL *)finfo->file;
	FRESULT res = FR_INT_ERR;
	res = f_lseek(fil, length);
	if (res > 0) {
		return -1;
	}
	res = f_truncate(fil);
	if (res > 0) {
		return -1;
	}
	return 0;
}

int fatfs_opendir(const char *name, vfs_file *finfo)
{
	DIR *pdir = rtw_malloc(sizeof(DIR));
	FRESULT res = FR_OK;
	if (pdir == NULL) {
		return -1;
	}
	finfo->file = (void *)pdir;
	res = f_opendir(pdir, name);
	return res;
}

struct dirent *fatfs_readdir(vfs_file *finfo)
{
	DIR *pdir = (DIR *)finfo->file;
	FRESULT res;
	char *fn;
	if (pdir == NULL) {
		return NULL;
	}
	struct dirent *ent = rtw_malloc(sizeof(struct dirent));
	if (ent == NULL) {
		return NULL;
	}
	FILINFO m_fileinfo;

	res = f_readdir(pdir, &m_fileinfo);
	if (res != FR_OK) {
		rtw_free(ent);
		VFS_DBG(VFS_ERROR, "readdir: error (%d)", res);
		return NULL;
	}

	if (m_fileinfo.fname[0] == 0) {
		rtw_free(ent);
		return NULL;
	}

	fn = m_fileinfo.fname;
	ent->d_ino = 0;
	ent->d_off = 0;
	ent->d_reclen = m_fileinfo.fsize;
	if (m_fileinfo.fattrib & AM_DIR) {
		ent->d_type = DT_DIR;    // directory
	} else {
		ent->d_type = DT_REG;    // regular file
	}

	fn = m_fileinfo.fname;
	sprintf(ent->d_name, "%s", fn);
	return ent;
}

int fatfs_closedir(vfs_file *finfo)
{
	DIR *pdir = (DIR *)finfo->file;
	FRESULT res = f_closedir(pdir);
	rtw_free(pdir);
	if (res > 0) {
		return -1;
	} else {
		return 0;
	}
}

int fatfs_mkdir(const char *pathname)
{
	FRESULT res = f_mkdir(pathname);
	return -res;
}

int fatfs_rmdir(const char *path)
{
	FRESULT res = f_unlink(path);
	return -res;
}

int fatfs_access(const char *pathname, int mode)
{
	FRESULT res;
	FILINFO finfo;

	res = f_stat(pathname, &finfo);

	VFS_DBG(VFS_INFO, "%s, check 0x%x", pathname, mode);

	if (res == FR_NO_FILE) {
		return -1;
	}



	if (mode & VFS_R_OK) {
		return 0;
	}

	if (mode & VFS_W_OK) {
		if (finfo.fattrib & AM_RDO) {
			return -1;
		} else {
			return 0;
		}
	}

	if (mode & VFS_X_OK) {
		if (finfo.fattrib & AM_RDO) {
			return -1;
		} else {
			return 0;
		}
	}
	return 0;
}

int fatfs_stat(char *path, struct stat *buf)
{
	FILINFO finfo;

	FRESULT res = f_stat(path, &finfo);
	if (res == FR_NO_FILE) {
		return -1;
	}

	buf->st_dev = 0;
	buf->st_ino = 0;

	mode_t st_mode = 0;
	if (finfo.fattrib & AM_DIR) {
		if (finfo.fattrib & AM_RDO) {
			st_mode |= (S_IFDIR | 0x555);
		} else {
			st_mode |= (S_IFDIR | 0x777);
		}
	} else {
		if (finfo.fattrib & AM_RDO) {
			st_mode |= (S_IFREG | 0x555);
		} else {
			st_mode |= (S_IFREG | 0x777);
		}
	}

	buf->st_mode = st_mode;
	buf->st_nlink = 0;
	buf->st_uid = 0;
	buf->st_gid = 0;
	buf->st_rdev = 0;
	buf->st_size = finfo.fsize;
	buf->st_blksize = 0;
	buf->st_blocks = 0;

	struct tm tm  = {0};
	int year = (finfo.fdate >> 9) + 1980;
	int month = (finfo.fdate >> 5) & 15;
	int day = finfo.fdate & 31;
	int hour = finfo.ftime >> 11;
	int minute = (finfo.ftime >> 5) & 63;
	char timestr[512];
	snprintf(timestr, sizeof(timestr), "%d/%d/%d %d:%d", year, month, day, hour, minute);

	tm.tm_year = year - 1900; /* years since 1900 */
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;

	time_t t = mktime(&tm);

	buf->st_atime = t;
	buf->st_mtime = t;
	buf->st_ctime = t;
	return 0;
}

int fatfs_mount(int interface)
{
	int ret = 0;
	if (interface == VFS_INF_SD) {
		VFS_DBG(VFS_INFO, "sd mount");
#if FATFS_DISK_SD
		ret = fatfs_sd_init();
#endif
	} else if (interface == VFS_INF_FLASH) {
		VFS_DBG(VFS_INFO, "flash mount");
#if FATFS_DISK_FLASH
		ret = fatfs_flash_init();
#endif
	} else {
		VFS_DBG(VFS_ERROR, "It don't support the interface %d", interface);
		return -1;
	}
	return ret;
}

int fatfs_ummount(int interface)
{
	int ret = 0;
	if (interface == VFS_INF_SD) {
		VFS_DBG(VFS_INFO, "sd unmount");
#if FATFS_DISK_SD
		ret = fatfs_sd_close();
#endif
	} else if (interface == VFS_INF_FLASH) {
		VFS_DBG(VFS_INFO, "flash unmount");
#if FATFS_DISK_FLASH
		ret = fatfs_flash_close();
#endif
	} else {
		VFS_DBG(VFS_ERROR, "It don't support the interface %d", interface);
		return -1;
	}
	return ret;
}

vfs_opt fatfs_drv = {
	.open = fatfs_open,
	.read = fatfs_read,
	.write = fatfs_write,
	.close = fatfs_close,
	.seek  = fatfs_seek,
	.rewind = fatfs_rewind,
	.fgetpos = fatfs_fgetops,
	.fsetpos = fatfs_fsetops,
	.fflush = fatfs_fflush,
	.remove = fatfs_remove,
	.rename = fatfs_rename,
	.eof   = fatfs_feof,
	.error = fatfs_ferror, //ferror
	.tell  = fatfs_ftell,
	.ftruncate = fatfs_ftruncate,
	.opendir = fatfs_opendir,
	.readdir = fatfs_readdir,
	.closedir = fatfs_closedir,
	.mkdir = fatfs_mkdir,
	.rmdir = fatfs_rmdir,
	.stat = fatfs_stat,
	.access = fatfs_access,
	.mount = fatfs_mount,
	.unmount = fatfs_ummount,
	.get_interface = fatfs_get_interface,
	.TAG	= "fatfs"
};
