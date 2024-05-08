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
 * pm/pm_timer/pm_timer.h
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

#ifndef __PM_TIMER_H
#define __PM_TIMER_H

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <assert.h>
#include <debug.h>
#include <stdint.h>
#include <stdbool.h>
#include <queue.h>

#define PM_TIMER_FAIL     -1
#define PM_TIMER_SUCCESS   0

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Public Type Declarations
 ************************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* PM timer Definitions *************************************************/
/* Flag bits for the flags field of struct pm_wakeup_timer_s */

#define PM_STATIC      (1 << 0)	        /* Bit 0: pm timer is statically allocated */
#define PM_ALLOCED      (1 << 1)	/* Bit 1: pm timer is allocated at run time */

#define PM_ISALLOCED(w)  (((w)->flags & PM_ALLOCED) != 0)
#define PM_ISSTATIC(w)   (((w)->flags & PM_STATIC) != 0)

/* The g_pm_timer_freelist is a singly linked list of pm timers available
 * to the system */

extern sq_queue_t g_pm_timer_freelist;

/* The g_pm_timer_activelist data structure is a singly linked list ordered by
 * pm wakeup timer expiration time.
 */

extern sq_queue_t g_pm_timer_activelist;

/* This is the number of free, pre-allocated pm wakeup timer structures in the
 * g_pm_timer_freelist. 
 */

extern uint16_t g_pm_timer_nfree;

/************************************************************************
 * Public Function Prototypes
 ************************************************************************/

/************************************************************************
 * Name: pm_timer_create
 *
 * Description:
 *   This function creates a pm timer structure for the user requested and
 *   initializes the timer fields and returns the id.
 *
 * Parameters:
 *   is_periodic - if the timer should be periodic
 *
 * Return Value:
 *   id of the timer
 *
 ************************************************************************/

pm_timer_t *pm_timer_create();

/************************************************************************
 * Name: pm_timer_delete
 *
 * Description:
 *   This function deletes a pm timer. It either returns it to the 
 *   g_pm_timer_freelist or frees it if it was created dynamically.
 *
 * Parameters:
 *   pm timer to be deleted
 *
 * Return Value:
 *   None
 *
 ************************************************************************/

void pm_timer_delete(pm_timer_t *timer) ;

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __PM_TIMER_H */
