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

#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include <tinyara/config.h>
#include <debug.h>
#include <sys/types.h>
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#define SYS_MBOX_NULL ((sys_mbox_t *)NULL)
#define SYS_SEM_NULL  ((sys_sem_t *)NULL)
#define SYS_DEFAULT_THREAD_STACK_DEPTH  PTHREAD_STACK_MIN
#define SYS_MBOX_MAXSIZE 128

// === PROTECTION ===
typedef int sys_prot_t;
typedef sem_t sys_sem_t;
typedef pthread_mutex_t sys_mutex_t;
typedef pid_t sys_thread_t;

// === MAIL BOX ===

struct sys_mbox {
	u8_t is_valid;
	u8_t id;
	u32_t queue_size;
	u32_t wait_send;
	u32_t wait_fetch;
	u32_t front;
	u32_t rear;
	void *msgs[SYS_MBOX_MAXSIZE];
	sys_sem_t mail;
	sys_sem_t mutex;
};

typedef struct sys_mbox sys_mbox_t;

#endif							/* __ARCH_SYS_ARCH_H__ */
