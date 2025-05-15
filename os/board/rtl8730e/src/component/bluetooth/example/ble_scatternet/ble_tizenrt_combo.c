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

#define RTK_BT_CONN_OFFSET			24		/* Connection Offset (Server) */
#define RTK_BT_MAX_TIZENRT_ADV		5		/* Maximum number of Adv */
#define RTK_BT_MAX_TIZENRT_CONN		3		/* Maximum number of Connection */

typedef struct {
	uint8_t adv_started;					/*!< Adv started */
	uint32_t pre_adv_interval;				/*!< Adv interval */
	uint32_t adv_interval;					/*!< Adv interval */
} rtk_bt_adv_status_param_t;

typedef struct {
	uint8_t connected;						/*!< Conencted */
	uint16_t conn_interval;					/*!< Connection interval */
} rtk_bt_conn_status_param_t;

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
extern int attr_counter;

rtk_bt_adv_status_param_t ble_adv_status_param[RTK_BT_MAX_TIZENRT_ADV];
rtk_bt_conn_status_param_t ble_conn_status_param[RTK_BT_MAX_TIZENRT_CONN];
rtk_bt_le_conn_ind_t *ble_tizenrt_scatternet_conn_ind = NULL;

static void *bt_service_add_task_hdl = NULL;

trble_result_e rtw_ble_combo_init(trble_client_init_config* init_client, trble_server_init_config* init_server)
{ 
    if (init_client == NULL) {
        return TRBLE_FAIL;
    } else if (is_server_init != false || client_init_parm != NULL) {
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
    client_init_parm->trble_operation_indication_cb = init_client->trble_operation_indication_cb;
    client_init_parm->trble_device_passkey_display_cb = init_client->trble_device_passkey_display_cb;
    client_init_parm->mtu = init_client->mtu;

    //init server
	server_profile_count = 0;
    if (init_server != NULL && init_server->profile != NULL && init_server->profile_count != 0) {	/* There is valid profile*/
        uint16_t gatt_char_num = 0;
        server_profile_count = init_server->profile_count;
        for (int i = 0; i < init_server->profile_count; i++) {
            if(init_server->profile[i].type == TRBLE_GATT_CHARACT) {
                gatt_char_num++;
            }
        }
        server_init_parm.profile_count = init_server->profile_count + gatt_char_num;
        server_init_parm.profile = init_server->profile;
        server_init_parm.connected_cb = init_server->connected_cb;
        server_init_parm.disconnected_cb = init_server->disconnected_cb;
        server_init_parm.mtu_update_cb = init_server->mtu_update_cb;
        server_init_parm.passkey_display_cb = init_server->passkey_display_cb;
        server_init_parm.is_secured_connect_allowed = init_server->is_secured_connect_allowed;
    }

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
	memset(&server_init_parm, 0, sizeof(server_init_parm));
	tizenrt_ble_srv_count = 0;
    is_server_init = false;

	tizenrt_remove_service();
	rtw_ble_combo_clear_status();

    attr_counter = 0;
    return TRBLE_SUCCESS; 
}

trble_result_e rtw_ble_combo_set_server_config(trble_server_init_config* init_server)
{
	uint16_t gatt_char_num = 0;

	if (false == is_server_init) {
		dbg("[APP] Server is not init\r\n");
		return TRBLE_FAIL;
	}

	if (init_server != NULL || init_server->profile != NULL || init_server->profile_count != 0) {	/* There is valid profile*/
		server_profile_count = init_server->profile_count;
		for (int i = 0; i < init_server->profile_count; i++) {
			if (init_server->profile[i].type == TRBLE_GATT_CHARACT) {
				gatt_char_num++;
			}
		}
		server_init_parm.profile_count = init_server->profile_count + gatt_char_num;
		server_init_parm.profile = init_server->profile;
		server_init_parm.connected_cb = init_server->connected_cb;
		server_init_parm.disconnected_cb = init_server->disconnected_cb;
		server_init_parm.mtu_update_cb = init_server->mtu_update_cb;
		server_init_parm.passkey_display_cb = init_server->passkey_display_cb;
		server_init_parm.is_secured_connect_allowed = init_server->is_secured_connect_allowed;

		ble_tizenrt_srv_add();	/* Add service */
	}
}

void rtw_ble_combo_add_advstatus(uint8_t adv_handle, uint32_t adv_interval)
{
	if (adv_handle < RTK_BT_MAX_TIZENRT_ADV) {
		ble_adv_status_param[adv_handle].pre_adv_interval = adv_interval;
	} else {
		dbg("[APP] Exceeded Number of Adv\r\n");
	}
	debug_print("[APP] adv_handle: %d, adv_interval: %x\r\n", adv_handle, adv_interval);
}

void rtw_ble_combo_update_advstatus(uint8_t adv_handle, uint8_t adv_start)
{
	if (adv_handle < RTK_BT_MAX_TIZENRT_CONN) {
		ble_adv_status_param[adv_handle].adv_started = adv_start;
		ble_adv_status_param[adv_handle].adv_interval = ble_adv_status_param[adv_handle].pre_adv_interval;
	} else {
		dbg("[APP] Exceeded Number of Adv\r\n");
	}
	debug_print("[APP] adv_handle: %d, adv_start: %d\r\n", adv_handle, adv_start);
}

void rtw_ble_combo_update_connnectstatus(uint8_t conn_handle, uint8_t connected, uint16_t conn_interval)
{
	uint8_t l_conn_handle = 0;

	if (conn_handle < RTK_BT_CONN_OFFSET) {
		dbg("[APP] Only record Server Connection\r\n");
		return;
	}

	l_conn_handle = conn_handle - RTK_BT_CONN_OFFSET;
	if (l_conn_handle < RTK_BT_MAX_TIZENRT_CONN) {
		ble_conn_status_param[l_conn_handle].connected = connected;
		ble_conn_status_param[l_conn_handle].conn_interval = conn_interval;
	} else {
		dbg("[APP] Exceeded Number of Connection\r\n");
	}
	debug_print("[APP] l_conn_handle: %d, connected: %d, conn_interval: %x\r\n", l_conn_handle, connected, conn_interval);
}

void rtw_ble_combo_print_ble_status(void)
{
	uint8_t l_handle = 0;

	for (l_handle = 0; l_handle < RTK_BT_MAX_TIZENRT_ADV; l_handle++) {
		if (ble_adv_status_param[l_handle].adv_started) {
			pmvdbg("[BT_PS] Adv: %d, adv_interval: %.2f ms\r\n", l_handle, (ble_adv_status_param[l_handle].adv_interval * 0.625));
		}
	}
	for (l_handle = 0; l_handle < RTK_BT_MAX_TIZENRT_CONN; l_handle++) {
		if (ble_conn_status_param[l_handle].connected) {
			pmvdbg("[BT_PS] Connection: %d, conn_interval: %.2f ms\r\n", l_handle + RTK_BT_CONN_OFFSET, (ble_conn_status_param[l_handle].conn_interval * 1.25));
		}
	}
}

void rtw_ble_combo_clear_status(void)
{
	memset(ble_adv_status_param, 0, sizeof(ble_adv_status_param));
	memset(ble_conn_status_param, 0, sizeof(ble_conn_status_param));
}

#endif /* TRBLE_COMBO_C_ */
