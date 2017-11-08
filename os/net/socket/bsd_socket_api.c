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
/*
 * Copyright (c) 2016 Samsung Electronics co. ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack's wrapper
 *
 * Author: Byoungtae Cho <bt.cho@samsung.com>
 *
 */

/**
 * @file
 * Sockets BSD-Like API wrapper module
 *
 */

#include <tinyara/config.h>

#ifdef CONFIG_NET

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/lwip/netdb.h>
#include <net/lwip/sockets.h>
#ifdef CONFIG_NET_IPv4
#include <net/lwip/ip4_addr.h>
#endif
#ifdef CONFIG_NET_IPv6
#include <net/lwip/ip6_addr.h>
#endif

int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_bind(s, name, namelen);
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	return lwip_accept(s, addr, addrlen);
}

int shutdown(int s, int how)
{
	return lwip_shutdown(s, how);
}

int closesocket(int s)
{
	return lwip_close(s);
}

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_connect(s, name, namelen);
}

int getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getsockname(s, name, namelen);
}

int getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getpeername(s, name, namelen);
}

int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	return lwip_setsockopt(s, level, optname, optval, optlen);
}

int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	return lwip_getsockopt(s, level, optname, optval, optlen);
}

int listen(int s, int backlog)
{
	return lwip_listen(s, backlog);
}

int recv(int s, void *mem, size_t len, int flags)
{
	return lwip_recv(s, mem, len, flags);
}

int recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	return lwip_recvfrom(s, mem, len, flags, from, fromlen);
}

int send(int s, const void *data, size_t size, int flags)
{
	return lwip_send(s, data, size, flags);
}

int sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	return lwip_sendto(s, data, size, flags, to, tolen);
}

int recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	return lwip_recvmsg(sockfd, msg, flags);
}

int sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
	return lwip_sendmsg(sockfd, msg, flags);
}

int socket(int domain, int type, int protocol)
{
	return lwip_socket(domain, type, protocol);
}

#ifdef CONFIG_DISABLE_POLL
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
	return lwip_select(maxfdp1, readset, writeset, exceptset, timeout);
}
#endif

int ioctlsocket(int s, long cmd, void *argp)
{
	return lwip_ioctl(s, cmd, argp);
}

uint16_t htons(uint16_t hs)
{
#if BYTE_ORDER == BIG_ENDIAN
	return hs;
#else
	return lwip_htons(hs);
#endif
}

uint16_t ntohs(uint16_t ns)
{
#if BYTE_ORDER == BIG_ENDIAN
	return ns;
#else
	return lwip_htons(ns);
#endif
}

uint32_t ntohl(uint32_t nl)
{
#if BYTE_ORDER == BIG_ENDIAN
	return nl;
#else
	return lwip_htonl(nl);
#endif
}

uint32_t htonl(uint32_t nl)
{
#if BYTE_ORDER == BIG_ENDIAN
	return nl;
#else
	return lwip_htonl(nl);
#endif
}

FAR char *inet_ntoa(struct in_addr in)
{
	return ip4addr_ntoa((const ip4_addr_t *)&(in));
}

in_addr_t inet_addr(FAR const char *cp)
{
	return ipaddr_addr(cp);
}

int inet_aton(const char *cp, struct in_addr *inp)
{
	return ip4addr_aton(cp, (ip4_addr_t *)inp);
}

FAR const char *inet_ntop(int af, FAR const void *src, FAR char *dest, socklen_t size)
{

#ifdef CONFIG_NET_IPv4
	if (af == AF_INET) {
		return ip4addr_ntoa_r((const ip4_addr_t *)(src), ((char *)(dest)), (size));
	} else
#endif
#ifdef CONFIG_NET_IPv6
	if (af == AF_INET6) {
		return ip6addr_ntoa_r((const ip6_addr_t *)(src), ((char *)(dest)), (size));
	}
#endif
	return NULL;
}

int inet_pton(int af, FAR const char *src, FAR void *dest)
{
#ifdef CONFIG_NET_IPv4
	if (af == AF_INET) {
		return ip4addr_aton(((const char *)(src)), (ip4_addr_t *)((char *)(dest)));
	} else
#endif
#ifdef CONFIG_NET_IPv6
	if (af == AF_INET6) {
		return ip6addr_aton(((const char *)(src)), ((ip6_addr_t *)(dest)));
	}
#endif
	return 0;
}

#ifdef CONFIG_NET_LWIP_NETDB
struct hostent *gethostbyname(const char *name)
{
	return lwip_gethostbyname(name);
}

void freeaddrinfo(struct addrinfo *ai)
{
	lwip_freeaddrinfo(ai);
}

int getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res)
{
	return lwip_getaddrinfo(nodename, servname, hints, res);
}

#if LWIP_COMPAT_SOCKETS
int getnameinfo(const struct sockaddr *sa, size_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)
{
	return lwip_getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
}
#endif /* LWIP_COMPAT_SOCKETS */

#endif /* NET_LWIP_NETDB */

#endif
