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

#ifndef __ST_THINGS_UTIL_H__
#define __ST_THINGS_UTIL_H__

#include <string.h>
#include <stdlib.h>
#include "st_things_logger.h"

#define RET_IF_PARAM_IS_NULL(var) \
	do { \
		if (NULL == (var)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #var " is NULL."); \
			return; \
		} \
	} while (0)

#define RET_NULL_IF_PARAM_IS_NULL(var) \
	do { \
		if (NULL == (var)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #var " is NULL."); \
			return NULL; \
		} \
	} while (0)

#define RET_FALSE_IF_PARAM_IS_NULL(var) \
	do { \
		if (NULL == (var)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #var " is NULL."); \
			return false; \
		} \
	} while (0)

#define RET_VAL_IF_PARAM_IS_NULL(var, ret_val) \
	do { \
		if (NULL == (var)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #var " is NULL."); \
			return (ret_val); \
		} \
	} while (0)

#define RET_VAL_IF_NULL(var, msg, ret_val) \
	do { \
		if (NULL == (var)) { \
			ST_LOG(ST_ERROR, msg); \
			return (ret_val); \
		} \
	} while (0)

#define RET_FALSE_IF_NULL(var, msg) \
	do { \
		if (NULL == (var)) { \
			ST_LOG(ST_ERROR, msg); \
			return false; \
		} \
	} while (0)

#define RET_IF_PARAM_EXPR_IS_TRUE(expr) \
	do { \
		if ((expr)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #expr); \
			return; \
		} \
	} while (0)

#define RET_NULL_IF_PARAM_EXPR_IS_TRUE(expr) \
	do { \
		if ((expr)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #expr); \
			return NULL; \
		} \
	} while (0)

#define RET_FALSE_IF_PARAM_EXPR_IS_TRUE(expr) \
	do { \
		if ((expr)) { \
			ST_LOG(ST_ERROR, "Invalid Parameter: " #expr); \
			return false; \
		} \
	} while (0)

#define RET_IF_EXPR_IS_TRUE(expr, msg) \
	do { \
		if ((expr)) { \
			ST_LOG(ST_ERROR, msg); \
			return; \
		} \
	} while (0)

#define RET_FALSE_IF_EXPR_IS_TRUE(expr, msg) \
	do { \
		if ((expr)) { \
			ST_LOG(ST_ERROR, msg); \
			return false; \
		} \
	} while (0)

#define RET_VAL_IF_EXPR_IS_TRUE(expr, msg, ret_val) \
	do { \
		if ((expr)) { \
			ST_LOG(ST_ERROR, msg); \
			return (ret_val); \
		} \
	} while (0)

void *util_malloc(size_t size);

void *util_calloc(size_t num, size_t size);

void util_free(void *ptr);

char *util_clone_string(const char *str);

/**
* Destroys the array items and the array.
*/
void util_free_str_array(char **ptr, size_t size);

#endif							// __ST_THINGS_UTIL_H__
