#ifndef __WIFIFW_PHYDM_RAM_H__
#define __WIFIFW_PHYDM_RAM_H__
#if CONFIG_RATE_ADAPTIVE

//Ctrl Info access
//#define ODM_ReadCtrlInfo(macid, offset)                      ReadCtrlInfoByte(macid, offset)
//#define ODM_WriteCtrlInfo(macid, offset, val)                WriteCtrlInfoByte(macid, offset, val)

//4  TxCtrlInfo
// offset0
#define TXCTRLINFO_DATA_RATE            (BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
#define TXCTRLINFO_SGI                  (BIT7)
// offset1
#define TXCTRLINFO_PWR_STS              (BIT2|BIT1|BIT0)
#define TXCTRLINFO_DATA_BW              (BIT4|BIT3)
#define TXCTRLINFO_TRY_RATE             (BIT6)

#define TXCTRLINFO_DATA_BW_BITS TXCTRLINFO_DATA_BW
#define TXCTRLINFO_DATA_BW_OFFSET         10

//RA_MASK access
//#define ODM_ReadRAMASKByte(macid, offset)                    ReadRAMASKByte(macid, offset)
//#define ODM_ReadRAMASKDWORD(macid, offset)                   ReadRAMASKDWORD(macid, offset)
//#define ODM_WriteRAMASKByte(macid, offset, val)              WriteRAMASKByte(macid, offset, val)
//#define ODM_WriteRAMASKDWORD(macid, offset, val)             WriteRAMASKDWORD(macid, offset, val)

#define BWSIZE                          3 // BW20M ~ BW80M
#define MULTI_RATE_NUM          2
#define RETRY_RATIO_VAR_SIZE    10
#define LIMIT_RATE_SIZE         2


#define RATESIZE                        108
#define NUM_HE_RATE                     24
#define NUM_VHT_RATE                    40  // 8198F is 4SS
#define NUM_HT_RATE                     32
#define NUM_MULTI_TRY                   2   // 4
#define BW_SWITCH_DISABLE_2SS_ALL       0

//---New Band Switch Mode
#define COMMON_RATE_DOWN_OFFSET 3
#define LIMIT_RATE_SIZE         2
#define GILTF_TRAINING 1

//#define PHYRATE_LGI2SGI(rate) (rate + (rate >> 4) + (rate >> 5) + (rate >> 7)) //1+0.0625+0.03125+0.0078125 ~= 0.102
//#define PHYDM_IS_LEGACY_RATE(rate) ((rate <= RATE_OFDM_54M) ? true : false)

#define RATE_UNUSED                     0xff
#define BW_NO_UPDATE                    0xff
#define GI_NO_UPDATE                    0xff
#define RAMASK_SIZE                     2

#define TRYING_DISABLE   0
#define TRYING_ENABLE    1

#define TRY_SUCCESS      1
#define TRY_FAIL         0

//#define DIFF(A,B) ((A>=B) ? (A-B):(B-A))
#define SHIFT_ROUND(A, SHIFT)  ((A + (1 << (SHIFT-1)))>>SHIFT)
#define MA_ADD(NEW, OLD, SHIFT) (NEW + OLD - (OLD>>SHIFT))
#define MINUS(A,B) ((A > B) ? (A-B) : 0)
#define PHYRATE_LGI2SGI(rate) (rate + (rate >> 4) + (rate >> 5) + (rate >> 7)) //1+0.0625+0.03125+0.0078125 ~= 0.102
// For HE GI_LTF, add 04/32
#define PHYRATE_GI3P2_TO_GI0P8(rate) (rate + (rate>>3) + (rate>>5) + (rate>>6)) // + (rate>>8)) ~= 1.17
#define PHYRATE_GI3P2_TO_GI1P6(rate) (rate + (rate>>4) + (rate>>5) + (rate>>6)) // + (rate>>9)) ~= 1.11
//
#define PHYDM_IS_LEGACY_RATE(rate) ((rate <= RATE_OFDM_54M) ? true : false)
#define PHYDM_IS_CCK_RATE(rate) ((rate <= RATE_CCK_11M) ? true : false)
#define DIFF(a,b)                   ((a>=b)? a-b: b-a)

#define RATE_DEFAULT    0
#define RATE_UP         1
#define RATE_DOWN       2

// packet based RA
#define RATIO_CNT_TH            3
#define RATIO_SHIFT_SGI         1
#define TRY_SUCCESS_RATIO_SHIFT 5
#define RATE_UP_P_OFST          3
#define RATE_UP_CNT             3
#define RATE_UP_FAIL_CNT_LMT    200  // 50
#define RATE_DOWN_CNT           2
#define RATE_DOWN_RATIO_TH      30
#define FORCE_RATE_DOWN_TH      80 //
#define RSSI_TOLERANCE          2
#define MULTI_RATE_NUM          2 /*MULTI_TRY_NUM after first try rate, will try 1+2 times */
#define SMOOTHING_FACTOR        3
#define RETRY_RATIO_VAR_SIZE    10
#define DIFF2VAR_SHIFT          10
#define RETRY_RATIO_VAR_MAX     30

//Eric add for RA fine tune
#define RATE_UP_FAIL_CNT_LMT_1024QAM 5
#define RATE_UP_FAIL_CNT_LMT_256QAM 5
#define RATE_UP_FAIL_CNT_LMT_64QAM 5
#define RATE_UP_FAIL_CNT_LMT_16QAM 5
#define RATE_UP_FAIL_CNT_LMT_QPSK 5
#define RATE_UP_FAIL_CNT_LMT_BPSK 5

#define FAST_DECISION_TRUE  1
#define FAST_DECISION_FALSE 0

//offset 3
#define TXRPTFIELD_DATA_RATE            (BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
#define TXRPTFIELD_SGI                  BIT7

#define DROP_TH             3

#define CLEAN_ENV_VAR_TH    3 // 2
#define GOOD_PER_TH         10
#define DEC_OFFSET_MAX_N    40

//#define NUM_MULTI_TRY                   3

#define RAMSK_HT_VHT_OFFSET     32
#define RAMSK_HT_HE_OFFSET      (RAMSK_HT_VHT_OFFSET+40)

/*-------------------- Define logic and bitwise operation  ---------------*/
typedef struct phydm_rate_adaptive {
	u8 RATE_UP_RTY_RATIO[RATESIZE];
	u8 RATE_DOWN_RTY_RATIO[RATESIZE];
	//u8 first_rate_down; /*ARFB first rate down number ex: Reg0x430=00000001, first_try_rate_down = 4*/
	//u8 first_try_rate_down; /*ARFB first rate down number ex: Reg0x430=01020304, first_try_rate_down = 2*/
	u16 retry_ratio_per_rate_ma[RATESIZE];
	u16 variance_per_rate[RATESIZE];
	u8 DIFF2VAR_TABLE[RETRY_RATIO_VAR_SIZE];
	//u8 rpt_per_macid[3];
} PHYDM_RA_T, *PPHYDM_RA_T;



typedef struct _CCTRL_INFO_ {
	//offset:0
	u16 DATARATE: 9;
	u16 DIS_SR: 1;
	u16 DATA_BW: 2;
	u16 GI_LTF: 3;
	u16 DARF_TC_INDEX: 1;
	u16 ARFR_CTRL: 4;
	u16 rsvd0: 1;
	u16 PROTECTION_EN: 1;
	u16 RTS_EN: 1;
	u16 CTS2SELF: 1;
	u16 HW_RTS_EN: 1;
	u16 rsvd1: 6;
	u16 RTY_LOW_RATE_EN: 1;
	//offset:4
	u16 DATA_RTY_LOWEST_RATE: 9;
	u16 AMPDU_TIME_EN: 1;
	u16 AMPDU_MAX_TIME: 4;
	u16 rsvd2: 2;
	u16 rsvd3: 2;
	u16 LSIG_TXOP_EN: 1;
	u16 FORCE_TXOP: 1;
	u16 TRYRATE: 1;
	u16 FARA_BW_EN: 1;
	u16 PKT_TX_ONE_SEL: 1;
	u16 RTS_RTY_LOWEST_RATE: 9;
	//offset:8
	u32 RTY_CNT_LMT_EN: 1;
	u32 RTY_CNT_LMT: 6;
	u32 MAX_AGG_NUM_EN: 1;
	u32 MAX_AGG_NUM: 5;
	u32 AMPDU_LEN_EN: 1;
	u32 AMPDU_MAX_LEN: 11;
	u32 rsvd4: 7;
	//offset 12
	u32 TXPWR_TOLERANCE: 6;
	u32 DCM: 1;
	u32 EXTEND_RANGE: 1;
	u32 DATA_BW_ER: 1;
	u32 DATA_LDPC: 1;
	u32 DATA_STBC: 1;
	u32 UL_DL: 1;
	u32 DOPPLER_CTL: 2;
	u32 NOMI_PAD20: 2;
	u32 A_CTRL: 4;
	u32 BSR_QUE_SIZE: 1;
	u32 SMOOTH_CTRL: 1;
	u32 rsvd5: 1;
	u32 CTRL_WRAPPER_EN: 1;
	u32 RXID_MATCH_EN: 1;
	u32 BSR_OM_UPD_EN: 1;
	u32 rsvd6: 6;
	//offset 16
	u32 rsvd7: 10;
	u32 PAID: 9;
	u32 NC: 3;
	u32 NR: 3;
	u32 NG: 2;
	u32 CB: 2;
	u32 COEFFOC_SIZE: 2;
	u32 CSI_TXBF_EN: 1;
	//offset 20
	u32 CSI_LDPC_EN: 1;
	u32 CSI_STBC_EN: 1;
	u32 CSI_PARA_EN: 1;
	u32 CSI_FIX_RATE: 9;
	u32 CSI_BW: 2;
	u32 CTRLPKT_CHK_EN: 1;
	u32 ALL_ACK_SUPPORT: 1;
	u32 RESP_REF_RATE: 9;
	u32 MFB: 7;

} CCTRL_INFO, *PCCTRL_INFO;


typedef struct _bb_sta_info {
	//downlink su
	u8 dl_su_bw: 2;
	u8 dl_su_gi_ltf: 3;
	u8 dl_su_coding: 1;
	u8 dl_su_txbf: 1;
	u8 dl_su_stbc: 1;

	u8 sgi_pending_cnt: 3; // For modified SGI try rate flow
	u8 rsvd1: 5; //for alignment

	u16 dl_su_dcm: 1;
	u16 rsvd2: 6;
	u16 dl_su_rate: 9;

	/*RA report*/
	u16 TOTAL;   /* TOTAL = 1*R[0]+2*R[1]+...+(N+1)*R[N]:rate down*/
	//u16 DROP;
	u16 OK;      /* OK = R[0]+R[1]+R[2]+R[3]: Initial rate, no rate down*/
	u16 OK_ALL;  /* OK_ALL = R[0]+R[1]+...+R[N]:rate down*/
	u8 TOTAL_PKT; /*Total Packet*/
	u8  TXCLS_PKT; /*Tx Collision Packet*/

	/*RA report for Beacon Cnt*/
	u16 BCN_TOTAL;    /* TOTAL = 1*R[0]+2*R[1]+...+(N+1)*R[N]:rate down*/
	u16 BCN_OK;       /* OK = R[0]+R[1]+R[2]+R[3]: Initial rate, no rate down*/
	u8  BCN_CNT;
	u8 	rsvd3;
	/*RA report for try rate*/

	u16 TRY_TOTAL;
	u16 TRY_OK;
	u16 TRY_OK_ALL;
	u8 TRY_TOTAL_PKT; /*Total Packet*/

	u8 disable_bw_switch;

	/*for H2C_AP_Req_Txrpt*/
	u8 avg_agg;
	u16 acq_rpt[2][4]; /*0: ok, 1: retry =1*/ /* BE BK VI VO */
	u16 TOTAL_CNT_C2H;
	// ra report info -- End

	u32 ra_mask;
	u32 backup_mask;
	// su-ra info
	u8 highest_rate;
	u8 lowest_rate;
	u8 rate;         /*curretn rate*/
	u8 pre_mode;
	u8 pre_rate;     /*pre rate*/
	u8 pre_gi_ltf;   // v
	u8 multi_rate[MULTI_RATE_NUM]; /*multi_rate[0]: 2nd rate, multi_rate[1]: 3rd rate*/
	u8 max_start_rate;

	u8 Initial_BW: 2; /*max support bw*/
	u8 BW_setting: 2; /*current bw */
	u8 pre_bw: 2;    /*pre bw */
	u8 rsvd4: 2;

	//u8 RateID: 5;
	u8 mode_sel: 5;
	/*en_cck, en_legofdm, en_ht, en_vht, en_he*/
	u8 DISPT: 1;
	u8 DISRA: 1;
	u8 rsvd5: 1;

	//u8 multi_rate_state_idx: 2;
	u8 updown_bitmap: 4;
	u8 rsvd6: 4;

	u8 rate_down_cnt;
	u8 rate_up_cnt;
	u8 rate_up_pending;

	u8 try_fail_cnt: 4; /*Just for debug*/
	u8 rate_up_down: 4;

	u8 VHT_EN: 2;
	u8 SGI: 1;
	u8 multi_rate_state: 2;
	u8 try_fail_count: 2;
	u8 trying_enable: 1;

	u8 rsvd7: 1; /*for power training*/
	u8 gi_ltf_cap: 3; /* bit(0)=4x0.8, bit(1)=1x0.8*/
	u8 max_ss_support: 3;
	u8 bForcePktRA: 1;

	u16 ra_ratio;       /*MA vlaue of ra_ratio*/
	u16 ra_down_ratio;  /*MA vlaue of ra_down_ratio*/

	u8 ldpc_cap: 1;
	u8 fix_giltf_en: 1;
	u8 fix_giltf: 3;
	u8 rsvd8: 3;

	u8 try_ness;
	u8 try_ness_max;

	u8 RSSI; /* It is also means RSSI_A when we send per path RSSI to FW */
	u8 bcn_rssi_a;

	// ============= RA ctrl info ============== //
	u8 RAINFO_STBC_STATE: 1;
	u8 ra_info_rsvd6: 2; // instead of SHORT_CUT
	u8 TxBF_STATE: 1;
	u8 ra_info_rsvd7: 4;
	// ======================================== //

#ifdef HALBB_CSI_RATE
	// ============= CSI rate ctrl info ============== //
	u8 ra_csi_rate_en: 1;
	u8 fixed_csi_rate_en: 1;
	u8 cr_tbl_sel: 1;
	u8 rsvd_csi_rate: 5;
	u8 band_num;
	u16 fixed_csi_rate;
	// ======================================== //
#endif
	// ============= FW fixed rate (H2C) ============== /
	u8 bfixed_rate: 1;
	u8 fixed_gi_ltf: 3;
	u8 fixed_bw: 2;
	u8 fixed_mode: 2;

	u8 fixed_rate: 7;
	u8 fixed_rsvd0: 1;

	// ============= Driver shift RU/RD threshold (H2C) ============== //
	u8 drv_shift_value: 7;
	u8 drv_shift_en: 1;
	// =============================================================== //

	// ============= Driver adjust RA mask (H2C) ============== //
	u8 mask_or_reveal: 1;
	u8 mask_rate: 7;

	u8 mask_rate_md: 2;
	u8 is_manual_adjust_ra_mask: 1;
	u8 backup_num: 1;
	u8 ra_mask_rsvd0: 4;
	// ======================================================== //


	u8 up_fail_limit_rate[LIMIT_RATE_SIZE]; /*for 1SS & 2ss rate*/
	u16 up_fail_limit_cnt[LIMIT_RATE_SIZE];
#if 0 // CONFIG_PKT_BASED_RA
	u8 ra_decision_occur_num;
	u8 trig_ra_pkt_num; /*make ra decision when collect enough(trig_ra_pkt_num) pachets*/
	u8 trigger_mode; /*0: default pkt based RA, 1: TBTT based RA*/
#endif
	// su-ra info -- END

	//
	u8 ser_try_cnt;

	u8 wd_not_release;
	//
	u16 rate_up_fail_cnt_lmt_val;
	u8    multi_try_times: 4;
	u8    consective_rate_up_fail_cnt: 4;
	// Multi Try RA
	u8    rate_up_waiting;
	//u8    ra_up_fail_rssi;
	u8    ra_candidate_rate1;
	u8    ra_candidate_rate2;
	u8    no_rate_up_cnt;

	u16 decision_offset_n;
	u16 decision_offset_p;
#ifdef CONFIG_RA_DEBUG
	u8 d_o_timer_max;
	u8 d_o_timer_cnt;
	u8 d_o_timer_state;
	BOOLEAN d_o_timer_en;
#endif
} BB_STA_INFO, *PBB_STA_INFO;


typedef  struct _RA_ADP_FUNC_ { /*the 4-th layer*/

	//PHYDM_RA_T             RA;
	//PHY_FWDBG_PARM    Phy_fwdbgParm;
	//TXRPT_INFO                 TxRptInfo;
	TxRpt_Cfg              TxRptCfg;
	CCTRL_INFO			   CtrlInfo;
	u8		RaMask[MACID_NUM][4];

} RA_ADP_FUNC, *PRA_ADP_FUNC;

struct _PPHYDM {

	//PHYDM_FUNC            Phydm;
	BB_STA_INFO sta_info_list[MACID_NUM];
	//PHYDM_RA_T phydm_ra;
#if CONFIG_RATE_ADAPTIVE
	RA_ADP_FUNC           Ra_adp;
#endif

	u8 en_th_adapt;
	u8 max_ser_try_cnt;
	//u32 RTY_C2H;
	PHYDM_RA_T phydm_ra;
	//BB_COM		com;
#ifdef CONFIG_RA_DEBUG
	u32 halbb_fwdbg_comp;
	/* Debug com for all bb features */
	u8 halbb_fwdbg_trace;
#endif
};

#endif
#endif //#ifndef __ODM_RAM_H__

