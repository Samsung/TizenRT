/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/rtc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define LCD_DEV_PATH "/dev/lcd%d"
#define LCD_DEV_PORT 0
#define SIZE 40
static int xres;
static int yres;
// RGB565 helper macro
#define RGB565(r, g, b) (((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F))

// --- Helper for RGB565 blending ---
static inline uint16_t blend_rgb565(uint16_t bg, uint16_t fg, uint8_t alpha)
{
	uint8_t br = (bg >> 11) & 0x1F;
	uint8_t bgc = (bg >> 5) & 0x3F;
	uint8_t bb = bg & 0x1F;

	uint8_t fr = (fg >> 11) & 0x1F;
	uint8_t fg_c = (fg >> 5) & 0x3F;
	uint8_t fb = fg & 0x1F;

	uint8_t r = (br * (255 - alpha) + fr * alpha) / 255;
	uint8_t g = (bgc * (255 - alpha) + fg_c * alpha) / 255;
	uint8_t b = (bb * (255 - alpha) + fb * alpha) / 255;

	return (r << 11) | (g << 5) | b;
}

// --- Safe pixel write with alpha ---
static inline void set_pixel_aa(int x, int y, int width, int height, uint16_t color, uint8_t alpha, uint8_t *lcd_data)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	int idx = (y * width + x) * 2;
	uint16_t bg = (lcd_data[idx] << 8) | lcd_data[idx + 1];
	uint16_t blended = blend_rgb565(bg, color, alpha);
	lcd_data[idx + 1] = (blended >> 8) & 0xFF;
	lcd_data[idx] = blended & 0xFF;
}

// --- Draw circle with anti-aliasing ---
static uint16_t alpha_blend(uint16_t bg, uint16_t fg, float alpha)
{
	// Extract 24-bit channels
	int fg_r = ((fg >> 11) & 0x1F) << 3;
	int fg_g = ((fg >> 5) & 0x3F) << 2;
	int fg_b = (fg & 0x1F) << 3;

	int bg_r = ((bg >> 11) & 0x1F) << 3;
	int bg_g = ((bg >> 5) & 0x3F) << 2;
	int bg_b = (bg & 0x1F) << 3;

	// Blend in 24-bit
	int r = (int)(alpha * fg_r + (1.0f - alpha) * bg_r);
	int g = (int)(alpha * fg_g + (1.0f - alpha) * bg_g);
	int b = (int)(alpha * fg_b + (1.0f - alpha) * bg_b);

	// Convert back to 16-bit RGB565
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

/* Draws a filled circle using Supersampling Anti-Aliasing (SSAA) with alpha blending for smooth edges */
void draw_circle_test(int xres, int yres, int color)
{
	struct lcddev_area_s area;
	char port[20] = {'\0'};
	int fd = 0;
	size_t len;
	int x0;
	int y0;
	len = xres * yres * 2 + 1;
	x0 = xres / 2;
	y0 = yres / 2;
	uint8_t *lcd_data = (uint8_t *)malloc(len);
	if (lcd_data == NULL) {
		printf("malloc failed for lcd data : %d\n", len);
		return;
	}
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		free(lcd_data);
		return;
	}
	set_lcd_area_data(&area, lcd_data);

	// Fill background with white
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data[i + 1] = (0xFFFF & 0xFF00) >> 8;
		lcd_data[i] = 0xFFFF & 0x00FF;
	}
	int width = xres;
	int height = yres;
	int radius = 70;

	// Supersampling factor (4×4 grid inside each pixel)
	const int SS = 4;
	const int SS2 = SS * SS;
	int r2 = radius * radius;

	for (int py = -radius; py <= radius; py++) {
		for (int px = -radius; px <= radius; px++) {
			int screen_x = x0 + px;
			int screen_y = y0 + py;
			if (screen_x < 0 || screen_x >= width || screen_y < 0 || screen_y >= height)
				continue;

			int inside_count = 0;

			// Supersample this pixel
			for (int sy = 0; sy < SS; sy++) {
				for (int sx = 0; sx < SS; sx++) {
					float sub_x = px + (sx + 0.5f) / SS;
					float sub_y = py + (sy + 0.5f) / SS;
					if (sub_x * sub_x + sub_y * sub_y <= r2) {
						inside_count++;
					}
				}
			}

			float coverage = (float)inside_count / SS2;

			if (coverage > 0.0f) {
				uint16_t blended = alpha_blend(0xFFFF, color, coverage); // blend with white background
				int idx = (screen_y * width + screen_x) * 2;
				lcd_data[idx + 1] = (blended & 0xFF00) >> 8;
				lcd_data[idx] = blended & 0x00FF;
			}
		}
	}

	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
	}
	free(lcd_data);
	close(fd);
}

void checkerboard_pattern_test(int xres, int yres)
{
	int fd = 0;
	char port[20] = {'\0'};
	struct lcddev_area_s area;
	size_t len;
	uint16_t black = RGB565(0, 0, 0);
	uint16_t white = RGB565(0x1F, 0x3F, 0x1F);
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return;
	}
	len = xres * yres * 2 + 1;
	uint8_t *lcd_data = (uint8_t *)malloc(len);
	if (lcd_data == NULL) {
		printf("malloc failed for lcd data : %d\n", len);
		close(fd);
		return;
	}
	set_lcd_area_data(&area, lcd_data);
	uint16_t color;
	bool flag = true;
	for (int y = 0; y < yres / SIZE * 2; y++) {
		for (int x = 0; x < xres / SIZE; x++) {
			if (flag) {
				color = white;
			} else {
				color = black;
			}
			flag = !flag; /* Toggle flag */
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					int pixel_x = x * SIZE + i;
					int pixel_y = y * SIZE + j;
					lcd_data[pixel_y * xres + pixel_x] = (color & 0xFF00) >> 8;
				}
			}
		}
		flag = !flag; /* Toggle flag */
	}
	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
	}
	close(fd);
	free(lcd_data);
}

// Bresenham's line algorithm implementation for drawing lines
static void draw_line(int x0, int y0, int x1, int y1, uint16_t color, uint8_t *framebuffer, int xres, int yres)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (1) {
		if (x0 >= 0 && x0 < xres && y0 >= 0 && y0 < yres) {
			framebuffer[2 * (y0 * xres + x0)] = color & 0xFF;
			framebuffer[2 * (y0 * xres + x0) + 1] = (color >> 8) & 0xFF;
		}

		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void line_draw_test(int xres, int yres)
{
	int fd = 0;
	int width = xres;
	int height = yres;
	char port[20] = {'\0'};
	struct lcddev_area_s area;
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return;
	}
	uint8_t *framebuffer = (uint8_t *)malloc(2 * xres * yres + 1);
	if (framebuffer == NULL) {
		printf("malloc failed for framebuffer\n");
		close(fd);
		return;
	}
	uint16_t red = RGB565(31, 0, 0);   // red
	uint16_t green = RGB565(0, 63, 0); // green
	uint16_t blue = RGB565(0, 0, 31);  // blue
	// Fill background with black
	for (int i = 0; i < 2 * xres * yres + 1; i++) {
		framebuffer[i] = 0;
	}
	// Horizontal middle line
	draw_line(0, yres / 2, xres - 1, yres / 2, red, framebuffer, xres, yres);

	// Vertical middle line
	draw_line(xres / 2, 0, xres / 2, yres - 1, green, framebuffer, xres, yres);

	// Diagonals across full screen
	draw_line(0, 0, xres - 1, yres - 1, blue, framebuffer, xres, yres); // top-left → bottom-right
	draw_line(xres - 1, 0, 0, yres - 1, blue, framebuffer, xres, yres); // top-right → bottom-left
	set_lcd_area_data(&area, framebuffer);
	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
	}
	free(framebuffer);
	close(fd);
}

static inline uint8_t *pixel_ptr(uint8_t *buff, int x, int y, int xres)
{
	return (buff + ((y * xres) + x) * 2);
}

void sliding_frame_test(int xres, int yres)
{
	int fd = 0;
	char port[20] = {'\0'};
	int step;
	int step_size = 4;
	struct lcddev_area_s area;
	uint16_t red = RGB565(31, 0, 0);   // red
	uint16_t green = RGB565(0, 63, 0); // green
	uint16_t blue = RGB565(0, 0, 31);  // blue
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return;
	}
	uint8_t *lcd_data = (uint8_t *)malloc(xres * yres * 2 + 1);
	if (lcd_data == NULL) {
		printf("malloc failed for lcd data\n");
		close(fd);
		return;
	}
	uint8_t *lcd_data1 = (uint8_t *)malloc(xres * yres * 2 + 1);
	if (lcd_data1 == NULL) {
		printf("malloc failed for lcd data1\n");
		free(lcd_data);
		close(fd);
		return;
	}
	uint8_t *lcd_data2 = (uint8_t *)malloc(xres * yres * 2 + 1);
	if (lcd_data2 == NULL) {
		printf("malloc failed for lcd data2\n");
		free(lcd_data);
		free(lcd_data1);
		close(fd);
		return;
	}
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data1[i + 1] = (blue & 0xFF00) >> 8;
		lcd_data1[i] = blue & 0x00FF;
	}
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data2[i + 1] = (red & 0xFF00) >> 8;
		lcd_data2[i] = red & 0x00FF;
	}
	draw_line(xres / 8, 0, xres / 8, yres - 1, 0, lcd_data1, xres, yres);
	draw_line(7 * xres / 8, 0, 7 * xres / 8, yres - 1, 0, lcd_data1, xres, yres);
	draw_line(xres / 8, 0, xres / 8, yres - 1, 0, lcd_data2, xres, yres);
	draw_line(7 * xres / 8, 0, 7 * xres / 8, yres - 1, 0, lcd_data2, xres, yres);
	set_lcd_area_data(&area, lcd_data);
	for (step = 0; step <= xres; step += step_size) {
		for (int y = 0; y < yres; y++) {
			if (step > 0) {
				memcpy(pixel_ptr(lcd_data, xres - step, y, xres), pixel_ptr(lcd_data2, 0, y, xres), step * 2);
			}
			if (step < xres) {
				memcpy(pixel_ptr(lcd_data, 0, y, xres), pixel_ptr(lcd_data1, step, y, xres), (xres - step) * 2);
			}
		}
		if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
			printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
			goto errout;
		}
		if (step == 0) {
			sleep(1);
		}
	}
	sleep(2);
	for (step = 0; step <= xres; step += step_size) {
		for (int y = 0; y < yres; y++) {
			if (step > 0) {
				memcpy(pixel_ptr(lcd_data, 0, y, xres), pixel_ptr(lcd_data1, xres - step, y, xres), step * 2);
			}
			if (step < xres) {
				memcpy(pixel_ptr(lcd_data, step, y, xres), pixel_ptr(lcd_data2, 0, y, xres), (xres - step) * 2);
			}
		}
		if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
			printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
			goto errout;
		}
		if (step == 0) {
			sleep(1);
		}
	}
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data1[i + 1] = (blue & 0xFF00) >> 8;
		lcd_data1[i] = blue & 0x00FF;
	}
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data2[i + 1] = (red & 0xFF00) >> 8;
		lcd_data2[i] = red & 0x00FF;
	}
	draw_line(0, yres / 8, xres - 1, yres / 8, 0, lcd_data1, xres, yres);
	draw_line(0, 7 * yres / 8, xres - 1, 7 * yres / 8, 0, lcd_data1, xres, yres);
	draw_line(0, yres / 8, xres - 1, yres / 8, 0, lcd_data2, xres, yres);
	draw_line(0, 7 * yres / 8, xres - 1, 7 * yres / 8, 0, lcd_data2, xres, yres);
	sleep(2);
	for (step = 0; step < yres; step += step_size) {
		memcpy(pixel_ptr(lcd_data, 0, 0, xres), pixel_ptr(lcd_data2, 0, yres - step - 1, xres), 2 * xres * (step + 1));
		if (step + 1 < yres) {
			memcpy(pixel_ptr(lcd_data, 0, step + 1, xres), pixel_ptr(lcd_data1, 0, 0, xres), 2 * xres * (yres - step - 1));
		}
		if (step + step_size >= yres) {
			memcpy(pixel_ptr(lcd_data, 0, 0, xres), pixel_ptr(lcd_data2, 0, 0, xres), 2 * xres * yres);
		}
		if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
			printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
			goto errout;
		}
	}
	sleep(2);
	for (step = 0; step < yres; step += step_size) {
		memcpy(pixel_ptr(lcd_data, 0, yres - step - 1, xres), pixel_ptr(lcd_data1, 0, 0, xres), 2 * xres * (step + 1));
		if (step + 1 < yres) {
			memcpy(pixel_ptr(lcd_data, 0, 0, xres), pixel_ptr(lcd_data2, 0, step + 1, xres), 2 * xres * (yres - step - 1));
		}
		if (step + step_size >= yres) {
			memcpy(pixel_ptr(lcd_data, 0, 0, xres), pixel_ptr(lcd_data1, 0, 0, xres), 2 * xres * yres);
		}
		if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
			printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
			goto errout;
		}
	}
errout:
	free(lcd_data);
	free(lcd_data1);
	free(lcd_data2);
	close(fd);
}

void gradual_power_test(void)
{
	int power;
	for (power = 99; power >= 0; power--) {
		if (power_test(power) != OK) {
			printf("ERROR: power_test failed\n");
			return; /* Return if any error occurs during power setting */
		}
		usleep(90000);
	}
	sleep(1);
	for (power = 1; power <= 100; power++) {
		if (power_test(power) != OK) {
			printf("ERROR: power_test failed\n");
			return; /* Return if any error occurs during power setting */
		}
		usleep(90000);
	}
}
