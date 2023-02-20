/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_app.h
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _TIZENRT_PERIPHERAL_APP__
#define _TIZENRT_PERIPHERAL_APP__

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_le.h>
#include <profile_server.h>
#include <tinyara/net/if/ble.h>
#include <tizenrt_ble_common.h>
#include <osdep_service.h>
/*============================================================================*
 *                              Variables
 *============================================================================*/

/*============================================================================*
 *                              Functions
 *============================================================================*/
#define debug_print blevdbg
//#define CONFIG_DEBUG_SCAN_INFO

typedef enum
{
    BLE_TIZENRT_MSG_START_ADV = 12,
    BLE_TIZENRT_MSG_STOP_ADV,
    BLE_TIZENRT_MSG_DISCONNECT,
    BLE_TIZENRT_MSG_NOTIFY,
    BLE_TIZENRT_MSG_INDICATE,
    BLE_TIZENRT_MSG_DELETE_BOND,
    BLE_TIZENRT_MSG_DELETE_BOND_ALL,
    BLE_TIZENRT_SERVER_MSG_MAX
} BLE_TIZENRT_SERVER_MSG_TYPE;

typedef enum
{
	BLE_TIZENRT_CALLBACK_TYPE_CONN = 8,
    BLE_TIZENRT_CALLBACK_TYPE_PROFILE,
    BLE_TIZENRT_CALLBACK_TYPE_MTU_UPDATE,
    BLE_TIZENRT_CALLBACK_TYPE_MAX,
} BLE_TIZENRT_CALLBACK_TYPE;

typedef struct
{
    trble_attr_handle att_handle;
    trble_conn_handle conn_id;
    uint8_t	 *data;
    uint16_t len;
} T_TIZENRT_NOTIFY_PARAM;

typedef struct
{
    trble_attr_handle att_handle;
    trble_conn_handle conn_id;
    uint8_t	 *data;
    uint16_t len;
} T_TIZENRT_INDICATE_PARAM;

typedef struct
{
    uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN];
} T_TIZENRT_DIRECT_ADV_PARAM;

typedef struct
{
    bool flag;
    T_GAP_CAUSE result;
    uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN];
} T_TIZENRT_SERVER_DELETE_BOND_PARAM;

typedef struct
{
    trble_attr_cb_type_e type;
    trble_conn_handle conn_id;
    trble_attr_handle att_handle;
    void *arg;
    trble_server_cb_t cb;
} T_TIZENRT_PROFILE_CALLBACK_DATA;

typedef struct
{
    trble_conn_handle conn_id;
    trble_server_connection_type_e conn_type;
    uint8_t remote_bd[TRBLE_BD_ADDR_MAX_LEN];
} T_TIZENRT_CONNECTED_CALLBACK_DATA;

typedef struct
{
    trble_conn_handle conn_id;
    uint16_t mtu_size;
} T_TIZENRT_MTU_UDPATE_CALLBACK_DATA;


/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ble_tizenrt_app_handle_io_msg(T_IO_MSG io_msg);

/**
 * @brief    All the BT Profile service callback events are handled in this function
 * @note     Then the event handling function shall be called according to the
 *           service_id.
 * @param[in] service_id  Profile service ID
 * @param[in] p_data      Pointer to callback data
 * @return   Indicates the function call is successful or not
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT ble_tizenrt_app_profile_callback(T_SERVER_ID service_id, void *p_data);

/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT ble_tizenrt_app_gap_callback(uint8_t cb_type, void *p_cb_data);

bool ble_tizenrt_send_msg(uint16_t sub_type, void *arg);                                      

#ifdef __cplusplus
}
#endif
#endif
