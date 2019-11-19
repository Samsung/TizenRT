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

/**
 * keyval_array.h
 *
 * keyval_array object
 */

#include "keyval.h"

typedef struct keyval_array_t {
	struct keyval_t kv; // Inherits keyval_t

	unsigned int len;			// length of array_val
	size_t *array_element_size; // Array of size of each element
	void **array_val;			// Array

} keyval_array_t;

keyval_array_t *keyval_array_new(keyval_array_t *kva, const char *key, const int type, const void **array_val, const unsigned int len);
void keyval_array_free(keyval_array_t *kva, int do_free_object);
int keyval_array_compare(keyval_array_t *kva1, keyval_array_t *kva2);
size_t keyval_array_get_encoded_size(keyval_array_t *kva);
size_t keyval_array_encode(keyval_array_t *kva, void **byte, size_t *byte_len);
size_t keyval_array_decode(void *byte, keyval_array_t **kva);
int keyval_array_copy_array(keyval_array_t *kva, void **array_val, unsigned int array_len, size_t (*measure_val_len)(void *val));
int keyval_array_get_data(keyval_array_t *kva, int *type, void ***array_val, unsigned int *len, size_t **array_element_size);
int keyval_array_set_element(keyval_array_t *kva, int idx, void *val, size_t size);
