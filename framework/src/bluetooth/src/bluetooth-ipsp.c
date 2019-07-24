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

int bt_ipsp_initialize(bt_ipsp_init_state_changed_cb callback,
								void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ipsp_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ipsp_connect(const char *address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ipsp_disconnect(const char *address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ipsp_set_connection_state_changed_cb(bt_ipsp_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ipsp_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
