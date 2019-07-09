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

#ifndef __UI_WIDGET_INTERNAL_H__
#define __UI_WIDGET_INTERNAL_H__

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>
#include <araui/ui_commons.h>
#include <araui/ui_widget.h>
#include "ui_asset_internal.h"
#include "ui_widget_internal.h"
#include "libs/ui_list.h"

typedef struct ui_widget_body_s ui_widget_body_t;

typedef void (*add_callback)(ui_widget_t widget);
typedef void (*remove_callback)(ui_widget_t widget);
typedef void (*draw_callback)(ui_widget_t widget, uint32_t dt);
typedef void (*tween_callback)(ui_widget_t widget, uint32_t t);
typedef float (*easing_callback)(float t, float b, float c, float d);
typedef void (*touch_callback)(ui_widget_body_t *widget, ui_touch_event_t event, ui_coord_t coord);
typedef void (*update_callback)(ui_widget_t widget, uint32_t dt);

typedef enum {
	UI_EMPTY_WIDGET,
	UI_IMAGE_WIDGET,
	UI_TEXT_WIDGET,
	UI_BUTTON_WIDGET,
	UI_SCROLL_WIDGET,
	UI_PAGINATOR_WIDGET,
	UI_CANVAS_WIDGET,
	UI_FRAME_ANIMATION_WIDGET,
	UI_GLOBAL_WIDGET
} ui_widget_type_t;

typedef struct {
	uint32_t timeout;
	uint32_t current;
} interval_info_t;

typedef struct {
	ui_rect_t origin;
	ui_rect_t gap;
	tween_finished_callback tween_finished_cb;
	easing_callback easing_cb;
	uint32_t t;
	uint32_t d;
} tween_info_t;

typedef struct {
	ui_coord_t down_coord;
	ui_coord_t move_coord;
} ui_touch_info_t;

struct ui_widget_body_s {
	ui_widget_type_t type;

	bool visible;
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	bool touchable;
	bool is_hooker;
#endif

	ui_rect_t local_rect;	//!< Widget's local position and size
	ui_rect_t global_rect;	//!< Widget's global(absolute) position and size (size is same with the local's)
	ui_rect_t crop_rect;	//!< Widget's global cropped rectangular region

	ui_align_t align;

	struct ui_widget_body_s *parent;
	ui_list_t children;

	/**
	 * @brief Public callback functions
	 */
	tick_callback tick_cb;
	interval_callback interval_cb;

	/**
	 * @brief Internal callback functions
	 */
	add_callback add_cb;
	remove_callback remove_cb;
	draw_callback draw_cb;
	tween_callback tween_cb;
	update_callback update_cb;

#if defined(CONFIG_UI_TOUCH_INTERFACE)
	ui_touch_info_t touch_info;
	touch_callback touch_cb;
#endif
	interval_info_t interval_info;
	tween_info_t tween_info;
};

typedef struct {
	ui_widget_body_t base;
	ui_image_asset_body_t *image;
	int32_t resized_width;
	int32_t resized_height;

	int32_t anchor_x;
	int32_t anchor_y;
	ui_rect_t crop_rect;
} ui_image_widget_body_t;

typedef struct {
	ui_widget_body_t base;
	ui_font_asset_body_t *font;
	uint32_t *utf_code;
	size_t text_length;
	size_t line_num;
	ui_align_t align;
	bool word_wrap;

	ui_color_t txt_foreground_color;
	ui_color_t txt_background_color;
} ui_text_widget_body_t;

typedef struct {
	ui_widget_body_t base;
	bool pressed;
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	button_touched_callback touched_cb;
#endif
} ui_button_widget_body_t;

typedef enum {
	UI_SCROLL_STATE_NONE,		//!< If touch down, state will be changed to UI_SCROLL_STATE_THRESHOLD
	UI_SCROLL_STATE_THRESHOLD,	//!< If the moving delta position is over the threshold, state be changed to UI_SCROLL_STATE_SCROLLING
	UI_SCROLL_STATE_SCROLLING	//!< If touch up, state will be changed to UI_SCROLL_STATE_NONE
} ui_scroll_state_t;

typedef struct {
	ui_widget_body_t base;

	//!< Scroll widget's primitive variables
	int32_t content_width;
	int32_t content_height;
	ui_direction_t direction;
	ui_scroll_state_t state;
	ui_coord_t offset;
	ui_coord_t prev_offset;

	/**
	 * @brief All children of the scroll widget can be moved (min_offset.x ~ 0), (min_offset.y ~ 0)
	 * min_offset values are negative integers.
	 */
	ui_coord_t min_offset;
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	ui_coord_t prev_touch;
	//!< This variables are updated when every frame by using touch delta value and the delta time
	float scroll_velocity_x;
	float scroll_velocity_y;
#endif // CONFIG_UI_TOUCH_INTERFACE
} ui_scroll_widget_body_t;

typedef enum {
	UI_PAGINATOR_STATE_NONE,
	UI_PAGINATOR_STATE_THRESHOLD,
	UI_PAGINATOR_STATE_SCROLLING,
	UI_PAGINATOR_STATE_TWEEN_NEXT,
	UI_PAGINATOR_STATE_TWEEN_PREV,
	UI_PAGINATOR_STATE_TWEEN_CUR
} ui_paginator_state_t;

typedef struct {
	ui_widget_body_t base;
	ui_direction_t direction;
	ui_paginator_state_t state;
	uint32_t page_count;
	int32_t cur_page_num;
	ui_widget_body_t **pages;
	ui_widget_body_t *cur_page;
	ui_widget_body_t *next_page;
	ui_widget_body_t *prev_page;
	int32_t offset;
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	ui_coord_t prev_touch;
#endif // CONFIG_UI_TOUCH_INTERFACE
} ui_paginator_widget_body_t;

typedef struct {
	ui_widget_body_t base;
	canvas_draw_callback canvas_draw_cb;
} ui_canvas_widget_body_t;

typedef struct {
	ui_widget_body_t base;
	ui_image_asset_body_t **frames;
	uint32_t frame_count;

	uint32_t duration;
	uint32_t interval;
	uint32_t time;
	bool play;
	bool loop;
} ui_frame_animation_widget_body_t;

typedef enum {
	UI_GLOBAL_WIDGET_STATE_NONE,
	UI_GLOBAL_WIDGET_STATE_TRANSITION,
	UI_GLOBAL_WIDGET_STATE_AT_SCREEN
} ui_global_widget_state_t;

typedef struct {
	ui_widget_body_t base;

	bool press;
	ui_coord_t touch_down;
	ui_global_widget_state_t state;

	ui_global_widget_event_type_t event_type;
	ui_transit_type_t transit_type;
} ui_global_widget_body_t;


#ifdef __cplusplus
extern "C" {
#endif

bool ui_widget_check_widget_type(ui_widget_t widget, ui_widget_type_t type);
ui_error_t ui_widget_update_position_info(ui_widget_body_t *widget);
void ui_widget_init(ui_widget_body_t *body, int32_t width, int32_t height);
ui_error_t ui_widget_destroy_sync(ui_widget_body_t *body);

ui_widget_body_t *ui_widget_search_by_coord(ui_widget_body_t *widget, ui_coord_t coord);

#ifdef __cplusplus
}
#endif

#endif
