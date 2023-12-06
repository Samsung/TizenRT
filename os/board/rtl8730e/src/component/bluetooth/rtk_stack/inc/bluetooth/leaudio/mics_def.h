/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _MICS_DEF_H_
#define _MICS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/**
 * \defgroup    LEA_GAF_MICP_Def Microphone Control Profile Definition
 *
 * \brief   Microphone Control Profile related definition.
 */

/**
 * \defgroup    MICP_Def_Exported_Macros Microphone Control Profile Definition Exported Macros
 *
 * \ingroup LEA_GAF_MICP_Def
 * \{
 */

/**
 * mics_def.h
 *
 * \brief  Define MICS Service Error Code.
 *
 * \ingroup MICP_Def_Exported_Macros
 */
#define ATT_ERR_MICS_MUTE_DISABLED_ERROR          (ATT_ERR | 0x80)

/**
 * mics_def.h
 *
 * \defgroup    MICS_UUID MICS Service UUID
 *
 * \brief  Define MICS Service UUID and Characteristic UUID.
 *
 * \ingroup MICP_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_MICS                            0x184D

#define MICS_UUID_CHAR_MUTE                       0x2BC3
/**
 * End of MICS_UUID
 * \}
 */

/**
 * End of MICP_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    MICP_Def_Exported_Types Microphone Control Profile Definition Exported Types
 *
 * \ingroup LEA_GAF_MICP_Def
 * \{
 */

/**
 * mics_def.h
 *
 * \brief  MICS Service mute value.
 *
 * \ingroup MICP_Def_Exported_Types
 */
typedef enum
{
    MICS_NOT_MUTE   = 0,
    MICS_MUTED      = 1,
    MICS_DISABLED   = 2,
    MICS_RFU,
} T_MICS_MUTE;
/**
 * End of MICP_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
