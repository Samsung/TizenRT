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
#include "bluetooth_private.h"

int bt_nap_activate(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_nap_deactivate(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_nap_disconnect_all(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_nap_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_nap_set_connection_state_changed_cb(
				bt_nap_connection_state_changed_cb callback,
				void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_nap_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_panu_set_connection_state_changed_cb(
				bt_panu_connection_state_changed_cb callback,
				void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_panu_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_panu_connect(const char *remote_address, bt_panu_service_type_e type)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_panu_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}
