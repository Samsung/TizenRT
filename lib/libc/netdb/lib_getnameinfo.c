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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include "lib_internal.h"
#include "netdb/lib_netdb.h"

#ifdef CONFIG_LIBC_NETDB
int getnameinfo(const struct sockaddr *sa, size_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)
{
	struct sockaddr_in *sin = (struct sockaddr_in *)sa;
	struct hostent *hp;
	char tmpserv[16];

	if (serv != NULL) {
		snprintf(tmpserv, sizeof(tmpserv), "%d", ntohs(sin->sin_port));
		if (strlcpy(serv, tmpserv, servlen) >= servlen) {
			return (EAI_MEMORY);
		}
	}

	if (host != NULL) {
		if (flags & NI_NUMERICHOST) {
			if (strlcpy(host, (const char *)inet_ntoa(sin->sin_addr), hostlen) >= hostlen) {
				return (EAI_MEMORY);
			} else {
				return (0);
			}
		} else {
			hp = gethostbyaddr((char *)&sin->sin_addr, sizeof(struct in_addr), AF_INET);
			if (hp == NULL) {
				return (NO_DATA);
			}

			if (strlcpy(host, hp->h_name, hostlen) >= hostlen) {
				return (EAI_MEMORY);
			} else {
				return (0);
			}
		}
	}
	return (0);
}

#endif							/* CONFIG_LIBC_NETDB */
