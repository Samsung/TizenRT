/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_NETWORK_BLUETOOTH_TYPE_H__
#define __TIZEN_NETWORK_BLUETOOTH_TYPE_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "tizen_error.h"

 #ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file bluetooth_type.h
 * @brief API to control the Bluetooth adapter, devices and communications.
 * @ingroup     CAPI_NETWORK_BLUETOOTH_TYPE_MODULE
 */


/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Enumeration of PBAP fields.
 * @since_tizen 3.0
 */
typedef enum {
	BT_PBAP_FIELD_ALL = (0xFFFFFFFFU), /**< All field */
	BT_PBAP_FIELD_VERSION = (1U << 0), /**< vCard Version */
	BT_PBAP_FIELD_FN = (1U << 1), /**< Formatted Name */
	BT_PBAP_FIELD_N = (1U << 2), /**< Structured Presentation of Name */
	BT_PBAP_FIELD_PHOTO = (1U << 3), /**< Associated Image or Photo */
	BT_PBAP_FIELD_BDAY = (1U << 4), /**< Birthday */
	BT_PBAP_FIELD_ADR = (1U << 5), /**< Delivery Address */
	BT_PBAP_FIELD_LABEL = (1U << 6), /**< Delivery */
	BT_PBAP_FIELD_TEL = (1U << 7), /**< Telephone Number */
	BT_PBAP_FIELD_EMAIL = (1U << 8), /**< Electronic Mail Address */
	BT_PBAP_FIELD_MAILER = (1U << 9), /**< Electronic Mail */
	BT_PBAP_FIELD_TZ = (1U << 10), /**< Time Zone */
	BT_PBAP_FIELD_GEO = (1U << 11), /**< Geographic Position */
	BT_PBAP_FIELD_TITLE = (1U << 12), /**< Job */
	BT_PBAP_FIELD_ROLE = (1U << 13), /**< Role within the Organization */
	BT_PBAP_FIELD_LOGO = (1U << 14), /**< Organization Logo */
	BT_PBAP_FIELD_AGENT = (1U << 15), /**< vCard of Person Representing */
	BT_PBAP_FIELD_ORG = (1U << 16), /**< Name of Organization */
	BT_PBAP_FIELD_NOTE = (1U << 17), /**< Comments */
	BT_PBAP_FIELD_REV = (1U << 18), /**< Revision */
	BT_PBAP_FIELD_SOUND = (1U << 19), /**< Pronunciation of Name */
	BT_PBAP_FIELD_URL = (1U << 20), /**< Uniform Resource Locator */
	BT_PBAP_FIELD_UID = (1U << 21), /**< Unique ID */
	BT_PBAP_FIELD_KEY = (1U << 22), /**< Public Encryption Key */
	BT_PBAP_FIELD_NICKNAME = (1U << 23), /**< Nickname */
	BT_PBAP_FIELD_CATEGORIES = (1U << 24), /**< Categories */
	BT_PBAP_FIELD_PROID = (1U << 25), /**< Product ID */
	BT_PBAP_FIELD_CLASS = (1U << 26), /**< Class information */
	BT_PBAP_FIELD_SORT_STRING = (1U << 27), /**< String used for sorting operations */
	BT_PBAP_FIELD_X_IRMC_CALL_DATETIME = (1U << 28), /**< Time stamp */
} bt_pbap_field_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 * @brief Enumerations of Bluetooth error codes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful*/
	BT_ERROR_CANCELLED = TIZEN_ERROR_CANCELED, /**< Operation cancelled */
	BT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	BT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	BT_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**< Device or resource busy */
	BT_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Timeout error */
	BT_ERROR_NOW_IN_PROGRESS = TIZEN_ERROR_NOW_IN_PROGRESS, /**< Operation now in progress */
	BT_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< BT is Not Supported */
	BT_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	BT_ERROR_QUOTA_EXCEEDED = TIZEN_ERROR_QUOTA_EXCEEDED, /**< Quota exceeded */
	BT_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA, /**< No data available */
	BT_ERROR_DEVICE_POLICY_RESTRICTION = TIZEN_ERROR_DEVICE_POLICY_RESTRICTION, /**< Device policy restriction (Since 3.0) */
	BT_ERROR_NOT_INITIALIZED = TIZEN_ERROR_BLUETOOTH|0x0101, /**< Local adapter not initialized */
	BT_ERROR_NOT_ENABLED = TIZEN_ERROR_BLUETOOTH|0x0102, /**< Local adapter not enabled */
	BT_ERROR_ALREADY_DONE = TIZEN_ERROR_BLUETOOTH|0x0103, /**< Operation already done  */
	BT_ERROR_OPERATION_FAILED = TIZEN_ERROR_BLUETOOTH|0x0104, /**< Operation failed */
	BT_ERROR_NOT_IN_PROGRESS = TIZEN_ERROR_BLUETOOTH|0x0105, /**< Operation not in progress */
	BT_ERROR_REMOTE_DEVICE_NOT_BONDED = TIZEN_ERROR_BLUETOOTH|0x0106, /**< Remote device not bonded */
	BT_ERROR_AUTH_REJECTED = TIZEN_ERROR_BLUETOOTH|0x0107, /**< Authentication rejected */
	BT_ERROR_AUTH_FAILED = TIZEN_ERROR_BLUETOOTH|0x0108, /**< Authentication failed */
	BT_ERROR_REMOTE_DEVICE_NOT_FOUND = TIZEN_ERROR_BLUETOOTH|0x0109, /**< Remote device not found */
	BT_ERROR_SERVICE_SEARCH_FAILED = TIZEN_ERROR_BLUETOOTH|0x010A, /**< Service search failed */
	BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED = TIZEN_ERROR_BLUETOOTH|0x010B, /**< Remote device is not connected */
	BT_ERROR_AGAIN = TIZEN_ERROR_BLUETOOTH|0x010C, /**< Resource temporarily unavailable */
	BT_ERROR_SERVICE_NOT_FOUND = TIZEN_ERROR_BLUETOOTH|0x010D, /**< Service Not Found */
	BT_ERROR_AUTHORIZATION_REJECTED = TIZEN_ERROR_BLUETOOTH|0x010E, /**< Authorization rejected (Since 5.0) */
} bt_error_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Enumerations of the Bluetooth adapter state.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_DISABLED = 0x00, /**< Bluetooth adapter is disabled */
	BT_ADAPTER_ENABLED, /**< Bluetooth adapter is enabled */
} bt_adapter_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumerations of the Bluetooth visibility mode.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE = 0x00,  /**< Other devices cannot find your device via discovery */
	BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE,  /**< Discoverable mode */
	BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE,  /**< Discoverable mode with time limit. After specific period,
							    it is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE.*/
} bt_adapter_visibility_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumerations of the discovery state of Bluetooth device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_DEVICE_DISCOVERY_STARTED, /**< Device discovery is started */
	BT_ADAPTER_DEVICE_DISCOVERY_FINISHED, /**< Device discovery is finished */
	BT_ADAPTER_DEVICE_DISCOVERY_FOUND, /**< The remote Bluetooth device is found */
} bt_adapter_device_discovery_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising state.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_LE_ADVERTISING_STOPPED = 0x00, /**< Bluetooth advertising is stopped */
	BT_ADAPTER_LE_ADVERTISING_STARTED, /**< Bluetooth advertising is started */
} bt_adapter_le_advertising_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising mode.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_ADAPTER_LE_ADVERTISING_MODE_BALANCED,  /**< Balanced advertising mode */
	BT_ADAPTER_LE_ADVERTISING_MODE_LOW_LATENCY,  /**< Low latency advertising mode */
	BT_ADAPTER_LE_ADVERTISING_MODE_LOW_ENERGY  /**< Low energy advertising mode */
} bt_adapter_le_advertising_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising filter policy.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_LE_ADVERTISING_FILTER_DEFAULT = 0x00, /**< White list is not in use */
	BT_ADAPTER_LE_ADVERTISING_FILTER_ALLOW_SCAN_WL = 0x01, /**< Allow the scan
					request that in the White list */
	BT_ADAPTER_LE_ADVERTISING_FILTER_ALLOW_CONN_WL = 0x02, /**< Allow the connection
					request that in the White list */
	BT_ADAPTER_LE_ADVERTISING_FILTER_ALLOW_SCAN_CONN_WL = 0x03, /**< Allow the
					scan and connection request that in the White list */
} bt_adapter_le_advertising_filter_policy_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth LE packet type.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_LE_PACKET_ADVERTISING, /**< Advertising packet */
	BT_ADAPTER_LE_PACKET_SCAN_RESPONSE, /**< Scan response packet */
} bt_adapter_le_packet_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth le scan mode.
 * @since_tizen 3.0
 */
typedef enum {
	BT_ADAPTER_LE_SCAN_MODE_BALANCED, /**< Balanced mode of power consumption and connection latency */
	BT_ADAPTER_LE_SCAN_MODE_LOW_LATENCY, /**< Low connection latency but high power consumption */
	BT_ADAPTER_LE_SCAN_MODE_LOW_ENERGY /**< Low power consumption but high connection latency */
} bt_adapter_le_scan_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of device disconnect reason.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_DEVICE_DISCONNECT_REASON_UNKNOWN, /**< Disconnected by unknown reason */
	BT_DEVICE_DISCONNECT_REASON_TIMEOUT, /**< Disconnected by timeout */
	BT_DEVICE_DISCONNECT_REASON_LOCAL_HOST, /**< Disconnected by local host */
	BT_DEVICE_DISCONNECT_REASON_REMOTE, /**< Disconnected by remote */
} bt_device_disconnect_reason_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of connection link type.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_DEVICE_CONNECTION_LINK_BREDR, /**< BR/EDR link */
	BT_DEVICE_CONNECTION_LINK_LE, /**< LE link */
	BT_DEVICE_CONNECTION_LINK_DEFAULT = 0xFF, /**< The connection type default */
} bt_device_connection_link_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of device authorization state.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_DEVICE_AUTHORIZED, /**< The remote Bluetooth device is authorized */
	BT_DEVICE_UNAUTHORIZED, /**< The remote Bluetooth device is unauthorized */
} bt_device_authorization_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of Bluetooth profile.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_PROFILE_RFCOMM = 0x01, /**< RFCOMM Profile */
	BT_PROFILE_A2DP = 0x02, /**< Advanced Audio Distribution Profile Source role */
	BT_PROFILE_HSP = 0x04, /**< Headset Profile */
	BT_PROFILE_HID = 0x08, /**< Human Interface Device Profile */
	BT_PROFILE_NAP = 0x10, /**< Network Access Point Profile */
	BT_PROFILE_AG = 0x20, /**< Audio Gateway Profile */
	BT_PROFILE_GATT = 0x40, /**< Generic Attribute Profile */
	BT_PROFILE_NAP_SERVER = 0x80, /**< NAP server Profile */
	BT_PROFILE_A2DP_SINK = 0x100, /**< Advanced Audio Distribution Profile Sink role */
} bt_profile_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Enumerations of device address type.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_DEVICE_PUBLIC_ADDRESS = 0x00, /**< Public address */
	BT_DEVICE_RANDOM_ADDRESS, /**< Random address */
} bt_device_address_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of service class.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_SC_NONE = 0, /**< No service class */
	BT_SC_RES_SERVICE_MASK = 0x00000001, /**< RES service class */
	BT_SC_SPP_SERVICE_MASK = 0x00000002, /**< SPP service class */
	BT_SC_DUN_SERVICE_MASK = 0x00000004, /**< DUN service class */
	BT_SC_FAX_SERVICE_MASK = 0x00000008, /**< FAX service class */
	BT_SC_LAP_SERVICE_MASK = 0x00000010, /**< LAP service class */
	BT_SC_HSP_SERVICE_MASK = 0x00000020, /**< HSP service class */
	BT_SC_HFP_SERVICE_MASK = 0x00000040, /**< HFP service class */
	BT_SC_OPP_SERVICE_MASK = 0x00000080, /**< OPP service class */
	BT_SC_FTP_SERVICE_MASK = 0x00000100, /**< FTP service class */
	BT_SC_CTP_SERVICE_MASK = 0x00000200, /**< CTP service class */
	BT_SC_ICP_SERVICE_MASK = 0x00000400, /**< ICP service class */
	BT_SC_SYNC_SERVICE_MASK = 0x00000800, /**< SYNC service class */
	BT_SC_BPP_SERVICE_MASK = 0x00001000, /**< BPP service class */
	BT_SC_BIP_SERVICE_MASK = 0x00002000, /**< BIP service class */
	BT_SC_PANU_SERVICE_MASK = 0x00004000, /**< PANU service class */
	BT_SC_NAP_SERVICE_MASK = 0x00008000, /**< NAP service class */
	BT_SC_GN_SERVICE_MASK = 0x00010000, /**< GN service class */
	BT_SC_SAP_SERVICE_MASK = 0x00020000, /**< SAP service class */
	BT_SC_A2DP_SERVICE_MASK = 0x00040000, /**< A2DP service class */
	BT_SC_AVRCP_SERVICE_MASK = 0x00080000, /**< AVRCP service class */
	BT_SC_PBAP_SERVICE_MASK = 0x00100000, /**< PBAP service class */
	BT_SC_HID_SERVICE_MASK = 0x00200000, /**< HID service class */
	BT_SC_A2DP_SOURCE_SERVICE_MASK = 0x00400000, /**< A2DP SOURCE service class */
	BT_SC_ALL_SERVICE_MASK = 0x00FFFFFF, /**< ALL service class */
	BT_SC_MAX /**< MAX service class */
} bt_service_class_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of major service class.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_MAJOR_SERVICE_CLASS_LIMITED_DISCOVERABLE_MODE  = 0x002000, /**< Limited discoverable mode */
	BT_MAJOR_SERVICE_CLASS_POSITIONING                = 0x010000, /**< Positioning class */
	BT_MAJOR_SERVICE_CLASS_NETWORKING                 = 0x020000, /**< Networking class */
	BT_MAJOR_SERVICE_CLASS_RENDERING                  = 0x040000, /**< Rendering class */
	BT_MAJOR_SERVICE_CLASS_CAPTURING                  = 0x080000, /**< Capturing class */
	BT_MAJOR_SERVICE_CLASS_OBJECT_TRANSFER            = 0x100000, /**< Object transferring class */
	BT_MAJOR_SERVICE_CLASS_AUDIO                      = 0x200000, /**< Audio class*/
	BT_MAJOR_SERVICE_CLASS_TELEPHONY                  = 0x400000, /**< Telephony class */
	BT_MAJOR_SERVICE_CLASS_INFORMATION                = 0x800000, /**< Information class */
} bt_major_service_class_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of major device class.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_MAJOR_DEVICE_CLASS_MISC = 0x00, /**< Miscellaneous major device class*/
	BT_MAJOR_DEVICE_CLASS_COMPUTER = 0x01, /**< Computer major device class */
	BT_MAJOR_DEVICE_CLASS_PHONE = 0x02, /**< Phone major device class */
	BT_MAJOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT = 0x03, /**< LAN/Network access point major device class */
	BT_MAJOR_DEVICE_CLASS_AUDIO_VIDEO = 0x04, /**< Audio/Video major device class */
	BT_MAJOR_DEVICE_CLASS_PERIPHERAL = 0x05, /**< Peripheral major device class */
	BT_MAJOR_DEVICE_CLASS_IMAGING = 0x06, /**< Imaging major device class */
	BT_MAJOR_DEVICE_CLASS_WEARABLE = 0x07, /**< Wearable device class */
	BT_MAJOR_DEVICE_CLASS_TOY = 0x08, /**< Toy device class */
	BT_MAJOR_DEVICE_CLASS_HEALTH = 0x09, /**< Health device class */
	BT_MAJOR_DEVICE_CLASS_UNCATEGORIZED = 0x1F, /**< Uncategorized major device class */
} bt_major_device_class_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of minor device class.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_MINOR_DEVICE_CLASS_COMPUTER_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_DESKTOP_WORKSTATION = 0x04, /**< Desktop workstation minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_SERVER_CLASS = 0x08, /**< Server minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_LAPTOP = 0x0C, /**< Laptop minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_HANDHELD_PC_OR_PDA = 0x10, /**< Handheld PC/PDA minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_PALM_SIZED_PC_OR_PDA = 0x14, /**< Palm sized PC/PDA minor device class of computer */
	BT_MINOR_DEVICE_CLASS_COMPUTER_WEARABLE_COMPUTER = 0x18, /**< Wearable(watch sized) minor device class of computer */

	BT_MINOR_DEVICE_CLASS_PHONE_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_CELLULAR = 0x04, /**< Cellular minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_CORDLESS = 0x08, /**< Cordless minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_SMART_PHONE = 0x0C, /**< Smart phone minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_WIRED_MODEM_OR_VOICE_GATEWAY = 0x10, /**< Wired modem or voice gateway minor device class of phone */
	BT_MINOR_DEVICE_CLASS_PHONE_COMMON_ISDN_ACCESS = 0x14, /**< Common ISDN minor device class of phone */

	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_FULLY_AVAILABLE = 0x04, /**< Fully available minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_1_TO_17_PERCENT_UTILIZED = 0x20, /**< 1-17% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_17_TO_33_PERCENT_UTILIZED = 0x40, /**< 17-33% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_33_TO_50_PERCENT_UTILIZED = 0x60, /**< 33-50% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_50_to_67_PERCENT_UTILIZED = 0x80, /**< 50-67% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_67_TO_83_PERCENT_UTILIZED = 0xA0, /**< 67-83% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_83_TO_99_PERCENT_UTILIZED = 0xC0, /**< 83-99% utilized minor device class of LAN/Network access point */
	BT_MINOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT_NO_SERVICE_AVAILABLE = 0xE0, /**< No service available minor device class of LAN/Network access point */

	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_WEARABLE_HEADSET = 0x04, /**< Wearable headset minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HANDS_FREE = 0x08, /**< Hands-free minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_MICROPHONE = 0x10, /**< Microphone minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_LOUDSPEAKER = 0x14, /**< Loudspeaker minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HEADPHONES = 0x18, /**< Headphones minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_PORTABLE_AUDIO = 0x1C, /**< Portable audio minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_CAR_AUDIO = 0x20, /**< Car audio minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_SET_TOP_BOX = 0x24, /**< Set-top box minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HIFI_AUDIO_DEVICE = 0x28, /**< Hifi audio minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VCR = 0x2C, /**< VCR minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_CAMERA = 0x30, /**< Video camera minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_CAMCORDER = 0x34, /**< Camcorder minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_MONITOR = 0x38, /**< Video monitor minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_DISPLAY_LOUDSPEAKER = 0x3C, /**< Video display and loudspeaker minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_CONFERENCING = 0x40, /**< Video conferencing minor device class of audio/video */
	BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_GAMING_TOY = 0x48, /**< Gaming/toy minor device class of audio/video */

	BT_MINOR_DEVICE_CLASS_PERIPHERA_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_KEY_BOARD = 0x40, /**< Key board minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_POINTING_DEVICE = 0x80, /**< Pointing device minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_COMBO_KEYBOARD_POINTING_DEVICE = 0xC0, /**< Combo keyboard or pointing device minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_JOYSTICK = 0x04, /**< Joystick minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_GAME_PAD = 0x08, /**< Game pad minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_REMOTE_CONTROL = 0x0C, /**< Remote control minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_SENSING_DEVICE = 0x10, /**< Sensing device minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITIZER_TABLET = 0x14, /**< Digitizer minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_CARD_READER = 0x18, /**< Card reader minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITAL_PEN = 0x1C, /**< Digital pen minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_SCANNER = 0x20, /**< Handheld scanner minor device class of peripheral */
	BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_GESTURAL_INPUT_DEVICE = 0x24, /**< Handheld gestural input device minor device class of peripheral */

	BT_MINOR_DEVICE_CLASS_IMAGING_DISPLAY = 0x10, /**< Display minor device class of imaging */
	BT_MINOR_DEVICE_CLASS_IMAGING_CAMERA = 0x20, /**< Camera minor device class of imaging */
	BT_MINOR_DEVICE_CLASS_IMAGING_SCANNER = 0x40, /**< Scanner minor device class of imaging */
	BT_MINOR_DEVICE_CLASS_IMAGING_PRINTER = 0x80, /**< Printer minor device class of imaging */

	BT_MINOR_DEVICE_CLASS_WEARABLE_WRIST_WATCH = 0x04, /**< Wrist watch minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_PAGER = 0x08, /**< Pager minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_JACKET = 0x0C, /**< Jacket minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_HELMET = 0x10, /**< Helmet minor device class of wearable */
	BT_MINOR_DEVICE_CLASS_WEARABLE_GLASSES = 0x14, /**< Glasses minor device class of wearable */

	BT_MINOR_DEVICE_CLASS_TOY_ROBOT = 0x04, /**< Robot minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_VEHICLE = 0x08, /**< Vehicle minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_DOLL_ACTION = 0x0C, /**< Doll/Action minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_CONTROLLER = 0x10, /**< Controller minor device class of toy */
	BT_MINOR_DEVICE_CLASS_TOY_GAME = 0x14, /**< Game minor device class of toy */

	BT_MINOR_DEVICE_CLASS_HEALTH_UNCATEGORIZED = 0x00, /**< Uncategorized minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_BLOOD_PRESSURE_MONITOR = 0x04, /**< Blood pressure monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_THERMOMETER = 0x08, /**< Thermometer minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_WEIGHING_SCALE = 0x0C, /**< Weighing scale minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_GLUCOSE_METER = 0x10, /**< Glucose minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_PULSE_OXIMETER = 0x14, /**< Pulse oximeter minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_HEART_PULSE_RATE_MONITOR = 0x18, /**< Heart/Pulse rate monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_DATA_DISPLAY = 0x1C, /**< Health data display minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_STEP_COUNTER = 0x20, /**< Step counter minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_BODY_COMPOSITION_ANALYZER = 0x24, /**< Body composition analyzer minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_PEAK_FLOW_MONITOR = 0x28, /**< Peak flow monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_MEDICATION_MONITOR = 0x2C, /**< Medication monitor minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_KNEE_PROSTHESIS = 0x30, /**< Knee prosthesis minor device class of health */
	BT_MINOR_DEVICE_CLASS_HEALTH_ANKLE_PROSTHESIS = 0x34, /**< Ankle prosthesis minor device class of health */
} bt_minor_device_class_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of gap appearance type.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_APPEARANCE_TYPE_UNKNOWN = 0x00, /**< Unknown appearance type */
	BT_APPEARANCE_TYPE_GENERIC_PHONE = 0x40, /**< Generic Phone type - Generic category */
	BT_APPEARANCE_TYPE_GENERIC_COMPUTER = 0x80, /**< Generic Computer type - Generic category */
	BT_APPEARANCE_TYPE_GENERIC_WATCH = 0xC0, /**< Generic Watch type - Generic category */
} bt_appearance_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Enumerations of the Bluetooth device's LE connection mode.
 * @since_tizen 3.0
 */
typedef enum {
	BT_DEVICE_LE_CONNECTION_MODE_BALANCED, /**< Balanced mode of power consumption and connection latency */
	BT_DEVICE_LE_CONNECTION_MODE_LOW_LATENCY, /**< Low connection latency but high power consumption */
	BT_DEVICE_LE_CONNECTION_MODE_LOW_ENERGY, /**< Low power consumption but high connection latency */
} bt_device_le_connection_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Enumerations of connected Bluetooth device event role.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_SOCKET_UNKNOWN = 0x00, /**< Unknown role*/
	BT_SOCKET_SERVER , /**< Server role*/
	BT_SOCKET_CLIENT, /**< Client role*/
} bt_socket_role_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Enumerations of Bluetooth socket connection state.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_SOCKET_CONNECTED, /**< RFCOMM is connected */
	BT_SOCKET_DISCONNECTED, /**< RFCOMM is disconnected */
} bt_socket_connection_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Enumerations for the types of profiles related with audio.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_AUDIO_PROFILE_TYPE_ALL = 0,  /**< All supported profiles related with audio */
	BT_AUDIO_PROFILE_TYPE_HSP_HFP,  /**< HSP(Headset Profile) and HFP(Hands-Free Profile) */
	BT_AUDIO_PROFILE_TYPE_A2DP,  /**< A2DP(Advanced Audio Distribution Profile) */
	BT_AUDIO_PROFILE_TYPE_AG,  /**< AG(Audio Gateway) */
	BT_AUDIO_PROFILE_TYPE_A2DP_SINK,  /**< A2DP(Advanced Audio Distribution Profile) Sink role */
} bt_audio_profile_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call handling event.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_AG_CALL_HANDLING_EVENT_ANSWER = 0x00,  /**< Request to answer an incoming call */
	BT_AG_CALL_HANDLING_EVENT_RELEASE,  /**< Request to release a call */
	BT_AG_CALL_HANDLING_EVENT_REJECT,  /**< Request to reject an incoming call */
} bt_ag_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the multi call handling event.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_AG_MULTI_CALL_HANDLING_EVENT_RELEASE_HELD_CALLS = 0x00,  /**< Request to release held calls */
	BT_AG_MULTI_CALL_HANDLING_EVENT_RELEASE_ACTIVE_CALLS,  /**< Request to release active calls */
	BT_AG_MULTI_CALL_HANDLING_EVENT_ACTIVATE_HELD_CALL,  /**< Request to put active calls into hold state and activate another (held or waiting) call */
	BT_AG_MULTI_CALL_HANDLING_EVENT_MERGE_CALLS,  /**< Request to add a held call to the conversation */
	BT_AG_MULTI_CALL_HANDLING_EVENT_EXPLICIT_CALL_TRANSFER,  /**< Request to let a user who has two calls to connect these two calls together and release its connections to both other parties */
} bt_ag_multi_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the equalizer state.
 * @since_tizen @if WEARABLE 3.0 @else 2.4 @endif
 */
typedef enum {
	BT_AVRCP_EQUALIZER_STATE_OFF = 0x01,  /**< Equalizer Off */
	BT_AVRCP_EQUALIZER_STATE_ON,  /**< Equalizer On */
} bt_avrcp_equalizer_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the repeat mode.
 * @since_tizen @if WEARABLE 3.0 @else 2.4 @endif
 */
typedef enum {
	BT_AVRCP_REPEAT_MODE_OFF = 0x01,  /**< Repeat Off */
	BT_AVRCP_REPEAT_MODE_SINGLE_TRACK,  /**< Single track repeat */
	BT_AVRCP_REPEAT_MODE_ALL_TRACK,  /**< All track repeat */
	BT_AVRCP_REPEAT_MODE_GROUP,  /**< Group repeat */
} bt_avrcp_repeat_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the shuffle mode.
 * @since_tizen @if WEARABLE 3.0 @else 2.4 @endif
 */
typedef enum {
	BT_AVRCP_SHUFFLE_MODE_OFF = 0x01,  /**< Shuffle Off */
	BT_AVRCP_SHUFFLE_MODE_ALL_TRACK,  /**< All tracks shuffle */
	BT_AVRCP_SHUFFLE_MODE_GROUP,  /**< Group shuffle */
} bt_avrcp_shuffle_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the scan mode.
 * @since_tizen @if WEARABLE 3.0 @else 2.4 @endif
 */
typedef enum {
	BT_AVRCP_SCAN_MODE_OFF = 0x01,  /**< Scan Off */
	BT_AVRCP_SCAN_MODE_ALL_TRACK,  /**< All tracks scan */
	BT_AVRCP_SCAN_MODE_GROUP,  /**< Group scan */
} bt_avrcp_scan_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the player state.
 * @since_tizen 3.0
 */
typedef enum {
	BT_AVRCP_PLAYER_STATE_STOPPED = 0x00,  /**< Stopped */
	BT_AVRCP_PLAYER_STATE_PLAYING,  /**< Playing */
	BT_AVRCP_PLAYER_STATE_PAUSED,  /**< Paused */
	BT_AVRCP_PLAYER_STATE_FORWARD_SEEK,  /**< Seek Forward */
	BT_AVRCP_PLAYER_STATE_REWIND_SEEK,  /**< Seek Rewind */
} bt_avrcp_player_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumeration for the player control commands.
 * @since_tizen 3.0
 */
typedef enum {
	BT_AVRCP_CONTROL_PLAY = 0x01,   /**< Play */
	BT_AVRCP_CONTROL_PAUSE,   /**< Pause */
	BT_AVRCP_CONTROL_STOP,   /**< Stop */
	BT_AVRCP_CONTROL_NEXT,   /**< Next Track*/
	BT_AVRCP_CONTROL_PREVIOUS,   /**< Previous track */
	BT_AVRCP_CONTROL_FAST_FORWARD,   /**< Fast Forward */
	BT_AVRCP_CONTROL_REWIND   /**< Rewind */
} bt_avrcp_player_command_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Structure of Track metadata information.
 * @since_tizen 3.0
 *
 * @see #bt_class_s
 */
typedef struct {
	const char *title;   /**< Title */
	const char *artist;   /**< Artist */
	const char *album;   /**< Album name */
	const char *genre;   /**< Album Genre */
	unsigned int total_tracks;   /**< The total number of tracks */
	unsigned int number;   /**< Track number */
	unsigned int duration;   /**< Duration */
} bt_avrcp_metadata_attributes_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen 3.0
 * @param[in] connected  The state to be changed. true means connected state, Otherwise, false.
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_control_initialize()
 * @see bt_avrcp_control_deinitialize()
 */
typedef void (*bt_avrcp_control_connection_state_changed_cb) (bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the Song position mode is changed by the remote target device.
 * @since_tizen 3.0
 * @param[in] position Playback position in milliseconds. When position is 0 it means the track is starting and when it's greater than or equal to track's duration the track has ended.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_position_changed_cb()
 * @see bt_avrcp_unset_position_changed_cb()
 */
typedef void (*bt_avrcp_position_changed_cb) (unsigned int position, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the Song metadata information is changed by the remote target device.
 * @since_tizen 3.0
 * @param[in] track The song metadata information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_track_info_changed_cb()
 * @see bt_avrcp_unset_track_info_changed_cb()
 */
typedef void (*bt_avrcp_track_info_changed_cb) (bt_avrcp_metadata_attributes_info_s *track, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the Song Play status mode is changed by the remote target device.
 * @since_tizen 3.0
 * @param[in] play_state The song play status
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_play_status_changed_cb()
 * @see bt_avrcp_unset_play_status_changed_cb()
 */
typedef void (*bt_avrcp_play_status_changed_cb) (bt_avrcp_player_state_e play_state, void *user_data);

/**
 * @deprecated Deprecated since 5.0.
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Enumerations for the data channel type.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @remarks Deprecated, because of no usecase and supported devices.
 */
typedef enum {
	BT_HDP_CHANNEL_TYPE_RELIABLE  = 0x01,  /**< Reliable Data Channel */
	BT_HDP_CHANNEL_TYPE_STREAMING,  /**< Streaming Data Channel */
} bt_hdp_channel_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the integer type for GATT handle's value.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_DATA_TYPE_SINT8,  /**< 8 bit signed int type */
	BT_DATA_TYPE_SINT16,  /**< 16 bit signed int type */
	BT_DATA_TYPE_SINT32,  /**< 32 bit signed int type */
	BT_DATA_TYPE_UINT8,  /**< 8 bit unsigned int type */
	BT_DATA_TYPE_UINT16,  /**< 16 bit unsigned int type */
	BT_DATA_TYPE_UINT32  /**< 32 bit unsigned int type */
} bt_data_type_int_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the float type for GATT handle's value.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_DATA_TYPE_FLOAT,  /**< 32 bit float type */
	BT_DATA_TYPE_SFLOAT  /**< 16 bit float type */
} bt_data_type_float_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the write type.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_GATT_WRITE_TYPE_WRITE_NO_RESPONSE,  /**< Write without response type */
	BT_GATT_WRITE_TYPE_WRITE  /**< Write type */
} bt_gatt_write_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the GATT handle's type.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_GATT_TYPE_SERVICE = 0x01,  /**< GATT service type */
	BT_GATT_TYPE_CHARACTERISTIC = 0x02,  /**< GATT characteristic type */
	BT_GATT_TYPE_DESCRIPTOR = 0x03,  /**< GATT descriptor type */
} bt_gatt_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the service type.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_GATT_SERVICE_TYPE_PRIMARY = 0x01,  /**< GATT primary service type */
	BT_GATT_SERVICE_TYPE_SECONDARY = 0x02,  /**< GATT secondary service type */
} bt_gatt_service_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the characteristic's property.
 * @since_tizen 2.3.1
 */
typedef enum {
	BT_GATT_PROPERTY_BROADCAST = 0x01,  /**< Broadcast property */
	BT_GATT_PROPERTY_READ = 0x02,  /**< Read property */
	BT_GATT_PROPERTY_WRITE_WITHOUT_RESPONSE = 0x04,  /**< Write without response property */
	BT_GATT_PROPERTY_WRITE = 0x08,  /**< Write property */
	BT_GATT_PROPERTY_NOTIFY = 0x10,  /**< Notify property */
	BT_GATT_PROPERTY_INDICATE = 0x20,  /**< Indicate property */
	BT_GATT_PROPERTY_AUTHENTICATED_SIGNED_WRITES = 0x40,  /**< Authenticated signed writes property */
	BT_GATT_PROPERTY_EXTENDED_PROPERTIES = 0x80,  /**< Extended properties */
} bt_gatt_property_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Enumerations of gatt server's service changing mode.
 * @since_tizen 3.0
 */
typedef enum {
	BT_GATT_CLIENT_SERVICE_ADDED,	/**< Service added */
	BT_GATT_CLIENT_SERVICE_REMOVED,	/**< Service removed */
} bt_gatt_client_service_change_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the attribute's permission.
 * @since_tizen 3.0
 */
typedef enum {
	BT_GATT_PERMISSION_READ = 0x01,   /**< Readable permission */
	BT_GATT_PERMISSION_WRITE = 0x02, /**< Writable permission */
	BT_GATT_PERMISSION_ENCRYPT_READ = 0x04,  /**< Readable permission required encryption */
	BT_GATT_PERMISSION_ENCRYPT_WRITE = 0x08,  /**< Writable permission required encryption */
	BT_GATT_PERMISSION_ENCRYPT_AUTHENTICATED_READ = 0x10,	 /**< Readable permission required encryption and authentication */
	BT_GATT_PERMISSION_ENCRYPT_AUTHENTICATED_WRITE = 0x20,	/**< Writable permission required encryption and authentication */
} bt_gatt_permission_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Enumerations of the remote device request types for attributes.
 * @since_tizen 3.0
 */
typedef enum {
	BT_GATT_REQUEST_TYPE_READ = 0x00, /**< Read Requested */
	BT_GATT_REQUEST_TYPE_WRITE = 0x01, /**< Write Requested */
} bt_gatt_att_request_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Enumerations for the types of PAN (Personal Area Networking) service.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_PANU_SERVICE_TYPE_NAP = 0,  /**< Network Access Point */
} bt_panu_service_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle to control Bluetooth LE advertising.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef void *bt_advertiser_h;

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Enumeration of address book location for PBAP.
 * @since_tizen 3.0
 */
typedef enum {
	BT_PBAP_SOURCE_DEVICE = 0x00, /**< Request for Addressbook from remote device */
	BT_PBAP_SOURCE_SIM , /**< Request for address book from SIM */
} bt_pbap_address_book_source_e;

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Enumeration of folder type.
 * @since_tizen 3.0
 */
typedef enum {
	BT_PBAP_FOLDER_PHONE_BOOK = 0x00, /**< Request for address book */
	BT_PBAP_FOLDER_INCOMING , /**< Request for incoming calls */
	BT_PBAP_FOLDER_OUTGOING , /**< Request for outgoing calls */
	BT_PBAP_FOLDER_MISSED , /**< Request for missed calls */
	BT_PBAP_FOLDER_COMBINED , /**< Request for combined calls */
} bt_pbap_folder_type_e;

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Enumeration of phone book search fields.
 * @since_tizen 3.0
 */
typedef enum {
	BT_PBAP_SEARCH_NAME = 0x00, /**< Request for search by name (default) */
	BT_PBAP_SEARCH_NUMBER, /**< Request for search by phone number */
	BT_PBAP_SEARCH_SOUND, /**< Request for search by sound */
} bt_pbap_search_field_e;

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Enumeration of vCard Formats.
 * @since_tizen 3.0
 */
typedef enum {
	BT_PBAP_VCARD_FORMAT_VCARD21 = 0x00, /**< vCard format 2.1 (default) */
	BT_PBAP_VCARD_FORMAT_VCARD30, /**< vCard format 3.0 */
} bt_pbap_vcard_format_e;

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Enumeration of sorting orders.
 * @since_tizen 3.0
 */
typedef enum {
	BT_PBAP_ORDER_INDEXED = 0x00, /**< Filter order indexed (default) */
	BT_PBAP_ORDER_ALPHANUMERIC, /**< Filter order alphanumeric */
	BT_PBAP_ORDER_PHONETIC, /**< Filter order phonetic */
} bt_pbap_sort_order_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief The handle of a service, characteristic or descriptor.
 * @since_tizen 2.3.1
 */
typedef void *bt_gatt_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief The handle of a GATT client which is associated with a remote device.
 * @since_tizen 2.3.1
 */
typedef void *bt_gatt_client_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief The handle of a GATT server.
 * @since_tizen 3.0
 */
typedef void *bt_gatt_server_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  The handle of calls state.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef void *bt_call_list_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Class structure of device and service.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @see #bt_device_info_s
 * @see #bt_adapter_device_discovery_info_s
 * @see bt_device_bond_created_cb()
 * @see bt_adapter_device_discovery_state_changed_cb()
 */
typedef struct {
	bt_major_device_class_e major_device_class;	/**< Major device class. */
	bt_minor_device_class_e minor_device_class;	/**< Minor device class. */
	int major_service_class_mask;	/**< Major service class mask.
	This value can be a combination of #bt_major_service_class_e like #BT_MAJOR_SERVICE_CLASS_RENDERING | #BT_MAJOR_SERVICE_CLASS_AUDIO */
} bt_class_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Structure of device discovery information.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @see #bt_class_s
 * @see bt_adapter_device_discovery_state_changed_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	char *remote_name;	/**< The name of remote device */
	bt_class_s bt_class;	/**< The Bluetooth classes */
	int rssi;	/**< The strength indicator of received signal  */
	bool is_bonded;	/**< The bonding state */
	char **service_uuid;  /**< The UUID list of service */
	int service_count;	/**< The number of services */
	bt_appearance_type_e appearance;	/**< The Bluetooth appearance */
	int manufacturer_data_len;	/**< manufacturer specific data length */
	char *manufacturer_data;		/**< manufacturer specific data */
} bt_adapter_device_discovery_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Structure of le scan result information.
 * @since_tizen 2.3.1
 *
 * @see bt_adapter_le_start_scan()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	bt_device_address_type_e address_type;	/**< The address type of remote device */
	int rssi;		/**< The strength indicator of received signal  */
	int adv_data_len;	/**< advertising indication data length */
	char *adv_data;		/**< advertising indication data */
	int scan_data_len;	/**< scan response data length */
	char *scan_data;	/**< scan response data */
} bt_adapter_le_device_scan_result_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The structure for LE iBeacon scan result information.
 * @since_tizen 4.0
 *
 * @see bt_adapter_le_start_scan()
 */
typedef struct {
	int company_id;		/**< Company ID */
	int ibeacon_type;	/**< iBeacon type */
	char *uuid;			/**< UUID */
	int major_id;		/**< Major ID */
	int minor_id;		/**< Minor ID */
	int measured_power;	/**< Measured power */
} bt_adapter_le_ibeacon_scan_result_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief LE service data structure.
 * @since_tizen 2.3.1
 *
 * @see bt_adapter_le_get_scan_result_service_data()
 */
typedef struct {
	char *service_uuid;	/**< 16 bit UUID of the service data */
	char *service_data;	/**< Service data */
	int service_data_len;	/**< Service data length */
} bt_adapter_le_service_data_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_IPSP_MODULE
 * @brief Called when the IPSP initialization state changes.
 * @since_tizen 4.0
 * @param[in] result The result of the initialization or deinitialization request
 * @param[in] ipsp_initialized The initialized state: (@c true = initialized, @c false = deinitialized)
 * @param[in] user_data The user data passed from the initialization request function
 * @see bt_ipsp_initialize()
 * @see bt_ipsp_deinitialize()
 */
typedef void (*bt_ipsp_init_state_changed_cb)
		(int result, bool ipsp_initialized, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_IPSP_MODULE
 * @brief Called when the IPSP connection state changes.
 * @since_tizen 4.0
 * @param[in] result The result of the connection or disconnection request
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] remote_address  Address of remote device
 * @param[in] iface_name The network interface name for IPv6 network interface
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ipsp_connect()
 * @see bt_ipsp_disconnect()
 */
typedef void (*bt_ipsp_connection_state_changed_cb)
		(int result, bool connected, const char *remote_address, const char *iface_name, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device information structure used for identifying pear device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @see #bt_class_s
 * @see bt_device_bond_created_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	char *remote_name;	/**< The name of remote device */
	bt_class_s bt_class;	/**< The Bluetooth classes */
	char **service_uuid;  /**< The UUID list of service */
	int service_count;	/**< The number of services */
	bool is_bonded;	/**< The bonding state */
	bool is_connected;	/**< The connection state */
	bool is_authorized;	/**< The authorization state */
	int manufacturer_data_len;	/**< manufacturer specific data length */
	char *manufacturer_data;		/**< manufacturer specific data */
} bt_device_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Service Discovery Protocol (SDP) data structure.
 *
 * @details This protocol is used for discovering available services or pear device,
 * and finding one to connect with.
 *
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @see bt_device_service_searched_cb()
 */
typedef struct {
	char *remote_address;   /**< The address of remote device */
	char **service_uuid;  /**< The UUID list of service */
	int service_count;    /**< The number of services. */
} bt_device_sdp_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device connection information structure.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @see bt_device_connection_state_changed_cb()
 */
typedef struct {
	char *remote_address;   /**< The address of remote device */
	bt_device_connection_link_type_e link;  /**< Link type */
	bt_device_disconnect_reason_e disconn_reason;  /**< Disconnection reason */
} bt_device_connection_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Rfcomm connection data used for exchanging data between Bluetooth devices.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @see bt_socket_connection_state_changed_cb()
 */
typedef struct {
	int socket_fd;	/**< The file descriptor of connected socket */
	int server_fd;	/**< The file descriptor of the server socket or -1 for client connection */
	bt_socket_role_e local_role;	/**< The local device role in this connection */
	char *remote_address;	/**< The remote device address */
	char *service_uuid;	/**< The service UUId */
} bt_socket_connection_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Structure of RFCOMM received data.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @remarks User can use standard linux functions for reading/writing \n
 * data from/to sockets.
 *
 * @see bt_socket_data_received_cb()
 */
typedef struct {
	int socket_fd;	/**< The socket fd */
	int data_size;	/**< The length of the received data */
	char *data;	/**< The received data */
} bt_socket_received_data_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the Bluetooth adapter state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in]   result  The result of the adapter state changing
 * @param[in]   adapter_state  The adapter state to be changed
 * @param[in]   user_data  The user data passed from the callback registration function
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 */
typedef void (*bt_adapter_state_changed_cb)(int result, bt_adapter_state_e adapter_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when adapter name changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in]   device_name	The name of the Bluetooth device to be changed
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when the name of Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_name_changed_cb().
 * @see bt_adapter_set_name()
 * @see bt_adapter_set_name_changed_cb()
 * @see bt_adapter_unset_name_changed_cb()
 */
typedef void (*bt_adapter_name_changed_cb)(char *device_name, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the visibility mode changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result The result of the visibility mode changing
 * @param[in] visibility_mode The visibility mode to be changed
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre This function will be invoked when the visibility of Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_visibility_mode_changed_cb().
 *
 * @see bt_adapter_set_visibility_mode_changed_cb()
 * @see bt_adapter_unset_visibility_mode_changed_cb()
 */
typedef void (*bt_adapter_visibility_mode_changed_cb)
	(int result, bt_adapter_visibility_mode_e visibility_mode, void *user_data);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called every second until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @remarks  This callback function is called only if visibility mode is #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE.
 * @param[in]  duration  The duration until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE (in seconds)
 * @param[in]  user_data  The user data passed from the callback registration function
 * @pre  This function will be invoked if you register this callback using bt_adapter_set_visibility_duration_changed_cb().
 * @see  bt_adapter_set_visibility_duration_changed_cb()
 * @see  bt_adapter_unset_visibility_duration_changed_cb()
 */
typedef void (*bt_adapter_visibility_duration_changed_cb)(int duration, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the state of device discovery changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @remarks If \a discovery_state is #BT_ADAPTER_DEVICE_DISCOVERY_FOUND, \n
 * then you can get some information, such as remote device address, remote device name, rssi, and bonding state.
 *
 * @param[in] result The result of the device discovery
 * @param[in] discovery_state The discovery state to be changed
 * @param[in] discovery_info The information of the discovered device \n
 *					If \a discovery_state is #BT_ADAPTER_DEVICE_DISCOVERY_STARTED or
 * #BT_ADAPTER_DEVICE_DISCOVERY_FINISHED, then \a discovery_info is NULL.
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre Either bt_adapter_start_device_discovery() or bt_adapter_stop_device_discovery() will invoke this function
 * if you register this callback using bt_adapter_set_device_discovery_state_changed_cb().
 *
 * @see bt_adapter_start_device_discovery()
 * @see bt_adapter_stop_device_discovery()
 * @see bt_adapter_set_device_discovery_state_changed_cb()
 * @see bt_adapter_unset_device_discovery_state_changed_cb()
 *
 */
typedef void (*bt_adapter_device_discovery_state_changed_cb)
	(int result, bt_adapter_device_discovery_state_e discovery_state, bt_adapter_device_discovery_info_s *discovery_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when you get bonded devices repeatedly.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] device_info The bonded device information
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre bt_adapter_foreach_bonded_device() will invoke this function.
 *
 * @see bt_adapter_foreach_bonded_device()
 *
 */
typedef bool (*bt_adapter_bonded_device_cb)(bt_device_info_s *device_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the LE advertisement has been found.
 * @since_tizen 2.3.1
 *
 * @param[in] result The result of the LE scan
 * @param[in] info The information of the found LE advertisement.
 * @param[in] user_data The user data passed from the start function
 *
 * @see bt_adapter_le_start_scan()
 *
 */
typedef void (*bt_adapter_le_scan_result_cb)(int result,
		bt_adapter_le_device_scan_result_info_s *info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the state of advertiser changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[out] result The result of the requested state change of advertiser
 * @param[out] advertiser The handle of the state changed advertiser
 * @param[out] adv_state The advertiser state to be changed
 * @param[out] user_data The user data passed from the start function
 *
 * @see bt_adapter_le_start_advertising_new()
 * @see bt_adapter_le_stop_advertising()
 */
typedef void (*bt_adapter_le_advertising_state_changed_cb)(int result,
		bt_advertiser_h advertiser, bt_adapter_le_advertising_state_e adv_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Called when the process of creating bond finishes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @remarks If the remote user does not respond within 60 seconds, a time out will happen with #BT_ERROR_TIMED_OUT result code. \n
 * If bt_device_cancel_bonding() is called and it returns #BT_ERROR_NONE, then this callback function will be called \n
 * with #BT_ERROR_CANCELLED result. \n
 * If creating a bond succeeds but service search fails, then this callback will be called with #BT_ERROR_SERVICE_SEARCH_FAILED. \n
 * In this case, you should try service search again by bt_device_start_service_search() to get the supported service list.
 *
 * @param[in] result The result of the bonding device
 * @param[in] device_info The device information which you creates bond with
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre Either bt_device_create_bond() will invoke this function
 * if you register this callback using bt_device_set_bond_created_cb().
 *
 * @see bt_device_create_bond()
 * @see bt_device_cancel_bonding()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
typedef void (*bt_device_bond_created_cb)(int result, bt_device_info_s *device_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when you get connected profiles repeatedly.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] profile The connected Bluetooth profile
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre bt_device_foreach_connected_profiles() will invoke this function.
 * @see bt_device_foreach_connected_profiles()
 */
typedef bool (*bt_device_connected_profile)(bt_profile_e profile, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the bond destroys.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result The result that a bond is destroyed
 * @param[in] remote_address The address of the remote Bluetooth device to destroy bond with
 * @param[in] user_data The user data passed from the callback registration function
 * @pre bt_device_destroy_bond() will invoke this function
 * if you register this callback using bt_device_set_bond_destroyed_cb().
 *
 * @see bt_device_destroy_bond()
 * @see bt_device_set_bond_destroyed_cb()
 * @see bt_device_unset_bond_destroyed_cb()
 */
typedef void (*bt_device_bond_destroyed_cb)(int result, char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the authorization of device changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] authorization The authorization of device
 * @param[in] remote_address The address of the remote Bluetooth device which is (un)authorized
 * @param[in] user_data The user data passed from the callback registration function
 * @pre bt_device_set_authorization() will invoke this function if you register this callback using bt_device_set_authorization_changed_cb().
 *
 * @see bt_device_set_authorization()
 * @see #bt_device_set_authorization_changed_cb()
 * @see #bt_device_unset_authorization_changed_cb()
 */
typedef void (*bt_device_authorization_changed_cb)
	(bt_device_authorization_e authorization, char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the process of service search finishes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result The result of the service searching
 * @param[in] sdp_info The structure of service lists found on a device
 * @param[in] user_data The user data passed from the callback registration function
 * @pre Either bt_device_start_service_search() will invoke this function
 * if you register this callback using  bt_device_set_service_searched_cb().
 *
 * @see bt_device_start_service_search()
 * @see bt_device_set_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 *
 */
typedef void (*bt_device_service_searched_cb)(int result, bt_device_sdp_info_s *sdp_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] conn_info The connection information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_device_set_connection_state_changed_cb()
 * @see bt_device_unset_connection_state_changed_cb()
 */
typedef void (*bt_device_connection_state_changed_cb)(bool connected, bt_device_connection_info_s *conn_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Called when you receive data.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] data The received data from the remote device
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre When the connected remote Bluetooth device invokes bt_socket_send_data(),
 *		this function will be invoked if you register this function using bt_socket_set_data_received_cb().
 *
 * @see bt_socket_set_data_received_cb()
 * @see bt_socket_unset_data_received_cb()
 * @see bt_socket_send_data()
 */
typedef void (*bt_socket_data_received_cb)(bt_socket_received_data_s *data, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Called when the socket connection state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result The result of connection state changing
 * @param[in] connection_state The connection state
 * @param[in] connection The connection information which is established or disconnected
 * @param[in] user_data The user data passed from the callback registration function
 * @pre Either bt_socket_connect_rfcomm() will invoke this function.
 * In addition, bt_socket_connection_state_changed_cb() will be invoked when the socket connection state is changed.
 * @see bt_socket_listen_and_accept_rfcomm()
 * @see bt_socket_connect_rfcomm()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
typedef void (*bt_socket_connection_state_changed_cb)
	(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Called when a RFCOMM connection is requested.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] socket_fd  The file descriptor of socket on which a connection is requested
 * @param[in] remote_address  The address of remote device
 * @param[in] user_data The user data passed from the callback registration function
 * @pre If you register this callback function by bt_socket_set_connection_requested_cb(),
 * bt_socket_connection_requested_cb() will be invoked.
 */
typedef void (*bt_socket_connection_requested_cb) (int socket_fd, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when an OPP connection is requested.
 *
 * @details You must call bt_opp_server_accept_connection() if you want to accept.
 * Otherwise, you must call bt_opp_server_reject_connection().
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] remote_address  The address of remote device
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_accept()
 * @see bt_opp_server_reject()
 */
typedef void (*bt_opp_server_connection_requested_cb)(const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when a file is being transferred.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] percent The progress in percentage (1 ~ 100)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_accept()
 * @see bt_opp_server_accept_connection()
 */
typedef void (*bt_opp_server_transfer_progress_cb) (const char *file, long long size, int percent, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when a transfer is finished.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result  The result of push
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_accept()
 * @see bt_opp_server_accept_connection()
 */
typedef void (*bt_opp_server_transfer_finished_cb) (int result, const char *file, long long size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 * @brief  Called when OPP server responds to the push request.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result  The result of OPP server response
 * @param[in] remote_address  The remote address
 * @param[in] user_data  The user data passed from the callback registration function
 * @pre bt_opp_client_push_files() will invoke this function.
 * @see bt_opp_client_push_files()
 */
typedef void (*bt_opp_client_push_responded_cb)(int result, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 * @brief  Called when each file is being transferred.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] percent The progress in percentage (1 ~ 100). 100 means that a file is transferred completely.
 * @param[in] user_data The user data passed from the callback registration function
 * @pre bt_opp_client_push_files() will invoke this function.
 * @see bt_opp_client_push_files()
 */
typedef void (*bt_opp_client_push_progress_cb)(const char *file, long long size, int percent, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE
 * @brief  Called when the push request is finished.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result  The result of the push request
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_client_push_files()
 */
typedef void (*bt_opp_client_push_finished_cb)(int result, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Called when the connection state is changed.
 *
 * @details  This callback is called when the connection state is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * When you call bt_audio_connect() or bt_audio_disconnect(), this callback is also called with error result even though these functions fail.
 * @param[in] result  The result of changing the connection state
 * @param[in] connected  The state to be changed. true means connected state, Otherwise, false.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of audio profile except #BT_AUDIO_PROFILE_TYPE_ALL
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_audio_set_connection_state_changed_cb()
 * @see bt_audio_unset_connection_state_changed_cb()
 */
typedef void (*bt_audio_connection_state_changed_cb) (int result, bool connected, const char *remote_address, bt_audio_profile_type_e type, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] call_id  The call ID
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
typedef void (*bt_ag_call_handling_event_cb) (bt_ag_call_handling_event_e event, unsigned int call_id, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a multi call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
typedef void (*bt_ag_multi_call_handling_event_cb) (bt_ag_multi_call_handling_event_e event, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a DTMF (Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] dtmf  The DTMF transmitted from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_dtmf_transmitted_cb()
 * @see bt_ag_unset_dtmf_transmitted_cb()
 */
typedef void (*bt_ag_dtmf_transmitted_cb) (const char *dtmf, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the speaker gain of the remote device is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_speaker_gain_changed_cb()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
typedef void (*bt_ag_speaker_gain_changed_cb) (int gain, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the microphone gain of the remote device is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] gain The gain of microphone (0 ~ 15)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_microphone_gain_changed_cb()
 * @see bt_ag_unset_microphone_gain_changed_cb()
 */
typedef void (*bt_ag_microphone_gain_changed_cb) (int gain, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] connected  The state to be changed. true means connected state, Otherwise, false.
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_target_initialize()
 * @see bt_avrcp_target_deinitialize()
 */
typedef void (*bt_avrcp_target_connection_state_changed_cb) (bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the equalizer state is changed by the remote control device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] equalizer The equalizer state
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_equalizer_state_changed_cb()
 * @see bt_avrcp_unset_equalizer_state_changed_cb()
 */
typedef void (*bt_avrcp_equalizer_state_changed_cb) (bt_avrcp_equalizer_state_e equalizer, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the repeat mode is changed by the remote control device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] repeat The repeat mode
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_repeat_mode_changed_cb()
 * @see bt_avrcp_unset_repeat_mode_changed_cb()
 */
typedef void (*bt_avrcp_repeat_mode_changed_cb) (bt_avrcp_repeat_mode_e repeat, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the shuffle mode is changed by the remote control device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] shuffle The shuffle mode
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_shuffle_mode_changed_cb()
 * @see bt_avrcp_unset_shuffle_mode_changed_cb()
 */
typedef void (*bt_avrcp_shuffle_mode_changed_cb) (bt_avrcp_shuffle_mode_e shuffle, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the scan mode is changed by the remote control device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] scan The scan mode
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_scan_mode_changed_cb()
 * @see bt_avrcp_unset_scan_mode_changed_cb()
 */
typedef void (*bt_avrcp_scan_mode_changed_cb) (bt_avrcp_scan_mode_e scan, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief  Called when the connection state is changed.
 *
 * @details  This callback is called when the connection state is changed.
 * When you call bt_hid_host_connect() or bt_hid_host_disconnect(), this callback is also called with error result even though these functions fail.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] result  The result of changing the connection state
 * @param[in] connected  The state to be changed. true means connected state, Otherwise, false.
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hid_host_connect()
 * @see bt_hid_host_disconnect()
 */
typedef void (*bt_hid_host_connection_state_changed_cb) (int result, bool connected, const char *remote_address, void *user_data);

/**
 * @deprecated Deprecated since 5.0.
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Called when the connection is established.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @remarks Deprecated, because of no usecase and supported devices.
 *
 * @param[in] result  The result of connecting to the remote device
 * @param[in] remote_address  The address of connected remote device
 * @param[in] app_id  The ID of application
 * @param[in] type  The type of HDP (Health Device Profile) channel
 * @param[in] channel  The connected data channel
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hdp_set_connection_state_changed_cb()
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
typedef void (*bt_hdp_connected_cb) (int result, const char *remote_address, const char *app_id,
	bt_hdp_channel_type_e type, unsigned int channel, void *user_data);

/**
 * @deprecated Deprecated since 5.0.
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Called when the connection is disconnected.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @remarks Deprecated, because of no usecase and supported devices.
 *
 * @param[in] result  The result of disconnecting from the remote device
 * @param[in] remote_address  The address of disconnected remote device
 * @param[in] channel  The connected data channel
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hdp_set_connection_state_changed_cb()
 * @see bt_hdp_unset_connection_state_changed_cb()
 */
typedef void (*bt_hdp_disconnected_cb) (int result, const char *remote_address, unsigned int channel, void *user_data);

/**
 * @deprecated Deprecated since 5.0.
 * @ingroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE
 * @brief  Called when the you receive the data.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @remarks Deprecated, because of no usecase and supported devices.
 *
 * @param[in] channel  The connected data channel
 * @param[in] data  The received data
 * @param[in] size  The size of received data (byte)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hdp_set_data_received_cb()
 * @see bt_hdp_unset_data_received_cb()
 */
typedef void (*bt_hdp_data_received_cb) (unsigned int channel, const char *data, unsigned int size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when you get GATT handles repeatedly.
 * @since_tizen 2.3.1
 *
 * @param[in] total The total number of GATT handles to be called
 * @param[in] index The index of current GATT handle. It starts from 0.
 * @param[in] gatt_handle The GATT handle
 * @param[in] user_data The user data passed from the foreach function
 *
 * @see bt_gatt_service_foreach_characteristics()
 * @see bt_gatt_service_foreach_included_services()
 * @see bt_gatt_characteristic_foreach_descriptors()
 * @see bt_gatt_client_foreach_services()
 */
typedef bool (*bt_gatt_foreach_cb) (int total, int index, bt_gatt_h gatt_handle,
				    void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief Attribute protocol MTU change information structure.
 * @since_tizen 4.0
 *
 * @see bt_gatt_client_att_mtu_changed_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	unsigned int mtu;		/**< MTU value */
	unsigned int status;		/**< Request status*/
} bt_gatt_client_att_mtu_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief Called when the ATT MTU value is changed.
 * @since_tizen 4.0
 *
 * @remarks The @a mtu_info must not be freed by application. \n
 *                @a mtu_info can be used only inside the callback. \n
 *                If it's needed outside, make a copy.
 *
 * @param[in] client The handle of a GATT client which is associated with a remote device
 * @param[in] mtu_info The MTU information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_gatt_client_set_connection_state_changed_cb()
 * @see bt_gatt_client_unset_connection_state_changed_cb()
 */
typedef void (*bt_gatt_client_att_mtu_changed_cb)(bt_gatt_client_h client,
		const bt_gatt_client_att_mtu_info_s *mtu_info,
		void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when the client request(e.g. read / write) has been completed.
 * @since_tizen 2.3.1
 *
 * @param[in] result The result of a request
 * @param[in] request_handle The requesting GATT handle
 * @param[in] user_data The user data passed from the requesting function
 *
 * @see bt_gatt_client_read_value()
 * @see bt_gatt_client_write_value()
 */
typedef void (*bt_gatt_client_request_completed_cb) (int result,
		bt_gatt_h request_handle, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when a value of a watched characteristic's GATT handle has been changed.
 * @since_tizen 2.3.1
 *
 * @remarks After this function is returned, a changed value is automatically \n
 * applied to @a characteristic. Before that, @a characteristic has an old value.
 *
 * @param[in] characteristic The characteristic's GATT handle of which value change is informed. It has an old value.
 * @param[in] value The new value
 * @param[in] len The length of @a value
 * @param[in] user_data The user data passed from the registering function
 *
 * @see bt_gatt_client_set_characteristic_value_changed_cb()
 */
typedef void (*bt_gatt_client_characteristic_value_changed_cb) (bt_gatt_h characteristic,
		char *value, int len, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Called when the connection state is changed.
 *
 * @details This callback is called when the connection state is changed.
 * When you called bt_gatt_connect() or bt_gatt_disconnect(), this callback is also called with error result even though these functions fail.
 *
 * @since_tizen 4.0
 * @param[in] result The result of changing the connection state.
 * @param[in] connected The state to be changed, true means connected state, Otherwise, false.
 * @param[in] remote_address The remote_address
 * @param[in] user_data The user data passed from the callback registration function.
 *
 */
typedef void (*bt_hrp_connection_state_changed_cb)(int result, bool connected, const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when a service of a remote GATT server has been changed.
 * @since_tizen 3.0
 *
 * @param[in] client The handle of a GATT client which is associated with a remote device.
 * @param[in] change_type The changed type
 * @param[in] service_uuid The changed service uuid
 * @param[in] user_data The user data passed from the registering function
 *
 * @see bt_gatt_client_set_characteristic_value_changed_cb()
 */
typedef void (*bt_gatt_client_service_changed_cb) (bt_gatt_client_h client,
		bt_gatt_client_service_change_type_e change_type,
		const char* service_uuid, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Called when the connection state is changed.
 *
 * @details This callback is called when the connection state is changed.
 * When you called bt_gatt_connect() or bt_gatt_disconnect(), this callback is also called with error result even though these functions fail.
 *
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] result The result of changing the connection state.
 * @param[in] connected The state to be changed, true means connected state, Otherwise, false.
 * @param[in] remote_address The remote_address
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @see bt_gatt_connect()
 * @see bt_gatt_disconnect()
 * @see bt_gatt_set_connection_state_changed_cb()
 * @see bt_gatt_unset_connection_state_changed_cb()
 */
typedef void (*bt_gatt_connection_state_changed_cb)(int result, bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Called when a value of a characteristic or descriptor's GATT handle has been changed.
 *
 * @details For finishing the request, call the function #bt_gatt_server_send_response in callback.
 *
 * @since_tizen 3.0
 * @remarks After this function is returned, a changed value is automatically \n
 * applied to @a gatt_handle. Before that, @a gatt_handle has an old value.
 * @remarks The @a remote_address must not be freed by application.
 * @remarks The @a server must not be freed by application.
 * @remarks The @a gatt_handle must not be freed by application.
 * @remarks The @a value must not be freed by application.
 *
 * @param[in] remote_address The address of the remote device which requests a change
 * @param[in] request_id The identification of this request. It will be used to send a response.
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic or descriptor's GATT handle which has an old value
 * @param[in] response_needed Indicates whether a response is required by the remote device - @c true if required, @c false if not
 * @param[in] offset The requested offset from where the @a gatt_handle value will be updated
 * @param[in] value The new value
 * @param[in] len The length of @a value
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_write_value_requested_cb()
 * @see bt_gatt_server_send_response()
 */
typedef void (*bt_gatt_server_write_value_requested_cb) (const char *remote_address,
				int request_id, bt_gatt_server_h server,
				bt_gatt_h gatt_handle, bool response_needed, int offset,
				const char *value, int len, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Called when the remote device requests to read a value on a GATT server.
 *
 * @details For finishing the request, call the function #bt_gatt_server_send_response in callback.
 *
 * @since_tizen 3.0
 * @remarks The @a remote_address must not be freed by application.
 * @remarks The @a server must not be freed by application.
 * @remarks The @a gatt_handle must not be freed by application.
 *
 * @param[in] remote_address The address of the requesting remote device
 * @param[in] request_id The identification of this request. It will be used to send a response.
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic or descriptor's GATT handle to be read
 * @param[in] offset The requested offset from where the GATT handle's value is read
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_read_value_requested_cb()
 * @see bt_gatt_server_send_response()
 */
typedef void (*bt_gatt_server_read_value_requested_cb) (const char *remote_address,
		int request_id, bt_gatt_server_h server, bt_gatt_h gatt_handle,
		int offset, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Called when the remote device enables or disables the Notification/Indication for particular characteristics.
 *
 * @details By using this callback function, server can know notification state.
 *
 * @since_tizen 3.0
 * @remarks The @a server must not be freed by application.
 * @remarks The @a gatt_handle must not be freed by application.
 *
 * @param[in] notify Indicates whether the Notification/Indication is enabled or not
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic's GATT handle to be read
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_read_value_requested_cb()
 */
typedef void (*bt_gatt_server_characteristic_notification_state_changed_cb) (bool notify,
			bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Called when the remote device enables or disables the Notification/Indication for particular characteristics for HRP.
 *
 * @details By using this callback function, server can know notification state.
 *
 * @since_tizen 3.0
 * @remarks The @a server must not be freed by application.
 * @remarks The @a gatt_handle must not be freed by application.
 *
 * @param[in] notify Indicates whether the Notification/Indication is enabled or not
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic's GATT handle to be read
 * @param[in] user_data The user data passed from the registration function
 *
 */
typedef void (*bt_hrp_sensor_characteristic_notification_state_changed_cb) (bool notify,
						bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Called when the sending notification / indication is done.
 * @since_tizen 3.0
 *
 * @remarks The @a remote_address must not be freed by application.
 * @remarks The @a server must not be freed by application.
 * @remarks The @a characteristic must not be freed by application.
 *
 * @remarks In case of an indication, once a confirmation is received from the remote device this callback will be called. \n
 * This callback will be called several times if there are two or more remote devices which enable a Client Characteristic Configuration Descriptor(CCCD). \n
 * For the last remote device, @a completed will be set as true.
 *
 * @param[in] result The result of a sending operation
 * @param[in] remote_address The address of the remote device
 * @param[in] server The GATT server handle
 * @param[in] characteristic The characteristic's GATT handle
 * @param[in] completed If this callback is for the last remote device which enables a CCCD, it will be true. Or it will be false.
 * @param[in] user_data The user data passed from the requesting function
 *
 * @see bt_gatt_server_notify_characteristic_changed_value()
 */
typedef void (*bt_gatt_server_notification_sent_cb) (int result,
		const char *remote_address, bt_gatt_server_h server,
		bt_gatt_h characteristic, bool completed, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] connected  Indicates whether a client is connected or disconnected
 * @param[in] remote_address  The remote address
 * @param[in] interface_name  The interface name. For example, bnep0, bnep1.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_nap_set_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
typedef void (*bt_nap_connection_state_changed_cb) (bool connected, const char *remote_address, const char *interface_name, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Called when the connection state is changed.
 *
 * @details  This callback is called when the connection state is changed.
 * When you call bt_panu_connect() or bt_panu_disconnect(), this callback is also called with error result even though these functions fail.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] result  The result of changing the connection state
 * @param[in] connected  The state to be changed. true means connected state, Otherwise, false.
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of PAN service
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_nap_set_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
typedef void (*bt_panu_connection_state_changed_cb) (int result, bool connected, const char *remote_address, bt_panu_service_type_e type, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief Enumerations of the Bluetooth HID mouse's button.
 * @since_tizen @if WEARABLE 3.0 @endif
 */
typedef enum {
	BT_HID_MOUSE_BUTTON_NONE = 0x00, /**<The mouse's none value*/
	BT_HID_MOUSE_BUTTON_LEFT = 0x01, /**<The mouse's left button value*/
	BT_HID_MOUSE_BUTTON_RIGHT = 0x02,  /**<The mouse's right button value*/
	BT_HID_MOUSE_BUTTON_MIDDLE = 0x04 /**<The mouse's middle button value*/
} bt_hid_mouse_button_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief The structure type containing the HID mouse event information.
 * @since_tizen @if WEARABLE 3.0 @endif
 *
 * @see bt_hid_device_send_mouse_event()
 */
typedef struct {
	int buttons; /**< The button values, we can combine key's values when we pressed multiple mouse buttons*/
	int axis_x; /**< The location's x value, -128 ~127 */
	int axis_y; /**< The location's y value, -128 ~127 */
	int padding; /**< The padding value, -128 ~127 */
} bt_hid_mouse_data_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief The structure type containing the HID keyboard event information.
 * @details If you want to know more detail values, refer to http://www.usb.org/developers/hidpage/ and see "HID Usage Tables"
 * @since_tizen @if WEARABLE 3.0 @endif
 *
 * @see bt_hid_device_send_key_event()
 */
typedef struct {
	unsigned char modifier; /**< The modifier keys : such as shift, alt */
	unsigned char key[8]; /**< The key value - currently pressed keys : Max 8 at once */
} bt_hid_key_data_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief Enumerations of the Bluetooth HID header type.
 * @since_tizen @if WEARABLE 3.0 @endif
 */
typedef enum {
	BT_HID_HEADER_HANDSHAKE, /**< The Bluetooth HID header type: Handshake */
	BT_HID_HEADER_HID_CONTROL, /**< The Bluetooth HID header type: HID control */
	BT_HID_HEADER_GET_REPORT, /**< The Bluetooth HID header type: Get report */
	BT_HID_HEADER_SET_REPORT, /**< The Bluetooth HID header type: Set report */
	BT_HID_HEADER_GET_PROTOCOL, /**< The Bluetooth HID header type: Get protocol */
	BT_HID_HEADER_SET_PROTOCOL, /**< The Bluetooth HID header type: Set protocol */
	BT_HID_HEADER_DATA, /**< The Bluetooth HID header type: Data */
	BT_HID_HEADER_UNKNOWN /**< The Bluetooth HID header type: Unknown */
} bt_hid_header_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief Enumerations of the Bluetooth HID parameter type.
 * @since_tizen @if WEARABLE 3.0 @endif
 */
typedef enum {
	BT_HID_PARAM_DATA_RTYPE_INPUT, /**< Parameter type: Input */
	BT_HID_PARAM_DATA_RTYPE_OUTPUT /**< Parameter type: Output */
} bt_hid_param_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief Enumerations of the Bluetooth HID handshake type.
 * @since_tizen @if WEARABLE 3.0 @endif
 */
typedef enum {
	BT_HID_HANDSHAKE_SUCCESSFUL = 0x00, /**< Handshake error code none */
	BT_HID_HANDSHAKE_NOT_READY, /**< Handshake error code Not Ready */
	BT_HID_HANDSHAKE_ERR_INVALID_REPORT_ID, /**< Handshake error code send invalid report id */
	BT_HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST, /**< Handshake error code request unsupported request */
	BT_HID_HANDSHAKE_ERR_INVALID_PARAMETER, /**< Handshake error code received invalid parameter */
	BT_HID_HANDSHAKE_ERR_UNKNOWN = 0x0e, /**< unknown error */
	BT_HID_HANDSHAKE_ERR_FATAL /**< Fatal error */
} bt_hid_handshake_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief The structure type containing data received from the HID Host.
 * @since_tizen @if WEARABLE 3.0 @endif
 */
typedef struct {
	const char *address;  /**< The remote device's address  */
	bt_hid_header_type_e header_type;  /**< The header type */
	bt_hid_param_type_e param_type;  /**< The parameter type */
	int data_size;  /**< The length of the received data */
	const char *data;     /**< The received data */
} bt_hid_device_received_data_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief  Called when the Bluetooth HID Device connection state changes.
 * @details The following error codes can be delivered: \n
 *             #BT_ERROR_NONE \n
 *             #BT_ERROR_OPERATION_FAILED \n
 * @since_tizen @if WEARABLE 3.0 @endif
 *
 * @param[in]   result  The result of changing the connection state.
 * @param[in]   connected  The requested state. true means the connection is enabled, false means the connection is disabled.
 * @param[in]   remote_address  The remote device's address
 * @param[in]   user_data  The user data passed from the callback registration function
 * @see bt_hid_device_activate()
 */
typedef void (*bt_hid_device_connection_state_changed_cb) (int result,
	bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE
 * @brief  Called when the HID Device receives data from the HID Host.
 * @details The following error codes can be delivered: \n
 *             #BT_ERROR_NONE \n
 *             #BT_ERROR_OPERATION_FAILED \n
 * @since_tizen @if WEARABLE 3.0 @endif
 *
 * @param[in]   data  The data received from the HID Host.
 * @param[in]   user_data  The user data passed from the callback registration function
 * @see bt_hid_device_set_data_received_cb()
 */
typedef void (*bt_hid_device_data_received_cb)(const bt_hid_device_received_data_s *data, void *user_data);
/* HID device related type */


/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief The structure type containing vCard information.
 * @since_tizen 3.0
 *
 * @see bt_pbap_client_pull_vcard()
 */
typedef struct {
	int index;					/**< The vcard index, used as a parameter for bt_pbap_client_pull_vcard() */
	const char *contact_name;	/**< The contact name of the vCard */
} bt_pbap_vcard_info_s;

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Called when PBAP is Connected or Disconnected.
 * @details The following error codes can be delivered: \n
 *             #BT_ERROR_NONE \n
 *             #BT_ERROR_AUTH_REJECTED \n
 *             #BT_ERROR_AUTHORIZATION_REJECTED \n
 *             #BT_ERROR_SERVICE_NOT_FOUND \n
 *             #BT_ERROR_TIMED_OUT \n
 *             #BT_ERROR_OPERATION_FAILED \n
 * @since_tizen 3.0
 *
 * @param[in] result The result of connecting to or disconnecting from the remote device
 * @param[in] connected The PBAP connection status (@c true = connected, @c false = disconnected)
 * @param[in] remote_address The remote device address (@a remote_address is valid only inside this function. To use outside the callback, make a copy. @a remote_address should not be freed.)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_pbap_client_connect()
 * @see bt_pbap_client_disconnect()
 */
typedef void (*bt_pbap_connection_state_changed_cb)(int result, bool connected,
		const char *remote_address, void *user_data);

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Called when PBAP Phonebook size calculation completes.
 * @details The following error codes can be delivered: \n
 *             #BT_ERROR_NONE \n
 *             #BT_ERROR_OPERATION_FAILED \n
 * @since_tizen 3.0
 *
 * @param[in] result The result of getting the phone book size
 * @param[in] remote_address The remote device address (@a remote_address is valid only inside this function. To use outside the callback, make a copy. @a remote_address should not be freed.)
 * @param[in] size Size of Phonebook
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_pbap_client_get_phone_book_size()
 * @see bt_pbap_client_connect()
 * @see bt_pbap_client_disconnect()
 */
typedef void (*bt_pbap_phone_book_size_cb)(int result, const char *remote_address,
		int size, void *user_data);

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Called when PBAP Phonebook Pull completes.
 * @details The received phone book file will be saved in the platform downloads folder. \n
 *              The following error codes can be delivered: \n
 *             #BT_ERROR_NONE \n
 *             #BT_ERROR_OPERATION_FAILED \n
 * @since_tizen 3.0
 *
 * @param[in] result The result of getting the phone book
 * @param[in] remote_address The remote device address (@a remote_address is valid only inside this function. To use outside the callback, make a copy. @a remote_address should not be freed.)
 * @param[in] vcf_file The absolute path of the file in which the vCards are saved (@a vcf_file is valid only inside this function. To use outside the callback, make a copy. @a vcf_file should not be freed.)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_pbap_client_get_phone_book()
 * @see bt_pbap_client_connect()
 * @see bt_pbap_client_disconnect()
 */
typedef void (*bt_pbap_phone_book_received_cb)(int result, const char *remote_address,
		const char *vcf_file, void *user_data);

/**
 * @WEARABLE_ONLY
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief  Called when PBAP List vCards completes.
 * @details The following error codes can be delivered: \n
 *             #BT_ERROR_NONE \n
 *             #BT_ERROR_OPERATION_FAILED \n
 * @since_tizen 3.0
 *
 * @param[in] result The result of getting the vcard list
 * @param[in] remote_address The remote device address (@a remote_address is valid only inside this function. To use outside the callback, make a copy. @a remote_address should not be freed.)
 * @param[in] vcard_info List of vcard information (@a vcard_info is valid only inside this function. To use outside the callback, make a copy. @a vcard_info should not be freed.)
 * @param[in] count Number of contacts in the list
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_pbap_client_get_list()
 * @see bt_pbap_client_connect()
 * @see bt_pbap_client_disconnect()
 */
typedef void (*bt_pbap_list_vcards_cb)(int result, const char *remote_address,
		const bt_pbap_vcard_info_s *vcard_info, int count, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Enumeration for the scan filter type.
 * @since_tizen 4.0
 * @see bt_adapter_le_scan_filter_set_type()
 */
typedef enum {
	BT_ADAPTER_LE_SCAN_FILTER_TYPE_IBEACON, /**< iBeacon filter type */
	BT_ADAPTER_LE_SCAN_FILTER_TYPE_PROXIMITY_UUID, /**< Proximity UUID filter type */
} bt_adapter_le_scan_filter_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle of a Bluetooth LE scan filter.
 * @since_tizen 4.0
 */
typedef void *bt_scan_filter_h;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_NETWORK_BLUETOOTH_TYPE_H__ */
