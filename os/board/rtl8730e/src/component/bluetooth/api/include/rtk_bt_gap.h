/**
 * @file      rtk_bt_gap.h
 * @author    ping_yan@realsil.com.cn
 * @brief     Bluetooth common GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_GAP_H__
#define __RTK_BT_GAP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <basic_types.h>
#include <bt_api_config.h>

/**
 * @typedef   rtk_bt_addr_type_t
 * @brief     Bluetooth device address type definition.
 */
typedef enum {
	RTK_BT_ADDR_LE_PUBLIC =             0x00,      /*!< Public Device Address */
	RTK_BT_ADDR_LE_RANDOM =             0x01,      /*!< Random Device Address */
	RTK_BT_ADDR_LE_RPA_PUBLIC =         0x02,      /*!< Public Identity Address (RPA) */
	RTK_BT_ADDR_LE_RPA_RANDOM =         0x03,      /*!< Random (static) Identity Address (RPA)*/
	RTK_BT_ADDR_CLASSIC =               0x10,      /*!< BR/EDR Device address type. */
	RTK_BT_ADDR_LE_ANONYMOUS =          0xFF,      /*!< Anonymous Device Address */
} rtk_bt_addr_type_t;

#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT

/**
 * @def     RTK_BT_GAP_ECFC_CHANN_MAX_NUM
 * @brief   The maximum number of channels created at one time.
 */
#define RTK_BT_GAP_ECFC_CHANN_MAX_NUM          5

/**
 * @def     RTK_BT_GAP_ECFC_MIN_MTU
 * @brief   Minimum length of MTU.
 */
#define RTK_BT_GAP_ECFC_MIN_MTU                64

/**
 * @def     RTK_BT_GAP_ECFC_MIN_MPS
 * @brief   Minimum length of MPS.
 */
#define RTK_BT_GAP_ECFC_MIN_MPS                64

/**
 * @def   RTK_BD_ADDR_LEN
 * @brief     Bluetooth device address length macro.
 */
#define RTK_BD_ADDR_LEN                        6

/**
 * @brief Prototype of callback function to handle L2CAP ECFC related messages
 * @param[in] p_buf     Message data and the content of data is dependent on message type.
 * @param[in] msg       callback msy type @ref T_GAP_ECFC_MSG.
 * @return Result
 * @retval 0 Success
 * @retval other Failed
  */
// typedef uint16_t (* P_GAP_ECFC_CB)(void *p_buf, T_GAP_ECFC_MSG msg);

/**
 * @typedef   rtk_bt_gap_ecfc_sec_setting_t
 * @brief     Bluetooth GAP ECFC Security Setting.
 */
typedef enum {
	RTK_BT_GAP_ECFC_SEC_BIT_AUTHEN      = 0x02, /*!< Authentication is required. */
	RTK_BT_GAP_ECFC_SEC_BIT_MITM        = 0x04, /*!< MITM authentication is required. */
	RTK_BT_GAP_ECFC_SEC_BIT_AUTHOR      = 0x08, /*!< User level authorization is required. */
	RTK_BT_GAP_ECFC_SEC_BIT_ENCRYPT     = 0x10, /*!< Encryption on link is required. */
	RTK_BT_GAP_ECFC_SEC_BIT_SC          = 0x20, /*!< Secure connection on link is required. */
} rtk_bt_gap_ecfc_sec_setting_t;

/**
 * @typedef   rtk_bt_gap_ecfc_data_path_t
 * @brief     Bluetooth GAP ECFC data path type.
 */
typedef enum {
	RTK_BT_GAP_ECFC_DATA_PATH_APP      = 0x00, /*!< Application handles the data. ATT layer will not handle the data. */
	RTK_BT_GAP_ECFC_DATA_PATH_GATT     = 0x01, /*!< ATT layer handles the data. */
} rtk_bt_gap_ecfc_data_path_t;

/**
 * @typedef   rtk_bt_gap_ecfc_param_t
 * @brief     Bluetooth GAP ECFC paramter type.
 */
typedef enum {
	RTK_BT_GAP_ECFC_PARAM_INIT_CREDITS      = 0x430, /*!< Local initial credits. Write Only. Default is 4. Range is 1 ~ 65535. */
	RTK_BT_GAP_ECFC_PARAM_LOCAL_MPS         = 0x431, /*!< Local MPS. Write Only. Default is 245. Range is 64 ~ 25533. */
} rtk_bt_gap_ecfc_param_t;

/**
 * @typedef   rtk_bt_gap_ecfc_param_set_t
 * @brief     Bluetooth GAP ECFC set paramter definition.
 */
typedef struct {
	rtk_bt_gap_ecfc_param_t type;       /*!< Parameter type. @ref rtk_bt_gap_ecfc_param_t. */
	uint16_t value;                     /*!< Value to set. */
} rtk_bt_gap_ecfc_param_set_t;

/**
 * @typedef   rtk_bt_gap_ecfc_register_t
 * @brief     Bluetooth GAP ECFC register protocal definition.
 */
typedef struct {
	uint16_t psm;                /*!< Protocol service multiplexer that the callback function is related to. Set to @ref RTK_BT_PSM_EATT if channels are created for EATT. */
	bool is_le;                  /*!< Whether is LE transport. */
	uint16_t *p_proto_id;        /*!< Protocol ID assigned by GAP ECFC module which will be used in @ref rtk_gap_ecfc_send_conn_req. */
	rtk_bt_gap_ecfc_data_path_t data_path;
} rtk_bt_gap_ecfc_register_t;

/**
 * @typedef   rtk_bt_gap_ecfc_send_sec_reg_req_t
 * @brief     Bluetooth GAP ECFC security register/deregister request definition.
 */
typedef struct {
	bool is_le;                  /*!< Whether is LE transport. */
	bool active;                 /*!< Register/Deregister the security entry. */
	uint16_t psm;                /*!< PSM value for protocol. Set to @ref RTK_BT_PSM_EATT if channels are created for EATT. */
	uint16_t uuid;               /*!< BT Protocol/Profile UUID. Ignored when is_le is true. */
	uint8_t requirement;         /*!< Security requirement of the entry. Valid values are combinations of @ref rtk_bt_gap_ecfc_sec_setting_t. */
	uint8_t key_size;            /*!< Key size requirement of the entry. MAX is 16. */
} rtk_bt_gap_ecfc_sec_reg_req_t;

/**
 * @typedef   rtk_bt_gap_ecfc_conn_req_t
 * @brief     Bluetooth GAP ECFC connection request definition.
 */
typedef struct {
	uint16_t psm;                /*!< Protocol service multiplexer of the L2CAP ECFC channel that will be established. Set to @ref RTK_BT_PSM_EATT if channels are created for EATT. */
	uint16_t proto_id;           /*!< Protocol ID created by @ref bt_stack_gap_ecfc_register. */
	uint16_t local_mtu;          /*!< Preferred MTU size value of the L2CAP ECFC channel. */
	uint8_t bd_addr[RTK_BD_ADDR_LEN];          /*!< Bluetooth address of remote device. */
	uint8_t bd_type;             /*!< Bluetooth address type of remote device. */
	uint8_t cid_num;             /*!< The number of the L2CAP ECFC channels request to establish. Range: (1 ~ RTK_BT_GAP_ECFC_CHANN_MAX_NUM) */
} rtk_bt_gap_ecfc_conn_req_t;

/**
 * @typedef   rtk_bt_gap_ecfc_conn_cfm_cause_t
 * @brief     Bluetooth GAP ECFC connection confirm cause definition.
 */
typedef enum {
	RTK_BT_L2C_ECFC_ALL_CONN_SUCCESS = 0x0000,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_PSM_NOT_SUPPORTED = 0x0002,
	RTK_BT_L2C_ECFC_CONN_RSP_SOME_CONN_NO_RESOURCES = 0x0004,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_AUTHEN = 0x0005,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_AUTHOR = 0x0006,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_ENCRYP_KEY_SIZE = 0x0007,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_ENCRYP = 0x0008,
	RTK_BT_L2C_ECFC_CONN_RSP_SOME_CONN_INVALID_SOURCE_CID = 0x0009,
	RTK_BT_L2C_ECFC_CONN_RSP_SOME_CONN_SOURCE_CID_ALREADY_ALLOC = 0x000A,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_UNACCEPTABLE_PARAMS = 0x000B,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_INVALID_PARAMS = 0x000C,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_NO_INFO = 0x000D,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_AUTHER = 0x000E,
	RTK_BT_L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_AUTHOR = 0x000F,
} rtk_bt_gap_ecfc_conn_cfm_cause_t;

/**
 * @typedef   rtk_bt_gap_ecfc_send_conn_cfm_t
 * @brief     Bluetooth GAP ECFC connection confirm definition.
 */
typedef struct {
	uint16_t conn_handle;        /*!< Connection handle. The value shall be get from @ref RTK_BT_GAP_ACT_ECFC_CONN_REQ message. */
	uint16_t identity_id;        /*!< Indentity id. The value shall be get from @ref RTK_BT_GAP_ACT_ECFC_CONN_REQ message. */
	rtk_bt_gap_ecfc_conn_cfm_cause_t cause;          /*!< Confirmation cause for the connection request from remote device. */
	uint16_t p_cid[RTK_BT_GAP_ECFC_CHANN_MAX_NUM];   /*!< Confirmation the cid array allow to establish.
                                  *   The array shall be the subset of the p_cid get from @ref RTK_BT_GAP_ACT_ECFC_CONN_REQ message. */
	uint8_t cid_num;             /*!< Confirmation the number of the L2CAP ECFC channels allow to establish. */
	uint16_t local_mtu;          /*!< Preferred MTU size value of the L2CAP ECFC channel. */
} rtk_bt_gap_ecfc_conn_cfm_t;

/**
 * @typedef   rtk_bt_gap_ecfc_send_disconn_req_t
 * @brief     Bluetooth GAP ECFC disconnection request definition.
 */
typedef struct {
	uint16_t conn_handle;        /*!< Connection handle. The value shall be get from @ref RTK_BT_GAP_ACT_ECFC_CONN_CFM message. */
	uint16_t cid;                /*!< Local channel ID of the L2CAP ECFC channel to disconnect. */
} rtk_bt_gap_ecfc_disconn_req_t;

/**
 * @typedef   rtk_bt_gap_ecfc_send_data_t
 * @brief     Bluetooth GAP ECFC send data request definition.
 */
typedef struct {
	uint16_t conn_handle;        /*!< Connection handle. The value shall be get from @ref RTK_BT_GAP_ACT_ECFC_CONN_CFM message. */
	uint16_t cid;                /*!< Local channel ID. The value can be get from @ref GAP_ECFC_CONN_CMPL message. */
	uint8_t *p_data;             /*!< Point to data to be sent. */
	uint16_t length;             /*!< Length of value to be sent. Range: 0 ~ local_mtu. */
} rtk_bt_gap_ecfc_send_data_t;

/**
 * @typedef   rtk_bt_gap_ecfc_reconf_req_t
 * @brief     Bluetooth GAP ECFC reconfigure request definition.
 */
typedef struct {
	uint16_t conn_handle;        /*!< Connection handle. The value shall be get from @ref RTK_BT_GAP_ACT_ECFC_CONN_CFM message. */
	uint16_t p_cid[RTK_BT_GAP_ECFC_CHANN_MAX_NUM];     /*!< Local CID array need to reconfigure. */
	uint8_t cid_num;             /*!< The cid number of p_cid array. Range: (1 ~ RTK_BT_GAP_ECFC_CHANN_MAX_NUM) */
	uint16_t local_mtu;          /*!< Preferred Local MTU size value of the L2CAP ECFC channel. */
	uint16_t local_mps;          /*!< Preferred Local MPS value of the L2CAP ECFC channel. */
} rtk_bt_gap_ecfc_reconf_req_t;

/**
 * @struct    rtk_bt_le_adv_start_ind_t
 * @brief     Bluetooth L2CAP ECFC channel establish result indication.
 */
typedef struct {
	uint16_t proto_id;           /*!< Protocol ID assigned by GAP ECFC Module. */
	uint16_t err;                /*!< Result of establishing a L2CAP ECFC channels with remote device. */
	uint16_t conn_handle;        /*!< Connection handle. */
	uint8_t cid_num;             /*!< The number of the L2CAP ECFC channels successfully established. */
	uint16_t cid[RTK_BT_GAP_ECFC_CHANN_MAX_NUM]; /*!< Local CID array assigned by Bluetooth stack. */
	uint16_t remote_mtu;         /*!< Remote MTU value. */
	uint16_t local_mtu;          /*!< Local MTU value. */
	uint16_t local_mps;          /*!< Local MPS value. */
	uint16_t ds_data_offset;     /*!< Offset used to fill user data when sending L2CAP data to remote. */
	uint8_t bd_addr[RTK_BD_ADDR_LEN];  /*!< Bluetooth address of remote device. */
	rtk_bt_addr_type_t bd_type;  /*!< Bluetooth address type of remote device. */
} rtk_bt_ecfc_conn_cmpl_ind_t;

/**
 * @struct    rtk_bt_ecfc_data_ind_t
 * @brief     Bluetooth L2CAP ECFC data reception indication.
 */
typedef struct {
	uint16_t proto_id;          /*!< Protocol ID assigned by GAP ECFC Module. */
	uint16_t conn_handle;       /*!< Connection handle. */
	uint16_t cid;               /*!< Local CID assigned by Bluetooth stack. */
	uint16_t length;            /*!< Length of L2CAP data. */
	uint8_t *data;              /*!< L2CAP data. */
} rtk_bt_ecfc_data_ind_t;

/**
 * @struct    rtk_bt_ecfc_conn_req_ind_t
 * @brief     Bluetooth L2CAP ECFC channel establish request indication.
 */
typedef struct {
	uint16_t proto_id;          /*!< Protocol ID assigned by GAP ECFC Module. */
	uint16_t conn_handle;       /*!< Connection handle. */
	uint16_t remote_mtu;        /*!< Remote MTU value. */
	uint8_t cid_num;            /*!< The number of the L2CAP ECFC channels successfully established. */
	uint16_t cid[RTK_BT_GAP_ECFC_CHANN_MAX_NUM]; /*!< Local CID array assigned by Bluetooth stack. */
	uint8_t bd_addr[RTK_BD_ADDR_LEN];  /*!< Bluetooth address of remote device. */
	rtk_bt_addr_type_t bd_type; /*!< Bluetooth address type of remote device. */
	uint16_t identity_id;       /*!< Identity id for @ref rtk_bt_gap_ecfc_send_conn_cfm. */
} rtk_bt_ecfc_conn_req_ind_t;

/**
 * @struct    rtk_bt_ecfc_disconn_ind_t
 * @brief     Bluetooth L2CAP ECFC channel disconnection indication.
 */
typedef struct {
	uint16_t proto_id;          /*!< Protocol ID assigned by GAP ECFC Module. */
	uint16_t conn_handle;       /*!< Connection handle. */
	uint16_t cid;               /*!< Local CID assigned by Bluetooth stack. */
	uint16_t cause;             /*!< L2CAP channel disconnect reason received. */
} rtk_bt_ecfc_disconn_ind_t;

/**
 * @struct    rtk_bt_ecfc_reconf_req_ind_t
 * @brief     Bluetooth L2CAP ECFC channel reconfigure request indication.
 */
typedef struct {
	uint16_t proto_id;          /*!< Protocol ID assigned by GAP ECFC Module. */
	uint16_t conn_handle;       /*!< Connection handle. */
	uint8_t cid_num;            /*!< The number of the L2CAP ECFC channels need to reconfigure. */
	uint16_t cid[RTK_BT_GAP_ECFC_CHANN_MAX_NUM]; /*!< Local CID array assigned by Bluetooth stack. */
	uint16_t remote_mtu;        /*!< Remote MTU. */
	uint16_t remote_mps;        /*!< REmote MPS. */
} rtk_bt_ecfc_reconf_req_ind_t;

/**
 * @struct    rtk_bt_ecfc_reconf_rsp_ind_t
 * @brief     Bluetooth L2CAP ECFC channel reconfigure response indication.
 */
typedef struct {
	uint16_t proto_id;          /*!< Protocol ID assigned by GAP ECFC Module. */
	uint16_t cause;             /*!< Result of reconfiguration. */
	uint16_t conn_handle;       /*!< Connection handle. */
	uint8_t cid_num;            /*!< The number of the L2CAP ECFC channels successfully reconfigured. */
	uint16_t cid[RTK_BT_GAP_ECFC_CHANN_MAX_NUM]; /*!< Local CID array assigned by Bluetooth stack. */
	uint16_t local_mtu;         /*!< Local MTU value. */
	uint16_t local_mps;         /*!< Local MPS value. */
} rtk_bt_ecfc_reconf_rsp_ind_t;
#endif

/**
 * @struct    rtk_bt_gap_vendor_cmd_param_t
 * @brief     Bluetooth GAP vendor cmd parameter.
 */
typedef struct {
	uint16_t op;                    /*!< Vendor cmd opcode */
	uint8_t len;                    /*!< Length of cmd parameters */
	uint8_t *cmd_param;             /*!< Pointer to parameters to write */
} rtk_bt_gap_vendor_cmd_param_t;

/********************************* Functions Declaration *******************************/
#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
/**
 * @defgroup  bt_gap BT GAP APIs
 * @brief     BT GAP related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief     Set ECFC paramter.
 * @param[in] type: Parameter type.
 * @param[in] value: Paramter value.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_set_param(rtk_bt_gap_ecfc_param_t type, uint16_t value);

/**
 * @brief     Register L2CAP ECFC for a specific PSM.
 * @param[in] p_param: Protocol register parameter.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_register(rtk_bt_gap_ecfc_register_t *p_param);

/**
 * @brief     Send a request to register/deregister a protocol security entry over L2CAP.
 * @param[in] p_param: Security register request parameter.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_send_sec_reg_req(rtk_bt_gap_ecfc_sec_reg_req_t *p_param);

/**
 * @brief     Send a request to create L2CAP ECFC connections. @ref RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND will tell the result of L2CAP channel establishment.
 * @param[in] p_param: Create connection request parameter.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_send_conn_req(rtk_bt_gap_ecfc_conn_req_t *p_param);

/**
 * @brief     Send a confirmation for a L2CAP ECFC connection request from remote device. @ref RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND will tell the result of L2CAP channel establishment.
 * @param[in] p_param: Create connection comfirm parameter.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_send_conn_cfm(rtk_bt_gap_ecfc_conn_cfm_t *p_param);

/**
 * @brief     Send a request to disconnect a L2CAP ECFC connection.
 * @param[in] conn_handle: Connection handle.
 * @param[in] cid: Channel id.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_send_disconn_req(uint16_t conn_handle, uint16_t cid);

/**
 * @brief     RSend a request to send L2CAP ECFC channel data to remote device. This API can only be used when param->data_path in @ref rtk_bt_gap_ecfc_register is RTK_BT_GAP_ECFC_DATA_PATH_APP.
 * @param[in] conn_handle: Connection handle.
 * @param[in] cid: Channel id.
 * @param[in] p_data: Data content.
 * @param[in] length: Data length.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_send_data(uint16_t conn_handle, uint16_t cid, uint8_t *p_data, uint16_t length);

/**
 * @brief     Send a request to reconfigure L2CAP ECFC channel.
 * @param[in] p_param: Protocol register parameter.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_ecfc_send_reconf_req(rtk_bt_gap_ecfc_reconf_req_t *p_param);
/**
 * @}
 */
#endif

/**
 * @brief     Send vendor command to controller
 * @param[in] vendor_param: Vendor cmd information parameter
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gap_vendor_cmd_req(rtk_bt_gap_vendor_cmd_param_t *vendor_param);

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_GAP_H__ */
