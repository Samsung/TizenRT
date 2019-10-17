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
#include <araui/ui_window.h>
#include <araui/ui_commons.h>
#include "ui_debug.h"
#include "ui_core_internal.h"
#include "ui_window_internal.h"
#include "ui_request_callback.h"
#include "ui_commons_internal.h"

static vec_void_t g_window_list;
static ui_window_body_t *g_current_window = UI_NULL;
#if defined(CONFIG_UI_PARTIAL_UPDATE)
static vec_void_t g_window_redraw_list;
static ui_rect_t g_rect_mempool[CONFIG_UI_UPDATE_MEMPOOL_SIZE];
static int g_rect_mempool_idx = 0;
#endif

static void _ui_window_create_func(void *userdata);
static void _ui_window_destroy_func(void *userdata);
#if defined(CONFIG_UI_PARTIAL_UPDATE)
static ui_rect_t *_ui_window_get_mempool_rect(void);
#endif

ui_error_t ui_window_list_init(void)
{
	vec_init(&g_window_list);

	return UI_OK;
}

ui_error_t ui_window_list_deinit(void)
{
	ui_window_body_t *window;
	int iter;

	vec_foreach(&g_window_list, window, iter) {
		_ui_window_destroy_func((void *)window);
	}
	vec_deinit(&g_window_list);

	return UI_OK;
}

#if defined(CONFIG_UI_PARTIAL_UPDATE)
ui_error_t ui_window_redraw_list_init(void)
{
	vec_init(&g_window_redraw_list);

	return UI_OK;
}

ui_error_t ui_window_redraw_list_deinit(void)
{
	vec_deinit(&g_window_redraw_list);

	return UI_OK;
}
#endif

ui_window_t ui_window_create(created_callback created_cb, destroyed_callback destroyed_cb, shown_callback shown_cb, hidden_callback hidden_cb)
{
	ui_window_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!created_cb) {
		UI_LOGE("error: invalid parameter!\n");
		return UI_NULL;
	}

	body = (ui_window_body_t *)UI_ALLOC(sizeof(ui_window_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_window_body_t));

	body->root = (ui_widget_body_t *)ui_widget_create(CONFIG_UI_DISPLAY_WIDTH, CONFIG_UI_DISPLAY_HEIGHT);
	if (!body->root) {
		UI_FREE(body);
		UI_LOGE("error: create body root failed!\n");
		return UI_NULL;
	}

	body->focus = body->root;
	body->created_cb = created_cb;
	body->shown_cb = shown_cb;
	body->hidden_cb = hidden_cb;
	body->destroyed_cb = destroyed_cb;

	if (ui_request_callback(_ui_window_create_func, body) != UI_OK) {
		ui_widget_destroy_sync(body->root);
		UI_FREE(body);
		UI_LOGE("error: failed to request!\n");
		return UI_NULL;
	}

	return (ui_window_t)body;
}

static void _ui_window_create_func(void *userdata)
{
	ui_window_body_t *body;
	ui_window_body_t *prev_win;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_window_body_t *)userdata;

	if (g_window_list.length > 0) {
		prev_win = vec_last(&g_window_list);
		if (prev_win->hidden_cb) {
			prev_win->hidden_cb((ui_window_t)prev_win);
		}
	}

	vec_push(&g_window_list, body);

	ui_widget_update_position_info(body->root);

	if (body->created_cb) {
		body->created_cb((ui_window_t)body);
	}

	g_current_window = body;
}

ui_error_t ui_window_destroy(ui_window_t window)
{
	ui_window_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!window) {
		return UI_INVALID_PARAM;
	}

	body = (ui_window_body_t *)window;

	if (ui_request_callback(_ui_window_destroy_func, body) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_window_destroy_func(void *userdata)
{
	ui_window_body_t *body;
	ui_window_body_t *next_win;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_window_body_t *)userdata;

	vec_remove(&g_window_list, body);

	if (body->destroyed_cb) {
		body->destroyed_cb((ui_window_t)body);
	}

	if (g_window_list.length > 0) {
		next_win = vec_last(&g_window_list);
		if (next_win) {
			next_win->shown_cb((ui_window_t)next_win);
		}
		g_current_window = next_win;
	} else {
		g_current_window = NULL;
	}

	// This callback function is called when request callback process phase.
	// Therefore the widget should be deleted by synchronized call.
	// If not, the widget will not be deleted for some cases.
	if (ui_widget_destroy((ui_widget_t)(body->root)) != UI_OK) {
		UI_LOGE("error: Failed to destroy the widget!\n");
	}

	UI_FREE(body);
}

ui_widget_t ui_window_get_root(ui_window_t window)
{
	if (!window) {
		UI_LOGE("error: invalied parameter!\n");
		return UI_NULL;
	}

	return (ui_widget_t)(((ui_window_body_t *)window)->root);
}

ui_error_t ui_window_add_widget(ui_window_t window, ui_widget_t widget, int32_t x, int32_t y)
{
	ui_widget_t root;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!window || !widget) {
		return UI_INVALID_PARAM;
	}

	root = ui_window_get_root(window);
	if (!root) {
		UI_LOGE("error: failed to get root of window!\n");
		return UI_OPERATION_FAIL;
	}

	if (ui_widget_add_child(root, widget, x, y) != UI_OK) {
		UI_LOGE("error: failed to set the root as a parent!\n");
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

#if defined(CONFIG_UI_PARTIAL_UPDATE)
vec_void_t *ui_window_get_redraw_list(void)
{
	return &g_window_redraw_list;
}

ui_error_t ui_window_add_redraw_list(ui_rect_t redraw_rect)
{
	ui_rect_t *window;
	ui_rect_t *new_area;
	ui_rect_t previous;
	ui_rect_t ret;
	int iter;

	if (redraw_rect.x < 0) {
		redraw_rect.width += redraw_rect.x;
		redraw_rect.x = 0;
	} else if (redraw_rect.x >= CONFIG_UI_DISPLAY_WIDTH) {
		return UI_OK;
	}

	if (redraw_rect.y < 0) {
		redraw_rect.height += redraw_rect.y;
		redraw_rect.y = 0;
	} else if (redraw_rect.y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return UI_OK;
	}

	if (redraw_rect.width <= 0 || redraw_rect.height <= 0) {
		return UI_OK;
	}

	new_area = _ui_window_get_mempool_rect();

	new_area->x = redraw_rect.x;
	new_area->y = redraw_rect.y;
	new_area->width = redraw_rect.width;
	new_area->height = redraw_rect.height;

	if (new_area->x + new_area->width >= CONFIG_UI_DISPLAY_WIDTH) {
		new_area->width = CONFIG_UI_DISPLAY_WIDTH - new_area->x;
	}
	if (new_area->y + new_area->height >= CONFIG_UI_DISPLAY_HEIGHT) {
		new_area->height = CONFIG_UI_DISPLAY_HEIGHT - new_area->y;
	}

	vec_foreach(&g_window_redraw_list, window, iter) {
		// window is whole screen case
		if ((window->x == 0) && (window->y == 0) &&
			(window->width == CONFIG_UI_DISPLAY_WIDTH) &&
			(window->height == CONFIG_UI_DISPLAY_HEIGHT)) {
			return UI_OK;
		}

		previous.x = window->x;
		previous.y = window->y;
		previous.width = window->width;
		previous.height = window->height;

		ret = ui_rect_intersect(previous, *new_area);
		if (ret.x == 0 && ret.y == 0 && ret.width == 0 && ret.height == 0) {
			continue;
		}

		ret = ui_get_contain_rect(previous, *new_area);
		new_area->x = ret.x;
		new_area->y = ret.y;
		new_area->width = ret.width;
		new_area->height = ret.height;

		vec_remove(&g_window_redraw_list, window);
		iter--;
	}

	vec_push(&g_window_redraw_list, new_area);

	return UI_OK;
}

ui_error_t ui_window_redraw_list_clear(void)
{
	vec_clear(&g_window_redraw_list);

	return UI_OK;
}

static ui_rect_t *_ui_window_get_mempool_rect(void)
{
	int alloc_idx = g_rect_mempool_idx;

	g_rect_mempool_idx++;
	if (g_rect_mempool_idx >= CONFIG_UI_UPDATE_MEMPOOL_SIZE) {
		g_rect_mempool_idx = 0;
	}

	return &g_rect_mempool[alloc_idx];
}
#endif // CONFIG_UI_PARTIAL_UPDATE

ui_window_body_t *ui_window_get_current(void)
{
	return g_current_window;
}
