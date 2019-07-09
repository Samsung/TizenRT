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
#include <sys/types.h>
#include <araui/ui_commons.h>
#include <araui/ui_asset.h>
#include "ui_core_internal.h"
#include "ui_asset_internal.h"
#include "ui_request_callback.h"
#include "ui_log.h"
#include "libs/upng.h"

typedef struct {
	ui_image_asset_body_t *body;
	int32_t x;
	int32_t y;
} ui_image_asset_info_t;

typedef struct {
	uint8_t *buf;
	ui_pixel_format_t dithered_format;
} ui_dither_t;

static void _ui_image_asset_destroy_func(void *userdata);
static void _ui_image_asset_set_pivot_point(void *userdata);
static ui_asset_t _ui_image_asset_create_from_png_file(const char *filename);
static ui_pixel_format_t _ui_image_asset_get_pixel_format_from_upng(upng_t *upng);
static size_t _ui_image_asset_get_bpp(ui_pixel_format_t type);
static ui_dither_t _ui_image_asset_dithering(const uint8_t *in_buf, size_t in_buf_size, ui_pixel_format_t format);

ui_asset_t ui_image_asset_create_from_buffer(const uint8_t *buf)
{
	ui_image_asset_body_t *body;
	ui_image_data_t *data;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!buf) {
		UI_LOGE("error: invalid parameter!\n");
		return UI_NULL;
	}

	body = (ui_image_asset_body_t *)UI_ALLOC(sizeof(ui_image_asset_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_image_asset_body_t));
	((ui_asset_body_t *)body)->type = UI_IMAGE_ASSET;

	data = (ui_image_data_t *)buf;

	body->width = data->width;
	body->height = data->height;
	body->buf = (uint8_t *)(buf + data->header_size);

	body->pixel_format = data->pf;
	body->bits_per_pixel = _ui_image_asset_get_bpp(body->pixel_format);
	body->bytes_per_line = body->width * body->bits_per_pixel / 8;
	body->from_buf = true;

	return (ui_asset_t)body;
}

ui_error_t ui_image_asset_destroy(ui_asset_t image)
{
	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!image) {
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_image_asset_destroy_func, (void *)image) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_image_asset_destroy_func(void *userdata)
{
	ui_image_asset_body_t *body;

	body = (ui_image_asset_body_t *)userdata;

	if (!body->from_buf) {
		UI_FREE(body->buf);
	}
	UI_FREE(body);
}

ui_error_t ui_image_asset_set_pivot_point(ui_asset_t image, int32_t x, int32_t y)
{
	ui_image_asset_info_t *cb_data;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!image) {
		return UI_INVALID_PARAM;
	}

	cb_data = (ui_image_asset_info_t *)UI_ALLOC(sizeof(ui_image_asset_info_t));
	if (!cb_data) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	cb_data->body = (ui_image_asset_body_t *)image;
	cb_data->x = x;
	cb_data->y = y;

	if (ui_request_callback(_ui_image_asset_set_pivot_point, (void *)cb_data) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_image_asset_set_pivot_point(void *userdata)
{
	ui_image_asset_info_t *info;
	ui_image_asset_body_t *body;

	info = (ui_image_asset_info_t *)userdata;
	body = info->body;

	body->pivot_x = info->x;
	body->pivot_y = info->y;

	UI_FREE(info);
}

ui_asset_t ui_image_asset_create_from_file(const char *filename)
{
	ui_asset_t asset = UI_NULL;
	char *extension;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return asset;
	}

	if (!filename) {
		UI_LOGE("error: invalid parameter!\n");
		return asset;
	}

	extension = strrchr(filename, '.');

	if (strncmp(extension, ".png", 5) == 0) {
		asset = _ui_image_asset_create_from_png_file(filename);
	}

	return asset;
}

static ui_asset_t _ui_image_asset_create_from_png_file(const char *filename)
{
	ui_image_asset_body_t *body;
	upng_t *upng;
	ui_dither_t dithered;

	upng = upng_new_from_file(filename);
	if (upng == NULL) {
		UI_LOGE("error: failed to read png file!\n");
		return UI_NULL;
	}

	(void)upng_decode(upng);
	if (upng_get_error(upng) != UPNG_EOK) {
		UI_LOGE("error: failed to decode!\n");
		upng_free(upng);
		return UI_NULL;
	}

	body = (ui_image_asset_body_t *)UI_ALLOC(sizeof(ui_image_asset_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_image_asset_body_t));
	((ui_asset_body_t *)body)->type = UI_IMAGE_ASSET;

	dithered = _ui_image_asset_dithering(upng_get_buffer(upng), upng_get_size(upng), _ui_image_asset_get_pixel_format_from_upng(upng));
	if (dithered.buf == NULL) {
		UI_LOGE("error: failed to dithering!\n");
		upng_free(upng);
		UI_FREE(body);
		return UI_NULL;
	}

	body->width = upng_get_width(upng);
	body->height = upng_get_height(upng);
	body->buf = dithered.buf;
	body->pixel_format = dithered.dithered_format;
	body->bits_per_pixel = _ui_image_asset_get_bpp(body->pixel_format);
	body->bytes_per_line = body->width * body->bits_per_pixel / 8;
	body->pivot_x = 0;
	body->pivot_y = 0;

	upng_free(upng);

	return (ui_asset_t)body;
}

static ui_pixel_format_t _ui_image_asset_get_pixel_format_from_upng(upng_t *upng)
{
	upng_format format;

	format = upng_get_format(upng);
	switch (format) {
		case UPNG_LUMINANCE1:
// Currently, Not Supported format.
//			return UI_PIXEL_FORMAT_GRAY1;
		case UPNG_LUMINANCE2:
// Currently, Not Supported format.
//			return UI_PIXEL_FORMAT_GRAY2;
		case UPNG_LUMINANCE4:
			return UI_PIXEL_FORMAT_GRAY4;
		case UPNG_LUMINANCE8:
// Currently, Not Supported format.
//			return UI_PIXEL_FORMAT_GRAY8;
		case UPNG_LUMINANCE_ALPHA8:
// Currently, Not Supported format.
//			return UI_PIXEL_FORMAT_GRAYA88;
		case UPNG_RGB8:
			return UI_PIXEL_FORMAT_RGB888;
		case UPNG_RGB16:
// Currently, Not Supported format.
//			return UI_PIXEL_FORMAT_RGB161616;
		case UPNG_RGBA8:
			return UI_PIXEL_FORMAT_RGBA8888;
		case UPNG_RGBA16:
// Currently, Not Supported format.
//			return UI_PIXEL_FORMAT_RGBA16161616;
		case UPNG_LUMINANCE_ALPHA1:
		case UPNG_LUMINANCE_ALPHA2:
		case UPNG_LUMINANCE_ALPHA4:
		default:
			return UI_INVALID_PIXEL_FORMAT;
	}
}

static size_t _ui_image_asset_get_bpp(ui_pixel_format_t type)
{
	switch (type) {
		case UI_PIXEL_FORMAT_GRAY1:
// Currently, Not Supported format.
//			return 1;
		case UI_PIXEL_FORMAT_GRAY2:
// Currently, Not Supported format.
//			return 2;
		case UI_PIXEL_FORMAT_GRAY4:
			return 4;
		case UI_PIXEL_FORMAT_GRAYA44:
			return 8;
		case UI_PIXEL_FORMAT_GRAY8:
// Currently, Not Supported format.
//			return 8;
		case UI_PIXEL_FORMAT_GRAYA88:
// Currently, Not Supported format.
//			return 16;
		case UI_PIXEL_FORMAT_RGB565:
			return 16;
		case UI_PIXEL_FORMAT_RGBA5658:
			return 24;
		case UI_PIXEL_FORMAT_RGB888:
			return 24;
		case UI_PIXEL_FORMAT_RGBA8888:
			return 32;
		case UI_PIXEL_FORMAT_RGB161616:
// Currently, Not Supported format.
//			return 48;
		case UI_PIXEL_FORMAT_RGBA16161616:
// Currently, Not Supported format.
//			return 64;
		default:
			return 0;
	}
}

static ui_error_t _ui_image_asset_gray4_dithering(const uint8_t *in_buf, size_t in_buf_size, uint8_t *out_buf)
{
	int buf_idx;

#if defined(CONFIG_UI_DISPLAY_RGB565)
	uint8_t gray;
	uint8_t r;
	uint8_t g;
	uint8_t b;

	for (buf_idx = 0; buf_idx < in_buf_size; buf_idx++) {
		gray = (in_buf[buf_idx] & 0xF0) >> 4;

		r = gray << 1;
		g = gray << 2;
		b = gray << 1;

		out_buf[4 * buf_idx] = (r << 3) | (g >> 3);
		out_buf[4 * buf_idx + 1] = (g << 5) | b;

		gray = in_buf[buf_idx] & 0x0F;
		r = gray << 1;
		g = gray << 2;
		b = gray << 1;

		out_buf[4 * buf_idx + 2] = (r << 3) | (g >> 3);
		out_buf[4 * buf_idx + 3] = (g << 5) | b;
	}

	return UI_OK;
#elif defined(CONFIG_UI_DISPLAY_4BIT_GRAY)
	for (buf_idx = 0; buf_idx < in_buf_size; buf_idx++) {
		out_buf[buf_idx] = in_buf[buf_idx];
	}

	return UI_OK;
#else
	UI_LOGE("error: unsupported display!\n");
	return UI_OPERATION_FAIL;
#endif
}

static ui_error_t _ui_image_asset_rgb888_dithering(const uint8_t *in_buf, size_t in_buf_size, uint8_t *out_buf)
{
	int buf_idx;
	uint8_t r;
	uint8_t g;
	uint8_t b;

#if defined(CONFIG_UI_DISPLAY_RGB565)
	for (buf_idx = 0; 3 * buf_idx < in_buf_size; buf_idx++) {
		r = in_buf[3 * buf_idx] >> 3;
		g = in_buf[3 * buf_idx + 1] >> 2;
		b = in_buf[3 * buf_idx + 2] >> 3;

		out_buf[2 * buf_idx] = (r << 3) | (g >> 3);
		out_buf[2 * buf_idx + 1] = (g << 5) | b;
	}

	return UI_OK;
#elif defined(CONFIG_UI_DISPLAY_4BIT_GRAY)
	for (buf_idx = 0; 6 * buf_idx < in_buf_size; buf_idx++) {
		r = in_buf[6 * buf_idx] >> 4;
		g = in_buf[6 * buf_idx + 1] >> 4;
		b = in_buf[6 * buf_idx + 2] >> 4;
		out_buf[buf_idx] = ((r * 5 + g * 9 + b * 2) / 16) << 4;

		r = in_buf[6 * buf_idx + 3] >> 4;
		g = in_buf[6 * buf_idx + 4] >> 4;
		b = in_buf[6 * buf_idx + 5] >> 4;
		out_buf[buf_idx] = out_buf[buf_idx] | ((r * 5 + g * 9 + b * 2) / 16);
	}

	return UI_OK;
#else
	UI_LOGE("error: unsupported display!\n");
	return UI_OPERATION_FAIL;
#endif
}

static ui_error_t _ui_image_asset_rgba8888_dithering(const uint8_t *in_buf, size_t in_buf_size, uint8_t *out_buf)
{
	int buf_idx;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

#if defined(CONFIG_UI_DISPLAY_RGB565)
	for (buf_idx = 0; 4 * buf_idx < in_buf_size; buf_idx++) {
		r = in_buf[4 * buf_idx] >> 3;
		g = in_buf[4 * buf_idx + 1] >> 2;
		b = in_buf[4 * buf_idx + 2] >> 3;
		a = in_buf[4 * buf_idx + 3];

		out_buf[3 * buf_idx] = (r << 3) | (g >> 3);
		out_buf[3 * buf_idx + 1] = (g << 5) | b;
		out_buf[3 * buf_idx + 2] = a;
	}

	return UI_OK;
#elif defined(CONFIG_UI_DISPLAY_4BIT_GRAY)
	for (buf_idx = 0; 4 * buf_idx < in_buf_size; buf_idx++) {
		r = in_buf[4 * buf_idx] >> 4;
		g = in_buf[4 * buf_idx + 1] >> 4;
		b = in_buf[4 * buf_idx + 2] >> 4;
		a = in_buf[4 * buf_idx + 3] >> 4;
		out_buf[buf_idx] = (((r * 5 + g * 9 + b * 2) / 16) << 4) | a;
	}

	return UI_OK;
#else
	UI_LOGE("error: unsupported display!\n");

	return UI_OPERATION_FAIL;
#endif
}

bool ui_image_asset_has_alpha(ui_pixel_format_t format)
{
	if (format == UI_PIXEL_FORMAT_GRAYA44 || format == UI_PIXEL_FORMAT_GRAYA88) {
		return true;
	}

	if (format == UI_PIXEL_FORMAT_RGBA5658 || format == UI_PIXEL_FORMAT_RGBA8888 || format == UI_PIXEL_FORMAT_RGBA16161616) {
		return true;
	}

	return false;
}

static ui_dither_t _ui_image_asset_dithering(const uint8_t *in_buf, size_t in_buf_size, ui_pixel_format_t format)
{
	ui_dither_t output;
	uint8_t *out_buf;
	size_t out_buf_size;
	size_t input_bpp;
	size_t out_bpp;

	output.buf = NULL;
#if defined(CONFIG_UI_DISPLAY_RGB565)
	if (ui_image_asset_has_alpha(format)) {
		out_bpp = _ui_image_asset_get_bpp(UI_PIXEL_FORMAT_RGBA5658);
		output.dithered_format = UI_PIXEL_FORMAT_RGBA5658;
	} else {
		out_bpp = _ui_image_asset_get_bpp(UI_PIXEL_FORMAT_RGB565);
		output.dithered_format = UI_PIXEL_FORMAT_RGB565;
	}
#elif defined(CONFIG_UI_DISPLAY_4BIT_GRAY)
	if (ui_image_asset_has_alpha(format)) {
		out_bpp = _ui_image_asset_get_bpp(UI_PIXEL_FORMAT_GRAYA44);
		output.dithered_format = UI_PIXEL_FORMAT_GRAYA44;
	} else {
		out_bpp = _ui_image_asset_get_bpp(UI_PIXEL_FORMAT_GRAY4);
		output.dithered_format = UI_PIXEL_FORMAT_GRAY4;
	}
#else
	UI_LOGE("error: unsupported display!\n");
	return output;
#endif

	input_bpp = _ui_image_asset_get_bpp(format);
	if (!input_bpp) {
		UI_LOGE("error: invalid image type!\n");
		return output;
	}

	// todo: need to fix
	out_buf_size = in_buf_size * out_bpp / input_bpp;
	out_buf = (uint8_t *)UI_ALLOC(out_buf_size);
	if (!out_buf) {
		UI_LOGE("error: out of memory!\n");
		return output;
	}

	switch (format) {
		case UI_PIXEL_FORMAT_GRAY4:
			if (_ui_image_asset_gray4_dithering(in_buf, in_buf_size, out_buf) != UI_OK) {
				UI_FREE(out_buf);
				return output;
			}
			break;
		case UI_PIXEL_FORMAT_RGB888:
			if (_ui_image_asset_rgb888_dithering(in_buf, in_buf_size, out_buf) != UI_OK) {
				UI_FREE(out_buf);
				return output;
			}
			break;
		case UI_PIXEL_FORMAT_RGBA8888:
			if (_ui_image_asset_rgba8888_dithering(in_buf, in_buf_size, out_buf) != UI_OK) {
				UI_FREE(out_buf);
				return output;
			}
			break;
		default:
			UI_FREE(out_buf);
			return output;
	}

	output.buf = out_buf;

	return output;
}

size_t ui_image_asset_get_width(ui_asset_t image)
{
	size_t ret = 0;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return ret;
	}

	if (!image) {
		UI_LOGE("error: invalid parameter!\n");
		return ret;
	}

	ret = ((ui_image_asset_body_t *)image)->width;

	return ret;
}

size_t ui_image_asset_get_height(ui_asset_t image)
{
	size_t ret = 0;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return ret;
	}

	if (!image) {
		UI_LOGE("error: invalid parameter!\n");
		return ret;
	}

	ret = ((ui_image_asset_body_t *)image)->height;

	return ret;
}
