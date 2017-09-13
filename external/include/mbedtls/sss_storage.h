/**
 * @defgroup SECURITY SECURITY
 * @ingroup SECURITY
 * @{
 */

/**
 * @file sss_storage.h
 * @brief Security API header file which is using SSS.
 *
 */

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
/// @file see/see_api.h
/// @brief SEE api is supporting security api for using secure storage.

#ifndef __SSS_STORAGE_H
#define __SSS_STORAGE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include "mbedtls/see_api.h"

typedef struct sss_file_control
{
	unsigned char data[SEE_IOTIVITY_MAXSIZE];
	unsigned int size;
} sss_file_ctrl;

FILE *sss_open(const char * filename, const char * mode);
size_t sss_read(void * ptr, size_t size, size_t count, FILE * stream);
size_t sss_write(const void *ptr, size_t size, size_t count, FILE *stream);
int sss_close(FILE *fp);
int sss_unlink(const char *path);

#define SSS_WRITE_ERR	0
#define SSS_CLOSE_ERR	1

#endif /* __SSS_STORAGE_H */
