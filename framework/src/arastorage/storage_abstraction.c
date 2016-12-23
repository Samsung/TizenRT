/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
*****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
#include <sys/stat.h>
#endif
#include "db_debug.h"
#include "storage.h"

/****************************************************************************
* Public Functions
****************************************************************************/
/**
 * \Description
 *      The below functions are defined for storage abstraction.
 *              So, you just change function to each one in specific file system that you want to use.
 */

/* It mapped with open function in specific file system */
db_storage_id_t storage_open(const char *filename, int oflag)
{
	db_storage_id_t fd;
	char *rel_path;
	rel_path = (char *)malloc(sizeof(char) * (strlen(CONFIG_MOUNT_POINT) + strlen(filename) + 1));
	if (rel_path == NULL) {
		return INVALID_STORAGE_ID;
	}
	snprintf(rel_path, DB_MAX_FILENAME_LENGTH, "%s%s\0", CONFIG_MOUNT_POINT, filename);
	fd = open(rel_path, oflag);
	free(rel_path);
	return fd;
}

/* It mapped with close function in specific file system */
db_storage_id_t storage_close(db_storage_id_t fd)
{
	return close(fd);
}

/* It mapped with unlink function in specific file system */
db_result_t storage_remove(const char *filename)
{
	char *rel_path;
	int res = DB_STORAGE_ERROR;
	rel_path = (char *)malloc(sizeof(char) * (strlen(CONFIG_MOUNT_POINT) + strlen(filename) + 1));
	if (rel_path == NULL) {
		return DB_STORAGE_ERROR;
	}
	snprintf(rel_path, DB_MAX_FILENAME_LENGTH, "%s%s\0", CONFIG_MOUNT_POINT, filename);
	if (unlink(rel_path) == OK) {
		res = DB_OK;
	}
	free(rel_path);
	return res;
}

/* It mapped with rename function in specific file system */
db_result_t storage_rename(const char *old_name, const char *new_name)
{
	char *old_path;
	char *new_path;
	int res = DB_STORAGE_ERROR;
	old_path = (char *)malloc(sizeof(char) * (strlen(CONFIG_MOUNT_POINT) + strlen(old_name) + 1));
	if (old_path == NULL) {
		return DB_STORAGE_ERROR;
	}

	new_path = (char *)malloc(sizeof(char) * (strlen(CONFIG_MOUNT_POINT) + strlen(new_name) + 1));
	if (new_path == NULL) {
		free(old_path);
		return DB_STORAGE_ERROR;
	}

	snprintf(old_path, DB_MAX_FILENAME_LENGTH, "%s%s\0", CONFIG_MOUNT_POINT, old_name);
	snprintf(new_path, DB_MAX_FILENAME_LENGTH, "%s%s\0", CONFIG_MOUNT_POINT, new_name);

	if (rename(old_path, new_path) == OK) {
		res = DB_OK;
	}
	free(old_path);
	free(new_path);
	return res;
}

/* It mapped with seek function in specific file system */
off_t storage_seek(db_storage_id_t fd, unsigned long offset, int whence)
{
	return lseek(fd, offset, whence);
}

/* It mapped with read function in specific file system */
ssize_t storage_read(db_storage_id_t fd, void *buffer, unsigned length)
{
	return read(fd, buffer, length);
}

/* It mapped with write function in specific file system */
ssize_t storage_write(db_storage_id_t fd, void *buffer, unsigned length)
{
	return write(fd, buffer, length);
}

#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
ssize_t storage_get_availbyte_size(void)
{
	struct stat st;
	if (stat(CONFIG_MOUNT_POINT, &st) != OK) {
		return 0;
	}
	return st.st_blksize;
}
#endif
