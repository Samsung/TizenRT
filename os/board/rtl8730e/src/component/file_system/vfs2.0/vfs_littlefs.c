#include <stdio.h>
#include "time.h"
#include "os_wrapper.h"
// #include "FreeRTOS.h"
#include <cmsis.h>
// #include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "section_config.h"

#include "lfs_util.h"
#include "vfs.h"
#include "lfs.h"

extern lfs_t g_lfs;
extern int rt_lfs_init(lfs_t *lfs);
char lfs_mount_fail = 0;

int fmodeflags(const char *mode)
{
	int flags;
	if (strchr(mode, '+')) {
		flags = VFS_O_RDWR;
	} else if (*mode == 'r') {
		flags = VFS_O_RDONLY;
	} else {
		flags = VFS_O_WRONLY;
	}
	if (strchr(mode, 'x')) {
		flags |= VFS_O_EXCL;
	}
	if (strchr(mode, 'e')) {
		flags |= VFS_O_CLOEXEC;
	}
	if (*mode != 'r') {
		flags |= VFS_O_CREAT;
	}
	if (*mode == 'w') {
		flags |= VFS_O_TRUNC;
	}
	if (*mode == 'a') {
		flags |= VFS_O_APPEND;
	}
	return flags;
}

int littlefs_open(const char *filename, const char *mode, vfs_file *finfo)
{
	int ret = 0;
	int flags = fmodeflags(mode);
	int mode_flag = 0;
	lfs_file_t *file = rtw_malloc(sizeof(lfs_file_t));
	if (file == NULL) {
		return -1;
	}

	if ((flags & 3) == VFS_O_RDONLY) {
		mode_flag |= LFS_O_RDONLY;
	}
	if ((flags & 3) == VFS_O_WRONLY) {
		mode_flag |= LFS_O_WRONLY;
	}
	if ((flags & 3) == VFS_O_RDWR) {
		mode_flag |= LFS_O_RDWR;
	}
	if (flags & VFS_O_CREAT) {
		mode_flag |= LFS_O_CREAT;
	}
	if (flags & VFS_O_EXCL) {
		mode_flag |= LFS_O_EXCL;
	}
	if (flags & VFS_O_TRUNC) {
		mode_flag |= LFS_O_TRUNC;
	}
	if (flags & VFS_O_APPEND) {
		mode_flag |= LFS_O_APPEND;
	}

	ret = lfs_file_open(&g_lfs, file, filename, mode_flag);
	if (ret < 0) {
		rtw_free(file);
		return ret;
	}
	VFS_DBG(VFS_INFO, "Open file mode 0x%x", mode_flag);
	finfo->file = (void *)file;
	return ret;
}

int littlefs_read(unsigned char *buf, unsigned int size, unsigned int count, vfs_file *finfo)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	ret = lfs_file_read(&g_lfs, file, buf, size * count);
	if (ret < 0) {
		return ret;
	}
	return ret / size;
}

int littlefs_write(unsigned char *buf, unsigned int size, unsigned int count, vfs_file *finfo)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	ret = lfs_file_write(&g_lfs, file, buf, size * count);
	if (ret < 0) {
		return ret;
	}
	return ret / size;
}

int littlefs_close(vfs_file *finfo)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	ret = lfs_file_close(&g_lfs, file);
	if (file) {
		rtw_free(file);
	}
	return ret;
}

int littlefs_seek(long int offset, int origin, vfs_file *finfo)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	switch (origin) {
	case SEEK_SET:
		ret = lfs_file_seek(&g_lfs, file, offset, LFS_SEEK_SET);
		break;
	case SEEK_CUR:
		ret = lfs_file_seek(&g_lfs, file, offset, LFS_SEEK_CUR);
		break;
	case SEEK_END:
		ret = lfs_file_seek(&g_lfs, file, offset, LFS_SEEK_END);
		break;
	}
	return ret;
}

void littlefs_rewind(vfs_file *finfo)
{
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	lfs_file_rewind(&g_lfs, file);
}

int littlefs_fgetops(vfs_file *finfo)
{
	int location = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	location = lfs_file_tell(&g_lfs, file);
	return location;
}

int littlefs_fsetops(unsigned int offset, vfs_file *finfo)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	ret = lfs_file_seek(&g_lfs, file, offset, LFS_SEEK_SET);
	return ret;
}

int littlefs_fflush(vfs_file *finfo)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	ret = lfs_file_sync(&g_lfs, file);
	return ret;
}

int littlefs_remove(const char *name)
{
	int ret = 0;
	ret = lfs_remove(&g_lfs, name);
	return ret;
}

int littlefs_rename(const char *old_name, const char *new_name)
{
	int ret = 0;
	ret = lfs_rename(&g_lfs, old_name, new_name);
	return ret;
}

int littlefs_feof(vfs_file *finfo)
{
	int location = 0;//lfs_file_size
	int size = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	location = lfs_file_tell(&g_lfs, file);
	size = lfs_file_size(&g_lfs, file);
	if ((size - location) <= 0) {
		VFS_DBG(VFS_INFO, "End of file");
		return 1;
	} else {
		VFS_DBG(VFS_INFO, "Remain %d", size - location);
		return 0;
	}
}

int littlefs_ferror(vfs_file *finfo)
{
	(void) finfo;
	return 0;
}

int littlefs_ftell(vfs_file *finfo)
{
	int location = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	location = lfs_file_tell(&g_lfs, file);
	return location;
}

int littlefs_ftruncate(vfs_file *finfo, off_t length)
{
	int ret = 0;
	lfs_file_t *file = (lfs_file_t *)finfo->file;
	ret = lfs_file_truncate(&g_lfs, file, length);
	if (ret < 0) {
		return -1;
	}
	return 0;
}

int littlefs_opendir(const char *name, vfs_file *finfo)
{
	lfs_dir_t *dir = rtw_malloc(sizeof(lfs_dir_t));
	if (dir == NULL) {
		return -1;
	}
	rtw_memset(dir, 0, sizeof(lfs_dir_t));

	int err = lfs_dir_open(&g_lfs, dir, name);
	if (err) {
		rtw_free(dir);
		return err;
	}
	finfo->file = (void *)dir;
	return err;
}

struct dirent *littlefs_readdir(vfs_file *finfo)
{
	lfs_dir_t *dir = (lfs_dir_t *)finfo->file;
	struct lfs_info info;
	struct dirent *ent = rtw_malloc(sizeof(struct dirent));
	if (ent == NULL) {
		return NULL;
	}
	rtw_memset(ent, 0, sizeof(struct dirent));
	int err = lfs_dir_read(&g_lfs, dir, &info);
	if (err <= 0) {
		return NULL;
	}
	if (info.name[0] == 0) {
		return NULL;
	}
	ent->d_ino = 0;
	ent->d_off = 0;
	ent->d_reclen = info.size;
	if (info.type == LFS_TYPE_DIR) {
		ent->d_type = DT_DIR;
	} else {
		ent->d_type = DT_REG;
	}
	sprintf(ent->d_name, "%s", info.name);
	return ent;
}

int littlefs_closedir(vfs_file *finfo)
{
	int ret = 0;
	lfs_dir_t *dir = (lfs_dir_t *)finfo->file;
	ret = lfs_dir_close(&g_lfs, dir);
	rtw_free(dir);
	if (ret < 0) {
		VFS_DBG(VFS_ERROR, "Close directory fail: %d", ret);
	}
	return ret;
}

int littlefs_mkdir(const char *pathname)
{
	int ret = 0;
	ret = lfs_mkdir(&g_lfs, pathname);
	return ret;
}

int littlefs_rmdir(const char *path)
{
	int ret = 0;
	ret = lfs_remove(&g_lfs, path);
	return ret;
}

int littlefs_access(const char *pathname, int mode)
{
	(void) mode;
	struct lfs_info info;
	int ret = 0;
	ret = lfs_stat(&g_lfs, pathname, &info);
	if (ret < 0) {
		return -1;
	} else {
		return ret;
	}
}

int littlefs_stat(char *path, struct stat *buf)
{
	struct lfs_info info;
	int ret = 0;
	ret = lfs_stat(&g_lfs, path, &info);
	if (ret < 0) {
		return -1;
	}
	buf->st_dev = 0;
	buf->st_ino = 0;
	mode_t st_mode = 0;
	if (info.type == LFS_TYPE_DIR) {
		st_mode |= (S_IFDIR | 0x777);
	} else {
		st_mode |= (S_IFREG | 0x777);
	}

	buf->st_mode = st_mode;
	buf->st_nlink = 0;
	buf->st_uid = 0;
	buf->st_gid = 0;
	buf->st_rdev = 0;
	buf->st_size = info.size;
	buf->st_blksize = 0;
	buf->st_blocks = 0;
	buf->st_atime = 0;
	buf->st_mtime = 0;
	buf->st_ctime = 0;
	return ret;
}

int littlefs_mount(int interface)
{
	(void) interface;
	int ret = 0;
	ret = rt_lfs_init(&g_lfs);
	if (ret) {
		VFS_DBG(VFS_ERROR, "Littlefs mount fail, ret is %d", ret);
		return ret;
	}
	VFS_DBG(VFS_INFO, "Littlefs mount");
	return ret;
}

int littlefs_unmount(int interface)
{
	(void) interface;
	int ret = 0;
	ret = lfs_unmount(&g_lfs);
	VFS_DBG(VFS_INFO, "Littlefs unmount");
	return ret;
}

vfs_opt littlefs_drv = {
	.open = littlefs_open,
	.read = littlefs_read,
	.write = littlefs_write,
	.close = littlefs_close,
	.seek  = littlefs_seek,
	.rewind = littlefs_rewind,
	.fgetpos = littlefs_fgetops,
	.fsetpos = littlefs_fsetops,
	.fflush = littlefs_fflush,
	.remove = littlefs_remove,
	.rename = littlefs_rename,
	.eof   = littlefs_feof,
	.error = littlefs_ferror, //ferror
	.tell  = littlefs_ftell,
	.ftruncate = littlefs_ftruncate,
	.opendir = littlefs_opendir,
	.readdir = littlefs_readdir,
	.closedir = littlefs_closedir,
	.mkdir = littlefs_mkdir,
	.rmdir = littlefs_rmdir,
	.stat = littlefs_stat,
	.access = littlefs_access,
	.mount = littlefs_mount,
	.unmount = littlefs_unmount,
	.TAG	= "littlefs"
};