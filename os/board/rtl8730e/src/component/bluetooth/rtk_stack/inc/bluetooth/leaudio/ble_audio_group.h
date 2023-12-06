/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BLE_AUDIO_GROUP_H_
#define _BLE_AUDIO_GROUP_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "gap_msg.h"

/**
 * \defgroup    LEA_GAF_BAP BAP Profile
 *
 * \brief   Basic Audio Profile.
 */

/**
 * \defgroup    BAP_Exported_Types BAP Profile Exported Types
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap_audio_group.h
 *
 * \brief  Define type T_BLE_AUDIO_GROUP_HANDLE. BLE Audio Group Handle.
 *
 * \ingroup BAP_Exported_Types
 */
typedef void *T_BLE_AUDIO_GROUP_HANDLE;

/**
 * bap_audio_group.h
 *
 * \brief  Define type T_BLE_AUDIO_DEV_HANDLE. BLE Audio Device Handle.
 *
 * \ingroup BAP_Exported_Types
 */
typedef void *T_BLE_AUDIO_DEV_HANDLE;

/**
 * bap_audio_group.h
 *
 * \brief  BLE Audio Group Message Type.
 *
 * \ingroup BAP_Exported_Types
 */
typedef enum
{
    AUDIO_GROUP_MSG_BAP_STATE                         = 0x01,
    AUDIO_GROUP_MSG_BAP_SESSION_REMOVE                = 0x02,
    AUDIO_GROUP_MSG_BAP_START_QOS_CFG                 = 0x04,
    AUDIO_GROUP_MSG_BAP_CREATE_CIS                    = 0x05,
    AUDIO_GROUP_MSG_BAP_START_METADATA_CFG            = 0x06,
    AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH               = 0x07,
    AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH              = 0x08,
    AUDIO_GROUP_MSG_BAP_METADATA_UPDATE               = 0x09,
    AUDIO_GROUP_MSG_BAP_CIS_DISCONN                   = 0x0A,

    AUDIO_GROUP_MSG_DEV_CONN                          = 0x20,
    AUDIO_GROUP_MSG_DEV_DISCONN                       = 0x21,
    AUDIO_GROUP_MSG_DEV_BOND_REMOVE                   = 0x22,
    AUDIO_GROUP_MSG_DEV_BOND_CLEAR                    = 0x23,
    AUDIO_GROUP_MSG_DEV_EMPTY                         = 0x24,    /**< trigger by ble_audio_group_remove_dev */
} T_AUDIO_GROUP_MSG;

/**
 * bap_audio_group.h
 *
 * \brief  BLE Audio Device Connection Message Data.
 *         The message data for AUDIO_GROUP_MSG_DEV_CONN.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
} T_AUDIO_GROUP_MSG_DEV_CONN;

/**
 * bap_audio_group.h
 *
 * \brief  BLE Audio Device Disconnect Message Data.
 *         The message data for AUDIO_GROUP_MSG_DEV_DISCONN.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
    uint16_t               cause;
} T_AUDIO_GROUP_MSG_DEV_DISCONN;

/**
 * bap_audio_group.h
 *
 * \brief  BLE Audio Device Remove Bond Information Message Data.
 *         The message data for AUDIO_GROUP_MSG_DEV_BOND_REMOVE.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
} T_AUDIO_GROUP_MSG_DEV_BOND_REMOVE;

/**
 * bap_audio_group.h
 *
 * \brief  BLE Audio Device Information.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    bool                   is_used;
    T_GAP_CONN_STATE       conn_state;
    uint16_t               conn_handle;
    T_GAP_REMOTE_ADDR_TYPE addr_type;
    uint8_t                bd_addr[6];
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
} T_AUDIO_DEV_INFO;

/**
 * bap_audio_group.h
 *
 * \brief  P_FUN_GATT_CLIENT_CB Audio Group Callback Function Point Definition.
 *         Function pointer used in ble audio group module, to send events
 *         @ref T_AUDIO_GROUP_MSG to application.
 *
 * \ingroup BAP_Exported_Types
 */
typedef void(*P_FUN_AUDIO_GROUP_CB)(T_AUDIO_GROUP_MSG msg, T_BLE_AUDIO_GROUP_HANDLE handle,
                                    void *buf);
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
 * bap_audio_group.h
 *
 * \brief  Allocate ble audio group.
 *
 * \return         BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \retval NULL    Allocate ble audio group failed.
 *
 * \ingroup BAP_Exported_Functions
 */
T_BLE_AUDIO_GROUP_HANDLE ble_audio_group_allocate(void);

/**
 * bap_audio_group.h
 *
 * \brief  Register ble audio group callback function.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  p_fun_cb          Audio group callback function: @ref P_FUN_AUDIO_GROUP_CB.
 *
 * \return         The result of register ble audio group callback function.
 * \retval true    Register ble audio group callback function success.
 * \retval false   Register ble audio group callback function failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_group_reg_cb(T_BLE_AUDIO_GROUP_HANDLE group_handle, P_FUN_AUDIO_GROUP_CB p_fun_cb);

/**
 * bap_audio_group.h
 *
 * \brief  Release ble audio group.
 *         If this api is successfully invoked, the referred group handle will be cleared.
 *
 * \param[in]  group_handle      Pointer to audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 *
 * \return         The result of release ble audio group.
 * \retval true    Release ble audio group success.
 * \retval false   Release ble audio group failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_group_release(T_BLE_AUDIO_GROUP_HANDLE *p_group_handle);

/**
 * bap_audio_group.h
 *
 * \brief  Add device into ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  bd_addr           Pointer to bluetooth device address.
 * \param[in]  addr_type         Address type.
 *
 * \return         The ble audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \retval NULL    Add device to ble audio group failed.
 *
 * \ingroup BAP_Exported_Functions
 */
T_BLE_AUDIO_DEV_HANDLE ble_audio_group_add_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                               uint8_t *bd_addr, uint8_t addr_type);

/**
 * bap_audio_group.h
 *
 * \brief  Delete device from ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  p_dev_handle      Pointer to device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 *
 * \return         The result of remove device from ble audio group.
 * \retval true    Remove device from ble audio group success.
 * \retval false   Remove device from ble audio group failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_group_del_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                             T_BLE_AUDIO_DEV_HANDLE *p_dev_handle);

/**
 * bap_audio_group.h
 *
 * \brief  Find device from ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  bd_addr           Pointer to bluetooth device address.
 * \param[in]  addr_type         Address type.
 *
 * \return         The ble audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \retval NULL    Find device from ble audio group failed.
 *
 * \ingroup BAP_Exported_Functions
 */
T_BLE_AUDIO_DEV_HANDLE ble_audio_group_find_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                                uint8_t *bd_addr, uint8_t addr_type);

/**
 * bap_audio_group.h
 *
 * \brief  Find device from ble audio group by connection handle.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  conn_handle       Connection handle.
 *
 * \return         The ble audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \retval NULL    Find device from ble audio group by connection handle failed.
 *
 * \ingroup BAP_Exported_Functions
 */
T_BLE_AUDIO_DEV_HANDLE ble_audio_group_find_dev_by_conn_handle(T_BLE_AUDIO_GROUP_HANDLE
                                                               group_handle,
                                                               uint16_t conn_handle);

/**
 * bap_audio_group.h
 *
 * \brief  Get device information from ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  dev_handle        Device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \param[in,out]  p_info        Pointer to audio device information: @ref T_AUDIO_DEV_INFO.
 *
 * \return         The result of get device information.
 * \retval true    Get device information success.
 * \retval false   Get device information failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_group_get_dev_info(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                  T_BLE_AUDIO_DEV_HANDLE dev_handle,
                                  T_AUDIO_DEV_INFO *p_info);

/**
 * bap_audio_group.h
 *
 * \brief  Get used device number from ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  check_conn        Whether to check the connection state is connected.
 * \arg    true    Check the connection state.
 * \arg    false   Not check the connection state.
 *
 * \return         The number of used ble audio device.
 *
 * \ingroup BAP_Exported_Functions
 */


uint8_t ble_audio_group_get_used_dev_num(T_BLE_AUDIO_GROUP_HANDLE group_handle, bool check_conn);

/**
 * bap_audio_group.h
 *
 * \brief  Get device number from ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 *
 * \return         The number of ble audio device.
 *
 * \ingroup BAP_Exported_Functions
 */
uint8_t ble_audio_group_get_dev_num(T_BLE_AUDIO_GROUP_HANDLE group_handle);

/**
 * bap_audio_group.h
 *
 * \brief  Get device information from ble audio group.
 *
 * \param[in]  group_handle      Audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in,out]  p_dev_num     Pointer to device number.
 * \param[in,out]  p_dev_tbl     Pointer to device information table: @ref T_AUDIO_DEV_INFO.
 *
 * \return         The result of get device information.
 * \retval true    Get device information success.
 * \retval false   Get device information failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool ble_audio_group_get_info(T_BLE_AUDIO_GROUP_HANDLE group_handle, uint8_t *p_dev_num,
                              T_AUDIO_DEV_INFO *p_dev_tbl);
/**
 * End of BAP_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
