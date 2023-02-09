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
#ifndef __RTW_80211FRAME_H_
#define __RTW_80211FRAME_H_

extern const u8 WMM_OUI[];
extern const u8 WPS_OUI[];
extern const u8 WFD_OUI[];
extern const u8 P2P_OUI[];

//extern const unsigned char WMM_INFO_OUI[];
extern const u8	WMM_PARA_OUI[];

struct mlme_handler {
	unsigned int   num;
	unsigned int (*func)(_adapter *padapter, union recv_frame *precv_frame);
};

struct action_handler {
	unsigned int   num;
	unsigned int (*func)(_adapter *padapter, union recv_frame *precv_frame);
};


/* WFA Transition Disable KDE (using OUI_WFA) */
/* Transition Disable Bitmap bits */
#define TRANSITION_DISABLE_WPA3_PERSONAL BIT(0)
#define TRANSITION_DISABLE_SAE_PK BIT(1)
#define TRANSITION_DISABLE_WPA3_ENTERPRISE BIT(2)
#define TRANSITION_DISABLE_ENHANCED_OPEN BIT(3)

void update_mgntframe_attrib(_adapter *padapter, struct pkt_attrib *pattrib);
int dump_mgntframe(_adapter *padapter, struct xmit_frame *pmgntframe);

void issue_beacon(_adapter *padapter);
void issue_probersp(_adapter *padapter, unsigned char *da, u8 is_valid_p2p_probereq);
void issue_assocreq(_adapter *padapter, u8 is_reassoc);
void issue_asocrsp(_adapter *padapter, unsigned short status, struct sta_info *pstat, int pkt_type);
void issue_auth(_adapter *padapter, struct sta_info *psta, unsigned short status);
//	Added by Albert 2010/07/26
//	blnbc: 1 -> broadcast probe request
//	blnbc: 0 -> unicast probe request. The address 1 will be the BSSID.
void issue_probereq(_adapter *padapter, NDIS_802_11_SSID *pssid, u8 blnbc);
void issue_nulldata(_adapter *padapter, unsigned int power_mode);
void issue_qos_nulldata(_adapter *padapter, unsigned char *da, u16 tid);
void issue_deauth(_adapter *padapter, unsigned char *da, u32 reason);

void issue_action_BA(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short status);
#ifdef CONFIG_IEEE80211W
void issue_action_SA_Query(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short tid, u8 key_type);
#endif

unsigned int send_delba(_adapter *padapter, u8 initiator, u8 *addr);

unsigned int OnAssocReq(_adapter *padapter, union recv_frame *precv_frame);
void OnAssocRsp_HT(_adapter *padapter);
void OnAssocRsp_WMM(_adapter *padapter);
unsigned int OnAssocRsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnProbeReq(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnProbeRsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int DoReserved(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_DFS
void OnBeacon_csa(_adapter *padapter, u8 *pframe, uint len);
#endif //CONFIG_DFS
unsigned int OnBeacon(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAtim(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnDisassoc(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAuth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAuthClient(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnDeAuth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction(_adapter *padapter, union recv_frame *precv_frame);

unsigned int OnAction_qos(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_dls(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_back(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_public(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_ht(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_IEEE80211W
unsigned int OnAction_sa_query(_adapter *padapter, union recv_frame *precv_frame);
#endif
#ifdef CONFIG_RTW_WNM
unsigned int OnAction_wnm(_adapter *adapter, union recv_frame *precv_frame);
#endif
unsigned int OnAction_wmm(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_csa(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_TWT
void issue_action_twt(_adapter *padapter, u8 setup, struct twt_ie_t *twt_ie);
unsigned int OnAction_twt(_adapter *padapter, union recv_frame *precv_frame);
#endif
#ifdef CONFIG_80211AC_VHT
unsigned int OnAction_vht(_adapter *padapter, union recv_frame *precv_frame);
#endif


#endif

