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

#include <string.h>
#include <araui/ui_commons.h>
#include "ui_log.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_view_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "hal/ui_hal_interface.h"

typedef struct {
	ui_image_widget_body_t *body;
	ui_color_t colorkey;
} ui_set_colorkey_info_t;

typedef struct {
	ui_image_widget_body_t *body;
	ui_image_asset_body_t *image;
} ui_set_image_info_t;

typedef struct {
	ui_image_widget_body_t *body;
	int32_t x;
	int32_t y;
} ui_set_anchor_info_t;

typedef struct {
	ui_image_widget_body_t *body;
	ui_rect_t crop_rect;
} ui_set_crop_area_info_t;

typedef struct {
	ui_image_widget_body_t *body;
	int32_t width;
	int32_t height;
} ui_resize_image_asset_info_t;

static void _ui_image_widget_set_image_func(void *userdata);
static void _ui_image_widget_set_anchor_point(void *userdata);
static void _ui_image_widget_set_crop_area_func(void *userdata);
static void _ui_image_widget_resize_image_asset_func(void *userdata);

ui_error_t ui_image_widget_set_image(ui_widget_t widget, ui_asset_t image)
{
	ui_set_image_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_IMAGE_WIDGET) || !image) {
		return UI_INVALID_PARAM;
	}

	if (image && !ui_asset_check_type(image, UI_IMAGE_ASSET)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_image_info_t *)UI_ALLOC(sizeof(ui_set_image_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_image_info_t));

	info->body = (ui_image_widget_body_t *)widget;
	info->image = (ui_image_asset_body_t *)image;

	if (ui_request_callback(_ui_image_widget_set_image_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_image_widget_set_image_func(void *userdata)
{
	ui_set_image_info_t *info;

	if (!userdata || !((ui_set_image_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_image_info_t *)userdata;

	info->body->image = info->image;

	if (info->body->base.local_rect.width == 0) {
		info->body->base.local_rect.width = info->image->width;
	}

	if (info->body->base.local_rect.height == 0) {
		info->body->base.local_rect.height = info->image->height;
	}

	info->body->crop_rect.x = 0;
	info->body->crop_rect.y = 0;
	info->body->crop_rect.width = info->image->width;
	info->body->crop_rect.height = info->image->height;
	info->body->resized_width = info->image->width;
	info->body->resized_height = info->image->height;

	ui_widget_update_position_info((ui_widget_body_t *)info->body);

	UI_FREE(info);
}

static void ui_image_widget_draw_func(ui_widget_t widget, uint32_t dt)
{
	ui_image_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_image_widget_body_t *)widget;

	if (body->image) {
		ui_hal_draw_cropped_resized_image(
			body->base.global_rect.x + body->anchor_x - body->image->pivot_x,
			body->base.global_rect.y + body->anchor_y - body->image->pivot_y,
			(ui_asset_t)body->image, body->crop_rect, body->resized_width, body->resized_height);
	}
}

ui_error_t ui_image_widget_set_anchor_point(ui_widget_t widget, int32_t x, int32_t y)
{
	ui_set_anchor_info_t *cb_data;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget) {
		return UI_INVALID_PARAM;
	}

	cb_data = (ui_set_anchor_info_t *)UI_ALLOC(sizeof(ui_set_anchor_info_t));
	if (!cb_data) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	cb_data->body = (ui_image_widget_body_t *)widget;
	cb_data->x = x;
	cb_data->y = y;

	if (ui_request_callback(_ui_image_widget_set_anchor_point, (void *)cb_data) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_image_widget_set_anchor_point(void *userdata)
{
	ui_set_anchor_info_t *info;
	ui_image_widget_body_t *body;

	info = (ui_set_anchor_info_t *)userdata;
	body = info->body;

	body->anchor_x = info->x;
	body->anchor_y = info->y;

	UI_FREE(info);
}

ui_widget_t ui_image_widget_create(ui_asset_t image)
{
	ui_image_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_image_widget_body_t *)UI_ALLOC(sizeof(ui_image_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_image_widget_body_t));
	((ui_widget_body_t *)body)->type = UI_IMAGE_WIDGET;

	if (image) {
		if (!ui_asset_check_type(image, UI_IMAGE_ASSET)) {
			UI_LOGE("error: invalid parameter!\n");
			UI_FREE(body);
			return UI_NULL;
		}
		ui_widget_init((ui_widget_body_t *)body, ((ui_image_asset_body_t *)image)->width, ((ui_image_asset_body_t *)image)->height);
		body->image = (ui_image_asset_body_t *)image;
		body->crop_rect.x = 0;
		body->crop_rect.y = 0;
		body->crop_rect.width = ((ui_image_asset_body_t *)image)->width;
		body->crop_rect.height = ((ui_image_asset_body_t *)image)->height;
		body->resized_width = ((ui_image_asset_body_t *)image)->width;
		body->resized_height = ((ui_image_asset_body_t *)image)->height;
	} else {
		ui_widget_init((ui_widget_body_t *)body, 0, 0);
	}

	body->base.draw_cb = ui_image_widget_draw_func;

	UI_LOGD("UI_OK.\n");

	return (ui_widget_t)body;
}

ui_error_t ui_image_widget_set_crop_area(ui_widget_t widget, ui_rect_t crop_rect)
{
	ui_image_widget_body_t *body;
	ui_set_crop_area_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_IMAGE_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	if (crop_rect.x < 0 || crop_rect.y < 0) {
		return UI_INVALID_PARAM;
	}

	body = (ui_image_widget_body_t *)widget;

	if (!body->image || (crop_rect.x + crop_rect.width > body->image->width) || (crop_rect.y + crop_rect.height > body->image->height)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_crop_area_info_t *)UI_ALLOC(sizeof(ui_set_crop_area_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_crop_area_info_t));

	info->body = (ui_image_widget_body_t *)widget;
	info->crop_rect = crop_rect;

	if (ui_request_callback(_ui_image_widget_set_crop_area_func, (void *)info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_image_widget_set_crop_area_func(void *userdata)
{
	ui_set_crop_area_info_t *info;
	ui_image_widget_body_t *body;

	if (!userdata || !((ui_set_crop_area_info_t *)userdata)->body) {
		UI_LOGE("error: invalid parameter!\n");
		return;
	}

	info = (ui_set_crop_area_info_t *)userdata;
	body = info->body;
	body->crop_rect.x = info->crop_rect.x;
	body->crop_rect.y = info->crop_rect.y;
	body->crop_rect.width = info->crop_rect.width;
	body->crop_rect.height = info->crop_rect.height;

	UI_FREE(info);
}

ui_error_t ui_image_widget_resize_image_asset(ui_widget_t widget, int32_t width, int32_t height)
{
	ui_image_widget_body_t *body;
	ui_resize_image_asset_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_IMAGE_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	body = (ui_image_widget_body_t *)widget;

	if (!body->image) {
		return UI_INVALID_PARAM;
	}

	info = (ui_resize_image_asset_info_t *)UI_ALLOC(sizeof(ui_resize_image_asset_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_resize_image_asset_info_t));

	info->body = body;
	info->width = width;
	info->height = height;

	if (ui_request_callback(_ui_image_widget_resize_image_asset_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_image_widget_resize_image_asset_func(void *userdata)
{
	ui_resize_image_asset_info_t *info;

	if (!userdata || !((ui_resize_image_asset_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_resize_image_asset_info_t *)userdata;

	info->body->resized_width = info->width;
	info->body->resized_height = info->height;

	ui_widget_update_position_info((ui_widget_body_t *)info->body);

	UI_FREE(info);
}
