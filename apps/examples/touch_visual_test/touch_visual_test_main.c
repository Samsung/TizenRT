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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include "touch_map.h"
#if !defined(CONFIG_DISABLE_POLL)
#include <poll.h>
#endif

#include <tinyara/input/touchscreen.h>
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/rtc.h>

#define LCD_DEV_PATH "/dev/lcd%d"

#if defined(CONFIG_TOUCH_POLL)
static bool g_terminated;
#endif

#if defined(CONFIG_TOUCH_CALLBACK)
static struct touch_set_callback_s touch_set_callback;
#endif

//global variables
static int xres;
static int yres;
uint8_t *g_buffer;
hashtable_t g_table;

// Function to create a new pair
static pair_t create_pair(int first, int second)
{
	pair_t pair;
	pair.first = first;
	pair.second = second;
	return pair;
}

// Function to create a new linked list node
static node_t* create_node(pair_t data)
{
	node_t* node = (node_t* )malloc(sizeof(node_t));
	if (node == NULL) {
		printf("malloc failed for node\n");
		return NULL;
	}
	node->data = data;
	node->next = NULL;
	return node;
}

// Hash function to calculate the index for a given key
static int hash_function(int key)
{
	return key % TABLE_SIZE;
}

// Function to insert a pair into the hash table
static void insert_pair(hashtable_t* table, int key, pair_t value)
{
	int index = hash_function(key);
	node_t* node = create_node(value);
	node->next = table->buckets[index];
	table->buckets[index] = node;
}

// Function to retrieve the vector of pairs for a given key
static node_t* get_pairs(hashtable_t* table, int key)
{
	int index = hash_function(key);
	return table->buckets[index];
}

// Function to delete  pairs of a particular key from the hash table
static void delete_pairs(hashtable_t* table, int key)
{
	int index = hash_function(key);
	node_t* current = table->buckets[index];

	while (current != NULL) {
		node_t* temp = current;
		current = current->next;
		free(temp);
	}
	table->buckets[index] = NULL;
}

//Function clear the map and free all nodes in the hash table
static void clear_map(hashtable_t* table)
{
	for (int i = 0; i < TABLE_SIZE; i++) {
		node_t* current = table->buckets[i];
		while (current != NULL) {
			node_t* temp = current;
			current = current->next;
			free(temp);
		}
		table->buckets[i] = NULL;
	}
}

// Function to check if a pair exists within a certain range of coordinates for a given key in the hash table
static int find_pair_in_range(hashtable_t* table, int key, int x, int y)
{
	int index = hash_function(key);
	node_t* current = table->buckets[index];

	while (current != NULL) {
		if ((abs(current->data.first - x) <= 30) && (abs(current->data.second - y) <= 30)) {
			return 1; // Pair found within the range
		}
		current = current->next;
	}
	return 0;
}


static void putarea(int x1, int x2, int y1, int y2)
{
	struct lcddev_area_s area;
	char port[20] = { '\0' };
	int fd = 0;
	int p = 0;
	sprintf(port, LCD_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
		return;
	}
	area.planeno = 0;
	area.row_start = x1;
	area.row_end = x2;
	area.col_start = y1;
	area.col_end = y2;
	area.stride = 2 * xres;
	area.data = g_buffer;
	ioctl(fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&area);
	close(fd);
}

static void set_pixel(int x, int y, int width, uint16_t color)
{
	int index = 2 * (y * width + x);
	g_buffer[index + 1] = (color & 0xFF00) >> 8;
	g_buffer[index] = (color & 0x00FF);
}

static uint16_t invert_colour(int is_draw)
{
	if (is_draw == 1) {
		return 65535;
	} else {
		return 0;
	 }
}

static void draw_circle(int x_cord, int y_cord, int is_draw)
{
#if defined(CONFIG_LCD_SW_ROTATION)
	int x0 = xres - x_cord;
	int y0 = yres - y_cord;
#else
	int x0 = y_cord;
	int y0 = yres - x_cord;
#endif
	int width = xres;
	int height = yres;
	int radius = 50;
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y) {
		if (x0 + x < width && y0 + y < height) {
			set_pixel(x0 + x, y0 + y, width, invert_colour(is_draw));
		}
		if (x0 + y < width && y0 + x < height) {
			set_pixel(x0 + y, y0 + x, width, invert_colour(is_draw));
		}
		if (x0 - y >= 0 && y0 + x < height) {
			set_pixel(x0 - y, y0 + x, width, invert_colour(is_draw));
		}
		if (x0 - x >= 0 && y0 + y < height) {
			set_pixel(x0 - x, y0 + y, width, invert_colour(is_draw));
		}
		if (x0 - x >= 0 && y0 - y >= 0) {
			set_pixel(x0 - x, y0 - y, width, invert_colour(is_draw));
		}
		if (x0 - y >= 0 && y0 - x >= 0) {
			set_pixel(x0 - y, y0 - x, width, invert_colour(is_draw));
		}
		if (x0 + y < width && y0 - x >= 0) {
			set_pixel(x0 + y, y0 - x, width, invert_colour(is_draw));
		}
		if (x0 + x < width && y0 - y >= 0) {
			set_pixel(x0 + x, y0 - y, width, invert_colour(is_draw));
		}
		y += 1;
		err += 1 + 2 * y;
		if (2 * (err - x) + 1 > 0) {
			x -= 1;
			err += 1 - 2 * x;
		}
	}
}

#if defined(CONFIG_TOUCH_POLL)
static void touch_test(void)
{
	/* read first 10 events */
	int ret;
	int cnt = 0;//to skip the hold events
	int touch_status = 0 ;// to check if touch is active
	g_buffer = (uint8_t *)malloc(2 * xres * yres + 1);
	if (g_buffer == NULL) {
		printf("malloc failed for g_buffer\n");
		return;
	}
	memset(g_buffer, 0, 2 * xres * yres + 1);
	for (int i = 0; i < TABLE_SIZE; i++) {
		g_table.buckets[i] = NULL;
	}
	int fd = open(TOUCH_DEV_PATH, O_RDONLY);
	if (fd < 0) {
		free(g_buffer);
		printf("Error: Failed to open /dev/input0, errno : %d\n", get_errno());
		return;
	}

	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;

	struct touch_sample_s buf;
	while (!g_terminated) {
		ret = poll(fds, 1, 5000);
		if (ret == ERROR) {
			printf("Polling Failed : %d\n", get_errno());
			break;
		}
		if (fds[0].revents & POLLIN) {
			touch_status = 1;
			ret = read(fd, &buf, sizeof(struct touch_sample_s));
			if (ret != ERROR) {
				for (int i = 0; i < buf.npoints; i++) {
					if (buf.point[i].flags == TOUCH_DOWN) {
						insert_pair(&g_table, buf.point[i].id, create_pair(buf.point[i].x, buf.point[i].y));
						draw_circle(buf.point[i].x, buf.point[i].y, 1);
						putarea(0, yres - 1, 0, xres - 1);
					} else if (buf.point[i].flags == TOUCH_MOVE) {
						if (cnt == 0) {
							if (!find_pair_in_range(&g_table, buf.point[i].id, buf.point[i].x, buf.point[i].y)) {
								insert_pair(&g_table, buf.point[i].id, create_pair(buf.point[i].x, buf.point[i].y));
								draw_circle(buf.point[i].x, buf.point[i].y, 1);
								putarea(0, yres - 1, 0, xres - 1);
								cnt++;
							}
						} else if (cnt < CONFIG_TOUCH_HOLD_EVENT_SKIP) {
							cnt++;
						} else {
							cnt = 0;
						}
					} else if (buf.point[i].flags == TOUCH_UP) {
						node_t* pairs = get_pairs(&g_table, buf.point[i].id);
						while (pairs != NULL) {
							draw_circle(pairs->data.first, pairs->data.second, 0);
							pairs = pairs->next;
						}
						putarea(0, yres - 1, 0, xres - 1);
						delete_pairs(&g_table, buf.point[i].id);
					}
				}
			}
		} else {
			if (touch_status) {
				touch_status = 0;
				clear_map(&g_table);
				memset(g_buffer, 0, 2 * xres * yres + 1);
				putarea(0, yres - 1, 0, xres - 1);
			}
		}
	}
	close(fd);
	clear_map(&g_table);
	memset(g_buffer, 0, 2 * xres * yres + 1);
	putarea(0, yres - 1, 0, xres - 1);
	free(g_buffer);
}
#endif

static int touch_visual_test_start(void)
{
#if defined(CONFIG_TOUCH_POLL)
	int touch = task_create("touch", SCHED_PRIORITY_DEFAULT, 8096, (main_t)touch_test, NULL);
	if (touch < 0) {
		printf("Error: Failed to create touch reader, error : %d\n", get_errno());
		return ERROR;
	}
	g_terminated = false;
	return OK;
#endif
}

static int touch_visual_test_stop(void)
{
#if defined(CONFIG_TOUCH_POLL)
	g_terminated = true;
#endif
	return OK;
}

static void show_usage(void)
{
	printf("usage: touch_visual_test <command #>\n");
	printf("Excute touchscreen visual testing.\n\n");
	printf("    start: Start the touch visual test \n");
	printf("    stop : Stop  the touch visual test\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int touch_visual_test_main(int argc, char *argv[])
#endif
{
	if (argc <= 1 || !strncmp(argv[1], "-h", 2) || !strncmp(argv[1], "--help", 6)) {
		show_usage();
		return OK;
	}
	if (argc == 2) {
		if (!strncmp(argv[1], "start", 5)) {
			printf("==TOUCH Visual Test Start==\n");
			int fd = 0;
			int p = 0;
			char port[20] = { '\0' };
			sprintf(port, LCD_DEV_PATH, p);
			fd = open(port, O_RDWR | O_SYNC, 0666);
			if (fd < 0) {
				printf("ERROR: Failed to open lcd port : %s error:%d\n", port, fd);
				return ERROR;
			}
			struct fb_videoinfo_s vinfo;
			ioctl(fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo);
			xres = vinfo.xres;
			yres = vinfo.yres;
			close(fd);
			return touch_visual_test_start();
		} else if (!strncmp(argv[1], "stop", 4)) {
			printf("== TOUCH Visual Test Stop ==\n");
			return touch_visual_test_stop();
		}
	}
}

