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

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: sem_register
 *
 * Description:
 *   Register semaphore to a list of kernel semaphores.
 *
 * Parameters:
 *   sem - Semaphore descriptor
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   This function may be called when semaphore in kernel region is initialized.
 *
 ****************************************************************************/
void sem_register(FAR sem_t *sem)
{
	sem_t *sem_ptr;
	irqstate_t flags;

	if (!sem) {
		return;
	}

	if (is_kernel_sem(sem)) {
		flags = irqsave();
		sem_ptr = (sem_t *)sq_peek(&g_sem_list);
		while (sem_ptr) {
			if (sem_ptr == sem) {
				/* Already registered */
				irqrestore(flags);
				return;
			}
			sem_ptr = sq_next(sem_ptr);
		}
		sq_addlast((FAR sq_entry_t *)sem, g_sem_list);
	}

	irqrestore(flags);
}

/****************************************************************************
 * Name: sem_unregister
 *
 * Description:
 *   Unegister semaphore from a list of kernel semaphores.
 *
 * Parameters:
 *   sem - Semaphore descriptor
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   This function may be called when semaphore in kernel region is destroyed.
 *
 ****************************************************************************/
void sem_unregister(FAR sem_t *sem)
{
	irqstate_t flags;

	if (!sem) {
		return;
	}

	flags = irqsave();

	if (is_kernel_sem(sem)) {
		/* A list of kernel semaphores */
		sq_rem((FAR sq_entry_t *)sem, (sq_queue_t *)&g_sem_list);
	}
	irqrestore(flags);
}
