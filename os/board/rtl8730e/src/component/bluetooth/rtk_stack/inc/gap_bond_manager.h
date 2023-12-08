/**
*****************************************************************************************
*     Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_bond_manager.h
  * @brief   This file contains all the functions prototypes for the GAP bond manager
  *          related functions.
  * @details
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_BOND_MANAGER_H
#define GAP_BOND_MANAGER_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"
#include "gap_storage_le.h"

/** @defgroup GAP_BOND_Manager GAP Bond Manager
  * @brief GAP Bond Manager
  * @{
  */

#if F_BT_APP_BOND_MANAGER
/** @defgroup GAP_CB_MSG_MODULE GAP Callback Message
  * @brief GAP Callback Message
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_BOND_MANAGER_Exported_Macros GAP Bond Manager Exported Macros
  * @{
  */

/** @defgroup APP_BOND_MANAGER_MSG_Types APP Bond Manager Msg Types
  * @{
  */
#define GAP_MSG_APP_BOND_MANAGER_INFO          0x80
/** End of APP_BOND_MANAGER_MSG_Types
  * @}
  */

/** @defgroup APP_BOND_MANAGER_MSG_Opcodes APP Bond Manager Msg Opcodes
  * @{
  */
#define APP_BOND_MGR_SIGN_COUNTER_RESULT_INFO                   0x0001   /* no confirm */
#define APP_BOND_MGR_SET_REMOTE_CLIENT_SUPPORTED_FEATURES_INFO  0x0002   /* return value of callback. @ref T_APP_RESULT */
#define APP_BOND_MGR_SET_CCCD_DATA_PENDING_INFO                 0x0003   /* return value of callback. @ref T_APP_RESULT */

#define APP_BOND_MGR_GATT_SERVER_STORE_IND                      0x0020   /* GATT_STORE_OP_GET_CCC_BITS: gap_update_cccd_info; GATT_STORE_OP_SET_CCC_BITS: no confirm */

#define APP_BOND_MGR_GET_REMOTE_CLIENT_SUPPORTED_FEATURES_IND   0x0040   /* gap_acl_update_remote_client_supported_features */

#define APP_BOND_MGR_LE_ACL_STATUS_INFO                         0x0060   /* no confirm */

#define APP_BOND_MGR_LE_AUTHEN_RESULT_IND                       0x0080   /* le_bond_authen_result_confirm */

#define APP_BOND_MGR_LE_AUTHEN_KEY_REQ_IND                      0x00A0  /* le_bond_authen_key_req_confirm */
/** End of APP_BOND_MANAGER_MSG_Opcodes
  * @}
  */

/** End of GAP_BOND_MANAGER_Exported_Macros
* @}
*/

/** End of GAP_CB_MSG_MODULE
* @}
*/
#endif

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_BOND_MANAGER_Exported_Types GAP Bond Manager Exported Types
  * @{
  */

typedef enum
{
    GATT_STORE_OP_GET_CCC_BITS,            /**< get CCC Bits for one <bd_addr,bdtype> */
    GATT_STORE_OP_SET_CCC_BITS,            /**< set CCC Bits for one <bd_addr,bdtype> */
} T_GATT_STORE_OPCODE;

typedef enum
{
    LE_ACL_AUTHEN_START       = 0x03, /**< ACL link authentication start. */
    LE_ACL_AUTHEN_SUCCESS     = 0x04, /**< ACL link authentication success. */
    LE_ACL_AUTHEN_FAIL        = 0x05, /**< ACL link authentication fail. */
    LE_ACL_CONN_ENCRYPTED     = 0x06, /**< ACL link encrypted. */
    LE_ACL_CONN_NOT_ENCRYPTED = 0x08, /**< ACL link not encrypted. */
    LE_ACL_ADDR_RESOLVED      = 0x09, /**< Address is resolved. */
} T_LE_ACL_STATUS;

typedef struct
{
    T_GATT_STORE_OPCODE         op;
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    uint8_t                     bd_addr[6];
    uint8_t                     data_len;
    uint8_t                    *p_data;
    uint16_t                    cccd_handle;
    uint16_t                    cccd_bits;
} T_GATT_SERVER_STORE_IND;

#if F_BT_APP_BOND_MANAGER
typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    bool                        update_local;
    uint32_t                    local_sign_count;
    uint32_t                    remote_sign_count;
} T_SIGN_COUNTER_RESULT_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    uint16_t                    length;
    uint8_t                     *p_client_supp_feats;
} T_SET_REMOTE_CLIENT_SUPPORTED_FEATURES_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    uint16_t                    handle;                /* Attribute Handle */
    bool                        data_pending;
} T_SET_CCCD_DATA_PENDING_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
} T_GET_REMOTE_CLIENT_SUPPORTED_FEATURES_IND;

typedef struct
{
    T_GAP_KEY_TYPE      key_type;       /**< Key type of the link. */
    uint8_t             key_size;       /**< Key size of the link. */
    uint8_t             encrypt_type;   /**< Encryption type of the link. */
    uint16_t            cause;          /**< Authentication result. */
} T_LE_ACL_STATUS_PARAM_AUTHEN;

typedef struct
{
    uint8_t                 bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE  remote_addr_type;
} T_LE_ACL_STATUS_PARAM_RESOLVE;

typedef union
{
    T_LE_ACL_STATUS_PARAM_AUTHEN        authen;       /**< Parameter for authentication status. */
    T_LE_ACL_STATUS_PARAM_RESOLVE       resolve;      /**< Address resolvable paramter. */
} T_LE_ACL_STATUS_PARAM;

typedef struct
{
    uint8_t                     bd_addr[6];         /**< Bluetooth address of remote device. */
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;   /**< Address type of remote device. */
    T_LE_ACL_STATUS             status;             /**< ACL status information type. */
    T_LE_ACL_STATUS_PARAM       p;                  /**< ACL status information parameter. */
} T_LE_ACL_STATUS_INFO;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    uint8_t                     key_len;
    T_GAP_KEY_TYPE              key_type;           /**< Key type. */
    uint8_t                     link_key[28];
    uint16_t                    cause;
} T_LE_AUTHEN_RESULT_IND;

typedef struct
{
    uint8_t                     bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE      remote_addr_type;
    T_GAP_KEY_TYPE              key_type;           /**< Requested Key type. */
} T_LE_AUTHEN_KEY_REQ_IND;

typedef union
{
    T_SIGN_COUNTER_RESULT_INFO                   *p_sign_counter_result_info;
    T_SET_REMOTE_CLIENT_SUPPORTED_FEATURES_INFO  *p_set_remote_client_supported_features_info;
    T_SET_CCCD_DATA_PENDING_INFO                 *p_set_cccd_data_pending_info;

    T_GATT_SERVER_STORE_IND                      *p_gatt_server_store_ind;

    T_GET_REMOTE_CLIENT_SUPPORTED_FEATURES_IND   *p_get_remote_client_supported_features_ind;

    T_LE_ACL_STATUS_INFO                         *p_le_acl_status_info;

    T_LE_AUTHEN_RESULT_IND                       *p_le_authen_result_ind;

    T_LE_AUTHEN_KEY_REQ_IND                      *p_le_authen_key_req_ind;
} T_APP_BOND_MGR_CB_DATA;

typedef struct
{
    uint16_t                   opcode;
    T_APP_BOND_MGR_CB_DATA  data;
} T_APP_BOND_MGR_CB;
#endif
/** End of GAP_BOND_MANAGER_Exported_Types
* @}
*/

#if F_BT_APP_BOND_MANAGER
/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_BOND_MANAGER_Exported_Functions GAP Bond Manager Exported Functions
  * @brief
  * @{
  */

bool gap_get_gap_bond_manager(void);

T_GAP_CAUSE le_bond_authen_result_confirm(uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE remote_addr_type,
                                          T_GAP_KEY_TYPE key_type, T_GAP_CFM_CAUSE cause);

T_GAP_CAUSE le_bond_authen_key_req_confirm(uint8_t *bd_addr,
                                           T_GAP_REMOTE_ADDR_TYPE remote_addr_type,
                                           uint8_t key_len, uint8_t *p_key, T_GAP_KEY_TYPE key_type,
                                           T_GAP_CFM_CAUSE cause);

bool gap_update_cccd_info(uint16_t conn_handle, uint8_t data_len, uint8_t *data);
/** End of GAP_BOND_MANAGER_Exported_Functions
  * @}
  */

/** End of GAP_BOND_Manager
  * @}
  */

#endif
#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_BOND_MANAGER_H */
