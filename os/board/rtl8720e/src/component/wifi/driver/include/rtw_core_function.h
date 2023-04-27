#ifndef _RTW_CORE_FUNCTION_H_
#define _RTW_CORE_FUNCTION_H_
#ifdef _RTW_ADAPTER_DEFINED_H_

/*-------------------------------------------------------------------------------
	rtw_recv.c.
--------------------------------------------------------------------------------*/
sint rtw_recv_init_priv(struct recv_priv *precvpriv, _adapter *padapter);
void rtw_recv_free_priv(struct recv_priv *precvpriv);
union recv_frame *rtw_recv_alloc_frame(_queue *pfree_recv_queue);
int rtw_recv_free_frame(union recv_frame *precvframe, _queue *pfree_recv_queue);
void rtw_recv_free_frame_queue(_queue *pframequeue,  _queue *pfree_recv_queue);
u32 rtw_recv_free_uc_swdec_pending_queue(_adapter *padapter);
void rtw_recv_free_defrag_queue(PADAPTER padapter, struct sta_info *psta);
int rtw_recv_tkip_countermeasure(_adapter *padapter);
sint rtw_recv_frame_chkmic(_adapter *padapter,  union recv_frame *precvframe);
sint rtw_recv_frame_chkpn(_adapter *padapter,  union recv_frame *precvframe);
sint rtw_recv_frame_chk_frag_pn(_adapter *padapter,  union recv_frame *precvframe, u8 fragnum);
union recv_frame *rtw_recv_frame_chk_plaintext(_adapter *padapter, union recv_frame *precv_frame);
union recv_frame *rtw_recv_frame_defrag(_adapter *padapter, _queue *defrag_q);
union recv_frame *rtw_recv_frame_chk_defrag(PADAPTER padapter, union recv_frame *precv_frame);
union recv_frame *rtw_recv_decryptor(_adapter *padapter, union recv_frame *precv_frame);
void rtw_recv_count_rx_stats(_adapter *padapter, union recv_frame *prframe, struct sta_info *sta);
sint rtw_recv_sta2sta_data_frame(_adapter *padapter, union recv_frame *precv_frame, struct sta_info **psta);
sint rtw_recv_ap2sta_data_frame(_adapter *padapter, union recv_frame *precv_frame, struct sta_info **psta);
sint rtw_recv_sta2ap_data_frame(_adapter *padapter, union recv_frame *precv_frame, struct sta_info **psta);
void rtw_recv_validate_ctrl_frame(_adapter *padapter, union recv_frame *precv_frame);
void rtw_recv_validate_mgnt_frame(PADAPTER padapter, union recv_frame **pprecv_frame);
sint rtw_recv_validate_data_frame(_adapter *padapter, union recv_frame *precv_frame);
sint rtw_recv_validate_frame(_adapter *padapter, union recv_frame **pprecv_frame);
sint rtw_recv_wlanhdr_to_ethhdr(union recv_frame *precvframe);
int rtw_recv_amsdu_to_msdu(_adapter *padapter, union recv_frame *prframe);
int rtw_recv_enqueue_reorder_recvframe(struct recv_reorder_ctrl *preorder_ctrl, union recv_frame *prframe);
int rtw_recv_indicatepkt(_adapter *padapter, union recv_frame *precv_frame);
int rtw_recv_indicatepkt_in_order(_adapter *padapter, struct recv_reorder_ctrl *preorder_ctrl, int bforced);
int rtw_recv_indicatepkt_reorder(_adapter *padapter, union recv_frame *prframe);
int rtw_recv_process_indicatepkts(_adapter *padapter, union recv_frame *prframe);
int rtw_recv_func_prehandle(_adapter *padapter, union recv_frame *rframe);
int rtw_recv_func_posthandle(_adapter *padapter, union recv_frame *prframe);
s32 rtw_recv_entry(union recv_frame *precvframe);
void rtw_recv_reordering_ctrl_timer_hdl(void *FunctionContext);
void rtw_recv_signal_stat_timer_hdl(void *FunctionContext);
u8 *rtw_recv_get_recvframe_data(union recv_frame *precvframe);
u8 *rtw_recv_recvframe_pull(union recv_frame *precvframe, sint sz);
u8 *rtw_recv_recvframe_put(union recv_frame *precvframe, sint sz);
u8 *rtw_recv_recvframe_pull_tail(union recv_frame *precvframe, sint sz);

/*-------------------------------------------------------------------------------
	rtw_xmit.c.
--------------------------------------------------------------------------------*/
s32	rtw_xmit_priv_init(struct xmit_priv *pxmitpriv, _adapter *padapter);
void rtw_xmit_priv_free(struct xmit_priv *pxmitpriv);
void rtw_xmit_open_pktfile(struct sk_buff *pktptr, struct pkt_file *pfile);
uint rtw_xmit_read_pktfile(struct pkt_file *pfile, u8 *rmem, uint rlen);
s32 rtw_xmit_enc_tkip_addmic(_adapter *padapter, struct xmit_frame *pxmitframe);
#ifdef SW_ENCRYPT_HTC_PKT
u32 rtw_xmit_enc_aes_use_ipsec(u8 *pframe, u32 wlan_hdr_len, u32 payload_len, u8 *key, u32 frame_type);
#endif
s32 rtw_xmit_enc_software(_adapter *padapter, struct xmit_frame *pxmitframe);
s32 rtw_xmit_make_wlanhdr(_adapter *padapter, u8 *hdr, struct pkt_attrib *pattrib);
int rtw_xmit_raw_frame(_adapter *padapter, raw_data_desc_t *raw_data_desc);
s32 rtw_xmit_pending_queue_check(_adapter *padapter, int prio);
void rtw_xmit_update_attrib_vcs_info(_adapter *padapter, struct xmit_frame *pxmitframe);
s32 rtw_xmit_put_snap(u8 *data, u16 h_proto);
void rtw_xmit_count_stats(PADAPTER padapter, struct xmit_frame *pxmitframe, int sz);
s32 rtw_xmit_classifier(_adapter *padapter, struct xmit_frame *pxmitframe);
int rtw_xmit_mgntframe(_adapter *padapter, struct xmit_frame *pmgntframe);
int rtw_xmit_entry(struct sk_buff *pkt, struct net_device *pnetdev);
u8	rtw_xmit_query_short_GI(struct sta_info *psta, u8 bw);
void rtw_xmit_nulldata(_adapter *padapter, unsigned int power_mode);
void rtw_xmit_qos_nulldata(_adapter *padapter, unsigned char *da, u16 tid);
s32 rtw_txframes_pending(_adapter *padapter);


/*-------------------------------------------------------------------------------
	rtw_xmitbuff.c.
--------------------------------------------------------------------------------*/
struct xmit_frame *rtw_xmitframe_alloc(_adapter *padapter);
s32 rtw_xmitframe_free(struct xmit_priv *pxmitpriv, struct xmit_frame *pxmitframe);
void rtw_xmitframe_queue_free(struct xmit_priv *pxmitpriv, _queue *pframequeue);
s32 rtw_xmitframe_enqueue_one(_adapter *padapter, struct xmit_frame *pxmitframe);
struct xmit_frame *rtw_xmitframe_dequeue_one(struct xmit_priv *pxmitpriv, int *qcnt, _queue *pframe_queue);
struct xmit_buf *rtw_xmitbuf_alloc(struct xmit_priv *pxmitpriv);
s32 rtw_xmitbuf_free(struct xmit_priv *pxmitpriv, struct xmit_buf *pxmitbuf);
struct xmit_buf *rtw_xmitbuf_ext_alloc(struct xmit_priv *pxmitpriv, u32 size);
s32 rtw_xmitbuf_ext_free(struct xmit_priv *pxmitpriv, struct xmit_buf *pxmitbuf);
BOOLEAN rtw_xmitbuf_enqueue(IN  struct rtw_tx_ring	*ring, IN  struct xmit_buf *pxmitbuf);
struct xmit_buf *rtw_xmitbuf_dequeue(IN  struct rtw_tx_ring	*ring);

/*-------------------------------------------------------------------------------
	rtw_shortcut.c.
--------------------------------------------------------------------------------*/
#ifdef RX_SHORTCUT
int rtw_recv_get_rx_sc_index(ADAPTER *padapter, unsigned char *hdr);
int rtw_recv_get_rx_sc_free_entry(ADAPTER *padapter, unsigned char *hdr);
int rtw_recv_indicatepkt_sc(_adapter *padapter, union recv_frame *precv_frame, int idx);
int rtw_recv_check_rx_shortcut_path(_adapter *padapter, union recv_frame *precv_frame);
#endif // RX_SHORTCUT
#ifdef TX_SHORTCUT
int rtw_get_tx_sc_index(struct sta_info *pstat, unsigned char *hdr);
int rtw_get_tx_sc_free_entry(struct sta_info *pstat, unsigned char *hdr);
s32 rtw_xmit_entry_sc(_adapter *padapter, struct sk_buff *pkt);
#endif

/*-------------------------------------------------------------------------------
	rtw_sta_mgt.c.
--------------------------------------------------------------------------------*/
void _rtw_init_stainfo(struct sta_info *psta);
void _rtw_defrag_timer_handler(void *FunctionContext);
void init_defrag_queue_timer(_adapter *padapter, struct sta_info *psta);
struct	sta_info *rtw_alloc_stainfo(struct sta_priv *pstapriv, u8 *hwaddr);
u32 rtw_free_stainfo(_adapter *padapter, struct sta_info *psta);
u8 rtw_free_stainfo_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u32 rtw_free_stainfo_enqueue_cmd(_adapter *padapter, struct sta_info *psta);
struct sta_info *rtw_get_stainfo(struct sta_priv *pstapriv, u8 *hwaddr);
struct sta_info *rtw_get_bcmc_stainfo(_adapter *padapter);
void rtw_free_all_stainfo(_adapter *padapter);
void rtw_update_sta_info(_adapter *padapter, struct sta_info *psta);
u32 rtw_init_bcmc_stainfo(_adapter *padapter);
struct recv_reorder_ctrl *rtw_alloc_reorder_ctrl(_adapter *padapter);
void rtw_free_reorder_ctrl(_adapter *padapter, struct sta_info *psta, u8 tid);
void _rtw_sta_del_event_callback(_adapter *padapter);
u8 rtw_sta_del_event_callback(_adapter *padapter, struct cmd_obj *cmd_obj);
void rtw_sta_del_event(_adapter *padapter, unsigned char *MacAddr, unsigned short reason);

/*-------------------------------------------------------------------------------
	rtw_task.c.
--------------------------------------------------------------------------------*/
int rtw_if_wifi_create_task(struct task_struct *ptask, const char *name, u32  stack_size, u32 priority, thread_func_t func, void *thctx);
void rtw_if_wifi_delete_task(struct task_struct *ptask);
void rtw_if_wifi_wakeup_task(struct task_struct *ptask);

/*-------------------------------------------------------------------------------
	rtw_sitesurvey.c.
--------------------------------------------------------------------------------*/
void rtw_scan_report_free_remaining_node(struct mlme_priv *pmlmepriv);
int rtw_scan_start_api(rtw_scan_param_t *scan_param, unsigned char block);
u8 rtw_scan_start_cmd(_adapter  *padapter, struct sitesurvey_parm *pparm);
u8 rtw_scan_cmd_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
void rtw_scan_cmd_callback(_adapter	*padapter,  struct cmd_obj *pcmd);
void rtw_scan_one_channel(_adapter *padapter);
int rtw_scan_report_node_process(_adapter *padapter, rtw_scan_report_node_t *new_node);
int rtw_scan_report_node_sort_and_insert(_adapter *padapter, rtw_scan_report_node_t *new_node);
int rtw_scan_report_one_node(_adapter *padapter, WLAN_BSSID_EX *bssid);
#ifdef CONFIG_MBSSID_AX
u8 rtw_scan_mbssid_bssid_convert(u8 trans_bssid_b5, u8 idx, u8 n);
void rtw_scan_mbssid_parse(WLAN_BSSID_EX *bssid);
#endif
u8 rtw_scan_collect_bss_info(_adapter *padapter, union recv_frame *precv_frame, WLAN_BSSID_EX *bssid);
void rtw_scan_one_beacon_report(_adapter *padapter, union recv_frame *precv_frame);
void rtw_scan_done_user_callback(_adapter	*padapter);
void rtw_scan_one_channel_done(_adapter *padapter);
void rtw_scan_timeout_handler(void *FunctionContext);
void rtw_scan_timer_hdl(void *FunctionContext);

/*-------------------------------------------------------------------------------
	rtw_join.c.
--------------------------------------------------------------------------------*/
void rtw_joinbss_timer_hdl(void *FunctionContext);
sint rtw_joinbss_linked_check(_adapter *padapter);
void rtw_joinbss_chk_done_concurrent(_adapter *padapter, int join_res);
void rtw_leavebss_indicate(_adapter *padapter);
void rtw_joinbss_assoc_timer_hdl(void *FunctionContext);
void rtw_joinbss_done_indicate(_adapter *padapter);
void rtw_joinbss_result_callback(_adapter *padapter, int join_res);
void rtw_joinbss_result(_adapter *padapter, int res);
void rtw_joinbss_cmd_callback(_adapter	*padapter,  struct cmd_obj *pcmd);
u8 rtw_joinbss_cmd_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_joinbss_cmd(_adapter  *padapter, struct wlan_network *ptgt_network);
int rtw_joinbss_from_scanned_queue(struct mlme_priv *pmlmepriv);
u8 rtw_joinbss_start(_adapter *padapter);
u8 _rtw_joinbss_set_bssid(_adapter *padapter, u8 *bssid);
int rtw_joinbss_set_bssid(unsigned char wlan_idx, __u8 *bssid);
u8 _rtw_joinbss_set_ssid(_adapter *padapter, NDIS_802_11_SSID *ssid);
int rtw_joinbss_set_ssid(__u8 *ssid, __u16 ssid_len);
int rtw_bss_set_auth(unsigned char wlan_idx, __u32 value);
int rtw_bss_set_enc(unsigned char wlan_idx, __u16 alg, __u8 *addr, int key_idx, int set_tx, __u8 *seq, __u16 seq_len, __u8 *key, __u16 key_len);
int rtw_bss_set_passphrase(unsigned char wlan_idx, __u8 *passphrase, __u16 passphrase_len);
int rtw_joinbss_start_api(rtw_network_info_t *connect_param);
void rtw_leavebss_status_update(_adapter *padapter);
u8 rtw_leavebss_cmd_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
unsigned int rtw_leavebss_start(_adapter *padapter, unsigned char *MacAddr, unsigned short reason);
void rtw_leavebss_event_indicate(_adapter *padapter);

/*-------------------------------------------------------------------------------
	rtw_probe.c.
--------------------------------------------------------------------------------*/
void rtw_issue_probersp(_adapter *padapter, unsigned char *da, u8 is_valid_p2p_probereq);
void rtw_issue_probereq(_adapter *padapter, NDIS_802_11_SSID *pssid, u8 blnbc);
unsigned int rtw_on_probereq(_adapter *padapter, union recv_frame *precv_frame);
unsigned int rtw_on_probersp(_adapter *padapter, union recv_frame *precv_frame);

/*-------------------------------------------------------------------------------
	rtw_auth.c.
--------------------------------------------------------------------------------*/
void rtw_start_clnt_join(_adapter *padapter);
void rtw_start_clnt_auth(_adapter *padapter);
void rtw_issue_auth(_adapter *padapter, struct sta_info *psta, unsigned short status);
void rtw_issue_deauth(_adapter *padapter, unsigned char *da, u32 reason);
unsigned int rtw_on_auth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int rtw_on_auth_client(_adapter *padapter, union recv_frame *precv_frame);
unsigned int rtw_on_deauth(_adapter *padapter, union recv_frame *precv_frame);

/*-------------------------------------------------------------------------------
	rtw_assoc.c.
--------------------------------------------------------------------------------*/
void rtw_start_clnt_assoc(_adapter *padapter);
void rtw_issue_assocreq(_adapter *padapter, u8 is_reassoc);
void rtw_issue_assocrsp(_adapter *padapter, unsigned short status, struct sta_info *pstat, int pkt_type);
unsigned int rtw_on_assocreq(_adapter *padapter, union recv_frame *precv_frame);
void rtw_on_assocrsp_wmm(_adapter *padapter);
unsigned int rtw_on_assocrsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int rtw_on_disassoc(_adapter *padapter, union recv_frame *precv_frame);
int rtw_assoc_set_wpa_ie(_adapter *padapter, char *pie, unsigned short ielen);
u8 rtw_disassoc_cmd(_adapter *padapter);
void rtw_assoc_resources_free(_adapter *padapter, int lock_scanned_queue);

/*-------------------------------------------------------------------------------
	rtw_setkey.c.
--------------------------------------------------------------------------------*/
sint rtw_setkey_to_cam(_adapter *padapter, struct security_priv *psecuritypriv, sint keyid, u8 set_tx);
void rtw_setkey_cmd_callback(_adapter *padapter,  struct cmd_obj *pcmd);
u8 rtw_setkey_clear_stakey(_adapter *padapter, u8 *psta, u8 entry);
u8 rtw_setkey_set_stakey(_adapter *padapter, u8 *psta, u8 unicast_key);
u8 rtw_setkey_cmd_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
void rtw_setkey_remove_ptk(_adapter *padapter, struct sta_info *psta);
void rtw_setkey_set_ptk(_adapter *padapter, struct sta_info *psta);
void rtw_setkey_set_gtk(_adapter *padapter);
void rtw_setkey_set_igtk(_adapter *padapter);

/*-------------------------------------------------------------------------------
	rtw_psk.c.
--------------------------------------------------------------------------------*/
void rtw_psk_sta_wait_4way_msg(_adapter *padapter, struct sta_info *psta);
void rtw_psk_sta_send_eapol(_adapter *padapter, struct sta_info *psta, int resend);
void rtw_psk_sta_eapolkey_recvd(_adapter *padapter, struct sta_info *psta);
void rtw_psk_ap_send_eapol(_adapter *padapter, struct sta_info *psta, int resend);
void rtw_psk_ap_eapolkey_recvd(_adapter *padapter, struct sta_info *psta);
void rtw_psk_init(_adapter *padapter, unsigned char *pie, unsigned short ielen);
void rtw_psk_init_wpa_sta_info(_adapter *padapter, struct sta_info *psta);
int rtw_psk_tkip_send_mic_failure_report(_adapter *padapter, struct	sta_info *psta);

/*-------------------------------------------------------------------------------
	rtw_sae.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_SAE_SUPPORT
int rtw_sae_generate_commit_msg(struct sae_data *sae, u8 *own_addr, u8 *peer_addr, enum sae_group_config group_config);
int rtw_sae_handle_auth(_adapter *padapter, struct sta_info *psta, struct sae_data *sae, u8 *own_addr, u8 *peer_addr,
						u8 *sae_msg, u32 sae_msg_len, u16 seq_num, u16 status);
struct sae_data *rtw_sae_allocate_instance(_adapter *padapter);
void rtw_sae_free_instance(_adapter *padapter, const char *reason);
#endif

/*-------------------------------------------------------------------------------
	rtw_wnm.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_RTW_WNM
u8 rtw_wmn_btm_rsp_reason_decision(_adapter *padapter, u8 *req_mode);
u8 rtw_wnm_btm_reassoc_req(_adapter *padapter);
void rtw_wnm_roam_scan_hdl(void *ctx);
void rtw_wnm_disassoc_chk_hdl(void *ctx);
u8 rtw_wnm_try_btm_roam_imnt(_adapter *padapter);
void rtw_wnm_process_btm_req(_adapter *padapter, u8 *pframe, u32 frame_len);
void rtw_wnm_reset_btm_candidate(struct roam_nb_info *pnb);
void rtw_wnm_reset_btm_cache(_adapter *padapter);
void rtw_wnm_reset_btm_state(_adapter *padapter);
void rtw_wnm_issue_action(_adapter *padapter, u8 action, u8 reason, u8 dialog);
#endif /* CONFIG_RTW_WNM */
void wnm_dbg_cmd(_adapter *padapter, char *s);
#if defined(CONFIG_RTW_WNM) || defined(CONFIG_IEEE80211K)
void rtw_roam_nb_info_init(_adapter *padapter);
void rtw_roam_nb_info_deinit(_adapter *padapter);
void rtw_roam_nb_enable(_adapter *padapter);
void rtw_roam_nb_disable(_adapter *padapter);
void rtw_check_peer_btm_cap(_adapter *padapter, struct wlan_network *pcur_network);
u8 rtw_roam_nb_scan_list_set(_adapter *padapter, struct sitesurvey_parm *pparm);
u32 rtw_wnm_btm_candidates_survey(_adapter *padapter, u8 *pframe, u32 elem_len, u8 from_btm);
void sitesurvey_res_reset(_adapter *padapter, struct sitesurvey_parm *pparm);
#endif /*defined(CONFIG_RTW_WNM) || defined(CONFIG_IEEE80211K) */

/*-------------------------------------------------------------------------------
	rtw_autoreconnect.c.
--------------------------------------------------------------------------------*/
#if CONFIG_AUTO_RECONNECT
void rtw_autoreconnect_psk_timeout(void *task_psta);
void rtw_autoreconnect_thread(void *param);
void rtw_autoreconnect_hdl(rtw_security_t security_type, char *ssid, int ssid_len, char *password, int password_len, int key_id);
void rtw_autoreconnect_timer_hdl(void *FunctionContext);
u8 rtw_autoreconnect_start(_adapter *padapter, bool continue_by, bool fixed_timeout);
#endif

/*-------------------------------------------------------------------------------
	rtw_network.c.
--------------------------------------------------------------------------------*/
struct wlan_network *rtw_network_alloc(struct	mlme_priv *pmlmepriv);
void rtw_network_queue_free(_adapter *dev);
struct wlan_network *rtw_network_find(_queue *scanned_queue, u8 *addr);
void rtw_network_update_for_scan(WLAN_BSSID_EX *dst, WLAN_BSSID_EX *src, _adapter *padapter, bool update_ie);
void rtw_network_update_vcs(_adapter *padapter, u8 *ie, uint ie_len);
void rtw_network_add_for_scan(_adapter *padapter, WLAN_BSSID_EX *target);
int rtw_network_check_same(WLAN_BSSID_EX *src, WLAN_BSSID_EX *dst);

/*-------------------------------------------------------------------------------
	rtw_rm_fsm.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_IEEE80211K
static void rm_state_initial(struct rm_obj *prm);
static void rm_state_goto(struct rm_obj *prm, enum RM_STATE rm_state);
static void rm_state_run(struct rm_obj *prm, enum RM_EV_ID evid);
static struct rm_event *rm_dequeue_ev(_queue *queue);
static struct rm_obj *rm_dequeue_rm(_queue *queue);
void rm_timer_callback(void *data);
void rm_free_rmobj(struct rm_obj *prm);
struct rm_obj *rm_alloc_rmobj(_adapter *padapter);
int rm_enqueue_rmobj(_adapter *padapter, struct rm_obj *prm, bool to_head);
struct sta_info *rm_get_psta(_adapter *padapter, u32 rmid);
struct rm_obj *rm_get_rmobj(_adapter *padapter, u32 rmid);
u8 rtw_rm_post_envent_cmd(_adapter *padapter, u32 rmid, u8 evid);
u8 rtw_rm_post_event_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
int rm_post_event(_adapter *padapter, u32 rmid, enum RM_EV_ID evid);
int _rm_post_event(_adapter *padapter, u32 rmid, enum RM_EV_ID evid);
void rm_handler(_adapter *padapter, struct rm_event *pe);
char *rm_event_name(enum RM_EV_ID evid);
void rtw_deinit_rm(_adapter *padapter);
void rtw_rm_enable(_adapter *padapter);
void rtw_rm_disable(_adapter *padapter);
void rtw_check_peer_rm_cap(_adapter *padapter, struct wlan_network *pcur_network);
#endif /* CONFIG_IEEE80211K */

/*-------------------------------------------------------------------------------
	rtw_rm_util.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_IEEE80211K
void rm_init_rtw_op_class(_adapter *padapter);
u8 rm_set_rtw_op_class(u32 country_code);
u8 rm_get_ch_set(struct rtw_ieee80211_channel *pch_set, u8 op_class, u8 ch_num);
u8 rm_get_oper_class_via_ch(u8 ch);
int is_wildcard_bssid(u8 *bssid);
u8 translate_dbm_to_rcpi(s8 SignalPower);
u8 translate_percentage_to_rcpi(u32 SignalStrengthIndex);
u8 rm_get_bcn_rcpi(struct rm_obj *prm, struct wlan_network *pcur_network);
u8 rm_get_frame_rsni(struct rm_obj *prm, union recv_frame *pframe);
u8 rm_get_bcn_rsni(struct rm_obj *prm, struct wlan_network *pcur_network);
int rm_get_tx_power(PADAPTER padapter, enum rf_path path, u8 rate, s8 *pwr);
int rm_get_rx_sensitivity(PADAPTER padapter, enum channel_width bw, u8 rate, s8 *pwr);
int rm_get_path_a_max_tx_power(_adapter *padapter, s8 *path_a);
#endif /* CONFIG_IEEE80211K */

/*-------------------------------------------------------------------------------
	rtw_rm.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_IEEE80211K
char *rm_type_rep_name(u8 meas_type);
int rm_en_cap_chk_and_set(struct rm_obj *prm, enum rm_cap_en en);
/* for caller outside rm */
u8 rm_add_nb_req(_adapter *padapter, struct sta_info *psta);
void rm_set_rep_mode(struct rm_obj *prm, u8 mode);
int issue_null_reply(struct rm_obj *prm);
int ready_for_scan(struct rm_obj *prm);
int rm_sitesurvey(struct rm_obj *prm);
int rm_recv_radio_mens_req(_adapter *padapter, union recv_frame *precv_frame, struct sta_info *psta);
int rm_recv_radio_mens_rep(_adapter *padapter, union recv_frame *precv_frame, struct sta_info *psta);
int rm_recv_link_mens_req(_adapter *padapter, union recv_frame *precv_frame, struct sta_info *psta);
int rm_recv_link_mens_rep(_adapter *padapter, union recv_frame *precv_frame, struct sta_info *psta);
int rm_radio_mens_nb_rep(_adapter *padapter, union recv_frame *precv_frame, struct sta_info *psta);
unsigned int onAction_rm(_adapter *padapter, union recv_frame *precv_frame);
u8 rm_bcn_req_cond_mach(struct rm_obj *prm, struct wlan_network *pcur_network);
int issue_beacon_rep(struct rm_obj *prm);
/* neighbor request */
int issue_nb_req(struct rm_obj *prm);
/* issue link measurement request */
int issue_link_meas_req(struct rm_obj *prm);
/* issue link measurement report */
int issue_link_meas_rep(struct rm_obj *prm);
int issue_radio_meas_req(struct rm_obj *prm);
int rm_radio_meas_report_cond(struct rm_obj *prm);
int retrieve_radio_meas_result(struct rm_obj *prm);
int issue_radio_meas_rep(struct rm_obj *prm);
void RM_IE_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
#if defined(CONFIG_LAYER2_ROAMING)
void rtw_roam_nb_discover(_adapter *padapter, u8 bfroce);
#endif
#if (RM_SUPPORT_IWPRIV_DBG)
void rm_dbg_list_sta(_adapter *padapter, char *s);
void rm_dbg_help(_adapter *padapter, char *s);
struct sta_info *rm_get_sta(_adapter *padapter, u16 aid, u8 *pbssid);
#endif /* RM_SUPPORT_IWPRIV_DBG */
void rm_dbg_cmd(_adapter *padapter, char *s);
#endif /* CONFIG_IEEE80211K */

/*-------------------------------------------------------------------------------
	rtw_dynamic.c.
--------------------------------------------------------------------------------*/
u8 rtw_dynamic_check_work_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_dynamic_check_work_cmd(_adapter *padapter);
u8 rtw_dynamic_check_timer_hdl(_adapter *padapter);
void rtw_dynamic_timer_hdl(void *FunctionContext);
void rtw_dynamic_check_link_status(_adapter *padapter);

/*-------------------------------------------------------------------------------
	wifi_performance_monitor.c.
--------------------------------------------------------------------------------*/
#ifdef WIFI_PERFORMANCE_MONITOR
u32 WIFI_PMTimer_GetCount(void);
u32 WIFI_PMTimer_GetPassTime(u32 start);
void wifi_performance_print();
#endif //WIFI_PERFORMANCE_MONITOR
#ifndef CONFIG_AS_INIC_AP
u32 wifi_heap_size_printt(void);
#endif

/*-------------------------------------------------------------------------------
	rtw_mgnt.c.
--------------------------------------------------------------------------------*/
void rtw_mgntframe_dispatcher(_adapter *padapter, union recv_frame *precv_frame);
unsigned int rtw_mgntframe_do_reserved(_adapter *padapter, union recv_frame *precv_frame);
struct xmit_frame *rtw_mgntframe_alloc(_adapter *padapter);
void rtw_mgntframe_attrib_update(_adapter *padapter, struct pkt_attrib *pattrib);
void rtw_mgntframe_indicate(_adapter *padapter, u8 *data, u16 len, u8 channel);
s32 rtw_mgmt_xmitframe_coalesce(_adapter *padapter, struct sk_buff *pkt, struct xmit_frame *pxmitframe);

/*-------------------------------------------------------------------------------
	rtw_pmksa_cache.c.
--------------------------------------------------------------------------------*/
void rtw_pmksa_cache_entry_free(struct pmksa_cache_entry *pentry);
int rtw_pmksa_cache_entry_add(struct pmksa_cache *ppmksa, u8 *pmk, u8 *pmkid, u8 *addr1, u32 akmp, u8 *encrypted_addr);
struct pmksa_cache_entry *rtw_pmksa_cache_entry_remove(struct pmksa_cache *ppmksa, struct pmksa_cache_entry *pdelete);
struct pmksa_cache_entry *rtw_pmksa_cache_entry_search(struct pmksa_cache *ppmksa, u8 *addr1, u32 akmp);
void rtw_pmksa_cache_set_current_entry(struct pmksa_cache *ppmksa, struct pmksa_cache_entry *pentry);
struct pmksa_cache_entry *rtw_pmksa_cache_get_current_entry(struct pmksa_cache *ppmksa);
int rtw_pmksa_cache_init(_adapter *padapter);
int rtw_pmksa_cache_deinit(_adapter *padapter);

/*-------------------------------------------------------------------------------
	rtw_ft.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_IEEE80211R
void rtw_ft_info_init(_adapter *padapter);
void rtw_ft_info_deinit(_adapter *padapter);
void rtw_ft_enable(_adapter *padapter);
void rtw_ft_disable(_adapter *padapter);
int rtw_ft_derive_ptk(_adapter *padapter);
int rtw_ft_set_key(_adapter *padapter, struct sta_info *psta);
int rtw_ft_validate_ie_eapol_3(_adapter *padapter, u8 *buf, u32 len);
int rtw_ft_process_assoc_resp(_adapter *padapter, u8 *pframe, u32 frame_len);
int rtw_ft_process_auth_resp(_adapter *padapter, u8 *pframe, u32 frame_len);
int rtw_ft_chk_roaming_candidate(_adapter *padapter, struct wlan_network *competitor);
void rtw_ft_validate_akm_type(_adapter  *padapter, struct wlan_network *pcur_network);
void rtw_ft_build_eapol_2_ies(_adapter *padapter, OCTET_STRING *EapolKeyMsgSend);
void rtw_ft_build_auth_req_ies(_adapter *padapter, struct pkt_attrib *pattrib, u8 **pframe);
u8 rtw_ft_build_assoc_req_ies(_adapter *padapter, u8 is_reassoc, struct pkt_attrib *pattrib, u8 **pframe);
void rtw_ft_issue_action_req(_adapter *padapter, u8 *pTargetAddr);
void rtw_ft_start_roam(_adapter *padapter, u8 *pTargetAddr);
void rtw_ft_link_timer_hdl(void *FunctionContext);
void rtw_ft_roam_timer_hdl(void *FunctionContext);
void rtw_ft_roam_status_reset(_adapter *padapter);
void rtw_ft_start_clnt_join(_adapter *padapter);
unsigned int OnAction_ft(_adapter *padapter, union recv_frame *precv_frame);
void rtw_ft_dbg_cmd(_adapter *padapter, char *s);
int rtw_ft_process_igtk_subelem(_adapter *padapter, const u8 *igtk_elem, u32 igtk_elem_len);
#endif /* CONFIG_IEEE80211R */

/*-------------------------------------------------------------------------------
	rtw_he.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_80211AX_HE
void HE_caps_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void HE_operation_update_asoc_cap(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void HE_operation_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE, u8 update);
void HE_mu_edca_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE, u8 first);
void HE_spatial_reuse_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE, u8 first);
void HE_phy_caps_update_asoc_cap(_adapter *padapter, struct rtw_phl_stainfo_t *phl_sta, u8 *ele_start);
void HE_caps_update_asoc_cap(_adapter *padapter, u8 *pIE);
u32 rtw_restructure_he_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
void rtw_he_fill_htc(u8 *phtc_buf);
#endif

/*-------------------------------------------------------------------------------
	rtw_ht.c.
--------------------------------------------------------------------------------*/
void rtw_ht_caps_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void rtw_ht_info_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
unsigned int rtw_restructure_ht_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
void rtw_update_ht_cap(_adapter *padapter, u8 *pie, uint ie_len);
int rtw_ht_operation_update(_adapter *padapter);

/*-------------------------------------------------------------------------------
	rtw_vht.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_80211AC_VHT
void rtw_vht_use_default_setting(_adapter *padapter);
void rtw_process_vht_op_mode_notify(_adapter *padapter, u8 *pframe, PVOID sta);
u32	rtw_build_vht_op_mode_notify_ie(_adapter *padapter, u8 *pbuf, u8 bw);
u32	rtw_build_vht_cap_ie(_adapter *padapter, u8 *pbuf);
u32 rtw_restructure_vht_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
unsigned int rtw_on_action_vht(_adapter *padapter, union recv_frame *precv_frame);
#endif /* CONFIG_80211AC_VHT */

/*-------------------------------------------------------------------------------
	rtw_ips.c.
--------------------------------------------------------------------------------*/
void ips_enter(_adapter *padapter);
int ips_leave(_adapter *padapter);
bool rtw_pwr_unassociated_idle(_adapter *padapter);
void rtw_ps_processor(_adapter *padapter);
u8 rtw_ps_wk_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_ps_cmd(_adapter *padapter);
int rtw_pwr_wakeup(_adapter *padapter, u32 ips_deffer_ms);
int rtw_pm_set_ips_enable(_adapter *padapter, u8 enable);

/*-------------------------------------------------------------------------------
	rtw_lps.c.
--------------------------------------------------------------------------------*/
void pwr_state_check_handler(void *FunctionContext);
u8 PS_RDY_CHECK(_adapter *padapter);
void rtw_set_ps_mode(PADAPTER padapter, u8 ps_mode, u8 smart_ps, u8 bcn_ant_mode);
s32 LPS_RF_ON_check(PADAPTER padapter, u32 delay_ms);
void LPS_Enter(PADAPTER padapter);
void LPS_Leave(PADAPTER padapter);
void LeaveAllPowerSaveMode(IN PADAPTER Adapter);
void rtw_init_pwrctrl_priv(PADAPTER padapter);
void rtw_free_pwrctrl_priv(PADAPTER padapter);
int rtw_pm_set_lps_enable(_adapter *padapter, u8 enable);
int rtw_pm_set_lps_level(_adapter *padapter, u8 level);
u8 _lps_chk_by_tp(_adapter *padapter);
u8 traffic_status_watchdog(_adapter *padapter);
void _rtw_lps_ctrl_wk_hdl(_adapter *padapter, u8 lps_ctrl_type);
u8 rtw_lps_ctrl_wk_cmd(_adapter *padapter, u8 lps_ctrl_type, u8 enqueue);
u8 rtw_lps_ctrl_wk_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);

/*-------------------------------------------------------------------------------
	rtw_twt.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_TWT
void rtw_twt_timer_enable(u8 idx, u8 port);
void rtw_twt_set_early_time(u8 idx, u8 early_time);
void rtw_twt_set_target_wake_time(u8 idx, u64 twt);
void rtw_twt_get_tsf(u8 port, u64 *cur);
unsigned int rtw_on_action_twt(_adapter *padapter, union recv_frame *precv_frame);
void rtw_issue_action_twt(_adapter *padapter, u8 setup, struct twt_ie_t *twt_ie);
#endif

/*-------------------------------------------------------------------------------
	rtw_layer2_rm.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_LAYER2_ROAMING
int is_same_ess(WLAN_BSSID_EX *a, WLAN_BSSID_EX *b);
void rtw_set_to_roam(_adapter *padapter, u8 to_roam);
u8 rtw_dec_to_roam(_adapter *padapter);
u8 rtw_to_roam(_adapter *padapter);
void rtw_roaming(_adapter *padapter, struct wlan_network *tgt_network);
void _rtw_roaming(_adapter *padapter, struct wlan_network *tgt_network);
void rtw_roaming_sitesurvey_again(_adapter *padapter);
void rtw_start_roam(_adapter *padapter);
void rtw_drv_scan_by_self(_adapter *padapter, u8 reason);
int rtw_is_desired_network(_adapter *padapter, struct wlan_network *pcur_network_new);
void rtw_roamimg_info_init(_adapter *padapter);
int rtw_select_roaming_candidate(struct mlme_priv *mlme);
#endif /* CONFIG_LAYER2_ROAMING */

/*-------------------------------------------------------------------------------
	rtw_ap.c.
--------------------------------------------------------------------------------*/
void init_mlme_ap_info(_adapter *padapter);
void free_mlme_ap_info(_adapter *padapter);
void ap_polling_sta_hdl(void *padapter);
void expire_timeout_chk(_adapter *padapter);
u8 rtw_createbss_hdl(_adapter *padapter, u8 *pbuf);
void start_bss_network(_adapter *padapter, u8 *pbuf);
u8 rtw_ap_add_sta_callback(_adapter *padapter, struct cmd_obj *cmd_obj);
void rtw_ap_add_sta_event(_adapter *padapter, unsigned char *MacAddr, int cam_idx);
void associated_clients_update(_adapter *padapter, u8 updated);
void bss_cap_update_on_sta_join(_adapter *padapter, struct sta_info *psta);
u8 bss_cap_update_on_sta_leave(_adapter *padapter, struct sta_info *psta);
u8 ap_free_sta(_adapter *padapter, struct sta_info *psta, unsigned short reason);
int rtw_sta_flush(_adapter *padapter);
void sta_info_update(_adapter *padapter, struct sta_info *psta);
int rtw_ap_start_api(rtw_softap_info_t *softAP_config, unsigned char value);

/*-------------------------------------------------------------------------------
	rtw_ap_xmit.c.
--------------------------------------------------------------------------------*/
sint rtw_ap_xmitframe_enqueue_for_sleeping_sta(_adapter *padapter, struct xmit_frame *pxmitframe);
void rtw_ap_stop_sta_xmit(_adapter *padapter, struct sta_info *psta);
void rtw_ap_wakeup_sta_to_xmit(_adapter *padapter, struct sta_info *psta);
void rtw_ap_xmit_delivery_enabled_frames(_adapter *padapter, struct sta_info *psta);

/*-------------------------------------------------------------------------------
	rtw_beacon.c.
--------------------------------------------------------------------------------*/
void rtw_on_beacon_rate_check(_adapter *padapter, struct sta_info *psta, u8 *rate, u8 rate_len, u8 *MCS_rate);
void rtw_on_beacon_info_update(_adapter *padapter, u8 *pframe, uint pkt_len, struct sta_info *psta);
int rtw_on_beacon_info_check(ADAPTER *Adapter, u8 *pframe, u32 packet_len);
unsigned int rtw_on_beacon(_adapter *padapter, union recv_frame *precv_frame);
void rtw_get_bcn_info(struct wlan_network *pcur_network);
void rtw_ap_issue_beacon(_adapter *padapter);
int rtw_ap_set_beacon(_adapter *padapter, u8 *pbuf,  int len);
int rtw_ap_generate_bcn_ie(_adapter *padapter, u8 *ssid, u16 ssid_len, u8 *ie, u32 max_ie_sz);
void rtw_ap_update_beacon(_adapter *padapter, u8 ie_id, u8 *oui, u8 tx);
u8 rtw_ap_tx_beacon_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_ap_set_tx_beacon_cmd(_adapter *padapter);

/*-------------------------------------------------------------------------------
	rtw_action.c.
--------------------------------------------------------------------------------*/
unsigned int rtw_on_action(_adapter *padapter, union recv_frame *precv_frame);
unsigned int rtw_on_action_back(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_RTW_WNM
unsigned int rtw_on_action_wnm(_adapter *padapter, union recv_frame *precv_frame);
#endif /* CONFIG_RTW_WNM */

/*-------------------------------------------------------------------------------
	rtw_addba.c.
--------------------------------------------------------------------------------*/
void rtw_on_addba_req(_adapter *padapter, u8 *paddba_req, u8 *addr);
void rtw_issue_action_ba(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short status);
unsigned int rtw_send_delba(_adapter *padapter, u8 initiator, u8 *addr);
u8 rtw_add_ba_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_addbareq_cmd(_adapter *padapter, u8 tid, u8 *addr);
void rtw_issue_addbareq_cmd(_adapter *padapter, struct xmit_frame *pxmitframe);
void rtw_addba_timer_hdl(void *FunctionContext);
void rtw_init_addba_retry_timer(_adapter *padapter, struct sta_info *psta);

/*-------------------------------------------------------------------------------
	rtw_80211w.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_IEEE80211W
void rtw_dot11w_issue_action_sa_query(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short tid, u8 key_type);
unsigned int rtw_dot11w_on_action_sa_query(_adapter *padapter, union recv_frame *precv_frame);
u8 rtw_dot11w_sta_timeout_event_callback(_adapter *padapter, struct cmd_obj *cmd_obj);
void rtw_dot11w_sa_query_timer_hdl(void *FunctionContext);
void rtw_dot11w_init_expire_timer(_adapter *padapter, struct sta_info *psta);
u32	rtw_dot11w_BIP_verify(_adapter *padapter, u8 *precvframe);
sint rtw_dot11w_validate_mgmt(_adapter *padapter, union recv_frame **pprecv_frame);
#endif

/*-------------------------------------------------------------------------------
	rtw_cmd.c.
--------------------------------------------------------------------------------*/
int rtw_cmd_filter(struct cmd_priv *pcmdpriv, struct cmd_obj *cmd_obj);
u32 rtw_enqueue_cmd(struct cmd_priv *pcmdpriv, struct cmd_obj *cmd_obj);
struct	cmd_obj	*rtw_dequeue_cmd(struct cmd_priv *pcmdpriv);
void rtw_free_cmd_obj(struct cmd_obj *pcmd);
u8 rtw_cmd_process(_adapter *padapter, void *task);
void rtw_cmd_resource_free(_adapter *padapter);

/*-------------------------------------------------------------------------------
	rtw_btcoex_soc.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_BT_COEXIST_SOC
void rtw_coex_run(_adapter *padapter);
int rtw_coex_ble_scan_duty_update(_adapter *padapter, u8 duty);
int rtw_coex_wifi_info_update(_adapter *padapter, u8 wifi_event);
void rtw_coex_bt_info_update(_adapter *padapter, u8 *mailbox_data);
sint rtw_enqueue_btmailbox(_queue *queue, struct mailbox_info *obj);
struct mailbox_info *rtw_dequeue_btmailbox(_queue *queue);
int rtw_coex_mailbox_to_wifi(_adapter *padapter, u8 *data, u8 len);
thread_return rtw_coex_mailbox_thread(thread_context context);
void rtw_coex_run_enable(_adapter *padapter, u8 enable);
char rtw_coex_pcoex_init(void);
void rtw_coex_pcoex_deinit(void);
void rtw_coex_mailbox_check(_adapter *padapter, u8 enable);
int rtw_coex_bt_enable(_adapter *padapter, u8 enable);
void rtw_coex_wifi_enable(PADAPTER padapter, u8 enable);
u16 rtw_coex_wifi_tdma_scan_start(PADAPTER padapter, RT_SCAN_TYPE scan_type);
u16 rtw_coex_wifi_tdma_scan_handle(PADAPTER padapter);
u8 rtw_coex_is_bt_disabled(PADAPTER padapter);
#endif

/*-------------------------------------------------------------------------------
	rtw_dfs.c.
--------------------------------------------------------------------------------*/
void rtw_rfctl_init(_adapter *padapter);
void rtw_rfctl_deinit(_adapter *padapter);
u8 rtw_change_bss_chbw_cmd(_adapter *padapter, int flags, u8 ifbmp, u8 excl_ifbmp, s16 req_ch, s8 req_bw, s8 req_offset);
void rtw_on_beacon_csa(_adapter *padapter, u8 *pframe, uint pkt_len);
unsigned int rtw_on_action_csa(_adapter *padapter, union recv_frame *precv_frame);
void rtw_csa_timer_hdl(void *FunctionContext);
void rtw_csa_process(_adapter *padapter, u8 new_ch_no, u8 new_ch_count);
void rtw_issue_action_csa(_adapter *padapter, unsigned char *da, unsigned char new_channel, unsigned char switch_mode, unsigned char switch_cnt);
void rtw_ap_csa_update_timer_hdl(void *FunctionContext);

/*-------------------------------------------------------------------------------
	rtw_mp.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_MP_INCLUDED
u32 read_bbreg(_adapter *padapter, u32 addr, u32 bitmask);
void write_bbreg(_adapter *padapter, u32 addr, u32 bitmask, u32 val);
u32 _read_rfreg(PADAPTER padapter, u8 rfpath, u32 addr, u32 bitmask);
void _write_rfreg(PADAPTER padapter, u8 rfpath, u32 addr, u32 bitmask, u32 val);
u32 read_rfreg(PADAPTER padapter, u8 rfpath, u32 addr);
void write_rfreg(PADAPTER padapter, u8 rfpath, u32 addr, u32 val);
s32 init_mp_priv(PADAPTER padapter);
VOID MPT_DeInitAdapter(IN PADAPTER	padapter);
void MPT_PwrCtlDM(PADAPTER padapter, u32 bstart);
s32 mp_start_test(PADAPTER padapter);
void mp_stop_test(PADAPTER padapter);
void SetBandwidth(PADAPTER padapter);
void MP_PHY_SetRFPathSwitch(PADAPTER padapter, BOOLEAN bMain);
void fill_txdesc_for_mp(PADAPTER padapter, struct tx_desc *ptxdesc);
void SetPacketTx(PADAPTER padapter);
void SetPacketRx(PADAPTER padapter, u8 bStartRx);
#if (PHYDM_VERSION == 3)
void mp_init_psd(PADAPTER padapter, struct rtw_point *wrqu);
void mp_get_point_data_psd(PADAPTER padapter, struct rtw_point *wrqu);
void mp_restore_psd(PADAPTER padapter, struct rtw_point *wrqu);
#endif
u8 MptToMgntRate(IN	u32 MptRateIdx);
u8 MgntRateToMPTRate(u8 rate);
u32 mpt_ProQueryCalTxPower(PADAPTER padapter, u8 RfPath);
int rtfloor(float x);
void ByteToBit(u8 *out, bool *in, u8	in_size);
void CRC16_generator(bool *out, bool *in, u8 in_size);
void CCK_generator(PRT_PMAC_TX_INFO pPMacTxInfo, PRT_PMAC_PKT_INFO pPMacPktInfo);
u32 LDPC_parameter_generator(u32 N_pld_int, u32 N_CBPSS, u32 N_SS, u32 R, u32 m_STBC, u32 N_TCB_int);
void PMAC_Nsym_generator(PRT_PMAC_TX_INFO pPMacTxInfo, PRT_PMAC_PKT_INFO pPMacPktInfo);
void L_SIG_generator(u32 N_SYM, PRT_PMAC_TX_INFO pPMacTxInfo, PRT_PMAC_PKT_INFO pPMacPktInfo);
void CRC8_generator(bool *out, bool *in, u8 in_size);
void HT_SIG_generator(PRT_PMAC_TX_INFO pPMacTxInfo, PRT_PMAC_PKT_INFO pPMacPktInfo);
void VHT_SIG_A_generator(PRT_PMAC_TX_INFO pPMacTxInfo, PRT_PMAC_PKT_INFO pPMacPktInfo);
void VHT_SIG_B_generator(PRT_PMAC_TX_INFO pPMacTxInfo);
void VHT_Delimiter_generator(PRT_PMAC_TX_INFO pPMacTxInfo);
void mp_FA_statistics(PADAPTER	padapter);
#endif // #ifdef CONFIG_MP_INCLUDED

/*-------------------------------------------------------------------------------
	rtw_promisc.c.
--------------------------------------------------------------------------------*/
#ifdef CONFIG_PROMISC
void promisc_deinit(_adapter *padapter);
int promisc_recv_func(_adapter *padapter, union recv_frame *rframe);
int promisc_recv_lens_func(void *padapter, u8 *payload, u8 plen);
int promisc_ctrl_packet_rpt(u8 enable);
void promisc_filter_by_ap_and_phone_mac(u8 enable, void *ap_mac, void *phone_mac);
#endif
int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
unsigned char is_promisc_enabled(void);
int promisc_get_fixed_channel(void *fixed_bssid, u8 *ssid, int *ssid_length);
void promisc_issue_probersp(unsigned char *da);
void promisc_stop_tx_beacn(void);
void promisc_resume_tx_beacn(void);
void _promisc_deinit(_adapter *padapter);
int _promisc_recv_func(_adapter *padapter, union recv_frame *rframe);
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
int _promisc_recv_lens_func(_adapter *padapter, u8 *payload, u8 plen);
#endif

/*-------------------------------------------------------------------------------
	rtw_state.c.
--------------------------------------------------------------------------------*/
int rtw_check_sta_mode(_adapter *padapter);
int rtw_check_ap_mode(_adapter *padapter);
int rtw_check_sta_associated_to_ap(_adapter *padapter);
int rtw_check_sta_linking(_adapter *padapter);
void rtw_state_set_sitesurvey_doing(_adapter *padapter, u8 state);
int rtw_check_under_sitesurvey(_adapter *padapter);
void rtw_state_set_join_doing(_adapter *padapter);
void rtw_state_set_auth_doing(_adapter *padapter);
void rtw_state_set_assoc_doing(_adapter *padapter);
void rtw_state_set_assoc_success(_adapter *padapter);
int rtw_state_check_sta_no_link(_adapter *padapter);
int rtw_state_check_sta_prepare_join(_adapter *padapter);
int rtw_state_check_sta_auth_doing(_adapter *padapter);
int rtw_state_check_sta_assoc_doing(_adapter *padapter);
void rtw_state_reset(_adapter *padapter);
void rtw_state_set_ap_beacon_on(_adapter *padapter, u8 state);
int rtw_state_check_ap_on(_adapter *padapter);
int rtw_check_ap_linked(_adapter *padapter);
void rtw_state_set_ap_client_auth_ongoing(struct sta_info *psta);
void rtw_state_set_ap_client_auth_success(struct sta_info *psta);
void rtw_state_set_ap_client_assoc_success(struct sta_info *psta);
void rtw_state_reset_ap_client_state(struct sta_info *psta);
int rtw_state_check_ap_client_auth_ongoing(struct sta_info *psta);
int rtw_state_check_ap_client_auth_success(struct sta_info *psta);
int rtw_state_check_ap_client_assoc_success(struct sta_info *psta);

/*-------------------------------------------------------------------------------
	rtw_drv_init.c: for core, rtw_intf_inter.
--------------------------------------------------------------------------------*/
void rtw_cancel_all_timer(_adapter *padapter);
u8 rtw_free_drv_sw(_adapter *padapter);
u8 rtw_reset_drv_sw(_adapter *padapter);
void rtw_init_drv_sw(_adapter *padapter);
void rtw_reset_securitypriv(_adapter *padapter);
u32 rtw_start_drv_threads(_adapter *padapter);
void rtw_stop_drv_threads(_adapter *padapter);

#endif // _RTW_ADAPTER_DEFINED_H_
#endif // _RTW_CORE_FUNCTION_H_

/*-------------------------------------------------------------------------------
	rtw_skbuff.c: for core, inic and api. Place outside _RTW_CORE_FUNCTION_H_.
--------------------------------------------------------------------------------*/
void skb_reserve(struct sk_buff *skb, unsigned int len);
void skb_assign_buf(struct sk_buff *skb, unsigned char *buf, unsigned int len);
unsigned char *skb_tail_pointer(const struct sk_buff *skb);
void skb_set_tail_pointer(struct sk_buff *skb, const int offset);
unsigned char *skb_end_pointer(const struct sk_buff *skb);
struct sk_buff *dev_alloc_skb(unsigned int length, unsigned int reserve_len);
void kfree_skb(struct sk_buff *skb);
struct sk_buff *skb_clone(struct sk_buff *skb, int gfp_mask);
struct sk_buff *skb_copy(const struct sk_buff *skb, int gfp_mask, unsigned int reserve_len);
void kfree_skb_chk_key(struct sk_buff *skb, struct net_device *root_dev);
void init_skb_pool(void);
void deinit_skb_pool(void);
int skb_fail_get_and_rst(void);
void skb_fail_inc(void);

