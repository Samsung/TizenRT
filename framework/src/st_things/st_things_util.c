/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
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
#include "sdkapi_logger.h"

void *util_malloc(size_t size)
{
	if (0 == size) {
		SDKAPI_LOG(SDKAPI_ERROR, "Invalid paramter.");
		return NULL;
	}

	return malloc(size);
}

void *util_calloc(size_t num, size_t size)
{
	if (0 == size || 0 == num) {
		SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
		return NULL;
	}

	return calloc(num, size);
}

void util_free(void *ptr)
{
	free(ptr);
}

char *util_clone_string(const char *src_str)
{
	if (NULL == src_str) {
		SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter.");
		return NULL;
	}

	int len = strlen(src_str);
	char *dest_str = (char *)util_malloc(sizeof(char) * len + 1);
	if (NULL != dest_str) {
		strncpy(dest_str, src_str, len + 1);
	} else {
		SDKAPI_LOG(SDKAPI_ERROR, "Memory allocation failed.");
	}

	return dest_str;
}

void util_free_str_array(char **ptr, size_t size)
{
	if (NULL == ptr) {
		SDKAPI_LOG(SDKAPI_ERROR, "Invalid pointer.");
		return;
	}

	for (size_t index = 0; index < size; index++) {
		free(ptr[index]);
	}
	free(ptr);
}
