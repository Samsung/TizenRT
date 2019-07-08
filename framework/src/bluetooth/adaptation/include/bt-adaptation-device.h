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


#ifndef __BT_ADAPTATION_DEVICE_H__
#define __BT_ADAPTATION_DEVICE_H__

#include <bluetooth/bluetooth_type.h>
#include <bluetooth/bluetooth_type_internal.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int bt_adapt_bond_device(const char *device_address);

int bt_adapt_unbond_device(const char *device_address);

int bt_adapt_get_bonded_device(const char *device_address,
					      bt_device_info_s *device_info);

int bt_adapt_is_device_connected(const char *device_address,
				bt_profile_e bt_profile,
				bool *is_connected);

int bt_adapt_connect_le(const char *device_address);

int bt_adapt_disconnect_le(const char *device_address);

int bt_adapt_le_conn_update(const char *device_address,
				const bt_le_conn_update_s *parameters);

int bt_adapt_update_le_connection_mode(const char *device_address,
				bt_device_le_connection_mode_e mode);

int bt_adapt_request_att_mtu(const char *device_address, unsigned int mtu);

int bt_adapt_get_att_mtu(const char *device_address, unsigned int *mtu);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BT_ADAPTATION_DEVICE_H__ */
