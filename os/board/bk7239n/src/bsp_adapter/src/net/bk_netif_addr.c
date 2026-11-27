#include <tinyara/config.h>
#include "bk_netif_addr.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <debug.h>
#include <lwip/netif.h>
#include <lwip/netifapi.h>
#include <lwip/dns.h>
#include <lwip/etharp.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "bk_wifi.h"
#include "bk_wifi_types.h"

static int g_sta_dhcp = DHCP_CLIENT;
static int g_uap_dhcp = DHCP_SERVER;

/* This runs in kernel space, so the netutils/dhcpc helpers (user space, they
 * work through socket ioctls) are not linkable here. Talk to lwIP directly.
 * netdev_lwip.c stores the owning struct netdev pointer immediately after
 * struct netif. Match by TizenRT's ifname instead of lwIP list order/number:
 * loopback consumes lo0 first, so wlan0 may appear as wl1 in lwIP logs. */
static struct netif *wifi_netif_by_role(int iface)
{
	struct netif *nif;
	const char *target_ifname = (iface == BK_SOFT_AP) ? "wlan1" : "wlan0";

	for (nif = netif_list; nif != NULL; nif = nif->next) {
		struct netdev *dev = *((struct netdev **)((char *)nif + sizeof(struct netif)));
		if (nif->name[0] != 'w' || nif->name[1] != 'l') {
			continue;
		}
		if (dev && strcmp(dev->ifname, target_ifname) == 0) {
			return nif;
		}
	}

	return NULL;
}

int sta_send_gratuitous_arp(void)
{
	struct netif *nif = wifi_netif_by_role(BK_STATION);
	err_t err;

	if (nif == NULL) {
		printf("[QT] gratuitous arp failed: no STA netif\n");
		return -1;
	}

	err = etharp_gratuitous(nif);
	if (err != ERR_OK) {
		printf("[QT] gratuitous arp failed: err=%d\n", err);
	}
	return err == ERR_OK ? 0 : -1;
}

int ip_address_set(int iface, int dhcp, char *ip, char *mask, char *gw, char *dns)
{
	struct netif *nif = wifi_netif_by_role(iface);
	ip4_addr_t addr, netmask, gateway;
	ip_addr_t dns_addr;
	char old_ip[16];
	char new_ip[16];
	char new_mask[16];
	char new_gw[16];
	u8_t was_up;
	err_t err;

	if (iface == BK_SOFT_AP) {
		g_uap_dhcp = dhcp;
	} else {
		g_sta_dhcp = dhcp;
	}

	if (dhcp == DHCP_CLIENT || ip == NULL || ip[0] == '\0') {
		if (nif) {
			netifapi_dhcp_start(nif);
		}
		return 0;
	}

	if (nif == NULL) {
		printf("[QT] static ip failed: no %s netif\n",
			   iface == BK_SOFT_AP ? "AP" : "STA");
		lldbg("No Wi-Fi netif to configure\n");
		return -1;
	}

	ip4_addr_set_zero(&addr);
	ip4_addr_set_zero(&netmask);
	ip4_addr_set_zero(&gateway);

	if (!ip4addr_aton(ip, &addr)) {
		printf("[QT] static ip failed: invalid ip %s\n", ip);
		lldbg("Invalid static IP: %s\n", ip);
		return -1;
	}
	if (mask && mask[0]) {
		if (!ip4addr_aton(mask, &netmask)) {
			printf("[QT] static ip failed: invalid mask %s\n", mask);
			lldbg("Invalid netmask: %s\n", mask);
			return -1;
		}
	}
	if (gw && gw[0]) {
		if (!ip4addr_aton(gw, &gateway)) {
			printf("[QT] static ip failed: invalid gateway %s\n", gw);
			lldbg("Invalid gateway: %s\n", gw);
			return -1;
		}
	}

	ip4addr_ntoa_r(netif_ip4_addr(nif), old_ip, sizeof(old_ip));
	lldbg("set %s static ip=%s mask=%s gw=%s dns=%s netif=%c%c%d old=%s\n",
		  iface == BK_SOFT_AP ? "AP" : "STA",
		  ip, mask ? mask : "", gw ? gw : "", dns ? dns : "",
		  nif->name[0], nif->name[1], nif->num, old_ip);

	netifapi_dhcp_stop(nif);
	was_up = netif_is_up(nif);
	if (was_up) {
		netifapi_netif_set_down(nif);
	}
	err = netifapi_netif_set_addr(nif, &addr, &netmask, &gateway);
	if (err != ERR_OK) {
		printf("[QT] static ip failed: netif_set_addr err=%d\n", err);
		lldbg("netif_set_addr failed: %d\n", err);
		return -1;
	}

	netifapi_netif_set_link_up(nif);
	netifapi_netif_set_up(nif);
	netifapi_netif_set_default(nif);
	if (iface == BK_STATION) {
		wifi_netif_notify_sta_got_ip(IP4);
	}
	if (iface == BK_STATION) {
		sta_send_gratuitous_arp();
	} else {
		err = etharp_gratuitous(nif);
		if (err != ERR_OK) {
			printf("[QT] static ip gratuitous arp failed: err=%d\n", err);
		}
	}

	if (dns && dns[0]) {
		ip_addr_set_zero(&dns_addr);
		if (ipaddr_aton(dns, &dns_addr)) {
			dns_setserver(0, &dns_addr);
		}
	}

	ip4addr_ntoa_r(netif_ip4_addr(nif), new_ip, sizeof(new_ip));
	ip4addr_ntoa_r(netif_ip4_netmask(nif), new_mask, sizeof(new_mask));
	ip4addr_ntoa_r(netif_ip4_gw(nif), new_gw, sizeof(new_gw));
	printf("[QT] static ip done: %s %c%c%d ip=%s mask=%s gw=%s\n",
		   iface == BK_SOFT_AP ? "AP" : "STA",
		   nif->name[0], nif->name[1], nif->num,
		   new_ip, new_mask, new_gw);

	return 0;
}

void sta_ip_down(void)
{
	struct netif *nif = wifi_netif_by_role(BK_STATION);

	if (nif) {
		netifapi_dhcp_stop(nif);
	}
}

void sta_ip_start(void)
{
	struct netif *nif = wifi_netif_by_role(BK_STATION);

	if (!nif) {
		return;
	}

	if (g_sta_dhcp == DHCP_CLIENT) {
		netifapi_dhcp_start(nif);
		return;
	}

	if (netif_ip4_addr(nif)->addr != 0) {
		netifapi_netif_set_link_up(nif);
		netifapi_netif_set_up(nif);
		netifapi_netif_set_default(nif);
		wifi_netif_notify_sta_got_ip(IP4);
		sta_send_gratuitous_arp();
	}
}

void uap_ip_down(void)
{
	struct netif *nif = wifi_netif_by_role(BK_SOFT_AP);

	if (nif) {
		netifapi_dhcp_stop(nif);
	}
}

void uap_ip_start(void)
{
	struct netif *nif = wifi_netif_by_role(BK_SOFT_AP);

	if (nif && g_uap_dhcp == DHCP_CLIENT) {
		netifapi_dhcp_start(nif);
	}
}

void sta_ip_mode_set(int dhcp)
{
	g_sta_dhcp = dhcp;
}

uint32_t sta_ip_is_start(void)
{
	struct netif *nif = wifi_netif_by_role(BK_STATION);

	return (nif && netif_is_up(nif) && netif_ip4_addr(nif)->addr != 0) ? 1 : 0;
}

uint32_t uap_ip_is_start(void)
{
	struct netif *nif = wifi_netif_by_role(BK_SOFT_AP);

	return (nif && netif_is_up(nif) && netif_ip4_addr(nif)->addr != 0) ? 1 : 0;
}

void *net_get_sta_handle(void)
{
	return wifi_netif_by_role(BK_STATION);
}

void *net_get_uap_handle(void)
{
	return wifi_netif_by_role(BK_SOFT_AP);
}

int net_get_if_macaddr(void *macaddr, void *intrfc_handle)
{
	struct netif *nif = (struct netif *)intrfc_handle;

	if (!macaddr) {
		return -1;
	}
	if (nif == NULL) {
		nif = wifi_netif_by_role(BK_STATION);
	}
	if (nif == NULL) {
		return -1;
	}

	memcpy(macaddr, nif->hwaddr, nif->hwaddr_len);
	return 0;
}

int net_wlan_remove_netif(uint8_t *mac)
{
	(void)mac;
	return 0;
}

void net_restart_dhcp(void)
{
	sta_ip_down();
	sta_ip_start();
}

int net_get_if_addr(struct wlan_ip_config *addr, void *intrfc_handle)
{
	struct netif *nif = (struct netif *)intrfc_handle;

	if (!addr) {
		return -1;
	}
	memset(addr, 0, sizeof(*addr));
	if (nif == NULL || nif == (void *)1) {
		nif = wifi_netif_by_role(BK_STATION);
	}
	if (nif == NULL) {
		return -1;
	}
	addr->ipv4.address = (unsigned)netif_ip4_addr(nif)->addr;
	return 0;
}

/* lwIP 2.0.2 has no SO_BINDTODEVICE (no per-pcb netif binding). The board has a
 * single wireless netif, so binding to its address gives the same routing. */
int net_bind_socket_to_iface(int sock, const char *ifname)
{
#ifdef SO_BINDTODEVICE
	return setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname));
#else
	struct sockaddr_in local;
	struct netif *nif = wifi_netif_by_role(BK_STATION);

	(void)ifname;
	if (nif == NULL || netif_ip4_addr(nif)->addr == 0) {
		/* No address yet: nothing to bind to, let lwIP pick the only netif. */
		return 0;
	}

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = netif_ip4_addr(nif)->addr;

	return bind(sock, (struct sockaddr *)&local, sizeof(local));
#endif
}

const char *wlan_ipv4_ntoa(struct wlan_ip_config *addr)
{
	static char buf[INET_ADDRSTRLEN];
	struct in_addr ip;

	if (!addr) {
		return "0.0.0.0";
	}
	ip.s_addr = addr->ipv4.address;
	if (!inet_ntop(AF_INET, &ip, buf, sizeof(buf))) {
		return "0.0.0.0";
	}
	return buf;
}
