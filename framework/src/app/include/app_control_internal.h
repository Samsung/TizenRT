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

#ifndef __TIZEN_APPFW_APP_CONTROL_INTERNAL_H__
#define __TIZEN_APPFW_APP_CONTROL_INTERNAL_H__

#include <app/bundle.h>

#include <app/app_control.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_control_internal.h
 */

/**
 * @addtogroup CAPI_APP_CONTROL_MODULE
 * @{
 */

/**
 * @brief Definition for the app_control data: Connects the previous application with the next application when the sub-application is terminated.
 * @details If a sub-application is terminated, the framework will connect the previous application with the next application.
 *          By default, this flag is 'false'
 */
#define APP_CONTROL_DATA_REROUTE "__K_REROUTE__"

/**
 * @brief Definition for the app_control data: The flag for attaching the application.
 * @details If this flag is set, the callee application will be attached to the caller application for a while and it will be detached when the callee application is lowered.
 *          By default, this flag is 'false'
 */
#define APP_CONTROL_DATA_SHIFT_WINDOW "__K_SHIFT_WINDOW"

/**
 * @brief Definition for the app_control data: The flag for supporting recycling processes.
 * @details By default, this flag is 'false'. Once it is set to 'true', the launched sub-application process will be reused even if it was lowered later.
 *
 */
#define APP_CONTROL_DATA_RECYCLE "__K_RECYCLE"

/**
 * @brief Definition for the app_control data: The value for supporting the relocating launched application under the callee application.
 * @details By default, this value is NULL. Once it is set to application ID, that application will be relocated under the callee applictaion after the callee application is resumed.
 * @remarks The value should be an application ID for the main application which has been launched before, otherwise it will be ignored.
 */
#define APP_CONTROL_DATA_RELOCATE_BELOW "__K_RELOCATE_BELOW"

/**
 * @brief Definition for the app_control data: The value for supporting the background launch mode.
 * @details By default, this flag is 'disable'. Once it is set to 'enable', A window of the callee application doesn't appear on the screen.
 */
#define APP_CONTROL_DATA_BACKGROUND_LAUNCH "__K_BG_LAUNCH"

/**
 * @brief Definition for the app_control data: The value for checking RPC-Port request.
 * @details If the value exists, the application is launched by RPC-Port request.
 */
#define APP_CONTROL_DATA_RPC_PORT "__K_RPC_PORT"

/**
 * @brief Replaces all data in the app_control with the bundle.
 *
 * @remarks This function clears all data in the app_control and adds all key-value pairs in the bundle into the app_control.
 * @param [in] app_control The app_control handle
 * @param [in] data The bundle handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_export_as_bundle()
 * @code
 *
 * #include <app/bundle.h>
 * #include <app/app_control.h>
 *
 * app_control_h app_control = NULL;
 * app_control_create(&app_control);
 * app_control_import_from_bundle(app_control, b);
 *
 * @endcode
 *
 */
int app_control_import_from_bundle(app_control_h app_control, bundle *data);

/**
 * @brief Returns a new bundle containing all data contained int the app_control.
 *
 * @remarks The @a data must be released with bundle_free() by you.
 * @param [in] app_control The app_control handle
 * @param [out] data The bundle handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_control_import_from_bundle()
 * @code
 *
 * #include <app/bundle.h>
 * #include <app/app_control.h>
 *
 * bundle* b = NULL;
 * app_control_export_as_bundle(app_control, &b);
 *
 * @endcode
 */
int app_control_export_as_bundle(app_control_h app_control, bundle **data);

int app_control_create_request(bundle *data, app_control_h *app_control);

int app_control_create_event(bundle *data, app_control_h *app_control);

int app_control_to_bundle(app_control_h app_control, bundle **data);

/**
 * @brief Sets the window ID of the application.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[in] id The window ID of the caller application (if the @a id is not positive, it clears the previous value)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_get_window()
 */
int app_control_set_window(app_control_h app_control, unsigned int id);

/**
 * @brief Gets the window ID of the application.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] app_control The app_control handle
 * @param[out] id The window ID of the caller application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_app_id()
 */
int app_control_get_window(app_control_h app_control, unsigned int *id);

/**
 * @brief Requests the specified callee window to be transient for the caller window.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a callee_id window is transient for the top-level caller window and should be handled accordingly.
 * @param[in] app_control The app_control handle
 * @param[in] callee_id The callee window ID
 * @param[in] cbfunc The callback function to be called when the transient is requested
 * @param[in] data A data pointer to pass to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_control_request_transient_app(app_control_h app_control, unsigned int callee_id, app_control_host_res_fn cbfunc, void *data);

/**
 * @platform
 * @brief Sets the ID of the default application associated with an operation, MIME-type, and URI.
 *
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in] app_control The app_control handle
 * @param[in] app_id The ID of the application
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #APP_CONTROL_ERROR_IO_ERROR IO error
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int app_control_set_defapp(app_control_h app_control, const char *app_id);

/**
 * @platform
 * @brief Unsets the default application control setting of an application.
 *
 * @details When the user calls this API, all the default application settings for the app_id are unset.
 *
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in] app_id The ID of the application
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #APP_CONTROL_ERROR_IO_ERROR IO error
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int app_control_unset_defapp(const char *app_id);

/**
 * @brief Sets the instance ID of the application.
 *
 * @since_tizen tizen_3.0
 * @param[in] app_control The app_control handle
 * @param[in] instance_id The instance ID of the application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_OUT_OF_MEMORY Out of memory
 */
int app_control_set_instance_id(app_control_h app_control, const char *instance_id);

/**
 * @brief Gets the instance ID of the application.
 *
 * @since_tizen tizen_3.0
 * @param[in] app_control The app_control handle
 * @param[out] instance_id The instance ID of the application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_OUT_OF_MEMORY Out of memory
 */
int app_control_get_instance_id(app_control_h app_control, char **instance_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_CONTROL_INTERNAL_H__ */
