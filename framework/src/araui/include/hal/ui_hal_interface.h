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

#ifndef __UI_HAL_INTERFACE_H__
#define __UI_HAL_INTERFACE_H__

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>
#include <araui/ui_commons.h>
#include <araui/ui_asset.h>

#define INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ui_hal_init()
 *
 * Initialize the HAL implementation.
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
INTERFACE ui_error_t ui_hal_init(void);

/**
 * @brief ui_hal_deinit()
 *
 * Deinitialize the HAL implementation.
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
INTERFACE ui_error_t ui_hal_deinit(void);

/**
 * @brief ui_hal_redraw()
 *
 * Redraw a rectangular region of the screen.
 *
 * @param[in] x x coordinate of the rectangular region to redraw
 * @param[in] y y coordinate of the rectangular region to redraw
 * @param[in] width Width of the rectangular region to redraw
 * @param[in] height Height of the rectangular region to redraw
 *
 */
INTERFACE void ui_hal_redraw(int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * @brief ui_hal_clear()
 *
 * Clear the screen with the given color
 *
 * @param[in] color Color to clear the screen
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
INTERFACE ui_error_t ui_hal_clear(ui_color_t color);

/**
 * @brief ui_hal_draw_pixel()
 *
 * Draw a pixel to (x, y) coordinate with the given color. The color contains the alpha value.
 *
 * @param[in] x x coordinate of the pixel
 * @param[in] y y coordinate of the pixel
 * @param[in] color Color of the pixel
 *
 */
extern INTERFACE void ui_hal_draw_pixel(int32_t x, int32_t y, ui_color_t color);

/**
 * @brief ui_hal_get_pixel_color()
 *
 * Get a color of the specific pixel (x, y).
 *
 * @param[in] x x coordinate of the pixel
 * @param[in] y y coordinate of the pixel
 *
 * @return On success, the color of pixel is returned.
 *
 */
INTERFACE ui_color_t ui_hal_get_pixel_color(int32_t x, int32_t y);

/**
 * @brief ui_hal_draw_image()
 *
 * Draw an image with the given information.
 *
 * @param[in] x x coordinate of the origin point
 * @param[in] y y coordinate of the origin point
 * @param[in] image Image asset handle
 *
 */
INTERFACE void ui_hal_draw_image(int32_t x, int32_t y, ui_asset_t image);

/**
 * @brief ui_hal_draw_cropped_image()
 *
 * Draw an image with the given information.
 *
 * @param[in] x x coordinate of the origin point
 * @param[in] y y coordinate of the origin point
 * @param[in] image Image asset handle
 * @param[in] crop_rect Cropped area of the image asset
 *
 */
INTERFACE void ui_hal_draw_cropped_image(int32_t x, int32_t y, ui_asset_t image, ui_rect_t crop_rect);
/**
 * @brief ui_hal_draw_cropped_image()
 *
 * @param[in] x x coordinate of the origin point
 * @param[in] y y coordinate of the origin point
 * @param[in] image Image asset handle
 * @param[in] crop_rect Cropped area of the image asset
 * @param[in] resized_width Resized width of the image
 * @param[in] resized_width Resized height of the image
 *
 */
INTERFACE void ui_hal_draw_cropped_resized_image(int32_t x, int32_t y, ui_asset_t image, ui_rect_t crop_rect, int32_t resized_width, int32_t resized_height);

/**
 * @brief ui_hal_set_viewport()
 *
 * Set the rectangular region of the viewport.
 * All of drawing features can draw only inside of the viewport region.
 *
 * @param[in] x x coordinate of the rectangular region of the viewport
 * @param[in] y y coordinate of the rectangular region of the viewport
 * @param[in] width Width of the rectangular region of the viewport
 * @param[in] height Height of the rectangular region of the viewport
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
INTERFACE ui_error_t ui_hal_set_viewport(int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * @brief ui_hal_get_viewport()
 *
 * Get current viewport rectangular region.
 *
 * @return On success, a normal ui_rect_t value is returned. On failure, empty region(0, 0, 0, 0) is returned.
 *
 */
INTERFACE ui_rect_t ui_hal_get_viewport(void);

#if defined(CONFIG_UI_TOUCH_INTERFACE)

/**
 * @brief ui_hal_get_touch()
 *
 * Get current touch status.
 *
 * @param[out] pressed True if the touchscreen is pressed
 * @param[out] coord Coordinate of touch position
 *
 * @return After get the touch event, if the touch event queue is empty, false is returned. Otherwise, true is returned.
 *
 */
INTERFACE bool ui_hal_get_touch(bool *pressed, ui_coord_t *coord);

#endif // CONFIG_UI_TOUCH_INTERFACE

#ifdef __cplusplus
}
#endif

#endif
