/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <tinyara/kmalloc.h>
#include "lwip/opt.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/igmp.h"

/**********************************************************
 * Private Function
 **********************************************************/

static struct addrinfo *_copy_addrinfo(struct addrinfo *src)
{
	struct addrinfo *tmp = src;
	struct addrinfo *prev = NULL;
	struct addrinfo *root = NULL;
	while (tmp) {
		struct addrinfo *dst = NULL;
		dst = (struct addrinfo *)kumm_malloc(sizeof(struct addrinfo));
		if (!dst) {
			ndbg("_copy_addrinfo() kumm_malloc failed\n");
			break;
		}
		dst->ai_flags = tmp->ai_flags;
		dst->ai_family = tmp->ai_family;
		dst->ai_socktype = tmp->ai_socktype;
		dst->ai_protocol = tmp->ai_protocol;
		dst->ai_addrlen = tmp->ai_addrlen;

		dst->ai_addr = (struct sockaddr *)kumm_malloc(sizeof(struct sockaddr));
		if (!dst->ai_addr) {
			ndbg("_copy_addrinfo() kumm_malloc failed\n");
			kumm_free(dst);
			break;
		}
		memcpy(dst->ai_addr, tmp->ai_addr, sizeof(struct sockaddr));

		if (tmp->ai_canonname) {
			dst->ai_canonname = (char *)kumm_malloc(sizeof(tmp->ai_canonname));
			if (!dst->ai_canonname) {
				ndbg("_copy_addrinfo() kumm_malloc failed\n");
				kumm_free(dst->ai_addr);
				kumm_free(dst);
				break;
			}
			memcpy(dst->ai_canonname, tmp->ai_canonname, sizeof(tmp->ai_canonname));
		} else {
			dst->ai_canonname = NULL;
		}

		dst->ai_next = NULL;
		if (prev) {
			prev->ai_next = dst;
		} else {
			root = dst;
		}
		tmp = tmp->ai_next;
		prev = dst;
	}

	return root;
}

static int _free_addrinfo(struct addrinfo *ai)
{
	struct addrinfo *next;

	while (ai != NULL) {
		next = ai->ai_next;
		kumm_free(ai);
		ai = next;
	}
	return 0;
}

/****************************************************************************
 * Function: lwip_func_ioctl
 *
 * Description:
 *   Call lwip API
 *
 * Parameters:
 *   cmd      The ioctl command
 *   arg      Type of the information to get
 *
 * Returned Value:
 *   0 on success, negated errno on failure.
 *
 ****************************************************************************/
static int lwip_func_ioctl(int cmd, void *arg)
{
	int ret = -EINVAL;
	struct req_lwip_data *in_arg = (struct req_lwip_data *)arg;
	if (!in_arg) {
		return ret;
	}

#if LWIP_DNS
	struct addrinfo *res = NULL;
	struct hostent *host_ent = NULL;
	struct hostent *user_ent = NULL;
#endif

	switch (in_arg->type) {
#if LWIP_DNS
	case GETADDRINFO:
		in_arg->req_res = lwip_getaddrinfo(in_arg->host_name, in_arg->serv_name, in_arg->ai_hint, &res);
		if (in_arg->req_res != 0) {
			ndbg("lwip_getaddrinfo() returned with the error code: %d\n", ret);
			in_arg->ai_res = NULL;
			ret = -EINVAL;
		} else {
			in_arg->ai_res = _copy_addrinfo(res);
			ret = OK;
		}
		break;
	case FREEADDRINFO:
		in_arg->req_res = _free_addrinfo(in_arg->ai);
		ret = OK;
		break;
	case DNSSETSERVER:
		dns_setserver(in_arg->num_dns, in_arg->dns_server);
		ret = OK;
		break;
	case GETHOSTBYNAME:
		host_ent = lwip_gethostbyname(in_arg->host_name);
		if (!host_ent) {
			ndbg("lwip_gethostbyname() returned with the error code: %d\n", HOST_NOT_FOUND);
			ret = -EINVAL;
		} else {
			user_ent = in_arg->host_entry;
			strncpy(user_ent->h_name, host_ent->h_name, DNS_MAX_NAME_LENGTH);
			user_ent->h_name[DNS_MAX_NAME_LENGTH] = 0;
			memcpy(user_ent->h_addr_list[0], host_ent->h_addr_list[0], sizeof(struct in_addr));
			user_ent->h_addrtype = host_ent->h_addrtype;
			user_ent->h_length = host_ent->h_length;

			ret	= OK;
		}
		break;
	case GETNAMEINFO:
		in_arg->req_res = lwip_getnameinfo(in_arg->sa, in_arg->sa_len, (char *)in_arg->host_name, in_arg->host_len, (char *)in_arg->serv_name, in_arg->serv_len, in_arg->flags);
		if (in_arg->req_res != 0) {
			ndbg("lwip_getnameinfo() returned with the error code: %d\n", ret);
			ret = -EINVAL;
		} else {
			ret = OK;
		}
		break;
#endif

#if defined(CONFIG_NET_LWIP_DHCP)
#if defined(CONFIG_LWIP_DHCPC)
	case DHCPCSTART:
		in_arg->req_res = netdev_dhcp_client_start((const char *)in_arg->host_name);
		if (in_arg->req_res != 0) {
			ret = -EINVAL;
			ndbg("start dhcp fail\n");
		} else {
			ret = OK;
		}
		break;
	case DHCPCSTOP:
		netdev_dhcp_client_stop((const char *)in_arg->host_name);
		in_arg->req_res = 0;
		ret = OK;
		break;
#endif

#if defined(CONFIG_LWIP_DHCPS)
	case DHCPDSTART:
		in_arg->req_res = netdev_dhcp_server_start((char *)in_arg->host_name, _dhcpd_join);
		if (in_arg->req_res != 0) {
			ret = -EINVAL;
			ndbg("start dhcpd fail\n");
		} else {
			ret = OK;
		}
		break;
	case DHCPDSTOP:
		in_arg->req_res = netdev_dhcp_server_stop((char *)in_arg->host_name);
		if (in_arg->req_res != 0) {
			ret = -EINVAL;
			ndbg("stop dhcpd fail\n");
		} else {
			ret = OK;
		}
		break;
	case DHCPDSTATUS:
		in_arg->req_res = netdev_dhcp_server_status((char *)in_arg->host_name);
		if (in_arg->req_res != 0) {
			ret = -EINVAL;
			ndbg("stop dhcpd fail\n");
		} else {
			ret = OK;
		}
		break;
#endif // CONFIG_LWIP_DHCPS
#endif // CONFIG_NET_LWIP_DHCP
	default:
		ndbg("Wrong request type: %d\n", in_arg->type);
		break;
	}

	return ret;
}

/****************************************************************************
 * Function: lwipioctl
 *
 * Description:
 *   Call lwip_ioctl() with FIONREAD/FIONBIO commands or
 *   call lwip API with SIOCLWIP command
 *
 * Parameters:
 *   sockfd   Socket file descriptor
 *   cmd      The ioctl command
 *   arg      Type of the information to get
 *
 * Returned Value:
 *   0 on success, negated errno on failure.
 *
 ****************************************************************************/

int netdev_lwipioctl(int sockfd, int cmd, void *arg)
{
	int ret = -ENOTTY;

	if (cmd == FIONREAD || cmd == FIONBIO) {
		ret = lwip_ioctl(sockfd, (long)cmd, arg);
		if (ret == -1) {
			return -get_errno();
		}
	} else if (cmd == SIOCLWIP) {
		return lwip_func_ioctl(cmd, arg);
	}

	return ret;
}
