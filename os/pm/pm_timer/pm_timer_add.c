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
 *   This function adds a wakeup timer in the g_pm_timer_activelist. So that it will be
 *   invoked just before sleep when needed. 
 * 
 * Parameters:
 *   pm_wakeup_timer_s pointer
 *
 * Return Value:
 *   None
 * 
 ************************************************************************/

void pm_timer_add(pm_timer_t *timer)
{
        /* Case where there are no timers in the list */

        if (g_pm_timer_activelist.head == NULL) {
                sq_addlast((FAR sq_entry_t *)timer, &g_pm_timer_activelist);
        }
        
        /* We should add the timer in the sorted position of delay time */

         else {
                pm_timer_t *curr;
                pm_timer_t *prev;
                pm_timer_t *next;
                prev = curr = (pm_timer_t *)g_pm_timer_activelist.head;
                int now = 0;

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
                        if (curr == (pm_timer_t *)g_pm_timer_activelist.head) {
                                sq_addfirst((FAR sq_entry_t *)timer, &g_pm_timer_activelist);
                        } else {
                                sq_addafter((FAR sq_entry_t *)prev, (FAR sq_entry_t *)timer, &g_pm_timer_activelist);
                        }  

                /* timer expire time is greater than every other timer. 
                 * timer should be added to the end of the linked list*/
                } else {
                        timer->delay -= now;
                        if (!curr->next) {
                                sq_addlast((FAR sq_entry_t *)timer, &g_pm_timer_activelist);
                        } else {
                                next = curr->next;
				next->delay -= timer->delay;
                                sq_addafter((FAR sq_entry_t *)curr, (FAR sq_entry_t *)timer, &g_pm_timer_activelist);
                        }
                }
        }

}


