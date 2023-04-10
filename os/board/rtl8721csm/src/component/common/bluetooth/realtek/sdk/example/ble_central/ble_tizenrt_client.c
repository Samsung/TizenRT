/*
 * ble_tizenrt_client.c
 *
 *  Created on: 2020. 7. 27.
 *      Author: lexi_zhao
 */

#ifndef TRBLE_CLIENT_C_
#define TRBLE_CLIENT_C_

#include <tinyara/net/if/ble.h>
#include "stddef.h"
#include <gap_conn_le.h>
#include "ble_tizenrt_scatternet_app.h"
#include "ble_tizenrt_scatternet_link_mgr.h"
#include "ble_tizenrt_central_client_app.h"
#include "ble_tizenrt_central_link_mgr.h"
#include "os_mem.h"
#include "os_timer.h"
#include <gcs_client.h>
#include <tinyara/net/if/ble.h>

extern T_TIZENRT_CLIENT_READ_RESULT ble_tizenrt_central_read_results[BLE_TIZENRT_CENTRAL_APP_MAX_LINKS];
extern T_GCS_WRITE_RESULT g_write_result;
extern T_GCS_WRITE_RESULT g_write_no_rsp_result;

uint8_t g_master_link_num = 0;
trble_client_init_config *client_init_parm = NULL;
T_GCS_WRITE_RESULT *write_request_result = NULL;
T_GCS_WRITE_RESULT *write_no_rsponse_result = NULL;
uint8_t ble_app_link_table_size = 0;
uint8_t ble_client_connect_is_running = 0;
BLE_TIZENRT_BOND_REQ *ble_tizenrt_bond_req_info = NULL;
T_TIZENRT_CLIENT_READ_RESULT *ble_read_results = NULL;
BLE_TIZENRT_APP_LINK *ble_app_link_table = NULL;
bool (*ble_tizenrt_client_send_msg)(uint16_t sub_type, void *arg) = NULL;
void *ble_tizenrt_modify_whitelist_sem = NULL;
uint8_t modify_whitelist_code = 1;

trble_result_e rtw_ble_client_init(trble_client_init_config* init_parm)
{ 
    if (init_parm == NULL)
    {
        return TRBLE_FAIL;
    }

    client_init_parm = os_mem_alloc(0, sizeof(trble_client_init_config));
    if(client_init_parm == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    ble_tizenrt_bond_req_info = os_mem_alloc(0, sizeof(BLE_TIZENRT_BOND_REQ));
    if(ble_tizenrt_bond_req_info == NULL)
    {
        os_mem_free(client_init_parm);
        client_init_parm = NULL;
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    
    client_init_parm->trble_scan_state_changed_cb = init_parm->trble_scan_state_changed_cb;
	client_init_parm->trble_device_scanned_cb = init_parm->trble_device_scanned_cb;
	client_init_parm->trble_device_connected_cb = init_parm->trble_device_connected_cb;
	client_init_parm->trble_device_disconnected_cb = init_parm->trble_device_disconnected_cb;
	client_init_parm->trble_operation_notification_cb = init_parm->trble_operation_notification_cb;
	client_init_parm->trble_operation_indication_cb = init_parm->trble_operation_indication_cb;

    write_request_result = &g_write_result;
    write_no_rsponse_result = &g_write_no_rsp_result;
    ble_app_link_table_size = BLE_TIZENRT_CENTRAL_APP_MAX_LINKS;
    memset(ble_tizenrt_bond_req_info, 0, sizeof(BLE_TIZENRT_BOND_REQ));
    ble_read_results = ble_tizenrt_central_read_results;
    ble_app_link_table = ble_tizenrt_central_app_link_table;
    ble_tizenrt_client_send_msg = ble_tizenrt_central_send_msg;

    ble_tizenrt_central_app_init();
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_client_scan_whitelist_add(trble_addr *addr)
{
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL || addr == NULL)
    {
        return TRBLE_FAIL;
    }

    if (ble_tizenrt_modify_whitelist_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_modify_whitelist_sem))
        {
            debug_print("create sem fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("create sem 0x%x success \n", ble_tizenrt_modify_whitelist_sem);
        }
    }

    T_TIZENRT_MODIFY_WHITELIST_PARAM *whitelist_arg = os_mem_alloc(0, sizeof(T_TIZENRT_MODIFY_WHITELIST_PARAM));
    if (whitelist_arg == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    memcpy(whitelist_arg->remote_bd, addr->mac, GAP_BD_ADDR_LEN);
    whitelist_arg->type = GAP_WHITE_LIST_OP_ADD;
    if (addr->type == TRBLE_ADDR_TYPE_PUBLIC)
    {
        whitelist_arg->remote_bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;    /* Public Address */
    } else if ((TRBLE_ADDR_TYPE_RANDOM_STATIC <= addr->type) && (TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE >= addr->type)) {
        whitelist_arg->remote_bd_type = GAP_REMOTE_ADDR_LE_RANDOM;    /* Random Address */
    } else {
        os_mem_free(whitelist_arg);
        debug_print("Unknown Address Type \n");
        return TRBLE_FAIL;
    }

    if (ble_tizenrt_client_send_msg(BLE_TIZENRT_MODIFY_WHITELIST, whitelist_arg) == false)
    {
        os_mem_free(whitelist_arg);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    if (false == os_mutex_take(ble_tizenrt_modify_whitelist_sem,3000))
    {
        debug_print("set whitelist timeout \n");
        return TRBLE_FAIL;
    }
    if (modify_whitelist_code != 0)
    {
        debug_print("set whitelist fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_scan_whitelist_delete(trble_addr *addr)
{
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL || addr == NULL)
    {
        return TRBLE_FAIL;
    }

    if (ble_tizenrt_modify_whitelist_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_modify_whitelist_sem))
        {
            debug_print("create sem fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("create sem 0x%x success \n", ble_tizenrt_modify_whitelist_sem);
        }
    }

    T_TIZENRT_MODIFY_WHITELIST_PARAM *whitelist_arg = os_mem_alloc(0, sizeof(T_TIZENRT_MODIFY_WHITELIST_PARAM));
    if (whitelist_arg == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    memcpy(whitelist_arg->remote_bd, addr->mac, GAP_BD_ADDR_LEN);
    whitelist_arg->type = GAP_WHITE_LIST_OP_REMOVE;
    if (addr->type == TRBLE_ADDR_TYPE_PUBLIC)
    {
        whitelist_arg->remote_bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;    /* Public Address */
    } else if ((TRBLE_ADDR_TYPE_RANDOM_STATIC <= addr->type) && (TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE >= addr->type)) {
        whitelist_arg->remote_bd_type = GAP_REMOTE_ADDR_LE_RANDOM;    /* Random Address */
    } else {
        os_mem_free(whitelist_arg);
        debug_print("Unknown Address Type \n");
        return TRBLE_FAIL;
    }

    if (ble_tizenrt_client_send_msg(BLE_TIZENRT_MODIFY_WHITELIST, whitelist_arg) == false)
    {
        os_mem_free(whitelist_arg);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    if (false == os_mutex_take(ble_tizenrt_modify_whitelist_sem,3000))
    {
        debug_print("set whitelist timeout \n");
        return TRBLE_FAIL;
    }
    if (modify_whitelist_code != 0)
    {
        debug_print("set whitelist fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_scan_whitelist_clear_all(void)
{
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    if (ble_tizenrt_modify_whitelist_sem == NULL)
    {
        if (false == os_mutex_create(&ble_tizenrt_modify_whitelist_sem))
        {
            debug_print("create sem fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("create sem 0x%x success \n", ble_tizenrt_modify_whitelist_sem);
        }
    }

    T_TIZENRT_MODIFY_WHITELIST_PARAM *whitelist_arg = os_mem_alloc(0, sizeof(T_TIZENRT_MODIFY_WHITELIST_PARAM));
    if (whitelist_arg == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    whitelist_arg->type = GAP_WHITE_LIST_OP_CLEAR;
    whitelist_arg->remote_bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;    /* Public Address */
    if (ble_tizenrt_client_send_msg(BLE_TIZENRT_MODIFY_WHITELIST, whitelist_arg) == false)
    {
        os_mem_free(whitelist_arg);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    if (false == os_mutex_take(ble_tizenrt_modify_whitelist_sem,3000))
    {
        debug_print("set whitelist timeout \n");
        return TRBLE_FAIL;
    }
    if (modify_whitelist_code != 0)
    {
        debug_print("set whitelist fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_start_scan(void)
{ 
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL)
    {
        return TRBLE_FAIL;
    }

	T_GAP_DEV_STATE new_state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
		return TRBLE_INVALID_STATE;
	}

    if(le_scan_info_filter(false, 0, 0, NULL))
    {
        debug_print("disable scan info filter success \n");
    } else {
        debug_print("disable scan info filter fail!!! \n");
    }

    uint8_t *scan_filter_policy = os_mem_alloc(0, sizeof(uint8_t));
    if(scan_filter_policy == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    *scan_filter_policy = GAP_SCAN_FILTER_ANY;
    if (ble_tizenrt_client_send_msg(BLE_TIZENRT_START_SCAN, scan_filter_policy) == false)
    {
        os_mem_free(scan_filter_policy);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

void *scan_filter_tmr_handle = NULL;
void scan_stop_cb(void *arg)
{
    dbg("scan duration exhausted \n");
    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_STOP_SCAN, NULL) == false)
    {
        debug_print("msg send fail \n");
        return;
    }
    T_GAP_DEV_STATE new_state;
    do {
        os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	} while(new_state.gap_scan_state != GAP_SCAN_STATE_IDLE);
}

trble_result_e rtw_ble_client_start_scan_with_filter(trble_scan_filter* scan_parm, bool whitelist_enable)
{ 
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL || scan_parm == NULL)
    {
        return TRBLE_FAIL;
    }

	T_GAP_DEV_STATE new_state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
        dbg("Scan already started!! \n");
		return TRBLE_INVALID_STATE;
	}

    if (scan_parm->raw_data_length != 0) {
        if(!le_scan_info_filter(true, 0, scan_parm->raw_data_length, scan_parm->raw_data))
        {
            debug_print("set scan info fail !! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("set scan info success \n");
        }
    } else {
        if(le_scan_info_filter(false, 0, 0, NULL))
        {
            debug_print("disable scan info filter success \n");
        } else {
            debug_print("disable scan info filter fail!!! \n");
        }
    }

    uint8_t *scan_filter_policy = os_mem_alloc(0, sizeof(uint8_t));
    if(scan_filter_policy == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    if (whitelist_enable == true) {
        *scan_filter_policy = GAP_SCAN_FILTER_WHITE_LIST;
    } else {
        *scan_filter_policy = GAP_SCAN_FILTER_ANY;
    }

    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_START_SCAN, scan_filter_policy) == false)
    {
        os_mem_free(scan_filter_policy);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    if(scan_parm->scan_duration != 0)
    {
        if(NULL == scan_filter_tmr_handle)
        {
            if(!os_timer_create(&scan_filter_tmr_handle, "scan_with_filter", 0, scan_parm->scan_duration, 0, scan_stop_cb))
            {
                dbg("timer creat fail!! \n");
                return TRBLE_FAIL;
            } else {
                debug_print("timer creat success \n");
            }

            if(!os_timer_start(&scan_filter_tmr_handle))
            {
                dbg("timer start fail!! \n");
                return TRBLE_FAIL;
            } else {
                debug_print("timer start success \n");
            }
        } else {
            if(!os_timer_restart(&scan_filter_tmr_handle, scan_parm->scan_duration))
            {
                dbg("timer restart fail!! \n");
                return TRBLE_FAIL;
            } else {
                debug_print("timer restart success \n");
            }
        }
    }

    return TRBLE_SUCCESS;  
}

trble_result_e rtw_ble_client_stop_scan(void)
{ 
	T_GAP_DEV_STATE new_state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_scan_state != GAP_SCAN_STATE_SCANNING) {
		return TRBLE_INVALID_STATE;
	}

    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_STOP_SCAN, NULL) == false)
    {
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_connect(trble_conn_info* conn_info, bool is_secured_connect)
{
    if(ble_client_connect_is_running)
    {
        dbg("ble_client_connect is running \n");
        return TRBLE_FAIL;
    } else
        ble_client_connect_is_running = 1;

    if(conn_info == NULL || g_master_link_num >= ble_app_link_table_size)
    {
        dbg("invalid \n");
        ble_client_connect_is_running = 0;
        return TRBLE_FAIL;
    }

    memcpy(ble_tizenrt_bond_req_info->addr, conn_info->addr.mac, GAP_BD_ADDR_LEN);
    ble_tizenrt_bond_req_info->is_secured_connect = is_secured_connect;

    T_TIZENRT_CONN_PARAM *conn_arg = os_mem_alloc(0, sizeof(T_TIZENRT_CONN_PARAM));
    if(conn_arg == NULL)
    {
        debug_print("Memory allocation failed \n");
        ble_client_connect_is_running = 0;
        return TRBLE_FAIL;
    }
    memcpy(conn_arg->remote_bd, conn_info->addr.mac, GAP_BD_ADDR_LEN);
    conn_arg->remote_bd_type = conn_info->addr.type;
    conn_arg->conn_interval = conn_info->conn_interval;
    conn_arg->conn_latency = conn_info->slave_latency;
    /* Defines the timeout multiplier as a multiple of 10ms. */
    conn_arg->scan_timeout = conn_info->scan_timeout / 10;
    dbg("DestAddr: 0x%02X:0x%02X:0x%02X:0x%02X:0x%02X:0x%02X \n", 
                        conn_arg->remote_bd[5], conn_arg->remote_bd[4], conn_arg->remote_bd[3],
                        conn_arg->remote_bd[2], conn_arg->remote_bd[1], conn_arg->remote_bd[0]);
    dbg("ci: %d si: %d \n", conn_info->conn_interval, conn_info->slave_latency);
    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_CONNECT, conn_arg) == false)
    {
        os_mem_free(conn_arg);
        debug_print("msg send fail \n");
        ble_client_connect_is_running = 0;
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_conn_param_update(trble_conn_handle *conn_handle, trble_conn_param *conn_param)
{
    T_TIZENRT_CLIENT_CONN_UPDATE_PARAM *update_param = os_mem_alloc(0, sizeof(T_TIZENRT_CLIENT_CONN_UPDATE_PARAM));
    if(!update_param)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    update_param->conn_id = *conn_handle;
    update_param->min_conn_interval = conn_param->min_conn_interval;
    update_param->max_conn_interval = conn_param->max_conn_interval;
    update_param->slave_latency = conn_param->slave_latency;
    update_param->supervision_timeout = conn_param->supervision_timeout;

    if(GAP_CONN_STATE_CONNECTED == ble_app_link_table[*conn_handle].conn_state)
    {
        if (ble_tizenrt_client_send_msg(BLE_TIZENRT_CONN_PARAM_UPDATE, update_param) == false)
        {
            os_mem_free(update_param);
            debug_print("msg send fail \n");
            return TRBLE_FAIL;
        }
    } else {
        debug_print("Invaild conn handle %d \n", *conn_handle);
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_client_read_connected_device_list(trble_connected_list* out_connected_list)
{ 
    if (out_connected_list == NULL)
    {
        return TRBLE_FAIL;
    }
    
    memset(out_connected_list, 0, sizeof(trble_connected_list));
    for(uint16_t i = 0; i < ble_app_link_table_size; i++)
    {
        if(ble_app_link_table[i].conn_state == GAP_CONN_STATE_CONNECTED)
        {
            out_connected_list->connected_count++;
            out_connected_list->conn_handle[i] =  ble_app_link_table[i].conn_id;
        }
    }
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected* out_connected_device)
{ 
    if (out_connected_device == NULL)
    {
        return TRBLE_FAIL;
    }

    if(ble_app_link_table[conn_handle].conn_state != GAP_CONN_STATE_CONNECTED)
        return TRBLE_INVALID_STATE;
    
    T_GAP_CONN_INFO conn_info;
    if(le_get_conn_info(conn_handle, &conn_info))
    {
        out_connected_device->conn_handle = ble_app_link_table[conn_handle].conn_id;
        out_connected_device->conn_info.addr.type = ble_app_link_table[conn_handle].remote_bd_type;
        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &out_connected_device->conn_info.conn_interval, conn_handle);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &out_connected_device->conn_info.slave_latency, conn_handle);
        le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &out_connected_device->conn_info.mtu, conn_handle);
        memcpy(out_connected_device->conn_info.addr.mac, ble_app_link_table[conn_handle].remote_bd, GAP_BD_ADDR_LEN);
        if(ble_app_link_table[conn_handle].auth_state == GAP_AUTHEN_STATE_COMPLETE)
            out_connected_device->is_bonded = true;
        else
            out_connected_device->is_bonded = false;
    } else {
        dbg("Fail to get connected info of %d \n", conn_handle);
        return TRBLE_FAIL;
    }
    
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_delete_bond(trble_addr* addr)
{
    if(NULL == addr)
    {
        dbg("Invalid Input \n");
        return TRBLE_FAIL;
    }

    T_TIZENRT_DELETE_BOND_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_DELETE_BOND_PARAM));
    if(param == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    memcpy(param->remote_bd, addr->mac, GAP_BD_ADDR_LEN);
    param->remote_bd_type = addr->type;
    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_DELETE_BOND, param) == false)
    {
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_delete_bond_all(void)
{
    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_CLEAR_ALL_BONDS, NULL) == false)
    {
        debug_print("msg send fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_client_disconnect(trble_conn_handle conn_handle)
{
    trble_conn_handle *conn_id = os_mem_alloc(0, sizeof(trble_conn_handle));
    if(conn_id == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    if(GAP_CONN_STATE_CONNECTED == ble_app_link_table[conn_handle].conn_state)
    {
        *conn_id = conn_handle;
        if(ble_tizenrt_client_send_msg(BLE_TIZENRT_DISCONNECT, conn_id) == false)
        {
            os_mem_free(conn_id);
            debug_print("msg send fail \n");
            return TRBLE_FAIL;
        }
    } else {
        debug_print("Invaild conn handle %d \n", conn_handle);
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_disconnect_all(void)
{
    trble_conn_handle *conn_id = os_mem_alloc(0, sizeof(trble_conn_handle));
    if(conn_id == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    for(uint16_t i = 0; i < ble_app_link_table_size; i++)
    {
        if(ble_app_link_table[i].conn_state == GAP_CONN_STATE_CONNECTED)
        {
            *conn_id = i;
            if(ble_tizenrt_client_send_msg(BLE_TIZENRT_DISCONNECT, conn_id) == false)
            {
                os_mem_free(conn_id);
                debug_print("msg send fail \n");
                return TRBLE_FAIL;
            }
        }
    }
    return TRBLE_SUCCESS; 
}

void *ble_tizenrt_read_sem = NULL;
trble_result_e rtw_ble_client_operation_read(trble_operation_handle* handle, trble_data* out_data)
{ 
    if(ble_tizenrt_read_sem == NULL)
    {
        if(!os_mutex_create(&ble_tizenrt_read_sem))
        {
            dbg("create mutex fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("create mutex 0x%x success \n", ble_tizenrt_read_sem);
        }
    }

    if (handle == NULL || out_data == NULL || out_data->data == NULL)
    {
        return TRBLE_FAIL;
    }

    if(!le_get_active_link_num())
    {
        debug_print("No active connection \n");
        return TRBLE_FAIL;
    }

    BLE_TIZENRT_READ_PARAM *param = os_mem_alloc(0, sizeof(BLE_TIZENRT_READ_PARAM));
    if(param == NULL)
    {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    param->conn_id = handle->conn_handle;
    param->att_handle = handle->attr_handle;
    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_READ, param) == false)
    {
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL; 
    }

	ble_read_results[handle->conn_handle].cause = 0xFF;
    int ticks = 0;
    while(ticks++ < 30)
    {
        debug_print("ticks %d \n", ticks);
        if(os_mutex_take(ble_tizenrt_read_sem, 1000))
        {  
            debug_print("take sema success \n");
            os_mutex_delete(ble_tizenrt_read_sem);
            ble_tizenrt_read_sem = NULL;
            if(ble_read_results[handle->conn_handle].cause == GAP_SUCCESS)
            {
                out_data->length = ble_read_results[handle->conn_handle].read_data.length;
                memcpy(out_data->data, ble_read_results[handle->conn_handle].read_data.data,
                                            ble_read_results[handle->conn_handle].read_data.length);
                os_mem_free(ble_read_results[handle->conn_handle].read_data.data);
                ble_read_results[handle->conn_handle].read_data.data = NULL;
                debug_print("read success: conn_id %d attr_handle 0x%x! \n",
                                                            handle->conn_handle, handle->attr_handle);
                return TRBLE_SUCCESS;
            } else {
                debug_print("read fail: conn_id %d attr_handle 0x%x! \n",
                                                            handle->conn_handle, handle->attr_handle);
                return TRBLE_FAIL;
            }
        }
    }
    return TRBLE_FAIL; 
}

void *ble_tizenrt_write_sem = NULL;
trble_result_e rtw_ble_client_operation_write(trble_operation_handle* handle, trble_data* in_data)
{
    if (handle == NULL || in_data == NULL || in_data->data == NULL)
    {
        return TRBLE_FAIL;
    }

    if(!le_get_active_link_num())
    {
        debug_print("No active connection \n");
        return TRBLE_FAIL;
    }

    if(ble_tizenrt_write_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_write_sem))
        {
            dbg("creat write mutex fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("creat write mutex 0x%x success \n", ble_tizenrt_write_sem);
        }
    }

    BLE_TIZENRT_WRITE_PARAM *param = os_mem_alloc(0, sizeof(BLE_TIZENRT_WRITE_PARAM));
    if(param == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    debug_print("att_handle 0x%x len 0x%x data \n", handle->attr_handle, in_data->length);
    param->data = in_data->data;
    param->length = in_data->length;
    param->conn_id = handle->conn_handle;
    param->att_handle = handle->attr_handle;

    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_WRITE, param) == false)
    {
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL; 
    }

	write_request_result->cause = 0xFF;
    int wticks = 0;
    while(wticks++ < 30)
    {
        debug_print("wticks %d \n", wticks);
        if(os_mutex_take(ble_tizenrt_write_sem, 1000))
        {  
            debug_print("take write mutex success \n");
            debug_print("conn_id %d att_handle 0x%x! \n", handle->conn_handle, handle->attr_handle);
            os_mutex_delete(ble_tizenrt_write_sem);
            ble_tizenrt_write_sem = NULL;
            if(write_request_result->cause == GAP_SUCCESS)
            {
                debug_print("write_req success \n");
                return TRBLE_SUCCESS;
            } else {
                debug_print("write_req fail \n");
                return TRBLE_FAIL;
            }
        }
    }
    return TRBLE_FAIL; 
}

void *ble_tizenrt_write_no_rsp_sem = NULL;
trble_result_e rtw_ble_client_operation_write_no_response(trble_operation_handle* handle, trble_data* in_data)
{
    if (handle == NULL || in_data == NULL || in_data->data == NULL)
    {
        return TRBLE_FAIL;
    }

    if(!le_get_active_link_num())
    {
        debug_print("No active connection \n");
        return TRBLE_FAIL;
    }

    if(ble_tizenrt_write_no_rsp_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_write_no_rsp_sem))
        {
            debug_print("create mutex fail! \n");
            return TRBLE_FAIL;
        } else {
            debug_print("create mutex 0x%x success \n", ble_tizenrt_write_no_rsp_sem);
        }
    }

    BLE_TIZENRT_WRITE_PARAM *param = os_mem_alloc(0, sizeof(BLE_TIZENRT_WRITE_PARAM));
    if(param == NULL)
    {
        debug_print("\n[%s] Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    debug_print("att_handle 0x%x len 0x%x data \n", handle->attr_handle, in_data->length);
    param->data = in_data->data;
    param->length = in_data->length;
    param->conn_id = handle->conn_handle;
    param->att_handle = handle->attr_handle;

    if(ble_tizenrt_client_send_msg(BLE_TIZENRT_WRITE_NO_RSP, param) == false)
    {
        os_mem_free(param);
        debug_print("msg send fail \n");
        return TRBLE_FAIL; 
    }

	write_no_rsponse_result->cause = 0xFF;
    int wticks = 0;
    while(wticks++ < 30)
    {
        debug_print("wticks %d \n", wticks);
        if(os_mutex_take(ble_tizenrt_write_no_rsp_sem, 1000))
        {
            debug_print("take write_no_rsp mutex success \n");
            debug_print("conn_id %d att_handle 0x%x! \n", handle->conn_handle, handle->attr_handle);
            os_mutex_delete(ble_tizenrt_write_no_rsp_sem);
            ble_tizenrt_write_no_rsp_sem = NULL;
            if(write_no_rsponse_result->cause == GAP_SUCCESS)
            {
                debug_print("send write_cmd success \n");
                return TRBLE_SUCCESS;
            } else {
                debug_print("send write_cmd fail \n");
                return TRBLE_FAIL;
            }
        }
    }
    return TRBLE_FAIL;
}

trble_result_e rtw_ble_client_operation_enable_notification(trble_operation_handle* handle)
{ 
    if (handle == NULL)
    {
        return TRBLE_FAIL;
    }

    if(!le_get_active_link_num())
    {
        debug_print("No active connection \n");
        return TRBLE_FAIL;
    }

    if (client_init_parm == NULL || client_init_parm->trble_operation_notification_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    uint8_t val[2] = {0x1, 0x0};
    trble_data in_data;
    in_data.length = 2;
    in_data.data = val;

    return rtw_ble_client_operation_write(handle, &in_data);
}

trble_result_e rtw_ble_client_operation_enable_indication(trble_operation_handle* handle)
{
    if (handle == NULL)
    {
        return TRBLE_FAIL;
    }

    if(!le_get_active_link_num())
    {
        debug_print("No active connection \n");
        return TRBLE_FAIL;
    }

    if (client_init_parm == NULL || client_init_parm->trble_operation_notification_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    uint8_t val[2] = {0x2, 0x0};
    trble_data in_data;
    in_data.length = 2;
    in_data.data = val;

    return rtw_ble_client_operation_write(handle, &in_data);
}

trble_result_e rtw_ble_client_operation_enable_notification_and_indication(trble_operation_handle* handle)
{
    if (handle == NULL)
    {
        return TRBLE_FAIL;
    }

    if(!le_get_active_link_num())
    {
        debug_print("No active connection \n");
        return TRBLE_FAIL;
    }

    if (client_init_parm == NULL || client_init_parm->trble_operation_notification_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    uint8_t val[2] = {0x3, 0x0};
    trble_data in_data;
    in_data.length = 2;
    in_data.data = val;

    return rtw_ble_client_operation_write(handle, &in_data);
}

trble_result_e rtw_ble_client_deinit(void)
{
    ble_tizenrt_central_app_deinit();

    g_master_link_num = 0;
    os_mem_free(client_init_parm);
    client_init_parm = NULL;
    os_mem_free(ble_tizenrt_bond_req_info);
    ble_tizenrt_bond_req_info = NULL;

    return TRBLE_SUCCESS; 
}
#endif /* TRBLE_CLIENT_C_ */
