#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"

#include "bk_ble_tizenrt.h"
#include "ble_tizenrt_central.h"

#include "components/bluetooth/bk_ble_types.h"
#include "components/bluetooth/bk_ble.h"
#include "components/bluetooth/bk_dm_bluetooth.h"

#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

#define LOG_TAG "bkble_client"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)


static trble_client_init_config client_init_parm = {0};
static beken_mutex_t read_mutex = NULL;
static beken_semaphore_t read_sema = NULL;
static beken_semaphore_t write_sema = NULL;
static beken_semaphore_t write_no_rsp_sema = NULL;
static ble_gattc_read_ind_t read_inds[HAL_BLE_CON_NUM];
trble_client_init_config *bktr_ble_client_get_param(void)
{
    return &client_init_parm;
}

///Scanner(Observer)
trble_result_e bktr_ble_client_set_scan(uint16_t scan_interval, uint16_t scan_window, trble_scan_type scan_type)
{
    LOGD("set scan");
    hal_ble_scan_env.scan_param.scan_intv = scan_interval;
    hal_ble_scan_env.scan_param.scan_wd = scan_window;

    if (scan_type == TRBLE_SCAN_PASSIVE)
    {
        hal_ble_scan_env.scan_param.scan_type = PASSIVE_SCANNING;
    }
    else if (scan_type == TRBLE_SCAN_ACTIVE)
    {
        hal_ble_scan_env.scan_param.scan_type = ACTIVE_SCANNING;
    }

    hal_ble_scan_env.scan_param.scan_filter = BASIC_UNFILTER_SCAN_POLICY;
    hal_ble_scan_env.filt_duplicate = 0;

    hal_ble_scan_env.filter_param.len = 0;
    hal_ble_scan_env.filter_param.enable = false;
    os_memset(hal_ble_scan_env.filter_param.p_filter, 0, TRBLE_ADV_RAW_DATA_MAX_LEN);
    hal_ble_scan_env.filter_param.offset = 0;
    LOGD("set scan param, scan_interval %d scan_window %d scan_type %d", scan_interval, scan_window, scan_type);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_start_scan_with_filter(trble_scan_filter *scan_parm, bool whitelist_enable)
{
    LOGD("scan with filter, raw_data_length %d, whitelist_enable:%d", scan_parm->raw_data_length, whitelist_enable);

    if (client_init_parm.trble_device_scanned_cb == NULL || scan_parm == NULL)
    {
        LOGE("trble_device_scanned_cb is NULL or scan_parm is NULL");
        return TRBLE_FAIL;
    }

    if (BLE_HAL_SCAN_STATE_IDLE == hal_ble_scan_env.scan_status)
    {
        hal_ble_scan_env.duration = scan_parm->scan_duration/10;//1ms->10ms unit
        LOGD("scan duration %dms", scan_parm->scan_duration);
        if (scan_parm->raw_data_length != 0)
        {
            hal_ble_scan_env.filter_param.len = scan_parm->raw_data_length;
            hal_ble_scan_env.filter_param.enable = true;
            os_memset(hal_ble_scan_env.filter_param.p_filter, 0, TRBLE_ADV_RAW_DATA_MAX_LEN);
            memcpy(hal_ble_scan_env.filter_param.p_filter, scan_parm->raw_data, scan_parm->raw_data_length);
            hal_ble_scan_env.filter_param.offset = 0;
        }else
        {
            hal_ble_scan_env.filter_param.len = 0;
            hal_ble_scan_env.filter_param.enable = false;
            os_memset(hal_ble_scan_env.filter_param.p_filter, 0, TRBLE_ADV_RAW_DATA_MAX_LEN);
            hal_ble_scan_env.filter_param.offset = 0;
        }

        if (whitelist_enable)
        {
            hal_ble_scan_env.scan_param.scan_type = PASSIVE_SCANNING;
            hal_ble_scan_env.scan_param.scan_filter = BASIC_FILTER_SCAN_POLICY;
        }
        else
        {
            hal_ble_scan_env.scan_param.scan_filter = BASIC_UNFILTER_SCAN_POLICY;
        }

        if (ble_cmd_queue_push(CMD_BLE_START_SCAN, hal_ble_scan_env.scan_idx) != 0)
        {
            LOGE("ble_cmd_queue_push fail");
        }

        return TRBLE_SUCCESS;
    }
    else
    {
        LOGE("can not start scan, scan_status %d", hal_ble_scan_env.scan_status);
        return TRBLE_INVALID_STATE;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_stop_scan(void)
{
    LOGD("stop scan");

    if (ble_cmd_queue_push(CMD_BLE_STOP_SCAN, hal_ble_scan_env.scan_idx) != 0)
    {
        LOGE("ble_cmd_queue_push fail");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_scan_whitelist_add(trble_addr *addr)
{
    uint8_t addr_type = BLE_ADDR_TYPE_PUBLIC;
    if(addr->type == TRBLE_ADDR_TYPE_PUBLIC)
    {
        addr_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    }else if((TRBLE_ADDR_TYPE_RANDOM_STATIC <= addr->type) && (TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE >= addr->type))
    {
        addr_type = GAP_REMOTE_ADDR_LE_RANDOM;
    }else
    {
        LOGE("error addr type %d not support", addr->type);
        return TRBLE_FAIL;
    }
    uint8_t ret = le_modify_white_list(GAP_WHITE_LIST_OP_ADD, addr->mac, addr_type);

    if (ret)
    {
        LOGE("le_modify_white_list add fail");
        return TRBLE_FAIL;
    }

    bk_adapter_ble_add_white_list_data(addr->mac, addr_type);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_scan_whitelist_delete(trble_addr *addr)
{
    uint8_t addr_type = BLE_ADDR_TYPE_PUBLIC;
    if(addr->type == TRBLE_ADDR_TYPE_PUBLIC)
    {
        addr_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    }else if((TRBLE_ADDR_TYPE_RANDOM_STATIC <= addr->type) && (TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE >= addr->type))
    {
        addr_type = GAP_REMOTE_ADDR_LE_RANDOM;
    }else
    {
        LOGE("error addr type %d not support", addr->type);
        return TRBLE_FAIL;
    }
    uint8_t ret = le_modify_white_list(GAP_WHITE_LIST_OP_REMOVE, addr->mac, addr_type);

    if (ret)
    {
        LOGE("le_modify_white_list remove fail");
        return TRBLE_FAIL;
    }

    bk_adapter_ble_remove_white_list_data(addr->mac, addr_type);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_scan_whitelist_clear_all(void)
{
    uint8_t ret = le_modify_white_list(GAP_WHITE_LIST_OP_CLEAR, NULL, 0);

    if (ret)
    {
        LOGE("le_modify_white_list clear fail");
        return TRBLE_FAIL;
    }

    bk_adapter_ble_clear_white_list_data();
    return TRBLE_SUCCESS;
}

///Central(Client)
trble_result_e bktr_ble_client_connect(trble_conn_info *conn_info, bool is_secured_connect)
{
    if (conn_info == NULL)
    {
        LOGE("conn_info is NULL");
        return TRBLE_INVALID_ARGS;
    }

    if (hal_ble_env.master_count >= (HAL_BLE_CON_NUM - 1))
    {
        LOGE("reach the upper limit!!!");
        return TRBLE_INVALID_STATE;
    }

    if (BLE_HAL_CONN_DEV_STATE_IDLE == hal_ble_con_env.init_status)
    {
        os_memcpy(hal_ble_con_env.peer_addr.addr, conn_info->addr.mac, TRBLE_BD_ADDR_MAX_LEN);
        hal_ble_con_env.peer_addr_type = bk_adapter_ble_convert_addr_type_2_bk(conn_info->addr.type);
        hal_ble_con_env.conn_param.con_latency = conn_info->slave_latency;
        hal_ble_con_env.conn_param.intv_max = hal_ble_con_env.conn_param.intv_min = conn_info->conn_interval;
        hal_ble_con_env.conn_param.sup_to = BLE_CON_CON_TIMEOUT;
        hal_ble_con_env.conn_param.filter_policy = 0;
        hal_ble_con_env.is_secured_connect = is_secured_connect;
        hal_ble_con_env.mtu = conn_info->mtu;
        hal_ble_con_env.conn_scan_timeout = conn_info->scan_timeout;
        hal_ble_con_env.init_idx = bk_ble_get_idle_conn_idx_handle();
        LOGI("peer_addr:" BD_ADDR_FMT " addr_type:%d, scan_timeout:%dms, is_sec:%d", BD_ADDR_ARG(conn_info->addr.mac), hal_ble_con_env.peer_addr_type,
            conn_info->scan_timeout, is_secured_connect);
        if (ble_cmd_queue_push(CMD_BLE_INIT_CREATE, hal_ble_con_env.init_idx) != 0)
        {
            LOGE("ble_cmd_queue_push fail");
        }
    }
    else
    {
        LOGE("can not create an acl connection, init_status %d", hal_ble_con_env.init_status);
        return TRBLE_INVALID_STATE;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_bond(trble_conn_handle conn_handle)
{
    T_GAP_CONN_INFO con_info;

    if (le_get_conn_info(conn_handle, &con_info) == 0)
    {
        LOGE("error not in connected state");
        return TRBLE_INVALID_STATE;
    }

    if (le_bond_pair(conn_handle))
    {
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_disconnect(trble_conn_handle conn_handle)
{
    T_GAP_CONN_INFO con_info;

    if (le_get_conn_info(conn_handle, &con_info) == 0)
    {
        LOGE("error not in connected state");
        return TRBLE_INVALID_STATE;
    }

    if (ble_cmd_queue_push(CMD_BLE_CONN_DIS_CONN, conn_handle) != 0)
    {
        LOGE("ble_cmd_queue_push fail");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_read_connected_device_list(trble_connected_list *out_connected_list)
{
    if (out_connected_list == NULL)
    {
        LOGE("out_connected_list is NULL");
        return TRBLE_FAIL;
    }
    os_memset(out_connected_list, 0, sizeof(trble_connected_list));
    uint8_t active_link = le_get_active_link_num();
    if(active_link > TRBLE_MAX_CONNECTION_COUNT)
    {
        LOGW("active_link %d > TRBLE_MAX_CONNECTION_COUNT %d", active_link, TRBLE_MAX_CONNECTION_COUNT);
    }
    uint8_t conn_num = 0;
    for(int i=0; i<HAL_BLE_CON_NUM;i++)
    {
        if (HAL_CONN_STATE_CONNECTED == hal_ble_con_env.con_dev[i].con_status && conn_num < TRBLE_MAX_CONNECTION_COUNT)
        {
            out_connected_list->conn_handle[conn_num] = i;
            conn_num++;
        }
    }
    out_connected_list->connected_count = conn_num;
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_read_connected_info(trble_conn_handle conn_handle, trble_device_connected *out_connected_device)
{
    if(out_connected_device == NULL)
    {
        LOGE("out_connected_device is NULL");
        return TRBLE_FAIL;
    }
    T_GAP_CONN_INFO con_info;
    if(le_get_conn_info(conn_handle, &con_info) == 0)
    {
        LOGE("error not in connected state");
        return TRBLE_INVALID_STATE;
    }
    uint8_t bond_idx = bk_adapter_bt_storage_find_linkkey_info_index(con_info.remote_bd, con_info.remote_bd_type);
    if(bond_idx >= 0)
    {
        out_connected_device->is_bonded = true;
    }
    else
    {
        out_connected_device->is_bonded = false;
    }
    os_memcpy(out_connected_device->conn_info.addr.mac, con_info.remote_bd, TRBLE_BD_ADDR_MAX_LEN);
    out_connected_device->conn_info.addr.type = bk_adapter_ble_convert_addr_type_2_tr(con_info.remote_bd_type, con_info.remote_bd);
    out_connected_device->conn_info.conn_interval = hal_ble_con_env.con_dev[conn_handle].intv;
    out_connected_device->conn_info.slave_latency = hal_ble_con_env.con_dev[conn_handle].con_latency;
    out_connected_device->conn_info.mtu = hal_ble_con_env.con_dev[conn_handle].mtu_size;
    LOGD("con_idx:%d, addr:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x, addr_type %d", conn_handle,
        out_connected_device->conn_info.addr.mac[0], out_connected_device->conn_info.addr.mac[1], out_connected_device->conn_info.addr.mac[2],
        out_connected_device->conn_info.addr.mac[3], out_connected_device->conn_info.addr.mac[4], out_connected_device->conn_info.addr.mac[5],
        out_connected_device->conn_info.addr.type);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_disconnect_all(void)
{
    for(int i=0; i<HAL_BLE_CON_NUM; i++)
    {
        if (HAL_CONN_STATE_CONNECTED == hal_ble_con_env.con_dev[i].con_status
                        && hal_ble_con_env.con_dev[i].role == LINK_ROLE_MASTER)
        {
            if (ble_cmd_queue_push(CMD_BLE_CONN_DIS_CONN, i) != 0)
            {
                LOGE("ble_cmd_queue_push fail, conn_idx %d", i);
            }
        }
    }
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_operation_read(trble_operation_handle *handle, trble_data *out_data)
{
    if(handle == NULL || out_data == NULL || out_data->data == NULL)
    {
        LOGE("handle or out_data is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[handle->conn_handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", handle->conn_handle);
        return TRBLE_FAIL;
    }
    rtos_lock_mutex(&read_mutex);
    if(ble_push_read_info(handle->conn_handle, 0) != 0)
    {
        LOGE("ble_push_read_info fail, conn_handle %d", handle->conn_handle);
        rtos_unlock_mutex(&read_mutex);
        return TRBLE_FAIL;
    }
    if (BK_ERR_BLE_SUCCESS !=  bk_ble_gattc_read(handle->conn_handle, handle->attr_handle, 0))
    {
        LOGE("gattc read fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        rtos_unlock_mutex(&read_mutex);
        return TRBLE_FAIL;
    }else
    {
        LOGD("gattc read success, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
    }
    uint8_t ticks = 0;
    while(ticks++ < 3)
    {
        LOGD("gattc read tick %d", ticks);
        if(rtos_get_semaphore(&read_sema, 1000) == kNoErr)
        {
            LOGD("gattc read sema get");
            out_data->length = read_inds[handle->conn_handle].len;
            if(read_inds[handle->conn_handle].len)
            {
                os_memcpy(out_data->data, read_inds[handle->conn_handle].data, read_inds[handle->conn_handle].len);
                os_free(read_inds[handle->conn_handle].data);
                read_inds[handle->conn_handle].data = NULL;
                read_inds[handle->conn_handle].len = 0;
            }
            rtos_unlock_mutex(&read_mutex);
            return TRBLE_SUCCESS;
        }
    }
    rtos_unlock_mutex(&read_mutex);
    return TRBLE_FAIL;
}

trble_result_e bktr_ble_client_operation_write(trble_operation_handle *handle, trble_data *in_data)
{
    if(in_data == NULL || in_data->data == NULL)
    {
        LOGE("in_data is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[handle->conn_handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", handle->conn_handle);
        return TRBLE_FAIL;
    }
    if(ble_push_write_info(handle->conn_handle, 0, handle->attr_handle) != 0)
    {
        LOGE("ble_push_write_info fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }
    if (BK_ERR_BLE_SUCCESS !=  bk_ble_gattc_write(handle->conn_handle, handle->attr_handle, in_data->data, in_data->length, 0))
    {
        LOGE("gattc write fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }else
    {
        LOGD("gattc write success, conn_handle %d, attr_handle %d, data_len:%d", handle->conn_handle, handle->attr_handle, in_data->length);
    }
    uint8_t ticks = 0;
    while(ticks++ < 3)
    {
        LOGD("gattc write tick %d", ticks);
        if(rtos_get_semaphore(&write_sema, 1000) == kNoErr)
        {
            LOGD("gattc write sema get");
            return TRBLE_SUCCESS;
        }
    }
    return TRBLE_FAIL;
}

trble_result_e bktr_ble_client_operation_write_no_response(trble_operation_handle *handle, trble_data *in_data)
{
    if(in_data == NULL || in_data->data == NULL)
    {
        LOGE("in_data is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[handle->conn_handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", handle->conn_handle);
        return TRBLE_FAIL;
    }
    if(ble_push_write_info(handle->conn_handle, 0, handle->attr_handle) != 0)
    {
        LOGE("ble_push_write_info fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }
    if (BK_ERR_BLE_SUCCESS !=  bk_ble_gattc_write(handle->conn_handle, handle->attr_handle, in_data->data, in_data->length, 1))
    {
        LOGE("gattc write no response fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }else
    {
        LOGD("gattc write no response success, conn_handle %d, attr_handle %d, data_len:%d", handle->conn_handle, handle->attr_handle, in_data->length);
    }
    uint8_t ticks = 0;
    while(ticks++ < 3)
    {
        LOGD("gattc write no response tick %d", ticks);
        if(rtos_get_semaphore(&write_no_rsp_sema, 1000) == kNoErr)
        {
            LOGD("gattc write no response sema get");
            return TRBLE_SUCCESS;
        }
    }
    return TRBLE_FAIL;
}

trble_result_e bktr_ble_client_write_read_queue_cnt(trble_conn_handle *handle, uint8_t *write_read_count)
{
    if(handle == NULL || write_read_count == NULL)
    {
        LOGE("handle or write_read_count is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[*handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", *handle);
        return TRBLE_FAIL;
    }
    *write_read_count = hal_ble_con_env.con_dev[*handle].wr_list->length + hal_ble_con_env.con_dev[*handle].rd_list->length;
    LOGD("gattc write&read count %d", *write_read_count);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_operation_enable_notification(trble_operation_handle *handle)
{
    if(handle == NULL)
    {
        LOGE("handle is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[handle->conn_handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", handle->conn_handle);
        return TRBLE_FAIL;
    }
    uint8_t enable[2] = {0x01, 0x00};
    if (BK_ERR_BLE_SUCCESS !=  bk_ble_gattc_write(handle->conn_handle, handle->attr_handle, enable, 2, 0))
    {
        LOGE("gattc write notification fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_operation_enable_indication(trble_operation_handle *handle)
{
    if(handle == NULL)
    {
        LOGE("handle is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[handle->conn_handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", handle->conn_handle);
        return TRBLE_FAIL;
    }
    uint8_t enable[2] = {0x02, 0x00};
    if (BK_ERR_BLE_SUCCESS !=  bk_ble_gattc_write(handle->conn_handle, handle->attr_handle, enable, 2, 0))
    {
        LOGE("gattc write indication fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_client_operation_enable_notification_and_indication(trble_operation_handle *handle)
{
          if(handle == NULL)
    {
        LOGE("handle is NULL");
        return TRBLE_FAIL;
    }
    if(hal_ble_con_env.con_dev[handle->conn_handle].con_status != HAL_CONN_STATE_CONNECTED)
    {
        LOGE("conn_handle %d not connected", handle->conn_handle);
        return TRBLE_FAIL;
    }
    uint8_t enable[2] = {0x03, 0x00};
    if (BK_ERR_BLE_SUCCESS !=  bk_ble_gattc_write(handle->conn_handle, handle->attr_handle, enable, 2, 0))
    {
        LOGE("gattc write indication&notification fail, conn_handle %d, attr_handle %d", handle->conn_handle, handle->attr_handle);
        return TRBLE_FAIL;
    }
    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_client_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
    case BLE_5_INIT_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        LOGD("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x",
             c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
             c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

        hal_ble_con_env.con_dev[c_ind->conn_idx].con_status = HAL_CONN_STATE_CONNECTED;
        hal_ble_con_env.con_dev[c_ind->conn_idx].role = LINK_ROLE_MASTER;
        hal_ble_con_env.con_dev[c_ind->conn_idx].peer_addr_type = c_ind->peer_addr_type;
        os_memcpy(hal_ble_con_env.con_dev[c_ind->conn_idx].peer_addr, c_ind->peer_addr, 6);

        if (BLE_IS_RPA(c_ind->peer_addr_type, (c_ind->peer_addr[5])))
        {
            bk_adapter_ble_find_initial_pseudo_addr(hal_ble_con_env.con_dev[c_ind->conn_idx].peer_addr);
        }

        hal_ble_con_env.con_dev[c_ind->conn_idx].intv = c_ind->con_interval;
        hal_ble_con_env.con_dev[c_ind->conn_idx].con_latency = c_ind->con_latency;
        hal_ble_con_env.con_dev[c_ind->conn_idx].sup_to = c_ind->sup_to;

        bk_adapter_reset_client_info(c_ind->conn_idx);

        hal_ble_env.master_count++;

        bk_ble_set_max_mtu(hal_ble_con_env.mtu);
        bk_ble_gatt_mtu_change(c_ind->conn_idx);

        if (BLE_HAL_CONN_DEV_STATE_INITIATING == hal_ble_con_env.init_status)
        {
            hal_ble_env.dev_state.gap_conn_state = GAP_CONN_DEV_STATE_IDLE;
            bk_adapter_notify_bt_status(GAP_MSG_LE_DEV_STATE_CHANGE, NULL);

            hal_ble_con_env.init_status = BLE_HAL_CONN_DEV_STATE_IDLE;
            hal_ble_con_env.init_idx = 0xff;

            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_conn_state_change.conn_id = c_ind->conn_idx;
            msg_data.gap_conn_state_change.new_state = GAP_CONN_STATE_CONNECTED;
            msg_data.gap_conn_state_change.disc_cause = 0;
            bk_adapter_notify_bt_status(GAP_MSG_LE_CONN_STATE_CHANGE, &msg_data);
        }
    }
    break;

    case BLE_5_INIT_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        LOGD("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d, reason:0x%x, peer_addr:" BD_ADDR_FMT, d_ind->conn_idx, d_ind->reason, BD_ADDR_ARG(hal_ble_con_env.con_dev[d_ind->conn_idx].peer_addr));
        uint8_t set_semp = 0;
        {
            T_LE_GAP_MSG_DATA msg_data = {0};
            msg_data.gap_conn_state_change.conn_id = d_ind->conn_idx;
            if(!hal_ble_con_env.con_dev[d_ind->conn_idx].app_connected)
            {
                msg_data.gap_conn_state_change.conn_id = 0xff; //connet fail
            }
            msg_data.gap_conn_state_change.new_state = GAP_CONN_STATE_DISCONNECTED;
            msg_data.gap_conn_state_change.disc_cause = d_ind->reason;
            bk_adapter_notify_bt_status(GAP_MSG_LE_CONN_STATE_CHANGE, &msg_data);
        }
        if(hal_ble_con_env.con_dev[d_ind->conn_idx].con_status == HAL_CONN_STATE_DISCONNECTING)
        {
            LOGD("disconnect by user");
            set_semp = 1;
        }

        hal_ble_con_env.con_dev[d_ind->conn_idx].con_status = HAL_CONN_STATE_DISCONNECTED;
        os_memset(hal_ble_con_env.con_dev[d_ind->conn_idx].peer_addr, 0, 6);
        hal_ble_con_env.con_dev[d_ind->conn_idx].role = LINK_ROLE_UNDEFINED;
        hal_ble_con_env.con_dev[d_ind->conn_idx].auth_state = 0xff;
        hal_ble_con_env.con_dev[d_ind->conn_idx].app_connected = 0;
        hal_ble_env.master_count--;

        bk_adapter_reset_client_info(d_ind->conn_idx);
        if(set_semp)
        {
            bk_set_ble_cmd_sema();
        }
    }
    break;

    case BLE_5_INIT_CONNECT_FAILED_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        LOGD("BLE_5_INIT_CONNECT_FAILED_EVENT:conn_idx:%d,reason:0x%x", d_ind->conn_idx, d_ind->reason);

        if (BLE_HAL_CONN_DEV_STATE_INITIATING == hal_ble_con_env.init_status)
        {
            hal_ble_env.dev_state.gap_conn_state = GAP_CONN_DEV_STATE_IDLE;
            hal_ble_con_env.init_status = BLE_HAL_CONN_DEV_STATE_IDLE;
            hal_ble_con_env.init_idx = 0xff;
            {
                T_LE_GAP_MSG_DATA msg_data = {0};
                msg_data.gap_conn_state_change.conn_id = 0xff;
                msg_data.gap_conn_state_change.new_state = GAP_CONN_STATE_DISCONNECTED;
                msg_data.gap_conn_state_change.disc_cause = bk_adapter_ble_hci_error_code(d_ind->reason);
                bk_adapter_notify_bt_status(GAP_MSG_LE_CONN_STATE_CHANGE, &msg_data);
            }
        }
    }
    break;

    default:
        break;
    }

    return 0;
}

static void bktr_ble_gattc_sdp_comm_callback(MASTER_COMMON_TYPE type, uint8_t conidx, void *param)
{
    switch(type)
    {
        case MST_TYPE_SVR_UUID:
        case MST_TYPE_DISCOVER_PRI_SERVICE_RSP:
        case MST_TYPE_DISCOVER_PRI_SERVICE_BY_UUID_RSP:
        case MST_TYPE_DISCOVER_PRI_SERVICE_BY_128_UUID_RSP:
        {
            struct ble_sdp_svc_ind *srv_ind = (typeof(srv_ind))param;
            // uint8_t client_idx = 0;  // Unused variable
            if(srv_ind->uuid_len == BK_UUID_LEN_16)
            {
                LOGD("service discovery uuid 0x%02x%02x sh 0x%04x eh 0x%04x len %d", srv_ind->uuid[0], srv_ind->uuid[1],
                    srv_ind->start_hdl, srv_ind->end_hdl, srv_ind->uuid_len);
            }else if(srv_ind->uuid_len == BK_UUID_LEN_128)
            {
                LOGD("service discovery uuid " UUID_128_FORMAT " sh 0x%04x eh 0x%04x len %d", UUID_128(srv_ind->uuid), srv_ind->start_hdl, srv_ind->end_hdl, srv_ind->uuid_len);
            }else
            {
                LOGD("Invalid service discover uuid len %d", srv_ind->uuid_len);
            }
        }
        break;
        case MST_TYPE_ATT_UUID:
        case MST_TYPE_DISCOVER_CHAR_RSP:
        case MST_TYPE_DISCOVER_CHAR_BY_UUID_RSP:
        case MST_TYPE_DISCOVER_CHAR_BY_128_UUID_RSP:
        {
            struct ble_sdp_char_inf *char_inf = (typeof(char_inf))param;
            // uint8_t client_idx = 0;  // Unused variable
            if(char_inf->uuid_len == BK_UUID_16_LEN)
            {
                LOGD("characteristic discovery uuid 0x%02x%02x decl handle 0x%04x value handle 0x%04x prop 0x%x len %d\n", char_inf->uuid[0], char_inf->uuid[1],
                    char_inf->char_hdl, char_inf->val_hdl, char_inf->prop, char_inf->uuid_len);
            }else if(char_inf->uuid_len ==BK_UUID_128_LEN)
            {
                LOGD("characteristic discovery uuid " UUID_128_FORMAT " decl handle 0x%04x value handle 0x%04x prop 0x%x len %d\n", UUID_128(char_inf->uuid), char_inf->char_hdl, char_inf->val_hdl, char_inf->prop, char_inf->uuid_len);
            }else
            {
                LOGD("Invalid characteristic discovery uuid len %d", char_inf->uuid_len);
            }
            LOGD("characteristic properties: read:%d, write norsp:%d, write:%d, notify:%d, indicate:%d",
                (char_inf->prop & GATT_DB_CHAR_READ_PROPERTY),
                (char_inf->prop & GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY),
                (char_inf->prop & GATT_DB_CHAR_WRITE_PROPERTY),
                (char_inf->prop & GATT_DB_CHAR_NOTIFY_PROPERTY),
                (char_inf->prop & GATT_DB_CHAR_INDICATE_PROPERTY));
        }
        break;
        case MST_TYPE_ATT_DESC:
        case MST_TYPE_DISCOVER_CHAR_DESC:
        {
            struct ble_sdp_char_desc_inf *desc_inf = (typeof(desc_inf))param;
            if(desc_inf->uuid_len == BK_UUID_16_LEN)
            {
                LOGD("characteristic desc rsp uuid 0x%02x%02x handle 0x%04x len %d\n", desc_inf->uuid[0], desc_inf->uuid[1],
                    desc_inf->desc_hdl, desc_inf->uuid_len);
            }else if(desc_inf->uuid_len == BK_UUID_128_LEN)
            {
                LOGD("characteristic desc rsp uuid " UUID_128_FORMAT " handle 0x%04x len %d\n", UUID_128(desc_inf->uuid), desc_inf->desc_hdl, desc_inf->uuid_len);
            }else
            {
                LOGD("Invalid characteristic desc rsp uuid len %d", desc_inf->uuid_len);
            }
        }
        break;

        case MST_TYPE_DISCOVER_COMPLETED:
        {
            struct ble_descover_complete_inf *desc_inf = (typeof(desc_inf))param;
            LOGD("discover complete action 0x%x con_id 0x%x\n", desc_inf->action_id, conidx);
        }
        break;
        case MST_TYPE_SDP_END:
        {
            LOGD("discover end con_id 0x%x\n", conidx);
        }
        break;

        case MST_TYPE_UPP_ASK:
        {
            struct mst_comm_updata_para *tmp = (typeof(tmp))param;
            LOGD("MST_TYPE_UPP_ASK accept");
            tmp->is_agree = 1;
        }
        break;

        case MST_TYPE_ATTC_WRITE_RSP:
        {
            struct ble_attc_wr_rd_op *op = (typeof(op))param;
            LOGD("MST_TYPE_ATTC_WRITE_RSP, status:%d", op->status);
            uint8_t client_idx = 0;
            uint16_t wr_handle = 0;
            if(ble_pop_write_info(conidx, &client_idx, &wr_handle) == 0)
            {
                if(write_sema != NULL)
                {
                    rtos_set_semaphore(&write_sema);
                }
            }
        }
        break;
        case MST_TYPE_ATTC_WRITE_NO_RESPONSE:
        {
            struct ble_attc_wr_rd_op *op = (typeof(op))param;
            LOGD("MST_TYPE_ATTC_WRITE_NO_RESPONSE, status:%d", op->status);
            uint8_t client_idx = 0;
            uint16_t wr_handle = 0;
            if(ble_pop_write_info(conidx, &client_idx, &wr_handle) == 0)
            {
                if(write_no_rsp_sema != NULL)
                {
                    rtos_set_semaphore(&write_no_rsp_sema);
                }
            }
        }
        break;

        default:
            break;
    }
}

static void bkrt_ble_gattc_sdp_charac_callback(CHAR_TYPE type, uint8_t conidx, uint16_t hdl, uint16_t len, uint8_t *data)
{
    LOGD("%s type:%d len:%d, hdl 0x%04x",__func__, type, len, hdl);

    switch(type)
    {
        case CHARAC_NOTIFY:
        {
            LOGD("CHARAC_NOTIFY conidx:%d, hdl:0x%04x, len:%d", conidx, hdl, len);
            ble_tizenrt_notification_t *noti = os_zalloc(sizeof(ble_tizenrt_notification_t));
            if(noti)
            {
                noti->handle.conn_handle = conidx;
                noti->handle.attr_handle = hdl;
                if(len)
                {
                    noti->noti_data.data = os_zalloc(len);
                    if(noti->noti_data.data)
                    {
                        os_memcpy(noti->noti_data.data, data, len);
                        noti->noti_data.length = len;
                    }else
                    {
                        LOGE("os_zalloc noti->noti_data.data failed %d", len);
                        break;
                    }
                }
                if(ble_evt_queue_push(EVT_BLE_NOTIFICATION_MSG, noti) != 0)
                {
                    LOGE("ble_evt_queue_push EVT_BLE_NOTIFICATION_MSG failed");
                    if(len)
                    {
                        os_free(noti->noti_data.data);
                    }
                    os_free(noti);
                }
            }else
            {
                LOGE("os_zalloc ble_tizenrt_notification_t failed");
            }
        }
        break;
        case CHARAC_INDICATE:
        {
            LOGD("CHARAC_INDICATE conidx:%d, hdl:0x%04x, len:%d", conidx, hdl, len);
            ble_tizenrt_indication_t *ind = os_zalloc(sizeof(ble_tizenrt_indication_t));
            if(ind)
            {
                ind->handle.conn_handle = conidx;
                ind->handle.attr_handle = hdl;
                if(len)
                {
                    ind->indi_data.data = os_zalloc(len);
                    if(ind->indi_data.data)
                    {
                        os_memcpy(ind->indi_data.data, data, len);
                        ind->indi_data.length = len;
                    }else
                    {
                        LOGE("os_zalloc ind->ind_data.data failed %d", len);
                        break;
                    }
                }
                if(ble_evt_queue_push(EVT_BLE_INDICATION_MSG, ind) != 0)
                {
                    LOGE("ble_evt_queue_push EVT_BLE_INDICATION_MSG failed");
                    if(len)
                    {
                        os_free(ind->indi_data.data);
                    }
                    os_free(ind);
                }
            }else
            {
                LOGE("os_zalloc ble_tizenrt_indication_t failed");
            }
        }
        break;

        case CHARAC_WRITE_DONE:
        break;

        case CHARAC_READ:
        {
            LOGD("CHARAC_READ, conidx:%d, hdl:0x%04x, len:%d", conidx, hdl, len);
            ble_gattc_read_ind_t *read_ind = &read_inds[conidx];
            uint8_t client_idx = 0;
            ble_pop_read_info(conidx, &client_idx);
            os_memset(read_ind, 0, sizeof(ble_gattc_read_ind_t));
            read_ind->conidx = conidx;
            read_ind->hdl = hdl;
            read_ind->len = len;
            if(len)
            {
                read_ind->data = os_zalloc(len);
                if(read_ind->data)
                {
                    os_memcpy(read_ind->data, data, len);
                }else
                {
                    LOGE("os_zalloc read_ind->data failed %d", len);
                }
            }
            if(read_sema != NULL)
            {
                rtos_set_semaphore(&read_sema);
            }
        }
        break;

        case CHARAC_READ_DONE:
        break;

        default:
            LOGE("%s, type(%x) not supported\r\n",__func__,type);
        break;
    }
}


int32_t bk_tr_ble_client_init(trble_client_init_config *config)
{
    //init client
    if(read_mutex == NULL)
    {
        rtos_init_mutex(&read_mutex);
    }
    if(read_sema == NULL)
    {
        rtos_init_semaphore(&read_sema, 0);
    }
    if(write_sema == NULL)
    {
        rtos_init_semaphore(&write_sema, 0);
    }
    if(write_no_rsp_sema == NULL)
    {
        rtos_init_semaphore(&write_no_rsp_sema, 0);
    }
    os_memcpy(&client_init_parm, config, sizeof(*config));
    bk_ble_register_app_sdp_common_callback(bktr_ble_gattc_sdp_comm_callback);
    bk_ble_register_app_sdp_charac_callback(bkrt_ble_gattc_sdp_charac_callback);
    return 0;
}

int32_t bk_tr_ble_client_deinit(void)
{
    os_memset(&client_init_parm, 0, sizeof(client_init_parm));
    if(read_mutex != NULL)
    {
        rtos_deinit_mutex(&read_mutex);
        read_mutex = NULL;
    }
    if(read_sema != NULL)
    {
        rtos_deinit_semaphore(&read_sema);
        read_sema = NULL;
    }
    if(write_sema != NULL)
    {
        rtos_deinit_semaphore(&write_sema);
        write_sema = NULL;
    }
    if(write_no_rsp_sema != NULL)
    {
        rtos_deinit_semaphore(&write_no_rsp_sema);
        write_no_rsp_sema = NULL;
    }
    return 0;
}
