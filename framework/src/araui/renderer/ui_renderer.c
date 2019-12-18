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
#include <float.h>
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

#define UI_SUB_DIVIDE_SHIFT (4)
#define UI_SUB_DIVIDE_SIZE (1 << UI_SUB_DIVIDE_SHIFT)
#define UI_SUB_PIX(a) (ceilf(a) - (a))

#define CONFIG_UI_DEFAULT_FILL_COLOR 0x000000

/****************************************************************************
 * Private function declaration
 ****************************************************************************/
static void ui_draw_triangle_segment(int32_t y1, int32_t y2);

/****************************************************************************
 * Private types
 ****************************************************************************/
typedef struct {
	uint8_t          *texture;
	int32_t           tex_width;
	int32_t           tex_height;
	ui_pixel_format_t tex_pf;
	ui_color_t        fill_color;
} ui_render_context_t;

//!< Render context (global instance)
ui_render_context_t g_rc = {
	.texture = NULL,
	.tex_width = 0,
	.tex_height = 0,
	.tex_pf = UI_PIXEL_FORMAT_UNKNOWN,
	.fill_color = CONFIG_UI_DEFAULT_FILL_COLOR
};

float g_left_dxdy;
float g_right_dxdy;
float g_leftx;
float g_rightx;
float g_left_dudy;
float g_leftu;
float g_left_dvdy;
float g_leftv;
float g_left_dzdy;
float g_leftz;
float g_pk_dudx;
float g_pk_dvdx;
float g_pk_dzdx;
float g_pk_dudx_;
float g_pk_dvdx_;
float g_pk_dzdx_;

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

void ui_renderer_translate(ui_mat3_t *parent_mat, ui_mat3_t *mat, float x, float y)
{
	*mat = ui_mat3_mat3_multiply(parent_mat, &(ui_mat3_t) {
		.m = {
			{ 1.0f, 0.0f, x },
			{ 0.0f, 1.0f, y },
			{ 0.0f, 0.0f, 1.0f }
		}
	});
}

void ui_renderer_rotate(ui_mat3_t *mat, int32_t deg)
{
	float rad = (deg * UI_RENDERER_PI) / 180.0f;

	*mat = ui_mat3_mat3_multiply(mat, &(ui_mat3_t) {
		.m = {
			{ cosf(rad), -sinf(rad), 0.0f },
			{ sinf(rad), cosf(rad), 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		}
	});
}

void ui_renderer_scale(ui_mat3_t *mat, float x, float y)
{
	*mat = ui_mat3_mat3_multiply(mat, &(ui_mat3_t) {
		.m = {
			{ x, 0.0f, 0.0f },
			{ 0.0f, y, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		}
	});
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

void ui_renderer_set_fill_color(ui_color_t color)
{
	g_rc.fill_color = color;
}

void ui_render_triangle_uv(ui_mat3_t *trans_mat,
	ui_vec3_t v1, ui_vec3_t v2, ui_vec3_t v3,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3)
{
	float u_a;
	float v_a;
	float z_a;
	float u_b;
	float v_b;
	float z_b;
	float u_c;
	float v_c;
	float z_c;
	int32_t y1i;
	int32_t y2i;
	int32_t y3i;
	float prestep;
	float dXdY_V1V3;
	float dXdY_V2V3;
	float dXdY_V1V2;
	float dUdY_V1V3;
	float dUdY_V2V3;
	float dUdY_V1V2;
	float dVdY_V1V3;
	float dVdY_V2V3;
	float dVdY_V1V2;
	float dZdY_V1V3;
	float dZdY_V2V3;
	float dZdY_V1V2;
	float denom;

	v1 = ui_mat3_vec3_multiply(trans_mat, &v1);
	v2 = ui_mat3_vec3_multiply(trans_mat, &v2);
	v3 = ui_mat3_vec3_multiply(trans_mat, &v3);

	if (v1.y > v2.y) {
		UI_SWAP(v1, v2);
		UI_SWAP(uv1, uv2);
	}
	if (v1.y > v3.y) {
		UI_SWAP(v1, v3);
		UI_SWAP(uv1, uv3);
	}
	if (v2.y > v3.y) {
		UI_SWAP(v2, v3);
		UI_SWAP(uv2, uv3);
	}

	y1i = (int32_t)ceilf(v1.y);
	y2i = (int32_t)ceilf(v2.y);
	y3i = (int32_t)ceilf(v3.y);

	if (y1i == y3i) {
		return;
	}

	u_a = uv1.u;
	u_b = uv2.u;
	u_c = uv3.u;
	v_a = uv1.v;
	v_b = uv2.v;
	v_c = uv3.v;
	z_a = 1.0f;
	z_b = 1.0f;
	z_c = 1.0f;

	dXdY_V1V3 = (v3.x - v1.x) / (v3.y - v1.y);
	dXdY_V2V3 = (v3.x - v2.x) / (v3.y - v2.y);
	dXdY_V1V2 = (v2.x - v1.x) / (v2.y - v1.y);

	dUdY_V1V3 = (u_c - u_a) / (v3.y - v1.y);
	dUdY_V2V3 = (u_c - u_b) / (v3.y - v2.y);
	dUdY_V1V2 = (u_b - u_a) / (v2.y - v1.y);

	dVdY_V1V3 = (v_c - v_a) / (v3.y - v1.y);
	dVdY_V2V3 = (v_c - v_b) / (v3.y - v2.y);
	dVdY_V1V2 = (v_b - v_a) / (v2.y - v1.y);

	dZdY_V1V3 = (z_c - z_a) / (v3.y - v1.y);
	dZdY_V2V3 = (z_c - z_b) / (v3.y - v2.y);
	dZdY_V1V2 = (z_b - z_a) / (v2.y - v1.y);

	denom = ((v3.x - v1.x) * (v2.y - v1.y) - (v2.x - v1.x) * (v3.y - v1.y));

	if (!denom) {
		return;
	}

	denom = 1.0f / denom;

	g_pk_dudx = ((u_c - u_a) * (v2.y - v1.y) - (u_b - u_a) * (v3.y - v1.y)) * denom;
	g_pk_dvdx = ((v_c - v_a) * (v2.y - v1.y) - (v_b - v_a) * (v3.y - v1.y)) * denom;
	g_pk_dzdx = ((z_c - z_a) * (v2.y - v1.y) - (z_b - z_a) * (v3.y - v1.y)) * denom;

	g_pk_dudx_ = g_pk_dudx * UI_SUB_DIVIDE_SIZE;
	g_pk_dvdx_ = g_pk_dvdx * UI_SUB_DIVIDE_SIZE;
	g_pk_dzdx_ = g_pk_dzdx * UI_SUB_DIVIDE_SIZE;

	bool mid = dXdY_V1V3 < dXdY_V1V2;
	if (!mid) {
		prestep = UI_SUB_PIX(v1.y);
		if (y1i == y2i) {

			g_left_dudy = dUdY_V2V3;
			g_left_dvdy = dVdY_V2V3;
			g_left_dzdy = dZdY_V2V3;
			g_left_dxdy = dXdY_V2V3;
			g_right_dxdy = dXdY_V1V3;

			g_leftu = u_b + UI_SUB_PIX(v2.y) * g_left_dudy;
			g_leftv = v_b + UI_SUB_PIX(v2.y) * g_left_dvdy;
			g_leftz = z_b + UI_SUB_PIX(v2.y) * g_left_dzdy;
			g_leftx = v2.x + UI_SUB_PIX(v2.y) * g_left_dxdy;
			g_rightx = v1.x + prestep * g_right_dxdy;

			ui_draw_triangle_segment(y1i, y3i);

			return;
		}

		g_right_dxdy = dXdY_V1V3;

		if (y1i < y2i) {

			g_left_dudy = dUdY_V1V2;
			g_left_dvdy = dVdY_V1V2;
			g_left_dzdy = dZdY_V1V2;
			g_left_dxdy = dXdY_V1V2;

			g_leftu = u_a + prestep * g_left_dudy;
			g_leftv = v_a + prestep * g_left_dvdy;
			g_leftz = z_a + prestep * g_left_dzdy;
			g_leftx = v1.x + prestep * g_left_dxdy;
			g_rightx = v1.x + prestep * g_right_dxdy;

			ui_draw_triangle_segment(y1i, y2i);
		}

		if (y2i < y3i) {

			g_left_dxdy = dXdY_V2V3;
			g_left_dudy = dUdY_V2V3;
			g_left_dvdy = dVdY_V2V3;
			g_left_dzdy = dZdY_V2V3;

			g_leftu = u_b + UI_SUB_PIX(v2.y) * g_left_dudy;
			g_leftv = v_b + UI_SUB_PIX(v2.y) * g_left_dvdy;
			g_leftz = z_b + UI_SUB_PIX(v2.y) * g_left_dzdy;
			g_leftx = v2.x + UI_SUB_PIX(v2.y) * g_left_dxdy;

			ui_draw_triangle_segment(y2i, y3i);
		}
	} else if (mid) {

		prestep = UI_SUB_PIX(v1.y);

		if (y1i == y2i) {

			g_left_dudy = dUdY_V1V3;
			g_left_dvdy = dVdY_V1V3;
			g_left_dzdy = dZdY_V1V3;
			g_left_dxdy = dXdY_V1V3;
			g_right_dxdy = dXdY_V2V3;

			g_leftu = u_a + prestep * g_left_dudy;
			g_leftv = v_a + prestep * g_left_dvdy;
			g_leftz = z_a + prestep * g_left_dzdy;
			g_leftx = v1.x + prestep * g_left_dxdy;
			g_rightx = v2.x + UI_SUB_PIX(v2.y) * g_right_dxdy;

			ui_draw_triangle_segment(y1i, y3i);
			return;
		}

		g_left_dxdy = dXdY_V1V3;
		g_left_dudy = dUdY_V1V3;
		g_left_dvdy = dVdY_V1V3;
		g_left_dzdy = dZdY_V1V3;

		if (y1i < y2i) {

			g_right_dxdy = dXdY_V1V2;

			g_leftu = u_a + prestep * g_left_dudy;
			g_leftv = v_a + prestep * g_left_dvdy;
			g_leftz = z_a + prestep * g_left_dzdy;
			g_leftx = v1.x + prestep * g_left_dxdy;
			g_rightx = v1.x + prestep * g_right_dxdy;

			ui_draw_triangle_segment(y1i, y2i);
		}

		if (y2i < y3i) {
			g_right_dxdy = dXdY_V2V3;
			g_rightx = v2.x + UI_SUB_PIX(v2.y) * g_right_dxdy;

			ui_draw_triangle_segment(y2i, y3i);
		}
	}
}

void ui_render_quad_uv(ui_mat3_t *trans_mat,
	ui_vec3_t v1, ui_vec3_t v2, ui_vec3_t v3, ui_vec3_t v4,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3, ui_uv_t uv4)
{
	ui_render_triangle_uv(trans_mat, v1, v2, v3, uv1, uv2, uv3);
	ui_render_triangle_uv(trans_mat, v1, v3, v4, uv1, uv3, uv4);
}

/****************************************************************************
 * Private function implementation
 ****************************************************************************/
static void ui_draw_triangle_segment(int32_t y1, int32_t y2)
{
	float u;
	float v;
	float z;
	float Z;
	int32_t du;
	int32_t dv;
	int32_t width;
	int32_t U;
	int32_t V;
	int32_t U1;
	int32_t V1;
	int32_t U2;
	int32_t V2;
	int32_t x1;
	int32_t x2;
	int32_t y;
	int32_t x;
	int32_t iu;
	int32_t iv;
	int32_t uv_offset;

	for (y = y1; y < y2; y++) {

		x1 = ceilf(g_leftx);
		x2 = ceilf(g_rightx);

		u = g_leftu + UI_SUB_PIX(g_leftx) * g_pk_dudx;
		v = g_leftv + UI_SUB_PIX(g_leftx) * g_pk_dvdx;
		z = g_leftz + UI_SUB_PIX(g_leftx) * g_pk_dzdx;

		Z = 65536.0f;
		U2 = u * Z;
		V2 = v * Z;
		width = x2 - x1;

		while (width >= UI_SUB_DIVIDE_SIZE) {

			u += g_pk_dudx_;
			v += g_pk_dvdx_;
			z += g_pk_dzdx_;

			U1 = U2;
			V1 = V2;

			Z = 65536.0f;
			U2 = u * Z;
			V2 = v * Z;

			du = (U2 - U1) >> UI_SUB_DIVIDE_SHIFT;
			dv = (V2 - V1) >> UI_SUB_DIVIDE_SHIFT;
			U = U1;
			V = V1;
			x = UI_SUB_DIVIDE_SIZE;

			while (x--) {
				iu = (int32_t)((U / 65536.0f) * (g_rc.tex_width - 1) + 0.5f);
				iv = (int32_t)((V / 65536.0f) * (g_rc.tex_height - 1) + 0.5f);

				if (g_rc.tex_pf == UI_PIXEL_FORMAT_RGBA8888) {
					uv_offset = ((iv * g_rc.tex_width) + iu) * 4;
					ui_dal_put_pixel_rgba8888(x1++, y, UI_COLOR_RGBA8888(
						g_rc.texture[uv_offset + 0],
						g_rc.texture[uv_offset + 1],
						g_rc.texture[uv_offset + 2],
						g_rc.texture[uv_offset + 3]
					));
				} else if (g_rc.tex_pf == UI_PIXEL_FORMAT_RGB888) {
					uv_offset = ((iv * g_rc.tex_width) + iu) * 3;
					ui_dal_put_pixel_rgb888(x1++, y, UI_COLOR_RGB888(
						g_rc.texture[uv_offset + 0],
						g_rc.texture[uv_offset + 1],
						g_rc.texture[uv_offset + 2]
					));
				} else if (g_rc.tex_pf == UI_PIXEL_FORMAT_A8) {
					uv_offset = ((iv * g_rc.tex_width) + iu);
					ui_dal_put_pixel_rgba8888(x1++, y, UI_COLOR_RGBA8888(
						(g_rc.fill_color & 0xff0000) >> 16,
						(g_rc.fill_color & 0x00ff00) >> 8,
						(g_rc.fill_color & 0x0000ff) >> 0,
						g_rc.texture[uv_offset]
					));
				}

				U += du;
				V += dv;
			}

			width -= UI_SUB_DIVIDE_SIZE;
		}

		if (width > 0) {

			U1 = U2;
			V1 = V2;

			u += (g_pk_dudx * width);
			v += (g_pk_dvdx * width);
			z += (g_pk_dzdx * width);

			Z = 65536.0f;
			U2 = u * Z;
			V2 = v * Z;

			du = (U2 - U1) / width;
			dv = (V2 - V1) / width;
			U = U1;
			V = V1;

			while (width--) {
				iu = (int32_t)((U / 65536.0f) * (g_rc.tex_width - 1) + 0.5f);
				iv = (int32_t)((V / 65536.0f) * (g_rc.tex_height - 1) + 0.5f);

				if (g_rc.tex_pf == UI_PIXEL_FORMAT_RGBA8888) {
					uv_offset = ((iv * g_rc.tex_width) + iu) * 4;
					ui_dal_put_pixel_rgba8888(x1++, y, UI_COLOR_RGBA8888(
						g_rc.texture[uv_offset + 0],
						g_rc.texture[uv_offset + 1],
						g_rc.texture[uv_offset + 2],
						g_rc.texture[uv_offset + 3]
					));
				} else if (g_rc.tex_pf == UI_PIXEL_FORMAT_RGB888) {
					uv_offset = ((iv * g_rc.tex_width) + iu) * 3;
					ui_dal_put_pixel_rgb888(x1++, y, UI_COLOR_RGB888(
						g_rc.texture[uv_offset + 0],
						g_rc.texture[uv_offset + 1],
						g_rc.texture[uv_offset + 2]
					));
				} else if (g_rc.tex_pf == UI_PIXEL_FORMAT_A8) {
					uv_offset = ((iv * g_rc.tex_width) + iu);
					ui_dal_put_pixel_rgba8888(x1++, y, UI_COLOR_RGBA8888(
						(g_rc.fill_color & 0xff0000) >> 16,
						(g_rc.fill_color & 0x00ff00) >> 8,
						(g_rc.fill_color & 0x0000ff) >> 0,
						g_rc.texture[uv_offset]
					));
				}

				U += du;
				V += dv;
			}
		}

		g_leftu += g_left_dudy;
		g_leftv += g_left_dvdy;
		g_leftz += g_left_dzdy;
		g_leftx += g_left_dxdy;
		g_rightx += g_right_dxdy;
	}
}

