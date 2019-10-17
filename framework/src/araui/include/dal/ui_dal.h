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
#ifndef __UI_DAL_H__
#define __UI_DAL_H__

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>
#include <araui/ui_commons.h>
#include <araui/ui_asset.h>

#define UI_DAL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ui_dal_init()
 *
 * Initialize the base interface.
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
UI_DAL ui_error_t ui_dal_init(void);

/**
 * @brief ui_dal_deinit()
 *
 * Deinitialize the HAL implementation.
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
UI_DAL ui_error_t ui_dal_deinit(void);

/**
 * @brief ui_dal_redraw()
 *
 * Redraw a rectangular region of the screen.
 *
 * @param[in] x x coordinate of the rectangular region to redraw
 * @param[in] y y coordinate of the rectangular region to redraw
 * @param[in] width Width of the rectangular region to redraw
 * @param[in] height Height of the rectangular region to redraw
 *
 */
UI_DAL void ui_dal_redraw(int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * @brief ui_dal_clear()
 *
 * Clear the screen with zero value
 *
 */
UI_DAL void ui_dal_clear(void);

/**
 * @brief ui_dal_put_pixel_rgba8888()
 *
 * Put a pixel to (x, y) coordinate with the given color.
 *
 * @param[in] x x coordinate of the pixel
 * @param[in] y y coordinate of the pixel
 * @param[in] color Color of the pixel
 *
 */
UI_DAL void ui_dal_put_pixel_rgba8888(int32_t x, int32_t y, ui_color_t color);

/**
 * @brief ui_dal_put_pixel_rgb888()
 *
 * Put a pixel to (x, y) coordinate with the given color.
 *
 * @param[in] x x coordinate of the pixel
 * @param[in] y y coordinate of the pixel
 * @param[in] color Color of the pixel
 *
 */
UI_DAL void ui_dal_put_pixel_rgb888(int32_t x, int32_t y, ui_color_t color);

/**
 * @brief ui_dal_set_viewport()
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
UI_DAL ui_error_t ui_dal_set_viewport(int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * @brief ui_dal_get_viewport()
 *
 * Get current viewport rectangular region.
 *
 * @return On success, a normal ui_rect_t value is returned. On failure, empty region(0, 0, 0, 0) is returned.
 *
 */
UI_DAL ui_rect_t ui_dal_get_viewport(void);

#if defined(CONFIG_UI_ENABLE_TOUCH)

/**
 * @brief ui_dal_get_touch()
 *
 * Get current touch status.
 *
 * @param[out] pressed True if the touchscreen is pressed
 * @param[out] coord Coordinate of touch position
 *
 * @return After get the touch event, if the touch event queue is empty, false is returned. Otherwise, true is returned.
 *
 */
UI_DAL bool ui_dal_get_touch(bool *pressed, ui_coord_t *coord);

#endif // CONFIG_UI_ENABLE_TOUCH

#if defined(CONFIG_UI_ENABLE_HW_ACC)

#if defined(CONFIG_UI_ENABLE_HW_ACC_CHROM_ART)

/**
 * @brief ui_dal_draw_bitmap_dma2d()
 *
 * Draw the original image with the given information via DMA2D acceleration.
 * This function is called when the image has no any transform such as translation, rotation, scaling.
 * If the image has transform, the image is drew by renderer via ui_dal_put_pixel function.
 *
 * @param[in] x x coordinate of the origin point
 * @param[in] y y coordinate of the origin point
 * @param[in] bitmap Bitmap buffer pointer
 * @param[in] width Width of the bitmap
 * @param[in] height Height of the bitmap
 * @param[in] pf Pixel format of the bitmap
 *
 */
UI_DAL void ui_dal_draw_bitmap_dma2d(int32_t x, int32_t y,
    uint8_t *bitmap, int32_t width, int32_t height, ui_pixel_format_t pf);

#endif // CONFIG_UI_ENABLE_HW_ACC_CHROM_ART

#endif // CONFIG_UI_ENABLE_HW_ACC

#ifdef __cplusplus
}
#endif

#endif
