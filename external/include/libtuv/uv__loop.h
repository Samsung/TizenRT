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

#ifndef __uv__loop_header__
#define __uv__loop_header__

#ifndef __uv_header__
#error Please include with uv.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// uv_loop_t

struct uv_loop_s {
	/* User data - use this for whatever. */
	void *data;
	/* Loop reference counting. */
	uint32_t active_handles;
	void *handles_queue[2];
	void *active_reqs[2];
	/* Internal flag to signal loop stop. */
	uint32_t stop_flag;
	/* platform dependent fields */
	UV_LOOP_PRIVATE_FIELDS
};

typedef enum {
	UV_RUN_DEFAULT = 0,
	UV_RUN_ONCE,
	UV_RUN_NOWAIT
} uv_run_mode;

//-----------------------------------------------------------------------------

int uv_loop_init(uv_loop_t *loop);
int uv_loop_close(uv_loop_t *loop);
uv_loop_t *uv_default_loop(void);
int uv_loop_alive(const uv_loop_t *loop);

int uv_run(uv_loop_t *loop, uv_run_mode mode);

void uv_update_time(uv_loop_t *loop);
uint64_t uv_now(const uv_loop_t *loop);
void uv_stop(uv_loop_t* loop);
int uv__loop_alive(const uv_loop_t *loop);

int uv__platform_loop_init(uv_loop_t *loop);
void uv__platform_loop_delete(uv_loop_t *loop);
void uv__platform_invalidate_fd(uv_loop_t *loop, int fd);

#ifdef __cplusplus
}
#endif
#endif							// __uv__loop_header__
