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

#ifndef __TASK_MANAGER_INTERNAL_H__
#define __TASK_MANAGER_INTERNAL_H__

/* Command Types */
#define TASKMGT_REGISTER_TASK        0
#define TASKMGT_REGISTER_THREAD      1
#define TASKMGT_START                2
#define TASKMGT_TERMINATE            3
#define TASKMGT_RESTART              4
#define TASKMGT_PAUSE                5
#define TASKMGT_RESUME               6
#define TASKMGT_SCAN                 7
#define TASKMGT_UNICAST              8
#define TASKMGT_BROADCAST            9
#define TASKMGT_SCAN_NAME            10
#define TASKMGT_SCAN_HANDLE          11
#define TASKMGT_SCAN_GROUP           12
#define TASKMGT_UNREGISTER_TASK      13

/* Message Queue Values */
#define TM_MQ_PRIO   50
#define TM_PUBLIC_MQ "tm_public_mq"

/* Wrapper of allocation APIs */
#define TM_ALLOC(a)  malloc(a)
#define TM_FREE(a)   free(a)

#endif
