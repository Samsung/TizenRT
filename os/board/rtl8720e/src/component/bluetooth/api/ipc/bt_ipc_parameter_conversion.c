/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <platform_opts_bt.h>
#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <bt_api_config.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_avrcp.h>
#include <rtk_bt_a2dp.h>
#include <rtk_bt_spp.h>
#include <rtk_bt_hfp.h>
#include <rtk_bt_sdp.h>
#include <rtk_bt_le_iso.h>
#include <rtk_bt_le_audio.h>
#include <bt_ipc_cfg.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_profile_config.h>
#include <bt_ipc_parameter_conversion.h>

#define ALIGN_UP(a, size)       ((a+size-1) & (~(size-1)))

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
/* ===============================================rtk_bt_device ipc action======================================= */
uint16_t rtk_bt_device_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_DEVICE_IPC_ACT_BT_ENABLE) {
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)	
		rtk_bt_app_conf_t *param = (rtk_bt_app_conf_t *)dst;
		rtk_bt_le_audio_app_conf_t *le_audio_app_conf = &param->le_audio_app_conf;
		*le_audio_app_conf->pacs_param.p_sink_pac_id = *((int8_t *)src);
		*le_audio_app_conf->pacs_param.p_pac_sink_codec = *((int8_t *)src + 1);
		*le_audio_app_conf->pacs_param.p_source_pac_id = *((int8_t *)src + 2);
		*le_audio_app_conf->pacs_param.p_pac_source_codec = *((int8_t *)src + 3);
		if(le_audio_app_conf->profile_support & RTK_BT_LE_AUDIO_PROFILE_MCS)
			*le_audio_app_conf->mcs_param.p_mcs_id = *((int8_t *)src + 4);
#else 
		(void)src;
		(void)dst;
#endif
	}
	return RTK_BT_OK;
}

/* ===============================================rtk_bt_gap ipc action======================================= */
uint16_t rtk_gap_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)dst;
	(void)src;

	switch(act) {
#if RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_ACT_ECFC_REGISTER: {
		rtk_bt_gap_ecfc_register_t *param = (rtk_bt_gap_ecfc_register_t *)dst;
		memcpy(param->p_proto_id, (void *)src, sizeof(uint16_t));
		break;
	}
#endif

	default:
		break;
	}

	return RTK_BT_OK;
}

#if RTK_BT_5_2_L2C_ECFC_SUPPORT
static int gap_ipc_ecfc_send_data_push(uint8_t *buf, rtk_bt_gap_ecfc_send_data_t *param, 
									   uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_gap_ecfc_send_data_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->length;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gap_ecfc_send_data_t *)buf)->p_data = (uint8_t *)(struct_end - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("gap_ipc_ecfc_send_data_filter_push size is over deep copy buf\r\n");
	return -1;
}
#endif

void *bt_gap_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
#if RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_ACT_ECFC_SEND_DATA:
		ret = gap_ipc_ecfc_send_data_push((uint8_t *)host_msg->param_buf, (rtk_bt_gap_ecfc_send_data_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size);
		break;
#endif
	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}

	if (ret) {
		osif_mem_free(host_msg);
		host_msg = NULL;
	}

	return (void *)host_msg;
}

/* ===============================================rtk_bt_le_gap ipc action======================================= */
static struct act_mem_option rtk_bt_le_gap_act_mem_op_tab[] = {
	{RTK_BT_LE_GAP_ACT_GET_VERSION, sizeof(rtk_bt_le_version_info_t)},
	{RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR, sizeof(rtk_bt_le_addr_t)}, 
	{RTK_BT_LE_GAP_ACT_READ_RSSI, sizeof(int8_t)},
	{RTK_BT_LE_GAP_ACT_GET_BOND_NUM, sizeof(uint8_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_BOND_INFO, sizeof(rtk_bt_le_bond_info_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_CONN_INFO,sizeof(rtk_bt_le_conn_info_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_DEV_STATE,sizeof(rtk_bt_le_gap_dev_state_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN, sizeof(rtk_bt_le_get_active_conn_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR,sizeof(uint16_t)}, 
	{RTK_BT_LE_GAP_ACT_GET_MTU_SIZE, sizeof(uint16_t)},
	{RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV, sizeof(uint8_t)},
	{RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE, sizeof(uint8_t)},
	{RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM, sizeof(uint16_t)},	
	{RTK_BT_LE_GAP_ACT_MAX, 0}
};

uint32_t rtk_le_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_gap_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_le_gap_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_le_gap_act_mem_op_tab[i].act == RTK_BT_LE_GAP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_le_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_LE_GAP_ACT_GET_VERSION ||
		act == RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR || 
		act == RTK_BT_LE_GAP_ACT_GET_BOND_NUM ||
		act == RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV) {
		memcpy((void *)dst, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_READ_RSSI) {
		rtk_bt_le_read_rssi_param_t *read_rssi = (rtk_bt_le_read_rssi_param_t *)dst;
		memcpy((void *)read_rssi->p_rssi, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_CONN_INFO) {
		rtk_bt_le_get_conn_info_param_t *pget_info = (rtk_bt_le_get_conn_info_param_t *)dst;
		memcpy((void *)pget_info->p_conn_info, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_BOND_INFO) {
		rtk_bt_le_get_bond_info_param_t *pget_bondinfo = (rtk_bt_le_get_bond_info_param_t *)dst;
		memcpy((void *)pget_bondinfo->p_size, (void *)src, sizeof(uint8_t));
		memcpy((void *)pget_bondinfo->p_bond_info, (void *)((uint8_t *)src + 4), sizeof(rtk_bt_le_bond_info_t)*(*pget_bondinfo->p_size));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_DEV_STATE) {
		rtk_bt_le_gap_dev_state_t *pget_dev_state = (rtk_bt_le_gap_dev_state_t *)dst;
		memcpy((void *)pget_dev_state, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN) {
		rtk_bt_le_get_active_conn_t *pactive_conn = (rtk_bt_le_get_active_conn_t *)dst;
		memcpy((void *)pactive_conn, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR) {
		rtk_bt_le_get_conn_handle_by_addr_param_t *pconn_handle_param = (rtk_bt_le_get_conn_handle_by_addr_param_t *)dst;
		memcpy((void *)pconn_handle_param->p_conn_handle, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_MTU_SIZE) {
		rtk_bt_le_get_mtu_size_param_t *pget_mtu_size_param = (rtk_bt_le_get_mtu_size_param_t *)dst;
		memcpy((void *)pget_mtu_size_param->p_mtu_size, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM) {
		rtk_bt_le_get_tx_pending_num_param_t *tx_pending_num_param = (rtk_bt_le_get_tx_pending_num_param_t *)dst;
		memcpy((void *)tx_pending_num_param->p_tx_pending_num, (void *)src, rtk_le_get_act_ret_param_size(act));
	} 
#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	else if (act == RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE) {
		rtk_bt_le_pa_sync_create_t *pa_sync_create = (rtk_bt_le_pa_sync_create_t *)dst;
		memcpy((void *)pa_sync_create->p_sync_id, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM) {
		rtk_bt_le_pa_sync_get_param_t *sync_get_param = (rtk_bt_le_pa_sync_get_param_t *)dst;
		uint8_t size = 0;		
		if(sync_get_param->type == RTK_BT_LE_PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE 
					|| sync_get_param->type == RTK_BT_LE_PA_SYNC_PARAM_DEV_STATE) {
			size = sizeof(uint8_t); 
		} else if(sync_get_param->type == RTK_BT_LE_PA_SYNC_PARAM_SYNCHRONIZED_PARAM) {
			size = sizeof(rtk_bt_le_pa_sync_sync_param_t); 
		} else {
			printf("RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM type is error\r\n");
		}
		memcpy((void *)sync_get_param->p_value, (void *)src, size);
	}
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */
#if RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	else if (act == RTK_BT_LE_GAP_READ_LOCAL_TX_POWER) {
		rtk_bt_le_txpower_read_local_t *param = (rtk_bt_le_txpower_read_local_t *)dst;
		*param->cur_txpower = *((int8_t *)src);
		*param->max_txpower = *((int8_t *)src + 1);
	}
#endif
	return RTK_BT_OK;
}

static int le_ipc_scan_info_filter_push(uint8_t *buf, rtk_bt_le_scan_info_filter_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_le_scan_info_filter_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_filter, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_scan_info_filter_param_t *)buf)->p_filter = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_scan_info_filter_param_t size is over deep copy buf\r\n");
	return -1;
}

#if RTK_BLE_5_0_AE_ADV_SUPPORT
static int le_ipc_set_ext_adv_data_push(uint8_t *buf, rtk_bt_le_ext_adv_data_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	/* memory space for rtk_bt_le_ext_adv_data_t */
	size = sizeof(rtk_bt_le_ext_adv_data_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->pdata, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_ext_adv_data_t *)buf)->pdata = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_ext_adv_data_t size is over deep copy buf\r\n");
	return -1;
}
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */

#if RTK_BLE_5_0_PA_ADV_SUPPORT
static int le_ipc_start_pa_push(uint8_t *buf, rtk_bt_le_pa_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_pa_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->pa_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->pa_data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_pa_param_t *)buf)->pa_data = (uint8_t *)(struct_end - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_pa_param_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_update_pa_push(uint8_t *buf, rtk_bt_le_pa_update_t *param, 
								 uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_pa_update_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->pa_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->pa_data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_pa_update_t *)buf)->pa_data = (uint8_t *)(struct_end - buf);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_pa_update_t size is over deep copy buf\r\n");
	return -1;
}
#endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */

static int le_ipc_vendor_cmd_req_push(uint8_t *buf, rtk_bt_le_gap_vendor_cmd_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_gap_vendor_cmd_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->cmd_param, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_gap_vendor_cmd_param_t *)buf)->cmd_param = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_gap_vendor_cmd_param_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_le_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER:
		ret = le_ipc_scan_info_filter_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_scan_info_filter_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_GAP_ACT_VENDOR_CMD_REQ:
		ret = le_ipc_vendor_cmd_req_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_gap_vendor_cmd_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size);
		break;
#if RTK_BLE_5_0_AE_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA:
	case RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA:
		ret = le_ipc_set_ext_adv_data_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_ext_adv_data_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size);
		break;
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */
#if RTK_BLE_5_0_PA_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_START_PA:
		ret = le_ipc_start_pa_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_pa_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_GAP_ACT_UPDATE_PA:
		ret = le_ipc_update_pa_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_pa_update_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size);
		break;
#endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */
	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}

	if (ret) {
		osif_mem_free(host_msg);
		host_msg = NULL;
	}

	return (void *)host_msg;
}

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
static void le_ipc_ext_scan_res_evt_pop(rtk_bt_le_ext_scan_res_ind_t *ext_scan_res)
{
	ext_scan_res->data = (uint8_t *)((uint8_t *)ext_scan_res + (uint32_t)ext_scan_res->data);
}
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if RTK_BLE_5_0_PA_SYNC_SUPPORT
static void le_ipc_pa_adv_report_evt_pop(rtk_bt_le_pa_adv_report_ind_t *pa_adv_report)
{
	pa_adv_report->p_data = (uint8_t *)((uint8_t *)pa_adv_report + (uint32_t)pa_adv_report->p_data);
}
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */

void bt_le_ipc_pop_event_param(uint8_t act, void *param)
{
#if !defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) || !(RTK_BLE_5_0_AE_SCAN_SUPPORT)
	(void)param;
#endif
	switch (act) {
#if RTK_BLE_5_0_AE_SCAN_SUPPORT
	case RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND:
		le_ipc_ext_scan_res_evt_pop((rtk_bt_le_ext_scan_res_ind_t *)param);
		break;
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND:
		le_ipc_pa_adv_report_evt_pop((rtk_bt_le_pa_adv_report_ind_t *)param);
		break;
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */

	default:
		break;
	}
}

/* ===============================================rtk_bt_gatts ipc action======================================= */
static struct act_mem_option rtk_bt_gatts_act_mem_op_tab[] = {
	{RTK_BT_GATTS_ACT_MAX, 0},
};

uint32_t rtk_gatts_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gatts_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_gatts_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_gatts_act_mem_op_tab[i].act == RTK_BT_GATTS_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_gatts_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	/* TODO */
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

static void gatts_ipc_write_evt_pop(rtk_bt_gatts_write_ind_t *write_evt)
{
	write_evt->value = (void *)((uint8_t *)write_evt + (uint32_t)(write_evt->value));
}

static uint32_t get_struct_bt_uuid_size(struct bt_uuid *uuid)
{
	uint32_t size = 0;
	if (BT_UUID_TYPE_16 == uuid->type) {
		size = sizeof(struct bt_uuid_16);
	} else if (BT_UUID_TYPE_128 == uuid->type) {
		size = sizeof(struct bt_uuid_128);
	}
	return size;
}

static uint32_t get_struct_attr_data_size(void *param)
{
	rtk_bt_gatt_attr_t *attr = (rtk_bt_gatt_attr_t *)param;
	uint32_t size = 0;
	uint16_t uuid = ((struct bt_uuid_16 *)attr->uuid)->val;
	switch (uuid) {
	case BT_UUID_GATT_PRIMARY_VAL:
	case BT_UUID_GATT_SECONDARY_VAL:
		size = get_struct_bt_uuid_size((struct bt_uuid *)attr->user_data);
		break;
	case BT_UUID_GATT_INCLUDE_VAL:
		size = sizeof(struct rtk_bt_gatt_service);
		break;
	case BT_UUID_GATT_CHRC_VAL:
		size = sizeof(struct rtk_bt_gatt_chrc);
		break;
	case BT_UUID_GATT_CEP_VAL:
		size = sizeof(struct rtk_bt_gatt_cep);
		break;
	case BT_UUID_GATT_CUD_VAL:
		size = strlen(attr->user_data) + 1;
		break;
	case BT_UUID_GATT_CCC_VAL:
		size = 0;
		break;
	case BT_UUID_GATT_SCC_VAL:
		size = sizeof(struct rtk_bt_gatt_scc);
		break;
	case BT_UUID_GATT_CPF_VAL:
		size = sizeof(struct rtk_bt_gatt_cpf);
		break;
	case BT_UUID_GATT_CAF_VAL:
		size = sizeof(struct rtk_bt_gatt_caf);
		break;
	default:
		size = attr->len;
		break;
	}
	
	return size;
}

/* push the multi-level pointer param to a blocked memory buf */
static int gatts_ipc_reg_srv_param_push(uint8_t *buf, struct rtk_bt_gatt_service *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint16_t uuid_val;
	struct rtk_bt_gatt_service *buf_ptr = (struct rtk_bt_gatt_service *)buf;
	struct rtk_bt_gatt_chrc *charac = NULL;
	struct rtk_bt_gatt_caf *caf = NULL;
	uint32_t size = 0;
	uint32_t attr_count = param->attr_count;
	uint32_t i = 0;

	/* memory space for struct rtk_bt_gatt_service */
	size = sizeof(struct rtk_bt_gatt_service);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	buf_ptr->attrs = (rtk_bt_gatt_attr_t *)end;

	/* memory space for struct rtk_bt_gatt_attr_t of attr_count num */
	size = sizeof(rtk_bt_gatt_attr_t) * attr_count;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->attrs, size);
	end += ALIGN_UP(size, 4);
	
	for (i = 0; i < attr_count; i++) {
		buf_ptr->attrs[i].uuid = (const struct bt_uuid *)end;
		/* memory space for struct bt_uuid */
		size = get_struct_bt_uuid_size((struct bt_uuid *)param->attrs[i].uuid);

		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->attrs[i].uuid, size);
		end += ALIGN_UP(size, 4);

		/* memory space for void *user_data */
		if (param->attrs[i].user_data) {
			buf_ptr->attrs[i].user_data = end;
			size =  get_struct_attr_data_size(&param->attrs[i]);
			
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->attrs[i].user_data, size);
			end += ALIGN_UP(size, 4);
		} else {
			buf_ptr->attrs[i].user_data = NULL;
		}
	}

	void *data_in_user_data = NULL;
	for (i = 0; i < attr_count; i++) {
		uuid_val = ((struct bt_uuid_16 *)param->attrs[i].uuid)->val;
		/* memory space for second level pointer in user_data */
		if (uuid_val == BT_UUID_GATT_CHRC_VAL) {
			charac = (struct rtk_bt_gatt_chrc *)param->attrs[i].user_data;
			((struct rtk_bt_gatt_chrc *)buf_ptr->attrs[i].user_data)->uuid = (const struct bt_uuid *)end;
			size = get_struct_bt_uuid_size((struct bt_uuid *)charac->uuid);
			data_in_user_data = (void *)charac->uuid;
		} else if (uuid_val == BT_UUID_GATT_CAF_VAL) {
			caf = (struct rtk_bt_gatt_caf *)param->attrs[i].user_data;
			((struct rtk_bt_gatt_caf *)buf_ptr->attrs[i].user_data)->handles = (uint16_t *)end;
			size = (caf->count) * sizeof(*(caf->handles));
			data_in_user_data = (void *)caf->handles;
		} else {
			continue;
		}

		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, data_in_user_data, size);
		end += ALIGN_UP(size, 4);
	}
	/* !!! The value of pointer should be assigned with memory address offset */ 
	/* cause once the memory is copied by another core, the pointer memory addres is meanless, so memory address offset is useful */
	for (i = 0; i < attr_count; i++) {
		uuid_val = ((struct bt_uuid_16 *)param->attrs[i].uuid)->val;
		if (param->attrs[i].user_data) {
			if (uuid_val == BT_UUID_GATT_CHRC_VAL) {
				charac = (struct rtk_bt_gatt_chrc *)buf_ptr->attrs[i].user_data;
				charac->uuid = (const struct bt_uuid *)(((uint8_t *)charac->uuid) - buf);
			} else if (uuid_val == BT_UUID_GATT_CAF_VAL) {
				caf = (struct rtk_bt_gatt_caf *)buf_ptr->attrs[i].user_data;
				caf->handles = (uint16_t *)(((uint8_t *)caf->handles) - buf);
			}
			buf_ptr->attrs[i].user_data = (void *)((uint8_t *)buf_ptr->attrs[i].user_data - buf);
		}
		buf_ptr->attrs[i].uuid = (const struct bt_uuid *)((uint8_t *)buf_ptr->attrs[i].uuid - buf);
	}
	buf_ptr->attrs = (rtk_bt_gatt_attr_t *)((uint8_t *)buf_ptr->attrs - buf);
	
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatt_service size is over deep copy buf\r\n");
	return -1;
}

static int gatts_ipc_indicate_param_push(uint8_t *buf, rtk_bt_gatts_ntf_and_ind_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_gatts_ntf_and_ind_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gatts_ntf_and_ind_param_t *)buf)->data = (const void *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatts_ntf_and_ind_param_t size is over deep copy buf\r\n");
	return -1;
}

static int gatts_ipc_notify_param_push(uint8_t *buf, rtk_bt_gatts_ntf_and_ind_param_t *param, 
							uint32_t size_limit, uint32_t *actual_size)
{
	/* the structure of notify param is same as indicate param */
	return gatts_ipc_indicate_param_push(buf, param, size_limit, actual_size);
}

static int gatts_ipc_read_resp_param_push(uint8_t *buf, rtk_bt_gatts_read_resp_param_t *param, 
							uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gatts_read_resp_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gatts_read_resp_param_t *)buf)->data = (const void *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatts_read_resp_param_t size is over deep copy buf\r\n");
	return -1;
}

void bt_gatts_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTS_EVT_WRITE_IND:
		gatts_ipc_write_evt_pop(param);
		break;
	default:
		break;
	}
}

void *bt_gatts_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_GATTS_ACT_REGISTER_SERVICE:
		if (gatts_ipc_reg_srv_param_push((uint8_t *)host_msg->param_buf, (struct rtk_bt_gatt_service *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	
	case RTK_BT_GATTS_ACT_NOTIFY:
		if (gatts_ipc_notify_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gatts_ntf_and_ind_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)){
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_GATTS_ACT_INDICATE:
		if (gatts_ipc_indicate_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gatts_ntf_and_ind_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_GATTS_ACT_READ_RSP:
		if (gatts_ipc_read_resp_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gatts_read_resp_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

/* ===============================================rtk_bt_gattc ipc action======================================= */
static struct act_mem_option rtk_bt_gattc_act_mem_op_tab[] = {
	{RTK_BT_GATTC_ACT_MAX, 0},
};

uint32_t rtk_gattc_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gattc_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_gattc_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_gattc_act_mem_op_tab[i].act == RTK_BT_GATTC_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_gattc_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
#if RTK_BLE_MGR_LIB
	if (act == RTK_BT_GATTC_ACT_FIND) {
		rtk_bt_gattc_find_param_t *find_param = (rtk_bt_gattc_find_param_t *)dst;
		uint8_t *ret = (uint8_t *)src;
		if (find_param->type == RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE) {
			memcpy((void *)find_param->find_char.p_handle, (void *)ret, sizeof(uint16_t));
		} else if (find_param->type == RTK_BT_GATT_FIND_CHARACTERISTIC_DESC_RANGE) {
			memcpy((void *)find_param->find_char_range.p_handle_start, (void *)ret, sizeof(uint16_t));
			ret += sizeof(uint16_t);
			memcpy((void *)find_param->find_char_range.p_handle_end, (void *)ret, sizeof(uint16_t));
		} else if (find_param->type == RTK_BT_GATT_FIND_CHARACTERISTIC_CCCD_HANDLE) {
			memcpy((void *)find_param->find_char_cccd.p_handle, (void *)ret, sizeof(uint16_t));
			ret += sizeof(uint16_t);
			memcpy((void *)find_param->find_char_cccd.p_notify, (void *)ret, sizeof(bool));
			ret += sizeof(bool);
			memcpy((void *)find_param->find_char_cccd.p_indicate, (void *)ret, sizeof(bool));
		} else if (find_param->type == RTK_BT_GATT_FIND_INCLUDE_SERVICE) {
			memcpy((void *)find_param->find_include_srv.p_instance_num, (void *)ret, sizeof(uint8_t));
			ret += sizeof(uint8_t);
			memcpy((void *)find_param->find_include_srv.instance_id, (void *)ret, sizeof(find_param->find_include_srv.instance_id));
		}
	}
#else
	(void)act;
	(void)dst;
	(void)src;
#endif
	return RTK_BT_OK;
}

static void gattc_ipc_read_evt_pop(rtk_bt_gattc_read_ind_t *read_evt)
{
#if RTK_BLE_MGR_LIB
	read_evt->value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->value));
#else
	switch (read_evt->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
		read_evt->by_handle.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->by_handle.value));
		break;

	case RTK_BT_GATT_CHAR_READ_BY_UUID:
		read_evt->by_uuid_per.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->by_uuid_per.value));
		break;
	
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		read_evt->multiple_per.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->multiple_per.value));
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		read_evt->multiple_variable_per.value = (uint8_t *)((uint8_t *)read_evt + (uint32_t)(read_evt->multiple_variable_per.value));
		break;

	default:
		printf("rtk_bt_gattc_read_ind_t pop type unknown\r\n");
		break;
	}
#endif
}

static void gattc_ipc_notify_indicate_evt_pop(rtk_bt_gattc_cccd_value_ind_t *evt)
{
	evt->value = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->value));
}

/* TODO */
void bt_gattc_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		gattc_ipc_read_evt_pop(param);
		break;

	case RTK_BT_GATTC_EVT_NOTIFY_IND:
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		gattc_ipc_notify_indicate_evt_pop(param);
		break;

	default:
		break;
	}
}

#if !RTK_BLE_MGR_LIB
/* push the multi-level pointer param to a blocked memory buf */
static int gattc_ipc_read_param_push(uint8_t *buf, rtk_bt_gattc_read_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_gattc_read_param_t *p_read_param = (rtk_bt_gattc_read_param_t *)buf;
	uint32_t size = 0;
	uint32_t handle_cout=0;

	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		handle_cout = param->multiple.handle_count;
		/* memory space for struct rtk_bt_gattc_read_param_t */
		size = sizeof(rtk_bt_gattc_read_param_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		/* parsing handles */
		p_read_param->multiple.handles = (uint16_t *)end;
		if (end + handle_cout * 2 > buf + size_limit)
			goto exit;
		memcpy((void *)p_read_param->multiple.handles, (void *)param->multiple.handles, handle_cout * 2);
		p_read_param->multiple.handles = (uint16_t *)(end - buf);
		*actual_size = (uint32_t)((end + handle_cout * 2) - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		handle_cout = param->multiple_variable.handle_count;
		/* memory space for struct rtk_bt_gattc_read_param_t */
		size = sizeof(rtk_bt_gattc_read_param_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		/* parsing handles */
		p_read_param->multiple_variable.handles = (uint16_t *)end;
		if (end + handle_cout * 2 > buf + size_limit)
			goto exit;
		memcpy((void *)p_read_param->multiple_variable.handles, (void *)param->multiple_variable.handles, handle_cout * 2);
		p_read_param->multiple_variable.handles = (uint16_t *)(end - buf);
		*actual_size = (uint32_t)((end + handle_cout * 2) - buf);
		break;

	default:
		memcpy((void *)buf, (void *)param, sizeof(rtk_bt_gattc_read_param_t));
		*actual_size = sizeof(rtk_bt_gattc_read_param_t);
		break;
	}

	return 0;

exit:
	printf("rtk_bt_gattc_read_param_t size is over deep copy buf\r\n");
	return -1;
}
#endif

/* push the multi-level pointer param to a blocked memory buf */
static int gattc_ipc_write_param_push(uint8_t *buf, rtk_bt_gattc_write_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_gattc_write_param_t *p_write_param = (rtk_bt_gattc_write_param_t *)buf;
	uint32_t size = 0;

	/* memory space for struct rtk_bt_gattc_write_param_t */
	size = sizeof(rtk_bt_gattc_write_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_write_param->data = (void *)end;
	if (end + param->length > buf + size_limit)
		goto exit;
	memcpy((void *)p_write_param->data, (void *)param->data, param->length);
	p_write_param->data = (void *)(end - buf);
	*actual_size = (uint32_t)((end + param->length) - buf);

	return 0;

exit:
	printf("rtk_bt_gattc_write_param_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_gattc_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
#if !RTK_BLE_MGR_LIB
	case RTK_BT_GATTC_ACT_READ:
		if (gattc_ipc_read_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gattc_read_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif

	case RTK_BT_GATTC_ACT_WRITE:
		if (gattc_ipc_write_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_gattc_write_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

/* ===============================================rtk_bt_br_gap ipc action======================================= */
static struct act_mem_option rtk_bt_br_gap_act_mem_op_tab[] = {
	{RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR, sizeof(rtk_bt_br_bd_addr_t)}, 
	{RTK_BT_BR_GAP_ACT_MAX, 0}
};

uint32_t rtk_br_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_br_gap_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_br_gap_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_br_gap_act_mem_op_tab[i].act == RTK_BT_BR_GAP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_br_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR) {
		memcpy((void *)dst, (void *)src, rtk_br_get_act_ret_param_size(act));
	}

	return RTK_BT_OK;
}

static void br_gap_ipc_inquiry_scan_evt_pop(rtk_bt_br_inquiry_result_t *inquiry_result_evt)
{
	inquiry_result_evt->p_eir = (uint8_t *)((uint8_t *)inquiry_result_evt + (uint32_t)(inquiry_result_evt->p_eir));
}

void *bt_br_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_br_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	
	case RTK_BT_BR_GAP_INQUIRY_RESULT:
		br_gap_ipc_inquiry_scan_evt_pop((rtk_bt_br_inquiry_result_t *)param);
		break;

	default:
		break;
	}
}

/* ===============================================rtk_bt_a2dp ipc action======================================= */
static struct act_mem_option rtk_bt_a2dp_act_mem_op_tab[] = {
	{RTK_BT_A2DP_ACT_MAX, 0}
};

uint32_t rtk_a2dp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_a2dp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_a2dp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_a2dp_act_mem_op_tab[i].act == RTK_BT_A2DP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_a2dp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

static int a2dp_ipc_stream_data_send_param_push(uint8_t *buf, rtk_bt_a2dp_stream_data_send_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_a2dp_stream_data_send_t *p_data_send_param = (rtk_bt_a2dp_stream_data_send_t *)buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_a2dp_stream_data_send_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_data_send_param->frame_buf = (void *)end;
	if (end + param->len > buf + size_limit)
		goto exit;
	memcpy((void *)p_data_send_param->frame_buf, (void *)param->frame_buf, param->len);
	p_data_send_param->frame_buf = (void *)(end - buf);
	*actual_size = (uint32_t)((end + param->len) - buf);

	return 0;

exit:
	printf("rtk_bt_a2dp_stream_data_send_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_a2dp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	
	case RTK_BT_A2DP_ACT_SEND_DATA:
		if (a2dp_ipc_stream_data_send_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_a2dp_stream_data_send_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_a2dp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_avrcp ipc action======================================= */
static struct act_mem_option rtk_bt_avrcp_act_mem_op_tab[] = {
	{RTK_BT_AVRCP_ACT_MAX, 0}
};

uint32_t rtk_avrcp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_avrcp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_avrcp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_avrcp_act_mem_op_tab[i].act == RTK_BT_AVRCP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}

uint16_t rtk_avrcp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

void *bt_avrcp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_avrcp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_spp ipc action======================================= */
static struct act_mem_option rtk_bt_spp_act_mem_op_tab[] = {
	{RTK_BT_SPP_ACT_MAX, 0}
};

uint32_t rtk_spp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_spp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_spp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_spp_act_mem_op_tab[i].act == RTK_BT_SPP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}

uint16_t rtk_spp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

static int spp_ipc_data_send_param_push(uint8_t *buf, rtk_bt_spp_send_data_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_spp_send_data_t *p_data_send_param = (rtk_bt_spp_send_data_t *)buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_spp_send_data_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_data_send_param->data = (void *)end;
	if (end + param->len > buf + size_limit)
		goto exit;
	memcpy((void *)p_data_send_param->data, (void *)param->data, param->len);
	p_data_send_param->data = (void *)(end - buf);
	*actual_size = (uint32_t)((end + param->len) - buf);

	return 0;

exit:
	printf("rtk_bt_spp_send_data_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_spp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	
	case RTK_BT_SPP_ACT_SEND_DATA:
		if (spp_ipc_data_send_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_spp_send_data_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_spp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_sdp ipc action======================================= */
static struct act_mem_option rtk_bt_sdp_act_mem_op_tab[] = {
	{RTK_BT_SDP_ACT_MAX, 0}
};

uint32_t rtk_sdp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_sdp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_sdp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_sdp_act_mem_op_tab[i].act == RTK_BT_SDP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}

uint16_t rtk_sdp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

void *bt_sdp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_sdp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_hfp ipc action======================================= */
static struct act_mem_option rtk_bt_hfp_act_mem_op_tab[] = {
	{RTK_BT_HFP_ACT_MAX, 0}
};

uint32_t rtk_hfp_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_hfp_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_hfp_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_hfp_act_mem_op_tab[i].act == RTK_BT_HFP_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}
	
	return max_size;
}

uint16_t rtk_hfp_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;

	return RTK_BT_OK;
}

static int hfp_ipc_sco_data_send_param_push(uint8_t *buf, rtk_bt_hfp_sco_data_send_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	rtk_bt_hfp_sco_data_send_t *p_data_send_param = (rtk_bt_hfp_sco_data_send_t *)buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_hfp_sco_data_send_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_data_send_param->buf = (void *)end;
	if (end + param->len > buf + size_limit)
		goto exit;
	memcpy((void *)p_data_send_param->buf, (void *)param->buf, param->len);
	p_data_send_param->buf = (void *)(end - buf);
	*actual_size = (uint32_t)((end + param->len) - buf);

	return 0;

exit:
	printf("rtk_bt_hfp_sco_data_send_t size is over deep copy buf\r\n");

	return -1;
}

void *bt_hfp_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	
	case RTK_BT_HFP_ACT_SEND_SCO_DATA:
		if (hfp_ipc_sco_data_send_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_hfp_sco_data_send_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

void bt_hfp_ipc_pop_event_param(uint8_t act, void *param)
{
	/* TODO */
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_le_iso ipc action======================================= */
#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)
static struct act_mem_option rtk_bt_le_iso_act_mem_op_tab[] = {	
	{RTK_BT_LE_ISO_ACT_CIG_READ_ISO_TX_SYNC,sizeof(rtk_bt_le_iso_cig_read_iso_tx_sync_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_READ_LINK_QUALITY,sizeof(rtk_bt_le_iso_cig_read_link_quality_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_CONN_HANDLE,sizeof(uint16_t)},
	{RTK_BT_LE_ISO_ACT_CIG_GET_CONN_ID,sizeof(uint8_t)},	
	{RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO,sizeof(rtk_bt_le_iso_cig_cis_info_t)},	
	{RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO,sizeof(rtk_bt_le_iso_cis_channel_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE,sizeof(uint16_t)},			
	{RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE,sizeof(uint8_t)},	
	{RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_READ_TX_SYNC,sizeof(rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t)},	
	{RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC,sizeof(uint8_t)},	
	{RTK_BT_LE_ISO_ACT_BIG_RECEIVER_READ_LINK_QUALITY,sizeof(rtk_bt_le_iso_big_receiver_read_link_quality_info_t)},		
	{RTK_BT_LE_ISO_ACT_MAX, 0}
};

uint32_t rtk_le_iso_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_iso_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_le_iso_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_le_iso_act_mem_op_tab[i].act == RTK_BT_LE_ISO_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}

uint16_t rtk_le_iso_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_LE_ISO_ACT_CIG_READ_ISO_TX_SYNC) {
		rtk_bt_le_iso_cig_read_iso_tx_sync_t *param = (rtk_bt_le_iso_cig_read_iso_tx_sync_t *)dst;
		memcpy((void *)param->p_tx_sync_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_READ_LINK_QUALITY) {
		rtk_bt_le_iso_cig_read_link_quality_t *param = (rtk_bt_le_iso_cig_read_link_quality_t *)dst;
		memcpy((void *)param->p_link_quality_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_CONN_HANDLE) {
		rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle_param_t *param = 
										(rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle_param_t *)dst;
		memcpy((void *)param->p_cis_conn_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_GET_CONN_ID) {
		rtk_bt_le_iso_cig_get_conn_id_param_t *param = (rtk_bt_le_iso_cig_get_conn_id_param_t *)dst;
		memcpy((void *)param->p_conn_id, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO) {
		rtk_bt_le_iso_cig_get_cis_info_param_t *param = (rtk_bt_le_iso_cig_get_cis_info_param_t *)dst;
		memcpy((void *)param->p_cis_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO) {
		rtk_bt_le_iso_cig_get_isoch_info_param_t *param = (rtk_bt_le_iso_cig_get_isoch_info_param_t *)dst;
		memcpy((void *)param->p_isoch_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE) {
		rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t *param = (rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t *)dst;
		memcpy((void *)param->p_cis_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE) {
		rtk_bt_le_iso_big_broadcaster_create_param_t *param = (rtk_bt_le_iso_big_broadcaster_create_param_t *)dst;
		memcpy((void *)param->big_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_READ_TX_SYNC) {
		rtk_bt_le_iso_big_broadcaster_read_tx_sync_t *param = (rtk_bt_le_iso_big_broadcaster_read_tx_sync_t *)dst;
		memcpy((void *)param->p_tx_sync_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC) {
		rtk_bt_le_iso_big_receiver_create_sync_t *param = (rtk_bt_le_iso_big_receiver_create_sync_t *)dst;
		memcpy((void *)param->p_big_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	}

	return RTK_BT_OK;
}

static int le_ipc_iso_setup_path_push(uint8_t *buf, rtk_bt_le_iso_setup_path_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_setup_path_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->codec_config_len;

	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_codec_config, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_setup_path_param_t *)buf)->p_codec_config = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);

	return 0;

exit:
	printf("rtk_bt_le_iso_setup_path_param_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_iso_big_broadcaster_create_push(uint8_t *buf, rtk_bt_le_iso_big_broadcaster_create_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_big_broadcaster_create_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = sizeof(rtk_bt_le_iso_big_broadcaster_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_big_param, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_big_broadcaster_create_param_t *)buf)->p_big_param = (rtk_bt_le_iso_big_broadcaster_param_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);

	return 0;

exit:
	printf("rtk_bt_le_iso_big_broadcaster_create_param_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_iso_big_receiver_create_sync_push(uint8_t *buf, rtk_bt_le_iso_big_receiver_create_sync_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_big_receiver_create_sync_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = sizeof(rtk_bt_le_iso_big_receiver_create_sync_param_t);

	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_sync_param, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_big_receiver_create_sync_t *)buf)->p_sync_param = (rtk_bt_le_iso_big_receiver_create_sync_param_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);

	return 0;

exit:
	printf("rtk_bt_le_iso_big_receiver_create_sync_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_iso_data_send_push(uint8_t *buf, rtk_bt_le_iso_data_send_info_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_data_send_info_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->data_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_data_send_info_t *)buf)->p_data = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_iso_data_send_info_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_le_iso_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_LE_ISO_ACT_CIG_SETUP_PATH:
	case RTK_BT_LE_ISO_ACT_BIG_SETUP_PATH: 
		if (le_ipc_iso_setup_path_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_iso_setup_path_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE:
		if (le_ipc_iso_big_broadcaster_create_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_iso_big_broadcaster_create_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC:
		if (le_ipc_iso_big_receiver_create_sync_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_iso_big_receiver_create_sync_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_LE_ISO_ACT_ISO_DATA_SEND:
		if (le_ipc_iso_data_send_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_iso_data_send_info_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;		

	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

static void iso_ipc_data_ind_evt_pop(rtk_bt_le_iso_direct_iso_data_ind_t *evt)
{
	evt->p_buf = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_buf));
}

void bt_le_iso_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND:
		iso_ipc_data_ind_evt_pop((rtk_bt_le_iso_direct_iso_data_ind_t *)param);
		break;

	default:
		break;
	}
}
#else 
uint32_t rtk_le_iso_get_act_ret_param_size(uint16_t act)
{
	(void)act;
	return 0;	
}
uint16_t rtk_le_iso_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;	
	(void)src;	
	return RTK_BT_OK;
}
void *bt_le_iso_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	(void)act;
	(void)data;	
	(void)buf_len;
	(void)pactual_size;
	return NULL;
}
void bt_le_iso_ipc_pop_event_param(uint8_t act, void *param)
{
	(void)act;
	(void)param;		
}
#endif
/* ===============================================rtk_bt_le_audio ipc action======================================= */
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
static struct act_mem_option rtk_bt_le_audio_act_mem_op_tab[] = {	
	{RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI,RTK_BT_LE_CSI_RSI_LEN},
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
	{RTK_BT_LE_AUDIO_ACT_SYNC_CREATE,sizeof(uint8_t)},
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIG_INFO,sizeof(rtk_bt_le_audio_biginfo_t)},	
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG,sizeof(rtk_bt_le_audio_cfg_codec_t)},
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO,sizeof(rtk_bt_le_audio_sync_bis_info_t)},
#endif 
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
	{RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE,sizeof(rtk_bt_le_audio_broadcast_source_create_ipc_param_t)},
	{RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO,sizeof(rtk_bt_le_audio_broadcast_source_info_t)},	
	{RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_CONN_HANDLE,sizeof(uint16_t)},	
#endif
	{RTK_BT_LE_AUDIO_ACT_VCS_PARAM_GET,sizeof(rtk_bt_le_audio_vcs_param_t)},
	{RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE,sizeof(rtk_bt_le_audio_vcs_volume_state_t)},	
	{RTK_BT_LE_AUDIO_ACT_MAX, 0}
};

uint32_t rtk_le_audio_get_act_ret_param_size(uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_audio_act_mem_op_tab[i].act == act) {
			max_size = rtk_bt_le_audio_act_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_le_audio_act_mem_op_tab[i].act == RTK_BT_LE_AUDIO_ACT_MAX) {
			//printf("%s: cannot find matched act %d \r\n", __func__, act);
			break;
		}
		i++;
	}

	return max_size;
}
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
static void le_audio_broadcast_source_create_ret_param_pop(void *dst, void *src)
{
	uint16_t i = 0 , j = 0 , k = 0;
	rtk_bt_le_audio_broadcast_source_create_param_t *dst_param = (rtk_bt_le_audio_broadcast_source_create_param_t *)dst;
	rtk_bt_le_audio_broadcast_source_create_ipc_param_t *src_param = (rtk_bt_le_audio_broadcast_source_create_ipc_param_t *)src;
	for(i = 0 ; i < dst_param->num_groups; i++) {
		*dst_param->group[i].group_idx = src_param->group[i].group_idx; 
		for(j = 0 ; j < dst_param->group[i].num_subgroups; j++) {
			*dst_param->group[i].subgroup[j].subgroup_idx = src_param->group[i].subgroup[j].subgroup_idx; 
			for(k = 0 ; k < dst_param->group[i].subgroup[j].num_bis; k++) {
				*dst_param->group[i].subgroup[j].bis[k].bis_idx = src_param->group[i].subgroup[j].bis[k].bis_idx; 
			}
		}
	}
}
#endif
uint16_t rtk_le_audio_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI) {
		rtk_bt_le_audio_csis_gen_rsi_info_t *param = (rtk_bt_le_audio_csis_gen_rsi_info_t *)dst;
		memcpy((void *)param->p_rsik, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} 
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
	else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_CREATE) {
		rtk_bt_le_audio_sync_create_param_t *param = (rtk_bt_le_audio_sync_create_param_t *)dst;
		memcpy((void *)param->sync_handle_id, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIG_INFO) {
		rtk_bt_le_audio_sync_get_big_info_param_t *param = (rtk_bt_le_audio_sync_get_big_info_param_t *)dst;
		memcpy((void *)param->p_big_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG) {
		rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t *param = (rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t *)dst;
		memcpy((void *)param->p_codec_cfg, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO) {
		rtk_bt_le_audio_sync_get_bis_info_param_t *param = (rtk_bt_le_audio_sync_get_bis_info_param_t *)dst;
		memcpy((void *)param->p_bis_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} 
#endif
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
	else if (act == RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE) {		
		le_audio_broadcast_source_create_ret_param_pop(dst, src);
	} else if (act == RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO) {		
		rtk_bt_le_audio_broadcast_source_info_get_param_t *param = (rtk_bt_le_audio_broadcast_source_info_get_param_t *)dst;
		memcpy((void *)param->p_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_CONN_HANDLE) {		
		rtk_bt_le_audio_broadcast_source_conn_handle_get_param_t *param = (rtk_bt_le_audio_broadcast_source_conn_handle_get_param_t *)dst;
		memcpy((void *)param->p_bis_conn_handle, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif
	else if (act == RTK_BT_LE_AUDIO_ACT_VCS_PARAM_GET) {		
		rtk_bt_le_audio_vcs_param_t *param = (rtk_bt_le_audio_vcs_param_t *)dst;
		memcpy((void *)param, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE) {		
		rtk_bt_le_audio_vcs_get_volume_state_param_t *param = (rtk_bt_le_audio_vcs_get_volume_state_param_t *)dst;
		memcpy((void *)param->p_vcs_volume_state, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
	return RTK_BT_OK;
}

static int le_ipc_audio_iso_data_send_push(uint8_t *buf, rtk_bt_le_audio_iso_data_send_info_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_iso_data_send_info_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->data_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_data, size);
	((rtk_bt_le_audio_iso_data_send_info_t *)buf)->p_data = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_iso_data_send_info_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_audio_mcs_media_player_info_set_push(uint8_t *buf, rtk_bt_le_audio_mcs_media_player_info_set_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_mcs_media_player_info_t *p_media_player_info = &param->media_player_info;

	size = sizeof(rtk_bt_le_audio_mcs_media_player_info_set_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = p_media_player_info->player_name_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, p_media_player_info->p_player_name, size);
	p_media_player_info->p_player_name = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	size = p_media_player_info->icon_fmt_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, p_media_player_info->p_icon_fmt, size);
	p_media_player_info->p_icon_fmt = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	size = p_media_player_info->url_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, p_media_player_info->p_url, size);
	p_media_player_info->p_url = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_mcs_media_player_info_set_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_audio_mcs_media_track_info_set_push(uint8_t *buf, rtk_bt_le_audio_mcs_media_track_info_set_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_mcs_media_track_info_t *p_media_track_info = &param->media_track_info;

	size = sizeof(rtk_bt_le_audio_mcs_media_track_info_set_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = p_media_track_info->title_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, p_media_track_info->p_title, size);
	p_media_track_info->p_title = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_mcs_media_track_info_set_t size is over deep copy buf\r\n");
	return -1;
}

#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
static int le_ipc_audio_sync_setup_path_push(uint8_t *buf, rtk_bt_le_audio_sync_setup_data_path_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bis_setup_data_path_t *p_setup_param = &param->setup_param;
	rtk_bt_le_audio_sync_setup_data_path_param_t *p_buf= (rtk_bt_le_audio_sync_setup_data_path_param_t *)buf;

	size = sizeof(rtk_bt_le_audio_sync_setup_data_path_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = p_setup_param->codec_config_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, p_setup_param->p_codec_config, size);
	p_buf->setup_param.p_codec_config = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);
	
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_sync_setup_data_path_param_t size is over deep copy buf\r\n");
	return -1;
}
#endif
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
static int le_ipc_audio_broadcast_source_create_push(uint8_t *buf, rtk_bt_le_audio_broadcast_source_create_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	rtk_bt_le_audio_broadcast_source_create_param_t * create_buf = (rtk_bt_le_audio_broadcast_source_create_param_t *)buf;
	uint8_t i=0,j=0,k=0;
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_broadcast_source_create_param_t);
	if (end + size > buf + size_limit) {
		printf("%s size = %d,size_limit = %d\r\n",__func__,size,size_limit);
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	for(i=0;i<param->num_groups;i++) {
		for(j=0;j<param->group[i].num_subgroups;j++) {
			//copy codec_cfg_l2
			size = param->group[i].subgroup[j].codec_cfg_l2_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->group[i].subgroup[j].p_codec_cfg_l2, size);
			create_buf->group[i].subgroup[j].p_codec_cfg_l2 = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			
			//copy metadata
			size = param->group[i].subgroup[j].metadata_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->group[i].subgroup[j].p_metadata, size);
			create_buf->group[i].subgroup[j].p_metadata = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			
			for(k=0;k<param->group[i].subgroup[j].num_bis;k++) {
				//copy p_codec_cfg_l3 
				size = param->group[i].subgroup[j].bis[k].codec_cfg_l3_len;
				if (end + size > buf + size_limit)
					goto exit;
				memcpy(end, param->group[i].subgroup[j].bis[k].p_codec_cfg_l3, size);
				create_buf->group[i].subgroup[j].bis[k].p_codec_cfg_l3 = (uint8_t *)(end - buf);
				end += ALIGN_UP(size, 4);				
			}
		}
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_broadcast_source_create_param_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_audio_broadcast_source_setup_path_push(uint8_t *buf, rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bis_setup_data_path_t *p_setup_param = &param->setup_param;
	rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *p_buf = (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)buf;

	size = sizeof(rtk_bt_le_audio_broadcast_source_setup_data_path_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = p_setup_param->codec_config_len;
	if (end + size > buf + size_limit) 
		goto exit;

	memcpy(end, p_setup_param->p_codec_config, size);
	p_buf->setup_param.p_codec_config = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);	
	
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_bis_setup_data_path_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_audio_broadcast_source_reconfig_push(uint8_t *buf, rtk_bt_le_audio_broadcast_source_reconfig_param_t *param, 
									uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_broadcast_source_reconfig_param_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->metadata_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_metadata, size);
	((rtk_bt_le_audio_broadcast_source_reconfig_param_t *)buf)->p_metadata = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);
	
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_broadcast_source_reconfig_param_t size is over deep copy buf\r\n");
	return -1;
}
#endif
void *bt_le_audio_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
	case RTK_BT_LE_AUDIO_ACT_ISO_DATA_SEND:
		if (le_ipc_audio_iso_data_send_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_iso_data_send_info_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;	
	case RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_PLAYER_INFO_SET:
		if (le_ipc_audio_mcs_media_player_info_set_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_mcs_media_player_info_set_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;	
	case RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_TRACK_INFO_SET:
		if (le_ipc_audio_mcs_media_track_info_set_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_mcs_media_track_info_set_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
	case RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH:
		if (le_ipc_audio_sync_setup_path_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_sync_setup_data_path_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;	
#endif		
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE:
		if (le_ipc_audio_broadcast_source_create_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_broadcast_source_create_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;	

	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH:
		if (le_ipc_audio_broadcast_source_setup_path_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;		

	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG:
		if (le_ipc_audio_broadcast_source_reconfig_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_broadcast_source_reconfig_param_t *)data, 
									IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;	
#endif
	default:
		memcpy((void *)host_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)host_msg;
}

static void le_audio_ipc_iso_data_ind_evt_pop(rtk_bt_le_audio_direct_iso_data_ind_t *evt)
{
	evt->p_buf = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_buf));
}

static void le_audio_ipc_mcs_client_read_result_ind_evt_pop(rtk_bt_le_audio_mcs_client_read_result_ind_t *evt)
{
	switch (evt->uuid) {
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_NAME_CHAR_UUID:
			evt->data.media_player_name.player_name = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.media_player_name.player_name));
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_OBJID_CHAR_UUID:
			evt->data.media_player_icon_url.p_icon_url = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.media_player_icon_url.p_icon_url));
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_URL_CHAR_UUID:
			evt->data.track_title_url.title_name = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.track_title_url.title_name));
			break;
		default:
			break;
	}	
}
static void le_audio_ipc_mcs_client_notify_result_ind_evt_pop(rtk_bt_le_audio_mcs_client_notify_result_ind_t *evt)
{
	switch (evt->uuid) {
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_NAME_CHAR_UUID:
			evt->data.media_player_name.player_name = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.media_player_name.player_name));
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_OBJID_CHAR_UUID:
			evt->data.media_player_icon_url.p_icon_url = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.media_player_icon_url.p_icon_url));
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_URL_CHAR_UUID:
			evt->data.track_title_url.title_name = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.track_title_url.title_name));
			break;
		default:
			break;
	}	
}
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
static void le_audio_ipc_pa_adv_report_ind_evt_pop(rtk_bt_le_audio_pa_adv_report_ind_t *evt)
{
	evt->p_data = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_data));
}
#endif
void bt_le_audio_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND:
		le_audio_ipc_iso_data_ind_evt_pop((rtk_bt_le_audio_direct_iso_data_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_READ_RESULT_IND:
		le_audio_ipc_mcs_client_read_result_ind_evt_pop((rtk_bt_le_audio_mcs_client_read_result_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_NOTIFY_RESULT_IND:
		le_audio_ipc_mcs_client_notify_result_ind_evt_pop((rtk_bt_le_audio_mcs_client_notify_result_ind_t *)param);
		break;
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
	case RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND:
		le_audio_ipc_pa_adv_report_ind_evt_pop((rtk_bt_le_audio_pa_adv_report_ind_t*)param);
		break;
#endif
	default:
		break;
	}
}
#else
uint32_t rtk_le_audio_get_act_ret_param_size(uint16_t act)
{
	(void)act;
	return 0;	
}
uint16_t rtk_le_audio_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;	
	(void)src;	
	return RTK_BT_OK;
}
void *bt_le_audio_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	(void)act;
	(void)data;	
	(void)buf_len;
	(void)pactual_size;
	return NULL;
}
void bt_le_audio_ipc_pop_event_param(uint8_t act, void *param)
{
	(void)act;
	(void)param;		
}
#endif
#elif defined(CONFIG_BT_NP) && CONFIG_BT_NP
/* ===============================================rtk_bt_le_gap_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_gap_evt_mem_op_tab[] = {
#if RTK_BT_5_2_L2C_ECFC_SUPPORT
	{RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND, sizeof(rtk_bt_ecfc_conn_cmpl_ind_t)}, 
	{RTK_BT_GAP_EVT_ECFC_DATA_IND, sizeof(rtk_bt_ecfc_data_ind_t)}, 
	{RTK_BT_GAP_EVT_ECFC_CONN_REQ_IND, sizeof(rtk_bt_ecfc_conn_req_ind_t)}, 
	{RTK_BT_GAP_EVT_ECFC_DISCONN_IND, sizeof(rtk_bt_ecfc_disconn_ind_t)}, 
	{RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND, sizeof(rtk_bt_ecfc_reconf_req_ind_t)}, 
	{RTK_BT_GAP_EVT_ECFC_RECONF_RSP_IND, sizeof(rtk_bt_ecfc_reconf_rsp_ind_t)}, 
#endif
	{RTK_BT_GAP_EVT_MAX, 0}
};

uint32_t get_bt_gap_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gap_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_gap_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_gap_evt_mem_op_tab[i].evt == RTK_BT_GAP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

#if RTK_BT_5_2_L2C_ECFC_SUPPORT
static void gap_ipc_ecfc_send_data_pop(rtk_bt_gap_ecfc_send_data_t *param)
{
	param->p_data = (uint8_t *)param + (uint32_t)param->p_data;
}
#endif

void bt_gap_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)param;

	switch (act) {
#if RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_ACT_ECFC_SEND_DATA:
		gap_ipc_ecfc_send_data_pop((rtk_bt_gap_ecfc_send_data_t *)param);
		break;
#endif

	default:
		break;
	}
}

/* ===============================================rtk_bt_le_gap_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_le_gap_evt_mem_op_tab[] = {
	{RTK_BT_LE_GAP_EVT_ADV_START_IND, sizeof(rtk_bt_le_adv_start_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_ADV_STOP_IND, sizeof(rtk_bt_le_adv_stop_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_CONNECT_IND, sizeof(rtk_bt_le_conn_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_DISCONN_IND, sizeof(rtk_bt_le_disconn_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_SCAN_START_IND, sizeof(uint16_t)}, 
	{RTK_BT_LE_GAP_EVT_SCAN_RES_IND, sizeof(rtk_bt_le_scan_res_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_SCAN_STOP_IND, sizeof(uint16_t)}, 
	{RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND, sizeof(rtk_bt_le_conn_update_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND, sizeof(rtk_bt_le_remote_conn_update_req_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND, sizeof(rtk_bt_le_data_len_change_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND, sizeof(rtk_bt_le_phy_update_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND, sizeof(rtk_bt_le_auth_pair_cfm_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND, sizeof(rtk_bt_le_auth_key_display_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND, sizeof(rtk_bt_le_auth_key_input_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND, sizeof(rtk_bt_le_auth_key_cfm_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND, sizeof(rtk_bt_le_auth_oob_input_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND, sizeof(rtk_bt_le_auth_complete_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND, sizeof(rtk_bt_le_bond_modify_ind_t)}, 
#if RTK_BLE_5_0_AE_ADV_SUPPORT
	{RTK_BT_LE_GAP_EVT_EXT_ADV_IND, sizeof(rtk_bt_le_ext_adv_ind_t)}, 
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */
#if RTK_BLE_5_0_AE_SCAN_SUPPORT
	{RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND, sizeof(rtk_bt_le_ext_scan_res_ind_t)}, 
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */
#if RTK_BLE_5_0_PA_ADV_SUPPORT
	{RTK_BT_LE_GAP_EVT_PA_IND, sizeof(rtk_bt_le_pa_ind_t)}, 
#endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */
#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	{RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND, sizeof(rtk_bt_le_pa_sync_ind_t)}, 
	{RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND, sizeof(rtk_bt_le_pa_adv_report_ind_t)}, 
#endif
#if RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
	{RTK_BT_LE_GAP_PAST_RECEIVED_INFO_IND, sizeof(rtk_bt_le_past_recv_ind_t)}, 
#endif
	{RTK_BT_LE_GAP_EVT_RESOLV_LIST_MODIFY_IND, sizeof(rtk_bt_le_modify_resolv_list_ind_t)}, 
#if RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	{RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND, sizeof(rtk_bt_le_txpower_ind_t)}, 
#endif
	{RTK_BT_LE_GAP_EVT_MAX, 0}
};

uint32_t get_bt_le_gap_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_gap_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_le_gap_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_le_gap_evt_mem_op_tab[i].evt == RTK_BT_LE_GAP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

static void le_ipc_scan_info_filter_pop(rtk_bt_le_scan_info_filter_param_t *p_scan_info)
{
	p_scan_info->p_filter = (uint8_t *)((uint8_t *)p_scan_info + (uint32_t)p_scan_info->p_filter);
}

static void le_ipc_vendor_cmd_req_pop(rtk_bt_le_gap_vendor_cmd_param_t *vendor_cmd)
{
	vendor_cmd->cmd_param = (uint8_t *)((uint8_t *)vendor_cmd + (uint32_t)vendor_cmd->cmd_param);
}

#if RTK_BLE_5_0_AE_ADV_SUPPORT
static void le_ipc_set_ext_adv_data_pop(rtk_bt_le_ext_adv_data_t *ext_adv_data)
{
	ext_adv_data->pdata = (uint8_t *)((uint8_t *)ext_adv_data + (uint32_t)ext_adv_data->pdata);
}
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */

#if RTK_BLE_5_0_PA_ADV_SUPPORT
static void le_ipc_start_pa_pop(rtk_bt_le_pa_param_t *pa_param)
{
	pa_param->pa_data = (uint8_t *)((uint8_t *)pa_param + (uint32_t)pa_param->pa_data);
}

static void le_ipc_update_pa_pop(rtk_bt_le_pa_update_t *pa_param)
{
	pa_param->pa_data = (uint8_t *)((uint8_t *)pa_param + (uint32_t)pa_param->pa_data);
}
#endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */

void bt_le_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER:
		le_ipc_scan_info_filter_pop((rtk_bt_le_scan_info_filter_param_t *)param);
		break;
	case RTK_BT_LE_GAP_ACT_VENDOR_CMD_REQ:
		le_ipc_vendor_cmd_req_pop((rtk_bt_le_gap_vendor_cmd_param_t *)param);
		break;
#if RTK_BLE_5_0_AE_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA:
	case RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA:
		le_ipc_set_ext_adv_data_pop((rtk_bt_le_ext_adv_data_t *)param);
		break;
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */
#if RTK_BLE_5_0_PA_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_START_PA:
		le_ipc_start_pa_pop((rtk_bt_le_pa_param_t *)param);
		break;
	case RTK_BT_LE_GAP_ACT_UPDATE_PA:
		le_ipc_update_pa_pop((rtk_bt_le_pa_update_t *)param);
		break;
#endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */
	default:
		break;
	}
}

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
static int le_ipc_ext_scan_res_evt_push(uint8_t *buf, rtk_bt_le_ext_scan_res_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_ext_scan_res_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_ext_scan_res_ind_t *)buf)->data = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_ext_scan_res_ind_t size is over deep copy buf\r\n");
	return -1;
}
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if RTK_BLE_5_0_PA_SYNC_SUPPORT
static int le_ipc_pa_adv_report_evt_push(uint8_t *buf, rtk_bt_le_pa_adv_report_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_pa_adv_report_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->data_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_data, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_pa_adv_report_ind_t *)buf)->p_data = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_ext_scan_res_ind_t size is over deep copy buf\r\n");
	return -1;
}
#endif

void *bt_le_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
#if RTK_BLE_5_0_AE_SCAN_SUPPORT
	case RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND:
		ret = le_ipc_ext_scan_res_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND:
		ret = le_ipc_pa_adv_report_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	if (ret) {
		osif_mem_free(dev_msg);
		dev_msg = NULL;
	}

	return (void *)dev_msg;
}

/* ===============================================rtk_bt_gatts_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_gatts_evt_mem_op_tab[] = {
	{RTK_BT_GATTS_EVT_REGISTER_SERVICE, sizeof(rtk_bt_gatts_reg_ind_t)}, 
	{RTK_BT_GATTS_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t)}, 
	{RTK_BT_GATTS_EVT_READ_IND, sizeof(rtk_bt_gatts_read_ind_t)}, 
	{RTK_BT_GATTS_EVT_WRITE_IND,sizeof(rtk_bt_gatts_write_ind_t)}, 
	{RTK_BT_GATTS_EVT_CCCD_IND,sizeof(rtk_bt_gatts_cccd_ind_t)}, 
	{RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND, sizeof(rtk_bt_gatts_ntf_and_ind_ind_t)}, 
	{RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND,sizeof(rtk_bt_gatts_ntf_and_ind_ind_t)}, 
	{RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES,sizeof(rtk_bt_gatts_client_supported_features_ind_t)}, 
	{RTK_BT_GATTS_EVT_MAX, 0}
};

uint32_t get_bt_gatts_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gatts_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_gatts_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_gatts_evt_mem_op_tab[i].evt == RTK_BT_GATTS_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

static void gatts_ipc_reg_srv_param_pop(struct rtk_bt_gatt_service *srv_reg)
{
	uint32_t i, attr_count = 0;
	uint16_t uuid_val = 0;

	attr_count = srv_reg->attr_count;
	srv_reg->attrs = (void *) ((uint8_t *)srv_reg + (uint32_t)(srv_reg->attrs));
	for (i = 0; i < attr_count; i++) {
		srv_reg->attrs[i].uuid = (void *)((uint8_t *)srv_reg + (uint32_t)(srv_reg->attrs[i].uuid));
		if (srv_reg->attrs[i].user_data) {
			srv_reg->attrs[i].user_data = (void *)((uint8_t *)srv_reg + 
											(uint32_t)(srv_reg->attrs[i].user_data));

			if (BT_UUID_TYPE_16 == ((struct bt_uuid *)srv_reg->attrs[i].uuid)->type) {
				uuid_val =  ((struct bt_uuid_16 *)srv_reg->attrs[i].uuid)->val;
				if (BT_UUID_GATT_CHRC_VAL == uuid_val) {
					struct rtk_bt_gatt_chrc *chrc_user_data = srv_reg->attrs[i].user_data;
					chrc_user_data->uuid = (void *)((uint8_t *)(srv_reg) + (uint32_t)(chrc_user_data->uuid));
				} else if (BT_UUID_GATT_CAF_VAL == uuid_val) {
					struct rtk_bt_gatt_caf *caf = srv_reg->attrs[i].user_data;
					caf->handles = (uint16_t *)((uint8_t *)(srv_reg) + (uint32_t)(caf->handles));
				}
			}								
		}
	}
	
}

static void gatts_ipc_indicate_param_pop(rtk_bt_gatts_ntf_and_ind_param_t *ind)
{
	ind->data = (void *)((uint8_t *)ind + (uint32_t)ind->data);
}

static void gatts_ipc_notify_param_pop(rtk_bt_gatts_ntf_and_ind_param_t *notify)
{
	gatts_ipc_indicate_param_pop(notify);
}

static void gatts_ipc_read_resp_param_pop(rtk_bt_gatts_read_resp_param_t *resp_read)
{
	resp_read->data = (void *)((uint8_t *)resp_read + (uint32_t)(resp_read->data));
}

int gatts_ipc_write_evt_push(uint8_t *buf, rtk_bt_gatts_write_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gatts_write_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->value, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gatts_write_ind_t *)buf)->value = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gatts_write_ind_t size is over deep copy buf\r\n");
	return -1;
}

void bt_gatts_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_GATTS_ACT_REGISTER_SERVICE:
		gatts_ipc_reg_srv_param_pop((struct rtk_bt_gatt_service *)param);
		break;

	case RTK_BT_GATTS_ACT_NOTIFY:
		gatts_ipc_notify_param_pop((rtk_bt_gatts_ntf_and_ind_param_t *)param);
		break;

	case RTK_BT_GATTS_ACT_INDICATE:
		gatts_ipc_indicate_param_pop((rtk_bt_gatts_ntf_and_ind_param_t *)param);
		break;

	case RTK_BT_GATTS_ACT_READ_RSP:
		gatts_ipc_read_resp_param_pop((rtk_bt_gatts_read_resp_param_t *)param);
		break;

	default:
		break;
	}
}

void *bt_gatts_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	case RTK_BT_GATTS_EVT_WRITE_IND:
		if (gatts_ipc_write_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_gattc_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_gattc_evt_mem_op_tab[] = {
	{RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND, sizeof(rtk_bt_gattc_discover_ind_t)},
#if RTK_BLE_MGR_LIB
	{RTK_BT_GATTC_EVT_DISCOVER_ALL_STATE_IND, sizeof(rtk_bt_gattc_discover_all_state_ind_t)},
	{RTK_BT_GATTC_EVT_GATT_SERVICE_INFO_IND, sizeof(rtk_bt_gattc_gatt_service_info_ind_t)},
#endif
	{RTK_BT_GATTC_EVT_READ_RESULT_IND, sizeof(rtk_bt_gattc_read_ind_t)},
	{RTK_BT_GATTC_EVT_WRITE_RESULT_IND, sizeof(rtk_bt_gattc_write_ind_t)},
	{RTK_BT_GATTC_EVT_CCCD_ENABLE_IND, sizeof(rtk_bt_gattc_cccd_update_ind_t)},
	{RTK_BT_GATTC_EVT_CCCD_DISABLE_IND, sizeof(rtk_bt_gattc_cccd_update_ind_t)},
	{RTK_BT_GATTC_EVT_NOTIFY_IND, sizeof(rtk_bt_gattc_cccd_value_ind_t)},
	{RTK_BT_GATTC_EVT_INDICATE_IND, sizeof(rtk_bt_gattc_cccd_value_ind_t)},
	{RTK_BT_GATTC_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t)},
	{RTK_BT_GATTC_EVT_MAX, 0}
};

uint32_t get_bt_gattc_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_gattc_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_gattc_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_gattc_evt_mem_op_tab[i].evt == RTK_BT_GATTC_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

#if !RTK_BLE_MGR_LIB
static void gattc_ipc_read_param_pop(rtk_bt_gattc_read_param_t *param)
{
	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		param->multiple.handles = (uint16_t *)((uint8_t *)param + (uint32_t)param->multiple.handles);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		param->multiple_variable.handles = (uint16_t *)((uint8_t *)param + (uint32_t)param->multiple_variable.handles);
		break;

	default:
		break;
	}
}
#endif

static void gattc_ipc_write_param_pop(rtk_bt_gattc_write_param_t *param)
{
	param->data = (void *)((uint8_t *)param + (uint32_t)param->data);
}

void bt_gattc_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
#if !RTK_BLE_MGR_LIB
	case RTK_BT_GATTC_ACT_READ:
		gattc_ipc_read_param_pop((rtk_bt_gattc_read_param_t *)param);
		break;
#endif

	case RTK_BT_GATTC_ACT_WRITE:
		gattc_ipc_write_param_pop((rtk_bt_gattc_write_param_t *)param);
		break;

	default:
		break;
	}
}

static int gattc_ipc_read_evt_push(uint8_t *buf, rtk_bt_gattc_read_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

#if RTK_BLE_MGR_LIB

	size = sizeof(rtk_bt_gattc_read_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->value, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gattc_read_ind_t *)buf)->value = (uint8_t *)(struct_end - buf);
	*actual_size = (uint32_t)(end - buf);

#else

	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->by_handle.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->by_handle.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->by_handle.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_BY_UUID:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->by_uuid_per.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->by_uuid_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->by_uuid_per.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;
	
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->multiple_per.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->multiple_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->multiple_per.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->multiple_variable_per.len;
		if (end + size > buf + size_limit)
			goto exit;
		memcpy(end, param->multiple_variable_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->multiple_variable_per.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	default:
		printf("rtk_bt_gattc_read_ind_t type unknown\r\n");
		goto exit;
		break;
	}
#endif

	return 0;

exit:
	printf("rtk_bt_gattc_read_ind_t size is over deep copy buf\r\n");
	return -1;
}

static int gatts_ipc_notify_indicate_evt_push(uint8_t *buf, rtk_bt_gattc_cccd_value_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gattc_cccd_value_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->value, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gattc_cccd_value_ind_t *)buf)->value = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gattc_cccd_value_ind_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_gattc_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		if (gattc_ipc_read_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	case RTK_BT_GATTC_EVT_NOTIFY_IND:
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		if (gatts_ipc_notify_indicate_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

/* ===============================================rtk_bt_br_gap_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_br_gap_evt_mem_op_tab[] = {
	{RTK_BT_BR_GAP_INQUIRY_RESULT, sizeof(rtk_bt_br_inquiry_result_t)},
	{RTK_BT_BR_GAP_ACL_CONN_IND, 6},
	{RKT_BT_BR_GAP_ACL_CONN_SUCCESS, 6},
	{RTK_BT_BR_GAP_ACL_DISCONN, 6},
	{RTK_BT_BR_GAP_EVT_MAX, 0}
};

uint32_t get_bt_br_gap_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_br_gap_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_br_gap_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_br_gap_evt_mem_op_tab[i].evt == RTK_BT_BR_GAP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

int br_gap_ipc_inquiry_result_evt_push(uint8_t *buf, rtk_bt_br_inquiry_result_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_br_inquiry_result_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->eir_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_eir, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_br_inquiry_result_t *)buf)->p_eir = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_br_inquiry_result_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_br_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
		case RTK_BT_BR_GAP_INQUIRY_RESULT:
			if (br_gap_ipc_inquiry_result_evt_push((uint8_t*)dev_msg->param_buf, data,
										IPC_DEV_API_DATA_MAX, pactual_size)) {
				osif_mem_free(dev_msg);
				dev_msg = NULL;
			}
		break;
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

void bt_br_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_a2dp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_a2dp_evt_mem_op_tab[] = {
	{RTK_BT_A2DP_EVT_CONN_IND, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_CONN_CMPL, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_DISCONN_CMPL, sizeof(rtk_bt_a2dp_disconn_ind_t)},
	{RTK_BT_A2DP_EVT_CONFIG_CMPL, sizeof(rtk_bt_a2dp_codec_t)},
	{RTK_BT_A2DP_EVT_STREAM_OPEN, 0},
	{RTK_BT_A2DP_EVT_STREAM_START_IND, sizeof(rtk_bt_a2dp_stream_start_t)},
	{RTK_BT_A2DP_EVT_STREAM_START_RSP, sizeof(rtk_bt_a2dp_stream_start_t)},
	{RTK_BT_A2DP_EVT_STREAM_STOP, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_STREAM_CLOSE, sizeof(rtk_bt_a2dp_conn_ind_t)},
	{RTK_BT_A2DP_EVT_STREAM_DATA_IND, sizeof(rtk_bt_a2dp_stream_data_ind_t)},
	{RTK_BT_A2DP_EVT_STREAM_DATA_RSP, sizeof(rtk_bt_a2dp_stream_data_rsp_t)},
	{RTK_BT_A2DP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_a2dp_sdp_attr_info_t)},
	{RTK_BT_A2DP_EVT_MAX, 0}
};

uint32_t get_bt_a2dp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_a2dp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_a2dp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_a2dp_evt_mem_op_tab[i].evt == RTK_BT_A2DP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_a2dp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

static void a2dp_ipc_stream_data_send_pop(rtk_bt_a2dp_stream_data_send_t *p_data_send_t)
{
	p_data_send_t->frame_buf = (uint8_t *)((uint8_t *)p_data_send_t + (uint32_t)(p_data_send_t->frame_buf));
}

void bt_a2dp_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_A2DP_ACT_SEND_DATA:
		a2dp_ipc_stream_data_send_pop((rtk_bt_a2dp_stream_data_send_t *)param);
		break;

	default:
		break;
	}
}

/* ===============================================rtk_bt_avrcp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_avrcp_evt_mem_op_tab[] = {
	{RTK_BT_AVRCP_EVT_CONN_IND, sizeof(rtk_bt_avrcp_conn_ind_t)}, 
	{RTK_BT_AVRCP_EVT_ABSOLUTE_VOLUME_SET, sizeof(rtk_bt_avrcp_absolute_volume_set_t)},
	{RTK_BT_AVRCP_EVT_VOLUME_CHANGED, sizeof(rtk_bt_avrcp_volume_t)}, 
	{RTK_BT_AVRCP_EVT_VOLUME_UP, 0}, 
	{RTK_BT_AVRCP_EVT_VOLUME_DOWN, 0}, 
	{RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED, 0}, 
	{RTK_BT_AVRCP_EVT_CONN_CMPL, 0}, 
	{RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED, sizeof(rtk_bt_avrcp_sub_event_t)}, 
	{RTK_BT_AVRCP_EVT_MAX, 0}
};

uint32_t get_bt_avrcp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_avrcp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_avrcp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_avrcp_evt_mem_op_tab[i].evt == RTK_BT_AVRCP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_avrcp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

void bt_avrcp_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_spp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_spp_evt_mem_op_tab[] = {
	{RTK_BT_SPP_EVT_CONN_COMPL, sizeof(rtk_bt_spp_conn_cmpl_t)},
	{RTK_BT_SPP_EVT_CREDIT_RCVD, sizeof(rtk_bt_spp_credit_rcvd_t)},
	{RTK_BT_SPP_EVT_DATA_IND, sizeof(rtk_bt_spp_data_ind_t)},
	{RTK_BT_SPP_EVT_DATA_RSP, sizeof(rtk_bt_spp_data_rsp_t)},
	{RTK_BT_SPP_EVT_CONN_IND, sizeof(rtk_bt_spp_conn_ind_t)},
	{RTK_BT_SPP_EVT_DISCONN_CMPL, sizeof(rtk_bt_spp_disconn_cmpl_t)},
	{RTK_BT_SPP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_spp_attr_info_t)},
	{RTK_BT_SPP_EVT_SDP_DISCOV_CMPL, sizeof(rtk_bt_spp_sdp_discov_cmpl_t)},
	{RTK_BT_SPP_EVT_MAX, 0}
};

uint32_t get_bt_spp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_spp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_spp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_spp_evt_mem_op_tab[i].evt == RTK_BT_SPP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_spp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

static void spp_ipc_data_send_pop(rtk_bt_spp_send_data_t *p_data_send_t)
{
	p_data_send_t->data = (uint8_t *)((uint8_t *)p_data_send_t + (uint32_t)(p_data_send_t->data));
}

void bt_spp_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_SPP_ACT_SEND_DATA:
		spp_ipc_data_send_pop((rtk_bt_spp_send_data_t *)param);
		break;

	default:
		break;
	}
}

/* ===============================================rtk_bt_sdp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_sdp_evt_mem_op_tab[] = {
	{RTK_BT_SDP_EVT_MAX, 0}
};

uint32_t get_bt_sdp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_sdp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_sdp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_sdp_evt_mem_op_tab[i].evt == RTK_BT_SDP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_sdp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

void bt_sdp_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)act;
	(void)param;
}

/* ===============================================rtk_bt_hfp_ipc_dev action======================================= */
static struct evt_mem_option rtk_bt_hfp_evt_mem_op_tab[] = {
	{RTK_BT_HFP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_hfp_sdp_attr_info_t)},
	{RTK_BT_HFP_EVT_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t)},
	{RTK_BT_HFP_EVT_CONN_CMPL, sizeof(rtk_bt_hfp_conn_ind_t)},
	{RTK_BT_HFP_EVT_CALL_STATUS, sizeof(rtk_bt_hfp_call_status_ind_t)},
	{RTK_BT_HFP_EVT_CALLER_ID_IND, sizeof(rtk_bt_hfp_caller_id_ind_t)},
	{RTK_BT_HFP_EVT_DISCONN_CMPL, sizeof(rtk_bt_hfp_disconn_ind_t)},
	{RTK_BT_HFP_EVT_SCO_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t)},
	{RTK_BT_HFP_EVT_SCO_CONN_CMPL, sizeof(rtk_bt_hfp_codec_t)},
	{RTK_BT_HFP_EVT_SCO_DATA_IND, sizeof(rtk_bt_hfp_sco_data_ind_t)},
	{RTK_BT_HFP_EVT_SCO_DISCONNCTED_IND, sizeof(rtk_bt_hfp_disconn_ind_t)},
	{RTK_BT_HFP_EVT_AG_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t)},
	{RTK_BT_HFP_EVT_AG_CONN_CMPL, sizeof(rtk_bt_hfp_conn_ind_t)},
	{RTK_BT_HFP_EVT_AG_DISCONN_CMPL, sizeof(rtk_bt_hfp_disconn_ind_t)},
	{RTK_BT_HFP_EVT_AG_INDICATORS_STATUS_REQ, sizeof(rtk_bt_hfp_ag_indicators_status_req_t)},
	{RTK_BT_HFP_EVT_AG_CURR_CALLS_LIST_QUERY, sizeof(rtk_bt_hfp_ag_curr_calls_list_query_t)},
	{RTK_BT_HFP_EVT_AG_DIAL_LAST_NUMBER, sizeof(rtk_bt_hfp_ag_dial_last_number_ind_t)},
	{RTK_BT_HFP_EVT_AG_CALL_ANSWER_REQ, sizeof(rtk_bt_hfp_ag_call_answer_req_ind_t)},
	{RTK_BT_HFP_EVT_AG_CALL_TERMINATE_REQ, sizeof(rtk_bt_hfp_ag_call_terminate_req_ind_t)},
	{RTK_BT_HFP_EVT_AG_CALL_STATUS, sizeof(rtk_bt_hfp_call_status_ind_t)},
	{RTK_BT_HFP_EVT_MAX, 0}
};

uint32_t get_bt_hfp_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_hfp_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_hfp_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_hfp_evt_mem_op_tab[i].evt == RTK_BT_HFP_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}

void *bt_hfp_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	/* TODO */

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	
	return (void *)dev_msg;
}

static void hfp_ipc_stream_data_send_pop(rtk_bt_hfp_sco_data_send_t *p_data_send_t)
{
	p_data_send_t->buf = (uint8_t *)((uint8_t *)p_data_send_t + (uint32_t)(p_data_send_t->buf));
}

void bt_hfp_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_HFP_ACT_SEND_SCO_DATA:
		hfp_ipc_stream_data_send_pop((rtk_bt_hfp_sco_data_send_t *)param);
		break;

	default:
		break;
	}
}

/* ===============================================rtk_bt_le_iso_ipc_dev action======================================= */
#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)
static struct evt_mem_option rtk_bt_le_iso_evt_mem_op_tab[] = {
	{RTK_BT_LE_ISO_EVT_CIG_DISCONNECT_INFO, sizeof(rtk_bt_le_iso_cig_disconnect_info_t)},
	{RTK_BT_LE_ISO_EVT_CIG_CIS_ESTABLISHED_INFO, sizeof(rtk_bt_le_iso_cig_cis_established_info_t)},
	{RTK_BT_LE_ISO_EVT_CIG_ACCEPTOR_REQUEST_CIS_IND, sizeof(rtk_bt_le_iso_cig_acceptor_request_cis_ind_t)},
	{RTK_BT_LE_ISO_EVT_BIG_BROADCASTER_CREATE_CMPL_INFO, sizeof(rtk_bt_le_iso_big_broadcaster_create_cmpl_info_t)},
	{RTK_BT_LE_ISO_EVT_BIG_BROADCASTER_SYNC_STATE_IND, sizeof(rtk_bt_le_iso_big_broadcaster_sync_state_ind_t)},
	{RTK_BT_LE_ISO_EVT_BIG_RECEIVER_SYNC_ESTABLISHED_INFO, sizeof(rtk_bt_le_iso_big_receiver_sync_established_info_t)},
	{RTK_BT_LE_ISO_EVT_BIG_RECEIVER_SYNC_STATE_IND, sizeof(rtk_bt_le_iso_big_receiver_sync_state_ind_t)},
	{RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND, sizeof(rtk_bt_le_iso_direct_iso_data_ind_t)},
	{RTK_BT_LE_ISO_EVT_MAX, 0}
};

uint32_t get_bt_le_iso_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_iso_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_le_iso_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_le_iso_evt_mem_op_tab[i].evt == RTK_BT_LE_ISO_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}
int iso_ipc_data_ind_evt_push(uint8_t *buf, rtk_bt_le_iso_direct_iso_data_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_direct_iso_data_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->buf_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_buf, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_direct_iso_data_ind_t *)buf)->p_buf = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_iso_direct_iso_data_ind_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_le_iso_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	case RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND:
		ret = iso_ipc_data_ind_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;

	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}
	if (ret) {
		osif_mem_free(dev_msg);
		dev_msg = NULL;
	}	
	return (void *)dev_msg;
}

static void le_ipc_iso_setup_path_pop(rtk_bt_le_iso_setup_path_param_t *param)
{	
	param->p_codec_config = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_codec_config);
}	
static void le_ipc_iso_big_broadcaster_create_pop(rtk_bt_le_iso_big_broadcaster_create_param_t *param)
{
	param->p_big_param = (rtk_bt_le_iso_big_broadcaster_param_t *)((uint8_t *)param + (uint32_t)param->p_big_param);
}
static void le_ipc_iso_big_recevier_create_sync_pop(rtk_bt_le_iso_big_receiver_create_sync_t *param)
{
	param->p_sync_param = (rtk_bt_le_iso_big_receiver_create_sync_param_t *)((uint8_t *)param + (uint32_t)param->p_sync_param);
}		
static void le_ipc_iso_data_send_pop(rtk_bt_le_iso_data_send_info_t *param)
{
	param->p_data = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_data);
}	

void bt_le_iso_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
		case RTK_BT_LE_ISO_ACT_BIG_SETUP_PATH:
		case RTK_BT_LE_ISO_ACT_CIG_SETUP_PATH:
			le_ipc_iso_setup_path_pop((rtk_bt_le_iso_setup_path_param_t *)param);
			break;
		case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE:
			le_ipc_iso_big_broadcaster_create_pop((rtk_bt_le_iso_big_broadcaster_create_param_t *)param);
			break;
		case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC:
			le_ipc_iso_big_recevier_create_sync_pop((rtk_bt_le_iso_big_receiver_create_sync_t *)param);
			break;			
		case RTK_BT_LE_ISO_ACT_ISO_DATA_SEND:
			le_ipc_iso_data_send_pop((rtk_bt_le_iso_data_send_info_t *)param);
			break;
		default:
			break;
	}
}
#else
uint32_t get_bt_le_iso_evt_mem_size(uint8_t evt_code)
{
	(void)evt_code;
	return 0;
}
void *bt_le_iso_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	(void)event;	
	(void)data;	
	(void)buf_len;	
	(void)pactual_size;
	return NULL;
}
void bt_le_iso_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)act;	
	(void)param;		
}
#endif

/* ===============================================rtk_bt_le_audio_ipc_dev action======================================= */
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
static struct evt_mem_option rtk_bt_le_audio_evt_mem_op_tab[] = {
	{RTK_BT_LE_AUDIO_EVT_CODEC_CFG_IND, sizeof(rtk_bt_le_audio_codec_cfg_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_QOS_CFG_IND, sizeof(rtk_bt_le_audio_qos_cfg_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_ENABLE_IND, sizeof(rtk_bt_le_audio_enable_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_DISABLE_IND, sizeof(rtk_bt_le_audio_disable_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_RELEASE_IND, sizeof(rtk_bt_le_audio_release_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND, sizeof(rtk_bt_le_audio_direct_iso_data_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_DISCOVERY_DONE_IND, sizeof(rtk_bt_le_audio_mcs_client_discovery_done_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_READ_RESULT_IND, sizeof(rtk_bt_le_audio_mcs_client_read_result_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_NOTIFY_RESULT_IND, sizeof(rtk_bt_le_audio_mcs_client_notify_result_ind_t)},
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
	{RTK_BT_LE_AUDIO_EVT_PA_SYNC_STATE_IND, sizeof(rtk_bt_le_audio_pa_sync_state_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_BIG_SYNC_STATE_IND, sizeof(rtk_bt_le_audio_big_sync_state_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_PA_BIG_INFO_IND, sizeof(rtk_bt_le_audio_pa_biginfo_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND, sizeof(rtk_bt_le_audio_pa_adv_report_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND, sizeof(rtk_bt_le_audio_bass_get_pa_sync_param_ind_t)},	
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND, sizeof(rtk_bt_le_audio_bass_get_big_sync_param_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND, sizeof(rtk_bt_le_audio_bass_get_broadcast_code_ind_t)},
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND, sizeof(rtk_bt_le_audio_bass_set_prefer_bis_sync_ind_t)},
#endif
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
	{RTK_BT_LE_AUDIO_EVT_BROADCAST_SOURCE_STATE_IND, sizeof(rtk_bt_le_audio_broadcast_source_state_ind_t)},
#endif
	{RTK_BT_LE_AUDIO_EVT_MAX, 0}
};

uint32_t get_bt_le_audio_evt_mem_size(uint8_t evt_code)
{
	uint8_t i = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_audio_evt_mem_op_tab[i].evt == evt_code) {
			return rtk_bt_le_audio_evt_mem_op_tab[i].size;
		}
		if (rtk_bt_le_audio_evt_mem_op_tab[i].evt == RTK_BT_LE_AUDIO_EVT_MAX) {
			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
			return 0xFFFFFFFF;
		}
		i++;
	}
}
int le_audio_ipc_iso_data_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_direct_iso_data_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_direct_iso_data_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->buf_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_buf, size);
	((rtk_bt_le_audio_direct_iso_data_ind_t *)buf)->p_buf = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_direct_iso_data_ind_t size is over deep copy buf\r\n");
	return -1;
}

int le_audio_ipc_mcs_client_read_result_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_mcs_client_read_result_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_mcs_client_read_result_ind_t *p_ind = (rtk_bt_le_audio_mcs_client_read_result_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_mcs_client_read_result_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	switch (param->uuid) {
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_NAME_CHAR_UUID:
			size = param->data.media_player_name.name_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->data.media_player_name.player_name, size);
			p_ind->data.media_player_name.player_name = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_OBJID_CHAR_UUID:
			size = param->data.media_player_icon_url.url_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->data.media_player_icon_url.p_icon_url, size);
			p_ind->data.media_player_icon_url.p_icon_url = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_URL_CHAR_UUID:
			size = param->data.track_title_url.title_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->data.track_title_url.title_name, size);
			p_ind->data.track_title_url.title_name = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			break;
		default:
			break;
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_mcs_client_read_result_ind_t size is over deep copy buf\r\n");
	return -1;
}
int le_audio_ipc_mcs_client_notify_result_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_mcs_client_notify_result_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_mcs_client_notify_result_ind_t *p_ind = (rtk_bt_le_audio_mcs_client_notify_result_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_mcs_client_notify_result_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	switch (param->uuid) {
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_NAME_CHAR_UUID:
			size = param->data.media_player_name.name_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->data.media_player_name.player_name, size);
			p_ind->data.media_player_name.player_name = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_OBJID_CHAR_UUID:
			size = param->data.media_player_icon_url.url_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->data.media_player_icon_url.p_icon_url, size);
			p_ind->data.media_player_icon_url.p_icon_url = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			break;
		case RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_URL_CHAR_UUID:
			size = param->data.track_title_url.title_len;
			if (end + size > buf + size_limit)
				goto exit;
			memcpy(end, param->data.track_title_url.title_name, size);
			p_ind->data.track_title_url.title_name = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);
			break;
		default:
			break;
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_mcs_client_read_result_ind_t size is over deep copy buf\r\n");
	return -1;
}
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
int le_audio_ipc_pa_adv_report_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_pa_adv_report_ind_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_pa_adv_report_ind_t *buf_report = (rtk_bt_le_audio_pa_adv_report_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_pa_adv_report_ind_t);
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->data_len;
	if (end + size > buf + size_limit)
		goto exit;
	memcpy(end, param->p_data, size);
	buf_report->p_data = (uint8_t *)(end - buf);	
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_pa_adv_report_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
#endif
void *bt_le_audio_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));
	memset(dev_msg,0,sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	switch (event) {
	case RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND:
		ret = le_audio_ipc_iso_data_ind_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_READ_RESULT_IND:
		ret = le_audio_ipc_mcs_client_read_result_ind_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_MCS_CLIENT_NOTIFY_RESULT_IND:
		ret = le_audio_ipc_mcs_client_notify_result_ind_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
	case RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND:
		ret = le_audio_ipc_pa_adv_report_ind_evt_push((uint8_t*)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
	default:
		memcpy((void *)dev_msg->param_buf, (void *)data, buf_len);
		*pactual_size = buf_len;
		break;
	}

	if(ret) {
		osif_mem_free(dev_msg);
		dev_msg = NULL;
	}	
	return (void *)dev_msg;
}

static void le_ipc_audio_iso_data_send_pop(rtk_bt_le_audio_iso_data_send_info_t *param)
{
	param->p_data = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_data);
}
static void le_ipc_audio_mcs_media_player_info_set_pop(rtk_bt_le_audio_mcs_media_player_info_set_t *param)
{
	param->media_player_info.p_player_name = (uint8_t *)((uint8_t *)param + (uint32_t)param->media_player_info.p_player_name);
	param->media_player_info.p_icon_fmt = (uint8_t *)((uint8_t *)param + (uint32_t)param->media_player_info.p_icon_fmt);
	param->media_player_info.p_url = (uint8_t *)((uint8_t *)param + (uint32_t)param->media_player_info.p_url);
}
static void le_ipc_audio_mcs_media_track_info_set_pop(rtk_bt_le_audio_mcs_media_track_info_set_t *param)
{
	param->media_track_info.p_title = (uint8_t *)((uint8_t *)param + (uint32_t)param->media_track_info.p_title);
}	
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
static void le_ipc_audio_sync_setup_path_pop(rtk_bt_le_audio_sync_setup_data_path_param_t *param)
{
	param->setup_param.p_codec_config = (uint8_t *)((uint8_t *)param + (uint32_t)param->setup_param.p_codec_config);
}
#endif
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
static void le_ipc_audio_broadcast_source_create_pop(rtk_bt_le_audio_broadcast_source_create_param_t *param)
{
	uint8_t i=0,j=0,k=0;

	for(i=0;i<param->num_groups;i++) {
		for(j=0;j<param->group[i].num_subgroups;j++) {
			//restore codec_cfg_l2
			param->group[i].subgroup[j].p_codec_cfg_l2 = (uint8_t *)((uint8_t *)param + (uint32_t)param->group[i].subgroup[j].p_codec_cfg_l2);

			//restore metadata
			param->group[i].subgroup[j].p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)param->group[i].subgroup[j].p_metadata);
		
			for(k=0;k<param->group[i].subgroup[j].num_bis;k++) {
				//restore p_codec_cfg_l3 
				param->group[i].subgroup[j].bis[k].p_codec_cfg_l3 = (uint8_t *)((uint8_t *)param + (uint32_t)param->group[i].subgroup[j].bis[k].p_codec_cfg_l3);			
			}
		}
	}
}
static void le_ipc_audio_broadcast_source_setup_path_pop(rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *param)
{
		param->setup_param.p_codec_config = (uint8_t *)((uint8_t *)param + (uint32_t)param->setup_param.p_codec_config);
}
static void le_ipc_audio_broadcast_source_reconfig_pop(rtk_bt_le_audio_broadcast_source_reconfig_param_t *param)
{
		param->p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_metadata);
}
#endif
void bt_le_audio_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {			
		case RTK_BT_LE_AUDIO_ACT_ISO_DATA_SEND:
			le_ipc_audio_iso_data_send_pop((rtk_bt_le_audio_iso_data_send_info_t *)param);
			break;
		case RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_PLAYER_INFO_SET:
			le_ipc_audio_mcs_media_player_info_set_pop((rtk_bt_le_audio_mcs_media_player_info_set_t *)param);
			break;
		case RTK_BT_LE_AUDIO_ACT_MCS_MEDIA_TRACK_INFO_SET:
			le_ipc_audio_mcs_media_track_info_set_pop((rtk_bt_le_audio_mcs_media_track_info_set_t *)param);
			break;
#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)
		case RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH:
			le_ipc_audio_sync_setup_path_pop((rtk_bt_le_audio_sync_setup_data_path_param_t *)param);
			break;
#endif
#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
		case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE:
			le_ipc_audio_broadcast_source_create_pop((rtk_bt_le_audio_broadcast_source_create_param_t *)param);
			break;
		case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH:
			le_ipc_audio_broadcast_source_setup_path_pop((rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)param);
			break;
		case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG:
			le_ipc_audio_broadcast_source_reconfig_pop((rtk_bt_le_audio_broadcast_source_reconfig_param_t *)param);
			break;
#endif
		default:
			break;
	}
}
#else
uint32_t get_bt_le_audio_evt_mem_size(uint8_t evt_code)
{
	(void)evt_code;
	return 0;
}
void *bt_le_audio_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	(void)event;	
	(void)data;	
	(void)buf_len;	
	(void)pactual_size;
	return NULL;
}
void bt_le_audio_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)act;	
	(void)param;		
}
#endif
#endif