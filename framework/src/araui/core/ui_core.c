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

#if defined(UI_PLATFORM_LINUX)
#define _GNU_SOURCE
#endif

#include <tinyara/config.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <vec/vec.h>
#include <araui/ui_commons.h>
#include "ui_renderer.h"
#include "ui_request_callback.h"
#include "ui_log.h"
#include "ui_core_internal.h"
#include "ui_asset_internal.h"
#include "ui_widget_internal.h"
#include "ui_window_internal.h"
#include "ui_commons_internal.h"
#include "dal/ui_dal.h"

#if defined(CONFIG_UI_ENABLE_EMOJI)
#include "utils/emoji.h"
#endif

#define UI_CORE_THREAD_NAME              "UI Core Service"
#define UI_DEFAULT_BACKGROUND_COLOR      0x0000
#define CONFIG_UI_GLOBAL_X_THRESHOLD     20
#define CONFIG_UI_GLOBAL_Y_THRESHOLD     20

typedef struct {
	ui_core_state_t state;
	pthread_t pid;
	pid_t caller_pid;
	ui_quick_panel_event_type_t visible_event_type;

#if defined(CONFIG_UI_ENABLE_TOUCH)
	ui_widget_body_t *locked_target;
#endif // CONFIG_UI_ENABLE_TOUCH
} ui_core_t;

typedef struct {
	bool pressed;
	ui_coord_t coord;
} ui_touch_state_t;

static ui_core_t g_core;
static ui_widget_body_t *g_quick_panel_info[UI_QUICK_PANEL_TYPE_NUM];

static ui_error_t _ui_process_widget_recur(ui_widget_body_t *widget, uint32_t dt);
static void _ui_call_tween_finished_cb(void *userdata);
static void *_ui_core_thread_loop(void *param);
static bool _ui_core_quick_panel_visible(void);

#if defined(CONFIG_UI_ENABLE_TOUCH)
static void _ui_core_dispatch_touch_event(void);
static void _ui_core_handle_touch_event(ui_touch_event_t touch_event, ui_coord_t coord);
static bool _ui_core_quick_panel_touch_down(ui_touch_event_t touch_event, ui_coord_t coord);
#endif

ui_error_t ui_start(void)
{
	int idx;
	pthread_attr_t attr;

	if (ui_is_running()) {
		UI_LOGE("Error: UI_ALREADY_RUNNING.\n");
		return UI_ALREADY_RUNNING;
	}

	if (ui_dal_init() != UI_OK) {
		UI_LOGE("Error: UI_INIT_FAILURE.\n");
		return UI_INIT_FAILURE;
	}

	if (ui_window_list_init() != UI_OK) {
		ui_dal_deinit();
		UI_LOGE("Error: UI_INIT_FAILURE.\n");
		return UI_INIT_FAILURE;
	}

	if (ui_window_update_list_init() != UI_OK) {
		ui_dal_deinit();
		ui_window_list_deinit();
		return UI_INIT_FAILURE;
	}

	for (idx = 0; idx < UI_QUICK_PANEL_TYPE_NUM; idx++) {
		g_quick_panel_info[idx] = NULL;
	}

	if (pthread_attr_init(&attr)) {
		ui_dal_deinit();
		ui_window_list_deinit();
		ui_window_update_list_deinit();
		UI_LOGE("Error: UI_INIT_FAILURE.\n");
		return UI_INIT_FAILURE;
	}

#if defined(UI_PLATFORM_TIZENRT)
	attr.stacksize = CONFIG_UI_STACK_SIZE;
#endif

	if (ui_request_callback_init() != UI_OK) {
		ui_dal_deinit();
		ui_window_list_deinit();
		ui_window_update_list_deinit();
		UI_LOGE("Error: UI_INIT_FAILURE.\n");
		return UI_INIT_FAILURE;
	}

	g_core.state = UI_CORE_STATE_RUNNING;

#if defined(CONFIG_UI_ENABLE_EMOJI)
	emoji_init();
#endif

	if (pthread_create(&g_core.pid, &attr, _ui_core_thread_loop, NULL)) {
		ui_dal_deinit();
		ui_window_list_deinit();
		ui_window_update_list_deinit();
		ui_request_callback_deinit();
		g_core.state = UI_CORE_STATE_STOP;
		UI_LOGE("Error: UI_INIT_FAILURE.\n");
		return UI_INIT_FAILURE;
	}

#if !defined(UI_PLATFORM_DARWIN)
	if (pthread_setname_np(g_core.pid, UI_CORE_THREAD_NAME)) {
		UI_LOGE("error: failed to set pthread name!\n");
	}
#endif

	g_core.caller_pid = getpid();

	return UI_OK;
}

ui_error_t ui_stop(void)
{
	if (!ui_is_running()) {
		UI_LOGE("Error: UI_NOT_RUNNING.\n");
		return UI_NOT_RUNNING;
	}

	if (g_core.caller_pid != getpid()) {
		UI_LOGE("Error: Permission denied.\n");
		return UI_OPERATION_FAIL;
	}

#if defined(CONFIG_UI_ENABLE_EMOJI)
	emoji_deinit();
#endif

	g_core.state = UI_CORE_STATE_STOPPING;

	if (pthread_join(g_core.pid, NULL) != OK) {
		UI_LOGE("pthread_join failed.\n");
		return UI_OPERATION_FAIL;
	}

	if (ui_request_callback_deinit() != UI_OK) {
		UI_LOGE("ui_request_callback_deinit failed.\n");
		return UI_OPERATION_FAIL;
	}

	if (ui_window_update_list_deinit() != UI_OK) {
		UI_LOGE("ui_window_update_list_deinit failed.\n");
		return UI_OPERATION_FAIL;
	}

	if (ui_window_list_deinit() != UI_OK) {
		UI_LOGE("ui_window_list_deinit failed.\n");
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static ui_error_t _ui_process_widget_recur(ui_widget_body_t *widget, uint32_t dt)
{
	int iter;
	ui_widget_body_t *child;

	if (!widget) {
		UI_LOGE("Error: widget is null!\n");
		return UI_INVALID_PARAM;
	}

	if (widget->tween_cb) {
		if (widget->tween_info.t > widget->tween_info.d) {
			widget->tween_info.t = widget->tween_info.d;
			widget->tween_cb((ui_widget_t)widget, widget->tween_info.t);
			widget->tween_cb = UI_NULL;

			if (ui_request_callback(_ui_call_tween_finished_cb, widget) != UI_OK) {
				UI_LOGE("Error: cannot make a request to call.tween_finished_cb!\n");
			}
		} else {
			widget->tween_cb((ui_widget_t)widget, widget->tween_info.t);
			widget->tween_info.t += dt;
		}
	}

	if (widget->anim_cb) {
		if (widget->anim_cb((ui_widget_t)widget, dt)) {
			widget->anim_cb = UI_NULL;
		}
	}

	if (widget->tick_cb) {
		widget->tick_cb((ui_widget_t)widget, dt);
	}

	if (widget->interval_cb) {
		widget->interval_info.current += dt;
		if (widget->interval_info.current >= widget->interval_info.timeout) {
			widget->interval_info.current -= widget->interval_info.timeout;
			widget->interval_cb((ui_widget_t)widget);
		}
	}

	if (widget->visible) {
		if (widget->update_cb) {
			widget->update_cb((ui_widget_t)widget, dt);
		}

		vec_foreach(&widget->children, child, iter) {
			_ui_process_widget_recur(child, dt);
		}
	}

	return UI_OK;
}

static ui_error_t _ui_draw_widget_recur(ui_widget_body_t *widget, ui_rect_t draw_area, uint32_t dt)
{
	int iter;
	ui_widget_body_t *child;
	ui_rect_t new_vp;

	if (!widget) {
		UI_LOGE("Error: widget is null!\n");
		return UI_INVALID_PARAM;
	}

	if (widget->visible) {
		if (widget->draw_cb) {
			new_vp = ui_rect_intersect(draw_area, widget->crop_rect);
			ui_dal_set_viewport(new_vp.x, new_vp.y, new_vp.width, new_vp.height);
			widget->draw_cb((ui_widget_t)widget, dt);
			ui_dal_set_viewport(draw_area.x, draw_area.y, draw_area.width, draw_area.height);
		}

		vec_foreach(&widget->children, child, iter) {
			_ui_draw_widget_recur(child, draw_area, dt);
		}
	}

	return UI_OK;
}

static void _ui_call_tween_finished_cb(void *userdata)
{
	ui_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_widget_body_t *)userdata;
	if (body && body->tween_info.tween_finished_cb) {
		body->tween_info.tween_finished_cb((ui_widget_t)body);
	}
}

static void _ui_redraw(uint32_t dt)
{
	ui_rect_t *redraw_area;
	ui_window_body_t *window;
	int iter;
	static bool clear_flag = false;

	vec_foreach(ui_window_get_update_list(), redraw_area, iter) {
		window = ui_window_get_current();
		if (window) {
			_ui_draw_widget_recur(window->root, *redraw_area, dt);
		}

		if (_ui_core_quick_panel_visible()) {
			_ui_draw_widget_recur(g_quick_panel_info[g_core.visible_event_type], *redraw_area, dt);
		}

		if (window || _ui_core_quick_panel_visible()) {
			ui_dal_redraw(redraw_area->x, redraw_area->y, redraw_area->width, redraw_area->height);
		}

		clear_flag = true;
	}

	if (clear_flag) {
		clear_flag = false;
		ui_window_update_list_clear();
	}
}

static void *_ui_core_thread_loop(void *param)
{
	ui_widget_body_t *root;
	ui_window_body_t *window;
	static ui_window_body_t *prev_window;
	struct timespec before;
	struct timespec now;
	uint32_t dt;

#if (CONFIG_UI_MAXIMUM_FPS > 0)
	uint32_t fps_dt = 0;
	const uint32_t ms_per_frame = 1000 / CONFIG_UI_MAXIMUM_FPS;
#endif

	memset(&before, 0, sizeof(struct timespec));
	memset(&now, 0, sizeof(struct timespec));

	clock_gettime(CLOCK_MONOTONIC, &before);

	while (g_core.state == UI_CORE_STATE_RUNNING) {
		ui_dal_clear();

		clock_gettime(CLOCK_MONOTONIC, &now);

		dt = ((now.tv_sec - before.tv_sec) * 1000) + ((now.tv_nsec - before.tv_nsec) / 1000000);
		before = now;

#if (CONFIG_UI_MAXIMUM_FPS > 0)
		fps_dt += dt;
		if (fps_dt < ms_per_frame) {
			usleep((ms_per_frame - fps_dt) * 1000);
		}
		fps_dt -= ms_per_frame;
#endif

		window = ui_window_get_current();
		if (window) {
			prev_window = window;
			root = window->root;

			if (_ui_process_widget_recur(root, dt) != UI_OK) {
				UI_LOGE("error: processing widget recursively failed!\n");
			}

			ui_render_widget(root);
		}

		if (_ui_core_quick_panel_visible()) {
			_ui_process_widget_recur(g_quick_panel_info[g_core.visible_event_type], dt);
		}

		_ui_redraw(dt);

#if defined(CONFIG_UI_ENABLE_TOUCH)
		_ui_core_dispatch_touch_event();
#endif

		ui_process_all_requests();
	}

	g_core.state = UI_CORE_STATE_STOP;

	UI_LOGD("[%s] has been finished.\n", UI_CORE_THREAD_NAME);

	return NULL;
}

bool ui_is_running(void)
{
	return (g_core.state != UI_CORE_STATE_STOP);
}

#if defined(CONFIG_UI_ENABLE_TOUCH)

static void _ui_deliver_touch_event(ui_widget_body_t *widget, ui_touch_event_t touch_event, ui_coord_t coord)
{
	ui_widget_body_t *parent = UI_NULL;

	// If there is locked target, send all event to the target only
	// else, send event to the focus widget and its hookers.

	if (g_core.locked_target) {
		g_core.locked_target->touch_cb(g_core.locked_target, touch_event, coord);
	} else {
		widget->touch_cb(widget, touch_event, coord);

		// send touch event to the all hookers also
		parent = widget->parent;
		while (parent) {
			if (parent->is_hooker && parent->touch_cb) {
				parent->touch_cb(parent, touch_event, coord);
			}
			parent = parent->parent;
		}
	}
}

/**
 * @brief Called when some widget is locked as an event target.
 * But, the locked target doesn't get the event.
 */
static void _ui_cancel_touch_event_except_locked_target(ui_widget_body_t *widget)
{
	ui_coord_t null_coord = {0, };
	ui_widget_body_t *parent;

	// The focus widget should receive the cancel touch event
	// whether the focus widget is a hooker or not.
	if (widget->touch_cb && widget != g_core.locked_target) {
		widget->touch_cb(widget, UI_TOUCH_EVENT_CANCEL, null_coord);
	}

	// Only hooker widgets inside the focus widget's parent list,
	// should receive the touch cancel event.
	parent = widget->parent;
	while (parent) {
		if (parent->is_hooker && parent->touch_cb && parent != g_core.locked_target) {
			parent->touch_cb(parent, UI_TOUCH_EVENT_CANCEL, null_coord);
		}
		parent = parent->parent;
	}
}

static void _ui_core_handle_touch_event(ui_touch_event_t touch_event, ui_coord_t coord)
{
	ui_window_body_t *window;
	ui_widget_body_t *root;
	ui_quick_panel_body_t *quick_panel;

	window = ui_window_get_current();
	if (!window) {
		UI_LOGE("error: active window is null!\n");
		return;
	}

	root = window->root;

	switch (touch_event) {
	case UI_TOUCH_EVENT_DOWN:
		{
			// If touch down, reset all infomations about focus and touch
			ui_core_unlock_touch_event_target();

			// If there is the visible quick panel,
			// a touch event will be delivered to only that quick panel.
			if (_ui_core_quick_panel_visible()) {
				quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[g_core.visible_event_type];

				if (quick_panel->focus) {
					_ui_cancel_touch_event_except_locked_target(quick_panel->focus);
				}

				quick_panel->focus = ui_widget_search_by_coord(g_quick_panel_info[g_core.visible_event_type], coord);
				if (quick_panel->focus && quick_panel->focus->touch_cb) {
					_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
				}
				break;
			}

			// If a touch down event occurs at the area which quick panel is set,
			// a touch event will be delivered to only that quick panel.
			if (_ui_core_quick_panel_touch_down(touch_event, coord)) {
				break;
			}

			if (window->focus) {
				_ui_cancel_touch_event_except_locked_target(window->focus);
			}

			window->focus = ui_widget_search_by_coord(root, coord);
			if (window->focus) {
				if (window->focus->touch_cb) {
					_ui_deliver_touch_event(window->focus, touch_event, coord);
				}
			}
		}
		break;

	case UI_TOUCH_EVENT_MOVE:
		{
			// If there is the visible quick panel,
			// a touch event will be delivered to only that quick panel.
			if (_ui_core_quick_panel_visible()) {
				quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[g_core.visible_event_type];
				if (quick_panel->focus && quick_panel->focus->touch_cb) {
					_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
				}
				break;
			}

			if (window->focus && window->focus->touch_cb) {
				_ui_deliver_touch_event(window->focus, touch_event, coord);
			}
		}
		break;

	case UI_TOUCH_EVENT_UP:
		{
			if (_ui_core_quick_panel_visible()) {
				quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[g_core.visible_event_type];
				if (quick_panel->focus && quick_panel->focus->touch_cb) {
					_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
				}
				ui_core_unlock_touch_event_target();
				break;
			}

			if (window->focus && window->focus->touch_cb) {
				_ui_deliver_touch_event(window->focus, touch_event, coord);
			}
			ui_core_unlock_touch_event_target();
		}
		break;

	default:
		break;
	}
}

static void _ui_core_dispatch_touch_event(void)
{
	static ui_touch_state_t before = {false, };
	static ui_touch_state_t cur = {false, };

	while (ui_dal_get_touch(&cur.pressed, &cur.coord)) {
		if (cur.pressed != before.pressed) {
			if (cur.pressed) {
				_ui_core_handle_touch_event(UI_TOUCH_EVENT_DOWN, cur.coord);
			} else {
				_ui_core_handle_touch_event(UI_TOUCH_EVENT_UP, cur.coord);
			}
		} else if ((cur.coord.x != before.coord.x) || (cur.coord.y != before.coord.y)) {
			_ui_core_handle_touch_event(UI_TOUCH_EVENT_MOVE, cur.coord);
		}

		before = cur;
	}
}

void ui_core_lock_touch_event_target(ui_widget_body_t *target)
{
	ui_window_body_t *window;
	ui_widget_body_t *focus;
	ui_quick_panel_body_t *quick_panel;

	if (_ui_core_quick_panel_visible()) {
		quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[g_core.visible_event_type];
		focus = quick_panel->focus;
	} else {
		window = ui_window_get_current();
		if (!window) {
			UI_LOGE("error: active window is null!\n");
			return;
		}

		focus = window->focus;
	}

	g_core.locked_target = target;

	// Cancel all hookers touch event except the locked target
	_ui_cancel_touch_event_except_locked_target(focus);
}

void ui_core_unlock_touch_event_target(void)
{
	g_core.locked_target = NULL;
}

void ui_core_unlock_and_deliver_touch(ui_widget_body_t *body, ui_touch_event_t touch_event, ui_coord_t touch)
{
	ui_core_unlock_touch_event_target();

	_ui_deliver_touch_event(body, touch_event, touch);
}

#endif // CONFIG_UI_ENABLE_TOUCH

ui_error_t ui_core_quick_panel_appear(ui_quick_panel_event_type_t event_type)
{
	ui_quick_panel_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!g_quick_panel_info[event_type]) {
		return UI_INVALID_PARAM;
	}

	body = (ui_quick_panel_body_t *)g_quick_panel_info[event_type];
	if (body->base.visible) {
		UI_LOGE("error: event type [%d] already appears!\n", event_type);
		return UI_INVALID_PARAM;
	}

	switch (event_type) {
	case UI_QUICK_PANEL_TOP_SWIPE:
	case UI_QUICK_PANEL_BOTTOM_SWIPE:
	case UI_QUICK_PANEL_LEFT_SWIPE:
	case UI_QUICK_PANEL_RIGHT_SWIPE:
		if (body->transition_type == UI_TRANSITION_SLIDE) {
			ui_widget_tween_moveto((ui_widget_t)g_quick_panel_info[event_type], 0, 0, CONFIG_UI_QUICK_PANEL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_quick_panel_appear_tween_end_func);
		}
		break;

	case UI_QUICK_PANEL_BUTTON: {
		}
		break;

	default: {
		}
		break;
	}

	return UI_OK;
}

ui_error_t ui_core_quick_panel_disappear(ui_quick_panel_event_type_t event_type)
{
	ui_quick_panel_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!g_quick_panel_info[event_type]) {
		return UI_INVALID_PARAM;
	}

	body = (ui_quick_panel_body_t *)g_quick_panel_info[event_type];
	if (!body->base.visible) {
		UI_LOGE("error: event type [%d] already disappears!\n", event_type);
		return UI_INVALID_PARAM;
	}

	if (body->transition_type == UI_TRANSITION_SLIDE) {
		switch (event_type) {
			case UI_QUICK_PANEL_TOP_SWIPE: {
				ui_widget_tween_moveto((ui_widget_t)g_quick_panel_info[event_type], 0, -1 * CONFIG_UI_DISPLAY_HEIGHT, CONFIG_UI_QUICK_PANEL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_quick_panel_disappear_tween_end_func);
			}
			break;

			case UI_QUICK_PANEL_BOTTOM_SWIPE: {
				ui_widget_tween_moveto((ui_widget_t)g_quick_panel_info[event_type], 0, CONFIG_UI_DISPLAY_HEIGHT, CONFIG_UI_QUICK_PANEL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_quick_panel_disappear_tween_end_func);
			}
			break;

			case UI_QUICK_PANEL_LEFT_SWIPE: {
				ui_widget_tween_moveto((ui_widget_t)g_quick_panel_info[event_type], -1 * CONFIG_UI_DISPLAY_WIDTH, 0, CONFIG_UI_QUICK_PANEL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_quick_panel_disappear_tween_end_func);
			}
			break;

			case UI_QUICK_PANEL_RIGHT_SWIPE: {
				ui_widget_tween_moveto((ui_widget_t)g_quick_panel_info[event_type], CONFIG_UI_DISPLAY_WIDTH, 0, CONFIG_UI_QUICK_PANEL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_quick_panel_disappear_tween_end_func);
			}
			break;

			case UI_QUICK_PANEL_BUTTON: {
			}
			break;

			default: {
			}
			break;
		}
	}

	return UI_OK;
}

ui_error_t ui_core_set_quick_panel(ui_quick_panel_event_type_t event_type, ui_widget_t widget)
{
	ui_quick_panel_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (g_quick_panel_info[event_type]) {
		UI_LOGE("error: quick panel for event type [%d] is already exist!\n", event_type);
		return UI_INVALID_PARAM;
	}

	body = (ui_quick_panel_body_t *)widget;
	body->event_type = event_type;
	body->base.visible = false;

	g_quick_panel_info[event_type] = (ui_widget_body_t *)widget;

	switch (event_type) {
		case UI_QUICK_PANEL_TOP_SWIPE: {
			body->base.local_rect.x = 0;
			body->base.local_rect.y = -1 * CONFIG_UI_DISPLAY_HEIGHT;
		}
		break;

		case UI_QUICK_PANEL_BOTTOM_SWIPE: {
			body->base.local_rect.x = 0;
			body->base.local_rect.y = CONFIG_UI_DISPLAY_HEIGHT;
		}
		break;

		case UI_QUICK_PANEL_LEFT_SWIPE: {
			body->base.local_rect.x = -1 * CONFIG_UI_DISPLAY_WIDTH;
			body->base.local_rect.y = 0;
		}
		break;

		case UI_QUICK_PANEL_RIGHT_SWIPE: {
			body->base.local_rect.x = CONFIG_UI_DISPLAY_WIDTH;
			body->base.local_rect.y = 0;
		}
		break;

		case UI_QUICK_PANEL_BUTTON: {
		}
		break;

		default: {
		}
		break;
	}

	return UI_OK;
}

ui_error_t ui_core_unset_quick_panel(ui_quick_panel_event_type_t event_type)
{
	ui_widget_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!g_quick_panel_info[event_type]) {
		UI_LOGE("error: quick panel for event type [%d] is not exist!\n", event_type);
		return UI_INVALID_PARAM;
	}

	body = (ui_widget_body_t *)g_quick_panel_info[event_type];
	if (body->visible) {
		UI_LOGE("error: quick panel for event type [%d] is displayed at the screen!\n", event_type);
		return UI_INVALID_PARAM;
	}

	g_quick_panel_info[event_type] = NULL;

	return UI_OK;
}

static bool _ui_core_quick_panel_visible(void)
{
	int idx;

	for (idx = 0; idx < UI_QUICK_PANEL_TYPE_NUM; idx++) {
		if (g_quick_panel_info[idx] && (g_quick_panel_info[idx])->visible) {
			g_core.visible_event_type = idx;
			return true;
		}
	}

	return false;
}

#if defined(CONFIG_UI_ENABLE_TOUCH)
static bool _ui_core_quick_panel_touch_down(ui_touch_event_t touch_event, ui_coord_t coord)
{
	ui_quick_panel_body_t *quick_panel;

	if (coord.y < CONFIG_UI_GLOBAL_Y_THRESHOLD) {
		if (g_quick_panel_info[UI_QUICK_PANEL_TOP_SWIPE]) {
			quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[UI_QUICK_PANEL_TOP_SWIPE];
			quick_panel->focus = g_quick_panel_info[UI_QUICK_PANEL_TOP_SWIPE];
			if (quick_panel->focus && quick_panel->focus->touch_cb) {
				_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
			}
			return true;
		}
		return false;
	} else if (coord.y > CONFIG_UI_DISPLAY_HEIGHT - CONFIG_UI_GLOBAL_Y_THRESHOLD) {
		if (g_quick_panel_info[UI_QUICK_PANEL_BOTTOM_SWIPE]) {
			quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[UI_QUICK_PANEL_BOTTOM_SWIPE];
			quick_panel->focus = g_quick_panel_info[UI_QUICK_PANEL_BOTTOM_SWIPE];
			if (quick_panel->focus && quick_panel->focus->touch_cb) {
				_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
			}
			return true;
		}
		return false;
	} else if (coord.x < CONFIG_UI_GLOBAL_X_THRESHOLD) {
		if (g_quick_panel_info[UI_QUICK_PANEL_LEFT_SWIPE]) {
			quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[UI_QUICK_PANEL_LEFT_SWIPE];
			quick_panel->focus = g_quick_panel_info[UI_QUICK_PANEL_LEFT_SWIPE];
			if (quick_panel->focus && quick_panel->focus->touch_cb) {
				_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
			}
			return true;
		}
		return false;
	} else if (coord.x > CONFIG_UI_DISPLAY_WIDTH - CONFIG_UI_GLOBAL_X_THRESHOLD) {
		if (g_quick_panel_info[UI_QUICK_PANEL_RIGHT_SWIPE]) {
			quick_panel = (ui_quick_panel_body_t *)g_quick_panel_info[UI_QUICK_PANEL_RIGHT_SWIPE];
			quick_panel->focus = g_quick_panel_info[UI_QUICK_PANEL_RIGHT_SWIPE];
			if (quick_panel->focus && quick_panel->focus->touch_cb) {
				_ui_deliver_touch_event(quick_panel->focus, touch_event, coord);
			}
			return true;
		}
		return false;
	}

	return false;
}
#endif

