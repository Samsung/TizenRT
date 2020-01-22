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
 * @defgroup COMMONS Commons
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_COMMONS_H__
#define __UI_COMMONS_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief Global macros for AraUI Framework
 */
#define UI_NULL     (0)
#define UI_ALLOC(a) (malloc(a))
#define UI_FREE(a)  do { if (a) { free(a); a = UI_NULL; } } while (false)

/**
 * @brief Type-safe swap macro.
 */
#define UI_SWAP(a, b) \
	do { \
		typeof((a)) __swap; \
		__swap = (a); \
		(a) = (b); \
		(b) = __swap; \
	} while (false)

/**
 * @brief Generating color macros. Color will be generated as big-endian.
 */
#define UI_COLOR_ARGB8888(a, r, g, b) (((b) << 24) | ((g) << 16) | ((r) << 8) | ((a)))
#define UI_COLOR_RGBA8888(r, g, b, a) (((a) << 24) | ((b) << 16) | ((g) << 8) | ((r)))
#define UI_COLOR_RGB888(r, g, b)      (((b) << 16) | ((g) << 8) | ((r)))

/**
 * @brief Structures for extracting RGB from the hex value
 */
typedef uint32_t ui_color_t;

typedef struct {
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ui_color_argb8888_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} ui_color_rgba8888_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ui_color_rgb888_t;

/**
 * @brief Enum type of the error value returned from the AraUI Framework
 */
typedef enum {
	UI_OK                 =  0, //!< No error (Success)
	UI_INVALID_PARAM      = -1, //!< Invalid Parameter
	UI_ALREADY_RUNNING    = -2, //!< UI Core Service is already running
	UI_NOT_RUNNING        = -3, //!< UI Core Service is not running
	UI_NOT_ENOUGH_MEMORY  = -4, //!< Failed to allocate the memory due to the out of memory
	UI_INIT_FAILURE       = -5, //!< Failed to initialization
	UI_DEINIT_FAILURE     = -6, //!< Failed to release resources created during initialization
	UI_OPERATION_FAIL     = -7  //!< Operation failure
} ui_error_t;

/**
 * @brief Enum types for the align type for text or image
 *
 * @see ui_text_widget_set_align()
 */
typedef enum {
	UI_ALIGN_DEFAULT = (1 << 0) | (1 << 4),
	UI_ALIGN_LEFT    = (1 << 0),
	UI_ALIGN_CENTER  = (1 << 2),
	UI_ALIGN_RIGHT   = (1 << 3),
	UI_ALIGN_TOP     = (1 << 4),
	UI_ALIGN_MIDDLE  = (1 << 5),
	UI_ALIGN_BOTTOM  = (1 << 6)
} ui_align_t;

typedef enum {
	UI_DIRECTION_ALL,
	UI_DIRECTION_HORIZONTAL,
	UI_DIRECTION_VERTICAL
} ui_direction_t;

/**
 * @brief Enum types for the pixel format
 *
 * @see ui_image_asset_create_from_buffer()
 */
typedef enum {
	UI_PIXEL_FORMAT_UNKNOWN,
	UI_PIXEL_FORMAT_GRAY4,
	UI_PIXEL_FORMAT_GRAYA44,
	UI_PIXEL_FORMAT_RGB565,
	UI_PIXEL_FORMAT_ARGB8565,
	UI_PIXEL_FORMAT_RGBA5658,
	UI_PIXEL_FORMAT_ARGB4444,
	UI_PIXEL_FORMAT_RGBA4444,
	UI_PIXEL_FORMAT_RGB888,
	UI_PIXEL_FORMAT_ARGB8888,
	UI_PIXEL_FORMAT_RGBA8888,
	UI_PIXEL_FORMAT_A8
} ui_pixel_format_t;

/**
 * @brief Structure for representing the position (coordinate)
 *
 * @see ui_widget_set_position()
 */
typedef struct {
	int32_t x; //!< x coordinate
	int32_t y; //!< y coordinate
} ui_coord_t;

/**
 * @brief Structure for representing the information of size
 *
 * @see ui_widget_set_size()
 */
typedef struct {
	int32_t width;  //!< width
	int32_t height; //!< heignt
} ui_size_t;

/**
 * @brief Structure that represents information about a rectangular region
 */
typedef struct {
	int32_t x;      //!< x coordinate of the rectangular region
	int32_t y;      //!< y coordinate of the rectangular region
	int32_t width;  //!< width of the rectangular region
	int32_t height; //!< height of the rectangular region
} ui_rect_t;

/**
 * @brief Enumeration that represents information about the touch screen
 */
typedef enum {
	UI_TOUCH_EVENT_NONE,
	UI_TOUCH_EVENT_DOWN,
	UI_TOUCH_EVENT_MOVE,
	UI_TOUCH_EVENT_UP,
	UI_TOUCH_EVENT_CANCEL
} ui_touch_event_t;

/**
 * @brief Enumeration that represents the type of transition options
 */
typedef enum {
	UI_TRANSITION_FADE_OUT,
	UI_TRANSITION_FADE_IN,
	UI_TRANSITION_SLIDE
} ui_transition_type_t;

#endif

/** @} */ // end of COMMONS group
