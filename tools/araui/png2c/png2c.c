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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <png.h>

typedef enum {
	UI_PIXEL_FORMAT_UNKNOWN,
	UI_PIXEL_FORMAT_GRAY4,
	UI_PIXEL_FORMAT_GRAYA44,
	UI_PIXEL_FORMAT_RGB565,
	UI_PIXEL_FORMAT_ARGB8565,
	UI_PIXEL_FORMAT_RGBA5658,
	UI_PIXEL_FORMAT_ARGB4444,
	UI_PIXEL_FORMAT_RGBA4444,
	UI_PIXEL_FORMAT_RGB888,
	UI_PIXEL_FORMAT_ARGB8888,
	UI_PIXEL_FORMAT_RGBA8888,
	UI_PIXEL_FORMAT_A8
} ui_pixel_format_t;

typedef struct {
	uint32_t id;
	int32_t width;
	int32_t height;
	ui_pixel_format_t pf;
	uint32_t header_size;
	uint32_t data_size;
	int32_t reserved[8];
} __attribute__((packed)) ui_bitmap_data_t;

static void convert_to_c(
	const char *c_filename, const char *var_name, png_bytep *row_pointers,
	int width, int height, png_byte color_type, png_byte bit_depth) {

	int data_size = 0;

	FILE *fp = fopen(c_filename, "w");

	fprintf(fp, "#include <stdint.h>\n\n");

	if (color_type == 6) { // RGBA
		data_size = width * height * (bit_depth / 8) * 4;
	} else if (color_type == 2) { // RGB
		data_size = width * height * (bit_depth / 8) * 3;
	}

	fprintf(fp, "const uint8_t %s[%lu] = {\n", var_name, data_size + sizeof(ui_bitmap_data_t));

	ui_bitmap_data_t header = {
		.id = 0x00,
		.width = width,
		.height = height,
		.pf = 0x0a,
		.header_size = sizeof(ui_bitmap_data_t),
		.data_size = data_size
	};

	header.reserved[0] = header.reserved[1] = header.reserved[2] = header.reserved[3] = 0x00;
	header.reserved[4] = header.reserved[5] = header.reserved[6] = header.reserved[7] = 0x00;

	uint8_t *hp = (uint8_t *)&header;

	fprintf(fp, "\t");
	for (int i = 0; i < header.header_size; i++) {
		fprintf(fp, "0x%02x,", *hp++);
	}
	fprintf(fp, "\n");

	int bytes_per_pixel = data_size / (width * height);

	for (int y = 0; y < height; y++) {
		fprintf(fp, "\t");
		for (int x = 0; x < width; x++) {
			for (int i = 0; i < bytes_per_pixel; i++) {
				fprintf(fp, "0x%02x", row_pointers[y][x * bytes_per_pixel + i]);
				if (y != (height - 1) || x != (width - 1) || i != (bytes_per_pixel - 1)) {
					fprintf(fp, ",");
				}
			}
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "};\n\n");

	fclose(fp);
}

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Usage: %s <png filename> <c filename> <variable name>\n", argv[0]);
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		return 0;
	}

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	setjmp(png_jmpbuf(png));

	png_init_io(png, fp);
	png_read_info(png, info);

	int width = png_get_image_width(png, info);
	int height = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	png_read_update_info(png, info);

	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);

	convert_to_c(argv[2], argv[3], row_pointers, width, height, color_type, bit_depth);

	fclose(fp);

	for (int y = 0; y < height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);

	png_destroy_read_struct(&png, &info, NULL);

	return 0;
}
