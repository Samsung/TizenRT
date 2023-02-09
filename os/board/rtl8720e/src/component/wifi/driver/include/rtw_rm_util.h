#ifndef _RTW_RM_UTIL_H_
#define _RTW_RM_UTIL_H_
/*
 * define the following channels as the max channels in each channel plan.
 * 2G, total 14 chnls
 * {1,2,3,4,5,6,7,8,9,10,11,12,13,14}
 * 5G, total 25 chnls
 * {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165}
 */
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifdef CONFIG_IEEE80211K
struct rate_section_ent {
	u8 tx_num; /* value of RF_TX_NUM */
	u8 rate_num;
	u8 *rates;
};

#define rate_section_to_tx_num(section) (rates_by_sections[(section)].tx_num)
#define rate_section_rate_num(section) (rates_by_sections[(section)].rate_num)
#endif

void rm_init_rtw_op_class(_adapter *padapter);
u8 rm_set_rtw_op_class(u32 country_code);
u8 rm_get_oper_class_via_ch(u8 ch);
u8 rm_get_ch_set(struct rtw_ieee80211_channel *pch_set, u8 op_class, u8 ch_num);
u8 rm_get_bcn_rsni(struct rm_obj *prm, struct wlan_network *pnetwork);
u8 rm_get_bcn_rcpi(struct rm_obj *prm, struct wlan_network *pnetwork);
u8 rm_get_frame_rsni(struct rm_obj *prm, union recv_frame *pframe);
u8 translate_percentage_to_rcpi(u32 SignalStrengthIndex);
u8 translate_dbm_to_rcpi(s8 SignalPower);
int is_wildcard_bssid(u8 *bssid);

int rm_get_path_a_max_tx_power(_adapter *adapter, s8 *path_a);
int rm_get_tx_power(PADAPTER adapter, enum rf_path path, u8 rate, s8 *pwr);
int rm_get_rx_sensitivity(PADAPTER adapter, enum channel_width bw, u8 rate, s8 *pwr);

#endif /* _RTW_RM_UTIL_H_ */
