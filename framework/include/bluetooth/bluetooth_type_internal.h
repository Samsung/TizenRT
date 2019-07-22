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

#ifndef __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__
#define __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__

 #ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef GLIB_SUPPORTED
#include <glib.h>
#endif

/* This variable will be added into bt_service_class_t in tizen 4.0 */
#define BT_SC_MAP_SERVICE_MASK 0x00800000 /**< MAP service class */

/**
 * @file bluetooth_type_internal.h
 */

#define BT_PROFILE_SERVICE_UUID_SPP		"1101"
#define BT_PROFILE_SERVICE_UUID_HSP_HS		"1108"
#define BT_PROFILE_SERVICE_UUID_HSP_AG		"1112"
#define BT_PROFILE_SERVICE_UUID_OPP		"1105"
#define BT_PROFILE_SERVICE_UUID_HFP_HF		"111e"
#define BT_PROFILE_SERVICE_UUID_HFP_AG		"111f"
#define BT_PROFILE_SERVICE_UUID_A2DP_SOURCE	"110a"
#define BT_PROFILE_SERVICE_UUID_A2DP_SINK	"110b"
#define BT_PROFILE_SERVICE_UUID_AVRCP_TARGET	"110c"
#define BT_PROFILE_SERVICE_UUID_AVRCP_CONTROL	"110f"
#define BT_PROFILE_SERVICE_UUID_PAN_PANU	"1115"
#define BT_PROFILE_SERVICE_UUID_PAN_NAP		"1116"
#define BT_PROFILE_SERVICE_UUID_PAN_GN		"1117"
#define BT_PROFILE_SERVICE_UUID_HDP_SOURCE	"1401"
#define BT_PROFILE_SERVICE_UUID_HDP_SINK	"1402"
#define BT_PROFILE_SERVICE_UUID_HID		"1124"

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth adapter le state.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_LE_DISABLED = 0x00, /**< Bluetooth le is disabled */
	BT_ADAPTER_LE_ENABLED, /**< Bluetooth le is enabled */
} bt_adapter_le_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the ATT error codes.
 * @since_tizen 4.0
 */
typedef enum {
	BT_ATT_ERROR_INTERNAL = -1,
	BT_ATT_ERROR_NONE =	0x00,

	/* Attribute error codes */
	BT_ATT_ERROR_INVALID_HANDLE = 0x01,
	BT_ATT_ERROR_READ_NOT_PERMITTED = 0x02,
	BT_ATT_ERROR_WRITE_NOT_PERMITTED = 0x03,
	BT_ATT_ERROR_INVALID_PDU = 0x04,
	BT_ATT_ERROR_AUTHENTICATION = 0x05,
	BT_ATT_ERROR_REQUEST_NOT_SUPPORTED = 0x06,
	BT_ATT_ERROR_INVALID_OFFSET = 0x07,
	BT_ATT_ERROR_AUTHORIZATION = 0x08,
	BT_ATT_ERROR_PREPARE_QUEUE_FULL = 0x09,
	BT_ATT_ERROR_ATTRIBUTE_NOT_FOUND = 0x0A,
	BT_ATT_ERROR_ATTRIBUTE_NOT_LONG = 0x0B,
	BT_ATT_ERROR_INSUFFICIENT_ENCRYPTION_KEY_SIZE = 0x0C,
	BT_ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LEN = 0x0D,
	BT_ATT_ERROR_UNLIKELY = 0x0E,
	BT_ATT_ERROR_INSUFFICIENT_ENCRYPTION = 0x0F,
	BT_ATT_ERROR_UNSUPPORTED_GROUP_TYPE = 0x10,
	BT_ATT_ERROR_INSUFFICIENT_RESOURCES = 0x11,

	/* Common profile error codes */
	BT_ATT_ERROR_WRITE_REQUEST_REJECTED = 0xFC,
	BT_ATT_ERROR_CCCD_IMPROPERLY_CONFIGURED = 0xFD,
	BT_ATT_ERROR_PROCEDURE_ALREADY_IN_PROGRESS = 0xFE,
	BT_ATT_ERROR_OUT_OF_RANGE = 0xFF,

} bt_att_error_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OTP_MODULE
 * @brief  Enumerations of the ATT error codes specific to OTP.
 * @since_tizen 4.0
 */
typedef enum {
	/* Bluetooth OTP error codes */
	BT_OTP_ERROR_WRITE_REQUEST_REJECTED = 0x80,
	BT_OTP_ERROR_OBJECT_NOT_SELECTED = 0x81,
	BT_OTP_ERROR_CONCURRENCY_LIMIT_EXCEEDED = 0x82,
	BT_OTP_ERROR_OBJECT_NAME_EXISTS = 0x83,
} bt_otp_error_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the Bluetooth adapter le state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in]   result  The result of the adapter state changing
 * @param[in]   adapter_le_state  The adapter le state to be changed
 * @param[in]   user_data  The user data passed from the callback registration function
 * @pre Either bt_adapter_le_enable() or bt_adapter_le_disable() will invoke this callback if you register this callback using bt_adapter_le_set_state_changed_cb().
 * @see bt_adapter_le_enable()
 * @see bt_adapter_le_disable()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 */
typedef void (*bt_adapter_le_state_changed_cb)(int result, bt_adapter_le_state_e adapter_le_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the delay is changed by the remote device.
 * @since_tizen 5.0
 * @param[in] delay Streaming delay in milliseconds.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_delay_changed_cb()
 * @see bt_avrcp_unset_delay_changed_cb()
 */
typedef void (*bt_avrcp_delay_changed_cb) (unsigned int delay, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_AG_CALL_EVENT_IDLE = 0x00,  /**< Idle */
	BT_AG_CALL_EVENT_ANSWERED,  /**< Answered */
	BT_AG_CALL_EVENT_HELD,  /**< Held */
	BT_AG_CALL_EVENT_RETRIEVED,  /**< Retrieved */
	BT_AG_CALL_EVENT_DIALING,  /**< Dialing */
	BT_AG_CALL_EVENT_ALERTING,  /**< Alerting */
	BT_AG_CALL_EVENT_INCOMING,  /**< Incoming */
} bt_ag_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_AG_CALL_STATE_IDLE = 0x00,  /**< Idle state */
	BT_AG_CALL_STATE_ACTIVE,  /**< Active state */
	BT_AG_CALL_STATE_HELD,  /**< Held state */
	BT_AG_CALL_STATE_DIALING,  /**< Dialing state */
	BT_AG_CALL_STATE_ALERTING,  /**< Alerting state */
	BT_AG_CALL_STATE_INCOMING,  /**< Incoming state */
	BT_AG_CALL_STATE_WAITING,  /**< Waiting for connected indication event after answering an incoming call*/
} bt_ag_call_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the call event from Audio-Gateway device
 * @since_tizen @if WEARABLE 3.0 @endif
 */
typedef enum {
	BT_HF_REMOTE_CALL_EVENT_IDLE = 0x00,  /**< Idle. Neither setup call nor active call exist */
	BT_HF_REMOTE_CALL_EVENT_INCOMING,  /**< (Call-setup event) Received an incoming call on AG */
	BT_HF_REMOTE_CALL_EVENT_DIALING,  /**< (Call-setup event) Dialing an outgoing call on AG */
	BT_HF_REMOTE_CALL_EVENT_ALERTING,  /**< (Call-setup event) Remote party being alerted in an outgoing call of AG */
	BT_HF_REMOTE_CALL_EVENT_CALL_TERMINATED,  /**< (Call-setup event) Setup call is terminated without activating */
	BT_HF_REMOTE_CALL_EVENT_CALL_STARTED,  /**< (Active call state event) Call is started on AG */
	BT_HF_REMOTE_CALL_EVENT_CALL_ENDED,  /**< (Active call state event) Active call is terminated on AG */
	BT_HF_REMOTE_CALL_EVENT_UNHELD,  /**< (Call held event) No calls on hold */
	BT_HF_REMOTE_CALL_EVENT_SWAPPED,  /**< (Call held event) Call is placed on hold or active/held calls swapped */
	BT_HF_REMOTE_CALL_EVENT_HELD,  /**< (Call held event) Calls on hold, no active call */
	BT_HF_REMOTE_CALL_EVENT_RINGING,  /**< Incoming call is ringing event with number. This event is optional event. */
	BT_HF_REMOTE_CALL_EVENT_WAITING,  /**< Call Waiting notification in 3-way call scenario */
	BT_HF_REMOTE_CALL_EVENT_FAILED_TO_DIALING,  /**< Failed to dialing a outgoing call on AG */
} bt_hf_remote_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_MODULE
 * @brief  Enumerations for the transfer type
 * @since_tizen 3.0
 */
typedef enum {
	BT_TRANSFER_INBOUND,	    /**< Inbound Transfer Type */
	BT_TRANSFER_OUTBOUND,	    /**< Outbound Transfer Type */
} bt_opp_transfer_type_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Enumerations of the Bluetooth A2DP role.
 * @since_tizen 4.0
 */
typedef enum {
	BT_A2DP_SOURCE,
	BT_A2DP_SINK,
} bt_audio_role_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MAP_CLIENT_MODULE
 * @brief
 * @since_tizen 3.0
 */
typedef void* bt_map_client_session_info_h;

typedef void* bt_map_client_message_object_h;

typedef struct {
	int16_t offset;
	int16_t max_count;
} bt_map_client_list_folders_filter_s;

typedef struct {
	int16_t offset;
	int16_t max_count;
	int8_t subject_length;
	char *fields;
	char *types;
	char *period_begin;
	char *period_end;
	int is_read;
	char *recipient;
	char *sender;
	int is_priority;
} bt_map_client_list_messages_filter_s;

typedef struct {
	bt_map_client_message_object_h message_object;
	char *folder;
	char *subject;
	char *timestamp;
	char *sender;
	char *sender_address;
	char *reply_to;
	char *recipient;
	char *recipient_address;
	char *type;
	int64_t size;
	int is_text;
	char *status;
	int64_t attachment_size;
	int is_priority;
	int is_read;
	int is_sent;
	int is_protected;
} bt_map_client_message_item_s;

typedef struct {
	int is_transparent;
	int is_retry;
	char *charset;
} bt_map_client_push_message_args_s;

typedef struct {
	char *file_path;
// TODO: maybe some additional fields could be supported
//	char *folder;
//	char *subject;
//	char *timestamp;
//	char *sender;
//	char *sender_address;
//	char *reply_to;
//	char *recipient;
//	char *recipient_address;
//	char *type;
//	int64_t size;
//	char *status;
//	int is_priority;
//	int is_read;
//	int is_deleted;
//	int is_sent;
//	int is_protected;
} bt_map_client_message_s;

typedef bt_map_client_list_folders_filter_s *bt_map_client_list_folders_filter_h;

typedef bt_map_client_list_messages_filter_s *bt_map_client_list_messages_filter_h;

typedef bt_map_client_message_item_s *bt_map_client_message_item_h;

typedef bt_map_client_push_message_args_s *bt_map_client_push_message_args_h;

typedef bt_map_client_message_s *bt_map_client_message_h;

typedef void (*bt_map_client_list_folders_cb)(int result, char **folders, int count, void *user_data);

typedef void (*bt_map_client_list_filter_fields_cb)(int result, char **filter_fields, int count, void *user_data);

typedef void (*bt_map_client_list_messages_cb)(int result, bt_map_client_message_item_s *messages, int count, void *user_data);

typedef void (*bt_map_client_push_message_cb)(int result, void *user_data);

typedef void (*bt_map_client_get_message_cb)(int result, bt_map_client_message_h message, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the connectable state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] result The result of the connectable state changing
 * @param[in] connectable The connectable to be changed
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre This function will be invoked when the connectable state of local Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_connectable_changed_cb().
 *
 * @see bt_adapter_set_connectable()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
typedef void (*bt_adapter_connectable_changed_cb)
	(int result, bool connectable, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when the push is requested.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @details You must call bt_opp_server_accept() if you want to accept.
 * Otherwise, you must call bt_opp_server_reject().
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_initialize()
 */
typedef void (*bt_opp_server_push_requested_cb)(const char *file, int size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth adapter le scan type.
 * @since_tizen 2.3
 */
typedef enum {
	BT_ADAPTER_LE_PASSIVE_SCAN = 0x00,
	BT_ADAPTER_LE_ACTIVE_SCAN
} bt_adapter_le_scan_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the call handling event
 * @since_tizen @if WEARABLE 2.3 @endif
 */
typedef enum {
	BT_HF_CALL_HANDLING_EVENT_ANSWER = 0x00,  /**< Request to answer an incoming call */
	BT_HF_CALL_HANDLING_EVENT_RELEASE,  /**< Request to release a call */
	BT_HF_CALL_HANDLING_EVENT_REJECT,  /**< Request to reject an incoming call */
	BT_HF_CALL_HANDLING_EVENT_RING,  /**< Request of ringing call */
	BT_HF_CALL_HANDLING_EVENT_CALL_STARTED,  /**< Request of Call started */
	BT_HF_CALL_HANDLING_EVENT_CALL_ENDED,  /**< Request of Call Ended */
	BT_HF_CALL_HANDLING_EVENT_VOICE_RECOGNITION_ENABLED,  /**< Request of voice recognition enabled */
	BT_HF_CALL_HANDLING_EVENT_VOICE_RECOGNITION_DISABLED,  /**< Request of voice recognition disabled */
	BT_HF_CALL_HANDLING_EVENT_VENDOR_DEP_CMD,  /**< Request of Vendor command */
	BT_HF_CALL_HANDLING_EVENT_WAITING,  /**< Request to waiting a call */
	BT_HF_CALL_HANDLING_EVENT_HELD,  /**< Request to hold a call */
	BT_HF_CALL_HANDLING_EVENT_UNHELD,  /**< Request to unhold calls */
	BT_HF_CALL_HANDLING_EVENT_SWAPPED,  /**< Request to swap calls */
} bt_hf_call_handling_event_e;


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the multi call handling event
 * @since_tizen @if WEARABLE 2.3 @endif
 */
typedef enum {
	BT_HF_MULTI_CALL_HANDLING_EVENT_RELEASE_HELD_CALLS = 0x00,  /**< Request to release held calls */
	BT_HF_MULTI_CALL_HANDLING_EVENT_RELEASE_ACTIVE_CALLS,  /**< Request to release active calls */
	BT_HF_MULTI_CALL_HANDLING_EVENT_ACTIVATE_HELD_CALL,  /**< Request to put active calls into hold state and activate another (held or waiting) call */
	BT_HF_MULTI_CALL_HANDLING_EVENT_MERGE_CALLS,  /**< Request to add a held call to the conversation */
	BT_HF_MULTI_CALL_HANDLING_EVENT_EXPLICIT_CALL_TRANSFER,  /**< Request to let a user who has two calls to connect these two calls together and release its connections to both other parties */
} bt_hf_multi_call_handling_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen @if WEARABLE 2.3 @endif
 */
typedef enum {
	BT_HF_CALL_EVENT_IDLE = 0x00,  /**< Idle */
	BT_HF_CALL_EVENT_ANSWER,  /**< Answered */
	BT_HF_CALL_EVENT_HOLD,  /**< Held */
	BT_HF_CALL_EVENT_RETRIEVE,  /**< Retrieved */
	BT_HF_CALL_EVENT_DIAL,  /**< Dialing */
	BT_HF_CALL_EVENT_ALERT,  /**< Alerting */
	BT_HF_CALL_EVENT_INCOMING,  /**< Incoming */
	BT_HF_CALL_EVENT_REDIAL,  /**< Redialling */
	BT_HF_CALL_EVENT_RELEASE_ALL_NONACTIVE_CALLS,  /**< Release all nonactive calls */
	BT_HF_CALL_EVENT_ACCEPT_AND_RELEASE,  /**< Accept and Release */
	BT_HF_CALL_EVENT_ACCEPT_AND_HOLD,  /**< Accept and Hold */
	BT_HF_CALL_EVENT_ADD_TO_CONVERSATION,  /**< Add to the conversation */
} bt_hf_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  HF Call status information.
 * @since_tizen @if WEARABLE 2.3 @endif
 */
typedef struct {
	char *number;  /**< Phone Number */
	int direction;  /**< Direction :Incoming(1), Outgoing(0) */
	int status;  /**< Call Status :Active(0), Held(1), Waiting(5), Dailing(2) */
	int multi_party;  /**< Multiparty/conf call: Yes(1), No(0) */
	int index;  /**< Call index/ID */
} bt_hf_call_status_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the manufacturer dat changes.
 * @since_tizen 2.3
 *
 * @param[in]   data		The manufacurer data of the Bluetooth device to be changed
 * @param[in]   len			The length of @a data
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when the manufacturer data of Bluetooth adapter changes
 * if callback is registered using bt_adapter_set_manufacturer_data_changed_cb().
 * @see bt_adapter_set_manufacturer_data()
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
typedef void (*bt_adapter_manufacturer_data_changed_cb) (char *data,
		int len, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called repeatedly when you get the devices connected with specific profile.
 * @since_tizen 3.0
 *
 * @param[in]   remote_address	The address of remote device
 * @param[in]   user_data	The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre bt_adapter_foreach_profile_connected_devices() will invoke this function.
 * @see bt_adapter_foreach_profile_connected_devices()
 */
typedef bool (*bt_adapter_profile_connected_devices_cb)(const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when the SCO(Synchronous Connection Oriented link) state is changed.
 * @since_tizen @if WEARABLE 2.3 @endif
 *
 * @details  This callback is called when the SCO state is changed.
 * When you call bt_ag_open_sco() or bt_ag_close_sco(), this callback is also called with error result even though these functions failed.
 * @param[in] result  The result of changing the connection state
 * @param[in] opened  The state to be changed: (@c true = opened, @c  false = not opened)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 * @see bt_ag_open_sco()
 * @see bt_ag_close_sco()
 */
typedef void (*bt_hf_sco_state_changed_cb) (int result, bool opened, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when a call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3 @endif
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] call_id  The call ID
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
typedef void (*bt_hf_call_handling_event_cb) (bt_hf_call_handling_event_e event, char *phone_number, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when a multi call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3 @endif
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
typedef void (*bt_hf_multi_call_handling_event_cb) (bt_hf_multi_call_handling_event_e event, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when the speaker gain of the remote device is changed.
 * @since_tizen @if WEARABLE 2.3 @endif
 *
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hf_set_speaker_gain_changed_cb()
 * @see bt_hf_unset_speaker_gain_changed_cb()
 */
typedef void (*bt_hf_speaker_gain_changed_cb) (int gain, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called when a call status updated event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3 @endif
 * @remarks call_info_list has elements which consist of bt_hf_call_status_info_s
 * @remarks The @a call_info_list must be released with bt_hf_free_call_status_info_list() by you.
 *
 * @param[in] event  The call handling event happened from Hands-Free
 * @param[in] call_id  The call ID
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_hf_call_status_info_s
 * @see bt_hf_set_call_status_updated_event_cb()
 * @see bt_hf_unset_call_status_updated_event_cb()
 */
#ifdef GLIB_SUPPORTED
typedef void (*bt_hf_call_status_updated_event_cb) (GSList *call_info_list, void *user_data);
#endif

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
* @brief DPM BT allowance state
* @since_tizen 3.0
*/
typedef enum {
	BT_DPM_ERROR	  = -1,   /**< bluetooth allowance error */
	BT_DPM_BT_ALLOWED,	  /**< bluetooth allowance allowed */
	BT_DPM_HANDSFREE_ONLY, /**< bluetooth allowance handsfree only */
	BT_DPM_BT_RESTRICTED,  /**< bluetooth allowance restricted */
} bt_dpm_allow_e;

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
* @brief DPM Policy status
* @since_tizen 3.0
*/
typedef enum {
	BT_DPM_STATUS_ERROR = -1,

	BT_DPM_ALLOWED	 = 0,	 /**< DPM Policy status allowed. */
	BT_DPM_RESTRICTED		 = 1,	 /**< DPM Policy status restricted. */

	BT_DPM_ENABLE			 = 1,	 /**< DPM Policy status enabled. */
	BT_DPM_DISABLE	= 0,	 /**< DPM Policy status disabled. */

	BT_DPM_FALSE	 = 0,	/**< DPM Policy status false. */
	BT_DPM_TRUE		= 1,	/**< DPM Policy status true. */
} bt_dpm_status_e;

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
* @brief DPM Profile states
* @since_tizen 3.0
*/
typedef enum {
	BT_DPM_POLICY_A2DP_PROFILE_STATE,
	BT_DPM_POLICY_AVRCP_PROFILE_STATE,
	BT_DPM_POLICY_BPP_PROFILE_STATE,
	BT_DPM_POLICY_DUN_PROFILE_STATE,
	BT_DPM_POLICY_FTP_PROFILE_STATE,
	BT_DPM_POLICY_HFP_PROFILE_STATE,
	BT_DPM_POLICY_HSP_PROFILE_STATE,
	BT_DPM_POLICY_PBAP_PROFILE_STATE,
	BT_DPM_POLICY_SAP_PROFILE_STATE,
	BT_DPM_POLICY_SPP_PROFILE_STATE,
	BT_DPM_PROFILE_NONE,
} bt_dpm_profile_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device LE connection update structure.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef struct {
	unsigned int interval_min;   /**< Minimum value for the connection event interval (msec) */
	unsigned int interval_max;   /**< Maximum value for the connection event interval (msec) */
	unsigned int latency;   /**< Slave latency (msec) */
	unsigned int time_out;   /**< Supervision timeout (msec) */
} bt_le_conn_update_s;

/**
 * Structure to DPM device list
 */
typedef struct {
	int count;
	char **devices;
} bt_dpm_device_list_s;

/**
 * Structure to DPM uuid list
 */
typedef struct {
	int count;
	char **uuids;
} bt_dpm_uuids_list_s;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumaration for the address type field in manufactrer specific data
 * @since_tizen 3.0
 */
typedef enum {
	ADDRESS_NONE_TYPE = 0x00,
	WI_FI_P2P_ADDRESS = 0x01,
	BLUETOOTH_ADDRESS = 0x02,
	INDICATION_ADDRESS = 0x04,
	IPV4_ADDRESS = 0x08,
	IPV6_ADDRESS = 0x10,
	UNKNOWN_ADDRESS = 0xff
} connectivity_address_t;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumaration for the proximity locality type field in manufactrer specific data
 * @since_tizen 3.0
 */
typedef enum {
	NONE_TYPE = 0x00,
	PROXIMITY = 0x01,
	CO_PRESENCE = 0x02,
	UNKNOWN_TYPE = 0xff
} bt_proximity_locality_t;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Structure of samsung specific manufacturer data
 * @since_tizen 3.0
 */
typedef struct {
	unsigned char version;
	unsigned char service_id;
	unsigned char discovery_version;
	unsigned char associated_service_id;
	bt_proximity_locality_t proximity_locality_type;
	unsigned char proximity_locality_info;
	unsigned char device_type;
	unsigned char device_icon;
	unsigned char auth_info[5];
	connectivity_address_t addr_type;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char channel_info;
	unsigned char associated_service_data_len;
	unsigned char *associated_service_data_val;
	char *name;
} bt_manufacturer_data;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Trusted Profile enumeration.
 * @since_tizen 3.0
 *
 * @see bt_device_set_profile_trusted()
 * @see bt_device_get_profile_trusted()
 */
typedef enum {
	BT_TRUSTED_PROFILE_PBAP = 1,
	BT_TRUSTED_PROFILE_MAP,
	BT_TRUSTED_PROFILE_SAP,
	BT_TRUSTED_PROFILE_ALL = 0xFFFFFFFF,
} bt_trusted_profile_t;

/**
 * @deprecated Deprecated since 4.0. Use bt_trusted_profile_t instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Restricted Profile enumeration.
 * @since_tizen 3.0
 *
 * @see bt_device_set_profile_restricted()
 * @see bt_device_get_profile_restricted()
 */
typedef enum {
	BT_RESTRICTED_PROFILE_HFP_HS = 1,
	BT_RESTRICTED_PROFILE_A2DP,
} bt_restricted_profile_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumerations of the authentication event types.
 * @since_tizen 3.0
 *
 */
typedef enum {
	BT_AUTH_KEYBOARD_PASSKEY_DISPLAY = 0, /**< PIN display event to user for entering PIN in keyboard */
	BT_AUTH_PIN_REQUEST,                  /**< Legacy PIN or PASSKEY request event */
	BT_AUTH_PASSKEY_CONFIRM_REQUEST,      /**< PASSKEY confirmation event to match PASSKEY in remote device */
} bt_authentication_type_info_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Proximity Profile Role
 * @since_tizen 3.0
 */
typedef enum {
	BT_PROXIMITY_REPORTER, /**< Proximity Profile Reporter role. */
	BT_PROXIMITY_MONITOR, /**< Proximity Profile Monitor role. */
} bt_proximity_role_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Proximity Profile Role
 * @since_tizen 3.0
 */
typedef enum {
	BT_PROXIMITY_LINKLOSS_ALERT = 0x01, /**< Proximity profile Link Loss alert property */
	BT_PROXIMITY_IMMEDIATE_ALERT = 0x02, /**< Proximity Profile Immediate alert property */
	BT_PROXIMITY_TX_POWER = 0x04, /**< Proximity Profile Immediate alert property */
} bt_proximity_property_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when Trusted Profiles is changed.
 * @since_tizen 3.0
 *
 * @param[in]   result  The result of supported profile callback
 * @param[in]   remote_address  Address of remote device
 * @param[in]   trusted_profiles  Trusted profile FLAG
 * @param[in]   user_data  The user data passed from the callback registration function
 * @see bt_device_set_trusted_profile_cb()
 * @see bt_device_unset_trusted_profile_cb()
 */
typedef void (*bt_device_trusted_profiles_cb)
	(int result, char *remote_address, int trusted_profile, bool supported, bool trusted, void *user_data);

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_att_mtu_info_s instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Attribute protocol MTU change information structure.
 * @since_tizen 3.0
 *
 * @see bt_device_att_mtu_changed_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	unsigned int mtu;		/** < MTU value */
	unsigned int status;		/** < request status*/
} bt_device_att_mtu_info_s;

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_att_mtu_changed_cb instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen 3.0
 *
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] conn_info The connection information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_device_set_connection_state_changed_cb()
 * @see bt_device_unset_connection_state_changed_cb()
 */
typedef void (*bt_device_att_mtu_changed_cb)(int result, bt_device_att_mtu_info_s *mtu_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief The handle of a Proximity Monitor client handle which is associated with a remote device.
 * @since_tizen 4.0
 */
typedef void *bt_proximity_monitor_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief The handle of a Proximity Reporter server.
 * @since_tizen 4.0
 */
typedef void *bt_proximity_reporter_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief  Called when the Proximity profile reporter property is changed.
 * @since_tizen 4.0
 *
 * @param[in] result callback result
 * @param[in] remote_address remote device address on which property is changed
 * @param[in] service_type The property type for which the value is changed
 * @param[in] prop_value The property value changed
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_proximity_reporter_set_property_changed_cb()
 * @see bt_proximity_reporter_unset_property_changed_cb()
 */
typedef void (*bt_proximity_reporter_property_changed_cb)
		(int result, const char *remote_address, int service_type, int prop_value, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief  Called when the Proximity monitor profile connection state is changed.
 * @since_tizen 4.0
 *
 * @param[in] result callback result
 * @param[in] remote_address remote device address to which the device is connected
 * @param[in] monitor proximity profile monitor handle
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] supported_service supported services of proximity profile
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_proximity_monitor_set_connection_state_changed_cb()
 * @see bt_proximity_monitor_unset_connection_state_changed_cb()
 */
typedef void (*bt_proximity_monitor_connection_state_changed_cb)
		(int result, const char *remote_address, bt_proximity_monitor_h monitor, bool connected, int supported_service, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief  Called when the Proximity reporter profile connection state is changed.
 * @since_tizen 4.0
 *
 * @param[in] result callback result
 * @param[in] remote_address remote device address to which the device is connected
 * @param[in] reporter proximity profile reporter handle
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] supported_service supported services of proximity profile
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_proximity_reporter_set_connection_state_changed_cb()
 * @see bt_proximity_reporter_unset_connection_state_changed_cb()
 */
typedef void (*bt_proximity_reporter_connection_state_changed_cb)
		(int result, const char *remote_address, bt_proximity_reporter_h reporter, bool connected, int supported_service, void *user_data);


/**
 * @internal
 * @brief Various TDS transport states.
 * @since_tizen 3.0
 */
typedef enum {
	BT_TDS_TRANSPORT_STATE_OFF,	/**< Transport is currently in OFF state */
	BT_TDS_TRANSPORT_STATE_ON,	/**< Transport is currently in ON state */
	BT_TDS_TRANSPORT_STATE_UNAVAILABLE,	/**< Transport is temporarily unavailable */
} bt_tds_transport_state_e;

/**
 * @internal
 * @brief Various TDS transports.
 * @since_tizen 3.0
 */
typedef enum {
	BT_TDS_TRANSPORT_BT = 0x01, /* Transport BR-EDR */
	BT_TDS_TRANSPORT_CUSTOM,       /* Transport custom */
	/* ... */
	BT_TDS_TRANSPORT_INVALID
} bt_tds_transport_e;

/**
 * @internal
 * @brief The structure type of TDS transport data block
 * @since_tizen 3.0
 */
typedef struct {
	bt_tds_transport_e transport;
	bt_tds_transport_state_e state;
	bool is_data_complete;
	char *data;
	int length;
} tds_transport_data_s;

/**
 * @internal
 * @brief The structure type of list of TDS transport data block
 * @since_tizen 3.0
 */
typedef struct {
	int num_transport_block;            /**< Number of Transport Data Blocks */
	tds_transport_data_s **data;        /**< Array of Transport Data Block */
} bt_tds_transport_block_list_s;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Called when remote device requests authentication.
 * @since_tizen 3.0
 * @param[in] result
 * @param[in] auth_type
 *             typedef enum {
 *              BT_AUTH_KEYBOARD_PASSKEY_DISPLAY = 0, : PIN display event to user for entering PIN in keyboard
 *              BT_AUTH_PIN_REQUEST,                  : Legacy PIN or PASSKEY request event
 *              BT_AUTH_PASSKEY_CONFIRM_REQUEST,      : PASSKEY confirmation event to match PASSKEY in remote device
 *             } bt_authentication_type_info_e;
 * @param[in] device_name  Name of the remote device
 * @param[in] remote_addr  Remote BD address
 * @param[in] pass_key     PASSKEY string
 *            PASSKEY string is valid only if authentication types are following
 *             a/ BT_AUTH_KEYBOARD_PASSKEY_DISPLAY
 *             b/ BT_AUTH_PASSKEY_CONFIRM_REQUEST
 *            pass_key string will be invalid if authentication event is of type BT_AUTH_PIN_REQUEST
 *            as this event indicates that user MUST enter PIN or PASSKEY and perform authentication.
 *
 *            Upon receiving BT_AUTH_KEYBOARD_PASSKEY_DISPLAY event, user should enter PASSKEY in keyboard
 *            Application can also call bt_device_cancel_bonding() Upon receiving BT_AUTH_KEYBOARD_PASSKEY_DISPLAY
 *            event which will fail the on-going pairing with remote device.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_adapter_set_authentication_req_cb()
 */
typedef void (*bt_adapter_authentication_req_cb)(int result, bt_authentication_type_info_e auth_type,
						char *device_name, char *remote_addr,
						char *pass_key, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Samsung XSAT Vendor dependent command.
 * @since_tizen 2.3.2
 */
typedef struct {
	int app_id; /**< The application ID */
	char *message; /**< Command message */
} bt_hf_vendor_dep_at_cmd_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Called  when a call event happend from Audio-Gateway device
 * @since_tizen @if WEARABLE 3.0 @endif
 *
 * @param[in] event The call state chagned event from remote Audio-Gateway device
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see bt_hf_set_remote_call_event_cb()
 * @see bt_hf_unset_remote_call_event_cb()
 */
typedef void (*bt_hf_remote_call_event_cb) (bt_hf_remote_call_event_e event, char *phone_number, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a XSAT vendor command is transmitted from Hands-Free.
 * @since_tizen 2.3
 *
 * @param[in] command  The XSAT vendor command transmitted from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_vendor_cmd_cb()
 * @see bt_ag_unset_vendor_cmd_cb()
 */
typedef void (*bt_ag_vendor_cmd_cb) (char *command, void *user_data);

/**
 * @internal
 * @brief The handle of a Transport Discover Service Provider.
 * @since_tizen 3.0
 */
typedef void *bt_tds_provider_h;

/**
 * @internal
 * @brief Called when remote device requests transport activation
 * @since_tizen 3.0
 * @param[in] remote_bd_addr Remote device address
 * @param[in] transport transport to be activated
 * @param[in] buf transport activation request data buffer
 * @param[in] len transport activation request data buffer length
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see bt_tds_set_transport_activation_requested_cb()
 */
typedef void (*bt_tds_activation_requested_cb)(char *remote_bd_addr,
	bt_tds_transport_e transport, unsigned char *buf, int len, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle of a TDS Seeker client which is associated with a remote TDS provider
 * @since_tizen 3.0
 */
typedef void *bt_tds_seeker_h;

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile Scan result callback containing filtered TDS service information and
 * complete LE discovery informations
 */
typedef void (*bt_tds_provider_scan_result_cb)(int result, const char *remote_address,
		bt_tds_transport_block_list_s *info, bt_adapter_le_device_scan_result_info_s *scan_info,
		void *user_data);

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile Connection State changed callback which is associated with a remote TDS provider
 */
typedef void (*bt_tds_seeker_connection_state_changed_cb)
		(int result, const char *remote_address, bt_tds_seeker_h seeker, bool connected, void *user_data);

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile complete TDS data block read callback from remote TDS provider
 */
typedef void (*bt_tds_seeker_complete_transport_data_cb)
		(int result, const char *remote_address, bt_tds_transport_block_list_s *info, void *user_data);

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile TDS Control Point Activation request callback which is associated with remote TDS provider
 */
typedef void (*bt_tds_control_point_activation_indication_cb)
		(int result, const char *remote_address, unsigned char *data, int data_length, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief  Called when OTP Server is Enabled or Disabled.
 */
typedef void (*bt_otp_server_state_changed_cb)(int result, bool connected);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle of a OTP client which is associated with a remote OTP Server
 * @since_tizen 4.0
 */
typedef void *bt_otp_client_h;

/**
 * @internal
 * @brief The structure type of OTP object metadata
 * @since_tizen 4.0
 */
typedef struct {
	unsigned long long id;
	char *name;
} otp_object_metadata_s;

/**
 * @internal
 * @brief The structure type of list of OTP discovered objects
 * @since_tizen 4.0
 */
typedef struct {
	int num_objects;            /**< Number of Objects Discovered */
	otp_object_metadata_s **data;	/**< Array of Discovered Object MetaData */
} bt_otp_object_list_s;

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile Connection State changed callback which is associated with a remote OTP Server
 */
typedef void (*bt_otp_client_state_changed_cb)
		(int result, const char *remote_address, bt_otp_client_h otp_client, bool connected, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile remote object discovery callback
 */
typedef void (*bt_otp_client_object_discovery_cb)
		(int result, const char *remote_address, bt_otp_object_list_s *obj_list, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile select object callback
 */
typedef void (*bt_otp_client_object_select_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile remote object read complete callback
 */
typedef void (*bt_otp_client_read_object_complete_cb)
		(int result, const char *remote_address, char *file_path, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object create callback
 */
typedef void (*bt_otp_client_object_create_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object create callback
 */
typedef void (*bt_otp_client_object_write_cb)
		(int result, const char *remote_address, unsigned int length, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object execute callback
 */
typedef void (*bt_otp_client_object_execute_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object delete callback
 */
typedef void (*bt_otp_client_object_delete_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @}
 */

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising state for HRP.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 */
typedef enum {
	BT_HRP_SENSOR_ADVERTISING_STOPPED = 0x00, /**< Bluetooth advertising is stopped */
	BT_HRP_SENSOR_ADVERTISING_STARTED, /**< Bluetooth advertising is started */
} bt_hrp_sensor_advertising_state_e;

/**
 * @internal tizen 4.0
 * @brief HRP Sensor Advertisement state change callback
 */

typedef void (*bt_hrp_sensor_advertising_state_changed_cb)(int result,
		bt_hrp_sensor_advertising_state_e adv_state, void *user_data);


/* Body Sensor Enumeration
*/
typedef enum {
	BT_BSL_NOTSUPPORTED = -1,
	BT_BSL_OTHER = 0,
	BT_BSL_CHEST,
	BT_BSL_WRIST,
	BT_BSL_FINGER,
	BT_BSL_HAND,
	BT_BSL_EARLOBE,
	BT_BSL_FOOT,
} bt_body_sensor_location_e;

/**
 * @internal
 * @brief The handle of a HRP Collector client which is associated with a remote HRP sensor
 * @since_tizen 4.0
 */
typedef void *bt_hrp_collector_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when a value of a watched characteristic's GATT handle has been changed in HRP.
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
typedef void (*bt_hrp_collector_heart_rate_value_changed_cb) (bt_gatt_client_h characteristic,
		unsigned short hr_value, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief type information structure ,define what type of reequest is made.
 * @since_tizen 4.0
 *
 * @see bt_gatt_client_att_mtu_changed_cb()
 */

typedef enum {
	HEART_RATE = 1,
	BODY_SENSOR_LOCATION,
	HEART_CONTROL_POINT,
} bt_serv_char_type_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the LE advertisement callback for HRP sensor has been found.
 * @since_tizen 4.0
 *
 * @param[in] result The result of the LE scan
 * @param[in] info The information of the found LE advertisement.
 * @param[in] user_data The user data passed from the start function
 *
 *
 */
typedef void (*bt_hrp_collector_scan_result_cb)(int result,
		bt_adapter_le_device_scan_result_info_s *info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when the client request read operation on body sensor location has been completed.
 * @since_tizen 4
 *
 * @param[in] result The result of a request
 * @param[in] request_handle The requesting GATT handle
 * @param[in] type : Heart Rate Value , Body Sensor Location or Heart Control Point
 * @param[in] value : Int value corresponding to the type recieved .
 * @param[in] user_data The user data passed from the requesting function
 *
 */
typedef void (*bt_hrp_collector_bsl_read_completed_cb) (int result,
		bt_hrp_collector_h request_handle, bt_body_sensor_location_e location, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief HRP Collector profile Connection State changed callback which is associated with
 *	a remote HRP sensor.
 */
typedef void (*bt_hrp_collector_connection_state_changed_cb)
		(int result, const char *remote_address, bt_hrp_collector_h collector,
		  bool connected, void *user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__ */
