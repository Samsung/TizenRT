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
 * include/arpa/inet.h
 *
 *   Copyright (C) 2007, 2009 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
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
 * @file arpa/inet.h
 * @brief inet API
 */

#ifndef __INCLUDE_ARPA_INET_H
#define __INCLUDE_ARPA_INET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/
/*  Length of the string form for IP address (excludes NULL termination) */

#define INET_ADDRSTRLEN 16

/*  Length of the string form for IPv6 address (excludes NULL termination) */

#define INET6_ADDRSTRLEN 46

/* This macro to convert a 16/32-bit constant values quantity from host byte
 * order to network byte order.  The 16-bit version of this macro is required
 * for uIP:
 *
 *   Author Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2001-2003, Adam Dunkels.
 *   All rights reserved.
 */

#ifdef CONFIG_ENDIAN_BIG
# define HTONS(ns) (ns)
# define HTONL(nl) (nl)
#else
# define HTONS(ns) \
	(unsigned short) \
	(((((unsigned short)(ns)) & 0x00ff) << 8) | \
	((((unsigned short)(ns)) >> 8) & 0x00ff))
# define HTONL(nl) \
	(unsigned long) \
	(((((unsigned long)(nl)) & 0x000000ffUL) << 24) | \
	((((unsigned long)(nl)) & 0x0000ff00UL) <<  8) | \
	((((unsigned long)(nl)) & 0x00ff0000UL) >>  8) | \
	((((unsigned long)(nl)) & 0xff000000UL) >> 24))
#endif

#define NTOHS(hs) HTONS(hs)
#define NTOHL(hl) HTONL(hl)


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* Functions to convert between nost and network byte ordering */

/**
 * @brief Convert the unsigned integer netlong from network byte order to host byte order
 *
 * @details @b #include <arpa/inet.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] nl netlong
 *
 * @return On success, the host byte-ordered netlong is returned. On failure, -1 is returned.
*/
uint32_t ntohl(uint32_t nl);

/**
 * @brief Convert the unsigned short integer netshort from network byte order to host byte order
 *
 * @details @b #include <arpa/inet.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] ns netshort
 *
 * @return On success, the host byte-ordered netshort is returned. On failure, -1 is returned.
*/
uint16_t ntohs(uint16_t ns);

/**
 * @brief Convert the unsigned integer hostlong from host byte order to network byte order
 *
 * @details @b #include <arpa/inet.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] hl hostlong
 *
 * @return On success, the network byte-ordered hostlong is returned. On failure, -1 is returned.
*/
uint32_t htonl(uint32_t hl);

/**
 * @brief Convert the unsigned short integer hostshort from host byte order to network byte order
 *
 * @details @b #include <arpa/inet.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] hs hostshort
 *
 * @return On success, the network byte-ordered hostshort is returned. On failure, -1 is returned.
*/
uint16_t htons(uint16_t hs);

/* Functions to manipulate address representations */
/**
 * @brief Convert the Internet host address from the IPv4 numbers-and-dots notation into binary form
 *
 * @details @b #include <arpa/inet.h>
 * @param[in] cp the Internet host address
 * @param[in] inp structure that will store the converted value (IPv4 numbers-and-dots notation)
 *
 * @return On success, nonzero is returned. On failure, zero is returned.
*/
int inet_aton(FAR const char *cp, FAR struct in_addr *inp);

/**
 * @brief Convert the Internet host address from the IPv4 numbers-and-dots notation into binary data in network byte order.
 *
 * @details @b #include <arpa/inet.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] cp the Internet host address
 *
 * @return On success, binary data in network byte order is returned. On failure, INADDR_NONE(usually -1) is returned.
*/
in_addr_t inet_addr(FAR const char *cp);

///@cond
in_addr_t inet_network(FAR const char *cp);
///@endcond

#ifdef CONFIG_CAN_PASS_STRUCTS
/**
 * @brief Convert the Internet host address to a string in IPv4 dotted-decimal notation.
 *
 * @details @b #include <arpa/inet.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] in the Internet host address in network byte order
 *
 * @return On success, a string in IPv4 dotted-decimal notation is returned. On failure, -1 is returned.
*/
FAR char *inet_ntoa(struct in_addr in);

///@cond
in_addr_t inet_lnaof(struct in_addr in);
in_addr_t inet_netof(struct in_addr in);
///@endcond
#else
///@cond
FAR char *_inet_ntoa(in_addr_t in);
# define inet_ntoa(in) _inet_ntoa(in.s_addr);

in_addr_t _inet_lnaof(in_addr_t in);
# define inet_lnaof(in) _inet_lnaof(in.s_addr);

in_addr_t _inet_netof(in_addr_t in);
# define inet_netof(in) _inet_netof(in.s_addr);
///@endcond
#endif
///@cond
struct in_addr inet_makeaddr(in_addr_t net, in_addr_t host);

int inet_pton(int af, FAR const char *src, FAR void *dst);
const char *inet_ntop(int af, FAR const void *src, FAR char *dst, socklen_t size);
///@endcond

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_ARPA_INET_H */

/** @} */// end of SOCKET group
