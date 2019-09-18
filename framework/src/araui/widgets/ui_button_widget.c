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

#include <tinyara/config.h>
#include <string.h>
#include <araui/ui_commons.h>
#include "ui_debug.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_window_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "ui_commons_internal.h"

#if defined(CONFIG_UI_ENABLE_TOUCH)
typedef struct {
	ui_button_widget_body_t *body;
	button_touched_callback touched_cb;
} ui_set_button_cb_t;
#endif

#if defined(CONFIG_UI_ENABLE_TOUCH)
static void _ui_button_widget_set_touched_callback_func(void *userdata);
#endif

#if defined(CONFIG_UI_ENABLE_TOUCH)
ui_error_t ui_button_widget_set_touched_callback(ui_widget_t widget, button_touched_callback touched_cb)
{
	ui_set_button_cb_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_BUTTON_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_button_cb_t *)UI_ALLOC(sizeof(ui_set_button_cb_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_button_cb_t));

	info->body = (ui_button_widget_body_t *)widget;
	info->touched_cb = touched_cb;

	if (ui_request_callback(_ui_button_widget_set_touched_callback_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_button_widget_set_touched_callback_func(void *userdata)
{
	ui_set_button_cb_t *info;

	if (!userdata || !((ui_set_button_cb_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_button_cb_t *)userdata;

	info->body->touched_cb = info->touched_cb;

	UI_FREE(info);
}
#endif

#if defined(CONFIG_UI_ENABLE_TOUCH)

void ui_button_widget_touch_func(ui_widget_body_t *widget, ui_touch_event_t event, ui_coord_t coord)
{
	ui_button_widget_body_t *body;

	if (!widget) {
		UI_LOGE("Error: widget is null!\n");
		return;
	}

	body = (ui_button_widget_body_t *)widget;

	switch (event) {
	case UI_TOUCH_EVENT_CANCEL:
		body->pressed = false;
		break;

	case UI_TOUCH_EVENT_DOWN:
		body->pressed = true;
		break;

	case UI_TOUCH_EVENT_UP:
		if (body->pressed) {
			body->pressed = false;

			if (ui_coord_inside_rect(coord, body->base.global_rect)) {
				if (body->touched_cb) {
					body->touched_cb((ui_widget_t)body);
				}
			}
		}
		break;

	default:
		break;
	}

	if (event == UI_TOUCH_EVENT_DOWN) {
		
	}
}

#endif // CONFIG_UI_ENABLE_TOUCH

ui_widget_t ui_button_widget_create(int32_t width, int32_t height)
{
	ui_button_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_button_widget_body_t *)UI_ALLOC(sizeof(ui_button_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_button_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, width, height);
	((ui_widget_body_t *)body)->type = UI_BUTTON_WIDGET;

#if defined(CONFIG_UI_ENABLE_TOUCH)
	((ui_widget_body_t *)body)->touchable = true;
	body->base.touch_cb = ui_button_widget_touch_func;
#endif

	return (ui_widget_t)body;
}

