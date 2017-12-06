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
 * Copyright (c) 2001, 02  Motoyuki Kasahara
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <netdb.h>
#include <errno.h>

#include "lib_internal.h"
#include "netdb/lib_netdb.h"

#ifdef CONFIG_LIBC_NETDB

/*
 * Default hints for getaddrinfo().
 */
static struct addrinfo default_hints = {
	0, PF_UNSPEC, 0, 0, 0, NULL, NULL, NULL
};

/*
 * Return 1 if the string `s' represents an integer.
 */
static int is_integer(const char *s)
{
	if (*s == '-' || *s == '+') {
		s++;
	}
	if (*s < '0' || '9' < *s) {
		return 0;
	}

	s++;
	while ('0' <= *s && *s <= '9') {
		s++;
	}

	return (*s == '\0');
}

/*
 * Return 1 if the string `s' represents an IPv4 address.
 * Unlike inet_addr(), it doesn't permit malformed nortation such
 * as "192.168".
 */
static int is_address(const char *s)
{
	const static char delimiters[] = { '.', '.', '.', '\0' };
	int i, j;
	int octet;

	for (i = 0; i < 4; i++) {
		if (*s == '0' && *(s + 1) != delimiters[i]) {
			return 0;
		}
		for (j = 0, octet = 0; '0' <= *s && *s <= '9' && j < 3; s++, j++) {
			octet = octet * 10 + (*s - '0');
		}
		if (j == 0 || octet > 255 || *s != delimiters[i]) {
			return 0;
		}
		s++;
	}

	return 1;
}

/*
 * getaddrinfo().
 */
int getaddrinfo(nodename, servname, hints, res)
const char *nodename;
const char *servname;
const struct addrinfo *hints;
struct addrinfo **res;
{
	struct addrinfo *head_res = NULL;
	struct addrinfo *tail_res = NULL;
	struct addrinfo *new_res;
	struct sockaddr_in *sa_in;
	struct in_addr **addr_list;
	struct in_addr *addr_list_buf[2];
	struct in_addr addr_buf;
	struct in_addr **ap;
	struct servent *servent;
	struct hostent *hostent;
	const char *canonname = NULL;
	in_port_t port;
	int saved_h_errno;
	int result = 0;

#ifdef ENABLE_PTHREAD
	pthread_mutex_lock(&gai_mutex);
#endif

	saved_h_errno = h_errno;

	if (nodename == NULL && servname == NULL) {
		result = EAI_NONAME;
		goto end;
	}

	if (hints != NULL) {
		if (hints->ai_family != PF_INET && hints->ai_family != PF_UNSPEC) {
			result = EAI_FAMILY;
			goto end;
		}
		if (hints->ai_socktype != SOCK_DGRAM && hints->ai_socktype != SOCK_STREAM && hints->ai_socktype != 0) {
			result = EAI_SOCKTYPE;
			goto end;
		}
	} else {
		hints = &default_hints;
	}

	if (servname != NULL) {
		if (is_integer(servname)) {
			port = htons(atoi(servname));
		} else {
			if (hints->ai_flags & AI_NUMERICSERV) {
				result = EAI_NONAME;
				goto end;
			}

			if (hints->ai_socktype == SOCK_DGRAM) {
				servent = getservbyname(servname, "udp");
			} else if (hints->ai_socktype == SOCK_STREAM) {
				servent = getservbyname(servname, "tcp");
			} else if (hints->ai_socktype == 0) {
				servent = getservbyname(servname, "tcp");
			} else {
				result = EAI_SOCKTYPE;
				goto end;
			}

			if (servent == NULL) {
				result = EAI_SERVICE;
				goto end;
			}
			port = servent->s_port;
		}
	} else {
		port = htons(0);
	}

	if (nodename != NULL) {
		if (is_address(nodename)) {
			addr_buf.s_addr = inet_addr(nodename);
			addr_list_buf[0] = &addr_buf;
			addr_list_buf[1] = NULL;
			addr_list = addr_list_buf;

			if (hints->ai_flags & AI_CANONNAME && !(hints->ai_flags & AI_NUMERICHOST)) {
				hostent = gethostbyaddr((char *)&addr_buf, sizeof(struct in_addr), AF_INET);
				if (hostent != NULL) {
					canonname = hostent->h_name;
				} else {
					canonname = nodename;
				}
			}
		} else {
			if (hints->ai_flags & AI_NUMERICHOST) {
				result = EAI_NONAME;
				goto end;
			}

			hostent = gethostbyname(nodename);
			if (hostent == NULL) {
				switch (h_errno) {
				case HOST_NOT_FOUND:
				case NO_DATA:
					result = EAI_NONAME;
					goto end;
				case TRY_AGAIN:
					result = EAI_AGAIN;
					goto end;
				default:
					result = EAI_FAIL;
					goto end;
				}
			}
			addr_list = (struct in_addr **)hostent->h_addr_list;

			if (hints->ai_flags & AI_CANONNAME) {
				canonname = hostent->h_name;
			}
		}
	} else {
		if (hints->ai_flags & AI_PASSIVE) {
			addr_buf.s_addr = htonl(INADDR_ANY);
		} else {
			addr_buf.s_addr = htonl(0x7F000001);
		}
		addr_list_buf[0] = &addr_buf;
		addr_list_buf[1] = NULL;
		addr_list = addr_list_buf;
	}

	for (ap = addr_list; *ap != NULL; ap++) {
		new_res = (struct addrinfo *)malloc(sizeof(struct addrinfo));
		if (new_res == NULL) {
			if (head_res != NULL) {
				freeaddrinfo(head_res);
			}
			result = EAI_MEMORY;
			goto end;
		}

		new_res->ai_family = PF_INET;
		new_res->ai_socktype = hints->ai_socktype;
		new_res->ai_protocol = hints->ai_protocol;
		new_res->ai_addr = NULL;
		new_res->ai_addrlen = sizeof(struct sockaddr_in);
		new_res->ai_canonname = NULL;
		new_res->ai_next = NULL;

		new_res->ai_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr_in));
		if (new_res->ai_addr == NULL) {
			free(new_res);
			if (head_res != NULL) {
				freeaddrinfo(head_res);
			}
			result = EAI_MEMORY;
			goto end;
		}

		sa_in = (struct sockaddr_in *)new_res->ai_addr;
		memset(sa_in, 0, sizeof(struct sockaddr_in));
		sa_in->sin_family = PF_INET;
		sa_in->sin_port = port;
		memcpy(&sa_in->sin_addr, *ap, sizeof(struct in_addr));

		if (head_res == NULL) {
			head_res = new_res;
		} else {
			tail_res->ai_next = new_res;
		}
		tail_res = new_res;
	}

	if (canonname != NULL && head_res != NULL) {
		head_res->ai_canonname = (char *)malloc(strlen(canonname) + 1);
		if (head_res->ai_canonname != NULL) {
			strcpy(head_res->ai_canonname, canonname);
		}
	}

	*res = head_res;

end:
	h_errno = saved_h_errno;
#ifdef ENABLE_PTHREAD
	pthread_mutex_unlock(&gai_mutex);
#endif
	return result;
}

void freeaddrinfo(struct addrinfo *ai)
{
	struct addrinfo *next;

	for (; ai != NULL;) {
		next = ai->ai_next;
		if (ai->ai_addr) {
			free(ai->ai_addr);
		}
		if (ai->ai_canonname) {
			free(ai->ai_canonname);
		}
		free(ai);
		ai = next;
	}
}

#endif							/* CONFIG_LIBC_NETDB */
