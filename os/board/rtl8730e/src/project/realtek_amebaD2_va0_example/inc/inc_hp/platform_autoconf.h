
/*
 * < CONFIG BOOT OPTION
 */
#undef  CONFIG_XIP_FLASH

/*
 * < CONFIG SOC PS
 */
#define CONFIG_SOC_PS_EN 1
#define CONFIG_SOC_PS_MODULE 1

/*
 * < CONFIG BT
 */
#undef  CONFIG_BT_MENU
#undef  CONFIG_BT
#undef  CONFIG_BT_AP
#undef  CONFIG_BT_NP
#undef  CONFIG_BT_SINGLE_CORE

/*
 * < CONFIG WIFI
 */
#define CONFIG_WLAN_MENU 1
#define CONFIG_AS_INIC_KM4_NP_CA32_AP 1
#undef  CONFIG_SINGLE_CORE_WIFI_KM4
#define CONFIG_WLAN 1
#define CONFIG_AS_INIC_NP 1
#undef  CONFIG_HIGH_TP_TEST


/*
 * < MENUCONFIG FOR HP CONFIG
 */

/*
 * < CONFIG CHIP
 */
#define CONFIG_AMEBAD2 1
#define ARM_CORE_CM4 1
#define CONFIG_FPGA 1
#undef  CONFIG_RL6678_A_CUT
#define CONFIG_RL6678_B_CUT 1

/*
 * < CONFIG TEST MODE
 */
#undef  CONFIG_MP
#undef  CONFIG_CP
#undef  CONFIG_FT
#undef  CONFIG_OLT
#undef  CONFIG_EQC
#undef  CONFIG_QA
#undef  CONFIG_CornerTest
#undef  CONFIG_EMC
#undef  CONFIG_POST_SIM

/*
 * < CONFIG TrustZone
 */
#undef  CONFIG_TRUSTZONE

/*
 * < CONFIG OS
 */
#define CONFIG_KERNEL 1
#define PLATFORM_FREERTOS 1
#define TASK_SCHEDULER_DISABLED (0)

/*
 * < CONFIG USB
 */
#undef  CONFIG_USB_OTG_EN

/*
 * < MBED_API
 */
#undef  CONFIG_MBED_API_EN

/*
 * < CONFIG FUNCTION TEST
 */
#undef  CONFIG_PER_TEST

/*
 * < CONFIG SECURE TEST
 */
#undef  CONFIG_SEC_VERIFY

/*
 * < CONFIG 802154
 */
#undef  CONFIG_802154_PHY_EN

/*
 * < SSL Config
 */
#define CONFIG_USE_MBEDTLS_ROM 1
#define CONFIG_MBED_TLS_ENABLED 1
#undef  CONFIG_SSL_ROM_TEST

/*
 * < GUI Config
 */
#undef  CONFIG_GUI_EN

/*
 * < Audio Config
 */
#undef  CONFIG_AUDIO_FWK

/*
 * < IPC Message Queue Config
 */
#undef  CONFIG_IMQ

/*
 * < CLINTWOOD Config
 */
#undef  CONFIG_CLINTWOOD

/*
 * To set debug msg flag
 */
#define CONFIG_DEBUG_LOG 1

/*
 * < Build Option
 */
#define CONFIG_TOOLCHAIN_ASDK 1
#undef  CONFIG_TOOLCHAIN_ARM_GCC
#undef  CONFIG_LINK_ROM_LIB
#define CONFIG_LINK_ROM_SYMB 1
/*
 * < CONFIG IAR PROJECT
 */
#if defined(CONFIG_INIC_IPC_DISABLE) && (CONFIG_INIC_IPC_DISABLE == 1)
#ifdef CONFIG_INIC_EN
#undef  CONFIG_INIC_EN
#endif
#ifdef CONFIG_HIGH_TP_TEST
#undef  CONFIG_HIGH_TP_TEST
#endif
#ifdef CONFIG_INIC_IPC
#undef CONFIG_INIC_IPC
#endif
#ifdef CONFIG_INIC_IPC_HIGH_TP
#undef CONFIG_INIC_IPC_HIGH_TP
#endif
#ifdef CONFIG_AS_INIC_NP
#undef CONFIG_AS_INIC_NP
#endif
#define CONFIG_SINGLE_CORE_WIFI 1
#define CONFIG_LWIP_LAYER 1
#endif
