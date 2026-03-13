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

#include "rom_wpa_lite_intf.h"

#ifndef TRUE
#define TRUE					1
#endif
#ifndef FALSE
#define FALSE					0
#endif

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

struct wpa_global_joininfo {
	u8	AuthInfoBuf[INFO_ELEMENT_SIZE];
	struct wpa_gtk_info 	GTKInfo;
#ifdef CONFIG_IEEE80211W
	struct wpa_igkt_info	IGTKInfo;
#endif
};

// group key info
struct wpa_global {
	struct wpa_global_joininfo	*wpaglobaljoininfo;
	struct _OCTET_STRING		AuthInfoElement;
	union  _OCTET32_INTEGER		Counter;
	struct wpa_gtk_info			*GTKInfo;
#ifdef CONFIG_IEEE80211W
	struct wpa_igkt_info		*IGTKInfo;
#endif

	u32					AuthKeyMgmt;	// add for 802.11w

	u8					*AuthInfoBuf;
	u8					MulticastCipher;
	u8					transition_disable_bitmap;
	u8					transition_disable_exist;

};

struct wpa_joininfo {
	u8	AnonceBuf[KEY_NONCE_LEN];
	u8	SnonceBuf[KEY_NONCE_LEN];
	u8	eapSendBuf[MAX_EAPOLMSG_LEN];
};

// wpa sta info
struct wpa_sta_info {
	rtos_mutex_t				mutex;
	rtos_mutex_t				sae_mutex;

	struct list_head						list;

	struct wpa_joininfo			*wpajoininfo;
	struct _OCTET_STRING		ANonce;
	struct _OCTET_STRING		SNonce;
	struct _OCTET_STRING		EAPOLMsgRecvd;
	struct _OCTET_STRING		EAPOLMsgSend;
	struct _OCTET_STRING		EapolKeyMsgRecvd;
	struct _OCTET_STRING		EapolKeyMsgSend;
	struct sae_data				*sae_priv;/*for per sta*/

	struct timer_list			resendTimer;
	struct timer_list			waitTimer;	// wait for 4-1 or 2-1
	struct timer_list			sae_timer;

	union _LARGE_INTEGER		CurrentReplayCounter;
	union _LARGE_INTEGER		ReplayCounterStarted; // david+1-12-2007
	union _LARGE_INTEGER		clientMICReportReplayCounter;


	u8					UnicastCipher;
	u8					*AnonceBuf;
	u8					*SnonceBuf;
	u8					PMK[PMK_LEN_MAX];
	u8					PTK[PTK_LEN_MAX];
	u8					*eapSendBuf;
	u8					mac_addr[6];
	u8					port;
	u8 					pmksa_id[PMKID_LEN];
	u8					use_pmksa;
	s8					wpa_stainfo_state;
	s8					RSNEnabled;		// bit0-WPA, bit1-WPA2
	s8					resendCnt;
	u8					eapol_key_rsnd_limit;

	u8					b_enterprise : 1;
	u8					b_clientHndshkProcessing : 1;
	u8					b_clientHndshkDone : 1;
	u8					b_clientGkeyUpdate : 1;
	u8					b_4way_triggered_by_join : 1;
	u8 					b_pmk_ready : 1;
};

struct key_joininfo {
	u8	wappriv_psk_ssid_cfg[RTW_ESSID_MAX_SIZE + 1];
	u8	wappriv_passphrase_cfg[RTW_MAX_PSK_LEN + 1];
	u8	wappriv_global_psk_cfg[A_SHA_DIGEST_LEN * 2];
};

struct wpa_priv_t {
	struct wpa_global	wpa_global_info;
	struct key_joininfo	*keyjoininfo;

	rtos_mutex_t		sta_list_mutex;
	struct list_head  				sta_list;
	_lock				lock;

	u64					mgnt_80211w_IPN;
	union pn48			dot11txpn;

	enum rtw_security	security_type;
	u32 				dot11_wpa_mode;/*wpa/wpa2/wpa3/wpa_psk/wpa2_psk/wpa3_psk...*/
	u8					self_mac_cfg[6];/*only valid after assoc success and request 4 way handshake*/
	u8					*psk_ssid_cfg;
	u8 					*passphrase_cfg;
	u8					*global_psk_cfg;
	u8					psk_asoc_sta_count;
	u8 					psk_rsnxe_ie[RSNXE_MAX_LEN];

	u8					wpa_priv_port;
	u8					b_wpa_priv_initialized : 1;
};

enum key_type {
	KEY_PTK,
	KEY_GTK,
	KEY_IGTK,
};

static __inline__ struct _OCTET_STRING SubStr(struct _OCTET_STRING f, unsigned short s, unsigned short l)
{
	struct _OCTET_STRING res;

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
extern struct wpa_priv_t	wpa_lite[2];

__inline static void set_eapol_params(
	struct eapol_params *params,
	struct _OCTET_STRING *pEapolKeyMsgSend,
	unsigned char	KeyDescVer,
	unsigned char KeyLen,
	union _LARGE_INTEGER *pReplayCounter,
	struct _OCTET_STRING *pNonce,
	struct wpa_sta_info *pStaInfo)
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
	struct eapol_params_2 *params, unsigned char port, struct wpa_sta_info *pStaInfo)
{
	struct wpa_global *pGblInfo = &wpa_lite[port].wpa_global_info;

	params->pAuthInfoElement = &pGblInfo->AuthInfoElement;
	params->MulticastCipher = pGblInfo->MulticastCipher;
	params->pGTKInfo = pGblInfo->GTKInfo;
	params->pIGTKInfo = pGblInfo->IGTKInfo;
	params->AuthKeyMgmt = pGblInfo->AuthKeyMgmt;
	params->mgnt_80211w_IPN = &wpa_lite[port].mgnt_80211w_IPN;
	params->pCounter = &pGblInfo->Counter;
	params->pPTK = pStaInfo->PTK;
	params->dot11txpn = &wpa_lite[port].dot11txpn;
}

__inline static void set_eapol_hdr_params(
	struct eapol_hdr_params *params,
	struct _OCTET_STRING *pEAPOLMsgSend,
	struct _OCTET_STRING *pEapolKeyMsgSend,
	u8 *da, u8 *sa, int algo, u8 *PTK, int CalMIC, u32 AuthKeyMgmt)
{
	params->pEAPOLMsgSend = pEAPOLMsgSend;
	params->pEapolKeyMsgSend = pEapolKeyMsgSend;
	params->da = da;
	params->sa = sa;
	params->algo = algo;
	params->PTK = PTK;
	params->CalMIC = CalMIC;
	params->AuthKeyMgmt = AuthKeyMgmt;
}

extern const char *TAG_WLAN_WPA;

void rtw_psk_sta_send_eapol(struct wpa_sta_info *pStaInfo, struct wpa_global *pGblInfo, u8 *rsnxe_ie, int resend);
void rtw_psk_ap_send_eapol(struct wpa_sta_info	*pStaInfo, int resend);
void rtw_psk_timer_hdl(void *task_psta);
struct wpa_sta_info *rtw_psk_stainfo_get(u8 port, u8 *hwaddr);
struct wpa_sta_info *rtw_psk_stainfo_alloc(u8 port, u8 *hwaddr);
int rtw_psk_stainfo_free(u8 port, u8 *hwaddr);
void rtw_psk_set_key(u8 port, enum key_type type, struct wpa_sta_info *pStaInfo, struct wpa_global *pGblInfo);
void rtw_psk_get_global_joininfo(struct wpa_global *pGblInfo);
void rtw_psk_free_global_joininfo(struct wpa_global *pGblInfo);

#endif // _WPA_LITE_PSK_H_

