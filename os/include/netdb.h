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
 * @defgroup NETDB DNS Client
 * @ingroup NETWORK
 * @brief Support LIBC compatible NETDB library based on LWIP APIs
 * @{
 */

/**
 * @file netdb.h
 * @brief Support LIBC compatible NETDB library based on LWIP APIs
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

#include <net/lwip/netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

/****************************************************************************
 * Public Data
 ****************************************************************************/
/** @cond HIDDEN */
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
/** @endcond */

/**
* @brief gethostbyname() is a function to get 32bit ip address from domain name or ip address string.
*
* @param[in] name This field can be the string of ip address or domain name
* @return On success, 0 is returned. On failure, a pointer value is returned
*/
struct hostent *gethostbyname(const char *name);

/**
* @brief freeaddrinfo() frees the memory allocated by the getaddrinfo() function.
*
* @param[in] ai This field is the head of the addrinfo list
* @return void
*/
void freeaddrinfo(struct addrinfo *ai);

/**
* @brief getaddrinfo() is a function that returns information on a particular host name (such as its IP address) and loads up a struct sockaddr.
*
* @param[in] nodename This field can be among a domain name, ip address and NULL
* @param[in] servname This field jcan be a port number passed as string or a service name
* @param[in] hints This field can be either NULL or an addrinfo structure with the type of service requested
* @param[out] res This field is a return pointer that points to a new addrinfo structure
* @return On success, 0 is returned. On failure, a fail number is returned
*/
int getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res);

/**
* @brief getnameinfo() is a function that returns translated string from 32bit(ipv4)/128bit(ipv6) IP address. As lwip doesn't support rarp and relative functions it has restricted usage.
*
* @param[in] sa This field is an ip address structure pointer
* @param[in] salen This field contains the length of sa structure, it is used to figure out ipv4/v6 structure
* @param[out] host This field is the return buffer that contains string that is translated from 32bit(ipv4)/128bit(ipv6) address.
* @param[out] hostlen This field contains the length of host buffer length.
* @param[out] serv This field is the return buffer that contains port number string.
* @param[out] servlen This field contains the length of serv buffer length.
* @param[in] flags This field is the bit field flag, this function supports NI_NUMERICHOST, NI_NUMERICSERV only.
* @return On success, 0 is returned. On failure, a fail number is returned
*/
int getnameinfo(const struct sockaddr *sa, size_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);

/** @cond HIDDEN */
/* REVISIT:  This should at least be per-task? */
EXTERN int h_errno;
/** @endcond */

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_NETDB_H */
/** @} */ // end of SOCKET group
