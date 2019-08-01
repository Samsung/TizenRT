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

///
/// This sample provides the way to create cloud sample
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "cacommon.h"
#include "cacommonutil.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "logger.h"
#include "mbedtls/ssl_ciphersuites.h"

#include "sss_storage_server.h"
#include "sss_storage.h"

#include "utils/things_malloc.h"
#include "logging/things_logger.h"

#define SSSFILE  "/sss/00014280"

int g_secureStorageType;

int getSecureStorageType(void)
{
	return g_secureStorageType;
}

void setSecureStorageType(int type)
{
	g_secureStorageType = type;
}

//#define LOGSECURE(data) printf("%s:%d|%s() SSS[%s] data:%d\n",__FILE__,__LINE__,__func__,getSecureStorageType() == 0 ? "PERSISTENT" : "MEMORY", data)
#define LOGSECURE(data)

/** Persistent storage open handler.*/
FILE *secure_fopen(const char *path, const char *mode)
{
	LOGSECURE(0);
	if (getSecureStorageType() == 0)
		return sss_open(path, mode);

	return sss_open_mem(path, mode);
}

/** Persistent storage read handler.*/
size_t secure_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	LOGSECURE(nmemb);
	size_t count = 0;

	if (getSecureStorageType() == 0) {
		count = sss_read(ptr, size, nmemb, stream);
		LOGSECURE(count);
		return count;
	}
	count = sss_read_mem(ptr, size, nmemb, stream);
	LOGSECURE(count);
	return count;
}

/** Persistent storage write handler.*/
size_t secure_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	LOGSECURE(nmemb);
	if (getSecureStorageType() == 0)
		return sss_write(ptr, size, nmemb, stream);

	return sss_write_mem(ptr, size, nmemb, stream);
}

void secure_sync(void)
{
	LOGSECURE(0);
	sss_sync();
}

/** Persistent storage close handler.*/
int secure_fclose(FILE *fp)
{
	return sss_close(fp);
}
/** Persistent storage unlink handler.*/
int secure_funlink(const char *path)
{
	return sss_unlink(path);
}

/** Persistent secure_remove handler.*/
extern int secure_remove(const char *path)
{
	char *fullpath;
	int ret = -1;

	fullpath = get_fullpath(SSSFILE);

	usleep(500 * 1000);

	if (fullpath) {
		ret = unlink(fullpath);
		if (ret < 0) {
			printf("unlink fail [%d]", ret);
		}
		free(fullpath);
		printf("File [%s] Clear Success", SSSFILE);
	} else {
		printf("delete fail");
	}

	return ret;
}
