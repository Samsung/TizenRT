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

#include "ui_widget_internal.h"

void ui_render_widget(ui_widget_body_t *widget);

void ui_render_image_widget(ui_image_widget_body_t *widget);

void ui_render_text_widget(ui_text_widget_body_t *widget);

void ui_render_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, ui_color_t color);

void ui_matrix_copy(float dest_mat[3][3], float src_mat[3][3]);

void ui_matrix_multiply(float dest_mat[3][3], float left_mat[3][3], float right_mat[3][3]);
