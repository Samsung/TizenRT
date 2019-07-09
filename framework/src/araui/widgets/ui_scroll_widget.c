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
#include "ui_log.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_view_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "ui_commons_internal.h"
#include "hal/ui_hal_interface.h"

#define CONFIG_UI_SCROLL_DAMPING 10.0f

typedef struct {
	ui_scroll_widget_body_t *body;
	int32_t width;
	int32_t height;
} ui_set_content_size_t;

typedef struct {
	ui_scroll_widget_body_t *body;
	ui_direction_t direction;
} ui_set_scroll_direction_t;

static void _ui_scroll_widget_set_direction_func(void *userdata);
static void _ui_scroll_widget_set_content_size_func(void *userdata);

ui_error_t ui_scroll_widget_set_content_size(ui_widget_t widget, int32_t width, int32_t height)
{
	ui_set_content_size_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_SCROLL_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_content_size_t *)UI_ALLOC(sizeof(ui_set_content_size_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_content_size_t));

	info->body = (ui_scroll_widget_body_t *)widget;
	info->width = width;
	info->height = height;

	if (ui_request_callback(_ui_scroll_widget_set_content_size_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _recalc_offset_info(ui_scroll_widget_body_t *scroll)
{
	scroll->min_offset.x = scroll->base.local_rect.width - scroll->content_width;
	scroll->min_offset.y = scroll->base.local_rect.height - scroll->content_height;
	
	if (scroll->min_offset.x > 0) {
		scroll->min_offset.x = 0;
	}

	if (scroll->min_offset.y > 0) {
		scroll->min_offset.y = 0;
	}

	if (scroll->direction == UI_DIRECTION_VERTICAL) {
		scroll->min_offset.x = 0;
	}

	if (scroll->direction == UI_DIRECTION_HORIZONTAL) {
		scroll->min_offset.y = 0;
	}
}

static void _ui_scroll_widget_set_content_size_func(void *userdata)
{
	ui_set_content_size_t *info;

	if (!userdata || !((ui_set_content_size_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_content_size_t *)userdata;

	info->body->content_width = info->width;
	info->body->content_height = info->height;

	_recalc_offset_info(info->body);

	UI_FREE(info);
}

ui_error_t ui_scroll_widget_set_direction(ui_widget_t widget, ui_direction_t direction)
{
	ui_set_scroll_direction_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_SCROLL_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_scroll_direction_t *)UI_ALLOC(sizeof(ui_set_scroll_direction_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_scroll_direction_t));

	info->body = (ui_scroll_widget_body_t *)widget;
	info->direction = direction;

	if (ui_request_callback(_ui_scroll_widget_set_direction_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_scroll_widget_set_direction_func(void *userdata)
{
	ui_set_scroll_direction_t *info;

	if (!userdata || !((ui_set_scroll_direction_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_scroll_direction_t *)userdata;

	info->body->direction = info->direction;

	_recalc_offset_info(info->body);

	UI_FREE(info);
}

#if defined(CONFIG_UI_TOUCH_INTERFACE)

static void _apply_delta_to_all_children(ui_scroll_widget_body_t *widget, ui_coord_t delta)
{
	ui_list_node_t *node;

	node = ui_list_loop_begin(&widget->base.children);
	while (node != ui_list_loop_end(&widget->base.children)) {
		((ui_widget_body_t *)node->data)->local_rect.x += delta.x;
		((ui_widget_body_t *)node->data)->local_rect.y += delta.y;
		ui_widget_update_position_info((ui_widget_body_t *)node->data);
		node = node->next;
	}
}

void ui_scroll_widget_touch_func(ui_widget_body_t *widget, ui_touch_event_t event, ui_coord_t coord)
{
	ui_coord_t delta;
	ui_scroll_widget_body_t *body;

	if (!widget) {
		UI_LOGE("Error: widget is null!\n");
		return;
	}

	body = (ui_scroll_widget_body_t *)widget;

	switch (event) {
		case UI_TOUCH_EVENT_CANCEL: {
			body->state = UI_SCROLL_STATE_NONE;
		}
		break;

		case UI_TOUCH_EVENT_DOWN: {
			switch (body->state) {
				case UI_SCROLL_STATE_NONE: {
					body->state = UI_SCROLL_STATE_THRESHOLD;
					body->prev_touch = coord;
					body->scroll_velocity_x = 0.0f;
					body->scroll_velocity_y = 0.0f;
				}
				break;

				default: {
				}
				break;
			}
		}
		break;

		case UI_TOUCH_EVENT_MOVE: {
			delta.x = coord.x - body->prev_touch.x;
			delta.y = coord.y - body->prev_touch.y;

			switch (body->state) {
				case UI_SCROLL_STATE_THRESHOLD: {
					if ((body->direction == UI_DIRECTION_VERTICAL && UI_ABS(delta.y) >= CONFIG_UI_TOUCH_THRESHOLD) ||
						(body->direction == UI_DIRECTION_HORIZONTAL && UI_ABS(delta.x) >= CONFIG_UI_TOUCH_THRESHOLD) ||
						(body->direction == UI_DIRECTION_ALL && (UI_ABS(delta.x) >= CONFIG_UI_TOUCH_THRESHOLD || UI_ABS(delta.y) >= CONFIG_UI_TOUCH_THRESHOLD))) {
						ui_core_lock_touch_event_target((ui_widget_body_t *)body);
						body->state = UI_SCROLL_STATE_SCROLLING;
					}
				}
				break;

				case UI_SCROLL_STATE_SCROLLING: {
					body->prev_touch = coord;
					body->prev_offset.x = body->offset.x;
					body->prev_offset.y = body->offset.y;

					body->offset.x += delta.x;
					body->offset.y += delta.y;

					if (body->offset.x > 0) {
						body->offset.x = 0;
					}

					if (body->offset.y > 0) {
						body->offset.y = 0;
					}

					if (body->offset.x < body->min_offset.x) {
						body->offset.x = body->min_offset.x;
					}

					if (body->offset.y < body->min_offset.y) {
						body->offset.y = body->min_offset.y;
					}

					// From now, the delta means the delta of the previous offset and the current offset
					delta.x = body->offset.x - body->prev_offset.x;
					delta.y = body->offset.y - body->prev_offset.y;

					// Update scroll velocity
					body->scroll_velocity_x += delta.x;
					body->scroll_velocity_y += delta.y;

					// update position of all children (sync)
					_apply_delta_to_all_children(body, delta);
				}
				break;

				default: {
				}
				break;
			}
		}
		break;

		case UI_TOUCH_EVENT_UP: {
			body->state = UI_SCROLL_STATE_NONE;
		}
		break;

		default:
		break;
	}
}

#endif // CONFIG_UI_TOUCH_INTERFACE

static void ui_scroll_widget_update_func(ui_widget_t widget, uint32_t dt)
{
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	ui_coord_t delta;
	ui_scroll_widget_body_t *body;
#endif

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

#if defined(CONFIG_UI_TOUCH_INTERFACE)

	body = (ui_scroll_widget_body_t *)widget;

	// Apply scroll velocity on the touch interface environment
	// If CONFIG_UI_TOUCH_INTERFACE is disabled, this part is not needed.
	if (body->state == UI_SCROLL_STATE_NONE) {
		delta.x = body->scroll_velocity_x;
		delta.y = body->scroll_velocity_y;

		body->prev_offset.x = body->offset.x;
		body->prev_offset.y = body->offset.y;

		body->offset.x += delta.x;
		body->offset.y += delta.y;

		if (body->offset.x > 0) {
			body->offset.x = 0;
		}

		if (body->offset.y > 0) {
			body->offset.y = 0;
		}

		if (body->offset.x < body->min_offset.x) {
			body->offset.x = body->min_offset.x;
		}

		if (body->offset.y < body->min_offset.y) {
			body->offset.y = body->min_offset.y;
		}

		delta.x = body->offset.x - body->prev_offset.x;
		delta.y = body->offset.y - body->prev_offset.y;

		if (delta.x != 0 || delta.y != 0) {
			_apply_delta_to_all_children(body, delta);
			ui_widget_update_position_info((ui_widget_body_t *)body);
		}

		body->scroll_velocity_x *= CONFIG_UI_SCROLL_DAMPING * (dt / 400.0f);
		body->scroll_velocity_y *= CONFIG_UI_SCROLL_DAMPING * (dt / 400.0f);

	} else if (body->state == UI_SCROLL_STATE_SCROLLING) {

		// If the scroll widget is scrolling now,
		// velocity must be decreased faster than touch up case.
		// If not, although the user stops the finger for some time, velocity will be applied when touch up.
		body->scroll_velocity_x *= CONFIG_UI_SCROLL_DAMPING * (dt / 600.0f);
		body->scroll_velocity_y *= CONFIG_UI_SCROLL_DAMPING * (dt / 600.0f);
		ui_widget_update_position_info((ui_widget_body_t *)body);
	}

#endif // CONFIG_UI_TOUCH_INTERFACE
}

ui_widget_t ui_scroll_widget_create(int32_t width, int32_t height)
{
	ui_scroll_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_scroll_widget_body_t *)UI_ALLOC(sizeof(ui_scroll_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_scroll_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, width, height);
	((ui_widget_body_t *)body)->type = UI_SCROLL_WIDGET;

	// default settings
	body->base.update_cb = ui_scroll_widget_update_func;
	body->content_width = width;
	body->content_height = height;

#if defined(CONFIG_UI_TOUCH_INTERFACE)
	body->base.touchable = true;
	body->base.is_hooker = true;
	body->base.touch_cb = ui_scroll_widget_touch_func;
#endif

	UI_LOGD("UI_OK.\n");

	return (ui_widget_t)body;
}
