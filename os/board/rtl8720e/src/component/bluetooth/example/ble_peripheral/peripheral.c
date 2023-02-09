/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <bt_api_config.h>
#include <rtk_service_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_device.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gatts.h>

#include <bt_utils.h>
#include <rtk_service_config.h>
#include <ble_tizenrt_service.h>


#define RTK_BT_DEV_NAME "BLE_TIZENRT"

#define BT_APP_PROCESS(process)							 \
	do {													\
		if (RTK_BT_OK != process) {						 \
			dbg("[APP] %s failed!\r\n", __FUNCTION__);   \
			return -1;									  \
		}												   \
	} while (0)


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

static rtk_bt_le_security_param_t sec_param = {
    .io_cap = RTK_IO_CAP_NO_IN_NO_OUT,
    .oob_data_flag = 0,
    .bond_flag = 1,
    .mitm_flag = 0,
    .sec_pair_flag = 0,
    .use_fixed_key = 0,
    .fixed_key = 000000,
};

// static rtk_bt_le_adv_param_t adv_param = {
// 	.interval_min = 0x30,
// 	.interval_max = 0x60,
// 	.type = RTK_BT_LE_ADV_TYPE_IND,
// 	.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
// 	.peer_addr = {
// 		.type = (rtk_bt_le_addr_type_t)0,
// 		.addr_val = {0},
// 	},
// 	.channel_map = RTK_BT_LE_ADV_CHNL_ALL,
// 	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
// };

extern trble_server_init_config server_init_parm;
static rtk_bt_evt_cb_ret_t ble_tizenrt_peripheral_gap_app_callback(uint8_t evt_code, void* param) 
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


#if 1 /* The role peripheral, no scan function, just for test here */
	case RTK_BT_LE_GAP_EVT_SCAN_START_IND: {
		rtk_bt_le_scan_start_ind_t *scan_start_ind = (rtk_bt_le_scan_start_ind_t *)param;
		if (!scan_start_ind->err) {
			dbg("[APP] Scan started, scan_type: %d\r\n", scan_start_ind->scan_type);
		} else {
			dbg("[APP] Scan start failed(err: 0x%x)\r\n", scan_start_ind->err);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_SCAN_RES_IND: {
		rtk_bt_le_scan_res_ind_t *scan_res_ind = (rtk_bt_le_scan_res_ind_t *)param;
		rtk_bt_le_addr_to_str(&(scan_res_ind->adv_report.addr), le_addr, sizeof(le_addr));
		dbg("[APP] Scan info, [Device]: %s, AD evt type: %d, RSSI: %i\r\n", 
				le_addr, scan_res_ind->adv_report.evt_type, scan_res_ind->adv_report.rssi);
		break;
	}
	
	case RTK_BT_LE_GAP_EVT_SCAN_STOP_IND: {
		rtk_bt_le_scan_stop_ind_t *scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)param;
		if (!scan_stop_ind->err) {
			dbg("[APP] Scan stopped\r\n");
		} else {
			dbg("[APP] Scan stop failed(err: 0x%x)\r\n", scan_stop_ind->err);
		}
		break;
	}
#endif

	case RTK_BT_LE_GAP_EVT_CONNECT_IND: {
		rtk_bt_le_conn_ind_t *conn_ind = (rtk_bt_le_conn_ind_t *)param;
		rtk_bt_le_addr_to_str(&(conn_ind->peer_addr), le_addr, sizeof(le_addr));
		if (!conn_ind->err) {
			role = conn_ind->role ? "slave" : "master";
			dbg("[APP] Connected, handle: %d, role: %s, remote device: %s\r\n", 
					conn_ind->conn_handle, role, le_addr);
			server_init_parm.connected_cb(conn_ind->conn_handle, TRBLE_SERVER_LL_CONNECTED, conn_ind->peer_addr.addr_val);
		} else {
			dbg("[APP] Connection establish failed(err: 0x%x), remote device: %s\r\n",
					conn_ind->err, le_addr);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_DISCONN_IND: {
		rtk_bt_le_disconn_ind_t *disconn_ind = (rtk_bt_le_disconn_ind_t *)param;
		rtk_bt_le_addr_to_str(&(disconn_ind->peer_addr), le_addr, sizeof(le_addr));
		role = disconn_ind->role ? "slave" : "master";
		dbg("[APP] Disconnected, reason: 0x%x, handle: %d, role: %s, remote device: %s\r\n", 
				disconn_ind->reason, disconn_ind->conn_handle, role, le_addr);
		server_init_parm.connected_cb(disconn_ind->conn_handle, TRBLE_SERVER_DISCONNECTED, disconn_ind->peer_addr.addr_val);
		//rtk_bt_le_gap_start_adv(&adv_param);
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND: {
		rtk_bt_le_conn_update_ind_t *conn_update_ind = 
										(rtk_bt_le_conn_update_ind_t *)param;
		if (conn_update_ind->err) {
			dbg("[APP] Update conn param failed, conn_handle: %d, err: 0x%x\r\n", 
					conn_update_ind->conn_handle, conn_update_ind->err);
		} else {
			dbg("[APP] Conn param is updated, conn_handle: %d, conn_interval: 0x%x, "	   \
					"conn_latency: 0x%x, supervision_Timeout: 0x%x\r\n",
						conn_update_ind->conn_handle, 
						conn_update_ind->conn_interval,
						conn_update_ind->conn_latency, 
						conn_update_ind->supv_timeout);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND: {
		rtk_bt_le_remote_conn_update_req_ind_t *rmt_update_req = 
								(rtk_bt_le_remote_conn_update_req_ind_t *)param;
		dbg("[APP] Remote device request a change in conn param, conn_handle: %d, "	  \
				"conn_interval_max: 0x%x, conn_interval_min: 0x%x, conn_latency: 0x%x, "	  \
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
		dbg("[APP] Data len is updated, conn_handle: %d, "	   \
				"max_tx_octets: 0x%x, max_tx_time: 0x%x, "		\
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
		APP_PROMOTE("[APP] Auth passkey display: %d, conn_handle:%d\r\n", 
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
		APP_PROMOTE("[APP] Auth passkey confirm: %d, conn_handle: %d."  \
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
			rtk_bt_le_conn_info_t conn_info;
			if (RTK_BT_OK != rtk_bt_le_gap_get_conn_info(auth_cplt_ind->conn_handle, &conn_info))
			{
				dbg("[APP] Get conn info failed! \r\n");
				break;
			}
			server_init_parm.connected_cb(auth_cplt_ind->conn_handle, TRBLE_SERVER_SM_CONNECTED, conn_info.remote.addr_val);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND: {
		rtk_bt_le_bond_modify_ind_t *bond_mdf_ind = 
										(rtk_bt_le_bond_modify_ind_t *)param;
		dbg("[APP] Bond info modified, op: %d", bond_mdf_ind->op);
		break;
	}

	default:
		debug_print("[APP] Unkown gap cb evt type: %d", evt_code);
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

static rtk_bt_evt_cb_ret_t ble_tizenrt_peripheral_gatts_app_callback(uint8_t event, void *data)
{
	if(RTK_BT_GATTS_EVT_MTU_EXCHANGE == event){
		rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
		if(p_gatt_mtu_ind->result == RTK_BT_OK){
			dbg("mtu exchange successfully: mtu_size = %d, conn_handle = %d \r\n",p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
		}else{
			dbg("mtu exchange fail \r\n");
		}
		return RTK_BT_EVT_CB_OK;
	}else {
		ble_tizenrt_srv_callback(event,data);
	}
	return RTK_BT_EVT_CB_OK;
}

extern bool rtk_bt_pre_enable(void);
int ble_tizenrt_peripheral_main(uint8_t enable)
{
	rtk_bt_app_conf_t bt_app_conf = {0};
	rtk_bt_le_addr_t bd_addr = {(rtk_bt_le_addr_type_t)0, {0}};
	char addr_str[30] = {0};
	char name[30] = {0};
	if (1 == enable)
	{
		if (rtk_bt_pre_enable() == false) {
			dbg("%s fail!\r\n", __func__);
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
		bt_app_conf.user_def_service = false;
		bt_app_conf.cccd_not_check = false;

		/* Enable BT */
		BT_APP_PROCESS(rtk_bt_enable(&bt_app_conf)); 
		BT_APP_PROCESS(rtk_bt_le_gap_get_address(&bd_addr));
		rtk_bt_le_addr_to_str(&bd_addr, addr_str, sizeof(addr_str));
		dbg("[APP] BD_ADDR: %s\r\n", addr_str);

		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GAP, ble_tizenrt_peripheral_gap_app_callback));
		BT_APP_PROCESS(rtk_bt_set_evt_cb_direct_calling(RTK_BT_LE_GP_GAP, 
														1 << RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND));
		memcpy(name,(unsigned char*)RTK_BT_DEV_NAME,strlen((const char*)RTK_BT_DEV_NAME));
		BT_APP_PROCESS(rtk_bt_le_gap_set_device_name((uint8_t *)name)); 
		BT_APP_PROCESS(rtk_bt_le_gap_set_appearance(RTK_BT_LE_GAP_APPEARANCE_HEART_RATE_BELT));
		
		BT_APP_PROCESS(rtk_bt_le_sm_set_security_param(&sec_param));
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTS, ble_tizenrt_peripheral_gatts_app_callback));
		BT_APP_PROCESS(ble_tizenrt_srv_add());
		BT_APP_PROCESS(rtk_bt_le_gap_set_adv_data(adv_data, sizeof(adv_data))); 
		BT_APP_PROCESS(rtk_bt_le_gap_set_scan_rsp_data(scan_rsp_data, sizeof(scan_rsp_data)));
		//BT_APP_PROCESS(rtk_bt_le_gap_start_adv(&adv_param)); 
	}
	else if (0 == enable)
	{
		/* no need to unreg callback here, it is done in rtk_bt_disable */
		// BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_LE_GP_GATTS));
		
		/* Disable BT */
		BT_APP_PROCESS(rtk_bt_disable());
		dbg("Peripheral deinit\r\n");
	}

	return 0;
}

