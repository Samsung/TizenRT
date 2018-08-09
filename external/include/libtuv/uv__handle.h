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

#ifndef __uv__handle_header__
#define __uv__handle_header__

#ifndef __uv_header__
#error Please include with uv.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// uv_handle_t

#define UV_HANDLE_TYPE_MAP(XX)                                                \
  XX(ASYNC, async)                                                            \
  XX(HANDLE, handle)                                                          \
  XX(IDLE, idle)                                                              \
  XX(NAMED_PIPE, pipe)                                                        \
  XX(STREAM, stream)                                                          \
  XX(TCP, tcp)                                                                \
  XX(TIMER, timer)                                                            \
  XX(TTY, tty)                                                                \
  XX(POLL, poll)                                                              \
  XX(SIGNAL, signal)                                                          \

/*
  XX(ASYNC, async)                                                            \
  XX(CHECK, check)                                                            \
  XX(FS_EVENT, fs_event)                                                      \
  XX(FS_POLL, fs_poll)                                                        \
  XX(HANDLE, handle)                                                          \
  XX(IDLE, idle)                                                              \
  XX(NAMED_PIPE, pipe)                                                        \
  XX(POLL, poll)                                                              \
  XX(PREPARE, prepare)                                                        \
  XX(PROCESS, process)                                                        \
  XX(STREAM, stream)                                                          \
  XX(TCP, tcp)                                                                \
  XX(TIMER, timer)                                                            \
  XX(TTY, tty)                                                                \
  XX(UDP, udp)                                                                \
  XX(SIGNAL, signal)                                                          \
*/

//-----------------------------------------------------------------------------
#define XX(uc, lc) UV_##uc,

typedef enum {
	UV_UNKNOWN_HANDLE = 0,
	UV_HANDLE_TYPE_MAP(XX)
	UV_FILE,
	UV_HANDLE_TYPE_MAX
} uv_handle_type;

#undef XX
//-----------------------------------------------------------------------------

/* Leans on the fact that, on Linux, POLLRDHUP == EPOLLRDHUP. */
#ifdef POLLRDHUP
#define UV__POLLRDHUP POLLRDHUP
#else
#define UV__POLLRDHUP 0x2000
#endif

/* handle flags */
enum {
	UV_CLOSING = 0x0001,	/* uv_close() called but not finished. */
	UV_CLOSED = 0x0002,		/* close(2) finished. */
	UV_STREAM_READING = 0x0004,	/* uv_read_start() called. */
	UV_STREAM_SHUTTING = 0x0008,	/* uv_shutdown() called but not complete. */
	UV_STREAM_SHUT = 0x0010,	/* Write side closed. */
	UV_STREAM_READABLE = 0x0020,	/* The stream is readable */
	UV_STREAM_WRITABLE = 0x0040,	/* The stream is writable */
	UV_STREAM_BLOCKING = 0x0080,	/* Synchronous writes. */
	UV_STREAM_READ_PARTIAL = 0x0100,	/* read(2) read less than requested. */
	UV_STREAM_READ_EOF = 0x0200,	/* read(2) read EOF. */
	UV_TCP_NODELAY = 0x0400,	/* Disable Nagle. */
	UV_TCP_KEEPALIVE = 0x0800,	/* Turn on keep-alive. */
	UV_TCP_SINGLE_ACCEPT = 0x1000,	/* Only accept() when idle. */
	UV_HANDLE_IPV6 = 0x10000	/* Handle is bound to a IPv6 socket. */
};

//-----------------------------------------------------------------------------

#define UV_HANDLE_FIELDS                                                      \
  /* public */                                                                \
  void* data;                                                                 \
  /* read-only */                                                             \
  uv_loop_t* loop;                                                            \
  uv_handle_type type;                                                        \
  /* private */                                                               \
  unsigned int flags;                                                         \
  uv_close_cb close_cb;                                                       \
  void* handle_queue[2];                                                      \
  UV_HANDLE_PRIVATE_FIELDS                                                    \
 
// 'void* reserved[4]' came in to libuv @
//  06f9e1438ed20dd83618bca78759ea8635a79289
/*
  union {                                                                     \
    int fd;                                                                   \
    void* reserved[4];                                                        \
  } u;                                                                        \
*/

struct uv_handle_s {
	UV_HANDLE_FIELDS
};

#define uv__active_handle_add(h)                                              \
  do {                                                                        \
    (h)->loop->active_handles++;                                              \
  }                                                                           \
  while (0)

#define uv__active_handle_rm(h)                                               \
  do {                                                                        \
    (h)->loop->active_handles--;                                              \
  }                                                                           \
  while (0)

#define uv__is_active(h)                                                      \
  (((h)->flags & UV__HANDLE_ACTIVE) != 0)

#define uv__is_closing(h)                                                     \
  (((h)->flags & (UV_CLOSING | UV_CLOSED)) != 0)

#define uv__handle_start(h)                                                   \
  do {                                                                        \
    assert(((h)->flags & UV__HANDLE_CLOSING) == 0);                           \
    if (((h)->flags & UV__HANDLE_ACTIVE) != 0) break;                         \
    (h)->flags |= UV__HANDLE_ACTIVE;                                          \
    if (((h)->flags & UV__HANDLE_REF) != 0) uv__active_handle_add(h);         \
  }                                                                           \
  while (0)

#define uv__handle_stop(h)                                                    \
  do {                                                                        \
    assert(((h)->flags & UV__HANDLE_CLOSING) == 0);                           \
    if (((h)->flags & UV__HANDLE_ACTIVE) == 0) break;                         \
    (h)->flags &= ~UV__HANDLE_ACTIVE;                                         \
    if (((h)->flags & UV__HANDLE_REF) != 0) uv__active_handle_rm(h);          \
  }                                                                           \
  while (0)

#define uv__handle_init(loop_, h, type_)                                      \
  do {                                                                        \
    (h)->loop = (loop_);                                                      \
    (h)->type = (type_);                                                      \
    (h)->flags = UV__HANDLE_REF;  /* Ref the loop when active. */             \
    QUEUE_INIT(&(h)->handle_queue);                                           \
    QUEUE_INSERT_TAIL(&(loop_)->handles_queue, &(h)->handle_queue);           \
    uv__handle_platform_init(h);                                              \
  }                                                                           \
  while (0)

#define uv__handle_deinit(h)                                                  \
  do {                                                                        \
    QUEUE_REMOVE(&(h)->handle_queue);                                         \
    QUEUE_INIT(&(h)->handle_queue);                                           \
  }                                                                           \
  while (0)

#define uv__handle_ref(h)                                                     \
  do {                                                                        \
    if (((h)->flags & UV__HANDLE_REF) != 0) break;                            \
    (h)->flags |= UV__HANDLE_REF;                                             \
    if (((h)->flags & UV__HANDLE_CLOSING) != 0) break;                        \
    if (((h)->flags & UV__HANDLE_ACTIVE) != 0) uv__active_handle_add(h);      \
  }                                                                           \
  while (0)

#define uv__handle_unref(h)                                                   \
  do {                                                                        \
    if (((h)->flags & UV__HANDLE_REF) == 0) break;                            \
    (h)->flags &= ~UV__HANDLE_REF;                                            \
    if (((h)->flags & UV__HANDLE_CLOSING) != 0) break;                        \
    if (((h)->flags & UV__HANDLE_ACTIVE) != 0) uv__active_handle_rm(h);       \
  }                                                                           \
  while (0)

#define uv__has_ref(h)                                                       \
  (((h)->flags & UV__HANDLE_REF) != 0)

#define uv__has_active_handles(loop)                                          \
  ((loop)->active_handles > 0)

//-----------------------------------------------------------------------------

void uv_ref(uv_handle_t *);
void uv_unref(uv_handle_t *);
void uv_deinit(uv_loop_t *, uv_handle_t *);

int uv_is_closing(const uv_handle_t *handle);
int uv_is_active(const uv_handle_t *handle);

void uv_close(uv_handle_t *handle, uv_close_cb close_cb);

void uv_walk(uv_loop_t *loop, uv_walk_cb walk_cb, void *arg);

#ifdef UV__O_NONBLOCK
#define UV__F_NONBLOCK UV__O_NONBLOCK
#else
#define UV__F_NONBLOCK 1
#endif

#ifdef __cplusplus
}
#endif
#endif							// __uv__handle_header__
