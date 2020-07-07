/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/
#include <queue.h>
#include <semaphore.h>

#include <tinyara/arch.h>

#include "sched/sched.h"
#include "binary_manager/binary_manager.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define is_kernel_sem(a) is_kernel_space((void *)a)

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
sq_queue_t g_sem_list;

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void sem_addlist(FAR sem_t *sem, sq_queue_t *list)
{
	sem_t *sem_ptr;
	irqstate_t flags;

	flags = irqsave();

	sem_ptr = (sem_t *)sq_peek(list);
	while (sem_ptr) {
		if (sem_ptr == sem) {
			/* Already registered */
			irqrestore(flags);
			return;
		}
		sem_ptr = sq_next(sem_ptr);
	}
	sq_addlast((FAR sq_entry_t *)sem, list);

	irqrestore(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: sem_register
 *
 * Description:
 *   Register semaphore to a list of kernel and user semaphores.
 *
 * Parameters:
 *   sem - Semaphore descriptor
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   This function may be called when semaphore is initialized.
 *
 ****************************************************************************/
void sem_register(FAR sem_t *sem)
{
	int binidx;
	struct tcb_s *tcb;

	if (is_kernel_sem(sem)) {
		/* A list of kernel semaphores */
		sem_addlist(sem, &g_sem_list);
	} else {
		tcb = this_task();
		if (tcb && tcb->group && tcb->group->tg_binidx > 0) {
			/* A list of user binary semaphores */
			sem_addlist(sem, &BIN_SEMLIST(tcb->group->tg_binidx));
		}
	}
}

/****************************************************************************
 * Name: sem_unregister
 *
 * Description:
 *   Unegister semaphore from a list of kernel and user semaphores.
 *
 * Parameters:
 *   sem - Semaphore descriptor
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   This function may be called when semaphore is destroyed.
 *
 ****************************************************************************/
void sem_unregister(FAR sem_t *sem)
{
	int binidx;
	struct tcb_s *tcb;
	irqstate_t flags;

	flags = irqsave();

	if (is_kernel_sem(sem)) {
		/* A list of kernel semaphores */
		sq_rem((FAR sq_entry_t *)sem, (sq_queue_t *)&g_sem_list);
	} else {
		tcb = this_task();
		if (tcb && tcb->group && tcb->group->tg_binidx > 0) {
			/* A list of user binary semaphores */
			binidx = tcb->group->tg_binidx;
			sq_rem((FAR sq_entry_t *)sem, (sq_queue_t *)&BIN_SEMLIST(binidx));
		}
	}
	irqrestore(flags);
}
