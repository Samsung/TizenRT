#include <tinyara/config.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <task_manager/task_manager.h>
#include <bixby_apps/app_utils.h>
#include "dlog.h"
#include "bundle.h"
#include "app_control.h"

#define AUL_SVC_RET_EINVAL -2
#define AUL_SVC_K_LAUNCH_RESULT_APP_STARTED "__K_LAUNCH_RESULT_APP_STARTED__"

#define AUL_SVC_K_OPERATION "__APP_SVC_OP_TYPE__"
#define AUL_SVC_K_PKG_NAME  "__APP_SVC_PKG_NAME__"

static int __set_bundle(bundle *b, const char *key, const char *value)
{
	const char *val = NULL;

	if (b == NULL || key == NULL)
		return AUL_SVC_RET_EINVAL;

	val = bundle_get_val(b, key);
	if (val) {
		if (bundle_del(b, key) != 0)
			return ERROR;
	}

	if (!value)
		return AUL_SVC_RET_EINVAL;

	if (bundle_add_str(b, key, value) != 0)
		return ERROR;

	return OK;
}

static int __set_bundle_array(bundle *b, const char *key, const char **value, int len)
{
	int type;

	if (b == NULL || key == NULL) {
		return AUL_SVC_RET_EINVAL;
	}
	type = bundle_get_type(b, key);
	if (type <= 0) {
		type = 0;
	}

	if (type & BUNDLE_TYPE_ARRAY) {
		if (bundle_del(b, key) != 0)
			return ERROR;
	}

	if (!value)
		return AUL_SVC_RET_EINVAL;

	if (bundle_add_str_array(b, key, value, len) != 0)
		return ERROR;

	return OK;
}

static const char *app_control_error_to_string(app_control_error_e error)
{
	switch (error) {
	case APP_CONTROL_ERROR_NONE:
		return "NONE";
	case APP_CONTROL_ERROR_INVALID_PARAMETER:
		return "INVALID_PARAMETER";
	case APP_CONTROL_ERROR_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY";
	case APP_CONTROL_ERROR_APP_NOT_FOUND:
		return "APP_NOT_FOUND";
	case APP_CONTROL_ERROR_KEY_NOT_FOUND:
		return "KEY_NOT_FOUND";
	case APP_CONTROL_ERROR_KEY_REJECTED:
		return "KEY_REJECTED";
	case APP_CONTROL_ERROR_INVALID_DATA_TYPE:
		return "INVALID_DATA_TYPE";
	case APP_CONTROL_ERROR_LAUNCH_REJECTED:
		return "LAUNCH_REJECTED";
	case APP_CONTROL_ERROR_PERMISSION_DENIED:
		return "PERMISSION_DENIED";
	case APP_CONTROL_ERROR_LAUNCH_FAILED:
		return "LAUNCH_FAILED";
	case APP_CONTROL_ERROR_TIMED_OUT:
		return "TIMED_OUT";
	case APP_CONTROL_ERROR_IO_ERROR:
		return "IO ERROR";
	default:
		return "UNKNOWN";
	}
}

static int app_control_error(app_control_error_e error, const char *function, const char *description)
{
	if (description)
		LOGE("[%s] %s(0x%08x) : %s", function, app_control_error_to_string(error), error, description);
	else {
		if (error == APP_CONTROL_ERROR_KEY_NOT_FOUND)
			LOGE("[%s] %s(0x%08x)", function, app_control_error_to_string(error), error);
		else
			LOGE("[%s] %s(0x%08x)", function, app_control_error_to_string(error), error);
	}

	return error;
}

static int app_control_validate_extra_data(const char *data)
{
	if (data == NULL || data[0] == '\0')
		return APP_CONTROL_ERROR_INVALID_PARAMETER;

	return APP_CONTROL_ERROR_NONE;
}

static int app_control_validate(app_control_h app_control)
{
	if (app_control == NULL || app_control->data == NULL)
		return APP_CONTROL_ERROR_INVALID_PARAMETER;

	return APP_CONTROL_ERROR_NONE;
}

int app_control_validate_internal_key(const char *key)
{
	if (strncmp(BUNDLE_KEY_PREFIX_AUL, key, strlen(BUNDLE_KEY_PREFIX_AUL)) == 0)
		return -1;

	if (strncmp(BUNDLE_KEY_PREFIX_SERVICE, key, strlen(BUNDLE_KEY_PREFIX_SERVICE)) == 0)
		return -1;

	return 0;
}

static int app_control_new_id(void)
{
	static int sid = 0;
	return sid++;
}

int app_control_create(app_control_h *app_control)
{
	app_control_h app_control_request;

	if (app_control == NULL) {
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}

	app_control_request = (app_control_h)malloc(sizeof(struct app_control_s));
	if (app_control_request == NULL) {
		return app_control_error(APP_CONTROL_ERROR_OUT_OF_MEMORY, __FUNCTION__, "failed to create a app_control handle");;
	}

	app_control_request->type = APP_CONTROL_TYPE_REQUEST;

	app_control_request->data = bundle_create();
	if (app_control_request->data == NULL) {
		free(app_control_request);
		return app_control_error(APP_CONTROL_ERROR_OUT_OF_MEMORY, __FUNCTION__, "failed to create a bundle");
	}

	app_control_request->id = app_control_new_id();
	app_control_request->launch_pid = -1;

	*app_control = app_control_request;

	return APP_CONTROL_ERROR_NONE;
}

int app_control_destroy(app_control_h app_control)
{
	if (app_control_validate(app_control))
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);


	if (app_control->type == APP_CONTROL_TYPE_REQUEST && app_control->launch_pid > 0
			&& bundle_get_val(app_control->data, AUL_SVC_K_LAUNCH_RESULT_APP_STARTED) == NULL)
//		aul_remove_caller_cb(app_control->launch_pid, app_control);


	bundle_free(app_control->data);
	app_control->data = NULL;
	free(app_control);

	return APP_CONTROL_ERROR_NONE;
}

int app_control_set_app_id(app_control_h app_control, const char *app_id)
{
	if (app_control_validate(app_control))
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	if (app_id != NULL) {
		if (__set_bundle(app_control->data, AUL_SVC_K_PKG_NAME, app_id) != 0)
			return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid application ID");
	} else {
		bundle_del(app_control->data, BUNDLE_KEY_PACKAGE);
	}

	return APP_CONTROL_ERROR_NONE;
}

int app_control_set_operation(app_control_h app_control, const char *operation)
{
	if (app_control_validate(app_control))
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	if (operation != NULL) {
		if (__set_bundle(app_control->data, AUL_SVC_K_OPERATION, operation) != 0)
			return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid operation");
	} else {
		bundle_del(app_control->data, BUNDLE_KEY_OPERATION);
	}

	return APP_CONTROL_ERROR_NONE;
}

int app_control_add_extra_data(app_control_h app_control, const char *key, const char *value)
{
	if (app_control_validate(app_control) || app_control_validate_extra_data(key) || app_control_validate_extra_data(value))
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	if (app_control_validate_internal_key(key))
		return app_control_error(APP_CONTROL_ERROR_KEY_REJECTED, __FUNCTION__, "the given key is reserved as internal use");


	if (bundle_get_val(app_control->data, key) != NULL) {
		/* overwrite any existing value */
		bundle_del(app_control->data, key);
	}

	if (__set_bundle(app_control->data, key, value) != 0)
		return app_control_error(APP_CONTROL_ERROR_KEY_REJECTED, __FUNCTION__, "failed to add data to the appsvc handle");

	return APP_CONTROL_ERROR_NONE;
}

int app_control_add_extra_data_array(app_control_h app_control, const char *key, const char* value[], int length)
{
	if (app_control_validate(app_control) || app_control_validate_extra_data(key))
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	if (value == NULL || length <= 0)
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid array");

	if (app_control_validate_internal_key(key))
		return app_control_error(APP_CONTROL_ERROR_KEY_REJECTED, __FUNCTION__, "the given key is reserved as internal use");

	if (bundle_get_str_array(app_control->data, key, NULL) != NULL) {
		/* overwrite any existing value */
		bundle_del(app_control->data, key);
	}

	if (__set_bundle_array(app_control->data, key, value, length) != 0)
		return app_control_error(APP_CONTROL_ERROR_KEY_REJECTED, __FUNCTION__, "failed to add array data to the appsvc handle");

	return APP_CONTROL_ERROR_NONE;
}

int app_control_clone(app_control_h *clone, app_control_h app_control)
{
	app_control_h app_control_clone;

	if (app_control_validate(app_control) || clone == NULL)
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	app_control_clone = malloc(sizeof(struct app_control_s));
	if (app_control_clone == NULL)
		return app_control_error(APP_CONTROL_ERROR_OUT_OF_MEMORY, __FUNCTION__, "failed to create a app_control handle");

	app_control_clone->id = app_control_new_id();
	app_control_clone->type = app_control->type;
	app_control_clone->data = bundle_dup(app_control->data);

	*clone = app_control_clone;

	return APP_CONTROL_ERROR_NONE;
}

int app_control_send_launch_request(app_control_h app_control, app_control_reply_cb callback, void *user_data)
{
	const char *operation;
	bool implicit_default_operation = false;
#if 0//reply_cb
	int launch_pid = 0;
	app_control_request_context_h request_context = NULL;
#endif
	int ret;
	const char *app_id;
	int handle;

	int status;
	struct mq_attr attr;
	mqd_t ac_send_mqfd;
	app_control_h app_control_dup;

	tm_appinfo_t *info;
	char *q_name;

	printf("[eon] test1\n");

	if (app_control_validate(app_control)) {
		printf("[eon] test2\n");
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	operation = bundle_get_val(app_control->data, AUL_SVC_K_OPERATION);
	if (operation == NULL) {
		printf("[eon] test3\n");
		implicit_default_operation = true;
		operation = APP_CONTROL_OPERATION_DEFAULT;
	}

	if (!strcmp(operation, APP_CONTROL_OPERATION_LAUNCH_ON_EVENT)) {
		printf("[eon] test3\n");
		return app_control_error(APP_CONTROL_ERROR_LAUNCH_REJECTED, __FUNCTION__,
				"Not supported operation value");
	}
	/* TODO: Check the privilege for call operation */

	/* operation : default */
	/* printf("[eon]operation : %s\n", operation); */
	/* printf("[eon]APP_CONTROL_OPERATION_DEFAULT : %s\n", APP_CONTROL_OPERATION_DEFAULT); */
	if (!strcmp(operation, APP_CONTROL_OPERATION_DEFAULT)) {
		printf("[eon] test4\n");
		const char *appid  = bundle_get_val(app_control->data, AUL_SVC_K_PKG_NAME);
		if (appid == NULL)
			return app_control_error(APP_CONTROL_ERROR_APP_NOT_FOUND, __FUNCTION__, "package must be specified if the operation is default value");
	}

#if 0//reply_cb

	if (callback != NULL) {
		app_control_h request_clone = NULL;

		request_context = calloc(1, sizeof(struct app_control_request_context_s));
		if (request_context == NULL)
			return app_control_error(APP_CONTROL_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);

		request_context->reply_cb = callback;

		ret = app_control_clone(&request_clone, app_control);
		if (ret != APP_CONTROL_ERROR_NONE) {
			free(request_context);
			return app_control_error(ret, __FUNCTION__, "failed to clone the app_control request handle");
		}

		request_context->app_control = request_clone;
		request_context->user_data = user_data;
	}
#endif

	if (implicit_default_operation == true) {
		printf("[eon] test5\n");
		__set_bundle(app_control->data, AUL_SVC_K_OPERATION, APP_CONTROL_OPERATION_DEFAULT);
	}
/*
	launch_pid = aul_svc_run_service_for_uid(app_control->data, app_control->id, callback ? app_control_request_result_broker : NULL, request_context, getuid());
*/
	app_id = bundle_get_val(app_control->data, AUL_SVC_K_PKG_NAME);
//	printf("[eon2] add_id : %s\n", app_id);
	handle = get_tm_handle_by_appid(app_id);
//	printf("[eon2] handle : %d\n", handle);
	if (handle < 0) {
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	}
	ret = task_manager_start(handle, TM_RESPONSE_WAIT_INF);
	if (ret == TM_INVALID_PARAM) {
		return app_control_error(APP_CONTROL_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);
	} else if (ret == TM_OUT_OF_MEMORY) {
		return app_control_error(APP_CONTROL_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	} else if (ret == TM_NO_PERMISSION) {
		return app_control_error(APP_CONTROL_ERROR_PERMISSION_DENIED, __FUNCTION__, NULL);
	} else if (ret == TM_UNREGISTERED_APP) {
		printf("[eon] not found 1\n");
		return app_control_error(APP_CONTROL_ERROR_APP_NOT_FOUND, __FUNCTION__, NULL);
	} else if (ret == TM_ALREADY_STARTED_APP || ret == TM_OPERATION_FAIL || ret == TM_INVALID_DRVFD || ret < 0) {
		return app_control_error(APP_CONTROL_ERROR_LAUNCH_FAILED, __FUNCTION__, NULL);
	}

	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(struct app_control_s);
	attr.mq_flags = 0;

	info = task_manager_getinfo_with_handle(handle, TM_RESPONSE_WAIT_INF);
	if (info == NULL) {
//		printf("[eon] not found 2\n");
		return app_control_error(APP_CONTROL_ERROR_APP_NOT_FOUND, __FUNCTION__, NULL);
	}
	asprintf(&q_name, "%s%d", "appcontrol_", info->handle);

	ac_send_mqfd = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);
	if (ac_send_mqfd == (mqd_t)ERROR) {
		free(q_name);
		task_manager_clean_info(&info);
		return app_control_error(APP_CONTROL_ERROR_LAUNCH_FAILED, __FUNCTION__, NULL);
	}

	app_control_dup = (app_control_h)malloc(sizeof(struct app_control_s));
	if (app_control_dup == NULL) {
		free(q_name);
		task_manager_clean_info(&info);
		return app_control_error(APP_CONTROL_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL);
	}

	app_control_clone(&app_control_dup, app_control);
	status = mq_send(ac_send_mqfd, (const char *)app_control_dup, sizeof(struct app_control_s), 100);
	if (status < 0) {
		free(q_name);
		app_control_destroy(app_control_dup);
		task_manager_clean_info(&info);
		return app_control_error(APP_CONTROL_ERROR_LAUNCH_FAILED, __FUNCTION__, NULL);
	}

	free(q_name);
	task_manager_clean_info(&info);
	mq_close(ac_send_mqfd);

//	launch_pid = info->pid

	if (implicit_default_operation == true)
		bundle_del(app_control->data, BUNDLE_KEY_OPERATION);

#if 0 //reply_cb
/*
	if (launch_pid < 0) {
		if (request_context) {
			if (request_context->app_control)
				app_control_destroy(request_context->app_control);

			free(request_context);
		}

	}
*/

	app_control->launch_pid = launch_pid;
	/* app_control_enable_app_started_result_event called */
	if (bundle_get_val(app_control->data, AUL_SVC_K_LAUNCH_RESULT_APP_STARTED)) {
//		char callee[255] = {0,};
//		if (aul_app_get_appid_bypid(launch_pid, callee, sizeof(callee)) != AUL_R_OK)
			dbg("aul_app_get_appid_bypid failed: %d", launch_pid);

		if (request_context && request_context->app_control)
			request_context->app_control->launch_pid = launch_pid;

//		aul_add_caller_cb(launch_pid, __handle_launch_result, request_context);

		/* launched without app selector */
/*
		if ((strcmp(callee, APP_SELECTOR) != 0) &&
				(strcmp(callee, SHARE_PANEL) != 0))
			aul_invoke_caller_cb(request_context);
*/

	} else { /* default case */
//		aul_add_caller_cb(launch_pid, __update_launch_pid, app_control);
	}
#endif
	return APP_CONTROL_ERROR_NONE;
}
