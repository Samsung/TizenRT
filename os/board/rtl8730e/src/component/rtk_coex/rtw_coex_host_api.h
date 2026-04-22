/******************************************************************************
 *
 * Copyright(c) 2016 - 2017 Realtek Corporation.
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
 *****************************************************************************/
#ifndef RTW_COEX_HOST_API_H
#define RTW_COEX_HOST_API_H

/**
 * @defgroup  COEX_APIs
 * @brief     COEX_API module
 * @{
 */

#include "ameba.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup COEX_Exported_Types COEX Exported Types
 * @{
 */

#define TYPE_BITS   (8) // BIT15~BIT8
#define SUBTYPE_BITS (8) // BIT7~BIT0
#define TOTALTYPE_BITS (TYPE_BITS + SUBTYPE_BITS)

#define COEX_BT_TYPE(x)     (((RTK_COEX_TYPE_BT << SUBTYPE_BITS) + ((x) & ((1 << SUBTYPE_BITS) - 1))) & ((1 << TOTALTYPE_BITS) - 1))
#define COEX_EXT_TYPE(x)    (((RTK_COEX_TYPE_EXT << SUBTYPE_BITS) + ((x) & ((1 << SUBTYPE_BITS) - 1))) & ((1 << TOTALTYPE_BITS) - 1))
#define COEX_WP_TYPE(x)     (((RTK_COEX_TYPE_WPAN << SUBTYPE_BITS) + ((x) & ((1 << SUBTYPE_BITS) - 1))) & ((1 << TOTALTYPE_BITS) - 1))
#define COEX_COM_TYPE(x)	(((RTK_COEX_TYPE_COM << SUBTYPE_BITS) + ((x) & ((1 << SUBTYPE_BITS) - 1))) & ((1 << TOTALTYPE_BITS) - 1))

#define COEX_TYPE_GET(x)    ((x & ((1 << TOTALTYPE_BITS) - 1)) >> SUBTYPE_BITS)
#define COEX_SUBTYPE_GET(x) ((x & ((1 << TOTALTYPE_BITS) - 1)) & ((1 << SUBTYPE_BITS) - 1))
/**
 * @brief  The enumeration lists main type for ipc send.
 */
enum coex_type {
	RTK_COEX_TYPE_BT = 0,
	RTK_COEX_TYPE_EXT,
	RTK_COEX_TYPE_WPAN,
	RTK_COEX_TYPE_COM,
	/* end */
	RTK_COEX_TYPE_INVALID = ((1 << TYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (common use).
 */
enum coex_subtype_h2c_com {
	COEX_H2C_COM_UNDEF = 0,
	COEX_H2C_COM_VENDOR_INFO_SET,
	COEX_H2C_COM_WL_SLOT_SET,
	COEX_H2C_COM_STATE_GET,
	COEX_H2C_COM_SET_COEX_ENABLE,
	COEX_H2C_COM_GET_IS_ENABLED,
	/* end */
	COEX_H2C_COM_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (bt use).
 */
enum coex_subtype_h2c_bt {
	COEX_H2C_BT_UNDEF = 0,
	COEX_H2C_BT_HCI_NOTIFY_HCI_EVENT,
	COEX_H2C_BT_HCI_NOTIFY_HCI_CMD,
	COEX_H2C_BT_HCI_NOTIFY_SW_MAILBOX,
	COEX_H2C_BT_RFK,
	COEX_H2C_BT_SET_BT_SEL,
	COEX_H2C_BT_SET_PTA,
	/* end */
	COEX_H2C_BT_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (extchip use).
 */
enum coex_subtype_h2c_ext {
	/* common info */
	COEX_H2C_EXT_UNDEF = 0,
	COEX_H2C_EXT_GET_READY,
	COEX_H2C_EXT_WL_PERFORMANCE_REQUEST,
	/* info for WPAN*/
	COEX_H2C_EXT_WPAN_CHANNEL,
	/* info for BT*/
	COEX_H2C_EXT_BT_PROFILE,
	/* end */
	COEX_H2C_EXT_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (internal wpan use).
 */
enum coex_subtype_h2c_wpan {
	COEX_H2C_WPAN_UNDEF = 0,
	COEX_H2C_WPAN_ZB_RFK,
	COEX_H2C_WPAN_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (common use).
 */
enum coex_subtype_c2h_com {
	COEX_C2H_COM_UNDEF = 0,
	/* end */
	COEX_C2H_COM_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (bt use).
 */
enum coex_subtype_c2h_bt {
	COEX_C2H_BT_UNDEF = 0,
	COEX_C2H_BT_HCI_MSG_MAILBOX_TRIGGER,
	/* end */
	COEX_C2H_BT_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (extchip use).
 */
enum coex_subtype_c2h_ext {
	COEX_C2H_EXT_UNDEF = 0,
	COEX_C2H_EXT_GET_INITPARAMS,
	/* end */
	COEX_C2H_EXT_INVALID = ((1 << SUBTYPE_BITS) - 1),
};
/**
 * @brief  The enumeration lists subtype for ipc h2c send (internal wpan use).
 */
enum coex_subtype_c2h_wpan {
	COEX_C2H_WPAN_UNDEF = 0,
	COEX_C2H_WPAN_INVALID = ((1 << SUBTYPE_BITS) - 1),
};

//////////////////////////////////////////////////////////
///////// for COMMON Variables
//////////////////////////////////////////////////////////
/**
 * @brief  The enumeration lists vendor id.
 */
enum rtk_coex_custom_vendor_id {
	RTK_COEX_CUSTOM_VID_UNDEF = 0,
	/* Add from here*/
	RTK_COEX_CUSTOM_VID_CLINTWOOD,			/* 1 */
	RTK_COEX_CUSTOM_VID_MAX = 0xFF
};
/**
 * @brief  The enumeration lists product id.
 */
enum rtk_coex_custom_product_id {
	RTK_COEX_CUSTOM_PID_UNDEF = 0,
	/* Add from here*/
	RTK_COEX_CUSTOM_PID_GAMEPAD,			/* 1 */
	RTK_COEX_CUSTOM_PID_GAMEPAD_5G_WL_PRI,	/* 2 */
	RTK_COEX_CUSTOM_PID_MAX = 0xFF
};

/**
 * @struct   rtk_coex_vendor_info
 * @brief    vendor info.
 */
struct rtk_coex_vendor_info {
	uint8_t  vendor_id;
	uint8_t  product_id;
};


//////////////////////////////////////////////////////////
///////// for BT Variables
//////////////////////////////////////////////////////////
/**
 * @brief  The enumeration lists pta request module type.
 */
enum pta_host_role {
	PTA_HOST_WIFI			= 0,
	PTA_HOST_BT				= 1
};
/**
 * @brief  The enumeration lists action for pta running.
 */
enum pta_process_action {
	COMMON_ACTION			= 0,
	CALIBRATION_START		= 1,
	CALIBRATION_STOP		= 2
};
/**
 * @brief  The enumeration lists pta winner module type.
 */
enum pta_type {
	PTA_AUTO            = 0,
	PTA_WIFI			= 1,
	PTA_BT				= 2,
	PTA_DISABLE		    = 0xFF,
};
/**
 * @brief  The enumeration lists bt rfk type.
 */
enum bt_rfk_type {
	BT_RX_DCK       = 0,
	BT_LOK          = 1,
	BT_LOK_RES      = 2,
	BT_DAC_DCK      = 3,
	BT_ADC_DCK      = 4,
	BT_DCK          = 5,
	BT_IQK          = 6,
	BT_FLATK        = 7,
};
/**
 * @struct   bt_rfk_param
 * @brief    bt rfk parameter data structure.
 */
struct bt_rfk_param {
	enum bt_rfk_type type;
	uint8_t  rfk_data1;
	uint8_t  rfk_data2;
	uint8_t  rfk_data3;
	uint8_t  rfk_data4;
};
/**
 * @struct   pta_para_t
 * @brief    pta parameter structure.
 */
struct pta_para_t {
	u8 type;
	u8 role;
	u8 process;
};

//////////////////////////////////////////////////////////
///////// for EXT Variables
//////////////////////////////////////////////////////////
/**
 * @brief   The enumeration lists state for extchip.
 */
enum EXT_STATE {
	EXT_DISABLE = 0,
	EXT_ENABLE,
} ;
/**
 * @brief   The enumeration lists protocol for extchip.
 */
enum EXT_PROTOCOL {
	EXT_PTA_PROTOCOL_UNDEF = 0,
	EXT_PTA_PROTOCOL_WPAN,
	EXT_PTA_PROTOCOL_BT,
	EXT_PTA_PROTOCOL_BT_WPAN,
} ;
/**
 * @brief   The enumeration lists wl perfomance request for extchip.
 */
enum EXT_PERFORMANCE {
	EXT_PERF_DEFAULT,
	EXT_PERF_HIGH,
	EXT_PERF_MED,
	EXT_PERF_LOW,
	EXT_PERF_INVALID,
};
/**
 * @brief   The enumeration lists bt profile running in extchip.
 */
enum EXT_BT_PROFILE {
	EXT_BT_UNDEF = 0,
	EXT_BT_SCO = BIT(0),
	EXT_BT_HID = BIT(1),
	EXT_BT_A2DP = BIT(2),
	EXT_BT_HFP = BIT(3),
};
/**
 * @brief   The enumeration lists pta index need for extchip.
 */
enum PTA_INDEX {
	EXT_PTA1 = 0,
	EXT_PTA2 = 1,
	EXT_PTA_INVALID,
};
/**
 * @brief   The enumeration lists pinmux port index.
 */
enum PORT_e {
	PAD_PORT_A = 0,
	PAD_PORT_B = 1,
	PAD_PORT_C = 2,
};
/**
 * @struct   port_pin_t
 * @brief    pinmux port define.
 */
struct port_pin_t {
	u8 port;
	u8 pin;
};

/**
 * @struct   extchip_para_t
 * @brief    pta related parameter structure for extchip.
 */
struct extchip_para_t {
	u8 valid;
	u8 pri_det_time;	///< T1 us
	u8 trx_det_time;	///< T2 us
	u8 pri_mode: 1;		///< 1: static Priority; 0: directional Priority
	u8 req_polar: 1;	///< 1: active high; 0: active low
	u8 gnt_polar: 1;	///< 1: active high; 0: active low
	u8 pta_index: 1;	///< 1: extbt; 0: extwpan
	u8 rsvd : 4;		///< rsvd
	enum EXT_PROTOCOL active_protocol;	///< -> enum EXT_PROTOCOL
	struct port_pin_t port_req;			///< pinmux pad define, port(PORT_A,PORT_B,PORT_C,...) + pin(0...31)
	struct port_pin_t port_pri;			///< pinmux pad define, port(PORT_A,PORT_B,PORT_C,...) + pin(0...31)
	struct port_pin_t port_gnt;			///< pinmux pad define, port(PORT_A,PORT_B,PORT_C,...) + pin(0...31)
};

//////////////////////////////////////////////////////////
///////// for Internal Wpan Variables
//////////////////////////////////////////////////////////
// ADD HERE

/**
 * @}
 */

/**
 * @defgroup COEX_Exported_Functions COEX Exported Functions
 * @{
 */
//////////////////////////////////////////////////////////
///////// for COMMON Function Declare
//////////////////////////////////////////////////////////
/**
 * @brief      set coex enable or disable.
 * @param[in]  enable coex enable status
 * @return
 *             - None.
 */
void rtk_coex_com_coex_set_enable(bool enable);
/**
 * @brief      get if coex is enabled.
 * @return
 *             - true: coex enabled, false: coex disabled.
 */
bool rtk_coex_com_coex_is_enabled(void);
/**
 * @brief      Vendor info set.
 * @param[in]  vendor_id
 * @param[in]  product_id
 * @return
 *             - None.
 */
void rtk_coex_com_vendor_info_set(u8 vendor_id, u8 product_id);
/**
 * @brief      wlan slot duration set.
 * @param[in]  wl_slot  wlan slot duration, unit: percent, value: [0-100].
 * @return
 *             - None.
 */
void rtk_coex_com_wl_slot_set(u8 wl_slot);
/**
 * @brief      coex state get.
 * @return
 *             - None. (print as result)
 */
void rtk_coex_com_state_get(void);

//////////////////////////////////////////////////////////
///////// for BT Function Declare
//////////////////////////////////////////////////////////
/**
 * @brief      Hci notify.
 * @param[in]  pdata  	A pointer to hci notify data.
 * @param[in]  len 		size of hci notify data.
 * @param[in]  type		type for hci notify.
 * @return
 *             - None.
 */
void rtk_coex_btc_bt_hci_notify(u8 *pdata, u16 len, u8 type);
/**
 * @brief      RFK for BT.
 * @param[in]  p_rfk_param  A pointer to struct bt_rfk_param.
 * @param[in]  length 		size of struct bt_rfk_param.
 * @return
 *             - None.
 */
int rtk_coex_btc_bt_rfk(void *p_rfk_param, u16 length);
/**
 * @brief      Set BTS0 or BTS1.
 * @param[in]  bt_ant: the BT ant(BTS0/BTS1).
 * @return
 *             - None.
 */
void rtk_coex_btc_set_bt_ant(u8 bt_ant);
/**
 * @brief      Set PTA.
 * @param[in]  type  refer to enum pta_type.
 * @param[in]  role  refer to enum enum pta_host_role.
 * @param[in]  process refer to enum enum pta_process_action.
 * @return
 *             - None.
 */
void rtk_coex_btc_set_pta(u8 type, u8 role, u8 process);

//////////////////////////////////////////////////////////
///////// for EXT Function Declare
//////////////////////////////////////////////////////////
/**
 * @brief     is extc ready.
 * @return
 *            - true: ready, false: not ready.
 */
bool rtk_coex_extc_is_ready(void);
/**
 * @brief     ext wpan channel notification.
 * @param[in] channel  802.15.4 channel number [11-26].
 * @return
 *            - None.
 */
void rtk_coex_extc_ntfy_wpan_channel(u8 channel);
/**
 * @brief     ext chip req wl performance.
 * @param[in] performance_req  req wl performance level, refer to enum EXT_PERFORMANCE.
 * @return
 *            - None.
 */
void rtk_coex_extc_ntfy_wl_performance_req(u8 performance_req);
/**
 * @brief     ext bt profile notification.
 * @param[in] profile  bt profile, refer to enum EXT_BT_PROFILE.
 * @return
 *            - None.
 */
void rtk_coex_extc_ntfy_bt_profile(u32 profile);

//////////////////////////////////////////////////////////
///////// for Internal wpan Function Declare
//////////////////////////////////////////////////////////
/**
 * @brief     RFK for Internal wpan module.
 * @return
 *            - 0 : if Sucess, else if Fail.
 */
int rtk_coex_wpc_zb_rfk(void);

/**
 * @}
 */

/**
* @}
*/

#ifdef __cplusplus
}
#endif
#endif
