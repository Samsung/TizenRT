/* *****************************************************************
 *
 * Copyright 2017 Microsoft
 *
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
 ******************************************************************/
#ifndef OC_ATOMIC_H
#define OC_ATOMIC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * Increments (increases by one) the value of the specified int32_t variable atomically.
 *
 * @param[in] addend  Pointer to the variable to be incremented.
 * @return int32_t  The resulting incremented value.
 */
int32_t oc_atomic_increment(volatile int32_t *addend);

/**
 * Decrements (decreases by one) the value of the specified int32_t variable atomically.
 *
 * @param[in] addend  Pointer to the variable to be decremented.
 * @return int32_t  The resulting decremented value.
 */
int32_t oc_atomic_decrement(volatile int32_t *addend);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* OC_ATOMIC_H */
