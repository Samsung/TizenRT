/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BASS_DEF_H_
#define _BASS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "metadata_def.h"
#include "ble_audio_def.h"

/**
 * \defgroup    LEA_GAF_BASS_Def Broadcast Audio Scan Service Definition
 *
 * \brief   Broadcast Audio Scan Service related definition.
 */

/**
 * \defgroup    BASS_Def_Exported_Macros Broadcast Audio Scan Service Definition Exported Macros
 *
 * \ingroup LEA_GAF_BASS_Def
 * \{
 */

/**
 * bass_def.h
 *
 * \defgroup    BASS_ERR_CODE BASS Error Code
 *
 * \brief  Define Broadcast Audio Scan Service Error Code.
 *
 * \ingroup BASS_Def_Exported_Macros
 * \{
 */
#define ATT_ERR_BASS_OPCODE_NOT_SUPPORT (ATT_ERR | 0x80)     /**< Opcode Not Supported */
#define ATT_ERR_BASS_INVALID_SRC_ID     (ATT_ERR | 0x81)     /**< Invalid Source_ID */
/**
 * End of BASS_ERR_CODE
 * \}
 */

/**
 * bass_def.h
 *
 * \defgroup    BASS_PARAM_DESC BASS Parameter Description
 *
 * \brief  Define BASS Parameter Description.
 *
 * \ingroup BASS_Def_Exported_Macros
 * \{
 */
#define BASS_FAILED_TO_SYNC_TO_BIG 0xFFFFFFFF
#define BASS_CP_BIS_SYNC_NO_PREFER 0xFFFFFFFF
#define BASS_PA_INTERVAL_UNKNOWN   0xFFFF
/**
 * End of BASS_PARAM_DESC
 * \}
 */

/**
 * bass_def.h
 *
 * \defgroup    BASS_DATE_LEN BASS Data Length
 *
 * \brief  Define Broadcast Audio Scan Service Data Length.
 *
 * \ingroup BASS_Def_Exported_Macros
 * \{
 */
#define BASS_BRS_DATA_MIN_LEN 15                 /**< Broadcast Receive State Data Minimum Length */

#define BASS_CP_OP_MIN_LEN                1      /**< Broadcast Audio Scan Control Point Minimum Possible Total Length */
#define BASS_CP_OP_REMOTE_SCAN_STOP_LEN   1      /**< Remote Scan Stopped Length */
#define BASS_CP_OP_REMOTE_SCAN_START_LEN  1      /**< Remote Scan Started Length */
#define BASS_CP_OP_ADD_SOURCE_MIN_LEN     16     /**< Add Source Minimum Length */
#define BASS_CP_OP_MODIFY_SOURCE_MIN_LEN  6      /**< Modify Source Minimum Length */
#define BASS_CP_OP_SET_BROADCAST_CODE_LEN 18     /**< Set Broadcast_Code Minimum Length */
#define BASS_CP_OP_REMOVE_SOURCE_LEN      2      /**< Remove Source Minimum Length */

#define BASS_BRA_INC_BST_CODE_LEN         31
/**
 * End of BASS_DATE_LEN
 * \}
 */

/**
 * bass_def.h
 *
 * \brief  Define Broadcast Receive State Characteristic Maximum Number.
 *
 * \ingroup BASS_Def_Exported_Macros
 */
#define BASS_BRS_CHAR_MAX_NUM                    4

/**
 * bass_def.h
 *
 * \defgroup    BASS_UUID BASS Service UUID
 *
 * \brief  Define Broadcast Audio Scan Service UUID.
 *
 * \ingroup BASS_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_BASS                           0x184F    /**< Broadcast Audio Scan Service */
#define BASS_UUID_CHAR_BROADCAST_AUDIO_SCAN_CP   0x2BC7    /**< Broadcast Audio Scan Control Point */
#define BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE   0x2BC8    /**< Broadcast Receive State */
/**
 * End of BASS_UUID
 * \}
 */

/**
 * End of BASS_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    BASS_Def_Exported_Types Broadcast Audio Scan Service Definition Exported Types
 *
 * \ingroup LEA_GAF_BASS_Def
 * \{
 */

/**
 * bass_def.h
 *
 * \brief  Broadcast Receive State PA_Sync_State Field.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef enum
{
    PA_SYNC_STATE_NOT_SYNC     = 0x00,      /**< Not synchronized to PA */
    PA_SYNC_STATE_SYNC_INFO_REQ = 0x01,     /**< SyncInfo Request */
    PA_SYNC_STATE_SYNC         = 0x02,      /**< Synchronized to PA */
    PA_SYNC_STATE_FAILED       = 0x03,      /**< Failed to synchronize to PA */
    PA_SYNC_STATE_NO_PAST      = 0x04,      /**< No PAST */
} T_PA_SYNC_STATE;

/**
 * bass_def.h
 *
 * \brief  Broadcast Receive State BIG_Encryption Field.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef enum
{
    BIG_NOT_ENCRYPTED           = 0x00,      /**< Not encrypted */
    BIG_BROADCAST_CODE_REQUIRED = 0x01,      /**< Broadcast_Code required */
    BIG_DECRYPTING              = 0x02,      /**< Decrypting */
    BIG_BAD_CODE                = 0x03,      /**< Bad_Code (incorrect encryption key) */
} T_BIG_ENCRYPTION_STATE;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Opcode.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef enum
{
    BASS_CP_OP_REMOTE_SCAN_STOP   = 0x00,
    BASS_CP_OP_REMOTE_SCAN_START  = 0x01,
    BASS_CP_OP_ADD_SOURCE         = 0x02,
    BASS_CP_OP_MODIFY_SOURCE      = 0x03,
    BASS_CP_OP_SET_BROADCAST_CODE = 0x04,
    BASS_CP_OP_REMOVE_SOURCE      = 0x05,
} T_BASS_CP_OP;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point PA_Sync Parameter.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef enum
{
    BASS_PA_NOT_SYNC     = 0x00,      /**< Do not synchronize to PA */
    BASS_PA_SYNC_PAST    = 0x01,      /**< Synchronize to PA: PAST available */
    BASS_PA_SYNC_NO_PAST = 0x02,      /**< Synchronize to PA: PAST not available */
} T_BASS_PA_SYNC;

/**
 * bass_def.h
 *
 * \brief  PAST Service Data.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    uint8_t adv_a_match_ext_adv; /**< AdvA in PAST matches AdvA in ADB_EXT_IND, 0b0=Yes, 0b1=No/Do't know */
    uint8_t adv_a_match_src;     /**< AdvA in PAST matches Source_Address, 0b0=Yes, 0b1=No/Do't know */
    uint8_t source_id;
} T_BASS_PAST_SRV_DATA;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point BIS Information.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    uint32_t bis_sync;
    uint8_t metadata_len;
    uint8_t *p_metadata;
} T_BASS_CP_BIS_INFO;

/**
 * bass_def.h
 *
 * \brief  Broadcast Receive State Data.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    bool brs_is_used;
    /**< Broadcast Receive State Field */
    uint8_t source_id;
    uint8_t source_address_type;
    uint8_t source_address[GAP_BD_ADDR_LEN];
    uint8_t source_adv_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    T_PA_SYNC_STATE pa_sync_state;
    uint32_t bis_sync_state;
    T_BIG_ENCRYPTION_STATE big_encryption;
    uint8_t bad_code[BROADCAST_CODE_LEN];
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    T_BASS_CP_BIS_INFO  *p_cp_bis_info;
} T_BASS_BRS_DATA;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Add Source Operation Data Format.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    uint8_t advertiser_address_type;
    uint8_t advertiser_address[GAP_BD_ADDR_LEN];
    uint8_t advertiser_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    T_BASS_PA_SYNC pa_sync;
    uint16_t pa_interval;
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    T_BASS_CP_BIS_INFO  *p_cp_bis_info;
} T_BASS_CP_ADD_SOURCE;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Modify Source Operation Data Format.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    uint8_t source_id;
    T_BASS_PA_SYNC pa_sync;
    uint16_t pa_interval;
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    T_BASS_CP_BIS_INFO  *p_cp_bis_info;
} T_BASS_CP_MODIFY_SOURCE;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Set Broadcast_Code Operation Data Format.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    uint8_t source_id;
    uint8_t broadcast_code[BROADCAST_CODE_LEN];
} T_BASS_CP_SET_BROADCAST_CODE;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Remove Source Operation Data Format.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    uint8_t source_id;
} T_BASS_CP_REMOVE_SOURCE;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Characteristic Parameter.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef union
{
    T_BASS_CP_ADD_SOURCE         add_source;
    T_BASS_CP_MODIFY_SOURCE      modify_source;
    T_BASS_CP_SET_BROADCAST_CODE set_broadcast_code;
    T_BASS_CP_REMOVE_SOURCE      remove_source;
} T_BASS_CP_PARAM;

/**
 * bass_def.h
 *
 * \brief  Broadcast Audio Scan Control Point Characteristic Data.
 *
 * \ingroup BASS_Def_Exported_Types
 */
typedef struct
{
    T_BASS_CP_OP cp_op;
    T_BASS_CP_PARAM param;
} T_BASS_CP_DATA;
/**
 * End of BASS_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
