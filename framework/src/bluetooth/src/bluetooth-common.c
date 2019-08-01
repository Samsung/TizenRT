/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tinyara/config.h>
#ifdef GLIB_SUPPORT
#include <glib.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
#include <bluetooth/bluetooth_extension.h>

#include "bt-adaptation-common.h"
#include "bluetooth_private.h"

static bool is_initialized = false;

/* These structs will be used in later */

static bt_event_sig_event_slot_s bt_event_slot_container[BT_EVENT_MAX] = { {NULL, NULL}, };


typedef struct {
	int err;
	char *str;
} bt_err2str_table_t;

static bt_err2str_table_t err2str[] = {
	{ BT_ERROR_CANCELLED, "CANCELLED" },
	{ BT_ERROR_INVALID_PARAMETER, "INVALID_PARAMETER" },
	{ BT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY" },
	{ BT_ERROR_RESOURCE_BUSY, "RESOURCE_BUSY" },
	{ BT_ERROR_TIMED_OUT, "TIMED_OUT" },
	{ BT_ERROR_NOW_IN_PROGRESS, "NOW_IN_PROGRESS" },
	{ BT_ERROR_NOT_INITIALIZED, "NOT_INITIALIZED" },
	{ BT_ERROR_NOT_ENABLED, "DEVICE_NOT_ENABLED" },
	{ BT_ERROR_ALREADY_DONE, "ALREADY_DONE" },
	{ BT_ERROR_OPERATION_FAILED, "OPERATION_FAILED" },
	{ BT_ERROR_NOT_IN_PROGRESS, "NOT_IN_PROGRESS" },
	{ BT_ERROR_REMOTE_DEVICE_NOT_BONDED, "REMOTE_DEVICE_NOT_BONDED" },
	{ BT_ERROR_AUTH_REJECTED, "AUTH_REJECTED" },
	{ BT_ERROR_AUTH_FAILED, "AUTH_FAILED" },
	{ BT_ERROR_REMOTE_DEVICE_NOT_FOUND, "REMOTE_DEVICE_NOT_FOUND" },
	{ BT_ERROR_SERVICE_SEARCH_FAILED, "SERVICE_SEARCH_FAILED" },
	{ BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED, "REMOTE_DEVICE_NOT_CONNECTED" },
	{ BT_ERROR_PERMISSION_DENIED, "PERMISSION_DENIED" },
	{ BT_ERROR_SERVICE_NOT_FOUND, "SERVICE_NOT_FOUND" },
	{ BT_ERROR_NOT_SUPPORTED, "NOT_SUPPORTD" },
	{ BT_ERROR_NO_DATA, "NO_DATA" },
	{ BT_ERROR_QUOTA_EXCEEDED, "QUOTA EXCEEDED" },
	{ BT_ERROR_AGAIN, "AGAIN" },
	{ BT_ERROR_DEVICE_POLICY_RESTRICTION, "DEVICE_POLICY_RESTRICTION" },
	{ 0, "UNKNOWN" }
};

typedef struct {
	int event;
	int index;
} bt_event2index_table_t;

/*
 *  Public Functions
 */

int bt_initialize(void)
{
	printf("bt_initialize() entered!!!\n");
	bt_adapt_init();

	is_initialized = true;

	return BT_ERROR_NONE;
}

int bt_deinitialize(void)
{
	BT_CHECK_INIT_STATUS();

	bt_adapt_deinit();

	is_initialized = false;

	return BT_ERROR_NONE;
}

int bt_get_uuid_name(const char *uuid, char **name)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/*
 *  Common Functions
 */
int _bt_check_init_status(void)
{
	if (is_initialized != true) {
		BT_ERR("NOT_INITIALIZED(0x%08x)", BT_ERROR_NOT_INITIALIZED);
		return BT_ERROR_NOT_INITIALIZED;
	}

	return BT_ERROR_NONE;
}

void _bt_set_cb(int events, void *callback, void *user_data)
{
	bt_event_slot_container[events].callback = callback;
	bt_event_slot_container[events].user_data = user_data;
}

void _bt_unset_cb(int events)
{
	if (bt_event_slot_container[events].callback != NULL) {
		bt_event_slot_container[events].callback = NULL;
		bt_event_slot_container[events].user_data = NULL;
	}
}

bool _bt_check_cb(int events)
{
	return (bt_event_slot_container[events].callback != NULL) ? true : false;
}

void _bt_convert_address_to_hex(unsigned char **addr_hex, const char *addr_str)
{
	char *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

	if (addr_str == NULL || addr_str[0] == '\0')
		return;

	*addr_hex[0] = strtol(addr_str, &ptr5, 16);
	*addr_hex[1] = strtol(ptr5 + 1, &ptr4, 16);
	*addr_hex[2] = strtol(ptr4 + 1, &ptr3, 16);
	*addr_hex[3] = strtol(ptr3 + 1, &ptr2, 16);
	*addr_hex[4] = strtol(ptr2 + 1, &ptr1, 16);
	*addr_hex[5] = strtol(ptr1 + 1, NULL, 16);
}

int _bt_convert_address_to_string(char **addr_str, unsigned char *addr_hex)
{
	char address[18] = { 0, };

	BT_CHECK_INPUT_PARAMETER(addr_hex);

	snprintf(address, 18, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X", addr_hex[0], addr_hex[1], addr_hex[2], addr_hex[3], addr_hex[4], addr_hex[5]);
	*addr_str = strdup(address);

	if (*addr_str != NULL)
		return BT_ERROR_NONE;
	else
		return BT_ERROR_OUT_OF_MEMORY; /* LCOV_EXCL_LINE */
}

int _bt_get_error_code(int origin_error)
{
	return BT_ERROR_OPERATION_FAILED;
}

char *_bt_convert_uuid_to_uuid128(const char *uuid)
{
	int len;
	char *uuid128;

	len = strlen(uuid);

	switch (len) {
	case 4:         /* UUID 16bits */
#ifdef GLIB_SUPPORTED
		uuid128 = g_strdup_printf("0000%s-0000-1000-8000-00805f9b34fb", uuid);
#else
		len = snprintf(NULL, 0, "0000%s-0000-1000-8000-00805f9b34fb", uuid);
		uuid128 = malloc(len + 1);
		snprintf(uuid128, len + 1,  "0000%s-0000-1000-8000-00805f9b34fb", uuid);
#endif
		break;

	case 8:         /* UUID 32bits */
#ifdef GLIB_SUPPORTED
		uuid128 = g_strdup_printf("%s-0000-1000-8000-00805f9b34fb", uuid); /* LCOV_EXCL_LINE */
#else
		len = snprintf(NULL, 0, "%s-0000-1000-8000-00805f9b34fb", uuid);
		uuid128 = malloc(len + 1);
		snprintf(uuid128, len + 1,  "%s-0000-1000-8000-00805f9b34fb", uuid);
#endif
		break; /* LCOV_EXCL_LINE */

	case 36:        /* UUID 128bits */
		uuid128 = strdup(uuid); /* LCOV_EXCL_LINE */
		break; /* LCOV_EXCL_LINE */

	default:
		return NULL;
	}

	return uuid128;
}

void _bt_free_bt_device_info_s(bt_device_info_s *device_info)
{
	int i = 0;

	if (device_info == NULL)
		return;

	if (device_info->remote_name != NULL)
		free(device_info->remote_name);

	if (device_info->remote_address != NULL)
		free(device_info->remote_address);

	if (device_info->service_uuid != NULL) {
		for (i = 0; i < device_info->service_count; i++) {
			if (device_info->service_uuid[i] != NULL)
			free(device_info->service_uuid[i]);
		}
		free(device_info->service_uuid);
	}

	if (device_info->manufacturer_data != NULL)
		free(device_info->manufacturer_data);

	free(device_info);
	device_info = NULL;
}

char *_bt_convert_error_to_string(int error)
{
	int i;

	for (i = 0; err2str[i].err != 0; i++) {
		if (err2str[i].err == error)
		return err2str[i].str;
	}

	return "UNKNOWN"; /* LCOV_EXCL_LINE */
}

