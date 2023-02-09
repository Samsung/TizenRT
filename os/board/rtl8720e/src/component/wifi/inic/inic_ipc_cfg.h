/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 * this is the configuration file for AmebaD2 inic driver.
 ******************************************************************************/
#ifndef __AMEBA_INIC_IPC_CFG_H__
#define __AMEBA_INIC_IPC_CFG_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */

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
#define IPC_DIR_MSG_RX IPC_NP_TO_AP
#define IPC_DIR_MSG_TX IPC_AP_TO_NP
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#if defined(CONFIG_AS_INIC_NP)
/* configuration for device */
#define IPC_H2D_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#elif defined(CONFIG_AS_INIC_AP)
/* configuration for host */
#define IPC_D2H_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#endif
#elif defined(ARM_CORE_CM4)
#define IPC_DIR_MSG_RX IPC_AP_TO_NP
#define IPC_DIR_MSG_TX IPC_NP_TO_AP
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#if defined(CONFIG_AS_INIC_AP)
/* configuration for host */
#define IPC_D2H_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#elif defined(CONFIG_AS_INIC_NP)
/* configuration for device */
#define IPC_H2D_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#endif
#endif

#elif defined(CONFIG_PLATFORM_AMEBALITE)
#if defined(RSICV_CORE_KR4)
#if defined(CONFIG_AS_INIC_AP)
/* configuration for AP */
#define IPC_DIR_MSG_RX IPC_KM4_TO_KR4
#define IPC_DIR_MSG_TX IPC_KR4_TO_KM4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_R2M_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_TRX_TRAN	IPC_M2R_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#elif defined(CONFIG_AS_INIC_NP)
/* configuration for NP */
#define IPC_DIR_MSG_RX IPC_KM4_TO_KR4
#define IPC_DIR_MSG_TX IPC_KR4_TO_KM4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_R2M_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_TRX_TRAN	IPC_M2R_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#endif
#elif defined(ARM_CORE_CM4)
#if defined(CONFIG_AS_INIC_AP)
/* configuration for AP */
#define IPC_DIR_MSG_RX IPC_KR4_TO_KM4
#define IPC_DIR_MSG_TX IPC_KM4_TO_KR4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_M2R_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_TRX_TRAN	IPC_R2M_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#elif defined(CONFIG_AS_INIC_NP)
/* configuration for NP */
#define IPC_DIR_MSG_RX IPC_KR4_TO_KM4
#define IPC_DIR_MSG_TX IPC_KM4_TO_KR4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_M2R_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_TRX_TRAN	IPC_R2M_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#endif
#endif

#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
#if defined(ARM_CORE_CM0)
#define IPC_DIR_MSG_RX IPC_KM4_TO_KM0
#define IPC_DIR_MSG_TX IPC_KM0_TO_KM4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#if defined(CONFIG_AS_INIC_NP)
/* configuration for device */
#define IPC_H2D_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#endif
#elif defined(ARM_CORE_CM4)
#define IPC_DIR_MSG_RX IPC_KM0_TO_KM4
#define IPC_DIR_MSG_TX IPC_KM4_TO_KM0
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#if defined(CONFIG_AS_INIC_AP)
/* configuration for host */
#define IPC_D2H_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#endif
#endif

#endif

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */

#endif /* __AMEBA_INIC_IPC_CFG_H__ */

