/**
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_TYPES_H_
#define _BT_TYPES_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2019

#ifdef __cplusplus
extern "C" {
#endif

#include <bt_flags.h>
/**
 * \defgroup    BTTYPES     BT Types
 *
 * \brief   Defines BT related macros for the upper layers.
 *
 */


/**
 * bt_types.h
 *
 * \name    BT_LE_LOCAL_SUPPORTED_FEATURES
 * \brief   BT LE Local Supported Features definitions.
 * \anchor  BT_LE_LOCAL_SUPPORTED_FEATURES
 */
/**
 * \ingroup     BTTYPES
 */
#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX0                           0

#define LE_SUPPORT_FEATURES_ENCRYPTION_MASK_BIT                         ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_CONNPARA_REQ_PROC_MASK_BIT                  ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_EXTENDED_REJECTION_IND_MASK_BIT             ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_SLAVE_INITIATED_FEATURE_EXCHANGE_MASK_BIT   ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_LE_PING_MASK_BIT                            ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_LE_DATA_LENGTH_EXTENSION_MASK_BIT           ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_LL_PRIVACY_MASK_BIT                         ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_EXTENDED_SCANNER_FILTER_POLICY_MASK_BIT     ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1                           1

#define LE_SUPPORT_FEATURES_LE_2M_MASK_BIT                              ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_STABLE_MODULATION_INDEX_TX_MASK_BIT         ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_STABLE_MODULATION_INDEX_RX_MASK_BIT         ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_LE_CODED_PHY_MASK_BIT                       ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_LE_EXTENDED_ADV_BIT                         ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_LE_PERIODIC_ADV_MASK_BIT                    ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_CSA2_BIT                                    ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_LE_POWER_CLASS1_MASK_BIT                    ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX2                           2

#define LE_SUPPORT_FEATURES_MIN_NUM_USED_CHANNEL_MASK_BIT               ((uint8_t)0x01)

/**
 * bt_types.h
 *
 * \name    BT_STACK_MODULE_ERROR
 * \brief   BT stack module error code mask.
 * \anchor  BT_STACK_MODULE_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define GAP_ERR                 0x0000
#define HCI_ERR                 0x0100
#define L2C_ERR                 0x0200
#define SDP_ERR                 0x0300
#define ATT_ERR                 0x0400
#define SM_ERR                  0x0500

#define GATT_ERR                0x0C00
#define APP_ERR                 0x0D00

/**
 * bt_types.h
 *
 * \name    BT_HCI_ERROR
 * \brief   BT hci error code definitions.
 * \anchor  BT_HCI_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define HCI_SUCCESS                             0x00
#define HCI_ERR_UNKNOWN_CMD                     0x01
#define HCI_ERR_UNKNOWN_CONN_ID                 0x02
#define HCI_ERR_HARDWARE_FAIL                   0x03
#define HCI_ERR_PAGE_TIMEOUT                    0x04
#define HCI_ERR_AUTHEN_FAIL                     0x05
#define HCI_ERR_KEY_MISSING                     0x06
#define HCI_ERR_MEMORY_FULL                     0x07
#define HCI_ERR_CONN_TIMEOUT                    0x08
#define HCI_ERR_MAX_NUM_CONN                    0x09
#define HCI_ERR_MAX_NUM_SCO_CONN                0x0A  /* 10 */
#define HCI_ERR_ACL_CONN_EXIST                  0x0B  /* 11 */
#define HCI_ERR_CMD_DISALLOWED                  0x0C  /* 12 */
#define HCI_ERR_REJECT_LIMITED_RESOURCE         0x0D  /* 13 */
#define HCI_ERR_REJECT_SECURITY_REASON          0x0E  /* 14 */
#define HCI_ERR_REJECT_UNACCEPTABLE_ADDR        0x0F  /* 15 */
#define HCI_ERR_HOST_TIMEOUT                    0x10  /* 16 */
#define HCI_ERR_UNSUPPORTED_PARAM               0x11  /* 17 */
#define HCI_ERR_INVALID_PARAM                   0x12  /* 18 */
#define HCI_ERR_REMOTE_USER_TERMINATE           0x13  /* 19 */
#define HCI_ERR_REMOTE_LOW_RESOURCE             0x14  /* 20 */
#define HCI_ERR_REMOTE_POWER_OFF                0x15  /* 21 */
#define HCI_ERR_LOCAL_HOST_TERMINATE            0x16  /* 22 */
#define HCI_ERR_REPEATED_ATTEMPTS               0x17  /* 23 */
#define HCI_ERR_PARING_NOT_ALLOWED              0x18  /* 24 */
#define HCI_ERR_UNKNOWN_LMP_PDU                 0x19  /* 25 */
#define HCI_ERR_UNSUPPORTED_REMOTE_FEAT         0x1A  /* 26 */
#define HCI_ERR_SCO_OFFSET_REJECTED             0x1B  /* 27 */
#define HCI_ERR_SCO_INTERVAL_REJECTED           0x1C  /* 28 */
#define HCI_ERR_SCO_AIR_MODE_REJECTED           0x1D  /* 29 */
#define HCI_ERR_INVALID_LMP_PARAM               0x1E  /* 30 */
#define HCI_ERR_UNSPECIFIED_ERROR               0x1F  /* 31 */
#define HCI_ERR_UNSUPPORTED_LMP_PARAM           0x20  /* 32 */
#define HCI_ERR_ROLE_CHANGE_NOT_ALLOWED         0x21  /* 33 */
#define HCI_ERR_LMP_RESPONSE_TIMEOUT            0x22  /* 34 */
#define HCI_ERR_LMP_ERROR_TRANS_COLLISION       0x23  /* 35 */
#define HCI_ERR_LMP_PDU_NOT_ALLOWED             0x24  /* 36 */
#define HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE     0x25  /* 37 */
#define HCI_ERR_UNIT_KEY_USED                   0x26  /* 38 */
#define HCI_ERR_QOS_NOT_SUPPORTED               0x27  /* 39 */
#define HCI_ERR_INSTANT_PASSED                  0x28  /* 40 */
#define HCI_ERR_PAIR_UNIT_KEY_NOT_SUPPORT       0x29  /* 41 */
#define HCI_ERR_DIFF_TRANS_COLLISION            0x2A  /* 42 */
#define HCI_ERR_QOS_UNACCEPTABLE_PARAM          0x2C  /* 44 */
#define HCI_ERR_QOS_REJECT                      0x2D  /* 45 */
#define HCI_ERR_CHANN_ASSESS_NOT_SUPPORT        0x2E  /* 46 */
#define HCI_ERR_INSUFFICIENT_SECURITY           0x2F  /* 47 */
#define HCI_ERR_PARAM_OUT_OF_RANGE              0x30  /* 48 */
#define HCI_ERR_ROLE_SWITCH_PANDING             0x32  /* 50 */
#define HCI_ERR_RESERVED_SLOT_VIOLATION         0x34  /* 52 */
#define HCI_ERR_ROLE_SWITCH_FAILED              0x35  /* 53 */
#define HCI_ERR_EXT_INQUIRY_RSP_TOO_LARGE       0x36  /* 54 */
#define HCI_ERR_SSP_NOT_SUPPORTED_BY_HOST       0x37  /* 55 */
#define HCI_ERR_HOST_BUSY_PAIRING               0x38  /* 56 */
#define HCI_ERR_REJECT_NO_SUITABLE_CHANN        0x39  /* 57 */
#define HCI_ERR_CONTROLLER_BUSY                 0x3A  /* 58 */
#define HCI_ERR_UNACCEPTABLE_CONN_INTERVAL      0x3B  /* 59 */
#define HCI_ERR_DIRECTED_ADV_TIMEOUT            0x3C  /* 60 */
#define HCI_ERR_MIC_FAILURE                     0x3D  /* 61 */
#define HCI_ERR_FAIL_TO_ESTABLISH_CONN          0x3E  /* 62 */
#define HCI_ERR_MAC_CONN_FAIL                   0x3F  /* 63 */
#define HCI_ERR_COARSE_CLOCK_ADJUST_REJECTED    0x40
#define HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED        0x41
#define HCI_ERR_UNKNOWN_ADVERTISING_IDENTIFIER  0x42
#define HCI_ERR_LIMIT_REACHED                   0x43
#define HCI_ERR_OPERATION_CANCELLED_BY_HOST     0x44

/* hci local error code */
#define HCI_ERR_INVALID_ADDR                    0x64  /* 100 */
#define HCI_ERR_TIMEOUT                         0x65  /* 101 */
#define HCI_ERR_OUT_OF_SYNC                     0x66  /* 102 */
#define HCI_ERR_NO_DESCRIPTOR                   0x67  /* 103 */
#define HCI_ERR_NO_MEMORY                       0x68

/**
 * bt_types.h
 *
 * \name    BT_L2CAP_ERROR
 * \brief   BT l2cap error code definitions.
 * \anchor  BT_L2CAP_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define L2C_SUCCESS                             0x00

#define L2C_CONN_ACCEPT                         0x0000
#define L2C_CONN_RSP_PENDING                    0x0001
#define L2C_CONN_RSP_INVALID_PSM                0x0002
#define L2C_CONN_RSP_SECURITY_BLOCK             0x0003
#define L2C_CONN_RSP_NO_RESOURCE                0x0004
#define L2C_CONN_RSP_INVALID_PARAM              0x0005
#define L2C_CONN_RSP_INVALID_SOURCE_CID         0x0006
#define L2C_CONN_RSP_SOURCE_CID_ALLOCATED       0x0007

/* requset cmd reject reasons */
#define L2C_CMD_REJ_NOT_UNDERSTOOD              0x0000
#define L2C_CMD_REJ_SIGNAL_MTU_EXCEEDED         0x0001
#define L2C_CMD_REJ_INVALID_CID_IN_REQ          0x0002

/* config response result codes */
#define L2C_CFG_RSP_SUCCESS                     0x0000
#define L2C_CFG_RSP_UNACCEPTABLE_PARAMS         0x0001
#define L2C_CFG_RSP_REJECTED                    0x0002
#define L2C_CFG_RSP_UNKNOWN_OPTIONS             0x0003
#define L2C_CFG_RSP_PENDING                     0x0004
#define L2C_CFG_RSP_FLOW_SPEC_REJECTED          0x0005

#define L2C_LE_CREDIT_CONN_SUCCESS                      0x0000
#define L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED             0x0002
#define L2C_LE_CREDIT_RSP_NO_RESOURCES                  0x0004
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN           0x0005
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR           0x0006
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE  0x0007
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP           0x0008
#define L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID            0x0009
#define L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC      0x000A

/* status for connection updata response */
#define L2C_LE_CONN_UPDATE_ACCEPT               0x0000
#define L2C_LE_CONN_UPDATE_RSP_REJECT           0x0001
#define L2C_LE_CONN_UPDATE_RSP_TOUT             0x0002

/* l2cap error code */
#define L2C_ERR_REJ_BASE                        0x0010
#define L2C_ERR_CFG_BASE                        0x0020
#define L2C_ERR_LE_CREDIT_BASE                  0x0030
#define L2C_ERR_LE_CONN_UPDATE_BASE             0x0040
#define L2C_ERR_VND_BASE                        0x00F0

#define L2C_ERR_PENDING                         L2C_CONN_RSP_PENDING
#define L2C_ERR_INVALID_PSM                     L2C_CONN_RSP_INVALID_PSM
#define L2C_ERR_SECURITY_BLOCK                  L2C_CONN_RSP_SECURITY_BLOCK
#define L2C_ERR_NO_RESOURCE                     L2C_CONN_RSP_NO_RESOURCE
#define L2C_ERR_INVALID_PARAM                   L2C_CONN_RSP_INVALID_PARAM
#define L2C_ERR_INVALID_SOURCE_CID              L2C_CONN_RSP_INVALID_SOURCE_CID
#define L2C_ERR_SOURCE_CID_ALLOCATED            L2C_CONN_RSP_SOURCE_CID_ALLOCATED

#define L2C_ERR_CMD_NOT_UNDERSTOOD              (L2C_ERR_REJ_BASE + L2C_CMD_REJ_NOT_UNDERSTOOD)
#define L2C_ERR_SIGNAL_MTU_EXCEEDED             (L2C_ERR_REJ_BASE + L2C_CMD_REJ_SIGNAL_MTU_EXCEEDED)
#define L2C_ERR_INVALID_CID_IN_REQ              (L2C_ERR_REJ_BASE + L2C_CMD_REJ_INVALID_CID_IN_REQ)

#define L2C_ERR_CFG_UNACCEPTABLE_PARAM          (L2C_ERR_CFG_BASE + L2C_CFG_RSP_UNACCEPTABLE_PARAMS)
#define L2C_ERR_CFG_REJECTED                    (L2C_ERR_CFG_BASE + L2C_CFG_RSP_REJECTED)
#define L2C_ERR_CFG_UNKNOWN_OPTIONS             (L2C_ERR_CFG_BASE + L2C_CFG_RSP_UNKNOWN_OPTIONS)
#define L2C_ERR_CFG_PENDING                     (L2C_ERR_CFG_BASE + L2C_CFG_RSP_PENDING)
#define L2C_ERR_CFG_FLOW_SPEC_REJECTED          (L2C_ERR_CFG_BASE + L2C_CFG_RSP_FLOW_SPEC_REJECTED)

#define L2C_ERR_LE_CREDIT_PSM_NOT_SUPPORTED             (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED)
#define L2C_ERR_LE_CREDIT_NO_RESOURCES                  (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_NO_RESOURCES)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_AUTHEN           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_AUTHOR           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_ENCRYP_KEY_SIZE  (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_ENCRYP           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP)
#define L2C_ERR_LE_CREDIT_INVALID_SOURCE_CID            (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID)
#define L2C_ERR_LE_CREDIT_SOURCE_CID_ALREADY_ALLOC      (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC)

#define L2C_ERR_LE_CONN_PARAM_UPDATE_REJECT     (L2C_ERR_LE_CONN_UPDATE_BASE + L2C_LE_CONN_UPDATE_RSP_REJECT)
#define L2C_ERR_LE_CONN_PARAM_UPDATE_TOUT       (L2C_ERR_LE_CONN_UPDATE_BASE + L2C_LE_CONN_UPDATE_RSP_TOUT)

#define L2C_ERR_VND_CREDITS_EXCEED              (L2C_ERR_VND_BASE + 0x00)
#define L2C_ERR_VND_INVAILD_PDU                 (L2C_ERR_VND_BASE + 0x01)
#define L2C_ERR_VND_CREDITS_LACK                (L2C_ERR_VND_BASE + 0x02)
#define L2C_ERR_VND_NO_MEMORY                   (L2C_ERR_VND_BASE + 0x03)
#define L2C_ERR_VND_INVALID_STATE               (L2C_ERR_VND_BASE + 0x04)
#define L2C_ERR_VND_INVALID_RX_SEQ              (L2C_ERR_VND_BASE + 0x05)
#define L2C_ERR_VND_TIMEOUT                     (L2C_ERR_VND_BASE + 0x06)
#define L2C_ERR_VND_INVALID_MODE                (L2C_ERR_VND_BASE + 0x07)
#define L2C_ERR_VND_REMOTE_DISCONNECT           (L2C_ERR_VND_BASE + 0x08)

/**
 * bt_types.h
 *
 * \name    BT_ATT_ERROR
 * \brief   BT att error code definitions.
 * \anchor  BT_ATT_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define ATT_SUCCESS                         0    /**< internal value .. */
#define ATT_ERR_INVALID_HANDLE              0x01 /**< Attribute handle value given was not valid on this attribute server */
#define ATT_ERR_READ_NOT_PERMITTED          0x02 /**< Attribute cannot be read */
#define ATT_ERR_WRITE_NOT_PERMITTED         0x03 /**< Attribute cannot be written */
#define ATT_ERR_INVALID_PDU                 0x04 /**< The attribute PDU was invalid */
#define ATT_ERR_INSUFFICIENT_AUTHEN         0x05 /**< The attribute requires authentication before it can be read or written */
#define ATT_ERR_UNSUPPORTED_REQ             0x06 /**< Attribute server doesn't support the request received from the attribute client */
#define ATT_ERR_INVALID_OFFSET              0x07 /**< Offset specified was past the end of the attribute */
#define ATT_ERR_INSUFFICIENT_AUTHOR         0x08 /**< The attribute requires an authorization before it can be read or written */
#define ATT_ERR_PREP_QUEUE_FULL             0x09 /**< Too many prepare writes have been queued */
#define ATT_ERR_ATTR_NOT_FOUND              0x0A /**< No attribute found within the given attribute handle range */
#define ATT_ERR_ATTR_NOT_LONG               0x0B /**< Attribute cannot be read or written using the Read Blob Request or Prepare Write Request */
#define ATT_ERR_INSUFFICIENT_KEY_SIZE       0x0C /**< The Encryption Key Size used for encrypting this link is insufficient */
#define ATT_ERR_INVALID_VALUE_SIZE          0x0D /**< The attribute value length is invalid for the operation */
#define ATT_ERR_UNLIKELY                    0x0E /**< The attribute request that was requested has encountered an error that was very unlikely, and therefore could not be completed as requested */
#define ATT_ERR_INSUFFICIENT_ENCRYPT        0x0F /**< The attribute requires encryption before it can be read or written */
#define ATT_ERR_UNSUPPORTED_GROUP_TYPE      0x10 /**< The attribute type is not a supported grouping attribute as defined by a higher layer specification */
#define ATT_ERR_INSUFFICIENT_RESOURCES      0x11 /**< Insufficient Resources to complete the request */

/* profile dependent application error codes >= 0x80: */
#define ATT_ERR_MIN_APPLIC_CODE             0x80 /**< minimal application error code */
#define ATT_ERR_INVALID_VALUE               0x80 /**< The attribute value is invalid for the operation */

/* manufacturer specific error codes that are "missing" in GATT spec. >= 0xC0:   */
#define ATT_ERR_INVALID_CCC_BITS            0xC0 /**< Invalid client char. config. bits */
#define ATT_ERR_INVALID_SIGNED_COUNT        0xC1 /**< Invalid sign count */
#define ATT_ERR_INVALID_SIGNED_MAC_FAILED   0xC2 /**< Invalid sign mac value */

/* error codes common to various profiles (see "CSS v2.pdf"), >= 0xE0 */
#define ATT_ERR_CCCD_IMPROPERLY_CONFIGURED  0xFD /**< CCCD improperly configured */
#define ATT_ERR_PROC_ALREADY_IN_PROGRESS    0xFE /**< Procedure Already in Progress */
#define ATT_ERR_OUT_OF_RANGE                0xFF /**< Client Characteristic Configuration Descriptor Improperly Configured */

/**
 * bt_types.h
 *
 * \name    BT_GATT_ERROR
 * \brief   BT gatt error code definitions.
 * \anchor  BT_GATT_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define GATT_SUCCESS                        0x00
#define GATT_ERR_OUT_OF_RESOURCE            0x01
#define GATT_ERR_UNSUPPORTED                0x02
#define GATT_ERR_INVALID_ROLE               0x03
#define GATT_ERR_INVALID_STATE              0x04
#define GATT_ERR_INVALID_CODING             0x05
#define GATT_ERR_INVALID_HANDLE             0x06
#define GATT_ERR_INVALID_PARAM              0x07
#define GATT_ERR_INTERNAL                   0x08
#define GATT_ERR_NOT_ALLOWED                0x09
#define GATT_ERR_NOTIF_IND_NOT_CFG          0x0A
#define GATT_ERR_NOTIF_IND_CFG              0x0B
#define GATT_ERR_NOTIF_IND_CONF_PD          0x0C
#define GATT_ERR_TIMEOUT                    0x0D
#define GATT_ERR_LINK_DEACTIVATED           0x0E
#define GATT_ERR_NOT_AUTHENTICATED          0x0F
#define GATT_ERR_NOT_ENCRYPTED              0x10
#define GATT_ERR_PROC_FAIL                  0x11

/* sm error code */
#define SM_SUCCESS                          0x00
/* SMP_OP_PAIRING_FAILED causes */
#define SMP_ERR_SUCCESS                         0x00
#define SMP_ERR_PASSKEY_ENTRY_FAIL              0x01
#define SMP_ERR_OOB_NOT_AVAIABLE                0x02
#define SMP_ERR_AUTHEN_REQUIREMENTS             0x03
#define SMP_ERR_CFM_VALUE_FAILED                0x04
#define SMP_ERR_PAIRING_NOT_SUPPORTED           0x05
#define SMP_ERR_ENCRYPTION_KEY_SIZE             0x06
#define SMP_ERR_CMD_NOT_SUPPORTED               0x07
#define SMP_ERR_UNSPECIFIED_REASON              0x08
#define SMP_ERR_REPEATED_ATTEMPTS               0x09
#define SMP_ERR_INVALID_PARAM                   0x0A
#define SMP_ERR_DHKEY_CHECK_FAILED              0x0B
#define SMP_ERR_NUMERIC_COMPARISION_FAIL        0x0C
#define SMP_ERR_BREDR_PAIRING_IN_PROGRESS       0x0D
#define SMP_ERR_CRS_TRANS_KEY_GEN_NOT_ALLOW     0x0E

#define SM_ERR_NO_ENTRY                         0xE1
#define SM_ERR_DB_FULL                          0xE2
#define SM_ERR_INVALID_PARAM                    0xE3
#define SM_ERR_INSUFFICIENT_LINK_KEY            0xE4
#define SM_ERR_LE_ADDR_NOT_RESOLVED             0xE5
#define SM_ERR_INVALID_STATE                    0xE6
#define SM_ERR_NO_RESOURCE                      0xE7
#define SM_ERR_LINK_KEY_MISSING                 0xE8
#define SM_ERR_DISCONNECT                       0xE9
#define SM_ERR_PARING_NOT_ALLOWED               0xEA
#define SM_ERR_KEY_SAVE_FAILED                  0xEB
#define SM_ERR_TIMEOUT                          0xEC
#define SM_ERR_UNKNOWN                          0xED
#define SM_ERR_NO_PAIRABLE_MODE                 0xEF
#define SM_ERR_VENDOR                           0xFF

/**
 * bt_types.h
 *
 * \name    BT_BTIF_ERROR
 * \brief   BT btif error code definitions.
 * \anchor  BT_BTIF_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define BTIF_ERR                0x0000
#define BTIF_SUCCESS                        0x00
#define BTIF_ACCEPT                         0x01
#define BTIF_ERR_REJECT                     0x02
#define BTIF_ERR_NO_RESOURCE                0x03
#define BTIF_ERR_INVALID_PARAM              0x04
#define BTIF_ERR_INVALID_STATE              0x05
#define BTIF_ERR_CONN_DISCONNECT            0x06
#define BTIF_ERR_CONN_LOST                  0x07
#define BTIF_ERR_AUTHEN_FAIL                0x08
#define BTIF_ERR_INIT_TOUT                  0x09
#define BTIF_ERR_INIT_OUT_OF_SYNC           0x0A
#define BTIF_ERR_INIT_HARDWARE_ERROR        0x0B
#define BTIF_ERR_LOWER_LAYER_ERROR          0x0C
#define BTIF_ERR_ADDR_NOT_RESOLVED          0x0D
#define BTIF_ERR_TOUT                       0x0E

#define BTIF_ERR_UNSPECIFIED                0xFD
#define BTIF_ERR_NOT_SUPPORTED              0xFE

/**
 * bt_types.h
 *
 * \name    BT_GAP_ERROR
 * \brief   BT GAP error code definitions.
 * \anchor  BT_GAP_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define GAP_SUCCESS                         0x00
#define GAP_ACCEPT                          0x01
#define GAP_ERR_REJECT                      0x02
#define GAP_ERR_NO_RESOURCE                 0x03
#define GAP_ERR_INVALID_PARAM               0x04
#define GAP_ERR_INVALID_STATE               0x05
#define GAP_ERR_CONN_DISCONNECT             0x06
#define GAP_ERR_CONN_LOST                   0x07
#define GAP_ERR_AUTHEN_FAIL                 0x08
#define GAP_ERR_INIT_TOUT                   0x09
#define GAP_ERR_INIT_OUT_OF_SYNC            0x0A
#define GAP_ERR_INIT_HARDWARE_ERROR         0x0B
#define GAP_ERR_LOWER_LAYER_ERROR           0x0C
#define GAP_ERR_ADDR_NOT_RESOLVED           0x0D
#define GAP_ERR_TOUT                        0x0E

#define GAP_ERR_SW_RESET                    0xFC
#define GAP_ERR_UNSPECIFIED                 0xFD
#define GAP_ERR_NOT_SUPPORTED               0xFE

/**
 * bt_types.h
 *
 * \name    BT_APP_ERROR
 * \brief   BT APP error code definitions.
 * \anchor  BT_APP_ERROR
 */
/**
 * \ingroup     BTTYPES
 */
#define APP_SUCCESS                             0x00
#define APP_ERR_PENDING                         0x01
#define APP_ERR_ACCEPT                          0x03
#define APP_ERR_REJECT                          0x04
#define APP_ERR_NOT_RELEASE                     0x05

/**
 * bt_types.h
 *
 * \name    BT_BYTE_ORDER
 * \brief   BT buffer/array byte-order utility macros.
 * \anchor  BT_BYTE_ORDER
 */
/**
 * \ingroup     BTTYPES
 */
/** Calculate integer bit count of b'1 */
#define INT_BIT_COUNT(integer, count)   {               \
        count = 0;                                      \
        while (integer)                                 \
        {                                               \
            count++;                                    \
            integer &= integer - 1;                     \
        }                                               \
    }

/** Stream skip len */
#define STREAM_SKIP_LEN(s, len)     {                   \
        s += len;                                       \
    }

/** Stream to array */
#define STREAM_TO_ARRAY(a, s, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *((uint8_t *)(a) + ii) = *s++;              \
        }                                               \
    }

/** Array to stream */
#define ARRAY_TO_STREAM(s, a, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *s++ = *((uint8_t *)(a) + ii);              \
        }                                               \
    }

/** Little Endian stream to uint8 */
#define LE_STREAM_TO_UINT8(u8, s)   {                   \
        u8  = (uint8_t)(*s);                            \
        s  += 1;                                        \
    }

/** Little Endian stream to uint16 */
#define LE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 0) +            \
               ((uint16_t)(*(s + 1)) << 8);             \
        s   += 2;                                       \
    }

/** Little Endian stream to uint24 */
#define LE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16);            \
        s   += 3;                                       \
    }

/** Little Endian stream to uint32 */
#define LE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16) +           \
               ((uint32_t)(*(s + 3)) << 24);            \
        s   += 4;                                       \
    }

/** Little Endian uint8 to stream */
#define LE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Little Endian uint16 to stream */
#define LE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
    }

/** Little Endian uint24 to stream */
#define LE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >>  0);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >> 16);                  \
    }

/** Little Endian uint32 to stream */
#define LE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >>  0);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >> 24);                  \
    }

/** Little Endian array to uint8 */
#define LE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Little Endian array to uint16 */
#define LE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 0) +             \
              ((uint16_t)(*(a + 1)) << 8);              \
    }

/** Little Endian array to uint24 */
#define LE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16);             \
    }

/** Little Endian array to uint32 */
#define LE_ARRAY_TO_UINT32(u32, a) {                    \
        u32 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16) +            \
              ((uint32_t)(*(a + 3)) << 24);             \
    }

/** Little Endian uint8 to array */
#define LE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Little Endian uint16 to array */
#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

/** Little Endian uint24 to array */
#define LE_UINT24_TO_ARRAY(a, u24) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >> 16); \
    }

/** Little Endian uint32 to array */
#define LE_UINT32_TO_ARRAY(a, u32) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >> 24); \
    }

/** Big Endian stream to uint8 */
#define BE_STREAM_TO_UINT8(u8, s)   {                   \
        u8   = (uint8_t)(*(s + 0));                     \
        s   += 1;                                       \
    }

/** Big Endian stream to uint16 */
#define BE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 8) +            \
               ((uint16_t)(*(s + 1)) << 0);             \
        s   += 2;                                       \
    }

/** Big Endian stream to uint24 */
#define BE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) << 16) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) <<  0);            \
        s   += 3;                                       \
    }

/** Big Endian stream to uint32 */
#define BE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) << 24) +           \
               ((uint32_t)(*(s + 1)) << 16) +           \
               ((uint32_t)(*(s + 2)) <<  8) +           \
               ((uint32_t)(*(s + 3)) <<  0);            \
        s   += 4;                                       \
    }

/** Big Endian uint8 to stream */
#define BE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Big Endian uint16 to stream */
#define BE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
    }

/** Big Endian uint24 to stream */
#define BE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >> 16);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >>  0);                  \
    }

/** Big Endian uint32 to stream */
#define BE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >> 24);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >>  0);                  \
    }

/** Big Endian array to uint8 */
#define BE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Big Endian array to uint16 */
#define BE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 8) +             \
              ((uint16_t)(*(a + 1)) << 0);              \
    }

/** Big Endian array to uint24 */
#define BE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) << 16) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) <<  0);             \
    }

/** Big Endian array to uint32 */
#define BE_ARRAY_TO_UINT32(u32, a)  {                   \
        u32 = ((uint32_t)(*(a + 0)) << 24) +            \
              ((uint32_t)(*(a + 1)) << 16) +            \
              ((uint32_t)(*(a + 2)) <<  8) +            \
              ((uint32_t)(*(a + 3)) <<  0);             \
    }

/** Big Endian uint8 to array */
#define BE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Big Endian uint16 to array */
#define BE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 8);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 0);  \
    }

/** Big Endian uint24 to array */
#define BE_UINT24_TO_ARRAY(a, u24)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >> 16); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >>  0); \
    }

/** Big Endian uint32 to array */
#define BE_UINT32_TO_ARRAY(a, u32)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >> 24); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >>  0); \
    }

#ifdef __cplusplus
}
#endif

#elif UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C" {
#endif

#include "upperstack_config.h"

/**
 * \defgroup    BT_TYPES    BT Types
 *
 * \brief   Defines BT related macros for the upper layers.
 *
 */

/**
 * bt_types.h
 *
 * \name    BT_COD_MAJOR_CLASS
 * \brief   BT Major device class definitions.
 * \anchor  BT_COD_MAJOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MAJOR_DEVICE_CLASS_MISCELLANEOUS          ((uint32_t)0x000000)
#define MAJOR_DEVICE_CLASS_COMPUTER               ((uint32_t)0x000100)
#define MAJOR_DEVICE_CLASS_PHONE                  ((uint32_t)0x000200)
#define MAJOR_DEVICE_CLASS_LAN_ACCESS_POINT       ((uint32_t)0x000300)
#define MAJOR_DEVICE_CLASS_AUDIO                  ((uint32_t)0x000400)
#define MAJOR_DEVICE_CLASS_PERIPHERAL             ((uint32_t)0x000500)
#define MAJOR_DEVICE_CLASS_IMAGING                ((uint32_t)0x000600)
#define MAJOR_DEVICE_CLASS_HEALTH                 ((uint32_t)0x000900)
#define MAJOR_DEVICE_CLASS_UNCLASSIFIED           ((uint32_t)0x001F00)

/**
 * bt_types.h
 *
 * \name    BT_UNCLASSIFIED_MINOR_CLASS
 * \brief   BT Unclassified minor device class definition.
 * \anchor  BT_UNCLASSIFIED_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_CLASS_UNCLASSIFIED           ((uint32_t)0x000000)

/**
 * bt_types.h
 *
 * \name    BT_COMPUTER_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class COMPUTER.
 * \anchor  BT_COMPUTER_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_CLASS_DESKTOP_WORKSTATION    ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_SERVER_CLASS_COMPUTER  ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_LAPTOP                 ((uint32_t)0x00000C)
#define MINOR_DEVICE_CLASS_HANDHELD               ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_PALM_SIZED             ((uint32_t)0x000014)
#define MINOR_DEVICE_CLASS_WEARABLE_COMPUTER      ((uint32_t)0x000018)

/**
 * bt_types.h
 *
 * \name    BT_PHONE_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class PHONE.
 * \anchor  BT_PHONE_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_CLASS_CELLULAR               ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_CORDLESS               ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_SMART_PHONE            ((uint32_t)0x00000C)
#define MINOR_DEVICE_CLASS_WIRED_MODEM            ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_VOICE_GATEWAY          ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_ISDN_GATEWAY           ((uint32_t)0x000014)

/**
 * bt_types.h
 *
 * \name    BT_LAN_ACCESS_POINT_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class LAN_ACCESS_POINT.
 * \anchor  BT_LAN_ACCESS_POINT_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_CLASS_FULLY_AVAILABLE        ((uint32_t)0x000000)
#define MINOR_DEVICE_CLASS_1_17_PERCENT_UTILIZED  ((uint32_t)0x000020)
#define MINOR_DEVICE_CLASS_17_33_PERCENT_UTILIZED ((uint32_t)0x000040)
#define MINOR_DEVICE_CLASS_33_50_PERCENT_UTILIZED ((uint32_t)0x000060)
#define MINOR_DEVICE_CLASS_50_67_PERCENT_UTILIZED ((uint32_t)0x000080)
#define MINOR_DEVICE_CLASS_67_83_PERCENT_UTILIZED ((uint32_t)0x0000A0)
#define MINOR_DEVICE_CLASS_83_99_PERCENT_UTILIZED ((uint32_t)0x0000C0)
#define MINOR_DEVICE_CLASS_NO_SERVICE_AVAILABLE   ((uint32_t)0x0000E0)

/**
 * bt_types.h
 *
 * \name    BT_AV_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class AUDIO/VIDEO.
 * \anchor  BT_AV_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_CLASS_HEADSET                ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_HANDSFREE              ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_MICROPHONE             ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_LOUDSPEAKER            ((uint32_t)0x000014)
#define MINOR_DEVICE_CLASS_HEADPHONES             ((uint32_t)0x000018)
#define MINOR_DEVICE_CLASS_PORTABLEAUDIOLE        ((uint32_t)0x00001c)
#define MINOR_DEVICE_CLASS_CARAUDIO               ((uint32_t)0x000020)
#define MINOR_DEVICE_CLASS_SETTOPBOX              ((uint32_t)0x000024)
#define MINOR_DEVICE_CLASS_HIFIAUDIO              ((uint32_t)0x000028)
#define MINOR_DEVICE_CLASS_VCR                    ((uint32_t)0x00002c)
#define MINOR_DEVICE_CLASS_VIDEOCAMERA            ((uint32_t)0x000030)
#define MINOR_DEVICE_CLASS_CAMCORDER              ((uint32_t)0x000034)
#define MINOR_DEVICE_CLASS_VIDEOMONITOR           ((uint32_t)0x000038)
#define MINOR_DEVICE_CLASS_VIDEODISPLAYSPEAKER    ((uint32_t)0x00003c)
#define MINOR_DEVICE_CLASS_VIDEOCONFERENCING      ((uint32_t)0x000040)
#define MINOR_DEVICE_CLASS_GAMINGTOY              ((uint32_t)0x000048)

/**
 * bt_types.h
 *
 * \name    BT_PERIPHERAL_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class PERIPHERAL.
 * \anchor  BT_PERIPHERAL_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_CLASS_KEYBOARD               ((uint32_t)0x000040)
#define MINOR_DEVICE_CLASS_POINTING_DEVICE        ((uint32_t)0x000080)
#define MINOR_DEVICE_CLASS_COMBO_KEY_POINTING     ((uint32_t)0x0000C0)
#define MINOR_DEVICE_CLASS_PERIMULT_UNCATEGORIZED ((uint32_t)0x000000)
#define MINOR_DEVICE_CLASS_PERIMULT_JOYSTICK      ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_PERIMULT_GAMEPAD       ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_PERIMULT_REMOTECONTROL ((uint32_t)0x00000C)
#define MINOR_DEVICE_CLASS_PERIMULT_SENSING       ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_PERIMULT_DIGITIZER     ((uint32_t)0x000014)
#define MINOR_DEVICE_CLASS_PERIMULT_CARDREADER    ((uint32_t)0x000018)

/**
 * bt_types.h
 *
 * \name    BT_IMAGING_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class IMAGING. May be combined use as mask.
 * \anchor  BT_IMAGING_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_IMAGE_DISPLAY                ((uint32_t)0x000010)
#define MINOR_DEVICE_IMAGE_CAMERA                 ((uint32_t)0x000020)
#define MINOR_DEVICE_IMAGE_SCANNER                ((uint32_t)0x000040)
#define MINOR_DEVICE_IMAGE_PRINTER                ((uint32_t)0x000080)

/**
 * bt_types.h
 *
 * \name    BT_HEALTH_MINOR_CLASS
 * \brief   BT Minor device class definitions for major class HEALTH.
 * \anchor  BT_HEALTH_MINOR_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define MINOR_DEVICE_HEALTH_BLOODPRESSURE         ((uint32_t)0x0004)
#define MINOR_DEVICE_HEALTH_THERMOMETER           ((uint32_t)0x0008)
#define MINOR_DEVICE_HEALTH_WEIGHINGSCALE         ((uint32_t)0x000C)
#define MINOR_DEVICE_HEALTH_GLUCOSEMETER          ((uint32_t)0x0010)
#define MINOR_DEVICE_HEALTH_PULSEOXIMETER         ((uint32_t)0x0014)
#define MINOR_DEVICE_HEALTH_HEARTPULSERATE        ((uint32_t)0x0018)
#define MINOR_DEVICE_HEALTH_DATADISPLAY           ((uint32_t)0x001C)
#define MINOR_DEVICE_HEALTH_STEPCOUNTER           ((uint32_t)0x0020)

/**
 * bt_types.h
 *
 * \name    BT_SERVICE_CLASS
 * \brief   BT Service class definitions.
 * \anchor  BT_SERVICE_CLASS
 */
/**
 * \ingroup     BT_TYPES
 */
#define SERVICE_CLASS_LIMITED_DISCOVERABLE_MODE   ((uint32_t)0x002000)
#define SERVICE_CLASS_RESERVED1                   ((uint32_t)0x004000)
#define SERVICE_CLASS_RESERVED2                   ((uint32_t)0x008000)
#define SERVICE_CLASS_POSITIONING                 ((uint32_t)0x010000)
#define SERVICE_CLASS_NETWORKING                  ((uint32_t)0x020000)
#define SERVICE_CLASS_RENDERING                   ((uint32_t)0x040000)
#define SERVICE_CLASS_CAPTURING                   ((uint32_t)0x080000)
#define SERVICE_CLASS_OBJECT_TRANSFER             ((uint32_t)0x100000)
#define SERVICE_CLASS_AUDIO                       ((uint32_t)0x200000)
#define SERVICE_CLASS_TELEPHONY                   ((uint32_t)0x400000)
#define SERVICE_CLASS_INFORMATION                 ((uint32_t)0x800000)

/**
 * bt_types.h
 *
 * \name    BT_COD_MASK
 * \brief   BT Device and service bitmask in COD.
 * \anchor  BT_COD_MASK
 */
/**
 * \ingroup     BT_TYPES
 */
#define SERVICE_CLASS_DEVICE_MASK                 ((uint32_t)0x00001FFF)
#define SERVICE_CLASS_SERVICE_MASK                ((uint32_t)0x00FFE000)

/**
 * bt_types.h
 *
 * \name    BT_UUID
 * \brief   BT Protocol/Profile UUID definitions.
 * \anchor  BT_UUID
 */
/**
 * \ingroup     BT_TYPES
 */
#define UUID_SDP                            0x0001
#define UUID_UDP                            0x0002
#define UUID_RFCOMM                         0x0003
#define UUID_TCP                            0x0004
#define UUID_TCSBIN                         0x0005
#define UUID_TCSAT                          0x0006
#define UUID_ATT                            0x0007
#define UUID_OBEX                           0x0008
#define UUID_IP                             0x0009
#define UUID_FTP                            0x000A
#define UUID_HTTP                           0x000C
#define UUID_WSP                            0x000E
#define UUID_BNEP                           0x000F
#define UUID_UPNP                           0x0010
#define UUID_HIDP                           0x0011
#define UUID_HC_CONTROL                     0x0012
#define UUID_HC_DATA                        0x0014
#define UUID_HC_NOTIFICATION                0x0016
#define UUID_AVCTP                          0x0017
#define UUID_AVDTP                          0x0019
#define UUID_CMTP                           0x001B
#define UUID_HDP_CTRL_CHANNEL               0x001E
#define UUID_HDP_DATA_CHANNEL               0x001F
#define UUID_RDTP                           0x00D1

#define UUID_L2CAP                          0x0100

#define UUID_SERVICE_DISCOVERY_SERVER       0x1000
#define UUID_BROWSE_GROUP_DESCRIPTOR        0x1001
#define UUID_PUBLIC_BROWSE_GROUP            0x1002
#define UUID_SERIAL_PORT                    0x1101
#define UUID_LAN_ACCESS                     0x1102
#define UUID_DIAL_UP_NETWORKING             0x1103
#define UUID_IRMC_SYNC                      0x1104
#define UUID_OBEX_OBJECT_PUSH               0x1105
#define UUID_OBEX_FILE_TRANSFER             0x1106
#define UUID_IRM_SYNC_COMMAND               0x1107
#define UUID_HEADSET                        0x1108
#define UUID_HEADSET_HS                     0x1131
#define UUID_CORDLESS_TELEPHONY             0x1109
#define UUID_AUDIO_SOURCE                   0x110A
#define UUID_AUDIO_SINK                     0x110B
#define UUID_AV_REMOTE_CONTROL_TARGET       0x110C
#define UUID_ADVANCED_AUDIO_DISTRIBUTION    0x110D
#define UUID_AV_REMOTE_CONTROL              0x110E
#define UUID_AV_REMOTE_CONTROL_CONTROLLER   0x110F
#define UUID_INTERCOM                       0x1110
#define UUID_FAX                            0x1111
#define UUID_HEADSET_AUDIO_GATEWAY          0x1112
#define UUID_WAP                            0x1113
#define UUID_WAP_CLIENT                     0x1114
#define UUID_PANU                           0x1115
#define UUID_NAP                            0x1116
#define UUID_GN                             0x1117
#define UUID_DIRECT_PRINTING                0x1118
#define UUID_REFERENCE_PRINTING             0x1119
#define UUID_IMAGING                        0x111A
#define UUID_IMAGING_RESPONDER              0x111B
#define UUID_IMAGING_AUTOMATIC_ARCHIVE      0x111C
#define UUID_REFERENCED_OBJECTS             0x111D
#define UUID_HANDSFREE                      0x111E
#define UUID_HANDSFREE_AUDIO_GATEWAY        0x111F
#define UUID_DIRECT_PRINTING_REF_OBJECTS    0x1120
#define UUID_REFLECTED_UI                   0x1121
#define UUID_BASIC_PRINTING                 0x1122
#define UUID_PRINTING_STATUS                0x1123
#define UUID_HUMAN_INTERFACE_DEVICE_SERVICE 0x1124
#define UUID_HARDCOPY_CABLE_REPLACEMENT     0x1125
#define UUID_HCR_PRINT                      0x1126
#define UUID_HCR_SCAN                       0x1127
#define UUID_COMMON_ISDN_ACCESS             0x1128
#define UUID_VIDEO_CONFERENCING_GW          0x1129
#define UUID_SIM                            0x112D
#define UUID_PBAP_PCE                       0x112E
#define UUID_PBAP_PSE                       0x112F
#define UUID_PBAP                           0x1130
#define UUID_MSG_ACCESS_SERVER              0x1132
#define UUID_MSG_NOTIFICATION_SERVER        0x1133
#define UUID_MSG_ACCESS_PROFILE             0x1134
#define UUID_PNP_INFORMATION                0x1200
#define UUID_GENERIC_NETWORKING             0x1201
#define UUID_GENERIC_FILE_TRANSFER          0x1202
#define UUID_GENERIC_AUDIO                  0x1203
#define UUID_GENERIC_TELEPHONY              0x1204
#define UUID_HDP_PROFILE                    0x1400
#define UUID_HDP_PROFILE_SOURCE             0x1401
#define UUID_HDP_PROFILE_SINK               0x1402
#define UUID_GAP                            0x1800
#define UUID_GATT                           0x1801

/**
 * bt_types.h
 *
 * \name    BT_PSM
 * \brief   BT Protocol/Service Multiplexer definitions.
 * \anchor  BT_PSM
 */
/**
 * \ingroup     BT_TYPES
 */
#define PSM_SDP                             0x0001
#define PSM_RFCOMM                          0x0003
#define PSM_HID_CONTROL                     0x0011
#define PSM_HID_INTERRUPT                   0x0013
#define PSM_AVCTP                           0x0017
#define PSM_AVDTP                           0x0019
#define PSM_AVCTP_BROWSING                  0x001B
#define PSM_ATT                             0x001F
#define PSM_RDTP                            0x00D1
#define PSM_EATT                            0x0027

/**
 * bt_types.h
 *
 * \name    BT_SDP_HEADER
 * \brief   BT SDP Header definitions.
 * \anchor  BT_SDP_HEADER
 */
/**
 * \ingroup     BT_TYPES
 */
#define SDP_UUID16_HDR                      0x19  /**< 00011 001 type:3 UUID; size: 2 bytes */
#define SDP_UUID32_HDR                      0x1a  /**< 00011 010 type:3 UUID; size: 4 bytes */
#define SDP_UUID128_HDR                     0x1c  /**< 00011 010 type:3 UUID; size: 16 bytes */
#define SDP_DATA_ELEM_SEQ_HDR               0x35  /**< 00110 101 type:6 size:5(next byte) */
#define SDP_DATA_ELEM_SEQ_HDR_2BYTE         0x36  /**< 00110 110 type:6 size:6(next two bytes) */
#define SDP_STRING_HDR                      0x25

#define SDP_UNSIGNED_ONE_BYTE               0x08
#define SDP_UNSIGNED_TWO_BYTE               0x09
#define SDP_UNSIGNED_FOUR_BYTE              0x0A
#define SDP_BOOL_ONE_BYTE                   0x28

/**
 * bt_types.h
 *
 * \name    BT_SDP_ATTRIBUTES
 * \brief   BT SDP Attribute definitions.
 * \anchor  BT_SDP_ATTRIBUTES
 */
/**
 * \ingroup     BT_TYPES
 */
#define SDP_ATTR_SRV_RECORD_HANDLE          0x0000
#define SDP_ATTR_SRV_CLASS_ID_LIST          0x0001
#define SDP_ATTR_SRV_RECORD_STATE           0x0002
#define SDP_ATTR_SRV_ID                     0x0003
#define SDP_ATTR_PROTO_DESC_LIST            0x0004
#define SDP_ATTR_BROWSE_GROUP_LIST          0x0005
#define SDP_ATTR_LANG_BASE_ATTR_ID_LIST     0x0006
#define SDP_ATTR_SRV_INFO_TIME_TO_LIVE      0x0007
#define SDP_ATTR_SRV_AVAILABILITY           0x0008
#define SDP_ATTR_PROFILE_DESC_LIST          0x0009
#define SDP_ATTR_DOC_URL                    0x000A
#define SDP_ATTR_CLIENT_EXEC_URL            0x000B
#define SDP_ATTR_ICON_URL                   0x000C
#define SDP_ATTR_ADD_PROTO_DESC_LIST        0x000D

#define SDP_ATTR_SRV_NAME                   0x0000  /**< these attrib values are base values */
#define SDP_ATTR_SRV_DESC                   0x0001  /**< these attrib values are base values */
#define SDP_ATTR_PROVIDER_NAME              0x0002  /**< these attrib values are base values */

#define SDP_ATTR_VER_NUMBER_LIST            0x0200
#define SDP_ATTR_GROUP_ID                   0x0200
#define SDP_ATTR_IP_SUBNET                  0x0200
#define SDP_ATTR_SRV_DB_STATE               0x0201

#define SDP_BASE_LANG_OFFSET                0x0100  /**< offset to base language */
#define SDP_LANG_ENGLISH                    0x656e  /**< this is "en", according to ISO 639:1988 */
#define SDP_LANG_GERMAN                     0x6465  /**< this is "de", according to ISO 639:1988 */
#define SDP_CHARACTER_UTF8                  106     /**< UTF8 encoding, cf. http://www.isi.edu/in-notes/iana/assignments/character-sets */

/**
 * bt_types.h
 *
 * \name    BT_SDP_PROFILE_ATTRIBUTES
 * \brief   BT SDP Profile Attribute definitions.
 * \anchor  BT_SDP_PROFILE_ATTRIBUTES
 */
/**
 * \ingroup     BT_TYPES
 */
#define SDP_ATTR_L2C_PSM                    0x0200

#define SDP_ATTR_SRV_VER                    0x0300
#define SDP_ATTR_EXT_NETWORK                0x0301
#define SDP_ATTR_FAX_CLASS1_SUPPORT         0x0302
#define SDP_ATTR_RMT_AUDIO_VOL_CTRL         0x0302
#define SDP_ATTR_FAX_CLASS20_SUPPORT        0x0303
#define SDP_ATTR_SUPPORTED_FORMATS_LIST     0x0303
#define SDP_ATTR_FAX_CLASS2_SUPPORT         0x0304
#define SDP_ATTR_AUDIO_FEEDBACK_SUPPORT     0x0305
#define SDP_ATTR_NETWORK_ADDR               0x0306
#define SDP_ATTR_WAP_GATEWAY                0x0307
#define SDP_ATTR_HOME_PAGE_URL              0x0308
#define SDP_ATTR_WAP_STACK_TYPE             0x0309
#define SDP_ATTR_SECURITY_DESC              0x030A
#define SDP_ATTR_NET_ACCESS_TYPE            0x030B
#define SDP_ATTR_MAX_NET_ACCESS_RATE        0x030C
#define SDP_ATTR_SUPPORTED_CAPABILITIES     0x0310
#define SDP_ATTR_SUPPORTED_FEATURES         0x0311
#define SDP_ATTR_SUPPORTED_FUNCTIONS        0x0312
#define SDP_ATTR_TOTAL_IMAGING_CAPACITY     0x0313
#define SDP_ATTR_SUPPORTED_REPOSITORIES     0x0314

#define SDP_ATTR_MAS_INSTANCE_ID            0x0315
#define SDP_ATTR_SUPPORTED_MSG_TYPES        0x0316
#define SDP_ATTR_MAP_SUPPERTED_FEATS        0x0317

#define SDP_ATTR_HID_DEV_RELEASE_NUM        0x0200
#define SDP_ATTR_HID_PARSER_VER             0x0201
#define SDP_ATTR_HID_DEV_SUB_CLASS          0x0202
#define SDP_ATTR_HID_COUNTRY_CODE           0x0203
#define SDP_ATTR_HID_VIRTUAL_CABLE          0x0204
#define SDP_ATTR_HID_RECONN_INIT            0x0205
#define SDP_ATTR_HID_DESC_LIST              0x0206
#define SDP_ATTR_HID_LANG_ID_BASE_LIST      0x0207
#define SDP_ATTR_HID_SDP_DISABLE            0x0208
#define SDP_ATTR_HID_BATTERY_POWER          0x0209
#define SDP_ATTR_HID_REMOTE_WAKE            0x020A
#define SDP_ATTR_HID_PROFILE_VER            0x020B
#define SDP_ATTR_HID_SUPERVISION_TIMEOUT    0x020C
#define SDP_ATTR_HID_NORMALLY_CONNECTABLE   0x020D
#define SDP_ATTR_HID_BOOT_DEV               0x020E

#define SDP_ATTR_PBAP_SUPPORTED_FEAT        0x0317

#define SDP_ATTR_DIP_SPECIFICATION_ID       0x0200
#define SDP_ATTR_DIP_VENDOR_ID              0x0201
#define SDP_ATTR_DIP_PRODUCT_ID             0x0202
#define SDP_ATTR_DIP_PRODUCT_VERSION        0x0203
#define SDP_ATTR_DIP_PRIMARY_RECORD         0x0204
#define SDP_ATTR_DIP_VENDOR_ID_SOURCE       0x0205

/**
 * bt_types.h
 *
 * \name    BT_L2CAP_FIXED_CID
 * \brief   BT L2CAP Fixed cid definitions.
 * \anchor  BT_L2CAP_FIXED_CID
 */
/**
 * \ingroup     BT_TYPES
 */
#define L2C_FIXED_CID_ATT                   0x0004

/**
 * bt_types.h
 *
 * \name    BT_LE_LOCAL_SUPPORTED_FEATURES
 * \brief   BT LE Local Supported Features definitions.
 * \anchor  BT_LE_LOCAL_SUPPORTED_FEATURES
 */
/**
 * \ingroup     BT_TYPES
 */
#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX0                           0

#define LE_SUPPORT_FEATURES_ENCRYPTION_MASK_BIT                         ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_CONNPARA_REQ_PROC_MASK_BIT                  ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_EXTENDED_REJECTION_IND_MASK_BIT             ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_SLAVE_INITIATED_FEATURE_EXCHANGE_MASK_BIT   ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_LE_PING_MASK_BIT                            ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_LE_DATA_LENGTH_EXTENSION_MASK_BIT           ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_LL_PRIVACY_MASK_BIT                         ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_EXTENDED_SCANNER_FILTER_POLICY_MASK_BIT     ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1                           1

#define LE_SUPPORT_FEATURES_LE_2M_MASK_BIT                              ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_STABLE_MODULATION_INDEX_TX_MASK_BIT         ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_STABLE_MODULATION_INDEX_RX_MASK_BIT         ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_LE_CODED_PHY_MASK_BIT                       ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_LE_EXTENDED_ADV_BIT                         ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_LE_PERIODIC_ADV_MASK_BIT                    ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_CSA2_BIT                                    ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_LE_POWER_CLASS1_MASK_BIT                    ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX2                           2

#define LE_SUPPORT_FEATURES_MIN_NUM_USED_CHANNEL_MASK_BIT               ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_CONNECTION_CTE_REQUEST_MASK_BIT             ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_CONNECTION_CTE_RESPONSE_MASK_BIT            ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_CONNECTIONLESS_CTE_TRANSMITTER_MASK_BIT     ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_CONNECTIONLESS_CTE_RECEIVER_MASK_BIT        ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_AOD_MASK_BIT                                ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_AOA_MASK_BIT                                ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_RECEIVING_CTE_MASK_BIT                      ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX3                           3

#define LE_SUPPORT_FEATURES_PAST_SENDER_MASK_BIT                        ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_PAST_RECIPIENT_MASK_BIT                     ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_SLEEP_CLOCK_ACCURACY_UPDATES_MASK_BIT       ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_REMOTE_PUBLIC_KEY_VALIDATION_MASK_BIT       ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_CIS_MASTER_MASK_BIT                         ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_CIS_SLAVE_MASK_BIT                          ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_ISOCHRONOUS_BROADCASTER_MASK_BIT            ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_SYNCHRONIZED_RECEIVER_MASK_BIT              ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX4                           4

#define LE_SUPPORT_FEATURES_ISOCH_HOST_SUPPORT_MASK_BIT                 ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_LE_POWER_CONTROL_REQUEST_MASK_BIT           ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_LE_POWER_CHANGE_INDICATION_MASK_BIT         ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_LE_PATH_LOSS_MONITORING_MASK_BIT            ((uint8_t)0x08)

/**
 * bt_types.h
 *
 * \name    BT_LE_SET_HOST_FEATURE
 * \brief   BT LE Set Host Feature definitions.
 * \anchor  BT_LE_SET_HOST_FEATURE
 */
/**
 * \ingroup     BTTYPES
 */
#define LE_SET_HOST_FEATURE_BIT_VALUE_DISABLE                            ((uint8_t)0x00)
#define LE_SET_HOST_FEATURE_BIT_VALUE_ENABLE                             ((uint8_t)0x01)
#define LE_SET_HOST_FEATURE_BIT_VALUE_RFU                                ((uint8_t)0xFF)

#define LE_SET_HOST_FEATURE_BIT_NUMBER_CIS_HOST_SUPPORT                  ((uint8_t)0x20)
#define LE_SET_HOST_FEATURE_BIT_NUMBER_RFU                               ((uint8_t)0xFF)

/**
 * bt_types.h
 *
 * \name    BT_STACK_MODULE_ERROR
 * \brief   BT stack module error code mask.
 * \anchor  BT_STACK_MODULE_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define GAP_ERR                 0x0000
#define HCI_ERR                 0x0100
#define L2C_ERR                 0x0200
#define SDP_ERR                 0x0300
#define ATT_ERR                 0x0400
#define SM_ERR                  0x0500

#define RFC_ERR                 0x0B00
#define GATT_ERR                0x0C00
#define APP_ERR                 0x0D00
#define IAP_ERR                 0x0E00
#define SPP_ERR                 0x0F00

/**
 * bt_types.h
 *
 * \name    BT_HCI_ERROR
 * \brief   BT hci error code definitions.
 * \anchor  BT_HCI_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define HCI_SUCCESS                             0x00
#define HCI_ERR_UNKNOWN_CMD                     0x01
#define HCI_ERR_UNKNOWN_CONN_ID                 0x02
#define HCI_ERR_HARDWARE_FAIL                   0x03
#define HCI_ERR_PAGE_TIMEOUT                    0x04
#define HCI_ERR_AUTHEN_FAIL                     0x05
#define HCI_ERR_KEY_MISSING                     0x06
#define HCI_ERR_MEMORY_FULL                     0x07
#define HCI_ERR_CONN_TIMEOUT                    0x08
#define HCI_ERR_MAX_NUM_CONN                    0x09
#define HCI_ERR_MAX_NUM_SCO_CONN                0x0A  /* 10 */
#define HCI_ERR_ACL_CONN_EXIST                  0x0B  /* 11 */
#define HCI_ERR_CMD_DISALLOWED                  0x0C  /* 12 */
#define HCI_ERR_REJECT_LIMITED_RESOURCE         0x0D  /* 13 */
#define HCI_ERR_REJECT_SECURITY_REASON          0x0E  /* 14 */
#define HCI_ERR_REJECT_UNACCEPTABLE_ADDR        0x0F  /* 15 */
#define HCI_ERR_HOST_TIMEOUT                    0x10  /* 16 */
#define HCI_ERR_UNSUPPORTED_PARAM               0x11  /* 17 */
#define HCI_ERR_INVALID_PARAM                   0x12  /* 18 */
#define HCI_ERR_REMOTE_USER_TERMINATE           0x13  /* 19 */
#define HCI_ERR_REMOTE_LOW_RESOURCE             0x14  /* 20 */
#define HCI_ERR_REMOTE_POWER_OFF                0x15  /* 21 */
#define HCI_ERR_LOCAL_HOST_TERMINATE            0x16  /* 22 */
#define HCI_ERR_REPEATED_ATTEMPTS               0x17  /* 23 */
#define HCI_ERR_PARING_NOT_ALLOWED              0x18  /* 24 */
#define HCI_ERR_UNKNOWN_LMP_PDU                 0x19  /* 25 */
#define HCI_ERR_UNSUPPORTED_REMOTE_FEAT         0x1A  /* 26 */
#define HCI_ERR_SCO_OFFSET_REJECTED             0x1B  /* 27 */
#define HCI_ERR_SCO_INTERVAL_REJECTED           0x1C  /* 28 */
#define HCI_ERR_SCO_AIR_MODE_REJECTED           0x1D  /* 29 */
#define HCI_ERR_INVALID_LMP_PARAM               0x1E  /* 30 */
#define HCI_ERR_UNSPECIFIED_ERROR               0x1F  /* 31 */
#define HCI_ERR_UNSUPPORTED_LMP_PARAM           0x20  /* 32 */
#define HCI_ERR_ROLE_CHANGE_NOT_ALLOWED         0x21  /* 33 */
#define HCI_ERR_LMP_RESPONSE_TIMEOUT            0x22  /* 34 */
#define HCI_ERR_LMP_ERROR_TRANS_COLLISION       0x23  /* 35 */
#define HCI_ERR_LMP_PDU_NOT_ALLOWED             0x24  /* 36 */
#define HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE     0x25  /* 37 */
#define HCI_ERR_UNIT_KEY_USED                   0x26  /* 38 */
#define HCI_ERR_QOS_NOT_SUPPORTED               0x27  /* 39 */
#define HCI_ERR_INSTANT_PASSED                  0x28  /* 40 */
#define HCI_ERR_PAIR_UNIT_KEY_NOT_SUPPORT       0x29  /* 41 */
#define HCI_ERR_DIFF_TRANS_COLLISION            0x2A  /* 42 */
#define HCI_ERR_QOS_UNACCEPTABLE_PARAM          0x2C  /* 44 */
#define HCI_ERR_QOS_REJECT                      0x2D  /* 45 */
#define HCI_ERR_CHANN_ASSESS_NOT_SUPPORT        0x2E  /* 46 */
#define HCI_ERR_INSUFFICIENT_SECURITY           0x2F  /* 47 */
#define HCI_ERR_PARAM_OUT_OF_RANGE              0x30  /* 48 */
#define HCI_ERR_ROLE_SWITCH_PENDING             0x32  /* 50 */
#define HCI_ERR_RESERVED_SLOT_VIOLATION         0x34  /* 52 */
#define HCI_ERR_ROLE_SWITCH_FAILED              0x35  /* 53 */
#define HCI_ERR_EXT_INQUIRY_RSP_TOO_LARGE       0x36  /* 54 */
#define HCI_ERR_SSP_NOT_SUPPORTED_BY_HOST       0x37  /* 55 */
#define HCI_ERR_HOST_BUSY_PAIRING               0x38  /* 56 */
#define HCI_ERR_REJECT_NO_SUITABLE_CHANN        0x39  /* 57 */
#define HCI_ERR_CONTROLLER_BUSY                 0x3A  /* 58 */
#define HCI_ERR_UNACCEPTABLE_CONN_INTERVAL      0x3B  /* 59 */
#define HCI_ERR_DIRECTED_ADV_TIMEOUT            0x3C  /* 60 */
#define HCI_ERR_MIC_FAILURE                     0x3D  /* 61 */
#define HCI_ERR_FAIL_TO_ESTABLISH_CONN          0x3E  /* 62 */
#define HCI_ERR_MAC_CONN_FAIL                   0x3F  /* 63 */
#define HCI_ERR_COARSE_CLOCK_ADJUST_REJECTED    0x40
#define HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED        0x41
#define HCI_ERR_UNKNOWN_ADVERTISING_IDENTIFIER  0x42
#define HCI_ERR_LIMIT_REACHED                   0x43
#define HCI_ERR_OPERATION_CANCELLED_BY_HOST     0x44
#define HCI_ERR_OPERATION_PACKET_TOO_LONG       0x45

/* hci local error code */
#define HCI_ERR_INVALID_ADDR                    0x64  /* 100 */
#define HCI_ERR_TIMEOUT                         0x65  /* 101 */
#define HCI_ERR_OUT_OF_SYNC                     0x66  /* 102 */
#define HCI_ERR_NO_DESCRIPTOR                   0x67  /* 103 */
#define HCI_ERR_NO_MEMORY                       0x68  /* 104 */
#define HCI_ERR_INVALID_STATE                   0x69  /* 105 */
#define HCI_ERR_LOCAL_KEY_MISSING               0x6A  /* 106 */
#define HCI_ERR_AUDIO_STOP                      0xFE  /* 254 */
#define HCI_ERR_CONN_ROLESWAP                   0xFF  /* 255 */

/**
 * bt_types.h
 *
 * \name    BT_L2CAP_ERROR
 * \brief   BT l2cap error code definitions.
 * \anchor  BT_L2CAP_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define L2C_SUCCESS                             0x00

#define L2C_CONN_ACCEPT                         0x0000
#define L2C_CONN_RSP_PENDING                    0x0001
#define L2C_CONN_RSP_INVALID_PSM                0x0002
#define L2C_CONN_RSP_SECURITY_BLOCK             0x0003
#define L2C_CONN_RSP_NO_RESOURCE                0x0004
#define L2C_CONN_RSP_INVALID_PARAM              0x0005
#define L2C_CONN_RSP_INVALID_SOURCE_CID         0x0006
#define L2C_CONN_RSP_SOURCE_CID_ALLOCATED       0x0007

/* requset cmd reject reasons */
#define L2C_CMD_REJ_NOT_UNDERSTOOD              0x0000
#define L2C_CMD_REJ_SIGNAL_MTU_EXCEEDED         0x0001
#define L2C_CMD_REJ_INVALID_CID_IN_REQ          0x0002

/* config response result codes */
#define L2C_CFG_RSP_SUCCESS                     0x0000
#define L2C_CFG_RSP_UNACCEPTABLE_PARAMS         0x0001
#define L2C_CFG_RSP_REJECTED                    0x0002
#define L2C_CFG_RSP_UNKNOWN_OPTIONS             0x0003
#define L2C_CFG_RSP_PENDING                     0x0004
#define L2C_CFG_RSP_FLOW_SPEC_REJECTED          0x0005

/* LE credit based connection response result codes */
#define L2C_LE_CREDIT_CONN_SUCCESS                      0x0000
#define L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED             0x0002
#define L2C_LE_CREDIT_RSP_NO_RESOURCES                  0x0004
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN           0x0005
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR           0x0006
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE  0x0007
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP           0x0008
#define L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID            0x0009
#define L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC      0x000A

/* Credit based connection response result codes */
#define L2C_ECFC_ALL_CONN_SUCCESS                               0x0000
#define L2C_ECFC_CONN_RSP_ALL_CONN_PSM_NOT_SUPPORTED            0x0002
#define L2C_ECFC_CONN_RSP_SOME_CONN_NO_RESOURCES                0x0004
#define L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_AUTHEN          0x0005
#define L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_AUTHOR          0x0006
#define L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_ENCRYP_KEY_SIZE 0x0007
#define L2C_ECFC_CONN_RSP_ALL_CONN_INSUFFICIENT_ENCRYP          0x0008
#define L2C_ECFC_CONN_RSP_SOME_CONN_INVALID_SOURCE_CID          0x0009
#define L2C_ECFC_CONN_RSP_SOME_CONN_SOURCE_CID_ALREADY_ALLOC    0x000A
#define L2C_ECFC_CONN_RSP_ALL_CONN_UNACCEPTABLE_PARAMS          0x000B
#define L2C_ECFC_CONN_RSP_ALL_CONN_INVALID_PARAMS               0x000C
#define L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_NO_INFO              0x000D
#define L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_AUTHER               0x000E
#define L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_AUTHOR               0x000F

/* Credit based reconfigure response result codes */
#define L2C_ECFC_RCFG_RSP_REDUCE_MTU_NOT_ALLOWED                0x0001
#define L2C_ECFC_RCFG_RSP_REDUCE_MPS_NOT_ALLOWED                0x0002
#define L2C_ECFC_RCFG_RSP_INVALID_DCID                          0x0003
#define L2C_ECFC_RCFG_RSP_UNACCEPTABLE_PARAMS                   0x0004

/* status for connection updata response */
#define L2C_LE_CONN_UPDATE_ACCEPT               0x0000
#define L2C_LE_CONN_UPDATE_RSP_REJECT           0x0001
#define L2C_LE_CONN_UPDATE_RSP_TOUT             0x0002

/* l2cap error code */
#define L2C_ERR_REJ_BASE                        0x0010
#define L2C_ERR_CFG_BASE                        0x0020
#define L2C_ERR_LE_CREDIT_BASE                  0x0030
#define L2C_ERR_LE_CONN_UPDATE_BASE             0x0040
#define L2C_ERR_ECFC_CONN_BASE                  0x0050
#define L2C_ERR_ECFC_RCFG_BASE                  0x0060
#define L2C_ERR_VND_BASE                        0x00F0

#define L2C_ERR_PENDING                         L2C_CONN_RSP_PENDING
#define L2C_ERR_INVALID_PSM                     L2C_CONN_RSP_INVALID_PSM
#define L2C_ERR_SECURITY_BLOCK                  L2C_CONN_RSP_SECURITY_BLOCK
#define L2C_ERR_NO_RESOURCE                     L2C_CONN_RSP_NO_RESOURCE
#define L2C_ERR_INVALID_PARAM                   L2C_CONN_RSP_INVALID_PARAM
#define L2C_ERR_INVALID_SOURCE_CID              L2C_CONN_RSP_INVALID_SOURCE_CID
#define L2C_ERR_SOURCE_CID_ALLOCATED            L2C_CONN_RSP_SOURCE_CID_ALLOCATED

#define L2C_ERR_CMD_NOT_UNDERSTOOD              (L2C_ERR_REJ_BASE + L2C_CMD_REJ_NOT_UNDERSTOOD)
#define L2C_ERR_SIGNAL_MTU_EXCEEDED             (L2C_ERR_REJ_BASE + L2C_CMD_REJ_SIGNAL_MTU_EXCEEDED)
#define L2C_ERR_INVALID_CID_IN_REQ              (L2C_ERR_REJ_BASE + L2C_CMD_REJ_INVALID_CID_IN_REQ)

#define L2C_ERR_CFG_UNACCEPTABLE_PARAM          (L2C_ERR_CFG_BASE + L2C_CFG_RSP_UNACCEPTABLE_PARAMS)
#define L2C_ERR_CFG_REJECTED                    (L2C_ERR_CFG_BASE + L2C_CFG_RSP_REJECTED)
#define L2C_ERR_CFG_UNKNOWN_OPTIONS             (L2C_ERR_CFG_BASE + L2C_CFG_RSP_UNKNOWN_OPTIONS)
#define L2C_ERR_CFG_PENDING                     (L2C_ERR_CFG_BASE + L2C_CFG_RSP_PENDING)
#define L2C_ERR_CFG_FLOW_SPEC_REJECTED          (L2C_ERR_CFG_BASE + L2C_CFG_RSP_FLOW_SPEC_REJECTED)

#define L2C_ERR_LE_CREDIT_PSM_NOT_SUPPORTED             (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED)
#define L2C_ERR_LE_CREDIT_NO_RESOURCES                  (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_NO_RESOURCES)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_AUTHEN           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_AUTHOR           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_ENCRYP_KEY_SIZE  (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_ENCRYP           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP)
#define L2C_ERR_LE_CREDIT_INVALID_SOURCE_CID            (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID)
#define L2C_ERR_LE_CREDIT_SOURCE_CID_ALREADY_ALLOC      (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC)

#define L2C_ERR_LE_CONN_PARAM_UPDATE_REJECT     (L2C_ERR_LE_CONN_UPDATE_BASE + L2C_LE_CONN_UPDATE_RSP_REJECT)
#define L2C_ERR_LE_CONN_PARAM_UPDATE_TOUT       (L2C_ERR_LE_CONN_UPDATE_BASE + L2C_LE_CONN_UPDATE_RSP_TOUT)

#define L2C_ERR_VND_CREDITS_EXCEED              (L2C_ERR_VND_BASE + 0x00)
#define L2C_ERR_VND_INVALID_PDU                 (L2C_ERR_VND_BASE + 0x01)
#define L2C_ERR_VND_CREDITS_LACK                (L2C_ERR_VND_BASE + 0x02)
#define L2C_ERR_VND_NO_MEMORY                   (L2C_ERR_VND_BASE + 0x03)
#define L2C_ERR_VND_INVALID_STATE               (L2C_ERR_VND_BASE + 0x04)
#define L2C_ERR_VND_INVALID_RX_SEQ              (L2C_ERR_VND_BASE + 0x05)
#define L2C_ERR_VND_TIMEOUT                     (L2C_ERR_VND_BASE + 0x06)
#define L2C_ERR_VND_INVALID_MODE                (L2C_ERR_VND_BASE + 0x07)
#define L2C_ERR_VND_REMOTE_DISCONNECT           (L2C_ERR_VND_BASE + 0x08)
#define L2C_ERR_VND_INVALID_PARAM               (L2C_ERR_VND_BASE + 0x09)

#if F_BT_BREDR_SUPPORT
/**
 * bt_types.h
 *
 * \name    BT_SDP_ERROR
 * \brief   BT sdp error code definitions.
 * \anchor  BT_SDP_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define SDP_ERR_INVALID_VERSION                 0x01
#define SDP_ERR_INVALID_SRV_RECORD_HANDLE       0x02
#define SDP_ERR_INVALID_REQ_SYNTAX              0x03
#define SDP_ERR_INVALID_PDU_SIZE                0x04
#define SDP_ERR_INVALID_CONTINUATION_STATE      0x05
#define SDP_ERR_INSUFFICIENT_RESOURCE           0x06

/* sdp local error code */
#define SDP_ERR_NO_RESOURCE                     0x61
#define SDP_ERR_INVALID_PARAM                   0x62
#define SDP_ERR_INVALID_STATE                   0x63
#define SDP_ERR_UNHANDLED_CODE                  0x64  /**< 100 Client received unhandled SDP opcode */
#define SDP_ERR_TIMEOUT                         0x65  /**< 101 No answer from server (timeout)      */
#define SDP_ERR_INVALID_RSP_SYNTAX              0x66  /**< 102 Syntax Error in Response             */
#define SDP_ERR_NOT_FOUND                       0xC8  /**< 200 not really an error code             */
#endif

/**
 * bt_types.h
 *
 * \name    BT_ATT_ERROR
 * \brief   BT att error code definitions.
 * \anchor  BT_ATT_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define ATT_SUCCESS                         0    /**< internal value .. */
#define ATT_ERR_INVALID_HANDLE              0x01 /**< Attribute handle value given was not valid on this attribute server */
#define ATT_ERR_READ_NOT_PERMITTED          0x02 /**< Attribute cannot be read */
#define ATT_ERR_WRITE_NOT_PERMITTED         0x03 /**< Attribute cannot be written */
#define ATT_ERR_INVALID_PDU                 0x04 /**< The attribute PDU was invalid */
#define ATT_ERR_INSUFFICIENT_AUTHEN         0x05 /**< The attribute requires authentication before it can be read or written */
#define ATT_ERR_UNSUPPORTED_REQ             0x06 /**< Attribute server doesn't support the request received from the attribute client */
#define ATT_ERR_INVALID_OFFSET              0x07 /**< Offset specified was past the end of the attribute */
#define ATT_ERR_INSUFFICIENT_AUTHOR         0x08 /**< The attribute requires an authorization before it can be read or written */
#define ATT_ERR_PREP_QUEUE_FULL             0x09 /**< Too many prepare writes have been queued */
#define ATT_ERR_ATTR_NOT_FOUND              0x0A /**< No attribute found within the given attribute handle range */
#define ATT_ERR_ATTR_NOT_LONG               0x0B /**< Attribute cannot be read or written using the Read Blob Request or Prepare Write Request */
#define ATT_ERR_INSUFFICIENT_KEY_SIZE       0x0C /**< The Encryption Key Size used for encrypting this link is insufficient */
#define ATT_ERR_INVALID_VALUE_SIZE          0x0D /**< The attribute value length is invalid for the operation */
#define ATT_ERR_UNLIKELY                    0x0E /**< The attribute request that was requested has encountered an error that was very unlikely, and therefore could not be completed as requested */
#define ATT_ERR_INSUFFICIENT_ENCRYPT        0x0F /**< The attribute requires encryption before it can be read or written */
#define ATT_ERR_UNSUPPORTED_GROUP_TYPE      0x10 /**< The attribute type is not a supported grouping attribute as defined by a higher layer specification */
#define ATT_ERR_INSUFFICIENT_RESOURCES      0x11 /**< Insufficient Resources to complete the request */
#define ATT_ERR_DATABASE_OUT_OF_SYNC        0x12 /**< The server requests the client to rediscover the database. */
#define ATT_ERR_VALUE_NOT_ALLOWED           0x13 /**< The attribute parameter value was not allowed. */

/* Application Error 0x80 - 0x9F */
#define ATT_ERR_INVALID_VALUE               0x80 /**< The attribute value is invalid for the operation */

/* manufacturer specific error codes that are "missing" in GATT spec. >= 0xC0:   */
#define ATT_ERR_INVALID_CCC_BITS            0xC0 /**< Invalid client char. config. bits */
#define ATT_ERR_INVALID_SIGNED_COUNT        0xC1 /**< Invalid sign count */
#define ATT_ERR_INVALID_SIGNED_MAC_FAILED   0xC2 /**< Invalid sign mac value */
#define ATT_ERR_MIN_APPLIC_CODE             0xC3

/* error codes common to various profiles >= 0xE0 (CSS)*/
#define ATT_ERR_WRITE_REQUEST_REJECTED      0xFC /**< Write Request Rejected */
#define ATT_ERR_CCCD_IMPROPERLY_CONFIGURED  0xFD /**< CCCD improperly configured */
#define ATT_ERR_PROC_ALREADY_IN_PROGRESS    0xFE /**< Procedure Already in Progress */
#define ATT_ERR_OUT_OF_RANGE                0xFF /**< An attribute value is out of range as defined by a profile or service specification. */

/**
 * bt_types.h
 *
 * \name    BT_RFC_ERROR
 * \brief   BT rfcomm error code definitions.
 * \anchor  BT_RFC_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define RFC_SUCCESS                     0x00
#define RFC_ERR_TIMEOUT                 0x01
#define RFC_ERR_NSC                     0x02
#define RFC_ERR_INVALID_STATE           0x03
#define RFC_ERR_REJECT_SECURITY         0x04

/**
 * bt_types.h
 *
 * \name    BT_GATT_ERROR
 * \brief   BT gatt error code definitions.
 * \anchor  BT_GATT_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define GATT_SUCCESS                        0x00
#define GATT_ERR_OUT_OF_RESOURCE            0x01
#define GATT_ERR_UNSUPPORTED                0x02
#define GATT_ERR_INVALID_ROLE               0x03
#define GATT_ERR_INVALID_STATE              0x04
#define GATT_ERR_INVALID_CODING             0x05
#define GATT_ERR_INVALID_HANDLE             0x06
#define GATT_ERR_INVALID_PARAM              0x07
#define GATT_ERR_INTERNAL                   0x08
#define GATT_ERR_NOT_ALLOWED                0x09
#define GATT_ERR_NOTIF_IND_NOT_CFG          0x0A
#define GATT_ERR_NOTIF_IND_CFG              0x0B
#define GATT_ERR_NOTIF_IND_CONF_PD          0x0C
#define GATT_ERR_TIMEOUT                    0x0D
#define GATT_ERR_LINK_DEACTIVATED           0x0E
#define GATT_ERR_NOT_AUTHENTICATED          0x0F
#define GATT_ERR_NOT_ENCRYPTED              0x10
#define GATT_ERR_PROC_FAIL                  0x11

/* sm error code */
#define SM_SUCCESS                          0x00
/* SMP_OP_PAIRING_FAILED causes */
#define SMP_ERR_SUCCESS                         0x00
#define SMP_ERR_PASSKEY_ENTRY_FAIL              0x01
#define SMP_ERR_OOB_NOT_AVAIABLE                0x02
#define SMP_ERR_AUTHEN_REQUIREMENTS             0x03
#define SMP_ERR_CFM_VALUE_FAILED                0x04
#define SMP_ERR_PAIRING_NOT_SUPPORTED           0x05
#define SMP_ERR_ENCRYPTION_KEY_SIZE             0x06
#define SMP_ERR_CMD_NOT_SUPPORTED               0x07
#define SMP_ERR_UNSPECIFIED_REASON              0x08
#define SMP_ERR_REPEATED_ATTEMPTS               0x09
#define SMP_ERR_INVALID_PARAM                   0x0A
#define SMP_ERR_DHKEY_CHECK_FAILED              0x0B
#define SMP_ERR_NUMERIC_COMPARISION_FAIL        0x0C
#define SMP_ERR_BREDR_PAIRING_IN_PROGRESS       0x0D
#define SMP_ERR_CRS_TRANS_KEY_GEN_NOT_ALLOW     0x0E

#define SM_ERR_NO_ENTRY                         0xE1
#define SM_ERR_DB_FULL                          0xE2
#define SM_ERR_INVALID_PARAM                    0xE3
#define SM_ERR_INSUFFICIENT_LINK_KEY            0xE4
#define SM_ERR_LE_ADDR_NOT_RESOLVED             0xE5
#define SM_ERR_INVALID_STATE                    0xE6
#define SM_ERR_NO_RESOURCE                      0xE7
#define SM_ERR_LINK_KEY_MISSING                 0xE8
#define SM_ERR_DISCONNECT                       0xE9
#define SM_ERR_PARING_NOT_ALLOWED               0xEA
#define SM_ERR_KEY_SAVE_FAILED                  0xEB
#define SM_ERR_TIMEOUT                          0xEC
#define SM_ERR_UNKNOWN                          0xED
#define SM_ERR_LINK_DEACTIVATED                 0xEE
#define SM_ERR_NO_PAIRABLE_MODE                 0xEF

/**
 * bt_types.h
 *
 * \name    BT_BTIF_ERROR
 * \brief   BT btif error code definitions.
 * \anchor  BT_BTIF_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define BTIF_ERR                0x0000
#define BTIF_SUCCESS                        0x00
#define BTIF_ACCEPT                         0x01
#define BTIF_ERR_REJECT                     0x02
#define BTIF_ERR_NO_RESOURCE                0x03
#define BTIF_ERR_INVALID_PARAM              0x04
#define BTIF_ERR_INVALID_STATE              0x05
#define BTIF_ERR_CONN_DISCONNECT            0x06
#define BTIF_ERR_CONN_LOST                  0x07
#define BTIF_ERR_AUTHEN_FAIL                0x08
#define BTIF_ERR_INIT_TOUT                  0x09
#define BTIF_ERR_INIT_OUT_OF_SYNC           0x0A
#define BTIF_ERR_INIT_HARDWARE_ERROR        0x0B
#define BTIF_ERR_LOWER_LAYER_ERROR          0x0C
#define BTIF_ERR_ADDR_NOT_RESOLVED          0x0D
#define BTIF_ERR_TOUT                       0x0E

#define BTIF_ERR_UNSPECIFIED                0xFD
#define BTIF_ERR_NOT_SUPPORTED              0xFE

/**
 * bt_types.h
 *
 * \name    BT_GAP_ERROR
 * \brief   BT GAP error code definitions.
 * \anchor  BT_GAP_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define GAP_SUCCESS                         0x00
#define GAP_ACCEPT                          0x01
#define GAP_ERR_REJECT                      0x02
#define GAP_ERR_NO_RESOURCE                 0x03
#define GAP_ERR_INVALID_PARAM               0x04
#define GAP_ERR_INVALID_STATE               0x05
#define GAP_ERR_CONN_DISCONNECT             0x06
#define GAP_ERR_CONN_LOST                   0x07
#define GAP_ERR_AUTHEN_FAIL                 0x08
#define GAP_ERR_INIT_TOUT                   0x09
#define GAP_ERR_INIT_OUT_OF_SYNC            0x0A
#define GAP_ERR_INIT_HARDWARE_ERROR         0x0B
#define GAP_ERR_LOWER_LAYER_ERROR           0x0C
#define GAP_ERR_ADDR_NOT_RESOLVED           0x0D
#define GAP_ERR_TOUT                        0x0E

#define GAP_ERR_REQ_FAILED                  0xFB
#define GAP_ERR_SW_RESET                    0xFC
#define GAP_ERR_UNSPECIFIED                 0xFD
#define GAP_ERR_NOT_SUPPORTED               0xFE

/**
 * bt_types.h
 *
 * \name    BT_APP_ERROR
 * \brief   BT APP error code definitions.
 * \anchor  BT_APP_ERROR
 */
/**
 * \ingroup     BT_TYPES
 */
#define APP_SUCCESS                             0x00
#define APP_ERR_PENDING                         0x01
#define APP_ERR_ACCEPT                          0x03
#define APP_ERR_REJECT                          0x04
#define APP_ERR_NOT_RELEASE                     0x05

#define IAP_SUCCESS                             0x00
#define IAP_ERR_RETRANS_CNT_EXCEED              0x01

#define SPP_SUCCESS                             0x00
#define SPP_ERR_BROKEN_LINK                     0x01

/**
 * bt_types.h
 *
 * \name    BT_BYTE_ORDER
 * \brief   BT buffer/array byte-order utility macros.
 * \anchor  BT_BYTE_ORDER
 */
/**
 * \ingroup     BT_TYPES
 */
/** Calculate integer bit count of b'1 */
#define INT_BIT_COUNT(integer, count)   {               \
        count = 0;                                      \
        while (integer)                                 \
        {                                               \
            count++;                                    \
            integer &= integer - 1;                     \
        }                                               \
    }

/** Stream skip len */
#define STREAM_SKIP_LEN(s, len)     {                   \
        s += len;                                       \
    }

/** Stream to array */
#define STREAM_TO_ARRAY(a, s, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *((uint8_t *)(a) + ii) = *s++;              \
        }                                               \
    }

/** Array to stream */
#define ARRAY_TO_STREAM(s, a, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *s++ = *((uint8_t *)(a) + ii);              \
        }                                               \
    }

/** Little Endian stream to uint8 */
#define LE_STREAM_TO_UINT8(u8, s)   {                   \
        u8  = (uint8_t)(*s);                            \
        s  += 1;                                        \
    }

/** Little Endian stream to uint16 */
#define LE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 0) +            \
               ((uint16_t)(*(s + 1)) << 8);             \
        s   += 2;                                       \
    }

/** Little Endian stream to uint24 */
#define LE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16);            \
        s   += 3;                                       \
    }

/** Little Endian stream to uint32 */
#define LE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16) +           \
               ((uint32_t)(*(s + 3)) << 24);            \
        s   += 4;                                       \
    }

/** Little Endian uint8 to stream */
#define LE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Little Endian uint16 to stream */
#define LE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
    }

/** Little Endian uint24 to stream */
#define LE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >>  0);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >> 16);                  \
    }

/** Little Endian uint32 to stream */
#define LE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >>  0);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >> 24);                  \
    }

/** Little Endian array to uint8 */
#define LE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Little Endian array to uint16 */
#define LE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 0) +             \
              ((uint16_t)(*(a + 1)) << 8);              \
    }

/** Little Endian array to uint24 */
#define LE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16);             \
    }

/** Little Endian array to uint32 */
#define LE_ARRAY_TO_UINT32(u32, a) {                    \
        u32 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16) +            \
              ((uint32_t)(*(a + 3)) << 24);             \
    }

/** Little Endian uint8 to array */
#define LE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Little Endian uint16 to array */
#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

/** Little Endian uint24 to array */
#define LE_UINT24_TO_ARRAY(a, u24) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >> 16); \
    }

/** Little Endian uint32 to array */
#define LE_UINT32_TO_ARRAY(a, u32) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >> 24); \
    }

/** Big Endian stream to uint8 */
#define BE_STREAM_TO_UINT8(u8, s)   {                   \
        u8   = (uint8_t)(*(s + 0));                     \
        s   += 1;                                       \
    }

/** Big Endian stream to uint16 */
#define BE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 8) +            \
               ((uint16_t)(*(s + 1)) << 0);             \
        s   += 2;                                       \
    }

/** Big Endian stream to uint24 */
#define BE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) << 16) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) <<  0);            \
        s   += 3;                                       \
    }

/** Big Endian stream to uint32 */
#define BE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) << 24) +           \
               ((uint32_t)(*(s + 1)) << 16) +           \
               ((uint32_t)(*(s + 2)) <<  8) +           \
               ((uint32_t)(*(s + 3)) <<  0);            \
        s   += 4;                                       \
    }

/** Big Endian uint8 to stream */
#define BE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Big Endian uint16 to stream */
#define BE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
    }

/** Big Endian uint24 to stream */
#define BE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >> 16);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >>  0);                  \
    }

/** Big Endian uint32 to stream */
#define BE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >> 24);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >>  0);                  \
    }

/** Big Endian array to uint8 */
#define BE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Big Endian array to uint16 */
#define BE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 8) +             \
              ((uint16_t)(*(a + 1)) << 0);              \
    }

/** Big Endian array to uint24 */
#define BE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) << 16) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) <<  0);             \
    }

/** Big Endian array to uint32 */
#define BE_ARRAY_TO_UINT32(u32, a)  {                   \
        u32 = ((uint32_t)(*(a + 0)) << 24) +            \
              ((uint32_t)(*(a + 1)) << 16) +            \
              ((uint32_t)(*(a + 2)) <<  8) +            \
              ((uint32_t)(*(a + 3)) <<  0);             \
    }

/** Big Endian uint8 to array */
#define BE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Big Endian uint16 to array */
#define BE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 8);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 0);  \
    }

/** Big Endian uint24 to array */
#define BE_UINT24_TO_ARRAY(a, u24)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >> 16); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >>  0); \
    }

/** Big Endian uint32 to array */
#define BE_UINT32_TO_ARRAY(a, u32)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >> 24); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >>  0); \
    }

#ifdef __cplusplus
}
#endif

#endif

#endif /* _BT_TYPES_H_ */
