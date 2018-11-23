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

#include <string.h>
#include "ocstack.h"
#include "es_common.h"
#include "octypes.h"

#ifndef ES_RESOURCE_HANDLER_H_
#define ES_RESOURCE_HANDLER_H_

typedef enum {
	WIFI_INIT = 0,
	WIFI_READY = 1,
	WIFI_RESPONSE = 2,
	WIFI_SET = 3,
	WIFI_DONE
} wifi_prov_state_e;

typedef void (*es_wifi_cb)(es_result_e, es_wifi_prov_data_s *);
typedef void (*es_cloud_cb)(es_result_e, es_cloud_prov_data_s *);
typedef void (*es_dev_conf_cb)(es_result_e, es_dev_conf_prov_data_s *);

/* Structure to represent a Light resource */
typedef struct prov_resource_s {
	OCResourceHandle handle;
	things_prov_status_e status;		// provisiong status
	es_error_code_e last_err_code;
	ci_error_code_e vd_err_code;
} prov_resource_s;

typedef struct {
	OCResourceHandle handle;
	wifi_mode_e supported_mode[NUM_WIFIMODE];
	uint8_t num_mode;			// the number of device's supported wifi modes
	wifi_freq_e supported_freq;
	char ssid[WIFIMGR_SSID_LEN+1];		// target network name, i.e. SSID for WLAN, MAC address for BT.
	char cred[WIFIMGR_PASSPHRASE_LEN+1];		// credential information.
	wifi_auth_type_e auth_type;
	wifi_enc_type_e enc_type;
	int discovery_channel;
} wifi_resource_s;

typedef struct {
	OCResourceHandle handle;
	char auth_code[THINGS_STRING_MAX_VALUE];
	char auth_provider[THINGS_STRING_MAX_VALUE];
	char ci_server[THINGS_STRING_MAX_VALUE];
	char server_id[THINGS_STRING_MAX_VALUE];
	char client_id[THINGS_STRING_MAX_VALUE];
	char accesstoken[THINGS_STRING_MAX_VALUE];
	int actoken_type;
	char refreshtoken[THINGS_STRING_MAX_VALUE];
	char uid[THINGS_STRING_MAX_VALUE];
} cloud_resource_s;

typedef struct {
	OCResourceHandle handle;
	char devName[MAX_DEVICELEN];
	char language[THINGS_STRING_MAX_VALUE];
	char country[THINGS_STRING_MAX_VALUE];
	char device_type[THINGS_STRING_MAX_VALUE];
#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
	char device_sub_type[THINGS_STRING_MAX_VALUE];
#endif
	char datetime[THINGS_STRING_MAX_VALUE];
} dev_conf_resource_s;

void set_ssid_in_wifi_resource(const char *ssid);
void init_es_cloud_prov_data(es_cloud_prov_data_s *p_cloud_data);
void clone_es_cloud_prov_data(es_cloud_prov_data_s *des, es_cloud_prov_data_s *src);
void set_wifi_prov_state(wifi_prov_state_e value);
wifi_prov_state_e get_wifi_prov_state(void);

OCStackResult create_easysetup_resources(bool is_secured, es_resource_mask_e resource_mask);
OCStackResult delete_easysetup_resources();
OCStackResult prov_rsc_notify_all_observers(void);

things_es_enrollee_state_e get_enrollee_state(void);

OCStackResult set_device_property(es_device_property *device_property);
OCStackResult set_enrollee_state(things_es_enrollee_state_e es_state);
OCStackResult set_enrollee_err_code(es_error_code_e es_err_code);

void get_target_network_info_from_prov_resource(char *, char *);
void register_wifi_rsrc_event_callback(es_wifi_cb);
void register_cloud_rsrc_event_callback(es_cloud_cb);
void register_dev_conf_rsrc_event_callback(es_dev_conf_cb);
void unregister_resource_event_callback(void);

const char *get_result(OCStackResult result);
const char *get_prov_status(things_es_enrollee_state_e status);
const char *get_prov_errcode(es_error_code_e last_errcode);

#endif							//ES_RESOURCE_HANDLER_H_
