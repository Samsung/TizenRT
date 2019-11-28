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

#ifndef __TIZEN_APPFW_SERVICE_APP_H__
#define __TIZEN_APPFW_SERVICE_APP_H__

#include <app/tizen.h>
#include <app/app_control.h>
#include <app/app_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SERVICE_APP_MODULE
 * @{
 */

/**
 * @brief Called at the start of the agent application.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] user_data	The user data passed from the callback registration function
 * @return @c true on success,
 *         otherwise @c false
 * @pre	service_app_main() will invoke this callback function.
 * @see service_app_main()
 * @see #service_app_lifecycle_callback_s
 */
typedef bool (*service_app_create_cb) (void *user_data);


/**
 * @brief Called once after the main loop of the agent application exits.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] user_data	The user data passed from the callback registration function
 * @see	service_app_main()
 * @see #service_app_lifecycle_callback_s
 */
typedef void (*service_app_terminate_cb) (void *user_data);


/**
 * @brief Called when another application sends the launch request to the agent application.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The handle to the app_control
 * @param[in] user_data The user data passed from the callback registration function
 * @see service_app_main()
 * @see #service_app_lifecycle_callback_s
 * @see @ref CAPI_APP_CONTROL_MODULE API
 */
typedef void (*service_app_control_cb) (app_control_h app_control, void *user_data);


/**
 * @brief The structure type containing the set of callback functions for handling application events.
 * @details It is one of the input parameters of the service_app_efl_main() function.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see service_app_main()
 * @see service_app_create_cb()
 * @see service_app_terminate_cb()
 * @see service_app_control_cb()
 */
typedef struct {
	service_app_create_cb create; /**< This callback function is called at the start of the application. */
	service_app_terminate_cb terminate; /**< This callback function is called once after the main loop of the application exits. */
	service_app_control_cb app_control; /**< This callback function is called when another application sends the launch request to the application. */
} service_app_lifecycle_callback_s;


/**
 * @brief Adds the system event handler
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The service application can handle low memory event, low battery event, language setting changed event and region format changed event.
 * @param[out] handler The event handler
 * @param[in] event_type The system event type
 * @param[in] callback The callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_event_type_e
 * @see app_event_cb
 * @see service_app_remove_event_handler
 */
int service_app_add_event_handler(app_event_handler_h *handler, app_event_type_e event_type, app_event_cb callback, void *user_data);


/**
 * @brief Removes registered event handler
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] event_handler The event handler
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @see service_app_add_event_handler
 */
int service_app_remove_event_handler(app_event_handler_h event_handler);


/**
 * @brief Runs the main loop of the application until service_app_exit() is called.
 * @details This function is the main entry point of the Tizen service application.
 *          This main loop supports event handling for the GMainLoop and the Ecore Main Loop.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] argc The argument count
 * @param[in] argv The argument vector
 * @param[in] callback The set of callback functions to handle application events
 * @param[in] user_data The user data to be passed to the callback functions
 * @return @c 0 on success,
 *         otherwise a negative error value.
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT The application is launched illegally, not launched by the launch system.
 * @retval #APP_ERROR_ALREADY_RUNNING The main loop has already started
 * @see service_app_create_cb()
 * @see service_app_terminate_cb()
 * @see service_app_control_cb()
 * @see service_app_exit()
 * @see #service_app_lifecycle_callback_s
 */
int service_app_main(int argc, char **argv, service_app_lifecycle_callback_s *callback, void *user_data);


/**
 * @brief Exits the main loop of the application.
 * @details The main loop of the application stops and service_app_terminate_cb() is invoked.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see service_app_main()
 * @see service_app_terminate_cb()
 */
void service_app_exit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_SERVICE_APP_H__ */
