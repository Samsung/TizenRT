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
 * include/netdb.h
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Reference: http://pubs.opengroup.org/onlinepubs/009695399/basedefs/netdb.h.html
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
 * @addtogroup SOCKET
 * @ingroup NETWORK
 * @{
 */

/**
 * @file netdb.h
 * @brief DNS client API
 */

#ifndef __INCLUDE_NETDB_H
#define __INCLUDE_NETDB_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

/* Inclusion of the <netdb.h> header may also make visible all symbols from
 * <netinet/in.h>, <sys/socket.h>, and <inttypes.h>.
 */

#include <inttypes.h>

#include "lwip/netdb.h"
#include <netinet/in.h>
#include <sys/socket.h>

 /****************************************************************************
 * Public Types
 ****************************************************************************/
#define h_addr h_addr_list[0]	/* For backward compatibility */

struct netent {
	FAR char *n_name;			/* Official, fully-qualified (including the domain)
								 * name of the host. */
	FAR char **n_aliases;		/* A pointer to an array of pointers to alternative
								 * network names, terminated by a null pointer. */
	int n_addrtype;				/* The address type of the network. */
	uint32_t n_net;				/* The network number, in host byte order. */
};

struct protoent {
	FAR char *p_name;			/* Official name of the protocol. */
	FAR char **p_aliases;		/* A pointer to an array of pointers to
								 * alternative protocol names, terminated by a
								 * null pointer. */
	int p_proto;				/* The protocol number. */
};

struct servent {
	FAR char *s_name;			/* Official name of the service. */
	FAR char **s_aliases;		/* A pointer to an array of pointers to
								 * alternative service names, terminated by a
								 * null pointer.  */
	int s_port;					/* The port number at which the service resides,
								 * in network byte order. */
	FAR char *s_proto;			/* The name of the protocol to use when
								 * contacting the service. */
};

struct servent_data {
	void *fp;
	char **aliases;
	int maxaliases;
	int stayopen;
	char *line;
};

#if CONFIG_NET_LWIP

typedef enum {
	GETADDRINFO,
	FREEADDRINFO,
	GETHOSTBYNAME,
	GETNAMEINFO,
	DNSSETSERVER,
	DHCPCSTART,
	DHCPCSTOP,
	DHCPDSTART,
	DHCPDSTOP,
	DHCPDSTATUS,
	GETNETSTATS,
} req_type;

/* To send a request to lwip stack by ioctl() use */
struct req_lwip_data {
	req_type type;
	int req_res;
	const char *intf;
	const char *host_name;
	const char *serv_name;
	const struct addrinfo *ai_hint;
	struct addrinfo *ai_res;
	struct addrinfo *ai;
	struct hostent *host_entry;
	const struct sockaddr *sa;
	size_t sa_len;
	size_t host_len;
	size_t serv_len;
	int flags;
	int index;
	struct sockaddr *addr;
};

#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
* @brief gethostbyname is a function to get 32bit ip address from domain name or ip address string.
*
* @param[in] name can be the string of ip address or domain name
* @return On success, 0 is returned. On failure, a pointer value is returned
* @since Tizen RT v1.0
*/
struct hostent *gethostbyname(const char *name);

/**
* @brief freeaddrinfo() frees the memory allocated by the getaddrinfo() function.
*
* @param[in] ai is the head of the addrinfo list
* @return void
* @since Tizen RT v1.0
*/
void freeaddrinfo(struct addrinfo *ai);

/**
* @brief getaddrinfo() is a function that returns information on a particular host name (such as its IP address) and loads up a struct sockaddr.
*
* @param[in] nodename can be among a domain name, ip address and NULL
* @param[in] servname can be a port number passed as string or a service name
* @param[in] hints can be either NULL or an addrinfo structure with the type of service requested
* @param[out] res is a return pointer that points to a new addrinfo structure
* @return On success, 0 is returned. On failure, a fail number is returned
* @since Tizen RT v1.0
*/
int getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res);

/**
* @brief getnameinfo() is a function that returns translated string from 32bit(ipv4)/128bit(ipv6) IP address. As lwip doesn't support rarp and relative functions it has restricted usage.
*
* @param[in] sa is an ip address structure pointer
* @param[in] salen contains the length of sa structure, it is used to figure out ipv4/v6 structure
* @param[out] host is the return buffer that contains string that is translated from 32bit(ipv4)/128bit(ipv6) address.
* @param[out] hostlen contains the length of host buffer length.
* @param[out] serv is the return buffer that contains port number string.
* @param[out] servlen contains the length of serv buffer length.
* @param[in] flags is the bit field flag, this function supports NI_NUMERICHOST, NI_NUMERICSERV only.
* @return On success, 0 is returned. On failure, a fail number is returned
* @since Tizen RT v1.0
*/
int getnameinfo(const struct sockaddr *sa, size_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);

/* REVISIT:  This should at least be per-task? */
EXTERN int h_errno;

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_NETDB_H */
/** @} */
