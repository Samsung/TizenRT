#ifndef _WIFI_PROTOCOL_RAM_H_
#define _WIFI_PROTOCOL_RAM_H_

#define LENGTH_IEEE80211_HDR_3ADDR  24
#define _FIX_IE_LENGTH_             12
#define WLAN_HDR_A3_QOS_LEN         26
#define WLAN_LLC_HEADER_SIZE        6

#define _ASOCREQ_IE_OFFSET_         4   // excluding wlan_hdr
#define _ASOCRSP_IE_OFFSET_         6
#define _REASOCREQ_IE_OFFSET_       10
#define _REASOCRSP_IE_OFFSET_       6
#define _PROBEREQ_IE_OFFSET_        0
#define _PROBERSP_IE_OFFSET_        12
#define _AUTH_IE_OFFSET_            6
#define _DEAUTH_IE_OFFSET_          0
#define _BEACON_IE_OFFSET_          12

#define _SSID_IE_                   0
#define _SUPPORTEDRATES_IE_         1
#define _DSSET_IE_                  3
#define _TIM_IE_                    5
#define _IBSS_PARA_IE_              6
#define _COUNTRY_IE_                7
#define _QBSSLOAD_IE_               11
#define _EDCASETS_IE_               12
#define _TS_IE_                     13
#define _TCLASS_IE_                 14
#define _SCHEDULE_IE_               15
#define _TSDELAY_IE_                43
#define _TCLASSPROCESSING_IE_       44
#define _QOS_CAPABILITY_IE_         46

#define _CHLGETXT_IE_               16
#define _RSN_IE_2_                  48
#define _ERPINFO_IE_                42
#define _EXT_SUPPORTEDRATES_IE_     50
#define _RSN_IE_1_                  221
#define _RESERVED47_                47
#define _HT_CAPS_IE_                45
#define _HT_INFO_IE_                61
#define _HT_BSSCOEXIT_IE_           72  // 20/40 BSS Coexistence
#define _HT_40INTOLERANT_IE_        73
#define _HT_OBSS_SCAN_IE_           74  // Overlapping BSS Scan Parameters
#define _HT_EXTCAP_IE_              127

/*  below is MAC Header offset */
#define MAC_ADRESS1_OFFSET  4
#define MAC_ADRESS2_OFFSET 10
#define MAC_ADRESS3_OFFSET 16
#define MAC_SEQ_OFFSET         22
#define ETH_ALEN           6
#define IPv4_ALEN          4

/* below is IPv4 header offset */
#define IP_VERSION_IHL 1
#define TOS            1
#define IP_LENGTH      2
#define IPID           2
#define FLAG_OFFSET    2
#define TTL            1
#define PROTOCOL       1
#define IPCHECKSUM     2
#define IP_ID_OFFSET   4
#define IP_CHECKSUM_OFFSET  10
#define IP_SRC_ADDR_OFFSET  12
#define IP_DES_ADDR_OFFSET  16
#define IP_HEADER      20

/*  below is LLC offset */
#define LLC_LEN        8

/**
 *  @brief  REASON CODE
 *  16 bit field, See textbook Table.4-5.
 */
enum WIFI_REASON_CODE   {
	_RSON_RESERVED_                 = 0,    // Reserved.
	_RSON_UNSPECIFIED_              = 1,    // Unspecified.
	_RSON_AUTH_NO_LONGER_VALID_     = 2,    // Auth invalid.
	_RSON_DEAUTH_STA_LEAVING_       = 3,    // STA leave BSS or ESS, DeAuth.
	_RSON_INACTIVITY_               = 4,    // Exceed idle timer, Disconnect.
	_RSON_UNABLE_HANDLE_            = 5,    // BS resoruce insufficient.
	_RSON_CLS2_                     = 6,    // UnAuth STA frame or SubType incorrect.
	_RSON_CLS3_                     = 7,    // UnAuth STA frame or SubType incorrect.
	_RSON_DISAOC_STA_LEAVING_       = 8,    // STA leave BSS or ESS, DeAssoc.
	_RSON_ASOC_NOT_AUTH_            = 9,    // Request assiciate or reassociate, before authenticate
	// 10,11,12 for 802.11h
	// WPA reason
	_RSON_INVALID_IE_               = 13,
	_RSON_MIC_FAILURE_              = 14,
	_RSON_4WAY_HNDSHK_TIMEOUT_      = 15,
	_RSON_GROUP_KEY_UPDATE_TIMEOUT_ = 16,
	_RSON_DIFF_IE_                  = 17,
	_RSON_MLTCST_CIPHER_NOT_VALID_  = 18,
	_RSON_UNICST_CIPHER_NOT_VALID_  = 19,
	_RSON_AKMP_NOT_VALID_           = 20,
	_RSON_UNSUPPORT_RSNE_VER_       = 21,
	_RSON_INVALID_RSNE_CAP_         = 22,
	_RSON_IEEE_802DOT1X_AUTH_FAIL_  = 23,

	//belowing are Realtek definition
	_RSON_PMK_NOT_AVAILABLE_        = 24,

	_RSON_USK_HANDSHAKE_TIMEOUT_    = 25,   /* handshake timeout for unicast session key*/
	_RSON_MSK_HANDSHAKE_TIMEOUT_    = 26,   /* handshake timeout for multicast session key*/
	_RSON_IE_NOT_CONSISTENT_            = 27,   /* IE was different between USK handshake & assocReq/probeReq/Beacon */
	_RSON_INVALID_USK_              = 28,   /* Invalid unicast key set */
	_RSON_INVALID_MSK_              = 29,   /* Invalid multicast key set */
	_RSON_INVALID_WAPI_VERSION_     = 30,   /* Invalid wapi version */
	_RSON_INVALID_WAPI_CAPABILITY_  = 31,   /* Wapi capability not support */

};

enum WIFI_STATUS_CODE {
	_STATS_SUCCESSFUL_              = 0,    // Success.
	_STATS_FAILURE_                 = 1,    // Failure.
	_STATS_CAP_FAIL_                = 10,   // Capability too wide, can't support
	_STATS_NO_ASOC_                 = 11,   // Denial reassociate
	_STATS_OTHER_                   = 12,   // Denial connect, not 802.11 standard.
	_STATS_NO_SUPP_ALG_             = 13,   // Authenticate algorithm not support .
	_STATS_OUT_OF_AUTH_SEQ_         = 14,   // Out of authenticate sequence number.
	_STATS_CHALLENGE_FAIL_          = 15,   // Denial authenticate, Response message fail.
	_STATS_AUTH_TIMEOUT_            = 16,   // Denial authenticate, timeout.
	_STATS_UNABLE_HANDLE_STA_       = 17,   // Denial authenticate, BS resoruce insufficient.
	_STATS_RATE_FAIL_               = 18,   // Denial authenticate, STA not support BSS request datarate.
	_STATS_REQ_DECLINED_        = 37,

	__STATS_INVALID_IE_ = 40,
	__STATS_INVALID_AKMP_ = 43,
	__STATS_CIPER_REJECT_ = 46,
	__STATS_INVALID_USK_ = 47,
	__STATS_INVALID_MSK_ = 48,
	__STATS_INVALID_WAPI_VERSION_ = 49,
	__STATS_INVALID_WAPI_CAPABILITY_ = 50,

};

/* ---------------------------------------------------------------------------
                    Below is the fixed elements...
-----------------------------------------------------------------------------*/
#define _AUTH_ALGM_NUM_         2
#define _AUTH_SEQ_NUM_          2
#define _BEACON_ITERVAL_        2
#define _CAPABILITY_            2
#define _CURRENT_APADDR_        6
#define _LISTEN_INTERVAL_       2
#define _RSON_CODE_             2
#define _ASOC_ID_               2
#define _STATUS_CODE_           2
#define _TIMESTAMP_             8

#define AUTH_ODD_TO             0
#define AUTH_EVEN_TO            1

#define WLAN_ETHCONV_ENCAP      1
#define WLAN_ETHCONV_RFC1042    2
#define WLAN_ETHCONV_8021h      3

/*-----------------------------------------------------------------------------
                Below is the definition for 802.11i / 802.1x
------------------------------------------------------------------------------*/
#define _IEEE8021X_MGT_         1       // WPA
#define _IEEE8021X_PSK_         2       // WPA with pre-shared key

#define _NO_PRIVACY_            0
#define _WEP_40_PRIVACY_        1
#define _TKIP_PRIVACY_          2
#define _WRAP_PRIVACY_          3
#define _CCMP_PRIVACY_          4
#define _WEP_104_PRIVACY_       5
#define _WEP_WPA_MIXED_PRIVACY_ 6   // WEP + WPA
#define _WAPI_SMS4_             7

/*-----------------------------------------------------------------------------
            Below is for QoS related definition
------------------------------------------------------------------------------*/
#define _WMM_IE_Length_             7
#define _WMM_Para_Element_Length_   24
#define _ADDBA_Req_Frame_Length_    9
#define _ADDBA_Rsp_Frame_Length_    9
#define _DELBA_Frame_Length         6
#define _ADDBA_Maximum_Buffer_Size_ 64

#define _BLOCK_ACK_CATEGORY_ID_     3
#define _ADDBA_Req_ACTION_ID_       0
#define _ADDBA_Rsp_ACTION_ID_       1
#define _DELBA_ACTION_ID_           2

/*-----------------------------------------------------------------------------
            Below is for HT related definition
------------------------------------------------------------------------------*/
#define _HT_CAP_                    45
#define _HT_IE_                     61

#define _HT_MIMO_PS_STATIC_         BIT(0)
#define _HT_MIMO_PS_DYNAMIC_        BIT(1)

#define _HT_MIMO_PS_Frame_Length_   3

#ifdef WIFI_11N_2040_COEXIST
#define _PUBLIC_CATEGORY_ID_        4
#define _2040_COEXIST_ACTION_ID_    0

#define _2040_BSS_COEXIST_IE_       72
#define _40M_INTOLERANT_            BIT(1)
#define _20M_BSS_WIDTH_REQ_     BIT(2)

#define _2040_Intolerant_ChRpt_IE_  73
#define _OBSS_SCAN_PARA_IE_     74

#define _EXTENDED_CAP_IE_           127
#define _2040_COEXIST_SUPPORT_  BIT(0)
#endif
#define _HT_CATEGORY_ID_            7
#define _HT_MIMO_PS_ACTION_ID_      1

/*-----------------------------------------------------------------------------
            Below is the bit definition for HT Capabilities element
------------------------------------------------------------------------------*/
#define _HTCAP_SUPPORT_CH_WDTH_     BIT(1)
#define _HTCAP_SMPWR_STATIC_        0
#define _HTCAP_SMPWR_DYNAMIC_       BIT(2)
#define _HTCAP_STATIC_SMPWR_        0
#define _HTCAP_SMPWR_DISABLE_       (BIT(2) | BIT(3))
#define _HTCAP_SHORTGI_20M_         BIT(5)
#define _HTCAP_SHORTGI_40M_         BIT(6)
#define _HTCAP_TX_STBC_             BIT(7)
#define _HTCAP_RX_STBC_1S_          BIT(8)
#define _HTCAP_RX_STBC_2S_          BIT(9)
#define _HTCAP_RX_STBC_3S_          (BIT(8) | BIT(9))
#define _HTCAP_RX_STBC_CAP_         (BIT(8) | BIT(9))
#define _HTCAP_AMSDU_LEN_8K_        BIT(11)
#define _HTCAP_CCK_IN_40M_          BIT(12)
#ifdef WIFI_11N_2040_COEXIST
#define _HTCAP_40M_INTOLERANT_      BIT(14)
#endif
#define _HTCAP_AMPDU_FAC_8K_        0
#define _HTCAP_AMPDU_FAC_16K_       BIT(0)
#define _HTCAP_AMPDU_FAC_32K_       BIT(1)
#define _HTCAP_AMPDU_FAC_64K_       (BIT(0) | BIT(1))
#define _HTCAP_AMPDU_SPC_SHIFT_     2
#define _HTCAP_AMPDU_SPC_MASK_      0x1c
#define _HTCAP_AMPDU_SPC_NORES_     0
#define _HTCAP_AMPDU_SPC_QUAR_US_   1
#define _HTCAP_AMPDU_SPC_HALF_US_   2
#define _HTCAP_AMPDU_SPC_1_US_      3
#define _HTCAP_AMPDU_SPC_2_US_      4
#define _HTCAP_AMPDU_SPC_4_US_      5
#define _HTCAP_AMPDU_SPC_8_US_      6
#define _HTCAP_AMPDU_SPC_16_US_     7

/*-----------------------------------------------------------------------------
            Below is the bit definition for HT Information element
------------------------------------------------------------------------------*/
#define _HTIE_2NDCH_OFFSET_NO_      0
#define _HTIE_2NDCH_OFFSET_AB_      BIT(0)
#define _HTIE_2NDCH_OFFSET_BL_      (BIT(0) | BIT(1))
#define _HTIE_STA_CH_WDTH_          BIT(2)
#define _HTIE_OP_MODE0_             0
#define _HTIE_OP_MODE1_             BIT(0)
#define _HTIE_OP_MODE2_             BIT(1)
#define _HTIE_OP_MODE3_             (BIT(0) | BIT(1))
#define _HTIE_NGF_STA_              BIT(2)
#define _HTIE_TXBURST_LIMIT_        BIT(3)
#define _HTIE_OBSS_NHT_STA_         BIT(4)

#endif  /* _WIFI_H_ */
