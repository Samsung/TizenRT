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


/**
 * keyval_array.c
 * Implementation of keyval_array object
 */

#include "keyval_array.h"
#include "keyval.h"
#include "keyval_type.h"
#include "bundle.h"
#include "bundle_log.h"

#include <stdlib.h>
#include <string.h>

static keyval_method_collection_t method = {
	(keyval_method_free_t) keyval_array_free,
};

keyval_array_t *keyval_array_new(keyval_array_t *kva, const char *key, const int type, const void **array_val, const unsigned int len)
{
	int must_free_obj;
	must_free_obj = kva ? 0 : 1;

	if (!kva) {
		kva = calloc(1, sizeof(keyval_array_t));
		if (unlikely(NULL == kva)) {
			return NULL;
		}
	}

	// keyval setting
	keyval_t *kv = keyval_new((keyval_t *)kva, key, type, NULL, 0);
	if (unlikely(NULL == kv)) {
			return NULL;
	}

	kv->type = kv->type | BUNDLE_TYPE_ARRAY;

	kva->len = len;

	// Set array value, if exist
	if (kva->array_val) {
		if (must_free_obj) keyval_array_free(kva, 1);
		return NULL;
	}
	kva->array_val = calloc(len, sizeof(void *));
	if (!(kva->array_val)) {
		keyval_array_free(kva, 1);
		return NULL;
	}
	// array_element_size
	kva->array_element_size = calloc(len, sizeof(size_t));
	if (!(kva->array_element_size)) {
		keyval_array_free(kva, 1);
		return NULL;
	}
	// If avaliable, copy array val
	if (array_val
		&& keyval_type_is_measurable(type)
		&& keyval_type_get_measure_size_func(type)) {
		// array_val have original data array. copy it!

		if (keyval_array_copy_array((keyval_array_t*)kv,
					(void**)array_val,
					len,
					keyval_type_get_measure_size_func(type))
				) {
			keyval_array_free(kva, 1);
			return NULL;
		}
	}

	// Set methods
	kv->method = &method;

	return kva;
}

void keyval_array_free(keyval_array_t *kva, int do_free_object)
{
	if (!kva) return;

	// free keyval_array elements
	free(kva->array_element_size);
	int i;
	for (i = 0; i < kva->len; i++) {
		if (kva->array_val[i]) free(kva->array_val[i]);
	}
	free(kva->array_val);
	
	// free parent
	keyval_free((keyval_t *)kva, 0);

	// free object
	if (do_free_object) free(kva);
}

int keyval_array_copy_array(keyval_array_t *kva, void **array_val, unsigned int array_len, size_t (*measure_val_len)(void *val))
{
	keyval_t *kv = (keyval_t *)kva;

	// Get measure_size function of the value type
	keyval_type_measure_size_func_t measure_size = keyval_type_get_measure_size_func(kv->type);
	if (!measure_size) return -1;
	
	// Copy each array item
	int i;
	for (i = 0; i < array_len; i++) {
		kva->array_val[i] = malloc(measure_size(array_val[i]));
		if (!(kva->array_val[i])) {
			goto cleanup_exit;
		}
		memcpy(kva->array_val[i], array_val[i], measure_size(array_val[i]));
		kva->array_element_size[i] = measure_size(array_val[i]);
	}
	return 0;

cleanup_exit:
	for (i = 0; i < array_len; i++) {
		if (kva->array_val[i]) {
			free(kva->array_val[i]);
			kva->array_val[i] = NULL;
		}
	}
	return -1;
}

int keyval_array_set_element(keyval_array_t *kva, int idx, void *val, size_t size)
{
	if (kva->array_val[idx]) {	// An element is already exist in the idx!
		if (!val) {	// val==NULL means 'Free this element!' 
			free(kva->array_val[idx]);
			kva->array_val[idx] = NULL;
			kva->array_element_size[idx] = 0;
		} else {
			// Error case!
			return BUNDLE_ERROR_INVALID_PARAMETER;
		}
	} else {
		// Normal case. Copy value into the array.
		kva->array_val[idx] = malloc(size);
		if (!(kva->array_val[idx])) {
			return BUNDLE_ERROR_OUT_OF_MEMORY;
		}
		if (val) {
			memcpy(kva->array_val[idx], val, size);	// val
			kva->array_element_size[idx] = size;	// size
		}
	}

	return BUNDLE_ERROR_NONE;
}

int keyval_array_get_data(keyval_array_t *kva, int *type,
		void ***array_val, unsigned int *len, size_t **array_element_size)
{
	if (!kva)
		return BUNDLE_ERROR_INVALID_PARAMETER;
	keyval_t *kv = (keyval_t *)kva;
	if (!keyval_type_is_array(kv->type))
		return BUNDLE_ERROR_INVALID_PARAMETER;
	
	// Return values
	if (type) *type = kv->type;
	if (array_val) *array_val = kva->array_val;
	if (len) *len = kva->len;
	if (array_element_size) *array_element_size = kva->array_element_size;

	return BUNDLE_ERROR_NONE;
}
