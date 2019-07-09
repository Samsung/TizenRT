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
#include <stdint.h>
#include <araui/ui_commons.h>
#include "hal/ui_hal_interface.h"
#include "ui_log.h"
#include "ui_request_callback.h"
#include "ui_view_internal.h"
#include "ui_widget_internal.h"
#include "ui_core_internal.h"

typedef struct {
	ui_frame_animation_widget_body_t *body;
	uint32_t duration;
} ui_set_duration_info_t;

static void _ui_frame_animation_widget_update_func(ui_widget_t widget, uint32_t dt);
static void _ui_frame_animation_widget_draw_func(ui_widget_t widget, uint32_t dt);
static void _ui_frame_animation_widget_remove_func(ui_widget_t widget);
static void _ui_frame_animation_widget_set_duration_func(void *userdata);
static void _ui_frame_animation_widget_play_func(void *userdata);
static void _ui_frame_animation_widget_play_loop_func(void *userdata);
static void _ui_frame_animation_widget_pause_func(void *userdata);
static void _ui_frame_animation_widget_resume_func(void *userdata);
static void _ui_frame_animation_widget_stop_func(void *userdata);

ui_widget_t ui_frame_animation_widget_create(int32_t width, int32_t height, ui_asset_t frames[], uint32_t frame_count, uint32_t duration)
{
	ui_frame_animation_widget_body_t *body;
	int frame_idx;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!frames) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_NULL;
	}

	if (frame_count > duration) {
		UI_LOGE("error: duration must be larger than the frame_count");
		return UI_NULL;
	}

	body = (ui_frame_animation_widget_body_t *)UI_ALLOC(sizeof(ui_frame_animation_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_frame_animation_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, width, height);
	((ui_widget_body_t *)body)->type = UI_FRAME_ANIMATION_WIDGET;

	body->frames = (ui_image_asset_body_t **)UI_ALLOC(frame_count * sizeof(ui_image_asset_body_t *));
	if (!body->frames) {
		UI_FREE(body);
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	for (frame_idx = 0; frame_idx < frame_count; frame_idx++) {
		body->frames[frame_idx] = (ui_image_asset_body_t *)frames[frame_idx];
	}

	body->base.draw_cb = _ui_frame_animation_widget_draw_func;
	body->base.update_cb = _ui_frame_animation_widget_update_func;
	body->base.remove_cb = _ui_frame_animation_widget_remove_func;
	body->frame_count = frame_count;
	body->duration = duration;
	body->interval = duration / frame_count;

	UI_LOGD("UI_OK.\n");

	return (ui_widget_t)body;
}

static void _ui_frame_animation_widget_update_func(ui_widget_t widget, uint32_t dt)
{
	ui_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_widget_body_t *)widget;
	if (ui_view_add_update_list(body->global_rect) != UI_OK) {
		UI_LOGE("error: failed to add to the update list!\n");
	}
}

static void _ui_frame_animation_widget_draw_func(ui_widget_t widget, uint32_t dt)
{
	ui_frame_animation_widget_body_t *body;
	uint32_t cur_frame;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)widget;
	if (body->play) {
		if (body->time > body->duration) {
			cur_frame = body->frame_count - 1;
			ui_hal_draw_image(body->base.global_rect.x, body->base.global_rect.y, (ui_asset_t)body->frames[cur_frame]);
			body->time = 0;

			if (!body->loop) {
				body->play = false;
			}
		} else {
			cur_frame = body->time / body->interval;
			if (cur_frame >= body->frame_count) {
				if (!body->loop) {
					body->play = false;
					return;
				}
				cur_frame = 0;
				body->time = 0;
			}
			ui_hal_draw_image(body->base.global_rect.x, body->base.global_rect.y, (ui_asset_t)body->frames[cur_frame]);
			body->time += dt;
		}
	}
}

static void _ui_frame_animation_widget_remove_func(ui_widget_t widget)
{
	ui_frame_animation_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)widget;

	UI_FREE(body->frames);
}

ui_error_t ui_frame_animation_widget_set_duration(ui_widget_t widget, uint32_t duration)
{
	ui_frame_animation_widget_body_t *body;
	ui_set_duration_info_t *info;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_INVALID_PARAM;
	}

	body = (ui_frame_animation_widget_body_t *)widget;

	if (body->frame_count > duration) {
		UI_LOGE("error: duration must be larger than the frame_count");
		return UI_INVALID_PARAM;
	}

	info = (ui_set_duration_info_t *)UI_ALLOC(sizeof(ui_set_duration_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_duration_info_t));

	info->body = (ui_frame_animation_widget_body_t *)widget;
	info->duration = duration;

	if (ui_request_callback(_ui_frame_animation_widget_set_duration_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_frame_animation_widget_set_duration_func(void *userdata)
{
	ui_set_duration_info_t *info;

	if (!userdata || !((ui_set_duration_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_duration_info_t *)userdata;

	info->body->duration= info->duration;

	UI_FREE(info);
}

ui_error_t ui_frame_animation_widget_play(ui_widget_t widget)
{
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_frame_animation_widget_play_func, (void *)widget) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_frame_animation_widget_play_func(void *userdata)
{
	ui_frame_animation_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)userdata;

	body->play = true;
	body->loop = false;
	body->time = 0;
}

ui_error_t ui_frame_animation_widget_play_loop(ui_widget_t widget)
{
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_frame_animation_widget_play_loop_func, (void *)widget) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_frame_animation_widget_play_loop_func(void *userdata)
{
	ui_frame_animation_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)userdata;

	body->play = true;
	body->loop = true;
	body->time = 0;
}

ui_error_t ui_frame_animation_widget_pause(ui_widget_t widget)
{
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_frame_animation_widget_pause_func, (void *)widget) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_frame_animation_widget_pause_func(void *userdata)
{
	ui_frame_animation_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)userdata;

	body->play = false;
}

ui_error_t ui_frame_animation_widget_resume(ui_widget_t widget)
{
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_frame_animation_widget_resume_func, (void *)widget) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_frame_animation_widget_resume_func(void *userdata)
{
	ui_frame_animation_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)userdata;

	body->play = true;
}

ui_error_t ui_frame_animation_widget_stop(ui_widget_t widget)
{
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		UI_LOGE("error: frames are NULL!\n");
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_frame_animation_widget_stop_func, (void *)widget) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_frame_animation_widget_stop_func(void *userdata)
{
	ui_frame_animation_widget_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_frame_animation_widget_body_t *)userdata;

	body->play = false;
	body->loop = false;
	body->time = 0;
}
