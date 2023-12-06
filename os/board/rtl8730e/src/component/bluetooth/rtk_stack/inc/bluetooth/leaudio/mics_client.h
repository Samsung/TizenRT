/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _MICS_CLIENT_H_
#define _MICS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "mics_def.h"
#include "bt_gatt_client.h"

/**
 * \defgroup    LEA_GAF_MICP_Client Microphone Control Client
 *
 * \brief   The client role for Microphone Control Profile.
 */

/**
 * \defgroup    MICP_Client_Exported_Types Microphone Control Client Exported Types
 *
 * \ingroup LEA_GAF_MICP_Client
 * \{
 */

/**
 * mics_client.h
 *
 * \brief  MICS client discover service result.
 *         The message data for LE_AUDIO_MSG_MICS_CLIENT_DIS_DONE.
 *
 * \ingroup MICP_Client_Exported_Types
 */
typedef struct
{
    uint16_t      conn_handle;
    bool          is_found;      /**< Whether to find service. */
    bool          load_from_ftl; /**< Whether the service table is loaded from FTL. */
    T_MICS_MUTE   mic_mute;
} T_MICS_CLIENT_DIS_DONE;

/**
 * mics_client.h
 *
 * \brief  MICS client write characteristic value result.
 *         The message data for LE_AUDIO_MSG_MICS_CLIENT_WRITE_RESULT.
 *
 * \ingroup MICP_Client_Exported_Types
 */
typedef struct
{
    uint16_t        conn_handle;
    uint16_t        cause;
} T_MICS_CLIENT_WRITE_RESULT;

/**
 * mics_client.h
 *
 * \brief  MICS notification data.
 *         The message data for LE_AUDIO_MSG_MICS_CLIENT_NOTIFY.
 *
 * \ingroup MICP_Client_Exported_Types
 */
typedef struct
{
    uint16_t            conn_handle;
    T_MICS_MUTE         mic_mute;
} T_MICS_CLIENT_NOTIFY;
/**
 * End of MICP_Client_Exported_Types
 * \}
 */

/**
 * \defgroup    MICP_Client_Exported_Functions Microphone Control Client Exported Functions
 *
 * \ingroup LEA_GAF_MICP_Client
 * \{
 */

/**
 * mics_client.h
 *
 * \brief  Get all of include AICS services.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  p_aics_instance  Pointer to AICS services attribute instance:
 *                              @ref T_ATTR_INSTANCE.
 *
 * \return         The result of get all of include AICS services.
 * \retval true    Get all of include AICS services success.
 * \retval false   Get all of include AICS services failed.
 *
 * \ingroup MICP_Client_Exported_Functions
 */
bool mics_get_all_inc_aics(uint16_t conn_handle, T_ATTR_INSTANCE *p_aics_instance);

/**
 * mics_client.h
 *
 * \brief  MICS client write mute value.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  mic_mute         MICS mute value: @ref T_MICS_MUTE.
 *
 * \return         The result of MICS client write mute value.
 * \retval true    MICS client write mute value success.
 * \retval false   MICS client write mute value failed.
 *
 * \ingroup MICP_Client_Exported_Functions
 */
bool mics_write_mute_value(uint16_t conn_handle, T_MICS_MUTE mic_mute);

/**
 * mics_client.h
 *
 * \brief  Get MICS mute value.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in,out]  p_value      Pointer to MICS mute value: @ref T_MICS_MUTE.
 *
 * \return         The result of get MICS mute value.
 * \retval true    Get MICS mute value success.
 * \retval false   Get MICS mute value failed.
 *
 * \ingroup MICP_Client_Exported_Functions
 */
bool mics_get_mute_value(uint16_t conn_handle, T_MICS_MUTE *p_value);
/**
 * End of MICP_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
