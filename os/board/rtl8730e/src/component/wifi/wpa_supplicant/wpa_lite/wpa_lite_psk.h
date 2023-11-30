/*
 *  Header files of WPA PSK module
 *
 *  $Id: 8192cd_psk.h,v 1.1 2009/11/06 12:26:48 victoryman Exp $
 *
 *  Copyright (c) 2009 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#ifndef _WPA_LITE_PSK_H_
#define _WPA_LITE_PSK_H_

#ifndef TRUE
#define TRUE					1
#endif
#ifndef FALSE
#define FALSE					0
#endif

/*
	2008-12-16, For Corega CG-WLCB54GL 54Mbps NIC interoperability issue.
	The behavior of this NIC when it connect to the other AP with WPA/TKIP is:
		AP	<----------------------> 	STA
			....................
			------------> Assoc Rsp (ok)
			------------> EAPOL-key (4-way msg 1)
			<------------ unknown TKIP encryption data
			------------> EAPOL-key (4-way msg 1)
			<------------ unknown TKIP encryption data
			.....................
			<------------ disassoc (code=8, STA is leaving) when the 5 seconds timer timeout counting from Assoc_Rsp is got.
			....................
			------------> Assoc Rsp (ok)
			<-----------> EAPOL-key (4-way handshake success)

	If MAX_RESEND_NUM=3, our AP will send disassoc (code=15, 4-way timeout) to STA before STA sending disassoc to AP.
	And this NIC will always can not connect to our AP.
	set MAX_RESEND_NUM=5 can fix this issue.
 */
//#define MAX_RESEND_NUM	3
#define MAX_RESEND_NUM		5

/*
	Revise STA EAPOL-key(4-2) resend time from "RESEND_TIME" to "CLIENT_RESEND_TIME" to make sure one 4-1 followed
	by only one 4-2 but not 'several' to fix:
	As to AP "HUAWEI WS5100", in case of situation that if STA (re)connect to the AP with a wrong password for several times
	and in each (re)connect the 4-2 msg with the wrong pw info (re)sent more than once in response to every 4-1, then the AP will
	probably put the STA into blacklist and won't respond to STA's AUTH again even the STA may has corrected the password.

	AP(WS5100)	<-----------> 	STA
					....................
					------------> Assoc Rsp (ok)
					------------> EAPOL-key (4-1)
					<------------ EAPOL-key (4-2 with wrong pw info)
					<------------ EAPOL-key (resend 4-2 with wrong pw info)

					------------> EAPOL-key (4-way msg 1)
					<------------ EAPOL-key (4-2 with wrong pw info)
					<------------ EAPOL-key (resend 4-2 with wrong pw info)
					....................
*/
#define RESEND_TIME			1000
#define CLIENT_RESEND_TIME		1200
#define WAIT_TIME			7000

#define GK_REKEY_TIME		3600000		//Set rekey period to 1 hour

enum {
	PSK_STATE_IDLE,
	PSK_STATE_PTKSTART,
	PSK_STATE_PTKINITNEGOTIATING,
	PSK_STATE_PTKINITDONE,
};

enum {
	PSK_GSTATE_REKEYNEGOTIATING,
	PSK_GSTATE_REKEYESTABLISHED,
	PSK_GSTATE_KEYERROR,
};


// group key info
typedef struct _wpa_global_info {
	OCTET32_INTEGER		Counter;
	OCTET_STRING		AuthInfoElement;
	unsigned char		AuthInfoBuf[INFO_ELEMENT_SIZE];
	unsigned char		MulticastCipher;
	unsigned int		AuthKeyMgmt;	// add for 802.11w
	OCTET_STRING		GNonce;
	unsigned char		GNonceBuf[KEY_NONCE_LEN];

	struct wpa_gtk_info GTKInfo;

#ifdef CONFIG_IEEE80211W
	struct wpa_igkt_info IGTKInfo;
#endif
	u8					transition_disable_bitmap;
	u8					transition_disable_exist;

} WPA_GLOBAL_INFO;

// wpa sta info
typedef struct _wpa_sta_info {
	_lock				lock;
	_list				list;
	int					state;
	int					gstate;
	int					RSNEnabled;		// bit0-WPA, bit1-WPA2
	int					PInitAKeys;
	unsigned char		UnicastCipher;
	LARGE_INTEGER		CurrentReplayCounter;
	LARGE_INTEGER		ReplayCounterStarted; // david+1-12-2007
	OCTET_STRING		ANonce;
	OCTET_STRING		SNonce;
	unsigned char		AnonceBuf[KEY_NONCE_LEN];
	unsigned char		SnonceBuf[KEY_NONCE_LEN];
	unsigned char		PMK[PMK_LEN];
	unsigned char		PTK[PTK_LEN_TKIP];
	OCTET_STRING		EAPOLMsgRecvd;
	OCTET_STRING		EAPOLMsgSend;
	OCTET_STRING		EapolKeyMsgRecvd;
	OCTET_STRING		EapolKeyMsgSend;

	unsigned char		eapSendBuf[MAX_EAPOLMSG_LEN];
//	unsigned char		eapRecvdBuf[MAX_EAPOLMSG_LEN];
	struct timer_list	resendTimer;
	int					resendCnt;
	struct timer_list	waitTimer;	// wait for 4-1 or 2-1
	int					clientHndshkProcessing;
	int					clientHndshkDone;
	int 				clientGkeyUpdate;
	LARGE_INTEGER		clientMICReportReplayCounter;
	u8					mac_addr[6];
	u8					port;
	u8					is_enterprise;
	u8 					pmksa_id[PMKID_LEN];
	u8					use_pmksa;

	struct				sae_data *sae_priv;/*for per sta*/
	_lock				sae_lock;
	struct timer_list	sae_timer;
} WPA_STA_INFO;

struct wpa_priv_t {
	_lock					sta_list_lock;
	_list  					sta_list;
	_lock					lock;
	int					asoc_sta_count;
	WPA_GLOBAL_INFO		wpa_global_info;
	u32 				ndisauthtype;/*wpa/wpa2/wpa3/wpa_psk/wpa2_psk/wpa3_psk...*/
	u8 					rsnxe_ie[RSNXE_MAX_LEN];
	union pn48			dot11txpn;/*WPA_TODO, delete this when rom code move to AP*/
	u64					mgnt_80211w_IPN;/*WPA_TODO, delete this when rom code move to AP*/
	u8					self_macaddr[6];/*only valid after assoc success and request 4 way handshake*/
	u8					port;
	u8					initialized;
	u8					ssid[RTW_ESSID_MAX_SIZE + 4];
	u8 					passphrase[RTW_PASSPHRASE_MAX_SIZE + 1];
	u8					global_psk[A_SHA_DIGEST_LEN * 2];
	u8					global_pmk[PMK_LEN];
	rtw_security_t      security_type;
};

enum key_type {
	KEY_PTK,
	KEY_GTK,
	KEY_IGTK,
};

static __inline__ OCTET_STRING SubStr(OCTET_STRING f, unsigned short s, unsigned short l)
{
	OCTET_STRING res;

	res.Length = l;
	res.Octet = f.Octet + s;

	return res;
}

#define LargeIntegerZero(x)				memset(&x.charData, 0, 8)
#define Octet16IntegerZero(x)				memset(&x.charData, 0, 16)
#define SetNonce(ocDst, oc32Counter)		wifi_rom_set_eapolkeyiv(ocDst, oc32Counter)

#if defined(CONFIG_IEEE80211W) || defined(CONFIG_SAE_SUPPORT)
extern const unsigned char igtk_expansion_const[];
#endif
extern struct wpa_priv_t	wpa_lite[MAX_IFACE_PORT];

__inline static void set_eapol_params(
	struct eapol_params *params,
	OCTET_STRING *pEapolKeyMsgSend,
	unsigned char	KeyDescVer,
	unsigned char KeyLen,
	LARGE_INTEGER *pReplayCounter,
	OCTET_STRING *pNonce,
	WPA_STA_INFO *pStaInfo)
{
	params->pEapolKeyMsgSend = pEapolKeyMsgSend;
	params->KeyDescVer = KeyDescVer;
	params->KeyLen = KeyLen;
	params->pReplayCounter = pReplayCounter;
	params->pNonce = pNonce;
	params->pEapolKeyMsgRecvd = &(pStaInfo->EapolKeyMsgRecvd);
	params->RSNEnabled = pStaInfo->RSNEnabled;
	params->use_pmksa = pStaInfo->use_pmksa;
	params->pmkid = pStaInfo->pmksa_id;
}

__inline static void set_eapol_params_2(
	struct eapol_params_2 *params, unsigned char port, WPA_STA_INFO *pStaInfo)
{
	WPA_GLOBAL_INFO *pGblInfo = &wpa_lite[port].wpa_global_info;

	params->pAuthInfoElement = &pGblInfo->AuthInfoElement;
	params->MulticastCipher = pGblInfo->MulticastCipher;
	params->pGTKInfo = &pGblInfo->GTKInfo;
	params->pIGTKInfo = &pGblInfo->IGTKInfo;
	params->AuthKeyMgmt = pGblInfo->AuthKeyMgmt;
	params->mgnt_80211w_IPN = &wpa_lite[port].mgnt_80211w_IPN;
	params->pCounter = &pGblInfo->Counter;
	params->pPTK = pStaInfo->PTK;
	params->dot11txpn = &wpa_lite[port].dot11txpn;
}

__inline static void set_eapol_hdr_params(
	struct eapol_hdr_params *params,
	OCTET_STRING *pEAPOLMsgSend,
	OCTET_STRING *pEapolKeyMsgSend,
	u8 *da, u8 *sa, int algo, u8 *PTK, int CalMIC)
{
	params->pEAPOLMsgSend = pEAPOLMsgSend;
	params->pEapolKeyMsgSend = pEapolKeyMsgSend;
	params->da = da;
	params->sa = sa;
	params->algo = algo;
	params->PTK = PTK;
	params->CalMIC = CalMIC;
}

void rtw_psk_sta_start_4way(char *buf, int buf_len, int flags, void *userdata);
void rtw_psk_sta_send_eapol(WPA_STA_INFO *pStaInfo, WPA_GLOBAL_INFO *pGblInfo, u8 *rsnxe_ie, int resend);
void rtw_psk_sta_recv_eapol(char *data, int data_len, int flags, void *userdata);
void rtw_psk_ap_start_4way(char *buf, int buf_len, int flags, void *userdata);
void rtw_psk_ap_send_eapol(WPA_STA_INFO	*pStaInfo, int resend);
void rtw_psk_ap_recv_eapol(char *data, int data_len, int flags, void *userdata);
void rtw_psk_set_psk_info_evt_hdl(char *buf, int buf_len, int flags, void *userdata);
void rtw_psk_timer_hdl(void *task_psta);
WPA_STA_INFO *rtw_psk_stainfo_get(u8 port, u8 *hwaddr);
WPA_STA_INFO *rtw_psk_stainfo_alloc(u8 port, u8 *hwaddr);
u32 rtw_psk_stainfo_free(u8 port, u8 *hwaddr);

#endif // _WPA_LITE_PSK_H_

