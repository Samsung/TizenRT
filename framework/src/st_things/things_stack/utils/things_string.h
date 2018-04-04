/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef THINGS_STRING_H_
#define THINGS_STRING_H_

#include <stddef.h>

#define MAX_BUF_LEN 512
#define MAX_INPUT_QUERY_LEN 100

#ifndef ENABLE_THINGS_MALLOC

#define things_strdup(x) strdup(x)

#else

char *things_strdup(const char *str);

#endif

char *things_clone_string(const char *str);
void things_free_str_array(char **ptr, size_t size);
int things_string_compare(const char *src, const char *dst);
int things_string_duplicate(const char *src, char **dst);
void things_string_concat(char **target, char *attach);
int things_string_hex_to_int(const char *hex, int *num);
int things_get_id_value_from_query(char id_value[], char *input_query, int size);
char *things_strcat(char *dest, size_t dest_size, const char *src);
char *things_strncpy(char *destination, const char *source, size_t num);

#endif // THINGS_STRING_H_
