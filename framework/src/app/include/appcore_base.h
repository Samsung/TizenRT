/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdbool.h>
// #include <libintl.h>
#include <app/bundle.h>
#include <aul/aul.h>

#ifdef __cplusplus
extern "C" {
#endif

enum appcore_base_rm {
	APPCORE_BASE_RM_UNKNOWN,
	APPCORE_BASE_RM_PORTRAIT_NORMAL,
	APPCORE_BASE_RM_PORTRAIT_REVERSE,
	APPCORE_BASE_RM_LANDSCAPE_NORMAL,
	APPCORE_BASE_RM_LANDSCAPE_REVERSE,
};

enum appcore_base_event {
	APPCORE_BASE_EVENT_START,
	APPCORE_BASE_EVENT_LOW_MEMORY,
	APPCORE_BASE_EVENT_LOW_BATTERY,
	APPCORE_BASE_EVENT_LANG_CHANGE,
	APPCORE_BASE_EVENT_DEVICE_ORIENTATION_CHANGED,
	APPCORE_BASE_EVENT_REGION_CHANGE,
	APPCORE_BASE_EVENT_SUSPENDED_STATE_CHANGE,
	APPCORE_BASE_EVENT_UPDATE_REQUESTED,
	APPCORE_BASE_EVENT_MAX,
};

enum appcore_base_suspended_state {
	APPCORE_BASE_SUSPENDED_STATE_WILL_ENTER_SUSPEND = 0,
	APPCORE_BASE_SUSPENDED_STATE_DID_EXIT_FROM_SUSPEND
};


typedef int (*appcore_base_event_cb)(void *event, void *data);
typedef void *appcore_base_event_h;

typedef struct _appcore_base_ops {
	int (*create) (void *data);
	int (*terminate) (void *data);
	int (*control) (bundle *b, void *data);
	int (*receive)(aul_type type, bundle *b, void *data);
	int (*set_i18n)(void *data);
	void (*init)(int argc, char **argv, void *data);
	void (*finish)(void);
	void (*run)(void *data);
	void (*exit)(void *data);
	void (*set_event)(enum appcore_base_event event, void *data);
	void (*unset_event)(enum appcore_base_event event, void *data);
	void (*trim_memory)(void *data);
} appcore_base_ops;

int appcore_base_on_receive(aul_type type, bundle *b);
int appcore_base_on_create(void);
int appcore_base_on_control(bundle *b);
int appcore_base_on_terminate(void);
int appcore_base_on_set_i18n(void);
void appcore_base_on_set_event(enum appcore_base_event event);
void appcore_base_on_unset_event(enum appcore_base_event event);
int appcore_base_on_trim_memory(void);
int appcore_base_init(appcore_base_ops ops, int argc, char **argv, void *data);
void appcore_base_fini(void);
appcore_base_ops appcore_base_get_default_ops(void);
appcore_base_event_h appcore_base_add_event(enum appcore_base_event event,
		appcore_base_event_cb cb, void *data);
int appcore_base_remove_event(appcore_base_event_h handle);
int appcore_base_raise_event(void *event, enum appcore_base_event type);
int appcore_base_flush_memory(void);
int appcore_base_get_rotation_state(enum appcore_base_rm *curr);
bool appcore_base_is_bg_allowed(void);
bool appcore_base_is_suspended(void);
void appcore_base_toggle_suspended_state(void);
int appcore_base_set_i18n(const char *domain_name, const char *dir_name);
void appcore_base_exit(void);
void appcore_base_add_suspend_timer(void);
void appcore_base_remove_suspend_timer(void);

#ifdef __cplusplus
}
#endif


