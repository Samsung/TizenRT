/******************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include "ble_tizenrt_impl.h"
#include "bk_ble_tizenrt.h"

#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#ifdef LOGE
    #undef LOGE
#endif

#ifdef LOGW
    #undef LOGW
#endif

#ifdef LOGI
    #undef LOGI
#endif

#ifdef LOGD
    #undef LOGD
#endif

#ifdef LOGV
    #undef LOGV
#endif

#ifdef LOG_TAG
    #undef LOG_TAG
#endif

#ifdef LOG_LEVEL
    #undef LOG_LEVEL
#endif

enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE,
};

#define LOG_TAG "bkblemgr"
#define LOG_LEVEL LOG_LEVEL_INFO

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)

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

static bool _check_mac_empty(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
	for (int i = 0; i < TRBLE_BD_ADDR_MAX_LEN; i++) {
		if (mac[i] != 0){
			return false;
		}
	}
	return true;
}

/*** Common ***/
trble_result_e trble_netmgr_init(struct bledev *dev, trble_client_init_config *client, trble_server_init_config *server);
trble_result_e trble_netmgr_deinit(struct bledev *dev);
trble_result_e trble_netmgr_get_mac_addr(struct bledev *dev, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e trble_netmgr_set_sec_param(struct bledev *dev, trble_sec_param *sec_param);
trble_result_e trble_netmgr_passkey_confirm(struct bledev *dev, uint8_t *conn_handle, uint8_t *confirm);
trble_result_e trble_netmgr_get_bonded_device(struct bledev *dev, trble_bonded_device_list_s *device_list, uint16_t *device_count);
trble_result_e trble_netmgr_delete_bond(struct bledev *dev, trble_addr *addr);
trble_result_e trble_netmgr_delete_bond_all(struct bledev *dev);
trble_result_e trble_netmgr_conn_is_active(struct bledev *dev, trble_conn_handle con_handle, bool *is_active);
trble_result_e trble_netmgr_conn_is_any_active(struct bledev *dev, bool *is_active);
trble_result_e trble_netmgr_conn_param_update(struct bledev *dev, trble_conn_handle *conn_handle, trble_conn_param *conn_param);
trble_result_e trble_netmgr_ioctl(struct bledev *dev, trble_msg_s *msg);

/*** Scanner(Observer) ***/
trble_result_e trble_netmgr_set_scan(struct bledev *dev, uint16_t scan_interval, uint16_t scan_window, trble_scan_type scan_type);
trble_result_e trble_netmgr_start_scan(struct bledev *dev, trble_scan_filter *filter);
trble_result_e trble_netmgr_stop_scan(struct bledev *dev);
trble_result_e trble_netmgr_scan_whitelist_add(struct bledev *dev, trble_addr *addr);
trble_result_e trble_netmgr_scan_whitelist_delete(struct bledev *dev, trble_addr *addr);
trble_result_e trble_netmgr_scan_whitelist_clear_all(struct bledev *dev);

/*** Central(Client) ***/
trble_result_e trble_netmgr_client_connect(struct bledev *dev, trble_conn_info *conn_info);
trble_result_e trble_netmgr_client_bond(struct bledev *dev, trble_conn_handle con_handle);
trble_result_e trble_netmgr_client_disconnect(struct bledev *dev, trble_conn_handle con_handle);
trble_result_e trble_netmgr_client_disconnect_all(struct bledev *dev);
trble_result_e trble_netmgr_connected_device_list(struct bledev *dev, trble_connected_list *out_connected_list);
trble_result_e trble_netmgr_connected_info(struct bledev *dev, trble_conn_handle conn_handle, trble_device_connected *out_connected_device);
trble_result_e trble_netmgr_operation_enable_notification(struct bledev *dev, trble_operation_handle *handle);
trble_result_e trble_netmgr_operation_enable_indication(struct bledev *dev, trble_operation_handle *handle);
trble_result_e trble_netmgr_operation_enable_notification_and_indication(struct bledev *dev, trble_operation_handle *handle);
trble_result_e trble_netmgr_operation_read(struct bledev *dev, trble_operation_handle *handle, trble_data *out_data);
trble_result_e trble_netmgr_operation_write(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data);
trble_result_e trble_netmgr_operation_write_no_response(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data);
trble_result_e trble_netmgr_write_read_queue_count(struct bledev *dev, trble_conn_handle *handle, uint8_t *count);

/*** Peripheral(Server) ***/
trble_result_e trble_netmgr_set_server_config(struct bledev *dev, trble_server_init_config *server);
trble_result_e trble_netmgr_get_profile_count(struct bledev *dev, uint16_t *count);
trble_result_e trble_netmgr_charact_notify(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data);
trble_result_e trble_netmgr_charact_indicate(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data);
trble_result_e trble_netmgr_indicate_queue_count(struct bledev *dev, trble_conn_handle *con_handle, uint8_t *count);
trble_result_e trble_netmgr_attr_set_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data);
trble_result_e trble_netmgr_attr_get_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data);
trble_result_e trble_netmgr_attr_reject(struct bledev *dev, trble_attr_handle attr_handle, uint8_t app_errorcode);
trble_result_e trble_netmgr_server_disconnect(struct bledev *dev, trble_conn_handle con_handle);
trble_result_e trble_netmgr_get_mac_addr_by_conn_handle(struct bledev *dev, trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN]);
trble_result_e trble_netmgr_get_conn_handle_by_addr(struct bledev *dev, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle);
trble_result_e trble_netmgr_set_gap_device_name(struct bledev *dev, uint8_t *device_name);

/*** Advertiser(Broadcaster) ***/
trble_result_e trble_netmgr_set_adv_data(struct bledev *dev, trble_data *data);
trble_result_e trble_netmgr_set_adv_resp(struct bledev *dev, trble_data *data);
trble_result_e trble_netmgr_set_adv_type(struct bledev *dev, trble_adv_type_e adv_type, trble_addr *addr);
trble_result_e trble_netmgr_set_adv_interval(struct bledev *dev, uint16_t interval);
trble_result_e trble_netmgr_set_adv_txpower(struct bledev *dev, uint16_t tx_power);
trble_result_e trble_netmgr_start_adv(struct bledev *dev);
trble_result_e trble_netmgr_stop_adv(struct bledev *dev);

trble_result_e trble_netmgr_one_shot_adv_init(struct bledev *dev);
trble_result_e trble_netmgr_one_shot_adv_deinit(struct bledev *dev);
trble_result_e trble_netmgr_one_shot_adv_set(struct bledev *dev, uint8_t *adv_id, trble_data *data_adv, trble_data *data_scan_rsp, uint8_t *type);
trble_result_e trble_netmgr_one_shot_adv(struct bledev *dev, uint8_t adv_id);
trble_result_e trble_netmgr_create_multi_adv(struct bledev *dev, uint8_t adv_event_prop, uint32_t *primary_adv_interval, uint8_t own_addr_type, uint8_t *own_addr_val, uint8_t *adv_handle);
trble_result_e trble_netmgr_delete_multi_adv(struct bledev *dev, uint8_t adv_handle);
trble_result_e trble_netmgr_set_multi_adv_data(struct bledev *dev, uint8_t adv_handle, uint8_t *pdata, uint8_t len);
trble_result_e trble_netmgr_set_multi_resp_data(struct bledev *dev, uint8_t adv_handle, uint8_t *pdata, uint8_t len);
trble_result_e trble_netmgr_start_multi_adv(struct bledev *dev, uint8_t adv_handle);
trble_result_e trble_netmgr_stop_multi_adv(struct bledev *dev, uint8_t adv_handle);

trble_result_e trble_netmgr_init(struct bledev *dev, trble_client_init_config *client, trble_server_init_config *server)
{
	printf("[%s : %d]\n", __FILE__, __LINE__);
	trble_result_e ret = TRBLE_INVALID_ARGS;

	if (client != NULL && server != NULL) {
		ret = bktr_ble_init(client, server);
	}

	if (ret == TRBLE_SUCCESS) {
		ret = bktr_ble_common_get_mac_address(dev->hwaddr);

		if (ret != TRBLE_SUCCESS) {
			LOGE("[TRBLE] Fail to get BLE mac\n");
		} else {
			// _reverse_mac(dev->hwaddr, NULL);
		}
	}
	return ret;
}

trble_result_e trble_netmgr_deinit(struct bledev *dev)
{
	trble_result_e ret = TRBLE_FAIL;
	ret = bktr_ble_deinit();
	if (ret == TRBLE_SUCCESS) {
		memset(dev->hwaddr, 0, TRBLE_BD_ADDR_MAX_LEN);
	}
	return ret;
}

trble_result_e trble_netmgr_get_mac_addr(struct bledev *dev, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
	if (_check_mac_empty(dev->hwaddr)){
		if (bktr_ble_common_get_mac_address(mac))
		{
			return TRBLE_FAIL;
		}
	} else {
		memcpy(mac, dev->hwaddr, TRBLE_BD_ADDR_MAX_LEN);
	}
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_set_sec_param(struct bledev *dev, trble_sec_param *sec_param)
{
	trble_sec_param sec_param_input;
	sec_param_input.io_cap = sec_param->io_cap;
	sec_param_input.oob_data_flag = sec_param->oob_data_flag;
	sec_param_input.bond_flag = sec_param->bond_flag;
	sec_param_input.mitm_flag = sec_param->mitm_flag;
	sec_param_input.sec_pair_flag = sec_param->sec_pair_flag;
	sec_param_input.sec_pair_only_flag = sec_param->sec_pair_only_flag;
	sec_param_input.use_fixed_key = sec_param->use_fixed_key;
	sec_param_input.fixed_key = sec_param->fixed_key;
	return bktr_ble_common_sm_set_security_param(&sec_param_input);
}

trble_result_e trble_netmgr_passkey_confirm(struct bledev *dev, uint8_t *conn_handle, uint8_t *confirm)
{
	return bktr_ble_common_pairing_passkey_confirm(conn_handle, confirm);
}

trble_result_e trble_netmgr_get_bonded_device(struct bledev *dev, trble_bonded_device_list_s *device_list, uint16_t *device_count)
{
	int i;
	trble_result_e ret = bktr_ble_common_get_bonded_device(device_list, device_count);

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
	memcpy(t_addr->mac, addr->mac, TRBLE_BD_ADDR_MAX_LEN);
	_reverse_mac(addr->mac, t_addr->mac);
	return bktr_ble_common_delete_bond(t_addr);
}

trble_result_e trble_netmgr_delete_bond_all(struct bledev *dev)
{
	return bktr_ble_common_delete_bond_all();
}

trble_result_e trble_netmgr_conn_is_active(struct bledev *dev, trble_conn_handle con_handle, bool *is_active)
{
	*is_active = bktr_ble_common_conn_is_active(con_handle);
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_conn_is_any_active(struct bledev *dev, bool *is_active)
{
	*is_active = bktr_ble_common_conn_is_any_active();
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_conn_param_update(struct bledev *dev, trble_conn_handle *conn_handle, trble_conn_param *conn_param)
{
	return bktr_ble_common_conn_param_update(conn_handle, conn_param);
}

trble_result_e trble_netmgr_ioctl(struct bledev *dev, trble_msg_s *msg)
{
	trble_result_e ret = TRBLE_UNSUPPORTED;

	if (msg->cmd == TRBLE_MSG_GET_VERSION) {
		uint8_t *version = (uint8_t *)msg->data;
		// temporary remove
		ret = bktr_ble_common_get_version(version);
	}
	return ret;
}

/*** Scanner(Observer) ***/
trble_result_e trble_netmgr_set_scan(struct bledev *dev, uint16_t scan_interval, uint16_t scan_window, trble_scan_type scan_type)
{
	return bktr_ble_client_set_scan(scan_interval, scan_window, scan_type);
}

trble_result_e trble_netmgr_start_scan(struct bledev *dev, trble_scan_filter *filter)
{
	return bktr_ble_client_start_scan_with_filter(filter, filter->whitelist_enable);
}

trble_result_e trble_netmgr_stop_scan(struct bledev *dev)
{
	return bktr_ble_client_stop_scan();
}

trble_result_e trble_netmgr_scan_whitelist_add(struct bledev *dev, trble_addr *addr)
{
	trble_addr t_addr[1] = { 0, };
	t_addr->type = addr->type;
	_reverse_mac(addr->mac, t_addr->mac);
	return bktr_ble_client_scan_whitelist_add(t_addr);
}

trble_result_e trble_netmgr_scan_whitelist_delete(struct bledev *dev, trble_addr *addr)
{
	trble_addr t_addr[1] = { 0, };
	t_addr->type = addr->type;
	_reverse_mac(addr->mac, t_addr->mac);
	return bktr_ble_client_scan_whitelist_delete(t_addr);
}

trble_result_e trble_netmgr_scan_whitelist_clear_all(struct bledev *dev)
{
	return bktr_ble_client_scan_whitelist_clear_all();
}

/*** Central(Client) ***/
trble_result_e trble_netmgr_client_connect(struct bledev *dev, trble_conn_info *conn_info)
{
	trble_conn_info conn_info_copy[1] = { 0, };
	memcpy(conn_info_copy, conn_info, sizeof(trble_conn_info));
	_reverse_mac(conn_info_copy->addr.mac, NULL);
	return bktr_ble_client_connect(conn_info_copy, conn_info_copy->is_secured_connect);
}

trble_result_e trble_netmgr_client_bond(struct bledev *dev, trble_conn_handle con_handle)
{
	return bktr_ble_client_bond(con_handle);
}

trble_result_e trble_netmgr_client_disconnect(struct bledev *dev, trble_conn_handle con_handle)
{
	return bktr_ble_client_disconnect(con_handle);
}

trble_result_e trble_netmgr_client_disconnect_all(struct bledev *dev)
{
	return bktr_ble_client_disconnect_all();
}

trble_result_e trble_netmgr_connected_device_list(struct bledev *dev, trble_connected_list *out_connected_list)
{
	return bktr_ble_client_read_connected_device_list(out_connected_list);
}

trble_result_e trble_netmgr_connected_info(struct bledev *dev, trble_conn_handle conn_handle, trble_device_connected *out_connected_device)
{
	return bktr_ble_client_read_connected_info(conn_handle, out_connected_device);
}

trble_result_e trble_netmgr_operation_enable_notification(struct bledev *dev, trble_operation_handle *handle)
{
	return bktr_ble_client_operation_enable_notification(handle);
}

trble_result_e trble_netmgr_operation_enable_indication(struct bledev *dev, trble_operation_handle *handle)
{
	return bktr_ble_client_operation_enable_indication(handle);
}

trble_result_e trble_netmgr_operation_enable_notification_and_indication(struct bledev *dev, trble_operation_handle *handle)
{
	return bktr_ble_client_operation_enable_notification_and_indication(handle);
}

trble_result_e trble_netmgr_operation_read(struct bledev *dev, trble_operation_handle *handle, trble_data *out_data)
{
	return bktr_ble_client_operation_read(handle, out_data);
}

trble_result_e trble_netmgr_operation_write(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data)
{
	return bktr_ble_client_operation_write(handle, in_data);
}

trble_result_e trble_netmgr_operation_write_no_response(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data)
{
	return bktr_ble_client_operation_write_no_response(handle, in_data);
}

trble_result_e trble_netmgr_write_read_queue_count(struct bledev *dev, trble_conn_handle *handle, uint8_t *count)
{
	return bktr_ble_client_write_read_queue_cnt(handle, count);
}

/*** Peripheral(Server) ***/
trble_result_e trble_netmgr_set_server_config(struct bledev *dev, trble_server_init_config *server)
{
    trble_result_e ret = bk_tr_ble_server_init(server);

    if(ret == TRBLE_ALREADY_WORKING)
    {
        ret = TRBLE_SUCCESS;
    }

    return ret;
}

trble_result_e trble_netmgr_get_profile_count(struct bledev *dev, uint16_t *count)
{
	*count = bktr_ble_server_get_profile_count();
	return TRBLE_SUCCESS;
}

trble_result_e trble_netmgr_charact_notify(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data)
{
    return bk_tr_ble_server_charact_notify(attr_handle, con_handle, data->data, data->length, 1);
}

trble_result_e trble_netmgr_charact_indicate(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data)
{
    return bk_tr_ble_server_charact_notify(attr_handle, con_handle, data->data, data->length, 0);
}

trble_result_e trble_netmgr_indicate_queue_count(struct bledev *dev, trble_conn_handle *con_handle, uint8_t *count)
{
    return bk_tr_ble_server_indicate_queue_count(con_handle, count);
}

trble_result_e trble_netmgr_attr_set_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data)
{
    LOGV("attr handle %d len %d", attr_handle, data->length);
    return bk_tr_ble_server_attr_set_data_ptr(attr_handle, data->data, data->length);
}

trble_result_e trble_netmgr_attr_get_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data)
{
    trble_result_e ret = TRBLE_SUCCESS;

    LOGV("attr handle %d", attr_handle);

    ret = bk_tr_ble_server_attr_get_data_ptr(attr_handle, &(data->data), &data->length, NULL);

    return ret;
}

trble_result_e trble_netmgr_attr_reject(struct bledev *dev, trble_attr_handle attr_handle, uint8_t app_errorcode)
{
    return bk_tr_ble_server_attr_reject(attr_handle, app_errorcode);
}

trble_result_e trble_netmgr_server_disconnect(struct bledev *dev, trble_conn_handle con_handle)
{
    return bk_tr_ble_server_disconnect(con_handle);
}

trble_result_e trble_netmgr_get_mac_addr_by_conn_handle(struct bledev *dev, trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN])
{
    return bk_tr_ble_server_get_mac_addr_by_conn_handle(con_handle, bd_addr);
}

trble_result_e trble_netmgr_get_conn_handle_by_addr(struct bledev *dev, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle)
{
    return bk_tr_ble_server_get_conn_handle_by_addr(bd_addr, con_handle);
}

trble_result_e trble_netmgr_set_gap_device_name(struct bledev *dev, uint8_t* device_name)
{
    uint8_t len = os_strlen(device_name);

    return bk_tr_ble_server_set_gap_device_name(len, device_name);
}

/*** Advertiser(Broadcaster) ***/
trble_result_e trble_netmgr_set_adv_data(struct bledev *dev, trble_data *data)
{
    return bk_tr_ble_advertiser_set_adv_data(data->data, data->length);
}

trble_result_e trble_netmgr_set_adv_resp(struct bledev *dev, trble_data *data)
{
    return bk_tr_ble_advertiser_set_adv_resp(data->data, data->length);
}

trble_result_e trble_netmgr_set_adv_type(struct bledev *dev, trble_adv_type_e adv_type, trble_addr *addr)
{
    return bk_tr_ble_advertiser_set_adv_type(adv_type, addr);
}

trble_result_e trble_netmgr_set_adv_interval(struct bledev *dev, uint16_t interval)
{
    return bk_tr_ble_advertiser_set_adv_interval(interval);
}

trble_result_e trble_netmgr_set_adv_txpower(struct bledev *dev, uint16_t tx_power)
{
    LOGE("This api is discarded, please replace trble_netmgr_set_multi_adv_tx_power!");
    return TRBLE_UNSUPPORTED;
}

trble_result_e trble_netmgr_stop_adv(struct bledev *dev)
{
    return bk_tr_ble_advertiser_adv_enable(0);
}

trble_result_e trble_netmgr_start_adv(struct bledev *dev)
{
    return bk_tr_ble_advertiser_adv_enable(1);
}

trble_result_e trble_netmgr_one_shot_adv_init(struct bledev *dev)
{
    return bk_tr_ble_advertiser_one_shot_adv_init();
}

trble_result_e trble_netmgr_one_shot_adv_deinit(struct bledev *dev)
{
    return bk_tr_ble_advertiser_one_shot_adv_deinit();
}

trble_result_e trble_netmgr_one_shot_adv_set(struct bledev *dev, uint8_t *adv_id,trble_data *data_adv, trble_data *data_scan_rsp, uint8_t* type)
{
    return bk_tr_ble_advertiser_one_shot_adv_set(adv_id, data_adv, data_scan_rsp, type);
}

trble_result_e trble_netmgr_one_shot_adv(struct bledev *dev, uint8_t adv_id)
{
    return bk_tr_ble_advertiser_one_shot_adv_enable(adv_id);
}

trble_result_e trble_netmgr_create_multi_adv(struct bledev *dev, uint8_t adv_event_prop, uint32_t *primary_adv_interval,
													uint8_t own_addr_type, uint8_t *own_addr_val, uint8_t *adv_handle)
{
    return bk_tr_ble_advertiser_create_multi_adv(adv_event_prop, primary_adv_interval, own_addr_type, own_addr_val, adv_handle);
}

trble_result_e trble_netmgr_delete_multi_adv(struct bledev *dev, uint8_t adv_handle)
{
    return bk_tr_ble_advertiser_delete_multi_adv(adv_handle);
}

trble_result_e trble_netmgr_set_multi_adv_data(struct bledev *dev, uint8_t adv_handle, uint8_t *pdata, uint8_t len)
{
    return bk_tr_ble_advertiser_set_multi_adv_data(adv_handle, pdata, len);
}

trble_result_e trble_netmgr_set_multi_resp_data(struct bledev *dev, uint8_t adv_handle, uint8_t *pdata, uint8_t len)
{
    return bk_tr_ble_advertiser_set_multi_resp_data(adv_handle, pdata, len);
}

trble_result_e trble_netmgr_start_multi_adv(struct bledev *dev, uint8_t adv_handle)
{
    return bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 1);
}

trble_result_e trble_netmgr_stop_multi_adv(struct bledev *dev, uint8_t adv_handle)
{
    return bk_tr_ble_advertiser_multi_adv_enable(adv_handle, 0);
}

trble_result_e trble_netmgr_set_multi_adv_type(struct bledev *dev, uint8_t adv_handle, uint8_t adv_event_prop, trble_addr *addr)
{
    return bk_tr_ble_advertiser_set_multi_adv_type(adv_handle, adv_event_prop, addr);
}

trble_result_e trble_netmgr_set_multi_adv_interval(struct bledev *dev, uint8_t adv_handle, unsigned int interval)
{
    return bk_tr_ble_advertiser_set_multi_adv_interval(adv_handle, interval);
}

trble_result_e trble_netmgr_set_multi_adv_tx_power(struct bledev *dev, uint8_t adv_handle, uint8_t txpower)
{
    return bk_tr_ble_advertiser_set_multi_adv_tx_power(adv_handle, txpower);
}

trble_result_e trble_netmgr_coc_init(struct bledev *dev, trble_le_coc_init_config *le_coc)
{
    return bk_tr_ble_coc_init(le_coc);
}

trble_result_e trble_netmgr_coc_register_psm(struct bledev *dev, uint8_t is_reg, uint16_t psm)
{
    return bk_tr_ble_coc_register_psm(is_reg, psm);
}

trble_result_e trble_netmgr_coc_set_psm_security(struct bledev *dev, uint16_t le_psm, uint8_t active, uint8_t sec_mode, uint8_t key_size)
{
    return bk_tr_ble_coc_set_psm_security(le_psm, active, sec_mode, key_size);
}

trble_result_e trble_netmgr_coc_set_param(struct bledev *dev, uint16_t value)
{
    return bk_tr_ble_coc_set_param(BK_LE_COC_CHAN_PARAM_LOCAL_MTU, value);
}

trble_result_e trble_netmgr_coc_chan_get_param(struct bledev *dev, uint8_t param_type, uint16_t cid, uint16_t *value)
{
    return bk_tr_ble_coc_chan_get_param(0xff, param_type, cid, value);
}

trble_result_e trble_netmgr_coc_connect(struct bledev *dev, uint16_t conn_handle, uint16_t le_psm)
{
    return bk_tr_ble_coc_connect(conn_handle, le_psm);
}

trble_result_e trble_netmgr_coc_disconnect(struct bledev *dev, uint16_t cid)
{
    return bk_tr_ble_coc_disconnect(0xff, cid);
}

trble_result_e trble_netmgr_coc_send_data(struct bledev *dev, uint16_t cid, uint16_t len, uint8_t *data)
{
    return bk_tr_ble_coc_send_data(0xff, cid, len, data);
}

struct trble_ops g_trble_drv_ops =
{
    // Common
    .init = trble_netmgr_init,
    .deinit = trble_netmgr_deinit,
    .get_mac = trble_netmgr_get_mac_addr,
    .set_sec_param = trble_netmgr_set_sec_param,
    .passkey_confirm = trble_netmgr_passkey_confirm,
    .get_bonded_dev = trble_netmgr_get_bonded_device,
    .del_bond = trble_netmgr_delete_bond,
    .del_bond_all = trble_netmgr_delete_bond_all,
    .conn_is_active = trble_netmgr_conn_is_active,
    .conn_is_any_active = trble_netmgr_conn_is_any_active,
    .conn_param_update = trble_netmgr_conn_param_update,
    .drv_ioctl = trble_netmgr_ioctl,

    // Observer
    .set_scan = trble_netmgr_set_scan,
    .start_scan = trble_netmgr_start_scan,
    .stop_scan = trble_netmgr_stop_scan,
    .whitelist_add = trble_netmgr_scan_whitelist_add,
    .whitelist_delete = trble_netmgr_scan_whitelist_delete,
    .whitelist_clear_all = trble_netmgr_scan_whitelist_clear_all,

    // Client
    .client_connect = trble_netmgr_client_connect,
    .start_bond = trble_netmgr_client_bond,
    .client_disconnect = trble_netmgr_client_disconnect,
    .client_disconnect_all = trble_netmgr_client_disconnect_all,
    .conn_dev_list = trble_netmgr_connected_device_list,
    .conn_info = trble_netmgr_connected_info,
    .op_enable_noti = trble_netmgr_operation_enable_notification,
    .op_enable_indi = trble_netmgr_operation_enable_indication,
    .op_enable_noti_n_indi = trble_netmgr_operation_enable_notification_and_indication,
    .op_read = trble_netmgr_operation_read,
    .op_write = trble_netmgr_operation_write,
    .op_wrtie_no_resp = trble_netmgr_operation_write_no_response,
    .get_write_read_queue_cnt = trble_netmgr_write_read_queue_count,

    // Server
    .set_server_config = trble_netmgr_set_server_config,
    .get_profile_count = trble_netmgr_get_profile_count,
    .charact_noti = trble_netmgr_charact_notify,
    .charact_indi = trble_netmgr_charact_indicate,
    .get_indi_queue_cnt = trble_netmgr_indicate_queue_count,
    .attr_set_data = trble_netmgr_attr_set_data,
    .attr_get_data = trble_netmgr_attr_get_data,
    .attr_reject = trble_netmgr_attr_reject,
    .server_disconnect = trble_netmgr_server_disconnect,
    .get_mac_by_conn = trble_netmgr_get_mac_addr_by_conn_handle,
    .get_conn_by_mac = trble_netmgr_get_conn_handle_by_addr,
    .set_gap_device_name = trble_netmgr_set_gap_device_name,

    // Broadcaster
    .set_adv_data = trble_netmgr_set_adv_data,
    .set_adv_resp = trble_netmgr_set_adv_resp,
    .set_adv_type = trble_netmgr_set_adv_type,
    .set_adv_interval = trble_netmgr_set_adv_interval,
    .set_adv_txpower = trble_netmgr_set_adv_txpower,
    .start_adv = trble_netmgr_start_adv,
    .stop_adv = trble_netmgr_stop_adv,
    .one_shot_adv_init = trble_netmgr_one_shot_adv_init,
    .one_shot_adv_deinit = trble_netmgr_one_shot_adv_deinit,
    .one_shot_adv_set = trble_netmgr_one_shot_adv_set,
    .one_shot_adv = trble_netmgr_one_shot_adv,

    .create_multi_adv = trble_netmgr_create_multi_adv,
    .delete_multi_adv = trble_netmgr_delete_multi_adv,
    .set_multi_adv_data = trble_netmgr_set_multi_adv_data,
    .set_multi_resp_data = trble_netmgr_set_multi_resp_data,
    .start_multi_adv = trble_netmgr_start_multi_adv,
    .stop_multi_adv = trble_netmgr_stop_multi_adv,
#if 0
    .set_multi_adv_type = trble_netmgr_set_multi_adv_type,
    .set_multi_adv_interval = trble_netmgr_set_multi_adv_interval,
    .set_multi_adv_tx_power = trble_netmgr_set_multi_adv_tx_power,
#endif

#if 1
    .le_coc_init = trble_netmgr_coc_init,
    .coc_register_psm = trble_netmgr_coc_register_psm,
    .coc_set_psm_security = trble_netmgr_coc_set_psm_security,
    .coc_set_param = trble_netmgr_coc_set_param,
    .coc_get_param = trble_netmgr_coc_chan_get_param,
    .coc_connect = trble_netmgr_coc_connect,
    .coc_disconnect = trble_netmgr_coc_disconnect,
    .coc_send_data = trble_netmgr_coc_send_data,
#endif
};
