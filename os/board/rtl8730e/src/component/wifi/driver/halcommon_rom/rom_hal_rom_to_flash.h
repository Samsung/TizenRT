#ifndef __ROM_HAL_ROM_TO_FLASH_H__
#define __ROM_HAL_ROM_TO_FLASH_H__

#include "os_wrapper.h"
#include "rtw_skbuff.h"
#include "rtw_timer.h"
#include "rtw_wifi_constants.h"

union recv_frame;
struct xmit_frame;
struct cmd_obj;
struct security_priv;
struct wlan_network;
struct txdesc_priv;
struct pkt_attrib;
struct rtw_network_info;
struct rtw_ssid;

struct _wifi_rom_to_flash_func_map {
	/* OS interface */
	int (*rtw_rtos_mutex_take_t)(rtos_mutex_t p_handle, uint32_t wait_ms);
	int (*rtw_rtos_mutex_give_t)(rtos_mutex_t p_handle);
	uint32_t (*rtos_time_get_current_system_time_ms)(void);

	/* sema */
	int (*rtos_sema_give)(rtos_sema_t p_handle);
	int (*rtos_sema_take)(rtos_sema_t p_handle, uint32_t wait_ms);

	void (*rtw_rtos_critical_enter_t)(void);
	void (*rtw_rtos_critical_exit_t)(void);

	/* skb related */
	void (*rtw_dev_kfree_skb_any_t)(struct sk_buff *skb);
	struct sk_buff *(*rtw_dev_alloc_skb_t)(unsigned int length, unsigned int reserve_len);

	/* reorder */
	void (*rtos_time_delay_ms_t)(uint32_t ms);
	int (*rtos_mutex_delete_static_t)(rtos_mutex_t p_handle);
	int (*rtos_mutex_create_static_t)(rtos_mutex_t *pp_handle);

	/* wakelock */
	void (*rtw_acquire_wakelock_t)(void);
	void (*rtw_release_wakelock_t)(void);

	/* below for driver core only */
	void (*wifi_hal_normal_rx_process_t)(union recv_frame *precvframe);

	bool(*wifi_get_country_code_info)(u8 *country_code, u8 *channel_plan, u8 *power_limit); //data type of country_code in rom is char *

	/* adapter */
	u8 *(*rtw_get_adapter_mac_addr)(u8 iface_type);
	u8 *(*rtw_get_adapter_bssid)(u8 iface_type);
	struct mlme_info *(*rtw_get_adapter_mlmeinfo)(u8 iface_type);
	struct bcn_info_param *(*rtw_get_adapter_bcninfo)(u8 iface_type);
	u8(*rtw_get_adapter_stopped)(u8 iface_type);
	int (*rtw_state_check_ap_on)(void);
	struct dot11d_priv *(*rtw_get_adapter_dot11dpriv)(u8 iface_type);
	struct addba_request *(*rtw_get_adapter_addba_req)(u8 iface_type);
	void (*rtw_reset_securitypriv)(void);

	struct hw_xmit *(*rtw_get_pendingqueue)(u8 iface_type);

	void (*rtw_init_timer_t)(_timer *ptimer, void *adapter, TIMER_FUN pfunc, void *cntx, const char *name);
	void (*rtw_set_timer_t)(_timer *ptimer, u32 delay_time_ms);
	u8(*rtw_cancel_timer_t)(_timer *ptimer);
	void (*rtw_del_timer_t)(_timer *ptimer);
	uint32_t (*rtos_timer_is_timer_active)(rtos_timer_t p_handle);
	uint32_t (*pmu_set_sysactive_time_t)(uint32_t timeout);
	void (*rtw_wakelock_timeout)(u32 timeoutms);

	void (*wifi_if_netif_rx_t)(struct sk_buff *skb, void *preorder_ctrl);
	void (*rtw_start_clnt_assoc)(void);
	void (*rtw_start_clnt_auth)(void);
	int (*rtw_leavebss_start)(unsigned char *MacAddr, unsigned short reason);

	void (*rtw_network_copy)(struct wlan_network *target_network, struct wlan_network *src_network, u8 with_ie);
	struct wlan_network *(*rtw_network_zmalloc)(struct wlan_network *network);

	void (*rtw_11d_update_regulatory_from_country)(u8 *country_code);
	void (*HE_operation_update_asoc_cap)(u8 iface_type, struct _NDIS_802_11_VARIABLE_IEs *pIE);
	int (*rtw_ips_wakeup)(u32 ips_deffer_ms);

	u8(*rtw_ft_check_en)(struct wlan_network *target_network);

	/* psta */
	struct	sta_mlme_priv *(*rtw_alloc_stainfo)(u8 iface_type, u8 *hwaddr);
	int(*rtw_free_stainfo)(u8 iface_type, struct sta_mlme_priv *psta_mlmepriv);
	struct sta_mlme_priv *(*rtw_sta_get_mlmepriv)(u8 iface_type, u8 *sta_addr);
	struct sta_security_priv *(*rtw_sta_get_secpriv)(u8 iface_type, u8 *sta_addr);
	struct sta_xmit_priv *(*rtw_sta_get_xmitpriv)(u8 iface_type, u8 *sta_addr);
	struct sta_recv_priv *(*rtw_sta_get_recvpriv)(u8 iface_type, u8 *sta_addr);
	struct ht_priv *(*rtw_sta_get_htpriv)(u8 iface_type, u8 *sta_addr);
	struct sta_sc_priv *(*rtw_sta_get_scpriv)(u8 iface_type, u8 *sta_addr);
	u8(*rtw_ap_deauth_and_free_sta)(struct sta_mlme_priv *psta_mlmepriv, unsigned short reason);
	int(*rtw_free_stainfo_enqueue_cmd)(u8 iface_type, struct sta_mlme_priv *psta_mlmepriv);
	void (*rtw_ap_polling_sta_hdl)(void);
	void (*rtw_sta_del_event)(u8 iface_type, unsigned char *MacAddr, unsigned short reason);
	u8(*rtw_macid_max_num)(void);

	/* join & scan */
	void (*rtw_joinbss_result)(int res);
	int(*rtw_joinbss_cmd)(struct wlan_network *target_network);
	int (*rtw_disassoc_cmd)(void);
	int (*rtw_scan_start_cmd)(u8 iface_type, struct dot11_ssid *pssid, u16 duration, u32 dot11k_token);
	int(*rtw_xmit_mgntframe)(u8 iface_type, struct xmit_frame *pmgntframe);
	int(*wifi_hal_mgnt_xmit)(u8 iface_type, struct xmit_frame *pmgntframe);
	int(*wifi_hal_xmit)(u8 iface_type, struct xmit_frame *pxmitframe);
	void(*wifi_hal_txdesc_fill)(struct xmit_frame *pxmitframe, u8 *pmem);
	int(*rtw_mgmt_xmitframe_coalesce)(u8 iface_type, struct security_priv *psecuritypriv, struct xmit_frame *pxmitframe, u8 b_swenc);
	void (*rtw_recv_validate_mgnt_frame)(u8 iface_type, union recv_frame **pprecv_frame);
	int (*rtw_enqueue_cmd)(u8 iface_type, u8 *parmbuf, int(*cmd_hdl)(u8 iface_type, struct cmd_obj *pcmd), u32 parm_nobuf);
	int(*wifi_hal_xmitframe_dump_sc)(u8 iface_type, struct sk_buff *pkt, struct sta_mlme_priv *psta_mlmepriv, int idx, u8 UserPriority);
	void (*wifi_hal_set_channel_bw)(unsigned char channel, unsigned char channel_offset, unsigned short bwmode);
	void (*wifi_hal_bb_sta_add)(u8 iface_type, struct sta_mlme_priv *psta_mlmepriv);
	void (*wifi_hal_bb_sta_del)(u8 iface_type, struct sta_mlme_priv *psta_mlmepriv);
	void (*wifi_hal_txdesc_setrate)(u8 iface_type, struct txdesc_priv *ptxdesc);
	void (*wifi_hal_ra_register)(u8 iface_type, struct sta_mlme_priv *psta_mlmepriv);
	void (*wifi_hal_update_uapsd_tid)(u8 uapsd_tid);

	int(*rtw_setkey_cmd_hdl)(u8 iface_type, struct cmd_obj *cmd_obj);
	void (*rtw_hal_btc_sec_key_exchange_notify)(u8 iface_type, u8 b_start, u8 b_grp_key_update);
	bool (*halbb_set_bss_color)(u8 bss_color, enum phl_phy_idx phy_idx);
	void (*wifi_hal_ra_update_support_rate)(struct sta_mlme_priv *psta_mlmepriv);
	void (*wifi_indication)(u32 event, u8 *evt_info, s32 evt_len); //data type of buf in rom is char *
	u8(*rtw_is_adapter_up)(u8 iface_type);
	/* nan */
	u8(*rtw_is_nan_frame)(union recv_frame *precv_frame);
	u8(*rtw_nan_schdlr_set_pause_schedule)(u8 iface_type, bool bpause);
	void (*rtw_nan_schdlr_ap_set)(u8 iface_type, u8 status);
	void (*rtw_nan_schdlr_sta_set)(u8 iface_type, bool bconnect);
	int (*rtw_check_nan_enabled)(void);
	int (*rtw_check_nan_datalink_exist)(void);
	void (*rtw_xmit_make_wlanhdr_nanaddr)(struct rtw_ieee80211_hdr *pwlanhdr, struct pkt_attrib *pattrib);

	/* p2p */
	u8(*rtw_get_p2p_role)(void);

	/* wifi tunnel */
	void (*wifi_tunnelapi_update_txdata_macswap)(struct txdesc_priv *ptxdesc, u8 *pframe, u8 sc);

	/* others */
	/* todo: to rom */
	int (*_sscanf_ss)(const char *buf, const char *fmt, ...);

	u8 cfg80211;
	u8 rtw_trp_tis_cert_en;
	u8 bw_40_enable;
	u8 b_wifi_logo_en : 1;
	u8 b_dfs_en : 1;
	u8 b_owe_en : 1;
	u8 b_wmm_en : 1;

	u8 wifi_drv_mode;
	rtos_sema_t singlethread_wakeup_sema;
};

extern struct _wifi_rom_to_flash_func_map wifi_rom2flash;
extern struct _wifi_rom_to_flash_func_map *p_wifi_rom2flash;

#endif
