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
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gattc.h>
#include <gap_msg.h>
#include <gap.h>
#include <mem_types.h>
#include <bt_utils.h>
#include <rtk_client_config.h>
#include <rtk_stack_config.h>

extern rtk_bt_gattc_read_ind_t ble_tizenrt_read_result[GAP_MAX_LINKS];
extern rtk_bt_gattc_write_ind_t g_write_result;
extern rtk_bt_gattc_write_ind_t g_write_no_rsp_result;

trble_client_init_config *client_init_parm = NULL;
uint8_t ble_client_connect_is_running = 0;
rtk_bt_gattc_read_ind_t *ble_read_results = NULL;
rtk_bt_gattc_write_ind_t *ble_write_request_result = NULL;
rtk_bt_gattc_write_ind_t *ble_write_no_rsp_result = NULL;
rtk_bt_le_conn_ind_t *ble_tizenrt_conn_ind = NULL;
bool is_secured = false;
uint16_t scan_timeout = 0;
static uint16_t general_profile_id = GCS_CLIENT_PROFILE_ID;

trble_result_e rtw_ble_client_init(trble_client_init_config* init_parm)
{ 
    if (init_parm == NULL)
    {
        return TRBLE_FAIL;
    }

    client_init_parm = (trble_client_init_config *)osif_mem_alloc(0, sizeof(trble_client_init_config));
	ble_tizenrt_conn_ind = (rtk_bt_le_conn_ind_t *)osif_mem_alloc(0, sizeof(rtk_bt_le_conn_ind_t));
	
    if(client_init_parm == NULL)
    {
//        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    
    client_init_parm->trble_scan_state_changed_cb = init_parm->trble_scan_state_changed_cb;
	client_init_parm->trble_device_scanned_cb = init_parm->trble_device_scanned_cb;
	client_init_parm->trble_device_connected_cb = init_parm->trble_device_connected_cb;
	client_init_parm->trble_device_disconnected_cb = init_parm->trble_device_disconnected_cb;
	client_init_parm->trble_operation_notification_cb = init_parm->trble_operation_notification_cb;

	ble_read_results = ble_tizenrt_read_result;
	ble_write_request_result = &g_write_result;
	ble_write_no_rsp_result = &g_write_no_rsp_result;
	
    ble_tizenrt_central_main(1);
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_client_scan_whitelist_add(trble_addr *addr)
{
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL || addr == NULL)
    {
        return TRBLE_FAIL;
    }

    rtk_bt_le_modify_wl_param_t op_param;
    memcpy(op_param.addr.addr_val, addr->mac, GAP_BD_ADDR_LEN);
    op_param.op = RTK_BT_LE_WHITELIST_ADD;
    if (addr->type == TRBLE_ADDR_TYPE_PUBLIC)
    {
        op_param.addr.type = GAP_REMOTE_ADDR_LE_PUBLIC;    /* Public Address */
    } else if ((TRBLE_ADDR_TYPE_RANDOM_STATIC <= addr->type) && (TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE >= addr->type)) {
        op_param.addr.type = GAP_REMOTE_ADDR_LE_RANDOM;    /* Random Address */
    } else {
//        debug_print("Unknown Address Type \n");
        return TRBLE_FAIL;
    }

    if (RTK_BT_FAIL == rtk_bt_le_gap_modify_whitelist(&op_param))
    {
//        debug_print("modify whitelist fail \n");
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

    rtk_bt_le_modify_wl_param_t op_param;
    memcpy(op_param.addr.addr_val, addr->mac, GAP_BD_ADDR_LEN);
    op_param.op = RTK_BT_LE_WHITELIST_REMOVE;
    if (addr->type == TRBLE_ADDR_TYPE_PUBLIC)
    {
        op_param.addr.type = GAP_REMOTE_ADDR_LE_PUBLIC;    /* Public Address */
    } else if ((TRBLE_ADDR_TYPE_RANDOM_STATIC <= addr->type) && (TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE >= addr->type)) {
        op_param.addr.type = GAP_REMOTE_ADDR_LE_RANDOM;    /* Random Address */
    } else {
//        debug_print("Unknown Address Type \n");
        return TRBLE_FAIL;
    }


    if (RTK_BT_FAIL == rtk_bt_le_gap_modify_whitelist(&op_param))
    {
//        debug_print("modify whitelist fail \n");
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

    rtk_bt_le_modify_wl_param_t op_param = {0};
    op_param.op = RTK_BT_LE_WHITELIST_CLEAR;

    if (RTK_BT_OK != rtk_bt_le_gap_modify_whitelist(&op_param))
    {
//        debug_print("whitelist clear failed!");
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

    rtk_bt_le_gap_dev_state_t new_state;
    if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
    {
//		debug_print("get dev state fail \n");
    }

	if (new_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
		return TRBLE_INVALID_STATE;
	}
	
    if (RTK_BT_FAIL == rtk_bt_le_gap_start_scan())
    {
// 	   debug_print("start scan failed! \n");
 	   return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

void *scan_filter_tmr_handle = NULL;
void scan_stop_cb(void *arg)
{
    //dbg("scan duration exhausted \n");
    if (RTK_BT_OK != rtk_bt_le_gap_stop_scan())
    {
// 	   debug_print("stop scan failed! \n");
       return;
    }
    rtk_bt_le_gap_dev_state_t new_state;
    do {
        osif_delay(100);
        rtk_bt_le_gap_get_dev_state(&new_state);
	} while(new_state.gap_scan_state != GAP_SCAN_STATE_IDLE);
}

trble_result_e rtw_ble_client_start_scan_with_filter(trble_scan_filter* scan_parm, bool whitelist_enable)
{ 
    if (client_init_parm == NULL || client_init_parm->trble_device_scanned_cb == NULL || scan_parm == NULL)
    {
        return TRBLE_FAIL;
    }

    rtk_bt_le_gap_dev_state_t new_state;
    if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
    {
//		debug_print("get dev state fail \n");
    }

	if (new_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
//        dbg("Scan already started!! \n");
		return TRBLE_INVALID_STATE;
	}
	rtk_bt_le_scan_info_filter_param_t scan_info;
    if (scan_parm->raw_data_length != 0) {
		scan_info.len = scan_parm->raw_data_length;
		scan_info.enable = true;
		scan_info.p_filter = scan_parm->raw_data;
		scan_info.offset = 0;
        if(RTK_BT_OK != rtk_bt_le_gap_scan_info_filter(&scan_info))
        {
            printf("set scan info fail !! \n");
            return TRBLE_FAIL;
        } else {
//            debug_print("set scan info success \n");
        }
    } else {
    	scan_info.enable = false;
		scan_info.len = 0;
		scan_info.p_filter = NULL;
		scan_info.offset = 0;
        if(RTK_BT_OK == rtk_bt_le_gap_scan_info_filter(&scan_info))
        {
//            debug_print("disable scan info filter success \n");
        } else {
//            debug_print("disable scan info filter fail!!! \n");
        }
    }

	if(whitelist_enable){
		rtk_bt_le_scan_param_t gap_scan_param;
		gap_scan_param.type = RTK_BT_LE_SCAN_TYPE_PASSIVE;
		gap_scan_param.interval = 0x60;
		gap_scan_param.window = 0x30;
		gap_scan_param.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC;
		gap_scan_param.filter_policy = RTK_BT_LE_SCAN_FILTER_ALLOW_ONLY_WLST;
		gap_scan_param.duplicate_opt = RTK_BT_LE_SCAN_DUPLICATE_DISABLE;
        if(RTK_BT_OK != rtk_bt_le_gap_set_scan_param(&gap_scan_param))
        {
//            debug_print("set scan param fail \n");
        }
	}
    if (RTK_BT_OK != rtk_bt_le_gap_start_scan())
    {
// 	   debug_print("start scan failed! \n");
 	   return TRBLE_FAIL;
    }

    if(scan_parm->scan_duration != 0)
    {
        if(NULL == scan_filter_tmr_handle)
        {
            if(!osif_timer_create(&scan_filter_tmr_handle, "scan_with_filter", 0, scan_parm->scan_duration, 0, scan_stop_cb))
            {
//                dbg("timer creat fail!! \n");
                return TRBLE_FAIL;
            } else {
//                debug_print("timer creat success \n");
            }

            if(!osif_timer_start(&scan_filter_tmr_handle))
            {
//                dbg("timer start fail!! \n");
                return TRBLE_FAIL;
            } else {
//                debug_print("timer start success \n");
            }
        } else {
            if(!osif_timer_restart(&scan_filter_tmr_handle, scan_parm->scan_duration))
            {
//                dbg("timer restart fail!! \n");
                return TRBLE_FAIL;
            } else {
//                debug_print("timer restart success \n");
            }
        }
    }
    return TRBLE_SUCCESS;  
}

trble_result_e rtw_ble_client_stop_scan(void)
{ 
    rtk_bt_le_gap_dev_state_t new_state;
    if(RTK_BT_OK != rtk_bt_le_gap_get_dev_state(&new_state))
    {
//		debug_print("get dev state fail \n");
    }

	if (new_state.gap_scan_state != GAP_SCAN_STATE_SCANNING) {
		return TRBLE_INVALID_STATE;
	}

    if (RTK_BT_OK != rtk_bt_le_gap_stop_scan())
    {
// 	   debug_print("stop scan failed! \n");
 	   return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_connect(trble_conn_info* conn_info, bool is_secured_connect)
{
    if(ble_client_connect_is_running)
    {
//        dbg("ble_client_connect is running \n");
        return TRBLE_FAIL;
    } else
        ble_client_connect_is_running = 1;

    if(conn_info == NULL)
    {
//        dbg("invalid \n");
        ble_client_connect_is_running = 0;
        return TRBLE_FAIL;
    }

	is_secured = is_secured_connect;
	
    rtk_bt_le_create_conn_param_t conn_param;
    memcpy(conn_param.peer_addr.addr_val, conn_info->addr.mac, GAP_BD_ADDR_LEN);
    conn_param.peer_addr.type = conn_info->addr.type;
    conn_param.conn_interval_max = conn_info->conn_interval;
	conn_param.conn_interval_min = conn_info->conn_interval;
    conn_param.conn_latency = conn_info->slave_latency;
    conn_param.supv_timeout = 1000;
	conn_param.scan_timeout = conn_info->scan_timeout / 10;
	scan_timeout = conn_param.scan_timeout;
	conn_param.own_addr_type = 0;
	conn_param.scan_interval = 0x60;
	conn_param.scan_window = 0x30;
	conn_param.filter_policy = RTK_BT_LE_CONN_FILTER_WITHOUT_WHITELIST;
//    dbg("DestAddr: 0x%02X:0x%02X:0x%02X:0x%02X:0x%02X:0x%02X \n", 
//                        conn_param.peer_addr.addr_val[5], conn_param.peer_addr.addr_val[4], conn_param.peer_addr.addr_val[3],
//                        conn_param.peer_addr.addr_val[2], conn_param.peer_addr.addr_val[1], conn_param.peer_addr.addr_val[0]);
//    dbg("ci: %d si: %d \n", conn_info->conn_interval, conn_info->slave_latency);
    if (RTK_BT_OK != rtk_bt_le_gap_connect(&conn_param))
    {
        if(ble_client_connect_is_running)
    		ble_client_connect_is_running = 0;

// 	   debug_print("connect failed! \n");
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
	uint16_t ret = 0;
	rtk_bt_le_get_active_conn_t active_conn = {0};
	ret = rtk_bt_le_gap_get_active_conn(&active_conn);
    if (ret) {
//        debug_print("GAP get active conn num failed! err: 0x%x", ret);
        return TRBLE_FAIL;
    }
    for(uint16_t i = 0; i < active_conn.conn_num; i++)
    {
        out_connected_list->conn_handle[i] = active_conn.conn_handle[i];
    }
	out_connected_list->connected_count = active_conn.conn_num;
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected* out_connected_device)
{ 
    if (out_connected_device == NULL)
    {
        return TRBLE_FAIL;
    }

	rtk_bt_le_conn_info_t conn_info;
	uint8_t bond_size;
	bool is_bonded = false;
	uint16_t mtu_size = 0;
	
	if (RTK_BT_OK != rtk_bt_le_sm_get_bond_num(&bond_size))
    {
//        debug_print("get bond number failed! \n");
        return TRBLE_FAIL;
    }

	if (RTK_BT_OK != rtk_bt_le_gap_get_conn_info((uint8_t)conn_handle, &conn_info))
    {
// 	   debug_print("get connenct info failed! \n");
 	   return TRBLE_FAIL;
    } 

	if (RTK_BT_OK != rtk_bt_le_gap_get_mtu_size(conn_handle, &mtu_size))
    {
// 	   debug_print("get mtu size failed! \n");
 	   return TRBLE_FAIL;
    }

    rtk_bt_le_bond_info_t* bond_info = (rtk_bt_le_bond_info_t*)osif_mem_alloc(RAM_TYPE_DATA_ON, bond_size * sizeof(rtk_bt_le_bond_info_t));
	if(!bond_info){
//		debug_print("bond info malloc failed! \n");
		return TRBLE_FAIL;
	}
    memset(bond_info, 0, bond_size * sizeof(rtk_bt_le_bond_info_t));
	if (RTK_BT_OK != rtk_bt_le_sm_get_bond_info(bond_info, &bond_size))
    {
    	osif_mem_free(bond_info);
//        debug_print("get bond info failed! \n");
        return TRBLE_FAIL;
    }
		
    out_connected_device->conn_handle = conn_handle;
    out_connected_device->conn_info.addr.type = conn_info.remote.type;
	out_connected_device->conn_info.conn_interval = conn_info.interval;
    out_connected_device->conn_info.slave_latency = conn_info.latency;
    out_connected_device->conn_info.mtu = mtu_size;
    memcpy(out_connected_device->conn_info.addr.mac, conn_info.remote.addr_val, GAP_BD_ADDR_LEN);

	for(int i = 0; i < bond_size; i++){
		if(!memcmp(conn_info.remote.addr_val, bond_info[i].remote_addr.addr_val, GAP_BD_ADDR_LEN)){
			is_bonded = true;
			break;
		}		
	}
    out_connected_device->is_bonded = is_bonded;

    if (bond_info)
        osif_mem_free(bond_info);

    return TRBLE_SUCCESS;
}

uint8_t *del_bond_addr = NULL;
trble_result_e rtw_ble_client_delete_bond(trble_addr* addr)
{
    if(addr == NULL)
    {
//        debug_print("Invalid input \n");
        return TRBLE_INVALID_ARGS;
    }
	
	uint16_t device_count;
    trble_result_e ret = TRBLE_FAIL;
    rtk_bt_le_addr_t del_addr;
	bool bond_addr_found = false;
	
	del_bond_addr = addr->mac;
    if(RTK_BT_OK != rtk_bt_le_sm_get_bond_num((uint8_t *)&device_count)){
//		debug_print("get bond num failed \n");
    }
//    debug_print("bonded num : %d \n", device_count);

	if (!device_count)
    {
//        debug_print("no bond device! \n");
        return TRBLE_FAIL;
    }

    rtk_bt_le_bond_info_t* bond_info = (rtk_bt_le_bond_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, device_count * sizeof(rtk_bt_le_bond_info_t));
	if(!bond_info){
//		debug_print("bond info malloc failed \n");
		return TRBLE_FAIL;
	}
    memset(bond_info, 0, device_count * sizeof(rtk_bt_le_bond_info_t));
	if(RTK_BT_OK != rtk_bt_le_sm_get_bond_info(bond_info, (uint8_t *)&device_count)){
//		debug_print("get bond info failed \n");
		osif_mem_free(bond_info);
		return TRBLE_FAIL;
	}

	for(int i = 0; i < (device_count); i++)
	{
		if(!memcmp(bond_info[i].remote_addr.addr_val, addr->mac, TRBLE_BD_ADDR_MAX_LEN)){
			del_addr.type = bond_info[i].remote_addr.type;
			bond_addr_found = true;
			break;
		}
	}
	if(!bond_addr_found)
	{
//		debug_print("bond addr not found! \n");
		osif_mem_free(bond_info);
		return TRBLE_FAIL;
	}
	
	memcpy(del_addr.addr_val, addr->mac, TRBLE_BD_ADDR_MAX_LEN);
	
    if(RTK_BT_OK == rtk_bt_le_sm_delete_bond_device(&del_addr))
    {
//        debug_print("success \n");
        ret = TRBLE_SUCCESS;
    }else{
//        debug_print("delete bonded device failed \n");
    }

	if (bond_info)
        osif_mem_free(bond_info);
	
    return ret;

}

trble_result_e rtw_ble_client_delete_bond_all(void)
{
    if(RTK_BT_OK != rtk_bt_le_sm_clear_bond_list())
    {
//        debug_print("delete bonded device all fail \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_disconnect(trble_conn_handle conn_handle)
{
    if (RTK_BT_OK != rtk_bt_le_gap_disconnect(conn_handle))
    {
//        debug_print("disconnect failed! \n");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_disconnect_all(void)
{
	uint16_t ret = 0;
	rtk_bt_le_get_active_conn_t active_conn = {0};
	ret = rtk_bt_le_gap_get_active_conn(&active_conn);
    if (ret) {
//        debug_print("GAP get active conn num failed! err: 0x%x", ret);
        return TRBLE_FAIL;
    }
	
    for(uint16_t i = 0; i < active_conn.conn_num; i++)
    {
        if (RTK_BT_OK != rtk_bt_le_gap_disconnect(active_conn.conn_handle[i]))
	    {
//	        debug_print("conn id %d disconnect failed! \n",active_conn.conn_handle[i]);
	        return TRBLE_FAIL;
	    }
    }
    return TRBLE_SUCCESS; 
}

void *ble_tizenrt_read_sem = NULL;
trble_result_e rtw_ble_client_operation_read(trble_operation_handle* handle, trble_data* out_data)
{ 
    if (handle == NULL || out_data == NULL || out_data->data == NULL)
    {
        return TRBLE_FAIL;
    }

	rtk_bt_le_get_active_conn_t active_conn = {0};
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
//        debug_print("get active connection failed! \n");
        return TRBLE_FAIL;
    }
	if(!active_conn.conn_num){
//		debug_print("No active connection \r\n");
		return TRBLE_FAIL;
	}

    rtk_bt_gattc_read_param_t read_param;
	read_param.profile_id = general_profile_id;
    read_param.conn_handle = handle->conn_handle;
    read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
	read_param.by_handle.handle = handle->attr_handle;
    if (RTK_BT_OK != rtk_bt_gattc_read(&read_param))
    {
//        debug_print("read failed! \n");
        return TRBLE_FAIL;
    }
	return TRBLE_SUCCESS;
}

void *ble_tizenrt_write_sem = NULL;
trble_result_e rtw_ble_client_operation_write(trble_operation_handle* handle, trble_data* in_data)
{
    if (handle == NULL || in_data == NULL || in_data->data == NULL)
    {
        return TRBLE_FAIL;
    }

	rtk_bt_le_get_active_conn_t active_conn = {0};
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
//        debug_print("get active connection failed! \n");
        return TRBLE_FAIL;
    }
	if(!active_conn.conn_num){
//		debug_print("No active connection \r\n");
		return TRBLE_FAIL;
	}


    if(ble_tizenrt_write_sem == NULL)
    {
        if(false == osif_sem_create(&ble_tizenrt_write_sem, 0, 1))
        {
//            dbg("creat write sema fail! \n");
            return TRBLE_FAIL;
        } else {
//            debug_print("creat write sema 0x%x success \n", ble_tizenrt_write_sem);
        }
    }

    rtk_bt_gattc_write_param_t write_param;;
//    debug_print("att_handle 0x%x len 0x%x data \n", handle->attr_handle, in_data->length);
	write_param.profile_id = general_profile_id;
    write_param.length = in_data->length;
    write_param.conn_handle = handle->conn_handle;
    write_param.handle = handle->attr_handle;
	write_param.type = RTK_BT_GATT_CHAR_WRITE_REQ;
	write_param.data = in_data->data;
	
	if(!write_param.data){
//		debug_print("write failed: cannot alloc memory \n");
	}
	
    ble_write_request_result->status = 0xff;
    if (RTK_BT_FAIL == rtk_bt_gattc_write(&write_param))
    {
        if (write_param.data)
            osif_mem_free((void*)write_param.data);	
//        debug_print("write failed! \n");
        return TRBLE_FAIL;
    }
    
    return TRBLE_SUCCESS;
}

void *ble_tizenrt_write_no_rsp_sem = NULL;
trble_result_e rtw_ble_client_operation_write_no_response(trble_operation_handle* handle, trble_data* in_data)
{
    if (handle == NULL || in_data == NULL || in_data->data == NULL)
    {
        return TRBLE_FAIL;
    }

	rtk_bt_le_get_active_conn_t active_conn = {0};
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
//        debug_print("get active connection failed! \n");
        return TRBLE_FAIL;
    }
	if(!active_conn.conn_num){
//		debug_print("No active connection \r\n");
		return TRBLE_FAIL;
	}


    if(ble_tizenrt_write_no_rsp_sem == NULL)
    {
        if(false == osif_sem_create(&ble_tizenrt_write_no_rsp_sem, 0, 1))
        {
//            debug_print("create sema fail! \n");
            return TRBLE_FAIL;
        } else {
//            debug_print("create sema 0x%x success \n", ble_tizenrt_write_no_rsp_sem);
        }
    }

    rtk_bt_gattc_write_param_t write_param;;
//    debug_print("att_handle 0x%x len 0x%x data \n", handle->attr_handle, in_data->length);
	write_param.profile_id = general_profile_id;
    write_param.length = in_data->length;
    write_param.conn_handle = handle->conn_handle;
    write_param.handle = handle->attr_handle;
	write_param.type = RTK_BT_GATT_CHAR_WRITE_NO_RSP;
	write_param.data = in_data->data;
	
	if(!write_param.data){
//		debug_print("write failed: cannot alloc memory \n");
	}

    ble_write_no_rsp_result->status = 0xff;
    if (RTK_BT_FAIL == rtk_bt_gattc_write(&write_param))
    {
//        debug_print("read failed! \n");
        return TRBLE_FAIL;
    }


    int wticks = 0;
    while(wticks++ < 30)
    {
//        debug_print("wticks %d \n", wticks);
        if(osif_sem_take(ble_tizenrt_write_no_rsp_sem, 1000))
        {
//            debug_print("take write_no_rsp sema success \n");
//            debug_print("conn_id %d att_handle 0x%x! \n", handle->conn_handle, handle->attr_handle);
            osif_sem_delete(ble_tizenrt_write_no_rsp_sem);
            ble_tizenrt_write_no_rsp_sem = NULL;
            if(ble_write_no_rsp_result->status == RTK_BT_STATUS_DONE)
            {
//                debug_print("send write_cmd success \n");
                return TRBLE_SUCCESS;
            } else {
//                debug_print("send write_cmd fail \n");
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

	rtk_bt_le_get_active_conn_t active_conn = {0};
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
//        debug_print("get active connection failed! \n");
        return TRBLE_FAIL;
    }
	if(!active_conn.conn_num){
//		debug_print("No active connection \r\n");
		return TRBLE_FAIL;
	}


    if (client_init_parm == NULL || client_init_parm->trble_operation_notification_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    rtk_bt_gattc_update_cccd_param_t p_update_cccd_param = {0};
	p_update_cccd_param.conn_handle = handle->conn_handle;
	p_update_cccd_param.profile_id = general_profile_id;
	p_update_cccd_param.cccd_handle = handle->attr_handle;
	p_update_cccd_param.char_val_handle = handle->attr_handle - 1;
	p_update_cccd_param.bnotify = true;

	if (RTK_BT_OK != rtk_bt_gattc_enable_notify_or_indicate(&p_update_cccd_param))
    {
//        debug_print("enable notify failed! \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_operation_enable_indication(trble_operation_handle* handle)
{ 
    if (handle == NULL)
    {
        return TRBLE_FAIL;
    }

	rtk_bt_le_get_active_conn_t active_conn = {0};
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
//        debug_print("get active connection failed! \n");
        return TRBLE_FAIL;
    }
	if(!active_conn.conn_num){
//		debug_print("No active connection \r\n");
		return TRBLE_FAIL;
	}


    if (client_init_parm == NULL || client_init_parm->trble_operation_notification_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    rtk_bt_gattc_update_cccd_param_t p_update_cccd_param = {0};
	p_update_cccd_param.conn_handle = handle->conn_handle;
	p_update_cccd_param.profile_id = general_profile_id;
	p_update_cccd_param.cccd_handle = handle->attr_handle;
	p_update_cccd_param.char_val_handle = handle->attr_handle - 1;
	p_update_cccd_param.bindicate = true;

	if (RTK_BT_OK != rtk_bt_gattc_enable_notify_or_indicate(&p_update_cccd_param))
    {
//        debug_print("enable notify failed! \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e rtw_ble_client_operation_enable_notification_and_indication(trble_operation_handle* handle)
{ 
    if (handle == NULL)
    {
        return TRBLE_FAIL;
    }

	rtk_bt_le_get_active_conn_t active_conn = {0};
    if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
    {
//        debug_print("get active connection failed! \n");
        return TRBLE_FAIL;
    }
	if(!active_conn.conn_num){
//		debug_print("No active connection \r\n");
		return TRBLE_FAIL;
	}


    if (client_init_parm == NULL || client_init_parm->trble_operation_notification_cb == NULL)
    {
        return TRBLE_FAIL;
    }

    rtk_bt_gattc_update_cccd_param_t p_update_cccd_param = {0};
	p_update_cccd_param.conn_handle = handle->conn_handle;
	p_update_cccd_param.profile_id = general_profile_id;
	p_update_cccd_param.cccd_handle = handle->attr_handle;
	p_update_cccd_param.char_val_handle = handle->attr_handle - 1;
	p_update_cccd_param.bnotify = true;
	p_update_cccd_param.bindicate = true;

	if (RTK_BT_OK != rtk_bt_gattc_enable_notify_or_indicate(&p_update_cccd_param))
    {
//        debug_print("enable notify failed! \n");
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}


trble_result_e rtw_ble_client_deinit(void)
{
    ble_tizenrt_central_main(0);

    osif_mem_free(client_init_parm);
    client_init_parm = NULL;
    osif_mem_free(ble_tizenrt_conn_ind);
	ble_tizenrt_conn_ind = NULL;

    return TRBLE_SUCCESS; 
}

#endif /* TRBLE_CLIENT_C_ */
