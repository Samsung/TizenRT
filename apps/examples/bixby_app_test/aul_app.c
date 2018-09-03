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
#include <task_manager/task_manager.h>
#include <bixby_apps/app_utils.h>
#include "aul_app.h"

int aul_app_get_appid_bypid(int pid, char *appid, int len)
{
	tm_appinfo_t *app_info;
	int ret;
	char *app_id;

	if (pid <= 0 || appid == NULL) {
		return AUL_R_EINVAL;
	}

	app_info = task_manager_getinfo_with_pid(pid, TM_RESPONSE_WAIT_INF);
	if (app_info == NULL) {
		return AUL_R_ENOAPP;
	}

	ret = get_appid_by_tm_handle(app_info->handle, &app_id);
	if (ret == FAIL_OUT_OF_MEMORY) {
		task_manager_clean_info(&app_info);		
		return AUL_R_ERROR;
	} else if (ret == FAIL_INVALID_PARAM) {
		task_manager_clean_info(&app_info);
		return AUL_R_EINVAL;
	}

	strncpy(appid, app_id, strlen(app_id) + 1);

	free(app_id);
	task_manager_clean_info(&app_info);

	return AUL_R_OK;
}
