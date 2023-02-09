#ifndef __WIFIFW_TXREPORT_RAM_H__
#define __WIFIFW_TXREPORT_RAM_H__
/*--------------------Define MACRO--------------------------------------*/
#define TEST_FORCE_TRXRPT_FULL          0       //set 1 to force TRxRPT full
#define TXRPTFIELD_RPT_SEL              (BIT7|BIT6|BIT5)

/*--------------------Define Struct---------------------------------------*/
typedef enum _RPT_TYPE_ {
	TXRPT       = 0,
	FTMRPT      = 1,
	CCXRPT      = 2,
	FTMACKRPT   = 3,
	TXBCNRPT    = 4,
	RXBCNRPT    = 6,
	TXRPTNew	= 7,
	TRIGGER		= 8,
} RPT_TYPE, *PRPT_TYPE;

typedef struct _TxRpt_Cfg_ {
	//offset 0
	u8 tx_polluted: 1;
	u8 tx_state: 4;
	u8 rpt_sel: 3;
	//offset 1
	u8 rpt_qsel: 5;
	u8 collision_head: 1; //for 97G/12F
	u8 collision_tail: 1; //for 97G/12F
	u8 fixed_rate: 1; //driver fixed rate
	//offset 2
	u16 macid: 7;
	//offset 3
	u16 init_rate: 9;
	//offset 4
	u8 ok_num: 6;
	u8 rsvd0: 1;
	u8 try_rate: 1;
	//offset 5
	u8 drop_num: 6;
	u8 rsvd1: 2;
	//offset 6
	u8 sw_def;
	//offset 7
	u8 rts_rty_count: 4;
	u8 tb_ppdu_flag: 1;
	u8 rts_bmc: 1;
	u8 rx_polluted: 1;
	u8 rsvd2: 1;
	//offset 8
	u16 data_rty_count: 6;
	//offset 9
	u16 final_data_rate: 9;
	u16 rsvd3: 1;
	//offset 10
	u8 rsvd4;
	//offset 11
	u8 rsvd5: 2;
	u8 ch_sc: 4;
	u8 ch_bw: 2;
	//offset 12
	u8 total_cnt: 6; /*total number of MPDU have been transmitted*/
	u8 ppdu_type: 2;
	//offset 13
	u8 zero_retry_pkt_num: 6;
	u8 rsvd: 2;
	//offset 14
	u16 bSR: 1;
	u16 init_data_gi_ltf: 3;
	u16 final_gi_ltf: 3;
	//offset 15
	u16 txpwr_pd: 5;
} TxRpt_Cfg, *pTxRpt_Cfg;


/*------------------------------Function declaration--------------------------*/
extern void InitCtrlInfo_8720E(void);
extern void ParseTRXRpt(void);

#endif  /* #ifndef   __WIFIFW_TXREPORT_RAM_H__ */
