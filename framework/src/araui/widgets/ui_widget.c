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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <vec/vec.h>
#include <araui/ui_commons.h>
#include <araui/ui_widget.h>
#include <araui/ui_animation.h>
#include "ui_debug.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "ui_window_internal.h"
#include "ui_commons_internal.h"
#include "ui_animation_internal.h"
#include "ui_renderer.h"
#include "utils/easing_fn.h"

typedef void (*touch_event_callback)(ui_widget_t widget);

typedef struct {
	ui_widget_body_t *body;
	bool visible;
} ui_set_visible_info_t;

typedef struct {
	ui_widget_body_t *body;
	bool touchable;
} ui_set_touchable_info_t;

typedef struct {
	ui_widget_body_t *body;
	ui_coord_t coord;
} ui_set_position_info_t;

typedef struct {
	ui_widget_body_t *body;
	ui_size_t size;
} ui_set_size_info_t;

typedef struct {
	ui_widget_body_t *body;
	ui_widget_body_t *child;
	int32_t x;
	int32_t y;
} ui_add_child_info_t;

typedef struct {
	ui_widget_body_t *body;
	ui_widget_body_t *child;
} ui_remove_child_info_t;

typedef struct {
	ui_widget_body_t *body;
	int32_t x;
	int32_t y;
} ui_set_pivot_point_info_t;

typedef struct {
	ui_widget_body_t *body;
	float scale_x;
	float scale_y;
} ui_set_scale_info_t;

typedef struct {
	ui_widget_body_t *body;
	int32_t degree;
} ui_rotate_info_t;

typedef struct {
	ui_widget_body_t *body;
	ui_anim_t *anim;
	anim_finished_callback anim_finished_cb;
} ui_set_anim_info_t;

/**
 * @brief Matrix multiplication macros
 */
#define UI_GET_TRANS_X(widget, x, y) ( \
	((widget)->trans_mat.m[0][0] * (x)) + \
	((widget)->trans_mat.m[0][1] * (y)) + \
	((widget)->trans_mat.m[0][2]))

#define UI_GET_TRANS_Y(widget, x, y) ( \
	((widget)->trans_mat.m[1][0] * (x)) + \
	((widget)->trans_mat.m[1][1] * (y)) + \
	((widget)->trans_mat.m[1][2]))

static void _ui_widget_set_visible_func(void *userdata);
static void _ui_widget_set_position_func(void *userdata);
static void _ui_widget_set_size_func(void *userdata);
static void _ui_widget_play_anim(void *userdata);

static void _ui_widget_destroy_func(void *userdata);
static void _ui_widget_add_child_func(void *userdata);
static void _ui_widget_remove_child_func(void *userdata);
static void _ui_widget_remove_all_children_func(void *widget);
static void _ui_widget_set_pivot_point_func(void *userdata);
static void _ui_widget_set_scale_func(void *userdata);
static void _ui_widget_set_rotation_func(void *userdata);

#if defined(CONFIG_UI_ENABLE_TOUCH)
static void _ui_widget_set_touchable_func(void *userdata);
#endif

static const ui_rect_t NULL_RECT = {0, };
static ui_widget_body_queue_t g_widget_body_queue;

inline bool ui_widget_check_widget_type(ui_widget_t widget, ui_widget_type_t type)
{
	return (((ui_widget_body_t *)widget)->type == type);
}

ui_error_t ui_widget_update_position_info(ui_widget_body_t *widget)
{
	if (!widget) {
		return UI_INVALID_PARAM;
	}

	widget->update_flag = true;

	return UI_OK;
}

ui_error_t ui_widget_set_visible(ui_widget_t widget, bool visible)
{
	ui_set_visible_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_visible_info_t *)UI_ALLOC(sizeof(ui_set_visible_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->visible = visible;

	if (ui_request_callback(_ui_widget_set_visible_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_visible_func(void *userdata)
{
	ui_set_visible_info_t *info;
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid param!\n");
		return;
	}

	info = (ui_set_visible_info_t *)userdata;
	body = (ui_widget_body_t *)info->body;
	body->visible = info->visible;

	body->update_flag = true;

	UI_FREE(info);
}

ui_error_t ui_widget_set_position(ui_widget_t widget, int32_t x, int32_t y)
{
	ui_set_position_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_position_info_t *)UI_ALLOC(sizeof(ui_set_position_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->coord.x = x;
	info->coord.y = y;

	if (ui_request_callback(_ui_widget_set_position_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_position_func(void *userdata)
{
	ui_set_position_info_t *info;
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid param!\n");
		return;
	}

	info = (ui_set_position_info_t *)userdata;
	body = (ui_widget_body_t *)info->body;
	body->local_rect.x = info->coord.x;
	body->local_rect.y = info->coord.y;

	body->update_flag = true;

	UI_FREE(info);
}

ui_rect_t ui_widget_get_rect(ui_widget_t widget)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI Framework is not running!\n");
		return NULL_RECT;
	}

	if (!widget) {
		return NULL_RECT;
	}

	body = (ui_widget_body_t *)widget;

	return body->local_rect;
}

ui_error_t ui_widget_set_size(ui_widget_t widget, int32_t width, int32_t height)
{
	ui_set_size_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_size_info_t *)UI_ALLOC(sizeof(ui_set_size_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->size.width = width;
	info->size.height = height;

	if (ui_request_callback(_ui_widget_set_size_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_size_func(void *userdata)
{
	ui_set_size_info_t *info;
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	info = (ui_set_size_info_t *)userdata;
	body = (ui_widget_body_t *)info->body;

	body->local_rect.width = info->size.width;
	body->local_rect.height = info->size.height;

	if (ui_widget_update_position_info(body) != UI_OK) {
		UI_LOGE("error: failed to update position information!\n");
		return;
	}

	UI_FREE(info);
}

ui_error_t ui_widget_set_tick_callback(ui_widget_t widget, tick_callback tick_cb)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	body = (ui_widget_body_t *)widget;
	body->tick_cb = tick_cb;

	return UI_OK;
}

ui_error_t ui_widget_set_interval_callback(ui_widget_t widget, interval_callback interval_cb, uint32_t timeout)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	body = (ui_widget_body_t *)widget;

	body->interval_cb = interval_cb;
	body->interval_info.timeout = timeout;
	body->interval_info.current = 0;

	return UI_OK;
}

#if defined(CONFIG_UI_ENABLE_TOUCH)

ui_error_t ui_widget_set_touchable(ui_widget_t widget, bool touchable)
{
	ui_set_touchable_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_touchable_info_t *)UI_ALLOC(sizeof(ui_set_touchable_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_touchable_info_t));

	info->body = (ui_widget_body_t *)widget;
	info->touchable = touchable;

	if (ui_request_callback(_ui_widget_set_touchable_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_touchable_func(void *userdata)
{
	ui_set_touchable_info_t *info;

	if (!userdata || !((ui_set_touchable_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_touchable_info_t *)userdata;

	info->body->touchable = info->touchable;

	UI_FREE(info);
}

/**
 * @brief Recursive function to find most top touchable widget by coordinates
 */
ui_widget_body_t *ui_widget_search_by_coord(ui_widget_body_t *widget, ui_coord_t coord)
{
	ui_widget_body_t *result = UI_NULL;
	ui_widget_body_t *found = UI_NULL;
	ui_widget_body_t *child;
	int iter;

	if (!widget) {
		UI_LOGE("Error: widget is null!\n");
		return UI_NULL;
	}

	if (ui_coord_inside_rect(coord, widget->global_rect) && widget->visible) {
		if (widget->touchable) {
			result = widget;
		}

		vec_foreach(&widget->children, child, iter) {
			found = ui_widget_search_by_coord(child, coord);

			if (found != UI_NULL && found->touchable) {
				result = found;
			}
		}
	} else {
		return UI_NULL;
	}

	return result;
}

#endif // CONFIG_UI_ENABLE_TOUCH

ui_widget_t ui_widget_create(int32_t width, int32_t height)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_widget_body_t *)UI_ALLOC(sizeof(ui_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_widget_body_t));
	ui_widget_init(body, width, height);
	((ui_widget_body_t *)body)->type = UI_EMPTY_WIDGET;

	body->visible = true;

	return (ui_widget_t)body;
}

ui_error_t ui_widget_destroy(ui_widget_t widget)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	body = (ui_widget_body_t *)widget;
	body->tick_cb = NULL;
	body->interval_cb = NULL;

	if (ui_request_callback(_ui_widget_destroy_func, body) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_destroy_func(void *userdata)
{
	int iter;
	ui_widget_body_t *widget;
	ui_widget_body_t *child;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	widget = (ui_widget_body_t *)userdata;

	ui_widget_queue_init();
	ui_widget_queue_enqueue(widget);

	while (!ui_widget_is_queue_empty()) {
		widget = ui_widget_queue_dequeue();
		if (!widget) {
			UI_LOGE("error: curr widget is NULL!\n");
			break;
		}

		if (widget->remove_cb) {
			widget->remove_cb((ui_widget_t)widget);
		}

		if (widget->parent) {
			vec_remove(&widget->parent->children, widget);
		}


		// In the below function, An item of the widget->children will be deleted.
		// So this while loop will be finished when the all items are deleted.
		vec_foreach(&widget->children, child, iter) {
			ui_widget_queue_enqueue(child);
		}

		ui_widget_deinit(widget);

		UI_FREE(widget);
	}
}

ui_error_t ui_widget_add_child(ui_widget_t widget, ui_widget_t child, int32_t x, int32_t y)
{
	ui_add_child_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !child) {
		return UI_INVALID_PARAM;
	}

	info = (ui_add_child_info_t *)UI_ALLOC(sizeof(ui_add_child_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->child = (ui_widget_body_t *)child;
	info->x = x;
	info->y = y;
	info->child->parent = info->body;

	if (ui_request_callback(_ui_widget_add_child_func, (void *)info) != UI_OK) {
		UI_FREE(info);
		info->child->parent = UI_NULL;
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

ui_widget_t ui_widget_get_parent(ui_widget_t widget)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI Framework is not running!\n");
		return UI_NULL;
	}

	if (!widget) {
		return UI_NULL;
	}

	body = (ui_widget_body_t *)widget;

	return (ui_widget_t)(body->parent);
}

static void _ui_widget_add_child_func(void *userdata)
{
	ui_add_child_info_t *info;
	ui_widget_body_t *child;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	info = (ui_add_child_info_t *)userdata;

	child = info->child;

	child->parent = info->body;
	child->local_rect.x = info->x;
	child->local_rect.y = info->y;

	vec_push(&info->body->children, child);

	ui_widget_update_position_info(child);

	UI_FREE(info);
}

ui_error_t ui_widget_remove_child(ui_widget_t widget, ui_widget_t child)
{
	ui_remove_child_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !child) {
		return UI_INVALID_PARAM;
	}

	info = (ui_remove_child_info_t *)UI_ALLOC(sizeof(ui_remove_child_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->child = (ui_widget_body_t *)child;

	if (ui_request_callback(_ui_widget_remove_child_func, (void *)info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_remove_child_func(void *userdata)
{
	ui_remove_child_info_t *info;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	info = (ui_remove_child_info_t *)userdata;

	vec_remove(&info->body->children, info->child);

	info->child->parent = NULL;
#if defined(CONFIG_UI_PARTIAL_UPDATE)
	ui_window_add_redraw_list(info->child->global_rect);
#endif

	UI_FREE(info);
}

ui_error_t ui_widget_remove_all_children(ui_widget_t widget)
{
	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_widget_remove_all_children_func, (void *)widget) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_remove_all_children_func(void *widget)
{
	ui_widget_body_t *body;
	ui_widget_body_t *child;
	int iter;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_widget_body_t *)widget;

	vec_foreach(&body->children, child, iter) {
		child->parent = NULL;
#if defined(CONFIG_UI_PARTIAL_UPDATE)
		ui_window_add_redraw_list(child->global_rect);
#endif
	}
	vec_clear(&body->children);
}

void ui_widget_init(ui_widget_body_t *body, int32_t width, int32_t height)
{
	if (!body) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body->visible = true;
	body->local_rect.width = width;
	body->local_rect.height = height;
	body->scale_x = 1.0f;
	body->scale_y = 1.0f;
	body->trans_mat = ui_mat3_identity();

	vec_init(&body->children);
}

void ui_widget_deinit(ui_widget_body_t *body)
{
	if (body) {
		vec_deinit(&body->children);
	}
}

ui_error_t ui_widget_destroy_sync(ui_widget_body_t *body)
{
	if (!body) {
		return UI_INVALID_PARAM;
	}

	body->tick_cb = NULL;
	body->interval_cb = NULL;

	_ui_widget_destroy_func((void *)body);

	return UI_OK;
}

ui_error_t ui_widget_set_pivot_point(ui_widget_t widget, int32_t x, int32_t y)
{
	ui_set_pivot_point_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_pivot_point_info_t *)UI_ALLOC(sizeof(ui_set_pivot_point_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->x = x;
	info->y = y;

	if (ui_request_callback(_ui_widget_set_pivot_point_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_pivot_point_func(void *userdata)
{
	ui_set_pivot_point_info_t *info;

	info = (ui_set_pivot_point_info_t *)userdata;

	info->body->pivot_x = info->x;
	info->body->pivot_y = info->y;

	info->body->update_flag = true;

	UI_FREE(info);
}

ui_error_t ui_widget_set_scale(ui_widget_t widget, float scale_x, float scale_y)
{
	ui_set_scale_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || scale_x <= 0 || scale_y <= 0) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_scale_info_t *)UI_ALLOC(sizeof(ui_set_scale_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->scale_x = scale_x;
	info->scale_y = scale_y;

	if (ui_request_callback(_ui_widget_set_scale_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_scale_func(void *userdata)
{
	ui_set_scale_info_t *info;

	info = (ui_set_scale_info_t *)userdata;

	info->body->scale_x = info->scale_x;
	info->body->scale_y = info->scale_y;

	info->body->update_flag = true;

	UI_FREE(info);
}

ui_error_t ui_widget_set_rotation(ui_widget_t widget, int32_t degree)
{
	ui_rotate_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_rotate_info_t *)UI_ALLOC(sizeof(ui_rotate_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->degree = degree;

	if (ui_request_callback(_ui_widget_set_rotation_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_rotation_func(void *userdata)
{
	ui_rotate_info_t *info;

	info = (ui_rotate_info_t *)userdata;

	info->body->degree = info->degree;

	info->body->update_flag = true;

	UI_FREE(info);
}

ui_error_t ui_widget_set_userdata(ui_widget_t widget, void *userdata)
{
	ui_widget_body_t *body = (ui_widget_body_t *)widget;

	if (!body) {
		return UI_INVALID_PARAM;
	}

	body->userdata = userdata;

	return UI_OK;
}

void *ui_widget_get_userdata(ui_widget_t widget)
{
	ui_widget_body_t *body = (ui_widget_body_t *)widget;

	if (!body) {
		return NULL;
	}

	return body->userdata;
}

ui_error_t ui_widget_play_anim(ui_widget_t widget, ui_anim_t anim, anim_finished_callback anim_finished_cb, bool loop)
{
	ui_set_anim_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !anim) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_anim_info_t *)UI_ALLOC(sizeof(ui_set_anim_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_anim_info_t));
	info->body = (ui_widget_body_t *)widget;
	info->anim = (ui_anim_t *)anim;
	info->anim_finished_cb = anim_finished_cb;

	if (ui_request_callback(_ui_widget_play_anim, info)) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_play_anim(void *userdata)
{
	ui_set_anim_info_t *info;
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_anim_info_t *)userdata;
	body = (ui_widget_body_t *)info->body;

	body->anim_finished_cb = info->anim_finished_cb;
	body->anim = info->anim;
}

void ui_widget_update_global_rect(ui_widget_body_t *widget)
{
	ui_vec3_t vertex[4];

	vertex[0].x = UI_GET_TRANS_X(widget, - widget->pivot_x, - widget->pivot_y);
	vertex[0].y = UI_GET_TRANS_Y(widget, - widget->pivot_x, - widget->pivot_y);
	vertex[1].x = UI_GET_TRANS_X(widget, - widget->pivot_x + widget->local_rect.width, - widget->pivot_y);
	vertex[1].y = UI_GET_TRANS_Y(widget, - widget->pivot_x + widget->local_rect.width, - widget->pivot_y);
	vertex[2].x = UI_GET_TRANS_X(widget, - widget->pivot_x, - widget->pivot_y + widget->local_rect.height);
	vertex[2].y = UI_GET_TRANS_Y(widget, - widget->pivot_x, - widget->pivot_y + widget->local_rect.height);
	vertex[3].x = UI_GET_TRANS_X(widget, - widget->pivot_x + widget->local_rect.width, - widget->pivot_y + widget->local_rect.height);
	vertex[3].y = UI_GET_TRANS_Y(widget, - widget->pivot_x + widget->local_rect.width, - widget->pivot_y + widget->local_rect.height);

	widget->global_rect.x = (int32_t)UI_MIN4(vertex[0].x, vertex[1].x, vertex[2].x, vertex[3].x);
	widget->global_rect.y = (int32_t)UI_MIN4(vertex[0].y, vertex[1].y, vertex[2].y, vertex[3].y);
	widget->global_rect.width = (int32_t)UI_MAX4(vertex[0].x, vertex[1].x, vertex[2].x, vertex[3].x) - widget->global_rect.x;
	widget->global_rect.height = (int32_t)UI_MAX4(vertex[0].y, vertex[1].y, vertex[2].y, vertex[3].y) - widget->global_rect.y;
}

ui_error_t ui_widget_set_position_sync(ui_widget_body_t *body, int32_t x, int32_t y)
{
	if (!body) {
		return UI_INVALID_PARAM;
	}

	body->local_rect.x = x;
	body->local_rect.y = y;
	body->update_flag = true;

	return UI_OK;
}

ui_error_t ui_widget_set_rotation_sync(ui_widget_body_t *body, int32_t degree)
{
	if (!body) {
		return UI_INVALID_PARAM;
	}

	body->degree = degree;
	body->update_flag = true;

	return UI_OK;
}

ui_error_t ui_widget_set_scale_sync(ui_widget_body_t *body, uint32_t scale_x, uint32_t scale_y)
{
	if (!body) {
		return UI_INVALID_PARAM;
	}

	body->scale_x = scale_x;
	body->scale_y = scale_y;
	body->update_flag = true;

	return UI_OK;
}

void ui_widget_queue_init(void)
{
	g_widget_body_queue.start = g_widget_body_queue.end = 0;
}

bool ui_widget_is_queue_empty(void)
{
	if (g_widget_body_queue.start == g_widget_body_queue.end) {
		return true;
	}

	return false;
}

void ui_widget_queue_enqueue(ui_widget_body_t *body)
{
	g_widget_body_queue.queue[g_widget_body_queue.end++] = body;
}

ui_widget_body_t *ui_widget_queue_dequeue(void)
{
	return g_widget_body_queue.queue[g_widget_body_queue.start++];
}

