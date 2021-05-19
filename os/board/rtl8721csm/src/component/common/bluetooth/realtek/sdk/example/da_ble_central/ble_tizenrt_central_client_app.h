/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_central_client_app.h
   * @brief     This file handles BLE central client application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _BLE_TIZENRT_CENTRAL_CLIENT_APP_H_
#define _BLE_TIZENRT_CENTRAL_CLIENT_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <profile_client.h>
#include <app_msg.h>
#include "da_ble_client.h"
#include <gap_msg.h>
#include <tizenrt_ble_common.h>

#define BD_ADDR_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BD_ADDR_ARG(x) (x)[5],(x)[4],(x)[3],(x)[2],(x)[1],(x)[0]
#define UUID_128_FORMAT "0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X"
#define UUID_128(x)  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]

#define RTK_DEBUG_ON 0
#if RTK_DEBUG_ON
#define debug_print printf
#else
#if defined CONFIG_AMEBAD_BLE_SCATTERNET && CONFIG_AMEBAD_BLE_SCATTERNET
extern void print_no_combo(const char* format, ...);
#define debug_print print_no_combo
#else
extern void print_no_client(const char* format, ...);
#define debug_print print_no_client
#endif
#endif

typedef struct
{
  uint8_t *addr;
  bool is_secured_connect;
} BLE_TIZENRT_BOND_REQ;

typedef enum
{
	BLE_TIZENRT_BONDED_MSG,
	BLE_TIZENRT_CONNECTED_MSG,
  BLE_TIZENRT_SCAN_STATE_MSG,
	BLE_TIZENRT_SCANED_DEVICE_MSG,
  BLE_TIZENRT_DISCONNECTED_MSG,
  BLE_TIZENRT_NOTIFICATION_MSG,
  BLE_TIZENRT_READ_RESULT_MSG,
	BLE_TIZENRT_APP_MSG_MAX
} BLE_TIZENRT_CLIENT_APP_CALLBACK_TYPE;

typedef struct
{
  uint16_t cause;
  da_ble_client_data_info read_data;
} T_TIZENRT_CLIENT_READ_RESULT;

typedef struct
{
  da_ble_client_operation_handle_parm handle;
  da_ble_client_data_info noti_data;
} T_TIZENRT_CLIENT_NOTIFICATION;


typedef struct
{
  uint8_t conn_id;
  uint16_t att_handle;
} BLE_TIZENRT_READ_PARAM;

typedef struct
{
  uint8_t conn_id;
  uint16_t att_handle;
  uint8_t* data;
	uint16_t length;
} BLE_TIZENRT_WRITE_PARAM;

typedef struct
{
  uint8_t conn_id;
} BLE_TIZENRT_ENABLE_NOTIFY_PARAM;

typedef struct
{
  uint8_t conn_id;
} T_TIZENRT_BOND_PARAM;

typedef struct
{
  uint8_t remote_bd[GAP_BD_ADDR_LEN];
  T_GAP_REMOTE_ADDR_TYPE remote_bd_type;
} T_TIZENRT_DELETE_BOND_PARAM;

typedef struct
{
  uint8_t conn_id;
} T_TIZENRT_DISCONN_PARAM;

typedef struct
{
  uint8_t remote_bd[GAP_BD_ADDR_LEN];
  T_GAP_REMOTE_ADDR_TYPE remote_bd_type;
  uint16_t conn_interval;
  uint16_t conn_latency;
} T_TIZENRT_CONN_PARAM;

typedef enum
{
	BLE_TIZENRT_START_SCAN,
	BLE_TIZENRT_STOP_SCAN,
  BLE_TIZENRT_CONNECT,
	BLE_TIZENRT_DISCONNECT,
  BLE_TIZENRT_BOND,
  BLE_TIZENRT_READ,
  BLE_TIZENRT_WRITE,
  BLE_TIZENRT_WRITE_NO_RSP,
  BLE_TIZENRT_ENABLE_NOTIFY,
  BLE_TIZENRT_DELETE_BOND,
  BLE_TIZENRT_CLEAR_ALL_BONDS,
  BLE_TIZENRT_CLIENT_MSG_MAX
} BLE_TIZENRT_CLIENT_MSG_TYPE;

/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_CLIENT_ID   ble_tizenrt_central_gcs_client_id;         /**< General Common Services client client id*/

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ble_tizenrt_central_app_handle_io_msg(T_IO_MSG io_msg);
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT ble_tizenrt_central_app_gap_callback(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT ble_tizenrt_central_gcs_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

void ble_tizenrt_central_send_msg(uint16_t sub_type, void *arg);

#ifdef __cplusplus
}
#endif
#endif
