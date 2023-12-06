/**
 * @file      rtk_bt_mesh_generic_model.h
 * @author    quinn_yang@realsil.com.cn
 * @brief     Bluetooth LE MESH generic client and server model data struct and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MESH_GENERIC_MODEL_H__
#define __RTK_BT_MESH_GENERIC_MODEL_H__

#include <stdint.h>
#include <rtk_bt_mesh_def.h>

#include <rtk_bt_mesh_generic_onoff_model.h>

#define GENERIC_PROPERTY_DATA_MAX_LEN    0x10

/***************generic level*******************/
/***********client************/
/**
 * @typedef   rtk_bt_mesh_generic_level_client_model_act_t
 * @brief     BLE MESH generic level client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_SET,
    RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_DELTA_SET,
    RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_MOVE_SET,
	RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_level_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_client_model_evt_t
 * @brief     BLE MESH generic level client model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_level_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_get_t
 * @brief     BLE MESH generic level get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_level_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_set_t
 * @brief     BLE MESH generic level set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    int16_t level;
    uint8_t tid;
    bool optional;
    rtk_bt_mesh_generic_transition_time_t trans_time;
    uint8_t delay;
    bool ack;
} rtk_bt_mesh_generic_level_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_delta_set_t
 * @brief     BLE MESH generic level delta set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    int32_t delta_level;
    uint8_t tid;
    bool optional;
    rtk_bt_mesh_generic_transition_time_t trans_time;
    uint8_t delay;
    bool ack;
} rtk_bt_mesh_generic_level_delta_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_move_set_t
 * @brief     BLE MESH generic level move set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    int16_t delta_level;
    uint8_t tid;
    bool optional;
    rtk_bt_mesh_generic_transition_time_t trans_time;
    uint8_t delay;
    bool ack;
} rtk_bt_mesh_generic_level_move_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_client_status_t
 * @brief     BLE MESH generic level client model status evt message structure.
 */
typedef struct
{
    uint16_t src;
    int16_t present_level;
    bool optional;
    int16_t target_level;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_level_client_status_t;


/************server**************/
/**
 * @typedef   rtk_bt_mesh_generic_level_server_model_evt_t
 * @brief     BLE MESH generic level server model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_GET = 1,
    RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_SET,
    RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_SET_MOVE,
	RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_level_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_server_get_t
 * @brief     BLE MESH generic level get evt message structure.
 */
typedef struct
{
    int16_t *level;
} rtk_bt_mesh_generic_level_server_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_server_get_default_transition_time_t
 * @brief     BLE MESH generic level get default transition time evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_generic_level_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_server_set_t
 * @brief     BLE MESH generic level set evt message structure.
 */
typedef struct
{
    int16_t level;
    rtk_bt_mesh_generic_transition_time_t total_time;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_level_server_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_level_server_set_move_t
 * @brief     BLE MESH generic level set move evt message structure.
 */
typedef struct
{
    int16_t move_delta;
    int16_t target_level;
    rtk_bt_mesh_generic_transition_time_t total_time;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_level_server_set_move_t;

/***********generic power on off*************/
/****** client ********/
/**
 * @typedef   rtk_bt_mesh_generic_on_power_up_client_model_act_t
 * @brief     BLE MESH generic on power up client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_SET,
	RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_on_power_up_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_on_power_up_client_model_evt_t
 * @brief     BLE MESH generic on power up client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_on_power_up_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_on_power_up_get_t
 * @brief     BLE MESH generic on power up get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_on_power_up_get_t;

/**
 * @typedef rtk_bt_mesh_generic_on_power_up_t
 * @brief BLE MESH generic on power up set data structure
 */
typedef enum
{
    /* After being powered up, the element is in an off state. */
    RTK_BT_MESH_GENERIC_ON_POWER_UP_OFF,
    /* After being powered up, the element is in an On state and uses default state values. */
    RTK_BT_MESH_GENERIC_ON_POWER_UP_DEFAULT,
    /* If a transition was in progress when powered down, the element restores the target state when powered up.
       Otherwise the element restores the state it was in when powered down. */
    RTK_BT_MESH_GENERIC_ON_POWER_UP_RESTORE
} rtk_bt_mesh_generic_on_power_up_t;

/**
 * @typedef   rtk_bt_mesh_generic_on_power_up_set_t
 * @brief     BLE MESH generic on power up set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_generic_on_power_up_t on_power_up;
    bool ack;
} rtk_bt_mesh_generic_on_power_up_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_on_off_client_status_t
 * @brief     BLE MESH generic power on off client model status evt message structure.
 */
typedef struct {
	uint16_t src;
	rtk_bt_mesh_generic_on_power_up_t on_power_up;
} rtk_bt_mesh_generic_power_on_off_client_status_t;

/*******server*********/

/**
 * @typedef   rtk_bt_mesh_generic_power_on_off_server_model_evt_t
 * @brief     BLE MESH generic power on off server model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_power_on_off_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_on_off_setup_server_model_evt_t
 * @brief     BLE MESH generic power on off setup server model event definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL_SET = 1,
	RTK_BT_MESH_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_power_on_off_setup_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_on_off_server_get_t
 * @brief     BLE MESH generic power on off get evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_on_power_up_t *on_power_up;
} rtk_bt_mesh_generic_power_on_off_server_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_on_off_server_set_t
 * @brief     BLE MESH generic power on off set evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_on_power_up_t on_power_up;
} rtk_bt_mesh_generic_power_on_off_server_set_t;

/******power level********/
/*****client******/

/**
 * @typedef   rtk_bt_mesh_generic_power_level_client_model_act_t
 * @brief     BLE MESH generic power level client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_SET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_LAST_GET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_GET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_SET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_GET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_SET,
	RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_power_level_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_client_model_evt_t
 * @brief     BLE MESH generic power level client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_STATUS = 1,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_LAST_STATUS,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_STATUS,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_STATUS,
	RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_power_level_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_get_t
 * @brief     BLE MESH generic power level get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_power_level_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_set_t
 * @brief     BLE MESH generic power level set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t power;
    uint8_t tid;
    bool optional;
    rtk_bt_mesh_generic_transition_time_t trans_time;
    uint8_t delay;
    bool ack;
} rtk_bt_mesh_generic_power_level_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_last_get_t
 * @brief     BLE MESH generic power level last get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_power_level_last_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_default_get_t
 * @brief     BLE MESH generic power level default get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_power_level_default_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_default_set_t
 * @brief     BLE MESH generic power level default set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t power;
    bool ack;
} rtk_bt_mesh_generic_power_level_default_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_range_get_t
 * @brief     BLE MESH generic power level range get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_power_level_range_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_range_set_t
 * @brief     BLE MESH generic power level range set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t range_min;
    uint16_t range_max;
    bool ack;
} rtk_bt_mesh_generic_power_level_range_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_client_status_t
 * @brief     BLE MESH generic power level client model status evt message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t present_power;
    bool optional;
    uint16_t target_power;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_power_level_client_status_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_client_status_simple_t
 * @brief     BLE MESH generic power level client model simple status evt message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t power;
} rtk_bt_mesh_generic_power_level_client_status_simple_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_client_status_range_t
 * @brief     BLE MESH generic power level client model range status evt message structure.
 */
typedef struct
{
    uint16_t src;
    rtk_bt_mesh_generic_stat_t stat;
    uint16_t range_min;
    uint16_t range_max;
} rtk_bt_mesh_generic_power_level_client_status_range_t;

/****server****/

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_model_evt_t
 * @brief     BLE MESH generic power level server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_GET = 1,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_SET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_LAST_GET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_LAST_SET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_DEFAULT_GET,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_RANGE_GET,
	RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_power_level_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_setup_server_model_evt_t
 * @brief     BLE MESH generic power level setup server model evt definition.
 */
typedef enum {
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL_DEFAULT_SET = 1,
    RTK_BT_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL_RANGE_SET,
	RTK_BT_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_power_level_setup_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_get_t
 * @brief     BLE MESH generic power level server model get evt message structure.
 */
typedef struct
{
    uint16_t *power;
} rtk_bt_mesh_generic_power_level_server_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_get_range_t
 * @brief     BLE MESH generic power level server model get range evt message structure.
 */
typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} rtk_bt_mesh_generic_power_level_server_get_range_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_get_range_t
 * @brief     BLE MESH generic power level server model get range for direct calling evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_power_level_server_get_range_t *value;
} rtk_bt_mesh_generic_power_level_server_direct_get_range_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_get_default_transition_time_t
 * @brief     BLE MESH generic power level server model get default transition time evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_transition_time_t trans_time;
} rtk_bt_mesh_generic_power_level_server_get_default_transition_time_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_set_t
 * @brief     BLE MESH generic power level server model set evt message structure.
 */
typedef struct
{
    uint16_t power;
    rtk_bt_mesh_generic_transition_time_t total_time;
    rtk_bt_mesh_generic_transition_time_t remaining_time;
} rtk_bt_mesh_generic_power_level_server_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_set_last_t
 * @brief     BLE MESH generic power level server model set last evt message structure.
 */
typedef struct
{
    uint16_t power;
} rtk_bt_mesh_generic_power_level_server_set_last_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_set_default_t
 * @brief     BLE MESH generic power level server model set default evt message structure.
 */
typedef struct
{
    uint16_t power;
} rtk_bt_mesh_generic_power_level_server_set_default_t;

/**
 * @typedef   rtk_bt_mesh_generic_power_level_server_set_range_t
 * @brief     BLE MESH generic power level server model set range evt message structure.
 */
typedef struct
{
    uint16_t range_min;
    uint16_t range_max;
} rtk_bt_mesh_generic_power_level_server_set_range_t;

/********generic battery**********/
/**
 * @typedef   rtk_bt_mesh_generic_battery_client_model_act_t
 * @brief     BLE MESH generic battery client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_battery_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_battery_client_model_evt_t
 * @brief     BLE MESH generic battery client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_STATUS = 1,
	RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_battery_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_battery_get_t
 * @brief     BLE MESH generic battery get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_battery_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_battery_flags_t
 * @brief     BLE MESH generic battery flags structure.
 */
typedef struct
{
    uint8_t presence: 2;
    uint8_t indicator: 2;
    uint8_t charging: 2;
    uint8_t serviceability: 2;
} rtk_bt_mesh_generic_battery_flags_t;

/**
 * @typedef   rtk_bt_mesh_generic_battery_client_status_t
 * @brief     BLE MESH generic battery client status act message structure.
 */
typedef struct
{
    uint16_t src;
    uint8_t battery_level;
    uint32_t time_to_discharge;
    uint32_t time_to_charge;
    rtk_bt_mesh_generic_battery_flags_t flags;
} rtk_bt_mesh_generic_battery_client_status_t;

/**
 * @typedef   rtk_bt_mesh_generic_bttery_server_model_evt_t
 * @brief     BLE MESH generic battery server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_BATTERY_SERVER_MODEL_GET = 1,
	RTK_BT_MESH_GENERIC_BATTERY_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_battery_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_battery_server_get_t
 * @brief     BLE MESH generic battery server get status message structure.
 */
typedef struct
{
    uint8_t battery_level;
    uint32_t time_to_discharge;
    uint32_t time_to_charge;
    rtk_bt_mesh_generic_battery_flags_t flags;
} rtk_bt_mesh_generic_battery_server_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_battery_server_direct_get_t
 * @brief     BBLE MESH generic battery server get status for direct calling message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_battery_server_get_t *value;
} rtk_bt_mesh_generic_battery_server_direct_get_t;

/******generic location*******/
/**
 * @typedef   rtk_bt_mesh_generic_location_client_model_act_t
 * @brief     BLE MESH generic location client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_GET = 1,
    RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_SET,
    RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_GET,
    RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_SET,
	RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_location_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_client_model_evt_t
 * @brief     BLE MESH generic location client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_STATUS = 1,
    RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_STATUS,
	RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_location_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_global_get_t
 * @brief     BLE MESH generic location global get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_location_global_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_global_t
 * @brief     BLE MESH generic location global parameter structure.
 */
typedef struct
{
    int32_t global_latitude;
    int32_t global_longitude;
    int16_t global_altitude;
} _PACKED4_ rtk_bt_mesh_generic_location_global_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_global_set_t
 * @brief     BLE MESH generic location global set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_generic_location_global_t global;
    bool ack;
} rtk_bt_mesh_generic_location_global_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_local_get_t
 * @brief     BLE MESH generic location local get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_location_local_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_uncertainty_t
 * @brief     BLE MESH generic location uncertainty parameter structure.
 */
typedef union
{
    struct
    {
        uint16_t stationary: 1;
        uint16_t rfu: 7;
        uint16_t update_time: 4;
        uint16_t precision: 4;
    };
    uint16_t uncertainty;
} _PACKED4_ rtk_bt_mesh_generic_location_uncertainty_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_local_t
 * @brief     BLE MESH generic location local parameter structure.
 */
typedef struct
{
    int16_t local_north;
    int16_t local_east;
    int16_t local_altitude;
    uint8_t floor_num;
    rtk_bt_mesh_generic_location_uncertainty_t uncertainty;
} _PACKED4_ rtk_bt_mesh_generic_location_local_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_local_set_t
 * @brief     BLE MESH generic location local set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    rtk_bt_mesh_generic_location_local_t local;
    bool ack;
} rtk_bt_mesh_generic_location_local_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_client_status_global_t
 * @brief     BLE MESH generic location global status evt message structure.
 */
typedef struct
{
    uint16_t src;
    rtk_bt_mesh_generic_location_global_t global;
} rtk_bt_mesh_generic_location_client_status_global_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_client_status_local_t
 * @brief     BLE MESH generic location local status evt message structure.
 */
typedef struct
{
    uint16_t src;
    rtk_bt_mesh_generic_location_local_t local;
} rtk_bt_mesh_generic_location_client_status_local_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_client_model_evt_t
 * @brief     BLE MESH generic location client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_GLOBAL_GET = 1,
    RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_LOCAL_GET,
	RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_location_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_setup_server_model_evt_t
 * @brief     BLE MESH generic location setup server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_GLOBAL_SET = 1,
    RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_LOCAL_SET,
    RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_GLOBAL_GET,
    RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_LOCAL_GET,
	RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_location_setup_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_server_get_global_t
 * @brief     BLE MESH generic location global get evt message structure.
 */
typedef rtk_bt_mesh_generic_location_global_t rtk_bt_mesh_generic_location_server_get_global_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_server_direct_get_global_t
 * @brief     BLE MESH generic location global direct get evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_location_server_get_global_t *value;
} rtk_bt_mesh_generic_location_server_direct_get_global_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_server_get_local_t
 * @brief     BLE MESH generic location local get evt message structure.
 */
typedef rtk_bt_mesh_generic_location_local_t rtk_bt_mesh_generic_location_server_get_local_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_server_direct_get_local_t
 * @brief     BLE MESH generic location local direct get evt message structure.
 */
typedef struct
{
    rtk_bt_mesh_generic_location_server_get_local_t *value;
} rtk_bt_mesh_generic_location_server_direct_get_local_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_server_set_global_t
 * @brief     BLE MESH generic location global set evt message structure.
 */
typedef rtk_bt_mesh_generic_location_global_t rtk_bt_mesh_generic_location_server_set_global_t;

/**
 * @typedef   rtk_bt_mesh_generic_location_server_set_local_t
 * @brief     BLE MESH generic location local set evt message structure.
 */
typedef rtk_bt_mesh_generic_location_local_t rtk_bt_mesh_generic_location_server_set_local_t;

/*******generic property*******/
/**
 * @typedef   rtk_bt_mesh_generic_property_client_model_act_t
 * @brief     BLE MESH generic property client model act definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTIES_GET = 1,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_GET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_SET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTIES_GET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_GET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_SET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTIES_GET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_GET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_SET,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_CLIENT_PROPERTY_GET,
	RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_property_client_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_client_model_evt_t
 * @brief     BLE MESH generic property client model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTIES_STATUS = 1,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_STATUS,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTIES_STATUS,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_STATUS,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTIES_STATUS,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_STATUS,
    RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_CLIENT_PROPERTY_STATUS,
	RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_property_client_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_user_properties_get_t
 * @brief     BLE MESH generic user properties get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_user_properties_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_user_property_get_t
 * @brief     BLE MESH generic user property get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_generic_user_property_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_user_property_set_t
 * @brief     BLE MESH generic user property set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint8_t pvalue[GENERIC_PROPERTY_DATA_MAX_LEN];
    uint8_t value_len;
    bool ack;
} rtk_bt_mesh_generic_user_property_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_admin_properties_get_t
 * @brief     BLE MESH generic admin properties get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_admin_properties_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_admin_property_get_t
 * @brief     BLE MESH generic admin property get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_generic_admin_property_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_admin_property_set_t
 * @brief     BLE MESH generic admin property set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint8_t property_access;
    uint8_t pvalue[GENERIC_PROPERTY_DATA_MAX_LEN];
    uint8_t value_len;
    bool ack;
} rtk_bt_mesh_generic_admin_property_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_manu_properties_get_t
 * @brief     BLE MESH generic manufacturer properties get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
} rtk_bt_mesh_generic_manu_properties_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_manu_property_get_t
 * @brief     BLE MESH generic manufacturer property get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_generic_manu_property_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_manu_property_set_t
 * @brief     BLE MESH generic manufacturer property set act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
    uint8_t property_access;
    bool ack;
} rtk_bt_mesh_generic_manu_property_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_client_properties_get_t
 * @brief     BLE MESH generic client properties get act message structure.
 */
typedef struct {
	uint16_t dst;
	uint16_t app_key_index;
    uint16_t property_id;
} rtk_bt_mesh_generic_client_properties_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_properties_client_status_t
 * @brief     BLE MESH generic client properties status evt message structure.
 */
typedef struct
{
    uint16_t src;
    uint8_t num_ids;
    uint16_t *pproperty_ids;
} rtk_bt_mesh_generic_properties_client_status_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_client_status_t
 * @brief     BLE MESH generic client property status evt message structure.
 */
typedef struct
{
    uint16_t src;
    uint16_t property_id;
    uint8_t property_access;
    uint16_t value_len;
    uint8_t *pproperty_value;
} rtk_bt_mesh_generic_property_client_status_t;

/**
 * @typedef   rtk_bt_mesh_generic_user_property_server_model_evt_t
 * @brief     BLE MESH generic user property server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_PROPERTY_SET = 1,
    RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_PARAMETER_GET,
    RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_VALUE_GET,
    RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_NUM_IDS_GET,
    RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_IDS_VALUE_GET,
    RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_user_property_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_user_property_server_model_act_t
 * @brief     BLE MESH generic user property server model act definition.
 */
typedef enum {
    RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_user_property_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_db_t
 * @brief     BLE MESH generic property server database structure.
 */
typedef struct
{
    uint16_t property_id;
    uint8_t property_access;
    uint8_t *pproperty_value; //!< variable length
    uint16_t value_len;
} rtk_bt_mesh_generic_property_db_t;

/**
 * @typedef   rtk_bt_mesh_generic_user_property_server_set_t
 * @brief     BLE MESH generic user property server set evt message structure.
 */
typedef struct
{
    uint16_t property_id;
    /*
     * just notify property value changed in database,
     * do not need to modify it, just use it if needed
     */
    uint8_t *pproperty_value;
    uint16_t value_len;
} rtk_bt_mesh_generic_user_property_server_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_set_t
 * @brief     BLE MESH generic property server parameter message structure.
 */
typedef struct {
	bool pdb;//check wehther pdb is NULL
	uint8_t property_access;
	uint16_t value_len;
} rtk_bt_mesh_generic_property_server_para_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_para_get_t
 * @brief     BLE MESH generic property server get parameter message structure.
 */
typedef struct {
	uint16_t property_id;
	rtk_bt_mesh_generic_property_server_para_t *pvalue;
} rtk_bt_mesh_generic_property_server_para_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_value_get_t
 * @brief     BLE MESH generic property server get value message structure.
 */
typedef struct {
	uint16_t property_id;
	uint16_t value_len;
	uint8_t *pproperty_value;
} rtk_bt_mesh_generic_property_server_value_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_num_ids_t
 * @brief     BLE MESH generic property server num ids message structure.
 */
typedef struct {
	bool pdb;//check wehther pdb is NULL
	uint8_t num_ids;
} rtk_bt_mesh_generic_property_server_num_ids_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_num_ids_get_t
 * @brief     BLE MESH generic property server num ids get message structure.
 */
typedef struct {
	rtk_bt_mesh_generic_property_server_num_ids_t *pvalue;
} rtk_bt_mesh_generic_property_server_num_ids_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_num_ids_value_get_t
 * @brief     BLE MESH generic user property server num ids value get message structure.
 */
typedef struct {
	uint8_t num_ids;
	uint16_t *property_ids;
} rtk_bt_mesh_generic_property_server_num_ids_value_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_admin_property_server_model_evt_t
 * @brief     BLE MESH generic admin property server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_PROPERTY_SET = 1,
    RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_PARAMETER_GET,
    RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_VALUE_GET,
    RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_NUM_IDS_GET,
    RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_IDS_VALUE_GET,
    RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_admin_property_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_admin_property_server_model_act_t
 * @brief     BLE MESH generic admin property server model act definition.
 */
typedef enum {
    RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_admin_property_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_manu_property_server_model_evt_t
 * @brief     BLE MESH generic manu property server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_PROPERTY_SET = 1,
    RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_PARAMETER_GET,
    RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_VALUE_GET,
    RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_NUM_IDS_GET,
    RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_IDS_VALUE_GET,
    RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_manu_property_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_manu_property_server_model_act_t
 * @brief     BLE MESH generic manu property server model act definition.
 */
typedef enum {
    RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_ACT_DB_SET = 1,
	RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_ACT_MAX,
} rtk_bt_mesh_generic_manu_property_server_model_act_t;

/**
 * @typedef   rtk_bt_mesh_generic_manu_property_server_set_t
 * @brief     BLE MESH generic manufacturer property server set message structure.
 */
typedef struct {
	uint16_t property_id;
	/*
	 * just notify property access changed in database,
	 * do not need to modify it, just use it if needed
	 */
	uint8_t property_access;
} rtk_bt_mesh_generic_manu_property_server_set_t;

/**
 * @typedef   rtk_bt_mesh_generic_client_property_server_model_evt_t
 * @brief     BLE MESH generic client property server model evt definition.
 */
typedef enum {
	RTK_BT_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL_PROPERTY_GET = 1,
    RTK_BT_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL_EVT_MAX,
} rtk_bt_mesh_generic_client_property_server_model_evt_t;

/**
 * @typedef   rtk_bt_mesh_generic_client_property_server_direct_get_t
 * @brief     BLE MESH generic client property server get for direct calling message structure.
 */
typedef struct {
	uint16_t property_id_start;
    uint8_t *value;
} rtk_bt_mesh_generic_client_property_server_direct_get_t;

/**
 * @typedef   rtk_bt_mesh_generic_property_server_db_set_t
 * @brief     BLE MESH generic property server database set message structure.
 */
typedef struct {
	uint16_t value_len;
	uint8_t p_data[GENERIC_PROPERTY_DATA_MAX_LEN * 4];
} rtk_bt_mesh_generic_property_server_db_set_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  ble_mesh_generic_model BT LE Mesh Generic Model APIs
 * @brief     BT LE Mesh Generic Model related function APIs
 * @ingroup   ble_mesh_api_group
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_mesh_generic_level_client_get(rtk_bt_mesh_generic_level_get_t *level_get)
 * @brief     Send generic level get message to generic level server, will cause event @ref RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_GET
 * @param[in] level_get: generic level get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_level_client_get(rtk_bt_mesh_generic_level_get_t *level_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_level_client_set(rtk_bt_mesh_generic_level_set_t *level_set)
 * @brief     Send generic level set message to generic level server, will cause event @ref RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_SET
 * @param[in] level_set: generic level set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_level_client_set(rtk_bt_mesh_generic_level_set_t *level_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_level_client_move_set(rtk_bt_mesh_generic_level_move_set_t *move_set)
 * @brief     Send generic level move set message to generic level server, will cause event @ref RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_MOVE_SET
 * @param[in] move_set: generic move set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_level_client_move_set(rtk_bt_mesh_generic_level_move_set_t *move_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_level_client_delta_set(rtk_bt_mesh_generic_level_delta_set_t *delta_set)
 * @brief     Send generic level delta set message to generic level server, will cause event @ref RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_DELTA_SET
 * @param[in] delta_set: generic delta set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_level_client_delta_set(rtk_bt_mesh_generic_level_delta_set_t *delta_set);

/********generic power on off*********/

/**
 * @fn        uint16_t rtk_bt_mesh_generic_on_power_up_client_get(rtk_bt_mesh_generic_on_power_up_get_t *power_get)
 * @brief     Send generic on power up get message to generic power on off server, will cause event @ref RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_GET
 * @param[in] power_get: generic on power up get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_on_power_up_client_get(rtk_bt_mesh_generic_on_power_up_get_t *power_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_on_power_up_client_set(rtk_bt_mesh_generic_on_power_up_set_t *power_set)
 * @brief     Send generic on power up set message to generic power on off setup server, will cause event @ref RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_SET
 * @param[in] power_set: generic on power up set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_on_power_up_client_set(rtk_bt_mesh_generic_on_power_up_set_t *power_set);

/***********generic power level*************/

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_get(rtk_bt_mesh_generic_power_level_get_t *power_level_get)
 * @brief     Send generic power level get message to generic power level server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_GET
 * @param[in] power_level_get: generic power level get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_get(rtk_bt_mesh_generic_power_level_get_t *power_level_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_set(rtk_bt_mesh_generic_power_level_set_t *power_level_set)
 * @brief     Send generic power level set message to generic power level server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_SET
 * @param[in] power_level_set: generic power level set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_set(rtk_bt_mesh_generic_power_level_set_t *power_level_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_last_get(rtk_bt_mesh_generic_power_level_last_get_t *power_level_last_get)
 * @brief     Send generic power level last get message to generic power level server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_LAST_GET
 * @param[in] power_level_last_get: generic power level last get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_last_get(rtk_bt_mesh_generic_power_level_last_get_t *power_level_last_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_default_get(rtk_bt_mesh_generic_power_level_default_get_t *power_level_default_get)
 * @brief     Send generic power level default get message to generic power level server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_GET
 * @param[in] power_level_default_get: generic power level default get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_default_get(rtk_bt_mesh_generic_power_level_default_get_t *power_level_default_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_default_set(rtk_bt_mesh_generic_power_level_default_set_t *power_level_default_set)
 * @brief     Send generic power level default set message to generic power level setup server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_SET
 * @param[in] power_level_default_set: generic power level default set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_default_set(rtk_bt_mesh_generic_power_level_default_set_t *power_level_default_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_range_get(rtk_bt_mesh_generic_power_level_range_get_t *power_level_range_get)
 * @brief     Send generic power level range get message to generic power level server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_GET
 * @param[in] power_level_range_get: generic power level range get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_range_get(rtk_bt_mesh_generic_power_level_range_get_t *power_level_range_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_power_level_client_range_set(rtk_bt_mesh_generic_power_level_range_set_t *power_level_range_set)
 * @brief     Send generic power level range set message to generic power level setup server, will cause event @ref RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_SET
 * @param[in] power_level_range_set: generic power level range set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_power_level_client_range_set(rtk_bt_mesh_generic_power_level_range_set_t *power_level_range_set);

/*******generic battery********/
/**
 * @fn        uint16_t rtk_bt_mesh_generic_battery_client_get(rtk_bt_mesh_generic_battery_get_t *battery_get)
 * @brief     Send generic battery get message to generic battery server, will cause event @ref RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_GET
 * @param[in] battery_get: generic battery get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_battery_client_get(rtk_bt_mesh_generic_battery_get_t *battery_get);

/********generic location**********/
/**
 * @fn        uint16_t rtk_bt_mesh_generic_location_client_global_get(rtk_bt_mesh_generic_location_global_get_t *global_get)
 * @brief     Send generic location global get message to generic location server, will cause event @ref RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_GET
 * @param[in] global_get: generic location global get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_location_client_global_get(rtk_bt_mesh_generic_location_global_get_t *global_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_location_client_global_set(rtk_bt_mesh_generic_location_global_set_t *global_set)
 * @brief     Send generic location global set message to generic location setup server, will cause event @ref RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_SET
 * @param[in] global_set: generic location global set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_location_client_global_set(rtk_bt_mesh_generic_location_global_set_t *global_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_location_client_local_get(rtk_bt_mesh_generic_location_local_get_t *local_get)
 * @brief     Send generic location local get message to generic location server, will cause event @ref RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_GET
 * @param[in] local_get: generic location local get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_location_client_local_get(rtk_bt_mesh_generic_location_local_get_t *local_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_location_client_local_set(rtk_bt_mesh_generic_location_local_set_t *local_set)
 * @brief     Send generic location local set message to generic location setup server, will cause event @ref RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_SET
 * @param[in] local_set: generic location local set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_location_client_local_set(rtk_bt_mesh_generic_location_local_set_t *local_set);

/*******generic property*********/
/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_user_properties_get(rtk_bt_mesh_generic_user_properties_get_t *user_get)
 * @brief     Send generic user properties get message to generic user property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTIES_GET
 * @param[in] user_get: generic user properties get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_user_properties_get(rtk_bt_mesh_generic_user_properties_get_t *user_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_user_property_get(rtk_bt_mesh_generic_user_property_get_t *user_property_get)
 * @brief     Send generic user property get message to generic user property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_GET
 * @param[in] user_property_get: generic user property get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_user_property_get(rtk_bt_mesh_generic_user_property_get_t *user_property_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_user_property_set(rtk_bt_mesh_generic_user_property_set_t *user_property_set)
 * @brief     Send generic user property set message to generic user property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_SET
 * @param[in] user_property_set: generic user property set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_user_property_set(rtk_bt_mesh_generic_user_property_set_t *user_property_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_admin_properties_get(rtk_bt_mesh_generic_admin_properties_get_t *admin_get)
 * @brief     Send generic admin properties get message to generic admin property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTIES_GET
 * @param[in] admin_get: generic admin properties get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_admin_properties_get(rtk_bt_mesh_generic_admin_properties_get_t *admin_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_admin_property_get(rtk_bt_mesh_generic_admin_property_get_t *admin_property_get)
 * @brief     Send generic admin property get message to generic admin property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_GET
 * @param[in] admin_property_get: generic admin property get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_admin_property_get(rtk_bt_mesh_generic_admin_property_get_t *admin_property_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_admin_property_set(rtk_bt_mesh_generic_admin_property_set_t *admin_property_set)
 * @brief     Send generic admin property set message to generic admin property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_SET
 * @param[in] admin_property_set: generic admin property set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_admin_property_set(rtk_bt_mesh_generic_admin_property_set_t *admin_property_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_manu_properties_get(rtk_bt_mesh_generic_manu_properties_get_t *manu_get)
 * @brief     Send generic manufacturer properties get message to generic manufacturer property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTIES_GET
 * @param[in] manu_get: generic manufacturer properties get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_manu_properties_get(rtk_bt_mesh_generic_manu_properties_get_t *manu_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_manu_property_get(rtk_bt_mesh_generic_manu_property_get_t *manu_property_get)
 * @brief     Send generic manufacturer property get message to generic manufacturer property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_GET
 * @param[in] manu_property_get: generic manufacturer property get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_manu_property_get(rtk_bt_mesh_generic_manu_property_get_t *manu_property_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_manu_property_set(rtk_bt_mesh_generic_manu_property_set_t *manu_property_set)
 * @brief     Send generic manufacturer property set message to generic manufacturer property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_SET
 * @param[in] manu_property_set: generic manufacturer property set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_manu_property_set(rtk_bt_mesh_generic_manu_property_set_t *manu_property_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_client_properties_get(rtk_bt_mesh_generic_client_properties_get_t *client_get)
 * @brief     Send generic client properties get message to generic client property server, will cause event @ref RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_CLIENT_PROPERTY_GET
 * @param[in] client_get: generic client properties get message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_client_properties_get(rtk_bt_mesh_generic_client_properties_get_t *client_get);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_user_db_set(rtk_bt_mesh_generic_property_server_db_set_t *db_set)
 * @brief     Send generic user property database set message to set user property database, will cause event @ref RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: generic database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_user_db_set(rtk_bt_mesh_generic_property_server_db_set_t *db_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_admin_db_set(rtk_bt_mesh_generic_property_server_db_set_t *db_set)
 * @brief     Send generic admin property database set message to set admin property database, will cause event @ref RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: generic database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_admin_db_set(rtk_bt_mesh_generic_property_server_db_set_t *db_set);

/**
 * @fn        uint16_t rtk_bt_mesh_generic_property_client_manu_db_set(rtk_bt_mesh_generic_property_server_db_set_t *db_set)
 * @brief     Send generic manu property database set message to set manu property database, will cause event @ref RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_ACT_DB_SET
 * @param[in] db_set: generic database set message structure
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_mesh_generic_property_client_manu_db_set(rtk_bt_mesh_generic_property_server_db_set_t *db_set);

/**
 * @}
 */

#endif