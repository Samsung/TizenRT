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

#ifndef __UI_MAP_H__
#define __UI_MAP_H__

#include <stdbool.h>
#include <araui/ui_commons.h>
#include "libs/ui_list.h"

typedef struct {
	uint32_t key;
	void *value;
} key_value_pair_t;

typedef struct {
	ui_list_t *data;
	size_t hash_capacity;
} ui_map_t;

ui_error_t ui_map_init(ui_map_t *map, size_t capacity);
ui_error_t ui_map_deinit(ui_map_t *map);
ui_error_t ui_map_push(ui_map_t *map, uint32_t key, void *item);
void *ui_map_get(ui_map_t *map, uint32_t key);

#endif
