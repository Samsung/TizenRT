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

#ifndef _THINGS_TYPES_H_
#define _THINGS_TYPES_H_

#include <wifi_manager/wifi_manager.h>
#define THINGS_STRING_MAX_DNSNAME  256
#define THINGS_STRING_MAX_LENGTH   100

#define IP_PORT               22

#define MAX_DEVICEID_LEN      8
#define MAX_RESOURCE_LEN      128
#define MAX_URI_LEN           128
#define MAX_QUERY_LEN         100
#define MAX_ATTRIBUTE_LEN     64
#define MAX_VALUETYPE_LEN     32
#define MAX_ATTRVALUE_LEN     512
#define MAX_KEY_LENGTH        50
#define MAX_SPEC_LENGTH       7
#define MAX_RID_LENGTH        10
#define MAX_IT_CNT            5
#define MAX_RT_CNT            10

#define MAX_LEVEL_SIGNAL      12
#define MAX_TYPE_AUTH         16
#define MAX_TYPE_ENC          16
#define MAX_CHANNEL           8

#define ES_COMPLETE            1
#define ES_NOT_COMPLETE        0

#define PATH_MNT "/mnt/"
#define PATH_ROM "/rom/"

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
	char ssid[WIFIMGR_SSID_LEN + 1];	/**< ssid of the Enroller**/
	char pwd[WIFIMGR_PASSPHRASE_LEN + 1]; /**< pwd of the Enroller**/
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

typedef struct things_cloud_info_s {
	char *domain;			// mandatory ( You can choose one in both "domain" and "ip". )
	char *ip;
	char *port;				// optional
	char *auth_provider;	// mandatory
	char *access_token;		// mandatory
	char *access_token_type;	// optional
	char *refresh_token;	// mandatory
	char *user_id;			// mandatory
	char *client_id;		// mandatory
} things_cloud_info_s;

typedef struct things_info_s {
	char device_id[MAX_DEVICEID_LEN];
	char resource[MAX_RESOURCE_LEN];
	char query[MAX_QUERY_LEN];
	char attribute[MAX_ATTRIBUTE_LEN];
	char type[MAX_VALUETYPE_LEN];
	char value[MAX_ATTRVALUE_LEN];
	char id[MAX_DEVICEID_LEN];

	void *request_handle;
	void *resource_handle;
} things_info_s;

typedef struct things_attribute_info_s {
	char key[MAX_KEY_LENGTH];
	char spec[MAX_SPEC_LENGTH];
	int type;
	bool mandatory;
	int rw;
} things_attribute_info_s;

typedef struct things_resource_info_s {
	char rid[MAX_RID_LENGTH];
	char uri[MAX_URI_LEN];
	char *interface_types[MAX_IT_CNT];
	char *resource_types[MAX_RT_CNT];

	int if_cnt;
	int rt_cnt;
	bool observable;
	int policy;
} things_resource_info_s;

typedef enum {
	RST_NEED_CONFIRM = 0,

	RST_AUTO_RESET = 1,

	RST_ENUM_EOF
} things_es_enrollee_reset_e;

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
} things_es_enrollee_abort_e;

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
} things_es_enrollee_state_e, things_prov_status_e, things_cloud_status_e;

typedef struct access_point_info_s {
	char e_ssid[WIFIMGR_SSID_LEN + 1];		// mandatory
	char security_key[WIFIMGR_PASSPHRASE_LEN + 1];	// mandatory
	char auth_type[MAX_TYPE_AUTH];	// mandatory (None | WEP | WPA-PSK | WPA2-PSK)
	char enc_type[MAX_TYPE_ENC];	// mandatory (WEP-64 | WEP-128 | TKIP | AES | TKIP_AES)
	char channel[MAX_CHANNEL];	// optional
	char signal_level[MAX_LEVEL_SIGNAL];	// optional
	char bss_id[WIFIMGR_MACADDR_STR_LEN + 1];		// optional
	struct access_point_info_s* next;
} access_point_info_s;

#endif							/* _THINGS_TYPES_H_ */
