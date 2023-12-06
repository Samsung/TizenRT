/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _VCS_DEF_H_
#define _VCS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>

/**
 * \defgroup    LEA_GAF_VCP_Def Volume Control Profile Definition
 *
 * \brief   Volume Control Profile related definition.
 */

/**
 * \defgroup    VCP_Def_Exported_Macros Volume Control Profile Definition Exported Macros
 *
 * \ingroup LEA_GAF_VCP_Def
 * \{
 */

/**
 * vcs_def.h
 *
 * \defgroup    VCS_ERR_CODE VCS Service Error Code
 *
 * \brief  Define Volume Control Service Error Code.
 *
 * \ingroup VCP_Def_Exported_Macros
 * \{
 */
#define ATT_ERR_VCS_INVALID_CHANGE_COUNTER       (ATT_ERR | 0x80)
#define ATT_ERR_VCS_OPCODE_NOT_SUPPORT           (ATT_ERR | 0x81)
/**
 * End of VCS_ERR_CODE
 * \}
 */

/**
 * vcs_def.h
 *
 * \defgroup    VCS_UUID VCS Service UUID
 *
 * \brief  Define Volume Control Service UUID.
 *
 * \ingroup VCP_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_VCS                            0x1844

#define VCS_UUID_CHAR_VOLUME_STATE               0x2B7D
#define VCS_UUID_CHAR_VOLUME_CP                  0x2B7E
#define VCS_UUID_CHAR_VOLUME_FLAGS               0x2B7F
/**
 * End of VCS_UUID
 * \}
 */

/**
 * vcs_def.h
 *
 * \defgroup    VOL_SET_RANGE Volume Setting Range
 *
 * \brief  Define Volume Setting Range.
 *
 * \ingroup VCP_Def_Exported_Macros
 * \{
 */
#define VCS_VOLUME_SETTING_MIN                   0       /**< minimum Volume Setting */
#define VCS_VOLUME_SETTING_MAX                   255     /**< maximum Volume Setting */
/**
 * End of VOL_SET_RANGE
 * \}
 */

/**
 * vcs_def.h
 *
 * \defgroup    VOL_MUTE_VALUE Volume Mute Field Value
 *
 * \brief  Define Volume Mute Field Value.
 *
 * \ingroup VCP_Def_Exported_Macros
 * \{
 */
#define VCS_MUTED                                1
#define VCS_NOT_MUTED                            0
/**
 * End of VOL_MUTE_VALUE
 * \}
 */

/**
 * vcs_def.h
 *
 * \defgroup    VOL_FLAG Volume Flags
 *
 * \brief  Define Volume Flags Bit Mask and Value.
 *
 * \ingroup VCP_Def_Exported_Macros
 * \{
 */
#define VCS_VOLUME_SETTING_PERSISTED_FLAG        0x01     /**< Volume_Setting_Persisted */

#define VCS_RESET_VOLUME_SETTING                 0x00     /**< Reset Volume Setting */
#define VCS_USER_SET_VOLUME_SETTING              0x01     /**< User Set Volume Setting */
/**
 * End of VOL_FLAG
 * \}
 */

/**
 * End of VCP_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    VCP_Def_Exported_Types Volume Control Profile Definition Exported Types
 *
 * \ingroup LEA_GAF_VCP_Def
 * \{
 */

/**
 * vcs_def.h
 *
 * \brief  Volume Control Point Opcodes.
 *
 * \ingroup VCP_Def_Exported_Types
 */
typedef enum
{
    VCS_CP_RELATIVE_VOLUME_DOWN        = 0x00,     /**< Relative Volume Down */
    VCS_CP_RELATIVE_VOLUME_UP          = 0x01,     /**< Relative Volume Up */
    VCS_CP_UNMUTE_RELATIVE_VOLUME_DOWN = 0x02,     /**< Unmute/Relative Volume Down */
    VCS_CP_UNMUTE_RELATIVE_VOLUME_UP   = 0x03,     /**< Unmute/Relative Volume Up */
    VCS_CP_SET_ABSOLUTE_VOLUME         = 0x04,     /**< Set Absolute Volume */
    VCS_CP_UNMUTE                      = 0x05,     /**< Unmute */
    VCS_CP_MUTE                        = 0x06,     /**< Mute */
    VCS_CP_MAX
} T_VCS_CP_OP;

/**
 * vcs_def.h
 *
 * \brief  Volume State Data.
 *
 * \ingroup VCP_Def_Exported_Types
 */
typedef struct
{
    uint8_t volume_setting;
    uint8_t mute;
    uint8_t change_counter;
} T_VOLUME_STATE;
/**
 * End of VCP_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
