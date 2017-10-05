/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

///@file tinyara/ringbuf.h
///@brief ttrace ringbuffer APIs

#ifndef __INCLUDE_TINYARA_TTRACE_RINGBUF_H
#define __INCLUDE_TINYARA_TTRACE_RINGBUF_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

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

struct ringbuf {
	char buffer[CONFIG_TTRACE_BUFSIZE];
	size_t bufsize;
	uint16_t index;
	int is_overwritten;
	int is_overwritable;
};

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

void printBuf(char *buf, struct ringbuf *rbp);
ssize_t ringbuf_write(FAR const char *buffer, size_t len, struct ringbuf *rbp);
ssize_t ringbuf_read(char *buffer, size_t len, struct ringbuf *rbp);

#if defined(__cplusplus)
}
#endif
#endif							/* __INCLUDE_TINYARA_TTRACE_RINGBUF_H */
/** @} */
