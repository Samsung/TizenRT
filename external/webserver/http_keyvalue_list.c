/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <tinyara/progmem.h>
#include <sys/stat.h>

#include <protocols/webserver/http_err.h>
#include <protocols/webserver/http_keyvalue_list.h>

#include "http.h"
#include "http_arch.h"
#include "http_log.h"

char *null_string = "(null)";

int http_keyvalue_list_init(struct http_keyvalue_list_t *list)
{
	HTTP_MEMSET(list, 0, sizeof(struct http_keyvalue_list_t));

	list->head = (struct http_keyvalue_t *)HTTP_MALLOC(sizeof(struct http_keyvalue_t));
	if (!list->head) {
		return HTTP_ERROR;
	}

	list->tail = (struct http_keyvalue_t *)HTTP_MALLOC(sizeof(struct http_keyvalue_t));
	if (!list->tail) {
		return HTTP_ERROR;
	}

	list->head->prev = NULL;
	list->head->next = list->tail;

	list->tail->prev = list->head;
	list->tail->next = NULL;

	return HTTP_OK;
}

int http_keyvalue_list_release(struct http_keyvalue_list_t *list)
{
	if (list->head && list->tail) {
		while (http_keyvalue_list_delete_tail(list) == HTTP_OK) {
			/* Delete all containers */
		}

		HTTP_FREE(list->head);
		HTTP_FREE(list->tail);
	}
	return HTTP_OK;
}

int http_keyvalue_list_add(struct http_keyvalue_list_t *list, const char *key, const char *value)
{
	struct http_keyvalue_t *keyvalue = NULL;

	keyvalue = (struct http_keyvalue_t *)HTTP_MALLOC(sizeof(struct http_keyvalue_t));
	if (!keyvalue) {
		HTTP_LOGE("Error: Cannot allocate keyvalue!!\n");
		return HTTP_ERROR;
	}

	HTTP_MEMSET(keyvalue, 0, sizeof(struct http_keyvalue_t));
	strncpy(keyvalue->key, key, sizeof(keyvalue->key) - 1);
	strncpy(keyvalue->value, value, sizeof(keyvalue->value) - 1);

	keyvalue->prev = list->tail->prev;
	keyvalue->next = list->tail;

	list->tail->prev->next = keyvalue;
	list->tail->prev = keyvalue;

	return HTTP_OK;
}

int http_keyvalue_list_delete_tail(struct http_keyvalue_list_t *list)
{
	if (list->tail->prev != list->head) {
		struct http_keyvalue_t *target = list->tail->prev;

		target->prev->next = target->next;
		target->next->prev = target->prev;
		HTTP_FREE(target);

		return HTTP_OK;
	}

	/* List is empty */
	return HTTP_ERROR;
}

char *http_keyvalue_list_find(struct http_keyvalue_list_t *list, const char *key)
{
	struct http_keyvalue_t *cur = list->head->next;

	while (cur != list->tail) {
		if (strcmp(key, cur->key) == 0) {
			return cur->value;
		}
		cur = cur->next;
	}

	return null_string;
}
