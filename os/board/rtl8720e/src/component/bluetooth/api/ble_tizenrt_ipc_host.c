/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include "ameba_soc.h"
#include <bt_ipc_host_api.h>
#include <ble_tizenrt_ipc.h>

trble_server_init_config *ble_tizenrt_server_init_param = NULL;
trble_client_init_config *ble_tizenrt_client_init_param = NULL;

void bt_customer_evt_hdl(uint8_t evt_code, uint8_t *param)
{
	if (BLE_TIZENRT_EVT_CONNECTED == evt_code) {
		T_TIZENRT_CONNECTED_CALLBACK_DATA *cb = (T_TIZENRT_CONNECTED_CALLBACK_DATA *)param;
		(ble_tizenrt_server_init_param->connected_cb)(cb->conn_id, cb->conn_type, cb->remote_bd);
	} else if (BLE_TIZENRT_EVT_PROFILE == evt_code) {
		T_TIZENRT_PROFILE_CALLBACK_DATA *profile = (T_TIZENRT_PROFILE_CALLBACK_DATA *)param;
		uint16_t count = ble_tizenrt_server_init_param->profile_count;
		trble_gatt_t *tmp = ble_tizenrt_server_init_param->profile; 
		
		for(int i = 0; i < count; i++){
			if((tmp+ i)->attr_handle == profile->att_handle){
				(tmp+ i)->cb(profile->type, profile->conn_id, profile->att_handle, profile->arg);
				break;
			}
		}
	}else if(BLE_TIZENRT_EVT_CONN == evt_code) {
		(ble_tizenrt_client_init_param->trble_device_connected_cb)((trble_device_connected *)param);
	}else if(BLE_TIZENRT_EVT_SCAN_STATE == evt_code){
		(ble_tizenrt_client_init_param->trble_scan_state_changed_cb)(*param);
	}else if(BLE_TIZENRT_EVT_SCANNED_DEVICE == evt_code){
		(ble_tizenrt_client_init_param->trble_device_scanned_cb)((trble_scanned_device *)param);
	}else if(BLE_TIZENRT_EVT_DISCONNECTED == evt_code){
		(ble_tizenrt_client_init_param->trble_device_disconnected_cb)(*param);
	}else if(BLE_TIZENRT_EVT_NOTIFICATION == evt_code){
		T_TIZENRT_CLIENT_NOTIFICATION *notify_result = (T_TIZENRT_CLIENT_NOTIFICATION *)param;
		(ble_tizenrt_client_init_param->trble_operation_notification_cb)(&notify_result->handle, &notify_result->noti_data);
	}
}

trble_result_e rtw_ble_server_init(trble_server_init_config* init_parm)
{
	int *ret = NULL;
    trble_result_e err;
	ble_tizenrt_server_init_param = init_parm;
		
	if (sizeof(trble_server_init_config) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_INIT,
                                (uint32_t *)init_parm, sizeof(trble_server_init_config));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_server_deinit(void)
{
	int *ret = NULL;
    trble_result_e err;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_DEINIT,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_server_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
	int *ret = NULL;
    trble_result_e err;

	if (TRBLE_BD_ADDR_MAX_LEN > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_GET_MAC_ADDRESS,
                                		NULL, 0);
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} else {
		memcpy(mac, &ret[1], TRBLE_BD_ADDR_MAX_LEN);
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_server_att_set_data_ptr(trble_attr_handle attr_handle, uint8_t *new_data_ptr)
{
	int *ret = NULL;
    trble_result_e err;
	struct data_param_s {
        trble_attr_handle attr_handle;
        uint8_t new_data_ptr;
    } data_param = {0};

	data_param.attr_handle = attr_handle;
	memcpy(&data_param.new_data_ptr, new_data_ptr, sizeof(uint8_t));
	
	if (sizeof(data_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SET_DATA_PTR,
                                (uint32_t *)&data_param, sizeof(data_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_server_att_set_length(trble_attr_handle attr_handle, uint16_t new_length)
{
	int *ret = NULL;
    trble_result_e err;
	struct len_param_s {
        trble_attr_handle attr_handle;
        uint16_t new_length;
    } len_param = {0};

	len_param.attr_handle = attr_handle;
	len_param.new_length = new_length;
	
	if (sizeof(len_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SET_LENGTH,
                                (uint32_t *)&len_param, sizeof(len_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

uint8_t* rtw_ble_server_att_get_data_ptr(trble_attr_handle attr_handle)
{
	int *ret = NULL;
    trble_result_e err;
	uint8_t *data_ptr = NULL;
	data_ptr = (uint8_t *)osif_mem_alloc(0, sizeof(uint8_t));
	
	if (sizeof(trble_attr_handle) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_GET_DATA_PTR,
                                (uint32_t *)&attr_handle, sizeof(trble_attr_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} else {
		memcpy(data_ptr, &ret[1], sizeof(*data_ptr));
	}
    err = ret[0];
    osif_mem_free(ret);
    return data_ptr;

}

uint16_t rtw_ble_server_att_get_length(trble_attr_handle attr_handle)
{
	int *ret = NULL;
    trble_result_e err;
	uint16_t length;
	
	if (sizeof(trble_attr_handle) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_GET_LENGTH,
                                (uint32_t *)&attr_handle, sizeof(trble_attr_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} else {
		length = ret[1];
	}
    err = ret[0];
    osif_mem_free(ret);
    return length;

}

trble_result_e rtw_ble_server_charact_notify(trble_attr_handle attr_handle, trble_conn_handle con_handle, uint8_t *data_ptr, uint16_t data_length)
{
	int *ret = NULL;
    trble_result_e err;
	struct chara_param_s {
        trble_attr_handle attr_handle;
        trble_conn_handle con_handle;
		uint8_t data_ptr;
		uint16_t data_length;
    } chara_param = {0};

	chara_param.attr_handle = attr_handle;
	chara_param.con_handle = con_handle;
	memcpy(&chara_param.data_ptr, data_ptr, sizeof(uint8_t));
	chara_param.data_length = data_length;
	
	if (sizeof(chara_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_CHARACT_NOTIFY,
                                (uint32_t *)&chara_param, sizeof(chara_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_server_reject(trble_attr_handle attr_handle, uint8_t app_errorcode)
{
	int *ret = NULL;
    trble_result_e err;
	struct server_param_s {
        trble_attr_handle attr_handle;
        uint8_t app_errorcode;
    } server_param = {0};

	server_param.attr_handle = attr_handle;
	server_param.app_errorcode = app_errorcode;
	
	if (sizeof(server_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SERVER_REJECT,
                                (uint32_t *)&server_param, sizeof(server_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

static uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN];
uint8_t* rtw_ble_server_get_mac_address_by_conn_handle(trble_conn_handle con_handle)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_conn_handle) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_GET_MAC_ADDRESS_BY_CONN_HANDLE,
                                (uint32_t *)&con_handle, sizeof(trble_conn_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} else {
		memcpy(bd_addr, &ret[1], TRBLE_BD_ADDR_MAX_LEN);
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return bd_addr;

}

trble_conn_handle rtw_ble_server_get_conn_handle_by_address(uint8_t* mac)
{
	int *ret = NULL;
    trble_result_e err;
	trble_conn_handle conn_id = 0xff;
	
	if (sizeof(*mac) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_GET_CONN_HANDLE_BY_ADDRESS,
                                (uint32_t *)mac, TRBLE_BD_ADDR_MAX_LEN);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} else {
		conn_id = ret[1];
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return conn_id;

}

void rtw_ble_server_adv_into_idle(void)
{
	int *ret = NULL;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_ADV_INTO_IDLE,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}

    osif_mem_free(ret);
}

trble_result_e rtw_ble_server_set_adv_data(uint8_t* data, uint16_t length)
{
	int *ret = NULL;
    trble_result_e err;
	struct adv_param_s {
        uint8_t data;
        uint16_t length;
    } adv_param = {0};

	memcpy(&adv_param.data, data, sizeof(uint8_t));
	adv_param.length = length;
	
	if (sizeof(adv_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SET_ADV_DATA,
                                (uint32_t *)&adv_param, sizeof(adv_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_server_set_adv_name(uint8_t* data, uint16_t length)
{
	int *ret = NULL;
    trble_result_e err;
	struct name_param_s {
        uint8_t data;
        uint16_t length;
    } name_param = {0};

	memcpy(&name_param.data, data, sizeof(uint8_t));
	name_param.length = length;
	
	if (sizeof(name_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SET_ADV_NAME,
                                (uint32_t *)&name_param, sizeof(name_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

bool rtw_ble_server_conn_is_active(trble_conn_handle con_handle)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_conn_handle) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_CONN_IS_ACTIVE,
                                (uint32_t *)&con_handle, sizeof(trble_conn_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} 
	
    err = ret[0];
    osif_mem_free(ret);
	if(err == 0){
    	return true;
	}else{
		return false;
	}
}

bool rtw_ble_server_conn_is_any_active(void)
{
	int *ret = NULL;
	trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_CONN_IS_ANY_ACTIVE,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}

	err = ret[0];
    osif_mem_free(ret);
	if(err == 0){
    	return true;
	}else{
		return false;
	}
}

trble_result_e rtw_ble_server_disconnect(trble_conn_handle con_handle)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_conn_handle) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_DISCONNECT,
                                (uint32_t *)&con_handle, sizeof(trble_conn_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} 
	
    err = ret[0];
    osif_mem_free(ret);
	return err;

}

trble_result_e rtw_ble_server_start_adv(void)
{
	int *ret = NULL;
    trble_result_e err;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_START_ADV,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_server_stop_adv(void)
{
	int *ret = NULL;
    trble_result_e err;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_STOP_ADV,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_server_get_bonded_device(trble_bonded_device_list_s* bonded_device_list, uint16_t* device_count)
{
	int *ret = NULL;
    trble_result_e err;
	struct bond_param_s {
        trble_bonded_device_list_s bonded_device_list;
        uint16_t device_count;
    } bond_param = {0};

	memcpy(&bond_param.bonded_device_list, bonded_device_list, sizeof(trble_bonded_device_list_s));
	memcpy(&bond_param.device_count, device_count, sizeof(uint16_t));
	
	if (sizeof(bond_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_GET_BONDED_DEVICE,
                                (uint32_t *)&bond_param, sizeof(bond_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}else{
		memcpy(bonded_device_list, &ret[1], sizeof(*bonded_device_list));
		memcpy(device_count, &ret[2], sizeof(*device_count));
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_server_delete_bonded_device(uint8_t bond_addr[TRBLE_BD_ADDR_MAX_LEN])
{
	int *ret = NULL;
    trble_result_e err;
	
	if (TRBLE_BD_ADDR_MAX_LEN > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_DELETE_BONDED_DEVICE,
                                (uint32_t *)bond_addr, TRBLE_BD_ADDR_MAX_LEN);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_server_delete_bonded_device_all(void)
{
	int *ret = NULL;
    trble_result_e err;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_DELETE_BONDED_DEVICE_ALL,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

int rtw_ble_server_set_adv_interval(unsigned int interval)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(int) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SET_ADV_INTERVAL,
                                (uint32_t *)&interval, sizeof(int));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	} 
	
    err = ret[0];
    osif_mem_free(ret);

}

trble_result_e rtw_ble_server_set_adv_type(trble_adv_type_e type, trble_addr *addr)
{
	int *ret = NULL;
    trble_result_e err;
	struct type_param_s {
        trble_adv_type_e type;
        trble_addr addr;
    } type_param = {0};

	type_param.type = type;
	memcpy(&type_param.addr, addr, sizeof(trble_addr));
	
	if (sizeof(type_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_SERVER_SET_ADV_TYPE,
                                (uint32_t *)&type_param, sizeof(type_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;

}

trble_result_e rtw_ble_client_init(trble_client_init_config* init_parm)
{
	int *ret = NULL;
    trble_result_e err;

	ble_tizenrt_client_init_param = init_parm;

	if (sizeof(trble_client_init_config) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_INIT,
                                (uint32_t *)init_parm, sizeof(trble_client_init_config));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_scan_whitelist_add(trble_addr *addr)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_addr) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_SCAN_WHITELIST_ADD,
                                (uint32_t *)addr, sizeof(trble_addr));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_scan_whitelist_delete(trble_addr *addr)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_addr) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_SCAN_WHITELIST_DELETE,
                                (uint32_t *)addr, sizeof(trble_addr));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_scan_whitelist_clear_all()
{
	int *ret = NULL;
    trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_SCAN_WHITELIST_CLEAR_ALL,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_start_scan(void)
{
	int *ret = NULL;
    trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_START_SCAN,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_start_scan_with_filter(trble_scan_filter* scan_parm, bool whitelist_enable)
{
	int *ret = NULL;
    trble_result_e err;
	struct filter_param_s {
        trble_scan_filter scan_parm;
        bool whitelist_enable;
    } filter_param = {0};

	memcpy(&filter_param.scan_parm, scan_parm, sizeof(trble_scan_filter));
	filter_param.whitelist_enable = whitelist_enable;
	
	if (sizeof(filter_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_START_SCAN_WITH_FILTER,
                                (uint32_t *)&filter_param, sizeof(filter_param));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_stop_scan(void)
{
	int *ret = NULL;
    trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_STOP_SCAN,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_connect(trble_conn_info* conn_info, bool is_secured_connect)
{
	int *ret = NULL;
    trble_result_e err;
	struct conn_param_s {
        trble_conn_info conn_info;
        bool is_secured_connect;
    } conn_param = {0};

	memcpy(&conn_param.conn_info, conn_info, sizeof(trble_conn_info));
	conn_param.is_secured_connect = is_secured_connect;
	
	if (sizeof(conn_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_CONNECT,
                                (uint32_t *)&conn_param, sizeof(conn_param));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_read_connected_device_list(trble_connected_list* out_connected_list)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_connected_list) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_READ_CONNECTED_DEVICE_LIST,
                                (uint32_t *)out_connected_list, sizeof(trble_connected_list));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}else{
		memcpy(out_connected_list, &ret[1], sizeof(*out_connected_list));
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected* out_connected_device)
{
	int *ret = NULL;
    trble_result_e err;
	struct conn_param_s {
        trble_conn_handle conn_handle;
        trble_device_connected out_connected_device;
    } conn_param = {0};

	memcpy(&conn_param.out_connected_device, out_connected_device, sizeof(trble_device_connected));
	conn_param.conn_handle = conn_handle;
	
	if (sizeof(conn_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_READ_CONNECTED_INFO,
                                (uint32_t *)&conn_param, sizeof(conn_param));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}else{
		memcpy(out_connected_device, &ret[1], sizeof(*out_connected_device));
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_delete_bond(trble_addr* addr)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_addr) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_DELETE_BOND,
                                (uint32_t *)addr, sizeof(trble_addr));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_delete_bond_all(void)
{
	int *ret = NULL;
    trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_DELETE_BOND_ALL,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_disconnect(trble_conn_handle conn_handle)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_addr) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_DISCONNECTED,
                                (uint32_t *)&conn_handle, sizeof(trble_conn_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_disconnect_all(void)
{
	int *ret = NULL;
    trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_DISCONNECTED_ALL,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_operation_read(trble_operation_handle* handle, trble_data* out_data)
{
	int *ret = NULL;
    trble_result_e err;
	struct read_param_s {
        trble_operation_handle handle;
        trble_data out_data;
    } read_param = {0};

	memcpy(&read_param.handle, handle, sizeof(trble_operation_handle));
	memcpy(&read_param.out_data, out_data, sizeof(trble_data));
	
	if (sizeof(read_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_OPERATION_READ,
                                (uint32_t *)&read_param, sizeof(read_param));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}else{
		memcpy(out_data, &ret[1], sizeof(*out_data));
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_operation_write(trble_operation_handle* handle, trble_data* in_data)
{
	int *ret = NULL;
    trble_result_e err;
	struct write_param_s {
        trble_operation_handle handle;
        trble_data in_data;
    } write_param = {0};

	memcpy(&write_param.handle, handle, sizeof(trble_operation_handle));
	memcpy(&write_param.in_data, in_data, sizeof(trble_data));
	
	if (sizeof(write_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_OPERATION_WRITE,
                                (uint32_t *)&write_param, sizeof(write_param));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_operation_write_no_response(trble_operation_handle* handle, trble_data* in_data)
{
	int *ret = NULL;
    trble_result_e err;
	struct write_param_s {
        trble_operation_handle handle;
        trble_data in_data;
    } write_param = {0};

	memcpy(&write_param.handle, handle, sizeof(trble_operation_handle));
	memcpy(&write_param.in_data, in_data, sizeof(trble_data));
	
	if (sizeof(write_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_OPERATION_WRITE_NO_RESPONSE,
                                (uint32_t *)&write_param, sizeof(write_param));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_operation_enable_notification(trble_operation_handle* handle)
{
	int *ret = NULL;
    trble_result_e err;
	
	if (sizeof(trble_operation_handle) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_OPERATION_ENABLE_NOTIFICATION,
                                (uint32_t *)handle, sizeof(trble_operation_handle));
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_client_deinit(void)
{
	int *ret = NULL;
    trble_result_e err;
	
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_CLIENT_DEINIT,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
	
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_combo_init(trble_client_init_config* init_client, trble_server_init_config* init_server)
{
	int *ret = NULL;
    trble_result_e err;
	ble_tizenrt_client_init_param = init_client;
	ble_tizenrt_server_init_param = init_server;

	struct init_param_s {
        trble_client_init_config init_client;
        trble_server_init_config init_server;
    } init_param = {0};

	memcpy(&init_param.init_client, init_client, sizeof(trble_client_init_config));
	memcpy(&init_param.init_server, init_server, sizeof(trble_server_init_config));
	
	if (sizeof(init_param) > IPC_HOST_API_DATA_MAX) {
        printf("The param of %s is over ipc message memory\r\n", __func__);
        return BT_TIZENRT_FAIL;
    }
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_COMBO_INIT,
                                (uint32_t *)&init_param, sizeof(init_param));
	
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

trble_result_e rtw_ble_combo_deinit(void)
{
	int *ret = NULL;
    trble_result_e err;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_CUSTOMER, BLE_TIZENRT_ACT_COMBO_DEINIT,
                                		NULL, 0);
	if (ret[0] != BT_TIZENRT_OK) {
		printf("[core KR4][IPC] %s fail ! \r\n", __func__);
	}
    err = ret[0];
    osif_mem_free(ret);
    return err;
}

