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
#include "ui_debug.h"
#include "ui_request_callback.h"
#include "ui_core_internal.h"
#include "ui_animation_internal.h"
#include "ui_commons_internal.h"
#include "utils/easing_fn.h"
#include <vec/vec.h>

static void _ui_anim_destroy_func(void *userdata);
static bool _ui_anim_move_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_opacity_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_rotate_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_scale_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_delay_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_sequence_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static bool _ui_anim_spawn_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
static int _comp_anim_duration(const void *a, const void *b);

static ui_anim_body_queue_t g_anim_body_queue;

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
	switch (intrp_type) {
	case UI_INTRP_EASE_IN_QUAD:
		body->intrp_func = ease_in_quad;
		break;
	case UI_INTRP_EASE_INOUT_QUAD:
		body->intrp_func = ease_inout_quad;
		break;
	case UI_INTRP_EASE_OUT_QUAD:
		body->intrp_func = ease_out_quad;
		break;
	case UI_INTRP_LINEAR:
		body->intrp_func = ease_linear;
		break;
	}
	return (ui_anim_t)body;
}

ui_anim_t ui_opacity_anim_create(uint32_t from_opacity, uint32_t to_opacity, uint32_t duration, ui_intrp_type_t intrp_type)
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
	switch (intrp_type) {
	case UI_INTRP_EASE_IN_QUAD:
		body->intrp_func = ease_in_quad;
		break;
	case UI_INTRP_EASE_INOUT_QUAD:
		body->intrp_func = ease_inout_quad;
		break;
	case UI_INTRP_EASE_OUT_QUAD:
		body->intrp_func = ease_out_quad;
		break;
	case UI_INTRP_LINEAR:
		body->intrp_func = ease_linear;
		break;
	}

	return (ui_anim_t)body;
}

ui_anim_t ui_rotate_anim_create(int32_t from_degree, int32_t to_degree, uint32_t duration, ui_intrp_type_t intrp_type)
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
	switch (intrp_type) {
	case UI_INTRP_EASE_IN_QUAD:
		body->intrp_func = ease_in_quad;
		break;
	case UI_INTRP_EASE_INOUT_QUAD:
		body->intrp_func = ease_inout_quad;
		break;
	case UI_INTRP_EASE_OUT_QUAD:
		body->intrp_func = ease_out_quad;
		break;
	case UI_INTRP_LINEAR:
		body->intrp_func = ease_linear;
		break;
	}

	return (ui_anim_t)body;
}

ui_anim_t ui_scale_anim_create(uint32_t from_scale, uint32_t to_scale, uint32_t duration, ui_intrp_type_t intrp_type)
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
	switch (intrp_type) {
	case UI_INTRP_EASE_IN_QUAD:
		body->intrp_func = ease_in_quad;
		break;
	case UI_INTRP_EASE_INOUT_QUAD:
		body->intrp_func = ease_inout_quad;
		break;
	case UI_INTRP_EASE_OUT_QUAD:
		body->intrp_func = ease_out_quad;
		break;
	case UI_INTRP_LINEAR:
		body->intrp_func = ease_linear;
		break;
	}

	return (ui_anim_t)body;
}

ui_anim_t ui_delay_anim_create(uint32_t duration)
{
	ui_delay_anim_body_t *body;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_delay_anim_body_t *)UI_ALLOC(sizeof(ui_delay_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_delay_anim_body_t));
	ui_anim_init((ui_anim_body_t *)body, UI_DELAY_ANIM, duration);

	return (ui_anim_t)body;
}

ui_anim_t ui_sequence_anim_create(ui_anim_t anim, ...)
{
	ui_sequence_anim_body_t *body;
	uint32_t duration = 0;
	ui_anim_t *iter;
	va_list ap;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!anim) {
		UI_LOGE("error: invalid parameter\n");
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
	
	body->index = 0;

	ui_anim_init((ui_anim_body_t *)body, UI_SEQUENCE_ANIM, duration);

	return (ui_anim_t)body;
}

ui_anim_t ui_spawn_anim_create(ui_anim_t anim, ...)
{
	ui_spawn_anim_body_t *body;
	ui_anim_t *iter;
	va_list ap;
	uint32_t duration = 0;
	
	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	if (!anim) {
		UI_LOGE("error: invalid parameter\n");
	}

	body = (ui_spawn_anim_body_t *)UI_ALLOC(sizeof(ui_spawn_anim_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_spawn_anim_body_t));

	va_start(ap, anim);
	vec_init(&body->spawn);

	iter = (ui_anim_t *)anim;
	while (iter) {
		vec_push(&body->spawn, iter);
		if (((ui_anim_body_t *)iter)->d > duration) {
			duration = ((ui_anim_body_t *)iter)->d;
		}
		iter = (ui_anim_t *)va_arg(ap, ui_anim_t);
	}
	vec_sort(&body->spawn, _comp_anim_duration);
	body->index = 0;

	ui_anim_init((ui_anim_body_t *)body, UI_SPAWN_ANIM, duration);

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

	if (ui_request_callback(_ui_anim_destroy_func, body) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_anim_destroy_func(void *userdata)
{
	int iter;
	ui_anim_body_t *anim;
	ui_anim_body_t *child;
	ui_sequence_anim_body_t *sequence_body;
	ui_spawn_anim_body_t *spawn_body;

	if (!userdata) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	anim = (ui_anim_body_t *)userdata;

	ui_anim_queue_init();
	ui_anim_queue_enqueue(anim);

	while (!ui_anim_is_queue_empty()) {
		anim = ui_anim_queue_dequeue();

		if (!anim) {
			UI_LOGE("error: current animation is NULL\n");
			break;
		}

		switch (anim->type)	{
		case UI_SEQUENCE_ANIM:
			sequence_body = (ui_sequence_anim_body_t *)anim;
			vec_foreach(&sequence_body->sequence, child, iter) {
				ui_anim_queue_enqueue(child);
			}
			vec_deinit(&sequence_body->sequence);
			break;

		case UI_SPAWN_ANIM:
			spawn_body = (ui_spawn_anim_body_t *)anim;
			vec_foreach(&spawn_body->spawn, child, iter) {
				ui_anim_queue_enqueue(child);
			}
			vec_deinit(&spawn_body->spawn);
			break;

		default:
			break;
		}
		anim->func = UI_NULL;
		UI_FREE(anim);
	}
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
	case UI_DELAY_ANIM:
		body->func = _ui_anim_delay_func;
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
	ui_anim_body_t *anim_body;
	ui_move_anim_body_t *move_anim_body;
	uint32_t remain_time;
	int32_t intrp_x;
	int32_t intrp_y;

	anim_body = (ui_anim_body_t *)anim;
	move_anim_body = (ui_move_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;

	intrp_x = (int32_t)move_anim_body->intrp_func(anim_body->t, move_anim_body->from_coord.x, move_anim_body->to_coord.x - move_anim_body->from_coord.x, anim_body->d);
	intrp_y = (int32_t)move_anim_body->intrp_func(anim_body->t, move_anim_body->from_coord.y, move_anim_body->to_coord.y - move_anim_body->from_coord.y, anim_body->d);

	if (ui_widget_set_position_sync((ui_widget_body_t *)widget, intrp_x, intrp_y) != UI_OK) {
		UI_LOGE("Failed to set widget position!\n");
	}

	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_opacity_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_anim_body_t *anim_body;
	uint32_t remain_time;

	anim_body = (ui_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_rotate_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_anim_body_t *anim_body;
	ui_rotate_anim_body_t *rotate_anim_body;
	uint32_t remain_time;
	int32_t intrp_degree;

	anim_body = (ui_anim_body_t *)anim;
	rotate_anim_body = (ui_rotate_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;

	intrp_degree = rotate_anim_body->intrp_func(anim_body->t, rotate_anim_body->from_degree, rotate_anim_body->to_degree - rotate_anim_body->from_degree, anim_body->d);

	if (ui_widget_set_rotation_sync((ui_widget_body_t *)widget, intrp_degree) != UI_OK) {
		UI_LOGE("Failed to set widget rotation!\n");
	}
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_scale_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_anim_body_t *anim_body;
	ui_scale_anim_body_t *scale_anim_body;
	uint32_t remain_time;
	uint32_t intrp_scale;

	anim_body = (ui_anim_body_t *)anim;
	scale_anim_body = (ui_scale_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;

	intrp_scale = scale_anim_body->intrp_func(anim_body->t, scale_anim_body->from_scale, scale_anim_body->to_scale - scale_anim_body->from_scale, anim_body->d);

	if (ui_widget_set_scale_sync((ui_widget_body_t *)widget, intrp_scale, intrp_scale) != UI_OK) {
		UI_LOGE("Failed to set widget scale!\n");
	}
	
	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_delay_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_anim_body_t *anim_body;
	uint32_t remain_time;

	anim_body = (ui_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;

	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}

static bool _ui_anim_sequence_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_anim_body_t *anim_body;
	ui_sequence_anim_body_t *sequence_anim_body;
	ui_anim_body_t *child;
	uint32_t remain_time;

	anim_body = (ui_anim_body_t *)anim;
	sequence_anim_body = (ui_sequence_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;

	while (remain_time > 0) {
		child = sequence_anim_body->sequence.data[sequence_anim_body->index];

		if (child->func(widget, (ui_anim_t)child, &remain_time)) {
			sequence_anim_body->index++;
		}
	}

	if (anim_body->t == anim_body->d) {
		sequence_anim_body->index = 0;
		anim_body->t = 0;
		return true;
	} else {
		return false;
	}
}
static bool _ui_anim_spawn_func(ui_widget_t widget, ui_anim_t anim, uint32_t *dt)
{
	ui_anim_body_t *anim_body;
	ui_spawn_anim_body_t *spawn_anim_body;
	ui_anim_body_t *child;
	uint32_t remain_time;
	uint32_t i;
	uint32_t temp;

	anim_body = (ui_anim_body_t *)anim;
	spawn_anim_body = (ui_spawn_anim_body_t *)anim;
	remain_time = anim_body->d - anim_body->t;

	if (*dt <= remain_time) {
		remain_time = *dt;
	}
	anim_body->t += remain_time;
	*dt -= remain_time;

	temp = remain_time;
	for (i = spawn_anim_body->index; i < spawn_anim_body->spawn.length; i++) {
		child = spawn_anim_body->spawn.data[i];
		if (child->func(widget, (ui_anim_t)child, &remain_time)) {
			spawn_anim_body->index++;
		}
		remain_time = temp;
	}

	if (anim_body->t == anim_body->d) {
		anim_body->t = 0;
		spawn_anim_body->index = 0;
		return true;
	} else {
		return false;
	}
}

static int _comp_anim_duration(const void *a, const void *b)
{
	return ((ui_anim_body_t *)(*(long *)a))->d - ((ui_anim_body_t *)(*(long *)b))->d;
}

void ui_anim_queue_init(void)
{
	g_anim_body_queue.start = g_anim_body_queue.end = 0;
}

bool ui_anim_is_queue_empty(void)
{
	if (g_anim_body_queue.start == g_anim_body_queue.end) {
		return true;
	}

	return false;
}

void ui_anim_queue_enqueue(ui_anim_body_t *body)
{
	g_anim_body_queue.queue[g_anim_body_queue.end++] = body;
}

ui_anim_body_t *ui_anim_queue_dequeue(void)
{
	return g_anim_body_queue.queue[g_anim_body_queue.start++];
}
