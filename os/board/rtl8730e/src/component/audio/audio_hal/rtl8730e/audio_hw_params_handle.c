/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "errno.h"
#include "audio_hw_debug.h"
#include "audio_hw_params_handle.h"

char *param_strdup(const char *s)
{
	if (s == NULL) {
		return NULL;
	}

	const char *sin = s;
	size_t len = strlen(sin) + 1;
	char *new = (char *) malloc(len);
	if (new == NULL) {
		return NULL;
	}

	return (char *) memcpy(new, sin, len);
}

char *param_strndup(const char *s, int count)
{
	const char *sin = s;
	size_t len = strnlen(sin, (count)) + 1;
	HAL_AUDIO_VERBOSE("count:%d, len:%d", count, len);
	char *new = (char *) malloc(len);
	if (new == NULL) {
		return NULL;
	}
	new[len - 1] = '\0';
	return (char *) memcpy(new, sin, count);
}

static struct string_cell *string_cells_create(void)
{
	HAL_AUDIO_VERBOSE("malloc string cells");
	struct string_cell *cell_head = (struct string_cell *) malloc(sizeof(struct string_cell));
	cell_head->key = NULL;
	cell_head->value = NULL;
	cell_head->next = NULL;
	return cell_head;
}

static void insert_string_cell_by_last(struct string_cell *cell_head, char *key, char *value)
{
	struct string_cell *new_cell = (struct string_cell *) malloc(sizeof(struct string_cell));
	new_cell->key = param_strdup(key);
	new_cell->value = param_strdup(value);
	struct string_cell *cell_iterate = cell_head;
	while (cell_iterate->next != NULL) {
		cell_iterate = cell_iterate->next;
	}
	cell_iterate->next = new_cell;
	new_cell->next = NULL;
}

void string_cells_destroy(struct string_cell *cell_head)
{
	HAL_AUDIO_VERBOSE("%s", __FUNCTION__);
	struct string_cell *cell_iterate = cell_head;
	struct string_cell *next_cell = NULL;
	while (cell_iterate != NULL) {
		if (cell_iterate->key) {
			free(cell_iterate->key);
		}
		if (cell_iterate->value) {
			free(cell_iterate->value);
		}
		next_cell = cell_iterate->next;
		free(cell_iterate);
		cell_iterate = next_cell;
	}
}

struct string_cell *string_cells_create_from_str(const char *string)
{
	struct string_cell *cell_head;
	char *duped_str;
	char *one_cell_str;
	char *str_left;

	cell_head = string_cells_create();

	duped_str = param_strdup(string);
	if (!duped_str) {
		HAL_AUDIO_VERBOSE("%s string null ", __func__);
		string_cells_destroy(cell_head);
		return NULL;
	}

	one_cell_str = (char *)strtok_r(duped_str, ";", &str_left);

	while (one_cell_str && *one_cell_str) {
		char *str_after_equal = strchr(one_cell_str, '=');
		char *value = NULL;
		char *key = NULL;

		if (str_after_equal == one_cell_str) {
			HAL_AUDIO_ERROR("warning: should have =");
			one_cell_str = (char *)strtok_r(NULL, ";", &str_left);
			continue;
		}

		if (str_after_equal) {
			key = param_strndup(one_cell_str, str_after_equal - one_cell_str);
			if (*(++str_after_equal)) {
				value = param_strdup(str_after_equal);
			} else {
				HAL_AUDIO_ERROR("warning, should have value after =");
				value = param_strdup("");
			}
		} else {
			key = param_strdup(one_cell_str);
			value = param_strdup("");
		}
		if (key) {
			HAL_AUDIO_VERBOSE("key:%s\n", key);
		}
		if (value) {
			HAL_AUDIO_VERBOSE("value:%s", value);
		}
		insert_string_cell_by_last(cell_head, key, value);

		if (NULL != key) {
			free(key);
		}
		if (value) {
			free(value);
		}

		one_cell_str = (char *)strtok_r(NULL, ";", &str_left);
	}

	free(duped_str);

	return cell_head;

}

int string_cells_has_key(struct string_cell *cell_head, const char *key)
{
	struct string_cell *cell_iterate = cell_head;
	while (cell_iterate != NULL) {
		if (cell_iterate->key == NULL) {
			HAL_AUDIO_VERBOSE("%s cell_iterate->key null..", __func__);
			cell_iterate = cell_iterate->next;
			continue;
		}
		HAL_AUDIO_VERBOSE("%s cell_iterate->key:%s, key:%s", __func__, cell_iterate->key, key);
		if (!strcmp(cell_iterate->key, key)) {
			HAL_AUDIO_VERBOSE("found key");
			return TRUE;
		}
		cell_iterate = cell_iterate->next;
	}
	return FALSE;
}

int string_cells_get_str(struct string_cell *cell_head, const char *key, char *val,
						 int len)
{
	struct string_cell *cell_iterate = cell_head;
	while (cell_iterate != NULL) {
		if (cell_iterate->key == NULL) {
			HAL_AUDIO_VERBOSE("%s cell_iterate->key null..", __func__);
			cell_iterate = cell_iterate->next;
			continue;
		}
		HAL_AUDIO_VERBOSE("%s cell_iterate->key:%s,cell_iterate->value:%s, key:%s", __func__, cell_iterate->key, cell_iterate->value, key);
		if (!strcmp(cell_iterate->key, key)) {
			char *value = cell_iterate->value;
			return strlcpy(val, value, len);
		}
		cell_iterate = cell_iterate->next;
	}

	return -ENOENT;
}

int string_cells_get_int(struct string_cell *cell_head, const char *key, int *val)
{
	struct string_cell *cell_iterate = cell_head;
	while (cell_iterate != NULL) {
		if (cell_iterate->key == NULL) {
			HAL_AUDIO_VERBOSE("%s cell_iterate->key null..", __func__);
			cell_iterate = cell_iterate->next;
			continue;
		}
		HAL_AUDIO_VERBOSE("%s cell_iterate->key:%s,cell_iterate->value:%s, key:%s", __func__, cell_iterate->key, cell_iterate->value, key);
		if (!strcmp(cell_iterate->key, key)) {
			char *value = cell_iterate->value;
			if (!value) {
				return -ENOENT;
			}
			char *end;
			*val = (int)strtol(value, &end, 0);
			if (*value != '\0' && *end == '\0') {
				return 0;
			}

		}
		cell_iterate = cell_iterate->next;
	}

	return -EINVAL;
}
