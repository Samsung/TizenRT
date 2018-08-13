/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#ifndef __INCLUDE_DEBUG_SYSDBG_H
#define __INCLUDE_DEBUG_SYSDBG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/sched.h>
#include <sys/types.h>
#include <stdint.h>
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Reducing the size intentionally for debug purpose to avoid memory crunch */
#define TASK_NAME_SIZE 15

#define SEMAPHORE_STATUS_SIZE 2
/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * system debug Function Declarations
 ****************************************************************************/
int sysdbg_init(void);

/* Important data structures to save the task and irq history */
struct sched_history_s {
	clock_t time;
#if CONFIG_TASK_NAME_SIZE > 0
	char task[TASK_NAME_SIZE + 1];
#endif
	pid_t pid;
	uint8_t sched_priority;
	struct tcb_s *ptcb;
};

typedef struct sched_history_s sched_history_t;

struct irq_history_s {
	clock_t time;
	uint32_t irq;
	void *fn;
};

typedef struct irq_history_s irq_history_t;

struct sem_history_s {
	clock_t time;
	char status[SEMAPHORE_STATUS_SIZE + 1];
#if CONFIG_TASK_NAME_SIZE > 0
	char task_name[TASK_NAME_SIZE + 1];
#endif
	sem_t *sem;
	void *ptcb;
	pid_t pid;
};

typedef struct sem_history_s sem_history_t;

enum sem_status_s {
	SEM_INIT = 0,
	SEM_AQUIRE,
	SEM_RELEASE,
	SEM_WAITING,
	SEM_DESTROY,
};

typedef enum sem_status_s sem_status_t;

struct sysdbg_s {
#ifdef CONFIG_TASK_SCHED_HISTORY
	sched_history_t *sched;
	int task_lastindex;
#endif
#ifdef CONFIG_IRQ_SCHED_HISTORY
	irq_history_t *irq;
	int irq_lastindex;
#endif
#ifdef CONFIG_SEMAPHORE_HISTORY
	sem_history_t *sem_log;
	int sem_lastindex;
#endif
};

typedef struct sysdbg_s sysdbg_t;

extern void save_task_scheduling_status(struct tcb_s *tcb);
extern void save_irq_scheduling_status(uint32_t irq, void *fn);
extern void save_semaphore_history(FAR sem_t *sem, void *addr, sem_status_t status);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif							/* __ASSEMBLY__ */
#endif							/*  __INCLUDE_DEBUG_SYSDBG_H */
