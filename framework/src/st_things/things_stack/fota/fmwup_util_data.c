/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <pthread.h>
#include <json/cJSON.h>
#include <stdlib.h>
#include "fmwup_util_data.h"
#include "fmwup_api.h"

#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "framework/things_data_manager.h"

#include "logging/things_logger.h"
#define TAG "[things_fota]"

#define KEY_MANAGER_FILE_NAME "/mnt/fota_key_manager.dat"

static bool g_active_check = false;
static pthread_mutex_t g_data_key_manager_mutex = PTHREAD_MUTEX_INITIALIZER;
static const char *key_manager_original_json_str = "{\n\
}";

cJSON *root = NULL;

int64_t fmwup_data_get_update_int64(const char *str)
{
	THINGS_LOG_D(TAG, "str : %s", str);
	if (strcmp(FIRMWARE_PROPERTY_UPDATE_INIT, str) == 0) {
		return FMWUP_COMMAND_INIT;
	} else if (strcmp(FIRMWARE_PROPERTY_UPDATE_CHECK, str) == 0) {
		return FMWUP_COMMAND_CHECK;
	} else if (strcmp(FIRMWARE_PROPERTY_UPDATE_DOWNLOAD, str) == 0) {
		return FMWUP_COMMAND_DOWNLOAD;
	} else if (strcmp(FIRMWARE_PROPERTY_UPDATE_UPDATE, str) == 0) {
		return FMWUP_COMMAND_UPDATE;
	} else if (strcmp(FIRMWARE_PROPERTY_UPDATE_DOWNLOADUPDATE, str) == 0) {
		return FMWUP_COMMAND_DOWNLOADUPDATE;
	} else {
		return FMWUP_COMMAND_UNKNOWN;
	}
}

char *fmwup_data_get_update_string(int64_t cmd)
{
	THINGS_LOG_D(TAG, "cmd : %d", cmd);
	switch (cmd) {
	case FMWUP_COMMAND_INIT:
		return FIRMWARE_PROPERTY_UPDATE_INIT;
	case FMWUP_COMMAND_CHECK:
		return FIRMWARE_PROPERTY_UPDATE_CHECK;
	case FMWUP_COMMAND_DOWNLOAD:
		return FIRMWARE_PROPERTY_UPDATE_DOWNLOAD;
	case FMWUP_COMMAND_UPDATE:
		return FIRMWARE_PROPERTY_UPDATE_UPDATE;
	case FMWUP_COMMAND_DOWNLOADUPDATE:
		return FIRMWARE_PROPERTY_UPDATE_DOWNLOADUPDATE;
	}

	return "";
}

int key_manager_init()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	int is_valid = 1;
	char *json_str = get_json_string_from_file(KEY_MANAGER_FILE_NAME);

	if (json_str != NULL) {
		root = cJSON_Parse((const char *)json_str);
		if (root == NULL) {
			THINGS_LOG_D(TAG, "cJSON_Parse is null.");
			is_valid = 0;
		} else {
			THINGS_LOG_D(TAG, "cJSON_Parse is true.");
		}
		things_free(json_str);		
	} else {
		THINGS_LOG_D(TAG, "KEY_MANAGER_FILE_NAME is empty.");
		is_valid = 0;
	}

	if (is_valid == 0) {
		THINGS_LOG_D(TAG, "Try to reset the info file.");
		return key_manager_reset_data();
	}

	return 0;
}

int key_manager_set_data(const char *name, char *data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (!name || !data) {
		THINGS_LOG_E(TAG, "name or data is NULL");
		if (name) {
			THINGS_LOG_E(TAG, "name [%s]", name);
		}
		if (data) {
			THINGS_LOG_E(TAG, "data [%s]", data);
		}
		return -1;
	}

	if (root == NULL) {
		THINGS_LOG_E(TAG, "cJSON_Parse is null.");
		return -1;
	}

	cJSON_DeleteItemFromObject(root, name);
	cJSON_AddStringToObject(root, name, data);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return 0;
}

char *key_manager_set_default_data(const char *name)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (!name) {
		THINGS_LOG_E(TAG, "name is NULL");
		return NULL;
	}
	THINGS_LOG_D(TAG, "key [%s]", name);

	char *default_data = NULL;

	if (strcmp(name, FIRMWARE_PROPERTY_UPDATE) == 0) {
		default_data = things_strdup(KEY_MANAGER_INT_DEFAULT_DATA);
	} else if (strcmp(name, FIRMWARE_PROPERTY_UPDATE_TIME) == 0) {
		default_data = things_strdup(KEY_MANAGER_STR_DEFAULT_DATA);
	} else if (strcmp(name, FIRMWARE_PROPERTY_STATE) == 0) {
		default_data = things_strdup(KEY_MANAGER_INT_DEFAULT_DATA);
	} else if (strcmp(name, FIRMWARE_PROPERTY_RESULT) == 0) {
		default_data = things_strdup(KEY_MANAGER_INT_DEFAULT_DATA);
	} else if (strcmp(name, FIRMWARE_PROPERTY_NEW_VERSION) == 0) {
		default_data = things_strdup(KEY_MANAGER_STR_DEFAULT_DATA);
	} else if (strcmp(name, FIRMWARE_PROPERTY_PACKAGE_URI) == 0) {
		default_data = things_strdup(KEY_MANAGER_STR_DEFAULT_DATA);
	} else {
		THINGS_LOG_D(TAG, "Invalid key[%s]", name);
		return NULL;
	}

	if (0 != key_manager_set_data(name, default_data)) {
		THINGS_LOG_D(TAG, "failed set default data [%s]", name);
	}

	return default_data;
}

char *key_manager_get_data(const char *name)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (!name) {
		THINGS_LOG_E(TAG, "name or data is NULL");
		if (name) {
			THINGS_LOG_E(TAG, "name [%s]", name);
		}
		return NULL;
	}

	if (root == NULL) {
		THINGS_LOG_E(TAG, "cJSON_Parse is null.");
		return NULL;
	}

	cJSON *json_data = cJSON_GetObjectItem(root, name);

	if (json_data == NULL) {
		THINGS_LOG_E(TAG, "json_data == NULL");
		return NULL;
	}

	char *text_data = things_malloc(sizeof(char) * strlen(json_data->valuestring) + 1);
	strncpy(text_data, json_data->valuestring, strlen(json_data->valuestring) + 1);

	THINGS_LOG_E(TAG, "value [%s]", text_data);

	return text_data;
}

int key_manager_remove_data(const char *name)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	return 0;
}

int key_manager_save_data()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	char *json_update = cJSON_Print(root);
	if (set_json_string_into_file(KEY_MANAGER_FILE_NAME, json_update) == 0) {
		THINGS_LOG_E(TAG, "reset fail.");
		return -1;
	}

	return 0;
}

int key_manager_reset_data()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	cJSON_Delete(root);

	root = cJSON_Parse((const char *)key_manager_original_json_str);

	if (root == NULL) {
		THINGS_LOG_E(TAG, "cJSON_Parse is null.");
		return -1;
	}

	key_manager_set_default_data(FIRMWARE_PROPERTY_UPDATE);
	key_manager_set_default_data(FIRMWARE_PROPERTY_UPDATE_TIME);
	key_manager_set_default_data(FIRMWARE_PROPERTY_STATE);
	key_manager_set_default_data(FIRMWARE_PROPERTY_RESULT);
	key_manager_set_default_data(FIRMWARE_PROPERTY_NEW_VERSION);
	key_manager_set_default_data(FIRMWARE_PROPERTY_PACKAGE_URI);

	return key_manager_save_data();
}

char *platform_get_data(const char *name)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (!name) {
		THINGS_LOG_E(TAG, "name is NULL");
		return NULL;
	}

	if (strcmp(name, FIRMWARE_PROPERTY_CURRENT_VERSION) == 0) {
		char *version = dm_get_firmware_version();
		char *ret = things_malloc(sizeof(char) * strlen(version) + 1);
		strncpy(ret, version, strlen(version) + 1);
		THINGS_LOG_D(TAG, "value [%s]", ret);
		return ret;
	} else if (strcmp(name, FIRMWARE_PROPERTY_VENDER) == 0) {
		char *vid = dm_get_vendor_id();
		char *ret = things_malloc(sizeof(char) * strlen(vid) + 1);
		strncpy(ret, vid, strlen(vid) + 1);
		THINGS_LOG_D(TAG, "value [%s]", ret);
		return ret;
	} else if (strcmp(name, FIRMWARE_PROPERTY_MODEL) == 0) {
		char *model_no = dm_get_model_number();
		char *ret = things_malloc(sizeof(char) * strlen(model_no) + 1);
		strncpy(ret, model_no, strlen(model_no) + 1);
		THINGS_LOG_D(TAG, "value [%s]", ret);
		return ret;
	} else {
		THINGS_LOG_D(TAG, "Invalid parameter[%s]", name);
	}

	return NULL;
}

fmwup_data_s *fmwup_data_get_properties()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	fmwup_data_s *fmwup_get_data = NULL;
	fmwup_get_data = (fmwup_data_s *) things_calloc(1, sizeof(fmwup_data_s));

	char *temp = NULL;

	temp = key_manager_get_data(FIRMWARE_PROPERTY_STATE);
	if (temp) {
		fmwup_get_data->state = (int64_t) atoi(temp);
		THINGS_LOG_D(TAG, "%s:%lld", FIRMWARE_PROPERTY_STATE, fmwup_get_data->state);
		things_free(temp);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_STATE);
	}

	temp = key_manager_get_data(FIRMWARE_PROPERTY_RESULT);
	if (temp) {
		fmwup_get_data->result = (int64_t) atoi(temp);
		THINGS_LOG_D(TAG, "%s:%lld", FIRMWARE_PROPERTY_RESULT, fmwup_get_data->result);
		things_free(temp);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_RESULT);
	}

	temp = key_manager_get_data(FIRMWARE_PROPERTY_UPDATE);
	if (temp) {
		fmwup_get_data->update = (int64_t) atoi(temp);
		THINGS_LOG_D(TAG, "%s:%lld", FIRMWARE_PROPERTY_UPDATE, fmwup_get_data->update);
		things_free(temp);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_UPDATE);
	}

	fmwup_get_data->update_time = key_manager_get_data(FIRMWARE_PROPERTY_UPDATE_TIME);
	if (fmwup_get_data->update_time) {
		THINGS_LOG_D(TAG, "%s:%s", FIRMWARE_PROPERTY_UPDATE_TIME, fmwup_get_data->update_time);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_UPDATE_TIME);
	}

	fmwup_get_data->new_version = key_manager_get_data(FIRMWARE_PROPERTY_NEW_VERSION);
	if (fmwup_get_data->new_version) {
		THINGS_LOG_D(TAG, "%s:%s", FIRMWARE_PROPERTY_NEW_VERSION, fmwup_get_data->new_version);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_NEW_VERSION);
	}

	fmwup_get_data->package_uri = key_manager_get_data(FIRMWARE_PROPERTY_PACKAGE_URI);
	if (fmwup_get_data->package_uri) {
		THINGS_LOG_D(TAG, "%s:%s", FIRMWARE_PROPERTY_PACKAGE_URI, fmwup_get_data->package_uri);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_PACKAGE_URI);
	}

	fmwup_get_data->current_version = platform_get_data(FIRMWARE_PROPERTY_CURRENT_VERSION);
	if (fmwup_get_data->current_version) {
		THINGS_LOG_D(TAG, "%s:%s", FIRMWARE_PROPERTY_CURRENT_VERSION, fmwup_get_data->current_version);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_CURRENT_VERSION);
	}

	fmwup_get_data->manufacturer = platform_get_data(FIRMWARE_PROPERTY_VENDER);
	if (fmwup_get_data->manufacturer) {
		THINGS_LOG_D(TAG, "%s:%s", FIRMWARE_PROPERTY_VENDER, fmwup_get_data->manufacturer);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_VENDER);
	}

	fmwup_get_data->model_name = platform_get_data(FIRMWARE_PROPERTY_MODEL);
	if (fmwup_get_data->model_name) {
		THINGS_LOG_D(TAG, "%s:%s", FIRMWARE_PROPERTY_MODEL, fmwup_get_data->model_name);
	} else {
		THINGS_LOG_D(TAG, "failed to get data [%s]", FIRMWARE_PROPERTY_MODEL);
	}
	return fmwup_get_data;
}

int fmwup_data_set_property_int64(const char *name, int64_t data)
{
	if (!name) {
		THINGS_LOG_E(TAG, "name is NULL");
		return -1;
	}

	char temp[KEY_MANAGER_MAX_DATA_LENGTH] = { 0, };
	snprintf(temp, sizeof(temp), "%lld", data);

	return (fmwup_data_set_property(name, temp));
}

int fmwup_data_set_property(const char *name, char *data)
{
	if (!name) {
		THINGS_LOG_E(TAG, "name is NULL");
		return -1;
	}

	if (!data) {
		THINGS_LOG_E(TAG, "data is NULL");
		return -1;
	}

	THINGS_LOG_V(TAG, "key [%s], data[%s]", name, data);

	if ((strcmp(name, FIRMWARE_PROPERTY_UPDATE) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_UPDATE_TIME) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_STATE) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_RESULT) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_NEW_VERSION) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_PACKAGE_URI) == 0)
	   ) {
		return key_manager_set_data(name, data);
	} else {
		THINGS_LOG_D(TAG, "Invalid key[%s]", name);
		return -1;
	}

	return 0;
}

char *fmwup_data_get_property(const char *name)
{
	if (!name) {
		THINGS_LOG_E(TAG, "name is NULL");
		return NULL;
	}

	THINGS_LOG_V(TAG, "key [%s]", name);

	if ((strcmp(name, FIRMWARE_PROPERTY_UPDATE) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_UPDATE_TIME) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_STATE) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_RESULT) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_NEW_VERSION) == 0)
		|| (strcmp(name, FIRMWARE_PROPERTY_PACKAGE_URI) == 0)
	   ) {
		return key_manager_get_data(name);
	} else if ((strcmp(name, FIRMWARE_PROPERTY_CURRENT_VERSION) == 0)
			   || (strcmp(name, FIRMWARE_PROPERTY_VENDER) == 0)
			   || (strcmp(name, FIRMWARE_PROPERTY_MODEL) == 0)
			  ) {
		return platform_get_data(name);
	} else {
		THINGS_LOG_D(TAG, "Invalid key[%s]", name);
		return NULL;
	}

	return NULL;
}
