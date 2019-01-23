/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _THINGS_API_H_
#define _THINGS_API_H_

#include <stdint.h>
#include "things_types.h"
#include "things_resource.h"

#ifdef __cplusplus
extern "C" {
#endif							// __cplusplus

//--------------------------------------------------------------------------------
/**
 * @brief callback for handling result of reset.
 * @param is success result of reset.(1 : success, = : failure)
 */
typedef void (*things_reset_result_func_type)(int result);

//--------------------------------------------------------------------------------
/**
 * @brief callback for informing the st thing to close the PIN display.
 */
typedef void (*things_pin_display_close_func_type)(void);

//--------------------------------------------------------------------------------
/**
 * @brief callback for carrying the randomly generated PIN info.
 * @param pin_data : PIN data in string format.
 * @param pin_size : Length of the PIN string.
 */
typedef void (*things_pin_generated_func_type)(char *pin_data, size_t pin_size);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for getting the easysetup states.
 * @param state : hold the enrollee state.
 */
typedef void (*things_get_easysetup_state_func_type)(things_es_enrollee_state_e state);

//--------------------------------------------------------------------------------
/**
 * @brief callback for connection result of cloud.It is called when finish Cloud-Connection.
 * @param result : 1 = published resource to cloud , 0 =failed to publish resource
 *			to cloud.
 * @param domain : cloud domain name.
 * @param access_token : Contains information of cloud.
 */
typedef void (*things_cloud_con_result_func_type)(int result, char *domain, char *access_token);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for providing ownership transfer method change state.
 * @param addr : address
 * @param port : port number
 * @param uuid : uuid
 * @param event : Holds value of oic sec otm event.
 */
typedef void (*things_sec_otm_state_func_type)(const char *addr, uint16_t port, const char *uuid, int event);

//--------------------------------------------------------------------------------
typedef int (*things_stop_soft_ap_func_type)(int value);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for getting user's input confirmation for ownership transfer.
 * @return : 1 in case of confirmed, otherwise return 0.
 */
typedef int (*things_get_user_confirm_func_type)(void);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for handling request message.
 * @param p_resource : Instance of target resource.
 * @return : success = 1, failure = 0.
 */
typedef int (*things_handle_request_func_type)(struct things_resource_s *p_resource);

//--------------------------------------------------------------------------------
typedef int (*things_update_dev_prov_data_func_type)(es_dev_conf_prov_data_s *p_dev_prov_data);

//--------------------------------------------------------------------------------
/**
 * @brief callback function to get the request for user's confirmation for reset.
 * @param func_carrier : callback for handling result of reset.
 * @param reset_type   : Type of reset.It will be NEED confirmation for reset or auto reset.
 * @return : success = 1, failure = 0.
 */
typedef int (*things_reset_confirm_func_type)(things_reset_result_func_type *func_carrier, things_es_enrollee_reset_e reset_type);

//--------------------------------------------------------------------------------
/**
 * @brief Initialize the things stack.
 * @param json_path	: Path to Json file which defines a thing. Definition includes
 *				the device information,resources and their properties,
 *				 configuration info for connectivity and easy-setup, etc.
 * @easysetup_completed : Indicates whether easysetup is completed already or not.
 * @return		: 1 on success, otherwise 0 value.
 */
int things_initialize_stack(const char *json_path, bool *easysetup_completed);

//--------------------------------------------------------------------------------
/**
 * @brief Deinitialize the things stack.It delete all the device information.
 * @return	: 1 on success.
 */
int things_deinitialize_stack(void);

//--------------------------------------------------------------------------------
/**
 * @brief Starts things stack.Stack should have been initialized before calling this API.
 * Check the user authentication method,wheather it is PIN based, USER confirmation based
 * or both type.Parses the thing definition, configures the thing, creates the resources
 * and prepares it for easy-setup.For hosting device,first it register the device-id and
 * resource.
 * @return	: 1 on success, otherwise 0 value.
 */
int things_start_stack(void);

//--------------------------------------------------------------------------------
/**
 * @brief api for stop things stack.It terminate cloud connection, easysetup, release
 * the server builder and deinitialize the module callback.
 * @return      : 1 on success.
 */
int things_stop_stack(void);

//--------------------------------------------------------------------------------
/**
 * @brief Notify the observers of a specific resource.
 *        Stack should have been initialized and started before calling this API.
 * @param uri	: URI of the resource which will be notified to observer.
 * @return      : 1 on success, otherwise 0 value.
 */
int things_notify_observers(const char *uri);

//--------------------------------------------------------------------------------
/**
 * @brief Reset all the data related to security, easy setup provisioning disable
 *	and cloud being used in the stack.
 * @param remote_owner
 * @things_es_enrollee_reset_e	: It is type of reset. It will be NEED confirmation
 *	for reset or auto reset.
 * @return	: 1 on success otherwise 0 or negative value.
 */
int things_reset(void *remote_owner, things_es_enrollee_reset_e reset_type);

//--------------------------------------------------------------------------------
/**
 * @brief api for register callback for reset confirmation to start.
 * @param things_reset_confirm_func_type	: callback function to get the request
 *		for user's confirmation for reset.
 * @return	1 on success otherwise 0 value.
 */
int things_register_confirm_reset_start_func(things_reset_confirm_func_type func);

//--------------------------------------------------------------------------------
/**
 * @brief callback to get user's confirmation to reset and pass the result to things stack.
 * @param reset	: Depends on user's confirmation for reset.( 1 = user confirmed,
			0 = user not confirmed).
 * @return result : 1 on success otherwise 0 value.
 */
int things_return_user_opinion_for_reset(int reset);

//--------------------------------------------------------------------------------
/**
 * @brief Callback registration function for handling request message.
 * @param get_func	: Reference of the callback function to handle GET request.
 * @param set_func	: Reference of the callback function to handle SET(POST) request.
 * @return	: 1 on success otherwise 0 value.
 */
int things_register_handle_request_func(things_handle_request_func_type get_func, things_handle_request_func_type set_func);

//--------------------------------------------------------------------------------
/**
 * @brief register callback for the pin generation for ownership transfer.
 * @param things_pin_generated_func_type	: callback function for generate
		pin for random pin based ownership transfer.
 * @return	: 1 on success otherwise 0 value.
 */
int things_register_pin_generated_func(things_pin_generated_func_type func);

//--------------------------------------------------------------------------------
/**
 * @brief register callback for the close pin display.
 * @param things_pin_display_close_func_type : callback function for close pin
	display.
 * @return      : 1 on success otherwise 0 value.
 */
int things_register_pin_display_close_func(things_pin_display_close_func_type func);

//--------------------------------------------------------------------------------
/**
 * @brief register callback function for getting confirmation from user during
 * ownership transfer.
 * @param things_get_user_confirm_func_type	: callback function for getting user's
 *			input confirmation for ownership transfer.
 * @return	: 1 on success otherwise 0 value.
 */
int things_register_user_confirm_func(things_get_user_confirm_func_type func);

//--------------------------------------------------------------------------------
/**
 * @brief register callback for check the status of easysetup.
 * @param things_get_easysetup_state_func_type	: callback function for getting the
 *			easysetup states.
 * @return	: 1 on success otherwise 0 value.
 */
int things_register_easysetup_state_func(things_get_easysetup_state_func_type func);

//--------------------------------------------------------------------------------
/**
 * @brief register callback for the security ownership tranfer method state changes.
 * @param things_sec_otm_state_func_type otm	: callback function for providing
 *			ownership transfer method change state.
 * @return	: 1 on success otherwise 0 value.
 */
int things_register_otm_event_handler(things_sec_otm_state_func_type otm_event_callback);

//--------------------------------------------------------------------------------
/**
 * @brief things get the resource properties based on resource uri.
 * @param resource_uri	: URI of the resource.
 * @param count		: Number of resource types for a particular resource.
 * @char ***resource_types : reference for storing resource type.
 * @return	: 1 on success otherwise 0 value.
 */
int things_get_resource_type(const char *resource_uri, int *count, char ***resource_types);

//--------------------------------------------------------------------------------
/**
 * @brief things get the resource properties based on resource type.
 * @param res_type  : resource type.
 * @param count         : Number of resource types for a particular resource.
 * @things_attribute_info_s ***attributes : reference for storing things properties.
 * @return      : 1 on success otherwise 0 value.
 */
int things_get_attributes_by_resource_type(const char *res_type, int *count, things_attribute_info_s ***attributes);

//--------------------------------------------------------------------------------
/**
 * @brief callback for getting the child resource for collection resource.
 * @param col_res_uri	: URI of the collection resource.
 * @param count		: Number of children
 * @child_resurces	: Refernece for storing child resource.
 */
int things_get_child_resources(const char *col_res_uri, int *count, things_resource_info_s ***child_resurces);

//--------------------------------------------------------------------------------
/**
 * @brief function prototype for checking things module initialized or not.
 * @return	: 1 on initialized otherwise 0 value.
 */
int things_is_things_module_initialized(void);

//--------------------------------------------------------------------------------
#ifdef CONFIG_ST_THINGS_COLLECTION
/**
 *@brief function prototype for checking the things is collection resource or not.
 *@param res_uri	: URI of the resource.
 *@return	: true = collection resource, false = Not collection resource.
 */
bool things_is_collection_resource(const char *res_uri);
#endif

#ifdef __cplusplus
}
#endif							// __cplusplus
#endif							// _THINGS_API_H_
