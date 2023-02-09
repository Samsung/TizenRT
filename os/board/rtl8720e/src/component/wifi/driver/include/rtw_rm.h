#ifndef __RTW_RM_H_
#define __RTW_RM_H_

#ifdef CONFIG_IEEE80211K
#define RM_TIMER_NUM 		32
#define RM_ALL_MEAS		BIT(1)
#define RM_ID_FOR_ALL(aid)	((aid<<16)|RM_ALL_MEAS)

#define RM_CAP_ARG(x) ((u8 *)(x))[4], ((u8 *)(x))[3], ((u8 *)(x))[2], ((u8 *)(x))[1], ((u8 *)(x))[0]
#define RM_CAP_FMT "%02x %02x%02x %02x%02x"

/* remember to modify rm_event_name() when adding new event */
enum RM_EV_ID {
	RM_EV_state_in,
	RM_EV_busy_timer_expire,
	RM_EV_delay_timer_expire,
	RM_EV_meas_timer_expire,
	RM_EV_retry_timer_expire,
	RM_EV_repeat_delay_expire,
	RM_EV_request_timer_expire,
	RM_EV_wait_report,
	RM_EV_start_meas,
	RM_EV_survey_done,
	RM_EV_recv_rep,
	RM_EV_cancel,
	RM_EV_state_out,
	RM_EV_max
};

struct rm_event {
	u32 rmid;
	enum RM_EV_ID evid;
	_list list;
};

struct rm_clock {
	struct rm_obj *prm;
	ATOMIC_T counter;
	enum RM_EV_ID evid;
};

struct rm_priv {
#ifdef CONFIG_IEEE80211K
	u8 enable;
	_queue ev_queue;
	_queue rm_queue;
	_timer rm_timer;

	struct rm_clock clock[RM_TIMER_NUM];
	u8 rm_en_cap_def[5];
	u8 rm_en_cap_assoc[5];

	/* rm debug */
	void *prm_sel;

#if defined(CONFIG_RTW_WNM)
	struct roam_nb_info *nb_info;
#endif
#endif

#ifdef CONFIG_LAYER2_ROAMING
	u8 roam_flags;
	u8 roam_status;
	u8 to_roam; /* roaming trying times */
	struct wlan_network *roam_network; /* the target of active roam */
	u8 roam_rssi_diff_th; /* rssi difference threshold for active scan candidate selection */
	u8 roam_scan_int;		/* scan interval for active roam (Unit:2 second)*/
	u16 roam_scanr_exp_ms; /* scan result expire time in ms  for roam */
	u8 roam_tgt_addr[ETH_ALEN]; /* request to roam to speicific target without other consideration */
	u8 roam_rssi_threshold;
	u32 last_roaming;
	bool need_to_roam;
	u8 previous_ap[ETH_ALEN];
#endif

#ifdef CONFIG_IEEE80211R
	u8	ft_flags;
	struct ft_roam_info *ft_info;
#endif

};

void rtw_rm_enable(_adapter *padapter);
void rtw_rm_disable(_adapter *padapter);
void rtw_check_peer_rm_cap(_adapter *padapter, struct wlan_network *pnetwork);

unsigned int onAction_rm(_adapter *padapter, union recv_frame *precv_frame);
void RM_IE_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);

int rm_post_event(_adapter *padapter, u32 rmid, enum RM_EV_ID evid);
void rm_handler(_adapter *padapter, struct rm_event *pev);

u8 rm_add_nb_req(_adapter *padapter, struct sta_info *psta);
#endif
#endif /* __RTW_RM_H_ */
