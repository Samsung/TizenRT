/**
  ******************************************************************************
  * @file    rtw_coex_ipc_cfg.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __RTW_COEX_IPC_CFG_H__
#define __RTW_COEX_IPC_CFG_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */

/* internal head files */
#include "rtw_wifi_common.h"

/* -------------------------------- Defines --------------------------------- */

#if defined(CONFIG_AMEBASMART)

#if defined(CONFIG_ARM_CORE_CA32)
#define COEX_IPC_DIR_MSG_RX IPC_NP_TO_AP
#define COEX_IPC_DIR_MSG_TX IPC_AP_TO_NP
#if defined(CONFIG_COEXIST_HOST)
/* configuration for host */
#define IPC_D2H_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_DEV)
/* configuration for device */
#define IPC_H2D_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#endif
#elif defined(CONFIG_ARM_CORE_CM4)
#define COEX_IPC_DIR_MSG_RX IPC_AP_TO_NP
#define COEX_IPC_DIR_MSG_TX IPC_NP_TO_AP
#if defined(CONFIG_COEXIST_HOST)
/* configuration for host */
#define IPC_D2H_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_DEV)
/* configuration for device */
#define IPC_H2D_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBALITE)
#if defined(CONFIG_RSICV_CORE_KR4)
#if defined(CONFIG_COEXIST_HOST)
/* configuration for AP */
#define COEX_IPC_DIR_MSG_RX IPC_KM4_TO_KR4
#define COEX_IPC_DIR_MSG_TX IPC_KR4_TO_KM4
#define IPC_D2H_COEX_API_TRAN	IPC_M2R_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN	IPC_R2M_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_DEV)
/* configuration for NP */
#define COEX_IPC_DIR_MSG_RX IPC_KM4_TO_KR4
#define COEX_IPC_DIR_MSG_TX IPC_KR4_TO_KM4
#define IPC_H2D_COEX_API_TRAN	IPC_M2R_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN	IPC_R2M_COEX_API_TRAN
#endif
#elif defined(CONFIG_ARM_CORE_CM4)
#if defined(CONFIG_COEXIST_HOST)
/* configuration for AP */
#define COEX_IPC_DIR_MSG_RX IPC_KR4_TO_KM4
#define COEX_IPC_DIR_MSG_TX IPC_KM4_TO_KR4
#define IPC_D2H_COEX_API_TRAN	IPC_R2M_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN	IPC_M2R_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_DEV)
/* configuration for NP */
#define COEX_IPC_DIR_MSG_RX IPC_KR4_TO_KM4
#define COEX_IPC_DIR_MSG_TX IPC_KM4_TO_KR4
#define IPC_H2D_COEX_API_TRAN	IPC_R2M_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN	IPC_M2R_COEX_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBADPLUS)
#if defined(CONFIG_ARM_CORE_CM0)
#define COEX_IPC_DIR_MSG_RX IPC_KM4_TO_KM0
#define COEX_IPC_DIR_MSG_TX IPC_KM0_TO_KM4
#if defined(CONFIG_COEXIST_DEV)
/* configuration for device */
#define IPC_H2D_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_HOST)
/* configuration for host */
#define IPC_D2H_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#endif
#elif defined(CONFIG_ARM_CORE_CM4)
#define COEX_IPC_DIR_MSG_RX IPC_KM0_TO_KM4
#define COEX_IPC_DIR_MSG_TX IPC_KM4_TO_KM0
#if defined(CONFIG_COEXIST_HOST)
/* configuration for host */
#define IPC_D2H_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_DEV)
/* configuration for device */
#define IPC_H2D_COEX_API_TRAN IPC_N2A_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN IPC_A2N_COEX_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBAGREEN2) || defined(CONFIG_AMEBAPRO3) || defined(CONFIG_AMEBAL2)
#if defined(CONFIG_ARM_CORE_CM4)
#if defined(CONFIG_COEXIST_HOST)
/* configuration for AP */
#define COEX_IPC_DIR_MSG_RX IPC_NP_TO_AP
#define COEX_IPC_DIR_MSG_TX IPC_AP_TO_NP
#define IPC_D2H_COEX_API_TRAN	IPC_N2A_COEX_API_TRAN
#define IPC_H2D_COEX_API_TRAN	IPC_A2N_COEX_API_TRAN
#elif defined(CONFIG_COEXIST_DEV)
/* configuration for NP */
#define COEX_IPC_DIR_MSG_RX IPC_AP_TO_NP
#define COEX_IPC_DIR_MSG_TX IPC_NP_TO_AP
#define IPC_H2D_COEX_API_TRAN	IPC_A2N_COEX_API_TRAN
#define IPC_D2H_COEX_API_TRAN	IPC_N2A_COEX_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBAD)
// add for compile, no actual use
#define COEX_IPC_DIR_MSG_RX 0
#define COEX_IPC_DIR_MSG_TX 0
#define IPC_H2D_COEX_API_TRAN	0
#define IPC_D2H_COEX_API_TRAN	0

#else
#error "Please Add Proper Platform Setting!!!"
#endif

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */

#endif /* __RTW_COEX_IPC_CFG_H__ */

