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

#ifndef __KEYVAL_TYPE_H__
#define __KEYVAL_TYPE_H__

/**
 * keyval_type.h
 *
 * Definitions & short funcs for keyval type
 */

#include <app/bundle.h>
#include <stddef.h>
#include <string.h>

// measure_size function type
typedef size_t (*keyval_type_measure_size_func_t)(void *val);

void _type_init_measure_size_func(void);
int keyval_type_is_array(int type);
int keyval_type_is_measurable(int type);
keyval_type_measure_size_func_t keyval_type_get_measure_size_func(int type);

/* Measure functions for each type */
size_t keyval_type_measure_size_str(void *val);
void keyval_type_init(void);

#endif /* __KEYVAL_TYPE_H__ */
