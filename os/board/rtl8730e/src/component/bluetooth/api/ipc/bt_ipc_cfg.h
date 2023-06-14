/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 * this is the configuration file for AmebaD2/AmebaLite bt ipc driver.
 ******************************************************************************/
#ifndef __BT_IPC_CFG_H__
#define __BT_IPC_CFG_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include <platform_opts_bt.h>
/* internal head files */

/* -------------------------------- Defines --------------------------------- */

#if defined(CONFIG_PLATFORM_8721D)
#if defined(ARM_CORE_CM0)
/* configuration for LP */
#elif defined(ARM_CORE_CM4)
/* configuration for HP */
#endif /* ARM_CORE_CM0 */

#elif defined(CONFIG_PLATFORM_AMEBAD2)

#if defined(ARM_CORE_CA32)
#define BT_IPC_DIR_EVENT_RX IPC_NP_TO_AP
#define BT_IPC_DIR_API_TX IPC_AP_TO_NP
#define BT_IPC_DIR_DATA_RX IPC_NP_TO_AP
#define BT_IPC_DIR_DATA_TX IPC_AP_TO_NP
#if defined(CONFIG_BT_NP)
/* configuration for device */
#define BT_IPC_D2H_API_TRAN IPC_A2N_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_N2A_BT_API_TRAN
#define BT_IPC_D2H_DATA_TRAN IPC_A2N_BT_DATA_TRAN
#define BT_IPC_H2D_DATA_TRAN IPC_N2A_BT_DATA_TRAN
#elif defined(CONFIG_BT_AP)
/* configuration for host */
#define BT_IPC_D2H_API_TRAN IPC_N2A_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_A2N_BT_API_TRAN
#define BT_IPC_D2H_DATA_TRAN IPC_N2A_BT_DATA_TRAN
#define BT_IPC_H2D_DATA_TRAN IPC_A2N_BT_DATA_TRAN
#endif
#elif defined(ARM_CORE_CM4)
#define BT_IPC_DIR_EVENT_RX IPC_AP_TO_NP
#define BT_IPC_DIR_API_TX IPC_NP_TO_AP
#define BT_IPC_DIR_DATA_RX IPC_AP_TO_NP
#define BT_IPC_DIR_DATA_TX IPC_NP_TO_AP
#if defined(CONFIG_BT_AP)
/* configuration for host */
#define BT_IPC_D2H_API_TRAN IPC_A2N_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_N2A_BT_API_TRAN
#define BT_IPC_D2H_DATA_TRAN IPC_A2N_BT_DATA_TRAN
#define BT_IPC_H2D_DATA_TRAN IPC_N2A_BT_DATA_TRAN
#elif defined(CONFIG_BT_NP)
/* configuration for device */
#define BT_IPC_D2H_API_TRAN IPC_N2A_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_A2N_BT_API_TRAN
#define BT_IPC_D2H_DATA_TRAN IPC_N2A_BT_DATA_TRAN
#define BT_IPC_H2D_DATA_TRAN IPC_A2N_BT_DATA_TRAN
#endif
#endif

#elif defined(CONFIG_PLATFORM_AMEBALITE)
#if defined(RSICV_CORE_KR4)
#define BT_IPC_DIR_EVENT_RX IPC_KM4_TO_KR4
#define BT_IPC_DIR_API_TX IPC_KR4_TO_KM4
#if defined(CONFIG_BT_AP)
/* configuration for AP */
#define BT_IPC_D2H_API_TRAN IPC_M2R_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_R2M_BT_API_TRAN
#elif defined(CONFIG_BT_NP)
/* configuration for NP */
#define BT_IPC_D2H_API_TRAN IPC_R2M_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_M2R_BT_API_TRAN
#endif
#elif defined(ARM_CORE_CM4)
#define BT_IPC_DIR_EVENT_RX IPC_KR4_TO_KM4
#define BT_IPC_DIR_API_TX IPC_KM4_TO_KR4
#if defined(CONFIG_BT_AP)
/* configuration for AP */
#define BT_IPC_D2H_API_TRAN IPC_R2M_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_M2R_BT_API_TRAN
#elif defined(CONFIG_BT_NP)
/* configuration for NP */
#define BT_IPC_D2H_API_TRAN IPC_M2R_BT_API_TRAN
#define BT_IPC_H2D_API_TRAN IPC_R2M_BT_API_TRAN
#endif
#endif
#endif

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */

#endif /* __BT_IPC_CFG_H__ */

