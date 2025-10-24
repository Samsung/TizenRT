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

#ifndef INCLUDE_MODULES_BK_DM_BT_API_H_
#define INCLUDE_MODULES_BK_DM_BT_API_H_


#include <modules/dm_bt_types.h>

#ifdef __cplusplus
extern"C" {
#endif

/**
 * @brief           sets BT event callback function
 *
 * @param[in]       func : callback
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - BK_ERR_BT_FAIL: others
 */
bt_err_t bk_bt_gap_set_event_callback(bt_event_cb_t func);



/**
 * @brief           send linkkey when peer need authentication
 *
 * @param[in]       is_exist : if linkkey exist. If no exist, peer may trigger pair procedure
 * @param[in]       key : linkkey,
 *
 * @attention       key->addr must be valued
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - BK_ERR_BT_FAIL: others
 */
bt_err_t bk_bt_gap_linkkey_reply(uint8_t is_exist, bk_bt_linkkey_storage_t *key);


/**
 * @brief           sets the Bluetooth connectable, discoverable
 *
 * @param[in]       conn_mode : one of the enums of bk_bt_conn_mode_t
 *
 * @param[in]       disc_mode : one of the enums of bk_bt_disc_mode_t
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - BK_ERR_BT_FAIL: others
 */
bt_err_t bk_bt_gap_set_visibility(bk_bt_conn_mode_t conn_mode, bk_bt_disc_mode_t disc_mode);


/**
 * @brief set the class of device
 *
 * @param[in]       cod: Class of Device for the device(@see enum common_cod_t)
 *
 * @return
 *                 - BK_ERR_BT_SUCCESS: succeed
 *                 - others: fail
 */
bt_err_t bk_bt_gap_set_device_class(uint32_t cod);

/**
 * @brief set the local name
 *
 * @param[in]       name: pointer of name
 * @param[in]       len: the length of name
 *
 * @return
 *                 - BK_ERR_BT_SUCCESS: succeed
 *                 - others: fail
 */
bt_err_t bk_bt_gap_set_local_name(uint8_t *name, uint8_t len);

/**
 *
 * @brief     Initialize the bluetooth A2DP sink module.
 *
 * @param[in]   aac_supported: 1 means aac is supported, 0 means aac is not supported.
 *
 * @return
 *            - BK_ERR_BT_SUCCESS: the initialization request is successfully
 *            -  others: fail
 */
bt_err_t bk_bt_a2dp_sink_init(uint8_t aac_supported);


/**
 * @brief           Register application callback function to A2DP module. used by both A2DP source
 *                  and sink.
 *
 * @param[in]       callback: A2DP event callback function
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 */
bt_err_t bk_bt_a2dp_register_callback(bk_bt_a2dp_cb_t callback);

/**
 * @brief           Register A2DP sink data output function;
 *
 * @param[in]       callback: A2DP sink data callback function
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 */
bt_err_t bk_bt_a2dp_sink_register_data_callback(bk_bt_sink_data_cb_t callback);

/**
 *
 * @brief           Connect to remote bluetooth A2DP source device. This API must be called after
 *                  bk_bt_a2dp_sink_init().
 *
 * @param[in]       remote_bda: remote bluetooth device address
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 *
 */
bt_err_t bk_bt_a2dp_sink_connect(uint8_t *remote_bda);

/**
 *
 * @brief           Disconnect from the remote A2DP source device. This API must be called after
 *                  bk_bt_a2dp_sink_init().
 *
 * @param[in]       remote_bda: remote bluetooth device address
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 *
 */
bt_err_t bk_bt_a2dp_sink_disconnect(uint8_t *remote_bda);


#ifdef __cplusplus
}
#endif

#endif
