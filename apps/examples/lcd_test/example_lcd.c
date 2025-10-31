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
#define GREEN 0x07E0
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
#define RGB565(r, g, b)  (((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F))

void set_lcd_area_data(struct lcddev_area_s *area, uint8_t *lcd_data)
{
	area->planeno = 0;
	area->row_start = 0;
	area->row_end = yres - 1;
	area->col_start = 0;
	area->col_end = xres - 1;
	area->stride = 2 * xres;
	area->data = lcd_data;
}

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
	for (int i = 0; i < xres * yres * 2; i += 2) {
		lcd_data[i + 1] = (color & 0xFF00) >> 8;
		lcd_data[i] = color & 0x00FF;
	}
	set_lcd_area_data(&area, lcd_data);
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
	set_lcd_area_data(&area, lcd_data);
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
	set_lcd_area_data(&area, lcd_data);
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
			lcd_data[pixel_index + 1] = (color & 0xFF00) >> 8;
			lcd_data[pixel_index] = color & 0x00FF;
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
		lcd_data[i + 1] = (color & 0xFF00) >> 8;
		lcd_data[i] = color & 0x00FF;
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
	printf("    verification_test   : Execute LCD verification test\n");
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

int power_test(int power)
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

static int lcd_verification_test(void)
{
	printf("====LCD Verification Test Start====\n");
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
	uint16_t red = RGB565(31, 0, 0);   // red
	uint16_t green = RGB565(0, 63, 0); // green
	uint16_t blue = RGB565(0, 0, 31);  // blue
	printf("Sliding frame test\n");
	sliding_frame_test(xres, yres);
	sleep(5);
	printf("Line draw test\n");
	line_draw_test(xres, yres);
	sleep(5);
	printf("Checkerboard Pattern Test for distortion\n");
	checkerboard_pattern_test(xres, yres);
	sleep(5);
	printf("Circle Test for distortion\n");
	draw_circle_test(xres, yres, blue);
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
		} else if (!strncmp(argv[1], "verification_test", 18)) {
			return lcd_verification_test();
		}
	}
	show_usage();
	return OK;
}
