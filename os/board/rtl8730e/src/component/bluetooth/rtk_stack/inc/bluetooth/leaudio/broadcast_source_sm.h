/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BROADCAST_SOURCE_SM_H_
#define _BROADCAST_SOURCE_SM_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "gap_pa_adv.h"
#include "gap_big_mgr.h"
#include "ble_audio_def.h"

/**
 * \defgroup    LEA_GAF_Broadcast_Source_SM   Broadcast Source State Machine
 *
 * \brief   Broadcast Source State Machine.
 */

/**
 * \defgroup    Broadcast_Source_SM_Exported_Macros Broadcast Source State Machine Exported Macros
 *
 * \ingroup LEA_GAF_Broadcast_Source_SM
 * \{
 */

/**
 * broadcast_source_sm.h
 *
 * \brief  Define Type T_BROADCAST_SOURCE_HANDLE. Broadcast Source Handle.
 *
 * \ingroup Broadcast_Source_SM_Exported_Macros
 */
typedef void *T_BROADCAST_SOURCE_HANDLE;

/**
 * bap.h
 *
 * \defgroup   BROADCAST_SOURCE_CB_MSG Broadcast Source Callback Message
 *
 * \brief  Define Broadcast Source Callback Message.
 *
 * \ingroup Broadcast_Source_SM_Exported_Macros
 * \{
 */
#define MSG_BROADCAST_SOURCE_STATE_CHANGE         0x01
#define MSG_BROADCAST_SOURCE_METADATA_UPDATE      0x02
#define MSG_BROADCAST_SOURCE_RECONFIG             0x03
#define MSG_BROADCAST_SOURCE_SETUP_DATA_PATH      0x04
#define MSG_BROADCAST_SOURCE_REMOVE_DATA_PATH     0x05
#define MSG_BROADCAST_SOURCE_BIG_SYNC_MODE        0x06
/**
 * End of BROADCAST_SOURCE_CB_MSG
 * \}
 */

/**
 * End of Broadcast_Source_SM_Exported_Macros
 * \}
 */

/**
 * \defgroup    Broadcast_Source_SM_Exported_Types Broadcast Source State Machine Exported Types
 *
 * \ingroup LEA_GAF_Broadcast_Source_SM
 * \{
 */

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source State Types.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef enum
{
    BROADCAST_SOURCE_STATE_IDLE                = 0x00,
    BROADCAST_SOURCE_STATE_CONFIGURED_STARTING = 0x01,
    BROADCAST_SOURCE_STATE_CONFIGURED          = 0x02,
    BROADCAST_SOURCE_STATE_CONFIGURED_STOPPING = 0x03,
    BROADCAST_SOURCE_STATE_STREAMING_STARTING  = 0x04,
    BROADCAST_SOURCE_STATE_STREAMING           = 0x05,
    BROADCAST_SOURCE_STATE_STREAMING_STOPPING  = 0x06,
} T_BROADCAST_SOURCE_STATE;

/**
 * broadcast_source_sm.h
 *
 * \brief  Extended advertising state.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef enum
{
    BLE_EXT_ADV_STATE_IDLE,         /**< Idle, no advertising. */
    BLE_EXT_ADV_STATE_START,        /**< Start Advertising. A temporary state, haven't received the result. */
    BLE_EXT_ADV_STATE_ADVERTISING,  /**< Advertising. */
    BLE_EXT_ADV_STATE_STOP,         /**< Stop Advertising. A temporary state, haven't received the result. */
} T_BLE_EXT_ADV_STATE;

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source State Change Parameters.
 *         The callback message data for MSG_BROADCAST_SOURCE_STATE_CHANGE.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef struct
{
    T_BROADCAST_SOURCE_STATE state;
    uint16_t cause;
} T_BROADCAST_SOURCE_STATE_CHANGE;

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source BIG Sync Mode Parameters.
 *         The callback message data for MSG_BROADCAST_SOURCE_BIG_SYNC_MODE
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef struct
{
    bool big_sync_mode;
    uint16_t cause;
} T_BROADCAST_SOURCE_BIG_SYNC_MODE;

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source Setup Data Path Parameters.
 *         The callback message data for MSG_BROADCAST_SOURCE_SETUP_DATA_PATH.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef struct
{
    uint8_t bis_idx;
    uint16_t bis_conn_handle;
    uint16_t cause;
} T_BROADCAST_SOURCE_SETUP_DATA_PATH;

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source Remove Data Path Parameters.
 *         The callback message data for MSG_BROADCAST_SOURCE_REMOVE_DATA_PATH.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef struct
{
    uint16_t bis_conn_handle;
    uint16_t cause;
} T_BROADCAST_SOURCE_REMOVE_DATA_PATH;

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source State Machine Callback Data.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef union
{
    uint16_t cause;
    T_BROADCAST_SOURCE_STATE_CHANGE *p_state_change;
    T_BROADCAST_SOURCE_SETUP_DATA_PATH *p_setup_data_path;
    T_BROADCAST_SOURCE_REMOVE_DATA_PATH *p_remove_data_path;
    T_BROADCAST_SOURCE_BIG_SYNC_MODE *p_big_sync_mode;
} T_BROADCAST_SOURCE_SM_CB_DATA;

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast Source Information.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef struct
{
    T_BROADCAST_SOURCE_STATE state;
    uint8_t adv_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    T_BLE_EXT_ADV_STATE ext_adv_state;
    T_GAP_PA_ADV_STATE pa_state;
    T_GAP_BIG_ISOC_BROADCAST_STATE big_state;
    uint8_t adv_handle;
    uint8_t big_handle;
    T_GAP_LOCAL_ADDR_TYPE own_address_type;
} T_BROADCAST_SOURCE_INFO;

/**
 * broadcast_source_sm.h
 *
 * \brief  P_FUN_BROADCAST_SOURCE_SM_CB Broadcast Source State Machine Callback Function Definition.
 *         Function pointer is used to send broadcast source state machine callback message
 *         @ref BROADCAST_SOURCE_CB_MSG to application.
 *
 * \ingroup Broadcast_Source_SM_Exported_Types
 */
typedef void(*P_FUN_BROADCAST_SOURCE_SM_CB)(T_BROADCAST_SOURCE_HANDLE handle,
                                            uint8_t cb_type,
                                            void *p_cb_data);
/**
 * End of Broadcast_Source_SM_Exported_Types
 * \}
 */

/**
 * \defgroup    Broadcast_Source_SM_Exported_Functions Broadcast Source State Machine Exported Functions
 *
 * \ingroup LEA_GAF_Broadcast_Source_SM
 * \{
 */

/**
 * broadcast_source_sm.h
 *
 * \brief  Add broadcast source.
 *
 * \param[in]  cb_pfn      Broadcast source state machine callback function:
 *                         @ref P_FUN_BROADCAST_SOURCE_SM_CB.
 *
 * \return         Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \retval NULL    Add broadcast source failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
T_BROADCAST_SOURCE_HANDLE broadcast_source_add(P_FUN_BROADCAST_SOURCE_SM_CB cb_pfn);

/**
 * broadcast_source_sm.h
 *
 * \brief  Update broadcast id of broadcast source.
 *
 * \xrefitem Added_API_2_12_0_0 "Added Since 2.12.0.0" "Added API"
 *
 * \param[in]  handle            Broadcast source handle.
 * \param[in]  broadcast_id      Broadcast id.
 *
 * \return         The result of updating broadcast id of broadcast source.
 * \retval true    Update broadcast id of broadcast source success.
 * \retval false   Update broadcast id of broadcast source failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_update_broadcast_id(T_BROADCAST_SOURCE_HANDLE handle,
                                          uint8_t broadcast_id[BROADCAST_ID_LEN]);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source set extended advertising parameters.
 *
 * \param[in]  handle                     Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  adv_sid                    Value of the Advertising SID subfield in the ADI field of the PDU
 *                                        Range: 0x00 to 0x0F.
 * \param[in]  primary_adv_interval_min   Minimum advertising interval for undirected and low duty cycle
                                          directed advertising.
                                          Range: 0x000020 to 0xFFFFFF.
                                          Time = N * 0.625 ms, Time Range: 20 ms to 10,485.759375 s.
 * \param[in]  primary_adv_interval_max   Maximum advertising interval for undirected and low duty cycle
                                          directed advertising.
                                          Range: 0x000020 to 0xFFFFFF
                                          Time = N * 0.625 ms, Time Range: 20 ms to 10,485.759375 s.
 * \param[in]  primary_adv_channel_map    Primary advertising channel map: @ref ADV_CHANNEL_MAP.
 * \param[in]  own_address_type           Own address type: @ref T_GAP_LOCAL_ADDR_TYPE.
 * \param[in]  p_local_rand_addr          Pointer to local random device address.
 * \param[in]  filter_policy              Advertising filter policy: @ref T_GAP_ADV_FILTER_POLICY.
 * \param[in]  tx_power                   Advertising tx power. Range: -127 to +20, Units: dBm.
 * \param[in]  primary_adv_phy            Primary advertising phy: @ref T_GAP_PHYS_PRIM_ADV_TYPE.
 * \param[in]  secondary_adv_max_skip     Maximum advertising events the Controller can skip on
 *                                        the secondary advertising phy channel.
 * \param[in]  secondary_adv_phy          Secondary advertising phy: @ref T_GAP_PHYS_TYPE.
 * \param[in]  broadcast_data_len         Length of broadcast data.
 * \param[in]  p_broadcast_data           Pointer to broadcast data.
 *
 * \return         The result of broadcast source set extended advertising parameters.
 * \retval true    Broadcast source set extended advertising parameters success.
 * \retval false   Broadcast source set extended advertising parameters failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_set_eadv_param(T_BROADCAST_SOURCE_HANDLE handle, uint8_t adv_sid,
                                     uint32_t primary_adv_interval_min, uint32_t primary_adv_interval_max,
                                     uint8_t primary_adv_channel_map,
                                     T_GAP_LOCAL_ADDR_TYPE own_address_type, uint8_t *p_local_rand_addr,
                                     T_GAP_ADV_FILTER_POLICY filter_policy, uint8_t tx_power,
                                     T_GAP_PHYS_PRIM_ADV_TYPE primary_adv_phy, uint8_t secondary_adv_max_skip,
                                     T_GAP_PHYS_TYPE secondary_adv_phy,
                                     uint8_t broadcast_data_len, uint8_t *p_broadcast_data);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source set periodic advertising parameters.
 *
 * \param[in]  handle                      Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  periodic_adv_interval_min   Minimum advertising interval for periodic advertising.
 *                                         Range: 0x0006 to 0xFFFF.
 *                                         Time = N * 1.25 ms, Time Range: 7.5 ms to 81.91875 s.
 * \param[in]  periodic_adv_interval_max   Maximum advertising interval for periodic advertising.
 *                                         Range: 0x0006 to 0xFFFF.
 *                                         Time = N * 1.25 ms, Time Range: 7.5 ms to 81.91875 s.
 * \param[in]  periodic_adv_prop           Periodic advertising properties.
 * \param[in]  basic_data_len              Length of basic data.
 * \param[in]  p_basic_data                Pointer to basic data.
 *
 * \return         The result of broadcast source set periodic advertising parameters.
 * \retval true    Broadcast source set periodic advertising parameters success.
 * \retval false   Broadcast source set periodic advertising parameters failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_set_pa_param(T_BROADCAST_SOURCE_HANDLE handle,
                                   uint16_t periodic_adv_interval_min,
                                   uint16_t periodic_adv_interval_max, uint16_t periodic_adv_prop,
                                   uint8_t basic_data_len, uint8_t *p_basic_data);

/**
 * broadcast_source_sm.h
 *
 * \brief  Get broadcast source information.
 *
 * \param[in]  handle       Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in, out]  p_info  Pointer to broadcast source information: @ref T_BROADCAST_SOURCE_INFO.
 *
 * \return         The result of getting broadcast source information.
 * \retval true    Get broadcast source information success.
 * \retval false   Get broadcast source information failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_get_info(T_BROADCAST_SOURCE_HANDLE handle, T_BROADCAST_SOURCE_INFO *p_info);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source action stream configuration procedure.
 *
 * \param[in]  handle      Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 *
 * \return         The result of broadcast source action stream configuration procedure.
 * \retval true    Broadcast source action stream configuration procedure success.
 * \retval false   Broadcast source action stream configuration procedure failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_config(T_BROADCAST_SOURCE_HANDLE handle);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source action stream reconfiguration procedure.
 *
 * \param[in]  handle          Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  basic_data_len  Length of basic data.
 * \param[in]  p_basic_data    Pointer to basic data.
 *
 * \return         The result of broadcast source action stream reconfiguration procedure.
 * \retval true    Broadcast source action stream reconfiguration procedure success.
 * \retval false   Broadcast source action stream reconfiguration procedure failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_reconfig(T_BROADCAST_SOURCE_HANDLE handle, uint8_t basic_data_len,
                               uint8_t *p_basic_data);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source action PA data update procedure.
 *
 * \param[in]  handle          Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  pa_data_len     PA data length.
 * \param[in]  p_pa_data       Pointer to PA data.
 *
 * \return         The result of broadcast source action PA data update procedure.
 * \retval true    Broadcast source action PA data update procedure success.
 * \retval false   Broadcast source action PA data update procedure failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_pa_update(T_BROADCAST_SOURCE_HANDLE handle, uint8_t pa_data_len,
                                uint8_t *p_pa_data);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source action stream establishment procedure.
 *
 * \param[in]  handle            Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  create_big_param  Parameter for create big: @ref T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM.
 *
 * \return         The result of broadcast source action stream establishment procedure.
 * \retval true    Broadcast source action stream establishment procedure success.
 * \retval false   Broadcast source action stream establishment procedure failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_establish(T_BROADCAST_SOURCE_HANDLE handle,
                                T_BIG_MGR_ISOC_BROADCASTER_CREATE_BIG_PARAM create_big_param);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source action stream disable procedure.
 *
 * \param[in]  handle      Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  reason      The reason of stream disable: @ref BT_HCI_ERROR.
 *
 * \return         The result of broadcast source action stream disable procedure.
 * \retval true    Broadcast source action stream disable procedure success.
 * \retval false   Broadcast source action stream disable procedure failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_disable(T_BROADCAST_SOURCE_HANDLE handle, uint8_t reason);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source action stream release procedure.
 *
 * \param[in]  handle      Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 *
 * \return         The result of broadcast source action stream release procedure.
 * \retval true    Broadcast source action stream release procedure success.
 * \retval false   Broadcast source action stream release procedure failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_release(T_BROADCAST_SOURCE_HANDLE handle);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source configure big sync mode.
 *
 * \param[in]  handle         Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  big_sync_mode  Whether to set big sync mode.
 * \arg    true    Set big sync mode.
 * \arg    false   Not set big sync mode.
 *
 * \return         The result of broadcast source configure big sync mode.
 * \retval true    Broadcast source configure big sync mode success.
 * \retval false   Broadcast source configure big sync mode failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_big_sync_mode(T_BROADCAST_SOURCE_HANDLE handle, bool big_sync_mode);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source setup data path.
 *
 * \param[in]  handle            Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  bis_idx           Bis index.
 * \param[in]  codec_id          Codec id.
 * \param[in]  controller_delay  Controller delay in microseconds.
 *                               Range: 0x000000 to 0x3D0900. Time range: 0 s to 4 s.
 * \param[in]  codec_len         Length of codec configuration.
 * \param[in]  p_codec_data      Pointer to codec configuration data.
 *
 * \return         The result of broadcast source setup data path.
 * \retval true    Broadcast source setup data path success.
 * \retval false   Broadcast source setup data path failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_setup_data_path(T_BROADCAST_SOURCE_HANDLE handle, uint8_t bis_idx,
                                      uint8_t codec_id[5], uint32_t controller_delay,
                                      uint8_t codec_len, uint8_t *p_codec_data);

/**
 * broadcast_source_sm.h
 *
 * \brief  Broadcast source remove data path.
 *
 * \param[in]  handle            Broadcast source handle: @ref T_BROADCAST_SOURCE_HANDLE.
 * \param[in]  bis_idx           Bis index.
 *
 * \return         The result of broadcast source remove data path.
 * \retval true    Broadcast source remove data path success.
 * \retval false   Broadcast source remove data path failed.
 *
 * \ingroup Broadcast_Source_SM_Exported_Functions
 */
bool broadcast_source_remove_data_path(T_BROADCAST_SOURCE_HANDLE handle, uint8_t bis_idx);
/**
 * End of Broadcast_Source_SM_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
