/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "st_things_util.h"

void *util_malloc(size_t size)
{
	RET_NULL_IF_PARAM_EXPR_IS_TRUE(size < 1);

	return malloc(size);
}

void *util_calloc(size_t num, size_t size)
{
	RET_NULL_IF_PARAM_EXPR_IS_TRUE(num < 1);
	RET_NULL_IF_PARAM_EXPR_IS_TRUE(size < 1);

	return calloc(num, size);
}

void util_free(void *ptr)
{
	free(ptr);
}

char *util_clone_string(const char *str)
{
	RET_NULL_IF_PARAM_IS_NULL(str);

	int len = strlen(str);
	char *dest_str = (char *)util_malloc(sizeof(char) * len + 1);
	if (NULL != dest_str) {
		strncpy(dest_str, str, len + 1);
	} else {
		ST_LOG(ST_ERROR, "Memory allocation failed.");
	}

	return dest_str;
}

void util_free_str_array(char **ptr, size_t size)
{
	RET_IF_PARAM_IS_NULL(ptr);
	RET_IF_PARAM_EXPR_IS_TRUE(size < 1);

	for (size_t index = 0; index < size; index++) {
		free(ptr[index]);
	}
	free(ptr);
}
