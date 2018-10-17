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

#ifndef __uv_unix_extenstion_header__
#define __uv_unix_extenstion_header__

#ifdef __cplusplus
extern "C" {
#endif

#define UV_HANDLE_PRIVATE_FIELDS                                              \
  uv_handle_t* next_closing;                                                  \
 
#define UV_POLL_PRIVATE_FIELDS                                                \
  uv__io_t io_watcher;

#define UV_IDLE_PRIVATE_FIELDS                                                \
  uv_idle_cb idle_cb;                                                         \
  void* queue[2];                                                             \
 

#define UV_TIMER_PRIVATE_FIELDS                                               \
  uv_timer_cb timer_cb;                                                       \
  void* heap_node[3];                                                         \
  uint64_t timeout;                                                           \
  uint64_t repeat;                                                            \
  uint64_t start_id;

#define UV_REQ_TYPE_PRIVATE		/* empty */

#define UV_REQ_PRIVATE_FIELDS	/* empty */

#define UV_FS_PRIVATE_FIELDS                                                  \
  const char *new_path;                                                       \
  uv_file file;                                                               \
  int flags;                                                                  \
  mode_t mode;                                                                \
  unsigned int nbufs;                                                         \
  uv_buf_t* bufs;                                                             \
  off_t off;                                                                  \
  double atime;                                                               \
  double mtime;                                                               \
  struct uv__work work_req;                                                   \
  uv_buf_t bufsml[4];                                                         \
								/*
								   const char *new_path;                                                       \
								   uv_file file;                                                               \
								   int flags;                                                                  \
								   mode_t mode;                                                                \
								   unsigned int nbufs;                                                         \
								   uv_buf_t* bufs;                                                             \
								   off_t off;                                                                  \
								   uv_uid_t uid;                                                               \
								   uv_gid_t gid;                                                               \
								   double atime;                                                               \
								   double mtime;                                                               \
								   struct uv__work work_req;                                                   \
								   uv_buf_t bufsml[4];                                                         \
								 */

#define UV_WORK_PRIVATE_FIELDS                                                \
  struct uv__work work_req;

#define UV_ASYNC_PRIVATE_FIELDS                                               \
  uv_async_cb async_cb;                                                       \
  void* queue[2];                                                             \
  int pending;                                                                \
 

#define UV_IO_PRIVATE_PLATFORM_FIELDS	/* empty */

#define UV_TCP_PRIVATE_FIELDS	/* empty */

#define UV_CONNECT_PRIVATE_FIELDS                                             \
  void* queue[2];                                                             \
 

#define UV_WRITE_PRIVATE_FIELDS                                               \
  void* queue[2];                                                             \
  unsigned int write_index;                                                   \
  uv_buf_t* bufs;                                                             \
  unsigned int nbufs;                                                         \
  int error;                                                                  \
  uv_buf_t bufsml[4];                                                         \
 
#define UV_SIGNAL_PRIVATE_FIELDS                                              \
  /* RB_ENTRY(uv_signal_s) tree_entry; */                                     \
  struct {                                                                    \
    struct uv_signal_s* rbe_left;                                             \
    struct uv_signal_s* rbe_right;                                            \
    struct uv_signal_s* rbe_parent;                                           \
    int rbe_color;                                                            \
  } tree_entry;                                                               \
  /* Use two counters here so we don have to fiddle with atomics. */          \
  unsigned int caught_signals;                                                \
  unsigned int dispatched_signals;

#define UV_SHUTDOWN_PRIVATE_FIELDS	/* empty */

//-----------------------------------------------------------------------------
// as loop uses async, handle and async needs to be included here

#include "uv__handle.h"
#include "uv__async.h"
#include "uv__signal.h"

//-----------------------------------------------------------------------------
// loop, this needs to be at the bottom

#define UV_LOOP_PRIVATE_FIELDS                                                \
  unsigned long flags;                                                        \
  int backend_fd;                                                             \
  void* pending_queue[2];                                                     \
  void* watcher_queue[2];                                                     \
  uv__io_t** watchers;                                                        \
  unsigned int nwatchers;                                                     \
  unsigned int nfds;                                                          \
  void* wq[2];                                                                \
  uv_mutex_t wq_mutex;                                                        \
  uv_async_t wq_async;                                                        \
  uv_rwlock_t cloexec_lock;                                                   \
  uv_handle_t* closing_handles;                                               \
  void* idle_handles[2];                                                      \
  void* async_handles[2];                                                     \
  struct uv__async async_watcher;                                             \
  struct {                                                                    \
    void* min;                                                                \
    unsigned int nelts;                                                       \
  } timer_heap;                                                               \
  uint64_t timer_counter;                                                     \
  uint64_t time;                                                              \
  int signal_pipefd[2];                                                       \
  uv__io_t signal_io_watcher;                                                 \
  uv_signal_t child_watcher;                                                  \
  int emfile_fd;                                                              \
  UV_PLATFORM_LOOP_FIELDS                                                     \
 
// timer_heap should be same as 'struct heap' in heap-inl.h

/*
  unsigned long flags;                                                        \
  int backend_fd;                                                             \
  void* pending_queue[2];                                                     \
  void* watcher_queue[2];                                                     \
  uv__io_t** watchers;                                                        \
  unsigned int nwatchers;                                                     \
  unsigned int nfds;                                                          \
  void* wq[2];                                                                \
  uv_mutex_t wq_mutex;                                                        \
  uv_async_t wq_async;                                                        \
  uv_rwlock_t cloexec_lock;                                                   \
  uv_handle_t* closing_handles;                                               \
  void* process_handles[2];                                                   \
  void* prepare_handles[2];                                                   \
  void* check_handles[2];                                                     \
  void* idle_handles[2];                                                      \
  void* async_handles[2];                                                     \
  struct uv__async async_watcher;                                             \
  struct {                                                                    \
    void* min;                                                                \
    unsigned int nelts;                                                       \
  } timer_heap;                                                               \
  uint64_t timer_counter;                                                     \
  uint64_t time;                                                              \
  int signal_pipefd[2];                                                       \
  uv__io_t signal_io_watcher;                                                 \
  uv_signal_t child_watcher;                                                  \
  int emfile_fd;                                                              \
  UV_PLATFORM_LOOP_FIELDS                                                     \
*/

#define UV_PIPE_PRIVATE_FIELDS                                                \
  const char* pipe_fname;		/* strdup'ed */

#define UV_STREAM_PRIVATE_FIELDS                                              \
  uv_connect_t *connect_req;                                                  \
  uv_shutdown_t *shutdown_req;                                                \
  uv__io_t io_watcher;                                                        \
  void* write_queue[2];                                                       \
  void* write_completed_queue[2];                                             \
  uv_connection_cb connection_cb;                                             \
  int delayed_error;                                                          \
  int accepted_fd;                                                            \
  void* queued_fds;                                                           \
  UV_STREAM_PRIVATE_PLATFORM_FIELDS

//-----------------------------------------------------------------------------
// should be here for its used inside extension

enum {
	UV__HANDLE_INTERNAL = 0x8000,
	UV__HANDLE_ACTIVE = 0x4000,
	UV__HANDLE_REF = 0x2000,
	UV__HANDLE_CLOSING = 0	/* no-op on unix */
};

//-----------------------------------------------------------------------------
// getaddrinfo

#define UV_GETADDRINFO_PRIVATE_FIELDS                                         \
  struct uv__work work_req;                                                   \
  uv_getaddrinfo_cb cb;                                                       \
  struct addrinfo* hints;                                                     \
  char* hostname;                                                             \
  char* service;                                                              \
  struct addrinfo* addrinfo;                                                  \
  int retcode;

#ifdef __cplusplus
}
#endif
#endif							// __uv_unix_extenstion_header__
