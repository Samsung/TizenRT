/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include <libtuvapi.h>
#include <libtuv/uv.h>

struct _timeout {
	timeout_callback func;
	void *user_data;
	uv_timer_t *handle;
};

struct _watch {
	watch_callback func;
	void *user_data;
	uv_poll_t *handle;
	int fd;
};

struct _idle {
	idle_callback func;
	void *user_data;
	uv_idle_t *handle;
};

static uv_loop_t *_loop;

static void _check_loop(void)
{
	if (_loop == NULL) {
		_loop = uv_default_loop();
	}
}

static uv_loop_t *_get_loop(void)
{
	_check_loop();
	return _loop;
}

void libtuv_loop_run(void)
{
	uv_run(_get_loop(), UV_RUN_DEFAULT);
}

void libtuv_loop_quit(void)
{
	if (_loop) {
		uv_loop_close(_loop);
	}

	_loop = NULL;
}

static void _timeout_callback(uv_timer_t *handle)
{
	struct _timeout *timeout = handle->data;

	if (timeout->func) {
		timeout->func(timeout->user_data);
	}

	uv_timer_stop(timeout->handle);
	free(timeout->handle);

	memset(timeout, 0, sizeof(struct _timeout));
	free(timeout);
}

int libtuv_add_timeout_callback(unsigned int msec, timeout_callback func, void *user_data)
{
	struct _timeout *timeout = NULL;

	timeout = (struct _timeout *)malloc(sizeof(struct _timeout));
	timeout->func = func;
	timeout->user_data = user_data;
	timeout->handle = (uv_timer_t *)malloc(sizeof(uv_timer_t));
	timeout->handle->data = timeout;

	uv_update_time(_get_loop());
	uv_timer_init(_get_loop(), timeout->handle);
	uv_timer_start(timeout->handle, _timeout_callback, msec, 0);

	return 0;
}

static void _watch_callback(uv_poll_t *handle, int status, int events)
{
	struct _watch *watch = handle->data;
	enum watch_io io = 0;
	int ret;

	if (events & UV_READABLE) {
		io |= WATCH_IO_IN;
	}
	if (events & UV_WRITABLE) {
		io |= WATCH_IO_OUT;
	}
	if (events & UV_DISCONNECT) {
		io |= WATCH_IO_HUP;
	}

	ret = watch->func(watch->fd, io, watch->user_data);
	if (!ret) {
		uv_poll_stop(watch->handle);
		free(watch->handle);

		memset(watch, 0, sizeof(struct _watch));
		free(watch);
	}
}

int libtuv_add_fd_watch(int fd, enum watch_io io, watch_callback func, void *user_data, int *watch_id)
{
	struct _watch *watch;
	int events = 0;

	watch = (struct _watch *)malloc(sizeof(struct _watch));
	watch->func = func;
	watch->user_data = user_data;
	watch->handle = (uv_poll_t *)malloc(sizeof(uv_poll_t));
	watch->handle->data = watch;

	if (io & WATCH_IO_IN) {
		events |= UV_READABLE;
	}
	if (io & WATCH_IO_OUT) {
		events |= UV_WRITABLE;
	}
	if (io & WATCH_IO_HUP) {
		events |= UV_DISCONNECT;
	}

	uv_poll_init_socket(_get_loop(), watch->handle, fd);
	uv_poll_start(watch->handle, events, _watch_callback);

	watch->fd = fd;

	return 0;
}

static void _idle_callback(uv_idle_t *handle)
{
	struct _idle *idle = handle->data;
	int ret;

	ret = idle->func(idle->user_data);
	if (!ret) {
		uv_idle_stop(idle->handle);
		free(idle->handle);

		memset(idle, 0, sizeof(struct _idle));
		free(idle);
	}
}

int libtuv_add_idle_callback(idle_callback func, void *user_data)
{
	struct _idle *idle;

	idle = (struct _idle *)malloc(sizeof(struct _idle));
	idle->func = func;
	idle->user_data = user_data;
	idle->handle = (uv_idle_t *)malloc(sizeof(uv_idle_t));
	idle->handle->data = idle;

	uv_idle_init(_get_loop(), idle->handle);
	uv_idle_start(idle->handle, _idle_callback);

	return 0;
}
