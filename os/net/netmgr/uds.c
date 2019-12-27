/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include "uds.h"

int uds_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	return 0;
}

int uds_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	return 0;
}

int uds_shutdown(int s, int how)
{
	return 0;
}

int uds_connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	return 0;
}

int uds_getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	return 0;
}

int uds_getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	return 0;
}

int uds_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	return 0;
}

int uds_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	return 0;
}

int uds_listen(int s, int backlog)
{
	return 0;
}

ssize_t uds_recv(int s, void *mem, size_t len, int flags)
{
	return 0;
}

ssize_t uds_recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	return 0;
}

ssize_t uds_recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	return 0;
}

ssize_t uds_send(int s, const void *data, size_t size, int flags)
{
	return 0;
}

ssize_t uds_sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	return 0;
}

ssize_t uds_sendmsg(int sockfd, struct msghdr *msg, int flags)
{
	return 0;
}
