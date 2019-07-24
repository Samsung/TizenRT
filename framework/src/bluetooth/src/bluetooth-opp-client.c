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

int bt_opp_client_initialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_client_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_client_add_file(const char *file)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_client_clear_files(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_client_push_files(const char *remote_address,
			bt_opp_client_push_responded_cb responded_cb,
			bt_opp_client_push_progress_cb progress_cb,
			bt_opp_client_push_finished_cb finished_cb,
			void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_client_cancel_push(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

