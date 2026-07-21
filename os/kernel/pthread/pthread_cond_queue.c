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
 * kernel/pthread/pthread_cond_queue.c
 *
 *   Condition variable waiter tracking using linked list.
 *   This is a kernel-internal implementation to track waiter counts
 *   per condition variable without modifying public structures.
 *
 *   Memory allocation (kmm_malloc) and deallocation (kmm_free) are
 *   the responsibility of the caller, and must NOT be performed while
 *   a critical section is held. The caller pre-allocates entries before
 *   entering a critical section and frees them after leaving it.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <pthread.h>
#include <errno.h>

#include <tinyara/irq.h>

#include "pthread/pthread.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Global list head for condition variable waiter tracking
 *
 * NOTE: Entries are auto-removed when waiter count reaches 0.
 * If pthread_cond_destroy() is called while threads are waiting,
 * the entry remains in the list (memory leak). This is acceptable
 * because POSIX defines this as undefined behavior.
 */
static struct cond_waiter_entry *g_cond_waiterlist_head = NULL;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cond_waiter_find
 *
 * Description:
 *   Find an entry in the list by condition variable pointer.
 *   Must be called within critical section.
 *
 * Parameters:
 *   cond - Pointer to condition variable
 *
 * Return:
 *   Pointer to entry if found, NULL otherwise
 *
 ****************************************************************************/

static struct cond_waiter_entry *cond_waiter_find(FAR pthread_cond_t *cond)
{
	struct cond_waiter_entry *entry = g_cond_waiterlist_head;

	while (entry != NULL) {
		if (entry->cond == cond) {
			return entry;
		}
		entry = entry->next;
	}

	return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cond_waiter_increment
 *
 * Description:
 *   Increment the waiter count for a condition variable.
 *   If an entry already exists, increments its count. If not, initializes
 *   and links the caller-provided pre_alloc entry.
 *
 *   The caller must pre-allocate the entry BEFORE entering a critical
 *   section, since kmm_malloc() must not be called with interrupts
 *   disabled. This function only performs list operations (no allocation).
 *
 * Parameters:
 *   cond      - Pointer to condition variable
 *   pre_alloc - Pre-allocated entry (allocated outside critical section)
 *
 * Return:
 *   0 if pre_alloc was consumed (linked into the list)
 *   1 if pre_alloc was NOT consumed (existing entry found, caller must free)
 *
 ****************************************************************************/

int cond_waiter_increment(FAR pthread_cond_t *cond, FAR struct cond_waiter_entry *pre_alloc)
{
	irqstate_t flags;
	struct cond_waiter_entry *entry;

	flags = enter_critical_section();

	/* Find existing entry */
	entry = cond_waiter_find(cond);

	if (entry != NULL) {
		/* Entry already exists — increment and discard pre_alloc */
		entry->waiters++;
		leave_critical_section(flags);
		return 1;	/* pre_alloc NOT consumed */
	}

	/* No existing entry — initialize and link the pre-allocated node */
	pre_alloc->cond = cond;
	pre_alloc->waiters = 1;
	pre_alloc->next = g_cond_waiterlist_head;
	g_cond_waiterlist_head = pre_alloc;

	leave_critical_section(flags);
	return 0;	/* pre_alloc consumed */
}

/****************************************************************************
 * Name: cond_waiter_decrement
 *
 * Description:
 *   Decrement the waiter count for a condition variable.
 *   Auto-unlinks entry when waiter count reaches 0 and returns it
 *   to the caller for freeing outside the critical section.
 *
 *   The caller must free the returned entry AFTER leaving the critical
 *   section, since kmm_free() must not be called with interrupts disabled.
 *   This function only performs list operations (no deallocation).
 *
 * Parameters:
 *   cond - Pointer to condition variable
 *
 * Return:
 *   Pointer to unlinked entry if waiter count reached 0 (caller must free),
 *   NULL otherwise
 *
 ****************************************************************************/

FAR struct cond_waiter_entry *cond_waiter_decrement(FAR pthread_cond_t *cond)
{
	irqstate_t flags;
	struct cond_waiter_entry *entry;
	struct cond_waiter_entry *prev = NULL;
	struct cond_waiter_entry *to_free = NULL;

	flags = enter_critical_section();

	/* Find entry AND track previous pointer for removal */
	entry = g_cond_waiterlist_head;
	while (entry != NULL) {
		if (entry->cond == cond) {
			break;
		}
		prev = entry;
		entry = entry->next;
	}

	if (entry != NULL && entry->waiters > 0) {
		entry->waiters--;

		/* Auto-remove when no more waiters */
		if (entry->waiters == 0) {
			if (prev == NULL) {
				/* Entry is at head */
				g_cond_waiterlist_head = entry->next;
			} else {
				/* Entry is in middle or tail */
				prev->next = entry->next;
			}
			to_free = entry;
		}
	}

	leave_critical_section(flags);
	return to_free;
}

/****************************************************************************
 * Name: cond_waiter_get_count
 *
 * Description:
 *   Get the current waiter count for a condition variable.
 *
 * Parameters:
 *   cond - Pointer to condition variable
 *
 * Return:
 *   Number of waiters (0 if entry doesn't exist)
 *
 ****************************************************************************/

uint16_t cond_waiter_get_count(FAR pthread_cond_t *cond)
{
	irqstate_t flags;
	struct cond_waiter_entry *entry;
	uint16_t count = 0;

	flags = enter_critical_section();

	/* Find existing entry */
	entry = cond_waiter_find(cond);

	if (entry != NULL) {
		count = entry->waiters;
	}

	leave_critical_section(flags);

	return count;
}
