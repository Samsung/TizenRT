/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AMEBA_BASE_CUTILS_INTERFACES_CUTILS_PROPERTY_H
#define AMEBA_BASE_CUTILS_INTERFACES_CUTILS_PROPERTY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	kPropertyTest                = 'test',  // string
	kPropertyNetMode             = 'nmod',  // int32_t
	kPropertyUsingTsf            = 'utsf',  // int32_t
};


bool FastProperty_SetString(uint32_t key, const char *value);
bool FastProperty_SetInt32(uint32_t key, int32_t value);
bool FastProperty_SetInt64(uint32_t key, int64_t value);
bool FastProperty_SetFloat(uint32_t key, float value);

bool FastProperty_GetString(uint32_t key, const char **value);
bool FastProperty_GetInt32(uint32_t key, int32_t *value);
bool FastProperty_GetInt64(uint32_t key, int64_t *value);
bool FastProperty_GetFloat(uint32_t key, float *value);

void FastProperty_DumpAll(void);

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_BASE_CUTILS_INTERFACES_CUTILS_PROPERTY_H
