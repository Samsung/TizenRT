/*
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_APPFW_APP_CONTROL_H__
#define __TIZEN_APPFW_APP_CONTROL_H__

#include <app/tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_control.h
 */

/**
 * @addtogroup CAPI_APP_CONTROL_MODULE
 * @{
 */


typedef struct _bundle_t bundle;
typedef unsigned char bundle_raw;

/**
 * @brief The application control handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct app_control_s *app_control_h;


/**
 * @brief Enumeration for the application control error.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_CONTROL_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_CONTROL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	APP_CONTROL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_CONTROL_ERROR_APP_NOT_FOUND = TIZEN_ERROR_APPLICATION | 0x21, /**< The application is not found */
	APP_CONTROL_ERROR_KEY_NOT_FOUND = TIZEN_ERROR_KEY_NOT_AVAILABLE, /**< Specified key is not found */
	APP_CONTROL_ERROR_KEY_REJECTED = TIZEN_ERROR_KEY_REJECTED, /**< Key is not available */
	APP_CONTROL_ERROR_INVALID_DATA_TYPE = TIZEN_ERROR_APPLICATION | 0x22, /**< Invalid data type */
	APP_CONTROL_ERROR_LAUNCH_REJECTED = TIZEN_ERROR_APPLICATION | 0x23, /**< The application cannot be launched now*/
	APP_CONTROL_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission denied */
	APP_CONTROL_ERROR_LAUNCH_FAILED = TIZEN_ERROR_APPLICATION | 0x24, /**< Internal launch error */
	APP_CONTROL_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Time out */
	APP_CONTROL_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR	/**< IO error */
} app_control_error_e;


/**
 * @brief Enumeration for the application control result.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see app_control_enable_app_started_result_event()
 */
typedef enum {
	APP_CONTROL_RESULT_APP_STARTED = 1, /**< Callee application launched actually (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	APP_CONTROL_RESULT_SUCCEEDED = 0, /**< Operation succeeded */
	APP_CONTROL_RESULT_FAILED = -1, /**< Operation failed by the callee */
	APP_CONTROL_RESULT_CANCELED = -2, /**< Operation canceled by the platform */
} app_control_result_e;


/**
 * @brief Enumeration for the application control launch mode.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
typedef enum {
	APP_CONTROL_LAUNCH_MODE_SINGLE = 0, /**< Prefer to launch an application as single mode */
	APP_CONTROL_LAUNCH_MODE_GROUP, /**< Prefer to launch an application as group mode */
} app_control_launch_mode_e;


/**
 * @brief Definition for the app_control operation: An explicit launch for the homescreen application.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_MAIN "http://tizen.org/appcontrol/operation/main"


/**
 * @brief Definition for the app_control operation: An explicit launch for an application that excludes the homescreen application.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_DEFAULT "http://tizen.org/appcontrol/operation/default"


/**
 * @brief Definition for the app_control operation: Provides an editable access to the given data.
 * @details Input: It depends on the application scenario or configuration.\n
 *	    	Output: It depends on the application scenario or configuration.\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_EDIT "http://tizen.org/appcontrol/operation/edit"


/**
 * @brief Definition for the app_control operation: Displays the data.
 * @details Input: URI is usually used to inform the path to show. In some cases, some other data in extra can be used (for example, the VIEW operation for viewing the contact and calendar data).\n
 *	    	Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_VIEW "http://tizen.org/appcontrol/operation/view"


/**
 * @brief Definition for the app_control operation: Picks the item.
 * @details Input: It depends on the application scenario or configuration.\n
 *	   	 	Output: APP_CONTROL_DATA_SELECTED in extra.\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_PICK "http://tizen.org/appcontrol/operation/pick"


/**
 * @brief Definition for the app_control operation: Creates the contents.
 * @details Input: It depends on application scenario or configuration.\n
 *	    	Output: APP_CONTROL_DATA_SELECTED in extra.\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_CREATE_CONTENT "http://tizen.org/appcontrol/operation/create_content"


/**
 * @brief Definition for the app_control operation: Performs a call to someone.
 * @details Input: URI is the number to be dialed. This is a mandatory field.\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/call
 * @remarks When you request this operation, you must declare this privilege.
 */
#define APP_CONTROL_OPERATION_CALL "http://tizen.org/appcontrol/operation/call"


/**
 * @deprecated Deprecated since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif. Use #APP_CONTROL_OPERATION_SHARE instead.
 * @brief Definition for the app_control operation: Delivers some data to someone else.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_SEND "http://tizen.org/appcontrol/operation/send"


/**
 * @deprecated Deprecated since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif. Use #APP_CONTROL_OPERATION_SHARE_TEXT instead.
 * @brief Definition for the app_control operation: Delivers text data to someone else.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_SEND_TEXT "http://tizen.org/appcontrol/operation/send_text"


/**
 * @brief Definition for the app_control operation: Shares an item with someone else.
 * @details Input: URI is usually used to inform the path to share. In some cases, some other data in extra can be used (for example, the SHARE operation for sharing the contact data, the SHARE operation for sharing an item through a message and email applications).\n
 *	        Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_SHARE "http://tizen.org/appcontrol/operation/share"


/**
 * @brief Definition for the app_control operation: Shares multiple items with someone else.
 * @details Input: APP_CONTROL_DATA_PATH in extra is usually used to inform the data to share. In some cases, some other data in extra can be used (for example, the MULTI_SHARE operation for sharing the contact data).\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_MULTI_SHARE "http://tizen.org/appcontrol/operation/multi_share"


/**
 * @brief Definition for the app_control operation: Shares the text data with someone else.
 * @details Input: APP_CONTROL_DATA_TEXT in extra is the text to share\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_SHARE_TEXT "http://tizen.org/appcontrol/operation/share_text"


/**
 * @brief Definition for the app_control operation: Dials a number. This shows a UI with the number to be dialed, allowing the user to explicitly initiate the call.
 * @details Input: URI is the number to be dialed. If empty, show a UI without the number.\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_DIAL "http://tizen.org/appcontrol/operation/dial"


/**
 * @brief Definition for the app_control operation: Performs a search.
 * @details Input: APP_CONTROL_DATA_TEXT in extra is the text to search for. If empty, show a search UI.\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_SEARCH "http://tizen.org/appcontrol/operation/search"


/**
 * @brief Definition for the app_control operation: Downloads the item.
 * @details Input: It depends on the application scenario or configuration.\n
 *	    	Output: It depends on the application scenario or configuration.\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif)
 * @remarks When you request this operation, you must declare this privilege. (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif)
 */
#define APP_CONTROL_OPERATION_DOWNLOAD "http://tizen.org/appcontrol/operation/download"


/**
 * @brief Definition for the app_control operation: Prints the content.
 * @details Input: It depends on the application scenario or configuration.\n
 *	    	Output: It depends on the application scenario or configuration.\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_PRINT "http://tizen.org/appcontrol/operation/print"


/**
 * @brief Definition for the app_control operation: Composes a message.
 * @details Input: It depends on the application scenario or configuration.\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_COMPOSE "http://tizen.org/appcontrol/operation/compose"


/**
 * @brief Definition for the app_control operation: Can be launched by an interested system-event.
 * @details Input : URI and extras data defined in event module.\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks This operation is for handling an event from the platform. This operation cannot be requested through the app_control_send_launch_request().
 * @remarks Refer to the Launch-On-Event section of the event module.
 */
#define APP_CONTROL_OPERATION_LAUNCH_ON_EVENT "http://tizen.org/appcontrol/operation/launch_on_event"


/**
 * @brief Definition for the app_control operation: Adds an item.
 * @details Input: It depends on the application scenario or configuration.\n
 *          Output: It depends on the application scenario or configuration.\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_ADD "http://tizen.org/appcontrol/operation/add"


/**
 * @brief Definition for the app_control operation: Captures images by camera applications.
 * @details Input: Nothing\n
 *	        Output: APP_CONTROL_DATA_SELECTED in extra.\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_IMAGE_CAPTURE "http://tizen.org/appcontrol/operation/image_capture"


/**
 * @brief Definition for the app_control operation: Captures the videos by camera applications.
 * @details Input: Nothing\n
 *	    	Output: APP_CONTROL_DATA_SELECTED in extra.\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_VIDEO_CAPTURE "http://tizen.org/appcontrol/operation/video_capture"


/**
 * @brief Definition for the app_control operation: Shows the settings to enable Bluetooth.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_SETTING_BT_ENABLE "http://tizen.org/appcontrol/operation/setting/bt_enable"


/**
 * @brief Definition for the app_control operation: Shows the settings to configure Bluetooth visibility.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_SETTING_BT_VISIBILITY "http://tizen.org/appcontrol/operation/setting/bt_visibility"


/**
 * @brief Definition for the app_control operation: Shows the settings to allow configuration of current location sources.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_SETTING_LOCATION "http://tizen.org/appcontrol/operation/setting/location"


/**
 * @brief Definition for the app_control operation: Shows the NFC settings.
 * @details Input: APP_CONTROL_DATA_TYPE in extra (Since 3.0).\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks Prior to Tizen 3.0, no input value was needed.
 */
#define APP_CONTROL_OPERATION_SETTING_NFC "http://tizen.org/appcontrol/operation/setting/nfc"


/**
 * @brief Definition for the app_control operation: Shows the system settings.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_SETTING "http://tizen.org/appcontrol/operation/setting"


/**
 * @brief Definition for the app_control operation: Shows the settings to allow configuration of Wi-Fi.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_SETTING_WIFI "http://tizen.org/appcontrol/operation/setting/wifi"


/**
 * @brief Definition for the application control operation: Shows the VPN service settings.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen 3.0
 */
#define APP_CONTROL_OPERATION_SETTING_VPN "http://tizen.org/appcontrol/operation/setting/vpn"


/**
 * @brief Definition for the application control operation: Select the type of input method to receive an input from the user.
 * @details Input: It depends on the application scenario or configuration.\n
 *          Output: APP_CONTROL_DATA_TEXT in extra.\n
 * @since_tizen @if WEARABLE 2.3.2 @elseif MOBILE 3.0 @endif
 */
#define APP_CONTROL_OPERATION_GET_INPUT "http://tizen.org/appcontrol/operation/get_input"


/**
 * @brief Definition for the application control operation: launch controller server or client.
 * @details Input: APP_CONTROL_DATA_TYPE in extra is the text of launch type("server" or "client"). This is a mandatory field.\n
 *                  In some cases, URI is used to inform the path to play for the media controller server.\n
 *          Output: Nothing\n
 * @since_tizen 4.0
 */
#define APP_CONTROL_OPERATION_MEDIA_CONTROLLER "http://tizen.org/appcontrol/operation/media_control"

/**
 * @brief Definition for the application control operation: Shows guide popup to privacy settings.
 * @details Input: APP_CONTROL_DATA_TYPE in extra is the text of guide type("open" or "use"). This is a mandatory field.\n
 *          Input: APP_CONTROL_DATA_FEATURE in extra is the text of the feature or function name which requires privacy privileges. \n
 *          Input: APP_CONTROL_DATA_PRIVILEGES in extra is the text array of the required privileges. This is a mandatory field. \n
 *          Output: APP_CONTROL_DATA_SELECTED in extra. The value is "yes" or "no" according to the user response. \n
 * @since_tizen @if WEARABLE 5.0 @endif
 */
#define APP_CONTROL_OPERATION_PRIVACY_SETTING_GUIDE "http://tizen.org/appcontrol/operation/guide_privacy_setting"

/**
 * @brief Definition for the app_control data: Feature of function name.
 * @since_tizen @if WEARABLE 5.0 @endif
 */
#define APP_CONTROL_DATA_FEATURE "http://tizen.org/appcontrol/data/feature"

/**
 * @brief Definition for the app_control data: List of privileges.
 * @since_tizen @if WEARABLE 5.0 @endif
 */
#define APP_CONTROL_DATA_PRIVILEGES "http://tizen.org/appcontrol/data/privileges"

/**
 * @brief Definition for the app_control data: Subject.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_SUBJECT "http://tizen.org/appcontrol/data/subject"


/**
 * @brief Definition for the app_control data: Recipients.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_TO "http://tizen.org/appcontrol/data/to"


/**
 * @brief Definition for the app_control data: Email addresses that should be carbon copied.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_CC "http://tizen.org/appcontrol/data/cc"


/**
 * @brief Definition for the app_control data: Email addresses that should be blind carbon copied.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_BCC "http://tizen.org/appcontrol/data/bcc"


/**
 * @brief Definition for the app_control data: Text.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_TEXT "http://tizen.org/appcontrol/data/text"


/**
 * @brief Definition for the app_control data: Title.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_TITLE "http://tizen.org/appcontrol/data/title"


/**
 * @brief Definition for the app_control data: Selected items.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks Since Tizen 3.0, if all added paths with this key are under the caller application's data path which can be obtained by calling app_get_data_path() function, those will be shared to the callee application. Framework will grant a temporary permission to the callee application for those files and revoke it when the callee application is terminated. Paths should be regular files. The callee application can just read them. Note that the callee application doesn't have read permission of the directory that is obtained by caller's app_get_data_path(), you should open the file path with read only mode directly. For example, access() call to the file path will return error because access() needs the read permission of the directory. The callee application can call open() with O_RDONLY mode for the passed file path, because framework grants read permission to the passed file path.
 */
#define APP_CONTROL_DATA_SELECTED "http://tizen.org/appcontrol/data/selected"


/**
 * @brief Definition for the app_control data: Paths of items.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks Since Tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif, if all added paths with this key are under the caller application's data path which can be obtained by the calling app_get_data_path() function, those will be shared to the callee application. Framework will grant a temporary permission to the callee application for those files and revoke it when the callee application is terminated. Paths should be regular files. The callee application can just read them. Note that the callee application doesn't have read permission of the directory that is obtained by the caller's app_get_data_path(), you should open the file path with the read only mode directly. For example, access() call to the file path will return error because access() needs the read permission of the directory. The callee application can call open() with the O_RDONLY mode for the passed file path, because framework grants the read permission to the passed file path.
 */
#define APP_CONTROL_DATA_PATH "http://tizen.org/appcontrol/data/path"


/**
 * @brief Definition for the app_control data: Selection mode ("single" or "multiple").
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_DATA_SELECTION_MODE "http://tizen.org/appcontrol/data/selection_mode"


/**
 * @brief Definition for the app_control data: All-day mode of the event ("true" or "false").
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_CALENDAR_ALL_DAY "http://tizen.org/appcontrol/data/calendar/all_day"


/**
 * @brief Definition for the app_control data: Start time of the event (format: YYYY-MM-DD HH:MM:SS).
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_CALENDAR_START_TIME "http://tizen.org/appcontrol/data/calendar/start_time"


/**
 * @brief Definition for the app_control data: End time of event (format: YYYY-MM-DD HH:MM:SS).
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_CALENDAR_END_TIME "http://tizen.org/appcontrol/data/calendar/end_time"


/**
 * @brief Definition for the app_control data: Email addresses.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_EMAIL "http://tizen.org/appcontrol/data/email"


/**
 * @brief Definition for the app_control data: Phone numbers.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_PHONE "http://tizen.org/appcontrol/data/phone"


/**
 * @brief Definition for the app_control data: URLs.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_URL "http://tizen.org/appcontrol/data/url"


/**
 * @brief Definition for the app_control data: IDs.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_ID "http://tizen.org/appcontrol/data/id"


/**
 * @brief Definition for the app_control data: Type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define APP_CONTROL_DATA_TYPE "http://tizen.org/appcontrol/data/type"


/**
 * @brief Definition for the app_control data: Total count.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_TOTAL_COUNT "http://tizen.org/appcontrol/data/total_count"


/**
 * @brief Definition for the app_control data: Total size (unit : bytes).
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_TOTAL_SIZE "http://tizen.org/appcontrol/data/total_size"


/**
 * @brief Definition for the app_control data: Name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
#define APP_CONTROL_DATA_NAME "http://tizen.org/appcontrol/data/name"


/**
 * @brief Definition for the app_control data: Location.
 * @since_tizen 3.0
 */
#define APP_CONTROL_DATA_LOCATION "http://tizen.org/appcontrol/data/location"


/**
 * @brief Definition for the app_control data: Selects the type of the input method.
 * @details Type: "input_voice" Launch the input method with the voice type.
 *                "input_emoticon" Launch the input method with the emoticon type.
 *                "input_keyboard" Launch the input method with the keyboard type.
 *                "input_reply" Launch the input method with the context based reply type.
 *                "input_drawing" Launch the input method with the drawing type.
 *                "input_recording" Launch the input method with the voice recording type.
 * input_reply, input_drawing and input_recording types are valid since tizen 4.0.
 * @since_tizen @if WEARABLE 2.3.2 @elseif MOBILE 3.0 @endif
 */
#define APP_CONTROL_DATA_INPUT_TYPE "http://tizen.org/appcontrol/data/input_type"


/**
 * @brief Definition for the app_control data: Sends the pre-input text such as "http://" in web.
 * @since_tizen @if WEARABLE 2.3.2 @elseif MOBILE 3.0 @endif
 */
#define APP_CONTROL_DATA_INPUT_DEFAULT_TEXT "http://tizen.org/appcontrol/data/input_default_text"


/**
 * @brief Definition for the app_control data: Sends the guide text to show the user such as "Input user name".
 * @since_tizen @if WEARABLE 2.3.2 @elseif MOBILE 3.0 @endif
 */
#define APP_CONTROL_DATA_INPUT_GUIDE_TEXT "http://tizen.org/appcontrol/data/input_guide_text"


/**
 * @brief Definition for the app_control data: Sends text to receive the result from smart reply.
 * @since_tizen @if WEARABLE 2.3.2 @elseif MOBILE 3.0 @endif
 */
#define APP_CONTROL_DATA_INPUT_PREDICTION_HINT "http://tizen.org/appcontrol/data/input_prediction_hint"


/**
 * @brief Definition for the app_control data: Sends type to set the return key type in the keyboard input type.
 * @details Type: "Done" Set the type with Done label.
 *                "Send" Set the type with Send label.
 *                "Join" Set the type with Join label.
 *                "Login" Set the type with Login label.
 *                "Next" Set the type with Next label.
 *                "Sign-in" Set the type with Sign-in label.
 *                "Search" Set the type with Search label.
 *                "Go" Set the type with Go label.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_INPUT_RETURNKEY_TYPE "http://tizen.org/appcontrol/data/input_returnkey_type"


/**
 * @brief Definition for the app_control data: Sends the length value to set the max text length in the keyboard input type.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_INPUT_MAX_TEXT_LENGTH "http://tizen.org/appcontrol/data/input_max_text_length"


/* @brief Definition for app_control data: Send value to set cursor position to entry in keyboard input type.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_INPUT_CURSOR_POSITION_SET "http://tizen.org/appcontrol/data/input_cursor_position_set"


/**
 * @brief Definition for app_control data: Receive value to get cursor position from entry in keyboard input type.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_INPUT_CURSOR_POSITION_GET "http://tizen.org/appcontrol/data/input_cursor_position_get"


/**
 * @brief Definition for the app_control data: Receives the reply type from the input method.
 * @details Type: "input_voice" Receive the result from the voice input type.
 *                "input_emoticon" Receive the result from the emoticon input type.
 *                "input_keyboard" Receive the result from the keyboard input type.
 *                "input_reply" Receive the result from the reply input type.
 *                "input_image" Receive the result from the image input type.
 *                "input_audio" Receive the result from the audio input type.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_INPUT_REPLY_TYPE "http://tizen.org/appcontrol/data/input_reply_type"


/**
 * @brief Definition for the app_control data: Sends the widget instance ID to the widget setup application.
 * @details The value is the ID of the widget instance.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_WIDGET_INSTANCE_ID "http://tizen.org/appcontrol/data/widget_instance_id"


/**
 * @brief Definition for the app_control data: Sends the widget content to the widget setup application.
 * @details The value is the content information of the widget instance.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_WIDGET_CONTENT "http://tizen.org/appcontrol/data/widget_content"


/**
 * @brief Definition for the app_control data: Sends the widget application ID to the widget setup application.
 * @details The value is the application ID declared in the widget application manifest file.
 * @since_tizen 4.0
 */
#define APP_CONTROL_DATA_WIDGET_APP_ID "http://tizen.org/appcontrol/data/widget_app_id"


/**
 * @brief Called when the reply of the launch request is delivered.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a request and @a reply must not be deallocated by the application.
 * @param[in] request The app_control handle of the launch request that has been sent
 * @param[in] reply The app_control handle in which the results of the callee are contained
 * @param[in] result The result code of the launch request
 * @param[in] user_data The user data passed from the callback registration function
 * @pre When the callee replies to the launch request, this callback will be invoked.
 * @pre Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif, if #APP_CONTROL_RESULT_APP_STARTED event is enabled,
 *            this callback will also be invoked when the callee application actually launched.
 * @see app_control_send_launch_request()
 * @see app_control_reply_to_launch_request()
 * @see app_control_enable_app_started_result_event()
 * @see #APP_CONTROL_RESULT_APP_STARTED
 */
typedef void (*app_control_reply_cb) (app_control_h request, app_control_h reply, app_control_result_e result, void *user_data);


/**
 * @brief Called to retrieve the extra data contained in the app_control.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a key must not be deallocated by the application.
 * @param[in] app_control The app_control handle
 * @param[in] key The key of the value contained in the app_control
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre app_control_foreach_extra_data() will invoke this callback.
 * @see app_control_foreach_extra_data()
 */
typedef bool (*app_control_extra_data_cb)(app_control_h app_control, const char *key, void *user_data);


/**
 * @brief Called once for each matched application that can be launched to handle the given app_control request.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] package The package name of the application that can handle the launch request of the given app_control
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre app_control_foreach_app_matched() will invoke this callback.
 * @see app_control_foreach_app_matched()
 */
typedef bool (*app_control_app_matched_cb)(app_control_h app_control, const char *appid, void *user_data);

/**
 * @brief Called when the result of the launch request is delivered.
 * @details Following error codes can be delivered: \n
 *          #APP_CONTROL_ERROR_NONE, \n
 *          #APP_CONTROL_ERROR_APP_NOT_FOUND, \n
 *          #APP_CONTROL_ERROR_PERMISSION_DENIED, \n
 *          #APP_CONTROL_ERROR_INVALID_PARAMETER, \n
 *          #APP_CONTROL_ERROR_LAUNCH_REJECTED
 *
 * @since_tizen 5.0
 * @remarks The @a request is the same object for which app_control_send_launch_request_async() was called.
 * @param[in]   request         The app_control handle of the launch request that has been sent
 * @param[in]   result          The result value of the launch request
 * @param[in]   user_data       The user data passed from the callback registration function
 * @see app_control_send_launch_request_async()
 */
typedef void (*app_control_result_cb)(app_control_h request, app_control_error_e result, void *user_data);


typedef int (*app_control_host_res_fn)(void *data);

/**
 * @brief Creates an app_control handle.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a app_control must be released using app_control_destroy().
 * @param[out] app_control The app_control handle to be newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_destroy()
 */
int app_control_create(app_control_h *app_control);


/**
 * @brief Destroys the app_control handle and releases all its resources.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_create()
 */
int app_control_destroy(app_control_h app_control);


/**
 * @brief Sets the operation to be performed.
 *
 * @details The @a operation is the mandatory information for the launch request.
 *          If the operation is not specified, #APP_CONTROL_OPERATION_DEFAULT is used for the launch request.
 *          If the operation is #APP_CONTROL_OPERATION_DEFAULT, the package information is mandatory to explicitly launch the application.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] operation The operation to be performed (if the @a operation is @c NULL, it clears the previous value)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_get_operation()
 * @see APP_CONTROL_OPERATION_DEFAULT
 * @see APP_CONTROL_OPERATION_EDIT
 * @see APP_CONTROL_OPERATION_VIEW
 * @see APP_CONTROL_OPERATION_PICK
 * @see APP_CONTROL_OPERATION_CREATE_CONTENT
 * @see APP_CONTROL_OPERATION_CALL
 * @see APP_CONTROL_OPERATION_SEND
 * @see APP_CONTROL_OPERATION_SEND_TEXT
 * @see APP_CONTROL_OPERATION_DIAL
 * @see APP_CONTROL_OPERATION_SEARCH
 */
int app_control_set_operation(app_control_h app_control, const char *operation);


/**
 * @brief Gets the operation to be performed.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a operation must be released using free().
 * @param[in] app_control The app_control handle
 * @param[out] operation The operation to be performed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_operation()
 */
int app_control_get_operation(app_control_h app_control, char **operation);


/**
 * @brief Sets the URI of the data.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks Since Tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif, if the parameter @a uri is started with 'file://' and it is a regular file in this application's data path which can be obtained by calling the app_get_data_path() function, it will be shared to the callee application. Framework will grant a temporary permission to the callee application for this file and revoke it when the callee application is terminated. The callee application can just read it.
 * @param[in] app_control The app_control handle
 * @param[in] uri The URI of the data this app_control is operating on (if the @a uri is @c NULL, it clears the previous value)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_get_uri()
 */
int app_control_set_uri(app_control_h app_control, const char *uri);


/**
 * @brief Gets the URI of the data.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a uri must be released using free().
 * @param[in] app_control The app_control handle
 * @param[out] uri The URI of the data this app_control is operating on
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_uri()
 */
int app_control_get_uri(app_control_h app_control, char **uri);


/**
 * @brief Sets the explicit MIME type of the data.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] mime The explicit MIME type of the data this app_control is operating on (if the @a mime is @c NULL, it clears the previous value)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_get_mime()
 */
int app_control_set_mime(app_control_h app_control, const char *mime);


/**
 * @brief Gets the explicit MIME type of the data.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a uri must be released using free().
 * @param[in] app_control The app_control handle
 * @param[out] mime The explicit MIME type of the data this app_control is operating on
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_mime()
 */
int app_control_get_mime(app_control_h app_control, char **mime);


/**
 * @brief Sets the explicit category.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] category The explicit category (if the @a category is @c NULL, it clears the previous value)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_get_category()
 */
int app_control_set_category(app_control_h app_control, const char *category);


/**
 * @brief Gets the explicit category.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a category must be released using free().
 * @param[in] app_control The app_control handle
 * @param[out] category The explicit category
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_category()
 */
int app_control_get_category(app_control_h app_control, char **category);


/**
 * @brief Sets the ID of the application to explicitly launch.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] app_id The ID of the application to explicitly launch (if the @a app_id is @c NULL, it clears the previous value)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_get_app_id()
 */
int app_control_set_app_id(app_control_h app_control, const char *app_id);


/**
 * @brief Gets the ID of the application to explicitly launch.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a app_id must be released with free().
 * @param[in] app_control The app_control handle
 * @param[out] app_id The ID of the application to explicitly launch
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_app_id()
 */
int app_control_get_app_id(app_control_h app_control, char **app_id);

/**
 * @brief Adds extra data to the app_control.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The function replaces any existing value for the given key.
 * @remarks The function returns #APP_CONTROL_ERROR_INVALID_PARAMETER if @a key or @a value is a zero-length string.
 * @remarks The function returns #APP_CONTROL_ERROR_KEY_REJECTED if the application tries to use the same key with the system-defined key.
 * @param[in] app_control The app_control handle
 * @param[in] key The name of the extra data
 * @param[in] value The value associated with the given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_KEY_REJECTED Key not available
 * @see app_control_add_extra_data_array()
 * @see app_control_remove_extra_data()
 * @see app_control_get_extra_data()
 */
int app_control_add_extra_data(app_control_h app_control, const char *key, const char *value);


/**
 * @brief Adds the extra data array to the app_control.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The function replaces any existing value for the given key.
 * @remarks The function returns #APP_CONTROL_ERROR_INVALID_PARAMETER if @a key is a zero-length string.
 * @remarks The function returns #APP_CONTROL_ERROR_KEY_REJECTED if the application tries to use the same key with the system-defined key.
 * @param[in] app_control The app_control handle
 * @param[in] key The name of the extra data
 * @param[in] value The array value associated with the given key
 * @param[in] length The length of the array
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_KEY_REJECTED Key not available
 * @see app_control_add_extra_data()
 * @see app_control_remove_extra_data()
 * @see app_control_get_extra_data()
 */
int app_control_add_extra_data_array(app_control_h app_control, const char *key, const char* value[], int length);


/**
 * @brief Removes the extra data from the app_control.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] key The name of the extra data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_KEY_NOT_FOUND Specified key not found
 * @retval #APP_CONTROL_ERROR_KEY_REJECTED Key not available
 * @see app_control_add_extra_data()
 * @see app_control_add_extra_data_array()
 * @see app_control_get_extra_data()
 */
int app_control_remove_extra_data(app_control_h app_control, const char *key);


/**
 * @brief Gets the extra data from the app_control.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a value must be released using free().
 * @remarks The function returns #APP_CONTROL_ERROR_INVALID_DATA_TYPE if @a value is of array data type.
 * @param[in] app_control The app_control handle
 * @param[in] key The name of the extra data
 * @param[out] value The value associated with the given key
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_KEY_NOT_FOUND Specified key not found
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_CONTROL_ERROR_INVALID_DATA_TYPE Invalid data type
 * @retval #APP_CONTROL_ERROR_KEY_REJECTED Key not available
 * @see app_control_add_extra_data()
 * @see app_control_add_extra_data_array()
 * @see app_control_get_extra_data()
 * @see app_control_remove_extra_data()
 * @see app_control_foreach_extra_data()
 */
int app_control_get_extra_data(app_control_h app_control, const char *key, char **value);


/**
 * @brief Gets the extra data array from the app_control.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a value must be released using free().
 * @remarks The function returns #APP_CONTROL_ERROR_INVALID_DATA_TYPE if @a value is not of array data type.
 * @param[in] app_control The app_control handle
 * @param[in] key The name of the extra data
 * @param[out] value The array value associated with the given key
 * @param[out] length The length of the array
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_KEY_NOT_FOUND Specified key not found
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_CONTROL_ERROR_INVALID_DATA_TYPE Invalid data type
 * @retval #APP_CONTROL_ERROR_KEY_REJECTED Key not available
 * @see app_control_add_extra_data()
 * @see app_control_add_extra_data_array()
 * @see app_control_remove_extra_data()
 * @see app_control_foreach_extra_data()
 */
int app_control_get_extra_data_array(app_control_h app_control, const char *key, char ***value, int *length);


/**
 * @brief Checks whether the extra data associated with the given @a key is of array data type.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] key The name of the extra data
 * @param[out] array If @c true the extra data is of array data type,
 *                   otherwise @c false
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_add_extra_data()
 * @see app_control_add_extra_data_array()
 * @see app_control_remove_extra_data()
 * @see app_control_foreach_extra_data()
 */
int app_control_is_extra_data_array(app_control_h app_control, const char *key, bool *array);


/**
 * @brief Retrieves all extra data contained in app_control.
 * @details This function calls app_control_extra_data_cb() once for each key-value pair for extra data contained in app_control. \n
 *          If the app_control_extra_data_cb() callback function returns @c false, then iteration will be finished.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] callback The iteration callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @post This function invokes app_control_extra_data_cb().
 * @see app_control_extra_data_cb()
 */
int app_control_foreach_extra_data(app_control_h app_control, app_control_extra_data_cb callback, void *user_data);


/**
 * @brief Retrieves all applications that can be launched to handle the given app_control request.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] callback The iteration callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Success
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @post This function invokes app_control_app_matched_cb().
 * @see app_control_app_matched_cb()
 */
int app_control_foreach_app_matched(app_control_h app_control, app_control_app_matched_cb callback, void *user_data);


/**
 * @brief Sends the launch request.
 *
 * @details The operation is mandatory information for the launch request. \n
 *          If the operation is not specified, #APP_CONTROL_OPERATION_DEFAULT is used by default.
 *          If the operation is #APP_CONTROL_OPERATION_DEFAULT, the application ID is mandatory to explicitly launch the application.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @remarks The function returns #APP_CONTROL_ERROR_LAUNCH_REJECTED if the operation value is #APP_CONTROL_OPERATION_LAUNCH_ON_EVENT which is only for handling the event from the platform or other application, refer to the @a Event module.
 * @remarks Since Tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif, the launch request of the service application over out of packages is restricted by the platform. Also, implicit launch requests are NOT delivered to service applications since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif. To launch a service application, an explicit launch request with application ID given by the app_control_set_app_id() must be sent.
 * @param[in] app_control The app_control handle
 * @param[in] callback The callback function to be called when the reply is delivered
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_CONTROL_ERROR_APP_NOT_FOUND The application to run the given launch request is not found
 * @retval #APP_CONTROL_ERROR_LAUNCH_REJECTED The application cannot be launched in current context
 * @retval #APP_CONTROL_ERROR_LAUNCH_FAILED Failed to launch the application
 * @retval #APP_CONTROL_ERROR_TIMED_OUT Failed due to timeout. The application that handles @a app_control may be busy
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @post If the launch request is sent for the result, the result will come back through the app_control_reply_cb() from the callee application. Additional replies may be delivered on the app_control_enable_app_started_result_event() called.
 * @see app_control_reply_to_launch_request()
 * @see app_control_reply_cb()
 * @see app_control_enable_app_started_result_event()
 */
int app_control_send_launch_request(app_control_h app_control, app_control_reply_cb callback, void *user_data);


/**
 * @brief Sends the terminate request to the application that is launched by app_control. This API is only effective for some applications that are provided by default for handling platform default app_controls. You are not allowed to terminate other general applications using this API.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks Since Tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif, this API can be used to terminate sub-applications which were launched as group mode by the caller application.
 *          Once callee application is being terminated by this API, other applications which were launched by the callee application as a group mode will be terminated as well.
 * @param[in] app_control The app_control handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @see app_control_send_launch_request()
 * @see app_control_set_launch_mode()
 */
int app_control_send_terminate_request(app_control_h app_control);


/**
 * @brief Replies to the launch request sent by the caller.
 * @details If the caller application sent the launch request to receive the result, the callee application can return the result back to the caller.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The function is not allowed to send reply #APP_CONTROL_RESULT_APP_STARTED as @a result which is reserved for platform developers.
 * @param[in] reply The app_control handle in which the results of the callee are contained
 * @param[in] request The app_control handle sent by the caller
 * @param[in] result The result code of the launch request
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_send_launch_request()
 */
int app_control_reply_to_launch_request(app_control_h reply, app_control_h request, app_control_result_e result);


/**
 * @brief Creates and returns a copy of the given app_control handle.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks A newly created app_control should be destroyed by calling the app_control_destroy() if it is no longer needed.
 *
 * @param[out] clone If successful, a newly created app_control handle will be returned
 * @param[in] app_control The app_control handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_destroy()
 */
int app_control_clone(app_control_h *clone, app_control_h app_control);


/**
 * @brief Gets the application ID of the caller from the launch request.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a app_control must be the launch request from app_control_cb().
 * @remarks This function returns #APP_CONTROL_ERROR_INVALID_PARAMETER if the given app_control is not the launch request.
 * @remarks The @a id must be released using free().
 * @param[in] app_control The app_control handle from app_control_cb()
 * @param[out] id The application ID of the caller
 * @return @a 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_control_get_caller(app_control_h app_control, char **id);


/**
 * @brief Checks whether the caller is requesting a reply from the launch request.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a app_control must be the launch request from app_control_cb().
 * @remarks This function returns #APP_CONTROL_ERROR_INVALID_PARAMETER if the given app_control is not the launch request.
 * @param[in] app_control The app_control handle from app_control_cb()
 * @param[out] requested If @c true a reply is requested by the caller,
 *                       otherwise @c false
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_control_is_reply_requested(app_control_h app_control, bool *requested);

/**
 * @brief Sets the launch mode of the application.
 *
 * @details This function allows the callee application to be launched as a group or as a single mode.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks Although launch_mode were set as #APP_CONTROL_LAUNCH_MODE_GROUP, the callee application would be launched as a single mode if the manifest file of callee application defined the launch mode as "single".
 *          This function can just set the preference of the caller application to launch an application.
 * @remarks Sub-applications which were launched as a group mode always have own process.
 *
 * @param[in] app_control The app_control handle
 * @param[in] mode The launch mode of the application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_set_launch_mode()
 * @see app_control_launch_mode_e
 */
int app_control_set_launch_mode(app_control_h app_control,
		app_control_launch_mode_e mode);

/**
 * @brief Gets the launch mode of the application.
 *
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks Since Tizen 3.0, if launch mode is not set in the caller application control,
 *          the function returns the #APP_CONTROL_LAUNCH_MODE_SINGLE launch mode.
 * @param[in] app_control The app_control handle
 * @param[out] mode The launch mode of the application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_get_launch_mode()
 * @see app_control_launch_mode_e
 */
int app_control_get_launch_mode(app_control_h app_control,
		app_control_launch_mode_e *mode);

/**
 * @brief Enables an additional launch result event on the launch request.
 *
 * @details The function allows to receive the #APP_CONTROL_RESULT_APP_STARTED event on\n
 *         application get launched by the app_control.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks app_control_reply_cb() will be called on the APP_CONTROL_RESULT_APP_STARTED event received.
 * @param[in] app_control The app_control handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_send_launch_request()
 * @see #APP_CONTROL_RESULT_APP_STARTED
 */
int app_control_enable_app_started_result_event(app_control_h app_control);

/**
 * @brief Sends the launch request asynchronously.
 *
 * @details The operation is mandatory information for the launch request. \n
 *          If the operation is not specified, #APP_CONTROL_OPERATION_DEFAULT is used by default.
 *          If the operation is #APP_CONTROL_OPERATION_DEFAULT, the application ID is mandatory to explicitly launch the application.
 * @details After the callee application is initialized or the launch request is delivered to the running application successfully, the result callback function will be invoked.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @remarks The function returns #APP_CONTROL_ERROR_LAUNCH_REJECTED if the operation value is #APP_CONTROL_OPERATION_LAUNCH_ON_EVENT which is only for handling the event from the platform or other application, refer to the @a Event Module.
 * @remarks The launch request of the service application over out of packages is restricted by the platform. Also, implicit launch requests are NOT delivered to service applications since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif. To launch a service application, an explicit launch request with application ID given by the app_control_set_app_id() must be sent.
 * @param[in] app_control The app_control handle
 * @param[in] result_cb The callback function to be called when the result is delivered
 * @param[in] reply_cb The callback function to be called when the reply is delivered
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_CONTROL_ERROR_APP_NOT_FOUND The application to run the given launch request is not found
 * @retval #APP_CONTROL_ERROR_LAUNCH_REJECTED The application cannot be launched in current context
 * @retval #APP_CONTROL_ERROR_LAUNCH_FAILED Failed to launch the application
 * @retval #APP_CONTROL_ERROR_TIMED_OUT Failed due to timeout. The application that handles @a app_control may be busy
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @post If the launch request is sent for the result, the result will come back through the app_control_reply_cb() from the callee application. Additional replies may be delivered if app_control_enable_app_started_result_event() was called.
 * @see app_control_result_cb()
 * @see app_control_reply_to_launch_request()
 * @see app_control_reply_cb()
 * @see app_control_enable_app_started_result_event()
 */
int app_control_send_launch_request_async(app_control_h app_control,
		app_control_result_cb result_cb,
		app_control_reply_cb reply_cb,
		void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_CONTROL_H__ */

