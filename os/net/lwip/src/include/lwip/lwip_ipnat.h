#ifndef __LWIP_NAT_H__
#define __LWIP_NAT_H__

#include "lwip/opt.h"

#ifdef __cplusplus
extern "C" {
#endif

#if IP_FORWARD
#if defined(CONFIG_ENABLE_HOMELYNK) && (CONFIG_ENABLE_HOMELYNK == 1)

/* Default size of the tables used for NAT */
/* TODO : Need to make this size configurable with menu config? */
#define IP_NAT_MAX 256


/* Timeouts in sec for the various protocol types */
#define IP_NAT_MAX_TIMEOUT_MS_TCP (8*60*1000)
#define IP_NAT_MAX_TIMEOUT_MS_TCP_FIN_WAIT (5*60*1000)
//#define IP_NAT_MAX_TIMEOUT_MS_TCP_FIN_WAIT (60*1000)
#define IP_NAT_MAX_TIMEOUT_MS_FILTER_DROP (60*1000)
#define IP_NAT_MAX_TIMEOUT_MS_TCP_DISCON (12*1000)
#define IP_NAT_MAX_TIMEOUT_MS_TCP_RST_DISCON (6*1000)
#define IP_NAT_MAX_TIMEOUT_MS_UDP (8*1000)
#define IP_NAT_MAX_TIMEOUT_MS_UDP_ALG (20*1000)
#define IP_NAT_MAX_TIMEOUT_MS_ICMP (2*1000)

void ip_nat_initialize(void);
err_t ip_nat_enqueue(struct pbuf *p, struct netif *inp);
err_t ip_nat_transfer(struct pbuf *p, struct netif *src, struct netif *target);
#endif /* CONFIG_ENABLE_HOMELYNK */
#endif /* IP_FORWARD */
#ifdef __cplusplus
}
#endif

#endif /* __LWIP_NAT_H__ */
