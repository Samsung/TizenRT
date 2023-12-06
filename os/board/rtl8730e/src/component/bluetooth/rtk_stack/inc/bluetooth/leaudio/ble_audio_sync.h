/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BLE_AUDIO_SYNC_H_
#define _BLE_AUDIO_SYNC_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "gap_big_mgr.h"
#include "gap_pa_sync.h"
#include "gap_past_recipient.h"
#include "ble_audio_def.h"
#include "base_data_parse.h"

/**
 * \defgroup    LEA_GAF_BAP BAP Profile
 *
 * \brief   Basic Audio Profile.
 */

/**
 * \defgroup    BAP_Exported_Macros BAP Profile Exported Macros
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap_audio_sync.h
 *
 * \brief  Define type T_BLE_AUDIO_SYNC_HANDLE. BLE Audio Sync Handle.
 *
 * \ingroup BAP_Exported_Macros
 */
typedef void *T_BLE_AUDIO_SYNC_HANDLE;

/**
 * bap_audio_sync.h
 *
 * \defgroup BLE_AUDIO_SYNC_MSG BLE Audio Sync Message.
 *
 * \brief  Define BLE Audio Synchronization Message.
 *
 * \ingroup BAP_Exported_Macros
 * \{
 */
#define MSG_BLE_AUDIO_PA_SYNC_STATE         0x01
#define MSG_BLE_AUDIO_PA_REPORT_INFO        0x02
#define MSG_BLE_AUDIO_BASE_DATA_MODIFY_INFO 0x03
#define MSG_BLE_AUDIO_PA_BIGINFO            0x04

#define MSG_BLE_AUDIO_BIG_SYNC_STATE        0x11
#define MSG_BLE_AUDIO_BIG_SETUP_DATA_PATH   0x12
#define MSG_BLE_AUDIO_BIG_REMOVE_DATA_PATH  0x13

#define MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED  0x20
#define MSG_BLE_AUDIO_ADDR_UPDATE           0x21
/**
 * End of BLE_AUDIO_SYNC_MSG
 * \}
 */

/**
 * End of BAP_Exported_Macros
 * \}
 */

/**
 * \defgroup    BAP_Exported_Types BAP Profile Exported Types
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap_audio_sync.h
 *
 * \brief  Define BLE Audio Action Role.
 *
 * \ingroup BAP_Exported_Types
 */
typedef enum
{
    BLE_AUDIO_ACTION_ROLE_IDLE      = 0x00,
    BLE_AUDIO_ACTION_ROLE_LOCAL_API = 0x01,
    BLE_AUDIO_ACTION_ROLE_BASS      = 0x02,
} T_BLE_AUDIO_ACTION_ROLE;

/**
 * bap_audio_sync.h
 *
 * \brief  Define BLE Audio BIG Action.
 *
 * \ingroup BAP_Exported_Types
 */
typedef enum
{
    BLE_AUDIO_BIG_IDLE         = 0x00,
    BLE_AUDIO_BIG_SYNC         = 0x01,
    BLE_AUDIO_BIG_TERMINATE    = 0x02,
    BLE_AUDIO_BIG_LOST         = 0x03,
} T_BLE_AUDIO_BIG_ACTION;

/**
 * bap_audio_sync.h
 *
 * \brief  Parameters for BLE Audio BIG Synchronization State.
 *         The message data for MSG_BLE_AUDIO_BIG_SYNC_STATE.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE       sync_state;
    uint8_t encryption;
    T_BLE_AUDIO_BIG_ACTION     action;
    T_BLE_AUDIO_ACTION_ROLE    action_role;
    uint16_t                   cause;
} T_BLE_AUDIO_BIG_SYNC_STATE;

/**
 * bap_audio_sync.h
 *
 * \brief  Parameters for BLE Audio BIG Setup Data Path.
 *         The message data for MSG_BLE_AUDIO_BIG_SETUP_DATA_PATH.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint8_t bis_idx;
    uint16_t bis_conn_handle;
    uint16_t cause;
} T_BLE_AUDIO_BIG_SETUP_DATA_PATH;

/**
 * bap_audio_sync.h
 *
 * \brief  Parameters for BLE Audio Base Data Modify Information.
 *         The message data for MSG_BLE_AUDIO_BASE_DATA_MODIFY_INFO.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_BASE_DATA_MAPPING *p_base_mapping;
} T_BLE_AUDIO_BASE_DATA_MODIFY_INFO;

/**
 * bap_audio_sync.h
 *
 * \brief  Parameters for BLE Audio BIG Remove Data Path.
 *         The message data for MSG_BLE_AUDIO_BIG_REMOVE_DATA_PATH.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint8_t bis_idx;
    uint16_t bis_conn_handle;
    uint16_t cause;
} T_BLE_AUDIO_BIG_REMOVE_DATA_PATH;

/**
 * bap_audio_sync.h
 *
 * \brief  Define BLE Audio PA Action.
 *
 * \ingroup BAP_Exported_Types
 */
typedef enum
{
    BLE_AUDIO_PA_IDLE         = 0x00,
    BLE_AUDIO_PA_SYNC         = 0x01,
    BLE_AUDIO_PA_TERMINATE    = 0x02,
    BLE_AUDIO_PA_LOST         = 0x03,
} T_BLE_AUDIO_PA_ACTION;

/**
 * bap_audio_sync.h
 *
 * \brief  BLE Audio PA Synchronization State.
 *         The Message Data for MSG_BLE_AUDIO_PA_SYNC_STATE.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_GAP_PA_SYNC_STATE   sync_state;
    T_BLE_AUDIO_PA_ACTION action;
    T_BLE_AUDIO_ACTION_ROLE action_role;
    uint16_t              cause;
} T_BLE_AUDIO_PA_SYNC_STATE;

/**
 * bap_audio_sync.h
 *
 * \brief  BLE Audio Sync Handle Released Parameter.
 *         The Message Data for MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_ACTION_ROLE action_role;
} T_BLE_AUDIO_SYNC_HANDLE_RELEASED;

/**
 * bap_audio_sync.h
 *
 * \brief  BLE Audio Advertiser Address Update.
 *         The Message Data for MSG_BLE_AUDIO_ADDR_UPDATE.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint8_t *advertiser_address;
} T_BLE_AUDIO_ADDR_UPDATE;

/**
 * bap_audio_sync.h
 *
 * \brief  BLE Audio Synchronization Callback Data.
 *         The Message Data for MSG_BLE_AUDIO_ADDR_UPDATE.
 *
 * \ingroup BAP_Exported_Types
 */
typedef union
{
    T_BLE_AUDIO_PA_SYNC_STATE         *p_pa_sync_state;
    T_LE_PERIODIC_ADV_REPORT_INFO     *p_le_periodic_adv_report_info;
    T_BLE_AUDIO_BASE_DATA_MODIFY_INFO *p_base_data_modify_info;

    T_BLE_AUDIO_BIG_SYNC_STATE        *p_big_sync_state;
    T_LE_BIGINFO_ADV_REPORT_INFO      *p_le_biginfo_adv_report_info;
    T_BLE_AUDIO_BIG_SETUP_DATA_PATH   *p_setup_data_path;
    T_BLE_AUDIO_BIG_REMOVE_DATA_PATH  *p_remove_data_path;

    T_BLE_AUDIO_SYNC_HANDLE_RELEASED  *p_sync_handle_released;
    T_BLE_AUDIO_ADDR_UPDATE           *p_addr_update;
} T_BLE_AUDIO_SYNC_CB_DATA;

/**
 * bap_audio_sync.h
 *
 * \brief  BLE Audio Sync Information.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint8_t advertiser_address_type;
    uint8_t advertiser_address[GAP_BD_ADDR_LEN];
    uint8_t adv_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    /**< PA Sync information */
    uint8_t sync_id;
    T_GAP_PA_SYNC_STATE pa_state;
    uint16_t pa_interval;
    T_BASE_DATA_MAPPING *p_base_mapping;
    uint8_t pa_sync_addr[GAP_BD_ADDR_LEN];
    uint8_t pa_sync_addr_type;
    bool big_info_received;
    T_LE_BIGINFO_ADV_REPORT_INFO big_info;
    /**< BIG Sync information */
    T_GAP_BIG_SYNC_RECEIVER_SYNC_STATE big_state;
} T_BLE_AUDIO_SYNC_INFO;

/**
 * bap_audio_sync.h
 *
 * \brief  BLE Audio BIS Information.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint8_t  bis_num;
    T_BIG_MGR_BIS_CONN_HANDLE_INFO bis_info[GAP_BIG_MGR_MAX_BIS_NUM];
} T_BLE_AUDIO_BIS_INFO;

/**
 * bap_audio_sync.h
 *
 * \brief  P_FUN_BLE_AUDIO_SYNC_CB BLE Audio Sync Callback Function Point Definition.
 *         Function pointer used in ble audio module, to send events @ref BLE_AUDIO_SYNC_MSG
 *         to application.
 *
 * \ingroup BAP_Exported_Types
 */
typedef void(*P_FUN_BLE_AUDIO_SYNC_CB)(T_BLE_AUDIO_SYNC_HANDLE handle, uint8_t cb_type,
                                       void *p_cb_data);
/**
 * End of BAP_Exported_Types
 * \}
 */

/**
 * \defgroup    BAP_Exported_Functions BAP Profile Exported Functions
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap_audio_sync.h
 *
 * \brief  Allocate ble audio sync.
 *
 * \param[in]  cb_pfn                   Audio sync callback function: @ref P_FUN_BLE_AUDIO_SYNC_CB.
 * \param[in]  advertiser_address_type  Advertiser address type.
 * \param[in]  advertiser_address       Pointer to advertiser address.
 * \param[in]  adv_sid                  Advertising SID.
 * \param[in]  broadcast_id             Broadcast ID.
 *
 * \return         The ble audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \retval NULL    Allocate ble audio PA synchronization failed.
 *
 * \ingroup BAP_Exported_Functions
 */
T_BLE_AUDIO_SYNC_HANDLE ble_audio_sync_allocate(P_FUN_BLE_AUDIO_SYNC_CB cb_pfn,
                                                uint8_t advertiser_address_type,
                                                uint8_t *advertiser_address, uint8_t adv_sid,
                                                uint8_t broadcast_id[BROADCAST_ID_LEN]);

/**
 * bap_audio_sync.h
 *
 * \brief  Register ble audio sync update callback function to handle ble audio sync message.
 *
 * \param[in]  handle      BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  cb_pfn      BLE audio sync callback function: @ref P_FUN_BLE_AUDIO_SYNC_CB.
 *
 * \return         The result of register ble audio sync update callback function.
 * \retval true    Register ble audio sync update callback function success.
 * \retval false   Register ble audio sync update callback function failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_sync_update_cb(T_BLE_AUDIO_SYNC_HANDLE handle,
                              P_FUN_BLE_AUDIO_SYNC_CB cb_pfn);

/**
 * bap_audio_sync.h
 *
 * \brief  Update ble audio sync advertiser address.
 *
 * \param[in]  handle              BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  advertiser_address  Pointer to advertiser address.
 *
 * \return         The result of update ble audio sync advertiser address.
 * \retval true    Update ble audio sync advertiser address success.
 * \retval false   Update ble audio sync advertiser address failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_sync_update_addr(T_BLE_AUDIO_SYNC_HANDLE handle,
                                uint8_t *advertiser_address);

/**
 * bap_audio_sync.h
 *
 * \brief  Find ble audio sync.
 *
 * \param[in]  adv_sid                  Advertising sid.
 * \param[in]  broadcast_id             Broadcast id.
 *
 * \return         The ble audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \retval NULL    Not find ble audio sync handle.
 *
 * \ingroup BAP_Exported_Functions
 */
T_BLE_AUDIO_SYNC_HANDLE ble_audio_sync_find(uint8_t adv_sid,
                                            uint8_t broadcast_id[BROADCAST_ID_LEN]);

/**
 * bap_audio_sync.h
 *
 * \brief  Get ble audio sync information.
 *
 * \param[in]  handle      BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in,out]  p_info  Pointer to ble audio sync information: @ref T_BLE_AUDIO_SYNC_INFO.
 *
 * \return         The result of get ble audio sync information.
 * \retval true    Get ble audio sync information success.
 * \retval false   Get ble audio sync information failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_sync_get_info(T_BLE_AUDIO_SYNC_HANDLE handle, T_BLE_AUDIO_SYNC_INFO *p_info);

/**
 * bap_audio_sync.h
 *
 * \brief  Release ble audio sync.
 *         If this api is successfully invoked, the referred sync handle will be cleared.
 *
 * \param[in]  p_handle     Pointer to ble audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 *
 * \return         The result of release ble audio sync.
 * \retval true    Release ble audio sync success.
 * \retval false   Release ble audio sync failed.
 *
 * \ingroup BAP_Exported_Functions
 */

bool ble_audio_sync_release(T_BLE_AUDIO_SYNC_HANDLE *p_handle);

/**
 * bap_audio_sync.h
 *
 * \brief  Send establish ble audio PA sync request.
 *
 * \param[in]  handle         BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  options        PA create sync Options.
 * \param[in]  sync_cte_type  PA sync CTE type.
 * \param[in]  skip           The maximum number of periodic advertising events that can be
                              skipped after a successful receive.
                              Range: 0x0000 to 0x01F3
 * \param[in]  sync_timeout   Sync timeout for the PA train.
                              Range: 0x000A to 0x4000
                              Time = N*10 ms
                              Time Range: 100 ms to 163.84 s
 *
 * \return         The result of establish ble audio PA sync.
 * \retval true    Send request success.
 * \retval false   Send request failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_pa_sync_establish(T_BLE_AUDIO_SYNC_HANDLE handle, uint8_t options,
                                 uint8_t sync_cte_type,
                                 uint16_t skip, uint16_t sync_timeout);

/**
 * bap_audio_sync.h
 *
 * \brief  Send terminate ble audio PA sync request.
 *
 * \param[in]  handle      BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 *
 * \return         The result of terminate ble audio PA sync.
 * \retval true    Send request success.
 * \retval false   Send request failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_pa_terminate(T_BLE_AUDIO_SYNC_HANDLE handle);

/**
 * bap_audio_sync.h
 *
 * \brief  Send establish ble audio big sync request.
 *
 * \param[in]  handle        BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  p_param       Pointer to big create sync parameter:
 *                           @ref T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM.
 *
 * \return         The result of establish ble audio big sync.
 * \retval true    Send request success.
 * \retval false   Send request failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_big_sync_establish(T_BLE_AUDIO_SYNC_HANDLE handle,
                                  T_BIG_MGR_SYNC_RECEIVER_BIG_CREATE_SYNC_PARAM *p_param);

/**
 * bap_audio_sync.h
 *
 * \brief  Send terminate ble audio big sync request.
 *
 * \param[in]  handle      BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 *
 * \return         The result of terminate ble audio big sync.
 * \retval true    Send request success.
 * \retval false   Send request failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_big_terminate(T_BLE_AUDIO_SYNC_HANDLE handle);

/**
 * bap_audio_sync.h
 *
 * \brief  Get ble audio bis sync information.
 *
 * \param[in]  handle      BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in,out]  p_info  Pointer to bis sync information: @ref T_BLE_AUDIO_BIS_INFO.
 *
 * \return         The result of get ble audio bis sync information.
 * \retval true    Get ble audio bis sync information success.
 * \retval false   Get ble audio bis sync information failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_get_bis_sync_info(T_BLE_AUDIO_SYNC_HANDLE handle, T_BLE_AUDIO_BIS_INFO *p_info);

/**
 * bap_audio_sync.h
 *
 * \brief  Setup ble audio bis data path.
 *
 * \param[in]  handle            BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  bis_idx           BIS index.
 * \param[in]  codec_id          Codec id.
 * \param[in]  controller_delay  Controller delay in microseconds.
 *                               Range: 0x000000 to 0x3D0900
                                 Time range: 0 s to 4 s
 * \param[in]  codec_len         Length of codec configuration.
 * \param[in]  p_codec_data      Pointer to codec-specific configuration data.
 *
 * \return         The result of setup ble audio bis data path.
 * \retval true    Setup ble audio bis data path success.
 * \retval false   Setup ble audio bis data path failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_bis_setup_data_path(T_BLE_AUDIO_SYNC_HANDLE handle, uint8_t bis_idx,
                                   uint8_t codec_id[5], uint32_t controller_delay,
                                   uint8_t codec_len, uint8_t *p_codec_data);

/**
 * bap_audio_sync.h
 *
 * \brief  Remove ble audio bis data path.
 *
 * \param[in]  handle       BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  bis_idx      BIS index.
 *
 * \return         The result of remove ble audio bis data path.
 * \retval true    Remove ble audio bis data path success.
 * \retval false   Remove ble audio bis data path failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_bis_remove_data_path(T_BLE_AUDIO_SYNC_HANDLE handle, uint8_t bis_idx);
/**
 * End of BAP_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
