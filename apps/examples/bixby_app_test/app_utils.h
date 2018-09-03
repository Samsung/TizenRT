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
#ifndef __EXTERNAL_INCLUDE_BIXBY_APPS_APP_UTILS_H
#define __EXTERNAL_INCLUDE_BIXBY_APPS_APP_UTILS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdbool.h>

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#define FAIL_OUT_OF_MEMORY -1
#define FAIL_INVALID_PARAM -2
/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
int get_tm_handle_by_appid(const char *app_id);
int get_appid_by_tm_handle(int handle, char **app_id);

typedef int (*APP_ENTRY)(int argc, char *argv[]);
struct app_s {
	const char *app_id;
	const APP_ENTRY entry;
	const int stack_size;
	const int priority;
	const bool is_on_boot;
	int handle;
};
typedef struct app_s app_t;

extern app_t bixby_app_list[];
#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __EXTERNAL_INCLUDE_BIXBY_APPS_APP_UTILS_H */
