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

#ifndef __INCLUDE_TINYARA_TASKMGT_H
#define __INCLUDE_TINYARA_TASKMGT_H

/* This file will be used to provide definitions to support
 * task manager framework
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_TASKMGT_DRIVER

/****************************************************************************
 * Public Data
 ****************************************************************************/
struct task_list_s {
    const char *name;
    const char *entry;
    const int priority;
    const int stacksize;
};
typedef struct task_list_s task_list_t;

struct tm_request_s {
    int cmd;
    int tid;
    int gid;
    char *name;
    int permission;
    bool returnable;
    char *q_name;
}
typedef struct tm_request_s tm_request_t;

struct tm_response_t {
    int status;
}

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration
 *
 * CONFIG_TASKMGT_DRIVER - Enables task management driver support
 */

/* IOCTL Commands */
#define TASKMGT_REGISTER_TASK       0
#define TASKMGT_REGISTER_THREAD     1
#define TASKMGT_UNREGISTER          2
#define TASKMGT_START               3
#define TASKMGT_TERMINATE           4
#define TASKMGT_PAUSE               5
#define TASKMGT_RESUME              6
#define TASKMGT_SCAN                7
#define TASKMGT_UNICAST             8
#define TASKMGT_BROADCAST           9

#define TM_MQ_PRIO 50
#define TM_PUBLIC_MQ "tm_public_mq"
#define TM_PRIVATE_MQ "tm_priv_mq"
mqd_t g_tm_send_mqfd;
mqd_t g_tm_recv_mqfd;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: taskmgt_register
 *
 * Description:
 *   This function creates a device node like "/dev/taskmgt" which will be
 *   used by the task manager which manages tasks
 *
 ****************************************************************************/

void taskmgt_register(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_KERNEL_TEST_DRV */
#endif							/* __INCLUDE_TINYARA_TEST_FW_H */
