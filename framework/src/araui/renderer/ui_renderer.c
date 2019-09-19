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
#include <math.h>
#include <vec/vec.h>
#include <araui/ui_widget.h>
#include "ui_renderer.h"
#include "ui_window_internal.h"
#include "ui_widget_internal.h"
#include "ui_commons_internal.h"
#include "ui_math.h"
#include "ui_debug.h"
#include "dal/ui_dal.h"

#define MAX_RENDERER_MATRIX_STACK (256)
#define UI_TM (g_rc.tm_stack[g_rc.sp])

/****************************************************************************
 * Private function declaration
 ****************************************************************************/
static void _ui_fill_bottom_flat_triangle_uv(
	ui_coord_t coord1, ui_coord_t coord2, ui_coord_t coord3,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3);
static void _ui_fill_top_flat_triangle_uv(
	ui_coord_t coord1, ui_coord_t coord2, ui_coord_t coord3,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3);

/****************************************************************************
 * Private types
 ****************************************************************************/
typedef struct {
	ui_mat3_t tm_stack[MAX_RENDERER_MATRIX_STACK]; //!< Transform Matrix
	int32_t   sp; //!< TM stack pointer

	uint8_t          *texture;
	int32_t           tex_width;
	int32_t           tex_height;
	ui_pixel_format_t tex_pf;
} ui_render_context_t;

//!< Render context (global instance)
ui_render_context_t g_rc = {
	.tm_stack = { [0].m = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } } },
	.sp = 0,

	.texture = NULL,
	.tex_width = 0,
	.tex_height = 0,
	.tex_pf = UI_PIXEL_FORMAT_UNKNOWN
};

/****************************************************************************
 * Public function implementation
 ****************************************************************************/

ui_vec3_t ui_mat3_vec3_multiply(ui_mat3_t *mat, ui_vec3_t *vec)
{
	ui_vec3_t result;

	result.x = (mat->m[0][0] * vec->x) + (mat->m[0][1] * vec->y) + (mat->m[0][2] * vec->w);
	result.y = (mat->m[1][0] * vec->x) + (mat->m[1][1] * vec->y) + (mat->m[1][2] * vec->w);
	result.w = (mat->m[2][0] * vec->x) + (mat->m[2][1] * vec->y) + (mat->m[2][2] * vec->w);

	return result;
}

ui_mat3_t ui_mat3_identity(void)
{
	ui_mat3_t result;

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;

	return result;
}

ui_mat3_t ui_mat3_mat3_multiply(ui_mat3_t *left, ui_mat3_t *right)
{
	ui_mat3_t result;

	result.m[0][0] = (left->m[0][0] * right->m[0][0]) + (left->m[0][1] * right->m[1][0]) + (left->m[0][2] * right->m[2][0]);
	result.m[0][1] = (left->m[0][0] * right->m[0][1]) + (left->m[0][1] * right->m[1][1]) + (left->m[0][2] * right->m[2][1]);
	result.m[0][2] = (left->m[0][0] * right->m[0][2]) + (left->m[0][1] * right->m[1][2]) + (left->m[0][2] * right->m[2][2]);

	result.m[1][0] = (left->m[1][0] * right->m[0][0]) + (left->m[1][1] * right->m[1][0]) + (left->m[1][2] * right->m[2][0]);
	result.m[1][1] = (left->m[1][0] * right->m[0][1]) + (left->m[1][1] * right->m[1][1]) + (left->m[1][2] * right->m[2][1]);
	result.m[1][2] = (left->m[1][0] * right->m[0][2]) + (left->m[1][1] * right->m[1][2]) + (left->m[1][2] * right->m[2][2]);

	result.m[2][0] = (left->m[2][0] * right->m[0][0]) + (left->m[2][1] * right->m[1][0]) + (left->m[2][2] * right->m[2][0]);
	result.m[2][1] = (left->m[2][0] * right->m[0][1]) + (left->m[2][1] * right->m[1][1]) + (left->m[2][2] * right->m[2][1]);
	result.m[2][2] = (left->m[2][0] * right->m[0][2]) + (left->m[2][1] * right->m[1][2]) + (left->m[2][2] * right->m[2][2]);

	return result;
}

void ui_renderer_push_matrix(void)
{
	g_rc.sp++;
	UI_ASSERT(g_rc.sp < MAX_RENDERER_MATRIX_STACK);

	g_rc.tm_stack[g_rc.sp] = g_rc.tm_stack[g_rc.sp - 1];
}

void ui_renderer_pop_matrix(void)
{
	g_rc.sp--;
	UI_ASSERT(g_rc.sp >= 0);
}

void ui_renderer_load_identity(void)
{
	g_rc.tm_stack[g_rc.sp] = ui_mat3_identity();
}

void ui_renderer_translate(float x, float y)
{
	g_rc.tm_stack[g_rc.sp] = ui_mat3_mat3_multiply(&(ui_mat3_t) {
		.m = {
			{ 1.0f, 0.0f, x },
			{ 0.0f, 1.0f, y },
			{ 0.0f, 0.0f, 1.0f }
		}
	}, &g_rc.tm_stack[g_rc.sp]);
}

void ui_renderer_rotate(int32_t deg)
{
	float rad = (deg * UI_RENDERER_PI) / 180.0f;
	g_rc.tm_stack[g_rc.sp] = ui_mat3_mat3_multiply(&(ui_mat3_t) {
		.m = {
			{ cosf(rad), -sinf(rad), 0.0f },
			{ sinf(rad), cosf(rad), 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		}
	}, &g_rc.tm_stack[g_rc.sp]);
}

void ui_renderer_scale(float x, float y)
{
	g_rc.tm_stack[g_rc.sp] = ui_mat3_mat3_multiply(&(ui_mat3_t) {
		.m = {
			{ x, 0.0f, 0.0f },
			{ 0.0f, y, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		}
	}, &g_rc.tm_stack[g_rc.sp]);
}

void ui_renderer_set_texture(uint8_t *bitmap, int32_t width, int32_t height, ui_pixel_format_t pf)
{
	g_rc.texture = bitmap;

	if (bitmap) {
		g_rc.tex_width = width;
		g_rc.tex_height = height;
		g_rc.tex_pf = pf;
	} else {
		g_rc.tex_width = 0;
		g_rc.tex_height = 0;
		g_rc.tex_pf = UI_PIXEL_FORMAT_UNKNOWN;
	}
}

void ui_render_hline_uv(int32_t x, int32_t y, int32_t len, ui_uv_t uv[2])
{
	int32_t i;
	int32_t offset;
	float u_step = (uv[1].u - uv[0].u) / len;
	float v_step = (uv[1].v - uv[0].v) / len;
	float u = uv[0].u;
	float v = uv[0].v;
	int32_t iu, iv; //!< Convert to bitmap coord from uv coord
	ui_color_rgba8888_t tex_color;

	// If the 'uv' is passed, g_rc.texture must be set in advance.
	UI_ASSERT(uv && g_rc.texture);

	for (i = 0; i < len; i++) {
		iu = (int32_t)(u * (g_rc.tex_width - 1));
		iv = (int32_t)(v * (g_rc.tex_height - 1));

		// todo: Need to support various texture pixel format
		offset = ((iv * g_rc.tex_width) + iu) << 2; // For only RGBA texture

		// todo: Need to apply pixel interpolation
		tex_color.r = g_rc.texture[offset];
		tex_color.g = g_rc.texture[offset + 1];
		tex_color.b = g_rc.texture[offset + 2];
		tex_color.a = g_rc.texture[offset + 3];
		//ui_dal_put_pixel_rgba8888(x + i, y, *(ui_color_t *)&tex_color);
		ui_dal_put_pixel_rgba8888(x + i, y, 0xff0000ff);

		u += u_step;
		v += v_step;
	}
}

/***
 * IMPORTANT!
 * ----------
 * ui_render_triangle uses 'ui_coord_t' instead of 'ui_vec3_t'.
 * It means this function only supports 'int32_t' coordinate system.
 * In near future, this function will support the 'float' coordinate system.
 */
void ui_render_triangle_uv(ui_coord_t coord[3], ui_uv_t uv[3])
{
	int i;

	UI_ASSERT(coord && uv);

	ui_vec3_t _v;
	ui_coord_t _coord[3];
	ui_uv_t _uv[3];
	ui_coord_t mid_coord;

	// For prevent the parameters' original value,
	// need to copy it in advance.
	for (i = 0; i < 3; i++) {
		_v.x = (float)coord[i].x;
		_v.y = (float)coord[i].y;
		_v.w = 1.0f;
		_v = ui_mat3_vec3_multiply(&UI_TM, &_v);

		_coord[i].x = (int32_t)_v.x;
		_coord[i].y = (int32_t)_v.y;

		_uv[i] = uv[i];
	}

	// Sort by Y-coordiate
	if (_coord[1].y > _coord[2].y) {
		UI_SWAP(_coord[1], _coord[2]);
		UI_SWAP(_uv[1], _uv[2]);
	}
	if (_coord[0].y > _coord[1].y) {
		UI_SWAP(_coord[0], _coord[1]);
		UI_SWAP(_uv[0], _uv[1]);
	}
	if (_coord[1].y > _coord[2].y) {
		UI_SWAP(_coord[1], _coord[2]);
		UI_SWAP(_uv[1], _uv[2]);
	}

	if (_coord[1].y == _coord[2].y) {
		_ui_fill_bottom_flat_triangle_uv(_coord[0], _coord[1], _coord[2], _uv[0], _uv[1], _uv[2]);

	} else if (_coord[0].y == _coord[1].y) {
		_ui_fill_top_flat_triangle_uv(_coord[0], _coord[1], _coord[2], _uv[0], _uv[1], _uv[2]);

	} else {
		mid_coord = (ui_coord_t) {
			(int32_t)(_coord[0].x + ((float)(_coord[1].y - _coord[0].y) / (_coord[2].y - _coord[0].y)) * (_coord[2].x - _coord[0].x)),
			_coord[1].y,
		};

		ui_uv_t mid_uv = (ui_uv_t) {
			UI_GET_WEIGHTED_VALUE(_uv[0].u, _uv[2].u, (float)(_coord[1].y - _coord[0].y) / (_coord[2].y - _coord[0].y)),
			UI_GET_WEIGHTED_VALUE(_uv[0].v, _uv[2].v, (float)(_coord[1].y - _coord[0].y) / (_coord[2].y - _coord[0].y))
		};

		_ui_fill_bottom_flat_triangle_uv(_coord[0], _coord[1], mid_coord, _uv[0], _uv[1], mid_uv);
		_ui_fill_top_flat_triangle_uv(_coord[1], mid_coord, _coord[2], _uv[1], mid_uv, _uv[2]);

	}
}

void ui_render_quad_uv(ui_coord_t coord[4], ui_uv_t uv[4])
{
	ui_coord_t _coord[2][3];
	ui_uv_t _uv[2][3];

	_coord[0][0] = coord[0];
	_coord[0][1] = coord[1];
	_coord[0][2] = coord[2];

	_coord[1][0] = coord[0];
	_coord[1][1] = coord[2];
	_coord[1][2] = coord[3];

	_uv[0][0] = uv[0];
	_uv[0][1] = uv[1];
	_uv[0][2] = uv[2];

	_uv[1][0] = uv[0];
	_uv[1][1] = uv[2];
	_uv[1][2] = uv[3];

	ui_render_triangle_uv(_coord[0], _uv[0]);
	ui_render_triangle_uv(_coord[1], _uv[1]);
}

/****************************************************************************
 * Private function implementation
 ****************************************************************************/

static void _ui_fill_bottom_flat_triangle_uv(
	ui_coord_t coord1, ui_coord_t coord2, ui_coord_t coord3,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3
)
{
	ui_uv_t uv[2];
	float weight;
	int32_t scanline;
	int32_t x;
	int32_t len;

	int32_t height = (coord2.y - coord1.y) + 1;
	float invslope1 = (float)(coord2.x - coord1.x) / height;
	float invslope2 = (float)(coord3.x - coord1.x) / height;
	float curx1 = (float)coord1.x;
	float curx2 = (float)coord1.x;

	for (scanline = coord1.y; scanline <= coord2.y; scanline++) {
		weight = (float)(scanline - coord1.y + 1) / (height);

		uv[0].u = UI_GET_WEIGHTED_VALUE(uv1.u, uv2.u, weight);
		uv[0].v = UI_GET_WEIGHTED_VALUE(uv1.v, uv2.v, weight);
		uv[1].u = UI_GET_WEIGHTED_VALUE(uv1.u, uv3.u, weight);
		uv[1].v = UI_GET_WEIGHTED_VALUE(uv1.v, uv3.v, weight);

		if (curx2 >= curx1) {
			x = (int32_t)curx1;
			len = (int32_t)curx2 - x + 1;
		} else {
			x = (int32_t)curx2;
			len = (int32_t)curx1 - x + 1;
			UI_SWAP(uv[0], uv[1]);
		}

		ui_render_hline_uv(x, scanline, len, uv);

		curx1 += invslope1;
		curx2 += invslope2;
	}
}

static void _ui_fill_top_flat_triangle_uv(
	ui_coord_t coord1, ui_coord_t coord2, ui_coord_t coord3,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3
)
{
	ui_uv_t uv[2];
	float weight;
	int32_t scanline;
	int32_t x;
	int32_t len;

	int32_t height = (coord3.y - coord1.y) + 1;
	float invslope1 = (float)(coord3.x - coord1.x) / height;
	float invslope2 = (float)(coord3.x - coord2.x) / height;
	float curx1 = (float)coord3.x;
	float curx2 = (float)coord3.x;

	for (scanline = coord3.y; scanline >= coord1.y; scanline--) {
		weight = (float)(scanline - coord1.y + 1) / (height);

		uv[0].u = UI_GET_WEIGHTED_VALUE(uv1.u, uv3.u, weight);
		uv[0].v = UI_GET_WEIGHTED_VALUE(uv1.v, uv3.v, weight);
		uv[1].u = UI_GET_WEIGHTED_VALUE(uv2.u, uv3.u, weight);
		uv[1].v = UI_GET_WEIGHTED_VALUE(uv2.v, uv3.v, weight);

		if (curx2 >= curx1) {
			x = (int32_t)curx1;
			len = (int32_t)curx2 - x + 1;
		} else {
			x = (int32_t)curx2;
			len = (int32_t)curx1 - x + 1;
			UI_SWAP(uv[0], uv[1]);
		}

		ui_render_hline_uv(x, scanline, len, uv);

		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}
