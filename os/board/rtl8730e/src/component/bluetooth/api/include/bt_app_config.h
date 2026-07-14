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

#include "platform_autoconf.h"

#include <bt_api_config.h>
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
#include <bt_le_audio_app_config.h>
#endif
#if defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT
#include <bt_le_iso_app_config.h>
#endif
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <rtk_bt_mesh_def.h>
#endif

#define RTK_BT_LE_GAP_IRK_LEN       16

/**
 * @typedef rtk_bt_app_conf_t
 * @brief   structure of default APP configuration
 */
typedef struct {
	uint8_t  a2dp_role;                        /*!< Default A2DP profile role (Only used in BR/EDR) */
	uint8_t  spp_role;                         /*!< Default SPP profile role (Only used in BR/EDR) */
	uint8_t  hfp_role;                         /*!< Default HFP profile role (Only used in BR/EDR) */
	uint8_t  hid_role;                         /*!< Default HID profile role (Only used in BR/EDR) */
	uint32_t app_profile_support;              /*!< app profile support @ref rtk_bt_profile_t */
	uint8_t  server_chann;                     /*!< rfc server channel */
	uint16_t mtu_size;                         /*!< Deafult MTU config, Max value can be set is 512 */
	bool     master_init_mtu_req;              /*!< Whether local device as master will auto initiate exchange MTU request when connected.
                                                    If this value is false, user can use @ref rtk_bt_gattc_exchange_mtu
                                                    to initiate exchange MTU request. */
	bool     slave_init_mtu_req;               /*!< Whether local device as slave will auto initiate exchange MTU request when connected. */
	uint8_t  prefer_all_phy;                   /*!< Deafult PHY config: preferred all phy @ref rtk_bt_le_all_phy_prefer_t */
	uint8_t  prefer_tx_phy;                    /*!< Deafult PHY config: preferred tx phy @ref rtk_bt_le_trx_phy_prefer_t */
	uint8_t  prefer_rx_phy;                    /*!< Deafult PHY config: preferred rx phy @ref rtk_bt_le_trx_phy_prefer_t */
	uint16_t max_tx_octets;                    /*!< Default data len config: Max tx octects */
	uint16_t max_tx_time;                      /*!< Default data len config: Max tx time */
#if defined(RTK_BLE_SET_TX_QUEUE_NUM) && RTK_BLE_SET_TX_QUEUE_NUM
	uint16_t max_stack_tx_pending_num;         /*!< Default max stack tx pending num: default is RTK_BT_GATT_DEFAULT_CREDITS */
#endif
	bool     irk_auto_gen;                     /*!< Whether stack auto generate local IRK. If true, stack will auto generate a value for local IRK and save to
                                                    FTL first time, then load it from FTL when need to use local IRK next time. */
	uint8_t  irk[RTK_BT_LE_GAP_IRK_LEN];       /*!< Local IRK data. If irk_auto_gen is false, user can set local IRK value by this field. */
	bool     user_def_service;                 /*!< Whether register gap and gatt service in app, or use builtin service in stack */
	bool     cccd_not_check;                   /*!< Whether check if cccd is enabled before send indicate or notify. If true, cccd will not be checked;
                                                    if false, cccd will be checked, if bit of indicate/notify is not enabled, send indicate or notify will fail */
	bool     cccd_not_save;                    /*!< Whether save local server's cccd info to flash when bonded. If true, cccd will not be saved. If false,
                                                    cccd will be saved, and when reconnect to the bonded device, @ref RTK_BT_GATTS_EVT_CCCD_IND events
                                                    will be indicated immediately.
                                                    By default, cccd will be saved. And this value will be ignored if @ref RTK_BLE_AUDIO_SUPPORT is enabled.*/
	bool     key_convert_le_to_bredr;          /*!< Whether convert BLE LTK(only secure connections pairing) to BR/EDR link key (This configuration can only be used in BLE-BR/EDR Dual mode) */
	uint8_t  min_enc_key_size;                 /*!< The minimum encryption key size that can be permitted when BLE pairing. */
#if defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT
	rtk_bt_le_iso_app_conf_t le_iso_app_conf; /*!< Default configuration for le iso */
#endif
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
	rtk_bt_le_audio_app_conf_t le_audio_app_conf; /*!< Default configuration for le audio */
#endif
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	rtk_bt_mesh_app_conf_t bt_mesh_app_conf;   /*!< Default configuration for bt mesh */
#endif
} rtk_bt_app_conf_t;

#ifdef __cplusplus
}
#endif

#endif  /* __BT_APP_CFG_H__ */