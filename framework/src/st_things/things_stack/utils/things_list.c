/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "logging/things_logger.h"
#include "things_malloc.h"
#include "things_list.h"

#define TAG "[things_list]"

static void *erase(list_s *p_list, things_node_s *p_prev_node, things_node_s *p_del_node);

// begin
things_node_s *begin(list_s *p_list)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return p_list->head;
}

void *pop(list_s *p_list)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	void *item = NULL;
	if (p_list == NULL) {
		THINGS_LOG_D(TAG, "p_list is NULL.");
		return NULL;
	}

	pthread_mutex_lock(&(p_list->q_mutex));
	things_node_s *node = p_list->begin(p_list);
	if (node != NULL) {
		item = erase(p_list, NULL, node);
	}
	pthread_mutex_unlock(&(p_list->q_mutex));

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return item;
}

// end
things_node_s *end(list_s *p_list)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return p_list->tail;
}

// size
int list_size(list_s *p_list)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	pthread_mutex_lock(&(p_list->q_mutex));
	int n_size = p_list->list_size;
	pthread_mutex_unlock(&(p_list->q_mutex));

//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return n_size;
}

// front
things_node_s *front(list_s *p_list)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return p_list->begin(p_list);
}

static void link_node(list_s *p_list, things_node_s *p_prev_node, things_node_s *p_new_node)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

//    pthread_mutex_lock(&(p_list->q_mutex));
	if (NULL == p_prev_node) {
		p_list->head = p_new_node;
		// Invalid Param
		// THINGS_LOG_D(TAG, THINGS_INVALID_PARAM);
		// return;
	} else {
		p_prev_node->next = p_new_node;
	}
	p_list->size++;

//    pthread_mutex_unlock(&(p_list->q_mutex));
//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

// insert
void insert(list_s *p_list, void *item)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	things_node_s *p_new_node = (things_node_s *) things_malloc(sizeof(things_node_s));
	if (NULL == p_new_node) {
		//THINGS_LOG_E(TAG, THINGS_MEMORY_ERROR);
		return;
	}
	p_new_node->item = item;
	p_new_node->next = NULL;

	pthread_mutex_lock(&(p_list->q_mutex));
	things_node_s *p_node = p_list->begin(p_list);
	if (NULL != p_node) {
		while (p_node->next) {
			// THINGS_LOG_E(TAG, "p_node->item->key = %s", (char*)(((RequestInfo*)p_node->item)->frameId));
			p_node = p_node->next;
		}

		link_node(p_list, p_node, p_new_node);
	} else {
		p_list->head = p_new_node;
	}

	p_list->tail = p_new_node;
	pthread_mutex_unlock(&(p_list->q_mutex));
//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

// erase
static void *erase(list_s *p_list, things_node_s *p_prev_node, things_node_s *p_del_node)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (p_del_node == p_list->head) {
		p_list->head = p_del_node->next;
		p_prev_node = p_list->head;
	} else {
		p_prev_node->next = p_del_node->next;
	}

	if (p_del_node == p_list->tail) {
		p_list->tail = p_prev_node;
	}

	void *item = p_del_node->item;
	p_del_node->item = NULL;
	p_del_node->next = NULL;
	things_free(p_del_node);
	p_list->size--;

//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return item;
}

void *erase_by_key(list_s *p_list, key_compare compare, const void *key)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	void *item = NULL;

	pthread_mutex_lock(&(p_list->q_mutex));
	things_node_s *p_node = p_list->begin(p_list);

	if (NULL != p_node) {
		// Matched key with head's key.
		if (compare(p_node->item, key)) {
			THINGS_LOG_D(TAG, "Delete Head");
			item = erase(p_list, p_list->head, p_node);
		} else {
			while (p_node->next) {
				if (compare(p_node->next->item, key)) {
					item = erase(p_list, p_node, p_node->next);
					// THINGS_LOG_D(TAG, "src key = %s dst_key = %s", pSrc_key, dst_key);
					// exit(0);
					break;
				}

				p_node = p_node->next;
			}
		}
	}

	pthread_mutex_unlock(&(p_list->q_mutex));
//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return item;
}

// clear
void clear(list_s *p_list)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	pthread_mutex_lock(&(p_list->q_mutex));
	things_node_s *p_node = p_list->begin(p_list);
	if (NULL != p_node) {
		while (p_node->next) {
			erase(p_list, p_node, p_node->next);
			// p_node = p_node->next;
		}
	}

	p_list->head = p_list->tail = NULL;
	pthread_mutex_unlock(&(p_list->q_mutex));

//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

// find
things_node_s *find_by_key(list_s *p_list, key_compare compare, const void *key)
{
//    THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	pthread_mutex_lock(&(p_list->q_mutex));
	things_node_s *p_node = p_list->begin(p_list);

	if (NULL != p_node) {
		while (p_node) {
			if (compare(p_node->item, key)) {
				break;
			}
			p_node = p_node->next;
		}
	}
	pthread_mutex_unlock(&(p_list->q_mutex));

//    THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return p_node;
}

list_s *create_list(void)
{
	// THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	list_s *list = (list_s *) things_malloc(sizeof(list_s));
	if (list == NULL) {
		THINGS_LOG_E(TAG, "memory allocation is failed.");
		return NULL;
	}

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;

	list->begin = &begin;
	list->pop = &pop;
	list->end = &end;
	list->list_size = &list_size;
	list->front = &front;
	list->insert = &insert;
	list->erase_by_key = &erase_by_key;
	list->clear = &clear;
	list->find_by_key = &find_by_key;

	pthread_mutex_init(&(list->q_mutex), NULL);

	// THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return list;
}

void terminate_list(list_s *list)
{
	if (list == NULL) {
		return;
	}

	list->clear(list);
	pthread_mutex_destroy(&(list->q_mutex));
	things_free(list);
}
