/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_TASK_MANAGER_DRV_H
#define __INCLUDE_TINYARA_TASK_MANAGER_DRV_H

/* This file will be used to provide definitions to support
 * task manager framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define TASK_MANAGER_DRVPATH     "/dev/taskmgr"

#if defined(HAVE_TASK_GROUP) && !defined(CONFIG_DISABLE_PTHREAD)
struct tm_pthread_pid_s {
	pid_t parent_pid;
	pid_t child_pid;
};
typedef struct tm_pthread_pid_s tm_pthread_pid_t;
#endif

void task_manager_drv_register(void);

/**
 * @brief function for run exit callback when task is terminated.
 */
int task_manager_run_exit_cb(int pid);

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_TASK_MANAGER_DRV_H */
