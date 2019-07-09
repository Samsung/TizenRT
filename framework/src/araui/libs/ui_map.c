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

#include <stdlib.h>
#include <sys/types.h>
#include "libs/ui_map.h"
#include "ui_log.h"

ui_error_t ui_map_init(ui_map_t *map, size_t hash_capacity)
{
	size_t i = 0;

	map->data = (ui_list_t *)UI_ALLOC(sizeof(ui_list_t) * hash_capacity);
	if (!map->data) {
		return UI_NOT_ENOUGH_MEMORY;
	}
	map->hash_capacity = hash_capacity;

	for (i = 0; i < hash_capacity; i++) {
		if (ui_list_init(&map->data[i]) != UI_OK) {
			UI_LOGE("error: Failed to initiate list!\n");
			return UI_OPERATION_FAIL;
		}
	}

	return UI_OK;
}

ui_error_t ui_map_deinit(ui_map_t *map)
{
	size_t i = 0;
	ui_list_node_t *node;
	key_value_pair_t *pair;

	for (i = 0; i < map->hash_capacity; i++) {
		node = ui_list_loop_begin(&map->data[i]);
		while (node != ui_list_loop_end(&map->data[i])) {
			pair = (key_value_pair_t *)node->data;
			UI_FREE(pair);
			node = node->next;
		}

		if (ui_list_deinit(&map->data[i]) != UI_OK) {
			UI_LOGE("error: Failed to initiate list!\n");
			return UI_OPERATION_FAIL;
		}
	}

	UI_FREE(map->data);
	map->data = NULL;
	map->hash_capacity = 0;

	return UI_OK;
}

inline static uint32_t get_hash_index(ui_map_t *map, uint32_t key)
{
	return (key % map->hash_capacity);
}

ui_error_t ui_map_push(ui_map_t *map, uint32_t key, void *value)
{
	key_value_pair_t *pair = (key_value_pair_t *)UI_ALLOC(sizeof(key_value_pair_t));
	if (!pair) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	pair->key = key;
	pair->value = value;

	if (ui_list_push_back(&map->data[get_hash_index(map, key)], pair) != UI_OK) {
		UI_LOGE("error: Failed to push back!\n");
		UI_FREE(pair);
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

void *ui_map_get(ui_map_t *map, uint32_t key)
{
	key_value_pair_t *pair = NULL;
	ui_list_t *list = &(map->data[get_hash_index(map, key)]);

	ui_list_node_t *node = ui_list_loop_begin(list);
	while (node != ui_list_loop_end(list)) {
		pair = (key_value_pair_t *)node->data;

		if (pair->key == key) {
			return pair->value;
		}

		node = node->next;
	}

	return NULL;
}
