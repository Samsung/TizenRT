/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 ****************************************************************************/

 /*
 * s1_utils.h
 */
#ifndef __S1_UTILS_H__
#define __S1_UTILS_H__

#include <stdint.h>
#include <stddef.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

void *s1_memcpy(void *dest, const void *src, size_t n);
void *s1_memset(void *s, int c, size_t n);

#endif /* __S1_UTILS_H__ */
