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

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <araui/ui_commons.h>
#include <araui/ui_commons.h>
#include "ui_commons_internal.h"
#include "ui_debug.h"

#define UI_FREAD_RETRY_COUNT 32

ui_rect_t ui_rect_intersect(ui_rect_t r1, ui_rect_t r2)
{
	ui_rect_t ret = {0, 0, 0, 0};

	if (r1.x > (r2.x + r2.width)) {
		return ret;
	}
	if ((r1.x + r1.width) < r2.x) {
		return ret;
	}
	if (r1.y > (r2.y + r2.height)) {
		return ret;
	}
	if ((r1.y + r1.height) < r2.y) {
		return ret;
	}

	ret.x = UI_MAX(r1.x, r2.x);
	ret.y = UI_MAX(r1.y, r2.y);
	ret.width = UI_MIN(r1.x + r1.width, r2.x + r2.width) - ret.x;
	ret.height = UI_MIN(r1.y + r1.height, r2.y + r2.height) - ret.y;

	return ret;
}

ui_rect_t ui_get_contain_rect(ui_rect_t r1, ui_rect_t r2)
{
	ui_rect_t ret = {0, 0, 0, 0};

	if (r1.width < 0 || r1.height < 0 || r2.width < 0 || r2.height < 0) {
		return ret;
	}

	ret.x = UI_MIN(r1.x, r2.x);
	ret.y = UI_MIN(r1.y, r2.y);
	ret.width = UI_MAX(r1.x + r1.width, r2.x + r2.width) - ret.x;
	ret.height = UI_MAX(r1.y + r1.height, r2.y + r2.height) - ret.y;

	return ret;
}

bool ui_coord_inside_rect(ui_coord_t coord, ui_rect_t rect)
{
	if ((coord.x >= rect.x) && (coord.x < (rect.x + rect.width)) &&
		(coord.y >= rect.y) && (coord.y < (rect.y + rect.height))) {
		return true;
	}

	return false;
}

void ui_fread(void *ptr, size_t size, size_t n_items, FILE *stream)
{
	size_t ret = 0;
	size_t remain_items = n_items;

	while (remain_items > 0) {
		ret = fread(ptr + size * (n_items - remain_items), size, remain_items, stream);
		remain_items -= ret;

		if (!ret) {
			UI_LOGE("error: fread failed! errno: %d\n", get_errno());
			return;
		}
	}
}
