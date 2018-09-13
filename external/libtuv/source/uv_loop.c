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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <uv.h>
#include "heap-inl.h"

static uv_loop_t default_loop_struct;
static uv_loop_t *default_loop_ptr = NULL;

int uv_loop_init(uv_loop_t *loop)
{
	int err;

	memset(loop, 0, sizeof(uv_loop_t));
	heap_init((struct heap *)&loop->timer_heap);

	QUEUE_INIT(&loop->wq);
	QUEUE_INIT(&loop->active_reqs);
	QUEUE_INIT(&loop->idle_handles);
	QUEUE_INIT(&loop->async_handles);
	QUEUE_INIT(&loop->handles_queue);

	loop->nfds = 0;
	loop->watchers = NULL;
	loop->nwatchers = 0;
	QUEUE_INIT(&loop->pending_queue);
	QUEUE_INIT(&loop->watcher_queue);

	loop->closing_handles = NULL;
	uv__update_time(loop);
	uv__async_init(&loop->async_watcher);
	loop->backend_fd = -1;
	loop->emfile_fd = -1;

	loop->signal_pipefd[0] = -1;
	loop->signal_pipefd[1] = -1;

	loop->timer_counter = 0;
	loop->stop_flag = 0;

	err = uv__platform_loop_init(loop);
	if (err) {
		return err;
	}

	uv__signal_global_once_init();

	if (uv_rwlock_init(&loop->cloexec_lock)) {
		TDLOG("uv_loop_init rwlock abort");
		ABORT();
	}

	if (uv_mutex_init(&loop->wq_mutex)) {
		TDLOG("uv_loop_init mutex abort");
		ABORT();
	}

	if (uv_async_init(loop, &loop->wq_async, uv__work_done)) {
		TDLOG("uv_loop_init async abort");
		ABORT();
	}

	uv__handle_unref(&loop->wq_async);
	loop->wq_async.flags |= UV__HANDLE_INTERNAL;

	// add update timer here for slow systems
	uv__update_time(loop);

	return 0;
}

void uv__loop_close(uv_loop_t *loop)
{
	uv__platform_loop_delete(loop);
	uv__async_stop(loop, &loop->async_watcher);

	if (loop->emfile_fd != -1) {
		uv__close(loop->emfile_fd);
		loop->emfile_fd = -1;
	}

	if (loop->backend_fd != -1) {
		uv__close(loop->backend_fd);
		loop->backend_fd = -1;
	}

	uv_mutex_lock(&loop->wq_mutex);
	assert(QUEUE_EMPTY(&loop->wq) && "thread pool work queue not empty!");
	assert(!uv__has_active_reqs(loop));
	uv_mutex_unlock(&loop->wq_mutex);
	uv_mutex_destroy(&loop->wq_mutex);

	/*
	 * Note that all thread pool stuff is finished at this point and
	 * it is safe to just destroy rw lock
	 */
	uv_rwlock_destroy(&loop->cloexec_lock);

	free(loop->watchers);
	loop->watchers = NULL;
	loop->nwatchers = 0;
}

uv_loop_t *uv_default_loop(void)
{
	if (default_loop_ptr != NULL) {
		return default_loop_ptr;
	}

	if (uv_loop_init(&default_loop_struct)) {
		return NULL;
	}

	default_loop_ptr = &default_loop_struct;
	return default_loop_ptr;
}

int uv_loop_close(uv_loop_t *loop)
{
	QUEUE *q;
	uv_handle_t *h;

	uv__signal_loop_cleanup(loop);

	if (!QUEUE_EMPTY(&(loop)->active_reqs)) {
		TDDLOG("uv_loop_close active_req exist, cancel");
		return UV_EBUSY;
	}

	uv_async_deinit(loop, &loop->wq_async);

	QUEUE_FOREACH(q, &loop->handles_queue) {
		h = QUEUE_DATA(q, uv_handle_t, handle_queue);
		if (!(h->flags & UV__HANDLE_INTERNAL)) {
			TDDLOG("uv_loop_close handle(%p) exist, cancel", h);
			return UV_EBUSY;
		}
	}

	uv__loop_close(loop);

	if (loop == default_loop_ptr) {
		default_loop_ptr = NULL;
	}

	return 0;
}

void uv_update_time(uv_loop_t *loop)
{
	uv__update_time(loop);
}

uint64_t uv_now(const uv_loop_t *loop)
{
	return loop->time;
}

void uv_stop(uv_loop_t* loop) {
	loop->stop_flag = 1;
}

int uv__loop_alive(const uv_loop_t *loop)
{
	return uv__has_active_handles(loop) || uv__has_active_reqs(loop) || loop->closing_handles != NULL;
}

int uv_loop_alive(const uv_loop_t *loop)
{
	return uv__loop_alive(loop);
}
