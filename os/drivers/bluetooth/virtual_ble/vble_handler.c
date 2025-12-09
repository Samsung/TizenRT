/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/types.h>
#include <tinyara/config.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <time.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/ble.h>
#include "vble_handler.h"

trble_client_init_config client_init_parm;
trble_server_init_config server_init_parm;
bool is_server_init;
bool is_client_init;
bool is_scan_started;
uint16_t server_profile_count;

ble_scan_whitelist g_scan_whitelist[SCAN_WHITELIST_SIZE] = {0, };
trble_connected_list vble_connected_list;
trble_scanned_device vble_scanned_items;
trble_scan_filter g_filter;
trble_device_connected vble_connect;
trble_device_connected connected_device[TRBLE_MAX_CONNECTION_COUNT];
trble_operation_handle vble_op_handle;
static int g_vble_result = TRBLE_SUCCESS;
static uint8_t data[] = {
    0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x05, 0x03, 0x12, 0x18, 0x0f, 0x18
};
trble_data vble_op_data;
vattr_table attr_table;

static inline void vdev_reverse_mac(uint8_t *mac)
{
    int i;
    int j;
    uint8_t temp;

    for (i = 0, j = TRBLE_BD_ADDR_MAX_LEN - 1; i < j; i++, j--) {
        temp = mac[i];
        mac[i] = mac[j];
        mac[j] = temp;
    }
    return;
}

static void _whitelist_delete(trble_addr *addr)
{
    int i;
    for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
        if (memcmp(g_scan_whitelist[i].addr.mac, addr->mac, TRBLE_BD_ADDR_MAX_LEN) == 0) {
            g_scan_whitelist[i].use = SCAN_WHITELIST_EMPTY;
            memset(&g_scan_whitelist[i], 0, sizeof(ble_scan_whitelist));
            return;
        }
    }
}

static void _whitelist_add(trble_addr *addr)
{
    int i;
    for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
        if (g_scan_whitelist[i].use != SCAN_WHITELIST_EMPTY) {
            continue;
        }
        g_scan_whitelist[i].use = SCAN_WHITELIST_IN_USE;
        memcpy(g_scan_whitelist[i].addr.mac, addr->mac, TRBLE_BD_ADDR_MAX_LEN);
        return;
    }
}

static void vble_callback_handler(lwnl_cb_status evt, int result)
{
    switch(evt.evt) {
    case LWNL_EVT_BLE_CLIENT_CONNECT:
        vble_connected_list.connected_count++;
        client_init_parm.trble_device_connected_cb(&vble_connect);
        break;
    case LWNL_EVT_BLE_CLIENT_DISCONNECT:
        vble_connected_list.connected_count--;
        vble_connected_list.conn_handle[vble_connect.conn_handle] = VBLE_NO_CONNECTION;
        client_init_parm.trble_device_disconnected_cb(vble_connect.conn_handle);
        break;
    case LWNL_EVT_BLE_CLIENT_NOTI:
        client_init_parm.trble_operation_notification_cb(&vble_op_handle, &vble_op_data);
        break;
    case LWNL_EVT_BLE_SCAN_STATE:
        client_init_parm.trble_scan_state_changed_cb(result);
        if (result == TRBLE_SCAN_STOPPED) {
            break;
        }
        if (g_filter.whitelist_enable != 0) {
            int i;
            for (i = 0; i < SCAN_WHITELIST_SIZE; i++) {
                if (g_scan_whitelist[i].use != SCAN_WHITELIST_EMPTY) {
                    memcpy(vble_scanned_items.addr.mac, g_scan_whitelist[i].addr.mac, TRBLE_BD_ADDR_MAX_LEN);
                    client_init_parm.trble_device_scanned_cb(&vble_scanned_items);
                }
            }
        } else {
            uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
            memcpy(vble_scanned_items.addr.mac, dummy, TRBLE_BD_ADDR_MAX_LEN);
            client_init_parm.trble_device_scanned_cb(&vble_scanned_items);
            vble_scanned_items.addr.mac[5] = 0xF4;
            client_init_parm.trble_device_scanned_cb(&vble_scanned_items);
            vble_scanned_items.addr.mac[5] = 0xF5;
            client_init_parm.trble_device_scanned_cb(&vble_scanned_items);
        }
        break;
    }
}

static void _generate_evt(int argc, char *argv[])
{
    lwnl_cb_status event_type = {LWNL_DEV_BLE, 0};
    event_type.evt = atoi(argv[1]);
    int result = atoi(argv[2]);
    int sleep_time = atoi(argv[3]);

    VBLE_LOG("[VBLE] sleep (%d) result (%d)event type(%d)\n",
                 sleep_time, result, event_type.evt);

    sleep(sleep_time);
    vble_callback_handler(event_type, result);
}

int vble_create_event(uint32_t event, int32_t result, int32_t sleep)
{
    char event_buf[16] = {0,};
    char res_buf[16] = {0,};
    char sleep_buf[16] = {0,};

    memset(event_buf, 0, 16);
    memset(res_buf, 0, 16);
    memset(sleep_buf, 0, 16);

    snprintf(event_buf, 16, "%d", event);
    snprintf(res_buf, 16, "%d", result);
    snprintf(sleep_buf, 16, "%d", sleep);

    char *argv[4] = {0,};
    argv[0] = event_buf;
    argv[1] = res_buf;
    argv[2] = sleep_buf;
    argv[3] = NULL;
    int res = kernel_thread("vble_evt", 100, 2048, _generate_evt, argv);
    if (res == -1) {
        VBLE_ERROR(0);
        return -1;
    }
    return 0;
}

static void init_handler(struct vconfigs *config) {
    //init client
    client_init_parm.trble_scan_state_changed_cb = config->client->trble_scan_state_changed_cb;
    client_init_parm.trble_device_scanned_cb = config->client->trble_device_scanned_cb;
    client_init_parm.trble_device_connected_cb = config->client->trble_device_connected_cb;
    client_init_parm.trble_device_disconnected_cb = config->client->trble_device_disconnected_cb;
    client_init_parm.trble_operation_notification_cb = config->client->trble_operation_notification_cb;
    client_init_parm.mtu = config->client->mtu;

    //init server
    server_profile_count = config->server->profile_count;
    uint16_t gatt_char_num = 0;
    for (int i = 0; i < config->server->profile_count; i++)
    {
        if (config->server->profile[i].type == TRBLE_GATT_CHARACT) {
            gatt_char_num++;
        }
    }
    server_init_parm.profile_count = config->server->profile_count + gatt_char_num;
    server_init_parm.profile = config->server->profile;
    server_init_parm.connected_cb = config->server->connected_cb;
    server_init_parm.is_secured_connect_allowed = config->server->is_secured_connect_allowed;
    is_server_init = true;
    is_client_init = true;
    for (uint16_t i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++)
    {
        vble_connected_list.conn_handle[i] = VBLE_NO_CONNECTION;
    }
    vble_connected_list.connected_count = 0;
    attr_table.count = 0;
    return;
}

static void connect_handler(trble_conn_info *conn_info) {
    vble_connect.conn_info = *conn_info;
    vdev_reverse_mac(vble_connect.conn_info.addr.mac);
    uint8_t index = vble_connected_list.connected_count;
    connected_device[index].conn_info = *conn_info;
    connected_device[index].is_bonded = conn_info->is_secured_connect;
    connected_device[index].conn_handle = index;
    vble_connected_list.conn_handle[index] = index;
    return;
}

static void disconnect_all_handler(void) {
    vble_connected_list.connected_count = 0;
    bool check = false;
    for (int i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++)
    {
        if (vble_connected_list.conn_handle[i] != VBLE_NO_CONNECTION) {
            check = true;
            vble_connected_list.conn_handle[i] = VBLE_NO_CONNECTION;
        }
    }
    client_init_parm.trble_device_disconnected_cb(0);
    return;
}

static void connected_device_list_handler(trble_connected_list *out_connected_list) {
    memset(out_connected_list, 0, sizeof(trble_connected_list));
    out_connected_list->connected_count = vble_connected_list.connected_count;
    for (int i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++)
    {
        out_connected_list->conn_handle[i] = vble_connected_list.conn_handle[i];
    }
    return;
}

static void ioctl_handler(trble_msg_s *msg) {
    vble_ioctl_msg_s *vmsg = (vble_ioctl_msg_s *)msg->data;
    if (!vmsg) {
        VBLE_ERROR(0);
        return;
    }
    switch ((int)msg->cmd) {
    case VBLE_CMD_SET:
        g_vble_result = vmsg->value;
        break;
    case VBLE_CMD_GEN_EVT:
        vble_create_event(vmsg->key, vmsg->value, vmsg->wait);
        break;
    }
    return;
}

int vble_handle_message(struct vble_req *req)
{
    int res = 0;
    switch(req->type) {
    case VBLE_MSG_INIT:
        VBLE_LOG("[VBLE] init \n");
        struct vconfigs *config = (struct vconfigs *)req->arg;
        init_handler(config);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_DEINIT:
        VBLE_LOG("[VBLE] deinit \n");
        is_server_init = false;
        is_client_init = false;
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_GET_MAC_ADDR:
        VBLE_LOG("[VBLE] Get Bonded device\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_GET_BONDED_DEVICE:
        VBLE_LOG("[VBLE] Get Bonded device\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_DELETE_BONDED:
        VBLE_LOG("[VBLE] Delete Bonded\n");
        trble_addr *addr = (trble_addr *)req->arg;
        for (int i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++)
        {
            if (memcmp(connected_device[i].conn_info.addr.mac, addr->mac, BLE_BD_ADDR_MAX_LEN) == 0) {
                connected_device[i].is_bonded = false;
            }
        }
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_DELETE_BONDED_ALL:
        VBLE_LOG("[VBLE] Delete Bonded all\n");
        for (int i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++)
        {
            connected_device[i].is_bonded = false;
        }
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CONN_IS_ACTIVE:
        VBLE_LOG("[VBLE] conn is active\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CONN_IS_ANY_ACTIVE:
        VBLE_LOG("[VBLE] conn is any active\n");
        bool *is_active = (bool *)req->arg;
        *is_active = false;
        for (int i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++) {
            if (vble_connected_list.conn_handle[i] != VBLE_NO_CONNECTION) {
                *is_active = true;
                break;
            }
        }
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_IOCTL:
        VBLE_LOG("[VBLE] ioctl call\n");
        trble_msg_s *msg = (trble_msg_s *)req->arg;
        ioctl_handler(msg);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CLIENT_CONNECT:
        VBLE_LOG("[VBLE] connect request\n");
        trble_conn_info *conn_info = (trble_conn_info *)req->arg;
        connect_handler(conn_info);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CLIENT_DISCONNECT:
        VBLE_LOG("[VBLE] disconnect request\n");
        trble_conn_handle *conn_handle = (trble_conn_handle *)req->arg;
        vble_connect.conn_handle = *conn_handle;
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CLIENT_DISCONNECT_ALL:
        VBLE_LOG("[VBLE] disconnect all request\n");
        disconnect_all_handler();
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CONNECTED_DEVICE_LIST:
        VBLE_LOG("[VBLE] connected device list\n");
        trble_connected_list *out_connected_list = (trble_connected_list *)req->arg;
        connected_device_list_handler(out_connected_list);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CONNECTED_INFO:
        VBLE_LOG("[VBLE] connected info\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_START_SCAN:
        VBLE_LOG("[VBLE] scan start\n");
        trble_scan_filter *filter = (trble_scan_filter *)req->arg;
        g_filter = *filter;
        is_scan_started = true;
        vble_scanned_items.adv_type = BLE_ADV_TYPE_IND;
        vble_scanned_items.raw_data_length = g_filter.raw_data_length;
        memcpy(vble_scanned_items.raw_data, g_filter.raw_data, TRBLE_ADV_RAW_DATA_MAX_LEN);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_STOP_SCAN:
        VBLE_LOG("[VBLE] scan stop\n");
        is_scan_started = false;
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SCAN_WHITELIST_ADD:
        VBLE_LOG("[VBLE] whitelist add\n");
        trble_addr *t_addr = (trble_addr *)req->arg;
        _whitelist_add(t_addr);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SCAN_WHITELIST_DELETE:
        VBLE_LOG("[VBLE] whitelist delete\n");
        _whitelist_delete((trble_addr *)req->arg);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SCAN_WHITELIST_CLEAR_ALL:
        VBLE_LOG("[VBLE] whitelist clear all\n");
        for (int i = 0; i < SCAN_WHITELIST_SIZE; i++) {
            memset(&g_scan_whitelist[i], 0, sizeof(ble_scan_whitelist));
        }
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_OPERATION_ENABLE_NOTIFICATION:
        VBLE_LOG("[VBLE] operation enable notification\n");
        trble_operation_handle *handle = (trble_operation_handle *)req->arg;
        vble_op_handle = *handle;
        vble_op_data.data = data;
        vble_op_data.length = sizeof(data);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_ATTR_SET_DATA:
        VBLE_LOG("[VBLE] attr set data\n");
        vattr *attr_data_s = (vattr *)req->arg;
        attr_table.attr_data[attr_table.count] = *attr_data_s;
        attr_table.count++;
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_ATTR_GET_DATA:
        VBLE_LOG("[VBLE] attr get data\n");
        vattr *attr_data_g = (vattr *)req->arg;
        for (int i=0; i < attr_table.count; ++i) {
            if (attr_table.attr_data[i].attr_handle == attr_data_g->attr_handle) {
                attr_data_g->data = attr_table.attr_data[i].data;
                break;
            }
        }
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_ATTR_REJECT:
        VBLE_LOG("[VBLE] attr reject\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SERVER_DISCONNECT:
        VBLE_LOG("[VBLE] server disconnect\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_GET_MAC_ADDR_BY_CONN_HANDLE:
        VBLE_LOG("[VBLE] mac addr by conn_handle\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_GET_CONN_HANDLE_BY_ADDR:
        VBLE_LOG("[VBLE] conn_handle by mac addr\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_OPERATION_READ:
        VBLE_LOG("[VBLE] operation read\n");
        trble_data *out_data = (trble_data *)req->arg;
        memcpy(out_data->data, data, TRBLE_ADV_RAW_DATA_MAX_LEN);
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_OPERATION_WRITE:
        VBLE_LOG("[VBLE] operation write\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_OPERATION_WRITE_NO_RESPONSE:
        VBLE_LOG("[VBLE] operation write no resp\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_GET_PROFILE_COUNT:
        VBLE_LOG("[VBLE] profile count\n");
        uint16_t *count = (uint16_t *)req->arg;
        *count = server_init_parm.profile_count;
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_CHARACTER_NOTIFY:
        VBLE_LOG("[VBLE] character notify\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SET_ADV_DATA:
        VBLE_LOG("[VBLE] adv data\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SET_ADV_RESP:
        VBLE_LOG("[VBLE] adv resp\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SET_ADV_TYPE:
        VBLE_LOG("[VBLE] adv type\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_SET_ADV_INTERVAL:
        VBLE_LOG("[VBLE] adv interval\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_START_ADV:
        VBLE_LOG("[VBLE] start adv\n");
        *req->res = g_vble_result;
        break;
    case VBLE_MSG_STOP_ADV:
        VBLE_LOG("[VBLE] stop adv\n");
        *req->res = g_vble_result;
        break;
    default:
        return TRBLE_UNKNOWN;
    }
    return res;
}