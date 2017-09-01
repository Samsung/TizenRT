/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <stdint.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#define LOGM_TASK_PRORITY CONFIG_LOGM_TASK_PRIORITY
#define LOGM_TASK_STACKSIZE CONFIG_LOGM_TASK_STACKSIZE

#ifdef CONFIG_LOGM_BUFFER_SIZE
#define LOGM_BUFFER_SIZE CONFIG_LOGM_BUFFER_SIZE
#else
#define LOGM_BUFFER_SIZE (10240)
#endif

#ifdef CONFIG_LOGM_PRINT_INTERVAL
#define LOGM_PRINT_INTERVAL        CONFIG_LOGM_PRINT_INTERVAL
#else
#define LOGM_PRINT_INTERVAL        (1000)
#endif

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define LOGM_READY BIT(0)
#define LOGM_BUFFER_RESIZE_REQ BIT(1)
#define LOGM_BUFFER_OVERFLOW BIT(2)

#define LOGM_STATUS(a) (logm_status & (a))
#define LOGM_STATUS_SET(a) (logm_status |= (a))
#define LOGM_STATUS_CLEAR(a) (logm_status &= ~(a))

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

EXTERN int g_logm_head;
EXTERN int g_logm_tail;
EXTERN int g_logm_overflow_offset;
EXTERN int g_logm_dropmsg_count;
EXTERN char * g_logm_rsvbuf;
EXTERN int logm_bufsize;
EXTERN uint8_t logm_status;
EXTERN volatile int new_logm_bufsize;
EXTERN volatile int logm_print_interval;

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/
int logm_task(int argc, char *argv[]);
void logm_register_tashcmds(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __OS_LOGM_LOGM_H */
