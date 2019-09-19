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
#include <pthread.h>
#include <SDL2/SDL.h>

//!< AraUI Public
#include <araui/ui_asset.h>
#include <araui/ui_commons.h>

//!< AraUI Internal
#include "ui_debug.h"
#include "ui_commons_internal.h"
#include "ui_asset_internal.h"
#include "ui_core_internal.h"
#include "dal/ui_dal.h"

//!< Local
#include "dal_sdl.h"

/****************************************************************************
 * Macros
 ****************************************************************************/
#define FRONT_PAGE     (0)
#define BACK_PAGE      (1)
#define FB_SIZE        (CONFIG_UI_DISPLAY_WIDTH * CONFIG_UI_DISPLAY_HEIGHT * 3)

#define UI_MAX_TOUCH_QUEUE_SIZE (32)

/****************************************************************************
 * Private Types
 ****************************************************************************/
typedef struct {
	int touch_press;
	ui_touch_event_t event;
	int32_t x;
	int32_t y;
} ui_touch_queue_t;

/****************************************************************************
 * Private Functions Declaration
 ****************************************************************************/
static void _enqueue_touch_event(ui_touch_event_t type, int32_t x, int32_t y);

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static SDL_Window          *g_window = NULL;
static SDL_Surface         *g_screenSurface = NULL;
static SDL_Event            g_event;
static uint8_t             *g_fb[2];
static pthread_mutex_t      g_mutex;
static ui_rect_t            g_viewport = {0, };
static ui_touch_queue_t     g_touch_queue[UI_MAX_TOUCH_QUEUE_SIZE];
static void               (*g_hotkey_cb[2])(void);

/****************************************************************************
 * DAL Interface Implementation
 ****************************************************************************/
UI_DAL ui_error_t ui_dal_init(void)
{
	int queue_idx;

	g_fb[FRONT_PAGE] = (uint8_t *)UI_ALLOC(FB_SIZE);
	if (!g_fb[FRONT_PAGE]) {
		UI_LOGE("error: cannot alloc the framebuffer!\n");
		return UI_INIT_FAILURE;
	}

	g_fb[BACK_PAGE] = (uint8_t *)UI_ALLOC(FB_SIZE);
	if (!g_fb[BACK_PAGE]) {
		UI_LOGE("error: cannot alloc the framebuffer!\n");
		return UI_INIT_FAILURE;
	}

	pthread_mutex_init(&g_mutex, NULL);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return UI_INIT_FAILURE;
	}

	g_window = SDL_CreateWindow("ui_sim",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		(CONFIG_UI_DISPLAY_WIDTH) * (CONFIG_UI_DISPLAY_SCALE),
		(CONFIG_UI_DISPLAY_HEIGHT) * (CONFIG_UI_DISPLAY_SCALE),
		SDL_WINDOW_SHOWN);

	if (g_window == NULL) {
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		SDL_Quit();
		return UI_INIT_FAILURE;
	}

	g_screenSurface = SDL_GetWindowSurface(g_window);

	for (queue_idx = 0; queue_idx < UI_MAX_TOUCH_QUEUE_SIZE; queue_idx++) {
		g_touch_queue[queue_idx].touch_press = -1;
	}

	return UI_OK;
}

UI_DAL ui_error_t ui_dal_deinit(void)
{
	UI_FREE(g_fb[FRONT_PAGE]);
	UI_FREE(g_fb[BACK_PAGE]);

	pthread_mutex_destroy(&g_mutex);

	SDL_DestroyWindow(g_window);
	SDL_Quit();

	return UI_OK;
}

UI_DAL void ui_dal_redraw(int32_t x, int32_t y, int32_t width, int32_t height)
{
	int32_t offset;
	int i;
	int j;

	pthread_mutex_lock(&g_mutex);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			// todo: Enhance the logic
			offset = (y + i) * CONFIG_UI_DISPLAY_WIDTH * 3 + (x + j) * 3;
			g_fb[FRONT_PAGE][offset] = g_fb[BACK_PAGE][offset];
			g_fb[FRONT_PAGE][offset + 1] = g_fb[BACK_PAGE][offset + 1];
			g_fb[FRONT_PAGE][offset + 2] = g_fb[BACK_PAGE][offset + 2];
		}
	}
	pthread_mutex_unlock(&g_mutex);
}

UI_DAL void ui_dal_clear(void)
{
	memset(g_fb[BACK_PAGE], 0, FB_SIZE);
}

inline UI_DAL void ui_dal_put_pixel_rgba8888(int32_t x, int32_t y, ui_color_t color)
{
	ui_color_rgba8888_t *fg;
	ui_color_rgb888_t *bg;

	if (x < 0 || x >= CONFIG_UI_DISPLAY_WIDTH || y < 0 || y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return;
	}

	fg = (ui_color_rgba8888_t *)&color;
	bg = (ui_color_rgb888_t *)&g_fb[BACK_PAGE][(y * CONFIG_UI_DISPLAY_WIDTH + x) * 3];

	bg->r = ((fg->r * fg->a) + (bg->r * (255 - fg->a))) / 255;
	bg->g = ((fg->g * fg->a) + (bg->g * (255 - fg->a))) / 255;
	bg->b = ((fg->b * fg->a) + (bg->b * (255 - fg->a))) / 255;
}

inline UI_DAL void ui_dal_put_pixel_rgb888(int32_t x, int32_t y, ui_color_t color)
{
	ui_color_rgb888_t *fg;
	ui_color_rgb888_t *bg;

	if (x < 0 || x >= CONFIG_UI_DISPLAY_WIDTH || y < 0 || y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return;
	}

	fg = (ui_color_rgb888_t *)&color;
	bg = (ui_color_rgb888_t *)&g_fb[BACK_PAGE][(y * CONFIG_UI_DISPLAY_WIDTH + x) * 3];
	bg->r = fg->r;
	bg->g = fg->g;
	bg->b = fg->b;
}

UI_DAL ui_error_t ui_dal_set_viewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	g_viewport.x = x;
	g_viewport.y = y;
	g_viewport.width = width;
	g_viewport.height = height;

	return UI_OK;
}

UI_DAL ui_rect_t ui_dal_get_viewport(void)
{
	return g_viewport;
}

UI_DAL bool ui_dal_get_touch(bool *pressed, ui_coord_t *coord)
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

/****************************************************************************
 * Private Functions Implementation
 ****************************************************************************/
void set_key1_callback(void (*cb)(void))
{
	g_hotkey_cb[0] = cb;
}

void set_key2_callback(void (*cb)(void))
{
	g_hotkey_cb[1] = cb;
}

void sdl_loop(void)
{
	int i, j;

	while (SDL_PollEvent(&g_event) >= 0) {
		switch (g_event.type) {
		case SDL_KEYDOWN: {
			// todo
		}
		break;

		case SDL_KEYUP: {
			// todo
		}
		break;

		case SDL_MOUSEBUTTONDOWN: {
			if (g_event.motion.x > 457 && g_event.motion.y > 112 &&
				g_event.motion.x < (457 + 50) && g_event.motion.y < (112 + 85)) {

				if (g_hotkey_cb[0]) {
					g_hotkey_cb[0]();
				}

			}

			if (g_event.motion.x > 457 && g_event.motion.y > 343 &&
				g_event.motion.x < (457 + 50) && g_event.motion.y < (343 + 85)) {

				if (g_hotkey_cb[1]) {
					g_hotkey_cb[1]();
				}

			}

			_enqueue_touch_event(
				UI_TOUCH_EVENT_DOWN,
				(g_event.motion.x - 90) / CONFIG_UI_DISPLAY_SCALE,
				(g_event.motion.y - 90) / CONFIG_UI_DISPLAY_SCALE);
		}
		break;

		case SDL_MOUSEMOTION: {
			_enqueue_touch_event(
				UI_TOUCH_EVENT_MOVE,
				(g_event.motion.x - 90) / CONFIG_UI_DISPLAY_SCALE,
				(g_event.motion.y - 90) / CONFIG_UI_DISPLAY_SCALE);
		}
		break;

		case SDL_MOUSEBUTTONUP: {
			_enqueue_touch_event(
				UI_TOUCH_EVENT_UP,
				(g_event.motion.x - 90) / CONFIG_UI_DISPLAY_SCALE,
				(g_event.motion.y - 90) / CONFIG_UI_DISPLAY_SCALE);
		}
		break;

		case SDL_QUIT: {
			return;
		}
		break;
		}

		memset(&g_event, 0, sizeof(g_event));

		pthread_mutex_lock(&g_mutex);

		SDL_FillRect(g_screenSurface, NULL, SDL_MapRGB(g_screenSurface->format, 0x00, 0x00, 0x00));

		for (i = 0; i < CONFIG_UI_DISPLAY_HEIGHT; i++) {
			for (j = 0; j < CONFIG_UI_DISPLAY_WIDTH; j++) {
				uint8_t r = g_fb[FRONT_PAGE][((CONFIG_UI_DISPLAY_WIDTH * i + j) * 3)];
				uint8_t g = g_fb[FRONT_PAGE][((CONFIG_UI_DISPLAY_WIDTH * i + j) * 3) + 1];
				uint8_t b = g_fb[FRONT_PAGE][((CONFIG_UI_DISPLAY_WIDTH * i + j) * 3) + 2];

				SDL_Rect rect = {
					((j) * CONFIG_UI_DISPLAY_SCALE),
					((i) * CONFIG_UI_DISPLAY_SCALE),
					(CONFIG_UI_DISPLAY_SCALE),
					(CONFIG_UI_DISPLAY_SCALE)
				};
				SDL_FillRect(g_screenSurface, &rect, SDL_MapRGB(g_screenSurface->format, r, g, b));
			}
		}

		SDL_UpdateWindowSurface(g_window);

		pthread_mutex_unlock(&g_mutex);

		usleep(1000);
	}
}

static void _enqueue_touch_event(ui_touch_event_t type, int32_t x, int32_t y)
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
		printf("error: invalid touch event!\n");
		return;
	}
	g_touch_queue[enqueue_idx].touch_press = touch_press;
	g_touch_queue[enqueue_idx].event = type;
	g_touch_queue[enqueue_idx].x = x;
	g_touch_queue[enqueue_idx].y = y;

	prev_touch_event = type;
}
