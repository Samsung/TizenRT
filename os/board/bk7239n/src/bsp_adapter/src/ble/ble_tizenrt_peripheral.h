#pragma once

#include <tinyara/net/if/ble.h>

/**
 * @brief bk tizenrt adapter server init
 *
 * @param
 *    - trble_server_init_config: server config
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_init(trble_server_init_config *config);

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
 * @brief bk tizenrt adapter server add config
 *
 * @param
 *    - trble_server_init_config: server config
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_add_config(trble_server_init_config *config);

/**
 * @brief bk tizenrt adapter server get profile count
 *
 * @param
 *
 * @return
 *    - registered gatt profile count
 */
uint16_t bktr_ble_server_get_profile_count(void);

/**
 * @brief bk tizenrt adapter server get init param ptr
 *
 * @param
 *
 * @return
 *    - pointer to trble_server_init_config held by adapter
 */
trble_server_init_config *bktr_ble_server_get_param(void);

/**
 * @brief bk tizenrt adapter server set attr data buffer ptr
 *
 * @param
 *    - attr_handle: gatt attribute handle
 *    - buffer: pointer to app write buffer
 *    - buffer_len: current data length
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_attr_set_peer_read_data_ptr(trble_attr_handle attr_handle, uint8_t *buffer, uint16_t buffer_len);

/**
 * @brief bk tizenrt adapter server get attr data buffer ptr
 *
 * @param
 *    - attr_handle: gatt attribute handle
 *    - buffer: out, pointer to data
 *    - buffer_len: out, current length
 *    - buffer_max_len: out, buffer capacity
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_attr_get_data_ptr(trble_attr_handle attr_handle, uint8_t **buffer, uint16_t *buffer_len, uint16_t *buffer_max_len);

/**
 * @brief bk tizenrt adapter server send notify or indicate
 *
 * @param
 *    - attr_handle: gatt attribute handle
 *    - con_handle: connection handle
 *    - buffer: pdu data
 *    - buffer_len: pdu length
 *    - is_notify: non-zero notify, zero indicate
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *buffer, uint16_t buffer_len, uint8_t is_notify);

/**
 * @brief bk tizenrt adapter server disconnect link
 *
 * @param
 *    - conn_handle: connection handle
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_disconnect(trble_conn_handle conn_handle);

/**
 * @brief bk tizenrt adapter server get peer bd addr by conn handle
 *
 * @param
 *    - conn_handle: connection handle
 *    - bd_addr: out, peer address buffer
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_get_mac_addr_by_conn_handle(trble_conn_handle conn_handle, uint8_t *bd_addr);

/**
 * @brief bk tizenrt adapter server get conn handle by peer bd addr
 *
 * @param
 *    - bd_addr: peer address
 *    - con_handle: out, connection handle
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_get_conn_handle_by_addr(uint8_t *bd_addr, trble_conn_handle *con_handle);

/**
 * @brief bk tizenrt adapter server set gap device name
 *
 * @param
 *    - len: name length
 *    - device_name: name bytes
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_set_gap_device_name(uint8_t len, uint8_t *device_name);

/**
 * @brief bk tizenrt adapter server get indicate pending queue count
 *
 * @param
 *    - con_handle: pointer to connection handle
 *    - count: out, pending indicate count
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_indicate_queue_count(trble_conn_handle *con_handle, uint8_t *count);

/**
 * @brief bk tizenrt adapter server set attr reject app error code
 *
 * @param
 *    - attr_handle: gatt attribute handle
 *    - app_errorcode: application error code for att response
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_attr_reject(trble_attr_handle attr_handle, uint8_t app_errorcode);

/**
 * @brief private adapter func, bk ble evt process in peripheral module
 *
 * @param
 *    - notice: ble evt type
 *    - param: param associated with notice
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
 *    - service_p: gatt service point
 *    - att_index: att index
 *    - buffer: point to buff
 *    - buffer_len: buffer len
 *    - buffer_max_len: buffer max len
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_server_attr_set_data_ptr_private(void* service_p, uint8_t att_index,
        uint8_t *buffer, uint16_t buffer_len, uint16_t buffer_max_len);
