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

#define _POSIX_C_SOURCE 200809L

#ifndef _EASYSETUP_MANAGER_H_
#define _EASYSETUP_MANAGER_H_

#include "things_api.h"
#include "es_common.h"
#include "framework/things_server_builder.h"

typedef enum {
	ESM_OK = 0,
	ESM_ERROR,
	ESM_PARAM_INVAIND,
	ESM_MODULE_STARTED,
	ESM_MODULE_TERMINATED,
	ESM_REQ_DENIED
} esm_result_e;

typedef void (*pin_generated_func_type)(char *pin_data, size_t pin_size);
typedef int (*pin_close_func_type)(void);
typedef int (*user_confirm_result_func_type)(void);
int esm_set_device_property_by_app(char *name, const wifi_mode_e *mode, int ea_mode, const wifi_freq_e freq);
esm_result_e esm_init_easysetup(int restart_flag, things_server_builder_s *server_builder);
esm_result_e esm_terminate_easysetup(void);
esm_result_e esm_set_wifi_conn_err(void);
bool esm_get_network_status(void);
void *esm_register_cloud_cb(void *);
int esm_register_pin_generated_cb(pin_generated_func_type func);
int esm_register_pin_close_cb(pin_close_func_type func);
int esm_register_user_confirm_cb(user_confirm_result_func_type func);
int esm_wifi_prov_check_cb(int enabled, char *ssid, char *addr);
int esm_save_easysetup_state(int state);
int esm_read_easysetup_state(void);

#endif							// _EASYSETUP_MANAGER_H_
