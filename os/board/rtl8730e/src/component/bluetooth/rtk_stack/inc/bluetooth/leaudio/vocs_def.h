/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _VOCS_DEF_H_
#define _VOCS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>

/**
 * \defgroup    LEA_GAF_VOCS_Def Volume Offset Control Service Definition
 *
 * \brief   Volume Offset Control Service related definition.
 */

/**
 * \defgroup    VOCS_Def_Exported_Macros Volume Offset Control Service Definition Exported Macros
 *
 * \ingroup LEA_GAF_VOCS_Def
 * \{
 */

/**
 * vocs_def.h
 *
 * \defgroup  VOCS_ERR_CODE  VOCS Service Error Code
 *
 * \brief  Define Volume Offset Control Service Error Code.
 *
 * \ingroup VOCS_Def_Exported_Macros
 * \{
 */
#define ATT_ERR_VOCS_INVALID_CHANGE_COUNTER       (ATT_ERR | 0x80)   /**< Invalid Change Counter. 
                                                                        The Change_Counter operand value does not match the Change_Counter
                                                                        field value of the Volume Offset State characteristic. */
#define ATT_ERR_VOCS_OPCODE_NOT_SUPPORT           (ATT_ERR | 0x81)   /**< Opcode Not Supported.
                                                                          An invalid opcode has been used in a control point procedure. */
#define ATT_ERR_VOCS_VALUE_OUT_OF_RANGE           (ATT_ERR | 0x82)   /**< Value Out of Range.
                                                                          An operand value used in a control point procedure
                                                                          is outside the permissible range. */
/**
 * End of VOCS_ERR_CODE
 * \}
 */

/**
 * vocs_def.h
 *
 * \defgroup  VOCS_UUID  VOCS Service UUID
 *
 * \brief  Define Volume Offset Control Service and Characteristic UUID.
 *
 * \ingroup VOCS_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_VOCS                           0x1845

#define VOCS_UUID_CHAR_VOLUME_OFFSET_STATE       0x2B80   /**< Volume Offset State */
#define VOCS_UUID_CHAR_AUDIO_LOCATION            0x2B81   /**< Audio Location */
#define VOCS_UUID_CHAR_VOLUME_OFFSET_CP          0x2B82   /**< Volume Offset Control Point */
#define VOCS_UUID_CHAR_AUDIO_OUTPUT_DES          0x2B83   /**< Audio Output Description */
/**
 * End of VOCS_UUID
 * \}
 */

/**
 * vocs_client.h
 *
 * \defgroup  VOCS_CCCD_FLAGS  VOCS CCCD Flags
 *
 * \brief  Define Volume Offset Control Service CCCD Flags.
 *
 * \ingroup VOCS_Def_Exported_Macros
 * \{
 */
#define VOCS_VOLUME_OFFSET_STATE_FLAG       0x01
#define VOCS_AUDIO_LOCATION_FLAG            0x02
#define VOCS_AUDIO_OUTPUT_DES_FLAG          0x04
/**
 * End of VOCS_CCCD_FLAGS
 * \}
 */

/**
 * End of VOCS_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    VOCS_Def_Exported_Types Volume Offset Control Service Definition Exported Types
 *
 * \ingroup LEA_GAF_VOCS_Def
 * \{
 */

/**
 * vocs_def.h
 *
 * \brief  Volume Offset Control Point opcode.
 *
 * \ingroup VOCS_Def_Exported_Types
 */
typedef enum
{
    VOCS_CP_SET_VOLUME_OFFSET = 0x01,   /**< Set Volume Offset */
    VOCS_CP_MAX
} T_VOCS_CP_OP;

/**
 * vocs_def.h
 *
 * \brief  Volume Offset State value.
 *
 * \ingroup VOCS_Def_Exported_Types
 */
typedef struct
{
    int16_t volume_offset;
    uint8_t change_counter;
} T_VOCS_VOLUME_OFFSET_STATE;

/**
 * vocs_def.h
 *
 * \brief  Audio Output Description value.
 *
 * \ingroup VOCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t    *p_output_des;
    uint16_t    output_des_len;
} T_VOCS_OUTPUT_DES;
/**
 * End of VOCS_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
