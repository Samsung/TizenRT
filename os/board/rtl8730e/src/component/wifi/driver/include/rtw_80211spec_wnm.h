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
#ifndef	__RTW_80211SPEC_WNM_H_
#define __RTW_80211SPEC_WNM_H_

#define RTW_RRM_NB_RPT_EN		BIT(1)
#define RTW_MAX_NB_RPT_NUM	16

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

/*IEEE Std 80211v,  Table 7-43b Optional Subelement IDs for Neighbor Report*/
/* BSS Transition Candidate Preference */
#define WNM_BTM_CAND_PREF_SUBEID 0x03

/* BSS Termination Duration */
#define WNM_BTM_TERM_DUR_SUBEID		0x04

struct wnm_btm_cant {
	struct nb_rpt_hdr nb_rpt;
	u8 preference;	/* BSS Transition Candidate Preference */
};

#endif

