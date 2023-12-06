/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _PACS_CLIENT_H_
#define _PACS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/**
 * \defgroup    LEA_GAF_PACS_Client Published Audio Capabilities Client
 *
 * \brief   The client role for Published Audio Capabilities Service.
 */

/**
 * \defgroup    PACS_Client_Exported_Types Published Audio Capabilities Client Exported Types
 *
 * \ingroup LEA_GAF_PACS_Client
 * \{
 */

/**
 * pacs_client.h
 *
 * \brief  Published Audio Capabilities Service characteristic Type.
 *
 * \ingroup PACS_Client_Exported_Types
 */
typedef enum
{
    PACS_AUDIO_AVAILABLE_CONTEXTS,
    PACS_AUDIO_SUPPORTED_CONTEXTS,
    PACS_SINK_AUDIO_LOC,
    PACS_SINK_PAC,
    PACS_SOURCE_AUDIO_LOC,
    PACS_SOURCE_PAC,
} T_PACS_TYPE;

/**
 * pacs_client.h
 *
 * \brief  PACS Service Client Discover Result.
 *         The message data for LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE.
 *
 * \ingroup PACS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;      /**< Whether to find service. */
    bool    load_from_ftl; /**< Whether the service table is loaded from FTL. */
    uint8_t sink_pac_num;
    uint8_t source_pac_num;
    bool    sink_loc_writable;
    bool    sink_loc_exist;
    bool    source_loc_writable;
    bool    source_loc_exist;
} T_PACS_CLIENT_DIS_DONE;

/**
 * pacs_client.h
 *
 * \brief  PACS Service Client Write Characteristic Value Result.
 *         The message data for LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT.
 *
 * \ingroup PACS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint16_t cause;
} T_PACS_CLIENT_WRITE_SINK_AUDIO_LOC_RESULT;

/**
 * pacs_client.h
 *
 * \brief  PACS Service Client Write Characteristic Value Result.
 *         The message data for LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT.
 *
 * \ingroup PACS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint16_t cause;
} T_PACS_CLIENT_WRITE_SOURCE_AUDIO_LOC_RESULT;
/**
 * End of PACS_Client_Exported_Types
 * \}
 */

/**
 * \defgroup    PACS_Client_Exported_Functions Published Audio Capabilities Client Exported Functions
 *
 * \ingroup LEA_GAF_PACS_Client
 * \{
 */

/**
 * pacs_client.h
 *
 * \brief  PACS client write sink audio locations.
 *
 * \param[in]  conn_handle          Connection handle.
 * \param[in]  sink_audio_location  Sink audio location.
 *
 * \return         The result of write sink audio locations.
 * \retval true    PACS write sink audio locations success.
 * \retval false   PACS write sink audio locations failed.
 *
 * \ingroup PACS_Client_Exported_Functions
 */
bool pacs_write_sink_audio_locations(uint16_t conn_handle, uint32_t sink_audio_location);

/**
 * pacs_client.h
 *
 * \brief  PACS client write source audio locations.
 *
 * \param[in]  conn_handle            Connection handle.
 * \param[in]  source_audio_location  Source audio location.
 *
 * \return         The result of write source audio locations.
 * \retval true    PACS write source audio locations success.
 * \retval false   PACS write source audio locations failed.
 *
 * \ingroup PACS_Client_Exported_Functions
 */
bool pacs_write_source_audio_locations(uint16_t conn_handle, uint32_t source_audio_location);
/**
 * End of PACS_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
