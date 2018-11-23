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

#ifndef __uv_header__
#define __uv_header__

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#include "queue.h"
#include "tree.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))

#include "uv__types.h"			// types should be placed in the first
#include "uv_platform.h"		// platform should be placed before extension
#include "uv_extension.h"		// extension should be placed before others

#include "uv__error.h"
#include "uv__handle.h"
#include "uv__loop.h"
#include "uv__idle.h"
#include "uv__timer.h"
#include "uv__inet.h"

#include "uv__poll.h"

#include "uv__async.h"
#include "uv__req.h"			// req should be placed before fs
#include "uv__fs.h"
#include "uv__fd.h"
#include "uv__stream.h"
#include "uv__tcp.h"
#include "uv__pipe.h"
#include "uv__thread.h"
#include "uv__signal.h"

#include "uv__threadpool.h"
#include "uv__getaddrinfo.h"

#include "uv__dir.h"
#include "uv__util.h"

#include "tuv_debuglog.h"

#undef UV_HANDLE_PRIVATE_FIELDS
#undef UV_ASYNC_PRIVATE_FIELDS
#undef UV_LOOP_PRIVATE_FIELDS
#undef UV_IDLE_PRIVATE_FIELDS
#undef UV_TIMER_PRIVATE_FIELDS
#undef UV_FS_PRIVATE_FIELDS
#undef UV_REQ_TYPE_PRIVATE
#undef UV_REQ_PRIVATE_FIELDS
#undef UV_FS_REQ_PRIVATE_FIELDS
#undef UV_WORK_PRIVATE_FIELDS
#undef UV_IO_PRIVATE_PLATFORM_FIELDS
#undef UV_TCP_PRIVATE_FIELDS
#undef UV_STREAM_PRIVATE_FIELDS
#undef UV_PIPE_PRIVATE_FIELDS
#undef UV_CONNECT_PRIVATE_FIELDS
#undef UV_WRITE_PRIVATE_FIELDS
#undef UV_SHUTDOWN_PRIVATE_FIELDS
#undef UV_GETADDRINFO_PRIVATE_FIELDS
#undef UV_SIGNAL_PRIVATE_FIELDS

#endif							// __uv_header__
