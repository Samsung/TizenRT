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
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sockio.h>
#include <errno.h>
#include <debug.h>
#include <net/if.h>
#include <net/route.h>
#include <tinyara/fs/fs.h>
#include <tinyara/net/net.h>
#ifdef CONFIG_LWNL80211
#include <tinyara/lwnl/lwnl.h>
#endif
#include "netstack.h"
#include <tinyara/net/netlog.h>

#define TAG "[NETMGR]"

/****************************************************************************
 * A socket is a file of the VFS (os/fs/socket).  The file operations of the
 * socket inode and the VFS call the psock_*() functions of this file, which
 * call the network stack with the socket descriptor of the stack that the
 * socket structure keeps (s_sd).
 *
 * The psock_*() functions return a negated errno value on failure.  The
 * methods of the stack follow the BSD socket API: they return -1 with the
 * errno set.
 ****************************************************************************/

extern int netdev_imsfioctl(FAR struct socket *sock, int cmd, FAR struct ip_msfilter *imsf);
extern int netdev_ifrioctl(FAR struct socket *sock, int cmd, FAR struct ifreq *req);
#ifdef CONFIG_NET_ROUTE
extern int netdev_rtioctl(FAR struct socket *sock, int cmd, FAR struct rtentry *rtentry);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* The negated errno value of the method of the stack that has failed */

static inline int _stack_errno(void)
{
	int errcode = get_errno();

	return errcode > 0 ? -errcode : -EIO;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: psock_socket
 *
 * Description:
 *   Create a socket of the network stack and initialize the socket
 *   structure with it.  SOCK_NONBLOCK of type is applied to the socket;
 *   SOCK_CLOEXEC is a flag of the file descriptor and is handled by the
 *   caller.
 *
 ****************************************************************************/

int psock_socket(int domain, int type, int protocol, FAR struct socket *psock)
{
	struct netstack *stk = get_netstack(TR_SOCKET);
	int nonblock = (type & SOCK_NONBLOCK) != 0;
	int sd;

	if (!psock || !stk || !stk->ops->socket) {
		return -EINVAL;
	}

	type &= ~(SOCK_CLOEXEC | SOCK_NONBLOCK);

	set_errno(EPROTONOSUPPORT);
	sd = stk->ops->socket(domain, type, protocol);
	if (sd < 0) {
		return _stack_errno();
	}

	if (nonblock) {
		int ret = stk->ops->ioctl(sd, FIONBIO, (unsigned long)((uintptr_t)&nonblock));
		if (ret < 0) {
			stk->ops->close(sd);
			return ret;
		}
	}

	psock->s_domain = domain;
	psock->s_type = type;
	psock->s_proto = protocol;
	psock->s_conn = NULL;
	psock->s_sd = sd;
	return OK;
}

/****************************************************************************
 * Name: psock_close
 *
 * Description:
 *   Close the socket of the network stack.  It is called when the last
 *   reference of the file of the socket is released.
 *
 ****************************************************************************/

int psock_close(FAR struct socket *psock)
{
	struct netstack *stk = get_netstack(TR_SOCKET);
	int ret;

	if (!psock || !stk) {
		return -EBADF;
	}

	ret = stk->ops->close(psock->s_sd);
	if (ret < 0) {
		return _stack_errno();
	}

	psock->s_sd = -1;
	return OK;
}

/****************************************************************************
 * Name: psock_accept
 *
 * Description:
 *   Accept a connection of the listening socket psock and initialize
 *   newsock with the socket of the connection.
 *
 ****************************************************************************/

int psock_accept(FAR struct socket *psock, FAR struct sockaddr *addr, FAR socklen_t *addrlen, FAR struct socket *newsock, int flags)
{
	struct netstack *stk = get_netstack(TR_SOCKET);
	int sd;

	if (!psock || !newsock || !stk) {
		return -EBADF;
	}

	sd = stk->ops->accept(psock->s_sd, addr, addrlen);
	if (sd < 0) {
		return _stack_errno();
	}

	if (flags & SOCK_NONBLOCK) {
		int nonblock = 1;
		int ret = stk->ops->ioctl(sd, FIONBIO, (unsigned long)((uintptr_t)&nonblock));
		if (ret < 0) {
			stk->ops->close(sd);
			return ret;
		}
	}

	newsock->s_domain = psock->s_domain;
	newsock->s_type = psock->s_type;
	newsock->s_proto = psock->s_proto;
	newsock->s_conn = NULL;
	newsock->s_sd = sd;
	return OK;
}

/****************************************************************************
 * Name: psock_send / psock_recvfrom
 *
 * Description:
 *   write() and read() of a socket.
 *
 ****************************************************************************/

ssize_t psock_send(FAR struct socket *psock, FAR const void *buf, size_t len, int flags)
{
	struct netstack *stk = get_netstack(TR_SOCKET);
	ssize_t ret;

	if (!psock || !stk) {
		return -EBADF;
	}

	ret = stk->ops->send(psock->s_sd, buf, len, flags);
	return ret < 0 ? _stack_errno() : ret;
}

ssize_t psock_recvfrom(FAR struct socket *psock, FAR void *buf, size_t len, int flags, FAR struct sockaddr *from, FAR socklen_t *fromlen)
{
	struct netstack *stk = get_netstack(TR_SOCKET);
	ssize_t ret;

	if (!psock || !stk) {
		return -EBADF;
	}

	ret = stk->ops->recvfrom(psock->s_sd, buf, len, flags, from, fromlen);
	return ret < 0 ? _stack_errno() : ret;
}

/****************************************************************************
 * Name: psock_ioctl
 *
 * Description:
 *   Perform network device specific operations.
 *
 *   FIONBIO and FIONREAD are commands of the socket of the stack.  The
 *   other commands do not depend on the socket: SIOCLWIP (name resolution,
 *   DHCP, ...) is handled by the stack, the interface requests by the
 *   network device manager.
 *
 *   fcntl(F_SETFL, O_NONBLOCK) of the VFS comes here as FIONBIO.
 *
 * Returned Value:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   A negated errno value on failure:
 *
 *   -ENOTTY
 *     'cmd' is not valid, or it does not apply to a socket.
 *   -EINVAL
 *     'arg' is not valid.
 *
 ****************************************************************************/

int psock_ioctl(FAR struct socket *psock, int cmd, unsigned long arg)
{
	struct netstack *stk = get_netstack(TR_SOCKET);
	int ret = -ENOTTY;

	if (!psock || !stk) {
		return -EBADF;
	}

	/* Check if this is a valid command. */
	if (!((_FIOCVALID(cmd)) || (_SIOCVALID(cmd)))) {
		return -ENOTTY;
	}

	/* Execute the command */
	NETSTACK_CALL_RET(stk, ioctl, (psock->s_sd, cmd, arg), ret);
	if (ret == -ENOTTY) {
		ret = netdev_ifrioctl(NULL, cmd, (FAR struct ifreq *)((uintptr_t)arg));
	}
#ifdef CONFIG_NET_IGMP
	if (ret == -ENOTTY) {
		ret = netdev_imsfioctl(NULL, cmd, (FAR struct ip_msfilter *)((uintptr_t)arg));
	}
#endif							/* CONFIG_NET_IGMP */
#ifdef CONFIG_NET_ROUTE
	if (ret == -ENOTTY) {
		ret = netdev_rtioctl(NULL, cmd, (FAR struct rtentry *)((uintptr_t)arg));
	}
#endif							/* CONFIG_NET_ROUTE */

	return ret;
}

/****************************************************************************
 * Name: psock_poll
 *
 * Description:
 *   Set up or tear down the poll of a socket.  The stack reports the
 *   events with poll_notify().
 *
 ****************************************************************************/

int psock_poll(FAR struct socket *psock, FAR struct pollfd *fds, bool setup)
{
	struct netstack *stk = get_netstack(TR_SOCKET);

	if (!psock || !stk || !stk->ops->poll) {
		return -EBADF;
	}

	return stk->ops->poll(psock->s_sd, fds, setup);
}

/****************************************************************************
 * Name: psock_dup2
 *
 * Description:
 *   Clone a socket structure.  It is called when a file structure is
 *   duplicated (file_dup2()).
 *
 *   dup(), dup2(), fcntl(F_DUPFD) and the inheritance of the descriptors by
 *   a new task do not come here: the new descriptor shares the file, and
 *   the socket of the stack is closed with the last reference of the file.
 *   The socket of lwIP cannot be shared by two socket structures.
 *
 ****************************************************************************/

int psock_dup2(FAR struct socket *psock1, FAR struct socket *psock2)
{
	return -ENOSYS;
}

/****************************************************************************
 * Name: psock_fstat
 *
 * Description:
 *   fstat() of a socket.
 *
 ****************************************************************************/

int psock_fstat(FAR struct socket *psock, FAR struct stat *buf)
{
	if (!psock || !buf) {
		return -EINVAL;
	}

	memset(buf, 0, sizeof(struct stat));
	buf->st_mode = S_IFSOCK;
	return OK;
}

/****************************************************************************
 * Name: psock_socketpair
 *
 * Description:
 *   socketpair() needs local sockets (AF_UNIX), which the network stack
 *   does not have.
 *
 ****************************************************************************/

int psock_socketpair(int domain, int type, int protocol, FAR struct socket *psocks[2])
{
	return -EAFNOSUPPORT;
}

/****************************************************************************
 * Name: net_lwnl_socket
 *
 * Description:
 *   socket() of the AF_LWNL domain: open the lwnl driver.
 *
 ****************************************************************************/

#ifdef CONFIG_LWNL80211
int net_lwnl_socket(int domain, int type, int protocol)
{
	struct netstack *stk = get_netstack(TR_LWNL);

	if (!stk || !stk->ops->socket) {
		set_errno(EAFNOSUPPORT);
		return ERROR;
	}

	return stk->ops->socket(domain, type, protocol);
}
#endif
