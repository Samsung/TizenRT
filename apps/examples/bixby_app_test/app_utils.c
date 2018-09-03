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
#include <string.h>
#include <stdlib.h>
#include <bixby_apps/app_utils.h>
extern app_t bixby_app_list[];

int get_tm_handle_by_appid(const char *app_id)
{
	int app_idx;
	for (app_idx = 0; bixby_app_list[app_idx].app_id != NULL; app_idx++) {
		if (strncmp(bixby_app_list[app_idx].app_id, app_id, strlen(app_id) + 1) == 0) {
			return bixby_app_list[app_idx].handle;
		}
	}
	return FAIL_INVALID_PARAM;
}

int get_appid_by_tm_handle(int handle, char **app_id)
{
	int app_idx;
	for (app_idx = 0; bixby_app_list[app_idx].app_id != NULL; app_idx++) {
		if (bixby_app_list[app_idx].handle == handle) {
			*app_id = (char *)malloc(strlen(bixby_app_list[app_idx].app_id) + 1);
			if (*app_id == NULL) {
				return FAIL_OUT_OF_MEMORY;
			}
			strncpy(*app_id, bixby_app_list[app_idx].app_id, strlen(bixby_app_list[app_idx].app_id) + 1);
			return OK;
		}
	}
	return FAIL_INVALID_PARAM;
}
