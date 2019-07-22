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


#ifndef __TIZEN_NETWORK_BLUETOOTH_INTERNAL_H__
#define __TIZEN_NETWORK_BLUETOOTH_INTERNAL_H__

#ifdef GLIB_SUPPORTED
#include <glib.h>
#endif
#include "bluetooth_type.h"
#include "bluetooth_type_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file        bluetooth_internal.h
 */

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enables the local Bluetooth adapter, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function enables Bluetooth protocol stack and hardware.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_ALREADY_DONE  Already enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @pre The state of local Bluetooth must be #BT_ADAPTER_DISABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_initialize()
 * @see bt_adapter_get_state()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 * @see bt_adapter_state_changed_cb()
 *
 */
int bt_adapter_enable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Disables the local Bluetooth adapter, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function disables Bluetooth protocol stack and hardware.
 *
 * @remarks You should disable Bluetooth adapter, which is helpful for saving power.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_adapter_get_state()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb ()
 *
 */
int bt_adapter_disable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Recover the local Bluetooth adapter, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function does recovery logic, disables Bluetooth protocol stack and hardware, then enables after a few seconds.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_adapter_get_state()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb ()
 *
 */
int bt_adapter_recover(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  A2DP source/sink role is switched using this API
 * @since_tizen 4.0
 * @param[in] Role for A2DP
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_OPERATION_FAILED Internal Error
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_initialize()
 * @see bt_audio_initialize()
 */
int bt_audio_select_role(bt_audio_role_e role);
/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Reset the local Bluetooth adapter, synchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function resets Bluetooth protocol and values.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_state_changed_cb() will be invoked if The state of local Bluetooth was #BT_ADAPTER_ENABLED.
 *
 * @see bt_initialize()
 * @see bt_adapter_get_state()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 * @see bt_adapter_state_changed_cb()
 *
 */
int bt_adapter_reset(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the version of local Bluetooth adapter.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @remarks The @a local_version must be released with free() by you.
 *
 * @param[out] local_version The version of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_version(char **local_version);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the information regarding local Bluetooth adapter.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @remarks The @a all parameters must be released with free() by you.
 *
 * @param[out] chipset Chipset name of local Bluetooth adapter
 * @param[out] firmware Firmware info. of local Bluetooth adapter
 * @param[out] stack_version Bluetooth stack version
 * @param[out] profiles The profile list of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_local_info(char **chipset, char **firmware, char **stack_version, char **profiles);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the visibility mode.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE will change to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE
 * after the given @a duration goes.
 *
 * @param[in] discoverable_mode The Bluetooth visibility mode to set
 * @param[in] duration The duration until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE (in seconds).
 * @a duration is used only for #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE mode.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_visibility_mode_changed_cb() will be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_get_visibility()
 * @see bt_adapter_visibility_mode_changed_cb()
 * @see bt_adapter_set_visibility_mode_changed_cb()
 * @see bt_adapter_unset_visibility_mode_changed_cb()
 */
int bt_adapter_set_visibility(bt_adapter_visibility_mode_e discoverable_mode, int duration);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Registers a callback function that will be invoked when remote device requests authentication.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED Not initialized
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @param[in] callback  callback function to be set when a request is received.
 * @param[in] user_data data from application which will be provided in callback.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @see bt_initialize()
 * @see bt_adapter_set_authentication_req_cb()
 */
int bt_adapter_set_authentication_req_cb(bt_adapter_authentication_req_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Unregisters a callback function that will be invoked when remote device requests authentication.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED Not initialized
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_adapter_set_authentication_req_cb()
 */
int bt_adapter_unset_authentication_req_cb(void);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  API to reply with PIN or PASSKEY with authentication type - TRUE or FALSE.
 * @remarks  This function can be called by application when remote device requests PIN or PASSKEY from
 *           local adapter.
 * @param[in]  passkey  The passkey to be provided by application when remote devices requests for it.
 * @param[in]  authentication_reply This indicates whether application wants to accept or cancel the on-going pairing
 * @pre  This function can only be called when application receieves authentication event (BT_AUTH_PIN_REQUEST)
 *       from remote device.
 * @see  bt_adapter_set_authentication_req_cb()
 */
int bt_adapter_passkey_reply(char *passkey, bool authentication_reply);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  API to reply to the PASSKEY confirmation for on-going pairing with remote device.
 * @remarks  This function can be called by application, when local adapter wants PASSKEY confirmation from user.
 * @param[in]  confirmation_reply This indicates whether application wants to accepts or cancels the on-going pairing
 *             confirmation_reply : TRUE will indicate that Application has confirmed the PASSKEY
 *             confirmation_reply : FALSE will indicate that Application has failed to confirm the PASSKEY. In this situation
 *             the pairing will be failed.
 * @pre  This function can only be called when application receives authentication event (BT_AUTH_PASSKEY_CONFIRM_REQUEST)
 *       from remote device.
 * @see  bt_adapter_set_authentication_req_cb()
 */
int bt_adapter_passkey_confirmation_reply(bool confirmation_reply);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the connectable state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_connectable_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
int bt_adapter_set_connectable_changed_cb(bt_adapter_connectable_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Unregisters the callback function.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 *
 * @see  bt_initialize()
 * @see  bt_adapter_set_connectable_changed_cb()
 */
int bt_adapter_unset_connectable_changed_cb(void);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Gets the connectable state of local Bluetooth adapter.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @remarks When connectable state is false, no device can connect to this device and visibility mode cannot be changed.
 *
 * @param[out] connectable The connectable state of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_set_connectable()
 */
int bt_adapter_get_connectable(bool *connectable);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Sets the connectable state of local Bluetooth adapter.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks When connectable state is false, no device can connect to this device and visibility mode cannot be changed.
 *
 * @param[in] connectable The connectable state to set
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_connectable_changed_cb() will be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_get_connectable()
 * @see bt_adapter_connectable_changed_cb()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
int bt_adapter_set_connectable(bool connectable);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the manufacturer data of local Bluetooth adapter.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]   data	The manufacturer specific data of the Bluetooth device.
 * @param[in]   len	The length of @a data.Maximaum length is 240 bytes.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_manufacturer_data_changed_cb() will be invoked
 * if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_manufacturer_data_changed_cb
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
int bt_adapter_set_manufacturer_data(char *data, int len);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Parse the samsung specific manufacturer data of discovered device.
 *
 * @remarks bt_adapter_free_manufacturer_data should be invoked
 * to free the memory associated with parsed data
 *
 * @param[in]   manufacturer_data	The manufacturer specific data of the discovered Bluetooth device.
 * @param[in]   len		length of the received manufacturer data
 * @param[out] data Parsed manufacturer data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_free_manufacturer_data()
 */
int bt_adapter_parse_manufacturer_data(bt_manufacturer_data *data,
		char *manufacturer_data, int manufacturer_data_len);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Free the memory associated with parsed manufacturer data
 *
 * @param[in] data parsed manufactrer data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 *
 * @see bt_adapter_parse_manufacturer_data()
 */
int bt_adapter_free_manufacturer_data(bt_manufacturer_data *data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked
 * when the manufacturer data of Bluetooth adapter changes.
 * @since_tizen 2.3
 *
 * @param[in]   callback	The callback function to invoke
 * @param[in]   user_data	The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_adapter_manufacturer_data_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
int bt_adapter_set_manufacturer_data_changed_cb(
		bt_adapter_manufacturer_data_changed_cb callback,
		void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	Unregisters the callback function.
 * @since_tizen 2.3
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 */
int bt_adapter_unset_manufacturer_data_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Enables the local Bluetooth le adapter, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function enables Bluetooth protocol stack and hardware.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_ALREADY_DONE  Already enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @post This function invokes bt_adapter_le_state_changed_cb().
 *
 * @see bt_initialize()
 * @see bt_adapter_le_get_state()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 * @see bt_adapter_le_state_changed_cb()
 *
 */
int bt_adapter_le_enable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Disables the local Bluetooth le adapter, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function disables Bluetooth le protocol stack and hardware.
 *
 * @remarks
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_LE_ENABLED
 * @post This function invokes bt_adapter_le_state_changed_cb().
 *
 * @see bt_adapter_le_get_state()
 * @see bt_adapter_le_state_changed_cb()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb ()
 *
 */
int bt_adapter_le_disable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Gets the current state of local Bluetooth adapter.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[out] adapter_le_state The current adapter le state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 */
int bt_adapter_le_get_state(bt_adapter_le_state_e *adapter_le_state);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Registers a callback function to be invoked when the Bluetooth adapter le state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @param[in] callback	The callback function to invoke
 * @param[in] user_data	The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_le_state_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_le_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 */
int bt_adapter_le_set_state_changed_cb(bt_adapter_le_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief	Unregisters the callback function.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_le_set_state_changed_cb()
 */
int bt_adapter_le_unset_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief add address to whitelist for accepting scanning request.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks If the adress is in the whitelist then other LE devices are able to
 * search this device. Before calling this API, make sure that the adapter is
 * enabled. There is no callback event for this API.

 * @param[in] address The other device's address
 * @param[in] address_type The other device's address type
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Adapter is not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_le_start_advertising_new()
 * @see bt_adapter_le_stop_advertising()
 */
int bt_adapter_le_add_white_list(const char *address, bt_device_address_type_e address_type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief remove address from the whitelist for not accepting scanning request.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks If the adress is in the whitelist then other LE devices are able to
 * search this device. Before calling this API, make sure that the adapter is
 * enabled. There is no callback event for this API.
 *
 * @param[in] address The other device's address
 * @param[in] address_type The other device's address type
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Adapter is not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_le_start_advertising_new()
 * @see bt_adapter_le_stop_advertising()
 */
int bt_adapter_le_remove_white_list(const char *address, bt_device_address_type_e address_type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Sets the Privacy feature state of local Bluetooth adapter.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] enable_privacy The privacy feature to set/unset.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The state of local Bluetooth must be #BT_ADAPTER_LE_ENABLED.
 *
 */
int bt_adapter_le_enable_privacy(bool enable_privacy);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Sets the Hash and Randomizer value, synchronously for LE OOB pairing.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address Remote device address
 * @param[in] address_type Remote device address type
 * @param[in] hash256 The P-256 hash value received via OOB from remote device
 * @param[in] randomizer256 The P-256 randomizer value received via OOB from remote device
 * @param[in] hash256_len The length of @a hash256
 * @param[in] randomizer256_len The length of @a randomizer256
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 */
int bt_adapter_le_set_remote_oob_ext_data(const char *remote_address,
		bt_device_address_type_e address_type,
		const unsigned char *hash256, const unsigned char *randomizer256,
		int hash256_len, int randomizer256_len);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Set advertising filter policy to use white list
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 *
 * @param[in] advertiser The handle of advertiser
 * @param[in] filter_policy The filter policy of advertising
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation is now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_adapter_le_start_advertising_new()
 */
int bt_adapter_le_set_advertising_filter_policy(bt_advertiser_h advertiser, bt_adapter_le_advertising_filter_policy_e filter_policy);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Retrieves the address of the devices connected with the specific profile.
 * @since_tizen 4.0
 *
 * @param[in] profile_uuid The UUID of profile service uuid of the connected device
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY	Out of memory
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  There is no connected device
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED	Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post This function invokes bt_adapter_profile_connected_devices_cb().
 *
 * @see bt_adapter_profile_connected_devices_cb()
 */
int bt_adapter_foreach_profile_connected_devices(const char *profile_uuid,
	bt_adapter_profile_connected_devices_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Disconnects a remote Bluetooth device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_device_set_connection_state_changed_cb()
 */
int bt_device_disconnect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets a connection state
 * @since_tizen @if WEARABLE @else 2.4 @endif
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[in] link_type The link type to get a connection state
 * @param[out] connected The connection state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED   Operation failed
 */
int bt_device_get_connection_state(const char *remote_address, bt_device_connection_link_type_e link_type, bool *connected);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Creates a bond with a remote Bluetooth device, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks A bond can be destroyed by bt_device_destroy_bond().\n
 * The bonding request can be cancelled by bt_device_cancel_bonding().
 *
 * @param[in] remote_address The address of the remote Bluetooth device with which the bond should be created
 * @param[in] conn_type The connection type(LE or BREDR) to create bond with remote device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY     Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @post This function invokes bt_device_bond_created_cb().
 *
 * @see bt_adapter_start_device_discovery()
 * @see bt_device_create_bond()
 * @see bt_device_bond_created_cb()
 * @see bt_device_cancel_bonding()
 * @see bt_device_destroy_bond()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
int bt_device_create_bond_by_type(const char *remote_address,
				  bt_device_connection_link_type_e conn_type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Cancels service search process.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation not in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The service search must be in progress by bt_device_start_service_search().
 *
 * @see bt_device_start_service_search()
 * @see bt_device_service_searched_cb()
 * @see bt_device_set_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 */
int bt_device_cancel_service_search(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets a connection state
 * @since_tizen @if WEARABLE 2.4 @endif
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[in] link_type The link type to get a connection state
 * @param[out] connected The connection state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED   Operation failed
 */
int bt_device_get_connection_state(const char *remote_address, bt_device_connection_link_type_e link_type, bool *connected);

/**
 * @internal
 * @deprecated Deprecated since 4.0. Use bt_device_set_profile_trusted instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Sets a profile restricted connection for a device
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation not in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_device_set_profile_restricted(const char *device_address, bt_restricted_profile_t profile, bool restricted);

/**
 * @internal
 * @deprecated Deprecated since 4.0. Use bt_device_get_profile_trusted instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets a profile restricted connection for a device
 * @since_tizen 3.0
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation not in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_device_get_profile_restricted(const char *device_address, bt_restricted_profile_t profile, int *restricted);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief   Set a profile trusted or untrusted for a device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INTERNAL Internal Error
 * @retval #BT_ERROR_NOT_INITIALIZED  Not supported
 * @retval #BT_ERROR_INVALID_PARAMETER Invalid Parameter
 * @retval #BT_ERROR_NOT_ENABLED Invalid Parameter
 */
int bt_device_set_profile_trusted(const char *device_address, bt_trusted_profile_t profile, bool trust);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief   Gets a profile is trusted or untrusted for a device.
 * @since_tizen 4.0
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INTERNAL Internal Error
 * @retval #BT_ERROR_NOT_INITIALIZED  Not supported
 * @retval #BT_ERROR_NOT_ENABLED Invalid Parameter
 * @retval #BT_ERROR_INVALID_PARAMETER Invalid Parameter
 */
int bt_device_get_profile_trusted(const char *device_address, bt_trusted_profile_t profile, int *trust);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief   Registers a callback function to be invoked when Trusted Profiles are changed.
 * @since_tizen 2.4
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not supported
 * @retval #BT_ERROR_INVALID_PARAMETER Invalid Parameter
 */
int bt_device_set_trusted_profile_cb(bt_device_trusted_profiles_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief   Unregisters the callback function registered with bt_device_set_trusted_profile_cb().
 * @since_tizen 2.4
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not supported
 */
int bt_device_unset_trusted_profile_cb(void);

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_request_att_mtu_change instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Request to change ATT MTU value
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[in] mtu New MTU value to be changed
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED   Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_device_request_att_mtu(const char *remote_address, unsigned int mtu) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_get_att_mtu instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets the ATT MTU value set for a connection
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[ot] mtu MTU value set for a connection
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED	Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED	Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED	Not supported
 */
int bt_device_get_att_mtu(const char *remote_address, unsigned int *mtu) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_set_att_mtu_changed_cb instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Registers a callback function to be invoked when the ATT MTU is changed.
 * @since_tizen 3.0
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_device_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_device_unset_att_mtu_changed_cb()
 */
int bt_device_set_att_mtu_changed_cb(bt_device_att_mtu_changed_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_unset_att_mtu_changed_cb instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief	Unregisters the callback function to be invoked when the ATT MTU is changed.
 * @since_tizen 3.0
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_device_set_att_mtu_changed_cb()
 */
int bt_device_unset_att_mtu_changed_cb(void) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Registers a rfcomm server socket with a specific UUID. Activation by dbus is possible when the profile is connected.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks A socket can be destroyed by bt_socket_destroy_rfcomm_ex().
 * Application should call this API to receive a connection event when launched again by dbus.
 *
 * @param[in] uuid The UUID of service to provide
 * @param[in] bus_name bus_name of the application which is provided in service file.
 * @param[in] object_path dbus of the application
 * @return 0 on success, otherwise a negative error value.
 *
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Already registered
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_socket_listen_and_accept_rfcomm_ex()
 * @see bt_socket_destroy_rfcomm_ex()
 */
int bt_socket_create_rfcomm_ex(const char *uuid, const char *bus_name, const char *object_path);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Removes the rfcomm server socket which was created using bt_socket_create_rfcomm_ex().
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @remarks If callback function bt_socket_connection_state_changed_cb() is set and the remote Bluetooth device is connected,
 * then bt_socket_connection_state_changed_cb() will be called when this function is finished successfully.
 *
 * @param[in] uuid The UUID (which was created using bt_socket_create_rfcomm()) to destroy
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The socket must be created with bt_socket_create_rfcomm_ex().
 * @post If callback function bt_socket_connection_state_changed_cb() is set and the remote Bluetooth device is connected,
 * then bt_socket_connection_state_changed_cb() will be called.
 * @see bt_socket_create_rfcomm_ex()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_destroy_rfcomm_ex(const char *uuid);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Starts listening on passed rfcomm socket and accepts connection requests. Activation by dbus is possible when the profile is connected.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @details Pop-up is shown automatically when a RFCOMM connection is requested.
 * bt_socket_connection_state_changed_cb() will be called with
 * #BT_SOCKET_CONNECTED if you click "yes" and connection is finished successfully.
 * @param[in] uuid The UUID of service to provide
 * @param[in] max_pending_connections The maximum number of pending connections
 * @param[in] bus_name bus_name of the application which is provided in service file.
 * @param[in] object_path dbus of the application
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The socket must be created with bt_socket_create_rfcomm_ex().
 * @post If callback function bt_socket_connection_state_changed_cb() is set,
 * then bt_socket_connection_state_changed_cb() will be called when the remote Bluetooth device is connected.
 * @see bt_socket_create_rfcomm_ex()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_listen_and_accept_rfcomm_ex(const char *uuid, int max_pending_connections, const char* bus_name, const char *object_path);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Starts listening on passed rfcomm socket.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @details bt_socket_connection_requested_cb() will be called when a RFCOMM connection is requested.
 *
 * @param[in] socket_fd  The file descriptor socket on which start to listen
 * @param[in] max_pending_connections  The number of pending connections
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The socket must be created with bt_socket_create_rfcomm().
 * @post This function invokes bt_socket_connection_state_changed_cb().
 *
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_set_connection_requested_cb()
 * @see bt_socket_unset_connection_requested_cb()
 * @see bt_socket_connection_requested_cb()
 */
int bt_socket_listen(int socket_fd, int max_pending_connections);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Accepts a connection request.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] requested_socket_fd  The file descriptor of socket on which a connection is requested
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The connection is requested by bt_socket_connection_requested_cb().
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_requested_cb()
 * @see bt_socket_listen()
 * @see bt_socket_reject()
*/
int bt_socket_accept(int requested_socket_fd);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Rejects a connection request.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] socket_fd  The file descriptor of socket on which a connection is requested
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The connection is requested by bt_socket_connection_requested_cb().
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_requested_cb()
 * @see bt_socket_listen()
 * @see bt_socket_accept()
 */
int bt_socket_reject(int socket_fd);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Initializes the Bluetooth OPP server requested by bt_opp_server_push_requested_cb().
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @details The popup appears when an OPP connection is requested from a remote device.
 * If you accept the request, then connection will be established and bt_opp_server_push_requested_cb() will be called.
 * At that time, you can call either bt_opp_server_accept() or bt_opp_server_reject().
 * @remarks This function must be called to start Bluetooth OPP server. You must free all resources of the Bluetooth service
 * by calling bt_opp_server_deinitialize() if Bluetooth OPP service is no longer needed.
 * @param[in] destination  The destination path
 * @param[in] push_requested_cb  The callback called when a push is requested
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED   Not supported
 *
 * @see  bt_opp_server_push_requested_cb()
 * @see  bt_opp_server_deinitialize()
 * @see  bt_opp_server_accept()
 * @see  bt_opp_server_reject()
 */
int bt_opp_server_initialize(const char *destination, bt_opp_server_push_requested_cb push_requested_cb, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_MODULE
 * @brief Gets the percentage progress for ongoing transfers.
 * @since_tizen 3.0
 * @exception   None
 * @param[in] transfer_type	Transfer Type: (@c BLUETOOTH_TRANSFER_INBOUND = receiving , @c  BLUETOOTH_TRANSFER_OUTBOUND = sending)
 * @param[in] transfer_id	Transfer ID
 * @param[out] progress		Percentage Progress
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED	Operation failed
 */
int bt_opp_get_transfer_progress(bt_opp_transfer_type_t transfer_type,
		int transfer_id, unsigned char *progress);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the call event to the remote bluetooth device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @remarks Before notifying #BT_AG_CALL_EVENT_ANSWERED or #BT_AG_CALL_EVENT_DIALING, you should open SCO(Synchronous Connection Oriented link)
 * if Bluetooth Hands-Free need SCO connection.
 * @param[in] event  The call event
 * @param[in] call_id  The call ID
 * @param[in] phone_number  The phone number. You must set this value in case of #BT_AG_CALL_EVENT_DIALING and #BT_AG_CALL_EVENT_INCOMING.
 * In other cases, this value can be NULL.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_call_event(bt_ag_call_event_e event, unsigned int call_id, const char *phone_number);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the call list to the remote bluetooth device.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] list  The call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_call_list(bt_call_list_h list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the state of voice recognition.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] state  The state of voice recognition: (@c true = enabled, @c  false = disabled)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_voice_recognition_state(bool state);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
int bt_ag_set_call_handling_event_cb(bt_ag_call_handling_event_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_set_call_handling_event_cb()
 */
int bt_ag_unset_call_handling_event_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
int bt_ag_set_multi_call_handling_event_cb(bt_ag_multi_call_handling_event_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_set_multi_call_handling_event_cb()
 */
int bt_ag_unset_multi_call_handling_event_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_dtmf_transmitted_cb()
 * @see bt_ag_unset_dtmf_transmitted_cb()
 */
int bt_ag_set_dtmf_transmitted_cb(bt_ag_dtmf_transmitted_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_dtmf_transmitted_cb()
 * @see bt_ag_set_dtmf_transmitted_cb()
 */
int bt_ag_unset_dtmf_transmitted_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Checks whether the remoted device enables NREC(Noise Reduction and Echo Canceling) or not.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[out] enabled The NREC status: (@c true = enabled, @c  false = not enabled)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_audio_connect()
 */
int bt_ag_is_nrec_enabled(bool *enabled);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Creates a handle of call list.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[out] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_destroy()
 */
int bt_call_list_create(bt_call_list_h *list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Destroys the handle of call list.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_create()
 */
int bt_call_list_destroy(bt_call_list_h list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Resets the handle of call list.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_create()
 */
int bt_call_list_reset(bt_call_list_h list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Adds a call to the handle of call list.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] list  The handle of call list
 * @param[in] call_id  The call ID
 * @param[in] state  The state of audio gate call
 * @param[in] phone_number The phone number. You must set this value in case of #BT_AG_CALL_EVENT_DIALING and      #BT_AG_CALL_EVENT_INCOMING.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_create()
 */
int bt_call_list_add(bt_call_list_h list, unsigned int call_id, bt_ag_call_state_e state, const char *phone_number);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a XSATvendor AT command is transmitted from Hands-Free.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_vendor_cmd_cb()
 * @see bt_ag_unset_vendor_cmd_cb()
 */
int bt_ag_set_vendor_cmd_cb(bt_ag_vendor_cmd_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a XSATvendor AT command is transmitted from Hands-Free
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_vendor_cmd_cb()
 * @see bt_ag_set_vendor_cmd_cb()
 */
int bt_ag_unset_vendor_cmd_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Checks whether the remoted device is wbs (Wide Band Speech) mode or not.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[out] wbs_mode The wbs status: (@c true = wide band speech, @c  false = narrow band speech)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_audio_connect()
 */
int bt_ag_is_wbs_mode(bool *wbs_mode);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Gets the HF(Hands-Free) profile connected status for AG role.
 * @since_tizen 2.4
 * @param[out] connected the connected status: (@c true = connected , @c  false = not connected )
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_ag_is_connected(bool *connected);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Switch between the connected headsets for AG role.
 * @since_tizen 3.0
 * @param[out]
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED	Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Not connected
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_ag_switch_headset(const char *remote_addr);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Registers a callback function that will be invoked when the SCO(Synchronous Connection Oriented link) state is changed.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 */
int bt_hf_set_sco_state_changed_cb(bt_hf_sco_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Unregisters a callback function that will be invoked when the SCO(Synchronous Connection Oriented link) state is changed.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_sco_state_changed_cb()
 * @see bt_ag_set_sco_state_changed_cb()
 */
int bt_hf_unset_sco_state_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Registers a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
int bt_hf_set_call_handling_event_cb(bt_hf_call_handling_event_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Unregisters a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_set_call_handling_event_cb()
 */
int bt_hf_unset_call_handling_event_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Registers a callback function that will be invoked when a call event happend from Audio-Gateway device.
 * @since_tizen 3.0
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_hf_remote_call_event_cb()
 * @see bt_hf_unset_remote_call_event_cb()
 */
int bt_hf_set_remote_call_event_cb(bt_hf_remote_call_event_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Unregisters a callback function.
 * @since_tizen 3.0
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_hf_remote_call_event_cb()
 * @see bt_hf_set_remote_call_event_cb()
 */
int bt_hf_unset_remote_call_event_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Registers a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
int bt_hf_set_multi_call_handling_event_cb(bt_hf_multi_call_handling_event_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Unregisters a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_set_multi_call_handling_event_cb()
 */
int bt_hf_unset_multi_call_handling_event_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Initializes the Bluetooth HF profile related with audio.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @remarks This function must be called before Bluetooth profiles related with audio starts.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_hf_deinitialize()
 */
int bt_hf_initialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Deinitializes the Bluetooth HF profile related with audio.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 */
int bt_hf_deinitialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Notifies the call event to the remote bluetooth device.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege http://tizen.org/privilege/bluetooth.admin
 * @remarks Before notifying #BT_HF_CALL_EVENT_ANSWERED or #BT_HF_CALL_EVENT_DIALING, you should open SCO(Synchronous Connection Oriented link)
 * if Bluetooth Hands-Free need SCO connection.
 * @param[in] event  The call event
 * @param[in] call_id  The call ID
 * @param[in] phone_number  The phone number. You must set this value in case of #BT_HF_CALL_EVENT_DIALING and #BT_HF_CALL_EVENT_INCOMING.
 * In other cases, this value can be NULL.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_hf_notify_call_event(bt_hf_call_event_e event, char *phone_number);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Notifies the speaker gain to the remote device.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege http://tizen.org/privilege/bluetooth.admin
 * @details This function sends a signal to the remote device. This signal has the gain value.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * When the speaker gain of remote device is changed to the requested gain, bt_audio_speaker_gain_changed_cb() will be called.
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_hf_get_speaker_gain()
 * @see bt_hf_set_speaker_gain_changed_cb()
 * @see bt_hf_unset_speaker_gain_changed_cb()
 */
int bt_hf_notify_speaker_gain(int gain);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Registers a callback function that will be invoked when the speaker gain of the remote device is changed.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @details This function let you know the change of the speaker gain of the remote device.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_hf_unset_speaker_gain_changed_cb()
 */
int bt_hf_set_speaker_gain_changed_cb(bt_hf_speaker_gain_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Unregisters a callback function that will be invoked when the speaker gain of the remote device is changed.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_hf_set_speaker_gain_changed_cb()
 */
int bt_hf_unset_speaker_gain_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Notifies the state of voice recognition.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege http://tizen.org/privilege/bluetooth.admin
 * @param[in] state  The state of voice recognition: (@c true = enabled, @c  false = disabled)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_hf_notify_voice_recognition_state(bool state);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Registers a callback function that will be invoked when a call status event happened from Hands-Free.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_hf_unset_call_status_updated_event_cb()
 */
#ifdef GLIB_SUPPORTED
int bt_hf_set_call_status_updated_event_cb(bt_hf_call_status_updated_event_cb callback, void *user_data);
#endif

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief  Unregisters a callback function that will be invoked when a call status event happened from Hands-Free.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_hf_set_call_status_updated_event_cb()
 */
int bt_hf_unset_call_status_updated_event_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Closes an opened SCO(Synchronous Connection Oriented link), synchronously.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_hf_close_sco(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Sends the DTMF(Dual Tone Multi Frequency).
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege http://tizen.org/privilege/bluetooth.admin
 * @param[in] dtmf  The DTMF to send
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_hf_send_dtmf(char *dtmf);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Gets the HF(Hands-Free) profile connected status for HF role.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @param[out] connected the connected status: (@c true = connected , @c  false = not connected )
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED	Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_hf_is_connected(bool *connected);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Checks whether an opened SCO(Synchronous Connection Oriented link) exists or not.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] opened The SCO status: (@c true = opened, @c  false = not opened)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_hf_is_sco_opened(bool *opened);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Gets the codec ID.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a codec_id must be released with free() by you.
 *
 * @param[out] codec_id The codec ID
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_hf_get_codec_id(unsigned int *codec_id);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Gets the call status information list.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 * @remarks The @a call_list must be released with bt_hf_free_call_status_info_list() by you.
 * @remarks call_info_list has elements which consist of bt_hf_call_status_info_s
 *
 * @param[out] call_list The call status information list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_hf_call_status_info_s
 * @see bt_audio_connect()
 */
#ifdef GLIB_SUPPORTED
int bt_hf_get_call_status_info_list(GSList **call_list);
#endif

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Frees the call status information list.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] call_list The call status information list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_hf_get_call_status_info_list()
 */
#ifdef GLIB_SUPPORTED
int bt_hf_free_call_status_info_list(GSList *call_list);
#endif

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Requests the call status information list.
 * @since_tizen 3.0
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NO_DATA No data available
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @pre The bt_hf_set_call_status_updated_event_cb() should be set to receive
 * the event of call status update.
 * @post This function invokes bt_hf_call_status_updated_event_cb().
 *
 * @see bt_audio_connect()
 * @see bt_hf_set_call_status_updated_event_cb()
 * @see bt_hf_unset_call_status_updated_event_cb()
 * @see bt_hf_call_status_updated_event_cb()
 */
int bt_hf_request_call_status_info_list(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Checks whether the remote AG device supports inband ringtone or not.
 * @since_tizen @if MOBILE @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] supported Inband support: (@c true = supported, @c  false = not supported)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_hf_is_inband_ringtone_supported(bool *supported);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AUDIO_A2DP_MODULE
 * @brief  Sets copy protection. streaming application that needs to have the copy protection for the streaming data, shall invoke this API.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] status - TRUE/FALSE
 * @return  0 on success, otherwise negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_OPERATION_FAILED  on failure
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_a2dp_set_content_protection(bool status);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE
 * @brief Sends a command to the target device.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] cmd The commands, one of: Play, Pause, Next, Rewind.
 * @param[in] remote_address The address of the remote Bluetooth device
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_avrcp_control_initialize().
 * @see bt_avrcp_control_initialize()
 * @see bt_avrcp_control_connect()
 */
int bt_avrcp_control_send_player_command_to(bt_avrcp_player_command_e cmd, char *remote_address);


/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Gets the specification name from the UUID
 * @since_tizen 2.4
 *
 * @remarks @a name must be released with free() by you.
 *
 * @param[in] uuid The UUID
 * @param[out] name The specification name which defined from www.bluetooth.org
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_get_uuid()
 */
int bt_gatt_get_uuid_specification_name(const char *uuid, char **name);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Updates the permissions which a characteristic or descriptor's GATT handle has.
 * @since_tizen 3.0
 *
 * @param[in] gatt_handle The handle of a characteristic or descriptor
 * @param[in] permissions The permissions to be updated
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_characteristic_create()
 * @see bt_gatt_descriptor_create()
 * @see bt_gatt_permission_e
 */
int bt_gatt_set_permissions(bt_gatt_h gatt_handle, int permissions);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Updates the properties which a characteristic's GATT handle has.
 * @since_tizen 3.0
 *
 * @param[in] characteristic The characteristic's GATT handle
 * @param[in] properties The properties to be updated
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_property_e
 */
int bt_gatt_characteristic_set_properties(bt_gatt_h characteristic, int properties);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Destroys the GATT handle
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The handle of service, characteristic or descriptor
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_service_create()
 * @see bt_gatt_characteristic_create()
 * @see bt_gatt_descriptor_create()
 */
int bt_gatt_destroy(bt_gatt_h gatt_handle);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Registers a callback function that will be invoked when the A2DP Source connection state is changed.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_audio_connection_state_changed_cb()
 */
int bt_a2dp_source_audio_set_connection_state_changed_cb(bt_audio_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Unregisters a callback function that will be invoked when the A2DP Source connection state is changed.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_audio_connection_state_changed_cb()
 * @see bt_audio_set_connection_state_changed_cb()
 */
int bt_a2dp_source_audio_unset_connection_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Activates the NAP(Network Access Point).
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @see bt_nap_deactivate()
 */
int bt_nap_activate(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Deactivates the NAP(Network Access Point).
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_deactivate(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Disconnects the all PANUs(Personal Area Networking User) which are connected to you.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_disconnect_all(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Disconnects the specified PANU(Personal Area Networking User) which is connected to you.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_disconnect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_nap_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
int bt_nap_set_connection_state_changed_cb(bt_nap_connection_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_nap_connection_state_changed_cb()
 * @see bt_nap_set_connection_state_changed_cb()
 */
int bt_nap_unset_connection_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_panu_connection_state_changed_cb()
 * @see bt_panu_unset_connection_state_changed_cb()
 */
int bt_panu_set_connection_state_changed_cb(bt_panu_connection_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state changes.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_panu_connection_state_changed_cb()
 * @see bt_panu_set_connection_state_changed_cb()
 */
int bt_panu_unset_connection_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief Connects the remote device with the PAN(Personal Area Networking) service, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of PAN service
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @post bt_panu_connection_state_changed_cb() will be invoked.
 * @see bt_panu_disconnect()
 * @see bt_panu_connection_state_changed_cb()
 */
int bt_panu_connect(const char *remote_address, bt_panu_service_type_e type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief Disconnects the remote device with the PAN(Personal Area Networking) service, asynchronously.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected by bt_panu_connect().
 * @post bt_panu_connection_state_changed_cb() will be invoked.
 * @see bt_panu_connect()
 * @see bt_panu_connection_state_changed_cb()
 */
int bt_panu_disconnect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief   update LE connection.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @pre The remote device must be connected with bt_gatt_connect().
 */
int bt_device_le_conn_update(const char *device_address,
			     const bt_le_conn_update_s *parameters);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets the is_alias_set property of a bonded device.
 * @since_tizen 3.0
 *
 * @param[in] remote_address The address of remote device
 * @param[out] is_alias_set The is_alias_set property of device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
  * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_bonded_device_is_alias_set(const char *remote_address, bool *is_alias_set);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Registers the method for HID Device role
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks This function must be called to register HID UUID
 * then only remote device could be able identify this one as HID device
 *
 * @param[in] callback  The callback called when the connection state is changed
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Already activated
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED   Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_hid_device_deactivate()
 */
int bt_hid_device_activate(bt_hid_device_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Unregisters the method for HID Device role
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks This function must be called to deregister HID UUID
 *
 * @param[in] socket_fd on which uuid need to be deregistered.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Not activated
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_hid_device_activate()
 */
int bt_hid_device_deactivate(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Initiates HID device Connection with device role, asynchronously.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks This function must be called to Initiate Hid device role connection
 *
 * @param[in] remote_address device address of remote device.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Not activated
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_ALREADY_DONE   Already connected
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @see bt_initialize()
 * @see bt_hid_device_activate()
 */
int bt_hid_device_connect(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Disconnects the connection with HID Host device, asynchronously.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address device address of remote device.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_disconnect(const char *remote_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Sends the mouse event data.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address device address of remote device.
 * @param[in] mouse_data mouse data that need to be passed to remote device
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The HID connection must be established.
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_send_mouse_event(const char *remote_address,
			const bt_hid_mouse_data_s *mouse_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Sends the keyboard event data.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address device address of remote device.
 * @param[in] key_data  key data the need to be passed to remote device
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The HID connection must be established.
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_send_key_event(const char *remote_address,
			const bt_hid_key_data_s *key_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Sets the callback when device gets some data from HID Host.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 *
 * @param[in] callback  callback function to be set when data is received.
 * @param[in] user_data data from application which will be provided in callback.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_set_data_received_cb(bt_hid_device_data_received_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Unsets the callback when device gets some data from HID Host.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 *
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_unset_data_received_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Responds to the reports from HID Host.
 * @since_tizen @if WEAREABLE @else 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address device address of remote device.
 * @param[in] header_type Header type to be there in response
 * @param[in] param_type Parameter type to be there in response.
 * @param[in] data Data to be present in data payload of response.
 * @param[in] data_len The length of the response data
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The HID connection must be established.
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_reply_to_report(const char *remote_address,
		bt_hid_header_type_e header_type, bt_hid_param_type_e param_type,
		const char *data, unsigned int data_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Sends the custom event data.
 * @since_tizen @if WEAREABLE @else 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address device address of remote device.
 * @param[in] report_id  reoport id need to be passed to remote device
 * @param[in] data  The data need to be passed to remote device
 * @param[in] data_len  The length of the data
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The HID connection must be established.
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_send_custom_event(const char *remote_address,
		unsigned char report_id, const char *data, unsigned int data_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets Restriction for BT mode(BT allowed or not).
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - BT Allow value.
 *		BT_DPM_ERROR	 = -1,	 < bluetooth allowance error
 *		BT_DPM_BT_ALLOWED,		 < bluetooth allowance allowed
 *		BT_DPM_HANDSFREE_ONLY,  < bluetooth allowance handsfree only
 *		BT_DPM_BT_RESTRICTED,  < bluetooth allowance restricted
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre none.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_allow_bluetooth_mode(bt_dpm_allow_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads Restriction for BT mode(BT allowed or not).
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] none
 * @param[out]	value - BT Allow value.
 *		BT_DPM_ERROR	 = -1,	 < bluetooth allowance error
 *		BT_DPM_BT_ALLOWED,		 < bluetooth allowance allowed
 *		BT_DPM_HANDSFREE_ONLY,	< bluetooth allowance handsfree only
 *		BT_DPM_BT_RESTRICTED,  < bluetooth allowance restricted
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre none.
 * @see bt_dpm_set_allow_bluetooth_mode()
 */
int bt_dpm_get_allow_bluetooth_mode(bt_dpm_allow_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for device.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_activate_device_restriction(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for device.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_is_device_restriction_active(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for uuid.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_activate_uuid_restriction(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for uuid.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_is_uuid_restriction_active(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Adds the device to blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	device_address - Device address
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_add_devices_to_blacklist(const char *device_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Adds the device to whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	device_address - Device address
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_add_devices_to_whitelist(const char *device_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Adds the uuids to blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	service_uuids - profile or custom service uuids
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_add_uuids_to_blacklist(const char *service_uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Adds the uuid to whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	service_uuids - profile or custom service uuids
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_add_uuids_to_whitelist(const char *service_uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Clears the device from blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	none
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_clear_devices_from_blacklist(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Clears the device from whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	none
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_clear_devices_from_whitelist(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Clears the uuids from blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	none
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_clear_uuids_from_blacklist(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Clears the uuids from whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	none
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_clear_uuids_from_whitelist(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the devices from blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out] device_list - list of devices
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_devices_from_blacklist(bt_dpm_device_list_s **device_list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the devices from whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out] device_list - list of devices
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_devices_from_whitelist(bt_dpm_device_list_s **device_list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the uuids from blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out] uuid_list - list of uuids
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_uuids_from_blacklist(bt_dpm_uuids_list_s **uuid_list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the uuids from whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out] uuid_list - list of uuids
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_uuids_from_whitelist(bt_dpm_uuids_list_s **uuid_list);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Removes the devices from whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	device_address - Device address
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_remove_device_from_whitelist(const char *device_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Removes the devices from blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	device_address - Device address
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_remove_device_from_blacklist(const char *device_address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Removes the uuids from whitelist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	service_uuids - profile or custom service uuids
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_remove_uuid_from_whitelist(const char *service_uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Removes the uuids from blacklist.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	service_uuids - profile or custom service uuids
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_remove_uuid_from_blacklist(const char *service_uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for  outgoing call.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_allow_outgoing_call(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for  outgoing call.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_allow_outgoing_call(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for pairing.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_pairing_state(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for pairing.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_pairing_state(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for profiles.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]profile - Profile.
 *		BT_DPM_POLICY_A2DP_PROFILE_STATE,
 *		BT_DPM_POLICY_AVRCP_PROFILE_STATE,
 *		BT_DPM_POLICY_BPP_PROFILE_STATE,
 *		BT_DPM_POLICY_DUN_PROFILE_STATE,
 *		BT_DPM_POLICY_FTP_PROFILE_STATE,
 *		BT_DPM_POLICY_HFP_PROFILE_STATE,
 *		BT_DPM_POLICY_HSP_PROFILE_STATE,
 *		BT_DPM_POLICY_PBAP_PROFILE_STATE,
 *		BT_DPM_POLICY_SAP_PROFILE_STATE,
 *		BT_DPM_POLICY_SPP_PROFILE_STATE,
 *		BT_DPM_PROFILE_NONE
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_profile_state(bt_dpm_profile_e profile, bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for profiles.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]profile - Profile.
 *		BT_DPM_POLICY_A2DP_PROFILE_STATE,
 *		BT_DPM_POLICY_AVRCP_PROFILE_STATE,
 *		BT_DPM_POLICY_BPP_PROFILE_STATE,
 *		BT_DPM_POLICY_DUN_PROFILE_STATE,
 *		BT_DPM_POLICY_FTP_PROFILE_STATE,
 *		BT_DPM_POLICY_HFP_PROFILE_STATE,
 *		BT_DPM_POLICY_HSP_PROFILE_STATE,
 *		BT_DPM_POLICY_PBAP_PROFILE_STATE,
 *		BT_DPM_POLICY_SAP_PROFILE_STATE,
 *		BT_DPM_POLICY_SPP_PROFILE_STATE,
 *		BT_DPM_PROFILE_NONE
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_profile_state(bt_dpm_profile_e profile, bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for desktop connectivity.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_desktop_connectivity_state(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for desktop connectivity.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_desktop_connectivity_state(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for discoverable mode.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_discoverable_state(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for discoverable mode.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_discoverable_state(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for limited discoverable mode.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */

int bt_dpm_set_limited_discoverable_state(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for limited discoverable mode.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_limited_discoverable_state(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Sets the Restriction for Data transfer.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_set_data_transfer_state(bt_dpm_status_e value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
 * @brief Reads the Restriction for Data transfer.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[out]	value - State value.
 *		BT_DPM_ALLOWED		= 0,	< DPM Policy status allowed.
 *		BT_DPM_RESTRICTED		= 1,	< DPM Policy status restricted.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	invalid paramter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not suported
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED operation failed
 *
 * @pre bt_dpm_set_allow_bluetooth_mode must be allowed.
 * @see bt_dpm_get_allow_bluetooth_mode()
 */
int bt_dpm_get_data_transfer_state(bt_dpm_status_e *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE
 * @brief Gets the pbap connected status.
 * @since_tizen 3.0
 * @param[in] remote_address The address of the remote device
 * @param[out] connected_status the connected status
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED	Remote device not bonded
 * @retval #BT_ERROR_OPERATION_FAILED	Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_pbap_client_is_connected(const char *address, bool *connected_status);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 * @brief Sends the custom event data.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] remote_address device address of remote device.
 * @param[in] report_id  reoport id need to be passed to remote device
 * @param[in] data  The data need to be passed to remote device
 * @param[in] data_len  The length of the data
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 *
 * @pre The HID connection must be established.
 * @see bt_hid_device_connection_state_changed_cb()
 */
int bt_hid_device_send_custom_event(const char *remote_address,
		unsigned char report_id, const char *data, unsigned int data_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE
 * @brief  Sends the volume change request to the remote device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] value The new volume level
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_set_absolute_volume(unsigned int value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE
 * @brief  Sends the volume up request to the remote device.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_control_connect()
 * @see bt_avrcp_control_initialize()
 */
int bt_avrcp_control_increase_volume(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE
 * @brief  Sends the volume down request to the remote device.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_control_connect()
 * @see bt_avrcp_control_initialize()
 */
int bt_avrcp_control_decrease_volume(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE
 * @brief  Sends the delay report to the remote device.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] value The new delay level
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_send_delay_report(unsigned int value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the delay is changed.
 * @since_tizen 5.0
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The AVRCP service must be initialized with bt_avrcp_target_initialize() or bt_avrcp_control_initialize().
 * @see bt_avrcp_target_initialize()
 * @see bt_avrcp_control_initialize()
 */
int bt_avrcp_set_delay_changed_cb(bt_avrcp_delay_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the delay is changed.
 * @since_tizen 5.0
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The AVRCP service must be initialized with bt_avrcp_target_initialize() or bt_avrcp_control_initialize().
 * @see bt_avrcp_target_initialize()
 * @see bt_avrcp_control_initialize()
 * @see bt_avrcp_set_delay_changed_cb()
 */
int bt_avrcp_unset_delay_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Creates the Proximity Profile Reporter Role.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] None
 * @param[out] reporter proximity reporter server handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_ALREADY_DONE Already Enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY out Of Memory
 */
int bt_proximity_reporter_create(bt_proximity_reporter_h *reporter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Destroys the Proximity Profile Reporter Role.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] reporter proximity reporter server handle
 * @return @c 0 on success,
 *		   otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED Not initialized
 */
int bt_proximity_reporter_destroy(bt_proximity_reporter_h reporter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Creates the Proximity Profile Monitor Role.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote address
 * @param[out] monitor proximity monitor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_ALREADY_DONE Already Enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY out Of Memory
 */
int bt_proximity_monitor_create(const char *remote_address, bt_proximity_monitor_h *monitor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Creates the Proximity Profile Monitor Role.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 */
int bt_proximity_monitor_destroy(bt_proximity_monitor_h monitor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Proximity Monitor initiates the connection to remote reporter device.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED Not initialized
 */
int bt_proximity_monitor_connect(bt_proximity_monitor_h monitor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Proximity Monitor disconnects with the remote reporter device.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] proximity monitor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED Not initialized
 */
int bt_proximity_monitor_disconnect(bt_proximity_monitor_h monitor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Sets proximity monitor connection state changed event change callback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] proximity monitor handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 */
int bt_proximity_monitor_set_connection_state_changed_cb(bt_proximity_monitor_h monitor,
				bt_proximity_monitor_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Unsets proximity monitor connection state changed event change callback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] proximity monitor handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 */
int bt_proximity_monitor_unset_connection_state_changed_cb(bt_proximity_monitor_h monitor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Sets proximity reporter connection state changed event change callback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] reporter proximity reporter handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 */
int bt_proximity_reporter_set_connection_state_changed_cb(bt_proximity_reporter_h reporter,
				bt_proximity_reporter_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Unsets proximity reporter connection state changed event change callback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] proximity reporter handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 */
int bt_proximity_reporter_unset_connection_state_changed_cb(bt_proximity_reporter_h reporter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Sets proximity reporter properties change callback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] reporter proximity reporter handle
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 */
int bt_proximity_reporter_set_property_changed_cb(bt_proximity_reporter_h reporter,
				bt_proximity_reporter_property_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Unsets proximity reporter properties change callback.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] reporter proximity reporter handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 */
int bt_proximity_reporter_unset_property_changed_cb(bt_proximity_reporter_h reporter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Set the Proximity Profile monitor linkloss alert value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @param[in] value The value to be set for link loss alert
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_monitor_set_linkloss_alert(bt_proximity_monitor_h monitor, int value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Set the Proximity Profile monitor immediate alert value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @param[in] value The value to be set for immediate alert
 * @return @c 0 on success,
 *		   otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_monitor_set_immediate_alert(bt_proximity_monitor_h monitor, int value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Read the Proximity Profile monitor linkloss alert value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @param[out] value value read for the linkloss alert.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_monitor_get_linkloss_alert(bt_proximity_monitor_h monitor, int *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Read the Proximity Profile monitor immediate alert value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @param[out] value value read for the immedaite alert.
 * @return @c 0 on success,
 *		   otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_monitor_get_immediate_alert(bt_proximity_monitor_h monitor, int *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Read the Proximity Profile monitor Signal level value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] monitor proximity monitor handle
 * @param[out] value value read for the signal level.
 * @return @c 0 on success,
 *		   otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED Device not Connected
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_monitor_get_signal_level(bt_proximity_monitor_h monitor, int *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Read the Proximity Profile reporter linkloss alert value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address remote device reporter address
 * @param[out] value value read for the linkloss alert.
 * @return @c 0 on success,
 *		   otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_reporter_get_linkloss_alert(const char *remote_address, int *value);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Read the Proximity Profile reporter immediate alert value.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address remote device reporter address
 * @param[out] value value read for the immediate alert.
 * @return @c 0 on success,
 *		   otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 */
int bt_proximity_reporter_get_immediate_alert(const char *remote_address, int *value);

/* This internal API will be removed */
/**
 * @internal
 * @deprecated Deprecated since 4.0. Use bt_adapter_le_scan_filter_unregister instead.
 * @brief API to Unset the Proximity uuids scan report
 */
int bt_adapter_le_scan_filter_unset_proximity_uuid(bt_scan_filter_h scan_filter) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @brief API to register TDS provider.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] None
 * @param[out] None
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_unregister()
 */
int bt_tds_provider_register(void);

/**
 * @internal
 * @brief API to unregister TDS provider.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] None
 * @param[out] None
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre TDS provider must be registered with bt_tds_provider_register().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_register()
 */
int bt_tds_provider_unregister(void);

/**
 * @internal
 * @brief API to create TDS provider.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] transport Transport to be discovered over BLE.
 * @param[out] provider The handle associated with the newly created provider.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre TDS provider must be registered with bt_tds_provider_register().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_register()
 * @see bt_tds_provider_unregister()
 * @see bt_tds_provider_destroy()
 */
int bt_tds_provider_create(bt_tds_provider_h *provider, bt_tds_transport_e transport);

/**
 * @internal
 * @brief API to destroy TDS provider.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] provider The handle associated with the provider
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre TDS provider must be registered with bt_tds_provider_register().
 * @pre TDS provider must be created with bt_tds_provider_create().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_register()
 * @see bt_tds_provider_create()
 */
int bt_tds_provider_destroy(bt_tds_provider_h provider);

/**
 * @internal
 * @brief API to set transport block data for transport provider.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] provider The handle associated with the provider
 * @param[in] transport_state State of transport
 * @param[in] buf transport block data
 * @param[in] length transport block data length
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre TDS provider must be registered with bt_tds_provider_register().
 * @pre TDS provider must be created with bt_tds_provider_create().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_register()
 * @see bt_tds_provider_create()
 */
int bt_tds_provider_set_transport_data(bt_tds_provider_h provider,
		bt_tds_transport_state_e transport_state, unsigned char *buf, int length);

/**
 * @internal
 * @brief API to set TDS provider manufacturer specific data.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] buf manufacturer specific data
 * @param[in] length manufacturer data length
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre TDS provider must be registered with bt_tds_provider_register().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_register()
 * @see bt_tds_provider_create()
 */
int bt_tds_provider_set_manufacturer_data(unsigned char *buf, int length);

/**
 * @internal
 * @brief API to send activation response to TDS seeker.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] address Remote device bluetooth address
 * @param[in] result Result of activation request
 *		BT_ERROR_NONE: success
 *		BT_ERROR_INVALID_PARAMETER: Invalid data in activation request
 *		BT_ERROR_OPERATION_FAILED: Operation failed
 * @param[in] provider The handle associated with the provider
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @pre TDS provider must be registered with bt_tds_provider_register().
 *
 * @see bt_initialize()
 * @see bt_tds_provider_register()
 * @see bt_tds_activation_requested_cb()
 * @see bt_tds_set_transport_activation_requested_cb()
 * @see bt_tds_unset_transport_activation_requested_cb()
 */
int bt_tds_provider_send_activation_resp(char *address, int result, bt_tds_provider_h provider);

/**
 * @internal
 * @brief API to set activation requeste callback.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] callback  The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_tds_activation_requested_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_tds_activation_requested_cb()
 * @see bt_tds_unset_transport_activation_requested_cb()
 */
int bt_tds_set_transport_activation_requested_cb(
		bt_tds_activation_requested_cb callback, void *user_data);

/**
 * @internal
 * @brief API to unset activation requeste callback.
 * @since_tizen @if WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] None
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_SUPPORTED Not supported
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 *
 * @see bt_initialize()
 * @see bt_tds_set_transport_activation_requested_cb()
 */
int bt_tds_unset_transport_activation_requested_cb(void);

/**
 * @internal
 * @brief TDS Seeker Profile API to start seeking Remote TDS providers only
 */
int bt_tds_start_seeking_providers(bt_tds_provider_scan_result_cb cb, void *user_data);

/**
 * @internal
 * @brief TDS Seeker Profile API to stop seeking Remote TDS providers
 */
int bt_tds_stop_seeking_providers(void);

/**
 * @internal
 * @brief TDS Seeker Profile API to create Seeker Role associated with Remote TDS provider
 */
int bt_tds_seeker_create(const char *remote_address, bt_tds_seeker_h *seeker);

/**
 * @internal
 * @brief TDS Seeker Profile API to destroy Seeker Role associated with Remote TDS provider
 */
int bt_tds_seeker_destroy(bt_tds_seeker_h seeker);

/**
 * @internal
 * @brief TDS Seeker Profile API to set connection state changed event associated with Remote TDS provider
 */
int bt_tds_seeker_set_connection_state_changed_cb(bt_tds_seeker_h seeker,
		bt_tds_seeker_connection_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @brief TDS Seeker Profile API to unset connection state changed event associated with Remote TDS provider
 */
int bt_tds_seeker_unset_connection_state_changed_cb(bt_tds_seeker_h seeker);

/**
 * @internal
 * @brief TDS Seeker Profile API to connect to Remote TDS provider
 */
int bt_tds_seeker_connect(bt_tds_seeker_h seeker);

/**
 * @internal
 * @brief TDS Seeker Profile API to disconnect to Remote TDS provider
 */
int bt_tds_seeker_disconnect(bt_tds_seeker_h seeker);

/**
 * @internal
 * @brief TDS Seeker Profile API to read complete TDS block data which is stored in Remote TDS provider's GATT database
 */
int bt_tds_seeker_get_complete_transport_blocks(bt_tds_seeker_h seeker,
		bt_tds_seeker_complete_transport_data_cb callback, void *user_data);

/**
 * @internal
 * @brief TDS Seeker Profile API to Activate ALternate transport supported by Remote TDS provider
 */
int bt_tds_seeker_activate_control_point(bt_tds_seeker_h seeker,
				bt_tds_transport_e transport, unsigned char *buffer, int len,
				bt_tds_control_point_activation_indication_cb callback, void *user_data);

/**
 * @ingroup
 * @brief Starts OTP server on the given directory location.
 */
int bt_otp_server_initialize(const char *directory);

/**
 * @ingroup
 * @brief Stops OTP server.
 */
int bt_otp_server_deinitialize(void);

/**
 * @ingroup
 * @brief Registers a callback function that will be invoked when the server state is changed.
 */
int bt_otp_set_server_state_changed_cb(bt_otp_server_state_changed_cb callback, void *user_data);

/**
 * @ingroup
 * @brief Registers a callback function that will be invoked when the server state is changed.
 */
int bt_otp_unset_server_state_changed_cb(void);

/**
 * @internal
 * @brief OTP Client Profile API to create Client Role associated with Remote OTP Server
 */
int bt_otp_client_create(const char *remote_address, bt_otp_client_h *otp_client);

/**
 * @internal
 * @brief OTP Client Profile API to destroy Client Role associated with Remote OTP Server
 */
int bt_otp_client_destroy(bt_otp_client_h otp_client);

/**
 * @internal
 * @brief OTP Client Profile API to set connection state changed event associated with Remote OTP Server
 */
int bt_otp_client_set_connection_state_changed_cb(bt_otp_client_h otp_client,
						bt_otp_client_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @brief OTP Client Profile API to unset connection state changed event associated with Remote OTP Server
 */
int bt_otp_client_unset_connection_state_changed_cb(bt_otp_client_h otp_client);

/**
 * @internal
 * @brief OTP client API to connect to Remote OTP Server
 */
int bt_otp_client_connect(bt_otp_client_h otp_client);

/**
 * @internal
 * @brief OTP client API to disconnect to Remote OTP Server
 */
int bt_otp_client_disconnect(bt_otp_client_h otp_client);

/**
 * @internal
 * @brief OTP client API to discover all objects in Remote OTP Server
 */
int bt_otp_client_discover_all_objects(bt_otp_client_h otp_client,
						bt_otp_client_object_discovery_cb callback, void *user_data);

/**
 * @internal
 * @brief OTP client API to select object in Remote OTP Server
 */
int bt_otp_client_select_object(bt_otp_client_h otp_client, unsigned long long id,
							bt_otp_client_object_select_cb callback, void *user_data);
/**
 * @internal
 * @brief OTP client API to read remote objects contents
 */
int bt_otp_client_read_object_contents(bt_otp_client_h otp_client,
					bt_otp_client_read_object_complete_cb callback, void *user_data);
/**
 * @internal
 * @brief OTP client API to create object on remote server
 */
int bt_otp_client_create_object(bt_otp_client_h otp_client,
							const char *file_path,
							bt_otp_client_object_create_cb callback,
							void *user_data);
/**
 * @internal
 * @brief OTP client API to write object contents on remote server
 */
int bt_otp_client_write_object(bt_otp_client_h otp_client,
							const char *file_path,
							int offset, int length, int mode,
							bt_otp_client_object_write_cb callback,
							void *user_data);
/**
 * @internal
 * @brief OTP client API to execute object on remote server
 */
int bt_otp_client_execute_object(bt_otp_client_h otp_client,
							bt_otp_client_object_execute_cb callback,
							void *user_data);
/**
 * @internal
 * @brief OTP client API to delete object on remote server
 */
int bt_otp_client_delete_object(bt_otp_client_h otp_client,
							bt_otp_client_object_delete_cb callback,
							void *user_data);

//======================================================================================

/**
 * @internal
 * @brief HRP Sensor API to set initial heart rate value , will also notify to all connected clients if notification is enabled .
 */
int bt_hrp_sensor_set_heartrate_value(int heartrate);

/**
 * @internal
 * @brief HRP Sensor API to set initial Location value , before creating sensor.
 */
int bt_hrp_sensor_set_location_value(bt_body_sensor_location_e location);

/**
 * @internal
 * @brief HRP Sensor API to set initial energy value (Value Range : 0-9) , before creating sensor.
 */
int bt_hrp_sensor_set_energy_value(int energy_value);

/**
 * @internal
 * @brief HRP Sensor API to set initial rr interval value (Value range : 0-8 If EE is supported,
 * 0-9 if EE not supported) before creating sensor.
 */
int bt_hrp_sensor_set_rr_interval(int interval);

/**
 * @internal
 * @brief HRP Sensor API to set initial contact value , wearable in contact with
 * user or user has removed it, before creating sensor.
 */
int bt_hrp_sensor_set_contact_value(bool enable);

/**
 * @internal
 * @brief HRP Sensor API to set manufacturer name , before creating sensor.
 */
int bt_hrp_sensor_set_device_manufacturer_name(char *manuf_name);

/**
 * @internal
 * @brief HRP Sensor API for creating sensor and services.
 */

int bt_hrp_sensor_create(void);

/**
 * @internal
 * @brief HRP Sensor API to destory heart rate sensor .
 */

int bt_hrp_sensor_destroy(void);

/**
 * @internal
 * @brief HRP collector API to create collector .
 */

int bt_hrp_collector_create(const char *remote_address,
				bt_hrp_collector_h *collector);

/**
 * @internal
 * @brief HRP collector API to destory collector.
 */
int bt_hrp_collector_destory(bt_hrp_collector_h collector);

/**
 * @internal
 * @brief HRP collector API to start scan .
 */
int bt_hrp_collector_start_scan(bt_hrp_collector_scan_result_cb cb, void *user_data);

/**
 * @internal
 * @brief HRP collector API to stop scan.
 */
int bt_hrp_collector_stop_scan(void);

/**
 * @internal
 * @brief HRP collector API to connect to remote sensor .
 */
int bt_hrp_collector_connect(bt_hrp_collector_h collector);

/**
 * @internal
 * @brief HRP collector API to disconnect to remote sensor .
 */
int bt_hrp_collector_disconnect(bt_hrp_collector_h collector);

/**
 * @internal
 * @brief HRP collector API to set notification on / off.
 */
int bt_hrp_collector_set_notification(bt_hrp_collector_h collector,
			bt_hrp_collector_heart_rate_value_changed_cb callback,
			bool isNotify);

/**
 * @internal
 * @brief HRP collector API to read Body Sensor location , fills body sensor location in bsl_location
 */

int bt_hrp_collector_get_body_sensor_location(bt_hrp_collector_h collector,
			bt_hrp_collector_bsl_read_completed_cb bsl_read_cb);


/**
 * @internal
 * @brief HRP collector API to set connection state callback
 */

int bt_hrp_collector_set_connection_state_changed_cb(bt_hrp_collector_h collector,
			bt_hrp_collector_connection_state_changed_cb callback,
			void *user_data);

/**
 * @internal
 * @brief HRP collector API to unset connection state callback
 */

int bt_hrp_collector_unset_connection_state_changed_cb(bt_hrp_collector_h collector);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_NETWORK_BLUETOOTH_INTERNAL_H__ */
