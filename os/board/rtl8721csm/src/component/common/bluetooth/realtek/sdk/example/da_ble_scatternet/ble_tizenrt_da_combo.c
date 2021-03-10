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
#include "os_mem.h"
#include "os_timer.h"
#include <gcs_client.h>

extern da_ble_client_init_parm *client_init_parm;
extern BLE_TIZENRT_BOND_REQ ble_tizenrt_bond_req_table[BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS];
extern da_ble_server_init_parm server_init_parm;
extern bool is_server_init;
extern uint16_t server_profile_count;

da_ble_result_type rtw_ble_combo_init(da_ble_client_init_parm* init_client, da_ble_server_init_parm* init_server)
{ 
    if (init_client == NULL || init_server == NULL || init_server->profile == NULL \
                            || init_server->profile_count == 0)
    {
        return DA_BLE_RESULT_TYPE_FAILURE;
    }
    if(client_init_parm == NULL)
    {
        printf("\r\n[%s] init client", __FUNCTION__);
        memset(ble_tizenrt_bond_req_table, 0, BLE_TIZENRT_SCATTERNET_APP_MAX_LINKS * sizeof(BLE_TIZENRT_BOND_REQ));
        client_init_parm = os_mem_alloc(0, sizeof(da_ble_client_init_parm));
        if (client_init_parm == NULL) {
            return DA_BLE_RESULT_TYPE_FAILURE;
        }
        client_init_parm->da_ble_client_scan_state_changed_cb = init_client->da_ble_client_scan_state_changed_cb;
        client_init_parm->da_ble_client_device_scanned_cb = init_client->da_ble_client_device_scanned_cb;
        client_init_parm->da_ble_client_device_connected_cb = init_client->da_ble_client_device_connected_cb;
        client_init_parm->da_ble_client_device_disconnected_cb = init_client->da_ble_client_device_disconnected_cb;
        client_init_parm->da_ble_client_operation_notification_cb = init_client->da_ble_client_operation_notification_cb;
    }

    if(server_profile_count == 0)
    {
        printf("\r\n[%s] init server", __FUNCTION__);
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
    }
    ble_tizenrt_scatternet_app_init();
    is_server_init = true;
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}

da_ble_result_type rtw_ble_combo_deinit(void)
{
    ble_tizenrt_scatternet_app_deinit();
    return DA_BLE_RESULT_TYPE_SUCCESS; 
}
#endif /* DA_BLE_COMBO_C_ */
