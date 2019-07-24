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

/* LCOV_EXCL_START */
int bt_opp_server_initialize(const char *destination,
			bt_opp_server_push_requested_cb push_requested_cb,
			void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_opp_server_initialize_by_connection_request(const char *destination,
		bt_opp_server_connection_requested_cb connection_requested_cb,
		void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_server_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_server_accept(bt_opp_server_transfer_progress_cb progress_cb,
			bt_opp_server_transfer_finished_cb finished_cb,
			const char *name,
			void *user_data,
			int *transfer_id)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_server_reject(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_server_set_destination(const char *destination)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_opp_server_cancel_transfer(int transfer_id)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_opp_get_transfer_progress(bt_opp_transfer_type_t transfer_type,
		int transfer_id, unsigned char *progress)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */
