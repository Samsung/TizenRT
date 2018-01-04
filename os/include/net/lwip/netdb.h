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
#ifndef LWIP_HDR_NETDB_H
#define LWIP_HDR_NETDB_H

#include <net/lwip/opt.h>

#if LWIP_DNS && LWIP_SOCKET

#include <net/lwip/arch.h>
#include <net/lwip/inet.h>
#include <net/lwip/sockets.h>

#ifdef __cplusplus
extern "C" {
#endif

/* some rarely used options */
#ifndef LWIP_DNS_API_DECLARE_H_ERRNO
#define LWIP_DNS_API_DECLARE_H_ERRNO  1
#endif

#ifndef LWIP_DNS_API_DEFINE_ERRORS
#define LWIP_DNS_API_DEFINE_ERRORS    1
#endif

#ifndef LWIP_DNS_API_DEFINE_FLAGS
#define LWIP_DNS_API_DEFINE_FLAGS     1
#endif

#ifndef LWIP_DNS_API_DECLARE_STRUCTS
#define LWIP_DNS_API_DECLARE_STRUCTS  1
#endif

#if LWIP_DNS_API_DEFINE_ERRORS
/** Errors used by the DNS API functions, h_errno can be one of them */
#define EAI_NONAME      200
#define EAI_SERVICE     201
#define EAI_FAIL        202
#define EAI_MEMORY      203
#define EAI_FAMILY      204

#define HOST_NOT_FOUND  210
#define NO_DATA         211
#define NO_RECOVERY     212
#define TRY_AGAIN       213
#endif /* LWIP_DNS_API_DEFINE_ERRORS */

#if LWIP_DNS_API_DEFINE_FLAGS
/* input flags for struct addrinfo */
#define AI_PASSIVE      0x01
#define AI_CANONNAME    0x02
#define AI_NUMERICHOST  0x04
#define AI_NUMERICSERV  0x08
#define AI_V4MAPPED     0x10
#define AI_ALL          0x20
#define AI_ADDRCONFIG   0x40
#endif /* LWIP_DNS_API_DEFINE_FLAGS */

#if LWIP_DNS_API_DECLARE_STRUCTS
struct hostent {
	char *h_name;      /* Official name of the host. */
	char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
						terminated by a null pointer. */
	int h_addrtype;  /* Address type. */
	int h_length;    /* The length, in bytes, of the address. */
	char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
						network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
};

struct addrinfo {
	int ai_flags;      /* Input flags. */
	int ai_family;     /* Address family of socket. */
	int ai_socktype;   /* Socket type. */
	int ai_protocol;   /* Protocol of socket. */
	socklen_t ai_addrlen;    /* Length of socket address. */
	struct sockaddr *ai_addr;       /* Socket address of socket. */
	char *ai_canonname;  /* Canonical name of service location. */
	struct addrinfo *ai_next;       /* Pointer to next in list. */
};
#endif /* LWIP_DNS_API_DECLARE_STRUCTS */

#define NETDB_ELEM_SIZE           (sizeof(struct addrinfo) + sizeof(struct sockaddr_storage) + DNS_MAX_NAME_LENGTH + 1)

#if LWIP_DNS_API_DECLARE_H_ERRNO
/* application accessible error code set by the DNS API functions */
extern int h_errno;
#endif /* LWIP_DNS_API_DECLARE_H_ERRNO*/

struct hostent *lwip_gethostbyname(const char *name);
int lwip_gethostbyname_r(const char *name, struct hostent *ret, char *buf,
							size_t buflen, struct hostent **result, int *h_errnop);
void lwip_freeaddrinfo(struct addrinfo *ai);
int lwip_getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints,
									       struct addrinfo **res);

#if LWIP_COMPAT_SOCKETS
/** @ingroup netdbapi */
#define gethostbyname(name) lwip_gethostbyname(name)
/** @ingroup netdbapi */
#define gethostbyname_r(name, ret, buf, buflen, result, h_errnop) \
	lwip_gethostbyname_r(name, ret, buf, buflen, result, h_errnop)
/** @ingroup netdbapi */
#define freeaddrinfo(addrinfo) lwip_freeaddrinfo(addrinfo)
/** @ingroup netdbapi */
#define getaddrinfo(nodname, servname, hints, res) lwip_getaddrinfo(nodname, servname, hints, res)
#endif /* LWIP_COMPAT_SOCKETS */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DNS && LWIP_SOCKET */

#endif /* LWIP_HDR_NETDB_H */
