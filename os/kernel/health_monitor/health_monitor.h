/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#ifndef __KERNEL_HEALTH_MONITOR_H
#define __KERNEL_HEALTH_MONITOR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <tinyara/compiler.h>
#include <tinyara/health_monitor.h>
#include <tinyara/sched.h>

#ifdef CONFIG_HEALTH_MONITOR

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HEALTH_MONITOR_HEAP_CAPACITY     CONFIG_MAX_TASKS

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

bool health_monitor_is_tick_before(clock_t lhs, clock_t rhs);

void health_monitor_timer(void);
int health_monitor_get_next_check_time(clock_t *check_at);
void health_monitor_init_task(struct tcb_s *tcb);
void health_monitor_deinit_task(struct tcb_s *tcb);

#endif /* CONFIG_HEALTH_MONITOR */
#endif /* __KERNEL_HEALTH_MONITOR_H */
