/**
 * @file      rtk_bt_power_control.h
 * @author
 * @brief     Bluetooth Common function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_POWER_CONTROL_H__
#define __RTK_BT_POWER_CONTROL_H__

#include <rtk_bt_def.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*rtk_bt_ps_callback)(void);

/**
 * @defgroup  bt_power_control BT Power Control APIs
 * @brief     BT power control function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
* @brief     BT enable power save.
* @param     None
* @return    None
*/
void rtk_bt_enable_power_save(void);

/**
* @brief     BT disable power save.
* @param     None
* @return    None
*/
void rtk_bt_disable_power_save(void);

/**
* @brief     BT power save init.
* @param[in] p_suspend_callback: Callback invoked before system entering power save mode.
* @param[in] p_resume_callback: Callback invoked after system waking from power save mode.
* @return    None
*/
void rtk_bt_power_save_init(rtk_bt_ps_callback p_suspend_callback, rtk_bt_ps_callback p_resume_callback);

/**
* @brief     BT power save deinit.
* @param     None
* @return    None
*/
void rtk_bt_power_save_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_POWER_CONTROL_H__ */
