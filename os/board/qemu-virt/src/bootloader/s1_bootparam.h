/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#ifndef __BOOTPARAM_H__
#define __BOOTPARAM_H__

#include <stdint.h>
#include <tinyara/config.h>

#define BOOTPARAM_PART_SIZE 8192
#define BOOTPARAM_COUNT 2
#define BIN_NAME_MAX 16


#ifdef CONFIG_APP_BINARY_SEPARATION
/* User bootparam data */
struct userbp_s {
	char name[BIN_NAME_MAX];
	uint8_t useidx;
};
typedef struct userbp_s userbp_t;
#endif

struct bpdata_s {
	uint32_t crc_hash;
	uint32_t version;
	uint32_t format_ver;
	uint8_t active_idx;
	uint32_t kernel_address[BOOTPARAM_COUNT];
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint8_t app_count;
	userbp_t app_data[CONFIG_NUM_APPS + 1];
#endif
	uint8_t resource_active_idx;
} __attribute__((__packed__));
typedef struct bpdata_s bpdata_t;

#endif //__BOOTPARAM_H__