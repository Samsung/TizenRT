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
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <uv.h>

int uv__async_make_pending(int *pending)
{
	/* Do a cheap read first. */
	if (ACCESS_ONCE(int, *pending) != 0) {
		return 1;
	}

	/* Micro-optimization: use atomic memory operations to detect if we've been
	 * preempted by another thread and don't have to make an expensive syscall.
	 * This speeds up the heavily contended case by about 1-2% and has little
	 * if any impact on the non-contended case.
	 *
	 * Use XCHG instead of the CMPXCHG that __sync_val_compare_and_swap() emits
	 * on x86, it's about 4x faster. It probably makes zero difference in the
	 * grand scheme of things but I'm OCD enough not to let this one pass.
	 */
#if defined(__i386__) || defined(__x86_64__)
	{
		unsigned int val = 1;
		__asm__ __volatile__("xchgl %0, %1":"+r"(val)
							 :"m"(*pending));
		return val != 0;
	}
#elif defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ > 0)
	return __sync_val_compare_and_swap(pending, 0, 1) != 0;
#else
	ACCESS_ONCE(int, *pending) = 1;
	return 0;
#endif
}

static int uv__async_eventfd(void)
{
#if defined(__linux__)
	static int no_eventfd2;
	static int no_eventfd;
	int fd;

	if (no_eventfd2) {
		goto skip_eventfd2;
	}

	fd = uv__eventfd2(0, UV__EFD_CLOEXEC | UV__EFD_NONBLOCK);
	if (fd != -1) {
		return fd;
	}

	if (errno != ENOSYS) {
		return -errno;
	}

	no_eventfd2 = 1;

skip_eventfd2:

	if (no_eventfd) {
		goto skip_eventfd;
	}

	fd = uv__eventfd(0);
	if (fd != -1) {
		uv__cloexec(fd, 1);
		uv__nonblock(fd, 1);
		return fd;
	}

	if (errno != ENOSYS) {
		return -errno;
	}

	no_eventfd = 1;

skip_eventfd:

#endif

	return -ENOSYS;
}

static void uv__async_io(uv_loop_t *loop, uv__io_t *w, unsigned int events)
{
	struct uv__async *wa;
	char buf[1024];
	unsigned n;
	ssize_t r;
	int err;

	n = 0;
	for (;;) {
		r = read(w->fd, buf, sizeof(buf));

		if (r > 0) {
			n += r;
		}

		if (r == sizeof(buf)) {
			continue;
		}

		if (r != -1) {
			break;
		}

		err = get_errno();
		if (err == EAGAIN || err == EWOULDBLOCK) {
			break;
		}

		if (err == EINTR) {
			continue;
		}

		TDLOG("uv__async_io abort for errno(%d)", err);
		ABORT();
	}

	wa = container_of(w, struct uv__async, io_watcher);

#if defined(__linux__)
	if (wa->wfd == -1) {
		uint64_t val;
		assert(n == sizeof(val));
		memcpy(&val, buf, sizeof(val));	/* Avoid alignment issues. */
		wa->cb(loop, wa, val);
		return;
	}
#endif

	wa->cb(loop, wa, n);
}

//-----------------------------------------------------------------------------
//

void uv__async_init(struct uv__async *wa)
{
	QUEUE_INIT(&(wa->io_watcher.pending_queue));
	QUEUE_INIT(&(wa->io_watcher.watcher_queue));
	wa->io_watcher.fd = -1;
	wa->wfd = -1;
}

void uv__async_close(uv_async_t *handle)
{
	QUEUE_REMOVE(&handle->queue);
	uv__handle_stop(handle);
}

void uv__async_stop(uv_loop_t *loop, struct uv__async *wa)
{
	if (wa->io_watcher.fd == -1) {
		return;
	}

	if (wa->wfd != -1) {
		if (wa->wfd != wa->io_watcher.fd) {
			uv__close(wa->wfd);
		}
		wa->wfd = -1;
	}

	uv__io_stop(loop, &wa->io_watcher, UV__POLLIN);
	uv__close(wa->io_watcher.fd);
	wa->io_watcher.fd = -1;
}

int uv__async_start(uv_loop_t *loop, struct uv__async *wa, uv__async_cb cb)
{
	int pipefd[2];
	int err;

	if (wa->io_watcher.fd != -1) {
		return 0;
	}

	err = uv__async_eventfd();
	if (err >= 0) {
		pipefd[0] = err;
		pipefd[1] = -1;
	} else if (err == -ENOSYS) {
		err = uv__make_pipe(pipefd, UV__F_NONBLOCK);
#if defined(__linux__)
		/* Save a file descriptor by opening one of the pipe descriptors as
		 * read/write through the procfs.  That file descriptor can then
		 * function as both ends of the pipe.
		 */
		if (err == 0) {
			char buf[32];
			int fd;

			snprintf(buf, sizeof(buf), "/proc/self/fd/%d", pipefd[0]);
			fd = uv__open_cloexec(buf, O_RDWR);
			if (fd >= 0) {
				uv__close(pipefd[0]);
				uv__close(pipefd[1]);
				pipefd[0] = fd;
				pipefd[1] = fd;
			}
		}
#endif
	}

	if (err < 0) {
		return err;
	}

	uv__io_init(&wa->io_watcher, uv__async_io, pipefd[0]);
	uv__io_start(loop, &wa->io_watcher, UV__POLLIN);
	wa->wfd = pipefd[1];
	wa->cb = cb;

	return 0;
}

void uv__async_send(struct uv__async *wa)
{
	const void *buf;
	ssize_t len;
	int fd;
	int r;

	buf = "";
	len = 1;
	fd = wa->wfd;

#if defined(__linux__)
	if (fd == -1) {
		static const uint64_t val = 1;
		buf = &val;
		len = sizeof(val);
		fd = wa->io_watcher.fd;	/* eventfd */
	}
#endif

	do {
		r = write(fd, buf, len);
	} while (r == -1 && errno == EINTR);

	if (r == len) {
		return;
	}

	if (r == -1) {
		int err = get_errno();
		if (err == EAGAIN || err == EWOULDBLOCK) {
			return;
		}
	}

	TDLOG("uv__async_send abort");
	ABORT();
}
