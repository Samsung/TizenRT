/*
 * ble_tizenrt_da_client.c
 *
 *  Created on: 2020. 7. 27.
 *      Author: lexi_zhao
 */

#ifndef DA_BLE_CLIENT_C_
#define DA_BLE_CLIENT_C_

#include "da_ble_client.h"
#include "stddef.h"
#include <gap_conn_le.h>
#include "ble_tizenrt_scatternet_app.h"
#include "ble_tizenrt_scatternet_link_mgr.h"
#include "ble_tizenrt_central_client_app.h"
#include "ble_tizenrt_central_link_mgr.h"
#include "os_mem.h"
#include "os_timer.h"
#include <gcs_client.h>

extern T_TIZENRT_CLIENT_READ_RESULT ble_tizenrt_central_read_results[BLE_TIZENRT_CENTRAL_APP_MAX_LINKS];
extern T_GCS_WRITE_RESULT g_write_result;
extern T_GCS_WRITE_RESULT g_write_no_rsp_result;

uint16_t g_conn_req_num = 0;
da_ble_client_init_parm *client_init_parm = NULL;
T_GCS_WRITE_RESULT *write_request_result = NULL;
T_GCS_WRITE_RESULT *write_no_rsponse_result = NULL;
uint8_t ble_app_link_table_size = 0;
BLE_TIZENRT_BOND_REQ *ble_tizenrt_bond_req_table = NULL;
T_TIZENRT_CLIENT_READ_RESULT *ble_read_results = NULL;
BLE_TIZENRT_APP_LINK *da_ble_app_link_table = NULL;
void (*ble_tizenrt_client_send_msg)(uint16_t sub_type, void *arg) = NULL;

da_ble_result_type rtw_ble_client_init(da_ble_client_init_parm* init_parm)
{ 
    if (init_parm == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    client_init_parm = os_mem_alloc(0, sizeof(da_ble_client_init_parm));
    client_init_parm->da_ble_client_scan_state_changed_cb = init_parm->da_ble_client_scan_state_changed_cb;
	client_init_parm->da_ble_client_device_scanned_cb = init_parm->da_ble_client_device_scanned_cb;
	client_init_parm->da_ble_client_device_connected_cb = init_parm->da_ble_client_device_connected_cb;
	client_init_parm->da_ble_client_device_disconnected_cb = init_parm->da_ble_client_device_disconnected_cb;
	client_init_parm->da_ble_client_operation_notification_cb = init_parm->da_ble_client_operation_notification_cb;

    write_request_result = &g_write_result;
    write_no_rsponse_result = &g_write_no_rsp_result;
    ble_app_link_table_size = BLE_TIZENRT_CENTRAL_APP_MAX_LINKS;
    ble_tizenrt_bond_req_table = os_mem_alloc(0, BLE_TIZENRT_CENTRAL_APP_MAX_LINKS * sizeof(BLE_TIZENRT_BOND_REQ));
    memset(ble_tizenrt_bond_req_table, 0, BLE_TIZENRT_CENTRAL_APP_MAX_LINKS * sizeof(BLE_TIZENRT_BOND_REQ));
    ble_read_results = ble_tizenrt_central_read_results;
    da_ble_app_link_table = ble_tizenrt_central_app_link_table;
    ble_tizenrt_client_send_msg = ble_tizenrt_central_send_msg;

    if(ble_tizenrt_central_app_init())
        return DA_BLE_RESULT_TYPE_FAILURE;
    else
        return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_client_start_scan(void)
{ 
    if (client_init_parm == NULL || client_init_parm->da_ble_client_device_scanned_cb == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

	T_GAP_DEV_STATE new_state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
		return DA_BLE_RESULT_TYPE_INVALID_STATE;
	}

    if(le_scan_info_filter(false, 0, 0, NULL))
    {
        debug_print("\r\n[%s] disable scan info filter success", __FUNCTION__);
    } else {
        debug_print("\r\n[%s] disable scan info filter fail!!!", __FUNCTION__);
    }

    ble_tizenrt_client_send_msg(BLE_TIZENRT_START_SCAN, NULL);
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

void *scan_filter_tmr_handle = NULL;
void scan_stop_cb(void *arg)
{
    printf("\r\n[%s] scan duration exhausted", __FUNCTION__);
    ble_tizenrt_client_send_msg(BLE_TIZENRT_STOP_SCAN, NULL);
    T_GAP_DEV_STATE new_state;
    do {
        os_delay(100);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	} while(new_state.gap_scan_state != GAP_SCAN_STATE_IDLE);
}

da_ble_result_type rtw_ble_client_start_scan_with_filter(da_ble_client_scan_filter_parm* scan_parm)
{ 
    if (client_init_parm == NULL || client_init_parm->da_ble_client_device_scanned_cb == NULL || scan_parm == NULL || scan_parm->raw_data_length == 0)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

	T_GAP_DEV_STATE new_state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_scan_state != GAP_SCAN_STATE_IDLE) {
        printf("\r\n[%s] Scan already started !!", __FUNCTION__);
		return DA_BLE_RESULT_TYPE_INVALID_STATE;
	}

    if(!le_scan_info_filter(true, 0, scan_parm->raw_data_length, scan_parm->raw_data))
    {
        printf("\r\n[%s] set scan info fail !!", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    } else {
        debug_print("\r\n[%s] set scan info success", __FUNCTION__);
    }
    
    ble_tizenrt_client_send_msg(BLE_TIZENRT_START_SCAN, NULL);
    if(scan_parm->scan_duration != 0)
    {
        if(NULL == scan_filter_tmr_handle)
        {
            if(!os_timer_create(&scan_filter_tmr_handle, "scan_with_filter", 0, scan_parm->scan_duration, 0, scan_stop_cb))
            {
                printf("\r\n[%s] timer creat fail !!", __FUNCTION__);
                return DA_BLE_RESULT_TYPE_FAILURE;
            } else {
                debug_print("\r\n[%s] timer creat success", __FUNCTION__);
            }

            if(!os_timer_start(&scan_filter_tmr_handle))
            {
                printf("\r\n[%s] timer start fail !!", __FUNCTION__);
                return DA_BLE_RESULT_TYPE_FAILURE;
            } else {
                debug_print("\r\n[%s] timer start success", __FUNCTION__);
            }
        } else {
            if(!os_timer_restart(&scan_filter_tmr_handle, scan_parm->scan_duration))
            {
                printf("\r\n[%s] timer restart fail !!", __FUNCTION__);
                return DA_BLE_RESULT_TYPE_FAILURE;
            } else {
                debug_print("\r\n[%s] timer restart success", __FUNCTION__);
            }
        }
    }

    return DA_BLE_RESULT_TYPE_SUCCESS;  
}

da_ble_result_type rtw_ble_client_stop_scan(void)
{ 
	T_GAP_DEV_STATE new_state;
	le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
	if (new_state.gap_scan_state != GAP_SCAN_STATE_SCANNING) {
		return DA_BLE_RESULT_TYPE_INVALID_STATE;
	}

    ble_tizenrt_client_send_msg(BLE_TIZENRT_STOP_SCAN, NULL);
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_client_connect(da_ble_client_bd_addr* addr, bool is_secured_connect)
{ 
    if (addr == NULL || g_conn_req_num >= ble_app_link_table_size)
    {
        printf("\r\n[%s] invalid\r\n", __FUNCTION__);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    int i = 0;
    while(i < ble_app_link_table_size && ble_tizenrt_bond_req_table[i++].addr) ;
    ble_tizenrt_bond_req_table[i - 1].addr = os_mem_alloc(0, GAP_BD_ADDR_LEN);
    memcpy(ble_tizenrt_bond_req_table[i - 1].addr, addr->bd_addr, GAP_BD_ADDR_LEN);
    ble_tizenrt_bond_req_table[i - 1].is_secured_connect = is_secured_connect;
    g_conn_req_num++;
    debug_print("\r\n[%s] ble_tizenrt_bond_req_table[%d]\r\n", __FUNCTION__, i - 1);
    T_TIZENRT_CONN_PARAM *conn_arg = os_mem_alloc(0, sizeof(T_TIZENRT_CONN_PARAM));
    memcpy(conn_arg->remote_bd, addr->bd_addr, GAP_BD_ADDR_LEN);
    conn_arg->remote_bd_type = addr->addr_type;
    conn_arg->conn_interval = addr->conn_interval;
    conn_arg->conn_latency = addr->slave_latency;
    printf("\r\n[%s] DestAddr: 0x%02X:0x%02X:0x%02X:0x%02X:0x%02X:0x%02X\r\n", __FUNCTION__, 
                        conn_arg->remote_bd[5], conn_arg->remote_bd[4], conn_arg->remote_bd[3],
                        conn_arg->remote_bd[2], conn_arg->remote_bd[1], conn_arg->remote_bd[0]);
    printf("\r\n[%s] ci: %d si: %d\r\n", __FUNCTION__, addr->conn_interval, addr->slave_latency);
    ble_tizenrt_client_send_msg(BLE_TIZENRT_CONNECT, conn_arg);
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_client_read_connected_device_list(da_ble_client_connected_list* out_connected_list)
{ 
    if (out_connected_list == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }
    
    memset(out_connected_list, 0, sizeof(da_ble_client_connected_list));
    for(uint16_t i = 0; i < ble_app_link_table_size; i++)
    {
        if(da_ble_app_link_table[i].conn_state == GAP_CONN_STATE_CONNECTED)
        {
            out_connected_list->connected_count++;
            out_connected_list->conn_handle[i] =  da_ble_app_link_table[i].conn_id;
        }
    }
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_client_read_connected_info(da_ble_conn_handle conn_handle, da_ble_client_device_connected* out_connected_device)
{ 
    if (out_connected_device == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if(da_ble_app_link_table[conn_handle].conn_state != GAP_CONN_STATE_CONNECTED)
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    
    T_GAP_CONN_INFO conn_info;
    if(le_get_conn_info(conn_handle, &conn_info))
    {
        out_connected_device->conn_handle = da_ble_app_link_table[conn_handle].conn_id;
        out_connected_device->addr.addr_type = da_ble_app_link_table[conn_handle].remote_bd_type;
        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &out_connected_device->addr.conn_interval, conn_handle);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &out_connected_device->addr.slave_latency, conn_handle);
        le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &out_connected_device->addr.mtu, conn_handle);
        memcpy(out_connected_device->addr.bd_addr, da_ble_app_link_table[conn_handle].remote_bd, GAP_BD_ADDR_LEN);
        if(da_ble_app_link_table[conn_handle].auth_state == GAP_AUTHEN_STATE_COMPLETE)
            out_connected_device->is_bonded = true;
        else
            out_connected_device->is_bonded = false;
    } else {
        printf("\r\n[%s] Fail to get connected info of %d", __FUNCTION__, conn_handle);
        return DA_BLE_RESULT_TYPE_FAILURE;
    }
    
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_client_delete_bond(da_ble_client_bd_addr* addr)
{
    if(NULL == addr)
    {
        printf("\r\n[%s] Invalid Input");
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    T_TIZENRT_DELETE_BOND_PARAM *param = os_mem_alloc(0, sizeof(T_TIZENRT_DELETE_BOND_PARAM));
    memcpy(param->remote_bd, addr->bd_addr, GAP_BD_ADDR_LEN);
    param->remote_bd_type = addr->addr_type;
    ble_tizenrt_client_send_msg(BLE_TIZENRT_DELETE_BOND, param);
#if 0
    for (int i = 0; i < ble_app_link_table_size; i++)
    {
        if(!memcmp(da_ble_app_link_table[i].remote_bd, addr->bd_addr, GAP_BD_ADDR_LEN))
        {
            da_ble_app_link_table[i].auth_state = 0;
        }
    }
#endif
    return DA_BLE_RESULT_TYPE_SUCCESS;
}

da_ble_result_type rtw_ble_client_delete_bond_all(void)
{
    ble_tizenrt_client_send_msg(BLE_TIZENRT_CLEAR_ALL_BONDS, NULL);
    for (int i = 0; i < ble_app_link_table_size; i++)
    {
            da_ble_app_link_table[i].auth_state = 0;
    }
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_client_disconnect(da_ble_conn_handle conn_handle)
{
    uint32_t conn_id = conn_handle;
    if(GAP_CONN_STATE_CONNECTED == da_ble_app_link_table[conn_handle].conn_state)
    {
        ble_tizenrt_client_send_msg(BLE_TIZENRT_DISCONNECT, (void *)conn_id);
        return DA_BLE_RESULT_TYPE_SUCCESS;
    } else {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }
}

da_ble_result_type rtw_ble_client_disconnect_all(void)
{
    uint32_t conn_id;
    for(uint16_t i = 0; i < ble_app_link_table_size; i++)
    {
        if(da_ble_app_link_table[i].conn_state == GAP_CONN_STATE_CONNECTED)
        {
            conn_id = i;
            ble_tizenrt_client_send_msg(BLE_TIZENRT_DISCONNECT, (void *)conn_id);
        }
    }
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

void *ble_tizenrt_read_sem = NULL;
da_ble_result_type rtw_ble_client_operation_read(da_ble_client_operation_handle_parm* handle, da_ble_client_data_info* out_data)
{ 
    if(ble_tizenrt_read_sem == NULL)
    {
        if(!os_mutex_create(&ble_tizenrt_read_sem))
        {
            printf("\r\n[%s] create mutex fail!", __FUNCTION__);
            return DA_BLE_RESULT_TYPE_FAILURE;
        } else {
            debug_print("\r\n[%s] create mutex 0x%x success", __FUNCTION__, ble_tizenrt_read_sem);
        }
    }

    if (handle == NULL || out_data == NULL || out_data->data == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    BLE_TIZENRT_READ_PARAM *param = os_mem_alloc(0, sizeof(BLE_TIZENRT_READ_PARAM));
    param->conn_id = handle->conn_handle;
    param->att_handle = handle->attr_handle;
    ble_tizenrt_client_send_msg(BLE_TIZENRT_READ, param);

    int ticks = 0;
    while(ticks++ < 30)
    {
        debug_print("\r\n[%s] ticks %d",__FUNCTION__, ticks);
        if(os_mutex_take(ble_tizenrt_read_sem, 1000))
        {  
            debug_print("\r\n[%s] take sema success",__FUNCTION__);
            if(ble_read_results[handle->conn_handle].cause == GAP_SUCCESS)
            {
                out_data->length = ble_read_results[handle->conn_handle].read_data.length;
                memcpy(out_data->data, ble_read_results[handle->conn_handle].read_data.data,
                                            ble_read_results[handle->conn_handle].read_data.length);
                os_mem_free(ble_read_results[handle->conn_handle].read_data.data);
                ble_read_results[handle->conn_handle].read_data.data = NULL;
                debug_print("\r\n[%s] read success: conn_id %d attr_handle 0x%x!",__FUNCTION__,
                                                            handle->conn_handle, handle->attr_handle);
                return DA_BLE_RESULT_TYPE_SUCCESS;
            } else {
                debug_print("\r\n[%s] read fail: conn_id %d attr_handle 0x%x!",__FUNCTION__,
                                                            handle->conn_handle, handle->attr_handle);
                return DA_BLE_RESULT_TYPE_FAILURE;
            }
        }
    }
    return DA_BLE_RESULT_TYPE_FAILURE; 
}

//extern T_GCS_WRITE_RESULT g_write_result;
void *ble_tizenrt_write_sem = NULL;
da_ble_result_type rtw_ble_client_operation_write(da_ble_client_operation_handle_parm* handle, da_ble_client_data_info* in_data)
{
    if (handle == NULL || in_data == NULL || in_data->data == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if(ble_tizenrt_write_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_write_sem))
        {
            printf("\r\n[%s] creat write mutex fail!", __FUNCTION__);
            return DA_BLE_RESULT_TYPE_FAILURE;
        } else {
            debug_print("\r\n[%s] creat write mutex 0x%x success", __FUNCTION__, ble_tizenrt_write_sem);
        }
    }

    BLE_TIZENRT_WRITE_PARAM param;
    debug_print("\r\n[%s] att_handle 0x%x len 0x%x data",__FUNCTION__, handle->attr_handle, in_data->length);

    param.data = in_data->data;
    param.length = in_data->length;
    param.conn_id = handle->conn_handle;
    param.att_handle = handle->attr_handle;
    ble_tizenrt_client_send_msg(BLE_TIZENRT_WRITE, &param);

    int wticks = 0;
    while(wticks++ < 30)
    {
        debug_print("\r\n[%s] wticks %d",__FUNCTION__, wticks);
        if(os_mutex_take(ble_tizenrt_write_sem, 1000))
        {  
            debug_print("\r\n[%s] take write mutex success",__FUNCTION__);
            if((*write_request_result).cause == GAP_SUCCESS)
            {
                debug_print("\r\n[%s] write success: conn_id %d attr_handle 0x%x!",__FUNCTION__,
                                                            handle->conn_handle, handle->attr_handle);
                                                            
                return DA_BLE_RESULT_TYPE_SUCCESS;
            } else {
                debug_print("\r\n[%s] write fail: conn_id %d attr_handle 0x%x cause %d ",__FUNCTION__,
                                        handle->conn_handle, handle->attr_handle, (*write_request_result).cause);
                return DA_BLE_RESULT_TYPE_FAILURE;
            }
        }
    }
    return DA_BLE_RESULT_TYPE_FAILURE; 
}

//extern T_GCS_WRITE_RESULT g_write_no_rsp_result;
void *ble_tizenrt_write_no_rsp_sem = NULL;
da_ble_result_type rtw_ble_client_operation_write_no_response(da_ble_client_operation_handle_parm* handle, da_ble_client_data_info* in_data)
{
    if (handle == NULL || in_data == NULL || in_data->data == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if(ble_tizenrt_write_no_rsp_sem == NULL)
    {
        if(false == os_mutex_create(&ble_tizenrt_write_no_rsp_sem))
        {
            printf("\r\n[%s] creat write mutex fail!", __FUNCTION__);
            return DA_BLE_RESULT_TYPE_FAILURE;
        } else {
            printf("\r\n[%s] creat ble_tizenrt_write_no_rsp_sem 0x%x success", __FUNCTION__, ble_tizenrt_write_no_rsp_sem);
        }
    }

    BLE_TIZENRT_WRITE_PARAM param;
    debug_print("\r\n[%s] att_handle 0x%x len 0x%x data",__FUNCTION__, handle->attr_handle, in_data->length);
    param.data = in_data->data;
    param.length = in_data->length;
    param.conn_id = handle->conn_handle;
    param.att_handle = handle->attr_handle;

    ble_tizenrt_client_send_msg(BLE_TIZENRT_WRITE_NO_RSP, &param);
    do {
        if(os_mutex_take(ble_tizenrt_write_no_rsp_sem, 1000))
        {
            debug_print("\r\n[%s] take write mutex success",__FUNCTION__);
            if((*write_no_rsponse_result).cause == GAP_SUCCESS)
            {
                debug_print("\r\n[%s] write success: conn_id %d attr_handle 0x%x!",__FUNCTION__,
                                                            handle->conn_handle, handle->attr_handle);
                printf("\r\n[%s] snd write cmd success",__FUNCTION__);
                return DA_BLE_RESULT_TYPE_SUCCESS;
            } else {
                debug_print("\r\n[%s] write fail: conn_id %d attr_handle 0x%x!",__FUNCTION__,
                                                            handle->conn_handle, handle->attr_handle);
                return DA_BLE_RESULT_TYPE_FAILURE;
            }
            memset(write_no_rsponse_result, 0, sizeof((*write_no_rsponse_result)));
        }
    } while(1);

    return DA_BLE_RESULT_TYPE_FAILURE;
}

da_ble_result_type rtw_ble_client_operation_enable_notification(da_ble_client_operation_handle_parm* handle)
{ 
    if (handle == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    if (client_init_parm == NULL || client_init_parm->da_ble_client_operation_notification_cb == NULL)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }

    uint8_t val[2] = {0x1, 0x0};
    da_ble_client_data_info in_data;
    in_data.length = 2;
    in_data.data = val;

    return rtw_ble_client_operation_write(handle, &in_data);
}

da_ble_result_type rtw_ble_client_deinit(void)
{
    ble_tizenrt_central_app_deinit();

    g_conn_req_num = 0;
    os_mem_free(client_init_parm);
    client_init_parm = NULL;
    os_mem_free(ble_tizenrt_bond_req_table);
    ble_tizenrt_bond_req_table = NULL;

    return DA_BLE_RESULT_TYPE_SUCCESS; 
}
#endif /* DA_BLE_CLIENT_C_ */
