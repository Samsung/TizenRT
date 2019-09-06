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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <vec/vec.h>
#include <araui/ui_commons.h>
#include <araui/ui_widget.h>
#include <araui/ui_animation.h>
#include "ui_log.h"
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
	uint32_t duration;
	ui_tween_type_t type;
	tween_finished_callback tween_finished_cb;
} ui_tween_moveto_info_t;

typedef struct {
	ui_widget_body_t *body;
	ui_align_t align;
} ui_align_info_t;

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
} ui_set_anim_info_t;

static void _ui_widget_set_visible_func(void *userdata);
static void _ui_widget_set_position_func(void *userdata);
static void _ui_widget_set_size_func(void *userdata);
static void _ui_widget_tween_moveto_func(void *userdata);
static void _ui_widget_tween_move_func(ui_widget_t widget, uint32_t t);
static void _ui_widget_play_anim(void *userdata);
static bool _ui_widget_play_anim_func(ui_widget_t widget, uint32_t t);
static void _ui_widget_stop_anim(void *userdata);
static void _ui_widget_pause_anim(void *userdata);
static void _ui_widget_resume_anim(void *userdata);

static void _ui_widget_destroy_func(void *userdata);
static void _ui_widget_add_child_func(void *userdata);
static void _ui_widget_remove_child_func(void *userdata);
static void _ui_widget_remove_all_children_func(void *widget);
static void _ui_widget_set_align_func(void *widget);
static void _ui_widget_set_pivot_point_func(void *userdata);
static void _ui_widget_set_scale_func(void *userdata);
static void _ui_widget_set_rotation_func(void *userdata);

#if defined(CONFIG_UI_ENABLE_TOUCH)
static void _ui_widget_set_touchable_func(void *userdata);
#endif

static void _ui_widget_destroy_recur(ui_widget_body_t *widget);

static const ui_rect_t NULL_RECT = {0, };

inline bool ui_widget_check_widget_type(ui_widget_t widget, ui_widget_type_t type)
{
	return (((ui_widget_body_t *)widget)->type == type);
}

ui_error_t ui_widget_update_position_info(ui_widget_body_t *widget)
{
	ui_widget_body_t *parent;
	int32_t x;
	int32_t y;
	ui_widget_body_t *child;
	int iter;

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	// Add update region to clear previously located region
	if (ui_window_add_update_list(widget->crop_rect) != UI_OK) {
		UI_LOGE("error: failed to add update list!\n");
		return UI_OPERATION_FAIL;
	}

	widget->global_rect.width = widget->local_rect.width;
	widget->global_rect.height = widget->local_rect.height;

	if (widget->parent) {
		x = widget->local_rect.x + widget->parent->global_rect.x;
		y = widget->local_rect.y + widget->parent->global_rect.y;

		if (widget->align & UI_ALIGN_CENTER) {
			x = x + ((widget->parent->global_rect.width - widget->global_rect.width) >> 1);
		} else if (widget->align & UI_ALIGN_RIGHT) {
			x = x + (widget->parent->global_rect.width - widget->global_rect.width);
		}

		if (widget->align & UI_ALIGN_MIDDLE) {
			y = y + ((widget->parent->global_rect.height - widget->global_rect.height) >> 1);
		} else if (widget->align & UI_ALIGN_BOTTOM) {
			y = y + (widget->parent->global_rect.height - widget->global_rect.height);
		}
	} else {
		x = widget->local_rect.x;
		y = widget->local_rect.y;
	}

	widget->global_rect.x = x;
	widget->global_rect.y = y;
	widget->crop_rect = widget->global_rect;

	parent = widget->parent;
	while (parent) {
		widget->crop_rect = ui_rect_intersect(widget->crop_rect, parent->crop_rect);
		parent = parent->parent;
	}

	// Add update region to present newly located region
	if (ui_window_add_update_list(widget->crop_rect) != UI_OK) {
		UI_LOGE("error: failed to add update list!\n");
		return UI_OPERATION_FAIL;
	}

	vec_foreach(&widget->children, child, iter) {
		ui_widget_update_position_info(child);
	}

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

	if (ui_window_add_update_list(body->global_rect) != UI_OK) {
		UI_LOGE("error: failed to add to the update list!\n");
	}

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
	info->body->mat_update_flag = true;

	ui_widget_update_position_info(body);

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

ui_error_t ui_widget_tween_moveto(ui_widget_t widget, int32_t x, int32_t y,
	uint32_t duration, ui_tween_type_t type, tween_finished_callback tween_finished_cb)
{
	ui_tween_moveto_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_tween_moveto_info_t *)UI_ALLOC(sizeof(ui_tween_moveto_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->x = x;
	info->y = y;
	info->duration = duration;
	info->type = type;
	info->tween_finished_cb = tween_finished_cb;

	if (ui_request_callback(_ui_widget_tween_moveto_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_tween_moveto_func(void *userdata)
{
	ui_tween_moveto_info_t *info;
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	info = (ui_tween_moveto_info_t *)userdata;
	body = (ui_widget_body_t *)info->body;

	body->tween_cb = _ui_widget_tween_move_func;

	memset(&info->body->tween_info, 0, sizeof(tween_info_t));
	info->body->tween_info.origin = ui_widget_get_rect((ui_widget_t)body);
	info->body->tween_info.gap.x = info->x - info->body->tween_info.origin.x;
	info->body->tween_info.gap.y = info->y - info->body->tween_info.origin.y;
	info->body->tween_info.d = info->duration;
	info->body->tween_info.tween_finished_cb = info->tween_finished_cb;

	switch (info->type) {
	case TWEEN_EASE_IN_QUAD:
		body->tween_info.easing_cb = ease_in_quad;
		break;

	case TWEEN_EASE_OUT_QUAD:
		body->tween_info.easing_cb = ease_out_quad;
		break;

	case TWEEN_EASE_INOUT_QUAD:
		body->tween_info.easing_cb = ease_inout_quad;
		break;

	default:
		body->tween_info.easing_cb = ease_linear;
		break;
	}

	UI_FREE(info);
}

static void _ui_widget_tween_move_func(ui_widget_t widget, uint32_t t)
{
	ui_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_widget_body_t *)widget;

	body->local_rect.x = (int32_t)body->tween_info.easing_cb(t,
		body->tween_info.origin.x,
		body->tween_info.gap.x,
		body->tween_info.d);

	body->local_rect.y = (int32_t)body->tween_info.easing_cb(t,
		body->tween_info.origin.y,
		body->tween_info.gap.y,
		body->tween_info.d);

	ui_widget_update_position_info(body);
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

	if (ui_coord_inside_rect(coord, widget->crop_rect) && widget->visible) {
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
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_widget_body_t *)userdata;

	_ui_widget_destroy_recur(body);
}

static void _ui_widget_destroy_recur(ui_widget_body_t *widget)
{
	ui_widget_body_t *child;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	if (widget->remove_cb) {
		widget->remove_cb((ui_widget_t)widget);
	}

	if (widget->parent) {
		vec_remove(&widget->parent->children, widget);
	}

	while (widget->children.length) {
		child = vec_first(&widget->children);

		// In the below function, An item of the widget->children will be deleted.
		// So this while loop will be finished when the all items are deleted.
		_ui_widget_destroy_recur(child);
	}

	ui_widget_deinit(widget);

	UI_FREE(widget);
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
	ui_window_add_update_list(info->child->global_rect);

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
		ui_window_add_update_list(child->global_rect);
	}
	vec_clear(&body->children);
}

void ui_widget_init(ui_widget_body_t *body, int32_t width, int32_t height)
{
	float id_mat[3][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};

	if (!body) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body->visible = true;
	body->local_rect.width = width;
	body->local_rect.height = height;
	body->scale_x = 1.0f;
	body->scale_y = 1.0f;
	ui_matrix_copy(body->trans_mat, id_mat);
	ui_matrix_copy(body->inverse_mat, id_mat);

	vec_init(&body->children);
}

void ui_widget_deinit(ui_widget_body_t *body)
{
	if (body) {
		vec_deinit(&body->children);
	}
}

ui_error_t ui_widget_set_align(ui_widget_t widget, ui_align_t align)
{
	ui_align_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	info = (ui_align_info_t *)UI_ALLOC(sizeof(ui_align_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_widget_body_t *)widget;
	info->align = align;

	if (ui_request_callback(_ui_widget_set_align_func, (void *)info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_widget_set_align_func(void *userdata)
{
	ui_align_info_t *info;

	info = (ui_align_info_t *)userdata;

	info->body->align = info->align;

	if (ui_widget_update_position_info(info->body) != UI_OK) {
		UI_LOGE("error: failed to update position information!\n");
		UI_FREE(info);
		return;
	}

	UI_FREE(info);
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
	info->body->mat_update_flag = true;

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
	info->body->mat_update_flag = true;

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

ui_error_t ui_widget_play_anim(ui_widget_t widget, ui_anim_t anim, bool loop)
{
	ui_set_anim_info_t *info;

	if (!widget) {
		return UI_INVALID_PARAM;
	}
	
	if (!anim) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_anim_info_t *)UI_ALLOC(sizeof(ui_set_anim_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_anim_info_t));
	info->body = (ui_widget_body_t *)widget;
	info->anim = (ui_anim_t *)anim;

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

	if (!userdata || !((ui_set_anim_info_t *)userdata)->body || !((ui_set_anim_info_t *)userdata)->anim) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_anim_info_t *)userdata;
	body = (ui_widget_body_t *)info->body;

	body->anim_cb = _ui_widget_play_anim_func;
	
	body->anim = info->anim;
}

static bool _ui_widget_play_anim_func(ui_widget_t widget, uint32_t t)
{
	ui_widget_body_t *body;
	ui_anim_body_t *anim;
	ui_anim_t *item;
	uint32_t _t = t;

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	body = (ui_widget_body_t *)widget;
	anim = (ui_anim_body_t *)body->anim;

	return anim->func(widget, body->anim, &_t);
}

ui_error_t ui_widget_stop_anim(ui_widget_t widget)
{

}

static void _ui_widget_stop_anim(void *userdata)
{

}

ui_error_t ui_widget_pause_anim(ui_widget_t widget)
{

}

static void _ui_widget_pause_anim(void *userdata)
{

}

ui_error_t ui_widget_resume_anim(ui_widget_t widget)
{

}

static void _ui_widget_resume_anim(void *userdata)
{

}
