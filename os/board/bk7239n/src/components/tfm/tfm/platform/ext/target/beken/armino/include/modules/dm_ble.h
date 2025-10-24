// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#pragma once

#include <modules/dm_ble_types.h>

#ifdef __cplusplus
extern"C" {
#endif


/**
 * @brief dm_ble APIs Version 1.0
 * @defgroup bk_dm_ble_api_v1 New dm ble api group
 * @{
 */

/**
 * @brief     Register dm_ble event notification callback
 *
 * @param
 *    - func: event callback
 *
 * @attention 1. you must regist it, otherwise you cant get any event !
 * @attention 2. you must regist it before bk_ble_create_db, otherwise you cant get BLE_5_CREATE_DB event
 *
 * User example:
 * @code

 * @endcode
 * @return
 *    - void
 */
void bk_ble_set_event_callback(ble_event_cb_t func);

/**
  @brief     Set adv param

  @param param: adv param see @ref ble_adv_parameter_t
  @param callback: register a callback for this action, ble_cmd_t: BLE_CREATE_ADV

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_set_advertising_params(
    ble_adv_parameter_t *param,
    ble_cmd_cb_t callback);

/**
  @brief     Set adv data

  @param adv_len: adv data len
  @param adv_buff: adv data
  @param callback: register a callback for this action, ble_cmd_t: BLE_SET_ADV_DATA

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_set_advertising_data(
    uint8_t adv_len,
    uint8_t *adv_buff,
    ble_cmd_cb_t callback);


/**
  @brief     Set adv scan response data.

  @param scan_response_data_length: data len
  @param scan_response_data: data
  @param callback: register a callback for this action, ble_cmd_t: BLE_SET_RSP_DATA
  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_set_scan_response_data(
    uint8_t  scan_response_data_length,
    uint8_t *scan_response_data,
    ble_cmd_cb_t callback);


/**
  @brief     Set enable/disable adv

  @param enable: enable or disable
  @param callback: register a callback for this action, ble_cmd_t: BLE_START_ADV(enable) BLE_STOP_ADV(disable)
  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_set_advertising_enable(
    uint8_t    enable,
    ble_cmd_cb_t callback);

/**
  @brief     disconnect ble connection

  @param addr: peer addr
  @param callback: register a callback for this action, ble_cmd_t: BLE_CONN_DIS_CONN
  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_disconnect_connection(bd_addr_t *addr, ble_cmd_cb_t callback);


/**
  @brief     add a primary service in gatt db

  @param service_info: service info
  @param num_attr_handles: how many attr in this service
  @param[out] service_handle: out put a handle to user.

  User example:
  @code
    GATT_DB_SERVICE_INFO service_info;
    uint16_t num_attr_handles;
    uint16_t service_handle;

    service_info.is_primary = 1;
    service_info.uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_BOARDING_SERVICE_UUID;
    service_info.link_req = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req = GATT_DB_SER_NO_SECURITY_PROPERTY;
    num_attr_handles = 30U;
    retval = bk_ble_gatt_db_add_service
               (
                   &service_info,
                   num_attr_handles,
                   &service_handle
               );
  @endcode
  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_gatt_db_add_service
(
    /* IN */   GATT_DB_SERVICE_INFO *service_info,
    /* IN */   uint16_t                 num_attr_handles,
    /* OUT */  uint16_t                *service_handle
);

/**
   @brief     add a characteristic in a gatt service

   @param service_handle: service handle, get from bk_ble_gatt_db_add_service
   @param char_uuid: characteristic's uuid, in ATT_UUID16 or ATT_UUID128, see @ref GATT_DB_UUID_TYPE
   @param perm: permission, such as GATT_DB_PERM_READ GATT_DB_PERM_WRITE
   @param property: property, such as @ref GATT_DB_CHAR_READ_PROPERTY @ref GATT_DB_CHAR_NOTIFY_PROPERTY
   @param char_value: att data, see @ref ATT_VALUE
   @param[out] char_handle: output a handle

   User example:
   @code
     GATT_DB_UUID_TYPE char_uuid;
     uint16_t perm;
     uint16_t property;
     ATT_VALUE char_value;
     UINT16 char_handle;

     char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
     char_uuid.uuid.uuid_16 = GATT_BOARDING_NOTIFY_CHARACTERISTIC;
     perm = GATT_DB_PERM_READ;
     property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
     char_value.val = s_boarding_notify;
     char_value.len = sizeof(s_boarding_notify);
     char_value.actual_len = char_value.len;
     retval = bk_ble_gatt_db_add_characteristic
                    (
                        service_handle,
                        &char_uuid,
                        perm,
                        property,
                        &char_value,
                        &char_handle
                    );
   @endcode

   @attention 1.you must wait callback status, 0 mean success.
   @return
      - BK_ERR_BLE_SUCCESS: succeed
      - others: other errors.
 */
ble_err_t bk_ble_gatt_db_add_characteristic
(
    /* IN */  uint16_t              service_handle,
    /* IN */  GATT_DB_UUID_TYPE *char_uuid,
    /* IN */  uint16_t              perm,
    /* IN */  uint16_t              property,
    /* IN */  ATT_VALUE          *char_value,
    /* OUT */ uint16_t             *char_handle
);



/**
   @brief     add a characteristic descriptor in a characteristic

   @param service_handle: service handle, get from bk_ble_gatt_db_add_service
   @param char_handle: get from bk_ble_gatt_db_add_characteristic
   @param desc_uuid: characteristic's uuid, in ATT_UUID16 or ATT_UUID128, see @ref GATT_DB_UUID_TYPE
   @param perm: permission, such as @ref GATT_DB_PERM_READ @ref GATT_DB_PERM_WRITE
   @param desc_value: att data, see @ref ATT_VALUE

   User example:
   @code
     GATT_DB_UUID_TYPE    desc_uuid;
     ATT_VALUE            desc_value;

     uint8_t cccd_value[2U]    = { 0x00U, 0x00U };

     desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
     desc_uuid.uuid.uuid_16 = GATT_CLIENT_CONFIG;

     perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);

     desc_value.val         = cccd_value;
     desc_value.len         = 2U;
     desc_value.actual_len  = desc_value.len;

     // Add descriptor CCCD
     retval = bk_ble_gatt_db_add_characteristic_descriptor
              (
                  service_handle,
                  boarding_env.chara_notify_handle,
                  &desc_uuid,
                  perm,
                  &desc_value
              );
   @endcode

   @attention 1.you must wait callback status, 0 mean success.
   @return
      - BK_ERR_BLE_SUCCESS: succeed
      - others: other errors.
 */

ble_err_t bk_ble_gatt_db_add_characteristic_descriptor
(
    /* IN */  uint16_t              service_handle,
    /* IN */  uint16_t              char_handle,
    /* IN */  GATT_DB_UUID_TYPE *desc_uuid,
    /* IN */  uint16_t              perm,
    /* IN */  ATT_VALUE          *desc_value
);

/**
  @brief     call when gatt db add completed

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_gatt_db_add_completed(void);

/**
  @brief     set callback, when recv write read req
  @param hndlr_cb: callback, see @ref ble_gatt_db_callback_t

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_gatt_db_set_callback(ble_gatt_db_callback_t hndlr_cb);

/**
  @brief     When recv GATT_DB_CHAR_PEER_READ_REQ, async response

  @param conn_handle: connection handle
  @param value: a point to buffer
  @param length: buffer len

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_gatt_read_resp(uint8_t conn_handle, uint8_t *value, uint16_t length);




/**
  @brief     send notify when recv enable notify

  @param conn_handle: connection handle
  @param adv service_handle: from bk_ble_gatt_db_add_service
  @param char_handle: get from bk_ble_gatt_db_add_characteristic
  @param data: a point to buffer
  @param len: buffer len

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_send_notify
(
    uint8_t conn_handle,
    uint16_t service_handle,
    uint16_t char_handle,
    uint8_t *data,
    uint16_t len
);



/**
  @brief     get db value
  @param handle: handle
  @param attr_value: attr

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_gatt_get_char_val(GATT_DB_HANDLE *handle, ATT_VALUE *attr_value);

/*
 * @}
 */

#ifdef __cplusplus
}
#endif

