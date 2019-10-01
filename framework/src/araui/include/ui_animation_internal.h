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

#ifndef __UI_ANIMATION_INTERNAL_H__
#define __UI_ANIMATION_INTERNAL_H__

#include <tinyara/config.h>
#include <stdint.h>
#include <vec/vec.h>

#define CONFIG_UI_MAX_ANIM_NUM         100

typedef struct ui_anim_body_s ui_anim_body_t;
typedef bool (*ui_anim_func)(ui_widget_t widget, ui_anim_t anim, uint32_t *dt);
typedef float (*ui_intrp_func)(float t, float b, float c, float d);

typedef enum {
	UI_MOVE_ANIM,
	UI_ROTATE_ANIM,
	UI_SCALE_ANIM,
	UI_OPACITY_ANIM,
	UI_DELAY_ANIM,
	UI_SEQUENCE_ANIM,
	UI_SPAWN_ANIM
} ui_anim_type_t;

struct ui_anim_body_s {
	ui_anim_type_t type;
	ui_anim_func func;
	uint32_t d;
	uint32_t t;
};

typedef struct {
	ui_anim_body_t base;
	ui_coord_t from_coord;
	ui_coord_t to_coord;
	ui_intrp_func intrp_func;
} ui_move_anim_body_t;

typedef struct {
	ui_anim_body_t base;
	int32_t from_degree;
	int32_t to_degree;
	ui_intrp_func intrp_func;
} ui_rotate_anim_body_t;

typedef struct {
	ui_anim_body_t base;
	int32_t from_scale;
	int32_t to_scale;
	ui_intrp_func intrp_func;
} ui_scale_anim_body_t;

typedef struct {
	ui_anim_body_t base;
	int32_t from_opacity;
	int32_t to_opacity;
	ui_intrp_func intrp_func;
} ui_opacity_anim_body_t;

typedef struct {
	ui_anim_body_t base;
} ui_delay_anim_body_t;

typedef struct {
	ui_anim_body_t base;
	vec_void_t sequence;
	uint32_t index;
} ui_sequence_anim_body_t;

typedef struct {
	ui_anim_body_t base;
	vec_void_t spawn;
	uint32_t index;
} ui_spawn_anim_body_t;

typedef struct {
	ui_anim_body_t *queue[CONFIG_UI_MAX_ANIM_NUM];
	int start;
	int end;
} ui_anim_body_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

void ui_anim_init(ui_anim_body_t *body, ui_anim_type_t type, uint32_t duration);

void ui_anim_queue_init(void);
bool ui_anim_is_queue_empty(void);
void ui_anim_queue_enqueue(ui_anim_body_t *body);
ui_anim_body_t *ui_anim_queue_dequeue(void);

#ifdef __cplusplus
}
#endif

#endif
