/* This is a reduced header file from wifi_api_types.h containing items needed for example_wifi_csi.c */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define STA_WLAN_INDEX	    0
#define SOFTAP_WLAN_INDEX	1

#define RTW_WPA3_MAX_PSK_LEN		(128)		/**< Maximum PSK length for WPA3. */
#define RTW_MAX_PSK_LEN		RTW_WPA3_MAX_PSK_LEN /**< Maximum PSK length used. */

#define MAC_ARG(x)		((u8*)(x))[0],((u8*)(x))[1],\
				((u8*)(x))[2],((u8*)(x))[3],\
				((u8*)(x))[4],((u8*)(x))[5]      /**< Formats MAC address for printing. Usage Example: RTK_LOGS(NOTAG, RTK_LOG_INFO, "MAC addr="MAC_FMT"\n", MAC_ARG(mac_addr));*/
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"  /**< Format string for printing MAC address.*/

/**********************************************************************************************
 *                                     csi enum
 *********************************************************************************************/
/**
  * @brief CSI triggering management frame subtypes (size: u16).
  */
enum rtw_csi_trig_frame_mgnt {
	RTW_CSI_TRIG_ASSOCREQ   = BIT(0),
	RTW_CSI_TRIG_ASSOCRSP   = BIT(1),
	RTW_CSI_TRIG_REASSOCREQ = BIT(2),
	RTW_CSI_TRIG_REASSOCRSP = BIT(3),
	RTW_CSI_TRIG_PROBEREQ   = BIT(4),
	RTW_CSI_TRIG_PROBERSP   = BIT(5),
	RTW_CSI_TRIG_BEACON     = BIT(8),
	RTW_CSI_TRIG_ATIM       = BIT(9),
	RTW_CSI_TRIG_DISASSOC   = BIT(10),
	RTW_CSI_TRIG_AUTH       = BIT(11),
	RTW_CSI_TRIG_DEAUTH     = BIT(12),
	RTW_CSI_TRIG_ACTION     = BIT(13)
};

/**
  * @brief CSI triggering control frame subtypes (size: u16).
  */
enum rtw_csi_trig_frame_ctrl {
	RTW_CSI_TRIG_TRIGGER     = BIT(2),
	RTW_CSI_TRIG_BA          = BIT(9),
	RTW_CSI_TRIG_PSPOLL      = BIT(10),
	RTW_CSI_TRIG_RTS         = BIT(11),
	RTW_CSI_TRIG_CTS         = BIT(12),
	RTW_CSI_TRIG_ACK         = BIT(13),
	RTW_CSI_TRIG_CFEND       = BIT(14),
	RTW_CSI_TRIG_CFEND_CFACK = BIT(15)
};

/**
  * @brief CSI triggering data frame subtypes (size: u16).
  */
enum rtw_csi_trig_frame_data {
	RTW_CSI_TRIG_DATA           = BIT(0),
	RTW_CSI_TRIG_DATA_CFACK     = BIT(1),
	RTW_CSI_TRIG_DATA_CFPOLL    = BIT(2),
	RTW_CSI_TRIG_DATA_CFACKPOLL = BIT(3),
	RTW_CSI_TRIG_DATA_NULL      = BIT(4),
	RTW_CSI_TRIG_CF_ACK         = BIT(5), /**< CF-Ack (no data) */
	RTW_CSI_TRIG_CF_POLL        = BIT(6),
	RTW_CSI_TRIG_CF_ACKPOLL     = BIT(7),
	RTW_CSI_TRIG_QOS_DATA       = BIT(8),
	RTW_CSI_TRIG_QOS_DATA_NULL	= BIT(12)
};

/**
  * @brief CSI action types for wifi_csi_config() (size: u8).
  */
enum rtw_csi_action_type {
	RTW_CSI_ACT_EN,           /**< enable or disable csi func */
	RTW_CSI_ACT_CFG,          /**< config csi parameters */
	RTW_CSI_ACT_MAX
};

/**
  * @brief CSI info subcarrier decimation for reducing data volume (size: u8).
  */
enum rtw_csi_group_num {
	RTW_CSI_GROUP_NUM_1 = 0,  /**< per tone */
	RTW_CSI_GROUP_NUM_2,      /**< per 2tone */
	RTW_CSI_GROUP_NUM_4,      /**< per 4tone */
	RTW_CSI_GROUP_NUM_8_16,   /**< per 8tone for dplus; per 16tone for others */
	RTW_CSI_GROUP_NUM_MAX
};

/**
  * @brief CSI mode for fetching CSI info (size: u8).
  */
enum rtw_csi_mode {
	RTW_CSI_MODE_NORMAL = 0,  /**< Rx normal mode (estimating CSI by the currently received packet). */
	RTW_CSI_MODE_NDP,         /**< Rx ndp mode: not support */
	RTW_CSI_MODE_RX_RESP,     /**< Rx response mode (estimating CSI by receiving ACK for the previous transmission). */
	RTW_CSI_MODE_MAX,
};

/**
  * @brief  CSI accuracy specifies the format of CSI raw data represented as complex numbers (size: u8).
  * @note
  *       - Each CSI data point consists of an I (In-phase, real part) and Q (Quadrature, imaginary part) value.
  *         Both I and Q use the same format for a given accuracy setting.
  *       - For example, the format S(8,3) means a signed 8-bit number with 3 fractional bits.
  */
enum rtw_csi_accuracy {
	RTW_CSI_ACCU_1BYTE = 0,   /**< S(8,3) for dplus and S(8,4) for others */
	RTW_CSI_ACCU_2BYTES,      /**< S(16,11) for dplus and S(16,12) for others */
	RTW_CSI_ACCU_MAX
};

/**
  * @brief CSI algorithm options (size: u8).
  */
enum rtw_csi_alg {
	RTW_CSI_ALG_LS = 0,       /**< Least Squares algorithm */
	RTW_CSI_ALG_SMOTHING,     /**< Smoothing algorithm */
	RTW_CSI_ALG_MAX           /**< Other algorithm */
};

/**
  * @brief CSI acquisition from leagcy or non-legacy LTF (size: u8).
  */
enum rtw_csi_ch {
	RTW_CSI_CH_LEGACY = 0,    /**< legacy part(L-LTF) channel estmation result */
	RTW_CSI_CH_NON_LEGACY,    /**< non-legacy(HT-LTF) part */
	RTW_CSI_CH_MAX
};

/**
  * @brief CSI operation role (size: u8).
  */
enum rtw_csi_role {
	RTW_CSI_OP_ROLE_TRX = 0,  /**< both trx */
	RTW_CSI_OP_ROLE_TX  = 1,  /**< only tx csi triggering frame */
	RTW_CSI_OP_ROLE_RX  = 2,  /**< only rx csi triggering frame for fetching csi report */
	RTW_CSI_OP_ROLE_MAX
};

/**
  * @brief Supported PHY rates (size: u8):
  *        - CCK rates: 1M, 2M, 5.5M, 11M
  *        - OFDM rates: 6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M
  *        - HT rates: MCS0 to MCS7
  *        - VHT rates: MCS0 to MCS8
  *        - HE rates: MCS0 to MCS9
  * @note For HT/VHT/HE PHY, only single spatial stream is supported.
  */
enum rtw_rate {
	RTW_RATE_1M		= 0x02,     /**< 0x02 */
	RTW_RATE_2M		= 0x04,     /**< 0x04 */
	RTW_RATE_5_5M	= 0x0B,     /**< 0x0B */
	RTW_RATE_6M		= 0x0C,     /**< 0x0C */
	RTW_RATE_9M		= 0x12,     /**< 0x12 */
	RTW_RATE_11M 	= 0x16,     /**< 0x16 */
	RTW_RATE_12M = 0x18,      /**< 0x18 */
	RTW_RATE_18M = 0x24,      /**< 0x24 */
	RTW_RATE_24M = 0x30,      /**< 0x30 */
	RTW_RATE_36M = 0x48,      /**< 0x48 */
	RTW_RATE_48M = 0x60,      /**< 0x60 */
	RTW_RATE_54M = 0x6C,      /**< 0x6C */
	RTW_RATE_MCS32	= 0x7F,  /**< 0x7f */
	RTW_RATE_MCS0,   /**< 0x80 */
	RTW_RATE_MCS1,   /**< 0x81 */
	RTW_RATE_MCS2,   /**< 0x82 */
	RTW_RATE_MCS3,   /**< 0x83 */
	RTW_RATE_MCS4,   /**< 0x84 */
	RTW_RATE_MCS5,   /**< 0x85 */
	RTW_RATE_MCS6,
	RTW_RATE_MCS7,

	RTW_RATE_VHT1SS_MCS0 = 0xa0,
	RTW_RATE_VHT1SS_MCS1,
	RTW_RATE_VHT1SS_MCS2,
	RTW_RATE_VHT1SS_MCS3,
	RTW_RATE_VHT1SS_MCS4,
	RTW_RATE_VHT1SS_MCS5,
	RTW_RATE_VHT1SS_MCS6,
	RTW_RATE_VHT1SS_MCS7,
	RTW_RATE_VHT1SS_MCS8,
	RTW_RATE_VHT1SS_MCS9,

	RTW_RATE_HE1SS_MCS0 = 0xd0,   /**< 0xd0 */
	RTW_RATE_HE1SS_MCS1,
	RTW_RATE_HE1SS_MCS2,
	RTW_RATE_HE1SS_MCS3,
	RTW_RATE_HE1SS_MCS4,
	RTW_RATE_HE1SS_MCS5,
	RTW_RATE_HE1SS_MCS6,
	RTW_RATE_HE1SS_MCS7,
	RTW_RATE_HE1SS_MCS8,
	RTW_RATE_HE1SS_MCS9,
	RTW_RATE_HE1SS_MCS10,
	RTW_RATE_HE1SS_MCS11,

	RTW_RATE_UNKNOWN = 0xff  /**< 0xff */
};


/**********************************************************************************************
 *                                     csi structures
 *********************************************************************************************/
/**
  * @brief  Configuration parameters used for csi report.
  * @note  The mac_addr if not specified, the default value must be 0.
  */
struct rtw_csi_action_parm {
	u8 group_num;   /**< CSI info subcarrier decimation. val: @ref RTW_CSI_GROUP_NUM_1, @ref RTW_CSI_GROUP_NUM_2... */
	u8 accuracy;    /**< The format (bit length) of CSI raw data for both I and Q components. val: @ref RTW_CSI_ACCU_1BYTE, @ref RTW_CSI_ACCU_2BYTES */
	u8 alg_opt;     /**< Reserved. */
	u8 ch_opt;      /**< CSI acquisition from leagcy or non-legacy LTF. val: @ref RTW_CSI_CH_LEGACY, @ref RTW_CSI_CH_NON_LEGACY. */
	u8 csi_role;    /**< The CSI operation role, val: @ref RTW_CSI_OP_ROLE_TRX, @ref RTW_CSI_OP_ROLE_TX, @ref RTW_CSI_OP_ROLE_RX. */
	u8 mode;        /**< Mode for fetching CSI. val: @ref RTW_CSI_MODE_NORMAL, @ref RTW_CSI_MODE_RX_RESP*/
	u8 act;         /**< Enable CSI or configure CSI parameters. val: @ref RTW_CSI_ACT_EN, @ref RTW_CSI_ACT_CFG. */
	u16 trig_frame_mgnt;	/**< Management frame type(s) of rx CSI triggering frame. Used for @ref RTW_CSI_MODE_NORMAL. val: @ref RTW_CSI_TRIG_ASSOCREQ... */
	u16 trig_frame_ctrl;	/**< Control frame type(s) of rx CSI triggering frame. Used for @ref RTW_CSI_MODE_NORMAL. val: @ref RTW_CSI_TRIG_TRIGGER... */
	u16 trig_frame_data;	/**< Data frame type(s) of rx CSI triggering frame. Used for @ref RTW_CSI_MODE_NORMAL. val: @ref RTW_CSI_TRIG_DATA... */
	u8 enable;      /**< 0: disable Wi-Fi CSI function; 1: enable Wi-Fi CSI function. */
	u8 trig_period; /**< CSI sounding rate, unit: 320us (recommended value: 15~255). */
	u8 data_rate; /**< Tx data rate of CSI triggering frame, invalid in @ref RTW_CSI_MODE_RX_RESP mode. val: only support OFDM/HT rate. */
	u8 data_bw;   /**< The bandwidth of CSI triggering frame, invalid in @ref RTW_CSI_MODE_RX_RESP mode. val: 0 for 20MHz, 1 for 40MHz (only supported by dplus). */
	u8 mac_addr[6]; /**< Destination address (MAC address) for CSI triggering frame, invalid when set `multi_type = 1`.*/
	u8 multi_type;  /**< 0: unicast csi triggering frame; 1: broadcast csi triggering frame. Valid in Active CSI. */

	/** Indicate role for transmitting CSI triggering frame in METHOD4 and
	 * role for transmitting response ACK for CSI triggering frame in METHOD1_Variant,
	 * others are reserved. \n
	 * Value=1 ~ 15 (0 is reserved)*/
	u8 trig_flag;
};

/**
  * @brief  Represents a unique 6-byte MAC address.
  */
struct rtw_mac {
	u8		octet[6]; /**< Unique 6-byte MAC address. */
};

/**
  * @brief  Stores information about clients associated with the SoftAP.
  */
struct rtw_client_list {
	u32    count;         /**< Number of associated clients.    */
	struct rtw_mac mac_list[MACID_HW_MAX_NUM - 2]; /**< Array of client MAC addresses. */
};

/**
  * @brief  Stores Wi-Fi settings retrieved from the Wi-Fi driver.
  * @note	Structure size must remain unchanged for compatibility.
  */
struct rtw_wifi_setting {
	u8		mode;   /**< Current WLAN interface mode: @ref RTW_MODE_STA, @ref RTW_MODE_AP, @ref RTW_MODE_NAN. */
	u8 		ssid[33];   /**< SSID of connected AP or SoftAP (null-terminated string). */
	u8		bssid[6];   /**< BSSID (MAC address) of connected AP or SoftAP. */
	u8		channel;    /**< Operating channel of connected AP or SoftAP. */
	u32		security_type; /**< Security type of connected AP or SoftAP: @ref RTW_SECURITY_OPEN, @ref RTW_SECURITY_WEP_PSK, etc.*/
	u8 		password[RTW_MAX_PSK_LEN];   /**< Password of connected AP or SoftAP. */
	u8		key_idx;      /**< WEP key index (only valid when security type is WEP).*/
	u8		alg;		/**< Reserved for internal driver compatibility; users can ignore. */
	u32		auth_type;  /**< Reserved for internal driver use; users can ignore. */
	u8		is_wps_trigger;	/**< Indicates if connection was triggered by WPS process. */
	u32		rom_rsvd;
};

#ifdef __cplusplus
}
#endif
