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

#include <tinyara/config.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/araui_drv.h>
#include <araui/ui_asset.h>
#include <araui/ui_commons.h>
#include "ui_log.h"
#include "ui_commons_internal.h"
#include "ui_asset_internal.h"
#include "ui_core_internal.h"
#include "hal/ui_hal_interface.h"

#define RENDER_TASK_PRIO		125
//divide to 2 part for flushing, there isn't enough memory for buffer
#define RENDER_GRAN				2
#define UI_IMAGE_BUF_R(image, x, y)		((image->buf[image->bytes_per_line * y + 3 * x]) >> 3)
#define UI_IMAGE_BUF_G(image, x, y)		((((image->buf[image->bytes_per_line * y + 3 * x]) & 0b00000111) << 3) | ((image->buf[image->bytes_per_line * y + 3 * x  + 1] & 0b11100000) >> 5))
#define UI_IMAGE_BUF_B(image, x, y)		((image->buf[image->bytes_per_line * y + 3 * x + 1]) & 0b00011111)
#define UI_IMAGE_BUF_A(image, x, y)		(image->buf[image->bytes_per_line * y + 3 * x + 2])
#define UI_INTERPOLATION(color1, color2, weight)	(color1 * weight + color2 * (1 - weight))

#define DEBUG_RENDER_TIME		0

uint32_t _ui_hal_get_buf_color(ui_image_asset_body_t *body, int32_t x, int32_t y);
uint32_t _ui_hal_get_buf_interpolated_color(ui_image_asset_body_t *body, double x, double y);

typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
} area_t;

static uint16_t *g_framebuf;
static uint16_t *g_renderbuf;
static const int32_t FRAMEBUF_SIZE = (CONFIG_UI_DISPLAY_WIDTH * CONFIG_UI_DISPLAY_HEIGHT) * 2;
static pid_t g_tpid;
static sem_t g_mutex;
static sem_t g_notify;
static area_t g_rendering_area;
static ui_rect_t g_viewport = {0, };
static int g_ui_fd;

int ui_get_drvfd(void)
{
	return g_ui_fd;
}

static int ui_open_driver(void)
{
	g_ui_fd = open(ARAUI_DRVPATH, O_RDWR);
	if (g_ui_fd < 0) {
		UI_LOGE("Failed to open AraUI driver %d\n", errno);
		return ERROR;
	}
	return g_ui_fd;
}

static int hal_render_task(int argc, char *argv[]);

ui_error_t ui_hal_init(void)
{
	int fd;
	int ret;

	araui_drv_register();

	fd = ui_open_driver();
	if (fd < 0) {
		UI_LOGE("error: failed to open driver!\n");
		return UI_INIT_FAILURE;
	}

	ret = ioctl(fd, UIIOC_LCDINIT, UI_NULL);
	if (ret == ERROR) {
		UI_LOGE("error: failed to init LCD!\n");
		return UI_OPERATION_FAIL;
	}

	g_framebuf = (uint16_t *)UI_ALLOC(FRAMEBUF_SIZE);
	if (!g_framebuf) {
		UI_LOGE("error: cannot alloc the framebuffer!\n");
		return UI_INIT_FAILURE;
	}
	memset(g_framebuf, 0xFF, FRAMEBUF_SIZE);

	g_renderbuf = (uint16_t *)UI_ALLOC(FRAMEBUF_SIZE / RENDER_GRAN);
	if (!g_renderbuf) {
		UI_LOGE("error: cannot alloc the render buffer!\n");
		return UI_INIT_FAILURE;
	}

	sem_init(&g_mutex, 0, 1);
	sem_init(&g_notify, 0, 0);

	g_tpid = task_create("uifw_hal", RENDER_TASK_PRIO, 0x1000, hal_render_task, NULL);

	return UI_OK;
}

ui_error_t ui_hal_deinit(void)
{
	UI_FREE(g_framebuf);
	UI_FREE(g_renderbuf);

	sem_destroy(&g_mutex);
	sem_destroy(&g_notify);

	task_delete(g_tpid);

	return UI_OK;
}

static void color_cpy(uint16_t *d, uint16_t *s, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		*d++ = *s++;
	}
}

void ui_hal_redraw(int32_t x, int32_t y, int32_t width, int32_t height)
{
	int i;
	int j;
	int fr_offset;
	int render_offset;
	int idx;
	int block_h;
	int area_h;
	int area_size;
	area_t area[RENDER_GRAN];
	int block_size = FRAMEBUF_SIZE / (RENDER_GRAN * 2); //offset for int16_t, pixels for each render block

	if (x < 0 || x >= CONFIG_UI_DISPLAY_WIDTH || y < 0 || y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return;
	}

	if (x + width > CONFIG_UI_DISPLAY_WIDTH) {
		UI_LOGE("error: invalid width %d\n", width);
		width = CONFIG_UI_DISPLAY_WIDTH - x;
	}
	if (y + height > CONFIG_UI_DISPLAY_HEIGHT) {
		UI_LOGE("error: invalid height %d\n", height);
		height = CONFIG_UI_DISPLAY_HEIGHT - y;
	}

	area_h = height;
	idx = 0;
	block_h = block_size / width;
	while (area_h > 0) {
		area[idx].x1 = x;
		area[idx].x2 = x + width - 1;
		if (area_h >= block_h) {
			area[idx].y2 = y + height - 1 - (idx * block_h);
			area[idx].y1 = area[idx].y2 - block_h + 1;
		} else {
			area[idx].y1 = y;
			area[idx].y2 = y + area_h - 1;
		}

		idx++;
		area_h -= block_h;
	}

	fr_offset = 0;
	render_offset = 0;
	for (i = idx - 1; i >= 0; i--) {
		// flush the fixed 1/2 screen buff each time (full width, and 1/2 height)
		sem_wait(&g_mutex);
		//full width copy
		if (width == CONFIG_UI_DISPLAY_WIDTH) {
			fr_offset = width * area[i].y1;
			area_size = width * (area[i].y2 - area[i].y1 + 1);
			color_cpy(g_renderbuf, g_framebuf + fr_offset, area_size);
			fr_offset += area_size;
		} else {
			// copy buffer for each line
			for (j = area[i].y1; j <= area[i].y2; j++) {
				fr_offset = CONFIG_UI_DISPLAY_WIDTH * j + x;
				color_cpy(g_renderbuf + render_offset, g_framebuf + fr_offset, width);
				render_offset += width;
			}
		}
		sem_post(&g_mutex);

		g_rendering_area = area[i];
		sem_post(&g_notify);
	}
}

static int hal_render_task(int argc, char *argv[])
{
	int fd;
	int ret;
	ui_flush_area_t flush_area;

#if DEBUG_RENDER_TIME
	struct timespec before;
	struct timespec now;
	uint32_t dt;
	
	memset(&before, 0, sizeof(struct timespec));
	memset(&now, 0, sizeof(struct timespec));
#endif

	fd = ui_get_drvfd();
	if (fd < 0) {
		UI_LOGE("error: invalid driver path!\n");
		return 0;
	}

	while (1) {
		sem_wait(&g_notify);
		flush_area.x1 = g_rendering_area.x1;
		flush_area.x2 = g_rendering_area.x2;
		flush_area.y1 = g_rendering_area.y1;
		flush_area.y2 = g_rendering_area.y2;
		flush_area.color = g_renderbuf;

		sem_wait(&g_mutex); 
#if DEBUG_RENDER_TIME
		clock_gettime(CLOCK_MONOTONIC, &before);
#endif

		ret = ioctl(fd, UIIOC_LCDFLUSH, (int)&flush_area);
		if (ret == ERROR) {
			UI_LOGE("error: failed to flush LCD!\n");
		}

#if DEBUG_RENDER_TIME
		clock_gettime(CLOCK_MONOTONIC, &now);
		dt = ((now.tv_sec - before.tv_sec) * 1000) + ((now.tv_nsec - before.tv_nsec) / 1000000);
#endif
		sem_post(&g_mutex);
	}

	return 0;
}

ui_error_t ui_hal_clear(ui_color_t color)
{
	//unsigned long long uc = color & 0xFFFF;
	//uc = (uc << 48) | (uc << 32) | (uc << 16) | uc;

	int8_t *p = (int8_t*)g_framebuf;
	int size = 0;
	while (size < FRAMEBUF_SIZE) {
		*(int64_t*)p = 0;
		size += 8;
		p += 8;
	}

	return UI_OK;
}

inline void ui_hal_draw_pixel(int32_t x, int32_t y, ui_color_t color)
{
	uint32_t alpha;
	uint32_t bg;
	uint32_t fg;
	uint32_t offset;

	if (x < 0 || x >= CONFIG_UI_DISPLAY_WIDTH || y < 0 || y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return;
	}

	offset = (y * CONFIG_UI_DISPLAY_WIDTH + x);
	alpha = (color & 0xff);
	if (alpha == 0) {
		return;
	} else if (alpha == 0xff) {
		g_framebuf[offset] = color >> 8;
		return;
	}
	
	bg = g_framebuf[offset];
	// foreground color type will be r:5 g:6 b:5 a:8, extract color value
	fg = color >> 8;

	// blend with the fast algorithm
	alpha = (alpha + 4) >> 3;
	
	bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
	fg = (fg | (fg << 16)) & 0b00000111111000001111100000011111;
	uint32_t blend = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
	uint16_t result = (uint16_t)((blend >> 16) | blend);

	g_framebuf[offset] = result;
}

void ui_hal_draw_image(int32_t x, int32_t y, ui_asset_t image)
{
	ui_rect_t viewport = ui_hal_get_viewport();
	ui_rect_t image_rect = {0, };
	ui_rect_t image_crop_rect = {0, };
	int32_t image_offset_x = 0;
	int32_t image_offset_y = 0;
	int32_t idx_x = 0;
	int32_t idx_y = 0;
	uint32_t color = 0;

	ui_image_asset_body_t *body = (ui_image_asset_body_t *)image;

	if (!body || body->width == 0 || body->height == 0 || viewport.width == 0 || viewport.height == 0) {
		return;
	}

	if (!ui_image_asset_has_alpha(body->pixel_format)) {
		UI_LOGW("Sorry, now only support an image with alpha values!\n");
	}
	
	image_rect.x = x;
	image_rect.y = y;
	image_rect.width = body->width;
	image_rect.height = body->height;

	image_crop_rect = ui_rect_intersect(image_rect, viewport);
	if (image_crop_rect.width <= 0 || image_crop_rect.height <= 0) {
		return;
	}

	image_offset_y = (image_crop_rect.y - y);
	for (idx_y = image_crop_rect.y; idx_y < (image_crop_rect.y + image_crop_rect.height); idx_y++) {
		image_offset_x = (image_crop_rect.x - x);
		for (idx_x = image_crop_rect.x; idx_x < (image_crop_rect.x + image_crop_rect.width); idx_x++) {
			color = _ui_hal_get_buf_color(body, image_offset_x, image_offset_y);
			ui_hal_draw_pixel(idx_x, idx_y, color);
			image_offset_x++;
		}
		image_offset_y++;
	}
}

void ui_hal_draw_cropped_image(int32_t x, int32_t y, ui_asset_t image, ui_rect_t crop_rect)
{
	ui_rect_t viewport = ui_hal_get_viewport();
	ui_rect_t image_rect = {0, };
	ui_rect_t image_crop_rect = {0, };

	int32_t image_offset_x = 0;
	int32_t image_offset_y = 0;
	int32_t idx_x = 0;
	int32_t idx_y = 0;
	uint16_t rgb;
	uint8_t alpha;
	int offset;
	int row_offset;

	ui_image_asset_body_t *body = (ui_image_asset_body_t *)image;

	if (!body || body->width == 0 || body->height == 0 || viewport.width == 0 || viewport.height == 0) {
		return;
	}

	if (!ui_image_asset_has_alpha(body->pixel_format)) {
		UI_LOGW("Sorry, now only support an image with alpha values!\n");
	}

	image_rect.x = x;
	image_rect.y = y;
	image_rect.width = crop_rect.width;
	image_rect.height = crop_rect.height;

	image_crop_rect = ui_rect_intersect(image_rect, viewport);
	if (image_crop_rect.width <= 0 || image_crop_rect.height <= 0) {
		return;
	}

	image_offset_y = (image_crop_rect.y - y + crop_rect.y);
	image_offset_x = (image_crop_rect.x - x + crop_rect.x) * 3;
	row_offset = body->bytes_per_line * image_offset_y + image_offset_x;
	for (idx_y = 0; idx_y < image_crop_rect.height; idx_y++) {		
		offset = row_offset;
		for (idx_x = 0; idx_x < image_crop_rect.width; idx_x++) {
			rgb = ((uint16_t)body->buf[offset] << 8) | body->buf[offset+1];
			alpha = body->buf[offset+2];
			ui_hal_draw_pixel(image_crop_rect.x + idx_x, image_crop_rect.y + idx_y, UI_COLOR_ALPHA(rgb, alpha));
			offset += 3;
		}
		row_offset += body->bytes_per_line;
	}
}

void ui_hal_draw_cropped_resized_image(int32_t x, int32_t y, ui_asset_t image, ui_rect_t crop_rect, int32_t resized_width, int32_t resized_height)
{
	ui_rect_t viewport = ui_hal_get_viewport();
	ui_rect_t image_rect = {0, };
	ui_rect_t image_crop_rect = {0, };
	int32_t draw_x = 0;
	int32_t draw_y = 0;
	uint32_t color = 0;
	double scale_x;
	double scale_y;
	double scaled_x;
	double scaled_y;
	double row_x;

	ui_image_asset_body_t *body = (ui_image_asset_body_t *)image;

	if ((resized_width == crop_rect.width) && (resized_height == crop_rect.height)) {
		ui_hal_draw_cropped_image(x, y, image, crop_rect);
		return;
	}

	if (!body || body->width == 0 || body->height == 0 || viewport.width == 0 || viewport.height == 0) {
		return;
	}

	if (!ui_image_asset_has_alpha(body->pixel_format)) {
		UI_LOGW("Sorry, now only support an image with alpha values!\n");
	}

	scale_x = (double)crop_rect.width / (double)resized_width;
	scale_y = (double)crop_rect.height / (double)resized_height;

	image_rect.x = x;
	image_rect.y = y;
	image_rect.width = resized_width;
	image_rect.height = resized_height;

	image_crop_rect = ui_rect_intersect(image_rect, viewport);
	if (image_crop_rect.width <= 0 || image_crop_rect.height <= 0) {
		return;
	}

	scaled_y = (image_crop_rect.y - y) * scale_y + crop_rect.y;
	row_x = (double)(image_crop_rect.x - x) * scale_x + crop_rect.x;
	for (draw_y = 0; draw_y < image_crop_rect.height; draw_y++) {
		
		scaled_x = row_x;
		for (draw_x = 0; draw_x < image_crop_rect.width; draw_x++) {
			if ((fmod(scaled_x, 1.0) == 0.0) && ((fmod(scaled_y, 1.0) == 0.0))) {
				color = _ui_hal_get_buf_color(body, (int32_t)scaled_x, (int32_t)scaled_y);
			} else {
				color = _ui_hal_get_buf_interpolated_color(body, scaled_x, scaled_y);
			}
			ui_hal_draw_pixel(image_crop_rect.x + draw_x, image_crop_rect.y + draw_y, color);
			scaled_x += scale_x;
		}
		scaled_y += scale_y;
	}
}

uint32_t _ui_hal_get_buf_color(ui_image_asset_body_t *body, int32_t x, int32_t y)
{
	uint32_t color;

	color = ((uint32_t)UI_IMAGE_BUF_R(body, x, y) << 19);
	color |= ((uint32_t)UI_IMAGE_BUF_G(body, x, y) << 13);
	color |= ((uint32_t)UI_IMAGE_BUF_B(body, x, y) << 8);
	color |= (uint32_t)UI_IMAGE_BUF_A(body, x, y);

	return color;
}

uint32_t _ui_hal_get_buf_interpolated_color(ui_image_asset_body_t *image, double x, double y)
{
	int32_t lower_x;
	int32_t lower_y;
	int32_t upper_x;
	int32_t upper_y;
	double diff_x;
	double diff_y;
	double r1;
	double r2;
	double g1;
	double g2;
	double b1;
	double b2;
	double a1;
	double a2;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	uint32_t color;

	lower_x = (int32_t)floor(x);
	lower_y = (int32_t)floor(y);

	upper_x = lower_x + 1;
	if (upper_x >= image->width) {
		upper_x--;
		diff_x = 1.0;
	} else {
		diff_x = upper_x - x;
	}
	
	upper_y = lower_y + 1;
	if (upper_y >= image->height) {
		upper_y--;
		diff_y = 1.0;
	} else {
		diff_y = upper_y - y;
	}

	r1 = UI_INTERPOLATION(UI_IMAGE_BUF_R(image, lower_x, lower_y), UI_IMAGE_BUF_R(image, upper_x, lower_y), diff_x);
	r2 = UI_INTERPOLATION(UI_IMAGE_BUF_R(image, lower_x, upper_y), UI_IMAGE_BUF_R(image, upper_x, upper_y), diff_x);
	r = (uint8_t)(UI_INTERPOLATION(r1, r2, diff_y));

	g1 = UI_INTERPOLATION(UI_IMAGE_BUF_G(image, lower_x, lower_y), UI_IMAGE_BUF_G(image, upper_x, lower_y), diff_x);
	g2 = UI_INTERPOLATION(UI_IMAGE_BUF_G(image, lower_x, upper_y), UI_IMAGE_BUF_G(image, upper_x, upper_y), diff_x);
	g = (uint8_t)(UI_INTERPOLATION(g1, g2, diff_y));

	b1 = UI_INTERPOLATION(UI_IMAGE_BUF_B(image, lower_x, lower_y), UI_IMAGE_BUF_B(image, upper_x, lower_y), diff_x);
	b2 = UI_INTERPOLATION(UI_IMAGE_BUF_B(image, lower_x, upper_y), UI_IMAGE_BUF_B(image, upper_x, upper_y), diff_x);
	b = (uint8_t)(UI_INTERPOLATION(b1, b2, diff_y));

	a1 = UI_INTERPOLATION(UI_IMAGE_BUF_A(image, lower_x, lower_y), UI_IMAGE_BUF_A(image, upper_x, lower_y), diff_x);
	a2 = UI_INTERPOLATION(UI_IMAGE_BUF_A(image, lower_x, upper_y), UI_IMAGE_BUF_A(image, upper_x, upper_y), diff_x);
	a = (uint8_t)(UI_INTERPOLATION(a1, a2, diff_y));

	color = (r << 19) | (g << 13) | (b << 8) | a;

	return color;
}

ui_error_t ui_hal_set_viewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	g_viewport.x = x;
	g_viewport.y = y;
	g_viewport.width = width;
	g_viewport.height = height;

	return UI_OK;
}

ui_rect_t ui_hal_get_viewport(void)
{
	return g_viewport;
}

#if defined(CONFIG_UI_TOUCH_INTERFACE)
static void _ui_hal_enqueue_touch_event(ui_touch_event_t type, int32_t x, int32_t y)
{
	static ui_touch_event_t prev_touch_event = UI_TOUCH_EVENT_NONE;
	static bool touch_press = false;
	static int enqueue_idx = -1; // This is because enqueue index always increase by 1 at the first time.

	if (type != prev_touch_event) {
		enqueue_idx = (enqueue_idx + 1) % UI_MAX_TOUCH_QUEUE_SIZE;
	}

	switch (type) {
	case UI_TOUCH_EVENT_MOVE:
		break;
	case UI_TOUCH_EVENT_DOWN:
		touch_press = true;
		break;
	case UI_TOUCH_EVENT_UP:
		touch_press = false;
		break;
	default:
		UI_LOGE("error: invalid touch event!\n");
		return;
	}
	g_touch_queue[enqueue_idx].touch_press = touch_press;
	g_touch_queue[enqueue_idx].event = type;
	g_touch_queue[enqueue_idx].x = x;
	g_touch_queue[enqueue_idx].y = y;

	prev_touch_event = type;
}

bool ui_hal_get_touch(bool *pressed, ui_coord_t *coord)
{
	static ui_touch_event_t prev_touch_event = UI_TOUCH_EVENT_NONE;
	static int dequeue_idx = 0;
	int trial_idx;

	if (g_touch_queue[dequeue_idx].touch_press == -1) {
		// If a previous event was MOVE event, there is a possibility that the next queue is occupied.
		if (prev_touch_event == UI_TOUCH_EVENT_MOVE) {
			// Let's check the next queue
			trial_idx = (dequeue_idx + 1) % UI_MAX_TOUCH_QUEUE_SIZE;
			if (g_touch_queue[trial_idx].touch_press != -1) {
				// Yes, the next queue is occupied.
				dequeue_idx = trial_idx;
			} else {
				// Sorry, the next queue is also not occupied.
				return false;
			}
		} else {
			// If a previous event was not MOVE event, there is no possibility.
			return false;
		}
	}

	*pressed = g_touch_queue[dequeue_idx].touch_press;
	coord->x = g_touch_queue[dequeue_idx].x;
	coord->y = g_touch_queue[dequeue_idx].y;
	prev_touch_event = g_touch_queue[dequeue_idx].event;
	g_touch_queue[dequeue_idx].touch_press = -1;

	// If the touch event type of dequeued event is not MOVE event, the dequeue index must increase.
	if (g_touch_queue[dequeue_idx].event != UI_TOUCH_EVENT_MOVE) {
		dequeue_idx = (dequeue_idx + 1) % UI_MAX_TOUCH_QUEUE_SIZE;
	}

	return true;
}
#endif //CONFIG_UI_TOUCH_INTERFACE

ui_color_t ui_hal_get_pixel_color(int32_t x, int32_t y)
{
	int32_t offset;

	offset = CONFIG_UI_DISPLAY_WIDTH * y + x;

	return (ui_color_t)g_framebuf[offset];
}
