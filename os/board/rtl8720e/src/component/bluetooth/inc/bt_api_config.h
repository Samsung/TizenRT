/**
 * @file      bt_api_config.h
 * @author    frank_shen@realsil.com.cn
 * @brief     Bluetooth platform config macros definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __BT_API_CONFIG_H__
#define __BT_API_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * PLATFORM_8710C (amebaZ2)
 */
#if defined(CONFIG_PLATFORM_8710C)
#define RTK_BLE_GAP_MAX_LINKS               3
#define RTK_BLE_SUPPORT                     1

#if RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             0
#endif /* RTK_BLE_SUPPORT */

#if RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             0
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    0
#endif /* RTK_BLE_4_2_SUPPORT */

#if RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        0
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * PLATFORM_AMEBAD2 (amebaD2)
 */
#elif defined(CONFIG_PLATFORM_AMEBAD2)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1

#if RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#endif /* RTK_BLE_SUPPORT */

#if RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          1
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         1
#define RTK_BLE_5_0_PA_SUPPORT              1
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * PLATFORM_AMEBALITE (amebaLite)
 */
#elif defined(CONFIG_PLATFORM_AMEBALITE)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1

#if RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#endif /* RTK_BLE_SUPPORT */

#if RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#define RTK_BLE_5_0_PA_SUPPORT              0
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * PLATFORM_8721D (amebaD)
 */
#elif defined(CONFIG_PLATFORM_8721D)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1

#if RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#endif /* RTK_BLE_SUPPORT */

#if RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             0
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * PLATFORM_8735B (amebaPro2)
 */
#elif defined(CONFIG_PLATFORM_8735B)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1

#if RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
#endif /* RTK_BLE_SUPPORT */

#if RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * Error Platform
 */
#else
#error Please choose a corret platform
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BT_API_CONFIG_H__ */