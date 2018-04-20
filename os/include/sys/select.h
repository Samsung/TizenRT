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
/****************************************************************************
 * include/sys/select.h
 *
 *   Copyright (C) 2008-2009, 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @defgroup SELECT_KERNEL SELECT
 * @brief Provides APIs for Select
 * @ingroup KERNEL
 *
 * @{
 */

/// @file sys/select.h
/// @brief synchronous I/O multiplexing APIs

#ifndef __INCLUDE_SYS_SELECT_H
#define __INCLUDE_SYS_SELECT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <time.h>

#if CONFIG_NFILE_DESCRIPTORS > 0 || CONFIG_NSOCKET_DESCRIPTORS > 0

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Get the total number of descriptors that we will have to support */

#define __SELECT_NDESCRIPTORS (CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS)

/* We will use a 32-bit bitsets to represent the set of descriptors.  How
 * many uint32_t's do we need to span all descriptors?
 */

#if __SELECT_NDESCRIPTORS <= 32
#define __SELECT_NUINT32 1
#elif __SELECT_NDESCRIPTORS <= 64
#define __SELECT_NUINT32 2
#elif __SELECT_NDESCRIPTORS <= 96
#define __SELECT_NUINT32 3
#elif __SELECT_NDESCRIPTORS <= 128
#define __SELECT_NUINT32 4
#elif __SELECT_NDESCRIPTORS <= 160
#define __SELECT_NUINT32 5
#elif __SELECT_NDESCRIPTORS <= 192
#define __SELECT_NUINT32 6
#elif __SELECT_NDESCRIPTORS <= 224
#define __SELECT_NUINT32 7
#elif __SELECT_NDESCRIPTORS <= 256
#define __SELECT_NUINT32 8
#else
#warning "Large fd_set needed"
#endif

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

typedef struct fd_set {
	uint8_t fd_bits[__SELECT_NUINT32 * 4];
} fd_set;

/****************************************************************************
 * Integration with LWIP
 *
 * select() can be defined in fs/vfs/fs_select.c or
 * or in net/socket/bsd_socket_api.c when CONFIG_DISABLE_POLL=y
 *
 * in the second case LWIP implementation will be used with own declatations
 *
 ****************************************************************************/

#if defined(CONFIG_DISABLE_POLL)

#if !defined(CONFIG_NET_SOCKET)
/* #error Both system poll and network sockets are disabled */
#else
#include <net/lwip/sockets.h>
#endif

#else

/* Standard helper macros */

#define FD_CLR(fd, set)   ((set)->fd_bits[(fd)/8]) &= ~(1 << ((fd) & 7))
#define FD_SET(fd, set)   ((set)->fd_bits[(fd)/8]) |= (1 << ((fd) & 7))
#define FD_ISSET(fd, set) ((set)->fd_bits[(fd)/8]) &  (1 << ((fd) & 7))
#define FD_ZERO(set)      memset(set, 0, sizeof(fd_set))

#endif							/* CONFIG_DISABLE_POLL */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @ingroup SELECT_KERNEL
 * @brief synchronous I/O multiplexing
 * @details @b #include <sys/select.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
EXTERN int select(int nfds, FAR fd_set *readfds, FAR fd_set *writefds, FAR fd_set *exceptfds, FAR struct timeval *timeout);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* CONFIG_NFILE_DESCRIPTORS || CONFIG_NSOCKET_DESCRIPTORS */

#endif							/* __INCLUDE_SYS_SELECT_H */
/**
 * @} */
