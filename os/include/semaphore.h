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
/****************************************************************************
 * include/semaphore.h
 *
 *   Copyright (C) 2007-2009, 2012-2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @defgroup SEMAPHORE_KERNEL SEMAPHORE
 * @brief Provides APIs for Semaphore
 * @ingroup KERNEL
 */

/// @file semaphore.h
/// @brief Semaphore APIs

#ifndef __INCLUDE_SEMAPHORE_H
#define __INCLUDE_SEMAPHORE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <limits.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Bit definitions for the struct sem_s flags field */

#define PRIOINHERIT_FLAGS_DISABLE (1 << 0) /* Bit 0: Priority inheritance
					    * is disabled for this semaphore */
#define FLAGS_INITIALIZED         (1 << 1) /* Bit 1: This semaphore initialized */
/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/

/* This structure contains information about the holder of a semaphore */

#ifdef CONFIG_PRIORITY_INHERITANCE
struct tcb_s;					/* Forward reference */
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief Structure of semholder
 */
struct semholder_s {
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	struct semholder_s *flink;	/* Implements singly linked list */
#endif
	FAR struct tcb_s *htcb;		/* Holder TCB */
	int16_t counts;				/* Number of counts owned by this holder */
};

#if CONFIG_SEM_PREALLOCHOLDERS > 0
#define SEMHOLDER_INITIALIZER {NULL, NULL, 0}
#else
#define SEMHOLDER_INITIALIZER {NULL, 0}
#endif
#endif							/* CONFIG_PRIORITY_INHERITANCE */

/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief Structure of generic semaphore
 */
struct sem_s {
	int16_t semcount;			/* >0 -> Num counts available */
	/* <0 -> Num tasks waiting for semaphore */
	/* If priority inheritance is enabled, then we have to keep track of which
	 * tasks hold references to the semaphore.
	 */

	uint8_t flags;			/* See definitions for the struct sem_s flags */
#ifdef CONFIG_PRIORITY_INHERITANCE
#if CONFIG_SEM_PREALLOCHOLDERS > 0
	FAR struct semholder_s *hhead;	/* List of holders of semaphore counts */
#else
	struct semholder_s holder;	/* Single holder */
#endif
#endif
};

typedef struct sem_s sem_t;

/* Initializers */
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief Sem initializer
 */
#ifdef CONFIG_PRIORITY_INHERITANCE
#if CONFIG_SEM_PREALLOCHOLDERS > 0
#define SEM_INITIALIZER(c) {(c), FLAGS_INITIALIZED, NULL} /* semcount, flags, hhead */
#else
#define SEM_INITIALIZER(c) {(c), FLAGS_INITIALIZED, SEMHOLDER_INITIALIZER} /* semcount, flags, holder */
#endif
#else
#define SEM_INITIALIZER(c) {(c), FLAGS_INITIALIZED}	/* semcount, flags */
#endif

/****************************************************************************
 * Public Variables
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/* Forward references needed by some prototypes */

struct timespec;				/* Defined in time.h */

/* Counting Semaphore Interfaces (based on POSIX APIs) */
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief initialize an unnamed semaphore
 * @details @b #include <semaphore.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_init(FAR sem_t *sem, int pshared, unsigned int value);

/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief destroy an unnamed semaphore
 * @details @b #include <semaphore.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_destroy(FAR sem_t *sem);
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief lock a semaphore
 * @details @b #include <semaphore.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_wait(FAR sem_t *sem);
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief lock a semaphore
 * @details @b #include <semaphore.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_timedwait(FAR sem_t *sem, FAR const struct timespec *abstime);
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief lock a semaphore
 * @details @b #include <semaphore.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_trywait(FAR sem_t *sem);
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief unlock a semaphore
 * @details @b #include <semaphore.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_post(FAR sem_t *sem);
/**
 * @ingroup SEMAPHORE_KERNEL
 * @brief get the value of a semaphore
 * @details @b #include <semaphore.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sem_getvalue(FAR sem_t *sem, FAR int *sval);

void sem_timeout(int argc, uint32_t pid);

#ifdef CONFIG_FS_NAMED_SEMAPHORES
/**
 * @cond
 * @internal
 */
FAR sem_t *sem_open(FAR const char *name, int oflag, ...);
/**
 * @internal
 */
int sem_close(FAR sem_t *sem);
/**
 * @internal
 */
int sem_unlink(FAR const char *name);
/**
 * @endcond
 */
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_SEMAPHORE_H */
