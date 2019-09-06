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
#include <vec/vec.h>
#include <araui/ui_widget.h>
#include "ui_window_internal.h"
#include "ui_widget_internal.h"
#include "ui_commons_internal.h"
#include "ui_math.h"
#include "ui_log.h"
#include "dal/ui_dal.h"

/****************************************************************************
 * Private function declaration
 ****************************************************************************/
static void _ui_renderer_update_tm_recur(ui_widget_body_t *widget);
static void _ui_renderer_update_invtm_recur(ui_widget_body_t *widget);

/**
 * @brief Matrix multiplication macros
 */
#define UI_TM_APPLIED_X(widget, x, y) ( \
	((widget)->trans_mat[0][0] * (x)) + \
	((widget)->trans_mat[0][1] * (y)) + \
	((widget)->trans_mat[0][2]))

#define UI_TM_APPLIED_Y(widget, x, y) ( \
	((widget)->trans_mat[1][0] * (x)) + \
	((widget)->trans_mat[1][1] * (y)) + \
	((widget)->trans_mat[1][2]))

#define UI_GL_GET_ORI_X(widget, x, y) ( \
	((widget)->inverse_mat[0][0] * (x)) + \
	((widget)->inverse_mat[0][1] * (y)) + \
	((widget)->inverse_mat[0][2]))

#define UI_GL_GET_ORI_Y(widget, x, y) ( \
	((widget)->inverse_mat[1][0] * (x)) + \
	((widget)->inverse_mat[1][1] * (y)) + \
	((widget)->inverse_mat[1][2]))

/****************************************************************************
 * Public function implementation
 ****************************************************************************/

void ui_matrix_copy(float dest_mat[3][3], float src_mat[3][3])
{
	int i;
	int j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			dest_mat[i][j] = src_mat[i][j];
		}
	}
}

void ui_matrix_multiply(float dest_mat[3][3], float left_mat[3][3], float right_mat[3][3])
{
	float left_cpy_mat[3][3];
	float right_cpy_mat[3][3];
	int i;
	int j;
	int k;

	// Need to make copies for the case that the dest_mat is same with left or right.
	ui_matrix_copy(left_cpy_mat, left_mat);
	ui_matrix_copy(right_cpy_mat, right_mat);

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			dest_mat[i][j] = 0.0f;
			for (k = 0; k < 3; k++) {
				dest_mat[i][j] += left_cpy_mat[i][k] * right_cpy_mat[k][j];
			}
		}
	}
}

void ui_render_widget(ui_widget_body_t *widget)
{
	int iter;
	ui_widget_body_t *child;
	ui_coord_t vertices[4];
	ui_rect_t update_rect;

	if (!widget) {
		UI_LOGE("error: invalid widget");
		return;
	}

	if (widget->mat_update_flag) {
		_ui_renderer_update_tm_recur(widget);
		_ui_renderer_update_invtm_recur(widget);

		vertices[0].x = UI_TM_APPLIED_X(widget, 0, 0);
		vertices[0].y = UI_TM_APPLIED_Y(widget, 0, 0);
		vertices[1].x = UI_TM_APPLIED_X(widget, widget->local_rect.width, 0);
		vertices[1].y = UI_TM_APPLIED_Y(widget, widget->local_rect.width, 0);
		vertices[2].x = UI_TM_APPLIED_X(widget, 0, widget->local_rect.height);
		vertices[2].y = UI_TM_APPLIED_Y(widget, 0, widget->local_rect.height);
		vertices[3].x = UI_TM_APPLIED_X(widget, widget->local_rect.width, widget->local_rect.height);
		vertices[3].y = UI_TM_APPLIED_Y(widget, widget->local_rect.width, widget->local_rect.height);

		update_rect.x = (int32_t)UI_MIN4(vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x);
		update_rect.y = (int32_t)UI_MIN4(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y);
		update_rect.width = (int32_t)UI_MAX4(vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x) - update_rect.x;
		update_rect.height = (int32_t)UI_MAX4(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y) - update_rect.y;
		update_rect.x -= widget->pivot_x;
		update_rect.y -= widget->pivot_y;

		ui_window_add_update_list(update_rect);
	}

	vec_foreach(&widget->children, child, iter) {
		ui_render_widget(child);
	}
}

void ui_render_image_widget(ui_image_widget_body_t *widget)
{
//	ui_rect_t viewport = ui_hal_get_viewport();
//	ui_rect_t trans_view;
//	ui_rect_t image_crop_rect;
	ui_widget_body_t *base_body;
	ui_coord_t vertices[4];
	ui_rect_t image_rect;
	int draw_x;
	int draw_y;
	int offset_x;
	int offset_y;
	float original_x;
	float original_y;

	ui_color_rgba8888_t *color_rgba8888;
	ui_color_rgb888_t *color_rgb888;

	if (!widget) {
		UI_LOGE("error: invalid widget");
		return;
	}

	base_body = &widget->base;

	vertices[0].x = UI_TM_APPLIED_X(base_body, widget->crop_rect.x, widget->crop_rect.y);
	vertices[0].y = UI_TM_APPLIED_Y(base_body, widget->crop_rect.x, widget->crop_rect.y);
	vertices[1].x = UI_TM_APPLIED_X(base_body, widget->crop_rect.x + widget->crop_rect.width, widget->crop_rect.y);
	vertices[1].y = UI_TM_APPLIED_Y(base_body, widget->crop_rect.x + widget->crop_rect.width, widget->crop_rect.y);
	vertices[2].x = UI_TM_APPLIED_X(base_body, widget->crop_rect.x, widget->crop_rect.y + widget->crop_rect.height);
	vertices[2].y = UI_TM_APPLIED_Y(base_body, widget->crop_rect.x, widget->crop_rect.y + widget->crop_rect.height);
	vertices[3].x = UI_TM_APPLIED_X(base_body, widget->crop_rect.x + widget->crop_rect.width, widget->crop_rect.y + widget->crop_rect.height);
	vertices[3].y = UI_TM_APPLIED_Y(base_body, widget->crop_rect.x + widget->crop_rect.width, widget->crop_rect.y + widget->crop_rect.height);

	image_rect.x = (int32_t)UI_MIN(UI_MIN(vertices[0].x, vertices[1].x), UI_MIN(vertices[2].x, vertices[3].x));
	image_rect.y = (int32_t)UI_MIN(UI_MIN(vertices[0].y, vertices[1].y), UI_MIN(vertices[2].y, vertices[3].y));
	image_rect.width = (int32_t)UI_MAX(UI_MAX(vertices[0].x, vertices[1].x), UI_MAX(vertices[2].x, vertices[3].x)) - image_rect.x;
	image_rect.height = (int32_t)UI_MAX(UI_MAX(vertices[0].y, vertices[1].y), UI_MAX(vertices[2].y, vertices[3].y)) - image_rect.y;

	offset_x = base_body->parent->crop_rect.x - widget->base.pivot_x;
	offset_y = base_body->parent->crop_rect.y - widget->base.pivot_y;
	// TODO: viewport must be enabled in the near future.
	// image_rect -> image_crop_rect
	for (draw_y = image_rect.y; draw_y < image_rect.y + image_rect.height; draw_y++) {
		for (draw_x = image_rect.x; draw_x < image_rect.x + image_rect.width; draw_x++) {
			original_x = UI_GL_GET_ORI_X(base_body, draw_x, draw_y);
			original_y = UI_GL_GET_ORI_Y(base_body, draw_x, draw_y);
			if ((original_x < widget->crop_rect.x) ||
				(original_x > widget->crop_rect.x + widget->crop_rect.width) ||
				(original_y < widget->crop_rect.y) ||
				(original_y > widget->crop_rect.y + widget->crop_rect.height)) {
				continue;
			}

			switch (widget->image->pixel_format) {
			case UI_PIXEL_FORMAT_RGBA8888: {
				color_rgba8888 = (ui_color_rgba8888_t *)&(widget->image->buf[
					(widget->image->width * (int)original_y + (int)original_x) * (widget->image->bits_per_pixel >> 3)]
				);
				ui_dal_put_pixel_rgba8888(draw_x + offset_x, draw_y + offset_y, *(ui_color_t *)color_rgba8888);
			}
			break;

			case UI_PIXEL_FORMAT_RGB888: {
				color_rgb888 = (ui_color_rgb888_t *)&(widget->image->buf[
					(widget->image->width * (int)original_y + (int)original_x) * (widget->image->bits_per_pixel >> 3)]
				);
				ui_dal_put_pixel_rgb888(draw_x + offset_x, draw_y + offset_y, (*(ui_color_t *)color_rgb888) & 0x00ffffff);
			}
			break;

			default:
			break;
			}
		}
	}
}

void ui_render_text_widget(ui_text_widget_body_t *widget)
{
	// todo
}

/****************************************************************************
 * Private function implementation
 ****************************************************************************/
static void _ui_renderer_update_tm_recur(ui_widget_body_t *widget)
{
	float trans_x;
	float trans_y;
	float pivot_x;
	float pivot_y;
	float scale_x;
	float scale_y;
	int iter;
	ui_widget_body_t *child;

	trans_x = (float)widget->local_rect.x;
	trans_y = (float)widget->local_rect.y;
	pivot_x = (float)widget->pivot_x;
	pivot_y = (float)widget->pivot_y;
	scale_x = widget->scale_x;
	scale_y = widget->scale_y;

	float translate_mat[3][3] = {
		{ 1.0f, 0.0f, trans_x },
		{ 0.0f, 1.0f, trans_y },
		{ 0.0f, 0.0f, 1.0f }
	};

	float rotate_mat[3][3] = {
		{ cosd(widget->degree), (-1.0f) * sind(widget->degree), (pivot_x * (1.0f - cosd(widget->degree))) + (pivot_y * sind(widget->degree)) },
		{ sind(widget->degree), cosd(widget->degree), (pivot_y * (1.0f - cosd(widget->degree))) - (pivot_x * sind(widget->degree)) },
		{0.0f, 0.0f, 1.0f}
	};

	float scale_mat[3][3] = {
		{ scale_x, 0.0f, pivot_x * (1.0f - scale_x) },
		{ 0.0f, scale_y, pivot_y * (1.0f - scale_y) },
		{ 0.0f, 0.0f, 1.0f }
	};

	ui_matrix_multiply(widget->trans_mat, widget->parent->trans_mat, translate_mat);
	ui_matrix_multiply(widget->trans_mat, widget->trans_mat, rotate_mat);
	ui_matrix_multiply(widget->trans_mat, widget->trans_mat, scale_mat);

	widget->mat_update_flag = false;

	vec_foreach(&widget->children, child, iter) {
		_ui_renderer_update_tm_recur(child);
	}
}

static void _ui_renderer_update_invtm_recur(ui_widget_body_t *widget)
{
	float trans_x;
	float trans_y;
	float pivot_x;
	float pivot_y;
	float scale_x;
	float scale_y;
	int iter;
	ui_widget_body_t *child;

	trans_x = -1 * (float)widget->local_rect.x;
	trans_y = -1 * (float)widget->local_rect.y;
	pivot_x = (float)widget->pivot_x;
	pivot_y = (float)widget->pivot_y;
	scale_x = 1 / widget->scale_x;
	scale_y = 1 / widget->scale_y;

	float translate_mat[3][3] = {
		{1.0f, 0.0f, trans_x},
		{0.0f, 1.0f, trans_y},
		{0.0f, 0.0f, 1.0f}
	};

	float rotate_mat[3][3] = {
		{cosd(-widget->degree), -1 * sind(-widget->degree), pivot_x * (1.0f - cosd(-widget->degree)) + pivot_y * sind(-widget->degree)},
		{sind(-widget->degree), cosd(-widget->degree), pivot_y * (1.0f - cosd(-widget->degree)) - pivot_x * sind(-widget->degree)},
		{0.0f, 0.0f, 1.0f}
	};

	float scale_mat[3][3] = {
		{scale_x, 0.0f, pivot_x * (1.0f - scale_x)},
		{0.0f, scale_y, pivot_y * (1.0f - scale_y)},
		{0.0f, 0.0f, 1.0f}
	};

	ui_matrix_multiply(widget->inverse_mat, translate_mat, widget->parent->inverse_mat);
	ui_matrix_multiply(widget->inverse_mat, rotate_mat, widget->inverse_mat);
	ui_matrix_multiply(widget->inverse_mat, scale_mat, widget->inverse_mat);

	widget->mat_update_flag = false;

	vec_foreach(&widget->children, child, iter) {
		_ui_renderer_update_invtm_recur(child);
	}
}
