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
 * bundle.c
 */

#include "bundle.h"
//#include "bundle_internal.h"
#include "keyval.h"
#include "keyval_array.h"
#include "keyval_type.h"
#include "bundle_log.h"
//#include <glib.h>

#include <stdlib.h>		/* calloc, free */
#include <string.h>		/* strdup */

#define CHECKSUM_LENGTH 32
#define TAG_IMPORT_EXPORT_CHECK "`zaybxcwdveuftgsh`"
/* ADT */
struct _bundle_t {
	keyval_t *kv_head;
};

/**
 * Find a kv from bundle
 */
static keyval_t *_bundle_find_kv(bundle *b, const char *key)
{
	keyval_t *kv;
	if (NULL == b) {
		return NULL;
	}
	if (NULL == key) {
		return NULL;
	}

	kv = b->kv_head;
	while (kv != NULL) {
		if (0 == strcmp(key, kv->key)) return kv;
		kv = kv->next;
	}
	/* Not found */
	return NULL;
}

/**
 * Append kv into bundle
 */
static int _bundle_append_kv(bundle *b, keyval_t *new_kv)
{
	keyval_t *kv;

	if (NULL == b->kv_head) b->kv_head = new_kv;
	else {
		kv = b->kv_head;
		while (NULL != kv->next) kv = kv->next;
		kv->next = new_kv;
	}
	return BUNDLE_ERROR_NONE;
}

static int _bundle_add_kv(bundle *b, const char *key, const void *val, const size_t size, const int type, const unsigned int len)
{
	/* basic value check */
	if (NULL == b)
		return BUNDLE_ERROR_INVALID_PARAMETER;
	if (NULL == key)
		return BUNDLE_ERROR_INVALID_PARAMETER;
	if (0 == strlen(key))
		return BUNDLE_ERROR_INVALID_PARAMETER;

	keyval_t *kv = _bundle_find_kv(b, key);
	if (kv) {	/* Key already exists */
		return BUNDLE_ERROR_KEY_EXISTS;
	}

	keyval_t *new_kv = NULL;
	if (keyval_type_is_array(type)) {
		// array type
		keyval_array_t *kva = keyval_array_new(NULL, key, type, (const void **) val, len);
		new_kv = (keyval_t *)kva;
	} else {
		// normal type
		new_kv = keyval_new(NULL, key, type, val, size);
	}
	if (!new_kv) {
		return BUNDLE_ERROR_OUT_OF_MEMORY;
	}

	_bundle_append_kv(b, new_kv);

	return BUNDLE_ERROR_NONE;
}

static int _bundle_get_val(bundle *b, const char *key, const int type, void **val, size_t *size, unsigned int *len, size_t **array_element_size)
{
	keyval_t *kv = _bundle_find_kv(b, key);
	if (!kv) {	/* Key doesn't exist */
		return BUNDLE_ERROR_INVALID_PARAMETER;
	}
	if (BUNDLE_TYPE_ANY != type && type != kv->type) {
		return BUNDLE_ERROR_INVALID_PARAMETER;
	}

	if (keyval_type_is_array(type)) {
		keyval_array_t *kva = (keyval_array_t *)kv;
		keyval_array_get_data(kva, NULL, (void ***)val, len, array_element_size);
	} else {
		keyval_get_data(kv, NULL, val, size);
	}

	return BUNDLE_ERROR_NONE;
}

static int bundle_get_val_array(bundle *b, const char *key, char ***str_array, int *len)
{
	return _bundle_get_val(b, key, BUNDLE_TYPE_STR_ARRAY, (void **)str_array, NULL, (unsigned int *)len, NULL);
}

/** global initialization
 *  Run only once.
 */
static void _bundle_global_init(void)
{
	static int _is_done = 0;
	if (_is_done) return;

	// Run init functions
	keyval_type_init();

	_is_done = 1;
	return;
}

/* APIs */
bundle *bundle_create(void)
{
	bundle *b = NULL;

	_bundle_global_init();

	b = calloc(1, sizeof(bundle));	/* fill mem with NULL */
	if (NULL == b) {
		BUNDLE_EXCEPTION_PRINT("Unable to allocate memory for bundle\n");
		return NULL;
	}

	return b;
}

int bundle_free(bundle *b)
{
	keyval_t *kv, *tmp_kv;

	if (NULL == b) {
		BUNDLE_EXCEPTION_PRINT("Bundle is already freed\n");
		return BUNDLE_ERROR_INVALID_PARAMETER;
	}

	/* Free keyval list */
	kv = b->kv_head;
	while (kv != NULL) {
		tmp_kv = kv;
		kv = kv->next;
		tmp_kv->method->free(tmp_kv, 1);
	}

	/* free bundle */
	free(b);

	return BUNDLE_ERROR_NONE;
}
// str type
int bundle_add_str(bundle *b, const char *key, const char *str)
{
	if (!str) {
		return BUNDLE_ERROR_INVALID_PARAMETER;
	}
	return _bundle_add_kv(b, key, str, strlen(str) + 1, BUNDLE_TYPE_STR, 1);
}

int bundle_get_str(bundle *b, const char *key, char **str)
{
	return _bundle_get_val(b, key, BUNDLE_TYPE_STR, (void **)str, NULL, NULL, NULL);
}

const char **bundle_get_str_array(bundle *b, const char *key, int *len)
{
	const char **arr_val = NULL;

	(void)bundle_get_val_array(b, key, (char ***)&arr_val, len);

	return arr_val;
}

int bundle_add_str_array(bundle *b, const char *key, const char **str_array, const int len)
{
	return _bundle_add_kv(b, key, str_array, 0, BUNDLE_TYPE_STR_ARRAY, len);
}

int bundle_del(bundle *b, const char *key)
{
	keyval_t *kv = NULL, *prev_kv = NULL;

	/* basic value check */
	if (NULL == b)
		return BUNDLE_ERROR_INVALID_PARAMETER;
	if (NULL == key)
		return BUNDLE_ERROR_INVALID_PARAMETER;
	if (0 == strlen(key))
		return BUNDLE_ERROR_INVALID_PARAMETER;

	kv = b->kv_head;
	while (kv != NULL) {
		if (0 == strcmp(key, kv->key)) break;
		prev_kv = kv;
		kv = kv->next;
	}
	if (NULL == kv)
		return BUNDLE_ERROR_KEY_NOT_AVAILABLE;
	else {
		if (NULL != prev_kv) {
			prev_kv->next = kv->next;
		}
		if (kv == b->kv_head) b->kv_head = kv->next;
		kv->method->free(kv, 1);
	}
	return BUNDLE_ERROR_NONE;

}

void bundle_foreach(bundle *b, bundle_iterator_t iter, void *user_data)
{
	if (NULL == b || NULL == iter) {
		return;		/*TC_FIX if b=NULL- error handling */
	}

	keyval_t *kv = b->kv_head;
	while (NULL != kv) {
		iter(kv->key, kv->type, kv, user_data);
		kv = kv->next;
	}
}

/* keyval functions */
int bundle_keyval_get_type(bundle_keyval_t *kv)
{
	if (NULL == kv) {
		return -1;
	}
	return kv->type;
}

int bundle_keyval_type_is_array(bundle_keyval_t *kv)
{
	if (NULL == kv) {
		return -1;
	}
	return keyval_type_is_array(kv->type);
}

int bundle_keyval_get_basic_val(bundle_keyval_t *kv, void **val, size_t *size)
{
	return keyval_get_data(kv, NULL, val, size);
}

bundle *bundle_dup(bundle *b_from)
{
	bundle *b_to = NULL;
	int i;

	if (NULL == b_from) {
		return NULL;
	}
	b_to = bundle_create();
	if (NULL == b_to)
		return NULL;

	keyval_t *kv_from = b_from->kv_head;
	keyval_t *kv_to = NULL;
	while (kv_from != NULL) {
		if (keyval_type_is_array(kv_from->type)) {
			keyval_array_t *kva_from = (keyval_array_t *)kv_from;
			kv_to = (keyval_t *)keyval_array_new(NULL, kv_from->key, kv_from->type, NULL, kva_from->len);
			if (!kv_to) goto ERR_CLEANUP;
			for (i = 0; i < kva_from->len; i++) {
				if (((keyval_array_t *)kv_from)->array_val[i]) {
					keyval_array_set_element((keyval_array_t*)kv_to, i, ((keyval_array_t *)kv_from)->array_val[i], ((keyval_array_t *)kv_from)->array_element_size[i]);
				}
			}
			_bundle_append_kv(b_to, kv_to);
		} else {
			if (_bundle_add_kv(b_to, kv_from->key, kv_from->val, kv_from->size, kv_from->type, 0)) goto ERR_CLEANUP;
		}

		kv_from = kv_from->next;
	}
	return b_to;

ERR_CLEANUP:
	bundle_free(b_to);
	return NULL;
}

// byte type
int bundle_add_byte(bundle *b, const char *key, const void *byte, const size_t size)
{
	return _bundle_add_kv(b, key, byte, size, BUNDLE_TYPE_BYTE, 1);
}

int bundle_get_byte(bundle *b, const char *key, void **byte, size_t *size)
{
	return _bundle_get_val(b, key, BUNDLE_TYPE_BYTE, (void **)byte, size, NULL, NULL);
}

const char *bundle_get_val(bundle *b, const char *key)
{
	char *val = NULL;
	(void)bundle_get_str(b, key, &val);

	return val;
}

int bundle_get_type(bundle *b, const char *key)
{
	keyval_t *kv = _bundle_find_kv(b, key);
	if (kv) return kv->type;
	else {
		return BUNDLE_TYPE_NONE;
	}
}

int bundle_get_size(bundle *b, int *len)
{
	keyval_t *kv;
	size_t msize = 0;	// Sum of required size

	if (NULL == b || NULL == len)
		return BUNDLE_ERROR_INVALID_PARAMETER;

	/* calculate memory size */

	kv = b->kv_head;
	while (kv != NULL) {
		msize += kv->size;
		kv = kv->next;
	}

	*len = msize;

	return BUNDLE_ERROR_NONE;
}
