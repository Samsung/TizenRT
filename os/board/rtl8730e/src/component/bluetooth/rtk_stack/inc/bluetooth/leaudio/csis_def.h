/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CSIS_DEF_H_
#define _CSIS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/**
 * \defgroup    LEA_GAF_CSIP_Def Coordinated Set Identification Profile Definition
 *
 * \brief   Coordinated Set Identification Profile related definition.
 */

/**
 * \defgroup    CSIP_Def_Exported_Macros Coordinated Set Identification Profile Definition Exported Macros
 *
 * \ingroup LEA_GAF_CSIP_Def
 * \{
 */

/**
 * csis_def.h
 *
 * \defgroup  SET_MEMBER_FEATURE CSIS Service Features
 *
 * \brief  Define Coordinated Set Identification Service Features Exist flags.
 *
 * \ingroup CSIP_Def_Exported_Macros
 * \{
 */
#define SET_MEMBER_LOCK_EXIST          0x01
#define SET_MEMBER_SIZE_EXIST          0x02
#define SET_MEMBER_RANK_EXIST          0x04
#define SET_MEMBER_SIRK_NOTIFY_SUPPORT 0x10
#define SET_MEMBER_SIZE_NOTIFY_SUPPORT 0x20
/**
 * End of SET_MEMBER_FEATURE
 * \}
 */

/**
 * csis_def.h
 *
 * \defgroup  CSIS_ERR_CODE CSIS Service Error Code
 *
 * \brief  Define Coordinated Set Identification Service Error Code.
 *
 * \ingroup CSIP_Def_Exported_Macros
 * \{
 */
#define ATT_ERR_CSIS_LOCK_DENIED              (ATT_ERR | 0x80)
#define ATT_ERR_CSIS_LOCK_RELEASE_NOT_ALLOWED (ATT_ERR | 0x81)
#define ATT_ERR_CSIS_INVALID_LOCK_VALUE       (ATT_ERR | 0x82)
#define ATT_ERR_CSIS_OOB_SIRK_ONLY            (ATT_ERR | 0x83)
#define ATT_ERR_CSIS_LOCK_ALREADY_GRANTED     (ATT_ERR | 0x84)
/**
 * End of CSIS_ERR_CODE
 * \}
 */

/**
 * csis_def.h
 *
 * \defgroup  CSIS_UUID CSIS Service UUID
 *
 * \brief  Define Coordinated Set Identification Service UUID.
 *
 * \ingroup CSIP_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_CSIS                        0x1846

#define CSIS_UUID_CHAR_SIRK                   0x2B84
#define CSIS_UUID_CHAR_SIZE                   0x2B85
#define CSIS_UUID_CHAR_LOCK                   0x2B86
#define CSIS_UUID_CHAR_RANK                   0x2B87
/**
 * End of CSIS_UUID
 * \}
 */

/**
 * csis_def.h
 *
 * \brief  Define Set Identity Resolving Key Length.
 *
 * \ingroup CSIP_Def_Exported_Macros
 */
#define CSIS_SIRK_LEN           16

/**
 * csis_def.h
 *
 * \brief  Define Coordinated Set Identification Service Timeout.
 *
 * \ingroup CSIP_Def_Exported_Macros
 */
#define CSIS_LOCK_DEFAULT_TIMEOUT 60000

/**
 * csis_def.h
 *
 * \defgroup  UNKNOWN_CSIS_VALUE Unknown CSIS Value
 *
 * \brief  Define Unknown Coordinated Set Size and Set Member Rank Value.
 *
 * \ingroup CSIP_Def_Exported_Macros
 * \{
 */
#define CSIS_SIZE_UNKNOWN     0
#define CSIS_RANK_UNKNOWN     0
/**
 * End of UNKNOWN_CSIS_VALUE
 * \}
 */

/**
 * End of CSIP_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    CSIP_Def_Exported_Types Coordinated Set Identification Profile Definition Exported Types
 *
 * \ingroup LEA_GAF_CSIP_Def
 * \{
 */

/**
 * csis_def.h
 *
 * \brief  Define CSIS Set Member Lock State.
 *
 * \ingroup CSIP_Def_Exported_Types
 */
typedef enum
{
    CSIS_NONE_LOCK  = 0,
    CSIS_UNLOCKED   = 0x01,
    CSIS_LOCKED     = 0x02,
} T_CSIS_LOCK;

/**
 * csis_def.h
 *
 * \brief  Define Set Identity Resolving Key Type.
 *
 * \ingroup CSIP_Def_Exported_Types
 */
typedef enum
{
    CSIS_SIRK_ENC = 0x00,
    CSIS_SIRK_PLN = 0x01,
} T_CSIS_SIRK_TYPE;
/**
 * End of CSIP_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
