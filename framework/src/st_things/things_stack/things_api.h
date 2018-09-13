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

typedef void (*things_reset_result_func_type)(int result);
typedef void (*things_pin_display_close_func_type)(void);
typedef void (*things_pin_generated_func_type)(char *pin_data, size_t pin_size);
typedef void (*things_get_easysetup_state_func_type)(things_es_enrollee_state_e state);
typedef void (*things_cloud_con_result_func_type)(int result, char *domain, char *access_token);
typedef void (*things_abort_easysetup_func_type)(things_es_enrollee_abort_e level);
typedef void (*things_sec_otm_state_func_type)(const char *addr, uint16_t port, const char *uuid, int event);
typedef int (*things_stop_soft_ap_func_type)(int value);
typedef int (*things_get_user_confirm_func_type)(void);
typedef int (*things_handle_request_func_type)(struct things_resource_s *p_resource);
typedef int (*things_update_dev_prov_data_func_type)(es_dev_conf_prov_data_s *p_dev_prov_data);
typedef int (*things_reset_confirm_func_type)(things_reset_result_func_type *func_carrier, things_es_enrollee_reset_e reset_type);

int things_initialize_stack(const char *json_path, bool *easysetup_completed);
int things_deinitialize_stack(void);
int things_start_stack(void);
int things_stop_stack(void);
int things_notify_observers(const char *uri);
int things_reset(void *remote_owner, things_es_enrollee_reset_e reset_type);

int things_register_confirm_reset_start_func(things_reset_confirm_func_type func);
int things_return_user_opinion_for_reset(int b_reset_start);
int things_register_handle_request_func(things_handle_request_func_type get_func, things_handle_request_func_type set_func);
int things_register_pin_generated_func(things_pin_generated_func_type func);
int things_register_pin_display_close_func(things_pin_display_close_func_type func);
int things_register_user_confirm_func(things_get_user_confirm_func_type func);
int things_register_easysetup_state_func(things_get_easysetup_state_func_type func);
int things_register_otm_event_handler(things_sec_otm_state_func_type otm_event_callback);
int things_get_resource_type(const char *resource_uri, int *count, char ***resource_types);
int things_get_attributes_by_resource_type(const char *res_type, int *count, things_attribute_info_s ***attributes);
int things_get_child_resources(const char *col_res_uri, int *count, things_resource_info_s ***child_resurces);
int things_is_things_module_initialized(void);
#ifdef CONFIG_ST_THINGS_COLLECTION
bool things_is_collection_resource(const char *res_uri);
#endif

#ifdef __cplusplus
}
#endif							// __cplusplus
#endif							// _THINGS_API_H_
