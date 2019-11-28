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

// #include <libintl.h>
#include <app/bundle.h>
#include "aul/aul.h"
#include "appcore_base.h"

typedef struct _appcore_ui_base_window_ops {
	void (*show)(int type, void *event, void *data);
	void (*hide)(int type, void *event, void *data);
	void (*lower)(int type, void *event, void *data);
	void (*visibility)(int type, void *event, void *data);
	void (*pre_visibility)(int type, void *event, void *data);
} appcore_ui_base_window_ops;

typedef struct _appcore_ui_base_ops {
	int (*pause) (void *data);
	int (*resume) (void *data);
	appcore_base_ops base;
	appcore_ui_base_window_ops window;
} appcore_ui_base_ops;

#ifdef __cplusplus
extern "C" {
#endif

enum appcore_ui_base_hint {
	APPCORE_UI_BASE_HINT_WINDOW_GROUP_CONTROL = 0x1,
	APPCORE_UI_BASE_HINT_WINDOW_STACK_CONTROL = 0x2,
	APPCORE_UI_BASE_HINT_BG_LAUNCH_CONTROL = 0x4,
	APPCORE_UI_BASE_HINT_HW_ACC_CONTROL = 0x8,
	APPCORE_UI_BASE_HINT_WINDOW_AUTO_CONTROL = 0x10,
	APPCORE_UI_BASE_HINT_LEGACY_CONTROL = 0x20,
};

int appcore_ui_base_on_receive(aul_type type, bundle *b);
int appcore_ui_base_on_create(void);
int appcore_ui_base_on_terminate(void);
int appcore_ui_base_on_pause(void);
int appcore_ui_base_on_resume(void);
int appcore_ui_base_on_control(bundle *b);
int appcore_ui_base_on_trim_memory(void);
void appcore_ui_base_window_on_show(int type, void *event);
void appcore_ui_base_window_on_hide(int type, void *event);
void appcore_ui_base_window_on_lower(int type, void *event);
void appcore_ui_base_window_on_visibility(int type, void *event);
void appcore_ui_base_window_on_pre_visibility(int type, void *event);
int appcore_ui_base_init(appcore_ui_base_ops ops, int argc, char **argv, void *data, unsigned int hint);
void appcore_ui_base_fini(void);
appcore_ui_base_ops appcore_ui_base_get_default_ops(void);
void appcore_ui_base_pause(void);
void appcore_ui_base_resume(void);
bool appcore_ui_base_is_resumed(void);
void appcore_ui_base_exit(void);
void appcore_ui_base_group_add(void);
void appcore_ui_base_group_remove(void);
unsigned int appcore_ui_base_get_main_window(void);
unsigned int appcore_ui_base_get_main_surface(void);
int appcore_ui_base_get_hint(void);
bool appcore_ui_base_get_bg_state(void);
void appcore_ui_base_set_bg_state(bool bg_state);
void appcore_ui_base_set_system_resource_reclaiming(bool enable);

#ifdef __cplusplus
}
#endif


