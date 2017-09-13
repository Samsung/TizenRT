//******************************************************************
//
// Copyright 2014 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// @file tls/sss_storage.c
/// @brief sss api is supporting security api for using secure storage.


#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "tls/see_api.h"
#include "tls/sss_storage.h"

unsigned char *sss_p;
static sss_file_ctrl sss_fs;

FILE *sss_open(const char * filename, const char * mode)
{
	int ret;
	unsigned int flen;

	if (!strcmp(mode, "r") || !strcmp(mode, "rb")) {	
		ret = see_read_iotivity_storage(sss_fs.data, SEE_IOTIVITY_MAXSIZE, &flen);
		if (ret != 0) {
			printf("see_open error : %d\n", ret);
			return NULL;
		}
		sss_p = sss_fs.data;
		sss_fs.size = flen;
	}
	printf("[pkss] see open success\n");
	return (FILE *)sss_fs.data;
}

size_t sss_read(void * ptr, size_t size, size_t count, FILE * stream)
{
	unsigned int read_size = 0;

	if (ptr == NULL || stream == NULL || count == 0)
		return 0;

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

	return read_size;
}

size_t sss_write(const void *ptr, size_t size, size_t count, FILE *stream)
{
	int ret;

	if (ptr == NULL || stream == NULL || size == 0 || count == 0)
		return SSS_WRITE_ERR;

	ret = see_write_iotivity_storage((unsigned char *)ptr, count);
	if (ret != 0) {
		printf("sss_write fail : %d\n", ret);
		return SSS_WRITE_ERR;
	}

	return count;
}

int sss_close(FILE *fp)
{
	if (fp == 0)
	       return SSS_CLOSE_ERR;

	return 0;
}

int sss_unlink(const char *path)
{
	return 0;
}
