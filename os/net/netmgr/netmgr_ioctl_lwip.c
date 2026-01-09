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
#include "netstack.h"
#include <tinyara/net/netlog.h>
#include <tinyara/netmgr/netctl.h>
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/netdb.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/igmp.h"
#include "lwip/netifapi.h"

#define TAG "[NETMGR]"

#if defined(CONFIG_NET_LWIP_DHCP)
static struct netif *_netdev_dhcp_dev(FAR const char *intf)
{
	struct netif *cnif;
	if (intf == NULL) {
		NET_LOGKE(TAG, "Input NULL\n");
		return NULL;
	}

	cnif = GET_NETIF_FROM_NETDEV(nm_get_netdev((uint8_t *)intf));
	if (cnif == NULL) {
		NET_LOGKE(TAG, "No netif which has the name of %s\n", intf);
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
			NET_LOGKE(TAG, "index go beyond the limits index(%d) max(%d)\n",
						index, CONFIG_NET_DNS_MAX_SERVERS);
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
	NET_LOGKE(TAG, "set dnsserver fail\n");
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
	NET_LOGKV(TAG, "LWIP DHCPC started (%s)\n", intf);
	struct netif *cnif;
	cnif = _netdev_dhcp_dev(intf);
	if (cnif == NULL) {
		NET_LOGKE(TAG, "No network interface for dhcpc %s\n", intf);
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
		NET_LOGKE(TAG, "DHCP client start failed %d\n", res);
		return ERROR;
	}

	NET_LOGKV(TAG, "DHCP client started successfully, waiting IP address (timeout %d msecs)\n", timeleft);
	while (netifapi_dhcp_address_valid(cnif) != 0) {
		usleep(100000);
		timeleft -= 100;
		if (timeleft <= 0) {
			NET_LOGKE(TAG, "DHCP client timeout\n");
			netifapi_dhcp_stop(cnif);
			return ERROR;
		}
	}

	memcpy(&ip_check, &ip4_addr_get_u32(ip_2_ip4(&cnif->ip_addr)), sizeof(struct in_addr));
	NET_LOGKV(TAG, "Got IP address %s\n", inet_ntoa(ip_check));

	return 0;
}

int _netdev_dhcpc_stop(const char *intf)
{
	struct in_addr in = {.s_addr = INADDR_NONE};
	struct netif *cnif;
	cnif = _netdev_dhcp_dev(intf);
	if (cnif == NULL) {
		NET_LOGKE(TAG, "No network interface for dhcpc %s\n", intf);
		return ERROR;
	}
	_netdev_set_ipv4addr(cnif, &in, &in, &in);
	netifapi_dhcp_stop(cnif);
	NET_LOGKV(TAG, "Release IP address (lwip)\n");

	return OK;
}

ap_type _netdev_dhcpc_get_aptype(void)
{
	return dhcp_get_aptype();
}

int _netdev_dhcpc_sethostname(struct lwip_dhcp_msg *msg)
{
#ifdef CONFIG_LWIP_DHCP_HOSTNAME
	/* it's blocked call. So it's ok to use local variable */
	struct lwip_dhcpc_msg dmsg;
	dmsg.netif = _netdev_dhcp_dev(msg->intf);
	if (dmsg.netif == NULL) {
		NET_LOGKE(TAG, "No network interface for dhcpc %s\n", msg->intf);
		return ERROR;
	}
	dmsg.hostname = msg->hostname;

	netifapi_dhcp_sethostname(dmsg.netif, (void *)&dmsg);

	return 0;
#else
	NET_LOGKE(TAG, "Enable LWIP_DHCP_HOSTNAME\n");
	return -1;
#endif
}
#endif

#if defined(CONFIG_LWIP_DHCPS)
#define DHCPD_MQ_NAME "dhcpd_queue"
#define DHCPD_MQ_LEN 11
#define DHCPD_MQ_MAX_LEN 20

void _netdev_dhcp_join_cb(dhcp_evt_type_e type, void *data)
{
	NET_LOGKI(TAG, "dhcpd joined\n");

	struct mq_attr attr;
	attr.mq_maxmsg = DHCPD_MQ_MAX_LEN;
	attr.mq_msgsize = DHCPD_MQ_LEN;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	mqd_t md = mq_open(DHCPD_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (!md) {
		NET_LOGKE(TAG, "mq open fail (errno %d)\n", errno);
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
		NET_LOGKE(TAG, "send mq fail (errno %d)\n", errno);
	}

	if (mq_close(md) != 0) {
		NET_LOGKE(TAG, "fail to close mq\n");
	}

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
			NET_LOGKE(TAG, "kumm_malloc failed\n");
			break;
		}
		dst->ai_flags = tmp->ai_flags;
		dst->ai_family = tmp->ai_family;
		dst->ai_socktype = tmp->ai_socktype;
		dst->ai_protocol = tmp->ai_protocol;
		dst->ai_addrlen = tmp->ai_addrlen;

#ifdef CONFIG_NET_IPv6
		/* For IPv6 support: use ai_addrlen to handle both IPv4 (16 bytes) and IPv6 (28 bytes) correctly */
		dst->ai_addr = (struct sockaddr *)kumm_malloc(tmp->ai_addrlen);
		if (!dst->ai_addr) {
			NET_LOGKE(TAG, "kumm_malloc failed\n");
			kumm_free(dst);
			break;
		}
		memcpy(dst->ai_addr, tmp->ai_addr, tmp->ai_addrlen);
#else
		dst->ai_addr = (struct sockaddr *)kumm_malloc(sizeof(struct sockaddr));
		if (!dst->ai_addr) {
			NET_LOGKE(TAG, "kumm_malloc failed\n");
			kumm_free(dst);
			break;
		}
		memcpy(dst->ai_addr, tmp->ai_addr, sizeof(struct sockaddr));
#endif
		if (tmp->ai_canonname) {
			dst->ai_canonname = (char *)kumm_malloc(strlen(tmp->ai_canonname) + 1);
			if (!dst->ai_canonname) {
				NET_LOGKE(TAG, "kumm_malloc failed\n");
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

	struct lwip_sock *sock = get_socket_by_pid(s, getpid());
	if (!sock) {
		NET_LOGKE(TAG, "get socket fail\n");
		ret = -EBADF;
		return ret;
	}
	struct req_lwip_data *req = (struct req_lwip_data *)arg;
	if (!req) {
		NET_LOGKE(TAG, "invalid parameter req\n");
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
		ret = OK; /*  req->req_res stores return value. So doesn't need to set ret */
		req->req_res = lwip_getaddrinfo(req->msg.netdb.host_name,
										req->msg.netdb.serv_name,
										req->msg.netdb.ai_hint, &res);
		if (req->req_res != 0) {
			NET_LOGKE(TAG, "lwip_getaddrinfo() returned with the error code: %d\n", req->req_res);
			req->msg.netdb.ai_res = NULL;
		} else {
			req->msg.netdb.ai_res = _netdev_copy_addrinfo(res);
			lwip_freeaddrinfo(res);
		}
		break;
	case FREEADDRINFO:
		req->req_res = _netdev_free_addrinfo(req->msg.netdb.ai_res);
		ret = OK;
		break;
	case DNSSETSERVER:
		req->req_res = _netdev_set_dnsserver(req->msg.dns.addr, req->msg.dns.index);
		ret = OK;
		break;
	case GETHOSTBYNAME:
		host_ent = lwip_gethostbyname(req->msg.netdb.host_name);
		if (!host_ent) {
			NET_LOGKE(TAG, "lwip_gethostbyname() returned with the error code: %d\n", HOST_NOT_FOUND);
			ret = -EINVAL;
		} else {
			user_ent = req->msg.netdb.host_entry;
			strncpy(user_ent->h_name, host_ent->h_name, DNS_MAX_NAME_LENGTH);
			user_ent->h_name[DNS_MAX_NAME_LENGTH] = 0;
			memcpy(user_ent->h_addr_list[0], host_ent->h_addr_list[0], sizeof(struct in_addr));
			user_ent->h_addrtype = host_ent->h_addrtype;
			user_ent->h_length = host_ent->h_length;

			ret	= OK;
		}
		break;
	case GETNAMEINFO:
		req->req_res = lwip_getnameinfo(req->msg.netdb.sa,
										req->msg.netdb.sa_len,
										(char *)req->msg.netdb.host_name,
										req->msg.netdb.host_len,
										(char *)req->msg.netdb.serv_name,
										req->msg.netdb.serv_len,
										req->msg.netdb.flags);
		if (req->req_res != 0) {
			NET_LOGKE(TAG, "lwip_getnameinfo() returned with the error code: %d\n", req->req_res);
			ret = -EINVAL;
		} else {
			ret = OK;
		}
		break;
#endif

#if defined(CONFIG_NET_LWIP_DHCP)
#if defined(CONFIG_LWIP_DHCPC)
	case DHCPCSTART:
		req->req_res = _netdev_dhcpc_start((const char *)req->msg.dhcp.intf);
		if (req->req_res != OK) {
			NET_LOGKE(TAG, "Start DHCP clent failed %d\n", req->req_res);
			goto errout;
		}
		ret = OK;
		break;
	case DHCPCSTOP:
		req->req_res = _netdev_dhcpc_stop((const char *)req->msg.dhcp.intf);
		if (req->req_res != OK) {
			NET_LOGKE(TAG, "Stop DHCP client failed %d\n", req->req_res);
			goto errout;
		}
		ret = OK;
		break;
	case DHCPCSETHOSTNAME:
		req->req_res = _netdev_dhcpc_sethostname(&req->msg.dhcp);
		if (req->req_res != OK) {
			NET_LOGKE(TAG, "Set dhcp host name failed %d\n", req->req_res);
			goto errout;
		}
		ret = OK;
		break;
	case DHCPCGETAPTYPE:
		req->req_res = (int)_netdev_dhcpc_get_aptype();
		ret = OK;
		break;
#endif

#if defined(CONFIG_LWIP_DHCPS)
	case DHCPDSTART: {
		struct netif *cnif = _netdev_dhcp_dev((const char *)req->msg.dhcp.intf);
		if (cnif == NULL) {
			NET_LOGKE(TAG, "No network interface for dhcpd start\n");
			req->req_res = ERROR;
			goto errout;
		}
		if (dhcps_register_cb(_netdev_dhcp_join_cb) != OK) {
			NET_LOGKE(TAG, "Register link callback failed\n");
			req->req_res = ERROR;
			goto errout;
		}
		if ((req->req_res = netifapi_dhcps_start(cnif)) != OK) {
			NET_LOGKE(TAG, "DHCP server start failed (LWIP)\n");
			goto errout;
		}
		NET_LOGKV(TAG, "DHCP server started successfully (LWIP)\n");
		req->req_res = OK;
		ret = OK;
		break;
	}
	case DHCPDSTOP: {
		struct netif *cnif = _netdev_dhcp_dev((const char *)req->msg.dhcp.intf);
		if (cnif == NULL) {
			NET_LOGKE(TAG, "No network interface for dhcpd stop\n");
			req->req_res = ERROR;
			goto errout;
		}
		if (cnif->dhcps_pcb == NULL) {
			NET_LOGKE(TAG, "stop dhcpd fail: no pcb\n");
			req->req_res = ERROR;
			goto errout;
		}
		netifapi_dhcps_stop(cnif);
		NET_LOGKV(TAG, "DHCP server stopped successfully (LWIP)\n");
		req->req_res = OK;
		ret = OK;
		break;
	}
	case DHCPDSTATUS: {
		struct netif *cnif = _netdev_dhcp_dev((const char *)req->msg.dhcp.intf);
		if (cnif == NULL) {
			NET_LOGKE(TAG, "No network interface for dhcpd status\n");
			req->req_res = ERROR;
			goto errout;
		}
		if (cnif->dhcps_pcb == NULL) {
			NET_LOGKE(TAG, "No running DHCP server\n");
			req->req_res = ERROR;
			goto errout;
		}
		NET_LOGKV(TAG, "DHCP server opened\n");
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
	case GETSOCKINFO: {
		struct lwip_netmon_msg *msg = &(((struct req_lwip_data *)arg)->msg.netmon);
		struct netstack *st = get_netstack(TR_SOCKET);
		if (!st) {
			return -1;
		}
		req->req_res = st->ops->getstats((void *)msg);
		ret = OK;
		break;
	}
	case GETDEVSTATS: {
		struct lwip_netmon_msg *msg = &(((struct req_lwip_data *)arg)->msg.netmon);
		struct netdev *dev = nm_get_netdev((uint8_t *)msg->ifname);
		if (!dev) {
			ret = -ENOTTY;
		} else {
			req->req_res = ((struct netdev_ops *)(dev->ops))->get_stats(dev, (void *)msg);
			ret = OK;
		}
		break;
	}
	default:
		NET_LOGKE(TAG, "Wrong request type: %d\n", req->type);
		ret = -ENOTTY;
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
	NET_LOGKV(TAG, "cmd %d\n", cmd);

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
