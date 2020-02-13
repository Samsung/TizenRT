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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stb/stb_truetype.h>
#include <araui/ui_commons.h>
#include <araui/ui_widget.h>
#include "ui_request_callback.h"
#include "ui_debug.h"
#include "ui_commons_internal.h"
#include "ui_core_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "ui_window_internal.h"
#include "dal/ui_dal.h"

#if defined(CONFIG_UI_ENABLE_EMOJI)
#include "utils/emoji.h"
#endif

typedef struct {
	ui_text_widget_body_t *body;
	char *text;
} ui_set_text_info_t;

typedef struct {
	ui_text_widget_body_t *body;
	ui_align_t align;
} ui_set_align_info_t;

typedef struct {
	ui_text_widget_body_t *body;
	ui_color_t color;
} ui_set_color_info_t;

typedef struct {
	ui_text_widget_body_t *body;
	bool word_wrap;
} ui_set_word_wrap_info_t;

typedef struct {
	ui_text_widget_body_t *body;
	size_t font_size;
} ui_set_font_size_info_t;

#define CONFIG_UI_TEXT_FORMAT_MAX_LENGTH  512
#define CONFIG_UI_GLYPH_BITMAP_WIDTH      256
#define CONFIG_UI_GLYPH_BITMAP_HEIGHT     256
#define CONFIG_UI_DEFAULT_FILL_COLOR      0x000000

static ui_error_t _ui_text_widget_text2utf(ui_text_widget_body_t *body, const char *text);
static void _ui_text_widget_render_func(ui_widget_t widget, uint32_t dt);
static void _ui_text_widget_removed_func(ui_widget_t widget);
static void _ui_text_widget_set_text_func(void *userdata);
static void _ui_text_widget_set_align_func(void *userdata);
static void _ui_text_widget_set_color(void *userdata);
static void _ui_text_widget_set_word_wrap_func(void *userdata);
static void _ui_text_widget_set_font_size_func(void *userdata);
static void _ui_text_widget_calculate_line_num(ui_text_widget_body_t *body);

static uint8_t g_glyph_bitmap[CONFIG_UI_GLYPH_BITMAP_WIDTH * CONFIG_UI_GLYPH_BITMAP_HEIGHT];

ui_widget_t ui_text_widget_create(int32_t width, int32_t height, ui_asset_t font, const char *text, size_t font_size)
{
	ui_text_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	// Now, there is no API that set a font asset of the text widget, font must be set at HERE.
	// Text can be set later by using ui_text_widget_set_text().
	if (!font || !ui_asset_check_type(font, UI_FONT_ASSET)) {
		UI_LOGE("error: invalid parameter!\n");
		return UI_NULL;
	}

	body = (ui_text_widget_body_t *)UI_ALLOC(sizeof(ui_text_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_text_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, width, height);
	((ui_widget_body_t *)body)->type = UI_TEXT_WIDGET;
	body->font = (ui_font_asset_body_t *)font;
	body->font_size = font_size;

	if (text) {
		if (_ui_text_widget_text2utf(body, text) != UI_OK) {
			ui_widget_deinit((ui_widget_body_t *)body);
			UI_FREE(body);
			UI_LOGE("error: out of memory!\n");
			return UI_NULL;
		}
	}

	body->align = UI_ALIGN_DEFAULT;

	// Fill the color variable by 0xff for white color
	memset(&body->font_color, 0xff, sizeof(body->font_color));

	body->base.render_cb = _ui_text_widget_render_func;
	body->base.remove_cb = _ui_text_widget_removed_func;

	return (ui_widget_t)body;
}

static ui_error_t _ui_text_widget_text2utf(ui_text_widget_body_t *body, const char *text)
{
	size_t length = 0;
	size_t utf_idx = 0;
	size_t str_idx = 0;
	uint8_t b0, b1, b2;

	if (!body || !text) {
		return UI_INVALID_PARAM;
	}

	length = strlen(text);
	if (!length) {
		body->utf_code = NULL;
		body->width_array = NULL;
		body->text_length = length;
		body->line_num = 0;

		return UI_OK;
	}

	body->utf_code = (uint32_t *)UI_ALLOC(length * sizeof(uint32_t));
	if (!body->utf_code) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	body->width_array = (uint32_t *)UI_ALLOC(length * sizeof(uint32_t));
	if (!body->width_array) {
		UI_FREE(body->utf_code);
		return UI_NOT_ENOUGH_MEMORY;
	}

	body->text_length = length;

	// Convert char array to UTF8 code array
	while (str_idx < length) {
		/**
		 * 7-bits UTF-8
		 */
		if ((text[str_idx] & 0x80) != 0x80) {
			body->utf_code[utf_idx] = (uint32_t)text[str_idx];
			str_idx++;

		/**
		 * 16-bits UTF-8 (Such as Korean, Japanese, Chinese)
		 */
		} else if (
			(str_idx + 2 < length) &&
			((text[str_idx] & 0xf0) == 0xe0) &&
			((text[str_idx + 1] & 0xc0) == 0x80) &&
			((text[str_idx + 2] & 0xc0) == 0x80)) {

			b1 = b0 = 0;
			b1 |= (text[str_idx] & 0x0f) << 4;
			b1 |= (text[str_idx + 1] & 0x3c) >> 2;
			b0 |= (text[str_idx + 1] & 0x03) << 6;
			b0 |= (text[str_idx + 2] & 0x3f);

			body->utf_code[utf_idx] = (uint16_t)(b1 << 8) + b0;
			str_idx += 3;

		/**
		 * 21-bits UTF-8 (Such as Emoji)
		 */
		} else if (
			(str_idx + 3 < length) &&
			((text[str_idx] & 0xf8) == 0xf0) &&
			((text[str_idx + 1] & 0xc0) == 0x80) &&
			((text[str_idx + 2] & 0xc0) == 0x80) &&
			((text[str_idx + 3] & 0xc0) == 0x80)) {

			b2 = b1 = b0 = 0;
			b2 |= (text[str_idx] & 0x07) << 2;
			b2 |= (text[str_idx + 1] & 0x30) >> 4;
			b1 |= (text[str_idx + 1] & 0x0f) << 4;
			b1 |= (text[str_idx + 2] & 0x3c) >> 2;
			b0 |= (text[str_idx + 2] & 0x03) << 6;
			b0 |= (text[str_idx + 3] & 0x3f);

			body->utf_code[utf_idx] = (uint32_t)(b2 << 16) + (b1 << 8) + b0;
			str_idx += 4;
		}
		utf_idx++;
	}

	body->text_length = utf_idx;
	_ui_text_widget_calculate_line_num(body);

	return UI_OK;
}

ui_error_t ui_text_widget_set_text(ui_widget_t widget, const char *text)
{
	size_t length = 0;
	ui_set_text_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_TEXT_WIDGET) || !text) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_text_info_t *)UI_ALLOC(sizeof(ui_set_text_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_text_info_t));

	length = strlen(text);

	info->body = (ui_text_widget_body_t *)widget;

	info->text = (char *)UI_ALLOC(length + 1);
	if (!info->text) {
		UI_FREE(info);
		return UI_NOT_ENOUGH_MEMORY;
	}

	strncpy(info->text, text, length + 1);

	if (ui_request_callback(_ui_text_widget_set_text_func, info) != UI_OK) {
		UI_FREE(info->text);
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

ui_error_t ui_text_widget_set_text_format(ui_widget_t widget, const char *format, ...)
{
	size_t length = 0;
	ui_set_text_info_t *info;
	va_list ap;
	char text[CONFIG_UI_TEXT_FORMAT_MAX_LENGTH];

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_TEXT_WIDGET) || !format) {
		return UI_INVALID_PARAM;
	}

	va_start(ap, format);
	vsnprintf(text, CONFIG_UI_TEXT_FORMAT_MAX_LENGTH, format, ap);
	va_end(ap);

	info = (ui_set_text_info_t *)UI_ALLOC(sizeof(ui_set_text_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_text_info_t));

	info->body = (ui_text_widget_body_t *)widget;

	length = strlen(text);
	info->text = (char *)UI_ALLOC(length + 1);
	if (!info->text) {
		UI_FREE(info);
		return UI_NOT_ENOUGH_MEMORY;
	}
	strncpy(info->text, text, length + 1);

	if (ui_request_callback(_ui_text_widget_set_text_func, info) != UI_OK) {
		UI_FREE(info->text);
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_text_widget_set_text_func(void *userdata)
{
	ui_set_text_info_t *info;
	ui_text_widget_body_t *body;
	char *text;

	if (!userdata || !((ui_set_text_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_text_info_t *)userdata;
	body = info->body;
	text = info->text;

	UI_FREE(body->utf_code);
	UI_FREE(body->width_array);

	if (_ui_text_widget_text2utf(body, text) != UI_OK) {
		UI_LOGE("error: out of memory!\n");
		UI_FREE(info->text);
		UI_FREE(info);
		return;
	}

	body->base.update_flag = true;

	UI_FREE(info->text);
	UI_FREE(info);
}

ui_error_t ui_text_widget_set_align(ui_widget_t widget, ui_align_t align)
{
	ui_set_align_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_TEXT_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_align_info_t *)UI_ALLOC(sizeof(ui_set_align_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_align_info_t));

	info->body = (ui_text_widget_body_t *)widget;
	info->align = align;

	if (ui_request_callback(_ui_text_widget_set_align_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_text_widget_set_align_func(void *userdata)
{
	ui_set_align_info_t *info;

	if (!userdata || !((ui_set_align_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_align_info_t *)userdata;

	info->body->align = info->align;
	info->body->base.update_flag = true;

	UI_FREE(info);
}

ui_error_t ui_text_widget_set_color(ui_widget_t widget, ui_color_t color)
{
	ui_set_color_info_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || color < 0) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_color_info_t *)UI_ALLOC(sizeof(ui_set_color_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	info->body = (ui_text_widget_body_t *)widget;
	info->color = color;

	if (ui_request_callback(_ui_text_widget_set_color, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_text_widget_set_color(void *userdata)
{
	ui_set_color_info_t *info;

	if (!userdata || !((ui_set_color_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_color_info_t *)userdata;
	info->body->font_color = info->color;

	UI_FREE(info);
}

static void _ui_text_widget_render_func(ui_widget_t widget, uint32_t dt)
{
	ui_text_widget_body_t *body;
	float scale;
	int ascent;
	int i;
	int c_x1;
	int c_y1;
	int c_x2;
	int c_y2;
	int out_w;
	int out_h;
	int x;
	int y;
	int32_t text_width;
	size_t utf_idx = 0;
	size_t draw_idx = 0;
#if defined(CONFIG_UI_DISPLAY_RGB888)
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0;
#endif
	ui_vec3_t v1;
	ui_vec3_t v2;
	ui_vec3_t v3;
	ui_vec3_t v4;
	ui_mat3_t text_mat;

#if defined(CONFIG_UI_ENABLE_EMOJI)
	ui_bitmap_data_t *emoji_bitmap;
	ui_vec3_t emoji_v1;
	ui_vec3_t emoji_v2;
	ui_vec3_t emoji_v3;
	ui_vec3_t emoji_v4;
#endif

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_text_widget_body_t *)widget;

	if (!body->text_length) {
		UI_LOGD("Empty text widget!\n");
		return;
	}

	memset(g_glyph_bitmap, 0, CONFIG_UI_GLYPH_BITMAP_WIDTH * CONFIG_UI_GLYPH_BITMAP_HEIGHT);

	scale = stbtt_ScaleForPixelHeight(&(body->font->ttf_info), body->font_size);

	stbtt_GetFontVMetrics(&(body->font->ttf_info), &ascent, NULL, NULL);
	ascent *= scale;

	x = 0;
	y = 0;

	if (body->align & UI_ALIGN_MIDDLE) {
		y = (body->base.global_rect.height - ((int32_t)body->line_num * body->font_size)) >> 1;
	} else if (body->align & UI_ALIGN_BOTTOM) {
		y = (body->base.global_rect.height - ((int32_t)body->line_num * body->font_size));
	}

	for (i = 0; i < body->line_num; i++) {
		// Calculate the width of text
		text_width = 0;
		while (body->utf_code[utf_idx] != '\n') {
			text_width += body->width_array[utf_idx];

			if (body->word_wrap) {
				if (text_width > body->base.local_rect.width) {
					// As the width of text exceeds the width of text widget,
					// this utf code will be printed at the next line.
					text_width -= body->width_array[utf_idx];
					utf_idx--;
					break;
				}
			}

			if (utf_idx >= (body->text_length - 1)) {
				break;
			} else {
				utf_idx++;
			}
		}

		utf_idx++;

		// Calculate proper x coordinate according to align
		if (body->align & UI_ALIGN_LEFT) {
			x = 0;
		} else if (body->align & UI_ALIGN_CENTER) {
			x = ((body->base.global_rect.width - text_width) >> 1);
		} else if (body->align & UI_ALIGN_RIGHT) {
			x = (body->base.global_rect.width - text_width);
		}

#if defined(CONFIG_UI_DISPLAY_RGB888)
		a = ((ui_color_argb8888_t *)&(body->font_color))->a;
		r = ((ui_color_argb8888_t *)&(body->font_color))->r;
		g = ((ui_color_argb8888_t *)&(body->font_color))->g;
		b = ((ui_color_argb8888_t *)&(body->font_color))->b;
#endif

		while (draw_idx < utf_idx) {
			if (body->utf_code[draw_idx] == '\n') {
				draw_idx++;
				continue;
			}

#if defined(CONFIG_UI_ENABLE_EMOJI)
			// If the code is emoji
			if (is_emoji(body->utf_code[draw_idx])) {
				emoji_bitmap = emoji_get_bitmap(body->utf_code[draw_idx]);
				if (emoji_bitmap) {
					ui_renderer_set_texture(
						((uint8_t *)emoji_bitmap) + sizeof(ui_bitmap_data_t),
						emoji_bitmap->width,
						emoji_bitmap->height,
						emoji_bitmap->pf);

						emoji_v1 = (ui_vec3_t){ .x = x - body->base.global_rect.x, .y = y - body->base.global_rect.y, .w = 1.0f };
						emoji_v2 = (ui_vec3_t){ .x = x - body->base.global_rect.x, .y = y - body->base.global_rect.y + body->font_size, .w = 1.0f };
						emoji_v3 = (ui_vec3_t){ .x = x - body->base.global_rect.x + body->font_size, .y = y - body->base.global_rect.y + body->font_size, .w = 1.0f };
						emoji_v4 = (ui_vec3_t){ .x = x - body->base.global_rect.x + body->font_size, .y = y - body->base.global_rect.y, .w = 1.0f };

						ui_render_quad_uv(&body->base.trans_mat, emoji_v1, emoji_v2, emoji_v3, emoji_v4,
							(ui_uv_t){ 0.0f, 0.0f },
							(ui_uv_t){ 0.0f, 1.0f },
							(ui_uv_t){ 1.0f, 1.0f },
							(ui_uv_t){ 1.0f, 0.0f });

						ui_renderer_set_texture(NULL, 0, 0, UI_PIXEL_FORMAT_UNKNOWN);
				}
				x += body->font_size;
			} else {
#endif
				/* get bounding box for character (may be offset to account for chars that dip above or below the line */
				stbtt_GetCodepointBitmapBox(&(body->font->ttf_info), body->utf_code[draw_idx],
					scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

				out_w = c_x2 - c_x1;
				out_h = c_y2 - c_y1;

				/* render character (stride and offset is important here) */
				stbtt_MakeCodepointBitmap(&(body->font->ttf_info), g_glyph_bitmap,
					out_w, out_h,
					out_w,
					scale, scale,
					body->utf_code[draw_idx]);

				ui_renderer_translate(&body->base.trans_mat, &text_mat, (float)x, (float)(y + ascent + c_y1));
				ui_renderer_set_texture(g_glyph_bitmap, out_w, out_h, UI_PIXEL_FORMAT_A8);
				ui_renderer_set_fill_color(body->font_color);

				v1 = (ui_vec3_t){
					.x = 0.0f,
					.y = 0.0f,
					1.0f
				};
				v2 = (ui_vec3_t){
					.x = 0.0f,
					.y = out_h,
					1.0f
				};
				v3 = (ui_vec3_t){
					.x = out_w,
					.y = out_h,
					1.0f
				};
				v4 = (ui_vec3_t){
					.x = out_w,
					.y = 0.0f,
					1.0f
				};

				ui_render_quad_uv(&text_mat, v1, v2, v3, v4,
							(ui_uv_t){ 0.0f, 0.0f },
							(ui_uv_t){ 0.0f, 1.0f },
							(ui_uv_t){ 1.0f, 1.0f },
							(ui_uv_t){ 1.0f, 0.0f });

				ui_renderer_set_texture(NULL, 0, 0, UI_PIXEL_FORMAT_UNKNOWN);
				ui_renderer_set_fill_color(CONFIG_UI_DEFAULT_FILL_COLOR);
				memset(g_glyph_bitmap, 0, out_w * out_h);

				x += body->width_array[draw_idx];
#if defined(CONFIG_UI_ENABLE_EMOJI)
			}
#endif
			draw_idx++;
		}

		y += body->font_size;
	}
}

static void _ui_text_widget_removed_func(ui_widget_t widget)
{
	ui_text_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_text_widget_body_t *)widget;

	UI_FREE(body->utf_code);
	UI_FREE(body->width_array);
}

ui_error_t ui_text_widget_set_word_wrap(ui_widget_t widget, bool word_wrap)
{
	ui_set_word_wrap_info_t *info;
	ui_text_widget_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_TEXT_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	body = (ui_text_widget_body_t *)widget;

	if (body->word_wrap == word_wrap) {
		return UI_OK;
	}

	info = (ui_set_word_wrap_info_t *)UI_ALLOC(sizeof(ui_set_word_wrap_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_align_info_t));

	info->body = body;
	info->word_wrap = word_wrap;

	if (ui_request_callback(_ui_text_widget_set_word_wrap_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_text_widget_set_word_wrap_func(void *userdata)
{
	ui_set_word_wrap_info_t *info;
	ui_text_widget_body_t *body;

	if (!userdata || !((ui_set_word_wrap_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_word_wrap_info_t *)userdata;
	body = info->body;
	body->word_wrap = info->word_wrap;

	// According to the text wrap option, a line number of the text widget can be differ from the current one.
	// Therefore, this value should be recalculated.
	_ui_text_widget_calculate_line_num(body);
	body->base.update_flag = true;

	UI_FREE(info);
}

ui_error_t ui_text_widget_set_font_size(ui_widget_t widget, size_t font_size)
{
	ui_set_font_size_info_t *info;
	ui_text_widget_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_TEXT_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	body = (ui_text_widget_body_t *)widget;

	if (body->font_size == font_size) {
		return UI_OK;
	}

	info = (ui_set_font_size_info_t *)UI_ALLOC(sizeof(ui_set_font_size_info_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_font_size_info_t));

	info->body = body;
	info->font_size = font_size;

	if (ui_request_callback(_ui_text_widget_set_font_size_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_text_widget_set_font_size_func(void *userdata)
{
	ui_set_font_size_info_t *info;
	ui_text_widget_body_t *body;

	if (!userdata || !((ui_set_font_size_info_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_font_size_info_t *)userdata;
	body = info->body;
	body->font_size = info->font_size;

	// According to the text wrap option, a line number of the text widget can be differ from the current one.
	// Therefore, this value should be recalculated.
	_ui_text_widget_calculate_line_num(body);
	body->base.update_flag = true;

	UI_FREE(info);
}

static void _ui_text_widget_calculate_line_num(ui_text_widget_body_t *body)
{
	size_t utf_idx = 0;
	size_t text_width = 0;
	size_t line_num = 1;
	float scale;
	int ax;
	int kern;

	if (!body) {
		UI_LOGE("error: invalid parameter!\n");
		return;
	}

	scale = stbtt_ScaleForPixelHeight(&(body->font->ttf_info), body->font_size);

	if (body->word_wrap) {
		while (utf_idx < body->text_length) {
			if (body->utf_code[utf_idx] == '\n') {
				line_num++;
				text_width = 0;
				body->width_array[utf_idx] = 0;
			} else {
#if defined(CONFIG_UI_ENABLE_EMOJI)
				if (is_emoji(body->utf_code[utf_idx])) {
					body->width_array[utf_idx] = body->font_size;
				} else {
#endif
					stbtt_GetCodepointHMetrics(&(body->font->ttf_info),  body->utf_code[utf_idx], &ax, 0);
					body->width_array[utf_idx] = ax * scale;

					if (utf_idx < body->text_length - 1) {
						kern = stbtt_GetCodepointKernAdvance(&(body->font->ttf_info), body->utf_code[utf_idx + 1],
							stbtt_FindGlyphIndex(&(body->font->ttf_info), body->utf_code[utf_idx + 1]));
						body->width_array[utf_idx] += kern * scale;
					}
#if defined(CONFIG_UI_ENABLE_EMOJI)
				}
#endif

				text_width += body->width_array[utf_idx];
				if (text_width > body->base.local_rect.width) {
					line_num++;
					text_width = 0;
					continue;
				}
			}
			utf_idx++;
		}
		body->line_num = line_num;
	} else {
		while (utf_idx < body->text_length) {
			if (body->utf_code[utf_idx] == '\n') {
				line_num++;
			}

#if defined(CONFIG_UI_ENABLE_EMOJI)
			if (is_emoji(body->utf_code[utf_idx])) {
				body->width_array[utf_idx] = body->font_size;
			} else {
#endif
				stbtt_GetCodepointHMetrics(&(body->font->ttf_info),  body->utf_code[utf_idx], &ax, 0);
				body->width_array[utf_idx] = ax * scale;

				if (utf_idx < body->text_length - 1) {
					kern = stbtt_GetCodepointKernAdvance(&(body->font->ttf_info), body->utf_code[utf_idx + 1],
						stbtt_FindGlyphIndex(&(body->font->ttf_info), body->utf_code[utf_idx + 1]));
					body->width_array[utf_idx] += kern * scale;
				}
#if defined(CONFIG_UI_ENABLE_EMOJI)
			}
#endif
			utf_idx++;
		}
		body->line_num = line_num;
	}
}

