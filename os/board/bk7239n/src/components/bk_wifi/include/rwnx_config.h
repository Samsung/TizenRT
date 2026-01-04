/**
 ****************************************************************************************
 *
 * @file rwnx_config.h
 *
 * @brief Inclusion of appropriate config files.
 *
 * This file should be included each time an NX_xxx definition is needed.
 * CFG_xxx macros must no be used in the code, only NX_xxx, defined in
 * the included config files.
 *
 * Copyright (C) RivieraWaves 2011-2020
 *
 ****************************************************************************************
 */

#ifndef _RWNX_CONFIG_H_
#define _RWNX_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup MACSW
 * @{
 * The CFG_xxx macros should be added on the compilation command line
 * by the SCons scripts. Because it has been not implemented yet, any
 * undefined option fall back on the default behavior.
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sdkconfig.h"
#include "fhost_mac.h"
#include "soc/soc.h"

/*
 ****************************************************************************************
 *                 starts of rwnx config                                                *
 ****************************************************************************************
 */
#define CFG_VERIFICATION
#define CFG_EMB
#define CFG_SPLIT
#define CFG_UMAC
#define CFG_DBG           CONFIG_WIFI6_IP_DEBUG
#define CFG_BCN

//#if CONFIG_CACHE_ENABLE
#undef __SHAREDRAM
//#define __SHAREDRAM       __attribute__((section(".sram_nocache")))
//#else
#define __SHAREDRAM
//#endif

#if CONFIG_SOC_BK7256XX
/////bk7256 32K ITCM,important and normal function can pick to ITCM
#define __ITCM __attribute__((section(".itcm_sec_code ")))
#define __ITCM_N __attribute__((section(".itcm_sec_code ")))
#else
////other only 16K ITCM,pick the important one to ITCM
#define __ITCM __attribute__((section(".itcm_sec_code ")))
#define __ITCM_N  __attribute__((section(".itcm_sec_code ")))
#endif

#if CONFIG_SOC_BK7256XX && !CONFIG_RWNX_NO_USE_DTCM
#define __DTCM __attribute__((section(".dtcm_sec_data ")))
#define __DTCM_BSS __attribute__((section(".dtcm_sec_bss ")))
#else
#define __DTCM
#define __DTCM_BSS
#endif

#define CFG_MAC_VER_V21
#if (CONFIG_SOC_BK7256XX)
#define CFG_MDM_VER_V31
#elif CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX
#define CFG_MDM_VER_V32
#else
#define CFG_MDM_VER_V30
#endif
//#define CFG_IPC_VER_V11
#define CFG_PLF_VER_V30


/// A-MPDU TX fast retry support
#define NX_AMPDU_TX_FAST_RETRY 1

#define CFG_AMSDU_8K      // change to 8K for HE pkt large than 4096, it may cause system crashed

#define CFG_STA_MAX       CONFIG_WIFI_MAC_SUPPORT_STAS_MAX_NUM

#define CFG_SPC           16

#if CONFIG_SOC_BK7256XX
// decrease TXDESC count to reduce memory
#define CFG_TXDESC0       1
#define CFG_TXDESC1       32
#define CFG_TXDESC2       32
#define CFG_TXDESC3       4
#define CFG_TXDESC4       1
#elif CONFIG_SOC_BK7236XX || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX
// BK_MAC doesn't alloc static memory for txdesc, but for ba agg txdesc (minimum 3)
#define CFG_TXDESC0       24
#define CFG_TXDESC1       32
#define CFG_TXDESC2       24
#define CFG_TXDESC3       24
#define CFG_TXDESC4       24
#else
#define CFG_TXDESC0       16
#define CFG_TXDESC1       16
#define CFG_TXDESC2       16
#define CFG_TXDESC3       16
#define CFG_TXDESC4       16
#endif


#define CFG_AGG
#define CFG_PS
#define CFG_DPSM
#define CFG_CMON
#define CFG_MROLE
#define CFG_HWSCAN
#define CFG_AUTOBCN
#define CFG_WAPI
#define CFG_FHOST
#define CFG_MU_CNT      1
//#define CFG_UAPSD
#define CFG_KEYCFG
#if CONFIG_WIFI6
#define CFG_HE
#define NX_HE           1
#endif
#define CFG_AMSDU
//#define CFG_REC
#define CFG_UF
#define CFG_MON_DATA
#define CFG_BFMEE
#define CFG_MU_RX
#define CFG_TWT         8
//#define CFG_TDLS
//#define CFG_ANT_DIV

/* hardware security unit, BK7236 doesn't Support */
//#define CFG_HSU         1
#define CFG_COEX
//#define CONFIG_RWNX_LWIP
//#define CFG_TG
//#define CFG_FHOST_MONITOR
//#define CFG_SMARTCONFIG
#if CONFIG_P2P
#define CFG_P2P             2
#if CONFIG_P2P_GO
#define CFG_P2P_GO
#endif //CONFIG_P2P_GO
#else //CONFIG_P2P
#define CFG_P2P             0
#endif //CONFIG_P2P

#if CONFIG_WIFI_FTM
#define CFG_FTM_INIT 1
#define CFG_FTM_RSP 1
#else
#define CFG_FTM_INIT 0
#define CFG_FTM_RSP 0
#endif

#if CONFIG_WIFI4
#define CFG_BWLEN

#if (!CONFIG_RF_OTA_TEST)
#define CFG_RC
#endif

#define CFG_BARX            8
#define CFG_BATX            5

//#define CFG_TDLS
//#define CFG_AMSDU
#define CFG_REORD_BUF       30
#endif // CONFIG_IEEE80211N

#if CONFIG_PMF
#define CFG_MFP
#endif

#if CONFIG_WPA_SME
#define NX_HOST_SME            1
#else
#define NX_HOST_SME            0
#endif
#define CFG_VIF_MAX            2

#if CFG_SUPPORT_MESH
#define CFG_MESH

//Number of supported Mesh Point Interfaces
#define CFG_MESH_VIF        2
//Number of supported Mesh Links (shared between the Mesh VIFs)
#define CFG_MESH_LINK       2
//Number of supported Mesh Paths
#define CFG_MESH_PATH       10
//Number of supported Mesh Proxy Information
#define CFG_MESH_PROXY      2
#endif

/*
 ****************************************************************************************
 *                 ends of rwnx config                                                *
 ****************************************************************************************
 */

/// Build for fully hosted partitioning
#ifdef CFG_FHOST
  #define NX_FULLY_HOSTED  1
  // No A-MSDU TX in fully hosted
  #undef CFG_AMSDU
  // A-MSDU de-aggregation enabled
  #define CFG_DEAGG
#else  // !CFG_FHOST
  #define NX_FULLY_HOSTED  0
#endif // CFG_FHOST

// Features that must be enabled for MESH support
#ifdef CFG_MESH
  #undef CFG_MROLE
  #define CFG_MROLE
  #undef CFG_BCN
  #define CFG_BCN
  #undef CFG_AUTOBCN
  #define CFG_AUTOBCN
  #undef CFG_MFP
  #define CFG_MFP
  #undef CFG_PS
  #define CFG_PS
#endif //defined CFG_MESH

// Features that must be enabled for UMAC support
#ifdef CFG_UMAC
  /// Whether UMAC is enable or not
  #define NX_UMAC_PRESENT 1
  #undef CFG_CMON
  #define CFG_CMON
  #undef CFG_HWSCAN
  #define CFG_HWSCAN
  #undef CFG_MROLE
  #define CFG_MROLE
  #undef CFG_AUTOBCN
  #ifdef CFG_BCN
    #define CFG_AUTOBCN
  #endif
#else  // !CFG_UMAC
  /// Whether UMAC is enable or not
  #define NX_UMAC_PRESENT 0
#endif // CFG_UMAC


#if BK_MAC
#define NX_CRM  1
#define NX_PTA  0
#endif


/**
 *******************************************************************************
 * @name MAC HW version
 *******************************************************************************
 */
/// MAC version
#ifdef CFG_MAC_VER_V10
  #define NX_MAC_VER 10
#elif defined CFG_MAC_VER_V20
  #define NX_MAC_VER 20
#elif defined CFG_MAC_VER_V21
  #define NX_MAC_VER 21
#endif
#if (NX_MAC_VER == 20)
#error "MAC version 20 is deprecated"
#endif
/// Flag indicating if the FW is compiled for a MAC HW supporting HE
#define NX_MAC_HE (NX_MAC_VER >= 20)
/// Flag indicating if the FW is compiled for a MAC HW implementing a RX ring buffer
#define NX_RX_RING (NX_MAC_VER >= 21)

/** @} MAC */

/**
 *******************************************************************************
 * @name Modem version
 *******************************************************************************
 */
/// Modem version
#ifdef CFG_MDM_VER_V10
  #define NX_MDM_VER 10
#elif defined CFG_MDM_VER_V11
  #define NX_MDM_VER 11
#elif defined CFG_MDM_VER_V20
  #define NX_MDM_VER 20
#elif defined CFG_MDM_VER_V21
  #define NX_MDM_VER 21
#elif defined CFG_MDM_VER_V22
  #define NX_MDM_VER 22
#elif defined CFG_MDM_VER_V30
  #define NX_MDM_VER 30
#elif defined CFG_MDM_VER_V31
  #define NX_MDM_VER 31
#elif defined CFG_MDM_VER_V32
  #define NX_MDM_VER 32
#endif
/** @} Modem */

/**
 *******************************************************************************
 * @name Platform version
 *******************************************************************************
 */
/// Platform version
#ifdef CFG_PLF_VER_V10
  #define NX_PLF_VER 10
#elif defined CFG_PLF_VER_V20
  #define NX_PLF_VER 20
#elif defined CFG_PLF_VER_V30
  #define NX_PLF_VER 30
#endif
/** @} Platform */

/**
 *******************************************************************************
 *   @name General Configuration
 *******************************************************************************
 */
/// Max number of virtual interfaces managed. MAC HW is limited to 6, but the LMAC assumes
/// that this number is a power of 2, thus only 1, 2 or 4 are allowed
#define NX_VIRT_DEV_MAX CFG_VIF_MAX
#if (NX_VIRT_DEV_MAX & (NX_VIRT_DEV_MAX - 1))
  #error "Not a power of 2"
#endif
#if (NX_VIRT_DEV_MAX > 4)
  #error "Max number of VIF shall be 4"
#endif

/// Max number of peer devices managed
#define NX_REMOTE_STA_MAX CFG_STA_MAX

/// Heap size
#define NX_HEAP_SIZE (2048 + 2048 * NX_UMAC_PRESENT + 256 * NX_VIRT_DEV_MAX + 64 * NX_REMOTE_STA_MAX)
/** @} General */

/**
 *******************************************************************************
 *   @name Beacon Configuration
 *******************************************************************************
 */
/// Beaconing support
#ifdef CFG_BCN
  #define NX_BEACONING 1
#else
  #define NX_BEACONING 0
#endif

/// Autonomous beacon transmission support
#ifdef CFG_AUTOBCN
  #define NX_BCN_AUTONOMOUS_TX        1
#else
  #define NX_BCN_AUTONOMOUS_TX        0
#endif

/// Maximum size of a beacon frame
#define NX_BCNFRAME_LEN 512
/** @} Beacon */

/**
 *******************************************************************************
 *   @name Power Save Configuration
 *******************************************************************************
 */
/// UAPSD support
#ifdef CFG_UAPSD
  #define NX_UAPSD 1
  // If UAPSD is enabled, we force the legacy PS mode to be enabled
  #undef CFG_PS
  #define CFG_PS
#else
  #define NX_UAPSD 0
#endif

/// DPSM support
#ifdef CFG_DPSM
  #define NX_DPSM 1
  // If DPSM is enabled, we force the legacy PS mode to be enabled
  #undef CFG_PS
  #define CFG_PS
#else
  #define NX_DPSM 0
#endif

/// Legacy power save support
#ifdef CFG_PS
  #define NX_POWERSAVE 1
#else
  #define NX_POWERSAVE 0
#endif
/** @} Power Save */

/**
 *******************************************************************************
 * @name HE Configuration
 *******************************************************************************
*/
// Disable HE support if FW is compiled for a 11n/ac only HW
#if NX_MDM_VER<30 || NX_MAC_VER<20
  #undef CFG_HE
#endif

// HE not yet supported for softmac
#if !NX_UMAC_PRESENT
  #undef CFG_HE
#endif

/// HE Support
#ifdef CFG_HE
  #define NX_HE 1
  // Force aggregation if HE is enabled
  #undef CFG_AGG
  #define CFG_AGG
  // Force VHT support if HE is enabled
  #undef CFG_VHT
  #define CFG_VHT
  // Force MFP support if HE is enabled, as it is mandatory for WiFi certification
  #undef CFG_MFP
  #define CFG_MFP
  // Force Radio Measurement module
  #define NX_RM 0
#else // !CFG_HE
  #define CFG_MFP
  #define NX_HE 0
  #define NX_RM 0
#endif // CFG_HE

/** @} HE */

/**
 *******************************************************************************
 * @name TWT Configuration
 *******************************************************************************
 */
#if NX_UMAC_PRESENT && NX_HE && CFG_TWT
    /// TWT support
    #define NX_TWT              1
    /// Maximum Number of Flows
    #define NX_TWT_FLOW_NB      CFG_TWT
#else
    #define NX_TWT              0
    #define NX_TWT_FLOW_NB      0
#endif //NX_UMAC_PRESENT && NX_HE && defined CFG_TWT

/** @} TWT */

/**
 *******************************************************************************
 * @name VHT Configuration
 *******************************************************************************
 */
#if NX_UMAC_PRESENT
  /// VHT Support
  #ifdef CFG_VHT
    #define NX_VHT 1
    // Force MFP support if VHT is enabled, as it is mandatory for 802.11ac WiFi certification
    #undef CFG_MFP
    #define CFG_MFP
  #else // !CFG_VHT
    #define NX_VHT 0
  #endif // CFG_VHT
#endif // NX_UMAC_PRESENT
/** @} VHT */

/**
 *******************************************************************************
 * @name A-MSDU Configuration
 *******************************************************************************
 */
/// Define the A-MSDU option for TX
#ifdef CFG_AMSDU
  #define NX_AMSDU_TX 1
  // Force the aggregation to be supported
  #undef CFG_AGG
  #define CFG_AGG
  /// Number of payloads per TX descriptor
  #define NX_TX_PAYLOAD_MAX 6
#else
  #define NX_AMSDU_TX 0
  /// Number of payloads per TX descriptor
  #define NX_TX_PAYLOAD_MAX 0
#endif

/// Maximum size of A-MSDU supported in reception
#ifdef CFG_AMSDU_4K
  #define RWNX_MAX_AMSDU_RX    4096
#elif defined CFG_AMSDU_8K
  #define RWNX_MAX_AMSDU_RX    8192
#elif defined CFG_AMSDU_12K
  #define RWNX_MAX_AMSDU_RX    12288
#endif
/** @} A-MSDU */

/**
 *******************************************************************************
 * @name A-MPDU Configuration
 *******************************************************************************
 */
#ifdef CFG_AGG
  /// A-MPDU TX support
  #define NX_AMPDU_TX 1
  /// A-MPDU TX dynamic choose retry ac support
  #define NX_DYNAMIC_CHOOSE_RETRY_AC 0
  #if NX_UMAC_PRESENT
    /// Maximum number of TX Block Ack
    #define NX_MAX_BA_TX CFG_BATX
    #if (NX_MAX_BA_TX == 0)
      #error "At least one BA TX agreement shall be allowed"
    #endif
  #else  // !NX_UMAC_PRESENT
    #define NX_MAX_BA_TX 0
  #endif // NX_UMAC_PRESENT
#else  // !CFG_AGG
  #define NX_AMPDU_TX  0
  #define NX_DYNAMIC_CHOOSE_RETRY_AC 0
  #define NX_MAX_BA_TX 0
  #undef CFG_BWLEN
  #undef CFG_MU_CNT
  #define CFG_MU_CNT          1
#endif // CFG_AGG

#if NX_UMAC_PRESENT
  /// Maximum number of RX Block Ack
  #define NX_MAX_BA_RX CFG_BARX
  #define NX_AMPDU_RX CFG_BARX
  /// RX Packet Reordering Buffer Size
  #define NX_AMPDU_RX_BUF_SIZE CFG_REORD_BUF
  #if (NX_AMPDU_RX && ((NX_AMPDU_RX_BUF_SIZE < 1) || (NX_AMPDU_RX_BUF_SIZE > 64)))
     #error "Incorrect reordering buffer size"
  #endif
  /// A-MSDU de-aggregation support
  #ifdef CFG_DEAGG
    #define NX_AMSDU_DEAGG 1
  #else
    #define NX_AMSDU_DEAGG 0
  #endif
#else  // !NX_UMAC_PRESENT
  /// RX Packet Reordering support
  #define NX_AMPDU_RX     0
  /// A-MSDU de-aggregation support
  #define NX_AMSDU_DEAGG 0
#endif // NX_UMAC_PRESENT
/** @} A-MPDU */

/**
 *******************************************************************************
 * @name BeamForming Configuration
 *******************************************************************************
 */
// We support only the VHT and HE calibration, so no need to compile the BFMER code if
// neither VHT nor HE is supported by the fullMAC.
// In softMAC we don't know if the host will use VHT/HE or not, so we keep this code
#if NX_UMAC_PRESENT && !NX_HE && !NX_VHT
  #undef CFG_BFMER
#endif  // NX_UMAC_PRESENT && !NX_HE && !NX_VHT

/// Beamformee support
#ifdef CFG_BFMEE
  #define RW_BFMEE_EN 1
#else
  #define RW_BFMEE_EN 0
  // Disable MU-MIMO RX if Beamformee is not supported
  #undef CFG_MU_RX
#endif

#ifdef CFG_BFMER
  /// Beamformer support
  #define RW_BFMER_EN         1
  /// Maximum number of frames reserved for calibration
  #define RW_BFR_TXFRAME_CNT  CFG_MU_CNT
#else // !CFG_BFMER
  #define RW_BFMER_EN         0
  #define RW_BFR_TXFRAME_CNT  0
  // Disable MU-MIMO TX if Beamformer is not supported
  #undef CFG_MU_CNT
  #define CFG_MU_CNT          1
#endif // CFG_BFMER

/// Number of users supported
#define RW_USER_MAX            CFG_MU_CNT
/// MU-MIMO TX support
#define RW_MUMIMO_TX_EN        (RW_USER_MAX > 1)
/// Support for up to one secondary user
#define RW_MUMIMO_SEC_USER1_EN (RW_USER_MAX > 1)
/// Support for up to two secondary users
#define RW_MUMIMO_SEC_USER2_EN (RW_USER_MAX > 2)
/// Support for up to three secondary users
#define RW_MUMIMO_SEC_USER3_EN (RW_USER_MAX > 3)

/// MU-MIMO RX support
#ifdef CFG_MU_RX
  #define RW_MUMIMO_RX_EN 1
#else
  #define RW_MUMIMO_RX_EN 0
#endif
/** @} BeamForming */

/**
 *******************************************************************************
 * @name P2P Configuration
 *******************************************************************************
 */
#if CFG_P2P
  /// P2P support
  #define NX_P2P             1
  /// Maximum number of simultaneous P2P connections
  #define NX_P2P_VIF_MAX     CFG_P2P

  /// P2P GO Support
  #ifdef CFG_P2P_GO
    // AUTOBCN must be supported
    #if !(defined CFG_AUTOBCN)
      #error 'Autonomous Beacon TX feature (AUTOBCN) must be enabled'
    #endif
    #define NX_P2P_GO 1
  #else
    #define NX_P2P_GO 0
  #endif //(GFG_P2P_GO)
#else
  #define NX_P2P         0
  #define NX_P2P_VIF_MAX 0
  #define NX_P2P_GO      0
#endif //(CFG_P2P)
/** @} P2P */

/**
 *******************************************************************************
 * @name MESH Configuration
 *******************************************************************************
 */
#ifdef CFG_MESH
  /// Wireless Mesh Networking support
  #define RW_MESH_EN       (1)
  /// UMAC support for MESH
  #define RW_UMESH_EN      (NX_UMAC_PRESENT)
  /// Maximum Number of
  #define RW_MESH_VIF_NB   (CFG_MESH_VIF)
  /// Maximum number of MESH link
  #define RW_MESH_LINK_NB  (CFG_MESH_LINK)
  /// Maximum number of MESH path
  #define RW_MESH_PATH_NB  (CFG_MESH_PATH)
  /// Maximum number of MESH proxy
  #define RW_MESH_PROXY_NB (CFG_MESH_PROXY)
#else
  #define RW_MESH_EN       (0)
  #define RW_UMESH_EN      (0)
  #define RW_MESH_VIF_NB   (0)
  #define RW_MESH_LINK_NB  (0)
  #define RW_MESH_PATH_NB  (0)
  #define RW_MESH_PROXY_NB (0)
#endif //defined CFG_MESH
/** @} MESH */

/**
 *******************************************************************************
 *   @name TX Configuration
 *******************************************************************************
 */
/// Minimal MPDU spacing we support in TX
#define NX_TX_MPDU_SPACING   CFG_SPC

/// Number of TX queues in the LMAC
#define NX_TXQ_CNT          (AC_MAX + NX_BEACONING)

/// Number of TX descriptors available in the system (BK)
#define NX_TXDESC_CNT0       CFG_TXDESC0
/// Number of TX descriptors available in the system (BE)
#define NX_TXDESC_CNT1       CFG_TXDESC1
/// Number of TX descriptors available in the system (VI)
#define NX_TXDESC_CNT2       CFG_TXDESC2
/// Number of TX descriptors available in the system (VO)
#define NX_TXDESC_CNT3       CFG_TXDESC3
#if (NX_BEACONING)
/// Number of TX descriptors available in the system (BCN)
#define NX_TXDESC_CNT4       CFG_TXDESC4
#endif

/// Number of TX frame descriptors and buffers available for frames generated internally
#define NX_TXFRAME_CNT (NX_VIRT_DEV_MAX + RW_BFR_TXFRAME_CNT + 1)
#if NX_TXFRAME_CNT < 2
#undef NX_TXFRAME_CNT
#define NX_TXFRAME_CNT  2
#endif


/// Maximum size of a TX frame generated internally
#if NX_UMAC_PRESENT
  #if (NX_P2P)
    #define NX_TXFRAME_LEN 384
  #elif NX_RM
    #define NX_TXFRAME_LEN 318
  #else
    #define NX_TXFRAME_LEN 256
  #endif //(NX_P2P)
#else
  #define NX_TXFRAME_LEN   128
#endif

/// Number of TX flow control credits allocated by default per RA/TID (UMAC only)
#define NX_DEFAULT_TX_CREDIT_CNT 32  //default 4
/** @} TX */

/**
 *******************************************************************************
 *   @name RX Configuration
 *******************************************************************************
 */
/// RX Payload buffer size
#define NX_RX_PAYLOAD_LEN    512

/// Maximum number of the longest A-MSDUs that can be stored at the same time
#if NX_UMAC_PRESENT && NX_VHT
  #define NX_RX_LONG_MPDU_CNT  3
#else
  #define NX_RX_LONG_MPDU_CNT  2
#endif

/// Number of RX payload descriptors - defined to be n times the maximum A-MSDU size
/// plus one extra one used for HW flow control
#define NX_RX_PAYLOAD_DESC_CNT ((RWNX_MAX_AMSDU_RX / NX_RX_PAYLOAD_LEN) * NX_RX_LONG_MPDU_CNT + 1)

/// Number of RX descriptors (SW and Header descriptors)
#define NX_RXDESC_CNT NX_RX_PAYLOAD_DESC_CNT

#if NX_AMSDU_DEAGG
/// Maximum number MSDUs supported in one received A-MSDU
#define NX_MAX_MSDU_PER_RX_AMSDU 8
#endif

/// Number of platform DMA descriptors in each RX payload descriptor
#if NX_AMSDU_DEAGG
#define NX_DMADESC_PER_RX_PDB_CNT NX_MAX_MSDU_PER_RX_AMSDU
#else
#define NX_DMADESC_PER_RX_PDB_CNT 1
#endif

/** @} RX */

/**
 *******************************************************************************
 * @name Radar Configuration
 *******************************************************************************
 */
#ifdef CFG_RADAR
  /// Radar enable software define
  #define NX_RADAR_DETECT 1
  /// Radar enable hardware define (i.e. used in registers file)
  #define RW_RADAR_EN     1
#else
  #define NX_RADAR_DETECT 0
  #define RW_RADAR_EN     0
#endif
/** @} Radar */

/**
 *******************************************************************************
 * @name Unsupported HT Frame Logging Configuration
 *******************************************************************************
 */
#ifdef CFG_UF
  #if (NX_MDM_VER >= 20) && (NX_MDM_VER < 30)
    /// Unsupported HT Frame Logging enable
    #define NX_UF_EN 1
  #else
    #define NX_UF_EN 0
  #endif
#else
  #define NX_UF_EN 0
#endif
/** @} Unsupported HT Frame Logging */

/**
 *******************************************************************************
 * @name Monitor + Data interface Support
 *******************************************************************************
 */
#if NX_UMAC_PRESENT && defined CFG_MON_DATA
  #define NX_MON_DATA 1
#else
  #define NX_MON_DATA 0
#endif
/** @} Monitor + Data interface Support */

/**
 *******************************************************************************
 * @name FTM Configuration
 * @{
 *******************************************************************************
 */
#if NX_UMAC_PRESENT
  /// FTM intiator Support
  #if CFG_FTM_INIT
    #define NX_FTM_INITIATOR 1
  #else // !CFG_FTM_INIT
    #define NX_FTM_INITIATOR 0
    #undef CFG_FTM_RSP
  #endif // CFG_FTM_INIT

  /// Fake FTM responder Support
  #if CFG_FTM_RSP
    #define NX_FAKE_FTM_RSP 0
  #else // !CFG_FTM_RSP
    #define NX_FAKE_FTM_RSP 0
  #endif // CFG_FTM_RSP
#else
#undef CFG_FTM_INIT
#undef CFG_FTM_RSP
/// FTM intiator Support
#define NX_FTM_INITIATOR 0
/// Fake FTM responder Support
#define NX_FAKE_FTM_RSP 0
#endif // NX_UMAC_PRESENT
/** @} FTM */

/**
 *******************************************************************************
 * @name Debug Configuration
 *******************************************************************************
 */
/// Debug dump forwarding
#ifdef CFG_DBGDUMP
  #define NX_DEBUG_DUMP 1
#else
  #define NX_DEBUG_DUMP 0
#endif

/// Debug key RAM forwarding
#ifdef CFG_DBGDUMPKEY
  #define NX_DEBUG_DUMP_KEY 1
#else
  #define NX_DEBUG_DUMP_KEY 0
#endif

/// Debug support
#define NX_DEBUG  CFG_DBG
#define RW_BUS_HANG_WHEN_DOZE_DBG  CFG_DBG

/// Debug print output
#if NX_DEBUG
  #define NX_PRINT NX_PRINT_PRINTF
  #define TEMP_DEBUG 1
  #define BK_SAVE_DIAGS_INFO 1
  #define DBG_TX_TIMEOUT 1
#else
  #define NX_PRINT NX_PRINT_NONE
  #define TEMP_DEBUG 0
  #define BK_SAVE_DIAGS_INFO 0
  #define DBG_TX_TIMEOUT 0
#endif

/// Trace Buffer Support
#ifdef CFG_TRACE
  #define NX_TRACE 1
#else
  #define NX_TRACE 0
#endif

/// Profiling support
#ifdef CFG_PROF
  #define NX_PROFILING_ON 1
#else
  #define NX_PROFILING_ON 0
#endif

/// System statistics support
#ifdef CFG_STATS
  #define NX_SYS_STAT 1
#else
  #define NX_SYS_STAT 0
#endif
/** @} Debug */

/**
 *******************************************************************************
 * @name Extra Configuration
 *******************************************************************************
 */
/// Recovery support
#ifdef CFG_REC
  #define NX_RECOVERY 1
#else
  #define NX_RECOVERY 0
#endif

/// Connection monitoring support
#ifdef CFG_CMON
  #define NX_CONNECTION_MONITOR 1
#else
  #define NX_CONNECTION_MONITOR 0
#endif

/// Multi-role support (AP+STA, STA+STA)
#ifdef CFG_MROLE
  #define NX_MULTI_ROLE 1
#else
  #define NX_MULTI_ROLE 0
#endif

/// HW scan support
#ifdef CFG_HWSCAN
  #define NX_HW_SCAN 1
#else
  #define NX_HW_SCAN 0
#endif

/// MFP support (for UMAC only)
#if NX_UMAC_PRESENT && defined CFG_MFP
  #define NX_MFP 1
#else
  #define NX_MFP 0
#endif

/// WAPI support
#ifdef CFG_WAPI
  #define RW_WAPI_EN 1
#else
  #define RW_WAPI_EN 0
#endif

/// WLAN coexistence support
#ifdef CFG_COEX
  #define RW_WLAN_COEX_EN 1
#else
  #define RW_WLAN_COEX_EN 0
#endif

/// Compilation for a HW supporting Key RAM configuration
#ifdef CFG_KEYCFG
  #define NX_KEY_RAM_CONFIG 1
#else
  #define NX_KEY_RAM_CONFIG 0
#endif

/// TDLS support
#ifdef CFG_TDLS
  #define NX_TDLS 1
#else
  #define NX_TDLS 0
#endif

#ifdef CFG_BWLEN
  /// per-BW length adaptation support
  #define NX_BW_LEN_ADAPT 1
  /// Number of steps for BW length adaptation
  #define NX_BW_LEN_STEPS 4
#else
  #define NX_BW_LEN_ADAPT 0
  #define NX_BW_LEN_STEPS 1
#endif

/// HSU support. Possible values are:
/// - 0: Don't use HSU, use software implementation.
/// - 1: Use HSU and fallback to software implementation if not available.
/// - 2: Only use HSU. (runtime error is generated if HSU is not available)
#ifdef CFG_HSU
  #define NX_HSU (CFG_HSU)
  #define RW_HSU_SHA_EN 1
  #define RW_HSU_IP_CHK_EN 1
  #define RW_HSU_RSA_EN 1
  #define RW_HSU_EPM_EN 1
#else
  #define NX_HSU 0
#endif

/// Antenna Diversity support
#if NX_UMAC_PRESENT && defined CFG_ANT_DIV && (NX_MDM_VER < 30)
  #define NX_ANT_DIV 1
#else
  #define NX_ANT_DIV 0
#endif

/// Traffic generator support
#ifdef CFG_TG
  #define NX_TG 1
#else
  #define NX_TG 0
#endif

/// Monitor mode support
#ifdef CFG_FHOST_MONITOR
  #define NX_FHOST_MONITOR 1
#else
  #define NX_FHOST_MONITOR 0
#endif

/// Smartconfig support
#ifdef CFG_SMARTCONFIG
  #undef NX_FHOST_MONITOR
  #define NX_FHOST_MONITOR 1
  #define NX_SMARTCONFIG 1
#else
  #define NX_SMARTCONFIG 0
#endif

/// External crypto lib compiled
#ifdef CFG_CRYPTO
  #define NX_CRYPTOLIB 1
#else
  #define NX_CRYPTOLIB 0
#endif

/** @} Extra */

/**
 *******************************************************************************
 * @name Fw Features Configuration
 * Features automatically enabled if required by the selected configuration
 *******************************************************************************
 */
/// Channel Contexts support
#define NX_CHNL_CTXT (NX_HW_SCAN || NX_P2P)
/// Maximum number of operating channel contexts
#define NX_CHAN_CTXT_CNT 3

/// General Purpose DMA module
#if BK_MAC
#define NX_GP_DMA 0
#else
#define NX_GP_DMA (NX_CHNL_CTXT || NX_BCN_AUTONOMOUS_TX || NX_RADAR_DETECT || RW_BFMER_EN)
#endif

/// Internal handling of received frame
#define NX_RX_FRAME_HANDLING (NX_POWERSAVE || NX_CONNECTION_MONITOR || NX_UMAC_PRESENT || RW_BFMER_EN)

/// Internal frame generation support
#define NX_TX_FRAME (NX_POWERSAVE || NX_CONNECTION_MONITOR || NX_CHNL_CTXT || NX_BCN_AUTONOMOUS_TX || RW_BFMER_EN)

/// Traffic Detection per STA support
#define NX_TD_STA (RW_BFMER_EN || NX_TDLS || NX_HE)

/// Traffic Detection support
#define NX_TD (NX_DPSM || NX_CHNL_CTXT || NX_P2P_GO || NX_TD_STA)

/// MAC Management timer support
#define NX_MM_TIMER (NX_MULTI_ROLE || NX_CHNL_CTXT || NX_DPSM || NX_P2P || NX_UAPSD || RW_BFMER_EN || NX_BEACONING || NX_HE)

/** @} Features */

/**
 *******************************************************************************
 * @name Misc
 *******************************************************************************
 */
/// Implement IEEE802.11ax-D2.0 (instead of D3.2)
#define NX_11AX_DRAFT_2_0               0

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_LDPC_DEC                  1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_AGC_SNR_EN                1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_IQ_COMP_EN                1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_FIQ_COMP_EN               1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_DERIV_80211B              1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_KEY_EXTENDED                 1
/// @} misc

#if BK_MAC
#define BK_NX_RC                        1

/**
 *******************************************************************************
 * @name BEKEN Additional feature
 *******************************************************************************
 */

/// add peak cancel function for tx data path
#define BK_NX_PEAK_CANCEL               1

/// add DPD for PA
#define BK_NX_DPD_EN                    0

/// add Power Table for PA
#define BK_NX_PWRTBL_EN                 0

/// test ahb access phy memory
#define TEST_AHB_ACCESS_PHY_MEM_EN      0

/// CSI
#if CONFIG_WIFI_CSI_EN
#define RW_NX_CSI                       1
#endif
/// minimize rx buffer size: rx buffer is allocated after the mpdu len is
/// calculated and wants to push to rx queue
#define BK_MIN_RX_BUFSZ                 1

/// fast connect with AP by bypassing join procedure
#if CONFIG_SM_FAST_CONNECT
#define SM_FAST_CONNECT                 1
#endif

/// Enable Software TX Queue
#if CONFIG_RWNX_SW_TXQ
#define NX_SW_TXQ                       1
#endif

/// @} beken
/// auto set keep alive period
#if CONFIG_MAC_RESET_BCMC_AUTO_MODE
#define MAC_RESET_BCMC_AUTO_MODE        1
#endif

/// auto set keep alive period
#if CONFIG_MAC_KEEP_ALIVE_PERIOD_AUTO_MODE
#define MAC_KEEP_ALIVE_PERIOD_AUTO_MODE 1
#endif

//Enable when mqtt and arp need align each other
#if CONFIG_MQTT_ARP_ALIGN_EN
#define MQTT_ARP_ALIGN_EN               1
#endif

/// protect dhcp in connected by receiving every DTIM
#if CONFIG_DHCP_PROTECT_IN_CONNECTED
#define DHCP_PROTECT_IN_CONNECTED       1
#endif

/// protect tcp in connected by receiving every DTIM
#if CONFIG_TCP_PROTECT_IN_CONNECTED
#define TCP_PROTECT_IN_CONNECTED        1
#endif

/// Optimize memory usage in macsw
#if CONFIG_BK_MEMORY_CUT_EN
#define BK_MEMORY_CUT_EN                1
#endif

/// enable HW beacon TIM element parser
#if CONFIG_NX_HW_PARSER_TIM_ELEMENT
#define NX_HW_PARSER_TIM_ELEMENT        1
#endif

/// read tim_beacon peer tsf ande local time from the register
#if CONFIG_TIM_TSF_FROM_REG
#define MM_TIM_TSF_FORM_REG        1
#endif

/// Dynamically changes CPU frequency
#if CONFIG_MAC_SUPPORT_DYNAMIC_CPU_FREQ
#define MAC_SUPPORT_DYNAMIC_CPU_FREQ    1
#endif

/// PM rosc calibration
#if CONFIG_MAC_PM_ROSC_CALI
#define MAC_PM_ROSC_CALI                0
#endif

/// Dynamic changes listen interval when media occurred
#if CONFIG_NX_MEDIA_DYNC_DTIM
#define NX_MEDIA_DYNC_DTIM              1
#endif

#if CONFIG_PER_PACKET_INFO_OUTPUT
#define PER_PACKET_INFO_OUTPUT          1
#endif

/// Enable frame retry in macsw
#if CONFIG_MAC_SFRAME_SOFTWARE_RETRY
#define MAC_SFRAME_SOFTWARE_RETRY       1
#endif

/// Debug dynamically: Read MAC/PHY debug port and save diag values to memory,
/// and save temporarily values
#if CONFIG_BK_MEM_DYNA_APPLY_EN
#define BK_MEM_DYNA_APPLY_EN            1
#endif

/// Dynamic changes listen interval when OTA
#if CONFIG_NX_OTA_DYNC_DTIM
#define NX_OTA_DYNC_DTIM                1
#endif

/// Dynamic changes listen interval when PS Null send fail
#define NX_PSNULL_FAIL_DYNC_DTIM        1

#if CONFIG_BK_NO_CHECK_RX_PN
#define BK_NO_CHECK_RX_PN               1
#endif
// interoperability
#if CONFIG_BK_INTEROPER
#define BK_INTEROPER                    1
#endif

//Dynamic changes td window
#define MAC_SUPPORT_DYNAMIC_TD_WINDOW   1

//use ckmn for rosc
#define USE_CKMN                        1

#endif
#endif // _RWNX_CONFIG_H_
/// @} MACSW


