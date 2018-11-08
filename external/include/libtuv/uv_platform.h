/* Copyright 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __uv__platform_tinyara_header__
#define __uv__platform_tinyara_header__

#include <sys/types.h>
#include <sys/uio.h>
#include <poll.h>				// nuttx poll // kbuild
#include <unistd.h>
#include <errno.h>

#include "uv__unix_platform.h"

#include <netdb.h>

/* for testing */
#define TUV_POLL_EVENTS_SIZE  32

#ifndef TUV_POLL_EVENTS_SIZE
#define TUV_POLL_EVENTS_SIZE  1024
#endif

#define TUV_TINYARA_IOV_MAX     TUV_POLL_EVENTS_SIZE	/* check this */

//-----------------------------------------------------------------------------

#define ENOTSUP       EOPNOTSUPP

#define STDIN_FILNO   0
#define STDOUT_FILNO  1
#define STDERR_FILENO 2

#ifndef SIGCHLD
#define SIGCHLD       17
#endif
#define SIGPROF       27

#ifndef CONFIG_NET_LWIP
#define TCP_NODELAY   1
#endif

#define _SC_CLK_TCK           0x0006
#define _SC_NPROCESSORS_ONLN  0x0061

#define CLOCK_MONOTONIC 1

//-----------------------------------------------------------------------------

#define UV__POLLIN    POLLIN	/* 0x01 */
#define UV__POLLOUT   POLLOUT	/* 0x02 */
#define UV__POLLERR   POLLERR	/* 0x04 */
#define UV__POLLHUP   POLLHUP	/* 0x08 */

//-----------------------------------------------------------------------------

#define SAVE_ERRNO(block)                                                     \
  do {                                                                        \
    int _saved_errno = get_errno();                                           \
    do { block; } while (0);                                                  \
    set_errno(_saved_errno);                                                  \
  }                                                                           \
  while (0)

//-----------------------------------------------------------------------------
// date time extension

// in (os)/uv_clock.cpp
uint64_t uv__hrtime(void);

#define uv__update_time(loop)                                                 \
  loop->time = uv__hrtime() / 1000000

uint64_t uv__time_precise(void);

//-----------------------------------------------------------------------------
// thread and mutex

#define UV_ONCE_INIT PTHREAD_ONCE_INIT

typedef pthread_t uv_thread_t;
typedef pthread_once_t uv_once_t;
typedef pthread_mutex_t uv_mutex_t;
typedef sem_t uv_sem_t;
typedef pthread_cond_t uv_cond_t;
typedef pthread_mutex_t uv_rwlock_t;	// no rwlock for nuttx

ssize_t readv(int __fd, const struct iovec *__iovec, int __count);
ssize_t writev(int __fd, const struct iovec *__iovec, int __count);

//-----------------------------------------------------------------------------
// etc
int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN 8

#endif							// __uv__platform_tinyara_header__
