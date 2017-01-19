/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#ifndef __OS_LOGM_LOGM_H
#define __OS_LOGM_LOGM_H

#include <queue.h>
#include <stdio.h>
#include <tinyara/config.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#define LOGM_RSVBUF_COUNT CONFIG_LOGM_BUFFER_COUNT
#define LOGM_TASK_PRORITY CONFIG_LOGM_TASK_PRIORITY
#define LOGM_TASK_STACKSIZE CONFIG_LOGM_TASK_STACKSIZE
#define LOGM_MAX_MSG_LEN CONFIG_LOGM_MAX_MSG_LENGTH
#define LOGM_PRINTERR_AND_RETURN() do { dbg("LOGM Launch Failed \n"); return; } while (0)

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

/* Structure for a single debug message */

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

EXTERN volatile int g_logm_isready;

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/
int logm_task(int argc, char *argv[]);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __OS_LOGM_LOGM_H */
