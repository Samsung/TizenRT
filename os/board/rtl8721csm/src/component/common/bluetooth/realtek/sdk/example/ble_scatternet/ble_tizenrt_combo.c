/*
 * ble_tizenrt_combo.c
 *
 *  Created on: 2020. 3. 9.
 *      Author: lexi_zhao
 */

#ifndef TRBLE_COMBO_C_
#define TRBLE_COMBO_C_

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

extern T_TIZENRT_CLIENT_READ_RESULT ble_tizenrt_scatternet_read_results[BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS];
extern T_GCS_WRITE_RESULT g_scatternet_write_result;
extern T_GCS_WRITE_RESULT g_scatternet_write_no_rsp_result;
#if defined(CONFIG_BLE_INDICATION)
extern T_SEND_DATA_RESULT g_scatternet_indicate_result;
#endif
extern uint8_t g_master_link_num;
extern trble_client_init_config *client_init_parm;
extern T_GCS_WRITE_RESULT *write_request_result;
extern T_GCS_WRITE_RESULT *write_no_rsponse_result;
#if defined(CONFIG_BLE_INDICATION)
extern T_SEND_DATA_RESULT *send_indication_result;
#endif
extern uint8_t ble_app_link_table_size;
extern BLE_TIZENRT_BOND_REQ *ble_tizenrt_bond_req_info;
extern T_TIZENRT_CLIENT_READ_RESULT *ble_read_results;
extern BLE_TIZENRT_APP_LINK *ble_app_link_table;
extern bool (*ble_tizenrt_client_send_msg)(uint16_t sub_type, void *arg);

extern trble_server_init_config server_init_parm;
extern bool (*ble_tizenrt_server_send_msg)(uint16_t sub_type, void *arg);
extern bool is_server_init;
extern uint16_t server_profile_count;

trble_result_e rtw_ble_combo_init(trble_client_init_config* init_client, trble_server_init_config* init_server)
{ 
    if (init_client == NULL || init_server == NULL || init_server->profile == NULL \
                            || init_server->profile_count == 0)
    {
        return TRBLE_FAIL;
    } else if(is_server_init != false || client_init_parm != NULL) {
        return TRBLE_INVALID_STATE;
    }

    client_init_parm = os_mem_alloc(0, sizeof(trble_client_init_config));
    if (client_init_parm == NULL) {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }
    ble_tizenrt_bond_req_info = os_mem_alloc(0, sizeof(BLE_TIZENRT_BOND_REQ));
    if (ble_tizenrt_bond_req_info == NULL) {
        os_mem_free(client_init_parm);
        client_init_parm = NULL;
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    write_request_result = &g_scatternet_write_result;
    write_no_rsponse_result = &g_scatternet_write_no_rsp_result;
#if defined(CONFIG_BLE_INDICATION)
    send_indication_result = &g_scatternet_indicate_result;
#endif
    ble_app_link_table_size = BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS;
    memset(ble_tizenrt_bond_req_info, 0, sizeof(BLE_TIZENRT_BOND_REQ));
    ble_read_results = ble_tizenrt_scatternet_read_results;
    ble_app_link_table = ble_tizenrt_scatternet_app_link_table;
    ble_tizenrt_client_send_msg = ble_tizenrt_scatternet_send_msg;
    ble_tizenrt_server_send_msg = ble_tizenrt_scatternet_send_msg;

    //init client
    client_init_parm->trble_scan_state_changed_cb = init_client->trble_scan_state_changed_cb;
    client_init_parm->trble_device_scanned_cb = init_client->trble_device_scanned_cb;
    client_init_parm->trble_device_connected_cb = init_client->trble_device_connected_cb;
    client_init_parm->trble_device_disconnected_cb = init_client->trble_device_disconnected_cb;
    client_init_parm->trble_operation_notification_cb = init_client->trble_operation_notification_cb;
    client_init_parm->trble_operation_indication_cb = init_client->trble_operation_indication_cb;
    client_init_parm->mtu = init_client->mtu;

    //init server
    server_profile_count = init_server->profile_count;
    uint16_t gatt_char_num = 0;
    for (int i = 0; i < init_server->profile_count; i++)
    {
        if(init_server->profile[i].type == TRBLE_GATT_CHARACT)
            gatt_char_num++;
    }
    server_init_parm.profile_count = init_server->profile_count + gatt_char_num;
    server_init_parm.profile = init_server->profile;
    server_init_parm.connected_cb = init_server->connected_cb;
    server_init_parm.mtu_update_cb = init_server->mtu_update_cb;
    server_init_parm.is_secured_connect_allowed = init_server->is_secured_connect_allowed;

    ble_tizenrt_scatternet_app_init();
    is_server_init = true;
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_combo_deinit(void)
{
    ble_tizenrt_scatternet_app_deinit();

    g_master_link_num = 0;
    os_mem_free(client_init_parm);
    client_init_parm = NULL;
    os_mem_free(ble_tizenrt_bond_req_info);
    ble_tizenrt_bond_req_info = NULL;
    is_server_init = false;
    return TRBLE_SUCCESS; 
}
#endif /* TRBLE_COMBO_C_ */
