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

#ifndef __uv__tcp_header__
#define __uv__tcp_header__

#include <assert.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

int uv_ip4_addr(const char *ip, int port, struct sockaddr_in *addr);
int uv_ip4_name(const struct sockaddr_in *src, char *dst, size_t size);
int uv_inet_ntop(int af, const void *src, char *dst, size_t size);

/*
 * uv_tcp_t is a subclass of uv_stream_t.
 *
 * Represents a TCP stream or TCP server.
 */
struct uv_tcp_s {
	UV_HANDLE_FIELDS UV_STREAM_FIELDS UV_TCP_PRIVATE_FIELDS
};

int uv_tcp_init(uv_loop_t *, uv_tcp_t *handle);
int uv_tcp_open(uv_tcp_t *handle, uv_os_sock_t sock);
int uv_tcp_keepalive(uv_tcp_t *handle, int enable, unsigned int delay);

enum uv_tcp_flags {
	/* Used with uv_tcp_bind, when an IPv6 address is used. */
	UV_TCP_IPV6ONLY = 0x0001
};

int uv_tcp_bind(uv_tcp_t *handle, const struct sockaddr *addr, unsigned int flags);
int uv_tcp_connect(uv_connect_t *req, uv_tcp_t *handle, const struct sockaddr *addr, uv_connect_cb cb);

int uv_tcp_listen(uv_tcp_t *tcp, int backlog, uv_connection_cb cb);

int uv__tcp_bind(uv_tcp_t *tcp, const struct sockaddr *addr, unsigned int addrlen, unsigned int flags);
int uv__tcp_connect(uv_connect_t *req, uv_tcp_t *handle, const struct sockaddr *addr, unsigned int addrlen, uv_connect_cb cb);

int uv__tcp_nodelay(int fd, int on);
int uv__tcp_keepalive(int fd, int on, unsigned int delay);
void uv__tcp_close(uv_tcp_t *handle);

/* uv_connect_t is a subclass of uv_req_t. */
struct uv_connect_s {
	UV_REQ_FIELDS uv_connect_cb cb;
	uv_stream_t *handle;
	UV_CONNECT_PRIVATE_FIELDS
};

/* uv_write_t is a subclass of uv_req_t. */
struct uv_write_s {
	UV_REQ_FIELDS uv_write_cb cb;
	uv_stream_t *send_handle;
	uv_stream_t *handle;
	UV_WRITE_PRIVATE_FIELDS
};

struct uv_shutdown_s {
	UV_REQ_FIELDS uv_stream_t *handle;
	uv_shutdown_cb cb;
	UV_SHUTDOWN_PRIVATE_FIELDS
};

int uv_shutdown(uv_shutdown_t *req, uv_stream_t *handle, uv_shutdown_cb cb);

#ifdef __cplusplus
}
#endif
#endif							// __uv__tcp_header__
