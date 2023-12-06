/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CSIS_CLIENT_H_
#define _CSIS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "csis_rsi.h"
#include "csis_def.h"
#include "ble_audio_group.h"

/**
 * \defgroup    LEA_GAF_CSIP_Client Coordinated Set Identification Client
 *
 * \brief   The client role for Coordinated Set Identification Profile.
 */

/**
 * \defgroup    CSIP_Client_Exported_Macros Coordinated Set Identification Client Exported Macros
 *
 * \ingroup LEA_GAF_CSIP_Client
 * \{
 */

/**
 * csis_client.h
 *
 * \defgroup    CSIS_CHAR_FLAG CSIS Characteristic Flag
 *
 * \brief  Define CSIS Characteristic Flags.
 *
 * \ingroup CSIP_Client_Exported_Macros
 * \{
 */
#define CSIS_LOCK_FLAG 0x01
#define CSIS_SIRK_FLAG 0x02
#define CSIS_SIZE_FLAG 0x04
#define CSIS_RANK_FLAG 0x08
/**
 * End of CSIS_CHAR_FLAG
 * \}
 */

/**
 * End of CSIP_Client_Exported_Macros
 * \}
 */

/**
 * \defgroup    CSIP_Client_Exported_Types Coordinated Set Identification Client Exported Types
 *
 * \ingroup LEA_GAF_CSIP_Client
 * \{
 */

/**
 * csis_client.h
 *
 * \brief  Set Coordinator Lock State Event Types.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef enum
{
    SET_COORDINATOR_EVENT_INIT              = 0x00,
    SET_COORDINATOR_EVENT_LOCK_REQ          = 0x01,
    SET_COORDINATOR_EVENT_UNLOCK_REQ        = 0x02,
    SET_COORDINATOR_EVENT_LOCK_NOTIFY       = 0x03,
    SET_COORDINATOR_EVENT_MEM_ADD           = 0x04,
    SET_COORDINATOR_EVENT_MEM_DISCONN       = 0x05,
    /**< Error Event. */
    SET_COORDINATOR_EVENT_LOCK_REQ_FAILED   = 0x10,
    SET_COORDINATOR_EVENT_UNLOCK_REQ_FAILED = 0x11,
    SET_COORDINATOR_EVENT_LOCK_DENIED       = 0x12,
} T_SET_COORDINATOR_EVENT;

/**
 * csis_client.h
 *
 * \brief  Set Coordinator Lock State Types.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef enum
{
    SET_COORDINATOR_LOCK_UNKNOWN     = 0x00,
    SET_COORDINATOR_LOCK_NO_CONN     = 0x01,
    SET_COORDINATOR_LOCK_NOT_SUPPORT = 0x02,
    SET_COORDINATOR_UNLOCK           = 0x03,
    SET_COORDINATOR_WAIT_FOR_LOCK    = 0x04,
    SET_COORDINATOR_LOCK_GRANTED     = 0x05,
    SET_COORDINATOR_WAIT_FOR_UNLOCK  = 0x06,
    SET_COORDINATOR_LOCK_DENIED      = 0x07,
} T_SET_COORDINATOR_LOCK;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Search Set Member Timeout.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_TIMEOUT.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    uint8_t         set_mem_size;
    bool            search_done;
} T_CSIS_CLIENT_SEARCH_TIMEOUT;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Search Set Member Result.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_DONE.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    uint8_t         set_mem_size;
    bool            search_done;
} T_CSIS_CLIENT_SEARCH_DONE;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Find Set Member Parameter.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_SET_MEM_FOUND.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    T_BLE_AUDIO_DEV_HANDLE   dev_handle;
    uint8_t                 bd_addr[6];
    uint8_t                 addr_type;
    uint16_t                srv_uuid;
    uint8_t                 rank;
    uint8_t                 set_mem_size;
    uint8_t                 sirk[CSIS_SIRK_LEN];
} T_CSIS_CLIENT_SET_MEM_FOUND;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Discovery CSIS Service Result.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;       /**< Whether to find service. */
    bool    load_from_ftl;  /**< Whether the service table is loaded from FTL. */
    uint8_t srv_num;        /**< Service instance number. */
} T_CSIS_CLIENT_DIS_DONE;

/**
 * csis_client.h
 *
 * \brief  Coordinator Lock State Change Information.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_LOCK_STATE.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    T_SET_COORDINATOR_EVENT  event;
    T_SET_COORDINATOR_LOCK   lock_state;
} T_CSIS_CLIENT_LOCK_STATE;

/**
 * csis_client.h
 *
 * \brief  Set Member Information.
 *         The parameters for set member.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    uint8_t                 bd_addr[6];
    uint8_t                 addr_type;
    uint16_t                srv_uuid;
    uint8_t                 srv_instance_id;   /**< Service instance id. Used when the
                                                number of services is greater than 1. */
    uint8_t                 char_exit;
    uint8_t                 rank;
    T_CSIS_LOCK             lock;
    uint8_t                 set_mem_size;
    T_CSIS_SIRK_TYPE        sirk_type;
    uint8_t                 sirk[CSIS_SIRK_LEN];
} T_CSIS_SET_MEM_INFO;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Read Characteristic Value Result.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    uint16_t                 cause;
    uint16_t                 conn_handle;
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    T_BLE_AUDIO_DEV_HANDLE   dev_handle;
    T_CSIS_SET_MEM_INFO      mem_info;
} T_CSIS_CLIENT_READ_RESULT;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Receive SIRK Change Notify.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_SIRK_NOTIFY.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    T_CSIS_SIRK_TYPE         sirk_type;
    uint8_t                  sirk[CSIS_SIRK_LEN];
} T_CSIS_CLIENT_SIRK_NOTIFY;

/**
 * csis_client.h
 *
 * \brief  CSIS Client Receive Size Change Notify.
 *         The message data for LE_AUDIO_MSG_CSIS_CLIENT_SIZE_NOTIFY.
 *
 * Note: When the Set Coordinator receives a notification of the Coordinated Set Size characteristic,
 * the Set Coordinator should execute the Set Member Discovery procedure.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    uint8_t                  old_size;
    uint8_t                  new_size;
} T_CSIS_CLIENT_SIZE_NOTIFY;

/**
 * csis_client.h
 *
 * \brief  Coordinator Set Group Information.
 *
 * \ingroup CSIP_Client_Exported_Types
 */
typedef struct
{
    uint16_t srv_uuid;
    uint8_t  char_exit;
    uint8_t  set_mem_size;
    T_CSIS_SIRK_TYPE sirk_type;
    uint8_t  sirk[CSIS_SIRK_LEN];
} T_CSIS_GROUP_INFO;
/**
 * End of CSIP_Client_Exported_Types
 * \}
 */

/**
 * \defgroup    CSIP_Client_Exported_Functions Coordinated Set Identification Client Exported Functions
 *
 * \ingroup LEA_GAF_CSIP_Client
 * \{
 */

/**
 * csis_client.h
 *
 * \brief  CSIS client check advertising RSI.
 *
 * \param[in]  report_data_len  Advertising report data length.
 * \param[in]  p_report_data    Pointer to advertising report data.
 * \param[in]  bd_addr        Pointer to Address.
 * \param[in]  addr_type        Address type.
 *
 * \return         The result of CSIS client check advertising RSI.
 * \retval true    CSIS client check advertising RSI success.
 * \retval false   CSIS client check advertising RSI failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_check_adv_rsi(uint8_t report_data_len, uint8_t *p_report_data,
                                   uint8_t *bd_addr, uint8_t addr_type);

/**
 * csis_client.h
 *
 * \brief  Set coordinator configure discover mode.
 *
 * \param[in]  group_handle   BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  discover       The status for discover set member.
 * \arg    true    set_coordinator is discovering set member.
 * \arg    false   set_coordinator is not discovering set member.
 * \param[in]  timeout_ms     Discover timeout. Unit: ms.
 *
 * \return         The result of set coordinator configure discover mode.
 * \retval true    Set coordinator configure discover mode success.
 * \retval false   Set coordinator configure discover mode failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_cfg_discover(T_BLE_AUDIO_GROUP_HANDLE group_handle, bool discover,
                                  uint32_t timeout_ms);

/**
 * csis_client.h
 *
 * \brief  Set coordinator add ble audio group.
 *         When ble audio group is successfully added, the LE_AUDIO_MSG_CAP_SET_MEM_ADD
 *         will be send to Application.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in,out]  p_group_handle   Pointer to ble audio group: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]      p_fun_cb         BLE audio group callback function: @ref P_FUN_AUDIO_GROUP_CB.
 * \param[in,out]  p_dev_handle     Pointer to ble audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \param[in]      p_mem_info       Pointer to set member information: @ref T_CSIS_SET_MEM_INFO.
 *
 * \return         The result of set coordinator add ble audio group.
 * \retval true    Set coordinator add ble audio group success.
 * \retval false   Set coordinator add ble audio group failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_add_group(T_BLE_AUDIO_GROUP_HANDLE *p_group_handle,
                               P_FUN_AUDIO_GROUP_CB p_fun_cb,
                               T_BLE_AUDIO_DEV_HANDLE *p_dev_handle, T_CSIS_SET_MEM_INFO *p_mem_info);

/**
 * csis_client.h
 *
 * \brief  Set coordinator add device.
 *         When the device is successfully added, the LE_AUDIO_MSG_CAP_SET_MEM_ADD
 *         will be send to Application.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  p_group_handle   Pointer to ble audio group: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  p_dev_handle     Pointer to ble audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \param[in]  p_mem_info       Pointer to set member information: @ref T_CSIS_SET_MEM_INFO.
 *
 * \return         The result of set coordinator add device.
 * \retval true    Set coordinator add device success.
 * \retval false   Set coordinator add device failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_add_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                             T_BLE_AUDIO_DEV_HANDLE *p_dev_handle,
                             T_CSIS_SET_MEM_INFO *p_mem_info);

/**
 * csis_client.h
 *
 * \brief  CSIS client write coordinator set members lock.
 *
 * \param[in]  group_handle      BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 *
 * \return         The result of CSIS client write coordinator set members lock.
 * \retval true    CSIS client write coordinator set members lock success.
 * \retval false   CSIS client write coordinator set members lock failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_write_lock(T_BLE_AUDIO_GROUP_HANDLE group_handle);

/**
 * csis_client.h
 *
 * \brief  CSIS client write coordinator set members unlock.
 *
 * \param[in]  group_handle      BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 *
 * \return         The result of CSIS client write coordinator set members unlock.
 * \retval true    CSIS client write coordinator set members unlock success.
 * \retval false   CSIS client write coordinator set members unlock failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_write_unlock(T_BLE_AUDIO_GROUP_HANDLE group_handle);

/**
 * csis_client.h
 *
 * \brief  Set coordinator get lock state.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  group_handle   BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  p_lock         Pointer to set coordinator lock state: @ref T_SET_COORDINATOR_LOCK.
 *
 * \return         The result of set coordinator get lock state.
 * \retval true    Set coordinator get lock state success.
 * \retval false   Set coordinator get lock state failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_get_lock_state(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                    T_SET_COORDINATOR_LOCK *p_lock);

/**
 * csis_client.h
 *
 * \brief  Find set coordinator by rsi.
 *
 * \param[in]  p_rsik      Pointer to rsi.
 *
 * \return         BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \retval NULL    Not find set coordinator by rsi.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
T_BLE_AUDIO_GROUP_HANDLE set_coordinator_find_by_rsi(uint8_t *p_rsi);

/**
 * csis_client.h
 *
 * \brief  Find set coordinator by address.
 *
 * \param[in]  bd_addr            Pointer to address.
 * \param[in]  addr_type          Address type: @ref T_GAP_REMOTE_ADDR_TYPE.
 * \param[in]  serv_uuid          Service uuid.
 * \param[in,out]  p_dev_handle   Pointer to ble audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 *
 * \return         BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \retval NULL    Not find set coordinator by sirk.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
T_BLE_AUDIO_GROUP_HANDLE set_coordinator_find_by_addr(uint8_t *bd_addr, uint8_t addr_type,
                                                      uint16_t srv_uuid,
                                                      T_BLE_AUDIO_DEV_HANDLE *p_dev_handle);

/**
 * csis_client.h
 *
 * \brief  Set coordinator get CSIS group information.
 *
 * \param[in]  group_handle    BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in,out]  p_info      Pointer to CSIS group information: @ref T_CSIS_GROUP_INFO.
 *
 * \return         The result of set coordinator get CSIS group information.
 * \retval true    Set coordinator get CSIS group information success.
 * \retval false   Set coordinator get CSIS group information failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_get_group_info(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                    T_CSIS_GROUP_INFO *p_info);

/**
 * csis_client.h
 *
 * \brief  Set coordinator get set member information.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  group_handle    BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  dev_handle      BLE audio device handle: @ref T_BLE_AUDIO_DEV_HANDLE.
 * \param[in,out]  p_info      Pointer to set member information: @ref T_CSIS_SET_MEM_INFO.
 *
 * \return         The result of set coordinator get set member information.
 * \retval true    Set coordinator get set member information success.
 * \retval false   Set coordinator get set member information failed.
 *
 * \ingroup CSIP_Client_Exported_Functions
 */
bool set_coordinator_get_mem_info(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                  T_BLE_AUDIO_DEV_HANDLE dev_handle,
                                  T_CSIS_SET_MEM_INFO *p_info);
/**
 * End of CSIP_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
