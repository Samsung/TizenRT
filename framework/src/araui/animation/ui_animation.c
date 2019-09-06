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

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <araui/ui_commons.h>
#include <araui/ui_animation.h>
#include "ui_log.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_animation_internal.h"
#include "ui_commons_internal.h"
#include "utils/easing_fn.h"
#include <vec/vec.h>

static void _ui_anim_destory_func(void *userdata);
static bool _ui_anim_move_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_opacity_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_rotate_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_scale_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_sequence_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_spawn_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);

ui_anim_t ui_move_anim_create(int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, uint32_t duration, ui_intrp_type_t intrp_type)
{
	ui_move_anim_body_t *body;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_move_anim_body_t *)UI_ALLOC(sizeof(ui_move_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_move_anim_body_t));
	ui_anim_init((ui_anim_body_t *)body, UI_MOVE_ANIM, duration);
	body->from_coord.x = from_x;
	body->from_coord.y = from_y;
	body->to_coord.x = to_x;
	body->to_coord.y = to_y;
	body->intrp_type = intrp_type;

	return (ui_anim_t)body;
}

ui_anim_t ui_opacity_anim_create(uint32_t from_opacity, uint32_t to_opacity, uint32_t duration)
{
	ui_opacity_anim_body_t *body;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_opacity_anim_body_t *)UI_ALLOC(sizeof(ui_opacity_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_opacity_anim_body_t));
	ui_anim_init((ui_anim_body_t *)body, UI_OPACITY_ANIM, duration);
	body->from_opacity = from_opacity;
	body->to_opacity = to_opacity;

	return (ui_anim_t)body;
}

ui_anim_t ui_rotate_anim_create(int32_t from_degree, int32_t to_degree, uint32_t duration)
{
	ui_rotate_anim_body_t *body;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_rotate_anim_body_t *)UI_ALLOC(sizeof(ui_rotate_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_rotate_anim_body_t));
	ui_anim_init((ui_anim_body_t *)body, UI_ROTATE_ANIM, duration);
	body->from_degree = from_degree;
	body->to_degree = to_degree;

	return (ui_anim_t)body;
}

ui_anim_t ui_scale_anim_create(uint32_t from_scale, uint32_t to_scale, uint32_t duration)
{
	ui_scale_anim_body_t *body;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_scale_anim_body_t *)UI_ALLOC(sizeof(ui_scale_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_scale_anim_body_t));
	ui_anim_init((ui_anim_body_t *)body, UI_SCALE_ANIM, duration);
	body->from_scale = from_scale;
	body->to_scale = to_scale;

	return (ui_anim_t)body;
}

ui_anim_t ui_sequence_anim_create(ui_anim_t anim, ...)
{
	ui_sequence_anim_body_t *body;
	uint32_t duration = 0;
	ui_anim_t *iter;
	va_list ap;

	if (!anim) {
		UI_LOGE("error: invalid parameter\n");
	}

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_sequence_anim_body_t *)UI_ALLOC(sizeof(ui_sequence_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_sequence_anim_body_t));

	va_start(ap, anim);
	vec_init(&body->sequence);

	iter = (ui_anim_t *)anim;
	while (iter) {
		vec_push(&body->sequence, iter);
		duration += ((ui_anim_body_t *)iter)->d;
		iter = (ui_anim_t *)va_arg(ap, ui_anim_t);
	}
	
	body->seq_index = 0;

	ui_anim_init((ui_anim_body_t *)body, UI_SEQUENCE_ANIM, duration);

	return (ui_anim_t)body;
}

ui_anim_t ui_spawn_anim_create(ui_anim_t anim, ...)
{
	ui_spawn_anim_body_t *body;
	ui_anim_t *iter;
	va_list ap;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_spawn_anim_body_t *)UI_ALLOC(sizeof(ui_spawn_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_spawn_anim_body_t));
	ui_anim_init((ui_anim_body_t *)body, UI_SPAWN_ANIM, 0);

	return (ui_anim_t)body;
}

ui_error_t ui_anim_destroy(ui_anim_t anim)
{
	ui_anim_body_t *body;

	if (!ui_is_running()) {
		return UI_NOT_RUNNING;
	}

	if (!anim) {
		return UI_INVALID_PARAM;
	}

	body = (ui_anim_body_t *)anim;
	
	if (ui_request_callback(_ui_anim_destory_func, body) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_anim_destory_func(void *userdata)
{
	ui_anim_body_t *body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_anim_body_t *)userdata;

	UI_FREE(body);
}

void ui_anim_init(ui_anim_body_t *body, ui_anim_type_t type, uint32_t duration)
{
	if (!body) {
		UI_LOGE("error: Invalid Parameter!\n");
		return ;
	}

	body->type = type;
	body->d = duration;
	body->t = 0;

	switch (type) {
	case UI_MOVE_ANIM:
		body->func = _ui_anim_move_func;
		break;
	case UI_ROTATE_ANIM:
		body->func = _ui_anim_rotate_func;
		break;
	case UI_SCALE_ANIM:
		body->func = _ui_anim_scale_func;
		break;
	case UI_OPACITY_ANIM:
		body->func = _ui_anim_opacity_func;
		break;
	case UI_SEQUENCE_ANIM:
		body->func = _ui_anim_sequence_func;
		break;
	case UI_SPAWN_ANIM:
		body->func = _ui_anim_spawn_func;
		break;
	default:
		break;
	}
}

static bool _ui_anim_move_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_widget_body_t *widget_body = (ui_widget_body_t *)widget;
	ui_anim_body_t *anim_body = (ui_anim_body_t *)anim;
	ui_move_anim_body_t *move_anim_body = (ui_move_anim_body_t *)anim;
	uint32_t to_go = anim_body->d - anim_body->t;

	if (*dt <= to_go) {
		to_go = *dt;
	}
	anim_body->t += to_go;
	*dt -= to_go;

	switch (move_anim_body->intrp_type) {
	case UI_INTRP_EASE_IN_QUAD:
		widget_body->local_rect.x = (int32_t)ease_in_quad(anim_body->t, move_anim_body->from_coord.x, move_anim_body->to_coord.x - move_anim_body->from_coord.x, anim_body->d);
		widget_body->local_rect.y = (int32_t)ease_in_quad(anim_body->t, move_anim_body->from_coord.y, move_anim_body->to_coord.y - move_anim_body->from_coord.y, anim_body->d);
		break;
	case UI_INTRP_EASE_OUT_QUAD:
		widget_body->local_rect.x = (int32_t)ease_out_quad(anim_body->t, move_anim_body->from_coord.x, move_anim_body->to_coord.x - move_anim_body->from_coord.x, anim_body->d);
		widget_body->local_rect.y = (int32_t)ease_out_quad(anim_body->t, move_anim_body->from_coord.y, move_anim_body->to_coord.y - move_anim_body->from_coord.y, anim_body->d);
		break;
	case UI_INTRP_EASE_INOUT_QUAD:
		widget_body->local_rect.x = (int32_t)ease_inout_quad(anim_body->t, move_anim_body->from_coord.x, move_anim_body->to_coord.x - move_anim_body->from_coord.x, anim_body->d);
		widget_body->local_rect.y = (int32_t)ease_inout_quad(anim_body->t, move_anim_body->from_coord.y, move_anim_body->to_coord.y - move_anim_body->from_coord.y, anim_body->d);
		break;
	case UI_INTRP_LINEAR:
		widget_body->local_rect.x = (int32_t)ease_linear(anim_body->t, move_anim_body->from_coord.x, move_anim_body->to_coord.x - move_anim_body->from_coord.x, anim_body->d);
		widget_body->local_rect.y = (int32_t)ease_linear(anim_body->t, move_anim_body->from_coord.y, move_anim_body->to_coord.y - move_anim_body->from_coord.y, anim_body->d);
		break;
	}

	ui_widget_update_position_info(widget_body);
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_opacity_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_widget_body_t *widget_body = (ui_widget_body_t *)widget;
	ui_anim_body_t *anim_body = (ui_anim_body_t *)anim;
	uint32_t to_go = anim_body->d - anim_body->t;

	if (*dt <= to_go) {
		to_go = *dt;
	}
	anim_body->t += to_go;
	*dt -= to_go;
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_rotate_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_widget_body_t *widget_body = (ui_widget_body_t *)widget;
	ui_anim_body_t *anim_body = (ui_anim_body_t *)anim;
	uint32_t to_go = anim_body->d - anim_body->t;

	if (*dt <= to_go) {
		to_go = *dt;
	}
	anim_body->t += to_go;
	*dt -= to_go;
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_scale_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_widget_body_t *widget_body = (ui_widget_body_t *)widget;
	ui_anim_body_t *anim_body = (ui_anim_body_t *)anim;
	uint32_t to_go = anim_body->d - anim_body->t;

	if (*dt <= to_go) {
		to_go = *dt;
	}
	anim_body->t += to_go;
	*dt -= to_go;
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}
static bool _ui_anim_sequence_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_widget_body_t *widget_body = (ui_widget_body_t *)widget;
	ui_anim_body_t *anim_body = (ui_anim_body_t *)anim;
	ui_sequence_anim_body_t *seq_anim_body = (ui_sequence_anim_body_t *)anim;
	uint32_t to_go = anim_body->d - anim_body->t;
	ui_anim_body_t *child;

	if (*dt <= to_go) {
		to_go = *dt;
	}
	anim_body->t += to_go;
	*dt -= to_go;

	while (to_go > 0) {
		child = seq_anim_body->sequence.data[seq_anim_body->seq_index];

		if (child->func(widget, (ui_anim_t)child, &to_go)) {
			seq_anim_body->seq_index++;
		}
	}

	if (anim_body->t == anim_body->d) {
		seq_anim_body->seq_index = 0;
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}
static bool _ui_anim_spawn_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	return false;
}
