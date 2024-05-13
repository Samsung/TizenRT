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
 * Name: pm_timer_create
 *
 * Description:
 *   This function returns a free pm timer structure pointer.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   pm timer
 *
 ************************************************************************/

pm_timer_t *pm_timer_create()
{
        pm_timer_t *timer;
        irqstate_t state;

        /* If we have pre-allocated timer available then use that */
        if (g_pm_timer_nfree > 0) {
                /* Creating a timer should be atomic */
                state = enter_critical_section();

                /* Remove the pm timer from the free list and decrement the 
                 * count of free pm timers */
                timer = (pm_timer_t *)sq_remfirst(&g_pm_timer_freelist);

                if (timer) {
                        DEBUGASSERT(g_pm_timer_nfree > 0);
                        g_pm_timer_nfree--;
                        timer->next = NULL;
                        timer->pid = getpid();
                        timer->flags = PM_STATIC;
                        pmvdbg("pid of the timer is %d\n", timer->pid);
                } else {
                        /* if timer is NULL, g_pm_timer_nfree must be zero, else assert */
                        DEBUGASSERT(g_pm_timer_nfree == 0);
                }
                leave_critical_section(state);
        } else {
                
                timer = (pm_timer_t *)kmm_malloc(sizeof(pm_timer_t));

                if (timer) {
                        timer->next = NULL;
                        timer->pid = getpid();
                        timer->flags = PM_ALLOCED;
                        pmvdbg("pid of the timer is %d\n", timer->pid);
                }
        }   

        return timer;
}
