/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_PREFERENCE_H
#define __INCLUDE_TINYARA_PREFERENCE_H

/* This file will be used to provide definitions to support
 * preference framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MOUNT_PATH "/mnt"

#define PREF_PATH MOUNT_PATH"/pref"
#define PREF_PRIVATE_PATH PREF_PATH"/private"
#define PREF_SHARED_PATH PREF_PATH"/shared"

enum preference_type_e {
	PREFERENCE_TYPE_INT = 0,
	PREFERENCE_TYPE_DOUBLE,
	PREFERENCE_TYPE_BOOL,
	PREFERENCE_TYPE_STRING,
	PREFERENCE_TYPE_BINARY,
};

/* Error Type of Result Value returned from Preference */
enum preference_result_error_e {
	PREFERENCE_IO_ERROR = -1,
	PREFERENCE_INVALID_PARAMETER = -2,
	PREFERENCE_PATH_NOT_FOUND = -3,
	PREFERENCE_KEY_NOT_EXIST = -4,
	PREFERENCE_OUT_OF_MEMORY = -5,
	PREFERENCE_OPERATION_FAIL = -6,
	PREFERENCE_NOT_REGISTERED = -7,
	PREFERENCE_INVALID_DATA = -8,
	PREFERENCE_NOT_SUPPORTED = -9,
};

#define PRIVATE_PREFERENCE     0
#define SHARED_PREFERENCE      1
	
/* Wrapper of allocation APIs */
#define PREFERENCE_ALLOC(a)  malloc(a)
#define PREFERENCE_FREE(a)   free(a)
#define PREFERENCE_ASPRINTF(p, f, ...) asprintf(p, f, ##__VA_ARGS__)

/* Preference Callback Message Queue */
#define PREFERENCE_CBMSG_MQ        "preference_cb"
#define PREFERENCE_CBMQ_LEN         16
#define PREFERNENCE_CBMSG_MAX       32

struct value_attr_s {
	uint32_t crc;
	int type;
	int len;
};
typedef struct value_attr_s value_attr_t;

struct preference_data_s {
	const char *key;
	int type;
	value_attr_t attr;
	void *value;
};
typedef struct preference_data_s preference_data_t;

typedef void (*preference_changed_cb)(char *key, void *cb_data);

struct preference_callback_s {
	char *key;
	int type;
	preference_changed_cb cb_func;
	void *cb_data;
};
typedef struct preference_callback_s preference_callback_t;

int preference_init(void);

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_PREFERENCE_H */
