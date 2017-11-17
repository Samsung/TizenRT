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

#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdlib.h>
#include <json/cJSON.h>

#include "octypes.h"
#include "ocpayload.h"
#include "easysetup.h"
#include "ocstackconfig.h"
#include "ocrandom.h"
#include "things_def.h"
#include "things_logger.h"
#include "things_malloc.h"
#include "things_string_util.h"
#include "things_hashmap.h"
#include "things_resource.h"
#include "things_data_manager.h"
#include "things_server_builder.h"
#include "cloud_manager.h"
#include "cloud_connector.h"
#include "things_types.h"
#include <wifi_manager/wifi_manager.h>

#define TAG "[things_datamgr]"

#define KEY_CLOUD       "cloud"
#define KEY_WIFI       "wifi"

#define KEY_WIFI_SSID       "ssid"
#define KEY_WIFI_PASSPHARASE       "passphrase"
#define KEY_WIFI_AUTH_TYPE       "auth_type"
#define KEY_WIFI_CRYPTO_TYPE       "crypto_type"

#define KEY_CLOUD_DOMAIN   "domain"
#define KEY_CLOUD_ADDRESS  "address"
#define KEY_CLOUD_PORT     "port"
#define KEY_FILE_PATH    "fileRWpath"
#define KEY_ATTR_PATH2   "path"

#define KEY_DEVICE         "device"
#define KEY_DEVICE_SPECIFICATION    "specification"
#define KEY_DEVICE_SPECIFICATION_DEVICE     "device"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DEVICETYPE      "deviceType"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DEVICENAME      "deviceName"
#define KEY_DEVICE_SPECIFICATION_DEVICE_SPECVERSION     "specVersion"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DATAMODELVERSION    "dataModelVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM   "platform"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERNAME  "manufacturerName"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERURL   "manufacturerUrl"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURINGDATE "manufacturingDate"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MODELNUMBER   "modelNumber"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_PLATFORMVERSION   "platformVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_OSVERSION     "osVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_HARDWAREVERSION   "hardwareVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_FIRMWAREVERSION   "v"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_VENDORID  "vendorId"

#define KEY_RESOURCES       "resources"
#define KEY_RESOURCES_TYPE  "resourceTypes"

#define KEY_RESOURCES_COL   "collection"
#define KEY_RESOURCES_SIG   "single"

#define KEY_CONFIGURATION    "configuration"
#define KEY_CONFIGURATION_EASYSETUP    "easySetup"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY    "connectivity"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_TYPE    "type"

#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP    "softAP"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_MANUFACTUREID    "manufacturerId"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_SETUPID    "setupId"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_ARTIK    "artik"
#define KEY_CONFIGURATION_EASYSETUP_OWNERSHIP       "ownershipTransferMethod"

#define KEY_CONFIGURATION_WIFI    "wifi"
#define KEY_CONFIGURATION_WIFI_INTERFACES    "interfaces"
#define KEY_CONFIGURATION_WIFI_FREQUENCY    "frequency"

#define KEY_CONFIGURATION_FILEPATH      "filePath"
#define KEY_CONFIGURATION_FILEPATH_SVRDB    "svrdb"
#define KEY_CONFIGURATION_FILEPATH_PROVISIONING     "provisioning"
#define KEY_CONFIGURATION_FILEPATH_CERTIFICATE     "certificate"
#define KEY_CONFIGURATION_FILEPATH_PRIVATEKEY       "privateKey"

#define KEY_DEVICE_RESOURCE_URI "uri"
#define KEY_DEVICE_RESOURCE_TYPES "types"
#define KEY_DEVICE_RESOURCE_INTERFACES "interfaces"
#define KEY_DEVICE_RESOURCE_POLICY "policy"
#define KEY_DEVICE_RESOURCE_COLLECTION_LINKS    "links"

#define KEY_DEVICE_RESOURCETYPE_TYPE   "type"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES    "properties"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_KEY    "key"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_TYPE    "type"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_MANDATORY    "mandatory"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_RW    "rw"

#define KEY_ATTR_DEVICE_DEVICEID     "devid"

#define CHECK_BIT(var, pos)  (((var)>>(pos)) & 1)

#define CHECK_READABLE(var) CHECK_BIT(var, 0)
#define CHECK_WRITABLE(var) CHECK_BIT(var, 1)

#define CHECK_DISCOVERABLE(var) CHECK_BIT(var, 0)
#define CHECK_OBSERVABLE(var)   CHECK_BIT(var, 1)
#define CHECK_SECURE(var)       CHECK_BIT(var, 2)

#define MAX_FILE_ACCESS_CNT          (10)
#define MAX_KEY_LENGTH               (50)

#define MAX_URI_LENGTH          (128)
#define MAX_IT_CNT                    (5)
#define MAX_RT_CNT                   (10)

#define MAX_ATTRIBUTE_LENGTH     (64)

#define MAX_CLOUD_ADDRESS           (256)	//   Need to match with the Cloud Spec.
#define MAX_CLOUD_SESSIONKEY        (128)	//   Need to match with the Cloud Spec.

#define MAX_FILE_PATH_LENGTH        (250)

#define MAX_SUBDEVICE_EA            (100)

#define MAX_SOFTAP_SSID				(64)

static bool is_support_user_def_dev_list = true;	// It's allow to apply user-defined device-ID only when start Stack.
static char *user_def_dev_list[MAX_SUBDEVICE_EA + 1] = { 0, };

static volatile int resource_type_cnt = 0;

static char g_things_cloud_file_path[MAX_FILE_PATH_LENGTH] = { 0 };
static char g_things_info_file_path[MAX_FILE_PATH_LENGTH] = { 0 };
static char g_svrdb_file_path[MAX_FILE_PATH_LENGTH] = { 0 };
static char g_certificate_file_path[MAX_FILE_PATH_LENGTH] = { 0 };
static char g_private_key_file_path[MAX_FILE_PATH_LENGTH] = { 0 };

static char g_cloud_address[MAX_CLOUD_ADDRESS] = { 0 };

static char *g_manufacturer_name;
static char *g_setup_id;
static bool is_artik;
static char g_easysetup_softap_ssid[MAX_SOFTAP_SSID + 1];
static char *g_easysetup_softap_passphrase = "1111122222";
static char *g_easysetup_tag = "E1";
static int g_easysetup_softap_channel = 1;

static int g_wifi_interface;
static wifi_freq_e g_wifi_freq;

static int g_ownership_transfer_method = 0;

static struct hashmap_s *g_resource_type_hmap = NULL;	// map for resource types
static struct hashmap_s *g_device_hmap = NULL;

static bool is_there_cloud_data = false;
static const char *origin_cloud_json_str = "{\n\
		\"cloud\":    {\n\
			\"address\":    \"52.40.216.160:5683\"\n\
		},\n\
		\"wifi\": {\n\
			\"ssid\": \"unknown\"\n\
		}\n\
}";

static wifi_manager_softap_config_s ap_config;
static easysetup_connectivity_type_e es_conn_type = es_conn_type_none;
static wifi_manager_ap_config_s g_homeap_info;

typedef int8_t INT8;

#define STD_CI_CLOUD_ADDRESS "52.40.216.160:5683"
#define MAX_CI_CLOUD_ADDRESS 256
static es_cloud_signup_s *gpst_cloud_data = NULL;
static char g_ci_cloud_address[MAX_CI_CLOUD_ADDRESS] = { 0 };

struct things_attribute_info_s const gstAttr_x_com_samsung_provisioninginfo[] = {
	{
		.key = "x.com.samsung.provisioning.targets",
		.type = 9,
		.mandatory = false,
		.rw = 1
	},
	{
		.key = "x.com.samsung.provisioning.owned",
		.type = 0,
		.mandatory = false,
		.rw = 1
	},
	{
		.key = "x.com.samsung.provisioning.easysetupdi",
		.type = 3,
		.mandatory = false,
		.rw = 1
	},
	{
		.key = "x.com.samsung.provisioning.reset",
		.type = 0,
		.mandatory = false,
		.rw = 3
	},
	{
		.key = "x.com.samsung.provisioning.stopAP",
		.type = 0,
		.mandatory = false,
		.rw = 3
	}
};

const struct things_attribute_info_s const gstAttr_x_com_samsung_accesspointlist[] = {
	{
		.key = "x.com.samsung.accesspoint.items",
		.type = 9,
		.mandatory = true,
		.rw = 1
	}
};

static const struct st_resource_type_s const gst_resource_types[] = {
	{
		.rt = "x.com.samsung.provisioninginfo",
		.prop_cnt = 5,
		.prop[0] = &gstAttr_x_com_samsung_provisioninginfo[0],
		.prop[1] = &gstAttr_x_com_samsung_provisioninginfo[1],
		.prop[2] = &gstAttr_x_com_samsung_provisioninginfo[2],
		.prop[3] = &gstAttr_x_com_samsung_provisioninginfo[3],
		.prop[4] = &gstAttr_x_com_samsung_provisioninginfo[4]
	},
	{
		.rt = "x.com.samsung.accesspointlist",
		.prop_cnt = 1,
		.prop[0] = &gstAttr_x_com_samsung_accesspointlist[0]
	}
};

static const struct things_resource_info_s const gstResources[] = {
	{
		.uri = "/sec/provisioninginfo",
		.interface_types = {"oic.if.a"},
		.resource_types = {"x.com.samsung.provisioninginfo"},
		.if_cnt = 1,
		.rt_cnt = 1,
		.policy = 3
	},
	{
		.uri = "/sec/accesspointlist",
		.interface_types = {"oic.if.s"},
		.resource_types = {"x.com.samsung.accesspointlist"},
		.if_cnt = 1,
		.rt_cnt = 1,
		.policy = 3
	}
};

/**
 *	@fn get_cloud_server_address
 *	@brief Function to get cloud server address
 *  @param pch_current_server
 *  @return
 */
char *get_cloud_server_address(char *pch_current_server)
{
	if (gpst_cloud_data != NULL) {
		memset(g_ci_cloud_address, 0, MAX_CI_CLOUD_ADDRESS);
		memcpy(g_ci_cloud_address, gpst_cloud_data->address, strlen(gpst_cloud_data->address) + 1);
	} else {
		memcpy(g_ci_cloud_address, STD_CI_CLOUD_ADDRESS, strlen(STD_CI_CLOUD_ADDRESS) + 1);
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "CLOUD CI Address : %s", g_ci_cloud_address);

	return g_ci_cloud_address;
}

static struct things_attribute_info_s *create_property()
{
	struct things_attribute_info_s *property = things_malloc(sizeof(things_attribute_info_s));

	memset(property->key, 0, (size_t) MAX_KEY_LENGTH);
	property->type = 0;
	property->mandatory = false;
	property->rw = 0;

	return property;
}

static struct st_resource_type_s *create_resource_type()
{
	struct st_resource_type_s *type = things_malloc(sizeof(st_resource_type_s));

	if (type == NULL) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "Failed to create_resource_type");
		return NULL;
	}

	memset(type->rt, 0, (size_t) MAX_ATTRIBUTE_LENGTH);
	memset(type->prop, 0, (size_t) MAX_PROPERTY_CNT);
	type->prop_cnt = 0;

	return type;
}

static struct things_resource_info_s *create_resource()
{
	struct things_resource_info_s *resource = things_malloc(sizeof(things_resource_info_s));

	if (resource == NULL) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "Failed to create_resource");
		return NULL;
	}

	memset(resource->uri, 0, sizeof(resource->uri));
	memset(resource->interface_types, 0, sizeof(resource->interface_types));
	memset(resource->resource_types, 0, sizeof(resource->resource_types));
	resource->policy = 0;
	resource->if_cnt = 0;
	resource->rt_cnt = 0;

	return resource;
}

static st_device_s *create_device()
{
	st_device_s *device = things_malloc(sizeof(st_device_s));

	if (device == NULL) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "Failed to create_device");
		return NULL;
	}

	memset(device->type, 0, (size_t) MAX_DEVICE_TYPE_LENGTH);
	memset(device->name, 0, (size_t) MAX_DEVICE_NAME_LENGTH);
	memset(device->model_num, 0, (size_t) MAX_DEVICE_MODEL_ID_LENGTH);
	memset(device->ver_p, 0, (size_t) MAX_DEVICE_VER_P);
	memset(device->ver_os, 0, (size_t) MAX_DEVICE_VER_OS);
	memset(device->ver_hw, 0, (size_t) MAX_DEVICE_VER_HW);
	memset(device->ver_fw, 0, (size_t) MAX_DEVICE_VER_FW);
	memset(device->device_id, 0, (size_t) MAX_DEVICE_ID_LENGTH);
	memset(device->vender_id, 0, (size_t) MAX_DEVICE_VENDER_ID);
	memset(device->description, 0, (size_t) MAX_DEVICE_DESCRIPTION_LENGTH);

	device->no = -1;
	device->capa_cnt = 0;
	device->col_cnt = 0;
	device->sig_cnt = 0;
	device->is_physical = 0;

	for (int index = 0; index < MAX_DEVICE_CAPABILTY_CNT; index++) {
		device->pchild_resources[index] = NULL;
	}

	return device;
}

static size_t get_json_file_size(const char *filename)
{
	size_t size = 0;
	FILE *fp = fopen(filename, "r");

	if (fp) {
		if (0 == fseek(fp, 0L, SEEK_END)) {
			size = ftell(fp);
			rewind(fp);
		}
		fclose(fp);
	}
	return size;
}

static char *get_json_string_from_file(const char *filename)
{
	FILE *fp = NULL;
	char *json_str = NULL;
	size_t size = get_json_file_size(filename);
	size_t readed = 0;
	int fp_acess_cnt = 0;

	if (size <= 0) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "Failed converting to JSON");
		return NULL;
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "File size: %d ", size);
	}

	json_str = (char *)things_malloc(size + 1);

	// 1. File Read
	fp = fopen(filename, "r");
	if (NULL == fp) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to open %s", filename);
		things_free(json_str);
		return NULL;
	}
	// 2. Json String read from the given file.
	size_t bytes_read = 0;
	while (readed < size && fp_acess_cnt < MAX_FILE_ACCESS_CNT) {
		bytes_read = fread(&(json_str[readed]), 1, size - readed, fp);
		readed += bytes_read;
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Read Size: %d, total Size =%d", readed, size);
		fp_acess_cnt++;
	}

	if (fp_acess_cnt >= MAX_FILE_ACCESS_CNT) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Access-Times is Over for File Read. (Readed Size: %d, total Size =%d)", readed, size);
		things_free(json_str);
		fclose(fp);
		return NULL;
	}

	json_str[readed] = '\0';

	fclose(fp);
	fp = NULL;

	return json_str;
}

static int set_json_string_into_file(const char *filename, const char *json_str)
{
	FILE *fp = NULL;
	// 1. File Read
	fp = fopen(filename, "w");
	if (NULL == fp) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to open \"%s\" with Write permission.", filename);
		return 0;
	}
	// 2. Json String read from the given file.
	fwrite(json_str, 1, strlen(json_str), fp);

	fclose(fp);
	fp = NULL;
	return 1;
}

static int init_things_info_file(cJSON *cjson, char *origin_json_str)
{
	int res = 1;

	if (g_things_info_file_path[0] == 0) {
		int length = 0;
		cJSON *file_rw_path = cJSON_GetObjectItem(cjson, KEY_FILE_PATH);
		if (NULL == file_rw_path || (length = strlen(file_rw_path->valuestring)) <= 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "\"%s\" key is not exist in info file. or value is empty.", KEY_FILE_PATH);
			return 0;
		}

		memset(g_things_info_file_path, 0, MAX_FILE_PATH_LENGTH);
		memcpy(g_things_info_file_path, file_rw_path->valuestring, length + 1);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Info file path : %s", g_things_info_file_path);
	}

	int is_valid = 1;

	char *json_str = get_json_string_from_file(g_things_info_file_path);
	if (json_str != NULL) {
		cJSON *root = cJSON_Parse((const char *)json_str);
		if (root == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "%s fail to parsed.", g_things_info_file_path);
			is_valid = 0;
		}
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "%s is empty.", g_things_info_file_path);
		is_valid = 0;
	}

	if (is_valid == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Try to reset the info file.");
		if (set_json_string_into_file(g_things_info_file_path, origin_json_str) == 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "original info file copy failed.");
			res = 0;
		}
	}

	return res;
}

static int get_json_int(cJSON *json, int64_t *variable)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

	int res = 1;

	if (json == NULL || variable == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json = %d, variable = %d", json, variable);
		return 0;
	}

	if (json->type != cJSON_Number) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json-value Type is Not String value.");
		return 0;
	}

	*variable = json->valueint;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");

	return res;
}

static int get_json_string(cJSON *json, char **variable)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

	int res = 1;
	int length = 0;

	if (json == NULL || variable == NULL || *variable != NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "There is NULL value.(json = %d, variable = %d)", json, variable);
		if (variable != NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "variable value is not NULL.(*variable = %d)", *variable);
		}
		return 0;
	}

	if ((length = strlen(json->valuestring)) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "length is NULL");
		return 0;
	}

	*variable = (char *)things_malloc(sizeof(char) * (length + 1));
	if (*variable == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
		return 0;
	}

	memset(*variable, 0, length + 1);
	memcpy(*variable, json->valuestring, length + 1);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");

	return res;
}

static int load_cloud_signup_data(cJSON *json, es_cloud_signup_s **cl_data)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

	int res = 1;

	if (json == NULL || cl_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json = %d, cl_data = %d", json, cl_data);
		return -1;
	}

	cJSON *domain = cJSON_GetObjectItem(json, KEY_CLOUD_DOMAIN);
	cJSON *address = cJSON_GetObjectItem(json, KEY_CLOUD_ADDRESS);
	cJSON *port = cJSON_GetObjectItem(json, KEY_CLOUD_PORT);
	cJSON *accesstoken = cJSON_GetObjectItem(json, KEY_TOKEN_ACCESS);
	cJSON *refresh_accesstoken = cJSON_GetObjectItem(json, KEY_TOKEN_ACCESS_REFRESH);
	cJSON *token_type = cJSON_GetObjectItem(json, KEY_TOKEN_TYPE);
	cJSON *expire_time = cJSON_GetObjectItem(json, KEY_EXPIRE_TIME);
	cJSON *user_id = cJSON_GetObjectItem(json, KEY_ID_USER);
	cJSON *redirect_uri = cJSON_GetObjectItem(json, KEY_SERVER_REDIRECT_URI);
	cJSON *certificate = cJSON_GetObjectItem(json, KEY_CERTIFICATE_FILE);
	cJSON *server_id = cJSON_GetObjectItem(json, KEY_SERVER_ID);

	if (address == NULL || accesstoken == NULL || refresh_accesstoken == NULL || user_id == NULL || expire_time == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "address = %d, accesstoken = %d, refresh_accesstoken = %d, user_id = %d, expire_time = %d", address, accesstoken, refresh_accesstoken, user_id, expire_time);
		return -2;
	}

	if (es_cloud_signup_init(cl_data) == false) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ESCloud data initialization is failed.");
		return -1;
	}

	if (get_json_string(address, &((*cl_data)->address)) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String address is failed");
		res = -1;
	}

	if (get_json_string(accesstoken, &((*cl_data)->access_token)) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String accesstoken is failed");
		res = -1;
	}

	if (get_json_string(refresh_accesstoken, &((*cl_data)->refresh_token)) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String refresh_accesstoken is failed");
		res = -1;
	}

	if (get_json_string(user_id, &((*cl_data)->uid)) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String user_id is failed");
		res = -1;
	}

	if (get_json_int(expire_time, &((*cl_data)->expire_time)) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String expire_time is failed");
		res = -1;
	}

	get_json_string(token_type, &((*cl_data)->token_type));
	get_json_string(redirect_uri, &((*cl_data)->redirect_uri));
	get_json_string(certificate, &((*cl_data)->certificate));
	get_json_string(server_id, &((*cl_data)->sid));
	get_json_string(domain, &((*cl_data)->domain));
	get_json_string(port, &((*cl_data)->port));

	if (res == -1) {
		es_cloud_signup_clear(*cl_data);
		*cl_data = NULL;
	} else {
		memcpy(g_cloud_address, (*cl_data)->address, strlen((*cl_data)->address) + 1);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "CLOUD CI Address : %s", g_cloud_address);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");

	return res;
}

static int if_need_init_things_cloud_file(const char *filename)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	int res = 0;

	FILE *fp = fopen(filename, "r");
	if (fp) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Already Exist : provisioning file.");
		fclose(fp);
	} else {
		if (set_json_string_into_file(filename, origin_cloud_json_str) == 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Creating privisioning file is failed.");
			res = -1;
		} else {
			THINGS_LOG_D(THINGS_INFO, TAG, "Creating privisioning file is Success.");
			res = 1;
		}
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return res;
}

easysetup_connectivity_type_e dm_get_easysetup_connectivity_type(void)
{
	return es_conn_type;
}

wifi_manager_softap_config_s *dm_get_softap_wifi_config(void)
{
	st_device_s *device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)(0));
	int ssid_type = (is_artik == true ? 1 : 0);
	unsigned char mac_id[16] = { 0, };
	char ssid_device_name[17];

	snprintf(ssid_device_name, sizeof(ssid_device_name), "%s", device->name);
	THINGS_LOG_V(THINGS_INFO, TAG, "DeviceName : %s", ssid_device_name);

	wifi_manager_info_s st_wifi_info;
	wifi_manager_get_info(&st_wifi_info);

	snprintf(mac_id, sizeof(mac_id), "%02X%02X", st_wifi_info.mac_address[4], st_wifi_info.mac_address[5]);

	snprintf(g_easysetup_softap_ssid, sizeof(g_easysetup_softap_ssid), "%s_%s%s%s%d%s", ssid_device_name, g_easysetup_tag, g_manufacturer_name, g_setup_id, 0, mac_id);
	THINGS_LOG_V(THINGS_INFO, TAG, "SoftAP SSID : %s", g_easysetup_softap_ssid);

	snprintf(ap_config.ssid, sizeof(ap_config.ssid), "%s", g_easysetup_softap_ssid);
	snprintf(ap_config.passphrase, sizeof(ap_config.passphrase), "%s", g_easysetup_softap_passphrase);
	ap_config.channel = g_easysetup_softap_channel;

	return &ap_config;
}

wifi_manager_ap_config_s *dm_get_homeap_wifi_config(void)
{
	return &g_homeap_info;
}

int parse_things_cloud_json(const char *filename)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	int ret = 0;
	char *json_str = NULL;

	if (if_need_init_things_cloud_file(filename) == -1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "RW CloudFile-loading is failed. please check your \"%s\"", filename);
		return 0;
	}

RETRY_JSON:
	json_str = get_json_string_from_file(filename);

	if (json_str == NULL) {
		THINGS_LOG_V(THINGS_INFO, TAG, "cloud file initialization.");
		if (set_json_string_into_file(filename, origin_cloud_json_str) == 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Creating cloud file is failed.");
			return 0;
		}
		goto RETRY_JSON;
	}

	if (strlen(json_str) > 0) {
		// 3. Parse the Json string
		cJSON *root = cJSON_Parse((const char *)json_str);

		if (root == NULL) {
			THINGS_LOG_V(THINGS_INFO, TAG, "cloud file context has a error about json-format.");
			things_free(json_str);
			json_str = NULL;

			THINGS_LOG_V(THINGS_INFO, TAG, "cloud file initialization.");
			if (set_json_string_into_file(filename, origin_cloud_json_str) == 0) {
				THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Creating colud file is failed.");
				return 0;
			}
			goto RETRY_JSON;
		}

		cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
		cJSON *wifi = cJSON_GetObjectItem(root, KEY_WIFI);

		if (NULL != wifi && NULL != cloud) {
			/***** Cloud *****/
			cJSON *address = cJSON_GetObjectItem(cloud, KEY_CLOUD_ADDRESS);
			if (NULL != address) {
				memset(g_cloud_address, 0, (size_t) MAX_CLOUD_ADDRESS);
				memcpy(g_cloud_address, address->valuestring, strlen(address->valuestring) + 1);
				THINGS_LOG_D(THINGS_DEBUG, TAG, "[CLOUD] CI Address : %s", g_cloud_address);
				ret = 1;
			}

			cJSON *accesstoken = cJSON_GetObjectItem(cloud, KEY_TOKEN_ACCESS);
			cJSON *refresh_accesstoken = cJSON_GetObjectItem(cloud, KEY_TOKEN_ACCESS_REFRESH);
			cJSON *user_id = cJSON_GetObjectItem(cloud, KEY_ID_USER);
			cJSON *expire_time = cJSON_GetObjectItem(cloud, KEY_EXPIRE_TIME);

			/***** Wifi *****/
			cJSON *ssid = cJSON_GetObjectItem(wifi, KEY_WIFI_SSID);
			if (NULL != ssid) {
				memcpy(g_homeap_info.ssid, ssid->valuestring, strlen(ssid->valuestring) + 1);
				THINGS_LOG_D(THINGS_DEBUG, TAG, "[WIFI] SSID Address : %s", g_homeap_info.ssid);
				ret = 1;
				g_homeap_info.ssid_length = strlen(g_homeap_info.ssid);
			}

			cJSON *passphrase = cJSON_GetObjectItem(wifi, KEY_WIFI_PASSPHARASE);
			if (NULL != passphrase) {
				memcpy(g_homeap_info.passphrase, passphrase->valuestring, strlen(passphrase->valuestring) + 1);
				THINGS_LOG_D(THINGS_DEBUG, TAG, "[WIFI] PASSPHRASE Address : %s", g_homeap_info.passphrase);
				ret = 1;
				g_homeap_info.passphrase_length = strlen(g_homeap_info.passphrase);
			}

			cJSON *auth_type = cJSON_GetObjectItem(wifi, KEY_WIFI_AUTH_TYPE);

			if (NULL != auth_type) {
				g_homeap_info.ap_auth_type = auth_type->valueint;
			}

			cJSON *crypto_type = cJSON_GetObjectItem(wifi, KEY_WIFI_CRYPTO_TYPE);
			if (NULL != crypto_type) {
				g_homeap_info.ap_crypto_type = crypto_type->valueint;
			}

			if (address == NULL || accesstoken == NULL || refresh_accesstoken == NULL || user_id == NULL || expire_time == NULL || ssid == NULL || passphrase == NULL || auth_type == NULL || crypto_type == NULL) {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "[ProvisionInfo] address = %d, accesstoken = %d, refresh_accesstoken = %d, user_id = %d, expire_time = %d", address, accesstoken, refresh_accesstoken, user_id, expire_time);
				is_there_cloud_data = false;
			} else {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "[ProvisionInfo] There is mandatory elements in cloud file.");
				is_there_cloud_data = true;
			}
		}

		if (NULL != root) {
			cJSON_Delete(root);
		}
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Can not Read \"%s\" file.", filename);
	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int parse_things_info_json(const char *filename)
{

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	int ret = 1;
	char *json_str = get_json_string_from_file(filename);

	if (json_str != NULL && strlen(json_str) > 0) {
		// 3. Parse the Json string
		cJSON *root = cJSON_Parse((const char *)json_str);
		assert(root != NULL);

		st_device_s *node = NULL;

		// Device Items
		cJSON *device = cJSON_GetObjectItem(root, KEY_DEVICE);
		int device_cnt = cJSON_GetArraySize(device);

		THINGS_LOG_D(THINGS_DEBUG, TAG, "device_cnt = %d", device_cnt);
		if (g_device_hmap == NULL) {
			g_device_hmap = hashmap_create(device_cnt);
		}

		if (g_device_hmap == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "g_device_hmap is NULL");
			cJSON_Delete(root);

			if (json_str != NULL) {
				things_free(json_str);
			}
			return 0;
		}

		THINGS_LOG_D(THINGS_DEBUG, TAG, "device_cnt of hashmap = %d", hashmap_count(g_device_hmap));

		for (int device_num = 0; device_num < device_cnt; device_num++) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] ============================================");

			node = create_device();
			node->no = device_num;
			node->is_physical = 1;

			cJSON *device_item = cJSON_GetArrayItem(device, device_num);
			cJSON *specification = cJSON_GetObjectItem(device_item, KEY_DEVICE_SPECIFICATION);
			if (NULL != specification) {
				cJSON *spec_device = cJSON_GetObjectItem(specification, KEY_DEVICE_SPECIFICATION_DEVICE);
				if (NULL != spec_device) {
					cJSON *device_type = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DEVICETYPE);
					cJSON *device_name = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DEVICENAME);
					cJSON *spec_version = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_SPECVERSION);
					cJSON *data_model_version = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DATAMODELVERSION);

					if (NULL != device_type) {
						memcpy(node->type, device_type->valuestring, strlen(device_type->valuestring) + 1);
					}

					if (NULL != device_name) {
						memcpy(node->name, device_name->valuestring, strlen(device_name->valuestring) + 1);
					}
				}

				cJSON *spec_platform = cJSON_GetObjectItem(specification, KEY_DEVICE_SPECIFICATION_PLATFORM);
				if (NULL != spec_platform) {
					cJSON *manufacturer_name = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERNAME);
					cJSON *manufacturer_url = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERURL);
					cJSON *manufacturing_date = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURINGDATE);
					cJSON *model_number = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MODELNUMBER);
					cJSON *platform_version = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_PLATFORMVERSION);
					cJSON *os_version = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_OSVERSION);
					cJSON *hardware_version = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_HARDWAREVERSION);
					cJSON *firmware_version = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_FIRMWAREVERSION);
					cJSON *vendor_id = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_VENDORID);

					if (NULL != manufacturer_name) {
						memcpy(node->manufacturer_name, manufacturer_name->valuestring, strlen(manufacturer_name->valuestring) + 1);
					}
					if (NULL != manufacturer_url) {
						memcpy(node->manufacturer_url, manufacturer_url->valuestring, strlen(manufacturer_url->valuestring) + 1);
					}
					if (NULL != manufacturing_date) {
						memcpy(node->manufacturing_date, manufacturing_date->valuestring, strlen(manufacturing_date->valuestring) + 1);
					}
					if (NULL != model_number) {
						memcpy(node->model_num, model_number->valuestring, strlen(model_number->valuestring) + 1);
					}
					if (NULL != platform_version) {
						memcpy(node->ver_p, platform_version->valuestring, strlen(platform_version->valuestring) + 1);
					}
					if (NULL != os_version) {
						memcpy(node->ver_os, os_version->valuestring, strlen(os_version->valuestring) + 1);
					}
					if (NULL != hardware_version) {
						memcpy(node->ver_hw, hardware_version->valuestring, strlen(hardware_version->valuestring) + 1);
					}
					if (NULL != firmware_version) {
						memcpy(node->ver_fw, firmware_version->valuestring, strlen(firmware_version->valuestring) + 1);
					}
					if (NULL != vendor_id) {
						memcpy(node->vender_id, vendor_id->valuestring, strlen(vendor_id->valuestring) + 1);
					}
				}
			}
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] type : %s", (node->type));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] name : %s", (node->name));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] model num : %s", (node->model_num));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] plat. ver : %s", (node->ver_p));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] os version : %s", (node->ver_os));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] hw version : %s", (node->ver_hw));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] fw version : %s", (node->ver_fw));
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] vender id : %s", (node->vender_id));

			cJSON *resources = cJSON_GetObjectItem(device_item, KEY_RESOURCES);
			if (resources != NULL) {
				// 1. Read thsoe resource(s) which will be binded into the Collection resource(/device/#)
				cJSON *collection = cJSON_GetObjectItem(resources, KEY_RESOURCES_COL);

				if (collection) {
					node->col_cnt = cJSON_GetArraySize(collection);
					THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] Resources for Collection Cnt : %d", node->col_cnt);

					for (int iter = 0; iter < node->col_cnt; iter++) {
						cJSON *res = cJSON_GetArrayItem(collection, iter);
						if (res->type != NULL) {
							cJSON *uri = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_URI);
							memcpy(node->collection[iter].uri, uri->valuestring, strlen(uri->valuestring) + 1);
							THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] collection[0].uri : %s", (node->collection[iter].uri));

							cJSON *types = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_TYPES);
							if (types) {
								int type_cnt = cJSON_GetArraySize(types);
								node->collection[iter].rt_cnt = type_cnt;
								for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
									cJSON *type = cJSON_GetArrayItem(types, typeiter);
									node->collection[iter].resource_types[typeiter] = things_malloc(sizeof(char *) * strlen(type->valuestring) + 1);
									memcpy(node->collection[iter].resource_types[typeiter], type->valuestring, strlen(type->valuestring) + 1);
									THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] collection[iter].resource_types[typeiter] : %s", (node->collection[iter].resource_types[typeiter]));
								}
							} else {
								return 0;
							}

							cJSON *interfaces = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_INTERFACES);
							if (interfaces) {
								int if_cnt = cJSON_GetArraySize(interfaces);
								node->collection[iter].if_cnt = if_cnt;
								for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
									cJSON *interface = cJSON_GetArrayItem(interfaces, ifiter);
									node->collection[iter].interface_types[ifiter] = things_malloc(sizeof(char *) * strlen(interface->valuestring) + 1);
									memcpy(node->collection[iter].interface_types[ifiter], interface->valuestring, strlen(interface->valuestring) + 1);
								}
							} else {
								return 0;
							}

							cJSON *policy = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_POLICY);
							node->collection[iter].policy = policy->valueint;
							THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] collection[iter].policy : %d", (node->collection[iter].policy));

							cJSON *links = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_COLLECTION_LINKS);
							if (links) {
								int linkCnt = cJSON_GetArraySize(links);
								node->collection[iter].link_cnt = linkCnt;
								THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] collection[iter].link_cnt : %d", (node->collection[iter].link_cnt));
								for (int linkiter = 0; linkiter < linkCnt; linkiter++) {
									cJSON *link = cJSON_GetArrayItem(links, linkiter);

									struct things_resource_info_s *link_resource = create_resource();

									cJSON *uri = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_URI);
									if (uri) {
										memcpy(link_resource->uri, uri->valuestring, strlen(uri->valuestring) + 1);
										THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] link_resource->uri : %s", (link_resource->uri));
									}

									cJSON *types = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_TYPES);
									if (types) {
										int type_cnt = cJSON_GetArraySize(types);
										link_resource->rt_cnt = type_cnt;
										for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
											cJSON *type = cJSON_GetArrayItem(types, typeiter);
											link_resource->resource_types[typeiter] = things_malloc(sizeof(char *) * strlen(type->valuestring) + 1);
											memcpy(link_resource->resource_types[typeiter], type->valuestring, strlen(type->valuestring) + 1);
										}
									} else {
										return 0;
									}
									cJSON *interfaces = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_INTERFACES);
									if (interfaces) {
										int if_cnt = cJSON_GetArraySize(interfaces);
										link_resource->if_cnt = if_cnt;
										for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
											cJSON *interface = cJSON_GetArrayItem(interfaces, ifiter);
											link_resource->interface_types[ifiter] = things_malloc(sizeof(char *) * strlen(interface->valuestring) + 1);
											memcpy(link_resource->interface_types[ifiter], interface->valuestring, strlen(interface->valuestring) + 1);
										}
									} else {
										return 0;
									}
									cJSON *policy = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_POLICY);
									if (policy) {
										link_resource->policy = policy->valueint;
										node->collection[iter].links[linkiter] = link_resource;
									} else {
										return 0;
									}
								}
							} else {
								return 0;
							}
						}

					}
					ret = 1;

					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] URI. : %s", node->collection[0].uri);
					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] interface_type. : %s", node->collection[0].interface_types[0]);
					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] resource_types. : %s", node->collection[0].resource_types[0]);
					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] if_cnt. : %d", node->collection[0].if_cnt);
					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] rt_cnt. : %d", node->collection[0].rt_cnt);
					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] link_cnt. : %d", node->collection[0].link_cnt);
					THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] policy. : %d", node->collection[0].policy);
				} else {
					THINGS_LOG_D(THINGS_INFO, TAG, "Children Reosurces Not Exist");
					//ret = 0;
				}

				// 2. Read thsoe resource(s) which will not be binded into the Collection resource
				cJSON *single = cJSON_GetObjectItem(resources, KEY_RESOURCES_SIG);

				if (single) {
					node->sig_cnt = cJSON_GetArraySize(single);
					THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] Resources for Single Usage Cnt : %d", node->sig_cnt);

					for (int iter = 0; iter < node->sig_cnt; iter++) {
						cJSON *res = cJSON_GetArrayItem(single, iter);
						if (res->type != NULL) {
							cJSON *uri = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_URI);
							if (uri) {
								memcpy(node->single[iter].uri, uri->valuestring, strlen(uri->valuestring) + 1);
							}

							cJSON *types = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_TYPES);
							if (types) {
								int type_cnt = cJSON_GetArraySize(types);
								node->single[iter].rt_cnt = type_cnt;
								for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
									cJSON *type = cJSON_GetArrayItem(types, typeiter);
									node->single[iter].resource_types[typeiter] = things_malloc(sizeof(char *) * strlen(type->valuestring) + 1);
									memcpy(node->single[iter].resource_types[typeiter], type->valuestring, strlen(type->valuestring) + 1);
								}
							} else {
								return 0;
							}
							cJSON *interfaces = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_INTERFACES);
							if (interfaces) {
								int if_cnt = cJSON_GetArraySize(interfaces);
								node->single[iter].if_cnt = if_cnt;
								for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
									cJSON *interface = cJSON_GetArrayItem(interfaces, ifiter);
									node->single[iter].interface_types[ifiter] = things_malloc(sizeof(char *) * strlen(interface->valuestring) + 1);
									memcpy(node->single[iter].interface_types[ifiter], interface->valuestring, strlen(interface->valuestring) + 1);
								}
							} else {
								return 0;
							}
							cJSON *policy = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_POLICY);
							if (policy) {
								node->single[iter].policy = policy->valueint;
							} else {
								return 0;
							}

						}
					}

					/***** register additional resource(provisininginfo / accesspointlist) *****/
					int resCnt = sizeof(gstResources) / sizeof(things_resource_info_s);
					for (int itr = 0; itr < resCnt; itr++) {
						memcpy(node->single[node->sig_cnt].uri, gstResources[itr].uri, strlen(gstResources[itr].uri) + 1);
						int type_cnt = gstResources[itr].rt_cnt;
						node->single[node->sig_cnt].rt_cnt = type_cnt;
						for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
							node->single[node->sig_cnt].resource_types[typeiter] = gstResources[itr].resource_types[typeiter];
						}

						int if_cnt = gstResources[itr].if_cnt;
						node->single[node->sig_cnt].if_cnt = if_cnt;
						for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
							node->single[node->sig_cnt].interface_types[ifiter] = gstResources[itr].interface_types[ifiter];
						}
						node->single[node->sig_cnt].policy = gstResources[itr].policy;
						node->sig_cnt++;
					}

					THINGS_LOG_D(THINGS_INFO, TAG, "Resources for Single Usage Cnt : %d", node->sig_cnt);
				} else {
					THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Single Reosurces Not Exist");
				}
			} else {
				THINGS_LOG_D(THINGS_INFO, TAG, "Reosurces Not Exist");
				ret = 0;
			}
			hashmap_insert(g_device_hmap, node, (unsigned long)device_num);
		}

		THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] ============================================");
		THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] Total Device Num : %d", (int)hashmap_count(g_device_hmap));

		// for resourceType
		struct st_resource_type_s *restype = NULL;
		cJSON *resource_types = cJSON_GetObjectItem(root, KEY_RESOURCES_TYPE);
		resource_type_cnt = cJSON_GetArraySize(resource_types);
		g_resource_type_hmap = hashmap_create(resource_type_cnt);

		THINGS_LOG_D(THINGS_INFO, TAG, "Resource Types Cnt : %d", resource_type_cnt);
		for (int i = 0; i < resource_type_cnt; i++) {
			int index = 0;

			restype = create_resource_type();

			cJSON *cj_rt = cJSON_GetArrayItem(resource_types, i);
			cJSON *rtype = cJSON_GetObjectItem(cj_rt, KEY_DEVICE_RESOURCETYPE_TYPE);
			cJSON *properties = cJSON_GetObjectItem(cj_rt, KEY_DEVICE_RESOURCETYPE_PROPERTIES);

			if (NULL != rtype) {
				index = hashmap_get_hashval((unsigned char *)rtype->valuestring);
				memcpy(restype->rt, rtype->valuestring, strlen(rtype->valuestring) + 1);

				if (NULL != properties) {
					restype->prop_cnt = cJSON_GetArraySize(properties);

					for (int iter2 = 0; iter2 < (restype->prop_cnt); iter2++) {
						cJSON *attr = cJSON_GetArrayItem(properties, iter2);
						cJSON *key = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_KEY);
						cJSON *type = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_TYPE);
						cJSON *mandatory = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_MANDATORY);
						cJSON *rw = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_RW);
						restype->prop[iter2] = create_property();
						if (key->valuestring != NULL) {
							memcpy(restype->prop[iter2]->key, key->valuestring, strlen(key->valuestring) + 1);
						}
						if (type->valueint != NULL) {
							restype->prop[iter2]->type = type->valueint;
						}

						if (mandatory->type == cJSON_True) {
							restype->prop[iter2]->mandatory = true;
						} else {
							restype->prop[iter2]->mandatory = false;
						}

						restype->prop[iter2]->rw = rw->valueint;
					}
				} else {
					THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Attribute Exist~!!!! ");
				}
				hashmap_insert(g_resource_type_hmap, restype, index);
			}
		}

		/***** register additional resourcetype(provisininginfo / accesspointlist) *****/
		int res_type_cnt = sizeof(gst_resource_types) / sizeof(st_resource_type_s);
		for (int rtItr = 0; rtItr < res_type_cnt; rtItr++) {
			int idx = hashmap_get_hashval((unsigned char *)gst_resource_types[rtItr].rt);
			hashmap_insert(g_resource_type_hmap, &gst_resource_types[rtItr], idx);
			resource_type_cnt++;
		}

		//for configuration
		cJSON *configuration = cJSON_GetObjectItem(root, KEY_CONFIGURATION);
		if (NULL != configuration) {
			int connectivity_type = 0;

			cJSON *easysetup = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_EASYSETUP);
			if (NULL != easysetup) {
				cJSON *connectivity = cJSON_GetObjectItem(easysetup, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY);
				if (NULL != connectivity) {
					cJSON *type = cJSON_GetObjectItem(connectivity, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_TYPE);
					connectivity_type = type->valueint;
					THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] type       : %d", connectivity_type);
					if (connectivity_type == 1) {
						es_conn_type = es_conn_type_softap;
						cJSON *softap = cJSON_GetObjectItem(connectivity, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP);
						if (NULL != softap) {
							cJSON *manufature_id = cJSON_GetObjectItem(softap, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_MANUFACTUREID);
							cJSON *setup_id = cJSON_GetObjectItem(softap, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_SETUPID);
							cJSON *artik = cJSON_GetObjectItem(softap, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_ARTIK);

							if (NULL != manufature_id && NULL != setup_id) {
								is_artik = false;
								if (artik->type == cJSON_True) {
									is_artik = true;
								}

								THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] manufature_id : %s / setup_id : %s / artik : %d", manufature_id->valuestring, setup_id->valuestring, is_artik);

								g_manufacturer_name = things_malloc(sizeof(char *) * strlen(manufature_id->valuestring) + 1);
								strncpy(g_manufacturer_name, manufature_id->valuestring, strlen(manufature_id->valuestring) + 1);

								g_setup_id = things_malloc(sizeof(char *) * strlen(setup_id->valuestring) + 1);
								strncpy(g_setup_id, setup_id->valuestring, strlen(setup_id->valuestring) + 1);
							} else {
								return 0;
							}
						}
					} else if (connectivity_type == 2) {
						//TO DO
						es_conn_type = es_conn_type_ble;
					} else {
						return 0;
					}
				} else {
					return 0;
				}

				cJSON *ownership_transfer_method = cJSON_GetObjectItem(easysetup, KEY_CONFIGURATION_EASYSETUP_OWNERSHIP);
				if (NULL != ownership_transfer_method) {
					g_ownership_transfer_method = ownership_transfer_method->valueint;
					THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] ownership_transfer_method : %d", g_ownership_transfer_method);
				} else {
					return 0;
				}

			}
			cJSON *wifi = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_WIFI);
			if (NULL != wifi) {
				cJSON *wifi_interfaces = cJSON_GetObjectItem(wifi, KEY_CONFIGURATION_WIFI_INTERFACES);
				cJSON *wifi_frequency = cJSON_GetObjectItem(wifi, KEY_CONFIGURATION_WIFI_FREQUENCY);
				if (NULL != wifi_interfaces && NULL != wifi_frequency) {
					THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] wifi_interfaces : %d / wifi_frequency : %d", wifi_interfaces->valueint, wifi_frequency->valueint);
					g_wifi_interface = wifi_interfaces->valueint;

					if (wifi_frequency->valueint == 1) {
						g_wifi_freq = WiFi_24G;
					} else if (wifi_frequency->valueint == 2) {
						g_wifi_freq = WiFi_5G;
					} else if (wifi_frequency->valueint == 3) {
						g_wifi_freq = WiFi_BOTH;
					} else {
						THINGS_LOG_D(THINGS_INFO, TAG, "unknown wifi freq value");
					}
				} else {
					return 0;
				}
			} else {
				return 0;
			}

			cJSON *file_path = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_FILEPATH);
			if (NULL != file_path) {
				cJSON *svrdb = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_SVRDB);
				cJSON *provisioning = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_PROVISIONING);
				cJSON *certificate = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_CERTIFICATE);
				cJSON *privateKey = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_PRIVATEKEY);

				if (NULL != svrdb && NULL != provisioning && NULL != certificate && NULL != privateKey) {
					memset(g_svrdb_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
					memset(g_certificate_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
					memset(g_private_key_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
					memcpy(g_svrdb_file_path, svrdb->valuestring, strlen(svrdb->valuestring) + 1);
					memcpy(g_certificate_file_path, certificate->valuestring, strlen(certificate->valuestring) + 1);
					memcpy(g_private_key_file_path, privateKey->valuestring, strlen(privateKey->valuestring) + 1);
					memcpy(g_things_cloud_file_path, provisioning->valuestring, strlen(provisioning->valuestring) + 1);

					THINGS_LOG_D(THINGS_INFO, TAG, "Security SVR DB file path : %s", g_svrdb_file_path);
					THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] svrdb : %s / provisioning : %s", svrdb->valuestring, provisioning->valuestring);
					THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] certificate : %s / privateKey : %s", certificate->valuestring, privateKey->valuestring);

					ret = 1;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		}
		if (NULL != root) {
			cJSON_Delete(root);
		}

	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	parse_things_cloud_json(g_things_cloud_file_path);

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int get_signup_data_from_json(const char *filename, es_cloud_signup_s **cl_data)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	int ret = 1;
	char *json_str = get_json_string_from_file(filename);

	if (json_str != NULL && strlen(json_str) > 0) {
		// 3. Parse the Json string
		cJSON *root = cJSON_Parse((const char *)json_str);
		cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
		if (cloud == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud cJSON is NULL.");
			ret = 0;
		} else if ((ret = load_cloud_signup_data(cloud, cl_data)) != 1) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load Cloud SignUp Data Failed.");
		}

		cJSON_Delete(root);
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud file Reading is failed.");
		ret = 0;
	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return ret;
}

int save_acces_point_info(wifi_manager_ap_config_s *connect_config)
{
	THINGS_LOG_D(THINGS_INFO, TAG, THINGS_FUNC_ENTRY);

	char *filename = g_things_cloud_file_path;

	int ret = 0;
	cJSON *root = NULL;
	char *json_update = NULL;
	char *json_str = get_json_string_from_file(filename);

	if (json_str != NULL && strlen(json_str) > 0) {
		root = cJSON_Parse((const char *)json_str);

		cJSON *wifi = cJSON_GetObjectItem(root, KEY_WIFI);
		if (wifi == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud cJSON is NULL.");
			goto GOTO_OUT;
		}

		cJSON_DeleteItemFromObject(wifi, KEY_WIFI_SSID);
		cJSON_DeleteItemFromObject(wifi, KEY_WIFI_PASSPHARASE);
		cJSON_DeleteItemFromObject(wifi, KEY_WIFI_AUTH_TYPE);
		cJSON_DeleteItemFromObject(wifi, KEY_WIFI_CRYPTO_TYPE);

		if (strlen(connect_config->ssid) > 0) {
			cJSON_AddStringToObject(wifi, KEY_WIFI_SSID, connect_config->ssid);
		}
		if (strlen(connect_config->passphrase) > 0) {
			cJSON_AddStringToObject(wifi, KEY_WIFI_PASSPHARASE, connect_config->passphrase);
		}
		cJSON_AddNumberToObject(wifi, KEY_WIFI_AUTH_TYPE, connect_config->ap_auth_type);
		cJSON_AddNumberToObject(wifi, KEY_WIFI_CRYPTO_TYPE, connect_config->ap_crypto_type);

		json_update = cJSON_Print(root);

		if (set_json_string_into_file(filename, json_update) == 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Store data to info file.");
			goto GOTO_OUT;
		}

		ret = 1;
	}							// End of if

	THINGS_LOG_D(THINGS_INFO, TAG, "Update Success in \"%s\" file.", filename);

GOTO_OUT:
	if (root) {
		cJSON_Delete(root);
		root = NULL;
	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	if (json_update != NULL) {
		things_free(json_update);
	}

	THINGS_LOG_D(THINGS_INFO, TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int update_things_cloud_json_by_cloud_signup(const char *filename, es_cloud_signup_s *cl_data)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	int ret = 0;
	cJSON *root = NULL;
	char *json_update = NULL;
	char *json_str = get_json_string_from_file(filename);

	if (json_str != NULL && strlen(json_str) > 0) {
		// 3. Parse the Json string
		root = cJSON_Parse((const char *)json_str);

		// Cloud Items
		cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
		if (cloud == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud cJSON is NULL.");
			goto GOTO_OUT;
		}

		if (cl_data != NULL) {
			cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_ADDRESS);
		}
		cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_DOMAIN);
		cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_PORT);
		cJSON_DeleteItemFromObject(cloud, KEY_TOKEN_ACCESS);
		cJSON_DeleteItemFromObject(cloud, KEY_TOKEN_ACCESS_REFRESH);
		cJSON_DeleteItemFromObject(cloud, KEY_TOKEN_TYPE);
		cJSON_DeleteItemFromObject(cloud, KEY_EXPIRE_TIME);
		cJSON_DeleteItemFromObject(cloud, KEY_ID_USER);
		cJSON_DeleteItemFromObject(cloud, KEY_SERVER_REDIRECT_URI);
		cJSON_DeleteItemFromObject(cloud, KEY_CERTIFICATE_FILE);
		cJSON_DeleteItemFromObject(cloud, KEY_SERVER_ID);

		memset(g_cloud_address, 0, MAX_CLOUD_ADDRESS);
		if (cl_data != NULL) {
			if (things_strcat(g_cloud_address, MAX_CLOUD_ADDRESS, cl_data->address) == NULL) {
				THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Copy to g_cloud_address.");
				goto GOTO_OUT;
			}
			if (cl_data->domain != NULL && strlen(cl_data->domain) > 0) {
				cJSON_AddStringToObject(cloud, KEY_CLOUD_DOMAIN, cl_data->domain);
			}
			if (cl_data->address != NULL && strlen(cl_data->address) > 0) {
				cJSON_AddStringToObject(cloud, KEY_CLOUD_ADDRESS, cl_data->address);
			}
			if (cl_data->port != NULL && strlen(cl_data->port) > 0) {
				cJSON_AddStringToObject(cloud, KEY_CLOUD_PORT, cl_data->port);
			}
			if (cl_data->access_token != NULL && strlen(cl_data->access_token) > 0) {
				cJSON_AddStringToObject(cloud, KEY_TOKEN_ACCESS, cl_data->access_token);
			}
			if (cl_data->refresh_token != NULL && strlen(cl_data->refresh_token) > 0) {
				cJSON_AddStringToObject(cloud, KEY_TOKEN_ACCESS_REFRESH, cl_data->refresh_token);
			}
			if (cl_data->token_type != NULL && strlen(cl_data->token_type) > 0) {
				cJSON_AddStringToObject(cloud, KEY_TOKEN_TYPE, cl_data->token_type);
			}
			if (cl_data->expire_time != CLOUD_EXPIRESIN_INVALID) {
				cJSON_AddNumberToObject(cloud, KEY_EXPIRE_TIME, cl_data->expire_time);
			}
			if (cl_data->uid != NULL && strlen(cl_data->uid) > 0) {
				cJSON_AddStringToObject(cloud, KEY_ID_USER, cl_data->uid);
			}
			if (cl_data->redirect_uri != NULL && strlen(cl_data->redirect_uri) > 0) {
				cJSON_AddStringToObject(cloud, KEY_SERVER_REDIRECT_URI, cl_data->redirect_uri);
			}
			if (cl_data->certificate != NULL && strlen(cl_data->certificate) > 0) {
				cJSON_AddStringToObject(cloud, KEY_CERTIFICATE_FILE, cl_data->certificate);
			}
			if (cl_data->sid != NULL && strlen(cl_data->sid) > 0) {
				cJSON_AddStringToObject(cloud, KEY_SERVER_ID, cl_data->sid);
			}
		}

		json_update = cJSON_Print(root);

		if (set_json_string_into_file(filename, json_update) == 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Store data to info file.");
			goto GOTO_OUT;
		}

		ret = 1;
	}							// End of if

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Success in \"%s\" file.", filename);

GOTO_OUT:
	if (root) {
		cJSON_Delete(root);
		root = NULL;
	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	if (json_update != NULL) {
		things_free(json_update);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int Parse_things_files(const char *info_Path)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
	int res = 1;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "orginal info file path = %s", info_Path);
	if (!parse_things_info_json(info_Path)) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Info file parsing Failed");
		res = 0;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
	return res;
}

int dm_load_legacy_cloud_data(es_cloud_signup_s **cl_data)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	int ret = 1;

	if (g_things_cloud_file_path[0] == 0 || cl_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Provisioning file path is NULL.(%s) or Invalid argument.(0x%X)", g_things_cloud_file_path, cl_data);
		return 0;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "g_things_cloud_file_path = %s", g_things_cloud_file_path);

	if ((ret = get_signup_data_from_json(g_things_cloud_file_path, cl_data)) != 1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not Get sign-up data failed from Provisioning file.");
	}

	usleep(500000);

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

	return ret;
}

const char *dm_get_things_device_type(int device_id)
{
	//THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
	st_device_s *device = NULL;

	device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)device_id);
	if (device == NULL) {

		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
		return NULL;
	}

	return device->type;
}

const char *dm_get_svrdb_file_path(void)
{
	return g_svrdb_file_path;
}

const char *dm_get_certificate_file_path(void)
{
	return g_certificate_file_path;
}

const char *dm_get_privatekey_file_path(void)
{
	return g_private_key_file_path;
}

const char *dm_get_things_cloud_address(char *customized_ci_server)
{
	if (customized_ci_server && customized_ci_server[0] != 0) {
		return customized_ci_server;
	}
	return get_cloud_server_address(customized_ci_server);
}

bool dm_is_rsc_published(void)
{
	return (es_get_state() == ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD ? true : false);
}

static things_resource_s *register_resource(things_server_builder_s *p_builder, struct things_resource_info_s *resource, const char *id)
{
	things_resource_s *ret = NULL;
	char res_uri[MAX_URI_LENGTH] = { 0 };

	if (NULL != resource) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "RESOURCE TO REGISTER : %s", resource->uri);

		memset(res_uri, 0, (size_t) MAX_URI_LENGTH);
		strncat(res_uri, resource->uri, MAX_URI_LENGTH);

		ret = p_builder->create_resource(p_builder, res_uri, resource->resource_types[0], resource->interface_types[0], CHECK_DISCOVERABLE(resource->policy), CHECK_OBSERVABLE(resource->policy), CHECK_SECURE(resource->policy));

		THINGS_LOG_D(THINGS_DEBUG, TAG, "add_resource_type : %s", resource->resource_types[0]);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "add_interface_type : %s", resource->interface_types[0]);
		for (int rt_num = 1; rt_num < resource->rt_cnt; rt_num++) {
			p_builder->add_resource_type(ret, resource->resource_types[rt_num]);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "add_resource_type : %s", resource->resource_types[rt_num]);
		}

		for (int it_num = 1; it_num < resource->if_cnt; it_num++) {
			p_builder->add_interface_type(ret, resource->interface_types[it_num]);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "add_interface_type : %s", resource->interface_types[it_num]);
		}
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
	}

	return ret;
}

static things_resource_s *register_device_resource(things_server_builder_s *p_builder, st_device_s *device, const char *id)
{
	things_resource_s *ret = NULL;
	char res_uri[MAX_URI_LENGTH] = { 0 };

	if (NULL != device) {
		memset(res_uri, 0, (size_t) MAX_URI_LENGTH);
		strncat(res_uri, OC_RSRVD_DEVICE_URI, MAX_URI_LENGTH);

		ret = p_builder->create_resource(p_builder, res_uri, OC_RSRVD_RESOURCE_TYPE_DEVICE, OC_RSRVD_INTERFACE_READ, 0, 0, 1);

		p_builder->add_resource_type(ret, device->type);

		ret->rep = things_create_representation_inst(NULL);
		if (ret->rep) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/d] name :%s", device->name);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/d] type :%s", device->type);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_DEVICE_NAME, device->name);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_DEVICE_ID, device->device_id);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_SPEC_VERSION, OC_SPEC_VERSION);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_DATA_MODEL_VERSION, OC_DATA_MODEL_VERSION);
		} else {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not able to create representation");
		}
	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
	}

	return ret;
}

static things_resource_s *register_platform_resource(things_server_builder_s *p_builder, st_device_s *device, const char *id)
{
	things_resource_s *ret = NULL;
	char res_uri[MAX_URI_LENGTH] = { 0 };

	if (NULL != device) {
		memset(res_uri, 0, (size_t) MAX_URI_LENGTH);
		strncat(res_uri, OC_RSRVD_PLATFORM_URI, MAX_URI_LENGTH);

		ret = p_builder->create_resource(p_builder, res_uri, OC_RSRVD_RESOURCE_TYPE_PLATFORM, OC_RSRVD_INTERFACE_READ, 0, 0, 1);

		ret->rep = things_create_representation_inst(NULL);
		if (ret->rep) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Manufacturer :%s", device->manufacturer_name);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Model Name :%s", device->model_num);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Ver. Plaform :%s", device->ver_p);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Ver. OS :%s", device->ver_os);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Ver. HW :%s", device->ver_hw);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Ver. FW :%s", device->ver_fw);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "[/oic/p] Ver. vid :%s", device->vender_id);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_PLATFORM_ID, device->device_id);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_MFG_NAME, device->manufacturer_name);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_MODEL_NUM, device->model_num);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_PLATFORM_VERSION, device->ver_p);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_OS_VERSION, device->ver_os);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_HARDWARE_VERSION, device->ver_hw);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_FIRMWARE_VERSION, device->ver_fw);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_MFG_URL, device->manufacturer_url);

			ret->rep->things_set_value(ret->rep, OC_RSRVD_VID, device->vender_id);
		} else {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not able to create representation");
		}

	} else {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
	}

	return ret;
}

long dm_get_num_of_dev_cnt(void)
{
	return hashmap_count(g_device_hmap);
}

st_device_s *dm_get_info_of_dev(unsigned long number)
{
	return (st_device_s *)hashmap_get(g_device_hmap, number);
}

static void dm_delete_user_define_device_id(void)
{
	int i = 0;

	for (i = 0; i < MAX_SUBDEVICE_EA; i++) {
		things_free(user_def_dev_list[i]);
		user_def_dev_list[i] = NULL;
	}
}

bool dm_register_device_id(void)
{
	int i = 0;
	char *id = NULL;
	st_device_s **dev_list = NULL;
	int device_cnt = (int)hashmap_count(g_device_hmap);
	if (device_cnt < 1) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "No Device Information to Register");
		return false;
	}

	if ((dev_list = (st_device_s **)things_malloc(sizeof(st_device_s *) * device_cnt)) == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "st_device_s mem allocation is failed.");
		return false;
	}
	// Set Main-Device ID
	id = OCGetServerInstanceIDString();
	dev_list[0] = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)0);

	if (id == NULL || dev_list[0] == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error : id = %s, Main-device =0x%X", id, dev_list[0]);
		goto GOTO_ERROR;
	}
	memcpy(dev_list[0]->device_id, id, strlen(id) + 1);

	// Set Sub-Device ID
	for (i = 1; i < device_cnt; i++) {
		dev_list[i] = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)i);
		if (dev_list[i] == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error : device[%d] =0x%X", i, dev_list[i]);
			goto GOTO_ERROR;
		}

		if (user_def_dev_list[i] != NULL || strlen(dev_list[i]->device_id) == 0) {
			// a. If logical device is in a hosting(physical) device.
			if (dev_list[i]->is_physical == 1) {
				OCRandomUuidResult idGenRet = RAND_UUID_CONVERT_ERROR;

				if (user_def_dev_list[i]) {
					THINGS_LOG_D(THINGS_DEBUG, TAG, "Set Sub-device ID with user-defined device ID.");
					memcpy(dev_list[i]->device_id, user_def_dev_list[i], strlen(user_def_dev_list[i]) + 1);
					idGenRet = RAND_UUID_OK;
				} else {
					THINGS_LOG_D(THINGS_DEBUG, TAG, "Set Sub-device ID with Auto-Generated device ID.");
					idGenRet = OCGenerateUuidString(dev_list[i]->device_id);
				}

				if (idGenRet != RAND_UUID_OK) {
					THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[DEVICE] Device ID Creation Failed : %d", idGenRet);
				} else {
					THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] ID Created : %s", (dev_list[i]->device_id));
				}
			}
			// b. If logical device is in the other physical device
			else {
				THINGS_LOG_D(THINGS_INFO, TAG, "[DEVICE] Not Physically Separated Device ");
			}
		}
	}

	is_support_user_def_dev_list = false;	//  It's allow permission to apply user defined device-ID only when start THINGS_STACK.

	dm_delete_user_define_device_id();
	things_free(dev_list);
	dev_list = NULL;
	return true;

GOTO_ERROR:
	things_free(dev_list);
	dev_list = NULL;
	return false;
}

int dm_register_resource(things_server_builder_s *p_builder)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	if (p_builder == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Builder instance is NULL");

		return 0;
	}

	int device_cnt = (int)hashmap_count(g_device_hmap);
	if (device_cnt < 1) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "No Device Information to Register");

		return 0;
	}

	for (int device_num = 0; device_num < device_cnt; device_num++) {
		char id[11] = { 0 };
		char res_uri[MAX_URI_LENGTH] = { 0 };
		int key = 0;
		int n_count_of_children = 0;

		st_device_s *device = NULL;
		struct things_resource_info_s *resource = NULL;
		struct col_resource_s *col_resource_s = NULL;
		struct things_resource_s *p_collection_resource = NULL;

		// 2. Device Capability Resources Registration
		THINGS_LOG_D(THINGS_DEBUG, TAG, "BEFORE SEARCHING THE DEVICE  ITEM FROM HASH TABLE");

		device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)device_num);

		if (NULL != device) {
			snprintf(id, sizeof(id), "%d", device->no);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "==================== Device (%s) ====================", id);

			if (device->col_cnt < 1) {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "NO COLLECTION & ITS CHILDREN RESOURCE(S)");
			} else {
				THINGS_LOG_D(THINGS_INFO, TAG, "COLLECTION CHILDREN RESOURCE(S) CNT : %d", device->col_cnt);

				memset(res_uri, 0, (size_t) MAX_URI_LENGTH);
				strncat(res_uri, device->collection[0].uri, MAX_URI_LENGTH);

				p_collection_resource = p_builder->create_collection_resource(p_builder, res_uri, device->collection[0].resource_types[0], device->collection[0].interface_types[0]);

				for (int rt_num = 1; rt_num < device->collection[0].rt_cnt; rt_num++) {
					OCBindResourceTypeToResource(p_builder, device->collection[0].resource_types[rt_num]);
				}

				// THINGS_LOG_D(THINGS_DEBUG, TAG, "IT TO BIND : %d", resource->if_cnt);
				for (int it_num = 1; it_num < device->collection[0].if_cnt; it_num++) {
					OCBindResourceInterfaceToResource(p_builder, device->collection[0].interface_types[it_num]);
				}

				THINGS_LOG_D(THINGS_DEBUG, TAG, "AFTER REGISTERGING DEVICE RESOURCE");

				if (p_collection_resource != NULL) {
					THINGS_LOG_D(THINGS_DEBUG, TAG, "DEVICE RESOURCE(S) CNT : %d", device->col_cnt);
					for (int capa_num = 0; capa_num < device->collection[0].link_cnt; capa_num++) {
						resource = device->collection[0].links[capa_num];
						device->pchild_resources[n_count_of_children++] = register_resource(p_builder, resource, id);
					}			// End of for children resource(s) registration loop
				}				// End of single device

				p_builder->bind_all(p_collection_resource, device->pchild_resources, n_count_of_children);

				device->pchild_resources[n_count_of_children++] = p_collection_resource;
			}

			THINGS_LOG_D(THINGS_DEBUG, TAG, "SINGLE RESOURCE(S) CNT : %d", device->sig_cnt);

			for (int capa_num = 0; capa_num < device->sig_cnt; capa_num++) {
				resource = &device->single[capa_num];
				device->pchild_resources[n_count_of_children++] = register_resource(p_builder, resource, id);
			}					// End of for single resource registration loop

			// Create the Device(/oic/d) and Platform (/oic/p)
			// ex) Aircon Multi-Model
			if (device_num < 1) {
				// for hosting device
				p_builder->set_device_info(p_builder, device->name, device->type);
				p_builder->set_platform_info(p_builder, device->model_num,	// gDeviceModel,
											 device->ver_p,	// gPlatformVersion,
											 device->ver_os,	// gOSVersion,
											 device->ver_hw,	// gHWVersions,
											 device->ver_fw,	// gFWVersions,
											 device->vender_id,	// gVenderId
											 device->manufacturer_name,	// manufacturer_name
											 device->manufacturer_url);	// manufacturer_url
			} else {
				if (device->is_physical == 1) {
					THINGS_LOG_V(THINGS_INFO, TAG, "It's Physically Separated Device : %s", device->device_id);
					// for those physically separated devices
					// a. /oic/d/# resource
					device->pchild_resources[n_count_of_children++] = register_device_resource(p_builder, device, id);
					// b. /oic/p/# resource
					device->pchild_resources[n_count_of_children++] = register_platform_resource(p_builder, device, id);
				} else {
					THINGS_LOG_V(THINGS_INFO, TAG, "It's Logical Device in One Physical Device : [%d]", device_num);
				}
			}

			// Total Size of the device->pChildresources : col_cnt + sig_cnt + 1(/device) + a(/oic/d & /oic/p)
			device->capa_cnt = n_count_of_children;
		}						// End of device NULL Check
	}							// End of for device loop
	THINGS_LOG_D(THINGS_DEBUG, TAG, "=====================================================");

	if (p_builder->broadcast_presence(p_builder, 20) == 1) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Broadcast Presence Failed.");
		return 0;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

	return 1;
}

struct things_resource_s *dm_get_resource_instance(const char *uri, const int id)
{
	things_resource_s *ret = NULL;

	int device_cnt = (int)hashmap_count(g_device_hmap);
	if (device_cnt > 0 && device_cnt > id) {
		st_device_s *device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)(id));

		if (device) {
			for (int index = 0; index < MAX_DEVICE_CAPABILTY_CNT; index++) {
				if (device->pchild_resources[index] == NULL) {
					THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Resource with URI : %s not exist !!!!", uri);
					break;
				} else if (strncmp(device->pchild_resources[index]->uri, uri, strlen(uri)) == 0) {
					THINGS_LOG_V(THINGS_DEBUG, TAG, "Found %s from device[%d] : %s!!!!", uri, id, device->device_id);

					char *sValue = NULL;

					device->pchild_resources[index]->rep->things_get_value(device->pchild_resources[index]->rep, OC_RSRVD_DEVICE_ID, &sValue);
					ret = device->pchild_resources[index];
					break;
				} else {
					THINGS_LOG_D(THINGS_DEBUG, TAG, "[%d] Resource URI : %s", index, device->pchild_resources[index]->uri);
				}
			}
		}
	}
	return ret;
}

int dm_get_device_information(int *cnt, st_device_s ***list)
{
	int ret = 0;

	int device_cnt = (int)hashmap_count(g_device_hmap);
	if (device_cnt > 0) {
		st_device_s **devices = NULL;
		if ((devices = (st_device_s **)things_malloc(sizeof(st_device_s *) * device_cnt)) == NULL) {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "st_device_s memory allocation is failed.");
			return ret;
		}

		for (int index = 0; index < device_cnt; index++) {
			devices[index] = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)index);
		}

		(*list) = devices;
		(*cnt) = device_cnt;
		ret = 1;
	}

	return ret;
}

int dm_update_things_cloud(es_cloud_signup_s *cl_data)
{
	int ret = 1;

	THINGS_LOG_V(THINGS_INFO, TAG, "RTOS Recived Signup Data domain[%s], address[%s], port [%s],  access_token[%s],  refresh_token[%s], token_type[%s],  " "uid[%s]", cl_data->domain, cl_data->address, cl_data->port, cl_data->access_token, cl_data->refresh_token, cl_data->token_type, cl_data->uid);
	THINGS_LOG_V(THINGS_INFO, TAG, "RTOS Recived Signup Data redirect_uri[%s],  certificate[%s], sid[%s]", cl_data->redirect_uri, cl_data->certificate, cl_data->sid);

	if (g_things_cloud_file_path[0] == 0 || cl_data == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud file path is NULL(%s). or Invalid argument.(0x%X)", g_things_cloud_file_path, cl_data);
		ret = 0;
		return ret;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "g_things_cloud_file_path = %s", g_things_cloud_file_path);

	if (update_things_cloud_json_by_cloud_signup(g_things_cloud_file_path, cl_data) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Provisioning file update failed.");
		ret = 0;
	}

	if (ret == 1) {
		int res = 0;
		es_cloud_signup_s *confirmData = NULL;

		if ((res = dm_load_legacy_cloud_data(&confirmData)) == 1) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Success.");
			is_there_cloud_data = true;
			es_cloud_signup_clear(confirmData);
			confirmData = NULL;
		} else {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Failed.(%d)", res);	// 1 : Success , 0/-1 : system error. , -2 : saved invalid Cloud-Data.
			is_there_cloud_data = false;
		}
	}

	return ret;
}

int dm_del_things_cloud_data(void)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	int ret = 1;
	if (g_things_cloud_file_path[0] == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud file path is NULL.");
		ret = 0;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "g_things_cloud_file_path = %s", g_things_cloud_file_path);

	if (update_things_cloud_json_by_cloud_signup(g_things_cloud_file_path, NULL) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "It's failed to delete Cloud-Data in cloud file.");
		ret = 0;
	}

	if (ret == 1) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Delete Success.");
		is_there_cloud_data = false;
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return ret;
}

bool dm_is_there_things_cloud(void)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "is_there_cloud_data = %d", is_there_cloud_data);
	return is_there_cloud_data;
}

int dm_validate_attribute_in_request(char *rt, const void *payload)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
	int ret = 0;

	if (rt != NULL && payload != NULL) {
		// 1. Get index with resource type
		unsigned long index = hashmap_get_hashval((unsigned char *)rt);
		OCRepPayload *r_payload = (OCRepPayload *)payload;

		// 2. Get resource type instance
		THINGS_LOG_D(THINGS_INFO, TAG, "Index : %u  rt : %s", index, rt);
		struct st_resource_type_s *res_type = (struct st_resource_type_s *)hashmap_get(g_resource_type_hmap, index);

		if (NULL != res_type) {
			int iter = 0;
			// 3. Get attributes & check
			//    POLICY
			//    a. Check exsitence of read-only attributes in the payload which should not be..
			for (iter = 0; iter < res_type->prop_cnt; iter++) {
				if (NULL != res_type->prop[iter]
					&& (1 == CHECK_READABLE(res_type->prop[iter]->rw)
						&& 1 != CHECK_WRITABLE(res_type->prop[iter]->rw))
				   ) {
					THINGS_LOG_D(THINGS_INFO, TAG, "\tCHECKING READ-ONLY ATTR : %s", res_type->prop[iter]->key);
					if (!OCRepPayloadIsNull(r_payload, res_type->prop[iter]->key)) {
						THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> EXIST");
						goto VALIDATION_EXIT;
					} else {
						THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> NOT EXIST");
					}
				}
			}
			//    b. Check existence of rw attribute in the payload which should exist at least 1
			for (iter = 0; iter < res_type->prop_cnt; iter++) {
				if (NULL != res_type->prop[iter]
					&& 1 == CHECK_WRITABLE(res_type->prop[iter]->rw)) {
					THINGS_LOG_D(THINGS_INFO, TAG, "\tCHECKING WRITE-ABLE ATTR : %s", res_type->prop[iter]->key);
					if (!OCRepPayloadIsNull(r_payload, res_type->prop[iter]->key)) {
						THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> EXIST");

						ret = 1;
						break;
					} else {
						THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> NOT EXIST");
					}
				}
			}
			//    c. Make it pass if it's collection resource
			if (0 == strncmp(rt, OC_RSRVD_RESOURCE_TYPE_COLLECTION, strlen(OC_RSRVD_RESOURCE_TYPE_COLLECTION))
				|| 0 == strncmp(rt, SEC_RTYPE_THINGS_DEVICE, strlen(SEC_RTYPE_THINGS_DEVICE))) {
				// TODO : Need to check whether there an array delivered or not..
				THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> Child %s", (r_payload->next != NULL ? "EXIST" : "NOT EXIST"));
				THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> Request On Collection Resource. Making it PASS");
				ret = 1;
			}
		} else {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Supporting rt : %s", rt);
		}
	} else {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Input - rt : %s, payload %d", rt, payload);
	}

VALIDATION_EXIT:

	if (ret == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> INVALID REQUEST~!!!");
	}
	return ret;
}

int things_get_resource_type(const char *resource_uri, int *count, char ***resource_types)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "things_get_resource_type : %s", resource_uri);
	int device_cnt = (int)hashmap_count(g_device_hmap);
	if (device_cnt > 0) {
		st_device_s *device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)0);

		if (device != NULL) {
			if (device->sig_cnt > 0) {
				for (int index = 0; index < device->sig_cnt; index++) {
					if (strncmp(device->single[index].uri, resource_uri, strlen(resource_uri)) == 0) {
						(*count) = device->single[index].rt_cnt;
						(*resource_types) = device->single[index].resource_types;
						return 1;
					}
				}
			}
			if (device->col_cnt > 0) {
				for (int index = 0; index < device->col_cnt; index++) {
					if (strncmp(device->collection[index].uri, resource_uri, strlen(resource_uri)) == 0) {
						(*count) = device->collection[index].rt_cnt;
						(*resource_types) = device->collection[index].resource_types;
						return 1;
					} else {
						for (int index_links = 0; index_links < device->collection[index].link_cnt; index_links++) {
							if (strncmp(device->collection[index].links[index_links]->uri, resource_uri, strlen(resource_uri)) == 0) {
								(*count) = device->collection[index].links[index_links]->rt_cnt;
								(*resource_types) = device->collection[index].links[index_links]->resource_types;
								return 1;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

const int dm_get_wifi_property_interface()
{
	return g_wifi_interface;
}

const wifi_freq_e dm_get_wifi_property_freq()
{
	return g_wifi_freq;
}

const int dm_get_ownership_transfer_method()
{
	THINGS_LOG_V(THINGS_INFO, TAG, "ownership_transfer_method : %d", g_ownership_transfer_method);
	return g_ownership_transfer_method;
}

int things_get_attributes_by_resource_type(const char *res_type, int *count, things_attribute_info_s ***attributes)
{
	int resource_type_cnt = (int)hashmap_count(g_resource_type_hmap);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "things_get_attributes_by_resource_type : %s, resource_type_cnt : %d", res_type, resource_type_cnt);

	if (resource_type_cnt > 0) {

		int index = hashmap_get_hashval(res_type);
		struct st_resource_type_s *res_type = (struct st_resource_type_s *)hashmap_get(g_resource_type_hmap, index);
		if (res_type == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "res_type Not Exist");
			return 0;
		}

		THINGS_LOG_D(THINGS_DEBUG, TAG, "res_type : %s, res_type : %s", res_type->rt, res_type);
		if (strncmp(res_type->rt, res_type, strlen(res_type)) == 0) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "res_type->prop_cnt : %d", res_type->prop_cnt);
			(*count) = res_type->prop_cnt;
			(*attributes) = res_type->prop;
			return 1;
		}
	}
	return 0;
}

bool things_is_collection_resource(const char *res_uri)
{
	int device_cnt = (int)hashmap_count(g_device_hmap);
	if (device_cnt > 0) {
		st_device_s *device = NULL;

		device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)0);
		if (device == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
			return NULL;
		}

		if (device->col_cnt > 0) {
			for (int index = 0; index < device->col_cnt; index++) {
				if (strncmp(device->collection[index].uri, res_uri, strlen(res_uri)) == 0) {
					return true;
				}
			}
		}
	}
	return false;
}

int things_get_child_resources(const char *col_res_uri, int *count, things_resource_info_s ***child_resources)
{
	st_device_s *device = NULL;

	device = (st_device_s *)hashmap_get(g_device_hmap, (unsigned long)0);
	if (device == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
		return NULL;
	}

	if (device->col_cnt > 0) {
		for (int index = 0; index < device->col_cnt; index++) {
			if (strncmp(device->collection[index].uri, col_res_uri, strlen(col_res_uri)) == 0) {
				(*count) = device->collection[index].link_cnt;
				(*child_resources) = device->collection[index].links;
				return 1;
			}
		}
	}
	return 0;
}

int dm_init_module(const char *devJsonPath)
{
	if (devJsonPath == NULL) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "devJsonPath is NULL");
		return 0;
	}

	memset(g_svrdb_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
	memset(g_things_cloud_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
	memset(g_cloud_address, 0, (size_t) MAX_CLOUD_ADDRESS);

	return Parse_things_files(devJsonPath);
}

int dm_termiate_module()
{
	//  Need to backup all the keys in order to delete from the map.
	hashmap_delete(g_device_hmap);
	hashmap_delete(g_resource_type_hmap);
	return 1;
}
