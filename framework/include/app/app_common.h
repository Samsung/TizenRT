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


#ifndef __TIZEN_APPFW_APP_COMMON_H__
#define __TIZEN_APPFW_APP_COMMON_H__

#include <app/app_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file app_common.h
 */

/**
 * @addtogroup CAPI_APP_COMMON_MODULE
 * @{
 */


/**
 * @brief Enumeration for system events.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_EVENT_LOW_MEMORY, /**< The low memory event */
	APP_EVENT_LOW_BATTERY, /**< The low battery event */
	APP_EVENT_LANGUAGE_CHANGED, /**< The system language changed event */
	APP_EVENT_DEVICE_ORIENTATION_CHANGED, /**< The device orientation changed event */
	APP_EVENT_REGION_FORMAT_CHANGED, /**< The region format changed event */
	APP_EVENT_SUSPENDED_STATE_CHANGED, /**< The suspended state changed event of the application (since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif)
					     @see app_event_get_suspended_state */
	APP_EVENT_UPDATE_REQUESTED, /**< The update requested event (Since 3.0)
					This event can occur when an app needs to be updated.
					It is dependent on target devices. */
} app_event_type_e;


/**
 * @brief Enumeration for device orientation.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_DEVICE_ORIENTATION_0 = 0, /**< The device is oriented in a natural position */
	APP_DEVICE_ORIENTATION_90 = 90, /**< The device's left side is at the top */
	APP_DEVICE_ORIENTATION_180 = 180, /**< The device is upside down */
	APP_DEVICE_ORIENTATION_270 = 270, /**< The device's right side is at the top */
} app_device_orientation_e;


/**
 * @brief Enumeration for low memory status.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_EVENT_LOW_MEMORY_NORMAL = 0x01, /**< Normal status */
	APP_EVENT_LOW_MEMORY_SOFT_WARNING = 0x02, /**< Soft warning status */
	APP_EVENT_LOW_MEMORY_HARD_WARNING = 0x04, /**< Hard warning status */
} app_event_low_memory_status_e;


/**
 * @brief Enumeration for battery status.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_EVENT_LOW_BATTERY_POWER_OFF = 1, /**< The battery status is under 1% */
	APP_EVENT_LOW_BATTERY_CRITICAL_LOW, /**< The battery status is under 5% */
} app_event_low_battery_status_e;


/**
 * @brief Enumeration for suspended state.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
typedef enum {
	APP_SUSPENDED_STATE_WILL_ENTER = 0, /**< Application will enter the suspended state */
	APP_SUSPENDED_STATE_DID_EXIT, /**< Application did exit from the suspended state */
} app_suspended_state_e;


/**
 * @brief The event handler that returned from add event handler function to handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see app_event_type_e
 * @see app_add_event_handler
 * @see app_remove_event_handler
 * @see app_event_info_h
 */
typedef struct app_event_handler *app_event_handler_h;


/**
 * @brief The system event information handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see app_event_get_low_memory_status
 * @see app_event_get_low_battery_status
 * @see app_event_get_language
 * @see app_event_get_region_format
 * @see app_event_get_device_orientation
 * @see app_event_get_suspended_state
 */
typedef struct app_event_info *app_event_info_h;


/**
 * @brief The system event callback function.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] event_info The system event information
 * @param[in] user_data The user data passed from the add event handler function
 * @see app_add_event_handler
 * @see app_event_info_h
 * @remarks If the given @a event_info has #APP_SUSPENDED_STATE_WILL_ENTER value,
 *          the application should not call any asynchronous operations in this callback.
 *          After the callback returns, process of the application will be changed to suspended
 *          state immediately. Thus, asynchronous operations may work incorrectly. (since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif)
 */
typedef void (*app_event_cb)(app_event_info_h event_info, void *user_data);


/**
 * @brief Gets the low memory status from the given event info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] event_info The system event info
 * @param[out] status The low memory status
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid event context
 * @see app_event_info_h
 * @see app_event_low_memory_status_e
 */
int app_event_get_low_memory_status(app_event_info_h event_info, app_event_low_memory_status_e *status);


/**
 * @brief Gets the low battery status from given event info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] event_info The system event info
 * @param[out] status The low battery status
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid event context
 * @see app_event_info_h
 * @see app_event_low_battery_status_e
 */
int app_event_get_low_battery_status(app_event_info_h event_info, app_event_low_battery_status_e *status);


/**
 * @brief Gets the language from the given event info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks @a lang must be released using free().
 * @param[in] event_info The system event info
 * @param[out] lang The language changed
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid event context
 * @see app_event_info_h
 */
int app_event_get_language(app_event_info_h event_info, char **lang);


/**
 * @brief Gets the region format from the given event info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks @a region must be released using free().
 * @param[in] event_info The system event info
 * @param[out] region The region format changed
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid event context
 * @see app_event_info_h
 */
int app_event_get_region_format(app_event_info_h event_info, char **region);


/**
 * @brief Gets the device orientation from the given event info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] event_info The system event info
 * @param[out] orientation The device orientation changed
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid event context
 * @see app_event_info_h
 * @see app_device_orientation_e
 */
int app_event_get_device_orientation(app_event_info_h event_info, app_device_orientation_e *orientation);


/**
 * @brief Gets the suspended state of the application from the given event info.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The application should not use any asynchronous operations in #APP_SUSPENDED_STATE_WILL_ENTER event.
 *          Because applications will be changed to suspended state just after #APP_SUSPENDED_STATE_WILL_ENTER, asynchronous calls are not guaranteed to work properly.
 * @param[in] event_info The handle for getting the suspended state
 * @param[out] state The suspended state of the application
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid event context
 */
int app_event_get_suspended_state(app_event_info_h event_info, app_suspended_state_e *state);


/**
 * @brief Gets the ID of the application.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks @a id must be released using free().
 * @param[out] id The ID of the application
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT The application is launched illegally, not launched by the launch system
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_get_id(char **id);


/**
 * @brief Gets the localized name of the application.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks @a name must be released using free().
 * @param[out] name The name of the application
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT The application is launched illegally, not launched by the launch system
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_get_name(char **name);


/**
 * @brief Gets the version of the application package.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks @a version must be released using free().
 * @param[out] version The version of the application
 * @return @c 0 on success,
 *			otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT The application is launched illegally, not launched by the launch system
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_get_version(char **version);


/**
 * @brief Gets the absolute path to the application's data directory which is used to store private data of the application.
 * @details	An application can read and write its own files in the application's data directory.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The returned path should be released.
 * @return The absolute path to the application's data directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_data_path(void);


/**
 * @brief Gets the absolute path to the application's cache directory which is used to store temporary data of the application.
 * @details	An application can read and write its own files in the application's cache directory.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The returned path should be released. @n
 *			The files stored in the application's cache directory can be removed by setting application or platform while the application is running.
 * @return The absolute path to the application's cache directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_cache_path(void);


/**
 * @brief Gets the absolute path to the application resource directory. The resource files are delivered with the application package.
 * @details	An application can only read its own files in the application's resource directory.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The returned path should be released.
 * @return The absolute path to the application's resource directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_resource_path(void);


/**
 * @brief Gets the absolute path to the application's shared data directory which is used to share data with other applications.
 * @details	An application can read and write its own files in the application's shared data directory and others can only read the files.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel	public
 * @privilege	%http://tizen.org/privilege/appdir.shareddata
 * @remarks	The returned path should be released. @n
 *		Since Tizen 3.0, an application that want to use shared/data directory must declare http://tizen.org/privilege/appdir.shareddata privilege. If the application doesn't declare the privilege, the framework will not create shared/data directory for the application.
 *		Carefully consider the privacy implications when deciding whether to use the shared/data directory, since the application cannot control access to this directory by other applications.
 *		If you want to share files with other applications, consider passing path via @ref CAPI_APP_CONTROL_MODULE API.
 *		The @ref CAPI_APP_CONTROL_MODULE API supports giving permission to other applications by passing path via app_control. @n
 *		The specific error code can be obtained using the get_last_result(). Error codes are described in Exception section.
 *
 * @return	The absolute path to the application's shared data directory, @n
 *		otherwise a null pointer if the memory is insufficient. It will return NULL for applications with api-version 3.0 or later, and set #APP_ERROR_PERMISSION_DENIED if the application does not declare the shareddata privilege.
 * @exception APP_ERROR_NONE Success
 * @exception APP_ERROR_OUT_OF_MEMORY Out of memory
 * @exception APP_ERROR_PERMISSION_DENIED Permission denied
 */
char *app_get_shared_data_path(void);


/**
 * @brief Gets the absolute path to the application's shared resource directory which is used to share resources with other applications.
 * @details	An application can read its own files in the application's shared resource directory, and others can only read the files.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The returned path should be released.
 * @return The absolute path to the application's shared resource directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_shared_resource_path(void);


/**
 * @brief Gets the absolute path to the application's shared trusted directory which is used to share data with a family of trusted applications.
 * @details	An application can read and write its own files in the application's shared trusted directory
 *          and the family applications signed with the same certificate can read and write the files in the shared trusted directory.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The returned path should be released.
 * @return The absolute path to the application's shared trusted directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_shared_trusted_path(void);


/**
 * @brief Gets the absolute path to the application's external data directory which is used to store data of the application.
 * @details	An application can read and write its own files in the application's external data directory.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The returned path should be released. @n
 *          The important files stored in the application's external data directory should be encrypted because they can be exported via the external sdcard.
 *          To access the path returned by this function requires the privilege
 *		    that is "http://tizen.org/privilege/externalstorage.appdata".
 * @return The absolute path to the application's external data directory, @n
 *			otherwise a null pointer if the memory is insufficient
 */
char *app_get_external_data_path(void);


/**
 * @brief Gets the absolute path to the application's external cache directory which is used to store temporary data of the application.
 * @details	An application can read and write its own files in the application's external cache directory.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The returned path should be released. @n
 *          The files stored in the application's external cache directory can be removed by
 *          setting application while the application is running. @n
 *          The important files stored in the application's external cache directory should be
 *          encrypted because they can be exported via the external sdcard.
 *          To access the path returned by this function requires the privilege
 *		    that is "http://tizen.org/privilege/externalstorage.appdata".
 * @return The absolute path to the application's external cache directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_external_cache_path(void);


/**
 * @deprecated Deprecated since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif.
 * @brief Gets the absolute path to the application's external shared data directory which is used to share data with other applications.
 * @details	An application can read and write its own files in the application's external shared data directory, and others can only read the files.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	The specified @a path should be released.
 *          To access the path returned by this function requires the privilege
 *		    that is "http://tizen.org/privilege/externalstorage.appdata".
 *          The function may not work as intended in certain devices due to some implementation issues.
 * @return The absolute path to the application's external shared data directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_external_shared_data_path(void) TIZEN_DEPRECATED_API;


/**
 * @brief Gets the absolute path to the application's TEP(Tizen Expansion Package) directory. The resource files are delivered with the expansion package.
 * @details An application can only read its own files in the application's TEP(Tizen Expansion Package) directory.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The returned path should be released.
 * @return The absolute path to the application's TEP(Tizen Expansion Package) directory, @n
 *         otherwise a null pointer if the memory is insufficient
 **/
char *app_get_tep_resource_path(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_H__ */

