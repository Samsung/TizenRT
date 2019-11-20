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
#ifndef __UI_RENDERER_H__
#define __UI_RENDERER_H__

#include <stdint.h>
#include <araui/ui_commons.h>

/**
 * Constant value macros
 */
#define UI_RENDERER_PI (3.1415926535f)

/**
 * @brief Get weighted value between from ~ to.
 * When t is 0.0f, it returns 'from' value. When t is 1.0f, it returns 'to' value.
 */
#define UI_GET_WEIGHTED_VALUE(from, to, t) ((from) + ((t) * ((to) - (from))))

/**
 * @brief 3-dimentional vector structure (x, y, w)
 */
typedef struct {
	float x;
	float y;
	float w;
} ui_vec3_t;

/**
 * @brief 2-dimentional uv structure (u, v)
 */
typedef struct {
	float u;
	float v;
} ui_uv_t;

/**
 * @brief 3x3 matrix structure
 */
typedef struct {
	float m[3][3];
} ui_mat3_t;

/**
 * @brief Graphics common functions
 */
ui_vec3_t ui_mat3_vec3_multiply(ui_mat3_t *mat, ui_vec3_t *vec);
ui_mat3_t ui_mat3_identity(void);
ui_mat3_t ui_mat3_mat3_multiply(ui_mat3_t *left, ui_mat3_t *right);
float ui_get_weighted_value(float from, float to, float weight);

/**
 * @brief UI Renderer context configuration functions
 */
void ui_renderer_push_matrix(void);
void ui_renderer_pop_matrix(void);
void ui_renderer_load_identity(void);
void ui_renderer_translate(ui_mat3_t *parent_mat, ui_mat3_t *mat, float x, float y);
void ui_renderer_rotate(ui_mat3_t *mat, int32_t deg);
void ui_renderer_scale(ui_mat3_t *mat, float x, float y);
void ui_renderer_set_texture(uint8_t *bitmap, int32_t width, int32_t height, ui_pixel_format_t pf);
void ui_renderer_set_fill_color(ui_color_t color);

/**
 * @brief Rendering geometry functions
 * 
 * - ui_render_{geometry}_col: Render geometry with color gradient.
 * - ui_render_{geometry}_uv: Render geometry with texture uv.
 * - ui_render_{geometry}_col_uv: Render geometry with multiply of texture uv and color gradient.
 */
void ui_render_triangle_uv(ui_mat3_t *trans_mat,
	ui_vec3_t v1, ui_vec3_t v2, ui_vec3_t v3,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3);
void ui_render_quad_uv(ui_mat3_t *trans_mat,
	ui_vec3_t v1, ui_vec3_t v2, ui_vec3_t v3, ui_vec3_t v4,
	ui_uv_t uv1, ui_uv_t uv2, ui_uv_t uv3, ui_uv_t uv4);

#endif // __UI_RENDERER_H__
