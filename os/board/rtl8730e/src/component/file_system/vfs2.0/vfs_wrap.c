#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"
#include "vfs.h"

/**************************************************
* FILE api wrap for compiler
*
**************************************************/
/*
--redirect fopen=__wrap_fopen
--redirect fclose=__wrap_fclose
--redirect fread=__wrap_fread
--redirect fwrite=__wrap_fwrite
--redirect fseek=__wrap_fseek
--redirect fsetpos=__wrap_fsetpos
--redirect fgetpos=__wrap_fgetpos
--redirect rewind=__wrap_rewind
--redirect fflush=__wrap_fflush
--redirect remove=__wrap_remove
--redirect rename=__wrap_rename
--redirect feof=__wrap_feof
--redirect ferror=__wrap_ferror
--redirect ftell=__wrap_ftell
--redirect fputc=__wrap_fputc
--redirect fputs=__wrap_fputs
--redirect fgets=__wrap_fgets
*/

int alphasort(const struct dirent **a, const struct dirent **b)
{
	return strcoll((*a)->d_name, (*b)->d_name);
}

static int is_stdio(FILE *stream)
{
#ifndef __ICCARM__
	if (stream == stdout || stream == stderr || stream == stdin) {
		return 1;
	}
#endif
	return 0;
}

FILE *__wrap_fopen(const char *filename, const char *mode)
{
	if (lfs_mount_fail) {
		VFS_DBG(FS_ERROR, "vfs init fail, fopen is not allowed");
		return NULL;
	}

	int prefix_len = 0;
	int drv_id = 0;
	int ret = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(filename, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return NULL;
	}

	if (vfs.user[user_id].vfs_ro_flag && (strchr(mode, 'w') || strchr(mode, '+') || strchr(mode, 'a'))) {
		VFS_DBG(VFS_WARNING, "Vfs is read-only when initializing!");
		return NULL;
	}

	vfs_file *finfo = (vfs_file *)malloc(sizeof(vfs_file));
	if (finfo == NULL) {
		return NULL;
	}
	memset(finfo, 0x00, sizeof(vfs_file));
	finfo->vfs_id = vfs_id;
	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(finfo->name, sizeof(finfo->name), "%s%s", temp, filename + prefix_len);
	} else {
		snprintf(finfo->name, sizeof(finfo->name), "%s", filename + prefix_len);
	}

	ret = vfs.drv[vfs_id]->open(finfo->name, mode, finfo);
	if (ret < 0) {
		free(finfo);
		finfo = NULL;
	}
	return (FILE *)finfo;
}

int __wrap_fclose(FILE *stream)
{
	int ret = 0;

	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return 0;
	}

	ret = vfs.drv[finfo->vfs_id]->close((vfs_file *)stream);
	free(finfo);
	return ret;
}

size_t __wrap_fread(void *ptr, size_t size, size_t count, FILE *stream)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;

	if (is_stdio(stream)) {
		return 0;
	}

	ret = vfs.drv[finfo->vfs_id]->read(ptr, size, count, (vfs_file *)stream);
	return ret;
}

extern size_t _write(int file, const void *ptr, size_t len);
size_t __wrap_fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
#ifndef  __ICCARM__
	if (stream == stdout) {
		return _write(1, ptr, size * count);
	}
	if (stream == stderr) {
		return _write(2, ptr, size * count);
	}
	if (stream == stdin) {
		return 0;
	}
#endif
	ret = vfs.drv[finfo->vfs_id]->write((void *)ptr, size, count, (vfs_file *)stream);
	return ret;
}

int  __wrap_fseek(FILE *stream, long int offset, int origin)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return 0;
	}

	ret = vfs.drv[finfo->vfs_id]->seek(offset, origin, (vfs_file *)stream);
	return ret;
}

void  __wrap_rewind(FILE *stream)
{
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return;
	}
	vfs.drv[finfo->vfs_id]->rewind((vfs_file *)stream);
}

int __wrap_fgetpos(FILE *stream, fpos_t   *p)
{
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return 0;
	}
#if defined(__ICCARM__)
	p->_Off = vfs.drv[finfo->vfs_id]->fgetpos((vfs_file *)stream);
#elif defined(__GNUC__)
	*p = vfs.drv[finfo->vfs_id]->fgetpos((vfs_file *)stream);
#endif
	return 0;
}

int __wrap_fsetpos(FILE *stream, fpos_t   *p)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return 0;
	}
#if defined(__ICCARM__)
	ret = vfs.drv[finfo->vfs_id]->fsetpos(p->_Off, (vfs_file *)stream);
#elif defined(__GNUC__)
	ret = vfs.drv[finfo->vfs_id]->fsetpos((unsigned int) * p, (vfs_file *)stream);
#endif
	return ret;
}

//extern int _fflush_r(struct _reent *ptr, FILE *fp);
int  __wrap_fflush(FILE *stream)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
#ifndef __ICCARM__
	if (is_stdio(stream)) {
		_fflush_r(_REENT, stream);
		return 0;
	}
#endif
	ret = vfs.drv[finfo->vfs_id]->fflush((vfs_file *)stream);
	return ret;
}

int __wrap_remove(const char *filename)
{
	int ret = 0;
	char name[1024] = {0};
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(filename, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return -1;
	}

	if (vfs.user[user_id].vfs_ro_flag) {
		VFS_DBG(VFS_WARNING, "Vfs is read-only when initializing!");
		return -1;
	}

	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(name, sizeof(name), "%s%s", temp, filename + prefix_len);
	} else {
		snprintf(name, sizeof(name), "%s", filename + prefix_len);
	}

	ret = vfs.drv[vfs_id]->remove(name);
	return ret;
}

int __wrap_rename(const char *oldname, const char *newname)
{
	int ret = 0;
	char old_name[1024] = {0};
	char new_name[1024] = {0};
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(oldname, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return -1;
	}

	if (vfs.user[user_id].vfs_ro_flag) {
		VFS_DBG(VFS_WARNING, "Vfs is read-only when initializing!");
		return -1;
	}

	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(old_name, sizeof(old_name), "%s%s", temp, oldname + prefix_len);
		snprintf(new_name, sizeof(new_name), "%s%s", temp, newname + prefix_len);
	} else {
		snprintf(old_name, sizeof(old_name), "%s", oldname + prefix_len);
		snprintf(new_name, sizeof(new_name), "%s", newname + prefix_len);
	}

	ret = vfs.drv[vfs_id]->rename(old_name, new_name);
	return ret;
}

int __wrap_feof(FILE *stream)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return 0;
	}
	ret = vfs.drv[finfo->vfs_id]->eof((vfs_file *)stream);
	return ret;
}

int __wrap_ferror(FILE *stream)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return 0;
	}
	ret = vfs.drv[finfo->vfs_id]->error((vfs_file *)stream);
	return ret;
}

long int __wrap_ftell(FILE *stream)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return -1;
	}
	ret = vfs.drv[finfo->vfs_id]->tell((vfs_file *)stream);
	return ret;
}

long int __wrap_ftruncate(FILE *stream, off_t length)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)stream;
	if (is_stdio(stream)) {
		return -1;
	}

	ret = vfs.drv[finfo->vfs_id]->ftruncate((vfs_file *)stream, length);
	return ret;
}

int __wrap_fputc(int character, FILE *stream)
{
	(void) character;
	(void) stream;
	//vfs don't implement
	return -1;
}

int __wrap_fputs(const char *str, FILE *stream)
{
	(void) str;
	(void) stream;
	//vfs don't implement
	return -1;
}

char *__wrap_fgets(char *str, int num, FILE *stream)
{
	(void) str;
	(void) num;
	if (is_stdio(stream)) {
		return NULL;
	}

	//vfs don't implement
	return NULL;
}

DIR *__wrap_opendir(const char *name)
{
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(name, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return NULL;
	}
	vfs_file *finfo = (vfs_file *)malloc(sizeof(vfs_file));
	if (finfo == NULL) {
		return NULL;
	}
	memset(finfo, 0x00, sizeof(vfs_file));
	finfo->vfs_id = vfs_id;
	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(finfo->name, sizeof(finfo->name), "%s%s", temp, name + prefix_len);
	} else {
		snprintf(finfo->name, sizeof(finfo->name), "%s", name + prefix_len);
	}

	vfs.drv[vfs_id]->opendir(finfo->name, finfo);
	return (DIR *)finfo;
}

struct dirent *__wrap_readdir(DIR *pdir)
{
	struct dirent *ent = NULL;
	vfs_file *finfo = (vfs_file *)pdir;
	ent = vfs.drv[finfo->vfs_id]->readdir(((vfs_file *)pdir));
	return ent;
}

int __wrap_closedir(DIR *dirp)
{
	int ret = 0;
	vfs_file *finfo = (vfs_file *)dirp;
	ret = vfs.drv[finfo->vfs_id]->closedir(((vfs_file *)dirp));
	free(finfo);
	return ret;
}

int __wrap_scandir(const char *dirp, struct dirent ***namelist,
				   int (*filter)(const struct dirent *),
				   int (*compar)(const struct dirent **, const struct dirent **))
{
	(void) dirp;
	(void) filter;
	(void) compar;
	(void) namelist;
	return -1;
}

int __wrap_rmdir(const char *path)
{
	int ret = 0;
	char name[1024] = {0};
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(path, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return -1;
	}

	if (vfs.user[user_id].vfs_ro_flag) {
		VFS_DBG(VFS_WARNING, "Vfs is read-only when initializing!");
		return -1;
	}

	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(name, sizeof(name), "%s%s", temp, path + prefix_len);
	} else {
		snprintf(name, sizeof(name), "%s", path + prefix_len);
	}

	ret = vfs.drv[vfs_id]->rmdir(name);
	return ret;
}

int __wrap_mkdir(const char *pathname, mode_t mode)
{
	(void) mode;
	int ret = 0;
	char name[1024] = {0};
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(pathname, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return -1;
	}

	if (vfs.user[user_id].vfs_ro_flag) {
		VFS_DBG(VFS_WARNING, "Vfs is read-only when initializing!");
		return -1;
	}

	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(name, sizeof(name), "%s%s", temp, pathname + prefix_len);
	} else {
		snprintf(name, sizeof(name), "%s", pathname + prefix_len);
	}

	ret = vfs.drv[vfs_id]->mkdir(name);
	return ret;
}

int __wrap_access(const char *pathname, int mode)
{
	int ret = 0;
	char name[1024] = {0};
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(pathname, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return -1;
	}
	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(name, sizeof(name), "%s%s", temp, pathname + prefix_len);
	} else {
		snprintf(name, sizeof(name), "%s", pathname + prefix_len);
	}

	ret = vfs.drv[vfs_id]->access(name, mode);
	return ret;
}

int __wrap_stat(const char *path, struct stat *buf)
{
	int ret = 0;
	char name[1024] = {0};
	int prefix_len = 0;
	int user_id = 0;
	int vfs_id = find_vfs_number(path, &prefix_len, &user_id);
	if (vfs_id < 0) {
		VFS_DBG(VFS_ERROR, "It can't find the file system");
		return -1;
	}
	if (vfs.drv[vfs_id]->vfs_type == VFS_FATFS) {
		int drv_id = 0;
		drv_id = vfs.drv[vfs_id]->get_interface(vfs.user[user_id].vfs_interface_type);
		char temp[4] = {0};
		temp[0] = drv_id + '0';
		temp[1] = ':';
		temp[2] = '/';
		snprintf(name, sizeof(name), "%s%s", temp, path + prefix_len);
	} else {
		snprintf(name, sizeof(name), "%s", path + prefix_len);
	}

	ret = vfs.drv[vfs_id]->stat(name, buf);
	return ret;
}
