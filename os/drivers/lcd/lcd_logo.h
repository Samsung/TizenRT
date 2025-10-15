/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#ifndef __DRIVER_LCD_LOGO_H
#define __DRIVER_LCD_LOGO_H

#include <tinyara/config.h>

#pragma pack(push, 1)
typedef struct {
	uint16_t signature;
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t data_offset;
} bmp_header_t;

typedef struct {
	uint32_t header_size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t image_size;
	int32_t x_pixels_per_meter;
	int32_t y_pixels_per_meter;
	uint32_t colors_used;
	uint32_t colors_important;
} bmp_info_header_t;
#pragma pack(pop)
#endif      /* __DRIVER_LCD_LOGO_H */

