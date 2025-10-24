#ifndef __BK_ETHERNETIF_H__
#define __BK_ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

#ifndef WLAN0_NAME
#define WLAN0_NAME "wlan0"
#endif
#ifndef WLAN1_NAME
#define WLAN1_NAME "wlan1"
#endif

/**
 * @ingroup ieee
 * A list of often ethtypes (although lwIP does not use all of them).
 */
enum lwip_ieee_eth_type {
  ETHTYPE_EAPOL     = 0x888EU,
};
void wlanif_recv(struct netif *netif, int total_len);
err_t wlanif_init(struct netif *netif);


#endif
