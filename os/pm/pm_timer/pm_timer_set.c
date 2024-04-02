/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
/************************************************************************
 * pm/pm_timer/pm_timer_create.c
 *
 *   Copyright (C) 2007, 2009, 2014 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/pm/pm.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>

#include "pm_timer.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

pm_wakeup_timer_t *get_pm_timer(int id)
{
        /* If we also use malloc to allocate memory for timers,
         * We need a good way to return the required timer ??? 
         * For now, we just return timer from the g_pmTimer_list. */

        /* handle the case if it is a valid id */
        if (id < 0 || id >= CONFIG_PM_MAX_WAKEUP_TIMER) {
                return NULL;
        }

        /* We should not do any operation with FREE status timers unless created by app*/
        if (g_pmTimer_list[id].status == FREE) {
                return NULL;
        }

        return &g_pmTimer_list[id];
}

void remove_pm_timer(pm_wakeup_timer_t *timer)
{
        (void)sq_rem((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
        timer->status = INACTIVE;
        pmdbg("pm timer with id %d is removed from list\n", timer->id);
        /* check for extra operation or change any fields of the timer ??? */
}

void add_pm_timer(pm_wakeup_timer_t *timer, unsigned int timer_interval) 
{
        /* updating the expire time of the wakeup timer */
        timer->expire_timetick = clock_systimer() + timer_interval;
        pmdbg("add_pm_timer: timer->expire_timetick is %d\n", (int)timer->expire_timetick);

        /* Case where there are no timers in the list */

        if (g_pmTimer_activeList.head == NULL) {
                sq_addlast((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                pmdbg("add_pm_timer: timer added in the head of linked list\n");
        }
        
        /* We should add the timer in the sorted position of expiration time */

         else {
                pm_wakeup_timer_t *curr;
                pm_wakeup_timer_t *prev;
                prev = curr = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;

                while (timer->expire_timetick >= curr->expire_timetick && curr->next) {
                        prev = curr;
                        curr = curr->next;
                }

                /* timer should be added before the curr */
                if (timer->expire_timetick < curr->expire_timetick) {
                        
                        if (curr == (pm_wakeup_timer_t *)g_pmTimer_activeList.head) {
                                sq_addfirst((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        } else {
                                sq_addafter((FAR sq_entry_t *)prev, (FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        }  

                /* timer expire time is greater than every other timer. 
                 * timer should be added to the end of the linked list*/
                } else {
                        if (!curr->next) {
                                sq_addlast((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        } else {
                                sq_addafter((FAR sq_entry_t *)curr, (FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        }
                }
                pmdbg("add_pm_timer: timer with id %d added in linked list by using while loop\n", timer->id);
        }
        timer->status = ACTIVE;
}

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: pm_timer_set
 *
 * Description:
 *   This function adds a wakeup timer in the g_pmTimer_activeList. So that it will be
 *   invoked just before sleep when needed. It also removes the wakeup timer if
 *   its already present in the list.
 *
 * Parameters:
 *   id - id of the timer
 *   timer_interval - expected board sleep duration
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_timer_set(int id, unsigned int timer_interval)
{
        pm_wakeup_timer_t *timer = get_pm_timer(id);
        DEBUGASSERT(timer);
        irqstate_t state;

        /* handle the case when timer is already running.
         * This is not an expected case because app side should not
         * set the same timer before the same timer gets expired. */
        if (timer->status == RUNNING) {
                pmdbg("pm_timer_set: timer is already RUNNING!!!\n");
                /* TODO ??? */
                return PM_TIMER_FAIL;
        } 

        /* Remove the timer from the list if it is already present.
         * This is the case where system has not gone to sleep yet and
         * already the given timer expired and user wants to set it again.
         * possible scenarios:
         * 1. system is in pm lock period
         * 2. system takes too much time to sleep */
        if (timer->status == ACTIVE) {
                pmdbg("pm_timer_set: timer status is already ACTIVE!!!\n");
                remove_pm_timer(timer);
        }

        /* Now add the timer in the list 
         * Adding a wakeup timer in the linked list should be atomic.
         * Otherwise there is a chance of wrong ordering of the list.*/
        state = enter_critical_section();

        /* Check if pm state is locked 
         * if the timer is periodic, we might have locked the PM in the 
         * last pm_timer_callback(), so we should unlock the PM. 
         * These locks are specific to each wakeup timer.*/
        if (timer->is_periodic) {
                if (timer->is_pm_lock) {
                        pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
                        timer->is_pm_lock = false;
                        pmdbg("pm_timer_set: pm timer with id %d is unlocked\n", timer->id);
                }
        }

        add_pm_timer(timer, timer_interval);
        leave_critical_section(state);
        
        return PM_TIMER_SUCCESS;
}

/************************************************************************
 * Name: pm_timer_stop
 *
 * Description:
 *   This function removes a timer from the linked list
 *
 * Parameters:
 *   id - id of the timer
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_timer_stop(int id) 
{
        /* Check for all status cases ??? */
        pm_wakeup_timer_t *timer = get_pm_timer(id);
        DEBUGASSERT(timer);

        /* What if we want to stop the wakeup timer during it is 
         * running. */
        if (timer->status == RUNNING) {
                /* TODO */
        }

        remove_pm_timer(timer);
        return PM_TIMER_SUCCESS;
}

/************************************************************************
 * Name: pm_timer_delete
 *
 * Description:
 *   This function removes a wakeup timer from list if present and
 *   also fress the timer for other apps to use.
 *
 * Parameters:
 *   id - id of the timer
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_timer_delete(int id) 
{
        /* Check for all status cases ??? */
        pm_wakeup_timer_t *timer = get_pm_timer(id);
        DEBUGASSERT(timer);

        /* What if we want to delete the wakeup timer during it is 
         * running. */
        if (timer->status == RUNNING) {
                /* TODO */
        }

        remove_pm_timer(timer);

        /* Free the timer so that other app can use it.
         * Also this might not be that simple if we add dynamically
         * allocated timers. Need to check that ??? */
        timer->status = FREE;
        g_pmTimer_nfree++;
        DEBUGASSERT(g_pmTimer_nfree <= CONFIG_PM_MAX_WAKEUP_TIMER);

        return PM_TIMER_SUCCESS;
}
