/**
  ******************************************************************************
  * @file    rtw_80211spec_macro.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef	__RTW_80211SPEC_MACRO_H_
#define __RTW_80211SPEC_MACRO_H_

/* Frame control field constants */
#define RTW_IEEE80211_FCTL_VERS		0x0002
#define RTW_IEEE80211_FCTL_FTYPE		0x000c
#define RTW_IEEE80211_FCTL_STYPE		0x00f0
#define RTW_IEEE80211_FCTL_TODS		0x0100
#define RTW_IEEE80211_FCTL_FROMDS		0x0200
#define RTW_IEEE80211_FCTL_MOREFRAGS	0x0400
#define RTW_IEEE80211_FCTL_RETRY		0x0800
#define RTW_IEEE80211_FCTL_PM		0x1000
#define RTW_IEEE80211_FCTL_MOREDATA	0x2000
#define RTW_IEEE80211_FCTL_WEP		0x4000
#define RTW_IEEE80211_FCTL_ORDER		0x8000

#define RTW_IEEE80211_FTYPE_MGMT		0x0000
#define RTW_IEEE80211_FTYPE_CTL		0x0004
#define RTW_IEEE80211_FTYPE_DATA		0x0008

/* management */
#define RTW_IEEE80211_STYPE_ASSOC_REQ	0x0000
#define RTW_IEEE80211_STYPE_ASSOC_RESP 	0x0010
#define RTW_IEEE80211_STYPE_REASSOC_REQ	0x0020
#define RTW_IEEE80211_STYPE_REASSOC_RESP	0x0030
#define RTW_IEEE80211_STYPE_PROBE_REQ	0x0040
#define RTW_IEEE80211_STYPE_PROBE_RESP	0x0050
#define RTW_IEEE80211_STYPE_BEACON		0x0080
#define RTW_IEEE80211_STYPE_ATIM		0x0090
#define RTW_IEEE80211_STYPE_DISASSOC	0x00A0
#define RTW_IEEE80211_STYPE_AUTH		0x00B0
#define RTW_IEEE80211_STYPE_DEAUTH		0x00C0
#define RTW_IEEE80211_STYPE_ACTION		0x00D0

/* control */
#define RTW_IEEE80211_STYPE_PSPOLL		0x00A0
#define RTW_IEEE80211_STYPE_RTS		0x00B0
#define RTW_IEEE80211_STYPE_CTS		0x00C0
#define RTW_IEEE80211_STYPE_ACK		0x00D0
#define RTW_IEEE80211_STYPE_CFEND		0x00E0
#define RTW_IEEE80211_STYPE_CFENDACK	0x00F0

/* data */
#define RTW_IEEE80211_STYPE_DATA		0x0000
#define RTW_IEEE80211_STYPE_DATA_CFACK	0x0010
#define RTW_IEEE80211_STYPE_DATA_CFPOLL	0x0020
#define RTW_IEEE80211_STYPE_DATA_CFACKPOLL	0x0030
#define RTW_IEEE80211_STYPE_NULLFUNC	0x0040
#define RTW_IEEE80211_STYPE_CFACK		0x0050
#define RTW_IEEE80211_STYPE_CFPOLL		0x0060
#define RTW_IEEE80211_STYPE_CFACKPOLL	0x0070
#define RTW_IEEE80211_QOS_DATAGRP		0x0080
#define RTW_IEEE80211_QoS_DATAGRP		RTW_IEEE80211_QOS_DATAGRP
/* Mesh Control 802.11s */
#define RTW_IEEE80211_QOS_CTL_MESH_CONTROL_PRESENT  BIT(8)

/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1
#define WLAN_AUTH_CHALLENGE_LEN 128

/* 802.11b */
#define WLAN_STATUS_ASSOC_DENIED_NOSHORT 19
#define WLAN_STATUS_ASSOC_DENIED_NOPBCC 20
#define WLAN_STATUS_ASSOC_DENIED_NOAGILITY 21

#define IEEE80211_CCK_RATE_1MB		        0x02
#define IEEE80211_CCK_RATE_2MB		        0x04
#define IEEE80211_CCK_RATE_5MB		        0x0B
#define IEEE80211_CCK_RATE_11MB		        0x16
#define IEEE80211_OFDM_RATE_LEN 		8
#define IEEE80211_OFDM_RATE_6MB		        0x0C
#define IEEE80211_OFDM_RATE_9MB		        0x12
#define IEEE80211_OFDM_RATE_12MB		0x18
#define IEEE80211_OFDM_RATE_18MB		0x24
#define IEEE80211_OFDM_RATE_24MB		0x30
#define IEEE80211_OFDM_RATE_36MB		0x48
#define IEEE80211_OFDM_RATE_48MB		0x60
#define IEEE80211_OFDM_RATE_54MB		0x6C
#define IEEE80211_BASIC_RATE_MASK		0x80

#define IEEE80211_CCK_RATE_1MB_MASK		(1<<0)
#define IEEE80211_CCK_RATE_2MB_MASK		(1<<1)
#define IEEE80211_CCK_RATE_5MB_MASK		(1<<2)
#define IEEE80211_CCK_RATE_11MB_MASK		(1<<3)
#define IEEE80211_OFDM_RATE_6MB_MASK		(1<<4)
#define IEEE80211_OFDM_RATE_9MB_MASK		(1<<5)
#define IEEE80211_OFDM_RATE_12MB_MASK		(1<<6)
#define IEEE80211_OFDM_RATE_18MB_MASK		(1<<7)
#define IEEE80211_OFDM_RATE_24MB_MASK		(1<<8)
#define IEEE80211_OFDM_RATE_36MB_MASK		(1<<9)
#define IEEE80211_OFDM_RATE_48MB_MASK		(1<<10)
#define IEEE80211_OFDM_RATE_54MB_MASK		(1<<11)

#define IEEE80211_CCK_RATE_LEN  		4
#define IEEE80211_NUM_OFDM_RATESLEN	8

#define IEEE80211_CCK_RATES_MASK	        0x0000000F
#define IEEE80211_CCK_BASIC_RATES_MASK	(IEEE80211_CCK_RATE_1MB_MASK | \
	IEEE80211_CCK_RATE_2MB_MASK)
#define IEEE80211_CCK_DEFAULT_RATES_MASK	(IEEE80211_CCK_BASIC_RATES_MASK | \
        IEEE80211_CCK_RATE_5MB_MASK | \
        IEEE80211_CCK_RATE_11MB_MASK)

#define IEEE80211_OFDM_RATES_MASK		0x00000FF0
#define IEEE80211_OFDM_BASIC_RATES_MASK	(IEEE80211_OFDM_RATE_6MB_MASK | \
	IEEE80211_OFDM_RATE_12MB_MASK | \
	IEEE80211_OFDM_RATE_24MB_MASK)
#define IEEE80211_OFDM_DEFAULT_RATES_MASK	(IEEE80211_OFDM_BASIC_RATES_MASK | \
	IEEE80211_OFDM_RATE_9MB_MASK  | \
	IEEE80211_OFDM_RATE_18MB_MASK | \
	IEEE80211_OFDM_RATE_36MB_MASK | \
	IEEE80211_OFDM_RATE_48MB_MASK | \
	IEEE80211_OFDM_RATE_54MB_MASK)
#define IEEE80211_DEFAULT_RATES_MASK (IEEE80211_OFDM_DEFAULT_RATES_MASK | \
                                IEEE80211_CCK_DEFAULT_RATES_MASK)

#define IEEE80211_NUM_OFDM_RATES	    8
#define IEEE80211_NUM_CCK_RATES	            4
#define IEEE80211_OFDM_SHIFT_MASK_A         4

/* IEEE 802.11 requires that STA supports concurrent reception of at least
 * three fragmented frames. This define can be increased to support more
 * concurrent frames, but it should be noted that each entry can consume about
 * 2 kB of RAM and increasing cache size will slow down frame reassembly. */
#define IEEE80211_FRAG_CACHE_LEN 4

/* MAX_RATES_LENGTH needs to be 12.  The spec says 8, and many APs
 * only use 8, and then use extended rates for the remaining supported
 * rates.  Other APs, however, stick all of their supported rates on the
 * main rates information element... */
#define MAX_RATES_LENGTH                  ((u8)12)
#define MAX_RATES_EX_LENGTH               ((u8)16)
#define MAX_NETWORK_COUNT                  128
#define MAX_CHANNEL_NUMBER                 161

#define CRC_LENGTH		4U
#define P80211_OUI_LEN 3
#define LIB1X_EAPOL_HDRLEN	4

#define WEP_KEYS 4
#define WEP_KEY_LEN 13

#define WPA_SELECTOR_LEN 4
#define RSN_HEADER_LEN 4
#define RSN_SELECTOR_LEN 4
#define RSNXE_MIN_LEN 3
#define RSNXE_MAX_LEN (18)/*Draft P802.11REVmd_D5.0 p1464*/

#define MAXTID	16
#define WIRELESS_EXT		22

#define WLAN_IE_LEN_FIELD	1/*byte 0 is EID, byte 1 is length*/

/* Information Element IDs */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_FH_PARAMS 2
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_COUNTRY 7
#define WLAN_EID_CHALLENGE 16
/* EIDs defined by IEEE 802.11h - START */
#define WLAN_EID_PWR_CONSTRAINT 32
#define WLAN_EID_PWR_CAPABILITY 33
#define WLAN_EID_TPC_REQUEST 34
#define WLAN_EID_TPC_REPORT 35
#define WLAN_EID_SUPPORTED_CHANNELS 36
#define WLAN_EID_CHANNEL_SWITCH 37
#define WLAN_EID_MEASURE_REQUEST 38
#define WLAN_EID_MEASURE_REPORT 39
#define WLAN_EID_QUITE 40
#define WLAN_EID_IBSS_DFS 41
/* EIDs defined by IEEE 802.11h - END */
#define WLAN_EID_ERP_INFO 42
#define WLAN_EID_HT_CAP 45
#define WLAN_EID_RSN 48
#define WLAN_EID_EXT_SUPP_RATES 50
#define WLAN_EID_MOBILITY_DOMAIN 54
#define WLAN_EID_FAST_BSS_TRANSITION 55
#define WLAN_EID_TIMEOUT_INTERVAL 56
#define WLAN_EID_RIC_DATA 57
#define WLAN_EID_HT_OPERATION 61
#define WLAN_EID_SECONDARY_CHANNEL_OFFSET 62
#define WLAN_EID_RRM_EN_CAP			70
#define WLAN_EID_MULTIPLE_BSSID 71
#define WLAN_EID_20_40_BSS_COEXISTENCE 72
#define WLAN_EID_20_40_BSS_INTOLERANT 73
#define WLAN_EID_OVERLAPPING_BSS_SCAN_PARAMS 74
#define WLAN_EID_MMIE 76
#define WLAN_EID_NONTRANSMITTED_BSSID_CAPABILITY 83
#define WLAN_EID_MULTIPLE_BSSID_INDEX 85
#define WLAN_EID_VHT_CAPABILITY 191
#define WLAN_EID_VHT_OPERATION 192
#define WLAN_EID_VHT_OPERATING_MODE_NOTIFICATION 199
#define WLAN_EID_MESH_CONFIG 113
#define WLAN_EID_MESH_ID 114
#define WLAN_EID_MESH_PEERING_MANAGEMENT 117
#define WLAN_EID_GANN 125
#define WLAN_EID_RANN 126
#define WLAN_EID_EXT_CAP 127
#define WLAN_EID_PREQ 130
#define WLAN_EID_PREP 131
#define WLAN_EID_PERR 132
#define WLAN_EID_MIC 140
#define WLAN_EID_TWT 216
#define WLAN_EID_VENDOR_SPECIFIC 221
#define WLAN_EID_GENERIC (WLAN_EID_VENDOR_SPECIFIC)
#define WLAN_EID_RSNXE					244

#define WLAN_EID_WPA	0xdd
#define WLAN_EID_WPA2	0x30

/* EID Extension */
#define WLAN_EID_EXTENSION 255
#define WLAN_EID_EXT_OWE_DH_PARAM 32	//WPA3
#define WLAN_EID_EXT_HE_CAPABILITY 35
#define WLAN_EID_EXT_HE_OPERATION 36
#define WLAN_EID_EXT_HE_MU_EDCA 38
#define WLAN_EID_EXT_SPATIAL_REUSE_PARA 39
#define WLAN_EID_EXT_BSS_COLOR_CHANGE 42
#define WLAN_EID_EXT_MULTIPLE_BSSID_CONFIGURATION 55	//MBSSID
#define WLAN_EID_EXT_NON_INGERITANCE_ELEMENT 56		//MBSSID

#define WLAN_EID_EXT_CAP_MAX_LEN 10

#define MAX_WPA_IE_LEN (257)
#define MAX_WPS_IE_LEN (512)
#define MAX_P2P_IE_LEN (256)
#define MAX_WFD_IE_LEN (128)
#define MAX_FT_IE_LEN (256)

#define _WMM_IE_Length_				7  // for WMM STA
#define _WMM_Para_Element_Length_		24

#define _ASOCREQ_IE_OFFSET_		4	// excluding wlan_hdr
#define	_ASOCRSP_IE_OFFSET_		6
#define _REASOCREQ_IE_OFFSET_	10
#define _REASOCRSP_IE_OFFSET_	6
#define _PROBEREQ_IE_OFFSET_	0
#define	_PROBERSP_IE_OFFSET_	12
#define _AUTH_IE_OFFSET_		6
#define _DEAUTH_IE_OFFSET_		0
#define _BEACON_IE_OFFSET_		12
#define _PUBLIC_ACTION_IE_OFFSET_	8

#define _FIXED_IE_LENGTH_			_BEACON_IE_OFFSET_

#define WLAN_IEEE_OUI_LEN	3
#define WLAN_ADDR_LEN		6
#define WLAN_CRC_LEN		4
#define WLAN_BSSID_LEN		6
#define WLAN_BSS_TS_LEN		8
#define WLAN_HDR_A3_LEN		24
#define WLAN_HDR_A4_LEN		30
#define WLAN_HDR_A3_QOS_LEN	26
#define WLAN_HDR_A4_QOS_LEN	32
#define WLAN_HDR_A3_QOS_HTC_LEN	30
#define WLAN_DATA_MAXLEN	2312
#define WLAN_RATES_MAXLEN        12
#define WLAN_RATES_EX_MAXLEN     16

/* Maximum size of the ESSID and NICKN strings */
#define RTW_ESSID_MAX_SIZE	32
#define RTW_PASSPHRASE_WPA2_SIZE 64
#define RTW_PASSPHRASE_WPA3_SIZE 128
#define RTW_PASSPHRASE_MAX_SIZE RTW_PASSPHRASE_WPA3_SIZE

#define BIP_MAX_KEYID 5
#define BIP_AAD_SIZE  20

/*wlan over head : [WLAN_HDR_A4_QOS_HTC][IV][SNAP][][MIC][ICV]*/
#define WLAN_HDR_A4_QOS_HTC_LEN	36
#define WLAN_MAX_IV_LEN	8
#define WLAN_SNAP_HEADER	8
#define WLAN_MAX_MIC_LEN	8
#define WLAN_MAX_ICV_LEN	8
#define WLAN_MAX_PROTOCOL_OVERHEAD (WLAN_HDR_A4_QOS_HTC_LEN+WLAN_MAX_IV_LEN+\
							WLAN_SNAP_HEADER+WLAN_MAX_MIC_LEN+WLAN_MAX_ICV_LEN)/*=68*/

#define _TO_DS_		BIT(8)
#define _FROM_DS_	BIT(9)
#define _MORE_FRAG_	BIT(10)
#define _RETRY_		BIT(11)
#define _PWRMGT_	BIT(12)
#define _MORE_DATA_	BIT(13)
#define _PRIVACY_	BIT(14)
#define _ORDER_			BIT(15)

#define SetToDs(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_TO_DS_); \
	} while(0)

#define GetToDs(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_TO_DS_)) != 0)

#define ClearToDs(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_TO_DS_)); \
	} while(0)

#define SetFrDs(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_FROM_DS_); \
	} while(0)

#define GetFrDs(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_FROM_DS_)) != 0)

#define ClearFrDs(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_FROM_DS_)); \
	} while(0)

#define get_tofr_ds(pframe)	((GetToDs(pframe) << 1) | GetFrDs(pframe))
#define is_qos(pframe)	(get_frame_sub_type(pframe) & BIT(7))

#define SetMFrag(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_MORE_FRAG_); \
	} while(0)

#define GetMFrag(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_MORE_FRAG_)) != 0)

#define ClearMFrag(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_MORE_FRAG_)); \
	} while(0)

#define SetRetry(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_RETRY_); \
	} while(0)

#define GetRetry(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_RETRY_)) != 0)

#define ClearRetry(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_RETRY_)); \
	} while(0)

#define SetPwrMgt(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_PWRMGT_); \
	} while(0)

#define GetPwrMgt(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_PWRMGT_)) != 0)

#define ClearPwrMgt(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_PWRMGT_)); \
	} while(0)

#define SetMData(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_MORE_DATA_); \
	} while(0)

#define GetMData(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_MORE_DATA_)) != 0)

#define ClearMData(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_MORE_DATA_)); \
	} while(0)

#define SetPrivacy(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_PRIVACY_); \
	} while(0)

#define GetPrivacy(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_PRIVACY_)) != 0)

#define ClearPrivacy(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_PRIVACY_)); \
	} while(0)

#define SetOrder(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(_ORDER_); \
	} while (0)

#define ClearOrder(pbuf)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= (~cpu_to_le16(_ORDER_)); \
	} while(0)

#define GetOrder(pbuf)	(((*(unsigned short *)(void*)(pbuf)) & le16_to_cpu(_ORDER_)) != 0)

#define GetFrameType(pbuf)	(le16_to_cpu(*(unsigned short *)(void*)(pbuf)) & (BIT(3) | BIT(2)))

#define get_frame_sub_type(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)(pbuf)) & (BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2)))


#define set_frame_sub_type(pbuf, type) \
	do {    \
		*(unsigned short *)(void*)(pbuf) &= cpu_to_le16(~(BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2))); \
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(type); \
	} while (0)

#define SetFrameType(pbuf,type)	\
	do { 	\
		*(unsigned short *)(void*)(pbuf) &= cpu_to_le16(~(BIT(3) | BIT(2))); \
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(type); \
	} while(0)

#define GetFrameSubType(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)(pbuf)) & (BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2)))

#define SetFrameSubType(pbuf,type) \
	do {    \
		*(unsigned short *)(void*)(pbuf) &= cpu_to_le16(~(BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2))); \
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(type); \
	} while(0)

#define GetSequence(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22)) >> 4)

#define GetFragNum(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22)) & 0x0f)

#define GetTupleCache(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22)))

#define SetFragNum(pbuf, num) \
	do {    \
		*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22) = \
			((*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22)) & le16_to_cpu(~(0x000f))) | \
			cpu_to_le16(0x0f & (num));     \
	} while(0)

#define SetSeqNum(pbuf, num) \
	do {    \
		*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22) = \
			((*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 22)) & le16_to_cpu((unsigned short)~0xfff0)) | \
			le16_to_cpu((unsigned short)(0xfff0 & (num << 4))); \
	} while(0)
#define GetFrameControl(pbuf)(cpu_to_le16(*(unsigned short*)(void*)((SIZE_PTR)(pbuf))))
#define SetFrameControl(pbuf, val)		\
   do {    \
	   *(unsigned short *)(void*)((SIZE_PTR)(pbuf)) = cpu_to_le16(0xffff & (val)); \
   } while(0)

#define GetDuration(pbuf) (cpu_to_le16(*(unsigned short*)(void*)((SIZE_PTR)(pbuf) + 2)))
#define SetDuration(pbuf, dur) \
	do {    \
		*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 2) = cpu_to_le16(0xffff & (dur)); \
	} while(0)


#define SetPriority(pbuf, tid)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) &= ~0x000f; \
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16(tid & 0xf); \
	} while(0)

#define GetPriority(pbuf)	((le16_to_cpu(*(unsigned short *)(void*)(pbuf))) & 0xf)

#define SetEOSP(pbuf, eosp)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16( (eosp & 1) << 4); \
	} while(0)

#define SetAckpolicy(pbuf, ack)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16( (ack & 3) << 5); \
	} while(0)

#define GetAckpolicy(pbuf) (((le16_to_cpu(*(unsigned short *)(void*)pbuf)) >> 5) & 0x3)

#define GetAMsdu(pbuf) (((le16_to_cpu(*(unsigned short *)(void*)pbuf)) >> 7) & 0x1)

#define SetAMsdu(pbuf, amsdu)	\
	do	{	\
		*(unsigned short *)(void*)(pbuf) |= cpu_to_le16( (amsdu & 1) << 7); \
	} while(0)

#define GetAid(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + 2)) & 0x3fff)

#define GetTid(pbuf)	(cpu_to_le16(*(unsigned short *)(void*)((SIZE_PTR)(pbuf) + (((GetToDs(pbuf)<<1)|GetFrDs(pbuf))==3?30:24))) & 0x000f)

#define GetAddr1Ptr(pbuf)	((unsigned char *)((SIZE_PTR)(pbuf) + 4))

#define GetAddr2Ptr(pbuf)	((unsigned char *)((SIZE_PTR)(pbuf) + 10))

#define GetAddr3Ptr(pbuf)	((unsigned char *)((SIZE_PTR)(pbuf) + 16))

#define GetAddr4Ptr(pbuf)	((unsigned char *)((SIZE_PTR)(pbuf) + 24))

//WIFI_WMM
#define GetQosControl(pbuf) (unsigned char *)((SIZE_PTR)(pbuf) + (((GetToDs(pbuf)<<1)|GetFrDs(pbuf))==3?30:24))

#define MacAddr_isBcst(addr) \
( \
	( (addr[0] == 0xff) && (addr[1] == 0xff) && \
		(addr[2] == 0xff) && (addr[3] == 0xff) && \
		(addr[4] == 0xff) && (addr[5] == 0xff) )  ? _TRUE : _FALSE \
)

#define RTW_RSN_CAPABILITY_RSVD		BIT(15)
#define RTW_RSNX_CAPABILITY_RSVD		(BIT(6) | BIT(7))
#define RTW_RSNX_CAPABILITY_PROTECTED_TWT	BIT(4)
#define RTW_RSNX_FIELD_LEN_MASK		0xF

#define RTW_ERP_INFO_NON_ERP_PRESENT BIT(0)
#define RTW_ERP_INFO_USE_PROTECTION BIT(1)
#define RTW_ERP_INFO_BARKER_PREAMBLE_MODE BIT(2)

/* block-ack parameters */
#define IEEE80211_ADDBA_PARAM_POLICY_MASK 0x0002
#define IEEE80211_ADDBA_PARAM_TID_MASK 0x003C
#define IEEE80211_ADDBA_PARAM_BUF_SIZE_MASK 0xFFC0
#define IEEE80211_DELBA_PARAM_TID_MASK 0xF000
#define IEEE80211_DELBA_PARAM_INITIATOR_MASK 0x0800

#define WLAN_CAPABILITY_BSS BIT(0)
#define WLAN_CAPABILITY_IBSS BIT(1)
#define WLAN_CAPABILITY_CF_POLLABLE BIT(2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST BIT(3)
#define WLAN_CAPABILITY_PRIVACY BIT(4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE BIT(5)
#define WLAN_CAPABILITY_PBCC BIT(6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY BIT(7)
#define WLAN_CAPABILITY_SHORT_SLOT BIT(10)

/* Status codes (IEEE 802.11-2007, 7.3.1.9, Table 7-23) */
#define WLAN_STATUS_SUCCESS 0
#define WLAN_STATUS_UNSPECIFIED_FAILURE 1
#define WLAN_STATUS_CAPS_UNSUPPORTED 10
#define WLAN_STATUS_REASSOC_NO_ASSOC 11
#define WLAN_STATUS_ASSOC_DENIED_UNSPEC 12
#define WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG 13
#define WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION 14
#define WLAN_STATUS_CHALLENGE_FAIL 15
#define WLAN_STATUS_AUTH_TIMEOUT 16
#define WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA 17
#define WLAN_STATUS_ASSOC_DENIED_RATES 18
#define WLAN_STATUS_DENIED_INSUFFICIENT_BANDWIDTH 33
#define WLAN_STATUS_REQUEST_DECLINED 37
/* WPA3-SAE*/
#define WLAN_STATUS_ANTI_CLOGGING_TOKEN_REQ 		76
#define WLAN_STATUS_UNSUPPORTED_FCC_GROUP   		77
#define WLAN_STATUS_UNKNOWN_PASSWORD_IDENTIFIER		123
//entended
/* IEEE 802.11b */
#define WLAN_STATUS_ASSOC_DENIED_NOSHORT 19
#define WLAN_STATUS_ASSOC_DENIED_NOPBCC 20
#define WLAN_STATUS_ASSOC_DENIED_NOAGILITY 21
/* IEEE 802.11h */
#define WLAN_STATUS_SPEC_MGMT_REQUIRED 22
#define WLAN_STATUS_PWR_CAPABILITY_NOT_VALID 23
#define WLAN_STATUS_SUPPORTED_CHANNEL_NOT_VALID 24
/* IEEE 802.11g */
#define WLAN_STATUS_ASSOC_DENIED_NO_SHORT_SLOT_TIME 25
#define WLAN_STATUS_ASSOC_DENIED_NO_ER_PBCC 26
#define WLAN_STATUS_ASSOC_DENIED_NO_DSSS_OFDM 27
/* IEEE 802.11w */
#define WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY 30
#define WLAN_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION 31
/* IEEE 802.11i */
#define WLAN_STATUS_INVALID_IE 40
#define WLAN_STATUS_GROUP_CIPHER_NOT_VALID 41
#define WLAN_STATUS_PAIRWISE_CIPHER_NOT_VALID 42
#define WLAN_STATUS_AKMP_NOT_VALID 43
#define WLAN_STATUS_UNSUPPORTED_RSN_IE_VERSION 44
#define WLAN_STATUS_INVALID_RSN_IE_CAPAB 45
#define WLAN_STATUS_CIPHER_REJECTED_PER_POLICY 46
#define WLAN_STATUS_TS_NOT_CREATED 47
#define WLAN_STATUS_DIRECT_LINK_NOT_ALLOWED 48
#define WLAN_STATUS_DEST_STA_NOT_PRESENT 49
#define WLAN_STATUS_DEST_STA_NOT_QOS_STA 50
#define WLAN_STATUS_ASSOC_DENIED_LISTEN_INT_TOO_LARGE 51
/* IEEE 802.11r */
#define WLAN_STATUS_INVALID_FT_ACTION_FRAME_COUNT 52
#define WLAN_STATUS_INVALID_PMKID 53
#define WLAN_STATUS_INVALID_MDIE 54
#define WLAN_STATUS_INVALID_FTIE 55

#define WLAN_STATUS_SAE_HASH_TO_ELEMENT 126

//	===============WPS Section===============
//	For WPSv1.0
#define WPSOUI							0x0050f204
//	WPS attribute ID
#define WPS_ATTR_VER1					0x104A
#define WPS_ATTR_SIMPLE_CONF_STATE	0x1044
#define WPS_ATTR_RESP_TYPE			0x103B
#define WPS_ATTR_UUID_E				0x1047
#define WPS_ATTR_MANUFACTURER		0x1021
#define WPS_ATTR_MODEL_NAME			0x1023
#define WPS_ATTR_MODEL_NUMBER		0x1024
#define WPS_ATTR_SERIAL_NUMBER		0x1042
#define WPS_ATTR_PRIMARY_DEV_TYPE	0x1054
#define WPS_ATTR_SEC_DEV_TYPE_LIST	0x1055
#define WPS_ATTR_DEVICE_NAME			0x1011
#define WPS_ATTR_CONF_METHOD			0x1008
#define WPS_ATTR_RF_BANDS				0x103C
#define WPS_ATTR_DEVICE_PWID			0x1012
#define WPS_ATTR_REQUEST_TYPE			0x103A
#define WPS_ATTR_ASSOCIATION_STATE	0x1002
#define WPS_ATTR_CONFIG_ERROR			0x1009
#define WPS_ATTR_VENDOR_EXT			0x1049
#define WPS_ATTR_SELECTED_REGISTRAR	0x1041

//	Value of WPS attribute "WPS_ATTR_DEVICE_NAME
#define WPS_MAX_DEVICE_NAME_LEN		32

//	Value of WPS Request Type Attribute
#define WPS_REQ_TYPE_ENROLLEE_INFO_ONLY			0x00
#define WPS_REQ_TYPE_ENROLLEE_OPEN_8021X		0x01
#define WPS_REQ_TYPE_REGISTRAR					0x02
#define WPS_REQ_TYPE_WLAN_MANAGER_REGISTRAR	0x03

//	Value of WPS Response Type Attribute
#define WPS_RESPONSE_TYPE_INFO_ONLY	0x00
#define WPS_RESPONSE_TYPE_8021X		0x01
#define WPS_RESPONSE_TYPE_REGISTRAR	0x02
#define WPS_RESPONSE_TYPE_AP			0x03

//	Value of WPS WiFi Simple Configuration State Attribute
#define WPS_WSC_STATE_NOT_CONFIG	0x01
#define WPS_WSC_STATE_CONFIG			0x02

//	Value of WPS Version Attribute
#define WPS_VERSION_1					0x10

//	Value of WPS Configuration Method Attribute
#define WPS_CONFIG_METHOD_FLASH		0x0001
#define WPS_CONFIG_METHOD_ETHERNET	0x0002
#define WPS_CONFIG_METHOD_LABEL		0x0004
#define WPS_CONFIG_METHOD_DISPLAY	0x0008
#define WPS_CONFIG_METHOD_E_NFC		0x0010
#define WPS_CONFIG_METHOD_I_NFC		0x0020
#define WPS_CONFIG_METHOD_NFC		0x0040
#define WPS_CONFIG_METHOD_PBC		0x0080
#define WPS_CONFIG_METHOD_KEYPAD	0x0100
#define WPS_CONFIG_METHOD_VPBC		0x0280
#define WPS_CONFIG_METHOD_PPBC		0x0480
#define WPS_CONFIG_METHOD_VDISPLAY	0x2008
#define WPS_CONFIG_METHOD_PDISPLAY	0x4008

//	Value of Category ID of WPS Primary Device Type Attribute
#define WPS_PDT_CID_DISPLAYS			0x0007
#define WPS_PDT_CID_MULIT_MEDIA		0x0008
#define WPS_PDT_CID_RTK_WIDI			WPS_PDT_CID_MULIT_MEDIA

//	Value of Sub Category ID of WPS Primary Device Type Attribute
#define WPS_PDT_SCID_MEDIA_SERVER	0x0005
#define WPS_PDT_SCID_RTK_DMP			WPS_PDT_SCID_MEDIA_SERVER

//	Value of Device Password ID
#define WPS_DPID_PIN					0x0000
#define WPS_DPID_USER_SPEC			0x0001
#define WPS_DPID_MACHINE_SPEC			0x0002
#define WPS_DPID_REKEY					0x0003
#define WPS_DPID_PBC					0x0004
#define WPS_DPID_REGISTRAR_SPEC		0x0005

//	Value of WPS RF Bands Attribute
#define WPS_RF_BANDS_2_4_GHZ		0x01
#define WPS_RF_BANDS_5_GHZ		0x02

//	Value of WPS Association State Attribute
#define WPS_ASSOC_STATE_NOT_ASSOCIATED			0x00
#define WPS_ASSOC_STATE_CONNECTION_SUCCESS		0x01
#define WPS_ASSOC_STATE_CONFIGURATION_FAILURE	0x02
#define WPS_ASSOC_STATE_ASSOCIATION_FAILURE		0x03
#define WPS_ASSOC_STATE_IP_FAILURE				0x04


// This value is tested by WiFi 11n Test Plan 5.2.3.
// This test verifies the WLAN NIC can update the NAV through sending the CTS with large duration.
#define	WiFiNavUpperUs				32640

#define _MME_IE_LENGTH_  18

#define OP_MODE_PURE                    0
#define OP_MODE_MAY_BE_LEGACY_STAS      1
#define OP_MODE_20MHZ_HT_STA_ASSOCED    2
#define OP_MODE_MIXED                   3

#define HT_INFO_HT_PARAM_SECONDARY_CHNL_OFF_MASK	((u8) BIT(0) | BIT(1))
#define HT_INFO_HT_PARAM_SECONDARY_CHNL_ABOVE		((u8) BIT(0))
#define HT_INFO_HT_PARAM_SECONDARY_CHNL_BELOW		((u8) BIT(0) | BIT(1))
#define HT_INFO_HT_PARAM_REC_TRANS_CHNL_WIDTH		((u8) BIT(2))
#define HT_INFO_HT_PARAM_RIFS_MODE			((u8) BIT(3))
#define HT_INFO_HT_PARAM_CTRL_ACCESS_ONLY		((u8) BIT(4))
#define HT_INFO_HT_PARAM_SRV_INTERVAL_GRANULARITY	((u8) BIT(5))

#define HT_INFO_OPERATION_MODE_OP_MODE_MASK	\
		((u8) (0x0003))
#define HT_INFO_OPERATION_MODE_OP_MODE_OFFSET		0
#define HT_INFO_OPERATION_MODE_NON_GF_DEVS_PRESENT	((u8) BIT(2))
#define HT_INFO_OPERATION_MODE_TRANSMIT_BURST_LIMIT	((u8) BIT(3))
#define HT_INFO_OPERATION_MODE_NON_HT_STA_PRESENT	((u8) BIT(4))

#define HT_INFO_STBC_PARAM_DUAL_BEACON			((u16) BIT(6))
#define HT_INFO_STBC_PARAM_DUAL_STBC_PROTECT		((u16) BIT(7))
#define HT_INFO_STBC_PARAM_SECONDARY_BCN		((u16) BIT(8))
#define HT_INFO_STBC_PARAM_LSIG_TXOP_PROTECT_ALLOWED	((u16) BIT(9))
#define HT_INFO_STBC_PARAM_PCO_ACTIVE			((u16) BIT(10))
#define HT_INFO_STBC_PARAM_PCO_PHASE			((u16) BIT(11))

/*
 * A-PMDU buffer sizes
 * According to IEEE802.11n spec size varies from 8K to 64K (in powers of 2)
 */
#define IEEE80211_MIN_AMPDU_BUF 0x8
#define IEEE80211_MAX_AMPDU_BUF 0x40

//	P2P Public Action Frame Type
#define	P2P_GO_NEGO_REQ						0
#define	P2P_GO_NEGO_RESP						1
#define	P2P_GO_NEGO_CONF						2
#define	P2P_INVIT_REQ							3
#define	P2P_INVIT_RESP							4
#define	P2P_DEVDISC_REQ						5
#define	P2P_DEVDISC_RESP						6
#define	P2P_PROVISION_DISC_REQ				7
#define	P2P_PROVISION_DISC_RESP				8

#define OUI_MICROSOFT 0x0050f2 /* Microsoft (also used in Wi-Fi specs) 	* 00:50:F2 */
#define OUI_BROADCOM 0x00904c /* Broadcom (Epigram) */

#define WME_OUI_TYPE 2
#define WME_OUI_SUBTYPE_INFORMATION_ELEMENT 0
#define WME_OUI_SUBTYPE_PARAMETER_ELEMENT 1
#define WME_OUI_SUBTYPE_TSPEC_ELEMENT 2

#define VENDOR_HT_CAPAB_OUI_TYPE 0x33 /* 00-90-4c:0x33 */

static inline u8 *rtw_get_capability_from_ie(u8 *ie)
{
	return (ie + 8 + 2);
}

static inline u8 *rtw_get_beacon_interval_from_ie(u8 *ie)
{
	return (ie + 8);
}

__inline static int IS_IPV6_MCAST(const u8 *da)
{
	if ((*da == 0x33 && *(da + 1) == 0x33)) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}

//temporarily put inline here
__inline static int IS_MCAST(unsigned char *da)
{
	if ((*da) & 0x01) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}

__inline static int IsFrameTypeCtrl(unsigned char *pframe)
{
	if (BIT(2) == GetFrameType(pframe)) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}

__inline static unsigned char *get_ta(unsigned char *pframe)
{
	unsigned char 	*ta;
	ta = GetAddr2Ptr(pframe);
	return ta;
}

__inline static unsigned char *get_ra(unsigned char *pframe)
{
	unsigned char 	*ra;
	ra = GetAddr1Ptr(pframe);
	return ra;
}

__inline static unsigned char *get_da(unsigned char *pframe)
{
	unsigned char 	*da;
	unsigned int	to_fr_ds	= (GetToDs(pframe) << 1) | GetFrDs(pframe);

	switch (to_fr_ds) {
	case 0x00:	// ToDs=0, FromDs=0
		da = GetAddr1Ptr(pframe);
		break;
	case 0x01:	// ToDs=0, FromDs=1
		da = GetAddr1Ptr(pframe);
		break;
	case 0x02:	// ToDs=1, FromDs=0
		da = GetAddr3Ptr(pframe);
		break;
	default:	// ToDs=1, FromDs=1
		da = GetAddr3Ptr(pframe);
		break;
	}

	return da;
}


__inline static unsigned char *get_sa(unsigned char *pframe)
{
	unsigned char 	*sa;
	unsigned int	to_fr_ds	= (GetToDs(pframe) << 1) | GetFrDs(pframe);

	switch (to_fr_ds) {
	case 0x00:	// ToDs=0, FromDs=0
		sa = GetAddr2Ptr(pframe);
		break;
	case 0x01:	// ToDs=0, FromDs=1
		sa = GetAddr3Ptr(pframe);
		break;
	case 0x02:	// ToDs=1, FromDs=0
		sa = GetAddr2Ptr(pframe);
		break;
	default:	// ToDs=1, FromDs=1
		sa = GetAddr4Ptr(pframe);
		break;
	}

	return sa;
}

__inline static unsigned char *get_hdr_bssid(unsigned char *pframe)
{
	unsigned char 	*sa;
	unsigned int	to_fr_ds	= (GetToDs(pframe) << 1) | GetFrDs(pframe);

	switch (to_fr_ds) {
	case 0x00:	// ToDs=0, FromDs=0
		sa = GetAddr3Ptr(pframe);
		break;
	case 0x01:	// ToDs=0, FromDs=1
		sa = GetAddr2Ptr(pframe);
		break;
	case 0x02:	// ToDs=1, FromDs=0
		sa = GetAddr1Ptr(pframe);
		break;
	case 0x03:	// ToDs=1, FromDs=1
		sa = GetAddr1Ptr(pframe);
		break;
	default:
		sa = NULL; //???????
		break;
	}

	return sa;
}



#endif

