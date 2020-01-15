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
#include "netstack.h"

/****************************************************************************
 * Name: sockfd_socket
 *
 * Description:
 *   Given a socket descriptor, return the underlying socket structure.
 *
 * Input Parameters:
 *   sockfd - The socket descriptor index to use.
 *
 * Returned Value:
 *   On success, a reference to the socket structure associated with the
 *   the socket descriptor is returned.  NULL is returned on any failure.
 *
 ****************************************************************************/

FAR struct socket *sockfd_socket(int sockfd)
{
	FAR struct socketlist *list;
	int ndx = sockfd - __SOCKFD_OFFSET;

	if (ndx >= 0 && ndx < CONFIG_NSOCKET_DESCRIPTORS) {
		list = sched_getsockets();
		if (list) {
			return &list->sl_sockets[ndx];
		}
	}

	return NULL;
}

/*
 *
 */

int uds_checksd(int sd, int oflags)
{
	FAR struct socket *psock = sockfd_socket(sd);

	/* Verify that the sockfd corresponds to valid, allocated socket */

	if (!psock || psock->s_crefs <= 0) {
		nvdbg("No valid socket for sd: %d\n", sd);
		return -EBADF;
	}

	/* NOTE:  We permit the socket FD to be "wrapped" in a stream as
	 * soon as the socket descriptor is created by socket().  Therefore
	 * (1) we don't care if the socket is connected yet, and (2) there
	 * are no access restrictions that can be enforced yet.
	 */

	return OK;
}

static int uds_socket(int domain, int type, int protocol)
{
	return 0;
}

static int uds_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	return 0;
}

static int uds_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	return 0;
}

static int uds_shutdown(int s, int how)
{
	return 0;
}

static int uds_connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	return 0;
}

static int uds_getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	return 0;
}

static int uds_getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	return 0;
}

static int uds_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	return 0;
}

static int uds_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	return 0;
}

static int uds_listen(int s, int backlog)
{
	return 0;
}

static ssize_t uds_recv(int s, void *mem, size_t len, int flags)
{
	return 0;
}

static ssize_t uds_recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	return 0;
}

static ssize_t uds_recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	return 0;
}

static ssize_t uds_send(int s, const void *data, size_t size, int flags)
{
	return 0;
}

static ssize_t uds_sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	return 0;
}

static ssize_t uds_sendmsg(int sockfd, struct msghdr *msg, int flags)
{
	return 0;
}


struct netstack_ops g_uds_stack_ops = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	NULL,
	NULL,
	NULL,
	uds_checksd,
	NULL,
	NULL,
	NULL,

	uds_socket,
	uds_bind,
	uds_connect,
	uds_accept,
	uds_listen,
	uds_shutdown,

	uds_recv,
	uds_recvfrom,
	uds_recvmsg,
	uds_send,
	uds_sendto,
	uds_sendmsg,

	uds_getsockname,
	uds_getpeername,
	uds_setsockopt,
	uds_getsockopt,
#ifdef CONFIG_NET_ROUTE
	NULL,
	NULL,
#endif
#ifdef CONFIG_NET_NETMON
	NULL,
#endif
};

struct netstack g_uds_stack = {&g_uds_stack_ops, NULL};

struct netstack *get_netstack_uds(void)
{
	return &g_uds_stack;
}
