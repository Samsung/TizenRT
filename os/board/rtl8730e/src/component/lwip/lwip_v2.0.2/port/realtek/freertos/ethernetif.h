#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

//----- ------------------------------------------------------------------
// Ethernet Buffer
//----- ------------------------------------------------------------------
struct eth_drv_sg {
    unsigned int	buf;
    unsigned int 	len;
};

#define MAX_ETH_DRV_SG	32
#define MAX_ETH_MSG	1540

void ethernetif_recv(struct netif *netif, int total_len);
err_t ethernetif_init(struct netif *netif);

#endif 
