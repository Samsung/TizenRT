/*
 * Copyright (c) 2021 Realtek, LLC.
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

#ifndef AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_AUDIO_COMPAT_H
#define AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_AUDIO_COMPAT_H

#ifndef AUDIO_HAL_ATOMIC_INLINE
#define AUDIO_HAL_ATOMIC_INLINE static inline
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef __ICCARM__

#include <yvals.h>
#include <time64.h>
size_t strlcpy(char *dst, const char *src, size_t dsize);
typedef _Atomic(unsigned int)       atomic_uint;

AUDIO_HAL_ATOMIC_INLINE
int AudioHALAtomicCompareAddSwap(volatile uint32_t *addr, uint32_t *oldvalue, uint32_t *newvalue)
{
	return __iar_atomic_compare_exchange_strong((volatile atomic_uint *)addr, (atomic_uint *)oldvalue, (atomic_uint)(*newvalue), __MEMORY_ORDER_SEQ_CST__,
			__MEMORY_ORDER_SEQ_CST__);
}
#else
AUDIO_HAL_ATOMIC_INLINE
int AudioHALAtomicCompareAddSwap(volatile uint32_t *addr, uint32_t *oldvalue, uint32_t *newvalue)
{
	return __atomic_compare_exchange(addr, oldvalue, newvalue,
									 false,
									 __ATOMIC_SEQ_CST,
									 __ATOMIC_SEQ_CST);
}
#endif

#endif // AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_AUDIO_COMPAT_H
