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
 * keyval.c
 * Implementation of keyval object
 */

#include "keyval_type.h"
#include "keyval.h"
#include "bundle_log.h"
#include "bundle.h"
#include <stdlib.h>

static keyval_method_collection_t method = {
	keyval_free
};

keyval_t *keyval_new(keyval_t *kv, const char *key, const int type, const void *val, const size_t size)
{
	int must_free_obj;
	must_free_obj = kv ? 0 : 1;

	if (!kv) {
		kv = calloc(1, sizeof(keyval_t));
		if (!kv) {
			return NULL;
		}
	}

	// key
	if (kv->key) {
		keyval_free(kv, must_free_obj);
		return NULL;
	}
	kv->key = strdup(key);
	if (!kv->key) {
		keyval_free(kv, must_free_obj);
		return NULL;
	}

	// elementa of primitive types
	kv->type = type;
	kv->size = size;

	if (size) {
		kv->val = calloc(1, size);		// allocate memory unconditionally !
		if (!kv->val) {
			keyval_free(kv, 1);
			return NULL;
		}
		if (val) {
			memcpy(kv->val, val, size);
		}
	}

	// Set methods
	kv->method = &method;

	return kv;
}

void keyval_free(keyval_t *kv, int do_free_object)
{
	//int i;

	if (NULL == kv) return;

	if (kv->key) { 
		free(kv->key);
		kv->key = NULL;
	}

	if (NULL != kv->val) {
		free(kv->val);
		kv->val = NULL;
	}

	if (do_free_object) free(kv);

	return;
}

int keyval_get_data(keyval_t *kv, int *type, void **val, size_t *size)
{
	if (!kv)
		return BUNDLE_ERROR_INVALID_PARAMETER;
	if (keyval_type_is_array(kv->type))
		return BUNDLE_ERROR_INVALID_PARAMETER;

	if (type) *type = kv->type;
	if (val) *val = kv->val;
	if (size) *size = kv->size;

	return 0;
}

int keyval_compare(keyval_t *kv1, keyval_t *kv2)
{
	if (!kv1 || !kv2) return -1;

	if (0 != strcmp(kv1->key, kv2->key)) return 1;
	if (kv1->type != kv2->type) return 1;
	if (kv1->size != kv2->size) return 1;

	if (kv1->val == NULL && kv2->val == NULL) return 0;
	if (kv1->val == NULL || kv2->val == NULL) return 1;
	if (0 != memcmp(kv1->val, kv2->val, kv1->size)) return 1;

	return 0;
}
