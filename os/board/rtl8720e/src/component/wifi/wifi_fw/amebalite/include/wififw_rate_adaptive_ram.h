
#ifndef __WIFIFW_RATE_ADAPTIVE_RAM__
#define __WIFIFW_RATE_ADAPTIVE_RAM__

#if CONFIG_RATE_ADAPTIVE


//#define RA_TIMER_PERIOD 100 // 100 milli-second
//#define nonMU_MACIDNUM 128
//#define nonRU_MACIDNUM nonMU_MACIDNUM+30


#define BW20M   0x0
#define BW40M   0x100
#define BW80M   0x200
#define BW160M  0x300

#define START_RATE_RSSI_LV1 55
#define START_RATE_RSSI_LV2 35
#define START_RATE_RSSI_LV3 20

#define HE_RA_MASK_DWORD_OFFSET_0 0x8
#define HE_RA_MASK_DWORD_OFFSET_1 0xC

#define VHT_RA_MASK_DWORD_OFFSET_0 0x0
#define VHT_RA_MASK_DWORD_OFFSET_1 0x4

#define SGI_PENDING_CNT 3

#define MU_SU_rate_diff_TH 2

// decision offset timer mechanism
#define DECISION_OFFSET_CNT 20

#define PHYSTS_TYPE_NUM 32

#define CONFIG_RA_DBG_LOG       0
#if CONFIG_RA_DBG_LOG
#define DBG_RA_LOG   DBG_8195A
#else
#define DBG_RA_LOG(...)
#endif

#define RA_Pending_Code 	0
/*@--------------------------[Enum]------------------------------------------*/



enum DECISION_OFFSET_STATE {
	D_O_INIT = 0, //init and not lowest rate => ready to rate_down
	D_O_RATE_DOWN_UPDATE = 1, //timer=0 => ready to rate_down
	D_O_RATE_RESTORE = 2, // already rate_down => rate_restore, timer=default
	D_O_COUNTING_DOWN = 3, // env is noisy => timer--
	D_O_FORCE_UPDATE = 4 // env is clean || lowest_rate => update d_o, timer=default
};

/**
 * @enum ASSOC_MD
 *
 * @brief ASSOC_MD
 *
 * @var ASSOC_MD::TYPE_CCK
 * Please Place Description here.
 * @var ASSOC_MD::TYPE_LEGOFDM
 * Please Place Description here.
 * @var ASSOC_MD::TYPE_HT
 * Please Place Description here.
 * @var ASSOC_MD::TYPE_VHT
 * Please Place Description here.
 * @var ASSOC_MD::TYPE_HE
 * Please Place Description here.
 */
enum ASSOC_MD {
	TYPE_CCK             = BIT0,
	TYPE_LEGOFDM            = BIT1,
	TYPE_HT           = BIT2,
	TYPE_VHT           = BIT3,
	TYPE_HE           = BIT4
};

#if 0
enum ENABLE_MODE {
	TYPE_EN_CCK  = 0x1,
	TYPE_EN_OFDM = 0x2,
	TYPE_EN_HT = 0x4,
	TYPE_EN_VHT = 0x8,
	TYPE_EN_HE = 0x16,

};
#else

/**
 * @enum ENABLE_MODE
 *
 * @brief ENABLE_MODE
 *
 * @var ENABLE_MODE::TYPE_EN_CCK
 * Please Place Description here.
 * @var ENABLE_MODE::TYPE_EN_OFDM
 * Please Place Description here.
 * @var ENABLE_MODE::TYPE_EN_HT
 * Please Place Description here.
 * @var ENABLE_MODE::TYPE_EN_VHT
 * Please Place Description here.
 * @var ENABLE_MODE::TYPE_EN_HE
 * Please Place Description here.
 */
enum ENABLE_MODE {
	TYPE_EN_CCK  = 0x1,
	TYPE_EN_OFDM = 0x2,
	TYPE_EN_HT = 0x4,
	TYPE_EN_VHT = 0x8,
	TYPE_EN_HE = 0x10,

};
#endif


/**
 * @enum MASK_MD
 *
 * @brief MASK_MD
 *
 * @var MASK_MD::TYPE_MASK_CCK
 * Please Place Description here.
 * @var MASK_MD::TYPE_MASK_LEGOFDM
 * Please Place Description here.
 * @var MASK_MD::TYPE_MASK_HT
 * Please Place Description here.
 * @var MASK_MD::TYPE_MASK_VHT
 * Please Place Description here.
 * @var MASK_MD::TYPE_MASK_HE
 * Please Place Description here.
 */
enum MASK_MD {
	TYPE_MASK_CCK   = 0xf,
	TYPE_MASK_LEGOFDM   = 0xff0,
	TYPE_MASK_HT    = 0x00ff0ff0ff0ff000,
	TYPE_MASK_VHT   = 0x03ff3ff3ff3ff000,
	TYPE_MASK_HE   = 0x0ffffffffffff000
};


/**
 * @enum MASK_SS
 *
 * @brief MASK_SS
 *
 * @var MASK_SS::TYPE_MASK_1SS
 * Please Place Description here.
 * @var MASK_SS::TYPE_MASK_2SS
 * Please Place Description here.
 * @var MASK_SS::TYPE_MASK_3SS
 * Please Place Description here.
 * @var MASK_SS::TYPE_MASK_4SS
 * Please Place Description here.
 */
enum MASK_SS {
	TYPE_MASK_1SS   = 0xffffff,
	TYPE_MASK_2SS   = 0x0000000fffffffff,
	TYPE_MASK_3SS    = 0x0000ffffffffffff,
	TYPE_MASK_4SS   = 0x0fffffffffffffff
};


/**
 * @enum MAX_SS
 *
 * @brief MAX_SS
 *
 * @var MAX_SS::TYPE_1SS
 * Please Place Description here.
 * @var MAX_SS::TYPE_2SS
 * Please Place Description here.
 * @var MAX_SS::TYPE_3SS
 * Please Place Description here.
 * @var MAX_SS::TYPE_4SS
 * Please Place Description here.
 */
enum MAX_SS {
	TYPE_1SS   = 0,
	TYPE_2SS   = 1,
	TYPE_3SS    = 2,
	TYPE_4SS   = 3
};


/**
 * @enum WIFI_MODE
 *
 * @brief WIFI_MODE
 *
 * @var WIFI_MODE::TYPE_B
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_AG
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_BG
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_N
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_BN
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_GN
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_BGN
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_AC
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_5G_AC
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_2G_AC
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_5G_AX
 * Please Place Description here.
 * @var WIFI_MODE::TYPE_2G_AX
 * Please Place Description here.
 */
enum WIFI_MODE {
	TYPE_B   = 1,
	TYPE_AG   = 2,
	TYPE_BG    = 3,
	TYPE_N   = 4,
	TYPE_BN     = 5,
	TYPE_GN     = 6,
	TYPE_BGN    = 7,
	TYPE_AC     = 8,
	TYPE_5G_AC   = 10,
	TYPE_2G_AC  = 11,
	TYPE_5G_AX  = 18,
	TYPE_2G_AX  = 19
};

/*typedef enum _VHT_HT_SWITCH_ {
    TYPE_HT             = 0,
    TYPE_VHT            = 1,
    TYPE_MIX1           = 2,
    TYPE_MIX2           = 3
} VHT_SEL_SWITCH, *PVHT_SEL_SWITCH;*/


/**
 * @enum _BW_TYPE_
 *
 * @brief _BW_TYPE_
 *
 * @var _BW_TYPE_::TYPE_BW_20M
 * Please Place Description here.
 * @var _BW_TYPE_::TYPE_BW_40M
 * Please Place Description here.
 * @var _BW_TYPE_::TYPE_BW_80M
 * Please Place Description here.
 * @var _BW_TYPE_::TYPE_BW_160M
 * Please Place Description here.
 */
typedef enum _BW_TYPE_ {
	TYPE_BW_20M         = 0,
	TYPE_BW_40M         = 1,
	TYPE_BW_80M         = 2,
	TYPE_BW_160M        = 3
} BW_TYPE, *PBW_TYPE;


/**
 * @enum phydm_ctrl_info_rate
 *
 * @brief phydm_ctrl_info_rate
 *
 * @var phydm_ctrl_info_rate::ODM_RATE1M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE2M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE5_5M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE11M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE6M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE9M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE12M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE18M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE24M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE36M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE48M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATE54M
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS0
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS1
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS2
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS3
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS4
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS5
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS6
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS7
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS8
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS9
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS10
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS11
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS12
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS13
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS14
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS15
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS16
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS17
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS18
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS19
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS20
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS21
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS22
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS23
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS24
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS25
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS26
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS27
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS28
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS29
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS30
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEMCS31
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS0
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS1
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS2
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS3
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS4
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS5
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS6
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS7
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS8
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS1MCS9
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS0
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS1
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS2
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS3
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS4
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS5
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS6
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS7
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS8
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS2MCS9
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS0
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS1
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS2
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS3
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS4
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS5
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS6
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS7
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS8
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS3MCS9
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS0
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS1
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS2
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS3
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS4
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS5
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS6
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS7
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS8
 * Please Place Description here.
 * @var phydm_ctrl_info_rate::ODM_RATEVHTSS4MCS9
 * Please Place Description here.
 */
enum phydm_ctrl_info_rate {
	ODM_RATE1M      = 0x00,
	ODM_RATE2M      = 0x01,
	ODM_RATE5_5M        = 0x02,
	ODM_RATE11M     = 0x03,
	/* OFDM Rates, TxHT = 0 */
	ODM_RATE6M      = 0x04,
	ODM_RATE9M      = 0x05,
	ODM_RATE12M     = 0x06,
	ODM_RATE18M     = 0x07,
	ODM_RATE24M     = 0x08,
	ODM_RATE36M     = 0x09,
	ODM_RATE48M     = 0x0A,
	ODM_RATE54M     = 0x0B,
	/* @MCS Rates, TxHT = 1 */
	ODM_RATEMCS0        = 0x0C,
	ODM_RATEMCS1        = 0x0D,
	ODM_RATEMCS2        = 0x0E,
	ODM_RATEMCS3        = 0x0F,
	ODM_RATEMCS4        = 0x10,
	ODM_RATEMCS5        = 0x11,
	ODM_RATEMCS6        = 0x12,
	ODM_RATEMCS7        = 0x13,
	ODM_RATEMCS8        = 0x14,
	ODM_RATEMCS9        = 0x15,
	ODM_RATEMCS10       = 0x16,
	ODM_RATEMCS11       = 0x17,
	ODM_RATEMCS12       = 0x18,
	ODM_RATEMCS13       = 0x19,
	ODM_RATEMCS14       = 0x1A,
	ODM_RATEMCS15       = 0x1B,
	ODM_RATEMCS16       = 0x1C,
	ODM_RATEMCS17       = 0x1D,
	ODM_RATEMCS18       = 0x1E,
	ODM_RATEMCS19       = 0x1F,
	ODM_RATEMCS20       = 0x20,
	ODM_RATEMCS21       = 0x21,
	ODM_RATEMCS22       = 0x22,
	ODM_RATEMCS23       = 0x23,
	ODM_RATEMCS24       = 0x24,
	ODM_RATEMCS25       = 0x25,
	ODM_RATEMCS26       = 0x26,
	ODM_RATEMCS27       = 0x27,
	ODM_RATEMCS28       = 0x28,
	ODM_RATEMCS29       = 0x29,
	ODM_RATEMCS30       = 0x2A,
	ODM_RATEMCS31       = 0x2B,
	ODM_RATEVHTSS1MCS0  = 0x2C,
	ODM_RATEVHTSS1MCS1  = 0x2D,
	ODM_RATEVHTSS1MCS2  = 0x2E,
	ODM_RATEVHTSS1MCS3  = 0x2F,
	ODM_RATEVHTSS1MCS4  = 0x30,
	ODM_RATEVHTSS1MCS5  = 0x31,
	ODM_RATEVHTSS1MCS6  = 0x32,
	ODM_RATEVHTSS1MCS7  = 0x33,
	ODM_RATEVHTSS1MCS8  = 0x34,
	ODM_RATEVHTSS1MCS9  = 0x35,
	ODM_RATEVHTSS2MCS0  = 0x36,
	ODM_RATEVHTSS2MCS1  = 0x37,
	ODM_RATEVHTSS2MCS2  = 0x38,
	ODM_RATEVHTSS2MCS3  = 0x39,
	ODM_RATEVHTSS2MCS4  = 0x3A,
	ODM_RATEVHTSS2MCS5  = 0x3B,
	ODM_RATEVHTSS2MCS6  = 0x3C,
	ODM_RATEVHTSS2MCS7  = 0x3D,
	ODM_RATEVHTSS2MCS8  = 0x3E,
	ODM_RATEVHTSS2MCS9  = 0x3F,
	ODM_RATEVHTSS3MCS0  = 0x40,
	ODM_RATEVHTSS3MCS1  = 0x41,
	ODM_RATEVHTSS3MCS2  = 0x42,
	ODM_RATEVHTSS3MCS3  = 0x43,
	ODM_RATEVHTSS3MCS4  = 0x44,
	ODM_RATEVHTSS3MCS5  = 0x45,
	ODM_RATEVHTSS3MCS6  = 0x46,
	ODM_RATEVHTSS3MCS7  = 0x47,
	ODM_RATEVHTSS3MCS8  = 0x48,
	ODM_RATEVHTSS3MCS9  = 0x49,
	ODM_RATEVHTSS4MCS0  = 0x4A,
	ODM_RATEVHTSS4MCS1  = 0x4B,
	ODM_RATEVHTSS4MCS2  = 0x4C,
	ODM_RATEVHTSS4MCS3  = 0x4D,
	ODM_RATEVHTSS4MCS4  = 0x4E,
	ODM_RATEVHTSS4MCS5  = 0x4F,
	ODM_RATEVHTSS4MCS6  = 0x50,
	ODM_RATEVHTSS4MCS7  = 0x51,
	ODM_RATEVHTSS4MCS8  = 0x52,
	ODM_RATEVHTSS4MCS9  = 0x53,
};

enum rtw_gi_ltf {
	GILTF_LGI_4XHE32 = 0,
	GILTF_SGI_4XHE08 = 1,
	GILTF_2XHE16 = 2,
	GILTF_2XHE08 = 3,
	GILTF_1XHE16 = 4,
	GILTF_1XHE08 = 5,
	GILTF_MAX
};

enum  bb_opt_gi_ltf {
	BB_OPT_GILTF_4XHE08 = 0,
	BB_OPT_GILTF_1XHE08 = 1
};




// Ben add for code size reduction. 11AX is TBTT RA not pkt based
//#define CONFIG_PKT_BASED_RA 0
/**
 * @enum FWBB_DBG_COMP
 *
 * @brief FWBB_DBG_COMP
 *
 * @var FWBB_DBG_COMP::FWBBDBG_H2C
 * Please Place Description here.
 * @var FWBB_DBG_COMP::FWBBDBG_TRIG_TX
 * Please Place Description here.
 * @var FWBB_DBG_COMP::FWBBDBG_EDCCA
 * Please Place Description here.
 * @var FWBB_DBG_COMP::FWBBDBG_RA
 * Please Place Description here.
 * @var FWBB_DBG_COMP::FWBBDBG_RUA
 * Please Place Description here.
 * @var FWBB_DBG_COMP::FWBBDBG_ULPWR_CTRL
 * Please Place Description here.
 * @var FWBB_DBG_COMP::FWBBDBG_ULRA
 * Please Place Description here.
 */
enum FWBB_DBG_COMP {
	FWBBDBG_H2C = BIT0,
	//==========RA==========//
	FWBBDBG_RA = BIT1, // ra main debug log: rty ratio,
	FWBBDBG_RA_SEARCH_RATE = BIT2,
	FWBBDBG_RA_TXRPT = BIT4,
	FWBBDBG_RA_TRY_RATE = BIT5,
	FWBBDBG_MU = BIT6,
	//====================//
	FWBBDBG_MCC = BIT3,
	FWBBDBG_HALBB = BIT8,
	FWBBDBG_RUA = BIT16,
	FWBBDBG_ULPWR_CTRL = BIT24,
	FWBBDBG_ULRA = BIT25,
	FWBBDBG_TRIG_TX = BIT26,
	FWBBDBG_EDCCA = BIT27,
};


/**
 * @struct cctrl_info_dword_0
 * @brief cctrl_info_dword_0
 *
 * @var cctrl_info_dword_0::DATARATE
 * Please Place Description here.
 * @var cctrl_info_dword_0::rsvd0
 * Please Place Description here.
 * @var cctrl_info_dword_0::DATA_BW
 * Please Place Description here.
 * @var cctrl_info_dword_0::DATA_GI_LTF
 * Please Place Description here.
 * @var cctrl_info_dword_0::DARF_TC_index
 * Please Place Description here.
 * @var cctrl_info_dword_0::rsvd1
 * Please Place Description here.
 * @var cctrl_info_dword_0::TRYRATE
 * Please Place Description here.
 * @var cctrl_info_dword_0::rsvd2
 * Please Place Description here.
 */
typedef struct cctrl_info_dword_0 {
	//offset:0
	u32 DATARATE: 9;
	u32 DIS_SR: 1;
	u32 DATA_BW: 2;
	u32 GI_LTF: 3;
	u32 DARF_TC_INDEX: 1;
	u32 ARFR_CTRL: 4;
	u32 rsvd0: 1;
	u32 PROTECTION_EN: 1;
	u32 RTS_EN: 1;
	u32 CTS2SELF: 1;
	u32 HW_RTS_EN: 1;
	u32 rsvd1: 6;
	u32 RTY_LOW_RATE_EN: 1;
} CCTRL_INFO_DWORD0, *PCCTRL_INFO_DWORD0;


/**
 * @struct ra_rate_tuple
 * @brief ra_rate_tuple
 *
 * @var ra_rate_tuple::bw_idx
 * Please Place Description here.
 * @var ra_rate_tuple::mode
 * Please Place Description here.
 * @var ra_rate_tuple::gi_ltf
 * Please Place Description here.
 * @var ra_rate_tuple::rsvd0
 * Please Place Description here.
 * @var ra_rate_tuple::rate_idx
 * Please Place Description here.
 */
typedef struct ra_rate_tuple {
	u8 bw_idx: 2;
	u8 mode: 2;
	u8 gi_ltf: 3;
	u8 rsvd0: 1;
	u8 rate_idx;
} RA_RATE_T, *PRA_RATE_T;


/**
 * @struct _ra_mask_he
 * @brief _ra_mask_he
 *
 * @var _ra_mask_he::cck
 * Please Place Description here.
 * @var _ra_mask_he::legacy_ofdm
 * Please Place Description here.
 * @var _ra_mask_he::he_1ss
 * Please Place Description here.
 * @var _ra_mask_he::he_2ss
 * Please Place Description here.
 * @var _ra_mask_he::he_3ss
 * Please Place Description here.
 * @var _ra_mask_he::he_4ss
 * Please Place Description here.
 * @var _ra_mask_he::rsvd
 * Please Place Description here.
 * @var _ra_mask_he::mode_ctrl
 * Please Place Description here.
 */
typedef struct _ra_mask_he {
	u64 cck: 4;
	u64 legacy_ofdm: 8;
	u64 he_1ss: 12;
	u64 he_2ss: 12;
	u64 he_3ss: 12;
	u64 he_4ss: 12;
	u64 rsvd: 2;
	u64 mode_ctrl: 2;
} RA_MASK_HE, *PRA_MASK_HE;


/**
 * @struct _ra_mask_vht
 * @brief _ra_mask_vht
 *
 * @var _ra_mask_vht::cck
 * Please Place Description here.
 * @var _ra_mask_vht::legacy_ofdm
 * Please Place Description here.
 * @var _ra_mask_vht::vht_1ss
 * Please Place Description here.
 * @var _ra_mask_vht::rsvd0
 * Please Place Description here.
 * @var _ra_mask_vht::vht_2ss
 * Please Place Description here.
 * @var _ra_mask_vht::rsvd1
 * Please Place Description here.
 * @var _ra_mask_vht::vht_3ss
 * Please Place Description here.
 * @var _ra_mask_vht::rsvd2
 * Please Place Description here.
 * @var _ra_mask_vht::vht_4ss
 * Please Place Description here.
 * @var _ra_mask_vht::rsvd3
 * Please Place Description here.
 * @var _ra_mask_vht::mode_ctrl
 * Please Place Description here.
 */
typedef struct _ra_mask_vht {
	u64 cck: 4;
	u64 legacy_ofdm: 8;
	u64 vht_1ss: 10;
	u64 rsvd0: 2;
	u64 vht_2ss: 10;
	u64 rsvd1: 2;
	u64 vht_3ss: 10;
	u64 rsvd2: 2;
	u64 vht_4ss: 10;
	u64 rsvd3: 4;
	u64 mode_ctrl: 2;
} RA_MASK_VHT, *PRA_MASK_VHT;


/**
 * @struct _ra_mask_ht
 * @brief _ra_mask_ht
 *
 * @var _ra_mask_ht::cck
 * Please Place Description here.
 * @var _ra_mask_ht::legacy_ofdm
 * Please Place Description here.
 * @var _ra_mask_ht::ht_1
 * Please Place Description here.
 * @var _ra_mask_ht::rsvd0
 * Please Place Description here.
 * @var _ra_mask_ht::ht_2
 * Please Place Description here.
 * @var _ra_mask_ht::rsvd1
 * Please Place Description here.
 * @var _ra_mask_ht::ht_3
 * Please Place Description here.
 * @var _ra_mask_ht::rsvd2
 * Please Place Description here.
 * @var _ra_mask_ht::ht_4
 * Please Place Description here.
 * @var _ra_mask_ht::rsvd3
 * Please Place Description here.
 * @var _ra_mask_ht::mode_ctrl
 * Please Place Description here.
 */
typedef struct _ra_mask_ht {
	u64 cck: 4;
	u64 legacy_ofdm: 8;
	u64 ht_1: 8;
	u64 rsvd0: 4;
	u64 ht_2: 8;
	u64 rsvd1: 4;
	u64 ht_3: 8;
	u64 rsvd2: 4;
	u64 ht_4: 8;
	u64 rsvd3: 6;
	u64 mode_ctrl: 2;
} RA_MASK_HT, *PRA_MASK_HT;


/**
 * @struct _ra_c2h_rpt
 * @brief _ra_c2h_rpt
 *
 * @var _ra_c2h_rpt::rpt_macid
 * Please Place Description here.
 * @var _ra_c2h_rpt::retry_ratio
 * Please Place Description here.
 * @var _ra_c2h_rpt::rsvd0
 * Please Place Description here.
 * @var _ra_c2h_rpt::rpt_mcs_nss
 * Please Place Description here.
 * @var _ra_c2h_rpt::rsvd1
 * Please Place Description here.
 * @var _ra_c2h_rpt::rpt_md_sel
 * Please Place Description here.
 * @var _ra_c2h_rpt::rpt_gi_ltf
 * Please Place Description here.
 * @var _ra_c2h_rpt::rpt_bw
 * Please Place Description here.
 * @var _ra_c2h_rpt::rsvd2
 * Please Place Description here.
 * @var _ra_c2h_rpt::rsvd3
 * Please Place Description here.
 * @var _ra_c2h_rpt::rsvd4
 * Please Place Description here.
 */
struct _ra_c2h_rpt {
	u16 rpt_macid;
	u8 retry_ratio;
	u8 rsvd0;

	u8 rpt_rate: 7;
	u8 rsvd1: 1;

	u8 rpt_md_sel: 2;
	u8 rpt_gi_ltf: 3;
	u8 rpt_bw: 2;
	u8 rsvd2: 1;
	u8 rsvd3;
	u8 rsvd4;
};

typedef struct TPBASE_RA_PARM {
	u8 high_cadidante_rate;
	u8 low_cadidante_rate;
	u8 badpcr_trystate_bitmap;//BIT0:low rate BIT1:high rate
	u8 goodpcr_trystate_bitmap;//BIT0:high rate1 BIT1:high rate2
	u8 base_rate;
	u8 base_gi_ltf;
	u32 BaseTP;
	u32 CurrentTP;
	u8  try_down_gi_ltf[3];
	u8  try_up_gi_ltf[2];
	u8 trydown_rateidxtable[3];
	u8 tryup_rateidxtable[2];
	u32 trydown_rateidxTPtable[3];
	u32 tryup_rateidxTPtable[2];
	u8 fewpacket_trydec_cnt;
	u8 fewpacket_cnt;
	u8 rateupkeepgiltf;
	u8 rateupkeeprate;
	u8 rateupkeepcnt;
	u8 rateuplock;
} TPBASE_RA_PARM, *PTPBASE_RA_PARM;


/*--------------------Define MACRO--------------------------------------*/


/**
 * @brief RATimerCallback
 *
 * @param xTimer
 * @return Please Place Description here.
 * @retval extern
 */

extern void
RateDecision(
	void
);




/**
 * @brief write_rate_tab
 *
 * @param macid
 * @param prate_t
 * @param tryrate
 * @param darfc_index
 * @return Please Place Description here.
 * @retval extern
 */
extern void
write_rate_tab(
	u8 macid,
	PRA_RATE_T prate_t,
	u8 tryrate,
	u8 darfc_index
);

/**
 * @brief read_ctrl_info_rate
 *
 * @param macid
 * @return Please Place Description here.
 * @retval extern
 */

extern RA_RATE_T
read_ctrl_info_rate(
	u8 macid
);

/**
 * @brief fw_fixed_ctrl_info_rate
 *
 * @param macid
 * @param fixed_gi_ltf
 * @param fixed_bw
 * @param fixed_mode
 * @param fixed_rate
 * @return Please Place Description here.
 * @retval extern
 */

extern void
fw_fixed_ctrl_info_rate(
	u8 macid,
	u8 fixed_gi_ltf,
	u8 fixed_bw,
	u8 fixed_mode,
	u8 fixed_rate
);

/**
 * @brief Halbb_start_rate
 *
 * @param macid
 * @param start_rssi
 * @return Please Place Description here.
 * @retval extern
 */

// ---- Main functions ----
extern void
Halbb_start_rate(
	u8 macid,
	u8 start_rssi
);
#if 0
/**
 * @brief init_ra_rty_c2h
 *
 * @param void
 * @return Please Place Description here.
 * @retval extern
 */

extern void
init_ra_rty_c2h(
	void
);
#endif
/**
 * @brief init_ra_rpt
 *
 * @return Please Place Description here.
 * @retval extern
 */

extern void
init_ra_rpt(
	u8 macid
);

/**
 * @brief Halbb_arfrrefresh
 *
 * @param macid
 * @param mask0
 * @param mask1
 * @return Please Place Description here.
 * @retval extern
 */

//---------------------------------------------------------------------
// ARFR
//---------------------------------------------------------------------
extern void
Halbb_arfrrefresh(
	u8 macid,
	u32 mask0
);


/**
 * @brief get_tx_statistics
 *
 * @param *reportContent
 * @return Please Place Description here.
 * @retval extern
 */

extern void
get_tx_statistics(
	u8 rpt_idx
);

//---------------------------------------------------------------------
// Others
//---------------------------------------------------------------------

extern void
rate_idx_to_mcs_ss(
	u8 mode,
	u8 rate_idx,
	u8 *mcs,
	u8 *ss //Nss minus one
);

extern void
manual_adjust_ra_mask(
	u8 rate_idx,
	u8 mode,
	u8 macid
);

extern u8
mcs_ss_to_rate_idx(
	u8 mode,
	u8 mcs,
	u8 ss //Nss minus one
);

//---------------------------------------------------------------------
// Helper functions end
//---------------------------------------------------------------------
/**
 * @brief reset_ra_rpt_c2h
 *
 * @param macid
 * @return Please Place Description here.
 * @retval extern
 */

extern void
reset_ra_rpt_c2h(
	u8 macid
);

/**
 * @brief reset_ra_rpt_try
 *
 * @param macid
 * @return Please Place Description here.
 * @retval extern
 */

extern void
reset_ra_rpt_normal(
	u8 macid
);

/**
 * @brief reset_ra_rpt_try
 *
 * @param macid
 * @return Please Place Description here.
 * @retval extern
 */

extern void
reset_ra_rpt_try(
	u8 macid
);

extern void
InitRAInfo(
	void
);


#endif
#endif
