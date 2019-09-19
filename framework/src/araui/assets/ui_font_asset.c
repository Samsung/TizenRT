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
#include "ui_commons_internal.h"
#include "ui_request_callback.h"
#include "ui_debug.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include <stb/stb_truetype.h>

#define DEFAULT_GLYPH_MAP_CAPACITY 256

static void _ui_font_asset_destroy_func(void *userdata);

ui_asset_t ui_font_asset_create_from_file(const char *filename)
{
	ui_font_asset_body_t *body;
	FILE *file;
	size_t file_size;
	size_t read;
	char *extension;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!filename) {
		UI_LOGE("error: invaild parameter!\n");
		return UI_NULL;
	}

	extension = strrchr(filename, '.');
	if (strncmp(extension, ".ttf", 4) != 0) {
		UI_LOGE("error: unsupported font file!\n");
		return UI_NULL;
	}

	// file open
	file = fopen(filename, "r");
	if (!file) {
		UI_LOGE("error: failed to open file!\n");
		return UI_NULL;
	}

	// make font asset body
	body = (ui_font_asset_body_t *)UI_ALLOC(sizeof(ui_font_asset_body_t));
	if (!body) {
		(void)fclose(file);
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_font_asset_body_t));
	((ui_asset_body_t *)body)->type = UI_FONT_ASSET;

	/*******************************
	 * TTF file
	 */
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (!file_size) {
		(void)fclose(file);
		UI_LOGE("error: file size is zero!\n");
		UI_FREE(body);
		return UI_NULL;
	}

	body->ttf_buf = UI_ALLOC(file_size);
	if (!body->ttf_buf) {
		(void)fclose(file);
		UI_LOGE("error: out of memory!\n");
		UI_FREE(body);
		return UI_NULL;
	}

	read = fread(body->ttf_buf, 1, file_size, file);
	(void)fclose(file);

	if (read != file_size) {
		UI_LOGE("error: fread failed: %d\n", get_errno());
		UI_FREE(body->ttf_buf);
		UI_FREE(body);
		return UI_OPERATION_FAIL;
	}

	if (!stbtt_InitFont(&body->ttf_info, body->ttf_buf, 0)) {
		UI_LOGE("error: stbtt_InitFont\n");
		UI_FREE(body->ttf_buf);
		UI_FREE(body);
		return UI_OPERATION_FAIL;
	}

	return (ui_asset_t)body;
}

ui_asset_t ui_font_asset_create_from_buffer(const uint8_t *buf)
{
	ui_font_asset_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!buf) {
		UI_LOGE("error: invalid parameter!\n");
		return UI_NULL;
	}

	body = (ui_font_asset_body_t *)UI_ALLOC(sizeof(ui_font_asset_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_font_asset_body_t));
	((ui_asset_body_t *)body)->type = UI_FONT_ASSET;
	body->from_buf = true;

	if (!stbtt_InitFont(&body->ttf_info, buf, 0)) {
		UI_LOGE("error: stbtt_InitFont\n");
		UI_FREE(body);
		return UI_OPERATION_FAIL;
	}

	return (ui_asset_t)body;
}

ui_error_t ui_font_asset_destroy(ui_asset_t font)
{
	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!font) {
		return UI_INVALID_PARAM;
	}

	if (ui_request_callback(_ui_font_asset_destroy_func, (void *)font) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_font_asset_destroy_func(void *userdata)
{
	ui_font_asset_body_t *body;

	body = (ui_font_asset_body_t *)userdata;

	UI_FREE(body->ttf_buf);
	UI_FREE(body);
}
