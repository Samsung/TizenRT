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

#ifndef __uv__internal_header__
#define __uv__internal_header__

//-----------------------------------------------------------------------------
// common internal functions

// in uv_idle.cpp
void uv__run_idle(uv_loop_t *loop);
void uv__idle_close(uv_idle_t *handle);

// in <platform>/uv_timer.cpp
void uv__run_timers(uv_loop_t *loop);
void uv__timer_close(uv_timer_t *handle);
int uv__next_timeout(const uv_loop_t *loop);

// in uv_poll.c
void uv__poll_close(uv_poll_t *handle);

// in uv_run.cpp
void uv__make_close_pending(uv_handle_t *handle);

// in uv_threadpool.cpp
void uv__work_submit(uv_loop_t *loop, struct uv__work *w, void (*work)(struct uv__work *w), void (*done)(struct uv__work *w, int status));

// in uv_fs.cpp
void uv__fs_scandir_cleanup(uv_fs_t *req);

#endif							// __uv__internal_header__
