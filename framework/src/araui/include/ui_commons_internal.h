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

#ifndef __UI_COMMON_INTERNAL_H__
#define __UI_COMMON_INTERNAL_H__

#include <stdio.h>
#include <sys/types.h>
#include <araui/ui_commons.h>

#define UI_ABS(x)           ((x) < (0) ? (-(x)) : (x))
#define UI_MAX(a, b)        ((a) > (b) ? (a) : (b))
#define UI_MIN(a, b)        ((a) > (b) ? (b) : (a))
#define UI_MAX4(a, b, c, d) (UI_MAX(UI_MAX((a), (b)), UI_MAX((c), (d))))
#define UI_MIN4(a, b, c, d) (UI_MIN(UI_MIN((a), (b)), UI_MIN((c), (d))))

#ifdef __cplusplus
extern "C" {
#endif

ui_rect_t ui_rect_intersect(ui_rect_t r1, ui_rect_t r2);

ui_rect_t ui_get_contain_rect(ui_rect_t r1, ui_rect_t r2);

bool ui_coord_inside_rect(ui_coord_t coord, ui_rect_t rect);

void ui_fread(void *ptr, size_t size, size_t n_items, FILE *stream);

#ifdef __cplusplus
}
#endif

#endif
