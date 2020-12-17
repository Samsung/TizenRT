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
#include <stdlib.h>
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

	/*  create time */
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

	/*  create interval  */
	cJSON *period = NULL;
	char *str_interval = NULL;
	int interval = RR_INTERVAL;
	(void)rr_get_interval(&str_interval);

	if (str_interval) {
		interval = atoi(str_interval);
	}
	period = cJSON_CreateNumber(interval);
	if (!period) {
		RR_ERR;
		goto cleanup;
	}
	cJSON_AddItemToObject(rssi, "period", period);

	/*  create duration */
	uint32_t duration = 0;
	rr_get_duration(&duration);
	cJSON *jduration = cJSON_CreateNumber((double)duration);
	cJSON_AddItemToObject(rssi, "duration", jduration);

	/*  create location */
	char *str_location = NULL;
	(void)rr_get_location(&str_location);
	if (!str_location) {
		str_location = RR_LOCATION;
	}
	cJSON *location = cJSON_CreateString(str_location);
	if (!location) {
		RR_ERR;
		goto cleanup;
	}
	cJSON_AddItemToObject(rssi, "loc", location);

	/*  create rssi array */
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
