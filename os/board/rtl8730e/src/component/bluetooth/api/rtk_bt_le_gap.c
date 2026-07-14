/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include "platform_autoconf.h"
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <bt_api_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_device.h>

uint16_t rtk_bt_le_gap_get_conn_id(uint16_t conn_handle, uint8_t *p_conn_id)
{
	uint16_t ret = 0;
	rtk_bt_le_get_conn_id_t param = {
		.conn_handle = conn_handle,
		.p_conn_id = p_conn_id,
	};

	if (!p_conn_id) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_CONN_ID, &param, sizeof(rtk_bt_le_get_conn_id_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_version(rtk_bt_le_version_info_t *version)
{
	uint16_t ret = 0;

	if (!version) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_VERSION, version, sizeof(rtk_bt_le_version_info_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_bd_addr(rtk_bt_le_addr_t *paddr)
{
	uint16_t ret = 0;

	if (!paddr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR, paddr, sizeof(rtk_bt_le_addr_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_device_name(const uint8_t *name)
{
	uint16_t ret = 0;

	if (!name) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_DEVICE_NAME, (void *)name, strlen((const char *)name));

	return ret;
}

uint16_t rtk_bt_le_gap_set_rand_addr(bool auto_generate, rtk_bt_le_rand_addr_type_t type, uint8_t *p_addr)
{
	uint16_t ret = 0;
	rtk_bt_le_set_rand_addr_t param;

	if (!p_addr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (auto_generate && type > RTK_BT_LE_RAND_ADDR_RESOLVABLE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	param.auto_generate = auto_generate;
	param.type = type;
	param.addr_val = p_addr;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_RAND_ADDR, &param, sizeof(rtk_bt_le_set_rand_addr_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_appearance(uint16_t appearance)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_APPEARANCE, (void *)&appearance, 4);

	return ret;
}

uint16_t rtk_bt_le_gap_set_preferred_conn_param(rtk_bt_le_preferred_conn_param_t *p_preferred_conn_param)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_PREFERRED_CONN_PARAM,
						  (void *)p_preferred_conn_param, sizeof(rtk_bt_le_preferred_conn_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_gen_rand_addr(rtk_bt_le_rand_addr_type_t type, uint8_t *p_addr)
{
	uint16_t ret = 0;
	rtk_bt_le_gen_rand_addr_t param;

	if (type > RTK_BT_LE_RAND_ADDR_RESOLVABLE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!p_addr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	param.type = type;
	param.p_addr = p_addr;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GEN_RAND_ADDR,
						  (void *)&param, sizeof(rtk_bt_le_gen_rand_addr_t));

	return ret;
}

uint16_t rtk_bt_le_gap_cfg_local_ident_addr(rtk_bt_le_ident_addr_type_t type, uint8_t *p_addr)
{
	uint16_t ret = 0;
	rtk_bt_le_ident_addr_t param;

	if (type > RTK_BT_LE_IDENT_ADDR_RAND) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!p_addr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	param.type = type;
	memcpy(param.addr_val, p_addr, RTK_BD_ADDR_LEN);

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_CFG_LOCAL_IDENT_ADDR,
						  (void *)&param, sizeof(rtk_bt_le_ident_addr_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_adv_data(uint8_t *padv_data, uint32_t adv_len)
{
	uint16_t ret = 0;

	if (adv_len > RTK_BT_LE_MAX_ADV_DATA_LEN) {
		return RTK_BT_ERR_ADV_LENGTH_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_ADV_DATA, padv_data, adv_len);

	return ret;
}

uint16_t rtk_bt_le_gap_set_scan_rsp_data(uint8_t *pscan_rsp_data, uint32_t scan_rsp_len)
{
	uint16_t ret = 0;

	if (scan_rsp_len > RTK_BT_LE_MAX_ADV_DATA_LEN) {
		return RTK_BT_ERR_ADV_LENGTH_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_SCAN_RSP_DATA, pscan_rsp_data, scan_rsp_len);

	return ret;
}

uint16_t rtk_bt_le_gap_start_adv(rtk_bt_le_adv_param_t *padv_param)
{
	uint16_t ret = 0;

	if (!padv_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if ((padv_param->interval_min < 0x20) || (padv_param->interval_min > 0x4000)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if ((padv_param->interval_max < 0x20) || (padv_param->interval_max > 0x4000)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (padv_param->interval_max < padv_param->interval_min) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (padv_param->type > RTK_BT_LE_ADV_TYPE_DIRECT_IND_LOW) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if ((padv_param->own_addr_type > RTK_BT_LE_ADDR_TYPE_RPA_RANDOM) || (padv_param->peer_addr.type > RTK_BT_LE_ADDR_TYPE_RPA_RANDOM)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (padv_param->channel_map > RTK_BT_LE_ADV_CHNL_ALL) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (padv_param->filter_policy > RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
	/* When extended adv supported, ext adv apis are used to send legacy adv.
	   Ext adv parameter needs random address, but rtk_bt_le_adv_param_t does not include own address */
	if ((padv_param->own_addr_type == RTK_BT_LE_ADDR_TYPE_RANDOM) || (padv_param->own_addr_type == RTK_BT_LE_ADDR_TYPE_RPA_RANDOM)) {
		BT_LOGE("Use ext adv api for random addr!\r\n");
		return RTK_BT_ERR_PARAM_INVALID;
	}
#endif

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_START_ADV, padv_param, sizeof(rtk_bt_le_adv_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_stop_adv(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_STOP_ADV, NULL, 0);

	return ret;
}

uint16_t rtk_bt_le_gap_get_adv_param(rtk_bt_le_adv_param_t *padv_param)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_ADV_PARAM, padv_param, sizeof(rtk_bt_le_adv_param_t));

	return ret;
}

bool rtk_bt_le_gap_adv_is_idle(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_ADV_STATE, NULL, sizeof(bool));

	return ret ? true : false;
}

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
uint16_t rtk_bt_le_gap_create_ext_adv(rtk_bt_le_ext_adv_param_t *p_adv_param, uint8_t *p_adv_handle)
{
	uint16_t ret = 0;
	rtk_bt_le_ext_adv_create_t param = {
		.p_adv_param = p_adv_param,
		.p_adv_handle = p_adv_handle,
	};

	if (!p_adv_param || !p_adv_handle) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if ((p_adv_param->primary_adv_interval_min < 0x20) || (p_adv_param->primary_adv_interval_max < 0x20)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_adv_param->primary_adv_interval_max < p_adv_param->primary_adv_interval_min) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_adv_param->primary_adv_channel_map > RTK_BT_LE_ADV_CHNL_ALL) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_adv_param->adv_sid > 0xF) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_adv_param->filter_policy > RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_adv_param->primary_adv_phy != RTK_BT_LE_PHYS_PRIM_ADV_1M &&
		p_adv_param->primary_adv_phy != RTK_BT_LE_PHYS_PRIM_ADV_CODED) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_adv_param->secondary_adv_phy < RTK_BT_LE_PHYS_1M ||
		p_adv_param->secondary_adv_phy > RTK_BT_LE_PHYS_CODED) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV, &param, sizeof(rtk_bt_le_ext_adv_create_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_ext_adv_data(uint8_t adv_handle, uint8_t *padv_data, uint16_t adv_len)
{
	uint16_t ret = 0;

	if (adv_len > RTK_BT_LE_MAX_EXT_ADV_DATA_LEN) {
		return RTK_BT_ERR_ADV_LENGTH_INVALID;
	}

	rtk_bt_le_ext_adv_data_t param = {
		.adv_handle = adv_handle,
		.pdata = padv_data,
		.len = adv_len
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA, &param, sizeof(rtk_bt_le_ext_adv_data_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_ext_scan_rsp_data(uint8_t adv_handle, uint8_t *pscan_rsp_data, uint16_t scan_rsp_len)
{
	uint16_t ret = 0;

	if (scan_rsp_len > RTK_BT_LE_MAX_EXT_ADV_DATA_LEN) {
		return RTK_BT_ERR_ADV_LENGTH_INVALID;
	}

	rtk_bt_le_ext_adv_data_t param = {
		.adv_handle = adv_handle,
		.pdata = pscan_rsp_data,
		.len = scan_rsp_len
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA, &param, sizeof(rtk_bt_le_ext_adv_data_t));

	return ret;
}

uint16_t rtk_bt_le_gap_start_ext_adv(uint8_t adv_handle, uint16_t duration, uint8_t num_events)
{
	uint16_t ret = 0;
	rtk_bt_le_ext_adv_start_t param = {
		.adv_handle = adv_handle,
		.duration = duration,
		.num_events = num_events,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_START_EXT_ADV, &param, sizeof(rtk_bt_le_ext_adv_start_t));

	return ret;
}

uint16_t rtk_bt_le_gap_stop_ext_adv(uint8_t adv_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_STOP_EXT_ADV, &adv_handle, sizeof(uint8_t));

	return ret;
}

uint16_t rtk_bt_le_gap_remove_ext_adv(uint8_t adv_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_REMOVE_EXT_ADV, &adv_handle, sizeof(uint8_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_ext_adv_handle_by_conn_handle(uint16_t conn_handle, uint8_t *adv_handle)
{
	uint16_t ret = 0;
	rtk_bt_le_get_eadv_by_conn_handle_param_t get_adv_hdl = {
		.conn_handle = conn_handle,
		.adv_handle = adv_handle,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_EXT_ADV_HANDLE_BY_CONN_HANDLE, &get_adv_hdl, sizeof(get_adv_hdl));

	return ret;
}

uint16_t rtk_bt_le_gap_ext_connect(rtk_bt_le_ext_create_conn_param_t *p_ext_conn_param)
{
	uint16_t ret = 0;
	int i = 0;

	if (!p_ext_conn_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if ((false == p_ext_conn_param->init_phys[0]) &&
		(false == p_ext_conn_param->init_phys[1]) &&
		(false == p_ext_conn_param->init_phys[2])) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	for (i = 0; i < 3; i++) {
		if (false == p_ext_conn_param->init_phys[i]) {
			continue;
		}

		if (p_ext_conn_param->scan_interval[i] < 0x04) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_ext_conn_param->scan_window[i] < 0x04 ||
			p_ext_conn_param->scan_window[i] > p_ext_conn_param->scan_interval[i]) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_ext_conn_param->conn_interval_min[i] < 0x06 ||
			p_ext_conn_param->conn_interval_min[i] > 0x0C80) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_ext_conn_param->conn_interval_max[i] < 0x06 ||
			p_ext_conn_param->conn_interval_max[i] > 0x0C80 ||
			p_ext_conn_param->conn_interval_max[i] < p_ext_conn_param->conn_interval_min[i]) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_ext_conn_param->conn_latency[i] > 0x01F3) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_ext_conn_param->supv_timeout[i] < 0x0A || p_ext_conn_param->supv_timeout[i] > 0x0C80) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_EXT_CONN,
						  p_ext_conn_param, sizeof(rtk_bt_le_ext_create_conn_param_t));

	return ret;
}
#endif

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
uint16_t rtk_bt_le_gap_start_pa(rtk_bt_le_pa_param_t *param)
{
	uint16_t ret = 0;

	if (param->pa_interval_min < 6 || param->pa_interval_max < 6 || (!(param->pa_interval_min <= param->pa_interval_max))) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (param->pa_len > 252) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_START_PA, param, sizeof(rtk_bt_le_pa_param_t));

	if (ret) {
		/* PA needs RTK_BT_LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED or RTK_BT_LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED */
		BT_LOGE("Check if EA event prop is valid for PA.\r\n");
	}

	return ret;
}

uint16_t rtk_bt_le_gap_stop_pa(uint8_t adv_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_STOP_PA, &adv_handle, sizeof(uint8_t));

	return ret;
}

uint16_t rtk_bt_le_gap_update_pa(uint8_t adv_handle, bool update_did_only, uint8_t *data, uint16_t len)
{
	uint16_t ret = 0;

	rtk_bt_le_pa_update_t param = {
		.adv_handle = adv_handle,
		.update_did_only = update_did_only,
		.pa_data = data,
		.pa_len = len
	};

	if ((update_did_only == false) && (data == NULL || len == 0)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_UPDATE_PA, &param, sizeof(rtk_bt_le_pa_update_t));

	return ret;
}
#endif

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
uint16_t rtk_bt_le_gap_pa_sync_modify_adv_list(rtk_bt_le_pa_sync_advlist_op_t operation, rtk_bt_le_addr_t adv_addr, uint8_t adv_sid)
{
	uint16_t ret = 0;

	rtk_bt_le_pa_sync_modify_advlist_t param = {
		.operation = operation,
		.adv_addr = adv_addr,
		.adv_sid = adv_sid,
	};

	if (operation > RTK_BT_LE_PA_SYNC_ADV_LIST_OP_REMOVE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if ((operation != RTK_BT_LE_PA_SYNC_ADV_LIST_OP_CLEAR) &&
		(adv_addr.type != RTK_BT_LE_ADDR_TYPE_PUBLIC) &&
		(adv_addr.type != RTK_BT_LE_ADDR_TYPE_RANDOM)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PA_SYNC_MODIFY_ADV_LIST, &param, sizeof(rtk_bt_le_pa_sync_modify_advlist_t));

	return ret;
}

uint16_t rtk_bt_le_gap_pa_sync_create(rtk_bt_le_pa_sync_create_t *param)
{
	uint16_t ret = 0;

	if (param->p_sync_id == NULL) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (param->skip > 0x01F3) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (param->sync_timeout < 0x000A || param->sync_timeout > 0x4000) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!(param->options & RTK_BT_LE_PA_SYNC_CREATE_OPTIONS_USE_PERIODIC_ADV_LIST) &&
		(param->adv_addr.type != RTK_BT_LE_ADDR_TYPE_PUBLIC) &&
		(param->adv_addr.type != RTK_BT_LE_ADDR_TYPE_RANDOM)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE, param, sizeof(rtk_bt_le_pa_sync_create_t));

	return ret;
}

uint16_t rtk_bt_le_gap_pa_sync_terminate(uint8_t sync_id)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PA_SYNC_TERMINATE, &sync_id, sizeof(uint8_t));

	return ret;
}

// #if defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
// uint16_t rtk_bt_le_gap_pa_sync_report_set(uint8_t sync_id, bool report_enable, bool duplicate_filter_enable)
// {
//  uint16_t ret = 0;

//  rtk_bt_le_pa_sync_report_set_t param = {
//      .sync_id = sync_id,
//      .report_enable = report_enable,
//      .duplicate_filter_enable = duplicate_filter_enable,
//  };

//  ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PA_SYNC_REPORT, &param, sizeof(rtk_bt_le_pa_sync_report_set_t));

//  return ret;
// }
// #endif
#endif

#if defined(RTK_BLE_5_1_PAST_SENDER_SUPPORT) && RTK_BLE_5_1_PAST_SENDER_SUPPORT
uint16_t rtk_bt_le_gap_past_send(uint16_t conn_handle, uint16_t service_data, bool use_sync_id, uint8_t idx)
{
	uint16_t ret = 0;
	rtk_bt_le_past_send_t param = {
		.conn_handle = conn_handle,
		.service_data = service_data,
		.use_sync_id = use_sync_id,
		.idx = idx
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PAST_TRANSFER, &param, sizeof(rtk_bt_le_past_send_t));

	return ret;
}
#endif

#if defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
uint16_t rtk_bt_le_gap_past_recipient_set(rtk_bt_le_past_recv_param_t *param)
{
	uint16_t ret = 0;

	if (param->skip > 0x01F3) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (param->sync_timeout < 0x000A || param->sync_timeout > 0x4000) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PAST_RECV_SET, param, sizeof(rtk_bt_le_past_recv_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_default_past_recipient_set(rtk_bt_le_past_recv_param_t *param)
{
	uint16_t ret = 0;

	if (param->skip > 0x01F3) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (param->sync_timeout < 0x000A || param->sync_timeout > 0x4000) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_DEFAULT_PAST_RECV_SET, param, sizeof(rtk_bt_le_past_recv_param_t));

	return ret;
}
#endif

uint16_t rtk_bt_le_gap_scan_info_filter(rtk_bt_le_scan_info_filter_param_t *p_scan_info)
{
	uint16_t ret = 0;

	if (!p_scan_info) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER,
						  p_scan_info, sizeof(rtk_bt_le_scan_info_filter_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_scan_param(rtk_bt_le_scan_param_t *p_gap_scan_param)
{
	uint16_t ret = 0;

	if (!p_gap_scan_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (p_gap_scan_param->type > RTK_BT_LE_SCAN_TYPE_ACTIVE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if ((p_gap_scan_param->interval < 0x04) || (p_gap_scan_param->interval > 0x4000)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if ((p_gap_scan_param->window < 0x04) || (p_gap_scan_param->window > 0x4000) || (p_gap_scan_param->window > p_gap_scan_param->interval)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_gap_scan_param->filter_policy > RTK_BT_LE_SCAN_FILTER_ALLOW_WLIST_RPA_DIR) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_gap_scan_param->duplicate_opt > RTK_BT_LE_SCAN_DUPLICATE_ENABLE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_SCAN_PARAM, (void *)p_gap_scan_param, sizeof(rtk_bt_le_scan_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_scan_param(rtk_bt_le_scan_param_t *pscan_param)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_SCAN_PARAM, pscan_param, sizeof(rtk_bt_le_scan_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_start_scan(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_START_SCAN, NULL, 0);

	return ret;
}

uint16_t rtk_bt_le_gap_stop_scan(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_STOP_SCAN, NULL, 0);

	return ret;
}

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
uint16_t rtk_bt_le_gap_set_ext_scan_param(rtk_bt_le_ext_scan_param_t *p_param)
{
	uint16_t ret = 0;

	if (!p_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if ((p_param->phys[0] == false) && (p_param->phys[1] == false)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_param->phys[0]) {
		if (p_param->type[0] > RTK_BT_LE_SCAN_TYPE_ACTIVE) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_param->interval[0] < 0x04) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_param->window[0] < 0x04 ||
			p_param->window[0] > p_param->interval[0]) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
	}

	if (p_param->phys[1]) {
		if (p_param->type[1] > RTK_BT_LE_SCAN_TYPE_ACTIVE) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_param->interval[1] < 0x04) {
			return RTK_BT_ERR_PARAM_INVALID;
		}

		if (p_param->window[1] < 0x04 ||
			p_param->window[1] > p_param->interval[1]) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
	}

	if (p_param->filter_policy > RTK_BT_LE_SCAN_FILTER_ALLOW_WLIST_RPA_DIR) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_param->duplicate_opt > RTK_BT_LE_SCAN_DUPLICATE_ENABLE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	/* duration time should not be greater than period time */
	if ((p_param->period != 0) && (p_param->duration * 10 > p_param->period * 1280)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_EXT_SCAN_SET_PARAM, (void *)p_param, sizeof(rtk_bt_le_ext_scan_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_start_ext_scan(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_EXT_SCAN_START, NULL, 0);

	return ret;
}

uint16_t rtk_bt_le_gap_stop_ext_scan(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_EXT_SCAN_STOP, NULL, 0);

	return ret;
}
#endif

uint16_t rtk_bt_le_gap_connect(rtk_bt_le_create_conn_param_t *p_conn_param)
{
	uint16_t ret = 0;

	if (!p_conn_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (p_conn_param->scan_interval < 0x04 || p_conn_param->scan_interval > 0x4000) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->scan_window < 0x04 || p_conn_param->scan_window > 0x4000 ||
		p_conn_param->scan_window > p_conn_param->scan_interval) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->filter_policy > RTK_BT_LE_CONN_FILTER_WITH_WHITELIST) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->peer_addr.type > RTK_BT_LE_ADDR_TYPE_RPA_RANDOM) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->conn_interval_min < 0x06 || p_conn_param->conn_interval_min > 0x0C80) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->conn_interval_max < 0x06 || p_conn_param->conn_interval_max > 0x0C80 ||
		p_conn_param->conn_interval_max < p_conn_param->conn_interval_min) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->conn_latency > 0x01F3) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_conn_param->supv_timeout < 0x0A || p_conn_param->supv_timeout > 0x0C80) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_CONN,
						  p_conn_param, sizeof(rtk_bt_le_create_conn_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_connect_cancel(rtk_bt_le_addr_t *cancel_addr)
{
	uint16_t ret = 0;

	if (!cancel_addr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (cancel_addr->type > RTK_BT_LE_ADDR_TYPE_RPA_RANDOM) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_CONN_CANCEL,
						  cancel_addr, sizeof(rtk_bt_le_addr_t));
	return ret;
}

uint16_t rtk_bt_le_gap_disconnect(uint16_t conn_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_DISCONN,
						  &conn_handle, sizeof(uint16_t));

	return ret;
}

uint16_t rtk_bt_le_gap_disconnect_with_reason(uint16_t conn_handle, uint8_t reason)
{
	uint16_t ret = 0;
	rtk_bt_le_disconn_with_reason_param_t disconn_with_reason = {
		.conn_handle = conn_handle,
		.reason = reason,
	};

	if (reason != RTK_BT_HCI_ERR_REMOTE_USER_TERMINATE &&
		reason != RTK_BT_HCI_ERR_REMOTE_LOW_RESOURCE &&
		reason != RTK_BT_HCI_ERR_REMOTE_POWER_OFF &&
		reason != RTK_BT_HCI_ERR_UNSUPPORTED_REMOTE_FEAT &&
		reason != RTK_BT_HCI_ERR_UNACCEPTABLE_CONN_INTERVAL) {
		return RTK_BT_ERR_UNSUPPORTED;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_DISCONN_WITH_REASON,
						  &disconn_with_reason, sizeof(rtk_bt_le_disconn_with_reason_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_update_conn_param(rtk_bt_le_update_conn_param_t *p_update_conn_param)
{
	uint16_t ret = 0;

	if (!p_update_conn_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (p_update_conn_param->conn_interval_min < 0x06 ||
		p_update_conn_param->conn_interval_min > 0x0C80) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_update_conn_param->conn_interval_max < 0x06 ||
		p_update_conn_param->conn_interval_max > 0x0C80 ||
		p_update_conn_param->conn_interval_max < p_update_conn_param->conn_interval_min) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_update_conn_param->conn_latency > 0x01F3) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_update_conn_param->supv_timeout < 0x0A || p_update_conn_param->supv_timeout > 0x0C80) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_UPDATE_CONN_PARAM,
						  p_update_conn_param, sizeof(rtk_bt_le_update_conn_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_modify_whitelist(rtk_bt_le_modify_wl_param_t *p_wl_op_param)
{
	uint16_t ret = 0;

	if (!p_wl_op_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (p_wl_op_param->op >= RTK_BT_LE_WHITELIST_RESERVED) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_wl_op_param->op != RTK_BT_LE_WHITELIST_CLEAR) {
		if (p_wl_op_param->addr.type > RTK_BT_LE_ADDR_TYPE_RANDOM &&
			p_wl_op_param->addr.type != RTK_BT_LE_ADDR_TYPE_PEER_ANONYMOUS) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_MODIFY_WHITELIST,
						  p_wl_op_param, sizeof(rtk_bt_le_modify_wl_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_read_rssi(uint16_t conn_handle, int8_t *p_rssi)
{
	uint16_t ret = 0;
	rtk_bt_le_read_rssi_param_t read_rssi = {0};
	read_rssi.conn_handle = conn_handle;
	read_rssi.p_rssi = p_rssi;

	if (!p_rssi) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_READ_RSSI,
						  &read_rssi, sizeof(rtk_bt_le_read_rssi_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_read_remote_version(uint16_t conn_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_READ_REMOTE_VERSION,
						  &conn_handle, sizeof(uint16_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_dev_state(rtk_bt_le_gap_dev_state_t *p_dev_state)
{
	uint16_t ret = 0;

	if (!p_dev_state) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_DEV_STATE,
						  p_dev_state, sizeof(rtk_bt_le_gap_dev_state_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_active_conn(rtk_bt_le_get_active_conn_t *p_active_conn)
{
	uint16_t ret = 0;

	if (!p_active_conn) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN,
						  p_active_conn, sizeof(rtk_bt_le_get_active_conn_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_conn_handle_by_addr(rtk_bt_le_addr_t *p_addr, uint16_t *p_conn_handle)
{
	uint16_t ret = 0;
	rtk_bt_le_get_conn_handle_by_addr_param_t conn_handle_by_addr;
	memset(&conn_handle_by_addr, 0, sizeof(rtk_bt_le_get_conn_handle_by_addr_param_t));
	memcpy((void *)&conn_handle_by_addr.addr, (void *)p_addr, sizeof(rtk_bt_le_addr_t));
	conn_handle_by_addr.p_conn_handle = p_conn_handle;

	if (!p_conn_handle) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR,
						  &conn_handle_by_addr, sizeof(rtk_bt_le_get_conn_handle_by_addr_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_conn_info(uint16_t conn_handle, rtk_bt_le_conn_info_t *p_conn_info)
{
	uint16_t ret = 0;
	rtk_bt_le_get_conn_info_param_t get_info = {0};
	get_info.conn_handle = conn_handle;
	get_info.p_conn_info = p_conn_info;

	if (!p_conn_info) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_CONN_INFO,
						  &get_info, sizeof(rtk_bt_le_get_conn_info_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_get_mtu_size(uint16_t conn_handle, uint16_t *p_mtu_size)
{
	uint16_t ret = 0;
	rtk_bt_le_get_mtu_size_param_t get_mtu = {0};
	get_mtu.conn_handle = conn_handle;
	get_mtu.p_mtu_size = p_mtu_size;

	if (!p_mtu_size) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_MTU_SIZE,
						  &get_mtu, sizeof(rtk_bt_le_get_mtu_size_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_max_mtu_size(uint16_t mtu_size)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_MAX_MTU_SIZE,
						  &mtu_size, sizeof(uint16_t));

	return ret;
}

uint16_t rtk_bt_le_gap_set_channels(uint8_t *p_chan_map)
{
	uint16_t ret = 0;

	if (!p_chan_map) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_CHANNELS,
						  p_chan_map, RTK_BT_LE_CHAN_MAP_LEN);

	return ret;
}

#if defined(RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT) && RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT
uint16_t rtk_bt_le_gap_set_data_len(rtk_bt_le_set_datalen_param_t *p_datalen_param)
{
	uint16_t ret = 0;

	if (!p_datalen_param) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_datalen_param->max_tx_octets < 0x1B || p_datalen_param->max_tx_octets > 0xFB) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_datalen_param->max_tx_time < 0x148 || p_datalen_param->max_tx_time > 0x4290) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_DATA_LEN,
						  p_datalen_param, sizeof(rtk_bt_le_set_datalen_param_t));

	return ret;
}
#endif  /* RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT */

#if defined(RTK_BLE_5_0_SET_PHYS_SUPPORT) && RTK_BLE_5_0_SET_PHYS_SUPPORT
uint16_t rtk_bt_le_gap_set_phy(rtk_bt_le_set_phy_param_t *p_phy_param)
{
	uint16_t ret = 0;

	if (!p_phy_param ||
		p_phy_param->all_phys > 1 << 2 || p_phy_param->tx_phys > 1 << 3 ||
		p_phy_param->rx_phys > 1 << 3 || p_phy_param->phy_options > 1 << 2) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_PHY,
						  p_phy_param, sizeof(rtk_bt_le_set_phy_param_t));

	return ret;
}
#endif  /* RTK_BLE_5_0_SET_PHYS_SUPPORT */

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
uint16_t rtk_bt_le_gap_privacy_init(bool whitelist)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PRIVACY_INIT,
						  &whitelist, sizeof(bool));

	return ret;
}

uint16_t rtk_bt_le_gap_set_privacy_mode(rtk_bt_le_ident_addr_type_t peer_ident_addr_type,
										uint8_t *peer_addr, rtk_bt_le_privacy_mode_t privacy_mode)
{
	uint16_t ret = 0;
	rtk_bt_le_set_privacy_mode_param_t param = {
		.peer_ident_addr_type = peer_ident_addr_type,
		.peer_addr = peer_addr,
		.privacy_mode = privacy_mode,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_PRIVACY_MODE,
						  &param, sizeof(rtk_bt_le_set_privacy_mode_param_t));
	return ret;
}

uint16_t rtk_bt_le_gap_read_local_resolv_addr(rtk_bt_le_ident_addr_type_t peer_ident_addr_type,
											  uint8_t *peer_addr, uint8_t *local_rpa)
{
	uint16_t ret = 0;
	rtk_bt_le_read_local_rpa_param_t param = {
		.peer_ident_addr_type = peer_ident_addr_type,
		.peer_addr = peer_addr,
		.local_rpa = local_rpa,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_READ_LOCAL_RESOLV_ADDR,
						  &param, sizeof(rtk_bt_le_read_local_rpa_param_t));
	return ret;
}

uint16_t rtk_bt_le_gap_read_peer_resolv_addr(rtk_bt_le_ident_addr_type_t peer_ident_addr_type,
											 uint8_t *peer_addr, uint8_t *peer_rpa)
{
	uint16_t ret = 0;
	rtk_bt_le_read_peer_rpa_param_t param = {
		.peer_ident_addr_type = peer_ident_addr_type,
		.peer_addr = peer_addr,
		.peer_rpa = peer_rpa,
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_READ_PEER_RESOLV_ADDR,
						  &param, sizeof(rtk_bt_le_read_peer_rpa_param_t));
	return ret;
}
#endif  /* RTK_BLE_PRIVACY_SUPPORT */

uint16_t rtk_bt_le_sm_set_pairing_mode(rtk_bt_le_pairing_mode_t pairing_mode)
{
	uint16_t ret = 0;

	if (pairing_mode > RTK_PAIRING_MODE_PAIRABLE) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_PAIRING_MODE,
						  &pairing_mode, sizeof(pairing_mode));

	return ret;
}

uint16_t rtk_bt_le_sm_set_security_param(rtk_bt_le_security_param_t *p_sec_param)
{
	uint16_t ret = 0;

	if (!p_sec_param ||
		p_sec_param->io_cap >= RTK_IO_CAP_MAX ||
		p_sec_param->fixed_key >= RTK_PAIRING_PASSKEY_MAX) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_SET_SEC_PARAM,
						  p_sec_param, sizeof(rtk_bt_le_security_param_t));

	return ret;
}

uint16_t rtk_bt_le_sm_get_security_param(rtk_bt_le_security_param_t *p_sec_param)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_SEC_PARAM,
						  p_sec_param, sizeof(rtk_bt_le_security_param_t));

	return ret;
}

uint16_t rtk_bt_le_sm_start_security(uint16_t conn_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_START_SECURITY,
						  &conn_handle, sizeof(uint16_t));

	return ret;
}

uint16_t rtk_bt_le_sm_pairing_confirm(rtk_bt_le_pair_cfm_t *p_pair_cfm)
{
	uint16_t ret = 0;

	if (!p_pair_cfm) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PAIRING_CONFIRM,
						  p_pair_cfm, sizeof(rtk_bt_le_pair_cfm_t));

	return ret;
}

uint16_t rtk_bt_le_sm_passkey_entry(rtk_bt_le_auth_key_input_t *p_key_input)
{
	uint16_t ret = 0;

	if (!p_key_input || p_key_input->passkey > RTK_PAIRING_PASSKEY_MAX) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PASSKEY_ENTRY,
						  p_key_input, sizeof(rtk_bt_le_auth_key_input_t));

	return ret;
}

uint16_t rtk_bt_le_sm_passkey_confirm(rtk_bt_le_auth_key_confirm_t *p_key_cfm)
{
	uint16_t ret = 0;

	if (!p_key_cfm) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_PASSKEY_CONFIRM,
						  p_key_cfm, sizeof(rtk_bt_le_auth_key_confirm_t));

	return ret;
}

#if defined(RTK_BLE_SMP_OOB_SUPPORT) && RTK_BLE_SMP_OOB_SUPPORT
uint16_t rtk_bt_le_sm_set_oob_tk(rtk_bt_le_set_oob_key_t *p_set_oob_key)
{
	uint16_t ret = 0;

	if (!p_set_oob_key) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_OOB_KEY_INPUT,
						  p_set_oob_key, sizeof(rtk_bt_le_set_oob_key_t));

	return ret;
}

uint16_t rtk_bt_le_sm_get_sc_local_oob(rtk_bt_le_sc_local_oob_data_t *local_oob)
{
	uint16_t ret = 0;

	if (!local_oob) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_SC_LOCAL_OOB,
						  local_oob, sizeof(rtk_bt_le_sc_local_oob_data_t));

	return ret;
}

uint16_t rtk_bt_le_sm_input_sc_peer_oob(rtk_bt_le_sc_peer_oob_data_t *peer_oob)
{
	uint16_t ret = 0;

	if (!peer_oob) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_INPUT_SC_PEER_OOB,
						  peer_oob, sizeof(rtk_bt_le_sc_peer_oob_data_t));

	return ret;
}
#endif  /* RTK_BLE_SMP_OOB_SUPPORT */

uint16_t rtk_bt_le_sm_get_bond_num(uint8_t *bond_num)
{
	uint16_t ret = 0;

	if (!bond_num) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_BOND_NUM,
						  bond_num, sizeof(uint8_t));

	return ret;
}

bool rtk_bt_le_sm_is_device_bonded(rtk_bt_le_addr_t *paddr)
{
	bool ret = false;
	uint8_t i, bond_size = 0;

	if (rtk_bt_le_sm_get_bond_num(&bond_size) || bond_size == 0) {
		return false;
	}

	rtk_bt_le_bond_info_t *bond_info =
		(rtk_bt_le_bond_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,
												bond_size * sizeof(rtk_bt_le_bond_info_t));
	memset(bond_info, 0, bond_size * sizeof(rtk_bt_le_bond_info_t));

	if (rtk_bt_le_sm_get_bond_info(bond_info, &bond_size) == RTK_BT_OK) {
		for (i = 0; i < bond_size; i++) {
			if (paddr->type < RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC &&
				(memcmp(bond_info[i].remote_addr.addr_val, paddr->addr_val, RTK_BD_ADDR_LEN) == 0 ||
				 memcmp(bond_info[i].ident_addr.addr_val, paddr->addr_val, RTK_BD_ADDR_LEN) == 0)) {
				ret = true;
				break;
			}
			/* If the remote connected device use RPA, we shall use indenty addr to judge if
			the device is bonded, because the remote device address in air packets may often change  */
			if (paddr->type >= RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC &&
				memcmp(bond_info[i].ident_addr.addr_val, paddr->addr_val, RTK_BD_ADDR_LEN) == 0) {
				ret = true;
				break;
			}
		}
	}

	osif_mem_free(bond_info);
	return ret;
}

uint16_t rtk_bt_le_sm_get_bond_info(rtk_bt_le_bond_info_t *p_bond_info, uint8_t *size)
{
	uint16_t ret = 0;
	rtk_bt_le_get_bond_info_param_t get_bondinfo = {0};

	if (!p_bond_info || !size) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	get_bondinfo.p_bond_info = p_bond_info;
	get_bondinfo.p_size = size;
	get_bondinfo.size = *size;
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_BOND_INFO,
						  &get_bondinfo, sizeof(rtk_bt_le_get_bond_info_param_t));

	return ret;
}

uint16_t rtk_bt_le_sm_delete_bond_device(rtk_bt_le_addr_t *paddr)
{
	uint16_t ret = 0;

	if (!paddr) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_DELETE_BOND_DEVICE,
						  paddr, sizeof(rtk_bt_le_addr_t));

	return ret;
}

uint16_t rtk_bt_le_sm_clear_bond_list(void)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_CLEAR_BOND_LIST, NULL, 0);

	return ret;
}

uint16_t rtk_bt_le_gap_get_tx_pending_num(uint16_t conn_handle, uint16_t *p_tx_pending_num)
{
	uint16_t ret = 0;
	rtk_bt_le_get_tx_pending_num_param_t param = {0};

	if (!p_tx_pending_num) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	param.conn_handle = conn_handle;
	param.p_tx_pending_num = p_tx_pending_num;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM,
						  (void *)&param, sizeof(rtk_bt_le_get_tx_pending_num_param_t));

	return ret;
}

#if defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT
uint16_t rtk_bt_le_gap_read_local_tx_power(uint16_t conn_handle, rtk_bt_le_txpower_phy_t phy,
										   int8_t *cur_txpower, int8_t *max_txpower)
{
	uint16_t ret = 0;
	rtk_bt_le_txpower_read_local_t param = {
		.conn_handle = conn_handle,
		.phy = phy,
		.cur_txpower = cur_txpower,
		.max_txpower = max_txpower
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_READ_LOCAL_TX_POWER,
						  &param, sizeof(rtk_bt_le_txpower_read_local_t));

	return ret;
}

uint16_t rtk_bt_le_gap_read_remote_tx_power(uint16_t conn_handle, rtk_bt_le_txpower_phy_t phy)
{
	uint16_t ret = 0;
	rtk_bt_le_txpower_read_remote_t param = {
		.conn_handle = conn_handle,
		.phy = phy
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_READ_REMOTE_TX_POWER,
						  &param, sizeof(rtk_bt_le_txpower_read_remote_t));

	return ret;
}

uint16_t rtk_bt_le_gap_tx_power_report_set(uint16_t conn_handle, bool local_enable, bool remote_enable)
{
	uint16_t ret = 0;
	rtk_bt_le_txpower_report_set_t param = {
		.conn_handle = conn_handle,
		.local_enable = local_enable,
		.remote_enable = remote_enable
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_TX_POWER_REPORT_SET,
						  &param, sizeof(rtk_bt_le_txpower_report_set_t));

	return ret;
}
#endif

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT

uint16_t rtk_bt_le_gap_get_antenna_info(rtk_bt_le_gap_antenna_info_t *antenna_info)
{
	uint16_t ret = 0;

	if (!antenna_info) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_GET_ANTENNA_INFO,
						  antenna_info,
						  sizeof(rtk_bt_le_gap_antenna_info_t));

	return ret;
}

uint16_t rtk_bt_le_gap_connless_cte_rx_start(uint8_t sync_id, rtk_bt_le_gap_connless_cte_rx_param_t *params)
{
	uint16_t ret = 0;
	rtk_bt_le_gap_connless_cte_rx_start_t start_param = {
		.sync_id = sync_id,
		.param = params
	};

	if (!params) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_SLOT_DURATION_VALUE_IN_RANGE(params->slot_durations)) {
		BT_LOGE("CTE rx param slot_durations=%u invalid\r\n", params->slot_durations);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_MAX_SAMPLED_CTES_VALUE_IN_RANGE(params->max_sampled_ctes)) {
		BT_LOGE("CTE rx param max_sampled_ctes=%u invalid\r\n", params->max_sampled_ctes);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONNLESS_CTE_RX_START,
						  &start_param,
						  sizeof(rtk_bt_le_gap_connless_cte_rx_start_t));

	return ret;
}

uint16_t rtk_bt_le_gap_connless_cte_rx_stop(uint8_t sync_id)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONNLESS_CTE_RX_STOP,
						  &sync_id,
						  sizeof(uint8_t));

	return ret;
}

uint16_t rtk_bt_le_gap_conn_cte_rx_start(uint16_t conn_handle,
										 rtk_bt_le_gap_conn_cte_rx_param_t *rx_params)
{
	uint16_t ret = 0;

	rtk_bt_le_gap_conn_cte_rx_t start = {
		.conn_handle = conn_handle,
		.enable = true,
		.p_rx_param = rx_params
	};

	if (!rx_params) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_SLOT_DURATION_VALUE_IN_RANGE(rx_params->slot_durations)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_LEN_VALUE_IN_RANGE(rx_params->req_cte_len)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_TYPE_VALUE_IN_RANGE(rx_params->req_cte_type)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (rx_params->req_cte_type == RTK_BT_LE_GAP_CTE_TYPE_AOA && !RTK_BLE_GAP_CTE_NUM_ANT_IDS_VALUE_IN_RANGE(rx_params->num_ant_ids)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONN_CTE_RX_START,
						  &start,
						  sizeof(rtk_bt_le_gap_conn_cte_rx_t));

	return ret;
}

uint16_t rtk_bt_le_gap_conn_cte_rx_stop(uint16_t conn_handle)
{
	uint16_t ret = 0;

	rtk_bt_le_gap_conn_cte_rx_t stop = {
		.conn_handle = conn_handle,
		.enable = false,
		NULL,
		NULL
	};

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONN_CTE_RX_STOP,
						  &stop,
						  sizeof(rtk_bt_le_gap_conn_cte_rx_t));

	return ret;
}

uint16_t rtk_bt_le_gap_conn_cte_tx_start(uint16_t conn_handle, rtk_bt_le_gap_conn_cte_tx_param_t *params)
{
	uint16_t ret = 0;
	bool aod;

	rtk_bt_le_gap_conn_cte_tx_start_t start = {
		.conn_handle = conn_handle,
		.param = params
	};

	if (!params) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_TYPES_VALUE_IN_RANGE(params->cte_types)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	aod = params->cte_types & (RTK_BT_LE_GAP_CTE_TYPE_AOD_1US | RTK_BT_LE_GAP_CTE_TYPE_AOD_2US);
	if (aod && (!RTK_BLE_GAP_CTE_NUM_ANT_IDS_VALUE_IN_RANGE(params->num_ant_ids) || !params->ant_ids)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONN_CTE_TX_START,
						  &start,
						  sizeof(rtk_bt_le_gap_conn_cte_tx_start_t));

	return ret;
}

uint16_t rtk_bt_le_gap_conn_cte_tx_stop(uint16_t conn_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONN_CTE_TX_STOP,
						  &conn_handle,
						  sizeof(uint16_t));

	return ret;
}
#if ((defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT))

static uint16_t rtk_bt_le_gap_connless_cte_tx_enable(uint8_t adv_handle, rtk_bt_le_gap_connless_cte_tx_param_t *params)
{
	uint16_t ret = 0;
	bool aod;

	rtk_bt_le_gap_connless_cte_tx_start_t start = {
		.adv_handle = adv_handle,
		.param = params
	};

	if (!params) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_LEN_VALUE_IN_RANGE(params->cte_len)) {
		BT_LOGE("CTE connectionless tx param cte_len=%u invalid\r\n", params->cte_len);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_TYPE_VALUE_IN_RANGE(params->cte_type)) {
		BT_LOGE("CTE connectionless tx param cte_type=%u invalid\r\n", params->cte_type);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!RTK_BLE_GAP_CTE_CNT_VALUE_IN_RANGE(params->cte_count)) {
		BT_LOGE("CTE connectionless tx param cte_count=%u invalid\r\n", params->cte_count);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	aod = (params->cte_type == RTK_BT_LE_GAP_CTE_TYPE_AOD_1US) || (params->cte_type == RTK_BT_LE_GAP_CTE_TYPE_AOD_2US);
	if (aod && (!params->ant_ids || !RTK_BLE_GAP_CTE_NUM_ANT_IDS_VALUE_IN_RANGE(params->num_ant_ids))) {
		BT_LOGE("CTE connectionless tx param antenna invalid, num_ant_ids=%u, ant_ids=%08x\r\n", params->num_ant_ids, params->ant_ids);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONNLESS_CTE_TX_START,
						  &start,
						  sizeof(rtk_bt_le_gap_connless_cte_tx_start_t));

	return ret;
}

static uint16_t rtk_bt_le_gap_connless_cte_tx_disable(uint8_t adv_handle)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP,
						  RTK_BT_LE_GAP_ACT_CONNLESS_CTE_TX_STOP,
						  &adv_handle,
						  sizeof(uint8_t));

	return ret;
}

uint16_t rtk_bt_le_gap_connless_cte_tx_start(rtk_bt_le_gap_connless_cte_tx_param_t *p_cte_param,
											 rtk_bt_le_ext_adv_param_t *p_eadv_param,
											 rtk_bt_le_pa_param_t *p_pa_param,
											 uint8_t *p_adv_handle)
{
	uint16_t ret = 0;
	uint8_t  adv_handle;
	if (!p_cte_param || !p_eadv_param || !p_pa_param || !p_adv_handle) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	ret = rtk_bt_le_gap_create_ext_adv(p_eadv_param, &adv_handle);
	if (ret) {
		BT_LOGE("[LE GAP]Create eadv fail for connless CTE TX, ret=%u\r\n", ret);
		return ret;
	}

	p_pa_param->adv_handle = adv_handle;
	ret = rtk_bt_le_gap_start_pa(p_pa_param);
	if (ret) {
		BT_LOGE("[LE GAP]Start PA fail for connless CTE TX, ret=%u\r\n", ret);
		rtk_bt_le_gap_remove_ext_adv(adv_handle);
		return ret;
	}

	ret = rtk_bt_le_gap_connless_cte_tx_enable(adv_handle, p_cte_param);
	if (ret) {
		BT_LOGE("[LE GAP]Enable CTE TX fail for connless CTE TX, ret=%u\r\n", ret);
		rtk_bt_le_gap_stop_pa(adv_handle);
		rtk_bt_le_gap_remove_ext_adv(adv_handle);
		return ret;
	}

	ret = rtk_bt_le_gap_start_ext_adv(adv_handle, 0, 0);
	if (ret) {
		BT_LOGE("[LE GAP]Start eadv fail for connless CTE TX, ret=%u\r\n", ret);
		rtk_bt_le_gap_connless_cte_tx_disable(adv_handle);
		rtk_bt_le_gap_stop_pa(adv_handle);
		rtk_bt_le_gap_remove_ext_adv(adv_handle);
		return ret;
	}

	*p_adv_handle = adv_handle;
	return ret;
}



uint16_t rtk_bt_le_gap_connless_cte_tx_stop(uint8_t adv_handle)
{
	uint16_t ret = 0;
	ret = rtk_bt_le_gap_connless_cte_tx_disable(adv_handle);
	ret = rtk_bt_le_gap_stop_pa(adv_handle);
	ret = rtk_bt_le_gap_stop_ext_adv(adv_handle);
	ret = rtk_bt_le_gap_remove_ext_adv(adv_handle);
	return ret;
}

#endif /* RTK_BLE_5_0_USE_EXTENDED_ADV && RTK_BLE_5_0_PA_ADV_SUPPORT */

#endif /* RTK_BLE_5_1_CTE_SUPPORT */

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
uint16_t rtk_bt_le_gap_coc_register_psm(uint8_t is_register, uint16_t le_psm)
{
	uint16_t ret = 0;

	rtk_bt_le_coc_psm_reg_param_t param = {
		.is_register = is_register,
		.le_psm = le_psm,
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_REG_PSM,
						  &param, sizeof(rtk_bt_le_coc_psm_reg_param_t));
	return ret;
}

uint16_t rtk_bt_le_gap_coc_set_psm_security(uint16_t le_psm, uint8_t active,
											rtk_bt_le_coc_security_mode_t sec_mode,
											uint8_t key_size)
{
	uint16_t ret = 0;

	if (sec_mode > RTK_BT_LE_COC_SEC_SECURE_CONN_AUTHEN) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	rtk_bt_le_coc_set_psm_sec_param_t sec_param = {
		.le_psm = le_psm,
		.active = active,
		.sec_mode = sec_mode,
		.key_size = key_size,
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_SET_PSM_SECURITY,
						  &sec_param, sizeof(rtk_bt_le_coc_set_psm_sec_param_t));
	return ret;
}

uint16_t rtk_bt_le_gap_coc_set_param(rtk_bt_le_coc_param_type_t param_type, uint16_t value)
{
	uint16_t ret = 0;

	if (param_type > RTK_BT_LE_COC_PARAM_LOCAL_MTU) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	rtk_bt_le_coc_param_set_t param = {
		.param_type = param_type,
		.value = value,
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_SET_PARAM,
						  &param, sizeof(rtk_bt_le_coc_param_set_t));
	return ret;
}

uint16_t rtk_bt_le_gap_coc_get_chan_param(rtk_bt_le_coc_chan_param_type_t param_type,
										  uint16_t cid, uint16_t *value)
{
	uint16_t ret = 0;

	if (!value) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (param_type > RTK_BT_LE_COC_CHAN_PARAM_MTU) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	rtk_bt_le_coc_chan_param_get_t param = {
		.param_type = param_type,
		.cid = cid,
		.value = value,
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_GET_CHAN_PARAM,
						  &param, sizeof(rtk_bt_le_coc_chan_param_get_t));
	return ret;
}

uint16_t rtk_bt_le_gap_coc_connect(uint16_t conn_handle, uint16_t le_psm)
{
	uint16_t ret = 0;

	rtk_bt_le_coc_connect_param_t param = {
		.conn_handle = conn_handle,
		.le_psm = le_psm,
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_CONN,
						  &param, sizeof(rtk_bt_le_coc_connect_param_t));
	return ret;
}

uint16_t rtk_bt_le_gap_coc_disconnect(uint16_t cid)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_DISCONN,
						  &cid, sizeof(uint16_t));
	return ret;
}

uint16_t rtk_bt_le_gap_coc_send_data(uint16_t cid, uint16_t len, uint8_t *data)
{
	uint16_t ret = 0;

	if (!data) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	rtk_bt_le_coc_send_data_param_t param = {
		.cid = cid,
		.len = len,
		.data = data,
	};
	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_COC_SEND_DATA,
						  &param, sizeof(rtk_bt_le_coc_send_data_param_t));
	return ret;
}
#endif

uint16_t rtk_bt_le_gap_dtm_rx_test(rtk_bt_le_dtm_rx_param_t *p_rx_param, rtk_bt_le_dtm_rx_version_t rx_version)
{
	uint16_t ret = 0;
	rtk_bt_le_gap_act_t act;

	if (!p_rx_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (rx_version < RTK_BT_LE_DTM_RX_VERSION_V1 || rx_version > RTK_BT_LE_DTM_RX_VERSION_V3) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_rx_param->rx_channel > 0x27) {
		return RTK_BT_ERR_PARAM_INVALID;
	}
	act = RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V1;

	if (rx_version > RTK_BT_LE_DTM_RX_VERSION_V1) {
		if (p_rx_param->phy < RTK_BT_LE_DTM_RX_PHYS_1M || p_rx_param->phy > RTK_BT_LE_DTM_RX_PHYS_CODED) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_rx_param->mod_idx > RTK_BT_LE_DTM_MODULATION_INDEX_STABLE) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		act = RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V2;
	}

	if (rx_version > RTK_BT_LE_DTM_RX_VERSION_V2) {
		if (p_rx_param->exp_cte_len == 0x01 || p_rx_param->exp_cte_len > 0x14) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_rx_param->exp_cte_type > RTK_BT_LE_DTM_CTE_TYPE_AOD_2US_SLOT) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_rx_param->slot_durations < RTK_BT_LE_DTM_SLOT_DURATIONS_SWITCH_SAMPLE_1US ||
			p_rx_param->slot_durations > RTK_BT_LE_DTM_SLOT_DURATIONS_SWITCH_SAMPLE_2US) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_rx_param->sw_pattern_len < 0x02 || p_rx_param->sw_pattern_len > 0x4B) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_rx_param->p_antenna_ids == NULL) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		act = RTK_BT_LE_GAP_ACT_DTM_RX_TEST_V3;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, act, p_rx_param, sizeof(rtk_bt_le_dtm_rx_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_dtm_tx_test(rtk_bt_le_dtm_tx_param_t *p_tx_param, rtk_bt_le_dtm_tx_version_t tx_version)
{
	uint16_t ret = 0;
	rtk_bt_le_gap_act_t act;

	if (!p_tx_param) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (tx_version < RTK_BT_LE_DTM_TX_VERSION_V1 || tx_version > RTK_BT_LE_DTM_TX_VERSION_V4) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_tx_param->tx_channel > 0x27) {
		return RTK_BT_ERR_PARAM_INVALID;
	}
	if (p_tx_param->pkt_pl > RTK_BT_LE_DTM_PACKET_PAYLOAD_01) {
		return RTK_BT_ERR_PARAM_INVALID;
	}
	act = RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V1;

	if (tx_version > RTK_BT_LE_DTM_TX_VERSION_V1) {
		if (p_tx_param->phy < RTK_BT_LE_DTM_TX_PHYS_1M || p_tx_param->phy > RTK_BT_LE_DTM_TX_PHYS_CODED_S2) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		act = RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V2;
	}

	if (tx_version > RTK_BT_LE_DTM_TX_VERSION_V2) {
		if (p_tx_param->cte_len == 0x01 || p_tx_param->cte_len > 0x14) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_tx_param->cte_type > RTK_BT_LE_DTM_CTE_TYPE_AOD_2US_SLOT) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_tx_param->sw_pattern_len < 0x02 || p_tx_param->sw_pattern_len > 0x4B) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		if (p_tx_param->p_antenna_ids == NULL) {
			return RTK_BT_ERR_PARAM_INVALID;
		}
		act = RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V3;
	}

	if (tx_version > RTK_BT_LE_DTM_TX_VERSION_V3) {
		act = RTK_BT_LE_GAP_ACT_DTM_TX_TEST_V4;
	}

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, act, p_tx_param, sizeof(rtk_bt_le_dtm_tx_param_t));

	return ret;
}

uint16_t rtk_bt_le_gap_dtm_end(uint16_t *p_num_pkts)
{
	uint16_t ret = 0;

	ret = rtk_bt_send_cmd(RTK_BT_LE_GP_GAP, RTK_BT_LE_GAP_ACT_DTM_END_TEST, p_num_pkts, sizeof(uint16_t));

	return ret;
}