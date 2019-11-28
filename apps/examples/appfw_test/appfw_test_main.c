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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <app/app.h>

typedef struct appdata {
	int data;
} appdata_s;

static bool
app_create(void *data)
{
	printf("[%s] called\n", __FUNCTION__);
	/* Hook to take necessary actions before main event loop starts
	 * Initialize UI resources and application's data
	 * If this function returns true, the main loop of application starts
	 * If this function returns false, the application is terminated
	 */
	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	printf("[%s] called\n", __FUNCTION__);
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	printf("[%s] called\n", __FUNCTION__);
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	printf("[%s] called\n", __FUNCTION__);
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	printf("[%s] called\n", __FUNCTION__);
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	printf("[%s] called\n", __FUNCTION__);
	/*APP_EVENT_LANGUAGE_CHANGED*/
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	printf("[%s] called\n", __FUNCTION__);
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	printf("[%s] called\n", __FUNCTION__);
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	printf("[%s] called\n", __FUNCTION__);
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	printf("[%s] called\n", __FUNCTION__);
	/*APP_EVENT_LOW_MEMORY*/
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int appfw_test_main(int argc, char *argv[])
#endif
{
	printf("[%s] enter\n", __FUNCTION__);

	int ret;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL,};
	appdata_s ad = {0};

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		printf("app_main() failed, err = %d\n", ret);
	}

	printf("[%s] exit\n", __FUNCTION__);
	return ret;
}
