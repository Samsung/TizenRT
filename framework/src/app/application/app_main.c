/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlog.h>

#include <eventloop/eventloop.h>
#include <app/app.h>
#include <app/bundle.h>
#include <app/tizen_error.h>

#include "app_extension.h"
#include "app_control_internal.h"
#include "app_common_internal.h"
#include "appcore_ui_base.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_APPFW_APPLICATION"

#define UI_APP_EVENT_MAX 7

typedef enum {
	APP_STATE_NOT_RUNNING, /* The application has been launched or was running but was terminated */
	APP_STATE_CREATING, /* The application is initializing the resources on app_create_cb callback */
	APP_STATE_RUNNING, /* The application is running in the foreground and background */
} app_state_e;

struct app_event_handler {
	app_event_type_e type;
	app_event_cb cb;
	void *data;
	void *raw;
};

struct app_event_info {
	app_event_type_e type;
	void *value;
};

struct ui_app_context {
	ui_app_lifecycle_callback_s callback;
	void *data;
};

static struct ui_app_context __context;
static app_state_e __app_state = APP_STATE_NOT_RUNNING;

static int __app_event_converter[APPCORE_BASE_EVENT_MAX] = {
	[APP_EVENT_LOW_MEMORY] = APPCORE_BASE_EVENT_LOW_MEMORY,
	[APP_EVENT_LOW_BATTERY] = APPCORE_BASE_EVENT_LOW_BATTERY,
	[APP_EVENT_LANGUAGE_CHANGED] = APPCORE_BASE_EVENT_LANG_CHANGE,
	[APP_EVENT_DEVICE_ORIENTATION_CHANGED] = APPCORE_BASE_EVENT_DEVICE_ORIENTATION_CHANGED,
	[APP_EVENT_REGION_FORMAT_CHANGED] = APPCORE_BASE_EVENT_REGION_CHANGE,
	[APP_EVENT_SUSPENDED_STATE_CHANGED] = APPCORE_BASE_EVENT_SUSPENDED_STATE_CHANGE,
};

static int __ui_app_create(void *data)
{
	appcore_ui_base_on_create();
	if (__context.callback.create == NULL ||
			__context.callback.create(__context.data) == false)
		return app_error(APP_ERROR_INVALID_CONTEXT, __FUNCTION__, "app_create_cb() returns false");

	return APP_ERROR_NONE;
}

static int __ui_app_terminate(void *data)
{
	appcore_ui_base_on_terminate();

	if (__context.callback.terminate)
		__context.callback.terminate(__context.data);

	return APP_ERROR_NONE;
}

static int __ui_app_control(bundle *b, void *data)
{
	app_control_h app_control = NULL;

	appcore_ui_base_on_control(b);

	if (b) {
		if (app_control_create_event(b, &app_control) != APP_ERROR_NONE)
			return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "Failed to create an app_control handle");
	} else {
		if (app_control_create(&app_control) != APP_ERROR_NONE)
			return app_error(APP_ERROR_OUT_OF_MEMORY, __FUNCTION__, "Failed to create an app_control handle");
	}

	if (__context.callback.app_control)
		__context.callback.app_control(app_control, __context.data);

	app_control_destroy(app_control);

	return APP_ERROR_NONE;
}

static int __ui_app_pause(void *data)
{
	appcore_ui_base_on_pause();
	if (__context.callback.pause)
		__context.callback.pause(__context.data);
	return APP_ERROR_NONE;
}

static int __ui_app_resume(void *data)
{
	appcore_ui_base_on_resume();
	if (__context.callback.resume)
		__context.callback.resume(__context.data);
	return APP_ERROR_NONE;
}

static int __app_init(int argc, char **argv, ui_app_lifecycle_callback_s *callback, void *user_data, appcore_ui_base_ops ops)
{
	int ret;

	if (argc < 1 || argv == NULL || callback == NULL)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	if (callback->create == NULL)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "app_create_cb() callback must be registered");

	if (__app_state != APP_STATE_NOT_RUNNING)
		return app_error(APP_ERROR_ALREADY_RUNNING, __FUNCTION__, NULL);

	__context.callback = *callback;
	__context.data = user_data;
	__app_state = APP_STATE_CREATING;

	// UI Init
	ret = appcore_ui_base_init(ops, argc, argv, NULL, 0);

	if (ret < 0) {
		__app_state = APP_STATE_NOT_RUNNING;
		return app_error(APP_ERROR_INVALID_CONTEXT, __FUNCTION__, NULL);
	}

	return APP_ERROR_NONE;
}

static void __app_fini(void)
{
	// UI Finish
	// appcore_ui_base_fini();
	__app_state = APP_STATE_NOT_RUNNING;

}

int ui_app_main(int argc, char **argv, ui_app_lifecycle_callback_s *callback, void *user_data)
{
	int ret;
	appcore_ui_base_ops ops = appcore_ui_base_get_default_ops();

	/* override methods */
	ops.base.create = __ui_app_create;
	ops.base.control = __ui_app_control;
	ops.base.terminate = __ui_app_terminate;
	ops.pause = __ui_app_pause;
	ops.resume = __ui_app_resume;

	ret = __app_init(argc, argv, callback, user_data, ops);
	if (ret != APP_ERROR_NONE)
		return ret;

	__app_fini();

	return APP_ERROR_NONE;
}

void ui_app_exit(void)
{
	// appcore_ui_base_exit();
	eventloop_loop_stop();
}

int __event_cb(void *event, void *data)
{
	app_event_handler_h handler = data;

	struct app_event_info app_event;

	app_event.type = handler->type;
	app_event.value = event;

	if (handler->cb)
		handler->cb(&app_event, handler->data);

	return 0;
}

int ui_app_add_event_handler(app_event_handler_h *event_handler, app_event_type_e event_type, app_event_cb callback, void *user_data)
{
	app_event_handler_h handler;

	if (event_handler == NULL || callback == NULL)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "null parameter");

	if (event_type < APP_EVENT_LOW_MEMORY || event_type > APP_EVENT_UPDATE_REQUESTED)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid event type");

	handler = calloc(1, sizeof(struct app_event_handler));
	if (!handler)
		return app_error(APP_ERROR_OUT_OF_MEMORY, __FUNCTION__, "failed to create handler");

	handler->type = event_type;
	handler->cb = callback;
	handler->data = user_data;
	handler->raw = appcore_base_add_event(__app_event_converter[event_type], __event_cb, handler);

	*event_handler = handler;

	return APP_ERROR_NONE;
}

int ui_app_remove_event_handler(app_event_handler_h event_handler)
{
	int ret;
	app_event_type_e type;

	if (event_handler == NULL)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "handler is null");

	type = event_handler->type;
	if (type < APP_EVENT_LOW_MEMORY || type > APP_EVENT_UPDATE_REQUESTED)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid handler");

	ret = appcore_base_remove_event(event_handler->raw);
	if (ret < 0)
		return app_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid raw handler");

	free(event_handler);

	return APP_ERROR_NONE;
}

