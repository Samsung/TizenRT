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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#ifdef CONFIG_LCD_SPLASH_IMAGE
#include <debug.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <tinyara/kmalloc.h>
#include "image_lib.h"

#define BMP_SIGNATURE 0x4D42

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int count_trailing_zeros(uint32_t mask)
{
	int count = 0;

	if (mask == 0) {
		return 0;
	}
	while ((mask & 1) == 0) {
		mask >>= 1;
		count++;
	}
	return count;
}

static int count_set_bits(uint32_t mask)
{
	int count = 0;
	while (mask) {
		count += mask & 1;
		mask >>= 1;
	}
	return count;
}

#ifdef CONFIG_LCD_SPLASH_IMAGE_BMP_16BIT
static uint16_t convert_pixel_to_rgb565(uint16_t bmp_pixel, uint32_t r_mask, uint32_t g_mask, uint32_t b_mask)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	// Extract red channel
	if (r_mask == 0) {
		r = 0;
	} else {
		uint8_t r_bits = count_set_bits(r_mask);
		uint8_t r_shift =  count_trailing_zeros(r_mask);
		r = ((bmp_pixel & r_mask) >> r_shift) << (5 - r_bits); // Scale to 5 bits
		if (r_bits > 5)
			r >>= (r_bits - 5); // Scale down if source has more bits
	}
	// Extract green channel
	if (g_mask == 0) {
		g = 0;
	} else {
		uint8_t g_bits = count_set_bits(g_mask);
		uint8_t g_shift = count_trailing_zeros(g_mask);
		g = ((bmp_pixel & g_mask) >> g_shift) << (6 - g_bits); // Scale to 6 bits
		if (g_bits > 6)
			g >>= (g_bits - 6); // Scale down if source has more bits
	}
	// Extract blue channel
	if (b_mask == 0) {
		b = 0;
	} else {
		uint8_t b_bits = count_set_bits(b_mask);
		uint8_t b_shift = count_trailing_zeros(b_mask);
		b = ((bmp_pixel & b_mask) >> b_shift) << (5 - b_bits); // Scale to 5 bits
		if (b_bits > 5)
			b >>= (b_bits - 5); // Scale down if source has more bits
	}
	return (r << 11) | (g << 5) | b;
}

static int image_parse_bmp_header(int fd, bmp_header_t *header, bmp_info_header_t *info_header)
{
	if (read(fd, header, sizeof(bmp_header_t)) != sizeof(bmp_header_t)) {
		lcddbg("Failed to read BMP header\n");
		return ERROR;
	}
	if (header->signature != BMP_SIGNATURE) {
		lcddbg("Invalid BMP file: signature mismatch\n");
		return ERROR;
	}
	if (read(fd, info_header, sizeof(bmp_info_header_t)) != sizeof(bmp_info_header_t)) {
		lcddbg("Failed to read BMP info header\n");
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int image_load_bmp_file(const char *filename, uint8_t *buffer, int buffer_width, int buffer_height)
{
	bmp_header_t header;
	bmp_info_header_t info_header;
	int bmp_fd;
	int img_width;
	int img_height;

	bmp_fd = open(filename, O_RDONLY);
	if (bmp_fd < 0) {
		lcddbg("Failed to open BMP file: %s\n", filename);
		return ERROR;
	}

	if (image_parse_bmp_header(bmp_fd, &header, &info_header) != 0) {
		goto errout;
	}

	if (info_header.bits_per_pixel != 16) {
		lcddbg("Unsupported BMP format: only 16-bit color is supported (found %d-bit)\n", info_header.bits_per_pixel);
		goto errout;
	}

	uint32_t r_mask = 0, g_mask = 0, b_mask = 0;
	if (info_header.compression == 3) { // BI_BITFIELDS
		if (read(bmp_fd, &r_mask, sizeof(uint32_t)) != sizeof(uint32_t) ||
			read(bmp_fd, &g_mask, sizeof(uint32_t)) != sizeof(uint32_t) ||
			read(bmp_fd, &b_mask, sizeof(uint32_t)) != sizeof(uint32_t)) {
			lcddbg("Failed to read BMP bitmasks\n");
			goto errout;
		}
	} else if (info_header.compression == 0) { // BI_RGB
		r_mask = 0x00007C00;				   // 0111110000000000 (5 bits for red)
		g_mask = 0x000003E0;				   // 0000001111100000 (5 bits for green)
		b_mask = 0x0000001F;				   // 0000000000011111 (5 bits for blue)
	} else {
		lcddbg("Unsupported BMP compression: %d. Only BI_RGB (0) and BI_BITFIELDS (3) are supported for 16-bit.\n", info_header.compression);
		goto errout;
	}

	img_width = info_header.width;
	img_height = info_header.height;
	if (img_width <= 0 || img_height <= 0 || img_width > buffer_width || img_height > buffer_height) {
		lcddbg("Error: Invalid image dimensions or too large for buffer (width: %d, height: %d)\n", img_width, img_height);
		goto errout;
	}

	// For 16-bit, each pixel is 2 bytes. Padding is to the next 4-byte boundary.
	int padding = (4 - (img_width * 2) % 4) % 4;

	// Calculate offset to center the image
	int x_offset = (buffer_height - img_height) / 2;
	int y_offset = (buffer_width - img_width) / 2;
	if (x_offset < 0) {
		x_offset = 0;
	}
	if (y_offset < 0) {
		y_offset = 0;
	}

	// BMP stores pixels bottom-to-top. We read row by row.
	for (int src_y = 0; src_y < img_height; src_y++) {
		int bmp_file_y = img_height - 1 - src_y;
		off_t file_offset_for_row = header.data_offset + (off_t)bmp_file_y * (img_width * 2 + padding);
		if (lseek(bmp_fd, file_offset_for_row, SEEK_SET) == (off_t)-1) {
			lcddbg("Failed to seek to BMP row\n");
			goto errout;
		}
		// Read one row of pixel data (as 16-bit words)
		uint16_t *temp_row_buffer = (uint16_t *)kmm_malloc(sizeof(uint16_t) * img_width);
		if (!temp_row_buffer) {
			lcddbg("Failed to allocate row buffer\n");
			goto errout;
		}

		if (read(bmp_fd, temp_row_buffer, sizeof(uint16_t) * img_width) != sizeof(uint16_t) * img_width) {
			lcddbg("Failed to read BMP pixel row\n");
			kmm_free(temp_row_buffer);
			goto errout;
		}
		for (int src_x = 0; src_x < img_width; src_x++) {
			int dest_x = src_y;
			int dest_y = src_x;
			int target_buffer_index = ((x_offset + dest_x) * buffer_width + (y_offset + dest_y)) * 2;
			uint16_t bmp_pixel = temp_row_buffer[src_x];
			uint16_t rgb565_pixel = convert_pixel_to_rgb565(bmp_pixel, r_mask, g_mask, b_mask);
			buffer[target_buffer_index] = (uint8_t)(rgb565_pixel & 0xFF);
			buffer[target_buffer_index + 1] = (uint8_t)((rgb565_pixel >> 8) & 0xFF);
		}
		kmm_free(temp_row_buffer);
	}
	
	close(bmp_fd);
	return OK;

errout:
	close(bmp_fd);
	return ERROR;
}
#endif /* CONFIG_LCD_SPLASH_IMAGE_BMP_16BIT */

#endif /* CONFIG_LCD_SPLASH_IMAGE */
