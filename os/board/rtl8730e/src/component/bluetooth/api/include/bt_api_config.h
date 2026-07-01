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

#include "platform_autoconf.h"

/*
 * AmebaSmart
 */
#if defined(CONFIG_AMEBASMART) && CONFIG_AMEBASMART
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#if defined(CONFIG_BT_BLE_ONLY) && CONFIG_BT_BLE_ONLY   // Defined in menuconfig
#define RTK_BREDR_SUPPORT                   0
#endif
#if defined(CONFIG_BT_DUAL_MODE) && CONFIG_BT_DUAL_MODE // Defined in menuconfig
#define RTK_BREDR_SUPPORT                   1
#endif
#define RTK_BT_POWER_CONTROL_SUPPORT        1
#define RTK_BT_HC_CLOCK_OFFSET_SUPPORT      1
#define RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT 0
#define RTK_BLE_SET_TX_QUEUE_NUM            0
#define RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT       0

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#define RTK_BLE_COC_SUPPORT                 1
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          1
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         1
#define RTK_BLE_5_0_PA_ADV_SUPPORT         (0 && RTK_BLE_5_0_AE_ADV_SUPPORT)
#define RTK_BLE_5_0_PA_SYNC_SUPPORT        (0 && RTK_BLE_5_0_AE_SCAN_SUPPORT)
#endif /* RTK_BLE_5_0_SUPPORT */

#if defined(RTK_BLE_5_1_SUPPORT) && RTK_BLE_5_1_SUPPORT
#define RTK_BLE_5_1_PAST_SENDER_SUPPORT     0
#define RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT  0
#define RTK_BLE_5_1_CTE_SUPPORT             0
#endif /* RTK_BLE_5_1_SUPPORT */

#if defined(RTK_BLE_5_2_SUPPORT) && RTK_BLE_5_2_SUPPORT
#define RTK_BLE_5_2_POWER_CONTROL_SUPPORT   0
#define RTK_BT_5_2_EATT_SUPPORT             0
#define RTK_BT_5_2_L2C_ECFC_SUPPORT         (RTK_BT_5_2_EATT_SUPPORT)
#endif /* RTK_BLE_5_2_SUPPORT */

/*
 * AmebaLite
 */
#elif defined(CONFIG_AMEBALITE) && CONFIG_AMEBALITE
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#if defined(CONFIG_BT_BLE_ONLY) && CONFIG_BT_BLE_ONLY   // Defined in menuconfig
#define RTK_BREDR_SUPPORT                   0
#endif
#if defined(CONFIG_BT_DUAL_MODE) && CONFIG_BT_DUAL_MODE // Defined in menuconfig
#define RTK_BREDR_SUPPORT                   1
#endif
#define RTK_BT_POWER_CONTROL_SUPPORT        1
#define RTK_BT_HC_CLOCK_OFFSET_SUPPORT      1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#define RTK_BLE_COC_SUPPORT                 0
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#define RTK_BLE_5_0_PA_ADV_SUPPORT         (0 && RTK_BLE_5_0_AE_ADV_SUPPORT)
#define RTK_BLE_5_0_PA_SYNC_SUPPORT        (0 && RTK_BLE_5_0_AE_SCAN_SUPPORT)
#endif /* RTK_BLE_5_0_SUPPORT */

#if defined(RTK_BLE_5_1_SUPPORT) && RTK_BLE_5_1_SUPPORT
#define RTK_BLE_5_1_PAST_SENDER_SUPPORT     0
#define RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT  0
#define RTK_BLE_5_1_CTE_SUPPORT             0
#endif /* RTK_BLE_5_1_SUPPORT */

#if defined(RTK_BLE_5_2_SUPPORT) && RTK_BLE_5_2_SUPPORT
#define RTK_BLE_5_2_POWER_CONTROL_SUPPORT   0
#define RTK_BT_5_2_EATT_SUPPORT             0
#define RTK_BT_5_2_L2C_ECFC_SUPPORT         (RTK_BT_5_2_EATT_SUPPORT)
#endif /* RTK_BLE_5_2_SUPPORT */

/*
 * AmebaDplus
 */
#elif defined(CONFIG_AMEBADPLUS) && CONFIG_AMEBADPLUS
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
#define RTK_BLE_COC_SUPPORT                 0
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * AmebaGreen2
 */
#elif defined(CONFIG_AMEBAGREEN2) && CONFIG_AMEBAGREEN2
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 0
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define RTK_BLE_COC_SUPPORT                 1
#else
#define RTK_BLE_COC_SUPPORT                 0
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define RTK_BLE_5_0_AE_ADV_SUPPORT          1
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         1
#else
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * AmebaL2
 */
#elif defined(CONFIG_AMEBAL2) && CONFIG_AMEBAL2
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 0
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
#define RTK_BLE_COC_SUPPORT                 0
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#if defined(CONFIG_SDN) && CONFIG_SDN
#define RTK_BLE_PRIVACY_SUPPORT             0
#else
#define RTK_BLE_PRIVACY_SUPPORT             1
#endif
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * Ameba 8720F
 */
#elif defined(CONFIG_RTL8720F) && CONFIG_RTL8720F
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        0

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 0
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
#define RTK_BLE_COC_SUPPORT                 0
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             0
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * AmebaPro3
 */
#elif defined(CONFIG_AMEBAPRO3) && CONFIG_AMEBAPRO3
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_GATTS_SUPPORT               1
#define RTK_BLE_GATTC_SUPPORT               1
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 0
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
#define RTK_BLE_COC_SUPPORT                 0
#endif /* RTK_BLE_SUPPORT */

#if defined(RTK_BLE_4_2_SUPPORT) && RTK_BLE_4_2_SUPPORT
#define RTK_BLE_PRIVACY_SUPPORT             1
#define RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT    1
#endif /* RTK_BLE_4_2_SUPPORT */

#if defined(RTK_BLE_5_0_SUPPORT) && RTK_BLE_5_0_SUPPORT
#define RTK_BLE_5_0_SET_PHYS_SUPPORT        1
#define RTK_BLE_5_0_AE_ADV_SUPPORT          0
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         0
#endif /* RTK_BLE_5_0_SUPPORT */

/*
 * Error Platform
 */
#else
#error "Please choose a correct platform"
#endif

#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#if defined(CONFIG_BT_PROFILE_A2DP_SUPPORT) && CONFIG_BT_PROFILE_A2DP_SUPPORT
#define RTK_BT_PROFILE_A2DP_SUPPORT         1
#endif
#if defined(CONFIG_BT_PROFILE_AVRCP_SUPPORT) && CONFIG_BT_PROFILE_AVRCP_SUPPORT
#define RTK_BT_PROFILE_AVRCP_SUPPORT        1
#endif
#if defined(CONFIG_BT_PROFILE_HFP_SUPPORT) && CONFIG_BT_PROFILE_HFP_SUPPORT
#define RTK_BT_PROFILE_HFP_SUPPORT          1
#endif
#if defined(CONFIG_BT_PROFILE_PBAP_SUPPORT) && CONFIG_BT_PROFILE_PBAP_SUPPORT
#define RTK_BT_PROFILE_PBAP_SUPPORT         1
#endif
#if defined(CONFIG_BT_PROFILE_SPP_SUPPORT) && CONFIG_BT_PROFILE_SPP_SUPPORT
#define RTK_BT_PROFILE_SPP_SUPPORT          1
#endif
#if defined(CONFIG_BT_PROFILE_RFC_SUPPORT) && CONFIG_BT_PROFILE_RFC_SUPPORT
#define RTK_BT_PROFILE_RFC_SUPPORT          1
#endif
#if defined(CONFIG_BT_PROFILE_HID_SUPPORT) && CONFIG_BT_PROFILE_HID_SUPPORT
#define RTK_BT_PROFILE_HID_SUPPORT          1
#endif
#if defined(CONFIG_BT_PROFILE_SDP_SUPPORT) && CONFIG_BT_PROFILE_SDP_SUPPORT
#define RTK_BT_PROFILE_SDP_SUPPORT          1
#endif
#endif

#if defined(RTK_BLE_5_2_SUPPORT) && RTK_BLE_5_2_SUPPORT
#if defined(CONFIG_BT_ISO_SUPPORT) && CONFIG_BT_ISO_SUPPORT
#define RTK_BLE_ISO_SUPPORT                 1
#endif
#if defined(CONFIG_BT_LE_AUDIO_SUPPORT) && CONFIG_BT_LE_AUDIO_SUPPORT
#define RTK_BLE_AUDIO_SUPPORT               1
#if defined(CONFIG_BT_TMAP_SUPPORT) && CONFIG_BT_TMAP_SUPPORT
#define RTK_BLE_AUDIO_TMAP_SUPPORT          1
#endif
#if defined(CONFIG_BT_GMAP_SUPPORT) && CONFIG_BT_GMAP_SUPPORT
#define RTK_BLE_AUDIO_GMAP_SUPPORT          1
#endif
#endif
#endif /* RTK_BLE_5_2_SUPPORT */

#if (defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT) && (defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT)
#error "Can not enable RTK_BLE_ISO_SUPPORT and RTK_BLE_AUDIO_SUPPORT at same time"
#endif

#if defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT
#undef RTK_BLE_5_0_AE_ADV_SUPPORT
#undef RTK_BLE_5_0_AE_SCAN_SUPPORT
#undef RTK_BLE_5_0_PA_ADV_SUPPORT
#undef RTK_BLE_5_0_PA_SYNC_SUPPORT
#define RTK_BLE_5_0_AE_ADV_SUPPORT          1
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         1
#define RTK_BLE_5_0_PA_ADV_SUPPORT          1
#define RTK_BLE_5_0_PA_SYNC_SUPPORT         1
#endif

#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
#undef RTK_BLE_5_0_AE_ADV_SUPPORT
#undef RTK_BLE_5_0_AE_SCAN_SUPPORT
#undef RTK_BLE_MGR_LIB_EADV
#undef RTK_BLE_5_0_PA_ADV_SUPPORT
#undef RTK_BLE_5_0_PA_SYNC_SUPPORT
#define RTK_BLE_5_0_AE_ADV_SUPPORT          1
#define RTK_BLE_5_0_AE_SCAN_SUPPORT         1
#define RTK_BLE_MGR_LIB_EADV                1
#define RTK_BLE_5_0_PA_ADV_SUPPORT          0
#define RTK_BLE_5_0_PA_SYNC_SUPPORT         0

#define RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT    0    //can set 1 when CAP Commander role
#define RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT      0    //can set 1 when CAP Acceptor role
#define RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT      0    //can set 1 when CAP Commander role
#define RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT          0    //can set 1 when CAP Acceptor role
#define RTK_BLE_AUDIO_AICS_SUPPORT                     0    //can set 1 when CAP Acceptor or Commander role
#define RTK_BLE_AUDIO_VOCS_SUPPORT                     0    //can set 1 when CAP Acceptor or Commander role
#define RTK_BLE_AUDIO_CCP_CALL_CONTROL_SERVER_SUPPORT  0    //not support now
#define RTK_BLE_AUDIO_CCP_CALL_CONTROL_CLIENT_SUPPORT  0    //not support now
#define RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT 0    //can set 1 when CAP Initiator role
#define RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT 0    //can set 1 when CAP Acceptor or Commander role
#define RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT     0    //can set 1 when CAP Initiator or Commander role
#define RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT          0    //can set 1 when CAP Acceptor role
#endif

#if (!defined(CONFIG_BT_ZEPHYR) || !CONFIG_BT_ZEPHYR) && (defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT)
#define RTK_BLE_MGR_LIB             1
#define GATTC_TBL_STORAGE_SUPPORT   0
#else
#define RTK_BLE_MGR_LIB             0
#endif

#if (defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT) && \
    ((!defined(RTK_BLE_5_0_AE_ADV_SUPPORT) || !RTK_BLE_5_0_AE_ADV_SUPPORT) || (!defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) || !RTK_BLE_5_0_AE_SCAN_SUPPORT) || \
     (!defined(RTK_BLE_5_0_PA_ADV_SUPPORT) || !RTK_BLE_5_0_PA_ADV_SUPPORT) || (!defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) || !RTK_BLE_5_0_PA_SYNC_SUPPORT))
#error "Please enable AE, AE Scan, PA, PA Sync for correct platform when enable ISO DEMO"
#endif

#if (defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT) && \
    ((!defined(RTK_BLE_5_0_AE_ADV_SUPPORT) || !RTK_BLE_5_0_AE_ADV_SUPPORT) || (!defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) || !RTK_BLE_5_0_AE_SCAN_SUPPORT))
#error "Please enable AE, AE Scan for correct platform when enable LE AUDIO"
#endif

#if defined(CONFIG_BT_MESH_PROVISIONER_SUPPORT) && CONFIG_BT_MESH_PROVISIONER_SUPPORT
#define RTK_BLE_MESH_SUPPORT                1
#define RTK_BLE_MESH_PROVISIONER_SUPPORT    1
#define RTK_BLE_MESH_FN_SUPPORT             1
#else
#define RTK_BLE_MESH_PROVISIONER_SUPPORT    0
#endif
#if defined(CONFIG_BT_MESH_DEVICE_SUPPORT) && CONFIG_BT_MESH_DEVICE_SUPPORT
#define RTK_BLE_MESH_SUPPORT                1
#define RTK_BLE_MESH_DEVICE_SUPPORT         1
#define RTK_BLE_MESH_FN_SUPPORT             1
#define RTK_BLE_MESH_LPN_SUPPORT            1
#else
#define RTK_BLE_MESH_DEVICE_SUPPORT         0
#endif
#if (defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT) && (defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT)
#error "Can not enable RTK_BLE_MESH_PROVISIONER_SUPPORT and RTK_BLE_MESH_DEVICE_SUPPORT at same time"
#endif

#if (defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT) || (defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) && RTK_BLE_5_0_AE_SCAN_SUPPORT)
#define RTK_BLE_5_0_USE_EXTENDED_ADV        1
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#define RTK_BT_API_MEM_PRE_ALLOC            1
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
#define BT_API_SEM_POOL_SIZE                8
#define BT_EVT_SMALL_POOL_SIZE              32

#if (defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT) || \
    (defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT)
#define BT_EVT_MEDIUM_POOL_SIZE             8
#else
#define BT_EVT_MEDIUM_POOL_SIZE             4
#endif

#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#define BT_EVT_LARGE_POOL_SIZE              4
#else
#define BT_EVT_LARGE_POOL_SIZE              1
#endif

#define BT_EVT_SMALL_BUF_SIZE               128
#define BT_EVT_MEDIUM_BUF_SIZE              640
#define BT_EVT_LARGE_BUF_SIZE               1280
#endif  /* RTK_BT_API_MEM_PRE_ALLOC */

#ifdef __cplusplus
}
#endif

#endif /* __BT_API_CONFIG_H__ */
