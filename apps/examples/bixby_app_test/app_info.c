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
#include <task_manager/task_manager.h>
#include <bixby_apps/app_utils.h>
#include "app_info.h"
#include "dlog.h"

static const char *app_info_error_to_string(app_info_error_e error)
{
	switch (error) {
	case APP_INFO_ERROR_NONE:
		return "Successful";
	case APP_INFO_ERROR_INVALID_PARAMETER:
		return "Invalid parameter";
	case APP_INFO_ERROR_OUT_OF_MEMORY:
		return "Out of memory";
	case APP_INFO_ERROR_IO_ERROR:
		return "IO error";
	case APP_INFO_ERROR_NO_SUCH_APP:
		return "No such application";
	case APP_INFO_ERROR_DB_FAILED:
		return "DB error";
	case APP_INFO_ERROR_INVALID_PACKAGE:
		return "Invalid package";
	case APP_INFO_ERROR_NOT_SUPPORTED:
		return "Not supported";
	case APP_INFO_ERROR_PERMISSION_DENIED:
		return "Permission denied";
	default:
		return "Unknown";
	}
}

int app_info_error(app_info_error_e error, const char *function, const char *description)
{
	if (description)
		LOGE("[%s] %s(0x%08x) : %s", function, app_info_error_to_string(error), error, description);
	else
		LOGE("[%s] %s(0x%08x)", function, app_info_error_to_string(error), error);

	return error;
}

int app_info_get_localed_label(const char *app_id, const char *locale, char **label)
{
	return 0;
}

int app_info_create(const char *app_id, app_info_h *app_info)
{
	tm_appinfo_t *info;
	int handle;

	if (app_id == NULL || app_info == NULL) {
		return app_info_error(APP_INFO_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	handle = get_tm_handle_by_appid(app_id);
	if (handle < 0) {
		return app_info_error(APP_INFO_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	info = task_manager_getinfo_with_handle(handle, TM_RESPONSE_WAIT_INF);
	if (app_info == NULL) {
		return app_info_error(APP_INFO_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}
	task_manager_clean_info(&info);

	(*app_info)->app_id = (char *)malloc(strlen(app_id) + 1);
	if ((*app_info)->app_id == NULL) {
		return app_info_error(APP_INFO_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}
	strncpy((*app_info)->app_id, app_id, strlen(app_id) + 1);

	return APP_INFO_ERROR_NONE;
}

int app_info_destroy(app_info_h app_info)
{
	if (app_info == NULL) {
		return app_info_error(APP_INFO_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	if (app_info->app_id != NULL) {
		free(app_info->app_id);
		app_info->app_id = NULL;
	}

	free(app_info);
	return APP_INFO_ERROR_NONE;
}

int app_info_foreach_metadata(app_info_h app_info, app_info_metadata_cb callback, void *user_data)
{
	return APP_INFO_ERROR_NONE;
}
