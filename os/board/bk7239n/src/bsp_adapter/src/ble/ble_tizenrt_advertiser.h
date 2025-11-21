#pragma once

#include <tinyara/net/if/ble.h>

/**
 * @brief bk tizenrt adapter adv init
 *
 * @param
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_advertiser_init(void);

/**
 * @brief bk tizenrt adapter adv deinit
 *
 * @param
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_advertiser_deinit(void);

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
int32_t bk_tr_ble_advertiser_notice_cb(ble_notice_t notice, void *param);
