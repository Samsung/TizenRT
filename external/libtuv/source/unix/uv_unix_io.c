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
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>

#include <uv.h>

static unsigned int next_power_of_two(unsigned int val)
{
	val -= 1;
	val |= val >> 1;
	val |= val >> 2;
	val |= val >> 4;
	val |= val >> 8;
	val |= val >> 16;
	val += 1;
	return val;
}

static void maybe_resize(uv_loop_t *loop, unsigned int len)
{
	uv__io_t **watchers;
	void *fake_watcher_list;
	void *fake_watcher_count;
	unsigned int nwatchers;
	unsigned int i;

	if (len <= loop->nwatchers) {
		return;
	}

	/* Preserve fake watcher list and count at the end of the watchers */
	if (loop->watchers != NULL) {
		fake_watcher_list = loop->watchers[loop->nwatchers];
		fake_watcher_count = loop->watchers[loop->nwatchers + 1];
	} else {
		fake_watcher_list = NULL;
		fake_watcher_count = NULL;
	}

	nwatchers = next_power_of_two(len + 2) - 2;
	watchers = (uv__io_t **) realloc(loop->watchers, (nwatchers + 2) * sizeof(loop->watchers[0]));

	if (watchers == NULL) {
		TDLOG("maybe_resize watchers NULL abort");
		ABORT();
	}
	for (i = loop->nwatchers; i < nwatchers; i++) {
		watchers[i] = NULL;
	}
	watchers[nwatchers] = (uv__io_t *) fake_watcher_list;
	watchers[nwatchers + 1] = (uv__io_t *) fake_watcher_count;

	loop->watchers = watchers;
	loop->nwatchers = nwatchers;
}

//-----------------------------------------------------------------------------

void uv__io_init(uv__io_t *w, uv__io_cb cb, int fd)
{
	assert(cb != NULL);
	assert(fd >= -1);
	QUEUE_INIT(&w->pending_queue);
	QUEUE_INIT(&w->watcher_queue);
	w->cb = cb;
	w->fd = fd;
	w->events = 0;
	w->pevents = 0;

#if defined(UV_HAVE_KQUEUE)
	w->rcount = 0;
	w->wcount = 0;
#endif							/* defined(UV_HAVE_KQUEUE) */
}

void uv__io_start(uv_loop_t *loop, uv__io_t *w, unsigned int events)
{
	assert(0 == (events & ~(UV__POLLIN | UV__POLLOUT | UV__POLLRDHUP)));
	assert(0 != events);
	assert(w->fd >= 0);
	assert(w->fd < INT_MAX);

	w->pevents |= events;
	maybe_resize(loop, w->fd + 1);

#if !defined(__sun)
	/* The event ports backend needs to rearm all file descriptors on each and
	 * every tick of the event loop but the other backends allow us to
	 * short-circuit here if the event mask is unchanged.
	 */
	if (w->events == w->pevents) {
		if (w->events == 0 && !QUEUE_EMPTY(&w->watcher_queue)) {
			QUEUE_REMOVE(&w->watcher_queue);
			QUEUE_INIT(&w->watcher_queue);
		}
		return;
	}
#endif

	if (QUEUE_EMPTY(&w->watcher_queue)) {
		QUEUE_INSERT_TAIL(&loop->watcher_queue, &w->watcher_queue);
	}

	if (loop->watchers[w->fd] == NULL) {
		loop->watchers[w->fd] = w;
		loop->nfds++;
	}
}

void uv__io_stop(uv_loop_t *loop, uv__io_t *w, unsigned int events)
{
	assert(0 == (events & ~(UV__POLLIN | UV__POLLOUT | UV__POLLRDHUP)));
	assert(0 != events);

	if (w->fd == -1) {
		return;
	}

	assert(w->fd >= 0);

	/* Happens when uv__io_stop() is called on a handle that was never started. */
	if ((unsigned)w->fd >= loop->nwatchers) {
		return;
	}

	w->pevents &= ~events;

	if (w->pevents == 0) {
		QUEUE_REMOVE(&w->watcher_queue);
		QUEUE_INIT(&w->watcher_queue);

		if (loop->watchers[w->fd] != NULL) {
			assert(loop->watchers[w->fd] == w);
			assert(loop->nfds > 0);
			loop->watchers[w->fd] = NULL;
			loop->nfds--;
			w->events = 0;
		}
	} else if (QUEUE_EMPTY(&w->watcher_queue)) {
		QUEUE_INSERT_TAIL(&loop->watcher_queue, &w->watcher_queue);
	}
}

void uv__io_close(uv_loop_t *loop, uv__io_t *w)
{
	uv__io_stop(loop, w, UV__POLLIN | UV__POLLOUT);
	QUEUE_REMOVE(&w->pending_queue);

	/* Remove stale events for this file descriptor */
	uv__platform_invalidate_fd(loop, w->fd);
}

void uv__io_feed(uv_loop_t *loop, uv__io_t *w)
{
	if (QUEUE_EMPTY(&w->pending_queue)) {
		QUEUE_INSERT_TAIL(&loop->pending_queue, &w->pending_queue);
	}
}

int uv__io_active(const uv__io_t *w, unsigned int events)
{
	assert(0 == (events & ~(UV__POLLIN | UV__POLLOUT)));
	assert(0 != events);

	return 0 != (w->pevents & events);
}
