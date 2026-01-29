// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <common/bk_typedef.h>
#include "spinlock.h"

#if defined(CONFIG_SOC_SMP)
#include "cmsis_gcc.h"
#include "FreeRTOS.h"
#endif

#if (defined(CONFIG_SOC_SMP))
#else

#if (defined(CONFIG_SYS_CPU0))
#define CPU_ID     0
#endif
#if (defined(CONFIG_SYS_CPU1))
#define CPU_ID     1
#endif
#if (defined(CONFIG_SYS_CPU2))
#define CPU_ID     2
#endif

#define portGET_CORE_ID()   CPU_ID
#endif

extern void		arch_fence(void);
extern void		arch_atomic_set(volatile u32 * lock_addr);
extern void		arch_atomic_clear(volatile u32 * lock_addr);

#define arch_int_disable	rtos_disable_int
#define arch_int_restore	rtos_enable_int

void spinlock_init(spinlock_t *slock)
{
	slock->owner = 0;
	slock->count = 0;
	slock->core_id = 0xF2EE;
}

uint32_t spinlock_acquire(volatile spinlock_t *slock)
{
	uint32_t flag = arch_int_disable();

#if (CONFIG_CPU_CNT > 1)
	uint16_t core_id;

	// Note: The core IDs are the full 32 bit (CORE_ID_REGVAL_PRO/CORE_ID_REGVAL_APP) values
	core_id = (uint16_t)portGET_CORE_ID();

	if(core_id == slock->core_id)
	{
		slock->count++;
		return flag;

	}

	arch_atomic_set(( volatile u32 *)&slock->owner);

	arch_fence();

	slock->core_id = core_id;
	slock->count = 1;
#endif
	return flag;
}

void spinlock_release(volatile spinlock_t *slock, uint32_t flag)
{
#if (CONFIG_CPU_CNT > 1)
	uint16_t core_id;

	// Note: The core IDs are the full 32 bit (CORE_ID_REGVAL_PRO/CORE_ID_REGVAL_APP) values
	core_id = (uint16_t)portGET_CORE_ID();

	if(core_id != slock->core_id)
	{
		return;
	}

	if(slock->count == 0)
	{
		return;
	}

	slock->count--;

	if(slock->count == 0)
	{
		slock->core_id = 0xF2EE;

		arch_fence();

		arch_atomic_clear(( volatile u32 *)&slock->owner);
	}
#endif

	arch_int_restore(flag);
}

#if (defined(CONFIG_SOC_SMP))
#if CONFIG_CPU_CNT > 2
/* bk7236/58 three cores, just two exclusive access monitors
 * verification code:http://192.168.0.6/wangzhilei/bk7236_verification/-/tree/multicore_spinlock
 *
 * the exclusive operation is the pair of ldrex following by strex: ldrex performs a load of memory
 * but also tags the physical address to be monitored for exclusive access by the that core.
 * strex performs a conditional store to the memory, succeeding only if the target location is tagged
 * as being monitored for exclusive access by that core. this instruction returns non-zero in the
 * general-purpose register if the store does not succeed, and a value of 0 if the store is successful
 *
 * The issue: two cores occupy the exclusive signal, and the other core maybe cannot unlock/strex successfully
 */
 #error number of cpu cores greater than 2, does not support SMP for this configuration.
#endif

/*TODO: the driver layer shall be independent of the architecture or arm instruction*/
static inline int __spin_lock(volatile spinlock_t *lock)
{
	uint32_t core_id;
	int status = 0;

	// Note: The core IDs are the full 32 bit (CORE_ID_REGVAL_PRO/CORE_ID_REGVAL_APP) values
	core_id = portGET_CORE_ID();

	// The caller is already the owner of the lock. Simply increment the nesting count
	if (lock->owner == core_id)
	{
		BK_ASSERT(lock->count > 0 && lock->count < 0xFF);	  // Bad count value implies memory corruption
		lock->count ++;
		return 1;
	}

	do
	{
		// Note: __LDAEX and __STREXW are CMSIS functions

		while (__LDAEX(&lock->owner) != SPIN_LOCK_FREE)
		{
			__WFE();
		}

		// lock is free
		status = __STREXW(core_id, &lock->owner); // Try to set

	} while (status != 0); // retry until lock successfully

	lock->count ++;

	return 1;
}

static inline int __spin_lock_try(volatile spinlock_t *lock)
{
	uint32_t core_id;
	int status = 0;

	// Note: The core IDs are the full 32 bit (CORE_ID_REGVAL_PRO/CORE_ID_REGVAL_APP) values
	core_id = portGET_CORE_ID();

	// The caller is already the owner of the lock. Simply increment the nesting count
	if (lock->owner == core_id)
	{
		BK_ASSERT(lock->count > 0 && lock->count < 0xFF);	  // Bad count value implies memory corruption
		lock->count ++;
		return 1;
	}

	// Note: __LDAEX and __STREXW are CMSIS functions

	if (__LDAEX(&lock->owner) != SPIN_LOCK_FREE)
	{
		return 0;
	}

	// lock is free
	status = __STREXW(core_id, &lock->owner); // Try to set

	if(status != 0)
	{
		return 0;
	}

	lock->count ++;

	return 1;
}

void spin_lock(volatile spinlock_t *lock)
{
	__spin_lock(lock);
}

int spin_trylock(volatile spinlock_t *lock)
{
	return __spin_lock_try(lock);
}

void spin_unlock(volatile spinlock_t *lock)
{
    uint32_t core_id;

    core_id = portGET_CORE_ID();
    BK_ASSERT(core_id == lock->owner); // This is a lock that we didn't acquire, or the lock is corrupt
    BK_ASSERT((lock->count > 0) && (lock->count < 0x100));

    lock->count  --;

    if (!lock->count)
	{
		// If this is the last recursive release of the lock, mark the lock as free
		// Note: __STL, __DSB, __SEV are CMSIS functions.

		__STL(SPIN_LOCK_FREE, &lock->owner);
		__DSB();
 		__SEV();		
    }
}

uint32_t _spin_lock_irqsave(spinlock_t *lock)
{
	unsigned long flags = rtos_disable_int();
	spin_lock(lock);

	return flags;
}

void _spin_unlock_irqrestore(spinlock_t *lock, uint32_t flags)
{
	spin_unlock(lock);
	rtos_enable_int(flags);
}
#endif // CONFIG_SOC_BK7236_SMP_TEMP || CONFIG_SOC_BK7239_SMP_TEMP || CONFIG_SOC_BK7286_SMP_TEMP
// eof

