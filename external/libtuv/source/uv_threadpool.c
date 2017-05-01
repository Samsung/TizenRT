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
#include <stdio.h>

#include <uv.h>

//-----------------------------------------------------------------------------
#define MAX_THREADPOOL_SIZE 4

static uv_once_t _once = UV_ONCE_INIT;
static uv_cond_t _cond;
static uv_mutex_t _mutex;
static unsigned int _nthreads;
static uv_thread_t *_threads;
static uv_thread_t _default_threads[2];
static QUEUE _exit_message;
static QUEUE _wq;
static volatile int _initialized = 0;

//-----------------------------------------------------------------------------

static void uv__cancelled(struct uv__work *w)
{
	TDLOG("uv__cancelled async");
	ABORT();
}

/* To avoid deadlock with uv_cancel() it's crucial that the worker
 * never holds the global _mutex and the loop-local mutex at the same time.
 */
static void worker(void *arg)
{
	struct uv__work *w;
	QUEUE *q;

	(void)arg;

	for (;;) {
		uv_mutex_lock(&_mutex);

		while (QUEUE_EMPTY(&_wq)) {
			uv_cond_wait(&_cond, &_mutex);
		}

		q = QUEUE_HEAD(&_wq);

		if (q == &_exit_message) {
			uv_cond_signal(&_cond);
		} else {
			QUEUE_REMOVE(q);
			QUEUE_INIT(q);		/* Signal uv_cancel() that the work req is
								   executing. */
		}

		uv_mutex_unlock(&_mutex);

		if (q == &_exit_message) {
			break;
		}

		w = QUEUE_DATA(q, struct uv__work, wq);
		w->work(w);

		uv_mutex_lock(&w->loop->wq_mutex);
		w->work = NULL;			/* Signal uv_cancel() that the work req is done
								   executing. */
		QUEUE_INSERT_TAIL(&w->loop->wq, &w->wq);
		uv_async_send(&w->loop->wq_async);
		uv_mutex_unlock(&w->loop->wq_mutex);
	}
}

static void post(QUEUE *q)
{
	uv_mutex_lock(&_mutex);
	QUEUE_INSERT_TAIL(&_wq, q);
	uv_mutex_unlock(&_mutex);

	uv_cond_signal(&_cond);
}

#if defined(__TINYARA__)
static void cleanup(void)
{
#else
__attribute__((destructor))
static void cleanup(void)
{
#endif
	unsigned int i;

	if (_initialized == 0) {
		return;
	}

	post(&_exit_message);

	for (i = 0; i < _nthreads; i++)
		if (uv_thread_join(_threads + i)) {
			ABORT();
		}

	if (_threads != _default_threads) {
		free(_threads);
	}

	uv_mutex_destroy(&_mutex);
	uv_cond_destroy(&_cond);

	_threads = NULL;
	_nthreads = 0;
	_initialized = 0;
	_once = UV_ONCE_INIT;
}

static void init_once(void)
{
	unsigned int i;
	const char *val;

	assert(_initialized == 0);

	_nthreads = ARRAY_SIZE(_default_threads);
	val = getenv("UV_THREADPOOL_SIZE");
	if (val != NULL) {
		_nthreads = atoi(val);
	}
	if (_nthreads == 0) {
		_nthreads = 1;
	}
	if (_nthreads > MAX_THREADPOOL_SIZE) {
		_nthreads = MAX_THREADPOOL_SIZE;
	}

	_threads = _default_threads;
	if (_nthreads > ARRAY_SIZE(_default_threads)) {
		_threads = (uv_thread_t *)malloc(_nthreads * sizeof(_threads[0]));
		if (_threads == NULL) {
			_nthreads = ARRAY_SIZE(_default_threads);
			_threads = _default_threads;
		}
	}

	if (uv_cond_init(&_cond)) {
		TDLOG("init_once cond abort");
		ABORT();
	}

	if (uv_mutex_init(&_mutex)) {
		TDLOG("init_once mutex abort");
		ABORT();
	}

	QUEUE_INIT(&_wq);

	for (i = 0; i < _nthreads; i++) {
		if (uv_thread_create(_threads + i, worker, NULL)) {
			TDLOG("init_once thread %d abort", i);
			ABORT();
		}
	}

	_initialized = 1;
}

//-----------------------------------------------------------------------------

void uv__work_submit(uv_loop_t *loop, struct uv__work *w, void (*work)(struct uv__work *w), void (*done)(struct uv__work *w, int status))
{

	uv_once(&_once, init_once);
	w->loop = loop;
	w->work = work;
	w->done = done;
	QUEUE_INIT(&w->wq);
	post(&w->wq);
}

static int uv__work_cancel(uv_loop_t *loop, uv_req_t *req, struct uv__work *w)
{
	int cancelled;

	uv_mutex_lock(&_mutex);
	uv_mutex_lock(&w->loop->wq_mutex);

	cancelled = !QUEUE_EMPTY(&w->wq) && w->work != NULL;
	if (cancelled) {
		QUEUE_REMOVE(&w->wq);
	}

	uv_mutex_unlock(&w->loop->wq_mutex);
	uv_mutex_unlock(&_mutex);

	if (!cancelled) {
		return UV_EBUSY;
	}

	w->work = uv__cancelled;
	uv_mutex_lock(&loop->wq_mutex);
	QUEUE_INSERT_TAIL(&loop->wq, &w->wq);
	uv_async_send(&loop->wq_async);
	uv_mutex_unlock(&loop->wq_mutex);

	return 0;
}

void uv__work_done(uv_async_t *handle)
{
	struct uv__work *w;
	uv_loop_t *loop;
	QUEUE *q;
	QUEUE wq;
	int err;

	loop = container_of(handle, uv_loop_t, wq_async);
	QUEUE_INIT(&wq);

	uv_mutex_lock(&loop->wq_mutex);
	if (!QUEUE_EMPTY(&loop->wq)) {
		q = QUEUE_HEAD(&loop->wq);
		QUEUE_SPLIT(&loop->wq, q, &wq);
	}
	uv_mutex_unlock(&loop->wq_mutex);

	while (!QUEUE_EMPTY(&wq)) {
		q = QUEUE_HEAD(&wq);
		QUEUE_REMOVE(q);

		w = container_of(q, struct uv__work, wq);
		err = (w->work == uv__cancelled) ? UV_ECANCELED : 0;
		w->done(w, err);
	}
}

//-----------------------------------------------------------------------------

static void uv__queue_work(struct uv__work *w)
{
	uv_work_t *req = container_of(w, uv_work_t, work_req);

	req->work_cb(req);
}

static void uv__queue_done(struct uv__work *w, int err)
{
	uv_work_t *req;

	req = container_of(w, uv_work_t, work_req);
	uv__req_unregister(req->loop, req);

	if (req->after_work_cb == NULL) {
		return;
	}

	req->after_work_cb(req, err);
}

int uv_queue_work(uv_loop_t *loop, uv_work_t *req, uv_work_cb work_cb, uv_after_work_cb after_work_cb)
{
	if (work_cb == NULL) {
		return UV_EINVAL;
	}

	uv__req_init(loop, req, UV_WORK);
	req->loop = loop;
	req->work_cb = work_cb;
	req->after_work_cb = after_work_cb;
	uv__work_submit(loop, &req->work_req, uv__queue_work, uv__queue_done);
	return 0;
}

//-----------------------------------------------------------------------------

int uv_cancel(uv_req_t *req)
{
	struct uv__work *wreq;
	uv_loop_t *loop;

	switch (req->type) {
	case UV_FS:
		loop = ((uv_fs_t *) req)->loop;
		wreq = &((uv_fs_t *) req)->work_req;
		break;
	case UV_GETADDRINFO:
		loop = ((uv_getaddrinfo_t *) req)->loop;
		wreq = &((uv_getaddrinfo_t *) req)->work_req;
		break;
	/*
	  case UV_GETNAMEINFO:
	    loop = ((uv_getnameinfo_t*) req)->loop;
	    wreq = &((uv_getnameinfo_t*) req)->work_req;
	    break;
	*/
	case UV_WORK:
		loop = ((uv_work_t *) req)->loop;
		wreq = &((uv_work_t *) req)->work_req;
		break;
	default:
		return UV_EINVAL;
	}

	return uv__work_cancel(loop, req, wreq);
}

//-----------------------------------------------------------------------------
#if defined(__TINYARA__)
void uv_cleanup(void)
{
	cleanup();
}
#endif
