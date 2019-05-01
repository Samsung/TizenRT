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
 * include/sys/socket.h
 *
 *   Copyright (C) 2007, 2009, 2011 Gregory Nutt. All rights reserved.
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
/**
 * @defgroup NETWORK NETWORK
 * @defgroup SOCKET Socket
 * @brief Provides APIs for BSD Socket
 * @ingroup NETWORK
 *
 * @{
 */

/// @file sys/socket.h
/// @brief Socket APIs
#ifndef __INCLUDE_SYS_SOCKET_H
#define __INCLUDE_SYS_SOCKET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <sys/uio.h>

#ifdef CONFIG_NET_LWIP
#include <net/lwip/sockets.h>
#include <net/lwip/api.h>
#else
#include <sys/sock_internal.h>
#endif /* CONFIG_NET_LWIP */

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Structure
 ****************************************************************************/

struct msghdr {
	void *msg_name;                /* optional address */
	socklen_t msg_namelen;         /* size of address */
	struct iovec *msg_iov;         /* scatter/gather array */
	int msg_iovlen;                /* # elements in msg_iov */
	void *msg_control;             /* ancillary data, see below */
	socklen_t msg_controllen;      /* ancillary data buffer len */
	int msg_flags;                 /* flags on received message */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
* @brief creates an unbound socket in a communications domain.
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] domain the communications domain in which a socket is to be created.
* @param[in] type  the type of socket to be created
* @param[in] protocol the protocol to be used with the socket
* @return On success, a non negative integer is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int socket(int domain, int type, int protocol);

/**
* @brief  assigns an address to an unnamed socket.
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor of the socket to be bound.
* @param[in] addr  pointer to a sockaddr structure containing the address to be bound to the socket
* @param[in] addrlen the length of the sockaddr structure
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int bind(int sockfd, FAR const struct sockaddr *addr, socklen_t addrlen);

/**
* @brief   requests a connection to be made on a socket
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[in] addr  pointer to a sockaddr structure containing the peer address
* @param[in] addrlen the length of the sockaddr structure
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int connect(int sockfd, FAR const struct sockaddr *addr, socklen_t addrlen);

/**
* @brief    listen for socket connections and limit the queue of incoming connections
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[in] backlog  the number of outstanding connections in the socket's listen queue
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int listen(int sockfd, int backlog);

/**
* @brief   requests a connection to be made on a socket
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[inout] addr  null or pointer to a sockaddr structure where the address of the connecting socket will be returned
* @param[inout] addrlen on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
* @brief   send a message on a socket
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[in] buf  Pointer to the buffer containing the message to send.
* @param[in] len the length of the message in bytes.
* @param[in] flags the type of message transmission
* @return On success, returns the number of bytes sent, On failure, -1 is returned.
* @since TizenRT v1.0
*/
ssize_t send(int sockfd, FAR const void *buf, size_t len, int flags);
/**
* @brief   send a message on a socket
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[in] buf  Pointer to the buffer containing the message to send.
* @param[in] len the length of the message in bytes.
* @param[in] flags the type of message transmission
* @param[in] to pointer to a sockaddr structure containing the destination address
* @param[in] tolen  the length of the sockaddr structure
* @return On success, returns the number of bytes sent, On failure, -1 is returned.
* @since TizenRT v1.0
*/
ssize_t sendto(int sockfd, FAR const void *buf, size_t len, int flags, FAR const struct sockaddr *to, socklen_t tolen);

/**
* @brief   send a message on a socket
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[out] buf  Points to a buffer where the message should be stored
* @param[out] len the length in bytes of the buffer
* @param[in] flags the type of message reception.
* @return On success, returns  the length of the message in bytes, On failure, -1 is returned.
* @since TizenRT v1.0
*/
ssize_t recv(int sockfd, FAR void *buf, size_t len, int flags);

/**
* @brief   receive a message from a socket
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[out] buf  Pointer to the buffer where the message should be stored.
* @param[out] len the length of the message in bytes.
* @param[in] flags the type of message reception
* @param[inout] from  A null pointer, or pointer to  sockaddr structure in which the sending address is to be stored
* @param[inout] fromlen  null or the length of the sockaddr structure
* @return On success, returns the length of the message in bytes, On failure, -1 is returned.
* @since TizenRT v1.0
*/
ssize_t recvfrom(int sockfd, FAR void *buf, size_t len, int flags, FAR struct sockaddr *from, FAR socklen_t *fromlen);

/**
* @brief   shut down socket send and receive operations
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor of the socket
* @param[in] how the type of shutdown
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int shutdown(int sockfd, int how);

// @cond
/**
* @brief   close a socket
*
* @param[in] s the file descriptor of the socket
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
* @internal
*/
int closesocket(int s);
// @endcond

/**
* @brief   set the socket options
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor of the socket
* @param[in] level the protocol level
* @param[in] option the option to be set for the socket
* @param[in] value pointer to value of the option
* @param[in] value_len the length of the value
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int setsockopt(int sockfd, int level, int option, FAR const void *value, socklen_t value_len);

/**
* @brief   get the socket options
*
* @details @b #include <sys/socket.h>\n
* SYSTEM CALL API\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor of the socket
* @param[in] level the protocol level at which the option resides
* @param[in] option a single option to be retrieved
* @param[out] value pointer to value of the option retrieved
* @param[out] value_len the length of the value retrieved
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int getsockopt(int sockfd, int level, int option, FAR void *value, FAR socklen_t *value_len);
/**
* @brief  get the socket name
*
* @details @b #include <sys/socket.h>\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] sockfd the file descriptor associated with the socket.
* @param[inout] addr  null or pointer to a sockaddr structure where the address of the local socket will be returned
* @param[inout] addrlen on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int getsockname(int sockfd, FAR struct sockaddr *addr, FAR socklen_t *addrlen);

/**
* @brief  get the name of the peer socket
*
* @details @b #include <sys/socket.h>\n
* POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
* @param[in] s the file descriptor associated with the socket.
* @param[inout] name  null or pointer to a sockaddr structure where the address of the peer socket will be returned
* @param[inout] namelen on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.
* @return On success, 0 is returned. On failure, -1 is returned.
* @since TizenRT v1.0
*/
int getpeername(int s, struct sockaddr *name, socklen_t *namelen);

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_SYS_SOCKET_H */

/** @} */// end of NETWORK group
