/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
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

#ifndef _RTW_PROMISC_H_
#define _RTW_PROMISC_H_
#include <drv_types.h>
#ifdef CONFIG_PROMISC
void promisc_deinit(_adapter *padapter);
//void promisc_set_enable(_adapter *padapter, u8 enabled, u8 len_used);
int promisc_recv_func(_adapter *padapter, union recv_frame *rframe);
int promisc_recv_lens_func(void *padapter, u8 *payload, u8 plen);
int promisc_filter_with_len(u16 len);
void promisc_init_packet_filter(void);
int promisc_add_packet_filter(u8 filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule);
int promisc_enable_packet_filter(u8 filter_id);
int promisc_disable_packet_filter(u8 filter_id);
int promisc_remove_packet_filter(u8 filter_id);
int promisc_filter_retransmit_pkt(u8 enable, u8 filter_interval_ms);
int promisc_ctrl_packet_rpt(u8 enable);
void promisc_filter_by_ap_and_phone_mac(u8 enable, void *ap_mac, void *phone_mac);
#endif
int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
unsigned char is_promisc_enabled(void);
int promisc_get_fixed_channel(void *fixed_bssid, u8 *ssid, int *ssid_length);
void promisc_issue_probereq(void);
void promisc_issue_probersp(unsigned char *da);
void promisc_stop_tx_beacn(void);
void promisc_resume_tx_beacn(void);
void promisc_get_ap_info(rtw_result_t (*func)(char *ssid, u8 ssid_len, s16 rssi, char channel, char security));
void _promisc_deinit(_adapter *padapter);
int _promisc_recv_func(_adapter *padapter, union recv_frame *rframe);
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
int _promisc_recv_lens_func(_adapter *padapter, u8 *payload, u8 plen);
#endif

#endif	//_RTW_PROMISC_H_

