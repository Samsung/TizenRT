/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "rw_locks.h"

#include <pthread.h>

/****************************************************************************
* Public Functions
****************************************************************************/
void rw_init(struct rw_lock_s *rwLock)
{
	pthread_mutex_init(&rwLock->mutex, NULL);
	pthread_cond_init(&rwLock->noWriters, NULL);
	pthread_cond_init(&rwLock->noActiveWriter, NULL);
	pthread_cond_init(&rwLock->noReaders, NULL);
	rwLock->readers = 0;
	rwLock->writers = 0;
	rwLock->writerActive = false;
}

void rw_lock_read(struct rw_lock_s *rwLock)
{
	pthread_mutex_lock(&rwLock->mutex);

	// Wait until there are no writers
	while (rwLock->writers > 0) {
		pthread_cond_wait(&rwLock->noWriters, &rwLock->mutex);
	}
	// Since there are no writers, we can proceed to read
	rwLock->readers++;
	pthread_mutex_unlock(&rwLock->mutex);
}

void rw_unlock_read(struct rw_lock_s *rwLock)
{
	pthread_mutex_lock(&rwLock->mutex);
	rwLock->readers--;
	// The last reader will signal that there are no readers,
	// allowing any waiting writers to proceed
	if (rwLock->readers == 0) {
		pthread_cond_signal(&rwLock->noReaders);
	}
	pthread_mutex_unlock(&rwLock->mutex);
}

int rw_trylock_write(struct rw_lock_s *rwLock)
{
	pthread_mutex_lock(&rwLock->mutex);
	if (rwLock->readers > 0 || rwLock->writers > 0 || rwLock->writerActive) {
		pthread_mutex_unlock(&rwLock->mutex);
		return 0;
	} else {
		rwLock->writers++;
		rwLock->writerActive = true;
		pthread_mutex_unlock(&rwLock->mutex);
		return 1;
	}
}

void rw_lock_write(struct rw_lock_s *rwLock)
{
	pthread_mutex_lock(&rwLock->mutex);
	// Announce intent to write: keep out any new readers
	rwLock->writers++;
	// Wait for readers to finish
	while (rwLock->readers > 0) {
		pthread_cond_wait(&rwLock->noReaders, &rwLock->mutex);
	}
	// Wait for any active writer to finish
	while (rwLock->writerActive) {
		pthread_cond_wait(&rwLock->noActiveWriter, &rwLock->mutex);
	}
	// Now this thread is the active writer
	rwLock->writerActive = true;
	pthread_mutex_unlock(&rwLock->mutex);
}

void rw_unlock_write(struct rw_lock_s *rwLock)
{
	pthread_mutex_lock(&rwLock->mutex);
	rwLock->writerActive = false;
	rwLock->writers--;
	// If there are other writers waiting, let the next one take a turn.
	// Otherwise, signal that there are no more writers, allowing any waiting readers
	// to proceed.
	if (rwLock->writers > 0) {
		pthread_cond_signal(&rwLock->noActiveWriter);
	} else {					/* rwLock->writers == 0 */

		pthread_cond_signal(&rwLock->noWriters);
	}
	pthread_mutex_unlock(&rwLock->mutex);
}
