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
#include <stdint.h>
#include <math.h>
#include <araui/ui_commons.h>
#include "hal/ui_hal_interface.h"
#include "libs/ui_list.h"
#include "ui_commons_internal.h"
#include "ui_log.h"

typedef struct {
	int32_t x;
	int32_t y;
} ui_fill_coord_t;

static void _ui_painter_draw_line_driv_x_axis(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y, ui_color_t color)
{
	int32_t x;
	int32_t y;
	int dx;
	int dy;
	int inc_y;
	int epsilon;

	dx = end_x - start_x;
	dy = end_y - start_y;

	if (dy < 0) {
		inc_y = -1;
		dy = -1 * dy;
	} else {
		inc_y = 1;
	}

	epsilon = 2 * dy - dx;
	y = start_y;

	x = start_x;
	while (x <= end_x) {
		ui_hal_draw_pixel(x, y, color);
		if (epsilon > 0) {
			y = y + inc_y;
			epsilon = epsilon - 2 * dx;
		} else {
			epsilon = epsilon + 2 * dy;
		}
		x++;
	}
}

static void _ui_painter_draw_line_driv_y_axis(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y, ui_color_t color)
{
	int32_t x;
	int32_t y;
	int dx;
	int dy;
	int inc_x;
	int epsilon;

	dx = end_x - start_x;
	dy = end_y - start_y;

	if (dx < 0) {
		inc_x = -1;
		dx = -1 * dx;
	} else {
		inc_x = 1;
	}

	epsilon = 2 * dx - dy;
	x = start_x;

	y = start_y;
	while (y <= end_y) {
		ui_hal_draw_pixel(x, y, color);
		if (epsilon > 0) {
			x = x + inc_x;
			epsilon = epsilon - 2 * dy;
		} else {
			epsilon = epsilon + 2 * dx;
		}
		y++;
	}
}

ui_error_t ui_painter_draw_line(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y, ui_color_t color)
{
	if (UI_ABS(end_y - start_y) < UI_ABS(end_x - start_x)) {
		if (start_x > end_x) {
			_ui_painter_draw_line_driv_x_axis(end_x, end_y, start_x, start_y, color);
		} else {
			_ui_painter_draw_line_driv_x_axis(start_x, start_y, end_x, end_y, color);
		}
	} else {
		if (start_y > end_y) {
			_ui_painter_draw_line_driv_y_axis(end_x, end_y, start_x, start_y, color);
		} else {
			_ui_painter_draw_line_driv_y_axis(start_x, start_y, end_x, end_y, color);
		}
	}

	return UI_OK;
}

ui_error_t ui_painter_draw_hline(int32_t x, int32_t y, int32_t length, ui_color_t color)
{
	if (ui_painter_draw_line(x, y, x + length - 1, y, color) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

ui_error_t ui_painter_draw_vline(int32_t x, int32_t y, int32_t length, ui_color_t color)
{
	if (ui_painter_draw_line(x, y, x, y + length - 1, color) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

ui_error_t ui_painter_draw_rect(int32_t x, int32_t y, int32_t width, int32_t height, ui_color_t color)
{
	if (ui_painter_draw_line(x, y, x + width - 1, y, color) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	if (ui_painter_draw_line(x, y + 1, x, y + height - 2, color) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	if (ui_painter_draw_line(x + width - 1, y + 1, x + width - 1, y + height - 2, color) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	if (ui_painter_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

static void _ui_painter_draw_circle_pixel(int32_t center_x, int32_t center_y, int32_t x, int32_t y, ui_color_t color)
{
	ui_hal_draw_pixel(center_x + x, center_y + y, color);
	ui_hal_draw_pixel(center_x + y, center_y + x, color);
	ui_hal_draw_pixel(center_x - y, center_y + x, color);
	ui_hal_draw_pixel(center_x - x, center_y + y, color);
	ui_hal_draw_pixel(center_x - x, center_y - y, color);
	ui_hal_draw_pixel(center_x - y, center_y - x, color);
	ui_hal_draw_pixel(center_x + y, center_y - x, color);
	ui_hal_draw_pixel(center_x + x, center_y - y, color);
}

ui_error_t ui_painter_draw_circle(int32_t center_x, int32_t center_y, int32_t radius, ui_color_t color)
{
	int32_t x;
	int32_t y;
	int epsilon;

	if (radius < 0) {
		return UI_INVALID_PARAM;
	}

	x = 0;
	y = radius;
	epsilon = 5 - 4 * radius;

	_ui_painter_draw_circle_pixel(center_x, center_y, x, y, color);

	while (y >= x) {
		x++;
		if (epsilon > 0) {
			y--;
			epsilon = epsilon + 8 * (x - y) + 4;
		} else {
			epsilon = epsilon + 8 * x + 4;
		}
		_ui_painter_draw_circle_pixel(center_x, center_y, x, y, color);
	}

	return UI_OK;
}

static void _ui_painter_draw_ellipse_pixel(int32_t center_x, int32_t center_y, int32_t x, int32_t y, ui_color_t color)
{
	ui_hal_draw_pixel(center_x + x, center_y + y, color);
	ui_hal_draw_pixel(center_x - x, center_y + y, color);
	ui_hal_draw_pixel(center_x + x, center_y - y, color);
	ui_hal_draw_pixel(center_x - x, center_y - y, color);
}

ui_error_t ui_painter_draw_ellipse(int32_t center_x, int32_t center_y, int32_t radius_x, int32_t radius_y, ui_color_t color)
{
	int32_t x;
	int32_t y;
	int32_t radi_x_sq;
	int32_t radi_y_sq;
	int epsilon;

	if (radius_x < 0 || radius_y < 0) {
		return UI_INVALID_PARAM;
	}

	x = 0;
	y = radius_y;
	radi_x_sq = radius_x * radius_x;
	radi_y_sq = radius_y * radius_y;
	epsilon = 4 * radi_y_sq - 4 * radi_x_sq * radius_y + 1;

	_ui_painter_draw_ellipse_pixel(center_x, center_y, x, y, color);

	while ((radi_y_sq * x < radi_x_sq * y) && (x < radius_x)) {
		x++;
		if (epsilon >= 0) {
			y--;
			epsilon = epsilon + radi_y_sq * (2 * x + 3) + radi_x_sq * (3 - 2 * y);
		} else {
			epsilon = epsilon + radi_y_sq * (2 * x + 3);
		}
		_ui_painter_draw_ellipse_pixel(center_x, center_y, x, y, color);
	}

	while (y > 0 && y < radius_y) {
		y--;
		if (epsilon <= 0) {
			x++;
			epsilon = epsilon + radi_x_sq * (3 - 2 * y) + radi_y_sq * (2 * x + 3);
		} else {
			epsilon = epsilon + radi_x_sq * (3 - 2 * y);
		}
		_ui_painter_draw_ellipse_pixel(center_x, center_y, x, y, color);
	}

	return UI_OK;
}

static void _ui_painter_draw_arc(int32_t center_x, int32_t center_y,
	int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y,
	int32_t radius, int32_t quadrant, ui_color_t color)
{
	int32_t x;
	int32_t y;
	int epsilon;

	if (quadrant < 1 || quadrant > 4) {
		UI_LOGE("error: Invalid parameter!\n");
		return;
	}

	x = start_x;
	y = start_y;

	ui_hal_draw_pixel(center_x + x, center_y + y, color);

	if ((start_x == end_x) && (start_y == end_y)) {
		return;
	}

	switch (quadrant) {
		case 1:
			epsilon = (2 * x - 1) * (2 * x - 1) + 4 * (y + 1) * (y + 1) - 4 * radius * radius;
			while (y <= x && y < end_y) {
				y++;
				if (epsilon > 0) {
					x--;
					epsilon = epsilon + 8 * (y - x) + 4;
				} else {
					epsilon = epsilon + 8 * y + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			epsilon = 4 * (x - 1) * (x - 1) + (2 * y + 1) * (2 * y + 1) - 4 * radius * radius;
			while (x >= end_x) {
				x--;
				if (epsilon < 0) {
					y++;
					epsilon = epsilon + 8 * (y - x) + 4;
				} else {
					epsilon = epsilon - 8 * x + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			break;
		case 2:
			epsilon = 4 * (x - 1) * (x - 1) + (2 * y - 1) * (2 * y - 1) - 4 * radius * radius;
			while (-1 * x <= y && x > end_x) {
				x--;
				if (epsilon > 0) {
					y--;
					epsilon = epsilon - 8 * (x + y) + 4;
				} else {
					epsilon = epsilon - 8 * x + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			epsilon = (2 * x - 1) * (2 * x - 1) + 4 * (y - 1) * (y - 1) - 4 * radius * radius;
			while (y >= end_y) {
				y--;
				if (epsilon < 0) {
					x--;
					epsilon = epsilon - 8 * (x + y) + 4;
				} else {
					epsilon = epsilon - 8 * y + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			break;
		case 3:
			epsilon = (2 * x + 1) * (2 * x + 1) + 4 * (y - 1) * (y - 1) - 4 * radius * radius;
			while (y >= x && y > end_y) {
				y--;
				if (epsilon > 0) {
					x++;
					epsilon = epsilon + 8 * (x - y) + 4;
				} else {
					epsilon = epsilon - 8 * y + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			epsilon = 4 * (x + 1) * (x + 1) + (2 * y -1) * (2 * y - 1) - 4 * radius * radius;
			while (x <= end_x) {
				x++;
				if (epsilon < 0) {
					y--;
					epsilon = epsilon + 8 * (x - y) + 4;
				} else {
					epsilon = epsilon + 8 * x + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			break;
		case 4:
			epsilon = 4 * (x + 1) * (x + 1) + (2 * y + 1) * (2 * y + 1) - 4 * radius * radius;
			while (x <= -1 * y && x < end_x) {
				x++;
				if (epsilon > 0) {
					y++;
					epsilon = epsilon + 8 * (x + y) + 4;
				} else {
					epsilon = epsilon + 8 * x + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			epsilon = (2 * x + 1) * (2 * x + 1) + 4 * (y + 1) * (y + 1) - 4 * radius * radius;
			while (y <= end_y) {
				y++;
				if (epsilon < 0) {
					x++;
					epsilon = epsilon + 8 * (x + y) + 4;
				} else {
					epsilon = epsilon + 8 * y + 4;
				}
				ui_hal_draw_pixel(center_x + x, center_y + y, color);
			}
			break;
		default:
			break;
	}
}

static int _ui_painter_get_quadrant(int x, int y)
{
	if (x > 0 && y >= 0) {
		return 1;
	}

	if (x <= 0 && y > 0) {
		return 2;
	}

	if (x < 0 && y <= 0) {
		return 3;
	}

	if (x >= 0 && y < 0) {
		return 4;
	}

	//TO DO: Error message will be printed and return error value (INVALID_PARAM)
	return -1;
}

static int _ui_painter_get_next_quadrant(int quadrant)
{
	switch (quadrant) {
		case 1:
			return 2;
		case 2:
			return 3;
		case 3:
			return 4;
		case 4:
			return 1;
		default:
			return -1;
	}
}

static int _ui_painter_get_arc_cover_num(int start_quadrant, int end_quadrant, double sweep_radian)
{
	int num = 1;
	int quadrant = start_quadrant;

	if (start_quadrant == end_quadrant) {
		if (sweep_radian < M_PI_2) {
			return 1;
		}
	}

	while (quadrant != end_quadrant) {
		switch (quadrant) {
			case 1:
			case 2:
			case 3:
			case 4:
				num++;
				quadrant = _ui_painter_get_next_quadrant(quadrant);
				break;
			default:
				break;
		}
	}

	return num;
}

static int _ui_painter_draw_arc_get_x_const(int quadrant)
{
	switch (quadrant) {
		case 1:
			return 0;
		case 2:
			return -1;
		case 3:
			return 0;
		case 4:
			return 1;
		default:
			//TO DO: This value should be changed to represent the error.
			return -999999;
	}
}

static int _ui_painter_draw_arc_get_y_const(int quadrant)
{
	switch (quadrant) {
		case 1:
			return 1;
		case 2:
			return 0;
		case 3:
			return -1;
		case 4:
			return 0;
		default:
			//TO DO: This value should be changed to represent the error.
			return -999999;
	}
}
ui_error_t ui_painter_draw_arc(int32_t center_x, int32_t center_y, int32_t radius, double start_radian, double sweep_radian, ui_color_t color)
{
	int start_x;
	int start_y;
	int start_quadrant;
	int end_x;
	int end_y;
	int end_quadrant;
	int quadrant;
	int cover;
	int s_x;
	int s_y;
	int e_x;
	int e_y;

	//The reference to the start_radian is the positive x-axis and clockwise
	if (radius < 0 || sweep_radian < 0) {
		//TO DO: POC GUI Framework only supports drawing the arc in clockwise direction
		return UI_INVALID_PARAM;
	}

	if (sweep_radian >= 2 * M_PI) {
		ui_painter_draw_circle(center_x, center_y, radius, color);
	}

	start_x = (int)rint(radius * cos(start_radian));
	start_y = (int)rint(radius * sin(start_radian));
	start_quadrant = _ui_painter_get_quadrant(start_x, start_y);
	if (start_quadrant < 0) {
		return UI_INVALID_PARAM;
	}

	end_x = (int)rint(radius *cos(start_radian + sweep_radian));
	end_y = (int)rint(radius *sin(start_radian + sweep_radian));
	end_quadrant = _ui_painter_get_quadrant(end_x, end_y);
	if (end_quadrant < 0) {
		return UI_INVALID_PARAM;
	}

	cover = _ui_painter_get_arc_cover_num(start_quadrant, end_quadrant, sweep_radian);
	quadrant = start_quadrant;
	s_x = start_x;
	s_y = start_y;

	while (cover > 1) {
		e_x = _ui_painter_draw_arc_get_x_const(quadrant) * radius;
		e_y = _ui_painter_draw_arc_get_y_const(quadrant) * radius;
		_ui_painter_draw_arc(center_x, center_y, s_x, s_y, e_x, e_y, radius, quadrant, color);
		s_x = e_x;
		s_y = e_y;
		quadrant = _ui_painter_get_next_quadrant(quadrant);
		cover--;
	}

	e_x = end_x;
	e_y = end_y;
	_ui_painter_draw_arc(center_x, center_y, s_x, s_y, e_x, e_y, radius, quadrant, color);

	return UI_OK;
}

static void _ui_painter_fill_pixel(int32_t x, int32_t y, ui_color_t color, ui_list_t *list)
{
	ui_fill_coord_t *curr;

	ui_hal_draw_pixel(x, y, color);
	curr = (ui_fill_coord_t *)malloc(sizeof(ui_fill_coord_t));
	if (!curr) {
		UI_LOGE("error: Out of memory!\n");
		return;
	}
	curr->x = x;
	curr->y = y;
	if (ui_list_push_back(list, (void *)curr) != UI_OK) {
		UI_LOGE("error: Failed to push back!\n");
	}
}

ui_error_t ui_painter_fill_figure(int32_t x, int32_t y, ui_color_t color)
{
	ui_fill_coord_t *curr;
	ui_list_t list_filled;
	ui_color_t background_color;
	int32_t selected_x;
	int32_t selected_y;

	if (x < 0 || x >= CONFIG_UI_DISPLAY_WIDTH || y < 0 || y >= CONFIG_UI_DISPLAY_HEIGHT) {
		return UI_INVALID_PARAM;
	}

	ui_list_init(&list_filled);
	background_color = ui_hal_get_pixel_color(x, y);
	_ui_painter_fill_pixel(x, y, color, &list_filled);

	while (!ui_list_empty(&list_filled)) {
		curr = (ui_fill_coord_t *)ui_list_front(&list_filled);
		selected_x = curr->x;
		selected_y = curr->y;
		(void)ui_list_remove_front(&list_filled);
		UI_FREE(curr);
		if ((selected_x - 1 > 0) && (ui_hal_get_pixel_color(selected_x - 1, selected_y) == background_color)) {
			_ui_painter_fill_pixel(selected_x - 1, selected_y, color, &list_filled);
		}
		if ((selected_x + 1 < CONFIG_UI_DISPLAY_WIDTH) && (ui_hal_get_pixel_color(selected_x + 1, selected_y) == background_color)) {
			_ui_painter_fill_pixel(selected_x + 1, selected_y, color, &list_filled);
		}
		if ((selected_y - 1 > 0) && (ui_hal_get_pixel_color(selected_x, selected_y - 1) == background_color)) {
			_ui_painter_fill_pixel(selected_x, selected_y - 1, color, &list_filled);
		}
		if ((selected_y + 1 < CONFIG_UI_DISPLAY_HEIGHT) && (ui_hal_get_pixel_color(selected_x, selected_y + 1) == background_color)) {
			_ui_painter_fill_pixel(selected_x, selected_y + 1, color, &list_filled);
		}
	}

	if (ui_list_deinit(&list_filled) != UI_OK) {
		UI_LOGE("error: Failed to deinitiate list!\n");
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

