// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDE_MODULES_BK_BLE_TYPES_H_
#define INCLUDE_MODULES_BK_BLE_TYPES_H_

#include <common/bk_typedef.h>
#include <common/bk_err.h>


/**
 * @brief ble APIs Version 1.0
 * @addtogroup bk_ble_api_v1 New ble API group
 * @{
 */


/**
 * @brief hci type enum
 */
typedef enum
{
    BK_BLE_HCI_TYPE_CMD = 1,
    BK_BLE_HCI_TYPE_ACL = 2,
    BK_BLE_HCI_TYPE_SCO = 3,
    BK_BLE_HCI_TYPE_EVT = 4,
} BK_BLE_HCI_TYPE;


/**
 * @brief ble actv type
 */
typedef enum
{
    BLE_ACTV_IDLE,
    BLE_ACTV_ADV_CREATED,
    BLE_ACTV_ADV_STARTED,
    BLE_ACTV_SCAN_CREATED,
    BLE_ACTV_SCAN_STARTED,
    BLE_ACTV_INIT_CREATED,
    BLE_ACTV_PER_SYNC_CREATED,
    BLE_ACTV_PER_SYNC_STARTED,
} ble_actv_state;

/**
 * @brief ble api return enum
 */
typedef bk_err_t ble_err_t;

#define BK_ERR_BLE_SUCCESS               BK_OK                      /**< success */
#define BK_ERR_BLE_FAIL                  BK_FAIL                    /**< fail        */
#define BK_ERR_BLE_NO_MEM                BK_ERR_NO_MEM              /**<  no mem       */
#define BK_ERR_BLE_PROFILE               (BK_ERR_BLE_BASE - 1)      /**<  profile err       */
#define BK_ERR_BLE_CREATE_DB             (BK_ERR_BLE_BASE - 2)      /**< bk_ble_create_db err        */
#define BK_ERR_BLE_CMD_NOT_SUPPORT       (BK_ERR_BLE_BASE - 3)      /**< unknow cmd err        */
#define BK_ERR_BLE_UNKNOW_IDX            (BK_ERR_BLE_BASE - 4)      /**< index err, suchas conn_ind        */
#define BK_ERR_BLE_BLE_STATUS            (BK_ERR_BLE_BASE - 5)      /**<  ble status not match       */
#define BK_ERR_BLE_ADV_DATA              (BK_ERR_BLE_BASE - 6)      /**<  adv data err, such as too long       */
#define BK_ERR_BLE_CMD_RUN               (BK_ERR_BLE_BASE - 7)      /**< cmd run err        */
#define BK_ERR_BLE_INIT_CREATE           (BK_ERR_BLE_BASE - 8)      /**< create init err, such as bk_ble_create_init        */
#define BK_ERR_BLE_INIT_STATE            (BK_ERR_BLE_BASE - 9)      /**<  current init status not match       */
#define BK_ERR_BLE_ATTC_WRITE            (BK_ERR_BLE_BASE - 10)     /**<  att write err       */
#define BK_ERR_BLE_ATTC_WRITE_UNREGISTER (BK_ERR_BLE_BASE - 11)     /**<  att handle is not regist       */




/// max ble adv data len
#define BK_BLE_MAX_ADV_DATA_LEN           (0x1F)

/// BD address length
#define BK_BLE_GAP_BD_ADDR_LEN       (6)
/// Maximal length of the Device Name value
#define BK_BLE_APP_DEVICE_NAME_MAX_LEN      (18)


#define BK_UUID_LEN_16     2    /**< Length of 16-bit UUID in bytes */
#define BK_UUID_LEN_32     4    /**< Length of 32-bit UUID in bytes */
#define BK_UUID_LEN_128    16   /**< Length of 128-bit UUID in bytes */

/**
 * @brief for ble_attm_desc_t, build database perm,
 *
 */

#define BK_BLE_PERM_SET(access, right) \
    (((BK_BLE_PERM_RIGHT_ ## right) << (BK_BLE_ ## access ## _POS)) & (BK_BLE_ ## access ## _MASK))

#define BK_BLE_PERM_GET(perm, access)\
    (((perm) & (BK_BLE_ ## access ## _MASK)) >> (BK_BLE_ ## access ## _POS))



/**
 * @brief normal perm, for BK_BLE_PERM_SET
 * @attention you cant use this direct, use BK_BLE_PERM_SET(RD, ENABLE) instead
 * @brief
 *   15 | 14 | 13 | 12 | 11 | 10 |  9 | 8  |  7 - 6  |  5 - 4  |  3 - 2  |  1 - 0
 * -----|----|----|----|----|----|----|----|---------|---------|---------|---------
 *  EXT | WS | I  | N  | WR | WC | RD | B  |    NP   |    IP   |   WP    |    RP
 *
 * Bit [0-1]  : Read Permission         (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON) \n
 * Bit [2-3]  : Write Permission        (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON) \n
 * Bit [4-5]  : Indication Permission   (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON) \n
 * Bit [6-7]  : Notification Permission (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON) \n
 *
 * Bit [8]    : Broadcast permission   \n
 * Bit [9]    : Read Command accepted  \n
 * Bit [10]   : Write Command accepted \n
 * Bit [11]   : Write Request accepted \n
 * Bit [12]   : Send Notification \n
 * Bit [13]   : Send Indication \n
 * Bit [14]   : Write Signed accepted \n
 * Bit [15]   : Extended properties present \n
 */

typedef enum
{
    /// Read Permission Mask
    BK_BLE_RP_MASK             = 0x0003,
    BK_BLE_RP_POS              = 0,
    /// Write Permission Mask
    BK_BLE_WP_MASK             = 0x000C,
    BK_BLE_WP_POS              = 2,
    /// Indication Access Mask
    BK_BLE_IP_MASK            = 0x0030,
    BK_BLE_IP_POS             = 4,
    /// Notification Access Mask
    BK_BLE_NP_MASK            = 0x00C0,
    BK_BLE_NP_POS             = 6,
    /// Broadcast descriptor present
    BK_BLE_BROADCAST_MASK     = 0x0100,
    BK_BLE_BROADCAST_POS      = 8,
    /// Read Access Mask
    BK_BLE_RD_MASK            = 0x0200,
    BK_BLE_RD_POS             = 9,
    /// Write Command Enabled attribute Mask
    BK_BLE_WRITE_COMMAND_MASK = 0x0400,
    BK_BLE_WRITE_COMMAND_POS  = 10,
    /// Write Request Enabled attribute Mask
    BK_BLE_WRITE_REQ_MASK     = 0x0800,
    BK_BLE_WRITE_REQ_POS      = 11,
    /// Notification Access Mask
    BK_BLE_NTF_MASK           = 0x1000,
    BK_BLE_NTF_POS            = 12,
    /// Indication Access Mask
    BK_BLE_IND_MASK           = 0x2000,
    BK_BLE_IND_POS            = 13,
    /// Write Signed Enabled attribute Mask
    BK_BLE_WRITE_SIGNED_MASK  = 0x4000,
    BK_BLE_WRITE_SIGNED_POS   = 14,
    /// Extended properties descriptor present
    BK_BLE_EXT_MASK           = 0x8000,
    BK_BLE_EXT_POS            = 15,
} bk_ble_perm_mask;


/**
 * @brief Attribute Extended permissions, for BK_BLE_PERM_SET
 * @attention you cant use this direct, use BK_BLE_PERM_SET(EKS, ENABLE) instead
 * @brief
 *
 * Extended Value permission bit field
 *
 *   15  |  14 - 13  |  12  |  11  |  10 - 0
 * ------|-----------|------|------|----------
 *   RI  |  UUID_LEN |  EKS | INCL |  Reserved
 *
 * Bit [0-10] : Reserved \n
 * Bit [11]   : Attribute Value is included(Value present in Database) \n
 * Bit [12]   : Encryption key Size must be 16 bytes \n
 * Bit [13-14]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU) \n
 * Bit [15]   : Trigger Read Indication (0 = Value present in Database, 1 = Value not present in Database) \n
 */
typedef enum
{
    /// Attribute Value is included
    BK_BLE_VALUE_INCL_MASK  = 0x800,
    BK_BLE_VALUE_INCL_POS   = 11,
    /// Check Encryption key size Mask
    BK_BLE_EKS_MASK         = 0x1000,
    BK_BLE_EKS_POS          = 12,
    /// UUID Length
    BK_BLE_UUID_LEN_MASK    = 0x6000,
    BK_BLE_UUID_LEN_POS     = 13,
    /// Read trigger Indication
    BK_BLE_RI_MASK          = 0x8000,
    BK_BLE_RI_POS           = 15,
} bk_ble_ext_perm_mask;


/**
 * @brief Service permissions, for BK_BLE_PERM_SET
 * @attention you cant use this direct, use BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16) instead
 * @brief
 *
 *  7  |  6 - 5   |  4  | 3 - 2  |  1  |  0
 * ----|----------|-----|--------|-----|-----
 * SEC | UUID_LEN | DIS |  AUTH  | EKS | MI
 *
 * Bit [0]  : Task that manage service is multi-instantiated (Connection index is conveyed) \n
 * Bit [1]  : Encryption key Size must be 16 bytes \n
 * Bit [2-3]: Service Permission      (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = Secure Connect) \n
 * Bit [4]  : Disable the service \n
 * Bit [5-6]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU) \n
 * Bit [7]  : Secondary Service       (0 = Primary Service, 1 = Secondary Service) \n
 */
typedef enum
{
    /// Task that manage service is multi-instantiated
    BK_BLE_SVC_MI_MASK        = 0x01,
    BK_BLE_SVC_MI_POS         = 0,
    /// Check Encryption key size for service Access
    BK_BLE_SVC_EKS_MASK       = 0x02,
    BK_BLE_SVC_EKS_POS        = 1,
    /// Service Permission authentication
    BK_BLE_SVC_AUTH_MASK      = 0x0C,
    BK_BLE_SVC_AUTH_POS       = 2,
    /// Disable the service
    BK_BLE_SVC_DIS_MASK       = 0x10,
    BK_BLE_SVC_DIS_POS        = 4,
    /// Service UUID Length
    BK_BLE_SVC_UUID_LEN_MASK  = 0x60,
    BK_BLE_SVC_UUID_LEN_POS   = 5,
    /// Service type Secondary
    BK_BLE_SVC_SECONDARY_MASK = 0x80,
    BK_BLE_SVC_SECONDARY_POS  = 7,
} bk_ble_svc_perm_mask;


/*
 * @brief Attribute & Service access mode, for BK_BLE_PERM_SET
 * @attention you cant use this direct, use BK_BLE_PERM_SET(RD, ENABLE) instead
 *
 */

enum
{
    /// Disable access
    BK_BLE_PERM_RIGHT_DISABLE   = 0,
    /// Enable access
    BK_BLE_PERM_RIGHT_ENABLE   = 1,
};


/*
 * @brief Attribute & Service access rights, for BK_BLE_PERM_SET
 * @attention you cant use this direct, use BK_BLE_PERM_SET(SVC_AUTH, UNAUTH) instead
 *
 */
enum
{
    /// No Authentication
    BK_BLE_PERM_RIGHT_NO_AUTH  = 0,
    /// Access Requires Unauthenticated link
    BK_BLE_PERM_RIGHT_UNAUTH   = 1,
    /// Access Requires Authenticated link
    BK_BLE_PERM_RIGHT_AUTH     = 2,
    /// Access Requires Secure Connection link
    BK_BLE_PERM_RIGHT_SEC_CON  = 3,
};

/*
 * @brief Attribute & Service UUID Length, for BK_BLE_PERM_SET
 * @attention you cant use this direct, use BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16) instead
 *
 */
enum
{
    /// 16  bits UUID
    BK_BLE_PERM_RIGHT_UUID_16         = 0,
    /// 32  bits UUID
    BK_BLE_PERM_RIGHT_UUID_32         = 1,
    /// 128 bits UUID
    BK_BLE_PERM_RIGHT_UUID_128        = 2,
    /// Invalid
    BK_BLE_PERM_RIGHT_UUID_RFU        = 3,
};

/*
 * @brief for ble api async call result in ble_at_cmd_cb's cmd
 */
typedef enum
{
    BLE_CMD_NONE,
    /// ADV_CMD:FOR BLE 5.1
    BLE_CREATE_ADV,
    BLE_MODIFY_ADV,
    BLE_SET_ADV_DATA,
    BLE_SET_RSP_DATA,
    BLE_START_ADV,
    BLE_STOP_ADV,
    BLE_DELETE_ADV,
    /// ADV_CMD:FOR BLE 4.2
    BLE_INIT_ADV,
    BLE_DEINIT_ADV,
    /// SCAN_CMD:FOR BLE 5.1
    BLE_CREATE_SCAN,
    BLE_START_SCAN,
    BLE_STOP_SCAN,
    BLE_DELETE_SCAN,
    /// SCAN_CMD:FOR BLE 4.2
    BLE_INIT_SCAN,
    BLE_DEINIT_SCAN,
    /// conn
    BLE_CONN_UPDATE_MTU,
    BLE_CONN_UPDATE_PARAM,
    BLE_CONN_DIS_CONN,
    BLE_CONN_READ_PHY,
    BLE_CONN_SET_PHY,
    BLE_CONN_ENCRYPT,

    /// init
    BLE_INIT_CREATE,
    BLE_INIT_START_CONN,
    BLE_INIT_STOP_CONN,
    BLE_INIT_DIS_CONN,
    BLE_INIT_READ_CHAR,
    BLE_INIT_WRITE_CHAR,
    ///config
    BLE_SET_MAX_MTU,

    // PERIODIC
    BLE_CREATE_PERIODIC,
    BLE_START_PERIODIC,
    BLE_STOP_PERIODIC,
    BLE_DELETE_PERIODIC,

    BLE_SET_LOCAL_NAME,
    BLE_GET_LOCAL_NAME,

    BLE_READ_LOCAL_ADDR,

    BLE_SET_RANDOM_ADDR,
    BLE_SET_ADV_RANDOM_ADDR,

    BLE_ADD_WHITE_LIST,
    BLE_RMV_WHITE_LIST,
    BLE_CLE_WHITE_LIST,
    BLE_CMD_MAX,
} ble_cmd_t;


/*
 * @brief used in bk_ble_set_notice_cb, this enum show as "notice", you must analyse param in same time
 */
typedef enum
{
    /// ble stack init ok, param null.This event is deprecated,please do not use it.
    BLE_5_STACK_OK,
    /// peer write our, param ble_write_req_t
    BLE_5_WRITE_EVENT,
    /// peer read our, param ble_read_req_t
    BLE_5_READ_EVENT,
    /// scan peer adv report, param ble_recv_adv_t
    BLE_5_REPORT_ADV,
    /// mtu change event, param ble_mtu_change_t
    BLE_5_MTU_CHANGE,
    /// as slaver, recv connnect event, param ble_conn_ind_t
    BLE_5_CONNECT_EVENT,
    /// recv disconnect event, param ble_discon_ind_t
    BLE_5_DISCONNECT_EVENT,
    BLE_5_ATT_INFO_REQ,
    /// create db event, param ble_create_db_t
    BLE_5_CREATE_DB,
    /// tx complete event, param null
    BLE_5_TX_DONE,
    ///smp report
    BLE_5_PAIRING_REQ,
    BLE_5_PAIRING_SUCCEED,
    BLE_5_PAIRING_FAILED,
    ///discard in tizenrt, replace with BLE_5_PARING_PASSKEY_INPUT_REQ and BLE_5_PARING_PASSKEY_DISPLAY_REQ
    BLE_5_PARING_PASSKEY_REQ,
    BLE_5_ENCRYPT_EVENT,

    /// as master, recv connect event
    BLE_5_INIT_CONNECT_EVENT,
    BLE_5_INIT_DISCONNECT_EVENT,
    BLE_5_INIT_CONNECT_FAILED_EVENT,

    BLE_5_SDP_REGISTER_FAILED,
    /// get current conn phy result, param ble_read_phy_t
    BLE_5_READ_PHY_EVENT,
    /// recv conn update event, param ble_conn_param_t
    BLE_5_CONN_UPDATA_EVENT,

    BLE_5_PERIODIC_SYNC_CMPL_EVENT,

    BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT,
    BLE_5_DISCOVERY_CHAR_EVENT,

    BLE_5_RECV_NOTIFY_EVENT,

    BLE_5_ATT_READ_RESPONSE,

    BLE_5_CONN_UPD_PAR_ASK,

    //This event is deprecated,please do not use it.
    BLE_5_SHUTDOWN_SUCCEED,

    ///delete service event
    BLE_5_DELETE_SERVICE_DONE,

    // GAP command complete(including connection param update, mtu exchange, disconnection, phy update...)
    BLE_5_GAP_CMD_CMP_EVENT,

    BLE_5_ADV_STOPPED_EVENT,
    BLE_5_READ_RSSI_CMPL_EVENT,
    BLE_5_KEY_EVENT,
    BLE_5_BOND_INFO_REQ_EVENT,
    BLE_5_READ_BLOB_EVENT,
    BLE_5_PAIRING_SECURITY_REQ_EVENT,
    BLE_5_PARING_NUMBER_COMPARE_REQ_EVENT,

	BLE_5_OOB_REQ_EVENT,
    BLE_5_SC_OOB_REQ_EVENT,
    BLE_5_SC_LOC_OOB_IND,

    BLE_5_SCAN_STOPPED_EVENT,

    BLE_5_PARING_PASSKEY_INPUT_REQ,
    BLE_5_PARING_PASSKEY_DISPLAY_REQ,

    BLE_5_COC_REG_COMPL_EVENT,
    BLE_5_COC_UNREG_COMPL_EVENT,
    BLE_5_COC_CONFIG_COMPL_EVENT,
    BLE_5_COC_CONNECTION_COMPL_EVENT,
    BLE_5_COC_DISCCONNECT_COMPL_EVENT,
    BLE_5_COC_TX_DONE,
    BLE_5_COC_RX_IND,
    BLE_5_COC_CONNECT_REQ_EVENT,

} ble_notice_t;

typedef enum
{
    CHARAC_NOTIFY,
    CHARAC_INDICATE,
    CHARAC_READ,
    CHARAC_READ_DONE,
    CHARAC_WRITE_DONE,
} CHAR_TYPE;

typedef void (*app_sdp_callback)(unsigned char conidx, uint16_t chars_val_hdl, unsigned char uuid_len, unsigned char *uuid);
typedef void (*app_sdp_charac_callback)(CHAR_TYPE type, uint8 conidx, uint16_t hdl, uint16_t len, uint8 *data);

struct ble_sdp_svc_ind
{
    /// Service UUID Length
    uint8_t  uuid_len;
    /// Service UUID
    uint8_t  uuid[16];
    /// Service start handle
    uint16_t start_hdl;
    /// Service end handle
    uint16_t end_hdl;
};

/// characteristic info
struct ble_sdp_char_inf
{
    /// Characteristic UUID Length
    uint8_t uuid_len;
    /// Characteristic UUID
    uint8_t uuid[16];
    /// Characteristic handle
    uint16_t char_hdl;
    /// Value handle
    uint16_t val_hdl;
    /// Characteristic properties
    uint8_t prop;
    /// End of characteristic offset
    uint8_t char_ehdl_off;
};

/// characteristic description
struct ble_sdp_char_desc_inf
{
    /// UUID length
    uint8_t uuid_len;
    /// UUID
    uint8_t uuid[16];

    uint8_t char_code;
    /// Descriptor handle
    uint16_t desc_hdl;
};

/// discover completed evt param, see MST_TYPE_DISCOVER_COMPLETED
struct ble_descover_complete_inf
{
    uint32_t action_id; // such as MST_TYPE_DISCOVER_PRI_SERVICE_RSP
};

typedef struct
{
    uint8_t conn_idx;
    uint8_t status;
    uint32_t num;
} ble_smp_ind_t;


typedef enum
{
    MST_TYPE_SVR_UUID = 0,
    MST_TYPE_ATT_UUID,
    MST_TYPE_ATT_DESC,
    MST_TYPE_SDP_END,

    MST_TYPE_ATTC_SVR_UUID,  ///Service the UUID
    MST_TYPE_ATTC_ATT_UUID,  ///ATT of a service
    MST_TYPE_ATTC_ATT_DESC,  ///ATT DESC of a service
    MST_TYPS_ATTC_PARAM_ERR,  ///The delivered parameter is abnormal or unknown
    MST_TYPE_ATTC_ERR,   ///if appm_get_init_attc_info return is ok && ble is disconnect,so update the event
    MST_TYPE_ATTC_END,   ///End of the operation
    MST_TYPE_ATTC_WRITE_RSP,
    MST_TYPE_ATTC_WRITE_NO_RESPONSE,
    MST_TYPE_ATTC_CHARAC_READ_DONE,

    MST_TYPE_MTU_EXC = 0x10,
    MST_TYPE_MTU_EXC_DONE,

    MST_TYPE_UPP_ASK = 0x20,   ///Ask if you agree to update the parameter
    MST_TYPE_UPDATA_STATUS,    ////updata param status

    MST_TYPE_DISCOVER_PRI_SERVICE_RSP,
    MST_TYPE_DISCOVER_PRI_SERVICE_BY_UUID_RSP,
    MST_TYPE_DISCOVER_PRI_SERVICE_BY_128_UUID_RSP,
    MST_TYPE_DISCOVER_CHAR_RSP,
    MST_TYPE_DISCOVER_CHAR_BY_UUID_RSP,
    MST_TYPE_DISCOVER_CHAR_BY_128_UUID_RSP,
    MST_TYPE_DISCOVER_CHAR_DESC,
    MST_TYPE_DISCOVER_COMPLETED,


} MASTER_COMMON_TYPE;
typedef void (*app_sdp_comm_callback)(MASTER_COMMON_TYPE type, uint8 conidx, void *param);

enum msg_attc
{
    MST_ATTC_ALL = 0,
    MST_ATTC_GET_SVR_UUID_ALL,  ////Gets all the services for this connection
    MST_ATTC_GET_SVR_UUID_BY_SVR_UUID,
    MST_ATTC_GET_ATT_UUID_ALL,  ////Gets all the ATT's for this connection
    MST_ATTC_GET_ATT_DESC_UUID_ALL,  ////Gets all the ATT-DESC's for this connection
    MST_ATTC_SVR_ATT_BY_SVR_UUID, ////Gets all ATT's for this SVR-UUID for this connection
    MST_ATTC_SVR_ATT_DESC_BY_SVR_UUID, ///Gets all ATT-DESC's for this SVR-UUID for this connection
    MST_ATTC_SVR_ATT_AND_DESC_BY_SVR_UUID, ///Gets all ATT and ATT-DESC's for this SVR-UUID for this connection
};

struct ble_attc_wr_rd_op
{
    /// Status of the request
    uint8_t status;
    /// operation sequence number - provided when operation is started
    uint16_t seq_num;
};

struct ble_attc_event_ind
{
    /// Event Type
    enum msg_attc type;

    uint8_t uuid_len;
    uint8_t uuid[16];

    ///if start_hdl = end_hdl = 0,it is invaild
    uint16_t start_hdl;
    uint16_t end_hdl;
};

/**
 * @defgroup bk_ble_api_v1_typedef struct
 * @brief ble struct type
 * @ingroup bk_ble_api_v1_typedef struct
 * @{
 */

typedef struct
{
    uint8_t cmd_idx;      /**< actv_idx */
    ble_err_t status;     /**< The status for this command */
} ble_cmd_param_t;

typedef struct
{
    uint8_t conn_idx;     /**< The index of the connection */
    uint16_t prf_id;      /**< The id of the profile */
    uint16_t att_idx;     /**< The index of the attribute */
    uint16_t length;      /**< The length of the attribute */
    uint8_t status;       /**< Use to know if it's possible to modify the attribute ,can contains authorization or application error code */
} ble_att_info_req_t;

typedef struct
{
    uint8_t conn_idx;     /**< The index of the connection */
    uint16_t prf_id;      /**< The id of the profile */
    uint16_t att_idx;     /**< The index of the attribute */
    uint8_t *value;       /**< The attribute value */
    uint16_t len;         /**< The length of the attribute value */
    uint8_t is_cmd;
} ble_write_req_t;

typedef struct
{
    uint8_t conn_idx;     /**< The index of the connection */
    uint16_t prf_id;      /**< The id of the profile */
    uint16_t att_idx;     /**< The index of the attribute */
    uint8_t *value;       /**< The attribute value */
    uint16_t size;        /**< The size of attribute value to read*/
    uint16_t length;      /**< The data length read */
} ble_read_req_t;


/**<Advertising report type(evt_type(bit0~bit2))*/
enum adv_report_type
{
    /// Extended advertising report
    REPORT_TYPE_ADV_EXT = 0,
    /// Legacy advertising report
    REPORT_TYPE_ADV_LEG,
    /// Extended scan response report
    REPORT_TYPE_SCAN_RSP_EXT,
    /// Legacy scan response report
    REPORT_TYPE_SCAN_RSP_LEG,
    /// Periodic advertising report
    REPORT_TYPE_PER_ADV,
};

/**<Advertising report information
(evt_type(bit3~bit6))*/
enum adv_report_info
{
    /// Report Type
    REPORT_INFO_REPORT_TYPE_MASK    = 0x07,
    /// Report is complete
    REPORT_INFO_COMPLETE_BIT        = (1 << 3),
    /// Connectable advertising
    REPORT_INFO_CONN_ADV_BIT        = (1 << 4),
    /// Scannable advertising
    REPORT_INFO_SCAN_ADV_BIT        = (1 << 5),
    /// Directed advertising
    REPORT_INFO_DIR_ADV_BIT         = (1 << 6),
};

typedef struct
{
    uint8_t actv_idx;     /**< The index of the activity */
    uint8_t evt_type;     /**< Event type (see enum \ref adv_report_info and see enum \ref adv_report_type)*/
    uint8_t adv_addr_type;/**< Advertising address type: public/random */
    uint8_t adv_addr[6];  /**<Advertising address value */
    uint8_t data_len;     /**< Data length in advertising packet */
    uint8_t *data;        /**< Data of advertising packet */
    int8_t rssi;         /**< RSSI value for advertising packet (in dBm, between -127 and +20 dBm) */
} ble_recv_adv_t;

typedef struct
{
    uint8_t conn_idx;     /**< The index of connection */
    uint16_t mtu_size;    /**< The MTU size to exchange */
} ble_mtu_change_t;

typedef struct
{
    /// The index of connection
    uint8_t conn_idx;
    /// Peer address type
    uint8_t peer_addr_type;
    /// Peer BT address
    uint8_t peer_addr[6];
    /// Clock accuracy
    uint8_t clk_accuracy;
    /// Connection interval
    uint16_t con_interval;
    /// Connection latency
    uint16_t con_latency;
    /// Link supervision timeout
    uint16_t sup_to;
} ble_conn_ind_t;

typedef struct
{
    /// The index of connection
    uint8_t conn_idx;
    /// Reason of disconnection
    uint8_t reason;
} ble_discon_ind_t;

typedef struct
{
    uint8_t status;      /**< The status for creating db */
    uint8_t prf_id;      /**< The id of the profile */
    uint16_t start_hdl;
} ble_create_db_t;

typedef struct
{
    /// 16 bits UUID LSB First
    uint8_t uuid[16];
    /// Attribute Permissions (see enum \ref bk_ble_perm_mask)
    uint16_t perm;
    /// Attribute Extended Permissions (see enum \ref bk_ble_ext_perm_mask)
    uint16_t ext_perm;
    /// Attribute Max Size
    /// note: for characteristic declaration contains handle offset
    /// note: for included service, contains target service handle
    uint16_t max_size;

    uint16_t value_len;
    ///pointer to value if BK_BLE_PERM_SET(RI, ENABLE) not set and BK_BLE_PERM_SET(VALUE_INCL, ENABLE) set
    void *p_value_context;
} ble_attm_desc_t;

struct bk_ble_db_cfg
{
    uint16_t prf_task_id;
    ///Service uuid
    uint8_t uuid[16];
    ///Number of db
    uint8_t att_db_nb;
    ///Start handler, 0 means autoalloc
    uint16_t start_hdl;
    ///Attribute database
    ble_attm_desc_t *att_db;
    ///Service config
    uint8_t svc_perm;
};

typedef struct
{
    //TODO put customer specific init configuration here
} ble_init_config_t;

typedef struct
{
    uint8_t addr[BK_BLE_GAP_BD_ADDR_LEN];
} bd_addr_t;

typedef struct
{
    ///The index of connection
    uint8_t conn_idx;
    ///Command operation
    uint8_t cmd;
    ///Command operation status
    uint8_t status;

    union
    {
        struct
        {
            uint16_t psm;
        } coc_reg_evt;

        struct
        {
            uint16_t psm;
        } coc_config_evt;

        struct
        {
            uint16_t le_psm;
            uint16_t local_cid;
            uint16_t peer_credit;
            uint16_t peer_mtu;
            uint16_t peer_mps;
        } coc_connection_compl_evt;

        struct
        {
            uint16_t local_cid;
            uint8_t reason;
        } coc_disconnect_compl_evt;

        struct
        {
            uint16_t cid;
            uint16_t credit;
        } coc_send_compl_evt;

        struct
        {
            uint16_t offset;
            uint16_t cid;
            uint16_t credit;
            uint16_t length;
            uint8_t *data;
        } coc_recv_evt;

        struct
        {
            uint16_t le_psm;
            uint16_t peer_cid;
            uint16_t peer_mtu;
            uint16_t peer_mps;
            uint16_t peer_credit;
        } coc_connect_req_evt;
    };


} ble_cmd_cmp_evt_t;

typedef struct
{
    uint8_t  tx_phy;                       /**< The transmitter PHY */
    uint8_t  rx_phy;                       /**< The receiver PHY */
} ble_read_phy_t;

typedef struct
{
    uint8_t  tx_phy;                       /**< The transmitter PHY */
    uint8_t  rx_phy;                       /**< The receiver PHY */
    uint8_t  phy_opt;                       /**< PHY options  */
} ble_set_phy_t;

/// Type of advertising that can be created
enum adv_type
{
    /// Legacy advertising
    ADV_TYPE_LEGACY = 0,
    /// Extended advertising
    ADV_TYPE_EXTENDED,
    /// Periodic advertising
    ADV_TYPE_PERIODIC,
};

/// Advertising properties bit field bit positions
enum adv_prop_bf
{
    /// Indicate that advertising is connectable, reception of CONNECT_REQ or AUX_CONNECT_REQ
    /// PDUs is accepted. Not applicable for periodic advertising.
    ADV_PROP_CONNECTABLE_BIT     = (1UL << (0)),

    /// Indicate that advertising is scannable, reception of SCAN_REQ or AUX_SCAN_REQ PDUs is
    /// accepted
    ADV_PROP_SCANNABLE_BIT       = (1UL << (1)),

    /// Indicate that advertising targets a specific device. Only apply in following cases:
    ///   - Legacy advertising: if connectable
    ///   - Extended advertising: connectable or (non connectable and non discoverable)
    ADV_PROP_DIRECTED_BIT        = (1UL << (2)),

    /// Indicate that High Duty Cycle has to be used for advertising on primary channel
    /// Apply only if created advertising is not an extended advertising
    ADV_PROP_HDC_BIT             = (1UL << (3)),

    /// Use legacy advertising PDUs
    ADV_PROP_PROP_LEGACY_BIT     = (1UL << (4)),

    /// Enable anonymous mode. Device address won't appear in send PDUs
    /// Valid only if created advertising is an extended advertising
    ADV_PROP_ANONYMOUS_BIT       = (1UL << (5)),

    /// Include TX Power in the extended header of the advertising PDU.
    /// Valid only if created advertising is not a legacy advertising
    ADV_PROP_TX_PWR_BIT          = (1UL << (6)),
};

///Advertising channels enables
enum adv_chnl_map
{
    ///Byte value for advertising channel map for channel 37 enable
    ADV_CHNL_37                = 0x01,
    ///Byte value for advertising channel map for channel 38 enable
    ADV_CHNL_38                = 0x02,
    ///Byte value for advertising channel map for channel 39 enable
    ADV_CHNL_39                = 0x04,
    ///Byte value for advertising channel map for channel 37, 38 and 39 enable
    ADV_ALL_CHNLS              = 0x07,
};

/// PHY Type
enum phy_type_le
{
    /// LE 1M
    PHY_TYPE_LE_1M = 1,
    /// LE 2M
    PHY_TYPE_LE_2M,
    /// LE Coded
    PHY_TYPE_LE_CODED,
};

/// Initiating PHY Type
enum initiating_phy_type_le
{
    /// LE 1M
    INIT_PHY_TYPE_LE_1M = (1UL << (0)),
    /// LE 2M
    INIT_PHY_TYPE_LE_2M = (1UL << (1)),
    /// LE Coded
    INIT_PHY_TYPE_LE_CODED = (1UL << (2)),
};

/// Own addr type
enum ble_own_addr_type
{
    /// Public or Private Static Address according to device address configuration
    /// Attention: Private Static Address is not used now, so set will be public
    /// Attention: this enum is derecated, use OWN_ADDR_TYPE_PUBLIC_ADDR instead.
    OWN_ADDR_TYPE_PUBLIC_OR_STATIC_ADDR __attribute__((deprecated("use OWN_ADDR_TYPE_PUBLIC_ADDR instead"))),

    /// Generated resolvable private random or random address, will auto refresh periodic
    /// Attention: RPA is not used now, so set will be random
    OWN_ADDR_TYPE_GEN_RSLV_OR_RANDOM_ADDR,

    /// Generated non-resolvable private random or random address, will auto refresh periodic
    /// Attention: NRPA is not used now, so set will be random
    OWN_ADDR_TYPE_GEN_NON_RSLV_OR_RANDOM_ADDR,

    /// public addr
    OWN_ADDR_TYPE_PUBLIC_ADDR,

    ///use bk_ble_set_adv_random_addr addr
    OWN_ADDR_TYPE_RANDOM_ADDR,
};

enum initiating_filter_type_le
{
    /// Direct connection establishment, establish a connection with an indicated device
    INIT_TYPE_LE_DIRECT_CONN_EST = 0,
    /// Automatic connection establishment, establish a connection with all devices whose address is
    /// present in the white list
    INIT_TYPE_LE_AUTO_CONN_EST,
    /// Name discovery, Establish a connection with an indicated device in order to read content of its
    /// Device Name characteristic. Connection is closed once this operation is stopped.
    INIT_TYPE_LE_NAME_DISC,
};


/// Scan type
enum ble_scan_type
{
    /// Passive Scanning. No scanning PDUs shall be sent (default)
    PASSIVE_SCANNING,

    /// Active scanning. Scanning PDUs may be sent
    ACTIVE_SCANNING,
};


/// Scan filter policy
enum ble_scan_filter_policy
{
    ///Basic unfiltered scanning filter policy
    BASIC_UNFILTER_SCAN_POLICY            = 0x00,
    ///Basic filtered scanning filter policy
    BASIC_FILTER_SCAN_POLICY,
    ///Extended unfiltered scanning filter policy
    EXTENED_UNFILER_SCAN_POLICY,
    ///Extended filtered scanning filter policy
    EXTENED_FILER_SCAN_POLICY,
};

typedef struct
{
    /// Own address type: see enum \ref ble_own_addr_type
    uint8_t own_addr_type;
    /// Advertising type (see enum \ref adv_type)
    uint8_t adv_type;
    /// Bit field indicating the channel mapping (see enum \ref adv_chnl_map)
    uint8_t chnl_map;
    /// Bit field value provided advertising properties (see enum \ref adv_prop_bf)
    uint16_t adv_prop;
    /// Minimum advertising interval (in unit of 625us). Must be greater than or equal to 20ms
    uint32_t adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than or equal to 20ms
    uint32_t adv_intv_max;
    /// Indicate on which PHY primary advertising has to be performed (see enum \ref phy_type_le)
    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
    uint8_t prim_phy;
    /// Indicate on which PHY secondary advertising has to be performed (see enum \ref phy_type_le)
    uint8_t second_phy;
    /// Peer address type: 0=public/1=private random, used for directed adv
    uint8_t peer_addr_type;
    /// Peer address, used for directed adv
    bd_addr_t peer_addr;
} ble_adv_param_t;

typedef struct
{
    /// Own address type: see enum \ref ble_own_addr_type
    uint8_t own_addr_type;
    /// on which the advertising packets should be received:  LE 1M=1 / LE CODED=4 / LE 1M and LE CODED=5
    uint8_t scan_phy;
    /// Scan interval (in unit of 625us). Must be greater than or equal to 2.5ms
    uint16_t scan_intv;
    /// Scan window (in unit of 625us). Must be greater than or equal to 2.5ms
    uint16_t scan_wd;
    /// Scan type: see enum \ref ble_scan_type
    uint8_t scan_type;
    /// Scan type: see enum \ref ble_scan_filter_policy
    uint8_t scan_filter;
} ble_scan_param_t;

typedef struct
{
    /// Connection interval minimum (in unit of 1.25ms). Must be greater than or equal to 7.5ms
    uint16_t intv_min;
    /// Connection interval maximum (in unit of 1.25ms). Must be greater than or equal to 7.5ms
    uint16_t intv_max;
    /// Connection latency. The range is 0x0000 to 0x01F3
    uint16_t con_latency;
    /// Link supervision timeout(in unit of 10ms). Must be greater than or equal to 100ms
    uint16_t sup_to;
    /// on which the advertising packets should be received on the primary advertising physical channel (see enum \ref phy_type_le)
    uint8_t init_phys;
    /// The index of connection
    uint8_t conn_idx;
    /// filter policy 0:no whitelist used, 1:use whitelist
    uint8_t filter_policy;
} ble_conn_param_t;


/// Constant Tone Extension sync filtering type
enum ble_sync_cte_type
{
    /// Do not sync to packets with an AoA Constant Tone Extension
    CTE_NO_SYNC_WITH_AOA          = (1 << 0),
    /// Do not sync to packets with an AoD Constant Tone Extension with 1 us slots
    CTE_NO_SYNC_WITH_AOD_1US_SLOT = (1 << 1),
    /// Do not sync to packets with an AoD Constant Tone Extension with 2 us slots
    CTE_NO_SYNC_WITH_AOD_2US_SLOT = (1 << 2),
    /// Do not sync to packets with a type 3 Constant Tone Extension (currently reserved for future use)
    CTE_NO_SYNC_WITH_TYPE_3       = (1 << 3),
    /// Do not sync to packets without a Constant Tone Extension
    CTE_NO_SYNC_WITHOUT_CTE       = (1 << 4),
};

typedef struct
{
    /// 1 to disable periodic advertising report, 0 to enable them by default
    uint8_t                         report_disable;


    /// adv sid of advertiser(0x00 to 0x0F)
    uint8_t adv_sid;

    /// Address of advertiser with which synchronization has to be established (used only if use_pal is false)
    bd_addr_t adv_addr;

    uint8_t adv_addr_type;

    /// Number of periodic advertising that can be skipped after a successful receive. Maximum authorized
    /// value is 499
    uint16_t                        skip;
    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint16_t                        sync_to;
    /// Type of Constant Tone Extension device should sync on (see enum \ref ble_sync_cte_type).
    uint8_t                         cte_type;
} ble_periodic_param_t;


typedef struct
{
    uint8_t is_agree;   ///0:is not agree,1:is agree,0xFF;Let me think about it;other:is agree

    uint8_t conn_idx;

    /// Minimum Connection Event Duration
    uint16_t ce_len_min;
    /// Maximum Connection Event Duration
    uint16_t ce_len_max;

    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
} ble_conn_update_para_ind_t;



struct mst_comm_updata_para
{
    /// Status of the request
    uint8_t is_agree;   ///0:is not agree,1:is agree,0xFF;Let me think about it;other:is agree
    /// Minimum Connection Event Duration
    uint16_t ce_len_min;
    /// Maximum Connection Event Duration
    uint16_t ce_len_max;

    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
};

typedef struct
{
    /// The index of adv
    uint8_t adv_idx;
    /// the stopped reason,0:adv is stopped by user stop or link establishment;1:adv is stopped by timeout
    uint8_t reason;
    uint8_t conn_idx;
} ble_adv_stopped_ind_t;

typedef struct
{
    /// The index of scan
    uint8_t scan_idx;
    /// the stopped reason,0:scan is stopped by user stop;1:scan is stopped by timeout
    uint8_t reason;
} ble_scan_stopped_ind_t;


/**
 * @brief BLE_5_READ_RSSI_CMPL_EVENT data
 */
typedef struct
{
    /// The index of connection
    uint8_t conn_idx;
    ///RSSI value
    int8_t rssi;
} ble_read_rssi_rsp_t;

/**
* @brief BLE encryption keys
*/
typedef struct
{
    uint8_t ltk[16];          /*!< The long term key*/
    uint8_t rand[8];         /*!< The random number*/
    uint16_t ediv;         /*!< The ediv value*/
    uint8_t  key_size;     /*!< The key size(7~16) of the security link*/
} bk_ble_enc_keys_t;

/**
* @brief  BLE irk info
*/
typedef struct
{
    uint8_t irk[16];           /*!< The irk value */
    uint8_t addr_type;     /*!< The address type */
    uint8_t id_addr[6];   /*!< The Identity address */
} bk_ble_irk_info_t;

typedef struct
{
    /// Peer address type
    uint8_t peer_addr_type;
    /// Peer address
    uint8_t peer_addr[6];
    ///Authentication level
    uint8_t auth;
    ///received peer encryption key
    bk_ble_enc_keys_t penc;
    ///local encryption key
    bk_ble_enc_keys_t lenc;
    ///peer Identity Resolving Key information
    bk_ble_irk_info_t pirk;
} bk_ble_key_t;

typedef struct
{
    /// check whether key is found
    uint8_t key_found;
    //ble bonding key
    bk_ble_key_t key;
} bk_ble_bond_info_req_t;

typedef struct
{
    /// GATT request type
    uint8_t operation;
    /// Status of the request
    uint8_t status;
    /// service id
    uint16_t prf_id;
    /// attribute id
    uint16_t att_id;
    /// connection index
    uint8_t conn_idx;
} bk_ble_gatt_cmp_evt_t;

/**
 * @brief BLE_5_SC_LOC_OOB_IND data
 */
typedef struct
{
    /// The index of connection
    uint8_t conn_idx;
    ///Local OOB Data Confirmation/Commitment
    uint8_t local_oob_c[16];
    ///Local OOB Data Randomizer
    uint8_t local_oob_r[16];
} ble_loc_oob_data_t;

/**
 * @brief for sync ble api call return
 *
 * most ble api have ble_cmd_cb_t param, you must wait is callback.
 *
 * @param
 * - cmd: cmd id.
 * - param: param
 *
**/
typedef void (*ble_cmd_cb_t)(ble_cmd_t cmd, ble_cmd_param_t *param);

/**
 * @brief for async ble api event.
 *
 * ble event report.
 *
 * @param
 * - notice: event id.
 * - param: param
 *
**/
typedef void (*ble_notice_cb_t)(ble_notice_t notice, void *param);



/**
 * @brief for hci callback.
 *
 * stack report evt, acl to upper
 *
 * @param
 * - buf: payload
 * - len: buf's len
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
typedef ble_err_t (*ble_hci_to_host_cb)(uint8_t *buf, uint16_t len);

/// Authentication mask
enum gap_auth_mask
{
    /// No Flag set
    GAP_AUTH_NONE    = 0,
    /// Bond authentication
    GAP_AUTH_BOND    = (1 << 0),
    /// Man In the middle protection
    GAP_AUTH_MITM    = (1 << 2),
    /// Secure Connection
    GAP_AUTH_SEC_CON = (1 << 3),
    /// Key Notification
    GAP_AUTH_KEY_NOTIF = (1 << 4)
};

enum gap_auth
{
    /// No MITM No Bonding
    GAP_AUTH_REQ_NO_MITM_NO_BOND  = (GAP_AUTH_NONE),
    /// No MITM Bonding
    GAP_AUTH_REQ_NO_MITM_BOND     = (GAP_AUTH_BOND),
    /// MITM No Bonding
    GAP_AUTH_REQ_MITM_NO_BOND     = (GAP_AUTH_MITM),
    /// MITM and Bonding
    GAP_AUTH_REQ_MITM_BOND        = (GAP_AUTH_MITM | GAP_AUTH_BOND),
    /// SEC_CON and No Bonding
    GAP_AUTH_REQ_SEC_CON_NO_BOND  = (GAP_AUTH_SEC_CON | GAP_AUTH_MITM),
    /// SEC_CON and Bonding
    GAP_AUTH_REQ_SEC_CON_BOND     = (GAP_AUTH_SEC_CON | GAP_AUTH_MITM | GAP_AUTH_BOND),

    GAP_AUTH_REQ_LAST,

    /// Mask of  authentication features without reserved flag
    GAP_AUTH_REQ_MASK             = 0x1F,
};

/// IO Capability Values
enum bk_ble_gap_io_cap
{
    /// Display Only
    BK_BLE_GAP_IO_CAP_DISPLAY_ONLY = 0x00,
    /// Display Yes No
    BK_BLE_GAP_IO_CAP_DISPLAY_YES_NO,
    /// Keyboard Only
    BK_BLE_GAP_IO_CAP_KB_ONLY,
    /// No Input No Output
    BK_BLE_GAP_IO_CAP_NO_INPUT_NO_OUTPUT,
    /// Keyboard Display
    BK_BLE_GAP_IO_CAP_KB_DISPLAY,
    BK_BLE_GAP_IO_CAP_LAST
};

/// Security Defines
enum gap_sec_req
{
    /// No security (no authentication and encryption)
    GAP_NO_SEC = 0x00,
    /// Unauthenticated pairing with encryption
    GAP_SEC1_NOAUTH_PAIR_ENC,
    /// Authenticated pairing with encryption
    GAP_SEC1_AUTH_PAIR_ENC,
    /// Unauthenticated pairing with data signing
    GAP_SEC2_NOAUTH_DATA_SGN,
    /// Authentication pairing with data signing
    GAP_SEC2_AUTH_DATA_SGN,
    /// Secure Connection pairing with encryption
    GAP_SEC1_SEC_CON_PAIR_ENC,
};

/// OOB Data Present Flag Values
enum gap_oob
{
    /// OOB Data not present
    GAP_OOB_AUTH_DATA_NOT_PRESENT = 0x00,
    /// OOB data present
    GAP_OOB_AUTH_DATA_PRESENT,
    GAP_OOB_AUTH_DATA_LAST
};

/// key distr
enum gap_key_distr
{
    /// No Keys to distribute
    BK_BLE_GAP_KDIST_NONE = 0x00,
    /// Encryption key in distribution
    BK_BLE_GAP_KDIST_ENCKEY = (1 << 0),
    /// IRK (ID key)in distribution
    BK_BLE_GAP_KDIST_IDKEY  = (1 << 1),
    /// CSRK(Signature key) in distribution
    BK_BLE_GAP_KDIST_SIGNKEY = (1 << 2),
    /// LTK in distribution
    BK_BLE_GAP_KDIST_LINKKEY = (1 << 3),

    BK_BLE_GAP_KDIST_LAST = (1 << 4),
};

enum coc_info
{
    BK_BLE_COC_PEER_CURRENT_CREDIT,
    BK_BLE_COC_PEER_MAX_CREDIT,
    BK_BLE_COC_PEER_MTU,
    BK_BLE_COC_PEER_MPS,
    BK_BLE_COC_LOCAL_CURRENT_CREDIT,
    BK_BLE_COC_LOCAL_MAX_CREDIT,
    BK_BLE_COC_LOCAL_MTU,
    BK_BLE_COC_LOCAL_MPS,
};

enum coc_security
{
    BK_BLE_COC_SEC_NONE,
    BK_BLE_COC_SEC_UNAUTH_ENCRYPT,
    BK_BLE_COC_SEC_AUTH_ENCRYPT,
    BK_BLE_COC_SEC_SECURE_CONNECTION,
};

/**
 * @}
 */


/**
 * @}
 */


#endif /* INCLUDE_MODULES_BK_BLE_TYPES_H_ */
