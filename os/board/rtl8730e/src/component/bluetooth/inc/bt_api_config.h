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

#include <platform_opts_bt.h>

/*
 * PLATFORM_AMEBAD2 (AmebaSmart)
 */
#if defined(CONFIG_PLATFORM_AMEBAD2)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BREDR_SUPPORT                   1
#define RTK_BT_POWER_CONTROL_SUPPORT        1
#define RTK_BLE_SET_TX_QUEUE_NUM            0
#define RTK_BLE_TX_SOF_EOF_INDICATION       0

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#if ((defined(CONFIG_BT_MESH_PROVISIONER_SUPPORT) && CONFIG_BT_MESH_PROVISIONER_SUPPORT) || \
    (defined(CONFIG_BT_MESH_DEVICE_SUPPORT) && CONFIG_BT_MESH_DEVICE_SUPPORT))
#define RTK_BLE_MESH_SUPPORT                1
#endif
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
#endif /* RTK_BLE_5_1_SUPPORT */

#if defined(RTK_BLE_5_2_SUPPORT) && RTK_BLE_5_2_SUPPORT
#define RTK_BLE_ISO_CIS_SUPPORT             0
#define RTK_BLE_ISO_BIS_SUPPORT             0
#define RTK_BLE_ISO_SUPPORT                 (RTK_BLE_ISO_CIS_SUPPORT || RTK_BLE_ISO_BIS_SUPPORT)
#define RTK_BLE_5_2_POWER_CONTROL_SUPPORT   0
#define RTK_BT_5_2_EATT_SUPPORT             0
#define RTK_BT_5_2_L2C_ECFC_SUPPORT         (RTK_BT_5_2_EATT_SUPPORT)

#define RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT           0    //must set 1 as CAP Acceptor role, can set 0 as CAP other role 
#define RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT         0    //must set 1 when CAP Initiator role, can set 0 as CAP other role 
#define RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT           0    //must set 1 when CAP Acceptor role, can set 0 as CAP other role 
#define RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT           0    //must set 1 when CAP Initiator role, can set 0 as CAP other role 
#define RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT      0    //must set 1 when CAP Commander role, can set 0 as CAP other role  
#define RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT           0    //must set 1 when CAP Acceptor or Commander role, can set 0 as CAP other role 
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

#define RTK_BLE_AUDIO_TMAP_SUPPORT                              0    //must set when test TMAP example
#define RTK_BLE_AUDIO_TMAP_CALL_TERMINAL_SUPPORT                0    //not support now
#define RTK_BLE_AUDIO_TMAP_CALL_GATEWAY_SUPPORT                 0    //not support now
#define RTK_BLE_AUDIO_TMAP_UNICAST_MEDIA_SENDER_SUPPORT         0    //must set CAP Initiator && Commander role
#define RTK_BLE_AUDIO_TMAP_UNICAST_MEDIA_RECEIVER_SUPPORT       0    //must set CAP Acceptor role
#define RTK_BLE_AUDIO_TMAP_BROADCAST_MEDIA_SENDER_SUPPORT       0    
#define RTK_BLE_AUDIO_TMAP_BROADCAST_MEDIA_RECEIVER_SUPPORT     0    

#define RTK_BLE_AUDIO_PBP_SUPPORT                               0
#define RTK_BLE_AUDIO_PBP_BROADCAST_SOURCE_SUPPORT              0    //must set RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT 1
#define RTK_BLE_AUDIO_PBP_BROADCAST_SINK_SUPPORT                0
#define RTK_BLE_AUDIO_PBP_BROADCAST_ASSISTANT_SUPPORT           0

#define RTK_BLE_AUDIO_BIRDS_SING_PCM_SUPPORT            1    //audio data tx from birds_sing[] when set 1, otherwise from audio record 
#define RTK_BLE_AUDIO_RECORD_SUPPORT                    0
#define RTK_BLE_AUDIO_A2DP_PBP_TEST_SUPPORT             0

#define RTK_BLE_AUDIO_SINK_SUPPORT                  (RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT || RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT || RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
#define RTK_BLE_AUDIO_SOURCE_SUPPORT                (RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT || RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT || RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
#define RTK_BLE_AUDIO_SUPPORT                       (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT || RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT || \
                                                     RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT || RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT || \
                                                     RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT || RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT)
#endif /* RTK_BLE_5_2_SUPPORT */

#if defined(CONFIG_BT_MESH_PROVISIONER_SUPPORT) && CONFIG_BT_MESH_PROVISIONER_SUPPORT
#define RTK_BLE_MESH_PROVISIONER_SUPPORT    1
#define RTK_BLE_MESH_FN_SUPPORT             1
#else
#define RTK_BLE_MESH_PROVISIONER_SUPPORT    0
#endif /* BLE MESH PROVISIONER */
#if defined(CONFIG_BT_MESH_DEVICE_SUPPORT) && CONFIG_BT_MESH_DEVICE_SUPPORT
#define RTK_BLE_MESH_DEVICE_SUPPORT         1
#define RTK_BLE_MESH_FN_SUPPORT             1
#define RTK_BLE_MESH_LPN_SUPPORT            1
#else
#define RTK_BLE_MESH_DEVICE_SUPPORT         0
#endif /* BLE MESH DEVICE */
#if RTK_BLE_MESH_PROVISIONER_SUPPORT && RTK_BLE_MESH_DEVICE_SUPPORT
#error "Can not enable RTK_BLE_MESH_PROVISIONER_SUPPORT and RTK_BLE_MESH_DEVICE_SUPPORT at same time"
#endif

/*
 * PLATFORM_AMEBALITE (AmebaLite)
 */
#elif defined(CONFIG_PLATFORM_AMEBALITE)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#if defined(CONFIG_BT_BLE_ONLY) && CONFIG_BT_BLE_ONLY   // Defined in menuconfig
#define RTK_BREDR_SUPPORT                   0
#endif
#if defined(CONFIG_BT_DUAL_MODE) && CONFIG_BT_DUAL_MODE // Defined in menuconfig
#define RTK_BREDR_SUPPORT                   1
#endif
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 1
#define RTK_BLE_SMP_OOB_SUPPORT             1
#if ((defined(CONFIG_BT_MESH_PROVISIONER_SUPPORT) && CONFIG_BT_MESH_PROVISIONER_SUPPORT) || \
    (defined(CONFIG_BT_MESH_DEVICE_SUPPORT) && CONFIG_BT_MESH_DEVICE_SUPPORT))
#define RTK_BLE_MESH_SUPPORT                1
#endif
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
#endif /* RTK_BLE_5_1_SUPPORT */

#if defined(RTK_BLE_5_2_SUPPORT) && RTK_BLE_5_2_SUPPORT
#define RTK_BLE_ISO_CIS_SUPPORT             0
#define RTK_BLE_ISO_BIS_SUPPORT             0
#define RTK_BLE_ISO_SUPPORT                 (RTK_BLE_ISO_CIS_SUPPORT || RTK_BLE_ISO_BIS_SUPPORT)
#define RTK_BLE_5_2_POWER_CONTROL_SUPPORT   0
#define RTK_BT_5_2_EATT_SUPPORT             0
#define RTK_BT_5_2_L2C_ECFC_SUPPORT         (RTK_BT_5_2_EATT_SUPPORT)
#endif /* RTK_BLE_5_2_SUPPORT */

#if defined(CONFIG_BT_MESH_PROVISIONER_SUPPORT) && CONFIG_BT_MESH_PROVISIONER_SUPPORT
#define RTK_BLE_MESH_PROVISIONER_SUPPORT    1
#define RTK_BLE_MESH_FN_SUPPORT             1
#else
#define RTK_BLE_MESH_PROVISIONER_SUPPORT    0
#endif
#if defined(CONFIG_BT_MESH_DEVICE_SUPPORT) && CONFIG_BT_MESH_DEVICE_SUPPORT
#define RTK_BLE_MESH_DEVICE_SUPPORT         1
#define RTK_BLE_MESH_FN_SUPPORT             1
#define RTK_BLE_MESH_LPN_SUPPORT            1
#else
#define RTK_BLE_MESH_DEVICE_SUPPORT         0
#endif
#if RTK_BLE_MESH_PROVISIONER_SUPPORT && RTK_BLE_MESH_DEVICE_SUPPORT
#error "Can not enable RTK_BLE_MESH_PROVISIONER_SUPPORT and RTK_BLE_MESH_DEVICE_SUPPORT at same time"
#endif

/*
 * PLATFORM_8735B (AmebaPro2)
 */
#elif defined(CONFIG_PLATFORM_8735B)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
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
 * PLATFORM_AMEBADPLUS (AmebaDplus)
 */
#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 1
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
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
 * PLATFORM_RTL8720F (AmebaLite2)
 */
#elif defined(CONFIG_PLATFORM_RTL8720F)
#define RTK_BLE_GAP_MAX_LINKS               4
#define RTK_BLE_SUPPORT                     1
#define RTK_BT_POWER_CONTROL_SUPPORT        1

#if defined(RTK_BLE_SUPPORT) && RTK_BLE_SUPPORT
#define RTK_BLE_4_0_SUPPORT                 1
#define RTK_BLE_4_2_SUPPORT                 1
#define RTK_BLE_5_0_SUPPORT                 1
#define RTK_BLE_5_1_SUPPORT                 0
#define RTK_BLE_5_2_SUPPORT                 0
#define RTK_BLE_SMP_OOB_SUPPORT             1
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
#error Please choose a corret platform
#endif

#if ((!defined(CONFIG_BT_ZEPHYR)) && ((defined(RTK_BT_5_2_EATT_SUPPORT) && RTK_BT_5_2_EATT_SUPPORT) || (defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT)))
#define RTK_BLE_MGR_LIB             1
#define GATTC_TBL_STORAGE_SUPPORT   0
#else
#define RTK_BLE_MGR_LIB             0
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
/* Ext adv is used by audio lib and controled by ble_mgr.a, use ble_ext_adv.h instead of gap_ext_adv.h */
#define RTK_BLE_MGR_LIB_EADV        1
#else
#define RTK_BLE_MGR_LIB_EADV        0
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
/* PA adv is controled by audio lib when audio broadcast source is enabled. APP can NOT use PA adv API. */
#undef RTK_BLE_5_0_PA_ADV_SUPPORT
#define RTK_BLE_5_0_PA_ADV_SUPPORT  0
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
/* PA sync is controled by audio lib when audio broadcast sink is enabled. APP can NOT use PA sync API. */
#undef RTK_BLE_5_0_PA_SYNC_SUPPORT
#define RTK_BLE_5_0_PA_SYNC_SUPPORT  0
#else
#endif

#if (defined(CONFIG_BT_ISO_TEST) && CONFIG_BT_ISO_TEST) && (defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT) && \
    ((!defined(RTK_BLE_5_0_AE_ADV_SUPPORT) || !RTK_BLE_5_0_AE_ADV_SUPPORT) || (!defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) || !RTK_BLE_5_0_AE_SCAN_SUPPORT) || \
     (!defined(RTK_BLE_5_0_PA_ADV_SUPPORT) || !RTK_BLE_5_0_PA_ADV_SUPPORT) || (!defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) || !RTK_BLE_5_0_PA_SYNC_SUPPORT))
#error Please enable AE, AE Scan, PA, PA Sync for corret platform when enable ISO BIS!
#endif

#if (defined(CONFIG_BT_LE_AUDIO) && CONFIG_BT_LE_AUDIO) && (defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT) && \
    ((!defined(RTK_BLE_5_0_AE_ADV_SUPPORT) || !RTK_BLE_5_0_AE_ADV_SUPPORT) || (!defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) || !RTK_BLE_5_0_AE_SCAN_SUPPORT))
#error Please enable AE, AE Scan for corret platform when enable LE AUDIO!
#endif

#if defined(CONFIG_PLATFORM_AMEBAD2) && defined(__ICCARM__) && defined(RTK_BREDR_SUPPORT)
#undef RTK_BREDR_SUPPORT
#define RTK_BREDR_SUPPORT  0
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BT_API_CONFIG_H__ */
