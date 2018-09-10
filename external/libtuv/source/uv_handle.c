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

#include <assert.h>
#include <string.h>

#include <uv.h>
#include "uv_internal.h"

//-----------------------------------------------------------------------------

void uv__make_close_pending(uv_handle_t *handle)
{
	assert(handle->flags & UV_CLOSING);
	assert(!(handle->flags & UV_CLOSED));
	handle->next_closing = handle->loop->closing_handles;
	handle->loop->closing_handles = handle;
}

//-----------------------------------------------------------------------------

void uv_close(uv_handle_t *handle, uv_close_cb close_cb)
{
	assert(!(handle->flags & (UV_CLOSING | UV_CLOSED)));

	handle->flags |= UV_CLOSING;
	handle->close_cb = close_cb;

	switch (handle->type) {
	case UV_TTY:
		uv__stream_close((uv_stream_t *) handle);
		break;

	case UV_TCP:
		uv__tcp_close((uv_tcp_t *) handle);
		break;

	case UV_IDLE:
		uv__idle_close((uv_idle_t *) handle);
		break;

	case UV_ASYNC:
		uv__async_close((uv_async_t *) handle);
		break;

	case UV_TIMER:
		uv__timer_close((uv_timer_t *) handle);
		break;

	case UV_POLL:
		uv__poll_close((uv_poll_t *) handle);
		break;

	case UV_SIGNAL:
		uv__signal_close((uv_signal_t*) handle);
		/* Signal handles may not be closed immediately. The signal code will */
		/* itself close uv__make_close_pending whenever appropriate. */
		return;

	default:
		assert(0);
	}

	uv__make_close_pending(handle);
}

//-----------------------------------------------------------------------------

int uv_is_closing(const uv_handle_t *handle)
{
	return uv__is_closing(handle);
}

int uv_is_active(const uv_handle_t *handle)
{
	return uv__is_active(handle);
}

void uv_ref(uv_handle_t *handle)
{
	uv__handle_ref(handle);
}

void uv_unref(uv_handle_t *handle)
{
	uv__handle_unref(handle);
}

//-----------------------------------------------------------------------------

void uv_walk(uv_loop_t *loop, uv_walk_cb walk_cb, void *arg)
{
	QUEUE *q;
	uv_handle_t *h;

	QUEUE_FOREACH(q, &loop->handles_queue) {
		h = QUEUE_DATA(q, uv_handle_t, handle_queue);
		if (h->flags & UV__HANDLE_INTERNAL) {
			continue;
		}
		walk_cb(h, arg);
	}
}

//-----------------------------------------------------------------------------

void uv_deinit(uv_loop_t *loop, uv_handle_t *handle)
{
	QUEUE *q;
	uv_handle_t *h;

	QUEUE_FOREACH(q, &loop->handles_queue) {
		h = QUEUE_DATA(q, uv_handle_t, handle_queue);
		if (h == handle) {
			uv__handle_deinit(handle);
			break;
		}
	}
}
