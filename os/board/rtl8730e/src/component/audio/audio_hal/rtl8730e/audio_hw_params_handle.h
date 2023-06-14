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
#ifndef AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_PARAMS_HANDLE_H
#define AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_PARAMS_HANDLE_H

#include "platform_stdlib.h"

struct string_cell {
	char *key;
	char *value;
	struct string_cell *next;
};

struct string_cell *string_cells_create_from_str(const char *_string);
void string_cells_destroy(struct string_cell *cell_head);

int string_cells_has_key(struct string_cell *cell_head, const char *key);
int string_cells_get_str(struct string_cell *cell_head, const char *key, char *out_val, int len);
int string_cells_get_int(struct string_cell *cell_head, const char *key, int *out_val);

#endif