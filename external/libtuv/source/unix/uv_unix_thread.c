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

#include <stdlib.h>				// malloc(), free()
#include <stdio.h>

#include <uv.h>

//-----------------------------------------------------------------------------
// thread

struct thread_ctx {
	void (*entry)(void *arg);
	void *arg;
};

static void *uv__thread_start(void *arg)
{
	struct thread_ctx *ctx_p;
	struct thread_ctx ctx;

	ctx_p = (struct thread_ctx *)arg;
	ctx = *ctx_p;
	free(ctx_p);

	ctx.entry(ctx.arg);

	return 0;
}

int uv_thread_create(uv_thread_t *tid, void (*entry)(void *arg), void *arg)
{
	struct thread_ctx *ctx;
	int err;

	ctx = (struct thread_ctx *)malloc(sizeof(*ctx));
	if (ctx == NULL) {
		return UV_ENOMEM;
	}

	ctx->entry = entry;
	ctx->arg = arg;

	err = pthread_create(tid, NULL, uv__thread_start, ctx);

	if (err) {
		free(ctx);
	}

	return err ? -1 : 0;
}

uv_thread_t uv_thread_self(void)
{
	return pthread_self();
}

int uv_thread_join(uv_thread_t *tid)
{
	return -pthread_join(*tid, NULL);
}

int uv_thread_equal(const uv_thread_t *t1, const uv_thread_t *t2)
{
	return pthread_equal(*t1, *t2);
}

//-----------------------------------------------------------------------------
// once

void uv_once(uv_once_t *guard, void (*callback)(void))
{
	if (pthread_once(guard, callback)) {
		TDLOG("uv_once abort");
		ABORT();
	}
}

//-----------------------------------------------------------------------------
// mutex

int uv_mutex_init(uv_mutex_t *mutex)
{
#if defined(NDEBUG) || !defined(PTHREAD_MUTEX_ERRORCHECK)
	return -pthread_mutex_init(mutex, NULL);
#else
	pthread_mutexattr_t attr;
	int err;

	if (pthread_mutexattr_init(&attr)) {
		TDLOG("uv_mutex_init init abort");
		ABORT();
	}

	if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK)) {
		TDLOG("uv_mutex_init settype abort");
		ABORT();
	}

	err = pthread_mutex_init(mutex, &attr);

	if (pthread_mutexattr_destroy(&attr)) {
		TDLOG("uv_mutex_init destroy abort");
		ABORT();
	}

	return -err;
#endif
}

void uv_mutex_destroy(uv_mutex_t *mutex)
{
	if (pthread_mutex_destroy(mutex)) {
		TDLOG("uv_mutex_destroy abort");
		ABORT();
	}
}

void uv_mutex_lock(uv_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex)) {
		TDLOG("uv_mutex_lock abort");
		ABORT();
	}
}

int uv_mutex_trylock(uv_mutex_t *mutex)
{
	int err;

	/* FIXME(bnoordhuis) EAGAIN means recursive lock limit reached. Arguably
	 * a bug, should probably abort rather than return -EAGAIN.
	 */
	err = pthread_mutex_trylock(mutex);
	if (err && err != EBUSY && err != EAGAIN) {
		TDLOG("uv_mutex_trylock abort");
		ABORT();
	}

	return -err;
}

void uv_mutex_unlock(uv_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex)) {
		ABORT();
	}
}

//-----------------------------------------------------------------------------
// semaphore

int uv_sem_init(uv_sem_t *sem, unsigned int value)
{
	if (sem_init(sem, 0, value)) {
		return -errno;
	}
	return 0;
}

void uv_sem_destroy(uv_sem_t *sem)
{
	if (sem_destroy(sem)) {
		TDLOG("uv_sem_destroy abort");
		ABORT();
	}
}

void uv_sem_post(uv_sem_t *sem)
{
	if (sem_post(sem)) {
		TDLOG("uv_sem_post abort");
		ABORT();
	}
}

void uv_sem_wait(uv_sem_t *sem)
{
	int r;

	do {
		r = sem_wait(sem);
	} while (r == -1 && errno == EINTR);

	if (r) {
		TDLOG("uv_sem_wait abort");
		ABORT();
	}
}

int uv_sem_trywait(uv_sem_t *sem)
{
	int r;

	do {
		r = sem_trywait(sem);
	} while (r == -1 && errno == EINTR);

	if (r) {
		if (errno == EAGAIN) {
			return -EAGAIN;
		}
		ABORT();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// condition

int uv_cond_init(uv_cond_t *cond)
{
	pthread_condattr_t attr;
	int err;

	err = pthread_condattr_init(&attr);
	if (err) {
		return -err;
	}

#if !(defined(__ANDROID__) && defined(HAVE_PTHREAD_COND_TIMEDWAIT_MONOTONIC)) && !defined(__TINYARA__)
	err = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
	if (err) {
		goto error2;
	}
#endif

	err = pthread_cond_init(cond, &attr);
	if (err) {
		goto error2;
	}

	err = pthread_condattr_destroy(&attr);
	if (err) {
		goto error;
	}

	return 0;

error:
	pthread_cond_destroy(cond);
error2:
	pthread_condattr_destroy(&attr);
	return -err;
}

void uv_cond_destroy(uv_cond_t *cond)
{
	if (pthread_cond_destroy(cond)) {
		TDLOG("uv_cond_destroy abort");
		ABORT();
	}
}

void uv_cond_signal(uv_cond_t *cond)
{
	if (pthread_cond_signal(cond)) {
		TDLOG("uv_cond_signal abort");
		ABORT();
	}
}

void uv_cond_broadcast(uv_cond_t *cond)
{
	if (pthread_cond_broadcast(cond)) {
		TDLOG("uv_cond_broadcast abort");
		ABORT();
	}
}

void uv_cond_wait(uv_cond_t *cond, uv_mutex_t *mutex)
{
	if (pthread_cond_wait(cond, mutex)) {
		TDLOG("uv_cond_wait abort");
		ABORT();
	}
}

#undef NANOSEC
#define NANOSEC ((uint64_t) 1e9)

int uv_cond_timedwait(uv_cond_t *cond, uv_mutex_t *mutex, uint64_t timeout)
{
	int r;
	struct timespec ts;

	timeout += uv__time_precise();
	ts.tv_sec = timeout / NANOSEC;
	ts.tv_nsec = timeout % NANOSEC;
	r = pthread_cond_timedwait(cond, mutex, &ts);

	if (r == 0) {
		return 0;
	}

	if (r == ETIMEDOUT) {
		return -ETIMEDOUT;
	}

	TDLOG("uv_cond_timedwait abort");
	ABORT();
	return -EINVAL;				/* Satisfy the compiler. */
}
