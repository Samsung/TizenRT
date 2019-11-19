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
#include <app/bundle.h>
#include "bundle_log.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

static keyval_method_collection_t method = {
	(keyval_method_free_t)keyval_array_free,
	(keyval_method_compare_t)keyval_array_compare,
	(keyval_method_get_encoded_size_t)keyval_array_get_encoded_size,
	(keyval_method_encode_t)keyval_array_encode,
	(keyval_method_decode_t)keyval_array_decode};

keyval_array_t *
keyval_array_new(keyval_array_t *kva, const char *key, const int type, const void **array_val, const unsigned int len)
{
	int must_free_obj;
	must_free_obj = kva ? 0 : 1;

	if (!kva) {
		kva = calloc(1, sizeof(keyval_array_t));
		if (unlikely(NULL == kva)) {
			errno = ENOMEM;
			return NULL;
		}
	}

	// keyval setting
	keyval_t *kv = keyval_new((keyval_t *)kva, key, type, NULL, 0);
	if (unlikely(NULL == kv)) {
		errno = ENOMEM;
		return NULL;
	}

	kv->type = kv->type | BUNDLE_TYPE_ARRAY;

	kva->len = len;

	// Set array value, if exist
	if (kva->array_val) {
		errno = EINVAL;
		if (must_free_obj)
			keyval_array_free(kva, 1);
		return NULL;
	}
	kva->array_val = calloc(len, sizeof(void *));
	if (!(kva->array_val)) {
		errno = ENOMEM;
		keyval_array_free(kva, 1);
		return NULL;
	}
	// array_element_size
	kva->array_element_size = calloc(len, sizeof(size_t));
	if (!(kva->array_element_size)) {
		errno = ENOMEM;
		keyval_array_free(kva, 1);
		return NULL;
	}
	// If avaliable, copy array val
	if (array_val && keyval_type_is_measurable(type) && keyval_type_get_measure_size_func(type)) {
		// array_val have original data array. copy it!

		if (keyval_array_copy_array((keyval_array_t *)kv,
									(void **)array_val,
									len,
									keyval_type_get_measure_size_func(type))) {
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
	if (!kva)
		return;

	// free keyval_array elements
	free(kva->array_element_size);
	int i;
	for (i = 0; i < kva->len; i++) {
		if (kva->array_val[i])
			free(kva->array_val[i]);
	}
	free(kva->array_val);

	// free parent
	keyval_free((keyval_t *)kva, 0);

	// free object
	if (do_free_object)
		free(kva);
}

int keyval_array_compare(keyval_array_t *kva1, keyval_array_t *kva2)
{
	keyval_t *kv1, *kv2;
	if (!kva1 || !kva2)
		return -1;

	kv1 = (keyval_t *)kva1;
	kv2 = (keyval_t *)kva2;

	if (0 != strcmp(kv1->key, kv2->key))
		return 1;
	if (kv1->type != kv2->type)
		return 1;
	if (kva1->len != kva2->len)
		return 1;
	int i;
	for (i = 0; i < kva1->len; i++) {
		if (kva1->array_val[i] == NULL && kva2->array_val[i] == NULL)
			continue;
		if (kva1->array_val[i] == NULL || kva2->array_val[i] == NULL)
			return 1;
		if (0 != memcmp(kva1->array_val[i], kva2->array_val[i], kva1->array_element_size[i]))
			return 1;
	}

	return 0;
}

int keyval_array_copy_array(keyval_array_t *kva, void **array_val, unsigned int array_len, size_t (*measure_val_len)(void *val))
{
	keyval_t *kv = (keyval_t *)kva;

	// Get measure_size function of the value type
	keyval_type_measure_size_func_t measure_size = keyval_type_get_measure_size_func(kv->type);
	if (!measure_size)
		return -1;

	// Copy each array item
	int i;
	for (i = 0; i < array_len; i++) {
		kva->array_val[i] = malloc(measure_size(array_val[i]));
		if (!(kva->array_val[i])) {
			errno = ENOMEM;
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

int keyval_array_is_idx_valid(keyval_array_t *kva, int idx)
{
	//keyval_t *kv = (keyval_t *)kva;
	if (kva && kva->len > idx && 0 <= idx)
		return 1;
	return 0;
}

int keyval_array_set_element(keyval_array_t *kva, int idx, void *val, size_t size)
{
	if (kva->array_val[idx]) { // An element is already exist in the idx!
		if (!val) {			   // val==NULL means 'Free this element!'
			free(kva->array_val[idx]);
			kva->array_val[idx] = NULL;
			kva->array_element_size[idx] = 0;
		} else {
			// Error case!
			errno = EINVAL;
			return -1;
		}
	} else {
		// Normal case. Copy value into the array.
		kva->array_val[idx] = malloc(size);
		if (!(kva->array_val[idx])) {
			errno = ENOMEM;
			return -1;
		}
		if (val) {
			memcpy(kva->array_val[idx], val, size); // val
			kva->array_element_size[idx] = size;	// size
		}
	}

	return 0;
}

int keyval_array_get_data(keyval_array_t *kva, int *type,
						  void ***array_val, unsigned int *len, size_t **array_element_size)
{
	if (!kva)
		return -EINVAL;
	keyval_t *kv = (keyval_t *)kva;
	if (!keyval_type_is_array(kv->type))
		return -EINVAL;

	// Return values
	if (type)
		*type = kv->type;
	if (array_val)
		*array_val = kva->array_val;
	if (len)
		*len = kva->len;
	if (array_element_size)
		*array_element_size = kva->array_element_size;

	return 0;
}

size_t
keyval_array_get_encoded_size(keyval_array_t *kva)
{
	size_t sum_array_element_size = 0;
	int i;
	for (i = 0; i < kva->len; i++) {
		sum_array_element_size += kva->array_element_size[i];
	}
	size_t encoded_size = sizeof(size_t)					   // total size
						  + sizeof(int)						   // type
						  + sizeof(size_t)					   // keysize
						  + strlen(((keyval_t *)kva)->key) + 1 // key (+ null byte)
						  + sizeof(int)						   // len
						  + kva->len * sizeof(size_t)		   // array_element_size
						  + sum_array_element_size;

	return encoded_size;
}

size_t
keyval_array_encode(keyval_array_t *kva, void **byte, size_t *byte_len)
{
	keyval_t *kv = (keyval_t *)kva;
	int i;

	// Calculate memory size for kva
	static const size_t sz_type = sizeof(int);
	static const size_t sz_keysize = sizeof(size_t);
	size_t sz_key = strlen(kv->key) + 1;
	static const unsigned int sz_len = sizeof(int);
	size_t sz_array_element_size = kva->len * sizeof(size_t);
	size_t sz_array_val = 0;
	for (i = 0; i < kva->len; i++) {
		sz_array_val += kva->array_element_size[i];
	}

	// Allocate memory
	*byte_len = keyval_array_get_encoded_size(kva);
	*byte = calloc(1, *byte_len);
	if (!*byte)
		return 0;

	// Copy data
	unsigned char *p = *byte;

	memcpy(p, byte_len, sizeof(size_t));
	p += sizeof(size_t);
	memcpy(p, &(kv->type), sz_type);
	p += sz_type;
	memcpy(p, &sz_key, sz_keysize);
	p += sz_keysize;
	memcpy(p, kv->key, sz_key);
	p += sz_key;
	memcpy(p, &(kva->len), sz_len);
	p += sz_len;
	memcpy(p, kva->array_element_size, sz_array_element_size);
	p += sz_array_element_size;
	for (i = 0; i < kva->len; i++) {
		memcpy(p, kva->array_val[i], kva->array_element_size[i]);
		p += kva->array_element_size[i];
	}

	return *byte_len;
}

size_t
keyval_array_decode(void *byte, keyval_array_t **kva)
{
	static const size_t sz_byte_len = sizeof(size_t);
	static const size_t sz_type = sizeof(int);
	static const size_t sz_keysize = sizeof(size_t);
	static const int sz_len = sizeof(unsigned int);

	unsigned char *p = byte;

	// Get data
	size_t byte_len = *((size_t *)p);
	p += sz_byte_len;
	int type = *((int *)p);
	p += sz_type;
	size_t keysize = *((size_t *)p);
	p += sz_keysize;
	char *key = (char *)p;
	p += keysize;
	unsigned int len = *((unsigned int *)p);
	p += sz_len;
	size_t *array_element_size = (size_t *)p;
	p += sizeof(size_t) * len;
	void *array_val = (void *)p;

	*kva = keyval_array_new(NULL, key, type, NULL, len);
	int i;
	size_t elem_size = 0;
	for (i = 0; i < len; i++) {
		elem_size += i ? array_element_size[i - 1] : 0;
		if (keyval_array_set_element(*kva, i, (void *)(array_val + elem_size), array_element_size[i])) {
			keyval_array_free(*kva, 1);
			*kva = NULL;
			return 0;
		}
	}

	return byte_len;
}
