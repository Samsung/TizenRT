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

#ifndef __UI_LIST_H__
#define __UI_LIST_H__

#include <sys/types.h>
#include <stdbool.h>
#include <araui/ui_commons.h>

typedef struct ui_list_node_s {
	struct ui_list_node_s *prev;
	struct ui_list_node_s *next;
	void *data;
} ui_list_node_t;

typedef struct {
	ui_list_node_t *head;
	ui_list_node_t *tail;
	size_t size;
} ui_list_t;

ui_error_t ui_list_init(ui_list_t *list);
ui_error_t ui_list_deinit(ui_list_t *list);
ui_error_t ui_list_push_front(ui_list_t *list, void *item);
ui_error_t ui_list_push_back(ui_list_t *list, void *item);
ui_error_t ui_list_remove(ui_list_t *list, void *item);
ui_error_t ui_list_item_moveto_front(ui_list_t *list, void *item);
bool ui_list_empty(ui_list_t *list);

void *ui_list_front(ui_list_t *list);
void *ui_list_back(ui_list_t *list);
ui_error_t ui_list_remove_front(ui_list_t *list);
ui_error_t ui_list_remove_back(ui_list_t *list);

ui_list_node_t *ui_list_loop_begin(ui_list_t *list);
ui_list_node_t *ui_list_loop_end(ui_list_t *list);

#endif
