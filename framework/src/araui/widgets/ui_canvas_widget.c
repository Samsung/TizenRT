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
#include "ui_painter.h"
#include "hal/ui_hal_interface.h"

static void ui_canvas_widget_draw_func(ui_widget_t widget, uint32_t dt)
{
	ui_canvas_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_canvas_widget_body_t *)widget;

	if (body->canvas_draw_cb) {
		body->canvas_draw_cb(widget, dt);
	}
}

static void ui_canvas_widget_update_func(ui_widget_t widget, uint32_t dt)
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

ui_widget_t ui_canvas_widget_create(int32_t width, int32_t height, canvas_draw_callback draw_cb)
{
	ui_canvas_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_canvas_widget_body_t *)UI_ALLOC(sizeof(ui_canvas_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_canvas_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, width, height);
	((ui_widget_body_t *)body)->type = UI_CANVAS_WIDGET;

	body->base.draw_cb = ui_canvas_widget_draw_func;
	body->base.update_cb = ui_canvas_widget_update_func;
	body->canvas_draw_cb = draw_cb;

	UI_LOGD("UI_OK.\n");

	return (ui_widget_t)body;
}

ui_error_t ui_canvas_widget_draw_line(ui_widget_t widget, int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_line(start_x, start_y, end_x, end_y, color);
}

ui_error_t ui_canvas_widget_draw_hline(ui_widget_t widget, int32_t x, int32_t y, int32_t length, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_hline(x, y, length, color);
}

ui_error_t ui_canvas_widget_draw_vline(ui_widget_t widget, int32_t x, int32_t y, int32_t length, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_vline(x, y, length, color);
}

ui_error_t ui_canvas_widget_draw_rect(ui_widget_t widget, int32_t x, int32_t y, int32_t width, int32_t height, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_rect(x, y, width, height, color);
}

ui_error_t ui_canvas_widget_draw_circle(ui_widget_t widget, int32_t center_x, int32_t center_y, int32_t radius, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_circle(center_x, center_y, radius, color);
}

ui_error_t ui_canvas_widget_draw_ellipse(ui_widget_t widget, int32_t center_x, int32_t center_y, int32_t radius_x, int32_t radius_y, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_ellipse(center_x, center_y, radius_x, radius_y, color);
}

ui_error_t ui_canvas_widget_draw_arc(ui_widget_t widget, int32_t center_x, int32_t center_y, int32_t radius, double start_radian, double sweep_radian, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_draw_arc(center_x, center_y, radius, start_radian, sweep_radian, color);
}

ui_error_t ui_canvas_widget_fill_figure(ui_widget_t widget, int32_t x, int32_t y, ui_color_t color)
{
	if (!widget || !ui_widget_check_widget_type(widget, UI_CANVAS_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	return ui_painter_fill_figure(x, y, color);
}

