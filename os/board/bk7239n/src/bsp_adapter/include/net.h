#ifndef _BK_TIZENRT_NET_ADAPTER_H_
#define _BK_TIZENRT_NET_ADAPTER_H_

#ifdef CONFIG_QUICK_TRACK

#include <stdint.h>

#include "bk_netif_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

void uap_ip_down(void);
void uap_ip_start(void);
void sta_ip_mode_set(int dhcp);
uint32_t sta_ip_is_start(void);
uint32_t uap_ip_is_start(void);
void *net_get_uap_handle(void);
int net_get_if_macaddr(void *macaddr, void *intrfc_handle);
int net_wlan_remove_netif(uint8_t *mac);
void net_restart_dhcp(void);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_QUICK_TRACK */

#endif /* _BK_TIZENRT_NET_ADAPTER_H_ */
