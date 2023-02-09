/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#ifndef __BT_APP_CFG_H__
#define __BT_APP_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <bt_api_config.h>
#include <rtk_bt_le_audio.h>
#include <platform_opts_bt.h>

#define RTK_BT_LE_GAP_IRK_LEN		16

/**
 * @typedef rtk_bt_app_conf_t
 * @brief   structure of default APP configuration
 */
typedef struct {
	uint8_t   a2dp_role;                /*!< Default A2DP profile role(Only used in BR/EDR) */
	uint8_t   spp_role;                	/*!< Default SPP profile role(Only used in BR/EDR) */
	uint8_t   hfp_role;                 /*!< Default HFP profile role(Only used in BR/EDR) */
	uint8_t   app_profile_support;      /*!< app profile support @ref rtk_bt_profile_t */
	uint16_t  mtu_size;                 /*!< Deafult MTU config */
	bool	  master_init_mtu_req;      /*!< Whether master auto initiate exchange MTU request when connected.
											 When this value is false, user can use @ref uint16_t rtk_bt_gattc_exchange_mtu(uint16_t conn_handle)
											 to initiate exchange MTU request*/
	uint8_t   prefer_all_phy;           /*!< Deafult PHY config: preferred all phy */
	uint8_t   prefer_tx_phy;            /*!< Deafult PHY config: preferred tx phy */
	uint8_t   prefer_rx_phy;            /*!< Deafult PHY config: preferred rx phy */
	uint16_t  max_tx_octets;            /*!< Default data len config: Max tx octects */
	uint16_t  max_tx_time;              /*!< Default data len config: Max tx time */
	uint16_t  max_stack_tx_pending_num; /*!< Default max stack tx pending num: default is RTK_BT_GATT_DEFAULT_CREDITS */
	uint8_t   irk[RTK_BT_LE_GAP_IRK_LEN];    		/*!< Local IRK data */
	bool      user_def_service;         /*!< Whether register gap and gatt service in app, or use builtin service in stack */
	bool      cccd_not_check;           /*!< Whether check if cccd is enabled before send indicate or notify. When true, cccd will not be checked;
	 										 when false, cccd will be checked, if bit of indicate/notify is not enabled, send indicate or notify will fail */
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
	rtk_bt_le_audio_app_conf_t le_audio_app_conf;	/*!< Default configuration for le audio */
#endif
#if defined(CONFIG_BT_MESH) && CONFIG_BT_MESH
	uint8_t   bt_mesh_uuid[16];
#endif
} rtk_bt_app_conf_t;

#ifdef __cplusplus
}
#endif

#endif  /* __BT_APP_CFG_H__ */