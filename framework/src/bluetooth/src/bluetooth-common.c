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

#include <tinyara/config.h>
#ifdef GLIB_SUPPORT
#include <glib.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
#include <bluetooth/bluetooth_extension.h>

#include "bt-adaptation-common.h"
#include "bluetooth_private.h"

static bool is_initialized = false;

/* These structs will be used in later */

static bt_event_sig_event_slot_s bt_event_slot_container[BT_EVENT_MAX] = { {NULL, NULL}, };


typedef struct {
	int err;
	char *str;
} bt_err2str_table_t;

static bt_err2str_table_t err2str[] = {
	{ BT_ERROR_CANCELLED, "CANCELLED" },
	{ BT_ERROR_INVALID_PARAMETER, "INVALID_PARAMETER" },
	{ BT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY" },
	{ BT_ERROR_RESOURCE_BUSY, "RESOURCE_BUSY" },
	{ BT_ERROR_TIMED_OUT, "TIMED_OUT" },
	{ BT_ERROR_NOW_IN_PROGRESS, "NOW_IN_PROGRESS" },
	{ BT_ERROR_NOT_INITIALIZED, "NOT_INITIALIZED" },
	{ BT_ERROR_NOT_ENABLED, "DEVICE_NOT_ENABLED" },
	{ BT_ERROR_ALREADY_DONE, "ALREADY_DONE" },
	{ BT_ERROR_OPERATION_FAILED, "OPERATION_FAILED" },
	{ BT_ERROR_NOT_IN_PROGRESS, "NOT_IN_PROGRESS" },
	{ BT_ERROR_REMOTE_DEVICE_NOT_BONDED, "REMOTE_DEVICE_NOT_BONDED" },
	{ BT_ERROR_AUTH_REJECTED, "AUTH_REJECTED" },
	{ BT_ERROR_AUTH_FAILED, "AUTH_FAILED" },
	{ BT_ERROR_REMOTE_DEVICE_NOT_FOUND, "REMOTE_DEVICE_NOT_FOUND" },
	{ BT_ERROR_SERVICE_SEARCH_FAILED, "SERVICE_SEARCH_FAILED" },
	{ BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED, "REMOTE_DEVICE_NOT_CONNECTED" },
	{ BT_ERROR_PERMISSION_DENIED, "PERMISSION_DENIED" },
	{ BT_ERROR_SERVICE_NOT_FOUND, "SERVICE_NOT_FOUND" },
	{ BT_ERROR_NOT_SUPPORTED, "NOT_SUPPORTD" },
	{ BT_ERROR_NO_DATA, "NO_DATA" },
	{ BT_ERROR_QUOTA_EXCEEDED, "QUOTA EXCEEDED" },
	{ BT_ERROR_AGAIN, "AGAIN" },
	{ BT_ERROR_DEVICE_POLICY_RESTRICTION, "DEVICE_POLICY_RESTRICTION" },
	{ 0, "UNKNOWN" }
};

typedef struct {
	int event;
	int index;
} bt_event2index_table_t;

static bt_event2index_table_t event2index[] = {
	{ BLUETOOTH_EVENT_ENABLED, BT_EVENT_STATE_CHANGED },
	{ BLUETOOTH_EVENT_DISABLED, BT_EVENT_STATE_CHANGED },
	{ BLUETOOTH_EVENT_LE_ENABLED, BT_EVENT_LE_STATE_CHANGED },
	{ BLUETOOTH_EVENT_LE_DISABLED, BT_EVENT_LE_STATE_CHANGED },
	{ BLUETOOTH_EVENT_LOCAL_NAME_CHANGED, BT_EVENT_NAME_CHANGED },
	{ BLUETOOTH_EVENT_DISCOVERABLE_MODE_CHANGED, BT_EVENT_VISIBILITY_MODE_CHANGED },
	{ BLUETOOTH_EVENT_DISCOVERABLE_TIMEOUT_CHANGED, BT_EVENT_VISIBILITY_DURATION_CHANGED },
	{ BLUETOOTH_EVENT_DISCOVERY_STARTED, BT_EVENT_DEVICE_DISCOVERY_STATE_CHANGED },
	{ BLUETOOTH_EVENT_DISCOVERY_FINISHED, BT_EVENT_DEVICE_DISCOVERY_STATE_CHANGED },
	{ BLUETOOTH_EVENT_REMOTE_DEVICE_NAME_UPDATED, BT_EVENT_DEVICE_DISCOVERY_STATE_CHANGED },
	{ BLUETOOTH_EVENT_BONDING_FINISHED, BT_EVENT_BOND_CREATED },
	{ BLUETOOTH_EVENT_BONDED_DEVICE_REMOVED, BT_EVENT_BOND_DESTROYED },
	{ BLUETOOTH_EVENT_KEYBOARD_PASSKEY_DISPLAY, BT_EVENT_AUTHENTICATION_REQUEST },
	{ BLUETOOTH_EVENT_PASSKEY_REQUEST,  BT_EVENT_AUTHENTICATION_REQUEST},
	{ BLUETOOTH_EVENT_PIN_REQUEST, BT_EVENT_AUTHENTICATION_REQUEST },
	{ BLUETOOTH_EVENT_PASSKEY_CONFIRM_REQUEST, BT_EVENT_AUTHENTICATION_REQUEST },
	{ BLUETOOTH_EVENT_DEVICE_AUTHORIZED, BT_EVENT_AUTHORIZATION_CHANGED },
	{ BLUETOOTH_EVENT_DEVICE_UNAUTHORIZED, BT_EVENT_AUTHORIZATION_CHANGED },
	{ BLUETOOTH_EVENT_DEVICE_CONNECTED, BT_EVENT_DEVICE_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_DEVICE_DISCONNECTED, BT_EVENT_DEVICE_CONNECTION_STATUS },
#ifdef TIZEN_GATT_CLIENT
	{ BLUETOOTH_EVENT_GATT_CLIENT_CONNECTED, BT_EVENT_GATT_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_GATT_CLIENT_DISCONNECTED, BT_EVENT_GATT_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_GATT_SERVER_CONNECTED, BT_EVENT_GATT_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_GATT_SERVER_DISCONNECTED, BT_EVENT_GATT_CONNECTION_STATUS },
#else
	{ BLUETOOTH_EVENT_GATT_CONNECTED, BT_EVENT_GATT_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_GATT_DISCONNECTED, BT_EVENT_GATT_CONNECTION_STATUS },
#endif
	{ BLUETOOTH_EVENT_SERVICE_SEARCHED, BT_EVENT_SERVICE_SEARCHED },
	{ BLUETOOTH_EVENT_RFCOMM_DATA_RECEIVED, BT_EVENT_DATA_RECEIVED },
	{ BLUETOOTH_EVENT_RFCOMM_CONNECTED, BT_EVENT_CONNECTION_STATE_CHANGED },
	{ BLUETOOTH_EVENT_RFCOMM_DISCONNECTED, BT_EVENT_CONNECTION_STATE_CHANGED },
	{ BLUETOOTH_EVENT_RFCOMM_AUTHORIZE, BT_EVENT_RFCOMM_CONNECTION_REQUESTED },
	{ BLUETOOTH_EVENT_OBEX_SERVER_CONNECTION_AUTHORIZE, BT_EVENT_OPP_CONNECTION_REQUESTED },
	{ BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_AUTHORIZE, BT_EVENT_OPP_PUSH_REQUESTED },
	{ BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_STARTED, BT_EVENT_OPP_SERVER_TRANSFER_PROGRESS },
	{ BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_PROGRESS, BT_EVENT_OPP_SERVER_TRANSFER_PROGRESS },
	{ BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_COMPLETED, BT_EVENT_OPP_SERVER_TRANSFER_FINISHED },
	{ BLUETOOTH_EVENT_OPC_CONNECTED, BT_EVENT_OPP_CLIENT_PUSH_RESPONSED },
	{ BLUETOOTH_EVENT_OPC_TRANSFER_STARTED, BT_EVENT_OPP_CLIENT_PUSH_PROGRESS },
	{ BLUETOOTH_EVENT_OPC_TRANSFER_PROGRESS, BT_EVENT_OPP_CLIENT_PUSH_PROGRESS },
	{ BLUETOOTH_EVENT_OPC_TRANSFER_COMPLETE, BT_EVENT_OPP_CLIENT_PUSH_PROGRESS },
	{ BLUETOOTH_EVENT_OPC_DISCONNECTED, BT_EVENT_OPP_CLIENT_PUSH_FINISHED },
	{ BLUETOOTH_EVENT_MAP_LIST_FOLDERS_COMPLETE, BT_EVENT_MAP_CLIENT_LIST_FOLDERS },
	{ BLUETOOTH_EVENT_MAP_LIST_FILTER_FIELD_COMPLETE, BT_EVENT_MAP_CLIENT_LIST_FILTER_FIELDS },
	{ BLUETOOTH_EVENT_MAP_LIST_MESSAGES_COMPLETE, BT_EVENT_MAP_CLIENT_LIST_MESSAGES },
	{ BLUETOOTH_EVENT_MAP_GET_MESSAGE_COMPLETE, BT_EVENT_MAP_CLIENT_GET_MESSAGE },
	{ BLUETOOTH_EVENT_MAP_PUSH_MESSAGE_COMPLETE, BT_EVENT_MAP_CLIENT_PUSH_MESSAGE },
	{ BLUETOOTH_EVENT_NETWORK_SERVER_CONNECTED, BT_EVENT_NAP_CONNECTION_STATE_CHANGED },
	{ BLUETOOTH_EVENT_NETWORK_SERVER_DISCONNECTED, BT_EVENT_NAP_CONNECTION_STATE_CHANGED },
	{ BLUETOOTH_EVENT_NETWORK_CONNECTED, BT_EVENT_PAN_CONNECTION_STATE_CHANGED },
	{ BLUETOOTH_EVENT_NETWORK_DISCONNECTED, BT_EVENT_PAN_CONNECTION_STATE_CHANGED },
	{ BLUETOOTH_EVENT_HDP_DATA_RECEIVED, BT_EVENT_HDP_DATA_RECEIVED },
	{ BLUETOOTH_EVENT_HDP_CONNECTED, BT_EVENT_HDP_CONNECTED },
	{ BLUETOOTH_EVENT_HDP_DISCONNECTED, BT_EVENT_HDP_DISCONNECTED },
	{ BLUETOOTH_EVENT_AG_CONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AG_DISCONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AG_SPEAKER_GAIN, BT_EVENT_AG_SPEAKER_GAIN_CHANGE },
	{ BLUETOOTH_EVENT_AG_MIC_GAIN, BT_EVENT_AG_MICROPHONE_GAIN_CHANGE },
	{ BLUETOOTH_EVENT_AG_AUDIO_CONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AG_AUDIO_DISCONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_TELEPHONY_AUDIO_CONNECTED, BT_EVENT_AG_SCO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_TELEPHONY_AUDIO_DISCONNECTED, BT_EVENT_AG_SCO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_TELEPHONY_ANSWER_CALL, BT_EVENT_AG_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_RELEASE_CALL, BT_EVENT_AG_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_REJECT_CALL, BT_EVENT_AG_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_CHLD_0_RELEASE_ALL_HELD_CALL, BT_EVENT_AG_MULTI_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_CHLD_1_RELEASE_ALL_ACTIVE_CALL, BT_EVENT_AG_MULTI_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_CHLD_2_ACTIVE_HELD_CALL, BT_EVENT_AG_MULTI_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_CHLD_3_MERGE_CALL, BT_EVENT_AG_MULTI_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_CHLD_4_EXPLICIT_CALL_TRANSFER, BT_EVENT_AG_MULTI_CALL_HANDLING_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_SEND_DTMF, BT_EVENT_AG_DTMF_TRANSMITTED },
	{ BLUETOOTH_EVENT_AV_CONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AV_DISCONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AV_SOURCE_CONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AV_SOURCE_DISCONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AVRCP_CONNECTED, BT_EVENT_AVRCP_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AVRCP_DISCONNECTED, BT_EVENT_AVRCP_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AVRCP_CONTROL_CONNECTED, BT_EVENT_AVRCP_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AVRCP_CONTROL_DISCONNECTED, BT_EVENT_AVRCP_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_AVRCP_SETTING_SHUFFLE_STATUS, BT_EVENT_AVRCP_SHUFFLE_MODE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_CONTROL_SHUFFLE_STATUS, BT_EVENT_AVRCP_SHUFFLE_MODE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_SETTING_EQUALIZER_STATUS, BT_EVENT_AVRCP_EQUALIZER_STATE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_CONTROL_EQUALIZER_STATUS, BT_EVENT_AVRCP_EQUALIZER_STATE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_SETTING_REPEAT_STATUS, BT_EVENT_AVRCP_REPEAT_MODE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_CONTROL_REPEAT_STATUS, BT_EVENT_AVRCP_REPEAT_MODE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_SETTING_SCAN_STATUS, BT_EVENT_AVRCP_SCAN_MODE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_CONTROL_SCAN_STATUS, BT_EVENT_AVRCP_SCAN_MODE_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_SONG_POSITION_STATUS, BT_EVENT_AVRCP_SONG_POSITION_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_PLAY_STATUS_CHANGED, BT_EVENT_AVRCP_PLAY_STATUS_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_TRACK_CHANGED, BT_EVENT_AVRCP_TRACK_INFO_CHANGED },
	{ BLUETOOTH_EVENT_AVRCP_DELAY_CHANGED, BT_EVENT_AVRCP_DELAY_CHANGED },
	{ BLUETOOTH_HID_CONNECTED, BT_EVENT_HID_CONNECTION_STATUS },
	{ BLUETOOTH_HID_DISCONNECTED, BT_EVENT_HID_CONNECTION_STATUS },
	{ BLUETOOTH_HID_DEVICE_CONNECTED, BT_EVENT_HID_DEVICE_CONNECTION_STATUS },
	{ BLUETOOTH_HID_DEVICE_DISCONNECTED, BT_EVENT_HID_DEVICE_CONNECTION_STATUS },
	{ BLUETOOTH_HID_DEVICE_DATA_RECEIVED, BT_EVENT_HID_DEVICE_DATA_RECEIVED },
	{ BLUETOOTH_EVENT_GATT_CHAR_VAL_CHANGED, BT_EVENT_GATT_CLIENT_VALUE_CHANGED },
	{ BLUETOOTH_EVENT_GATT_READ_CHAR, BT_EVENT_GATT_CLIENT_READ_CHARACTERISTIC },
	{ BLUETOOTH_EVENT_GATT_WRITE_CHAR, BT_EVENT_GATT_CLIENT_WRITE_CHARACTERISTIC },
	{ BLUETOOTH_EVENT_GATT_READ_DESC, BT_EVENT_GATT_CLIENT_READ_DESCRIPTOR },
	{ BLUETOOTH_EVENT_GATT_WRITE_DESC, BT_EVENT_GATT_CLIENT_WRITE_DESCRIPTOR },
	{ BLUETOOTH_EVENT_GATT_ATT_MTU_CHANGED, BT_EVENT_GATT_ATT_MTU_CHANGE_STATUS },
	{ BLUETOOTH_EVENT_MANUFACTURER_DATA_CHANGED, BT_EVENT_MANUFACTURER_DATA_CHANGED },
	{ BLUETOOTH_EVENT_PASSKEY_NOTIFICATION, BT_EVENT_PASSKEY_NOTIFICATION_EVENT },
	{ BLUETOOTH_EVENT_CONNECTABLE_CHANGED, BT_EVENT_CONNECTABLE_CHANGED_EVENT },
	{ BLUETOOTH_EVENT_TELEPHONY_VENDOR_AT_CMD, BT_EVENT_AG_VENDOR_CMD },
	{ BLUETOOTH_EVENT_RSSI_ENABLED, BT_EVENT_RSSI_ENABLED_EVENT },
	{ BLUETOOTH_EVENT_RSSI_ALERT, BT_EVENT_RSSI_ALERT_EVENT },
	{ BLUETOOTH_EVENT_RAW_RSSI, BT_EVENT_GET_RSSI_EVENT },
	{ BLUETOOTH_EVENT_IPSP_CONNECTED, BT_EVENT_IPSP_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_IPSP_DISCONNECTED, BT_EVENT_IPSP_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_SUPPORTED_PROFILE_TRUSTED, BT_EVENT_SUPPORTED_TRUSTED_PROFILE_EVENT },
	{ BLUETOOTH_EVENT_LE_DATA_LENGTH_CHANGED, BT_EVENT_LE_DATA_LENGTH_CHANGED },
	{ BLUETOOTH_PBAP_CONNECTED, BT_EVENT_PBAP_CONNECTION_STATUS },
	{ BLUETOOTH_PBAP_DISCONNECTED, BT_EVENT_PBAP_CONNECTION_STATUS },
	{ BLUETOOTH_PBAP_PHONEBOOK_SIZE, BT_EVENT_PBAP_PHONEBOOK_SIZE },
	{ BLUETOOTH_PBAP_PHONEBOOK_PULL, BT_EVENT_PBAP_PHONEBOOK_PULL },
	{ BLUETOOTH_PBAP_VCARD_LIST, BT_EVENT_PBAP_VCARD_LIST },
	{ BLUETOOTH_PBAP_VCARD_PULL, BT_EVENT_PBAP_VCARD_PULL },
	{ BLUETOOTH_PBAP_PHONEBOOK_SEARCH, BT_EVENT_PBAP_PHONEBOOK_SEARCH },
	{ BLUETOOTH_EVENT_HF_CONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_HF_DISCONNECTED, BT_EVENT_AUDIO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_HF_AUDIO_CONNECTED, BT_EVENT_HF_SCO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_HF_AUDIO_DISCONNECTED, BT_EVENT_HF_SCO_CONNECTION_STATUS },
	{ BLUETOOTH_EVENT_HF_RING_INDICATOR, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_TERMINATED, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_STARTED, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_ENDED, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_WAITING, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_ON_HOLD, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_UNHOLD, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_SWAPPED, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_FAILED_TO_DIAL, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALL_IDLE, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALLSETUP_INCOMING, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALLSETUP_DIALING, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_CALLSETUP_ALERTING, BT_EVENT_HF_REMOTE_CALL_EVENT },
	{ BLUETOOTH_EVENT_HF_VENDOR_DEP_CMD, BT_EVENT_HF_VENDOR_DEP_CMD_EVENT },
	{ BLUETOOTH_EVENT_HF_VOLUME_SPEAKER, BT_EVENT_HF_SPEAKER_GAIN_CHANGE },
	{ BLUETOOTH_EVENT_HF_CALL_STATUS, BT_EVENT_HF_CALL_STATUS_UPDATED_EVENT },
	{ BLUETOOTH_EVENT_HF_CIEV_DEVICE_STATUS_CHANGED, BT_EVENT_HF_CIEV_DEVICE_STATUS_CHANGED },
	{ BLUETOOTH_EVENT_HF_VOICE_RECOGNITION_ENABLED, BT_EVENT_HF_CIEV_DEVICE_STATUS_CHANGED },
	{ BLUETOOTH_EVENT_HF_VOICE_RECOGNITION_DISABLED, BT_EVENT_HF_CIEV_DEVICE_STATUS_CHANGED },
	{ BLUETOOTH_EVENT_IPSP_INIT_STATE_CHANGED, BT_EVENT_IPSP_INIT_STATE_CHANGED },
	{ BLUETOOTH_EVENT_PXP_PROPERTY_CHANGED, BT_EVENT_PROXIMITY_REPORTER_PROPERTY_CHANGED },
	{ BLUETOOTH_EVENT_TDS_ACTIVATION_REQUESTED, BT_EVENT_TDS_ACTIVATION_REQUESTED },
	{ BLUETOOTH_EVENT_OTP_SERVER_STATE_CHANGED, BT_EVENT_OTP_SERVER_STATE_CHANGED },
	{ 0, -1 }
};

typedef struct {
	int origin_err;
	int public_err;
} bt_err2pub_table_t;

static bt_err2pub_table_t err2pub[] = {
	{ 0, BT_ERROR_NONE },
	{ -EINVAL, BT_ERROR_INVALID_PARAMETER },
	{ -EAGAIN, BT_ERROR_NOT_ENABLED },
	{ -EALREADY, BT_ERROR_NOW_IN_PROGRESS },
	{ 0xFF, BT_ERROR_OPERATION_FAILED }
};

static int __bt_get_cb_index(int event)
{
	int i;

	for (i = 0; event2index[i].event != 0; i++) {
		if (event2index[i].event == event)
			return event2index[i].index;
	}

	return -1; /* LCOV_EXCL_LINE */
}

static int __bt_get_bt_adapter_le_device_scan_info_s(
			bt_adapter_le_device_scan_result_info_s **scan_info,
			bluetooth_le_device_info_t *source_info)
{

	BT_CHECK_INPUT_PARAMETER(source_info);

	*scan_info = (bt_adapter_le_device_scan_result_info_s *)malloc(sizeof(bt_adapter_le_device_scan_result_info_s));
	if (*scan_info == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	_bt_convert_address_to_string(&((*scan_info)->remote_address), &(source_info->device_address));

	if (source_info->addr_type == 0x02)
		(*scan_info)->address_type = BT_DEVICE_RANDOM_ADDRESS;
	else
		(*scan_info)->address_type = BT_DEVICE_PUBLIC_ADDRESS; /* LCOV_EXCL_LINE */
	(*scan_info)->rssi = (int)source_info->rssi;
	(*scan_info)->adv_data_len = source_info->adv_ind_data.data_len;
	if ((*scan_info)->adv_data_len > 0) {
		(*scan_info)->adv_data = malloc(source_info->adv_ind_data.data_len);
		memcpy((*scan_info)->adv_data, source_info->adv_ind_data.data.data, source_info->adv_ind_data.data_len);
	} else {
		(*scan_info)->adv_data = NULL; /* LCOV_EXCL_LINE */
	}

	(*scan_info)->scan_data_len = source_info->scan_resp_data.data_len;
	if ((*scan_info)->scan_data_len > 0) {
		(*scan_info)->scan_data = malloc(source_info->scan_resp_data.data_len); /* LCOV_EXCL_LINE */
		memcpy((*scan_info)->scan_data, source_info->scan_resp_data.data.data, source_info->scan_resp_data.data_len); /* LCOV_EXCL_LINE */
	} else {
		(*scan_info)->scan_data = NULL;
	}

	return BT_ERROR_NONE;
}

void __bt_event_proxy(int event, bluetooth_event_param_t *param)
{
	int event_index = -1;

	BT_PRT("Enter\n");

	event_index = __bt_get_cb_index(event);

	switch (event) {
	case BLUETOOTH_EVENT_ENABLED:
		BT_INFO("bt_adapter_state_changed_cb() will be called with BT_ADAPTER_ENABLED");
		((bt_adapter_state_changed_cb) bt_event_slot_container[event_index].callback)
		    (_bt_get_error_code(param->result), BT_ADAPTER_ENABLED, bt_event_slot_container[event_index].user_data);
		break;
	case BLUETOOTH_EVENT_DISABLED:
		BT_INFO("bt_adapter_state_changed_cb() will be called with BT_ADAPTER_DISABLED");
		((bt_adapter_state_changed_cb) bt_event_slot_container[event_index].callback)
		    (_bt_get_error_code(param->result), BT_ADAPTER_DISABLED, bt_event_slot_container[event_index].user_data);
		break;
	default:
		BT_INFO("Unknown function");
		break;
	}
}

void __bt_le_event_proxy(int event, bluetooth_event_param_t *param)
{
	bt_adapter_le_device_scan_result_info_s *scan_info = NULL;
	int event_index = -1;

	BT_PRT("Enter\n");

	event_index = __bt_get_cb_index(event);

	switch (event) {
	case BLUETOOTH_EVENT_LE_ENABLED:
		BT_INFO("BLUETOOTH_EVENT_LE_ENABLED"); /* LCOV_EXCL_LINE */
		((bt_adapter_le_state_changed_cb) bt_event_slot_container[event_index].callback) /* LCOV_EXCL_LINE */
		    (_bt_get_error_code(param->result), BT_ADAPTER_LE_ENABLED, bt_event_slot_container[event_index].user_data);
		break;
	case BLUETOOTH_EVENT_LE_DISABLED:
		BT_INFO("BLUETOOTH_EVENT_LE_DISABLED"); /* LCOV_EXCL_LINE */
		((bt_adapter_le_state_changed_cb) bt_event_slot_container[event_index].callback) /* LCOV_EXCL_LINE */
		    (_bt_get_error_code(param->result), BT_ADAPTER_LE_DISABLED, bt_event_slot_container[event_index].user_data);
		break;
	case BLUETOOTH_EVENT_REMOTE_LE_DEVICE_FOUND: /* LCOV_EXCL_START */
		BT_INFO("BLUETOOTH_EVENT_REMOTE_LE_DEVICE_FOUND");
		event_index = BT_EVENT_LE_SCAN_RESULT_UPDATED;
		if (bt_event_slot_container[event_index].callback != NULL) {
			if (__bt_get_bt_adapter_le_device_scan_info_s(&scan_info, (bluetooth_le_device_info_t *)(param->param_data)) == BT_ERROR_NONE) {
				((bt_adapter_le_scan_result_cb)bt_event_slot_container[event_index].callback)
					(_bt_get_error_code(param->result), scan_info, bt_event_slot_container[event_index].user_data);
			} else {
				((bt_adapter_le_scan_result_cb)bt_event_slot_container[event_index].callback)
					(_bt_get_error_code(param->result), NULL, bt_event_slot_container[event_index].user_data);
			}
		}
	default:
		break;
	}
}

/*
 *  Public Functions
 */

int bt_initialize(void)
{
	printf("bt_initialize() entered!!!\n");
	bt_adapt_init();

	is_initialized = true;

	return BT_ERROR_NONE;
}

int bt_deinitialize(void)
{
	BT_CHECK_INIT_STATUS();

	bt_adapt_deinit();

	is_initialized = false;

	return BT_ERROR_NONE;
}

int bt_get_uuid_name(const char *uuid, char **name)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/*
 *  Common Functions
 */
int _bt_check_init_status(void)
{
	if (is_initialized != true) {
		BT_ERR("NOT_INITIALIZED(0x%08x)", BT_ERROR_NOT_INITIALIZED);
		return BT_ERROR_NOT_INITIALIZED;
	}

	return BT_ERROR_NONE;
}

void _bt_set_cb(int events, void *callback, void *user_data)
{
	bt_event_slot_container[events].callback = callback;
	bt_event_slot_container[events].user_data = user_data;
}

void _bt_unset_cb(int events)
{
	if (bt_event_slot_container[events].callback != NULL) {
		bt_event_slot_container[events].callback = NULL;
		bt_event_slot_container[events].user_data = NULL;
	}
}

bool _bt_check_cb(int events)
{
	return (bt_event_slot_container[events].callback != NULL) ? true : false;
}

void _bt_convert_address_to_hex(unsigned char **addr_hex, const char *addr_str)
{
	char *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

	if (addr_str == NULL || addr_str[0] == '\0')
		return;

	*addr_hex[0] = strtol(addr_str, &ptr5, 16);
	*addr_hex[1] = strtol(ptr5 + 1, &ptr4, 16);
	*addr_hex[2] = strtol(ptr4 + 1, &ptr3, 16);
	*addr_hex[3] = strtol(ptr3 + 1, &ptr2, 16);
	*addr_hex[4] = strtol(ptr2 + 1, &ptr1, 16);
	*addr_hex[5] = strtol(ptr1 + 1, NULL, 16);
}

int _bt_convert_address_to_string(char **addr_str, unsigned char *addr_hex)
{
	char address[18] = { 0, };

	BT_CHECK_INPUT_PARAMETER(addr_hex);

	snprintf(address, 18, "%02X:%02X:%02X:%02X:%02X:%02X", addr_hex[0], addr_hex[1], addr_hex[2], addr_hex[3], addr_hex[4], addr_hex[5]);
	*addr_str = strdup(address);

	if (*addr_str != NULL)
		return BT_ERROR_NONE;
	else
		return BT_ERROR_OUT_OF_MEMORY; /* LCOV_EXCL_LINE */
}

int _bt_get_error_code(int origin_error)
{
	int i;

	for (i = 0; err2pub[i].origin_err != 0xFF; i++) {
		if (err2pub[i].origin_err == origin_error)
			return err2pub[i].public_err;
	}

	return BT_ERROR_OPERATION_FAILED;
}

char *_bt_convert_uuid_to_uuid128(const char *uuid)
{
	int len;
	char *uuid128;

	len = strlen(uuid);

	switch (len) {
	case 4:         /* UUID 16bits */
#ifdef GLIB_SUPPORTED
		uuid128 = g_strdup_printf("0000%s-0000-1000-8000-00805f9b34fb", uuid);
#else
		len = snprintf(NULL, 0, "0000%s-0000-1000-8000-00805f9b34fb", uuid);
		uuid128 = malloc(len + 1);
		if (uuid128 == NULL)
			return NULL;
		snprintf(uuid128, len + 1,  "0000%s-0000-1000-8000-00805f9b34fb", uuid);
#endif
		break;

	case 8:         /* UUID 32bits */
#ifdef GLIB_SUPPORTED
		uuid128 = g_strdup_printf("%s-0000-1000-8000-00805f9b34fb", uuid); /* LCOV_EXCL_LINE */
#else
		len = snprintf(NULL, 0, "%s-0000-1000-8000-00805f9b34fb", uuid);
		uuid128 = malloc(len + 1);
		if (uuid128 == NULL)
			return NULL;
		snprintf(uuid128, len + 1,  "%s-0000-1000-8000-00805f9b34fb", uuid);
#endif
		break; /* LCOV_EXCL_LINE */

	case 36:        /* UUID 128bits */
		uuid128 = strdup(uuid); /* LCOV_EXCL_LINE */
		break; /* LCOV_EXCL_LINE */

	default:
		return NULL;
	}

	return uuid128;
}

void _bt_free_bt_device_info_s(bt_device_info_s *device_info)
{
	int i = 0;

	if (device_info == NULL)
		return;

	if (device_info->remote_name != NULL)
		free(device_info->remote_name);

	if (device_info->remote_address != NULL)
		free(device_info->remote_address);

	if (device_info->service_uuid != NULL) {
		for (i = 0; i < device_info->service_count; i++) {
			if (device_info->service_uuid[i] != NULL)
			free(device_info->service_uuid[i]);
		}
		free(device_info->service_uuid);
	}

	if (device_info->manufacturer_data != NULL)
		free(device_info->manufacturer_data);

	free(device_info);
	device_info = NULL;
}

char *_bt_convert_error_to_string(int error)
{
	int i;

	for (i = 0; err2str[i].err != 0; i++) {
		if (err2str[i].err == error)
		return err2str[i].str;
	}

	return "UNKNOWN"; /* LCOV_EXCL_LINE */
}

