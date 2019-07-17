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


#ifndef __BT_ADAPTATION_ADAPTER_H__
#define __BT_ADAPTATION_ADAPTER_H__

#ifdef GLIB_SUPPORTED
#include <glib.h>
#else
#include <queue.h>
#endif

#include <bluetooth/bluetooth_type.h>
#include <bluetooth/bluetooth_type_internal.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int bt_adapt_get_state(bt_adapter_state_e *adapter_state);

int bt_adapt_enable(void);

int bt_adapt_disable(void);

int bt_adapt_get_local_address(char **address);

int bt_adapt_get_local_name(char **name);

int bt_adapt_set_local_name(const char *name);

int bt_adapt_get_discoverable_mode(bt_adapter_visibility_mode_e *mode);

int bt_adapt_set_discoverable_mode(bt_adapter_visibility_mode_e mode);

int bt_adapt_start_discovery(void);

int bt_adapt_stop_discovery(void);

int bt_adapt_is_discovering(bool *is_discovering);

int bt_adapt_is_connectable(bool *is_connectable);

int bt_adapt_set_connectable(bool is_connectable);

#ifdef GLIB_SUPPORTED
int bt_adapt_get_bonded_device_list(GPtrArray **dev_list);
#else
int bt_adapt_get_bonded_device_list(sq_queue_t *dev_list);
#endif

int bt_adapt_start_le_scan(void);

int bt_adapt_stop_le_scan(void);

int bt_adapt_is_le_scanning(bool *is_scanning);

int bt_adapt_start_adv(int handle, int interval_min,
				int interval_max, unsigned int filter_policy,
				unsigned int type);

int bt_adapt_stop_adv(int handle);

int bt_adapt_is_adv(bool *is_advertising);

int bt_adapt_set_adv_data(int handle, const char *data, int len);

int bt_adapt_set_scan_res_data(int handle, const char *data, int len);

/*
	int type;  < passive 0, active 1 >
	float interval;  < LE scan interval >
	float window;  < LE scan window >
*/
int bt_adapt_set_scan_params(int type, unsigned int interval,
				unsigned int window);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BT_ADAPTATION_ADAPTER_H__ */
