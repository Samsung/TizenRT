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
#include <assert.h>

#include <uv.h>
#include "uv_internal.h"

static void uv__finish_close(uv_handle_t *handle)
{
	/* Note: while the handle is in the UV_CLOSING state now, it's still possible
	 * for it to be active in the sense that uv__is_active() returns true.
	 * A good example is when the user calls uv_shutdown(), immediately followed
	 * by uv_close(). The handle is considered active at this point because the
	 * completion of the shutdown req is still pending.
	 */
	assert(handle->flags & UV_CLOSING);
	assert(!(handle->flags & UV_CLOSED));
	handle->flags |= UV_CLOSED;

	switch (handle->type) {
	case UV_IDLE:
	case UV_ASYNC:
	case UV_TIMER:
	case UV_POLL:
	case UV_SIGNAL:
		break;

	case UV_NAMED_PIPE:
	case UV_TCP:
	case UV_TTY:
		uv__stream_destroy((uv_stream_t *) handle);
		break;

	default:
		assert(0);
		break;
	}

	uv__handle_unref(handle);
	QUEUE_REMOVE(&handle->handle_queue);
	QUEUE_INIT(&handle->handle_queue);

	if (handle->close_cb) {
		handle->close_cb(handle);
	}
}

static void uv__run_closing_handles(uv_loop_t *loop)
{
	uv_handle_t *p;
	uv_handle_t *q;

	p = loop->closing_handles;
	loop->closing_handles = NULL;

	while (p) {
		q = p->next_closing;
		uv__finish_close(p);
		p = q;
	}
}

static int uv__run_pending(uv_loop_t *loop)
{
	QUEUE *q;
	QUEUE pq;
	uv__io_t *w;

	if (QUEUE_EMPTY(&loop->pending_queue)) {
		return 0;
	}

	QUEUE_INIT(&pq);
	q = QUEUE_HEAD(&loop->pending_queue);
	QUEUE_SPLIT(&loop->pending_queue, q, &pq);

	while (!QUEUE_EMPTY(&pq)) {
		q = QUEUE_HEAD(&pq);
		QUEUE_REMOVE(q);
		QUEUE_INIT(q);
		w = QUEUE_DATA(q, uv__io_t, pending_queue);
		w->cb(loop, w, UV__POLLOUT);
	}

	return 1;
}

//-----------------------------------------------------------------------------

int uv_backend_timeout(const uv_loop_t *loop)
{
	if (loop->stop_flag != 0) {
		return 0;
	}

	if (!uv__has_active_handles(loop) && !uv__has_active_reqs(loop)) {
		return 0;
	}

	if (!QUEUE_EMPTY(&loop->idle_handles)) {
		return 0;
	}

	if (loop->closing_handles) {
		return 0;
	}

	return uv__next_timeout(loop);
}

int uv_run(uv_loop_t *loop, uv_run_mode mode)
{
	int timeout;
	int r;
	int ran_pending;

	r = uv__loop_alive(loop);
	if (!r) {
		uv__update_time(loop);
	}

	while (r != 0 && loop->stop_flag == 0) {
		uv__update_time(loop);
		uv__run_timers(loop);
		ran_pending = uv__run_pending(loop);
		uv__run_idle(loop);

		timeout = 0;
		if ((mode == UV_RUN_ONCE && !ran_pending) || mode == UV_RUN_DEFAULT) {
			timeout = uv_backend_timeout(loop);
		}

		uv__io_poll(loop, timeout);
		uv__run_closing_handles(loop);

		if (mode == UV_RUN_ONCE) {
			// read libuv/src/core.c comments
			uv__update_time(loop);
			uv__run_timers(loop);
		}

		r = uv__loop_alive(loop);
		if (mode == UV_RUN_ONCE || mode == UV_RUN_NOWAIT) {
			break;
		}
	}
	uv__run_closing_handles(loop);

	if (loop->stop_flag != 0) {
		loop->stop_flag = 0;
	}

	return r;
}
