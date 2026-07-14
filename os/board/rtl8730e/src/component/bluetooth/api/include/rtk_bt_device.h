/**
 * @file      rtk_bt_device.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth device related function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_DEVICE_H__
#define __RTK_BT_DEVICE_H__

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup  bt_device BT Device APIs
 * @brief     BT device function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief     Check bt enable status
 * @return
 *            - true  : enabled
 *            - false: not enabled
 */
bool rtk_bt_is_enable(void);

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
/**
 * @brief     Check bt mesh enable status
 * @return
 *            - true  : enabled
 *            - false: not enabled
 */
bool rtk_bt_mesh_is_enable(void);
#endif

/**
 * @brief     Enable BT.
 * @param[in] app_default_conf: Default config parameters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_enable(rtk_bt_app_conf_t *app_default_conf);

/**
 * @brief     Disable BT.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_disable(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_MAIN_H__ */
