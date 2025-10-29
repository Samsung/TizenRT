/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#ifndef __DRIVER_LCD_IMAGE_LIB_H
#define __DRIVER_LCD_IMAGE_LIB_H

#include <tinyara/config.h>
#include <stdio.h>

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

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: image_load_bmp_file
 *
 * Description:
 *   Load BMP file and convert to RGB565 format in the provided buffer.
 *   The image will be centered in the buffer based on buffer dimensions.
 *
 * Input Parameters:
 *   filename      - Path to BMP file
 *   buffer        - Buffer to store converted image data
 *   buffer_width  - Width of the image buffer
 *   buffer_height - Height of the image buffer
 *
 * Returned Value:
 *   OK on success, ERROR on failure
 *
 ****************************************************************************/
int image_load_bmp_file(const char *filename, uint8_t *buffer, int buffer_width, int buffer_height);

#endif /* __DRIVER_LCD_IMAGE_LIB_H */
