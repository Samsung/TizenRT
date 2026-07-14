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
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>

#include <rtk_service_config.h>
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include <rtk_stack_config.h>
#include <tinyara/net/if/ble.h>
#include <ble_tizenrt_service.h>
#include <tinyara/config.h>
#else
#include <rtk_bas.h>
#include <rtk_hrs.h>
#include <rtk_simple_ble_service.h>
#include <rtk_dis.h>
#include <rtk_ias.h>
#include <rtk_hids_kb.h>
#include <rtk_gls.h>
#include <rtk_long_uuid_service.h>
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include <rtk_client_config.h>
#include <bt_utils.h>

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define RTK_BT_DEV_NAME "BLE_TIZENRT"
#else
#define RTK_BT_DEV_NAME "RTK_BT_SCATTERNET"
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
#ifndef CONFIG_PLATFORM_TIZENRT_OS
static uint8_t ext_adv_data[] = {
	// Flags
	0x02,
	RTK_BT_LE_GAP_ADTYPE_FLAGS,
	RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
	// Local name
	0x12,
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'R', 'T', 'K', '_', 'B', 'T', '_', 'S', 'C', 'A', 'T', 'T', 'E', 'R', 'N', 'E', 'T',
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
};

static rtk_bt_le_ext_adv_param_t ext_adv_param = {
	.adv_event_prop = RTK_BT_LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED,
	.primary_adv_interval_min = 320,
	.primary_adv_interval_max = 360,
	.primary_adv_channel_map = RTK_BT_LE_ADV_CHNL_ALL,
	.own_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0}},
	.peer_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0}},//;{0x8A, 0xAA, 0xAA, 0x4C, 0xE0, 0x00},
	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
	.tx_power = 0x7F,
	.primary_adv_phy = RTK_BT_LE_PHYS_PRIM_ADV_1M,
	.secondary_adv_max_skip = 0,
	.secondary_adv_phy = RTK_BT_LE_PHYS_2M,
	.adv_sid = 0,
};
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
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
	LO_WORD(RTK_BT_LE_GAP_APPEARANCE_UNKNOWN),
	HI_WORD(RTK_BT_LE_GAP_APPEARANCE_UNKNOWN),
};

#ifndef CONFIG_PLATFORM_TIZENRT_OS
static rtk_bt_le_adv_param_t def_adv_param = {
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
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#endif

static rtk_bt_le_scan_param_t scan_param = {
	.type = RTK_BT_LE_SCAN_TYPE_ACTIVE,
	.interval = 0x60,
	.window = 0x30,
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
	.sec_pair_only_flag = 0,
	.use_fixed_key = 0,
	.fixed_key = 000000,
	.auto_sec_req = 0,
};

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
#define PRIVACY_USE_DIR_ADV_WHEN_BONDED    0
static bool privacy_enable = false;
static bool privacy_whitelist = true;
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
extern bool is_secured;
extern trble_client_init_config *client_init_parm;
extern trble_server_init_config server_init_parm;
extern rtk_bt_le_conn_ind_t *ble_tizenrt_scatternet_conn_ind;
extern uint8_t *del_bond_addr;
extern uint8_t ble_client_connect_is_running;
extern uint16_t scan_timeout;
extern trble_le_coc_init_config le_coc_init_parm;
rtk_bt_gattc_read_ind_t ble_tizenrt_scatternet_read_results[RTK_BLE_GAP_MAX_LINKS] = {0};
rtk_bt_gattc_write_ind_t g_scatternet_write_result = {0};
rtk_bt_gattc_write_ind_t g_scatternet_write_no_rsp_result = {0};
trble_device_connected ble_tizenrt_scatternet_bond_list[RTK_BLE_GAP_MAX_LINKS] = {0};

static void ble_tizenrt_dummy_callback(void)
{
	BT_LOGA("[APP] Application Dummy Callback API\r\n");
}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#if defined(RTK_BT_POWER_CONTROL_SUPPORT) && RTK_BT_POWER_CONTROL_SUPPORT
#define BT_WAKE_UP_HOST         0    //If set to 1, WAKE_SRC_BT_WAKE_HOST should be set to wakeup AP core in ameba_sleepcfg.c
#if defined(BT_WAKE_UP_HOST) && BT_WAKE_UP_HOST
#include "rtk_bt_power_control.h"

static void bt_wake_up_host_suspend(void)
{
	BT_LOGA("[BT_PS] Enter bt_wake_up_host_suspend\r\n");
}

static void bt_wake_up_host_resume(void)
{
	BT_LOGA("[BT_PS] Enter bt_wake_up_host_resume\r\n");

	/* Our demo releases BT wake lock here, it can be released anywhere as application's request */
	rtk_bt_release_wakelock();
}

static void bt_wake_up_host_init(void)
{
	rtk_bt_power_save_init((rtk_bt_ps_callback)bt_wake_up_host_suspend, (rtk_bt_ps_callback)bt_wake_up_host_resume);
}

static void bt_wake_up_host_deinit(void)
{
	rtk_bt_power_save_deinit();
}
#endif
#endif

static void app_server_disconnect(uint16_t conn_handle)
{
	simple_ble_srv_disconnect(conn_handle);
	bas_disconnect(conn_handle);
	hrs_disconnect(conn_handle);
	gls_disconnect(conn_handle);
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
	cte_srv_disconnect(conn_handle);
#endif
}

static void app_server_deinit(void)
{
	simple_ble_srv_status_deinit();
	bas_status_deinit();
	hrs_status_deinit();
	gls_status_deinit();
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
	cte_srv_status_deinit();
#endif
}

extern trble_result_e trble_netmgr_fw_reset(void);
static rtk_bt_evt_cb_ret_t scatternet_gap_app_callback(uint8_t evt_code, void *param, uint32_t len)
{
	(void)param;
	(void)len;
	rtk_bt_evt_cb_ret_t ret = RTK_BT_EVT_CB_OK;

	switch (evt_code) {
#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND: {
		char addr_str[40] = {0};
		rtk_bt_ecfc_conn_cmpl_ind_t *p_ind = (rtk_bt_ecfc_conn_cmpl_ind_t *)param;
		rtk_bt_addr_to_str(p_ind->bd_type, p_ind->bd_addr, addr_str, 40);
		if (p_ind->err) {
			BT_LOGE("[APP] ECFC connection established fail(0x%x). %s\r\n", p_ind->err, addr_str);
		} else {
			BT_LOGA("[APP] ECFC connection established OK. %s\r\n", addr_str);
			BT_LOGA("[APP] proto_id(%d) conn_handle(%d) remote_mtu(%d) local_mtu(%d) local_mps(%d)\r\n",
					p_ind->proto_id, p_ind->conn_handle, p_ind->remote_mtu, p_ind->local_mtu, p_ind->local_mps);
			BT_DUMP16A("[APP] cid: ", p_ind->cid, p_ind->cid_num);
		}
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_DATA_IND: {
		rtk_bt_ecfc_data_ind_t *p_ind = (rtk_bt_ecfc_data_ind_t *)param;
		BT_LOGA("[APP] ECFC data received. proto_id(%d) conn_handle(%d) cid(%d) length(%d)\r\n",
				p_ind->proto_id, p_ind->conn_handle, p_ind->cid, p_ind->length);
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_CONN_REQ_IND: {
		char addr_str[40] = {0};
		rtk_bt_ecfc_conn_req_ind_t *p_ind = (rtk_bt_ecfc_conn_req_ind_t *)param;
		rtk_bt_addr_to_str(p_ind->bd_type, p_ind->bd_addr, addr_str, 40);
		BT_LOGA("[APP] ECFC connection requset. %s\r\n", addr_str);
		BT_LOGA("[APP] proto_id(%d) conn_handle(%d) remote_mtu(%d) identity_id(%d)\r\n",
				p_ind->proto_id, p_ind->conn_handle, p_ind->remote_mtu, p_ind->identity_id);
		BT_DUMP16A("[APP] cid: ", p_ind->cid, p_ind->cid_num);
		rtk_bt_gap_ecfc_conn_cfm_t cfm_param = {0};
		uint16_t ret = 0;
		cfm_param.conn_handle = p_ind->conn_handle;
		cfm_param.identity_id = p_ind->identity_id;
		cfm_param.cause = 2;
		cfm_param.cid_num = p_ind->cid_num;
		memcpy(cfm_param.p_cid, p_ind->cid, p_ind->cid_num * sizeof(uint16_t));
		cfm_param.local_mtu = p_ind->remote_mtu;
		ret = rtk_bt_gap_ecfc_send_conn_cfm(&cfm_param);
		if (RTK_BT_OK == ret) {
			BT_LOGA("[APP] ECFC auto conn confirm success\r\n");
		}
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_DISCONN_IND: {
		rtk_bt_ecfc_disconn_ind_t *p_ind = (rtk_bt_ecfc_disconn_ind_t *)param;
		BT_LOGA("[APP] ECFC disconnect. proto_id(%d) conn_handle(%d) cid(%d) cause(0x%x)\r\n",
				p_ind->proto_id, p_ind->conn_handle, p_ind->cid, p_ind->cause);
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND: { //BT api shall not be called here
		rtk_bt_ecfc_reconf_req_ind_t *p_ind = (rtk_bt_ecfc_reconf_req_ind_t *)param;
		BT_LOGA("[APP] ECFC reconfigure requset. proto_id(%d) conn_handle(%d) remote_mtu(%d) remote_mps(%d)\r\n",
				p_ind->proto_id, p_ind->conn_handle, p_ind->remote_mtu, p_ind->remote_mps);
		BT_DUMP16A("[APP] cid: ", p_ind->cid, p_ind->cid_num);
		if (p_ind->remote_mtu < RTK_BT_GAP_ECFC_MIN_MTU || p_ind->remote_mps < RTK_BT_GAP_ECFC_MIN_MPS) {
			ret = RTK_BT_EVT_CB_REJECT;
			BT_LOGE("[APP] Reject!\r\n");
		} else {
			ret = RTK_BT_EVT_CB_OK;
			BT_LOGA("[APP] Accept!\r\n");
		}
		break;
	}
	case RTK_BT_GAP_EVT_ECFC_RECONF_RSP_IND: {
		rtk_bt_ecfc_reconf_rsp_ind_t *p_ind = (rtk_bt_ecfc_reconf_rsp_ind_t *)param;
		if (p_ind->cause) {
			BT_LOGE("[APP] ECFC reconfigure fail\r\n");
		} else {
			BT_LOGA("[APP] ECFC reconfigure OK, local_mtu(%d) local mps(%d)\r\n",
					p_ind->local_mtu, p_ind->local_mps);
			BT_DUMP16A("[APP] cid: ", p_ind->cid, p_ind->cid_num);
		}
		break;
	}
#endif
	case RTK_BT_GAP_EVT_FW_EXCEPTION_IND: {
		trble_netmgr_fw_reset();
		break;
	}
	default:
		BT_LOGE("[APP] Unknown common gap cb evt type: %d\r\n", evt_code);
		break;
	}

	return ret;
}

static rtk_bt_evt_cb_ret_t ble_scatternet_gap_app_callback(uint8_t evt_code, void *param, uint32_t len)
{
	(void)len;
	char le_addr[30] = {0};
	char *role;

	switch (evt_code) {
	case RTK_BT_LE_GAP_EVT_ADV_START_IND: {
		rtk_bt_le_adv_start_ind_t *adv_start_ind = (rtk_bt_le_adv_start_ind_t *)param;
		if (!adv_start_ind->err) {
			BT_LOGA("[APP] ADV started: adv_type %d  \r\n", adv_start_ind->adv_type);
		} else {
			BT_LOGE("[APP] ADV start failed, err 0x%x \r\n", adv_start_ind->err);
		}
		BT_AT_PRINT("+BLEGAP:adv,start,%d,%d\r\n", (adv_start_ind->err == 0) ? 0 : -1, adv_start_ind->adv_type);
		break;
	}

	case RTK_BT_LE_GAP_EVT_ADV_STOP_IND: {
		rtk_bt_le_adv_stop_ind_t *adv_stop_ind = (rtk_bt_le_adv_stop_ind_t *)param;
		if (!adv_stop_ind->err) {
			BT_LOGA("[APP] ADV stopped: reason 0x%x \r\n", adv_stop_ind->stop_reason);
		} else {
			BT_LOGE("[APP] ADV stop failed, err 0x%x \r\n", adv_stop_ind->err);
		}
		BT_AT_PRINT("+BLEGAP:adv,stop,%d,0x%x\r\n", (adv_stop_ind->err == 0) ? 0 : -1, adv_stop_ind->stop_reason);
		break;
	}

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	case RTK_BT_LE_GAP_EVT_EXT_ADV_IND: {
		rtk_bt_le_ext_adv_ind_t *ext_adv_ind = (rtk_bt_le_ext_adv_ind_t *)param;
		if (!ext_adv_ind->err) {
			if (ext_adv_ind->is_start) {
				BT_LOGA("[APP] Ext ADV(%d) started\r\n", ext_adv_ind->adv_handle);
			} else {
				BT_LOGA("[APP] Ext ADV(%d) stopped: reason 0x%x \r\n", ext_adv_ind->adv_handle, ext_adv_ind->stop_reason);
			}
		} else {
			if (ext_adv_ind->is_start) {
				BT_LOGE("[APP] Ext ADV(%d) started failed, err 0x%x\r\n", ext_adv_ind->adv_handle, ext_adv_ind->err);
			} else {
				BT_LOGE("[APP] Ext ADV(%d) stopped failed, err 0x%x\r\n", ext_adv_ind->adv_handle, ext_adv_ind->err);
			}
		}
		BT_AT_PRINT("+BLEGAP:eadv,%s,%d,%d\r\n",
					ext_adv_ind->is_start ? "start" : "stop",
					(ext_adv_ind->err == 0) ? 0 : -1,
					ext_adv_ind->adv_handle);
		break;
	}
#endif

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_IND: {
		rtk_bt_le_pa_ind_t *pa_ind = (rtk_bt_le_pa_ind_t *)param;
		if (pa_ind->state == RTK_BT_LE_PA_STATE_IDLE) {
			if (pa_ind->cause) {
				BT_LOGE("[APP] Periodic ADV(%d) stopped failed, err 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
			} else {
				BT_LOGA("[APP] Periodic ADV(%d) stopped\r\n", pa_ind->adv_handle);
			}
		} else if (pa_ind->state == RTK_BT_LE_PA_STATE_ADVERTISING) {
			if (pa_ind->cause) {
				BT_LOGE("[APP] Periodic ADV(%d) started failed, err 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
			} else {
				BT_LOGA("[APP] Periodic ADV(%d) started\r\n", pa_ind->adv_handle);
			}
		} else if (pa_ind->state == RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING) {
			BT_LOGE("[APP] Periodic ADV(%d) waiting for ext adv start, cause 0x%x\r\n", pa_ind->adv_handle, pa_ind->cause);
		}
		BT_AT_PRINT("+BLEGAP:pa_adv,%s,%d,%d\r\n",
					((pa_ind->state == RTK_BT_LE_PA_STATE_IDLE) ? "stop" : ((pa_ind->state == RTK_BT_LE_PA_STATE_ADVERTISING) ? "start" : "wait_eadv")),
					((pa_ind->state == RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING) ? 0 : ((pa_ind->cause == 0) ? 0 : -1)),
					pa_ind->adv_handle);
		break;
	}
#endif

	case RTK_BT_LE_GAP_EVT_SCAN_START_IND: {
		rtk_bt_le_scan_start_ind_t *scan_start_ind = (rtk_bt_le_scan_start_ind_t *)param;
		if (!scan_start_ind->err) {
#ifdef CONFIG_PLATFORM_TIZENRT_OS
			client_init_parm->trble_scan_state_changed_cb(TRBLE_SCAN_STARTED);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
			BT_LOGA("[APP] Scan started, scan_type: %d\r\n", scan_start_ind->scan_type);
		} else {
			BT_LOGE("[APP] Scan start failed(err: 0x%x)\r\n", scan_start_ind->err);
		}
		BT_AT_PRINT("+BLEGAP:scan,start,%d,%d\r\n", (scan_start_ind->err == 0) ? 0 : -1, scan_start_ind->scan_type);
		break;
	}

	case RTK_BT_LE_GAP_EVT_SCAN_RES_IND: {
		rtk_bt_le_scan_res_ind_t *scan_res_ind = (rtk_bt_le_scan_res_ind_t *)param;
		rtk_bt_le_addr_to_str(&(scan_res_ind->adv_report.addr), le_addr, sizeof(le_addr));
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		BT_LOGD("[APP] Scan info, [Device]: %s, AD evt type: %d, RSSI: %d, len: %d \r\n",
				le_addr, scan_res_ind->adv_report.evt_type, scan_res_ind->adv_report.rssi,
				scan_res_ind->adv_report.len);
		BT_AT_PRINT("+BLEGAP:scan,info,%s,%d,%d,%d\r\n",
					le_addr, scan_res_ind->adv_report.evt_type, scan_res_ind->adv_report.rssi,
					scan_res_ind->adv_report.len);

		trble_scanned_device scanned_device;
		scanned_device.adv_type = scan_res_ind->adv_report.evt_type;
		if (TRBLE_ADV_TYPE_SCAN_RSP == scanned_device.adv_type) {
			memcpy(scanned_device.resp_data, scan_res_ind->adv_report.data, scan_res_ind->adv_report.len);
			scanned_device.resp_data_length = scan_res_ind->adv_report.len;
		} else {
			memcpy(scanned_device.raw_data, scan_res_ind->adv_report.data, scan_res_ind->adv_report.len);
			scanned_device.raw_data_length = scan_res_ind->adv_report.len;
		}
		memcpy(scanned_device.addr.mac, scan_res_ind->adv_report.addr.addr_val, RTK_BD_ADDR_LEN);
		scanned_device.addr.type = scan_res_ind->adv_report.addr.type;
		scanned_device.rssi = scan_res_ind->adv_report.rssi;
		client_init_parm->trble_device_scanned_cb(&scanned_device);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	case RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND: {
		rtk_bt_le_ext_scan_res_ind_t *scan_res_ind = (rtk_bt_le_ext_scan_res_ind_t *)param;
		rtk_bt_le_addr_to_str(&(scan_res_ind->addr), le_addr, sizeof(le_addr));
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		BT_LOGD("[APP] Ext Scan info, [Device]: %s, AD evt type: 0x%x, RSSI: %d, PHY: 0x%x, TxPower: %d, Data_status: %d, Len: %d\r\n",
				le_addr, scan_res_ind->evt_type, scan_res_ind->rssi,
				(scan_res_ind->primary_phy << 4) | scan_res_ind->secondary_phy,
				scan_res_ind->tx_power, scan_res_ind->data_status, scan_res_ind->len);
		BT_AT_PRINT("+BLEGAP:escan,%s,0x%x,%d,0x%x,%d,%d,%d\r\n",
					le_addr, scan_res_ind->evt_type, scan_res_ind->rssi,
					(scan_res_ind->primary_phy << 4) | scan_res_ind->secondary_phy,
					scan_res_ind->tx_power, scan_res_ind->data_status, scan_res_ind->len);

		trble_scanned_device scanned_device = {0};
		if (scan_res_ind->len > 31) {
			scan_res_ind->len = 31;
		}
		scanned_device.adv_type = scan_res_ind->evt_type;
		if (scanned_device.adv_type == 0x1b) {
			memcpy(scanned_device.resp_data, scan_res_ind->data, scan_res_ind->len);
			scanned_device.resp_data_length = scan_res_ind->len;
		} else {
			memcpy(scanned_device.raw_data, scan_res_ind->data, scan_res_ind->len);
			scanned_device.raw_data_length = scan_res_ind->len;
		}
		memcpy(scanned_device.addr.mac, scan_res_ind->addr.addr_val, RTK_BD_ADDR_LEN);
		scanned_device.addr.type = scan_res_ind->addr.type;
		scanned_device.rssi = scan_res_ind->rssi;
		client_init_parm->trble_device_scanned_cb(&scanned_device);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}
#endif

	case RTK_BT_LE_GAP_EVT_SCAN_STOP_IND: {
		rtk_bt_le_scan_stop_ind_t *scan_stop_ind = (rtk_bt_le_scan_stop_ind_t *)param;
		if (!scan_stop_ind->err) {
#ifdef CONFIG_PLATFORM_TIZENRT_OS
			client_init_parm->trble_scan_state_changed_cb(TRBLE_SCAN_STOPPED);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
			BT_LOGA("[APP] Scan stopped, reason: 0x%x\r\n", scan_stop_ind->stop_reason);
		} else {
			BT_LOGE("[APP] Scan stop failed(err: 0x%x)\r\n", scan_stop_ind->err);
		}
		BT_AT_PRINT("+BLEGAP:scan,stop,%d,0x%x\r\n", (scan_stop_ind->err == 0) ? 0 : -1, scan_stop_ind->stop_reason);
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONNECT_IND: {
		rtk_bt_le_conn_ind_t *conn_ind = (rtk_bt_le_conn_ind_t *)param;
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		memcpy(ble_tizenrt_scatternet_conn_ind, conn_ind, sizeof(rtk_bt_le_conn_ind_t));
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		rtk_bt_le_addr_to_str(&(conn_ind->peer_addr), le_addr, sizeof(le_addr));
		if (!conn_ind->err) {
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_PM
			/* Register PM_BLE_DOMAIN and Perform 10 minutes timedsuspend */
			rtk_tizenrt_bt_pm_suspend(600000);
#endif //#ifdef CONFIG_PM
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
			role = conn_ind->role ? "slave" : "master";
			BT_LOGA("[APP] Connected, handle: %d, role: %s, remote device: %s\r\n",
					conn_ind->conn_handle, role, le_addr);

#ifdef CONFIG_PLATFORM_TIZENRT_OS
			if (RTK_BT_LE_ROLE_MASTER == conn_ind->role) {
				BT_LOGA("[APP] GATTC Profiles attach connection success, conn_handle: %d\r\n",
						conn_ind->conn_handle);
			}
			if (RTK_BT_LE_ROLE_MASTER == conn_ind->role) {
				trble_device_connected connected_dev = {0};
				uint16_t mtu_size = 0;
				uint8_t conn_id;
				rtk_bt_le_gap_get_conn_id(conn_ind->conn_handle, &conn_id);
				connected_dev.conn_handle = conn_ind->conn_handle;
				connected_dev.is_bonded = ble_tizenrt_scatternet_bond_list[conn_id].is_bonded;
				memcpy(connected_dev.conn_info.addr.mac, conn_ind->peer_addr.addr_val, RTK_BD_ADDR_LEN);
				connected_dev.conn_info.addr.type = (trble_addr_type_e)conn_ind->peer_addr.type;
				connected_dev.conn_info.conn_interval = conn_ind->conn_interval;
				connected_dev.conn_info.slave_latency = conn_ind->conn_latency;
				connected_dev.conn_info.scan_timeout = scan_timeout;
				connected_dev.conn_info.is_secured_connect = is_secured;
				connected_dev.conn_info.mtu = mtu_size;
				client_init_parm->trble_device_connected_cb(&connected_dev);
				if (ble_client_connect_is_running) {
					ble_client_connect_is_running = 0;
				}
			} else if (RTK_BT_LE_ROLE_SLAVE == conn_ind->role) {
				uint8_t adv_handle = 0xff;
				if (server_init_parm.connected_cb) {
					uint8_t addr[TRBLE_BD_ADDR_MAX_LEN];
					_reverse_mac(conn_ind->peer_addr.addr_val, addr);
#if defined (RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
					if (RTK_BT_OK != rtk_bt_le_gap_get_ext_adv_handle_by_conn_handle(conn_ind->conn_handle, &adv_handle)) {
						BT_LOGE("[APP] Get adv handle failed \r\n");
					}
#endif
					server_init_parm.connected_cb(conn_ind->conn_handle, TRBLE_SERVER_LL_CONNECTED, addr, adv_handle);
				} else {
					ble_tizenrt_dummy_callback();
				}
			}
#else
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
			/* gattc action */
			if (RTK_BT_LE_ROLE_MASTER == conn_ind->role &&
				RTK_BT_OK == general_client_attach_conn(conn_ind->conn_handle) &&
				RTK_BT_OK == bas_client_attach_conn(conn_ind->conn_handle) &&
				RTK_BT_OK == gaps_client_attach_conn(conn_ind->conn_handle) &&
				RTK_BT_OK == simple_ble_client_attach_conn(conn_ind->conn_handle)
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
				&& RTK_BT_OK == cte_client_attach_conn(conn_ind->conn_handle)
#endif
			   ) {
				BT_LOGA("[APP] GATTC Profiles attach connection success, conn_handle: %d\r\n",
						conn_ind->conn_handle);
			}
#endif
			/* central action */
			if (RTK_BT_LE_ROLE_MASTER == conn_ind->role &&
				rtk_bt_le_sm_is_device_bonded(&conn_ind->peer_addr)) {
				BT_LOGA("[APP] Bonded device, start link encryption procedure\r\n");
				rtk_bt_le_sm_start_security(conn_ind->conn_handle);
			}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		} else {
			BT_LOGE("[APP] Connection establish failed(err: 0x%x), remote device: %s\r\n",
					conn_ind->err, le_addr);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
			if (ble_client_connect_is_running) {
				ble_client_connect_is_running = 0;
			}

			if (RTK_BT_LE_ROLE_MASTER == conn_ind->role) {
				client_init_parm->trble_device_disconnected_cb(conn_ind->conn_handle, conn_ind->err);
			} else if (RTK_BT_LE_ROLE_SLAVE == conn_ind->role) {
				server_init_parm.disconnected_cb(conn_ind->conn_handle, conn_ind->err);
			}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		}
		BT_AT_PRINT("+BLEGAP:conn,%d,%d,%s\r\n", (conn_ind->err == 0) ? 0 : -1, (int)conn_ind->conn_handle, le_addr);
		break;
	}

	case RTK_BT_LE_GAP_EVT_DISCONN_IND: {
		rtk_bt_le_disconn_ind_t *disconn_ind = (rtk_bt_le_disconn_ind_t *)param;
		rtk_bt_le_addr_to_str(&(disconn_ind->peer_addr), le_addr, sizeof(le_addr));
		role = disconn_ind->role ? "slave" : "master";
		BT_LOGA("[APP] Disconnected, reason: 0x%x, handle: %d, role: %s, remote device: %s\r\n",
				disconn_ind->reason, disconn_ind->conn_handle, role, le_addr);
		BT_AT_PRINT("+BLEGAP:disconn,0x%x,%d,%s,%s\r\n",
					disconn_ind->reason, disconn_ind->conn_handle, role, le_addr);

#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (ble_client_connect_is_running) {
			ble_client_connect_is_running = 0;
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

		if (RTK_BT_LE_ROLE_SLAVE == disconn_ind->role) {
			/* gap action */
#ifdef CONFIG_PLATFORM_TIZENRT_OS
			if (server_init_parm.disconnected_cb) {
				server_init_parm.disconnected_cb(disconn_ind->conn_handle, disconn_ind->reason);
			} else {
				ble_tizenrt_dummy_callback();
			}
#else
#if !defined(RTK_BLE_5_0_USE_EXTENDED_ADV) || !RTK_BLE_5_0_USE_EXTENDED_ADV
			rtk_bt_le_gap_dev_state_t dev_state;
			rtk_bt_le_adv_param_t adv_param = {0};
			if (rtk_bt_le_gap_get_dev_state(&dev_state) == RTK_BT_OK &&
				dev_state.gap_adv_state == RTK_BT_LE_ADV_STATE_IDLE) {
				memcpy(&adv_param, &def_adv_param, sizeof(rtk_bt_le_adv_param_t));
#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
				if (privacy_enable) {
					uint8_t bond_size = 0;
					adv_param.own_addr_type = 2;
					BT_APP_PROCESS(rtk_bt_le_sm_get_bond_num(&bond_size));
					if (bond_size != 0) {
#if defined(PRIVACY_USE_DIR_ADV_WHEN_BONDED) && PRIVACY_USE_DIR_ADV_WHEN_BONDED
						rtk_bt_le_bond_info_t bond_info = {0};
						uint8_t bond_num = 1;
						rtk_bt_le_sm_get_bond_info(&bond_info, &bond_num);
						adv_param.type = RTK_BT_LE_ADV_TYPE_DIRECT_IND_LOW;
						adv_param.peer_addr.type = 0;
						memcpy(adv_param.peer_addr.addr_val, bond_info.ident_addr.addr_val, RTK_BD_ADDR_LEN);
						adv_param.own_addr_type = 2;
#endif /* PRIVACY_USE_DIR_ADV_WHEN_BONDED */
						if (privacy_whitelist) {
							adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
						} else {
							adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
						}
					}
				}
#endif /* RTK_BLE_PRIVACY_SUPPORT */
				BT_LOGA("[APP] Reconnect ADV starting, adv type:%d,  own_addr_type: %d, filter_policy: %d\r\n"
						, adv_param.type,  adv_param.own_addr_type, adv_param.filter_policy);
				BT_APP_PROCESS(rtk_bt_le_gap_start_adv(&adv_param));
			}
#endif /* RTK_BLE_5_0_USE_EXTENDED_ADV */
			/* gatts action */
			app_server_disconnect(disconn_ind->conn_handle);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		}

#ifdef CONFIG_PLATFORM_TIZENRT_OS
		else if (RTK_BT_LE_ROLE_MASTER == disconn_ind->role) {
			client_init_parm->trble_device_disconnected_cb(disconn_ind->conn_handle, disconn_ind->reason);
		}
#else
		/* gattc action */
		if (RTK_BT_LE_ROLE_MASTER == disconn_ind->role &&
			RTK_BT_OK == general_client_detach_conn(disconn_ind->conn_handle) &&
			RTK_BT_OK == bas_client_detach_conn(disconn_ind->conn_handle) &&
			RTK_BT_OK == gaps_client_detach_conn(disconn_ind->conn_handle) &&
			RTK_BT_OK == simple_ble_client_detach_conn(disconn_ind->conn_handle)
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
			&& RTK_BT_OK == cte_client_detach_conn(disconn_ind->conn_handle)
#endif
		   ) {
			BT_LOGA("[APP] GATTC Profiles detach connection success, conn_handle: %d\r\n",
					disconn_ind->conn_handle);
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND: {
		rtk_bt_le_conn_update_ind_t *conn_update_ind =
			(rtk_bt_le_conn_update_ind_t *)param;
		if (conn_update_ind->err) {
			BT_LOGE("[APP] Update conn param failed, conn_handle: %d, err: 0x%x\r\n",
					conn_update_ind->conn_handle, conn_update_ind->err);
			BT_AT_PRINT("+BLEGAP:conn_update,%d,-1\r\n", conn_update_ind->conn_handle);
		} else {
			BT_LOGA("[APP] Conn param is updated, conn_handle: %d, conn_interval: 0x%x, "       \
					"conn_latency: 0x%x, supervision_timeout: 0x%x\r\n",
					conn_update_ind->conn_handle,
					conn_update_ind->conn_interval,
					conn_update_ind->conn_latency,
					conn_update_ind->supv_timeout);
			BT_AT_PRINT("+BLEGAP:conn_update,%d,0,0x%x,0x%x,0x%x\r\n",
						conn_update_ind->conn_handle,
						conn_update_ind->conn_interval,
						conn_update_ind->conn_latency,
						conn_update_ind->supv_timeout);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND: { //BT api shall not be called here
		rtk_bt_le_remote_conn_update_req_ind_t *rmt_update_req =
			(rtk_bt_le_remote_conn_update_req_ind_t *)param;
		BT_LOGA("[APP] Remote device request a change in conn param, conn_handle: %d, "      \
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
		BT_LOGA("[APP] Data len is updated, conn_handle: %d, "       \
				"max_tx_octets: 0x%x, max_tx_time: 0x%x, "        \
				"max_rx_octets: 0x%x, max_rx_time: 0x%x\r\n",
				data_len_change->conn_handle,
				data_len_change->max_tx_octets,
				data_len_change->max_tx_time,
				data_len_change->max_rx_octets,
				data_len_change->max_rx_time);
		BT_AT_PRINT("+BLEGAP:conn_datalen,%d,0x%x,0x%x,0x%x,0x%x\r\n",
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
			BT_LOGE("[APP] Update PHY failed, conn_handle: %d, err: 0x%x\r\n",
					phy_update_ind->conn_handle,
					phy_update_ind->err);
			BT_AT_PRINT("+BLEGAP:conn_phy,%d,-1\r\n", phy_update_ind->conn_handle);
		} else {
			BT_LOGA("[APP] PHY is updated, conn_handle: %d, tx_phy: %d, rx_phy: %d\r\n",
					phy_update_ind->conn_handle,
					phy_update_ind->tx_phy,
					phy_update_ind->rx_phy);
			BT_AT_PRINT("+BLEGAP:conn_phy,%d,0,%d,%d\r\n",
						phy_update_ind->conn_handle,
						phy_update_ind->tx_phy,
						phy_update_ind->rx_phy);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_READ_REMOTE_VERSION_IND: {
		rtk_bt_le_read_remote_version_ind_t *rmt_ver = (rtk_bt_le_read_remote_version_ind_t *)param;
		if (rmt_ver->err) {
			BT_LOGE("[APP] Read remote version failed, conn_handle: %d, err: 0x%x\r\n",
					rmt_ver->conn_handle, rmt_ver->err);
			BT_AT_PRINT("+BLEGAP:remote_version,%d,-1\r\n", rmt_ver->conn_handle);
		} else {
			BT_LOGA("[APP] Read remote version, conn_handle: %d, version: 0x%x, company_id: 0x%x, subversion: 0x%x\r\n",
					rmt_ver->conn_handle, rmt_ver->version, rmt_ver->company_id, rmt_ver->subversion);
			BT_AT_PRINT("+BLEGAP:remote_version,%d,0,0x%x,0x%x,0x%x\r\n", rmt_ver->conn_handle,
						rmt_ver->version, rmt_ver->company_id, rmt_ver->subversion);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND: {
		rtk_bt_le_auth_pair_cfm_ind_t *pair_cfm_ind =
			(rtk_bt_le_auth_pair_cfm_ind_t *)param;
		BT_LOGA("[APP] Just work pairing need user to confirm, conn_handle: %d!\r\n",
				pair_cfm_ind->conn_handle);
		BT_AT_PRINT("+BLEGAP:pair_cfm,%d\r\n", pair_cfm_ind->conn_handle);
		rtk_bt_le_pair_cfm_t pair_cfm_param = {0};
		uint16_t ret = 0;
		pair_cfm_param.conn_handle = pair_cfm_ind->conn_handle;
		pair_cfm_param.confirm = 1;
		ret = rtk_bt_le_sm_pairing_confirm(&pair_cfm_param);
		if (RTK_BT_OK == ret) {
			BT_LOGA("[APP] Just work pairing auto confirm succcess\r\n");
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND: {
		rtk_bt_le_auth_key_display_ind_t *key_dis_ind =
			(rtk_bt_le_auth_key_display_ind_t *)param;
		BT_LOGA("[APP] Auth passkey display: %d, conn_handle:%d\r\n",
				key_dis_ind->passkey,
				key_dis_ind->conn_handle);
		BT_AT_PRINT("+BLEGAP:passkey_display,%d,%d\r\n",
					(int)key_dis_ind->conn_handle,
					(int)key_dis_ind->passkey);
		break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND: {
		rtk_bt_le_auth_key_input_ind_t *key_input_ind =
			(rtk_bt_le_auth_key_input_ind_t *)param;
		BT_LOGA("[APP] Please input the auth passkey get from remote, conn_handle: %d\r\n",
				key_input_ind->conn_handle);
		BT_AT_PRINT("+BLEGAP:passkey_input,%d\r\n", key_input_ind->conn_handle);
		break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND: {
		rtk_bt_le_auth_key_cfm_ind_t *key_cfm_ind =
			(rtk_bt_le_auth_key_cfm_ind_t *)param;
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (key_cfm_ind->conn_handle < 24) {
			if (client_init_parm->trble_device_passkey_display_cb) {
				client_init_parm->trble_device_passkey_display_cb(key_cfm_ind->passkey, key_cfm_ind->conn_handle);
			} else {
				ble_tizenrt_dummy_callback();
			}
		} else {
			if (server_init_parm.passkey_display_cb) {
				server_init_parm.passkey_display_cb(key_cfm_ind->passkey, key_cfm_ind->conn_handle);
			} else {
				ble_tizenrt_dummy_callback();
			}
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		BT_LOGA("[APP] Auth passkey confirm: %d, conn_handle: %d. "  \
				"Please comfirm if the passkeys are equal!\r\n",
				key_cfm_ind->passkey,
				key_cfm_ind->conn_handle);
		BT_AT_PRINT("+BLEGAP:passkey_cfm,%d,%d\r\n",
					(int)key_cfm_ind->conn_handle,
					(int)key_cfm_ind->passkey);
		break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND: {
		rtk_bt_le_auth_oob_input_ind_t *oob_input_ind =
			(rtk_bt_le_auth_oob_input_ind_t *)param;
		BT_LOGA("[APP] Bond use oob key, conn_handle: %d. Please input the oob tk \r\n",
				oob_input_ind->conn_handle);
		BT_AT_PRINT("+BLEGAP:oobkey_input,%d\r\n", oob_input_ind->conn_handle);
		break;
	}

	case RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND: {
		rtk_bt_le_auth_complete_ind_t *auth_cplt_ind =
			(rtk_bt_le_auth_complete_ind_t *)param;
		BT_AT_PRINT("+BLEGAP:sec,%d,%d\r\n",
					auth_cplt_ind->conn_handle,
					(auth_cplt_ind->err == 0) ? 0 : -1);
		if (auth_cplt_ind->err) {
			BT_LOGE("[APP] Pairing failed(err: 0x%x), conn_handle: %d\r\n",
					auth_cplt_ind->err, auth_cplt_ind->conn_handle);

#ifdef CONFIG_PLATFORM_TIZENRT_OS
			if (RTK_BT_LE_ROLE_SLAVE == ble_tizenrt_scatternet_conn_ind->role) {
				server_init_parm.pair_bond_cb(auth_cplt_ind->err, auth_cplt_ind->conn_handle);
			} else {
				client_init_parm->trble_device_pair_bond_cb(auth_cplt_ind->err, auth_cplt_ind->conn_handle);
			}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		} else {
			BT_LOGA("[APP] Pairing success, conn_handle: %d\r\n", auth_cplt_ind->conn_handle);
			BT_DUMPHEXA("[APP] long term key is 0x", auth_cplt_ind->dev_ltk, auth_cplt_ind->dev_ltk_length, true);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
			if (RTK_BT_LE_ROLE_MASTER == ble_tizenrt_scatternet_conn_ind->role) {
				uint8_t conn_id;
				rtk_bt_le_gap_get_conn_id(auth_cplt_ind->conn_handle, &conn_id);
				ble_tizenrt_scatternet_bond_list[conn_id].is_bonded = true;
				memcpy(ble_tizenrt_scatternet_bond_list[conn_id].conn_info.addr.mac, ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val, RTK_BD_ADDR_LEN);
				trble_device_connected connected_dev = {0};
				uint16_t mtu_size = 0;
				if (RTK_BT_OK != rtk_bt_le_gap_get_mtu_size(auth_cplt_ind->conn_handle, &mtu_size)) {
					BT_LOGE("[APP] Get mtu size failed \r\n");
				}
				connected_dev.conn_handle = ble_tizenrt_scatternet_conn_ind->conn_handle;
				connected_dev.is_bonded = ble_tizenrt_scatternet_bond_list[conn_id].is_bonded;
				memcpy(connected_dev.conn_info.addr.mac, ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val, RTK_BD_ADDR_LEN);
				connected_dev.conn_info.addr.type = (trble_addr_type_e)ble_tizenrt_scatternet_conn_ind->peer_addr.type;
				connected_dev.conn_info.conn_interval = ble_tizenrt_scatternet_conn_ind->conn_interval;
				connected_dev.conn_info.slave_latency = ble_tizenrt_scatternet_conn_ind->conn_latency;
				connected_dev.conn_info.scan_timeout = scan_timeout;
				connected_dev.conn_info.is_secured_connect = is_secured;
				connected_dev.conn_info.mtu = mtu_size;
				client_init_parm->trble_device_connected_cb(&connected_dev);
			} else if (RTK_BT_LE_ROLE_SLAVE == ble_tizenrt_scatternet_conn_ind->role) {
				uint8_t adv_handle = 0xff;
				if (server_init_parm.connected_cb) {
#if defined (RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
					if (RTK_BT_OK != rtk_bt_le_gap_get_ext_adv_handle_by_conn_handle(auth_cplt_ind->conn_handle, &adv_handle)) {
						BT_LOGE("[APP] Get adv handle failed \r\n");
					}
#endif
					uint8_t addr[TRBLE_BD_ADDR_MAX_LEN];
					_reverse_mac(ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val, addr);
					server_init_parm.connected_cb(auth_cplt_ind->conn_handle, TRBLE_SERVER_SM_CONNECTED, ble_tizenrt_scatternet_conn_ind->peer_addr.addr_val, adv_handle);
				} else {
					ble_tizenrt_dummy_callback();
				}
			}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND: {
		rtk_bt_le_bond_modify_ind_t *bond_mdf_ind =
			(rtk_bt_le_bond_modify_ind_t *)param;
		char ident_addr[30] = {0};
		rtk_bt_le_addr_to_str(&(bond_mdf_ind->remote_addr), le_addr, sizeof(le_addr));
		rtk_bt_le_addr_to_str(&(bond_mdf_ind->ident_addr), ident_addr, sizeof(ident_addr));
		BT_LOGA("[APP] Bond info modified, op: %d, addr: %s, ident_addr: %s\r\n",
				bond_mdf_ind->op, le_addr, ident_addr);
		BT_AT_PRINT("+BLEGAP:bond_modify,%d,%s,%s\r\n", bond_mdf_ind->op, le_addr, ident_addr);

#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (RTK_BT_LE_BOND_DELETE == bond_mdf_ind->op && del_bond_addr) {
			for (int i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
				if (!memcmp(ble_tizenrt_scatternet_bond_list[i].conn_info.addr.mac, del_bond_addr, RTK_BD_ADDR_LEN)) {
					ble_tizenrt_scatternet_bond_list[i].is_bonded = false;
					memset(ble_tizenrt_scatternet_bond_list[i].conn_info.addr.mac, 0, RTK_BD_ADDR_LEN);
					break;
				}
			}
		} else if (RTK_BT_LE_BOND_CLEAR == bond_mdf_ind->op) {
			for (int i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
				ble_tizenrt_scatternet_bond_list[i].is_bonded = false;
				memset(ble_tizenrt_scatternet_bond_list[i].conn_info.addr.mac, 0, RTK_BD_ADDR_LEN);
			}
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
	case RTK_BT_LE_GAP_EVT_RESOLV_LIST_MODIFY_IND: {
		rtk_bt_le_modify_resolv_list_ind_t *p_ind = (rtk_bt_le_modify_resolv_list_ind_t *)param;
		rtk_bt_le_addr_t addr;

		if (p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_ADD || p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_REMOVE) {
			addr.type = (rtk_bt_le_addr_type_t)p_ind->entry.addr_type;
			memcpy(addr.addr_val, p_ind->entry.addr, RTK_BD_ADDR_LEN);
			rtk_bt_le_addr_to_str(&addr, le_addr, sizeof(le_addr));
			if (p_ind->err) {
				BT_LOGE("[APP] Resolving list %s %s fail, cause:%x.\r\n",
						(p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_ADD) ? "add" : "remove",
						le_addr, p_ind->err);
				BT_AT_PRINT("+BLEGAP:resolv_list_modify,%d,-1\r\n", p_ind->op);
			} else {
				BT_LOGA("[APP] Resolving list %s %s success, %s privacy mode.\r\n",
						(p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_ADD) ? "add" : "remove",
						le_addr, p_ind->entry.device_mode ? "device" : "network");
				BT_AT_PRINT("+BLEGAP:resolv_list_modify,%d,0,%s,%s\r\n",
							p_ind->op, le_addr, p_ind->entry.device_mode ? "device" : "network");
			}
		} else if (p_ind->op == RTK_BT_LE_RESOLV_LIST_OP_CLEAR) {
			if (p_ind->err) {
				BT_LOGE("[APP] Resolving list clear fail, cause:%x.\r\n", p_ind->err);
			} else {
				BT_LOGA("[APP] Resolving list clear success.\r\n");
			}
			BT_AT_PRINT("+BLEGAP:resolv_list_modify,%d,%d\r\n", p_ind->op, (p_ind->err == 0) ? 0 : -1);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_RESOLV_LIST_PENDING_IND: {
		rtk_bt_le_resolv_list_pending_ind_t *p_ind = (rtk_bt_le_resolv_list_pending_ind_t *)param;
		BT_LOGA("[APP] WARN: Resolving list modification is pending because of adv(%d) scan(%d) connect(%d)!!!\r\n",
				(p_ind->reason & RTK_BT_LE_RESOLV_LIST_PEND_BY_ADV) ? 1 : 0,
				(p_ind->reason & RTK_BT_LE_RESOLV_LIST_PEND_BY_SCAN) ? 1 : 0,
				(p_ind->reason & RTK_BT_LE_RESOLV_LIST_PEND_BY_CONNECT) ? 1 : 0);
		BT_AT_PRINT("+BLEGAP:resolv_list_pending,%d,%d,%d\r\n",
					(p_ind->reason & RTK_BT_LE_RESOLV_LIST_PEND_BY_ADV) ? 1 : 0,
					(p_ind->reason & RTK_BT_LE_RESOLV_LIST_PEND_BY_SCAN) ? 1 : 0,
					(p_ind->reason & RTK_BT_LE_RESOLV_LIST_PEND_BY_CONNECT) ? 1 : 0);
		break;
	}
#endif

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND: {
		rtk_bt_le_pa_sync_ind_t *pa_sync_ind = (rtk_bt_le_pa_sync_ind_t *)param;
		BT_LOGA("[APP] PA sync state change: sync_id: %d, state = %d, cause: 0x%x\r\n",
				pa_sync_ind->sync_id, pa_sync_ind->state, pa_sync_ind->cause);

		if (pa_sync_ind->state == RTK_BT_LE_PA_SYNC_STATE_SYNCHRONIZED) {
			rtk_bt_le_pa_synced_info_t *p_info = &pa_sync_ind->info;
			rtk_bt_le_addr_to_str(&p_info->addr, le_addr, sizeof(le_addr));
			BT_LOGA("[APP] PA SYNCHRONIZED PARAM: [Device]: %s, sync_handle:0x%x, adv_sid: %d, past_received: %d\r\n",
					le_addr, p_info->sync_handle, p_info->adv_sid, p_info->past_received);
			BT_AT_PRINT("+BLEGAP:pa_sync,%s,0x%x,%d,%d\r\n",
						le_addr, p_info->sync_handle, p_info->adv_sid, p_info->past_received);
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND: {
		rtk_bt_le_pa_adv_report_ind_t *pa_report = (rtk_bt_le_pa_adv_report_ind_t *)param;
		BT_LOGA("[APP] PA sync ADV report: sync_id %d, sync_handle 0x%x, tx_power %d, rssi %d, cte_type %d, data_status 0x%x, data_len %d\r\n",
				pa_report->sync_id, pa_report->sync_handle, pa_report->tx_power, pa_report->rssi,
				pa_report->cte_type, pa_report->data_status, pa_report->data_len);
		BT_AT_PRINT("+BLEGAP:pa_report,%d,0x%x,%d,%d,%d,0x%x,%d\r\n",
					pa_report->sync_id, pa_report->sync_handle, pa_report->tx_power, pa_report->rssi,
					pa_report->cte_type, pa_report->data_status, pa_report->data_len);
		break;
	}
#endif

#if defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	case RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND: {
		rtk_bt_le_txpower_ind_t *txpower_ind = (rtk_bt_le_txpower_ind_t *)param;
		BT_LOGA("[APP] TX power report: conn_handle %d, type %d, txpower %d\r\n",
				txpower_ind->conn_handle, txpower_ind->type, txpower_ind->txpower);
		BT_AT_PRINT("+BLEGAP:txpower_report,%d,%d,%d\r\n",
					txpower_ind->conn_handle, txpower_ind->type, txpower_ind->txpower);
		break;
	}
#endif

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
	case RTK_BT_LE_GAP_EVT_CONNLESS_IQ_REPORT_IND: {
		rtk_bt_le_gap_connless_iq_report_ind_t *report_ind = (rtk_bt_le_gap_connless_iq_report_ind_t *)param;
		BT_LOGA("[APP] Connectionless IQ report: sync_id %u, sync_handle 0x%x, channel_index %u, "       \
				"rssi %d, rssi_antenna_id %u, cte_type %d, "     \
				"slot_durations %d, packet_status %d, periodic_event_counter %d, "       \
				"sample_count %d \r\n",
				report_ind->sync_id,
				report_ind->sync_handle,
				report_ind->channel_index,
				report_ind->rssi,
				report_ind->rssi_antenna_id,
				report_ind->cte_type,
				report_ind->slot_durations,
				report_ind->packet_status,
				report_ind->periodic_event_counter,
				report_ind->sample_count);
		BT_AT_PRINT("+BLEGAP:cte_connless_report,%u,0x%x,%u,%d,%u,%d,%d,%d,%d,%d\r\n",
					report_ind->sync_id,
					report_ind->sync_handle,
					report_ind->channel_index,
					report_ind->rssi,
					report_ind->rssi_antenna_id,
					report_ind->cte_type,
					report_ind->slot_durations,
					report_ind->packet_status,
					report_ind->periodic_event_counter,
					report_ind->sample_count);

		BT_DUMPA("[APP] Connectionless IQ sample:", report_ind->iq_sample, report_ind->sample_count * 2);
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONN_IQ_REPORT_IND: {
		rtk_bt_le_gap_conn_iq_report_ind_t *report_ind = (rtk_bt_le_gap_conn_iq_report_ind_t *)param;
		BT_LOGA("[APP] Connection IQ report: conn_handle %d, rx_phy %d, data_chan_index %d, "        \
				"rssi %d, rssi_antenna_id %d, cte_type %d, "     \
				"slot_durations %d, packet_status %d, connection_event_counter %d, "     \
				"sample_count %d\r\n",
				report_ind->conn_handle,
				report_ind->rx_phy,
				report_ind->data_chan_index,
				report_ind->rssi,
				report_ind->rssi_antenna_id,
				report_ind->cte_type,
				report_ind->slot_durations,
				report_ind->packet_status,
				report_ind->connection_event_counter,
				report_ind->sample_count);
		BT_AT_PRINT("+BLEGAP:cte_conn_report,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
					report_ind->conn_handle,
					report_ind->rx_phy,
					report_ind->data_chan_index,
					report_ind->rssi,
					report_ind->rssi_antenna_id,
					report_ind->cte_type,
					report_ind->slot_durations,
					report_ind->packet_status,
					report_ind->connection_event_counter,
					report_ind->sample_count);

		BT_DUMPA("[APP] Connection IQ sample:", report_ind->iq_sample, report_ind->sample_count * 2);
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONN_CTE_REQ_FAIL_IND: {
		rtk_bt_le_gap_cte_req_fail_ind_t *fail_ind = (rtk_bt_le_gap_cte_req_fail_ind_t *)param;
		BT_LOGE("[APP] CTE request fail conn_handle %u, cause %u\r\n", fail_ind->conn_handle, fail_ind->cause);
		BT_AT_PRINT("+BLEGAP:cte_conn_rx,%d,fail\r\n", fail_ind->conn_handle);
		break;
	}

	case RTK_BT_LE_GAP_EVT_CONNLESS_CTE_TX_STATE_IND: {
		rtk_bt_le_gap_connless_cte_tx_ind_t *tx_ind = (rtk_bt_le_gap_connless_cte_tx_ind_t *)param;
		BT_LOGA("[APP] Connectionless CTE transmit adv_handle %u, cause %u, ", tx_ind->adv_handle, tx_ind->cause);
		BT_AT_PRINT("+BLEGAP:connless_cte_tx,%d,%d\r\n", tx_ind->adv_handle, tx_ind->state);
		switch (tx_ind->state) {
		case RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_IDLE:
			BT_LOGA("state STATE_IDLE\r\n");
			break;
		case RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_EXT_ADV_IDLE:
			BT_LOGA("state STATE_EXT_ADV_IDLE\r\n");
			break;
		case RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_PA_ADV:
			BT_LOGA("state STATE_WAIT_PA_ADV\r\n");
			break;
		case RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_EXT_ADV:
			BT_LOGA("state STATE_WAIT_EXT_ADV\r\n");
			break;
		case RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_ENABLING:
			BT_LOGA("state STATE_ENABLING\r\n");
			break;
		case RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_TRANSMITTING:
			BT_LOGA("state STATE_TRANSMITTING\r\n");
			break;
		default:
			BT_LOGE("unsupport state\r\n");
			break;
		}
		break;
	}
#endif /* RTK_BLE_5_1_CTE_SUPPORT */

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	case RTK_BT_LE_GAP_EVT_COC_REG_PSM_IND: {
		rtk_bt_le_coc_reg_psm_ind_t *reg_psm_ind = (rtk_bt_le_coc_reg_psm_ind_t *)param;
		if (!reg_psm_ind->err) {
			BT_LOGA("[APP] LE COC register PSM success, le_psm: 0x%x, err: 0x%x\r\n",
					reg_psm_ind->le_psm, reg_psm_ind->err);
		} else {
			BT_LOGE("[APP] LE COC register PSM fail, le_psm: 0x%x, err: 0x%x\r\n",
					reg_psm_ind->le_psm, reg_psm_ind->err);
		}
		if (le_coc_init_parm.reg_psm_cb) {
			le_coc_init_parm.reg_psm_cb(reg_psm_ind->le_psm, reg_psm_ind->err);
		} else {
			ble_tizenrt_dummy_callback();
		}
		break;
	}

	case RTK_BT_LE_GAP_EVT_COC_SET_SEC_IND: {
		rtk_bt_le_coc_set_sec_ind_t *set_sec_ind = (rtk_bt_le_coc_set_sec_ind_t *)param;
		if (!set_sec_ind->err) {
			BT_LOGA("[APP] LE COC set security success, err: 0x%x\r\n",
					set_sec_ind->err);
		} else {
			BT_LOGE("[APP] LE COC set security fail, err: 0x%x\r\n",
					set_sec_ind->err);
		}
		if (le_coc_init_parm.set_sec_cb) {
			le_coc_init_parm.set_sec_cb(set_sec_ind->err);
		} else {
			ble_tizenrt_dummy_callback();
		}
		break;
	}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

	case RTK_BT_LE_GAP_EVT_COC_CONNECT_IND: {
		rtk_bt_le_coc_conn_state_ind_t *coc_conn_ind = (rtk_bt_le_coc_conn_state_ind_t *)param;
		if (!coc_conn_ind->err) {
			BT_LOGA("[APP] LE COC connected, conn_handle: %d, cid: 0x%x\r\n",
					coc_conn_ind->conn_handle, coc_conn_ind->cid);
		} else {
			BT_LOGE("[APP] LE COC connect failed, conn_handle: %d, cid: 0x%x, err: 0x%x\r\n",
					coc_conn_ind->conn_handle, coc_conn_ind->cid, coc_conn_ind->err);
		}
		BT_AT_PRINT("+BLEGAP:coc_conn,%d,0x%x,%d\r\n", coc_conn_ind->conn_handle, coc_conn_ind->cid,
					(coc_conn_ind->err == 0) ? 0 : -1);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (le_coc_init_parm.con_cb) {
			le_coc_init_parm.con_cb(coc_conn_ind->conn_handle, coc_conn_ind->cid, coc_conn_ind->err);
		} else {
			ble_tizenrt_dummy_callback();
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}

	case RTK_BT_LE_GAP_EVT_COC_DISCONNECT_IND: {
		rtk_bt_le_coc_conn_state_ind_t *coc_disconn_ind = (rtk_bt_le_coc_conn_state_ind_t *)param;
		if (!coc_disconn_ind->err) {
			BT_LOGA("[APP] LE COC disconnected, conn_handle: %d, cid: 0x%x\r\n",
					coc_disconn_ind->conn_handle, coc_disconn_ind->cid);
		} else {
			BT_LOGE("[APP] LE COC disconnect failed, conn_handle: %d, cid: 0x%x, err: 0x%x\r\n",
					coc_disconn_ind->conn_handle, coc_disconn_ind->cid, coc_disconn_ind->err);
		}
		BT_AT_PRINT("+BLEGAP:coc_disconn,%d,0x%x,%d\r\n", coc_disconn_ind->conn_handle,
					coc_disconn_ind->cid, (coc_disconn_ind->err == 0) ? 0 : -1);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (le_coc_init_parm.discon_cb) {
			le_coc_init_parm.discon_cb(coc_disconn_ind->conn_handle, coc_disconn_ind->cid, coc_disconn_ind->err);
		} else {
			ble_tizenrt_dummy_callback();
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}

	case RTK_BT_LE_GAP_EVT_COC_SEND_DATA_RESULT_IND: {
		rtk_bt_le_coc_send_data_res_ind_t *res_ind = (rtk_bt_le_coc_send_data_res_ind_t *)param;
		BT_LOGA("[APP] LE COC send data completed, conn_handle: %d, cid: 0x%x, credit: %d, err: 0x%x\r\n",
				res_ind->conn_handle, res_ind->cid, res_ind->credit, res_ind->err);
		BT_AT_PRINT("+BLEGAP:coc_send,%d,0x%x,%d,%d\r\n", res_ind->conn_handle, res_ind->cid,
					res_ind->credit, (res_ind->err == 0) ? 0 : -1);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (le_coc_init_parm.send_cb) {
			le_coc_init_parm.send_cb(res_ind->conn_handle, res_ind->cid, res_ind->err, res_ind->credit);
		} else {
			ble_tizenrt_dummy_callback();
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}

	case RTK_BT_LE_GAP_EVT_COC_RECEIVE_DATA_IND: {
		rtk_bt_le_coc_receive_data_ind_t *data_ind = (rtk_bt_le_coc_receive_data_ind_t *)param;
		BT_LOGA("[APP] LE COC receive data, conn_handle: %d, cid: 0x%x, len: %d\r\n",
				data_ind->conn_handle, data_ind->cid, data_ind->len);
		BT_DUMPA("[HEX]: ", data_ind->data, data_ind->len);
		BT_AT_PRINT("+BLEGAP:coc_recv,%d,0x%x\r\n", data_ind->conn_handle, data_ind->cid);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		trble_data read_result;
		read_result.length = data_ind->len;
		read_result.data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, read_result.length);
		if (!read_result.data) {
			BT_LOGE("[APP] fail to malloc data %s\n", __FUNCTION__);
			break;
		}
		memcpy(read_result.data, data_ind->data, read_result.length);
		if (le_coc_init_parm.recv_cb) {
			le_coc_init_parm.recv_cb(data_ind->conn_handle, data_ind->cid, &read_result);
		} else {
			ble_tizenrt_dummy_callback();
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		break;
	}
#endif /* RTK_BLE_COC_SUPPORT */

	default:
		BT_LOGE("[APP] Unknown gap cb evt type: %d\r\n", evt_code);
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

extern trble_result_e trble_netmgr_fw_reset(void);
static rtk_bt_evt_cb_ret_t tizenrt_scatternet_common_gap_app_callback(uint8_t evt_code, void *param, uint32_t len)
{
	(void)param;
	(void)len;

	switch (evt_code) {
	case RTK_BT_GAP_EVT_FW_EXCEPTION_IND:
		trble_netmgr_fw_reset();
		break;
	default:
		break;
	}
	return 0;
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

static rtk_bt_evt_cb_ret_t ble_scatternet_gatts_app_callback(uint8_t event, void *data, uint32_t len)
{
	(void)len;
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	uint16_t app_id = 0xFFFF;
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

	if (RTK_BT_GATTS_EVT_MTU_EXCHANGE == event) {
		rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
		if (p_gatt_mtu_ind->result == RTK_BT_OK) {
			BT_LOGA("[APP] GATTS mtu exchange successfully, mtu_size: %d, conn_handle: %d \r\n",
					p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
			if (RTK_BT_LE_ROLE_SLAVE == ble_tizenrt_scatternet_conn_ind->role) {
				if (server_init_parm.mtu_update_cb) {
					server_init_parm.mtu_update_cb(p_gatt_mtu_ind->conn_handle, p_gatt_mtu_ind->mtu_size);
				} else {
					ble_tizenrt_dummy_callback();
				}
			}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		} else {
			BT_LOGE("[APP] GATTS mtu exchange fail \r\n");
		}
		return RTK_BT_EVT_CB_OK;
	} else if (RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES == event) {
		rtk_bt_gatts_client_supported_features_ind_t *p_ind = (rtk_bt_gatts_client_supported_features_ind_t *)data;
		if (p_ind->features & RTK_BT_GATTS_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT) {
			BT_LOGA("[APP] Client Supported features is writed: conn_handle %d, features 0x%02x. Remote client supports EATT.\r\n",
					p_ind->conn_handle, p_ind->features);
		}
		return RTK_BT_EVT_CB_OK;
	} else if (RTK_BT_GATTS_EVT_SERVICE_CHANGED_CCCD_IND == event) {
		rtk_bt_gatts_service_changed_cccd_ind_t *srv_change = (rtk_bt_gatts_service_changed_cccd_ind_t *)data;
		BT_LOGA("[APP] Service Changed cccd is updated, conn_handle: %d, cccd_enable: %d\r\n",
				srv_change->conn_handle, srv_change->cccd_enable);
		return RTK_BT_EVT_CB_OK;
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	} else {
		ble_tizenrt_srv_callback(event, data);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
	}

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	app_id = app_get_gatts_app_id(event, data);
	switch (app_id) {
	case SIMPLE_BLE_SRV_ID:
		simple_ble_service_callback(event, data);
		break;
	case DEVICE_INFO_SRV_ID:
		device_info_srv_callback(event, data);
		break;
	case HEART_RATE_SRV_ID:
		heart_rate_srv_callback(event, data);
		break;
	case BATTERY_SRV_ID:
		battery_service_callback(event, data);
		break;
	case IMMEDIATE_ALERT_SRV_ID:
		immediate_alert_srv_callback(event, data);
		break;
	case GLUCOSE_SRV_ID:
		glucose_srv_callback(event, data);
		break;
	case HID_SRV_ID:
		hid_srv_callback(event, data);
		break;
	case LONG_UUID_SRV_ID:
		long_uuid_service_callback(event, data);
		break;
	case CTE_SRV_ID:
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
		cte_srv_callback(event, data);
#endif
		break;
	default:
		break;
	}
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
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

static rtk_bt_evt_cb_ret_t ble_scatternet_gattc_app_callback(uint8_t event, void *data, uint32_t len)
{
	(void)len;
	uint16_t profile_id = 0xFFFF;

	if (RTK_BT_GATTC_EVT_MTU_EXCHANGE == event) {
		rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)data;
		if (p_gatt_mtu_ind->result == RTK_BT_OK) {
			BT_LOGA("[APP] GATTC mtu exchange success, mtu_size: %d, conn_handle: %d \r\n",
					p_gatt_mtu_ind->mtu_size, p_gatt_mtu_ind->conn_handle);
		} else {
			BT_LOGE("[APP] GATTC mtu exchange fail \r\n");
		}
		return RTK_BT_EVT_CB_OK;
	}

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	if (RTK_BT_GATTC_EVT_DISCOVER_ALL_STATE_IND == event) {
		rtk_bt_gattc_discover_all_state_ind_t *p_ind = (rtk_bt_gattc_discover_all_state_ind_t *)data;
		BT_LOGA("[APP] GATTC discover all finished: conn_handle: %d, is_success: %d, load_from_storage: %d\r\n",
				p_ind->conn_handle, p_ind->is_success, p_ind->load_from_storage);

		return RTK_BT_EVT_CB_OK;
	}

	if (RTK_BT_GATTC_EVT_GATT_SERVICE_INFO_IND == event) {
		rtk_bt_gattc_gatt_service_info_ind_t *p_ind = (rtk_bt_gattc_gatt_service_info_ind_t *)data;
		BT_LOGA("[APP] GATTC discover GATT service info: conn_handle: %d, is_found: %d, load_from_storage: %d\r\n",
				p_ind->conn_handle, p_ind->is_found, p_ind->load_from_storage);
		if ((p_ind->char_flag & RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG) &&
			(p_ind->server_features[0] & RTK_BT_GATTS_SERVER_SUPPORTED_FEATURES_EATT_BIT)) {
			BT_LOGA("[APP] Server supported features: 0x%02x, support EATT.\r\n", p_ind->server_features[0]);
		}

		return RTK_BT_EVT_CB_OK;
	}
#endif

	profile_id = app_get_gattc_profile_id(event, data);
	switch (profile_id) {
	case GCS_CLIENT_PROFILE_ID:
		general_client_app_callback(event, data);
		break;
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	case GAPS_CLIENT_PROFILE_ID:
		gaps_client_app_callback(event, data);
		break;
	case BAS_CLIENT_PROFILE_ID:
		bas_client_app_callback(event, data);
		break;
	case SIMPLE_BLE_CLIENT_PROFILE_ID:
		simple_ble_client_app_callback(event, data);
		break;
	case CTE_CLIENT_PROFILE_ID:
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
		cte_client_app_callback(event, data);
#endif
		break;
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

int ble_scatternet_main(uint8_t enable)
{
	rtk_bt_app_conf_t bt_app_conf = {0};
	rtk_bt_le_addr_t bd_addr = {(rtk_bt_le_addr_type_t)0, {0}};
	bool adv_filter_whitelist = false;
	char addr_str[30] = {0};
	char name[30] = {0};
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	uint8_t adv_handle;
#else
	rtk_bt_le_adv_param_t adv_param = {0};
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
	uint8_t bond_size = 0;
#endif

	if (1 == enable) {
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_PM
		rtk_tizenrt_bt_pm_init();
#endif //#ifdef CONFIG_PM
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		//set GAP configuration
		bt_app_conf.app_profile_support = RTK_BT_PROFILE_GATTS | RTK_BT_PROFILE_GATTC;
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		bt_app_conf.mtu_size = 512;
#else
		bt_app_conf.mtu_size = 180;
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		bt_app_conf.master_init_mtu_req = true;
		bt_app_conf.slave_init_mtu_req = false;
		bt_app_conf.prefer_all_phy = RTK_BT_LE_PHYS_PREFER_ALL;
		bt_app_conf.prefer_tx_phy = RTK_BT_LE_PHYS_PREFER_1M | RTK_BT_LE_PHYS_PREFER_2M | RTK_BT_LE_PHYS_PREFER_CODED;
		bt_app_conf.prefer_rx_phy = RTK_BT_LE_PHYS_PREFER_1M | RTK_BT_LE_PHYS_PREFER_2M | RTK_BT_LE_PHYS_PREFER_CODED;
		bt_app_conf.max_tx_octets = 0x40;
		bt_app_conf.max_tx_time = 0x200;
#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
		bt_app_conf.irk_auto_gen = true;
#endif
		bt_app_conf.user_def_service = false;
		bt_app_conf.cccd_not_check = false;

		/* Enable BT */
		BT_APP_PROCESS(rtk_bt_enable(&bt_app_conf));
		BT_APP_PROCESS(rtk_bt_le_gap_get_bd_addr(&bd_addr));
		rtk_bt_le_addr_to_str(&bd_addr, addr_str, sizeof(addr_str));
		BT_LOGA("[APP] BD_ADDR: %s\r\n", addr_str);

		BT_APP_PROCESS(rtk_bt_le_gap_set_scan_param(&scan_param));
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_COMMON_GP_GAP,
													scatternet_gap_app_callback));
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GAP,
													ble_scatternet_gap_app_callback));
		memcpy(name, (const uint8_t *)RTK_BT_DEV_NAME, strlen((const char *)RTK_BT_DEV_NAME));
		BT_APP_PROCESS(rtk_bt_le_gap_set_device_name((uint8_t *)name));
		BT_APP_PROCESS(rtk_bt_le_gap_set_appearance(RTK_BT_LE_GAP_APPEARANCE_HEART_RATE_BELT));

		BT_APP_PROCESS(rtk_bt_le_sm_set_security_param(&sec_param));

#if !defined(RTK_BLE_5_0_USE_EXTENDED_ADV) || !RTK_BLE_5_0_USE_EXTENDED_ADV
		memcpy(&adv_param, &def_adv_param, sizeof(rtk_bt_le_adv_param_t));
#endif
#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
		if (privacy_enable) {
			BT_APP_PROCESS(rtk_bt_le_gap_privacy_init(privacy_whitelist));
#if !defined(RTK_BLE_5_0_USE_EXTENDED_ADV) || !RTK_BLE_5_0_USE_EXTENDED_ADV
			/* If privacy on, default use RPA adv, even not bonded */
			adv_param.own_addr_type = 2;
#endif
			BT_APP_PROCESS(rtk_bt_le_sm_get_bond_num(&bond_size));
			if (bond_size != 0) {
#if (defined(PRIVACY_USE_DIR_ADV_WHEN_BONDED) && PRIVACY_USE_DIR_ADV_WHEN_BONDED) && (!defined(RTK_BLE_5_0_USE_EXTENDED_ADV) || !RTK_BLE_5_0_USE_EXTENDED_ADV)
				rtk_bt_le_bond_info_t bond_info = {0};
				uint8_t bond_num = 1;
				rtk_bt_le_sm_get_bond_info(&bond_info, &bond_num);
				adv_param.type = RTK_BT_LE_ADV_TYPE_DIRECT_IND_LOW;
				adv_param.peer_addr.type = 0;
				memcpy(adv_param.peer_addr.addr_val, bond_info.ident_addr.addr_val, RTK_BD_ADDR_LEN);
				adv_param.own_addr_type = 2;
#endif
				if (privacy_whitelist) {
					adv_filter_whitelist = true;
				}
			}
		}
#endif /* RTK_BLE_PRIVACY_SUPPORT */

		/* gatts related */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTS,
													ble_scatternet_gatts_app_callback));
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		BT_APP_PROCESS(ble_tizenrt_srv_add());
#else
		BT_APP_PROCESS(simple_ble_srv_add());
		BT_APP_PROCESS(device_info_srv_add());
		BT_APP_PROCESS(heart_rate_srv_add());
		BT_APP_PROCESS(battery_srv_add());
		BT_APP_PROCESS(immediate_alert_srv_add());
		BT_APP_PROCESS(glucose_srv_add());
		BT_APP_PROCESS(long_uuid_srv_add());
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
		BT_APP_PROCESS(cte_srv_add());
#endif
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
		/* gattc related */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_LE_GP_GATTC,
													ble_scatternet_gattc_app_callback));
		BT_APP_PROCESS(general_client_add());
#ifndef CONFIG_PLATFORM_TIZENRT_OS
		BT_APP_PROCESS(bas_client_add());
		BT_APP_PROCESS(gaps_client_add());
		BT_APP_PROCESS(simple_ble_client_add());
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
		BT_APP_PROCESS(cte_client_add());
#endif
#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
		if (adv_filter_whitelist) {
			ext_adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
		}
		BT_APP_PROCESS(rtk_bt_le_gap_create_ext_adv(&ext_adv_param, &adv_handle));
		BT_APP_PROCESS(rtk_bt_le_gap_set_ext_adv_data(adv_handle, ext_adv_data, sizeof(ext_adv_data)));
		BT_APP_PROCESS(rtk_bt_le_gap_start_ext_adv(adv_handle, 0, 0));
#else
		BT_APP_PROCESS(rtk_bt_le_gap_set_adv_data(adv_data, sizeof(adv_data)));
		BT_APP_PROCESS(rtk_bt_le_gap_set_scan_rsp_data(scan_rsp_data, sizeof(scan_rsp_data)));
		if (adv_filter_whitelist) {
			adv_param.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
		}
		BT_APP_PROCESS(rtk_bt_le_gap_start_adv(&adv_param));
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

#if (defined(BT_WAKE_UP_HOST) && BT_WAKE_UP_HOST) && (defined(RTK_BT_POWER_CONTROL_SUPPORT) && RTK_BT_POWER_CONTROL_SUPPORT)
		bt_wake_up_host_init();
#endif
	} else if (0 == enable) {
#if (defined(BT_WAKE_UP_HOST) && BT_WAKE_UP_HOST) && (defined(RTK_BT_POWER_CONTROL_SUPPORT) && RTK_BT_POWER_CONTROL_SUPPORT)
		bt_wake_up_host_deinit();
#endif

		/* Disable BT */
		BT_APP_PROCESS(rtk_bt_disable());

#ifdef CONFIG_PLATFORM_TIZENRT_OS
		BT_APP_PROCESS(general_client_delete());
#ifdef CONFIG_PM
		rtk_tizenrt_bt_pm_deinit();
#endif //#ifdef CONFIG_PM
#else
		app_server_deinit();
		BT_APP_PROCESS(general_client_delete());
		BT_APP_PROCESS(bas_client_delete());
		BT_APP_PROCESS(gaps_client_delete());
		BT_APP_PROCESS(simple_ble_client_delete());
#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT
		BT_APP_PROCESS(cte_client_delete());
#endif
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
	}

	return 0;
}
