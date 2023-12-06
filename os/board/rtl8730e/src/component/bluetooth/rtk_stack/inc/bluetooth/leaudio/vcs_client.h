/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _VCS_CLIENT_H_
#define _VCS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "vcs_def.h"
#include "ble_audio.h"
#include "ble_audio_group.h"

/**
 * \defgroup    LEA_GAF_VCP_Client Volume Control Client
 *
 * \brief   The client role for Volume Control Profile.
 */

/**
 * \defgroup    VCP_Client_Exported_Macros Volume Control Client Exported Macros
 *
 * \ingroup LEA_GAF_VCP_Client
 * \{
 */

/**
 * vcs_client.h
 *
 * \defgroup    VCS_CHAR_EXIST VCS Service Characteristic Exist Flag
 *
 * \brief  Define VCS Service Characteristic Exist Flags.
 *
 * \ingroup VCP_Client_Exported_Macros
 * \{
 */
#define VCS_VOLUME_STATE_FLAG 0x01    /**< Volume State Exist Flag */
#define VCS_VOLUME_FLAGS_FLAG 0x02    /**< Volume Flags Exist Flag */
/**
 * End of VCS_CHAR_EXIST
 * \}
 */

/**
 * End of VCP_Client_Exported_Macros
 * \}
 */

/**
 * \defgroup    VCP_Client_Exported_Types Volume Control Client Exported Types
 *
 * \ingroup LEA_GAF_VCP_Client
 * \{
 */

/**
 * vcs_client.h
 *
 * \brief  VCS Client Discover Service Result.
 *         The message data for LE_AUDIO_MSG_VCS_CLIENT_DIS_DONE.
 *
 * \ingroup VCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool     is_found;      /**< Whether to find service. */
    bool     load_from_ftl; /**< Whether the service table is loaded from FTL. */
    uint8_t  type_exist;
} T_VCS_CLIENT_DIS_DONE;

/**
 * vcs_client.h
 *
 * \brief  VCS Client Send Volume Control Point Parameter.
 *
 * \ingroup VCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t volume_setting;   /**< volume_setting, used only for VCS_CP_SET_ABSOLUTE_VOLUME */
} T_VCS_VOLUME_CP_PARAM;

/**
 * vcs_client.h
 *
 * \brief  Volume Control Service Characteristic Type.
 *
 * \ingroup VCP_Client_Exported_Types
 */
typedef enum
{
    VCS_CHAR_VOLUME_STATE,
    VCS_CHAR_VOLUME_FLAGS,
} T_VCS_CHAR_TYPE;

/**
 * vcs_client.h
 *
 * \brief  Volume Control Service Data.
 *
 * \ingroup VCP_Client_Exported_Types
 */
typedef union
{
    uint8_t volume_flags;
    T_VOLUME_STATE volume_state;
} T_VCS_DATA;

/**
 * vcs_client.h
 *
 * \brief  VCS client read characteristic value result.
 *         The message data for LE_AUDIO_MSG_VCS_CLIENT_VOLUME_STATE_DATA.
 *
 * \ingroup VCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t        conn_handle;
    bool            is_notify;
    T_VCS_CHAR_TYPE type;
    T_VCS_DATA      data;
} T_VCS_CLIENT_VOLUME_STATE_DATA;

/**
 * vcs_client.h
 *
 * \brief  VCS Client write control point result.
 *
 * \ingroup VCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t    conn_handle;
    uint16_t    cause;
    T_VCS_CP_OP cp_op;
} T_VCS_CLIENT_CP_RESULT;
/**
 * End of VCP_Client_Exported_Types
 * \}
 */

/**
 * \defgroup    VCP_Client_Exported_Functions Volume Control Client Exported Functions
 *
 * \ingroup LEA_GAF_VCP_Client
 * \{
 */

/**
 * vcs_client.h
 *
 * \brief  VCS client write volume control point.
 *         If this API is called successfully, the Application will receive
 *         LE_AUDIO_MSG_VCS_CLIENT_CP_RESULT.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  op               Volume control point opcode: @ref T_VCS_CP_OP.
 * \param[in]  p_param          Pointer to volume control point parameter: @ref T_VCS_VOLUME_CP_PARAM.
 *
 * \return         The result of VCS client write volume control point.
 * \retval true    VCS client write volume control point success.
 * \retval false   VCS client write volume control point failed.
 *
 * \ingroup VCP_Client_Exported_Functions
 */
bool vcs_write_cp(uint16_t conn_handle, T_VCS_CP_OP op, T_VCS_VOLUME_CP_PARAM *p_param);

/**
 * vcs_client.h
 *
 * \brief  Get volume state.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in,out]  p_value      Pointer to volume state data: @ref T_VOLUME_STATE.
 *
 * \return         The result of get volume state.
 * \retval true    Get volume state success.
 * \retval false   Get volume state failed.
 *
 * \ingroup VCP_Client_Exported_Functions
 */
bool vcs_get_volume_state(uint16_t conn_handle, T_VOLUME_STATE *p_value);

/**
 * vcs_client.h
 *
 * \brief  Get volume flags.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in,out]  p_value      Pointer to volume flags.
 *
 * \return         The result of get volume flags.
 * \retval true    Get volume flags success.
 * \retval false   Get volume flags failed.
 *
 * \ingroup VCP_Client_Exported_Functions
 */
bool vcs_get_volume_flags(uint16_t conn_handle, uint8_t *p_value);

/**
 * vcs_client.h
 *
 * \brief  VCS client send volume control point by ble audio group.
 *         If this API is called successfully, the Application will receive
 *         LE_AUDIO_MSG_VCS_CLIENT_CP_RESULT.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  group_handle   BLE audio group: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  op             Volume control point opcode: @ref T_VCS_CP_OP.
 * \param[in]  p_param        Pointer to volume control point parameter: @ref T_VCS_VOLUME_CP_PARAM.
 *
 * \return         The result of VCS client send volume control point by ble audio group.
 * \retval true    VCS client send volume control point by ble audio group success.
 * \retval false   VCS client send volume control point by ble audio group failed.
 *
 * \ingroup VCP_Client_Exported_Functions
 */
T_LE_AUDIO_CAUSE vcs_write_cp_by_group(T_BLE_AUDIO_GROUP_HANDLE group_handle, T_VCS_CP_OP op,
                                       T_VCS_VOLUME_CP_PARAM *p_param);
/**
 * End of VCP_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
