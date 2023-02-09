#ifndef __RTW_WNM_H_
#define __RTW_WNM_H_

#define WNM_SUPPORT_IWPRIV_DBG 1
#define RTW_RRM_NB_RPT_EN		BIT(1)
#define RTW_MAX_NB_RPT_NUM	16

#define rtw_roam_busy_scan(a, nb)	\
	(nb && (rtw_get_mlmepriv(a)->LinkDetectInfo.bBusyTraffic == _TRUE) && \
	(((nb)->ch_cnt) < ((nb)->nb_rpt_ch_list_num)))

#define rtw_wnm_btm_preference_cap(a) \
	(rtw_get_rmpriv(a)->nb_info && rtw_get_rmpriv(a)->nb_info->preference_en == _TRUE)

#define rtw_btm_roam(a)	\
	((rtw_to_roam(a) > 0) && rtw_chk_roam_flags(a, RTW_ROAM_ON_BTM) \
	&& (rtw_get_rmpriv(a)->roam_status == RTW_ROAM_BTM_RUNNING))

/* target Bssid can not be own mac address*/
#define rtw_wnm_btm_diff_bss(a) \
	((rtw_wnm_btm_preference_cap(a)) && \
	(is_zero_mac_addr(rtw_get_rmpriv(a)->nb_info->roam_target_addr) == _FALSE) && \
	(rtw_memcmp(rtw_get_rmpriv(a)->nb_info->roam_target_addr,\
		rtw_get_bssid(a), ETH_ALEN) == _FALSE)  && \
	(rtw_memcmp(rtw_get_rmpriv(a)->nb_info->roam_target_addr,\
		(a)->mac_addr,ETH_ALEN) == _FALSE))


#define rtw_wnm_btm_roam_candidate(a, c) \
	((rtw_wnm_btm_preference_cap(a)) && \
	(is_zero_mac_addr(rtw_get_rmpriv(a)->nb_info->roam_target_addr) == _FALSE) && \
	(rtw_memcmp(rtw_get_rmpriv(a)->nb_info->roam_target_addr,\
		(c)->network.MacAddress, ETH_ALEN)))

#define rtw_wnm_set_ext_cap_btm(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart))+2, 3, 1, _val)

#define wnm_btm_bss_term_inc(p) (*((u8 *)((p)+3)) & BSS_TERMINATION_INCLUDED)

#define wnm_btm_ess_disassoc_im(p) (*((u8 *)((p)+3)) & ESS_DISASSOC_IMMINENT)

#define wnm_btm_dialog_token(p) (*((u8 *)((p)+2)))

#define wnm_btm_req_mode(p) (*((u8 *)((p)+3)))

#define wnm_btm_disassoc_timer(p) (*((u16 *)((p)+4)))

#define wnm_btm_valid_interval(p) (*((u8 *)((p)+6)))

#define wnm_btm_term_duration_offset(p) ((p)+7)

#define RTW_WLAN_ACTION_WNM_NB_RPT_ELEM	0x34

enum rtw_ieee80211_wnm_actioncode {
	RTW_WLAN_ACTION_WNM_BTM_QUERY = 6,
	RTW_WLAN_ACTION_WNM_BTM_REQ = 7,
	RTW_WLAN_ACTION_WNM_BTM_RSP = 8,
	RTW_WLAN_ACTION_WNM_NOTIF_REQ = 26,
	RTW_WLAN_ACTION_WNM_NOTIF_RSP = 27,
};

/*IEEE Std 80211k Figure 7-95b Neighbor Report element format*/
struct nb_rpt_hdr {
	u8 id; /*0x34: Neighbor Report Element ID*/
	u8 len;
	u8 bssid[ETH_ALEN];
	u32 bss_info;
	u8 reg_class;
	u8 ch_num;
	u8 phy_type;
};

/*IEEE Std 80211v, Figure 7-9 BSS Termination Duration subelement field format */
struct btm_term_duration {
	u8 id;
	u8 len;
	u64 tsf;		/* value of the TSF counter when BSS termination will occur in the future */
	u16 duration;		/* number of minutes for which the BSS is not present*/
};

/*IEEE Std 80211v, Figure 7-10 BSS Transition Management Request frame body format */
struct btm_req_hdr {
	u8 dialog_token;
	u8 req_mode;
	/* number of TBTTs until the AP sends a Disassociation frame to this STA */
	u16 disassoc_timer;
	/* number of TBTTs until the BSS transition candidate list is no longer valid */
	u8 validity_interval;
	struct btm_term_duration term_duration;
};

struct btm_rpt_cache {
	u8 dialog_token;
	u8 req_mode;
	u16 disassoc_timer;
	u8 validity_interval;
	struct btm_term_duration term_duration;

	/* from BTM req */
	u32 validity_time;
	u32 disassoc_time;

	systime req_stime;
};

/*IEEE Std 80211v,  Table 7-43b Optional Subelement IDs for Neighbor Report*/
/* BSS Transition Candidate Preference */
#define WNM_BTM_CAND_PREF_SUBEID 0x03

/* BSS Termination Duration */
#define WNM_BTM_TERM_DUR_SUBEID		0x04

struct wnm_btm_cant {
	struct nb_rpt_hdr nb_rpt;
	u8 preference;	/* BSS Transition Candidate Preference */
};

enum rtw_btm_req_mod {
	PREFERRED_CANDIDATE_LIST_INCLUDED = BIT(0),
	ABRIDGED = BIT(1),
	DISASSOC_IMMINENT = BIT(2),
	BSS_TERMINATION_INCLUDED = BIT(3),
	ESS_DISASSOC_IMMINENT = BIT(4),
};

struct roam_nb_info {
	struct nb_rpt_hdr nb_rpt[RTW_MAX_NB_RPT_NUM];
	struct rtw_ieee80211_channel nb_rpt_ch_list[RTW_MAX_NB_RPT_NUM];
	struct btm_rpt_cache btm_cache;
	bool	nb_rpt_valid;
	u8	nb_rpt_ch_list_num;
	u8 preference_en;
	u8 roam_target_addr[ETH_ALEN];
	u32	last_nb_rpt_entries;
	u8 nb_rpt_is_same;
	s8 disassoc_waiting;
	u8 ch_cnt;
	_timer roam_scan_timer;
	_timer disassoc_chk_timer;
};

u8 rtw_wnm_btm_reassoc_req(_adapter *padapter);
void rtw_wnm_roam_scan_hdl(void *ctx);
void rtw_wnm_disassoc_chk_hdl(void *ctx);
u8 rtw_wnm_try_btm_roam_imnt(_adapter *padapter);
void rtw_wnm_process_btm_req(_adapter *padapter,  u8 *pframe, u32 frame_len);
void rtw_wnm_reset_btm_candidate(struct roam_nb_info *pnb);
void rtw_wnm_reset_btm_state(_adapter *padapter);
void rtw_wnm_reset_btm_cache(_adapter *padapter);
void rtw_wnm_issue_action(_adapter *padapter, u8 action, u8 reason, u8 dialog);
void rtw_wnm_update_reassoc_req_ie(_adapter *padapter);

void rtw_roam_nb_info_init(_adapter *padapter);
u8 rtw_roam_nb_scan_list_set(
	_adapter *padapter, struct sitesurvey_parm *pparm);
u32 rtw_wnm_btm_candidates_survey(_adapter *padapter,
								  u8 *pframe, u32 elem_len, u8 is_preference);
#endif /* __RTW_WNM_H_ */

