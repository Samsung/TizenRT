/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_scatternet_app.h
   * @brief     This file handles BLE central client application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _BLE_TIZENRT_SCATTERNET_APP_H_
#define _BLE_TIZENRT_SCATTERNET_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <profile_client.h>
#include <app_msg.h>
#include <tinyara/net/if/ble.h>
#include "profile_server.h"
#include <gap_msg.h>
#include <tizenrt_ble_common.h>
#include <osdep_service.h>

#define BD_ADDR_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BD_ADDR_ARG(x) (x)[5],(x)[4],(x)[3],(x)[2],(x)[1],(x)[0]
#define UUID_128_FORMAT "0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X"
#define UUID_128(x)  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]

#define debug_print blevdbg
//#define CONFIG_DEBUG_SCAN_INFO

/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_CLIENT_ID   ble_tizenrt_scatternet_gcs_client_id;         /**< General Common Services client client id*/

/*============================================================================*
 *                              Functions
 *============================================================================*/
T_APP_RESULT ble_tizenrt_scatternet_app_profile_callback(T_SERVER_ID service_id, void *p_data);
/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ble_tizenrt_scatternet_app_handle_io_msg(T_IO_MSG io_msg);
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */

void app_vendor_callback(uint8_t cb_type, void *p_cb_data);

T_APP_RESULT ble_tizenrt_scatternet_app_gap_callback(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT ble_tizenrt_scatternet_gcs_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

bool ble_tizenrt_scatternet_send_msg(uint16_t sub_type, void *arg);

#ifdef __cplusplus
}
#endif
#endif
