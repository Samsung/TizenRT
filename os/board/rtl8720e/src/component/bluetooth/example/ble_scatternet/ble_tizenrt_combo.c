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
#include <rtk_bt_gattc.h>
#include <rtk_bt_le_gap.h>
#include <rtk_client_config.h>
#include <rtk_stack_config.h>
#include <bt_utils.h>
#include <ble_tizenrt_service.h>

extern rtk_bt_gattc_read_ind_t ble_tizenrt_scatternet_read_results[GAP_MAX_LINKS];
extern rtk_bt_gattc_write_ind_t g_scatternet_write_result;
extern rtk_bt_gattc_write_ind_t g_scatternet_write_no_rsp_result;

extern rtk_bt_gattc_read_ind_t *ble_read_results;
extern rtk_bt_gattc_write_ind_t *ble_write_request_result;
extern rtk_bt_gattc_write_ind_t *ble_write_no_rsp_result;

extern trble_client_init_config *client_init_parm;
extern rtk_bt_gattc_write_ind_t *ble_write_request_result;
extern rtk_bt_gattc_write_ind_t *ble_write_no_rsp_result;
extern rtk_bt_gattc_read_ind_t *ble_read_results ;

extern trble_server_init_config server_init_parm;
extern bool is_server_init;
extern uint16_t server_profile_count;
extern T_ATTRIB_APPL *tizenrt_ble_service_tbl;
extern TIZENERT_SRV_CNT tizenrt_ble_srv_count;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];

rtk_bt_le_conn_ind_t *ble_tizenrt_scatternet_conn_ind = NULL;

trble_result_e rtw_ble_combo_init(trble_client_init_config* init_client, trble_server_init_config* init_server)
{ 
    if (init_client == NULL || init_server == NULL || init_server->profile == NULL \
                            || init_server->profile_count == 0)
    {
        return TRBLE_FAIL;
    } else if(is_server_init != false || client_init_parm != NULL) {
        return TRBLE_INVALID_STATE;
    }

    client_init_parm = (trble_client_init_config *)osif_mem_alloc(0, sizeof(trble_client_init_config));
    if (client_init_parm == NULL) {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    ble_tizenrt_scatternet_conn_ind = (rtk_bt_le_conn_ind_t *)osif_mem_alloc(0, sizeof(rtk_bt_le_conn_ind_t));
    if (ble_tizenrt_scatternet_conn_ind == NULL) {
        debug_print("Memory allocation failed \n");
        return TRBLE_FAIL;
    }

    ble_write_request_result = &g_scatternet_write_result;
    ble_write_no_rsp_result = &g_scatternet_write_no_rsp_result;
    ble_read_results = ble_tizenrt_scatternet_read_results;

    //init client
    client_init_parm->trble_scan_state_changed_cb = init_client->trble_scan_state_changed_cb;
    client_init_parm->trble_device_scanned_cb = init_client->trble_device_scanned_cb;
    client_init_parm->trble_device_connected_cb = init_client->trble_device_connected_cb;
    client_init_parm->trble_device_disconnected_cb = init_client->trble_device_disconnected_cb;
    client_init_parm->trble_operation_notification_cb = init_client->trble_operation_notification_cb;
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
    server_init_parm.is_secured_connect_allowed = init_server->is_secured_connect_allowed;

    ble_tizenrt_scatternet_main(1);
    is_server_init = true;
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_combo_deinit(void)
{
    ble_tizenrt_scatternet_main(0);

    osif_mem_free(client_init_parm);
    client_init_parm = NULL;

    osif_mem_free(ble_tizenrt_scatternet_conn_ind);
    ble_tizenrt_scatternet_conn_ind = NULL;

	osif_mem_free(tizenrt_ble_service_tbl);
	memset(tizenrt_ble_srv_database, 0, (7 * sizeof(TIZENERT_SRV_DATABASE)));
	tizenrt_ble_srv_count = 0;
    is_server_init = false;
    return TRBLE_SUCCESS; 
}

#endif /* TRBLE_COMBO_C_ */
