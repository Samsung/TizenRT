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
 * @defgroup PAINTER Painter
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_PAINTER_H__
#define __UI_PAINTER_H__

#include <stdint.h>
#include <araui/ui_commons.h>

/**
 * @brief ui_painter_draw_line()
 *
 * Draw a line from (start_x, start_y) coordinate to (end_x, end_y) coordinate with the given color.
 *
 * @param[in] start_x x coordinate of the start point
 * @param[in] start_y y coordinate of the start point
 * @param[in] end_x x coordinate of the end point
 * @param[in] end_y y coordinate of the end point
 * @param[in] color Color of the line
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_line(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y, ui_color_t color);

/**
 * @brief ui_painter_draw_hline()
 *
 * Draw a horizontal line with the given length and the color.
 *
 * @param[in] x x coordinate of the start point
 * @param[in] y y coordinate of the start point
 * @param[in] length Length of the horizontal line
 * @param[in] color Color of the horizontal line
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_hline(int32_t x, int32_t y, int32_t length, ui_color_t color);

/**
 * @brief ui_painter_draw_vline()
 *
 * Draw a vertical line with the given length and the color.
 *
 * @param[in] x x coordinate of the start point
 * @param[in] y y coordinate of the start point
 * @param[in] length Length of the vertical line
 * @param[in] color Color of the vertical line
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_vline(int32_t x, int32_t y, int32_t length, ui_color_t color);

/**
 * @brief ui_painter_draw_rect()
 *
 * Draw a rectangle with the given information.
 *
 * @param[in] x x coordinate of the top left corner
 * @param[in] y y coordinate of the top left corner
 * @param[in] width Width of the rectangle
 * @param[in] height Height of the rectangle
 * @param[in] color Color of the rectangle
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_rect(int32_t x, int32_t y, int32_t width, int32_t height, ui_color_t color);

/**
 * @brief ui_painter_draw_circle()
 *
 * Draw a circle with the given information.
 *
 * @param[in] center_x x coordinate of the center of the circle
 * @param[in] center_y y coordinate of the center of the circle
 * @param[in] radius Radius of the circle (Pixel)
 * @param[in] color Color of the circle
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_circle(int32_t center_x, int32_t center_y, int32_t radius, ui_color_t color);

/**
 * @brief ui_painter_draw_ellipse()
 *
 * Draw an ellipse with the given information.
 *
 * @param[in] center_x x coordinate of the center of the ellipse
 * @param[in] center_y y coordinate of the center of the ellipse
 * @param[in] radius_x x Radius of the ellipse (Pixel)
 * @param[in] radius_y y Radius of the ellipse (Pixel)
 * @param[in] color Color of the ellipse
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_ellipse(int32_t center_x, int32_t center_y, int32_t radius_x, int32_t radius_y, ui_color_t color);

/**
 * @brief ui_painter_draw_arc()
 *
 * Draw an arc with the given information.
 *
 * @param[in] center_x x coordinate of the center of the arc
 * @param[in] center_y y coordinate of the center of the arc
 * @param[in] radius Radius of the arc (Pixel)
 * @param[in] start_radian Angle of the start (Radian)\n
 * The reference to the start_radian is the positive x-axis and clockwise.
 * @param[in] sweep_radian Angle of the end (Radian)\n
 * The reference to the sweep_radian is the start_radian and clockwise.
 * @param[in] color Color of the arc
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_draw_arc(int32_t center_x, int32_t center_y, int32_t radius, double start_radian, double sweep_radian, ui_color_t color);

/**
 * @brief ui_painter_fill_figure()
 *
 * Fill the inside of figure with given color.
 *
 * @param[in] x x coordinate of the center of the arc
 * @param[in] y y coordinate of the center of the arc
 * @param[in] color Color of the arc
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 */
ui_error_t ui_painter_fill_figure(int32_t x, int32_t y, ui_color_t color);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of PAINTER group
