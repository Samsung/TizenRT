/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <json/cJSON.h>
#include "rreport_config.h"

char *rr_generate_message(int *data, int size)
{
	char *string = NULL;
	cJSON *rssi = cJSON_CreateObject();
	if (!rssi) {
		RR_ERR;
		return NULL;
	}
	char cur_time[64];

	if (rr_get_time(cur_time, 64) < 0) {
		RR_ERR;
		goto cleanup;
	}

	cJSON *time = cJSON_CreateString(cur_time);
	if (!time) {
		RR_ERR;
		goto cleanup;
	}

	cJSON_AddItemToObject(rssi, "time", time);

	char str_interval[10] = {0};
	snprintf(str_interval, sizeof(str_interval), "%d", RR_INTERVAL);

	cJSON *period = cJSON_CreateString(str_interval);
	if (!period) {
		RR_ERR;
		goto cleanup;
	}

	cJSON_AddItemToObject(rssi, "period", period);

	cJSON *item = cJSON_CreateIntArray(data, size);;
	if (!item) {
		RR_ERR;
		goto cleanup;
	}

	cJSON_AddItemToObject(rssi, "rssi", item);

	string = cJSON_PrintUnformatted(rssi);
	if (!string) {
		RR_ERR;
	}

cleanup:
	cJSON_Delete(rssi);

	return string;
}
