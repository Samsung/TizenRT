/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 ****************************************************************************/

/// @file tls/sss_storage.c
/// @brief SSS storage is supporting secure storage file operations for iotivity.

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <security/security_api.h>
#include "sss_storage.h"

#define SSSFILE  "/sss/00014280"

static unsigned char *sss_p;
static sss_file_ctrl sss_fs;

static unsigned char *sss_p_mem;
static sss_file_ctrl sss_fs_mem;

static size_t g_sss_size = 0;

FILE *sss_open(const char* filename, const char* mode);

FILE *sss_open_mem(const char *filename, const char *mode)
{

	if (!strcmp(mode, "r") || !strcmp(mode, "rb")) {
		if (g_sss_size == 0) {
			(void *)sss_open(filename, mode);
		}

		sss_p_mem = sss_fs_mem.data;
		sss_fs_mem.size = g_sss_size;
	}

	return (FILE *) sss_fs_mem.data;
}

size_t sss_read_mem(void *ptr, size_t size, size_t count, FILE *stream)
{

	unsigned int read_size = 0;

	if (ptr == NULL || stream == NULL || count == 0) {
		return 0;
	}

	if (sss_fs_mem.size == 0) {
		sss_p_mem = sss_fs_mem.data;
	} else if (sss_fs_mem.size > count) {
		memcpy(ptr, sss_p_mem, count);
		sss_fs_mem.size = sss_fs_mem.size - count;
		sss_p_mem += count;
		read_size = count;
	} else {
		memcpy(ptr, sss_p_mem, sss_fs_mem.size);
		read_size = sss_fs_mem.size;
		sss_fs_mem.size = 0;
	}

	return read_size;
}

size_t sss_write_mem(const void *ptr, size_t size, size_t count, FILE *stream)
{

	memcpy(sss_fs_mem.data, ptr, count);
	sss_fs_mem.size = count;
	g_sss_size = count;

	return count;
}

int sss_sync(void)
{

	memcpy(sss_fs.data, sss_fs_mem.data, g_sss_size);

	sss_p = sss_fs.data;
	sss_fs.size = g_sss_size;

	// security_handle hnd;
	// security_error res = security_init(&hnd);
	// if (res != SECURITY_OK) {
	// 	free(hnd);
	// 	return NULL;
	// }

	// security_data input = {sss_fs.data, sss_fs.size};
	// // res = auth_set_certificate(hnd, IOTIVITY_SS, &input);
	// res = ss_write_secure_storage(hnd, IOTIVITY_SS, 0, &input);
	// if (res != SECURITY_OK) {
	// 	printf("setSecureStorageType sss_sync fail : %d\n", res);
	// 	security_deinit(hnd);
	// 	return -1;
	// }
	// security_deinit(hnd);

	FILE *fp = fopen(SSSFILE, "w");
	if (fp == NULL) {
		return 0;
	}
	sss_fs.size = fwrite(sss_fs.data, 1, sss_fs.size, fp);
	fclose(fp);


	return 0;
}

FILE *sss_open(const char *filename, const char *mode)
{
	printf("[DEBUG] %s:%d>>%s IN\n", __FILE__, __LINE__, __FUNCTION__);
	int ret;

	if (!strcmp(mode, "r") || !strcmp(mode, "rb")) {
		// security_handle hnd;
		// security_error res = security_init(&hnd);
		// if (res != SECURITY_OK) {
		// 	return NULL;
		// }

		// security_data data = {sss_fs.data, IOTIVITY_MAX_BUF_SIZE};
		// res = ss_read_secure_storage(hnd, IOTIVITY_SS, 0, &data);
		// // res = auth_get_certificate(hnd, filename, &data);
		// if (res != SECURITY_OK) {
		// 	(void)security_deinit(hnd);
		// 	return NULL;
		// }

		FILE *fp = fopen(SSSFILE, mode);
		if (fp == NULL) {
			return NULL;
		}

		sss_fs.size = fread(sss_fs.data, 1, IOTIVITY_MAX_BUF_SIZE, fp);
		fclose(fp);

		sss_p = sss_fs.data;
		// sss_fs.size = data.length;

		// g_sss_size = data.length;
		g_sss_size = sss_fs.size;
		memcpy(sss_fs_mem.data, sss_fs.data, g_sss_size);

		// (void)security_deinit(hnd);
	}

	printf("[DEBUG] %s:%d>>%s OUT\n", __FILE__, __LINE__, __FUNCTION__);
	return (FILE *)sss_fs.data;
}

size_t sss_read(void *ptr, size_t size, size_t count, FILE *stream)
{
	printf("[DEBUG] %s:%d>>%s IN\n", __FILE__, __LINE__, __FUNCTION__);
	unsigned int read_size = 0;

	if (ptr == NULL || stream == NULL || count == 0) {
		return 0;
	}

	if (sss_fs.size == 0) {
		sss_p = sss_fs.data;
	} else if (sss_fs.size > count) {
		memcpy(ptr, sss_p, count);
		sss_fs.size = sss_fs.size - count;
		sss_p += count;
		read_size = count;
	} else {
		memcpy(ptr, sss_p, sss_fs.size);
		read_size = sss_fs.size;
		sss_fs.size = 0;
	}
	printf("[DEBUG] %s:%d>>%s OUT\n", __FILE__, __LINE__, __FUNCTION__);
	return read_size;
}

size_t sss_write(const void *ptr, size_t size, size_t count, FILE *stream)
{
	printf("[DEBUG] %s:%d>>%s IN\n", __FILE__, __LINE__, __FUNCTION__);
	if (ptr == NULL || stream == NULL || size == 0 || count == 0) {
		return SSS_WRITE_ERR;
	}

	// security_handle hnd;
	// security_error res = security_init(&hnd);
	// if (res != SECURITY_OK) {
	// 	return SSS_WRITE_ERR;
	// }
	// security_data data = {ptr, count};

	// res = ss_write_secure_storage(hnd, IOTIVITY_SS, 0, &data);
	// // res = auth_set_certificate(hnd, IOTIVITY_SS, &data);
	// if (res != SECURITY_OK) {
	// 	printf("[ERROR] %s:%d\n", __FILE__, __LINE__);
	// 	security_deinit(hnd);
	// 	return SSS_WRITE_ERR;
	// }
	// security_deinit(hnd);

	FILE *fp = fopen(SSSFILE, "w");
	if (fp != NULL) {
		sss_fs.size = fwrite(ptr, 1, count, fp);
	}
	fclose(fp);

	g_sss_size = count;
	memcpy(sss_fs_mem.data, ptr, g_sss_size);

	printf("[DEBUG] %s:%d>>%s OUT\n", __FILE__, __LINE__, __FUNCTION__);
	return count;
}

int sss_close(FILE *fp)
{
	printf("[DEBUG] %s:%d>>%s IN\n", __FILE__, __LINE__, __FUNCTION__);
	if (fp == 0) {
		return SSS_CLOSE_ERR;
	}

	printf("[DEBUG] %s:%d>>%s OUT\n", __FILE__, __LINE__, __FUNCTION__);
	return 0;
}

int sss_unlink(const char *path)
{
	return 0;
}
