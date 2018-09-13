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

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <uv.h>

//-----------------------------------------------------------------------------
//

static void uv__async_event(uv_loop_t *loop, struct uv__async *w, unsigned int nevents)
{
	QUEUE queue;
	QUEUE *q;
	uv_async_t *h;

	QUEUE_MOVE(&loop->async_handles, &queue);
	while (!QUEUE_EMPTY(&queue)) {
		q = QUEUE_HEAD(&queue);
		h = QUEUE_DATA(q, uv_async_t, queue);
		QUEUE_REMOVE(q);
		QUEUE_INSERT_TAIL(&loop->async_handles, q);

		if (h->pending == 0) {
			continue;
		}
		h->pending = 0;

		if (h->async_cb == NULL) {
			continue;
		}
		h->async_cb(h);
	}
}

//-----------------------------------------------------------------------------
//

int uv_async_init(uv_loop_t *loop, uv_async_t *handle, uv_async_cb async_cb)
{
	int err;

	uv__async_platform_init(handle);
	uv__handle_init(loop, (uv_handle_t *) handle, UV_ASYNC);
	handle->async_cb = async_cb;
	handle->pending = 0;

	QUEUE_INSERT_TAIL(&loop->async_handles, &handle->queue);
	uv__handle_start(handle);

	err = uv__async_start(loop, &loop->async_watcher, uv__async_event);
	if (err) {
		return err;
	}

	return 0;
}

int uv_async_send(uv_async_t *handle)
{
	if (uv__async_make_pending(&handle->pending) == 0) {
		uv__async_send(&handle->loop->async_watcher);
	}

	return 0;
}

int uv_async_deinit(uv_loop_t *loop, uv_async_t *handle)
{
	uv__async_stop(loop, &loop->async_watcher);
	uv__handle_stop(handle);
	QUEUE_REMOVE(&handle->queue);
	QUEUE_INIT(&handle->queue);
	uv__handle_deinit(handle);

	return 0;
}
