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
 * pm/pm_timer/pm_timer_add.c
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

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: pm_timer_add
 *
 * Description:
 *   This function adds a wakeup timer in the g_pmTimer_activeList. So that it will be
 *   invoked just before sleep when needed. 
 * 
 * Parameters:
 *   timer_interval - expected board sleep duration
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_timer_add(unsigned int timer_interval)
{
        pm_wakeup_timer_t *timer = pm_timer_create();
        if (timer == NULL) {
                pmdbg("Unable to create pm timer\n");
                return PM_TIMER_FAIL;
        }

        irqstate_t state;

        /* Now add the timer in the list 
         * Adding a wakeup timer in the linked list should be atomic.
         * Otherwise there is a chance of wrong ordering of the list.*/
        state = enter_critical_section();
        
        /* updating the delay of the wakeup timer */
        timer->delay = timer_interval;

        /* Case where there are no timers in the list */

        if (g_pmTimer_activeList.head == NULL) {
                sq_addlast((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
        }
        
        /* We should add the timer in the sorted position of delay time */

         else {
                pm_wakeup_timer_t *curr;
                pm_wakeup_timer_t *prev;
                prev = curr = (pm_wakeup_timer_t *)g_pmTimer_activeList.head;
                unsigned int now = 0;

                /* Advance to positive time */
                while ((now += curr->delay) < 0 && curr->next) {
                        prev = curr;
                        curr = curr->next;
                }

                /* Advance past shorter delays */
                while (now <= timer->delay && curr->next) {
                        prev = curr;
                        curr = curr->next;
                        now += curr->delay;
                }

                /* timer should be added before the curr */
                if (timer->delay < now) {
                        
                        timer->delay -= (now - curr->delay);
                        curr->delay -= timer->delay;
                        if (curr == (pm_wakeup_timer_t *)g_pmTimer_activeList.head) {
                                sq_addfirst((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        } else {
                                sq_addafter((FAR sq_entry_t *)prev, (FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        }  

                /* timer expire time is greater than every other timer. 
                 * timer should be added to the end of the linked list*/
                } else {
                        timer->delay -= now;
                        if (!curr->next) {
                                sq_addlast((FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        } else {
                                sq_addafter((FAR sq_entry_t *)curr, (FAR sq_entry_t *)timer, &g_pmTimer_activeList);
                        }
                }
        }

        leave_critical_section(state);
        
        return PM_TIMER_SUCCESS;
}


