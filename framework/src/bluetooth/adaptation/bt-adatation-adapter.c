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
#include <bluetooth/bluetooth.h>
#include "bt-adaptation-adapter.h"
#include "bluetooth_private.h"

/* Need to include BT stack's header in here */

int bt_adapt_get_state(bt_adapter_state_e *adapter_state)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_enable(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_disable(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_get_local_address(char **address)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_get_local_name(char **name)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_local_name(const char *name)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_get_discoverable_mode(bt_adapter_visibility_mode_e *mode)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_discoverable_mode(bt_adapter_visibility_mode_e mode)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_start_discovery(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_stop_discovery(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_is_discovering(bool *is_discovering)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_is_connectable(bool *is_connectable)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_connectable(bool is_connectable)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

#ifdef GLIB_SUPPORTED
int bt_adapt_get_bonded_device_list(GPtrArray **dev_list)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}
#else
int bt_adapt_get_bonded_device_list(sq_queue_t *dev_list)
{
	/* To be implemented */
	struct bt_device_info_t *_device_info = NULL;
	_device_info = (struct bt_device_info_t *)malloc(sizeof(struct bt_device_info_t));
	//_device_info->dev_info = dev_info;
	sq_addlast((sq_entry_t *)_device_info, dev_list);

	return BT_ERROR_NONE;
}
#endif

int bt_adapt_start_le_scan(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_stop_le_scan(void)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_is_le_scanning(bool *is_scanning)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_start_adv(int handle, int interval_min,
				int interval_max, unsigned int filter_policy,
				unsigned int type)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_stop_adv(int handle)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_is_adv(bool *is_advertising)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_adv_data(int handle, const char *data, int len)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

int bt_adapt_set_scan_res_data(int handle, const char *data, int len)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

/*
	int type;  < passive 0, active 1 >
	float interval;  < LE scan interval >
	float window;  < LE scan window >
*/
int bt_adapt_set_scan_params(int type, unsigned int interval,
				unsigned int window)
{
	/* To be implemented */
	return BT_ERROR_NONE;
}

