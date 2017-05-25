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

#include <errno.h>
#include <assert.h>

#include <uv.h>

int uv_idle_init(uv_loop_t *loop, uv_idle_t *handle)
{
	uv__idle_platform_init(handle);
	uv__handle_init(loop, (uv_handle_t *) handle, UV_IDLE);

	return 0;
}

int uv_idle_start(uv_idle_t *handle, uv_idle_cb cb)
{
	if (uv__is_active(handle)) {
		TDDLOG("uv_idle_start, handle(%p) is already active", handle);
		return 0;
	}
	if (cb == NULL) {
		return -EINVAL;
	}
	QUEUE_INSERT_HEAD(&handle->loop->idle_handles, &handle->queue);
	handle->idle_cb = cb;
	uv__handle_start(handle);

	return 0;
}

int uv_idle_stop(uv_idle_t *handle)
{
	if (!uv__is_active(handle)) {
		TDDLOG("uv_idle_stop, handle(%p) is not active, cancel", handle);
		return 0;
	}
	QUEUE_REMOVE(&handle->queue);
	QUEUE_INIT(&handle->queue);
	uv__handle_stop(handle);

	return 0;
}

//-----------------------------------------------------------------------------

void uv__run_idle(uv_loop_t *loop)
{
	uv_idle_t *h;
	QUEUE *q;

	QUEUE_FOREACH(q, &loop->idle_handles) {
		h = QUEUE_DATA(q, uv_idle_t, queue);
		if (h) {
			h->idle_cb(h);
		}
	}
}

void uv__idle_close(uv_idle_t *handle)
{
	uv_idle_stop(handle);
}
