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
 ****************************************************************************/
#ifndef __RW_LOCKS_H__
#define __RW_LOCKS_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/types.h>

/****************************************************************************
* Public Type Definitions
****************************************************************************/
struct rw_lock_s {
	pthread_mutex_t mutex;
	pthread_cond_t noWriters;
	pthread_cond_t noActiveWriter;
	pthread_cond_t noReaders;
	int readers;
	int writers;
	bool writerActive;
};

/****************************************************************************
* Global Function Prototypes
****************************************************************************/
void rw_init(struct rw_lock_s *rwLock);
void rw_lock_read(struct rw_lock_s *rwLock);
void rw_unlock_read(struct rw_lock_s *rwLock);

int rw_trylock_write(struct rw_lock_s *rwLock);

void rw_lock_write(struct rw_lock_s *rwLock);

void rw_unlock_write(struct rw_lock_s *rwLock);

#endif							/* __RW_LOCKS_H__ */
