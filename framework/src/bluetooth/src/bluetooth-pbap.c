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
#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
#include "bluetooth_private.h"

/* LCOV_EXCL_START */
int bt_pbap_client_initialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_set_connection_state_changed_cb(bt_pbap_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_connect(const char *address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_disconnect(const char *address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_is_connected(const char *address, bool *connected_status)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_get_phone_book_size(const char *address, bt_pbap_address_book_source_e source,
		bt_pbap_folder_type_e folder_type, bt_pbap_phone_book_size_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_get_phone_book(const char *address, bt_pbap_address_book_source_e source,
		bt_pbap_folder_type_e folder_type, bt_pbap_vcard_format_e format,
		bt_pbap_sort_order_e order, unsigned short offset,
		unsigned short max_list_count, unsigned int fields,
		bt_pbap_phone_book_received_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_get_list(const char *address, bt_pbap_address_book_source_e source,
		bt_pbap_folder_type_e folder_type, bt_pbap_sort_order_e order,
		unsigned short offset, unsigned short max_list_count,
		bt_pbap_list_vcards_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_pull_vcard(const char *address, bt_pbap_address_book_source_e source,
		bt_pbap_folder_type_e folder_type, int index,
		bt_pbap_vcard_format_e format, unsigned int fields,
		bt_pbap_phone_book_received_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_pbap_client_search_phone_book(const char *address,
		bt_pbap_address_book_source_e source, bt_pbap_folder_type_e folder_type,
		bt_pbap_search_field_e search_attribute, const char *search_value,
		bt_pbap_sort_order_e order,
		unsigned short offset, unsigned short max_list_count,
		bt_pbap_list_vcards_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */
