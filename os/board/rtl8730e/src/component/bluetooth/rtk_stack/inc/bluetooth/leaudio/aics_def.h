/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _AICS_DEF_H_
#define _AICS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>

/**
 * \defgroup    LEA_GAF_AICS_Def Audio Input Control Service Definition
 *
 * \brief   Audio Input Control Service related definition.
 */

/**
 * \defgroup    AICS_Def_Exported_Macros Audio Input Control Definition Exported Macros
 *
 * \ingroup LEA_GAF_AICS_Def
 * \{
 */

/**
 * aics_def.h
 *
 * \defgroup AICS_ERR_CODE AICS service error code
 *
 * \brief  Define AICS service ATT error code.
 *
 * \ingroup AICS_Def_Exported_Macros
 * /{
 */
#define ATT_ERR_AICS_INVALID_CHANGE_COUNTER       (ATT_ERR | 0x80)  /**< The Change_Counter operand value does not match the Change_Counter
                                                                         field value of the Audio Input State characteristic. */
#define ATT_ERR_AICS_OPCODE_NOT_SUPPORT           (ATT_ERR | 0x81)  /**< An invalid opcode has been used in a control point procedure. */
#define ATT_ERR_AICS_MUTE_DISABLED                (ATT_ERR | 0x82)  /**< Mute/unmute commands are disabled. */
#define ATT_ERR_AICS_VALUE_OUT_OF_RANGE           (ATT_ERR | 0x83)  /**< An operand value used in a control point procedure is outside
                                                                         the permissible range. */
#define ATT_ERR_AICS_GAIN_MODE_CHANGE_NOT_ALLOWED (ATT_ERR | 0x84)  /**< A requested gain mode change is not allowed. */
/**
 * End of AICS_ERR_CODE
 * \}
 */

/**
 * aics_def.h
 *
 * \defgroup AICS_UUID AICS Audio Input Control Service UUID
 *
 * \brief  Define Audio Input Control Service UUID.
 *
 * \ingroup AICS_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_AICS                           0x1843       /**< Audio Input Control Service */

#define AICS_UUID_CHAR_INPUT_STATE               0x2B77       /**< Audio Input State */
#define AICS_UUID_CHAR_GAIN_SETTING_PROP         0x2B78       /**< Gain Setting Properties */
#define AICS_UUID_CHAR_INPUT_TYPE                0x2B79       /**< Audio Input Type */
#define AICS_UUID_CHAR_INPUT_STATUS              0x2B7A       /**< Audio Input Status */
#define AICS_UUID_CHAR_INPUT_CP                  0x2B7B       /**< Audio Input Control Point */
#define AICS_UUID_CHAR_INPUT_DES                 0x2B7C       /**< Audio Input Description */
/**
 * End of AICS_UUID
 * \}
 */

/**
 * aics_def.h
 *
 * \defgroup AICS_MUTE_STATE AICS Mute State
 *
 * \brief  Define AICS Mute State of Audio.
 *
 * \ingroup AICS_Def_Exported_Macros
 * \{
 */
#define AICS_NOT_MUTED                           0
#define AICS_MUTED                               1
#define AICS_MUTE_DISABLED                       2
/**
 * End of AICS_MUTE_STATE
 * \}
 */

/**
 * aics_def.h
 *
 * \defgroup AICS_GAIN_MODE AICS Gain Modes
 *
 * \brief  Define AICS Gain Modes.
 *
 * \ingroup AICS_Def_Exported_Macros
 * \{
 */
#define AICS_GAIN_MODE_MANUAL_ONLY               0
#define AICS_GAIN_MODE_AUTOMATIC_ONLY            1
#define AICS_GAIN_MODE_MANUAL                    2
#define AICS_GAIN_MODE_AUTOMATIC                 3
/**
 * End of AICS_GAIN_MODE
 * \}
 */

/**
 * aics_def.h
 *
 * \defgroup AICS_INPUT_STATUS AICS Audio Input Status Type
 *
 * \brief  Define AICS Audio Input Status Type.
 *
 * \ingroup AICS_Def_Exported_Macros
 * \{
 */
#define AICS_INPUT_STATUS_INACTIVE               0
#define AICS_INPUT_STATUS_ACTIVE                 1
/**
 * End of AICS_INPUT_STATUS
 * \}
 */

/**
 * End of AICS_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    AICS_Def_Exported_Types Audio Input Control Definition Exported Types
 *
 * \ingroup LEA_GAF_AICS_Def
 * \{
 */

/**
 * aics_def.h
 *
 * \brief  AICS Audio Input Control Point Opcodes.
 *
 * \ingroup AICS_Def_Exported_Types
 */
typedef enum
{
    AICS_CP_SET_GAIN_SETTING        = 0x01,
    AICS_CP_UNMUTE                  = 0x02,
    AICS_CP_MUTE                    = 0x03,
    AICS_CP_SET_MANUAL_GAIN_MODE    = 0x04,
    AICS_CP_SET_AUTOMATIC_GAIN_MODE = 0x05,
    AICS_CP_MAX
} T_AICS_CP_OP;

/**
 * aics_def.h
 *
 * \brief  AICS Service Audio Input State Characteristic Value.
 *
 * \ingroup AICS_Def_Exported_Types
 */
typedef struct
{
    int8_t  gain_setting;
    uint8_t mute;
    uint8_t gain_mode;
    uint8_t change_counter;
} T_AICS_INPUT_STATE;

/**
 * aics_def.h
 *
 * \brief  AICS Service Gain Setting Properties Characteristic Value.
 *
 * \ingroup AICS_Def_Exported_Types
 */
typedef struct
{
    uint8_t gain_setting_units;
    int8_t  gain_setting_min;
    int8_t  gain_setting_max;
} T_AICS_GAIN_SETTING_PROP;

/**
 * aics_def.h
 *
 * \brief  AICS Service Audio Input Description Characteristic Value.
 *
 * \ingroup AICS_Def_Exported_Types
 */
typedef struct
{
    uint16_t input_des_len;
    uint8_t  *p_input_des;
} T_AICS_INPUT_DES;
/**
 * End of AICS_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
