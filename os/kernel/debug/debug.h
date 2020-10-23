/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef __SCHED_DEBUG_DEBUG_H
#define __SCHED_DEBUG_DEBUG_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>
#include <tinyara/sched.h>
#include <sys/types.h>

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/
#if defined(CONFIG_ENABLE_STACKMONITOR) && defined(CONFIG_DEBUG)
void stackinfo_save_terminated(struct tcb_s *tcb);
#endif

void dbg_save_termination_info(struct tcb_s *tcb);

#endif							/* __SCHED_DEBUG_DEBUG_H */
