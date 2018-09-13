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

#include <stdio.h>

#include "utils/things_malloc.h"
#include "things_resource.h"
#include "framework/things_data_manager.h"

#include "fmwup_util_data.h"
#include "fmwup_util_internal.h"

#include "logging/things_logger.h"

#define TAG "[things_fota]"
#define DOWNLOAD_PATH "/mnt/firmware"

int fmwup_get_data(things_resource_s *target_resource)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	things_representation_s *rep = NULL;

	if (target_resource->rep == NULL) {
		rep = things_create_representation_inst(NULL);
		target_resource->things_set_representation(target_resource, rep);
	} else {
		rep = target_resource->rep;
	}

	fmwup_data_s *fmwup_data;
	fmwup_data = fmwup_data_get_properties();

	char *update;
	update = fmwup_data_get_update_string(fmwup_data->update);

	rep->things_set_value(rep, FIRMWARE_PROPERTY_UPDATE, update);
	rep->things_set_value(rep, FIRMWARE_PROPERTY_UPDATE_TIME, fmwup_data->update_time);
	rep->things_set_int_value(rep, FIRMWARE_PROPERTY_STATE, (int64_t)fmwup_data->state);
	rep->things_set_int_value(rep, FIRMWARE_PROPERTY_RESULT, (int64_t)fmwup_data->result);

	rep->things_set_value(rep, FIRMWARE_PROPERTY_NEW_VERSION, fmwup_data->new_version);
	rep->things_set_value(rep, FIRMWARE_PROPERTY_PACKAGE_URI, fmwup_data->package_uri);

	rep->things_set_value(rep, FIRMWARE_PROPERTY_CURRENT_VERSION, fmwup_data->current_version);
	rep->things_set_value(rep, FIRMWARE_PROPERTY_VENDER, fmwup_data->manufacturer);
	rep->things_set_value(rep, FIRMWARE_PROPERTY_MODEL, fmwup_data->model_name);

	THINGS_LOG_V(TAG, "firmware_version = %s", fmwup_data->current_version);
	THINGS_LOG_V(TAG, "vendor_id = %s", fmwup_data->manufacturer);
	THINGS_LOG_V(TAG, "model_number = %s", fmwup_data->model_name);

	things_free(fmwup_data->update_time);
	things_free(fmwup_data->new_version);
	things_free(fmwup_data->package_uri);
	things_free(fmwup_data->current_version);
	things_free(fmwup_data->manufacturer);
	things_free(fmwup_data->model_name);
	things_free(fmwup_data);

	fmwup_internal_propagate_cond_signal();

	return OC_EH_OK;
}

int fmwup_set_data(things_resource_s *target_res)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	OCEntityHandlerResult eh_result = OC_EH_ERROR;

	things_representation_s *rep = NULL;

	if (target_res->rep != NULL) {
		rep = target_res->rep;
	} else {
		THINGS_LOG_E(TAG, "fmwup_set_data is NULL");
		return OC_EH_ERROR;
	}

	int update;
	char *update_str;
	char *new_version;
	char *package_uri;

	rep->things_get_value(rep, FIRMWARE_PROPERTY_UPDATE, &update_str);
	rep->things_get_value(rep, FIRMWARE_PROPERTY_NEW_VERSION, &new_version);
	rep->things_get_value(rep, FIRMWARE_PROPERTY_PACKAGE_URI, &package_uri);

	update = fmwup_data_get_update_int64(update_str);

	if (fmwup_data_set_property_int64(FIRMWARE_PROPERTY_UPDATE, update) != 0) {
		THINGS_LOG_V(TAG, "fmwup_data_set_property [%s] failed", FIRMWARE_PROPERTY_UPDATE);
	}

	if (fmwup_data_set_property(FIRMWARE_PROPERTY_NEW_VERSION, new_version) != 0) {
		THINGS_LOG_V(TAG, "fmwup_data_set_property [%s] failed", FIRMWARE_PROPERTY_NEW_VERSION);
	}

	if (fmwup_data_set_property(FIRMWARE_PROPERTY_PACKAGE_URI, package_uri) != 0) {
		THINGS_LOG_V(TAG, "fmwup_data_set_property [%s] failed", FIRMWARE_PROPERTY_PACKAGE_URI);
	}

	THINGS_LOG_V(TAG, "update : %d, new_version : %s, package_uri : %s\n", update, new_version, package_uri);

	fmwup_internal_update_command(update);

	return OC_EH_OK;
}

int fmwup_check_firmware_upgraded(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	char *temp_state = fmwup_data_get_property(FIRMWARE_PROPERTY_STATE);
	if (!temp_state) {
		THINGS_LOG_E(TAG, "failed to get state property");
		return FMWUP_ERROR_OPERATION_FAILED;
	}

	int state = FMWUP_STATE_IDLE;
	state = atoi(temp_state);
	things_free(temp_state);

	if (state == FMWUP_STATE_UPDATING) {
		THINGS_LOG_V(TAG, "***Firmware Upgrade Done***");

		// TODO :: It should be checked and set result value according to upgrade result
		/* Initialize - new platform information - */
		char *new_version = fmwup_data_get_property(FIRMWARE_PROPERTY_NEW_VERSION);
		if (new_version) {
			fmwup_data_set_property(FIRMWARE_PROPERTY_CURRENT_VERSION, new_version);
			things_free(new_version);
		} else {
			THINGS_LOG_E(TAG, "failed get new version");
		}

		/* Initialize - new firmware information - */
		key_manager_reset_data();

		fmwup_internal_propagate_resource(FMWUP_STATE_IDLE, FMWUP_RESULT_SUCCESS, false);

	} else {
		THINGS_LOG_V(TAG, "is not updating, state[%d]", state);
		return FMWUP_ERROR_OPERATION_FAILED;
	}

	return FMWUP_ERROR_NONE;
}

int fmwup_initialize(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (key_manager_init() < 0) {
		THINGS_LOG_D(TAG, "fail key_manager_init()");
		return -1;
	}

	return 0;
}
