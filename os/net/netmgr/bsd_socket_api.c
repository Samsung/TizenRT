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

/**
 * @file
 * Sockets BSD-Like API wrapper module
 *
 */

#include <tinyara/config.h>
#include <tinyara/cancelpt.h>

#ifdef CONFIG_NET

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include "netstack.h"

/*  Private */
int _create_netlink(int type, int protocol)
{
	// to do message filter
	(void)type;
	(void)protocol;

	int fd = open(LWNL_PATH, O_RDWR);
	if (fd < 0) {
		ndbg("open netlink dev fail\n");
		return -1;
	}
	return fd;
}


int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	struct netstack *stk = get_netstack();
	return stk->ops->bind(s, name, namelen);
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->accept(s, addr, addrlen);
	leave_cancellation_point();
	return res;
}

int shutdown(int s, int how)
{
	struct netstack *stk = get_netstack();
	return stk->ops->shutdown(s, how);
}

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->connect(s, name, namelen);
	leave_cancellation_point();
	return res;
}

int getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	struct netstack *stk = get_netstack();
	return stk->ops->getsockname(s, name, namelen);
}

int getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	struct netstack *stk = get_netstack();
	return stk->ops->getpeername(s, name, namelen);
}

int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	struct netstack *stk = get_netstack();
	return stk->ops->setsockopt(s, level, optname, optval, optlen);
}

int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	struct netstack *stk = get_netstack();
	return stk->ops->getsockopt(s, level, optname, optval, optlen);
}

int listen(int s, int backlog)
{
	struct netstack *stk = get_netstack();
	return stk->ops->listen(s, backlog);
}

ssize_t recv(int s, void *mem, size_t len, int flags)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->recv(s, mem, len, flags);
	leave_cancellation_point();
	return res;
}

ssize_t recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->recvfrom(s, mem, len, flags, from, fromlen);
	leave_cancellation_point();
	return res;
}

/****************************************************************************
 * Function: recvmsg
 *
 * Description:
 *   The recvmsg() call is identical to recvfrom() with a NULL from parameter.
 *
 * Parameters:
 *   sockfd   Socket descriptor of socket
 *   buf      Buffer to receive data
 *   len      Length of buffer
 *   flags    Receive flags
 *
 * Returned Value:
 *  (see recvfrom)
 *
 * Assumptions:
 *
 ****************************************************************************/
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	// ToDo: It only supports limited features of sendmsg
	struct netstack *stk = get_netstack();
	return stk->ops->recvmsg(sockfd, msg, flags);
}

ssize_t send(int s, const void *data, size_t size, int flags)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->send(s, data, size, flags);
	leave_cancellation_point();
	return res;
}

ssize_t sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->sendto(s, data, size, flags, to, tolen);
	leave_cancellation_point();
	return res;
}

/****************************************************************************
 * Function: sendmsg
 *
 * Description:
 *   The sendmsg() call is identical to sendto() with a NULL from parameter.
  *
 * Parameters:
 *   sockfd   Socket descriptor of socket
 *   buf      Buffer to send data
 *   len      Length of buffer
 *   flags    Receive flags
 *
 * Returned Value:
 *  (see sendto)
 *
 * Assumptions:
 *
 ****************************************************************************/
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags)
{
	// ToDo: It only supports limited features of sendmsg
	struct netstack *stk = get_netstack();
	return stk->ops->sendmsg(sockfd, msg, flags);
}


int socket(int domain, int type, int protocol)
{
	if (domain == AF_LWNL) {
		int fd = _create_netlink(type, protocol);
		return fd;
	}
	struct netstack *stk = get_netstack();
	return stk->ops->socket(domain, type, protocol);
}
#endif // CONFIG_NET
