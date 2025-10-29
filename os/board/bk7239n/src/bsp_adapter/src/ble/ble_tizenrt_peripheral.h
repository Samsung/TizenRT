#pragma once

#include <tinyara/net/if/ble.h>

/**
 * @brief bk tizenrt adapter server deinit
 *
 * @param
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_deinit(void);

/**
 * @brief private adapter func, bk ble evt process in peripheral module
 *
 * @param
 *    - notice: ble evt type
 *    - param: param that assosicate with notice
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_peripheral_notice_cb(ble_notice_t notice, void *param);

/**
 * @brief private adapter func, report connect evt
 *
 * @param
 *    - conn_idx: connection handle
 *    - type: connection type, see TRBLE_SERVER_SM_CONNECTED
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_report_connected_evt(uint8_t conn_idx, uint8_t type);

/**
 * @brief private adapter func, report passkey evt
 *
 * @param
 *    - conn_idx: connection handle
 *    - passkey: passkey
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_report_passkey_evt(uint8_t conn_idx, uint32_t passkey);

/**
 * @brief private adapter func, set attr data
 *
 * @param
 *    - service_index: gatt service index
 *    - att_index: att index
 *    - buffer: point to buff
 *    - buffer_len: buffer len
 *    - buffer_max_len: buffer max len
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_attr_set_data_ptr_private(uint8_t service_index, uint8_t att_index,
        uint8_t *buffer, uint16_t buffer_len, uint16_t buffer_max_len);
