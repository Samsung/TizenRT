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

#include "keyval_type.h"
#include "bundle_log.h"
#include <stddef.h>

void keyval_type_init(void)
{
	static int is_done;
	is_done = 0;
	if (is_done)
		return;

	// Still do nothing

	is_done = 1;
}

int keyval_type_is_array(int type)
{
	if (type & BUNDLE_TYPE_ARRAY)
		return 1;
	return 0;
}

int keyval_type_is_measurable(int type)
{
	if (type & BUNDLE_TYPE_MEASURABLE)
		return 1;
	return 0;
}

keyval_type_measure_size_func_t
keyval_type_get_measure_size_func(int type)
{
	switch (type) {
	case BUNDLE_TYPE_STR:
	case BUNDLE_TYPE_STR_ARRAY:
		return keyval_type_measure_size_str;
		break;
	default:
		return NULL;
	}
	return NULL;
}

size_t
keyval_type_measure_size_str(void *val)
{
	if (!val)
		return 0;
	return strlen((char *)val) + 1;
}
