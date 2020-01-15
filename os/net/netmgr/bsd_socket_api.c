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

/**
 * Public
 */
int bind(int sockfd, const struct sockaddr *name, socklen_t namelen)
{
	NETSTACK_CALL_BYFD(sockfd, bind, (sockfd, name, namelen));
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	(void)enter_cancellation_point();
	struct netstack *stk = get_netstack_byfd(sockfd);
	int res = -1;
	NETSTACK_CALL_RET(stk, accept, (sockfd, addr, addrlen), res);
	leave_cancellation_point();
	return res;
}

int shutdown(int sockfd, int how)
{
	NETSTACK_CALL_BYFD(sockfd, shutdown, (sockfd, how));
}

int connect(int sockfd, const struct sockaddr *name, socklen_t namelen)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	int res = -1;
	NETSTACK_CALL_BYFD_RET(sockfd, connect, (sockfd, name, namelen), res);
	leave_cancellation_point();
	return res;
}

int getsockname(int sockfd, struct sockaddr *name, socklen_t *namelen)
{
	NETSTACK_CALL_BYFD(sockfd, getsockname, (sockfd, name, namelen));
}

int getpeername(int sockfd, struct sockaddr *name, socklen_t *namelen)
{
	NETSTACK_CALL_BYFD(sockfd, getpeername, (sockfd, name, namelen));
}

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	NETSTACK_CALL_BYFD(sockfd, setsockopt, (sockfd, level, optname, optval, optlen));
}

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
	NETSTACK_CALL_BYFD(sockfd, getsockopt, (sockfd, level, optname, optval, optlen));
}

int listen(int sockfd, int backlog)
{
	NETSTACK_CALL_BYFD(sockfd, listen, (sockfd, backlog));
}

ssize_t recv(int sockfd, void *mem, size_t len, int flags)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	int res = -1;
	NETSTACK_CALL_BYFD_RET(sockfd, recv, (sockfd, mem, len, flags), res);
	leave_cancellation_point();
	return res;
}

ssize_t recvfrom(int sockfd, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	int res = -1;
	NETSTACK_CALL_BYFD_RET(sockfd, recvfrom, (sockfd, mem, len, flags, from, fromlen), res);
	leave_cancellation_point();
	return res;
}

/****************************************************************************
 * Function: recvmsg
 *
 * Description:
 *	 The recvmsg() call is identical to recvfrom() with a NULL from parameter.
 *
 * Parameters:
 *	 sockfd	  Socket descriptor of socket
 *	 buf	  Buffer to receive data
 *	 len	  Length of buffer
 *	 flags	  Receive flags
 *
 * Returned Value:
 *	(see recvfrom)
 *
 * Assumptions:
 *
 ****************************************************************************/
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	// ToDo: It only supports limited features of sendmsg
	NETSTACK_CALL_BYFD(sockfd, recvmsg, (sockfd, msg, flags));
}

ssize_t send(int sockfd, const void *data, size_t size, int flags)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	int res = -1;
	NETSTACK_CALL_BYFD_RET(sockfd, send, (sockfd, data, size, flags), res);
	leave_cancellation_point();
	return res;
}

ssize_t sendto(int sockfd, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	/* Treat as a cancellation point */
	(void)enter_cancellation_point();
	int res = -1;
	NETSTACK_CALL_BYFD_RET(sockfd, sendto, (sockfd, data, size, flags, to, tolen), res);
	leave_cancellation_point();
	return res;
}

/****************************************************************************
 * Function: sendmsg
 *
 * Description:
 *	 The sendmsg() call is identical to sendto() with a NULL from parameter.
 *
 * Parameters:
 *	 sockfd	  Socket descriptor of socket
 *	 buf	  Buffer to send data
 *	 len	  Length of buffer
 *	 flags	  Receive flags
 *
 * Returned Value:
 *	(see sendto)
 *
 * Assumptions:
 *
 ****************************************************************************/
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags)
{
	// ToDo: It only supports limited features of sendmsg
	NETSTACK_CALL_BYFD(sockfd, sendmsg, (sockfd, msg, flags));
}

int socket(int domain, int type, int protocol)
{
	struct netstack *stk = NULL;
	if (domain == AF_LWNL) {
		stk = get_netstack(TR_LWNL);
	} else if (domain == AF_UNIX) {
		// create user domain socket and return it
		stk = get_netstack(TR_UDS);
	} else {
		stk = get_netstack(TR_SOCKET);
	}

	NETSTACK_CALL(stk, socket, (domain, type, protocol));
}

#endif // CONFIG_NET
