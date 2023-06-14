/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <osif.h>

#include <bt_api_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gattc.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_device.h>
#include <bt_utils.h>

#include <rtk_client_config.h>
#include <rtk_gcs_client.h>
#include <rtk_gaps_client.h>
#include <rtk_bas_client.h>
#include <rtk_simple_ble_client.h>

#define BT_APP_PROCESS(func)                                \
	do {                                                    \
		uint16_t __func_ret = func;                         \
		if (RTK_BT_OK != __func_ret) {                      \
			printf("[APP] %s failed! line: %d, err: 0x%x\r\n", __func__, __LINE__, __func_ret);   \
			return -1;                                      \
		}                                                   \
	} while (0)

static rtk_bt_evt_cb_ret_t central_gap_app_callback(uint8_t evt_code, void* param, uint32_t len) 
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

static rtk_bt_evt_cb_ret_t ble_central_gap_app_callback(uint8_t evt_code, void *param, uint32_t len)
{
	(void)len;
	char le_addr[30] = {0};
	char *role;

	switch (evt_code) {
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
		printf("[APP] Scan info, [Device]: %s, AD evt type: %d, RSSI: %i, len: %d \r\n", 
				le_addr, scan_res_ind->adv_report.evt_type, scan_res_ind->adv_report.rssi,
				scan_res_ind->adv_report.len);
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
			printf("[APP] Scan stopped, reason: 0x%x\r\n", scan_stop_ind->stop_reason);
		} else {
			printf("[APP] Scan stop failed(err: 0x%x)\r\n", scan_stop_ind->err);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONNECT_IND: {
		rtk_bt_le_conn_ind_t *conn_ind = (rtk_bt_le_conn_ind_t *)param;
		rtk_bt_le_addr_to_str(&(conn_ind->peer_addr), le_addr, sizeof(le_addr));
		if (!conn_ind->err) {
			role = conn_ind->role ? "slave" : "master";
			printf("[APP] Connected, handle: %d, role: %s, remote device: %s\r\n", 
					conn_ind->conn_handle, role, le_addr);

#if !RTK_BLE_MGR_LIB
			/* when connection established, attach the profile(who has been registered) 
			   to this connection */
			if (RTK_BT_OK == general_client_attach_conn(conn_ind->conn_handle) &&
				RTK_BT_OK == bas_client_attach_conn(conn_ind->conn_handle) &&
				RTK_BT_OK == gaps_client_attach_conn(conn_ind->conn_handle) &&
				RTK_BT_OK == simple_ble_client_attach_conn(conn_ind->conn_handle)) {
				printf("[APP] GATTC Profiles attach connection success, conn_handle: %d\r\n",
																	conn_ind->conn_handle);
			}
#endif
			if (rtk_bt_le_sm_is_device_bonded(&conn_ind->peer_addr)) {
				printf("[APP] Bonded device, start link encryption procedure\r\n");
				rtk_bt_le_sm_start_security(conn_ind->conn_handle);
			}
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
		if (RTK_BT_OK == general_client_detach_conn(disconn_ind->conn_handle) &&
			RTK_BT_OK == bas_client_detach_conn(disconn_ind->conn_handle) &&
			RTK_BT_OK == gaps_client_detach_conn(disconn_ind->conn_handle) &&
			RTK_BT_OK == simple_ble_client_detach_conn(disconn_ind->conn_handle)) {
			printf("[APP] GATTC Profiles detach connection success, conn_handle: %d\r\n",
																disconn_ind->conn_handle);
		}
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

static rtk_bt_evt_cb_ret_t ble_central_gattc_app_callback(uint8_t event, void *data, uint32_t len)
{
	(void)len;
	uint16_t profile_id = 0xFFFF;

	if (RTK_BT_GATTC_EVT_MTU_EXCHANGE == event) {
		rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
		if(p_gatt_mtu_ind->result == RTK_BT_OK){
			printf("[APP] GATTC mtu exchange success, mtu_size: %d, conn_handle: %d \r\n",
						p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
		}else{
			printf("[APP] GATTC mtu exchange fail \r\n");
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
			printf("[APP] Server supported features: 0x%02X, Remote server supports EATT.\r\n", p_ind->server_features[0]);
		
		return RTK_BT_EVT_CB_OK;
	}
#endif

	profile_id = app_get_gattc_profile_id(event, data);
	switch (profile_id) {
	case GCS_CLIENT_PROFILE_ID:
		general_client_app_callback(event, data);
		break;
	case GAPS_CLIENT_PROFILE_ID:
		gaps_client_app_callback(event, data);
		break;
	case BAS_CLIENT_PROFILE_ID:
		bas_client_app_callback(event, data);
		break;
	case SIMPLE_BLE_CLIENT_PROFILE_ID:
		simple_ble_client_app_callback(event, data);
		break;
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

static rtk_bt_le_scan_param_t scan_param = {
	.type          = RTK_BT_LE_SCAN_TYPE_ACTIVE,
	.interval      = 0x60,
	.window        = 0x30,
	.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
	.filter_policy = RTK_BT_LE_SCAN_FILTER_ALLOW_ALL,
	.duplicate_opt = 0,
};

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
static uint8_t privacy_irk[RTK_BT_LE_GAP_IRK_LEN] = "0123456789abcdef";
#endif

extern bool rtk_bt_pre_enable(void);
int ble_central_main(uint8_t enable)
{
	rtk_bt_app_conf_t bt_app_conf = {0};
	rtk_bt_le_addr_t bd_addr = {(rtk_bt_le_addr_type_t)0, {0}};
	char addr_str[30] = {0};

	if (1 == enable || 2 == enable)
	{
		if (rtk_bt_pre_enable() == false) {
			printf("%s fail!\r\n", __func__);
			return -1;
		}

		//set GAP configuration
		bt_app_conf.app_profile_support = RTK_BT_PROFILE_GATTC;
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

		BT_APP_PROCESS(rtk_bt_le_gap_set_scan_param(&scan_param));
		/* Initilize GAP part */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_COMMON_GP_GAP, central_gap_app_callback));
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GAP, ble_central_gap_app_callback));

        BT_APP_PROCESS(rtk_bt_le_sm_set_security_param(&sec_param));
#if RTK_BLE_PRIVACY_SUPPORT
		if(privacy_enable) {
			BT_APP_PROCESS(rtk_bt_le_gap_privacy_init(privacy_whitelist));
		}
#endif

		/* Initilize GATT (client) part */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTC, ble_central_gattc_app_callback));
		BT_APP_PROCESS(general_client_add());
		BT_APP_PROCESS(bas_client_add());
		BT_APP_PROCESS(gaps_client_add());
		BT_APP_PROCESS(simple_ble_client_add());
	} else if (0 == enable) {
		BT_APP_PROCESS(general_client_delete());
		BT_APP_PROCESS(bas_client_delete());
		BT_APP_PROCESS(gaps_client_delete());
		BT_APP_PROCESS(simple_ble_client_delete());

		/* no need to unreg callback here, it is done in rtk_bt_disable */
		// BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GAP));
		// BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GATTC));

		/* Disable BT */
		BT_APP_PROCESS(rtk_bt_disable());
	}

	return 0;
}

