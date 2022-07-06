/*
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __GAP_LEGACY_H__
#define __GAP_LEGACY_H__

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include <gap.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "upperstack_config.h"

#if F_BT_BREDR_SUPPORT
/**
 * \defgroup    GAP_BREDR              GAP BR/EDR
 *
 * \brief   Set BR/EDR parameters and provide BR/EDR link related funtions.
 * \details GAP Legacy Module manages BR/EDR only related parameters,
 *          also provides some APIs that can be used for BR/EDR role such as
 *          inquiry, sdp discovery, SCO connection, etc.\n
 *
 */

/**
 * gap_legacy.h
 *
 * \name  GAP_LINK_POLICY
 * \brief GAP Bluetooth BR/EDR link policy.
 * \anchor  GAP_LINK_POLICY
 *
 */
/**
 * \ingroup     GAP_BREDR
 */
#define GAP_LINK_POLICY_DISABLE_ALL             0x0000    /**< Disable all link policy */
#define GAP_LINK_POLICY_ROLE_SWITCH             0x0001    /**< Enable only role switch */
#define GAP_LINK_POLICY_SNIFF_MODE              0x0004    /**< Enable only sniff mode */

/**
 * gap_legacy.h
 *
 * \name  GAP_ACL_PACKET_TYPE
 * \brief GAP Bluetooth BR/EDR ACL packet type bitmap.
 * \anchor  GAP_ACL_PACKET_TYPE
 *
 */
/**
 * \ingroup     GAP_BREDR
 */
#define GAP_PKT_TYPE_NO_2DH1                    0x0002  /**< Packet type 2DH1 shall not be used */
#define GAP_PKT_TYPE_NO_3DH1                    0x0004  /**< Packet type 3DH1 shall not be used */
#define GAP_PKT_TYPE_DM1                        0x0008  /**< Packet type DM1 may be used */
#define GAP_PKT_TYPE_DH1                        0x0010  /**< Packet type DH1 may be used */
#define GAP_PKT_TYPE_NO_2DH3                    0x0100  /**< Packet type 2DH3 shall not be used */
#define GAP_PKT_TYPE_NO_3DH3                    0x0200  /**< Packet type 3DH3 shall not be used */
#define GAP_PKT_TYPE_DM3                        0x0400  /**< Packet type DM3 may be used */
#define GAP_PKT_TYPE_DH3                        0x0800  /**< Packet type DH3 may be used */
#define GAP_PKT_TYPE_NO_2DH5                    0x1000  /**< Packet type 2DH5 shall not be used */
#define GAP_PKT_TYPE_NO_3DH5                    0x2000  /**< Packet type 3DH5 shall not be used */
#define GAP_PKT_TYPE_DM5                        0x4000  /**< Packet type DM5 may be used */
#define GAP_PKT_TYPE_DH5                        0x8000  /**< Packet type DH5 may be used */

/**
 * gap_legacy.h
 *
 * \name  GAP_SCO_PACKET_TYPE
 * \brief GAP Bluetooth BR/EDR SCO packet type.
 * \anchor  GAP_SCO_PACKET_TYPE
 *
 */
/**
 * \ingroup     GAP_BREDR
 */
#define GAP_PKT_TYPE_HV1                        0x0001  /**< Packet type HV1 may be used */
#define GAP_PKT_TYPE_HV2                        0x0002  /**< Packet type HV2 may be used */
#define GAP_PKT_TYPE_HV3                        0x0004  /**< Packet type HV3 may be used */
#define GAP_PKT_TYPE_EV3                        0x0008  /**< Packet type EV3 may be used */
#define GAP_PKT_TYPE_EV4                        0x0010  /**< Packet type EV4 may be used */
#define GAP_PKT_TYPE_EV5                        0x0020  /**< Packet type EV5 may be used */
#define GAP_PKT_TYPE_NO_2EV3                    0x0040  /**< Packet type 2EV3 shall not be used */
#define GAP_PKT_TYPE_NO_3EV3                    0x0080  /**< Packet type 3EV3 shall not be used */
#define GAP_PKT_TYPE_NO_2EV5                    0x0100  /**< Packet type 2EV5 shall not be used */
#define GAP_PKT_TYPE_NO_3EV5                    0x0200  /**< Packet type 3EV5 shall not be used */

/**
 * gap_legacy.h
 *
 * \name  GAP_AIR_MODE
 * \brief Transmit air coding format on SCO/eSCO link.
 * \anchor  GAP_AIR_MODE
 */
/**
 * \ingroup     GAP_BREDR
 */
/**@{*/
#define GAP_AIR_MODE_U_LAW              0   //!< Air mode u-law log.
#define GAP_AIR_MODE_A_LAW              1  //!< Air mode A-law log.
#define GAP_AIR_MODE_CVSD               2   //!< Air mode CVSD.
#define GAP_AIR_MODE_TRANSPARENT        3   //!< Air mode transparent data.
/**@}*/

#define GAP_QOS_NO_TRAFFIC              0x00
#define GAP_QOS_BEST_EFFORT             0x01
#define GAP_QOS_GUARANTEED              0x02

/**
 * gap_legacy.h
 *
 * \brief GAP Bluetooth mode.
 *
 * \ingroup     GAP_BREDR
 */
typedef enum {
	GAP_BT_MODE_21DISABLED,    /**< Disable all Bluetooth 2.1 functionality. */
	GAP_BT_MODE_21ENABLED      /**< Enable Bluetooth 2.1 functionality. */
} T_GAP_BT_MODE;

/**
 * gap_legacy.h
 *
 * \brief GAP Bluetooth BR/EDR radio mode.
 *
 * \ingroup     GAP_BREDR
 */
typedef enum {
	GAP_RADIO_MODE_VISIABLE_CONNECTABLE = 0x01,  /**< Enable both inquiry scan and page scan. */
	GAP_RADIO_MODE_VISIABLE             = 0x02,  /**< Only enable inquiry scan. */
	GAP_RADIO_MODE_CONNECTABLE          = 0x03,  /**< Only enable page scan. */
	GAP_RADIO_MODE_NONE_DISCOVERABLE    = 0x04,  /**< Disable both inquiry scan and page scan. */
} T_GAP_RADIO_MODE;

/**
 * gap_legacy.h
 *
 * \brief GAP Bluetooth BR/EDR page scan type.
 *
 * \ingroup     GAP_BREDR
 */
typedef enum {
	GAP_PAGE_SCAN_TYPE_STANDARD  = 0,    /**< BR/EDR page scan type standard. */
	GAP_PAGE_SCAN_TYPE_INTERLACED,       /**< BR/EDR page scan type interlaced. */
} T_GAP_PAGE_SCAN_TYPE;

/**
 * gap_legacy.h
 *
 * \brief GAP Bluetooth BR/EDR inquiry scan type.
 *
 * \ingroup     GAP_BREDR
 */
typedef enum {
	GAP_INQUIRY_SCAN_TYPE_STANDARD = 0,  /**< BR/EDR inquiry scan type standard. */
	GAP_INQUIRY_SCAN_TYPE_INTERLACED,    /**< BR/EDR inquiry scan type interlaced. */
} T_GAP_INQUIRY_SCAN_TYPE;

/**
 * gap_legacy.h
 *
 * \brief GAP Bluetooth BR/EDR inquiry mode.
 *
 * \ingroup     GAP_BREDR
 */
typedef enum {
	GAP_INQUIRY_MODE_STANDARD_RESULT = 0, /**< Inquiry reusult standard format. */
	GAP_INQUIRY_MODE_RESULT_WITH_RSSI,    /**< Inquiry result format with RSSI. */
	GAP_INQUIRY_MODE_EXTENDED_RESULT,     /**< Inquiry result with RSSI format or Extended inquiry result format. */
} T_GAP_INQUIRY_MODE;

/**
 * gap_legacy.h
 *
 * \brief   GAP legacy parameter types.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	GAP_PARAM_BT_MODE,                /**< Bluetooth mode. <b>Write only. Parameter type is uint8_t. Valid values are \ref T_GAP_BT_MODE. Default value is \ref GAP_BT_MODE_21ENABLED.</b> */
	GAP_PARAM_LEGACY_NAME,            /**< BR/EDR name. <b>Write only. Name string length is \ref GAP_DEVICE_NAME_LEN.</b> */
	GAP_PARAM_COD,                    /**< Class of device. <b>Write only. Parameter type is uint32_t. Default value is 0x000900.</b> */
	GAP_PARAM_SUPV_TOUT,              /**< BR/EDR link supervision timeout value. <b>Write only. Parameter type is uint16_t. Default value is 0x7D00.</b> */
	GAP_PARAM_LINK_POLICY,            /**< Default link policy to set after link established. <b>Write only. Parameter type is uint8_t. Allowed values are combinations of \ref GAP_LINK_POLICY. Default value is \ref GAP_LINK_POLICY_ROLE_SWITCH | \ref GAP_LINK_POLICY_SNIFF_MODE.</b>*/
	GAP_PARAM_RADIO_MODE,             /**< BR/EDR radio mode. <b>Write only. Parameter type is uint8_t. Valid paramters are \ref T_GAP_RADIO_MODE. Default value is \ref GAP_RADIO_MODE_NONE_DISCOVERABLE.</b> */
	GAP_PARAM_LIMIT_DISCOV,           /**< Limited discoverable mode. <b>Write only. Parameter type is bool. Default value is false.</b> */
	GAP_PARAM_LIMIT_DISCOV_TOUT,      /**< Limited discoverable mode timeout value in seconds. Only valid when limited discoverable mode is enabled. <b>Write only. Parameter type is uint8_t. Default value is 60.</b> */
	GAP_PARAM_PAGE_SCAN_TYPE,         /**< BR/EDR page scan type. <b>Write only. Parameter type is uint8_t. Valid values are \ref T_GAP_PAGE_SCAN_TYPE. Default value is \ref GAP_PAGE_SCAN_TYPE_STANDARD.</b>*/
	GAP_PARAM_PAGE_SCAN_INTERVAL,     /**< BR/EDR page scan interval. <b>Write only. Parameter type is uint16_t. Default value is 0x800.</b>*/
	GAP_PARAM_PAGE_SCAN_WINDOW,       /**< BR/EDR page scan window. <b>Write only. Parameter type is uint16_t. Default value is 0x12.</b>*/
	GAP_PARAM_PAGE_TIMEOUT,           /**< BR/EDR page timeout value. <b>Write only. Parameter type is uint16_t. Default value is 0x2000.</b>*/
	GAP_PARAM_INQUIRY_SCAN_TYPE,      /**< BR/EDR inquiry scan type. <b>Write only. Parameter type is uint8_t. Valid values are \ref T_GAP_INQUIRY_SCAN_TYPE. Default value is \ref GAP_INQUIRY_SCAN_TYPE_STANDARD.</b>*/
	GAP_PARAM_INQUIRY_SCAN_INTERVAL,  /**< BR/EDR inquiry scan interval. <b>Write only. Parameter type is uint16_t. Default value is 0x1000.</b>*/
	GAP_PARAM_INQUIRY_SCAN_WINDOW,    /**< BR/EDR inquiry scan window. <b>Write only. Parameter type is uint16_t. Default value is 0x12.</b>*/
	GAP_PARAM_INQUIRY_MODE,           /**< BR/EDR inquiry mode. <b>Write only. Parameter type is uint8_t. Valid values are \ref T_GAP_INQUIRY_MODE. Default value is \ref GAP_INQUIRY_MODE_EXTENDED_RESULT.</b>*/
	GAP_PARAM_DATA_QUOTA,             /**< Max number of BR/EDR data to send without response. <b>Read only. Parameter type is uint8_t.</b>*/
	GAP_PARAM_AUTO_ACCEPT_ACL,        /**< Bluetooth stack accept ACL connection request automatically. <b>Write only. Parameter type is bool. default value is true.</b>*/
} T_GAP_LEGACY_PARAM_TYPE;

/**
 * gap_legacy.h
 *
 * \brief    BR/EDR link role.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	GAP_BR_LINK_ROLE_MASTER,   /**< BR/EDR link role master. */
	GAP_BR_LINK_ROLE_SLAVE     /**< BR/EDR link role slave. */
} T_GAP_BR_LINK_ROLE;

/**
 * gap_legacy.h
 *
 * \brief    Response of starting inquiry. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type
 *           as \ref INQUIRY_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< Result of start inquiry */
} T_GAP_INQUIRY_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Information of device found duing inquiry. It will be received in the
 *           callback function registered by \ref legacy_register_cb
 *           with message type as \ref INQUIRY_RESULT.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6];                 /**< Bluetooth address of device found when inquiry */
	uint32_t    cod;                        /**< Class of device of device found when inquiry */
	int8_t      rssi;                       /**< RSSI of device found when inquiry */
	uint8_t     name[GAP_DEVICE_NAME_LEN];  /**< Name of device found when inquiry */
	uint8_t     eir_len;
	uint8_t    *p_eir;
} T_GAP_INQUIRY_RESULT_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Information of inquiry procedure complete status. It will be received in the
 *           callback function registered by \ref legacy_register_cb with message
 *           type as \ref INQUIRY_CMPL.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< Status of inquiry procedure */
} T_GAP_INQUIRY_CMPL_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Response of canceling inquiry. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as
 *           \ref INQUIRY_CANCEL_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< Result of cancel inquiry */
} T_GAP_INQUIRY_CANCEL_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of starting SDP discovery. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as
 *           \ref SDP_DISCOV_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6];     /**< Bluetooth address of remote device */
	uint16_t        cause;          /**< Result of starting SDP discovery */
} T_GAP_SDP_DISCOV_RSP;

/**
 * gap_legacy.h
 *
 * \brief    SDP UUID type.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	GAP_UUID16,    /**< UUID type 16 bits */
	GAP_UUID32,    /**< UUID type 32 bits */
	GAP_UUID128    /**< UUID type 128 bits */
} T_GAP_UUID_TYPE;

/**
 * gap_legacy.h
 *
 * \brief    SDP UUID data.
 *
 * \ingroup  GAP_BREDR
 */
typedef union {
	uint16_t    uuid_16;        /**< UUID data in 16 bits */
	uint32_t    uuid_32;        /**< UUID data in 32 bits */
	uint8_t     uuid_128[16];   /**< UUID data in 128 bits */
} T_GAP_UUID_DATA;

/**
 * gap_legacy.h
 *
 * \brief    Information of attributes found during SDP discovery. It will be
 *           received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SDP_ATTR_INFO.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t                 bd_addr[6];          /**< Bluetooth address of remote device */
	uint8_t                 server_channel;      /**< Server channel number */
	uint8_t                 supported_repos;     /**< Supported repositories */
	uint16_t                l2c_psm;             /**< L2CAP PSM value */
	T_GAP_UUID_TYPE         srv_class_uuid_type; /**< Service class UUID type */
	T_GAP_UUID_DATA         srv_class_uuid_data; /**< Service class UUID data */
	uint16_t                remote_version;      /**< Remote profile version */
	uint16_t                supported_feat;      /**< Supported features */
	uint8_t                *p_attr;              /**< Address of attribute */
	uint16_t                attr_len;            /**< Attribute len */
} T_GAP_SDP_ATTR_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Information of DID attributes found during SDP discovery. It will be
 *           received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref DID_ATTR_INFO.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t        specific_id; /**< Specific ID of remote device */
	uint16_t        vendor_id;   /**< Vendor ID of remote device */
	uint16_t        vendor_src;  /**< Verndor source of remote device */
	uint16_t        product_id;  /**< Product ID of remote device */
	uint16_t        version;     /**< Version of remote device */
	uint8_t         bd_addr[6];  /**< Bluetooth address of remote device */
} T_GAP_DID_ATTR_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Information of SDP discovery complete. It will be received in the
 *           callback function registered by \ref legacy_register_cb with message
 *           type as \ref SDP_DISCOV_CMPL.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6];  /**< Bluetooth address of remote device */
	uint16_t        cause;       /**< Status of SDP discovery procedure */
} T_GAP_SDP_DISCOV_CMPL_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Response of stopping SDP discovery. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as
 *           \ref SDP_STOP_DISCOV_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6];  /**< Bluetooth address of remote device */
	uint16_t        cause;       /**< Result of stop SDP discovey */
} T_GAP_STOP_SDP_DISCOV_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Indication of SCO/eSCO connection request from remote side. It will be
 *           received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_CONN_IND.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t  bd_addr[6]; /**< Bluetooth address of remote device. */
	uint8_t  is_esco;    /**< Connection type remote device want to create. 1 for eSCO and 0 for SCO. */
} T_GAP_SCO_CONN_IND;

/**
 * gap_legacy.h
 *
 * \brief    Response of SCO/eSCO connection request from upper layer. It will be
 *           received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_CONN_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t    cause;      /**< Result of starting a SCO/eSCO connection. */
} T_GAP_SCO_CONN_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Result of SCO/eSCO connection establish procedure. It will be received
 *           in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_CONN_CMPL.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t    cause;      /**< Result of establishing the SCO/eSCO link. */
	uint16_t    handle;     /**< Connection handle of the SCO/eSCO link. */
	uint8_t     is_esco;    /**< Whether this link is a SCO/eSCO link. 1 for eSCO and 0 for SCO. */
	uint8_t     air_mode;   /**< Air mode of the SCO/eSCO link. */
	uint16_t    tx_length;  /**< Length of the eSCO payload in transmit direction. 0 for SCO links. */
	uint16_t    rx_length;  /**< Length of the eSCO payload in receive direction. 0 for SCO links. */
} T_GAP_SCO_CONN_CMPL_INFO;

/**
 * gap_legacy.h
 *
 * \brief    SCO packet status.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	GAP_SCO_PKT_STATUS_OK           = 0,   /**< SCO packet status for correctly received data */
	GAP_SCO_PKT_STATUS_INVALID      = 1,   /**< SCO packet status for invalid data */
	GAP_SCO_PKT_STATUS_NO_DATA      = 2,   /**< SCO packet status for no data received */
	GAP_SCO_PKT_STATUS_PARTIAL_LOST = 3    /**< SCO packet status for data partially lost */
} T_GAP_SCO_PKT_STATUS;

/**
 * gap_legacy.h
 *
 * \brief    BR/EDR reject connection request reason.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	GAP_ACL_REJECT_LIMITED_RESOURCE = 0x0D,
	GAP_ACL_REJECT_SECURITY_REASON = 0x0E,
	GAP_ACL_REJECT_UNACCEPTABLE_ADDR = 0x0F
} T_GAP_ACL_REJECT_CONN_REASON;

/**
 * gap_legacy.h
 *
 * \brief    Indication of SCO/eSCO data from remote side. It will be received
 *           in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_DATA_IND.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint32_t                bt_clock;   /**< Bluetooth native clock. */
	uint8_t                 bd_addr[6]; /**< Bluetooth address of remote device. */
	T_GAP_SCO_PKT_STATUS    status;     /**< Status of the SCO/eSCO data. */
	uint16_t                len;        /**< Length of the SCO/eSCO data. */
	uint16_t                gap;        /**< Offset from data parameter to the real SCO/eSCO data. */
	uint8_t                 data[1];    /**< Address of the real SCO/eSCO data at offset of gap. */
} T_GAP_SCO_DATA_IND;

/**
 * gap_legacy.h
 *
 * \brief    Indication of SCO/eSCO disconnect request from remote side. It will
 *           be received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_DISCONN_IND.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t    cause;      /**< Disconnect reason for the SCO/eSCO link. */
} T_GAP_SCO_DISCONN_IND;

/**
 * gap_legacy.h
 *
 * \brief    Response of SCO/eSCO disconnect request from upper layer. It will
 *           be received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_DISCONN_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t    cause;      /**< Result of disconnecting the SCO/eSCO link. */
	uint16_t    reason;     /**< Disconnect reason for the SCO/eSCO link. */
} T_GAP_SCO_DISCONN_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of SCO/eSCO link parameters change request from upper layer. It will be
 *           received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_PARAM_CHANGE_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t    cause;      /**< Result of change a SCO/eSCO link parameters. */
} T_GAP_SCO_PARAM_CHANGE_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Result of SCO/eSCO link parameters change procedure. It will be
 *           received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SCO_PARAM_CHANGE_INFO.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t    cause;          /**< Result of SCO/eSCO link parameters change procedure. */
	uint16_t    tx_length;      /**< Length of the eSCO payload in transmit direction. */
	uint16_t    rx_length;      /**< Length of the eSCO payload in receive direction. */
	uint8_t     trans_interval; /**< Time between two SCO/eSCO instants measured in slots. */
	uint8_t     retrans_window; /**< The size of the retransmission window measured in slots. */
} T_GAP_SCO_PARAM_CHANGE_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Information of result of getting name of remote device. It will be received
 *           in the callback function registered by \ref legacy_register_cb with message
 *           type as \ref REMOTE_NAME_INFO.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;                      /**< Result of getting remote device's name */
	uint8_t     bd_addr[6];                 /**< Bluetooth address of remote device */
	uint8_t     name[GAP_DEVICE_NAME_LEN];  /**< Name of remote device */
} T_GAP_REMOTE_NAME_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL link policy. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref LINK_POLICY_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link policy. */
	uint16_t        link_policy;/**< Current link policy. */
} T_GAP_ACL_CFG_LINK_POLICY_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL link supervision timeout. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref LINK_SUPV_TOUT_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link supervision timeout. */
} T_GAP_ACL_CFG_LINK_SUPV_TOUT_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL link role. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref ROLE_SWITCH_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link role. */
} T_GAP_ACL_CFG_LINK_ROLE_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL link packet type. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref PKT_TYPE_CHANGE_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link packet type. */
} T_GAP_ACL_CFG_PKT_TYPE_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL link flush timeout value. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref LINK_FLUSH_TOUT_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link flush timeout. */
} T_GAP_ACL_CFG_FLUSH_TOUT_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL to enter sniff mode. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref ENTER_SNIFF_MODE_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link to enter sniff mode. */
} T_GAP_ACL_ENTER_SNIFF_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring ACL to exit sniff mode. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type as \ref EXIT_SNIFF_MODE_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t         bd_addr[6]; /**< Bluetooth address of remote device. */
	uint16_t        cause;      /**< Result of configuring ACL link to exit sniff mode. */
} T_GAP_ACL_EXIT_SNIFF_RSP;

/**
 * gap_legacy.h
 *
 * \brief    GAP ACL status used in ACL status information message.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	GAP_ACL_CONN_ACTIVE        = 0x01,  /**< ACL link in active mode. */
	GAP_ACL_CONN_SNIFF         = 0x02,  /**< ACL link in sniff mode. */
	GAP_ACL_AUTHEN_START       = 0x03,  /**< ACL link authentication start. */
	GAP_ACL_AUTHEN_SUCCESS     = 0x04,  /**< ACL link authentication success. */
	GAP_ACL_AUTHEN_FAIL        = 0x05,  /**< ACL link authentication fail. */
	GAP_ACL_CONN_ENCRYPTED     = 0x06,  /**< ACL link encrypted. */
	GAP_ACL_CONN_DISCONN       = 0x07,  /**< ACL link disconnected. */
	GAP_ACL_CONN_NOT_ENCRYPTED = 0x08,  /**< ACL link not encrypted. */
	GAP_ACL_ADDR_RESOLVED      = 0x09,  /**< Address is resolved. */
	GAP_ACL_ROLE_MASTER        = 0x0A,  /**< ACL link role is master. */
	GAP_ACL_ROLE_SLAVE         = 0x0B,  /**< ACL link role is slave. */
	GAP_ACL_CONN_SNIFF_SUBRATE = 0x0C,  /**< ACL link in sniff subrate mode. */
	GAP_ACL_CONN_FAIL          = 0x0D,  /**< ACL link is failed to connect. */
	GAP_ACL_CONN_SUCCESS       = 0x0E,  /**< ACL link is connected successfully. */
	GAP_ACL_CONN_READY         = 0x0F,  /**< ACL link is ready to be configured. */
	GAP_ACL_CONN_BR_SC_START   = 0x10,  /**< BR/EDR Secure Connections information exchange is started. */
	GAP_ACL_CONN_BR_SC_CMPL    = 0x11,  /**< BR/EDR Secure Connections information exchange is finished. */
} T_GAP_ACL_STATUS;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_AUTHEN_START or \ref GAP_ACL_AUTHEN_SUCCESS or \ref
 *           GAP_ACL_AUTHEN_FAIL or \ref GAP_ACL_CONN_ENCRYPTED or \ref GAP_ACL_CONN_NOT_ENCRYPTED.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	T_GAP_KEY_TYPE      key_type;       /**< Key type of the link. */
	uint8_t             key_size;       /**< Key size of the link. */
	uint8_t             encrypt_type;   /**< Encryption type of the link. */
	uint16_t            cause;          /**< Authentication result. */
} T_GAP_ACL_STATUS_PARAM_AUTHEN;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_BR_SC_CMPL.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t             cause;         /**< BR/EDR Secure Connections information exchange result. */
} T_GAP_ACL_STATUS_PARAM_BR_SC_CMPL;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_ADDR_RESOLVED.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t                 bd_addr[6];         /**< Remote address. */
	T_GAP_REMOTE_ADDR_TYPE  remote_addr_type;   /**< Remote address type. */
} T_GAP_ACL_STATUS_PARAM_RESOLVE;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_SNIFF.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    interval;           /**< Sniff interval of the ACL link. */
} T_GAP_ACL_STATUS_PARAM_SNIFF;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_SNIFF_SUBRATE.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    max_tx_latency;  /**< Maximum latency for data transmitted to remote device. */
	uint16_t    max_rx_latency;  /**< Maximum latency for data received from remote device. */
	uint16_t    min_remote_tout; /**< Base sniff subrate timeout remote device shall use. */
	uint16_t    min_local_tout;  /**< Base sniff subrate timeout local device will use. */
} T_GAP_ACL_STATUS_PARAM_SNIFF_SUBRATE;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_DISCONN.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< ACL link disconnect reason. */
} T_GAP_ACL_CONNECTION_DISCONNECT;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_FAIL.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< ACL link establish fail reason. */
} T_GAP_ACL_CONNECTION_FAIL;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_SUCCESS.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    handle;     /**< ACL link handle. */
} T_GAP_ACL_CONNECTION_SUCCESS;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message when status is
 *           \ref GAP_ACL_CONN_READY.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    handle;     /**< ACL link handle. */
} T_GAP_ACL_CONNECTION_READY;

/**
 * gap_legacy.h
 *
 * \brief    Parameters used in ACL status information message.
 *
 * \ingroup  GAP_BREDR
 */
typedef union {
	T_GAP_ACL_CONNECTION_SUCCESS         conn_success; /**< Parameter for link success to establish. */
	T_GAP_ACL_CONNECTION_READY           conn_ready;   /**< Parameter for link ready to be used. */
	T_GAP_ACL_CONNECTION_FAIL            conn_fail;    /**< Parameter for link fail to establish. */
	T_GAP_ACL_CONNECTION_DISCONNECT      conn_disconn; /**< Parameter for link disconnected. */
	T_GAP_ACL_STATUS_PARAM_AUTHEN        authen;       /**< Parameter for authentication status. */
	T_GAP_ACL_STATUS_PARAM_RESOLVE       resolve;      /**< Address resolvable paramter. */
	T_GAP_ACL_STATUS_PARAM_SNIFF         sniff;        /**< Parameter for ACL link sniff mode. */
	T_GAP_ACL_STATUS_PARAM_SNIFF_SUBRATE sniff_subrate;/**< Parameter for ACL link sniff subrate. */
	T_GAP_ACL_STATUS_PARAM_BR_SC_CMPL    br_sc_cmpl;   /**< Parameter for BR SC complete. */
} T_GAP_ACL_STATUS_PARAM;

/**
 * gap_legacy.h
 *
 * \brief    ACL status information. It will be received in the callback function
 *           registered by \ref legacy_register_cb with message type as \ref ACL_STATUS_INFO.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t                    bd_addr[6];         /**< Bluetooth address of remote device. */
	T_GAP_REMOTE_ADDR_TYPE     remote_addr_type;   /**< Address type of remote device. */
	T_GAP_ACL_STATUS           status;             /**< ACL status information type. */
	T_GAP_ACL_STATUS_PARAM     p;                  /**< ACL status information parameter. */
} T_GAP_ACL_STATUS_INFO;

/**
 * gap_legacy.h
 *
 * \brief    Response of setup QOS for an ACL link. It will be received in the
 *           callback function registered by \ref legacy_register_cb with message
 *           type as \ref SETUP_QOS_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;  /**< Result of setup QOS for the ACL link. */
	uint16_t    handle; /**< Connection handle of the ACL link. */
} T_GAP_SETUP_QOS_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of ACL disconnect request from upper layer. It will
 *           be received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref ACL_DISCONN_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< Result of disconnecting ACL link. */
	uint16_t    reason;     /**< Disconnect reason for the ACL link. */
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_GAP_ACL_DISCONN_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of reading RSSI. It will be received in the callback function registered
 *           by \ref legacy_register_cb with message type as \ref READ_RSSI_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t                 bd_addr[6];       /**< Bluetooth address of remote device. */
	T_GAP_REMOTE_ADDR_TYPE  remote_addr_type; /**< Address type of remote device. */
	uint16_t                cause;            /**< Result of reading RSSI. */
	int8_t                  rssi;             /**< RSSI value read if success. */
} T_GAP_READ_RSSI_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Indication of ACL connection request from remote device. It will
 *           be received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref ACL_CONN_IND.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
	uint32_t    cod;
} T_GAP_ACL_CONN_IND;

/**
 * gap_legacy.h
 *
 * \brief   Response of set page with page scan simultaneous. It will
 *           be received in the callback function registered by \ref legacy_register_cb
 *           with message type as \ref SET_PAGE_WITH_SCAN_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;  /**< Response status of set page with page scan simultaneous.*/
} T_GAP_SET_PAGE_WITH_SCAN_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of setting radio mode. It will be received in the callback
 *           function registered by \ref legacy_register_cb with message type
 *           as \ref SET_RADIO_MODE_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;      /**< Result of set radio mode */
} T_GAP_RADIO_MODE_SET_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of configuring local device. It will be received in the
 *           callback function registered by \ref legacy_register_cb with message
 *           type as \ref SET_DEV_NAME_RSP or \ref SET_ACCEPT_ROLE_RSP or \ref SET_DID_EIR_RSP
 *           or \ref SET_EXT_EIR_RSP.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t                cause;  /**< Device configuration result. */
} T_GAP_DEV_CFG_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of add SDP record to Bluetooth stack. It will be received in the
 *           callback function registered by \ref legacy_register_cb with message
 *           type as \ref ADD_SDP_RECORD_RSP.
 *
 * \ingroup  GAP_LEGACY
 */
typedef struct {
	uint16_t                cause;  /**< Add SDP record result. */
	uint8_t                *p_buf;
} T_GAP_ADD_SDP_RECORD_RSP;

/**
 * gap_legacy.h
 *
 * \brief    Response of delete SDP record to Bluetooth stack. It will be received in the
 *           callback function registered by \ref legacy_register_cb with message
 *           type as \ref DEL_SDP_RECORD_RSP.
 *
 * \ingroup  GAP_LEGACY
 */
typedef struct {
	uint16_t                cause;  /**< Delete SDP record result. */
	uint8_t                *p_buf;
} T_GAP_DEL_SDP_RECORD_RSP;

/**
 * gap_legacy.h
 *
 * \brief   Response of set page with page scan simultaneous.
 *
 * \ingroup  GAP_BREDR
 */
typedef struct {
	uint16_t    cause;  /**< Response status of set page with page scan simultaneous.*/
} T_LEGACY_SET_PAGE_WITH_SCAN_RSP;

typedef struct {
	uint16_t    cause;
	uint8_t     lt_addr;
} T_GAP_LT_ADDR_SET_RSP;

typedef struct {
	uint16_t    cause;
	uint8_t     lt_addr;
} T_GAP_LT_ADDR_DEL_RSP;

typedef struct {
	uint16_t    cause;
	uint16_t    interval;
} T_GAP_SYNC_TRAIN_PARAM_SET_RSP;

typedef struct {
	uint16_t    cause;
} T_GAP_SYNC_TRAIN_RSP;

typedef struct {
	uint16_t    cause;
} T_GAP_SYNC_TRAIN_CMPL_INFO;

#define LEGACY_MSG_GROUP_DEV_CFG        0x0000
#define LEGACY_MSG_GROUP_INQUIRY        0x0100
#define LEGACY_MSG_GROUP_SDP            0x0200
#define LEGACY_MSG_GROUP_AUTHEN         0x0300
#define LEGACY_MSG_GROUP_ACL            0x0400
#define LEGACY_MSG_GROUP_SCO            0x0500
#define LEGACY_MSG_GROUP_VND            0xFF00

/**
 * gap_legacy.h
 *
 * \brief   Message types APP may get through callback function registered by \ref legacy_register_cb.
 *
 * \ingroup  GAP_BREDR
 */
typedef enum {
	PROTO_REG_CMPL              = (LEGACY_MSG_GROUP_DEV_CFG | 0x00),    /**< BR/EDR protocol register completely. This message should be the first one indicated to APP, meaning that BR/EDR role init successfully.*/
	SET_DEV_NAME_RSP            = (LEGACY_MSG_GROUP_DEV_CFG | 0x01),    /**< Response of set local device name. Message data is \ref T_GAP_DEV_CFG_RSP. */
	SET_RADIO_MODE_RSP          = (LEGACY_MSG_GROUP_DEV_CFG | 0x02),    /**< Response of set radio mode. Message data is \ref T_GAP_RADIO_MODE_SET_RSP. */
	SET_ACCEPT_ROLE_RSP         = (LEGACY_MSG_GROUP_DEV_CFG | 0x03),    /**< Response of set prefered link role to use when accepting connection request from remote device. Message data is \ref T_GAP_DEV_CFG_RSP. */
	SET_DID_EIR_RSP             = (LEGACY_MSG_GROUP_DEV_CFG | 0x04),    /**< Response of set DID information in EIR data. Message data is \ref T_GAP_DEV_CFG_RSP. */
	SET_EXT_EIR_RSP             = (LEGACY_MSG_GROUP_DEV_CFG | 0x05),    /**< Response of set user defined data in EIR. Message data is \ref T_GAP_DEV_CFG_RSP. */
	SET_DEFAULT_LINK_POLICY_RSP = (LEGACY_MSG_GROUP_DEV_CFG | 0x06),    /**< Response of set default link policy which will be set automatically after link established. Message data is \ref T_GAP_DEV_CFG_RSP. */
	SET_PAGE_SCAN_PARAM_RSP     = (LEGACY_MSG_GROUP_DEV_CFG | 0x07),    /**< Response of set page scan parameters. Message data is \ref T_GAP_DEV_CFG_RSP.*/
	SET_INQUIRY_SCAN_PARAM_RSP  = (LEGACY_MSG_GROUP_DEV_CFG | 0x08),    /**< Response of set inquiry scan parameters. Message data is \ref T_GAP_DEV_CFG_RSP.*/
	SET_SSP_DEBUG_MODE_RSP      = (LEGACY_MSG_GROUP_DEV_CFG | 0x09),    /**< Response of set ssp debug mode. Message data is \ref T_GAP_DEV_CFG_RSP.*/
	SET_ACL_DISCONN_TOUT_RSP    = (LEGACY_MSG_GROUP_DEV_CFG | 0x0A),    /**< Response of set timeout value to disconnect acl link after last l2cap channel disconnected. Message data is \ref T_GAP_DEV_CFG_RSP.*/
	SET_DEV_COD_RSP             = (LEGACY_MSG_GROUP_DEV_CFG | 0x0B),    /**< Response of set class of device. Message data is \ref T_GAP_DEV_CFG_RSP.*/
	ADD_SDP_RECORD_RSP          = (LEGACY_MSG_GROUP_DEV_CFG | 0x0C),    /**< Response of add sdp record. Message data is \ref T_GAP_ADD_SDP_RECORD_RSP.*/
	DEL_SDP_RECORD_RSP          = (LEGACY_MSG_GROUP_DEV_CFG | 0x0D),    /**< Response of delete sdp record. Message data is \ref T_GAP_DEL_SDP_RECORD_RSP.*/

	INQUIRY_RESULT      = (LEGACY_MSG_GROUP_INQUIRY | 0x00),    /**< Device information during inquiry. Message data is \ref T_GAP_INQUIRY_RESULT_INFO. */
	INQUIRY_RSP         = (LEGACY_MSG_GROUP_INQUIRY | 0x01),    /**< Response of start inquiry. Message data is \ref T_GAP_INQUIRY_RSP. */
	INQUIRY_CMPL        = (LEGACY_MSG_GROUP_INQUIRY | 0x02),    /**< Inquiry procedure finished. Message data is \ref T_GAP_INQUIRY_CMPL_INFO. */
	INQUIRY_CANCEL_RSP  = (LEGACY_MSG_GROUP_INQUIRY | 0x03),    /**< Response of stop inquiry. Message data is \ref T_GAP_INQUIRY_CANCEL_RSP. */

	SDP_DISCOV_RSP      = (LEGACY_MSG_GROUP_SDP | 0x00),        /**< Response of start SDP discovery. Message data is \ref T_GAP_SDP_DISCOV_RSP. */
	SDP_ATTR_INFO       = (LEGACY_MSG_GROUP_SDP | 0x01),        /**< SDP discovery information. Message data is \ref T_GAP_SDP_ATTR_INFO. */
	DID_ATTR_INFO       = (LEGACY_MSG_GROUP_SDP | 0x02),        /**< DID information discovered by SDP. Message data is \ref T_GAP_DID_ATTR_INFO. */
	SDP_DISCOV_CMPL     = (LEGACY_MSG_GROUP_SDP | 0x03),        /**< SDP discovery procedure finished. Message data is \ref T_GAP_SDP_DISCOV_CMPL_INFO. */
	SDP_STOP_DISCOV_RSP = (LEGACY_MSG_GROUP_SDP | 0x04),        /**< Response of stop SDP discovery. Message data is \ref T_GAP_STOP_SDP_DISCOV_RSP. */

	LEGACY_PAIR_RSP       = (LEGACY_MSG_GROUP_AUTHEN | 0x00),   /**< Response of BR/EDR pairing. Message data is \ref T_GAP_AUTHEN_RSP. */
	PASSKEY_REQ_REPLY_RSP = (LEGACY_MSG_GROUP_AUTHEN | 0x01),   /**< Response of passkey request reply. Message data is \ref T_GAP_USER_PASSKEY_REQ_REPLY_RSP. */
	KEYPRESS_NOTIF_RSP    = (LEGACY_MSG_GROUP_AUTHEN | 0x02),   /**< Response of keypress notification request. Message data is \ref T_GAP_KEYPRESS_NOTIF_RSP. */
	KEYPRESS_NOTIF_INFO   = (LEGACY_MSG_GROUP_AUTHEN | 0x03),   /**< Remote keypress notification information. Message data is \ref T_GAP_KEYPRESS_NOTIF_INFO. */
	LEGACY_OOB_INPUT      = (LEGACY_MSG_GROUP_AUTHEN | 0x04),   /**< Request APP to input BR/EDR OOB data. Message data is \ref T_GAP_LEGACY_REMOTE_OOB_REQ_IND. */
	USER_CONFIRM_REQ      = (LEGACY_MSG_GROUP_AUTHEN | 0x05),   /**< Request APP to confirm pairing display value. Message data is \ref T_GAP_USER_CFM_REQ_IND. */
	PASSKEY_INPUT         = (LEGACY_MSG_GROUP_AUTHEN | 0x06),   /**< Request APP to input passkey. Message data is \ref T_GAP_USER_PASSKEY_REQ_IND. */
	PASSKEY_DISPLAY       = (LEGACY_MSG_GROUP_AUTHEN | 0x07),   /**< Display passkey. Message data is \ref T_GAP_USER_PASSKEY_NOTIF_INFO. */
	LOCAL_OOB_RSP         = (LEGACY_MSG_GROUP_AUTHEN | 0x08),   /**< Response of read local oob. Message data is \ref T_GAP_LOCAL_OOB_RSP. */
	LINK_KEY_INFO         = (LEGACY_MSG_GROUP_AUTHEN | 0x09),   /**< Notification of new link key. Message data is \ref T_GAP_LINK_KEY_INFO. */
	LINK_KEY_REQ_IND      = (LEGACY_MSG_GROUP_AUTHEN | 0x0A),   /**< Indication of link key request during authentication. Message data is \ref T_GAP_LINK_KEY_REQ_IND. */
	PIN_CODE_REQ_IND      = (LEGACY_MSG_GROUP_AUTHEN | 0x0B),   /**< Indication of pin code request during authentication. Message data is \ref T_GAP_PIN_CODE_REQ_IND. */

	REMOTE_NAME_INFO     = (LEGACY_MSG_GROUP_ACL | 0x00),   /**< Result of get remote name. Message data is \ref T_GAP_REMOTE_NAME_INFO. */
	ACL_STATUS_INFO      = (LEGACY_MSG_GROUP_ACL | 0x01),   /**< ACL status information. Message data is \ref T_GAP_ACL_STATUS_INFO. */
	LINK_POLICY_RSP      = (LEGACY_MSG_GROUP_ACL | 0x02),   /**< Response of set ACL link policy. Message data is \ref T_GAP_ACL_CFG_LINK_POLICY_RSP. */
	LINK_SUPV_TOUT_RSP   = (LEGACY_MSG_GROUP_ACL | 0x03),   /**< Response of set ACL link supervision timeout value. Message data is \ref T_GAP_ACL_CFG_LINK_SUPV_TOUT_RSP. */
	LINK_FLUSH_TOUT_RSP  = (LEGACY_MSG_GROUP_ACL | 0x04),   /**< Response of set ACL link flush timeout value. Message data is \ref T_GAP_ACL_CFG_FLUSH_TOUT_RSP. */
	ROLE_SWITCH_RSP      = (LEGACY_MSG_GROUP_ACL | 0x05),   /**< Response of role switch. Message data is \ref T_GAP_ACL_CFG_LINK_ROLE_RSP. */
	ENTER_SNIFF_MODE_RSP = (LEGACY_MSG_GROUP_ACL | 0x06),   /**< Response of enter sniff mode. Message data is \ref T_GAP_ACL_ENTER_SNIFF_RSP. */
	EXIT_SNIFF_MODE_RSP  = (LEGACY_MSG_GROUP_ACL | 0x07),   /**< Response of exit sniff mode. Message data is \ref T_GAP_ACL_EXIT_SNIFF_RSP. */
	PKT_TYPE_CHANGE_RSP  = (LEGACY_MSG_GROUP_ACL | 0x08),   /**< Response of change connection packet type. Message data is \ref T_GAP_ACL_CFG_PKT_TYPE_RSP. */
	SETUP_QOS_RSP        = (LEGACY_MSG_GROUP_ACL | 0x09),   /**< Response of setup quality of service. Message data is \ref T_GAP_SETUP_QOS_RSP. */
	ACL_DISCONN_RSP      = (LEGACY_MSG_GROUP_ACL | 0x0A),   /**< Response of disconnect ACL link. Message data is \ref T_GAP_ACL_DISCONN_RSP. */
	READ_RSSI_RSP        = (LEGACY_MSG_GROUP_ACL | 0x0C),   /**< Response of read legacy RSSI. Message data is \ref T_GAP_READ_RSSI_RSP. */
	ACL_CONN_IND         = (LEGACY_MSG_GROUP_ACL | 0x0D),   /**< Indication of connection request from remote device. Message data is \ref T_GAP_ACL_CONN_IND. Need to set \ref GAP_PARAM_AUTO_ACCEPT_ACL to false. */

	SCO_CONN_IND          = (LEGACY_MSG_GROUP_SCO | 0x00),  /**< SCO/eSCO connection indicate. Message data is \ref T_GAP_SCO_CONN_IND. */
	SCO_CONN_RSP          = (LEGACY_MSG_GROUP_SCO | 0x01),  /**< SCO/eSCO connection response. Message data is \ref T_GAP_SCO_CONN_RSP. */
	SCO_CONN_CMPL         = (LEGACY_MSG_GROUP_SCO | 0x02),  /**< SCO/eSCO connection establish result. Message data is \ref T_GAP_SCO_CONN_CMPL_INFO. */
	SCO_PARAM_CHANGE_RSP  = (LEGACY_MSG_GROUP_SCO | 0x03),  /**< SCO/eSCO connection parameters change response. Message data is \ref T_GAP_SCO_PARAM_CHANGE_RSP. */
	SCO_PARAM_CHANGE_INFO = (LEGACY_MSG_GROUP_SCO | 0x04),  /**< SCO/eSCO connection parameters change result. Message data is \ref T_GAP_SCO_PARAM_CHANGE_INFO. */
	SCO_DATA_IND          = (LEGACY_MSG_GROUP_SCO | 0x05),  /**< SCO/eSCO data indicate. Message data is \ref T_GAP_SCO_DATA_IND. */
	SCO_DISCONN_IND       = (LEGACY_MSG_GROUP_SCO | 0x06),  /**< SCO/eSCO disconnection indicate. Message data is \ref T_GAP_SCO_DISCONN_IND. */
	SCO_DISCONN_RSP       = (LEGACY_MSG_GROUP_SCO | 0x07),  /**< SCO/eSCO disconnection response. Message data is \ref T_GAP_SCO_DISCONN_RSP. */

	VENDOR_EVT             = (LEGACY_MSG_GROUP_VND | 0x00), /**< Information of vendor event. Message data is \ref T_GAP_VENDOR_EVT_INFO */
	SET_PAGE_WITH_SCAN_RSP = (LEGACY_MSG_GROUP_VND | 0x01), /**< Response of set page with scan response simultaneous. Message data is \ref T_GAP_SET_PAGE_WITH_SCAN_RSP. */
} T_LEGACY_MSG;

/**
 * gap_legacy.h
 *
 * \brief   Prototype of callback function to handle GAP legacy messages.
 * \details GAP legacy message types are defined in \ref T_LEGACY_MSG. Message data
 *          is located at \c p_buf and the content of data is dependent on message type.\n
 *
 * \ingroup  GAP_BREDR
 */
typedef void (* P_LEGACY_CB)(void *p_buf, T_LEGACY_MSG msg);

/**
 * gap_legacy.h
 *
 * \brief   Init GAP legacy role.
 *
 * \param[in]   void.
 *
 * \return      None.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       legacy_gap_init();

       // ...
       // Create app task.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
void legacy_gap_init(void);

/**
 * gap_legacy.h
 *
 * \brief    Register a callback function to handle GAP legacy messages defined
 *           as \ref T_LEGACY_MSG.
 *
 * \param[in]  p_func    Callback function to handle GAP legacy messages. The
 *                       function must have the prototype defined as \ref P_LEGACY_CB.
 *
 * \return    None.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               APP_PRINT_TRACE0("GAP legacy was initialized");
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
void legacy_register_cb(P_LEGACY_CB p_func);

/**
 * gap_legacy.h
 *
 * \brief    Register a SDP record to BT stack. If this function is called before BT stack
 *           started, the record will be registered to BT stack during start procedure.
 *           If this function is called after BT stack started, the record will be registered
 *           immediately and \ref ADD_SDP_RECORD_RSP message will be received in GAP legacy
 *           callback function registed by \ref legacy_register_cb to indicate the result.
 *
 * \param[in]  p_buf    Address of SDP record buffer. <b>The buffer shall not
 *                      be released since BT stack will record the buffer address
 *                      instead of copy the whole buffer.</b>
 *
 * \return    Result of sending register SDP record request.
 * \retval    GAP_CAUSE_SUCCESS          Request was sent successfully.
 * \retval    GAP_CAUSE_NO_RESOURCE      No resource for the record.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED  Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   //DID SDP record buffer
   const uint8_t did_sdp_record[] =
   {
       SDP_DATA_ELEM_SEQ_HDR,
       0x4D,
       //attribute SDP_ATTR_SRV_CLASS_ID_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
       (uint8_t)SDP_ATTR_SRV_CLASS_ID_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x03,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PNP_INFORMATION >> 8),
       (uint8_t)(UUID_PNP_INFORMATION),

       //attribute SDP_ATTR_BROWSE_GROUP_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_BROWSE_GROUP_LIST >> 8),
       (uint8_t)SDP_ATTR_BROWSE_GROUP_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x03,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PUBLIC_BROWSE_GROUP >> 8),
       (uint8_t)UUID_PUBLIC_BROWSE_GROUP,

       //attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
       (uint8_t)SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x09,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_LANG_ENGLISH >> 8),
       (uint8_t)SDP_LANG_ENGLISH,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_CHARACTER_UTF8 >> 8),
       (uint8_t)SDP_CHARACTER_UTF8,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_BASE_LANG_OFFSET >> 8),
       (uint8_t)SDP_BASE_LANG_OFFSET,

       //attribute SDP_ATTR_PROFILE_DESC_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_PROFILE_DESC_LIST >> 8),
       (uint8_t)SDP_ATTR_PROFILE_DESC_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x08,
       SDP_DATA_ELEM_SEQ_HDR,
       0x06,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PNP_INFORMATION >> 8),
       (uint8_t)UUID_PNP_INFORMATION,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,//version 1.3
       0x03,

       //attribute SDP_ATTR_DIP_SPECIFICATION_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_SPECIFICATION_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_SPECIFICATION_ID,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,
       0x03,

       //attribute SDP_ATTR_DIP_VENDOR_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_VENDOR_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_VENDOR_ID,
       SDP_UNSIGNED_TWO_BYTE,
       //0x00,//0x005D : RealTek
       //0x5D,
       (uint8_t)(RTK_COMPANY_ID >> 8),
       (uint8_t)RTK_COMPANY_ID,

       //attribute SDP_ATTR_DIP_PRODUCT_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRODUCT_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_PRODUCT_ID,
       SDP_UNSIGNED_TWO_BYTE,
       0x22,//8763
       0x3B,

       //attribute SDP_ATTR_DIP_PRODUCT_VERSION
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRODUCT_VERSION >> 8),
       (uint8_t)SDP_ATTR_DIP_PRODUCT_VERSION,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,// 1.0.0
       0x00,

       //attribute SDP_ATTR_DIP_PRIMARY_RECORD
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRIMARY_RECORD >> 8),
       (uint8_t)SDP_ATTR_DIP_PRIMARY_RECORD,
       SDP_BOOL_ONE_BYTE,
       true,

       //attribute SDP_ATTR_DIP_VENDOR_ID_SOURCE
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_VENDOR_ID_SOURCE >> 8),
       (uint8_t)SDP_ATTR_DIP_VENDOR_ID_SOURCE,
       SDP_UNSIGNED_TWO_BYTE,
       0x00,//Bluetooth SIG
       0x01
   };

   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg
       {
       case PROTO_REG_CMPL:
           {
               legacy_add_sdp_record(did_sdp_record);
           }
           break;

       case ADD_SDP_RECORD_RSP:
           {
               T_GAP_ADD_SDP_RECORD_RSP *p_rsp = (T_GAP_ADD_SDP_RECORD_RSP *)buf;
               APP_PRINT_TRACE1("Add SDP record response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_LEGACY
 */
T_GAP_CAUSE legacy_add_sdp_record(void *p_buf);

/**
 * gap_legacy.h
 *
 * \brief    Unregister a SDP record to BT stack. If this function is called before BT stack
 *           started, the record will not be registered to BT stack during start procedure.
 *           If this function is called after BT stack started, the record will be unregistered
 *           immediately and \ref DEL_SDP_RECORD_RSP message will be received in GAP legacy
 *           callback function registed by \ref legacy_register_cb to indicate the result.
 *
 * \param[in]  p_buf    Address of SDP record buffer.
 *
 * \return    Result of sending unregister SDP record request.
 * \retval    GAP_CAUSE_SUCCESS          Request was sent successfully.
 * \retval    GAP_CAUSE_INVALID_PARAM    The record was not registered before.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED  Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   //DID SDP record buffer
   const uint8_t did_sdp_record[] =
   {
       SDP_DATA_ELEM_SEQ_HDR,
       0x4D,
       //attribute SDP_ATTR_SRV_CLASS_ID_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
       (uint8_t)SDP_ATTR_SRV_CLASS_ID_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x03,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PNP_INFORMATION >> 8),
       (uint8_t)(UUID_PNP_INFORMATION),

       //attribute SDP_ATTR_BROWSE_GROUP_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_BROWSE_GROUP_LIST >> 8),
       (uint8_t)SDP_ATTR_BROWSE_GROUP_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x03,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PUBLIC_BROWSE_GROUP >> 8),
       (uint8_t)UUID_PUBLIC_BROWSE_GROUP,

       //attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
       (uint8_t)SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x09,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_LANG_ENGLISH >> 8),
       (uint8_t)SDP_LANG_ENGLISH,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_CHARACTER_UTF8 >> 8),
       (uint8_t)SDP_CHARACTER_UTF8,
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_BASE_LANG_OFFSET >> 8),
       (uint8_t)SDP_BASE_LANG_OFFSET,

       //attribute SDP_ATTR_PROFILE_DESC_LIST
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_PROFILE_DESC_LIST >> 8),
       (uint8_t)SDP_ATTR_PROFILE_DESC_LIST,
       SDP_DATA_ELEM_SEQ_HDR,
       0x08,
       SDP_DATA_ELEM_SEQ_HDR,
       0x06,
       SDP_UUID16_HDR,
       (uint8_t)(UUID_PNP_INFORMATION >> 8),
       (uint8_t)UUID_PNP_INFORMATION,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,//version 1.3
       0x03,

       //attribute SDP_ATTR_DIP_SPECIFICATION_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_SPECIFICATION_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_SPECIFICATION_ID,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,
       0x03,

       //attribute SDP_ATTR_DIP_VENDOR_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_VENDOR_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_VENDOR_ID,
       SDP_UNSIGNED_TWO_BYTE,
       //0x00,//0x005D : RealTek
       //0x5D,
       (uint8_t)(RTK_COMPANY_ID >> 8),
       (uint8_t)RTK_COMPANY_ID,

       //attribute SDP_ATTR_DIP_PRODUCT_ID
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRODUCT_ID >> 8),
       (uint8_t)SDP_ATTR_DIP_PRODUCT_ID,
       SDP_UNSIGNED_TWO_BYTE,
       0x22,//8763
       0x3B,

       //attribute SDP_ATTR_DIP_PRODUCT_VERSION
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRODUCT_VERSION >> 8),
       (uint8_t)SDP_ATTR_DIP_PRODUCT_VERSION,
       SDP_UNSIGNED_TWO_BYTE,
       0x01,// 1.0.0
       0x00,

       //attribute SDP_ATTR_DIP_PRIMARY_RECORD
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_PRIMARY_RECORD >> 8),
       (uint8_t)SDP_ATTR_DIP_PRIMARY_RECORD,
       SDP_BOOL_ONE_BYTE,
       true,

       //attribute SDP_ATTR_DIP_VENDOR_ID_SOURCE
       SDP_UNSIGNED_TWO_BYTE,
       (uint8_t)(SDP_ATTR_DIP_VENDOR_ID_SOURCE >> 8),
       (uint8_t)SDP_ATTR_DIP_VENDOR_ID_SOURCE,
       SDP_UNSIGNED_TWO_BYTE,
       0x00,//Bluetooth SIG
       0x01
   };

   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg
       {
       case PROTO_REG_CMPL:
           {
               legacy_del_sdp_record(did_sdp_record);
           }
           break;

       case DEL_SDP_RECORD_RSP:
           {
               T_GAP_DEL_SDP_RECORD_RSP *p_rsp = (T_GAP_DEL_SDP_RECORD_RSP *)buf;
               APP_PRINT_TRACE1("Delete SDP record response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_del_sdp_record(void *p_buf);

/**
 * gap_legacy.h
 *
 * \brief   Set a GAP legacy parameter. GAP legacy parameter type(\ref T_GAP_LEGACY_PARAM_TYPE)
 *          should be used to declare which parameter will be set. Length is also used
 *          to declare the size of parameter to be set and it should match the parameter type.
 *          <b>Using \ref legacy_set_gap_param to set parameters should be done before start BT stack
 *          by \ref gap_start_bt_stack.</b>
 *
 * \param[in]  type     The type of GAP legacy parameter(\ref T_GAP_LEGACY_PARAM_TYPE) to set.
 * \param[in]  len      Length of data to set.
 * \param[in]  p_value  Address of data to set.
 *
 * \return    The status of setting GAP legacy parameter.
 * \retval    GAP_CAUSE_SUCCESS          The parameter was set succesfully.
 * \retval    GAP_CAUSE_INVALID_PARAM    The parameter was failed to set because of wrong type or len.
 *
 * <b>Example usage</b>
   \code{.c}
   #define DEFAULT_PAGESCAN_WINDOW             0x12
   #define DEFAULT_PAGESCAN_INTERVAL           0x0756
   #define DEFAULT_PAGE_TIMEOUT                0x2000
   #define DEFAULT_INQUIRYSCAN_WINDOW          0x12
   #define DEFAULT_INQUIRYSCAN_INTERVAL        0x0456

   //Set GAP legacy parameters
   void app_set_gap_param(void)
   {
       uint8_t radio_mode = GAP_RADIO_MODE_NONE_DISCOVERABLE;
       bool limited_discoverable = false;

       uint8_t pagescan_type = GAP_PAGE_SCAN_TYPE_STANDARD;
       uint16_t pagescan_interval = DEFAULT_PAGESCAN_INTERVAL;
       uint16_t pagescan_window = DEFAULT_PAGESCAN_WINDOW;
       uint16_t page_timeout = DEFAULT_PAGE_TIMEOUT;

       uint8_t inquiryscan_type = GAP_INQUIRY_SCAN_TYPE_STANDARD;
       uint16_t inquiryscan_window = DEFAULT_INQUIRYSCAN_WINDOW;
       uint16_t inquiryscan_interval = DEFAULT_INQUIRYSCAN_INTERVAL;
       uint8_t inquiry_mode = GAP_INQUIRY_MODE_EXTENDED_RESULT;

       legacy_set_gap_param(GAP_PARAM_RADIO_MODE, sizeof(uint8_t), &radio_mode);
       legacy_set_gap_param(GAP_PARAM_LIMIT_DISCOV, sizeof(bool), &limited_discoverable);

       legacy_set_gap_param(GAP_PARAM_PAGE_SCAN_TYPE, sizeof(uint8_t), &pagescan_type);
       legacy_set_gap_param(GAP_PARAM_PAGE_SCAN_INTERVAL, sizeof(uint16_t), &pagescan_interval);
       legacy_set_gap_param(GAP_PARAM_PAGE_SCAN_WINDOW, sizeof(uint16_t), &pagescan_window);
       legacy_set_gap_param(GAP_PARAM_PAGE_TIMEOUT, sizeof(uint16_t), &page_timeout);

       legacy_set_gap_param(GAP_PARAM_INQUIRY_SCAN_TYPE, sizeof(uint8_t), &inquiryscan_type);
       legacy_set_gap_param(GAP_PARAM_INQUIRY_SCAN_INTERVAL, sizeof(uint16_t), &inquiryscan_interval);
       legacy_set_gap_param(GAP_PARAM_INQUIRY_SCAN_WINDOW, sizeof(uint16_t), &inquiryscan_window);
       legacy_set_gap_param(GAP_PARAM_INQUIRY_MODE, sizeof(uint8_t), &inquiry_mode);
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       app_set_gap_param();

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_gap_param(T_GAP_LEGACY_PARAM_TYPE type, uint8_t len, void *p_value);

/**
 * gap_legacy.h
 *
 * \brief   Get a GAP legacy parameter. GAP legacy parameter type(\ref T_GAP_LEGACY_PARAM_TYPE)
 *          should be used to declare which parameter will be get.
 *
 * \param[in]   type     The type of GAP legacy parameter(\ref T_GAP_LEGACY_PARAM_TYPE) to get.
 * \param[out]  p_value  Pointer to location to get the value. It should match the parameter length to get.
 *
 * \return    The status of getting GAP legacy parameter.
 * \retval    GAP_CAUSE_SUCCESS          The parameter was get successfully.
 * \retval    GAP_CAUSE_INVALID_PARAM    The parameter was failed to get because of wrong type.
 *
 * <b>Example usage</b>
   \code{.c}

   int test(void)
   {
       uint8_t quota = 0;

       // ...
       // Init GAP legacy module.
       // ...

       legacy_get_gap_param(GAP_PARAM_DATA_QUOTA, &quota);
       APP_PRINT_TRACE1("GAP legacy buffer quota is %d", quota);

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_get_gap_param(T_GAP_LEGACY_PARAM_TYPE type, void *p_value);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to start BR/EDR inquiry. If the request was successfully
 *           sent, multiple messages will be received in GAP legacy callback function
 *           registed by \ref legacy_register_cb. The first one is \ref INQUIRY_RSP
 *           which indicates whether inquiry was started successfully. If inquiry
 *           was started successfully, one or more \ref INQUIRY_RESULT may be received,
 *           each message represents a remote device infomation.
 *           After inquiry procedure is done, \ref INQUIRY_CMPL will be received.
 *           <b>Inquiry procedure can only be started after receiving \ref PROTO_REG_CMPL
 *           message in GAP legacy callback.</b>
 *
 * \param[in]  limited_inquiry    Start a limited inquiry or not.
 * \arg \c     true               Start a limited inquiry.
 * \arg \c     false              Start a general inquiry.
 * \param[in]  timeout            Inquiry duration in 1.28s. The real inquiry time is timeout*1.28s.
 *
 * \return    The status of sending start BR/EDR inquiry request.
 * \retval    GAP_CAUSE_SUCCESS          Request was sent successfully.
 * \retval    GAP_CAUSE_INVALID_STATE    Unable to send request because already in inquiry state.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED  Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_start_inquiry(false, 8);
           }
           break;

       case INQUIRY_RSP:
           {
               T_GAP_INQUIRY_RSP *p_rsp = (T_GAP_INQUIRY_RSP *)buf;
               APP_PRINT_TRACE1("Inquiry response: status 0x%04x", p_rsp->cause);
           }
           break;

       case INQUIRY_RESULT:
           {
               T_GAP_INQUIRY_RESULT_INFO *p_info = (T_GAP_INQUIRY_RESULT_INFO *)buf;
               APP_PRINT_TRACE2("Inqury result: remote addr %s, name %s",
                                TRACE_BDADDR(p_info->bd_addr), TRACE_STRING(p_info->name));
           }
           break;

       case INQUIRY_CMPL:
           {
               T_GAP_INQUIRY_CMPL_INFO *p_info = (T_GAP_INQUIRY_CMPL_INFO *)buf;
               APP_PRINT_TRACE1("Inquiry complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_start_inquiry(bool limited_inquiry, uint8_t timeout);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to stop BR/EDR inquiry. If the request was send successfully,
 *           \ref INQUIRY_CANCEL_RSP message will be received in GAP legacy callback
 *           function registed by \ref legacy_register_cb to indicate whether inquiry
 *           was successfully stopped. If inquiry was successfully stopped, no
 *           \ref INQUIRY_CMPL message will be received.
 *
 * \param[in]  void.
 *
 * \return    The status of sending stop BR/EDR inquiry request.
 * \retval    GAP_CAUSE_SUCCESS          Request was successfully sent.
 * \retval    GAP_CAUSE_INVALID_STATE    Unable to send the request because not in inquiry state.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED  Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_start_inquiry(false, 8);
           }
           break;

       case INQUIRY_RSP:
           {
               T_GAP_INQUIRY_RSP *p_rsp = (T_GAP_INQUIRY_RSP *)buf;
               APP_PRINT_TRACE1("Inquiry response: status 0x%04x", p_rsp->cause);

               if (!p_rsp->cause)
               {
                   legacy_stop_inquiry();
               }
           }
           break;

       case INQUIRY_CANCEL_RSP:
           {
               T_GAP_INQUIRY_CANCEL_RSP *p_rsp = (T_GAP_INQUIRY_CANCEL_RSP *)buf;
               APP_PRINT_TRACE1("Stop inquiry response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_stop_inquiry(void);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to start SDP discovery. If the request was successfully
 *           sent, multiple messages will be received in GAP legacy callback function
 *           registed by \ref legacy_register_cb. The first one is \ref SDP_DISCOV_RSP
 *           which indicates whether SDP discovery was started successfully. If SDP discovery
 *           was started successfully, one or more \ref SDP_ATTR_INFO may be received.
 *           After SDP discovery procedure is done, \ref SDP_DISCOV_CMPL will be received.
 *           <b>SDP discovery procedure can only be started after receiving \ref PROTO_REG_CMPL
 *           message in GAP legacy callback. Only one SDP/DID discovery can be execute
 *           at the same time.</b>
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 * \param[in]  uuid_type        Type of UUID to discovery.
 * \arg \c     GAP_UUID16      UUID in 16 bits.
 * \arg \c     GAP_UUID32      UUID in 32 bits.
 * \arg \c     GAP_UUID128     UUID in 128 bits.
 * \param[in]  uuid_data        Discovery UUID data corresponding to uuid_type.
 *
 * \return    The status of sending start SDP discovery request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               T_GAP_UUID_DATA uuid;

               uuid.uuid_16 = UUID_HANDSFREE;
               legacy_start_sdp_discov(bd_addr, GAP_UUID16, uuid);
           }
           break;

       case SDP_DISCOV_RSP:
           {
               T_GAP_SDP_DISCOV_RSP *p_rsp = (T_GAP_SDP_DISCOV_RSP *)buf;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);
           }
           break;

       case SDP_ATTR_INFO:
           {
               T_GAP_SDP_ATTR_INFO *p_info = (T_GAP_SDP_ATTR_INFO *)buf;
               APP_PRINT_TRACE2("SDP discovery info: remote addr %s, remote version 0x%04x",
                                TRACE_BDADDR(p_info->bd_addr), p_info->remote_version);
           }
           break;

       case SDP_DISCOV_CMPL:
           {
               T_GAP_SDP_DISCOV_CMPL_INFO *p_info = (T_GAP_SDP_DISCOV_CMPL_INFO *)buf;
               APP_PRINT_TRACE1("SDP discovery complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_start_sdp_discov(uint8_t *bd_addr, T_GAP_UUID_TYPE uuid_type,
									T_GAP_UUID_DATA uuid_data);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to stop SDP discovery. If the request was successfully
 *           sent, \ref SDP_STOP_DISCOV_RSP message will be received in GAP legacy
 *           callback function registed by \ref legacy_register_cb to indicate
 *           whether SDP discovery was successfully stopped. <b>If SDP discovery was
 *           successfully stopped, no \ref SDP_DISCOV_CMPL message will be received.</b>
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 *
 * \return    The status of sending stop SDP discovery request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               T_GAP_UUID_DATA uuid;

               uuid.uuid_16 = UUID_HANDSFREE;
               legacy_start_sdp_discov(bd_addr, GAP_UUID16, uuid);
           }
           break;

       case SDP_DISCOV_RSP:
           {
               T_GAP_SDP_DISCOV_RSP *p_rsp = (T_GAP_SDP_DISCOV_RSP *)buf;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);

               if (!p_rsp->cause)
               {
                   legacy_stop_sdp_discov(p_rsp->bd_addr);
               }
           }
           break;

       case SDP_STOP_DISCOV_RSP:
           {
               T_GAP_STOP_SDP_DISCOV_RSP *p_rsp = (T_GAP_STOP_SDP_DISCOV_RSP *)buf;
               APP_PRINT_TRACE1("Stop SDP discovery response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_stop_sdp_discov(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to start DID discovery. If the request was successfully
 *           sent, multiple messages will be received in GAP legacy callback function
 *           registed by \ref legacy_register_cb. The first one is \ref SDP_DISCOV_RSP
 *           which indicates whether DID discovery was started successfully. If DID
 *           discovery was started successfully, one \ref DID_ATTR_INFO may be received.
 *           After DID discovery procedure is done, \ref SDP_DISCOV_CMPL will be received.
 *           <b>DID discovery procedure can only be started after receiving \ref PROTO_REG_CMPL
 *           message in GAP legacy callback. Only one SDP/DID discovery can be execute
 *           at the same time.</b>
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 *
 * \return    The status of sending start DID discovery request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

               legacy_start_did_discov(bd_addr);
           }
           break;

       case SDP_DISCOV_RSP:
           {
               T_GAP_SDP_DISCOV_RSP *p_rsp = (T_GAP_SDP_DISCOV_RSP *)buf;
               APP_PRINT_TRACE1("SDP discovery response: status 0x%04x", p_rsp->cause);
           }
           break;

       case DID_ATTR_INFO:
           {
               T_GAP_DID_ATTR_INFO *p_info = (T_GAP_DID_ATTR_INFO *)buf;
               APP_PRINT_TRACE2("DID information: remote addr %s, specific id 0x%04x",
                                TRACE_BDADDR(p_info->bd_addr), p_info->specific_id);
           }
           break;

       case SDP_DISCOV_CMPL:
           {
               T_GAP_SDP_DISCOV_CMPL_INFO *p_info = (T_GAP_SDP_DISCOV_CMPL_INFO *)buf;
               APP_PRINT_TRACE1("SDP discovery complete: status 0x%04x", p_info->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_start_did_discov(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to set legacy name of local device. This function is used to change
 *           local name after BT stack was started. \ref legacy_set_gap_param
 *           can be used to set local name before BT stack was started. If
 *           the request was successfully sent, \ref SET_DEV_NAME_RSP will be received
 *           in GAP legacy callback function registed by \ref legacy_register_cb
 *           to indicate the result.
 *
 * \param[in]  p_name     Address of name to set.
 * \param[in]  len        Length of name to set. If length is larger than \ref GAP_DEVICE_NAME_LEN,
 *                        the rest of the name will be discarded.
 *
 * \return    The status of sending set local legacy name request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   #define NEW_NAME       "new_name"
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_set_dev_name(NEW_NAME, sizeof(NEW_NAME));
           }
           break;

       case SET_DEV_NAME_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Set local name response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_dev_name(uint8_t *p_name, uint8_t len);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to set class of device of local device. This function is used to change
 *           class of device after BT stack was started. \ref legacy_set_gap_param can be used to
 *           set local class of device before BT stack was started. If the request was successfully
 *           sent, \ref SET_DEV_COD_RSP will be received in GAP legacy callback function registed
 *           by \ref legacy_register_cb to indicate the result.
 *
 * \param[in]  cod     Class of device to set.
 *
 * \return    The status of sending set class of device request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_set_dev_cod(0x340004);
           }
           break;

       case SET_DEV_COD_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Set class of device response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_dev_cod(uint32_t cod);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to set local radio mode. This function is used to change
 *           local radio mode after BT stack was started. \ref legacy_set_gap_param
 *           can be used to set local radio mode before BT stack was started. If
 *           the request was successfully sent, \ref SET_RADIO_MODE_RSP will be received
 *           in GAP legacy callback function registed by \ref legacy_register_cb
 *           to indicate the result.
 *
 * \param[in]  radio_mode               Radio mode to set. Valid values are defined in \ref T_GAP_RADIO_MODE.
 * \param[in]  limited_discov           Enter limited discoverable mode or not.
 * \arg \c     true             Enter limited discoverable mode.
 * \arg \c     false            Enter general discoverable mode.
 * \param[in]  limited_discov_time      Limited discoverable mode timeout value in seconds. Only valid when limited discoverable mode is enabled.
 *
 * \return    The status of sending set local radio mode request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_set_radio_mode(GAP_RADIO_MODE_VISIABLE_CONNECTABLE, false, 0);
           }
           break;

       case SET_RADIO_MODE_RSP:
           {
               T_GAP_RADIO_MODE_SET_RSP *p_rsp = (T_GAP_RADIO_MODE_SET_RSP *)buf;
               APP_PRINT_TRACE1("Set radio mode response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_radio_mode(uint8_t radio_mode, bool limited_discov,
								  uint8_t limited_discov_time);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to get remote device's name. If the request was successfully
 *           sent, \ref REMOTE_NAME_INFO will be received in GAP legacy callback
 *           function registed by \ref legacy_register_cb to indicate the result
 *           of the procedure. If success, remote name will also be contained in
 *           the message data. <b>Get remote name procedure can only be started
 *           after receiving \ref PROTO_REG_CMPL message in GAP legacy callback.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 *
 * \return    The status of sending get remote name request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               legacy_get_remote_name(bd_addr);
           }
           break;

       case REMOTE_NAME_INFO:
           {
               T_GAP_REMOTE_NAME_INFO *p_rsp = (T_GAP_REMOTE_NAME_INFO *)p_buf;

               if (!p_rsp->cause)
               {
                   APP_PRINT_TRACE1("Get remote name response: name is %s",
                                    TRACE_STRING(p_rsp->name));
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_get_remote_name(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure ACL link with remote device into sniff
 *           mode. If the request was successfully sent, \ref ENTER_SNIFF_MODE_RSP
 *           will be received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result of the procedure. If the
 *           link enters sniff mode successfully, \ref ACL_STATUS_INFO message will
 *           also be received with ACL status as \ref GAP_ACL_CONN_SNIFF, the
 *           actual sniff interval will also be contained in the message data.
 *           <b>To configure an ACL link into sniff mode, the link must be connected
 *           and ready to be configured, which can be sure by receiving \ref ACL_STATUS_INFO
 *           message with ACL status as \ref GAP_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  min_interval        Min sniff interval, only even values between 0x0002 and 0xFFFE are valid.
 * \param[in]  max_interval        Max sniff interval, only even values between 0x0002 and 0xFFFE are valid, should be larger than min_interval.
 * \param[in]  sniff_attempt       Number of baseband receive slots for sniff attempt.
 * \param[in]  sniff_timeout       Number of baseband receive slots for sniff timeout.
 * \param[in]  max_latency         Max allowed sniff subrate of the remote device. Set to 0 if do not want to configure sniff subrate.
 * \param[in]  min_remote_timeout  Min base sniff subrate timeout the remote device may use.
 * \param[in]  min_local_timeout   Min base sniff subrate timeout the local device may use.
 *
 * \return    The status of sending enter sniff mode request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_READY:
                   legacy_enter_sniff_mode(p_info->bd_addr, 192, 800, 4, 2);
                   break;

               case GAP_ACL_CONN_SNIFF:
                   APP_PRINT_TRACE1("ACL link into sniff mode: interval 0x%04x", p_info->p.sniff.interval);
                   break;

               default:
                   break;
               }
           }
           break;

       case ENTER_SNIFF_MODE_RSP:
           {
               T_GAP_ACL_ENTER_SNIFF_RSP *p_rsp = (T_GAP_ACL_ENTER_SNIFF_RSP *)p_buf;
               APP_PRINT_TRACE1("Enter sniff mode response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_enter_sniff_mode(uint8_t *bd_addr, uint16_t min_interval,
									uint16_t max_interval, uint16_t sniff_attempt, uint16_t sniff_timeout);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure ACL link with remote device to exit sniff
 *           mode. If the request was successfully sent, \ref EXIT_SNIFF_MODE_RSP
 *           will be received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result of the procedure. If the
 *           link exits sniff mode successfully, \ref ACL_STATUS_INFO message will
 *           also be received with ACL status as \ref GAP_ACL_CONN_ACTIVE.
 *           <b>To configure an ACL link to exit sniff mode, the link must be in
 *           sniff mode, which can be sure by receiving \ref ACL_STATUS_INFO
 *           message with ACL status as \ref GAP_ACL_CONN_SNIFF.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 *
 * \return    The status of sending exit sniff mode request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_SNIFF:
                   legacy_exit_sniff_mode(p_info->bd_addr);
                   break;

               case GAP_ACL_CONN_ACTIVE:
                   APP_PRINT_TRACE0("ACL link into active mode");
                   break;

               default:
                   break;
               }
           }
           break;

       case EXIT_SNIFF_MODE_RSP:
           {
               T_GAP_ACL_EXIT_SNIFF_RSP *p_rsp = (T_GAP_ACL_EXIT_SNIFF_RSP *)p_buf;
               APP_PRINT_TRACE1("Exit sniff mode response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_exit_sniff_mode(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure default link policy which Bluetooth stack will
 *           automatically set after ACL link is established. \ref SET_DEFAULT_LINK_POLICY_RSP
 *           will be received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result. <b>To change the default
 *           link policy before Bluetooth stack is started, \ref legacy_set_gap_param can be
 *           used.</b> \ref legacy_cfg_acl_link_policy can be used to change link
 *           policy after that for a specific link.
 *
 * \param[in]  link_policy         Link policy to set. Valid values are combinations of \ref GAP_LINK_POLICY.
 *
 * \return    The status of sending configure default link policy request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_cfg_default_link_policy(GAP_LINK_POLICY_ROLE_SWITCH);
           }
           break;

       case SET_DEFAULT_LINK_POLICY_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Set default link policy response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_default_link_policy(uint16_t link_policy);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure page scan parameters after Bluetooth stack is started.
 *           \ref SET_PAGE_SCAN_PARAM_RSP will be received in GAP legacy callback function
 *           registed by \ref legacy_register_cb to indicate the result. <b>To change these
 *           parameters before Bluetooth stack is started, \ref legacy_set_gap_param
 *           can be used.</b>
 *
 * \param[in]  type         Page scan type value to set. The valid value is \ref T_GAP_PAGE_SCAN_TYPE.
 * \param[in]  interval     Page scan interval value to set.
 * \param[in]  window       Page scan window value to set.
 *
 * \return    The status of sending configure page scan parameters request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_cfg_page_scan_param(GAP_PAGE_SCAN_TYPE_STANDARD, 0x800, 0x12);
           }
           break;

       case SET_PAGE_SCAN_PARAM_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Set page scan parameters response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_page_scan_param(uint8_t type, uint16_t interval, uint16_t window);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure inquiry scan parameters after Bluetooth stack is started.
 *           \ref SET_INQUIRY_SCAN_PARAM_RSP will be received in GAP legacy callback function
 *           registed by \ref legacy_register_cb to indicate the result. <b>To change these
 *           parameters before Bluetooth stack is started, \ref legacy_set_gap_param
 *           can be used.</b>
 *
 * \param[in]  type         Inquiry scan type value to set. The valid value is \ref T_GAP_INQUIRY_SCAN_TYPE.
 * \param[in]  interval     Inquiry scan interval value to set.
 * \param[in]  window       Inquiry scan window value to set.
 *
 * \return    The status of sending configure inquiry scan parameters request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_cfg_inquiry_scan_param(GAP_INQUIRY_SCAN_TYPE_STANDARD, 0x800, 0x12);
           }
           break;

       case SET_INQUIRY_SCAN_PARAM_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Set inquiry scan parameters response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_inquiry_scan_param(uint8_t type, uint16_t interval, uint16_t window);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure link policy for an ACL link. If the
 *           request was successfully sent, \ref LINK_POLICY_RSP will be received
 *           in GAP legacy callback function registed by \ref legacy_register_cb
 *           to indicate the result of the procedure. BT stack will automatically
 *           set link policy after ACL link is connected. <b>To change the default
 *           link policy that BT stack will set, \ref legacy_set_gap_param can be
 *           used.</b> \ref legacy_cfg_acl_link_policy can be used to change link
 *           policy after that. <b>In order to avoid collision, APP should set
 *           link policy after receiving \ref ACL_STATUS_INFO message with ACL
 *           status as \ref GAP_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  link_policy         Link policy to set. Valid values are combinations of \ref GAP_LINK_POLICY.
 *
 * \return    The status of sending configure link policy request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_READY:
                   legacy_cfg_acl_link_policy(p_info->bd_addr, GAP_LINK_POLICY_ROLE_SWITCH);
                   break;

               default:
                   break;
               }
           }
           break;

       case LINK_POLICY_RSP:
           {
               T_GAP_ACL_CFG_LINK_POLICY_RSP *p_rsp = (T_GAP_ACL_CFG_LINK_POLICY_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure link policy response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_acl_link_policy(uint8_t *bd_addr, uint16_t link_policy);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure link supervision timeout value for an
 *           ACL link. If the request was successfully sent, \ref LINK_SUPV_TOUT_RSP
 *           will be received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result of the procedure. BT stack
 *           will automatically set link supervision timeout value after ACL link
 *           is connected if local device is master. <b>To change the default
 *           link supervision timeout that BT stack will set, \ref legacy_set_gap_param
 *           can be used.</b> \ref legacy_cfg_acl_link_supv_tout can be used to
 *           change link supervision timeout value after that. <b>In order to
 *           avoid collision, APP should set link supervision timeout value after
 *           receiving \ref ACL_STATUS_INFO message with ACL status as \ref
 *           GAP_ACL_CONN_READY.</b> The function only works when local device
 *           is master of the ACL link.
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  tout                Supervision timeout value to set.
 *
 * \return    The status of sending configure link supervision timeout request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_READY:
                   legacy_cfg_acl_link_supv_tout(p_info->bd_addr, 0x7D00);
                   break;

               default:
                   break;
               }
           }
           break;

       case LINK_SUPV_TOUT_RSP:
           {
               T_GAP_ACL_CFG_LINK_SUPV_TOUT_RSP *p_rsp = (T_GAP_ACL_CFG_LINK_SUPV_TOUT_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure link supervision timeout response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_acl_link_supv_tout(uint8_t *bd_addr, uint16_t tout);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to switch link role for an ACL link. If the request
 *           was successfully sent, \ref ROLE_SWITCH_RSP will be received in GAP
 *           legacy callback function registed by \ref legacy_register_cb to
 *           indicate the result of the procedure. If switch role procedure was
 *           successful, \ref ACL_STATUS_INFO message will also be received with
 *           ACL status as \ref GAP_ACL_ROLE_MASTER or \ref GAP_ACL_ROLE_SLAVE
 *           depending on the switch request. If BR/EDR secure connections is
 *           supported, role change during SMP exchange is forbidden. <b>Recommended
 *           timing to switch is after receiving \ref ACL_STATUS_INFO message
 *           with ACL status as \ref GAP_ACL_CONN_ENCRYPTED or ACL status as
 *           \ref GAP_ACL_CONN_BR_SC_CMPL if \ref GAP_ACL_CONN_BR_SC_START was
 *           received before.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  device_role         Device role to configure.
 * \arg \c     0                   Switch to master role.
 * \arg \c     1                   Switch to slave role.
 *
 * \return    The status of sending switch link role request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t local_role = 0;

   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_ENCRYPTED:
                   if (local_role == 1)
                   {
                       legacy_cfg_acl_link_role(p_info->bd_addr, GAP_BR_LINK_ROLE_MASTER);
                   }
                   break;

               case GAP_ACL_ROLE_MASTER:
                   local_role = 0;
                   break;

               case GAP_ACL_ROLE_SLAVE:
                   local_role = 1;
                   break;

               default:
                   break;
               }
           }
           break;

       case ROLE_SWITCH_RSP:
           {
               T_GAP_ACL_CFG_LINK_ROLE_RSP *p_rsp = (T_GAP_ACL_CFG_LINK_ROLE_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure link role response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_acl_link_role(uint8_t *bd_addr, T_GAP_BR_LINK_ROLE device_role);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to change packet type for an ACL link. If the request
 *           was successfully sent, \ref PKT_TYPE_CHANGE_RSP will be received in
 *           GAP legacy callback function registed by \ref legacy_register_cb to
 *           indicate the result of the procedure. BT stack will automatically
 *           set link packet type after ACL link is connected. <b>In order to
 *           avoid collision, APP should change packet type after receiving \ref
 *           ACL_STATUS_INFO message with ACL status as \ref GAP_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  pkt_type            Packet type to use. Valid values are combinations of \ref GAP_ACL_PACKET_TYPE.
 *
 * \return    The status of sending change packet type request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_READY:
                   //Use 2M only
                   legacy_cfg_acl_pkt_type(p_info->bd_addr,
                                           GAP_PKT_TYPE_DM1 | GAP_PKT_TYPE_DH1 | \
                                           GAP_PKT_TYPE_DM3 | GAP_PKT_TYPE_DH3 | \
                                           GAP_PKT_TYPE_DM5 | GAP_PKT_TYPE_DH5 | \
                                           GAP_PKT_TYPE_NO_3DH1 | GAP_PKT_TYPE_NO_3DH3 |\
                                           GAP_PKT_TYPE_NO_3DH5);
                   break;

               default:
                   break;
               }
           }
           break;

       case PKT_TYPE_CHANGE_RSP:
           {
               T_GAP_ACL_CFG_PKT_TYPE_RSP *p_rsp = (T_GAP_ACL_CFG_PKT_TYPE_RSP *)p_buf;
               APP_PRINT_TRACE1("Change packet type response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_acl_pkt_type(uint8_t *bd_addr, uint16_t pkt_type);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure link flush timeout value for an ACL link.
 *           If the request was successfully sent, \ref LINK_FLUSH_TOUT_RSP
 *           will be received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result of the procedure.
 *           <b>APP should set link flush timeout value after receiving \ref ACL_STATUS_INFO
 *           message with ACL status as \ref GAP_ACL_CONN_READY.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  flush_tout          Flush timeout value to set. 0xFFFF for no flush.
 *
 * \return    The status of sending configure link flush timeout request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_READY:
                   legacy_cfg_acl_link_flush_tout(p_info->bd_addr, 0x00FF);
                   break;

               default:
                   break;
               }
           }
           break;

       case LINK_FLUSH_TOUT_RSP:
           {
               T_GAP_ACL_CFG_FLUSH_TOUT_RSP *p_rsp = (T_GAP_ACL_CFG_FLUSH_TOUT_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure link flush timeout response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_acl_link_flush_tout(uint8_t *bd_addr, uint16_t flush_tout);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure prefered link role to use when accepting
 *           connection request from remote device. If the request was successfully
 *           sent, \ref SET_ACCEPT_ROLE_RSP will be received in GAP legacy callback
 *           function registed by \ref legacy_register_cb.
 *
 * \param[in]  role              Prefered role to use when accepting connection request.
 * \arg \c     0             Become the Master for this connection.
 * \arg \c     1             Remain the Slave for this connection. This is the default value.
 *
 * \return    The status of sending configure prefered link role request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SET_ACCEPT_ROLE_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure accept role response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       legacy_cfg_accept_role(0);

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_accept_role(uint8_t role);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure simple pairing debug mode. If the request was successfully
 *           sent, \ref SET_SSP_DEBUG_MODE_RSP will be received in GAP legacy callback
 *           function registed by \ref legacy_register_cb.
 *
 * \param[in]  enable              Enable/disable simple pairing debug mode.
 *
 * \return    The status of sending configure simple pairing debug mode.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SET_SSP_DEBUG_MODE_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure ssp debug mode response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       legacy_cfg_ssp_debug_mode(true);

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_ssp_debug_mode(bool enable);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure timeout value that will be used to disconnect acl link
 *           after last l2cap channel is disconnected. \ref SET_ACL_DISCONN_TOUT_RSP will be
 *           received in GAP legacy callback function registed by \ref legacy_register_cb.
 *
 * \param[in]  tout         Timeout value in seconds to disconnect acl link in normal connected state. Default value is 4s.
 * \param[in]  pair_tout    Timeout value in seconds to disconnect acl link in pairing state. Default value is 30s.
 *
 * \return    The status of sending configure acl link disconnect timeout value.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SET_ACL_DISCONN_TOUT_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)p_buf;
               APP_PRINT_TRACE1("Configure acl disconnect timeout value response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       legacy_cfg_acl_disconn_tout(4, 30);

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_cfg_acl_disconn_tout(uint8_t tout, uint8_t pair_tout);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to disconnect an ACL link. If the request was
 *           successfully sent, \ref ACL_DISCONN_RSP will be received in GAP
 *           legacy callback function registed by \ref legacy_register_cb to
 *           indicate the result of the procedure. If the ACL link was successfully
 *           disconnected, \ref ACL_STATUS_INFO message will also be received with
 *           ACL status as \ref GAP_ACL_CONN_DISCONN. <b>To disconnect an ACL
 *           link, it must be connected, which can be sure by receiving \ref
 *           ACL_STATUS_INFO message with ACL status as \ref GAP_ACL_CONN_SUCCESS.</b>
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 *
 * \return    The status of sending disconnect ACL link request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_SUCCESS:
                   legacy_send_acl_disconn_req(p_info->bd_addr);
                   break;

               case GAP_ACL_CONN_DISCONN:
                   APP_PRINT_TRACE0("ACL link disconnected");
                   break;

               default:
                   break;
               }
           }
           break;

       case ACL_DISCONN_RSP:
           {
               T_GAP_ACL_DISCONN_RSP *p_rsp = (T_GAP_ACL_DISCONN_RSP *)p_buf;
               APP_PRINT_TRACE2("ACL link disconnect response: status 0x%04x, reason 0x%04x",
                                p_rsp->cause, p_rsp->reason);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_acl_disconn_req(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a confirm to accept acl connection request from remote device.
 *           Should be used when receiving \ref ACL_CONN_IND message in GAP legacy
 *           callback function registed by \ref legacy_register_cb. The message can only be
 *           received when \ref GAP_PARAM_AUTO_ACCEPT_ACL is set to false.
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  role                Desired role for the connection.
 *
 * \return    The status of sending confirm.
 * \retval    GAP_CAUSE_SUCCESS            Confirm was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Confirm was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_CONN_IND:
           {
               T_GAP_ACL_CONN_IND *p_ind = (T_GAP_ACL_CONN_IND *)p_buf;
               legacy_accept_acl_conn(p_ind->bd_addr, GAP_BR_LINK_ROLE_SLAVE);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_accept_acl_conn(uint8_t *bd_addr, T_GAP_BR_LINK_ROLE role);

/**
 * gap_legacy.h
 *
 * \brief    Send a confirm to reject acl connection request from remote device.
 *           Should be used when receiving \ref ACL_CONN_IND message in GAP legacy
 *           callback function registed by \ref legacy_register_cb. The message can only be
 *           received when \ref GAP_PARAM_AUTO_ACCEPT_ACL is set to false.
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  reason              Reason to reject the connection request.
 *
 * \return    The status of sending confirm.
 * \retval    GAP_CAUSE_SUCCESS            Confirm was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Confirm was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_CONN_IND:
           {
               T_GAP_ACL_CONN_IND *p_ind = (T_GAP_ACL_CONN_IND *)p_buf;
               legacy_reject_acl_conn(p_ind->bd_addr, GAP_ACL_REJECT_LIMITED_RESOURCE);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_reject_acl_conn(uint8_t *bd_addr, T_GAP_ACL_REJECT_CONN_REASON reason);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to specify Quality of Service parameters for an ACL
 *           link. If the request was successfully sent, \ref SETUP_QOS_RSP will
 *           be received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result of the procedure. <b> To
 *           set the QOS of an ACL link, the link must be connected, which can
 *           be sure by receive \ref ACL_STATUS_INFO message with ACL status as
 *           \ref GAP_ACL_CONN_SUCCESS.</b>
 *
 * \param[in]  handle              Connection handle of the link, which can be get by \ref ACL_STATUS_INFO message with ACL status as \ref GAP_ACL_CONN_SUCCESS.
 * \param[in]  flags               Reserved for feature.
 * \param[in]  type                Service type.
 * \arg \c     0               No traffic.
 * \arg \c     1               Best effort.
 * \arg \c     2               Guaranteed.
 * \param[in]  token_rate          Token rate in octets per second.
 * \param[in]  peak_bandwdith      Peak bandwidth in octets per second.
 * \param[in]  latency             Latency in microseconds.
 * \param[in]  delay_variation     Delay variation in microseconds.
 *
 * \return    The status of sending specify QOS request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_SUCCESS:
                   legacy_send_setup_qos_req(p_info->p.conn_success.handle, 0,
                                             QOS_GUARANTEED, 11250, 0, 10000 * 2, 0);
                   break;

               default:
                   break;
               }
           }
           break;

       case SETUP_QOS_RSP:
           {
               T_GAP_SETUP_QOS_RSP *p_rsp = (T_GAP_SETUP_QOS_RSP *)p_buf;
               APP_PRINT_TRACE1("Setup QOS response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_setup_qos_req(uint16_t handle, uint8_t flags, uint8_t type,
									  uint32_t token_rate, uint32_t peak_bandwdith, uint32_t latency,
									  uint32_t delay_variation);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to set page with scan simultaneous. If the
 *           request was successfully sent, \ref SET_PAGE_WITH_SCAN_RSP will be
 *           received in GAP legacy callback function registed by \ref
 *           legacy_register_cb to indicate the result of the procedure.
 *
 * \param[in]  enable           Whether enable page with scan simultaneous or not.
 * \arg \c     true             Enable page with scan simultaneous.
 * \arg \c     false            Disable page with scan simultaneous.
 *
 * \return    The status of sending set page with scan simultaneous request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SET_PAGE_WITH_SCAN_RSP:
           {
               T_GAP_SET_PAGE_WITH_SCAN_RSP *p_rsp = (T_GAP_SET_PAGE_WITH_SCAN_RSP *)p_buf;
               APP_PRINT_TRACE1("Set page with scan response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       legacy_set_page_with_scan(true);

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_page_with_scan(bool enable);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to create a SCO/eSCO connection. If the request was
 *           successfully sent, \ref SCO_CONN_RSP will be received in GAP legacy
 *           callback function registed by \ref legacy_register_cb to indicate
 *           whether the procedure was started successfully. If the procedure was
 *           started, \ref SCO_CONN_CMPL will be received later to tell the
 *           connection establish result. <b>The procedure can only be started
 *           after receiving \ref PROTO_REG_CMPL message in GAP legacy callback.</b>
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  tx_bandwidth         Transmit bandwidth in octets per second.
 * \param[in]  rx_bandwidth         Receive bandwidth in octets per second.
 * \param[in]  max_latency          A value in milliseconds representing upper limit of the sum of the synchronous interval. Valid between 0x0004 to 0xFFFE.
 * \param[in]  voice_setting        Voice setting bitmap.
 * \param[in]  retrans_effort       Retransmit effort.
 * \arg \c     0x00             No retransmissions.
 * \arg \c     0x01             At least one retransmission, optimize for power consumption.
 * \arg \c     0x02             At least one retransmission, optimize for link quality.
 * \arg \c     0xFF             Don't care.
 * \param[in]  packet_type          Transmit packet type.Valid values are combinations of \ref GAP_SCO_PACKET_TYPE.
 *
 * \return    The status of sending SCO/eSCO connect request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               legacy_send_sco_conn_req(bd_addr, 8000, 8000, 7, 0x0360, 0, GAP_PKT_TYPE_EV3 | GAP_PKT_TYPE_EV4 | GAP_PKT_TYPE_EV5);
           }
           break;

       case SCO_CONN_RSP:
           {
               T_GAP_SCO_CONN_RSP *p_rsp = (T_GAP_SCO_CONN_RSP *)p_buf;
               APP_PRINT_TRACE1("SCO connection response: status 0x%04x", p_rsp->cause);
           }
           break;

       case SCO_CONN_CMPL:
           {
               T_GAP_SCO_CONN_CMPL_INFO *p_info = (T_GAP_SCO_CONN_CMPL_INFO *)p_buf;

               if (!p_info->cause)
               {
                   APP_PRINT_TRACE1("SCO connection success: handle 0x%04x", p_info->handle);
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_sco_conn_req(uint8_t *bd_addr, uint32_t tx_bandwidth,
									 uint32_t rx_bandwidth, uint16_t max_latency,
									 uint16_t voice_setting, uint8_t retrans_effort,
									 uint16_t packet_type);

/**
 * gap_legacy.h
 *
 * \brief    Send a confirmation for SCO/eSCO connect indication. If the confirm
 *           was successfully sent and confirm cause is \ref GAP_CFM_CAUSE_ACCEPT,
 *           \ref SCO_CONN_CMPL will be received in GAP legacy callback function
 *           registed by \ref legacy_register_cb to tell the connection establish
 *           result. <b>The function can only be used when receive \ref
 *           SCO_CONN_IND message in GAP legacy callback.</b>
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  tx_bandwidth         Transmit bandwidth in octets per second.
 * \param[in]  rx_bandwidth         Receive bandwidth in octets per second.
 * \param[in]  max_latency          A value in milliseconds representing upper limit of the sum of the synchronous interval. Valid between 0x0004 to 0xFFFE.
 * \param[in]  voice_setting        Voice setting bitmap.
 * \param[in]  retrans_effort       Retransmit effort.
 * \arg \c     0x00             No retransmissions.
 * \arg \c     0x01             At least one retransmission, optimize for power consumption.
 * \arg \c     0x02             At least one retransmission, optimize for link quality.
 * \arg \c     0xFF             Don't care.
 * \param[in]  packet_type          Transmit packet type.Valid values are combinations of \ref GAP_SCO_PACKET_TYPE.
 * \param[in]  cause                Confirm cause for SCO connection request. \ref GAP_CFM_CAUSE_ACCEPT to accept the connection, otherwise reject.
 *
 * \return    The status of sending SCO/eSCO connect confirmation.
 * \retval    GAP_CAUSE_SUCCESS            Confirmation was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Confirmation was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SCO_CONN_IND:
           {
               T_GAP_SCO_CONN_IND *p_ind = (T_GAP_SCO_CONN_IND *)p_buf;

               legacy_send_sco_conn_cfm(p_ind->bd_addr, 8000, 8000, 13, 0x0363, 1,
                                        GAP_PKT_TYPE_EV3 | GAP_PKT_TYPE_EV4 | GAP_PKT_TYPE_EV5,
                                        GAP_CFM_CAUSE_ACCEPT);
           }
           break;

       case SCO_CONN_CMPL:
           {
               T_GAP_SCO_CONN_CMPL_INFO *p_info = (T_GAP_SCO_CONN_CMPL_INFO *)p_buf;

               if (!p_info->cause)
               {
                   APP_PRINT_TRACE1("SCO connection success: handle 0x%04x", p_info->handle);
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_sco_conn_cfm(uint8_t *bd_addr, uint32_t tx_bandwidth,
									 uint32_t rx_bandwidth, uint16_t max_latency,
									 uint16_t voice_setting, uint8_t retrans_effort,
									 uint16_t packet_type, T_GAP_CFM_CAUSE cause);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to disconnect a SCO/eSCO connection. If the request was
 *           successfully sent, \ref SCO_DISCONN_RSP will be received in GAP legacy
 *           callback function registed by \ref legacy_register_cb to indicate
 *           the disconnect result.
 *
 * \param[in]  bd_addr               Bluetooth address of remote device.
 *
 * \return    The status of sending SCO/eSCO disconnect request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SCO_CONN_CMPL:
           {
               T_GAP_SCO_CONN_CMPL_INFO *p_info = (T_GAP_SCO_CONN_CMPL_INFO *)p_buf;

               if (!p_info->cause)
               {
                   legacy_send_sco_disconn_req(p_info->bd_addr);
               }
           }
           break;

       case SCO_DISCONN_RSP:
           {
               T_GAP_SCO_DISCONN_RSP *p_rsp = (T_GAP_SCO_DISCONN_RSP *)p_buf;

               APP_PRINT_TRACE2("SCO disconnect response: status 0x%04x, reason 0x%04x",
                                p_rsp->cause, p_rsp->reason);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_sco_disconn_req(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a confirmation for SCO/eSCO disconnect indication. <b>The
 *           function can only be used when receive \ref SCO_DISCONN_IND message
 *           in GAP legacy callback.</b>
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 *
 * \return    The status of sending SCO/eSCO disconnect confirmation.
 * \retval    GAP_CAUSE_SUCCESS            Confirmation was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Confirmation was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SCO_DISCONN_IND:
           {
               T_GAP_SCO_DISCONN_IND *p_ind = (T_GAP_SCO_DISCONN_IND *)p_buf;
               legacy_send_sco_disconn_cfm(p_ind->bd_addr);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_sco_disconn_cfm(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to change parameters of a SCO/eSCO connection. If the request was
 *           successfully sent, \ref SCO_PARAM_CHANGE_RSP will be received in GAP legacy
 *           callback function registed by \ref legacy_register_cb to indicate
 *           whether the procedure was started successfully. If the procedure was
 *           started, \ref SCO_PARAM_CHANGE_INFO will be received later to tell the result, if
 *           success, the final parameters will also be notified.
 *
 * \param[in]  bd_addr              Bluetooth address of remote device.
 * \param[in]  tx_bandwidth         Transmit bandwidth in octets per second.
 * \param[in]  rx_bandwidth         Receive bandwidth in octets per second.
 * \param[in]  max_latency          A value in milliseconds representing upper limit of the sum of the synchronous interval. Valid between 0x0004 to 0xFFFE.
 * \param[in]  voice_setting        Voice setting bitmap.
 * \param[in]  retrans_effort       Retransmit effort.
 * \arg \c     0x00             No retransmissions.
 * \arg \c     0x01             At least one retransmission, optimize for power consumption.
 * \arg \c     0x02             At least one retransmission, optimize for link quality.
 * \arg \c     0xFF             Don't care.
 * \param[in]  packet_type          Transmit packet type.Valid values are combinations of \ref GAP_SCO_PACKET_TYPE.
 *
 * \return    The status of sending SCO/eSCO parameters change request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               legacy_send_sco_conn_req(bd_addr, 8000, 8000, 7, 0x0360, 0, GAP_PKT_TYPE_EV3 | GAP_PKT_TYPE_EV4 | GAP_PKT_TYPE_EV5);
           }
           break;

       case SCO_CONN_RSP:
           {
               T_GAP_SCO_CONN_RSP *p_rsp = (T_GAP_SCO_CONN_RSP *)p_buf;
               APP_PRINT_TRACE1("SCO connection response: status 0x%04x", p_rsp->cause);
           }
           break;

       case SCO_CONN_CMPL:
           {
               T_GAP_SCO_CONN_CMPL_INFO *p_info = (T_GAP_SCO_CONN_CMPL_INFO *)p_buf;

               if (!p_info->cause)
               {
                   APP_PRINT_TRACE1("SCO connection success: handle 0x%04x", p_info->handle);
                   legacy_send_sco_param_change_req(p_info->bd_addr, 8000, 8000, 7, 0x0360, 0, GAP_PKT_TYPE_EV3 | GAP_PKT_TYPE_EV4 | GAP_PKT_TYPE_EV5);
               }
           }
           break;

       case SCO_PARAM_CHANGE_RSP:
           {
               T_GAP_SCO_PARAM_CHANGE_RSP *p_rsp = (T_GAP_SCO_PARAM_CHANGE_RSP *)p_buf;
               APP_PRINT_TRACE1("SCO parameters change response: status 0x%04x", p_rsp->cause);
           }
           break;

       case SCO_PARAM_CHANGE_INFO:
           {
               T_GAP_SCO_PARAM_CHANGE_INFO *p_info = (T_GAP_SCO_PARAM_CHANGE_INFO *)p_buf;

               if (!p_info->cause)
               {
                   APP_PRINT_TRACE1("SCO parameters change success: handle 0x%04x", p_info->handle);
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_sco_param_change_req(uint8_t *bd_addr, uint32_t tx_bandwidth,
		uint32_t rx_bandwidth, uint16_t max_latency, uint16_t voice_setting,
		uint8_t retrans_effort, uint16_t packet_type);

/**
 * gap_legacy.h
 *
 * \brief    Get buffer from BT stack to put in SCO data which will be sent to remote device.
 *
 * \param[in]  len              Length of SCO data to put into the buffer.
 *
 * \return    The address of the allocated buffer. If the address in NULL, the
 *            buffer was get failed.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SCO_CONN_CMPL:
           {
               T_GAP_SCO_CONN_CMPL_INFO *p_info = (T_GAP_SCO_CONN_CMPL_INFO *)p_buf;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = legacy_get_sco_buffer(60);

                   if (p_buffer)
                   {
                       // ...
                       // Fill in the buffer with SCO data
                       // ...

                       legacy_send_sco_data(p_info->bd_addr, p_buffer, 60);
                   }
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
void *legacy_get_sco_buffer(uint8_t len);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to send SCO data to remote device.
 *
 * \param[in]  bd_addr             Bluetooth address of remote device.
 * \param[in]  p_buf               Address of SCO data buffer which is get by \ref legacy_get_sco_buffer.
 * \param[in]  length              Length of SCO data buffer. Must match the one in \ref legacy_get_sco_buffer.
 *
 * \return    The status of sending SCO data request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *p_buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SCO_CONN_CMPL:
           {
               T_GAP_SCO_CONN_CMPL_INFO *p_info = (T_GAP_SCO_CONN_CMPL_INFO *)p_buf;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = legacy_get_sco_buffer(60);

                   if (p_buffer)
                   {
                       // ...
                       // Fill in the buffer with SCO data
                       // ...

                       legacy_send_sco_data(p_info->bd_addr, p_buffer, 60);
                   }
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_send_sco_data(uint8_t *bd_addr, void *p_buf, uint8_t length);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure DID information in EIR data. If the request
 *           was successfully sent, \ref SET_DID_EIR_RSP will be received in the
 *           GAP legacy callback function registed by \ref legacy_register_cb
 *           to indicate the result.
 *
 * \param[in]  vendor_id           Vendor ID.
 * \param[in]  id_source           Vendor ID source.
 * \param[in]  product_id          Product ID.
 * \param[in]  product_version     Product version.
 *
 * \return    The status of sending configure DID information request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               legacy_set_did_eir(0x005D, 0x0001, 0x223B, 0x0100);
           }
           break;

       case SET_DID_EIR_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Set DID in EIR response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_did_eir(uint16_t vendor_id, uint16_t id_source, uint16_t product_id,
							   uint16_t product_version);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to configure extra information other than local name,
 *           service UUID and DID in EIR data. If the request was successfully sent,
 *           \ref SET_EXT_EIR_RSP will be received in the GAP legacy callback
 *           function registed by \ref legacy_register_cb to indicate the result.
 *
 * \param[in]  p_data        Address of extra EIR data.<b>The buffer should not be freed until another extra EIR data is used.</b>
 * \param[in]  length        The length of extra EIR data.
 *
 * \return    The status of sending configure extra EIR data request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t *p_ext = NULL;

   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case PROTO_REG_CMPL:
           {
               p_ext = os_mem_zalloc(1,30);
               if (p_ext != NULL)
               {
                   // ...
                   // Fill in extra EIR data.
                   // ...
                   legacy_set_ext_eir(p_ext, 30);
               }
           }
           break;

       case SET_EXT_EIR_RSP:
           {
               T_GAP_DEV_CFG_RSP *p_rsp = (T_GAP_DEV_CFG_RSP *)buf;
               APP_PRINT_TRACE1("Configure extra eir data result 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_set_ext_eir(uint8_t *p_data, uint8_t length);

/**
 * gap_legacy.h
 *
 * \brief    Send a request to get rssi value. If the request was successfully sent,
 *           \ref READ_RSSI_RSP will be received in the GAP legacy callback
 *           function registed by \ref legacy_register_cb.
 *
 * \param[in]  bd_addr        Bluetooth address of remote device.
 *
 * \return    The status of sending get rssi request.
 * \retval    GAP_CAUSE_SUCCESS            Request was sent successfully.
 * \retval    GAP_CAUSE_SEND_REQ_FAILED    Request was fail to send.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case ACL_STATUS_INFO:
           {
               T_GAP_ACL_STATUS_INFO *p_info = (T_GAP_ACL_STATUS_INFO *)p_buf;

               switch (p_info->status)
               {
               case GAP_ACL_CONN_READY:
                   legacy_read_rssi(p_info->bd_addr);
                   break;

               default:
                   break;
               }
           }
           break;

       case READ_RSSI_RSP:
           {
               T_GAP_READ_RSSI_RSP *p_rsp = (T_GAP_READ_RSSI_RSP *)p_buf;
               APP_PRINT_TRACE1("Get rssi response: status 0x%04x", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
T_GAP_CAUSE legacy_read_rssi(uint8_t *bd_addr);

/**
 * gap_legacy.h
 *
 * \brief    Find an attribute of the given value in a SDP record data.
 *
 * \param[in]  p_start        Start address of the SDP record data.
 * \param[in]  p_end          End address of the SDP record data.
 * \param[in]  attr_value     Attribute value to search for.
 *
 * \return    The address of the attribute.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SDP_ATTR_INFO:
           {
               T_GAP_SDP_ATTR_INFO *p_info = (T_GAP_SDP_ATTR_INFO *)p_buf;
               uint8_t *p_attr_start = p_info->p_attr;
               uint8_t *p_attr_end = p_attr_start + p_info->attr_len;
               uint8_t *p_attr;
               uint8_t *p_elem;
               uint16_t loop;
               T_GAP_UUID_TYPE srv_class_uuid_type;
               T_GAP_UUID_DATA srv_class_uuid_data;

               p_attr = legacy_find_sdp_attr(p_attr_start, p_attr_end, SDP_ATTR_SRV_CLASS_ID_LIST);
               loop = 1;
               while ((p_elem = legacy_access_sdp_elem(p_attr, p_attr_end, loop)) != NULL)
               {
                   if (legacy_get_sdp_uuid_value(p_elem, p_attr_end, &srv_class_uuid_type,
                                                 &srv_class_uuid_data))
                   {
                       break;
                   }
                   else
                   {
                       loop++;
                   }
               }

               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
uint8_t *legacy_find_sdp_attr(uint8_t *p_start, uint8_t *p_end, uint32_t attr_value);

/**
 * gap_legacy.h
 *
 * \brief    Get an attribute value in a SDP element.
 *
 * \param[in]  p_start        Start address of the SDP element.
 * \param[in]  p_end          End address of the SDP element.
 *
 * \return    The value of the attribute.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SDP_ATTR_INFO:
           {
               T_GAP_SDP_ATTR_INFO *p_info = (T_GAP_SDP_ATTR_INFO *)p_buf;
               uint8_t *p_attr_start = p_info->p_attr;
               uint8_t *p_attr_end = p_attr_start + p_info->attr_len;
               uint8_t *p_attr;
               uint16_t supported_feat;

               p_attr = legacy_find_sdp_attr(p_attr_start, p_attr_end, SDP_ATTR_SUPPORTED_FEATURES);
               if (p_attr)
               {
                   supported_feat = (uint16_t)legacy_get_sdp_value(p_attr, p_attr_end);
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
uint32_t legacy_get_sdp_value(uint8_t *p_start, uint8_t *p_end);

/**
 * gap_legacy.h
 *
 * \brief    Get an UUID data value in a SDP element.
 *
 * \param[in]  p_start        Start address of the SDP element.
 * \param[in]  p_end          End address of the SDP element.
 * \param[out]  p_type        The address to put in UUID data format.
 * \param[out]  p_data        The address to put in UUID data value.
 *
 * \return    The result of getting UUID value.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SDP_ATTR_INFO:
           {
               T_GAP_SDP_ATTR_INFO *p_info = (T_GAP_SDP_ATTR_INFO *)p_buf;
               uint8_t *p_attr_start = p_info->p_attr;
               uint8_t *p_attr_end = p_attr_start + p_info->attr_len;
               uint8_t *p_attr;
               uint8_t *p_elem;
               uint16_t loop;
               T_GAP_UUID_TYPE srv_class_uuid_type;
               T_GAP_UUID_DATA srv_class_uuid_data;

               p_attr = legacy_find_sdp_attr(p_attr_start, p_attr_end, SDP_ATTR_SRV_CLASS_ID_LIST);
               loop = 1;
               while ((p_elem = legacy_access_sdp_elem(p_attr, p_attr_end, loop)) != NULL)
               {
                   if (legacy_get_sdp_uuid_value(p_elem, p_attr_end, &srv_class_uuid_type,
                                                 &srv_class_uuid_data))
                   {
                       break;
                   }
                   else
                   {
                       loop++;
                   }
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
bool legacy_get_sdp_uuid_value(uint8_t *p_start, uint8_t *p_end, T_GAP_UUID_TYPE *p_type,
							   T_GAP_UUID_DATA *p_data);

/**
 * gap_legacy.h
 *
 * \brief    Get the address of SDP data element in a SDP element.
 *
 * \param[in]  p_start        Start address of the SDP element.
 * \param[in]  p_end          End address of the SDP element.
 * \param[in]  index          Data element index number.
 *
 * \return    The address of founded SDP data element.
 *
 * <b>Example usage</b>
   \code{.c}
   // GAP legacy callback function.
   void legacy_cb(void *buf, T_LEGACY_MSG legacy_msg)
   {
       switch (legacy_msg)
       {
       case SDP_ATTR_INFO:
           {
               T_GAP_SDP_ATTR_INFO *p_info = (T_GAP_SDP_ATTR_INFO *)p_buf;
               uint8_t *p_attr_start = p_info->p_attr;
               uint8_t *p_attr_end = p_attr_start + p_info->attr_len;
               uint8_t *p_attr;
               uint8_t *p_elem;
               uint8_t *p_attr_param;
               uint16_t loop;
               uint16_t profile_version;

               p_attr = legacy_find_sdp_attr(p_attr_start, p_attr_end, SDP_ATTR_PROFILE_DESC_LIST);
               if (p_attr)
               {
                   loop = 1;
                   while ((p_elem = legacy_access_sdp_elem(p_attr, p_attr_end, loop)) != NULL)
                   {
                       p_attr_param = legacy_access_sdp_elem(p_elem, p_attr_end, 2);

                       if (p_attr_param)
                       {
                           profile_version = (uint16_t)legacy_get_sdp_value(p_attr_param, p_attr_end);
                           break;
                       }
                       loop++;
                   }
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       // ...
       // Init GAP legacy module.
       // ...

       legacy_register_cb(legacy_cb);

       // ...
       // Create app task.
       // Start BT stack.
       // ...

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_BREDR
 */
uint8_t *legacy_access_sdp_elem(uint8_t *p_start, uint8_t *p_end, uint16_t index);

T_GAP_CAUSE legacy_set_lt_addr(uint8_t lt_addr);

T_GAP_CAUSE legacy_del_lt_addr(uint8_t lt_addr);

T_GAP_CAUSE legacy_set_sync_train_param(uint16_t min_interval, uint16_t max_interval,
										uint32_t sync_train_tout, uint8_t srv_data);

T_GAP_CAUSE legacy_start_sync_train(void);

T_GAP_CAUSE legacy_set_sync_train_rcv(uint8_t *bd_addr, uint16_t sync_scan_tout,
									  uint16_t sync_scan_window, uint16_t sync_scan_interval);

/** End of GAP_BREDR
* @}
*/

typedef enum {
	LEGACY_FIX_CHANN_REG_RSP,
	LEGACY_FIX_CHANN_DATA_IND
} T_LEGACY_FIX_CHANN_MSG;

typedef struct {
	uint16_t        cid;
	uint16_t        cause;
} T_LEGACY_FIX_CHANN_REG_RSP;

typedef struct {
	uint8_t     bd_addr[6];
	uint16_t    cid;
	uint8_t    *p_data;
	uint16_t    length;
} T_LEGACY_FIX_CHANN_DATA_IND;

typedef void (* P_LEGACY_FIX_CHANN_CB)(T_LEGACY_FIX_CHANN_MSG msg, void *p_buf);

void legacy_reg_fix_chann_cb(P_LEGACY_FIX_CHANN_CB p_func);

T_GAP_CAUSE legacy_reg_fix_chann(uint16_t cid);

T_GAP_CAUSE legacy_send_fix_chann_data(uint8_t *bd_addr, uint16_t cid, uint8_t *p_data,
									   uint16_t length);

T_GAP_CAUSE legacy_vendor_set_active_sco(uint16_t handle, uint8_t activate, uint8_t policy);

T_GAP_CAUSE legacy_vendor_cont_tx_begin(uint8_t *bd_addr, uint8_t pattern, uint8_t power_idx,
										uint8_t is_whiten, uint8_t whiten_coeff, uint8_t rf_chann,
										uint8_t pkt_type, uint8_t tx_gain_offset);

T_GAP_CAUSE legacy_vendor_cont_tx_stop(void);

T_GAP_CAUSE legacy_set_conn_encryption(uint16_t handle, bool enable);

T_GAP_CAUSE legacy_vendor_set_monitor(uint16_t handle, uint8_t type, bool enable, uint16_t period);
#endif

#ifdef __cplusplus
}
#endif    /*  __cplusplus */

#endif

#endif    /*  __GAP_LEGACY_H__*/
