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
* @brief     BT release wakelock.
* @param     None
* @return    None
*/
void rtk_bt_release_wakelock(void);

/**
* @brief     BT acquire wakelock.
* @param     None
* @return    None
*/
void rtk_bt_acquire_wakelock(void);

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

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_PM
void rtk_tizenrt_bt_pm_suspend(unsigned int milliseconds);
void rtk_tizenrt_bt_pm_resume(void);
void rtk_tizenrt_bt_pm_init(void);
void rtk_tizenrt_bt_pm_deinit(void);
#endif //#ifdef CONFIG_PM
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#endif /* __RTK_BT_POWER_CONTROL_H__ */
