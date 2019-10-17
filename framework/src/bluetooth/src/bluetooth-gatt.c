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
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <tinyara/math.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
#include <bluetooth/bluetooth_type_internal.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "bt-adaptation-gatt.h"
#include "bluetooth_private.h"

#define BT_ADDR_STR_LEN		17

#ifdef GLIB_SUPPORTED
static GSList *gatt_client_list = NULL;
static GSList *gatt_handle_list = NULL;
static GSList *gatt_server_list = NULL;
#else
static sq_queue_t gatt_client_list;
static sq_queue_t gatt_handle_list;
static sq_queue_t gatt_server_list;
#endif

int instance_id;
static bool is_gatt_server_initialized = false;
static bool is_gatt_server_started = false;

static bt_gatt_service_s* __bt_gatt_service_create(const char *uuid,
						   bt_gatt_service_type_e type);
#ifdef GLIB_SUPPORTED
static void __bt_gatt_free_service(bt_gatt_h gatt_handle);
#else
static void __bt_gatt_free_service(sq_queue_t *gatt_handle);
#endif

#define BT_CHECK_GATT_SUPPORT() \
{ \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_LE); \
	BT_CHECK_SUPPORTED_FEATURES(BT_FEATURE_GATT_SERVER, BT_FEATURE_GATT_CLIENT); \
}

#define BT_CHECK_GATT_SERVER_SUPPORT() \
{ \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_LE); \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_GATT_SERVER); \
}

#define BT_CHECK_GATT_CLIENT_SUPPORT() \
{ \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_LE); \
	BT_CHECK_SUPPORTED_FEATURE(BT_FEATURE_GATT_CLIENT); \
}

#define BT_CHECK_GATT_SERVER_INIT_STATUS() { \
	if (__bt_check_gatt_server_init_status() == BT_ERROR_NOT_INITIALIZED) { \
		BT_PRT("NOT_INITIALIZED(0x%08x)", BT_ERROR_NOT_INITIALIZED); \
		return BT_ERROR_NOT_INITIALIZED; \
	} \
}

#ifdef GLIB_SUPPORTED
#define BT_VALIDATE_GATT_HANDLE(h1) \
{ \
	GSList *l; \
	bool valid = FALSE; \
	for (l = gatt_handle_list; l; l = g_slist_next(l)) { \
		bt_gatt_h h2 = (bt_gatt_h)l->data; \
		if (h1 == h2) { \
			BT_INFO("Handle matched [%p]", h2); \
			valid = TRUE; break; \
		} \
	} \
	if (valid == FALSE) { \
		BT_ERR("App Handle [%p] did not match with any stored handles!!! Must be Invalid Handle!!", h1); \
		return BT_ERROR_INVALID_PARAMETER; \
	} \
}
#else
#define BT_VALIDATE_GATT_HANDLE(h1) \
{ \
	bool valid = FALSE; \
	sq_entry_t *h2 = (sq_entry_t *)sq_peek(&gatt_handle_list); \
	while (h2) { \
		if (h2 == h1) { \
			BT_INFO("Handle matched [%p]", h2); \
			valid = TRUE; break; \
		} \
		h2 = (sq_entry_t *)sq_next(h2); \
	} \
	if (valid == FALSE) { \
		BT_ERR("App Handle [%p] did not match with any stored handles!!! Must be Invalid Handle!!", h1); \
		return BT_ERROR_INVALID_PARAMETER; \
	} \
}
#endif

#ifndef GLIB_SUPPORTED
static int __bt_sq_queue_length(sq_queue_t *q)
{
	int length = 0;
	sq_entry_t *entry = NULL;

	if (q == NULL)
		return 0;

	entry = (sq_entry_t *)sq_peek(q);
	while (entry) {
		length++;
		entry = (sq_entry_t *)sq_next(entry);
	}
	return length;
}

static sq_entry_t *__bt_sq_queue_find(sq_queue_t *q, sq_entry_t *e)
{
	sq_entry_t *entry = NULL;

	if (e == NULL)
		return NULL;

	entry = (sq_entry_t *)sq_peek(q);
	while (entry) {
		if (entry == e)
			return entry;
		entry = (sq_entry_t *)sq_next(entry);
	}
	return NULL;
}
#endif
/* LCOV_EXCL_START */
void __bt_string_to_uuid_hex(const char *str, unsigned char *uuid)
{
	uint32_t uuid0, uuid4;
	uint16_t uuid1, uuid2, uuid3, uuid5;

	sscanf(str, "%08x-%04hx-%04hx-%04hx-%08x%04hx",
			&uuid0, &uuid1, &uuid2, &uuid3, &uuid4, &uuid5);

	uuid0 = htonl(uuid0);
	uuid1 = htons(uuid1);
	uuid2 = htons(uuid2);
	uuid3 = htons(uuid3);
	uuid4 = htonl(uuid4);
	uuid5 = htons(uuid5);

	memcpy(&(uuid[0]), &uuid0, 4);
	memcpy(&(uuid[4]), &uuid1, 2);
	memcpy(&(uuid[6]), &uuid2, 2);
	memcpy(&(uuid[8]), &uuid3, 2);
	memcpy(&(uuid[10]), &uuid4, 4);
	memcpy(&(uuid[14]), &uuid5, 2);
	return;
}

static int __bt_check_gatt_server_init_status(void)
{
	if (is_gatt_server_initialized != true) {
		BT_ERR("NOT_INITIALIZED(0x%08x)", BT_ERROR_NOT_INITIALIZED);
		return BT_ERROR_NOT_INITIALIZED;
	}

	return BT_ERROR_NONE;
}

#ifdef GLIB_SUPPORTED
static int __get_gatt_handle_by_uuid(GSList *list, const char *uuid,
		bt_gatt_h *gatt_handle)
{
	GSList *l;
	char *uuid128_a;
	char *uuid128_b;

	uuid128_a = _bt_convert_uuid_to_uuid128(uuid);
	if (uuid128_a == NULL) {
		BT_ERR("Wrong type of uuid : %s", uuid);
		return BT_ERROR_INVALID_PARAMETER;
	}

	for (l = list; l; l = g_slist_next(l)) {
		bt_gatt_common_s *common = (bt_gatt_common_s *)l->data;

		uuid128_b = _bt_convert_uuid_to_uuid128(common->uuid);

		if (uuid128_b == NULL) {
			BT_ERR("__get_gatt_handle_by_uuid Error Parameter are NULL..\n");
			continue;
		} else if (g_ascii_strcasecmp(uuid128_a, uuid128_b) == 0) {
			g_free(uuid128_b);
			break;
		}

		g_free(uuid128_b);
	}
	g_free(uuid128_a);

	if (!l) {
		char *name = NULL;
		bt_get_uuid_name(uuid, &name);
		BT_INFO("Find uuid : %s [%s]", uuid, name);
		g_free(name);
		for (l = list; l; l = g_slist_next(l)) {
			bt_gatt_common_s *common = (bt_gatt_common_s *)l->data;
			bt_get_uuid_name(common->uuid, &name);
			BT_INFO("%s [%s]", common->uuid, name);
			g_free(name);
		}
		return BT_ERROR_NO_DATA;
	}

	*gatt_handle = l->data;

	return BT_ERROR_NONE;
}
#else
static int __get_gatt_handle_by_uuid(sq_queue_t *list, const char *uuid,
		bt_gatt_h *gatt_handle)
{
	char *uuid128_a;
	char *uuid128_b;

	uuid128_a = _bt_convert_uuid_to_uuid128(uuid);
	if (uuid128_a == NULL) {
		BT_ERR("Wrong type of uuid : %s", uuid);
		return BT_ERROR_INVALID_PARAMETER;
	}

	bt_gatt_common_s *common = (bt_gatt_common_s *)sq_peek(list);
	while (common) {
		uuid128_b = _bt_convert_uuid_to_uuid128(common->uuid);

		if (uuid128_b == NULL) {
			BT_ERR("__get_gatt_handle_by_uuid Error Parameter are NULL..\n");
			continue;
		} else if (strcasecmp(uuid128_a, uuid128_b) == 0) {
			FREE(uuid128_b);
			break;
		}
		
		FREE(uuid128_b);
		common = (bt_gatt_common_s *)sq_next(common);
	}
	FREE(uuid128_a);
	
	if (!common) {
		char *name = NULL;
		bt_get_uuid_name(uuid, &name);
		BT_INFO("Find uuid : %s [%s]", uuid, name);
		FREE(name);
		common = (bt_gatt_common_s *)sq_peek(list);
		while (common) {
			bt_get_uuid_name(common->uuid, &name);
			BT_INFO("%s [%s]", common->uuid, name);
			FREE(name);
			common = (bt_gatt_common_s *)sq_next(common);
		}
		return BT_ERROR_NO_DATA;
	}

	*gatt_handle = (bt_gatt_common_s *)common;

	return BT_ERROR_NONE;
}
#endif

bt_gatt_client_h _bt_gatt_get_client(const char *remote_addr)
{
#ifdef GLIB_SUPPORTED
	GSList *l;

	for (l = gatt_client_list; l; l = g_slist_next(l)) {
		bt_gatt_client_s *client_s = l->data;

		if (client_s == NULL)
			continue;

		if (!g_ascii_strcasecmp(client_s->remote_address, remote_addr))
			return (bt_gatt_client_h)client_s;
	}
#else
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)sq_peek(&gatt_client_list);
	if (client_s == NULL) {
		BT_INFO("No GATT Client");
		return NULL;
	}

	while (client_s) {
		if (!strcasecmp(client_s->remote_address, remote_addr))
			return (bt_gatt_client_h)client_s;
		client_s = (bt_gatt_client_s *)sq_next(client_s);
	}

	return NULL;
#endif
}

#ifdef GLIB_SUPPORTED
const GSList* _bt_gatt_get_server_list(void)
{
	return gatt_server_list;
}
#else
const sq_queue_t *_bt_gatt_get_server_list(void)
{
	return &gatt_server_list;
}
#endif

#ifdef GLIB_SUPPORTED
static void __bt_gatt_client_handle_destroy(bt_gatt_h gatt_handle)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s*)gatt_handle;

	if (handle->type == BT_GATT_TYPE_SERVICE)
		bt_gatt_service_destroy(gatt_handle);
	else if (handle->type == BT_GATT_TYPE_CHARACTERISTIC)
		bt_gatt_characteristic_destroy(gatt_handle);
	else if (handle->type == BT_GATT_TYPE_DESCRIPTOR)
		bt_gatt_descriptor_destroy(gatt_handle);
}
#else
static void __bt_gatt_client_handle_destroy(sq_queue_t *gatt_handle)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)sq_peek(gatt_handle);

	if (handle->type == BT_GATT_TYPE_SERVICE)
		bt_gatt_service_destroy(gatt_handle);
	else if (handle->type == BT_GATT_TYPE_CHARACTERISTIC)
		bt_gatt_characteristic_destroy(gatt_handle);
	else if (handle->type == BT_GATT_TYPE_DESCRIPTOR)
		bt_gatt_descriptor_destroy(gatt_handle);
}
#endif

bt_gatt_h _bt_gatt_client_add_service(bt_gatt_client_h client,
		const char *uuid, int __instance_id)
{
	int ret;
	bt_gatt_client_s *client_s = (bt_gatt_client_s*)client;
	bt_gatt_service_s *svc = NULL;
	bt_gatt_svc_prop_t property;
	bt_gatt_handle_prop_t service;

	if (client == NULL || uuid == NULL) {
		BT_ERR("Invalid parameter");
		return NULL;
	}

	memset(&property, 0x00, sizeof(bt_gatt_svc_prop_t));
	memset(&service, 0x00, sizeof(bt_gatt_handle_prop_t));

	__bt_string_to_uuid_hex(uuid, service.uuid);
	service.instance_id = __instance_id;

	BT_INFO("GATT client add service: get all properties of this service");
	ret = bt_adapt_gatt_client_get_svc_property(client_s->remote_address,
			&service, &property);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("bt_adapt_gatt_client_get_svc_property is failed");
		return NULL;
	}

	BT_INFO("Service properties are retrieved successfully, now create and add service object");
	svc = __bt_gatt_service_create(property.uuid,
			BT_GATT_SERVICE_TYPE_PRIMARY);
	if (svc == NULL) {
		BT_ERR("svc is NULL");
		return NULL;
	}
	svc->role = BT_GATT_ROLE_CLIENT;
	svc->instance_id = __instance_id;
	svc->parent = (void *)client_s;
	svc->is_included_service = false;

	/* Copy included service handles and charc handles in just created service object */
	memcpy(&svc->svc_include_handles, &property.include_handles, sizeof(bt_gatt_handle_info_t));
	BT_INFO("Total number of Included service handles [%d]", svc->svc_include_handles.count);
	memcpy(&svc->charc_handles, &property.char_handle, sizeof(bt_gatt_handle_info_t));
	BT_INFO("Total number of Characteristic handles [%d]", svc->charc_handles.count);

	bt_adapt_gatt_free_svc_property(&property);

#ifdef GLIB_SUPPORTED
	client_s->services = g_slist_append(client_s->services, svc);
	BT_INFO("GATT service added , current count of Client services [%d]",
		g_slist_length(client_s->services));
#else
	BT_INFO("GATT service added , current count of Client services [%d]",
		__bt_sq_queue_length(client_s->services));

	client_s->services = calloc(1, sizeof(sq_queue_t));
	sq_addlast((sq_entry_t *)svc, client_s->services);
#endif

	return svc;
}

#ifdef GLIB_SUPPORTED
int _bt_gatt_client_update_services(bt_gatt_client_h client)
{
	bt_gatt_client_s *client_s = client;
	bt_gatt_handle_info_t prim_svc;
	int ret;
	int i;

	if (!client_s->connected) {
		BT_INFO("Not connected");
		return BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED;
	}

	if (client_s->services_discovered) {
		BT_INFO("Already discovered");
		return BT_ERROR_ALREADY_DONE;
	}

	BT_INFO("Update services for remote GATT server [%s]",
			client_s->remote_address);

	ret = bt_adapt_gatt_client_get_primary_services(client_s->remote_address, &prim_svc);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("bt_adapt_gatt_client_get_primary_services is failed");
		return ret;
	}

	if (client_s->services)
		g_slist_free_full(client_s->services, __bt_gatt_free_service);
	client_s->services = NULL;

	if (prim_svc.count == 0) {
		BT_INFO("There is no service");
		return BT_ERROR_NONE;
	}

	BT_INFO("Services discovered");
	client_s->services_discovered = true;
	for (i = 0; i < prim_svc.count; i++) {
		BT_INFO("Service UUID[%d][%s] Instance ID[%d]",
				i, prim_svc.uuids[i], prim_svc.inst_id[i]);
		if (!_bt_gatt_client_add_service(client, prim_svc.uuids[i], prim_svc.inst_id[i])) {
			BT_ERR("_bt_gatt_client_add_service is failed [%s]",
					prim_svc.uuids[i]);
			client_s->services_discovered = false;
		}
	}
	return BT_ERROR_NONE;
}
#else
int _bt_gatt_client_update_services(bt_gatt_client_h client)
{
	bt_gatt_client_s *client_s = client;
	bt_gatt_handle_info_t prim_svc;
	int ret;
	int i;

	if (!client_s->connected) {
		BT_INFO("Not connected");
		return BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED;
	}

	if (client_s->services_discovered) {
		BT_INFO("Already discovered");
		return BT_ERROR_ALREADY_DONE;
	}

	BT_INFO("Update services for remote GATT server [%s]",
			client_s->remote_address);

	ret = bt_adapt_gatt_client_get_primary_services(client_s->remote_address, &prim_svc);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("bt_adapt_gatt_client_get_primary_services is failed");
		return ret;
	}

	__bt_gatt_free_service(client_s->services);

	if (prim_svc.count == 0) {
		BT_INFO("There is no service");
		return BT_ERROR_NONE;
	}

	BT_INFO("Services discovered");
	client_s->services_discovered = true;
	for (i = 0; i < prim_svc.count; i++) {
		BT_INFO("Service UUID[%d][%s] Instance ID[%d]",
				i, prim_svc.uuids[i], prim_svc.inst_id[i]);
		if (!_bt_gatt_client_add_service(client, prim_svc.uuids[i], prim_svc.inst_id[i])) {
			BT_ERR("_bt_gatt_client_add_service is failed [%s]",
					prim_svc.uuids[i]);
			client_s->services_discovered = false;
		}
	}
	return BT_ERROR_NONE;
}
#endif

int _bt_gatt_client_update_include_services(bt_gatt_h service)
{
	bt_gatt_service_s *svc = service;
	bt_gatt_client_s *client_s;
#ifdef GLIB_SUPPORTED
	GSList *include_list = NULL;
#else
	sq_queue_t *include_list = NULL;
#endif
	int i;

	if (svc == NULL)
		return BT_ERROR_INVALID_PARAMETER;

	if (svc->include_handles == NULL)
		return BT_ERROR_NONE;

	client_s = (bt_gatt_client_s*)svc->parent;

	for (i = 0; svc->include_handles[i]; i++) {
		bt_gatt_service_s *sec_svc = NULL;
		bt_gatt_handle_prop_t prop;
		bt_gatt_svc_prop_t sec_property;
		int ret;

		memset(&prop, 0x00, sizeof(bt_gatt_handle_prop_t));
		memset(&sec_property, 0x00, sizeof(sec_property));

		ret = bt_adapt_gatt_client_get_svc_property(client_s->remote_address,
				&prop, &sec_property);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("bt_adapt_gatt_client_get_svc_property is failed");
			continue;
		}

		sec_svc = __bt_gatt_service_create(sec_property.uuid,
						BT_GATT_SERVICE_TYPE_SECONDARY);
		if (sec_svc == NULL) {
			BT_ERR("svc is NULL");
			goto next;
		}
		sec_svc->role = BT_GATT_ROLE_CLIENT;
		sec_svc->parent = (void *)svc;
		sec_svc->is_included_service = true;
		sec_svc->path = strdup(svc->include_handles[i]);

		sec_svc->char_handles = sec_property.char_handle.handle;
		sec_property.char_handle.handle = NULL;

#ifdef GLIB_SUPPORTED
		include_list = g_slist_append(include_list, sec_svc);
#else
		include_list = calloc(1, sizeof(sq_queue_t));
		sq_addlast((sq_entry_t *)sec_svc, include_list);
#endif
next:
		bt_adapt_gatt_free_svc_property(&sec_property);
	}

	FREEV(svc->include_handles);
	svc->include_handles = NULL;

//	g_slist_free_full(svc->included_services, __bt_gatt_free_service);
	__bt_gatt_free_service(svc->included_services);

	svc->included_services = include_list;

	return BT_ERROR_NONE;
}

int _bt_gatt_client_update_characteristics(bt_gatt_h service)
{
	bt_gatt_service_s *svc = service;
	bt_gatt_client_s *client_s;
	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t char_handle;
#ifdef GLIB_SUPPORTED
	GSList *chr_list = NULL;
#else
	sq_queue_t *chr_list = NULL;
#endif
	int i;

	BT_INFO("+");
	if (svc == NULL)
		return BT_ERROR_INVALID_PARAMETER;

	if (svc->charc_handles.count == 0)
		return BT_ERROR_NONE;

	memset(&svc_handle, 0x00, sizeof(bt_gatt_handle_prop_t));
	memset(&char_handle, 0x00, sizeof(bt_gatt_handle_prop_t));

	client_s = (bt_gatt_client_s*)svc->parent;

	__bt_string_to_uuid_hex(svc->uuid, svc_handle.uuid);
	svc_handle.instance_id = svc->instance_id;

	BT_INFO("Total number of charcs [%d]", svc->charc_handles.count);

	for (i = 0; i < svc->charc_handles.count; i++) {
		bt_gatt_characteristic_s *chr = NULL;
		bt_gatt_char_prop_t char_property;
		int ret;

		memset(&char_property, 0x00, sizeof(char_property));

		__bt_string_to_uuid_hex(svc->charc_handles.uuids[i], char_handle.uuid);
		char_handle.instance_id = svc->charc_handles.inst_id[i];

		BT_INFO("Retrieve Characteristics properties");
		ret = bt_adapt_gatt_client_get_char_property(
				client_s->remote_address,
				&svc_handle,
				&char_handle,
				&char_property);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("bluetooth_gatt_get_characteristics_property is failed");
			continue;
		}

		BT_INFO("Characteristics properties are retreived, not create charc object");
		ret = bt_gatt_characteristic_create((const char *)char_property.uuid, 0,
				char_property.permission,
				(char *)char_property.val,
				(int)char_property.val_len, (bt_gatt_h *)&chr);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("bt_gatt_characteristic_create is failed");
			goto next;
		}

		if (char_property.permission & BT_GATT_PROPERTY_WRITE_WITHOUT_RESPONSE) {
			chr->write_type = BT_GATT_WRITE_TYPE_WRITE_NO_RESPONSE;
			BT_INFO("Characterics has Write No Response property");
		}
		if (char_property.permission & BT_GATT_PROPERTY_WRITE) {
			BT_INFO("Characterics has Write property");
			chr->write_type = BT_GATT_WRITE_TYPE_WRITE;
		}
		if (char_property.permission & BT_GATT_PROPERTY_READ)
			BT_INFO("Characterics has Read property");

		if (char_property.permission & BT_GATT_PROPERTY_BROADCAST)
			BT_INFO("Characterics has Broadcast property");

		if (char_property.permission & BT_GATT_PROPERTY_INDICATE)
			BT_INFO("Characterics has Indicate property");

		if (char_property.permission & BT_GATT_PROPERTY_NOTIFY)
			BT_INFO("Characterics has Notify property");

		/* Copy descriptor handles in just created charc object */
		memcpy(&chr->descriptor_handles, &char_property.char_desc_handle,
				sizeof(bt_gatt_handle_info_t));
		chr->instance_id = char_handle.instance_id;
		chr->parent = (void *)svc;
		chr->role = BT_GATT_ROLE_CLIENT;
		chr->properties = char_property.permission;

#ifdef GLIB_SUPPORTED
		chr_list = g_slist_append(chr_list, chr);
#else
		chr_list = calloc(1, sizeof(sq_queue_t));
		sq_addlast((sq_entry_t *)chr, chr_list);
#endif
next:
		bt_adapt_gatt_free_char_property(&char_property);
	}

	memset(&svc->charc_handles, 0x00, sizeof(bt_gatt_handle_info_t));
#ifdef GLIB_SUPPORTED
	g_slist_free_full(svc->characteristics,
			__bt_gatt_client_handle_destroy);
#else
	__bt_gatt_client_handle_destroy(svc->characteristics);
#endif

	svc->characteristics = chr_list;
#ifdef GLIB_SUPPORTED
	BT_INFO("Total number of characteristics whose properties are retrieved is [%d]",
			g_slist_length(svc->characteristics));
#endif

	return BT_ERROR_NONE;
}

int _bt_gatt_client_update_descriptors(bt_gatt_h characteristic)
{
	bt_gatt_characteristic_s *chr = characteristic;
#ifdef GLIB_SUPPORTED
	GSList *desc_list = NULL;
#else
	sq_queue_t *desc_list = NULL;
#endif
	int i;

	bt_gatt_service_s *svc;
	bt_gatt_client_s *client_s;
	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t char_handle;
	bt_gatt_handle_prop_t desc_handle;

	BT_INFO("+");
	if (chr == NULL)
		return BT_ERROR_INVALID_PARAMETER;
	if (chr->descriptor_handles.count == 0)
		return BT_ERROR_NONE;

	svc = (bt_gatt_service_s*)chr->parent;
	memset(&svc_handle, 0x00, sizeof(bt_gatt_handle_prop_t));
	memset(&char_handle, 0x00, sizeof(bt_gatt_handle_prop_t));
	memset(&desc_handle, 0x00, sizeof(bt_gatt_handle_prop_t));

	client_s = (bt_gatt_client_s*)svc->parent;

	__bt_string_to_uuid_hex(svc->uuid, svc_handle.uuid);
	svc_handle.instance_id = svc->instance_id;

	__bt_string_to_uuid_hex(chr->uuid, char_handle.uuid);
	char_handle.instance_id = chr->instance_id;

	for (i = 0; i < chr->descriptor_handles.count; i++) {
		bt_gatt_descriptor_s *desc = NULL;
		bt_gatt_char_desc_prop_t desc_property;
		int ret;

		memset(&desc_property, 0x00, sizeof(desc_property));


		__bt_string_to_uuid_hex(chr->descriptor_handles.uuids[i], desc_handle.uuid);
		desc_handle.instance_id = chr->descriptor_handles.inst_id[i];

		BT_INFO("Retreieve Desceriptor Properties from remote [%s]",
				client_s->remote_address);
		ret = bt_adapt_gatt_client_get_char_desc_property(
				client_s->remote_address,
				&svc_handle,
				&char_handle,
				&desc_handle,
				&desc_property);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("bluetooth_gatt_get_char_descriptor_property is failed");
			continue;
		}

		BT_INFO("Retreieved Desceriptor Properties, now create Descriptor object");
		ret = bt_gatt_descriptor_create(desc_property.uuid, 0,
				(char*)desc_property.val,
				(int)desc_property.val_len,
				(bt_gatt_h *)&desc);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("bt_gatt_descriptor_create is failed");
			goto next;
		}


		desc->instance_id = desc_handle.instance_id;
		desc->parent = (void *)characteristic;
		desc->role = BT_GATT_ROLE_CLIENT;
#ifdef GLIB_SUPPORTED
		desc_list = g_slist_append(desc_list, desc);
#else
		desc_list = calloc(1, sizeof(sq_queue_t));
		sq_addlast((sq_entry_t *)desc, desc_list);
#endif
next:
		bt_adapt_gatt_free_desc_property(&desc_property);
	}


	memset(&chr->descriptor_handles, 0x00, sizeof(bt_gatt_handle_info_t));

#ifdef GLIB_SUPPORTED
	g_slist_free_full(chr->descriptors,
			__bt_gatt_client_handle_destroy);
#else
	__bt_gatt_client_handle_destroy(chr->descriptors);
#endif

	chr->descriptors = desc_list;

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_STOP */
int bt_gatt_connect(const char *address, bool auto_connect)
{
	int ret;

	bt_gatt_client_s *client_s;

	BT_INFO("Address [%s] Auto Connect [%d]",
				address, auto_connect);

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(address);

	/* Find any client attached with remote address or not */
	client_s = _bt_gatt_get_client(address);
	if (client_s) {
		BT_INFO("GATT client instance is already present for the rmeote addr [%s] client_id[%d]",
				address, client_s->client_id);
		ret = bt_adapt_gatt_connect(address, auto_connect, client_s->client_id);
	} else {
		BT_INFO("GATT client instance is NOT present for the remote addr [%s]",
				address);

		ret = bt_adapt_gatt_connect(address,	auto_connect, 0);
	}

	if (ret != BT_ERROR_NONE)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}

int bt_gatt_disconnect(const char *address)
{
	int ret;
	bt_gatt_client_s *client_s;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(address);

	/* Find any client attached with remote address or not */
	client_s = _bt_gatt_get_client(address);
	if (client_s) {
		BT_INFO("GATT client instance is already present for the remote addr [%s] client interface [%d]",
				address, client_s->client_id);
		ret = bt_adapt_gatt_disconnect(address, client_s->client_id);
	} else {
		BT_INFO("GATT client instance is NOT present for the remote addr [%s]",
				address);
		ret = bt_adapt_gatt_disconnect(address, 0);
	}

	if (ret != BT_ERROR_NONE)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}

int bt_gatt_set_connection_state_changed_cb(bt_gatt_connection_state_changed_cb callback,
							void *user_data)
{
	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);
	_bt_set_cb(BT_EVENT_GATT_CONNECTION_STATUS, callback, user_data);

	return BT_ERROR_NONE;
}

int bt_gatt_unset_connection_state_changed_cb(void)
{
	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	_bt_unset_cb(BT_EVENT_GATT_CONNECTION_STATUS);
	return BT_ERROR_NONE;
}

#ifdef GLIB_SUPPORTED
static void __bt_gatt_free_descriptor(bt_gatt_h gatt_handle)
{
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	/* Remove descriptor from List of GATT handles */
	if (gatt_handle_list)
		gatt_handle_list = g_slist_remove(gatt_handle_list, gatt_handle);

	g_free(desc->path);
	g_free(desc->uuid);
	g_free(desc->value);

	if (!desc->read_cb && !desc->write_cb)
		g_free(desc);
	else
		desc->parent = NULL;
}
#else
static void __bt_gatt_free_descriptor(sq_queue_t *gatt_handle)
{
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)sq_peek(gatt_handle);

	/* Remove descriptor from List of GATT handles */
	sq_rem((sq_entry_t *)gatt_handle, &gatt_handle_list);

	FREE(desc->path);
	FREE(desc->uuid);
	FREE(desc->value);

	if (!desc->read_cb && !desc->write_cb) {
		FREE(desc);
	} else {
		desc->parent = NULL;
	}
}
#endif

#ifdef GLIB_SUPPORTED
static void __bt_gatt_free_characteristic(bt_gatt_h gatt_handle)
{
	int ret;
	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t chr_handle;
	bt_gatt_service_s *service_s;
	bt_gatt_client_s  *client_s;

	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s*)gatt_handle;

	BT_INFO("+");

	/* Remove characteristic from List of GATT handles */
	if (gatt_handle_list)
		gatt_handle_list = g_slist_remove(gatt_handle_list, gatt_handle);

	service_s = (bt_gatt_service_s*)chr->parent;
	if (!service_s) {
		BT_ERR("service_s is null");
		goto fail;
	}
	client_s = (bt_gatt_client_s*)service_s->parent;
	if (!client_s) {
		BT_ERR("client_s is null");
		goto fail;
	}
	/* Create Service and Charc handles */
	__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
	svc_handle.instance_id = service_s->instance_id;

	BT_INFO("Service UUID [%s]", service_s->uuid);
	BT_INFO("Charc Instance ID [%d]", service_s->instance_id);

	BT_INFO("Charc UUID [%s]", chr->uuid);
	BT_INFO("Charc Instance ID [%d]", chr->instance_id);

	BT_INFO("Client ID [%d]", client_s->client_id);

	__bt_string_to_uuid_hex(chr->uuid, chr_handle.uuid);
	chr_handle.instance_id = chr->instance_id;

	if (chr->role == BT_GATT_ROLE_CLIENT && chr->value_changed_cb &&
		chr->properties &
		(BT_GATT_PROPERTY_NOTIFY | BT_GATT_PROPERTY_INDICATE)) {
		BT_DBG("Unwatch characteristic");
		ret = bt_adapt_gatt_client_unwatch_char(client_s->remote_address, &svc_handle, &chr_handle, chr->instance_id);
		if (ret != BT_ERROR_NONE)
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	}

fail:
	g_slist_free_full(chr->descriptors, __bt_gatt_free_descriptor);

	g_free(chr->path);
	g_free(chr->uuid);
	g_free(chr->value);

	if (!chr->read_cb && !chr->write_cb)
		g_free(chr);
	else
		chr->parent = NULL;
}
#else
static void __bt_gatt_free_characteristic(sq_queue_t *gatt_handle)
{
	int ret;
	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t chr_handle;
	bt_gatt_service_s *service_s;
	bt_gatt_client_s *client_s;

	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)sq_peek(gatt_handle);

	BT_INFO("+");

	/* Remove characteristic from List of GATT handles */
	sq_rem((sq_entry_t *)gatt_handle, &gatt_handle_list);

	service_s = (bt_gatt_service_s *)chr->parent;
	if (!service_s) {
		BT_ERR("service_s is null");
		goto fail;
	}
	client_s = (bt_gatt_client_s *)service_s->parent;
	if (!client_s) {
		BT_ERR("client_s is null");
		goto fail;
	}
	/* Create Service and Charc handles */
	__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
	svc_handle.instance_id = service_s->instance_id;

	BT_INFO("Service UUID [%s]", service_s->uuid);
	BT_INFO("Charc Instance ID [%d]", service_s->instance_id);

	BT_INFO("Charc UUID [%s]", chr->uuid);
	BT_INFO("Charc Instance ID [%d]", chr->instance_id);

	BT_INFO("Client ID [%d]", client_s->client_id);

	__bt_string_to_uuid_hex(chr->uuid, chr_handle.uuid);
	chr_handle.instance_id = chr->instance_id;

	if (chr->role == BT_GATT_ROLE_CLIENT && chr->value_changed_cb &&
		chr->properties &
		(BT_GATT_PROPERTY_NOTIFY | BT_GATT_PROPERTY_INDICATE)) {
		BT_DBG("Unwatch characteristic");
		ret = bt_adapt_gatt_client_unwatch_char(client_s->remote_address, &svc_handle, &chr_handle, chr->instance_id);
		if (ret != BT_ERROR_NONE)
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	}

fail:
	__bt_gatt_free_descriptor(chr->descriptors);

	FREE(chr->path);
	FREE(chr->uuid);
	FREE(chr->value);

	if (!chr->read_cb && !chr->write_cb) {
		FREE(chr);
	} else {
		chr->parent = NULL;
	}
}
#endif

#ifdef GLIB_SUPPORTED
static void __bt_gatt_free_service(bt_gatt_h gatt_handle)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_service_s *svc = (bt_gatt_service_s *)gatt_handle;

	g_slist_free_full(svc->included_services, __bt_gatt_free_service);
	g_slist_free_full(svc->characteristics, __bt_gatt_free_characteristic);

	/* Remove service from List of GATT handles */
	if (gatt_handle_list)
		gatt_handle_list = g_slist_remove(gatt_handle_list, gatt_handle);

	if (svc->role == BT_GATT_ROLE_SERVER) {
		if (is_gatt_server_initialized) {
			BT_INFO("GATT Server Delete Service: service handle [%d] Inst ID [%d]", svc->handle, instance_id);
			ret = bt_adapt_gatt_server_delete_service(svc->handle, instance_id);
			if (ret != BT_ERROR_NONE) {
				BT_ERR("%s(0x%08x)",
						_bt_convert_error_to_string(ret), ret);
			}
		}
	}

	g_strfreev(svc->include_handles);
	g_strfreev(svc->char_handles);

	g_free(svc->path);
	g_free(svc->uuid);
	g_free(svc);
}
#else
static void __bt_gatt_free_service(sq_queue_t *gatt_handle)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_service_s *svc = (bt_gatt_service_s *)sq_peek(gatt_handle);

	if (svc->included_services)
		__bt_gatt_free_service(svc->included_services);
	if (svc->characteristics)
	__bt_gatt_free_characteristic(svc->characteristics);

	/* Remove service from List of GATT handles */
	sq_rem((sq_entry_t *)gatt_handle, &gatt_handle_list);

	if (svc->role == BT_GATT_ROLE_SERVER) {
		if (is_gatt_server_initialized) {
			BT_INFO("GATT Server Delete Service: service handle [%d] Inst ID [%d]", svc->handle, instance_id);
			ret = bt_adapt_gatt_server_delete_service(svc->handle, instance_id);
			if (ret != BT_ERROR_NONE) {
				BT_ERR("%s(0x%08x)",
						_bt_convert_error_to_string(ret), ret);
			}
		}
	}

	FREEV(svc->include_handles);
	FREEV(svc->char_handles);

	FREE(svc->path);
	FREE(svc->uuid);
	FREE(svc);
}
#endif

static void __bt_gatt_destroy_descriptor(bt_gatt_h gatt_handle)
{
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;
	bt_gatt_characteristic_s *parent_chr = (bt_gatt_characteristic_s *)desc->parent;

#ifdef GLIB_SUPPORTED
	if (parent_chr)
		parent_chr->descriptors = g_slist_remove(parent_chr->descriptors,
									desc);
#else
	sq_rem((sq_entry_t *)desc, parent_chr->descriptors);
#endif

	__bt_gatt_free_descriptor(gatt_handle);
}

static void __bt_gatt_destroy_characteristic(bt_gatt_h gatt_handle)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_service_s *parent_svc = (bt_gatt_service_s *)chr->parent;

#ifdef GLIB_SUPPORTED
	if (parent_svc)
		parent_svc->characteristics = g_slist_remove(parent_svc->characteristics,
									chr);
#else
	sq_rem((sq_entry_t *)chr, parent_svc->characteristics);
#endif

	__bt_gatt_free_characteristic(gatt_handle);
}

static void __bt_gatt_destroy_service(bt_gatt_h gatt_handle)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)gatt_handle;
	bt_gatt_server_s *server = NULL;
	bt_gatt_client_s *client = NULL;
	bt_gatt_service_s *parent_svc = NULL;

	if (svc->is_included_service == FALSE) {
		if (svc->role == BT_GATT_ROLE_SERVER) {
			server = (bt_gatt_server_s *)svc->parent;
#ifdef GLIB_SUPPORTED
			if (server)
				server->services = g_slist_remove(server->services,
									svc);
#else
			sq_rem((sq_entry_t *)svc, server->services);
#endif
		} else {
			client = (bt_gatt_client_s *)svc->parent;
#ifdef GLIB_SUPPORTED
			if (client)
				client->services = g_slist_remove(client->services,
									svc);
#else
			sq_rem((sq_entry_t *)svc, client->services);
#endif
		}
	} else {
		parent_svc = (bt_gatt_service_s *)svc->parent;
#ifdef GLIB_SUPPORTED
		if (parent_svc)
			parent_svc->included_services = g_slist_remove(parent_svc->included_services,
									svc);
#else
			sq_rem((sq_entry_t *)svc, parent_svc->included_services);
#endif
	}

	__bt_gatt_free_service(gatt_handle);
}

/* LCOV_EXCL_START */
static int __convert_int_to_signed_bits(int i, int size)
{
	if (i < 0)
		i = (1 << (size - 1)) + (i & ((1 << (size - 1)) - 1));

	return i;
}

static int __convert_unsigned_to_signed(int value, int size)
{
	if ((value & (1 << (size-1))) != 0) {
		value = -1 * ((1 << (size-1))
				- (value & ((1 << (size-1)) - 1)));
	}
	return value;
}

static int __convert_unsigned_byte_to_int(char b)
{
	return b & 0xFF;
}

static int __convert_unsigned_bytes_to_int16(char b0, char b1)
{
	return (__convert_unsigned_byte_to_int(b0)
		+ (__convert_unsigned_byte_to_int(b1) << 8));
}

static int __convert_unsigned_bytes_to_int32(char b0, char b1, char b2, char b3)
{
	return (__convert_unsigned_byte_to_int(b0)
		+ (__convert_unsigned_byte_to_int(b1) << 8))
		+ (__convert_unsigned_byte_to_int(b2) << 16)
		+ (__convert_unsigned_byte_to_int(b3) << 24);
}

static double power(int x, int n)
{
	/* pow() cannot not referenced. */
	if (n == 0)
		return 1;
	else
		return x*power(x, n-1);
}

static float __convert_bytes_to_short_float(char b0, char b1)
{
	int mantissa;
	int exponent;
	float tmp;
	mantissa = __convert_unsigned_to_signed(__convert_unsigned_byte_to_int(b0)
		+ ((__convert_unsigned_byte_to_int(b1) & 0x0F) << 8), 12);
	exponent = __convert_unsigned_to_signed(__convert_unsigned_byte_to_int(b1) >> 4, 4);
	tmp = power(10, exponent);

	return (float)(mantissa * tmp);
}

float __convert_bytes_to_float(char b0, char b1, char b2, char b3)
{
	int mantissa;
	float exponent;
	mantissa = __convert_unsigned_to_signed(__convert_unsigned_byte_to_int(b0)
			+ (__convert_unsigned_byte_to_int(b1) << 8)
			+ (__convert_unsigned_byte_to_int(b2) << 16), 24);
	exponent = power(10, b3);

	return (float)(mantissa * exponent);
}

static int __get_data_type_float_size(bt_data_type_float_e type)
{
	switch (type) {
	case BT_DATA_TYPE_SFLOAT:
		return 2;
	case BT_DATA_TYPE_FLOAT:
		return 4;
	default:
		return 0;
	}
}

static int __get_data_type_int_size(bt_data_type_int_e format)
{
	switch (format) {
	case BT_DATA_TYPE_SINT8:
	case BT_DATA_TYPE_UINT8:
		return 1;
	case BT_DATA_TYPE_SINT16:
	case BT_DATA_TYPE_UINT16:
		return 2;
	case BT_DATA_TYPE_SINT32:
	case BT_DATA_TYPE_UINT32:
		return 4;
	default:
		return 0;
	}
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
int bt_gatt_destroy(bt_gatt_h gatt_handle)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);

	if (handle->type == BT_GATT_TYPE_SERVICE)
		__bt_gatt_destroy_service(gatt_handle);
	else if (handle->type == BT_GATT_TYPE_CHARACTERISTIC)
		__bt_gatt_destroy_characteristic(gatt_handle);
	else if (handle->type == BT_GATT_TYPE_DESCRIPTOR)
		__bt_gatt_destroy_descriptor(gatt_handle);
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

static int __get_write_prop(bt_gatt_write_type_e type, bt_gatt_property_e *prop)
{
	if (!prop)
		return BT_ERROR_INVALID_PARAMETER;

	switch (type) {
	case BT_GATT_WRITE_TYPE_WRITE:
		*prop = BT_GATT_PROPERTY_WRITE;
		break;
	case BT_GATT_WRITE_TYPE_WRITE_NO_RESPONSE:
		*prop = BT_GATT_PROPERTY_WRITE_WITHOUT_RESPONSE;
		break;
/* After ACR for BT_GATT_WRITE_TYPE_SIGNED_WRITE, will enable this code */
#if 0
	case BT_GATT_WRITE_TYPE_SIGNED_WRITE:
		*prop = BT_GATT_PROPERTY_AUTHENTICATED_SIGNED_WRITES;
		break;
#endif
	default:
		BT_ERR("Unknow write type : %d", type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

int bt_gatt_service_destroy(bt_gatt_h gatt_handle)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_INFO("Destroy GATT Service");

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_SERVICE)
		__bt_gatt_destroy_service(gatt_handle);
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

int bt_gatt_characteristic_destroy(bt_gatt_h gatt_handle)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC)
		__bt_gatt_destroy_characteristic(gatt_handle);
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

int bt_gatt_descriptor_destroy(bt_gatt_h gatt_handle)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);

	if (handle->type == BT_GATT_TYPE_DESCRIPTOR)
		__bt_gatt_destroy_descriptor(gatt_handle);
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

int bt_gatt_get_value(bt_gatt_h gatt_handle, char **value, int *value_length)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(value); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(value_length);

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		*value_length = chr->value_length;
		if (chr->value_length > 0) {
//			*value = g_memdup(chr->value, chr->value_length);
			*value = calloc(1, chr->value_length);
			memcpy(*value, chr->value, chr->value_length);
		}
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		*value_length = desc->value_length;
		if (desc->value_length > 0) {
//			*value = g_memdup(desc->value, desc->value_length);
			*value = calloc(1, desc->value_length);
			memcpy(*value, desc->value, desc->value_length);
		}
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_get_int_value(bt_gatt_h gatt_handle, bt_data_type_int_e type,
			int offset, int *value)
{
	char *val;
	int val_len;
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(value); /* LCOV_EXCL_START */

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		val = chr->value;
		val_len = chr->value_length;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		val = desc->value;
		val_len = desc->value_length;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if ((offset + __get_data_type_int_size(type)) > val_len) {
		BT_ERR("Offset exceed the memory");
		return BT_ERROR_INVALID_PARAMETER;
	}

	switch (type) {
	case BT_DATA_TYPE_UINT8:
		*value = __convert_unsigned_byte_to_int(val[offset]);
		break;
	case BT_DATA_TYPE_UINT16:
		*value = __convert_unsigned_bytes_to_int16(val[offset],
							val[offset+1]);
		break;

	case BT_DATA_TYPE_UINT32:
		*value = __convert_unsigned_bytes_to_int32(val[offset],
			val[offset+1], val[offset+2], val[offset+3]);
		break;
	case BT_DATA_TYPE_SINT8:
		*value = __convert_unsigned_to_signed(__convert_unsigned_byte_to_int(val[offset]), 8);
		break;

	case BT_DATA_TYPE_SINT16:
		*value = __convert_unsigned_to_signed(__convert_unsigned_bytes_to_int16(val[offset],
						   val[offset+1]), 16);
		break;

	case BT_DATA_TYPE_SINT32:
		*value = __convert_unsigned_to_signed(__convert_unsigned_bytes_to_int32(val[offset],
			val[offset+1], val[offset+2], val[offset+3]), 32);
		break;
	default:
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_get_float_value(bt_gatt_h gatt_handle, bt_data_type_float_e type,
						int offset, float *value)
{
	char *val;
	int val_len;
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(value); /* LCOV_EXCL_START */

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		val = chr->value;
		val_len = chr->value_length;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		val = desc->value;
		val_len = desc->value_length;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if ((offset + __get_data_type_float_size(type)) > val_len) {
		BT_ERR("Offset exceed the memory");
		return BT_ERROR_INVALID_PARAMETER;
	}

	switch (type) {
	case BT_DATA_TYPE_SFLOAT:
		*value = __convert_bytes_to_short_float(val[offset],
			val[offset+1]);
		break;

	case BT_DATA_TYPE_FLOAT:
		*value = __convert_bytes_to_float(val[offset],
			val[offset+1], val[offset+2], val[offset+3]);
		break;
	default:
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_set_value(bt_gatt_h gatt_handle, const char *value,
						int value_length)
{
	int ret;
	int k;
	char **val = NULL;
	int *val_len = NULL;
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_INFO("+");
	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(value); /* LCOV_EXCL_START */

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		BT_INFO("Request is for Char write value");
		val = &chr->value;
		val_len = &chr->value_length;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		BT_INFO("Request is for Descriptor write value");
		val = &desc->value;
		val_len = &desc->value_length;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (handle->role == BT_GATT_ROLE_SERVER && handle->handle) {
		/* DEBUG */
		for (k = 0; k < value_length; k++)
			BT_DBG("New Value[%d] = [0x%x]", k, value[k]);

		ret = bt_adapt_gatt_server_update_char(instance_id, handle->handle, value, value_length);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
			return ret;
		}
	}

	FREE(*val);
	*val = NULL;
	if (value_length > 0) {
		//*val = g_memdup(value, value_length);
		*val = calloc(1, value_length);
		*val = memcpy(*val, value, value_length);
	}
	*val_len = value_length;

	BT_INFO("Value is set");
	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_set_int_value(bt_gatt_h gatt_handle, bt_data_type_int_e type,
						int value, int offset)
{
	int ret;
	char **val = NULL;
	int *val_len = NULL;
	char *tmp;
	int fmt_size;
	int idx = offset;
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) { /* LCOV_EXCL_START */
		val = &chr->value;
		val_len = &chr->value_length;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		val = &desc->value;
		val_len = &desc->value_length;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (*val_len < offset) {
		BT_ERR("Offset is invalid");
		return BT_ERROR_INVALID_PARAMETER;
	}

	fmt_size = __get_data_type_int_size(type);
	if (*val == NULL) {
		*val = calloc(1, fmt_size);
		if (*val == NULL)
			return BT_ERROR_OUT_OF_MEMORY;
		*val_len = fmt_size;
	} else if (*val_len == offset) { /* Added */
		tmp = calloc(1, *val_len + fmt_size);
		/* Fix : NULL_RETURNS */
		if (!tmp) {
			FREE(*val);
			return BT_ERROR_OUT_OF_MEMORY;
		}
		memcpy(tmp, *val, *val_len);
		FREE(*val);
		*val = tmp;
		*val_len += fmt_size;
	} else if (*val_len < offset + fmt_size) {/* Overlapped */
		tmp = calloc(1, offset + fmt_size);
		/* Fix : NULL_RETURNS */
		if (!tmp) {
			FREE(*val);
			return BT_ERROR_OUT_OF_MEMORY;
		}
		memcpy(tmp, *val, *val_len);
		FREE(*val);
		*val = tmp;
		*val_len = offset + fmt_size;
	}

	switch (type) {
	case BT_DATA_TYPE_SINT8:
		value = __convert_int_to_signed_bits(value, 8);
		(*val)[idx] = (char)(value & 0xFF);
		break;
	case BT_DATA_TYPE_UINT8:
		(*val)[idx] = (char)(value & 0xFF);
		break;

	case BT_DATA_TYPE_SINT16:
		value = __convert_int_to_signed_bits(value, 16);
		 (*val)[idx++] = (char)(value & 0xFF);
		 (*val)[idx] = (char)((value >> 8) & 0xFF);
		break;
	case BT_DATA_TYPE_UINT16:
		 (*val)[idx++] = (char)(value & 0xFF);
		 (*val)[idx] = (char)((value >> 8) & 0xFF);
		break;

	case BT_DATA_TYPE_SINT32:
		value = __convert_int_to_signed_bits(value, 32);
		(*val)[idx++] = (char)(value & 0xFF);
		(*val)[idx++] = (char)((value >> 8) & 0xFF);
		(*val)[idx++] = (char)((value >> 16) & 0xFF);
		(*val)[idx] = (char)((value >> 24) & 0xFF);
		break;
	case BT_DATA_TYPE_UINT32:
		(*val)[idx++] = (char)(value & 0xFF);
		(*val)[idx++] = (char)((value >> 8) & 0xFF);
		(*val)[idx++] = (char)((value >> 16) & 0xFF);
		(*val)[idx] = (char)((value >> 24) & 0xFF);
		break;

	default:
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (handle->role == BT_GATT_ROLE_SERVER && handle->path) {
		ret = bt_adapt_gatt_server_update_char(instance_id, handle->handle, (const char *)*val, *val_len);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
			return ret;
		}
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_set_float_value(bt_gatt_h gatt_handle, bt_data_type_float_e type,
			int mantissa,  int exponent, int offset)
{
	int ret;
	char **val = NULL;
	int *val_len = NULL;
	int mant;
	int exp;
	char *tmp;
	int fmt_size;
	int idx = offset;
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) { /* LCOV_EXCL_START */
		val = &chr->value;
		val_len = &chr->value_length;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		val = &desc->value;
		val_len = &desc->value_length;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (*val_len < offset) {
		BT_ERR("Offset is invalid");
		return BT_ERROR_INVALID_PARAMETER;
	}

	fmt_size = __get_data_type_float_size(type);
	if (fmt_size == 0) {
		BT_ERR("Format is invalid");
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (*val == NULL) {
		*val = calloc(1, fmt_size);
		/* Fix : NULL_RETURNS */
		if (*val == NULL)
			return BT_ERROR_OUT_OF_MEMORY;
		*val_len = fmt_size;
	} else if (*val_len == offset) {/* Added */
		tmp = calloc(1, *val_len + fmt_size);
		/* Fix : NULL_RETURNS */
		if (tmp == NULL) {
			FREE(*val);
			return BT_ERROR_OUT_OF_MEMORY;
		}
		memcpy(tmp, *val, *val_len);
		FREE(*val);
		*val = tmp;
		*val_len += fmt_size;
	} else if (*val_len < offset + fmt_size) {/* Overlapped */
		tmp = calloc(1, offset + fmt_size);
		/* Fix : NULL_RETURNS */
		if (tmp == NULL) {
			FREE(*val);
			return BT_ERROR_OUT_OF_MEMORY;
		}
		memcpy(tmp, *val, *val_len);
		FREE(*val);
		*val = tmp;
		*val_len = offset + fmt_size;
	}

	switch (type) {
	case BT_DATA_TYPE_FLOAT:
		mant = __convert_int_to_signed_bits(mantissa, 24);
		exp = __convert_int_to_signed_bits(exponent, 8);
		(*val)[idx++] = (char)(mant & 0xFF);
		(*val)[idx++] = (char)((mant >> 8) & 0xFF);
		(*val)[idx++] = (char)((mant >> 16) & 0xFF);
		(*val)[idx] = (char)(exp & 0xFF);
		break;

	case BT_DATA_TYPE_SFLOAT:
		mant = __convert_int_to_signed_bits(mantissa, 12);
		exp = __convert_int_to_signed_bits(exponent, 4);
		(*val)[idx++] = (char)(mant & 0xFF);
		(*val)[idx] = (char)((mant >> 8) & 0x0F);
		(*val)[idx] += (char)((exp & 0x0F) << 4);
		break;

	default:
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (handle->role == BT_GATT_ROLE_SERVER && handle->path) {
		ret = bt_adapt_gatt_server_update_char(instance_id, handle->handle, (const char *)val, *val_len);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
			return ret;
		}
	}

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

/* LCOV_EXCL_START */
int bt_gatt_characteristic_get_permissions(bt_gatt_h gatt_handle, int *permissions)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(permissions);

	 BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC)
		*permissions = chr->permissions;
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

int bt_gatt_descriptor_get_permissions(bt_gatt_h gatt_handle, int *permissions)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(permissions);

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_DESCRIPTOR)
		*permissions = desc->permissions;
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

int bt_gatt_set_permissions(bt_gatt_h gatt_handle, int permissions)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC)
		chr->permissions = permissions;
	else if (handle->type == BT_GATT_TYPE_DESCRIPTOR)
		desc->permissions = permissions;
	else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

int bt_gatt_get_uuid(bt_gatt_h gatt_handle, char **uuid)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(uuid); /* LCOV_EXCL_LINE */

	*uuid = strdup(handle->uuid); /* LCOV_EXCL_LINE */

	return BT_ERROR_NONE; /* LCOV_EXCL_LINE */
}

int bt_gatt_get_type(bt_gatt_h gatt_handle, bt_gatt_type_e *gatt_type)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(gatt_type); /* LCOV_EXCL_LINE */

	*gatt_type = handle->type; /* LCOV_EXCL_LINE */

	return BT_ERROR_NONE; /* LCOV_EXCL_LINE */
}

static bt_gatt_service_s* __bt_gatt_service_create(const char *uuid,
				bt_gatt_service_type_e type)
{
	bt_gatt_service_s *svc;

	if (uuid == NULL)
		return NULL;

	svc = (bt_gatt_service_s *)calloc(1, sizeof(bt_gatt_service_s));
	if (svc == NULL)
		return NULL;

	svc->type = BT_GATT_TYPE_SERVICE;
	svc->uuid = _bt_convert_uuid_to_uuid128(uuid);

	if (svc->uuid == NULL) {
		FREE(svc);
		return NULL;
	}
	svc->service_type = type;

	return svc;
}

/* LCOV_EXCL_START */
int bt_gatt_service_create(const char *uuid, bt_gatt_service_type_e type,
			   bt_gatt_h *service)
{
	bt_gatt_service_s *svc;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(uuid);
	BT_CHECK_INPUT_PARAMETER(service);

	if (is_gatt_server_started) {
		BT_ERR("Already Server started");
		return BT_ERROR_OPERATION_FAILED;
	}

	svc = __bt_gatt_service_create(uuid, type);
	if (svc == NULL)
		return BT_ERROR_OUT_OF_MEMORY;
	svc->role = BT_GATT_ROLE_SERVER;
	svc->is_included_service = false;

	BT_INFO("Current handle count of service [%d]", svc->numhandles);
	svc->numhandles = 1; /* Initalize numhandles to 1 */

	*service = (bt_gatt_h)svc;

	/* Add service to list of GATT handles */
#ifdef GLIB_SUPPORTED
	gatt_handle_list = g_slist_append(gatt_handle_list, (gpointer)svc);
#else
	sq_addlast((sq_entry_t *)svc, &gatt_handle_list);
#endif

	return BT_ERROR_NONE;
}

int bt_gatt_service_add_characteristic(bt_gatt_h service,
					bt_gatt_h characteristic)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(characteristic);

	BT_VALIDATE_GATT_HANDLE(service);
	BT_VALIDATE_GATT_HANDLE(characteristic);

	if (chr->parent) {
		BT_ERR("This characteristic is already added.");
		return BT_ERROR_INVALID_PARAMETER;
	}

#ifdef GLIB_SUPPORTED
	svc->characteristics = g_slist_append(svc->characteristics, chr);
#else
	svc->characteristics = calloc(1, sizeof(sq_queue_t));
	sq_addlast((sq_entry_t *)chr, svc->characteristics);
#endif
	chr->parent = (void *)service;
	return BT_ERROR_NONE;
}

int bt_gatt_service_add_included_service(bt_gatt_h service,
					bt_gatt_h included_service)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
	bt_gatt_service_s *included_svc = (bt_gatt_service_s *)included_service;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(included_service);

	BT_VALIDATE_GATT_HANDLE(service);
	BT_VALIDATE_GATT_HANDLE(included_service);

	if (included_svc->parent) {
		BT_ERR("This service is already added.");
		return BT_ERROR_INVALID_PARAMETER;
	}

#ifdef GLIB_SUPPORTED
	svc->included_services = g_slist_append(svc->included_services,
							included_svc);
#else
	svc->included_services = calloc(1, sizeof(sq_queue_t));
	sq_addlast((sq_entry_t *)included_svc, svc->included_services);
#endif
	included_svc->parent = (void *)service;

	if (svc) {
		BT_INFO("Current handle count of service [%d]", svc->numhandles);
		svc->numhandles = 1; /* Initalize numhandles to 1 */
	}

	return BT_ERROR_NONE;
}

int bt_gatt_service_get_server(bt_gatt_h service, bt_gatt_server_h *server)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(server);

	if (svc->is_included_service) {
		BT_ERR("This is included service of %p", svc->parent);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (svc->role != BT_GATT_ROLE_SERVER) {
		BT_ERR("This is not server's service");
		return BT_ERROR_INVALID_PARAMETER;
	}

	*server = (bt_gatt_server_h)svc->parent;

	return BT_ERROR_NONE;
}
/* LCOV_EXCL_STOP */
int bt_gatt_service_get_client(bt_gatt_h service, bt_gatt_client_h *client)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(client); /* LCOV_EXCL_START */

	if (svc->is_included_service) {
		BT_ERR("This is included service of %p", svc->parent);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (svc->role != BT_GATT_ROLE_CLIENT) {
		BT_ERR("This is not client's service");
		return BT_ERROR_INVALID_PARAMETER;
	}

	*client = (bt_gatt_client_h)svc->parent;

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_service_get_characteristic(bt_gatt_h service, const char *uuid,
					bt_gatt_h *characteristic)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
	bt_gatt_h gatt_handle = NULL;
	int ret;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(uuid); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(characteristic);

	BT_INFO("Get Characteristic rom service, charc uuid [%s], total charcs of this service [%d]",
				uuid, svc->charc_handles.count);

	if (svc->charc_handles.count > 0) {
		ret = _bt_gatt_client_update_characteristics(svc);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("_bt_gatt_client_update_characteristics failed");
			return ret;
		}
	}

	ret = __get_gatt_handle_by_uuid(svc->characteristics, uuid, &gatt_handle);
	if (ret == BT_ERROR_NONE && gatt_handle != NULL) {
		*characteristic = gatt_handle;
		return BT_ERROR_NONE;
	}

	BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
					ret); /* LCOV_EXCL_STOP */
	return ret;
}

int bt_gatt_service_foreach_characteristics(bt_gatt_h service,
				bt_gatt_foreach_cb callback, void *user_data)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
	bt_gatt_characteristic_s *chr = NULL;
#ifdef GLIB_SUPPORTED
	GSList *l = NULL;
#endif
	int total = 0;
	int index = 1;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(callback); /* LCOV_EXCL_START */

	BT_INFO("Total num of characteristics [%d]", svc->charc_handles.count);
	if (svc->charc_handles.count > 0) {
		int ret;

		ret = _bt_gatt_client_update_characteristics(svc);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("_bt_gatt_client_update_characteristics is failed");
			return ret;
		}
	}

#ifdef GLIB_SUPPORTED
	total = g_slist_length(svc->characteristics);
	BT_INFO("Total charcs [%d]", total);

	for (l = svc->characteristics; l; l = g_slist_next(l)) {
		chr = l->data;
		if (!callback(total, index++, (bt_gatt_h)chr, user_data))
			break;
	}
#else
	total = __bt_sq_queue_length(svc->characteristics);
	BT_INFO("Total charcs [%d]", total);

	chr = (bt_gatt_characteristic_s *)sq_peek(svc->characteristics);
	while (chr) {
		if (!callback(total, index++, (bt_gatt_h)chr, user_data))
			break;
		chr = (bt_gatt_characteristic_s *)sq_next(chr);
	}
#endif

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_service_get_included_service(bt_gatt_h service, const char *uuid,
						bt_gatt_h *included_service)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
	bt_gatt_h gatt_handle = NULL;
	int ret;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(uuid); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(included_service);

	if (svc->include_handles) {
		ret = _bt_gatt_client_update_include_services(svc);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("_bt_gatt_client_update_include_services failed");
			return ret;
		}
	}

	ret = __get_gatt_handle_by_uuid(svc->included_services, uuid, &gatt_handle);
	if (ret == BT_ERROR_NONE && gatt_handle != NULL) {
		*included_service = gatt_handle;
		return BT_ERROR_NONE;
	}

	BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret),
					ret); /* LCOV_EXCL_STOP */
	return ret;
}

int bt_gatt_service_foreach_included_services(bt_gatt_h service,
				bt_gatt_foreach_cb callback, void *user_data)
{
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
	bt_gatt_service_s *included_svc = NULL;
#ifdef GLIB_SUPPORTED
	GSList *l = NULL;
#endif
	int total = 0;
	int index = 1;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(service);
	BT_CHECK_INPUT_PARAMETER(callback); /* LCOV_EXCL_LINE */

	if (svc->include_handles) {
		int ret;

		ret = _bt_gatt_client_update_include_services(svc);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("_bt_gatt_client_update_include_services failed");
			return ret;
		}
	}

#ifdef GLIB_SUPPORTED
	total = g_slist_length(svc->included_services); /* LCOV_EXCL_LINE */

	for (l = svc->included_services; l; l = g_slist_next(l)) {
		included_svc = l->data; /* LCOV_EXCL_LINE */
		if (!callback(total, index++, (bt_gatt_h)included_svc,
					user_data)) /* LCOV_EXCL_LINE */
			break;
	}
#else
	total = __bt_sq_queue_length(svc->included_services); /* LCOV_EXCL_LINE */

	included_svc = (bt_gatt_service_s *)sq_peek(svc->included_services);
	while (included_svc) {
		if (!callback(total, index++, (bt_gatt_h)included_svc, user_data))
			break;
		included_svc = (bt_gatt_service_s *)sq_next(included_svc);
	}
#endif

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_gatt_characteristic_create(const char *uuid, int permissions,
				int properties, const char *value,
				int value_length, bt_gatt_h *characteristic)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_characteristic_s *chr = NULL;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(uuid);
	BT_CHECK_INPUT_PARAMETER(characteristic);
	if (value_length > 0)
		BT_CHECK_INPUT_PARAMETER(value);


	chr = calloc(1, sizeof(*chr));

	memset(chr, 0x00, sizeof(*chr));

	chr->type = BT_GATT_TYPE_CHARACTERISTIC;
	chr->role = BT_GATT_ROLE_SERVER;
	BT_INFO("Characteristic UUID : [%s] len [%zd] permissions [%d] properties [%d]",
			uuid, strlen(uuid), permissions, properties);

	chr->uuid = _bt_convert_uuid_to_uuid128(uuid);

	if (chr->uuid == NULL) {
		ret = BT_ERROR_OUT_OF_MEMORY;
		goto fail;
	}
	chr->permissions = permissions;
	chr->properties = properties;
	chr->write_type = BT_GATT_WRITE_TYPE_WRITE;
	chr->value_length = value_length;
	if (value_length > 0) {
		//chr->value = g_memdup(value, value_length);
		chr->value = calloc(1, value_length);
		memcpy(chr->value, value, value_length);
		if (chr->value == NULL) {
			ret = BT_ERROR_OUT_OF_MEMORY;
			goto fail;
		}
	}

	*characteristic = (bt_gatt_h)chr;

	/* Add charactristic to list of GATT handles */
#ifdef GLIB_SUPPORTED
	gatt_handle_list = g_slist_append(gatt_handle_list, (gpointer)chr);
#else
	sq_addlast((sq_entry_t *)chr, &gatt_handle_list);
#endif

	return BT_ERROR_NONE;

fail:
	FREE(chr->value);
	FREE(chr->uuid);
	FREE(chr);
	BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	return ret;
}

int bt_gatt_characteristic_add_descriptor(bt_gatt_h characteristic,
					  bt_gatt_h descriptor)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)descriptor;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(descriptor);

	BT_VALIDATE_GATT_HANDLE(characteristic);
	BT_VALIDATE_GATT_HANDLE(descriptor);

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC ||
		desc->type != BT_GATT_TYPE_DESCRIPTOR) {
		BT_ERR("Wrong type. chr : %d, desc : %d", chr->type, desc->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (desc->parent) {
		BT_ERR("Already added descriptor : %p", desc->parent);
		return BT_ERROR_INVALID_PARAMETER;
	}

#ifdef GLIB_SUPPORTED
	chr->descriptors = g_slist_append(chr->descriptors, desc);
#else
	chr->descriptors = calloc(1, sizeof(sq_queue_t));
	sq_addlast((sq_entry_t *)desc, chr->descriptors);
#endif
	desc->parent = (void *)characteristic;

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_STOP */
int bt_gatt_characteristic_get_service(bt_gatt_h characteristic, bt_gatt_h *service)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(service); /* LCOV_EXCL_LINE */

	*service = (bt_gatt_h)chr->parent; /* LCOV_EXCL_LINE */

	return BT_ERROR_NONE; /* LCOV_EXCL_LINE */
}

int bt_gatt_characteristic_get_properties(bt_gatt_h characteristic, int *properties)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(properties); /* LCOV_EXCL_START */

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC) {
		BT_ERR("Wrong type of GATT handle : %d", chr->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	*properties = chr->properties;

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

/* LCOV_EXCL_START */
int bt_gatt_characteristic_set_properties(bt_gatt_h characteristic,
						int properties)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC) {
		BT_ERR("Wrong type of GATT handle : %d",
				chr->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	chr->properties = properties;

	return BT_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

int bt_gatt_characteristic_get_write_type(bt_gatt_h characteristic,
					bt_gatt_write_type_e *write_type)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(write_type); /* LCOV_EXCL_LINE */

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC) { /* LCOV_EXCL_LINE */
		BT_ERR("Wrong type of GATT handle : %d",
					chr->type); /* LCOV_EXCL_LINE */
		return BT_ERROR_INVALID_PARAMETER; /* LCOV_EXCL_LINE */
	}

	*write_type = chr->write_type; /* LCOV_EXCL_LINE */

	return BT_ERROR_NONE; /* LCOV_EXCL_LINE */
}

int bt_gatt_characteristic_set_write_type(bt_gatt_h characteristic,
					bt_gatt_write_type_e write_type)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	bt_gatt_property_e property = BT_GATT_PROPERTY_WRITE;
	int ret;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC) {
		BT_ERR("Wrong type of GATT handle : %d", chr->type); /* LCOV_EXCL_LINE */
		return BT_ERROR_INVALID_PARAMETER; /* LCOV_EXCL_LINE */
	}

	ret = __get_write_prop(write_type, &property);
	if (ret != BT_ERROR_NONE)
		return BT_ERROR_NOT_SUPPORTED;

	if (!(chr->properties & property))
		return BT_ERROR_NOT_SUPPORTED;

	chr->write_type = write_type;
	return BT_ERROR_NONE;
}

int bt_gatt_characteristic_get_descriptor(bt_gatt_h characteristic,
				const char *uuid, bt_gatt_h *descriptor)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	bt_gatt_h gatt_handle = NULL;
	int ret;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(uuid); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(descriptor);

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC) {
		BT_ERR("Wrong type of GATT handle : %d", chr->type);
		return BT_ERROR_INVALID_PARAMETER;
	}
	if (chr->descriptor_handles.count > 0) {
		ret = _bt_gatt_client_update_descriptors(chr);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("_bt_gatt_client_update_descriptors is failed");
			return ret;
		}
	}

	ret = __get_gatt_handle_by_uuid(chr->descriptors, uuid, &gatt_handle);
	if (ret == BT_ERROR_NONE && gatt_handle != NULL) {
		*descriptor = gatt_handle;
		return BT_ERROR_NONE;
	}

	BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	return ret; /* LCOV_EXCL_STOP */
}

int bt_gatt_characteristic_foreach_descriptors(bt_gatt_h characteristic,
				bt_gatt_foreach_cb callback, void *user_data)
{
#ifdef GLIB_SUPPORTED
	GSList *l;
#endif
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	int total;
	int i;

	BT_INFO("+");
	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(callback); /* LCOV_EXCL_START */

	if (chr->type != BT_GATT_TYPE_CHARACTERISTIC) {
		BT_ERR("Wrong type of GATT handle : %d", chr->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	BT_INFO("Total Descriptors for the current characteristic [%d]", chr->descriptor_handles.count);
	if (chr->descriptor_handles.count > 0) {
		int ret;

		ret = _bt_gatt_client_update_descriptors(chr);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("_bt_gatt_client_update_descriptors is failed");
			return ret;
		}
	}

#ifdef GLIB_SUPPORTED
	total = g_slist_length(chr->descriptors);
	BT_INFO("Total number of descriptors found [%d]", total);

	i = 1;
	for (l = chr->descriptors; l; l = g_slist_next(l)) {
		if (!callback(total, i++, (bt_gatt_h)l->data, user_data))
			break; /* LCOV_EXCL_STOP */
	}
#else
	total = __bt_sq_queue_length(chr->descriptors);
	BT_INFO("Total number of descriptors found [%d]", total);

	i = 1;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)sq_peek(chr->descriptors);
	while (desc) {
		if (!callback(total, i++, (bt_gatt_h)desc, user_data))
			break; /* LCOV_EXCL_STOP */
		desc = (bt_gatt_descriptor_s *)sq_next(desc);
	}
#endif

	return BT_ERROR_NONE;
}

/* LCOV_EXCL_START */
int bt_gatt_descriptor_create(const char *uuid, int permissions,
				const char *value, int value_length,
				bt_gatt_h *descriptor)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_descriptor_s *desc = NULL;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(uuid);
	BT_CHECK_INPUT_PARAMETER(descriptor);
	if (value_length > 0)
		BT_CHECK_INPUT_PARAMETER(value);

	desc = calloc(1, sizeof(*desc));
	memset(desc, 0x00, sizeof(*desc));

	desc->type = BT_GATT_TYPE_DESCRIPTOR;
	desc->role = BT_GATT_ROLE_SERVER;

	BT_INFO("Descriptor UUID : [%s] len [%zd] permissions [%d]", uuid, strlen(uuid), permissions);

	desc->uuid = _bt_convert_uuid_to_uuid128(uuid);

	if (desc->uuid == NULL) {
		ret = BT_ERROR_OUT_OF_MEMORY;
		goto fail;
	}

	desc->permissions = permissions;
	desc->value_length = value_length;
	if (value_length > 0) {
		//desc->value = g_memdup(value, value_length);
		desc->value = calloc(1, value_length);
		memcpy(desc->value, value, value_length);
		if (desc->value == NULL) {
			ret = BT_ERROR_OUT_OF_MEMORY;
			goto fail;
		}
	}

	*descriptor = (bt_gatt_h)desc;

	/* Add charactristic to list of GATT handles */
#ifdef GLIB_SUPPORTED
	gatt_handle_list = g_slist_append(gatt_handle_list, (gpointer)desc);
#else
	sq_addlast((sq_entry_t *)desc, &gatt_handle_list);
#endif

	return ret;

fail:
	FREE(desc->uuid);
	FREE(desc->value);
	FREE(desc);

	return ret;
}

/* LCOV_EXCL_STOP */
int bt_gatt_descriptor_get_characteristic(bt_gatt_h descriptor,
					bt_gatt_h *characteristic)
{
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)descriptor;

	BT_CHECK_GATT_SUPPORT();
	BT_CHECK_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(descriptor);
	BT_CHECK_INPUT_PARAMETER(characteristic);

	*characteristic = (bt_gatt_h)desc->parent;

	return BT_ERROR_NONE;
}

int bt_gatt_server_initialize(void)
{
	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	int ret = BT_ERROR_NONE;

	BT_INFO("gatt_server_initialized [%d] gatt_server_started[%d] instance_id [%d]",
		is_gatt_server_initialized, is_gatt_server_started, instance_id);

	if (is_gatt_server_started) {
		BT_ERR("Already Server started");
		return BT_ERROR_OPERATION_FAILED;
	}

	if (!is_gatt_server_initialized) {
//		ret = bt_adapt_gatt_server_init(&instance_id, _bt_gatt_server_event_proxy, NULL);
		ret = bt_adapt_gatt_server_init(&instance_id);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)",
				_bt_convert_error_to_string(ret), ret);
			return BT_ERROR_OPERATION_FAILED;
		}

		is_gatt_server_initialized = true;
		return BT_ERROR_NONE;
	}

	BT_INFO("Gatt-service already initialized");

	return ret;
}

int bt_gatt_server_deinitialize(void)
{
	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	int ret = BT_ERROR_NONE;

	BT_INFO("gatt_server_initialized [%d] gatt_server_started[%d] instance_id [%d]",
		is_gatt_server_initialized, is_gatt_server_started, instance_id);

	if (is_gatt_server_initialized) {
#ifdef GLIB_SUPPORTED
		GSList *l;
		for (l = gatt_server_list; l; l = g_slist_next(l)) {
			bt_gatt_server_s *serv = l->data;
			bt_gatt_server_destroy(serv);
		}
		g_slist_free(gatt_server_list);
		gatt_server_list = NULL;
#else
		bt_gatt_server_s *serv = (bt_gatt_server_s *)sq_peek(&gatt_server_list);
		bt_gatt_server_s *curr_serv = NULL;
		while (serv) {
			curr_serv = serv;
			serv = (bt_gatt_server_s *)sq_next(serv);
			bt_gatt_server_destroy(curr_serv);
		}
#endif

		ret = bt_adapt_gatt_server_deinit();
		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)",
				_bt_convert_error_to_string(ret), ret);
		}

		ret = bt_adapt_gatt_server_unregister(instance_id);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)",
				_bt_convert_error_to_string(ret), ret);
		}

		is_gatt_server_initialized = false;
		is_gatt_server_started = false;
		instance_id = -1;

		BT_INFO("gatt server deinitialised completed");
		return BT_ERROR_NONE;
	}

	BT_INFO("Gatt-service is not initialized");

	return ret;
}

int bt_gatt_server_create(bt_gatt_server_h *server)
{
	bt_gatt_server_s *serv = NULL;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(server);

	if (is_gatt_server_started) {
		BT_ERR("Already Server started");
		return BT_ERROR_OPERATION_FAILED;
	}

	serv = calloc(1, sizeof(bt_gatt_server_s));
	if (serv == NULL)
		return BT_ERROR_OUT_OF_MEMORY;

	*server = (bt_gatt_server_h)serv;

	BT_INFO("Creating a GATT Server");
#ifdef GLIB_SUPPORTED
	gatt_server_list = g_slist_append(gatt_server_list, serv);

	/* Add server to list of GATT handles */
	gatt_handle_list = g_slist_append(gatt_handle_list, (gpointer)serv);
#else
	sq_addlast((sq_entry_t *)serv, &gatt_server_list);

	/* Add server to list of GATT handles */
	sq_addlast((sq_entry_t *)serv, &gatt_handle_list);
#endif

	return BT_ERROR_NONE;
}

int bt_gatt_server_destroy(bt_gatt_server_h server)
{
	bt_gatt_server_s *serv = (bt_gatt_server_s *)server;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(server);
	BT_VALIDATE_GATT_HANDLE(server);
	BT_INFO("Destroy GATT Server");

	/* Remove Server from list of GATT handles */
#ifdef GLIB_SUPPORTED
	if (gatt_handle_list)
		gatt_handle_list = g_slist_remove(gatt_handle_list, serv);

	g_slist_free_full(serv->services, __bt_gatt_free_service);
	gatt_server_list = g_slist_remove(gatt_server_list, serv);
	g_free(serv);
#else
	sq_rem((sq_entry_t *)serv, &gatt_handle_list);
	__bt_gatt_free_service(serv->services);
	FREE(serv->services);

	sq_rem((sq_entry_t *)serv, &gatt_server_list);
	FREE(serv);
#endif

	return BT_ERROR_NONE;
}

int bt_gatt_server_set_read_value_requested_cb(bt_gatt_h gatt_handle,
				bt_gatt_server_read_value_requested_cb callback,
				void *user_data)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s*)gatt_handle;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(callback);

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		chr->read_requested_cb = callback;
		chr->read_requested_user_data = user_data;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		desc->read_requested_cb = callback;
		desc->read_requested_user_data = user_data;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	_bt_set_cb(BT_EVENT_GATT_SERVER_READ_REQUESTED, callback, user_data);

	return BT_ERROR_NONE;
}

int bt_gatt_server_set_characteristic_notification_state_change_cb(bt_gatt_h gatt_handle,
			bt_gatt_server_characteristic_notification_state_changed_cb callback,
			void *user_data)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(callback);

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	chr->notification_changed_cb = callback;
	chr->notification_changed_user_data = user_data;

	return BT_ERROR_NONE;
}

static int __gatt_service_add_num_handle(bt_gatt_service_s *service)
{
	int handles_count = 0;
#ifdef GLIB_SUPPORTED
	GSList *char_l = NULL;
#endif

	BT_CHECK_INPUT_PARAMETER(service);

	BT_INFO("total service handles [%d]", service->numhandles);

#ifdef GLIB_SUPPORTED
	for (char_l = service->characteristics; char_l; char_l = g_slist_next(char_l)) {
		bt_gatt_characteristic_s *chr = char_l->data;
		int desc_numhandles = 0;

		if (chr) {
			handles_count += 2;

			/*descriptor of each characteristic*/
			if (chr->descriptors) {
				desc_numhandles = g_slist_length(chr->descriptors);
				handles_count +=  desc_numhandles;
			}
		}
	}
#else
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)sq_peek(service->characteristics);
	while (chr) {
		int desc_numhandles = 0;
		handles_count += 2;

		/* descriptor of each characteristic */
		if (chr->descriptors) {
			//desc_numhandles = g_slist_length(chr->descriptors);
			desc_numhandles = __bt_sq_queue_length(chr->descriptors);
			handles_count += desc_numhandles;
		}

		chr = (bt_gatt_characteristic_s *)sq_next(chr);
	}
#endif

	service->numhandles += handles_count;
	BT_INFO("total service handles [%d]", service->numhandles);

	return BT_ERROR_NONE;
}

int bt_gatt_server_register_service(bt_gatt_server_h server, bt_gatt_h service)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_server_s *serv = (bt_gatt_server_s *)server;
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;
#ifdef GLIB_SUPPORTED
	GSList *char_l;
#endif

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(server);
	BT_CHECK_INPUT_PARAMETER(service);
	BT_VALIDATE_GATT_HANDLE(server);

#ifdef GLIB_SUPPORTED
	if (g_slist_find(serv->services, svc)) {
		BT_ERR("Already added service.");
		return BT_ERROR_ALREADY_DONE;
	}
#else
	if (__bt_sq_queue_find(serv->services, (sq_entry_t *)svc)) {
		BT_ERR("Already added service.");
		return BT_ERROR_ALREADY_DONE;
	}
#endif

	if (is_gatt_server_started) {
		BT_ERR("Already Server started");
		return BT_ERROR_OPERATION_FAILED;
	}

	__gatt_service_add_num_handle(svc);
	BT_INFO("Service number of total handles [%d]", svc->numhandles);

	ret = bt_adapt_gatt_server_add_service(svc->uuid, svc->type,
				svc->numhandles, instance_id, &svc->handle);

	BT_INFO("Service added handle [%d]", svc->handle);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		return ret;
	}

#ifdef GLIB_SUPPORTED
	for (char_l = svc->characteristics; char_l; char_l = g_slist_next(char_l)) { /* LCOV_EXCL_START */
		GSList *desc_l;
		bt_gatt_characteristic_s *chr = char_l->data;

		/* Add Characteristic to GATT database */

		BT_INFO("GATT Server Add Char: UUID [%s] permission [%d] properties [%d] service handle [%d] Inst ID [%d]",
				chr->uuid, chr->permissions, chr->properties, svc->handle, instance_id);

		ret = bt_adapt_gatt_server_add_new_char(chr->uuid, chr->permissions, chr->properties, svc->handle, instance_id, &chr->handle);

		BT_INFO("Char added handle [%d]", chr->handle);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)",
				_bt_convert_error_to_string(ret), ret);
			goto fail;
		}

		for (desc_l = chr->descriptors; desc_l; desc_l = g_slist_next(desc_l)) {
			bt_gatt_descriptor_s *desc = desc_l->data;

			/* Add descriptor to GATT database */
			BT_INFO("GATT Server Add Descriptor: UUID [%s] permission [%d] service handle [%d] Inst ID [%d]",
					desc->uuid, desc->permissions, svc->handle, instance_id);

			ret = bt_adapt_gatt_server_add_desc(desc->uuid, (bt_gatt_permission_e)desc->permissions,
						svc->handle, instance_id, &desc->handle);

			BT_INFO("Descriptor added handle [%d]", desc->handle);

			if (ret != BT_ERROR_NONE) {
				BT_ERR("%s(0x%08x)",
					_bt_convert_error_to_string(ret), ret);
				goto fail;
			}

		}
	} /* LCOV_EXCL_STOP */
#else
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)sq_peek(svc->characteristics);
	while (chr) {
		/* Add Characteristic to GATT database */
		BT_INFO("GATT Server Add Char: UUID [%s] permission [%d] properties [%d] service handle [%d] Inst ID [%d]",
				chr->uuid, chr->permissions, chr->properties, svc->handle, instance_id);

		ret = bt_adapt_gatt_server_add_new_char(chr->uuid, chr->permissions, chr->properties, svc->handle, instance_id, &chr->handle);

		BT_INFO("Char added handle [%d]", chr->handle);

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)",
				_bt_convert_error_to_string(ret), ret);
			goto fail;
		}

		bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)sq_peek(chr->descriptors);
		while (desc) {
			/* Add descriptor to GATT database */
			BT_INFO("GATT Server Add Descriptor: UUID [%s] permission [%d] service handle [%d] Inst ID [%d]",
					desc->uuid, desc->permissions, svc->handle, instance_id);

			ret = bt_adapt_gatt_server_add_desc(desc->uuid, (bt_gatt_permission_e)desc->permissions,
						svc->handle, instance_id, &desc->handle);

			BT_INFO("Descriptor added handle [%d]", desc->handle);

			if (ret != BT_ERROR_NONE) {
				BT_ERR("%s(0x%08x)",
					_bt_convert_error_to_string(ret), ret);
				goto fail;
			}
			desc = (bt_gatt_descriptor_s *)sq_next(desc);
		}
		chr = (bt_gatt_characteristic_s *)sq_next(chr);
	} /* LCOV_EXCL_STOP */
#endif

#ifdef GLIB_SUPPORTED
	serv->services = g_slist_append(serv->services, svc);
#else
	serv->services = calloc(1, sizeof(sq_queue_t));
	sq_addlast((sq_entry_t *)svc, serv->services);
#endif
	svc->parent = (void *)server;

	return ret;

fail:
//	bluetooth_gatt_unregister_service(svc->path);

	return ret;
}

int bt_gatt_server_unregister_service(bt_gatt_server_h server,
					bt_gatt_h service)
{
	bt_gatt_server_s *serv = (bt_gatt_server_s *)server;
	bt_gatt_service_s *svc = (bt_gatt_service_s *)service;

	BT_INFO("Unregister GATT Service");

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(server);
	BT_CHECK_INPUT_PARAMETER(service);

#ifdef GLIB_SUPPORTED
	if (!g_slist_find(serv->services, svc)) {
		BT_ERR("Service is NOT found.");
		return BT_ERROR_SERVICE_NOT_FOUND;
	}
#else
	if (!__bt_sq_queue_find(serv->services, (sq_entry_t *)svc)) {
		BT_ERR("Service is NOT found.");
		return BT_ERROR_SERVICE_NOT_FOUND;
	}
#endif

	bt_gatt_service_destroy(svc);

	return BT_ERROR_NONE;
}

int bt_gatt_server_unregister_all_services(bt_gatt_server_h server)
{
	int ret = BT_ERROR_NONE;

	bt_gatt_server_s *serv = (bt_gatt_server_s*)server;
	BT_INFO("Unregister All GATT Services");

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(server);

	//g_slist_free_full(serv->services, __bt_gatt_free_service);
	__bt_gatt_free_service(serv->services);
	serv->services = NULL;

	is_gatt_server_started = false;

	return ret;
}

int bt_gatt_server_start(void)
{
	int ret = BT_ERROR_NONE;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();

#ifdef GLIB_SUPPORTED
	if (!is_gatt_server_started) {
		GSList *l;
		GSList *l1;

		/* Kick Start all services*/
		BT_INFO("Total GATT Servers for this application [%d]", g_slist_length(gatt_server_list));
		for (l = gatt_server_list; l; l = g_slist_next(l)) {
			bt_gatt_server_s *server = (bt_gatt_server_s *)l->data;

			BT_INFO("Total GATT Services for this current server(server) of this App [%d]", g_slist_length(server->services));

			for (l1 = server->services; l1; l1 = g_slist_next(l1)) {
				bt_gatt_service_s *service = (bt_gatt_service_s *)l1->data;
				BT_DBG("Service handle [%d] Service UUID [%s]", service->handle, service->uuid);
				/* Mark the service Registered */
				ret = bt_adapt_gatt_server_start_service(service->handle, instance_id);

				if (ret != BT_ERROR_NONE)
					BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
			}
		}

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
			return ret;
		}

		is_gatt_server_started = true;
		return ret;
	}
#else
	if (!is_gatt_server_started) {
		/* Kick Start all services*/
		//BT_INFO("Total GATT Servers for this application [%d]", g_slist_length(gatt_server_list));
		bt_gatt_server_s *server = (bt_gatt_server_s *)sq_peek(&gatt_server_list);
		while (server) {
			bt_gatt_service_s *service = (bt_gatt_service_s *)sq_peek(server->services);
			while (service) {
				BT_DBG("Service handle [%d] Service UUID [%s]", service->handle, service->uuid);
				ret = bt_adapt_gatt_server_start_service(service->handle, instance_id);
				if (ret != BT_ERROR_NONE)
					BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

				service = (bt_gatt_service_s *)sq_next(service);
			}
			server = (bt_gatt_server_s *)sq_next(server);
		}

		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
			return ret;
		}

		is_gatt_server_started = true;
		return ret;
	}
#endif

	BT_DBG("Gatt-service already Running");

	return ret;
}

int bt_gatt_server_send_response(int request_id, bt_gatt_att_request_type_e request_type,
		int offset, int resp_status, char *value, int value_length)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_att_data_t resp;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();

	if (request_type == BT_GATT_REQUEST_TYPE_READ &&
		resp_status == BT_ATT_ERROR_NONE) {
		BT_CHECK_INPUT_PARAMETER(value);

		if (value_length < 0)
			return BT_ERROR_INVALID_PARAMETER;
	}

	memset(&resp,  0x00, sizeof(resp));

	/* Copy response data */
	resp.length = value_length;
	resp.offset = offset;
	memcpy(&resp.data[0], value, value_length);

	BT_INFO("GATT Server Send Response: Response type [%d]", request_type);
	BT_INFO("Req ID [%d] data_len [%d] offset [%d] Resp Status [%d]",
				request_id, value_length, offset, resp_status);

	ret = bt_adapt_gatt_server_send_response(request_type, request_id, resp_status, 0, &resp);

	if (ret != BT_ERROR_NONE)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}

int bt_gatt_server_notify_characteristic_changed_value(bt_gatt_h characteristic,
				bt_gatt_server_notification_sent_cb callback,
				const char *device_address, void *user_data)
{
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	bt_gatt_common_s *handle = (bt_gatt_common_s *)characteristic;
	int attribute_handle;
	bool need_confirmation;
	bt_gatt_att_data_t indication;
	int k;
	int ret = BT_ERROR_NONE;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(callback);

	BT_VALIDATE_GATT_HANDLE(characteristic);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		attribute_handle = chr->handle;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	if (!(chr->properties & BT_GATT_PROPERTY_INDICATE) &&
			!(chr->properties & BT_GATT_PROPERTY_NOTIFY)) {
		BT_ERR("Characteristic [%d] does not have either Notifcation or Indicate property enabled", attribute_handle);
		return BT_ERROR_INVALID_PARAMETER;
	}

	BT_INFO("GATT Server: Notify/Indicate char value changed to Remote GATT client");
	if (!device_address)
		BT_INFO("Indication to be Multicasted!");
	else
		BT_INFO("Indication to be Unicasted.. [%s]", device_address);

	if (chr->value_length > 0 && chr->value) { /* LCOV_EXCL_START */
		if (handle->role == BT_GATT_ROLE_SERVER) {

			/* DEBUG */
			for (k = 0; k < chr->value_length; k++)
				BT_DBG("Indication Data [%d] = [0x%x]", k, chr->value[k]);

			/* Copy Indication data */
			indication.length = chr->value_length;
			indication.offset = 0; /* Default value */
			memcpy(&indication.data[0], chr->value, chr->value_length);

			if (chr->properties & BT_GATT_PROPERTY_INDICATE) {
				BT_DBG("Characteritsic [%d]has Indicate Property", attribute_handle);
				need_confirmation = TRUE;
			} else {
				need_confirmation = FALSE;
				BT_DBG("Enable Notifcation for the characteristic [%d]", attribute_handle);
			}

			ret = bt_adapt_gatt_server_send_indication(device_address, chr->handle, need_confirmation, instance_id, &indication);
			BT_INFO("Sent Indication to Remote GATT client: result [%d]", ret);

			if (ret != BT_ERROR_NONE) {
				BT_ERR("%s(0x%08x)",
						_bt_convert_error_to_string(ret), ret);
				return BT_ERROR_OPERATION_FAILED;
			}
		}
	} /* LCOV_EXCL_STOP */

	/* Save the Remote GATT client address to be used for notification later on */
	_bt_convert_address_to_hex((unsigned char **)&chr->addr, device_address);

	chr->notified_cb = callback;
	chr->notified_user_data = user_data;

	return ret;
}

int bt_gatt_server_set_write_value_requested_cb(bt_gatt_h gatt_handle,
		bt_gatt_server_write_value_requested_cb callback,
		void *user_data)
{
	bt_gatt_common_s *handle = (bt_gatt_common_s *)gatt_handle;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;
	bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s*)gatt_handle;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(callback);

	BT_VALIDATE_GATT_HANDLE(gatt_handle);

	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		chr->write_value_requested_cb = callback;
		chr->write_value_requested_user_data = user_data;
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		desc->write_value_requested_cb = callback;
		desc->write_value_requested_user_data = user_data;
	} else {
		BT_ERR("Type is invalid(type:%d)", handle->type);
		return BT_ERROR_INVALID_PARAMETER;
	}

	return BT_ERROR_NONE;
}

int bt_gatt_server_get_service(bt_gatt_server_h server, const char *uuid,
				bt_gatt_h *service)
{
	bt_gatt_server_s *server_s = (bt_gatt_server_s *)server;
	bt_gatt_h gatt_handle = NULL;
	int ret;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(server);
	BT_CHECK_INPUT_PARAMETER(uuid);
	BT_CHECK_INPUT_PARAMETER(service);

	ret = __get_gatt_handle_by_uuid(server_s->services,
					uuid, &gatt_handle);
	if (ret == BT_ERROR_NONE && gatt_handle != NULL) {
		*service = gatt_handle;
		return BT_ERROR_NONE;
	}

	BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	return ret;
}

int bt_gatt_server_foreach_services(bt_gatt_server_h server,
		bt_gatt_foreach_cb callback, void *user_data)
{
	bt_gatt_server_s *server_s = (bt_gatt_server_s *)server;
#ifdef GLIB_SUPPORTED
	GSList *l = NULL;
#endif
	int total = 0;
	int index = 1;

	BT_CHECK_GATT_SERVER_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_GATT_SERVER_INIT_STATUS();

	BT_CHECK_INPUT_PARAMETER(server);
	BT_CHECK_INPUT_PARAMETER(callback);

#ifdef GLIB_SUPPORTED
	total = g_slist_length(server_s->services);

	for (l = server_s->services; l; l = g_slist_next(l)) {
		if (!callback(total, index++, (bt_gatt_h)l->data, user_data))
			break;
	}
#else
	total = __bt_sq_queue_length(server_s->services);

	bt_gatt_service_s *svc = (bt_gatt_service_s *)sq_peek(server_s->services);
	while (svc) {
		if (!callback(total, index++, svc, user_data))
			break;
		svc = (bt_gatt_service_s *)sq_next(svc);
	}
#endif

	return BT_ERROR_NONE;
}

int bt_gatt_client_create(const char *remote_address, bt_gatt_client_h *client)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_client_s *client_s;
	bool connected = false;
#ifdef GLIB_SUPPORTED
	GSList *l;
#endif

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(remote_address); /* LCOV_EXCL_START */

#ifdef GLIB_SUPPORTED
	BT_INFO("Create GATT client: address[%s] Total gatt client instance list [%d]",
			remote_address, g_slist_length(gatt_client_list));

	for (l = gatt_client_list; l; l = g_slist_next(l)) {
		bt_gatt_client_s *c = (bt_gatt_client_s *)l->data;
		BT_INFO("Already present GATT client instance addr [%s]", c->remote_address);

		if ((c == NULL) || (c->remote_address == NULL)) {
			BT_ERR("bt_gatt_client_create Error Parameter are NULL..\n");
			continue;
		} else if (!g_ascii_strcasecmp(c->remote_address, remote_address)) {
			BT_ERR("Gatt client for %s is already created",
					remote_address);
			return BT_ERROR_ALREADY_DONE;
		}
	}
#else
	BT_INFO("Create GATT client: address[%s] Total gatt client instance list [%d]",
			remote_address, __bt_sq_queue_length(&gatt_client_list));

	if (_bt_gatt_get_client(remote_address) != NULL) {
		BT_ERR("Gatt client for %s is already created", remote_address);
		return BT_ERROR_ALREADY_DONE;
	}
#endif

	client_s = calloc(1, sizeof(bt_gatt_client_s));
	if (client_s == NULL) {
		ret = BT_ERROR_OUT_OF_MEMORY;
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		return ret;
	}

	client_s->remote_address = strdup(remote_address);
	if (client_s->remote_address == NULL) {
		free(client_s->services);
		free(client_s);
		ret = BT_ERROR_OUT_OF_MEMORY;
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		return ret;
	}

	/* Attempt to Register GATT Client instance */

	ret = bt_adapt_gatt_client_init(remote_address, &client_s->client_id);

	if (BT_ERROR_NONE != ret) {
		free(client_s->remote_address);
		free(client_s);
		return ret;
	}
	if (bt_device_is_profile_connected(remote_address, BT_PROFILE_GATT,
				&connected) != BT_ERROR_NONE)
		BT_ERR("bt_device_is_profile_connected is failed");
	else
		client_s->connected = connected;

	ret = _bt_gatt_client_update_services(client_s);
	if (ret != BT_ERROR_NONE) {
		BT_INFO("_bt_gatt_client_update_services returns 0x%X. "
			"It could be updated when service is available.", ret);
		ret = BT_ERROR_NONE;
	}

	BT_INFO("GATT Client Handle is created instance ID [%d]",
				client_s->client_id);

	*client = (bt_gatt_client_h)client_s;
#ifdef GLIB_SUPPORTED
	gatt_client_list = g_slist_append(gatt_client_list, client_s);
#else
	sq_addlast((sq_entry_t *)client_s, &gatt_client_list);
#endif

	return ret; /* LCOV_EXCL_STOP */
}

int bt_gatt_client_destroy(bt_gatt_client_h client)
{
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client); /* LCOV_EXCL_START */
	BT_INFO("Destroy GATT client: Client interface ID [%d] REmote address [%s]",
			client_s->client_id, client_s->remote_address);

	if (client_s->service_changed_cb)
		bt_adapt_gatt_client_set_service_change_watcher(client_s->remote_address, FALSE);

#ifdef GLIB_SUPPORTED
	g_slist_free_full(client_s->services, __bt_gatt_free_service);
	gatt_client_list = g_slist_remove(gatt_client_list, client_s);
#else
	sq_rem((sq_entry_t *)client_s, &gatt_client_list);
	__bt_gatt_free_service(client_s->services);
#endif

	/* Unregister GATT Client instance: Unregister should never fail  */
	bt_adapt_gatt_client_deinit(client_s->client_id);
	FREE(client_s->remote_address);
	FREE(client_s);

	return BT_ERROR_NONE; /* LCOV_EXCL_STOP */
}

int bt_gatt_client_get_remote_address(bt_gatt_client_h client,
					char **remote_address)
{
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client); /* LCOV_EXCL_START */
	BT_CHECK_INPUT_PARAMETER(remote_address);

	*remote_address = strdup(client_s->remote_address);

	return BT_ERROR_NONE;
}

static bool __bt_gatt_client_is_in_progress(void)
{
	if (_bt_check_cb(BT_EVENT_GATT_CLIENT_WRITE_CHARACTERISTIC) ||
		_bt_check_cb(BT_EVENT_GATT_CLIENT_WRITE_DESCRIPTOR) ||
		_bt_check_cb(BT_EVENT_GATT_CLIENT_READ_CHARACTERISTIC) ||
		_bt_check_cb(BT_EVENT_GATT_CLIENT_READ_DESCRIPTOR)) {
		BT_ERR("Operation is in progress");
		return true;
	}

	return false; /* LCOV_EXCL_STOP */
}

/* LCOV_EXCL_START */
void _handle_gatt_client_read_completed_event(int result, bt_gatt_resp_data_t *resp)
{
	bt_gatt_common_s *handle;
	bt_gatt_client_request_completed_cb cb = NULL;
	void *user_data = NULL;

	if (resp == NULL) {
		BT_ERR("GATT Client response data is NULL");
		return;
	}

	handle = resp->user_data;
	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)handle;

		cb = chr->read_cb;
		user_data = chr->read_user_data;

		chr->read_cb = NULL;
		chr->read_user_data = NULL;

		if (!chr->parent) {
			BT_INFO("Already destroyed handle : %p", chr);
			if (!chr->write_cb)
				FREE(chr);
			return;
		}
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)handle;

		cb = desc->read_cb;
		user_data = desc->read_user_data;

		desc->read_cb = NULL;
		desc->read_user_data = NULL;

		if (!desc->parent) {
			BT_INFO("Already destroyed handle : %p", desc);
			if (!desc->write_cb)
				FREE(desc);
			return;
		}
	}

	if (result == BT_ERROR_NONE) {
		bt_gatt_set_value(handle,
				(char *)resp->value, (int)resp->len);
	}

	if (cb)
		cb(result, handle, user_data);

	return;
}

void _handle_gatt_client_write_completed_event(int result, bt_gatt_resp_data_t *resp)
{
	bt_gatt_common_s *handle;
	bt_gatt_client_request_completed_cb cb = NULL;
	void *user_data = NULL;

	if (resp == NULL) {
		BT_ERR("GATT Client response data is NULL");
		return;
	}

	handle = resp->user_data;
	if (handle->type == BT_GATT_TYPE_CHARACTERISTIC) {
		bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)handle;

		cb = chr->write_cb;
		user_data = chr->write_user_data;

		chr->write_cb = NULL;
		chr->write_user_data = NULL;

		if (!chr->parent) {
			BT_INFO("Already destroyed handle : %p", chr);
			if (!chr->read_cb)
				FREE(chr);
			return;
		}
	} else if (handle->type == BT_GATT_TYPE_DESCRIPTOR) {
		bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)handle;

		cb = desc->write_cb;
		user_data = desc->write_user_data;

		desc->write_cb = NULL;
		desc->write_user_data = NULL;

		if (!desc->parent) {
			BT_INFO("Already destroyed handle : %p", desc);
			if (!desc->read_cb)
				FREE(desc);
			return;
		}
	}

	if (cb)
		cb(result, handle, user_data);

	return;
}
/* LCOV_EXCL_STOP */

int bt_gatt_client_read_value(bt_gatt_h gatt_handle,
		bt_gatt_client_request_completed_cb callback, void *user_data)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_common_s *c = (bt_gatt_common_s *)gatt_handle;

	BT_INFO("+");
	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t chr_handle;
	bt_gatt_handle_prop_t desc_handle;
	bt_gatt_service_s *service_s;
	bt_gatt_characteristic_s *charc_s;
	bt_gatt_client_s  *client_s;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(callback); /* LCOV_EXCL_START */

	if (__bt_gatt_client_is_in_progress()) {
		BT_ERR("Operation is in progress");
		return BT_ERROR_NOW_IN_PROGRESS;
	}

	if (c->type == BT_GATT_TYPE_CHARACTERISTIC) {
		bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;

		if (chr->read_cb) {
			BT_ERR("read request is now in progress");
			return BT_ERROR_NOW_IN_PROGRESS;
		}

		service_s = (bt_gatt_service_s*)chr->parent;
		client_s = (bt_gatt_client_s*)service_s->parent;
		BT_INFO("Read Characteristic from remote address [%s] service UUID [%s] char UUID [%s]",
				client_s->remote_address,
				service_s->uuid,
				chr->uuid);

		/* Create Service and Charc handles */
		__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
		svc_handle.instance_id = service_s->instance_id;

		__bt_string_to_uuid_hex(chr->uuid, chr_handle.uuid);
		chr_handle.instance_id = chr->instance_id;

		ret = bt_adapt_gatt_client_read_char_value(
					client_s->remote_address,
					&svc_handle,
					&chr_handle);

		if (ret == BT_ERROR_NONE) {
			BT_INFO("Characteristic read scheduled successfully, set callback");
			chr->read_cb = callback;
			chr->read_user_data = user_data;
		} else {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		}
	} else if (c->type == BT_GATT_TYPE_DESCRIPTOR) {
		bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

		if (desc->read_cb) {
			BT_ERR("read request is now in progress");
			return BT_ERROR_NOW_IN_PROGRESS;
		}

		charc_s = (bt_gatt_characteristic_s*)desc->parent;
		service_s = (bt_gatt_service_s*)charc_s->parent;
		client_s = (bt_gatt_client_s*)service_s->parent;

		BT_INFO("Read Descriptor from remote address [%s] service UUID [%s] char UUID [%s] desc UUID [%s]",
				client_s->remote_address,
				service_s->uuid,
				charc_s->uuid,
				desc->uuid);
		/* Create Service and Charc & desc  handles */
		__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
		svc_handle.instance_id = service_s->instance_id;

		__bt_string_to_uuid_hex(charc_s->uuid, chr_handle.uuid);
		chr_handle.instance_id = charc_s->instance_id;

		__bt_string_to_uuid_hex(desc->uuid, desc_handle.uuid);
		desc_handle.instance_id = desc->instance_id;

		ret = bt_adapt_gatt_client_read_desc_value(
					client_s->remote_address,
					&svc_handle,
					&chr_handle,
					&desc_handle);

		if (ret == BT_ERROR_NONE) {
			BT_INFO("Descriptor read scheduled successfully, set callback");
			desc->read_cb = callback;
			desc->read_user_data = user_data;
		} else {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		}
	} else {
		ret = BT_ERROR_INVALID_PARAMETER;
		BT_ERR("Invalid handle type for read ");
	}

	BT_INFO("Result [%d]", ret);
	return ret; /* LCOV_EXCL_STOP */
}

int bt_gatt_client_write_value(bt_gatt_h gatt_handle,
		bt_gatt_client_request_completed_cb callback, void *user_data)
{
	int ret = BT_ERROR_NONE;
	bt_gatt_common_s *c = (bt_gatt_common_s *)gatt_handle;

	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t chr_handle;
	bt_gatt_handle_prop_t desc_handle;
	bt_gatt_service_s *service_s;
	bt_gatt_characteristic_s *charc_s;
	bt_gatt_client_s  *client_s;
	bt_gatt_att_data_t write_data;

	BT_INFO("+");
	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(gatt_handle);
	BT_CHECK_INPUT_PARAMETER(callback); /* LCOV_EXCL_START */

	if (__bt_gatt_client_is_in_progress()) {
		BT_ERR("Operation is in progress");
		return BT_ERROR_NOW_IN_PROGRESS;
	}

	if (c->type == BT_GATT_TYPE_CHARACTERISTIC) {
		bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)gatt_handle;

		int k;
		memset(&write_data,  0x00, sizeof(bt_gatt_att_data_t));
		write_data.length = chr->value_length;
		BT_INFO("Char write value length [%d]", write_data.length);
		for (k = 0; k < chr->value_length; k++)
			BT_INFO("Character val [%d] [0x%x]", k, chr->value[k]);
		memcpy(&write_data.data[0], chr->value, chr->value_length);
		for (k = 0; k < write_data.length; k++)
			BT_INFO("Write val [%d] [0x%x]", k, write_data.data[k]);

		service_s = (bt_gatt_service_s*)chr->parent;
		client_s = (bt_gatt_client_s*)service_s->parent;

		/* Create Service and Charc handles */
		__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
		svc_handle.instance_id = service_s->instance_id;

		__bt_string_to_uuid_hex(chr->uuid, chr_handle.uuid);
		chr_handle.instance_id = chr->instance_id;

		if (chr->write_type == BT_GATT_WRITE_TYPE_WRITE) {
			BT_INFO("Write characteristic: write type [%d]", chr->write_type);
			ret = bt_adapt_gatt_client_write_char_value_by_type(
						client_s->remote_address,
						&svc_handle,
						&chr_handle,
						&write_data,
						BT_GATT_WRITE_TYPE_WRITE);
		} else if (chr->write_type == BT_GATT_WRITE_TYPE_WRITE_NO_RESPONSE) {
			BT_INFO("Write characteristic: write type [%d]", chr->write_type);
			ret = bt_adapt_gatt_client_write_char_value_by_type(
						client_s->remote_address,
						&svc_handle,
						&chr_handle,
						&write_data,
						BT_GATT_WRITE_TYPE_WRITE_NO_RESPONSE);
		} else {
			BT_ERR("Unknow write type : %d", chr->write_type);
			ret = BT_ERROR_OPERATION_FAILED;
		}
		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		} else {
			BT_INFO("GATT char write value scheuled successfully, set callback");
			chr->write_cb = callback;
			chr->write_user_data = user_data;
		}
	} else if (c->type == BT_GATT_TYPE_DESCRIPTOR) {
		bt_gatt_descriptor_s *desc = (bt_gatt_descriptor_s *)gatt_handle;

		BT_INFO("Write Descriptor");
		int k;
		charc_s = (bt_gatt_characteristic_s*)desc->parent;
		service_s = (bt_gatt_service_s*)charc_s->parent;
		client_s = (bt_gatt_client_s*)service_s->parent;

		memset(&write_data,  0x00, sizeof(bt_gatt_att_data_t));
		write_data.length = desc->value_length;
		BT_INFO("Desc write value length [%d]", write_data.length);
		memcpy(&write_data.data[0], desc->value, desc->value_length);
		for (k = 0; k < write_data.length; k++)
			BT_INFO("Write val [%d] [0x%x]", k, write_data.data[k]);

		/* Create Service and Charc & desc  handles */
		__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
		svc_handle.instance_id = service_s->instance_id;

		__bt_string_to_uuid_hex(charc_s->uuid, chr_handle.uuid);
		chr_handle.instance_id = charc_s->instance_id;

		__bt_string_to_uuid_hex(desc->uuid, desc_handle.uuid);
		desc_handle.instance_id = desc->instance_id;

		ret = bt_adapt_gatt_client_write_desc_value(
					client_s->remote_address,
					&svc_handle,
					&chr_handle,
					&desc_handle,
					&write_data,
					BT_GATT_WRITE_TYPE_WRITE_NO_RESPONSE);
		if (ret != BT_ERROR_NONE) {
			BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		} else {
			BT_INFO("GATT Descriptor write value scheuled successfully, set callback");
			desc->write_cb = callback;
			desc->write_user_data = user_data;
		}
	} else {
		BT_ERR("Invalid handle type for write ");
	}

	return ret; /* LCOV_EXCL_STOP */
}

/* LCOV_EXCL_START */
void __uuid_hex_to_string(unsigned char *uuid, char *str)
{
	uint32_t uuid0, uuid4;
	uint16_t uuid1, uuid2, uuid3, uuid5;

	memcpy(&uuid0, &(uuid[0]), 4);
	memcpy(&uuid1, &(uuid[4]), 2);
	memcpy(&uuid2, &(uuid[6]), 2);
	memcpy(&uuid3, &(uuid[8]), 2);
	memcpy(&uuid4, &(uuid[10]), 4);
	memcpy(&uuid5, &(uuid[14]), 2);

	snprintf((char *)str, BT_UUID_STR_MAX, "%.8x-%.4x-%.4x-%.4x-%.8x%.4x",
		ntohl(uuid0), ntohs(uuid1),
		ntohs(uuid2), ntohs(uuid3),
		ntohl(uuid4), ntohs(uuid5));
	return;
}

#if 0
static bt_gatt_characteristic_s* __gatt_get_characteristic_handle(
				unsigned char *svc_uuid,
				int svc_inst,
				unsigned char *uuid,
				int instance_id,
				char *remote_address)
{
	GSList *l;
	GSList *ll;
	bt_gatt_client_h client;
	bt_gatt_client_s *client_s;
	bt_gatt_service_s *svc;
	bt_gatt_characteristic_s *chr;

	char uuid_string[BT_UUID_STR_MAX];
	char svc_uuid_string[BT_UUID_STR_MAX];

	__uuid_hex_to_string(svc_uuid, svc_uuid_string);
	__uuid_hex_to_string(uuid, uuid_string) ;

	BT_INFO("Address [%s] Char UUID [%s]", remote_address, uuid_string);


	client = _bt_gatt_get_client(remote_address);
	if (client == NULL) {
		BT_ERR("Cannot find client [%s]", remote_address);
		return NULL;
	}

	client_s = (bt_gatt_client_s *)client;
	for (l = client_s->services; l; l = g_slist_next(l)) {
		svc = (bt_gatt_service_s *)l->data;

		if (svc == NULL)
			continue;

		if (g_ascii_strcasecmp(svc->uuid, svc_uuid_string) == 0 &&
				svc->instance_id == svc_inst) {

			for (ll = svc->characteristics; ll; ll = g_slist_next(ll)) {
				chr = (bt_gatt_characteristic_s *)ll->data;

				if (chr == NULL)
					continue;
				if (g_ascii_strcasecmp(chr->uuid, uuid_string) == 0 &&
						chr->instance_id == instance_id) {
					return chr;
				}
			}
		}
	}
	return NULL;
}

static bt_gatt_descriptor_s* __gatt_get_descriptor_handle(
				unsigned char *svc_uuid,
				int svc_inst,
				unsigned char *char_uuid,
				int char_inst,
				unsigned char *uuid,
				int instance_id,
				char *remote_address)
{
	GSList *l;
	GSList *ll;
	GSList *lll;
	bt_gatt_client_h client;
	bt_gatt_client_s *client_s;
	bt_gatt_service_s *svc;
	bt_gatt_characteristic_s *chr;
	bt_gatt_descriptor_s *desc;

	char uuid_string[BT_UUID_STR_MAX];
	char svc_uuid_string[BT_UUID_STR_MAX];
	char char_uuid_string[BT_UUID_STR_MAX];

	__uuid_hex_to_string(uuid, uuid_string);
	__uuid_hex_to_string(svc_uuid, svc_uuid_string);
	__uuid_hex_to_string(char_uuid, char_uuid_string);

	BT_INFO("Address [%s] Descriptor UUID [%s]", remote_address, uuid_string);


	client = _bt_gatt_get_client(remote_address);
	if (client == NULL) {
		BT_ERR("Cannot find client [%s]", remote_address);
		return NULL;
	}

	client_s = (bt_gatt_client_s *)client;
	for (l = client_s->services; l; l = g_slist_next(l)) {
		svc = (bt_gatt_service_s *)l->data;

		if (svc == NULL)
			continue;

		if (g_ascii_strcasecmp(svc->uuid, svc_uuid_string) == 0 &&
				svc->instance_id == svc_inst) {

			for (ll = svc->characteristics; ll; ll = g_slist_next(ll)) {
				chr = (bt_gatt_characteristic_s *)ll->data;

				if (chr == NULL)
					continue;

				if (g_ascii_strcasecmp(chr->uuid, char_uuid_string) == 0 &&
						chr->instance_id == char_inst) {

					for (lll = chr->descriptors; lll; lll = g_slist_next(lll)) {
						desc = (bt_gatt_descriptor_s *)lll->data;

						if (desc == NULL)
							continue;

						if (g_ascii_strcasecmp(desc->uuid, uuid_string) == 0 &&
								desc->instance_id == instance_id) {
							return desc;
						}
					}
				}
			}
		}
	}
	return NULL;
}
#endif

static void __gatt_value_changed_cb(unsigned char *uuid, char *remote_address,
		char *value, int value_length, void *user_data)
{
#ifdef GLIB_SUPPORTED
	GSList *l;
	GSList *ll;
#endif
	bt_gatt_client_h client;
	bt_gatt_client_s *client_s;
	bt_gatt_service_s *svc;
	bt_gatt_characteristic_s *chr;
	BT_INFO("+");
	char uuid_string[BT_UUID_STR_MAX];
	__uuid_hex_to_string(uuid, uuid_string) ;

	BT_INFO("Address [%s] Char UUID [%s]", remote_address, uuid_string);

	client = _bt_gatt_get_client(remote_address);
	if (client == NULL) {
		BT_ERR("Cannot find client [%s]", remote_address);
		return;
	}

	client_s = (bt_gatt_client_s *)client;
#ifdef GLIB_SUPPORTED
	for (l = client_s->services; l; l = g_slist_next(l)) {
		svc = (bt_gatt_service_s *)l->data;
		for (ll = svc->characteristics; ll; ll = g_slist_next(ll)) {
			chr = (bt_gatt_characteristic_s *)ll->data;
			if (g_ascii_strcasecmp(chr->uuid, uuid_string) == 0) {
				if (chr->value_changed_cb)
					chr->value_changed_cb(chr, value,
						value_length,
						chr->value_changed_user_data);

				bt_gatt_set_value(chr, value, value_length);
				return;
			}
		}
	}
#else
	svc = (bt_gatt_service_s *)sq_peek(client_s->services);
	while (svc) {
		chr = (bt_gatt_characteristic_s *)sq_peek(svc->characteristics);
		while (chr) {
			if (strcasecmp(chr->uuid, uuid_string) == 0) {
				if (chr->value_changed_cb)
					chr->value_changed_cb(chr, value,
						value_length,
						chr->value_changed_user_data);

				bt_gatt_set_value(chr, value, value_length);
				return;
			}
			chr = (bt_gatt_characteristic_s *)sq_next(chr);
		}
		svc = (bt_gatt_service_s *)sq_next(svc);
	}
#endif
}

int bt_gatt_client_set_characteristic_value_changed_cb(bt_gatt_h characteristic,
		bt_gatt_client_characteristic_value_changed_cb callback,
		void *user_data)
{
	int ret;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	bool is_indicate = false;

	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t chr_handle;
	bt_gatt_service_s *service_s;
	bt_gatt_client_s  *client_s;

	BT_INFO("+");

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(characteristic);
	BT_CHECK_INPUT_PARAMETER(callback);

	chr->value_changed_cb = callback;
	chr->value_changed_user_data = user_data;

	service_s = (bt_gatt_service_s*)chr->parent;
	client_s = (bt_gatt_client_s*)service_s->parent;

	/* Create Service and Charc handles */
	__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
	svc_handle.instance_id = service_s->instance_id;

	__bt_string_to_uuid_hex(chr->uuid, chr_handle.uuid);
	chr_handle.instance_id = chr->instance_id;

	BT_INFO("Characteristic properties [%d] charc UUID [%s]",
			chr->properties, chr->uuid);

	if (chr->properties & BT_GATT_PROPERTY_INDICATE)
		is_indicate = true;

	if (chr->properties &
			(BT_GATT_PROPERTY_NOTIFY | BT_GATT_PROPERTY_INDICATE)) {

		ret = bt_adapt_gatt_client_watch_char(
					client_s->remote_address,
					&svc_handle,
					&chr_handle,
					client_s->client_id,
					TRUE,
					is_indicate);

		if (ret != BT_ERROR_NONE)
			BT_ERR("%s(0x%08x)",
					_bt_convert_error_to_string(ret), ret);
		else {
			BT_INFO("Set Char val changed callback at index [%d]", BT_EVENT_GATT_CLIENT_VALUE_CHANGED);
			_bt_set_cb(BT_EVENT_GATT_CLIENT_VALUE_CHANGED,
					__gatt_value_changed_cb, NULL);
			BT_INFO("Char value changed callback registsred successfully");
		}
	} else {
		BT_DBG("notify or indication is not supported");
		ret = BT_ERROR_NOT_SUPPORTED;
	}

	return ret;
}

int bt_gatt_client_unset_characteristic_value_changed_cb(bt_gatt_h characteristic)
{
	int ret;
	bt_gatt_characteristic_s *chr = (bt_gatt_characteristic_s *)characteristic;
	bool is_indicate = false;;
	bt_gatt_handle_prop_t svc_handle;
	bt_gatt_handle_prop_t chr_handle;
	bt_gatt_service_s *service_s;
	bt_gatt_client_s  *client_s;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(characteristic); /* LCOV_EXCL_START */

	BT_INFO("+");

	service_s = (bt_gatt_service_s*)chr->parent;
	client_s = (bt_gatt_client_s*)service_s->parent;

	/* Create Service and Charc handles */
	__bt_string_to_uuid_hex(service_s->uuid, svc_handle.uuid);
	svc_handle.instance_id = service_s->instance_id;

	__bt_string_to_uuid_hex(chr->uuid, chr_handle.uuid);
	chr_handle.instance_id = chr->instance_id;

	if (chr->properties & BT_GATT_PROPERTY_INDICATE)
		is_indicate = true;

	if (chr->properties &
			(BT_GATT_PROPERTY_NOTIFY | BT_GATT_PROPERTY_INDICATE)) {
		ret = bt_adapt_gatt_client_watch_char(
					client_s->remote_address,
					&svc_handle,
					&chr_handle,
					client_s->client_id,
					FALSE,
					is_indicate);
		if (ret != BT_ERROR_NONE)
			BT_ERR("%s(0x%08x)",
				_bt_convert_error_to_string(ret), ret);
		else {
			chr->value_changed_cb = NULL;
			chr->value_changed_user_data = NULL;
		}
	} else {
		BT_DBG("notify or indication is not supported");
		ret = BT_ERROR_NOT_SUPPORTED;
	}

	return ret; /* LCOV_EXCL_STOP */
}

int bt_gatt_client_get_service(bt_gatt_client_h client, const char *uuid,
				bt_gatt_h *service)
{
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;
	bt_gatt_h gatt_handle = NULL;
	int ret;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);
	BT_CHECK_INPUT_PARAMETER(uuid);
	BT_CHECK_INPUT_PARAMETER(service);

	BT_INFO("Get Service from UUID [%s]", uuid);
	ret = __get_gatt_handle_by_uuid(client_s->services, uuid, &gatt_handle);
	if (ret == BT_ERROR_NONE && gatt_handle != NULL) {
		*service = gatt_handle;
		return BT_ERROR_NONE;
	}

	BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	return ret;
}

int bt_gatt_client_foreach_services(bt_gatt_client_h client,
					bt_gatt_foreach_cb callback,
					void *user_data)
{
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;
#ifdef GLIB_SUPPORTED
	GSList *l = NULL;
#endif
	int total = 0;
	int index = 1;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);
	BT_CHECK_INPUT_PARAMETER(callback); /* LCOV_EXCL_LINE */

#ifdef GLIB_SUPPORTED
	total = g_slist_length(client_s->services); /* LCOV_EXCL_LINE */

	for (l = client_s->services; l; l = g_slist_next(l)) { /* LCOV_EXCL_LINE */
		BT_INFO("Call one service callback");
		if (!callback(total, index++,
			(bt_gatt_h)l->data, user_data)) /* LCOV_EXCL_LINE */
			break;
	}
#else
	total = __bt_sq_queue_length(client_s->services); /* LCOV_EXCL_LINE */

	bt_gatt_service_s *svc = (bt_gatt_service_s *)sq_peek(client_s->services);
	while (svc) {
		BT_INFO("Call one service callback");
		if (!callback(total, index++, svc, user_data))
			break;
		svc = (bt_gatt_service_s *)sq_next(svc);
	}
#endif

	return BT_ERROR_NONE;
}

int bt_gatt_client_set_service_changed_cb(bt_gatt_client_h client,
		bt_gatt_client_service_changed_cb callback, void *user_data)
{
	int ret;
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);
	BT_CHECK_INPUT_PARAMETER(callback);

	if (client_s->service_changed_cb) {
		BT_INFO("Already registered");
		client_s->service_changed_cb = callback;
		client_s->service_changed_user_data = user_data;
		return BT_ERROR_NONE;
	}

	ret = bt_adapt_gatt_client_set_service_change_watcher(client_s->remote_address, TRUE);

	if (ret == BT_ERROR_NONE) {
		BT_INFO("Service Changed callback registered");
		client_s->service_changed_cb = callback;
		client_s->service_changed_user_data = user_data;
	} else {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	}

	return ret;
}

int bt_gatt_client_unset_service_changed_cb(bt_gatt_client_h client)
{
	int ret;
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);

	ret = bt_adapt_gatt_client_set_service_change_watcher(client_s->remote_address, FALSE);

	if (ret == BT_ERROR_NONE) {
		BT_INFO("Service Changed callback unregistered [%s]", client_s->remote_address);
		client_s->service_changed_cb = NULL;
		client_s->service_changed_user_data = NULL;
	} else {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
	}

	return BT_ERROR_NONE;
}

#if 0
/* LCOV_EXCL_START */
void _bt_handle_gatt_client_char_read_completed_event(int result,
			void *resp)
{
	bt_gatt_client_request_completed_cb cb = NULL;
	void *user_data = NULL;
	bt_gatt_service_s *svc;
	bt_gatt_characteristic_s *chr;

	bt_gatt_char_property_t *char_prop1 = NULL; /* Pass Case */
	bluetooth_gatt_client_char_prop_info_t *char_prop2 = NULL;		/* Fail Case */
	bt_gatt_client_s *client_s;

	bt_gatt_h gatt_handle = NULL;
	char uuid_string[BLUETOOTH_UUID_STRING_MAX];

	if (resp == NULL) {
		BT_ERR("GATT Client response data is NULL");
		return;
	}
	if (result != BLUETOOTH_ERROR_NONE) {
		char_prop2 = (bluetooth_gatt_client_char_prop_info_t*)resp;
		char *device_addr = NULL;
		_bt_convert_address_to_string(&device_addr, &char_prop2->device_address);

		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(device_addr);
		if (!client_s) {
			BT_ERR("GATT client not found against address [%s]", device_addr);
			g_free(device_addr);
			return;
		}
		chr = __gatt_get_characteristic_handle(char_prop2->svc.uuid,
				char_prop2->svc.instance_id,
				char_prop2->characteristic.uuid,
				char_prop2->characteristic.instance_id,
				device_addr);

		__uuid_hex_to_string(char_prop2->characteristic.uuid, uuid_string);
		BT_INFO("GATT Char read completed Failed event UUID [%s] address [%s]",
				uuid_string, device_addr);
		g_free(device_addr);
		if (!chr) {
			BT_ERR("No characteristic found");
			return;
		}
	} else {
		char_prop1 = (bt_gatt_char_property_t *)resp;
		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(char_prop1->address);
		if (!client_s) {
			BT_ERR("GATT client not found against address [%s]",
					char_prop1->address);
			return;
		}
		chr = __gatt_get_characteristic_handle(
				char_prop1->svc_prop.uuid,
				char_prop1->svc_prop.instance_id,
				char_prop1->prop.uuid,
				char_prop1->prop.instance_id,
				char_prop1->address);

		if (!chr) {
			BT_ERR("No characteristic found");
			return;
		}

		__uuid_hex_to_string(char_prop1->prop.uuid, uuid_string);
		BT_INFO("GATT Char read completed Success event UUID [%s] address [%s]",
				uuid_string, char_prop1->address);
	}

	cb = chr->read_cb;
	user_data = chr->read_user_data;

	chr->read_cb = NULL;
	chr->read_user_data = NULL;

	if (!chr->parent) {
		BT_INFO("Already destroyed handle : %p", chr);
		if (!chr->write_cb)
			g_free(chr);
		return;
	}

	svc = (bt_gatt_service_s*) chr->parent;

	__get_gatt_handle_by_uuid(svc->characteristics, uuid_string, &gatt_handle);

	if (result == BLUETOOTH_ERROR_NONE) {
		BT_INFO("Set value");
		bt_gatt_set_value(gatt_handle,
				(char *)char_prop1->value, (int)char_prop1->val_len);
	}

	if (cb) {
		BT_INFO("callback present");
		cb(result, gatt_handle, user_data);
	} else
		BT_ERR("Callback not present!!!");
	return;
}

void _bt_handle_gatt_client_desc_read_completed_event(int result,
			void *resp)
{
	void *user_data = NULL;
	bt_gatt_h gatt_handle = NULL;
	bt_gatt_characteristic_s *chr;
	bt_gatt_client_request_completed_cb cb = NULL;

	bt_gatt_char_descriptor_property_t *desc_prop1 = NULL; /* Pass Case */
	bluetooth_gatt_client_desc_prop_info_t *desc_prop2 = NULL;		/* Fail Case */
	bt_gatt_descriptor_s *desc;
	bt_gatt_client_s *client_s;

	char uuid_string[BLUETOOTH_UUID_STRING_MAX];

	if (resp == NULL) {
		BT_ERR("GATT Client response data is NULL");
		return;
	}
	if (result != BLUETOOTH_ERROR_NONE) {
		desc_prop2 = (bluetooth_gatt_client_desc_prop_info_t*)resp;
		char *device_addr = NULL;
		_bt_convert_address_to_string(&device_addr, &desc_prop2->device_address);

		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(device_addr);
		if (!client_s) {
			BT_INFO("GATT client not found against address [%s]", device_addr);
			g_free(device_addr);
			return;
		}
		desc = __gatt_get_descriptor_handle(desc_prop2->svc.uuid,
						desc_prop2->svc.instance_id,
						desc_prop2->characteristic.uuid,
						desc_prop2->characteristic.instance_id,
						desc_prop2->descriptor.uuid,
						desc_prop2->descriptor.instance_id,
						device_addr);


		__uuid_hex_to_string(desc_prop2->descriptor.uuid, uuid_string);
		BT_INFO("GATT Desc read completed Failed event UUID [%s] address [%s]",
				uuid_string, device_addr);

		g_free(device_addr);
		if (!desc) {
			BT_ERR("No Descriptor found");
			return;
		}
	} else {
		desc_prop1 = (bt_gatt_char_descriptor_property_t *)resp;
		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(desc_prop1->address);
		if (!client_s) {
			BT_INFO("GATT client not found against address [%s]",
					desc_prop1->address);
			return;
		}
		desc = __gatt_get_descriptor_handle(desc_prop1->svc_prop.uuid,
						desc_prop1->svc_prop.instance_id,
						desc_prop1->char_prop.uuid,
						desc_prop1->char_prop.instance_id,
						desc_prop1->prop.uuid,
						desc_prop1->prop.instance_id,
						desc_prop1->address);
		if (!desc) {
			BT_ERR("No Descriptor found");
			return;
		}
		__uuid_hex_to_string(desc_prop1->prop.uuid, uuid_string);
		BT_INFO("GATT Desc read completed Success event UUID [%s] address [%s]",
				uuid_string, desc_prop1->address);
	}

	cb = desc->read_cb;
	user_data = desc->read_user_data;

	desc->read_cb = NULL;
	desc->read_user_data = NULL;

	if (!desc->parent) {
		BT_INFO("Already destroyed handle : %p", desc);
		if (!desc->write_cb)
			g_free(desc);
		return;
	}

	chr = (bt_gatt_characteristic_s*)desc->parent;

	__get_gatt_handle_by_uuid(chr->descriptors, uuid_string, &gatt_handle);

	if (result == BLUETOOTH_ERROR_NONE) {
		BT_INFO("Set value");
		bt_gatt_set_value(gatt_handle,
				(char *)desc_prop1->value, (int)desc_prop1->val_len);
	}

	if (cb) {
		BT_INFO("callback present");
		cb(result, gatt_handle, user_data);
	} else
		BT_ERR("Callback not present!!!!");

	return;
}

void _bt_handle_gatt_client_char_write_completed_event(int result,
		void *resp)
{
	bt_gatt_client_request_completed_cb cb = NULL;
	void *user_data = NULL;
	bt_gatt_service_s *svc;
	bt_gatt_h gatt_handle = NULL;

	bt_gatt_char_property_t *char_prop1; /* Pass Case */
	bluetooth_gatt_client_char_prop_info_t *char_prop2; /* Fail Case */
	bt_gatt_characteristic_s *chr;
	bt_gatt_client_s *client_s;

	char uuid_string[BLUETOOTH_UUID_STRING_MAX];

	if (resp == NULL) {
		BT_ERR("GATT Client response data is NULL");
		return;
	}

	BT_INFO("GATT Char Write Clalback result [%d]", result);

	if (result != BLUETOOTH_ERROR_NONE) {
		char_prop2 = (bluetooth_gatt_client_char_prop_info_t*)resp;
		char *device_addr = NULL;
		_bt_convert_address_to_string(&device_addr, &char_prop2->device_address);

		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(device_addr);
		if (!client_s) {
			BT_INFO("GATT client not found against address [%s]", device_addr);
			g_free(device_addr);
			return;
		}
		chr = __gatt_get_characteristic_handle(char_prop2->svc.uuid,
				char_prop2->svc.instance_id,
				char_prop2->characteristic.uuid,
				char_prop2->characteristic.instance_id,
				device_addr);

		__uuid_hex_to_string(char_prop2->characteristic.uuid, uuid_string);
		BT_INFO("GATT Char Write completed Failed event UUID [%s] address [%s]",
				uuid_string, device_addr);
		g_free(device_addr);
		if (!chr) {
			BT_ERR("No characteristic found");
			return;
		}
	} else {
		char_prop1 = (bt_gatt_char_property_t *)resp;
		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(char_prop1->address);
		if (!client_s) {
			BT_INFO("GATT client not found against address [%s]",
					char_prop1->address);
			return;
		}
		chr = __gatt_get_characteristic_handle(
				char_prop1->svc_prop.uuid,
				char_prop1->svc_prop.instance_id,
				char_prop1->prop.uuid,
				char_prop1->prop.instance_id,
				char_prop1->address);

		if (!chr) {
			BT_ERR("No characteristic found");
			return;
		}

		__uuid_hex_to_string(char_prop1->prop.uuid, uuid_string);
		BT_INFO("GATT Char Write completed Success event UUID [%s] address [%s]",
				uuid_string, char_prop1->address);
	}

	cb = chr->write_cb;
	user_data = chr->write_user_data;

	chr->write_cb = NULL;
	chr->write_user_data = NULL;

	if (!chr->parent) {
		BT_INFO("Already destroyed handle : %p", chr);
		if (!chr->read_cb)
			g_free(chr);
		return;
	}

	svc = (bt_gatt_service_s*) chr->parent;

	__get_gatt_handle_by_uuid(svc->characteristics, uuid_string, &gatt_handle);

	if (cb)
		cb(result, gatt_handle, user_data);
	else
		BT_ERR("Callback not present!!!");
	return;
}

void _bt_handle_gatt_client_desc_write_completed_event(int result,
				void *resp)
{
	void *user_data = NULL;
	bt_gatt_client_request_completed_cb cb = NULL;
	bt_gatt_characteristic_s *chr;
	bt_gatt_h gatt_handle;

	bt_gatt_char_descriptor_property_t *desc_prop1; /* Pass Case */
	bluetooth_gatt_client_desc_prop_info_t *desc_prop2;             /* Fail Case */
	bt_gatt_descriptor_s *desc;
	bt_gatt_client_s *client_s;

	char uuid_string[BLUETOOTH_UUID_STRING_MAX];

	if (resp == NULL) {
		BT_ERR("GATT Client response data is NULL");
		return;
	}
	if (result != BLUETOOTH_ERROR_NONE) {
		desc_prop2 = (bluetooth_gatt_client_desc_prop_info_t*)resp;
		char *device_addr = NULL;
		_bt_convert_address_to_string(&device_addr, &desc_prop2->device_address);

		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(device_addr);
		if (!client_s) {
			BT_INFO("GATT client not found against address [%s]", device_addr);
			g_free(device_addr);
			return;
		}
		desc = __gatt_get_descriptor_handle(desc_prop2->svc.uuid,
				desc_prop2->svc.instance_id,
				desc_prop2->characteristic.uuid,
				desc_prop2->characteristic.instance_id,
				desc_prop2->descriptor.uuid,
				desc_prop2->descriptor.instance_id,
				device_addr);

		__uuid_hex_to_string(desc_prop2->descriptor.uuid, uuid_string);
		BT_INFO("GATT Desc Write completed Failed event UUID [%s] address [%s]",
				uuid_string, device_addr);

		g_free(device_addr);
		if (!desc) {
			BT_ERR("No Descriptor found");
			return;
		}
	} else {
		desc_prop1 = (bt_gatt_char_descriptor_property_t *)resp;
		/* Return if Client not found */
		client_s = (bt_gatt_client_s *)_bt_gatt_get_client(desc_prop1->address);
		if (!client_s) {
			BT_INFO("GATT client not found against address [%s]",
					desc_prop1->address);
			return;
		}
		desc = __gatt_get_descriptor_handle(desc_prop1->svc_prop.uuid,
				desc_prop1->svc_prop.instance_id,
				desc_prop1->char_prop.uuid,
				desc_prop1->char_prop.instance_id,
				desc_prop1->prop.uuid,
				desc_prop1->prop.instance_id,
				desc_prop1->address);
		if (!desc) {
			BT_ERR("No Descriptor found");
			return;
		}
		__uuid_hex_to_string(desc_prop1->prop.uuid, uuid_string);
		BT_INFO("GATT Desc read completed Success event UUID [%s] address [%s]",
				uuid_string, desc_prop1->address);
	}

	cb = desc->write_cb;
	user_data = desc->write_user_data;

	desc->write_cb = NULL;
	desc->write_user_data = NULL;

	if (!desc->parent) {
		BT_INFO("Already destroyed handle : %p", desc);
		if (!desc->read_cb)
			g_free(desc);
		return;
	}

	chr = (bt_gatt_characteristic_s*)desc->parent;

	BT_INFO("Total descriptors char has [%d]", g_slist_length(chr->descriptors));

	__get_gatt_handle_by_uuid(chr->descriptors, uuid_string, &gatt_handle);

	if (cb)
		cb(result, gatt_handle, user_data);
	else
		BT_ERR("Callback not present!!!");

	return;
}
/* LCOV_EXCL_STOP */
#endif

int bt_gatt_client_request_att_mtu_change(bt_gatt_client_h client, unsigned int mtu)
{
	int ret;
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);

	ret = bt_adapt_request_att_mtu(client_s->remote_address, mtu);
	if (BT_ERROR_NONE != ret)
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);

	return ret;
}

int bt_gatt_client_get_att_mtu(bt_gatt_client_h client, unsigned int *mtu)
{
	int ret;
	unsigned int mtu_value = 0;
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);

	ret = bt_adapt_get_att_mtu(client_s->remote_address, &mtu_value);

	if (ret != BT_ERROR_NONE) {
		BT_ERR("%s(0x%08x)", _bt_convert_error_to_string(ret), ret);
		return ret;
	}

	*mtu = mtu_value;

	return ret;

}

int bt_gatt_client_set_att_mtu_changed_cb(bt_gatt_client_h client, bt_gatt_client_att_mtu_changed_cb callback, void *user_data)
{
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(callback);
	BT_CHECK_INPUT_PARAMETER(client);

	if (client_s->att_mtu_changed_cb) {
		BT_INFO("Already registered");
		client_s->att_mtu_changed_cb = callback;
		client_s->att_mtu_changed_user_data = user_data;
		return BT_ERROR_NONE;
	}

	BT_INFO("MTU Changed callback registered");
	client_s->att_mtu_changed_cb = callback;
	client_s->att_mtu_changed_user_data = user_data;

	return BT_ERROR_NONE;
}

int bt_gatt_client_unset_att_mtu_changed_cb(bt_gatt_client_h client)
{
	bt_gatt_client_s *client_s = (bt_gatt_client_s *)client;

	BT_CHECK_GATT_CLIENT_SUPPORT();
	BT_CHECK_INIT_STATUS();
	BT_CHECK_INPUT_PARAMETER(client);

	BT_INFO("MTU Changed callback unregistered [%s]", client_s->remote_address);
	client_s->service_changed_cb = NULL;
	client_s->service_changed_user_data = NULL;

	return BT_ERROR_NONE;
}
