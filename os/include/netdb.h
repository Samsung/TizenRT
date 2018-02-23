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

#include <netinet/in.h>
#include <sys/socket.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The <netdb.h> header shall define the IPPORT_RESERVED macro with the
 * value of the highest reserved Internet port number.
 */

#define IPPORT_RESERVED 0xffff	/* No reserved port numbers */

/* The <netdb.h> header shall define the following macros that evaluate to
 * bitwise-distinct integer constants for use in the flags field of the
 * addrinfo structure:
 *
 *   AI_PASSIVE      - Socket address is intended for bind().
 *   AI_CANONNAME    - Request for canonical name.
 *   AI_NUMERICHOST  - Return numeric host address as name.
 *   AI_NUMERICSERV  - Inhibit service name resolution.
 *   AI_V4MAPPED     - If no IPv6 addresses are found, query for IPv4
 *                     addresses and return them to the caller as IPv4-mapped
 *                     IPv6 addresses.
 *   AI_ALL          - Query for both IPv4 and IPv6 addresses.
 *   AI_ADDRCONFIG   - Query for IPv4 addresses only when an IPv4 address is
 *                     configured; query for IPv6 addresses only when an IPv6
 *                     address is configured.
 */

#define AI_PASSIVE      (1 << 0)
#define AI_CANONNAME    (1 << 1)
#define AI_NUMERICHOST  (1 << 2)
#define AI_NUMERICSERV  (1 << 3)
#define AI_V4MAPPED     (1 << 4)
#define AI_ALL          (1 << 5)
#define AI_ADDRCONFIG   (1 << 6)

/* The <netdb.h> header shall define the following macros that evaluate to
 * bitwise-distinct integer constants for use in the flags argument to
 * getnameinfo():
 *
 *   NI_NOFQDN       - Only the nodename portion of the FQDN is returned for
 *                     local hosts.
 *   NI_NUMERICHOST  - The numeric form of the node's address is returned
 *                     instead of its name.
 *   NI_NAMEREQD     - Return an error if the node's name cannot be located
 *                     in the database.
 *   NI_NUMERICSERV  - The numeric form of the service address is returned
 *                     instead of its name.
 *   NI_NUMERICSCOPE - For IPv6 addresses, the numeric form of the scope
 *                     identifier is returned instead of its name.
 *   NI_DGRAM        - Indicates that the service is a datagram service
 *                     (SOCK_DGRAM).
 */

#define NI_NOFQDN       (1 << 0)
#define NI_NUMERICHOST  (1 << 1)
#define NI_NAMEREQD     (1 << 2)
#define NI_NUMERICSERV  (1 << 3)
#define NI_NUMERICSCOPE (1 << 4)
#define NI_DGRAM        (1 << 5)

/* The <netdb.h> header shall define the following macros for use as error
 * values for gethostbyaddr() and gethostbyname()
 */

#define HOST_NOT_FOUND  1
#define NO_DATA         2
#define NO_RECOVERY     3
#define TRY_AGAIN       4

/* Address Information Errors.  The <netdb.h> header shall define the
 * following macros for use as error values for getaddrinfo() and
 * getnameinfo():
 *
 *   EAI_AGAIN       - The name could not be resolved at this time. Future
 *                     attempts may succeed.
 *   EAI_BADFLAGS    - The flags had an invalid value.EAI_FAILA non-
 *                     recoverable error occurred.
 *   EAI_FAMILY      - The address family was not recognized or the address
 *                     length was invalid for the specified family.
 *   EAI_MEMORY      - There was a memory allocation failure.
 *   EAI_NONAME      - The name does not resolve for the supplied
 *                     parameters.  NI_NAMEREQD is set and the host's name
 *                     cannot be located, or both nodename and servname were
 *                     null.
 *   EAI_SERVICE     - The service passed was not recognized for the
 *                     specified socket type.
 *   EAI_SOCKTYPE    - The intended socket type was not recognized.
 *   EAI_SYSTEM      - A system error occurred. The error code can be found
 *                     in errno.
 *   EAI_OVERFLOW    - An argument buffer overflowed.
 */

#define EAI_AGAIN       1
#define EAI_BADFLAGS    2
#define EAI_FAMILY      3
#define EAI_MEMORY      4
#define EAI_NONAME      5
#define EAI_SERVICE     6
#define EAI_SOCKTYPE    7
#define EAI_SYSTEM      8
#define EAI_OVERFLOW    9
#define EAI_FAIL    -1

/* h_errno values that may be returned by gethosbyname(), gethostbyname_r(),
 * gethostbyaddr(), or gethostbyaddr_r()
 *
 *   HOST_NOT_FOUND - No such host is known.
 *
 *   NO_DATA - The server recognized the request and the name, but no
 *     address is available. Another type of request to the name server
 *     for the domain might return an answer.
 *
 *   NO_RECOVERY - An unexpected server failure occurred which cannot be
 *     recovered.
 *
 *   TRY_AGAIN - A temporary and possibly transient error occurred, such as
 *     a failure of a server to respond.
 */

#define HOST_NOT_FOUND 1
#define NO_DATA        2
#define NO_RECOVERY    3
#define TRY_AGAIN      4

#define _PATH_SERVICES  "/etc/services"

/****************************************************************************
 * Public Types
 ****************************************************************************/
/** hostent structure contains DNS clients parameters */
struct hostent {
	FAR char *h_name;			/* Official name of the host. */
	FAR char **h_aliases;		/* A pointer to an array of pointers to alternative
								 * host names, terminated by a null pointer. */
	int h_addrtype;				/* Address type. */
	int h_length;				/* The length, in bytes, of the address. */
	FAR char **h_addr_list;		/* A pointer to an array of pointers to network
								 * addresses (in network byte order) for the host,
								 * terminated by a null pointer. */
};

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

struct addrinfo {
	int ai_flags;				/* Input flags.  */
	int ai_family;				/* Address family of socket.  */
	int ai_socktype;			/* Socket type.  */
	int ai_protocol;			/* Protocol of socket.  */
	socklen_t ai_addrlen;		/* Length of socket address.  */

	FAR struct sockaddr *ai_addr;	/* Socket address of socket.  */
	FAR char *ai_canonname;		/* Canonical name of service location.  */
	FAR struct addrinfo *ai_next;	/* Pointer to next in list.  */
};

struct servent_data {
	void *fp;
	char **aliases;
	int maxaliases;
	int stayopen;
	char *line;
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* When the <netdb.h> header is included, h_errno shall be available as a
 * modifiable lvalue of type int. It is unspecified whether h_errno is a
 * macro or an identifier declared with external linkage.
 */

/* REVISIT:  This should at least be per-task? */
EXTERN int h_errno;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_LIBC_NETDB
#if 0							/* None of these are yet supported */

void endhostent(void);
void endnetent(void);
void endprotoent(void);

FAR const char *gai_strerror(int);

#endif

// @cond
void endservent(void);
// @endcond

/**
 * @brief getaddrinfo() is a function that returns information on a particular host name (such as its IP address) and loads up a struct sockaddr.
 *
 * @details @b #include <netdb.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] host can be either a domain name or NULL
 * @param[in] service can be a port number passed as string or a service name
 * @param[in] hints can be either NULL or an addrinfo structure with the type of service requested
 * @param[in] res is a pointer that points to a new addrinfo structure
 * @return On success, 0 is returned. On failure, a negative value is returned
 * @since TizenRT v1.0
 */
int getaddrinfo(const char *host, const char *service, const struct addrinfo *hints, struct addrinfo **res);

/**
 * @brief freeaddrinfo() frees the memory allocated by the getaddrinfo() function.
 *
 * @details @b #include <netdb.h>\n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @param[in] ai is the head of the addrinfo list
 * @return void
 * @since TizenRT v1.0
 */
void freeaddrinfo(FAR struct addrinfo *ai);

/**
 * @brief gethostbyaddr() is the function returns a corresponding hostname with given IP addresses
 *
 * @details @b #include <netdb.h>\n
 * @param[in] *addr host address sending DNS server (e.g., 192.168.0.0)
 * @return On success, hostent, the structure contains hostname of requested IP addresses, returns. On failure, hostname parameters in hostent structure is NULL
 * @since TizenRT v1.0
 */
FAR struct hostent *gethostbyaddr(FAR const void *addr, socklen_t len, int type);

/**
 * @brief gethostbyname() is the function sending DNS query to DNS server with hostname
 *
 * @details @b #include <netdb.h>\n
 * @param[in] *name hostname sending DNS server (e.g., www.samsung.com)
 * @return On success, hostent, the structure contains IP addresses of requested hostname, returns. On failure, IP address parameters in hostent structure is NULL
 * @since TizenRT v1.0
 */
FAR struct hostent *gethostbyname(FAR const char *name);

#if 0							/* None of these are yet supported */
FAR struct hostent *gethostent(void);

FAR struct netent *getnetbyaddr(uint32_t, int);
FAR struct netent *getnetbyname(FAR const char *);
FAR struct netent *getnetent(void);
FAR struct protoent *getprotobyname(FAR const char *);
FAR struct protoent *getprotobynumber(int);
FAR struct protoent *getprotoent(void);
FAR struct servent *getservbyport(int, FAR const char *);
void sethostent(int);
void setnetent(int);
void setprotoent(int);

#endif							/* None of these are yet supported */

// @cond
FAR struct servent *getservbyname(FAR const char *, FAR const char *);
FAR struct servent *getservent(void);
void setservent(int);

int getnameinfo(const struct sockaddr *sa, size_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);

/* Non-standard interfaces similar to Glibc 2 interfaces */

int gethostbyaddr_r(FAR const void *addr, socklen_t len, int type, FAR struct hostent *host, FAR char *buf, size_t buflen, int *h_errnop);
int gethostbyname_r(FAR const char *name, FAR struct hostent *host, FAR char *buf, size_t buflen, int *h_errnop);
// @endcond

#endif							/* CONFIG_LIBC_NETDB */

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_NETDB_H */
/**
 * @}
 */
