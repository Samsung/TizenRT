/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <tinyara/config.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include "bt-adaptation-gatt.h"
#include "bluetooth_private.h"

/* Need to include BT stack's header in here */

/*******************************
 *   GATT Common Functions     *
********************************/

int bt_adapt_gatt_free_svc_property(bt_gatt_svc_prop_t *svc_pty)
{
#ifdef GLIB_SUPPORTED
	g_free(svc_pty->uuid);
	g_free(svc_pty->handle);
	g_strfreev(svc_pty->include_handles.handle);
	g_strfreev(svc_pty->char_handle.handle);
#else
	FREE(svc_pty->uuid);
	FREE(svc_pty->handle);
	FREEV(svc_pty->include_handles.handle);
	FREEV(svc_pty->char_handle.handle);
#endif

	memset(svc_pty, 0, sizeof(bt_gatt_svc_prop_t));

	return BT_ERROR_NONE;
}

int bt_adapt_gatt_free_char_property(bt_gatt_char_prop_t *char_pty)
{
#ifdef GLIB_SUPPORTED
	g_free(char_pty->uuid);
	g_free(char_pty->name);
	g_free(char_pty->description);
	g_free(char_pty->val);
	g_free(char_pty->handle);
	g_strfreev(char_pty->char_desc_handle.handle);
#else
	FREE(char_pty->uuid);
	FREE(char_pty->name);
	FREE(char_pty->description);
	FREE(char_pty->val);
	FREE(char_pty->handle);
	FREEV(char_pty->char_desc_handle.handle);
#endif

	memset(char_pty, 0, sizeof(bt_gatt_char_prop_t));

	return BT_ERROR_NONE;
}

int bt_adapt_gatt_free_desc_property(bt_gatt_char_desc_prop_t *desc_pty)
{
#ifdef GLIB_SUPPORTED
	g_free(desc_pty->uuid);
	g_free(desc_pty->val);
	g_free(desc_pty->handle);
#else
	FREE(desc_pty->uuid);
	FREE(desc_pty->val);
	FREE(desc_pty->handle);
#endif

	memset(desc_pty, 0, sizeof(bt_gatt_char_desc_prop_t));

	return BT_ERROR_NONE;
}

int bt_adapt_gatt_connect(const char *address, bool auto_connect, int client_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_disconnect(const char *address, int client_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}


/*******************************
 *   GATT Client Functions     *
********************************/


int bt_adapt_gatt_client_init(const char *address, int *client_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_deinit(int client_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_get_primary_services(
		const char *address, /* Remote GATT Server */
		bt_gatt_handle_info_t *prim_svc) /* UUID & instance_id */
{

	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_get_svc_property(const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_svc_prop_t *service)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_get_char_property(const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_char_prop_t *char_property)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_get_char_desc_property(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_handle_prop_t *descriptor_handle,
		bt_gatt_char_desc_prop_t *desc_prop)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_watch_char(
			const char *address,
			bt_gatt_handle_prop_t *service_handle,
			bt_gatt_handle_prop_t *char_handle,
			int client_id,
			bool is_notify,
			bool is_indicate)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_unwatch_char(
			const char *address,
			bt_gatt_handle_prop_t *service_handle,
			bt_gatt_handle_prop_t *char_handle,
			int client_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_read_char_value(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_read_desc_value(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_handle_prop_t *descriptor_handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_write_char_value_by_type(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_att_data_t *data,
		bt_gatt_write_type_e  write_type)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_write_desc_value(
		const char *address,
		bt_gatt_handle_prop_t *service_handle,
		bt_gatt_handle_prop_t *char_handle,
		bt_gatt_handle_prop_t *descriptor_handle,
		bt_gatt_att_data_t *data,
		bt_gatt_write_type_e  write_type)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_client_set_service_change_watcher(
		const char *address, bool enable)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}


/*******************************
 *   GATT Server Functions     *
********************************/

int bt_adapt_gatt_server_init(int *instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_deinit(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_register(int instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_unregister(int instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_add_service(const char *svc_uuid, int type, int numhandles, int instance_id, int *service_handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_add_included_service(int instance_id, int service_handle, int included_handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_add_new_char(const char *char_uuid,
		bt_gatt_permission_e permissions, int properties,
		int service_handle, int instance_id, int *char_handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_add_desc(const char *desc_uuid,
		bt_gatt_permission_e permissions, int service_handle,
		int instance_id, int *descriptor_handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_start_service(int service_handle, int instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_stop_service(int service_handle, int instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_delete_service(int service_handle, int instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_send_indication(const char *address,
		int atrribute_handle,
		bool need_confirmation,
		int instance_id,
		const bt_gatt_att_data_t *att_value)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_send_response(bt_gatt_att_request_type_e req_type,
		int request_id,
		int response_status,
		bool auth_req,
		const bt_gatt_att_data_t *value)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_update_multi_adv_instance(const char *unique_name, int instance_id)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_gatt_server_update_char(int instance_id,
		int attribute_handle,
		const char *value,
		int length)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

