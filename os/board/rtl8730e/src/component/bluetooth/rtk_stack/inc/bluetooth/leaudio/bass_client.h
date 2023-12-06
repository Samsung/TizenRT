/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BASS_CLIENT_H_
#define _BASS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "bass_def.h"
#include "ble_audio_sync.h"
#include "broadcast_source_sm.h"

/**
 * \defgroup    LEA_GAF_BASS_Client Broadcast Audio Scan Client
 *
 * \brief   The client role for Broadcast Audio Scan Service.
 */

/**
 * \defgroup    BASS_Client_Exported_Types Broadcast Audio Scan Client Exported Types
 *
 * \ingroup LEA_GAF_BASS_Client
 * \{
 */

/**
 * bass_client.h
 *
 * \brief  BASS Read Broadcast Receive State Result.
 *         The message data for LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA.
 *
 * \ingroup BASS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool notify;
    uint16_t read_cause;
    uint8_t char_instance_id;
    T_BASS_BRS_DATA *p_brs_data;
} T_BASS_CLIENT_BRS_DATA;

/**
 * bass_client.h
 *
 * \brief  PA Sync Information Request Data.
 *         The message data for LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ.
 *
 * \ingroup BASS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint8_t char_instance_id;
    T_BASS_BRS_DATA *p_brs_data;
} T_BASS_CLIENT_SYNC_INFO_REQ;

/**
 * bass_client.h
 *
 * \brief  BASS Client Write Control Point Result.
 *         The message data for LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT.
 *
 * \ingroup BASS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint16_t cause;
} T_BASS_CLIENT_CP_RESULT;
/**
 * End of BASS_Client_Exported_Types
 * \}
 */

/**
 * \defgroup    BASS_Client_Exported_Functions Broadcast Audio Scan Client Exported Functions
 *
 * \ingroup LEA_GAF_BASS_Client
 * \{
 */

/**
 * bass_client.h
 *
 * \brief  Write control point remote scan stopped operation.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point remote scan stopped operation.
 * \retval true    Write control point remote scan stopped operation success.
 * \retval false   Write control point remote scan stopped operation failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_remote_scan_stop(uint16_t conn_handle, bool is_req);

/**
 * bass_client.h
 *
 * \brief  Write control point remote scan started operation.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point remote scan started operation.
 * \retval true    Write control point remote scan started operation success.
 * \retval false   Write control point remote scan started operation failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_remote_scan_start(uint16_t conn_handle, bool is_req);

/**
 * bass_client.h
 *
 * \brief  Write control point add source operation.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  p_cp_data        Pointer to control point add source parameter:
 *                              @ref T_BASS_CP_ADD_SOURCE.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point add source operation.
 * \retval true    Write control point add source operation success.
 * \retval false   Write control point add source operation failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_add_source(uint16_t conn_handle, T_BASS_CP_ADD_SOURCE *p_cp_data, bool is_req);

/**
 * bass_client.h
 *
 * \brief  Write control point modify source operation.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  p_cp_data        Pointer to control point modify source parameter:
 *                              @ref T_BASS_CP_MODIFY_SOURCE.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point modify source operation.
 * \retval true    Write control point modify source operation success.
 * \retval false   Write control point modify source operation failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_modify_source(uint16_t conn_handle, T_BASS_CP_MODIFY_SOURCE *p_cp_data, bool is_req);

/**
 * bass_client.h
 *
 * \brief  Write control point set Broadcast_Code operation.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  p_cp_data        Pointer to control point set Broadcast_Code parameter:
 *                              @ref T_BASS_CP_SET_BROADCAST_CODE.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point set Broadcast_Code operation.
 * \retval true    Write control point set Broadcast_Code operation success.
 * \retval false   Write control point set Broadcast_Code operation failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_set_broadcast_code(uint16_t conn_handle, T_BASS_CP_SET_BROADCAST_CODE *p_cp_data,
                                bool is_req);

/**
 * bass_client.h
 *
 * \brief  Write control point remove source operation.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  p_cp_data        Pointer to control point remove source parameter:
 *                              @ref T_BASS_CP_REMOVE_SOURCE.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point remove source operation.
 * \retval true    Write control point remove source operation success.
 * \retval false   Write control point remove source operation failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_remove_source(uint16_t conn_handle, T_BASS_CP_REMOVE_SOURCE *p_cp_data, bool is_req);

/**
 * bass_client.h
 *
 * \brief  BASS client get Broadcast Receive State data.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  instance_id      BRS instance id.
 *
 * \return         The Broadcast Receive State data: @ref T_BASS_BRS_DATA.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
T_BASS_BRS_DATA *bass_get_brs_data(uint16_t conn_handle, uint8_t char_instance_id);

/**
 * bass_client.h
 *
 * \brief  Write control point add source operation by sync information.
 *
 * \param[in]  handle           BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  pa_sync          PA_Sync parameter: @ref T_BASS_PA_SYNC.
 * \param[in]  bis_array        BIS array.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point add source operation by sync information.
 * \retval true    Write control point add source operation by sync information success.
 * \retval false   Write control point add source operation by sync information failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_add_source_by_sync_info(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                                     T_BASS_PA_SYNC pa_sync, uint32_t bis_array, bool is_req);

/**
 * bass_client.h
 *
 * \brief  Write control point modify source operation by sync information.
 *
 * \param[in]  handle           BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  source_id        Broadcast source id.
 * \param[in]  pa_sync          PA_Sync parameter: @ref T_BASS_PA_SYNC.
 * \param[in]  bis_array        BIS array.
 * \param[in]  is_req           Whether to send write request.
 * \arg    true    Send write request.
 * \arg    false   Send write command.
 *
 * \return         The result of write control point modify source operation by sync information.
 * \retval true    Write control point modify source operation by sync information success.
 * \retval false   Write control point modify source operation by sync information failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_cp_modify_source_by_sync_info(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                                        uint8_t source_id,
                                        T_BASS_PA_SYNC pa_sync, uint32_t bis_array, bool is_req);
/**
 * bass_client.h
 *
 * \brief  Initiating PAST for local PA.
 *
 * \param[in]  handle           BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_data         PAST Service Data: @ref T_BASS_PAST_SRV_DATA.
 * \return         The result of Initiating PAST for local PA by sync information.
 * \retval true    Initiating PAST for local PA by sync information success.
 * \retval false   Initiating PAST for local PA by sync information failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_past_by_local_src(T_BROADCAST_SOURCE_HANDLE handle, uint16_t conn_handle,
                            T_BASS_PAST_SRV_DATA srv_data);

/**
 * bass_client.h
 *
 * \brief  Initiating PAST for remote PA.
 *
 * \param[in]  handle           BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_data         PAST Service Data: @ref T_BASS_PAST_SRV_DATA.
 * \return         The result of Initiating PAST for remote PA by sync information.
 * \retval true    Initiating PAST for remote PA by sync information success.
 * \retval false   Initiating PAST for remote PA by sync information failed.
 *
 * \ingroup BASS_Client_Exported_Functions
 */
bool bass_past_by_remote_src(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                             T_BASS_PAST_SRV_DATA srv_data);
/**
 * End of BASS_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
