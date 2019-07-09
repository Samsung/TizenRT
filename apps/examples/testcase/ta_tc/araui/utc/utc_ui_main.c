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
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <araui/ui_asset.h>
#include <araui/ui_commons.h>
#include <araui/ui_core.h>
#include <araui/ui_view.h>
#include <araui/ui_widget.h>
#include <sys/time.h>
#include <unistd.h>
#include "tc_common.h"

/****************************************************************************
 * Macros for UI UTC
 ****************************************************************************/

#define UTC_UI_DEFAULT_X               0
#define UTC_UI_DEFAULT_Y               0
#define UTC_UI_DEFAULT_WIDTH           4
#define UTC_UI_DEFAULT_HEIGHT          4
#define UTC_UI_DEFAULT_CENTER          5
#define UTC_UI_DEFAULT_RADIUS          4
#define UTC_UI_DEFAULT_RADIAN          (3.14 / 4)
#define UTC_UI_DEFAULT_COLOR           0xffff
#define UTC_UI_DEFAULT_INTERVAL        100
#define UTC_UI_WORD_WARP_WIDTH         35

#if defined(SIM_TC)
#define UTC_UI_DEFAULT_IMAGE_PATH      "assets/utc_ui_image.png"
#define UTC_UI_DEFAULT_IMAGE_PATH_RGB  "assets/utc_ui_image_rgb.png"
#define UTC_UI_DEFAULT_FONT_PATH       "assets/utc_ui_font.fnt"
#else
#define UTC_UI_DEFAULT_IMAGE_PATH      "/rom/utc_ui_image.png"
#define UTC_UI_DEFAULT_IMAGE_PATH_RGB  "/rom/utc_ui_image_rgb.png"
#define UTC_UI_DEFAULT_FONT_PATH       "/rom/utc_ui_font.fnt"
#endif // SIM_TC

#define UTC_UI_DEFAULT_CONTENT_SIZE_X  10
#define UTC_UI_DEFAULT_CONTENT_SIZE_Y  10
#define UTC_UI_MOVED_X                 10
#define UTC_UI_MOVED_Y                 10
#define UTC_UI_FONT_HEIGHT             12
#define UTC_UI_TEXT                    "UI가\n"
#define UTC_UI_U_WIDTH                 22
#define UTC_UI_I_WIDTH                 10
#define UTC_UI_GA_WIDTH                28
#define UTC_UI_INVALID_VIEW            UI_NULL
#define UTC_UI_INVALID_WIDGET          UI_NULL
#define UTC_UI_INVALID_IMAGE           UI_NULL
#define UTC_UI_INVALID_FONT            UI_NULL
#define UTC_UI_INVALID_TYPE            -1
#define UTC_UI_INVALID_PATH            NULL
#define UTC_UI_INVALID_FONT_EXTENSION  ".ttf"
#define UTC_UI_INVALID_COLOR           -1
#define UTC_UI_INVALID_X               -1
#define UTC_UI_INVALID_Y               -1
#define UTC_UI_INVALID_RADIUS          -1
#define UTC_UI_INVALID_RADIAN          -3.14
#define UTC_UI_NEGATIVE_OFFSET         -1
#define UTC_UI_POSITIVE_OFFSET         (UTC_UI_DEFAULT_CONTENT_SIZE_X + 1)

#define UTC_UI_WAIT_CALLBACK(timeout) \
	do { \
		gettimeofday(&g_curtime, NULL); \
		g_timeout.tv_sec = g_curtime.tv_sec + timeout; \
		g_timeout.tv_nsec = g_curtime.tv_usec * 1000; \
		g_ret = pthread_cond_timedwait(&g_callback_cond, &g_callback_lock, &g_timeout); \
	} while (0)

#define UTC_UI_CALLBACK_CALLED \
	do { \
		pthread_cond_signal(&g_callback_cond); \
	} while (0)

/****************************************************************************
 * Static variables for UI UTC
 ****************************************************************************/

static const uint8_t g_utc_ui_image[104] = {
	0x00, 0x00, 0x00, 0x00, // uint32_t id
	0x04, 0x00, 0x00, 0x00, // int32_t width
	0x04, 0x00, 0x00, 0x00, // int32_t height
	0x08, 0x00, 0x00, 0x00, // ui_pixel_format_t pf
	0x38, 0x00, 0x00, 0x00, // uint32_t header_size
	0x30, 0x00, 0x00, 0x00, // uint32_t data_size
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[0]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[1]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[2]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[3]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[4]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[5]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[6]
	0x00, 0x00, 0x00, 0x00, // int32_t reserved[7]
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

ui_asset_t g_utc_ui_font_asset;
ui_asset_t g_utc_ui_image_asset;

ui_widget_t g_root;

static pthread_mutex_t g_callback_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_callback_cond;
static const ui_rect_t UTC_UI_NULL_RECT = {0, };

int g_ret;
struct timeval g_curtime;
struct timespec g_timeout;

/****************************************************************************
 * Callbacks for UTC
 ****************************************************************************/

static void utc_ui_created_cb(ui_view_t view)
{
	UTC_UI_CALLBACK_CALLED;	
}

static void utc_ui_destroyed_cb(ui_view_t view)
{
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_shown_cb(ui_view_t view)
{
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_hidden_cb(ui_view_t view)
{
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_tick_cb(ui_widget_t widget, uint32_t dt)
{
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_interval_cb(ui_widget_t widget)
{
	static int cnt = 0;

	if (cnt < 5) {
		cnt++;
	} else {
		cnt = 0;
		UTC_UI_CALLBACK_CALLED;
	}
}

static void utc_ui_tween_finished_cb(ui_widget_t widget)
{
	UTC_UI_CALLBACK_CALLED;
}

#if defined(CONFIG_UI_TOUCH_INTERFACE)
static void utc_ui_touched_cb(ui_widget_t widget)
{
	return;
}
#endif // CONFIG_UI_TOUCH_INTERFACE

static void utc_ui_canvas_draw_line_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_line", ui_canvas_widget_draw_line(widget, 0, 0, 16, 16, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_line", ui_canvas_widget_draw_line(widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y, UTC_UI_DEFAULT_X + UTC_UI_DEFAULT_WIDTH * 2, UTC_UI_DEFAULT_Y + UTC_UI_DEFAULT_HEIGHT, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_line", ui_canvas_widget_draw_line(widget, UTC_UI_DEFAULT_X + UTC_UI_DEFAULT_WIDTH * 2, UTC_UI_DEFAULT_Y + UTC_UI_DEFAULT_HEIGHT, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_line", ui_canvas_widget_draw_line(widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y, UTC_UI_DEFAULT_X + UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_Y + UTC_UI_DEFAULT_HEIGHT * 2, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_line", ui_canvas_widget_draw_line(widget, UTC_UI_DEFAULT_X + UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_Y + UTC_UI_DEFAULT_HEIGHT * 2, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_line_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_line", ui_canvas_widget_draw_line(UI_NULL, 0, 0, 16, 16, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_hline_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_hline", ui_canvas_widget_draw_hline(widget, 0, 0, 16, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_hline_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_hline", ui_canvas_widget_draw_hline(UI_NULL, 0, 0, 16, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_vline_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_vline", ui_canvas_widget_draw_vline(widget, 0, 0, 16, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_vline_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_vline", ui_canvas_widget_draw_vline(UI_NULL, 0, 0, 16, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_rect_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_rect", ui_canvas_widget_draw_rect(widget, 0, 0, 16, 16, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_rect_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_rect", ui_canvas_widget_draw_rect(UI_NULL, 0, 0, 16, 16, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_circle_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_circle", ui_canvas_widget_draw_circle(widget, 0, 0, 16, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_circle_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_circle", ui_canvas_widget_draw_circle(UI_NULL, 0, 0, UTC_UI_INVALID_RADIUS, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_circle", ui_canvas_widget_draw_circle(widget, 0, 0, UTC_UI_INVALID_RADIUS, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_ellipse_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_ellipse", ui_canvas_widget_draw_ellipse(widget, 0, 0, 16, 16, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_ellipse_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_ellipse", ui_canvas_widget_draw_ellipse(UI_NULL, 0, 0, 16, 16, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_ellipse", ui_canvas_widget_draw_ellipse(widget, 0, 0, UTC_UI_DEFAULT_RADIUS, UTC_UI_INVALID_RADIUS, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_ellipse", ui_canvas_widget_draw_ellipse(widget, 0, 0, UTC_UI_INVALID_RADIUS, UTC_UI_DEFAULT_RADIUS, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_ellipse", ui_canvas_widget_draw_ellipse(widget, 0, 0, UTC_UI_INVALID_RADIUS, UTC_UI_INVALID_RADIUS, 0xffff), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_arc_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_arc", ui_canvas_widget_draw_arc(widget, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_RADIUS, UTC_UI_DEFAULT_RADIAN, UTC_UI_DEFAULT_RADIAN * 6, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_arc", ui_canvas_widget_draw_arc(widget, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_RADIUS, UTC_UI_DEFAULT_RADIAN, UTC_UI_DEFAULT_RADIAN * 8, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_draw_arc_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_arc", ui_canvas_widget_draw_arc(UI_NULL, 0, 0, 16, 0.0, 5.0, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_arc", ui_canvas_widget_draw_arc(widget, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_CENTER, UTC_UI_INVALID_RADIUS, UTC_UI_DEFAULT_RADIAN, UTC_UI_DEFAULT_RADIAN, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_arc", ui_canvas_widget_draw_arc(widget, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_RADIUS, UTC_UI_DEFAULT_RADIAN, UTC_UI_INVALID_RADIAN, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_draw_arc", ui_canvas_widget_draw_arc(widget, UTC_UI_DEFAULT_CENTER, UTC_UI_DEFAULT_CENTER, UTC_UI_INVALID_RADIUS, UTC_UI_DEFAULT_RADIAN, UTC_UI_INVALID_RADIAN, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_fill_figure_p_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, 0, 0, UTC_UI_DEFAULT_COLOR), UI_OK, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

static void utc_ui_canvas_fill_figure_n_cb(ui_widget_t widget, uint32_t dt)
{
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(UI_NULL, 0, 0, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, UTC_UI_INVALID_X, UTC_UI_DEFAULT_Y, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, UTC_UI_DEFAULT_X, UTC_UI_INVALID_Y, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, UTC_UI_INVALID_X, UTC_UI_INVALID_Y, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, CONFIG_UI_DISPLAY_WIDTH, UTC_UI_DEFAULT_Y, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, UTC_UI_DEFAULT_X, CONFIG_UI_DISPLAY_HEIGHT, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_fill_figure", ui_canvas_widget_fill_figure(widget, CONFIG_UI_DISPLAY_WIDTH, CONFIG_UI_DISPLAY_HEIGHT, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());
	UTC_UI_CALLBACK_CALLED;
}

/****************************************************************************
 * TC Functions
 ****************************************************************************/

static void utc_ui_start_p(void)
{
	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);
	TC_ASSERT_EQ_CLEANUP("ui_start", ui_start(), UI_ALREADY_RUNNING, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_stop_p(void)
{
	ui_view_t view;
	ui_view_t next_view;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", g_ret, ETIMEDOUT, ui_stop());

	next_view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", next_view, UI_NULL, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", g_ret, ETIMEDOUT, ui_stop());

	sleep(1);

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_NOT_RUNNING);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_create_p(void)
{
	ui_view_t view;
	ui_view_t next_view;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", g_ret, ETIMEDOUT, ui_stop());

	next_view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", next_view, UI_NULL, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", g_ret, ETIMEDOUT, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(next_view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_create_n(void)
{
	ui_view_t view;

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_EQ("ui_view_create", view, UI_NULL);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(NULL, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_EQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_destroy_p(void)
{
	ui_view_t view;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_view_destroy", g_ret, ETIMEDOUT, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_destroy_n(void)
{
	TC_ASSERT_EQ("ui_view_destroy", ui_view_destroy(UTC_UI_INVALID_VIEW), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(UTC_UI_INVALID_VIEW), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_get_root_p(void)
{
	ui_view_t view;
	ui_widget_t root;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", g_ret, ETIMEDOUT, ui_stop());

	root = ui_view_get_root(view);
	TC_ASSERT_NEQ_CLEANUP("ui_view_get_root", root, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_get_root_n(void)
{
	ui_widget_t root;

	root = ui_view_get_root(UTC_UI_INVALID_VIEW);
	TC_ASSERT_EQ("ui_view_get_root", root, UI_NULL);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_add_widget_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_add_widget_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_view_add_widget", ui_view_add_widget(UTC_UI_INVALID_VIEW, UTC_UI_INVALID_WIDGET, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, UTC_UI_INVALID_WIDGET, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(UTC_UI_INVALID_VIEW, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(UTC_UI_INVALID_VIEW, UTC_UI_INVALID_WIDGET, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_transit_p(void)
{
	ui_view_t view;
	ui_view_t next_view;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	next_view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", next_view, UI_NULL, ui_stop());

	// Wait for creating the views at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_view_transit", ui_view_transit(next_view, UI_TRANSITION_FADE_OUT, UTC_UI_DEFAULT_INTERVAL), UI_OK, ui_stop());

	// Wait for processing the transition at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(next_view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_view_transit_n(void)
{
	ui_view_t view;
	ui_view_t next_view;

	/* In order to make the negative test case, next_view is set to the UTC_UI_INVALID_VIEW value. */
	next_view = UTC_UI_INVALID_VIEW;

	TC_ASSERT_EQ("ui_view_transit", ui_view_transit(next_view, UI_TRANSITION_FADE_OUT, UTC_UI_DEFAULT_INTERVAL), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_view_transit", ui_view_transit(next_view, UI_TRANSITION_FADE_OUT, UTC_UI_DEFAULT_INTERVAL), UI_INVALID_PARAM, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	next_view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", next_view, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_transit", ui_view_transit(next_view, UTC_UI_INVALID_TYPE, UTC_UI_DEFAULT_INTERVAL), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(next_view), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_create_from_file_p(void)
{
	ui_asset_t image;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_file(UTC_UI_DEFAULT_IMAGE_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());

	image = ui_image_asset_create_from_file(UTC_UI_DEFAULT_IMAGE_PATH_RGB);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_create_from_file_n(void)
{
	ui_asset_t image;

	image = ui_image_asset_create_from_file(UTC_UI_INVALID_PATH);
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_start", ui_start(), UI_OK, ui_stop());

	image = ui_image_asset_create_from_file(UTC_UI_INVALID_PATH);
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_stop", ui_stop(), UI_OK, ui_stop());

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_create_from_buffer_p(void)
{
	ui_asset_t image;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_create_from_buffer_n(void)
{
	ui_asset_t image;

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_buffer(NULL);
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_destroy_p(void)
{
	ui_asset_t image;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_destroy_n(void)
{
	ui_asset_t image;

	/* In order to make the negative test case, image is set to the UTC_UI_INVALID_IMAGE value. */
	image = UTC_UI_INVALID_IMAGE;
	TC_ASSERT_EQ("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_set_pivot_point_p(void)
{
	ui_asset_t image;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_set_pivot_point", ui_image_asset_set_pivot_point(image, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_OK, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();

}

static void utc_ui_image_asset_set_pivot_point_n(void)
{
	ui_asset_t image;

	/* In order to make the negative test case, image is set to the UTC_UI_INVALID_IMAGE value. */
	image = UTC_UI_INVALID_IMAGE;
	TC_ASSERT_EQ("ui_image_asset_set_pivot_point", ui_image_asset_set_pivot_point(image, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_set_pivot_point", ui_image_asset_set_pivot_point(image, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();

}

static void utc_ui_image_asset_get_width_p(void)
{
	ui_asset_t image;
	size_t width;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL, ui_stop());

	width = ui_image_asset_get_width(image);
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_get_width", width, UTC_UI_DEFAULT_WIDTH,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_get_width_n(void)
{
	ui_asset_t image;

	/* In order to make the negative test case, image is set to the UTC_UI_INVALID_IMAGE value. */
	image = UTC_UI_INVALID_IMAGE;
	TC_ASSERT_EQ("ui_image_asset_get_width", ui_image_asset_get_width(image), 0);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_get_width", ui_image_asset_get_width(image), 0, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_get_height_p(void)
{
	ui_asset_t image;
	size_t height;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL, ui_stop());

	height = ui_image_asset_get_height(image);
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_get_width", height, UTC_UI_DEFAULT_HEIGHT,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_asset_get_height_n(void)
{
	ui_asset_t image;

	/* In order to make the negative test case, image is set to the UTC_UI_INVALID_IMAGE value. */
	image = UTC_UI_INVALID_IMAGE;
	TC_ASSERT_EQ("ui_image_asset_get_height", ui_image_asset_get_height(image), 0);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_get_height", ui_image_asset_get_height(image), 0, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_font_asset_create_from_file_p(void)
{
	ui_asset_t font;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_font_asset_create_from_file_n(void)
{
	ui_asset_t font;

	font = ui_font_asset_create_from_file(UTC_UI_INVALID_PATH);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL,
		do {
			ui_font_asset_destroy(font);
		} while (0));

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_INVALID_PATH);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	font = ui_font_asset_create_from_file(UTC_UI_INVALID_FONT_EXTENSION);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_font_asset_destroy_p(void)
{
	ui_asset_t font;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();;
}

static void utc_ui_font_asset_destroy_n(void)
{
	ui_asset_t font;

	/* In order to make the negative test case, font is set to the UTC_UI_INVALID_FONT value. */
	font = UTC_UI_INVALID_FONT;
	TC_ASSERT_EQ("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_font_asset_measure_text_p(void)
{
	ui_asset_t font;
	int ret = 0;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	ret = ui_font_asset_measure_text(font, UTC_UI_TEXT);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_measure_text", ret, UTC_UI_U_WIDTH + UTC_UI_I_WIDTH + UTC_UI_GA_WIDTH,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_font_asset_measure_text_n(void)
{
	ui_asset_t font;
	int ret;

	/* In order to make the negative test case, font is set to the UTC_UI_INVALID_FONT value. */
	font = UTC_UI_INVALID_FONT;
	ret = ui_font_asset_measure_text(font, UTC_UI_TEXT);
	TC_ASSERT_EQ("ui_font_asset_measure_text", ret, 0);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	ret = ui_font_asset_measure_text(font, UTC_UI_TEXT);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_measure_text", ret, 0, ui_stop());

	ret = ui_font_asset_measure_text(font, NULL);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_measure_text", ret, 0, ui_stop());

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	ret = ui_font_asset_measure_text(font, NULL);
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_measure_text", ret, 0,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_create_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_create_n(void)
{
	ui_widget_t widget;

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_EQ_CLEANUP("ui_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
		} while (0));

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_destroy_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_destroy_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_widget_destroy", ui_widget_destroy(widget), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_add_child_p(void)
{
	ui_widget_t parent;
	ui_widget_t child;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	parent = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", parent, UI_NULL, ui_stop());

	child = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", child, UI_NULL,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(parent), UI_OK, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_add_child_n(void)
{
	ui_widget_t parent;
	ui_widget_t child;

	// In order to make the negative test case, parent is set to the UTC_UI_INVALID_WIDGET value.
	parent = UTC_UI_INVALID_WIDGET;
	// In order to make the negative test case, child is set to the UTC_UI_INVALID_WIDGET value.
	child = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_INVALID_PARAM, ui_stop());

	parent = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", parent, UI_NULL, ui_stop());

	child = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", child, UI_NULL,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(UTC_UI_INVALID_WIDGET, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, UTC_UI_INVALID_WIDGET, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(parent), UI_OK,
		do {
			ui_widget_destroy(child);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(child), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_remove_child_p(void)
{
	ui_widget_t parent;
	ui_widget_t child;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	parent = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", parent, UI_NULL, ui_stop());

	child = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", child, UI_NULL,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_remove_child", ui_widget_remove_child(parent, child), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(child), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(parent), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_remove_child_n(void)
{
	ui_widget_t parent;
	ui_widget_t child;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	parent = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", parent, UI_NULL, ui_stop());

	child = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", child, UI_NULL,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_NEQ_CLEANUP("ui_widget_remove_child", ui_widget_remove_child(parent, UI_NULL), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_NEQ_CLEANUP("ui_widget_remove_child", ui_widget_remove_child(UI_NULL, child), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(parent), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_remove_all_children_p(void)
{
	ui_widget_t parent;
	ui_widget_t child;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	parent = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", parent, UI_NULL, ui_stop());

	child = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", child, UI_NULL,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_remove_all_children", ui_widget_remove_all_children(parent), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(child), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(parent), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_remove_all_children_n(void)
{
	ui_widget_t parent;
	ui_widget_t child;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	parent = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", parent, UI_NULL, ui_stop());

	child = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", child, UI_NULL,
		do {
			ui_widget_destroy(parent);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_add_child", ui_widget_add_child(parent, child, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_NEQ_CLEANUP("ui_widget_remove_all_children", ui_widget_remove_all_children(UI_NULL), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(child), UI_OK,
		do {
			ui_widget_destroy(parent);
			ui_widget_destroy(child);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(parent), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_visible_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_visible", ui_widget_set_visible(widget, true), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_visible", ui_widget_set_visible(widget, false), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_visible_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;

	TC_ASSERT_EQ("ui_widget_set_visible", ui_widget_set_visible(widget, true), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_visible", ui_widget_set_visible(widget, true), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_position_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_position", ui_widget_set_position(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_position_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;

	TC_ASSERT_EQ("ui_widget_set_position", ui_widget_set_position(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_position", ui_widget_set_position(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_get_rect_p(void)
{
	ui_view_t view;
	ui_widget_t widget;
	ui_rect_t ret;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	ret = ui_widget_get_rect(widget);
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.x, UTC_UI_DEFAULT_X, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.y, UTC_UI_DEFAULT_Y, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.width, UTC_UI_DEFAULT_WIDTH, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.height, UTC_UI_DEFAULT_HEIGHT, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_get_rect_n(void)
{
	ui_widget_t widget;
	ui_rect_t ret;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;

	ret = ui_widget_get_rect(widget);
	TC_ASSERT_EQ("ui_widget_get_rect", ret.x, UTC_UI_NULL_RECT.x);
	TC_ASSERT_EQ("ui_widget_get_rect", ret.y, UTC_UI_NULL_RECT.y);
	TC_ASSERT_EQ("ui_widget_get_rect", ret.width, UTC_UI_NULL_RECT.width);
	TC_ASSERT_EQ("ui_widget_get_rect", ret.height, UTC_UI_NULL_RECT.height);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	ret = ui_widget_get_rect(widget);
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.x, UTC_UI_NULL_RECT.x, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.y, UTC_UI_NULL_RECT.y, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.width, UTC_UI_NULL_RECT.width, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_get_rect", ret.height, UTC_UI_NULL_RECT.height, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_size_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_size", ui_widget_set_size(widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_size_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;

	TC_ASSERT_EQ("ui_widget_set_position", ui_widget_set_size(widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_position", ui_widget_set_size(widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_tick_callback_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_tick_callback", ui_widget_set_tick_callback(widget, utc_ui_tick_cb), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_set_tick_callback", g_ret, ETIMEDOUT, ui_stop());

	// Wait for processing the tick callback at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_tick_callback_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;

	TC_ASSERT_EQ("ui_widget_set_tick_callback", ui_widget_set_tick_callback(widget, utc_ui_tick_cb), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_tick_callback", ui_widget_set_tick_callback(widget, utc_ui_tick_cb), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_interval_callback_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_interval_callback", ui_widget_set_interval_callback(widget, utc_ui_interval_cb, UTC_UI_DEFAULT_INTERVAL), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_set_tick_callback", g_ret, ETIMEDOUT, ui_stop());

	// Wait for processing the interval callback at the ui core thread
	sleep(1);
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_interval_callback_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;

	TC_ASSERT_EQ("ui_widget_set_interval_callback", ui_widget_set_interval_callback(widget, utc_ui_interval_cb, UTC_UI_DEFAULT_INTERVAL), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_interval_callback", ui_widget_set_interval_callback(widget, utc_ui_interval_cb, UTC_UI_DEFAULT_INTERVAL), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_tween_moveto_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_tween_moveto", ui_widget_tween_moveto(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y, UTC_UI_DEFAULT_INTERVAL, TWEEN_LINEAR, utc_ui_tween_finished_cb), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_tween_moveto", g_ret, ETIMEDOUT, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_tween_moveto", ui_widget_tween_moveto(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y, UTC_UI_DEFAULT_INTERVAL, TWEEN_EASE_IN_QUAD, utc_ui_tween_finished_cb), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_tween_moveto", g_ret, ETIMEDOUT, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_tween_moveto", ui_widget_tween_moveto(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y, UTC_UI_DEFAULT_INTERVAL, TWEEN_EASE_OUT_QUAD, utc_ui_tween_finished_cb), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_tween_moveto", g_ret, ETIMEDOUT, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_tween_moveto", ui_widget_tween_moveto(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y, UTC_UI_DEFAULT_INTERVAL, TWEEN_EASE_INOUT_QUAD, utc_ui_tween_finished_cb), UI_OK, ui_stop());
	UTC_UI_WAIT_CALLBACK(1);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_tween_moveto", g_ret, ETIMEDOUT, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_tween_moveto_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, image is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_widget_tween_moveto", ui_widget_tween_moveto(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y, UTC_UI_DEFAULT_INTERVAL, TWEEN_LINEAR, utc_ui_tween_finished_cb), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_tween_moveto", ui_widget_tween_moveto(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y, UTC_UI_DEFAULT_INTERVAL, TWEEN_LINEAR, utc_ui_tween_finished_cb), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_align_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_DEFAULT), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_LEFT), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_CENTER), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_RIGHT), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_TOP), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_MIDDLE), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_BOTTOM), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_align_n(void)
{
	ui_widget_t widget;

	/* In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value. */
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_DEFAULT), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_align", ui_widget_set_align(widget, UI_ALIGN_DEFAULT), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_UI_TOUCH_INTERFACE)
static void utc_ui_widget_set_touchable_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_touchable", ui_widget_set_touchable(widget, true), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_touchable", ui_widget_set_touchable(widget, false), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_widget_set_touchable_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_widget_set_touchable", ui_widget_set_touchable(widget, true), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_widget_set_touchable", ui_widget_set_touchable(widget, true), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_set_touchable", ui_widget_set_touchable(widget, false), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}
#endif

static void utc_ui_image_widget_create_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_create_n(void)
{
	ui_asset_t font;
	ui_widget_t widget;

	widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
		} while (0));

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	widget = ui_image_widget_create(font);
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_set_image_p(void)
{
	ui_view_t view;
	ui_widget_t widget;
	ui_asset_t image;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(widget, image), UI_OK,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	// Wait for drawing the image widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_set_image_n(void)
{
	ui_widget_t image_widget;
	ui_widget_t widget;
	ui_asset_t image;
	ui_asset_t font;

	// In order to make the negative test case, image_widget is set to the UTC_UI_INVALID_WIDGET value.
	image_widget = UTC_UI_INVALID_WIDGET;
	// In order to make the negative test case, imgae is set to the UTC_UI_INVALID_IMAGE value.
	image = UTC_UI_INVALID_IMAGE;
	TC_ASSERT_EQ("ui_image_widget_set_image", ui_image_widget_set_image(image_widget, image), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(image_widget, image), UI_INVALID_PARAM, ui_stop());

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(image_widget, image), UI_INVALID_PARAM,
		do {
			ui_font_asset_destroy(font);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(widget, image), UI_INVALID_PARAM,
		do {
			ui_font_asset_destroy(font);
			ui_widget_destroy(widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	image_widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", image_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_widget_destroy(widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(image_widget, UTC_UI_INVALID_IMAGE), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_image_asset_destroy(image);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(image_widget, font), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_image_asset_destroy(image);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_image_asset_destroy(image);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(image_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_set_anchor_point_p(void)
{
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_anchor_point", ui_image_widget_set_anchor_point(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_set_anchor_point_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_image_widget_set_anchor_point", ui_image_widget_set_anchor_point(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_anchor_point", ui_image_widget_set_anchor_point(widget, UTC_UI_MOVED_X, UTC_UI_MOVED_Y), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_set_crop_area_p(void)
{
	ui_view_t view;
	ui_asset_t image;
	ui_widget_t widget;
	ui_rect_t crop_area;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL, ui_stop());

	widget = ui_image_widget_create(image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", widget, UI_NULL,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	crop_area.x = UTC_UI_DEFAULT_X;
	crop_area.y = UTC_UI_DEFAULT_Y;
	crop_area.width = UTC_UI_DEFAULT_WIDTH;
	crop_area.height = UTC_UI_DEFAULT_HEIGHT;
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(widget, crop_area), UI_OK, ui_stop());

	// Wait for drawing the image widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_set_crop_area_n(void)
{
	ui_widget_t image_widget;
	ui_widget_t widget;
	ui_asset_t image;
	ui_rect_t crop_area;

	// In order to make the negative test case, image_widget is set to the UTC_UI_INVALID_WIDGET value.
	image_widget = UTC_UI_INVALID_WIDGET;
	crop_area.x = UTC_UI_DEFAULT_X;
	crop_area.y = UTC_UI_DEFAULT_Y;
	crop_area.width = UTC_UI_DEFAULT_WIDTH;
	crop_area.height = UTC_UI_DEFAULT_HEIGHT;
	TC_ASSERT_EQ("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	image_widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", image_widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	crop_area.x = UTC_UI_INVALID_X;
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	crop_area.x = UTC_UI_DEFAULT_X;
	crop_area.y = UTC_UI_INVALID_Y;
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	crop_area.y = UTC_UI_DEFAULT_X;
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_image", ui_image_widget_set_image(image_widget, image), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	crop_area.width = UTC_UI_DEFAULT_WIDTH >> 1;
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	crop_area.width = UTC_UI_DEFAULT_WIDTH;
	crop_area.height = UTC_UI_DEFAULT_HEIGHT >> 1;
	TC_ASSERT_EQ_CLEANUP("ui_image_widget_set_crop_area", ui_image_widget_set_crop_area(image_widget, crop_area), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(image_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_resize_image_asset_p(void)
{
	ui_view_t view;
	ui_asset_t image;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_file", image, UI_NULL, ui_stop());

	widget = ui_image_widget_create(image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", widget, UI_NULL,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_resize_image_asset", ui_image_widget_resize_image_asset(widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	// Wait for drawing the image widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_image_widget_resize_image_asset_n(void)
{
	ui_widget_t image_widget;
	ui_widget_t widget;

	// In order to make the negative test case, image_widget is set to the UTC_UI_INVALID_WIDGET value.
	image_widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_image_widget_resize_image_asset", ui_image_widget_resize_image_asset(image_widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_resize_image_asset", ui_image_widget_resize_image_asset(image_widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_resize_image_asset", ui_image_widget_resize_image_asset(image_widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	image_widget = ui_image_widget_create(UI_NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_image_widget_create", image_widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_widget_resize_image_asset", ui_image_widget_resize_image_asset(image_widget, UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_widget_destroy(image_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(image_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_create_p(void)
{
	ui_view_t view;
	ui_asset_t font;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_NEQ_CLEANUP("ui_view_add_text_widget", widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_create_n(void)
{
	ui_asset_t image;
	ui_asset_t font;
	ui_widget_t widget;

	// In order to make the negative test case, font is set to the UTC_UI_INVALID_FONT value.
	font = UTC_UI_INVALID_FONT;

	widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
		} while (0));

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	image = ui_image_asset_create_from_buffer(g_utc_ui_image);
	TC_ASSERT_NEQ_CLEANUP("ui_image_asset_create_from_buffer", image, UI_NULL, ui_stop());

	widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, image, UTC_UI_TEXT);
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_image_asset_destroy(image);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_image_asset_destroy", ui_image_asset_destroy(image), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_text_p(void)
{
	ui_view_t view;
	ui_widget_t text_widget;
	ui_asset_t font;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	text_widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, "");
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, text_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text(text_widget, UTC_UI_TEXT), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_text_n(void)
{
	ui_widget_t widget;
	ui_widget_t text_widget;
	ui_asset_t font;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_text_widget_set_text", ui_text_widget_set_text(widget, NULL), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	text_widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text(widget, UTC_UI_TEXT), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text(widget, NULL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text(text_widget, NULL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text(widget, UTC_UI_TEXT), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text(widget, NULL), UI_INVALID_PARAM, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_color_p(void)
{
	ui_view_t view;
	ui_asset_t font;
	ui_widget_t text_widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	text_widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, text_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_color", ui_text_widget_set_color(text_widget, UTC_UI_DEFAULT_COLOR), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_color_n(void)
{
	ui_asset_t font;
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_text_widget_set_color", ui_text_widget_set_color(widget, UTC_UI_INVALID_COLOR), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_color", ui_text_widget_set_color(widget, UTC_UI_INVALID_COLOR), UI_INVALID_PARAM, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_color", ui_text_widget_set_color(widget, UTC_UI_DEFAULT_COLOR), UI_INVALID_PARAM, ui_stop());

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_color", ui_text_widget_set_color(widget, UTC_UI_INVALID_COLOR), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_text_format_p(void)
{
	ui_view_t view;
	ui_widget_t text_widget;
	ui_asset_t font;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	text_widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, "");
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, text_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text_format", ui_text_widget_set_text_format(text_widget, "%s", UTC_UI_TEXT), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_text_format_n(void)
{
	ui_widget_t widget;
	ui_widget_t text_widget;
	ui_asset_t font;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_text_widget_set_text", ui_text_widget_set_text_format(widget, NULL), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	text_widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text_format(widget, UTC_UI_TEXT), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text_format(widget, NULL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text", ui_text_widget_set_text_format(text_widget, NULL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text_format", ui_text_widget_set_text_format(widget, UTC_UI_TEXT), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_text_format", ui_text_widget_set_text_format(widget, NULL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(text_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_align_p(void)
{
	ui_view_t view;
	ui_asset_t font;
	ui_widget_t text_widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	text_widget = ui_text_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, text_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(text_widget, UI_ALIGN_LEFT), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(text_widget, UI_ALIGN_CENTER), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(text_widget, UI_ALIGN_RIGHT), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(text_widget, UI_ALIGN_TOP), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(text_widget, UI_ALIGN_MIDDLE), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(text_widget, UI_ALIGN_BOTTOM), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for drawing the text widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_align_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_text_widget_set_align", ui_text_widget_set_align(widget, UI_NULL), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(widget, UI_NULL), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_align", ui_text_widget_set_align(widget, UI_NULL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_word_wrap_p(void)
{
	ui_view_t view;
	ui_widget_t text_widget;
	ui_asset_t font;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	font = ui_font_asset_create_from_file(UTC_UI_DEFAULT_FONT_PATH);
	TC_ASSERT_NEQ_CLEANUP("ui_font_asset_create_from_file", font, UI_NULL, ui_stop());

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	text_widget = ui_text_widget_create(UTC_UI_WORD_WARP_WIDTH, UTC_UI_DEFAULT_HEIGHT, font, UTC_UI_TEXT);
	TC_ASSERT_NEQ_CLEANUP("ui_text_widget_create", text_widget, UI_NULL,
		do {
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, text_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_word_wrap", ui_text_widget_set_word_wrap(text_widget, true), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for processing ui_text_widget_set_word_wrap operation at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_word_wrap", ui_text_widget_set_word_wrap(text_widget, true), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_word_wrap", ui_text_widget_set_word_wrap(text_widget, false), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_font_asset_destroy(font);
			ui_stop();
		} while (0));
	// Wait for processing ui_text_widget_set_word_wrap operation at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_font_asset_destroy", ui_font_asset_destroy(font), UI_OK,
		do {
			ui_widget_destroy(text_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(text_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_text_widget_set_word_wrap_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_text_widget_set_word_wrap", ui_text_widget_set_word_wrap(widget, true), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_word_wrap", ui_text_widget_set_word_wrap(widget, true), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_text_widget_set_word_wrap", ui_text_widget_set_word_wrap(widget, true), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_button_widget_create_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_button_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_button_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_button_widget_create_n(void)
{
	ui_widget_t widget;

	widget = ui_button_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_EQ_CLEANUP("ui_button_widget_create", widget, UI_NULL,
	 do {
		 ui_widget_destroy(widget);
		 ui_stop();
	 } while (0));

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_UI_TOUCH_INTERFACE)
static void utc_ui_button_widget_set_touched_callback_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_button_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_button_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_button_widget_set_touched_callback", ui_button_widget_set_touched_callback(widget, utc_ui_touched_cb), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_button_widget_set_touched_callback_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_button_widget_set_touched_callback", ui_button_widget_set_touched_callback(widget, utc_ui_touched_cb), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_button_widget_set_touched_callback", ui_button_widget_set_touched_callback(widget, utc_ui_touched_cb), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_button_widget_set_touched_callback", ui_button_widget_set_touched_callback(widget, utc_ui_touched_cb), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}
#endif

static void utc_ui_scroll_widget_create_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_scroll_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_scroll_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_scroll_widget_create_n(void)
{
	ui_widget_t widget;

	widget = ui_scroll_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_SUCCESS_RESULT();
}

static void utc_ui_scroll_widget_set_direction_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	widget = ui_scroll_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_scroll_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(widget, UI_DIRECTION_ALL), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(widget, UI_DIRECTION_HORIZONTAL), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(widget, UI_DIRECTION_VERTICAL), UI_OK, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_scroll_widget_set_direction_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(widget, UI_DIRECTION_ALL), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(widget, UI_DIRECTION_ALL), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(widget, UI_DIRECTION_ALL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_scroll_widget_set_content_size_p(void)
{
	ui_view_t view;
	ui_widget_t horizontal_widget;
	ui_widget_t vertical_widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	horizontal_widget = ui_scroll_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_scroll_widget_create", horizontal_widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, horizontal_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_stop();
		} while (0));

	vertical_widget = ui_scroll_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_scroll_widget_create", vertical_widget, UI_NULL,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, vertical_widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(horizontal_widget, UI_DIRECTION_HORIZONTAL), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_direction", ui_scroll_widget_set_direction(vertical_widget, UI_DIRECTION_VERTICAL), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(horizontal_widget, UTC_UI_DEFAULT_CONTENT_SIZE_X, UTC_UI_DEFAULT_CONTENT_SIZE_Y), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(vertical_widget, UTC_UI_DEFAULT_CONTENT_SIZE_X, UTC_UI_DEFAULT_CONTENT_SIZE_Y), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(horizontal_widget, UTC_UI_DEFAULT_WIDTH - 1, UTC_UI_DEFAULT_HEIGHT - 1), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(vertical_widget, UTC_UI_DEFAULT_WIDTH - 1, UTC_UI_DEFAULT_HEIGHT - 1), UI_OK,
		do {
			ui_widget_destroy(horizontal_widget);
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));

	// Wait for drawing the scroll widget at the ui core thread
	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(horizontal_widget), UI_OK,
		do {
			ui_widget_destroy(vertical_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(vertical_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_scroll_widget_set_content_size_n(void)
{
	ui_widget_t widget;

	// In order to make the negative test case, widget is set to the UTC_UI_INVALID_WIDGET value.
	widget = UTC_UI_INVALID_WIDGET;
	TC_ASSERT_EQ("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(widget, UTC_UI_DEFAULT_CONTENT_SIZE_X, UTC_UI_DEFAULT_CONTENT_SIZE_Y), UI_NOT_RUNNING);

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(widget, UTC_UI_DEFAULT_CONTENT_SIZE_X, UTC_UI_DEFAULT_CONTENT_SIZE_Y), UI_INVALID_PARAM, ui_stop());

	widget = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ_CLEANUP("ui_scroll_widget_set_content_size", ui_scroll_widget_set_content_size(widget, UTC_UI_DEFAULT_CONTENT_SIZE_X, UTC_UI_DEFAULT_CONTENT_SIZE_Y), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_paginator_widget_create_p(void)
{
	ui_view_t view;
	ui_widget_t widget = UI_NULL;
	ui_widget_t pages[3] = { UI_NULL, };

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	pages[0] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[0], UI_NULL, ui_stop());

	pages[1] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[1], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_stop();
		} while (0));

	pages[2] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[2], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_stop();
		} while (0));

	widget = ui_paginator_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, pages, 3);
	TC_ASSERT_NEQ_CLEANUP("ui_paginator_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[0]), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[1]), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[2]), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_paginator_widget_create_n(void)
{
	ui_view_t view;
	ui_widget_t widget;
	ui_widget_t pages[3] = { UI_NULL, };

	widget = ui_paginator_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_create", widget, UI_NULL, ui_stop());

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	pages[0] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[0], UI_NULL, ui_stop());

	pages[1] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[1], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
		} while (0));

	pages[2] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[2], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
		} while (0));

	widget = ui_paginator_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, NULL, 3);
	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
		} while (0));

	widget = ui_paginator_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, pages, 0);
	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[0]), UI_OK,
		do {
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[1]), UI_OK,
		do {
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[2]), UI_OK,
		do {
			ui_widget_destroy(widget);
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_paginator_widget_set_direction_p(void)
{
	ui_view_t view;
	ui_widget_t widget;
	ui_widget_t pages[3] = { UI_NULL, };

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	pages[0] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[0], UI_NULL, ui_stop());

	pages[1] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[1], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_stop();
		} while (0));

	pages[2] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[2], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_stop();
		} while (0));

	widget = ui_paginator_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, pages, 3);
	TC_ASSERT_NEQ_CLEANUP("ui_paginator_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_set_direction", ui_paginator_widget_set_direction(widget, UI_DIRECTION_ALL), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_set_direction", ui_paginator_widget_set_direction(widget, UI_DIRECTION_HORIZONTAL), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_set_direction", ui_paginator_widget_set_direction(widget, UI_DIRECTION_VERTICAL), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[0]), UI_OK,
		do {
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[1]), UI_OK,
		do {
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[2]), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_paginator_widget_set_direction_n(void)
{
	ui_view_t view;
	ui_widget_t invalid_widget;
	ui_widget_t widget;
	ui_widget_t pages[3] = { UI_NULL, };

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	pages[0] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[0], UI_NULL, ui_stop());

	pages[1] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[1], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_stop();
		} while (0));

	pages[2] = ui_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_widget_create", pages[2], UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_stop();
		} while (0));

	widget = ui_paginator_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, pages, 3);
	TC_ASSERT_NEQ_CLEANUP("ui_paginator_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	invalid_widget = ui_button_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT);
	TC_ASSERT_NEQ_CLEANUP("ui_button_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_set_direction", ui_paginator_widget_set_direction(invalid_widget, UI_DIRECTION_ALL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_widget_destroy(invalid_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_set_direction", ui_paginator_widget_set_direction(UI_NULL, UI_DIRECTION_HORIZONTAL), UI_INVALID_PARAM,
		do {
			ui_widget_destroy(pages[0]);
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_widget_destroy(invalid_widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[0]), UI_OK,
		do {
			ui_widget_destroy(pages[1]);
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_widget_destroy(invalid_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[1]), UI_OK,
		do {
			ui_widget_destroy(pages[2]);
			ui_widget_destroy(widget);
			ui_widget_destroy(invalid_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(pages[2]), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_widget_destroy(invalid_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK,
		do {
			ui_widget_destroy(invalid_widget);
			ui_stop();
		} while (0));
	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(invalid_widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	sleep(1);

	TC_ASSERT_EQ_CLEANUP("ui_paginator_widget_set_direction", ui_paginator_widget_set_direction(widget, UI_DIRECTION_ALL), UI_NOT_RUNNING, ui_stop());

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_create_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, NULL);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_create_n(void)
{
	ui_widget_t widget;

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, NULL);
	TC_ASSERT_EQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_line_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_line_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_line_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_line_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_hline_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_hline_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_hline_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_hline_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_vline_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_vline_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_vline_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_vline_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_rect_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_rect_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_rect_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_rect_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_circle_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_circle_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_circle_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_circle_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_ellipse_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_ellipse_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_ellipse_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_ellipse_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_arc_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_arc_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_draw_arc_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_draw_arc_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_fill_figure_p(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_fill_figure_p_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}

static void utc_ui_canvas_widget_fill_figure_n(void)
{
	ui_view_t view;
	ui_widget_t widget;

	TC_ASSERT_EQ("ui_start", ui_start(), UI_OK);

	view = ui_view_create(utc_ui_created_cb, utc_ui_destroyed_cb, utc_ui_shown_cb, utc_ui_hidden_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_view_create", view, UI_NULL, ui_stop());

	UTC_UI_WAIT_CALLBACK(1);

	widget = ui_canvas_widget_create(UTC_UI_DEFAULT_WIDTH, UTC_UI_DEFAULT_HEIGHT, utc_ui_canvas_fill_figure_n_cb);
	TC_ASSERT_NEQ_CLEANUP("ui_canvas_widget_create", widget, UI_NULL, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_add_widget", ui_view_add_widget(view, widget, UTC_UI_DEFAULT_X, UTC_UI_DEFAULT_Y), UI_OK,
		do {
			ui_widget_destroy(widget);
			ui_stop();
		} while (0));

	UTC_UI_WAIT_CALLBACK(1);

	TC_ASSERT_EQ_CLEANUP("ui_widget_destroy", ui_widget_destroy(widget), UI_OK, ui_stop());
	TC_ASSERT_EQ_CLEANUP("ui_view_destroy", ui_view_destroy(view), UI_OK, ui_stop());
	TC_ASSERT_EQ("ui_stop", ui_stop(), UI_OK);

	TC_SUCCESS_RESULT();
}


/****************************************************************************
 * Name: utc_ui_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_ui_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "UI UTC") == ERROR) {
		return ERROR;
	}

	pthread_mutex_init(&g_callback_lock, NULL);
	pthread_cond_init(&g_callback_cond, NULL);

	utc_ui_start_p();
	utc_ui_stop_p();

	/**
	 * View
	 */
	utc_ui_view_create_p();
	utc_ui_view_create_n();
	utc_ui_view_destroy_p();
	utc_ui_view_destroy_n();
	utc_ui_view_get_root_p();
	utc_ui_view_get_root_n();
	utc_ui_view_add_widget_p();
	utc_ui_view_add_widget_n();
	utc_ui_view_transit_p();
	utc_ui_view_transit_n();

	/**
	 * Assets
	 */
	utc_ui_image_asset_create_from_file_p();
	utc_ui_image_asset_create_from_file_n();
	utc_ui_image_asset_create_from_buffer_p();
	utc_ui_image_asset_create_from_buffer_n();
	utc_ui_image_asset_destroy_p();
	utc_ui_image_asset_destroy_n();
	utc_ui_image_asset_set_pivot_point_p();
	utc_ui_image_asset_set_pivot_point_n();
	utc_ui_image_asset_get_width_p();
	utc_ui_image_asset_get_width_n();
	utc_ui_image_asset_get_height_p();
	utc_ui_image_asset_get_height_n();
	utc_ui_font_asset_create_from_file_p();
	utc_ui_font_asset_create_from_file_n();
	utc_ui_font_asset_destroy_p();
	utc_ui_font_asset_destroy_n();
	utc_ui_font_asset_measure_text_p();
	utc_ui_font_asset_measure_text_n();

	/**
	 * Widgets
	 */
	utc_ui_widget_create_p();
	utc_ui_widget_create_n();
	utc_ui_widget_destroy_p();
	utc_ui_widget_destroy_n();
	utc_ui_widget_add_child_p();
	utc_ui_widget_add_child_n();
	utc_ui_widget_remove_child_p();
	utc_ui_widget_remove_child_n();
	utc_ui_widget_remove_all_children_p();
	utc_ui_widget_remove_all_children_n();
	utc_ui_widget_set_visible_p();
	utc_ui_widget_set_visible_n();
	utc_ui_widget_set_position_p();
	utc_ui_widget_set_position_n();
	utc_ui_widget_get_rect_p();
	utc_ui_widget_get_rect_n();
	utc_ui_widget_set_size_p();
	utc_ui_widget_set_size_n();
	utc_ui_widget_set_tick_callback_p();
	utc_ui_widget_set_tick_callback_n();
	utc_ui_widget_set_interval_callback_p();
	utc_ui_widget_set_interval_callback_n();
	utc_ui_widget_tween_moveto_p();
	utc_ui_widget_tween_moveto_n();
	utc_ui_widget_set_align_p();
	utc_ui_widget_set_align_n();
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	utc_ui_widget_set_touchable_p();
	utc_ui_widget_set_touchable_n();
#endif

	utc_ui_image_widget_create_p();
	utc_ui_image_widget_create_n();
	utc_ui_image_widget_set_image_p();
	utc_ui_image_widget_set_image_n();
	utc_ui_image_widget_set_anchor_point_p();
	utc_ui_image_widget_set_anchor_point_n();
	utc_ui_image_widget_set_crop_area_p();
	utc_ui_image_widget_set_crop_area_n();
	utc_ui_image_widget_resize_image_asset_p();
	utc_ui_image_widget_resize_image_asset_n();

	utc_ui_text_widget_create_p();
	utc_ui_text_widget_create_n();
	utc_ui_text_widget_set_text_p();
	utc_ui_text_widget_set_text_n();
	utc_ui_text_widget_set_color_p();
	utc_ui_text_widget_set_color_n();
	utc_ui_text_widget_set_text_format_p();
	utc_ui_text_widget_set_text_format_n();
	utc_ui_text_widget_set_align_p();
	utc_ui_text_widget_set_align_n();
	utc_ui_text_widget_set_word_wrap_p();
	utc_ui_text_widget_set_word_wrap_n();

	utc_ui_button_widget_create_p();
	utc_ui_button_widget_create_n();
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	utc_ui_button_widget_set_touched_callback_p();
	utc_ui_button_widget_set_touched_callback_n();
#endif
	
	utc_ui_scroll_widget_create_p();
	utc_ui_scroll_widget_create_n();
	utc_ui_scroll_widget_set_direction_p();
	utc_ui_scroll_widget_set_direction_n();
	utc_ui_scroll_widget_set_content_size_p();
	utc_ui_scroll_widget_set_content_size_n();

	utc_ui_paginator_widget_create_p();
	utc_ui_paginator_widget_create_n();
	utc_ui_paginator_widget_set_direction_p();
	utc_ui_paginator_widget_set_direction_n();

	utc_ui_canvas_widget_create_p();
	utc_ui_canvas_widget_create_n();
	utc_ui_canvas_widget_draw_line_p();
	utc_ui_canvas_widget_draw_line_n();
	utc_ui_canvas_widget_draw_hline_p();
	utc_ui_canvas_widget_draw_hline_n();
	utc_ui_canvas_widget_draw_vline_p();
	utc_ui_canvas_widget_draw_vline_n();
	utc_ui_canvas_widget_draw_rect_p();
	utc_ui_canvas_widget_draw_rect_n();
	utc_ui_canvas_widget_draw_circle_p();
	utc_ui_canvas_widget_draw_circle_n();
	utc_ui_canvas_widget_draw_ellipse_p();
	utc_ui_canvas_widget_draw_ellipse_n();
	utc_ui_canvas_widget_draw_arc_p();
	utc_ui_canvas_widget_draw_arc_n();
	utc_ui_canvas_widget_fill_figure_p();
	utc_ui_canvas_widget_fill_figure_n();

	pthread_mutex_destroy(&g_callback_lock);
	pthread_cond_destroy(&g_callback_cond);

	(void)tc_handler(TC_END, "UI UTC");

	return 0;
}
