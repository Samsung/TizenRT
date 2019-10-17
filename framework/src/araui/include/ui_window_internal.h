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

#ifndef __UI_WINDOW_INTERNAL_H__
#define __UI_WINDOW_INTERNAL_H__

#include <tinyara/config.h>
#include <vec/vec.h>
#include <araui/ui_window.h>
#include <araui/ui_commons.h>
#include "ui_widget_internal.h"

typedef void (*window_created_callback_t)(ui_window_t window);
typedef void (*window_destroyed_callback_t)(ui_window_t window);
typedef void (*window_shown_callback_t)(ui_window_t window);
typedef void (*window_hidden_callback_t)(ui_window_t window);

typedef struct ui_window_body_s ui_window_body_t;

struct ui_window_body_s {
	ui_widget_body_t *root;
	ui_widget_body_t *focus;

	window_created_callback_t created_cb;
	window_destroyed_callback_t destroyed_cb;
	window_shown_callback_t shown_cb;
	window_hidden_callback_t hidden_cb;
};

#ifdef __cplusplus
extern "C" {
#endif

ui_error_t ui_window_list_init(void);
ui_error_t ui_window_list_deinit(void);

#if defined(CONFIG_UI_PARTIAL_UPDATE)
ui_error_t ui_window_redraw_list_init(void);
ui_error_t ui_window_redraw_list_deinit(void);

vec_void_t *ui_window_get_redraw_list(void);
ui_error_t ui_window_add_redraw_list(ui_rect_t update);
ui_error_t ui_window_redraw_list_clear(void);
#endif

ui_window_body_t *ui_window_get_current(void);

#ifdef __cplusplus
}
#endif

#endif
