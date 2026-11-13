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
#include <sys/socket.h>
#include <net/if.h>
#include <errno.h>
#include <tinyara/fs/fs.h>
#include <tinyara/net/net.h>
#include "netstack.h"
#include <tinyara/net/netlog.h>

#define TAG "[NETMGR]"
#ifdef CONFIG_NET_LWIP
extern struct netstack *get_netstack_lwip(void);
#endif
#ifdef CONFIG_LWNL80211
extern struct netstack *get_netstack_netlink(void);
#endif

struct netstack *get_netstack(sock_type type)
{
	if (type == TR_SOCKET) {
		return get_netstack_lwip();
	} else if (type == TR_LWNL) {
#ifdef CONFIG_LWNL80211
		return get_netstack_netlink();
#endif
	}
	NET_LOGKE(TAG, "not supported stack type\n");
	return NULL;
}

/****************************************************************************
 * Name: get_netstack_byfd
 *
 * Description:
 *   Get the stack that owns a file descriptor and replace the file
 *   descriptor with the descriptor of the stack.
 *
 *   A socket of lwIP is a file of the VFS with a socket inode.  Its socket
 *   structure keeps the lwIP socket descriptor.  The reference of the file
 *   is returned in filep and the caller releases it with
 *   put_netstack_byfd() after it called the stack: a socket that a thread
 *   is blocked on is not freed by close() of another thread.
 *
 *   A descriptor that is not a socket is a descriptor of the lwnl driver
 *   (socket(AF_LWNL) opens the driver).  The lwnl methods take the file
 *   descriptor and no reference is returned.
 *
 * Input Parameters:
 *   fd    - In: the file descriptor.  Out: the descriptor of the stack.
 *   filep - Out: the file of the socket, or NULL.
 *
 * Returned Value:
 *   The stack; NULL with the errno set to EBADF if fd is not open.
 *
 ****************************************************************************/

struct netstack *get_netstack_byfd(int *fd, FAR struct file **filep)
{
	FAR struct socket *psock;
	int ret;

	ret = sockfd_socket(*fd, filep, &psock);
	if (ret == OK) {
#ifdef CONFIG_NET_LWIP
		*fd = psock->s_sd;
		return get_netstack_lwip();
#else
		file_put(*filep);
		ret = -ENOTSOCK;
#endif
	}

	*filep = NULL;
	if (ret == -ENOTSOCK) {
#ifdef CONFIG_LWNL80211
		return get_netstack_netlink();
#endif
	}

	set_errno(ret == -ENOTSOCK ? ENOTSOCK : EBADF);
	return NULL;
}

/****************************************************************************
 * Name: put_netstack_byfd
 *
 * Description:
 *   Release the file that get_netstack_byfd() returned.  The socket is
 *   closed here if it was closed while the stack was called; the errno of
 *   the call is kept.
 *
 ****************************************************************************/

void put_netstack_byfd(FAR struct file *filep)
{
	if (filep != NULL) {
		int errcode = get_errno();

		file_put(filep);
		set_errno(errcode);
	}
}
