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
#include <araui/ui_commons.h>
#include "ui_debug.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_window_internal.h"
#include "ui_widget_internal.h"
#include "ui_asset_internal.h"
#include "ui_commons_internal.h"
#include "ui_animation_internal.h"

#define CONFIG_UI_PAGINATOR_TWEEN_DURATION (100)

/**
 * If the scrolled delta is larger than (paginator widget's width * CONFIG_UI_PAGINATOR_THRESHOLD),
 * the page will be changed to the next or previous page.
 */
#define CONFIG_UI_PAGINATOR_THRESHOLD (0.33f)

typedef struct {
	ui_paginator_widget_body_t *body;
	ui_direction_t direction;
} ui_set_paginator_direction_t;

static void _ui_paginator_widget_set_direction_func(void *userdata);

ui_error_t ui_paginator_widget_set_direction(ui_widget_t widget, ui_direction_t direction)
{
	ui_set_paginator_direction_t *info;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!widget || !ui_widget_check_widget_type(widget, UI_PAGINATOR_WIDGET)) {
		return UI_INVALID_PARAM;
	}

	info = (ui_set_paginator_direction_t *)UI_ALLOC(sizeof(ui_set_paginator_direction_t));
	if (!info) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	memset(info, 0, sizeof(ui_set_paginator_direction_t));

	info->body = (ui_paginator_widget_body_t *)widget;
	info->direction = direction;

	if (ui_request_callback(_ui_paginator_widget_set_direction_func, info) != UI_OK) {
		UI_FREE(info);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_paginator_widget_set_direction_func(void *userdata)
{
	ui_set_paginator_direction_t *info;

	if (!userdata || !((ui_set_paginator_direction_t *)userdata)->body) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	info = (ui_set_paginator_direction_t *)userdata;

	info->body->direction = info->direction;

	// todo: implementation set direction

	UI_FREE(info);
}

#if defined(CONFIG_UI_ENABLE_TOUCH)

static void _ui_paginator_tween_finish(ui_widget_t widget, ui_anim_t anim)
{
	ui_widget_body_t *temp = NULL;
	ui_paginator_widget_body_t *body;
	int32_t next_page_num;
	int32_t prev_page_num;

	body = (ui_paginator_widget_body_t *)(((ui_widget_body_t *)widget)->parent);

	switch (body->state) {
	case UI_PAGINATOR_STATE_TWEEN_NEXT:
		body->cur_page_num = (body->cur_page_num + 1) % body->page_count;

		temp = body->prev_page;
		body->prev_page = body->cur_page;
		body->cur_page = body->next_page;
		body->next_page = temp;

		// Update the next page
		next_page_num = (body->cur_page_num + 1) % body->page_count;
		ui_widget_remove_all_children((ui_widget_t)body->next_page);

		ui_widget_add_child((ui_widget_t)body->next_page, (ui_widget_t)body->pages[next_page_num], 0, 0);
		break;

	case UI_PAGINATOR_STATE_TWEEN_PREV:
		body->cur_page_num = (body->cur_page_num + body->page_count - 1) % body->page_count;

		temp = body->next_page;
		body->next_page = body->cur_page;
		body->cur_page = body->prev_page;
		body->prev_page = temp;

		// Update the previous page
		prev_page_num = (body->cur_page_num + body->page_count - 1) % body->page_count;
		ui_widget_remove_all_children((ui_widget_t)body->prev_page);

		ui_widget_add_child((ui_widget_t)body->prev_page, (ui_widget_t)body->pages[prev_page_num], 0, 0);
		break;

	default:
		break;
	}

	if (body->direction == UI_DIRECTION_HORIZONTAL) {
		ui_widget_set_position((ui_widget_t)body->cur_page, 0, 0);
		ui_widget_set_position((ui_widget_t)body->prev_page, -body->base.local_rect.width, 0);
		ui_widget_set_position((ui_widget_t)body->next_page, body->base.local_rect.width, 0);
	}

	body->state = UI_PAGINATOR_STATE_NONE;
	ui_anim_destroy(anim);
}

static void _ui_paginator_remove_anim(ui_widget_t widget, ui_anim_t anim)
{
	ui_anim_destroy(anim);
}

void ui_paginator_widget_touch_func(ui_widget_body_t *widget, ui_touch_event_t event, ui_coord_t coord)
{
	ui_coord_t delta;
	ui_paginator_widget_body_t *body;
	ui_anim_t move_prev_page;
	ui_anim_t move_cur_page;
	ui_anim_t move_next_page;
	ui_rect_t rect_prev_page;
	ui_rect_t rect_cur_page;
	ui_rect_t rect_next_page;

	if (!widget) {
		UI_LOGE("Error: widget is null!\n");
		return;
	}

	body = (ui_paginator_widget_body_t *)widget;

	switch (event) {
	case UI_TOUCH_EVENT_CANCEL:
		if (body->state == UI_PAGINATOR_STATE_THRESHOLD ||
			body->state == UI_PAGINATOR_STATE_SCROLLING) {
			body->state = UI_PAGINATOR_STATE_NONE;
		}
		body->offset = 0;
		break;

	case UI_TOUCH_EVENT_DOWN:
		body->offset = 0;
		switch (body->state) {
		case UI_PAGINATOR_STATE_NONE:
			body->state = UI_PAGINATOR_STATE_THRESHOLD;
			body->prev_touch = coord;
			break;

		default:
			break;
		}
		break;

	case UI_TOUCH_EVENT_MOVE:
		delta.x = coord.x - body->prev_touch.x;
		delta.y = coord.y - body->prev_touch.y;

		switch (body->state) {
		case UI_PAGINATOR_STATE_THRESHOLD:
			if ((body->direction == UI_DIRECTION_VERTICAL && UI_ABS(delta.y) >= CONFIG_UI_TOUCH_THRESHOLD) ||
				(body->direction == UI_DIRECTION_HORIZONTAL && UI_ABS(delta.x) >= CONFIG_UI_TOUCH_THRESHOLD) ||
				(body->direction == UI_DIRECTION_ALL && (UI_ABS(delta.x) >= CONFIG_UI_TOUCH_THRESHOLD || UI_ABS(delta.y) >= CONFIG_UI_TOUCH_THRESHOLD))) {
				ui_core_lock_touch_event_target((ui_widget_body_t *)body);
				body->state = UI_PAGINATOR_STATE_SCROLLING;
			}
			break;

		case UI_PAGINATOR_STATE_SCROLLING:
			body->prev_touch = coord;

			// update position of all children (sync, prev, cur, next)
			if (body->direction == UI_DIRECTION_HORIZONTAL) {
				body->cur_page->local_rect.x += delta.x;
				body->prev_page->local_rect.x += delta.x;
				body->next_page->local_rect.x += delta.x;
				body->offset += delta.x;
			} else if (body->direction == UI_DIRECTION_VERTICAL) {
				body->cur_page->local_rect.y += delta.y;
				body->prev_page->local_rect.y += delta.y;
				body->next_page->local_rect.y += delta.y;
				body->offset += delta.y;
			}

			ui_widget_update_position_info(body->cur_page);
			ui_widget_update_position_info(body->prev_page);
			ui_widget_update_position_info(body->next_page);
			break;

		default:
			break;
		}
		break;

	case UI_TOUCH_EVENT_UP:
		if (body->state == UI_PAGINATOR_STATE_SCROLLING) {
			if (body->direction == UI_DIRECTION_HORIZONTAL) {
				if (body->offset < -(body->base.local_rect.width * CONFIG_UI_PAGINATOR_THRESHOLD)) {
					/**
					 * Paging to Right
					 */
					body->state = UI_PAGINATOR_STATE_TWEEN_NEXT;

					rect_cur_page = ui_widget_get_rect((ui_widget_t)body->cur_page);
					rect_next_page = ui_widget_get_rect((ui_widget_t)body->next_page);

					move_cur_page = ui_move_anim_create(rect_cur_page.x, rect_cur_page.y, -body->base.local_rect.width, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);
					move_next_page = ui_move_anim_create(rect_next_page.x, rect_next_page.y, 0, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);
					
					ui_widget_play_anim((ui_widget_t)body->cur_page, move_cur_page, _ui_paginator_tween_finish, false);
					ui_widget_play_anim((ui_widget_t)body->next_page, move_next_page, _ui_paginator_remove_anim, false);
				} else if (body->offset > (body->base.local_rect.width * CONFIG_UI_PAGINATOR_THRESHOLD)) {
					/**
					 * Paging to Left
					 */
					body->state = UI_PAGINATOR_STATE_TWEEN_PREV;

					rect_cur_page = ui_widget_get_rect((ui_widget_t)body->cur_page);
					rect_prev_page = ui_widget_get_rect((ui_widget_t)body->prev_page);

					move_cur_page = ui_move_anim_create(rect_cur_page.x, rect_cur_page.y, body->base.local_rect.width, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);
					move_prev_page = ui_move_anim_create(rect_prev_page.x, rect_prev_page.y, 0, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);

					ui_widget_play_anim((ui_widget_t)body->cur_page, move_cur_page, _ui_paginator_tween_finish, false);
					ui_widget_play_anim((ui_widget_t)body->prev_page, move_prev_page, _ui_paginator_remove_anim, false);
				} else {
					/**
					 * Paging to Current
					 */
					body->state = UI_PAGINATOR_STATE_TWEEN_CUR;

					rect_cur_page = ui_widget_get_rect((ui_widget_t)body->cur_page);
					rect_prev_page = ui_widget_get_rect((ui_widget_t)body->prev_page);
					rect_next_page = ui_widget_get_rect((ui_widget_t)body->next_page);

					move_cur_page = ui_move_anim_create(rect_cur_page.x, rect_cur_page.y, 0, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);
					move_prev_page = ui_move_anim_create(rect_prev_page.x, rect_prev_page.y, -body->base.local_rect.width, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);
					move_next_page = ui_move_anim_create(rect_next_page.x, rect_next_page.y, body->base.local_rect.width, 0, CONFIG_UI_PAGINATOR_TWEEN_DURATION, UI_INTRP_EASE_OUT_QUAD);

					ui_widget_play_anim((ui_widget_t)body->cur_page, move_cur_page, _ui_paginator_tween_finish, false);
					ui_widget_play_anim((ui_widget_t)body->prev_page, move_prev_page, _ui_paginator_remove_anim, false);
					ui_widget_play_anim((ui_widget_t)body->next_page, move_next_page, _ui_paginator_remove_anim, false);
				}
			} else if (body->direction == UI_DIRECTION_VERTICAL) {
				// todo
			}
		} else {
			if (body->state != UI_PAGINATOR_STATE_TWEEN_CUR &&
				body->state != UI_PAGINATOR_STATE_TWEEN_PREV &&
				body->state != UI_PAGINATOR_STATE_TWEEN_NEXT) {
				body->state = UI_PAGINATOR_STATE_NONE;
				body->offset = 0;
			}
		}
		break;

	default:
		break;
	}
}

#endif // CONFIG_UI_ENABLE_TOUCH

static void _ui_paginator_widget_removed_func(ui_widget_t widget)
{
	ui_paginator_widget_body_t *body;
	ui_widget_body_t *widget_body;
	ui_widget_body_t *child;
	int iter;

	if (!widget) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	body = (ui_paginator_widget_body_t *)widget;
	widget_body = (ui_widget_body_t *)widget;

	vec_foreach(&widget_body->children, child, iter) {
		child->parent = NULL;

		if (ui_widget_destroy_sync(child) != UI_OK) {
			UI_LOGE("error: Failed to destroy widget!\n");
		}
	}
	vec_clear(&widget_body->children);

	UI_FREE(body->pages);
}

ui_widget_t ui_paginator_widget_create(int32_t width, int32_t height, ui_widget_t pages[], uint32_t page_count)
{
	int32_t i;
	ui_paginator_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!pages || page_count <= 0) {
		UI_LOGE("error: pages is NULL or page count is zero!\n");
		return UI_NULL;
	}

	body = (ui_paginator_widget_body_t *)UI_ALLOC(sizeof(ui_paginator_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_paginator_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, width, height);
	((ui_widget_body_t *)body)->type = UI_PAGINATOR_WIDGET;

#if defined(CONFIG_UI_ENABLE_TOUCH)
	body->base.touchable = true;
	body->base.is_hooker = true;
	body->base.touch_cb = ui_paginator_widget_touch_func;
#endif

	body->direction = UI_DIRECTION_HORIZONTAL;
	body->page_count = page_count;
	body->pages = (ui_widget_body_t **)UI_ALLOC(page_count * sizeof(ui_widget_body_t *));

	if (!body->pages) {
		UI_LOGE("error: out of memory!\n");
		UI_FREE(body);
		return UI_NULL;
	}

	// allocated pages variable has to be freed
	body->base.remove_cb = _ui_paginator_widget_removed_func;

	// add default 3 page (prev - cur - next)
	body->cur_page = (ui_widget_body_t *)ui_widget_create(width, height);
	body->prev_page = (ui_widget_body_t *)ui_widget_create(width, height);
	body->next_page = (ui_widget_body_t *)ui_widget_create(width, height);

	ui_widget_add_child((ui_widget_t)body, (ui_widget_t)body->cur_page, 0, 0);
	ui_widget_add_child((ui_widget_t)body, (ui_widget_t)body->prev_page, -width, 0);
	ui_widget_add_child((ui_widget_t)body, (ui_widget_t)body->next_page, width, 0);

	// link each pages
	for (i = 0; i < page_count; i++) {
		body->pages[i] = (ui_widget_body_t *)(pages[i]);

		if (i == 0) { // first page
			ui_widget_add_child((ui_widget_t)body->cur_page, (ui_widget_t)body->pages[i], 0, 0);
		}
		if (i == 1) { // second page
			ui_widget_add_child((ui_widget_t)body->next_page, (ui_widget_t)body->pages[i], 0, 0);
		}
		if (i == page_count - 1) { // last page
			ui_widget_add_child((ui_widget_t)body->prev_page, (ui_widget_t)body->pages[i], 0, 0);
		}
	}

	return (ui_widget_t)body;
}
