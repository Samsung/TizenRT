#pragma once

#include <tinyara/net/if/ble.h>


/**
 * @brief Initialize common BLE subsystem components
 * 
 * Prepares shared resources and configurations required by BLE features.
 * Should be called before any BLE operations.
 *
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful initialization
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_common_init(void);
/**
 * @brief Deinitialize BLE common components
 * 
 * Releases resources allocated by bk_tr_ble_common_init().
 * Should be called after all BLE operations complete.
 *
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful deinitialization
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_common_deinit(void);
