/*
 * ble_da_ble_combo.c
 *
 *  Created on: 2020. 3. 9.
 *      Author: lexi_zhao
 */

#ifndef DA_BLE_COMBO_C_
#define DA_BLE_COMBO_C_

#include "da_ble_client.h"
#include "da_ble_server.h"
#include "stddef.h"
#include <gap_conn_le.h>
#include "ble_tizenrt_scatternet_app.h"
#include "ble_tizenrt_scatternet_link_mgr.h"
#include "ble_tizenrt_central_client_app.h"
#include "ble_tizenrt_central_link_mgr.h"
#include "os_mem.h"
#include "os_timer.h"
#include <gcs_client.h>

extern T_TIZENRT_CLIENT_READ_RESULT ble_tizenrt_scatternet_read_results[BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS];
extern T_GCS_WRITE_RESULT g_scatternet_write_result;
extern T_GCS_WRITE_RESULT g_scatternet_write_no_rsp_result;

extern da_ble_client_init_parm *client_init_parm;
extern T_GCS_WRITE_RESULT *write_request_result;
extern T_GCS_WRITE_RESULT *write_no_rsponse_result;
extern uint8_t ble_app_link_table_size;
extern BLE_TIZENRT_BOND_REQ *ble_tizenrt_bond_req_table;
extern T_TIZENRT_CLIENT_READ_RESULT *ble_read_results;
extern BLE_TIZENRT_APP_LINK *da_ble_app_link_table;
extern void (*ble_tizenrt_client_send_msg)(uint16_t sub_type, void *arg);

extern da_ble_server_init_parm server_init_parm;
extern void (*ble_tizenrt_server_send_msg)(uint16_t sub_type, void *arg);
extern bool is_server_init;
extern uint16_t server_profile_count;

da_ble_result_type rtw_ble_combo_init(da_ble_client_init_parm* init_client, da_ble_server_init_parm* init_server)
{ 
    if (init_client == NULL || init_server == NULL || init_server->profile == NULL \
                            || init_server->profile_count == 0)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    } else if(is_server_init != false || client_init_parm != NULL) {
        return DA_BLE_RESULT_TYPE_INVALID_STATE;
    }

    write_request_result = &g_scatternet_write_result;
    write_no_rsponse_result = &g_scatternet_write_no_rsp_result;
    ble_app_link_table_size = BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS;
    ble_tizenrt_bond_req_table = os_mem_alloc(0, BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS * sizeof(BLE_TIZENRT_BOND_REQ));
    memset(ble_tizenrt_bond_req_table, 0, BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS * sizeof(BLE_TIZENRT_BOND_REQ));
    ble_read_results = ble_tizenrt_scatternet_read_results;
    da_ble_app_link_table = ble_tizenrt_scatternet_app_link_table;
    ble_tizenrt_client_send_msg = ble_tizenrt_scatternet_send_msg;
    ble_tizenrt_server_send_msg = ble_tizenrt_scatternet_send_msg;
    
    //init client
    debug_print("\r\n[%s] init client", __FUNCTION__);
    client_init_parm = os_mem_alloc(0, sizeof(da_ble_client_init_parm));
    if (client_init_parm == NULL) {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }
    client_init_parm->da_ble_client_scan_state_changed_cb = init_client->da_ble_client_scan_state_changed_cb;
    client_init_parm->da_ble_client_device_scanned_cb = init_client->da_ble_client_device_scanned_cb;
    client_init_parm->da_ble_client_device_connected_cb = init_client->da_ble_client_device_connected_cb;
    client_init_parm->da_ble_client_device_disconnected_cb = init_client->da_ble_client_device_disconnected_cb;
    client_init_parm->da_ble_client_operation_notification_cb = init_client->da_ble_client_operation_notification_cb;

    //init server
    debug_print("\r\n[%s] init server", __FUNCTION__);
    server_profile_count = init_server->profile_count;
    uint16_t gatt_char_num = 0;
    for (int i = 0; i < init_server->profile_count; i++)
    {
        if(init_server->profile[i].type == DA_BLE_SERVER_GATT_CHARACT)
            gatt_char_num++;
    }
    server_init_parm.profile_count = init_server->profile_count + gatt_char_num;
    server_init_parm.profile = init_server->profile;
    server_init_parm.connected_cb = init_server->connected_cb;
    server_init_parm.is_secured_connect_allowed = init_server->is_secured_connect_allowed;

    ble_tizenrt_scatternet_app_init();
    is_server_init = true;
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_combo_deinit(void)
{
    ble_tizenrt_scatternet_app_deinit();

    os_mem_free(client_init_parm);
    client_init_parm = NULL;
    os_mem_free(ble_tizenrt_bond_req_table);
    ble_tizenrt_bond_req_table = NULL;
    is_server_init = false;
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}
#endif /* DA_BLE_COMBO_C_ */
