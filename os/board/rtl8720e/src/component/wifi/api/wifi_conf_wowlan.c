//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if CONFIG_LWIP_LAYER
#include "lwipconf.h"
#include "lwip_netconf.h"
#endif
#endif
#include <platform_stdlib.h>
#include <wifi_conf.h>
#include <wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Variables Declarations
 ******************************************************/

#if !defined(CONFIG_MBED_ENABLED)
#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif
#endif

/******************************************************
 *               Variables Definitions
 ******************************************************/
/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN

static struct eth_addr *dst_eth_ret = NULL;
static struct eth_addr *dhcp_dst_eth_ret = NULL;
static int local_lan = 0;

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
#define IP_HDR_LEN   20
#define TCP_HDR_LEN  20
#define ETH_HDR_LEN  14
#define ETH_ALEN     6
static uint32_t _checksum32(uint32_t start_value, uint8_t *data, size_t len)
{
	uint32_t checksum32 = start_value;
	uint16_t data16 = 0;
	int i;

	for (i = 0; i < (len / 2 * 2); i += 2) {
		data16 = (data[i] << 8) | data[i + 1];
		checksum32 += data16;
	}

	if (len % 2) {
		data16 = data[len - 1] << 8;
		checksum32 += data16;
	}

	return checksum32;
}

static uint16_t _checksum32to16(uint32_t checksum32)
{
	uint16_t checksum16 = 0;

	checksum32 = (checksum32 >> 16) + (checksum32 & 0x0000ffff);
	checksum32 = (checksum32 >> 16) + (checksum32 & 0x0000ffff);
	checksum16 = (uint16_t) ~(checksum32 & 0xffff);

	return checksum16;
}
#endif

#ifdef CONFIG_WOWLAN_SSL_KEEP_ALIVE

static uint8_t ssl_offload = 0;
extern void rtw_hal_set_ssl_offload(uint8_t *ctr, uint8_t *iv, uint8_t *enc_key, uint8_t *dec_key, uint8_t *hmac_key, uint8_t *content, size_t len,
									uint8_t is_etm);

int wifi_set_ssl_offload(uint8_t *ctr, uint8_t *iv, uint8_t *enc_key, uint8_t *dec_key, uint8_t *hmac_key, uint8_t *content, size_t len, uint8_t is_etm)
{
	rtw_hal_set_ssl_offload(ctr, iv, enc_key, dec_key, hmac_key, content,  len,  is_etm);
	ssl_offload = 1;
	return 0;
}

extern void rtw_hal_set_publish_wakeup(void);
void wifi_set_publish_wakeup(void)
{
	rtw_hal_set_publish_wakeup();
}

extern void rtw_hal_set_tcp_mode(void);
void wifi_set_tcpssl_keepalive(void)
{
	rtw_hal_set_tcp_mode();
}

#endif

#ifdef CONFIG_ARP_KEEP_ALIVE
extern void rtw_set_arp_rsp_keep_alive(int enable, uint8_t *gw_ip);
int wifi_wowlan_set_arp_rsp_keep_alive(int enable)
{
	int ret = 0;
	uint8_t *gw_ip = NULL;

	gw_ip = LwIP_GetGW(0);

	rtw_set_arp_rsp_keep_alive(enable, gw_ip);

	return ret;
}
#endif

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
extern void rtw_set_keepalive_offload(uint8_t *eth_frame, uint32_t frame_len, uint32_t interval_ms, uint32_t resend_ms, uint8_t wake_sys);
int wifi_set_tcp_keep_alive_offload(int socket_fd, uint8_t *content, size_t len, uint32_t interval_ms, uint32_t resend_ms, uint8_t wake_sys)
{
	/* To avoid gcc warnings */
	(void) socket_fd;
	(void) content;
	(void) len;
	(void) interval_ms;

	struct sockaddr_in peer_addr, sock_addr;
	socklen_t peer_addr_len = sizeof(peer_addr);
	socklen_t sock_addr_len = sizeof(sock_addr);
	getpeername(socket_fd, (struct sockaddr *) &peer_addr, &peer_addr_len);
	getsockname(socket_fd, (struct sockaddr *) &sock_addr, &sock_addr_len);
	uint8_t *peer_ip = (uint8_t *) &peer_addr.sin_addr;
	uint16_t peer_port = ntohs(peer_addr.sin_port);
	uint8_t *sock_ip = (uint8_t *) &sock_addr.sin_addr;
	uint16_t sock_port = ntohs(sock_addr.sin_port);

	// ip header
	uint8_t ip_header[IP_HDR_LEN] = {0x45, 0x00, /*len*/ 0x00, 0x00 /*len*/, /*id*/ 0x00, 0x00 /*id*/, 0x00, 0x00, 0xff, /*protocol*/ 0x00 /*protocol*/,
									 /*chksum*/ 0x00, 0x00 /*chksum*/, /*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/
									};
	// len
	uint16_t ip_len = IP_HDR_LEN + TCP_HDR_LEN + len;
	ip_header[2] = (uint8_t)(ip_len >> 8);
	ip_header[3] = (uint8_t)(ip_len & 0xff);
	// id
	extern u16_t ip4_getipid(void);
	uint16_t ip_id = ip4_getipid();
	ip_header[4] = (uint8_t)(ip_id >> 8);
	ip_header[5] = (uint8_t)(ip_id & 0xff);
	// protocol
	ip_header[9] = 0x06;
	// src ip
	ip_header[12] = sock_ip[0];
	ip_header[13] = sock_ip[1];
	ip_header[14] = sock_ip[2];
	ip_header[15] = sock_ip[3];
	// dst ip
	ip_header[16] = peer_ip[0];
	ip_header[17] = peer_ip[1];
	ip_header[18] = peer_ip[2];
	ip_header[19] = peer_ip[3];
	// checksum
	uint32_t ip_checksum32 = 0;
	uint16_t ip_checksum16 = 0;
	ip_checksum32 = _checksum32(ip_checksum32, ip_header, sizeof(ip_header));
	ip_checksum16 = _checksum32to16(ip_checksum32);
	ip_header[10] = (uint8_t)(ip_checksum16 >> 8);
	ip_header[11] = (uint8_t)(ip_checksum16 & 0xff);

	// pseudo header
	uint8_t pseudo_header[12] = {/*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/,
										   0x00, /*protocol*/ 0x00 /*protocol*/, /*l4len*/ 0x00, 0x00 /*l4len*/
								};
	// src ip
	pseudo_header[0] = sock_ip[0];
	pseudo_header[1] = sock_ip[1];
	pseudo_header[2] = sock_ip[2];
	pseudo_header[3] = sock_ip[3];
	// dst ip
	pseudo_header[4] = peer_ip[0];
	pseudo_header[5] = peer_ip[1];
	pseudo_header[6] = peer_ip[2];
	pseudo_header[7] = peer_ip[3];
	// protocol
	pseudo_header[9] = 0x06;
	// layer 4 len
	uint16_t l4_len = TCP_HDR_LEN + len;
	pseudo_header[10] = (uint8_t)(l4_len >> 8);
	pseudo_header[11] = (uint8_t)(l4_len & 0xff);

	// tcp header
	uint8_t tcp_header[TCP_HDR_LEN] = {/*srcport*/ 0x00, 0x00 /*srcport*/, /*dstport*/ 0x00, 0x00 /*dstport*/, /*seqno*/ 0x00, 0x00, 0x00, 0x00 /*seqno*/,
												   /*ackno*/ 0x00, 0x00, 0x00, 0x00 /*ackno*/, 0x50, 0x18, /*window*/ 0x00, 0x00 /*window*/, /*checksum*/ 0x00, 0x00 /*checksum*/, 0x00, 0x00
									  };
	// src port
	tcp_header[0] = (uint8_t)(sock_port >> 8);
	tcp_header[1] = (uint8_t)(sock_port & 0xff);
	// dst port
	tcp_header[2] = (uint8_t)(peer_port >> 8);
	tcp_header[3] = (uint8_t)(peer_port & 0xff);

	// eth header
	uint8_t eth_header[ETH_HDR_LEN] = {/*dstaddr*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*dstaddr*/,
												   /*srcaddr*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*srcaddr*/, 0x08, 0x00
									  };

	ip4_addr_t *dst_ip, *dst_ip_ret = NULL;
	ip4_addr_t *dhcp_dst_ip, *dhcp_dst_ip_ret = NULL;
	uint8_t *mask = LwIP_GetMASK(0);
	uint8_t *temp_ip = LwIP_GetIP(0);
	dst_ip = (ip4_addr_t *) peer_ip;
	if (!ip4_addr_netcmp(dst_ip, (ip4_addr_t *)temp_ip, (ip4_addr_t *)mask)) {
		//outside local network
		dst_ip = (ip4_addr_t *) LwIP_GetGW(0);
	} else {
		local_lan = 1;
		dhcp_dst_ip = (ip4_addr_t *) LwIP_GetGW(0);
		// dhcp addr
		if (LwIP_etharp_find_addr(0, dhcp_dst_ip, &dhcp_dst_eth_ret, (const ip4_addr_t **)&dhcp_dst_ip_ret) >= 0) {
			memcpy(eth_header, dhcp_dst_eth_ret->addr, ETH_ALEN);
		} else {
			LwIP_etharp_request(0, dhcp_dst_ip);
			int retry_cnt = 0;
			vTaskDelay(100);
			while (LwIP_etharp_find_addr(0, dhcp_dst_ip, &dhcp_dst_eth_ret, (const ip4_addr_t **)&dhcp_dst_ip_ret) < 0) {
				LwIP_etharp_request(0, dhcp_dst_ip);
				vTaskDelay(100);
				retry_cnt++;
				if (retry_cnt > 500) {
					break;
				}
			}

			if (retry_cnt < 500) {
				memcpy(eth_header, dhcp_dst_eth_ret->addr, ETH_ALEN);
			}
		}
	}
	// dst addr
	if (LwIP_etharp_find_addr(0, dst_ip, &dst_eth_ret, (const ip4_addr_t **)&dst_ip_ret) >= 0) {
		memcpy(eth_header, dst_eth_ret->addr, ETH_ALEN);
	} else {
		LwIP_etharp_request(0, dst_ip);
		int retry_cnt = 0;
		vTaskDelay(100);
		while (LwIP_etharp_find_addr(0, dst_ip, &dst_eth_ret, (const ip4_addr_t **)&dst_ip_ret) < 0) {
			LwIP_etharp_request(0, dst_ip);
			vTaskDelay(100);
			retry_cnt++;
			if (retry_cnt > 500) {
				break;
			}
		}

		if (retry_cnt < 500) {
			memcpy(eth_header, dst_eth_ret->addr, ETH_ALEN);
		}
	}
	// src addr
	memcpy(eth_header + ETH_ALEN, LwIP_GetMAC(0), ETH_ALEN);

	uint32_t seqno = 0;
	uint32_t ackno = 0;
	uint16_t wnd = 0;
	extern int lwip_gettcpstatus(int s, uint32_t *seqno, uint32_t *ackno, uint16_t *wnd);
	lwip_gettcpstatus(socket_fd, &seqno, &ackno, &wnd);
	// seqno
	tcp_header[4] = (uint8_t)(seqno >> 24);
	tcp_header[5] = (uint8_t)((seqno & 0x00ff0000) >> 16);
	tcp_header[6] = (uint8_t)((seqno & 0x0000ff00) >> 8);
	tcp_header[7] = (uint8_t)(seqno & 0x000000ff);
	// ackno
	tcp_header[8] = (uint8_t)(ackno >> 24);
	tcp_header[9] = (uint8_t)((ackno & 0x00ff0000) >> 16);
	tcp_header[10] = (uint8_t)((ackno & 0x0000ff00) >> 8);
	tcp_header[11] = (uint8_t)(ackno & 0x000000ff);
	// window
	tcp_header[14] = (uint8_t)(wnd >> 8);
	tcp_header[15] = (uint8_t)(wnd & 0xff);
	// checksum
	uint32_t tcp_checksum32 = 0;
	uint16_t tcp_checksum16 = 0;
	tcp_checksum32 = _checksum32(tcp_checksum32, pseudo_header, sizeof(pseudo_header));
	tcp_checksum32 = _checksum32(tcp_checksum32, tcp_header, sizeof(tcp_header));
	tcp_checksum32 = _checksum32(tcp_checksum32, content, len);
	tcp_checksum16 = _checksum32to16(tcp_checksum32);
	tcp_header[16] = (uint8_t)(tcp_checksum16 >> 8);
	tcp_header[17] = (uint8_t)(tcp_checksum16 & 0xff);

	netifapi_netif_set_link_down(&xnetif[0]); // simulate system enter sleep

	// eth frame without FCS
	uint32_t frame_len = sizeof(eth_header) + sizeof(ip_header) + sizeof(tcp_header) + len;
	uint8_t *eth_frame = (uint8_t *) malloc(frame_len);
	memcpy(eth_frame, eth_header, sizeof(eth_header));
	memcpy(eth_frame + sizeof(eth_header), ip_header, sizeof(ip_header));
	memcpy(eth_frame + sizeof(eth_header) + sizeof(ip_header), tcp_header, sizeof(tcp_header));
	memcpy(eth_frame + sizeof(eth_header) + sizeof(ip_header) + sizeof(tcp_header), content, len);

#ifdef CONFIG_ARP_KEEP_ALIVE
	rtw_set_arp_rsp_keep_alive(RTW_TRUE, (uint8_t *)dst_ip);
#endif
	rtw_set_keepalive_offload(eth_frame, frame_len, interval_ms, resend_ms, wake_sys);

	free(eth_frame);

	if (wake_sys) {
		char buf[32], mac[6];

		const uint8_t *mac_temp = LwIP_GetMAC(0);

		//wifi_get_mac_address(buf);
		//sscanf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
		memcpy(mac, mac_temp, 6);

		printf("mac = 0x%2X,0x%2X,0x%2X,0x%2X,0x%2X,0x%2X \r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		//
		wowlan_pattern_t fin_pattern;
		//u8 fin_mask[6] = {0x3f, 0x70, 0x80, 0xc0, 0x0F, 0x80};
		u8 fin_mask[6] = {0x3f, 0x70, 0x80, 0xc0, 0x3F, 0x80};

		memset(&fin_pattern, 0, sizeof(wowlan_pattern_t));
		memcpy(fin_pattern.eth_da, mac, 6);
		fin_pattern.eth_proto_type[0] = 0x08;
		fin_pattern.eth_proto_type[1] = 0x00;
		fin_pattern.header_len[0] = 0x45;
		fin_pattern.ip_proto[0] = 0x06;
		memcpy(fin_pattern.ip_da, sock_ip, 4);
		//memcpy(fin_pattern.src_port, &peer_port, 2);
		fin_pattern.src_port[0] = (peer_port >> 8) & 0xFF;
		fin_pattern.src_port[1] = peer_port & 0xFF;
		fin_pattern.dest_port[0] = (sock_port >> 8) & 0xFF;
		fin_pattern.dest_port[1] = sock_port & 0xFF;
		fin_pattern.flag2[0] = 0x11; // FIN + ACK
		memcpy(fin_pattern.mask, fin_mask, 6);
		wifi_wowlan_set_pattern(fin_pattern);
	}

	return 0;
}

#endif

#ifdef CONFIG_WOWLAN_PARAM
extern void rtw_set_wowlan_param(u8  fwdis_period,
								 u8  fwdis_trypktnum,
								 u8  pno_enable,
								 u8  pno_timeout,
								 u8  l2_keepalive_period);
int wifi_wowlan_set_fwdecision_param(u8  fwdis_period,
									 u8  fwdis_trypktnum,
									 u8  pno_enable,
									 u8  pno_timeout,
									 u8  l2_keepalive_period)
{
	int ret = 0;
	rtw_set_wowlan_param(fwdis_period,
						 fwdis_trypktnum,
						 pno_enable,
						 pno_timeout,
						 l2_keepalive_period);

	return ret;
}
#endif

#if defined(CONFIG_WOWLAN_DTIMTO) || defined(CONFIG_SMART_DTIM)
extern void rtw_set_lps_dtim(uint8_t dtim);
extern int wifi_get_ap_dtim(u8 *dtim_period);
#endif

#ifdef CONFIG_WOWLAN_DTIMTO
extern void rtw_set_dtimto(uint8_t dtimto_enable, uint8_t retry_inc, uint8_t ack_timeout);
int wifi_wowlan_set_dtimto(uint8_t dtimto_enable, uint8_t retry_inc, uint8_t ack_timeout, uint8_t dtim)
{
	int ret = 0;

	if (dtim > 0) {
		printf("dtim: %d\r\n", dtim);
		rtw_set_lps_dtim(dtim);
	}

	rtw_set_dtimto(dtimto_enable, retry_inc, ack_timeout);

	return ret;
}
#endif

#ifdef CONFIG_SMART_DTIM
extern void rtw_set_smartdtim(uint8_t check_period, uint8_t threshold, uint8_t change_dtim);
int wifi_wowlan_set_smartdtim(uint8_t check_period, uint8_t threshold, uint8_t change_dtim, uint8_t dtim)
{
	int ret = 0;

	//check ap dtim
	uint8_t dtim_period = 0;
	wifi_get_ap_dtim(&dtim_period);

	if (dtim > 0) {
		printf("dtim: %d\r\n", dtim);
		uint8_t smartdtim = dtim_period;
		if (dtim > dtim_period) {
			smartdtim = (dtim / dtim_period) * dtim_period;
		}

		printf("smartdtim: %d\r\n", smartdtim);

		rtw_set_lps_dtim(dtim);
	}

	//check check_period& threshold
	if (threshold >=  check_period) {
		printf("warning: threshold >= check_period\r\n");
		threshold = check_period / 2;
	}

	//check change_dtim
	if (change_dtim >= dtim) {
		printf("warning: change_dtim >= dtim\r\n");
		change_dtim = 1;
	}

	rtw_set_smartdtim(check_period, threshold, change_dtim);

	return ret;
}
#endif

#ifdef CONFIG_ARP_REQUEST_KEEP_ALIVE
extern void rtw_set_arpreq_keepalive(u8  powerbit,
									 u8  dtim1to);

int wifi_wowlan_set_arpreq_keepalive(u8  powerbit,
									 u8  dtim1to)
{
	int ret = 0;
	rtw_set_arpreq_keepalive(powerbit, dtim1to);

	return ret;
}
#endif

#ifdef CONFIG_WOWLAN_IO_WDT
extern void rtw_set_wdt(u8  gpio,
						u8  interval);


int wifi_wowlan_set_wdt(u8  gpio,
						u8  interval)
{
	int ret = 0;
	rtw_set_wdt(gpio, interval);
	return ret;
}
#endif

#ifdef CONFIG_WOWLAN_DHCP_RENEW
extern void rtw_set_dhcp_offload(uint8_t *eth_frame, uint32_t frame_len, uint32_t lease_time, uint32_t t1_renew_time);

struct dhcprenew_msg {
	uint8_t op; 		/* Message op code/message type. 1 = BOOTREQUEST, 2 = BOOTREPLY */
	uint8_t	htype;		/* Hardware address type */
	uint8_t hlen;		/* Hardware address length */
	uint8_t hops;		/* Client sets to zero, optionally used by relay agents
				   when booting via a relay agent */
	uint8_t xid[4];		/* Transaction ID, a random number chosen by the client,
				   used by the client and server to associate messages and
				   responses between a client and a server */
	uint16_t secs;		/* Filled in by client, seconds elapsed since client began address
				   acquisition or renewal process.*/
	uint16_t flags;		/* bit 0: Broadcast flag, bit 1~15:MBZ must 0*/
	uint8_t ciaddr[4];	/* Client IP address; only filled in if client is in BOUND,
				   RENEW or REBINDING state and can respond to ARP requests. */
	uint8_t yiaddr[4];	/* 'your' (client) IP address */
	uint8_t siaddr[4];	/* IP address of next server to use in bootstrap;
				   returned in DHCPOFFER, DHCPACK by server. */
	uint8_t giaddr[4];	/* Relay agent IP address, used in booting via a relay agent.*/
	uint8_t chaddr[16];	/* Client hardware address */
	uint8_t sname[64];	/* Optional server host name, null terminated string.*/
	uint8_t file[128];	/* Boot file name, null terminated string; "generic" name or
			           null in DHCPDISCOVER, fully qualified directory-path name in DHCPOFFER.*/
	uint32_t cookie;
	uint8_t options[64];   /* Optional parameters field. reference the RFC 2132 */
};

#if CONFIG_LWIP_LAYER==0
/* REMOVE this, CONFIG_LWIP_LAYER cannot be turn off at this file */
#define PP_HTONL(x) ((((x) & 0x000000ffUL) << 24) | \
                     (((x) & 0x0000ff00UL) <<  8) | \
                     (((x) & 0x00ff0000UL) >>  8) | \
                     (((x) & 0xff000000UL) >> 24))
#endif

int wifi_set_dhcp_offload(void)
{
	int i = 0;
	uint8_t *gw_ip = NULL;
	uint8_t *dhcp_ip = NULL;
	gw_ip = LwIP_GetGW(0);
	dhcp_ip = LwIP_GetIP(0);
	uint8_t *dhcp_payload = NULL;
	int len = 0;
	uint32_t  seconds_elapsed = xTaskGetTickCount();
	uint32_t lease_time, t1_time, xid, xid_temp;
	/* dhcp msg */
	struct dhcprenew_msg *dhcprenew_msg = NULL;
	dhcprenew_msg = malloc(sizeof(struct dhcprenew_msg));
	memset(dhcprenew_msg, 0x0, sizeof(struct dhcprenew_msg));

	dhcprenew_msg->op = 1;
	dhcprenew_msg->htype = 1;
	dhcprenew_msg->hlen = 6;
	dhcprenew_msg->hops = 0;


	// dhcprenew_msg->xid[0] = 0x00;
	// dhcprenew_msg->xid[1] = 0x00;
	// dhcprenew_msg->xid[2] = 0xCD;
	// dhcprenew_msg->xid[3] = 0xAB;

	printf("LwIP_GetXID\r\n");
	xid = LwIP_GetXID(0);
	printf("xid = %d\r\n", xid);
	xid_temp = PP_HTONL(xid);
	memcpy(dhcprenew_msg->xid, &xid_temp, 4);
	dhcprenew_msg->secs = (uint16_t)((xTaskGetTickCount() - seconds_elapsed) / configTICK_RATE_HZ);			//Realtek add

	/* we don't need the broadcast flag since we can receive unicast traffic
	   before being fully configured! */
	dhcprenew_msg->flags = 0;
	dhcprenew_msg->ciaddr[0] = dhcp_ip[0];
	dhcprenew_msg->ciaddr[1] = dhcp_ip[1];
	dhcprenew_msg->ciaddr[2] = dhcp_ip[2];
	dhcprenew_msg->ciaddr[3] = dhcp_ip[3];
	printf("LwIP_GetMAC\r\n");
	memcpy(dhcprenew_msg->chaddr, LwIP_GetMAC(0), ETH_ALEN);

	for (i = 0; i < 64; i++) {
		dhcprenew_msg->sname[i] = 0;
	}
	for (i = 0; i < 128; i++) {
		dhcprenew_msg->file[i] = 0;
	}
	dhcprenew_msg->cookie = PP_HTONL(0x63825363UL);

	/* Add option MESSAGE_TYPE */
	dhcprenew_msg->options[0] = 0x35;
	dhcprenew_msg->options[1] = 0x1;
	dhcprenew_msg->options[2] = 0x3;
	dhcprenew_msg->options[3] = 0xFF;

	len = 244;
	dhcp_payload = (uint8_t *)dhcprenew_msg;

	// ip header
	uint8_t ip_header[IP_HDR_LEN] = {0x45, 0x00, /*len*/ 0x00, 0x00 /*len*/, /*id*/ 0x00, 0x00 /*id*/, 0x00, 0x00, 0xff, /*protocol*/ 0x00 /*protocol*/,
									 /*chksum*/ 0x00, 0x00 /*chksum*/, /*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/
									};
	// len
	uint16_t ip_len = IP_HDR_LEN + 8 /* UDP Header*/ + len;
	ip_header[2] = (uint8_t)(ip_len >> 8);
	ip_header[3] = (uint8_t)(ip_len & 0xff);
	// id
	extern u16_t ip4_getipid(void);
	uint16_t ip_id = ip4_getipid();
	ip_header[4] = (uint8_t)(ip_id >> 8);
	ip_header[5] = (uint8_t)(ip_id & 0xff);
	// protocol
	ip_header[9] = 0x11;
	// src ip
	ip_header[12] = dhcp_ip[0];
	ip_header[13] = dhcp_ip[1];
	ip_header[14] = dhcp_ip[2];
	ip_header[15] = dhcp_ip[3];
	// dst ip
	ip_header[16] = gw_ip[0];
	ip_header[17] = gw_ip[1];
	ip_header[18] = gw_ip[2];
	ip_header[19] = gw_ip[3];
	// checksum
	uint32_t ip_checksum32 = 0;
	uint16_t ip_checksum16 = 0;
	ip_checksum32 = _checksum32(ip_checksum32, ip_header, sizeof(ip_header));
	ip_checksum16 = _checksum32to16(ip_checksum32);
	ip_header[10] = (uint8_t)(ip_checksum16 >> 8);
	ip_header[11] = (uint8_t)(ip_checksum16 & 0xff);

	// pseudo header
#if 0
	uint8_t pseudo_header[12] = {/*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/,
										   0x00, /*protocol*/ 0x00 /*protocol*/, /*l4len*/ 0x00, 0x00 /*l4len*/
								};
	// src ip
	pseudo_header[0] = dhcp_ip[0];
	pseudo_header[1] = dhcp_ip[1];
	pseudo_header[2] = dhcp_ip[2];
	pseudo_header[3] = dhcp_ip[3];
	// dst ip
	pseudo_header[4] = gw_ip[0];
	pseudo_header[5] = gw_ip[1];
	pseudo_header[6] = gw_ip[2];
	pseudo_header[7] = gw_ip[3];
	// protocol
	pseudo_header[9] = 0x11;
	// layer 4 len
	uint16_t l4_len = 8 + len;
	pseudo_header[10] = (uint8_t)(l4_len >> 8);
	pseudo_header[11] = (uint8_t)(l4_len & 0xff);
#endif
	// udp header
	uint8_t udp_header[8] = {/*srcport*/ 0x00, 0x44 /*srcport*/, /*dstport*/ 0x00, 0x43 /*dstport*/, /*len*/ 0x00, 0x00 /*len*/, /*checksum*/ 0x00, 0x00 /*checksum*/};

	// len
	uint16_t udp_len = 8 /* UDP Header*/ + len;
	udp_header[4] = (uint8_t)(udp_len >> 8);
	udp_header[5] = (uint8_t)(udp_len & 0xff);


	// eth header
	uint8_t eth_header[ETH_HDR_LEN] = {/*dstaddr*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF /*dstaddr*/,
												   /*srcaddr*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*srcaddr*/, 0x08, 0x00
									  };

	ip4_addr_t *dst_ip, *dst_ip_ret = NULL;
	dst_ip = (ip4_addr_t *) gw_ip;

	// dst addr
	if (local_lan == 1) {
		memcpy(eth_header, dhcp_dst_eth_ret->addr, ETH_ALEN);
	} else {
		memcpy(eth_header, dst_eth_ret->addr, ETH_ALEN);
	}


	printf("LwIP_GetMAC2\r\n");
	// src addr
	memcpy(eth_header + ETH_ALEN, LwIP_GetMAC(0), ETH_ALEN);

	// eth frame without FCS
	uint32_t frame_len = sizeof(eth_header) + sizeof(ip_header) + 8 + len;
	uint8_t *eth_frame = (uint8_t *) malloc(frame_len);
	memcpy(eth_frame, eth_header, sizeof(eth_header));
	memcpy(eth_frame + sizeof(eth_header), ip_header, sizeof(ip_header));
	memcpy(eth_frame + sizeof(eth_header) + sizeof(ip_header), udp_header, 8);

	//dhcp header& body
	memcpy(eth_frame + sizeof(eth_header) + sizeof(ip_header) + 8, dhcp_payload, len);

	lease_time = LwIP_GetLEASETIME(0) / 60;
	t1_time = LwIP_GetRENEWTIME(0);

	if (lease_time >= 60) {
		rtw_set_dhcp_offload(eth_frame, frame_len, lease_time, t1_time);
	}
#if 0
	printf("wifi_set_dhcp_offload\r\n");

	printf("==== dhcp  page ====\r\n");
	for (int i = 0; i < len; i++) {
		printf("%02X,", dhcp_payload[i]);
		if ((i + 1) % 16 == 0) {
			printf("\r\n");
		}
	}
	printf("\r\n");
#endif

	free(eth_frame);

	return 0;
}

#endif

#ifdef CONFIG_WOWLAN
int wifi_wlan_redl_fw(void)
{
	int ret = 0;

	ret = rtw_wowlan_ctrl(STA_WLAN_INDEX, RTW_WOWLAN_REDOWNLOAD_FW, NULL);

	return ret;
}

int wifi_wowlan_ctrl(int enable)
{
	int ret = 0;
	u8 param = (u8)enable;

	ret = rtw_wowlan_ctrl(STA_WLAN_INDEX, RTW_WOWLAN_CTRL, &param);

	return ret;
}

#ifdef CONFIG_WOWLAN_SSL_KEEP_ALIVE
extern void rtw_hal_set_ssl_pattern(char *pattern, uint8_t len, uint16_t prefix_len);
void wifi_wowlan_set_ssl_pattern(char *pattern, uint8_t len, uint16_t prefix_len)
{
#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE_TEST
	extern void fw_set_ssl_pattern(char *pattern, uint8_t len, uint16_t prefix_len);
	fw_set_ssl_pattern(pattern, len, prefix_len);
#else
	rtw_hal_set_ssl_pattern(pattern, len, prefix_len);
#endif
}

#ifdef CONFIG_WOWLAN_SSL_SERVER_KEEP_ALIVE
extern void rtw_hal_set_ssl_serverkeepalive(uint16_t timeout, char *pattern, uint8_t len, uint16_t prefix_len);
void wifi_wowlan_set_serverkeepalive(uint16_t timeout, char *pattern, uint8_t len, uint16_t prefix_len)
{
	rtw_hal_set_ssl_serverkeepalive(timeout, pattern, len, prefix_len);
}
#endif

#endif

#ifdef CONFIG_WOWLAN_CUSTOM_PATTERN
int wifi_wowlan_set_pattern(wowlan_pattern_t pattern)
{
	int ret = 0;
	wowlan_pattern_t wowlan_pattern;

#if 1
	printf("eth_da: %02X %02X %02X %02X %02X %02X\r\n",
		   pattern.eth_da[0], pattern.eth_da[1], pattern.eth_da[2], pattern.eth_da[3], pattern.eth_da[4], pattern.eth_da[5]
		  );
	printf("eth_sa: %02X %02X %02X %02X %02X %02X\r\n",
		   pattern.eth_sa[0], pattern.eth_sa[1], pattern.eth_sa[2], pattern.eth_sa[3], pattern.eth_sa[4], pattern.eth_sa[5]
		  );
	printf("eth_proto_type: %02X %02X\r\n", pattern.eth_proto_type[0], pattern.eth_proto_type[1]);
	printf("header_len: %02X\r\n", pattern.header_len[0]);
	printf("ip_proto: %02X\r\n", pattern.ip_proto[0]);
	printf("ip_sa: %02X %02X %02X %02X\r\n", pattern.ip_sa[0], pattern.ip_sa[1], pattern.ip_sa[2], pattern.ip_sa[3]);
	printf("ip_da: %02X %02X %02X %02X\r\n", pattern.ip_da[0], pattern.ip_da[1], pattern.ip_da[2], pattern.ip_da[3]);
	printf("src_port: %02X %02X\r\n", pattern.src_port[0], pattern.src_port[1]);
	printf("dest_port: %02X %02X\r\n", pattern.dest_port[0], pattern.dest_port[1]);
	printf("flag2: %02X\r\n", pattern.flag2[0]);
	printf("mask: %02X %02X %02X %02X %02X %02X\r\n",
		   pattern.mask[0], pattern.mask[1], pattern.mask[2], pattern.mask[3], pattern.mask[4], pattern.mask[5]
		  );

	printf("payload_mask: %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
		   pattern.payload_mask[0], pattern.payload_mask[1], pattern.payload_mask[2], pattern.payload_mask[3], pattern.payload_mask[4], pattern.payload_mask[5],
		   pattern.payload_mask[6], pattern.payload_mask[7], pattern.payload_mask[8]
		  );

	int i;
	for (i = 0; i < 64; i++) {
		printf("%02X ", pattern.payload[i]);
	}

	printf("\r\n");

#endif

	rtw_memcpy(&wowlan_pattern, &pattern, sizeof(wowlan_pattern_t));
	ret = rtw_wowlan_ctrl(STA_WLAN_INDEX, RTW_WOWLAN_SET_PATTREN, &wowlan_pattern);

	return ret;
}
#endif
#endif

#endif	//#if CONFIG_WLAN
