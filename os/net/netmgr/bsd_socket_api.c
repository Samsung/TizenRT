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

#ifdef CONFIG_NET
#include <tinyara/cancelpt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include "netstack.h"
#include "uds.h"

/**
 * Private
 */
int _create_netlink(int type, int protocol)
{
	// to do: filter type and protocol
	(void)type;
	(void)protocol;

	int fd = open(LWNL_PATH, O_RDWR);
	if (fd < 0) {
		ndbg("open netlink dev fail\n");
		return -1;
	}
	return fd;
}

sock_type _get_socktype(int fd)
{
	if (fd < CONFIG_NFILE_DESCRIPTORS) {
		return TR_LWNL;
	} else if (fd < CONFIG_NFILE_DESCRIPTORS + CONFIG_NUDS_DESCRIPTORS) {
		return TR_UDS;
	} else if (fd < CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS) {
		return TR_SOCKET;
	}
	return TR_UNKNOWN;
}

/**
 * Public
 */
int bind(int sockfd, const struct sockaddr *name, socklen_t namelen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_bind(sockfd, name, namelen);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->bind(sockfd, name, namelen);
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_accept(sockfd, addr, addrlen);
	}

	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->accept(sockfd, addr, addrlen);
	leave_cancellation_point();
	return res;
}

int shutdown(int sockfd, int how)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_shutdown(sockfd, how);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->shutdown(sockfd, how);
}

int connect(int sockfd, const struct sockaddr *name, socklen_t namelen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_connect(sockfd, name, namelen);
	}

	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->connect(sockfd, name, namelen);
	leave_cancellation_point();
	return res;
}

int getsockname(int sockfd, struct sockaddr *name, socklen_t *namelen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_getsockname(sockfd, name, namelen);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->getsockname(sockfd, name, namelen);
}

int getpeername(int sockfd, struct sockaddr *name, socklen_t *namelen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_getpeername(sockfd, name, namelen);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->getpeername(sockfd, name, namelen);
}

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_setsockopt(sockfd, level, optname, optval, optlen);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->setsockopt(sockfd, level, optname, optval, optlen);
}

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_getsockopt(sockfd, level, optname, optval, optlen);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->getsockopt(sockfd, level, optname, optval, optlen);
}

int listen(int sockfd, int backlog)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_listen(sockfd, backlog);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->listen(sockfd, backlog);
}

ssize_t recv(int sockfd, void *mem, size_t len, int flags)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_recv(sockfd, mem, len, flags);
	}

	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->recv(sockfd, mem, len, flags);
	leave_cancellation_point();
	return res;
}

ssize_t recvfrom(int sockfd, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_recvfrom(sockfd, mem, len, flags, from, fromlen);
	}

	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->recvfrom(sockfd, mem, len, flags, from, fromlen);
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
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_recvmsg(sockfd, msg, flags);
	}

	// ToDo: It only supports limited features of sendmsg
	struct netstack *stk = get_netstack();
	return stk->ops->recvmsg(sockfd, msg, flags);
}

ssize_t send(int sockfd, const void *data, size_t size, int flags)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return 0;
	}

	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->send(sockfd, data, size, flags);
	leave_cancellation_point();
	return res;
}

ssize_t sendto(int sockfd, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_sendto(sockfd, data, size, flags, to, tolen);
	}

	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack();
	int res = stk->ops->sendto(sockfd, data, size, flags, to, tolen);
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
	sock_type type = _get_socktype(sockfd);
	if (type == UDS) {
		return uds_sendmsg(sockfd, msg, flags);
	}

	struct netstack *stk = get_netstack();
	return stk->ops->sendmsg(sockfd, msg, flags);
}


int socket(int domain, int type, int protocol)
{
	if (domain == AF_LWNL) {
		int fd = _create_netlink(type, protocol);
		return fd;
	} else if (domain == AF_UNIX) {
		// create user domain socket and return it
		return 0;
	}
	struct netstack *stk = get_netstack();
	return stk->ops->socket(domain, type, protocol);
}
#endif // CONFIG_NET
