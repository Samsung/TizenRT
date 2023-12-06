/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CAP_H_
#define _CAP_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_group.h"
#include "csis_def.h"
#include "ble_audio.h"
#include "mics_def.h"

/**
 * \defgroup    LEA_GAF_CAP CAP Profile
 *
 * \brief   Common Audio Profile.
 */

/**
 * \defgroup    CAP_Exported_Macros CAP Profile Exported Macros
 *
 * \ingroup LEA_GAF_CAP
 * \{
 */

/**
 * cap.h
 *
 * \defgroup  CAP_ROLE Common Audio Profile Role
 *
 * \brief  Define Common Audio Profile Role.
 *
 * \ingroup CAP_Exported_Macros
 * \{
 */
#define CAP_ACCEPTOR_ROLE    0x01    /**< Acceptor Role. */
#define CAP_INITIATOR_ROLE   0x02    /**< Initiator Role. */
#define CAP_COMMANDER_ROLE   0x04    /**< Commander Role. */
/**
 * End of CAP_ROLE
 * \}
 */

/**
 * cap.h
 *
 * \brief  Define Common Audio Service UUID.
 *
 * \ingroup CAP_Exported_Macros
 */
#define GATT_UUID_CAS        0x1853

#define AUDIO_DESCRIPTION_STORAGE  0x01
/**
 * End of CAP_Exported_Macros
 * \}
 */

/**
 * \defgroup    CAP_Exported_Types CAP Profile Exported Types
 *
 * \ingroup LEA_GAF_CAP
 * \{
 */

/**
 * cap.h
 *
 * \brief  Common Audio Profile Initialization Parameter.
 *
 * \ingroup CAP_Exported_Types
 */
typedef struct
{
    uint8_t cap_role;                  /**< CAP Role. @ref CAP_ROLE */
    bool cas_client;                   /**< Whether to initialize CAS client. */
    bool csip_set_coordinator;         /**< Whether to initialize set coordinator. */
    uint8_t csis_num;                  /**< CSIS set member number. */
    struct
    {
        bool enable;                   /**< Whether to initialize CAS service. */
        T_CSIS_SIRK_TYPE csis_sirk_type;
        uint8_t csis_size;
        uint8_t csis_rank;
        uint8_t csis_feature;          /**< @ref SET_MEMBER_FEATURE */
        uint8_t *csis_sirk;
    } cas;                             /**< Parameters for CSIS service */
    struct
    {
        bool vcp_vcs_client;           /**< Whether to initialize VCS client. */
        bool vcp_aics_client;          /**< Whether to initialize AICS client. */
        uint8_t vcp_aics_cfg;
        bool vcp_vocs_client;          /**< Whether to initialize VOCS client. */
        uint8_t vcp_vocs_cfg;
        bool micp_mic_controller;      /**< Whether to initialize MICS client. */
    } vcp_micp;                        /**< Parameters for VCP and MICP */

    bool ccp_call_control_client;      /**< Whether to initialize CCP client. */
    struct
    {
        bool ccp_call_control_server;  /**< Whether to initialize ccp server. */
        uint8_t tbs_num;               /**< TBS service number. */
    } tbs;                             /**< Parameters for TBS service */
    bool mcp_media_control_client;     /**< Whether to initialize MCP client. */
    struct
    {
        bool mcp_media_control_server; /**< Whether to initialize MCP client. */
        uint8_t mcs_num;               /**< MCS service number. */
        uint8_t ots_num;               /**< OTS service number. */
    } mcs;                             /**< Parameters for MCS service */
} T_CAP_INIT_PARAMS;

/**
 * cap.h
 *
 * \brief  Common Audio Profile Client Discover Service Result.
 *          The message data for LE_AUDIO_MSG_CAP_DIS_DONE.
 *
 * \ingroup CAP_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool     load_from_ftl; /**< Whether the service table is loaded from FTL. */
    bool     cas_is_found;  /**< Whether to find CAS. */
    bool     cas_inc_csis;  /**< Whether the CAS instance include the CSIS instance. */
    bool     vcs_is_found;  /**< Whether to find VCS. */
    bool     mics_is_found; /**< Whether to find MICS. */
} T_CAP_DIS_DONE;
/**
 * End of CAP_Exported_Types
 * \}
 */

/**
 * \defgroup    CAP_Exported_Functions CAP Profile Exported Functions
 *
 * \ingroup LEA_GAF_CAP
 * \{
 */

/**
 * cap.h
 *
 * \brief  Change Volume Mute State procedure.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  mute              Mute.
 *
 * \return Operation result        TThe result of change mute.
 * \retval LE_AUDIO_CAUSE_SUCCESS  Change mute success.
 * \retval Others                  Change mute failed.
 *
 * \ingroup CAP_Exported_Functions
 */
T_LE_AUDIO_CAUSE cap_vcs_change_mute(T_BLE_AUDIO_GROUP_HANDLE group_handle, uint8_t mute);

/**
 * cap.h
 *
 * \brief  Change volume setting.
 *         This API shall be called after the LE_AUDIO_MSG_CAP_DIS_DONE is received by application.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  volume_setting    Volume setting.
 *
 * \return Operation result        TThe result of change volume.
 * \retval LE_AUDIO_CAUSE_SUCCESS  Change volume success.
 * \retval Others                  Change volume failed.
 *
 * \ingroup CAP_Exported_Functions
 */
T_LE_AUDIO_CAUSE cap_vcs_change_volume(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                       uint8_t volume_setting);

/**
 * cap.h
 *
 * \brief  Change Microphone Mute State procedure.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  mic_mute          Mute.
 *
 * \return Operation result        TThe result of change mute.
 * \retval LE_AUDIO_CAUSE_SUCCESS  Change mute success.
 * \retval Others                  Change mute failed.
 *
 * \ingroup CAP_Exported_Functions
 */
T_LE_AUDIO_CAUSE cap_mics_change_mute(T_BLE_AUDIO_GROUP_HANDLE group_handle, T_MICS_MUTE mic_mute);

/**
 * cap.h
 *
 * \brief  Initialize CAP profile.
 *
 * \param[in]  p_param      CAP initialize parameters: @ref T_CAP_INIT_PARAMS.
 *
 * \return Operation result        The result of initialize CAP profile.
 * \retval true                    Initialize CAP profile success.
 * \retval false                   Initialize CAP profile failed.
 *
 * \ingroup CAP_Exported_Functions
 */
bool cap_init(T_CAP_INIT_PARAMS *p_param);
/**
 * End of CAP_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
