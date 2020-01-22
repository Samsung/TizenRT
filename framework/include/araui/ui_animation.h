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

/**
 * @brief A handle type to control all kind of animation in the AraUI Framework
 *
 * @see ui_move_anim_create()
 */
typedef long ui_anim_t;

/**
 * @brief Enumeration that represents the type of interpolation 
 */
typedef enum {
	UI_INTRP_LINEAR,          //!< Linear
	UI_INTRP_EASE_IN_QUAD,    //!< Ease In Quadratic
	UI_INTRP_EASE_OUT_QUAD,   //!< Ease Out Quadratic
	UI_INTRP_EASE_INOUT_QUAD  //!< Ease In/Out Quadratic
} ui_intrp_type_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a move animation
 *
 * If you want to get a current coordinate of widget, use ui_widget_get_rect.
 *
 * @param[in] from_x x coordinate to move from
 * @param[in] from_y y coordinate to move from
 * @param[in] to_x x coordinate to move to
 * @param[in] to_y y coordinate to move to
 * @param[in] duration Duration of animation
 * @param[in] intrp_type Interpolation type of the animation
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 *
 * @see ui_widget_get_rect()
 */
ui_anim_t ui_move_anim_create(int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, uint32_t duration, ui_intrp_type_t intrp_type);

/**
 * @brief Create a opacity animation
 *
 * @param[in] from_opacity The opacity to start
 * @param[in] to_opacity The opacity to end
 * @param[in] duration Duration of animation
 * @param[in] intrp_type Interpolation of animation
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 */
ui_anim_t ui_opacity_anim_create(uint32_t from_opacity, uint32_t to_opacity, uint32_t duration, ui_intrp_type_t intrp_type);

/**
 * @brief Create a rotate animation
 *
 * @param[in] from_degree The degree to start
 * @param[in] to_degree The degree to end
 * @param[in] duration Duration of animation
 * @param[in] intrp_type Interpolation of animation
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 */
ui_anim_t ui_rotate_anim_create(int32_t from_degree, int32_t to_degree, uint32_t duration, ui_intrp_type_t intrp_type);

/**
 * @brief Create a scale animation
 *
 * @param[in] from_scale The scale to start
 * @param[in] to_scale The scale to end
 * @param[in] duration Duration of animation
 * @param[in] intrp_type Interpolation of animation
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 */
ui_anim_t ui_scale_anim_create(uint32_t from_scale, uint32_t to_scale, uint32_t duration, ui_intrp_type_t intrp_type);

/**
 * @brief Create a delay animation
 *
 * @param[in] duration Duration to delay
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 */
ui_anim_t ui_delay_anim_create(uint32_t duration);

/**
 * @brief Create a sequence animation
 * 
 * The last argument must be NULL, it means end of arguments.
 * Sequence animation will run each animation for each duration sequentially.
 *
 * @param[in] anim First Animation
 * @param[in] ... Animations that will be executed sequentailly
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 */
ui_anim_t ui_sequence_anim_create(ui_anim_t anim, ...);

/**
 * @brief Create a spawn animation
 * 
 * The last argument must be NULL, it means end of arguments.
 * Spawn animation will run every animation at once for time given by core.
 *
 * @param[in] anim An animation
 * @param[in] ... Animations that will be executed simultaneously
 * @return On success, the handle of animation is returned. On failure, UI_NULL is returned.
 */
ui_anim_t ui_spawn_anim_create(ui_anim_t anim, ...);

/**
 * @brief Destroy the created animation and release the allocated memory.
 *
 * When the sequence or spawn animation is destroyed, the child animations of animation are also destroyed.
 * All animations can be destroyed with this function
 *
 * @param[in] anim Handle of the animation to be destroyed
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_move_anim_create()
 * @see ui_opacity_anim_create()
 * @see ui_rotate_anim_create()
 * @see ui_scale_anim_create()
 * @see ui_sequence_anim_create()
 * @see ui_spawn_anim_create()
 */
ui_error_t ui_anim_destroy(ui_anim_t anim);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of ANIMATION group
