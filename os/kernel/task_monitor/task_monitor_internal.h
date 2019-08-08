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

#ifndef __SCHED_TASK_MONITOR_TASK_MONITOR_INTERNAL_H
#define __SCHED_TASK_MONITOR_TASK_MONITOR_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#define NOT_MONITORED (-1)
#define MONITORED     (0)

#ifdef CONFIG_TASK_MONITOR

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
int task_monitor(int argc, char *argv[]);
void task_monitor_update_list(int pid, int status);

#endif							/* CONFIG_TASK_MONITOR */
#endif							/* __SCHED_TASK_MONITOR_TASK_MONITOR_INTERNAL_H */
