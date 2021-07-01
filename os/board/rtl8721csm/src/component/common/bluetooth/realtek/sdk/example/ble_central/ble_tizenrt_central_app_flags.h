/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_central_app_flags.h
   * @brief     This file is used to config app functions.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _BLE_TIZENRT_CENTRAL_APP_FLAGS_H_
#define _BLE_TIZENRT_CENTRAL_APP_FLAGS_H_

#include <trace_app.h>
#include <bt_flags.h>
#include <app_common_flags.h>

/** @defgroup  CENTRAL_CLIENT_Config Central Client App Configuration
    * @brief This file is used to config app functions.
    * @{
    */
/*============================================================================*
 *                              Constants
 *============================================================================*/

/** @brief  Config APP LE link number */
#if defined(CONFIG_PLATFORM_8721D)
#define BLE_TIZENRT_CENTRAL_APP_MAX_LINKS  3
#elif defined(CONFIG_PLATFORM_8710C)
#define BLE_TIZENRT_CENTRAL_APP_MAX_LINKS  1
#endif

/** @brief  Config the discovery table number of gcs_client */
#define BLE_TIZENRT_CENTRAL_APP_MAX_DISCOV_TABLE_NUM 40

/** @brief  Config set physical: 0-Not built in, 1-built in, use user command to set*/
#if defined(CONFIG_PLATFORM_8721D)
#define F_BT_LE_5_0_SET_PHY_SUPPORT         1
#elif defined(CONFIG_PLATFORM_8710C)
#define F_BT_LE_5_0_SET_PHY_SUPPORT         0
#endif

/** @} */ /* End of group CENTRAL_CLIENT_Config */

#endif
