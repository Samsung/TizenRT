/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _PACS_MGR_H_
#define _PACS_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "codec_def.h"

/**
 * \defgroup    LEA_GAF_PACS_Server Published Audio Capabilities Server
 *
 * \brief   The server role for Published Audio Capabilities Service.
 */

/**
 * \defgroup    PACS_Server_Exported_Types Published Audio Capabilities Server Exported Types
 *
 * \ingroup LEA_GAF_PACS_Server
 * \{
 */

/**
 * pasc_mgr.h
 *
 * \brief  Write Sink Audio Location Parameter.
 *         The message data for LE_AUDIO_MSG_PACS_WRITE_SINK_AUDIO_LOC_IND.
 *
 * \ingroup PACS_Server_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint32_t sink_audio_locations;
} T_PACS_WRITE_SINK_AUDIO_LOC_IND;

/**
 * pasc_mgr.h
 *
 * \brief  Write Source Audio Location Parameter.
 *         The message data for LE_AUDIO_MSG_PACS_WRITE_SOURCE_AUDIO_LOC_IND.
 *
 * \ingroup PACS_Server_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint32_t source_audio_locations;
} T_PACS_WRITE_SOURCE_AUDIO_LOC_IND;

/**
 * pasc_mgr.h
 *
 * \brief  Available Audio Contexts Read Indication Parameter.
 *         The message data for LE_AUDIO_MSG_PACS_READ_AVAILABLE_CONTEXTS_IND.
 *
 * \ingroup PACS_Server_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint16_t cid;
} T_PACS_READ_AVAILABLE_CONTEXTS_IND;

/**
 * pasc_mgr.h
 *
 * \brief  PACS Service Parameters.
 *
 * \ingroup PACS_Server_Exported_Types
 */
typedef struct
{
    struct
    {
        bool is_exist;                       /**< Whether the sink location exist. */
        bool is_notify;                      /**< Whether notify is supported. */
        bool is_write;                       /**< Whether write is supported. */
        uint32_t sink_audio_location;        /**< Sink audio locations. */
    } sink_locations;                        /**< Sink locations parameters. */
    struct
    {
        bool is_exist;
        bool is_notify;
        bool is_write;
        uint32_t source_audio_location;      /**< Source audio locations. */
    } source_locations;                      /**< Source locations parameters. */
    struct
    {
        bool is_notify;
        uint16_t sink_supported_contexts;    /**< Sink supported contexts. */
        uint16_t source_supported_contexts;  /**< Source Supported contexts. */
    } supported_contexts;                    /**< Supported contexts parameters. */
} T_PACS_PARAMS;
/**
 * End of PACS_Server_Exported_Types
 * \}
 */

/**
 * \defgroup    PACS_Server_Exported_Functions Published Audio Capabilities Server Exported Functions
 *
 * \ingroup LEA_GAF_PACS_Server
 * \{
 */

/**
 * pasc_mgr.h
 *
 * \brief  Register PAC record. This api shall be called before pacs_init.
 *
 * \param[in]  direction      Audio direction: @ref T_AUDIO_DIRECTION.
 * \param[in]  pac_data       Pointer to the PAC data.
 *                            This parameter shall not use local variables, and audio lib only holds Pointers internally.
 * \param[in]  pac_data_len   PAC data length. This parameter shall not use local variables.
 * \param[in]  notify         Whether notify is supported.
 * \arg    true    Notify is supported.
 * \arg    false   Notify is not supported.
 *
 * \return                          PAC id.
 * \retval PAC_RECORD_INVALID_ID    Register PAC record failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
uint8_t pacs_pac_add(T_AUDIO_DIRECTION direction, const uint8_t *pac_data, uint8_t pac_data_len,
                     bool notify);

/**
 * pasc_mgr.h
 *
 * \brief  Initialize PACS Service.
 *         The api shall be called before gap_start_bt_stack.
 *
 * \param[in]  p_param      Pointer to PACS Service parameters: @ref T_PACS_PARAMS.
 *
 * \return         The result of initialize PACS.
 * \retval true    Initialize PACS Service success.
 * \retval false   Initialize PACS Service failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_init(T_PACS_PARAMS *p_param);

/**
 * pasc_mgr.h
 *
 * \brief  Update PAC record.
 *
 * \param[in]  pac_id        PAC id.
 * \param[in]  pac_data      Pointer to the PAC data.
 *                           This parameter shall not use local variables,
 *                           and audio lib only holds Pointers internally.
 * \param[in]  pac_data_len  PAC data length. This parameter shall not use local variables.
 *
 * \return         The result of update PAC record.
 * \retval true    Update PAC record success.
 * \retval false   Update PAC record failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_pac_update(uint8_t pac_id, const uint8_t *pac_data, uint8_t pac_data_len);

/**
 * pasc_mgr.h
 *
 * \brief  Set sink audio location.
 *
 * \param[in]  sink_audio_location      Sink audio location: @ref AUDIO_LOC.
 *
 * \return         The result of set sink audio location.
 * \retval true    Set sink audio location success.
 * \retval false   Set sink audio location failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_update_sink_audio_locations(uint32_t sink_audio_location);

/**
 * pasc_mgr.h
 *
 * \brief  Set source audio location.
 *
 * \param[in]  source_audio_location      Source audio location: @ref AUDIO_LOC.
 *
 * \return         The result of set source audio location.
 * \retval true    Set source audio location success.
 * \retval false   Set source audio location failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_update_source_audio_locations(uint32_t source_audio_location);

/**
 * pasc_mgr.h
 *
 * \brief  Send Available contexts read confirm.
 *
 * \param[in]  conn_handle                Connection handle.
 * \param[in]  cid                        Local channel ID assigned by Bluetooth stack.
 * \param[in]  sink_available_contexts    Available sink contexts: @ref AUDIO_CONTEXT.
 * \param[in]  source_available_contexts  Available source contexts: @ref AUDIO_CONTEXT.
 *
 * \return         The result of send available contexts read confirm.
 * \retval true    Send Available contexts read confirm success.
 * \retval false   Send Available contexts read confirm failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_available_contexts_read_cfm(uint16_t conn_handle, uint16_t cid,
                                      uint16_t sink_available_contexts, uint16_t source_available_contexts);

/**
 * pasc_mgr.h
 *
 * \brief  Update available contexts notification.
 *
 * \param[in]  conn_handle                Connection handle.
 * \param[in]  sink_available_contexts    Available sink contexts: @ref AUDIO_CONTEXT.
 * \param[in]  source_available_contexts  Available source contexts: @ref AUDIO_CONTEXT.
 *
 * \return         The result of update available contexts notification.
 * \retval true    Update available contexts notification success.
 * \retval false   Update available contexts notification failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_update_available_contexts(uint16_t conn_handle, uint16_t sink_available_contexts,
                                    uint16_t source_available_contexts);

/**
 * pasc_mgr.h
 *
 * \brief  Update supported audio contexts.
 *
 * \param[in]  sink_supported_contexts    Supported sink contexts: @ref AUDIO_CONTEXT.
 * \param[in]  source_supported_contexts  Supported source contexts: @ref AUDIO_CONTEXT.
 *
 * \return         The result of update supported audio contexts.
 * \retval true    Update supported audio contexts success.
 * \retval false   Update supported audio contexts failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_update_supported_contexts(uint16_t sink_supported_contexts,
                                    uint16_t source_supported_contexts);

/**
 * pasc_mgr.h
 *
 * \brief  Check PAC record codec configuration.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  direction    Audio direction: @ref T_AUDIO_DIRECTION.
 * \param[in]  codec_id     Pointer to codec id.
 * \param[in]  p_cfg        Pointer to codec configuration: @ref T_CODEC_CFG.
 *
 * \return         The result of check PAC record codec configuration.
 * \retval true    PAC record codec configuration check success.
 * \retval false   PAC record codec configuration check failed.
 *
 * \ingroup PACS_Server_Exported_Functions
 */
bool pacs_check_codec_cfg(uint8_t direction, uint8_t *codec_id, T_CODEC_CFG *p_cfg);
/**
 * End of PACS_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
