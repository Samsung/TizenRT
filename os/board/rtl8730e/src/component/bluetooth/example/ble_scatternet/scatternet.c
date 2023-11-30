/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <bt_api_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_device.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>

#include <bt_utils.h>
#include <rtk_service_config.h>
#include <rtk_client_config.h>
#include <rtk_gcs_client.h>
#include <rtk_bas_client.h>
#include <rtk_simple_ble_client.h>
#include <rtk_gaps_client.h>
#include <rtk_stack_config.h>
#include <tinyara/net/if/ble.h>
#include <ble_tizenrt_service.h>
#include <debug.h>

#define RTK_BT_DEV_NAME "BLE_TIZENRT"

#define BT_APP_PROCESS(func)                                \
	do {                                                    \
		uint16_t __func_ret = func;                         \
		if (RTK_BT_OK != __func_ret) {                      \
			dbg("[APP] %s failed! line: %d, err: 0x%x\r\n", __func__, __LINE__, __func_ret);   \
			return -1;                                      \
		}                                                   \
	} while (0)

#if RTK_BLE_5_0_AE_ADV_SUPPORT
static uint8_t ext_adv_data[] =
{
    // Flags
    0x02,
    RTK_BT_LE_GAP_ADTYPE_FLAGS,
    RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
    // Local name
	0x12,
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'R', 'T', 'K', '_', 'B', 'T', '_', 'P', 'E', 'R', 'I', 'P', 'H', 'E', 'R', 'A', 'L', 
    // Manufacturer Specific Data
    0xc3,
    RTK_BT_LE_GAP_ADTYPE_MANUFACTURER_SPECIFIC,
    0xc0, 0x00,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
    0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9,
    0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa,
    0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb,
    0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
};

static rtk_bt_le_ext_adv_param_t ext_adv_param = {
	.adv_event_prop = RTK_BT_LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED,
	.primary_adv_interval_min = 320,
	.primary_adv_interval_max = 320,
	.primary_adv_channel_map = RTK_BT_LE_ADV_CHNL_ALL,
	.own_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0}},
	.peer_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0}},//;{0x8A, 0xAA, 0xAA, 0x4C, 0xE0, 0x00},
	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
	.tx_power = 0x7F,
	.primary_adv_phy = RTK_BT_LE_PHYS_PRIM_ADV_1M,
	.secondary_adv_max_skip = 0,
	.secondary_adv_phy = RTK_BT_LE_PHYS_CODED,
	.adv_sid = 0,
};
#else
static uint8_t adv_data[] = {
	0x02, //AD len
	RTK_BT_LE_GAP_ADTYPE_FLAGS, //AD types
    RTK_BT_LE_GAP_ADTYPE_FLAGS_GENERAL | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED, //AD data
    0xc,
    RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
    'B', 'L', 'E', '_', 'T', 'I', 'Z', 'E', 'N', 'R', 'T', 
};

static uint8_t scan_rsp_data[] = {
    0x3,
    RTK_BT_LE_GAP_ADTYPE_APPEARANCE, //GAP_ADTYPE_APPEARANCE
    0x00,
    0x00,
};

// static rtk_bt_le_adv_param_t adv_param = {
//     .interval_min = 0x30,
//     .interval_max = 0x60,
//     .type = RTK_BT_LE_ADV_TYPE_IND,
//     .own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
//     .peer_addr = {
//         .type = (rtk_bt_le_addr_type_t)0,
//         .addr_val = {0},
//     },
//     .channel_map = RTK_BT_LE_ADV_CHNL_ALL,
//	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
//};
#endif

 static rtk_bt_le_scan_param_t scan_param = {
     .type          = RTK_BT_LE_SCAN_TYPE_PASSIVE,
     .interval      = 0x60,
     .window        = 0x30,
     .own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
     .filter_policy = RTK_BT_LE_SCAN_FILTER_ALLOW_ALL,
     .duplicate_opt = 0,
 };

//static rtk_bt_le_security_param_t sec_param = {
//    .io_cap = RTK_IO_CAP_NO_IN_NO_OUT,
//    .oob_data_flag = 0,
//    .bond_flag = 1,
//    .mitm_flag = 0,
//    .sec_pair_flag = 0,
//    .use_fixed_key = 0,
//    .fixed_key = 000000,
//};

#if RTK_BLE_PRIVACY_SUPPORT
static bool privacy_enable = false;
static bool privacy_whitelist = true;
static uint8_t privacy_irk[RTK_BT_LE_GAP_IRK_LEN] = "abcdef0123456789";
#endif

typedef struct {
	bool is_active;
    rtk_bt_le_link_role_t role;
} app_conn_table_t;

extern bool is_secured;
extern trble_client_init_config *client_init_parm;
extern trble_server_init_config server_init_parm;
extern TIZENERT_SRV_DATABASE tizenrt_ble_srv_database[7];
extern rtk_bt_le_conn_ind_t *ble_tizenrt_scatternet_conn_ind;
extern uint8_t *del_bond_addr;
extern uint8_t ble_client_connect_is_running;
extern void *ble_tizenrt_read_sem;
extern void *ble_tizenrt_write_sem;
extern void *ble_tizenrt_write_no_rsp_sem;
extern uint16_t scan_timeout;

static app_conn_table_t conn_link[RTK_BLE_GAP_MAX_LINKS] = {0};
rtk_bt_gattc_read_ind_t ble_tizenrt_scatternet_read_results[RTK_BLE_GAP_MAX_LINKS] = {0};
rtk_bt_gattc_write_ind_t g_scatternet_write_result = {0};
rtk_bt_gattc_write_ind_t g_scatternet_write_no_rsp_result = {0};
trble_device_connected ble_tizenrt_scatternet_bond_list[RTK_BLE_GAP_MAX_LINKS] = {0};

static rtk_bt_evt_cb_ret_t ble_tizenrt_scatternet_gap_app_callback(uint8_t evt_code, void* param, uint32_t len)
{
    char le_addr[30] = {0};
    char *role;

    switch (evt_code) {
    case RTK_BT_LE_GAP_EVT_ADV_START_IND: {
        rtk_bt_le_adv_start_ind_t *adv_start_ind = (rtk_bt_le_adv_start_ind_t *)param;
        if(!adv_start_ind->err)
            dbg("[APP] ADV started: adv_type %d  \r\n", adv_start_ind->adv_type);
        else
            dbg("[APP] ADV start failed, err 0x%x \r\n", adv_start_ind->err);
        break;
    }

    case RTK_BT_LE_GAP_EVT_ADV_STOP_IND: {
        rtk_bt_le_adv_stop_ind_t *adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)param;
        if(!adv_stop_ind->err)
            dbg("[APP] ADV stopped: reason 0x%x \r\n",adv_stop_ind->stop_reason);
        else
            dbg("[APP] ADV stop failed, err 0x%x \r\n", adv_stop_ind->err);
        break;
    }

#if RTK_BLE_5_0_AE_ADV_SUPPORT
	case RTK_BT_LE_GAP_EVT_EXT_ADV_IND: {
		rtk_bt_le_ext_adv_ind_t *ext_adv_ind = (rtk_bt_le_ext_adv_ind_t *)param;
		if(!ext_adv_ind->err){
			if(ext_adv_ind->is_start)
				dbg("[APP] Ext ADV(%d) started\r\n", ext_adv_ind->adv_handle);
			else
				dbg("[APP] Ext ADV(%d) stopped: reason 0x%x \r\n", ext_adv_ind->adv_handle, ext_adv_ind->stop_reason);
		} else {
			if(ext_adv_ind->is_start)
				dbg("[APP] Ext ADV(%d) started failed, err 0x%x\r\n", ext_adv_ind->adv_handle, ext_adv_ind->err);
			else
				dbg("[APP] Ext ADV(%d) stopped failed, err 0x%x\r\n", ext_adv_ind->adv_handle, ext_adv_ind->err);
		}
		break;
	}
#endif
	
#if RTK_BLE_5_0_PA_ADV_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_IND: {
		rtk_bt_le_pa_ind_t *pa_ind = (rtk_bt_le_pa_ind_t *)param;
		if(pa_ind->state == RTK_BT_LE_PA_STATE_IDLE) {
			if(pa_ind->cause)
				dbg("[APP] Periodic ADV(%d) stopped failed, err 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
			else
				dbg("[APP] Periodic ADV(%d) stopped\r\n", pa_ind->adv_handle);
		} else if(pa_ind->state == RTK_BT_LE_PA_STATE_ADVERTISING) {
			if(pa_ind->cause)
				dbg("[APP] Periodic ADV(%d) started failed, err 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
			else
				dbg("[APP] Periodic ADV(%d) started\r\n", pa_ind->adv_handle);
		} else if(pa_ind->state == RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING) {
				dbg("[APP] Periodic ADV(%d) waiting for ext adv start, cause 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
		}
		break;
	}
#endif

    case RTK_BT_LE_GAP_EVT_SCAN_START_IND: {
        rtk_bt_le_scan_start_ind_t *scan_start_ind = (rtk_bt_le_scan_start_ind_t *)param;
        if (!scan_start_ind->err) {
			client_init_parm->trble_scan_state_changed_cb(TRBLE_SCAN_STARTED);
            dbg("[APP] Scan started, scan_type: %d\r\n", scan_start_ind->scan_type);
        } else {
            dbg("[APP] Scan start failed(err: 0x%x)\r\n", scan_start_ind->err);
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_SCAN_RES_IND: {
        rtk_bt_le_scan_res_ind_t *scan_res_ind = (rtk_bt_le_scan_res_ind_t *)param;
        rtk_bt_le_addr_to_str(&(scan_res_ind->adv_report.addr), le_addr, sizeof(le_addr));
#if defined(CONFIG_DEBUG_SCAN_INFO)
        debug_print("[APP] Scan info, [Device]: %s, AD evt type: %d, RSSI: %i\r\n", 
                le_addr, scan_res_ind->adv_report.evt_type, scan_res_ind->adv_report.rssi);
#endif
		trble_scanned_device scanned_device;
	    scanned_device.adv_type = scan_res_ind->adv_report.evt_type;
	    if(TRBLE_ADV_TYPE_SCAN_RSP == scanned_device.adv_type){
		    memcpy(scanned_device.resp_data, scan_res_ind->adv_report.data, scan_res_ind->adv_report.len);
		    scanned_device.raw_data_length = scan_res_ind->adv_report.len;
	    }else{
		    memcpy(scanned_device.raw_data, scan_res_ind->adv_report.data, scan_res_ind->adv_report.len);
		    scanned_device.raw_data_length = scan_res_ind->adv_report.len;
	    }
	    memcpy(scanned_device.addr.mac, scan_res_ind->adv_report.addr.addr_val, RTK_BD_ADDR_LEN);
		scanned_device.addr.type = scan_res_ind->adv_report.addr.type;
	    scanned_device.rssi = scan_res_ind->adv_report.rssi;
	    client_init_parm->trble_device_scanned_cb(&scanned_device);
        break;
    }

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
		case RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND: {
			rtk_bt_le_ext_scan_res_ind_t *scan_res_ind = (rtk_bt_le_ext_scan_res_ind_t *)param;
			rtk_bt_le_addr_to_str(&(scan_res_ind->addr), le_addr, sizeof(le_addr));
			printf("[APP] Ext Scan info, [Device]: %s, AD evt type: 0x%x, RSSI: %i, PHY: 0x%x, TxPower: %d, Len: %d\r\n", 
					le_addr, scan_res_ind->evt_type, scan_res_ind->rssi,
					(scan_res_ind->primary_phy << 4) | scan_res_ind->secondary_phy,
					scan_res_ind->tx_power, scan_res_ind->len);
			break;
		}
#endif

    case RTK_BT_LE_GAP_EVT_SCAN_STOP_IND: {
        rtk_bt_le_scan_stop_ind_t *scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)param;
        if (!scan_stop_ind->err) {
			client_init_parm->trble_scan_state_changed_cb(TRBLE_SCAN_STOPPED);
            dbg("[APP] Scan stopped\r\n");
        } else {
            dbg("[APP] Scan stop failed(err: 0x%x)\r\n", scan_stop_ind->err);
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_CONNECT_IND: {
        rtk_bt_le_conn_ind_t *conn_ind = (rtk_bt_le_conn_ind_t *)param;
		memcpy(ble_tizenrt_scatternet_conn_ind, conn_ind, sizeof(rtk_bt_le_conn_ind_t));
        rtk_bt_le_addr_to_str(&(conn_ind->peer_addr), le_addr, sizeof(le_addr));
        if (!conn_ind->err) {
            role = conn_ind->role ? "slave" : "master";
            dbg("[APP] Connected, handle: %d, role: %s, remote device: %s\r\n", 
                    conn_ind->conn_handle, role, le_addr);
            uint8_t conn_id;
            rtk_bt_le_gap_get_conn_id(conn_ind->conn_handle, &conn_id);
            conn_link[conn_id].is_active = true;
            conn_link[conn_id].role = conn_ind->role;
            /* gattc action */

            if (RTK_BT_LE_ROLE_MASTER == conn_link[conn_id].role &&
                RTK_BT_OK == general_client_attach_conn(conn_ind->conn_handle)) {
                dbg("[APP] GATTC Profiles attach connection success, conn_handle: %d\r\n",
																	conn_ind->conn_handle);
            }
			if(RTK_BT_LE_ROLE_MASTER == conn_ind->role)
			{
				if(is_secured && !ble_tizenrt_scatternet_bond_list[conn_id].is_bonded)
				{
					rtk_bt_le_get_active_conn_t active_conn;
					if (RTK_BT_OK != rtk_bt_le_gap_get_active_conn(&active_conn))
					{
						dbg("[APP] Start security flow failed!");
					}
					if (active_conn.conn_num > GAP_MAX_LINKS)
					{
						dbg("[APP] Start security flow failed!");
					}
					if (RTK_BT_OK != rtk_bt_le_sm_start_security(conn_ind->conn_handle))
					{
						dbg("[APP] Start security flow failed!");
					}
				} else {
					debug_print("LL connected %d, do not need pairing \n", conn_ind->conn_handle);
					trble_device_connected connected_dev;
					uint16_t mtu_size = 0;
					if(RTK_BT_OK != rtk_bt_le_gap_get_mtu_size(conn_ind->conn_handle, &mtu_size)){
						dbg("[APP] Get mtu size failed \r\n");
					}
					connected_dev.conn_handle = conn_ind->conn_handle;
					connected_dev.is_bonded = ble_tizenrt_scatternet_bond_list[conn_id].is_bonded;
					memcpy(connected_dev.conn_info.addr.mac, conn_ind->peer_addr.addr_val, RTK_BD_ADDR_LEN);
					connected_dev.conn_info.conn_interval = conn_ind->conn_interval;
					connected_dev.conn_info.slave_latency = conn_ind->conn_latency;
					connected_dev.conn_info.scan_timeout = scan_timeout;
					connected_dev.conn_info.is_secured_connect = is_secured;
					connected_dev.conn_info.mtu = mtu_size;
					client_init_parm->trble_device_connected_cb(&connected_dev);
				}

				if(ble_client_connect_is_running)
					ble_client_connect_is_running = 0;

			}else if (RTK_BT_LE_ROLE_SLAVE == conn_ind->role) {
				server_init_parm.connected_cb(conn_ind->conn_handle, TRBLE_SERVER_LL_CONNECTED, conn_ind->peer_addr.addr_val);
			}

        } else {
            dbg("[APP] Connection establish failed(err: 0x%x), remote device: %s\r\n",
                    conn_ind->err, le_addr);
			if(ble_client_connect_is_running)
				ble_client_connect_is_running = 0;
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_DISCONN_IND: {
        rtk_bt_le_disconn_ind_t *disconn_ind = (rtk_bt_le_disconn_ind_t *)param;
        rtk_bt_le_addr_to_str(&(disconn_ind->peer_addr), le_addr, sizeof(le_addr));
        role = disconn_ind->role ? "slave" : "master";
        dbg("[APP] Disconnected, reason: 0x%x, handle: %d, role: %s, remote device: %s\r\n", 
                disconn_ind->reason, disconn_ind->conn_handle, role, le_addr);

        if(ble_client_connect_is_running)
            ble_client_connect_is_running = 0;

        if(ble_tizenrt_read_sem != NULL) {
            osif_sem_give(ble_tizenrt_read_sem);
            ble_tizenrt_read_sem = NULL;
        }
        if(ble_tizenrt_write_sem != NULL) {
            osif_sem_give(ble_tizenrt_write_sem);
            ble_tizenrt_write_sem = NULL;
        }
        if(ble_tizenrt_write_no_rsp_sem != NULL) {
            osif_sem_give(ble_tizenrt_write_no_rsp_sem);
            ble_tizenrt_write_no_rsp_sem = NULL;
        }

        uint8_t conn_id;
        rtk_bt_le_gap_get_conn_id(disconn_ind->conn_handle, &conn_id);
        memset(&conn_link[conn_id], 0, sizeof(app_conn_table_t));
        /* gattc action */
        general_client_detach_conn(disconn_ind->conn_handle);
        /* gap action */
        if(RTK_BT_LE_ROLE_MASTER == disconn_ind->role){
            client_init_parm->trble_device_disconnected_cb(disconn_ind->conn_handle);
        }else if (RTK_BT_LE_ROLE_SLAVE == disconn_ind->role) {
            server_init_parm.disconnected_cb(disconn_ind->conn_handle, disconn_ind->reason);
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND: {
        rtk_bt_le_conn_update_ind_t *conn_update_ind = 
                                        (rtk_bt_le_conn_update_ind_t *)param;
        if (conn_update_ind->err) {
            dbg("[APP] Update conn param failed, conn_handle: %d, err: 0x%x\r\n", 
                    conn_update_ind->conn_handle, conn_update_ind->err);
        } else {
            dbg("[APP] Conn param is updated, conn_handle: %d, conn_interval: 0x%x, "       \
                    "conn_latency: 0x%x, supervision_Timeout: 0x%x\r\n",
                        conn_update_ind->conn_handle, 
                        conn_update_ind->conn_interval,
                        conn_update_ind->conn_latency, 
                        conn_update_ind->supv_timeout);
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND: { //BT sync api shall not be called here
        rtk_bt_le_remote_conn_update_req_ind_t *rmt_update_req = 
                                (rtk_bt_le_remote_conn_update_req_ind_t *)param;
        dbg("[APP] Remote device request a change in conn param, conn_handle: %d, "      \
                "conn_interval_max: 0x%x, conn_interval_min: 0x%x, conn_latency: 0x%x, "      \
                "timeout: 0x%x. The host stack accept it.\r\n",
                rmt_update_req->conn_handle, 
                rmt_update_req->conn_interval_max,
                rmt_update_req->conn_interval_min,
                rmt_update_req->conn_latency,
                rmt_update_req->supv_timeout);
        return RTK_BT_EVT_CB_ACCEPT;
        break;
    }

    case RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND: {
        rtk_bt_le_data_len_change_ind_t *data_len_change = 
                                        (rtk_bt_le_data_len_change_ind_t *)param;
        dbg("[APP] Data len is updated, conn_handle: %d, "       \
                "max_tx_octets: 0x%x, max_tx_time: 0x%x, "        \
                "max_rx_octets: 0x%x, max_rx_time: 0x%x\r\n", 
                data_len_change->conn_handle,
                data_len_change->max_tx_octets, 
                data_len_change->max_tx_time,
                data_len_change->max_rx_octets, 
                data_len_change->max_rx_time);
        break;
    }

    case RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND: {
        rtk_bt_le_phy_update_ind_t *phy_update_ind = 
                                    (rtk_bt_le_phy_update_ind_t *)param;
        if (phy_update_ind->err) {
            dbg("[APP] Update PHY failed, conn_handle: %d, err: 0x%x\r\n", 
                    phy_update_ind->conn_handle,
                    phy_update_ind->err);
        } else {
            dbg("[APP] PHY is updated, conn_handle: %d, tx_phy: %d, rx_phy: %d\r\n",
                    phy_update_ind->conn_handle, 
                    phy_update_ind->tx_phy, 
                    phy_update_ind->rx_phy);
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND:{
        rtk_bt_le_auth_pair_cfm_ind_t *pair_cfm_ind = 
                                        (rtk_bt_le_auth_pair_cfm_ind_t *)param;
        APP_PROMOTE("[APP] Just work pairing need user to confirm, conn_handle: %d!\r\n", 
                                                                pair_cfm_ind->conn_handle);
        rtk_bt_le_pair_cfm_t pair_cfm_param = {0};
        uint16_t ret = 0;
        pair_cfm_param.conn_handle = pair_cfm_ind->conn_handle;
        pair_cfm_param.confirm = 1;
        ret = rtk_bt_le_sm_pairing_confirm(&pair_cfm_param);
        if (RTK_BT_OK == ret) {
            dbg("[APP] Just work pairing auto confirm succcess\r\n");
        }
        break;
    }

	case RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND:{
		rtk_bt_le_auth_key_display_ind_t *key_dis_ind =
										(rtk_bt_le_auth_key_display_ind_t *)param;
		APP_PROMOTE("[APP] Auth passkey display: %ld, conn_handle:%d\r\n", 
													key_dis_ind->passkey,
													key_dis_ind->conn_handle);
		break;
	}

    case RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND: {
        rtk_bt_le_auth_key_input_ind_t *key_input_ind = 
                                        (rtk_bt_le_auth_key_input_ind_t *)param;
        APP_PROMOTE("[APP] Please input the auth passkey get from remote, conn_handle: %d\r\n",
                                                                key_input_ind->conn_handle);
        break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND: {
		rtk_bt_le_auth_key_cfm_ind_t *key_cfm_ind = 
										(rtk_bt_le_auth_key_cfm_ind_t *)param;
		APP_PROMOTE("[APP] Auth passkey confirm: %ld, conn_handle: %d. "  \
					"Please comfirm if the passkeys are equal!\r\n",
												key_cfm_ind->passkey,
												key_cfm_ind->conn_handle);
		break;
    }

    case RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND:{
        rtk_bt_le_auth_oob_input_ind_t *oob_input_ind = 
                                        (rtk_bt_le_auth_oob_input_ind_t *)param;
        APP_PROMOTE("[APP] Bond use oob key, conn_handle: %d. Please input the oob tk \r\n",
                                                                oob_input_ind->conn_handle);
        break;
    }

    case RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND: {
        rtk_bt_le_auth_complete_ind_t *auth_cplt_ind = 
                                        (rtk_bt_le_auth_complete_ind_t *)param;
        if (auth_cplt_ind->err) {
            dbg("[APP] Pairing failed(err: 0x%x), conn_handle: %d\r\n", 
                    auth_cplt_ind->err, auth_cplt_ind->conn_handle);
        } else {
            dbg("[APP] Pairing success, conn_handle: %d\r\n", auth_cplt_ind->conn_handle);
			if(RTK_BT_LE_ROLE_MASTER == ble_tizenrt_scatternet_conn_ind->role)
			{
				uint8_t conn_id;
				rtk_bt_le_gap_get_conn_id(auth_cplt_ind->conn_handle, &conn_id);
				ble_tizenrt_scatternet_bond_list[conn_id].is_bonded = true;
				memcpy(ble_tizenrt_scatternet_bond_list[conn_id].conn_info.addr.mac, ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val, RTK_BD_ADDR_LEN);
				trble_device_connected connected_dev;
				uint16_t mtu_size = 0;
				if(RTK_BT_OK != rtk_bt_le_gap_get_mtu_size(auth_cplt_ind->conn_handle, &mtu_size)){
					dbg("[APP] Get mtu size failed \r\n");
				}
				connected_dev.conn_handle = ble_tizenrt_scatternet_conn_ind->conn_handle;
				connected_dev.is_bonded = ble_tizenrt_scatternet_bond_list[conn_id].is_bonded;
				memcpy(connected_dev.conn_info.addr.mac, ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val, RTK_BD_ADDR_LEN);
				connected_dev.conn_info.conn_interval = ble_tizenrt_scatternet_conn_ind->conn_interval;
				connected_dev.conn_info.slave_latency = ble_tizenrt_scatternet_conn_ind->conn_latency;
				connected_dev.conn_info.scan_timeout = scan_timeout;
				connected_dev.conn_info.is_secured_connect = is_secured;
				connected_dev.conn_info.mtu = mtu_size;
				client_init_parm->trble_device_connected_cb(&connected_dev);
			}else if(RTK_BT_LE_ROLE_SLAVE == ble_tizenrt_scatternet_conn_ind->role)
			{
				server_init_parm.connected_cb(auth_cplt_ind->conn_handle, TRBLE_SERVER_SM_CONNECTED, ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val);				
			}
        }
        break;
    }

    case RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND: {
        rtk_bt_le_bond_modify_ind_t *bond_mdf_ind = 
                                        (rtk_bt_le_bond_modify_ind_t *)param;
        dbg("[APP] Bond info modified, op: %d", bond_mdf_ind->op);
		if(RTK_BT_LE_BOND_DELETE == bond_mdf_ind->op && del_bond_addr){
			for(int i = 0; i < GAP_MAX_LINKS; i++)
			{
				if(!memcmp(ble_tizenrt_scatternet_bond_list[i].conn_info.addr.mac, del_bond_addr, RTK_BD_ADDR_LEN))
				{
					ble_tizenrt_scatternet_bond_list[i].is_bonded = false;
					memset(ble_tizenrt_scatternet_bond_list[i].conn_info.addr.mac, 0, RTK_BD_ADDR_LEN);
					break;
				}
			}			
		}else if(RTK_BT_LE_BOND_CLEAR == bond_mdf_ind->op){
			for(int i = 0; i < GAP_MAX_LINKS; i++)
			{
				ble_tizenrt_scatternet_bond_list[i].is_bonded = false;
				memset(ble_tizenrt_scatternet_bond_list[i].conn_info.addr.mac, 0, RTK_BD_ADDR_LEN);
			}				
		}
        break;
    }

#if RTK_BLE_PRIVACY_SUPPORT
	case RTK_BT_LE_GAP_EVT_RESOLV_LIST_MODIFY_IND: {
		rtk_bt_le_modify_resolv_list_ind_t *p_ind = (rtk_bt_le_modify_resolv_list_ind_t *)param;
		rtk_bt_le_addr_t addr;

		if(p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_ADD || p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_REMOVE) {
			addr.type = (rtk_bt_le_addr_type_t)p_ind->entry.addr_type;
			memcpy(addr.addr_val, p_ind->entry.addr, RTK_BD_ADDR_LEN);
			rtk_bt_le_addr_to_str(&addr, le_addr, sizeof(le_addr));
			if(p_ind->err) {
				printf("[APP] Resolving list %s %s fail, cause:%x.\r\n",
					   (p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_ADD)?"add":"remove",
					   le_addr, p_ind->err);
			} else {
				printf("[APP] Resolving list %s %s success, %s privacy mode.\r\n", 
					   (p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_ADD)?"add":"remove",
					   le_addr, p_ind->entry.device_mode?"device":"network");
			}
		} else if (p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_CLEAR){
			if(p_ind->err)
				printf("[APP] Resolving list clear fail, cause:%x.\r\n", p_ind->err);
			else
				printf("[APP] Resolving list clear success.\r\n");
		}
		break;
	}
#endif

#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND: {
		rtk_bt_le_pa_sync_ind_t *pa_sync_ind = (rtk_bt_le_pa_sync_ind_t *)param;
		printf("[APP] PA sync state change: sync_id: %d, state = %d, cause: 0x%x\r\n",
			   pa_sync_ind->sync_id, pa_sync_ind->state, pa_sync_ind->cause);

		if(pa_sync_ind->state == RTK_BT_LE_PA_SYNC_STATE_SYNCHRONIZED) {
			rtk_bt_le_pa_sync_sync_param_t sync_param;
			if(RTK_BT_OK == rtk_bt_le_gap_pa_sync_get_param(RTK_BT_LE_PA_SYNC_PARAM_SYNCHRONIZED_PARAM,
															&sync_param, pa_sync_ind->sync_id)) {
				rtk_bt_le_addr_t addr;
				addr.type = sync_param.adv_addr_type;
				memcpy(addr.addr_val, sync_param.adv_addr, RTK_BD_ADDR_LEN);
				rtk_bt_le_addr_to_str(&addr, le_addr, sizeof(le_addr));
				printf("[APP] PA SYNCHRONIZED PARAM: [Device]: %s, sync_handle:0x%x, adv_sid:%d, skip:%d, sync_timeout:%d, ", 
						le_addr, sync_param.sync_handle, sync_param.adv_sid, sync_param.skip, sync_param.sync_timeout);
				printf("sync_cte_type:%d, adv_phy:%d, adv_clock_accuracy:0x%x, ", 
						sync_param.sync_cte_type, sync_param.adv_phy, sync_param.adv_clock_accuracy);
				printf("periodic_adv_interval:%d, sync_transfer_received_flag:%d\r\n",	
						sync_param.periodic_adv_interval, sync_param.sync_transfer_received_flag);
			}
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND: {
		rtk_bt_le_pa_adv_report_ind_t *pa_report = (rtk_bt_le_pa_adv_report_ind_t *)param;
		printf("[APP] PA sync ADV report: sync_id %d, sync_handle 0x%x, tx_power %d, rssi %d, cte_type %d, data_status 0x%x, data_len %d\r\n",
				pa_report->sync_id, pa_report->sync_handle, pa_report->tx_power, pa_report->rssi,
				pa_report->cte_type, pa_report->data_status, pa_report->data_len);
		break;
	}
#endif
	
#if RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
		case RTK_BT_LE_GAP_PAST_RECEIVED_INFO_IND: {
			rtk_bt_le_past_recv_ind_t *past_recv_ind = (rtk_bt_le_past_recv_ind_t *)param;
			printf("[APP] PAST received info: conn_handle %d, service_data %d\r\n",
					past_recv_ind->conn_handle, past_recv_ind->service_data);
			break;
		}
#endif
	
#if RTK_BLE_5_2_POWER_CONTROL_SUPPORT
		case RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND: {
			rtk_bt_le_txpower_ind_t *txpower_ind = (rtk_bt_le_txpower_ind_t *)param;
			printf("[APP] TX power report: conn_handle %d, type %d, txpower %d\r\n",
					txpower_ind->conn_handle, txpower_ind->type, txpower_ind->txpower);
			break;
	}
#endif

    default:
        debug_print("[APP] Unkown gap cb evt type: %d", evt_code);
        break;
	}

    return RTK_BT_EVT_CB_OK;
}

static uint16_t app_get_gatts_app_id(uint8_t event, void *data)
{
	uint16_t app_id = 0xFFFF;

	switch (event) {
	case RTK_BT_GATTS_EVT_REGISTER_SERVICE: {
		rtk_bt_gatts_reg_ind_t *p_reg_ind = (rtk_bt_gatts_reg_ind_t *)data;
		app_id = p_reg_ind->app_id;
        break;
    }
    case RTK_BT_GATTS_EVT_READ_IND: {
        rtk_bt_gatts_read_ind_t *p_read_ind = (rtk_bt_gatts_read_ind_t *)data;
        app_id = p_read_ind->app_id;
        break;
    }
    case RTK_BT_GATTS_EVT_WRITE_IND: {
        rtk_bt_gatts_write_ind_t *p_write_ind = (rtk_bt_gatts_write_ind_t *)data;
        app_id = p_write_ind->app_id;
        break;
    }
    case RTK_BT_GATTS_EVT_CCCD_IND: {
        rtk_bt_gatts_cccd_ind_t *p_cccd_ind = (rtk_bt_gatts_cccd_ind_t *)data;
        app_id = p_cccd_ind->app_id;
        break;
    }
    case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND:
    case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND: {
        rtk_bt_gatts_ntf_and_ind_ind_t *p_ind_ntf = (rtk_bt_gatts_ntf_and_ind_ind_t *)data;
        app_id = p_ind_ntf->app_id;
        break;
    }
    default:
        break; 
    }

    return app_id;
}

static rtk_bt_evt_cb_ret_t ble_tizenrt_scatternet_gatts_app_callback(uint8_t event, void *data, uint32_t len)
{
	/* printf("ble_peripheral_gatts_service_callback: app_id = %d, HID_SRV_ID = %d \r\n",app_id,HID_SRV_ID); */

    if (RTK_BT_GATTS_EVT_MTU_EXCHANGE == event) {
        rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
        if(p_gatt_mtu_ind->result == RTK_BT_OK){
            dbg("[APP] GATTS mtu exchange successfully, mtu_size: %d, conn_handle: %d \r\n",
                        p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
            if(RTK_BT_LE_ROLE_SLAVE == ble_tizenrt_scatternet_conn_ind->role){
                server_init_parm.mtu_update_cb( p_gatt_mtu_ind->conn_handle, p_gatt_mtu_ind->mtu_size);
            }
        }else{
            dbg("[APP] GATTS mtu exchange fail \r\n");
        }
        return RTK_BT_EVT_CB_OK;
    }else{
		ble_tizenrt_srv_callback(event,data);
    }

    return RTK_BT_EVT_CB_OK;
}

static uint16_t app_get_gattc_profile_id(uint8_t event, void *data)
{
	uint16_t profile_id = 0xFFFF;

	switch (event) {
	case RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND: {
		rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;
		profile_id = disc_res->profile_id;
		break;
    }
    case RTK_BT_GATTC_EVT_READ_RESULT_IND: {
        rtk_bt_gattc_read_ind_t *read_res = (rtk_bt_gattc_read_ind_t *)data;
        profile_id = read_res->profile_id;
        break;
    }
    case RTK_BT_GATTC_EVT_WRITE_RESULT_IND: {
        rtk_bt_gattc_write_ind_t *write_res = (rtk_bt_gattc_write_ind_t *)data;
        profile_id = write_res->profile_id;
        break;
    }
    case RTK_BT_GATTC_EVT_NOTIFY_IND: 
    case RTK_BT_GATTC_EVT_INDICATE_IND: {
        rtk_bt_gattc_cccd_value_ind_t *cccd_ind = (rtk_bt_gattc_cccd_value_ind_t *)data;
        profile_id = cccd_ind->profile_id;
        break;
    }
    case RTK_BT_GATTC_EVT_CCCD_ENABLE_IND:
    case RTK_BT_GATTC_EVT_CCCD_DISABLE_IND: {
        rtk_bt_gattc_cccd_update_ind_t *cccd_update = (rtk_bt_gattc_cccd_update_ind_t *)data;
        profile_id = cccd_update->profile_id;
        break;
    }
    default:
        break;
    }

    return profile_id;
}

static rtk_bt_evt_cb_ret_t ble_tizenrt_scatternet_gattc_app_callback(uint8_t event, void *data, uint32_t len)
{
	uint16_t profile_id = 0xFFFF;

	if (RTK_BT_GATTC_EVT_MTU_EXCHANGE == event) {
		rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
		if(p_gatt_mtu_ind->result == RTK_BT_OK){
			dbg("[APP] GATTC mtu exchange success, mtu_size: %d, conn_handle: %d \r\n",
						p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
		}else{
			dbg("[APP] GATTC mtu exchange fail \r\n");
		}
		return RTK_BT_EVT_CB_OK;
	}

#if RTK_BLE_MGR_LIB
	if (RTK_BT_GATTC_EVT_DISCOVER_ALL_STATE_IND == event) {
		rtk_bt_gattc_discover_all_state_ind_t *p_ind = (rtk_bt_gattc_discover_all_state_ind_t *)data;
		printf("[APP] GATTC discover all finished: conn_handle: %d, is_success: %d, load_from_storage: %d\r\n",
				p_ind->conn_handle, p_ind->is_success, p_ind->load_from_storage);
		
		return RTK_BT_EVT_CB_OK;
	}

	if (RTK_BT_GATTC_EVT_GATT_SERVICE_INFO_IND == event) {
		rtk_bt_gattc_gatt_service_info_ind_t *p_ind = (rtk_bt_gattc_gatt_service_info_ind_t *)data;
		printf("[APP] GATTC discover GATT service info: conn_handle: %d, is_found: %d, load_from_storage: %d\r\n",
				p_ind->conn_handle, p_ind->is_found, p_ind->load_from_storage);
		if((p_ind->char_flag & RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG) &&
		   (p_ind->server_features[0] & RTK_BT_GATTS_SERVER_SUPPORTED_FEATURES_EATT_BIT))
			printf("[APP] Server supported features: 0x%02X, support EATT.\r\n", p_ind->server_features[0]);
		
		return RTK_BT_EVT_CB_OK;
	}
#endif

	profile_id = app_get_gattc_profile_id(event, data);
	switch (profile_id) {
	case GCS_CLIENT_PROFILE_ID:
		general_client_app_callback(event, data);
		break;
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

extern bool rtk_bt_pre_enable(void);
int ble_tizenrt_scatternet_main(uint8_t enable)
{
    rtk_bt_app_conf_t bt_app_conf = {0};
    rtk_bt_le_addr_t bd_addr = {(rtk_bt_le_addr_type_t)0, {0}};
	rtk_bt_le_adv_filter_t adv_filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
    char addr_str[30] = {0};
    char name[30] = {0};

#if RTK_BLE_5_0_AE_ADV_SUPPORT
		uint8_t adv_handle;
#endif
#if RTK_BLE_PRIVACY_SUPPORT
		uint8_t bond_size = 0;
#endif

	if (1 == enable)
	{
        if (rtk_bt_pre_enable() == false) {
            dbg("%s fail!\r\n", __func__);
            return -1;
        }

        //set GAP configuration
		bt_app_conf.app_profile_support = RTK_BT_PROFILE_GATTS | RTK_BT_PROFILE_GATTC;
		bt_app_conf.mtu_size = 512;
		bt_app_conf.master_init_mtu_req = true;
		bt_app_conf.prefer_all_phy = 0;
		bt_app_conf.prefer_tx_phy = 1 | 1<<1 | 1<<2;
		bt_app_conf.prefer_rx_phy = 1 | 1<<1 | 1<<2;
		bt_app_conf.max_tx_octets = 0x40;
		bt_app_conf.max_tx_time = 0x200;
#if RTK_BLE_PRIVACY_SUPPORT
		memcpy(bt_app_conf.irk, privacy_irk, RTK_BT_LE_GAP_IRK_LEN);
#endif
		bt_app_conf.user_def_service = false;
		bt_app_conf.cccd_not_check = false;

        /* Enable BT */
		
		BT_APP_PROCESS(rtk_bt_enable(&bt_app_conf)); 
        BT_APP_PROCESS(rtk_bt_le_gap_get_address(&bd_addr));
        rtk_bt_le_addr_to_str(&bd_addr, addr_str, sizeof(addr_str));
        dbg("[APP] BD_ADDR: %s\r\n", addr_str);
		BT_APP_PROCESS(rtk_bt_le_gap_set_scan_param(&scan_param));
        BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GAP, 
                                                    (rtk_bt_evt_cb_t)ble_tizenrt_scatternet_gap_app_callback));
        memcpy(name,(const uint8_t*)RTK_BT_DEV_NAME,strlen((const char *)RTK_BT_DEV_NAME));
		BT_APP_PROCESS(rtk_bt_le_gap_set_device_name((uint8_t *)name)); 
        BT_APP_PROCESS(rtk_bt_le_gap_set_appearance(RTK_BT_LE_GAP_APPEARANCE_HEART_RATE_BELT));
        BT_APP_PROCESS(rtk_bt_le_gap_set_adv_data(adv_data,sizeof(adv_data))); 
        BT_APP_PROCESS(rtk_bt_le_gap_set_scan_rsp_data(scan_rsp_data,sizeof(scan_rsp_data)));

#if RTK_BLE_PRIVACY_SUPPORT
		if(privacy_enable) {
			BT_APP_PROCESS(rtk_bt_le_gap_privacy_init(privacy_whitelist));
			BT_APP_PROCESS(rtk_bt_le_sm_get_bond_num(&bond_size));
			if(privacy_whitelist && bond_size != 0)
				adv_filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
		}
#endif

        /* gatts related */
        BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTS, 
                                                    (rtk_bt_evt_cb_t)ble_tizenrt_scatternet_gatts_app_callback));
		BT_APP_PROCESS(ble_tizenrt_srv_add());
        /* gattc related */
        BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTC, 
                                                    (rtk_bt_evt_cb_t)ble_tizenrt_scatternet_gattc_app_callback));
        BT_APP_PROCESS(general_client_add());
	}
	else if (0 == enable)
    {
		BT_APP_PROCESS(general_client_delete());

        /* no need to unreg callback here, it is done in rtk_bt_disable */
        // BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GAP));
        // BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GATTS));
        // BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GATTC));

        /* Disable BT */
        BT_APP_PROCESS(rtk_bt_disable());

    }

    return 0;
}

