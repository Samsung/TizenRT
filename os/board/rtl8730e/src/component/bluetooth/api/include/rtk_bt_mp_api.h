/**
 * @file      rtk_bt_mp_api.h
 * @author    
 * @brief     Bluetooth Common function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_MP_API_H__
#define __RTK_BT_MP_API_H__

#include <rtk_bt_def.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup  bt_mp_api BT MP APIs
 * @brief     BT MP test APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief     Init BT power on for MP test.
 * @param     None
 * @return    None
 */
void rtk_bt_mp_power_on(void);

/**
 * @brief     Init BT power off for MP test.
 * @param     None
 * @return    None
 */
void rtk_bt_mp_power_off(void);

/**
 * @brief     set bt rf path.
 * @param[in] ant_switch:patch switch (0 for S0, 1 for S1)
 * @return    None
 * This api is only useable for AmebaSmart Platform
 */
void rtk_bt_set_ant(uint8_t ant_switch);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_MP_API_H__ */
