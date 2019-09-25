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

#ifndef __UI_ASSET_INTERNAL_H__
#define __UI_ASSET_INTERNAL_H__

#include <stdint.h>
#include <araui/ui_asset.h>
#include <stb/stb_truetype.h>

typedef enum {
	UI_IMAGE_ASSET,
	UI_FONT_ASSET
} ui_asset_type_t;

typedef struct {
	ui_asset_type_t type;
} ui_asset_body_t;

typedef struct {
	ui_asset_body_t base;

	int32_t width;
	int32_t height;

	ui_pixel_format_t pixel_format;
	uint32_t bytes_per_line;
	uint16_t bits_per_pixel;
	uint8_t *buf;
	bool from_buf;
} ui_image_asset_body_t;

typedef struct {
	uint32_t id;
	int32_t width;
	int32_t height;
	ui_pixel_format_t pf;
	uint32_t header_size;
	uint32_t data_size;
	int32_t reserved[8];
} ui_bitmap_data_t;

typedef struct {
	ui_asset_body_t base;
	bool from_buf;
	stbtt_fontinfo ttf_info;
	uint8_t *ttf_buf;
} ui_font_asset_body_t;

#ifdef __cplusplus
extern "C" {
#endif

bool ui_asset_check_type(ui_asset_t asset, ui_asset_type_t type);
bool ui_image_asset_has_alpha(ui_pixel_format_t format);

#ifdef __cplusplus
}
#endif

#endif
