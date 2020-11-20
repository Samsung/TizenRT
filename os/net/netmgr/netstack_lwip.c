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
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/route.h>
#include <tinyara/kmalloc.h>
#include <tinyara/net/net.h>
#include "netstack.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "lwip/netif.h"
#ifdef CONFIG_NET_NETMON
#include "lwip/ip.h"
#include "lwip/ip6.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/raw.h"
#endif

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

static inline int _netsock_clone(FAR struct lwip_sock *sock1, FAR struct lwip_sock *sock2)
{
	int ret = OK;

	/* Parts of this operation need to be atomic */
	if (sock1 == NULL || sock2 == NULL) {
		return ERROR;
	}

	/* Duplicate the socket state */
	sock2->conn = sock1->conn;	/* Netconn callback */
	sock2->lastdata = sock1->lastdata;	/* data that was left from the previous read */
	sock2->lastoffset = sock1->lastoffset;	/* offset in the data that was left from the previous read */
	sock2->rcvevent = sock1->rcvevent;	/*  number of times data was received, set by event_callback(),
											tested by the receive and select / poll functions */
	sock2->sendevent = sock1->sendevent;	/* number of times data was ACKed (free send buffer), set by event_callback(),
											   tested by select / poll */
	sock2->errevent = sock1->errevent;	/* error happened for this socket, set by event_callback(), tested by select / poll */

	sock2->err = sock1->err;	/* last error that occurred on this socket */

	sock2->select_waiting = sock1->select_waiting;	/* counter of how many threads are waiting for this socket using select */
	sock2->conn->crefs++;

	return ret;
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
	struct lwip_sock *sock1;
	struct lwip_sock *sock2;
	int err;
	int ret;

	/* Lock the scheduler throughout the following */

	sched_lock();

	/* Get the socket structures underly both descriptors */

	sock1 = (struct lwip_sock *)get_socket(sockfd1, getpid());
	sock2 = (struct lwip_sock *)get_socket(sockfd2, getpid());

	/* Verify that the sockfd1 and sockfd2 both refer to valid socket
	 * descriptors and that sockfd1 has valid allocated conn
	 */

	if (!sock1 || !sock2) {
		err = EBADF;
		goto errout;
	}

	/* If sockfd2 also has valid allocated conn, then we will have to
	 * close it!
	 */

	if (sock2->conn) {
		netconn_delete(sock2->conn);
		sock2->conn = NULL;
	}

	/* Duplicate the socket state */

	ret = _netsock_clone(sock1, sock2);
	if (ret < 0) {
		err = -ret;
		goto errout;
	}

	sched_unlock();
	return 0;

errout:
	sched_unlock();
	errno = err;
	return -1;
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


static int lwip_ns_poll(int fd, struct pollfd *fds, bool setup)
{
	return lwip_poll(fd, fds, setup);
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

#ifdef CONFIG_NET_NETMON
static inline void _get_tcp_info(struct netmon_sock *sock_info, struct lwip_sock *lsock)
{
	ip_addr_t addr = IPADDR4_INIT(0);
	u16_t port = 0;

	if (netconn_getaddr(lsock->conn, &addr, &port, 1)) {
		ndbg("fail to get local IP info\n");
	}
	sock_info->local.ip.sin_family = AF_INET;
	sock_info->local.ip.sin_port = port;
	sock_info->local.ip.sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4((&addr)));

	ip_addr_t raddr = IPADDR4_INIT(0);
	port = 0;

	if (netconn_getaddr(lsock->conn, &addr, &port, 0)) {
		ndbg("fail to get remote IP info\n");
	}
	sock_info->local.ip.sin_family = AF_INET;
	sock_info->local.ip.sin_port = port;
	sock_info->local.ip.sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4((&raddr)));
}

static void _get_udp_info(struct netmon_sock *sock_info, struct lwip_sock *lsock)
{
#if LWIP_IPV4 && LWIP_IPV6
	if (lsock->conn->pcb.ip->local_ip.type != IPADDR_TYPE_V4) {
		ndbg("monitoring ipv6 is not supported yet");
			return;
	}
#endif

	sock_info->local.ip.sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4(&(lsock->conn->pcb.ip->local_ip)));
	sock_info->remote.ip.sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4(&(lsock->conn->pcb.ip->remote_ip)));
	sock_info->local.ip.sin_port = lsock->conn->pcb.udp->local_port;
	if (lsock->conn->pcb.udp->flags & UDP_FLAGS_CONNECTED) {
		sock_info->remote.ip.sin_port = lsock->conn->pcb.udp->remote_port;
	} else {
		sock_info->remote.ip.sin_port = 0;
	}
}

static void _get_raw_info(struct netmon_sock *sock_info, struct lwip_sock *lsock)
{
#if LWIP_IPV4 && LWIP_IPV6
	if (lsock->conn->pcb.ip->local_ip.type == IPADDR_TYPE_V4) {
		ndbg("monitoring ipv6 is not supported yet");
		return;
	}
#endif
	sock_info->local.ip.sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4(&(lsock->conn->pcb.ip->local_ip)));
	sock_info->remote.ip.sin_addr.s_addr = ip4_addr_get_u32(ip_2_ip4(&(lsock->conn->pcb.ip->remote_ip)));
	sock_info->local.ip.sin_port = (u16_t)lsock->conn->pcb.raw->protocol;
	sock_info->remote.ip.sin_port = 0;
}

/**
 * Copy the global socket contents to the input
 *
 * @param data is used to target
 */

static int lwip_ns_getstats(int fd, struct netmon_sock **sock_info)
{
	struct socketlist *list = sched_getsockets();
	DEBUGASSERT(list);
	struct lwip_sock *sock = (struct lwip_sock *)list->sl_sockets[fd].sock;
	if (!sock || !sock->conn) {
		return -1;
	}
	if (!sock->conn->pcb.ip) {
		return -2;
	}

	struct netmon_sock *sinfo = (struct netmon_sock *)kmm_malloc(sizeof(struct netmon_sock));
	if (!sinfo) {
		ndbg("alloc sinfo memory fail\n");
		return -3;
	}

	sinfo->type = sock->conn->type;
	sinfo->state = sock->conn->state;
	sinfo->pid = sock->conn->pid;

	if (sock->conn->type & NETCONN_TCP) {
		_get_tcp_info(sinfo, sock);
	} else if ((sock->conn->type & NETCONN_UDP) ||
			   (sock->conn->type & NETCONN_UDPLITE) ||
			   (sock->conn->type & NETCONN_UDPNOCHKSUM)) {
		_get_udp_info(sinfo, sock);
	} else {
		_get_raw_info(sinfo, sock);
	}
	if (pthread_getname_np(sock->conn->pid, sinfo->pid_name)) {
		strncpy(sinfo->pid_name, "NONE", 5);
	}
	*sock_info = sinfo;

	return 0;
}
#endif

static void lwip_ns_initlist(struct socketlist *list)
{
	for (int i = 0; i < CONFIG_NSOCKET_DESCRIPTORS; ++i) {
		list->sl_sockets[i].sock = NULL;
	}

	/* Initialize the list access mutex */
	sem_init(&list->sl_sem, 0, 1);
}

static void lwip_ns_releaselist(struct socketlist *list)
{
	int i;
	int ret;
	struct lwip_sock *psock;

	for (i = 0; i < CONFIG_NSOCKET_DESCRIPTORS; ++i) {
		if (list->sl_sockets[i].sock) {

			psock = (struct lwip_sock *)list->sl_sockets[i].sock;

			if (list->sl_sockets[i].s_crefs == 1) {
				ret = lwip_sock_close(psock);
				if (ret) {
					ndbg("socket could not close properly\n");
					list->sl_sockets[i].sock = NULL;
					return;
				}
			}

			else if (list->sl_sockets[i].s_crefs > 1) {
				list->sl_sockets[i].s_crefs--;
			}

			else {
				memset(&list->sl_sockets[i], 0, sizeof(struct socket));
			}
		}
		list->sl_sockets[i].sock = NULL;
	}
}

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
	lwip_ns_delroute,
#endif
#ifdef CONFIG_NET_NETMON
	lwip_ns_getstats,
#endif
	lwip_ns_initlist,
	lwip_ns_releaselist
};


struct netstack g_lwip_stack = {&g_lwip_stack_ops, NULL};


struct netstack *get_netstack_lwip(void)
{
	return &g_lwip_stack;
}
