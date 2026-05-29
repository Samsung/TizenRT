/**
  ******************************************************************************
  * @file    inic_ipc_cfg.h
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

#ifndef __AMEBA_INIC_IPC_CFG_H__
#define __AMEBA_INIC_IPC_CFG_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */

/* internal head files */
#include "rtw_wifi_common.h"

/* -------------------------------- Defines --------------------------------- */

#if defined(CONFIG_AMEBASMART)

#if defined(CONFIG_ARM_CORE_CA32)
#define IPC_DIR_MSG_RX IPC_NP_TO_AP
#define IPC_DIR_MSG_TX IPC_AP_TO_NP
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#if defined(CONFIG_WHC_DEV)
/* configuration for device */
#define IPC_H2D_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#elif defined(CONFIG_WHC_HOST)
/* configuration for host */
#define IPC_D2H_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#endif
#elif defined(CONFIG_ARM_CORE_CM4)
#define IPC_DIR_MSG_RX IPC_AP_TO_NP
#define IPC_DIR_MSG_TX IPC_NP_TO_AP
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#if defined(CONFIG_WHC_HOST)
/* configuration for host */
#define IPC_D2H_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#elif defined(CONFIG_WHC_DEV)
/* configuration for device */
#define IPC_H2D_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBALITE)
#if defined(CONFIG_RSICV_CORE_KR4)
#if defined(CONFIG_WHC_HOST)
/* configuration for AP */
#define IPC_DIR_MSG_RX IPC_KM4_TO_KR4
#define IPC_DIR_MSG_TX IPC_KR4_TO_KM4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_R2M_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_TRX_TRAN	IPC_M2R_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#elif defined(CONFIG_WHC_DEV)
/* configuration for NP */
#define IPC_DIR_MSG_RX IPC_KM4_TO_KR4
#define IPC_DIR_MSG_TX IPC_KR4_TO_KM4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_R2M_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_TRX_TRAN	IPC_M2R_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#endif
#elif defined(CONFIG_ARM_CORE_CM4)
#if defined(CONFIG_WHC_HOST)
/* configuration for AP */
#define IPC_DIR_MSG_RX IPC_KR4_TO_KM4
#define IPC_DIR_MSG_TX IPC_KM4_TO_KR4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_M2R_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_TRX_TRAN	IPC_R2M_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#elif defined(CONFIG_WHC_DEV)
/* configuration for NP */
#define IPC_DIR_MSG_RX IPC_KR4_TO_KM4
#define IPC_DIR_MSG_TX IPC_KM4_TO_KR4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_M2R_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_TRX_TRAN	IPC_R2M_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_R2M_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_M2R_WIFI_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBADPLUS)
#if defined(CONFIG_ARM_CORE_CM0)
#define IPC_DIR_MSG_RX IPC_KM4_TO_KM0
#define IPC_DIR_MSG_TX IPC_KM0_TO_KM4
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#if defined(CONFIG_WHC_DEV)
/* configuration for device */
#define IPC_H2D_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#endif
#elif defined(CONFIG_ARM_CORE_CM4)
#define IPC_DIR_MSG_RX IPC_KM0_TO_KM4
#define IPC_DIR_MSG_TX IPC_KM4_TO_KM0
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#if defined(CONFIG_WHC_HOST)
/* configuration for host */
#define IPC_D2H_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN IPC_N2A_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN IPC_A2N_WIFI_API_TRAN
#endif
#endif

#elif defined(CONFIG_AMEBAGREEN2) || defined(CONFIG_AMEBAPRO3) || defined(CONFIG_AMEBAL2) || defined(CONFIG_RTL8720F)
#if defined(CONFIG_ARM_CORE_CM4)
#if defined(CONFIG_WHC_HOST)
/* configuration for AP */
#define IPC_DIR_MSG_RX IPC_NP_TO_AP
#define IPC_DIR_MSG_TX IPC_AP_TO_NP
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_A2N_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_TRX_TRAN	IPC_N2A_WIFI_TRX_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_N2A_WIFI_API_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_A2N_WIFI_API_TRAN
#elif defined(CONFIG_WHC_DEV)
/* configuration for NP */
#define IPC_DIR_MSG_RX IPC_AP_TO_NP
#define IPC_DIR_MSG_TX IPC_NP_TO_AP
#define IPC_INT_CHAN_WIFI_TRX_TRAN IPC_N2A_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_TRX_TRAN	IPC_A2N_WIFI_TRX_TRAN
#define IPC_H2D_WIFI_API_TRAN	IPC_A2N_WIFI_API_TRAN
#define IPC_D2H_WIFI_API_TRAN	IPC_N2A_WIFI_API_TRAN
#endif
#endif


#endif

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */

#endif /* __AMEBA_INIC_IPC_CFG_H__ */

