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


#ifndef __BT_ADAPTATION_DEVICE_H__
#define __BT_ADAPTATION_DEVICE_H__

#include <bluetooth/bluetooth_type.h>
#include <bluetooth/bluetooth_type_internal.h>
#include "bluetooth_private.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define BLUETOOTH_ADDRESS_LENGTH		6 /**< This specifies bluetooth device address length */
#define BLUETOOTH_ADVERTISING_DATA_LENGTH_MAX	31 /**< This specifies maximum AD data length */
#define BLUETOOTH_DEVICE_NAME_LENGTH_MAX	248 /**< This specifies maximum device name length */
#define BLUETOOTH_MANUFACTURER_DATA_LENGTH_MAX	240 /**< This specifies maximum manufacturer data length */
#define BLUETOOTH_MAX_SERVICES_FOR_DEVICE	40  /**< This specifies maximum number of services
							a device can support */
#define BLUETOOTH_UUID_STRING_MAX 50

/**
 * Connected state types
 */
typedef enum {
	BLUETOOTH_CONNECTED_LINK_NONE = 0x00,
	BLUETOOTH_CONNECTED_LINK_BREDR = 0x01,
	BLUETOOTH_CONNECTED_LINK_LE = 0x02,
	BLUETOOTH_CONNECTED_LINK_BREDR_LE = 0x03,
} bluetooth_connected_link_t;

/**
 * Major device class (part of Class of Device)
 */
typedef enum {
	BLUETOOTH_DEVICE_MAJOR_CLASS_MISC = 0x00,	/**< Miscellaneous major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_COMPUTER = 0x01,		/**< Computer major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_PHONE = 0x02,		/**< Phone major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_LAN_ACCESS_POINT = 0x03,	/**< LAN major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_AUDIO = 0x04,		/**< AUDIO major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_PERIPHERAL = 0x05,		/**< Peripheral major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_IMAGING = 0x06,		/**< Imaging major device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_WEARABLE = 0x07,		/**< Wearable device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_TOY = 0x08,		/**< Toy device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_HEALTH = 0x09,		/**< Health device class*/
	BLUETOOTH_DEVICE_MAJOR_CLASS_UNCLASSIFIED = 0x1F	/**< Unknown major device class*/
} bluetooth_device_major_class_t;

typedef enum {
	BLUETOOTH_DEVICE_MINOR_CLASS_UNCLASSIFIED = 0x00,	/**< unclassified minor class */

	/* About Computer Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_DESKTOP_WORKSTATION = 0x04,	/**< desktop workstation
									minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_SERVER_CLASS_COMPUTER = 0x08,	/**< server minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_LAPTOP = 0x0C,			/**< laptop minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HANDHELD_PC_OR_PDA = 0x10,		/**< PDA minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_PALM_SIZED_PC_OR_PDA = 0x14,	/**< PALM minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_WEARABLE_COMPUTER = 0x18,	/**< Wearable PC minor class */

	/* About Phone Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CELLULAR = 0x04,			/**< Cellular minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CORDLESS = 0x08,			/**< cordless minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_SMART_PHONE = 0x0C,	/**< smart phone minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_WIRED_MODEM_OR_VOICE_GATEWAY = 0x10,
								/**< voice gateway minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_COMMON_ISDN_ACCESS = 0x14,		/**< ISDN minor class */

	/* About LAN/Network Access Point Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_FULLY_AVAILABLE = 0x04,		/**< Fully available minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_1_TO_17_PERCENT_UTILIZED = 0x20,	/**< 1-17% utilized minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_17_TO_33_PERCENT_UTILIZED = 0x40,	/**< 17-33% utilized minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_33_TO_50_PERCENT_UTILIZED = 0x60,	/**< 33-50% utilized minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_50_to_67_PERCENT_UTILIZED = 0x80,	/**< 50-67% utilized minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_67_TO_83_PERCENT_UTILIZED = 0xA0,	/**< 67-83% utilized minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_83_TO_99_PERCENT_UTILIZED = 0xC0,	/**< 83-99% utilized minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_NO_SERVICE_AVAILABLE = 0xE0,		/**< No service available minor class */

	/* About Audio/Video Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HEADSET_PROFILE = 0x04,		/**< Headset minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HANDSFREE = 0x08,			/**< Handsfree minor class*/

	BLUETOOTH_DEVICE_MINOR_CLASS_MICROPHONE = 0x10,		/**< Microphone minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_LOUD_SPEAKER = 0x14,	/**< Loud Speaker minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HEADPHONES = 0x18,		/**< Headphones minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_PORTABLE_AUDIO = 0x1C,	/**< Portable Audio minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CAR_AUDIO = 0x20,		 /**< Car Audio minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_SET_TOP_BOX = 0x24,	/**< Set top box minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HIFI_AUDIO_DEVICE = 0x28,	/**< Hifi minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_VCR = 0x2C,		/**< VCR minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_VIDEO_CAMERA = 0x30,	/**< Video Camera minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CAM_CORDER = 0x34,		/**< CAM Corder minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_VIDEO_MONITOR = 0x38,	/**<Video Monitor minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_VIDEO_DISPLAY_AND_LOUD_SPEAKER = 0x3C,
									/**< Video Display and Loud
									Speaker minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_VIDEO_CONFERENCING = 0x40,	/**< Video Conferencing minor
								class */

	BLUETOOTH_DEVICE_MINOR_CLASS_GAMING_OR_TOY = 0x48,	/**< Gaming or toy minor class */

	/* About Peripheral Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_KEY_BOARD = 0x40,		/**< Key board minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_POINTING_DEVICE = 0x80,	/**< Pointing Device minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_COMBO_KEYBOARD_OR_POINTING_DEVICE = 0xC0,
								/**< Combo Keyboard or pointing
								device minorclass */

	BLUETOOTH_DEVICE_MINOR_CLASS_JOYSTICK = 0x04,		/**< JoyStick minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_GAME_PAD = 0x08,		/**< Game Pad minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_REMOTE_CONTROL = 0x0C,	/**< Remote Control minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_SENSING_DEVICE = 0x10,	/**< Sensing Device minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_DIGITIZER_TABLET = 0x14,	/**< Digitizer minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CARD_READER = 0x18,	/**< Card Reader minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_DIGITAL_PEN = 0x1C,	/**< Digital pen minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HANDHELD_SCANNER = 0x20,	/**< Handheld scanner for bar-codes, RFID minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HANDHELD_GESTURAL_INPUT_DEVICE = 0x24,	/**< Handheld gestural input device minor class */

	/* About Imaging Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_DISPLAY = 0x10,		/**< Display minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CAMERA = 0x20,		/**< Camera minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_SCANNER = 0x40,		/**< Scanner minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_PRINTER = 0x80,		/**< Printer minor class */

	/* About Wearable Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_WRIST_WATCH = 0x04,	/**< Wrist watch minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_PAGER = 0x08,		/**< Pager minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_JACKET = 0x0C,		/**< Jacket minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HELMET = 0x10,		/**< Helmet minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_GLASSES = 0x14,		/**< Glasses minor class */

	/* About Toy Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_ROBOT = 0x04,		/**< Robot minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_VEHICLE = 0x08,		/**< Vehicle minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_DOLL_OR_ACTION = 0x0C,	/**< Doll or Action minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_CONTROLLER = 0x10,		/**< Controller minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_GAME = 0x14,		/**< Game minor class */

	/* About Health Major class */
	BLUETOOTH_DEVICE_MINOR_CLASS_BLOOD_PRESSURE_MONITOR = 0x04,	/**< Blood Pressure minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_THERMOMETER = 0x08,		/**< Thermometer minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_WEIGHING_SCALE = 0x0C,		/**< Weighing Scale minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_GLUCOSE_METER = 0x10,		/**< Glucose minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_PULSE_OXIMETER = 0x14,		/**< Pulse Oximeter minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_HEART_OR_PULSE_RATE_MONITOR = 0x18,/**< Heart or pulse rate monitor minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_MEDICAL_DATA_DISPLAY = 0x1C,	/**< Medical minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_STEP_COUNTER = 0x20,		/**< Step Counter minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_BODY_COMPOSITION_ANALYZER = 0x24,	/**< Body composition analyzer minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_PEAK_FLOW_MONITOR = 0x28,	/**< Peak flow monitor minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_MEDICATION_MONITOR = 0x2C,	/**< Medication monitor minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_KNEE_PROSTHESIS = 0x30,	/**< Knee prosthesis minor class */
	BLUETOOTH_DEVICE_MINOR_CLASS_ANKLE_PROSTHESIS = 0x34,	/**< Ankle prosthesis minor class */
} bluetooth_device_minor_class_t;

/**
* Service class part of class of device returned from device discovery
*/
typedef enum {
	BLUETOOTH_DEVICE_SERVICE_CLASS_LIMITED_DISCOVERABLE_MODE = 0x002000,
	BLUETOOTH_DEVICE_SERVICE_CLASS_POSITIONING = 0x010000,			/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_NETWORKING = 0x020000,			/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_RENDERING = 0x040000,			/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_CAPTURING = 0x080000,			/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_OBJECT_TRANSFER = 0x100000,		/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_AUDIO = 0x200000,			/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_TELEPHONY = 0x400000,			/**<  */
	BLUETOOTH_DEVICE_SERVICE_CLASS_INFORMATION = 0x800000,			/**<  */
} bluetooth_device_service_class_t;

/**
 * structure to hold the device information
 */
typedef struct {
	bluetooth_device_major_class_t major_class; /**< major device class */
	bluetooth_device_minor_class_t minor_class; /**< minor device class */
	bluetooth_device_service_class_t service_class;
						    /**< service device class */
} bluetooth_device_class_t;

/**
 * This is Bluetooth device address type, fixed to 6 bytes ##:##:##:##:##:##
 */
typedef struct {
	unsigned char addr[BLUETOOTH_ADDRESS_LENGTH];
} bluetooth_device_address_t;

/**
 * This is Bluetooth device name type, maximum size of Bluetooth device name is 248 bytes
 */
typedef struct {
	char name[BLUETOOTH_DEVICE_NAME_LENGTH_MAX + 1];
} bluetooth_device_name_t;

/**
 * This is Bluetooth manufacturer specific data, maximum size of data is 240 bytes
 */
typedef struct {
	int data_len;		/**< manafacturer specific data length */
	char data[BLUETOOTH_MANUFACTURER_DATA_LENGTH_MAX];
} bluetooth_manufacturer_data_t;

/**
* structure to hold the device information
*/
typedef struct {
	bluetooth_device_address_t device_address;	/**< device address */
	bluetooth_device_name_t device_name;	/**< device name */
	bluetooth_device_class_t device_class;	/**< device class */
	char uuids[BLUETOOTH_MAX_SERVICES_FOR_DEVICE][BLUETOOTH_UUID_STRING_MAX];
	unsigned int service_list_array[BLUETOOTH_MAX_SERVICES_FOR_DEVICE]; /**< Use enum values in bt_service_uuid_list_t */
	int service_index;
	int rssi;				/**< received strength signal*/
	bool paired;			/**< paired flag */
	bluetooth_connected_link_t connected;	/**< connected link type */
	bool trust;				/**< trust flag */
	bool is_alias_set;		/** is device alias set flag**/
	bluetooth_manufacturer_data_t manufacturer_data;	/**< manafacturer specific class */
} bluetooth_device_info_t;

/**
 * Advertising data
 */
typedef struct {
	uint8_t data[BLUETOOTH_ADVERTISING_DATA_LENGTH_MAX];
} bluetooth_advertising_data_t;

/**
* structure to hold the LE device information
*/
typedef struct {
	int data_len;		/**< manafacturer specific data length */
	bluetooth_advertising_data_t data;		/**< manafacturer specific data */
} bluetooth_le_advertising_data_t;

typedef struct {
	bluetooth_device_address_t device_address;	/**< device address */
	int addr_type;			/**< address type*/
	int rssi;			/**< received strength signal*/
	bluetooth_le_advertising_data_t adv_ind_data;
	bluetooth_le_advertising_data_t scan_resp_data;
} bluetooth_le_device_info_t;

int bt_adapt_bond_device(const char *device_address);

int bt_adapt_unbond_device(const char *device_address);

int bt_adapt_get_bonded_device(const char *device_address,
					      bt_device_info_s *device_info);

int bt_adapt_is_device_connected(const char *device_address,
				bt_profile_e bt_profile,
				bool *is_connected);

int bt_adapt_connect_le(const char *device_address);

int bt_adapt_disconnect_le(const char *device_address);

int bt_adapt_le_conn_update(const char *device_address,
				const bt_le_conn_update_s *parameters);

int bt_adapt_update_le_connection_mode(const char *device_address,
				bt_device_le_connection_mode_e mode);

int bt_adapt_request_att_mtu(const char *device_address, unsigned int mtu);

int bt_adapt_get_att_mtu(const char *device_address, unsigned int *mtu);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BT_ADAPTATION_DEVICE_H__ */
