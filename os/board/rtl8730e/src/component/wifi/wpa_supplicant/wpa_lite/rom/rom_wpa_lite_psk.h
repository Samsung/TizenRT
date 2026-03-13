/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This is ROM code section.
 *
 *
 ******************************************************************************/
#ifndef _ROM_WPA_LITE_PSK_H_
#define _ROM_WPA_LITE_PSK_H_

#define PMK_EXPANSION_CONST				"Pairwise key expansion"
#define PMK_EXPANSION_CONST_SIZE		22
#define GMK_EXPANSION_CONST				"Group key expansion"
#define GMK_EXPANSION_CONST_SIZE		19
#define IGTK_EXPANSION_CONST				"IGTK key expansion"
#define RANDOM_EXPANSION_CONST			"Init Counter"
#define RANDOM_EXPANSION_CONST_SIZE		12

#define GMK_LEN					32
#define GTK_LEN					32
#define IGTK_LEN				16
#define WPA_IGTK_MAX_LEN		32
#define NumGroupKey				4
#define KEY_NONCE_LEN		32
#define KEY_RC_LEN				8
#define KEY_IV_LEN				16
#define KEY_RSC_LEN				8
#define KEY_ID_LEN				8
#define KEY_MIC_LEN				16
#define KEY_MIC_LEN_SUITE_B_192             24
#define KEY_MATERIAL_LEN		2
#define PTK_LEN_EAPOLMIC        16
#define PTK_LEN_EAPOLMIC_SUITE_B_192        24
#define PTK_LEN_EAPOLENC        16
#define PTK_LEN_EAPOLENC_SUITE_B_192        32
#define PTK_LEN_TKIP           	64
#define PTK_LEN_CCMP            48

#define PTK_LEN_GCMP_256        88
#define PTK_LEN_MAX             128

#define EAPOLMSG_HDRLEN			95      //EAPOL-key payload length without KeyData
#define EAPOLMSG_HDRLEN_SUITE_B_192		103  //diff=8bytes MIC(16->24)
#define MAX_EAPOLMSG_LEN        512
#define MAX_EAPOLKEYMSG_LEN		(MAX_EAPOLMSG_LEN-(ETH_HLEN+LIB1X_EAPOL_HDRLEN))

#define DescTypePos				0
#define KeyInfoPos				1
#define KeyLenPos				3
#define ReplayCounterPos		5
#define KeyNoncePos				13
#define KeyIVPos				45
#define KeyRSCPos				61
#define KeyIDPos				69
#define KeyMICPos				77
#define KeyDataLenPos			93
#define KeyDataLenPos_SUITE_B_192			101
#define KeyDataPos				95
#define KeyDataPos_SUITE_B_192				103

#define SetSubStr(f,a,l)					memcpy(f.Octet+l,a.Octet,a.Length)
#define GetKeyInfo0(f, mask) 				((f.Octet[KeyInfoPos + 1] & mask) ? 1 : 0)
#define SetKeyInfo0(f,mask,b)				(f.Octet[KeyInfoPos + 1] = (f.Octet[KeyInfoPos + 1] & ~mask) | ( b?mask:0x0) )
#define GetKeyInfo1(f, mask)				((f.Octet[KeyInfoPos] & mask) ? 1 : 0)
#define SetKeyInfo1(f,mask,b)				(f.Octet[KeyInfoPos] = (f.Octet[KeyInfoPos] & ~mask) | ( b?mask:0x0) )

// EAPOLKey
#define Message_DescType(f)				(f.Octet[DescTypePos])
#define Message_setDescType(f, type)		(f.Octet[DescTypePos] = type)
// Key Information Filed
#define Message_KeyDescVer(f)			(f.Octet[KeyInfoPos+1] & 0x07)
#define Message_setKeyDescVer(f, v)		(f.Octet[KeyInfoPos+1] &= 0xf8) , f.Octet[KeyInfoPos+1] |= (v & 0x07)
#define Message_KeyType(f)				GetKeyInfo0(f, 0x08)
#define Message_setKeyType(f, b)			SetKeyInfo0(f,0x08,b)
#define Message_KeyIndex(f)				((f.Octet[KeyInfoPos+1] & 0x30) >> 4) & 0x03
#define Message_setKeyIndex(f, v)			(f.Octet[KeyInfoPos+1] &= 0xcf), f.Octet[KeyInfoPos+1] |= ((v<<4) & 0x30)
#define Message_setInstall(f, b)			SetKeyInfo0(f,0x40,b)
#define Message_setKeyAck(f, b)			SetKeyInfo0(f,0x80,b)

#define Message_KeyMIC(f)				GetKeyInfo1(f, 0x01)
#define Message_setKeyMIC(f, b)			SetKeyInfo1(f,0x01,b)
#define Message_Secure(f)				GetKeyInfo1(f,0x02)
#define Message_setSecure(f, b)			SetKeyInfo1(f,0x02,b)
#define Message_Error(f)					GetKeyInfo1(f,0x04)
#define Message_setError(f, b)				SetKeyInfo1(f,0x04,b)
#define Message_Request(f)				GetKeyInfo1(f,0x08)
#define Message_setRequest(f, b)			SetKeyInfo1(f,0x08,b)
#define Message_setReserved(f, v)			(f.Octet[KeyInfoPos] |= (v<<4&0xff))
#define Message_KeyLength(f)				((unsigned short)(f.Octet[KeyLenPos] <<8) + (unsigned short)(f.Octet[KeyLenPos+1]))
#define Message_setKeyLength(f, v)		(f.Octet[KeyLenPos] = (v&0xff00) >>8 ,  f.Octet[KeyLenPos+1] = (v&0x00ff))

#define Message_KeyNonce(f)				SubStr(f, KeyNoncePos, KEY_NONCE_LEN)
#define Message_setKeyNonce(f, v)			SetSubStr(f, v, KeyNoncePos)
#define Message_EqualKeyNonce(f1, f2)		memcmp(f1.Octet + KeyNoncePos, f2.Octet, KEY_NONCE_LEN)? 0:1
#define Message_setKeyIV(f, v)			SetSubStr(f, v, KeyIVPos)
#define Message_setKeyRSC(f, v)			SetSubStr(f, v, KeyRSCPos)
#define Message_setKeyID(f, v)			SetSubStr(f, v, KeyIDPos)
#define Message_setMIC(f, v)				SetSubStr(f, v, KeyMICPos)
#define Message_KeyDataLength(f)			((unsigned short)(f.Octet[KeyDataLenPos] <<8) + (unsigned short)(f.Octet[KeyDataLenPos+1]))
#define Message_setKeyDataLength(f, v)	(f.Octet[KeyDataLenPos] = (v&0xff00) >>8 ,  f.Octet[KeyDataLenPos+1] = (v&0x00ff))
#define Message_setKeyData(f, v)			SetSubStr(f, v, KeyDataPos);

#define Message_KeyDataLength_SUITE_B_192(f)			((unsigned short)(f.Octet[KeyDataLenPos_SUITE_B_192] <<8) + (unsigned short)(f.Octet[KeyDataLenPos_SUITE_B_192+1]))
#define Message_setKeyDataLength_SUITE_B_192(f, v)	(f.Octet[KeyDataLenPos_SUITE_B_192] = (v&0xff00) >>8 ,  f.Octet[KeyDataLenPos_SUITE_B_192+1] = (v&0x00ff))
#define Message_setKeyData_SUITE_B_192(f, v)			SetSubStr(f, v, KeyDataPos_SUITE_B_192);

#define Message_CopyReplayCounter(f1, f2)	memcpy(f1.Octet + ReplayCounterPos, f2.Octet + ReplayCounterPos, KEY_RC_LEN)
#define Message_DefaultReplayCounter(li)	(((li.field.HighPart == 0xffffffff) && (li.field.LowPart == 0xffffffff) ) ?1:0)

#define LargeIntegerOverflow(x)			(x.field.HighPart == 0xffffffff) && (x.field.LowPart == 0xffffffff)
#define Octet16IntegerOverflow(x)			LargeIntegerOverflow(x.field.HighPart) && LargeIntegerOverflow(x.field.LowPart)
#define SetOctetString(f, buf, len)			(f.Octet = buf, f.Length = len)

enum DescTypeRSN {
	desc_type_RSN	= 2,
	desc_type_WPA	= 254
};

enum KeyType {
	type_Group	= 0,
	type_Pairwise	= 1
};

enum KeyDescVerType {
	key_desc_ver0 	= 0,
	key_desc_ver1 	= 1,
	key_desc_ver2 	= 2,
	key_desc_ver3 	= 3 //AES_128_CMAC, for 802.11w
};

struct _OCTET_STRING {
	unsigned char	*Octet;
	unsigned int Length;
};

union _LARGE_INTEGER {
	unsigned char 	charData[8];
	struct {
		unsigned long 	HighPart;
		unsigned long 	LowPart;
	} field;
};

union _OCTET16_INTEGER {
	unsigned char 	charData[16];
	struct {
		union _LARGE_INTEGER	HighPart;
		union _LARGE_INTEGER	LowPart;
	} field;
};

union  _OCTET32_INTEGER {
	unsigned char	charData[32];
	struct {
		union _OCTET16_INTEGER	HighPart;
		union _OCTET16_INTEGER	LowPart;
	} field;
};

struct eapol_params {
	struct _OCTET_STRING *pEapolKeyMsgSend;
	struct _OCTET_STRING *pEapolKeyMsgRecvd;
	unsigned char KeyDescVer;
	unsigned char KeyLen;
	union _LARGE_INTEGER *pReplayCounter;
	struct _OCTET_STRING *pNonce;
	int RSNEnabled;
	unsigned char use_pmksa;
	unsigned char *pmkid;
};

struct eapol_hdr_params {
	struct _OCTET_STRING *pEAPOLMsgSend;
	struct _OCTET_STRING *pEapolKeyMsgSend;
	u8 *da;
	u8 *sa;
	int algo;
	u8 *PTK;
	int CalMIC;
	u32 AuthKeyMgmt;
};

struct wpa_gtk_info {
	int				GN;
	int				GM;
	int				GTKRekey;
	int				GkeyReady;
	int				GTKAuthenticator;
	int				GInitAKeys;
	int				GUpdateStationKeys;

	unsigned char		GTK[NumGroupKey][GTK_LEN];
	unsigned char		GMK[GMK_LEN];

};

struct wpa_igkt_info {
	unsigned char		IGTK_IPN[2][8];
	unsigned char		IGTK[2][WPA_IGTK_MAX_LEN];
	int 			GN_igtk;
	int 			GM_igtk;
	int 	IGTK_len;
};

struct eapol_params_2 {
	struct _OCTET_STRING *pAuthInfoElement;
	unsigned char MulticastCipher;
	struct wpa_gtk_info *pGTKInfo;
	u64 *mgnt_80211w_IPN;
	unsigned int AuthKeyMgmt;
	struct wpa_igkt_info *pIGTKInfo;
	unsigned char *pPTK;
	union pn48 *dot11txpn;
	union  _OCTET32_INTEGER *pCounter;
};

struct _LIB1X_EAPOL_KEY {
	unsigned char		key_desc_ver;
	unsigned char		key_info[2];
	unsigned char		key_len[2];
	unsigned char		key_replay_counter[KEY_RC_LEN];
	unsigned char		key_nounce[KEY_NONCE_LEN];
	unsigned char		key_iv[KEY_IV_LEN];
	unsigned char		key_rsc[KEY_RSC_LEN];
	unsigned char		key_id[KEY_ID_LEN];
	unsigned char		key_mic[KEY_MIC_LEN_SUITE_B_192];
	unsigned char		key_data_len[KEY_MATERIAL_LEN];
	unsigned char		*key_data;
};

void rom_psk_CalcPTK(unsigned int AuthKeyMgmt,
					 unsigned char *addr1, unsigned char *addr2,
					 unsigned char *nonce1, unsigned char *nonce2,
					 unsigned char *keyin, int keyinlen,
					 unsigned char *keyout, int keyoutlen);

void rom_psk_CalcGTK(unsigned int AuthKeyMgmt,
					 unsigned char *addr, unsigned char *nonce,
					 unsigned char *keyin, int keyinlen,
					 unsigned char *keyout, int keyoutlen);


void wifi_rom_gen_nonce(unsigned char *nonce);
int wifi_rom_wpa_key_mgmt_sae(int akm);
int wifi_rom_wpa_key_mgmt_sha256(int akm);
int wifi_rom_wpa_key_mgmt_sha384(int akm);
void wifi_rom_ap_constructEAPOL_1Of4Way(struct eapol_params *params);
u8 wifi_rom_ap_constructEAPOL_3Of4Way(struct eapol_params *params, struct eapol_params_2 *params2, u8 is_80211w);
void wifi_rom_ap_constructEAPOL_1Of2Way(struct eapol_params *params, struct eapol_params_2 *params2, u8 is_80211w);
void wifi_rom_check_append_rsnxe(struct _OCTET_STRING *pAuthInfoElement, u8 h2e, u8 *rsnxe_ie);
void wifi_rom_set_playercounter(union _LARGE_INTEGER *x, unsigned long HighPart, unsigned long LowPart);
u8 wifi_rom_equal_playercounter(union _LARGE_INTEGER *x, union _LARGE_INTEGER *y);
void wifi_rom_fill_eapol_header(struct eapol_hdr_params *params);
u8 wifi_rom_check_playercounter(union _LARGE_INTEGER *pCurrentReplayCounter, struct _OCTET_STRING *pEapolKeyMsgRecvd);
void wifi_rom_set_group_cipher(u8 algo, unsigned char *MulticastCipher);
u8 wifi_rom_parse_group_kde(u8 *buf, u16 DataLength, struct wpa_gtk_info *pGTKInfo, struct wpa_igkt_info *pIGTKInfo, int *toSetKey);
u8 *wifi_rom_psk_ieee80211w_kde_add(u64 *mgnt_80211w_IPN, unsigned int AuthKeyMgmt, struct wpa_igkt_info *pIGTKInfo, unsigned char *key_data_pos);
u8 *wifi_rom_ap_fill_keydata(struct eapol_params_2 *params, unsigned char *key_data_pos, u8 is_rtw80211);
void wifi_rom_calc_mic(struct _OCTET_STRING EAPOLMsgSend, int algo, u32 AuthKeyMgmt, unsigned char *key, int keylen);
void wifi_rom_message_replaycounter_oc2li(struct _OCTET_STRING f, union _LARGE_INTEGER *li);
int wifi_rom_message_equalreplaycounter(union _LARGE_INTEGER li1, struct _OCTET_STRING f);
int wifi_rom_message_smallerequalreplaycounter(union _LARGE_INTEGER li1, struct _OCTET_STRING f);
int wifi_rom_message_smallerreplaycounter(union _LARGE_INTEGER li1, struct _OCTET_STRING f);
void wifi_rom_message_setreplaycounter(struct _OCTET_STRING f, unsigned long h, unsigned long l);
void wifi_rom_inc_largeinteger(union _LARGE_INTEGER *x);
union  _OCTET32_INTEGER *wifi_rom_inc_octet32_integer(union  _OCTET32_INTEGER *x);
void wifi_rom_set_eapolkeyiv(struct _OCTET_STRING ocDst, union  _OCTET32_INTEGER oc32Counter);
int wifi_rom_check_mic(struct _OCTET_STRING EAPOLMsgRecvd, u32 AuthKeyMgmt, unsigned char *key, int keylen);
int wifi_rom_decrypt_WPA2keydata(struct _OCTET_STRING EAPOLMsgRecvd, struct _OCTET_STRING EapolKeyMsgRecvd, unsigned char *key, int keylen, unsigned char *kek,
								 int keklen,
								 unsigned char *kout, int kout_size);
int wifi_rom_decrypt_GTK(struct _OCTET_STRING EAPOLMsgRecvd, unsigned char *kek, int keklen, int keylen, unsigned char *kout, int kout_size);
void wifi_rom_client_constructEAPOL_2Of4Way(struct _OCTET_STRING *pEapolKeyMsgSend, struct _OCTET_STRING StaInfoEapolKeyMsgRecvd,
		struct _OCTET_STRING StaInfoSNonce);
void wifi_rom_client_constructEAPOL_4Of4Way(struct _OCTET_STRING *pEapolKeyMsgSend, struct _OCTET_STRING StaInfoEapolKeyMsgRecvd, u32 AuthKeyMgmt);
void wifi_rom_client_constructEAPOL_2Of2Way(struct _OCTET_STRING *pEapolKeyMsgSend, struct _OCTET_STRING StaInfoEapolKeyMsgRecvd, int GblInfoGN,
		u32 AuthKeyMgmt);
void wifi_rom_client_constructEAPOL_MICof2Way(struct _OCTET_STRING *pEapolKeyMsgSend, union _LARGE_INTEGER *pStaInfoclientMICReportReplayCounter,
		int StaInfoRSNEnabled,
		u8 KeyDescriptorVer, u32 AuthKeyMgmt);
void strtopsk(u8 *des, u8 *src, u8 len);
int rom_psk_PasswordHash(unsigned char *password, int passwordlength, unsigned char *ssid, int ssidlength, unsigned char *output);
int wifi_rom_eapol_check_rsne_pmkid(u8 *ie, u8 ie_len, u8 *pmkid);


#endif	//_ROM_WPA_LITE_PSK_H_

