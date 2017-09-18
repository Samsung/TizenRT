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

#ifndef _THINGS_EASYSETUP_H_
#define _THINGS_EASYSETUP_H_

#define THINGS_STRING_MAX_DNSNAME  256
#define THINGS_STRING_MAX_LENGTH   100

#define MAX_SSIDLEN     33
#define MAX_CREDLEN     64
#define IP_PORT                 22

typedef enum {
	WiFi_11A = 0,
	WiFi_11B,
	WiFi_11G,
	WiFi_11N,
	WiFi_11AC,
	WiFi_EOF = 999,
} wifi_mode_e;

typedef enum {
	WiFi_24G = 0,
	WiFi_5G,
	WiFi_BOTH,
	WiFi_FREQ_EOF
} wifi_freq_e;

typedef enum {
	NONE_AUTH = 0,
	WEP,
	WPA_PSK,
	WPA2_PSK
} wifi_auth_type_e;

typedef enum {
	NONE_ENC = 0,
	WEP_64,
	WEP_128,
	TKIP,
	AES,
	TKIP_AES
} wifi_enc_type_e;

typedef struct {
	char ssid[MAX_SSIDLEN];	/**< ssid of the Enroller**/
	char pwd[MAX_CREDLEN]; /**< pwd of the Enroller**/
	wifi_auth_type_e authtype;	/**< auth type of the Enroller**/
	wifi_enc_type_e enctype; /**< encryption type of the Enroller**/
	int discovery_channel;		// samsung specific property
} es_wifi_prov_data_s;

typedef struct {
	char language[THINGS_STRING_MAX_LENGTH];
	char country[THINGS_STRING_MAX_LENGTH];
	char datetime[THINGS_STRING_MAX_LENGTH];
} es_dev_conf_prov_data_s;

typedef struct {
	char auth_code[THINGS_STRING_MAX_LENGTH];
	char auth_provider[THINGS_STRING_MAX_LENGTH];
	char ip[IP_PORT];
	char port[IP_PORT];
	char host_name[THINGS_STRING_MAX_DNSNAME];
	char ci_server[THINGS_STRING_MAX_LENGTH];
	char server_id[THINGS_STRING_MAX_LENGTH];
	char client_id[THINGS_STRING_MAX_LENGTH];
	char accesstoken[THINGS_STRING_MAX_LENGTH];
	int actoken_type;
	char refreshtoken[THINGS_STRING_MAX_LENGTH];
	char uid[THINGS_STRING_MAX_LENGTH];
} es_cloud_prov_data_s;

typedef enum {
	RST_NEED_CONFIRM = 0,

	RST_AUTO_RESET = 1,

	RST_ENUM_EOF
} es_enrollee_reset_e;

typedef enum {
	/**
	 * Default Abort State.
	 */
	ABORT_INIT = 0,

	/**
	 * Abort Status indicating cancel Reset Confirm.
	 */
	ABORT_BEFORE_RESET_CONFIRM = 1,

	/**
	 * Abort Status indicating cancel Security Confirm.
	 */
	ABORT_BEFORE_SEC_CONFIRM = 2,

	/**
	 * Abort Status indicating being security-process to delete Owned Security Data.
	 */
	ABORT_BEFORE_DATA_PROVISIONING = 3
} es_enrollee_abort_e;

/**
 * @brief Indicate enrollee and provisioning status. Provisioning status is shown in "provisioning
 *        status" property in provisioning resource.
 */
typedef enum {
	/**
	 * Default state of the device
	 */
	ES_STATE_INIT = 0,

	/**
	 * Status indicating being connecting to target network
	 */
	ES_STATE_CONNECTING_TO_ENROLLER,

	/**
	 * Status indicating successful connection to target network
	 */
	ES_STATE_CONNECTED_TO_ENROLLER,

	/**
	 * Status indicating connection failure to target network
	 */
	ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER,

	/**
	 * Status indicating being registering to cloud
	 */
	ES_STATE_REGISTERING_TO_CLOUD,

	/**
	 * Status indicating successful registration to cloud
	 */
	ES_STATE_REGISTERED_TO_CLOUD,

	/**
	 * Status indicating registration failure to cloud
	 */
	ES_STATE_FAILED_TO_REGISTER_TO_CLOUD,

	/**
	 * Status indicating being publishing resources to cloud
	 */
	ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD,

	/**
	 * Status indicating successful resource publish to cloud
	 */
	ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD,

	/**
	 * Status indicating resource publish failure to cloud
	 */
	ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD,

	/**
	 * Status indicating Logout from cloud
	 */
	ES_STATE_LOGOUT_FROM_CLOUD,

	/**
	 * End of Easy setup status
	 */
	ES_STATE_EOF = 255
} es_enrollee_state_e, prov_status_e, cloud_status_e;

#endif							/* _THINGS_EASYSETUP_H_ */
