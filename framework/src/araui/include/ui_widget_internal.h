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
#include <vec/vec.h>
#include <araui/ui_commons.h>
#include <araui/ui_widget.h>
#include <araui/ui_animation.h>
#include "ui_asset_internal.h"
#include "ui_widget_internal.h"
#include "ui_commons_internal.h"
#include "ui_renderer.h"

#define CONFIG_UI_MAX_WIDGET_NUM         100

enum {
	UV_TOP_LEFT,
	UV_BOTTOM_LEFT,
	UV_BOTTOM_RIGHT,
	UV_TOP_RIGHT
};

typedef struct ui_widget_body_s ui_widget_body_t;

typedef void (*add_callback)(ui_widget_t widget);
typedef void (*remove_callback)(ui_widget_t widget);
typedef void (*draw_callback)(ui_widget_t widget, uint32_t dt);
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
	UI_QUICK_PANEL
} ui_widget_type_t;

typedef struct {
	uint32_t timeout;
	uint32_t current;
} interval_info_t;

typedef struct {
	ui_coord_t down_coord;
	ui_coord_t move_coord;
} ui_touch_info_t;

struct ui_widget_body_s {
	ui_widget_type_t type;

	bool visible;
#if defined(CONFIG_UI_ENABLE_TOUCH)
	bool touchable;
	bool is_hooker;
#endif

	ui_rect_t local_rect;	//!< Widget's local position and size
	ui_rect_t global_rect;	//!< Widget's global(absolute) position and size (size is same with the local's)

	float scale_x;
	float scale_y;
	int32_t degree;
	int32_t pivot_x;
	int32_t pivot_y;
	ui_mat3_t trans_mat;
	bool update_flag;

	struct ui_widget_body_s *parent;
	vec_void_t children;

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
	draw_callback render_cb;
	update_callback update_cb;
	anim_finished_callback anim_finished_cb;

	ui_anim_t *anim;

#if defined(CONFIG_UI_ENABLE_TOUCH)
	ui_touch_info_t touch_info;
	touch_callback touch_cb;
#endif
	interval_info_t interval_info;

	void *userdata;
};

typedef struct {
	ui_widget_body_t base;
	ui_image_asset_body_t *image;

	ui_uv_t uv[4]; // top-left, bottom-left, bottom-right, top-right
} ui_image_widget_body_t;

typedef struct {
	ui_widget_body_t base;
	ui_font_asset_body_t *font;
	size_t font_size;
	ui_color_t font_color;
	uint32_t *utf_code;
	uint32_t *width_array;
	size_t text_length;
	size_t line_num;
	ui_align_t align;
	bool word_wrap;
} ui_text_widget_body_t;

typedef struct {
	ui_widget_body_t base;
	bool pressed;
#if defined(CONFIG_UI_ENABLE_TOUCH)
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
	ui_reach_offset_action_type_t min_offset_reach;
	ui_reach_offset_action_type_t max_offset_reach;
#if defined(CONFIG_UI_ENABLE_TOUCH)
	ui_coord_t prev_touch;
	//!< This variables are updated when every frame by using touch delta value and the delta time
	float scroll_velocity_x;
	float scroll_velocity_y;
#endif // CONFIG_UI_ENABLE_TOUCH
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
#if defined(CONFIG_UI_ENABLE_TOUCH)
	ui_coord_t prev_touch;
#endif // CONFIG_UI_ENABLE_TOUCH
} ui_paginator_widget_body_t;

typedef enum {
	UI_QUICK_PANEL_STATE_NONE,
	UI_QUICK_PANEL_STATE_THRESHOLD,
	UI_QUICK_PANEL_STATE_TRANSITION,
	UI_QUICK_PANEL_STATE_AT_SCREEN
} ui_quick_panel_state_t;

typedef struct {
	ui_widget_body_t base;

	bool press;
	ui_coord_t touch_down;
	ui_quick_panel_state_t state;
	ui_quick_panel_event_type_t event_type;
	ui_transition_type_t transition_type;
	ui_widget_body_t *focus;
} ui_quick_panel_body_t;

typedef struct {
	ui_widget_body_t *queue[CONFIG_UI_MAX_WIDGET_NUM];
	int start;
	int end;
} ui_widget_body_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

bool ui_widget_check_widget_type(ui_widget_t widget, ui_widget_type_t type);
ui_error_t ui_widget_update_position_info(ui_widget_body_t *widget);
void ui_widget_init(ui_widget_body_t *body, int32_t width, int32_t height);
void ui_widget_deinit(ui_widget_body_t *body);
void ui_widget_update_global_rect(ui_widget_body_t *widget);
ui_error_t ui_widget_destroy_sync(ui_widget_body_t *body);
ui_error_t ui_widget_set_position_sync(ui_widget_body_t *body, int32_t x, int32_t y);
ui_error_t ui_widget_set_rotation_sync(ui_widget_body_t *body, int32_t degree);
ui_error_t ui_widget_set_scale_sync(ui_widget_body_t *body, uint32_t scale_x, uint32_t scale_y);

ui_widget_body_t *ui_widget_search_by_coord(ui_widget_body_t *widget, ui_coord_t coord);

void ui_quick_panel_disappear_tween_end_func(ui_widget_t widget, ui_anim_t anim);
void ui_quick_panel_appear_tween_end_func(ui_widget_t widget, ui_anim_t anim);
void ui_widget_queue_init(void);
bool ui_widget_is_queue_empty(void);
void ui_widget_queue_enqueue(ui_widget_body_t *body);
ui_widget_body_t *ui_widget_queue_dequeue(void);

#ifdef __cplusplus
}
#endif

#endif
