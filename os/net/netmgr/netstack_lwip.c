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
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/route.h>
#include <tinyara/net/netlog.h>
#include <tinyara/sched.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mqueue.h>
#include <tinyara/net/net.h>
#include <tinyara/netmgr/netctl.h>
#include "netstack.h"
#include "lwip/sockets.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/raw.h"
#include "lwip/ip.h"
#include "lwip/ip6.h"

#define TCP_STR "TCP"
#define UDP_STR "UDP"
#define UNKNOWN_STR "UNKNOWN"

#define IPV6_ADDR_FORMAT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"
#define IPV4_ADDR_FORMAT "%d.%d.%d.%d"

#define TAG "[NETMGR]"
#define TTAG "[NTRACE]"

extern int netdev_lwipioctl(int sockfd, int cmd, void *arg);

static int _socket_argument_validation(int domain, int type, int protocol)
{
	if (domain != AF_INET && domain != AF_INET6 && domain != AF_UNSPEC) {
		NET_LOGKE(TAG, "invalid parameter domain\n");
		return -1;
	}
	switch (protocol) {
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE:
		if (type != SOCK_DGRAM && type != SOCK_RAW) {
			NET_LOGKE(TAG, "unknown type\n");
			return -1;
		}
		break;
	case IPPROTO_TCP:
		if (type != SOCK_STREAM) {
			NET_LOGKE(TAG, "unknown type\n");
			return -1;
		}
		break;
	case IPPROTO_ICMP:
	case IPPROTO_IGMP:
	case IPPROTO_ICMPV6:
		if (type != SOCK_RAW) {
			NET_LOGKE(TAG, "unknown type\n");
			return -1;
		}
		break;
	case IPPROTO_IP:
		if (type == SOCK_RAW) {
			NET_LOGKE(TAG, "unknown type\n");
			return -1;
		}
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

static char *_convert_netconn_state(enum netconn_state ns)
{
	switch (ns) {
	case NETCONN_WRITE:
		return "WRITE";
	case NETCONN_LISTEN:
		return "LISTEN";
	case NETCONN_CONNECT:
		return "CONNECT";
	case NETCONN_CLOSE:
		return "CLOSE";
	case NETCONN_NONE:
		return "NONE";
	}
	return "NONE";
}

static char *_convert_ip_type(enum netconn_type nt)
{
#ifdef CONFIG_NET_IPv6
	if ((NETCONN_TYPE_IPV6 & nt) == NETCONN_TYPE_IPV6) {
		return "IPV6";
	}
#endif
	return "IPV4";
}

static char *_convert_tcp_state(enum tcp_state ts)
{
	switch (ts) {
	case CLOSED:
		return "CLOSED";
	case LISTEN:
		return "LISTEN";
	case SYN_SENT:
		return "SYN_SENT";
	case SYN_RCVD:
		return "SYN_RCVD";
	case ESTABLISHED:
		return "ESTABLISHED";
	case FIN_WAIT_1:
		return "FIN_WAIT_1";
	case FIN_WAIT_2:
		return "FIN_WAIT_2";
	case CLOSE_WAIT:
		return "CLOSE_WAIT";
	case CLOSING:
		return "CLOSING";
	case LAST_ACK:
		return "LAST_ACK";
	case TIME_WAIT:
		return "TIME_WAIT";
	}
	return "UNKNOWN";
}

static char *_convert_udp_type(struct lwip_sock *sock)
{
	struct netconn *conn = sock->conn;
	if ((conn->type & NETCONN_UDPLITE) == NETCONN_UDPLITE) {
		return "UDPLITE";
	} else if ((conn->type & NETCONN_UDPNOCHKSUM) == NETCONN_UDPNOCHKSUM) {
		return "UDP_NOCHECKSUM";
	}
	return "UDP";
}
#ifdef CONFIG_NET_DEBUG_PORT
static void _get_port(int fd, int *port)
{
	*port = -1;
	struct lwip_sock *sock = get_socket_by_pid(fd, getpid());
	if (!sock) {
		NET_LOGKE(TTAG, "get socket fail\n");
		return;
	}

	struct netconn *conn = sock->conn;
	if (conn->type == NETCONN_TCP) {
		struct tcp_pcb *tpcb = conn->pcb.tcp;
		*port = tpcb->local_port;
	} else if (conn->type == NETCONN_UDP || conn->type == NETCONN_UDPLITE) {
		struct udp_pcb *upcb = conn->pcb.udp;
		*port = upcb->local_port;
	}
}
#endif

static inline void _get_tcp_info(int fd, struct lwip_sock *lsock, netmgr_logger_p logger)
{
	ip_addr_t addr = IPADDR4_INIT(0L);
	u16_t port = 0;
	ip_addr_t raddr = IPADDR4_INIT(0L);
	u16_t rport = 0;

	if (netconn_getaddr(lsock->conn, &addr, &port, 1)) {
		NET_LOGKV(TAG, "local IP isn't set\n");
	}
	if (netconn_getaddr(lsock->conn, &raddr, &rport, 0)) {
		NET_LOGKV(TAG, "remote IP isn't set\n");
	}

	netlogger_debug_msg(logger, "TCP\t%d\t%s:%d\t%s\t%s\t%s\t",
		   fd, lsock->pname, lsock->pid,
		   _convert_netconn_state(lsock->conn->state),
		   _convert_ip_type(lsock->conn->type),
		   _convert_tcp_state(lsock->conn->pcb.tcp->state));
#ifdef CONFIG_NET_IPv6
	if (lsock->conn->type & NETCONN_TYPE_IPV6) {
		netlogger_debug_msg(logger, IPV6_ADDR_FORMAT "\t%d\t" IPV6_ADDR_FORMAT "\t%d\n",
			   IP6_ADDR_BLOCK1(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK2(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK3(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK4(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK5(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK6(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK7(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK8(ip_2_ip6(&addr)),
			   port,
			   IP6_ADDR_BLOCK1(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK2(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK3(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK4(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK5(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK6(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK7(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK8(ip_2_ip6(&raddr)),
			   rport);
	} else
#endif
	{
		netlogger_debug_msg(logger, IPV4_ADDR_FORMAT "\t%d\t" IPV4_ADDR_FORMAT "\t%d\n",
			   ip4_addr1(ip_2_ip4(&addr)),
			   ip4_addr2(ip_2_ip4(&addr)),
			   ip4_addr3(ip_2_ip4(&addr)),
			   ip4_addr4(ip_2_ip4(&addr)),
			   port,
			   ip4_addr1(ip_2_ip4(&raddr)),
			   ip4_addr2(ip_2_ip4(&raddr)),
			   ip4_addr3(ip_2_ip4(&raddr)),
			   ip4_addr4(ip_2_ip4(&raddr)),
			   rport);
	}
}

static void _get_udp_info(int fd, struct lwip_sock *lsock, netmgr_logger_p logger)
{
	ip_addr_t addr = IPADDR4_INIT(0L);
	u16_t port = 0;
	ip_addr_t raddr = IPADDR4_INIT(0L);
	u16_t rport = 0;

	if (netconn_getaddr(lsock->conn, &addr, &port, 1)) {
		NET_LOGKV(TAG, "local addr isn't set\n");
	}
	if (netconn_getaddr(lsock->conn, &raddr, &rport, 0)) {
		NET_LOGKV(TAG, "remote addr isn't set\n");
	}
	netlogger_debug_msg(logger, "%s\t%d\t%s:%d\t%s\t%s\t%x\t",
		   _convert_udp_type(lsock),
		   fd, lsock->pname, lsock->pid,
		   _convert_netconn_state(lsock->conn->state),
		   _convert_ip_type(lsock->conn->type),
		   lsock->conn->pcb.udp->flags);
#ifdef CONFIG_NET_IPv6
	if (lsock->conn->type & NETCONN_TYPE_IPV6) {
		netlogger_debug_msg(logger, IPV6_ADDR_FORMAT "\t%d\t" IPV6_ADDR_FORMAT "\t%d\n",
			   IP6_ADDR_BLOCK1(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK2(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK3(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK4(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK5(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK6(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK7(ip_2_ip6(&addr)),
			   IP6_ADDR_BLOCK8(ip_2_ip6(&addr)),
			   port,
			   IP6_ADDR_BLOCK1(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK2(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK3(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK4(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK5(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK6(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK7(ip_2_ip6(&raddr)),
			   IP6_ADDR_BLOCK8(ip_2_ip6(&raddr)),
			   rport);
	} else
#endif
	{
		netlogger_debug_msg(logger, IPV4_ADDR_FORMAT "\t%d\t" IPV4_ADDR_FORMAT "\t%d\n",
			   ip4_addr1(ip_2_ip4(&addr)),
			   ip4_addr2(ip_2_ip4(&addr)),
			   ip4_addr3(ip_2_ip4(&addr)),
			   ip4_addr4(ip_2_ip4(&addr)),
			   port,
			   ip4_addr1(ip_2_ip4(&raddr)),
			   ip4_addr2(ip_2_ip4(&raddr)),
			   ip4_addr3(ip_2_ip4(&raddr)),
			   ip4_addr4(ip_2_ip4(&raddr)),
			   rport);
	}
}

static void _get_raw_info(int fd, struct lwip_sock *lsock, netmgr_logger_p logger)
{
		netlogger_debug_msg(logger, "RAW\t%d\t%s:%d\t%s\t%s\t%x\t",
		   fd, lsock->pname, lsock->pid,
		   _convert_netconn_state(lsock->conn->state),
		   _convert_ip_type(lsock->conn->type),
		   lsock->conn->pcb.raw->protocol);

	ip_addr_t *addr = &lsock->conn->pcb.raw->local_ip;
	ip_addr_t *raddr = &lsock->conn->pcb.raw->remote_ip;

#ifdef CONFIG_NET_IPv6
	if (lsock->conn->type & NETCONN_TYPE_IPV6) {
		netlogger_debug_msg(logger, IPV6_ADDR_FORMAT "\t" IPV6_ADDR_FORMAT "\n",
			   IP6_ADDR_BLOCK1(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK2(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK3(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK4(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK5(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK6(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK7(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK8(ip_2_ip6(addr)),
			   IP6_ADDR_BLOCK1(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK2(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK3(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK4(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK5(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK6(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK7(ip_2_ip6(raddr)),
			   IP6_ADDR_BLOCK8(ip_2_ip6(raddr)));
	} else
#endif
	{
		netlogger_debug_msg(logger, IPV4_ADDR_FORMAT "\t" IPV4_ADDR_FORMAT "\n",
			   ip4_addr1(ip_2_ip4(addr)),
			   ip4_addr2(ip_2_ip4(addr)),
			   ip4_addr3(ip_2_ip4(addr)),
			   ip4_addr4(ip_2_ip4(addr)),
			   ip4_addr1(ip_2_ip4(raddr)),
			   ip4_addr2(ip_2_ip4(raddr)),
			   ip4_addr3(ip_2_ip4(raddr)),
			   ip4_addr4(ip_2_ip4(raddr)));
	}
}

#ifdef CONFIG_NET_DEBUG_PORT
static void _get_proto(int fd, char **type)
{
	*type = UNKNOWN_STR;
	struct lwip_sock *sock = get_socket_by_pid(fd, getpid());
	if (!sock) {
		NET_LOGKE(TTAG, "get socket fail\n");
		return;
	}

	struct netconn *conn = sock->conn;
	if (conn->type == NETCONN_TCP) {
		*type = TCP_STR;
	} else if (conn->type == NETCONN_UDP || conn->type == NETCONN_UDPLITE) {
		*type = UDP_STR;
	}
}
#endif

static inline int _netsock_clone(FAR struct lwip_sock *sock1, FAR struct lwip_sock *sock2)
{
	int ret = OK;

	/* Parts of this operation need to be atomic */
	if (sock1 == NULL || sock2 == NULL) {
		NET_LOGKE(TAG, "invalid parameter\n");
		return ERROR;
	}

	/* Duplicate the socket state */
	sock2->conn = sock1->conn;			   /* Netconn callback */
	sock2->lastdata = sock1->lastdata; /* data that was left from the previous read */
	sock2->lastoffset = sock1->lastoffset; /* offset in the data that was left from the previous read */
	sock2->rcvevent = sock1->rcvevent; /*  number of times data was received, set by event_callback(), tested by the receive and select / poll functions */
	sock2->sendevent = sock1->sendevent;   /* number of times data was ACKed (free send buffer), set by event_callback(), tested by select / poll */
	sock2->errevent = sock1->errevent; /* error happened for this socket, set by event_callback(), tested by select / poll */

	sock2->err = sock1->err; /* last error that occurred on this socket */

	sock2->select_waiting = sock1->select_waiting; /* counter of how many threads are waiting for this socket using select */
	sock2->conn->crefs++;

	return ret;
}

static void _netmon_handler(struct tcb_s *tcb, void *arg)
{
	netmgr_logger_p logger= (netmgr_logger_p)arg;
	struct task_group_s *group = tcb->group;
	if (!group) {
		fflush(stdout);
		assert(0);
	}
	struct socketlist *slist = &group->tg_socketlist;
	for (int fd = 0; fd < CONFIG_NSOCKET_DESCRIPTORS; fd++) {
		struct lwip_sock *sock = (struct lwip_sock *)slist->sl_sockets[fd].sock;
		if (!sock || !sock->conn) {
			NET_LOGKV(TAG, "fd %d is not assigned socket %p %p\n", fd, sock, sock->conn);
			continue;
		};

		if (!sock->conn->pcb.ip) {
			NET_LOGKV(TAG, "no IP pcb\n");
			fflush(stdout);
			assert(0);
			continue;
		}

		if (NETCONNTYPE_GROUP(sock->conn->type) == NETCONN_TCP) {
			_get_tcp_info(fd, sock, logger);
		} else if (NETCONNTYPE_GROUP(sock->conn->type) == NETCONN_UDP) {
			_get_udp_info(fd, sock, logger);
		} else if (NETCONNTYPE_GROUP(sock->conn->type) == NETCONN_RAW) {
			_get_raw_info(fd, sock, logger);
		}
	}
}

/*
 * ops functions
 */
static int lwip_ns_close(int sockfd)
{
	int res = lwip_close(sockfd);
#ifdef CONFIG_NET_DEBUG_PORT
	NET_LOGKI(TTAG, "pid %d socket %d\n", getpid(), sockfd);
#endif
	return res;
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
	sock1 = (struct lwip_sock *)get_socket_by_pid(sockfd1, getpid());
	sock2 = (struct lwip_sock *)get_socket_by_pid(sockfd2, getpid());

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
	NET_LOGKV(TAG, "Not supported yet\n");
	return 0;
}

static int lwip_ns_checksd(int sd, int oflags)
{
	// ToDo
	NET_LOGKV(TAG, "Not supported yet\n");
	return 0;
}

static int lwip_ns_ioctl(int sockfd, int cmd, unsigned long arg)
{
	int res = netdev_lwipioctl(sockfd, cmd, (void *)arg);
	if (res < 0) {
		if (res == -ENOTTY) {
			NET_LOGKV(TAG, "command is not for lwip ioctl\n");
		} else {
			NET_LOGKE(TAG, "lwipioctl fail\n");
		}
	}
	return res;
}

static int lwip_ns_vfcntl(int sockfd, int cmd, va_list ap)
{
	if (cmd == F_SETFL) {
		int mode = va_arg(ap, int);
		return lwip_fcntl(sockfd, cmd, mode);
	} else if (cmd == F_GETFL) {
		return lwip_fcntl(sockfd, cmd, 0);
	}
	NET_LOGKE(TAG, "vfcntl fail\n");
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
		NET_LOGKE(TAG, "not supported socket type\n");
		return -1;
	}
	res = lwip_socket(domain, type, protocol);
#ifdef CONFIG_NET_DEBUG_PORT
	if (res != -1) {
		char *str = NULL;
		_get_proto(res, &str);
		NET_LOGKI(TTAG, "pid %d socket %d proto %s\n", getpid(), res, str);
	}
#endif
	return res;
}

static int lwip_ns_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	int res = lwip_bind(s, name, namelen);
#ifdef CONFIG_NET_DEBUG_PORT
	if (res == 0) {
		int pid = getpid();
		char *str = NULL;
		int request_port = ntohs(((struct sockaddr_in *)name)->sin_port);
		int assigned_port = 0;
		_get_port(s, &assigned_port);
		_get_proto(s, &str);
		NET_LOGKI(TTAG, "pid %d socket %d proto %s request %d assgiend %d\n",
				 pid, s, str, request_port, assigned_port);
	}
#endif
	return res;
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

	return sendto(sockfd, buf, len, flags, to, (socklen_t)*addrlen);
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
		NET_LOGKE(TAG, "invalid parameter\n");
		return -EINVAL;
	}

	// ToDo
	NET_LOGKV("not supported yet\n");
	return -ENOTTY;
}

static int lwip_ns_delroute(struct rtentry *entry)
{
	if (!entry || !entry->rt_target || !entry->rt_netmask) {
		NET_LOGKE(TAG, "invalid parameter\n");
		return -EINVAL;
	}

	// ToDo
	NET_LOGKV("not supported yet\n");
	return -ENOTTY;
}
#endif

/**
 * Copy the global socket contents to the input
 *
 * @param data is used to target
 */
static int lwip_ns_getstats(void *arg)
{
	if (!arg) {
		return -EINVAL;
	}
	struct lwip_netmon_msg *msg = (struct lwip_netmon_msg *)arg;
	netmgr_logger_p logger;
	char *buf;
	int res = netlogger_init(&logger);
	if (res != 0) {
		return -ENOTTY;
	}
	sched_foreach(_netmon_handler, (void *)logger);
	netlogger_serialize(logger, &buf);
	netlogger_deinit(logger);
	msg->info = buf;

	return 0;
}

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
					NET_LOGKE(TAG, "socket could not close properly\n");
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
	lwip_ns_getstats,
	lwip_ns_initlist,
	lwip_ns_releaselist};

struct netstack g_lwip_stack = {&g_lwip_stack_ops, NULL};

struct netstack *get_netstack_lwip(void)
{
	return &g_lwip_stack;
}
