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

#ifndef AMEBA_CUTILS_OSAL_ATOMIC_H
#define AMEBA_CUTILS_OSAL_ATOMIC_H

#include <stdint.h>

#ifndef OSAL_ATOMIC_INLINE
#define OSAL_ATOMIC_INLINE static inline
#endif

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicInc(volatile uint32_t *addr)
{
	return __atomic_fetch_add(addr, 1, __ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicDec(volatile uint32_t *addr)
{
	return __atomic_fetch_sub(addr, 1, __ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicAdd(uint32_t value, volatile uint32_t *addr)
{
	return __atomic_fetch_add(addr, value, __ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicAnd(uint32_t value, volatile uint32_t *addr)
{
	return __atomic_fetch_and(addr, value, __ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicOr(uint32_t value, volatile uint32_t *addr)
{
	return __atomic_fetch_or(addr, value, __ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicAcquireLoad(volatile const uint32_t *addr)
{
	return __atomic_load_n(addr, __ATOMIC_ACQUIRE);
}

OSAL_ATOMIC_INLINE
uint32_t OsalAtomicReleaseLoad(volatile const uint32_t *addr)
{
	__atomic_thread_fence(__ATOMIC_SEQ_CST);
	return __atomic_load_n(addr, __ATOMIC_RELAXED);
}

OSAL_ATOMIC_INLINE
void OsalAtomicAcquireStore(uint32_t value, volatile uint32_t *addr)
{
	__atomic_store_n(addr, value, __ATOMIC_RELAXED);
	__atomic_thread_fence(__ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
void OsalAtomicReleaseStore(uint32_t value, volatile uint32_t *addr)
{
	__atomic_store_n(addr, value, __ATOMIC_RELEASE);
}

OSAL_ATOMIC_INLINE
int OsalAtomicAcquireCas(uint32_t oldvalue, uint32_t newvalue, volatile uint32_t *addr)
{
	return !__atomic_compare_exchange_n(addr, &oldvalue, newvalue,
										false,
										__ATOMIC_ACQUIRE,
										__ATOMIC_ACQUIRE);
}

OSAL_ATOMIC_INLINE
int OsalAtomicReleaseCas(uint32_t oldvalue, uint32_t newvalue, volatile uint32_t *addr)
{
	return !__atomic_compare_exchange_n(addr, &oldvalue, newvalue,
										false,
										__ATOMIC_RELEASE,
										__ATOMIC_RELAXED);
}

OSAL_ATOMIC_INLINE
int OsalAtomicCompareAddSwap(uint32_t oldvalue, uint32_t newvalue, volatile uint32_t *addr)
{
	return __atomic_compare_exchange(addr, &oldvalue, &newvalue,
									 false,
									 __ATOMIC_SEQ_CST,
									 __ATOMIC_SEQ_CST);
}

OSAL_ATOMIC_INLINE
void OsalCompilerBarrier(void)
{
	__asm__ __volatile__("" : : : "memory");
}

OSAL_ATOMIC_INLINE
void OsalMemoryBarrier(void)
{
	__atomic_thread_fence(__ATOMIC_SEQ_CST);
}

#define OsalAtomicWrite OsalAtomicReleaseStore
#define OsalAtomicCmpxchg OsalAtomicReleaseCas

#endif /* AMEBA_CUTILS_OSAL_ATOMIC_H */
