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

#include <bt_utils.h>
#include <rtk_service_config.h>
//#include <rtk_bas.h>
//#include <rtk_hrs.h>
//#include <rtk_simple_ble_service.h>
//#include <rtk_dis.h>
//#include <rtk_ias.h>
//#include <rtk_hids_kb.h>
//#include <rtk_gls.h>

#define RTK_BT_DEV_NAME "RTK_BT_PERIPHERAL"

#define BT_APP_PROCESS(func)                                \
	do {                                                    \
		uint16_t __func_ret = func;                         \
		if (RTK_BT_OK != __func_ret) {                      \
			printf("[APP] %s failed!!! line: %d, err: 0x%x\r\n", __func__, __LINE__, __func_ret);   \
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
	0x12,
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'R', 'T', 'K', '_', 'B', 'T', '_', 'P', 'E', 'R', 'I', 'P', 'H', 'E', 'R', 'A', 'L', 
};

static uint8_t scan_rsp_data[] = {
	0x3,
	RTK_BT_LE_GAP_ADTYPE_APPEARANCE, //GAP_ADTYPE_APPEARANCE
	LO_WORD(RTK_BT_LE_GAP_APPEARANCE_UNKNOWN),
	HI_WORD(RTK_BT_LE_GAP_APPEARANCE_UNKNOWN),
};

static rtk_bt_le_adv_param_t adv_param = {
	.interval_min = 200, //units of 0.625ms
	.interval_max = 250,
	.type = RTK_BT_LE_ADV_TYPE_IND,
	.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
	.peer_addr = {
		.type = (rtk_bt_le_addr_type_t)0,
		.addr_val = {0},
	},
	.channel_map = RTK_BT_LE_ADV_CHNL_ALL,
	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};
#endif

static rtk_bt_le_security_param_t sec_param = {
    .io_cap = RTK_IO_CAP_NO_IN_NO_OUT,
    .oob_data_flag = 0,
    .bond_flag = 1,
    .mitm_flag = 0,
    .sec_pair_flag = 0,
    .use_fixed_key = 0,
    .fixed_key = 000000,
};

#if RTK_BLE_PRIVACY_SUPPORT
static bool privacy_enable = false;
static bool privacy_whitelist = true;
static uint8_t privacy_irk[RTK_BT_LE_GAP_IRK_LEN] = "abcdef0123456789";
#endif

static void app_server_disconnect(uint16_t conn_handle)
{
	simple_ble_srv_disconnect(conn_handle);
	bas_disconnect(conn_handle);
	hrs_disconnect(conn_handle);
	gls_disconnect(conn_handle);
}

static void app_server_deinit(void)
{
	simple_ble_srv_status_deinit();
	bas_status_deinit();
	hrs_status_deinit();
	gls_status_deinit();
}

static rtk_bt_evt_cb_ret_t peripheral_gap_app_callback(uint8_t evt_code, void* param, uint32_t len) 
{
	(void)param;
	(void)len;
	rtk_bt_evt_cb_ret_t ret = RTK_BT_EVT_CB_OK;

    switch (evt_code) {
#if RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND: {
    	char addr_str[40] = {0};
		rtk_bt_ecfc_conn_cmpl_ind_t *p_ind = (rtk_bt_ecfc_conn_cmpl_ind_t *)param;
		rtk_bt_addr_to_str(p_ind->bd_type, p_ind->bd_addr, addr_str, 40);
		if(p_ind->err) {
			printf("[APP] ECFC connection established fail(0x%x). %s\r\n", p_ind->err, addr_str);
		} else {
			printf("[APP] ECFC connection established OK. %s\r\n", addr_str);
			printf("[APP] proto_id(%d) conn_handle(%d) remote_mtu(%d) local_mtu(%d) local_mps(%d)\r\n",
				   p_ind->proto_id, p_ind->conn_handle, p_ind->remote_mtu, p_ind->local_mtu, p_ind->local_mps);
			rtk_bt_gap_ecfc_print_cid(p_ind->cid, p_ind->cid_num);
		}
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_DATA_IND: {
		rtk_bt_ecfc_data_ind_t *p_ind = (rtk_bt_ecfc_data_ind_t *)param;
		printf("[APP] ECFC data received. proto_id(%d) conn_handle(%d) cid(%d) length(%d)\r\n",
			   p_ind->proto_id, p_ind->conn_handle, p_ind->cid, p_ind->length);
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_CONN_REQ_IND: {
    	char addr_str[40] = {0};
		rtk_bt_ecfc_conn_req_ind_t *p_ind = (rtk_bt_ecfc_conn_req_ind_t *)param;
		rtk_bt_addr_to_str(p_ind->bd_type, p_ind->bd_addr, addr_str, 40);
		printf("[APP] ECFC connection requset. %s\r\n", addr_str);
		printf("[APP] proto_id(%d) conn_handle(%d) remote_mtu(%d) identity_id(%d)\r\n",
			   p_ind->proto_id, p_ind->conn_handle, p_ind->remote_mtu, p_ind->identity_id);
		rtk_bt_gap_ecfc_print_cid(p_ind->cid, p_ind->cid_num);
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_DISCONN_IND: {
		rtk_bt_ecfc_disconn_ind_t *p_ind = (rtk_bt_ecfc_disconn_ind_t *)param;
		printf("[APP] ECFC disconnect. proto_id(%d) conn_handle(%d) cid(%d) cause(0x%x)\r\n",
			   p_ind->proto_id, p_ind->conn_handle, p_ind->cid, p_ind->cause);
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND: { //BT sync api shall not be called here
		rtk_bt_ecfc_reconf_req_ind_t *p_ind = (rtk_bt_ecfc_reconf_req_ind_t *)param;
		printf("[APP] ECFC reconfigure requset. proto_id(%d) conn_handle(%d) remote_mtu(%d) remote_mps(%d)\r\n",
			   p_ind->proto_id, p_ind->conn_handle, p_ind->remote_mtu, p_ind->remote_mps);
		rtk_bt_gap_ecfc_print_cid(p_ind->cid, p_ind->cid_num);
		if(p_ind->remote_mtu < RTK_BT_GAP_ECFC_MIN_MTU || p_ind->remote_mps < RTK_BT_GAP_ECFC_MIN_MPS) {
			ret = RTK_BT_EVT_CB_REJECT;
			printf("[APP] Reject!\r\n");
		} else {
			ret = RTK_BT_EVT_CB_OK;
			printf("[APP] Accept!\r\n");
		}
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_RECONF_RSP_IND: {
		rtk_bt_ecfc_reconf_rsp_ind_t *p_ind = (rtk_bt_ecfc_reconf_rsp_ind_t *)param;
		if(p_ind->cause) {
			printf("[APP] ECFC reconfigure fail\r\n");
		} else {
			printf("[APP] ECFC reconfigure OK, local_mtu(%d) local mps(%d)\r\n",
			       p_ind->local_mtu, p_ind->local_mps);
			rtk_bt_gap_ecfc_print_cid(p_ind->cid, p_ind->cid_num);
		}
		break;
	}
#endif
	default:
		printf("[APP] Unkown common gap cb evt type: %d\r\n", evt_code);
		break;
	}

	return ret;
}

static rtk_bt_evt_cb_ret_t ble_peripheral_gap_app_callback(uint8_t evt_code, void* param, uint32_t len) 
{
	(void)len;
	char le_addr[30] = {0};
	char *role;

	switch (evt_code) {
	case RTK_BT_LE_GAP_EVT_ADV_START_IND: {
		rtk_bt_le_adv_start_ind_t *adv_start_ind = (rtk_bt_le_adv_start_ind_t *)param;
		if(!adv_start_ind->err)
			printf("[APP] ADV started: adv_type %d  \r\n", adv_start_ind->adv_type);
		else
			printf("[APP] ADV start failed, err 0x%x \r\n", adv_start_ind->err);
		break;
	}

	case RTK_BT_LE_GAP_EVT_ADV_STOP_IND: {
		rtk_bt_le_adv_stop_ind_t *adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)param;
		if(!adv_stop_ind->err)
			printf("[APP] ADV stopped: reason 0x%x \r\n",adv_stop_ind->stop_reason);
		else
			printf("[APP] ADV stop failed, err 0x%x \r\n", adv_stop_ind->err);
		break;
	}

#if RTK_BLE_5_0_AE_ADV_SUPPORT
    case RTK_BT_LE_GAP_EVT_EXT_ADV_IND: {
        rtk_bt_le_ext_adv_ind_t *ext_adv_ind = (rtk_bt_le_ext_adv_ind_t *)param;
        if(!ext_adv_ind->err){
            if(ext_adv_ind->is_start)
                printf("[APP] Ext ADV(%d) started\r\n", ext_adv_ind->adv_handle);
            else
                printf("[APP] Ext ADV(%d) stopped: reason 0x%x \r\n", ext_adv_ind->adv_handle, ext_adv_ind->stop_reason);
        } else {
            if(ext_adv_ind->is_start)
                printf("[APP] Ext ADV(%d) started failed, err 0x%x\r\n", ext_adv_ind->adv_handle, ext_adv_ind->err);
            else
                printf("[APP] Ext ADV(%d) stopped failed, err 0x%x\r\n", ext_adv_ind->adv_handle, ext_adv_ind->err);
        }
        break;
    }
#endif

#if RTK_BLE_5_0_PA_ADV_SUPPORT
    case RTK_BT_LE_GAP_EVT_PA_IND: {
        rtk_bt_le_pa_ind_t *pa_ind = (rtk_bt_le_pa_ind_t *)param;
		if(pa_ind->state == RTK_BT_LE_PA_STATE_IDLE) {
			if(pa_ind->cause)
                printf("[APP] Periodic ADV(%d) stopped failed, err 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
            else
                printf("[APP] Periodic ADV(%d) stopped\r\n", pa_ind->adv_handle);
		} else if(pa_ind->state == RTK_BT_LE_PA_STATE_ADVERTISING) {
			if(pa_ind->cause)
                printf("[APP] Periodic ADV(%d) started failed, err 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
            else
                printf("[APP] Periodic ADV(%d) started\r\n", pa_ind->adv_handle);
		} else if(pa_ind->state == RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING) {
            printf("[APP] Periodic ADV(%d) waiting for ext adv start, cause 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
		}
		break;
	}
#endif

#if 1 /* The role peripheral, no scan function, just for test here */
	case RTK_BT_LE_GAP_EVT_SCAN_START_IND: {
		rtk_bt_le_scan_start_ind_t *scan_start_ind = (rtk_bt_le_scan_start_ind_t *)param;
		if (!scan_start_ind->err) {
			printf("[APP] Scan started, scan_type: %d\r\n", scan_start_ind->scan_type);
		} else {
			printf("[APP] Scan start failed(err: 0x%x)\r\n", scan_start_ind->err);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_SCAN_RES_IND: {
		rtk_bt_le_scan_res_ind_t *scan_res_ind = (rtk_bt_le_scan_res_ind_t *)param;
		rtk_bt_le_addr_to_str(&(scan_res_ind->adv_report.addr), le_addr, sizeof(le_addr));
		printf("[APP] Scan info, [Device]: %s, AD evt type: %d, RSSI: %i\r\n", 
				le_addr, scan_res_ind->adv_report.evt_type, scan_res_ind->adv_report.rssi);
		break;
	}
	
	case RTK_BT_LE_GAP_EVT_SCAN_STOP_IND: {
		rtk_bt_le_scan_stop_ind_t *scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)param;
		if (!scan_stop_ind->err) {
			printf("[APP] Scan stopped, reason: 0x%x\r\n", scan_stop_ind->stop_reason);
		} else {
			printf("[APP] Scan stop failed(err: 0x%x)\r\n", scan_stop_ind->err);
		}
		break;
	}
#endif

	case RTK_BT_LE_GAP_EVT_CONNECT_IND: {
		rtk_bt_le_conn_ind_t *conn_ind = (rtk_bt_le_conn_ind_t *)param;
		rtk_bt_le_addr_to_str(&(conn_ind->peer_addr), le_addr, sizeof(le_addr));
		if (!conn_ind->err) {
			role = conn_ind->role ? "slave" : "master";
			printf("[APP] Connected, handle: %d, role: %s, remote device: %s\r\n", 
					conn_ind->conn_handle, role, le_addr);
		} else {
			printf("[APP] Connection establish failed(err: 0x%x), remote device: %s\r\n",
					conn_ind->err, le_addr);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_DISCONN_IND: {
		rtk_bt_le_disconn_ind_t *disconn_ind = (rtk_bt_le_disconn_ind_t *)param;
		rtk_bt_le_addr_to_str(&(disconn_ind->peer_addr), le_addr, sizeof(le_addr));
		role = disconn_ind->role ? "slave" : "master";
		printf("[APP] Disconnected, reason: 0x%x, handle: %d, role: %s, remote device: %s\r\n", 
				disconn_ind->reason, disconn_ind->conn_handle, role, le_addr);

#if !RTK_BLE_5_0_AE_ADV_SUPPORT
		rtk_bt_le_gap_dev_state_t dev_state;
		if(rtk_bt_le_gap_get_dev_state(&dev_state) == RTK_BT_OK &&
		   dev_state.gap_adv_state == RTK_BT_LE_ADV_STATE_IDLE) {
#if RTK_BLE_PRIVACY_SUPPORT
			if(privacy_enable && privacy_whitelist) {
				uint8_t bond_size = 0;
				BT_APP_PROCESS(rtk_bt_le_sm_get_bond_num(&bond_size));
				if(bond_size != 0) {
#if  0 /* reconnect adv use low_duty_dir adv, and adv's own_addr use RPA */
					rtk_bt_le_bond_info_t bond_info = {0};
					uint8_t bond_num = 1;
					rtk_bt_le_sm_get_bond_info(&bond_info, &bond_num);
					adv_param.type = RTK_BT_LE_ADV_TYPE_DIRECT_IND_LOW;
					adv_param.peer_addr.type = 0;
					memcpy(adv_param.peer_addr.addr_val, bond_info.ident_addr.addr_val, RTK_BD_ADDR_LEN);
					adv_param.own_addr_type = 2;
#endif
					adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
				} else {
					adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
				}
			}
#endif
			printf("[APP] Reconnect ADV starting, adv type:%d,  own_addr_type: %d, filter_policy: %d\r\n"
					, adv_param.type,  adv_param.own_addr_type, adv_param.filter_policy);
			BT_APP_PROCESS(rtk_bt_le_gap_start_adv(&adv_param));
		}
#endif
		/* gatts action */
		app_server_disconnect(disconn_ind->conn_handle);
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND: {
		rtk_bt_le_conn_update_ind_t *conn_update_ind = 
										(rtk_bt_le_conn_update_ind_t *)param;
		if (conn_update_ind->err) {
			printf("[APP] Update conn param failed, conn_handle: %d, err: 0x%x\r\n", 
					conn_update_ind->conn_handle, conn_update_ind->err);
		} else {
			printf("[APP] Conn param is updated, conn_handle: %d, conn_interval: 0x%x, "       \
					"conn_latency: 0x%x, supervision_timeout: 0x%x\r\n",
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
		printf("[APP] Remote device request a change in conn param, conn_handle: %d, "      \
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
		printf("[APP] Data len is updated, conn_handle: %d, "       \
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
			printf("[APP] Update PHY failed, conn_handle: %d, err: 0x%x\r\n", 
					phy_update_ind->conn_handle,
					phy_update_ind->err);
		} else {
			printf("[APP] PHY is updated, conn_handle: %d, tx_phy: %d, rx_phy: %d\r\n",
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
			printf("[APP] Just work pairing auto confirm succcess\r\n");
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
			printf("[APP] Pairing failed(err: 0x%x), conn_handle: %d\r\n", 
					auth_cplt_ind->err, auth_cplt_ind->conn_handle);
		} else {
			printf("[APP] Pairing success, conn_handle: %d\r\n", auth_cplt_ind->conn_handle);
			printf("[APP] long term key is 0x");
			for (uint8_t i = 1; i <= auth_cplt_ind->dev_ltk_length; i++) {
				printf("%02x", auth_cplt_ind->dev_ltk[auth_cplt_ind->dev_ltk_length - i]); //End size conversion
			}
			printf("\r\n");
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND: {
		rtk_bt_le_bond_modify_ind_t *bond_mdf_ind = 
										(rtk_bt_le_bond_modify_ind_t *)param;
		char ident_addr[30] = {0};
		rtk_bt_le_addr_to_str(&(bond_mdf_ind->remote_addr), le_addr, sizeof(le_addr));
		rtk_bt_le_addr_to_str(&(bond_mdf_ind->ident_addr), ident_addr, sizeof(ident_addr));
		printf("[APP] Bond info modified, op: %d, addr: %s, ident_addr: %s\r\n", 
				bond_mdf_ind->op, le_addr, ident_addr);

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
		printf("[APP] Unkown gap cb evt type: %d\r\n", evt_code);
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

static rtk_bt_evt_cb_ret_t ble_peripheral_gatts_app_callback(uint8_t event, void *data, uint32_t len)
{
	(void)len;
	uint16_t app_id = 0xFFFF;

	if (RTK_BT_GATTS_EVT_MTU_EXCHANGE == event) {
		rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
		if(p_gatt_mtu_ind->result == RTK_BT_OK){
			printf("[APP] GATTS mtu exchange successfully, mtu_size: %d, conn_handle: %d \r\n",
						p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
		}else{
			printf("[APP] GATTS mtu exchange fail \r\n");
		}
		return RTK_BT_EVT_CB_OK;
	}

	if (RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES == event) {
		rtk_bt_gatts_client_supported_features_ind_t *p_ind = (rtk_bt_gatts_client_supported_features_ind_t *)data;
		if(p_ind->features & RTK_BT_GATTS_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT) {
			printf("[APP] Client Supported features is writed: conn_handle %d, features 0x%02X. Remote client supports EATT.\r\n",
                   p_ind->conn_handle, p_ind->features);
		}
		return RTK_BT_EVT_CB_OK;
	}

	app_id = app_get_gatts_app_id(event, data);

	return RTK_BT_EVT_CB_OK;
}

extern bool rtk_bt_pre_enable(void);
int ble_peripheral_main(uint8_t enable)
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

	if (1 == enable) {
		if (rtk_bt_pre_enable() == false) {
			printf("%s fail!\r\n", __func__);
			return -1;
		}

		//set GAP configuration
		bt_app_conf.app_profile_support = RTK_BT_PROFILE_GATTS;
		bt_app_conf.mtu_size = 180;
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
		printf("[APP] BD_ADDR: %s\r\n", addr_str);

		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_COMMON_GP_GAP, peripheral_gap_app_callback));
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GAP, ble_peripheral_gap_app_callback));
		memcpy(name,(const char*)RTK_BT_DEV_NAME,strlen((const char*)RTK_BT_DEV_NAME));
		BT_APP_PROCESS(rtk_bt_le_gap_set_device_name((uint8_t *)name)); 
		BT_APP_PROCESS(rtk_bt_le_gap_set_appearance(RTK_BT_LE_GAP_APPEARANCE_HEART_RATE_BELT));
		
        BT_APP_PROCESS(rtk_bt_le_sm_set_security_param(&sec_param));
#if RTK_BLE_PRIVACY_SUPPORT
		if(privacy_enable) {
			BT_APP_PROCESS(rtk_bt_le_gap_privacy_init(privacy_whitelist));
			BT_APP_PROCESS(rtk_bt_le_sm_get_bond_num(&bond_size));
			if(privacy_whitelist && bond_size != 0)
				adv_filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
		}
#endif

		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTS, ble_peripheral_gatts_app_callback));

#if RTK_BLE_5_0_AE_ADV_SUPPORT
		ext_adv_param.filter_policy = adv_filter_policy;
		BT_APP_PROCESS(rtk_bt_le_gap_create_ext_adv(&ext_adv_param, &adv_handle));
		BT_APP_PROCESS(rtk_bt_le_gap_set_ext_adv_data(adv_handle, ext_adv_data, sizeof(ext_adv_data)));
		BT_APP_PROCESS(rtk_bt_le_gap_start_ext_adv(adv_handle, 0));
#else
		BT_APP_PROCESS(rtk_bt_le_gap_set_adv_data(adv_data, sizeof(adv_data))); 
		BT_APP_PROCESS(rtk_bt_le_gap_set_scan_rsp_data(scan_rsp_data, sizeof(scan_rsp_data)));
		adv_param.filter_policy = adv_filter_policy;
		BT_APP_PROCESS(rtk_bt_le_gap_start_adv(&adv_param)); 
#endif

	} else if (0 == enable) {
		app_server_deinit();

		/* no need to unreg callback here, it is done in rtk_bt_disable */
		// BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GATTS));

		/* Disable BT */
		BT_APP_PROCESS(rtk_bt_disable());

	}

	return 0;
}

