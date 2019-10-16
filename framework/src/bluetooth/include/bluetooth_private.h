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


#ifndef __TIZEN_NETWORK_BLUETOOTH_PRIVATE_H__
#define __TIZEN_NETWORK_BLUETOOTH_PRIVATE_H__

#include <stdbool.h>
#include <stdio.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>

#include "bt-adaptation-common.h"
#include "bt-adaptation-adapter.h"
#include "bt-adaptation-device.h"
#include "bt-adaptation-gatt.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef LOG_TAG
#define LOG_TAG "CAPI_NETWORK_BLUETOOTH"

#define BT_PRT(format, args...) printf("[BT_CAPI] %s:%d() "format, __FUNCTION__, __LINE__, ##args)

#define BT_INFO(fmt, args...) BT_PRT(fmt"\n", ##args)
#define BT_DBG(fmt, args...) BT_PRT(fmt"\n", ##args)
#define BT_ERR(fmt, args...) BT_PRT(fmt"\n", ##args)

/**
 * @internal
 * @brief Bluetooth callback.
 */
typedef enum {
	BT_EVENT_STATE_CHANGED = 0x00, /**< Adapter state is changed */
	BT_EVENT_LE_STATE_CHANGED, /**< Adapter le state is changed */
	BT_EVENT_NAME_CHANGED, /**< Adapter name is changed */
	BT_EVENT_VISIBILITY_MODE_CHANGED, /**< Adapter visibility mode is changed */
	BT_EVENT_VISIBILITY_DURATION_CHANGED, /**< Adapter visibility duration is changed */
	BT_EVENT_DEVICE_DISCOVERY_STATE_CHANGED, /**< Device discovery state is changed */
	BT_EVENT_LE_SCAN_RESULT_UPDATED, /**< LE Scan result is updated */
	BT_EVENT_BOND_CREATED, /**< A bond is created */
	BT_EVENT_BOND_DESTROYED, /**< A bond is destroyed */
	BT_EVENT_AUTHORIZATION_CHANGED, /**< Authorization is changed */
	BT_EVENT_AUTHENTICATION_REQUEST, /**< Authentication events during pairing process*/
	BT_EVENT_SERVICE_SEARCHED, /**< Service search finish */
	BT_EVENT_DATA_RECEIVED, /**< Data is received */
	BT_EVENT_CONNECTION_STATE_CHANGED, /**< Connection state is changed */
	BT_EVENT_RFCOMM_CONNECTION_REQUESTED, /**< RFCOMM connection is requested */
	BT_EVENT_OPP_CONNECTION_REQUESTED, /**< OPP connection is requested */
	BT_EVENT_OPP_PUSH_REQUESTED, /**< OPP push is requested */
	BT_EVENT_OPP_SERVER_TRANSFER_PROGRESS, /**< OPP transfer progress */
	BT_EVENT_OPP_SERVER_TRANSFER_FINISHED, /**< OPP transfer is completed */
	BT_EVENT_OPP_CLIENT_PUSH_RESPONSED, /**< OPP client connection is responsed */
	BT_EVENT_OPP_CLIENT_PUSH_PROGRESS, /**< OPP client push progress */
	BT_EVENT_OPP_CLIENT_PUSH_FINISHED, /**< OPP client push is finished */
	BT_EVENT_MAP_CONNECTION_REQUESTED, /**< MAP connection is requested */
	BT_EVENT_MAP_CLIENT_OP_COMPLETE, /**< MAP client operation is complete */
	BT_EVENT_PAN_CONNECTION_STATE_CHANGED, /**< PAN connection change */
	BT_EVENT_NAP_CONNECTION_STATE_CHANGED, /**< NAP connection change */
	BT_EVENT_HDP_CONNECTED, /**< HDP connection change */
	BT_EVENT_HDP_DISCONNECTED, /**< HDP disconnection change */
	BT_EVENT_HDP_DATA_RECEIVED, /**< HDP Data receive Callabck */
	BT_EVENT_AUDIO_CONNECTION_STATUS, /**< Audio Connection change callback */
	BT_EVENT_AG_SCO_CONNECTION_STATUS, /**< Audio - AG SCO Connection state change callback */
	BT_EVENT_AG_CALL_HANDLING_EVENT, /**< Audio - AG call event callback */
	BT_EVENT_AG_MULTI_CALL_HANDLING_EVENT, /**< Audio - AG 3-way call event callback */
	BT_EVENT_AG_DTMF_TRANSMITTED, /**< Audio - DTMF tone sending request */
	BT_EVENT_AG_MICROPHONE_GAIN_CHANGE, /**< Audio Microphone change callback */
	BT_EVENT_AG_SPEAKER_GAIN_CHANGE, /**< Audio Speaker gain change callback */
	BT_EVENT_AG_VENDOR_CMD, /**< Audio - XSAT Vendor cmd */
	BT_EVENT_AVRCP_CONNECTION_STATUS, /**< AVRCP connection change callback */
	BT_EVENT_AVRCP_EQUALIZER_STATE_CHANGED, /**< AVRCP equalizer state change callback */
	BT_EVENT_AVRCP_REPEAT_MODE_CHANGED, /**< AVRCP repeat mode change callback */
	BT_EVENT_AVRCP_SHUFFLE_MODE_CHANGED, /**< AVRCP equalizer mode change callback */
	BT_EVENT_AVRCP_SCAN_MODE_CHANGED, /**< AVRCP scan mode change callback */
	BT_EVENT_AVRCP_PLAY_STATUS_CHANGED, /**< AVRCP scan mode change callback */
	BT_EVENT_AVRCP_SONG_POSITION_CHANGED, /**< AVRCP scan mode change callback */
	BT_EVENT_AVRCP_TRACK_INFO_CHANGED, /**< AVRCP scan mode change callback */
	BT_EVENT_AVRCP_DELAY_CHANGED, /**< AVRCP scan mode change callback */
	BT_EVENT_HID_CONNECTION_STATUS, /**< HID connection status callback */
	BT_EVENT_HID_DEVICE_CONNECTION_STATUS, /**< HID Device connection status callback */
	BT_EVENT_HID_DEVICE_DATA_RECEIVED, /**< HID Device Data received callback */
	BT_EVENT_DEVICE_CONNECTION_STATUS, /**< Device connection status callback */
	BT_EVENT_GATT_CONNECTION_STATUS, /** < GATT connection status callback */
	BT_EVENT_GATT_ATT_MTU_CHANGE_STATUS, /** < GATT Attribute MTU change status callback */
	BT_EVENT_GATT_CLIENT_SERVICE_DISCOVERED, /** GATT services discovered callback */
	BT_EVENT_GATT_CLIENT_SERVICE_CHANGED, /** GATT services changed callback */
	BT_EVENT_GATT_CLIENT_VALUE_CHANGED, /**< GATT characteristic value changed callback */
	BT_EVENT_GATT_CLIENT_READ_CHARACTERISTIC, /**< GATT characteristic value read callback */
	BT_EVENT_GATT_CLIENT_WRITE_CHARACTERISTIC, /**< GATT characteristic value write callback */
	BT_EVENT_GATT_CLIENT_READ_DESCRIPTOR, /**< GATT descriptor value read callback */
	BT_EVENT_GATT_CLIENT_WRITE_DESCRIPTOR, /**< GATT descriptor value write callback */
	BT_EVENT_GATT_SERVER_READ_REQUESTED, /**< GATT Characteristic/Descriptor Read Requested callback*/
	BT_EVENT_IPSP_INIT_STATE_CHANGED, /**< IPSP Init status changed callback */
	BT_EVENT_IPSP_CONNECTION_STATUS, /**< IPSP connection status callback */
	BT_EVENT_LE_DATA_LENGTH_CHANGED, /** LE data length changed callback */
	BT_EVENT_ADVERTISING_STATE_CHANGED, /**< Advertising state changed callback */
	BT_EVENT_MANUFACTURER_DATA_CHANGED, /**< Manufacturer data changed callback */
	BT_EVENT_PASSKEY_NOTIFICATION_EVENT, /**< Display passkey callback */
	BT_EVENT_CONNECTABLE_CHANGED_EVENT, /**< Adapter connectable changed callback */
	BT_EVENT_RSSI_ENABLED_EVENT, /**< RSSI Enabled callback */
	BT_EVENT_RSSI_ALERT_EVENT, /**< RSSI Alert callback */
	BT_EVENT_GET_RSSI_EVENT, /**< Get RSSI Strength callback */
	BT_EVENT_LE_READ_MAXIMUM_DATA_LENGTH, /**< Read maximun LE data length callback */
	BT_EVENT_SUPPORTED_TRUSTED_PROFILE_EVENT, /**< Trusted Profile callback */
	BT_EVENT_PROXIMITY_CONNECTION_STATE_CHANGED, /**< Proximity Monitor/Reporter Connection sgate changed */
	BT_EVENT_PROXIMITY_REPORTER_PROPERTY_CHANGED, /**< Proximity property changed */
	BT_EVENT_PBAP_CONNECTION_STATUS, /**< PBAP connection status callback */
	BT_EVENT_PBAP_PHONEBOOK_SIZE, /**< PBAP Phonebook Size status callback */
	BT_EVENT_PBAP_PHONEBOOK_PULL, /**< PBAP Phonebook Pull status callback */
	BT_EVENT_PBAP_VCARD_LIST, /**< PBAP vCard List status callback */
	BT_EVENT_PBAP_VCARD_PULL, /**< PBAP vCard Pull status callback */
	BT_EVENT_PBAP_PHONEBOOK_SEARCH, /**< PBAP Phonebook Search status callback */
	BT_EVENT_HF_SCO_CONNECTION_STATUS, /**< Audio - HF SCO Connection state change callback */
	BT_EVENT_HF_SPEAKER_GAIN_CHANGE, /**< Audio - HF Speaker gain change callback */
	BT_EVENT_HF_CALL_HANDLING_EVENT, /**< Audio - HF call event callback */
	BT_EVENT_HF_VENDOR_DEP_CMD_EVENT, /**< Audio - HF Vendor Command callback */
	BT_EVENT_HF_MULTI_CALL_HANDLING_EVENT, /**< Audio - HF 3-way call event callback */
	BT_EVENT_HF_CALL_STATUS_UPDATED_EVENT, /**< Audio - HF call status updated callback */
	BT_EVENT_HF_REMOTE_CALL_EVENT, /**< Audio - HF : call state event callback */
	BT_EVENT_HF_CIEV_DEVICE_STATUS_CHANGED, /**< Audio - HF : device state changed callback */
	BT_EVENT_MAP_CLIENT_LIST_FOLDERS, /**< MAP client - listFolders event*/
	BT_EVENT_MAP_CLIENT_LIST_FILTER_FIELDS, /**< MAP - get filter field list callback */
	BT_EVENT_MAP_CLIENT_LIST_MESSAGES, /**< MAP - listMessages event*/
	BT_EVENT_MAP_CLIENT_GET_MESSAGE, /**< MAP - getMessage event*/
	BT_EVENT_MAP_CLIENT_PUSH_MESSAGE, /**< MAP - pushMessage event*/
	BT_EVENT_TDS_ACTIVATION_REQUESTED, /**< TDS - transport activation requested event */
	BT_EVENT_TDS_PROVIDER_FOUND_RESULT, /**< TDS - CUSTOM Provider */
	BT_EVENT_OTP_SERVER_STATE_CHANGED, /**< OTP - Server Status */
	BT_EVENT_MAX
} bt_event_e;

#define BLUETOOTH_EVENT_BASE            ((int)(0x0000))		/**< No event */
#define BLUETOOTH_EVENT_GAP_BASE        ((int)(BLUETOOTH_EVENT_BASE + 0x0010))
								/**< Base ID for GAP Event */
#define BLUETOOTH_EVENT_SDP_BASE        ((int)(BLUETOOTH_EVENT_GAP_BASE + 0x0020))
								/**< Base ID for SDP events */
#define BLUETOOTH_EVENT_RFCOMM_BASE     ((int)(BLUETOOTH_EVENT_SDP_BASE + 0x0020))
								/**< Base ID for RFCOMM events */
#define BLUETOOTH_EVENT_NETWORK_BASE     ((int)(BLUETOOTH_EVENT_RFCOMM_BASE + 0x0020))
								/**< Base ID for NETWORK events */
#define BLUETOOTH_EVENT_HDP_BASE     ((int)(BLUETOOTH_EVENT_NETWORK_BASE + 0x0020))
								/**< Base ID for HDP events */
#define BLUETOOTH_EVENT_OPC_BASE  ((int)(BLUETOOTH_EVENT_HDP_BASE + 0x0020))
								/**< Base ID for OPC events */
#define BLUETOOTH_EVENT_OBEX_SERVER_BASE ((int)(BLUETOOTH_EVENT_OPC_BASE + 0x0020))
								/**< Base ID for Obex Server events */
#define BLUETOOTH_EVENT_GATT_BASE ((int)(BLUETOOTH_EVENT_OBEX_SERVER_BASE + 0x0020))
								/**< Base ID for GATT events */

#define BLUETOOTH_EVENT_AUDIO_BASE ((int)(BLUETOOTH_EVENT_GATT_BASE + 0x0020))
								/**< Base ID for Audio events */
#define BLUETOOTH_EVENT_HID_BASE ((int)(BLUETOOTH_EVENT_AUDIO_BASE + 0x0030))
								/**< Base ID for HID events */
#define BLUETOOTH_EVENT_ADVERTISING_BASE ((int)(BLUETOOTH_EVENT_HID_BASE + 0x0020))
								/**< Base ID for Advertising events */
#define BLUETOOTH_EVENT_PBAP_CLIENT_BASE ((int)(BLUETOOTH_EVENT_ADVERTISING_BASE + 0x0020))
								/**< Base ID for PBAP Client events */
#define BLUETOOTH_EVENT_AVRCP_CONTROL_BASE ((int)(BLUETOOTH_EVENT_PBAP_CLIENT_BASE + 0x0020))
								/**< Base ID for AVRCP events */
#define BLUETOOTH_EVENT_IPSP_BASE ((int)(BLUETOOTH_EVENT_AVRCP_CONTROL_BASE + 0x0020))
								/**< Base ID for IPSP events */
#define BLUETOOTH_EVENT_MAP_BASE  ((int)(BLUETOOTH_EVENT_IPSP_BASE + 0x0020))
								/**< Base ID for MAP events */
#define BLUETOOTH_EVENT_TYPE_TELEPHONY_BASE	(unsigned int)(0x00500)

typedef enum {
	BLUETOOTH_EVENT_TELEPHONY_ANSWER_CALL = BLUETOOTH_EVENT_TYPE_TELEPHONY_BASE,
	BLUETOOTH_EVENT_TELEPHONY_RELEASE_CALL,
	BLUETOOTH_EVENT_TELEPHONY_REJECT_CALL,
	BLUETOOTH_EVENT_TELEPHONY_CHLD_0_RELEASE_ALL_HELD_CALL,
	BLUETOOTH_EVENT_TELEPHONY_CHLD_1_RELEASE_ALL_ACTIVE_CALL,
	BLUETOOTH_EVENT_TELEPHONY_CHLD_2_ACTIVE_HELD_CALL,
	BLUETOOTH_EVENT_TELEPHONY_CHLD_3_MERGE_CALL,
	BLUETOOTH_EVENT_TELEPHONY_CHLD_4_EXPLICIT_CALL_TRANSFER,
	BLUETOOTH_EVENT_TELEPHONY_SEND_DTMF,
	BLUETOOTH_EVENT_TELEPHONY_HFP_CONNECTED,
	BLUETOOTH_EVENT_TELEPHONY_HFP_DISCONNECTED,
	BLUETOOTH_EVENT_TELEPHONY_AUDIO_CONNECTED,
	BLUETOOTH_EVENT_TELEPHONY_AUDIO_DISCONNECTED,
	BLUETOOTH_EVENT_TELEPHONY_SET_SPEAKER_GAIN,
	BLUETOOTH_EVENT_TELEPHONY_SET_MIC_GAIN,
	BLUETOOTH_EVENT_TELEPHONY_NREC_CHANGED,
	BLUETOOTH_EVENT_TELEPHONY_VENDOR_AT_CMD,
} bluetooth_telephony_event_type;

/**
 * Bluetooth event type
 */
typedef enum {
	BLUETOOTH_EVENT_NONE = BLUETOOTH_EVENT_BASE,/**< No event */

	BLUETOOTH_EVENT_ENABLED,		    /**< Bluetooth event adpater enabled */
	BLUETOOTH_EVENT_DISABLED,		    /**< Bluetooth event adpater disabled */
	BLUETOOTH_EVENT_LE_ENABLED,		    /**< Bluetooth event adpater enabled */
	BLUETOOTH_EVENT_LE_DISABLED,		    /**< Bluetooth event adpater disabled */
	BLUETOOTH_EVENT_LOCAL_NAME_CHANGED,	    /**< Bluetooth event local name changed*/
	BLUETOOTH_EVENT_DISCOVERABLE_TIMEOUT_REQUESTED,
					/**< Bluetooth event Discoverable timeout requested*/
	BLUETOOTH_EVENT_DISCOVERABLE_MODE_CHANGED,  /**< Bluetooth event mode changed */
	BLUETOOTH_EVENT_DISCOVERY_OPTION_REQUESTED, /**< Bluetooth event discovery option */
	BLUETOOTH_EVENT_DISCOVERY_STARTED,	    /**< Bluetooth event discovery started */
	BLUETOOTH_EVENT_DISCOVERY_FINISHED,	    /**< Bluetooth event discovery finished */
	BLUETOOTH_EVENT_REMOTE_DEVICE_FOUND,	    /**< Bluetooth event remote deice found */
	BLUETOOTH_EVENT_LE_DISCOVERY_STARTED,		/**< Bluetooth event LE discovery started */
	BLUETOOTH_EVENT_LE_DISCOVERY_FINISHED,	/**< Bluetooth event LE discovery finished */
	BLUETOOTH_EVENT_REMOTE_LE_DEVICE_FOUND,	    /**< Bluetooth event remote deice found (LE dev) */
	BLUETOOTH_EVENT_REMOTE_DEVICE_NAME_UPDATED,/**< Bluetooth event remote device name updated*/
	BLUETOOTH_EVENT_BONDING_FINISHED,	    /**< Bluetooth event bonding completed */
	BLUETOOTH_EVENT_BONDED_DEVICE_REMOVED,	    /**< Bluetooth event bonding removed */
	BLUETOOTH_EVENT_BONDED_DEVICE_FOUND,	    /**< Bluetooth event paired device found */
	BLUETOOTH_EVENT_REMOTE_DEVICE_READ,	    /**< Bluetooth event read remote device */
	BLUETOOTH_EVENT_DEVICE_AUTHORIZED,	    /**< Bluetooth event authorize device */
	BLUETOOTH_EVENT_DEVICE_UNAUTHORIZED,	    /**< Bluetooth event unauthorize device */
	BLUETOOTH_EVENT_DISCOVERABLE_TIMEOUT_CHANGED,  /**< Bluetooth event mode changed */
	BLUETOOTH_EVENT_KEYBOARD_PASSKEY_DISPLAY,	/**Bluetooth event for displaying keyboard  passkey to user*/
	BLUETOOTH_EVENT_PIN_REQUEST,	/**Bluetooth event for PIN input by user*/
	BLUETOOTH_EVENT_PASSKEY_REQUEST,	/**Bluetooth event for entering Passkey by user*/
	BLUETOOTH_EVENT_PASSKEY_CONFIRM_REQUEST,	/**Bluetooth event for Passkey confirmation by user*/
	BLUETOOTH_EVENT_CONNECTABLE_CHANGED,	    /**< Bluetooth event connectable changed */

	BLUETOOTH_EVENT_RSSI_ENABLED,		/**< Bluetooth event RSSI monitoring enabled */
	BLUETOOTH_EVENT_RSSI_ALERT,				/**< Bluetooth event RSSI Alert */
	BLUETOOTH_EVENT_RAW_RSSI,				/**< Bluetooth event Raw RSSI */
	BLUETOOTH_EVENT_SUPPORTED_PROFILE_TRUSTED,	/**< Bluetooth event Supported Profile Trusted */
	BLUETOOTH_EVENT_PASSKEY_NOTIFICATION,	    /**< Bluetooth event passkey notification */

	BLUETOOTH_EVENT_SERVICE_SEARCHED = BLUETOOTH_EVENT_SDP_BASE,
						    /**< Bluetooth event serice search base id */
	BLUETOOTH_EVENT_SERVICE_SEARCH_CANCELLED,   /**< Bluetooth event service search cancelled */
	BLUETOOTH_EVENT_RFCOMM_DATA_RECEIVED = BLUETOOTH_EVENT_RFCOMM_BASE,
							/**< RFCOMM data receive event */
	BLUETOOTH_EVENT_RFCOMM_CONNECTED,		/**< Rfcomm server incomming connection */
	BLUETOOTH_EVENT_RFCOMM_DISCONNECTED,		/**< Rfcomm server/client disconnect */

	BLUETOOTH_EVENT_RFCOMM_AUTHORIZE,

	BLUETOOTH_EVENT_DEVICE_CONNECTED,	    /**< Bluetooth event device connected */
	BLUETOOTH_EVENT_DEVICE_DISCONNECTED,	    /**< Bluetooth event device disconnected */

	BLUETOOTH_EVENT_RFCOMM_SERVER_REMOVED,

	BLUETOOTH_EVENT_NETWORK_SERVER_ACTIVATED = BLUETOOTH_EVENT_NETWORK_BASE,
								/**< Bluetooth Network event */
	BLUETOOTH_EVENT_NETWORK_SERVER_DEACTIVATED, /**< Network server deactivated */
	BLUETOOTH_EVENT_NETWORK_SERVER_CONNECTED,     /**< Network connected event in server */
	BLUETOOTH_EVENT_NETWORK_SERVER_DISCONNECTED,
						   /**< Network disconnected evnet in server */

	BLUETOOTH_EVENT_NETWORK_CONNECTED,		/**< Network connected event in client*/
	BLUETOOTH_EVENT_NETWORK_DISCONNECTED,		/**< Network disconnected evnet in client*/

	BLUETOOTH_EVENT_HDP_CONNECTED
			= BLUETOOTH_EVENT_HDP_BASE,		   /**<HDP Connect>*/
	BLUETOOTH_EVENT_HDP_DISCONNECTED,	   /**<HDP Disconnect>*/
	BLUETOOTH_EVENT_HDP_DATA_RECEIVED,	   /**<HDP Data Indication>*/

	BLUETOOTH_EVENT_OPC_CONNECTED = BLUETOOTH_EVENT_OPC_BASE,
								/* OPC Connected event */
	BLUETOOTH_EVENT_OPC_DISCONNECTED,		/* OPC Disonnected event */
	BLUETOOTH_EVENT_OPC_TRANSFER_STARTED,	/* OPC Transfer started event */
	BLUETOOTH_EVENT_OPC_TRANSFER_PROGRESS,	/* OPC Transfer progress event */
	BLUETOOTH_EVENT_OPC_TRANSFER_COMPLETE,	/* OPC Transfer Complete event */

	BLUETOOTH_EVENT_MAP_CONNECTED = BLUETOOTH_EVENT_MAP_BASE,
	BLUETOOTH_EVENT_MAP_DISCONNECTED,
	/*
	BLUETOOTH_EVENT_MAP_SET_FOLDER_COMPLETE,
	BLUETOOTH_EVENT_MAP_SET_FOLDER_INVALID_ARGUMENTS,
	BLUETOOTH_EVENT_MAP_SET_FOLDER_FAILED,
	BLUETOOTH_EVENT_MAP_UPDATE_INBOX_COMPLETE,
	BLUETOOTH_EVENT_MAP_UPDATE_INBOX_FAILED,
	*/
	BLUETOOTH_EVENT_MAP_LIST_FOLDERS_COMPLETE,
	BLUETOOTH_EVENT_MAP_LIST_FOLDERS_INVALID_ARGUMENTS,
	BLUETOOTH_EVENT_MAP_LIST_FOLDERS_FAILED,
	BLUETOOTH_EVENT_MAP_LIST_MESSAGES_COMPLETE,
	BLUETOOTH_EVENT_MAP_LIST_MESSAGES_INVALID_ARGUMENTS,
	BLUETOOTH_EVENT_MAP_LIST_MESSAGES_FAILED,
	BLUETOOTH_EVENT_MAP_PUSH_MESSAGE_COMPLETE,
	BLUETOOTH_EVENT_MAP_PUSH_MESSAGE_INVALID_ARGUMENTS,
	BLUETOOTH_EVENT_MAP_PUSH_MESSAGE_FAILED,
	BLUETOOTH_EVENT_MAP_GET_MESSAGE_COMPLETE,
	BLUETOOTH_EVENT_MAP_GET_MESSAGE_INVALID_ARGUMENTS,
	BLUETOOTH_EVENT_MAP_GET_MESSAGE_FAILED,
	BLUETOOTH_EVENT_MAP_LIST_FILTER_FIELD_COMPLETE,

	BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_AUTHORIZE = BLUETOOTH_EVENT_OBEX_SERVER_BASE,
								/* Obex server authorize event*/
	BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_STARTED,	/* Obex Server transfer started event*/
	BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_PROGRESS,/* Obex Server transfer progress event*/
	BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_COMPLETED,/* Obex Server transfer complete event*/
	BLUETOOTH_EVENT_OBEX_SERVER_CONNECTION_AUTHORIZE,
	BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_CONNECTED, /* Obex Transfer connected event */
	BLUETOOTH_EVENT_OBEX_SERVER_TRANSFER_DISCONNECTED, /* Obex Transfer disconnected event */

	BLUETOOTH_EVENT_GATT_SVC_CHAR_DISCOVERED = BLUETOOTH_EVENT_GATT_BASE,
				/**<Discovered GATT service characteristics event*/
	BLUETOOTH_EVENT_GATT_CHAR_VAL_CHANGED,
				/**<Remote GATT charateristic value changed event*/
	BLUETOOTH_EVENT_GATT_GET_CHAR_FROM_UUID,
	BLUETOOTH_EVENT_GATT_READ_CHAR, /**<Gatt Read Characteristic Value */
	BLUETOOTH_EVENT_GATT_WRITE_CHAR, /**<Gatt Write Characteristic Value */
	BLUETOOTH_EVENT_GATT_READ_DESC, /**<Gatt Read Characteristic Descriptor Value */
	BLUETOOTH_EVENT_GATT_WRITE_DESC, /**<Gatt Write Characteristic Descriptor Value */
	BLUETOOTH_EVENT_GATT_SVC_CHAR_DESC_DISCOVERED, /**<Gatt Char Descriptors Discovered Event*/
#ifdef TIZEN_GATT_CLIENT
	BLUETOOTH_EVENT_GATT_SERVER_CONNECTED,/**<Local Gatt Server connected event */
	BLUETOOTH_EVENT_GATT_SERVER_DISCONNECTED, /**<Local Gatt Server Disconnected event */
	BLUETOOTH_EVENT_GATT_CLIENT_CONNECTED,/**<Local Gatt Client connected event */
	BLUETOOTH_EVENT_GATT_CLIENT_DISCONNECTED, /**<Local Gatt Client Disconnected event */
#else
	BLUETOOTH_EVENT_GATT_CONNECTED,/**<Gatt connected event */
	BLUETOOTH_EVENT_GATT_DISCONNECTED, /**<Gatt Disconnected event */
#endif
	BLUETOOTH_EVENT_GATT_ATT_MTU_CHANGED, /**<Attribute protocol MTU changed event */
	BLUETOOTH_EVENT_GATT_SERVER_CHARACTERISTIC_VALUE_CHANGED, /**<Gatt Char write callback event */
	BLUETOOTH_EVENT_GATT_SERVER_READ_REQUESTED, /** <GATT Characteristic/Descriptor Read Request event */
	BLUETOOTH_EVENT_GATT_SERVER_VALUE_CHANGED, /** <GATT Characteristic/Descriptor Value change event */
	BLUETOOTH_EVENT_GATT_SERVER_NOTIFICATION_STATE_CHANGED, /** <GATT Characteristic Notification change event */
	BLUETOOTH_EVENT_GATT_SERVER_NOTIFICATION_COMPLETED, /** <GATT Characteristic Notification or Indication completed event */
	BLUETOOTH_EVENT_GATT_CLIENT_SERVICE_CHANGED, /** <GATT Client service change event */
	BLUETOOTH_EVENT_GATT_SERVER_ACQUIRE_WRITE, /** <GATT Characteristic/Descriptor Value change event */
	BLUETOOTH_EVENT_GATT_SERVER_ACQUIRE_NOTIFY,
	BLUETOOTH_EVENT_AG_CONNECTED = BLUETOOTH_EVENT_AUDIO_BASE, /**<AG service connected event*/
	BLUETOOTH_EVENT_AG_DISCONNECTED, /**<AG service disconnected event*/
	BLUETOOTH_EVENT_AG_SPEAKER_GAIN, /**<Speaker gain request event*/
	BLUETOOTH_EVENT_AG_MIC_GAIN, /**<Mic gain request event*/
	BLUETOOTH_EVENT_AG_AUDIO_CONNECTED, /**<AV & AG service connected event*/
	BLUETOOTH_EVENT_AG_AUDIO_DISCONNECTED,  /**<AV & AG service disconnected event*/
	BLUETOOTH_EVENT_AV_CONNECTED, /**<AV service connected event*/
	BLUETOOTH_EVENT_AV_DISCONNECTED, /**<AV service disconnected event*/
	BLUETOOTH_EVENT_AV_SOURCE_CONNECTED, /**<AV Source device connected event */
	BLUETOOTH_EVENT_AV_SOURCE_DISCONNECTED, /**<AV Source device disconnected event */
	BLUETOOTH_EVENT_AVRCP_CONNECTED, /**<AVRCP service connected event*/
	BLUETOOTH_EVENT_AVRCP_DISCONNECTED, /**<AVRCP service disconnected event*/
	BLUETOOTH_EVENT_AVRCP_SETTING_SHUFFLE_STATUS, /**<AVRCP service player suffle  status event*/
	BLUETOOTH_EVENT_AVRCP_SETTING_EQUALIZER_STATUS, /**<AVRCP service player equalizer status event*/
	BLUETOOTH_EVENT_AVRCP_SETTING_REPEAT_STATUS, /**<AVRCP service player repeat status event*/
	BLUETOOTH_EVENT_AVRCP_SETTING_SCAN_STATUS, /**<AVRCP service player scan status event*/
	BLUETOOTH_EVENT_AVRCP_DELAY_CHANGED, /**<AVRCP service transport delay changed event*/
	BLUETOOTH_EVENT_HF_CONNECTED,
	BLUETOOTH_EVENT_HF_DISCONNECTED,
	BLUETOOTH_EVENT_HF_AUDIO_CONNECTED,
	BLUETOOTH_EVENT_HF_AUDIO_DISCONNECTED,
	BLUETOOTH_EVENT_HF_RING_INDICATOR,
	BLUETOOTH_EVENT_HF_CALL_WAITING,
	BLUETOOTH_EVENT_HF_CALL_TERMINATED,
	BLUETOOTH_EVENT_HF_CALL_STARTED,
	BLUETOOTH_EVENT_HF_CALL_ENDED,
	BLUETOOTH_EVENT_HF_CALL_UNHOLD,
	BLUETOOTH_EVENT_HF_CALL_SWAPPED,
	BLUETOOTH_EVENT_HF_CALL_ON_HOLD,
	BLUETOOTH_EVENT_HF_CALL_STATUS,
	BLUETOOTH_EVENT_HF_VOICE_RECOGNITION_ENABLED,
	BLUETOOTH_EVENT_HF_VOICE_RECOGNITION_DISABLED,
	BLUETOOTH_EVENT_HF_VOLUME_SPEAKER,
	BLUETOOTH_EVENT_HF_VENDOR_DEP_CMD,
	BLUETOOTH_EVENT_HF_CALLSETUP_INCOMING,
	BLUETOOTH_EVENT_HF_CALL_FAILED_TO_DIAL,
	BLUETOOTH_EVENT_HF_CALL_IDLE,
	BLUETOOTH_EVENT_HF_CALLSETUP_DIALING,
	BLUETOOTH_EVENT_HF_CALLSETUP_ALERTING,
	BLUETOOTH_EVENT_HF_CIEV_DEVICE_STATUS_CHANGED,

	BLUETOOTH_HID_CONNECTED = BLUETOOTH_EVENT_HID_BASE, /**< Input connectd event*/
	BLUETOOTH_HID_DISCONNECTED, /**< Input disconnectd event*/
	BLUETOOTH_HID_DEVICE_CONNECTED, /**< HID Device connected event*/
	BLUETOOTH_HID_DEVICE_DISCONNECTED, /**< HID Device disconnected event*/
	BLUETOOTH_HID_DEVICE_DATA_RECEIVED, /**< HID Device data received event*/

	BLUETOOTH_EVENT_ADVERTISING_STARTED = BLUETOOTH_EVENT_ADVERTISING_BASE, /**< Advertising started event */
	BLUETOOTH_EVENT_ADVERTISING_STOPPED, /**< Advertising stopped event */
	BLUETOOTH_EVENT_ADVERTISING_MANUFACTURER_DATA_CHANGED, /**< Advertising manufacturer data changed event */
	BLUETOOTH_EVENT_SCAN_RESPONSE_MANUFACTURER_DATA_CHANGED, /**< Scan response manufacturer data changed event */
	BLUETOOTH_EVENT_MANUFACTURER_DATA_CHANGED, /**< Manufacturer data changed event */
	BLUETOOTH_EVENT_DEVICE_ERROR, /**< Hardware error */
	BLUETOOTH_EVENT_TX_TIMEOUT_ERROR, /** TX Timeout Error*/
	BLUETOOTH_EVENT_MAX, /**< Bluetooth event Max value */

	BLUETOOTH_PBAP_CONNECTED = BLUETOOTH_EVENT_PBAP_CLIENT_BASE, /**< PBAP connected event*/
	BLUETOOTH_PBAP_DISCONNECTED, /**< PBAP disconnectd event*/
	BLUETOOTH_PBAP_PHONEBOOK_SIZE, /**< PBAP Phonebook Size event*/
	BLUETOOTH_PBAP_PHONEBOOK_PULL, /**< PBAP Phonebook Pull event*/
	BLUETOOTH_PBAP_VCARD_LIST, /**< PBAP vCard List event*/
	BLUETOOTH_PBAP_VCARD_PULL, /**< PBAP vCard Pull event*/
	BLUETOOTH_PBAP_PHONEBOOK_SEARCH, /**< PBAP Phonebook Search event*/

	BLUETOOTH_EVENT_AVRCP_CONTROL_CONNECTED = BLUETOOTH_EVENT_AVRCP_CONTROL_BASE, /**<AVRCP service connected event*/
	BLUETOOTH_EVENT_AVRCP_CONTROL_DISCONNECTED, /**<AVRCP service disconnected event*/
	BLUETOOTH_EVENT_AVRCP_CONTROL_SHUFFLE_STATUS, /**<AVRCP control suffle  status event*/
	BLUETOOTH_EVENT_AVRCP_CONTROL_EQUALIZER_STATUS, /**<AVRCP control equalizer status event*/
	BLUETOOTH_EVENT_AVRCP_CONTROL_REPEAT_STATUS, /**<AVRCP control repeat status event*/
	BLUETOOTH_EVENT_AVRCP_CONTROL_SCAN_STATUS, /**<AVRCP control scan status event*/
	BLUETOOTH_EVENT_AVRCP_SONG_POSITION_STATUS, /**<AVRCP control play Postion status event*/
	BLUETOOTH_EVENT_AVRCP_PLAY_STATUS_CHANGED, /**<AVRCP control play status event*/
	BLUETOOTH_EVENT_AVRCP_TRACK_CHANGED, /**<AVRCP control song metadata event*/

	BLUETOOTH_EVENT_IPSP_INIT_STATE_CHANGED = BLUETOOTH_EVENT_IPSP_BASE, /**<IPSP init event*/
	BLUETOOTH_EVENT_IPSP_CONNECTED, /**< IPSP connected event  */
	BLUETOOTH_EVENT_IPSP_DISCONNECTED, /**< IPSP Disconnected event */
	BLUETOOTH_EVENT_IPSP_INTERFACE_INFO, /** IPSP BT Interface Info after connection */
	BLUETOOTH_EVENT_LE_DATA_LENGTH_CHANGED,  /** LE data length values changed */
	BLUETOOTH_EVENT_PXP_PROPERTY_CHANGED, /** Proximity property changed */
	BLUETOOTH_EVENT_TDS_ACTIVATION_REQUESTED, /** TDS activation requested */
	BLUETOOTH_EVENT_TDS_TRANSPORT_DATA_RECEIVED,  /** TDS Transport Data received */
	BLUETOOTH_EVENT_TDS_ACTIVATION_RESULT,        /** TDS Activation Result */
	BLUETOOTH_EVENT_TDS_CONTROL_POINT_ENABLED,    /** TDS CCCD enabled event */
	BLUETOOTH_EVENT_TDS_ACTIVATION_INDICATION,    /** TDS Activation Indication from Provider */

	BLUETOOTH_EVENT_OTP_SERVER_STATE_CHANGED,	/* OTP Server Status **/
	BLUETOOTH_EVENT_OTP_READ_CHAR_VAL,	/* OTP Read Value Response */
	BLUETOOTH_EVENT_OTP_NOTIFICATION_ENABLED,	/* OTP Notification Enabled Response */
	BLUETOOTH_EVENT_OTP_WRITE_CHAR_VAL,	/* OTP Write Value Response */
	BLUETOOTH_EVENT_OTP_INDICATION,	/* OTP Indication */
	BLUETOOTH_EVENT_OTC_STATE_CHANGED,		/* OTC Connection State Changed Event */
} bluetooth_event_type_t;

/**
 * Stucture to hold event information
 */
typedef struct {
	int event; /**< event type */
	int result; /**< Success or error value */
	void *param_data; /**<parameter data pointer */
	//void *user_data;
} bluetooth_event_param_t;

/**
 * Callback pointer type
 */
typedef void (*bluetooth_cb_func_ptr)(int event, bluetooth_event_param_t *param);

void __bt_event_proxy(int event, bluetooth_event_param_t *param);
void __bt_le_event_proxy(int event, bluetooth_event_param_t *param);

typedef enum {
	BT_GATT_ROLE_SERVER = 0x01,
	BT_GATT_ROLE_CLIENT = 0x02,
} bt_gatt_role_e;

/**
 * @internal
 */
typedef enum {
	BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_16_BIT_SERVICE_CLASS_UUIDS = 0x02, /**<Incomplete list of 16 bit UUIDs */
	BT_ADAPTER_LE_ADVERTISING_DATA_COMP_LIST_16_BIT_SERVICE_CLASS_UUIDS = 0x03, /**< Complete list of 16 bit UUIDs */
	BT_ADAPTER_LE_ADVERTISING_DATA_INCOMP_LIST_128_BIT_SERVICE_CLASS_UUIDS = 0x06, /**< Incomplete list of 128 bit UUIDs */
	BT_ADAPTER_LE_ADVERTISING_DATA_COMP_LIST_128_BIT_SERVICE_CLASS_UUIDS = 0x07, /**< Complete list of 128 bit UUID */
	BT_ADAPTER_LE_ADVERTISING_DATA_SHORT_LOCAL_NAME = 0x08, /**<Shortened local name */
	BT_ADAPTER_LE_ADVERTISING_DATA_LOCAL_NAME = 0x09, /**<Complete local name */
	BT_ADAPTER_LE_ADVERTISING_DATA_TX_POWER_LEVEL = 0x0a, /**< TX-Power level*/
	BT_ADAPTER_LE_ADVERTISING_DATA_16_BIT_SERVICE_SOLICITATION_UUIDS = 0x14, /**< List of 16-bit Service Solicitation UUIDs*/
	BT_ADAPTER_LE_ADVERTISING_DATA_128_BIT_SERVICE_SOLICITATION_UUIDS = 0x15, /**< List of 128-bit Service Solicitation UUIDs*/
	BT_ADAPTER_LE_ADVERTISING_DATA_SERVICE_DATA = 0x16, /**< Service data */
	BT_ADAPTER_LE_ADVERTISING_DATA_APPEARANCE = 0x19, /**< Appearance*/
	BT_ADAPTER_LE_ADVERTISING_DATA_TRANSPORT_DISCOVERY = 0x26, /**< Transport Discovery*/
	BT_ADAPTER_LE_ADVERTISING_DATA_MANUFACTURER_SPECIFIC_DATA = 0xff, /**< Manufacturer data */
} bt_adapter_le_advertising_data_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising type.
 * @since_tizen @if WEARABLE @else 2.3 @endif
 */
typedef enum {
	BT_ADAPTER_LE_ADVERTISING_CONNECTABLE = 0x00, /**< Connectable undirected advertising (ADV_IND) */
	BT_ADAPTER_LE_ADVERTISING_CONNECTABLE_DIRECT_HIGH = 0x01, /**< Connectable high duty cycle directed advertising (ADV_DIRECT_IND) */
	BT_ADAPTER_LE_ADVERTISING_SCANNABLE = 0x02, /**< Scannable undirected advertising (ADV_SCAN_IND) */
	BT_ADAPTER_LE_ADVERTISING_NON_CONNECTABLE = 0x03, /**< Non connectable undirected advertising (ADV_NONCOND_IND) */
	BT_ADAPTER_LE_ADVERTISING_CONNECTABLE_DIRECT_LOW = 0x04, /**< Connectable low duty cycle directed advertising (ADV_DIRECT_IND) */
} bt_adapter_le_advertising_type_e;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when trying to be displayed the passkey.
 * @since_tizen 3.0
 *
 * @param[in]   remote_address	The address of remote device
 * @param[in]   passkey	The passkey to be paired with remote device
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when trying to be displayed the passkey
 * if callback is registered using bt_adapter_set_passkey_notification().
 * @see bt_adapter_set_passkey_notification()
 * @see bt_adapter_unset_passkey_notification()
 */
typedef void (*bt_adapter_passkey_notification_cb)(const char *remote_address, const char *passkey, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked
 * when trying to be displayed the passkey.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_passkey_notification_cb() will be invoked.
 *
 * @see bt_initialize()
 */
int bt_adapter_set_passkey_notification(bt_adapter_passkey_notification_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	Unregisters the callback function.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_set_passkey_notification()
 */
int bt_adapter_unset_passkey_notification(void);

/**
 * @internal
 */
typedef struct {
	bt_adapter_le_advertising_mode_e mode;
	bt_adapter_le_advertising_filter_policy_e filter_policy;
	bt_adapter_le_advertising_type_e type;
} bt_adapter_le_advertising_parameters_s;

#ifdef GLIB_SUPPORTED
typedef struct {
	int handle;

	bt_adapter_le_advertising_state_changed_cb cb;
	void *user_data;

	bt_adapter_le_advertising_parameters_s adv_params;

	unsigned int adv_data_len;
	char *adv_data;
	unsigned int adv_system_data_len;

	unsigned int scan_rsp_data_len;
	char *scan_rsp_data;
	unsigned int scan_rsp_system_data_len;
} bt_advertiser_s;
#else
struct bt_advertiser_t {
	struct bt_advertiser_t *flink; /* flink is required to use sq_queue_t */
	int handle;

	bt_adapter_le_advertising_state_changed_cb cb;
	void *user_data;

	bt_adapter_le_advertising_parameters_s adv_params;

	unsigned int adv_data_len;
	char *adv_data;
	unsigned int adv_system_data_len;

	unsigned int scan_rsp_data_len;
	char *scan_rsp_data;
	unsigned int scan_rsp_system_data_len;
} bt_advertiser_t;
typedef struct bt_advertiser_t bt_advertiser_s;
#endif

#ifndef GLIB_SUPPORTED
struct bt_device_info_t {
	struct bt_device_info_t *flink; /* flink is required to use sq_queue_t */
	bt_device_info_s *dev_info;
};
#endif

typedef struct {
	int slot_id;
	char *device_address;
	char *device_name;
	char *service_uuid;
	char *service_uuid_mask;
	char *service_solicitation_uuid;
	char *service_solicitation_uuid_mask;
	char *service_data_uuid;
	char *service_data;
	unsigned int service_data_len;
	char *service_data_mask;
	int manufacturer_id;
	char *manufacturer_data;
	unsigned int manufacturer_data_len;
	char *manufacturer_data_mask;
} bt_le_scan_filter_s;

/**
 * @internal
 */
typedef struct bt_event_sig_event_slot_s {
	const void *callback;
	void *user_data;
} bt_event_sig_event_slot_s;

#ifdef GLIB_SUPPORTED
typedef struct {
	GSList *services;
} bt_gatt_server_s;
#else
struct bt_gatt_server_t {
	struct bt_gatt_server_t *flink;
	sq_queue_t *services;
};
typedef struct bt_gatt_server_t bt_gatt_server_s;
#endif

#ifdef GLIB_SUPPORTED
typedef struct {
	GSList *services;
	char *remote_address;
	bool services_discovered;
	bool connected;

	bt_gatt_client_att_mtu_changed_cb att_mtu_changed_cb;
	bt_gatt_client_service_changed_cb service_changed_cb;
	void *service_changed_user_data;
	void *att_mtu_changed_user_data;
	int client_id;
} bt_gatt_client_s;
#else
struct bt_gatt_client_t {
	struct bt_gatt_client_t *flink; /* flink is required to use sq_queue_t */
	sq_queue_t *services;
	char *remote_address;
	bool services_discovered;
	bool connected;

	bt_gatt_client_att_mtu_changed_cb att_mtu_changed_cb;
	bt_gatt_client_service_changed_cb service_changed_cb;
	void *service_changed_user_data;
	void *att_mtu_changed_user_data;
	int client_id;
};
typedef struct bt_gatt_client_t bt_gatt_client_s;
#endif

#ifdef GLIB_SUPPORTED
typedef struct {
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;
} bt_gatt_common_s;
#else
struct bt_gatt_common_t {
	struct bt_gatt_common_t *flink; /* flink is required to use sq_queue_t */
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;
};
typedef struct bt_gatt_common_t bt_gatt_common_s;
#endif

#ifdef GLIB_SUPPORTED
typedef struct {
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;
	int numhandles;

	bool is_included_service;

	bt_gatt_service_type_e service_type;

	GSList *included_services;
	GSList *characteristics;

	int instance_id;  /* Instance ID of the service object */
	bt_gatt_handle_info_t svc_include_handles;
	bt_gatt_handle_info_t charc_handles;

	char **include_handles;
	char **char_handles;
} bt_gatt_service_s;
#else
struct bt_gatt_service_t {
	struct bt_gatt_service_t *flink; /* flink is required to use sq_queue_t */
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;
	int numhandles;

	bool is_included_service;

	bt_gatt_service_type_e service_type;

	sq_queue_t *included_services;
	sq_queue_t *characteristics;

	int instance_id;  /* Instance ID of the service object */
	bt_gatt_handle_info_t svc_include_handles;
	bt_gatt_handle_info_t charc_handles;

	char **include_handles;
	char **char_handles;
};
typedef struct bt_gatt_service_t bt_gatt_service_s;
#endif

#ifdef GLIB_SUPPORTED
typedef struct {
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;
	unsigned char addr[BT_ADDR_HEX_LEN];

	int permissions;
	int properties;
	bt_gatt_write_type_e write_type;

	GSList *descriptors;

	int instance_id;  /* Instance ID of the characteristic object */
	bt_gatt_handle_info_t descriptor_handles;

	char **desc_handles;

	bt_gatt_client_characteristic_value_changed_cb value_changed_cb;
	void *value_changed_user_data;

	bt_gatt_server_write_value_requested_cb write_value_requested_cb;
	void *write_value_requested_user_data;

	bt_gatt_server_read_value_requested_cb read_requested_cb;
	void *read_requested_user_data;

	bt_gatt_server_notification_sent_cb notified_cb;
	void *notified_user_data;

	bt_gatt_server_characteristic_notification_state_changed_cb notification_changed_cb;
	void *notification_changed_user_data;

	int value_length;
	char *value;
	bt_gatt_client_request_completed_cb read_cb;
	void *read_user_data;

	bt_gatt_client_request_completed_cb write_cb;
	void *write_user_data;
} bt_gatt_characteristic_s;
#else
struct bt_gatt_characteristic_t {
	struct bt_gatt_characteristic_t *flink; /* flink is required to use sq_queue_t */
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;
	unsigned char addr[BT_ADDR_HEX_LEN];

	int permissions;
	int properties;
	bt_gatt_write_type_e write_type;

	sq_queue_t *descriptors;

	int instance_id;  /* Instance ID of the characteristic object */
	bt_gatt_handle_info_t descriptor_handles;

	char **desc_handles;

	bt_gatt_client_characteristic_value_changed_cb value_changed_cb;
	void *value_changed_user_data;

	bt_gatt_server_write_value_requested_cb write_value_requested_cb;
	void *write_value_requested_user_data;

	bt_gatt_server_read_value_requested_cb read_requested_cb;
	void *read_requested_user_data;

	bt_gatt_server_notification_sent_cb notified_cb;
	void *notified_user_data;

	bt_gatt_server_characteristic_notification_state_changed_cb notification_changed_cb;
	void *notification_changed_user_data;

	int value_length;
	char *value;
	bt_gatt_client_request_completed_cb read_cb;
	void *read_user_data;

	bt_gatt_client_request_completed_cb write_cb;
	void *write_user_data;
};
typedef struct bt_gatt_characteristic_t bt_gatt_characteristic_s;
#endif

#ifdef GLIB_SUPPORTED
typedef struct {
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;

	int instance_id;  /* Instance ID of the descriptor object */
	int permissions;

	bt_gatt_server_write_value_requested_cb write_value_requested_cb;
	void *write_value_requested_user_data;

	bt_gatt_server_read_value_requested_cb read_requested_cb;
	void *read_requested_user_data;

	int value_length;
	char *value;
	bt_gatt_client_request_completed_cb read_cb;
	void *read_user_data;

	bt_gatt_client_request_completed_cb write_cb;
	void *write_user_data;
} bt_gatt_descriptor_s;
#else
struct bt_gatt_descriptor_t {
	struct bt_gatt_descriptor_t *flink; /* flink is required to use sq_queue_t */
	bt_gatt_type_e type;
	bt_gatt_role_e role;
	void *parent;
	char *path;
	char *uuid;
	int handle;

	int instance_id;  /* Instance ID of the descriptor object */
	int permissions;

	bt_gatt_server_write_value_requested_cb write_value_requested_cb;
	void *write_value_requested_user_data;

	bt_gatt_server_read_value_requested_cb read_requested_cb;
	void *read_requested_user_data;

	int value_length;
	char *value;
	bt_gatt_client_request_completed_cb read_cb;
	void *read_user_data;

	bt_gatt_client_request_completed_cb write_cb;
	void *write_user_data;
};
typedef struct bt_gatt_descriptor_t bt_gatt_descriptor_s;
#endif

typedef struct {
	bt_gatt_client_h client;
	bt_gatt_h gatt_handle;
	void *user_data;

	/*
	 * Temp callback. Once bluez's discover service issue is fixed,
	 * it will be removed
	 */
	bt_gatt_client_request_completed_cb cb;
} bt_gatt_client_cb_data_s;

typedef void (*_bt_gatt_client_value_changed_cb)(char *char_path,
		unsigned char *value, int value_length, void *user_data);

typedef void (*_bt_gatt_client_val_changed_cb)(unsigned char *uuid, char *remote_address,
		char *value, int value_length, void *user_data);

void _bt_handle_gatt_client_char_read_completed_event(int result,
		void *resp);

void _bt_handle_gatt_client_desc_read_completed_event(int result,
		void *resp);

void _bt_handle_gatt_client_char_write_completed_event(int result,
		void *resp);

void _bt_handle_gatt_client_desc_write_completed_event(int result,
		void *resp);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when trying to be displayed the passkey.
 * @since_tizen 3.0
 *
 * @param[in]   remote_address	The address of remote device
 * @param[in]   passkey	The passkey to be paired with remote device
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when trying to be displayed the passkey
 * if callback is registered using bt_adapter_set_passkey_notification().
 * @see bt_adapter_set_passkey_notification()
 * @see bt_adapter_unset_passkey_notification()
 */
typedef void (*bt_adapter_passkey_notification_cb)(const char *remote_address, const char *passkey, void *user_data);


#define BT_CHECK_INPUT_PARAMETER(arg) \
	if (arg == NULL) { \
		BT_PRT("INVALID_PARAMETER(0%s=NULL)", #arg); \
		return BT_ERROR_INVALID_PARAMETER; \
	}

#define BT_CHECK_INPUT_PARAMETER_UINT(arg) \
	if (arg < -1) { \
		BT_PRT("INVALID_PARAMETER_UINT(%s=%d)", #arg, arg); \
		return BT_ERROR_INVALID_PARAMETER; \
	}

#define BT_CHECK_INPUT_PARAMETER_BOOL(arg) \
	if (arg < -1 || arg > 1) { \
		BT_PRT("INVALID_PARAMETER_BOOL(%s=%d)", #arg, arg); \
		return BT_ERROR_INVALID_PARAMETER; \
	}

#define BT_FEATURE_COMMON "tizen.org/feature/network.bluetooth"
#define BT_FEATURE_LE "tizen.org/feature/network.bluetooth.le"
#define BT_FEATURE_LE_5_0 "tizen.org/feature/network.bluetooth.le.5_0"
#define BT_FEATURE_IPSP "tizen.org/feature/network.bluetooth.le.ipsp"
#define BT_FEATURE_AUDIO_CALL "tizen.org/feature/network.bluetooth.audio.call"
#define BT_FEATURE_AUDIO_MEDIA "tizen.org/feature/network.bluetooth.audio.media"
#define BT_FEATURE_AUDIO_CONTROLLER "tizen.org/feature/network.bluetooth.audio.controller"
#define BT_FEATURE_HEALTH "tizen.org/feature/network.bluetooth.health"
#define BT_FEATURE_HID_HOST "tizen.org/feature/network.bluetooth.hid"
#define BT_FEATURE_HID_DEVICE "tizen.org/feature/network.bluetooth.hid_device"
#define BT_FEATURE_OPP "tizen.org/feature/network.bluetooth.opp"
#define BT_FEATURE_MAP "tizen.org/feature/network.bluetooth.map"
#define BT_FEATURE_TETHERING "tizen.org/feature/network.tethering.bluetooth"
#define BT_FEATURE_PBAP_CLIENT "tizen.org/feature/network.bluetooth.phonebook.client"
#define BT_FEATURE_OOB "tizen.org/feature/network.bluetooth.oob"
#define BT_FEATURE_GATT_SERVER "tizen.org/feature/network.bluetooth.le.gatt.server"
#define BT_FEATURE_GATT_CLIENT "tizen.org/feature/network.bluetooth.le.gatt.client"

#define BT_CHECK_SUPPORTED_FEATURE(feature_name)

#define BT_CHECK_SUPPORTED_FEATURES(feature_name1, feature_name2)

/**
 * @internal
 * @brief Check the initialzating status
 */
int _bt_check_init_status(void);

#define BT_CHECK_INIT_STATUS() \
	if (_bt_check_init_status() == BT_ERROR_NOT_INITIALIZED) { \
		BT_PRT("NOT_INITIALIZED(0x%08x)", BT_ERROR_NOT_INITIALIZED); \
		return BT_ERROR_NOT_INITIALIZED; \
	}

#define BT_CHECK_ADAPTER_STATUS() \
	if (bluetooth_check_adapter() == BLUETOOTH_ADAPTER_DISABLED) { \
		BT_PRT("BT_ERROR_NOT_ENABLED(0x%08x)", BT_ERROR_NOT_ENABLED); \
		return BT_ERROR_NOT_ENABLED; \
	}

#define FREE(data) \
	if (data) { \
		free(data); \
	}

#define FREEV(data) \
	if (data) { \
		int tmp_i; \
		for (tmp_i = 0; data[tmp_i]; tmp_i++) \
			FREE(data[tmp_i]); \
		FREE(data); \
	}

/**
 * @internal
 * @brief Initialize Bluetooth LE adapter
 */
int _bt_le_adapter_init(void);

/**
 * @internal
 * @brief Deinitialize Bluetooth LE adapter
 */
int _bt_le_adapter_deinit(void);

/**
 * @internal
 * @brief Set the event callback.
 */
void _bt_set_cb(int events, void *callback, void *user_data);

/**
 * @internal
 * @brief Unset the event callback.
 */
void _bt_unset_cb(int events);

/**
 * @internal
 * @brief Check if the event callback exist or not.
 */
bool _bt_check_cb(int events);

/**
 * @internal
 * @brief Convert Bluetooth F/W error codes to capi Bluetooth error codes.
 */
int _bt_get_error_code(int origin_error);

/**
 * @internal
 * @brief Filter Advertising data based on AD type.
 */
int _bt_get_ad_data_by_type(char *in_data, int in_len,
		char in_type, char **data, int *data_len);

/**
 * @internal
 * @brief Free bt_device_info_s.
 */
void _bt_free_bt_device_info_s(bt_device_info_s *device_info);

/**
 * @internal
 * @brief Convert Bluetooth F/W bluetooth_device_address_t to string.
 */
int _bt_convert_address_to_string(char **addr_str, unsigned char *addr);


/**
 * @internal
 * @brief Convert string to Bluetooth F/W bluetooth_device_address_t.
 */
void _bt_convert_address_to_hex(unsigned char **addr, const char *addr_str);

void _handle_gatt_client_read_completed_event(int result, bt_gatt_resp_data_t *resp);

void _handle_gatt_client_write_completed_event(int result, bt_gatt_resp_data_t *resp);


/**
 * @internal
 * @brief Convert error code to string.
 */
char *_bt_convert_error_to_string(int error);

/**
 * @internal
 * @brief Convert uuid to uuid128
 */
char* _bt_convert_uuid_to_uuid128(const char *uuid);

void _bt_adapter_le_invoke_advertising_state_cb(int handle, int result, bt_adapter_le_advertising_state_e adv_state);

bool _bt_gatt_is_legacy_client_mode(void);

bt_gatt_client_h _bt_gatt_get_client(const char *remote_addr);

#ifdef GLIB_SUPPORTED
const GSList *_bt_gatt_get_server_list(void);
#else
const sq_queue_t *_bt_gatt_get_server_list(void);
#endif

bt_gatt_h _bt_gatt_client_add_service(bt_gatt_client_h client,
				const char *uuid, int instance_id);

int _bt_gatt_client_update_services(bt_gatt_client_h client);

int _bt_gatt_client_update_include_services(bt_gatt_h service);

int _bt_gatt_client_update_characteristics(bt_gatt_h service);

int _bt_gatt_client_update_descriptors(bt_gatt_h characteristic);


#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_NETWORK_BLUETOOTH_PRIVATE_H__ */
