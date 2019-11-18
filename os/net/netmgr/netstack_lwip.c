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
#include <poll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/route.h>
#include <tinyara/net/net.h>
#include <fcntl.h>
#include "netstack.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "lwip/netif.h"

static int _socket_argument_validation(int domain, int type, int protocol)
{
	if (domain != AF_INET && domain != AF_INET6 && domain != AF_UNSPEC) {
		return -1;
	}
	switch (protocol) {
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE:
		if (type != SOCK_DGRAM && type != SOCK_RAW) {
			return -1;
		}
		break;
	case IPPROTO_TCP:
		if (type != SOCK_STREAM) {
			return -1;
		}
		break;
	case IPPROTO_ICMP:
	case IPPROTO_IGMP:
	case IPPROTO_ICMPV6:
		if (type != SOCK_RAW) {
			return -1;
		}
		break;
	case IPPROTO_IP:
		if (type == SOCK_RAW) {
			return -1;
		}
		break;
	default:
		return -1;
		break;
	}
	return 0;
}


/*
 * ops functions
 */
static int lwip_ns_close(int sockfd)
{
	return lwip_close(sockfd);
}


static int lwip_ns_dup(int sockfd)
{
	// ToDo
	return 0;
}


static int lwip_ns_dup2(int sockfd1, int sockfd2)
{
	// ToDo
	return 0;
}


static int lwip_ns_clone(FAR struct socket *psock1, FAR struct socket *psock2)
{
	// ToDo
	return 0;
}


static int lwip_ns_checksd(int sd, int oflags)
{
	// ToDo
	return 0;
}

extern int netdev_lwipioctl(int sockfd, int cmd, void *arg);

static int lwip_ns_ioctl(int sockfd, int cmd, unsigned long arg)
{
	int res = netdev_lwipioctl(sockfd, cmd, (void *)arg);
	if (res < 0) {
		return -ENOTTY;
	}
	return 0;
}


static int lwip_ns_vfcntl(int sockfd, int cmd, va_list ap)
{
	if (cmd == F_SETFL) {
		int mode = va_arg(ap, int);
		return lwip_fcntl(sockfd, cmd, mode);
	} else if (cmd == F_GETFL) {
		return lwip_fcntl(sockfd, cmd, 0);
	}
	return -1;
}


/****************************************************************************
 * Function: lwip_poll
 *
 * Description:
 *	 The standard poll() operation redirects operations on socket descriptors
 *	 to this function.
 *
 * Input Parameters:
 *	 sock - An instance of the lwip socket structure.
 *	 fds   - The structure describing the events to be monitored, OR NULL if
 *			 this is a request to stop monitoring events.
 *	 setup - true: Setup up the poll; false: Teardown the poll
 *
 * Returned Value:
 *	0: Success; Negated errno on failure
 *
 ****************************************************************************/

static int lwip_ns_poll(int fd, struct pollfd *fds, bool setup)
{
	// ToDo
	return 0;
}


static int lwip_ns_socket(int domain, int type, int protocol)
{
	int res = _socket_argument_validation(domain, type, protocol);
	if (res < 0) {
		ndbg("not supported socket type\n");
		return -1;
	}
	return lwip_socket(domain, type, protocol);
}


static int lwip_ns_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_bind(s, name, namelen);
}

static int lwip_ns_connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_connect(s, name, namelen);
}

static int lwip_ns_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	return lwip_accept(s, addr, addrlen);
}


static int lwip_ns_listen(int s, int backlog)
{
	return lwip_listen(s, backlog);
}


static int lwip_ns_shutdown(int s, int how)
{
	return lwip_shutdown(s, how);
}


static ssize_t lwip_ns_recv(int s, void *mem, size_t len, int flags)
{
	return lwip_recv(s, mem, len, flags);
}


static ssize_t lwip_ns_recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	return lwip_recvfrom(s, mem, len, flags, from, fromlen);
}


static ssize_t lwip_ns_send(int s, const void *data, size_t size, int flags)
{
	return lwip_send(s, data, size, flags);
}


static ssize_t lwip_ns_sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	return lwip_sendto(s, data, size, flags, to, tolen);
}


static int lwip_ns_getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getsockname(s, name, namelen);
}


static int lwip_ns_getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getpeername(s, name, namelen);
}


static int lwip_ns_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	return lwip_setsockopt(s, level, optname, optval, optlen);
}


static int lwip_ns_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	return lwip_getsockopt(s, level, optname, optval, optlen);
}


static ssize_t lwip_ns_recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	uint8_t *buf = (uint8_t *)(msg->msg_iov->iov_base);
	size_t len = msg->msg_iov->iov_len;
	struct sockaddr *from = (struct sockaddr *)msg->msg_name;
	socklen_t *addrlen = &(msg->msg_namelen);
	msg->msg_controllen = 0;

	return recvfrom(sockfd, buf, len, flags, from, addrlen);
}


static ssize_t lwip_ns_sendmsg(int sockfd, struct msghdr *msg, int flags)
{
	uint8_t *buf = (uint8_t *)(msg->msg_iov->iov_base);
	size_t len = msg->msg_iov->iov_len;
	struct sockaddr *to = (struct sockaddr *)msg->msg_name;
	int *addrlen = (int *)&(msg->msg_namelen);

	return sendto(sockfd, buf, len, flags, to, (socklen_t) *addrlen);
}

static int lwip_ns_init(void *data)
{
	lwip_init();
	return 0;
}


static int lwip_ns_deinit(void *data)
{
	return 0;
}


static int lwip_ns_start(void *data)
{
	tcpip_init(NULL, NULL);
	return 0;
}


static int lwip_ns_stop(void *data)
{
	return 0;
}


#ifdef CONFIG_NET_ROUTE
static int lwip_ns_addroute(struct rtentry *entry)
{
	if (!entry || !entry->rt_target || !entry->rt_netmask) {
		return -EINVAL;
	}

	// ToDo
	return -ENOTTY;
}


static int lwip_ns_delroute(struct rtentry *entry)
{
	if (!entry || !entry->rt_target || !entry->rt_netmask) {
		return -EINVAL;
	}

	// ToDo
	return -ENOTTY;
}
#endif


struct netstack_ops g_lwip_stack_ops = {
	lwip_ns_init,
	lwip_ns_deinit,
	lwip_ns_start,
	lwip_ns_stop,
	lwip_ns_close,

	lwip_ns_dup,
	lwip_ns_dup2,
	lwip_ns_clone,
	lwip_ns_checksd,
	lwip_ns_ioctl,
	lwip_ns_vfcntl,
	lwip_ns_poll,

	lwip_ns_socket,
	lwip_ns_bind,
	lwip_ns_connect,
	lwip_ns_accept,
	lwip_ns_listen,
	lwip_ns_shutdown,

	lwip_ns_recv,
	lwip_ns_recvfrom,
	lwip_ns_recvmsg,
	lwip_ns_send,
	lwip_ns_sendto,
	lwip_ns_sendmsg,

	lwip_ns_getsockname,
	lwip_ns_getpeername,
	lwip_ns_setsockopt,
	lwip_ns_getsockopt,
#ifdef CONFIG_NET_ROUTE
	lwip_ns_addroute,
	lwip_ns_delroute
#endif
};


struct netstack g_lwip_stack = {&g_lwip_stack_ops, NULL};


struct netstack *get_netstack_lwip(void)
{
	return &g_lwip_stack;
}
