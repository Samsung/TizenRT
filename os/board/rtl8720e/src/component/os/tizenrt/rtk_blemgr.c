/******************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/ble/ble_manager.h>
#include "rtk_ble_utils.h"

/* This value is between 5 and 95 */
#define WIFI_TIME_SLOT 56

#define TRBLE_TEST_LOG printf("[TRBLE] %s : %s \n", __FILE__, __FUNCTION__)
#define TRBLE_TEST_INFO printf
#define TRBLE_TEST_ERR printf

int wifi_is_running(unsigned char wlan_idx);

static void _reverse_mac(uint8_t *mac, uint8_t *target)
{
	int i;
	int j;
	uint8_t temp;
	if (target != NULL) {
		for (i = 0; i < TRBLE_BD_ADDR_MAX_LEN; i++) {
			target[i] = mac[TRBLE_BD_ADDR_MAX_LEN - (i + 1)];
		}
	} else {
		for (i = 0, j = TRBLE_BD_ADDR_MAX_LEN - 1; i < j; i++, j--) {
			temp = mac[i];
			mac[i] = mac[j];
			mac[j] = temp;
		}
	}
}

/*** Common ***/
trble_result_e trble_netmgr_init(struct bledev *dev, trble_client_init_config *client, trble_server_init_config *server);
trble_result_e trble_netmgr_deinit(struct bledev *dev);
trble_result_e trble_netmgr_get_mac_addr(struct bledev *dev, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e trble_netmgr_get_bonded_device(struct bledev *dev, trble_bonded_device_list_s *device_list, uint16_t *device_count);
trble_result_e trble_netmgr_delete_bond(struct bledev *dev, trble_addr *addr);
trble_result_e trble_netmgr_delete_bond_all(struct bledev *dev);
trble_result_e trble_netmgr_conn_is_active(struct bledev *dev, trble_conn_handle con_handle, bool *is_active);
trble_result_e trble_netmgr_conn_is_any_active(struct bledev *dev, bool *is_active);
trble_result_e trble_netmgr_ioctl(struct bledev *dev, trble_msg_s *msg);

/*** Scanner(Observer) ***/
trble_result_e trble_netmgr_start_scan(struct bledev *dev, trble_scan_filter *filter);
trble_result_e trble_netmgr_stop_scan(struct bledev *dev);
trble_result_e trble_netmgr_scan_whitelist_add(struct bledev *dev, trble_addr *addr);
trble_result_e trble_netmgr_scan_whitelist_delete(struct bledev *dev, trble_addr *addr);
trble_result_e trble_netmgr_scan_whitelist_clear_all(struct bledev *dev);

/*** Central(Client) ***/
trble_result_e trble_netmgr_client_connect(struct bledev *dev, trble_conn_info *conn_info);
trble_result_e trble_netmgr_client_disconnect(struct bledev *dev, trble_conn_handle con_handle);
trble_result_e trble_netmgr_client_disconnect_all(struct bledev *dev);
trble_result_e trble_netmgr_connected_device_list(struct bledev *dev, trble_connected_list *out_connected_list);
trble_result_e trble_netmgr_connected_info(struct bledev *dev, trble_conn_handle conn_handle, trble_device_connected *out_connected_device);
trble_result_e trble_netmgr_operation_enable_notification(struct bledev *dev, trble_operation_handle *handle);
trble_result_e trble_netmgr_operation_read(struct bledev *dev, trble_operation_handle *handle, trble_data *out_data);
trble_result_e trble_netmgr_operation_write(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data);
trble_result_e trble_netmgr_operation_write_no_response(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data);

/*** Peripheral(Server) ***/
trble_result_e trble_netmgr_get_profile_count(struct bledev *dev, uint16_t *count);
trble_result_e trble_netmgr_charact_notify(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data);
trble_result_e trble_netmgr_attr_set_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data);
trble_result_e trble_netmgr_attr_get_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data);
trble_result_e trble_netmgr_attr_reject(struct bledev *dev, trble_attr_handle attr_handle, uint8_t app_errorcode);
trble_result_e trble_netmgr_server_disconnect(struct bledev *dev, trble_conn_handle con_handle);
trble_result_e trble_netmgr_get_mac_addr_by_conn_handle(struct bledev *dev, trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e trble_netmgr_get_conn_handle_by_addr(struct bledev *dev, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle);

/*** Advertiser(Broadcaster) ***/
trble_result_e trble_netmgr_set_adv_data(struct bledev *dev, trble_data *data);
trble_result_e trble_netmgr_set_adv_resp(struct bledev *dev, trble_data *data);
trble_result_e trble_netmgr_set_adv_type(struct bledev *dev, trble_adv_type_e adv_type, trble_addr *addr);
trble_result_e trble_netmgr_set_adv_interval(struct bledev *dev, uint16_t interval);
trble_result_e trble_netmgr_start_adv(struct bledev *dev);
trble_result_e trble_netmgr_stop_adv(struct bledev *dev);

struct trble_ops g_trble_drv_ops = {
	// Common
	trble_netmgr_init,
	trble_netmgr_deinit,
	trble_netmgr_get_mac_addr,
	trble_netmgr_get_bonded_device,
	trble_netmgr_delete_bond,
	trble_netmgr_delete_bond_all,
	trble_netmgr_conn_is_active,
	trble_netmgr_conn_is_any_active,
	trble_netmgr_ioctl,

	// Observer
	trble_netmgr_start_scan,
	trble_netmgr_stop_scan,
	trble_netmgr_scan_whitelist_add,
	trble_netmgr_scan_whitelist_delete,
	trble_netmgr_scan_whitelist_clear_all,

	// Client
	trble_netmgr_client_connect,
	trble_netmgr_client_disconnect,
	trble_netmgr_client_disconnect_all,
	trble_netmgr_connected_device_list,
	trble_netmgr_connected_info,
	trble_netmgr_operation_enable_notification,
	NULL,
	NULL,
	trble_netmgr_operation_read,
	trble_netmgr_operation_write,
	trble_netmgr_operation_write_no_response,

	// Server
	trble_netmgr_get_profile_count,
	trble_netmgr_charact_notify,
	NULL,
	trble_netmgr_attr_set_data,
	trble_netmgr_attr_get_data,
	trble_netmgr_attr_reject,
	trble_netmgr_server_disconnect,
	trble_netmgr_get_mac_addr_by_conn_handle,
	trble_netmgr_get_conn_handle_by_addr,

	// Broadcaster
	trble_netmgr_set_adv_data,
	trble_netmgr_set_adv_resp,
	trble_netmgr_set_adv_type,
	trble_netmgr_set_adv_interval,
	NULL,
	trble_netmgr_start_adv,
	trble_netmgr_stop_adv,
};

trble_result_e trble_netmgr_init(struct bledev *dev, trble_client_init_config *client, trble_server_init_config *server)
{
	printf("[%s : %d]\n", __FILE__, __LINE__);
	trble_result_e ret = TRBLE_INVALID_ARGS;
	 if (!wifi_is_running(0)) {
	 	TRBLE_TEST_ERR("[TRBLE] WiFi is off. Please turn on WiFi first.\n");
	 	return TRBLE_FAIL;
	 }
#ifdef CONFIG_AMEBALITE_BLE_SCATTERNET
	if (client != NULL && server != NULL) {
		ret = rtw_ble_combo_init(client, server);
	}
#elif defined(CONFIG_AMEBALITE_BLE_CENTRAL)
	if (client != NULL) {
		ret = rtw_ble_client_init(client);
	}
#elif defined(CONFIG_AMEBALITE_BLE_PERIPHERAL)
	if (server != NULL) {
		ret = rtw_ble_server_init(server);
	}
#endif
	if (ret == TRBLE_SUCCESS) {
		ret = rtw_ble_server_get_mac_address(dev->hwaddr);

		if (ret != TRBLE_SUCCESS) {
			TRBLE_TEST_ERR("[TRBLE] Fail to get BLE mac\n");
		} else {
			_reverse_mac(dev->hwaddr, NULL);
		}
	}
	return ret;
}

trble_result_e trble_netmgr_deinit(struct bledev *dev)
{
	trble_result_e ret = TRBLE_FAIL;
#ifdef CONFIG_AMEBALITE_BLE_SCATTERNET
	ret = rtw_ble_combo_deinit();
#elif defined(CONFIG_AMEBALITE_BLE_CENTRAL)
	ret = rtw_ble_client_deinit();
#elif defined(CONFIG_AMEBALITE_BLE_PERIPHERAL)
	ret = rtw_ble_server_deinit();
#endif
	if (ret == TRBLE_SUCCESS) {
		memset(dev->hwaddr, 0, TRBLE_BD_ADDR_MAX_LEN);
	}
	return ret;
}

trble_result_e trble_netmgr_get_mac_addr(struct bledev *dev, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
	memcpy(mac, dev->hwaddr, TRBLE_BD_ADDR_MAX_LEN);
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_get_bonded_device(struct bledev *dev, trble_bonded_device_list_s *device_list, uint16_t *device_count)
{
	int i;
	trble_result_e ret = rtw_ble_server_get_bonded_device(device_list, device_count);
	
	if (ret == TRBLE_SUCCESS) {
		for (i = 0; i < *device_count; i++) {
			_reverse_mac(device_list[i].bd_addr.mac, NULL);
		}
	}

	return ret;
}

trble_result_e trble_netmgr_delete_bond(struct bledev *dev, trble_addr *addr)
{
	trble_addr t_addr[1] = { 0, };
	t_addr->type = addr->type;
	_reverse_mac(addr->mac, t_addr->mac);
	return rtw_ble_client_delete_bond(t_addr);
}

trble_result_e trble_netmgr_delete_bond_all(struct bledev *dev)
{
	return rtw_ble_client_delete_bond_all();
}

trble_result_e trble_netmgr_conn_is_active(struct bledev *dev, trble_conn_handle con_handle, bool *is_active)
{
	*is_active = rtw_ble_server_conn_is_active(con_handle);
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_conn_is_any_active(struct bledev *dev, bool *is_active)
{
	*is_active = rtw_ble_server_conn_is_any_active();
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_ioctl(struct bledev *dev, trble_msg_s *msg)
{
	trble_result_e ret = TRBLE_UNSUPPORTED;
	
	if (msg->cmd == TRBLE_MSG_GET_VERSION) {
		// uint8_t *version = (uint8_t *)msg->data;
		// temporary remove
		// ret = rtw_ble_get_version(version);
	}
	return ret;
}

/*** Scanner(Observer) ***/
trble_result_e trble_netmgr_start_scan(struct bledev *dev, trble_scan_filter *filter)
{
	return rtw_ble_client_start_scan_with_filter(filter, filter->whitelist_enable);
}

trble_result_e trble_netmgr_stop_scan(struct bledev *dev)
{
	return rtw_ble_client_stop_scan();
}

trble_result_e trble_netmgr_scan_whitelist_add(struct bledev *dev, trble_addr *addr)
{
	trble_addr t_addr[1] = { 0, };
	t_addr->type = addr->type;
	_reverse_mac(addr->mac, t_addr->mac);
	return rtw_ble_client_scan_whitelist_add(t_addr);
}

trble_result_e trble_netmgr_scan_whitelist_delete(struct bledev *dev, trble_addr *addr)
{
	trble_addr t_addr[1] = { 0, };
	t_addr->type = addr->type;
	_reverse_mac(addr->mac, t_addr->mac);
	return rtw_ble_client_scan_whitelist_delete(t_addr);
}

trble_result_e trble_netmgr_scan_whitelist_clear_all(struct bledev *dev)
{
	return rtw_ble_client_scan_whitelist_clear_all();
}

/*** Central(Client) ***/
trble_result_e trble_netmgr_client_connect(struct bledev *dev, trble_conn_info *conn_info)
{
	trble_conn_info conn_info_copy[1] = { 0, };
	memcpy(conn_info_copy, conn_info, sizeof(trble_conn_info));
	_reverse_mac(conn_info_copy->addr.mac, NULL);
	return rtw_ble_client_connect(conn_info_copy, conn_info_copy->is_secured_connect);
}

trble_result_e trble_netmgr_client_disconnect(struct bledev *dev, trble_conn_handle con_handle)
{
	return rtw_ble_client_disconnect(con_handle);
}

trble_result_e trble_netmgr_client_disconnect_all(struct bledev *dev)
{
	return rtw_ble_client_disconnect_all();
}

trble_result_e trble_netmgr_connected_device_list(struct bledev *dev, trble_connected_list *out_connected_list)
{
	return rtw_ble_client_read_connected_device_list(out_connected_list);
}

trble_result_e trble_netmgr_connected_info(struct bledev *dev, trble_conn_handle conn_handle, trble_device_connected *out_connected_device)
{
	return rtw_ble_client_read_connected_info(conn_handle, out_connected_device);
}

trble_result_e trble_netmgr_operation_enable_notification(struct bledev *dev, trble_operation_handle *handle)
{
	return rtw_ble_client_operation_enable_notification(handle);
}

trble_result_e trble_netmgr_operation_read(struct bledev *dev, trble_operation_handle *handle, trble_data *out_data)
{
	return rtw_ble_client_operation_read(handle, out_data);
}

trble_result_e trble_netmgr_operation_write(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data)
{
	return rtw_ble_client_operation_write(handle, in_data);
}

trble_result_e trble_netmgr_operation_write_no_response(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data)
{
	return rtw_ble_client_operation_write_no_response(handle, in_data);
}


/*** Peripheral(Server) ***/
trble_result_e trble_netmgr_get_profile_count(struct bledev *dev, uint16_t *count)
{
	*count = rtw_ble_server_get_profile_count();
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_charact_notify(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data)
{
	return rtw_ble_server_charact_notify(attr_handle, con_handle, data->data, data->length);
}

trble_result_e trble_netmgr_attr_set_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data)
{
	trble_result_e ret = rtw_ble_server_att_set_data_ptr(attr_handle, data->data);
	if (ret != TRBLE_SUCCESS) {
		return ret;
	}
	ret = rtw_ble_server_att_set_length(attr_handle, data->length);
	return ret;
}

trble_result_e trble_netmgr_attr_get_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data)
{
	trble_result_e ret = TRBLE_SUCCESS;

	if (data != NULL && data->data != NULL){
		data->data = rtw_ble_server_att_get_data_ptr(attr_handle);
		data->length = rtw_ble_server_att_get_length(attr_handle);
	} else {
		return TRBLE_FAIL;
	}
	if (data->data == NULL || data->length == 0) {
		ret = TRBLE_FAIL;
	}
	return ret;
}

trble_result_e trble_netmgr_attr_reject(struct bledev *dev, trble_attr_handle attr_handle, uint8_t app_errorcode)
{
	return rtw_ble_server_reject(attr_handle, app_errorcode);
}

trble_result_e trble_netmgr_server_disconnect(struct bledev *dev, trble_conn_handle con_handle)
{
	return rtw_ble_server_disconnect(con_handle);
}

trble_result_e trble_netmgr_get_mac_addr_by_conn_handle(struct bledev *dev, trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN])
{
	trble_result_e ret = TRBLE_SUCCESS;
	uint8_t *mac;
	
	mac = rtw_ble_server_get_mac_address_by_conn_handle(con_handle);
	if (mac == NULL) {
		return TRBLE_FAIL;
	}
	memcpy(bd_addr, mac, TRBLE_BD_ADDR_MAX_LEN);

	return ret;
}

trble_result_e trble_netmgr_get_conn_handle_by_addr(struct bledev *dev, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle)
{
	*con_handle = rtw_ble_server_get_conn_handle_by_address(bd_addr);
	return TRBLE_SUCCESS;
}


/*** Advertiser(Broadcaster) ***/
trble_result_e trble_netmgr_set_adv_data(struct bledev *dev, trble_data *data)
{
	return rtw_ble_server_set_adv_data(data->data, data->length);
}

trble_result_e trble_netmgr_set_adv_resp(struct bledev *dev, trble_data *data)
{
	return rtw_ble_server_set_adv_name(data->data, data->length);
}

trble_result_e trble_netmgr_set_adv_type(struct bledev *dev, trble_adv_type_e adv_type, trble_addr *addr)
{
	return rtw_ble_server_set_adv_type(adv_type, addr);
}

trble_result_e trble_netmgr_set_adv_interval(struct bledev *dev, uint16_t interval)
{
	return rtw_ble_server_set_adv_interval(interval);
}

trble_result_e trble_netmgr_stop_adv(struct bledev *dev)
{
	return rtw_ble_server_stop_adv();
}

trble_result_e trble_netmgr_start_adv(struct bledev *dev)
{
	return rtw_ble_server_start_adv();
}
