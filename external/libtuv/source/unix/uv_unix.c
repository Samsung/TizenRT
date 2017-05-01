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

// uv_unix.cpp: unix, bsd, osx, linux, nuttx,

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <uv.h>

int uv__open_cloexec(const char *path, int flags)
{
	int err;
	int fd;

#if defined(__linux__) || (defined(__FreeBSD__) && __FreeBSD__ >= 9)
	static int no_cloexec;

	if (!no_cloexec) {
		fd = open(path, flags | UV__O_CLOEXEC);
		if (fd != -1) {
			return fd;
		}

		if (errno != EINVAL) {
			return -errno;
		}

		/* O_CLOEXEC not supported. */
		no_cloexec = 1;
	}
#endif

	fd = open(path, flags);
	if (fd == -1) {
		return -errno;
	}

	err = uv__cloexec(fd, 1);
	if (err) {
		uv__close(fd);
		return err;
	}

	return fd;
}

/* Open a socket in non-blocking close-on-exec mode, atomically if possible. */
int uv__socket(int domain, int type, int protocol)
{
	int sockfd;
	int err;

#if defined(SOCK_NONBLOCK) && defined(SOCK_CLOEXEC)
	sockfd = socket(domain, type | SOCK_NONBLOCK | SOCK_CLOEXEC, protocol);
	if (sockfd != -1) {
		return sockfd;
	}

	if (errno != EINVAL) {
		return -errno;
	}
#endif

	sockfd = socket(domain, type, protocol);
	if (sockfd == -1) {
		return -errno;
	}

	err = uv__nonblock(sockfd, 1);
	if (err == 0) {
		err = uv__cloexec(sockfd, 1);
	}

	if (err) {
		uv__close(sockfd);
		return err;
	}
#if defined(SO_NOSIGPIPE)
	{
		int on = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
	}
#endif

	return sockfd;
}

int uv__accept(int sockfd)
{
	int peerfd;
	int err;

	assert(sockfd >= 0);

	while (1) {
#if defined(__linux__) || (defined(__FreeBSD__) && __FreeBSD__ >= 10)
		static int no_accept4 = 0;

		if (no_accept4) {
			goto skip;
		}

		peerfd = uv__accept4(sockfd, NULL, NULL, UV__SOCK_NONBLOCK | UV__SOCK_CLOEXEC);
		if (peerfd != -1) {
			return peerfd;
		}

		if (errno == EINTR) {
			continue;
		}

		if (errno != ENOSYS) {
			return -errno;
		}

		no_accept4 = 1;
skip:
#endif

		peerfd = accept(sockfd, NULL, NULL);
		if (peerfd == -1) {
			err = get_errno();
			if (err == EINTR) {
				continue;
			}
			return -err;
		}

		err = uv__cloexec(peerfd, 1);
		if (err == 0) {
			err = uv__nonblock(peerfd, 1);
		}

		if (err) {
			uv__close(peerfd);
			return err;
		}

		return peerfd;
	}
}

int uv_ip4_addr(const char *ip, int port, struct sockaddr_in *addr)
{
	memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	return uv_inet_pton(AF_INET, ip, &(addr->sin_addr.s_addr));
}
