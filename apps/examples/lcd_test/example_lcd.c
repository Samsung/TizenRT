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

#define LCD_DEV_PATH "/dev/lcd%d"
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

#ifdef CONFIG_EXAMPLE_LCD_FPS_TEST
#define EXAMPLE_LCD_FPS_TEST CONFIG_EXAMPLE_LCD_FPS_TEST
#else
#define EXAMPLE_LCD_FPS_TEST 5000
#endif

static void putarea(int x1, int x2, int y1, int y2, int color)
{
	struct lcddev_area_s area;
	char port[20] = { '\0' };
	int fd = 0;
	int p = 0;
	size_t len;
	len = xres * yres * 2 + 1;
	uint8_t *lcd_data = (uint8_t *)malloc(len);
	if (lcd_data == NULL) {
		printf("malloc failed for lcd data : %d\n", len);
		return;
	}
	sprintf(port, LCD_DEV_PATH, p);
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
	ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area);
	close(fd);
	free(lcd_data);
}

static void test_init(void)
{
	int ret;
	int fd = 0;
	int p = 0;
	char port[20] = { '\0' };
	sprintf(port, LCD_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	ioctl(fd, LCDDEVIO_INIT, &ret);
	close(fd);
}

static void test_orientation(void)
{
	int fd = 0;
	int p = 0;
	char port[20] = { '\0' };
	sprintf(port, LCD_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_RLANDSCAPE);

	test_put_run();

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
	ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_PORTRAIT);
#elif defined(CONFIG_LCD_LANDSCAPE)
	ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_LANDSCAPE);
#elif defined(CONFIG_LCD_RLANDSCAPE)
	ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_RLANDSCAPE);
#elif defined(CONFIG_LCD_RPORTRAIT)
	ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_RPORTRAIT);
#else
	ioctl(fd, LCDDEVIO_SETORIENTATION, LCD_LANDSCAPE);
#endif
	close(fd);
}

static void test_put_area_pattern(void)
{
	int fd = 0;
	int p = 0;
	char port[20] = { '\0' };
	sprintf(port, LCD_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	struct fb_videoinfo_s vinfo;
	ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo);
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
	int p = 0;
	char port[20] = { '\0' };
	struct lcddev_area_s area;
	size_t len;
	int idx = 0;
	sprintf(port, LCD_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	struct fb_videoinfo_s vinfo;
	ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo);
	xres = vinfo.xres;
	yres = vinfo.yres;
	len = xres * yres * 2 + 1;
        uint8_t *lcd_data = (uint8_t *)malloc(len);
        if (lcd_data == NULL) {
                printf("malloc failed for lcd data : %d\n", len);
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
	ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area);
	close(fd);
	free(lcd_data);
}

static void test_fps(void)
{
	int fd_rtc = 0;
	int fd_lcd = 0;
	int p = 0;
	char port[20] = { '\0' };
	size_t len;

	fd_rtc = open("/dev/rtc0", O_RDWR);
	if (fd_rtc < 0) {
		printf("ERROR: LCD FPS test, Fail to open rtc.\n");
		return;
	}

	len = xres * yres * 2 + 1;
	uint8_t *lcd_data_red = (uint8_t *)malloc(len);
	if (lcd_data_red == NULL) {
		printf("FPS TEST, malloc failed for lcd data red : %d\n", len);
		close(fd_rtc);
		return;
	}

	uint8_t *lcd_data_blue = (uint8_t *)malloc(len);
	if (lcd_data_blue == NULL) {
		printf("FPS TEST, malloc failed for lcd data blue: %d\n", len);
		free(lcd_data_red);
		close(fd_rtc);
		return;
	}
	for (int i = 0; i < len - 1; i += 2) {
		lcd_data_red[i] = (RED & 0xFF00) >> 8;
		lcd_data_red[i + 1] = RED & 0x00FF;
		lcd_data_blue[i] = (BLUE & 0xFF00) >> 8;
		lcd_data_blue[i + 1] = BLUE & 0x00FF;
	}

	struct lcddev_area_s area_red;
	struct lcddev_area_s area_blue;
	area_red.planeno = 0;
	area_red.row_start = 0;
	area_red.row_end = yres - 1;
	area_red.col_start = 0;
	area_red.col_end = xres - 1;
	area_red.stride = 2 * xres;
	area_red.data = lcd_data_red;
	area_blue.planeno = 0;
	area_blue.row_start = 0;
	area_blue.row_end = yres - 1;
	area_blue.col_start = 0;
	area_blue.col_end = xres - 1;
	area_blue.stride = 2 * xres;
	area_blue.data = lcd_data_blue;

	sprintf(port, LCD_DEV_PATH, p);
	fd_lcd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd_lcd < 0) {
		printf("ERROR: FPS TEST, Failed to open lcd port : %s error:%d\n", port, fd_lcd);
		free(lcd_data_red);
		free(lcd_data_blue);
		close(fd_rtc);
		return;
	}

	struct rtc_time start_time = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time end_time;

	bool is_red = true;
	//Start Test
	ioctl(fd_rtc, RTC_RD_TIME, (unsigned long)&start_time);
	for (int itr = 0; itr < EXAMPLE_LCD_FPS_TEST; itr++) {
		if (is_red) {
			ioctl(fd_lcd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area_red);
			is_red = false;
		} else {
			ioctl(fd_lcd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area_blue);
			is_red = true;
		}
	}
	ioctl(fd_rtc, RTC_RD_TIME, (unsigned long)&end_time);
	//End test
	
	close(fd_rtc);
	close(fd_lcd);
	free(lcd_data_red);
	free(lcd_data_blue);

	time_t start;
	time_t end;
	start = mktime((FAR struct tm *)&start_time);
	end = mktime((FAR struct tm *)&end_time);

	int time_elapsed = difftime(end, start);
	if (time_elapsed != 0) {
		float fps = EXAMPLE_LCD_FPS_TEST / time_elapsed;
		printf("FPS Test: %d frames executed in %d sec, FPS: %.2f\n", EXAMPLE_LCD_FPS_TEST, time_elapsed, fps);
		return;
	} else {
		printf("FPS calculation failed! Please increase the number of frames execution using CONFIG_EXAMPLE_LCD_FPS_TEST!\n");
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int lcd_test_main(int argc, char *argv[])
#endif
{
	printf("=== LCD demo ===\n");
	int count = 0;
	int fd = 0;
	int p = 0;
	char port[20] = { '\0' };
	sprintf(port, LCD_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	while (count < 5) {
		test_put_area_pattern();
		test_bit_map();
		sleep(3);
		ioctl(fd, LCDDEVIO_SETPOWER, 0);
		sleep(15);
		ioctl(fd, LCDDEVIO_SETPOWER, 100);
		count++;
		printf("count :%d\n", count);
	}
	test_fps();
	close(fd);
	return 0;
}
