
#ifndef __WIFIFW_HALBB_CMD_RAM_H__
#define __WIFIFW_HALBB_CMD_RAM_H__

/*--------------------Define -------------------------------------------*/
#define STA_NUM_RSSI_CMD    8
#define SIZE_RSSI_CMD  8
#define H2C_CLASS_HALBB_MAX 7
#define C2H_CAT_HALBB   0x2
#define STA_NUM_CQI_CMD 8
#define SIZE_CQI_CMD  24
#define STA_NUM_SWGRP_CMD 8
#define SIZE_SWGRP_CMD  12
#define STA_NUM_ULMACIDCFG_CMD 8
#define SIZE_ULMACIDCFG_CMD  8
#define UPDATE_DL_CQI 0
#define UPDATE_UL_CQI 1
#define CSI_CONTENT_START 12

/*TestMode*/
//#define TEST_MACID  0x0
#define TEST_BW_NUUPDATE    0x0
#define TEST_DISABLERA  0x0
#define TEST_MODE_SEL_BG  BIT(0)|BIT(1)
#define TEST_MODE_SEL_HT  BIT(0)|BIT(1)|BIT(2)
#define TEST_MODE_SEL_VHT  BIT(0)|BIT(1)|BIT(3)
#define TEST_MODE_SEL_HE  BIT(0)|BIT(1)|BIT(4)
#define TEST_MODE_SEL TEST_MODE_SEL_HT
#define TEST_BW_20 0x0
#define TEST_BW_40 0x1
#define TEST_BW TEST_BW_20
#define TEST_SS_NUM 0x0 /*1SS*/
#define TEST_GI_LTF 0x0
#define TEST_INIT_LV    0x0
#define TEST_SGI_EN 0


#define CLASSID_RUA 0x0
#define CLASSID_RA 0x1
#define CLASSID_HALBB 0x2

#define     TBL_DLRU_GRPTBL 0x0
#define     TBL_ULRU_GRPTBL 0x1
#define     TBL_RU_STAINFO 0x2
#define     TBL_DL_RU_FIXTBL 0x3
#define     TBL_UL_RU_FIXTBL 0x4
#define     TBL_BA_INFOTBL 0x5
#define     TBL_MU_FIXINFO 0x6
#define     TBL_UL_FIXINFO 0x7
#define     TBL_TESTMODE 0x8


/*--------------------Define Enum---------------------------------------*/



/**
 * @enum HALBB_BAND_TYPE_
 *
 * @brief HALBB_BAND_TYPE_
 *
 * @var HALBB_BAND_TYPE_::BAND_ON_24G
 * Please Place Description here.
 * @var HALBB_BAND_TYPE_::BAND_ON_5G
 * Please Place Description here.
 * @var HALBB_BAND_TYPE_::BAND_ON_6G
 * Please Place Description here.
 * @var HALBB_BAND_TYPE_::BAND_MAX
 * Please Place Description here.
 */
enum HALBB_BAND_TYPE_ {
	BAND_ON_24G = 0,
	BAND_ON_5G  = 1,
	BAND_ON_6G  = 2,
	BAND_MAX,
};


/**
 * @enum HALBB_EDCCA_MODE_
 *
 * @brief HALBB_EDCCA_MODE_
 *
 * @var HALBB_EDCCA_MODE_::EDCCA_NORMAL_MODE
 * Please Place Description here.
 * @var HALBB_EDCCA_MODE_::EDCCA_ADAPT_MODE
 * Please Place Description here.
 * @var HALBB_EDCCA_MODE_::EDCCA_CARRIER_SENSE_MODE
 * Please Place Description here.
 */
enum HALBB_EDCCA_MODE_ {
	EDCCA_NORMAL_MODE = 0,
	EDCCA_ADAPT_MODE = 1,
	EDCCA_CARRIER_SENSE_MODE = 2,
};



/**
 * @enum HALBB_C2H_RA_CMDID
 *
 * @brief HALBB_C2H_RA_CMDID
 *
 * @var HALBB_C2H_RA_CMDID::C2HRA_STS_RPT
 * Please Place Description here.
 * @var HALBB_C2H_RA_CMDID::C2HRA_MU_GPTBL_RPT
 * Please Place Description here.
 * @var HALBB_C2H_RA_CMDID::C2HRA_TXSTS
 * Please Place Description here.
 * @var HALBB_C2H_RA_CMDID::MAX_C2HRACMD
 * Please Place Description here.
 */
enum HALBB_C2H_RA_CMDID {
	C2HRA_STS_RPT       = 0x16,
	//C2HRA_MU_GPTBL_RPT      = 0x1,
	C2HRA_TXSTS             = 0x17,
	MAX_C2HRACMD
};

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct-------------------------------------*/


/**
 * @struct halbb_c2h_hdr
 * @brief halbb_c2h_hdr
 *
 * @var halbb_c2h_hdr::func
 * Please Place Description here.
 * @var halbb_c2h_hdr::cat
 * Please Place Description here.
 * @var halbb_c2h_hdr::total_len
 * Please Place Description here.
 */
struct halbb_c2h_hdr {
	u8 func;
	u8 class;
	u8 cat;
	u16 total_len;
};


/**
 * @struct _h2c_fwtrace_
 * @brief _h2c_fwtrace_
 *
 * @var _h2c_fwtrace_::halbb_dbg_comp
 * Please Place Description here.
 * @var _h2c_fwtrace_::halbb_fwtrace
 * Please Place Description here.
 */
struct _h2c_fwtrace_ {
	u32 halbb_dbg_comp;
	u8 halbb_fwtrace;
};


/**
 * @struct _h2c_fw_edcca_
 * @brief _h2c_fw_edcca_
 *
 * @var _h2c_fw_edcca_::mode
 * Please Place Description here.
 * @var _h2c_fw_edcca_::band
 * Please Place Description here.
 * @var _h2c_fw_edcca_::pwr_th_5g
 * Please Place Description here.
 * @var _h2c_fw_edcca_::pwr_th_2p4
 * Please Place Description here.
 * @var _h2c_fw_edcca_::pwr_th_cs
 * Please Place Description here.
 */
struct _h2c_fw_edcca_ {
	u8 mode;
	u8 band;
	u8 pwr_th_5g;
	u8 pwr_th_2p4;
	u8 pwr_th_cs;
};


/**
 * @struct _c2h_rpt_txsts_
 * @brief _c2h_rpt_txsts_
 *
 * @var _c2h_rpt_txsts_::macid
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::avg_agg
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::rsvd
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::tx_ok
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::tx_retry
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::tx_rate
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::retry_ratio
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::rsvd2
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::tx_total
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::rsvd3
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::rsvd4
 * Please Place Description here.
 * @var _c2h_rpt_txsts_::rsvd5
 * Please Place Description here.
 */
struct _c2h_rpt_txsts_ {
	u8 macid;
	u8 avg_agg;
	u16 rsvd;

	u16 tx_ok[4];

	u16 tx_retry[4];

	u16 tx_rate;
	u8 retry_ratio;
	u8 rsvd2;

	u16 tx_total;
	u16 rsvd3;

	u16 rsvd4;
	u16 rsvd5;
};


/**
 * @struct _H2C_CMD_RSSI_CONTENT_
 * @brief _H2C_CMD_RSSI_CONTENT_
 *
 * @var _H2C_CMD_RSSI_CONTENT_::macid
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::rssi
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::rainfo
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::drv_ra_ctrl
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::is_fixed_rate
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::fixed_rate
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::fixed_gi_ltf
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::fixed_bw
 * Please Place Description here.
 * @var _H2C_CMD_RSSI_CONTENT_::rsvd4
 * Please Place Description here.
 */
typedef struct _H2C_CMD_RSSI_CONTENT_ {
	// DWORD 0
	u32 macid: 8;
	u32 rssi: 8;
	u32 rainfo: 16;

	//DWORD 1
	u32 drv_ra_ctrl: 8;
	u32 is_fixed_rate: 1;
	u32 fixed_rate: 9;
	u32 fixed_gi_ltf: 3;
	u32 fixed_bw: 2;
	u32 rsvd4: 9;

} H2C_CMD_RSSI_CONTENT, *PH2C_CMD_RSSI_CONTENT;

#if 0
extern void
halbb_fw_l6m_wa(
	u8 *content
);
#endif



extern void Halbb_h2chdl(
	u8  CmdID,
	u8  *content
);


extern void
Halbb_c2hhdl(
	u8  cmdid,
	u16 len,
	u8  *content);

#endif //HALBBCOMM_CMD_RAM_H__
