/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BLE_AUDIO_H_
#define _BLE_AUDIO_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap_msg.h"
#include "gap.h"
#include "app_msg.h"


/**
 * \defgroup    LEA_LIB LE Audio Lib
 *
 * \brief   General interface accessing to LE Audio Lib.
 */

/**
 * \defgroup    LEA_LIB_Exported_Macros BLE Audio Lib Exported Macros
 *
 * \ingroup LEA_LIB
 * \{
 */

/**
 * ble_audio.h
 *
 * \defgroup BLE_AUDIO_CB_RESULT BLE audio callback result
 *
 * \brief  Define BLE audio callback result.
 *
 * \ingroup LEA_LIB_Exported_Macros
 * \{
 */
#define BLE_AUDIO_CB_RESULT_SUCCESS 0x0000
#define BLE_AUDIO_CB_RESULT_REJECT  (APP_ERR | APP_ERR_REJECT)
#define BLE_AUDIO_CB_RESULT_PENDING (APP_ERR | APP_ERR_PENDING)
#define BLE_AUDIO_CB_RESULT_APP_ERR (ATT_ERR | ATT_ERR_MIN_APPLIC_CODE)
#define BLE_AUDIO_CB_RESULT_WRITE_REQUEST_REJECTED (ATT_ERR | ATT_ERR_WRITE_REQUEST_REJECTED)
/**
 * End of BLE_AUDIO_CB_RESULT
 * \}
 */

/**
 * End of LEA_LIB_Exported_Macros
 * \}
 */

/**
 * \defgroup    LEA_LIB_Exported_Types BLE Audio Lib Exported Types
 *
 * \ingroup LEA_LIB
 * \{
 */

/**
 * ble_audio.h
 *
 * \brief  LE Audio Message Group Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef enum
{
    LE_AUDIO_MSG_GROUP_BASS         = 0x0000,
    LE_AUDIO_MSG_GROUP_BASS_CLIENT  = 0x0100,
    LE_AUDIO_MSG_GROUP_PACS         = 0x0200,
    LE_AUDIO_MSG_GROUP_PACS_CLIENT  = 0x0300,
    LE_AUDIO_MSG_GROUP_CSIS         = 0x0400,
    LE_AUDIO_MSG_GROUP_CSIS_CLIENT  = 0x0500,
    LE_AUDIO_MSG_GROUP_ASCS         = 0x0600,
    LE_AUDIO_MSG_GROUP_ASCS_CLIENT  = 0x0700,
    LE_AUDIO_MSG_GROUP_VOCS         = 0x0800,
    LE_AUDIO_MSG_GROUP_VOCS_CLIENT  = 0x0900,
    LE_AUDIO_MSG_GROUP_VCS          = 0x0A00,
    LE_AUDIO_MSG_GROUP_VCS_CLIENT   = 0x0B00,
    LE_AUDIO_MSG_GROUP_AICS         = 0x0C00,
    LE_AUDIO_MSG_GROUP_AICS_CLIENT  = 0x0D00,
    LE_AUDIO_MSG_GROUP_MICS         = 0x0E00,
    LE_AUDIO_MSG_GROUP_MICS_CLIENT  = 0x0F00,
    LE_AUDIO_MSG_GROUP_CCP_SERVER   = 0x1000,
    LE_AUDIO_MSG_GROUP_CCP_CLIENT   = 0x1100,
    LE_AUDIO_MSG_GROUP_MCP_SERVER   = 0x1200,
    LE_AUDIO_MSG_GROUP_MCP_CLIENT   = 0x1300,
    LE_AUDIO_MSG_GROUP_OTP_SERVER   = 0x1400,
    LE_AUDIO_MSG_GROUP_OTP_CLIENT   = 0x1500,
    LE_AUDIO_MSG_GROUP_TMAS         = 0x1600,
    LE_AUDIO_MSG_GROUP_TMAS_CLIENT  = 0x1700,
    LE_AUDIO_MSG_GROUP_HAS          = 0x1800,
    LE_AUDIO_MSG_GROUP_HAS_CLIENT   = 0x1900,

    LE_AUDIO_MSG_GROUP_CAP          = 0x2000,
    LE_AUDIO_MSG_GROUP_BAP          = 0x2100,

    LE_AUDIO_MSG_GROUP_SERVER       = 0x3000,
    LE_AUDIO_MSG_GROUP_CLIENT       = 0x3100,
} T_LE_AUDIO_MSG_GROUP;

/**
 * ble_audio.h
 *
 * \brief  LE Audio Message Type.
 *         Each service and client has a different ble audio message group @ref T_LE_AUDIO_MSG_GROUP.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef enum
{
    /**< bass_mgr.h */
    LE_AUDIO_MSG_BASS_CP_IND                          = LE_AUDIO_MSG_GROUP_BASS | 0x00,
    LE_AUDIO_MSG_BASS_BRS_MODIFY                      = LE_AUDIO_MSG_GROUP_BASS | 0x01,
    LE_AUDIO_MSG_BASS_BA_ADD_SOURCE                   = LE_AUDIO_MSG_GROUP_BASS | 0x02,
    LE_AUDIO_MSG_BASS_LOCAL_ADD_SOURCE                = LE_AUDIO_MSG_GROUP_BASS | 0x03,
    LE_AUDIO_MSG_BASS_GET_PA_SYNC_PARAM               = LE_AUDIO_MSG_GROUP_BASS | 0x04,
    LE_AUDIO_MSG_BASS_GET_BROADCAST_CODE              = LE_AUDIO_MSG_GROUP_BASS | 0x05,
    LE_AUDIO_MSG_BASS_GET_BIG_SYNC_PARAM              = LE_AUDIO_MSG_GROUP_BASS | 0x06,
    LE_AUDIO_MSG_BASS_GET_PREFER_BIS_SYNC             = LE_AUDIO_MSG_GROUP_BASS | 0x07,
    LE_AUDIO_MSG_BASS_BRS_CHAR_NO_EMPTY               = LE_AUDIO_MSG_GROUP_BASS | 0x08,

    /**< bass_client.h */
    LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT                = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x00,
    LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA                 = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x01,
    LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ            = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x02,
    /**< pacs_mgr.h */
    LE_AUDIO_MSG_PACS_WRITE_SINK_AUDIO_LOC_IND        = LE_AUDIO_MSG_GROUP_PACS | 0x00,
    LE_AUDIO_MSG_PACS_WRITE_SOURCE_AUDIO_LOC_IND      = LE_AUDIO_MSG_GROUP_PACS | 0x01,
    LE_AUDIO_MSG_PACS_READ_AVAILABLE_CONTEXTS_IND     = LE_AUDIO_MSG_GROUP_PACS | 0x02,
    /**< pacs_client.h */
    LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE                 = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x00,
    LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x01,
    LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x02,

    /**< ascs_mgr.h */
    LE_AUDIO_MSG_ASCS_ASE_STATE                       = LE_AUDIO_MSG_GROUP_ASCS | 0x00,
    LE_AUDIO_MSG_ASCS_CP_CONFIG_CODEC_IND             = LE_AUDIO_MSG_GROUP_ASCS | 0x01,
    LE_AUDIO_MSG_ASCS_CP_CONFIG_QOS_IND               = LE_AUDIO_MSG_GROUP_ASCS | 0x02,
    LE_AUDIO_MSG_ASCS_CP_ENABLE_IND                   = LE_AUDIO_MSG_GROUP_ASCS | 0x03,
    LE_AUDIO_MSG_ASCS_CP_DISABLE_IND                  = LE_AUDIO_MSG_GROUP_ASCS | 0x04,
    LE_AUDIO_MSG_ASCS_CP_UPDATE_METADATA_IND          = LE_AUDIO_MSG_GROUP_ASCS | 0x05,
    LE_AUDIO_MSG_ASCS_SETUP_DATA_PATH                 = LE_AUDIO_MSG_GROUP_ASCS | 0x06,
    LE_AUDIO_MSG_ASCS_REMOVE_DATA_PATH                = LE_AUDIO_MSG_GROUP_ASCS | 0x07,
    LE_AUDIO_MSG_ASCS_GET_PREFER_QOS                  = LE_AUDIO_MSG_GROUP_ASCS | 0x08,
    LE_AUDIO_MSG_ASCS_CIS_REQUEST_IND                 = LE_AUDIO_MSG_GROUP_ASCS | 0x09,
    LE_AUDIO_MSG_ASCS_AUDIO_CONTEXTS_CHECK_IND        = LE_AUDIO_MSG_GROUP_ASCS | 0x0a,
    LE_AUDIO_MSG_ASCS_CIS_DISCONN_INFO                = LE_AUDIO_MSG_GROUP_ASCS | 0x0b,

    /**< vcs_mgr.h */
    LE_AUDIO_MSG_VCS_VOLUME_CP_IND                    = LE_AUDIO_MSG_GROUP_VCS | 0x00,
    /**< vcs_client.h */
    LE_AUDIO_MSG_VCS_CLIENT_DIS_DONE                  = LE_AUDIO_MSG_GROUP_VCS_CLIENT | 0x00,
    LE_AUDIO_MSG_VCS_CLIENT_CP_RESULT                 = LE_AUDIO_MSG_GROUP_VCS_CLIENT | 0x01,
    LE_AUDIO_MSG_VCS_CLIENT_VOLUME_STATE_DATA         = LE_AUDIO_MSG_GROUP_VCS_CLIENT | 0x02,
    /**< vocs_mgr.h */
    LE_AUDIO_MSG_VOCS_WRITE_OFFSET_STATE_IND          = LE_AUDIO_MSG_GROUP_VOCS | 0x00,
    LE_AUDIO_MSG_VOCS_WRITE_AUDIO_LOCATION_IND        = LE_AUDIO_MSG_GROUP_VOCS | 0x01,
    LE_AUDIO_MSG_VOCS_WRITE_OUTPUT_DES_IND            = LE_AUDIO_MSG_GROUP_VOCS | 0x02,
    /**< vocs_client.h */
    LE_AUDIO_MSG_VOCS_CLIENT_DIS_DONE                 = LE_AUDIO_MSG_GROUP_VOCS_CLIENT | 0x00,
    LE_AUDIO_MSG_VOCS_CLIENT_READ_RESULT              = LE_AUDIO_MSG_GROUP_VOCS_CLIENT | 0x01,
    LE_AUDIO_MSG_VOCS_CLIENT_CP_RESULT                = LE_AUDIO_MSG_GROUP_VOCS_CLIENT | 0x02,
    LE_AUDIO_MSG_VOCS_CLIENT_NOTIFY                   = LE_AUDIO_MSG_GROUP_VOCS_CLIENT | 0x03,
    /**< csis_mgr.h */
    LE_AUDIO_MSG_CSIS_READ_SIRK_IND                   = LE_AUDIO_MSG_GROUP_CSIS | 0x00,
    LE_AUDIO_MSG_CSIS_LOCK_STATE                      = LE_AUDIO_MSG_GROUP_CSIS | 0x01,
    /**< csis_client.h */
    LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE                 = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x00,
    LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_TIMEOUT           = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x01,
    LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_DONE              = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x02,
    LE_AUDIO_MSG_CSIS_CLIENT_SET_MEM_FOUND            = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x03,
    LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT              = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x04,
    LE_AUDIO_MSG_CSIS_CLIENT_SIRK_NOTIFY              = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x05,
    LE_AUDIO_MSG_CSIS_CLIENT_LOCK_STATE               = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x06,
    LE_AUDIO_MSG_CSIS_CLIENT_SIZE_NOTIFY              = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x07,
    /**< aics_mgr.h */
    LE_AUDIO_MSG_AICS_CP_IND                          = LE_AUDIO_MSG_GROUP_AICS | 0x00,
    LE_AUDIO_MSG_AICS_WRITE_INPUT_DES_IND             = LE_AUDIO_MSG_GROUP_AICS | 0x01,
    /**< aics_client.h */
    LE_AUDIO_MSG_AICS_CLIENT_DIS_DONE                 = LE_AUDIO_MSG_GROUP_AICS_CLIENT | 0x00,
    LE_AUDIO_MSG_AICS_CLIENT_READ_RESULT              = LE_AUDIO_MSG_GROUP_AICS_CLIENT | 0x01,
    LE_AUDIO_MSG_AICS_CLIENT_CP_RESULT                = LE_AUDIO_MSG_GROUP_AICS_CLIENT | 0x02,
    LE_AUDIO_MSG_AICS_CLIENT_NOTIFY                   = LE_AUDIO_MSG_GROUP_AICS_CLIENT | 0x03,
    /**< mics_mgr.h */
    LE_AUDIO_MSG_MICS_WRITE_MUTE_IND                  = LE_AUDIO_MSG_GROUP_MICS | 0x00,
    /**< mics_client.h */
    LE_AUDIO_MSG_MICS_CLIENT_DIS_DONE                 = LE_AUDIO_MSG_GROUP_MICS_CLIENT | 0x00,
    LE_AUDIO_MSG_MICS_CLIENT_WRITE_RESULT             = LE_AUDIO_MSG_GROUP_MICS_CLIENT | 0x01,
    LE_AUDIO_MSG_MICS_CLIENT_NOTIFY                   = LE_AUDIO_MSG_GROUP_MICS_CLIENT | 0x02,

    /**< ccp_mgr.h */
    LE_AUDIO_MSG_CCP_SERVER_WRITE_CALL_CP_IND                     = LE_AUDIO_MSG_GROUP_CCP_SERVER | 0x00,
    LE_AUDIO_MSG_CCP_SERVER_READ_IND                              = LE_AUDIO_MSG_GROUP_CCP_SERVER | 0x01,

    /**< ccp_client.h */
    LE_AUDIO_MSG_CCP_CLIENT_DIS_DONE                              = LE_AUDIO_MSG_GROUP_CCP_CLIENT | 0x00,
    LE_AUDIO_MSG_CCP_CLIENT_READ_RESULT                           = LE_AUDIO_MSG_GROUP_CCP_CLIENT | 0x01,
    LE_AUDIO_MSG_CCP_CLIENT_NOTIFY                                = LE_AUDIO_MSG_GROUP_CCP_CLIENT | 0x02,
    LE_AUDIO_MSG_CCP_CLIENT_CALL_CP_NOTIFY                        = LE_AUDIO_MSG_GROUP_CCP_CLIENT | 0x03,

    /**< mcp_mgr.h */
    LE_AUDIO_MSG_MCP_SERVER_WRITE_MEDIA_CP_IND                    = LE_AUDIO_MSG_GROUP_MCP_SERVER | 0x00,
    LE_AUDIO_MSG_MCP_SERVER_READ_IND                              = LE_AUDIO_MSG_GROUP_MCP_SERVER | 0x01,

    /**< mcp_client.h */
    LE_AUDIO_MSG_MCP_CLIENT_DIS_DONE                              = LE_AUDIO_MSG_GROUP_MCP_CLIENT | 0x00,
    LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT                           = LE_AUDIO_MSG_GROUP_MCP_CLIENT | 0x01,
    LE_AUDIO_MSG_MCP_CLIENT_NOTIFY                                = LE_AUDIO_MSG_GROUP_MCP_CLIENT | 0x02,
    LE_AUDIO_MSG_MCP_CLIENT_MEDIA_CP_NOTIFY                       = LE_AUDIO_MSG_GROUP_MCP_CLIENT | 0x03,

    /**< tmas_client.h */
    LE_AUDIO_MSG_TMAS_CLIENT_DIS_DONE                 = LE_AUDIO_MSG_GROUP_TMAS_CLIENT | 0x00,
    LE_AUDIO_MSG_TMAS_CLIENT_READ_ROLE_RESULT         = LE_AUDIO_MSG_GROUP_TMAS_CLIENT | 0x01,

    /**< has_mgr.h */
    LE_AUDIO_MSG_HAS_CP_IND                           = LE_AUDIO_MSG_GROUP_HAS | 0x00,
    LE_AUDIO_MSG_HAS_PENDING_PRESET_CHANGE            = LE_AUDIO_MSG_GROUP_HAS | 0x01,
    /**< has_client.h */
    LE_AUDIO_MSG_HAS_CLIENT_DIS_DONE                  = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x00,
    LE_AUDIO_MSG_HAS_CLIENT_READ_HA_FEATURES_RESULT   = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x01,
    LE_AUDIO_MSG_HAS_CLIENT_READ_ACTIVE_PRESET_IDX_RESULT = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x02,
    LE_AUDIO_MSG_HAS_CLIENT_CP_RESULT                 = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x03,
    LE_AUDIO_MSG_HAS_CLIENT_HA_FEATURES_NOTIFY        = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x04,
    LE_AUDIO_MSG_HAS_CLIENT_ACTIVE_PRESET_IDX_NOTIFY  = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x05,
    LE_AUDIO_MSG_HAS_CLIENT_CP_NOTIFY_IND_DATA        = LE_AUDIO_MSG_GROUP_HAS_CLIENT | 0x06,

    /**< bap.h */
    LE_AUDIO_MSG_BAP_DIS_ALL_DONE                     = LE_AUDIO_MSG_GROUP_BAP | 0x00,
    LE_AUDIO_MSG_BAP_PACS_NOTIFY                      = LE_AUDIO_MSG_GROUP_BAP | 0x01,

    /**< cap.h */
    LE_AUDIO_MSG_CAP_DIS_DONE                         = LE_AUDIO_MSG_GROUP_CAP | 0x00,

    LE_AUDIO_MSG_SERVER_ATTR_CCCD_INFO                = LE_AUDIO_MSG_GROUP_SERVER | 0x00,  /**< MCS_UUID_CHAR_MEDIA_PLAYER_NAME,
                                                                                                MCS_UUID_CHAR_TRACK_CHANGED,
                                                                                                MCS_UUID_CHAR_TRACK_TITLE,
                                                                                                MCS_UUID_CHAR_TRACK_DURATION,
                                                                                                MCS_UUID_CHAR_TRACK_POSITION,
                                                                                                MCS_UUID_CHAR_MEDIA_STATE,
                                                                                                MCS_UUID_CHAR_MEDIA_CONTROL_POINT,
                                                                                                MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED,
                                                                                                TBS_UUID_CHAR_BEARER_PROVIDER_NAME,
                                                                                                TBS_UUID_CHAR_BEARER_TECHNOLOGY,
                                                                                                TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH,
                                                                                                TBS_UUID_CHAR_BEARER_LIST_CURRENT_CALLS,
                                                                                                TBS_UUID_CHAR_STATUS_FLAGS,
                                                                                                TBS_UUID_CHAR_INCOMING_CALL_TARGET_BEARER_URI,
                                                                                                TBS_UUID_CHAR_CALL_STATE,
                                                                                                TBS_UUID_CHAR_CALL_CONTROL_POINT,
                                                                                                TBS_UUID_CHAR_TERMINATION_REASON,
                                                                                                TBS_UUID_CHAR_INCOMING_CALL,
                                                                                                TBS_UUID_CHAR_CALL_FRIENDLY_NAME,
                                                                                                ASCS_UUID_CHAR_SNK_ASE,
                                                                                                ASCS_UUID_CHAR_SRC_ASE,
                                                                                                ASCS_UUID_CHAR_ASE_CONTROL_POINT,
                                                                                                BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE,
                                                                                                PACS_UUID_CHAR_SINK_PAC,
                                                                                                PACS_UUID_CHAR_SINK_AUDIO_LOCATIONS,
                                                                                                PACS_UUID_CHAR_SOURCE_PAC,
                                                                                                PACS_UUID_CHAR_SOURCE_AUDIO_LOCATIONS,
                                                                                                PACS_UUID_CHAR_AUDIO_AVAILABILITY_CONTEXTS,
                                                                                                PACS_UUID_CHAR_SUPPORTED_AUDIO_CONTEXTS */
    LE_AUDIO_MSG_CLIENT_ATTR_CCCD_INFO                = LE_AUDIO_MSG_GROUP_CLIENT | 0x00,
} T_LE_AUDIO_MSG;

/**
 * ble_audio.h
 *
 * \brief  LE Audio Message Error Cause Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef enum
{
    LE_AUDIO_CAUSE_SUCCESS                 = 0x00,
    LE_AUDIO_CAUSE_INVALID_PARAM           = 0x01,
    LE_AUDIO_CAUSE_REQ_FAILED              = 0x02,
    LE_AUDIO_CAUSE_NO_CONN                 = 0x03,

    /**< LE Audio Group Error Cause */
    LE_AUDIO_CAUSE_INVALID_GROUP           = 0x10,
    LE_AUDIO_CAUSE_INVALID_CSIS_LOCK_STATE = 0x11,
    LE_AUDIO_CAUSE_INVALID_CSIS_UNKNOWN    = 0x12,
} T_LE_AUDIO_CAUSE;

/**
 * ble_audio.h
 *
 * \brief  P_FUN_BLE_AUDIO_CB BLE Audio Callback Function Point Definition.
 *         Function pointer used in ble audio module, to send events @ref T_LE_AUDIO_MSG
 *         to application.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef uint16_t(*P_FUN_BLE_AUDIO_CB)(T_LE_AUDIO_MSG msg, void *buf);

/**
 * ble_audio.h
 *
 * \brief  LE Audio parameters.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef struct
{
    void *evt_queue_handle;
    void *io_queue_handle;
    uint16_t io_event_type;
    uint16_t bt_gatt_client_init;
    uint8_t acl_link_num;
} T_BLE_AUDIO_PARAMS;

/**
 * ble_audio.h
 *
 * \brief  LE Audio Volume Control and Microphone Control parameter.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef struct
{
    uint8_t vocs_num;             /**< The number of vocs. */
    uint8_t aics_vcs_num;         /**< The number of aics included in vcs. */
    uint8_t aics_mics_num;        /**< The number of aics included in mics. */
    uint8_t aics_total_num;       /**< The total number of aics. */
    bool    vcs_enable;           /**< Whether to register vcs. True: register. False: Not register. */
    bool    mics_enable;          /**< Whether to register mics. True: register. False: Not register. */
    uint8_t *p_vocs_feature_tbl;  /**< VOCS feature. @ref VOCS_FEATURE_MASK . */
    uint8_t *p_aics_vcs_tbl;      /**< The srv_instance_id of aics included in vcs. The table size is aics_vcs_num. */
    uint8_t *p_aics_mics_tbl;     /**< The srv_instance_id of aics included in mics. The table size is aics_mics_num. */
} T_BLE_AUDIO_VC_MIC_PARAMS;

/**
 * ble_audio.h
 *
 * \brief  LE Bond Modify State Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef enum
{
    BT_BOND_DELETE,
    BT_BOND_ADD,
    BT_BOND_CLEAR,
} T_BT_BOND_MODIFY_TYPE;

/**
 * ble_audio.h
 *
 * \brief  LE Bond Modify Message Data.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef struct
{
    T_BT_BOND_MODIFY_TYPE type;
    uint8_t remote_bd_type;
    uint8_t remote_addr[6];
} T_BT_BOND_MODIFY;

/**
 * ble_audio.h
 *
 * \brief  LE Audio Server Attribute CCCD Data Information.
 *         The message data for LE_AUDIO_MSG_SERVER_ATTR_CCCD_INFO.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef struct
{
    uint16_t                conn_handle;
    uint8_t                 service_id;
    uint16_t                char_uuid;
    uint16_t                char_attrib_index;
    uint16_t                ccc_bits;
    uint32_t                param;
} T_SERVER_ATTR_CCCD_INFO;

/**
 * ble_audio.h
 *
 * \brief  LE Audio Client Attribute CCCD Data Information.
 *         The message data for LE_AUDIO_MSG_CLIENT_ATTR_CCCD_INFO.
 *         LE Audio Lib will send this message when CCCD configuration failed.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef struct
{
    uint16_t                conn_handle;
    bool                    srv_cfg;  /**<
                                       * \arg    true  : Enable all the characteristics CCCD in a service
                                       * \arg    false : Enable Characteristic CCCD request */
    uint8_t                 srv_instance_id;
    uint16_t                srv_uuid;
    uint8_t                 char_instance_id;
    uint16_t                char_uuid;
    uint16_t                cccd_cfg;  /**< CCCD value @ref group GATT_CLT_CONFIG. */
    uint16_t                cause;
} T_CLIENT_ATTR_CCCD_INFO;
/**
 * End of LEA_LIB_Exported_Types
 * \}
 */

/**
 * \defgroup    LEA_LIB_Exported_Functions BLE Audio Lib Exported Functions
 *
 * \ingroup LEA_LIB
 * \{
 */

/**
 * ble_audio.h
 *
 * \brief  Initialize LE audio.
 *
 * \param[in]  p_param      Pointer to ble audio initial parameters:
 *                          @ref T_BLE_AUDIO_PARAMS.
 *
 * \return         The result of initializing LE audio.
 * \retval true    Initialize LE audio success.
 * \retval false   Initialize LE audio failed.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
bool ble_audio_init(T_BLE_AUDIO_PARAMS *p_param);

/**
 * ble_audio.h
 *
 * \brief  Register BLE audio callback.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in] cback Callback function: @ref P_FUN_BLE_AUDIO_CB.
 * \return         The result of register BLE audio callback.
 * \retval true    Register BLE audio callback success.
 * \retval false   Register BLE audio callback failed.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
bool ble_audio_cback_register(P_FUN_BLE_AUDIO_CB cback);

/**
 * ble_audio.h
 *
 * \brief  Unregister BLE audio callback.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in] cback Callback function: @ref P_FUN_BLE_AUDIO_CB.
 * \return         The result of unregister BLE audio callback.
 * \retval true    Unregister BLE audio callback success.
 * \retval false   Unregister BLE audio callback failed.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
bool ble_audio_cback_unregister(P_FUN_BLE_AUDIO_CB cback);

/**
 * ble_audio.h
 *
 * \brief  Initialize LE audio Volume Control and Microphone Control.
 *
 * \param[in]  p_param      Pointer to ble audio Volume Control and Microphone
 *                          Control service initial parameter: @ref T_BLE_AUDIO_VC_MIC_PARAMS.
 *
 * \return         The result of initializing LE audio Volume Control and Microphone Control.
 * \retval true    Initialize LE audio Volume Control and Microphone Control success.
 * \retval false   Initialize LE audio Volume Control and Microphone Control failed.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
bool ble_audio_vc_mic_init(T_BLE_AUDIO_VC_MIC_PARAMS *p_param);

/**
 * ble_audio.h
 *
 * \brief  LE audio check remote features.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  array_index      Remote feature array index.
 * \param[in]  feature_mask     Remote feature mask.
 *
 * \return         The result of LE audio check remote features.
 * \retval true    LE audio remote features match.
 * \retval false   LE audio remote features not match.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
bool ble_audio_check_remote_features(uint16_t conn_handle, uint8_t array_index,
                                     uint8_t feature_mask);

/**
 * ble_audio.h
 *
 * \brief  LE audio send bond modify event.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  p_value      Pointer to le bond modify data: @ref T_BT_BOND_MODIFY.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
void ble_audio_bond_modify(T_BT_BOND_MODIFY *p_value);

/**
 * ble_audio.h
 *
 * \brief  LE audio handle io message.
 *         This api is used for message type IO_MSG_TYPE_LE_AUDIO.
 *
 * \param[in]  p_io_msg      Pointer to IO message: @ref T_IO_MSG.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
void ble_audio_handle_msg(T_IO_MSG *p_io_msg);

/**
 * ble_audio.h
 *
 * \brief  LE audio handle GAP message.
 *
 * \param[in]  subtype      GAP message subtype: @ref GAP_MSG_TYPE.
 * \param[in]  gap_msg      GAP message data: @ref T_LE_GAP_MSG.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
void ble_audio_handle_gap_msg(uint16_t subtype, T_LE_GAP_MSG gap_msg);

/**
 * ble_audio.h
 *
 * \brief  LE audio handle GAP callback message.
 *
 * \param[in]  cb_type      Callback type: @ref GAP_LE_MSG_Types.
 * \param[in]  p_cb_data    Pointer to the callback data.
 *
 * \ingroup LEA_LIB_Exported_Functions
 */
void ble_audio_handle_gap_cb(uint8_t cb_type, void *p_cb_data);


void ble_audio_deinit(void);
/**
 * End of LEA_LIB_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
