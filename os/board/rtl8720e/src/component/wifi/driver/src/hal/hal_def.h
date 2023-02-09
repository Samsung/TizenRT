#ifndef __HAL_DEF_H__
#define __HAL_DEF_H__

/*
* structs defined in this file are shared with halbb or halrf
*/
/*==================hal_general_def.h of g6 driver==============*/
#define FL_COMP_VER BIT0
#define FL_COMP_INIT BIT1
#define FL_COMP_TASK BIT2
#define FL_COMP_CNS BIT3
#define FL_COMP_H2C BIT4
#define FL_COMP_C2H BIT5
#define FL_COMP_TX BIT6
#define FL_COMP_RX BIT7
#define FL_COMP_IPSEC BIT8
#define FL_COMP_TIMER BIT9
#define FL_COMP_DBGPKT BIT10
#define FL_COMP_PS BIT11
#define FL_COMP_BB BIT16
#define FL_COMP_MCC BIT20

/*control of pwr by limit and power by rate*/
enum hal_pwr_by_rate_setting {
	PW_BY_RATE_ALL_SAME = 0,
	PW_BY_RATE_ON = 1
};

enum hal_pwr_limit_type {
	PWLMT_DISABLE = 0,
	PWBYRATE_AND_PWLMT = 1,
	PWLMT_BY_EFUSE = 2
};

enum rtw_chip_id {
	CHIP_WIFI6_8852A,
	CHIP_WIFI6_8834A,
	CHIP_WIFI6_8852B,
	CHIP_WIFI6_8852C,
	CHIP_WIFI6_8192XB,
	CHIP_WIFI6_8832BR,
	CHIP_WIFI6_8852BP,
	CHIP_WIFI6_8730E,
	CHIP_WIFI6_8720E,
	CHIP_WIFI6_MAX
};

enum rtw_cv {/*cut version*/
	CAV,
	CBV,
	CCV,
	CDV,
	CEV,
	CFV,
	CGV,
	CTV,
	CMAXV,
};

/*==================hal_def.h of g6 driver===================*/

struct rtw_trx_stat {
	u16 rx_rate_plurality;
};

enum rtw_hal_status {
	RTW_HAL_STATUS_SUCCESS, /* 0 */
	RTW_HAL_STATUS_FAILURE, /* 1 */
	RTW_HAL_STATUS_RESOURCE, /* 2 */
	RTW_HAL_STATUS_IO_INIT_FAILURE, /* 3 */
	RTW_HAL_STATUS_MAC_INIT_FAILURE, /* 4 */
	RTW_HAL_STATUS_BB_INIT_FAILURE, /* 5 */
	RTW_HAL_STATUS_RF_INIT_FAILURE, /* 6 */
	RTW_HAL_STATUS_BTC_INIT_FAILURE, /* 7 */
	RTW_HAL_STATUS_HAL_INIT_FAILURE, /* 8 */
	RTW_HAL_STATUS_EFUSE_UNINIT, /* 9 */
	RTW_HAL_STATUS_EFUSE_IVALID_OFFSET, /* 10 */
	RTW_HAL_STATUS_EFUSE_PG_FAIL, /* 11 */
	RTW_HAL_STATUS_MAC_API_FAILURE, /* 12 */
	RTW_HAL_STATUS_BB_CH_INFO_LAST_SEG, /*13*/
	RTW_HAL_STATUS_UNKNOWN_RFE_TYPE, /* 14 */
	RTW_HAL_STATUS_TIMEOUT, /* 15 */
};

struct rtw_rssi_info {
	u8 rssi; /* u(8,1), hal-bb provide, read only : 0~110 (dBm = rssi -110) */
	u16 rssi_ma; /* u(16,5),  hal-bb provide, read only : u16 U(12,4)*/
	u16 rssi_ma_path[4];
	u16 pkt_cnt_data;
	u8 rssi_bcn; /* u(8,1), beacon RSSI, hal-bb provide, read only : 0~110 (dBm = rssi -110) */
	u16 rssi_bcn_ma; /* u(16,5),  beacon RSSI, hal-bb provide, read only*/
	u16 rssi_bcn_ma_path[4];
	u16 pkt_cnt_bcn;
	u8 ma_factor: 4;
	u8 ma_factor_bcn: 4;
	u8 rssi_ofdm; /* u(8,1),  hal-bb provide, read only : packet, for debug */
	u8 rssi_cck; /* u(8,1),  hal-bb provide, read only : packet, for debug */
	u8 assoc_rssi; /* phl_rx provide, read only */
	/* phl_rx provide, read only : Moving Average RSSI information for the STA */
	u8 ma_rssi; /* moving average : 0 ~ PHL_MAX_RSSI (dBm = rssi - PHL_MAX_RSSI) */
	u8 ma_rssi_mgnt; /* moving average rssi for beacon/probe : 0 ~ PHL_MAX_RSSI (dBm = rssi - PHL_MAX_RSSI) */
	u16 snr_ma; /* u(16,4), hal-bb provide, read only, SNR= snr_ma dBm*/
};
enum hal_rate_mode {
	HAL_LEGACY_MODE	= 0,
	HAL_HT_MODE	= 1,
	HAL_VHT_MODE	= 2,
	HAL_HE_MODE	= 3
};

enum hal_rate_bw {
	HAL_RATE_BW_20	= 0,
	HAL_RATE_BW_40	= 1,
	HAL_RATE_BW_80	= 2,
	HAL_RATE_BW_160	= 3,
};

struct rtw_rate_info {
	enum rtw_gi_ltf gi_ltf; /* 3bit GILTF */
	enum hal_rate_mode mode; /* 2bit 0:legacy, 1:HT, 2:VHT, 3:HE*/
	enum hal_rate_bw bw; /*2bit 0:5M/10M/20M, 1:40M, 2:80M, 3:160M or 80+80*/
	u8 mcs_ss_idx; /*HE: 3bit SS + 4bit MCS; non-HE: 5bit MCS/rate idx */
};

enum rtw_h2c_pkt_type { //AX_TODO:need fw further check
	H2CB_TYPE_CMD = 0,
	H2CB_TYPE_DATA = 1,
	H2CB_TYPE_LONG_DATA = 2,
	H2CB_TYPE_MAX = 0x3
};

struct rtw_g6_h2c_hdr { //AX_TODO:need fw further check
	u8 h2c_class; //0x0~0x7: Phydm; 0x8~0xF: RF; 0x10~0x17: BTC
	u8 h2c_func;
	u8 seq_valid: 1;
	u8 seq: 3;
	u8 seq_stop: 1;
	enum rtw_h2c_pkt_type type; //0:cmd ; 1:cmd+data ; 2:cmd+long data
	u16 content_len: 12;
	u16 rec_ack: 1; //Ack when receive H2C
	u16 done_ack: 1; //Ack when FW execute H2C cmd done
	u16 rsvd2: 2;
};

enum tx_pause_rson {
	PAUSE_RSON_NOR_SCAN, /*normal scan*/
	PAUSE_RSON_UNSPEC_BY_MACID, /*P2P_SCAN*/
	PAUSE_RSON_RFK,
	PAUSE_RSON_PSD,
	PAUSE_RSON_DFS,
	PAUSE_RSON_DFS_CSA,
	PAUSE_RSON_DBCC,
	PAUSE_RSON_RESET,
	PAUSE_RSON_MAX
};

/*HW_BAND0 - CMAC0 + PHY0 + S0*/
/*HW_BAND1 - CMAC1 + PHY1 + S1*/
enum phl_band_idx {
	HW_BAND_0,
	HW_BAND_1,
	HW_BAND_MAX
};

enum phl_phy_idx {
	HW_PHY_0,
	HW_PHY_1,
	HW_PHY_MAX
};

/* from cmn_sta_info */
struct rtw_ra_sta_info {
	/*u8 rate_id;			remove !!! use wmode in phl, [PHYDM] ratr_idx*/
	/*u8 rssi_level;			[PHYDM]*/
	/*u8 is_first_connect:1;		change connect flow, [PHYDM] CE: ra_rpt_linked, AP: H2C_rssi_rpt*/
	/*u8 is_support_sgi:1;		mov to phl [driver]*/
	/*u8 is_vht_enable:2;		mov to phl [driver]*/
	/*u8 disable_ra:1;			mov to hal [driver]*/
	/*u8 disable_pt:1;			remove for no PT, [driver] remove is_disable_power_training*/
	/*u8 txrx_state:2;			///////////////need to check if needed, [PHYDM] 0: Tx, 1:Rx, 2:bi-direction*/
	/*u8 is_noisy:1;			///////////////need to check if needed, [PHYDM]*/
	u16 curr_tx_rate;			/*use struct bb_rate_info, [PHYDM] FW->Driver*/
	enum channel_width ra_bw_mode;	/*remove to phl, [Driver] max bandwidth, for RA only*/
	enum channel_width curr_tx_bw;	/*bb_rate_info, [PHYDM] FW->Driver*/
	/* u8 drv_ractrl; */

	/* Ctrl */
	bool dis_ra; /*move from rtw_hal_stainfo_t*/
	bool ra_registered;/*move from rtw_hal_stainfo_t*/
	u64 ra_mask;/*move from rtw_hal_stainfo_t*/ /*drv decide by specific req*/
	u64 cur_ra_mask;/*move from rtw_hal_stainfo_t*/

	/*halbb create, mod by driver and decide by rssi or other*/
	u8 cal_giltf; /* 3bit gi_ltf ctrl by driver*/			// only halbb_ra used
	bool fix_giltf_en; /*giltf from cal_giltf or halbb*/	// only halbb_ra used
	bool fixed_rt_en;								// only halbb_ra used
	struct rtw_rate_info fixed_rt_i;					// only halbb_ra used

	/* Report */
	struct rtw_rate_info rpt_rt_i;						// halbb support, core get raid from this report
	u8 curr_retry_ratio;		/*[HALBB] FW->Driver*/	//halbb support

	bool ra_csi_rate_en;								// halbb beamforming set
	bool fixed_csi_rate_en;							// halbb beamforming set
	u8 band_num;									// halbb beamforming set
	struct rtw_rate_info csi_rate;						// halbb beamforming set

	u8 avg_agg;				//no use
	u32 tx_ok_cnt[4];			//halbb support
	u32 tx_retry_cnt[4];		//halbb support
	u32 tx_total_cnt;			//halbb support
	/*u64 ramask;*/
};

enum rtw_tpu_op_mode {
	TPU_NORMAL_MODE		= 0,
	TPU_DBG_MODE		= 1
};

enum phl_pwr_table {
	PWR_BY_RATE	= BIT0,
	PWR_LIMIT	= BIT1,
	PWR_LIMIT_RU	= BIT2
};

#define HAL_MAX_PATH	1

/*[TX Power Unit(TPU) array size]*/
#define TPU_SIZE_PWR_TAB	16 /*MCS0~MCS11(12) + {dcm_0,1,3,4}4 = 16*/
#define TPU_SIZE_PWR_TAB_lGCY	12 /*cck(4) + ofdm(8) = 12*/
#define TPU_SIZE_MODE		5  /*0~4: HE, VHT, HT, Legacy, CCK, */
#define TPU_SIZE_BW		5 /*0~4: 80_80, 160, 80, 40, 20*/
#define TPU_SIZE_RUA		3 /*{26, 52, 106}*/
#define TPU_SIZE_BW20_SC	8 /*8 * 20M = 160M*/
#define TPU_SIZE_BW40_SC	4 /*4 * 40M = 160M*/
#define TPU_SIZE_BW80_SC	2 /*2 * 80M = 160M*/
#define TPU_SIZE_BF		2 /*{NON_BF, BF}*/

struct rtw_tpu_pwr_by_rate_info { /*TX Power Unit (TPU)*/
	s8 pwr_by_rate_lgcy[TPU_SIZE_PWR_TAB_lGCY];
	s8 pwr_by_rate[HAL_MAX_PATH][TPU_SIZE_PWR_TAB];
};

struct rtw_tpu_pwr_imt_info { /*TX Power Unit (TPU)*/
	s8 pwr_lmt_cck_20m[HAL_MAX_PATH][TPU_SIZE_BF];
	s8 pwr_lmt_cck_40m[HAL_MAX_PATH][TPU_SIZE_BF];
	s8 pwr_lmt_lgcy_20m[HAL_MAX_PATH][TPU_SIZE_BF]; /*ofdm*/
	s8 pwr_lmt_20m[HAL_MAX_PATH][TPU_SIZE_BW20_SC][TPU_SIZE_BF];
	s8 pwr_lmt_40m[HAL_MAX_PATH][TPU_SIZE_BW40_SC][TPU_SIZE_BF];
	s8 pwr_lmt_80m[HAL_MAX_PATH][TPU_SIZE_BW80_SC][TPU_SIZE_BF];
	s8 pwr_lmt_160m[HAL_MAX_PATH][TPU_SIZE_BF];
	s8 pwr_lmt_40m_0p5[HAL_MAX_PATH][TPU_SIZE_BF];
	s8 pwr_lmt_40m_2p5[HAL_MAX_PATH][TPU_SIZE_BF];
};

struct rtw_tpu_info { /*TX Power Unit (TPU)*/
	enum rtw_tpu_op_mode op_mode; /*In debug mode, only debug tool control TPU APIs*/
	bool normal_mode_lock_en;
	s8 ofst_int; /*SW: S(8,3) -16 ~ +15.875 (dB)*/
	u8 ofst_fraction; /*[0:3] * 0.125(dBm)*/
	u8 base_cw_0db; /*[63~39~15]: [+24~0~-24 dBm]*/
	u16 tssi_16dBm_cw;
	/*[Ref Pwr]*/
	s16 ref_pow_ofdm; /*-> HW: s(9,2)*/
	s16 ref_pow_cck; /*-> HW: s(9,2)*/
	u16 ref_pow_ofdm_cw; /*BBCR 0x58E0[9:0]*/
	u16 ref_pow_cck_cw; /*BBCR 0x58E0[21:12]*/
	/*[Pwr Ofsset]*/ /*-> HW: s(7,1)*/
	s8 pwr_ofst_mode[TPU_SIZE_MODE]; /*0~4: HE, VHT, HT, Legacy, CCK, */
	s8 pwr_ofst_bw[TPU_SIZE_BW]; /*0~4: 80_80, 160, 80, 40, 20*/
	/*[Pwr By rate]*/ /*-> HW: s(7,1)*/
	struct rtw_tpu_pwr_by_rate_info rtw_tpu_pwr_by_rate_i;
	/*[Pwr Limit]*/ /*-> HW: s(7,1)*/
	struct rtw_tpu_pwr_imt_info rtw_tpu_pwr_imt_i;
	/*[Pwr Limit RUA]*/ /*-> HW: s(7,1)*/
	s8 pwr_lmt_ru[HAL_MAX_PATH][TPU_SIZE_RUA][TPU_SIZE_BW20_SC];
	u16 pwr_lmt_ru_mem_size;
	bool pwr_lmt_en;
	u8 tx_ptrn_shap_idx;
	u8 tx_ptrn_shap_idx_cck;
};

struct rtw_hal_stat_info {
	u32 cnt_fail_all;
	u32 cnt_cck_fail;
	u32 cnt_ofdm_fail;
	u32 cnt_cca_all;
	u32 cnt_ofdm_cca;
	u32 cnt_cck_cca;
	u32 cnt_crc32_error_all;
	u32 cnt_he_crc32_error;
	u32 cnt_vht_crc32_error;
	u32 cnt_ht_crc32_error ;
	u32 cnt_ofdm_crc32_error;
	u32 cnt_cck_crc32_error;
	u32 cnt_crc32_ok_all;
	u32 cnt_he_crc32_ok;
	u32 cnt_vht_crc32_ok;
	u32 cnt_ht_crc32_ok;
	u32 cnt_ofdm_crc32_ok;
	u32 cnt_cck_crc32_ok;
	u32 igi_fa_rssi;
	u32 cck_mac_txen;
	u32 cck_phy_txon;
	u32 ofdm_mac_txen;
	u32 ofdm_phy_txon;
};

struct rtw_hw_band {
	struct rtw_chan_def cur_chandef;
#if (PHYDM_VERSION == 3)
	struct rtw_tpu_info rtw_tpu_i; /*TX Power Unit (TPU)*/
	struct rtw_hal_stat_info stat_info;
#endif
};

enum phl_rf_mode {
	RF_MODE_NORMAL		= 0,
	RF_MODE_SHUTDOWN	= 1,
	RF_MODE_STANDBY		= 2,
	RF_MODE_RX		= 3,
	RF_MODE_TX		= 4,
	RF_MODE_MAX
};

struct rtw_cfo_info {
	s32		cfo_tail;
	s32		pre_cfo_avg;
	s32		cfo_avg;
	u16		cfo_cnt;
	u32		tp;
};

#define FL_CFG_OP_SET 0
#define FL_CFG_OP_CLR 1
#define FL_CFG_OP_INFO 2

#define FL_CFG_TYPE_LEVEL 0
#define FL_CFG_TYPE_OUTPUT 1
#define FL_CFG_TYPE_COMP 2
#define FL_CFG_TYPE_COMP_EXT 3

#define FL_LV_OFF 0
#define FL_LV_CRT 1
#define FL_LV_SER 2
#define FL_LV_WARN 3
#define FL_LV_LOUD 4
#define FL_LV_TR 5

#define FL_OP_UART BIT0
#define FL_OP_C2H BIT1
#define FL_OP_SNI BIT2

struct hal_dfs_rpt {
	u8 *dfs_ptr;
	u16 dfs_num;
	u8 phy_idx; /*phy0,phy1*/
};

struct rtw_hal_mac_ax_cctl_info {
	/* dword 0 */
	u32 datarate: 9;
	u32 force_txop: 1;
	u32 data_bw: 2;
	u32 data_gi_ltf: 3;
	u32 darf_tc_index: 1;
	u32 arfr_ctrl: 4;
	u32 acq_rpt_en: 1;
	u32 mgq_rpt_en: 1;
	u32 ulq_rpt_en: 1;
	u32 twtq_rpt_en: 1;
	u32 rsvd0: 1;
	u32 disrtsfb: 1;
	u32 disdatafb: 1;
	u32 tryrate: 1;
	u32 ampdu_density: 4;
	/* dword 1 */
	u32 data_rty_lowest_rate: 9;
	u32 ampdu_time_sel: 1;
	u32 ampdu_len_sel: 1;
	u32 rts_txcnt_lmt_sel: 1;
	u32 rts_txcnt_lmt: 4;
	u32 rtsrate: 9;
	u32 rsvd1: 2;
	u32 vcs_stbc: 1;
	u32 rts_rty_lowest_rate: 4;
	/* dword 2 */
	u32 data_tx_cnt_lmt: 6;
	u32 data_txcnt_lmt_sel: 1;
	u32 max_agg_num_sel: 1;
	u32 rts_en: 1;
	u32 cts2self_en: 1;
	u32 cca_rts: 2;
	u32 hw_rts_en: 1;
	u32 rts_drop_data_mode: 2;
	u32 preld_en: 1;
	u32 ampdu_max_len: 11;
	u32 ul_mu_dis: 1;
	u32 ampdu_max_time: 4;
	/* dword 3 */
	u32 max_agg_num: 9;
	u32 ba_bmap: 2;
	u32 rsvd3: 5;
	u32 vo_lftime_sel: 3;
	u32 vi_lftime_sel: 3;
	u32 be_lftime_sel: 3;
	u32 bk_lftime_sel: 3;
	u32 sectype: 4;
	/* dword 4 */
	u32 multi_port_id: 3;
	u32 bmc: 1;
	u32 mbssid: 4;
	u32 navusehdr: 1;
	u32 txpwr_mode: 3;
	u32 data_dcm: 1;
	u32 data_er: 1;
	u32 data_ldpc: 1;
	u32 data_stbc: 1;
	u32 a_ctrl_bqr: 1;
	u32 a_ctrl_uph: 1;
	u32 a_ctrl_bsr: 1;
	u32 a_ctrl_cas: 1;
	u32 data_bw_er: 1;
	u32 lsig_txop_en: 1;
	u32 rsvd4: 5;
	u32 ctrl_cnt_vld: 1;
	u32 ctrl_cnt: 4;
	/* dword 5 */
	u32 resp_ref_rate: 9;
	u32 rsvd5: 3;
	u32 all_ack_support: 1;
	u32 bsr_queue_size_format: 1;
	u32 bsr_om_upd_en: 1;
	u32 macid_fwd_idc: 1;
	u32 ntx_path_en: 4;
	u32 path_map_a: 2;
	u32 path_map_b: 2;
	u32 path_map_c: 2;
	u32 path_map_d: 2;
	u32 antsel_a: 1;
	u32 antsel_b: 1;
	u32 antsel_c: 1;
	u32 antsel_d: 1;
	/* dword 6 */
	u32 addr_cam_index: 8;
	u32 paid: 9;
	u32 uldl: 1;
	u32 doppler_ctrl: 2;
	u32 nominal_pkt_padding: 2;
	u32 nominal_pkt_padding40: 2;
	u32 txpwr_tolerence: 6;
	/*u32 rsvd9:2;*/
	u32 nominal_pkt_padding80: 2;
	/* dword 7 */
	u32 nc: 3;
	u32 nr: 3;
	u32 ng: 2;
	u32 cb: 2;
	u32 cs: 2;
	u32 csi_txbf_en: 1;
	u32 csi_stbc_en: 1;
	u32 csi_ldpc_en: 1;
	u32 csi_para_en: 1;
	u32 csi_fix_rate: 9;
	u32 csi_gi_ltf: 3;
	u32 nominal_pkt_padding160: 2;
	u32 csi_bw: 2;
};

#define HAL_MAX_MU_STA_NUM 6
struct hal_mu_score_tbl_ctrl {
	u32 mu_sc_thr: 2;
	u32 mu_opt: 1;
	u32 rsvd: 29;
};
#define HAL_MAX_MU_SCORE_SIZE 8 /* Unit: Byte */
struct hal_mu_score_tbl_score {
	u8 valid;
	u16 macid;
	u8 score[HAL_MAX_MU_SCORE_SIZE]; /*by case: [1:0], [3:2], ..... */
};
struct hal_mu_score_tbl {
	struct hal_mu_score_tbl_ctrl mu_ctrl;
	//_os_lock lock;
	u8 num_mu_sta; /*By IC, shall alway <= than HAL_MAX_MU_STA_NUM , 0 = tbl invalid */
	u8 sz_mu_score; /*By IC, shall alway <= than HAL_MAX_MU_SCORE_SIZE , 0 = score invalid */
	struct hal_mu_score_tbl_score mu_score[HAL_MAX_MU_STA_NUM]; /* mu_score[num_mu_sta] */
};

struct rtw_hal_com_t {
	u8 test;
#if (PHYDM_VERSION == 3)
	enum rtw_chip_id chip_id;
	enum rtw_cv cv;/* cut version */
	struct dev_cap_t dev_hw_cap;/* halbb/halrf will set these values */
	void *hal_priv;/*pointer to hal_com_data*/
	void *adapter; /*pointer to adapter*/
	struct rtw_trx_stat trx_stat;/*halbb will set value*/
	u8 assoc_sta_cnt; /*number of associated nodes (sta or ap)*/
	void *drv_priv; /* useless, for halbb/rf_interface compile issue, driver no need take care*/
	bool dbcc_en; /* useless, could be deleted later when halbb code all use DBCC macro*/
	u8 rfpath_rx_num;
	u8 rfpath_tx_num;
#endif
	struct rtw_hw_band band[MAX_BAND_NUM];/*band0/band1 for DBCC*/
	bool csi_para_ctrl_sel; /*AX_TODO: beamformee related*/
	struct hal_mu_score_tbl bb_mu_score_tbl;
};

enum rtw_dv_sel {
	DAV,
	DDV,
};

struct rtw_hal_stainfo_t {
	struct rtw_rssi_info rssi_stat;
	/* from cmn_sta_info */
	struct rtw_ra_sta_info	ra_info;
	void *bb_sta;
	struct rtw_cfo_info cfo_stat;
};
#endif

