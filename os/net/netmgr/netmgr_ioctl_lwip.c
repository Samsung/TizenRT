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
#include <ifaddrs.h>
#include <tinyara/kmalloc.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include <net/if.h>
#include "netdev_mgr_internal.h"
#include "netdev_stats.h"
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/netdb.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/igmp.h"
#include "lwip/netifapi.h"

#if defined(CONFIG_NET_LWIP_DHCP)
#define GET_NETIF_FROM_NETDEV(dev) (struct netif *)(((struct netdev_ops *)(dev)->ops)->nic)

static struct netif *_netdev_dhcp_dev(FAR const char *intf)
{
	struct netif *cnif;

	if (intf == NULL) {
		ndbg("Input NULL\n");
		return NULL;
	}

	cnif = GET_NETIF_FROM_NETDEV(nm_get_netdev((uint8_t *)intf));
	if (cnif == NULL) {
		ndbg("No netif which has the name of %s\n", intf);
		return NULL;
	}

	return cnif;
}
#endif

static int _netdev_set_dnsserver(struct sockaddr *addr, int index)
{
	ip_addr_t dns_addr;
	struct sockaddr_in *iaddr = (struct sockaddr_in *)addr;
	ip_addr_set_ip4_u32(&dns_addr, iaddr->sin_addr.s_addr);
	if (index > 0) {
		if (index >= CONFIG_NET_DNS_MAX_SERVERS) {
			return -2;
		} else {
			dns_setserver(index, &dns_addr);
			return 0;
		}
	}
	/* Set DNS server to available slot.*/
	for (int i = 0; i < CONFIG_NET_DNS_MAX_SERVERS; i++) {
		if (ip_addr_isany_val(*(dns_getserver(i)))) {
			dns_setserver(i, &dns_addr);
			return 0;
		} else if (ip_addr_cmp(dns_getserver(i), &dns_addr)) {
			return 0;
		}
	}
	return -3;
}

/**********************************************************
 * Private Function
 **********************************************************/
#if defined(CONFIG_LWIP_DHCPC)
static void _netdev_set_ipv4addr(FAR struct netif *dev,
								FAR const struct in_addr *ip,
								FAR const struct in_addr *netmask,
								FAR const struct in_addr *gw)
{
	ip_addr_t t_ip;
	ip_addr_t t_netmask;
	ip_addr_t t_gw;

	if (ip) {
		ip_2_ip4(&t_ip)->addr = ip->s_addr;
	} else {
		t_ip = dev->ip_addr;
	}

	if (netmask) {
		ip_2_ip4(&t_ip)->addr = netmask->s_addr;
	} else {
		t_ip = dev->netmask;
	}

	if (gw) {
		ip_2_ip4(&t_gw)->addr = gw->s_addr;
	} else {
		t_gw = dev->gw;
	}

	netifapi_netif_set_addr(dev, ip_2_ip4(&t_ip), ip_2_ip4(&t_netmask), ip_2_ip4(&t_gw));
	netifapi_netif_set_up(dev);
}

int _netdev_dhcpc_start(const char *intf)
{
	nvdbg("LWIP DHCPC started (%s)\n", intf);
	struct netif *cnif;
	cnif = _netdev_dhcp_dev(intf);
	if (cnif == NULL) {
		ndbg("No network interface for dhcpc %s\n", intf);
		return ERROR;
	}
	int32_t timeleft = CONFIG_LWIP_DHCPC_TIMEOUT;
	struct in_addr l_ip = { .s_addr = IPADDR_ANY };
	struct in_addr l_netmask = { .s_addr = IPADDR_ANY };
	struct in_addr l_gw = { .s_addr = IPADDR_ANY };

	struct in_addr ip_check;

	/* Initialize dhcp structure if exists */
	if (netif_dhcp_data(cnif)) {
		netifapi_dhcp_stop(cnif);
	}

	_netdev_set_ipv4addr(cnif, &l_ip, &l_netmask, &l_gw);

	err_t res = netifapi_dhcp_start(cnif);
	if (res) {
		ndbg("DHCP client start failed %d\n", res);
		return ERROR;
	}

	nvdbg("DHCP client started successfully, waiting IP address (timeout %d msecs)\n", timeleft);
	while (netifapi_dhcp_address_valid(cnif) != 0) {
		usleep(100000);
		timeleft -= 100;
		if (timeleft <= 0) {
			ndbg("DHCP client timeout\n");
			netifapi_dhcp_stop(cnif);
			return ERROR;
		}
	}

	memcpy(&ip_check, &ip4_addr_get_u32(ip_2_ip4(&cnif->ip_addr)), sizeof(struct in_addr));
	ndbg("Got IP address %s\n", inet_ntoa(ip_check));

	return 0;
}

int _netdev_dhcpc_stop(const char *intf)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	struct netif *cnif;
	cnif = _netdev_dhcp_dev(intf);
	if (cnif == NULL) {
		ndbg("No network interface for dhcpc %s\n", intf);
		return ERROR;
	}
	_netdev_set_ipv4addr(cnif, &in, &in, &in);
	netifapi_dhcp_stop(cnif);
	nvdbg("Release IP address (lwip)\n");
	return OK;
}
#endif

#if defined(CONFIG_LWIP_DHCPS)
#define DHCPD_MQ_NAME "dhcpd_queue"
#define DHCPD_MQ_LEN 11
#define DHCPD_MQ_MAX_LEN 20

void _netdev_dhcp_join_cb(dhcp_evt_type_e type, void *data)
{
	ndbg("dhcpd joined");

	struct mq_attr attr;
	attr.mq_maxmsg = DHCPD_MQ_MAX_LEN;
	attr.mq_msgsize = DHCPD_MQ_LEN;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	mqd_t md = mq_open(DHCPD_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (!md) {
		ndbg("mq open fail (errno %d)\n", errno);
		mq_close(md);
		return;
	}

	char msg[DHCPD_MQ_LEN];
	dhcp_node_s *node = (dhcp_node_s *)data;
	msg[0] = 1;
	memcpy(&msg[1], &node->ipaddr, 4);
	memcpy(&msg[5], &node->macaddr, 6);
	int mq_ret = mq_send(md, msg, DHCPD_MQ_LEN, 100);
	if (mq_ret < 0) {
		ndbg("send mq fail (errno %d)\n", errno);
	}

	mq_close(md);

	return;
}
#endif

#if LWIP_DNS
static struct addrinfo *_netdev_copy_addrinfo(struct addrinfo *src)
{
	struct addrinfo *tmp = src;
	struct addrinfo *prev = NULL;
	struct addrinfo *root = NULL;
	while (tmp) {
		struct addrinfo *dst = NULL;
		dst = (struct addrinfo *)kumm_malloc(sizeof(struct addrinfo));
		if (!dst) {
			ndbg("kumm_malloc failed\n");
			break;
		}
		dst->ai_flags = tmp->ai_flags;
		dst->ai_family = tmp->ai_family;
		dst->ai_socktype = tmp->ai_socktype;
		dst->ai_protocol = tmp->ai_protocol;
		dst->ai_addrlen = tmp->ai_addrlen;

		dst->ai_addr = (struct sockaddr *)kumm_malloc(sizeof(struct sockaddr));
		if (!dst->ai_addr) {
			ndbg("kumm_malloc failed\n");
			kumm_free(dst);
			break;
		}
		memcpy(dst->ai_addr, tmp->ai_addr, sizeof(struct sockaddr));

		if (tmp->ai_canonname) {
			dst->ai_canonname = (char *)kumm_malloc(strlen(tmp->ai_canonname) + 1);
			if (!dst->ai_canonname) {
				ndbg("kumm_malloc failed\n");
				kumm_free(dst->ai_addr);
				kumm_free(dst);
				break;
			}
			memcpy(dst->ai_canonname, tmp->ai_canonname, strlen(tmp->ai_canonname) + 1);
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

static int _netdev_free_addrinfo(struct addrinfo *ai)
{
	struct addrinfo *next;

	while (ai != NULL) {
		next = ai->ai_next;
		if (ai->ai_addr) {
			kumm_free(ai->ai_addr);
			ai->ai_addr = NULL;
		}
		if (ai->ai_canonname) {
			kumm_free(ai->ai_canonname);
			ai->ai_canonname = NULL;
		}
		kumm_free(ai);
		ai = next;
	}
	return 0;
}
#endif

/****************************************************************************
 * Function: lwip_func_ioctl
 *
 * Description:
 *   Call lwip API
 *
 * Parameters:
 *   s        Descriptor of device
 *   cmd      The ioctl command
 *   arg      Type of the information to get
 *
 * Returned Value:
 *   0 on success, negated errno on failure.
 *
 ****************************************************************************/
static int lwip_func_ioctl(int s, int cmd, void *arg)
{
	int ret = -EINVAL;

	struct lwip_sock *sock = get_socket(s, getpid());
	if (!sock) {
		ret = -EBADF;
		return ret;
	}
	struct req_lwip_data *req = (struct req_lwip_data *)arg;
	if (!req) {
		goto errout;
	}

#if LWIP_DNS
	struct addrinfo *res = NULL;
	struct hostent *host_ent = NULL;
	struct hostent *user_ent = NULL;
#endif

	switch (req->type) {
#if LWIP_DNS
	case GETADDRINFO:
		req->req_res = lwip_getaddrinfo(req->host_name, req->serv_name, req->ai_hint, &res);
		if (req->req_res != 0) {
			ndbg("lwip_getaddrinfo() returned with the error code: %d\n", ret);
			req->ai_res = NULL;
			ret = -EINVAL;
		} else {
			req->ai_res = _netdev_copy_addrinfo(res);
			lwip_freeaddrinfo(res);
			ret = OK;
		}
		break;
	case FREEADDRINFO:
		req->req_res = _netdev_free_addrinfo(req->ai);
		ret = OK;
		break;
	case DNSSETSERVER:
		req->req_res = _netdev_set_dnsserver(req->addr, req->index);
		ret = OK;
		break;
	case GETHOSTBYNAME:
		host_ent = lwip_gethostbyname(req->host_name);
		if (!host_ent) {
			ndbg("lwip_gethostbyname() returned with the error code: %d\n", HOST_NOT_FOUND);
			ret = -EINVAL;
		} else {
			user_ent = req->host_entry;
			strncpy(user_ent->h_name, host_ent->h_name, DNS_MAX_NAME_LENGTH);
			user_ent->h_name[DNS_MAX_NAME_LENGTH] = 0;
			memcpy(user_ent->h_addr_list[0], host_ent->h_addr_list[0], sizeof(struct in_addr));
			user_ent->h_addrtype = host_ent->h_addrtype;
			user_ent->h_length = host_ent->h_length;

			ret	= OK;
		}
		break;
	case GETNAMEINFO:
		req->req_res = lwip_getnameinfo(req->sa, req->sa_len, (char *)req->host_name, req->host_len, (char *)req->serv_name, req->serv_len, req->flags);
		if (req->req_res != 0) {
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
		req->req_res = _netdev_dhcpc_start((const char *)req->intf);
		if (req->req_res != OK) {
			ndbg("Start DHCP clent failed\n");
			goto errout;
		}
		ret = OK;
		break;
	case DHCPCSTOP:
		req->req_res = _netdev_dhcpc_stop((const char *)req->intf);
		if (req->req_res != OK) {
			ndbg("Stop DHCP client failed\n");
			goto errout;
		}
		ret = OK;
		break;
#endif

#if defined(CONFIG_LWIP_DHCPS)
	case DHCPDSTART: {
		struct netif *cnif = _netdev_dhcp_dev((const char *)req->intf);
		if (cnif == NULL) {
			ndbg("No network interface for dhcpd start\n");
			req->req_res = ERROR;
			goto errout;
		}
		if (dhcps_register_cb(_netdev_dhcp_join_cb) != OK) {
			ndbg("Register link callback failed\n");
			req->req_res = ERROR;
			goto errout;
		}
		if ((req->req_res = netifapi_dhcps_start(cnif)) != OK) {
			ndbg("DHCP server start failed (LWIP)\n");
			goto errout;
		}
		ndbg("DHCP server started successfully (LWIP)\n");
		req->req_res = OK;
		ret = OK;
		break;
	}
	case DHCPDSTOP: {
		struct netif *cnif = _netdev_dhcp_dev((const char *)req->intf);
		if (cnif == NULL) {
			ndbg("No network interface for dhcpd stop\n");
			req->req_res = ERROR;
			goto errout;
		}
		if (cnif->dhcps_pcb == NULL) {
			ndbg("stop dhcpd fail: no pcb\n");
			req->req_res = ERROR;
			goto errout;
		}
		netifapi_dhcps_stop(cnif);
		ndbg("DHCP server stopped successfully (LWIP)\n");
		req->req_res = OK;
		ret = OK;
		break;
	}
	case DHCPDSTATUS: {
		struct netif *cnif = _netdev_dhcp_dev((const char *)req->intf);
		if (cnif == NULL) {
			ndbg("No network interface for dhcpd status\n");
			req->req_res = ERROR;
			goto errout;
		}
		if (cnif->dhcps_pcb == NULL) {
			ndbg("DHCP server closed\n");
			req->req_res = ERROR;
			goto errout;
		}
		nvdbg("DHCP server opened\n");
		req->req_res = OK;
		ret = OK;
		break;
	}
#endif // CONFIG_LWIP_DHCPS
#endif // CONFIG_NET_LWIP_DHCP
	case GETNETSTATS: {
		stats_display();
		netstats_display();
		ret = OK;
		break;
	}
	default:
		ndbg("Wrong request type: %d\n", req->type);
		break;
	}

	return ret;

errout:
	ret = -EINVAL;
	return ret;

}

/****************************************************************************
 * Function: netdev_lwipioctl
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
	ndbg("cmd %d\n", cmd);

	if (cmd == FIONREAD || cmd == FIONBIO) {
		ret = lwip_ioctl(sockfd, (long)cmd, arg);
		if (ret == -1) {
			return -get_errno();
		}
	} else if (cmd == SIOCLWIP) {
		return lwip_func_ioctl(sockfd, cmd, arg);
	}

	return ret;
}
