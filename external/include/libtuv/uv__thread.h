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

#ifndef __uv__thread_header__
#define __uv__thread_header__

#ifndef __uv_header__
#error Please include with uv.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// uv_thread

int uv_thread_create(uv_thread_t *tid, uv_thread_cb entry, void *arg);
uv_thread_t uv_thread_self(void);
int uv_thread_join(uv_thread_t *tid);
int uv_thread_equal(const uv_thread_t *t1, const uv_thread_t *t2);

//-----------------------------------------------------------------------------
// uv_once

void uv_once(uv_once_t *guard, void (*callback)(void));

//-----------------------------------------------------------------------------
// uv_mutex

int uv_mutex_init(uv_mutex_t *handle);
void uv_mutex_destroy(uv_mutex_t *handle);
void uv_mutex_lock(uv_mutex_t *handle);
int uv_mutex_trylock(uv_mutex_t *handle);
void uv_mutex_unlock(uv_mutex_t *handle);

//-----------------------------------------------------------------------------
// uv_sem

int uv_sem_init(uv_sem_t *sem, unsigned int value);
void uv_sem_destroy(uv_sem_t *sem);
void uv_sem_post(uv_sem_t *sem);
void uv_sem_wait(uv_sem_t *sem);
int uv_sem_trywait(uv_sem_t *sem);

//-----------------------------------------------------------------------------
// uv_cond

int uv_cond_init(uv_cond_t *cond);
void uv_cond_destroy(uv_cond_t *cond);
void uv_cond_signal(uv_cond_t *cond);
void uv_cond_broadcast(uv_cond_t *cond);

void uv_cond_wait(uv_cond_t *cond, uv_mutex_t *mutex);
int uv_cond_timedwait(uv_cond_t *cond, uv_mutex_t *mutex, uint64_t timeout);

//-----------------------------------------------------------------------------
// uv_rwlock

int uv_rwlock_init(uv_rwlock_t *rwlock);
void uv_rwlock_destroy(uv_rwlock_t *rwlock);
void uv_rwlock_rdlock(uv_rwlock_t *rwlock);
int uv_rwlock_tryrdlock(uv_rwlock_t *rwlock);
void uv_rwlock_rdunlock(uv_rwlock_t *rwlock);
void uv_rwlock_wrlock(uv_rwlock_t *rwlock);
int uv_rwlock_trywrlock(uv_rwlock_t *rwlock);
void uv_rwlock_wrunlock(uv_rwlock_t *rwlock);

#ifdef __cplusplus
}
#endif
#endif							//__uv__thread_header__
