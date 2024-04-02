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
 * pm/pm_timer/pm_timer_initialize.c
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

#include <tinyara/config.h>
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

/* The g_pm_activeList data structure is a singly linked list ordered by
 * pm wakeup timer expiration time.
 */

sq_queue_t g_pmTimer_activeList;

/* This is the number of free, pre-allocated pm wakeup timer structures in the
 * g_pm_list. 
 */

uint16_t g_pmTimer_nfree;

/* g_pm_list is a list of pre-allocated pm wakeup timers. The number of pm timers
 * in the list is a configuration item.
 */

pm_wakeup_timer_t g_pmTimer_list[CONFIG_PM_MAX_WAKEUP_TIMER];

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: pm_timer_initialize
 *
 * Description:
 * This function initializes the pm timer data structures
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   This function must be called early in the initialization sequence
 *   before the timer interrupt is attached and before any PM
 *   services are used.
 *
 ************************************************************************/

void pm_timer_initialize(void)
{
        pm_wakeup_timer_t *timer = g_pmTimer_list;

        /* Initialize pm timer list */

        sq_init(&g_pmTimer_activeList);

        /* The g_pmTimer_list must be initiated */
        for (int i = 0; i < 5; i++) {
                timer->id = i;
                timer->next = NULL;
                timer->status = FREE;
                timer++;
        }

        /* All pm timers are free */
        g_pmTimer_nfree = 5;
}
