/******************************************************************************
 *
 * Copyright(c) 2015 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 ******************************************************************************/
#ifndef _HAL_MCC_H_
#define _HAL_MCC_H_

#include <drv_types.h> /* PADAPTER */

#ifdef CONFIG_MCC_MODE

#if defined(CONFIG_PLATFORM_8195A)
#include "rtl8195a/rtl8195a_cmd.h"
#ifdef CONFIG_MCC_MODE
#include "rtl8195a/rtl8195a_mcc.h"
#endif
#define RSVDPAGE_LOC  RSVDPAGE_LOC_8195A
#elif defined(CONFIG_RTL8723D)
#include "rtl8723d/rtl8723d_cmd.h"
#define FCS_REQ_END   0xF
#elif defined(CONFIG_RTL8188F)
#include "rtl8188f/rtl8188f_cmd.h"
#define FCS_REQ_END   0xF
#endif

#ifdef CONFIG_MCC_MODE

#define CONFIG_RTW_MCC_EN					1
#define CONFIG_RTW_MCC_POLICY_IDX			4

/* ---------------------------------------------------------------------------------------------------------
 * -----------------------------------  H2C CMD DEFINITION  ------------------------------------------------
 * ---------------------------------------------------------------------------------------------------------
 * 88e, 8723b, 8812, 8821, 92e use the same FW code base */
#if defined(CONFIG_PLATFORM_8195A)
enum h2c_cmd {
#ifdef CONFIG_MCC_MODE
	H2C_MCC_LOCATION = 0x10,
	H2C_MCC_UPDATE_PARAM = 0x15,
	H2C_MCC_MACID_BITMAP = 0x16,
	H2C_MCC_CTRL_NEW = 0x17, //for 8822B and 8821C
	H2C_MCC_CTRL = 0x18,
	H2C_MCC_TIME_SETTING = 0x19,
	H2C_MCC_IQK_PARAM = 0x1A, //for 11ac IC IQK
#endif
	H2C_MAXID,
};
#endif

#define H2C_MCC_CTRL_LEN			7
#define H2C_MCC_LOCATION_LEN		3
#define H2C_MCC_MACID_BITMAP_LEN	6
#define H2C_MCC_UPDATE_INFO_LEN		4
#define H2C_MCC_TIME_SETTING_LEN	6
#define H2C_MCC_IQK_PARAM_LEN		7
#ifdef CONFIG_MCC_MODE_DEBUG
#define H2C_MCC_RQT_TSF__LEN		1
#endif

#ifdef CONFIG_MCC_MODE_DEBUG
/* MCC RQT TSF CMD 0xF*/
#define SET_H2CCMD_MCC_RQT_TSFX(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 4, __Value)
#define SET_H2CCMD_MCC_RQT_TSFY(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 4, __Value)
#endif

/* MCC LOC CMD 0x10 */
#define SET_H2CCMD_MCC_RSVDPAGE_LOC(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)

/* MCC MAC ID CMD 0x16 */
#define SET_H2CCMD_MCC_MACID_BITMAP_L(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 8, __Value)
#define SET_H2CCMD_MCC_MACID_BITMAP_H(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)

/* NEW MCC CTRL CMD 0x17 */
#define SET_H2CCMD_MCC_CTRL_V2_ORDER(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_TOTALNUM(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_CENTRAL_CH(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_PRIMARY_CH(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_BW(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 4, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_DURATION(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_ROLE(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 3, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_INCURCH(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 3, 1, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_DIS_SW_RETRY(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 4, 1, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_DISTXNULL(__pH2CCmd, __Value)		SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 5, 1, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_C2HRPT(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 6, 2, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_TSFX(__pH2CCmd, __Value)				SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_RSVD0(__pH2CCmd, __Value)			SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 4, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_V2_UPDATE_PARM(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 7, 1, __Value)

/* MCC CTRL CMD 0x18 */
#define SET_H2CCMD_MCC_CTRL_ORDER(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_TOTALNUM(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_CHIDX(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_MCC_CTRL_BW(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 2, __Value)
#define SET_H2CCMD_MCC_CTRL_BW40SC(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 2, 3, __Value)
#define SET_H2CCMD_MCC_CTRL_BW80SC(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 5, 3, __Value)
#define SET_H2CCMD_MCC_CTRL_DURATION(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value)
#define SET_H2CCMD_MCC_CTRL_ROLE(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 3, __Value)
#define SET_H2CCMD_MCC_CTRL_INCURCH(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 3, 1, __Value)
#define SET_H2CCMD_MCC_CTRL_RSVD0(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 4, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_RSVD1(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 8, __Value)
#define SET_H2CCMD_MCC_CTRL_RFETYPE(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 0, 4, __Value)
#define SET_H2CCMD_MCC_CTRL_DISTXNULL(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 4, 1, __Value)
#define SET_H2CCMD_MCC_CTRL_C2HRPT(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 5, 2, __Value)
#define SET_H2CCMD_MCC_CTRL_CHSCAN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 7, 1, __Value)

/* MCC NoA CMD 0x19 */
#define SET_H2CCMD_MCC_TIME_SETTING_FW_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_TSF_SYNC_OFFSET(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 7, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_START_TIME(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_INTERVAL(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 8, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_EARLY_SWITCH_RPT(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 8, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_ORDER_BASE(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 4, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_ORDER_SYNC(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 4, 4, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_UPDATE(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 1, __Value)
#define SET_H2CCMD_MCC_TIME_SETTING_DURATION(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 1, 7, __Value)

/* MCC IQK CMD 0x1A */
#define SET_H2CCMD_MCC_IQK_READY(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)
#define SET_H2CCMD_MCC_IQK_ORDER(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 4, __Value)
#define SET_H2CCMD_MCC_IQK_PATH(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 5, 2, __Value)
#define SET_H2CCMD_MCC_IQK_RX_L(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+1, 0, 8, __Value)
#define SET_H2CCMD_MCC_IQK_RX_M1(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 0, 2, __Value)
#define SET_H2CCMD_MCC_IQK_RX_M2(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+2, 2, 6, __Value)
#define SET_H2CCMD_MCC_IQK_RX_H(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+3, 0, 4, __Value)
#define SET_H2CCMD_MCC_IQK_TX_L(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+4, 0, 8, __Value)
#define SET_H2CCMD_MCC_IQK_TX_M1(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 0, 3, __Value)
#define SET_H2CCMD_MCC_IQK_TX_M2(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+5, 3, 5, __Value)
#define SET_H2CCMD_MCC_IQK_TX_H(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE((__pH2CCmd)+6, 0, 6, __Value)
#endif

#define MCC_STATUS_PROCESS_MCC_START_SETTING BIT0
#define MCC_STATUS_PROCESS_MCC_STOP_SETTING BIT1
#define MCC_STATUS_NEED_MCC BIT2
#define MCC_STATUS_DOING_MCC BIT3

#define MCC_SWCH_FW_EARLY_TIME 10 /* ms */
#define MCC_EXPIRE_TIME 50 /* ms */
#define MCC_TOLERANCE_TIME 2 /* 2*2 = 4s */

#define MCC_ROLE_STA_GC_MGMT_QUEUE_MACID 0
#define MCC_ROLE_SOFTAP_GO_MGMT_QUEUE_MACID 1

/* Lower for stop, Higher for start */
#define MCC_SETCMD_STATUS_STOP_DISCONNECT 0x0
#define MCC_SETCMD_STATUS_STOP_SCAN_START 0x1
#define MCC_SETCMD_STATUS_START_CONNECT 0x80
#define MCC_SETCMD_STATUS_START_SCAN_DONE 0x81

/*
* depenad platform or customer requirement(TP unit:Mbps),
* must be provided by PM or sales or product document
* too large value means not to limit tx bytes (current for ap mode)
* NOTE: following values ref from test results
*/
#define MCC_AP_BW20_TARGET_TX_TP (300)
#define MCC_AP_BW40_TARGET_TX_TP (300)
#define MCC_AP_BW80_TARGET_TX_TP (300)
#define MCC_STA_BW20_TARGET_TX_TP (35)
#define MCC_STA_BW40_TARGET_TX_TP (70)
#define MCC_STA_BW80_TARGET_TX_TP (140)
#define MCC_SINGLE_TX_CRITERIA 5 /* Mbps */

#define MAX_MCC_NUM 2

#define MCC_STOP(adapter) (adapter->mcc_adapterpriv.mcc_tx_stop)
#define MCC_EN(adapter) (adapter->registrypriv.en_mcc)

/* Represent Channel Tx Null setting */
enum mcc_channel_tx_null {
	MCC_ENABLE_TX_NULL = 0,
	MCC_DISABLE_TX_NULL = 1,
};

/* Represent C2H Report setting */
enum mcc_c2h_report {
	MCC_C2H_REPORT_DISABLE = 0,
	MCC_C2H_REPORT_FAIL_STATUS = 1,
	MCC_C2H_REPORT_ALL_STATUS = 2,
};

/* Represent Channel Scan */
enum mcc_channel_scan {
	MCC_CHIDX = 0,
	MCC_SCANCH_RSVD_LOC = 1,
};

/* Represent FW status report of channel switch */
enum mcc_status_rpt {
	MCC_RPT_SUCCESS					= 0,
	MCC_RPT_TXNULL_FAIL				= 1,
	MCC_RPT_STOPMCC					= 2,
	MCC_RPT_READY					= 3,
	MCC_RPT_STOPFCS_FAIL			= 4,
	MCC_RPT_MCC_ENABLE_FAIL			= 5,
	MCC_RPT_MCC_SWITCH_CH_FAIL		= 6,
	MCC_RPT_MCC_SWITCH_EARLY_RPT	= 7,
	MCC_RPT_BCN_EARLY				= 8,
	//MCC_RPT_TSFX					= 9,
	MCC_RPT_MAX,
};

enum MCC_ROLE {
	MCC_ROLE_STA = 0,
	MCC_ROLE_AP = 1,
	MCC_ROLE_GC = 2,
	MCC_ROLE_GO = 3,
	MCC_ROLE_MAX,
};

struct mcc_iqk_backup {
	u16 TX_X;
	u16 TX_Y;
	u16 RX_X;
	u16 RX_Y;
};

/*	mcc data for adapter */
struct mcc_adapter_priv {
	u8 order;		/* FW document, softap/AP must be 0 */
	u8 role;			/* MCC role(AP,STA,GO,GC) */
	u8 mcc_duration; /* channel stay period, UNIT:1TU */

	/* flow control */
	u8 mcc_tx_stop;				/* check if tp stop or not */
	u8 mcc_tp_limit;				/* check if tp limit or not */
	u32 mcc_target_tx_bytes_to_port;		/* customer require	 */
	u32 mcc_tx_bytes_to_port;	/* already tx to tx fifo (write port) */

#ifdef CONFIG_RUNTIME_MCC
	RUNTIME_MCC_PRIV;
#endif
	/* data from kernel to check if enqueue data or netif stop queue */
	u32 mcc_tp;
	u64 mcc_tx_bytes_from_kernel;
	u64 mcc_last_tx_bytes_from_kernel;

	/* Backup IQK value for MCC */
	struct mcc_iqk_backup mcc_iqk_arr[MAX_RF_PATH];

	/* mgmt queue macid to avoid RA issue */
	u8 mgmt_queue_macid;

	/* set macid bitmap to let fw know which macid should be tx pause */
	/* all interface share total 16 macid */
	u16 mcc_macid_bitmap;

#ifdef CONFIG_P2P_NEW
	/* use for NoA start time (unit: mircoseconds) */
	u32 noa_start_time;

	u8 p2p_go_noa_ie[MAX_P2P_IE_LEN];
	u32 p2p_go_noa_ie_len;
#endif

#ifdef CONFIG_PLATFORM_8195A
	FCSInfo FCSCtrl;
#endif
};

struct mcc_obj_priv {
	u8 duration; /* channel stay period, UNIT:1TU */
	u8 mcc_c2h_status;
	u8 cur_mcc_success_cnt; /* used for check mcc switch channel success */
	u8 prev_mcc_success_cnt; /* used for check mcc switch channel success */
	u8 mcc_tolerance_time; /* used for detect mcc switch channel success */
	u8 mcc_loc_rsvd_paga[MAX_MCC_NUM];	/* mcc rsvd page */
	u8 mcc_status; /* mcc status stop or start .... */
	u8 policy_index;
	u32 mcc_launch_time; /* mcc launch time, used for starting detect mcc switch channel success */
	_mutex mcc_mutex;
	_lock mcc_lock;
	PADAPTER iface[MAX_MCC_NUM]; /* by order, use for mcc parameter cmd */
	struct submit_ctx mcc_sctx;

#ifdef CONFIG_RUNTIME_MCC
	MCC_EXT_OBJ_PRIV;
#endif
};

/* backup IQK val */
void rtw_hal_mcc_restore_iqk_val(PADAPTER padapter);

/* check mcc status */
u8 rtw_hal_check_mcc_status(PADAPTER padapter, u8 mcc_status);

/* set mcc status */
void rtw_hal_set_mcc_status(PADAPTER padapter, u8 mcc_status);

/* clear mcc status */
void rtw_hal_clear_mcc_status(PADAPTER padapter, u8 mcc_status);

/* dl mcc rsvd page */
u8 rtw_hal_dl_mcc_fw_rsvd_page(_adapter *adapter, u8 *pframe, u16 *index
	, u8 tx_desc, u32 page_size, u8 *page_num, u32 *total_pkt_len, RSVDPAGE_LOC *rsvd_page_loc);

/* handle C2H */
void rtw_hal_mcc_c2h_handler(PADAPTER padapter, u8 buflen, u8 *tmpBuf);

/* switch channel successfully or not */
void rtw_hal_mcc_sw_status_check(PADAPTER padapter);

/* change some scan flags under site survey */
u8 rtw_hal_mcc_change_scan_flag(PADAPTER padapter, u8 *ch, u8 *bw, u8 *offset);

/* record data kernel TX to driver to check MCC concurrent TX  */
void rtw_hal_mcc_calc_tx_bytes_from_kernel(PADAPTER padapter, u32 len);

/* record data to port to let driver do flow ctrl  */
void rtw_hal_mcc_calc_tx_bytes_to_port(PADAPTER padapter, u32 len);

/* check stop write port or not	 */
u8 rtw_hal_mcc_stop_tx_bytes_to_port(PADAPTER padapter);

u8 rtw_hal_set_mcc_setting_scan_start(PADAPTER padapter);

u8 rtw_hal_set_mcc_setting_scan_complete(PADAPTER padapter);

u8 rtw_hal_set_mcc_setting_start_bss_network(PADAPTER padapter, u8 chbw_grouped);

u8 rtw_hal_set_mcc_setting_disconnect(PADAPTER padapter);

u8 rtw_hal_set_mcc_setting_join_done_chk_ch(PADAPTER padapter);

u8 rtw_hal_set_mcc_setting_chk_start_clnt_join(PADAPTER padapter, u8 *ch, u8 *bw, u8 *offset, u8 chbw_allow);

void rtw_hal_dump_mcc_info(void *sel, struct dvobj_priv *dvobj);

void update_mcc_mgntframe_attrib(_adapter *padapter, struct pkt_attrib *pattrib);

u8 rtw_hal_mcc_link_status_chk(_adapter *padapter, const char *msg);

void rtw_hal_mcc_issue_null_data(_adapter *padapter, u8 chbw_allow, u8 ps_mode);

#ifdef CONFIG_P2P_NEW
u8 *rtw_hal_mcc_append_go_p2p_ie(PADAPTER padapter, u8 *pframe, u32 *len);
#endif

void rtw_hal_mcc_update_switch_channel_policy_table(PADAPTER padapter);

void rtw_hal_dump_mcc_policy_table(void *sel);

void rtw_hal_mcc_update_macid_bitmap(PADAPTER padapter, u8 mac_id, u8 add);

#endif	//CONFIG_MCC_MODE
#endif	//_HAL_MCC_H_

