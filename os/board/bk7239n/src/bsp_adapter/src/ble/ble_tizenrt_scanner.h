#pragma once

#include <tinyara/net/if/ble.h>

/**
 * @brief Initialize the TizenRT BLE scanner.
 */
int32_t bk_tr_ble_scanner_init(void);
/**
 * @brief Deinitialize the TizenRT BLE scanner.
 */
int32_t bk_tr_ble_scanner_deinit(void);
/**
 * @brief Callback function to handle BLE notices.
 * @param notice The BLE notice to be handled.
 * @param param Pointer to the parameter related to the notice.
 */
int32_t bk_tr_ble_scanner_notice_cb(ble_notice_t notice, void *param);
