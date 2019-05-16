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
	RTL8188C_8192C,
	RTL8192D,
	RTL8723A,
	RTL8188E,
	RTL8192E,
	RTL8812,
	RTL8821, //RTL8811
	RTL8723B,
	RTL8195A_SOC,
	RTL8711B_SOC,
	RTL8188F,
	RTL8721D_SOC,	
	RTL8821C,
	RTL8192F,
	RTL8723D,
	RTL8195B_SOC,
	RTL8710C_SOC,
	MAX_CHIP_TYPE
};
typedef enum _HW_VARIABLES{
	HW_VAR_MEDIA_STATUS,  //0
	HW_VAR_MEDIA_STATUS1,
	HW_VAR_SET_OPMODE,
	HW_VAR_MAC_ADDR,
	HW_VAR_BSSID,
	HW_VAR_INIT_RTS_RATE,
	HW_VAR_BASIC_RATE,
	HW_VAR_TXPAUSE,
	HW_VAR_BCN_FUNC,
	HW_VAR_CORRECT_TSF,
	HW_VAR_CHECK_BSSID, //10
	HW_VAR_MLME_DISCONNECT,
	HW_VAR_MLME_SITESURVEY,
	HW_VAR_MLME_JOIN,
	HW_VAR_ON_RCR_AM,
	HW_VAR_OFF_RCR_AM,
	HW_VAR_BEACON_INTERVAL,
	HW_VAR_SLOT_TIME,
	HW_VAR_RESP_SIFS,
	HW_VAR_ACK_PREAMBLE,
	HW_VAR_SEC_CFG,  //20
	HW_VAR_SEC_DK_CFG,
	HW_VAR_BCN_VALID,
	HW_VAR_RF_TYPE,
	HW_VAR_DM_FLAG,
	HW_VAR_DM_FUNC_OP,
	HW_VAR_DM_FUNC_SET,
	HW_VAR_DM_FUNC_CLR,
	HW_VAR_CAM_EMPTY_ENTRY,
	HW_VAR_CAM_INVALID_ALL,
	HW_VAR_CAM_WRITE,  //30
	HW_VAR_CAM_READ,
	HW_VAR_AC_PARAM_VO,
	HW_VAR_AC_PARAM_VI,
	HW_VAR_AC_PARAM_BE,
	HW_VAR_AC_PARAM_BK,
	HW_VAR_ACM_CTRL,
	HW_VAR_AMPDU_MIN_SPACE,
	HW_VAR_AMPDU_FACTOR,
	HW_VAR_RXDMA_AGG_PG_TH,
	HW_VAR_SET_RPWM,  //40
	HW_VAR_GET_RPWM,
	HW_VAR_CPWM,
	HW_VAR_H2C_FW_PWRMODE,
	HW_VAR_H2C_PS_TUNE_PARAM,
	HW_VAR_H2C_FW_JOINBSSRPT,
	HW_VAR_FWLPS_RF_ON,
	HW_VAR_H2C_FW_P2P_PS_OFFLOAD,
	HW_VAR_TDLS_WRCR,
	HW_VAR_TDLS_INIT_CH_SEN,
	HW_VAR_TDLS_RS_RCR,  //50
	HW_VAR_TDLS_DONE_CH_SEN,
	HW_VAR_INITIAL_GAIN,  //52
#ifdef CONFIG_LPS_PG
	HW_VAR_LPS_PG_HANDLE,
#endif
	HW_VAR_TRIGGER_GPIO_0,
#if !defined(NOT_SUPPORT_BT)
	HW_VAR_BT_SET_COEXIST,
	HW_VAR_BT_ISSUE_DELBA,	
#endif
	HW_VAR_CURRENT_ANTENNA,
	HW_VAR_ANTENNA_DIVERSITY_LINK,
	HW_VAR_ANTENNA_DIVERSITY_SELECT,
	HW_VAR_SWITCH_EPHY_WoWLAN,
	HW_VAR_EFUSE_USAGE,
	HW_VAR_EFUSE_BYTES,
#if defined(CONFIG_BT_EFUSE)
	HW_VAR_EFUSE_BT_USAGE,
	HW_VAR_EFUSE_BT_BYTES,
#endif
	HW_VAR_FIFO_CLEARN_UP,
	HW_VAR_RESTORE_HW_SEQ,
	HW_VAR_HCI_SUS_STATE,
	HW_VAR_CHECK_TXBUF,   //63
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
	HW_VAR_CHK_HI_QUEUE_EMPTY,  //70
	HW_VAR_DL_BCN_SEL,
	HW_VAR_PORT_SWITCH,
	HW_VAR_DO_IQK,
	HW_VAR_DM_IN_LPS,
	HW_VAR_SET_REQ_FW_PS,
	HW_VAR_FW_PS_STATE,
	HW_VAR_DL_RSVD_PAGE,
	HW_VAR_MACID_SLEEP,
	HW_VAR_MACID_WAKEUP,
	HW_VAR_DUMP_MAC_QUEUE_INFO,  //80
	HW_VAR_ASIX_IOT,
#ifdef CONFIG_PROMISC
	HW_VAR_PROMISC,
#if CONFIG_UNSUPPORT_PLCPHDR_RPT
	HW_VAR_H2C_UNSUPPORT_PLCPHDR_RPT,
#endif
#endif
	HW_VAR_SET_ICV,
#ifdef CONFIG_MCC_MODE
	HW_VAR_TSF_AUTO_SYNC,
	HW_VAR_GET_TSF,
#endif
}HW_VARIABLES;

typedef enum _HAL_DEF_VARIABLE{
	HAL_DEF_UNDERCORATEDSMOOTHEDPWDB,  //0
	HAL_DEF_IS_SUPPORT_ANT_DIV,
	HAL_DEF_CURRENT_ANTENNA,
	HAL_DEF_DRVINFO_SZ,
	HAL_DEF_MAX_RECVBUF_SZ,
	HAL_DEF_RX_PACKET_OFFSET,      //5
	HAL_DEF_DBG_DUMP_RXPKT,//for dbg
	HAL_DEF_RX_DMA_SZ_WOW,
	HAL_DEF_RX_DMA_SZ,
	HAL_DEF_RX_PAGE_SIZE,
	HAL_DEF_DBG_DM_FUNC,//for dbg    //10
	HAL_DEF_RA_DECISION_RATE,
	HAL_DEF_RA_SGI,
	HAL_DEF_PT_PWR_STATUS,
	HAL_DEF_TX_LDPC,				/* LDPC support */
	HAL_DEF_RX_LDPC,				/* LDPC support */
	HAL_DEF_TX_STBC,				/* TX STBC support */
	HAL_DEF_RX_STBC, 				// RX STBC support
	HAL_DEF_EXPLICIT_BEAMFORMER,/* Explicit  Compressed Steering Capable */
	HAL_DEF_EXPLICIT_BEAMFORMEE,/* Explicit Compressed Beamforming Feedback Capable */
	HW_VAR_MAX_RX_AMPDU_FACTOR,     //20
	HW_DEF_RA_INFO_DUMP,
	HAL_DEF_DBG_DUMP_TXPKT,
	HW_DEF_ODM_DBG_FLAG,
	HW_DEF_ODM_DBG_LEVEL,
	HAL_DEF_TX_PAGE_SIZE,
	HAL_DEF_TX_PAGE_BOUNDARY,
	HAL_DEF_MACID_SLEEP, // Support for MACID sleep
	HAL_DEF_DBG_RX_INFO_DUMP,
}HAL_DEF_VARIABLE;

typedef enum _HAL_ODM_VARIABLE{
	HAL_ODM_STA_INFO,	
//	HAL_ODM_P2P_STATE,
//	HAL_ODM_WIFI_DISPLAY_STATE,
	HAL_ODM_DBG_FLAG,
//	HAL_ODM_DBG_LEVEL,
	HAL_ODM_RX_INFO_DUMP,
	HAL_ODM_NOISE_MONITOR,
	HAL_ODM_REGULATION,
}HAL_ODM_VARIABLE;

typedef enum _HAL_INTF_PS_FUNC{
	HAL_USB_SELECT_SUSPEND,
	HAL_MAX_ID,
}HAL_INTF_PS_FUNC;

#define C2H_TYPE_REG 0
#define C2H_TYPE_PKT 1

/* 
* C2H event format:
* Fields    TRIGGER    PAYLOAD    SEQ    PLEN    ID
* BITS     [127:120]    [119:16]   [15:8]  [7:4]  [3:0]
*/
#define C2H_ID(_c2h)		LE_BITS_TO_1BYTE(((u8*)(_c2h)), 0, 4)
#define C2H_PLEN(_c2h)		LE_BITS_TO_1BYTE(((u8*)(_c2h)), 4, 4)
#define C2H_SEQ(_c2h)		LE_BITS_TO_1BYTE(((u8*)(_c2h)) + 1, 0, 8)
#define C2H_PAYLOAD(_c2h)	(((u8*)(_c2h)) + 2)

#define SET_C2H_ID(_c2h, _val)		SET_BITS_TO_LE_1BYTE(((u8*)(_c2h)), 0, 4, _val)
#define SET_C2H_PLEN(_c2h, _val)	SET_BITS_TO_LE_1BYTE(((u8*)(_c2h)), 4, 4, _val)
#define SET_C2H_SEQ(_c2h, _val)		SET_BITS_TO_LE_1BYTE(((u8*)(_c2h)) + 1 , 0, 8, _val)

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

#define SET_C2H_ID_88XX(_c2h, _val)		SET_BITS_TO_LE_1BYTE(((u8*)(_c2h)), 0, 8, _val)
#define SET_C2H_SEQ_88XX(_c2h, _val)	SET_BITS_TO_LE_1BYTE(((u8*)(_c2h)) + 1, 0, 8, _val)
#define SET_C2H_PLEN_88XX(_c2h, _val)	SET_BITS_TO_LE_1BYTE(((u8*)(_c2h)) + 14, 0, 8, _val)

typedef s32 (*c2h_id_filter)(u8 *c2h_evt);

struct hal_ops {
	u32	(*hal_power_on)(_adapter *padapter);
	void	(*hal_power_off)(_adapter *padapter);
	u32	(*hal_init)(_adapter *padapter);
	u32	(*hal_deinit)(_adapter *padapter);

	void	(*free_hal_data)(_adapter *padapter);

	u32	(*inirp_init)(_adapter *padapter);
	u32	(*inirp_deinit)(_adapter *padapter);
	void	(*irp_reset)(_adapter *padapter);

	s32	(*init_xmit_priv)(_adapter *padapter);
	void	(*free_xmit_priv)(_adapter *padapter);
	
	s32	(*init_recv_priv)(_adapter *padapter);
	void	(*free_recv_priv)(_adapter *padapter);

	void (*update_txdesc)(struct xmit_frame *pxmitframe, u8 *pbuf);

	void	(*InitSwLeds)(_adapter *padapter);
	void	(*DeInitSwLeds)(_adapter *padapter);

	void	(*dm_init)(_adapter *padapter);
	void	(*dm_deinit)(_adapter *padapter);
	void	(*read_chip_version)(_adapter *padapter);

	void	(*init_default_value)(_adapter *padapter);

	void	(*intf_chip_configure)(_adapter *padapter);

	void	(*read_adapter_info)(_adapter *padapter);

	void	(*enable_interrupt)(_adapter *padapter);
	void	(*disable_interrupt)(_adapter *padapter);

	s32		(*interrupt_handler)(_adapter *padapter);
	void	(*clear_interrupt)(_adapter *padapter);
	
#ifdef CONFIG_WOWLAN
	void	(*disable_interrupt_but_cpwm2)(_adapter *padapter);
	void (*hal_set_wowlan_fw)(_adapter *adapter, u8 sleep);
#endif //CONFIG_WOWLAN

	void	(*set_bwmode_handler)(_adapter *padapter, enum channel_width Bandwidth, u8 Offset);
	void	(*set_channel_handler)(_adapter *padapter, u8 channel);
	void	(*set_chnl_bw_handler)(_adapter *padapter, u8 channel, enum channel_width Bandwidth, u8 Offset40, u8 Offset80);

	void	(*set_tx_power_level_handler)(_adapter *padapter, u8 channel);

	void	(*hal_dm_watchdog)(_adapter *padapter);
#if 0
	void	(*hal_dm_watchdog_in_lps)(_adapter *padapter);
#endif
	void	(*SetHwRegHandler)(_adapter *padapter, u8	variable,u8* val);
	void	(*GetHwRegHandler)(_adapter *padapter, u8	variable,u8* val);

	u8	(*GetHalDefVarHandler)(_adapter *padapter, HAL_DEF_VARIABLE eVariable, PVOID pValue);
	u8	(*SetHalDefVarHandler)(_adapter *padapter, HAL_DEF_VARIABLE eVariable, PVOID pValue);

	void	(*GetHalODMVarHandler)(_adapter *padapter, HAL_ODM_VARIABLE eVariable, PVOID pValue1,PVOID pValue2);
	void	(*SetHalODMVarHandler)(_adapter *padapter, HAL_ODM_VARIABLE eVariable, PVOID pValue1,BOOLEAN bSet);

	void	(*UpdateRAMaskHandler)(_adapter *padapter, u32 mac_id, u8 rssi_level);

#if 0
	void	(*SetBeaconRelatedRegistersHandler)(_adapter *padapter);
#endif

	void	(*Add_RateATid)(_adapter *padapter, u32 bitmap, u8 *arg, u8 rssi_level);
#ifdef CONFIG_CONCURRENT_MODE	
	void	(*clone_haldata)(_adapter *dst_padapter, _adapter *src_padapter);
#endif

	void	(*run_thread)(_adapter *padapter);
	void	(*cancel_thread)(_adapter *padapter);


#ifdef CONFIG_ANTENNA_DIVERSITY
	u8	(*AntDivBeforeLinkHandler)(_adapter *padapter);
	void	(*AntDivCompareHandler)(_adapter *padapter, WLAN_BSSID_EX *dst, WLAN_BSSID_EX *src);
#endif
#if 0
	u8	(*interface_ps_func)(_adapter *padapter,HAL_INTF_PS_FUNC efunc_id, u8* val);
#endif
	s32	(*hal_xmit)(_adapter *padapter, struct xmit_frame *pxmitframe);
	s32 (*mgnt_xmit)(_adapter *padapter, struct xmit_frame *pmgntframe);

	u32	(*read_bbreg)(_adapter *padapter, u32 RegAddr, u32 BitMask);
	void	(*write_bbreg)(_adapter *padapter, u32 RegAddr, u32 BitMask, u32 Data);
	u32	(*read_rfreg)(_adapter *padapter, u32 eRFPath, u32 RegAddr, u32 BitMask);
	void	(*write_rfreg)(_adapter *padapter, u32 eRFPath, u32 RegAddr, u32 BitMask, u32 Data);

#ifdef CONFIG_HOSTAPD_MLME
	s32	(*hostap_mgnt_xmit_entry)(_adapter *padapter, _pkt *pkt);
#endif

	void (*EfusePowerSwitch)(_adapter *padapter, u8 bWrite, u8 PwrState);
	void (*ReadEFuse)(_adapter *padapter, u8 efuseType, u16 _offset, u16 _size_byte, u8 *pbuf, BOOLEAN bPseudoTest);
	void (*EFUSEGetEfuseDefinition)(_adapter *padapter, u8 efuseType, u8 type, void *pOut, BOOLEAN bPseudoTest);
	u16	(*EfuseGetCurrentSize)(_adapter *padapter, u8 efuseType, BOOLEAN bPseudoTest);
	int 	(*Efuse_PgPacketRead)(_adapter *padapter, u8 offset, u8 *data, BOOLEAN bPseudoTest);
	int 	(*Efuse_PgPacketWrite)(_adapter *padapter, u8 offset, u8 word_en, u8 *data, BOOLEAN bPseudoTest);
	u8	(*Efuse_WordEnableDataWrite)(_adapter *padapter, u16 efuse_addr, u8 word_en, u8 *data, BOOLEAN bPseudoTest);
#if !defined(NOT_SUPPORT_BT)
	void		(*BTEfusePowerSwitch)(_adapter *padapter, u8 bWrite, u8 PwrState);
	BOOLEAN	(*Efuse_PgPacketWrite_BT)(_adapter *padapter, u8 offset, u8 word_en, u8 *data, BOOLEAN bPseudoTest);
#endif
	
#ifdef DBG_CONFIG_ERROR_DETECT
	void (*sreset_init_value)(_adapter *padapter);
	void (*sreset_reset_value)(_adapter *padapter);		
	void (*silentreset)(_adapter *padapter);
	void (*sreset_xmit_status_check)(_adapter *padapter);
	void (*sreset_linked_status_check) (_adapter *padapter);
	u8 (*sreset_get_wifi_status)(_adapter *padapter);
#endif

#ifdef CONFIG_IOL
	int (*IOL_exec_cmds_sync)(_adapter *padapter, struct xmit_frame *xmit_frame, u32 max_wating_ms);
#endif

#if 0//def CONFIG_XMIT_THREAD_MODE
	s32 (*xmit_thread_handler)(_adapter *padapter);
#endif
	void (*recv_tasklet) (void *padapter);

#if (RTW_NOTCH_FILTER != 0)
	void (*hal_notch_filter)(_adapter * adapter, bool enable);
#endif
#if 0
	void (*hal_reset_security_engine)(_adapter * adapter);
#endif	
#ifdef RTW_HALMAC
	void (*hal_mac_c2h_handler)(_adapter *adapter, u8 *pbuf, u16 length);
	u8(*init_mac_register)(PADAPTER);
	u8(*init_phy)(PADAPTER);
#else
	u8 (*c2h_handler)(_adapter *adapter, u8 id, u8 seq, u8 plen, u8 *payload);
#endif
	s32 (*fill_h2c_cmd)(PADAPTER, u8 ElementID, u32 CmdLen, u8 *pCmdBuffer);
	void (*fill_fake_txdesc)(PADAPTER, u8 *pDesc, u32 BufferLen,
			u8 IsPsPoll, u8 IsBTQosNull, u8 bDataFrame);
	u8 (*hal_get_tx_buff_rsvd_page_num)(_adapter *adapter, bool wowlan);
};

typedef	enum _RT_EEPROM_TYPE{
	EEPROM_93C46,
	EEPROM_93C56,
	EEPROM_BOOT_EFUSE,
}RT_EEPROM_TYPE,*PRT_EEPROM_TYPE;

#define USB_HIGH_SPEED_BULK_SIZE	512
#define USB_FULL_SPEED_BULK_SIZE	64

#define RF_CHANGE_BY_INIT	0
#define RF_CHANGE_BY_IPS 	BIT28
#define RF_CHANGE_BY_PS 	BIT29
#define RF_CHANGE_BY_HW 	BIT30
#define RF_CHANGE_BY_SW 	BIT31

typedef enum _HARDWARE_TYPE{
	HARDWARE_TYPE_RTL8180,
	HARDWARE_TYPE_RTL8185,
	HARDWARE_TYPE_RTL8187,
	HARDWARE_TYPE_RTL8188,
	HARDWARE_TYPE_RTL8190P,
	HARDWARE_TYPE_RTL8192E,
	HARDWARE_TYPE_RTL819xU,
	HARDWARE_TYPE_RTL8192SE,
	HARDWARE_TYPE_RTL8192SU,
	HARDWARE_TYPE_RTL8192CE,
	HARDWARE_TYPE_RTL8192CU,
	HARDWARE_TYPE_RTL8192DE,
	HARDWARE_TYPE_RTL8192DU,
	HARDWARE_TYPE_RTL8723AE,
	HARDWARE_TYPE_RTL8723AU,
	HARDWARE_TYPE_RTL8723AS,
	HARDWARE_TYPE_RTL8188EE,
	HARDWARE_TYPE_RTL8188EU,
	HARDWARE_TYPE_RTL8188ES,
//	NEW_GENERATION_IC
	HARDWARE_TYPE_RTL8192EE,
	HARDWARE_TYPE_RTL8192EU,
	HARDWARE_TYPE_RTL8192ES,
	HARDWARE_TYPE_RTL8814AE,
	HARDWARE_TYPE_RTL8814AU,
	HARDWARE_TYPE_RTL8814AS,
	HARDWARE_TYPE_RTL8821BE,
	HARDWARE_TYPE_RTL8821BU,
	HARDWARE_TYPE_RTL8821BS,
	HARDWARE_TYPE_RTL8812E,
	HARDWARE_TYPE_RTL8812AU,
	HARDWARE_TYPE_RTL8811AU,
	HARDWARE_TYPE_RTL8821E,
	HARDWARE_TYPE_RTL8821U,
	HARDWARE_TYPE_RTL8821S,
	HARDWARE_TYPE_RTL8723BE,
	HARDWARE_TYPE_RTL8723BU,
	HARDWARE_TYPE_RTL8723BS,
	HARDWARE_TYPE_RTL8195A,
	HARDWARE_TYPE_RTL8711B,
	HARDWARE_TYPE_RTL8188FE,
	HARDWARE_TYPE_RTL8188FU,
	HARDWARE_TYPE_RTL8188FS,
	HARDWARE_TYPE_RTL8821CE,
	HARDWARE_TYPE_RTL8821CU,
	HARDWARE_TYPE_RTL8821CS,
	HARDWARE_TYPE_RTL8822BE,
	HARDWARE_TYPE_RTL8822BU,
	HARDWARE_TYPE_RTL8822BS,
	HARDWARE_TYPE_RTL8703BE,
	HARDWARE_TYPE_RTL8703BU,
	HARDWARE_TYPE_RTL8703BS,
	HARDWARE_TYPE_RTL8723DE,
	HARDWARE_TYPE_RTL8723DU,
	HARDWARE_TYPE_RTL8723DS,
	HARDWARE_TYPE_RTL8721D,
	HARDWARE_TYPE_RTL8195B,
	HARDWARE_TYPE_RTL8710C,
	HARDWARE_TYPE_RTL8192FE,
	HARDWARE_TYPE_RTL8192FU,
	HARDWARE_TYPE_RTL8192FS,
	HARDWARE_TYPE_MAX,
}HARDWARE_TYPE;

#define IS_NEW_GENERATION_IC(_Adapter)	(((_adapter *)_Adapter)->HardwareType >=HARDWARE_TYPE_RTL8192EE)
//
// RTL8188E Series
//
#define IS_HARDWARE_TYPE_8188EE(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8188EE)
#define IS_HARDWARE_TYPE_8188EU(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8188EU)
#define IS_HARDWARE_TYPE_8188ES(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8188ES)
#define	IS_HARDWARE_TYPE_8188E(_Adapter)	\
(IS_HARDWARE_TYPE_8188EE(_Adapter) || IS_HARDWARE_TYPE_8188EU(_Adapter) || IS_HARDWARE_TYPE_8188ES(_Adapter))
//
//RTL8188F Series
//
#define IS_HARDWARE_TYPE_8188FE(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8188FE)
#define IS_HARDWARE_TYPE_8188FU(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8188EU)
#define IS_HARDWARE_TYPE_8188FS(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8188FS)
#define IS_HARDWARE_TYPE_8188F(_Adapter)	\
(IS_HARDWARE_TYPE_8188FE(_Adapter) || IS_HARDWARE_TYPE_8188FU(_Adapter) || IS_HARDWARE_TYPE_8188FS(_Adapter))

/* RTL8703B Series */
#define IS_HARDWARE_TYPE_8703BE(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8703BE)
#define IS_HARDWARE_TYPE_8703BS(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8703BS)
#define IS_HARDWARE_TYPE_8703BU(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8703BU)
#define IS_HARDWARE_TYPE_8703B(_Adapter)			\
	(IS_HARDWARE_TYPE_8703BE(_Adapter) || IS_HARDWARE_TYPE_8703BU(_Adapter) || IS_HARDWARE_TYPE_8703BS(_Adapter))

/* RTL8192F Series */
#define IS_HARDWARE_TYPE_8192FS(_Adapter)\
	(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8192FS)
#define IS_HARDWARE_TYPE_8192FU(_Adapter)\
	(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8192FU)	
#define IS_HARDWARE_TYPE_8192FE(_Adapter)\
	(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8192FE)	
#define	IS_HARDWARE_TYPE_8192F(_Adapter)\
	(IS_HARDWARE_TYPE_8192FS(_Adapter) ||\
	 IS_HARDWARE_TYPE_8192FU(_Adapter) ||\
	 IS_HARDWARE_TYPE_8192FE(_Adapter))


/* RTL8723D Series */
#define IS_HARDWARE_TYPE_8723DE(_Adapter)\
	(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8723DE)
#define IS_HARDWARE_TYPE_8723DS(_Adapter)\
	(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8723DS)
#define IS_HARDWARE_TYPE_8723DU(_Adapter)\
	(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8723DU)
#define IS_HARDWARE_TYPE_8723D(_Adapter)\
	(IS_HARDWARE_TYPE_8723DE(_Adapter) || \
	 IS_HARDWARE_TYPE_8723DU(_Adapter) || \
	 IS_HARDWARE_TYPE_8723DS(_Adapter))
//
// RTL8812 Series
//
#define IS_HARDWARE_TYPE_8812E(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8812E)
#define IS_HARDWARE_TYPE_8812AU(_Adapter)	(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8812AU)
#define IS_HARDWARE_TYPE_8812(_Adapter)			\
(IS_HARDWARE_TYPE_8812E(_Adapter) || IS_HARDWARE_TYPE_8812AU(_Adapter))

// RTL8821 Series
#define IS_HARDWARE_TYPE_8821E(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8821E)
#define IS_HARDWARE_TYPE_8821U(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8821U ||\
	              								 ((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8811AU)
#define IS_HARDWARE_TYPE_8821S(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8821S)
#define IS_HARDWARE_TYPE_8821(_Adapter)			\
(IS_HARDWARE_TYPE_8821E(_Adapter) || IS_HARDWARE_TYPE_8821U(_Adapter)|| IS_HARDWARE_TYPE_8821S(_Adapter))

#define IS_HARDWARE_TYPE_JAGUAR(_Adapter)		\
(IS_HARDWARE_TYPE_8812(_Adapter) || IS_HARDWARE_TYPE_8821(_Adapter))

/* RTL8814A Series */
#define IS_HARDWARE_TYPE_8814AE(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8814AE)
#define IS_HARDWARE_TYPE_8814AU(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8814AU)
#define IS_HARDWARE_TYPE_8814AS(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8814AS)

#define IS_HARDWARE_TYPE_8814A(_Adapter)		\
(IS_HARDWARE_TYPE_8814AE(_Adapter) || IS_HARDWARE_TYPE_8814AU(_Adapter) || IS_HARDWARE_TYPE_8814AS(_Adapter))

#define IS_HARDWARE_TYPE_8821BE(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8821BE)
#define IS_HARDWARE_TYPE_8821BU(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8821BU)
#define IS_HARDWARE_TYPE_8821BS(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8821BS)

#define IS_HARDWARE_TYPE_8821B(_Adapter)		\
	(IS_HARDWARE_TYPE_8821BE(_Adapter) || IS_HARDWARE_TYPE_8821BU(_Adapter) || IS_HARDWARE_TYPE_8821BS(_Adapter))

#define IS_HARDWARE_TYPE_8195B(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8195B)

#define IS_HARDWARE_TYPE_8710C(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8710C)

#define IS_HARDWARE_TYPE_JAGUAR2(_Adapter)		\
(IS_HARDWARE_TYPE_8814A(_Adapter) || IS_HARDWARE_TYPE_8821B(_Adapter) || IS_HARDWARE_TYPE_8821C(_Adapter) || IS_HARDWARE_TYPE_8195B(_Adapter))

#define IS_HARDWARE_TYPE_JAGUAR_AND_JAGUAR2(_Adapter)		\
(IS_HARDWARE_TYPE_JAGUAR(_Adapter) || IS_HARDWARE_TYPE_JAGUAR2(_Adapter))

//RTL8192E Series
#define IS_HARDWARE_TYPE_8192EE(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8192EE)
#define IS_HARDWARE_TYPE_8192EU(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8192EU)
#define IS_HARDWARE_TYPE_8192ES(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8192ES)

#define IS_HARDWARE_TYPE_8192E(_Adapter)		\
(IS_HARDWARE_TYPE_8192EE(_Adapter) || IS_HARDWARE_TYPE_8192EU(_Adapter) ||IS_HARDWARE_TYPE_8192ES(_Adapter))

#define IS_HARDWARE_TYPE_8723BE(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8723BE)
#define IS_HARDWARE_TYPE_8723BU(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8723BU)
#define IS_HARDWARE_TYPE_8723BS(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8723BS)


#define IS_HARDWARE_TYPE_8723B(_Adapter) \
	(IS_HARDWARE_TYPE_8723BE(_Adapter) || IS_HARDWARE_TYPE_8723BU(_Adapter) ||IS_HARDWARE_TYPE_8723BS(_Adapter))

#define IS_HARDWARE_TYPE_8821CE(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8821CE)
#define IS_HARDWARE_TYPE_8821CU(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8821CU)
#define IS_HARDWARE_TYPE_8821CS(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8821CS)
#define IS_HARDWARE_TYPE_8821C(_Adapter)		\
	(IS_HARDWARE_TYPE_8821CE(_Adapter) || IS_HARDWARE_TYPE_8821CU(_Adapter) || IS_HARDWARE_TYPE_8821CS(_Adapter))

//8822B Series
#define IS_HARDWARE_TYPE_8822BE(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8822BE)
#define IS_HARDWARE_TYPE_8822BU(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8822BU)
#define IS_HARDWARE_TYPE_8822BS(_Adapter)		(((_adapter *)_Adapter)->HardwareType == HARDWARE_TYPE_RTL8822BS)
#define IS_HARDWARE_TYPE_8822B(_Adapter)		\
	(IS_HARDWARE_TYPE_8822BE(_Adapter) || IS_HARDWARE_TYPE_8822BU(_Adapter) || IS_HARDWARE_TYPE_8822BS(_Adapter))


#define IS_HARDWARE_TYPE_8195A(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8195A)

#define IS_HARDWARE_TYPE_8711B(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8711B)

#define IS_HARDWARE_TYPE_8721D(_Adapter)		(((_adapter *)_Adapter)->HardwareType==HARDWARE_TYPE_RTL8721D)

typedef struct eeprom_priv EEPROM_EFUSE_PRIV, *PEEPROM_EFUSE_PRIV;
#define GET_EEPROM_EFUSE_PRIV(adapter) (&adapter->eeprompriv)
#define is_boot_from_eeprom(adapter) (adapter->eeprompriv.EepromOrEfuse)

//TODO

#ifdef CONFIG_WOWLAN

#define RTW_WOWLAN_MAX_RX_FILTERS		(5)
#define RTW_WOWLAN_MAX_PATTERN_SIZE		(128)
#define RTW_WOWLAN_MAX_MASK_SIZE		(16)

typedef enum _wowlan_subcode{
	WOWLAN_PATTERN_MATCH	= 1,
	WOWLAN_MAGIC_PACKET		= 2,
	WOWLAN_UNICAST			= 3,
	WOWLAN_SET_PATTERN		= 4,
	WOWLAN_DUMP_REG			= 5,
	WOWLAN_ENABLE			= 6,
	WOWLAN_DISABLE			= 7,
	WOWLAN_STATUS			= 8,
	WOWLAN_DEBUG_RELOAD_FW	= 9,
	WOWLAN_DEBUG_1			=10,
	WOWLAN_DEBUG_2			=11
}wowlan_subcode;

struct wowlan_ioctl_param{
	unsigned int subcode;
	unsigned int subcode_value;
	unsigned int wakeup_reason;
	unsigned int len;
	wowlan_pattern_t pattern;
};
/*
typedef struct _wowlan_pattern {
	int len;
	char content[RTW_WOWLAN_MAX_PATTERN_SIZE];
	char mask[RTW_WOWLAN_MAX_MASK_SIZE];
} wowlan_pattern_t;
*/
#define Rx_Pairwisekey			0x01
#define Rx_GTK					0x02
#define Rx_DisAssoc				0x04
#define Rx_DeAuth				0x08
#define FWDecisionDisconnect	0x10
#define Rx_MagicPkt				0x21
#define Rx_UnicastPkt			0x22
#define Rx_PatternPkt			0x23
#endif // CONFIG_WOWLAN

void rtw_hal_def_value_init(_adapter *padapter);

void	rtw_hal_free_data(_adapter *padapter);

void rtw_hal_dm_init(_adapter *padapter);
void rtw_hal_dm_deinit(_adapter *padapter);
#if 0
void rtw_hal_sw_led_init(_adapter *padapter);
void rtw_hal_sw_led_deinit(_adapter *padapter);
#endif
u32 rtw_hal_power_on(_adapter *padapter);
void rtw_hal_power_off(_adapter *padapter);
uint rtw_hal_init(_adapter *padapter);
uint rtw_hal_deinit(_adapter *padapter);
void rtw_hal_stop(_adapter *padapter);
void rtw_hal_set_hwreg(PADAPTER padapter, u8 variable, u8 *val);
void rtw_hal_get_hwreg(PADAPTER padapter, u8 variable, u8 *val);

void rtw_hal_chip_configure(_adapter *padapter);
void rtw_hal_read_chip_info(_adapter *padapter);
void rtw_hal_read_chip_version(_adapter *padapter);

u8 rtw_hal_set_def_var(_adapter *padapter, HAL_DEF_VARIABLE eVariable, PVOID pValue);
u8 rtw_hal_get_def_var(_adapter *padapter, HAL_DEF_VARIABLE eVariable, PVOID pValue);

void rtw_hal_set_odm_var(_adapter *padapter, HAL_ODM_VARIABLE eVariable, PVOID pValue1, BOOLEAN bSet);
void rtw_hal_get_odm_var(_adapter *padapter, HAL_ODM_VARIABLE eVariable, PVOID pValue1, PVOID pValue2);
	
void rtw_hal_enable_interrupt(_adapter *padapter);
void rtw_hal_disable_interrupt(_adapter *padapter);
void rtw_hal_clear_interrupt(_adapter *padapter);
#ifdef CONFIG_WOWLAN
void rtw_hal_disable_interrupt_but_cpwm2(_adapter *padapter);
#endif

u32	rtw_hal_inirp_init(_adapter *padapter);
u32	rtw_hal_inirp_deinit(_adapter *padapter);

void	rtw_hal_irp_reset(_adapter *padapter);
#if 0
u8	rtw_hal_intf_ps_func(_adapter *padapter,HAL_INTF_PS_FUNC efunc_id, u8* val);
#endif
s32	rtw_hal_xmit(_adapter *padapter, struct xmit_frame *pxmitframe);
s32	rtw_hal_mgnt_xmit(_adapter *padapter, struct xmit_frame *pmgntframe);

s32	rtw_hal_init_xmit_priv(_adapter *padapter);
void	rtw_hal_free_xmit_priv(_adapter *padapter);

#if 1
s32	rtw_hal_init_recv_priv(_adapter *padapter);
void	rtw_hal_free_recv_priv(_adapter *padapter);
#endif

void rtw_hal_update_ra_mask(struct sta_info *psta, u8 rssi_level);
void	rtw_hal_add_ra_tid(_adapter *padapter, u32 bitmap, u8 *arg, u8 rssi_level);
void	rtw_hal_clone_data(_adapter *dst_padapter, _adapter *src_padapter);
#ifdef CONFIG_LITTLE_WIFI_MCU_FUNCTION_THREAD
void	rtw_hal_start_thread(_adapter *padapter);
void	rtw_hal_stop_thread(_adapter *padapter);
#endif
void rtw_hal_bcn_related_reg_setting(_adapter *padapter);

u32	rtw_hal_read_bbreg(_adapter *padapter, u32 RegAddr, u32 BitMask);
void	rtw_hal_write_bbreg(_adapter *padapter, u32 RegAddr, u32 BitMask, u32 Data);
u32	rtw_hal_read_rfreg(_adapter *padapter, u32 eRFPath, u32 RegAddr, u32 BitMask);
void	rtw_hal_write_rfreg(_adapter *padapter, u32 eRFPath, u32 RegAddr, u32 BitMask, u32 Data);

s32	rtw_hal_interrupt_handler(_adapter *padapter);

void	rtw_hal_set_bwmode(_adapter *padapter, enum channel_width Bandwidth, u8 Offset);
void	rtw_hal_set_chan(_adapter *padapter, u8 channel);
void	rtw_hal_set_chnl_bw(_adapter *padapter, u8 channel, enum channel_width Bandwidth, u8 Offset40, u8 Offset80);
void	rtw_hal_dm_watchdog(_adapter *padapter);
#if 1
void rtw_hal_update_txdesc(_adapter  *padapter, struct xmit_frame *pxmitframe, u8 *pbuf);
#endif
s32 rtw_hal_fill_h2c_cmd(PADAPTER padapter, u8 ElementID, u32 CmdLen, u8 *pCmdBuffer);

void rtw_hal_fill_fake_txdesc(_adapter* padapter, u8* pDesc, u32 BufferLen,
		u8 IsPsPoll, u8 IsBTQosNull, u8 bDataFrame);

u8 rtw_hal_get_txbuff_rsvd_page_num(_adapter *padapter, bool wowlan);


void rtw_hal_set_wowlan_fw(_adapter *padapter, u8 sleep);


c2h_id_filter rtw_hal_c2h_id_filter_ccx(_adapter *padapter);

u8  rtw_hal_c2h_handler(_adapter *padapter, u8 id, u8 seq, u8 plen, u8 *payload);


#ifdef CONFIG_ANTENNA_DIVERSITY
u8	rtw_hal_antdiv_before_linked(_adapter *padapter);
void	rtw_hal_antdiv_rssi_compared(_adapter *padapter, WLAN_BSSID_EX *dst, WLAN_BSSID_EX *src);
#endif

#ifdef CONFIG_HOSTAPD_MLME
s32	rtw_hal_hostap_mgnt_xmit_entry(_adapter *padapter, _pkt *pkt);
#endif

#ifdef DBG_CONFIG_ERROR_DETECT
void rtw_hal_sreset_init(_adapter *padapter);
void rtw_hal_sreset_reset(_adapter *padapter);	
void rtw_hal_sreset_reset_value(_adapter *padapter);
void rtw_hal_sreset_xmit_status_check(_adapter *padapter);
void rtw_hal_sreset_linked_status_check (_adapter *padapter);
u8   rtw_hal_sreset_get_wifi_status(_adapter *padapter);
#endif

#ifdef CONFIG_IOL
int rtw_hal_iol_cmd(ADAPTER *adapter, struct xmit_frame *xmit_frame, u32 max_wating_ms, u32 bndy_cnt)
#endif

#if 0//def CONFIG_XMIT_THREAD_MODE
s32 rtw_hal_xmit_thread_handler(_adapter *padapter);
#endif

s32	rtw_hal_recv_tasklet(_adapter *padapter);
#if (RTW_NOTCH_FILTER != 0)
void rtw_hal_notch_filter(_adapter * adapter, bool enable);
#endif
#if 0
void rtw_hal_reset_security_engine(_adapter * adapter);
#endif

void decide_chip_type_by_device_id(_adapter *padapter);

s32 rtw_hal_macid_wakeup(PADAPTER padapter, u8 macid);
s32 rtw_hal_macid_sleep(PADAPTER padapter, u8 macid);

#ifdef CONFIG_RTL8188E
u32 rtl8188e_set_hal_ops(PADAPTER padapter);
#define hal_set_hal_ops(__adapter) rtl8188e_set_hal_ops(__adapter)
#endif

#ifdef CONFIG_RTL8188F
u32 rtl8188fs_set_hal_ops(PADAPTER padapter);
#define hal_set_hal_ops(__adapter) rtl8188fs_set_hal_ops(__adapter)
#endif

#ifdef CONFIG_RTL8192E
u32 rtl8192es_set_hal_ops(PADAPTER padapter);
#define hal_set_hal_ops(__adapter) rtl8192es_set_hal_ops(__adapter)
#endif

#ifdef CONFIG_RTL8703B
u32 rtl8703bs_set_hal_ops(PADAPTER padapter);
#define hal_set_hal_ops(__adapter) rtl8703bs_set_hal_ops(__adapter)
#endif

#ifdef CONFIG_RTL8723B
u32 rtl8723bs_set_hal_ops(PADAPTER padapter);
#define hal_set_hal_ops(__adapter) rtl8723bs_set_hal_ops(__adapter)
#endif

#ifdef CONFIG_RTL8723D
	#ifdef CONFIG_SDIO_HCI
		u32 rtl8723ds_set_hal_ops(PADAPTER padapter);
		#define hal_set_hal_ops(__adapter) rtl8723ds_set_hal_ops(__adapter)
	#elif  defined (CONFIG_USB_HCI)
		u32 rtl8723du_set_hal_ops(PADAPTER padapter);
		#define hal_set_hal_ops(__adapter) rtl8723du_set_hal_ops(__adapter)
	#endif
#endif

#ifdef CONFIG_RTL8195A
u32 rtl8195ab_set_hal_ops(_adapter * padapter);
#define hal_set_hal_ops rtl8195ab_set_hal_ops
#define hal_interuupt_recognized InterruptRecognized8195a
#elif defined(CONFIG_RTL8711B)
u32 rtl8711bb_set_hal_ops(_adapter * padapter);
#define hal_set_hal_ops rtl8711bb_set_hal_ops
#define hal_interuupt_recognized InterruptRecognized8711b
#elif defined(CONFIG_RTL8721D)
u32 rtl8721dd_set_hal_ops(_adapter * padapter);
#define hal_set_hal_ops rtl8721dd_set_hal_ops
#define hal_interuupt_recognized InterruptRecognized8721d
#elif defined(CONFIG_RTL8195B)
void rtl8195b_set_hal_ops(PADAPTER);
u8 rtl8195ba_set_hal_ops(PADAPTER);
#define hal_set_hal_ops(__adapter) rtl8195ba_set_hal_ops(__adapter)
#elif defined(CONFIG_RTL8710C)
void rtl8710c_set_hal_ops(PADAPTER);
u8 rtl8710ca_set_hal_ops(PADAPTER);
#define hal_set_hal_ops(__adapter) rtl8710ca_set_hal_ops(__adapter)
#endif
#ifdef CONFIG_RTL8821C
void rtl8821c_set_hal_ops(PADAPTER);
u8 rtl8821cs_set_hal_ops(PADAPTER);
#define hal_set_hal_ops(__adapter) rtl8821cs_set_hal_ops(__adapter)
#endif

#ifdef CONFIG_RTL8192F
	#ifdef CONFIG_SDIO_HCI
		u32 rtl8192fs_set_hal_ops(PADAPTER padapter);
		#define hal_set_hal_ops(__adapter) rtl8192fs_set_hal_ops(__adapter)
	#elif defined(CONFIG_USB_HCI)
		u32 rtl8192fu_set_hal_ops(PADAPTER padapter);
		#define hal_set_hal_ops(__adapter) rtl8192fu_set_hal_ops(__adapter)
	#endif
#endif

#endif //__HAL_INTF_H__

