/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTW_PWRCTRL_SD1_H_
#define __RTW_PWRCTRL_SD1_H_

#ifdef CONFIG_WOWLAN_SD1
#define DEFAULT_PATTERN_NUM 1  		/* ICMP */
#define MAX_CUSTOM_PATTERN_NUM 20
#define MAX_WKFM_CAM_NUM	22		/* Frame Mask Cam number for pattern match */
#define MAX_WKFM_SIZE		16 /* (16 bytes for WKFM bit mask, 16*8 = 128 bits) */
#define MAX_WKFM_PATTERN_SIZE	128
#define WKFMCAM_ADDR_NUM 8//6
//#define WKFMCAM_SIZE 		24 /* each entry need 6*4 bytes */

enum pattern_type {
	PATTERN_BROADCAST = 0,
	PATTERN_MULTICAST,
	PATTERN_UNICAST,
	PATTERN_VALID,
	PATTERN_INVALID,
};

typedef struct rtl_priv_pattern {
	int len;
	char content[MAX_WKFM_PATTERN_SIZE];
	char mask[MAX_WKFM_SIZE];
} rtl_priv_pattern_t;

#endif

struct pwrctrl_priv_sd1 {

#ifdef CONFIG_WOWLAN_SD1
	u8      wowlan_txpause_status;
	u8      bLowPwrRxBCN;
	u8      bLowPwrRxBCNOfdmOff;

#if defined(CONFIG_WOWLAN_HW_CAM)
	u8		wowlan_pattern;
	u8		wowlan_magic;
	u8		wowlan_unicast;
	bool	default_patterns_en;
	_mutex	wowlan_pattern_cam_mutex;
	u8		wowlan_pattern_idx;
	struct rtl_priv_pattern	patterns[MAX_WKFM_CAM_NUM];
#endif
#ifdef CONFIG_WOWLAN_CUSTOM_PATTERN
	wowlan_pattern_t	wowlan_custom_pattern[MAX_CUSTOM_PATTERN_NUM];
	u8		wowlan_custom_pattern_count;
#endif
#endif // CONFIG_WOWLAN

#ifdef CONFIG_LPS_PG
	u8 lpspg_rsvd_page_locate;
	u8 blpspg_info_up;
#endif

#ifdef CONFIG_WOWLAN_SSL_KEEP_ALIVE
	u8 ssl_info_rsvd_page_locate;
	u8 ssl_pattern_rsvd_page_locate;
#endif

#ifdef CONFIG_SMART_DTIM
	u8  smartdtim_enable;
	u8  smartdtim_resume_enable;
	u8  smartdtim_check_period;
	u8  smartdtim_threshold;
	u8  smartdtim_change_dtim;
#endif

#ifdef CONFIG_WOWLAN_DHCP_RENEW
	u8  dhcp_renew_rsvd_page_locate;
	u8 *dhcp_renew_pkt;
	u32 dhcp_renew_pkt_size;
	u16 dhcp_renew_lease_time;
	u16 dhcp_renew_t1;
#endif

#ifdef CONFIG_WOWLAN_DYNAMIC_TX_PWR
	u8  dynamic_tx_pwr_threshold_rssi_low;
	u8  dynamic_tx_pwr_threshold_rssi_high;
	u8  dynamic_tx_pwr_offset_db_low;
	u8  dynamic_tx_pwr_offset_db_high;
#endif

#ifdef CONFIG_ARP_KEEP_ALIVE
	u8  arp_rsp_keep_alive_enable;
	u8  gw_ip[4];
	u8  tcp_ip[4];
	u8  tcp_mac[6];
#endif

#ifdef CONFIG_WOWLAN_PARAM
	u8  wowlan_param_conf;
	u8  fw_disconnect_check_period;
	u8  fw_disconnect_trypktnum;
	u8  pno_enable;
	u8  pno_timeout;
	u8  l2_keepalive_period;
#endif

#ifdef CONFIG_ARP_REQUEST_KEEP_ALIVE
	u8  arpreq_enable;
	u8  arpreq_powerbit;
	u8  arpreq_dtim1to;
	u8  arpreq_rsvd_page_locate;
#endif

#ifdef CONFIG_WOWLAN_IO_WDT
	u8  wdt_enable;
	u8  wdt_gpio;
	u8  wdt_interval;
#endif

#ifdef CONFIG_WOWLAN_SSL_SERVER_KEEP_ALIVE
	u8  serverkeepalive_enable;
	u8  serverkeepalive_timeout_l;
	u8  serverkeepalive_timeout_h;
#endif

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
	u8  tcp_keep_alive_rsvd_page_locate;
	u32 tcp_keep_alive_interval_ms;
	u32 tcp_keep_alive_resend_ms;
	u8  tcp_keep_alive_wake_sys;
	u8 *tcp_keep_alive_pkt;
	u32 tcp_keep_alive_pkt_size;
#ifdef CONFIG_WOWLAN_DTIMTO
	u8  tcp_keep_alive_dtimto_enable;
	u8  tcp_keep_alive_dtimto_retry_inc;
	u8  tcp_keep_alive_dtimto_ack_timeout;
#endif
#endif

};

#endif
