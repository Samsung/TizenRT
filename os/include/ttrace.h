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
/**
 * @defgroup TTRACE_LIBC TTRACE
 * @ingroup KERNEL
 *
 * @{
 */

///@file ttrace.h
///@brief ttrace APIs

#ifndef __INCLUDE_TTRACE_H
#define __INCLUDE_TTRACE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/
#define TTRACE_TAG_ALL             -1
#define TTRACE_TAG_OFF             0
#define TTRACE_TAG_APPS            (1 << 0)
#define TTRACE_TAG_LIBS            (1 << 1)
#define TTRACE_TAG_LOCK            (1 << 2)
#define TTRACE_TAG_TASK            (1 << 3)
#define TTRACE_TAG_IPC             (1 << 4)

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif

#ifdef CONFIG_TTRACE
int trace_begin(int tag, char *str, ...);
int trace_begin_u(int tag, int8_t uid);
int trace_end(int tag);
int trace_end_u(int tag);
int trace_sched(struct tcb_s *prev, struct tcb_s *next);
#else
#define trace_begin(a, b, ...)
#define trace_begin_u(a, b)
#define trace_end(a)
#define trace_end_u(a)
#define trace_sched(a, b)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_TTRACE_H */
/** @} */
