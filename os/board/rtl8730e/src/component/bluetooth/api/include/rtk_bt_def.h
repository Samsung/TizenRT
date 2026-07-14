/**
 * @file      rtk_bt_def.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth status and error definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_DEF_H__
#define __RTK_BT_DEF_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @typedef   rtk_bt_status_t
 * @brief     Bluetooth API status definition.
 */
typedef enum {
	RTK_BT_STATUS_DONE = 0,
	RTK_BT_STATUS_CONTINUE,
	RTK_BT_STATUS_FAIL,
} rtk_bt_status_t;

/**
 * @enum    rtk_bt_err_group
 * @brief     Bluetooth API err group mask.
 */
enum rtk_bt_err_group {
	RTK_BT_ERR_COMMON_GROUP     =   0x0000,  /*!< BT API common err, ref @ref rtk_bt_err_common */
	RTK_BT_ERR_HCI_GROUP        =   0x0100,  /*!< BT HCI err, ref @ref rtk_bt_err_hci */
	RTK_BT_ERR_L2CAP_GROUP      =   0x0200,  /*!< BT L2CAP err, different L2CAP packets may have different subgroup err, ref @ref rtk_bt_err_l2cap_subgroup */
	RTK_BT_ERR_ATT_GROUP        =   0x0400,  /*!< BT ATT err, ref @ref rtk_bt_err_att */
	RTK_BT_ERR_SM_GROUP         =   0x0500,  /*!< BT SMP err, ref @ref rtk_bt_err_smp */
	RTK_BT_ERR_GATT_GROUP       =   0x0C00,  /*!< BT GATT err, ref @ref rtk_bt_err_gatt */
};

/**
 * @enum    rtk_bt_err_common
 * @brief   Bluetooth API common sub err code.
 */
enum rtk_bt_err_common {
	RTK_BT_OK                       =   0x00,   /*!< 0, means success */
	RTK_BT_FAIL                     =   0x01,   /*!< 1, means common failure */
	RTK_BT_ERR_NO_PERM              =   0x02,   /*!< 2 */
	RTK_BT_ERR_NOT_READY            =   0x03,   /*!< 3 */
	RTK_BT_ERR_NO_MEMORY            =   0x04,   /*!< 4 */
	RTK_BT_ERR_MISMATCH             =   0x05,   /*!< 5 */
	RTK_BT_ERR_BUSY                 =   0x06,   /*!< 6 */
	RTK_BT_ERR_OS_OPERATION         =   0x07,   /*!< 7 */
	RTK_BT_ERR_MSG_SEND             =   0x08,   /*!< 8 */
	RTK_BT_ERR_UNSUPPORTED          =   0x09,   /*!< 9 */
	RTK_BT_ERR_PARAM_INVALID        =   0x0A,   /*!< 10 */
	RTK_BT_ERR_UNHANDLED            =   0x0B,   /*!< 11 */
	RTK_BT_ERR_NEED_TRY_AGAIN       =   0x0C,   /*!< 12 */
	RTK_BT_ERR_ALREADY_DONE         =   0x0D,   /*!< 13 */
	RTK_BT_ERR_ALREADY_IN_PROGRESS  =   0x0E,   /*!< 14 */
	RTK_BT_ERR_POINTER_INVALID      =   0x0F,   /*!< 15 */
	RTK_BT_ERR_NO_CREDITS           =   0x10,   /*!< 16 */
	RTK_BT_ERR_NO_BUFS              =   0x11,   /*!< 17 */
	RTK_BT_ERR_NO_ENTRY             =   0x12,   /*!< 18 */
	RTK_BT_ERR_NO_RESOURCE          =   0x13,   /*!< 19 */
	RTK_BT_ERR_ADV_LENGTH_INVALID   =   0x14,   /*!< 20 */
	RTK_BT_ERR_NO_CONNECTION        =   0x15,   /*!< 21 */
	RTK_BT_ERR_IRK_NOT_FOUND        =   0x16,   /*!< 22 */
	RTK_BT_ERR_STATE_INVALID        =   0x17,   /*!< 23 */
	RTK_BT_ERR_QUEUE_FULL           =   0x18,   /*!< 24 */
	RTK_BT_ERR_SEND_REQ_FAILURE     =   0x19,   /*!< 25 */
	RTK_BT_ERR_PDU_SIZE_INVALID     =   0x1A,   /*!< 26 */
	RTK_BT_ERR_CONN_NUM_LIMIT       =   0x1B,   /*!< 27 */
	RTK_BT_ERR_NO_BOND              =   0x1C,   /*!< 28 */
	RTK_BT_ERR_SYNC_TIMEOUT         =   0x1D,   /*!< 29 */
	RTK_BT_ERR_LOWER_STACK_API      =   0x1E,   /*!< 30 */
	RTK_BT_ERR_LOWER_STACK_CB       =   0x1F,   /*!< 31 */
	RTK_BT_ERR_CREATE_CONN_TIMEOUT  =   0x20,   /*!< 32 */
	RTK_BT_ERR_NO_CASE_ELEMENT      =   0x21,   /*!< 33 */
	RTK_BT_ERR_UNKNOWN              =   0xFF,   /*!< 255 */
};

/**
 * @enum      rtk_bt_err_hci
 * @brief     Bluetooth API hci sub err code. Details ref bluetooth spec
 */
enum rtk_bt_err_hci {
	RTK_BT_HCI_ERR_UNKNOWN_CMD                     =    0x01,
	RTK_BT_HCI_ERR_UNKNOWN_CONN_ID                 =    0x02,
	RTK_BT_HCI_ERR_HARDWARE_FAIL                   =    0x03,
	RTK_BT_HCI_ERR_PAGE_TIMEOUT                    =    0x04,
	RTK_BT_HCI_ERR_AUTHEN_FAIL                     =    0x05,
	RTK_BT_HCI_ERR_KEY_MISSING                     =    0x06,
	RTK_BT_HCI_ERR_MEMORY_FULL                     =    0x07,
	RTK_BT_HCI_ERR_CONN_TIMEOUT                    =    0x08,
	RTK_BT_HCI_ERR_MAX_NUM_CONN                    =    0x09,
	RTK_BT_HCI_ERR_MAX_NUM_SCO_CONN                =    0x0A,
	RTK_BT_HCI_ERR_ACL_CONN_EXIST                  =    0x0B,
	RTK_BT_HCI_ERR_CMD_DISALLOWED                  =    0x0C,
	RTK_BT_HCI_ERR_REJECT_LIMITED_RESOURCE         =    0x0D,
	RTK_BT_HCI_ERR_REJECT_SECURITY_REASON          =    0x0E,
	RTK_BT_HCI_ERR_REJECT_UNACCEPTABLE_ADDR        =    0x0F,
	RTK_BT_HCI_ERR_HOST_TIMEOUT                    =    0x10,
	RTK_BT_HCI_ERR_UNSUPPORTED_PARAM               =    0x11,
	RTK_BT_HCI_ERR_INVALID_PARAM                   =    0x12,
	RTK_BT_HCI_ERR_REMOTE_USER_TERMINATE           =    0x13,
	RTK_BT_HCI_ERR_REMOTE_LOW_RESOURCE             =    0x14,
	RTK_BT_HCI_ERR_REMOTE_POWER_OFF                =    0x15,
	RTK_BT_HCI_ERR_LOCAL_HOST_TERMINATE            =    0x16,
	RTK_BT_HCI_ERR_REPEATED_ATTEMPTS               =    0x17,
	RTK_BT_HCI_ERR_PARING_NOT_ALLOWED              =    0x18,
	RTK_BT_HCI_ERR_UNKNOWN_LMP_PDU                 =    0x19,
	RTK_BT_HCI_ERR_UNSUPPORTED_REMOTE_FEAT         =    0x1A,
	RTK_BT_HCI_ERR_SCO_OFFSET_REJECTED             =    0x1B,
	RTK_BT_HCI_ERR_SCO_INTERVAL_REJECTED           =    0x1C,
	RTK_BT_HCI_ERR_SCO_AIR_MODE_REJECTED           =    0x1D,
	RTK_BT_HCI_ERR_INVALID_LMP_PARAM               =    0x1E,
	RTK_BT_HCI_ERR_UNSPECIFIED_ERROR               =    0x1F,
	RTK_BT_HCI_ERR_UNSUPPORTED_LMP_PARAM           =    0x20,
	RTK_BT_HCI_ERR_ROLE_CHANGE_NOT_ALLOWED         =    0x21,
	RTK_BT_HCI_ERR_LMP_RESPONSE_TIMEOUT            =    0x22,
	RTK_BT_HCI_ERR_LMP_ERROR_TRANS_COLLISION       =    0x23,
	RTK_BT_HCI_ERR_LMP_PDU_NOT_ALLOWED             =    0x24,
	RTK_BT_HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE     =    0x25,
	RTK_BT_HCI_ERR_UNIT_KEY_USED                   =    0x26,
	RTK_BT_HCI_ERR_QOS_NOT_SUPPORTED               =    0x27,
	RTK_BT_HCI_ERR_INSTANT_PASSED                  =    0x28,
	RTK_BT_HCI_ERR_PAIR_UNIT_KEY_NOT_SUPPORT       =    0x29,
	RTK_BT_HCI_ERR_DIFF_TRANS_COLLISION            =    0x2A,
	RTK_BT_HCI_ERR_QOS_UNACCEPTABLE_PARAM          =    0x2C,
	RTK_BT_HCI_ERR_QOS_REJECT                      =    0x2D,
	RTK_BT_HCI_ERR_CHANN_ASSESS_NOT_SUPPORT        =    0x2E,
	RTK_BT_HCI_ERR_INSUFFICIENT_SECURITY           =    0x2F,
	RTK_BT_HCI_ERR_PARAM_OUT_OF_RANGE              =    0x30,
	RTK_BT_HCI_ERR_ROLE_SWITCH_PANDING             =    0x32,
	RTK_BT_HCI_ERR_RESERVED_SLOT_VIOLATION         =    0x34,
	RTK_BT_HCI_ERR_ROLE_SWITCH_FAILED              =    0x35,
	RTK_BT_HCI_ERR_EXT_INQUIRY_RSP_TOO_LARGE       =    0x36,
	RTK_BT_HCI_ERR_SSP_NOT_SUPPORTED_BY_HOST       =    0x37,
	RTK_BT_HCI_ERR_HOST_BUSY_PAIRING               =    0x38,
	RTK_BT_HCI_ERR_REJECT_NO_SUITABLE_CHANN        =    0x39,
	RTK_BT_HCI_ERR_CONTROLLER_BUSY                 =    0x3A,
	RTK_BT_HCI_ERR_UNACCEPTABLE_CONN_INTERVAL      =    0x3B,
	RTK_BT_HCI_ERR_DIRECTED_ADV_TIMEOUT            =    0x3C,
	RTK_BT_HCI_ERR_MIC_FAILURE                     =    0x3D,
	RTK_BT_HCI_ERR_FAIL_TO_ESTABLISH_CONN          =    0x3E,
	RTK_BT_HCI_ERR_MAC_CONN_FAIL                   =    0x3F,
	RTK_BT_HCI_ERR_COARSE_CLOCK_ADJUST_REJECTED    =    0x40,
	RTK_BT_HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED        =    0x41,
	RTK_BT_HCI_ERR_UNKNOWN_ADVERTISING_IDENTIFIER  =    0x42,
	RTK_BT_HCI_ERR_LIMIT_REACHED                   =    0x43,
	RTK_BT_HCI_ERR_OPERATION_CANCELLED_BY_HOST     =    0x44,
	/* local error code */
	RTK_BT_HCI_ERR_INVALID_ADDR                    =    0x64,
	RTK_BT_HCI_ERR_TIMEOUT                         =    0x65,
	RTK_BT_HCI_ERR_OUT_OF_SYNC                     =    0x66,
	RTK_BT_HCI_ERR_NO_DESCRIPTOR                   =    0x67,
	RTK_BT_HCI_ERR_NO_MEMORY                       =    0x68,
};

/**
 * @enum      rtk_bt_err_l2cap_subgroup
 * @brief     Bluetooth API L2CAP subgroup err code.
 */
enum rtk_bt_err_l2cap_subgroup {
	RTK_BT_ERR_L2CAP_LE_COC_SUBGROUP  =   0x30,  /*!< L2CAP LE credit based err, ref @ref rtk_bt_err_l2cap_le_coc */
	RTK_BT_ERR_L2CAP_ECFC_SUBGROUP    =   0x50,  /*!< L2CAP enhanced credit based err, ref @ref rtk_bt_gap_ecfc_conn_cfm_cause_t */
};

/**
 * @enum      rtk_bt_err_l2cap_le_coc
 * @brief     Bluetooth API LE credit based connection-oriented channel(coc) err code. Details ref bluetooth spec
 */
enum rtk_bt_err_l2cap_le_coc {
	RTK_BT_LE_COC_ERR_PSM_NOT_SUPPORTED               =   0x02,
	RTK_BT_LE_COC_ERR_NO_RESOURCES                    =   0x04,
	RTK_BT_LE_COC_ERR_INSUFFICIENT_AUTHEN             =   0x05,
	RTK_BT_LE_COC_ERR_INSUFFICIENT_AUTHOR             =   0x06,
	RTK_BT_LE_COC_ERR_INSUFFICIENT_ENCRYP_KEY_SIZE    =   0x07,
	RTK_BT_LE_COC_ERR_INSUFFICIENT_ENCRYP             =   0x08,
	RTK_BT_LE_COC_ERR_INVALID_SOURCE_CID              =   0x09,
	RTK_BT_LE_COC_ERR_SOURCE_CID_ALREADY_ALLOC        =   0x0A,
};

/**
 * @enum      rtk_bt_err_att
 * @brief     Bluetooth API att sub err code. Details ref bluetooth spec
 */
enum rtk_bt_err_att {
	RTK_BT_ATT_ERR_INVALID_HANDLE               =   0x01,
	RTK_BT_ATT_ERR_READ_NOT_PERMITTED           =   0x02,
	RTK_BT_ATT_ERR_WRITE_NOT_PERMITTED          =   0x03,
	RTK_BT_ATT_ERR_INVALID_PDU                  =   0x04,
	RTK_BT_ATT_ERR_INSUFFICIENT_AUTHEN          =   0x05,
	RTK_BT_ATT_ERR_UNSUPPORTED_REQ              =   0x06,
	RTK_BT_ATT_ERR_INVALID_OFFSET               =   0x07,
	RTK_BT_ATT_ERR_INSUFFICIENT_AUTHOR          =   0x08,
	RTK_BT_ATT_ERR_PREP_QUEUE_FULL              =   0x09,
	RTK_BT_ATT_ERR_ATTR_NOT_FOUND               =   0x0A,
	RTK_BT_ATT_ERR_ATTR_NOT_LONG                =   0x0B,
	RTK_BT_ATT_ERR_INSUFFICIENT_KEY_SIZE        =   0x0C,
	RTK_BT_ATT_ERR_INVALID_VALUE_SIZE           =   0x0D,
	RTK_BT_ATT_ERR_UNLIKELY                     =   0x0E,
	RTK_BT_ATT_ERR_INSUFFICIENT_ENCRYPT         =   0x0F,
	RTK_BT_ATT_ERR_UNSUPPORTED_GROUP_TYPE       =   0x10,
	RTK_BT_ATT_ERR_INSUFFICIENT_RESOURCES       =   0x11,
	RTK_BT_ATT_ERR_DATABASE_OUT_OF_SYNC         =   0x12,
	RTK_BT_ATT_ERR_VALUE_NOT_ALLOWED            =   0x13,
	/* Application Error 0x80 - 0x9F */
	RTK_BT_ATT_ERR_INVALID_VALUE                =   0x80,
	/* Manufacturer specific error */
	RTK_BT_ATT_ERR_INVALID_CCC_BITS             =   0xC0,
	RTK_BT_ATT_ERR_INVALID_SIGNED_COUNT         =   0xC1,
	RTK_BT_ATT_ERR_INVALID_SIGNED_MAC_FAILED    =   0xC2,
	RTK_BT_ATT_ERR_MIN_APPLIC_CODE              =   0xC3,
	/* Common Profiles and Service Error 0xE0 - 0xFF */
	RTK_BT_ATT_ERR_WRITE_REQUEST_REJECTED       =   0xFC,
	RTK_BT_ATT_ERR_CCCD_IMPROPERLY_CONFIGURED   =   0xFD,
	RTK_BT_ATT_ERR_PROC_ALREADY_IN_PROGRESS     =   0xFE,
	RTK_BT_ATT_ERR_OUT_OF_RANGE                 =   0xFF,
};

/**
 * @enum      rtk_bt_err_smp
 * @brief     Bluetooth API smp sub err code. Details ref bluetooth spec
 */
enum rtk_bt_err_smp {
	/* pairing failed err code */
	RTK_BT_SMP_ERR_PASSKEY_ENTRY_FAIL           =   0x01,
	RTK_BT_SMP_ERR_OOB_NOT_AVAIABLE             =   0x02,
	RTK_BT_SMP_ERR_AUTHEN_REQUIREMENTS          =   0x03,
	RTK_BT_SMP_ERR_CFM_VALUE_FAILED             =   0x04,
	RTK_BT_SMP_ERR_PAIRING_NOT_SUPPORTED        =   0x05,
	RTK_BT_SMP_ERR_ENCRYPTION_KEY_SIZE          =   0x06,
	RTK_BT_SMP_ERR_CMD_NOT_SUPPORTED            =   0x07,
	RTK_BT_SMP_ERR_UNSPECIFIED_REASON           =   0x08,
	RTK_BT_SMP_ERR_REPEATED_ATTEMPTS            =   0x09,
	RTK_BT_SMP_ERR_INVALID_PARAM                =   0x0A,
	RTK_BT_SMP_ERR_DHKEY_CHECK_FAILED           =   0x0B,
	RTK_BT_SMP_ERR_NUMERIC_COMPARISION_FAIL     =   0x0C,
	RTK_BT_SMP_ERR_BREDR_PAIRING_IN_PROGRESS    =   0x0D,
	RTK_BT_SMP_ERR_CRS_TRANS_KEY_GEN_NOT_ALLOW  =   0x0E,
	/* local err code */
	RTK_BT_SM_ERR_NO_ENTRY                      =   0xE1,
	RTK_BT_SM_ERR_DB_FULL                       =   0xE2,
	RTK_BT_SM_ERR_INVALID_PARAM                 =   0xE3,
	RTK_BT_SM_ERR_INSUFFICIENT_LINK_KEY         =   0xE4,
	RTK_BT_SM_ERR_LE_ADDR_NOT_RESOLVED          =   0xE5,
	RTK_BT_SM_ERR_INVALID_STATE                 =   0xE6,
	RTK_BT_SM_ERR_NO_RESOURCE                   =   0xE7,
	RTK_BT_SM_ERR_LINK_KEY_MISSING              =   0xE8,
	RTK_BT_SM_ERR_DISCONNECT                    =   0xE9,
	RTK_BT_SM_ERR_PARING_NOT_ALLOWED            =   0xEA,
	RTK_BT_SM_ERR_KEY_SAVE_FAILED               =   0xEB,
	RTK_BT_SM_ERR_TIMEOUT                       =   0xEC,
	RTK_BT_SM_ERR_UNKNOWN                       =   0xED,
	RTK_BT_SM_ERR_NO_PAIRABLE_MODE              =   0xEF,
	RTK_BT_SM_ERR_VENDOR                        =   0xFF,
};

/**
 * @enum      rtk_bt_err_gatt
 * @brief     Bluetooth API gatt sub err code.
 */
enum rtk_bt_err_gatt {
	RTK_BT_GATT_ERR_OUT_OF_RESOURCE        =    0x01,
	RTK_BT_GATT_ERR_UNSUPPORTED            =    0x02,
	RTK_BT_GATT_ERR_INVALID_ROLE           =    0x03,
	RTK_BT_GATT_ERR_INVALID_STATE          =    0x04,
	RTK_BT_GATT_ERR_INVALID_CODING         =    0x05,
	RTK_BT_GATT_ERR_INVALID_HANDLE         =    0x06,
	RTK_BT_GATT_ERR_INVALID_PARAM          =    0x07,
	RTK_BT_GATT_ERR_INTERNAL               =    0x08,
	RTK_BT_GATT_ERR_NOT_ALLOWED            =    0x09,
	RTK_BT_GATT_ERR_NOTIF_IND_NOT_CFG      =    0x0A,
	RTK_BT_GATT_ERR_NOTIF_IND_CFG          =    0x0B,
	RTK_BT_GATT_ERR_NOTIF_IND_CONF_PD      =    0x0C,
	RTK_BT_GATT_ERR_TIMEOUT                =    0x0D,
	RTK_BT_GATT_ERR_LINK_DEACTIVATED       =    0x0E,
	RTK_BT_GATT_ERR_NOT_AUTHENTICATED      =    0x0F,
	RTK_BT_GATT_ERR_NOT_ENCRYPTED          =    0x10,
	RTK_BT_GATT_ERR_PROC_FAIL              =    0x11,
};

/**
 * @defgroup  BT_APIs BT APIs
 * @brief     List all BT related APIs
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_DEF_H__ */
