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
#include <araui/ui_commons.h>
#include "libs/ui_list.h"

ui_error_t ui_list_init(ui_list_t *list)
{
	if (!list) {
		return UI_INVALID_PARAM;
	}

	list->head = (ui_list_node_t *)UI_ALLOC(sizeof(ui_list_node_t));
	if (!list->head) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	list->tail = (ui_list_node_t *)UI_ALLOC(sizeof(ui_list_node_t));
	if (!list->tail) {
		UI_FREE(list->head);
		return UI_NOT_ENOUGH_MEMORY;
	}

	list->head->data = NULL;
	list->tail->data = NULL;

	list->head->prev = NULL;
	list->head->next = list->tail;

	list->tail->prev = list->head;
	list->tail->next = NULL;

	return UI_OK;
}

ui_error_t ui_list_deinit(ui_list_t *list)
{
	if (!list) {
		return UI_INVALID_PARAM;
	}

	while (!ui_list_empty(list)) {
		if (ui_list_remove_front(list) != UI_OK) {
			UI_FREE(list->head);
			UI_FREE(list->tail);
			return UI_DEINIT_FAILURE;
		}
	}

	UI_FREE(list->head);
	UI_FREE(list->tail);

	return UI_OK;
}

ui_error_t ui_list_push_front(ui_list_t *list, void *item)
{
	ui_list_node_t *node;

	if (!list || !item) {
		return UI_INVALID_PARAM;
	}

	node = (ui_list_node_t *)UI_ALLOC(sizeof(ui_list_node_t));
	if (!node) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	node->data = item;
	node->prev = list->head;
	node->next = list->head->next;
	list->head->next->prev = node;
	list->head->next = node;

	return UI_OK;
}

ui_error_t ui_list_push_back(ui_list_t *list, void *item)
{
	ui_list_node_t *node;

	if (!list || !item) {
		return UI_INVALID_PARAM;
	}

	node = (ui_list_node_t *)UI_ALLOC(sizeof(ui_list_node_t));
	if (!node) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	node->data = item;
	node->prev = list->tail->prev;
	node->next = list->tail;
	list->tail->prev->next = node;
	list->tail->prev = node;

	return UI_OK;
}

ui_error_t ui_list_remove(ui_list_t *list, void *item)
{
	ui_list_node_t *node;
	ui_list_node_t *next;

	if (!list || !item) {
		return UI_INVALID_PARAM;
	}

	node = list->head->next;
	while (node != list->tail) {
		if (node->data == item) {
			next = node->next;
			node->prev->next = node->next;
			node->next->prev = node->prev;
			UI_FREE(node);
			node = next;
			return UI_OK;
		} else {
			node = node->next;
		}
	}

	return UI_OPERATION_FAIL;
}

ui_error_t ui_list_item_moveto_front(ui_list_t *list, void *item)
{
	if (ui_list_remove(list, item) != UI_OK) {
		return UI_OPERATION_FAIL;
	}
	if (ui_list_push_front(list, item) != UI_OK) {
		return UI_OPERATION_FAIL;
	}

	return UI_OK;
}

bool ui_list_empty(ui_list_t *list)
{
	return (list->head->next == list->tail || list->tail->prev == list->head);
}

void *ui_list_front(ui_list_t *list)
{
	if (ui_list_empty(list)) {
		return NULL;
	}

	return list->head->next->data;
}

void *ui_list_back(ui_list_t *list)
{
	if (ui_list_empty(list)) {
		return NULL;
	}

	return list->tail->prev->data;
}

ui_error_t ui_list_remove_front(ui_list_t *list)
{
	ui_list_node_t *node;

	if (!list) {
		return UI_INVALID_PARAM;
	}

	if (!ui_list_empty(list)) {
		node = list->head->next;
		list->head->next = node->next;
		node->next->prev = list->head;

		UI_FREE(node);
		node = NULL;
	}

	return UI_OK;
}

ui_error_t ui_list_remove_back(ui_list_t *list)
{
	ui_list_node_t *node;

	if (!list) {
		return UI_INVALID_PARAM;
	}

	if (!ui_list_empty(list)) {
		node = list->tail->prev;
		list->tail->prev = node->prev;
		node->prev->next = list->tail;

		UI_FREE(node);
		node = NULL;
	}

	return UI_OK;
}

ui_list_node_t *ui_list_loop_begin(ui_list_t *list)
{
	if (!list) {
		return NULL;
	}

	if (ui_list_empty(list)) {
		return list->tail;
	}

	return list->head->next;
}

ui_list_node_t *ui_list_loop_end(ui_list_t *list)
{
	if (!list) {
		return NULL;
	}

	return list->tail;
}
