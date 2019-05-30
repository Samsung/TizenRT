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
#include "easy-setup/easysetup.h"
#include "easy-setup/easysetup_manager.h"
#include "ocstackconfig.h"
#include "ocrandom.h"
#include "things_def.h"
#include "things_iotivity_lock.h"
#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_util.h"
#include "utils/things_hashmap.h"
#include "utils/things_string.h"
#include "things_resource.h"
#include "things_data_manager.h"
#include "things_sss_manager.h"
#include "things_server_builder.h"
#include "cloud/cloud_manager.h"
#include "cloud/cloud_connector.h"
#include "things_types.h"
#include <wifi_manager/wifi_manager.h>


#define TAG "[things_datamgr]"

/* device define JSON */
#define KEY_DEVICE                                              "device"
#define KEY_DEVICE_SPECIFICATION                                "specification"

/* oic.d */
#define KEY_DEVICE_SPECIFICATION_DEVICE                         "device"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DEVICETYPE              "deviceType"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DEVICENAME              "deviceName"
#define KEY_DEVICE_SPECIFICATION_DEVICE_SPECVERSION             "specVersion"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DATAMODELVERSION        "dataModelVersion"

/* oic.p */
#define KEY_DEVICE_SPECIFICATION_PLATFORM                       "platform"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MNID                  "MNID"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERNAME      "manufacturerName"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERURL       "manufacturerUrl"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURINGDATE     "manufacturingDate"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MODELNUMBER           "modelNumber"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_PLATFORMVERSION       "platformVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_OSVERSION             "osVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_HARDWAREVERSION       "hardwareVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_FIRMWAREVERSION       "firmwareVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_VENDORID              "vendorId"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_VID                   "VID"

/* resources */
#define KEY_RESOURCES                                           "resources"
#define KEY_RESOURCES_SIG                                       "single"
#define KEY_DEVICE_RESOURCE_URI                                 "uri"
#define KEY_DEVICE_RESOURCE_TYPES                               "types"
#define KEY_DEVICE_RESOURCE_INTERFACES                          "interfaces"
#define KEY_DEVICE_RESOURCE_POLICY                              "policy"

/* resourceTypes */
#define KEY_RESOURCES_TYPE                                      "resourceTypes"
#define KEY_DEVICE_RESOURCETYPE_TYPE                            "type"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES                      "properties"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_KEY                  "key"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_TYPE                 "type"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_MANDATORY            "mandatory"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_RW                   "rw"

/* configuration */
#define KEY_CONFIGURATION                                       "configuration"
#define KEY_CONFIGURATION_EASYSETUP                             "easySetup"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY                "connectivity"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_TYPE           "type"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP         "softAP"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_SETUPID "setupId"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_ARTIK   "artik"
#define KEY_CONFIGURATION_EASYSETUP_OWNERSHIP                   "ownershipTransferMethod"
#define KEY_CONFIGURATION_WIFI                                  "wifi"
#define KEY_CONFIGURATION_WIFI_INTERFACES                       "interfaces"
#define KEY_CONFIGURATION_WIFI_FREQUENCY                        "frequency"
#define KEY_CONFIGURATION_FILEPATH                              "filePath"
#define KEY_CONFIGURATION_FILEPATH_SVRDB                        "svrdb"
#define KEY_CONFIGURATION_FILEPATH_PROVISIONING                 "provisioning"

#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
#define KEY_CONFIGURATION_FILEPATH_CERTIFICATE                  "certificate"
#define KEY_CONFIGURATION_FILEPATH_PRIVATEKEY                   "privateKey"
#endif

/* provisioning info */
#define KEY_CLOUD                                               "cloud"
#define KEY_CLOUD_DOMAIN                                        "domain"
#define KEY_CLOUD_ADDRESS                                       "address"
#define KEY_CLOUD_PORT                                          "port"
#ifdef CONFIG_ST_THINGS_COLLECTION
/* collection resource */
#define KEY_RESOURCES_COL                                       "collection"
#define KEY_DEVICE_RESOURCE_COLLECTION_LINKS                    "links"
#endif

/* validate_attribute_in_request */
#define CHECK_BIT(var, pos)         (((var)>>(pos)) & 1)

#define CHECK_READABLE(var)         CHECK_BIT(var, 0)
#define CHECK_WRITABLE(var)         CHECK_BIT(var, 1)

#define CHECK_DISCOVERABLE(var)     CHECK_BIT(var, 0)
#define CHECK_OBSERVABLE(var)       CHECK_BIT(var, 1)
#define CHECK_SECURE(var)           CHECK_BIT(var, 2)

/* etc */
#define MAX_ATTRIBUTE_LENGTH            (64)
#define MAX_CLOUD_ADDRESS               (256)	//   Need to match with the Cloud Spec.
#define MAX_CLOUD_SESSIONKEY            (128)	//   Need to match with the Cloud Spec.
#define MAX_SOFTAP_SSID                 (32)
#define MAX_SSID_DEVICE_NAME            (13)

#ifdef CONFIG_ST_THINGS_SECURESTORAGE
#define SECURESTOARGE_MAX_DATA_SIZE	       (208)
#define SECURESTOARGE_MAX_CERT_SIZE	       (2048)
#define SECURESTOARGE_CERT_INDEX           (1)
#define SECURESTOARGE_EASYSETUP_INDEX      (2)
#define SECURESTOARGE_CLOUD_DATA_INDEX_1   (3)
#define SECURESTOARGE_CLOUD_DATA_INDEX_2   (4)
#endif

typedef int8_t INT8;

static volatile int g_resource_type_cnt = 0;

static char g_things_cloud_file_path[MAX_FILE_PATH_LENGTH + 1] = { 0 };
static char g_svrdb_file_path[MAX_FILE_PATH_LENGTH + 1] = { 0 };
#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
static char g_certificate_file_path[MAX_FILE_PATH_LENGTH + 1] = { 0 };
static char g_private_key_file_path[MAX_FILE_PATH_LENGTH + 1] = { 0 };
#endif

static char g_cloud_address[MAX_CLOUD_ADDRESS] = { 0 };

static st_device_s *g_device;
static char *g_firmware_version;
static char *g_model_number;

static char *g_setup_id;
static bool is_artik;
static char g_easysetup_softap_ssid[MAX_SOFTAP_SSID + 1];
static const char *g_easysetup_softap_passphrase = "1111122222";
static const char *g_easysetup_tag = "E1";
static const int g_easysetup_softap_channel = 1;

static int g_wifi_interface;
static wifi_freq_e g_wifi_freq;

static int g_ownership_transfer_method = 0;

static struct hashmap_s *g_resource_type_hmap = NULL;	// map for resource types

static const char *origin_cloud_json_str = "{\n\
		\"cloud\":    {\n\
			\"address\":    \"\"\n\
		}\n\
}";

static wifi_manager_softap_config_s ap_config;
static easysetup_connectivity_type_e es_conn_type = es_conn_type_none;

static const char *internal_resource_json_str = "{\n\
		\"resources\": {\n\
			\"single\": [\n\
				{\n\
					\"uri\": \"/sec/provisioninginfo\",\n\
					\"types\": [\n\
						\"x.com.samsung.provisioninginfo\"\n\
					],\n\
					\"interfaces\": [\n\
						\"oic.if.a\"\n\
					],\n\
					\"policy\": 3\n\
				},\n\
				{\n\
					\"uri\": \"/sec/accesspointlist\",\n\
					\"types\": [\n\
						\"x.com.samsung.accesspointlist\"\n\
					],\n\
					\"interfaces\": [\n\
						\"oic.if.s\"\n\
					],\n\
					\"policy\": 3\n"
#ifdef CONFIG_ST_THINGS_FOTA
				"},\n\
				{\n\
					\"uri\": \"/firmware\",\n\
					\"types\": [\n\
						\"x.com.samsung.firmware\"\n\
					],\n\
					\"interfaces\": [\n\
						\"oic.if.baseline\"\n\
					],\n\
					\"policy\": 3\n"
#endif
				"}\n\
			]\n\
		},\n\
		\"resourceTypes\": [\n\
			{\n\
				\"type\": \"x.com.samsung.provisioninginfo\",\n\
				\"properties\": [\n\
					{\n\
						\"key\": \"x.com.samsung.provisioning.targets\",\n\
						\"type\": 9,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"x.com.samsung.provisioning.owned\",\n\
						\"type\": 0,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"x.com.samsung.provisioning.easysetupdi\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"x.com.samsung.provisioning.reset\",\n\
						\"type\": 0,\n\
						\"mandatory\": false,\n\
						\"rw\": 3\n\
					},\n\
					{\n\
						\"key\": \"x.com.samsung.provisioning.stopAP\",\n\
						\"type\": 0,\n\
						\"mandatory\": false,\n\
						\"rw\": 3\n\
					}\n\
				]\n"
#ifdef CONFIG_ST_THINGS_FOTA
			"},\n\
			{\n\
				\"type\": \"x.com.samsung.firmware\",\n\
				\"properties\": [\n\
					{\n\
						\"key\": \"version\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"newversion\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 3\n\
					},\n\
					{\n\
						\"key\": \"packageuri\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 3\n\
					},\n\
					{\n\
						\"key\": \"update\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 3\n\
					},\n\
					{\n\
						\"key\": \"vender\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"model\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"state\",\n\
						\"type\": 1,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"result\",\n\
						\"type\": 1,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					},\n\
					{\n\
						\"key\": \"updatetime\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 3\n\
					},\n\
					{\n\
						\"key\": \"supportprotocols\",\n\
						\"type\": 3,\n\
						\"mandatory\": false,\n\
						\"rw\": 1\n\
					}\n\
				]\n"
#endif
			"},\n\
			{\n\
				\"type\": \"x.com.samsung.accesspointlist\",\n\
				\"properties\": [\n\
					{\n\
						\"key\": \"x.com.samsung.accesspoint.items\",\n\
						\"type\": 9,\n\
						\"mandatory\": true,\n\
						\"rw\": 1\n\
					}\n\
				]\n\
			}\n\
		]\n\
	}";

static struct things_attribute_info_s *create_property(void)
{
	struct things_attribute_info_s *property = things_malloc(sizeof(things_attribute_info_s));

	memset(property->key, 0, (size_t) MAX_KEY_LENGTH);
	property->type = 0;
	property->mandatory = false;
	property->rw = 0;

	return property;
}

static struct st_resource_type_s *create_resource_type(void)
{
	struct st_resource_type_s *type = things_malloc(sizeof(st_resource_type_s));

	if (type == NULL) {
		THINGS_LOG_E(TAG, "Failed to create_resource_type");
		return NULL;
	}

	memset(type->rt, 0, (size_t) MAX_ATTRIBUTE_LENGTH);
	memset(type->prop, 0, (size_t)(sizeof(things_attribute_info_s*) * MAX_PROPERTY_CNT));
	type->prop_cnt = 0;

	return type;
}

static void delete_resource_type(st_resource_type_s *type)
{
	if (type == NULL) {
		return;
	}

	int count = type->prop_cnt;
	for (int i = 0; i < count; ++i) {
		things_free(type->prop[i]);
	}
	things_free(type);
}

static void delete_resource_type_map(void)
{
	if (g_resource_type_hmap == NULL) {
		return;
	}

	unsigned long *keyset = hashmap_get_keyset(g_resource_type_hmap);
	long key_cnt = hashmap_count(g_resource_type_hmap);
	for (long type_iter = 0; type_iter < key_cnt; ++type_iter) {
		st_resource_type_s *type = hashmap_get(g_resource_type_hmap, keyset[type_iter]);
		delete_resource_type(type);
	}
	hashmap_delete(g_resource_type_hmap);
	g_resource_type_hmap = NULL;
	g_resource_type_cnt = 0;
	things_free(keyset);
}

static struct things_resource_info_s *create_resource(void)
{
	struct things_resource_info_s *resource = things_malloc(sizeof(things_resource_info_s));

	if (resource == NULL) {
		THINGS_LOG_E(TAG, "Failed to create_resource");
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

static st_device_s *create_device(void)
{
	st_device_s *device = things_malloc(sizeof(st_device_s));

	if (device == NULL) {
		THINGS_LOG_E(TAG, "Failed to create_device");
		return NULL;
	}

	device->type = NULL;
	device->name = NULL;
	device->mnid = NULL;
	device->manufacturer_url = NULL;
	device->manufacturing_date = NULL;
	device->model_num = NULL;
	device->ver_p = NULL;
	device->ver_os = NULL;
	device->ver_hw = NULL;
	device->ver_fw = NULL;
	device->device_id = NULL;
	device->vid = NULL;

	device->no = -1;
	device->capa_cnt = 0;
	device->col_cnt = 0;
	device->sig_cnt = 0;
	device->is_physical = 0;
#ifdef CONFIG_ST_THINGS_COLLECTION
	device->collection = NULL;
#endif
	device->single = NULL;
	device->pchild_resources = NULL;

	return device;
}

static void delete_resource_info(things_resource_info_s *resource)
{
	if (resource == NULL) {
		return;
	}

	// De-allocate resource types.
	int rt_count = resource->rt_cnt;
	for (int i = 0; i < rt_count; ++i) {
		things_free(resource->resource_types[i]);
	}
	// De-allocate interface types.
	int if_count = resource->if_cnt;
	for (int i = 0; i < if_count; ++i) {
		things_free(resource->interface_types[i]);
	}
}

static void delete_device(st_device_s *device)
{
	if (device != NULL) {
		things_free(device->type);
		things_free(device->name);
		things_free(device->mnid);
		things_free(device->manufacturer_url);
		things_free(device->manufacturing_date);
		things_free(device->model_num);
		things_free(device->ver_p);
		things_free(device->ver_os);
		things_free(device->ver_hw);
		things_free(device->ver_fw);
		things_free(device->device_id);
		things_free(device->vid);
#ifdef CONFIG_ST_THINGS_COLLECTION
		// Delete collection resources.
		int col_count = device->col_cnt;
		for (int col_iter = 0; col_iter < col_count; ++col_iter) {
			// Delete resource types & interface types.
			col_resource_s *rsrc = &device->collection[i];
			if (rsrc == NULL) {
				continue;
			}
			int rt_count = rsrc->rt_cnt;
			for (int j = 0; j < rt_count; ++j) {
				things_free(rsrc->resource_types[j]);
			}

			int if_count = rsrc->if_cnt;
			for (int j = 0; j < if_count; ++j) {
				things_free(rsrc->interface_types[j]);
			}

			// Delete links.
			int link_count = rsrc->link_cnt;
			for (int link_iter = 0; link_iter < link_count; ++link_iter) {
				delete_resource_info(rsrc->links[link_iter]);
				things_free(rsrc->links[link_iter]);
			}
			things_free(rsrc->links);
		}
		things_free(device->collection);
#endif
		// Delete single resources.
		int single_count = device->sig_cnt;
		for (int i = 0; i < single_count; ++i) {
			delete_resource_info(&device->single[i]);
		}
		things_free(device->single);
		things_free(device);
	}
}

size_t get_json_file_size(const char *filename)
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

char *get_json_string_from_file(const char *filename)
{
	FILE *fp = NULL;
	char *json_str = NULL;
	size_t size = get_json_file_size(filename);
	size_t read = 0;
	int fp_acess_cnt = 0;

	if (size == 0) {
		THINGS_LOG_E(TAG, "Failed converting to JSON");
		return NULL;
	} else {
		THINGS_LOG_D(TAG, "File size: %d ", size);
	}

	json_str = (char *)things_malloc(size + 1);
	if (json_str == NULL) {
		THINGS_LOG_E(TAG, "Failed to allocate memory(%d).", size + 1);
		return NULL;
	}

	// 1. File Read
	fp = fopen(filename, "r");
	if (NULL == fp) {
		THINGS_LOG_V(TAG, "Failed to open %s", filename);
		things_free(json_str);
		return NULL;
	}
	// 2. Json String read from the given file.
	while (read < size && fp_acess_cnt < MAX_FILE_ACCESS_CNT) {
		read += fread(&(json_str[read]), 1, size - read, fp);
		THINGS_LOG_D(TAG, "Read Size: %d, total Size =%d", read, size);
		fp_acess_cnt++;
	}

	if (fp_acess_cnt >= MAX_FILE_ACCESS_CNT) {
		THINGS_LOG_V(TAG, "Access-Times is Over for File Read. (Readed Size: %d, total Size =%d)", read, size);
		things_free(json_str);
		fclose(fp);
		return NULL;
	}

	json_str[read] = '\0';

	fclose(fp);
	fp = NULL;

	return json_str;
}

#ifdef CONFIG_ST_THINGS_SECURESTORAGE
char *get_json_string_from_securestorage(void)
{
	cJSON *root = NULL;
	int ret = 1;
	char *empty = "";
	char *json_str = NULL;
	unsigned char read_data[SECURESTOARGE_MAX_DATA_SIZE * 2];
	unsigned int read_size[2] = {SECURESTOARGE_MAX_DATA_SIZE, SECURESTOARGE_MAX_DATA_SIZE};
	int err;
	if ((err = see_read_secure_storage(read_data, &read_size[0], SECURESTOARGE_CLOUD_DATA_INDEX_1)) != 0) {
		THINGS_LOG_V(TAG, "see_read_secure_storage error (%d)", err);
		read_size[0] = 0;
		ret = 0;
		goto GOTO_OUT;
	}
	if ((err = see_read_secure_storage(read_data + read_size[0], &read_size[1], SECURESTOARGE_CLOUD_DATA_INDEX_2)) != 0) {
		THINGS_LOG_V(TAG, "see_read_secure_storage error (%d)", err);
		read_size[1] = 0;
		ret = 0;
		goto GOTO_OUT;
	}
	int size = read_size[0] + read_size[1];
	read_data[size + 1] = 0;

	unsigned char cert_data[SECURESTOARGE_MAX_CERT_SIZE];
	unsigned int cert_size = SECURESTOARGE_MAX_CERT_SIZE;
	if ((err = see_get_certificate(cert_data, &cert_size, SECURESTOARGE_CERT_INDEX)) != 0) {
		THINGS_LOG_E(TAG, "see_get_certificate error (%d)", err);
		cert_size = 0;
	}
	cert_data[cert_size + 1] = 0;

	root = cJSON_Parse(read_data);
	if (root == NULL) {
		THINGS_LOG_E(TAG, "root cJSON is NULL.");
		ret = 0;
		goto GOTO_OUT;
	}
	cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
	if (cloud == NULL) {
		THINGS_LOG_E(TAG, "cloud cJSON is NULL.");
		ret = 0;
		goto GOTO_OUT;
	}
	if (cert_size > 0) {
		cJSON_AddStringToObject(cloud, KEY_CERTIFICATE_FILE, cert_data);
	}
	json_str = cJSON_Print(root);

	THINGS_LOG_V(TAG, "get_json_string_from_securestorage success");
	ret = 1;
GOTO_OUT:
	if (ret == 0) {
		json_str = things_strdup(origin_cloud_json_str);
	}

	THINGS_LOG_V(TAG, "get_json_string_from_securestorage [%s]", json_str);

	if (root != NULL) {
		cJSON_Delete(root);
	}
	return json_str;
}

int set_json_string_into_securestorage(const char *json_str)
{
	int ret = 0;
	cJSON *root = cJSON_Parse((const char *)json_str);
	cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
	if (cloud == NULL) {
		THINGS_LOG_E(TAG, "cloud cJSON is NULL.");
		goto GOTO_OUT;
	}

	cJSON *certificate = cJSON_GetObjectItem(cloud, KEY_CERTIFICATE_FILE);
	int err;
	if (certificate != NULL && certificate->valuestring != NULL) {
		THINGS_LOG_V(TAG, "json_print:cert [%s] size : %d", certificate->valuestring, strlen(certificate->valuestring));
		if ((err = see_set_certificate(certificate->valuestring, strlen(certificate->valuestring), SECURESTOARGE_CERT_INDEX, 0)) != 0) {
			THINGS_LOG_E(TAG, "see_set_certificate error (%d)", err);
			goto GOTO_OUT;
		}
	} else {
		THINGS_LOG_D(TAG, "certificate == NULL || json->valuestring == NULL");
		unsigned char *empty = "";
		if ((err = see_set_certificate(empty, 1, SECURESTOARGE_CERT_INDEX, 0)) != 0) {
			THINGS_LOG_V(TAG, "see_set_certificate error (%d)", err);
			goto GOTO_OUT;
		}
	}
	cJSON_DeleteItemFromObject(cloud, KEY_CERTIFICATE_FILE);

	char* json_print = cJSON_Print(root);
	if (json_print == NULL) {
		THINGS_LOG_E(TAG, "json_print is NULL");
		goto GOTO_OUT;
	}

	int json_print_len = strlen(json_print);
	THINGS_LOG_V(TAG, "json_print size : %d[%s] ", json_print, json_print_len);
	int len = 0;
	if (json_print_len > SECURESTOARGE_MAX_DATA_SIZE) {
		len = SECURESTOARGE_MAX_DATA_SIZE;
	} else {
		len = json_print_len;
	}

	see_write_secure_storage(json_print, len, SECURESTOARGE_CLOUD_DATA_INDEX_1);
	if (len < SECURESTOARGE_MAX_DATA_SIZE) {
		unsigned char *empty = "";
		see_write_secure_storage(empty, 1, SECURESTOARGE_CLOUD_DATA_INDEX_2);
	} else {
		see_write_secure_storage(json_print + SECURESTOARGE_MAX_DATA_SIZE, json_print_len - SECURESTOARGE_MAX_DATA_SIZE, SECURESTOARGE_CLOUD_DATA_INDEX_2);
	}
	THINGS_LOG_V(TAG, "set_json_string_into_securestorage success");
	ret = 1;
GOTO_OUT:
	if (root != NULL) {
		cJSON_Delete(root);
	}

	if (json_print != NULL) {
		things_free(json_print);
	}
	return ret;
}
#endif

int set_json_string_into_file(const char *filename, const char *json_str)
{
	FILE *fp = NULL;
	// 1. File Read
	fp = fopen(filename, "w");
	if (NULL == fp) {
		THINGS_LOG_V(TAG, "Failed to open \"%s\" with Write permission.", filename);
		return 0;
	}
	// 2. Json String read from the given file.
	fwrite(json_str, 1, strlen(json_str), fp);

	fclose(fp);
	fp = NULL;
	return 1;
}

static int get_json_int(cJSON *json, int64_t *variable)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int res = 1;

	if (json == NULL || variable == NULL) {
		THINGS_LOG_V(TAG, "json = %d, variable = %d", json, variable);
		return 0;
	}

	if (json->type != cJSON_Number) {
		THINGS_LOG_V(TAG, "json-value Type is Not String value.");
		return 0;
	}

	*variable = json->valueint;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return res;
}

static int get_json_string(cJSON *json, char **variable)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int res = 1;
	int length = 0;

	if (json == NULL || variable == NULL || *variable != NULL) {
		THINGS_LOG_V(TAG, "There is NULL value.(json = %d, variable = %d)", json, variable);
		if (variable != NULL) {
			THINGS_LOG_V(TAG, "variable value is not NULL.(*variable = %d)", *variable);
		}
		return 0;
	}
	if ((length = strlen(json->valuestring)) == 0) {
		THINGS_LOG_V(TAG, "length is NULL");
		return 0;
	}
	*variable = (char *)things_malloc(sizeof(char) * (length + 1));
	if (*variable == NULL) {
		THINGS_LOG_V(TAG, "memory allocation is failed.");
		return 0;
	}
	memset(*variable, 0, length + 1);
	memcpy(*variable, json->valuestring, length + 1);
	THINGS_LOG_D(TAG, "(variable = %s)", json->valuestring);
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return res;
}

static int load_cloud_signup_data(cJSON *json, es_cloud_signup_s **cl_data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int res = 1;
	if (json == NULL || cl_data == NULL) {
		THINGS_LOG_V(TAG, "json = %d, cl_data = %d", json, cl_data);
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
		THINGS_LOG_V(TAG, "address = %d, accesstoken = %d, refresh_accesstoken = %d, user_id = %d, expire_time = %d", address, accesstoken, refresh_accesstoken, user_id, expire_time);
		return -2;
	}
	if (es_cloud_signup_init(cl_data) == false) {
		THINGS_LOG_V(TAG, "ESCloud data initialization is failed.");
		return -1;
	}
	if (get_json_string(address, &((*cl_data)->address)) == 0) {
		THINGS_LOG_V(TAG, "Load JSON String address is failed");
		res = -1;
	}
	if (get_json_string(accesstoken, &((*cl_data)->access_token)) == 0) {
		THINGS_LOG_V(TAG, "Load JSON String accesstoken is failed");
		res = -1;
	}
	if (get_json_string(refresh_accesstoken, &((*cl_data)->refresh_token)) == 0) {
		THINGS_LOG_V(TAG, "Load JSON String refresh_accesstoken is failed");
		res = -1;
	}
	if (get_json_string(user_id, &((*cl_data)->uid)) == 0) {
		THINGS_LOG_V(TAG, "Load JSON String user_id is failed");
		res = -1;
	}
	if (get_json_int(expire_time, &((*cl_data)->expire_time)) == 0) {
		THINGS_LOG_V(TAG, "Load JSON String expire_time is failed");
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
		THINGS_LOG_D(TAG, "CLOUD CI Address : %s", g_cloud_address);
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return res;
}

easysetup_connectivity_type_e dm_get_easysetup_connectivity_type(void)
{
	return es_conn_type;
}

wifi_manager_softap_config_s *dm_get_softap_wifi_config(void)
{
	st_device_s *device = dm_get_info_of_dev(0);
	int ssid_type = (is_artik == true ? 1 : 0);
	unsigned char ext_value[9] = { 0, };
	char ssid_device_name[MAX_SSID_DEVICE_NAME + 1];

	memset(ext_value, 0, sizeof(ext_value));
	memset(ssid_device_name, 0, sizeof(ssid_device_name));

	snprintf(ssid_device_name, sizeof(ssid_device_name), "%s", device->name);
	THINGS_LOG_V(TAG, "[softap] DeviceName : %s", ssid_device_name);

	wifi_manager_info_s st_wifi_info;
	wifi_manager_get_info(&st_wifi_info);

#if defined(CONFIG_ST_THINGS_HW_CERT_KEY) && defined(CONFIG_TLS_WITH_SSS)
	if (is_artik) {
		if (!things_encrypt_artik_uuid(ext_value)) {
			THINGS_LOG_E(TAG, "Fail to encrypt artik uuid");
			return NULL;
		}
	} else
#endif
	{
		snprintf((char *)ext_value, sizeof(ext_value), "%02X%02X", st_wifi_info.mac_address[4], st_wifi_info.mac_address[5]);
	}

	snprintf(g_easysetup_softap_ssid, sizeof(g_easysetup_softap_ssid), "%s_%s%s%s%d%s", ssid_device_name, g_easysetup_tag, g_device->mnid, g_setup_id, ssid_type, ext_value);
	THINGS_LOG_V(TAG, "SoftAP SSID : %s", g_easysetup_softap_ssid);

	snprintf(ap_config.ssid, sizeof(ap_config.ssid), "%s", g_easysetup_softap_ssid);
	snprintf(ap_config.passphrase, sizeof(ap_config.passphrase), "%s", g_easysetup_softap_passphrase);
	ap_config.channel = g_easysetup_softap_channel;

	return &ap_config;
}

int parse_things_cloud_json(const char *filename)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ret = 0;
	char *json_str = NULL;
#ifdef CONFIG_ST_THINGS_SECURESTORAGE
	json_str = get_json_string_from_securestorage();
#else
	json_str = get_json_string_from_file(filename);
#endif

	if (json_str == NULL) {
		THINGS_LOG_V(TAG, "cloud file initialization.");
#ifdef CONFIG_ST_THINGS_SECURESTORAGE
		if (set_json_string_into_securestorage(origin_cloud_json_str) == 0) {
			THINGS_LOG_V(TAG, "[Error] Creating cloud file is failed.");
			return 0;
		}
#else
		if (set_json_string_into_file(filename, origin_cloud_json_str) == 0) {
			THINGS_LOG_V(TAG, "[Error] Creating cloud file is failed.");
			return 0;
		}
#endif
	} else {
		if (strlen(json_str) > 0) {
			cJSON *root = cJSON_Parse((const char *)json_str);
			if (root != NULL) {
				cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
				if (cloud != NULL) {
					cJSON *address = cJSON_GetObjectItem(cloud, KEY_CLOUD_ADDRESS);
					if (address != NULL) {
						memset(g_cloud_address, 0, (size_t) MAX_CLOUD_ADDRESS);
						memcpy(g_cloud_address, address->valuestring, strlen(address->valuestring) + 1);
						THINGS_LOG_D(TAG, "[CLOUD] CI Address : %s", g_cloud_address);
						ret = 1;
					}
				}
				cJSON_Delete(root);
			}
		}
		things_free(json_str);
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int parse_configuration_json(cJSON *configuration)
{
	int ret = 0;
	int connectivity_type = 0;

	if (configuration == NULL) {
		goto JSON_ERROR;
	}

	cJSON *easysetup = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_EASYSETUP);
	if (easysetup == NULL) {
		THINGS_LOG_V(TAG, "[configuration] easySetup is null");
		goto JSON_ERROR;
	}

	cJSON *connectivity = cJSON_GetObjectItem(easysetup, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY);
	if (connectivity == NULL) {
		THINGS_LOG_V(TAG, "[configuration] connectivity is null");
		goto JSON_ERROR;
	}

	cJSON *type = cJSON_GetObjectItem(connectivity, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_TYPE);
	if (type == NULL) {
		THINGS_LOG_V(TAG, "[configuration] type is null");
		goto JSON_ERROR;
	}
	connectivity_type = type->valueint;
	THINGS_LOG_V(TAG, "[configuration] type       : %d", connectivity_type);

	if (connectivity_type == 1) {
		es_conn_type = es_conn_type_softap;
		cJSON *softap = cJSON_GetObjectItem(connectivity, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP);
		if (softap == NULL) {
			THINGS_LOG_V(TAG, "[configuration] softAp is null");
			goto JSON_ERROR;
		}
		cJSON *setup_id = cJSON_GetObjectItem(softap, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_SETUPID);
		cJSON *artik = cJSON_GetObjectItem(softap, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_ARTIK);
		if (setup_id == NULL) {
			THINGS_LOG_V(TAG, "[configuration] setup_id is null");
			goto JSON_ERROR;
		}
		if (artik == NULL) {
			THINGS_LOG_V(TAG, "[configuration] artik is null");
			goto JSON_ERROR;
		}

		if (strlen(setup_id->valuestring) != 3) {
			THINGS_LOG_V(TAG, "setup_id exceeds 3 bytes. please check (3 bytes are fixed sizes.)");
			goto JSON_ERROR;
		}
		is_artik = false;
		if (artik->type == cJSON_True) {
			is_artik = true;
		}

		THINGS_LOG_V(TAG, "[configuration] setup_id : %s / artik : %d", setup_id->valuestring, is_artik);

		g_setup_id = things_malloc(sizeof(char) * strlen(setup_id->valuestring) + 1);
		strncpy(g_setup_id, setup_id->valuestring, strlen(setup_id->valuestring) + 1);
	} else if (connectivity_type == 2) {
		//TO DO
		es_conn_type = es_conn_type_ble;
	} else {
		THINGS_LOG_V(TAG, "[configuration] connectivity_type is unknown");
		goto JSON_ERROR;
	}


	cJSON *ownership_transfer_method = cJSON_GetObjectItem(easysetup, KEY_CONFIGURATION_EASYSETUP_OWNERSHIP);
	if (ownership_transfer_method == NULL) {
		THINGS_LOG_V(TAG, "ownershipTransferMethod is NULL");
		goto JSON_ERROR;
	}

	g_ownership_transfer_method = ownership_transfer_method->valueint;
	THINGS_LOG_V(TAG, "[configuration] ownership_transfer_method : %d", g_ownership_transfer_method);

	cJSON *wifi = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_WIFI);
	if (wifi == NULL) {
		THINGS_LOG_V(TAG, "[configuration] wifi is NULL");
		goto JSON_ERROR;
	}

	cJSON *wifi_interfaces = cJSON_GetObjectItem(wifi, KEY_CONFIGURATION_WIFI_INTERFACES);
	cJSON *wifi_frequency = cJSON_GetObjectItem(wifi, KEY_CONFIGURATION_WIFI_FREQUENCY);
	if (wifi_interfaces == NULL) {
		THINGS_LOG_V(TAG, "[configuration] wifi_interfaces is null");
		goto JSON_ERROR;
	}
	if (wifi_frequency == NULL) {
		THINGS_LOG_V(TAG, "[configuration] wifi_frequency is null");
		goto JSON_ERROR;
	}

	THINGS_LOG_V(TAG, "[configuration] wifi_interfaces : %d / wifi_frequency : %d", wifi_interfaces->valueint, wifi_frequency->valueint);
	g_wifi_interface = wifi_interfaces->valueint;

	if (wifi_frequency->valueint == 1) {
		g_wifi_freq = WiFi_24G;
	} else if (wifi_frequency->valueint == 2) {
		g_wifi_freq = WiFi_5G;
	} else if (wifi_frequency->valueint == 3) {
		g_wifi_freq = WiFi_BOTH;
	} else {
		THINGS_LOG_V(TAG, "unknown wifi freq value");
	}

	cJSON *file_path = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_FILEPATH);
	if (file_path == NULL) {
		THINGS_LOG_V(TAG, "filePath is NULL");
		goto JSON_ERROR;
	}

	cJSON *svrdb = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_SVRDB);
	cJSON *provisioning = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_PROVISIONING);
#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
	cJSON *certificate = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_CERTIFICATE);
	cJSON *privateKey = cJSON_GetObjectItem(file_path, KEY_CONFIGURATION_FILEPATH_PRIVATEKEY);
#endif

	if (svrdb == NULL) {
		THINGS_LOG_V(TAG, "[configuration] svrdb is null");
		goto JSON_ERROR;
	}
	if (provisioning == NULL) {
		THINGS_LOG_V(TAG, "[configuration] provisioning is null");
		goto JSON_ERROR;
	}
#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
	if (certificate == NULL) {
		THINGS_LOG_V(TAG, "[configuration] certificate is null");
		goto JSON_ERROR;
	}
	if (privateKey == NULL) {
		THINGS_LOG_V(TAG, "[configuration] privateKey is null");
		goto JSON_ERROR;
	}
#endif

	memset(g_svrdb_file_path, 0, (size_t)MAX_FILE_PATH_LENGTH + 1);
#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
	memset(g_certificate_file_path, 0, (size_t)MAX_FILE_PATH_LENGTH + 1);
	memset(g_private_key_file_path, 0, (size_t)MAX_FILE_PATH_LENGTH + 1);
#endif

	if (strncmp(svrdb->valuestring, "/", 1) == 0) {
		if (strlen(svrdb->valuestring) > (size_t)MAX_FILE_PATH_LENGTH) {
			THINGS_LOG_V(TAG, "svrdb file path length exceeded");
			goto JSON_ERROR;
		}
		memcpy(g_svrdb_file_path, svrdb->valuestring, strlen(svrdb->valuestring));
	} else {
		if (strlen(svrdb->valuestring) > (size_t)MAX_FILE_PATH_LENGTH - strlen(PATH_MNT)) {
			THINGS_LOG_V(TAG, "svrdb file path length exceeded");
			goto JSON_ERROR;
		}
		strncpy(g_svrdb_file_path, PATH_MNT, strlen(PATH_MNT));
		strncat(g_svrdb_file_path, svrdb->valuestring, (size_t)MAX_FILE_PATH_LENGTH - strlen(PATH_MNT));
	}

	if (strncmp(provisioning->valuestring, "/", 1) == 0) {
		if (strlen(provisioning->valuestring) > (size_t)MAX_CLOUD_ADDRESS) {
			THINGS_LOG_V(TAG, "provisioning file path length exceeded");
			goto JSON_ERROR;
		}
		memcpy(g_things_cloud_file_path, provisioning->valuestring, strlen(provisioning->valuestring));
	} else {
		if (strlen(provisioning->valuestring) > (size_t)MAX_CLOUD_ADDRESS - strlen(PATH_MNT)) {
			THINGS_LOG_V(TAG, "provisioning file path length exceeded");
			goto JSON_ERROR;
		}
		strncpy(g_things_cloud_file_path, PATH_MNT, strlen(PATH_MNT));
		strncat(g_things_cloud_file_path, provisioning->valuestring, (size_t)MAX_CLOUD_ADDRESS - strlen(PATH_MNT));
	}

#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
	if (strncmp(certificate->valuestring, "/", 1) == 0) {
		if (strlen(certificate->valuestring) > (size_t)MAX_FILE_PATH_LENGTH) {
			THINGS_LOG_V(TAG, "certificate file path length exceeded");
			goto JSON_ERROR;
		}
		memcpy(g_certificate_file_path, certificate->valuestring, strlen(certificate->valuestring));
	} else {
		if (strlen(certificate->valuestring) > (size_t)MAX_FILE_PATH_LENGTH - strlen(PATH_ROM)) {
			THINGS_LOG_V(TAG, "certificate file path length exceeded");
			goto JSON_ERROR;
		}
		strncpy(g_certificate_file_path, PATH_ROM, strlen(PATH_ROM));
		strncat(g_certificate_file_path, certificate->valuestring, (size_t)MAX_FILE_PATH_LENGTH - strlen(PATH_ROM));
	}


	if (strncmp(privateKey->valuestring, "/", 1) == 0) {
		if (strlen(privateKey->valuestring) > (size_t)MAX_FILE_PATH_LENGTH) {
			THINGS_LOG_V(TAG, "privateKey file path length exceeded");
			goto JSON_ERROR;
		}
		memcpy(g_private_key_file_path, privateKey->valuestring, strlen(privateKey->valuestring));
	} else {
		if (strlen(privateKey->valuestring) > (size_t)MAX_FILE_PATH_LENGTH - strlen(PATH_ROM)) {
			THINGS_LOG_V(TAG, "privateKey file path length exceeded");
			goto JSON_ERROR;
		}
		strncpy(g_private_key_file_path, PATH_ROM, strlen(PATH_ROM));
		strncat(g_private_key_file_path, privateKey->valuestring, (size_t)MAX_FILE_PATH_LENGTH - strlen(PATH_ROM));
	}
#endif

	THINGS_LOG_V(TAG, "Security SVR DB file path : %s", g_svrdb_file_path);
#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
	THINGS_LOG_V(TAG, "[configuration] svrdb : %s / provisioning : %s", svrdb->valuestring, provisioning->valuestring);
	THINGS_LOG_V(TAG, "[configuration] certificate : %s / privateKey : %s", certificate->valuestring, privateKey->valuestring);
#endif

	ret = 1;
JSON_ERROR:
	return ret;
}

static int parse_resource_type_json_with_internal(cJSON *resource_types_user)
{
	int ret = 0;
	struct st_resource_type_s *restype = NULL;
	int internal_resource_cnt = 0;
	int internal_resource_type_cnt = 0;

	if (resource_types_user == NULL) {
		THINGS_LOG_E(TAG, "resource_types_user is null");
		return ret;
	}

	cJSON *json_internal_root = cJSON_Parse((const char *)internal_resource_json_str);
	if (json_internal_root == NULL) {
		THINGS_LOG_E(TAG, "json_internal_root is null");
		return ret;
	}

	cJSON *resource_types_internal = cJSON_GetObjectItem(json_internal_root, KEY_RESOURCES_TYPE);
	if (resource_types_internal == NULL) {
		THINGS_LOG_E(TAG, "resource_types_internal is null");
		goto JSON_ERROR;
	}

	if (parse_internal_json(json_internal_root, &internal_resource_cnt, &internal_resource_type_cnt) == 0) {
		THINGS_LOG_E(TAG, "parse_internal_json fail");
		goto JSON_ERROR;
	}

	cJSON *resource_types = NULL;
	int iter_cnt = 0;
	int resource_type_cnt_user = cJSON_GetArraySize(resource_types_user);
	int resource_type_cnt_internal = cJSON_GetArraySize(resource_types_internal);

	g_resource_type_cnt = resource_type_cnt_user + resource_type_cnt_internal;
	g_resource_type_hmap = hashmap_create(g_resource_type_cnt);
	THINGS_LOG_V(TAG, "Resource Types Cnt : %d (itn : %d, user : %d)", g_resource_type_cnt, resource_type_cnt_internal, resource_type_cnt_user);
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			resource_types = resource_types_internal;
			iter_cnt = resource_type_cnt_internal;
		} else {
			resource_types = resource_types_user;
			iter_cnt = resource_type_cnt_user;
		}

		for (int iter = 0; iter < iter_cnt; iter++) {
			int index = 0;

			restype = create_resource_type();

			cJSON *cj_rt = cJSON_GetArrayItem(resource_types, iter);
			cJSON *rtype = cJSON_GetObjectItem(cj_rt, KEY_DEVICE_RESOURCETYPE_TYPE);
			cJSON *properties = cJSON_GetObjectItem(cj_rt, KEY_DEVICE_RESOURCETYPE_PROPERTIES);

			if (cj_rt == NULL) {
				THINGS_LOG_E(TAG, "cj_rt is null");
				goto JSON_ERROR;
			}
			if (rtype == NULL) {
				THINGS_LOG_E(TAG, "rtype is null");
				goto JSON_ERROR;
			}
			if (properties == NULL) {
				THINGS_LOG_E(TAG, "properties is null");
				goto JSON_ERROR;
			}

			index = hashmap_get_hashval((unsigned char *)rtype->valuestring);
			memcpy(restype->rt, rtype->valuestring, strlen(rtype->valuestring) + 1);

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
				restype->prop[iter2]->type = type->valueint;
				if (mandatory->type == cJSON_True) {
					restype->prop[iter2]->mandatory = true;
				} else {
					restype->prop[iter2]->mandatory = false;
				}
				if (rw != NULL) {
					restype->prop[iter2]->rw = rw->valueint;
				}
			}

			hashmap_insert(g_resource_type_hmap, restype, index);
		}
	}

	ret = 1;
JSON_ERROR:
	if (json_internal_root != NULL) {
		cJSON_Delete(json_internal_root);
	}
	return ret;
}

static int parse_resource_with_internal(cJSON *single_rsc, cJSON *internal_rsc, int user_single_cnt, int internal_single_cnt)
{
	int ret = 0;
	int i = 0;
	int rsr_cnt = 0;

	for (i = 0; i < 2; i++) {
		int iter_cnt;
		cJSON *single;

		if (i == 0) {
			iter_cnt = internal_single_cnt;
			single = internal_rsc;
		} else {
			iter_cnt = user_single_cnt;
			single = single_rsc;
		}

		if (single == NULL) {
			ret = 0;
			goto JSON_ERROR;
		}

		for (int iter = 0; iter < iter_cnt; iter++) {
			cJSON *res = cJSON_GetArrayItem(single, iter);
			if (res->type != 0) {
				cJSON *uri = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_URI);
				if (uri != NULL) {
					memcpy(g_device->single[rsr_cnt].uri, uri->valuestring, strlen(uri->valuestring) + 1);
				}

				cJSON *types = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_TYPES);
				if (types != NULL) {
					int type_cnt = cJSON_GetArraySize(types);
					g_device->single[rsr_cnt].rt_cnt = type_cnt;
					for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
						cJSON *type = cJSON_GetArrayItem(types, typeiter);
						g_device->single[rsr_cnt].resource_types[typeiter] = things_malloc(sizeof(char) * strlen(type->valuestring) + 1);
						memcpy(g_device->single[rsr_cnt].resource_types[typeiter], type->valuestring, strlen(type->valuestring) + 1);
					}
				} else {
					THINGS_LOG_D(TAG, "[SINGLE] resource type is NULL");
					ret = 0;
					goto JSON_ERROR;
				}
				cJSON *interfaces = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_INTERFACES);
				if (interfaces != NULL) {
					int if_cnt = cJSON_GetArraySize(interfaces);
					g_device->single[rsr_cnt].if_cnt = if_cnt;
					for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
						cJSON *interface = cJSON_GetArrayItem(interfaces, ifiter);
						g_device->single[rsr_cnt].interface_types[ifiter] = things_malloc(sizeof(char) * strlen(interface->valuestring) + 1);
						memcpy(g_device->single[rsr_cnt].interface_types[ifiter], interface->valuestring, strlen(interface->valuestring) + 1);
					}
				} else {
					THINGS_LOG_D(TAG, "[SINGLE] resource interface is NULL");
					ret = 0;
					goto JSON_ERROR;
				}
				cJSON *policy = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_POLICY);
				if (policy != NULL) {
					g_device->single[rsr_cnt].policy = policy->valueint;
				} else {
					THINGS_LOG_D(TAG, "[SINGLE] resource policy is NULL");
					ret = 0;
					goto JSON_ERROR;
				}
				rsr_cnt++;
			} else {
				ret = 0;
				goto JSON_ERROR;
			}
		}
	}
	ret = 1;
JSON_ERROR:
	return ret;
}

static int parse_resource_json(cJSON *device)
{
	int ret = 0;
	cJSON *json_internal_root = cJSON_Parse((const char *)internal_resource_json_str);
	if (json_internal_root == NULL) {
		THINGS_LOG_E(TAG, "json_internal_root is null");
		goto JSON_ERROR;
	}
	int internal_resource_cnt = 0;
	int internal_resource_type_cnt = 0;

	if (parse_internal_json(json_internal_root, &internal_resource_cnt, &internal_resource_type_cnt) == 0) {
		THINGS_LOG_E(TAG, "parse_internal_json fail");
		goto JSON_ERROR;
	}

	cJSON *resources = cJSON_GetObjectItem(device, KEY_RESOURCES);
	cJSON *internal_resources = cJSON_GetObjectItem(json_internal_root, KEY_RESOURCES);

	if (resources != NULL && internal_resources != NULL) {
		cJSON *single = cJSON_GetObjectItem(resources, KEY_RESOURCES_SIG);
		cJSON *internal_single = cJSON_GetObjectItem(internal_resources, KEY_RESOURCES_SIG);
		if (single != NULL && internal_single != NULL) {
			int user_single_cnt = cJSON_GetArraySize(single);
			int internal_single_cnt = cJSON_GetArraySize(internal_single);
			g_device->sig_cnt = (user_single_cnt + internal_single_cnt);
			THINGS_LOG_D(TAG, "[DEVICE] Total Resources for Single Usage Cnt : %d (itn : %d, user : %d)", g_device->sig_cnt, internal_single_cnt, user_single_cnt);

			g_device->single = (things_resource_info_s *)things_malloc(sizeof(things_resource_info_s) * (g_device->sig_cnt));
			if (g_device->single == NULL) {
				THINGS_LOG_D(TAG, "[SINGLE] resource is NULL");
				goto JSON_ERROR;
			}
			if (parse_resource_with_internal(single, internal_single, user_single_cnt, internal_single_cnt) == 0) {
				THINGS_LOG_E(TAG, "parse_resource_with_internal fail");
				goto JSON_ERROR;
			}

			THINGS_LOG_V(TAG, "[SINGLE] Resources for Single Usage Cnt : %d", g_device->sig_cnt);
		} else {
			THINGS_LOG_V(TAG, "[SINGLE] Reosurces Not Exist");
		}
#ifdef CONFIG_ST_THINGS_COLLECTION
		cJSON *collection = cJSON_GetObjectItem(resources, KEY_RESOURCES_COL);

		if (collection != NULL) {
			g_device->col_cnt = cJSON_GetArraySize(collection);
			THINGS_LOG_D(TAG, "[DEVICE] Resources for Collection Cnt : %d", g_device->col_cnt);

			g_device->collection = (col_resource_s *) things_malloc(sizeof(col_resource_s) * g_device->col_cnt);
			if (g_device->collection == NULL) {
				THINGS_LOG_D(TAG, "[COLLECTION] resource is NULL");
				ret = 0;
				goto JSON_ERROR;
			}

			for (int iter = 0; iter < g_device->col_cnt; iter++) {
				cJSON *res = cJSON_GetArrayItem(collection, iter);
				if (res->type != NULL) {
					cJSON *uri = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_URI);
					memcpy(g_device->collection[iter].uri, uri->valuestring, strlen(uri->valuestring) + 1);
					THINGS_LOG_V(TAG, "[COLLECTION] collection[0].uri : %s", (g_device->collection[iter].uri));

					cJSON *types = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_TYPES);
					if (types) {
						int type_cnt = cJSON_GetArraySize(types);
						g_device->collection[iter].rt_cnt = type_cnt;
						for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
							cJSON *type = cJSON_GetArrayItem(types, typeiter);
							g_device->collection[iter].resource_types[typeiter] = things_malloc(sizeof(char) * strlen(type->valuestring) + 1);
							memcpy(g_device->collection[iter].resource_types[typeiter], type->valuestring, strlen(type->valuestring) + 1);
							THINGS_LOG_V(TAG, "[COLLECTION] collection[iter].resource_types[typeiter] : %s", (g_device->collection[iter].resource_types[typeiter]));
						}
					} else {
						THINGS_LOG_D(TAG, "[COLLECTION] resource type is NULL");
						ret = 0;
						goto JSON_ERROR;
					}

					cJSON *interfaces = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_INTERFACES);
					if (interfaces != NULL) {
						int if_cnt = cJSON_GetArraySize(interfaces);
						g_device->collection[iter].if_cnt = if_cnt;
						for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
							cJSON *interface = cJSON_GetArrayItem(interfaces, ifiter);
							g_device->collection[iter].interface_types[ifiter] = things_malloc(sizeof(char) * strlen(interface->valuestring) + 1);
							memcpy(g_device->collection[iter].interface_types[ifiter], interface->valuestring, strlen(interface->valuestring) + 1);
						}
					} else {
						THINGS_LOG_D(TAG, "[COLLECTION] resource interface is NULL");
						ret = 0;
						goto JSON_ERROR;
					}

					cJSON *policy = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_POLICY);
					if (policy == NULL) {
						THINGS_LOG_E(TAG, "[COLLECTION] Fail to get collection[iter].policy");
						ret = 0;
						goto JSON_ERROR;
					}

					g_device->collection[iter].policy = policy->valueint;
					THINGS_LOG_V(TAG, "[COLLECTION] collection[iter].policy : %d", (g_device->collection[iter].policy));

					cJSON *links = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_COLLECTION_LINKS);
					if (links != NULL) {
						int linkCnt = cJSON_GetArraySize(links);

						g_device->collection[iter].link_cnt = linkCnt;
						g_device->collection[iter].links = (things_resource_info_s**)things_malloc(sizeof(things_resource_info_s*) * linkCnt);

						THINGS_LOG_V(TAG, "[COLLECTION] collection[iter].link_cnt : %d", (g_device->collection[iter].link_cnt));
						for (int linkiter = 0; linkiter < linkCnt; linkiter++) {
							cJSON *link = cJSON_GetArrayItem(links, linkiter);

							struct things_resource_info_s *link_resource = create_resource();

							cJSON *uri = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_URI);
							if (uri != NULL) {
								memcpy(link_resource->uri, uri->valuestring, strlen(uri->valuestring) + 1);
								THINGS_LOG_V(TAG, "[COLLECTION] link_resource->uri : %s", (link_resource->uri));
							}

							cJSON *types = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_TYPES);
							if (types) {
								int type_cnt = cJSON_GetArraySize(types);
								link_resource->rt_cnt = type_cnt;
								for (int typeiter = 0; typeiter < type_cnt; typeiter++) {
									cJSON *type = cJSON_GetArrayItem(types, typeiter);
									link_resource->resource_types[typeiter] = things_malloc(sizeof(char) * strlen(type->valuestring) + 1);
									memcpy(link_resource->resource_types[typeiter], type->valuestring, strlen(type->valuestring) + 1);
								}
							} else {
								THINGS_LOG_V(TAG, "[COLLECTION] resource type is NULL");
								ret = 0;
								goto JSON_ERROR;
							}
							cJSON *interfaces = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_INTERFACES);
							if (interfaces) {
								int if_cnt = cJSON_GetArraySize(interfaces);
								link_resource->if_cnt = if_cnt;
								for (int ifiter = 0; ifiter < if_cnt; ifiter++) {
									cJSON *interface = cJSON_GetArrayItem(interfaces, ifiter);
									link_resource->interface_types[ifiter] = things_malloc(sizeof(char) * strlen(interface->valuestring) + 1);
									memcpy(link_resource->interface_types[ifiter], interface->valuestring, strlen(interface->valuestring) + 1);
								}
							} else {
								THINGS_LOG_V(TAG, "[COLLECTION] resource interface is NULL");
								ret = 0;
								goto JSON_ERROR;
							}
							cJSON *policy = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_POLICY);
							if (policy) {
								link_resource->policy = policy->valueint;
								g_device->collection[iter].links[linkiter] = link_resource;
							} else {
								THINGS_LOG_V(TAG, "[COLLECTION] resource policy is NULL");
								ret = 0;
								goto JSON_ERROR;
							}
						}
					} else {
						THINGS_LOG_V(TAG, "[COLLECTION] link is NULL");
						ret = 0;
						goto JSON_ERROR;
					}
				}

			}
			THINGS_LOG_V(TAG, "[COLLECTION] URI. : %s", g_device->collection[0].uri);
			THINGS_LOG_V(TAG, "[COLLECTION] interface_type. : %s", g_device->collection[0].interface_types[0]);
			THINGS_LOG_V(TAG, "[COLLECTION] resource_types. : %s", g_device->collection[0].resource_types[0]);
			THINGS_LOG_V(TAG, "[COLLECTION] if_cnt. : %d", g_device->collection[0].if_cnt);
			THINGS_LOG_V(TAG, "[COLLECTION] rt_cnt. : %d", g_device->collection[0].rt_cnt);
			THINGS_LOG_V(TAG, "[COLLECTION] link_cnt. : %d", g_device->collection[0].link_cnt);
			THINGS_LOG_V(TAG, "[COLLECTION] policy. : %d", g_device->collection[0].policy);
		} else {
			THINGS_LOG_V(TAG, "Children Reosurces Not Exist");
		}
#endif
	} else {
		THINGS_LOG_V(TAG, "Reosurces Not Exist");
	}

	THINGS_LOG_D(TAG, "[DEVICE] ============================================");

	ret = 1;
JSON_ERROR:
	if (json_internal_root != NULL) {
		cJSON_Delete(json_internal_root);
	}

	return ret;
}

static int parse_device_json(cJSON *device)
{
	int device_num = 0;
	int ret = 0;

	THINGS_LOG_D(TAG, "[DEVICE] ============================================");

	delete_device(g_device);
	g_device = create_device();
	if (g_device == NULL) {
		THINGS_LOG_D(TAG, "device create fail");
		goto JSON_ERROR;
	}
	g_device->no = device_num;
	g_device->is_physical = 1;

	cJSON *specification = cJSON_GetObjectItem(device, KEY_DEVICE_SPECIFICATION);
	if (specification != NULL) {
		cJSON *spec_device = cJSON_GetObjectItem(specification, KEY_DEVICE_SPECIFICATION_DEVICE);
		if (spec_device != NULL) {
			cJSON *device_type = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DEVICETYPE);
			cJSON *device_name = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DEVICENAME);
			/* spec_ver & data_model_ver is not supported */

			if (device_type != NULL) {
				g_device->type = (char *) things_malloc(sizeof(char) * (strlen(device_type->valuestring) + 1));
				strncpy(g_device->type, device_type->valuestring, strlen(device_type->valuestring) + 1);
			} else {
				goto JSON_ERROR;
			}

			if (device_name != NULL) {
				g_device->name = (char *) things_malloc(sizeof(char) * (strlen(device_name->valuestring) + 1));
				strncpy(g_device->name, device_name->valuestring, strlen(device_name->valuestring) + 1);
			} else {
				goto JSON_ERROR;
			}
		}

		cJSON *spec_platform = cJSON_GetObjectItem(specification, KEY_DEVICE_SPECIFICATION_PLATFORM);
		if (spec_platform != NULL) {
			cJSON *mnid = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MNID);
			cJSON *manufacturer_url = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERURL);
			cJSON *manufacturing_date = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURINGDATE);
			cJSON *model_number = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MODELNUMBER);
			cJSON *firmware_version = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_FIRMWAREVERSION);
			cJSON *vid = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_VID);

			if (mnid == NULL) {
				mnid = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERNAME); // Backward compatibility for existing published IDEs
				if (mnid == NULL) {
					goto JSON_ERROR;
				}
			}

			if (strlen(mnid->valuestring) != 4) {
				THINGS_LOG_V(TAG, "MNID exceeds 4 bytes. please check (4 bytes are fixed sizes.)");
				goto JSON_ERROR;
			}

			g_device->mnid = (char *) things_malloc(sizeof(char) * (strlen(mnid->valuestring) + 1));
			strncpy(g_device->mnid, mnid->valuestring, strlen(mnid->valuestring) + 1);

			if (vid == NULL) {
				vid = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_VENDORID); // Backward compatibility for existing published IDEs
				if (vid == NULL) {
					goto JSON_ERROR;
				}
			}

			g_device->vid = (char *) things_malloc(sizeof(char) * (strlen(vid->valuestring) + 1));
			strncpy(g_device->vid, vid->valuestring, strlen(vid->valuestring) + 1);

			if (manufacturer_url != NULL) {
				g_device->manufacturer_url = (char *) things_malloc(sizeof(char) * (strlen(manufacturer_url->valuestring) + 1));
				strncpy(g_device->manufacturer_url, manufacturer_url->valuestring, strlen(manufacturer_url->valuestring) + 1);
			}
			if (manufacturing_date != NULL) {
				g_device->manufacturing_date = (char *) things_malloc(sizeof(char) * (strlen(manufacturing_date->valuestring) + 1));
				strncpy(g_device->manufacturing_date, manufacturing_date->valuestring, strlen(manufacturing_date->valuestring) + 1);
			}
			if (model_number != NULL) {
				g_device->model_num = (char *) things_malloc(sizeof(char) * (strlen(model_number->valuestring) + 1));
				strncpy(g_device->model_num, model_number->valuestring, strlen(model_number->valuestring) + 1);

				g_model_number = (char *) things_malloc(sizeof(char) * strlen(model_number->valuestring) + 1);
				strncpy(g_model_number, model_number->valuestring, strlen(model_number->valuestring) + 1);
			}

			g_device->ver_p = (char *) things_malloc(sizeof(char) * (sizeof(DEVICE_PLATFORM_VERSION)));
			strncpy(g_device->ver_p, DEVICE_PLATFORM_VERSION, sizeof(DEVICE_PLATFORM_VERSION));

			g_device->ver_os = (char *) things_malloc(sizeof(char) + (sizeof(DEVICE_OS_VERSION)));
			strncpy(g_device->ver_os, DEVICE_OS_VERSION, sizeof(DEVICE_OS_VERSION));

			g_device->ver_hw = (char *) things_malloc(sizeof(char) * (sizeof(DEVICE_HARDWARE_VERSION)));
			strncpy(g_device->ver_hw, DEVICE_HARDWARE_VERSION, sizeof(DEVICE_HARDWARE_VERSION));

			if (firmware_version != NULL) {
				g_device->ver_fw = (char *) things_malloc(sizeof(char) * (strlen(firmware_version->valuestring) + 1));
				strncpy(g_device->ver_fw, firmware_version->valuestring, strlen(firmware_version->valuestring) + 1);

				g_firmware_version = (char *) things_malloc(sizeof(char) * strlen(firmware_version->valuestring) + 1);
				strncpy(g_firmware_version, firmware_version->valuestring, strlen(firmware_version->valuestring) + 1);
			}
		}
	}
	THINGS_LOG_D(TAG, "[DEVICE] No. : %d", (g_device->no));
	THINGS_LOG_D(TAG, "[DEVICE] type : %s", (g_device->type));
	THINGS_LOG_D(TAG, "[DEVICE] name : %s", (g_device->name));
	THINGS_LOG_D(TAG, "[DEVICE] mf_name : %s", (g_device->mnid));
	THINGS_LOG_D(TAG, "[DEVICE] mf_url : %s", (g_device->manufacturer_url));
	THINGS_LOG_D(TAG, "[DEVICE] mf_date : %s", (g_device->manufacturing_date));
	THINGS_LOG_D(TAG, "[DEVICE] model num : %s", (g_device->model_num));
	THINGS_LOG_D(TAG, "[DEVICE] plat. ver : %s", (g_device->ver_p));
	THINGS_LOG_D(TAG, "[DEVICE] os version : %s", (g_device->ver_os));
	THINGS_LOG_D(TAG, "[DEVICE] hw version : %s", (g_device->ver_hw));
	THINGS_LOG_D(TAG, "[DEVICE] fw version : %s", (g_device->ver_fw));
	THINGS_LOG_D(TAG, "[DEVICE] vender id : %s", (g_device->vid));

	return parse_resource_json(device);

JSON_ERROR:
	delete_device(g_device);
	g_device = NULL;
	return ret;
}

int parse_internal_json(cJSON *internal_resource, int *rsr_cnt, int *rsr_type_cnt)
{
	int ret = 0;
	cJSON *resources = cJSON_GetObjectItem(internal_resource, KEY_RESOURCES);

	if (resources != NULL) {
		cJSON *single = cJSON_GetObjectItem(resources, KEY_RESOURCES_SIG);
		if (single != NULL) {
			*rsr_cnt = cJSON_GetArraySize(single);
		} else {
			goto JSON_ERROR;
		}
	} else {
		goto JSON_ERROR;
	}

	cJSON *resources_type = cJSON_GetObjectItem(internal_resource, KEY_RESOURCES_TYPE);

	if (resources_type != NULL) {
		*rsr_type_cnt = cJSON_GetArraySize(resources_type);
	} else {
		goto JSON_ERROR;
	}

	ret = 1;
JSON_ERROR:
	return ret;
}

static int parse_things_info_json(const char *filename)
{

	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ret = 0;
	char *json_str = get_json_string_from_file(filename);
	cJSON *json_user_root = NULL;

	if (json_str != NULL && strlen(json_str) > 0) {
		// 3. Parse the Json string
		json_user_root = cJSON_Parse((const char *)json_str);
		assert(json_user_root != NULL);

		// Device Items
		cJSON *devices = cJSON_GetObjectItem(json_user_root, KEY_DEVICE);
		if (devices == NULL) {
			THINGS_LOG_E(TAG, "device is NULL");
			goto JSON_ERROR;
		}

		int device_cnt = cJSON_GetArraySize(devices);
		THINGS_LOG_D(TAG, "device_cnt = %d", device_cnt);

		if (device_cnt > 1) {
			THINGS_LOG_E(TAG, "not supported one more deice");
			goto JSON_ERROR;
		}

		cJSON *device = cJSON_GetArrayItem(devices, 0);
		if (parse_device_json(device) == 0) {
			THINGS_LOG_E(TAG, "parse_device_json fail");
			goto JSON_ERROR;
		}

		cJSON *resource_types = cJSON_GetObjectItem(json_user_root, KEY_RESOURCES_TYPE);
		if (parse_resource_type_json_with_internal(resource_types) == 0) {
			THINGS_LOG_E(TAG, "parse_resource_type_json fail");
			goto JSON_ERROR;
		}

		cJSON *configuration = cJSON_GetObjectItem(json_user_root, KEY_CONFIGURATION);
		if (parse_configuration_json(configuration) == 0) {
			THINGS_LOG_E(TAG, "parse_configuration_json fail");
			goto JSON_ERROR;
		}
	}

	if (parse_things_cloud_json(g_things_cloud_file_path) == 0) {
		THINGS_LOG_D(TAG, "cloud data parsing error");
	}

	ret = 1;
JSON_ERROR:
	if (json_user_root != NULL) {
		cJSON_Delete(json_user_root);
	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int get_signup_data_from_json(const char *filename, es_cloud_signup_s **cl_data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ret = 1;
#ifdef CONFIG_ST_THINGS_SECURESTORAGE
	char *json_str = get_json_string_from_securestorage();
#else
	char *json_str = get_json_string_from_file(filename);
#endif
	cJSON *root = NULL;
	cJSON *cloud = NULL;

	if (json_str != NULL && strlen(json_str) > 0) {
		root = cJSON_Parse((const char *)json_str);
		cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
		if (cloud == NULL) {
			THINGS_LOG_V(TAG, "cloud cJSON is NULL.");
			ret = 0;
			goto JSON_ERROR;
		} else if ((ret = load_cloud_signup_data(cloud, cl_data)) != 1) {
			THINGS_LOG_V(TAG, "Load Cloud SignUp Data Failed.");
		}
	} else {
		THINGS_LOG_V(TAG, "cloud file Reading is failed.");
		ret = 0;
		goto JSON_ERROR;
	}
JSON_ERROR:
	if (root != NULL) {
		cJSON_Delete(root);
	}
	if (json_str != NULL) {
		things_free(json_str);
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int update_things_cloud_json_by_cloud_signup(const char *filename, es_cloud_signup_s *cl_data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ret = 0;
	cJSON *root = NULL;
	char *json_update = NULL;
#ifdef CONFIG_ST_THINGS_SECURESTORAGE
	char *json_str = get_json_string_from_securestorage();
#else
	char *json_str = get_json_string_from_file(filename);
#endif

	if (json_str != NULL && strlen(json_str) > 0) {
		root = cJSON_Parse((const char *)json_str);
		cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
		if (cloud == NULL) {
			THINGS_LOG_V(TAG, "cloud cJSON is NULL.");
			goto GOTO_OUT;
		}

		cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_ADDRESS);
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
				THINGS_LOG_V(TAG, "Fail : Copy to g_cloud_address.");
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
			json_update = cJSON_Print(root);
		} else {
			json_update = things_malloc(sizeof(char) * (strlen(origin_cloud_json_str) + 1));
			strncpy(json_update, origin_cloud_json_str, strlen(origin_cloud_json_str) + 1);
		}
#ifdef CONFIG_ST_THINGS_SECURESTORAGE
		if (set_json_string_into_securestorage(json_update) == 0) {
			THINGS_LOG_V(TAG, "Fail : Store data to info securestorage.");
			goto GOTO_OUT;
		}
#else
		if (set_json_string_into_file(filename, json_update) == 0) {
			THINGS_LOG_V(TAG, "Fail : Store data to info file.");
			goto GOTO_OUT;
		}
#endif
		ret = 1;
	}
	THINGS_LOG_D(TAG, "Update Success in \"%s\" file.", filename);
GOTO_OUT:
	if (root != NULL) {
		cJSON_Delete(root);
		root = NULL;
	}

	if (json_str != NULL) {
		things_free(json_str);
	}

	if (json_update != NULL) {
		things_free(json_update);
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ret;
}

static int Parse_things_files(const char *info_Path)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	int res = 1;

	THINGS_LOG_D(TAG, "orginal info file path = %s", info_Path);
	if (!parse_things_info_json(info_Path)) {
		THINGS_LOG_E(TAG, "Info file parsing Failed");
		res = 0;
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return res;
}

int dm_load_legacy_cloud_data(es_cloud_signup_s **cl_data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ret = 1;

	if (g_things_cloud_file_path[0] == 0 || cl_data == NULL) {
		THINGS_LOG_V(TAG, "Provisioning file path is NULL.(%s) or Invalid argument.(0x%X)", g_things_cloud_file_path, cl_data);
		return 0;
	}

	THINGS_LOG_D(TAG, "g_things_cloud_file_path = %s", g_things_cloud_file_path);

	if ((ret = get_signup_data_from_json(g_things_cloud_file_path, cl_data)) != 1) {
		THINGS_LOG_V(TAG, "Not Get sign-up data failed from Provisioning file.");
	}

	usleep(500000);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return ret;
}

const char *dm_get_things_device_type(const char* device_id)
{
	if (strncmp(g_device->device_id, device_id, MAX_DEVICE_ID_LENGTH) != 0) {
		THINGS_LOG_V(TAG, "unknown device_id : %s", device_id);
		return NULL;
	}

	return g_device->type;
}

const char *dm_get_svrdb_file_path(void)
{
	return g_svrdb_file_path;
}

#ifndef CONFIG_ST_THINGS_HW_CERT_KEY
const char *dm_get_certificate_file_path(void)
{
	return g_certificate_file_path;
}

const char *dm_get_privatekey_file_path(void)
{
	return g_private_key_file_path;
}
#endif

const char *dm_get_things_cloud_address(char *customized_ci_server)
{
	if (customized_ci_server && customized_ci_server[0] != 0) {
		return customized_ci_server;
	}
	return g_cloud_address;
}

bool dm_is_rsc_published(void)
{
	return (es_get_state() == ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD ? true : false);
}

static things_resource_s *register_resource(things_server_builder_s *p_builder, struct things_resource_info_s *resource, const char *id)
{
	things_resource_s *ret = NULL;
	if (NULL != resource) {
		THINGS_LOG_D(TAG, "RESOURCE TO REGISTER : %s", resource->uri);
		char res_uri[MAX_URI_LENGTH + 1] = { 0 };
		strncat(res_uri, resource->uri, MAX_URI_LENGTH);

		ret = p_builder->create_resource(p_builder, res_uri, resource->resource_types[0],
			resource->interface_types[0], CHECK_DISCOVERABLE(resource->policy),
			CHECK_OBSERVABLE(resource->policy), CHECK_SECURE(resource->policy));

		THINGS_LOG_D(TAG, "add_resource_type : %s", resource->resource_types[0]);
		THINGS_LOG_D(TAG, "add_interface_type : %s", resource->interface_types[0]);
		for (int rt_num = 1; rt_num < resource->rt_cnt; rt_num++) {
			p_builder->add_resource_type(ret, resource->resource_types[rt_num]);
			THINGS_LOG_D(TAG, "add_resource_type : %s", resource->resource_types[rt_num]);
		}

		for (int it_num = 1; it_num < resource->if_cnt; it_num++) {
			p_builder->add_interface_type(ret, resource->interface_types[it_num]);
			THINGS_LOG_D(TAG, "add_interface_type : %s", resource->interface_types[it_num]);
		}
	} else {
		THINGS_LOG_V(TAG, "Invalid Input Param ");
	}

	return ret;
}

st_device_s *dm_get_info_of_dev(unsigned long number)
{
	return g_device;
}

bool dm_register_device_id(void)
{
	const char *id = NULL;
	st_device_s **dev_list = NULL;

	if ((dev_list = (st_device_s **)things_malloc(sizeof(st_device_s *))) == NULL) {
		THINGS_LOG_E(TAG, "st_device_s mem allocation is failed.");
		return false;
	}
	// Set Main-Device ID
	iotivity_api_lock();
	id = OCGetServerInstanceIDString();
	iotivity_api_unlock();
	dev_list[0] = dm_get_info_of_dev(0);

	if (id == NULL || dev_list[0] == NULL) {
		THINGS_LOG_V(TAG, "Error : id = %s, Main-device =0x%X", id, dev_list[0]);
		goto GOTO_ERROR;
	}

	if (dev_list[0]->device_id != NULL) {
		things_free(dev_list[0]->device_id);
	}
	dev_list[0]->device_id = things_malloc(sizeof(char) * (strlen(id) + 1));
	strncpy(dev_list[0]->device_id, id, strlen(id) + 1);

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
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (p_builder == NULL) {
		THINGS_LOG_E(TAG, "Builder instance is NULL");
		return 0;
	}

	st_device_s *device = NULL;
#ifdef CONFIG_ST_THINGS_COLLECTION
	struct things_resource_s *p_collection_resource = NULL;
#endif

	// 2. Device Capability Resources Registration
	THINGS_LOG_D(TAG, "BEFORE SEARCHING THE DEVICE  ITEM FROM HASH TABLE");

	device = dm_get_info_of_dev(0);
	if (NULL != device) {
		int n_count_of_children = 0;
		char id[11] = { 0 };
		snprintf(id, sizeof(id), "%d", device->no);
		THINGS_LOG_D(TAG, "==================== Device (%s) ====================", id);
#ifdef CONFIG_ST_THINGS_COLLECTION
		device->pchild_resources = things_malloc(sizeof(things_resource_s *) * (device->col_cnt + device->sig_cnt));
		if (device->col_cnt < 1) {
			THINGS_LOG_D(TAG, "NO COLLECTION & ITS CHILDREN RESOURCE(S)");
		} else {
			THINGS_LOG_V(TAG, "COLLECTION CHILDREN RESOURCE(S) CNT : %d", device->col_cnt);

			char res_uri[MAX_URI_LENGTH + 1] = { 0 };
			strncat(res_uri, device->collection[0].uri, MAX_URI_LENGTH);

			p_collection_resource = p_builder->create_collection_resource(p_builder, res_uri, device->collection[0].resource_types[0]);

			iotivity_api_lock();
			for (int rt_num = 1; rt_num < device->collection[0].rt_cnt; rt_num++) {
				OCBindResourceTypeToResource(p_builder, device->collection[0].resource_types[rt_num]);
			}

			// THINGS_LOG_D(TAG, "IT TO BIND : %d", resource->if_cnt);
			for (int it_num = 1; it_num < device->collection[0].if_cnt; it_num++) {
				OCBindResourceInterfaceToResource(p_builder, device->collection[0].interface_types[it_num]);
			}
			iotivity_api_unlock();

			THINGS_LOG_D(TAG, "AFTER REGISTERGING DEVICE RESOURCE");

			if (p_collection_resource != NULL) {
				THINGS_LOG_D(TAG, "DEVICE RESOURCE(S) CNT : %d", device->col_cnt);
				for (int capa_num = 0; capa_num < device->collection[0].link_cnt; capa_num++) {
					struct things_resource_info_s *resource = device->collection[0].links[capa_num];
					device->pchild_resources[n_count_of_children++] = register_resource(p_builder, resource, id);
				}			// End of for children resource(s) registration loop
			}				// End of single device

			p_builder->bind_all(p_collection_resource, device->pchild_resources, n_count_of_children);

			device->pchild_resources[n_count_of_children++] = p_collection_resource;
		}
#else
		device->pchild_resources = things_malloc(sizeof(things_resource_s *) * device->sig_cnt);
#endif

		THINGS_LOG_D(TAG, "SINGLE RESOURCE(S) CNT : %d", device->sig_cnt);
		for (int capa_num = 0; capa_num < device->sig_cnt; capa_num++) {
			struct things_resource_info_s *resource = &device->single[capa_num];
			device->pchild_resources[n_count_of_children++] = register_resource(p_builder, resource, id);
		}					// End of for single resource registration loop

		// Create the Device(/oic/d) and Platform (/oic/p)
		// ex) Aircon Multi-Model
		// for hosting device
		THINGS_LOG_D(TAG, "REGISTERGING DEVICE INFO. TO DEVICE(/oic/d).");
		p_builder->set_device_info(p_builder, device->name, device->type);
		THINGS_LOG_D(TAG, "REGISTERGING DEVICE INFO. TO PLATFORM(/oic/p).");
		p_builder->set_platform_info(p_builder, device->model_num,	// gDeviceModel,
										device->ver_p,	// gPlatformVersion,
										device->ver_os,	// gOSVersion,
										device->ver_hw,	// gHWVersions,
										device->ver_fw,	// gFWVersions,
										device->vid,	// gVenderId
										device->mnid,	// manufacturer_name
										device->manufacturer_url);	// manufacturer_url

		THINGS_LOG_D(TAG,
			"[n_count_of_children : %d] : [device_cnt : %d]",
			n_count_of_children, (device->col_cnt + device->sig_cnt));

		// Total Size of the device->pChildresources : col_cnt + sig_cnt + 1(/device) + a(/oic/d & /oic/p)
		device->capa_cnt = n_count_of_children;
	}						// End of device NULL Check
	THINGS_LOG_D(TAG, "=====================================================");
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return 1;
}

int dm_update_things_cloud(es_cloud_signup_s *cl_data)
{
	THINGS_LOG_V(TAG, "RTOS Recived Signup Data domain[%s], address[%s], port [%s],  access_token[%s],  refresh_token[%s], token_type[%s],  " "uid[%s]", cl_data->domain, cl_data->address, cl_data->port, cl_data->access_token, cl_data->refresh_token, cl_data->token_type, cl_data->uid);
	THINGS_LOG_V(TAG, "RTOS Recived Signup Data redirect_uri[%s],  certificate[%s], sid[%s]", cl_data->redirect_uri, cl_data->certificate, cl_data->sid);

	if (g_things_cloud_file_path[0] == 0 || cl_data == NULL) {
		THINGS_LOG_E(TAG, "cloud file path is NULL(%s). or Invalid argument.(0x%X)", g_things_cloud_file_path, cl_data);
		return 0;
	}
	THINGS_LOG_D(TAG, "g_things_cloud_file_path = %s", g_things_cloud_file_path);
	if (update_things_cloud_json_by_cloud_signup(g_things_cloud_file_path, cl_data) == 0) {
		THINGS_LOG_E(TAG, "Provisioning file update failed.");
		return 0;
	}
	return 1;
}

int dm_del_things_cloud_data(void)
{
	THINGS_LOG_V(TAG, THINGS_FUNC_ENTRY);

	if (g_things_cloud_file_path[0] == 0) {
		THINGS_LOG_E(TAG, "cloud file path is NULL.");
		return 0;
	}
	THINGS_LOG_D(TAG, "g_things_cloud_file_path = %s", g_things_cloud_file_path);
	if (update_things_cloud_json_by_cloud_signup(g_things_cloud_file_path, NULL) == 0) {
		THINGS_LOG_E(TAG, "It's failed to delete Cloud-Data in cloud file.");
		return 0;
	}
	THINGS_LOG_D(TAG, "Delete Success.");
	esm_save_easysetup_state(ES_NOT_COMPLETE);
	THINGS_LOG_V(TAG, THINGS_FUNC_EXIT);
	return 1;
}

bool dm_is_es_complete(void)
{
	bool ret = (esm_read_easysetup_state() == 1);
	THINGS_LOG_D(TAG, "is_easysetup_complete = %d", ret);

	return ret;
}

int dm_validate_attribute_in_request(const char *rt, const void *payload)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	int ret = 0;

	if (rt != NULL && payload != NULL) {
		// 1. Get index with resource type
		unsigned long index = hashmap_get_hashval((unsigned char *)rt);
		OCRepPayload *r_payload = (OCRepPayload *)payload;

		// 2. Get resource type instance
		THINGS_LOG_V(TAG, "Index : %u  rt : %s", index, rt);
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
					THINGS_LOG_V(TAG, "\tCHECKING READ-ONLY ATTR : %s", res_type->prop[iter]->key);
					if (!OCRepPayloadIsNull(r_payload, res_type->prop[iter]->key)) {
						THINGS_LOG_D(TAG, "\t==> EXIST");
						goto VALIDATION_EXIT;
					} else {
						THINGS_LOG_D(TAG, "\t==> NOT EXIST");
					}
				}
			}
			//    b. Check existence of rw attribute in the payload which should exist at least 1
			for (iter = 0; iter < res_type->prop_cnt; iter++) {
				if (NULL != res_type->prop[iter]
					&& 1 == CHECK_WRITABLE(res_type->prop[iter]->rw)) {
					THINGS_LOG_V(TAG, "\tCHECKING WRITE-ABLE ATTR : %s", res_type->prop[iter]->key);
					if (!OCRepPayloadIsNull(r_payload, res_type->prop[iter]->key)) {
						THINGS_LOG_D(TAG, "\t==> EXIST");

						ret = 1;
						break;
					} else {
						THINGS_LOG_D(TAG, "\t==> NOT EXIST");
					}
				}
			}
#ifdef CONFIG_ST_THINGS_COLLECTION
			//    c. Make it pass if it's collection resource
			if (0 == strncmp(rt, OC_RSRVD_RESOURCE_TYPE_COLLECTION, strlen(OC_RSRVD_RESOURCE_TYPE_COLLECTION))
				|| 0 == strncmp(rt, SEC_RTYPE_THINGS_DEVICE, strlen(SEC_RTYPE_THINGS_DEVICE))) {
				// TODO : Need to check whether there an array delivered or not..
				THINGS_LOG_D(TAG, "\t==> Child %s", (r_payload->next != NULL ? "EXIST" : "NOT EXIST"));
				THINGS_LOG_D(TAG, "\t==> Request On Collection Resource. Making it PASS");
				ret = 1;
			}
#endif
		} else {
			THINGS_LOG_E(TAG, "Not Supporting rt : %s", rt);
		}
	} else {
		THINGS_LOG_E(TAG, "Invalid Input - rt : %s, payload %d", rt, payload);
	}

VALIDATION_EXIT:

	if (ret == 0) {
		THINGS_LOG_D(TAG, "\t==> INVALID REQUEST~!!!");
	}
	return ret;
}

int things_get_resource_type(const char *resource_uri, int *count, char ***resource_types)
{
	THINGS_LOG_D(TAG, "things_get_resource_type : %s", resource_uri);
	st_device_s *device = dm_get_info_of_dev(0);

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
#ifdef CONFIG_ST_THINGS_COLLECTION
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
#endif
	}
	return 0;
}

const int dm_get_wifi_property_interface(void)
{
	return g_wifi_interface;
}

const wifi_freq_e dm_get_wifi_property_freq(void)
{
	return g_wifi_freq;
}

const int dm_get_ownership_transfer_method(void)
{
	THINGS_LOG_V(TAG, "ownership_transfer_method : %d", g_ownership_transfer_method);
	return g_ownership_transfer_method;
}

int things_get_attributes_by_resource_type(const char *res_type, int *count, things_attribute_info_s ***attributes)
{
	int resource_type_cnt = (int)hashmap_count(g_resource_type_hmap);
	THINGS_LOG_D(TAG, "things_get_attributes_by_resource_type : %s, resource_type_cnt : %d", res_type, resource_type_cnt);

	if (resource_type_cnt > 0) {

		int index = hashmap_get_hashval((unsigned char *)res_type);
		struct st_resource_type_s *res_type_internal = (struct st_resource_type_s *)hashmap_get(g_resource_type_hmap, index);
		if (res_type_internal == NULL) {
			THINGS_LOG_V(TAG, "res_type_internal Not Exist");
			return 0;
		}

		THINGS_LOG_D(TAG, "res_type_internal : %s, res_type : %s", res_type_internal->rt, res_type);
		if (strncmp(res_type_internal->rt, res_type, strlen(res_type)) == 0) {
			THINGS_LOG_D(TAG, "res_type_internal->prop_cnt : %d", res_type_internal->prop_cnt);
			(*count) = res_type_internal->prop_cnt;
			(*attributes) = res_type_internal->prop;
			return 1;
		}
	}
	return 0;
}
#ifdef CONFIG_ST_THINGS_COLLECTION
bool things_is_collection_resource(const char *res_uri)
{
	st_device_s *device = NULL;
	device = dm_get_info_of_dev(0);
	if (device == NULL) {
		THINGS_LOG_V(TAG, "Device Not Exist");
		return NULL;
	}

	if (device->col_cnt > 0) {
		for (int index = 0; index < device->col_cnt; index++) {
			if (strncmp(device->collection[index].uri, res_uri, strlen(res_uri)) == 0) {
				return true;
			}
		}
	}
	return false;
}

int things_get_child_resources(const char *col_res_uri, int *count, things_resource_info_s ***child_resources)
{
	st_device_s *device = NULL;
	device = dm_get_info_of_dev(0);
	if (device == NULL) {
		THINGS_LOG_V(TAG, "Device Not Exist");
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
#endif

int dm_init_module(const char *devJsonPath)
{
	if (devJsonPath == NULL) {
		THINGS_LOG_E(TAG, "devJsonPath is NULL");
		return 0;
	}

	memset(g_svrdb_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
	memset(g_things_cloud_file_path, 0, (size_t) MAX_FILE_PATH_LENGTH);
	memset(g_cloud_address, 0, (size_t) MAX_CLOUD_ADDRESS);

	return Parse_things_files(devJsonPath);
}

int dm_termiate_module(void)
{
	st_device_s *device = dm_get_info_of_dev(0);
	delete_device(device);
	delete_resource_type_map();
	return 1;
}

bool dm_get_easy_setup_use_artik_crt(void)
{
	return is_artik;
}

char *dm_get_mnid(void)
{
	return g_device->mnid;
}

char *dm_get_firmware_version(void)
{
	return g_firmware_version;
}

char *dm_get_vendor_id(void)
{
	return g_device->vid;
}

char *dm_get_model_number(void)
{
	return g_model_number;
}

char *dm_get_access_token(void)
{
	es_cloud_signup_s *cloud_data = NULL;
	char *cloud_access_token = NULL;

	if (dm_load_legacy_cloud_data(&cloud_data) == 1) {
		cloud_access_token = things_strdup(cloud_data->access_token);
	}

	es_cloud_signup_clear(cloud_data);
	return cloud_access_token;
}

char *dm_get_uid(void)
{
	es_cloud_signup_s *cloud_data = NULL;
	char *cloud_uid = NULL;

	if (dm_load_legacy_cloud_data(&cloud_data) == 1) {
		cloud_uid = things_strdup(cloud_data->uid);
	}

	es_cloud_signup_clear(cloud_data);
	return cloud_uid;
}
