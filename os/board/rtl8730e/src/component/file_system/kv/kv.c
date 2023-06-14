/////////////////////////////////////////////////
//
// kv , key-value pair
//
/////////////////////////////////////////////////

#include "kv.h"
#include "vfs.h"
#include "littlefs_adapter.h"

extern FILE *fopen(const char *filename, const char *mode);
extern int fclose(FILE *stream);
extern size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
extern size_t fread(void *ptr, size_t size, size_t count, FILE *stream);


static char *prefix;
int rt_kv_init(void)
{
	int ret = -1;
	char path[MAX_KEY_LENGTH + 1] = {0};
	if (lfs_mount_fail) {
		FS_DBG(FS_ERROR, "KV init fail");
		return ret;
	}

	prefix = find_vfs1_tag();
	if (prefix == NULL) {
		return ret;
	}

	sprintf(path, "%s:KV", prefix);
	ret = mkdir(path, 0);
	if (ret == LFS_ERR_EXIST) {
		FS_DBG(FS_INFO, "KV dir already exist");
		ret = 0;
	}

	return ret;
}


int32_t rt_kv_set(const char *key, const void *val, int32_t len)
{
	vfs_file *finfo;
	int res = -1;
	char path[MAX_KEY_LENGTH + 2] = {0};

	if (lfs_mount_fail) {
		FS_DBG(FS_ERROR, "KV init fail");
		return res;
	}

	if (strlen(key) > MAX_KEY_LENGTH - 3) {
		FS_DBG(FS_ERROR, "key len limit exceed, max len is %d", MAX_KEY_LENGTH - 3);
		return res;
	}

	if (prefix == NULL) {
		return res;
	}

	sprintf(path, "%s:KV/%s", prefix, key);
	finfo = (vfs_file *)fopen(path, "w");
	if (finfo == NULL) {
		FS_DBG(FS_ERROR, "fopen failed, res is %d", res);
		return res;
	}

	res = fwrite(val, len, 1, (FILE *)finfo);
	if (res != 1) {
		FS_DBG(FS_ERROR, "fwrite failed,err is %d!!", res);
	}

	fclose((FILE *)finfo);
	return res;
}

int32_t rt_kv_get(const char *key, void *buffer, int32_t len)
{
	vfs_file *finfo;
	int res = -1;
	char path[MAX_KEY_LENGTH + 2] = {0};

	if (lfs_mount_fail) {
		FS_DBG(FS_ERROR, "KV init fail");
		return res;
	}

	if (strlen(key) > MAX_KEY_LENGTH - 3) {
		FS_DBG(FS_ERROR, "key len limite exceed, max len is %d", MAX_KEY_LENGTH - 3);
		return res;
	}

	if (prefix == NULL) {
		return res;
	}

	sprintf(path, "%s:KV/%s", prefix, key);
	finfo = (vfs_file *)fopen(path, "r");
	if (finfo == NULL) {
		FS_DBG(FS_ERROR, "fopen failed, res = %d", res);
		return res;
	}

	res = fread(buffer, len, 1, (FILE *)finfo);
	if (res < 0) {
		FS_DBG(FS_ERROR, "fread failed,err is %d!!!", res);
	}
	fclose((FILE *)finfo);
	return res;
}

int32_t rt_kv_delete(const char *key)
{
	int res = -1;
	char path[MAX_KEY_LENGTH + 2] = {0};

	if (lfs_mount_fail) {
		FS_DBG(FS_INFO, "KV init fail");
		return res;
	}

	if (strlen(key) > MAX_KEY_LENGTH - 3) {
		FS_DBG(FS_ERROR, "key len limite exceed, max len is %d", MAX_KEY_LENGTH - 3);
		return res;
	}

	if (prefix == NULL) {
		return res;
	}

	sprintf(path, "%s:KV/%s", prefix, key);
	res = remove(path);
	return res;
}
