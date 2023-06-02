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

	flags = enter_critical_section();
	sem_ptr = (sem_t *)sq_peek(&g_sem_list);
	while (sem_ptr) {
		if (sem_ptr == sem) {
			/* Already registered */
			leave_critical_section(flags);
			return;
		}
		sem_ptr = sq_next(sem_ptr);
	}
	/* Add semaphore to a list of kernel semaphore, g_sem_list */
	sq_addlast((FAR sq_entry_t *)sem, &g_sem_list);
	leave_critical_section(flags);
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

	flags = enter_critical_section();

	/* Remove semaphore from a list of kernel semaphore */
	sq_rem((FAR sq_entry_t *)sem, &g_sem_list);

	leave_critical_section(flags);
}
