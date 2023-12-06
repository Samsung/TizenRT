/**
 * @file      rtk_bt_mesh_light_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH light client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */


#ifndef __RTK_BT_MESH_LIGHT_LIGHTNESS_MODEL_H__
#define __RTK_BT_MESH_LIGHT_LIGHTNESS_MODEL_H__

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_onoff_model.h>

#define LIGHT_LC_PROPERTY_DATA_MAX_LEN    0x10

/* ------------------------------- Data Types ------------------------------- */

/**
 * @typedef   rtk_bt_mesh_light_lightness_client_model_act_t
 * @brief     BLE MESH light lightness client model act definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_light_lightness_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_client_model_evt_t
 * @brief     BLE MESH light lightness client model event definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_light_lightness_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_get_t
 * @brief     BLE MESH light lightness get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lightness_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_set_t
 * @brief     BLE MESH light lightness set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_lightness_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_linear_get_t
 * @brief     BLE MESH light lightness linear get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lightness_linear_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_linear_set_t
 * @brief     BLE MESH light lightness linear set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_lightness_linear_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_last_get_t
 * @brief     BLE MESH light lightness last get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lightness_last_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_default_get_t
 * @brief     BLE MESH light lightness default get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lightness_default_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_default_set_t
 * @brief     BLE MESH light lightness default set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	bool ack;
} rtk_bt_mesh_light_lightness_default_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_range_get_t
 * @brief     BLE MESH light lightness range get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lightness_range_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_range_set_t
 * @brief     BLE MESH light lightness range set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t range_min;
	uint16_t range_max;
	bool ack;
} rtk_bt_mesh_light_lightness_range_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_client_status_t
 * @brief     BLE MESH light lightness client model status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t present_lightness;
	bool optional;
	uint16_t target_lightness;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_lightness_client_status_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_client_status_last_t
 * @brief     BLE MESH light lightness client model last status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t lightness;
} rtk_bt_mesh_light_lightness_client_status_last_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_client_status_default_t
 * @brief     BLE MESH light lightness client model default status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t lightness;
} rtk_bt_mesh_light_lightness_client_status_default_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_client_status_range_t
 * @brief     BLE MESH light lightness client model range status message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_stat_t status;
	uint16_t range_min;
	uint16_t range_max;
} rtk_bt_mesh_light_lightness_client_status_range_t;

/******************light lightness server model*************************/
/**
 * @typedef   rtk_bt_mesh_light_lightness_server_model_evt_t
 * @brief     BLE MESH light lightness server model event definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_lightness_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_get_t
 * @brief     BLE MESH light lightness get evt message structure.
 */
typedef struct {
	uint16_t lightness;
} rtk_bt_mesh_light_lightness_server_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_direct_get_t
 * @brief     BLE MESH light lightness get direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_lightness_server_get_t *value;
} rtk_bt_mesh_light_lightness_server_direct_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_get_range_t
 * @brief     BLE MESH light lightness get range evt message structure.
 */
typedef struct {
	uint16_t range_min;
	uint16_t range_max;
} rtk_bt_mesh_light_lightness_server_get_range_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_direct_get_range_t
 * @brief     BLE MESH light lightness get range direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_lightness_server_get_range_t *value;
} rtk_bt_mesh_light_lightness_server_direct_get_range_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_get_default_transition_time_t
 * @brief     BLE MESH light lightness get default transition time evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_light_lightness_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_set_t
 * @brief     BLE MESH light lightness set evt message structure.
 */
typedef struct {
	uint16_t lightness;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_lightness_server_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_set_last_t
 * @brief     BLE MESH light lightness set last evt message structure.
 */
typedef struct {
	uint16_t lightness;
} rtk_bt_mesh_light_lightness_server_set_last_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_set_default_t
 * @brief     BLE MESH light lightness set default evt message structure.
 */
typedef struct {
	uint16_t lightness;
} rtk_bt_mesh_light_lightness_server_set_default_t;

/**
 * @typedef   rtk_bt_mesh_light_lightness_server_set_range_t
 * @brief     BLE MESH light lightness set range evt message structure.
 */
typedef struct {
	uint16_t range_min;
	uint16_t range_max;
} rtk_bt_mesh_light_lightness_server_set_range_t;

/*****************light lightness setup server model************************/
/**
 * @typedef   rtk_bt_mesh_light_lightness_setup_server_model_evt_t
 * @brief     BLE MESH light lightness setup server model event definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_SERVER_MODEL_SET = 1,
	RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_lightness_setup_server_model_evt_t;

/*****************light ctl model****************************/
/*****************light ctl client model***********************/
/**
 * @typedef   rtk_bt_mesh_light_ctl_client_model_act_t
 * @brief     BLE MESH light ctl client model act definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_GET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_SET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_GET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_SET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_GET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_SET,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_light_ctl_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_client_model_evt_t
 * @brief     BLE MESH light ctl client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_STATUS,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_STATUS,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_STATUS,
	RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_light_ctl_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_get_t
 * @brief     BLE MESH light ctl get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_ctl_get_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_set_t
 * @brief     BLE MESH light ctl set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	uint16_t temperature;
	int16_t delta_uv;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_ctl_set_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_temperature_get_t
 * @brief     BLE MESH light ctl temperature get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_ctl_temperature_get_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_temperature_set_t
 * @brief     BLE MESH light ctl temperature set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t temperature;
	int16_t delta_uv;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_ctl_temperature_set_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_temperature_range_get_t
 * @brief     BLE MESH light ctl temperature range get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_ctl_temperature_range_get_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_temperature_range_set_t
 * @brief     BLE MESH light ctl temperature range set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t range_min;
	uint16_t range_max;
	bool ack;
} rtk_bt_mesh_light_ctl_temperature_range_set_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_default_get_t
 * @brief     BLE MESH light ctl default get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_ctl_default_get_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_default_set_t
 * @brief     BLE MESH light ctl default set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	uint16_t temperature;
	int16_t delta_uv;
	bool ack;
} rtk_bt_mesh_light_ctl_default_set_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_client_status_t
 * @brief     BLE MESH light ctl client model status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t present_lightness;
	uint16_t present_temperature;
	bool optional;
	uint16_t target_lightness;
	uint16_t target_temperature;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_ctl_client_status_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_client_status_temperature_t
 * @brief     BLE MESH light ctl client model status temperature message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t present_temperature;
	uint16_t present_delta_uv;
	bool optional;
	uint16_t target_temperature;
	uint16_t target_delta_uv;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_ctl_client_status_temperature_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_client_status_temperature_range_t
 * @brief     BLE MESH light ctl client model status temperature range message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_stat_t status;
	uint16_t range_min;
	uint16_t range_max;
} rtk_bt_mesh_light_ctl_client_status_temperature_range_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_client_status_default_t
 * @brief     BLE MESH light ctl client model status default message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t lightness;
	uint16_t temperature;
	int16_t delta_uv;
} rtk_bt_mesh_light_ctl_client_status_default_t;

/*****************light ctl server model****************************/
/**
 * @typedef   rtk_bt_mesh_light_ctl_server_model_evt_t
 * @brief     BLE MESH light ctl server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_DEFAULT_GET,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_ctl_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_get_t
 * @brief     BLE MESH light ctl get evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t temperature;
} rtk_bt_mesh_light_ctl_server_get_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_direct_get_t
 * @brief     BLE MESH light ctl get direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_ctl_server_get_t *value;
} rtk_bt_mesh_light_ctl_server_direct_get_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_get_temperature_t
 * @brief     BLE MESH light ctl get temperature evt message structure.
 */
typedef struct {
	uint16_t temperature;
	int16_t delta_uv;
} rtk_bt_mesh_light_ctl_server_get_temperature_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_direct_get_temperature_t
 * @brief     BLE MESH light ctl get temperature direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_ctl_server_get_temperature_t *value;
} rtk_bt_mesh_light_ctl_server_direct_get_temperature_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_get_default_transition_time_t
 * @brief     BLE MESH light ctl get default transition time evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_light_ctl_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_get_default_t
 * @brief     BLE MESH light ctl get default evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t temperature;
	int16_t delta_uv;
} rtk_bt_mesh_light_ctl_server_get_default_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_direct_get_default_t
 * @brief     BLE MESH light ctl get default direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_ctl_server_get_default_t *value;
} rtk_bt_mesh_light_ctl_server_direct_get_default_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_get_temperature_range_t
 * @brief     BLE MESH light ctl get temperature range evt message structure.
 */
typedef struct {
	uint16_t range_min;
	uint16_t range_max;
} rtk_bt_mesh_light_ctl_server_get_temperature_range_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t
 * @brief     BLE MESH light ctl get temperature range direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_ctl_server_get_temperature_range_t *value;
} rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_set_t
 * @brief     BLE MESH light ctl set evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t temperature;
	int16_t delta_uv;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_ctl_server_set_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_set_temperature_t
 * @brief     BLE MESH light ctl set temperature evt message structure.
 */
typedef struct {
	uint16_t temperature;
	int16_t delta_uv;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_ctl_server_set_temperature_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_set_default_t
 * @brief     BLE MESH light ctl set default evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t temperature;
	int16_t delta_uv;
} rtk_bt_mesh_light_ctl_server_set_default_t;

/**
 * @typedef   rtk_bt_mesh_light_ctl_server_set_temperature_range_t
 * @brief     BLE MESH light ctl set temperature range evt message structure.
 */
typedef struct {
	uint16_t range_min;
	uint16_t range_max;
} rtk_bt_mesh_light_ctl_server_set_temperature_range_t;

/**************light ctl setup server model****************/
/**
 * @typedef   rtk_bt_mesh_light_ctl_setup_server_model_evt_t
 * @brief     BLE MESH light ctl setup server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_SET = 1,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_DEFAULT_SET,
	RTK_BT_MESH_LIGHT_CTL_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_ctl_setup_server_model_evt_t;

/*****************light ctl temperature server model*******************************/
/**
 * @typedef   rtk_bt_mesh_light_ctl_temperature_server_model_evt_t
 * @brief     BLE MESH light ctl temperature server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_GET = 1,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_SET,
	RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET_T,
	RTK_BT_MESH_LIGHT_CTL_SERVER_TEMPERATURE_MODEL_EVT_MAX,
} rtk_bt_mesh_light_ctl_temperature_server_model_evt_t;

/*******************light hsl model*******************************/
/**********************light hsl client model************************************/
/**
 * @typedef   rtk_bt_mesh_light_hsl_client_model_act_t
 * @brief     BLE MESH light hsl client model act definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_TARGET_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_light_hsl_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_client_model_evt_t
 * @brief     BLE MESH light hsl client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_LIGHT_HSL_TARGET_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_light_hsl_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_get_t
 * @brief     BLE MESH light hsl get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_hsl_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_set_t
 * @brief     BLE MESH light hsl set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_hsl_set_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_target_get_t
 * @brief     BLE MESH light hsl target get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_hsl_target_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_hue_get_t
 * @brief     BLE MESH light hsl hue get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_hsl_hue_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_hue_set_t
 * @brief     BLE MESH light hsl hue set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t hue;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_hsl_hue_set_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_saturation_get_t
 * @brief     BLE MESH light hsl saturation get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_hsl_saturation_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_saturation_set_t
 * @brief     BLE MESH light hsl saturation set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t saturation;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_hsl_saturation_set_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_default_get_t
 * @brief     BLE MESH light hsl default get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_hsl_default_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_default_set_t
 * @brief     BLE MESH light hsl default set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
	bool ack;
} rtk_bt_mesh_light_hsl_default_set_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_range_get_t
 * @brief     BLE MESH light hsl range get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_hsl_range_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_range_set_t
 * @brief     BLE MESH light hsl range set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t hue_range_min;
	uint16_t hue_range_max;
	uint16_t saturation_range_min;
	uint16_t saturation_range_max;
	bool ack;
} rtk_bt_mesh_light_hsl_range_set_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_client_status_t
 * @brief     BLE MESH light hsl client model status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_hsl_client_status_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_client_status_hue_t
 * @brief     BLE MESH light hsl hue client model status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t present_hue;
	bool optional;
	uint16_t target_hue;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_hsl_client_status_hue_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_client_status_saturation_t
 * @brief     BLE MESH light hsl saturation client model status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t present_saturation;
	bool optional;
	uint16_t target_saturation;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_hsl_client_status_saturation_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_client_status_default_t
 * @brief     BLE MESH light hsl client model default status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
} rtk_bt_mesh_light_hsl_client_status_default_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_client_status_range_t
 * @brief     BLE MESH light hsl client model range status message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_stat_t status;
	uint16_t hue_range_min;
	uint16_t hue_range_max;
	uint16_t saturation_range_min;
	uint16_t saturation_range_max;
} rtk_bt_mesh_light_hsl_client_status_range_t;

/*******************light hsl server model*******************************/
/**
 * @typedef   rtk_bt_mesh_light_hsl_server_model_evt_t
 * @brief     BLE MESH light hsl server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_HSL_DEFAULT_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_hsl_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_get_t
 * @brief     BLE MESH light hsl get evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
} rtk_bt_mesh_light_hsl_server_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_direct_get_t
 * @brief     BLE MESH light hsl get direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_hsl_server_get_t *value;
} rtk_bt_mesh_light_hsl_server_direct_get_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_get_hue_t
 * @brief     BLE MESH light hsl get hue evt message structure.
 */
typedef struct {
	uint16_t hue;
} rtk_bt_mesh_light_hsl_server_get_hue_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_direct_get_hue_t
 * @brief     BLE MESH light hsl get hue direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_hsl_server_get_hue_t *value;
} rtk_bt_mesh_light_hsl_server_direct_get_hue_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_get_saturation_t
 * @brief     BLE MESH light hsl get saturation evt message structure.
 */
typedef struct {
	uint16_t saturation;
} rtk_bt_mesh_light_hsl_server_get_saturation_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_direct_get_saturation_t
 * @brief     BLE MESH light hsl get saturation direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_hsl_server_get_saturation_t *value;
} rtk_bt_mesh_light_hsl_server_direct_get_saturation_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_get_default_t
 * @brief     BLE MESH light hsl get default evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
} rtk_bt_mesh_light_hsl_server_get_default_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_direct_get_default_t
 * @brief     BLE MESH light hsl get default direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_hsl_server_get_default_t *value;
} rtk_bt_mesh_light_hsl_server_direct_get_default_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_get_range_t
 * @brief     BLE MESH light hsl get range evt message structure.
 */
typedef struct {
	uint16_t hue_range_min;
	uint16_t hue_range_max;
	uint16_t saturation_range_min;
	uint16_t saturation_range_max;
} rtk_bt_mesh_light_hsl_server_get_range_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_direct_get_range_t
 * @brief     BLE MESH light hsl get range direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_hsl_server_get_range_t *value;
} rtk_bt_mesh_light_hsl_server_direct_get_range_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_get_default_transition_time_t
 * @brief     BLE MESH light hsl get default transition time evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_light_hsl_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_set_t
 * @brief     BLE MESH light hsl set evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_hsl_server_set_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_set_hue_t
 * @brief     BLE MESH light hsl set hue evt message structure.
 */
typedef struct {
	uint16_t hue;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_hsl_server_set_hue_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_set_saturation_t
 * @brief     BLE MESH light hsl set saturation evt message structure.
 */
typedef struct {
	uint16_t saturation;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_hsl_server_set_saturation_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_set_default_t
 * @brief     BLE MESH light hsl set default evt message structure.
 */
typedef struct {
	uint16_t lightness;
	uint16_t hue;
	uint16_t saturation;
} rtk_bt_mesh_light_hsl_server_set_default_t;

/**
 * @typedef   rtk_bt_mesh_light_hsl_server_set_range_t
 * @brief     BLE MESH light hsl set range evt message structure.
 */
typedef struct {
	uint16_t hue_range_min;
	uint16_t hue_range_max;
	uint16_t saturation_range_min;
	uint16_t saturation_range_max;
} rtk_bt_mesh_light_hsl_server_set_range_t;

/************************light hsl hue server model*************************************/
/**
 * @typedef   rtk_bt_mesh_light_hsl_hue_server_model_evt_t
 * @brief     BLE MESH light hsl hue server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_H,
	RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_hsl_hue_server_model_evt_t;

/***************light hsl saturation server model**********************/
/**
 * @typedef   rtk_bt_mesh_light_hsl_saturation_server_model_evt_t
 * @brief     BLE MESH light hsl saturation server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_S,
	RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_hsl_saturation_server_model_evt_t;

/********************light hsl setup server model**************************/
/**
 * @typedef   rtk_bt_mesh_light_hsl_setup_server_model_evt_t
 * @brief     BLE MESH light hsl setup server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_HSL_DEFAULT_SERVER_MODEL_SET = 1,
	RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_HSL_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_hsl_setup_server_model_evt_t;

/****************light xyl model***********************/
/**************light xyl client model***********************/
/**
 * @typedef   rtk_bt_mesh_light_xyl_client_model_act_t
 * @brief     BLE MESH light xyl client model act definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_XYL_TARGET_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_light_xyl_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_client_model_evt_t
 * @brief     BLE MESH light xyl client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_LIGHT_XYL_TARGET_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_light_xyl_client_model_evt_t;

/**
 * @typedef LIGHT_XYL_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 */
typedef struct {
	uint16_t xyl_lightness;
	uint16_t xyl_x;
	uint16_t xyl_y;
} _PACKED4_ rtk_bt_mesh_light_xyl_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_get_t
 * @brief     BLE MESH light xyl get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_xyl_get_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_set_t
 * @brief     BLE MESH light xyl set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	rtk_bt_mesh_light_xyl_t xyl;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_xyl_set_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_target_get_t
 * @brief     BLE MESH light xyl target get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_xyl_target_get_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_default_get_t
 * @brief     BLE MESH light xyl default get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_xyl_default_get_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_default_set_t
 * @brief     BLE MESH light xyl default set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	rtk_bt_mesh_light_xyl_t xyl;
	bool ack;
} rtk_bt_mesh_light_xyl_default_set_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_range_get_t
 * @brief     BLE MESH light xyl range get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_xyl_range_get_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_range_set_t
 * @brief     BLE MESH light xyl range set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t xyl_x_range_min;
	uint16_t xyl_x_range_max;
	uint16_t xyl_y_range_min;
	uint16_t xyl_y_range_max;
	bool ack;
} rtk_bt_mesh_light_xyl_range_set_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_client_status_t
 * @brief     BLE MESH light xyl client model status message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_light_xyl_t xyl;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_xyl_client_status_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_client_status_default_t
 * @brief     BLE MESH light xyl client model default status message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_light_xyl_t xyl;
} rtk_bt_mesh_light_xyl_client_status_default_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_client_status_range_t
 * @brief     BLE MESH light xyl client model range status message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_stat_t status;
	uint16_t xyl_x_range_min;
	uint16_t xyl_x_range_max;
	uint16_t xyl_y_range_min;
	uint16_t xyl_y_range_max;
} rtk_bt_mesh_light_xyl_client_status_range_t;

/*************light xyl server model***********/
/**
 * @typedef   rtk_bt_mesh_light_xyl_server_model_evt_t
 * @brief     BLE MESH light xyl server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_XYL_DEFAULT_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_XYL_RANGE_SERVER_MODEL_GET,
	RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_xyl_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_get_t
 * @brief     BLE MESH light xyl get evt message structure.
 */
typedef rtk_bt_mesh_light_xyl_t rtk_bt_mesh_light_xyl_server_get_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_get_t
 * @brief     BLE MESH light xyl get direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_xyl_server_get_t *value;
}rtk_bt_mesh_light_xyl_server_direct_get_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_get_range_t
 * @brief     BLE MESH light xyl get range evt message structure.
 */
typedef struct {
	uint16_t xyl_x_range_min;
	uint16_t xyl_x_range_max;
	uint16_t xyl_y_range_min;
	uint16_t xyl_y_range_max;
} rtk_bt_mesh_light_xyl_server_get_range_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_direct_get_range_t
 * @brief     BLE MESH light xyl get range direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_xyl_server_get_range_t *value;
} rtk_bt_mesh_light_xyl_server_direct_get_range_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_get_default_t
 * @brief     BLE MESH light xyl get default evt message structure.
 */
typedef rtk_bt_mesh_light_xyl_t rtk_bt_mesh_light_xyl_server_get_default_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_direct_get_default_t
 * @brief     BLE MESH light xyl get default direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_xyl_server_get_default_t *value;
}rtk_bt_mesh_light_xyl_server_direct_get_default_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_get_default_transition_time_t
 * @brief     BLE MESH light xyl get default transition time evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_light_xyl_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_set_t
 * @brief     BLE MESH light xyl set evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_xyl_t xyl;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_xyl_server_set_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_set_default_t
 * @brief     BLE MESH light xyl set default evt message structure.
 */
typedef rtk_bt_mesh_light_xyl_t rtk_bt_mesh_light_xyl_server_set_default_t;

/**
 * @typedef   rtk_bt_mesh_light_xyl_server_set_range_t
 * @brief     BLE MESH light xyl set range evt message structure.
 */
typedef struct {
	uint16_t xyl_x_range_min;
	uint16_t xyl_x_range_max;
	uint16_t xyl_y_range_min;
	uint16_t xyl_y_range_max;
} rtk_bt_mesh_light_xyl_server_set_range_t;

/***********light xyl setup server model**************/
/**
 * @typedef   rtk_bt_mesh_light_xyl_setup_server_model_evt_t
 * @brief     BLE MESH light xyl setup server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_XYL_DEFAULT_SERVER_MODEL_SET = 1,
	RTK_BT_MESH_LIGHT_XYL_RANGE_SERVER_MODEL_SET,
	RTK_BT_MESH_LIGHT_XYL_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_xyl_setup_server_model_evt_t;

/************light lc model*********************/
/**************light lc client model********************/
/**
 * @typedef   rtk_bt_mesh_light_lc_client_model_act_t
 * @brief     BLE MESH light lc client model act definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_GET,
	RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_SET,
	RTK_BT_MESH_LIGHT_LC_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_light_lc_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_client_model_evt_t
 * @brief     BLE MESH light lc client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_STATUS,
	RTK_BT_MESH_LIGHT_LC_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_light_lc_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_mode_get_t
 * @brief     BLE MESH light lc mode get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lc_mode_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_mode_set_t
 * @brief     BLE MESH light lc mode set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint8_t mode;
	bool ack;
} rtk_bt_mesh_light_lc_mode_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_om_get_t
 * @brief     BLE MESH light lc om get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lc_om_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_om_set_t
 * @brief     BLE MESH light lc om set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint8_t mode;
	bool ack;
} rtk_bt_mesh_light_lc_om_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_lightonoff_get_t
 * @brief     BLE MESH light lc light on off get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_light_lc_lightonoff_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_lightonoff_set_t
 * @brief     BLE MESH light lc light on off set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint8_t light_on_off;
	uint8_t tid;
	bool optional;
	rtk_bt_mesh_generic_transition_time_t trans_time;
	uint8_t delay;
	bool ack;
} rtk_bt_mesh_light_lc_lightonoff_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_property_get_t
 * @brief     BLE MESH light lc property get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t property_id;
} rtk_bt_mesh_light_lc_property_get_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_property_set_t
 * @brief     BLE MESH light lc property set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
	uint16_t property_id;
	uint8_t pvalue[LIGHT_LC_PROPERTY_DATA_MAX_LEN];//uint8_t *pvalue
	uint16_t value_len;
	bool ack;
} rtk_bt_mesh_light_lc_property_set_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_client_mode_status_t
 * @brief     BLE MESH light lc client model mode status message structure.
 */
typedef struct {
	uint16_t src;
	uint8_t mode;
} rtk_bt_mesh_light_lc_client_mode_status_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_client_om_status_t
 * @brief     BLE MESH light lc client model om status message structure.
 */
typedef struct {
	uint16_t src;
	uint8_t mode;
} rtk_bt_mesh_light_lc_client_om_status_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_client_light_on_off_status_t
 * @brief     BLE MESH light lc client model light on off status message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_on_off_t present_on_off;
	bool optional;
	rtk_bt_mesh_generic_on_off_t target_on_off;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_lc_client_light_on_off_status_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_client_property_status_t
 * @brief     BLE MESH light lc client model property status message structure.
 */
typedef struct {
	uint16_t src;
	uint16_t property_id;
	uint32_t property_value;
} rtk_bt_mesh_light_lc_client_property_status_t;

/*********light lc server model********************/
/**
 * @typedef   rtk_bt_mesh_light_lc_server_model_evt_t
 * @brief     BLE MESH light lc server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_MODE_GET = 1,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_MODE_SET,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_OM_GET,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_OM_SET,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_LIGHT_ON_OFF_GET,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_SM_TRANSITION_TIME_GET,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_LIGHT_ON_OFF_SET,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_lc_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_mode_t
 * @brief     BLE MESH light lc get mode evt message structure.
 */
typedef struct {
	uint8_t mode;
} rtk_bt_mesh_light_lc_server_get_mode_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_direct_get_mode_t
 * @brief     BLE MESH light lc direct get mode evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_lc_server_get_mode_t *value;
} rtk_bt_mesh_light_lc_server_direct_get_mode_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_om_t
 * @brief     BLE MESH light lc get om evt message structure.
 */
typedef struct {
	uint8_t mode;
} rtk_bt_mesh_light_lc_server_get_om_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_direct_get_mode_t
 * @brief     BLE MESH light lc direct get mode evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_lc_server_get_om_t *value;
} rtk_bt_mesh_light_lc_server_direct_get_om_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_light_on_off_t
 * @brief     BLE MESH light lc get light on off evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_on_off_t on_off;
} rtk_bt_mesh_light_lc_server_get_light_on_off_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t
 * @brief     BLE MESH light lc direct get light on off evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_lc_server_get_light_on_off_t *value;
} rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_property_t
 * @brief     BLE MESH light lc get property evt message structure.
 */
typedef struct {
	uint16_t property_id;
	uint32_t property_value;
	uint8_t value_len;
} rtk_bt_mesh_light_lc_server_get_property_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_property_t
 * @brief     BLE MESH light lc direct get property parameter evt message structure.
 */
typedef struct {
	uint8_t value_len;
	uint32_t property_value;
} rtk_bt_mesh_light_lc_server_direct_get_property_para_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_direct_get_property_t
 * @brief     BLE MESH light lc direct get property evt message structure.
 */
typedef struct {
	uint16_t property_id;
	rtk_bt_mesh_light_lc_server_direct_get_property_para_t *value;
} rtk_bt_mesh_light_lc_server_direct_get_property_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_default_transition_time_t
 * @brief     BLE MESH light lc get default transition time evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_light_lc_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_get_sm_transition_time_t
 * @brief     BLE MESH light lc get sm transition time evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_on_off_t light_on_off;
	rtk_bt_mesh_generic_transition_time_t sm_trans_time;
} rtk_bt_mesh_light_lc_server_get_sm_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t
 * @brief     BLE MESH light lc get sm transition time direct evt message structure.
 */
typedef struct {
	rtk_bt_mesh_light_lc_server_get_sm_transition_time_t *value;
} rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_set_mode_t
 * @brief     BLE MESH light lc set mode evt message structure.
 */
typedef struct {
	uint8_t mode;
} rtk_bt_mesh_light_lc_server_set_mode_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_set_om_t
 * @brief     BLE MESH light lc set om evt message structure.
 */
typedef struct {
	uint8_t mode;
} rtk_bt_mesh_light_lc_server_set_om_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_set_light_on_off_t
 * @brief     BLE MESH light lc set light on off evt message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_on_off_t light_on_off;
	rtk_bt_mesh_generic_transition_time_t total_time;
	rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_light_lc_server_set_light_on_off_t;

/**
 * @typedef   rtk_bt_mesh_light_lc_server_set_property_t
 * @brief     BLE MESH light lc set property evt message structure.
 */
typedef struct {
	uint16_t property_id;
	uint32_t property_value;
} rtk_bt_mesh_light_lc_server_set_property_t;

/***********light lc setup server*************/
/**
 * @typedef   rtk_bt_mesh_light_lc_setup_server_model_evt_t
 * @brief     BLE MESH light lc setup server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_PROPERTY_GET = 1,
	RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_PROPERTY_SET,
	RTK_BT_MESH_LIGHT_LC_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_light_lc_setup_server_model_evt_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_light_model BT LE Mesh Light Model APIs
 * @brief     BT LE Mesh Light Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_get(rtk_bt_mesh_light_lightness_get_t *lightness_get)
 * @brief     Send light lightness get message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_GET
 * @param[in] lightness_get: light lightness get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_get(rtk_bt_mesh_light_lightness_get_t *lightness_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_set(rtk_bt_mesh_light_lightness_set_t *lightness_set)
 * @brief     Send light lightness set message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_SET
 * @param[in] lightness_set: light lightness set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_set(rtk_bt_mesh_light_lightness_set_t *lightness_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_linear_get(rtk_bt_mesh_light_lightness_linear_get_t *lightness_linear_get)
 * @brief     Send light lightness linear get message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_GET
 * @param[in] lightness_linear_get: light lightness linear get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_linear_get(rtk_bt_mesh_light_lightness_linear_get_t *lightness_linear_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_linear_set(rtk_bt_mesh_light_lightness_linear_set_t *lightness_linear_set)
 * @brief     Send light lightness linear set message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_SET
 * @param[in] lightness_linear_set: light lightness linear set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_linear_set(rtk_bt_mesh_light_lightness_linear_set_t *lightness_linear_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_last_get(rtk_bt_mesh_light_lightness_last_get_t *lightness_last_get)
 * @brief     Send light lightness last get message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_CLIENT_MODEL_GET
 * @param[in] lightness_last_set: light lightness last get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_last_get(rtk_bt_mesh_light_lightness_last_get_t *lightness_last_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_default_get(rtk_bt_mesh_light_lightness_default_get_t *lightness_default_get)
 * @brief     Send light lightness default get message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_GET
 * @param[in] lightness_default_get: light lightness default get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_default_get(rtk_bt_mesh_light_lightness_default_get_t *lightness_default_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_default_set(rtk_bt_mesh_light_lightness_default_set_t *lightness_default_set)
 * @brief     Send light lightness default set message to light lightness setup server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_SET
 * @param[in] lightness_default_set: light lightness default set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_default_set(rtk_bt_mesh_light_lightness_default_set_t *lightness_default_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_range_get(rtk_bt_mesh_light_lightness_range_get_t *lightness_range_get)
 * @brief     Send light lightness range get message to light lightness server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_GET
 * @param[in] lightness_range_get: light lightness range get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_range_get(rtk_bt_mesh_light_lightness_range_get_t *lightness_range_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lightness_range_set(rtk_bt_mesh_light_lightness_range_set_t *lightness_range_set)
 * @brief     Send light lightness range set message to light lightness setup server, will cause event @ref RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_SET
 * @param[in] lightness_range_set: light lightness range set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lightness_range_set(rtk_bt_mesh_light_lightness_range_set_t *lightness_range_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_get(rtk_bt_mesh_light_ctl_get_t *ctl_get)
 * @brief     Send light ctl get message to light ctl server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_GET
 * @param[in] ctl_get: light ctl get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_get(rtk_bt_mesh_light_ctl_get_t *ctl_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_set(rtk_bt_mesh_light_ctl_set_t *ctl_set)
 * @brief     Send light ctl set message to light ctl server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_SET
 * @param[in] ctl_set: light ctl set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_set(rtk_bt_mesh_light_ctl_set_t *ctl_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_temperature_get(rtk_bt_mesh_light_ctl_temperature_get_t *ctl_temperature_get)
 * @brief     Send light ctl temperature get message to light ctl temperature server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_GET
 * @param[in] ctl_temperature_get: light ctl temperature get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_temperature_get(rtk_bt_mesh_light_ctl_temperature_get_t *ctl_temperature_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_temperature_set(rtk_bt_mesh_light_ctl_temperature_set_t *ctl_temperature_set)
 * @brief     Send light ctl temperature set message to light ctl temperature server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_SET
 * @param[in] ctl_temperature_set: light ctl temperature set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_temperature_set(rtk_bt_mesh_light_ctl_temperature_set_t *ctl_temperature_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_temperature_range_get(rtk_bt_mesh_light_ctl_temperature_range_get_t *ctl_temperature_range_get)
 * @brief     Send light ctl range get message to light ctl server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_GET
 * @param[in] ctl_temperature_range_get: light ctl temperature range get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_temperature_range_get(rtk_bt_mesh_light_ctl_temperature_range_get_t *ctl_temperature_range_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_temperature_range_set(rtk_bt_mesh_light_ctl_temperature_range_set_t *ctl_temperature_range_set)
 * @brief     Send light ctl range set message to light ctl setup server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_SET
 * @param[in] ctl_temperature_range_set: light ctl temperature range set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_temperature_range_set(rtk_bt_mesh_light_ctl_temperature_range_set_t *ctl_temperature_range_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_default_get(rtk_bt_mesh_light_ctl_default_get_t *ctl_default_get)
 * @brief     Send light ctl default get message to light ctl server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_GET
 * @param[in] ctl_default_get: light ctl defaultget message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_default_get(rtk_bt_mesh_light_ctl_default_get_t *ctl_default_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_ctl_default_set(rtk_bt_mesh_light_ctl_default_set_t *ctl_default_set)
 * @brief     Send light ctl default set message to light ctl setup server, will cause event @ref RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_SET
 * @param[in] ctl_default_set: light ctl default set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_ctl_default_set(rtk_bt_mesh_light_ctl_default_set_t *ctl_default_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_get(rtk_bt_mesh_light_hsl_get_t *hsl_get)
 * @brief     Send light hsl get message to light hsl server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_GET
 * @param[in] hsl_get: light hsl get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_get(rtk_bt_mesh_light_hsl_get_t *hsl_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_set(rtk_bt_mesh_light_hsl_set_t *hsl_set)
 * @brief     Send light hsl set message to light hsl server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_SET
 * @param[in] hsl_set: light hsl set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_set(rtk_bt_mesh_light_hsl_set_t *hsl_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_target_get(rtk_bt_mesh_light_hsl_target_get_t *hsl_target_get)
 * @brief     Send light hsl target get message to light hsl server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_TARGET_CLIENT_MODEL_GET
 * @param[in] hsl_target_get: light hsl target get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_target_get(rtk_bt_mesh_light_hsl_target_get_t *hsl_target_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_hue_get(rtk_bt_mesh_light_hsl_hue_get_t *hsl_hue_get)
 * @brief     Send light hsl hue get message to light hsl hue server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_GET
 * @param[in] hsl_hue_get: light hsl hue get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_hue_get(rtk_bt_mesh_light_hsl_hue_get_t *hsl_hue_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_hue_set(rtk_bt_mesh_light_hsl_hue_set_t *hsl_hue_set)
 * @brief     Send light hsl hue set message to light hsl hue server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_SET
 * @param[in] hsl_hue_set: light hsl hue set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_hue_set(rtk_bt_mesh_light_hsl_hue_set_t *hsl_hue_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_saturation_get(rtk_bt_mesh_light_hsl_saturation_get_t *hsl_saturation_get)
 * @brief     Send light hsl saturation get message to light hsl saturation server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_GET
 * @param[in] hsl_saturation_get: light hsl saturation get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_saturation_get(rtk_bt_mesh_light_hsl_saturation_get_t *hsl_saturation_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_saturation_set(rtk_bt_mesh_light_hsl_saturation_set_t *hsl_saturation_set)
 * @brief     Send light hsl saturation set message to light hsl saturation server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_SET
 * @param[in] hsl_saturation_set: light hsl saturation set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_saturation_set(rtk_bt_mesh_light_hsl_saturation_set_t *hsl_saturation_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_default_get(rtk_bt_mesh_light_hsl_default_get_t *hsl_default_get)
 * @brief     Send light hsl default get message to light hsl server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_GET
 * @param[in] hsl_default_get: light hsl default get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_default_get(rtk_bt_mesh_light_hsl_default_get_t *hsl_default_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_default_set(rtk_bt_mesh_light_hsl_default_set_t *hsl_default_set)
 * @brief     Send light hsl default set message to light hsl setup server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_SET
 * @param[in] hsl_default_set: light hsl default set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_default_set(rtk_bt_mesh_light_hsl_default_set_t *hsl_default_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_range_get(rtk_bt_mesh_light_hsl_range_get_t *hsl_range_get)
 * @brief     Send light hsl range get message to light hsl server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_GET
 * @param[in] hsl_range_get: light hsl range get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_range_get(rtk_bt_mesh_light_hsl_range_get_t *hsl_range_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_hsl_range_set(rtk_bt_mesh_light_hsl_range_set_t *hsl_range_set)
 * @brief     Send light hsl range set message to light hsl setup server, will cause event @ref RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_SET
 * @param[in] hsl_range_set: light hsl range set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_hsl_range_set(rtk_bt_mesh_light_hsl_range_set_t *hsl_range_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_get(rtk_bt_mesh_light_xyl_get_t *xyl_get)
 * @brief     Send light xyl get message to light xyl server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_GET
 * @param[in] xyl_get: light xyl get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_get(rtk_bt_mesh_light_xyl_get_t *xyl_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_set(rtk_bt_mesh_light_xyl_set_t *xyl_set)
 * @brief     Send light xyl set message to light xyl server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_SET
 * @param[in] xyl_set: light xyl set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_set(rtk_bt_mesh_light_xyl_set_t *xyl_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_target_get(rtk_bt_mesh_light_xyl_target_get_t *xyl_target_get)
 * @brief     Send light xyl target get message to light xyl server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_TARGET_CLIENT_MODEL_GET
 * @param[in] xyl_target_get: light xyl target get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_target_get(rtk_bt_mesh_light_xyl_target_get_t *xyl_target_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_default_get(rtk_bt_mesh_light_xyl_default_get_t *xyl_default_get)
 * @brief     Send light xyl default get message to light xyl server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_GET
 * @param[in] xyl_default_get: light xyl default get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_default_get(rtk_bt_mesh_light_xyl_default_get_t *xyl_default_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_default_set(rtk_bt_mesh_light_xyl_default_set_t *xyl_default_set)
 * @brief     Send light xyl default set message to light xyl setup server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_SET
 * @param[in] xyl_default_set: light xyl default set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_default_set(rtk_bt_mesh_light_xyl_default_set_t *xyl_default_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_range_get(rtk_bt_mesh_light_xyl_range_get_t *xyl_range_get)
 * @brief     Send light xyl range get message to light xyl server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_GET
 * @param[in] xyl_range_get: light xyl range get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_range_get(rtk_bt_mesh_light_xyl_range_get_t *xyl_range_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_xyl_range_set(rtk_bt_mesh_light_xyl_range_set_t *xyl_range_set)
 * @brief     Send light xyl range set message to light xyl setup server, will cause event @ref RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_SET
 * @param[in] xyl_range_set: light xyl range set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_xyl_range_set(rtk_bt_mesh_light_xyl_range_set_t *xyl_range_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_mode_get(rtk_bt_mesh_light_lc_mode_get_t *lc_mode_get)
 * @brief     Send light lc mode get message to light lc server, will cause event @ref RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_GET
 * @param[in] lc_mode_get: light lc mode get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_mode_get(rtk_bt_mesh_light_lc_mode_get_t *lc_mode_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_mode_set(rtk_bt_mesh_light_lc_mode_set_t *lc_mode_set)
 * @brief     Send light lc mode set message to light lc server, will cause event @ref RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_SET
 * @param[in] lc_mode_set: light lc mode get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_mode_set(rtk_bt_mesh_light_lc_mode_set_t *lc_mode_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_om_get(rtk_bt_mesh_light_lc_om_get_t *lc_om_get)
 * @brief     Send light lc om get message to light lc server, will cause event @ref RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_GET
 * @param[in] lc_om_get: light lc om get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_om_get(rtk_bt_mesh_light_lc_om_get_t *lc_om_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_om_set(rtk_bt_mesh_light_lc_om_set_t *lc_om_set)
 * @brief     Send light lc om set message to light lc server, will cause event @ref RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_SET
 * @param[in] lc_om_set: light lc om set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_om_set(rtk_bt_mesh_light_lc_om_set_t *lc_om_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_lightonoff_get(rtk_bt_mesh_light_lc_lightonoff_get_t *lc_lightonoff_get)
 * @brief     Send light lc light on off get message to light lc server, will cause event @ref RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_GET
 * @param[in] lc_lightonoff_get: light on off get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_lightonoff_get(rtk_bt_mesh_light_lc_lightonoff_get_t *lc_lightonoff_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_lightonoff_set(rtk_bt_mesh_light_lc_lightonoff_set_t *lc_lightonoff_set)
 * @brief     Send light lc light on off set message to light lc server, will cause event @ref RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_SET
 * @param[in] lc_lightonoff_set: light on off set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_lightonoff_set(rtk_bt_mesh_light_lc_lightonoff_set_t *lc_lightonoff_set);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_property_get(rtk_bt_mesh_light_lc_property_get_t *lc_property_get)
 * @brief     Send light lc property get message to light lc setup server, will cause event @ref RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_GET
 * @param[in] lc_property_get: light property get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_property_get(rtk_bt_mesh_light_lc_property_get_t *lc_property_get);

/**
 * @fn        uint16_t rtk_bt_mesh_light_lc_property_set(rtk_bt_mesh_light_lc_property_set_t *lc_property_set)
 * @brief     Send light lc property set message to light lc setup server, will cause event @ref RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_SET
 * @param[in] lc_property_set: light property set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_light_lc_property_set(rtk_bt_mesh_light_lc_property_set_t *lc_property_set);

/**
 * @}
 */

#endif /* __RTK_BT_COMMON_H__ */