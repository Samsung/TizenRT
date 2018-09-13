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

#ifndef __uv__error_header__
#define __uv__error_header__

#ifndef __uv_header__
#error Please include with uv.h
#endif

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(DEBUG)
#define ABORT()                  \
  do {                           \
    TDLOG("!!!!! ABORT !!!!!");  \
    exit(-1);                    \
  } while(0)
#else
#define ABORT() abort()
#endif

//-----------------------------------------------------------------------------
// errorno < uv_errorno.h

#define UV__EOF     (-4095)
#define UV__UNKNOWN (-4094)

#define UV__EAI_ADDRFAMILY  (-3000)
#define UV__EAI_AGAIN       (-3001)
#define UV__EAI_BADFLAGS    (-3002)
#define UV__EAI_CANCELED    (-3003)
#define UV__EAI_FAIL        (-3004)
#define UV__EAI_FAMILY      (-3005)
#define UV__EAI_MEMORY      (-3006)
#define UV__EAI_NODATA      (-3007)
#define UV__EAI_NONAME      (-3008)
#define UV__EAI_OVERFLOW    (-3009)
#define UV__EAI_SERVICE     (-3010)
#define UV__EAI_SOCKTYPE    (-3011)
#define UV__EAI_BADHINTS    (-3013)
#define UV__EAI_PROTOCOL    (-3014)

#if defined(EAFNOSUPPORT)
#define UV__EAFNOSUPPORT (-EAFNOSUPPORT)
#else
#define UV__EAFNOSUPPORT (-4089)
#endif

#if defined(EAGAIN)
#define UV__EAGAIN (-EAGAIN)
#else
#define UV__EAGAIN (-4088)
#endif

#if defined(EBUSY)
#define UV__EBUSY (-EBUSY)
#else
#define UV__EBUSY (-4082)
#endif

#if defined(ECANCELED)
#define UV__ECANCELED (-ECANCELED)
#else
#define UV__ECANCELED (-4081)
#endif

#if defined(ECONNREFUSED)
#define UV__ECONNREFUSED (-ECONNREFUSED)
#else
#define UV__ECONNREFUSED (-4078)
#endif

#if defined(EINVAL)
#define UV__EINVAL (-EINVAL)
#else
#define UV__EINVAL (-4071)
#endif

#if defined(ELOOP)
#define UV__ELOOP (-ELOOP)
#else
#define UV__ELOOP (-4067)
#endif

#if defined(ENAMETOOLONG)
#define UV__ENAMETOOLONG (-ENAMETOOLONG)
#else
#define UV__ENAMETOOLONG (-4064)
#endif

#if defined(ENOBUFS)
#define UV__ENOBUFS (-ENOBUFS)
#else
#define UV__ENOBUFS (-4060)
#endif

#if defined(ENOENT)
#define UV__ENOENT (-ENOENT)
#else
#define UV__ENOENT (-4058)
#endif

#if defined(ENOMEM)
#define UV__ENOMEM (-ENOMEM)
#else
#define UV__ENOMEM (-4057)
#endif

#if defined(ENOSPC)
#define UV__ENOSPC (-ENOSPC)
#else
#define UV__ENOSPC (-4055)
#endif

#if defined(ENOTDIR)
#define UV__ENOTDIR (-ENOTDIR)
#else
#define UV__ENOTDIR (-4052)
#endif

#if defined(EPERM)
#define UV__EPERM (-EPERM)
#else
#define UV__EPERM (-4048)
#endif

#if defined(ETIMEDOUT)
#define UV__ETIMEDOUT (-ETIMEDOUT)
#else
#define UV__ETIMEDOUT (-4039)
#endif

//-----------------------------------------------------------------------------
// errno map < uv.h

#define UV_ERRNO_MAP(XX)                                                      \
  XX(EAFNOSUPPORT, "address family not supported")                            \
  XX(EAI_ADDRFAMILY, "address family not supported")                          \
  XX(EAI_AGAIN, "temporary failure")                                          \
  XX(EAI_BADFLAGS, "bad ai_flags value")                                      \
  XX(EAI_BADHINTS, "invalid value for hints")                                 \
  XX(EAI_CANCELED, "request canceled")                                        \
  XX(EAI_FAIL, "permanent failure")                                           \
  XX(EAI_FAMILY, "ai_family not supported")                                   \
  XX(EAI_MEMORY, "out of memory")                                             \
  XX(EAI_NODATA, "no address")                                                \
  XX(EAI_NONAME, "unknown node or service")                                   \
  XX(EAI_OVERFLOW, "argument buffer overflow")                                \
  XX(EAI_PROTOCOL, "resolved protocol is unknown")                            \
  XX(EAI_SERVICE, "service not available for socket type")                    \
  XX(EAI_SOCKTYPE, "socket type not supported")                               \
  XX(EAGAIN, "resource temporarily unavailable")                              \
  XX(EBUSY, "resource busy or locked")                                        \
  XX(ECANCELED, "operation canceled")                                         \
  XX(ECONNREFUSED, "connection refused")                                      \
  XX(EINVAL, "invalid argument")                                              \
  XX(ELOOP, "too many symbolic links encountered")                            \
  XX(ENAMETOOLONG, "name too long")                                           \
  XX(ENOBUFS, "no buffer space available")                                    \
  XX(ENOENT, "no such file or directory")                                     \
  XX(ENOMEM, "not enough memory")                                             \
  XX(ENOSPC, "no space left on device")                                       \
  XX(ENOTDIR, "not a directory")                                              \
  XX(EPERM, "operation not permitted")                                        \
  XX(ETIMEDOUT, "connection timed out")                                       \
  XX(EOF, "end of file")                                                      \
 
//-----------------------------------------------------------------------------
#define XX(code, _) UV_ ## code = UV__ ## code,

typedef enum {
	UV_ERRNO_MAP(XX)
	UV_ERRNO_MAX = UV__EOF - 1
} uv_errno_t;

#undef XX

const char *uv_strerror(int err);
const char *uv_err_name(int err);

#ifdef __cplusplus
}
#endif
#endif							// __uv__error_header__
