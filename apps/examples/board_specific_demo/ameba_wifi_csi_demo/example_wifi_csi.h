/* As example layer is unable to access BSP layer, some BSP specific structs have to be added in the header file manually */
/* CSI struct below is specific to RTL8730E, other chips might have some differences */

#define WIFI_CSI_DONE 27
#ifndef BITs
#define BIT(x)	((__u32)1 << (x))
#endif

/**
  * @brief csi trig management frame subtype.
  */
enum trig_frame_mgnt_type {
	CSI_TRIG_ASSOCREQ   = BIT(0),
	CSI_TRIG_ASSOCRSP   = BIT(1),
	CSI_TRIG_REASSOCREQ = BIT(2),
	CSI_TRIG_REASSOCRSP = BIT(3),
	CSI_TRIG_PROBEREQ   = BIT(4),
	CSI_TRIG_PROBERSP   = BIT(5),
	CSI_TRIG_BEACON     = BIT(8),
	CSI_TRIG_ATIM       = BIT(9),
	CSI_TRIG_DISASSOC   = BIT(10),
	CSI_TRIG_AUTH       = BIT(11),
	CSI_TRIG_DEAUTH     = BIT(12),
	CSI_TRIG_ACTION     = BIT(13)
};

/**
  * @brief csi trig control frame subtype.
  */
enum trig_frame_ctrl_type {
	CSI_TRIG_TRIGGER     = BIT(2),
	CSI_TRIG_BA          = BIT(9),
	CSI_TRIG_PSPOLL      = BIT(10),
	CSI_TRIG_RTS         = BIT(11),
	CSI_TRIG_CTS         = BIT(12),
	CSI_TRIG_ACK         = BIT(13),
	CSI_TRIG_CFEND       = BIT(14),
	CSI_TRIG_CFEND_CFACK = BIT(15)
};

/**
  * @brief csi trig data frame subtype.
  */
enum trig_frame_data_type {
	CSI_TRIG_DATA           = BIT(0),
	CSI_TRIG_DATA_CFACK     = BIT(1),
	CSI_TRIG_DATA_CFPOLL    = BIT(2),
	CSI_TRIG_DATA_CFACKPOLL = BIT(3),
	CSI_TRIG_DATA_NULL      = BIT(4),
	CSI_TRIG_CF_ACK         = BIT(5),
	CSI_TRIG_CF_POLL        = BIT(6),
	CSI_TRIG_CF_ACKPOLL     = BIT(7),
	CSI_TRIG_QOS_DATA       = BIT(8),
	CSI_TRIG_QOS_DATA_NULL	= BIT(12)
};

/**
  * @brief csi enable or config
  */
enum rtw_csi_action_type {
	CSI_ACT_EN,    /**< enable or disable csi func */
	CSI_ACT_CFG,  /**< config csi parameters */
	CSI_ACT_MAX
};

/**
  * @brief csi group num
  */
enum rtw_csi_group_type {
	CSI_GROUP_NUM_1 = 0,  /**< per tone */
	CSI_GROUP_NUM_2,         /**< per 2tone */
	CSI_GROUP_NUM_4,        /**< per 4tone */
	CSI_GROUP_NUM_16,     /**< per 16tone */
	CSI_GROUP_NUM_MAX
};

/**
  * @brief csi mode
  */
enum rtw_csi_mode_type {
	CSI_MODE_NORMAL = 0,   ///<   normal mode
	CSI_MODE_NDP,                ///<   ndp mode
	CSI_MODE_RX_RESP,         ///<    rx rsp mode
	CSI_MODE_MAX,                 ///<    max mode
};

/**
  * @brief csi accuracy.
  */
enum rtw_csi_accuracy_type {
	CSI_ACCU_1BYTE = 0, /**< CSI_ACCU_1BYTE: S(8,4) */
	CSI_ACCU_2BYTES,  /**< CSI_ACCU_2BYTE: S(16,12) */
	CSI_ACCU_MAX
};

/**
  * @brief csi alg_opt.
  */
enum rtw_csi_alg_opt_type {
	CSI_ALG_LS = 0,  /**< ls algo */
	CSI_ALG_SMOTHING, /**< smothing algo */
	CSI_ALG_MAX            /**< other algo */
};

/**
  * @brief csi ch_opt.
  */
enum rtw_csi_ch_opt_type {
	CSI_CH_LEGACY = 0, /**< legacy part(L-LTF) channel estmation result */
	CSI_CH_NON_LEGACY,  /**< non-legacy(HT-LTF) part */
	CSI_CH_MAX
};

/**
  * @brief csi play_role for sta mode.
  */
enum rtw_csi_op_role {
	CSI_OP_ROLE_TRX = 0,  /**< both trx */
	CSI_OP_ROLE_TX  = 1,  /**< only tx csi triggering frame */
	CSI_OP_ROLE_RX  = 2,  /**< only rx csi triggering frame for fetching csi report */
	CSI_OP_ROLE_MAX
};

/**
  * @brief  The structure is used to describe the cfg parameters used for csi report,
  * @note  The mac_addr if not specified, the default value must be 0.
  */
typedef struct _rtw_csi_action_parm_t {
	enum rtw_csi_group_type group_num;
	enum rtw_csi_accuracy_type accuracy;
	enum rtw_csi_alg_opt_type alg_opt;
	enum rtw_csi_ch_opt_type ch_opt;
	enum rtw_csi_op_role csi_role; /* indicate csi operation role */
	enum rtw_csi_mode_type mode;
	enum rtw_csi_action_type act;
	unsigned short trig_frame_mgnt; /* indicate management frame subtype of rx csi triggering frame for fetching csi*/
	unsigned short trig_frame_ctrl; /* indicate control frame subtype of rx csi triggering frame for fetching csi*/
	unsigned short trig_frame_data; /* indicate data frame subtype of rx csi triggering frame for fetching csi*/
	unsigned char enable;
	unsigned char trig_period;
	unsigned char data_rate;
	unsigned char data_bw;
	unsigned char mac_addr[6];
	unsigned char multi_type;     /* 0-uc csi triggering frame; 1-bc csi triggering frame */
	unsigned char trig_flag;      /* indicate source of device for triggering csi[sta<->sta]: 4bits >> 1 ~ 15 */
} rtw_csi_action_parm_t;

/**
  * @brief  The structure is used to store the WIFI setting gotten from WIFI driver.
  * @note	size can't be changed
  */
 struct _rtw_wifi_setting_t {
	//enum rtw_mode_type			mode;   /**< the mode of current wlan interface */
	unsigned char 		mode;
	unsigned char 		ssid[33];   /**< the ssid of connected AP or softAP */
	unsigned char		bssid[6];   /**< the bssid of connected AP or softAP */
	unsigned char		channel;
	//enum rtw_security		security_type;   /**< the security type of connected AP or softAP */
	int 				security_type;
	//unsigned char 		password[RTW_MAX_PSK_LEN];   /**< the password of connected AP or softAP */
	unsigned char 		password[128];
	unsigned char		key_idx;
	unsigned char		iw_mode;	/**< RTK_IW_MODE */
	unsigned char		alg;		/**< encryption algorithm */
	unsigned int		auth_type;
	unsigned char		is_wps_trigger;	/**< connection triggered by WPS process**/
	unsigned int		rom_rsvd;
};

/**
  * @brief  The structure is used to describe the extra info of csi report
  */
typedef struct {
	unsigned char mac_addr[6];  /**< may be sta addr, driver define */
	unsigned char trig_addr[6];  /**< add new,tx addr for trig chan_info */
	unsigned int hw_assigned_timestamp;  /**< rxdesc: u32 r_rx_tsfl */
	unsigned char channel;  /**< driver define */
	unsigned char bandwidth; /**< rxdesc: u8 bw */
	unsigned char rx_data_rate;  /**< rxdesc: u16 rx_rate <ack> */
	unsigned char protocol_mode; /**< add new,ofdm(0)/ht(1)/vht(2)/he(3) */
	unsigned char nc;  /**< ch_rpt_hdr_info */
	unsigned char nr;  /**< ch_rpt_hdr_info */
	unsigned short num_sub_carrier;  /**< cfg param, driver define */
	unsigned char num_bit_per_tone;  /**< cfg param, driver define per I/Q */
	unsigned char avg_idle_noise_pwr;  /**< ch_rpt_hdr_info */
	unsigned char evm[2];  /**< ch_rpt_hdr_info + phy_info_rpt */
	unsigned char rssi[2];  /**< phy_info_rpt */
	unsigned int csi_data_length;  /**< ch_rpt_hdr_info */
	unsigned char rxsc;  /**< phy_info_rpt */
	unsigned char csi_valid;  /**< ch_rpt_hdr_info */
} rtw_csi_header_t;
