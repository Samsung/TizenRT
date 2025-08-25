/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/lcd_test/example_lcd.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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

#define RED   0xF800
#define WHITE 0xFFFF
#define BLACK 0x0000
#define GREEN 0xE007
#define BLUE  0x00F8
#define SIZE  40

#define COLINDEX 10
#define ROWINDEX 10
#define NOPIXELS 200
static int xres;
static int yres;
static bool g_terminate;

#ifdef CONFIG_EXAMPLE_LCD_FPS_TEST
#define EXAMPLE_LCD_FPS_TEST CONFIG_EXAMPLE_LCD_FPS_TEST
#else
#define EXAMPLE_LCD_FPS_TEST 5000
#endif
// RGB565 helper macro
#define RGB565(r,g,b)  (((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F))

static void putarea(int x1, int x2, int y1, int y2, int color)
{
	struct lcddev_area_s area;
	char port[20] = { '\0' };
	int fd = 0;
	size_t len;
	len = xres * yres * 2 + 1;
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
	area.planeno = 0;
	area.row_start = x1;
	area.row_end = x2;
	area.col_start = y1;
	area.col_end = y2;
	area.stride = 2 * xres;
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data[i] = (color & 0xFF00) >> 8;
		lcd_data[i + 1] = color & 0x00FF;
	}

	area.data = lcd_data;
	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		 printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
	}
	close(fd);
	free(lcd_data);
}

static void test_init(void)
{
	int ret;
	int fd = 0;
	char port[20] = { '\0' };
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	if (ioctl(fd, LCDDEVIO_INIT, &ret) < 0) {
		printf("Fail to call LCD INIT(errno %d)", get_errno());
	}
	close(fd);
}

static void test_orientation(void)
{
	int fd = 0;
	char port[20] = { '\0' };
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	if (ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_RLANDSCAPE) < 0) {
		printf("Fail to call LCD SETORIENTATION(errno %d)", get_errno());
		goto cleanup;
	}

	sleep(1);
	/* resolution should be swapped now as orientation is changed */
#if defined(CONFIG_LCD_PORTRAIT) || defined(CONFIG_LCD_RPORTRAIT)
	putarea(0, xres - 1, 0, yres - 1, RED);
#else
	putarea(0, yres - 1, 0, xres - 1, RED);
#endif
	/* fill square with side  = OFFSET */
	putarea(0, SIZE, 0, SIZE, WHITE);

	sleep(1);
	/* resetting original orientation - the one defined in config */
#if defined(CONFIG_LCD_PORTRAIT)
	if (ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_PORTRAIT) < 0) {
		printf("Fail to call LCD SETORIENTATION(errno %d)", get_errno());
		goto cleanup;
	}
#elif defined(CONFIG_LCD_LANDSCAPE)
	if (ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_LANDSCAPE) < 0) {
		printf("Fail to call LCD SETORIENTATION(errno %d)", get_errno());
		goto cleanup;
	}
#elif defined(CONFIG_LCD_RLANDSCAPE)
	if (ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_RLANDSCAPE) < 0) {
		printf("Fail to call LCD SETORIENTATION(errno %d)", get_errno());
		goto cleanup;
	}
#elif defined(CONFIG_LCD_RPORTRAIT)
	if (ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_RPORTRAIT) < 0) {
		printf("Fail to call LCD SETORIENTATION(errno %d)", get_errno());
		goto cleanup;
	}
#else
	if (ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_LANDSCAPE) < 0) {
		printf("Fail to call LCD SETORIENTATION(errno %d)", get_errno());
		goto cleanup;
	}
#endif

cleanup:
	close(fd);
}

static void test_put_area_pattern(void)
{
	int fd = 0;
	char port[20] = { '\0' };
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	struct fb_videoinfo_s vinfo;
	if (ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo) < 0) {
		printf("Fail to call LCD GETVIDEOINFO(errno %d)", get_errno());
		close(fd);
		return;
	}
	xres = vinfo.xres;
	yres = vinfo.yres;
	printf("xres : %d, yres:%d\n", xres, yres);
	close(fd);
	putarea(0, yres - 1, 0, xres - 1, BLUE);
	sleep(3);
	putarea(0, yres - 1, 0, xres - 1, GREEN);
	sleep(3);
	putarea(0, yres - 1, 0, xres - 1, RED);
	sleep(3);
	putarea(0, yres - 1, 0, xres - 1, BLACK);
	sleep(3);
	putarea(0, yres - 1, 0, xres - 1, WHITE);
	sleep(3);
}

static unsigned short generate_color_code(int red, int green, int blue)
{
	// Ensure that RGB values are within the valid range (0-31)
	red = red % 31;
	green = green % 31;
	blue = blue % 31;
	// Combine RGB components into a 16-bit hex color code
	unsigned short colorCode = (red << 11) | (green << 5) | blue;
	return colorCode;
}

static void test_bit_map(void)
{
	int fd = 0;
	char port[20] = { '\0' };
	struct lcddev_area_s area;
	size_t len;
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	struct fb_videoinfo_s vinfo;
	if (ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo) < 0) {
		printf("Fail to call LCD GETVIDEOINFO(errno %d)", get_errno());
		close(fd);
		return;
	}
	xres = vinfo.xres;
	yres = vinfo.yres;
	len = xres * yres * 2 + 1;
        uint8_t *lcd_data = (uint8_t *)malloc(len);
        if (lcd_data == NULL) {
                printf("malloc failed for lcd data : %d\n", len);
		close(fd);
                return;
        }
	area.planeno = 0;
	area.row_start = 0;
	area.row_end = yres - 1;
	area.col_start = 0;
	area.col_end = xres - 1;
	area.stride = 2 * xres;
	area.data = lcd_data;
	uint16_t color;
	for (int y = 0; y < yres / SIZE * 2; y++) {
		for (int x = 0; x < xres / SIZE; x++) {
			color = generate_color_code(rand() % 31, rand() % 31, rand() % 31);
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					int pixel_x = x * SIZE + i;
					int pixel_y = y * SIZE + j;
					lcd_data[pixel_y * xres + pixel_x] = (color & 0xFF00) >> 8;
				}
			}
		}
	}
	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
	}
	close(fd);
	free(lcd_data);
}

static void test_quad(void)
{
	int fd = 0;
	char port[20] = {'\0'};
	struct lcddev_area_s area;
	size_t len;
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	struct fb_videoinfo_s vinfo;
	if (ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo) < 0) {
		printf("Fail to call LCD GETVIDEOINFO(errno %d)", get_errno());
		close(fd);
		return;
	}
	xres = vinfo.xres;
	yres = vinfo.yres;
	len = xres * yres * 2 + 1;
	uint8_t *lcd_data = (uint8_t *)malloc(len);
	if (lcd_data == NULL) {
		printf("malloc failed for lcd data : %d\n", len);
		close(fd);
		return;
	}
	area.planeno = 0;
	area.row_start = 0;
	area.row_end = yres - 1;
	area.col_start = 0;
	area.col_end = xres - 1;
	area.stride = 2 * xres;
	area.data = lcd_data;
	int pixel_index = 0;
	uint16_t color;

	for (int y = 0; y < yres; y++) {
		for (int x = 0; x < xres; x++) {
			pixel_index = ((y * xres) + x) * 2;
			if (x < xres / 2) {
				if (y < yres / 2) {
					color = RED;
				} else {
					color = BLUE;
				}
			} else {
				if (y < yres / 2) {
					color = GREEN;
				} else {
					color = WHITE;
				}
			}
			lcd_data[pixel_index] = (color & 0xFF00) >> 8;
			lcd_data[pixel_index + 1] = color & 0x00FF;
		}
	}
	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
	}
	close(fd);
	free(lcd_data);
}

static void release_frame_buffer(struct lcddev_area_s *area)
{
	free(area->data);
}

static int prepare_frame_buffer(struct lcddev_area_s *area, uint16_t color, int xres, int yres)
{
	size_t len;
	len = xres * yres * 2 + 1;
	uint8_t *lcd_data = (uint8_t *)malloc(len);
	if (lcd_data == NULL) {
		printf("malloc failed for lcd data : %d\n", len);
		return ERROR;
	}

	for (int i = 0; i < len - 1; i += 2) {
		lcd_data[i] = (color & 0xFF00) >> 8;
		lcd_data[i + 1] = color & 0x00FF;
	}

	area->planeno = 0;
	area->row_start = 0;
	area->row_end = yres - 1;
	area->col_start = 0;
	area->col_end = xres - 1;
	area->stride = 2 * xres;
	area->data = lcd_data;

	return OK;
}

static void test_fps(void)
{
	int ret;
	int fd_rtc = 0;
	int fd_lcd = 0;
	char port[20] = { '\0' };
	struct lcddev_area_s area_red;
	struct lcddev_area_s area_blue;
	struct fb_videoinfo_s vinfo;
	fd_rtc = open("/dev/rtc0", O_RDWR);
	if (fd_rtc < 0) {
		printf("ERROR: LCD FPS test, Fail to open rtc.\n");
		return;
	}

	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd_lcd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd_lcd < 0) {
		printf("ERROR: LCD FPS test, Failed to open lcd port : %s error:%d\n", port, get_errno());
		close(fd_rtc);
		return;
	}
	if (ioctl(fd_lcd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo) < 0) {
		printf("Fail to call LCD GETVIDEOINFO(errno %d)", get_errno());
		close(fd_rtc);
		close(fd_lcd);
		return;
	}
	ret = prepare_frame_buffer(&area_red, RED, xres, yres);
	if (ret != OK) {
		printf("ERROR: prepare_frame_buffer failed\n");
		close(fd_rtc);
		close(fd_lcd);
		return;
	}
	ret = prepare_frame_buffer(&area_blue, BLUE, xres, yres);
	if (ret != OK) {
		printf("ERROR: prepare_frame_buffer failed\n");
		release_frame_buffer(&area_red);
		close(fd_rtc);
		close(fd_lcd);
		return;
	}

	struct rtc_time start_time = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time end_time;

	bool is_red = true;
	//Start Test
	if (ioctl(fd_rtc, RTC_RD_TIME, (unsigned long)&start_time) < 0) {
		printf("Fail to call RTC READ TIME(errno %d)", get_errno());
		goto cleanup;
	}
	for (int itr = 0; itr < EXAMPLE_LCD_FPS_TEST; itr++) {
		if (is_red) {
			if (ioctl(fd_lcd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area_red) < 0) {
				printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
				goto cleanup;
			}
			is_red = false;
		} else {
			if (ioctl(fd_lcd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area_blue) < 0) {
				printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
				goto cleanup;
			}
			is_red = true;
		}
	}
	if (ioctl(fd_rtc, RTC_RD_TIME, (unsigned long)&end_time) < 0) {
		printf("Fail to call RTC READ TIME(errno %d)", get_errno());
		goto cleanup;
	}
	//End test
	
	time_t start;
	time_t end;
	start = mktime((FAR struct tm *)&start_time);
	end = mktime((FAR struct tm *)&end_time);

	int time_elapsed = difftime(end, start);
	if (time_elapsed != 0) {
		float fps = EXAMPLE_LCD_FPS_TEST / time_elapsed;
		printf("FPS Test: %d frames executed in %d sec, FPS: %.2f\n", EXAMPLE_LCD_FPS_TEST, time_elapsed, fps);
		goto cleanup;
	} else {
		printf("FPS calculation failed! Please increase the number of frames execution using CONFIG_EXAMPLE_LCD_FPS_TEST!\n");
	}

cleanup:
	close(fd_rtc);
	close(fd_lcd);
	release_frame_buffer(&area_red);
	release_frame_buffer(&area_blue);
}

bool is_valid_power(char *power)
{
	int power_val_size = strlen(power);

	if (power_val_size < 1 || power_val_size > 3) {		/* Length of Power val should be 1, 2, or 3 */
		return false;
	}
	for (int i = 0; i < power_val_size; i++) {
		if (!isdigit(power[i])) {	/* If not a digit */
			return false;
		}
	}
	return true;
}

bool is_valid_stress_test_arg(char *arg)
{
	int mode;
	int arg_size = strlen(arg);

	if (arg_size != 1) {		/* Length of argument should be 1 */
		return false;
	}
	for (int i = 0; i < arg_size; i++) {
		if (!isdigit(arg[i])) {	/* If not a digit */
			return false;
		}
	}
	mode = atoi(arg);
	if (mode < 0 || mode > 2) {
		return false;
	}
	return true;
}

static void power_cycle_test(void)
{
	int fd = 0;
	char port[20] = { '\0' };
	int power = 0;
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return;
	}
	while (!g_terminate) {
		if (ioctl(fd, LCDDEVIO_SETPOWER, power) < 0) {
			printf("Fail to call LCD SETPOWER(errno %d)", get_errno());
			goto cleanup;
		}
		if (power == 100) {
			power = 0;
		} else {
			power = 100;
		}
		usleep(10000); /*Sleep for 10 ms*/
	}
cleanup:
	close(fd);
}

static void frame_change_test(void)
{
	int fd = 0;
	int ret;
	char port[20] = { '\0' };
	struct lcddev_area_s area_red;
	struct lcddev_area_s area_blue;
	struct fb_videoinfo_s vinfo;
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: STRESS TEST, Failed to open lcd port : %s error:%d\n", port, get_errno());
		return;
	}
	if (ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo) < 0) {
		printf("Fail to call LCD GETVIDEOINFO(errno %d)", get_errno());
		close(fd);
		return;
	}
	xres = vinfo.xres;
	yres = vinfo.yres;
	ret = prepare_frame_buffer(&area_red, RED, xres, yres);
	if (ret != OK) {
		printf("ERROR: prepare_frame_buffer failed\n");
		close(fd);
		return;
	}
	ret = prepare_frame_buffer(&area_blue, BLUE, xres, yres);
	if (ret != OK) {
		printf("ERROR: prepare_frame_buffer failed\n");
		close(fd);
		release_frame_buffer(&area_red);
		return;
	}

	bool is_red = true;
	while (!g_terminate) {
		if (is_red) {
			if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area_red) < 0) {
				printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
				goto cleanup;
			}
			is_red = false;
		} else {
			if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area_blue) < 0) {
				printf("ERROR: PUTAREA ioctl failed, errno: %d\n", get_errno());
				goto cleanup;
			}
			is_red = true;
		}
		usleep(100000);	/* Sleep for 100ms */
	}
cleanup:
	close(fd);
	release_frame_buffer(&area_red);
	release_frame_buffer(&area_blue);
}

static int lcd_get_info(void)
{
	int ret = OK;
	int fd = 0;
	char port[20] = {'\0'};
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return ERROR;
	}

	struct lcd_info_s lcdinfo;
	ret = ioctl(fd, LCDDEVIO_GETLCDINFO, (unsigned long)(uintptr_t)&lcdinfo);
	if (ret != OK) {
		printf("Fail to LCDDEVIO_GETLCDINFO %s, errno:%d\n", port, get_errno());
		close(fd);
		return ERROR;
	}

	printf("LCD HEIGHT MM: %d\n", lcdinfo.lcd_height_mm);
	printf("LCD WIDTH MM: %d\n", lcdinfo.lcd_width_mm);
	printf("LCD SIZE INCH: %2f\n", lcdinfo.lcd_size_inch);
	printf("LCD DPI: %2f\n", lcdinfo.lcd_dpi);

	close(fd);
	return ret;
}

static void show_usage(void)
{
	printf("usage: lcd_test <command args(optional)>\n");
	printf("    basic             : Execute basic lcd_test\n");
	printf("    lcdinfo           : Print LCD basic info like width, height, DPI \n");
	printf("    power <value>     : Sets the brightness to given value\n");
	printf("    stress_test <start> <mode> | <stop> : Start or stop stress test, <mode>: 0 = power cycle test only, 1 = frame change test only, 2 = both test simultaneously\n");
	printf("    factory_test      : Execute factory test\n");
}

static void stress_test(int num)
{
	switch (num) {
	case 0: {
		task_create("lcd_power_on_off_test", SCHED_PRIORITY_DEFAULT, 4096, power_cycle_test, NULL);	/* Create task for stress test */
	}
	break;
	case 1: {
		task_create("lcd_frame_repeat_test", SCHED_PRIORITY_DEFAULT, 4096, frame_change_test, NULL);	/* Create task for stress test */
	}
	break;
	case 2: {
		task_create("lcd_power_on_off_test", SCHED_PRIORITY_DEFAULT, 4096, power_cycle_test, NULL);	/* Create task for stress test */
		task_create("lcd_frame_repeat_test", SCHED_PRIORITY_DEFAULT, 4096, frame_change_test, NULL);	/* Create task for stress test */
	}
	break;
	default: {
		printf("ERROR: Invalid argument for stress test\n");
		show_usage();	/* Show usage */
	}
	break;
	}
}

static int power_test(int power)
{
	int count = 0;
	int fd = 0;
	char port[20] = { '\0' };
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return ERROR;
	}
	if (ioctl(fd, LCDDEVIO_SETPOWER, power) < 0) {
		printf("Fail to call LCD SETPOWER(errno %d)", get_errno());
	}
	close(fd);
	return OK;
}

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
static void draw_circle_test(int x0, int y0, int color)
{
	struct lcddev_area_s area;
	char port[20] = {'\0'};
	int fd = 0;
	size_t len;
	len = xres * yres * 2 + 1;
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

	area.planeno = 0;
	area.row_start = 0;
	area.row_end = yres - 1;
	area.col_start = 0;
	area.col_end = xres - 1;
	area.stride = 2 * xres;

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

	area.data = lcd_data;
	if (ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area) < 0) {
		printf("Fail to call LCD PUTAREA(errno %d)", get_errno());
	}
	free(lcd_data);
	close(fd);
}

static void checkerboard_pattern_test(void)
{
	int fd = 0;
	char port[20] = {'\0'};
	struct lcddev_area_s area;
	size_t len;
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
	area.planeno = 0;
	area.row_start = 0;
	area.row_end = yres - 1;
	area.col_start = 0;
	area.col_end = xres - 1;
	area.stride = 2 * xres;
	area.data = lcd_data;
	uint16_t color;
	bool flag = true;
	for (int y = 0; y < yres / SIZE * 2; y++) {
		for (int x = 0; x < xres / SIZE; x++) {
			if (flag) {
				color = WHITE;
			} else {
				color = BLACK;
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
void draw_line(int x0, int y0, int x1, int y1, uint16_t color, uint8_t *framebuffer)
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

static void line_draw_test(void)
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
	draw_line(0, yres / 2, xres - 1, yres / 2, red, framebuffer);

	// Vertical middle line
	draw_line(xres / 2, 0, xres / 2, yres - 1, green, framebuffer);

	// Diagonals across full screen
	draw_line(0, 0, xres - 1, yres - 1, blue, framebuffer); // top-left → bottom-right
	draw_line(xres - 1, 0, 0, yres - 1, blue, framebuffer); // top-right → bottom-left
	area.planeno = 0;
	area.row_start = 0;
	area.row_end = yres - 1;
	area.col_start = 0;
	area.col_end = xres - 1;
	area.stride = 2 * xres;
	area.data = (uint8_t *)framebuffer;
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

static void sliding_frame_test(void)
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
	draw_line(xres / 8, 0, xres / 8, yres - 1, 0, lcd_data1);
	draw_line(7 * xres / 8, 0, 7 * xres / 8, yres - 1, 0, lcd_data1);
	draw_line(xres / 8, 0, xres / 8, yres - 1, 0, lcd_data2);
	draw_line(7 * xres / 8, 0, 7 * xres / 8, yres - 1, 0, lcd_data2);
	area.planeno = 0;
	area.row_start = 0;
	area.row_end = yres - 1;
	area.col_start = 0;
	area.col_end = xres - 1;
	area.stride = 2 * xres;
	area.data = (uint8_t *)lcd_data;
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
	draw_line(0, yres / 8, xres - 1, yres / 8, 0, lcd_data1);
	draw_line(0, 7 * yres / 8, xres - 1, 7 * yres / 8, 0, lcd_data1);
	draw_line(0, yres / 8, xres - 1, yres / 8, 0, lcd_data2);
	draw_line(0, 7 * yres / 8, xres - 1, 7 * yres / 8, 0, lcd_data2);
	sleep(2);
	for (step = 0; step < yres; step += step_size) {
		memcpy(pixel_ptr(lcd_data, 0, 0, xres), pixel_ptr(lcd_data2, 0, yres - step - 1, xres), 2 * xres * (step + 1));
		if (step + 1 < yres) {
			memcpy(pixel_ptr(lcd_data, 0, step + 1, xres), pixel_ptr(lcd_data1, 0, 0, xres), 2 * xres * (yres - step - 1));
		}
		if(step + step_size >= yres) {
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
		if(step + step_size >= yres) {
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

static void gradual_power_test(void)
{
	int power;
	for (power = 99; power >= 0; power--) {
		if (power_test(power)!= OK) {
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

static int lcd_factory_test(void)
{
	printf("====factory test start====\n");
	int count = 0;
	int fd = 0;
	char port[20] = {'\0'};
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return ERROR;
	}
	struct fb_videoinfo_s vinfo;
	if (ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo) < 0) {
		printf("Fail to call LCD GETVIDEOINFO(errno %d)", get_errno());
		close(fd);
		return ERROR;
	}
	xres = vinfo.xres;
	yres = vinfo.yres;
	printf("xres: %d, yres: %d\n", xres, yres);
	uint16_t red = RGB565(31, 0, 0);   // red
	uint16_t green = RGB565(0, 63, 0); // green
	uint16_t blue = RGB565(0, 0, 31);  // blue
	printf("Sliding frame test\n");
	sliding_frame_test();
	sleep(5);
	printf("Line draw test\n");
	line_draw_test();
	sleep(5);
	printf("Checkerboard Pattern Test for distortion\n");
	checkerboard_pattern_test();
	sleep(5);
	printf("Circle Test for distortion\n");
	draw_circle_test(xres / 2, yres / 2, blue);
	sleep(5);
	printf("Gradual Power Test\n");
	gradual_power_test();
	close(fd);
	return OK;
}

static int lcd_basic_test(void)
{
	int count = 0;
	int fd = 0;
	char port[20] = { '\0' };
	snprintf(port, sizeof(port) / sizeof(port[0]), LCD_DEV_PATH, LCD_DEV_PORT);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, get_errno());
		return ERROR;
	}

	while (count < 5) {
		test_put_area_pattern();
		test_quad();
		sleep(3);
		test_bit_map();
		sleep(3);
		power_test(0);
		sleep(15);
		power_test(100);
		count++;
		printf("count :%d\n", count);
	}
	test_fps();
	close(fd);
	return OK;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int lcd_test_main(int argc, char *argv[])
#endif
{
	/* LCD Power test */
	if (argc == 3 && !strncmp(argv[1], "power", 6)) {
		if (is_valid_power(argv[2])) {
			return power_test(atoi(argv[2]));
		} else {
			printf("ERROR: Value of power should be int in range [0, 100]\n");
			printf("Usage: lcd_test power <value>\n");
			printf("0 --> LCD Power OFF\n");
			printf("100 --> LCD Power ON\n");
			return ERROR;
		}
	}
	/* Stress test*/
	if (argc >= 3 && !strncmp(argv[1], "stress_test", 12)) {
		if (!strncmp(argv[2], "start", 6)) {
			if (argc > 3 && is_valid_stress_test_arg(argv[3])) {
				g_terminate = false;
				stress_test(atoi(argv[3]));
				return OK;
			}
		} else if (!strncmp(argv[2], "stop", 5)) {
			g_terminate = true;
			return OK;
		}
		printf("ERROR: Invalid arguments for stress test\n");
	}
	if (argc == 2) {
		if (!strncmp(argv[1], "lcdinfo", 8)) {
			return lcd_get_info();
		} else if (!strncmp(argv[1], "basic", 6)) {
			return lcd_basic_test();
		} else if (!strncmp(argv[1], "factory_test", 13)) {
			return lcd_factory_test();
		}
	}
	show_usage();
	return OK;
}
