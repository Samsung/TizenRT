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

#ifndef _NETMGR_NETSTACK_H__
#define _NETMGR_NETSTACK_H__

#include <stdarg.h>
#include <errno.h>
#include <sys/socket.h>
#include <net/if.h>
#include <tinyara/net/net.h>
#include <tinyara/fs/fs.h>

#define NETSTACK_CALL(stk, method, arg)			\
	do {										\
		if (stk && stk->ops->method) {			\
			return (stk->ops->method)arg;		\
		}										\
		return -1;								\
	} while (0)

#define NETSTACK_CALL_RET(stk, method, arg, res)	\
	do {											\
		if (stk && stk->ops->method) {				\
			res = (stk->ops->method)arg;			\
		}											\
	} while (0)

/* The stack methods take the descriptor of the stack, not the file descriptor.
 * get_netstack_byfd() replaces the file descriptor in fd with the descriptor
 * of the stack that owns it, so arg must refer to fd.
 *
 * The reference of the file is kept while the stack is called: close() of
 * another thread does not free a socket that a thread is blocked on.  The
 * socket is closed when the last call returns.
 */
#define NETSTACK_CALL_BYFD_RET(fd, method, arg, res)				\
	do {															\
		FAR struct file *filep_ = NULL;								\
		struct netstack *stk_ = get_netstack_byfd(&fd, &filep_);	\
		if (stk_ && stk_->ops->method) {							\
			res = (stk_->ops->method)arg;							\
		} else if (stk_) {											\
			set_errno(ENOTSOCK);									\
		}															\
		put_netstack_byfd(filep_);									\
	} while (0)

#define NETSTACK_CALL_BYFD(fd, method, arg)						\
	do {														\
		int res_ = -1;											\
		NETSTACK_CALL_BYFD_RET(fd, method, arg, res_);			\
		return res_;											\
	} while (0)

struct netstack_ops {
	// start, stop
	int (*init)(void *data);
	int (*deinit)(void *data);
	int (*start)(void *data);
	int (*stop)(void *data);

	// VFS
	int (*close)(int sockfd);
	int (*dup)(int sockfd);
	int (*dup2)(int sockfd1, int sockfd2);
	int (*clone)(struct socket *sock1, struct socket *sock2);
	int (*checksd)(int sd, int oflags);
	int (*ioctl)(int sockfd, int cmd, unsigned long arg); // stack specific option
	int (*fcntl)(int sockfd, int cmd, va_list ap);
	int (*poll)(int fd, struct pollfd *fds, bool setup);
	// BSD Socket API
	int (*socket)(int domain, int type, int protocol);
	int (*bind)(int s, const struct sockaddr *name, socklen_t namelen);
	int (*connect)(int s, const struct sockaddr *name, socklen_t namelen);
	int (*accept)(int s, struct sockaddr *addr, socklen_t *addrlen);
	int (*listen)(int s, int backlog);
	int (*shutdown)(int s, int how);
	ssize_t (*recv)(int s, void *mem, size_t len, int flags);
	ssize_t (*recvfrom)(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
	ssize_t (*recvmsg)(int s, struct msghdr *msg, int flags);
	ssize_t (*send)(int s, const void *data, size_t size, int flags);
	ssize_t (*sendto)(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
	ssize_t (*sendmsg)(int s, const struct msghdr *msg, int flags);
	int (*getsockname)(int s, struct sockaddr *name, socklen_t *namelen);
	int (*getpeername)(int s, struct sockaddr *name, socklen_t *namelen);
	int (*setsockopt)(int s, int level, int optname, const void *optval, socklen_t optlen);
	int (*getsockopt)(int s, int level, int optname, void *optval, socklen_t *optlen);
	// etc
#ifdef CONFIG_NET_ROUTE
	int (*addroute)(struct rtentry *entry);
	int (*delroute)(struct rtentry *entry);
#endif
	int (*getstats)(void *arg);
};

struct netstack {
	struct netstack_ops *ops;
	void *data;
};

struct netstack *get_netstack(sock_type type);
struct netstack *get_netstack_byfd(int *fd, FAR struct file **filep);
void put_netstack_byfd(FAR struct file *filep);

#endif //  _NETMGR_NETSTACK_H__
