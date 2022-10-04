/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This is ROM code section.
 *
 *
 ******************************************************************************/
#ifndef __ROM_RTW_SECURITY_H_
#define __ROM_RTW_SECURITY_H_

struct mic_data {
	u32  K0, K1;         // Key
	u32  L, R;           // Current state
	u32  M;              // Message accumulator (single word)
	u32     nBytesInM;      // # bytes in M
};

union  u_crc {
	unsigned char	ch[4];
	int i;
};

//===============================
// WEP related
//===============================
void wep_80211_encrypt(
	u8 *pframe, u32 wlan_hdr_len, \
	u32 iv_len, u32 payload_len, \
	u8 *key, u32 key_len);

u8 wep_80211_decrypt(
	u8 *pframe, u32 wlan_hdr_len,
	u32 iv_len, u32 payload_len,
	u8 *key, u32 key_len,
	union u_crc *pcrc\
);

//===============================
// TKIP related
//===============================
void tkip_80211_encrypt(
	u8 *pframe, u32 wlan_hdr_len, \
	u32 iv_len, u32 payload_len, \
	u8 *key, u32 key_len, \
	u8 *ta);

u8 tkip_80211_decrypt(
	u8 *pframe, u32 wlan_hdr_len, \
	u32 iv_len, u32 payload_len, \
	u8 *key, u32 key_len, \
	u8 *ta, union u_crc *pcrc);

void tkip_micappendbyte(struct mic_data *pmicdata, u8 b);
void rtw_secmicsetkey(struct mic_data *pmicdata, u8 *key);
void rtw_secmicappend(struct mic_data *pmicdata, u8 *src, u32 nbytes);
void rtw_secgetmic(struct mic_data *pmicdata, u8 *dst);
void rtw_seccalctkipmic(u8 *key, u8 *header, u8 *data, u32 data_len, u8 *mic_code, u8 pri);
void tkip_phase1(u16 *p1k, const u8 *tk, const u8 *ta, u32 iv32);
void tkip_phase2(u8 *rc4key, const u8 *tk, const u16 *p1k, u16 iv16);


//===============================
// AES related
//===============================
void aes1_encrypt(u8 *key, u8 *data, u8 *ciphertext);
#ifdef CONFIG_IEEE80211W
void aesccmp_construct_mic_iv_ram(
	u8 *mic_iv, sint qc_exists, sint a4_exists,
	u8 *mpdu, uint payload_length, u8 *pn_vector, uint frtype);
void aesccmp_construct_mic_header1_ram(u8 *mic_header1, sint header_length, u8 *mpdu, uint frtype);
void aesccmp_construct_ctr_preload_ram(
	u8 *ctr_preload, sint a4_exists, sint qc_exists,
	u8 *mpdu, u8 *pn_vector, sint c, uint frtype);
#else
void aesccmp_construct_mic_iv(
	u8 *mic_iv, sint qc_exists, sint a4_exists,
	u8 *mpdu, uint payload_length, u8 *pn_vector);
void aesccmp_construct_mic_header1(u8 *mic_header1, sint header_length, u8 *mpdu);
void aesccmp_construct_ctr_preload(
	u8 *ctr_preload, sint a4_exists, sint qc_exists,
	u8 *mpdu, u8 *pn_vector, sint c);
#endif
void aesccmp_construct_mic_header2(
	u8 *mic_header2, u8 *mpdu, sint a4_exists, sint qc_exists);

u32 aes_80211_encrypt(
	u8 *pframe, u32 wlan_hdr_len, \
	u32 payload_len, u8 *key, \
	u32 frame_type, u8 *mic);

u32 aes_80211_decrypt(
	u8 *pframe, u32 wlan_hdr_len, \
	u32 payload_len, u8 *key, \
	u32 frame_type, u8 *mic);
#endif	//__ROM_RTW_SECURITY_H_

