/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_NET_NET_LOCK_H
#define __INCLUDE_NET_NET_LOCK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#ifdef CONFIG_NET

#include <semaphore.h>

/* Only UDS uses net_lock */
#ifdef CONFIG_NET_LOCAL
/****************************************************************************
 * Critical section management.
 *
 * Re-entrant mutex based locking of the network is supported:
 *
 *   net_lock()        - Locks the network via a re-entrant mutex.
 *   net_unlock()      - Unlocks the network.
 *   net_lockedwait()  - Like pthread_cond_wait() except releases the
 *                       network momentarily to wait on another semaphore.
 *   net_ioballoc()    - Like iob_alloc() except releases the network
 *                       momentarily to wait for an IOB to become
 *                       available.
 *
 ****************************************************************************/

/****************************************************************************
 * Name: net_lock
 *
 * Description:
 *   Take the network lock
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   failured (probably -ECANCELED).
 *
 ****************************************************************************/

int net_lock(void);

/****************************************************************************
 * Name: net_unlock
 *
 * Description:
 *   Release the network lock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_unlock(void);

/****************************************************************************
 * Name: net_timedwait
 *
 * Description:
 *   Atomically wait for sem (or a timeout( while temporarily releasing
 *   the lock on the network.
 *
 *   Caution should be utilized.  Because the network lock is relinquished
 *   during the wait, there could changes in the network state that occur
 *   before the lock is recovered.  Your design should account for this
 *   possibility.
 *
 * Input Parameters:
 *   sem     - A reference to the semaphore to be taken.
 *   timeout - The relative time to wait until a timeout is declared.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int net_timedwait(sem_t *sem, unsigned int timeout);

/****************************************************************************
 * Name: net_lockedwait
 *
 * Description:
 *   Atomically wait for sem while temporarily releasing the network lock.
 *
 *   Caution should be utilized.  Because the network lock is relinquished
 *   during the wait, there could changes in the network state that occur
 *   before the lock is recovered.  Your design should account for this
 *   possibility.
 *
 * Input Parameters:
 *   sem - A reference to the semaphore to be taken.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int net_lockedwait(sem_t *sem);

/****************************************************************************
 * Name: net_timedwait_uninterruptible
 *
 * Description:
 *   This function is wrapped version of net_timedwait(), which is
 *   uninterruptible and convenient for use.
 *
 * Input Parameters:
 *   sem     - A reference to the semaphore to be taken.
 *   timeout - The relative time to wait until a timeout is declared.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int net_timedwait_uninterruptible(sem_t *sem, unsigned int timeout);

/****************************************************************************
 * Name: net_lockedwait_uninterruptible
 *
 * Description:
 *   This function is wrapped version of net_lockedwait(), which is
 *   uninterruptible and convenient for use.
 *
 * Input Parameters:
 *   sem - A reference to the semaphore to be taken.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int net_lockedwait_uninterruptible(sem_t *sem);
#endif /* CONFIG_NET_LOCAL */
#endif /* CONFIG_NET */
#endif /* __INCLUDE_NET_NET_LOCK_H */
