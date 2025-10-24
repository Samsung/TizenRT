// Copyright 2020-2022 Beken
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

#ifndef _spinlock_h_
#define _spinlock_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>

typedef struct {
	uint32_t  owner;
	uint16_t  count;
	uint16_t  core_id;
} spinlock_t;

#define SPIN_LOCK_FREE   (0xF2EEF2EE)
#define SPIN_LOCK_INIT   {.owner = SPIN_LOCK_FREE, .count = 0}

void spinlock_init(spinlock_t *slock);
uint32_t spinlock_acquire(volatile spinlock_t *slock);
void spinlock_release(volatile spinlock_t *slock, uint32_t flag);

/* similiar with linux spin lock API */
/* NOTE: !!! lock must located in shared memory */
void spin_lock_init(spinlock_t *lock);
void spin_lock(volatile spinlock_t *lock);
void spin_unlock(volatile spinlock_t *lock);

/* spin_trylock: 0: spin lock failed, 1: spin lock success  */
int spin_trylock(volatile spinlock_t *lock);

#define spin_lock_irqsave(lock, flags)      \
	do {				                    \
		flags = _spin_lock_irqsave(lock);	\
	} while (0)

#define spin_unlock_irqrestore(lock, flags)		\
	do {							            \
		_spin_unlock_irqrestore(lock, flags);	\
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* _spinlock_h_ */

