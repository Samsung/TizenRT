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

///@file tinyara/ttrace.h
///@brief ttrace APIs

#ifndef __INCLUDE_TINYARA_TTRACE_H
#define __INCLUDE_TINYARA_TTRACE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#ifdef CONFIG_TTRACE
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <debug.h>
#include <time.h>
#include <sys/types.h>

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/
#define TTRACE_START               's'
#define TTRACE_OVERWRITE           'o'
#define TTRACE_FINISH              'f'
#define TTRACE_INFO                'i'
#define TTRACE_SELECTED_TAG        't'
#define TTRACE_FUNC_TAG            'g'
#define TTRACE_SET_BUFSIZE         'z'
#define TTRACE_USED_BUFSIZE        'u'
#define TTRACE_BUFFER              'b'
#define TTRACE_DUMP                'd'
#define TTRACE_PRINT               'p'

#define TTRACE_CODE_VARIABLE        0
#define TTRACE_CODE_UNIQUE         (1 << 7)

#define TTRACE_MSG_BYTES            32
#define TTRACE_COMM_BYTES           12
#define TTRACE_BYTE_ALIGN           4

#define TTRACE_NODATA              -2
#define TTRACE_INVALID             -1
#define TTRACE_VALID                0

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
struct sched_message {          // total 32B
	pid_t prev_pid;                     // 2B
	uint8_t prev_prio;                  // 1B
	uint8_t prev_state;                 // 1B
	char prev_comm[TTRACE_COMM_BYTES];  // 12B
	pid_t next_pid;                     // 2B
	uint8_t next_prio;                  // 1B
	int8_t pad;                         // 1B
	char next_comm[TTRACE_COMM_BYTES];  // 12B
};

union trace_message {              // total 32B
	char message[TTRACE_MSG_BYTES];  // 32B, message(256b)
	struct sched_message sched_msg;  // 32B
};

struct trace_packet {        // total 44 byte(message), 12byte(uid)
	struct timeval ts;         // 8B, time_t(32b) + long(32b)
	pid_t pid;                 // 2B, int16_t(16b)
	char event_type;           // 1B, char(8b)
	int8_t codelen;            // 1B, code(1b) + variable length(7b) or uid(7b)
	union trace_message msg;   // 32B
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @ingroup TTRACE_LIBC
 * @brief writes a trace log with string to indicate that a event has begun
 * @details @b #include <tinyara/ttrace.h>
 * @param[in] tag number for tag
 * @param[in] str unique strings like function name for distinguishing events
 * @return On success, TTRACE_VALID is returned. On failure, ERROR is returned and errno is set appropriately.
 * @since TizenRT v1.1
 */
int trace_begin(int tag, char *str, ...);

/**
 * @ingroup TTRACE_LIBC
 * @brief writes a trace log with unique id to indicate that a event has begun
 * @details @b #include <tinyara/ttrace.h>
 * @param[in] tag number for tag
 * @param[in] uniqueid unique id for distinguishing events
 * @return On success, TTRACE_VALID is returned. On failure, TTRACE_INVALID is returned and errno is set appropriately.
 * @since TizenRT v1.1
 */
int trace_begin_uid(int tag, int8_t uniqueid);

/**
 * @ingroup TTRACE_LIBC
 * @brief writes a trace log to indicate that the event has ended
 * @details @b #include <tinyara/ttrace.h>
 * @param[in] tag number for tag
 * @return On success, TTRACE_VALID is returned. On failure, TTRACE_INVALID is returned and errno is set appropriately.
 * @since TizenRT v1.1
 */
int trace_end(int tag);

/**
 * @ingroup TTRACE_LIBC
 * @brief writes a trace log to indicate that a event has ended
 * @details @b #include <tinyara/ttrace.h>
 * @param[in] tag number for tag
 * @return On success, TTRACE_VALID is returned. On failure, TTRACE_INVALID is returned and errno is set appropriately.
 * @since TizenRT v1.1
 */
int trace_end_uid(int tag);

/**
 * @ingroup TTRACE_LIBC
 * @brief writes a trace log for scheduler events
 * @details @b #include <tinyara/ttrace.h>
 * @param[in] prev tcb of current task
 * @param[in] next tcb of next task which will be switched
 * @return On success, TTRACE_VALID is returned. On failure, TTRACE_INVALID is returned and errno is set appropriately.
 * @since TizenRT v1.1
 */
int trace_sched(struct tcb_s *prev, struct tcb_s *next);
#else
#define trace_begin(a, b, ...)
#define trace_begin_uid(a, b)
#define trace_end(a)
#define trace_end_uid(a)
#define trace_sched(a, b)

#if defined(__cplusplus)
}
#endif

#endif /* CONFIG_TTRACE */
#endif /* __INCLUDE_TINYARA_TTRACE_INTERNAL_H */
/**
 * @}
 */
