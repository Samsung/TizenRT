/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _TMAS_DEF_H_
#define _TMAS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */
#include "ble_audio_def.h"

/**
 * \defgroup    LEA_USE_CASE_TMAP_Def Telephony and Media Audio Profile Definition
 *
 * \brief   Telephony and Media Audio Profile related definition.
 */

/**
 * \defgroup    TMAP_Def_Exported_Macros Telephony and Media Audio Profile Definition Exported Macros
 *
 * \ingroup LEA_USE_CASE_TMAP_Def
 * \{
 */

/**
 * tmas_def.h
 *
 * \defgroup    TMAP_UUID TMAP Service UUID
 *
 * \brief  Define Telephony and Media Audio Service UUID.
 *
 * \ingroup TMAP_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_TMAS                           0x1855

#define TMAS_UUID_CHAR_ROLE                      0x2B51     /**< TMAP Role. */
/**
 * End of TMAP_UUID
 * \}
 */

/**
 * tmas_def.h
 *
 * \defgroup  TMAP_ROLE_BIT_MASK  TMAP Role Bit Mask.
 *
 * \brief  Define TMAP Role Bit Mask.
 *
 * \ingroup TMAP_Def_Exported_Macros
 * \{
 */
#define TMAS_CG_ROLE                0x01     /**< Call Gateway (CG). */
#define TMAS_CT_ROLE                0x02     /**< Call Terminal (CT). */
#define TMAS_UMS_ROLE               0x04     /**< Unicast Media Sender (UMS). */
#define TMAS_UMR_ROLE               0x08     /**< Unicast Media Receiver (UMR). */
#define TMAS_BMS_ROLE               0x10     /**< Broadcast Media Sender (BMS). */
#define TMAS_BMR_ROLE               0x20     /**< Broadcast Media Receiver (BMR). */
#define TMAS_ROLE_RFU               0xC0     /**< RFU. */
/**
 * End of TMAP_ROLE_BIT_MASK
 * \}
 */

/**
 * End of TMAP_Def_Exported_Macros
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
