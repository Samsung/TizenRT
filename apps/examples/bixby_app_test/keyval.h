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
/*
 * bundle
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee <airjany@samsung.com>, Sewook Park <sewook7.park@samsung.com>,
 * Jaeho Lee <jaeho81.lee@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef __KEYVAL_H__
#define __KEYVAL_H__

/**
 * keyval.h
 *
 * keyval object
 */

#include <stddef.h>

// ADT: object
typedef struct keyval_t keyval_t;

// Object methods
typedef struct keyval_method_collection_t keyval_method_collection_t;

typedef void (*keyval_method_free_t)(keyval_t *kv, int do_free_object);

struct keyval_method_collection_t {
	keyval_method_free_t free;
};

struct keyval_t {
	int type;
	char *key;	// To be freed.
	void *val;	// To be freed.
	size_t size;	// Size of a single value.
	struct keyval_t *next;

	keyval_method_collection_t *method;

};

keyval_t * keyval_new(keyval_t *kv, const char *key, const int type, const void *val, const size_t size);
void keyval_free(keyval_t *kv, int do_free_object);
int keyval_get_data(keyval_t *kv, int *type, void **val, size_t *size);

#endif /* __KEYVAL_H__ */

