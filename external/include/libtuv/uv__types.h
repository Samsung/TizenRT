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

#ifndef __uv__types_header__
#define __uv__types_header__

#ifndef __uv_header__
#error Please include with uv.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// strcture types

// handle types
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_loop_s uv_loop_t;
typedef struct uv_timer_s uv_timer_t;
typedef struct uv_idle_s uv_idle_t;
typedef struct uv_async_s uv_async_t;
typedef struct uv_stream_s uv_stream_t;
typedef struct uv_tcp_s uv_tcp_t;
typedef struct uv_pipe_s uv_pipe_t;
typedef struct uv_poll_s uv_poll_t;
typedef struct uv_signal_s uv_signal_t;

// request types
typedef struct uv_req_s uv_req_t;
typedef struct uv_fs_s uv_fs_t;
typedef struct uv_connect_s uv_connect_t;
typedef struct uv_write_s uv_write_t;
typedef struct uv_shutdown_s uv_shutdown_t;
typedef struct uv_getaddrinfo_s uv_getaddrinfo_t;
typedef struct uv_work_s uv_work_t;

// ext types
typedef struct uv__io_s uv__io_t;
typedef struct uv_buf_s uv_buf_t;

// structure
struct uv__async;
struct addrinfo;

//-----------------------------------------------------------------------------
// callback types

typedef void (*uv_poll_cb)(uv_poll_t *handle, int status, int events);
typedef void (*uv_timer_cb)(uv_timer_t *handle);
typedef void (*uv_idle_cb)(uv_idle_t *handle);
typedef void (*uv_close_cb)(uv_handle_t *handle);
typedef void (*uv_async_cb)(uv_async_t *handle);

typedef void (*uv_thread_cb)(void *arg);

typedef void (*uv_fs_cb)(uv_fs_t *req);

typedef void (*uv__io_cb)(struct uv_loop_s *loop, struct uv__io_s *w, unsigned int events);
typedef void (*uv__async_cb)(struct uv_loop_s *loop, struct uv__async *w, unsigned int nevents);

typedef void (*uv_alloc_cb)(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
typedef void (*uv_read_cb)(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
typedef void (*uv_write_cb)(uv_write_t *req, int status);
typedef void (*uv_connect_cb)(uv_connect_t *req, int status);
typedef void (*uv_shutdown_cb)(uv_shutdown_t *req, int status);
typedef void (*uv_connection_cb)(uv_stream_t *server, int status);

typedef void (*uv_walk_cb)(uv_handle_t *handle, void *arg);

typedef void (*uv_work_cb)(uv_work_t *req);
typedef void (*uv_after_work_cb)(uv_work_t *req, int status);

typedef void (*uv_getaddrinfo_cb)(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
typedef void (*uv_signal_cb)(uv_signal_t* handle, int signum);

//-----------------------------------------------------------------------------

typedef struct {
	long tv_sec;
	long tv_nsec;
} uv_timespec_t;

typedef struct {
	uint64_t st_dev;
	uint64_t st_mode;
	uint64_t st_nlink;
	uint64_t st_uid;
	uint64_t st_gid;
	uint64_t st_rdev;
	uint64_t st_ino;
	uint64_t st_size;
	uint64_t st_blksize;
	uint64_t st_blocks;
	uint64_t st_flags;
	uint64_t st_gen;
	uv_timespec_t st_atim;
	uv_timespec_t st_mtim;
	uv_timespec_t st_ctim;
	uv_timespec_t st_birthtim;
} uv_stat_t;

//-----------------------------------------------------------------------------
// uv__work

struct uv__work {
	void (*work)(struct uv__work *w);
	void (*done)(struct uv__work *w, int status);
	struct uv_loop_s *loop;
	void *wq[2];
};

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif							// __uv__types_header__
