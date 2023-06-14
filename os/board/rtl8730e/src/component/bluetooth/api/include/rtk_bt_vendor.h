/**
 * @file      rtk_bt_vendor.h
 * @author    
 * @brief     Bluetooth Common function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_VENDOR_H__
#define __RTK_BT_VENDOR_H__

#include <rtk_bt_def.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup  bt_vendor BT Vendor APIs
 * @brief     BT vendor APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief     set bt tx power gain index.
 * @param[in] index: tx power gain index.
 * @return    None
 */
void rtk_bt_set_bt_tx_power_gain_index(uint32_t index);

/**
 * @brief     set bt tx power gain index.
 * @param[in] index: tx power gain index.
 * @return    None
 */
void rtk_bt_set_bt_antenna(uint8_t ant);

/**
 * @brief     enable hci debug
 * @return    None
 */
void rtk_bt_hci_debug_enable(void);

/**
 * @brief     bt le sof and eof interrupt indication enable or disable. This api is only useable for AmebaSmart Platform.
 * @param[in] conn_handle: connection handle
 * @param[in] enable: 0 for disable, 1 for enable
 * @param[in] cb: sof and eof interrupt callback
 * @return
 *            - 0  : Succeed
 *            - Others: Error code 
 */
uint16_t rtk_bt_le_sof_eof_ind(uint16_t conn_handle, uint8_t enable, void * cb);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_VENDOR_H__ */
