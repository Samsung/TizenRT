/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BTM_H_
#define _BTM_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include <stdint.h>
#include <stdbool.h>

#include "gap_legacy.h"
#include "remote.h"
#include "bt_pbap.h"
#include "bt_avrcp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup BT_BTM BT Device and Link Manager
 *
 * \brief Manipulate BT device and links.
 */

/**
 * btm.h
 *
 * \brief Define BT device mode.
 *
 * \ingroup BT_BTM
 * @{
 */
typedef enum t_bt_device_mode
{
    BT_DEVICE_MODE_IDLE                     = 0x00, /**< Page scan and inquiry scan disabled */
    BT_DEVICE_MODE_DISCOVERABLE             = 0x01, /**< Inquiry scan enabled */
    BT_DEVICE_MODE_CONNECTABLE              = 0x02, /**< Page scan enabled */
    BT_DEVICE_MODE_DISCOVERABLE_CONNECTABLE = 0x03, /**< Page scan and inquiry scan enabled */
} T_BT_DEVICE_MODE;
/**
 * @}
 */

/**
 * btm.h
 *
 * \brief    BR/EDR link role.
 *
 * \ingroup  BT_BTM
 */
typedef enum t_bt_link_role
{
    BT_LINK_ROLE_MASTER,   /**< BR/EDR link role master. */
    BT_LINK_ROLE_SLAVE     /**< BR/EDR link role slave. */
} T_BT_LINK_ROLE;

/**
 * btm.h
 *
 * \brief    BT link packet type.
 *
 * \ingroup  BT_BTM
 */
typedef enum t_bt_acl_pkt_type
{
    BT_ACL_PKT_TYPE_1M,
    BT_ACL_PKT_TYPE_2M,
    BT_ACL_PKT_TYPE_3M,
} T_BT_ACL_PKT_TYPE;

/**
 * btm.h
 *
 * \brief    Reason to reject an acl connection from remote device.
 *
 * \ingroup  BT_BTM
 */
typedef enum t_bt_acl_reject_reason
{
    BT_ACL_REJECT_LIMITED_RESOURCE  = 0x0D,
    BT_ACL_REJECT_SECURITY_REASON   = 0x0E,
    BT_ACL_REJECT_UNACCEPTABLE_ADDR = 0x0F
} T_BT_ACL_REJECT_REASON;

/**
 * btm.h
 *
 * \brief    BT link key type.
 *
 * \ingroup  BT_BTM
 */
typedef enum t_bt_link_key_type
{
    BT_LINK_KEY_TYPE_COMBINATION         = 0x00,
    BT_LINK_KEY_TYPE_LOCAL_UNIT          = 0x01,
    BT_LINK_KEY_TYPE_REMOTE_UNIT         = 0x02,
    BT_LINK_KEY_TYPE_DEBUG_COMBINATION   = 0x03,
    BT_LINK_KEY_TYPE_UNAUTHEN            = 0x04,
    BT_LINK_KEY_TYPE_AUTHEN              = 0x05,
    BT_LINK_KEY_TYPE_CHANGED_COMBINATION = 0x06,
    BT_LINK_KEY_TYPE_UNAUTHEN_P256       = 0x07,
    BT_LINK_KEY_TYPE_AUTHEN_P256         = 0x08,
} T_BT_LINK_KEY_TYPE;

/**
 * btm.h
 *
 * \brief    BT QoS type.
 *
 * \ingroup  BT_BTM
 */
typedef enum t_bt_qos_type
{
    BT_QOS_TYPE_NO_TRAFFIC    = 0x00,
    BT_QOS_TYPE_BEST_EFFORT   = 0x01,
    BT_QOS_TYPE_GUARANTEED    = 0x02,
} T_BT_QOS_TYPE;

/**
 * btm.h
 *
 * \brief  BT manager event type for \ref P_BT_MGR_CBACK.
 *
 * \ingroup BT_BTM
 */
typedef enum t_bt_event
{
    BT_EVENT_UNKNOWN                                  = 0x0000,

    /**
     * \brief Indicate BT stack and profiles are ready \ref T_BT_EVENT_PARAM_READY.
     */
    BT_EVENT_READY                                    = 0x0001,

    /**
     * \brief Indicate BT device mode response \ref T_BT_EVENT_PARAM_DEVICE_MODE_RSP.
     */
    BT_EVENT_DEVICE_MODE_RSP                          = 0x0002,

    /**
     * \brief Indicate local BT device address is changed \ref T_BT_EVENT_PARAM_LOCAL_ADDR_CHANGED.
     */
    BT_EVENT_LOCAL_ADDR_CHANGED                       = 0x0003,

    /**
     * \brief Indicate local BT device name is changed \ref T_BT_EVENT_PARAM_LOCAL_NAME_CHANGED.
     */
    BT_EVENT_LOCAL_NAME_CHANGED                       = 0x0004,

    /**
     * \brief Indicate BT user confirmation of a numeric value is requested \ref T_BT_EVENT_PARAM_USER_CONFIRMATION_REQ.
     */
    BT_EVENT_USER_CONFIRMATION_REQ                    = 0x0005,

    /**
     * \brief Indicate a single inquiry result \ref T_BT_EVENT_PARAM_INQUIRY_RESULT.
     */
    BT_EVENT_INQUIRY_RESULT                           = 0x0006,

    /**
     * \brief Indicate inquiry response event \ref T_BT_EVENT_PARAM_INQUIRY_RSP.
     */
    BT_EVENT_INQUIRY_RSP                              = 0x0007,

    /**
     * \brief Indicate inquiry complete event \ref T_BT_EVENT_PARAM_INQUIRY_CMPL.
     */
    BT_EVENT_INQUIRY_CMPL                             = 0x0008,

    /**
     * \brief Indicate inquiry cancel event \ref T_BT_EVENT_PARAM_INQUIRY_CANCEL_RSP.
     */
    BT_EVENT_INQUIRY_CANCEL_RSP                       = 0x0009,

    /**
     * \brief Indicate acl link status \ref T_BT_EVENT_PARAM_ACL_LINK_STATUS.
     */
    BT_EVENT_ACL_LINK_STATUS                          = 0x1000,

    /**
     * \brief Indicate remote device name \ref T_BT_EVENT_PARAM_ACL_REMOTE_NAME_RSP.
     */
    BT_EVENT_ACL_REMOTE_NAME_RSP                      = 0x1001,

    /**
     * \brief Indicate role switch fail \ref T_BT_EVENT_PARAM_ACL_ROLE_SWITCH_FAIL.
     */
    BT_EVENT_ACL_ROLE_SWITCH_FAIL                     = 0x1002,

    /**
     * \brief Indicate acl connection request from remote device \ref T_BT_EVENT_PARAM_ACL_CONN_IND.
     */
    BT_EVENT_ACL_CONN_IND                             = 0x1003,

    /**
     * \brief Indicate acl sniffing connection complete \ref T_BT_EVENT_PARAM_ACL_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_ACL_SNIFFING_CONN_CMPL                   = 0x1004,

    /**
     * \brief Indicate acl sniffing disconnection complete \ref T_BT_EVENT_PARAM_ACL_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_ACL_SNIFFING_DISCONN_CMPL                = 0x1005,

    /**
     * \brief Information of link key \ref T_BT_EVENT_PARAM_LINK_KEY_INFO.
     */
    BT_EVENT_LINK_KEY_INFO                            = 0x1006,

    /**
     * \brief Indicate link key request during authentication \ref T_BT_EVENT_PARAM_LINK_KEY_REQ.
     */
    BT_EVENT_LINK_KEY_REQ                             = 0x1007,

    /**
     * \brief Indicate pin code request during authentication \ref T_BT_EVENT_PARAM_LINK_PIN_CODE_REQ.
     */
    BT_EVENT_LINK_PIN_CODE_REQ                        = 0x1008,

    /**
     * \brief Information of acl link PER information \ref T_BT_EVENT_PARAM_LINK_PER_INFO.
     */
    BT_EVENT_LINK_PER_INFO                            = 0x1009,

    /**
     * \brief Information of acl link RSSI information \ref T_BT_EVENT_PARAM_LINK_RSSI_INFO.
     */
    BT_EVENT_LINK_RSSI_INFO                           = 0x100a,

    /**
     * \brief Information of set pairing tx power rsp \ref T_BT_EVENT_PARAM_PAIRING_TX_POWER_SET_RSP.
     */
    BT_EVENT_PAIRING_TX_POWER_SET_RSP                 = 0x100b,

    /**
     * \brief Information of set link tx power rsp \ref T_BT_EVENT_PARAM_LINK_TX_POWER_SET_RSP.
     */
    BT_EVENT_LINK_TX_POWER_SET_RSP                    = 0x100c,

    /**
     * \brief Indicate SCO connection indication \ref T_BT_EVENT_PARAM_SCO_CONN_IND.
     */
    BT_EVENT_SCO_CONN_IND                             = 0x1100,

    /**
     * \brief Indicate SCO connection response \ref T_BT_EVENT_PARAM_SCO_CONN_RSP.
     */
    BT_EVENT_SCO_CONN_RSP                             = 0x1101,

    /**
     * \brief Indicate SCO connection complete \ref T_BT_EVENT_PARAM_SCO_CONN_CMPL.
     */
    BT_EVENT_SCO_CONN_CMPL                            = 0x1102,

    /**
     * \brief Indicate SCO disconnected \ref T_BT_EVENT_PARAM_SCO_DISCONNECTED.
     */
    BT_EVENT_SCO_DISCONNECTED                         = 0x1103,

    /**
     * \brief Indicate active SCO link changed \ref T_BT_EVENT_PARAM_SCO_LINK_SWITCH.
     */
    BT_EVENT_SCO_LINK_SWITCH                          = 0x1104,

    /**
     * \brief Indicate SCO sniffing connection complete \ref T_BT_EVENT_PARAM_SCO_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_SCO_SNIFFING_CONN_CMPL                   = 0x1105,

    /**
     * \brief Indicate SCO sniffing started \ref T_BT_EVENT_PARAM_SCO_SNIFFING_STARTED.
     */
    BT_EVENT_SCO_SNIFFING_STARTED                     = 0x1106,

    /**
     * \brief Indicate SCO sniffing stopped \ref T_BT_EVENT_PARAM_SCO_SNIFFING_STOPPED.
     */
    BT_EVENT_SCO_SNIFFING_STOPPED                     = 0x1107,

    /**
     * \brief Indicate SCO sniffing disconnection complete \ref T_BT_EVENT_PARAM_SCO_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_SCO_SNIFFING_DISCONN_CMPL                = 0x1108,

    /**
     * \brief Indicate SDP attribute information \ref T_BT_EVENT_PARAM_SDP_ATTR_INFO.
     */
    BT_EVENT_SDP_ATTR_INFO                            = 0x2000,

    /**
     * \brief Indicate SDP attribute information \ref T_BT_EVENT_PARAM_DID_ATTR_INFO.
     */
    BT_EVENT_DID_ATTR_INFO                            = 0x2001,

    /**
     * \brief Indicate SDP discover completed \ref T_BT_EVENT_PARAM_SDP_DISCOV_CMPL.
     */
    BT_EVENT_SDP_DISCOV_CMPL                          = 0x2002,

    /**
     * \brief Indicate SDP discover stop \ref T_BT_EVENT_PARAM_SDP_DISCOV_STOP.
     */
    BT_EVENT_SDP_DISCOV_STOP                          = 0x2003,

    /**
     * \brief Indicate SPP connection indication \ref T_BT_EVENT_PARAM_SPP_CONN_IND.
     */
    BT_EVENT_SPP_CONN_IND                             = 0x3000,

    /**
     * \brief Indicate SPP connection complete \ref T_BT_EVENT_PARAM_SPP_CONN_CMPL.
     */
    BT_EVENT_SPP_CONN_CMPL                            = 0x3001,

    /**
     * \brief Indicate SPP disconnected \ref T_BT_EVENT_PARAM_SPP_DISCONN_CMPL.
     */
    BT_EVENT_SPP_DISCONN_CMPL                         = 0x3002,

    /**
     * \brief Indicate SPP credit received \ref T_BT_EVENT_PARAM_SPP_CREDIT_RCVD.
     */
    BT_EVENT_SPP_CREDIT_RCVD                          = 0x3003,

    /**
     * \brief Indicate data received via SPP \ref T_BT_EVENT_PARAM_SPP_DATA_IND.
     */
    BT_EVENT_SPP_DATA_IND                             = 0x3004,

    /**
     * \brief Indicate data response \ref T_BT_EVENT_PARAM_SPP_DATA_RSP.
     */
    BT_EVENT_SPP_DATA_RSP                             = 0x3005,

    /**
     * \brief Indicate SPP sniffing connection complete \ref T_BT_EVENT_PARAM_SPP_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_SPP_SNIFFING_CONN_CMPL                   = 0x3006,

    /**
     * \brief Indicate SPP sniffing disconnection complete \ref T_BT_EVENT_PARAM_SPP_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_SPP_SNIFFING_DISCONN_CMPL                = 0x3007,

    /**
     * \brief Indicate A2DP connection indication \ref T_BT_EVENT_PARAM_A2DP_CONN_IND.
     */
    BT_EVENT_A2DP_CONN_IND                            = 0x3100,

    /**
     * \brief Indicate A2DP connection complete \ref T_BT_EVENT_PARAM_A2DP_CONN_CMPL.
     */
    BT_EVENT_A2DP_CONN_CMPL                           = 0x3101,

    /**
     * \brief Indicate A2DP disconnected \ref T_BT_EVENT_PARAM_A2DP_DISCONN_CMPL.
     */
    BT_EVENT_A2DP_DISCONN_CMPL                        = 0x3102,

    /**
     * \brief Indicate A2DP set configuration complete \ref T_BT_EVENT_PARAM_A2DP_CONFIG_CMPL.
     */
    BT_EVENT_A2DP_CONFIG_CMPL                         = 0x3103,

    /**
     * \brief Indicate A2DP stream open \ref T_BT_EVENT_PARAM_A2DP_STREAM_OPEN.
     */
    BT_EVENT_A2DP_STREAM_OPEN                         = 0x3104,

    /**
     * \brief Indicate A2DP stream start indication \ref T_BT_EVENT_PARAM_A2DP_STREAM_START_IND.
     */
    BT_EVENT_A2DP_STREAM_START_IND                    = 0x3105,

    /**
     * \brief Indicate A2DP stream start response \ref T_BT_EVENT_PARAM_A2DP_STREAM_START_RSP.
     */
    BT_EVENT_A2DP_STREAM_START_RSP                    = 0x3106,

    /**
     * \brief Indicate A2DP stream data indication \ref T_BT_EVENT_PARAM_A2DP_STREAM_DATA_IND.
     */
    BT_EVENT_A2DP_STREAM_DATA_IND                     = 0x3107,

    /**
     * \brief Indicate A2DP stream data response \ref T_BT_EVENT_PARAM_A2DP_STREAM_DATA_RSP.
     */
    BT_EVENT_A2DP_STREAM_DATA_RSP                     = 0x3108,

    /**
     * \brief Indicate A2DP stream stop \ref T_BT_EVENT_PARAM_A2DP_STREAM_STOP.
     */
    BT_EVENT_A2DP_STREAM_STOP                         = 0x3109,

    /**
     * \brief Indicate A2DP stream close \ref T_BT_EVENT_PARAM_A2DP_STREAM_CLOSE.
     */
    BT_EVENT_A2DP_STREAM_CLOSE                        = 0x310a,

    /**
     * \brief Indicate A2DP sniffing connection complete \ref T_BT_EVENT_PARAM_A2DP_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_A2DP_SNIFFING_CONN_CMPL                  = 0x310b,

    /**
     * \brief Indicate A2DP sniffing start indication \ref T_BT_EVENT_PARAM_A2DP_SNIFFING_START_IND.
     */
    BT_EVENT_A2DP_SNIFFING_START_IND                  = 0x310c,

    /**
     * \brief Indicate A2DP sniffing started \ref T_BT_EVENT_PARAM_A2DP_SNIFFING_STARTED.
     */
    BT_EVENT_A2DP_SNIFFING_STARTED                    = 0x310d,

    /**
     * \brief Indicate A2DP sniffing stopped \ref T_BT_EVENT_PARAM_A2DP_SNIFFING_STOPPED.
     */
    BT_EVENT_A2DP_SNIFFING_STOPPED                    = 0x310e,

    /**
     * \brief Indicate A2DP sniffing disconnection complete \ref T_BT_EVENT_PARAM_A2DP_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_A2DP_SNIFFING_DISCONN_CMPL               = 0x310f,

    /**
     * \brief Indicate AVRCP connection indication \ref T_BT_EVENT_PARAM_AVRCP_CONN_IND.
     */
    BT_EVENT_AVRCP_CONN_IND                           = 0x3200,

    /**
     * \brief Indicate AVRCP connection complete \ref T_BT_EVENT_PARAM_AVRCP_CONN_CMPL.
     */
    BT_EVENT_AVRCP_CONN_CMPL                          = 0x3201,

    /**
     * \brief Indicate AVRCP disconnected \ref T_BT_EVENT_PARAM_AVRCP_DISCONN_CMPL.
     */
    BT_EVENT_AVRCP_DISCONN_CMPL                       = 0x3202,

    /**
     * \brief Indicate AVRCP element attribute \ref T_BT_EVENT_PARAM_AVRCP_ELEM_ATTR.
     */
    BT_EVENT_AVRCP_ELEM_ATTR                          = 0x3203,

    /**
     * \brief Indicate AVRCP play status response \ref T_BT_EVENT_PARAM_AVRCP_PLAY_STATUS_RSP.
     */
    BT_EVENT_AVRCP_PLAY_STATUS_RSP                    = 0x3204,

    /**
     * \brief Indicate AVRCP volume changed \ref T_BT_EVENT_PARAM_AVRCP_VOLUME_CHANGED.
     */
    BT_EVENT_AVRCP_VOLUME_CHANGED                     = 0x3205,

    /**
     * \brief Indicate AVRCP volume up \ref T_BT_EVENT_PARAM_AVRCP_VOLUME_UP.
     */
    BT_EVENT_AVRCP_VOLUME_UP                          = 0x3206,

    /**
     * \brief Indicate AVRCP volume down \ref T_BT_EVENT_PARAM_AVRCP_VOLUME_DOWN.
     */
    BT_EVENT_AVRCP_VOLUME_DOWN                        = 0x3207,

    /**
     * \brief Indicate AVRCP registre volume changed \ref T_BT_EVENT_PARAM_AVRCP_REG_VOLUME_CHANGED.
     */
    BT_EVENT_AVRCP_REG_VOLUME_CHANGED                 = 0x3208,

    /**
     * \brief Indicate AVRCP track changed \ref T_BT_EVENT_PARAM_AVRCP_TRACK_CHANGED.
     */
    BT_EVENT_AVRCP_TRACK_CHANGED                      = 0x3209,

    /**
     * \brief Indicate AVRCP play status changed \ref T_BT_EVENT_PARAM_AVRCP_PLAY_STATUS_CHANGED.
     */
    BT_EVENT_AVRCP_PLAY_STATUS_CHANGED                = 0x320a,

    /**
     * \brief Indicate AVRCP sniffing connection complete \ref T_BT_EVENT_PARAM_AVRCP_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_AVRCP_SNIFFING_CONN_CMPL                 = 0x320b,

    /**
     * \brief Indicate AVRCP sniffing disconnection complete \ref T_BT_EVENT_PARAM_AVRCP_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_AVRCP_SNIFFING_DISCONN_CMPL              = 0x320c,

    /**
     * \brief Indicate AVRCP player app setting changed \ref T_BT_EVENT_PARAM_AVRCP_APP_SETTING_CHANGED.
     */
    BT_EVENT_AVRCP_APP_SETTING_CHANGED                = 0x320d,

    /**
     * \brief Indicate AVRCP playing content changed \ref T_BT_EVENT_PARAM_AVRCP_PLAYING_CONTENT_CHANGED.
     */
    BT_EVENT_AVRCP_PLAYING_CONTENT_CHANGED            = 0x320f,

    /**
     * \brief Indicate AVRCP available player changed \ref T_BT_EVENT_PARAM_AVRCP_AVAILABLE_PLAYER_CHANGED.
     */
    BT_EVENT_AVRCP_AVAILABLE_PLAYER_CHANGED           = 0x3210,

    /**
     * \brief Indicate AVRCP addressed player changed \ref T_BT_EVENT_PARAM_AVRCP_ADDRESSED_PLAYER_CHANGED.
     */
    BT_EVENT_AVRCP_ADDRESSED_PLAYER_CHANGED           = 0x3211,

    /**
     * \brief Indicate AVRCP UID changed \ref T_BT_EVENT_PARAM_AVRCP_UID_CHANGED.
     */
    BT_EVENT_AVRCP_UID_CHANGED                        = 0x3212,

    /**
     * \brief Indicate AVRCP player app setting attributes \ref T_BT_EVENT_PARAM_AVRCP_APP_SETTING_ATTRS.
     */
    BT_EVENT_AVRCP_APP_SETTING_ATTRS                  = 0x3213,

    /**
     * \brief Indicate AVRCP player app setting values \ref T_BT_EVENT_PARAM_AVRCP_APP_SETTING_VALUES.
     */
    BT_EVENT_AVRCP_APP_SETTING_VALUES                 = 0x3214,

    /**
     * \brief Indicate AVRCP player app current setting \ref T_BT_EVENT_PARAM_AVRCP_APP_SETTING.
     */
    BT_EVENT_AVRCP_APP_SETTING                        = 0x3215,

    /**
     * \brief Indicate AVRCP addressed player \ref T_BT_EVENT_PARAM_AVRCP_ADDRESSED_PLAYER.
     */
    BT_EVENT_AVRCP_ADDRESSED_PLAYER                   = 0x3216,

    /**
     * \brief Indicate AVRCP play item \ref T_BT_EVENT_PARAM_AVRCP_PLAY_ITEM.
     */
    BT_EVENT_AVRCP_PLAY_ITEM                          = 0x3217,

    /**
     * \brief Indicate AVRCP folder items \ref T_BT_EVENT_PARAM_AVRCP_FOLDER_ITEMS.
     */
    BT_EVENT_AVRCP_FOLDER_ITEMS                       = 0x3218,

    /**
     * \brief Indicate AVRCP item attribute \ref T_BT_EVENT_PARAM_AVRCP_ITEM_ATTR.
     */
    BT_EVENT_AVRCP_ITEM_ATTR                          = 0x3219,

    /**
     * \brief Indicate AVRCP search \ref T_BT_EVENT_PARAM_AVRCP_SEARCH.
     */
    BT_EVENT_AVRCP_SEARCH                             = 0x321a,

    /**
     * \brief Indicate AVRCP browsed player \ref T_BT_EVENT_PARAM_AVRCP_BROWSED_PLAYER.
     */
    BT_EVENT_AVRCP_BROWSED_PLAYER                     = 0x321b,

    /**
     * \brief Indicate AVRCP change path \ref T_BT_EVENT_PARAM_AVRCP_CHANGE_PATH.
     */
    BT_EVENT_AVRCP_CHANGE_PATH                        = 0x321c,

    /**
     * \brief Indicate AVRCP browsing connection indication \ref T_BT_EVENT_PARAM_AVRCP_BROWSING_CONN_IND.
     */
    BT_EVENT_AVRCP_BROWSING_CONN_IND                  = 0x3250,

    /**
     * \brief Indicate AVRCP browsing connection complete \ref T_BT_EVENT_PARAM_AVRCP_BROWSING_CONN_CMPL.
     */
    BT_EVENT_AVRCP_BROWSING_CONN_CMPL                 = 0x3251,

    /**
     * \brief Indicate AVRCP browsing disconnected \ref T_BT_EVENT_PARAM_AVRCP_BROWSING_DISCONN_CMPL.
     */
    BT_EVENT_AVRCP_BROWSING_DISCONN_CMPL              = 0x3252,

    /**
     * \brief Indicate AVRCP cover art connection complete \ref T_BT_EVENT_PARAM_AVRCP_COVER_ART_CONN_CMPL.
     */
    BT_EVENT_AVRCP_COVER_ART_CONN_CMPL                = 0x32a0,

    /**
     * \brief Indicate AVRCP cover art disconnected \ref T_BT_EVENT_PARAM_AVRCP_COVER_ART_DISCONN_CMPL.
     */
    BT_EVENT_AVRCP_COVER_ART_DISCONN_CMPL             = 0x32a1,

    /**
     * \brief Indicate AVRCP cover art data \ref T_BT_EVENT_PARAM_AVRCP_COVER_ART_DATA_IND.
     */
    BT_EVENT_AVRCP_COVER_ART_DATA_IND                 = 0x32a2,

    /**
     * \brief Indicate iAP connection indication \ref T_BT_EVENT_PARAM_IAP_CONN_IND.
     */
    BT_EVENT_IAP_CONN_IND                             = 0x3300,

    /**
     * \brief Indicate iAP connection complete \ref T_BT_EVENT_PARAM_IAP_CONN_CMPL.
     */
    BT_EVENT_IAP_CONN_CMPL                            = 0x3301,

    /**
     * \brief Indicate iAP disconnected \ref T_BT_EVENT_PARAM_IAP_DISCONN_CMPL.
     */
    BT_EVENT_IAP_DISCONN_CMPL                         = 0x3302,

    /**
     * \brief Indicate iAP authentication complete \ref BT_EVENT_IAP_AUTHEN_CMPL.
     */
    BT_EVENT_IAP_AUTHEN_CMPL                          = 0x3303,

    /**
     * \brief Indicate that iAP data session opened \ref T_BT_EVENT_PARAM_IAP_DATA_SESSION_OPEN.
     */
    BT_EVENT_IAP_DATA_SESSION_OPEN                    = 0x3304,

    /**
     * \brief Indicate iAP data session closed \ref T_BT_EVENT_PARAM_IAP_DATA_SESSION_CLOSE.
     */
    BT_EVENT_IAP_DATA_SESSION_CLOSE                   = 0x3305,

    /**
     * \brief Indicate iAP data transmitted \ref T_BT_EVENT_PARAM_IAP_DATA_SENT.
     */
    BT_EVENT_IAP_DATA_SENT                            = 0x3306,

    /**
     * \brief Indicate data received via iap \ref BT_EVENT_IAP_DATA_IND.
     */
    BT_EVENT_IAP_DATA_IND                             = 0x3307,

    /**
     * \brief Indicate identity request \ref BT_EVENT_IAP_IDENTITY_INFO_REQ.
     */
    BT_EVENT_IAP_IDENTITY_INFO_REQ                    = 0x3308,

    /**
     * \brief Indicate iAP resetted \ref T_BT_EVENT_PARAM_IAP_RESET.
     */
    BT_EVENT_IAP_RESET                                = 0x3309,

    /**
     * \brief Indicate iAP control session message \ref T_BT_EVENT_PARAM_IAP_CTRL_MSG_IND.
     */
    BT_EVENT_IAP_CTRL_MSG_IND                         = 0x330a,

    /**
     * \brief Indicate iAP sniffing connection complete \ref T_BT_EVENT_PARAM_IAP_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_IAP_SNIFFING_CONN_CMPL                   = 0x330b,

    /**
     * \brief Indicate iAP sniffing disconnection complete \ref T_BT_EVENT_PARAM_IAP_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_IAP_SNIFFING_DISCONN_CMPL                = 0x330c,

    /**
     * \brief Indicate HFP connection indication \ref T_BT_EVENT_PARAM_HFP_CONN_IND.
     */
    BT_EVENT_HFP_CONN_IND                             = 0x3400,

    /**
     * \brief Indicate HFP connection complete \ref T_BT_EVENT_PARAM_HFP_CONN_CMPL.
     */
    BT_EVENT_HFP_CONN_CMPL                            = 0x3401,

    /**
     * \brief Indicate HFP disconnected \ref T_BT_EVENT_PARAM_HFP_DISCONN_CMPL.
     */
    BT_EVENT_HFP_DISCONN_CMPL                         = 0x3402,

    /**
     * \brief Indicate HFP speaker volume changed \ref T_BT_EVENT_PARAM_HFP_SPK_VOLUME_CHANGED.
     */
    BT_EVENT_HFP_SPK_VOLUME_CHANGED                   = 0x3403,

    /**
     * \brief Indicate HFP microphone volume changed \ref T_BT_EVENT_PARAM_HFP_MIC_VOLUME_CHANGED.
     */
    BT_EVENT_HFP_MIC_VOLUME_CHANGED                   = 0x3404,

    /**
     * \brief Indicate HFP call status change \ref T_BT_EVENT_PARAM_HFP_CALL_STATUS.
     */
    BT_EVENT_HFP_CALL_STATUS                          = 0x3405,

    /**
     * \brief Indicate HFP service status change \ref T_BT_EVENT_PARAM_HFP_SERVICE_STATUS.
     */
    BT_EVENT_HFP_SERVICE_STATUS                       = 0x3406,

    /**
     * \brief Indicate the incoming caller phone number \ref T_BT_EVENT_PARAM_HFP_CALLER_ID_IND.
     */
    BT_EVENT_HFP_CALLER_ID_IND                        = 0x3407,

    /**
     * \brief Indicate the network operator \ref T_BT_EVENT_PARAM_HFP_NETWORK_OPERATOR_IND.
     */
    BT_EVENT_HFP_NETWORK_OPERATOR_IND                 = 0x3408,

    /**
     * \brief Indicate the subscriber number \ref T_BT_EVENT_PARAM_HFP_SUBSCRIBER_NUMBER_IND.
     */
    BT_EVENT_HFP_SUBSCRIBER_NUMBER_IND                = 0x3409,

    /**
     * \brief Indicate the current calls \ref T_BT_EVENT_PARAM_HFP_CURRENT_CALL_LIST_IND.
     */
    BT_EVENT_HFP_CURRENT_CALL_LIST_IND                = 0x340a,

    /**
     * \brief Indicate the call waiting \ref T_BT_EVENT_PARAM_HFP_CALL_WAITING_IND.
     */
    BT_EVENT_HFP_CALL_WAITING_IND                     = 0x340b,

    /**
     * \brief Indicate the signal indicator \ref T_BT_EVENT_PARAM_HFP_SIGNAL_IND.
     */
    BT_EVENT_HFP_SIGNAL_IND                           = 0x340c,

    /**
    * \brief Indicate the roam indicator \ref T_BT_EVENT_PARAM_HFP_ROAM_IND.
    */
    BT_EVENT_HFP_ROAM_IND                             = 0x340d,

    /**
    * \brief Indicate the battery indicator \ref T_BT_EVENT_PARAM_HFP_BATTERY_IND.
    */
    BT_EVENT_HFP_BATTERY_IND                          = 0x340e,

    /**
    * \brief Indicate remote AG supported features \ref T_BT_EVENT_PARAM_HFP_SUPPORTED_FEATURES_IND.
    */
    BT_EVENT_HFP_SUPPORTED_FEATURES_IND               = 0x340f,

    /**
     * \brief Indicate HFP unknow AT event \ref T_BT_EVENT_PARAM_HFP_UNKNOW_AT_EVENT.
     */
    BT_EVENT_HFP_UNKNOW_AT_EVENT                      = 0x3410,

    /**
     * \brief Indicate HFP vendor AT command result \ref T_BT_EVENT_PARAM_HFP_VENDOR_AT_RESULT.
     */
    BT_EVENT_HFP_VENDOR_AT_RESULT                     = 0x3411,

    /**
     * \brief Indicate an incoming call \ref T_BT_EVENT_PARAM_HFP_RING_ALERT.
     */
    BT_EVENT_HFP_RING_ALERT                           = 0x3412,

    /**
     * \brief Indicate HFP sniffing connection complete \ref T_BT_EVENT_PARAM_HFP_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_HFP_SNIFFING_CONN_CMPL                   = 0x3413,

    /**
     * \brief Indicate HFP sniffing disconnection complete \ref T_BT_EVENT_PARAM_HFP_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_HFP_SNIFFING_DISCONN_CMPL                = 0x3414,

    /**
     * \brief Indicate HFP AG connection indication \ref T_BT_EVENT_PARAM_HFP_AG_CONN_IND.
     */
    BT_EVENT_HFP_AG_CONN_IND                          = 0x34a0,

    /**
     * \brief Indicate HFP AG connection complete \ref T_BT_EVENT_PARAM_HFP_AG_CONN_CMPL.
     */
    BT_EVENT_HFP_AG_CONN_CMPL                         = 0x34a1,

    /**
     * \brief Indicate HFP AG disconnected \ref T_BT_EVENT_PARAM_HFP_AG_DISCONN_CMPL.
     */
    BT_EVENT_HFP_AG_DISCONN_CMPL                      = 0x34a2,

    /**
     * \brief Indicate HFP AG microphone volume changed \ref T_BT_EVENT_PARAM_HFP_AG_MIC_VOLUME_CHANGED.
     */
    BT_EVENT_HFP_AG_MIC_VOLUME_CHANGED                = 0x34a3,

    /**
     * \brief Indicate HFP AG speaker volume changed \ref T_BT_EVENT_PARAM_HFP_AG_SPK_VOLUME_CHANGED.
     */
    BT_EVENT_HFP_AG_SPK_VOLUME_CHANGED                = 0x34a4,

    /**
     * \brief Indicate HFP AG call status change \ref T_BT_EVENT_PARAM_HFP_AG_CALL_STATUS.
     */
    BT_EVENT_HFP_AG_CALL_STATUS                       = 0x34a5,

    /**
     * \brief Indicate HFP AG call indicators status request \ref T_BT_EVENT_PARAM_HFP_AG_INDICATORS_STATUS_REQ.
     */
    BT_EVENT_HFP_AG_INDICATORS_STATUS_REQ             = 0x34a6,

    /**
     * \brief Indicate HF battery level \ref T_BT_EVENT_PARAM_HFP_AG_BATTERY_LEVEL.
     */
    BT_EVENT_HFP_AG_BATTERY_LEVEL                     = 0x34a7,

    /**
     * \brief Indicate HF select codec type \ref T_BT_EVENT_PARAM_HFP_AG_CODEC_TYPE_SELECTED.
     */
    BT_EVENT_HFP_AG_HF_CODEC_TYPE_SELECTED            = 0x34a8,

    /**
     * \brief Indicate HFP AG SCO packet recieved \ref T_BT_EVENT_PARAM_HFP_AG_SCO_DATA_IND.
     */
    BT_EVENT_HFP_AG_SCO_DATA_IND                      = 0x34a9,

    /**
    * \brief Indicate remote HF supported features \ref T_BT_EVENT_PARAM_HFP_AG_SUPPORTED_FEATURES_IND.
    */
    BT_EVENT_HFP_AG_SUPPORTED_FEATURES_IND            = 0x34aa,

    /**
     * \brief Indicate PBAP connection complete \ref T_BT_EVENT_PARAM_PBAP_CONN_CMPL.
     */
    BT_EVENT_PBAP_CONN_CMPL                           = 0x3500,

    /**
     * \brief Indicate PBAP disconnected \ref T_BT_EVENT_PARAM_PBAP_DISCONN_CMPL.
     */
    BT_EVENT_PBAP_DISCONN_CMPL                        = 0x3501,

    /**
     * \brief Indicate the incoming caller phone number \ref T_BT_EVENT_PARAM_PBAP_CALLER_ID_NAME.
     */
    BT_EVENT_PBAP_CALLER_ID_NAME                      = 0x3502,

    /**
     * \brief Indicate the phone_book set complete \ref T_BT_EVENT_PARAM_PBAP_SET_PHONE_BOOK_CMPL.
     */
    BT_EVENT_PBAP_SET_PHONE_BOOK_CMPL                 = 0x3503,

    /**
     * \brief Indicate PBAP sniffing connection complete \ref T_BT_EVENT_PARAM_PBAP_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_PBAP_SNIFFING_CONN_CMPL                  = 0x3504,

    /**
     * \brief Indicate PBAP sniffing disconnection complete \ref T_BT_EVENT_PARAM_PBAP_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_PBAP_SNIFFING_DISCONN_CMPL               = 0x3505,

    /**
     * \brief Indicate the phone_book pull complete \ref T_BT_EVENT_PARAM_PBAP_GET_PHONE_BOOK_CMPL.
     */
    BT_EVENT_PBAP_GET_PHONE_BOOK_CMPL                 = 0x3506,

    /**
     * \brief Indicate the phone_book pull complete \ref T_BT_EVENT_PARAM_PBAP_GET_PHONE_BOOK_SIZE_CMPL.
     */
    BT_EVENT_PBAP_GET_PHONE_BOOK_SIZE_CMPL            = 0x3507,

    /**
     * \brief Indicate HID control connection indication \ref T_BT_EVENT_PARAM_HID_CONN_IND.
     */
    BT_EVENT_HID_CONN_IND                             = 0x3600,

    /**
     * \brief Indicate HID control connection complete \ref T_BT_EVENT_PARAM_HID_CONN_CMPL.
     */
    BT_EVENT_HID_CONN_CMPL                            = 0x3601,

    /**
     * \brief Indicate HID control disconnection complete \ref T_BT_EVENT_PARAM_HID_DISCONN_CMPL.
     */
    BT_EVENT_HID_DISCONN_CMPL                         = 0x3602,

    /**
     * \brief Indicate HID data indication \ref T_BT_EVENT_PARAM_HID_CONTROL_DATA_IND.
     */
    BT_EVENT_HID_CONTROL_DATA_IND                     = 0x3603,

    /**
     * \brief Indicate HID get report indication \ref T_BT_EVENT_PARAM_HID_GET_REPORT_IND.
     */
    BT_EVENT_HID_GET_REPORT_IND                       = 0x3604,

    /**
     * \brief Indicate HID set report indication \ref T_BT_EVENT_PARAM_HID_SET_REPORT_IND.
     */
    BT_EVENT_HID_SET_REPORT_IND                       = 0x3605,

    /**
     * \brief Indicate HID get protocol indication \ref T_BT_EVENT_PARAM_HID_GET_PROTOCOL_IND.
     */
    BT_EVENT_HID_GET_PROTOCOL_IND                     = 0x3606,

    /**
     * \brief Indicate HID set protocol indication \ref T_BT_EVENT_PARAM_HID_SET_PROTOCOL_IND.
     */
    BT_EVENT_HID_SET_PROTOCOL_IND                     = 0x3607,

    /**
     * \brief Indicate HID set idle indication \ref T_BT_EVENT_PARAM_HID_SET_IDLE_IND.
     */
    BT_EVENT_HID_SET_IDLE_IND                         = 0x3608,

    /**
     * \brief Indicate HID sniffing connection complete \ref T_BT_EVENT_PARAM_HID_SNIFFING_CONN_CMPL.
     */
    BT_EVENT_HID_SNIFFING_CONN_CMPL                   = 0x3609,

    /**
     * \brief Indicate HID sniffing disconnection complete \ref T_BT_EVENT_PARAM_HID_SNIFFING_DISCONN_CMPL.
     */
    BT_EVENT_HID_SNIFFING_DISCONN_CMPL                = 0x360a,

    /**
     * \brief Indicate HID interrupt data indication \ref T_BT_EVENT_PARAM_HID_INTERRUPT_DATA_IND.
     */
    BT_EVENT_HID_INTERRUPT_DATA_IND                   = 0x360b,

    /**
     * \brief Indicate MAP MNS connection indication \ref T_BT_EVENT_PARAM_MAP_MNS_CONN_IND.
     */
    BT_EVENT_MAP_MNS_CONN_IND                         = 0x3700,

    /**
     * \brief Indicate MAP MNS connection complete \ref T_BT_EVENT_PARAM_MAP_MNS_CONN_CMPL.
     */
    BT_EVENT_MAP_MNS_CONN_CMPL                        = 0x3701,

    /**
     * \brief Indicate MAP MNS disconnection complete \ref T_BT_EVENT_PARAM_MAP_MNS_DISCONN_CMPL.
     */
    BT_EVENT_MAP_MNS_DISCONN_CMPL                     = 0x3702,

    /**
     * \brief Indicate MAP MAS connection complete \ref T_BT_EVENT_PARAM_MAP_MAS_CONN_CMPL.
     */
    BT_EVENT_MAP_MAS_CONN_CMPL                        = 0x3703,

    /**
     * \brief Indicate MAP MAS disconnection complete \ref T_BT_EVENT_PARAM_MAP_MAS_DISCONN_CMPL.
     */
    BT_EVENT_MAP_MAS_DISCONN_CMPL                     = 0x3704,

    /**
     * \brief Indicate MAP set folder complete \ref T_BT_EVENT_PARAM_MAP_SET_FOLDER_CMPL.
     */
    BT_EVENT_MAP_SET_FOLDER_CMPL                      = 0x3705,

    /**
     * \brief Indicate MAP register message notification complete \ref T_BT_EVENT_PARAM_MAP_REG_NOTIF_CMPL.
     */
    BT_EVENT_MAP_REG_NOTIF_CMPL                       = 0x3706,

    /**
     * \brief Indicate the get folder listing complete \ref T_BT_EVENT_PARAM_MAP_GET_FOLDER_LISTING_CMPL.
     */
    BT_EVENT_MAP_GET_FOLDER_LISTING_CMPL              = 0x3707,

    /**
     * \brief Indicate the get message listing complete \ref T_BT_EVENT_PARAM_MAP_GET_MSG_LISTING_CMPL.
     */
    BT_EVENT_MAP_GET_MSG_LISTING_CMPL                 = 0x3708,

    /**
     * \brief Indicate the get message complete \ref T_BT_EVENT_PARAM_MAP_GET_MSG_CMPL.
     */
    BT_EVENT_MAP_GET_MSG_CMPL                         = 0x3709,

    /**
     * \brief Indicate the push message complete \ref T_BT_EVENT_PARAM_MAP_PUSH_MSG_CMPL.
     */
    BT_EVENT_MAP_PUSH_MSG_CMPL                        = 0x370a,

    /**
     * \brief Indicate the MSE report change of the message listing  \ref T_BT_EVENT_PARAM_MAP_MSG_NOTIFICATION.
     */
    BT_EVENT_MAP_MSG_NOTIFICATION                     = 0x370b,

    /**
     * \brief Indicate remote roleswap status \ref T_BT_EVENT_PARAM_REMOTE_ROLESWAP_STATUS.
     */
    BT_EVENT_REMOTE_ROLESWAP_STATUS                   = 0x4000,

    /**
     * \brief Indicate romote connection complete \ref T_BT_EVENT_PARAM_REMOTE_CONN_CMPL.
     */
    BT_EVENT_REMOTE_CONN_CMPL                         = 0x4001,

    /**
     * \brief Indicate romote connected \ref T_BT_EVENT_PARAM_REMOTE_DISCONN_CMPL.
     */
    BT_EVENT_REMOTE_DISCONN_CMPL                      = 0x4002,
} T_BT_EVENT;

/**
 * btm.h
 *
 * \brief  BT manager roleswap status.
 *
 * \ingroup BT_BTM
 */
typedef enum t_btm_roleswap_status
{
    BTM_ROLESWAP_STATUS_ACL_INFO         = 0x00,
    BTM_ROLESWAP_STATUS_SCO_INFO         = 0x01,
    BTM_ROLESWAP_STATUS_SPP_INFO         = 0x02,
    BTM_ROLESWAP_STATUS_A2DP_INFO        = 0x03,
    BTM_ROLESWAP_STATUS_AVRCP_INFO       = 0x04,
    BTM_ROLESWAP_STATUS_HFP_INFO         = 0x05,
    BTM_ROLESWAP_STATUS_PBAP_INFO        = 0x06,
    BTM_ROLESWAP_STATUS_HID_INFO         = 0x07,
    BTM_ROLESWAP_STATUS_AVP_INFO         = 0x08,
    BTM_ROLESWAP_STATUS_SUCCESS          = 0x09,
    BTM_ROLESWAP_STATUS_FAIL             = 0x0a,
    BTM_ROLESWAP_STATUS_BDADDR           = 0x0b,
    BTM_ROLESWAP_STATUS_FLOW_STOP        = 0x0c,
    BTM_ROLESWAP_STATUS_FLOW_GO          = 0x0d,
    BTM_ROLESWAP_STATUS_DATA_SYNC_CMPL   = 0x0e,
    BTM_ROLESWAP_STATUS_IDENT_CHANGE     = 0x0f,
    BTM_ROLESWAP_STATUS_START_IND        = 0x10,
    BTM_ROLESWAP_STATUS_TERMINATED       = 0x11,
} T_BTM_ROLESWAP_STATUS;

/**
 * btm.h
 *
 * \brief Define BT SDP attribute information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_sdp_attr_info
{
    T_GAP_UUID_TYPE     srv_class_uuid_type;    /**< Service class UUID type */
    T_GAP_UUID_DATA     srv_class_uuid_data;    /**< Service class UUID data */
    uint16_t            profile_version;        /**< Remote profile version */
    uint16_t            protocol_version;       /**< Remote protocol version */
    uint8_t             server_channel;         /**< Server channel number */
    uint16_t            supported_feat;         /**< Supported features */
    uint16_t            l2c_psm;                /**< L2CAP PSM value */
    uint8_t             supported_repos;        /**< Supported repositories */
    uint32_t            pbap_supported_feat;    /**< PBAP supported features */
} T_BT_SDP_ATTR_INFO;

/**
 * btm.h
 *
 * \brief  BT manager event parameter BT ready.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_ready
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_READY;

/**
 * btm.h
 *
 * \brief  BT manager event parameter local BT address changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_local_addr_changed
{
    uint8_t     bd_addr[6];
    uint16_t    cause;
} T_BT_EVENT_PARAM_LOCAL_ADDR_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter local BT device name changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_local_name_changed
{
    uint16_t    cause;
} T_BT_EVENT_PARAM_LOCAL_NAME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter user confirmation request.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_user_confirmation_req
{
    uint8_t                 bd_addr[6];        /**< Bluetooth address of remote device. */
    uint32_t                display_value;     /**< Numeric value to be displayed. */
} T_BT_EVENT_PARAM_USER_CONFIRMATION_REQ;

/**
 * btm.h
 *
 * \brief  BT manager event parameter BT device mode rsp.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_device_mode_rsp
{
    T_BT_DEVICE_MODE dev_mode;
    uint16_t         cause;
} T_BT_EVENT_PARAM_DEVICE_MODE_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter inquiry result information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_inquiry_result
{
    uint8_t     bd_addr[6];
    uint32_t    cod;
    int8_t      rssi;
    uint8_t     name[GAP_DEVICE_NAME_LEN];
    uint8_t     eir_len;
    uint8_t    *p_eir;
} T_BT_EVENT_PARAM_INQUIRY_RESULT;

/**
 * btm.h
 *
 * \brief  BT manager event parameter inquiry response.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_inquiry_rsp
{
    uint16_t cause;
} T_BT_EVENT_PARAM_INQUIRY_RSP;


/**
 * btm.h
 *
 * \brief  BT manager event parameter inquiry completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_inquiry_cmpl
{
    uint16_t cause;
} T_BT_EVENT_PARAM_INQUIRY_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter inquiry cancel response.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_inquiry_cancel_rsp
{
    uint16_t cause;
} T_BT_EVENT_PARAM_INQUIRY_CANCEL_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_conn_ind
{
    uint8_t  bd_addr[6];
} T_BT_EVENT_PARAM_SCO_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO connection response.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_conn_rsp
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_SCO_CONN_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_conn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t handle;
    uint16_t cause;
    uint8_t  air_mode;
    uint8_t  tx_pkt_len;
    uint8_t  rx_pkt_len;
} T_BT_EVENT_PARAM_SCO_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO disconnected.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_disconnected
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_SCO_DISCONNECTED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO link switch.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_link_switch
{
    uint8_t bd_addr[6];
    uint16_t handle;
} T_BT_EVENT_PARAM_SCO_LINK_SWITCH;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_sniffing_conn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t handle;
    uint8_t  air_mode;
    uint8_t  tx_pkt_len;
    uint8_t  rx_pkt_len;
} T_BT_EVENT_PARAM_SCO_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO sniffing started.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_sniffing_started
{
    uint8_t  bd_addr[6];
    uint16_t cause;
    uint8_t  air_mode;
    uint8_t  tx_pkt_len;
    uint8_t  rx_pkt_len;
} T_BT_EVENT_PARAM_SCO_SNIFFING_STARTED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO sniffing stopped.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_sniffing_stopped
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_SCO_SNIFFING_STOPPED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SCO sniffing disconnection complete.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sco_sniffing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_SCO_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter acl link status.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_acl_link_status
{
    T_GAP_ACL_STATUS_INFO *info;
} T_BT_EVENT_PARAM_ACL_LINK_STATUS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter acl reply remote name.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_acl_remote_name_rsp
{
    T_GAP_REMOTE_NAME_INFO *info;
    uint16_t name_len;
} T_BT_EVENT_PARAM_ACL_REMOTE_NAME_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter acl role switch fail.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_acl_role_switch_fail
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_ACL_ROLE_SWITCH_FAIL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter acl link connection request indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_acl_conn_ind
{
    uint8_t  bd_addr[6];
    uint32_t cod;
} T_BT_EVENT_PARAM_ACL_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter acl sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_acl_sniffing_conn_cmpl
{
    uint8_t             bd_addr[6];
    uint16_t            cause;
} T_BT_EVENT_PARAM_ACL_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter acl sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_acl_sniffing_disconn_cmpl
{
    uint8_t             bd_addr[6];
    uint16_t            cause;
} T_BT_EVENT_PARAM_ACL_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP credit received.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_credit_rcvd
{
    uint8_t bd_addr[6];
    uint8_t local_server_chann;
    uint8_t link_credit;
} T_BT_EVENT_PARAM_SPP_CREDIT_RCVD;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP data transfer.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_data_ind
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
    uint8_t *data;
    uint16_t len;
} T_BT_EVENT_PARAM_SPP_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP data response with ack flag set.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_data_rsp
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
} T_BT_EVENT_PARAM_SPP_DATA_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sdpp_conn_ind
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
    uint16_t frame_size;
} T_BT_EVENT_PARAM_SPP_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_conn_cmpl
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
    uint8_t  link_credit;
    uint16_t frame_size;
} T_BT_EVENT_PARAM_SPP_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
    uint16_t cause;
} T_BT_EVENT_PARAM_SPP_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_sniffing_conn_cmpl
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
} T_BT_EVENT_PARAM_SPP_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SPP sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_spp_sniffing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint8_t  local_server_chann;
    uint16_t cause;
} T_BT_EVENT_PARAM_SPP_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_conn_ind
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_A2DP_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP config completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_config_cmpl
{
    uint8_t  bd_addr[6];
    uint8_t  codec_type;
    union
    {
        struct
        {
            uint8_t sampling_frequency;
            uint8_t channel_mode;
            uint8_t block_length;
            uint8_t subbands;
            uint8_t allocation_method;
            uint8_t min_bitpool;
            uint8_t max_bitpool;
        } sbc;
        struct
        {
            uint8_t  object_type;
            uint16_t sampling_frequency;
            uint8_t  channel_number;
            bool     vbr_supported;
            uint32_t bit_rate;
        } aac;
        struct
        {
            uint8_t  info[12];
        } vendor;
    } codec_info;
} T_BT_EVENT_PARAM_A2DP_CONFIG_CMPL;


/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream start indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_start_ind
{
    uint8_t  bd_addr[6];
    uint8_t  codec_type;
    union
    {
        struct
        {
            uint8_t sampling_frequency;
            uint8_t channel_mode;
            uint8_t block_length;
            uint8_t subbands;
            uint8_t allocation_method;
            uint8_t min_bitpool;
            uint8_t max_bitpool;
        } sbc;
        struct
        {
            uint8_t  object_type;
            uint16_t sampling_frequency;
            uint8_t  channel_number;
            bool     vbr_supported;
            uint32_t bit_rate;
        } aac;
        struct
        {
            uint8_t  info[12];
        } vendor;
    } codec_info;
} T_BT_EVENT_PARAM_A2DP_STREAM_START_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream start response.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_start_rsp
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_STREAM_START_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream data indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_data_ind
{
    uint8_t     bd_addr[6];
    uint32_t    bt_clock;
    uint16_t    seq_num;
    uint8_t     frame_num;
    uint16_t    len;
    uint8_t    *payload;
} T_BT_EVENT_PARAM_A2DP_STREAM_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream data response with ack flag set.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_data_rsp
{
    uint8_t     bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_STREAM_DATA_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream stop.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_stop
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_STREAM_STOP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream open.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_open
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_STREAM_OPEN;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP stream close.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_stream_close
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_A2DP_STREAM_CLOSE;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_sniffing_conn_cmpl
{
    uint8_t  bd_addr[6];
    uint8_t  codec_type;
    union
    {
        struct
        {
            uint8_t sampling_frequency;
            uint8_t channel_mode;
            uint8_t block_length;
            uint8_t subbands;
            uint8_t allocation_method;
            uint8_t min_bitpool;
            uint8_t max_bitpool;
        } sbc;
        struct
        {
            uint8_t  object_type;
            uint16_t sampling_frequency;
            uint8_t  channel_number;
            bool     vbr_supported;
            uint32_t bit_rate;
        } aac;
        struct
        {
            uint8_t  info[12];
        } vendor;
    } codec_info;
} T_BT_EVENT_PARAM_A2DP_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP sniffing start indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_sniffing_start_ind
{
    uint8_t  bd_addr[6];
    uint8_t  codec_type;
    union
    {
        struct
        {
            uint8_t sampling_frequency;
            uint8_t channel_mode;
            uint8_t block_length;
            uint8_t subbands;
            uint8_t allocation_method;
            uint8_t min_bitpool;
            uint8_t max_bitpool;
        } sbc;
        struct
        {
            uint8_t  object_type;
            uint16_t sampling_frequency;
            uint8_t  channel_number;
            bool     vbr_supported;
            uint32_t bit_rate;
        } aac;
        struct
        {
            uint8_t  info[12];
        } vendor;
    } codec_info;
} T_BT_EVENT_PARAM_A2DP_SNIFFING_START_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP sniffing started.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_sniffing_started
{
    uint8_t  bd_addr[6];
    uint16_t cause;
    uint8_t  codec_type;
    union
    {
        struct
        {
            uint8_t sampling_frequency;
            uint8_t channel_mode;
            uint8_t block_length;
            uint8_t subbands;
            uint8_t allocation_method;
            uint8_t min_bitpool;
            uint8_t max_bitpool;
        } sbc;
        struct
        {
            uint8_t  object_type;
            uint16_t sampling_frequency;
            uint8_t  channel_number;
            bool     vbr_supported;
            uint32_t bit_rate;
        } aac;
        struct
        {
            uint8_t  info[12];
        } vendor;
    } codec_info;
} T_BT_EVENT_PARAM_A2DP_SNIFFING_STARTED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP sniffing stopped.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_sniffing_stopped
{
    uint8_t bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_A2DP_SNIFFING_STOPPED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter A2DP sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_a2dp_sniffing_disconn_cmpl
{
    uint8_t bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_A2DP_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP element attribute.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_elem_attr
{
    uint8_t bd_addr[6];
    uint8_t state;
    uint8_t num_of_attr;
    T_BT_AVRCP_ELEMENT_ATTR *attr;
} T_BT_EVENT_PARAM_AVRCP_ELEM_ATTR;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP play status response.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_play_status_rsp
{
    uint8_t  bd_addr[6];
    uint8_t  state;
    uint8_t  play_status;
    uint32_t length_ms;
    uint32_t position_ms;
} T_BT_EVENT_PARAM_AVRCP_PLAY_STATUS_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP volume changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_volume_changed
{
    uint8_t bd_addr[6];
    uint8_t volume;
} T_BT_EVENT_PARAM_AVRCP_VOLUME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP volume up.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_volume_up
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_VOLUME_UP;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP volume down.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_volume_down
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_VOLUME_DOWN;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP register volume changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_reg_volume_changed
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_REG_VOLUME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_conn_ind
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_AVRCP_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP track changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_track_changed
{
    uint8_t  bd_addr[6];
    uint64_t track_id;
} T_BT_EVENT_PARAM_AVRCP_TRACK_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP play status changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_play_status_changed
{
    uint8_t bd_addr[6];
    uint8_t play_status;
} T_BT_EVENT_PARAM_AVRCP_PLAY_STATUS_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_sniffing_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_sniffing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_AVRCP_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP player app setting changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_app_setting_changed
{
    uint8_t                 bd_addr[6];
    uint8_t                 num_of_attr;
    T_BT_AVRCP_APP_SETTING *p_app_setting;
} T_BT_EVENT_PARAM_AVRCP_APP_SETTING_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP playing content changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_playing_content_changed
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_PLAYING_CONTENT_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP available player changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_available_player_changed
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_AVAILABLE_PLAYER_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP addressed player changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_addressed_player_changed
{
    uint8_t  bd_addr[6];
    uint16_t player_id;
    uint16_t uid_counter;
} T_BT_EVENT_PARAM_AVRCP_ADDRESSED_PLAYER_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP UID changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_uid_changed
{
    uint8_t  bd_addr[6];
    uint16_t uid_counter;
} T_BT_EVENT_PARAM_AVRCP_UID_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP player app setting attributes.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_app_setting_attrs
{
    uint8_t  bd_addr[6];
    uint8_t  state;
    uint8_t  num_of_attr;
    uint8_t *p_attr_id;
} T_BT_EVENT_PARAM_AVRCP_APP_SETTING_ATTRS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP player app setting values.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_app_setting_values
{
    uint8_t  bd_addr[6];
    uint8_t  state;
    uint8_t  num_of_value;
    uint8_t *p_value;
} T_BT_EVENT_PARAM_AVRCP_APP_SETTING_VALUES;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP player app current setting.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_app_setting
{
    uint8_t                  bd_addr[6];
    uint8_t                  state;
    uint8_t                  num_of_attr;
    T_BT_AVRCP_APP_SETTING  *p_app_setting;
} T_BT_EVENT_PARAM_AVRCP_APP_SETTING;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP addressed player.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_addressed_player
{
    uint8_t  bd_addr[6];
    uint8_t  state;
} T_BT_EVENT_PARAM_AVRCP_ADDRESSED_PLAYER;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP play item.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_play_item
{
    uint8_t  bd_addr[6];
    uint8_t  state;
} T_BT_EVENT_PARAM_AVRCP_PLAY_ITEM;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP folder items.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_folder_items
{
    uint8_t   bd_addr[6];
    uint8_t   state;
    uint16_t  uid_counter;
    uint16_t  num_of_items;
    uint8_t   item_type;
    union
    {
        T_BT_AVRCP_MEDIA_PLAYER_ITEM  *p_media_player_items;
        T_BT_AVRCP_FOLDER_ITEM        *p_folder_items;
        T_BT_AVRCP_MEDIA_ELEMENT_ITEM *p_media_element_items;
    } u;
} T_BT_EVENT_PARAM_AVRCP_FOLDER_ITEMS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP item attribute.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_item_attr
{
    uint8_t               bd_addr[6];
    uint8_t               state;
    uint8_t               num_of_attr;
    T_BT_AVRCP_ITEM_ATTR *p_attr;
} T_BT_EVENT_PARAM_AVRCP_ITEM_ATTR;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP search.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_search
{
    uint8_t   bd_addr[6];
    uint8_t   state;
    uint16_t  uid_counter;
    uint32_t  num_of_items;
} T_BT_EVENT_PARAM_AVRCP_SEARCH;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP browsed player.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_browsed_player
{
    uint8_t             bd_addr[6];
    uint8_t             state;
    uint16_t            uid_counter;
    uint32_t            num_of_items;
    uint16_t            character_set_id;
    uint8_t             folder_depth;
    T_BT_AVRCP_FOLDER  *p_folders;
} T_BT_EVENT_PARAM_AVRCP_BROWSED_PLAYER;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP change path.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_change_path
{
    uint8_t   bd_addr[6];
    uint8_t   state;
    uint32_t  num_of_items;
} T_BT_EVENT_PARAM_AVRCP_CHANGE_PATH;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP browsing connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_browsing_conn_ind
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_BROWSING_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP browsing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_browsing_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_BROWSING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP browsing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_browsing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_AVRCP_BROWSING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP cover art connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_cover_art_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_AVRCP_COVER_ART_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP cover art disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_cover_art_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_AVRCP_COVER_ART_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AVRCP cover art data indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_avrcp_cover_art_data_ind
{
    uint8_t    bd_addr[6];
    uint8_t   *p_data;
    uint16_t   data_len;
    bool       data_end;
} T_BT_EVENT_PARAM_AVRCP_COVER_ART_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP authentication completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_authen_cmpl
{
    uint8_t bd_addr[6];
    bool    result;
} T_BT_EVENT_PARAM_IAP_AUTHEN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP data session open.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_data_session_open
{
    uint8_t  bd_addr[6];
    uint8_t  protocol_id;
    uint16_t session_id;
} T_BT_EVENT_PARAM_IAP_DATA_SESSION_OPEN;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP data session close.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_data_session_close
{
    uint8_t  bd_addr[6];
    uint8_t  protocol_id;
    uint16_t session_id;
} T_BT_EVENT_PARAM_IAP_DATA_SESSION_CLOSE;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP send data finish.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_data_sent
{
    uint8_t  bd_addr[6];
    uint16_t session_id;
    bool     success;
} T_BT_EVENT_PARAM_IAP_DATA_SENT;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_conn_ind
{
    uint8_t     bd_addr[6];
    uint16_t    frame_size;
} T_BT_EVENT_PARAM_IAP_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_conn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t frame_size;
} T_BT_EVENT_PARAM_IAP_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_disconn_cmpl
{
    uint8_t     bd_addr[6];
    uint16_t    cause;
} T_BT_EVENT_PARAM_IAP_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP data indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_data_ind
{
    uint8_t     bd_addr[6];
    uint16_t    eap_session_id;
    uint8_t     dev_seq_num;
    uint8_t    *p_data;
    uint16_t    len;
} T_BT_EVENT_PARAM_IAP_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP identify information request.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_identity_info_req
{
    uint8_t     bd_addr[6];
} T_BT_EVENT_PARAM_IAP_IDENTITY_INFO_REQ;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP reset.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_reset
{
    uint8_t     bd_addr[6];
} T_BT_EVENT_PARAM_IAP_RESET;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP control session message indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_ctrl_msg_ind
{
    uint8_t     bd_addr[6];
    uint16_t    msg_id;
    uint16_t    param_len;
    uint8_t    *p_param;
} T_BT_EVENT_PARAM_IAP_CTRL_MSG_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_sniffing_conn_cmpl
{
    uint8_t     bd_addr[6];
} T_BT_EVENT_PARAM_IAP_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter iAP sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_iap_sniffing_disconn_cmpl
{
    uint8_t     bd_addr[6];
    uint16_t    cause;
} T_BT_EVENT_PARAM_IAP_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP speaker volume changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_spk_volume_changed
{
    uint8_t bd_addr[6];
    uint8_t volume;
} T_BT_EVENT_PARAM_HFP_SPK_VOLUME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP microphone volume changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_mic_volume_changed
{
    uint8_t bd_addr[6];
    uint8_t volume;
} T_BT_EVENT_PARAM_HFP_MIC_VOLUME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP call status.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_call_status
{
    uint8_t bd_addr[6];
    uint8_t prev_status;
    uint8_t curr_status;
} T_BT_EVENT_PARAM_HFP_CALL_STATUS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP service status.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_service_status
{
    uint8_t bd_addr[6];
    uint8_t status;
} T_BT_EVENT_PARAM_HFP_SERVICE_STATUS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP caller number.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_caller_id_ind
{
    uint8_t  bd_addr[6];
    uint8_t *number;
    uint16_t num_len;
} T_BT_EVENT_PARAM_HFP_CALLER_ID_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP network operator.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_network_operator_ind
{
    uint8_t     bd_addr[6];
    uint8_t     mode;
    uint8_t     format;
    char        name[17];
} T_BT_EVENT_PARAM_HFP_NETWORK_OPERATOR_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP subscriber number.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_subscriber_number_ind
{
    uint8_t     bd_addr[6];
    char        number[20];
    uint8_t     type;
    uint8_t     service;
} T_BT_EVENT_PARAM_HFP_SUBSCRIBER_NUMBER_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP current calls.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_current_call_list_ind
{
    uint8_t     bd_addr[6];
    uint8_t     call_idx;
    uint8_t     dir_incoming;
    uint8_t     status;
    uint8_t     mode;
    uint8_t     is_multi_party;
    char        number[20];
    uint8_t     type;
} T_BT_EVENT_PARAM_HFP_CURRENT_CALL_LIST_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP call waiting.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_call_waiting_ind
{
    uint8_t     bd_addr[6];
    const char *number;
    uint8_t     type;
} T_BT_EVENT_PARAM_HFP_CALL_WAITING_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP signal indicator.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_signal_ind
{
    uint8_t  bd_addr[6];
    uint8_t  state;
} T_BT_EVENT_PARAM_HFP_SIGNAL_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP roam indicator.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_roam_ind
{
    uint8_t  bd_addr[6];
    uint8_t  state;
} T_BT_EVENT_PARAM_HFP_ROAM_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP battery indicator.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_battery_ind
{
    uint8_t  bd_addr[6];
    uint8_t  state;
} T_BT_EVENT_PARAM_HFP_BATTERY_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG supported features.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_supported_features_ind
{
    uint8_t  bd_addr[6];
    uint32_t ag_bitmap;
} T_BT_EVENT_PARAM_HFP_SUPPORTED_FEATURES_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP unknow AT event.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_unknow_at_event
{
    uint8_t  bd_addr[6];
    uint8_t *at_cmd;
    uint16_t at_len;
} T_BT_EVENT_PARAM_HFP_UNKNOW_AT_EVENT;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP ring alert.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ring_alert
{
    uint8_t bd_addr[6];
    bool    is_inband;
} T_BT_EVENT_PARAM_HFP_RING_ALERT;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP vendor AT result.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_vendor_at_result
{
    uint8_t bd_addr[6];
    bool    is_ok;
} T_BT_EVENT_PARAM_HFP_VENDOR_AT_RESULT;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_conn_ind
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_HFP_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_conn_cmpl
{
    uint8_t bd_addr[6];
    bool    is_hfp;
} T_BT_EVENT_PARAM_HFP_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_disconn_cmpl
{
    uint8_t  bd_addr[6];
    bool     is_hfp;
    uint16_t cause;
} T_BT_EVENT_PARAM_HFP_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_sniffing_conn_cmpl
{
    uint8_t bd_addr[6];
    bool    is_hfp;
} T_BT_EVENT_PARAM_HFP_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_sniffing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    bool     is_hfp;
    uint16_t cause;
} T_BT_EVENT_PARAM_HFP_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_conn_ind
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_HFP_AG_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_conn_cmpl
{
    uint8_t bd_addr[6];
    bool    is_hfp;
} T_BT_EVENT_PARAM_HFP_AG_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_disconn_cmpl
{
    uint8_t  bd_addr[6];
    bool     is_hfp;
    uint16_t cause;
} T_BT_EVENT_PARAM_HFP_AG_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG microphone volume changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_mic_volume_changed
{
    uint8_t bd_addr[6];
    uint8_t volume;
} T_BT_EVENT_PARAM_HFP_AG_MIC_VOLUME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG speaker volume changed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_spk_volume_changed
{
    uint8_t bd_addr[6];
    uint8_t volume;
} T_BT_EVENT_PARAM_HFP_AG_SPK_VOLUME_CHANGED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG call status.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_call_status
{
    uint8_t bd_addr[6];
    uint8_t prev_status;
    uint8_t curr_status;
} T_BT_EVENT_PARAM_HFP_AG_CALL_STATUS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter AG call indicators status request.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_indicators_status_req
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_HFP_AG_INDICATORS_STATUS_REQ;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HF battery level.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_battery_level
{
    uint8_t bd_addr[6];
    uint8_t battery_level;
} T_BT_EVENT_PARAM_HFP_AG_BATTERY_LEVEL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter codec type that HF selected.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_codec_type_selected
{
    uint8_t bd_addr[6];
    uint8_t codec_type_selected;
} T_BT_EVENT_PARAM_HFP_AG_CODEC_TYPE_SELECTED;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP AG SCO packet received.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_sco_data_ind
{
    uint8_t     bd_addr[6];
    uint16_t    handle;
    uint8_t     packet_type;
    uint16_t    length;
    uint8_t    *p_data;
    uint32_t    bt_clk;
} T_BT_EVENT_PARAM_HFP_AG_SCO_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HFP HF supported features.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hfp_ag_supported_features_ind
{
    uint8_t  bd_addr[6];
    uint32_t hf_bitmap;
} T_BT_EVENT_PARAM_HFP_AG_SUPPORTED_FEATURES_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP caller id name.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_caller_id_name
{
    uint8_t  bd_addr[6];
    uint8_t *name_ptr;
    uint16_t name_len;
} T_BT_EVENT_PARAM_PBAP_CALLER_ID_NAME;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP set phone book completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_set_phone_book_cmpl
{
    uint8_t        bd_addr[6];
    bool           result;
    BT_PBAP_PATH   path;
} T_BT_EVENT_PARAM_PBAP_SET_PHONE_BOOK_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP pull phone book completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_get_phone_book_cmpl
{
    uint8_t     bd_addr[6];
    uint8_t    *p_data;
    uint16_t    data_len;
    uint16_t    pb_size;
    uint8_t     new_missed_calls;
    bool        data_end;
} T_BT_EVENT_PARAM_PBAP_GET_PHONE_BOOK_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP pull phone book completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_get_phone_book_size_cmpl
{
    uint8_t     bd_addr[6];
    uint16_t    pb_size;
    uint8_t     new_missed_calls;
} T_BT_EVENT_PARAM_PBAP_GET_PHONE_BOOK_SIZE_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_PBAP_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_PBAP_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_sniffing_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_PBAP_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter PBAP sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_pbap_sniffing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_PBAP_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_conn_ind
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_HID_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_HID_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_HID_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID control data indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_control_data_ind
{
    uint8_t    bd_addr[6];
    uint8_t    report_type;
    uint16_t   report_id;
    uint16_t   report_size;
    uint8_t   *p_data;
} T_BT_EVENT_PARAM_HID_CONTROL_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID get report indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_get_report_ind
{
    uint8_t    bd_addr[6];
    uint8_t    report_type;
    uint16_t   report_id;
    uint16_t   report_size;
    uint8_t   *p_data;
} T_BT_EVENT_PARAM_HID_GET_REPORT_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID set report indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_set_report_ind
{
    uint8_t    bd_addr[6];
    uint8_t    report_type;
    uint16_t   report_id;
    uint16_t   report_size;
    uint8_t   *p_data;
} T_BT_EVENT_PARAM_HID_SET_REPORT_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID get protocol indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_get_protocol_ind
{
    uint8_t    bd_addr[6];
    uint8_t    proto_mode;
} T_BT_EVENT_PARAM_HID_GET_PROTOCOL_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID set protocol indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_set_protocol_ind
{
    uint8_t    bd_addr[6];
    uint8_t    proto_mode;
} T_BT_EVENT_PARAM_HID_SET_PROTOCOL_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID set idle indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_set_idle_ind
{
    uint8_t    bd_addr[6];
    uint8_t    report_status;
} T_BT_EVENT_PARAM_HID_SET_IDLE_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID interrupt data indication.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_interrupt_data_ind
{
    uint8_t    bd_addr[6];
    uint8_t    report_type;
    uint16_t   report_id;
    uint16_t   report_size;
    uint8_t   *p_data;
} T_BT_EVENT_PARAM_HID_INTERRUPT_DATA_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID sniffing connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_sniffing_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_HID_SNIFFING_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter HID sniffing disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_hid_sniffing_disconn_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_HID_SNIFFING_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP MNS connection indication.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_MAP_MNS_CONN_IND;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP MNS connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_MAP_MNS_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP MNS disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_MAP_MNS_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP MAS connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t  bd_addr[6];
    uint16_t max_pkt_len;
} T_BT_EVENT_PARAM_MAP_MAS_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP MAS disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_MAP_MAS_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP set folder completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t  bd_addr[6];
    bool     result;
} T_BT_EVENT_PARAM_MAP_SET_FOLDER_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP register message notification completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t  bd_addr[6];
    bool     result;
} T_BT_EVENT_PARAM_MAP_REG_NOTIF_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP get folder listing completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t    *p_data;
    uint16_t    data_len;
    uint16_t    folder_listing_size;
    bool        data_end;
} T_BT_EVENT_PARAM_MAP_GET_FOLDER_LISTING_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP get message listing completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t    *p_data;
    uint16_t    data_len;
    uint16_t    msg_listing_size;
    uint8_t     new_msg;
    bool        data_end;
} T_BT_EVENT_PARAM_MAP_GET_MSG_LISTING_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP get message completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t    *p_data;
    uint16_t    data_len;
    bool        data_end;
} T_BT_EVENT_PARAM_MAP_GET_MSG_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP push message completed.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t     rsp_code;
    uint8_t    *p_msg_handle;
    uint16_t    msg_handle_len;
} T_BT_EVENT_PARAM_MAP_PUSH_MSG_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter MAP remote report message notification.
 *
 * \ingroup BT_BTM
 */
typedef struct
{
    uint8_t     bd_addr[6];
    uint8_t    *p_data;
    uint16_t    data_len;
    bool        data_end;
} T_BT_EVENT_PARAM_MAP_MSG_NOTIFICATION;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SDP attribute information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sdp_attr_info
{
    uint8_t bd_addr[6];
    T_BT_SDP_ATTR_INFO info;
} T_BT_EVENT_PARAM_SDP_ATTR_INFO;

/**
 * btm.h
 *
 * \brief  BT manager event parameter DID attribute information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_did_attr_info
{
    uint8_t  bd_addr[6];
    uint16_t specific_id;
    uint16_t vendor_id;
    uint16_t vendor_src;
    uint16_t product_id;
    uint16_t version;
} T_BT_EVENT_PARAM_DID_ATTR_INFO;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SDP discover completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sdp_discov_cmpl
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_SDP_DISCOV_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter SDP discover stop.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_sdp_discov_stop
{
    uint8_t  bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_SDP_DISCOV_STOP;


/**
 * btm.h
 *
 * \brief  BT manager event parameter link key information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_link_key_info
{
    uint8_t            bd_addr[6];
    T_BT_LINK_KEY_TYPE key_type;
    uint8_t            link_key[16];
} T_BT_EVENT_PARAM_LINK_KEY_INFO;

/**
 * btm.h
 *
 * \brief  BT manager event parameter link key request.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_link_key_req
{
    uint8_t            bd_addr[6];
} T_BT_EVENT_PARAM_LINK_KEY_REQ;

/**
 * btm.h
 *
 * \brief  BT manager event parameter link pin code request.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_link_pin_code_req
{
    uint8_t            bd_addr[6];
} T_BT_EVENT_PARAM_LINK_PIN_CODE_REQ;

/**
 * btm.h
 *
 * \brief  BT manager event parameter remote roleswap status.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_remote_roleswap_status
{
    T_BTM_ROLESWAP_STATUS   status;
    T_REMOTE_SESSION_ROLE   device_role;
    uint16_t                cause;

    union
    {
        struct
        {
            uint8_t            bd_addr[6];
            T_GAP_BR_LINK_ROLE role;
            bool               authenticated;
            bool               encrypted;
        } acl;

        struct
        {
            uint8_t    bd_addr[6];
            uint16_t   handle;
        } sco;

        struct
        {
            uint8_t bd_addr[6];
            uint8_t remote_credit;
            uint16_t frame_size;
            uint8_t local_server_chann;
        } spp;

        struct
        {
            uint8_t bd_addr[6];
            uint8_t streaming_fg;
        } a2dp;

        struct
        {
            uint8_t bd_addr[6];
            uint8_t play_status;
        } avrcp;

        struct
        {
            uint8_t bd_addr[6];
            uint8_t call_status;
            uint8_t service_status;
            bool    is_hfp;
        } hfp;

        struct
        {
            uint8_t bd_addr[6];
        } pbap;

        struct
        {
            uint8_t bd_addr[6];
        } hid;

        struct
        {
            uint8_t bd_addr[6];
        } avp;

        struct
        {
            uint8_t bd_addr[6];
        } gatt;

        struct
        {
            uint8_t pre_bd_addr[6];
            uint8_t curr_bd_addr[6];
            uint8_t curr_link_role;
        } bdaddr;
    } u;
} T_BT_EVENT_PARAM_REMOTE_ROLESWAP_STATUS;

/**
 * btm.h
 *
 * \brief  BT manager event parameter remote state.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_remote_state
{
    uint8_t state;
} T_BT_EVENT_PARAM_REMOTE_STATE;

/**
 * btm.h
 *
 * \brief  BT manager event parameter remote connection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_remote_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_EVENT_PARAM_REMOTE_CONN_CMPL;

/**
 * btm.h
 *
 * \brief  BT manager event parameter remote disconnection completed.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_remote_disconn_cmpl
{
    uint8_t bd_addr[6];
    uint16_t cause;
} T_BT_EVENT_PARAM_REMOTE_DISCONN_CMPL;

/**
 * btm.h
 *
 * \brief Define BT link PER information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_link_per_info
{
    uint8_t bd_addr[6];
    uint32_t total_pkts;
    uint32_t err_pkts;
} T_BT_EVENT_PARAM_LINK_PER_INFO;

/**
 * btm.h
 *
 * \brief Define BT link RSSI information.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_link_rssi_info
{
    uint8_t bd_addr[6];
    int8_t rssi;
} T_BT_EVENT_PARAM_LINK_RSSI_INFO;

/**
 * btm.h
 *
 * \brief BT manager event parameter set pairing tx power rsp.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_set_pairing_tx_power_rsp
{
    uint16_t cause;
    uint8_t  expect_tx_power_offset;
    uint8_t  real_tx_power_offset;
} T_BT_EVENT_PARAM_PAIRING_TX_POWER_SET_RSP;

/**
 * btm.h
 *
 * \brief BT manager event parameter set link tx power rsp.
 *
 * \ingroup BT_BTM
 */
typedef struct t_bt_event_param_set_link_tx_power_rsp
{
    uint8_t  bd_addr[6];
    uint16_t cause;
    uint8_t  expect_tx_power_offset;
    uint8_t  real_tx_power_offset;
} T_BT_EVENT_PARAM_LINK_TX_POWER_SET_RSP;

/**
 * btm.h
 *
 * \brief  BT manager event.
 *
 * \ingroup BT_BTM
 */
typedef union
{
    T_BT_EVENT_PARAM_READY                          ready;
    T_BT_EVENT_PARAM_LOCAL_ADDR_CHANGED             local_addr_changed;
    T_BT_EVENT_PARAM_LOCAL_NAME_CHANGED             local_name_changed;
    T_BT_EVENT_PARAM_USER_CONFIRMATION_REQ          user_confirmation_req;
    T_BT_EVENT_PARAM_DEVICE_MODE_RSP                device_mode_rsp;
    T_BT_EVENT_PARAM_INQUIRY_RESULT                 inquiry_result;
    T_BT_EVENT_PARAM_INQUIRY_RSP                    inquiry_rsp;
    T_BT_EVENT_PARAM_INQUIRY_CMPL                   inquiry_cmpl;
    T_BT_EVENT_PARAM_INQUIRY_CANCEL_RSP             inquiry_cancel_rsp;

    T_BT_EVENT_PARAM_SCO_CONN_IND                   sco_conn_ind;
    T_BT_EVENT_PARAM_SCO_CONN_RSP                   sco_conn_rsp;
    T_BT_EVENT_PARAM_SCO_CONN_CMPL                  sco_conn_cmpl;
    T_BT_EVENT_PARAM_SCO_DISCONNECTED               sco_disconnected;
    T_BT_EVENT_PARAM_SCO_LINK_SWITCH                sco_link_switch;
    T_BT_EVENT_PARAM_SCO_SNIFFING_CONN_CMPL         sco_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_SCO_SNIFFING_STARTED           sco_sniffing_started;
    T_BT_EVENT_PARAM_SCO_SNIFFING_STOPPED           sco_sniffing_stopped;
    T_BT_EVENT_PARAM_SCO_SNIFFING_DISCONN_CMPL      sco_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_ACL_LINK_STATUS                acl_link_status;
    T_BT_EVENT_PARAM_ACL_REMOTE_NAME_RSP            acl_remote_name_rsp;
    T_BT_EVENT_PARAM_ACL_ROLE_SWITCH_FAIL           acl_role_switch_fail;
    T_BT_EVENT_PARAM_ACL_CONN_IND                   acl_conn_ind;
    T_BT_EVENT_PARAM_ACL_SNIFFING_CONN_CMPL         acl_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_ACL_SNIFFING_DISCONN_CMPL      acl_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_LINK_KEY_INFO                  link_key_info;
    T_BT_EVENT_PARAM_LINK_KEY_REQ                   link_key_req;
    T_BT_EVENT_PARAM_LINK_PIN_CODE_REQ              link_pin_code_req;

    T_BT_EVENT_PARAM_SPP_CONN_IND                   spp_conn_ind;
    T_BT_EVENT_PARAM_SPP_CONN_CMPL                  spp_conn_cmpl;
    T_BT_EVENT_PARAM_SPP_DISCONN_CMPL               spp_disconn_cmpl;
    T_BT_EVENT_PARAM_SPP_CREDIT_RCVD                spp_credit_rcvd;
    T_BT_EVENT_PARAM_SPP_DATA_IND                   spp_data_ind;
    T_BT_EVENT_PARAM_SPP_DATA_RSP                   spp_data_rsp;
    T_BT_EVENT_PARAM_SPP_SNIFFING_CONN_CMPL         spp_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_SPP_SNIFFING_DISCONN_CMPL      spp_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_A2DP_CONN_IND                  a2dp_conn_ind;
    T_BT_EVENT_PARAM_A2DP_CONN_CMPL                 a2dp_conn_cmpl;
    T_BT_EVENT_PARAM_A2DP_DISCONN_CMPL              a2dp_disconn_cmpl;
    T_BT_EVENT_PARAM_A2DP_CONFIG_CMPL               a2dp_config_cmpl;
    T_BT_EVENT_PARAM_A2DP_STREAM_OPEN               a2dp_stream_open;
    T_BT_EVENT_PARAM_A2DP_STREAM_START_IND          a2dp_stream_start_ind;
    T_BT_EVENT_PARAM_A2DP_STREAM_START_RSP          a2dp_stream_start_rsp;
    T_BT_EVENT_PARAM_A2DP_STREAM_DATA_IND           a2dp_stream_data_ind;
    T_BT_EVENT_PARAM_A2DP_STREAM_DATA_RSP           a2dp_stream_data_rsp;
    T_BT_EVENT_PARAM_A2DP_STREAM_STOP               a2dp_stream_stop;
    T_BT_EVENT_PARAM_A2DP_STREAM_CLOSE              a2dp_stream_close;
    T_BT_EVENT_PARAM_A2DP_SNIFFING_CONN_CMPL        a2dp_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_A2DP_SNIFFING_START_IND        a2dp_sniffing_start_ind;
    T_BT_EVENT_PARAM_A2DP_SNIFFING_STARTED          a2dp_sniffing_started;
    T_BT_EVENT_PARAM_A2DP_SNIFFING_STOPPED          a2dp_sniffing_stopped;
    T_BT_EVENT_PARAM_A2DP_SNIFFING_DISCONN_CMPL     a2dp_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_AVRCP_CONN_IND                 avrcp_conn_ind;
    T_BT_EVENT_PARAM_AVRCP_CONN_CMPL                avrcp_conn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_DISCONN_CMPL             avrcp_disconn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_ELEM_ATTR                avrcp_elem_attr;
    T_BT_EVENT_PARAM_AVRCP_PLAY_STATUS_RSP          avrcp_play_status_rsp;
    T_BT_EVENT_PARAM_AVRCP_VOLUME_CHANGED           avrcp_volume_changed;
    T_BT_EVENT_PARAM_AVRCP_VOLUME_UP                avrcp_volume_up;
    T_BT_EVENT_PARAM_AVRCP_VOLUME_DOWN              avrcp_volume_down;
    T_BT_EVENT_PARAM_AVRCP_REG_VOLUME_CHANGED       avrcp_reg_volume_changed;
    T_BT_EVENT_PARAM_AVRCP_TRACK_CHANGED            avrcp_track_changed;
    T_BT_EVENT_PARAM_AVRCP_PLAY_STATUS_CHANGED      avrcp_play_status_changed;
    T_BT_EVENT_PARAM_AVRCP_SNIFFING_CONN_CMPL       avrcp_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_SNIFFING_DISCONN_CMPL    avrcp_sniffing_disconn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_APP_SETTING_CHANGED      avrcp_app_setting_changed;
    T_BT_EVENT_PARAM_AVRCP_PLAYING_CONTENT_CHANGED  avrcp_playing_content_changed;
    T_BT_EVENT_PARAM_AVRCP_AVAILABLE_PLAYER_CHANGED avrcp_available_player_changed;
    T_BT_EVENT_PARAM_AVRCP_ADDRESSED_PLAYER_CHANGED avrcp_addressed_player_changed;
    T_BT_EVENT_PARAM_AVRCP_UID_CHANGED              avrcp_uid_changed;
    T_BT_EVENT_PARAM_AVRCP_APP_SETTING_ATTRS        avrcp_app_setting_attrs;
    T_BT_EVENT_PARAM_AVRCP_APP_SETTING_VALUES       avrcp_app_setting_values;
    T_BT_EVENT_PARAM_AVRCP_APP_SETTING              avrcp_app_setting;
    T_BT_EVENT_PARAM_AVRCP_ADDRESSED_PLAYER         avrcp_addressed_player;
    T_BT_EVENT_PARAM_AVRCP_PLAY_ITEM                avrcp_play_item;
    T_BT_EVENT_PARAM_AVRCP_FOLDER_ITEMS             avrcp_folder_items;
    T_BT_EVENT_PARAM_AVRCP_ITEM_ATTR                avrcp_item_attr;
    T_BT_EVENT_PARAM_AVRCP_SEARCH                   avrcp_search;
    T_BT_EVENT_PARAM_AVRCP_BROWSED_PLAYER           avrcp_browsed_player;
    T_BT_EVENT_PARAM_AVRCP_CHANGE_PATH              avrcp_change_path;
    T_BT_EVENT_PARAM_AVRCP_BROWSING_CONN_IND        avrcp_browsing_conn_ind;
    T_BT_EVENT_PARAM_AVRCP_BROWSING_CONN_CMPL       avrcp_browsing_conn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_BROWSING_DISCONN_CMPL    avrcp_browsing_disconn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_COVER_ART_CONN_CMPL      avrcp_cover_art_conn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_COVER_ART_DISCONN_CMPL   avrcp_cover_art_disconn_cmpl;
    T_BT_EVENT_PARAM_AVRCP_COVER_ART_DATA_IND       avrcp_cover_art_data_ind;

    T_BT_EVENT_PARAM_IAP_CONN_IND                   iap_conn_ind;
    T_BT_EVENT_PARAM_IAP_CONN_CMPL                  iap_conn_cmpl;
    T_BT_EVENT_PARAM_IAP_DISCONN_CMPL               iap_disconn_cmpl;
    T_BT_EVENT_PARAM_IAP_AUTHEN_CMPL                iap_authen_cmpl;
    T_BT_EVENT_PARAM_IAP_DATA_SESSION_OPEN          iap_data_session_open;
    T_BT_EVENT_PARAM_IAP_DATA_SESSION_CLOSE         iap_data_session_close;
    T_BT_EVENT_PARAM_IAP_DATA_SENT                  iap_data_sent;
    T_BT_EVENT_PARAM_IAP_DATA_IND                   iap_data_ind;
    T_BT_EVENT_PARAM_IAP_IDENTITY_INFO_REQ          iap_identity_info_req;
    T_BT_EVENT_PARAM_IAP_RESET                      iap_reset;
    T_BT_EVENT_PARAM_IAP_CTRL_MSG_IND               iap_ctrl_msg_ind;
    T_BT_EVENT_PARAM_IAP_SNIFFING_CONN_CMPL         iap_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_IAP_SNIFFING_DISCONN_CMPL      iap_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_HFP_CONN_IND                   hfp_conn_ind;
    T_BT_EVENT_PARAM_HFP_CONN_CMPL                  hfp_conn_cmpl;
    T_BT_EVENT_PARAM_HFP_DISCONN_CMPL               hfp_disconn_cmpl;
    T_BT_EVENT_PARAM_HFP_SPK_VOLUME_CHANGED         hfp_spk_volume_changed;
    T_BT_EVENT_PARAM_HFP_MIC_VOLUME_CHANGED         hfp_mic_volume_changed;
    T_BT_EVENT_PARAM_HFP_CALL_STATUS                hfp_call_status;
    T_BT_EVENT_PARAM_HFP_SERVICE_STATUS             hfp_service_status;

    T_BT_EVENT_PARAM_HFP_AG_CONN_IND                hfp_ag_conn_ind;
    T_BT_EVENT_PARAM_HFP_AG_CONN_CMPL               hfp_ag_conn_cmpl;
    T_BT_EVENT_PARAM_HFP_AG_DISCONN_CMPL            hfp_ag_disconn_cmpl;
    T_BT_EVENT_PARAM_HFP_AG_MIC_VOLUME_CHANGED      hfp_ag_mic_volume_changed;
    T_BT_EVENT_PARAM_HFP_AG_SPK_VOLUME_CHANGED      hfp_ag_spk_volume_changed;
    T_BT_EVENT_PARAM_HFP_AG_CALL_STATUS             hfp_ag_call_status;
    T_BT_EVENT_PARAM_HFP_AG_INDICATORS_STATUS_REQ   hfp_ag_indicators_status_req;
    T_BT_EVENT_PARAM_HFP_AG_BATTERY_LEVEL           hfp_ag_battery_level;
    T_BT_EVENT_PARAM_HFP_AG_CODEC_TYPE_SELECTED     hfp_ag_codec_type_selected;
    T_BT_EVENT_PARAM_HFP_AG_SCO_DATA_IND            hfp_ag_sco_data_ind;
    T_BT_EVENT_PARAM_HFP_AG_SUPPORTED_FEATURES_IND  hfp_ag_supported_features_ind;

    T_BT_EVENT_PARAM_HFP_CALLER_ID_IND              hfp_caller_id_ind;
    T_BT_EVENT_PARAM_HFP_NETWORK_OPERATOR_IND       hfp_network_operator_ind;
    T_BT_EVENT_PARAM_HFP_SUBSCRIBER_NUMBER_IND      hfp_subscriber_number_ind;
    T_BT_EVENT_PARAM_HFP_CURRENT_CALL_LIST_IND      hfp_current_call_list_ind;
    T_BT_EVENT_PARAM_HFP_CALL_WAITING_IND           hfp_call_waiting_ind;
    T_BT_EVENT_PARAM_HFP_SIGNAL_IND                 hfp_signal_ind;
    T_BT_EVENT_PARAM_HFP_ROAM_IND                   hfp_roam_ind;
    T_BT_EVENT_PARAM_HFP_BATTERY_IND                hfp_battery_ind;
    T_BT_EVENT_PARAM_HFP_SUPPORTED_FEATURES_IND     hfp_supported_features_ind;
    T_BT_EVENT_PARAM_HFP_UNKNOW_AT_EVENT            hfp_unknow_at_event;
    T_BT_EVENT_PARAM_HFP_VENDOR_AT_RESULT           hfp_vendor_at_result;
    T_BT_EVENT_PARAM_HFP_RING_ALERT                 hfp_ring_alert;
    T_BT_EVENT_PARAM_HFP_SNIFFING_CONN_CMPL         hfp_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_HFP_SNIFFING_DISCONN_CMPL      hfp_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_PBAP_CONN_CMPL                 pbap_conn_cmpl;
    T_BT_EVENT_PARAM_PBAP_DISCONN_CMPL              pbap_disconn_cmpl;
    T_BT_EVENT_PARAM_PBAP_CALLER_ID_NAME            pbap_caller_id_name;
    T_BT_EVENT_PARAM_PBAP_SET_PHONE_BOOK_CMPL       pbap_set_phone_book_cmpl;
    T_BT_EVENT_PARAM_PBAP_GET_PHONE_BOOK_CMPL       pbap_get_phone_book_cmpl;
    T_BT_EVENT_PARAM_PBAP_GET_PHONE_BOOK_SIZE_CMPL  pbap_get_phone_book_size_cmpl;
    T_BT_EVENT_PARAM_PBAP_SNIFFING_CONN_CMPL        pbap_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_PBAP_SNIFFING_DISCONN_CMPL     pbap_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_HID_CONN_IND                   hid_conn_ind;
    T_BT_EVENT_PARAM_HID_CONN_CMPL                  hid_conn_cmpl;
    T_BT_EVENT_PARAM_HID_DISCONN_CMPL               hid_disconn_cmpl;
    T_BT_EVENT_PARAM_HID_CONTROL_DATA_IND           hid_control_data_ind;
    T_BT_EVENT_PARAM_HID_GET_REPORT_IND             hid_get_report_ind;
    T_BT_EVENT_PARAM_HID_SET_REPORT_IND             hid_set_report_ind;
    T_BT_EVENT_PARAM_HID_GET_PROTOCOL_IND           hid_get_protocol_ind;
    T_BT_EVENT_PARAM_HID_SET_PROTOCOL_IND           hid_set_protocol_ind;
    T_BT_EVENT_PARAM_HID_SET_IDLE_IND               hid_set_idle_ind;
    T_BT_EVENT_PARAM_HID_INTERRUPT_DATA_IND         hid_interrupt_data_ind;
    T_BT_EVENT_PARAM_HID_SNIFFING_CONN_CMPL         hid_sniffing_conn_cmpl;
    T_BT_EVENT_PARAM_HID_SNIFFING_DISCONN_CMPL      hid_sniffing_disconn_cmpl;

    T_BT_EVENT_PARAM_MAP_MNS_CONN_IND               bt_map_mns_conn_ind;
    T_BT_EVENT_PARAM_MAP_MNS_CONN_CMPL              bt_map_mns_conn_cmpl;
    T_BT_EVENT_PARAM_MAP_MNS_DISCONN_CMPL           bt_map_mns_disconn_cmpl;
    T_BT_EVENT_PARAM_MAP_MAS_CONN_CMPL              bt_map_mas_conn_cmpl;
    T_BT_EVENT_PARAM_MAP_MAS_DISCONN_CMPL           bt_map_mas_disconn_cmpl;
    T_BT_EVENT_PARAM_MAP_SET_FOLDER_CMPL            bt_map_set_folder_cmpl;
    T_BT_EVENT_PARAM_MAP_REG_NOTIF_CMPL             bt_map_reg_notif_cmpl;
    T_BT_EVENT_PARAM_MAP_GET_FOLDER_LISTING_CMPL    bt_map_get_folder_listing_cmpl;
    T_BT_EVENT_PARAM_MAP_GET_MSG_LISTING_CMPL       bt_map_get_msg_listing_cmpl;
    T_BT_EVENT_PARAM_MAP_GET_MSG_CMPL               bt_map_get_msg_cmpl;
    T_BT_EVENT_PARAM_MAP_PUSH_MSG_CMPL              bt_map_push_msg_cmpl;
    T_BT_EVENT_PARAM_MAP_MSG_NOTIFICATION           bt_map_msg_notification;

    T_BT_EVENT_PARAM_SDP_ATTR_INFO                  sdp_attr_info;
    T_BT_EVENT_PARAM_DID_ATTR_INFO                  did_attr_info;
    T_BT_EVENT_PARAM_SDP_DISCOV_CMPL                sdp_discov_cmpl;
    T_BT_EVENT_PARAM_SDP_DISCOV_STOP                sdp_discov_stop;

    T_BT_EVENT_PARAM_REMOTE_CONN_CMPL               remote_conn_cmpl;
    T_BT_EVENT_PARAM_REMOTE_DISCONN_CMPL            remote_disconn_cmpl;
    T_BT_EVENT_PARAM_REMOTE_ROLESWAP_STATUS         remote_roleswap_status;
    T_BT_EVENT_PARAM_REMOTE_STATE                   remote_state;

    T_BT_EVENT_PARAM_LINK_PER_INFO                  link_per_info;
    T_BT_EVENT_PARAM_LINK_RSSI_INFO                 link_rssi_info;
    T_BT_EVENT_PARAM_PAIRING_TX_POWER_SET_RSP       pairing_tx_power_set_rsp;
    T_BT_EVENT_PARAM_LINK_TX_POWER_SET_RSP          link_tx_power_set_rsp;
} T_BT_EVENT_PARAM;

/**
 * btm.h
 *
 * \brief  BT manager event callback prototype.
 *
 * \param[in] event_type   BT manager event type.
 * \param[in] event_buf    Event buffer address.
 * \param[in] buf_len      Event buffer length.
 *
 * \ingroup  BT_BTM
 */
typedef void (*P_BT_MGR_CBACK)(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len);

/**
 * btm.h
 *
 * \brief   BT manager event callback register.
 *
 * \param[in] cback       Callback function \ref P_BT_MGR_CBACK
 *
 * \return                The status of BT manager event callback register.
 * \retval    true        BT manager event callback was registered successfully.
 * \retval    false       BT manager event callback was failed to register.
 *
 * \ingroup  BT_BTM
 */
bool bt_mgr_cback_register(P_BT_MGR_CBACK cback);

/**
 * btm.h
 *
 * \brief   BT manager event callback unregister.
 *
 * \param[in] cback       Callback function \ref P_BT_MGR_CBACK
 *
 * \return                The status of BT manager event callback unregister.
 * \retval    true        BT manager event callback was unregistered successfully.
 * \retval    false       BT manager event callback was failed to unregister.
 *
 * \ingroup  BT_BTM
 */
bool bt_mgr_cback_unregister(P_BT_MGR_CBACK cback);

/**
 * btm.h
 *
 * \brief   Initialize bluetooth manager.
 *
 * \return The status of initializing bluetooth manager.
 * \retval true     Bluetooth manager was initialized successfully.
 * \retval false    Bluetooth manager was failed to initialize.
 *
 * \ingroup BT_BTM
 */
bool bt_mgr_init(void);

/**
 * btm.h
 *
 * \brief   Set local BT device name.
 *
 * \param[in] p_name    Local BT device name.
 * \param[in] len       Local BT device name length.
 *
 * \return The status of setting local BT device name.
 * \retval true     Local BT device name was set successfully.
 * \retval false    Local BT device name was failed to set.
 *
 * \ingroup BT_BTM
 */
bool bt_local_name_set(uint8_t *p_name, uint8_t len);

/**
 * btm.h
 *
 * \brief   Send a request to get remote device name.
 *
 * \param[in] bd_addr    Remote BT address.
 *
 * \return The status of sending remote device name request.
 * \retval true     Remote device name request was sent successfully.
 * \retval false    Remote device name request was failed to sent.
 *
 * \ingroup BT_BTM
 */
bool bt_remote_name_req(uint8_t *bd_addr);

/**
 * btm.h
 *
 * \brief   Accept acl connection request from remote device.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] role       Expected acl link role.
 * \arg    BT_LINK_ROLE_MASTER     Perform a role switch to become master of the link.
 * \arg    BT_LINK_ROLE_SLAVE      Stay slave role of the link.
 *
 * \return The status of sending accept connection confirmation.
 * \retval true     Accpet acl connection confirmation was sent successfully.
 * \retval false    Accept acl connection confirmation was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_acl_conn_accept(uint8_t *bd_addr, T_BT_LINK_ROLE role);

/**
 * btm.h
 *
 * \brief   Reject acl connection request from remote device.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] reason     Reject reason.
 *
 * \return The status of sending reject connection confirmation.
 * \retval true     Reject acl connection confirmation was sent successfully.
 * \retval false    Reject acl connection confirmation was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_acl_conn_reject(uint8_t *bd_addr, T_BT_ACL_REJECT_REASON reason);

/**
 * btm.h
 *
 * \brief   Send confirmation for link key request during authentication.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] found      Whether link key for the address is found.
 * \param[in] type       Link key type.
 * \param[in] p_key      Link key value.
 *
 * \return The status of sending link key request confirmation.
 * \retval true     Link key request confirmation was sent successfully.
 * \retval false    Link key request confirmation was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_link_key_cfm(uint8_t *bd_addr, bool found,
                     T_BT_LINK_KEY_TYPE type, uint8_t *p_key);

/**
 * btm.h
 *
 * \brief   Send confirmation for link pin code request during authentication.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] pin_code   Pin code.
 * \param[in] len        Pin code length.
 * \param[in] accept     Whether pin code for the address is found.
 *
 * \return The status of sending pin code request confirmation.
 * \retval true     Pin code request confirmation was sent successfully.
 * \retval false    Pin code request confirmation was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_link_pin_code_cfm(uint8_t *bd_addr, uint8_t *pin_code, uint8_t len, bool accept);

/**
 * btm.h
 *
 * \brief   Send a request to set BT link encryption.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] enable     Enable/disable encryption.
 *
 * \return The status of setting BT link encryption.
 * \retval true     BT link encryption setting request was sent successfully.
 * \retval false    BT link encryption setting request was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_link_encryption_set(uint8_t *bd_addr, bool enable);

/**
 * btm.h
 *
 * \brief   Send a SCO connection confirmation.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] accept     Accept or reject the SCO connection indication.
 * \arg    true     Accept the SCO connection indication.
 * \arg    false    Reject the SCO connection indication.
 *
 * \return The status of sending SCO connection confirmation.
 * \retval true     SCO connection confirmation was sent successfully.
 * \retval false    SCO connection confirmation was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_sco_conn_cfm(uint8_t *bd_addr, bool accept);

/**
 * btm.h
 *
 * \brief   Send SCO buffer after SCO connection setup.
 *
 * \param[in] bd_addr    Remote BT address.
 * \param[in] seq_num    SCO data seq number.
 * \param[in] buf        SCO buffer to send.
 * \param[in] len        SCO buffer length.
 *
 * \return The status of sending SCO connection confirmation.
 * \retval true     SCO connection confirmation was sent successfully.
 * \retval false    SCO connection confirmation was failed to send.
 *
 * \ingroup BT_BTM
 */
bool bt_sco_data_send(uint8_t *bd_addr,  uint8_t seq_num, uint8_t *p_data, uint8_t len);

/**
 * btm.h
 *
 * \brief   Switch BT link role to master or slave.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] set_master Role switched to master or slave.
 *
 * \return The status of switching BT link role.
 * \retval true     BT link role switch was started successfully.
 * \retval false    BT link role switch was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_link_role_switch(uint8_t *bd_addr, bool set_master);

/**
 * btm.h
 *
 * \brief   Set BT device mode.
 *
 * \param[in] device_mode   BT device mode defined in \ref T_BT_DEVICE_MODE.
 *
 * \return The status of setting BT device mode.
 * \retval true     BT device mode was set successfully.
 * \retval false    BT device mode was failed to set.
 *
 * \ingroup BT_BTM
 */
bool bt_device_mode_set(T_BT_DEVICE_MODE device_mode);

/**
 * btm.h
 *
 * \brief   Get BT device mode.
 *
 * \return  BT device mode defined in \ref T_BT_DEVICE_MODE.
 *
 * \ingroup BT_BTM
 */
T_BT_DEVICE_MODE bt_device_mode_get(void);

/**
 * btm.h
 *
 * \brief   Set BT acl packet type.
 *
 * \param[in] bd_addr      Bluetooth device address.
 * \param[in] acl_pkt_type Link packet type defined in \ref T_BT_ACL_PKT_TYPE.
 *
 * \return The status of set acl packet type.
 * \retval true     BT acl packet was set successfully.
 * \retval false    BT acl packet was failed to set.
 *
 * \ingroup BT_BTM
 */
bool bt_acl_pkt_type_set(uint8_t *bd_addr, T_BT_ACL_PKT_TYPE acl_pkt_type);

/**
 * btm.h
 *
 * \brief   Set BT link QoS.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] type       QoS type defined in \ref T_BT_QOS_TYPE.
 * \param[in] tpoll      The poll interval in slot unit.
 *
 * \return The status of set BT link QoS.
 * \retval true     BT link QoS was set successfully.
 * \retval false    BT link QoS was failed to set.
 *
 * \ingroup BT_BTM
 */
bool bt_link_qos_set(uint8_t *bd_addr, T_BT_QOS_TYPE type, uint32_t tpoll);

/**
 * btm.h
 *
 * \brief   Enable BT sniff mode.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] interval   Sniff mode interval in milliseconds.
 * \arg \c 0       Use default or last configured interval.
 * \arg \c 2-40900 Valid interval range.
 *
 * \return The status of enabling BT sniff mode.
 * \retval true     BT sniff mode was enabled successfully.
 * \retval false    BT sniff mode was failed to enable.
 *
 * \ingroup BT_BTM
 */
bool bt_sniff_mode_enable(uint8_t *bd_addr, uint16_t interval);

/**
 * btm.h
 *
 * \brief   Disable BT sniff mode.
 *
 * \param[in] bd_addr    Bluetooth device address.
 *
 * \return The status of disabling BT sniff mode.
 * \retval true     BT sniff mode was disabled successfully.
 * \retval false    BT sniff mode was failed to disable.
 *
 * \ingroup BT_BTM
 */
bool bt_sniff_mode_disable(uint8_t *bd_addr);

/**
 * btm.h
 *
 * \brief   Enable/disable PER report on BT link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] enable     True to enable PER report and false to disable.
 * \param[in] period     Report period in ms.
 *
 * \return The status of enable/disable PER report.
 * \retval true     Enable/disable PER report was started successfully.
 * \retval false    Enable/disable PER report was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_link_per_report(uint8_t *bd_addr, bool enable, uint16_t period);

/**
 * btm.h
 *
 * \brief   Enable/disable RSSI report on BT link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] enable     True to enable RSSI report and false to disable.
 * \param[in] period     Report period in ms.
 *
 * \return The status of enable/disable RSSI report.
 * \retval true     Enable/disable RSSI report was started successfully.
 * \retval false    Enable/disable RSSI report was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_link_rssi_report(uint8_t *bd_addr, bool enable, uint16_t period);

/**
 * btm.h
 *
 * \brief   Set pairing tx power.
 *
 * \param[in] offset_level     The offset level to be modified, range from -2 to 2.
 *
 * \return The status of control tx power.
 * \retval true     Tx power level was set successfully.
 * \retval false    Tx power level was failed to set.
 *
 * \ingroup BT_BTM
 */
bool bt_pairing_tx_power_set(int8_t offset_level);

/**
 * btm.h
 *
 * \brief   Set tx power on BT link.
 *
 * \param[in] bd_addr          Bluetooth device address.
 * \param[in] offset_level     The offset level to be modified, range from -2 to 2.
 *
 * \return The status of control tx power.
 * \retval true     Tx power level was set successfully.
 * \retval false    Tx power level was failed to set.
 *
 * \ingroup BT_BTM
 */
bool bt_link_tx_power_set(uint8_t *bd_addr, int8_t offset_level);

/**
 * btm.h
 *
 * \brief   Connect the sniffing link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 *
 * \return The status of starting the sniffing link connection.
 * \retval true     Sniffing link connection was started successfully.
 * \retval false    Sniffing link connection was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_connect(uint8_t *bd_addr);

/**
 * btm.h
 *
 * \brief   Disconnect the sniffing link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 *
 * \return The status of starting the sniffing link disconnection.
 * \retval true     Sniffing link disconnection was started successfully.
 * \retval false    Sniffing link disconnection was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_disconnect(uint8_t *bd_addr);

/**
 * btm.h
 *
 * \brief   Start the audio sniffing link.
 *
 * \param[in] bd_addr       Bluetooth device address.
 * \param[in] interval      Audio sniffing parameter interval.
 * \param[in] flush_tout    Audio sniffing parameter flush timeout.
 * \param[in] rsvd_slot     Audio sniffing parameter reserved slots.
 * \param[in] idle_slot     Audio sniffing parameter number of idle slots in a period.
 * \param[in] idle_skip     Audio sniffing parameter number of periods to skip before allocating idle bandwidth in a period.
 *
 * \return The status of starting the audio sniffing link.
 * \retval true     Audio sniffing link was started successfully.
 * \retval false    Audio sniffing link was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_audio_start(uint8_t *bd_addr, uint16_t interval, uint16_t flush_tout,
                                  uint8_t rsvd_slot, uint8_t idle_slot, uint8_t idle_skip);

/**
 * btm.h
 *
 * \brief   Stop the audio sniffing link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] reason     Sniffing link stop reason.
 *
 * \return The status of stopping audio sniffing link.
 * \retval true     Audio sniffing link stopping was started successfully.
 * \retval false    Audio sniffing link stopping was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_audio_stop(uint8_t *bd_addr, uint8_t reason);

/**
 * btm.h
 *
 * \brief   Configure audio sniffing link parameters.
 *
 * \param[in] bd_addr       Bluetooth device address.
 * \param[in] interval      Audio sniffing parameter interval.
 * \param[in] flush_tout    Audio sniffing parameter flush timeout.
 * \param[in] rsvd_slot     Audio sniffing parameter reserved slots.
 * \param[in] idle_slot     Audio sniffing parameter number of idle slots in a period.
 * \param[in] idle_skip     Audio sniffing parameter number of periods to skip before allocating idle bandwidth in a period.
 *
 * \return The status of configuring audio sniffing link.
 * \retval true     Audio sniffing link configuration was started successfully.
 * \retval false    Audio sniffing link configuration was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_audio_cfg(uint8_t *bd_addr, uint16_t interval, uint16_t flush_tout,
                                uint8_t rsvd_slot, uint8_t idle_slot, uint8_t idle_skip);

/**
 * btm.h
 *
 * \brief   Start voice sniffing link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 *
 * \return The status of starting voice sniffing link.
 * \retval true     Voice sniffing link was started successfully.
 * \retval false    Voice sniffing link was failed to start.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_voice_start(uint8_t *bd_addr);

/**
 * btm.h
 *
 * \brief   Configure voice sniffing link parameters.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] dup_num    Voice sniffing parameter duplicate number.
 *
 * \return The status of Configure voice sniffing link.
 * \retval true     Voice sniffing link was configured successfully.
 * \retval false    Voice sniffing link was failed to configure.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_voice_cfg(uint8_t *bd_addr, uint8_t dup_num);

/**
 * btm.h
 *
 * \brief   Stop voice sniffing link.
 *
 * \param[in] bd_addr    Bluetooth device address.
 * \param[in] reason     Sniffing link stop reason.
 *
 * \return The status of stopping voice sniffing link.
 * \retval true     Voice sniffing link stopping was started successfully.
 * \retval false    Voice sniffing link stopping was failed to started.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_link_voice_stop(uint8_t *bd_addr, uint8_t reason);

/**
 * btm.h
 *
 * \brief   Set a2dp packet nack number.
 *
 * \param[in] enable     enable or disable.
 *
 * \return The resault of this function.
 * \retval true     set successfully.
 * \retval false    set failed.
 *
 * \ingroup BT_BTM
 */
bool bt_sniffing_set_a2dp_dup_num(bool enable, bool qucik_flush);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#endif /* _BTM_H_ */
