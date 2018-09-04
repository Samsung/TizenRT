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
#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include "tizen_error.h"

typedef enum {
	APP_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	APP_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_ERROR_INVALID_CONTEXT = TIZEN_ERROR_APPLICATION | 0x01, /**< Invalid application context */
	APP_ERROR_NO_SUCH_FILE = TIZEN_ERROR_NO_SUCH_FILE, /**< No such file or directory */
	APP_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported (Since 4.0) */
	APP_ERROR_ALREADY_RUNNING = TIZEN_ERROR_ALREADY_IN_PROGRESS, /**< Application is already running */
	APP_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied @if MOBILE (Since 2.3.1) @endif */
} app_error_e;

typedef enum {
	APP_EVENT_LOW_MEMORY, /**< The low memory event */
	APP_EVENT_LOW_BATTERY, /**< The low battery event */
	APP_EVENT_LANGUAGE_CHANGED, /**< The system language changed event */
	APP_EVENT_DEVICE_ORIENTATION_CHANGED, /**< The device orientation changed event */
	APP_EVENT_REGION_FORMAT_CHANGED, /**< The region format changed event */
	APP_EVENT_SUSPENDED_STATE_CHANGED, /**< The suspended state changed event of the application (since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif)
					     @see app_event_get_suspended_state */
	APP_EVENT_UPDATE_REQUESTED, /**< The update requested event (Since 3.0)
					This event can occur when an app needs to be updated.
					It is dependent on target devices. */
} app_event_type_e;

struct app_event_info {
	app_event_type_e type;
	void *value;
};
typedef struct app_event_info *app_event_info_h;

typedef void (*app_event_cb)(app_event_info_h event_info, void *user_data);

struct app_event_handler {
	app_event_type_e type;
	app_event_cb cb;
	void *data;
	void *raw;
};
typedef struct app_event_handler *app_event_handler_h;

char *app_get_data_path(void);
char *app_get_resource_path(void);
char *app_get_shared_data_path(void);
char *app_get_shared_resource_path(void);
char *app_get_shared_trusted_path(void);

#endif
