/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __ST_THINGS_H__
#define __ST_THINGS_H__

#include <stdint.h>
#include <stdbool.h>

#include "st_things_types.h"

#ifdef __cplusplus
extern "C" {
#endif							/* __cplusplus */

/**
 * @brief Initializes things stack and returns whether easy-setup is completed or not.
 *            Easy-setup enable users to acquire the ownership of things and to connect the things with the cloud.
 *            After performing easy-setup, users can access things from anywhere through the cloud.
 *            In things stack, easy-setup is a primary and the first operation to be performed on the thing.
 *            Application running on the thing can know whether easy-setup is done already or not.
 *            If easy-setup is done, app can start the things stack by calling st_things_start().
 *            If easy-setup is not done, app can either wait for the user interaction before starting the things stack or
 *            start the things stack directly without waiting for any events(This case is for those things which doesn't
 *            support input capability and for all other unknown cases).
 * @param[in] json_path Path to Json file which defines a thing. Definition includes the device information,
 *                                       resources and their properties, configuration info for connectivity and easy-setup, etc.
 * @param[out] easysetup_complete Indicates whether easysetup is completed already or not.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_initialize(const char *json_path, bool *easysetup_complete);

/**
 * @brief Deinitializes things stack.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_deinitialize(void);

/**
 * @brief Callback for handling GET request.
 * @param[in]  req_msg GET request message.
 * @param[out] resp_rep Representation that will be set to payload of response.
 * @return @c true in case of success, otherwise @c false
 * @since Tizen RT v1.1
 */
typedef bool (*st_things_get_request_cb)(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);

/**
 * @brief Callback for handling SET(POST) request.
 * @param[in]  req_msg SET request message.
 * @param[out] resp_rep Representation that will be set to payload of response.
 * @return @c true in case of success, otherwise @c false
 * @since Tizen RT v1.1
 */
typedef bool (*st_things_set_request_cb)(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

/**
 * @brief Callback registration function for handling request messages.
 * @details The callbacks ensure that a request message will be carried with one of the resource uris from json file of st_things_start().
 * @remarks Only one callback function can be set with this API.\n
 *          If multiple callbacks are set, the last one is registered only.\n
 *          And the callbacks are called in the internal thread, which is not detached,\n
 *          so application should return it to get the next callbacks.
 * @param[in] get_cb Reference of the callback function to handle GET request.
 * @param[in] set_cb Reference of the callback function to handle SET(POST) request.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_register_request_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb);

/**
 * @brief Starts things stack.
 *            Parses the thing definition(whose path is passed to st_things_initialize(), configures the thing,
 *            creates the resources and prepares it for easy-setup.
 *            If easy-setup is not done yet, onboarding will be started using either SoftAP or BLE connection.
 *                Onboarding creates an ad-hoc network between the thing and the client for performing easy-setup.
 *            If easy-setup is already done, thing will be connected with the cloud.
 *            Application can know whether easy-setup is done or not through st_things_initialize API.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_start(void);

/**
 * @brief Stops things stack.
 *            Removes all the data being used internally and releases all the memory allocated for the stack.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_stop(void);

/**
 * @brief Callback for getting user's opinion regarding device reset.
 * @return @c true to confirm, otherwise @c to deny
 * @since Tizen RT v1.1
 */
typedef bool (*st_things_reset_confirm_cb)(void);

/**
 * @brief Callback for carrying the result of reset.
 * @param[in] is_success Result of Stack-reset. (true : success, false : failure)
 * @since Tizen RT v1.1
 */
typedef void (*st_things_reset_result_cb)(bool is_success);

/**
 * @brief Callback registration function for Reset-Confirmation and Reset-Result functions.
 * @remarks Only one callback function can be set with this API.\n
 *          If multiple callbacks are set, the last one is registered only.\n
 *          And the callbacks are called in the internal thread, which is not detached,\n
 *          so application should return it to get the next callbacks.
 * @param[in] confirm_cb Callback function that will be called to get the user's input when reset is triggered.
 * @param[in] result_cb Callback function that will be called after the reset process is done.
 *                      This parameter can be NULL if notification for result of reset is not needed.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_register_reset_cb(st_things_reset_confirm_cb confirm_cb, st_things_reset_result_cb result_cb);

/**
 * @brief Reset all the data related to security and cloud being used in the stack.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_reset(void);

/**
 * @brief Callback for carrying the randomly generated PIN info.
 * @details Device should show the PIN on display.
 * @param[in] pin_data PIN data in string format.
 * @param[in] pin_size Length of the PIN String.
 * @since Tizen RT v1.1
 */
typedef void (*st_things_pin_generated_cb)(const char *pin_data, const size_t pin_size);

/**
 * @brief Callback for informing the application to close the PIN display.
 * @since Tizen RT v1.1
 */
typedef void (*st_things_pin_display_close_cb)(void);

/**
 * @brief Callback registration function for getting randomly generated PIN for the PIN-Based Ownership Transfer Request.
 * @remarks Only one callback function can be set with this API.\n
 *          If multiple callbacks are set, the last one is registered only.\n
 *          And the callbacks are called in the internal thread, which is not detached,\n
 *          so application should return it to get the next callbacks.
 * @param[in] generated_cb Callback function that will be called when device receives a Ownership Transfer request from client.
 * @param[in] close_cb Callback function that will be called when Ownership Transfer is done so device can stop showing PIN on display.
 *                     This parameter can be NULL if stop triggering is not needed.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_register_pin_handling_cb(st_things_pin_generated_cb generated_cb, st_things_pin_display_close_cb close_cb);

/**
 * @brief Callback for getting user's input regarding mutual verification.
 * @return @c true true in cse of confirmed, otherwise @c false
 * @since Tizen RT v1.1
 */
typedef bool (*st_things_user_confirm_cb)(void);

/**
 * @brief Callback registration function for getting user confirmation for MUTUAL VERIFICATION BASED JUST WORK Ownership transfer.
 * @remarks Only one callback function can be set with this API.\n
 *          If multiple callbacks are set, the last one is registered only.\n
 *          And the callbacks are called in the internal thread, which is not detached,\n
 *          so application should return it to get the next callbacks.
 * @param[in] confirm_cb Callback function that will be called when device receives a confirm request from client.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_register_user_confirm_cb(st_things_user_confirm_cb confirm_cb);

/**
 * @brief Callback for getting the current state of ST Things.
 * @param[in]  things_status ST Things State
 * @since Tizen RT v1.1
 */
typedef void (*st_things_status_change_cb)(st_things_status_e things_status);

/**
 * @brief Callback registration function for getting notified when ST Things state changes.
 * @remarks Only one callback function can be set with this API.\n
 *          If multiple callbacks are set, the last one is registered only.\n
 *          And the callbacks are called in the internal thread, which is not detached,\n
 *          so application should return it to get the next callbacks.
 * @param[in] status_cb Refernce of the callback function to get ST Things status
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_register_things_status_change_cb(st_things_status_change_cb status_cb);

/**
 * @brief Notify the observers of a specific resource.
 * @param[in]  resource_uri Resource URI of the resource which will be notified to observers.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_notify_observers(const char *resource_uri);

/**
 * @brief Send Representation of a specific resource through push service to Samsung Connect client.
 * @param[in]  resource_uri Resource uri for which we want to send push message.
 * @param[in]  rep Representation for resource uri.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #ST_THINGS_ERROR_NONE Successful
 * @retval #ST_THINGS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #ST_THINGS_ERROR_OPERATION_FAILED Operation failed
 * @since Tizen RT v1.1
 */
int st_things_send_push_message(const char *resource_uri, st_things_representation_s *rep);

/**
 * @brief Create an instance of representation.
 * @remarks To destroy an instance, st_things_destroy_representation_inst() should be used.
 * @return a pointer of the created representation, otherwise a null pointer if the memory is insufficient.
 * @since Tizen RT v1.1
 */
st_things_representation_s *st_things_create_representation_inst(void);

/**
 * @brief Destroy an instance of representation.
 * @param[in]  rep Representation that will be destroyed.
 * @since Tizen RT v1.1
 */
void st_things_destroy_representation_inst(st_things_representation_s *rep);

#ifdef __cplusplus
}
#endif							/* __cplusplus */
#endif							/* __ST_THINGS_H__ */
