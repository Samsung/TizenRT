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

#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <araui/ui_commons.h>
#include <araui/ui_asset.h>
#include "libs/ui_map.h"
#include "libs/ui_list.h"
#include "ui_core_internal.h"
#include "ui_asset_internal.h"
#include "ui_commons_internal.h"
#include "ui_request_callback.h"
#include "ui_log.h"

#define DEFAULT_GLYPH_MAP_CAPACITY 256

static void _ui_font_asset_destroy_func(void *userdata);

ui_asset_t ui_font_asset_create_from_file(const char *filename)
{
	ui_font_asset_body_t *body;
	FILE *file;
	uint32_t font_height;
	uint32_t font_width;
	uint32_t glyph_num;
	uint32_t utf8_code;
	uint32_t data_size;
	int glyph_idx;
	char *extension;
	ui_asset_glyph_t *glyph;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!filename) {
		UI_LOGE("error: invaild parameter!\n");
		return UI_NULL;
	}

	extension = strrchr(filename, '.');
	if (strncmp(extension, ".fnt", 5) != 0) {
		UI_LOGE("error: unsupported font file!\n");
		return UI_NULL;
	}

	// file open
	file = fopen(filename, "r");
	if (!file) {
		UI_LOGE("error: failed to open file!\n");
		return UI_NULL;
	}

	// read header of file
	ui_fread(&font_height, 4, 1, file);
	ui_fread(&glyph_num, 4, 1, file);

	// make font asset body
	body = (ui_font_asset_body_t *)UI_ALLOC(sizeof(ui_font_asset_body_t));
	if (!body) {
		(void)fclose(file);
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_font_asset_body_t));
	((ui_asset_body_t *)body)->type = UI_FONT_ASSET;
	body->height = font_height;

	// read glyph from the buffer
	if (ui_map_init(&body->glyph_map, DEFAULT_GLYPH_MAP_CAPACITY) != UI_OK) {
		UI_LOGE("error: Cannot init the glyph map!\n");
		(void)fclose(file);
		UI_FREE(body);
		return UI_NULL;
	}

	for (glyph_idx = 0; glyph_idx < glyph_num; glyph_idx++) {
		ui_fread(&utf8_code, 4, 1, file);
		ui_fread(&font_width, 4, 1, file);
		ui_fread(&data_size, 4, 1, file);

		glyph = (ui_asset_glyph_t *)UI_ALLOC(sizeof(ui_asset_glyph_t));
		if (!glyph) {
			_ui_font_asset_destroy_func((void *)body);
			(void)fclose(file);
			UI_LOGE("error: out of memory!\n");
			return UI_NULL;
		}

		glyph->data = (uint8_t *)UI_ALLOC(data_size);
		if (!glyph->data) {
			UI_FREE(glyph);
			_ui_font_asset_destroy_func((void *)body);
			(void)fclose(file);
			UI_LOGE("error: out of memory!\n");
			return UI_NULL;
		}

		ui_fread(glyph->data, 1, data_size, file);

		glyph->size = data_size;
		glyph->width = font_width;

		if (ui_map_push(&body->glyph_map, utf8_code, glyph) != UI_OK) {
			UI_LOGW("warning: 0x%08F is not added to the map!\n", utf8_code);
		}
	}

	(void)fclose(file);

	return (ui_asset_t)body;
}

ui_asset_t ui_font_asset_create_from_buffer(const uint8_t *buf)
{
	ui_font_asset_body_t *body;
	ui_font_data_t *font_data;
	ui_font_glyph_data_t *glyph_data;
	uint32_t glyph_num;
	int glyph_idx;
	ui_asset_glyph_t *glyph;

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

	font_data = (ui_font_data_t *)buf;
	glyph_num = font_data->glyph_count;
	body->height = font_data->font_height;
	body->from_buf = true;

	glyph_data = (ui_font_glyph_data_t *)(buf + font_data->header_size);

	// read glyph from the buffer
	if (ui_map_init(&body->glyph_map, DEFAULT_GLYPH_MAP_CAPACITY) != UI_OK) {
		UI_LOGE("error: Cannot init the glyph map!\n");
		UI_FREE(body);
		return UI_NULL;
	}

	for (glyph_idx = 0; glyph_idx < glyph_num; glyph_idx++) {
		glyph = (ui_asset_glyph_t *)UI_ALLOC(sizeof(ui_asset_glyph_t));
		if (!glyph) {
			_ui_font_asset_destroy_func((void *)body);
			UI_LOGE("error: out of memory!\n");
			return UI_NULL;
		}

		glyph->data = (uint8_t *)glyph_data + glyph_data->header_size;
		glyph->size = glyph_data->data_size;
		glyph->width = glyph_data->font_width;

		if (ui_map_push(&body->glyph_map, glyph_data->utf8_code, glyph) != UI_OK) {
			UI_LOGW("warning: 0x%08F is not added to the map!\n", utf8_code);
		}

		glyph_data = (ui_font_glyph_data_t *)((uint8_t *)glyph_data + glyph_data->header_size + glyph_data->data_size);
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

	UI_LOGD("UI_OK.\n");

	return UI_OK;
}

static void _ui_font_asset_destroy_func(void *userdata)
{
	size_t i;
	ui_map_t *map;
	ui_font_asset_body_t *body;
	ui_list_t *list;
	ui_list_node_t *node;
	key_value_pair_t *pair;
	ui_asset_glyph_t *glyph;

	body = (ui_font_asset_body_t *)userdata;

	// clear all data if this asset is not from buffer.
	if (!body->from_buf) {
		map = &body->glyph_map;
		for (i = 0; i < map->hash_capacity; i++) {
			list = &map->data[i];

			node = ui_list_loop_begin(list);
			while (node != ui_list_loop_end(list)) {
				pair = (key_value_pair_t *)node->data;
				glyph = (ui_asset_glyph_t *)pair->value;

				UI_FREE(glyph->data);
				UI_FREE(glyph);

				node = node->next;
			}
		}
	}

	if (ui_map_deinit(&body->glyph_map) != UI_OK) {
		UI_LOGE("error: Error on ui_map_deinit!\n");
	}

	UI_FREE(body);
}

size_t ui_font_asset_measure_text(ui_asset_t font, const char *text)
{
	size_t i;
	size_t ret = 0;
	size_t text_len;
	uint32_t utf8_code;
	uint8_t lobyte;
	uint8_t hibyte;
	ui_asset_glyph_t *glyph;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return ret;
	}

	if (!font || !text) {
		UI_LOGE("error: invalid parameter!\n");
		return ret;
	}

	text_len = strlen(text);
	for (i = 0; i < text_len; i++) {
		utf8_code = 0;
		if ((text[i] & 0x80) != 0x80) {
			utf8_code = (uint32_t)text[i];
		} else if (((text[i] & 0xf0) == 0xe0) && ((text[i + 1] & 0xc0) == 0x80) && ((text[i + 2] & 0xc0) == 0x80)) {
			hibyte = 0;
			lobyte = 0;
			hibyte |= (text[i] & 0x0f) << 4;
			hibyte |= (text[i + 1] & 0x3c) >> 2;
			lobyte |= (text[i + 1] & 0x03) << 6;
			lobyte |= (text[i + 2] & 0x3f);

			utf8_code = (uint16_t)(hibyte << 8) + lobyte;
		}

		glyph = (ui_asset_glyph_t *)ui_map_get(&((ui_font_asset_body_t *)font)->glyph_map, utf8_code);
		if (glyph) {
			ret += glyph->width;
		}
	}

	return ret;
}
