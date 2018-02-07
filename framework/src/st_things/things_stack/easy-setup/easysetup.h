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

#ifndef EASYSETUP_ENROLLEE_H__
#define EASYSETUP_ENROLLEE_H__

#include "es_common.h"

typedef void (*es_enrollee_event_callback)(es_result_e es_result, things_es_enrollee_state_e enrolleeState);

typedef struct {
	void (*wifi_prov_cb)(es_wifi_prov_data_s *);
	void (*dev_conf_prov_cb)(es_dev_conf_prov_data_s *);
	void (*cloud_data_prov_cb)(es_cloud_prov_data_s *);
} es_provisioning_callbacks_s;

es_result_e es_init_enrollee(bool is_secured, es_resource_mask_e resource_mask, es_provisioning_callbacks_s callbacks);

void es_register_notify_easysetup_state(void *func);

things_es_enrollee_state_e es_get_state(void);

bool es_get_cloud_login_state(void);

es_result_e es_set_device_property(es_device_property *device_property);

es_result_e es_set_state(things_es_enrollee_state_e es_state);

es_result_e es_set_error_code(es_error_code_e es_err_code);

es_result_e es_set_cloud_error_code(ci_error_code_e es_err_code);

es_result_e es_terminate_enrollee();

#endif							/* EASYSETUP_ENROLLEE_H__ */
