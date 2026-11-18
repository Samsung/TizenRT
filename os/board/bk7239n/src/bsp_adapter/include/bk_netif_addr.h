#ifndef BK_NETIF_ADDR_H
#define BK_NETIF_ADDR_H

#ifdef CONFIG_QUICK_TRACK

#ifdef __cplusplus
extern "C" {
#endif

#ifndef QT_STA_IFNAME
#ifdef CONFIG_WIFIMGR_STA_IFNAME
#define QT_STA_IFNAME CONFIG_WIFIMGR_STA_IFNAME
#else
#define QT_STA_IFNAME "wlan0"
#endif
#endif

struct ipv4_config {
	unsigned address;
};

struct wlan_ip_config {
	struct ipv4_config ipv4;
};

int ip_address_set(int iface, int dhcp, char *ip, char *mask, char *gw, char *dns);
void sta_ip_down(void);
void sta_ip_start(void);
int sta_send_gratuitous_arp(void);
void *net_get_sta_handle(void);
int net_get_if_addr(struct wlan_ip_config *addr, void *intrfc_handle);
const char *wlan_ipv4_ntoa(struct wlan_ip_config *addr);
int net_bind_socket_to_iface(int sock, const char *ifname);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_QUICK_TRACK */

#endif
