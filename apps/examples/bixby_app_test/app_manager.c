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
#include <debug.h>
#include <task_manager/task_manager.h>
#include <bixby_apps/app_utils.h>
#include "app_manager.h"
#include "tizen_error.h"
#include "dlog.h"

static const char *app_manager_error_to_string(app_manager_error_e error)
{
	switch (error) {
	case APP_MANAGER_ERROR_NONE:
		return "Successful";
	case APP_MANAGER_ERROR_INVALID_PARAMETER:
		return "Invalid parameter";
	case APP_MANAGER_ERROR_OUT_OF_MEMORY:
		return "Out of memory";
	case APP_MANAGER_ERROR_IO_ERROR:
		return "IO error";
	case APP_MANAGER_ERROR_NO_SUCH_APP:
		return "No such application";
	case APP_MANAGER_ERROR_DB_FAILED:
		return "DB error";
	case APP_MANAGER_ERROR_INVALID_PACKAGE:
		return "Invalid package";
	case APP_MANAGER_ERROR_PERMISSION_DENIED:
		return "Permission denied";
	default:
		return "Unknown";
	}
}

int app_manager_error(app_manager_error_e error, const char *function, const char *description)
{
	if (description) {
		LOGE("[%s] %s(0x%08x) : %s", function, app_manager_error_to_string(error), error, description);
	} else {
		LOGE("[%s] %s(0x%08x)", function, app_manager_error_to_string(error), error);
	}

	return error;
}

int app_manager_get_app_id(pid_t pid, char **app_id)
{
	tm_appinfo_t *info;
	int ret;

	if (app_id == NULL || pid < 0) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	info = task_manager_getinfo_with_pid(pid, TM_RESPONSE_WAIT_INF);
	if (info == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, "Invalid process ID");
	}
	
	ret = get_appid_by_tm_handle(info->handle, app_id);
	if (ret == FAIL_OUT_OF_MEMORY) {
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, "Out of memory");
	} else if (ret == FAIL_INVALID_PARAM) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, "Invalid parameter");
	}
	
	return APP_MANAGER_ERROR_NONE;
}

int app_manager_get_app_context(const char *app_id, app_context_h *app_context)
{
	app_context_h context;
	tm_appinfo_t *info;
	int handle;

	if (app_id == NULL || app_context == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	context = (app_context_h)malloc(sizeof(struct app_context_s));
	if (context == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, "Out of memory");
	}

	context->app_id = strdup(app_id);
	if (context->app_id == NULL) {
		free(context);
		return app_manager_error(APP_MANAGER_ERROR_OUT_OF_MEMORY, __FUNCTION__, "Out of memory");
	}

	handle = get_tm_handle_by_appid(app_id);
	if (handle < 0) {
		free(context->app_id);
		free(context);
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	info = task_manager_getinfo_with_handle(handle, TM_RESPONSE_WAIT_INF);
	if (info == NULL) {
		free(context->app_id);
		free(context);
		return app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}

	if (info->status == TM_APP_STATE_RUNNING || info->status == TM_APP_STATE_PAUSE) {
		context->app_state = APP_STATE_SERVICE;
	} else if (info->status == TM_APP_STATE_STOP) {
		context->app_state = APP_STATE_TERMINATED;
	} else {
		context->app_state = APP_STATE_UNDEFINED;
	}
	task_manager_clean_info(&info);

	context->pid = 0;
	context->pkg_id = NULL;
	context->is_sub_app = false;
	context->instance_id = NULL;

	*app_context = context;

	return APP_MANAGER_ERROR_NONE;
}

int app_manager_resume_app(app_context_h app_context)
{
	int ret;
	int handle;
	if (app_context == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	handle = get_tm_handle_by_appid(app_context->app_id);
	if (handle < 0) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	ret = task_manager_resume(handle, TM_RESPONSE_WAIT_INF);
	if (ret == TM_INVALID_PARAM) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	} else if (ret == TM_NO_PERMISSION) {
		return app_manager_error(APP_MANAGER_ERROR_PERMISSION_DENIED, __FUNCTION__, NULL);
	} else if (ret != OK) {
		return app_manager_error(APP_MANAGER_ERROR_REQUEST_FAILED, __FUNCTION__, NULL);
	}

	return APP_MANAGER_ERROR_NONE;
}

int app_manager_get_app_info(const char *app_id, app_info_h *app_info)
{
	tm_appinfo_t *info;
	int handle;

	if (app_id == NULL || app_info == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	handle = get_tm_handle_by_appid(app_id);
	if (handle < 0) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	info = task_manager_getinfo_with_handle(handle, TM_RESPONSE_WAIT_INF);
	if (info == NULL) {
		return app_manager_error(APP_MANAGER_ERROR_NO_SUCH_APP, __FUNCTION__, NULL);
	}
	task_manager_clean_info(&info);
	return APP_MANAGER_ERROR_NONE;
}

int app_manager_is_running(const char *app_id, bool *running)
{
	int handle;
	tm_appinfo_t *app_info;

	if (app_id == NULL || running == NULL) {
		return APP_MANAGER_ERROR_INVALID_PARAMETER;
	}

	handle = get_tm_handle_by_appid(app_id);
	if (handle < 0) {
		return app_manager_error(APP_MANAGER_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_info = task_manager_getinfo_with_handle(handle, TM_RESPONSE_WAIT_INF);
	if (app_info == NULL) {
		*running = false;
		return APP_MANAGER_ERROR_NONE;
	}
	*running = true;
	task_manager_clean_info(&app_info);
	return APP_MANAGER_ERROR_NONE;
}
