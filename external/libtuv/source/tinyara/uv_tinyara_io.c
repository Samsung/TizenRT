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
#include <sys/select.h>

#include <uv.h>

static void uv__add_pollfd(uv_loop_t *loop, struct pollfd *pe)
{
	int i;
	bool exist = false;
	for (i = 0; i < loop->npollfds; ++i) {
		struct pollfd *cur = &loop->pollfds[i];
		if (cur->fd == pe->fd) {
			cur->events = pe->events;
			exist = true;
			break;
		}
	}
	if (!exist) {
		struct pollfd *cur = &loop->pollfds[loop->npollfds++];
		cur->fd = pe->fd;
		cur->events = pe->events;
		cur->revents = 0;
		cur->sem = 0;
		cur->priv = 0;
	}
}

static void uv__rem_pollfd(uv_loop_t *loop, struct pollfd *pe)
{
	int i = 0;
	while (i < loop->npollfds) {
		struct pollfd *cur = &loop->pollfds[i];
		if (cur->fd == pe->fd) {
			*cur = loop->pollfds[--loop->npollfds];
		} else {
			++i;
		}
	}
}

void uv__io_poll(uv_loop_t *loop, int timeout)
{
	struct pollfd pfd;
	struct pollfd *pe;
	QUEUE *q;
	uv__io_t *w;
	uint64_t base;
	uint64_t diff;
	int nevents;
	int count;
	int nfd;
	int i;

	if (loop->nfds == 0) {
		assert(QUEUE_EMPTY(&loop->watcher_queue));
		return;
	}

	while (!QUEUE_EMPTY(&loop->watcher_queue)) {
		q = QUEUE_HEAD(&loop->watcher_queue);
		QUEUE_REMOVE(q);
		QUEUE_INIT(q);

		w = QUEUE_DATA(q, uv__io_t, watcher_queue);
		assert(w->pevents != 0);
		assert(w->fd >= 0);
		assert(w->fd < (int)loop->nwatchers);

		pfd.fd = w->fd;
		pfd.events = w->pevents;
		uv__add_pollfd(loop, &pfd);

		w->events = w->pevents;
	}

	assert(timeout >= -1);
	base = loop->time;
	count = 5;

	for (;;) {
		nfd = poll(loop->pollfds, loop->npollfds, timeout);

		SAVE_ERRNO(uv__update_time(loop));

		if (nfd == 0) {
			assert(timeout != -1);
			return;
		}

		if (nfd == -1) {
			int err = get_errno();
			if (err == EAGAIN) {
				set_errno(0);
			} else if (err != EINTR) {
				// poll of which the watchers is null should be removed
				TDLOG("uv__io_poll abort for errno(%d)", err);
				goto handle_poll;
			}
			if (timeout == -1) {
				continue;
			}
			if (timeout == 0) {
				return;
			}
			goto update_timeout;
		}

handle_poll:
		nevents = 0;

		for (i = 0; i < loop->npollfds; ++i) {
			pe = &loop->pollfds[i];
			w = loop->watchers[pe->fd];

			if (w == NULL) {
				uv__rem_pollfd(loop, pe);
				--i;
				continue;
			}

			if (pe->fd >= 0 && (pe->revents & (POLLIN | POLLOUT | POLLHUP))) {
				w->cb(loop, w, pe->revents);
				++nevents;
			}
		}

		if (nevents != 0) {
			if (--count != 0) {
				timeout = 0;
				continue;
			}
			return;
		}
		if (timeout == 0) {
			return;
		}
		if (timeout == -1) {
			continue;
		}
update_timeout:
		assert(timeout > 0);

		diff = loop->time - base;
		if (diff >= (uint64_t) timeout) {
			return;
		}
		timeout -= diff;
	}
}
