/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http;//www.apache.org/licenses/LICENSE-2.0
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

#include "things_malloc.h"
#include "things_logger.h"

//pkssss #include "mbedtls/sss_storage.h"
//#include "mbedtls/sss_storage.h"

/** Persistent storage open handler.*/
FILE *secure_fopen(const char *path, const char *mode)
{
	return NULL;
	//return sss_open(path,mode);
}

/** Persistent storage read handler.*/
size_t secure_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return NULL;
	//return sss_read(ptr, size, nmemb, stream);
}

/** Persistent storage write handler.*/
size_t secure_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return NULL;
	//return sss_write(ptr, size, nmemb, stream);
}

/** Persistent storage close handler.*/
int secure_fclose(FILE *fp)
{
	return 0;
	//return sss_close(fp);
}

/** Persistent storage unlink handler.*/
int secure_funlink(const char *path)
{
	return 0;
	//return sss_unlink(path);
}

/** Persistent secure_remove handler.*/
extern int secure_remove(const char *path)
{
#if 0
#define SSSFILE1  "/sss/00013180"
#define SSSFILE2  "/sss/00014280"
	char *fullpath;
	fullpath = get_fullpath(SSSFILE1);

	things_task_sleep(500);
	int ret = -1;
	if (fullpath) {
		ret = unlink(fullpath);
		if (ret < 0) {
			printf("unlink fail [%d]\n");
		}
		free(fullpath);
		printf("File [%s] Clear Success \n", SSSFILE1);
	} else {
		printf("delete fail\n");
	}

	fullpath = get_fullpath(SSSFILE2);

	things_task_sleep(500);

	if (fullpath) {
		ret = unlink(fullpath);
		if (ret < 0) {
			printf("unlink fail [%d]\n");
		}
		free(fullpath);
		printf("File [%s] Clear Success \n", SSSFILE2);
	} else {
		printf("delete fail\n");
	}
	return ret;
#endif
	return 0;
}
