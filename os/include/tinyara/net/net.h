/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 *
 *   Copyright (C) 2007, 2009-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_NET_NET_H
#define __INCLUDE_NET_NET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#ifdef CONFIG_NET

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Capabilities of a socket */

#define SOCKCAP_NONBLOCKING (1 << 0) /* Bit 0: Socket supports non-blocking \
									  *        operation. */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Data link layer type.  This type is used with netdev_register in order to
 * identify the type of the network driver.
 */

enum net_lltype_e {
	NET_LL_ETHERNET = 0,		/* Ethernet */
	NET_LL_IEEE80211,			/* IEEE 802.11 */
	NET_LL_SLIP,				/* Serial Line Internet Protocol (SLIP) */
	NET_LL_PPP					/* Point-to-Point Protocol (PPP) */
};

/* This defines a bitmap big enough for one bit for each socket option */

typedef uint16_t sockopt_t;

/* This defines the storage size of a timeout value.  This effects only
 * range of supported timeout values.  With an LSB in seciseconds, the
 * 16-bit maximum of 65535 corresponds to 1 hr 49 min 13.5 sec at decisecond
 * resolution.
 */

typedef uint16_t socktimeo_t;

/* This type defines the type of the socket capabilities set */

typedef uint8_t sockcaps_t;

struct file;	 /* Forward reference */
struct stat;	 /* Forward reference */
struct pollfd;   /* Forward reference */
struct sockaddr; /* Forward reference */

typedef enum sock_type {
	TR_SOCKET,
	TR_UDS,
	TR_LWNL,
	TR_UNKNOWN,
} sock_type;

/* This is the internal representation of a socket.  As in NuttX 12.13.0, a
 * socket is a file of the VFS: the socket inode of os/fs/socket keeps a
 * pointer to this structure in the private data of the file, and the VFS
 * allocates the file descriptor.
 *
 * The network stack is lwIP: s_sd is the socket descriptor of lwIP (the
 * index in the socket table of lwIP).  It is not a file descriptor.
 */

struct socket {
	uint8_t s_domain;			/* IP domain */
	uint8_t s_type;				/* Protocol type */
	uint16_t s_proto;			/* Socket Protocol */
	FAR void *s_conn;			/* Not used with lwIP */
	int s_sd;					/* Socket descriptor of the network stack */
};

struct netif;					/* Forward reference. Defined in lwip/netif.h */
typedef int (*netdev_callback_t)(FAR struct netif *dev, void *arg);

#ifdef CONFIG_ENC28J60
#ifndef CONFIG_SYSTEM_IOTJS
uint64_t eth_tx_count;
uint64_t eth_rx_count;
uint64_t eth_max_message_size;
uint64_t eth_avg_message_size;
uint64_t eth_total_message_size;
#endif
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: net_initialize
 *
 * Description:
 *   This is called from the OS initialization logic at power-up reset in
 *   order to configure the networking subsystem.  It is called prior to
 *   the platform-specific driver initialization (up_initialize()), so that
 *   the network drivers can register their devices.
 *
 *   It initializes the network stack and the network device manager.  It
 *   cannot depend upon OS resources such as interrupts or timers, which
 *   are not yet available.
 *
 ****************************************************************************/

void net_initialize(void);

/****************************************************************************
 * Name: net_start
 *
 * Description:
 *   This is called from the OS initialization logic AFTER the
 *   initialization of the hardware facilities such as timers and
 *   interrupts.  It completes the initialization started by
 *   net_initialize(): it starts the threads of the network stack.
 *
 ****************************************************************************/

void net_start(void);

/****************************************************************************
 * Name: sockfd_allocate
 *
 * Description:
 *   Allocate a socket descriptor
 *
 * Input Parameters:
 *   psock    A pointer to socket structure.
 *   oflags   Open mode flags.
 *
 * Returned Value:
 *   Allocate a struct files instance and associate it with an socket
 *   instance.  Returns the file descriptor.
 *
 ****************************************************************************/

int sockfd_allocate(FAR struct socket *psock, int oflags);

/****************************************************************************
 * Name: file_socket / sockfd_socket
 *
 * Description:
 *   Given a file or a file descriptor, return the underlying socket
 *   structure.  sockfd_socket() returns the file with a reference: the
 *   caller must release it with file_put().
 *
 * Returned Value:
 *   file_socket(): the socket, NULL if the file is not a socket.
 *   sockfd_socket(): OK; -EBADF if the descriptor is not open; -ENOTSOCK
 *   if it is not a socket.
 *
 ****************************************************************************/

FAR struct socket *file_socket(FAR struct file *filep);
int sockfd_socket(int sockfd, FAR struct file **filep, FAR struct socket **socketp);

/****************************************************************************
 * Name: net_lwnl_socket
 *
 * Description:
 *   socket() of the AF_LWNL domain.  A lightweight netlink socket is a file
 *   descriptor of the lwnl driver.
 *
 * Returned Value:
 *   The file descriptor; -1 (ERROR) with the errno set on failure.
 *
 ****************************************************************************/

int net_lwnl_socket(int domain, int type, int protocol);

/****************************************************************************
 * Name: psock_*
 *
 * Description:
 *   The socket operations of the file operations of the socket inode
 *   (os/fs/socket) and of the VFS.  They are implemented by the network
 *   manager (os/net/netmgr) with the network stack.
 *
 * Returned Value:
 *   A non-negative value on success; a negated errno value on failure.
 *
 ****************************************************************************/

int psock_socket(int domain, int type, int protocol, FAR struct socket *psock);
int psock_close(FAR struct socket *psock);
int psock_accept(FAR struct socket *psock, FAR struct sockaddr *addr, FAR socklen_t *addrlen, FAR struct socket *newsock, int flags);
ssize_t psock_send(FAR struct socket *psock, FAR const void *buf, size_t len, int flags);
ssize_t psock_recvfrom(FAR struct socket *psock, FAR void *buf, size_t len, int flags, FAR struct sockaddr *from, FAR socklen_t *fromlen);
int psock_ioctl(FAR struct socket *psock, int cmd, unsigned long arg);
int psock_poll(FAR struct socket *psock, FAR struct pollfd *fds, bool setup);
int psock_dup2(FAR struct socket *psock1, FAR struct socket *psock2);
int psock_fstat(FAR struct socket *psock, FAR struct stat *buf);
int psock_socketpair(int domain, int type, int protocol, FAR struct socket *psocks[2]);

#define psock_recv(psock, buf, len, flags) \
	psock_recvfrom(psock, buf, len, flags, NULL, NULL)

/****************************************************************************
 * Name: netdev_foreach
 *
 * Description:
 *   Enumerate each registered network device.
 *
 *   NOTE: netdev semaphore held throughout enumeration.
 *
 * Parameters:
 *   callback - Will be called for each registered device
 *   arg      - User argument passed to callback()
 *
 * Returned Value:
 *  0:Enumeration completed 1:Enumeration terminated early by callback
 *
 * Assumptions:
 *  Called from normal user mode
 *
 ****************************************************************************/

int netdev_foreach(netdev_callback_t callback, void *arg);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_NET */
#endif							/* __INCLUDE_NET_NET_H */
