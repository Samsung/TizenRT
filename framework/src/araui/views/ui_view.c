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
#include <pthread.h>
#include <araui/ui_widget.h>
#include <araui/ui_view.h>
#include <araui/ui_commons.h>
#include "libs/ui_list.h"
#include "ui_log.h"
#include "ui_core_internal.h"
#include "ui_view_internal.h"
#include "ui_request_callback.h"
#include "ui_commons_internal.h"
#include "hal/ui_hal_interface.h"

#define UI_VIEW_SET_STATE(v, s)                                 \
	do {                                                        \
		if (v) {                                                \
			pthread_mutex_lock(&g_view_list.mutex);             \
			(v)->state = s;                                     \
			if (s == UI_VIEW_STATE_SHOWN) {                     \
				g_current_view = v;                              \
			}                                                   \
			pthread_mutex_unlock(&g_view_list.mutex);           \
			if ((v)->state_changed_cb[s]) {                     \
				(v)->state_changed_cb[s]((ui_view_t)v);         \
			}                                                   \
		}                                                       \
	} while (false)

typedef struct {
	pthread_mutex_t mutex;
	ui_list_t list;
	ui_list_node_t *iterator;
} ui_view_list_t;

// Update list is accessed by only UI Core Service, so it doesn't need a mutex.
typedef struct {
	ui_list_t list;
	ui_list_node_t *iterator;
} ui_view_update_list_t;

typedef struct {
	ui_view_body_t *cur_view;
	ui_view_body_t *next_view;
	ui_transit_type_t type;
	uint32_t duration;
} ui_view_transit_info_t;

static ui_view_list_t g_view_list;
static ui_view_update_list_t g_view_update_list;
static ui_rect_t g_rect_mempool[CONFIG_UI_UPDATE_MEMPOOL_SIZE];
static int g_rect_mempool_idx = 0;
static ui_view_body_t *g_current_view = UI_NULL;

static void _ui_view_create_func(void *userdata);
static void _ui_view_destroy_func(void *userdata);
static ui_rect_t *_ui_view_get_mempool_rect(void);
static void _ui_view_transit_func(void *userdata);

ui_error_t ui_view_list_init(void)
{
	if (pthread_mutex_init(&g_view_list.mutex, NULL)) {
		return UI_INIT_FAILURE;
	}

	pthread_mutex_lock(&g_view_list.mutex);

	if (ui_list_init(&g_view_list.list) != UI_OK) {
		pthread_mutex_unlock(&g_view_list.mutex);
		return UI_INIT_FAILURE;
	}

	pthread_mutex_unlock(&g_view_list.mutex);

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

ui_error_t ui_view_list_deinit(void)
{
	ui_view_body_t *view = ui_view_list_begin();

	while (view) {
		// This callback function is always called at the core thread.
		// Therefore views have to be deleted by synchronized call.
		// (_ui_view_destroy_func() is sync func)
		// If not, views are not deleted.
		_ui_view_destroy_func((void *)view);
		view = ui_view_list_begin();
	}

	pthread_mutex_lock(&g_view_list.mutex);

	if (ui_list_deinit(&g_view_list.list) != UI_OK) {
		pthread_mutex_unlock(&g_view_list.mutex);
		return UI_DEINIT_FAILURE;
	}

	pthread_mutex_unlock(&g_view_list.mutex);

	if (pthread_mutex_destroy(&g_view_list.mutex)) {
		return UI_DEINIT_FAILURE;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

ui_error_t ui_view_update_list_init(void)
{
	if (ui_list_init(&g_view_update_list.list) != UI_OK) {
		return UI_INIT_FAILURE;
	}

	return UI_OK;
}

ui_error_t ui_view_update_list_deinit(void)
{
	if (ui_list_deinit(&g_view_update_list.list) != UI_OK) {
		return UI_DEINIT_FAILURE;
	}

	return UI_OK;
}

ui_view_t ui_view_create(created_callback created_cb, destroyed_callback destroyed_cb, shown_callback shown_cb, hidden_callback hidden_cb)
{
	ui_view_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!created_cb) {
		UI_LOGE("error: invalid parameter!\n");
		return UI_NULL;
	}

	body = (ui_view_body_t *)UI_ALLOC(sizeof(ui_view_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_view_body_t));

	body->root = (ui_widget_body_t *)ui_widget_create(CONFIG_UI_DISPLAY_WIDTH, CONFIG_UI_DISPLAY_HEIGHT);
	body->focus = body->root;
	body->state_changed_cb[UI_VIEW_STATE_CREATED] = created_cb;
	body->state_changed_cb[UI_VIEW_STATE_SHOWN] = shown_cb;
	body->state_changed_cb[UI_VIEW_STATE_HIDDEN] = hidden_cb;
	body->state_changed_cb[UI_VIEW_STATE_DESTROYED] = destroyed_cb;

	if (ui_request_callback(_ui_view_create_func, body) != UI_OK) {
		UI_FREE(body);
		UI_LOGE("error: failed to request!\n");
		return UI_NULL;
	}

	UI_LOGD("UI_OK.\n");

	return (ui_view_t)body;
}

static void _ui_view_create_func(void *userdata)
{
	ui_view_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_view_body_t *)userdata;

	pthread_mutex_lock(&g_view_list.mutex);

	if (ui_list_push_back(&g_view_list.list, body) != UI_OK) {
		pthread_mutex_unlock(&g_view_list.mutex);
		UI_LOGE("error: Failed to push back!\n");
		return;
	}

	pthread_mutex_unlock(&g_view_list.mutex);

	UI_VIEW_SET_STATE(body, UI_VIEW_STATE_CREATED);

	ui_widget_update_position_info(body->root);

	if (!ui_view_get_current()) {
		g_current_view = body;
	}
}

ui_error_t ui_view_destroy(ui_view_t view)
{
	ui_view_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!view) {
		return UI_INVALID_PARAM;
	}

	body = (ui_view_body_t *)view;

	if (ui_request_callback(_ui_view_destroy_func, body) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_view_destroy_func(void *userdata)
{
	ui_view_body_t *body;
	ui_view_body_t *next_view;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_view_body_t *)userdata;

	pthread_mutex_lock(&g_view_list.mutex);

	if (ui_list_remove(&g_view_list.list, body) != UI_OK) {
		pthread_mutex_unlock(&g_view_list.mutex);
		UI_LOGE("error: Failed to remove!\n");
		return;
	}

	pthread_mutex_unlock(&g_view_list.mutex);

	UI_VIEW_SET_STATE(body, UI_VIEW_STATE_HIDDEN);
	UI_VIEW_SET_STATE(body, UI_VIEW_STATE_DESTROYED);

	next_view = ui_list_front(&g_view_list.list);
	if (next_view) {
		UI_VIEW_SET_STATE(next_view, UI_VIEW_STATE_SHOWN);
	} else {
		g_current_view = UI_NULL;
	}

	// This callback function is called when request callback process phase.
	// Therefore the widget should be deleted by synchronized call.
	// If not, the widget will not be deleted for some cases.
	if (ui_widget_destroy_sync(body->root) != UI_OK) {
		UI_LOGE("error: Failed to destroy the widget!\n");
	}

	UI_FREE(body);
}

ui_widget_t ui_view_get_root(ui_view_t view)
{
	if (!view) {
		UI_LOGE("error: invalied parameter!\n");
		return UI_NULL;
	}

	return (ui_widget_t)(((ui_view_body_t *)view)->root);
}

ui_error_t ui_view_add_widget(ui_view_t view, ui_widget_t widget, int32_t x, int32_t y)
{
	ui_widget_t root;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!view || !widget) {
		return UI_INVALID_PARAM;
	}

	root = ui_view_get_root(view);
	if (!root) {
		UI_LOGE("error: failed to get root of view!\n");
		return UI_OPERATION_FAIL;
	}

	if (ui_widget_add_child(root, widget, x, y) != UI_OK) {
		UI_LOGE("error: failed to set the root as a parent!\n");
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

ui_view_body_t *ui_view_list_begin(void)
{
	ui_view_body_t *result = UI_NULL;

	pthread_mutex_lock(&g_view_list.mutex);

	if (ui_list_empty(&g_view_list.list)) {
		pthread_mutex_unlock(&g_view_list.mutex);
		return UI_NULL;
	}

	g_view_list.iterator = ui_list_loop_begin(&g_view_list.list);

	result = (ui_view_body_t *)g_view_list.iterator->data;

	pthread_mutex_unlock(&g_view_list.mutex);

	return result;
}

ui_view_body_t *ui_view_list_next(void)
{
	ui_view_body_t *result = UI_NULL;

	pthread_mutex_lock(&g_view_list.mutex);

	g_view_list.iterator = g_view_list.iterator->next;

	if (g_view_list.iterator == ui_list_loop_end(&g_view_list.list)) {
		g_view_list.iterator = UI_NULL;
		pthread_mutex_unlock(&g_view_list.mutex);
		return UI_NULL;
	}

	result = (ui_view_body_t *)g_view_list.iterator->data;

	pthread_mutex_unlock(&g_view_list.mutex);

	return result;
}

ui_rect_t *ui_view_update_list_begin(void)
{
	if (ui_list_empty(&g_view_update_list.list)) {
		return UI_NULL;
	}

	g_view_update_list.iterator = ui_list_loop_begin(&g_view_update_list.list);

	return (ui_rect_t *)g_view_update_list.iterator->data;
}

ui_rect_t *ui_view_update_list_next(void)
{
	g_view_update_list.iterator = g_view_update_list.iterator->next;

	if (g_view_update_list.iterator == ui_list_loop_end(&g_view_update_list.list)) {
		g_view_update_list.iterator = UI_NULL;
		return UI_NULL;
	}

	return (ui_rect_t *)(g_view_update_list.iterator->data);
}

ui_error_t ui_view_add_update_list(ui_rect_t update)
{
	ui_rect_t *curr;
	ui_rect_t *next;
	ui_rect_t *new_area;
	ui_rect_t previous;
	ui_rect_t ret;

	if (update.x < 0) {
		update.width += update.x;
		update.x = 0;
	} else if (update.x >= CONFIG_UI_DISPLAY_WIDTH) {
		return UI_OK;
	}

	if (update.y < 0) {
		update.height += update.y;
		update.y = 0;
	} else if (update.y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return UI_OK;
	}

	if (update.width <= 0 || update.height <= 0) {
		return UI_OK;
	}

	new_area = _ui_view_get_mempool_rect();

	new_area->x = update.x;
	new_area->y = update.y;
	new_area->width = update.width;
	new_area->height = update.height;

	curr = (ui_rect_t *)ui_view_update_list_begin();
	while (curr) {
		// curr is whole screen case
		if (curr->x == 0 && curr->y == 0 && curr->width == CONFIG_UI_DISPLAY_WIDTH && curr->height == CONFIG_UI_DISPLAY_HEIGHT) {
			return UI_OK;
		}

		previous.x = curr->x;
		previous.y = curr->y;
		previous.width = curr->width;
		previous.height = curr->height;

		ret = ui_rect_intersect(previous, update);
		if (ret.x == 0 && ret.y == 0 && ret.width == 0 && ret.height == 0 ) {
			curr = ui_view_update_list_next();
			continue;
		}

		ret = ui_get_contain_rect(previous, update);
		new_area->x = ret.x;
		new_area->y = ret.y;
		if (ret.x + ret.width > CONFIG_UI_DISPLAY_WIDTH) {
			ret.width = CONFIG_UI_DISPLAY_WIDTH - ret.x;
		}
		if (ret.y + ret.height > CONFIG_UI_DISPLAY_HEIGHT) {
			ret.height = CONFIG_UI_DISPLAY_HEIGHT - ret.y;
		}
		new_area->width = ret.width;
		new_area->height = ret.height;

		next = ui_view_update_list_next();

		if (ui_list_remove(&g_view_update_list.list, (void *)curr) != UI_OK) {
			return UI_OPERATION_FAIL;
		}

		curr = next;
	}

	if (ui_list_push_back(&g_view_update_list.list, (void *)new_area) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

ui_error_t ui_view_update_list_clear(void)
{
	while (!ui_list_empty(&g_view_update_list.list)) {
		(void)ui_list_remove_front(&g_view_update_list.list);
	}

	g_view_update_list.iterator = UI_NULL;

	return UI_OK;
}

static ui_rect_t *_ui_view_get_mempool_rect(void)
{
	int alloc_idx = g_rect_mempool_idx;

	g_rect_mempool_idx++;
	if (g_rect_mempool_idx >= CONFIG_UI_UPDATE_MEMPOOL_SIZE) {
		g_rect_mempool_idx = 0;
	}

	return &g_rect_mempool[alloc_idx];
}

ui_view_body_t *ui_view_get_current(void)
{
	return g_current_view;
}

ui_error_t ui_view_transit(ui_view_t next_view, ui_transit_type_t type, uint32_t duration)
{
	ui_transit_type_t transit_type;
	ui_view_transit_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!next_view) {
		return UI_INVALID_PARAM;
	}

	switch (type) {
		case UI_TRANSITION_FADE_OUT:
			transit_type = type;
			break;
		// Unsupported transition option
		case UI_TRANSITION_FADE_IN:
		default:
			return UI_INVALID_PARAM;
	}

	info = (ui_view_transit_info_t *)UI_ALLOC(sizeof(ui_view_transit_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->cur_view = ui_view_get_current();
	info->next_view = (ui_view_body_t *)next_view;
	info->type = transit_type;
	info->duration = duration;

	if (ui_request_callback(_ui_view_transit_func, (void *)info) != UI_OK) {
		UI_FREE(info);
		UI_LOGE("error: failed to request!\n");
		return UI_NULL;
	}

	return UI_OK;
}

static void _ui_view_transit_fade_out_cb(ui_view_t view, uint32_t dt)
{
	ui_view_body_t *cur_view_body;
	ui_view_body_t *next_view_body;
	int draw_x;
	int draw_y;
	ui_color_t color;

	// This implementation is very temporary.
	// The logic will be updated soon.

	cur_view_body = (ui_view_body_t *)view;
	next_view_body = (ui_view_body_t *)cur_view_body->transit_info.next_view;

	if (cur_view_body->transit_info.time < cur_view_body->transit_info.duration) {
		if (next_view_body) {
			color = (uint32_t)((256.0f * cur_view_body->transit_info.time) / (cur_view_body->transit_info.duration));
			for (draw_x = 0; draw_x < CONFIG_UI_DISPLAY_WIDTH; draw_x++) {
				for (draw_y = 0; draw_y < CONFIG_UI_DISPLAY_HEIGHT; draw_y++) {
					ui_hal_draw_pixel(draw_x, draw_y, color);
				}
			}
		} else {
			// To do
		}
		cur_view_body->transit_info.time += dt;
		ui_view_add_update_list(cur_view_body->root->global_rect);
	} else {
		cur_view_body->transit_info.next_view = NULL;
		cur_view_body->transit_info.transit_cb = NULL;
		cur_view_body->transit_info.duration = 0;
		cur_view_body->transit_info.time = 0;

		UI_VIEW_SET_STATE(cur_view_body, UI_VIEW_STATE_HIDDEN);
		UI_VIEW_SET_STATE(next_view_body, UI_VIEW_STATE_SHOWN);
		ui_view_add_update_list(cur_view_body->root->global_rect);
	}
}

static void _ui_view_transit_func(void *userdata)
{
	ui_view_transit_info_t *info;
	view_transit_callback transit_cb;
	ui_view_body_t *body;

	if (!userdata) {
		UI_LOGE("error: invalid parameter!\n");
		return;
	}

	info = (ui_view_transit_info_t *)userdata;

	switch (info->type) {
		case UI_TRANSITION_FADE_OUT:
			transit_cb = _ui_view_transit_fade_out_cb;
			break;
		// Unsupported transition option
		case UI_TRANSITION_FADE_IN:
		default:
			return;
	}

	body = info->cur_view;
	memset(&body->transit_info, 0, sizeof(ui_view_transit_t));
	body->transit_info.next_view = info->next_view;
	body->transit_info.transit_cb = transit_cb;
	body->transit_info.duration = info->duration;

	UI_FREE(info);
}
