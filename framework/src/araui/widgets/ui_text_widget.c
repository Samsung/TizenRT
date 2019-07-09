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
#include <araui/ui_commons.h>
#include <araui/ui_widget.h>
#include "ui_request_callback.h"
#include "ui_log.h"
#include "libs/ui_map.h"
#include "ui_commons_internal.h"
#include "ui_core_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "ui_view_internal.h"
#include "hal/ui_hal_interface.h"

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

#if defined(CONFIG_UI_DISPLAY_RGB565)
#define UI_DEFAULT_TEXT_COLOR 0xffff;
#elif defined(CONFIG_UI_DISPLAY_4BIT_GRAY)
#define UI_DEFAULT_TEXT_COLOR 0xff;
#endif

#define CONFIG_UI_TEXT_FORMAT_MAX_LENGTH  512

static ui_error_t _ui_text_widget_text2utf(ui_text_widget_body_t *body, const char *text);
static void _ui_text_widget_draw_func(ui_widget_t widget, uint32_t dt);
static void _ui_text_widget_removed_func(ui_widget_t widget);
static void _ui_text_widget_set_text_func(void *userdata);
static void _ui_text_widget_set_align_func(void *userdata);
static void _ui_text_widget_set_color(void *userdata);
static void _ui_text_widget_draw_glyph(int32_t x, int32_t y, ui_asset_glyph_t *glyph, int32_t height, ui_color_t color);
static void _ui_text_widget_set_word_wrap_func(void *userdata);
static void _ui_text_widget_calculate_line_num(ui_text_widget_body_t *body);

ui_widget_t ui_text_widget_create(int32_t width, int32_t height, ui_asset_t font, const char *text)
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

	if (text) {
		if (_ui_text_widget_text2utf(body, text) != UI_OK) {
			(void)ui_list_deinit(&((ui_widget_body_t *)body)->children);
			UI_FREE(body);
			UI_LOGE("error: out of memory!\n");
			return UI_NULL;
		}
	}

	body->align = UI_ALIGN_DEFAULT;
	body->txt_foreground_color = UI_DEFAULT_TEXT_COLOR;
	body->base.draw_cb = _ui_text_widget_draw_func;
	body->base.remove_cb = _ui_text_widget_removed_func;

	UI_LOGD("UI_OK.\n");

	return (ui_widget_t)body;
}

static ui_error_t _ui_text_widget_text2utf(ui_text_widget_body_t *body, const char *text)
{
	size_t length = 0;
	size_t utf_idx = 0;
	size_t txt_idx = 0;
	uint8_t lobyte;
	uint8_t hibyte;
	uint32_t *realloc_utf;

	if (!body || !text) {
		return UI_INVALID_PARAM;
	}

	length = strlen(text);
	if (!length) {
		body->utf_code = NULL;
		body->text_length = length;
		body->line_num = 0;

		return UI_OK;
	}

	body->utf_code = (uint32_t *)UI_ALLOC(length * sizeof(uint32_t));
	if (!body->utf_code) {
		return UI_NOT_ENOUGH_MEMORY;
	}
	body->text_length = length;

	while(txt_idx < length) {
		if ((text[txt_idx] & 0x80) != 0x80) {
			body->utf_code[utf_idx] = (uint32_t)text[txt_idx];
			txt_idx++;
		} else if (((text[txt_idx] & 0xf0) == 0xe0) && ((text[txt_idx + 1] & 0xc0) == 0x80) && ((text[txt_idx + 2] & 0xc0) == 0x80)) {
			hibyte = 0;
			lobyte = 0;
			hibyte |= (text[txt_idx] & 0x0f) << 4;
			hibyte |= (text[txt_idx+ 1] & 0x3c) >> 2;
			lobyte |= (text[txt_idx + 1] & 0x03) << 6;
			lobyte |= (text[txt_idx + 2] & 0x3f);
			body->utf_code[utf_idx] = (uint16_t)(hibyte << 8) + lobyte;
			txt_idx += 3;
		}
		utf_idx++;
	}

	_ui_text_widget_calculate_line_num(body);

	if (body->text_length != utf_idx) {
		realloc_utf = (uint32_t *)UI_ALLOC(utf_idx * sizeof(uint32_t));
		if (!realloc_utf) {
			// Although the current allocation is failed,
			// the previous allocation can be used because it was successful allocated.
			body->text_length = utf_idx;
		} else {
			memcpy(realloc_utf, body->utf_code, utf_idx * sizeof(uint32_t));
			UI_FREE(body->utf_code);

			body->utf_code = realloc_utf;
			body->text_length = utf_idx;
		}
	}

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

	if (_ui_text_widget_text2utf(body, text) != UI_OK) {
		UI_LOGE("error: out of memory!\n");
		UI_FREE(info->text);
		UI_FREE(info);
		return;
	}

	if (ui_view_add_update_list(body->base.global_rect) != UI_OK) {
		UI_LOGE("error: failed to add to the update list!\n");
		UI_FREE(info->text);
		UI_FREE(info);
		return;
	}

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

	UI_LOGD("UI_OK.\n");

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

	if (ui_view_add_update_list(info->body->base.global_rect) != UI_OK) {
		UI_LOGE("error: failed to add to the update list!\n");
		UI_FREE(info);
		return;
	}

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
	info->body->txt_foreground_color = info->color;

	UI_FREE(info);
}

static void _ui_text_widget_draw_func(ui_widget_t widget, uint32_t dt)
{
	size_t i;
	ui_text_widget_body_t *body;
	int32_t x = 0;
	int32_t draw_x;
	int32_t y = 0;
	ui_asset_glyph_t *glyph;
	size_t text_length;
	int32_t text_width;
	size_t text_draw_idx = 0;
	size_t text_idx = 0;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_text_widget_body_t *)widget;
	text_length = body->text_length;

	if (!text_length) {
		UI_LOGD("Empty text widget!\n");
		return;
	}

	if (body->align & UI_ALIGN_TOP) {
		y = body->base.global_rect.y;
	} else if (body->align & UI_ALIGN_MIDDLE) {
		y = body->base.global_rect.y + ((body->base.global_rect.height - ((int32_t)body->line_num * body->font->height)) >> 1);
	} else if (body->align & UI_ALIGN_BOTTOM) {
		y = body->base.global_rect.y + (body->base.global_rect.height - ((int32_t)body->line_num * body->font->height));
	}

	for (i = 0; i < body->line_num; i++) {
		// Calculate the width of text
		text_width = 0;
		while (body->utf_code[text_idx] != '\n') {
			glyph = (ui_asset_glyph_t *)ui_map_get(&(body->font)->glyph_map, body->utf_code[text_idx]);
			if (glyph && glyph->data) {
				text_width += glyph->width;
			}

			if (body->word_wrap) {
				if (text_width > body->base.local_rect.width) {
					// As the width of text exceeds the width of text widget,
					// this utf code will be printed at the next line.
					text_width -= glyph->width;
					text_idx--;
					break;
				}
			}

			if (text_idx >= (text_length - 1)) {
				break;
			} else {
				text_idx++;
			}
		}
		text_idx++;

		// Calculate proper x coordinate according to align
		if (body->align & UI_ALIGN_LEFT) {
			x = body->base.global_rect.x;
		} else if (body->align & UI_ALIGN_CENTER) {
			x = body->base.global_rect.x + ((body->base.global_rect.width - text_width) >> 1);
		} else if (body->align & UI_ALIGN_RIGHT) {
			x = body->base.global_rect.x + (body->base.global_rect.width - text_width);
		}
		draw_x = x;

		while (text_draw_idx < text_idx) {
			glyph = (ui_asset_glyph_t *)ui_map_get(&(body->font)->glyph_map, body->utf_code[text_draw_idx]);
			if (glyph && glyph->data) {
				_ui_text_widget_draw_glyph(draw_x, y, glyph, body->font->height, body->txt_foreground_color);
				draw_x += glyph->width - 1;
			}
			text_draw_idx++;
		}
		y += body->font->height;
	}
}

void _ui_text_widget_draw_glyph(int32_t x, int32_t y, ui_asset_glyph_t *glyph, int32_t height, ui_color_t color)
{
	int32_t index;
	int32_t ypos;
	int32_t xpos;
	int32_t skip_x;
	uint8_t data;
	uint8_t count;
	ui_rect_t view_port;
	ui_coord_t draw_coord;

	index = 0;
	ypos = 0;
	xpos = 0;
	skip_x = 0;

	view_port = ui_hal_get_viewport();

	while (index < glyph->size) {
		data = glyph->data[index];

		if (data == 0) {
			index++;
			count = glyph->data[index];
			skip_x = ((count + 1) % (glyph->width));
			ypos += ((count + 1) / (glyph->width));
			xpos += skip_x;
			if (xpos >= glyph->width) {
				xpos -= glyph->width;
				ypos++;
			}
		} else {
			draw_coord.x = x + xpos;
			draw_coord.y = y + ypos;

			if (ui_coord_inside_rect(draw_coord, view_port)) {
				ui_hal_draw_pixel(x + xpos, y + ypos, UI_COLOR_ALPHA(color, data));
			}
			xpos++;
			if (xpos >= glyph->width) {
				xpos = 0;
				ypos++;
			}
		}

		index++;
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

	UI_LOGD("UI_OK.\n");

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

	if (ui_view_add_update_list(info->body->base.global_rect) != UI_OK) {
		UI_LOGE("error: failed to add to the update list!\n");
		UI_FREE(info);
		return;
	}

	UI_FREE(info);
}

static void _ui_text_widget_calculate_line_num(ui_text_widget_body_t *body)
{
	ui_asset_glyph_t *glyph;
	size_t utf_idx = 0;
	size_t text_width = 0;
	size_t line_num = 1;

	if (!body) {
		UI_LOGE("error: invalid parameter!\n");
		return;
	}

	if (body->word_wrap) {
		while (utf_idx < body->text_length) {
			if (body->utf_code[utf_idx] == '\n') {
				line_num++;
				text_width = 0;
			} else {
				glyph = (ui_asset_glyph_t *)ui_map_get(&(body->font)->glyph_map, body->utf_code[utf_idx]);
				text_width += glyph->width;
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
			utf_idx++;
		}
		body->line_num = line_num;
	}
}

