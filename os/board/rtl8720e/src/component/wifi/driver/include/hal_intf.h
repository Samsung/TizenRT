/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
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
 *
 ******************************************************************************/
#ifndef __HAL_INTF_H__
#define __HAL_INTF_H__

#ifdef CONFIG_80211AX_HE
#include "halbb_export_fun.h"
#include "halbb_hw_cfg_ex.h"
#include "halbb_ex.h"
#endif

#define phy_query_mac_reg(Adapter, RegAddr, BitMask) rtw_hal_read_bbreg((Adapter), (RegAddr), (BitMask))
#define phy_query_bb_reg(Adapter, RegAddr, BitMask) rtw_hal_read_bbreg((Adapter), (RegAddr), (BitMask))
#define phy_set_bb_reg(Adapter, RegAddr, BitMask, Data) rtw_hal_write_bbreg((Adapter), (RegAddr), (BitMask), (Data))
#define phy_query_rf_reg(Adapter, eRFPath, RegAddr, BitMask) rtw_hal_read_rfreg((Adapter), (eRFPath), (RegAddr), (BitMask))
#define phy_set_rf_reg(Adapter, eRFPath, RegAddr, BitMask, Data) rtw_hal_write_rfreg((Adapter), (eRFPath), (RegAddr), (BitMask), (Data))

enum RTL871X_HCI_TYPE {
	RTW_PCIE	= BIT0,
	RTW_USB 	= BIT1,
	RTW_SDIO	= BIT2,
	RTW_GSPI	= BIT3,
	RTW_LXBUS	= BIT4,
	RTW_AXI 	= BIT5
};

enum _CHIP_TYPE {

	NULL_CHIP_TYPE,
	RTL8721D_SOC,
	RTL8735B_SOC,
	RTL8720E_SOC,
	RTL8730E_SOC,
	RTL8721F_SOC,
	MAX_CHIP_TYPE
};

typedef enum _HW_VARIABLES {
	HW_VAR_SET_OPMODE,
	HW_VAR_MAC_ADDR,
	HW_VAR_BSSID,
	HW_VAR_BASIC_RATE,
	HW_VAR_TXPAUSE,
	HW_VAR_BCN_FUNC,
	HW_VAR_CORRECT_TSF,
	HW_VAR_CHECK_BSSID,
	HW_VAR_MLME_DISCONNECT,
	HW_VAR_MLME_SITESURVEY,
	HW_VAR_MLME_JOIN,
	HW_VAR_BEACON_INTERVAL,
	HW_VAR_SEC_CFG,
	HW_VAR_BCN_VALID,
	HW_VAR_RF_TYPE,
	HW_VAR_CAM_EMPTY_ENTRY,
	HW_VAR_CAM_INVALID_ALL,
	HW_VAR_CAM_READ,
	HW_VAR_ACM_CTRL,
	HW_VAR_AMPDU_MIN_SPACE,
	HW_VAR_AMPDU_FACTOR,
	HW_VAR_RXDMA_AGG_PG_TH,
	HW_VAR_TSF_PORT0,
	HW_VAR_TSF_PORT1,
	HW_VAR_SET_RPWM,
	HW_VAR_GET_RPWM,
	HW_VAR_CPWM,
	HW_VAR_H2C_FW_PWRMODE,
	HW_VAR_H2C_PS_TUNE_PARAM,
	HW_VAR_H2C_FW_JOINBSSRPT,
	HW_VAR_FWLPS_RF_ON,
	HW_VAR_TDLS_WRCR,
	HW_VAR_TDLS_RS_RCR,
	HW_VAR_INITIAL_GAIN,
#ifdef CONFIG_LPS_PG
	HW_VAR_LPS_PG_HANDLE,
#endif
	HW_VAR_CURRENT_ANTENNA,
	HW_VAR_ANTENNA_DIVERSITY_LINK,
	HW_VAR_ANTENNA_DIVERSITY_SELECT,
	HW_VAR_EFUSE_BYTES,
	HW_VAR_FIFO_CLEARN_UP,
	HW_VAR_CHECK_TXBUF,
	HW_VAR_APFM_ON_MAC, //Auto FSM to Turn On, include clock, isolation, power control for MAC only
	// The valid upper nav range for the HW updating, if the true value is larger than the upper range, the HW won't update it.
	// Unit in microsecond. 0 means disable this function.
#ifdef CONFIG_WOWLAN
	HW_VAR_WOWLAN,
	HW_VAR_WAKEUP_REASON,
	HW_VAR_RPWM_TOG,
#endif
#ifdef CONFIG_GPIO_WAKEUP
	HW_SET_GPIO_WL_CTRL,
#endif
	HW_VAR_NAV_UPPER,
	HW_VAR_C2H_HANDLE,
	HW_VAR_RPT_TIMER_SETTING,
	HW_VAR_TX_RPT_MAX_MACID,
	HW_VAR_H2C_MEDIA_STATUS_RPT,
	HW_VAR_CHK_HI_QUEUE_EMPTY,
	HW_VAR_DL_BCN_SEL,
	HW_VAR_PORT_SWITCH,
	HW_VAR_DO_IQK,
	HW_VAR_SET_REQ_FW_PS,
	HW_VAR_FW_PS_STATE,
	HW_VAR_DL_RSVD_PAGE,
#ifdef CONFIG_PROMISC
	HW_VAR_PROMISC,
#endif
	HW_VAR_SET_ICV,

	HW_VAR_SET_GNT_BT,
	HW_VAR_SET_GNT_BT_WITH_CLK_SRC,
	HW_VAR_SET_PTA_INIT,
	HW_VAR_GET_SUPPORT_BT,
	HW_VAR_GET_SUPPORT_BT_MESH,
	HW_VAR_GET_BT_FW,
	HW_VAR_GET_BT_FW_LEN,
	HW_VAR_ADAPTIVITY_CAM,
#ifdef CONFIG_BT_COEXIST_SOC
	HW_VAR_BTCOEX_RUN_CASE,
#endif
	HW_VAR_DATA_RATE_FALLBACK,
	HW_VAR_RETRY_LIMIT,
	HW_VAR_CTRL_PKT_RPT,
	HW_VAR_PORT1_NETTYPE,
	HW_VAR_TX_AGG_MAX_NUM,
	HW_VAR_CCA_EDCCA,
	HW_VAR_TXBUF_PKT_NUM,
	HW_VAR_SET_ANTENNA_DIVERSITY,
	HW_VAR_GET_THERMAL,

	/* add for 11ax support: start */
	HW_VAR_BSS_COLOR,
	/* add for 11ax support: end */
} HW_VARIABLES;

typedef enum _HAL_DEF_VARIABLE {
	HAL_DEF_IS_SUPPORT_ANT_DIV,
	HAL_DEF_CURRENT_ANTENNA,
	HAL_DEF_MAX_RECVBUF_SZ,
	HAL_DEF_RX_PACKET_OFFSET,
	HAL_DEF_RX_DMA_SZ_WOW,
	HAL_DEF_RX_DMA_SZ,
	HAL_DEF_DBG_DM_FUNC,//for dbg
	HAL_DEF_TX_LDPC,				/* LDPC support */
	HAL_DEF_RX_LDPC,				/* LDPC support */
	HAL_DEF_TX_STBC,				/* TX STBC support */
	HAL_DEF_RX_STBC, 				// RX STBC support
	HW_VAR_MAX_RX_AMPDU_FACTOR,
	HAL_DEF_DBG_DUMP_TXPKT,
	HAL_DEF_TX_PAGE_SIZE,
	HAL_DEF_TX_PAGE_BOUNDARY,
	HAL_DEF_EXPLICIT_BEAMFORMEE,
	HAL_DEF_VHT_MU_BEAMFORMEE,
	HAL_DEF_BEAMFORMEE_CAP,
} HAL_DEF_VARIABLE;

typedef enum _HAL_ODM_VARIABLE {
	HAL_ODM_STA_INFO,
//	HAL_ODM_P2P_STATE,
//	HAL_ODM_WIFI_DISPLAY_STATE,
	HAL_ODM_DBG_FLAG,
//	HAL_ODM_DBG_LEVEL,
	HAL_ODM_RX_INFO_DUMP,
	HAL_ODM_NOISE_MONITOR,
	HAL_ODM_REGULATION,
} HAL_ODM_VARIABLE;


#define C2H_TYPE_REG 0
#define C2H_TYPE_PKT 1

/*
* C2H event format:
* Fields    TRIGGER     PLEN      PAYLOAD    SEQ      ID
* BITS    [127:120]  [119:112]  [111:16]   [15:8]   [7:0]
*/
#define C2H_ID_88XX(_c2h)		LE_BITS_TO_1BYTE(((u8*)(_c2h)), 0, 8)
#define C2H_SEQ_88XX(_c2h)		LE_BITS_TO_1BYTE(((u8*)(_c2h)) + 1, 0, 8)
#define C2H_PAYLOAD_88XX(_c2h)	(((u8*)(_c2h)) + 2)
#define C2H_PLEN_88XX(_c2h)		LE_BITS_TO_1BYTE(((u8*)(_c2h)) + 14, 0, 8)
#define C2H_TRIGGER_88XX(_c2h)	LE_BITS_TO_1BYTE(((u8*)(_c2h)) + 15, 0, 8)

//TODO

#ifdef CONFIG_WOWLAN
#define Rx_Pairwisekey			0x01
#define Rx_GTK					0x02
#define Rx_DisAssoc				0x04
#define Rx_DeAuth				0x08
#define FWDecisionDisconnect	0x10
#define Rx_MagicPkt				0x21
#define Rx_UnicastPkt			0x22
#define Rx_PatternPkt			0x23
#endif

#define rtw_haldata_init(padapter) wifi_hal_init_haldata(padapter)
#define rtw_haldata_deinit(padapter) wifi_hal_free_haldata(padapter)
#define rtw_hal_fill_h2c_cmd(padapter, id, buf_len, pbuf) wifi_hal_fill_h2c_cmd(padapter, id, buf_len, pbuf)
#define rtw_hal_read_chip_info(padapter) wifi_hal_read_adapterinfo(padapter)
#define rtw_hal_read_chip_version(padapter) wifi_hal_read_chipversion(padapter)
#define rtw_hal_dm_init(padapter) wifi_hal_phy_init_dm_priv(padapter)
#define rtw_hal_init(padapter) wifi_hal_init(padapter)
#define rtw_hal_deinit(padapter) wifi_hal_deinit(padapter)
#define rtw_hal_set_msr(padapter, val) ROM_WIFI_SetMSR(padapter->iface_type, val)
#define rtw_hal_update_slottime(updateCap, preamble_mode, slotTime, cur_wireless_mode) ROM_WIFI_UPDATE_Slottime(updateCap, preamble_mode, slotTime, cur_wireless_mode)
#define rtw_hal_set_hwreg(padapter, variable, val) wifi_hal_hwreg_set(padapter, variable, val)
#define rtw_hal_get_hwreg(padapter, variable, val) wifi_hal_hwreg_get(padapter, variable, val)
#define rtw_hal_set_def_var(padapter, eVariable, pValue) wifi_hal_defaultvar_set(padapter, eVariable, pValue)
#define rtw_hal_get_def_var(padapter, eVariable, pValue) wifi_hal_defaultvar_get(padapter, eVariable, pValue)
#define rtw_hal_enable_interrupt(padapter) wifi_hal_interrupt_enable(padapter)
#define rtw_hal_disable_interrupt(padapter) wifi_hal_interrupt_disable(padapter)
#define rtw_hal_descring_init(padapter) wifi_hal_desc_ring_init(padapter)
#define rtw_hal_descring_deinit(padapter) wifi_hal_desc_ring_free(padapter)
#define rtw_hal_descring_reset(padapter) wifi_hal_desc_ring_reset(padapter)
#define rtw_hal_xmit(padapter, pxmitframe) wifi_hal_xmit(padapter, pxmitframe)
#define rtw_hal_mgnt_xmit(padapter, pmgntframe) wifi_hal_mgnt_xmit(padapter, pmgntframe)
#define rtw_hal_init_xmit_priv(padapter) wifi_hal_init_xmit_priv(padapter)
#define rtw_hal_free_xmit_priv(padapter) wifi_hal_free_xmit_priv(padapter)
#define rtw_hal_read_bbreg(padapter, RegAddr, BitMask) wifi_hal_read_bb_reg(padapter, RegAddr, BitMask)
#define rtw_hal_write_bbreg(padapter, RegAddr, BitMask, Data) wifi_hal_write_bb_reg(padapter, RegAddr, BitMask, Data)
#define rtw_hal_read_rfreg(padapter, eRFPath, RegAddr, BitMask) wifi_hal_read_rf_reg(padapter, eRFPath, RegAddr, BitMask)
#define rtw_hal_write_rfreg(padapter, eRFPath, RegAddr, BitMask, Data) wifi_hal_write_rf_reg(padapter, eRFPath, RegAddr, BitMask, Data)
#define rtw_hal_interrupt_handler(padapter)	wifi_hal_interrupt_handle(padapter)
#define rtw_hal_set_chan(padapter, channel)	wifi_hal_set_channel(padapter, channel)
#define rtw_hal_set_chnl_bw(padapter, channel, channel_offset, bwmode) wifi_hal_set_channel_bw(padapter, channel, channel_offset, bwmode)
#define rtw_hal_dm_watchdog(padapter)	wifi_hal_phy_haldm_watchdog(padapter)
#define rtw_hal_dbg(padapter, p)	wifi_hal_dbg(padapter, p)
#define rtw_hal_phydm_dbg(padapter, p) wifi_hal_phydm_dbg(padapter, p)
#define rtw_hal_antdiv_before_linked(padapter)	FALSE
#define rtw_hal_antdiv_rssi_compared(padapter, dst, src)
#define rtw_hal_macid_sleep(padapter, macid)
#define rtw_hal_macid_wakeup(padapter, macid)
#define rtw_hal_add_ra_tid(padapter, bitmap,arg, rssi_level)
#define rtw_hal_clone_data(dst_padapter, src_padapter)
#define	rtw_hal_update_ra_mask(padapter, mac_id, rssi_level)

#ifdef CONFIG_MBSSID_AX
#define rtw_hal_mbssid_sta_cfg(padapter, enable, bssid_idx_to_join, bssid_ref)	ROM_WIFI_MBSSID_Cfg(enable, bssid_idx_to_join, bssid_ref)
#else
#define rtw_hal_mbssid_sta_cfg(padapter, enable, bssid_idx_to_join, bssid_ref)
#endif
#ifdef CONFIG_CSI
#define rtw_hal_csi_en(padapter, act_param)	wifi_hal_csi_en(padapter, act_param)
#define rtw_hal_csi_cfg(padapter, act_param)	wifi_hal_csi_cfg(padapter, act_param)
#define rtw_hal_csi_report(pcsipriv, buf_len, csi_buf, len)	wifi_hal_csi_report(pcsipriv, buf_len, csi_buf, len)
#else
#define rtw_hal_csi_en(padapter, act_param)	FAIL
#define rtw_hal_csi_cfg(padapter, act_param)	FAIL
#define rtw_hal_csi_report(padapter, buf_len, csi_buf, len)	FAIL
#endif
#ifdef CONFIG_TWT
#define rtw_hal_twt_para_set(padapter, twt_para, enable)	wifi_hal_set_twt_para_cmd(padapter, twt_para, enable)
#else
#define rtw_hal_twt_para_set(padapter, twt_para, enable)	FAIL
#endif
#ifdef CONFIG_80211AX_HE
#define rtw_hal_set_bss_color(padapter, bss_color, phy_idx) halbb_set_bss_color(rtw_get_haldata(padapter)->bb, bss_color, phy_idx)
#else
#define rtw_hal_set_bss_color(padapter, bss_color, phy_idx)
#endif

#if defined(CONFIG_RTL8721D)
#define	rtw_hal_fill_fake_txdesc(padapter, pDesc, BufferLen, IsPsPoll, IsBTQosNull,bDataFrame)
#define rtw_hal_dm_deinit(padapter)
#define rtw_hal_dm_cancel_timer(padapter) wifi_hal_phy_dm_timer_cancel(padapter)
#define rtw_hal_set_mu_edca_param(val, first)
#define rtw_hal_set_spatial_reuse(padapter, val, enable, first)
#define rtw_hal_11ax_limitation(padapter)	FALSE
#define rtw_hal_dm_cmd(padapter, input, output, out_len)
#define rtw_hal_dbg_no_beacon(padapter, param)
#define rtw_hal_add_bb_sta(padapter, psta)
#define rtw_hal_del_bb_sta(padapter, psta)
#define rtw_hal_set_TXOP_thres(padapter, rts_th, enable)
#define rtw_hal_set_odm_var(padapter, eVariable, pValue1, bSet) wifi_hal_odmvar_set(padapter, eVariable, pValue1, bSet)
#define rtw_hal_get_odm_var(padapter, eVariable, pValue1, pValue2) wifi_hal_odmvar_get(padapter, eVariable, pValue1, pValue2)
#define rtw_hal_phy_statistic_query(padapter, phy_statistic)	wifi_hal_query_phy_statistic(padapter, phy_statistic)
#define rtw_hal_set_txpwr_done(padapter)
#endif
#if defined(CONFIG_RTL8721F)
#define	rtw_hal_fill_fake_txdesc(padapter, pDesc, BufferLen, IsPsPoll, IsBTQosNull,bDataFrame)
#define rtw_hal_dm_deinit(padapter)
#define rtw_hal_dm_cancel_timer(padapter) wifi_hal_phy_dm_timer_cancel(padapter)
#define rtw_hal_set_mu_edca_param(val, first)
#define rtw_hal_set_spatial_reuse(padapter, val, enable, first)
#define rtw_hal_11ax_limitation(padapter)	FALSE
#define rtw_hal_dm_cmd(padapter, input, output, out_len)
#define rtw_hal_dbg_no_beacon(padapter, param)
#define rtw_hal_add_bb_sta(padapter, psta)
#define rtw_hal_del_bb_sta(padapter, psta)
#define rtw_hal_set_TXOP_thres(padapter, rts_th, enable)
#define rtw_hal_set_odm_var(padapter, eVariable, pValue1, bSet) wifi_hal_odmvar_set(padapter, eVariable, pValue1, bSet)
#define rtw_hal_get_odm_var(padapter, eVariable, pValue1, pValue2) wifi_hal_odmvar_get(padapter, eVariable, pValue1, pValue2)
#define rtw_hal_phy_statistic_query(padapter, phy_statistic)
#define rtw_hal_set_txpwr_done(padapter)
#endif
#if defined(CONFIG_RTL8720E)
#define	rtw_hal_fill_fake_txdesc(padapter, pDesc, BufferLen, IsPsPoll, IsBTQosNull,bDataFrame)
#define rtw_hal_dm_deinit(padapter) wifi_hal_phy_deinit_dm_priv(padapter)
#define rtw_hal_dm_cancel_timer(padapter)
#define rtw_hal_set_mu_edca_param(val, first) ROM_WIFI_Set_MUEDCA_Para(val, first)
#define rtw_hal_set_spatial_reuse(padapter, val, enable, first) ROM_WIFI_Set_Special_Reuse(val, enable, first)
#define rtw_hal_11ax_limitation(padapter)	wifi_hal_11ax_limitation()
#define rtw_hal_dm_cmd(padapter, input, output, out_len)	wifi_hal_phy_dm_cmd(padapter, input, output, out_len)
#define rtw_hal_dbg_no_beacon(padapter, param)	wifi_hal_dbg_no_beacon(param)
#define rtw_hal_add_bb_sta(padapter, psta)	wifi_hal_bb_sta_add(padapter, psta)
#define rtw_hal_del_bb_sta(padapter, psta)	wifi_hal_bb_sta_del(padapter, psta)
#define rtw_hal_set_TXOP_thres(padapter, rts_th, enable)	ROM_WIFI_Set_TXOP_Thres(rts_th, enable)
#define rtw_hal_set_odm_var(padapter, eVariable, pValue1, bSet)
#define rtw_hal_get_odm_var(padapter, eVariable, pValue1, pValue2)
#define rtw_hal_phy_statistic_query(padapter, phy_statistic)
#endif
#if defined(CONFIG_RTL8730E)
#define	rtw_hal_fill_fake_txdesc(padapter, pDesc, BufferLen, IsPsPoll, IsBTQosNull,bDataFrame)
#define rtw_hal_dm_deinit(padapter) wifi_hal_phy_deinit_dm_priv(padapter)
#define rtw_hal_dm_cancel_timer(padapter)
#define rtw_hal_set_mu_edca_param(val, first) ROM_WIFI_Set_MUEDCA_Para(val, first)
#define rtw_hal_set_spatial_reuse(padapter, val, enable, first) ROM_WIFI_Set_Special_Reuse(val, enable, first)
#define rtw_hal_11ax_limitation(padapter)	FALSE
#define rtw_hal_dm_cmd(padapter, input, output, out_len)	wifi_hal_phy_dm_cmd(padapter, input, output, out_len)
#define rtw_hal_dbg_no_beacon(padapter, param)	wifi_hal_dbg_no_beacon(param)
#define rtw_hal_add_bb_sta(padapter, psta)	wifi_hal_bb_sta_add(padapter, psta)
#define rtw_hal_del_bb_sta(padapter, psta)	wifi_hal_bb_sta_del(padapter, psta)
#define rtw_hal_set_TXOP_thres(padapter, rts_th, enable)	ROM_WIFI_Set_TXOP_Thres(rts_th, enable)
#define rtw_hal_set_odm_var(padapter, eVariable, pValue1, bSet)
#define rtw_hal_get_odm_var(padapter, eVariable, pValue1, pValue2)
#define rtw_hal_phy_statistic_query(padapter, phy_statistic)
#endif
#if defined(CONFIG_RTL8735B)
#define	rtw_hal_fill_fake_txdesc(padapter,pDesc, BufferLen, IsPsPoll, IsBTQosNull,bDataFrame) wifi_hal_fill_fake_txdesc(padapter,pDesc, BufferLen, IsPsPoll, IsBTQosNull,bDataFrame)
#define rtw_hal_dm_deinit(padapter)
#define rtw_hal_dm_cancel_timer(padapter) wifi_hal_phy_dm_timer_cancel(padapter)
#define rtw_hal_set_mu_edca_param(val, first)
#define rtw_hal_set_spatial_reuse(padapter, val, enable, first)
#define rtw_hal_11ax_limitation(padapter)	FALSE
#define rtw_hal_dm_cmd(padapter, input, output, out_len)
#define rtw_hal_dbg_no_beacon(padapter, param)
#define rtw_hal_add_bb_sta(padapter, psta)
#define rtw_hal_del_bb_sta(padapter, psta)
#define rtw_hal_set_TXOP_thres(padapter, rts_th, enable)
#define rtw_hal_set_odm_var(padapter, eVariable, pValue1, bSet) wifi_hal_odmvar_set(padapter, eVariable, pValue1, bSet)
#define rtw_hal_get_odm_var(padapter, eVariable, pValue1, pValue2) wifi_hal_odmvar_get(padapter, eVariable, pValue1, pValue2)
#define rtw_hal_phy_statistic_query(padapter, phy_statistic)
#define rtw_hal_set_txpwr_done(padapter)	wifi_hal_set_txpwr_done(padapter)
#define rtw_hal_mac_c2h_handler(padapter, buf, len)	wifi_hal_c2h_handler(padapter, buf, len)
#undef rtw_hal_set_msr
#define rtw_hal_set_msr(padapter, val)	wifi_hal_set_msr(padapter, val)
#define rtw_hal_update_slottime(updateCap, preamble_mode, slotTime, cur_wireless_mode) wifi_hal_update_slottime(updateCap, preamble_mode, slotTime, cur_wireless_mode)

#endif

#endif //__HAL_INTF_H__

