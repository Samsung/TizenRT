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
#include "ui_debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static void              _ui_image_asset_destroy_func(void *userdata);
static ui_pixel_format_t _ui_get_pixel_format_from_channels(int channels);
static size_t            _ui_get_bpp_from_pf(ui_pixel_format_t type);

ui_asset_t ui_image_asset_create_from_buffer(const uint8_t *buf)
{
	ui_image_asset_body_t *body;
	ui_bitmap_data_t *bitmap;

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

	bitmap = (ui_bitmap_data_t *)buf;

	body->width = bitmap->width;
	body->height = bitmap->height;
	body->buf = (uint8_t *)(buf + bitmap->header_size);

	body->pixel_format = bitmap->pf;
	body->bits_per_pixel = _ui_get_bpp_from_pf(body->pixel_format);
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

	return UI_OK;
}

static void _ui_image_asset_destroy_func(void *userdata)
{
	ui_image_asset_body_t *body;

	body = (ui_image_asset_body_t *)userdata;

	if (!body->from_buf) {
		stbi_image_free(body->buf);
	}
	UI_FREE(body);
}

ui_asset_t ui_image_asset_create_from_file(const char *filename)
{
	ui_image_asset_body_t *body;
	int32_t channels = 0;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!filename) {
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

	body->buf = stbi_load(filename, &body->width, &body->height, &channels, 0);
	if (body->buf == NULL) {
		UI_LOGE("error: failed to read png file!\n");
		return UI_NULL;
	}

	body->pixel_format = _ui_get_pixel_format_from_channels(channels);
	if (body->pixel_format == UI_PIXEL_FORMAT_UNKNOWN) {
		UI_LOGE("error: Not supported pixel format for channel %d\n", channels);
		stbi_image_free(body->buf);
		UI_FREE(body);
		return UI_NULL;
	}

	body->bits_per_pixel = (channels << 3);
	body->bytes_per_line = (body->width * body->bits_per_pixel) >> 3;

	return (ui_asset_t)body;
}

static ui_pixel_format_t _ui_get_pixel_format_from_channels(int channels)
{
	// todo: Support 4bit ~ 16bit pixel format
	switch (channels) {
	case 3:
		return UI_PIXEL_FORMAT_RGB888;
	case 4:
		return UI_PIXEL_FORMAT_RGBA8888;
	default:
		return UI_PIXEL_FORMAT_UNKNOWN;
	}
}

static size_t _ui_get_bpp_from_pf(ui_pixel_format_t type)
{
	switch (type) {
	case UI_PIXEL_FORMAT_GRAY4:
		return 4;
	case UI_PIXEL_FORMAT_GRAYA44:
		return 8;
	case UI_PIXEL_FORMAT_RGB565:
		return 16;
	case UI_PIXEL_FORMAT_ARGB8565:
		return 24;
	case UI_PIXEL_FORMAT_RGBA5658:
		return 24;
	case UI_PIXEL_FORMAT_ARGB4444:
		return 16;
	case UI_PIXEL_FORMAT_RGBA4444:
		return 16;
	case UI_PIXEL_FORMAT_RGB888:
		return 24;
	case UI_PIXEL_FORMAT_ARGB8888:
		return 32;
	case UI_PIXEL_FORMAT_RGBA8888:
		return 32;
	default:
		return 0;
	}
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
