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

/**
 * @defgroup ANIMATION Animation
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_ANIMATION_H__
#define __UI_ANIMATION_H__

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>

typedef long ui_anim_t;

typedef enum {
	UI_INTRP_LINEAR,          //!< Linear
	UI_INTRP_EASE_IN_QUAD,    //!< Ease In Quadratic
	UI_INTRP_EASE_OUT_QUAD,   //!< Ease Out Quadratic
	UI_INTRP_EASE_INOUT_QUAD  //!< Ease In/Out Quadratic
} ui_intrp_type_t;

#ifdef __cplusplus
extern "C" {
#endif

ui_anim_t ui_move_anim_create(int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, uint32_t duration, ui_intrp_type_t intrp_type);
ui_anim_t ui_opacity_anim_create(uint32_t from_opacity, uint32_t to_opacity, uint32_t duration);
ui_anim_t ui_rotate_anim_create(int32_t from_degree, int32_t to_degree, uint32_t duration);
ui_anim_t ui_scale_anim_create(uint32_t from_scale, uint32_t to_scale, uint32_t duration);
ui_anim_t ui_sequence_anim_create(ui_anim_t anim, ...);
ui_anim_t ui_spawn_anim_create(ui_anim_t anim, ...);

ui_error_t ui_anim_destory(ui_anim_t anim);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of ANIMATION group
