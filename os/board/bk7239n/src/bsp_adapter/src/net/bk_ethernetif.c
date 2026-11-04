/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "common/bk_include.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#ifdef CONFIG_NET_IPv6
#include <lwip/ethip6.h>
#endif

#include <bk_ethernetif.h>

#include <stdio.h>
#include <string.h>

#include "lwip/netif/etharp.h"

//#include "lwip_netif_address.h"

#include "bk_drv_model.h"
#include <os/mem.h>

#include <os/os.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include <sys/socket.h>
#include <netdev_mgr_internal.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

#include "bk_uart.h"
#include "bk_wifi.h"
#include "bk_private/bk_wifi.h"
#include "bk_lwipif_tizenrt.h"
//TODO should use registered callback here!!!
extern int ke_l2_packet_tx(unsigned char *buf, int len, int flag);
extern int bmsg_tx_sender(struct pbuf *p, uint32_t vif_idx);
#if CONFIG_WIFI6_CODE_STACK
extern int bmsg_special_tx_sender(struct pbuf *p, uint32_t vif_idx);
#endif
/* Forward declarations. */
void ethernetif_input(int iface, struct pbuf *p);

#ifndef GET_NETIF_FROM_NETDEV
#define GET_NETIF_FROM_NETDEV(dev) (struct netif *)(((struct netdev_ops *)(dev)->ops)->nic)
#endif

#ifdef CONFIG_NET_NETMGR
extern struct netdev *armino_dev_wlan0;
#ifdef CONFIG_BK_CONCURRENT_MODE
extern struct netdev *armino_dev_wlan1;
#else
volatile int g_bk_wifi_current_vif = 0;
#endif
#ifdef CONFIG_QUICK_TRACK
extern volatile int g_qt_loopback_fast_echo_enabled;
extern volatile unsigned short g_qt_loopback_fast_echo_port;
extern volatile unsigned char *g_qt_loopback_fast_echo_buf;
extern volatile unsigned short g_qt_loopback_fast_echo_buf_len;
#endif

int get_idx_from_dev(struct netdev *dev)
{
#ifdef CONFIG_BK_CONCURRENT_MODE
	if (!strcmp(WLAN0_NAME, dev->ifname))
		return 0;
	else if(!strcmp(WLAN1_NAME, dev->ifname))
		return 1;
	else
		return -1;
#else
#ifdef CONFIG_QUICK_TRACK
	if (dev && !strcmp("wlan1", dev->ifname))
		return 1;
	if (dev && !strcmp("wlan0", dev->ifname))
		return 0;
#else
	(void)dev;
#endif
	return g_bk_wifi_current_vif;
#endif
}

static struct netdev *bk_get_netdev(int idx)
{
#ifdef CONFIG_BK_CONCURRENT_MODE
	if (idx == 1)
	    return armino_dev_wlan1;
	else
	    return armino_dev_wlan0;
#else
	(void)idx;
	return armino_dev_wlan0;
#endif
}
#endif

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */

const char wlan_name[][6] =
{
    "wlan0\0",
    "wlan1\0",
    "wlan2\0",
    "wlan3\0",
};

//#include "net.h"
static void low_level_init(struct netif *netif)
{
#if !CONFIG_FIXED_NETIF
    void *vif = netif->state;
    u8 *macptr = wifi_netif_vif_to_mac(vif);
    int vif_index = wifi_netif_vif_to_vifid(vif);
#else
    u8 macptr[6] = {0};

    if (netif == (struct netif *)net_get_sta_handle())
           bk_wifi_sta_get_mac(macptr);
    else if (netif == (struct netif *)net_get_uap_handle())
           bk_wifi_ap_get_mac(macptr);
    else
           return;
#endif
#if LWIP_NETIF_HOSTNAME
#if !CONFIG_FIXED_NETIF
    /* Initialize interface hostname */
    netif->hostname = (char*)&wlan_name[vif_index];
#endif
#endif /* LWIP_NETIF_HOSTNAME */

    /* set MAC hardware address length */
    LWIP_LOGD("enter low level!\r\n");
    LWIP_LOGI("mac %2x:%2x:%2x:%2x:%2x:%2x\r\n", macptr[0], macptr[1], macptr[2],
               macptr[3], macptr[4], macptr[5]);

    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    os_memcpy(netif->hwaddr, macptr, ETHARP_HWADDR_LEN);
    /* maximum transfer unit */
    netif->mtu = 1500;
    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
#if !CONFIG_BRIDGE
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
#else
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_LINK_UP;
#endif
#ifdef LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
#endif
#if defined (LWIP_IPV6_MLD) && (LWIP_IPV6_MLD == 1)
    netif->flags |= NETIF_FLAG_MLD6;
#endif
    LWIP_LOGD("leave low level!\r\n");
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
#ifdef CONFIG_NET_NETMGR
err_t low_level_output(struct netdev *dev, uint8_t *data, uint16_t dlen)
{
    int ret;
    err_t err = ERR_OK;
    uint8_t vif_idx = 0;

    vif_idx = get_idx_from_dev(dev);
    ret = bk_wlan_txdata_send(data, dlen, vif_idx);
#ifdef CONFIG_QUICK_TRACK
    if (ret != ERR_OK) {
        err = ERR_TIMEOUT;
    }
#endif

    return err;
}
#else
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    int ret;
    err_t err = ERR_OK;
    uint8_t vif_idx = wifi_netif_vif_to_vifid(netif->state);

    // Sanity check
    if (vif_idx == 0xff)
        return ERR_ARG;

#if CONFIG_WIFI6_CODE_STACK
    //LWIP_LOGI("output:%x\r\n", p);
    extern bool special_arp_flag;
    if(special_arp_flag)
    {
        ret = bmsg_special_tx_sender(p, (uint32_t)vif_idx);
        special_arp_flag = false;
    }
    else
#endif
    {
        ret = bmsg_tx_sender(p, (uint32_t)vif_idx);
    }
    if(0 != ret)
    {
        err = ERR_TIMEOUT;
    }

    return err;
}
#endif

static inline int is_broacast_mac_addr(const u8 *a)
{
    return (a[0] & a[1] & a[2] & a[3] & a[4] & a[5]) == 0xff;
}

#if defined(CONFIG_NET_NETMGR) && defined(CONFIG_QUICK_TRACK)
static void qt_fast_arp_reply(struct netdev *dev, struct netif *netif, const uint8_t *req, uint16_t len)
{
    const uint8_t *arp;
    const ip4_addr_t *own_ip;
    uint8_t reply[42];
    err_t err;

    if (!g_qt_loopback_fast_echo_enabled) {
        return;
    }
    if (!dev || !netif || !req || len < sizeof(reply)) {
        return;
    }
    if (req[12] != 0x08 || req[13] != 0x06) {
        return;
    }

    arp = req + 14;
    if (arp[6] != 0x00 || arp[7] != 0x01) {
        return;
    }

    own_ip = netif_ip4_addr(netif);
    if (arp[24] != ip4_addr1(own_ip) || arp[25] != ip4_addr2(own_ip) ||
        arp[26] != ip4_addr3(own_ip) || arp[27] != ip4_addr4(own_ip)) {
        return;
    }
    if (arp[14] == ip4_addr1(own_ip) && arp[15] == ip4_addr2(own_ip) &&
        arp[16] == ip4_addr3(own_ip) && arp[17] == ip4_addr4(own_ip)) {
        return;
    }

    memcpy(reply, req, sizeof(reply));
    memcpy(reply, req + 6, 6);
    memcpy(reply + 6, netif->hwaddr, 6);

    reply[20] = 0x00;
    reply[21] = 0x02;
    memcpy(reply + 22, netif->hwaddr, 6);
    reply[28] = ip4_addr1(own_ip);
    reply[29] = ip4_addr2(own_ip);
    reply[30] = ip4_addr3(own_ip);
    reply[31] = ip4_addr4(own_ip);
    memcpy(reply + 32, req + 22, 6);
    memcpy(reply + 38, req + 28, 4);

#ifdef CONFIG_NET_NETMGR_ZEROCOPY
    {
        struct pbuf out;

        memset(&out, 0, sizeof(out));
        out.payload = reply;
        out.len = sizeof(reply);
        out.tot_len = sizeof(reply);
        err = low_level_output(dev, (uint8_t *)&out, 0);
    }
#else
    err = low_level_output(dev, reply, sizeof(reply));
#endif
    (void)err;
}

static uint16_t qt_ipv4_checksum(const uint8_t *data, uint8_t len)
{
    uint32_t sum = 0;
    uint8_t i;

    for (i = 0; i + 1 < len; i += 2) {
        sum += ((uint16_t)data[i] << 8) | data[i + 1];
    }
    if (len & 1) {
        sum += (uint16_t)data[len - 1] << 8;
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (uint16_t)~sum;
}

static int qt_fast_udp_echo(struct netdev *dev, struct netif *netif, const uint8_t *req, uint16_t len)
{
    const ip4_addr_t *own_ip;
    const uint8_t *ip;
    const uint8_t *udp;
    uint8_t *reply = (uint8_t *)g_qt_loopback_fast_echo_buf;
    uint16_t total_len;
    uint16_t frame_len;
    uint16_t sport;
    uint16_t dport;
    uint16_t csum;
    uint8_t ihl;
    err_t err;

    if (!g_qt_loopback_fast_echo_enabled || !g_qt_loopback_fast_echo_port) {
        return 0;
    }
    if (!reply || !g_qt_loopback_fast_echo_buf_len) {
        return 0;
    }
    if (!dev || !netif || !req || len < 42) {
        return 0;
    }
    if (req[12] != 0x08 || req[13] != 0x00) {
        return 0;
    }

    ip = req + 14;
    if ((ip[0] >> 4) != 4 || ip[9] != 17) {
        return 0;
    }

    ihl = (ip[0] & 0x0f) * 4;
    if (ihl < 20 || len < 14 + ihl + 8) {
        return 0;
    }

    own_ip = netif_ip4_addr(netif);
    if (ip[16] != ip4_addr1(own_ip) || ip[17] != ip4_addr2(own_ip) ||
        ip[18] != ip4_addr3(own_ip) || ip[19] != ip4_addr4(own_ip)) {
        return 0;
    }

    udp = ip + ihl;
    sport = ((uint16_t)udp[0] << 8) | udp[1];
    dport = ((uint16_t)udp[2] << 8) | udp[3];
    if (dport != g_qt_loopback_fast_echo_port) {
        return 0;
    }

    total_len = ((uint16_t)ip[2] << 8) | ip[3];
    frame_len = total_len + 14;
    if (frame_len > len || frame_len > g_qt_loopback_fast_echo_buf_len) {
        return 0;
    }

    memcpy(reply, req, frame_len);
    memcpy(reply, req + 6, 6);
    memcpy(reply + 6, netif->hwaddr, 6);

    reply[26] = ip4_addr1(own_ip);
    reply[27] = ip4_addr2(own_ip);
    reply[28] = ip4_addr3(own_ip);
    reply[29] = ip4_addr4(own_ip);
    reply[30] = ip[12];
    reply[31] = ip[13];
    reply[32] = ip[14];
    reply[33] = ip[15];

    reply[14 + ihl] = (uint8_t)(dport >> 8);
    reply[15 + ihl] = (uint8_t)dport;
    reply[16 + ihl] = (uint8_t)(sport >> 8);
    reply[17 + ihl] = (uint8_t)sport;
    reply[24] = 0;
    reply[25] = 0;
    csum = qt_ipv4_checksum(reply + 14, ihl);
    reply[24] = (uint8_t)(csum >> 8);
    reply[25] = (uint8_t)csum;

    reply[20 + ihl] = 0;
    reply[21 + ihl] = 0;

#ifdef CONFIG_NET_NETMGR_ZEROCOPY
    {
        struct pbuf out;

        memset(&out, 0, sizeof(out));
        out.payload = reply;
        out.len = frame_len;
        out.tot_len = frame_len;
        err = low_level_output(dev, (uint8_t *)&out, 0);
    }
#else
    err = low_level_output(dev, reply, frame_len);
#endif
    if (err != ERR_OK) {
        return 0;
    }

    return 1;
}
#endif

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
ethernetif_input(int iface, struct pbuf *p)
{
    struct eth_hdr *ethhdr;
    struct netif *netif;
    struct netdev *dev_tmp = NULL;

    if (p->len <= SIZEOF_ETH_HDR) {
        pbuf_free(p);
        return;
    }
    dev_tmp = (struct netdev *)bk_get_netdev(iface);
    if(!dev_tmp) {
        pbuf_free(p);
        return;
    }
    netif = GET_NETIF_FROM_NETDEV(dev_tmp);
    if(!netif) {
        //LWIP_LOGI("ethernetif_input no netif found %d\r\n", iface);
        pbuf_free(p);
        p = NULL;
        return;
    }

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;
#if defined(CONFIG_NET_NETMGR) && defined(CONFIG_QUICK_TRACK)
    qt_fast_arp_reply(dev_tmp, netif, (const uint8_t *)p->payload, p->len);
    if (qt_fast_udp_echo(dev_tmp, netif, (const uint8_t *)p->payload, p->len)) {
        pbuf_free(p);
        return;
    }
#endif

    if( (memcmp(netif->hwaddr,ethhdr->src.addr,NETIF_MAX_HWADDR_LEN)==0) && (htons(ethhdr->type) !=ETHTYPE_ARP) )
    {
        LWIP_DEBUGF(ETHARP_DEBUG ,("ethernet_input frame is my send,drop it\r\n"));
        pbuf_free(p);
        return;
    }

    switch (htons(ethhdr->type))
    {
        /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#ifdef CONFIG_NET_IPv6
    case ETHTYPE_IPV6:
	wlan_set_multicast_flag();
#endif
#if PPPOE_SUPPORT
        /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        if (netif->input(p, netif) != ERR_OK)    // ethernet_input
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\r\n"));
            pbuf_free(p);
            p = NULL;
        }
        break;

    case ETHTYPE_EAPOL:
        ke_l2_packet_tx(p->payload, p->len, iface);
        pbuf_free(p);
        p = NULL;
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }

}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
wlanif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));
    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 10000000);

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = (netif_linkoutput_fn)low_level_output;
#ifdef CONFIG_NET_IPv6
    netif->output_ip6 = ethip6_output;
#endif

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netifapi_netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t lwip_netif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 10000000);

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = (netif_linkoutput_fn)low_level_output;
#ifdef CONFIG_NET_IPv6
    netif->output_ip6 = ethip6_output;
#endif

    /* initialize the hardware */
    low_level_init(netif);
    return ERR_OK;
}

err_t lwip_netif_uap_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

    //netif->state = NULL;
    netif->name[0] = 'u';
    netif->name[1] = 'a';
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = (netif_linkoutput_fn)low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

// eof
