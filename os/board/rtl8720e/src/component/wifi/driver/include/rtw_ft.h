#ifndef __RTW_FT_H_
#define __RTW_FT_H_
//#include <rtw_psk.h>
//#include <rtw_mlme.h>

enum rtw_ieee80211_ft_actioncode {
	RTW_WLAN_ACTION_FT_RESV,
	RTW_WLAN_ACTION_FT_REQ,
	RTW_WLAN_ACTION_FT_RSP,
	RTW_WLAN_ACTION_FT_CONF,
	RTW_WLAN_ACTION_FT_ACK,
	RTW_WLAN_ACTION_FT_MAX,
};

enum _rtw_ft_sta_status {
	RTW_FT_UNASSOCIATED_STA = 0,
	RTW_FT_AUTHENTICATING_STA,
	RTW_FT_AUTHENTICATED_STA,
	RTW_FT_ASSOCIATING_STA,
	RTW_FT_ASSOCIATED_STA,
	RTW_FT_REQUESTING_STA,
	RTW_FT_REQUESTED_STA,
	RTW_FT_CONFIRMED_STA,
	RTW_FT_UNSPECIFIED_STA
};

enum rtw_ft_capability {
	RTW_FT_EN = BIT(0),
	RTW_FT_OTD_EN = BIT(1),
	RTW_FT_PEER_EN = BIT(2),
	RTW_FT_PEER_OTD_EN = BIT(3),
};

#define rtw_ft_chk_status(padapter, s) \
	((padapter->pshare_adapter)->rmpriv->ft_info->ft_status == (s))

#define rtw_ft_roam_status(a, s)	\
	((rtw_to_roam(a) > 0) && rtw_ft_chk_status(a, s))

#define rtw_ft_authed_sta(a)	\
	((rtw_ft_chk_status(a, RTW_FT_AUTHENTICATED_STA)) ||	\
	(rtw_ft_chk_status(a, RTW_FT_ASSOCIATING_STA)) ||	\
	(rtw_ft_chk_status(a, RTW_FT_ASSOCIATED_STA)))

#define rtw_ft_set_status(padapter, s) \
	do { \
		((padapter->pshare_adapter)->rmpriv->ft_info->ft_status = (s)); \
	} while (0)

#define rtw_ft_lock_set_status(padapter, s, irq) \
	do { \
		rtw_enter_critical_bh(&rtw_get_mlmepriv(a)->lock, ((_irqL *)(irq)));	\
		((padapter->pshare_adapter)->rmpriv->ft_info->ft_status = (s));	\
		rtw_exit_critical_bh(&rtw_get_mlmepriv(a)->lock, ((_irqL *)(irq)));	\
	} while (0)

#define rtw_ft_reset_status(padapter) \
	do { \
		((padapter->pshare_adapter)->rmpriv->ft_info->ft_status = RTW_FT_UNASSOCIATED_STA); \
	} while (0)


#define rtw_ft_chk_flags(padapter, f) \
	((padapter->pshare_adapter)->rmpriv->ft_flags & (f))

#define rtw_ft_set_flags(padapter, f) \
	do { \
		((padapter->pshare_adapter)->rmpriv->ft_flags |= (f)); \
	} while (0)

#define rtw_ft_clr_flags(padapter, f) \
	do { \
		((padapter->pshare_adapter)->rmpriv->ft_flags &= ~(f)); \
	} while (0)

#define rtw_ft_roam(a)	\
	((rtw_to_roam(a) > 0) && rtw_ft_chk_flags(a, RTW_FT_PEER_EN))

#define rtw_ft_valid_akm(a, t)	\
	((rtw_ft_chk_flags(a, RTW_FT_EN)) && \
	(((t) == WPA_KEY_MGMT_FT_IEEE8021X) || ((t) == WPA_KEY_MGMT_FT_PSK)))

#define rtw_ft_otd_roam_en(padapter)	\
	((rtw_ft_chk_flags(a, RTW_FT_OTD_EN))	\
	&& ((padapter->pshare_adapter)->rmpriv->ft_info->ft_cap & 0x01))

#define rtw_ft_otd_roam(a) \
	(rtw_ft_chk_flags(a, RTW_FT_OTD_EN) && rtw_ft_chk_flags(a, RTW_FT_PEER_OTD_EN))

#define rtw_ft_valid_otd_candidate(a, p)	\
	((rtw_ft_chk_flags(a, RTW_FT_OTD_EN)) 	\
	&& ((rtw_ft_chk_flags(a, RTW_FT_PEER_OTD_EN)	\
	&& ((*((p)+4) & 0x01) == 0))	\
	|| ((rtw_ft_chk_flags(a, RTW_FT_PEER_OTD_EN) == 0)	\
	&& (*((p)+4) & 0x01))))

#define FTIE_SUBELEM_R1KH_ID 1
#define FTIE_SUBELEM_GTK 2
#define FTIE_SUBELEM_R0KH_ID 3

#define RTW_FT_ACTION_REQ_LMT	4
#define RTW_FT_MAX_IE_SZ	256

#define PMK_LEN_MAX 64
#define MOBILITY_DOMAIN_ID_LEN 2
#define FT_R0KH_ID_MAX_LEN 48
#define FT_R1KH_ID_LEN 6
#define WPA_PMK_NAME_LEN 16
#define ASSOC_RESP_IE_MAX_LEN 256

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int has_pairwise;
	int group_cipher;
	int has_group;
	int key_mgmt;
	int capabilities;
	u32 num_pmkid;
	u8 *pmkid;
	int mgmt_group_cipher;
};

struct ft_ie_parse {
	u8 *mdie;
	u32 mdie_len;
	u8 *ftie;
	u32 ftie_len;
	u8 *r1kh_id;
	u8 *gtk;
	u32 gtk_len;
	u8 *r0kh_id;
	u32 r0kh_id_len;
	u8 *rsn;
	u32 rsn_len;
	u8 *rsn_pmkid;
	u8 *tie;
	u32 tie_len;
	u8 *igtk;
	u32 igtk_len;
	u8 *ric;
	u32 ric_len;
	int key_mgmt;
	int pairwise_cipher;
};

struct ft_roam_info {
	u16 mdid;
	u8 ft_cap;
	u8	assoc_resp_ies[RTW_FT_MAX_IE_SZ];/* MDIE FTIE from Association Response */
	u32	assoc_resp_ies_len;
	u32 ft_status;
	u32 ft_req_retry_cnt;
	u32 reassoc_deadline;
	u32 key_lifetime;
	u8 snonce[KEY_NONCE_LEN]; //snonce in ft auth/reassoc
	u8 anonce[KEY_NONCE_LEN]; //anonce in ft auth/reassoc
	u8 pmk_r0[PMK_LEN_MAX];
	u32 pmk_r0_len;
	u8 pmk_r0_name[WPA_PMK_NAME_LEN];
	u8 pmk_r1[PMK_LEN_MAX];
	u32 pmk_r1_len;
	u8 pmk_r1_name[WPA_PMK_NAME_LEN];
	u8	ptk[PTK_LEN_TKIP];
	u8	gtk[GTK_LEN];
	u8	gtk_keyid;
	u8 r0kh_id[FT_R0KH_ID_MAX_LEN];
	u32 r0kh_id_len;
	u8 r1kh_id[FT_R1KH_ID_LEN];
#ifdef CONFIG_IEEE80211W
	u8	igtk[WPA_IGTK_MAX_LEN];
	u32	igtk_len;
	u32 igtk_keyid;
#endif
};

struct ft_ftie {
	u8 mic_control[2];
	u8 mic[16];
	u8 anonce[KEY_NONCE_LEN];
	u8 snonce[KEY_NONCE_LEN];
	/* followed by optional parameters */
};

struct ft_mdie {
	u8 mobility_domain[MOBILITY_DOMAIN_ID_LEN];
	u8 ft_capab;
};

struct ft_rdie {
	u8 id;
	u8 descr_count;
	u16 status_code;
};

struct rsn_ie_hdr {
	u8 elem_id; /* WLAN_EID_RSN */
	u8 len;
	u8 version[2]; /* little endian */
};

void rtw_ft_info_init(_adapter *padapter);

void rtw_ft_info_deinit(_adapter *padapter);

void rtw_ft_enable(_adapter *padapter);

void rtw_ft_disable(_adapter *padapter);

int rtw_ft_set_key(_adapter *padapter, struct sta_info *psta);

int rtw_ft_chk_roaming_candidate(_adapter *padapter, struct wlan_network *competitor);

void rtw_ft_validate_akm_type(_adapter  *padapter, struct wlan_network *pnetwork);

void rtw_ft_start_clnt_join(_adapter *padapter);

void rtw_ft_build_eapol_2_ies(_adapter *padapter, OCTET_STRING *EapolKeyMsgSend);

void rtw_ft_build_auth_req_ies(_adapter *padapter, struct pkt_attrib *pattrib, u8 **pframe);

u8 rtw_ft_build_assoc_req_ies(_adapter *padapter, u8 is_reassoc, struct pkt_attrib *pattrib, u8 **pframe);

void rtw_ft_start_roam(_adapter *padapter, u8 *pTargetAddr);

void rtw_ft_link_timer_hdl(void *FunctionContext);

void rtw_ft_roam_timer_hdl(void *FunctionContext);

void rtw_ft_roam_status_reset(_adapter *padapter);

unsigned int OnAction_ft(_adapter *padapter, union recv_frame *precv_frame);
#endif /* __RTW_FT_H_ */
