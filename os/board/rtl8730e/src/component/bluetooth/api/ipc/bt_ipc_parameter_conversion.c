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
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_avrcp.h>
#include <rtk_bt_a2dp.h>
#include <rtk_bt_spp.h>
#include <rtk_bt_hfp.h>
#include <rtk_bt_sdp.h>
#include <rtk_bt_le_iso.h>
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
#include <rtk_bt_le_audio.h>
#endif
#include <bt_ipc_cfg.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_profile_config.h>
#include <bt_ipc_parameter_conversion.h>
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_common.h>
#include <rtk_bt_mesh_config_client_model.h>
#include <rtk_bt_mesh_generic_onoff_model.h>
#include <rtk_bt_mesh_datatrans_model.h>
#include <rtk_bt_mesh_remote_prov_model.h>
#include <rtk_bt_mesh_light_model.h>
#include <rtk_bt_mesh_time_model.h>
#include <rtk_bt_mesh_scheduler_model.h>
#include <rtk_bt_mesh_scene_model.h>
#include <rtk_bt_mesh_generic_default_transition_time.h>
#include <rtk_bt_mesh_generic_model.h>
#include <rtk_bt_mesh_sensor_model.h>
#endif

#define ALIGN_UP(a, size)       ((a+size-1) & (~(size-1)))

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
/* ===============================================rtk_bt_device ipc action======================================= */

static int device_ipc_bt_enable_push(uint8_t *buf, rtk_bt_app_conf_t *param,
									 uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_app_conf_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
	rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf = &param->le_audio_app_conf;
	rtk_bt_le_audio_pacs_init_param_t *p_pacs_param	= &p_le_audio_app_conf->pacs_param;
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	rtk_bt_le_audio_aics_init_param_t *p_aics_param	= &p_le_audio_app_conf->cap_param.aics_param;
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	rtk_bt_le_audio_vocs_init_param_t *p_vocs_param	= &p_le_audio_app_conf->cap_param.vocs_param;
#endif
	if (p_pacs_param->pac_sink_codec_len) {
		//copy pac sink codec
		size = p_pacs_param->pac_sink_codec_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, p_pacs_param->p_pac_sink_codec, size);
		((rtk_bt_app_conf_t *)buf)->le_audio_app_conf.pacs_param.p_pac_sink_codec = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	if (p_pacs_param->pac_source_codec_len) {
		//copy pac source codec
		size = p_pacs_param->pac_source_codec_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, p_pacs_param->p_pac_source_codec, size);
		((rtk_bt_app_conf_t *)buf)->le_audio_app_conf.pacs_param.p_pac_source_codec = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	if (p_aics_param->aics_vcs_num) {
		//copy aics_vcs_tbl
		size = p_aics_param->aics_vcs_num * sizeof(uint8_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, p_aics_param->p_aics_vcs_tbl, size);
		((rtk_bt_app_conf_t *)buf)->le_audio_app_conf.cap_param.aics_param.p_aics_vcs_tbl = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}
	if (p_aics_param->aics_mics_num) {
		//copy aics_mics_tbl
		size = p_aics_param->aics_mics_num * sizeof(uint8_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, p_aics_param->p_aics_mics_tbl, size);
		((rtk_bt_app_conf_t *)buf)->le_audio_app_conf.cap_param.aics_param.p_aics_mics_tbl = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	if (p_vocs_param->vocs_num) {
		//copy vocs_feature_tbl
		size = p_vocs_param->vocs_num * sizeof(uint8_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, p_vocs_param->p_vocs_feature_tbl, size);
		((rtk_bt_app_conf_t *)buf)->le_audio_app_conf.cap_param.vocs_param.p_vocs_feature_tbl = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}
#endif

#endif

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("device_ipc_bt_enable_push size is over deep copy buf\r\n");
	return -1;
}

void *bt_device_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {

	case RTK_BT_DEVICE_IPC_ACT_BT_ENABLE:
		ret = device_ipc_bt_enable_push((uint8_t *)host_msg->param_buf, (rtk_bt_app_conf_t *)data,
										IPC_HOST_API_DATA_MAX, pactual_size);
		break;

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

uint16_t rtk_bt_device_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_DEVICE_IPC_ACT_BT_ENABLE) {
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
		rtk_bt_app_conf_t *param = (rtk_bt_app_conf_t *)dst;
		rtk_bt_le_audio_app_conf_t *le_audio_app_conf = &param->le_audio_app_conf;

		if (le_audio_app_conf == NULL || le_audio_app_conf->bap_role == RTK_BT_LE_AUDIO_BAP_ROLE_UNKNOWN) {
			return RTK_BT_OK;
		}

		if (&le_audio_app_conf->pacs_param) {
			if (le_audio_app_conf->pacs_param.p_sink_pac_id) {
				*le_audio_app_conf->pacs_param.p_sink_pac_id = *((int8_t *)src);
			}
			if (le_audio_app_conf->pacs_param.p_source_pac_id) {
				*le_audio_app_conf->pacs_param.p_source_pac_id = *((int8_t *)src + 1);
			}
		}
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
		if (&le_audio_app_conf->cap_param.mcs_param) {
			if (le_audio_app_conf->cap_param.mcs_param.p_mcs_service_id) {
				*le_audio_app_conf->cap_param.mcs_param.p_mcs_service_id = *((int8_t *)src + 2);
			}
		}
#endif
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

	switch (act) {
#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
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

#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
static int gap_ipc_ecfc_send_data_push(uint8_t *buf, rtk_bt_gap_ecfc_send_data_t *param,
									   uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

	/* memory space for rtk_bt_gatts_ntf_and_ind_param_t */
	size = sizeof(rtk_bt_gap_ecfc_send_data_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->length;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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

static int gap_ipc_vendor_cmd_req_push(uint8_t *buf, rtk_bt_gap_vendor_cmd_param_t *param,
									   uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_gap_vendor_cmd_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->cmd_param, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gap_vendor_cmd_param_t *)buf)->cmd_param = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_gap_vendor_cmd_param_t size is over deep copy buf\r\n");
	return -1;
}

void *bt_gap_push_cmd_ipc_buf(uint16_t act, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_host_request_message *host_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_ipc_host_request_message));

	if (!host_msg) {
		printf("%s allocate hsot_msg fail \r\n", __func__);
		return NULL;
	}
	switch (act) {
#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_ACT_ECFC_SEND_DATA:
		ret = gap_ipc_ecfc_send_data_push((uint8_t *)host_msg->param_buf, (rtk_bt_gap_ecfc_send_data_t *)data,
										  IPC_HOST_API_DATA_MAX, pactual_size);
		break;
#endif
	case RTK_BT_GAP_ACT_VENDOR_CMD_REQ:
		ret = gap_ipc_vendor_cmd_req_push((uint8_t *)host_msg->param_buf, (rtk_bt_gap_vendor_cmd_param_t *)data,
										  IPC_HOST_API_DATA_MAX, pactual_size);
		break;
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
	{RTK_BT_LE_GAP_ACT_GET_CONN_INFO, sizeof(rtk_bt_le_conn_info_t)},
	{RTK_BT_LE_GAP_ACT_GET_DEV_STATE, sizeof(rtk_bt_le_gap_dev_state_t)},
	{RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN, sizeof(rtk_bt_le_get_active_conn_t)},
	{RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR, sizeof(uint16_t)},
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
		act == RTK_BT_LE_GAP_ACT_GET_BOND_NUM) {
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
		memcpy((void *)pget_bondinfo->p_bond_info, (void *)((uint8_t *)src + 4), sizeof(rtk_bt_le_bond_info_t) * (*pget_bondinfo->p_size));
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
#if defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
	else if (act == RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV) {
		rtk_bt_le_ext_adv_create_t *p_create = (rtk_bt_le_ext_adv_create_t *)dst;
		memcpy((void *)p_create->p_adv_handle, (void *)src, rtk_le_get_act_ret_param_size(act));
	}
#endif
#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
	else if (act == RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE) {
		rtk_bt_le_pa_sync_create_t *pa_sync_create = (rtk_bt_le_pa_sync_create_t *)dst;
		memcpy((void *)pa_sync_create->p_sync_id, (void *)src, rtk_le_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM) {
		rtk_bt_le_pa_sync_get_param_t *sync_get_param = (rtk_bt_le_pa_sync_get_param_t *)dst;
		uint8_t size = 0;
		if (sync_get_param->type == RTK_BT_LE_PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE
			|| sync_get_param->type == RTK_BT_LE_PA_SYNC_PARAM_DEV_STATE) {
			size = sizeof(uint8_t);
		} else if (sync_get_param->type == RTK_BT_LE_PA_SYNC_PARAM_SYNCHRONIZED_PARAM) {
			size = sizeof(rtk_bt_le_pa_sync_sync_param_t);
		} else {
			printf("RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM type is error\r\n");
		}
		memcpy((void *)sync_get_param->p_value, (void *)src, size);
	}
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */
#if defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	else if (act == RTK_BT_LE_GAP_ACT_READ_LOCAL_TX_POWER) {
		rtk_bt_le_txpower_read_local_t *param = (rtk_bt_le_txpower_read_local_t *)dst;
		*param->cur_txpower = *((int8_t *)src);
		*param->max_txpower = *((int8_t *)src + 1);
	}
#endif
	else if (act == RTK_BT_LE_GAP_ACT_GET_CONN_ID) {
		rtk_bt_le_get_conn_id_t *param = (rtk_bt_le_get_conn_id_t *)dst;
		*param->p_conn_id = *((uint8_t *)src);
	}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_filter, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_scan_info_filter_param_t *)buf)->p_filter = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_scan_info_filter_param_t size is over deep copy buf\r\n");
	return -1;
}

#if defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
static int le_ipc_create_ext_adv_push(uint8_t *buf, rtk_bt_le_ext_adv_create_t *param,
									  uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

	/* memory space for rtk_bt_le_ext_adv_data_t */
	size = sizeof(rtk_bt_le_ext_adv_create_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = sizeof(rtk_bt_le_ext_adv_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_adv_param, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_ext_adv_create_t *)buf)->p_adv_param = (rtk_bt_le_ext_adv_param_t *)(struct_end - buf);
	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_ext_adv_create_t size is over deep copy buf\r\n");
	return -1;
}

static int le_ipc_set_ext_adv_data_push(uint8_t *buf, rtk_bt_le_ext_adv_data_t *param,
										uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	/* memory space for rtk_bt_le_ext_adv_data_t */
	size = sizeof(rtk_bt_le_ext_adv_data_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
static int le_ipc_start_pa_push(uint8_t *buf, rtk_bt_le_pa_param_t *param,
								uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *struct_end = NULL;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_pa_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->pa_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->pa_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
#if defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV:
		ret = le_ipc_create_ext_adv_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_ext_adv_create_t *)data,
										 IPC_HOST_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA:
	case RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA:
		ret = le_ipc_set_ext_adv_data_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_ext_adv_data_t *)data,
										   IPC_HOST_API_DATA_MAX, pactual_size);
		break;
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */
#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
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

#if defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) && RTK_BLE_5_0_AE_SCAN_SUPPORT
static void le_ipc_ext_scan_res_evt_pop(rtk_bt_le_ext_scan_res_ind_t *ext_scan_res)
{
	ext_scan_res->data = (uint8_t *)((uint8_t *)ext_scan_res + (uint32_t)ext_scan_res->data);
}
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
static void le_ipc_pa_adv_report_evt_pop(rtk_bt_le_pa_adv_report_ind_t *pa_adv_report)
{
	pa_adv_report->p_data = (uint8_t *)((uint8_t *)pa_adv_report + (uint32_t)pa_adv_report->p_data);
}
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */

void bt_le_ipc_pop_event_param(uint8_t act, void *param)
{
#if (!defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) || !RTK_BLE_5_0_AE_SCAN_SUPPORT) && (!defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) || !RTK_BLE_5_0_PA_SYNC_SUPPORT)
	(void)param;
#endif
	switch (act) {
#if defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) && RTK_BLE_5_0_AE_SCAN_SUPPORT
	case RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND:
		le_ipc_ext_scan_res_evt_pop((rtk_bt_le_ext_scan_res_ind_t *)param);
		break;
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	buf_ptr->attrs = (rtk_bt_gatt_attr_t *)end;

	/* memory space for struct rtk_bt_gatt_attr_t of attr_count num */
	size = sizeof(rtk_bt_gatt_attr_t) * attr_count;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->attrs, size);
	end += ALIGN_UP(size, 4);

	for (i = 0; i < attr_count; i++) {
		buf_ptr->attrs[i].uuid = (const struct bt_uuid *)end;
		/* memory space for struct bt_uuid */
		size = get_struct_bt_uuid_size((struct bt_uuid *)param->attrs[i].uuid);

		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->attrs[i].uuid, size);
		end += ALIGN_UP(size, 4);

		/* memory space for void *user_data */
		if (param->attrs[i].user_data) {
			buf_ptr->attrs[i].user_data = end;
			size =  get_struct_attr_data_size(&param->attrs[i]);

			if (end + size > buf + size_limit) {
				goto exit;
			}
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

		if (end + size > buf + size_limit) {
			goto exit;
		}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
										IPC_HOST_API_DATA_MAX, pactual_size)) {
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
	uint32_t handle_cout = 0;

	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		handle_cout = param->multiple.handle_count;
		/* memory space for struct rtk_bt_gattc_read_param_t */
		size = sizeof(rtk_bt_gattc_read_param_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		/* parsing handles */
		p_read_param->multiple.handles = (uint16_t *)end;
		if (end + handle_cout * 2 > buf + size_limit) {
			goto exit;
		}
		memcpy((void *)p_read_param->multiple.handles, (void *)param->multiple.handles, handle_cout * 2);
		p_read_param->multiple.handles = (uint16_t *)(end - buf);
		*actual_size = (uint32_t)((end + handle_cout * 2) - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		handle_cout = param->multiple_variable.handle_count;
		/* memory space for struct rtk_bt_gattc_read_param_t */
		size = sizeof(rtk_bt_gattc_read_param_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		/* parsing handles */
		p_read_param->multiple_variable.handles = (uint16_t *)end;
		if (end + handle_cout * 2 > buf + size_limit) {
			goto exit;
		}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_write_param->data = (void *)end;
	if (end + param->length > buf + size_limit) {
		goto exit;
	}
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

/* ===============================================rtk_bt_mesh ipc action======================================= */
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
static struct act_mem_option rtk_bt_mesh_stack_act_mem_op_tab[] = {
	{RTK_BT_MESH_STACK_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_config_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_CONFIG_MODEL_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_onoff_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_datatrans_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_DATATRANS_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_remote_prov_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_light_lightness_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_light_ctl_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_light_hsl_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_light_xyl_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_light_lc_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_LIGHT_LC_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_time_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_TIME_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_scheduler_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_scene_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_SCENE_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_default_transition_time_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_level_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_power_on_off_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_power_level_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_battery_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_location_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_generic_property_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

static struct act_mem_option rtk_bt_mesh_sensor_client_model_act_mem_op_tab[] = {
	{RTK_BT_MESH_SENSOR_CLIENT_MODEL_ACT_MAX, RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE},
};

uint32_t rtk_mesh_get_act_ret_param_size(uint32_t group, uint16_t act)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	struct act_mem_option *p_act_op_table = NULL;
	switch (group) {
	case RTK_BT_IPC_MESH_STACK:
		p_act_op_table = rtk_bt_mesh_stack_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_CONFIG_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_config_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_ONOFF_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_onoff_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_DATATRANS_MODEL:
		p_act_op_table = rtk_bt_mesh_datatrans_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_REMOTE_PROV_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_remote_prov_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_light_lightness_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_LIGHT_CTL_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_light_ctl_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_LIGHT_HSL_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_light_hsl_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_LIGHT_XYL_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_light_xyl_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_LIGHT_LC_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_light_lc_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_TIME_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_time_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_SCHEDULER_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_scheduler_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_SCENE_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_scene_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_default_transition_time_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_LEVEL_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_level_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_POWER_ON_OFF_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_power_on_off_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_power_level_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_BATTERY_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_battery_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_LOCATION_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_location_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_GENERIC_PROPERTY_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_generic_property_client_model_act_mem_op_tab;
		break;
	case RTK_BT_IPC_MESH_SENSOR_CLIENT_MODEL:
		p_act_op_table = rtk_bt_mesh_sensor_client_model_act_mem_op_tab;
		break;
	default:
		printf("[%s] Unknown group %d\r\n", __func__, (int)group);
		break;
	}
	/* foreach event table */
	if (p_act_op_table) {
		while (1) {
			if (p_act_op_table[i].act == act) {
				max_size = p_act_op_table[i].size;
				break;
			}
			if (p_act_op_table[i].size == RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE) {
				//printf("%s: cannot find matched act %d \r\n", __func__, act);
				break;
			}
			i++;
		}
	}

	return max_size;
}

uint16_t rtk_mesh_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	(void)act;
	(void)dst;
	(void)src;
	return RTK_BT_OK;
}

/* In event param, some 2nd level ptr is used to put returned data, the 2nd ptr from
another cpu core is useless, we need a space to put the returned param data.
So, alloc mem for 2nd ptr of event param, the allocated mem is in field 'ret'
of ipc buf, actually, it just change the 2nd ptr to bt_ipc_dev_request_message::ret[1] */

void bt_mesh_generic_onoff_server_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_GET: {
		rtk_bt_mesh_generic_onoff_server_evt_get_t *onoff_get =
			(rtk_bt_mesh_generic_onoff_server_evt_get_t *)ipc_dev_msg->param_buf;
		onoff_get->on_off = (uint8_t *)&ipc_dev_msg->ret[1];
		if (sizeof(rtk_bt_mesh_generic_on_off_t) >= (IPC_DEV_API_RET_MAX - 4)) {
			printf("[%s]: Error, event param returned data is over IPC_DEV_API_RET_MAX-4:%d\r\n", __func__, IPC_DEV_API_RET_MAX - 4);
			return;
		}
		break;
	}
	default:
		break;
	}
}

void bt_mesh_datatrans_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ: {
		rtk_bt_mesh_datatrans_server_read_event_t *server_read =
			(rtk_bt_mesh_datatrans_server_read_event_t *)ipc_dev_msg->param_buf;
		server_read->data = (uint8_t *)&ipc_dev_msg->ret[1];
		if (server_read->data_len >= (IPC_DEV_API_RET_MAX - 4)) {
			printf("[%s]: Error, event param returned data is over IPC_DEV_API_RET_MAX\r\n", __func__);
			return;
		}
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_lightness_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;
	// printf("enter bt_mesh_light_lightness_model_alloc_mem_for_event_param \r\n");
	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_lightness_server_direct_get_t *lightness_get =
			(rtk_bt_mesh_light_lightness_server_direct_get_t *)ipc_dev_msg->param_buf;
		lightness_get->value = (rtk_bt_mesh_light_lightness_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_lightness_server_direct_get_t *linear_get =
			(rtk_bt_mesh_light_lightness_server_direct_get_t *)ipc_dev_msg->param_buf;
		linear_get->value = (rtk_bt_mesh_light_lightness_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_lightness_server_direct_get_t *default_get =
			(rtk_bt_mesh_light_lightness_server_direct_get_t *)ipc_dev_msg->param_buf;
		default_get->value = (rtk_bt_mesh_light_lightness_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_lightness_server_direct_get_t *last_get =
			(rtk_bt_mesh_light_lightness_server_direct_get_t *)ipc_dev_msg->param_buf;
		last_get->value = (rtk_bt_mesh_light_lightness_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_lightness_server_direct_get_range_t *range_get =
			(rtk_bt_mesh_light_lightness_server_direct_get_range_t *)ipc_dev_msg->param_buf;
		range_get->value = (rtk_bt_mesh_light_lightness_server_get_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_ctl_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_ctl_server_direct_get_t *lightness_ctl_get =
			(rtk_bt_mesh_light_ctl_server_direct_get_t *)ipc_dev_msg->param_buf;
		lightness_ctl_get->value = (rtk_bt_mesh_light_ctl_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_DEFAULT_GET: {
		rtk_bt_mesh_light_ctl_server_direct_get_default_t *default_ctl_get =
			(rtk_bt_mesh_light_ctl_server_direct_get_default_t *)ipc_dev_msg->param_buf;
		default_ctl_get->value = (rtk_bt_mesh_light_ctl_server_get_default_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET: {
		rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *range_ctl_get =
			(rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *)ipc_dev_msg->param_buf;
		range_ctl_get->value = (rtk_bt_mesh_light_ctl_server_get_temperature_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_ctl_temperature_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_GET: {
		rtk_bt_mesh_light_ctl_server_direct_get_temperature_t *ctl_temperature_get =
			(rtk_bt_mesh_light_ctl_server_direct_get_temperature_t *)ipc_dev_msg->param_buf;
		ctl_temperature_get->value = (rtk_bt_mesh_light_ctl_server_get_temperature_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET_T: {
		rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *range_temperature_get =
			(rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *)ipc_dev_msg->param_buf;
		range_temperature_get->value = (rtk_bt_mesh_light_ctl_server_get_temperature_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_hsl_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_hsl_server_direct_get_t *hsl_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_t *)ipc_dev_msg->param_buf;
		hsl_get->value = (rtk_bt_mesh_light_hsl_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_DEFAULT_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_hsl_server_direct_get_default_t *default_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_default_t *)ipc_dev_msg->param_buf;
		default_get->value = (rtk_bt_mesh_light_hsl_server_get_default_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_hsl_server_direct_get_range_t *range_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_range_t *)ipc_dev_msg->param_buf;
		range_get->value = (rtk_bt_mesh_light_hsl_server_get_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_hsl_hue_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_hsl_server_direct_get_hue_t *hue_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_hue_t *)ipc_dev_msg->param_buf;
		hue_get->value = (rtk_bt_mesh_light_hsl_server_get_hue_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_H: {
		rtk_bt_mesh_light_hsl_server_direct_get_range_t *hue_range_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_range_t *)ipc_dev_msg->param_buf;
		hue_range_get->value = (rtk_bt_mesh_light_hsl_server_get_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_hsl_saturation_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_hsl_server_direct_get_saturation_t *hue_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_saturation_t *)ipc_dev_msg->param_buf;
		hue_get->value = (rtk_bt_mesh_light_hsl_server_get_saturation_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_S: {
		rtk_bt_mesh_light_hsl_server_direct_get_range_t *saturation_range_get =
			(rtk_bt_mesh_light_hsl_server_direct_get_range_t *)ipc_dev_msg->param_buf;
		saturation_range_get->value = (rtk_bt_mesh_light_hsl_server_get_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_xyl_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_xyl_server_direct_get_t *xyl_get =
			(rtk_bt_mesh_light_xyl_server_direct_get_t *)ipc_dev_msg->param_buf;
		xyl_get->value = (rtk_bt_mesh_light_xyl_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_DEFAULT_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_xyl_server_direct_get_default_t *default_get =
			(rtk_bt_mesh_light_xyl_server_direct_get_default_t *)ipc_dev_msg->param_buf;
		default_get->value = (rtk_bt_mesh_light_xyl_server_get_default_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_RANGE_SERVER_MODEL_GET: {
		rtk_bt_mesh_light_xyl_server_direct_get_range_t *range_get =
			(rtk_bt_mesh_light_xyl_server_direct_get_range_t *)ipc_dev_msg->param_buf;
		range_get->value = (rtk_bt_mesh_light_xyl_server_get_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_lc_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_MODE_GET: {
		rtk_bt_mesh_light_lc_server_direct_get_mode_t *mode_get =
			(rtk_bt_mesh_light_lc_server_direct_get_mode_t *)ipc_dev_msg->param_buf;
		mode_get->value = (rtk_bt_mesh_light_lc_server_get_mode_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_OM_GET: {
		rtk_bt_mesh_light_lc_server_direct_get_om_t *om_get =
			(rtk_bt_mesh_light_lc_server_direct_get_om_t *)ipc_dev_msg->param_buf;
		om_get->value = (rtk_bt_mesh_light_lc_server_get_om_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_LIGHT_ON_OFF_GET: {
		rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t *onoff_get =
			(rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t *)ipc_dev_msg->param_buf;
		onoff_get->value = (rtk_bt_mesh_light_lc_server_get_light_on_off_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_SM_TRANSITION_TIME_GET: {
		rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t *sm_get =
			(rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t *)ipc_dev_msg->param_buf;
		sm_get->value = (rtk_bt_mesh_light_lc_server_get_sm_transition_time_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_light_lc_setup_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_PROPERTY_GET: {
		rtk_bt_mesh_light_lc_server_direct_get_property_t *property_get =
			(rtk_bt_mesh_light_lc_server_direct_get_property_t *)ipc_dev_msg->param_buf;
		property_get->value = (rtk_bt_mesh_light_lc_server_direct_get_property_para_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_time_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_TIME_SERVER_MODEL_GET: {
		rtk_bt_mesh_time_server_direct_get_t *time_get =
			(rtk_bt_mesh_time_server_direct_get_t *)ipc_dev_msg->param_buf;
		time_get->value = (rtk_bt_mesh_time_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_TIME_SERVER_MODEL_ZONE_GET: {
		rtk_bt_mesh_time_server_direct_get_zone_t *zone_get =
			(rtk_bt_mesh_time_server_direct_get_zone_t *)ipc_dev_msg->param_buf;
		zone_get->value = (rtk_bt_mesh_time_server_get_zone_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_TIME_SERVER_MODEL_TAI_UTC_DELTA_GET: {
		rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *delta_get =
			(rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *)ipc_dev_msg->param_buf;
		delta_get->value = (rtk_bt_mesh_time_server_get_tai_utc_delta_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_time_setup_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ROLE_GET: {
		rtk_bt_mesh_time_server_get_role_t *role_get =
			(rtk_bt_mesh_time_server_get_role_t *)ipc_dev_msg->param_buf;
		role_get->role = (rtk_bt_mesh_time_role_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ZONE_GET: {
		rtk_bt_mesh_time_server_direct_get_zone_t *zone_get =
			(rtk_bt_mesh_time_server_direct_get_zone_t *)ipc_dev_msg->param_buf;
		zone_get->value = (rtk_bt_mesh_time_server_get_zone_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_TAI_UTC_DELTA_GET: {
		rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *delta_get =
			(rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *)ipc_dev_msg->param_buf;
		delta_get->value = (rtk_bt_mesh_time_server_get_tai_utc_delta_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_scheduler_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_SCHEDULER_SERVER_MODEL_GET: {
		rtk_bt_mesh_scheduler_server_get_t *scheduler_get =
			(rtk_bt_mesh_scheduler_server_get_t *)ipc_dev_msg->param_buf;
		scheduler_get->schedulers = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCHEDULER_ACTION_SERVER_MODEL_GET: {
		rtk_bt_mesh_scheduler_server_get_action_t *action_get =
			(rtk_bt_mesh_scheduler_server_get_action_t *)ipc_dev_msg->param_buf;
		action_get->scheduler = (rtk_bt_mesh_scheduler_register_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_scheduler_setup_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_SCHEDULER_SETUP_SERVER_MODEL_GET_ACTION: {
		rtk_bt_mesh_scheduler_server_get_action_t *action_get =
			(rtk_bt_mesh_scheduler_server_get_action_t *)ipc_dev_msg->param_buf;
		action_get->scheduler = (rtk_bt_mesh_scheduler_register_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_scene_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_SCENE_SERVER_MODEL_GET: {
		rtk_bt_mesh_scene_server_get_t *scene_get =
			(rtk_bt_mesh_scene_server_get_t *)ipc_dev_msg->param_buf;
		scene_get->current_scene = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_REGISTER_STATUS: {
		rtk_bt_mesh_scene_server_get_register_status_t *status_get =
			(rtk_bt_mesh_scene_server_get_register_status_t *)ipc_dev_msg->param_buf;
		status_get->status = (rtk_bt_mesh_scene_status_code_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES: {
		rtk_bt_mesh_scene_server_get_scenes_t *scenes_get =
			(rtk_bt_mesh_scene_server_get_scenes_t *)ipc_dev_msg->param_buf;
		scenes_get->scenes = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES_NUM: {
		rtk_bt_mesh_scene_server_get_scenes_num_t *num_get =
			(rtk_bt_mesh_scene_server_get_scenes_num_t *)ipc_dev_msg->param_buf;
		num_get->num_scenes = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_STATUS_RECALL: {
		rtk_bt_mesh_scene_server_get_status_recall_t *recall_status_get =
			(rtk_bt_mesh_scene_server_get_status_recall_t *)ipc_dev_msg->param_buf;
		recall_status_get->status_recall = (rtk_bt_mesh_scene_status_code_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_scene_setup_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_STORE: {
		rtk_bt_mesh_scene_server_store_t *scene_store =
			(rtk_bt_mesh_scene_server_store_t *)ipc_dev_msg->param_buf;
		scene_store->pmemory = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET: {
		rtk_bt_mesh_scene_server_get_t *scene_setup_get =
			(rtk_bt_mesh_scene_server_get_t *)ipc_dev_msg->param_buf;
		scene_setup_get->current_scene = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES: {
		rtk_bt_mesh_scene_server_get_scenes_t *scenes_setup_get =
			(rtk_bt_mesh_scene_server_get_scenes_t *)ipc_dev_msg->param_buf;
		scenes_setup_get->scenes = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES_NUM: {
		rtk_bt_mesh_scene_server_get_scenes_num_t *scenes_num_get =
			(rtk_bt_mesh_scene_server_get_scenes_num_t *)ipc_dev_msg->param_buf;
		scenes_num_get->num_scenes = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_STATUS_REGISTER: {
		rtk_bt_mesh_scene_server_get_register_status_t *status_register_get =
			(rtk_bt_mesh_scene_server_get_register_status_t *)ipc_dev_msg->param_buf;
		status_register_get->status = (rtk_bt_mesh_scene_status_code_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_default_transition_time_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_GET: {
		rtk_bt_mesh_generic_default_transition_time_server_get_t *time_get =
			(rtk_bt_mesh_generic_default_transition_time_server_get_t *)ipc_dev_msg->param_buf;
		time_get->trans_time = (rtk_bt_mesh_generic_transition_time_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_level_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_GET: {
		rtk_bt_mesh_generic_level_server_get_t *level_get =
			(rtk_bt_mesh_generic_level_server_get_t *)ipc_dev_msg->param_buf;
		level_get->level = (int16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_power_on_off_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_GET: {
		rtk_bt_mesh_generic_power_on_off_server_get_t *power_get =
			(rtk_bt_mesh_generic_power_on_off_server_get_t *)ipc_dev_msg->param_buf;
		power_get->on_power_up = (rtk_bt_mesh_generic_on_power_up_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_power_level_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_GET: {
		rtk_bt_mesh_generic_power_level_server_get_t *power_get =
			(rtk_bt_mesh_generic_power_level_server_get_t *)ipc_dev_msg->param_buf;
		power_get->power = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_LAST_GET: {
		rtk_bt_mesh_generic_power_level_server_get_t *last_get =
			(rtk_bt_mesh_generic_power_level_server_get_t *)ipc_dev_msg->param_buf;
		last_get->power = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_DEFAULT_GET: {
		rtk_bt_mesh_generic_power_level_server_get_t *default_get =
			(rtk_bt_mesh_generic_power_level_server_get_t *)ipc_dev_msg->param_buf;
		default_get->power = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_RANGE_GET: {
		rtk_bt_mesh_generic_power_level_server_direct_get_range_t *range_get =
			(rtk_bt_mesh_generic_power_level_server_direct_get_range_t *)ipc_dev_msg->param_buf;
		range_get->value = (rtk_bt_mesh_generic_power_level_server_get_range_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_battery_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_BATTERY_SERVER_MODEL_GET: {
		rtk_bt_mesh_generic_battery_server_direct_get_t *battery_get =
			(rtk_bt_mesh_generic_battery_server_direct_get_t *)ipc_dev_msg->param_buf;
		battery_get->value = (rtk_bt_mesh_generic_battery_server_get_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_location_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_GLOBAL_GET: {
		rtk_bt_mesh_generic_location_server_direct_get_global_t *global_get =
			(rtk_bt_mesh_generic_location_server_direct_get_global_t *)ipc_dev_msg->param_buf;
		global_get->value = (rtk_bt_mesh_generic_location_server_get_global_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_LOCAL_GET: {
		rtk_bt_mesh_generic_location_server_direct_get_local_t *local_get =
			(rtk_bt_mesh_generic_location_server_direct_get_local_t *)ipc_dev_msg->param_buf;
		local_get->value = (rtk_bt_mesh_generic_location_server_get_local_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_location_setup_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_GLOBAL_GET: {
		rtk_bt_mesh_generic_location_server_direct_get_global_t *global_get =
			(rtk_bt_mesh_generic_location_server_direct_get_global_t *)ipc_dev_msg->param_buf;
		global_get->value = (rtk_bt_mesh_generic_location_server_get_global_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_LOCAL_GET: {
		rtk_bt_mesh_generic_location_server_direct_get_local_t *local_get =
			(rtk_bt_mesh_generic_location_server_direct_get_local_t *)ipc_dev_msg->param_buf;
		local_get->value = (rtk_bt_mesh_generic_location_server_get_local_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_user_property_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_PARAMETER_GET: {
		rtk_bt_mesh_generic_property_server_para_get_t *user_para_get =
			(rtk_bt_mesh_generic_property_server_para_get_t *)ipc_dev_msg->param_buf;
		user_para_get->pvalue = (rtk_bt_mesh_generic_property_server_para_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_VALUE_GET: {
		rtk_bt_mesh_generic_property_server_value_get_t *user_val_get =
			(rtk_bt_mesh_generic_property_server_value_get_t *)ipc_dev_msg->param_buf;
		user_val_get->pproperty_value = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_NUM_IDS_GET: {
		rtk_bt_mesh_generic_property_server_num_ids_get_t *num_ids_get =
			(rtk_bt_mesh_generic_property_server_num_ids_get_t *)ipc_dev_msg->param_buf;
		num_ids_get->pvalue = (rtk_bt_mesh_generic_property_server_num_ids_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_IDS_VALUE_GET: {
		rtk_bt_mesh_generic_property_server_num_ids_value_get_t *ids_value_get =
			(rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)ipc_dev_msg->param_buf;
		ids_value_get->property_ids = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_admin_property_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_PARAMETER_GET: {
		rtk_bt_mesh_generic_property_server_para_get_t *admin_para_get =
			(rtk_bt_mesh_generic_property_server_para_get_t *)ipc_dev_msg->param_buf;
		admin_para_get->pvalue = (rtk_bt_mesh_generic_property_server_para_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_VALUE_GET: {
		rtk_bt_mesh_generic_property_server_value_get_t *admin_val_get =
			(rtk_bt_mesh_generic_property_server_value_get_t *)ipc_dev_msg->param_buf;
		admin_val_get->pproperty_value = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_NUM_IDS_GET: {
		rtk_bt_mesh_generic_property_server_num_ids_get_t *num_ids_get =
			(rtk_bt_mesh_generic_property_server_num_ids_get_t *)ipc_dev_msg->param_buf;
		num_ids_get->pvalue = (rtk_bt_mesh_generic_property_server_num_ids_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_IDS_VALUE_GET: {
		rtk_bt_mesh_generic_property_server_num_ids_value_get_t *ids_value_get =
			(rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)ipc_dev_msg->param_buf;
		ids_value_get->property_ids = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_manu_property_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_PARAMETER_GET: {
		rtk_bt_mesh_generic_property_server_para_get_t *manu_para_get =
			(rtk_bt_mesh_generic_property_server_para_get_t *)ipc_dev_msg->param_buf;
		manu_para_get->pvalue = (rtk_bt_mesh_generic_property_server_para_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_VALUE_GET: {
		rtk_bt_mesh_generic_property_server_value_get_t *manu_val_get =
			(rtk_bt_mesh_generic_property_server_value_get_t *)ipc_dev_msg->param_buf;
		manu_val_get->pproperty_value = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_NUM_IDS_GET: {
		rtk_bt_mesh_generic_property_server_num_ids_get_t *num_ids_get =
			(rtk_bt_mesh_generic_property_server_num_ids_get_t *)ipc_dev_msg->param_buf;
		num_ids_get->pvalue = (rtk_bt_mesh_generic_property_server_num_ids_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_IDS_VALUE_GET: {
		rtk_bt_mesh_generic_property_server_num_ids_value_get_t *ids_value_get =
			(rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)ipc_dev_msg->param_buf;
		ids_value_get->property_ids = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_generic_client_property_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL_PROPERTY_GET: {
		rtk_bt_mesh_generic_client_property_server_direct_get_t *client_para_get =
			(rtk_bt_mesh_generic_client_property_server_direct_get_t *)ipc_dev_msg->param_buf;
		client_para_get->value = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_sensor_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_GET: {
		rtk_bt_mesh_sensor_server_get_t *sensor_get =
			(rtk_bt_mesh_sensor_server_get_t *)ipc_dev_msg->param_buf;
		sensor_get->raw_data = (void *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_COLUMN_GET: {
		rtk_bt_mesh_sensor_server_direct_get_column_t *column_get =
			(rtk_bt_mesh_sensor_server_direct_get_column_t *)ipc_dev_msg->param_buf;
		column_get->value = (void *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_SERIES_GET: {
		rtk_bt_mesh_sensor_server_direct_get_series_t *series_get =
			(rtk_bt_mesh_sensor_server_direct_get_series_t *)ipc_dev_msg->param_buf;
		series_get->value = (void *)&ipc_dev_msg->ret[1];
		break;
	}
    case RTK_BT_MESH_SENSOR_SERVER_MODEL_COMPARE_CADENCE: {
		rtk_bt_mesh_sensor_server_compare_cadence_t *cadence_get =
			(rtk_bt_mesh_sensor_server_compare_cadence_t *)ipc_dev_msg->param_buf;
		cadence_get->need_fast_divisor = (bool *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_NUM_GET: {
		rtk_bt_mesh_sensor_server_get_descriptor_num_t *des_num_get =
			(rtk_bt_mesh_sensor_server_get_descriptor_num_t *)ipc_dev_msg->param_buf;
		des_num_get->descriptor_num = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_GET: {
		rtk_bt_mesh_sensor_server_get_descriptor_t *des_get =
			(rtk_bt_mesh_sensor_server_get_descriptor_t *)ipc_dev_msg->param_buf;
		des_get->descriptor = (rtk_bt_mesh_sensor_descriptor_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

void bt_mesh_sensor_setup_model_alloc_mem_for_event_param(uint8_t evt_code, void *ipc_msg)
{
	bt_ipc_dev_request_message *ipc_dev_msg = (bt_ipc_dev_request_message *)ipc_msg;

	switch (evt_code) {
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_CADENCE_GET: {
		rtk_bt_mesh_sensor_server_get_cadence_t *cadence_get =
			(rtk_bt_mesh_sensor_server_get_cadence_t *)ipc_dev_msg->param_buf;
		cadence_get->cadence = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTINGS_GET: {
		rtk_bt_mesh_sensor_server_get_settings_t *settings_get =
			(rtk_bt_mesh_sensor_server_get_settings_t *)ipc_dev_msg->param_buf;
		settings_get->settings_data = (uint16_t *)&ipc_dev_msg->ret[1];
		break;
	}
	case RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTING_GET: {
		rtk_bt_mesh_sensor_server_get_setting_t *setting_get =
			(rtk_bt_mesh_sensor_server_get_setting_t *)ipc_dev_msg->param_buf;
		setting_get->setting_data = (uint8_t *)&ipc_dev_msg->ret[1];
		break;
	}
	default:
		break;
	}
}

#endif

/* ===============================================rtk_bt_br_gap ipc action======================================= */
//static struct act_mem_option rtk_bt_br_gap_act_mem_op_tab[] = {
//	{RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR, sizeof(rtk_bt_br_bd_addr_t)},
//	{RTK_BT_BR_GAP_ACT_MAX_BOND_NUM_GET, sizeof(uint8_t)},
//	{RTK_BT_BR_GAP_ACT_BOND_NUM_GET, sizeof(uint8_t)},
//	{RTK_BT_BR_GAP_ACT_BOND_ADDR_GET, 6},
//	{RTK_BT_BR_GAP_ACT_BOND_INDEX_GET, sizeof(uint8_t)},
//	{RTK_BT_BR_GAP_ACT_BOND_KEY_GET, sizeof(rtk_bt_br_bond_key_t)},
//	{RTK_BT_BR_GAP_ACT_MAX, 0}
//};

uint32_t rtk_br_get_act_ret_param_size(uint16_t act)
{
//	uint8_t i = 0;
//	uint32_t max_size = 0;
//	/* foreach event table */
//	while (1) {
//		if (rtk_bt_br_gap_act_mem_op_tab[i].act == act) {
//			max_size = rtk_bt_br_gap_act_mem_op_tab[i].size;
//			break;
//		}
//		if (rtk_bt_br_gap_act_mem_op_tab[i].act == RTK_BT_BR_GAP_ACT_MAX) {
//			//printf("%s: cannot find matched act %d \r\n", __func__, act);
//			break;
//		}
//		i++;
//	}
//
//	return max_size;
}

uint16_t rtk_br_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
//	if (act == RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR) {
//		memcpy((void *)dst, (void *)src, rtk_br_get_act_ret_param_size(act));
//	} else if (act == RTK_BT_BR_GAP_ACT_MAX_BOND_NUM_GET ||
//			   act == RTK_BT_BR_GAP_ACT_BOND_NUM_GET) {
//		memcpy((void *)dst, (void *)src, rtk_br_get_act_ret_param_size(act));
//	} else if (act == RTK_BT_BR_GAP_ACT_BOND_ADDR_GET) {
//		rtk_bt_br_bond_addr_t *pbond_addr_t = (rtk_bt_br_bond_addr_t *)dst;
//		memcpy((void *)pbond_addr_t->bd_addr, (void *)src, rtk_br_get_act_ret_param_size(act));
//	} else if (act == RTK_BT_BR_GAP_ACT_BOND_INDEX_GET) {
//		rtk_bt_br_bond_index_t *pbond_index_t = (rtk_bt_br_bond_index_t *)dst;
//		memcpy((void *)&pbond_index_t->index, (void *)src, rtk_br_get_act_ret_param_size(act));
//	} else if (act == RTK_BT_BR_GAP_ACT_BOND_KEY_GET) {
//		memcpy((void *)dst, (void *)src, rtk_br_get_act_ret_param_size(act));
//	}
//
//	return RTK_BT_OK;
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_data_send_param->frame_buf = (void *)end;
	if (end + param->len > buf + size_limit) {
		goto exit;
	}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_data_send_param->data = (void *)end;
	if (end + param->len > buf + size_limit) {
		goto exit;
	}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	/* parsing data */
	p_data_send_param->buf = (void *)end;
	if (end + param->len > buf + size_limit) {
		goto exit;
	}
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
#if defined(RTK_BLE_ISO_SUPPORT) && RTK_BLE_ISO_SUPPORT
static struct act_mem_option rtk_bt_le_iso_act_mem_op_tab[] = {
#if defined(RTK_BLE_ISO_CIS_SUPPORT) && RTK_BLE_ISO_CIS_SUPPORT
	{RTK_BT_LE_ISO_ACT_CIG_READ_ISO_TX_SYNC, sizeof(rtk_bt_le_iso_cig_read_iso_tx_sync_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_READ_LINK_QUALITY, sizeof(rtk_bt_le_iso_cig_read_link_quality_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_CONN_HANDLE, sizeof(uint16_t)},
	{RTK_BT_LE_ISO_ACT_CIG_GET_CONN_HANDLE, sizeof(uint16_t)},
	{RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO, sizeof(rtk_bt_le_iso_cig_cis_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO, sizeof(rtk_bt_le_iso_cis_channel_info_t)},
	{RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE, sizeof(uint16_t)},
#endif
#if defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT
	{RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE, sizeof(uint8_t)},
	{RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_READ_TX_SYNC, sizeof(rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t)},
	{RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC, sizeof(uint8_t)},
	{RTK_BT_LE_ISO_ACT_BIG_RECEIVER_READ_LINK_QUALITY, sizeof(rtk_bt_le_iso_big_receiver_read_link_quality_info_t)},
#endif
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
#if defined(RTK_BLE_ISO_CIS_SUPPORT) && RTK_BLE_ISO_CIS_SUPPORT
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
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_GET_CONN_HANDLE) {
		rtk_bt_le_iso_cig_get_conn_handle_param_t *param = (rtk_bt_le_iso_cig_get_conn_handle_param_t *)dst;
		memcpy((void *)param->p_conn_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO) {
		rtk_bt_le_iso_cig_get_cis_info_param_t *param = (rtk_bt_le_iso_cig_get_cis_info_param_t *)dst;
		memcpy((void *)param->p_cis_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO) {
		rtk_bt_le_iso_cig_get_isoch_info_param_t *param = (rtk_bt_le_iso_cig_get_isoch_info_param_t *)dst;
		memcpy((void *)param->p_isoch_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE) {
		rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t *param = (rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t *)dst;
		memcpy((void *)param->p_cis_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	}
#endif
#if defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT
	if (act == RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE) {
		rtk_bt_le_iso_big_broadcaster_create_param_t *param = (rtk_bt_le_iso_big_broadcaster_create_param_t *)dst;
		memcpy((void *)param->big_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_READ_TX_SYNC) {
		rtk_bt_le_iso_big_broadcaster_read_tx_sync_t *param = (rtk_bt_le_iso_big_broadcaster_read_tx_sync_t *)dst;
		memcpy((void *)param->p_tx_sync_info, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC) {
		rtk_bt_le_iso_big_receiver_create_sync_t *param = (rtk_bt_le_iso_big_receiver_create_sync_t *)dst;
		memcpy((void *)param->p_big_handle, (void *)src, rtk_le_iso_get_act_ret_param_size(act));
	}
#endif

	return RTK_BT_OK;
}

static int le_ipc_iso_setup_path_push(uint8_t *buf, rtk_bt_le_iso_setup_path_param_t *param,
									  uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_setup_path_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->codec_config_len;

	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_codec_config, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_setup_path_param_t *)buf)->p_codec_config = (uint8_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);

	return 0;

exit:
	printf("rtk_bt_le_iso_setup_path_param_t size is over deep copy buf\r\n");
	return -1;
}
#if defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT
static int le_ipc_iso_big_broadcaster_create_push(uint8_t *buf, rtk_bt_le_iso_big_broadcaster_create_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_big_broadcaster_create_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = sizeof(rtk_bt_le_iso_big_broadcaster_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = sizeof(rtk_bt_le_iso_big_receiver_create_sync_param_t);

	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_sync_param, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_le_iso_big_receiver_create_sync_t *)buf)->p_sync_param = (rtk_bt_le_iso_big_receiver_create_sync_param_t *)(border[0] - buf);
	*actual_size = (uint32_t)(end - buf);

	return 0;

exit:
	printf("rtk_bt_le_iso_big_receiver_create_sync_t size is over deep copy buf\r\n");
	return -1;
}
#endif
static int le_ipc_iso_data_send_push(uint8_t *buf, rtk_bt_le_iso_data_send_info_t *param,
									 uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_data_send_info_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->data_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
#if defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT
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
#endif
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
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
static struct act_mem_option rtk_bt_le_audio_act_mem_op_tab[] = {
	{RTK_BT_LE_AUDIO_ACT_GET_PREFER_CODEC_CFG, sizeof(rtk_bt_le_audio_cfg_codec_t)},
	{RTK_BT_LE_AUDIO_ACT_PACS_GET_INFO, sizeof(rtk_bt_le_audio_bap_pacs_info_t)},
	{RTK_BT_LE_AUDIO_ACT_PACS_GET_LC3_TABLE_MASK, sizeof(uint32_t)},
	{RTK_BT_LE_AUDIO_ACT_PACS_GET_BIS_ARRAY_BY_SYNC_INFO, sizeof(uint32_t)},
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI, RTK_BT_LE_CSI_RSI_LEN},
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_GROUP, sizeof(rtk_bt_le_audio_group_handle_t) + sizeof(rtk_bt_le_audio_device_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_DEVICE, sizeof(rtk_bt_le_audio_device_handle_t)},
#endif
#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
	(defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)
	{RTK_BT_LE_AUDIO_ACT_SYNC_ALLOCATE, sizeof(rtk_bt_le_audio_sync_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_SYNC_FIND, sizeof(rtk_bt_le_audio_sync_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_SYNC_INFO, sizeof(rtk_bt_le_audio_sync_info_t)},
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG, sizeof(rtk_bt_le_audio_cfg_codec_t)},
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO, sizeof(rtk_bt_le_audio_bis_info_t)},
	{RTK_BT_LE_AUDIO_ACT_SYNC_GET_SUPPORT_BIS_ARRAY, sizeof(uint32_t)},
#endif
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE, sizeof(rtk_bt_le_audio_broadcast_source_create_ipc_param_t)},
	{RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO, sizeof(rtk_bt_le_audio_broadcast_source_info_t)},
	{RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH, sizeof(uint16_t)},
#endif
	{RTK_BT_LE_AUDIO_ACT_GROUP_ALLOCATE, sizeof(rtk_bt_le_audio_group_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_GROUP_ADD_DEVICE, sizeof(rtk_bt_le_audio_device_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE, sizeof(rtk_bt_le_audio_device_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE_BY_CONN_HANDLE, sizeof(rtk_bt_le_audio_device_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_NUM, sizeof(uint8_t)},
	{RTK_BT_LE_AUDIO_ACT_GROUP_GET_USED_DEVICE_NUM, sizeof(uint8_t)},
	{RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_INFO, sizeof(rtk_bt_le_audio_group_device_info_t)},
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_ALLOCATE, sizeof(rtk_bt_le_audio_stream_session_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_GET_GROUP_HANDLE, sizeof(rtk_bt_le_audio_group_handle_t)},
	{RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_INFO, sizeof(rtk_bt_le_audio_unicast_session_info_t)},
	{RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CFG_MASK, sizeof(uint32_t)},
	{RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CIS_INFO, sizeof(rtk_bt_le_audio_unicast_cis_infos_t)},
	{RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_QOS, sizeof(rtk_bt_le_audio_unicast_session_qos_t)},
	{RTK_BT_LE_AUDIO_ACT_UNICAST_GET_ASE_QOS, sizeof(rtk_bt_le_audio_unicast_ase_qos_t)},
#endif
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_VCS_GET_PARAM, sizeof(rtk_bt_le_audio_vcs_param_t)},
	{RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE, sizeof(rtk_bt_le_audio_vcs_volume_state_t)},
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_MICS_GET_MUTE_VALUE, sizeof(rtk_bt_le_audio_mics_mute_state_t)},
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
	{RTK_BT_LE_AUDIO_ACT_MICS_GET_PARAM, sizeof(rtk_bt_le_audio_mics_param_t)},
#endif
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
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
static void le_audio_broadcast_source_create_ret_param_pop(void *dst, void *src)
{
	uint16_t i = 0, j = 0, k = 0;
	rtk_bt_le_audio_broadcast_source_create_param_t *dst_param = (rtk_bt_le_audio_broadcast_source_create_param_t *)dst;
	rtk_bt_le_audio_broadcast_source_create_ipc_param_t *src_param = (rtk_bt_le_audio_broadcast_source_create_ipc_param_t *)src;
	*dst_param->p_broadcast_source_handle = src_param->broadcast_source_handle;
	for (i = 0 ; i < dst_param->num_groups; i++) {
		*dst_param->group[i].group_idx = src_param->group[i].group_idx;
		for (j = 0 ; j < dst_param->group[i].num_subgroups; j++) {
			*dst_param->group[i].subgroup[j].subgroup_idx = src_param->group[i].subgroup[j].subgroup_idx;
			for (k = 0 ; k < dst_param->group[i].subgroup[j].num_bis; k++) {
				*dst_param->group[i].subgroup[j].bis[k].bis_idx = src_param->group[i].subgroup[j].bis[k].bis_idx;
			}
		}
	}
}
#endif
#if 0
static uint16_t rtk_bt_device_le_audio_get_pac_record_ret_pop(void *dst, void *src)
{
	uint16_t size = 0, size_limit = 0;
	uint8_t *buf = (uint8_t *)src;
	uint8_t *end = buf;
	rtk_bt_le_audio_pacs_get_pac_record_param_t *dst_param = (rtk_bt_le_audio_pacs_get_pac_record_param_t *)dst;
	size_limit = dst_param->pac_tbl_max_len;

	//copy p_pac_num
	size = sizeof(uint8_t);
	if (end + size > buf + size_limit) {
		goto error;
	}
	memcpy((void *)&dst_param->pac_num, end, size);
	end += ALIGN_UP(size, 4);

	for (uint8_t i = 0; i < dst_param->pac_num; i++) {
		//copy p_pac_num
		size = sizeof(rtk_bt_le_audio_bap_pacs_pac_record_t);
		if (end + size > buf + size_limit) {
			goto error;
		}
		memcpy((void *)&dst_param->p_pac_tbl[i], end, size);
		end += ALIGN_UP(size, 4);

		//copy p_pac_tbl[i].p_metadata
		size = dst_param->p_pac_tbl[i].metadata_length;
		if (end + size > buf + size_limit) {
			goto error;
		}
		memcpy((void *)dst_param->p_pac_tbl[i].p_metadata, end, size);
		end += ALIGN_UP(size, 4);
	}

error:
	printf("%s, Error: exceed size_limit (%d)\r\n", __func__, size_limit);
	return RTK_BT_ERR_NO_MEMORY;
}
#endif
uint16_t rtk_le_audio_copy_act_ret_param(uint16_t act, void *dst, void *src)
{
	if (act == RTK_BT_LE_AUDIO_ACT_GET_PREFER_CODEC_CFG) {
		rtk_bt_le_audio_get_prefer_codec_cfg_param_t *param = (rtk_bt_le_audio_get_prefer_codec_cfg_param_t *)dst;
		memcpy((void *)param->p_cfg_codec, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_PACS_GET_INFO) {
		rtk_bt_le_audio_pacs_get_info_param_t *param = (rtk_bt_le_audio_pacs_get_info_param_t *)dst;
		memcpy((void *)param->p_pacs_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_PACS_GET_PAC_RECORD) {
		//return rtk_bt_device_le_audio_get_pac_record_ret_pop(dst,src);
		rtk_bt_le_audio_pacs_get_pac_record_param_t *param = (rtk_bt_le_audio_pacs_get_pac_record_param_t *)dst;
		memcpy((void *)param->p_pac_num, (void *)(uint8_t *)src, sizeof(uint8_t));
		memcpy((void *)param->p_pac_tbl, (void *)((uint8_t *)src + 1), (*param->p_pac_num) * sizeof(rtk_bt_le_audio_bap_pacs_pac_record_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_PACS_GET_LC3_TABLE_MASK) {
		rtk_bt_le_audio_pacs_get_lc3_table_mask_param_t *param = (rtk_bt_le_audio_pacs_get_lc3_table_mask_param_t *)dst;
		memcpy((void *)param->p_lc3_table_mask, (void *)(uint8_t *)src, sizeof(uint32_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_PACS_GET_BIS_ARRAY_BY_SYNC_INFO) {
		rtk_bt_le_audio_pacs_get_bis_array_by_sync_info_param_t *param = (rtk_bt_le_audio_pacs_get_bis_array_by_sync_info_param_t *)dst;
		memcpy((void *)param->p_bis_array, (void *)(uint8_t *)src, sizeof(uint32_t));
	}
#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || \
	(defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)
	else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_ALLOCATE) {
		rtk_bt_le_audio_sync_allocate_param_t *param = (rtk_bt_le_audio_sync_allocate_param_t *)dst;
		memcpy((void *)param->p_broadcast_sync_handle, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_FIND) {
		rtk_bt_le_audio_sync_find_param_t *param = (rtk_bt_le_audio_sync_find_param_t *)dst;
		memcpy((void *)param->p_broadcast_sync_handle, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_SYNC_INFO) {
		rtk_bt_le_audio_sync_get_sync_info_param_t *param = (rtk_bt_le_audio_sync_get_sync_info_param_t *)dst;
		memcpy((void *)param->p_sync_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_CODEC_CFG) {
		rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t *param = (rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t *)dst;
		memcpy((void *)param->p_codec_cfg, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_BIS_INFO) {
		rtk_bt_le_audio_sync_get_bis_info_param_t *param = (rtk_bt_le_audio_sync_get_bis_info_param_t *)dst;
		memcpy((void *)param->p_bis_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_SYNC_GET_SUPPORT_BIS_ARRAY) {
		rtk_bt_le_audio_sync_get_support_bis_array_param_t *param = (rtk_bt_le_audio_sync_get_support_bis_array_param_t *)dst;
		memcpy((void *)param->p_support_bis_array, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_GET_BRS_CHAR_DATA) {
		uint16_t ret_len = 0;
		bool is_get_bis_info = false;
		rtk_bt_le_audio_bass_brs_data_t *p_data_dst =
			(rtk_bt_le_audio_bass_brs_data_t *)((rtk_bt_le_audio_bass_client_get_brs_data_param_t *)dst)->p_brs_data;
		rtk_bt_le_audio_bass_brs_data_t *p_data_src = (rtk_bt_le_audio_bass_brs_data_t *)src;
		is_get_bis_info = ((rtk_bt_le_audio_bass_client_get_brs_data_param_t *)dst)->is_get_bis_info;
		if (is_get_bis_info && p_data_src->bis_info_size && p_data_dst->p_cp_bis_info) {
			for (uint8_t i = 0; i < p_data_src->num_subgroups; i++) {
				if (p_data_src->p_cp_bis_info[i].metadata_len && p_data_dst->p_cp_bis_info[i].p_metadata) {
					memcpy(p_data_dst->p_cp_bis_info[i].p_metadata, (uint8_t *)src + ret_len, p_data_src->p_cp_bis_info[i].metadata_len);
					ret_len += p_data_src->p_cp_bis_info[i].metadata_len;
					//change the p_metadata source ptr, ohterwise it will cover the dst ptr copy p_cp_bis_info
					p_data_src->p_cp_bis_info[i].p_metadata = p_data_dst->p_cp_bis_info[i].p_metadata;
				}
			}
			memcpy(p_data_dst->p_cp_bis_info, (uint8_t *)src + ret_len, p_data_src->bis_info_size);
			ret_len += p_data_src->bis_info_size;
			//change the p_cp_bis_info source ptr, ohterwise it will cover the dst ptr copy p_brs_data
			p_data_src->p_cp_bis_info = p_data_dst->p_cp_bis_info;
		}
		//copy content of p_brs_data
		memcpy(p_data_dst, src, sizeof(rtk_bt_le_audio_bass_brs_data_t));
	}
#endif
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE) {
		le_audio_broadcast_source_create_ret_param_pop(dst, src);
	} else if (act == RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_GET_INFO) {
		rtk_bt_le_audio_broadcast_source_info_get_param_t *param = (rtk_bt_le_audio_broadcast_source_info_get_param_t *)dst;
		memcpy((void *)param->p_info, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH) {
		rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *param = (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)dst;
		memcpy((void *)param->p_bis_conn_handle, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif
	else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_ALLOCATE) {
		memcpy((void *)dst, (void *)src, sizeof(rtk_bt_le_audio_group_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_ADD_DEVICE) {
		rtk_bt_le_audio_group_add_device_param_t *param = (rtk_bt_le_audio_group_add_device_param_t *)dst;
		memcpy((void *)param->p_device_handle, (void *)src, sizeof(rtk_bt_le_audio_device_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE) {
		rtk_bt_le_audio_group_find_device_param_t *param = (rtk_bt_le_audio_group_find_device_param_t *)dst;
		memcpy((void *)param->p_device_handle, (void *)src, sizeof(rtk_bt_le_audio_device_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_FIND_DEVICE_BY_CONN_HANDLE) {
		rtk_bt_le_audio_group_find_device_by_conn_handle_param_t *param = (rtk_bt_le_audio_group_find_device_by_conn_handle_param_t *)dst;
		memcpy((void *)param->p_device_handle, (void *)src, sizeof(rtk_bt_le_audio_device_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_NUM) {
		rtk_bt_le_audio_group_get_device_num_param_t *param = (rtk_bt_le_audio_group_get_device_num_param_t *)dst;
		memcpy((void *)param->p_device_num, (void *)src, sizeof(uint8_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_GET_USED_DEVICE_NUM) {
		rtk_bt_le_audio_group_get_used_device_num_param_t *param = (rtk_bt_le_audio_group_get_used_device_num_param_t *)dst;
		memcpy((void *)param->p_used_device_num, (void *)src, sizeof(uint8_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_GET_DEVICE_INFO) {
		rtk_bt_le_audio_group_get_device_info_param_t *param = (rtk_bt_le_audio_group_get_device_info_param_t *)dst;
		memcpy((void *)param->p_device_info, (void *)src, sizeof(rtk_bt_le_audio_group_device_info_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_GROUP_GET_ALL_DEVICE_INFO) {
		rtk_bt_le_audio_group_get_all_device_info_param_t *param = (rtk_bt_le_audio_group_get_all_device_info_param_t *)dst;
		memcpy((void *)param->p_device_num, (void *)(uint8_t *)src, sizeof(uint8_t));
		memcpy((void *)param->p_device_info_tbl, (void *)((uint8_t *)src + 1), (*param->p_device_num)*sizeof(rtk_bt_le_audio_group_device_info_t));
	}
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_ALLOCATE) {
		rtk_bt_le_audio_stream_session_allocate_param_t *param = (rtk_bt_le_audio_stream_session_allocate_param_t *)dst;
		memcpy((void *)param->p_stream_session_handle, (void *)src, sizeof(rtk_bt_le_audio_stream_session_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_STREAM_SESSION_GET_GROUP_HANDLE) {
		rtk_bt_le_audio_stream_session_get_group_handle_param_t *param = (rtk_bt_le_audio_stream_session_get_group_handle_param_t *)dst;
		memcpy((void *)param->p_group_handle, (void *)src, sizeof(rtk_bt_le_audio_group_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_INFO) {
		rtk_bt_le_audio_unicast_get_session_info_param_t *param = (rtk_bt_le_audio_unicast_get_session_info_param_t *)dst;
		memcpy((void *)param->p_session_info, (void *)src, sizeof(rtk_bt_le_audio_unicast_session_info_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CFG_MASK) {
		rtk_bt_le_audio_unicast_get_cfg_mask_param_t *param = (rtk_bt_le_audio_unicast_get_cfg_mask_param_t *)dst;
		memcpy((void *)param->p_cfg_mask, (void *)src, sizeof(uint32_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CIS_INFO) {
		rtk_bt_le_audio_unicast_get_cis_info_param_t *param = (rtk_bt_le_audio_unicast_get_cis_info_param_t *)dst;
		memcpy((void *)param->p_cis_infos->cis_info, (void *)src, sizeof(rtk_bt_le_audio_unicast_cis_infos_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_UNICAST_GET_SESSION_QOS) {
		rtk_bt_le_audio_unicast_get_session_qos_param_t *param = (rtk_bt_le_audio_unicast_get_session_qos_param_t *)dst;
		memcpy((void *)param->p_session_qos, (void *)src, sizeof(rtk_bt_le_audio_unicast_session_qos_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_UNICAST_GET_ASE_QOS) {
		rtk_bt_le_audio_unicast_get_ase_qos_param_t *param = (rtk_bt_le_audio_unicast_get_ase_qos_param_t *)dst;
		memcpy((void *)param->p_ase_qos, (void *)src, sizeof(rtk_bt_le_audio_unicast_ase_qos_t));
	}
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_CSIS_GEN_RSI) {
		rtk_bt_le_audio_csis_gen_rsi_info_t *param = (rtk_bt_le_audio_csis_gen_rsi_info_t *)dst;
		memcpy((void *)param->p_rsi, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_GROUP) {
		rtk_bt_le_audio_csis_set_coordinator_add_group_info_t *param = (rtk_bt_le_audio_csis_set_coordinator_add_group_info_t *)dst;
		memcpy((void *)param->p_group_handle, (void *)(uint8_t *)src, sizeof(rtk_bt_le_audio_group_handle_t));
		memcpy((void *)param->p_device_handle, (void *)((uint8_t *)src + 4), sizeof(rtk_bt_le_audio_device_handle_t));
	} else if (act == RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_ADD_DEVICE) {
		rtk_bt_le_audio_csis_set_coordinator_add_device_info_t *param = (rtk_bt_le_audio_csis_set_coordinator_add_device_info_t *)dst;
		memcpy((void *)param->p_device_handle, (void *)src, sizeof(rtk_bt_le_audio_device_handle_t));
	}
#endif
#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_VCS_GET_PARAM) {
		rtk_bt_le_audio_vcs_param_t *param = (rtk_bt_le_audio_vcs_param_t *)dst;
		memcpy((void *)param, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	} else if (act == RTK_BT_LE_AUDIO_ACT_VCS_GET_VOLUME_STATE) {
		rtk_bt_le_audio_vcs_get_volume_state_param_t *param = (rtk_bt_le_audio_vcs_get_volume_state_param_t *)dst;
		memcpy((void *)param->p_vcs_volume_state, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_MICS_GET_MUTE_VALUE) {
		rtk_bt_le_audio_mics_get_mute_value_param_t *param = (rtk_bt_le_audio_mics_get_mute_value_param_t *)dst;
		memcpy((void *)param->p_mic_mute, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_MICS_GET_PARAM) {
		memcpy((void *)dst, (void *)src, rtk_le_audio_get_act_ret_param_size(act));
	}
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_VOCS_GET_PARAM) {
		rtk_bt_le_audio_vocs_get_param_t *param_dst = (rtk_bt_le_audio_vocs_get_param_t *)dst;
		rtk_bt_le_audio_vocs_param_t *p_vocs_param_src = (rtk_bt_le_audio_vocs_param_t *)src;
		param_dst->p_vocs_param->volume_offset = p_vocs_param_src->volume_offset;
		param_dst->p_vocs_param->change_counter = p_vocs_param_src->change_counter;
		param_dst->p_vocs_param->audio_location = p_vocs_param_src->audio_location;
		param_dst->p_vocs_param->output_des.output_des_len = p_vocs_param_src->output_des.output_des_len;
		if (param_dst->p_vocs_param->output_des.p_output_des) {
			memcpy(param_dst->p_vocs_param->output_des.p_output_des, (uint8_t *)src + sizeof(rtk_bt_le_audio_vocs_param_t), p_vocs_param_src->output_des.output_des_len);
		}
	} else if (act == RTK_BT_LE_AUDIO_ACT_VOCS_GET_SRV_DATA) {
		rtk_bt_le_audio_vocs_get_srv_data_param_t *param_dst = (rtk_bt_le_audio_vocs_get_srv_data_param_t *)dst;
		rtk_bt_le_audio_vocs_srv_data_t *p_srv_data_src = (rtk_bt_le_audio_vocs_srv_data_t *)src;
		param_dst->p_srv_data->srv_instance_id = p_srv_data_src->srv_instance_id;
		param_dst->p_srv_data->type_exist = p_srv_data_src->type_exist;
		memcpy(&param_dst->p_srv_data->volume_offset, &p_srv_data_src->volume_offset, sizeof(rtk_bt_le_audio_vocs_volume_offset_state_t));
		param_dst->p_srv_data->audio_location = p_srv_data_src->audio_location;
		param_dst->p_srv_data->output_des.output_des_len = p_srv_data_src->output_des.output_des_len;
		if (param_dst->p_srv_data->output_des.p_output_des) {
			memcpy(param_dst->p_srv_data->output_des.p_output_des, (uint8_t *)src + sizeof(rtk_bt_le_audio_vocs_srv_data_t), p_srv_data_src->output_des.output_des_len);
		}
	}
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	else if (act == RTK_BT_LE_AUDIO_ACT_AICS_GET_PARAM) {
		rtk_bt_le_audio_aics_get_param_t *param = (rtk_bt_le_audio_aics_get_param_t *)dst;
		memcpy((void *)param->p_value, (void *)src, param->value_len);
	} else if (act == RTK_BT_LE_AUDIO_ACT_AICS_GET_SRV_DATA) {
		rtk_bt_le_audio_aics_get_srv_data_param_t *param_dst = (rtk_bt_le_audio_aics_get_srv_data_param_t *)dst;
		rtk_bt_le_audio_aics_srv_data_t *p_srv_data_src = (rtk_bt_le_audio_aics_srv_data_t *)src;
		param_dst->p_srv_data->srv_instance_id = p_srv_data_src->srv_instance_id;
		param_dst->p_srv_data->type_exist = p_srv_data_src->type_exist;
		memcpy(&param_dst->p_srv_data->input_state, &p_srv_data_src->input_state, sizeof(rtk_bt_le_audio_aics_input_state_t));
		memcpy(&param_dst->p_srv_data->setting_prop, &p_srv_data_src->setting_prop, sizeof(rtk_bt_le_audio_aics_gain_setting_prop_t));
		param_dst->p_srv_data->input_type = p_srv_data_src->input_type;
		param_dst->p_srv_data->input_status = p_srv_data_src->input_status;
		param_dst->p_srv_data->input_des.input_des_len = p_srv_data_src->input_des.input_des_len;
		if (param_dst->p_srv_data->input_des.p_input_des) {
			memcpy(param_dst->p_srv_data->input_des.p_input_des, p_srv_data_src->input_des.p_input_des, p_srv_data_src->input_des.input_des_len);
		}
	}
#endif
	return RTK_BT_OK;
}

static int le_audio_ipc_iso_data_send_push(uint8_t *buf, rtk_bt_le_audio_iso_data_send_info_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_iso_data_send_info_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->data_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_data, size);
	((rtk_bt_le_audio_iso_data_send_info_t *)buf)->p_data = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_iso_data_send_info_t size is over deep copy buf\r\n");
	return -1;
}

static int le_audio_ipc_pacs_get_pac_record_push(uint8_t *buf, rtk_bt_le_audio_pacs_get_pac_record_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_pacs_get_pac_record_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	//copy pac_num
	size = sizeof(uint8_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_pac_num, size);
	((rtk_bt_le_audio_pacs_get_pac_record_param_t *)buf)->p_pac_num = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_iso_data_send_info_t size is over deep copy buf\r\n");
	return -1;
}
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
static int le_audio_ipc_sync_setup_path_push(uint8_t *buf, rtk_bt_le_audio_sync_setup_data_path_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bis_data_path_param_t *p_setup_param = &param->setup_param;
	rtk_bt_le_audio_sync_setup_data_path_param_t *p_buf = (rtk_bt_le_audio_sync_setup_data_path_param_t *)buf;

	size = sizeof(rtk_bt_le_audio_sync_setup_data_path_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = p_setup_param->codec_config_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
static int le_audio_ipc_broadcast_source_create_push(uint8_t *buf, rtk_bt_le_audio_broadcast_source_create_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	rtk_bt_le_audio_broadcast_source_create_param_t *create_buf = (rtk_bt_le_audio_broadcast_source_create_param_t *)buf;
	uint8_t i = 0, j = 0, k = 0;
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_broadcast_source_create_param_t);
	if (end + size > buf + size_limit) {
		printf("%s size = %d,size_limit = %d\r\n", __func__, (int)size, (int)size_limit);
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	for (i = 0; i < param->num_groups; i++) {
		for (j = 0; j < param->group[i].num_subgroups; j++) {
			//copy codec_cfg_l2
			size = param->group[i].subgroup[j].codec_cfg_l2_len;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->group[i].subgroup[j].p_codec_cfg_l2, size);
			create_buf->group[i].subgroup[j].p_codec_cfg_l2 = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);

			//copy metadata
			size = param->group[i].subgroup[j].metadata_len;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->group[i].subgroup[j].p_metadata, size);
			create_buf->group[i].subgroup[j].p_metadata = (uint8_t *)(end - buf);
			end += ALIGN_UP(size, 4);

			for (k = 0; k < param->group[i].subgroup[j].num_bis; k++) {
				//copy p_codec_cfg_l3
				size = param->group[i].subgroup[j].bis[k].codec_cfg_l3_len;
				if (end + size > buf + size_limit) {
					goto exit;
				}
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

static int le_audio_ipc_broadcast_source_setup_path_push(uint8_t *buf, rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bis_data_path_param_t *p_setup_param = &param->setup_param;
	rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *p_buf = (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)buf;

	size = sizeof(rtk_bt_le_audio_broadcast_source_setup_data_path_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = p_setup_param->codec_config_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}

	memcpy(end, p_setup_param->p_codec_config, size);
	p_buf->setup_param.p_codec_config = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_bis_data_path_param_t size is over deep copy buf\r\n");
	return -1;
}

static int le_audio_ipc_broadcast_source_reconfig_push(uint8_t *buf, rtk_bt_le_audio_broadcast_source_reconfig_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_broadcast_source_reconfig_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->metadata_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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

#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
static int le_audio_ipc_bass_client_write_cp_op_push(uint8_t *buf, rtk_bt_le_audio_bass_client_write_cp_op_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	rtk_bt_le_audio_bass_client_write_cp_op_param_t *buf_param = (rtk_bt_le_audio_bass_client_write_cp_op_param_t *)buf;

	size = sizeof(rtk_bt_le_audio_bass_client_write_cp_op_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	switch (param->opcode) {
	case RTK_BT_LE_AUDIO_BASS_CP_ADD_SOURCE:
		if (param->op_param.add_source_param.bis_info_size) {
			//copy p_metadata
			for (uint8_t i = 0; i < param->op_param.add_source_param.num_subgroups; i++) {
				if (param->op_param.add_source_param.p_cp_bis_info[i].metadata_len) {
					size = param->op_param.add_source_param.p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->op_param.add_source_param.p_cp_bis_info[i].p_metadata, size);
					param->op_param.add_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}

			//copy p_cp_bis_info
			size = param->op_param.add_source_param.bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->op_param.add_source_param.p_cp_bis_info, size);
			buf_param->op_param.add_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}
		break;

	case RTK_BT_LE_AUDIO_BASS_CP_MODIFY_SOURCE:
		if (param->op_param.modify_source_param.bis_info_size) {
			//copy p_metadata
			for (uint8_t i = 0; i < param->op_param.modify_source_param.num_subgroups; i++) {
				if (param->op_param.modify_source_param.p_cp_bis_info[i].metadata_len) {
					size = param->op_param.modify_source_param.p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata, size);
					param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}

			//copy p_cp_bis_info
			size = param->op_param.modify_source_param.bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->op_param.modify_source_param.p_cp_bis_info, size);
			buf_param->op_param.modify_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}
		break;

	default:
		break;
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_bass_client_write_cp_op_push size is over deep copy buf\r\n");
	return -1;
}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
static int le_audio_ipc_unicast_config_push(uint8_t *buf, rtk_bt_le_audio_unicast_config_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_unicast_config_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->device_num * sizeof(rtk_bt_le_audio_device_handle_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_device_handle_tbl, size);
	((rtk_bt_le_audio_unicast_config_param_t *)buf)->p_device_handle_tbl = (rtk_bt_le_audio_device_handle_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_unicast_config_push size is over deep copy buf\r\n");
	return -1;
}
static int le_audio_ipc_unicast_get_cfg_mask_push(uint8_t *buf, rtk_bt_le_audio_unicast_get_cfg_mask_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_unicast_get_cfg_mask_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->device_num * sizeof(rtk_bt_le_audio_device_handle_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_device_handle_tbl, size);
	((rtk_bt_le_audio_unicast_get_cfg_mask_param_t *)buf)->p_device_handle_tbl = (rtk_bt_le_audio_device_handle_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_unicast_get_cfg_mask_push size is over deep copy buf\r\n");
	return -1;
}
#endif

#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
static int le_audio_ipc_csis_set_coordinator_check_adv_rsi_push(uint8_t *buf, rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->report_data_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_report_data, size);
	((rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *)buf)->p_report_data = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_csis_set_coordinator_check_adv_rsi_push size is over deep copy buf\r\n");
	return -1;
}
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
static int le_audio_ipc_mcp_server_send_data_push(uint8_t *buf, rtk_bt_le_audio_mcp_server_send_data_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_mcp_server_send_data_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME) {
		size = param->param.media_player_name.media_player_name_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->param.media_player_name.p_media_player_name, size);
		((rtk_bt_le_audio_mcp_server_send_data_param_t *)buf)->param.media_player_name.p_media_player_name = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	} else if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE) {
		size = param->param.track_title.track_title_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->param.track_title.p_track_title, size);
		((rtk_bt_le_audio_mcp_server_send_data_param_t *)buf)->param.track_title.p_track_title = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_mcp_server_send_data_push size is over deep copy buf\r\n");
	return -1;
}

static int le_audio_ipc_mcp_server_read_confirm_push(uint8_t *buf, rtk_bt_le_audio_mcp_server_read_confirm_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_mcp_server_read_confirm_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME) {
		size = param->param.media_player_name.media_player_name_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->param.media_player_name.p_media_player_name, size);
		((rtk_bt_le_audio_mcp_server_read_confirm_param_t *)buf)->param.media_player_name.p_media_player_name = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	} else if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE) {
		size = param->param.track_title.track_title_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->param.track_title.p_track_title, size);
		((rtk_bt_le_audio_mcp_server_read_confirm_param_t *)buf)->param.track_title.p_track_title = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	} else if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_ICON_URL) {
		size = param->param.media_player_icon_url.media_player_icon_url_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->param.media_player_icon_url.p_media_player_icon_url, size);
		((rtk_bt_le_audio_mcp_server_read_confirm_param_t *)buf)->param.media_player_icon_url.p_media_player_icon_url = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);

	return 0;

exit:
	printf("le_audio_ipc_mcp_server_read_confirm_push size is over deep copy buf\r\n");
	return -1;
}
#endif

#if 0
static int le_audio_ipc_unicast_cfg_ase_metadata_push(uint8_t *buf, rtk_bt_le_audio_unicast_cfg_ase_metadata_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_unicast_cfg_ase_metadata_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->metadata_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_metadata, size);
	((rtk_bt_le_audio_unicast_cfg_ase_metadata_param_t *)buf)->p_metadata = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_broadcast_source_reconfig_param_t size is over deep copy buf\r\n");
	return -1;
}
#endif


#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
static int le_audio_ipc_vocs_set_param_push(uint8_t *buf, rtk_bt_le_audio_vocs_set_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_vocs_set_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->vocs_param.output_des.output_des_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->vocs_param.output_des.p_output_des, size);
	((rtk_bt_le_audio_vocs_set_param_t *)buf)->vocs_param.output_des.p_output_des = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_vocs_set_param_push size is over deep copy buf\r\n");
	return -1;
}
static int le_audio_ipc_vocs_write_output_des_push(uint8_t *buf, rtk_bt_le_audio_vocs_write_output_des_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_vocs_write_output_des_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->output_des_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_output_des, size);
	((rtk_bt_le_audio_vocs_write_output_des_param_t *)buf)->p_output_des = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_vocs_write_output_des_push size is over deep copy buf\r\n");
	return -1;
}
#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
static int le_audio_ipc_aics_set_param_push(uint8_t *buf, rtk_bt_le_audio_aics_set_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_aics_set_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->value_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_value, size);
	((rtk_bt_le_audio_aics_set_param_t *)buf)->p_value = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_vocs_set_param_push size is over deep copy buf\r\n");
	return -1;
}
static int le_audio_ipc_aics_write_input_des_push(uint8_t *buf, rtk_bt_le_audio_aics_write_input_des_param_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_aics_write_input_des_param_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->input_des_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_input_des, size);
	((rtk_bt_le_audio_aics_write_input_des_param_t *)buf)->p_input_des = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_aics_write_input_des_push size is over deep copy buf\r\n");
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
		if (le_audio_ipc_iso_data_send_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_iso_data_send_info_t *)data,
											IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

	case RTK_BT_LE_AUDIO_ACT_PACS_GET_PAC_RECORD:
		if (le_audio_ipc_pacs_get_pac_record_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_pacs_get_pac_record_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;

#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH:
		if (le_audio_ipc_sync_setup_path_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_sync_setup_data_path_param_t *)data,
											  IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE:
		if (le_audio_ipc_broadcast_source_create_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_broadcast_source_create_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH:
		if (le_audio_ipc_broadcast_source_setup_path_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG:
		if (le_audio_ipc_broadcast_source_reconfig_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_broadcast_source_reconfig_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_WRITE_CP_OP:
		if (le_audio_ipc_bass_client_write_cp_op_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_bass_client_write_cp_op_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_UNICAST_CONFIG:
		if (le_audio_ipc_unicast_config_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_unicast_config_param_t *)data,
											 IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CFG_MASK:
		if (le_audio_ipc_unicast_get_cfg_mask_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_unicast_get_cfg_mask_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#if 0
	case RTK_BT_LE_AUDIO_ACT_UNICAST_CFG_ASE_METADATA:
		if (le_audio_ipc_unicast_cfg_ase_metadata_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_unicast_cfg_ase_metadata_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_CHECK_ADV_RSI:
		if (le_audio_ipc_csis_set_coordinator_check_adv_rsi_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_SEND_DATA:
		if (le_audio_ipc_mcp_server_send_data_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_mcp_server_send_data_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_READ_CFM:
		if (le_audio_ipc_mcp_server_read_confirm_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_mcp_server_read_confirm_param_t *)data,
				IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_VOCS_SET_PARAM:
		if (le_audio_ipc_vocs_set_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_vocs_set_param_t *)data,
											 IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	case RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_OUTPUT_DES:
		if (le_audio_ipc_vocs_write_output_des_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_vocs_write_output_des_param_t *)data,
											 IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_AICS_SET_PARAM:
		if (le_audio_ipc_aics_set_param_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_aics_set_param_t *)data,
											 IPC_HOST_API_DATA_MAX, pactual_size)) {
			osif_mem_free(host_msg);
			host_msg = NULL;
		}
		break;
	case RTK_BT_LE_AUDIO_ACT_AICS_WRITE_INPUT_DES:
		if (le_audio_ipc_aics_write_input_des_push((uint8_t *)host_msg->param_buf, (rtk_bt_le_audio_aics_write_input_des_param_t *)data,
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
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
static void le_audio_ipc_mcp_client_read_result_ind_evt_pop(rtk_bt_le_audio_mcp_client_read_result_ind_t *evt)
{
	switch (evt->char_uuid) {
	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
		evt->data.media_player_name.p_media_player_name = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.media_player_name.p_media_player_name));
		break;
	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE:
		evt->data.track_title.p_track_title = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.track_title.p_track_title));
		break;
	default:
		break;
	}
}
static void le_audio_ipc_mcs_client_notify_result_ind_evt_pop(rtk_bt_le_audio_mcp_client_notify_ind_t *evt)
{
	switch (evt->char_uuid) {
	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
		evt->data.media_player_name.p_media_player_name = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.media_player_name.p_media_player_name));
		break;
	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE:
		evt->data.track_title.p_track_title = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.track_title.p_track_title));
		break;
	default:
		break;
	}
}
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
static void le_audio_ipc_pa_adv_report_ind_evt_pop(rtk_bt_le_audio_pa_adv_report_ind_t *evt)
{
	evt->p_data = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_data));
}
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
static void le_audio_ipc_bass_client_sync_info_req_ind_pop(rtk_bt_le_audio_bass_client_sync_info_req_ind_t *evt)
{
	if (evt->p_brs_data) {
		evt->p_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data));
		if (evt->p_brs_data->bis_info_size) {
			evt->p_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data->p_cp_bis_info));
			for (uint8_t i = 0; i < evt->p_brs_data->num_subgroups; i++) {
				evt->p_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data->p_cp_bis_info[i].p_metadata));
			}
		}
	}
}

static void le_audio_ipc_bass_client_brs_data_ind_pop(rtk_bt_le_audio_bass_client_brs_data_ind_t *evt)
{
	if (evt->p_brs_data) {
		evt->p_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data));
		if (evt->p_brs_data->bis_info_size) {
			evt->p_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data->p_cp_bis_info));
			for (uint8_t i = 0; i < evt->p_brs_data->num_subgroups; i++) {
				evt->p_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data->p_cp_bis_info[i].p_metadata));
			}
		}
	}
}
#endif

#if defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT
static void le_audio_ipc_bass_cp_ind_pop(rtk_bt_le_audio_bass_cp_ind_t *evt)
{
	switch (evt->opcode) {
	case RTK_BT_LE_AUDIO_BASS_CP_ADD_SOURCE:
		if (evt->op_param.add_source_param.bis_info_size) {
			evt->op_param.add_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)evt + (uint32_t)(
						evt->op_param.add_source_param.p_cp_bis_info));
			for (uint8_t i = 0; i < evt->op_param.add_source_param.num_subgroups; i++) {
				evt->op_param.add_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)evt + (uint32_t)(
							evt->op_param.add_source_param.p_cp_bis_info[i].p_metadata));
			}
		}
		break;
	case RTK_BT_LE_AUDIO_BASS_CP_MODIFY_SOURCE:
		if (evt->op_param.modify_source_param.bis_info_size) {
			evt->op_param.modify_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)evt + (uint32_t)(
						evt->op_param.modify_source_param.p_cp_bis_info));
			for (uint8_t i = 0; i < evt->op_param.modify_source_param.num_subgroups; i++) {
				evt->op_param.modify_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)evt + (uint32_t)(
							evt->op_param.modify_source_param.p_cp_bis_info[i].p_metadata));
			}
		}
		break;
	default:
		break;
	}
}
static void le_audio_ipc_bass_brs_data_ind_pop(rtk_bt_le_audio_bass_brs_modify_ind_t *evt)
{
	if (evt->p_brs_data) {
		evt->p_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data));
		if (evt->p_brs_data->bis_info_size) {
			evt->p_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data->p_cp_bis_info));
			for (uint8_t i = 0; i < evt->p_brs_data->num_subgroups; i++) {
				evt->p_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_brs_data->p_cp_bis_info[i].p_metadata));
			}
		}
	}
}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
static void le_audio_ipc_bap_start_metadata_cfg_ind_evt_pop(rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *evt)
{
	evt->p_metadata = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->p_metadata));
}
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
static void le_audio_ipc_vocs_write_output_des_ind_evt_pop(rtk_bt_le_audio_vocs_write_output_des_ind_t *evt)
{
	evt->output_des.p_output_des = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->output_des.p_output_des));
}
static void le_audio_ipc_vocs_client_read_result_ind_evt_pop(rtk_bt_le_audio_vocs_client_read_result_ind_t *evt)
{
	if (evt->type == RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC) {
		evt->data.output_des.p_output_des = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.output_des.p_output_des));
	}
}
static void le_audio_ipc_vocs_client_notify_ind_evt_pop(rtk_bt_le_audio_vocs_client_notify_ind_t *evt)
{
	if (evt->type == RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC) {
		evt->data.output_des.p_output_des = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.output_des.p_output_des));
	}
}
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
static void le_audio_ipc_aics_write_input_des_ind_evt_pop(rtk_bt_le_audio_aics_write_input_des_ind_t *evt)
{
	evt->input_des.p_input_des = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->input_des.p_input_des));
}
static void le_audio_ipc_aics_client_read_result_ind_evt_pop(rtk_bt_le_audio_aics_client_read_result_ind_t *evt)
{
	if (evt->type == RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES) {
		evt->data.input_des.p_input_des = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.input_des.p_input_des));
	}
}
static void le_audio_ipc_aics_client_notify_ind_evt_pop(rtk_bt_le_audio_aics_client_notify_ind_t *evt)
{
	if (evt->type == RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES) {
		evt->data.input_des.p_input_des = (uint8_t *)((uint8_t *)evt + (uint32_t)(evt->data.input_des.p_input_des));
	}
}
#endif
void bt_le_audio_ipc_pop_event_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND:
		le_audio_ipc_iso_data_ind_evt_pop((rtk_bt_le_audio_direct_iso_data_ind_t *)param);
		break;
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_READ_RESULT_IND:
		le_audio_ipc_mcp_client_read_result_ind_evt_pop((rtk_bt_le_audio_mcp_client_read_result_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_NOTIFY_IND:
		le_audio_ipc_mcs_client_notify_result_ind_evt_pop((rtk_bt_le_audio_mcp_client_notify_ind_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND:
		le_audio_ipc_pa_adv_report_ind_evt_pop((rtk_bt_le_audio_pa_adv_report_ind_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BASS_CLIENT_SYNC_INFO_REQ_IND:
		le_audio_ipc_bass_client_sync_info_req_ind_pop((rtk_bt_le_audio_bass_client_sync_info_req_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_BASS_CLIENT_BRS_DATA_IND:
		le_audio_ipc_bass_client_brs_data_ind_pop((rtk_bt_le_audio_bass_client_brs_data_ind_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BASS_CP_IND:
		le_audio_ipc_bass_cp_ind_pop((rtk_bt_le_audio_bass_cp_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_BASS_BRS_MODIFY_IND:
		le_audio_ipc_bass_brs_data_ind_pop((rtk_bt_le_audio_bass_brs_modify_ind_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND:
		le_audio_ipc_bap_start_metadata_cfg_ind_evt_pop((rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_VOCS_WRITE_OUTPUT_DES_IND:
		le_audio_ipc_vocs_write_output_des_ind_evt_pop((rtk_bt_le_audio_vocs_write_output_des_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND:
		le_audio_ipc_vocs_client_read_result_ind_evt_pop((rtk_bt_le_audio_vocs_client_read_result_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_NOTIFY_IND:
		le_audio_ipc_vocs_client_notify_ind_evt_pop((rtk_bt_le_audio_vocs_client_notify_ind_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_AICS_WRITE_INPUT_DES_IND:
		le_audio_ipc_aics_write_input_des_ind_evt_pop((rtk_bt_le_audio_aics_write_input_des_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_READ_RESULT_IND:
		le_audio_ipc_aics_client_read_result_ind_evt_pop((rtk_bt_le_audio_aics_client_read_result_ind_t *)param);
		break;
	case RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_NOTIFY_IND:
		le_audio_ipc_aics_client_notify_ind_evt_pop((rtk_bt_le_audio_aics_client_notify_ind_t *)param);
		break;
#endif
	default:
		break;
	}
}
void bt_le_audio_push_event_ipc_ret(uint16_t event, void *dst, void *src)
{
	uint8_t *ret = (uint8_t *)dst;

	switch (event) {
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND: {
		rtk_bt_le_audio_bass_get_pa_sync_param_ind_t *param = (rtk_bt_le_audio_bass_get_pa_sync_param_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_pa_sync_param, sizeof(rtk_bt_le_audio_bass_pa_sync_param_t));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND: {
		rtk_bt_le_audio_bass_get_big_sync_param_ind_t *param = (rtk_bt_le_audio_bass_get_big_sync_param_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_big_sync_param, sizeof(rtk_bt_le_audio_bass_big_sync_param_t));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND: {
		rtk_bt_le_audio_bass_get_broadcast_code_ind_t *param = (rtk_bt_le_audio_bass_get_broadcast_code_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_broadcast_code, RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN);
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND: {
		rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t *param = (rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_support_bis_array, sizeof(uint32_t));
		break;
	}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND: {
		rtk_bt_le_audio_bap_start_qos_cfg_ind_t *param = (rtk_bt_le_audio_bap_start_qos_cfg_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_session_qos, sizeof(rtk_bt_le_audio_unicast_session_qos_t));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND: {
		rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *param = (rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_metadata_len, sizeof(uint8_t));
		memcpy((void *)(ret + 1), (void *)param->p_metadata, *param->p_metadata_len);
		break;
	}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND: {
		rtk_bt_le_audio_ascs_get_prefer_qos_ind_t *param = (rtk_bt_le_audio_ascs_get_prefer_qos_ind_t *)src;
		memcpy((void *)ret, (void *)param->p_prefer_qos_data, sizeof(rtk_bt_le_audio_ascs_prefer_qos_data_t));
		break;
	}
#endif
	default:
		break;
	}
	(void)ret;
	(void)src;
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
void bt_le_audio_push_event_ipc_ret(uint16_t event, void *dst, void *src)
{
	(void)event;
	(void)dst;
	(void)src;
}
#endif
#elif defined(CONFIG_BT_NP) && CONFIG_BT_NP
/* ===============================================rtk_bt_le_gap_ipc_dev action======================================= */
// static struct evt_mem_option rtk_bt_gap_evt_mem_op_tab[] = {
// #if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
// 	{RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND, sizeof(rtk_bt_ecfc_conn_cmpl_ind_t)},
// 	{RTK_BT_GAP_EVT_ECFC_DATA_IND, sizeof(rtk_bt_ecfc_data_ind_t)},
// 	{RTK_BT_GAP_EVT_ECFC_CONN_REQ_IND, sizeof(rtk_bt_ecfc_conn_req_ind_t)},
// 	{RTK_BT_GAP_EVT_ECFC_DISCONN_IND, sizeof(rtk_bt_ecfc_disconn_ind_t)},
// 	{RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND, sizeof(rtk_bt_ecfc_reconf_req_ind_t)},
// 	{RTK_BT_GAP_EVT_ECFC_RECONF_RSP_IND, sizeof(rtk_bt_ecfc_reconf_rsp_ind_t)},
// #endif
// 	{RTK_BT_GAP_EVT_MAX, 0}
// };

// uint32_t get_bt_gap_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_gap_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_gap_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_gap_evt_mem_op_tab[i].evt == RTK_BT_GAP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

static void device_ipc_bt_enable_pop(rtk_bt_app_conf_t *param)
{
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
	rtk_bt_le_audio_app_conf_t *p_le_audio_app_conf = &param->le_audio_app_conf;
	rtk_bt_le_audio_pacs_init_param_t *p_pacs_param	= &p_le_audio_app_conf->pacs_param;
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	rtk_bt_le_audio_aics_init_param_t *p_aics_param	= &p_le_audio_app_conf->cap_param.aics_param;
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	rtk_bt_le_audio_vocs_init_param_t *p_vocs_param	= &p_le_audio_app_conf->cap_param.vocs_param;
#endif

	if (p_pacs_param->pac_sink_codec_len) {
		p_pacs_param->p_pac_sink_codec = (uint8_t *)param + (uint32_t)p_pacs_param->p_pac_sink_codec;
	}
	if (p_pacs_param->pac_source_codec_len) {
		p_pacs_param->p_pac_source_codec = (uint8_t *)param + (uint32_t)p_pacs_param->p_pac_source_codec;
	}

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	if (p_aics_param->aics_vcs_num) {
		p_aics_param->p_aics_vcs_tbl = (uint8_t *)param + (uint32_t)p_aics_param->p_aics_vcs_tbl;
	}
	if (p_aics_param->aics_mics_num) {
		p_aics_param->p_aics_mics_tbl = (uint8_t *)param + (uint32_t)p_aics_param->p_aics_mics_tbl;
	}
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	if (p_vocs_param->vocs_num) {
		p_vocs_param->p_vocs_feature_tbl = (uint8_t *)param + (uint32_t)p_vocs_param->p_vocs_feature_tbl;
	}
#endif

#else
	(void)param;
#endif
}

void bt_device_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)param;

	switch (act) {

	case RTK_BT_DEVICE_IPC_ACT_BT_ENABLE:
		device_ipc_bt_enable_pop((rtk_bt_app_conf_t *)param);
		break;

	default:
		break;
	}
}

#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
static void gap_ipc_ecfc_send_data_pop(rtk_bt_gap_ecfc_send_data_t *param)
{
	param->p_data = (uint8_t *)param + (uint32_t)param->p_data;
}
#endif

static void gap_ipc_vendor_cmd_req_pop(rtk_bt_gap_vendor_cmd_param_t *vendor_cmd)
{
	vendor_cmd->cmd_param = (uint8_t *)((uint8_t *)vendor_cmd + (uint32_t)vendor_cmd->cmd_param);
}

void bt_gap_ipc_pop_cmd_param(uint8_t act, void *param)
{
	(void)param;

	switch (act) {
#if defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_ACT_ECFC_SEND_DATA:
		gap_ipc_ecfc_send_data_pop((rtk_bt_gap_ecfc_send_data_t *)param);
		break;
#endif
	case RTK_BT_GAP_ACT_VENDOR_CMD_REQ:
		gap_ipc_vendor_cmd_req_pop((rtk_bt_gap_vendor_cmd_param_t *)param);
		break;
	default:
		break;
	}
}

/* ===============================================rtk_bt_le_gap_ipc_dev action======================================= */
// static struct evt_mem_option rtk_bt_le_gap_evt_mem_op_tab[] = {
// 	{RTK_BT_LE_GAP_EVT_ADV_START_IND, sizeof(rtk_bt_le_adv_start_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_ADV_STOP_IND, sizeof(rtk_bt_le_adv_stop_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_CONNECT_IND, sizeof(rtk_bt_le_conn_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_DISCONN_IND, sizeof(rtk_bt_le_disconn_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_SCAN_START_IND, sizeof(uint16_t)},
// 	{RTK_BT_LE_GAP_EVT_SCAN_RES_IND, sizeof(rtk_bt_le_scan_res_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_SCAN_STOP_IND, sizeof(uint16_t)},
// 	{RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND, sizeof(rtk_bt_le_conn_update_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND, sizeof(rtk_bt_le_remote_conn_update_req_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND, sizeof(rtk_bt_le_data_len_change_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND, sizeof(rtk_bt_le_phy_update_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_AUTH_PAIRING_CONFIRM_IND, sizeof(rtk_bt_le_auth_pair_cfm_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_DISPLAY_IND, sizeof(rtk_bt_le_auth_key_display_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_INPUT_IND, sizeof(rtk_bt_le_auth_key_input_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_AUTH_PASSKEY_CONFIRM_IND, sizeof(rtk_bt_le_auth_key_cfm_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_AUTH_OOB_KEY_INPUT_IND, sizeof(rtk_bt_le_auth_oob_input_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND, sizeof(rtk_bt_le_auth_complete_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND, sizeof(rtk_bt_le_bond_modify_ind_t)},
// #if defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_EXT_ADV_IND, sizeof(rtk_bt_le_ext_adv_ind_t)},
// #endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */
// #if defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) && RTK_BLE_5_0_AE_SCAN_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND, sizeof(rtk_bt_le_ext_scan_res_ind_t)},
// #endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */
// #if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_PA_IND, sizeof(rtk_bt_le_pa_ind_t)},
// #endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */
// #if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND, sizeof(rtk_bt_le_pa_sync_ind_t)},
// 	{RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND, sizeof(rtk_bt_le_pa_adv_report_ind_t)},
// #endif
// #if defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_PAST_RECEIVED_INFO_IND, sizeof(rtk_bt_le_past_recv_ind_t)},
// #endif
// #if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_RESOLV_LIST_MODIFY_IND, sizeof(rtk_bt_le_modify_resolv_list_ind_t)},
// #endif
// #if defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT
// 	{RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND, sizeof(rtk_bt_le_txpower_ind_t)},
// #endif
// 	{RTK_BT_LE_GAP_EVT_MAX, 0}
// };

// uint32_t get_bt_le_gap_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_le_gap_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_le_gap_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_le_gap_evt_mem_op_tab[i].evt == RTK_BT_LE_GAP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

static void le_ipc_scan_info_filter_pop(rtk_bt_le_scan_info_filter_param_t *p_scan_info)
{
	p_scan_info->p_filter = (uint8_t *)((uint8_t *)p_scan_info + (uint32_t)p_scan_info->p_filter);
}

#if defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
static void le_ipc_create_ext_adv_pop(rtk_bt_le_ext_adv_create_t *p_create)
{
	p_create->p_adv_param = (rtk_bt_le_ext_adv_param_t *)((uint8_t *)p_create + (uint32_t)p_create->p_adv_param);
}

static void le_ipc_set_ext_adv_data_pop(rtk_bt_le_ext_adv_data_t *ext_adv_data)
{
	ext_adv_data->pdata = (uint8_t *)((uint8_t *)ext_adv_data + (uint32_t)ext_adv_data->pdata);
}
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
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
#if defined(RTK_BLE_5_0_AE_ADV_SUPPORT) && RTK_BLE_5_0_AE_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV:
		le_ipc_create_ext_adv_pop((rtk_bt_le_ext_adv_create_t *)param);
		break;
	case RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA:
	case RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA:
		le_ipc_set_ext_adv_data_pop((rtk_bt_le_ext_adv_data_t *)param);
		break;
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT */
#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
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

#if defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) && RTK_BLE_5_0_AE_SCAN_SUPPORT
static int le_ipc_ext_scan_res_evt_push(uint8_t *buf, rtk_bt_le_ext_scan_res_ind_t *param,
										uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_ext_scan_res_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
static int le_ipc_pa_adv_report_evt_push(uint8_t *buf, rtk_bt_le_pa_adv_report_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_pa_adv_report_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->data_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
#if defined(RTK_BLE_5_0_AE_SCAN_SUPPORT) && RTK_BLE_5_0_AE_SCAN_SUPPORT
	case RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND:
		ret = le_ipc_ext_scan_res_evt_push((uint8_t *)dev_msg->param_buf, data,
										   IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
	case RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND:
		ret = le_ipc_pa_adv_report_evt_push((uint8_t *)dev_msg->param_buf, data,
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
// static struct evt_mem_option rtk_bt_gatts_evt_mem_op_tab[] = {
// 	{RTK_BT_GATTS_EVT_REGISTER_SERVICE, sizeof(rtk_bt_gatts_reg_ind_t)},
// 	{RTK_BT_GATTS_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t)},
// 	{RTK_BT_GATTS_EVT_READ_IND, sizeof(rtk_bt_gatts_read_ind_t)},
// 	{RTK_BT_GATTS_EVT_WRITE_IND, sizeof(rtk_bt_gatts_write_ind_t)},
// 	{RTK_BT_GATTS_EVT_CCCD_IND, sizeof(rtk_bt_gatts_cccd_ind_t)},
// 	{RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND, sizeof(rtk_bt_gatts_ntf_and_ind_ind_t)},
// 	{RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND, sizeof(rtk_bt_gatts_ntf_and_ind_ind_t)},
// 	{RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES, sizeof(rtk_bt_gatts_client_supported_features_ind_t)},
// 	{RTK_BT_GATTS_EVT_MAX, 0}
// };

// uint32_t get_bt_gatts_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_gatts_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_gatts_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_gatts_evt_mem_op_tab[i].evt == RTK_BT_GATTS_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

static void gatts_ipc_reg_srv_param_pop(struct rtk_bt_gatt_service *srv_reg)
{
	uint32_t i, attr_count = 0;
	uint16_t uuid_val = 0;

	attr_count = srv_reg->attr_count;
	srv_reg->attrs = (void *)((uint8_t *)srv_reg + (uint32_t)(srv_reg->attrs));
	for (i = 0; i < attr_count; i++) {
		srv_reg->attrs[i].uuid = (void *)((uint8_t *)srv_reg + (uint32_t)(srv_reg->attrs[i].uuid));
		if (srv_reg->attrs[i].user_data) {
			srv_reg->attrs[i].user_data = (void *)((uint8_t *)srv_reg +
												   (uint32_t)(srv_reg->attrs[i].user_data));

			if (BT_UUID_TYPE_16 == ((struct bt_uuid *)srv_reg->attrs[i].uuid)->type) {
				uuid_val = ((struct bt_uuid_16 *)srv_reg->attrs[i].uuid)->val;
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
		if (gatts_ipc_write_evt_push((uint8_t *)dev_msg->param_buf, data,
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
// static struct evt_mem_option rtk_bt_gattc_evt_mem_op_tab[] = {
// 	{RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND, sizeof(rtk_bt_gattc_discover_ind_t)},
// #if RTK_BLE_MGR_LIB
// 	{RTK_BT_GATTC_EVT_DISCOVER_ALL_STATE_IND, sizeof(rtk_bt_gattc_discover_all_state_ind_t)},
// 	{RTK_BT_GATTC_EVT_GATT_SERVICE_INFO_IND, sizeof(rtk_bt_gattc_gatt_service_info_ind_t)},
// #endif
// 	{RTK_BT_GATTC_EVT_READ_RESULT_IND, sizeof(rtk_bt_gattc_read_ind_t)},
// 	{RTK_BT_GATTC_EVT_WRITE_RESULT_IND, sizeof(rtk_bt_gattc_write_ind_t)},
// 	{RTK_BT_GATTC_EVT_CCCD_ENABLE_IND, sizeof(rtk_bt_gattc_cccd_update_ind_t)},
// 	{RTK_BT_GATTC_EVT_CCCD_DISABLE_IND, sizeof(rtk_bt_gattc_cccd_update_ind_t)},
// 	{RTK_BT_GATTC_EVT_NOTIFY_IND, sizeof(rtk_bt_gattc_cccd_value_ind_t)},
// 	{RTK_BT_GATTC_EVT_INDICATE_IND, sizeof(rtk_bt_gattc_cccd_value_ind_t)},
// 	{RTK_BT_GATTC_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t)},
// 	{RTK_BT_GATTC_EVT_MAX, 0}
// };

// uint32_t get_bt_gattc_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_gattc_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_gattc_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_gattc_evt_mem_op_tab[i].evt == RTK_BT_GATTC_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	struct_end = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->value, size);
	end += ALIGN_UP(size, 4);

	((rtk_bt_gattc_read_ind_t *)buf)->value = (uint8_t *)(struct_end - buf);
	*actual_size = (uint32_t)(end - buf);

#else

	switch (param->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->by_handle.len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->by_handle.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->by_handle.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_BY_UUID:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->by_uuid_per.len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->by_uuid_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->by_uuid_per.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->multiple_per.len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->multiple_per.value, size);
		end += ALIGN_UP(size, 4);

		((rtk_bt_gattc_read_ind_t *)buf)->multiple_per.value = (uint8_t *)(struct_end - buf);
		*actual_size = (uint32_t)(end - buf);
		break;

	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
		size = sizeof(rtk_bt_gattc_read_ind_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param, size);
		end += ALIGN_UP(size, 4);
		struct_end = end;

		size = param->multiple_variable_per.len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
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
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
		if (gattc_ipc_read_evt_push((uint8_t *)dev_msg->param_buf, data,
									IPC_DEV_API_DATA_MAX, pactual_size)) {
			osif_mem_free(dev_msg);
			dev_msg = NULL;
		}
		break;

	case RTK_BT_GATTC_EVT_NOTIFY_IND:
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		if (gatts_ipc_notify_indicate_evt_push((uint8_t *)dev_msg->param_buf, data,
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

/* ===============================================rtk_bt_mesh_ipc_dev action======================================= */
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT

void *bt_mesh_push_event_ipc_buf(uint32_t group, uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	(void)group;
	(void)event;
	uint8_t *start = NULL, *end = NULL;
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));

	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}

#if 0
	uint32_t deep_copy_len = 0, deep_copy_offset = 0;
	uint8_t *border = NULL;
	// For event data struct with pointer, the pointer value name and len is different,so calcu the offset for memcpy later
	switch (group) {
	case RTK_BT_LE_GP_MESH_DATATRANS_MODEL: {
		switch (event) {
		// case RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ:
		// 	deep_copy_offset = (uint8_t *)(&(((rtk_bt_mesh_datatrans_server_read_event_t *)data)->data)) - (uint8_t *)data;
		// 	deep_copy_len = ((rtk_bt_mesh_datatrans_server_read_event_t *)data)->data_len;
		// 	break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
#endif
	start = end = (uint8_t *)dev_msg->param_buf;
	if (buf_len > IPC_DEV_API_DATA_MAX) {
		printf("[%s] Struct size %d extend IPC_DEV_API_DATA_MAX %d\r\n", __func__, (int)buf_len, IPC_DEV_API_DATA_MAX);
		goto exit;
	}
	memcpy(end, data, buf_len);
	end += ALIGN_UP(buf_len, 4);
#if 0
	// Only for data struct with pointer
	if (deep_copy_len) {
		border = end;
		if (end + deep_copy_len > start + IPC_DEV_API_DATA_MAX) {
			printf("[%s] Actual size %d extend IPC_DEV_API_DATA_MAX %d\r\n", __func__, (int)(buf_len + deep_copy_len), IPC_DEV_API_DATA_MAX);
			goto exit;
		}
		memcpy(end, (uint8_t *)data + buf_len, deep_copy_len);
		end += ALIGN_UP(deep_copy_len, 4);
		*(uint32_t *)(start + deep_copy_offset) = (uint32_t)(border - start);
	}
#endif
	*pactual_size = (uint32_t)(end - start);
	return (void *)dev_msg;

exit:
	osif_mem_free(dev_msg);
	dev_msg = NULL;

	return (void *)dev_msg;
}

/* copy event callback returned param data from 'ret' field of ipc buf
	to actual event callback's param  */
uint16_t rtk_mesh_copy_evt_ret_param(uint32_t group, uint8_t evt_code, uint8_t *cb_param, uint8_t *ipc_ret)
{
	if (RTK_BT_LE_GP_MESH_DATATRANS_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ: {
			rtk_bt_mesh_datatrans_server_read_event_t *server_read =
				(rtk_bt_mesh_datatrans_server_read_event_t *)cb_param;
			memcpy(server_read->data, ipc_ret, server_read->data_len);
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_lightness_server_direct_get_t *lightness_get =
				(rtk_bt_mesh_light_lightness_server_direct_get_t *)cb_param;
			memcpy(lightness_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lightness_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_lightness_server_direct_get_t *linear_get =
				(rtk_bt_mesh_light_lightness_server_direct_get_t *)cb_param;
			memcpy(linear_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lightness_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_lightness_server_direct_get_t *default_get =
				(rtk_bt_mesh_light_lightness_server_direct_get_t *)cb_param;
			memcpy(default_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lightness_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_lightness_server_direct_get_t *last_get =
				(rtk_bt_mesh_light_lightness_server_direct_get_t *)cb_param;
			memcpy(last_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lightness_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_lightness_server_direct_get_range_t *range_get =
				(rtk_bt_mesh_light_lightness_server_direct_get_range_t *)cb_param;
			memcpy(range_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lightness_server_get_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_ctl_server_direct_get_t *lightness_ctl_get =
				(rtk_bt_mesh_light_ctl_server_direct_get_t *)cb_param;
			memcpy(lightness_ctl_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_ctl_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_DEFAULT_GET: {
			rtk_bt_mesh_light_ctl_server_direct_get_default_t *default_ctl_get =
				(rtk_bt_mesh_light_ctl_server_direct_get_default_t *)cb_param;
			memcpy(default_ctl_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_ctl_server_get_default_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET: {
			rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *range_ctl_get =
				(rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *)cb_param;
			memcpy(range_ctl_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_ctl_server_get_temperature_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_GET: {
			rtk_bt_mesh_light_ctl_server_direct_get_temperature_t *ctl_temperature_get =
				(rtk_bt_mesh_light_ctl_server_direct_get_temperature_t *)cb_param;
			memcpy(ctl_temperature_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_ctl_server_get_temperature_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET_T: {
			rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *range_temperature_get =
				(rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *)cb_param;
			memcpy(range_temperature_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_ctl_server_get_temperature_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_hsl_server_direct_get_t *hsl_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_t *)cb_param;
			memcpy(hsl_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_HSL_DEFAULT_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_hsl_server_direct_get_default_t *hsl_default_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_default_t *)cb_param;
			memcpy(hsl_default_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_default_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_hsl_server_direct_get_range_t *hsl_range_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_range_t *)cb_param;
			memcpy(hsl_range_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_hsl_server_direct_get_hue_t *hue_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_hue_t *)cb_param;
			memcpy(hue_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_hue_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_H: {
			rtk_bt_mesh_light_hsl_server_direct_get_range_t *hue_range_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_range_t *)cb_param;
			memcpy(hue_range_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_hsl_server_direct_get_saturation_t *saturation_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_saturation_t *)cb_param;
			memcpy(saturation_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_saturation_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_S: {
			rtk_bt_mesh_light_hsl_server_direct_get_range_t *saturation_range_get =
				(rtk_bt_mesh_light_hsl_server_direct_get_range_t *)cb_param;
			memcpy(saturation_range_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_hsl_server_get_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_xyl_server_direct_get_t *xyl_get =
				(rtk_bt_mesh_light_xyl_server_direct_get_t *)cb_param;
			memcpy(xyl_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_xyl_server_get_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_XYL_DEFAULT_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_xyl_server_direct_get_default_t *xyl_default_get =
				(rtk_bt_mesh_light_xyl_server_direct_get_default_t *)cb_param;
			memcpy(xyl_default_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_xyl_server_get_default_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_XYL_RANGE_SERVER_MODEL_GET: {
			rtk_bt_mesh_light_xyl_server_direct_get_range_t *xyl_range_get =
				(rtk_bt_mesh_light_xyl_server_direct_get_range_t *)cb_param;
			memcpy(xyl_range_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_xyl_server_get_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_MODE_GET: {
			rtk_bt_mesh_light_lc_server_direct_get_mode_t *mode_get =
				(rtk_bt_mesh_light_lc_server_direct_get_mode_t *)cb_param;
			memcpy(mode_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lc_server_get_mode_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_OM_GET: {
			rtk_bt_mesh_light_lc_server_direct_get_om_t *om_get =
				(rtk_bt_mesh_light_lc_server_direct_get_om_t *)cb_param;
			memcpy(om_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lc_server_get_om_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_LIGHT_ON_OFF_GET: {
			rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t *onoff_get =
				(rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t *)cb_param;
			memcpy(onoff_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lc_server_get_light_on_off_t));
			break;
		}
		case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_SM_TRANSITION_TIME_GET: {
			rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t *sm_get =
				(rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t *)cb_param;
			memcpy(sm_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lc_server_get_light_on_off_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_PROPERTY_GET: {
			rtk_bt_mesh_light_lc_server_direct_get_property_t *property_get =
				(rtk_bt_mesh_light_lc_server_direct_get_property_t *)cb_param;
			memcpy(property_get->value, ipc_ret, sizeof(rtk_bt_mesh_light_lc_server_direct_get_property_para_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_GET: {
			rtk_bt_mesh_generic_onoff_server_evt_get_t *onoff_get;
			onoff_get = (rtk_bt_mesh_generic_onoff_server_evt_get_t *)cb_param;
			memcpy(onoff_get->on_off, ipc_ret, sizeof(rtk_bt_mesh_generic_on_off_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_TIME_SERVER_MODEL_GET: {
			rtk_bt_mesh_time_server_direct_get_t *time_get =
				(rtk_bt_mesh_time_server_direct_get_t *)cb_param;
			memcpy(time_get->value, ipc_ret, sizeof(rtk_bt_mesh_time_server_get_t));
			break;
		}
		case RTK_BT_MESH_TIME_SERVER_MODEL_ZONE_GET: {
			rtk_bt_mesh_time_server_direct_get_zone_t *zone_get =
				(rtk_bt_mesh_time_server_direct_get_zone_t *)cb_param;
			memcpy(zone_get->value, ipc_ret, sizeof(rtk_bt_mesh_time_server_get_zone_t));
			break;
		}
		case RTK_BT_MESH_TIME_SERVER_MODEL_TAI_UTC_DELTA_GET: {
			rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *delta_get =
				(rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *)cb_param;
			memcpy(delta_get->value, ipc_ret, sizeof(rtk_bt_mesh_time_server_get_tai_utc_delta_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ROLE_GET: {
			rtk_bt_mesh_time_server_get_role_t *role_get =
				(rtk_bt_mesh_time_server_get_role_t *)cb_param;
			memcpy(role_get->role, ipc_ret, sizeof(rtk_bt_mesh_time_role_t));
			break;
		}
		case RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ZONE_GET: {
			rtk_bt_mesh_time_server_direct_get_zone_t *zone_get =
				(rtk_bt_mesh_time_server_direct_get_zone_t *)cb_param;
			memcpy(zone_get->value, ipc_ret, sizeof(rtk_bt_mesh_time_server_get_zone_t));
			break;
		}
		case RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_TAI_UTC_DELTA_GET: {
			rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *delta_get =
				(rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *)cb_param;
			memcpy(delta_get->value, ipc_ret, sizeof(rtk_bt_mesh_time_server_get_tai_utc_delta_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_SCHEDULER_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_SCHEDULER_SERVER_MODEL_GET: {
			rtk_bt_mesh_scheduler_server_get_t *scheduler_get =
				(rtk_bt_mesh_scheduler_server_get_t *)cb_param;
			memcpy(scheduler_get->schedulers, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_SCHEDULER_ACTION_SERVER_MODEL_GET: {
			rtk_bt_mesh_scheduler_server_get_action_t *action_get =
				(rtk_bt_mesh_scheduler_server_get_action_t *)cb_param;
			memcpy(action_get->scheduler, ipc_ret, sizeof(rtk_bt_mesh_scheduler_register_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_SCHEDULER_SETUP_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_SCHEDULER_SETUP_SERVER_MODEL_GET_ACTION: {
			rtk_bt_mesh_scheduler_server_get_action_t *action_get =
				(rtk_bt_mesh_scheduler_server_get_action_t *)cb_param;
			memcpy(action_get->scheduler, ipc_ret, sizeof(rtk_bt_mesh_scheduler_register_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_SCENE_SERVER_MODEL_GET: {
			rtk_bt_mesh_scene_server_get_t *scene_get =
				(rtk_bt_mesh_scene_server_get_t *)cb_param;
			memcpy(scene_get->current_scene, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_REGISTER_STATUS: {
			rtk_bt_mesh_scene_server_get_register_status_t *status_get =
				(rtk_bt_mesh_scene_server_get_register_status_t *)cb_param;
			memcpy(status_get->status, ipc_ret, sizeof(rtk_bt_mesh_scene_status_code_t));
			break;
		}
		case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES: {
			rtk_bt_mesh_scene_server_get_scenes_t *scenes_get =
				(rtk_bt_mesh_scene_server_get_scenes_t *)cb_param;
			memcpy(scenes_get->scenes, ipc_ret, sizeof(uint16_t) * (scenes_get->num_scenes));
			break;
		}
		case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES_NUM: {
			rtk_bt_mesh_scene_server_get_scenes_num_t *num_get =
				(rtk_bt_mesh_scene_server_get_scenes_num_t *)cb_param;
			memcpy(num_get->num_scenes, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_SCENE_SERVER_MODEL_GET_STATUS_RECALL: {
			rtk_bt_mesh_scene_server_get_status_recall_t *status_recall_get =
				(rtk_bt_mesh_scene_server_get_status_recall_t *)cb_param;
			memcpy(status_recall_get->status_recall, ipc_ret, sizeof(rtk_bt_mesh_scene_status_code_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_STORE: {
			rtk_bt_mesh_scene_server_store_t *scene_store =
				(rtk_bt_mesh_scene_server_store_t *)cb_param;
			memcpy(scene_store->pmemory, ipc_ret, SCENE_DATA_MAX_LEN);
			break;
		}
		case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET: {
			rtk_bt_mesh_scene_server_get_t *scene_setup_get =
				(rtk_bt_mesh_scene_server_get_t *)cb_param;
			memcpy(scene_setup_get->current_scene, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES: {
			rtk_bt_mesh_scene_server_get_scenes_t *scenes_setup_get =
				(rtk_bt_mesh_scene_server_get_scenes_t *)cb_param;
			memcpy(scenes_setup_get->scenes, ipc_ret, sizeof(uint16_t) * (scenes_setup_get->num_scenes));
			break;
		}
		case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES_NUM: {
			rtk_bt_mesh_scene_server_get_scenes_num_t *num_get =
				(rtk_bt_mesh_scene_server_get_scenes_num_t *)cb_param;
			memcpy(num_get->num_scenes, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_STATUS_REGISTER: {
			rtk_bt_mesh_scene_server_get_register_status_t *status_register_get =
				(rtk_bt_mesh_scene_server_get_register_status_t *)cb_param;
			memcpy(status_register_get->status, ipc_ret, sizeof(rtk_bt_mesh_scene_status_code_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_GET: {
			rtk_bt_mesh_generic_default_transition_time_server_get_t *time_get =
				(rtk_bt_mesh_generic_default_transition_time_server_get_t *)cb_param;
			memcpy(time_get->trans_time, ipc_ret, sizeof(rtk_bt_mesh_generic_transition_time_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_GET: {
			rtk_bt_mesh_generic_level_server_get_t *level_get =
				(rtk_bt_mesh_generic_level_server_get_t *)cb_param;
			memcpy(level_get->level, ipc_ret, sizeof(int16_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_GET: {
			rtk_bt_mesh_generic_power_on_off_server_get_t *power_get =
				(rtk_bt_mesh_generic_power_on_off_server_get_t *)cb_param;
			memcpy(power_get->on_power_up, ipc_ret, sizeof(rtk_bt_mesh_generic_on_power_up_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_GET: {
			rtk_bt_mesh_generic_power_level_server_get_t *power_get =
				(rtk_bt_mesh_generic_power_level_server_get_t *)cb_param;
			memcpy(power_get->power, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_LAST_GET: {
			rtk_bt_mesh_generic_power_level_server_get_t *last_get =
				(rtk_bt_mesh_generic_power_level_server_get_t *)cb_param;
			memcpy(last_get->power, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_DEFAULT_GET: {
			rtk_bt_mesh_generic_power_level_server_get_t *default_get =
				(rtk_bt_mesh_generic_power_level_server_get_t *)cb_param;
			memcpy(default_get->power, ipc_ret, sizeof(uint16_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_RANGE_GET: {
			rtk_bt_mesh_generic_power_level_server_direct_get_range_t *range_get =
				(rtk_bt_mesh_generic_power_level_server_direct_get_range_t *)cb_param;
			memcpy(range_get->value, ipc_ret, sizeof(rtk_bt_mesh_generic_power_level_server_get_range_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_BATTERY_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_BATTERY_SERVER_MODEL_GET: {
			rtk_bt_mesh_generic_battery_server_direct_get_t *battery_get =
				(rtk_bt_mesh_generic_battery_server_direct_get_t *)cb_param;
			memcpy(battery_get->value, ipc_ret, sizeof(rtk_bt_mesh_generic_battery_server_get_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_GLOBAL_GET: {
			rtk_bt_mesh_generic_location_server_direct_get_global_t *global_get =
				(rtk_bt_mesh_generic_location_server_direct_get_global_t *)cb_param;
			memcpy(global_get->value, ipc_ret, sizeof(rtk_bt_mesh_generic_location_server_get_global_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_LOCAL_GET: {
			rtk_bt_mesh_generic_location_server_direct_get_local_t *local_get =
				(rtk_bt_mesh_generic_location_server_direct_get_local_t *)cb_param;
			memcpy(local_get->value, ipc_ret, sizeof(rtk_bt_mesh_generic_location_server_get_local_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_GLOBAL_GET: {
			rtk_bt_mesh_generic_location_server_direct_get_global_t *global_get =
				(rtk_bt_mesh_generic_location_server_direct_get_global_t *)cb_param;
			memcpy(global_get->value, ipc_ret, sizeof(rtk_bt_mesh_generic_location_server_get_global_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_LOCAL_GET: {
			rtk_bt_mesh_generic_location_server_direct_get_local_t *local_get =
				(rtk_bt_mesh_generic_location_server_direct_get_local_t *)cb_param;
			memcpy(local_get->value, ipc_ret, sizeof(rtk_bt_mesh_generic_location_server_get_local_t));
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_PARAMETER_GET: {
			rtk_bt_mesh_generic_property_server_para_get_t *user_para_get =
				(rtk_bt_mesh_generic_property_server_para_get_t *)cb_param;
			memcpy(user_para_get->pvalue, ipc_ret, sizeof(rtk_bt_mesh_generic_property_server_para_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_VALUE_GET: {
			rtk_bt_mesh_generic_property_server_value_get_t *user_val_get =
				(rtk_bt_mesh_generic_property_server_value_get_t *)cb_param;
			memcpy(user_val_get->pproperty_value, ipc_ret, user_val_get->value_len);
			break;
		}
		case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_NUM_IDS_GET: {
			rtk_bt_mesh_generic_property_server_num_ids_get_t *num_ids_get =
				(rtk_bt_mesh_generic_property_server_num_ids_get_t *)cb_param;
			memcpy(num_ids_get->pvalue, ipc_ret, sizeof(rtk_bt_mesh_generic_property_server_num_ids_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_IDS_VALUE_GET: {
			rtk_bt_mesh_generic_property_server_num_ids_value_get_t *ids_value_get =
				(rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)cb_param;
			memcpy(ids_value_get->property_ids, ipc_ret, sizeof(uint16_t) * ids_value_get->num_ids);
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL_PROPERTY_GET: {
			rtk_bt_mesh_generic_client_property_server_direct_get_t *client_ids_get =
				(rtk_bt_mesh_generic_client_property_server_direct_get_t *)cb_param;
			memcpy(client_ids_get->value, ipc_ret, ipc_ret[0] * 2 + 1);
			break;
		}
		default:
			break;
		}
    } else if (RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_PARAMETER_GET: {
			rtk_bt_mesh_generic_property_server_para_get_t *admin_para_get =
				(rtk_bt_mesh_generic_property_server_para_get_t *)cb_param;
			memcpy(admin_para_get->pvalue, ipc_ret, sizeof(rtk_bt_mesh_generic_property_server_para_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_VALUE_GET: {
			rtk_bt_mesh_generic_property_server_value_get_t *admin_val_get =
				(rtk_bt_mesh_generic_property_server_value_get_t *)cb_param;
			memcpy(admin_val_get->pproperty_value, ipc_ret, admin_val_get->value_len);
			break;
		}
		case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_NUM_IDS_GET: {
			rtk_bt_mesh_generic_property_server_num_ids_get_t *num_ids_get =
				(rtk_bt_mesh_generic_property_server_num_ids_get_t *)cb_param;
			memcpy(num_ids_get->pvalue, ipc_ret, sizeof(rtk_bt_mesh_generic_property_server_num_ids_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_IDS_VALUE_GET: {
			rtk_bt_mesh_generic_property_server_num_ids_value_get_t *ids_value_get =
				(rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)cb_param;
			memcpy(ids_value_get->property_ids, ipc_ret, sizeof(uint16_t) * ids_value_get->num_ids);
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_PARAMETER_GET: {
			rtk_bt_mesh_generic_property_server_para_get_t *manu_para_get =
				(rtk_bt_mesh_generic_property_server_para_get_t *)cb_param;
			memcpy(manu_para_get->pvalue, ipc_ret, sizeof(rtk_bt_mesh_generic_property_server_para_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_VALUE_GET: {
			rtk_bt_mesh_generic_property_server_value_get_t *manu_val_get =
				(rtk_bt_mesh_generic_property_server_value_get_t *)cb_param;
			memcpy(manu_val_get->pproperty_value, ipc_ret, manu_val_get->value_len);
			break;
		}
		case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_NUM_IDS_GET: {
			rtk_bt_mesh_generic_property_server_num_ids_get_t *num_ids_get =
				(rtk_bt_mesh_generic_property_server_num_ids_get_t *)cb_param;
			memcpy(num_ids_get->pvalue, ipc_ret, sizeof(rtk_bt_mesh_generic_property_server_num_ids_t));
			break;
		}
		case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_IDS_VALUE_GET: {
			rtk_bt_mesh_generic_property_server_num_ids_value_get_t *ids_value_get =
				(rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)cb_param;
			memcpy(ids_value_get->property_ids, ipc_ret, sizeof(uint16_t) * ids_value_get->num_ids);
			break;
		}
		default:
			break;
		}
    } else if (RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_SENSOR_SERVER_MODEL_GET: {
			rtk_bt_mesh_sensor_server_get_t *sensor_get =
				(rtk_bt_mesh_sensor_server_get_t *)cb_param;
            uint16_t data_len = MESH_LE_EXTRN2WORD(ipc_ret);
			memcpy(sensor_get->raw_data, ipc_ret, data_len);
			break;
		}
        case RTK_BT_MESH_SENSOR_SERVER_MODEL_COLUMN_GET: {
			rtk_bt_mesh_sensor_server_direct_get_column_t *column_get =
				(rtk_bt_mesh_sensor_server_direct_get_column_t *)cb_param;
            uint16_t data_len = MESH_LE_EXTRN2WORD(ipc_ret) + 2;
			memcpy(column_get->value, ipc_ret, data_len);
			break;
		}
        case RTK_BT_MESH_SENSOR_SERVER_MODEL_SERIES_GET: {
			rtk_bt_mesh_sensor_server_direct_get_series_t *series_get =
				(rtk_bt_mesh_sensor_server_direct_get_series_t *)cb_param;
            uint16_t data_len = MESH_LE_EXTRN2WORD(ipc_ret) + 2;
			memcpy(series_get->value, ipc_ret, data_len);
			break;
		}
        case RTK_BT_MESH_SENSOR_SERVER_MODEL_COMPARE_CADENCE: {
			rtk_bt_mesh_sensor_server_compare_cadence_t *cadence_get =
			    (rtk_bt_mesh_sensor_server_compare_cadence_t *)cb_param;
            memcpy(cadence_get->need_fast_divisor, ipc_ret, sizeof(bool));
            break;
	    }
		case RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_NUM_GET: {
			rtk_bt_mesh_sensor_server_get_descriptor_num_t *des_num_get =
			    (rtk_bt_mesh_sensor_server_get_descriptor_num_t *)cb_param;
            memcpy(des_num_get->descriptor_num, ipc_ret, sizeof(uint16_t));
            break;
	    }
		case RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_GET: {
			rtk_bt_mesh_sensor_server_get_descriptor_t *des_get =
			    (rtk_bt_mesh_sensor_server_get_descriptor_t *)cb_param;
            memcpy(des_get->descriptor, ipc_ret, des_get->descriptor_num * sizeof(rtk_bt_mesh_sensor_descriptor_t));
            break;
	    }
		default:
			break;
		}
    } else if (RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL == group) {
		switch (evt_code) {
		case RTK_BT_MESH_SENSOR_SERVER_MODEL_CADENCE_GET: {
			rtk_bt_mesh_sensor_server_get_cadence_t *cadence_get =
				(rtk_bt_mesh_sensor_server_get_cadence_t *)cb_param;
            uint16_t data_len = MESH_LE_EXTRN2WORD(ipc_ret);
			memcpy(cadence_get->cadence, ipc_ret, data_len);
			break;
		}
		case RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTINGS_GET: {
			rtk_bt_mesh_sensor_server_get_settings_t *settings_get =
				(rtk_bt_mesh_sensor_server_get_settings_t *)cb_param;
            uint16_t settings_num = MESH_LE_EXTRN2WORD(ipc_ret);
			memcpy(settings_get->settings_data, ipc_ret, settings_num * 2 + 2);
			break;
		}
		case RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTING_GET: {
			rtk_bt_mesh_sensor_server_get_setting_t *setting_get =
				(rtk_bt_mesh_sensor_server_get_setting_t *)cb_param;
				uint8_t raw_len = ipc_ret[1];
				memcpy(setting_get->setting_data, ipc_ret, raw_len + 2);
			break;
		}
		default:
			break;
		}
    }
	return RTK_BT_OK;
}

#endif

/* ===============================================rtk_bt_br_gap_ipc_dev action======================================= */
// static struct evt_mem_option rtk_bt_br_gap_evt_mem_op_tab[] = {
// 	{RTK_BT_BR_GAP_INQUIRY_RESULT, sizeof(rtk_bt_br_inquiry_result_t)},
// 	{RTK_BT_BR_GAP_REMOTE_NAME_RSP, sizeof(rtk_bt_br_remote_name_rsp_t)},
// 	{RTK_BT_BR_GAP_ACL_CONN_IND, 6},
// 	{RKT_BT_BR_GAP_ACL_CONN_SUCCESS, 6},
// 	{RTK_BT_BR_GAP_ACL_DISCONN, sizeof(rtk_bt_br_acl_disc_t)},
// 	{RTK_BT_BR_GAP_LINK_KEY_REQ, sizeof(uint8_t)},
// 	{RTK_BT_BR_GAP_LINK_KEY_INFO, sizeof(rtk_bt_br_bond_key_t)},
// 	{RTK_BT_BR_GAP_EVT_MAX, 0}
// };

// uint32_t get_bt_br_gap_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_br_gap_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_br_gap_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_br_gap_evt_mem_op_tab[i].evt == RTK_BT_BR_GAP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

int br_gap_ipc_inquiry_result_evt_push(uint8_t *buf, rtk_bt_br_inquiry_result_t *param,
									   uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_br_inquiry_result_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->eir_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
		if (br_gap_ipc_inquiry_result_evt_push((uint8_t *)dev_msg->param_buf, data,
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
// static struct evt_mem_option rtk_bt_a2dp_evt_mem_op_tab[] = {
// 	{RTK_BT_A2DP_EVT_CONN_IND, sizeof(rtk_bt_a2dp_conn_ind_t)},
// 	{RTK_BT_A2DP_EVT_CONN_CMPL, sizeof(rtk_bt_a2dp_conn_ind_t)},
// 	{RTK_BT_A2DP_EVT_DISCONN_CMPL, sizeof(rtk_bt_a2dp_disconn_ind_t)},
// 	{RTK_BT_A2DP_EVT_CONFIG_CMPL, sizeof(rtk_bt_a2dp_codec_t)},
// 	{RTK_BT_A2DP_EVT_STREAM_OPEN, 0},
// 	{RTK_BT_A2DP_EVT_STREAM_START_IND, sizeof(rtk_bt_a2dp_stream_start_t)},
// 	{RTK_BT_A2DP_EVT_STREAM_START_RSP, sizeof(rtk_bt_a2dp_stream_start_t)},
// 	{RTK_BT_A2DP_EVT_STREAM_STOP, sizeof(rtk_bt_a2dp_conn_ind_t)},
// 	{RTK_BT_A2DP_EVT_STREAM_CLOSE, sizeof(rtk_bt_a2dp_conn_ind_t)},
// 	{RTK_BT_A2DP_EVT_STREAM_DATA_IND, sizeof(rtk_bt_a2dp_stream_data_ind_t)},
// 	{RTK_BT_A2DP_EVT_STREAM_DATA_RSP, sizeof(rtk_bt_a2dp_stream_data_rsp_t)},
// 	{RTK_BT_A2DP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_a2dp_sdp_attr_info_t)},
// 	{RTK_BT_A2DP_EVT_MAX, 0}
// };

// uint32_t get_bt_a2dp_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_a2dp_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_a2dp_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_a2dp_evt_mem_op_tab[i].evt == RTK_BT_A2DP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

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
// static struct evt_mem_option rtk_bt_avrcp_evt_mem_op_tab[] = {
// 	{RTK_BT_AVRCP_EVT_CONN_IND, sizeof(rtk_bt_avrcp_conn_ind_t)},
// 	{RTK_BT_AVRCP_EVT_ABSOLUTE_VOLUME_SET, sizeof(rtk_bt_avrcp_absolute_volume_set_t)},
// 	{RTK_BT_AVRCP_EVT_VOLUME_CHANGED, sizeof(rtk_bt_avrcp_volume_t)},
// 	{RTK_BT_AVRCP_EVT_VOLUME_UP, 0},
// 	{RTK_BT_AVRCP_EVT_VOLUME_DOWN, 0},
// 	{RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED, sizeof(rtk_bt_avrcp_volume_change_req_t)},
// 	{RTK_BT_AVRCP_EVT_CONN_CMPL, 0},
// 	{RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED_REG_REQ, sizeof(rtk_bt_avrcp_sub_event_t)},
// 	{RTK_BT_AVRCP_EVT_PLAY_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_PAUSE_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_STOP_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_FORWARD_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_BACKWARD_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_FAST_FORWARD_START_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_FAST_FORWARD_STOP_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_REWIND_START_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_REWIND_STOP_REQ_EVENT, sizeof(rtk_bt_avrcp_digital_interface_command_t)},
// 	{RTK_BT_AVRCP_EVT_MAX, 0}
// };

// uint32_t get_bt_avrcp_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_avrcp_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_avrcp_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_avrcp_evt_mem_op_tab[i].evt == RTK_BT_AVRCP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

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
// static struct evt_mem_option rtk_bt_spp_evt_mem_op_tab[] = {
// 	{RTK_BT_SPP_EVT_CONN_COMPL, sizeof(rtk_bt_spp_conn_cmpl_t)},
// 	{RTK_BT_SPP_EVT_CREDIT_RCVD, sizeof(rtk_bt_spp_credit_rcvd_t)},
// 	{RTK_BT_SPP_EVT_DATA_IND, sizeof(rtk_bt_spp_data_ind_t)},
// 	{RTK_BT_SPP_EVT_DATA_RSP, sizeof(rtk_bt_spp_data_rsp_t)},
// 	{RTK_BT_SPP_EVT_CONN_IND, sizeof(rtk_bt_spp_conn_ind_t)},
// 	{RTK_BT_SPP_EVT_DISCONN_CMPL, sizeof(rtk_bt_spp_disconn_cmpl_t)},
// 	{RTK_BT_SPP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_spp_attr_info_t)},
// 	{RTK_BT_SPP_EVT_SDP_DISCOV_CMPL, sizeof(rtk_bt_spp_sdp_discov_cmpl_t)},
// 	{RTK_BT_SPP_EVT_MAX, 0}
// };

// uint32_t get_bt_spp_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_spp_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_spp_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_spp_evt_mem_op_tab[i].evt == RTK_BT_SPP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

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
// static struct evt_mem_option rtk_bt_sdp_evt_mem_op_tab[] = {
// 	{RTK_BT_SDP_EVT_MAX, 0}
// };

// uint32_t get_bt_sdp_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_sdp_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_sdp_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_sdp_evt_mem_op_tab[i].evt == RTK_BT_SDP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

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
// static struct evt_mem_option rtk_bt_hfp_evt_mem_op_tab[] = {
// 	{RTK_BT_HFP_EVT_SDP_ATTR_INFO, sizeof(rtk_bt_hfp_sdp_attr_info_t)},
// 	{RTK_BT_HFP_EVT_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t)},
// 	{RTK_BT_HFP_EVT_CONN_CMPL, sizeof(rtk_bt_hfp_conn_ind_t)},
// 	{RTK_BT_HFP_EVT_CALL_STATUS, sizeof(rtk_bt_hfp_call_status_ind_t)},
// 	{RTK_BT_HFP_EVT_CALLER_ID_IND, sizeof(rtk_bt_hfp_caller_id_ind_t)},
// 	{RTK_BT_HFP_EVT_DISCONN_CMPL, sizeof(rtk_bt_hfp_disconn_ind_t)},
// 	{RTK_BT_HFP_EVT_HF_BATTERY_IND, sizeof(rtk_bt_hfp_hf_battery_ind_t)},
// 	{RTK_BT_HFP_EVT_SCO_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t)},
// 	{RTK_BT_HFP_EVT_SCO_CONN_CMPL, sizeof(rtk_bt_hfp_codec_t)},
// 	{RTK_BT_HFP_EVT_SCO_DATA_IND, sizeof(rtk_bt_hfp_sco_data_ind_t)},
// 	{RTK_BT_HFP_EVT_SCO_DISCONNCTED_IND, sizeof(rtk_bt_hfp_disconn_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_CONN_IND, sizeof(rtk_bt_hfp_conn_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_CONN_CMPL, sizeof(rtk_bt_hfp_conn_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_DISCONN_CMPL, sizeof(rtk_bt_hfp_disconn_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_INDICATORS_STATUS_REQ, sizeof(rtk_bt_hfp_ag_indicators_status_req_t)},
// 	{RTK_BT_HFP_EVT_AG_CURR_CALLS_LIST_QUERY, sizeof(rtk_bt_hfp_ag_curr_calls_list_query_t)},
// 	{RTK_BT_HFP_EVT_AG_DIAL_LAST_NUMBER, sizeof(rtk_bt_hfp_ag_dial_last_number_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_CALL_ANSWER_REQ, sizeof(rtk_bt_hfp_ag_call_answer_req_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_CALL_TERMINATE_REQ, sizeof(rtk_bt_hfp_ag_call_terminate_req_ind_t)},
// 	{RTK_BT_HFP_EVT_AG_CALL_STATUS, sizeof(rtk_bt_hfp_call_status_ind_t)},
// 	{RTK_BT_HFP_EVT_MAX, 0}
// };

// uint32_t get_bt_hfp_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_hfp_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_hfp_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_hfp_evt_mem_op_tab[i].evt == RTK_BT_HFP_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

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
// static struct evt_mem_option rtk_bt_le_iso_evt_mem_op_tab[] = {
// #if (defined RTK_BLE_ISO_CIS_SUPPORT) && (RTK_BLE_ISO_CIS_SUPPORT)
// 	{RTK_BT_LE_ISO_EVT_CIG_DISCONNECT_INFO, sizeof(rtk_bt_le_iso_cig_disconnect_info_t)},
// 	{RTK_BT_LE_ISO_EVT_CIG_CIS_ESTABLISHED_INFO, sizeof(rtk_bt_le_iso_cig_cis_established_info_t)},
// 	{RTK_BT_LE_ISO_EVT_CIG_ACCEPTOR_REQUEST_CIS_IND, sizeof(rtk_bt_le_iso_cig_acceptor_request_cis_ind_t)},
// #endif
// #if (defined RTK_BLE_ISO_BIS_SUPPORT) && (RTK_BLE_ISO_BIS_SUPPORT)
// 	{RTK_BT_LE_ISO_EVT_BIG_BROADCASTER_CREATE_CMPL_INFO, sizeof(rtk_bt_le_iso_big_broadcaster_create_cmpl_info_t)},
// 	{RTK_BT_LE_ISO_EVT_BIG_BROADCASTER_SYNC_STATE_IND, sizeof(rtk_bt_le_iso_big_broadcaster_sync_state_ind_t)},
// 	{RTK_BT_LE_ISO_EVT_BIG_RECEIVER_SYNC_ESTABLISHED_INFO, sizeof(rtk_bt_le_iso_big_receiver_sync_established_info_t)},
// 	{RTK_BT_LE_ISO_EVT_BIG_RECEIVER_SYNC_STATE_IND, sizeof(rtk_bt_le_iso_big_receiver_sync_state_ind_t)},
// #endif
// 	{RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND, sizeof(rtk_bt_le_iso_direct_iso_data_ind_t)},
// 	{RTK_BT_LE_ISO_EVT_MAX, 0}
// };

// uint32_t get_bt_le_iso_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_le_iso_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_le_iso_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_le_iso_evt_mem_op_tab[i].evt == RTK_BT_LE_ISO_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

int iso_ipc_data_ind_evt_push(uint8_t *buf, rtk_bt_le_iso_direct_iso_data_ind_t *param,
							  uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint8_t *border[2] = {0};
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_iso_direct_iso_data_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);
	border[0] = end;

	size = param->buf_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
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
		ret = iso_ipc_data_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
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
#if defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT
static void le_ipc_iso_big_broadcaster_create_pop(rtk_bt_le_iso_big_broadcaster_create_param_t *param)
{
	param->p_big_param = (rtk_bt_le_iso_big_broadcaster_param_t *)((uint8_t *)param + (uint32_t)param->p_big_param);
}
static void le_ipc_iso_big_recevier_create_sync_pop(rtk_bt_le_iso_big_receiver_create_sync_t *param)
{
	param->p_sync_param = (rtk_bt_le_iso_big_receiver_create_sync_param_t *)((uint8_t *)param + (uint32_t)param->p_sync_param);
}
#endif
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
#if defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT
	case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE:
		le_ipc_iso_big_broadcaster_create_pop((rtk_bt_le_iso_big_broadcaster_create_param_t *)param);
		break;
	case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC:
		le_ipc_iso_big_recevier_create_sync_pop((rtk_bt_le_iso_big_receiver_create_sync_t *)param);
		break;
#endif
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
#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT
// static struct evt_mem_option rtk_bt_le_audio_evt_mem_op_tab[] = {
// 	{RTK_BT_LE_AUDIO_EVT_BAP_DISCOVERY_DONE_IND, sizeof(rtk_bt_le_audio_bap_discovery_done_ind_t)},
// #if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
// 	{RTK_BT_LE_AUDIO_EVT_BAP_STATE_IND, sizeof(rtk_bt_le_audio_bap_state_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND, sizeof(rtk_bt_le_audio_bap_start_qos_cfg_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND, sizeof(rtk_bt_le_audio_bap_start_metadata_cfg_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BAP_SETUP_DATA_PATH_IND, sizeof(rtk_bt_le_audio_bap_setup_data_path_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BAP_REMOVE_DATA_PATH_IND, sizeof(rtk_bt_le_audio_bap_remove_data_path_ind_t)},
// #endif
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_CP_CODEC_CFG_IND, sizeof(rtk_bt_le_audio_ascs_cp_codec_cfg_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_CP_QOS_CFG_IND, sizeof(rtk_bt_le_audio_ascs_cp_qos_cfg_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_CP_ENABLE_IND, sizeof(rtk_bt_le_audio_ascs_cp_enable_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_CP_DISABLE_IND, sizeof(rtk_bt_le_audio_ascs_cp_disable_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_ASE_STATE_IND, sizeof(rtk_bt_le_audio_ascs_ase_state_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_SETUP_DATA_PATH_IND, sizeof(rtk_bt_le_audio_ascs_setup_data_path_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ASCS_REMOVE_DATA_PATH_IND, sizeof(rtk_bt_le_audio_ascs_remove_data_path_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND, sizeof(rtk_bt_le_audio_direct_iso_data_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_DISCOVERY_DONE_IND, sizeof(rtk_bt_le_audio_mcp_client_discovery_done_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_READ_RESULT_IND, sizeof(rtk_bt_le_audio_mcp_client_read_result_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_NOTIFY_IND, sizeof(rtk_bt_le_audio_mcp_client_notify_ind_t)},
// #if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
// 	{RTK_BT_LE_AUDIO_EVT_PA_SYNC_STATE_IND, sizeof(rtk_bt_le_audio_pa_sync_state_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BIG_SYNC_STATE_IND, sizeof(rtk_bt_le_audio_big_sync_state_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_PA_BIG_INFO_IND, sizeof(rtk_bt_le_audio_pa_biginfo_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND, sizeof(rtk_bt_le_audio_pa_adv_report_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BASE_DATA_MAPPING_MODIFY_IND, sizeof(rtk_bt_le_audio_base_data_mapping_modify_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND, sizeof(rtk_bt_le_audio_bass_get_pa_sync_param_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND, sizeof(rtk_bt_le_audio_bass_get_big_sync_param_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND, sizeof(rtk_bt_le_audio_bass_get_broadcast_code_ind_t)},
// 	{RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND, sizeof(rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t)},
// #endif
// #if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
// 	{RTK_BT_LE_AUDIO_EVT_BROADCAST_SOURCE_STATE_IND, sizeof(rtk_bt_le_audio_broadcast_source_state_ind_t)},
// #endif
// 	{RTK_BT_LE_AUDIO_EVT_MAX, 0}
// };

// uint32_t get_bt_le_audio_evt_mem_size(uint8_t evt_code)
// {
// 	uint8_t i = 0;
// 	/* foreach event table */
// 	while (1) {
// 		if (rtk_bt_le_audio_evt_mem_op_tab[i].evt == evt_code) {
// 			return rtk_bt_le_audio_evt_mem_op_tab[i].size;
// 		}
// 		if (rtk_bt_le_audio_evt_mem_op_tab[i].evt == RTK_BT_LE_AUDIO_EVT_MAX) {
// 			printf("%s: cannot find matched event %d \r\n", __func__, evt_code);
// 			return 0xFFFFFFFF;
// 		}
// 		i++;
// 	}
// }

static struct evt_ret_mem_option rtk_bt_le_audio_evt_ret_mem_op_tab[] = {
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND, sizeof(rtk_bt_le_audio_bass_pa_sync_param_t)},
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND, sizeof(rtk_bt_le_audio_bass_big_sync_param_t)},
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND, RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN},
	{RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND, sizeof(uint32_t)},
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	{RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND, sizeof(rtk_bt_le_audio_unicast_session_qos_t)},
	{RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND, RTK_BT_LE_AUDIO_METADATA_MAX_LEN + sizeof(uint8_t)},
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT
	{RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND, sizeof(rtk_bt_le_audio_ascs_prefer_qos_data_t)},
#endif
	{RTK_BT_LE_AUDIO_EVT_MAX, 0}
};

uint32_t rtk_le_audio_get_evt_ret_param_size(uint16_t evt)
{
	uint8_t i = 0;
	uint32_t max_size = 0;
	/* foreach event table */
	while (1) {
		if (rtk_bt_le_audio_evt_ret_mem_op_tab[i].evt == evt) {
			max_size = rtk_bt_le_audio_evt_ret_mem_op_tab[i].size;
			break;
		}
		if (rtk_bt_le_audio_evt_ret_mem_op_tab[i].evt == RTK_BT_LE_AUDIO_EVT_MAX) {
			//printf("%s: cannot find matched evt %d \r\n", __func__, evt);
			break;
		}
		i++;
	}

	return max_size;
}


void bt_le_audio_pop_event_ipc_ret(uint16_t event, void *dst, void *src)
{
	uint8_t *ret = (uint8_t *)src;

	switch (event) {
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND: {
		rtk_bt_le_audio_bass_get_pa_sync_param_ind_t *param = (rtk_bt_le_audio_bass_get_pa_sync_param_ind_t *)dst;
		memcpy((void *)param->p_pa_sync_param, (void *)ret, rtk_le_audio_get_evt_ret_param_size(event));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND: {
		rtk_bt_le_audio_bass_get_big_sync_param_ind_t *param = (rtk_bt_le_audio_bass_get_big_sync_param_ind_t *)dst;
		memcpy((void *)param->p_big_sync_param, (void *)ret, rtk_le_audio_get_evt_ret_param_size(event));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND: {
		rtk_bt_le_audio_bass_get_broadcast_code_ind_t *param = (rtk_bt_le_audio_bass_get_broadcast_code_ind_t *)dst;
		memcpy((void *)param->p_broadcast_code, (void *)ret, rtk_le_audio_get_evt_ret_param_size(event));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND: {
		rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t *param = (rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t *)dst;
		memcpy((void *)param->p_support_bis_array, (void *)ret, rtk_le_audio_get_evt_ret_param_size(event));
		break;
	}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND: {
		rtk_bt_le_audio_bap_start_qos_cfg_ind_t *param = (rtk_bt_le_audio_bap_start_qos_cfg_ind_t *)dst;
		memcpy((void *)param->p_session_qos, (void *)ret, rtk_le_audio_get_evt_ret_param_size(event));
		break;
	}
	case RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND: {
		rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *param = (rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *)dst;
		memcpy((void *)param->p_metadata_len, (void *)ret, sizeof(uint8_t));
		memcpy((void *)param->p_metadata, (void *)(ret + 1), *param->p_metadata_len);
		break;
	}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT) && RTK_BLE_AUDIO_UNICAST_SERVER_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND: {
		rtk_bt_le_audio_ascs_get_prefer_qos_ind_t *param = (rtk_bt_le_audio_ascs_get_prefer_qos_ind_t *)dst;
		memcpy((void *)param->p_prefer_qos_data, (void *)ret, rtk_le_audio_get_evt_ret_param_size(event));
		break;
	}
#endif
	default:
		break;
	}
	(void)ret;
	(void)dst;
}

int le_audio_ipc_iso_data_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_direct_iso_data_ind_t *param,
									   uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;

	size = sizeof(rtk_bt_le_audio_direct_iso_data_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->buf_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_buf, size);
	((rtk_bt_le_audio_direct_iso_data_ind_t *)buf)->p_buf = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_direct_iso_data_ind_t size is over deep copy buf\r\n");
	return -1;
}
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
int le_audio_ipc_mcp_client_read_result_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_mcp_client_read_result_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_mcp_client_read_result_ind_t *p_ind = (rtk_bt_le_audio_mcp_client_read_result_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_mcp_client_read_result_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	switch (param->char_uuid) {
	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
		size = param->data.media_player_name.media_player_name_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.media_player_name.p_media_player_name, size);
		p_ind->data.media_player_name.p_media_player_name = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
		break;

	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE:
		size = param->data.track_title.track_title_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.track_title.p_track_title, size);
		p_ind->data.track_title.p_track_title = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
		break;

	default:
		break;
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_mcp_client_read_result_ind_t size is over deep copy buf\r\n");
	return -1;
}
int le_audio_ipc_mcs_client_notify_result_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_mcp_client_notify_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_mcp_client_notify_ind_t *p_ind = (rtk_bt_le_audio_mcp_client_notify_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_mcp_client_notify_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	switch (param->char_uuid) {
	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
		size = param->data.media_player_name.media_player_name_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.media_player_name.p_media_player_name, size);
		p_ind->data.media_player_name.p_media_player_name = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
		break;

	case RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE:
		size = param->data.track_title.track_title_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.track_title.p_track_title, size);
		p_ind->data.track_title.p_track_title = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
		break;

	default:
		break;
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("rtk_bt_le_audio_mcp_client_read_result_ind_t size is over deep copy buf\r\n");
	return -1;
}
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
int le_audio_ipc_pa_adv_report_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_pa_adv_report_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_pa_adv_report_ind_t *buf_ind = (rtk_bt_le_audio_pa_adv_report_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_pa_adv_report_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->data_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_data, size);
	buf_ind->p_data = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_pa_adv_report_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
int le_audio_ipc_bass_client_brs_data_ind_push(uint8_t *buf, rtk_bt_le_audio_bass_client_brs_data_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bass_client_brs_data_ind_t *buf_ind = (rtk_bt_le_audio_bass_client_brs_data_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_bass_client_brs_data_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	if (param->p_brs_data) {
		if (param->p_brs_data->bis_info_size) {
			for (uint8_t i = 0; i < param->p_brs_data->num_subgroups; i++) {
				//copy p_metadata
				if (param->p_brs_data->p_cp_bis_info[i].metadata_len) {
					size = param->p_brs_data->p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->p_brs_data->p_cp_bis_info[i].p_metadata, size);
					param->p_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}
			//copy p_cp_bis_info
			size = param->p_brs_data->bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->p_brs_data->p_cp_bis_info, size);
			param->p_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}

		//copy p_brs_data
		size = sizeof(rtk_bt_le_audio_bass_brs_data_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->p_brs_data, size);
		buf_ind->p_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_bass_client_brs_data_ind_push size is over deep copy buf\r\n");
	return -1;
}

int le_audio_ipc_bass_client_sync_info_req_ind_push(uint8_t *buf, rtk_bt_le_audio_bass_client_sync_info_req_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bass_client_sync_info_req_ind_t *buf_ind = (rtk_bt_le_audio_bass_client_sync_info_req_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_bass_client_sync_info_req_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	if (param->p_brs_data) {
		if (param->p_brs_data->bis_info_size) {
			for (uint8_t i = 0; i < param->p_brs_data->num_subgroups; i++) {
				//copy p_metadata
				if (param->p_brs_data->p_cp_bis_info[i].metadata_len) {
					size = param->p_brs_data->p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->p_brs_data->p_cp_bis_info[i].p_metadata, size);
					param->p_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}
			//copy p_cp_bis_info
			size = param->p_brs_data->bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->p_brs_data->p_cp_bis_info, size);
			param->p_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}
		//copy p_brs_data
		size = sizeof(rtk_bt_le_audio_bass_brs_data_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->p_brs_data, size);
		buf_ind->p_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_bass_client_sync_info_req_ind_push size is over deep copy buf\r\n");
	return -1;
}
#endif
#if defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT
int le_audio_ipc_bass_cp_ind_push(uint8_t *buf, rtk_bt_le_audio_bass_cp_ind_t *param,
								  uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bass_cp_ind_t *buf_ind = (rtk_bt_le_audio_bass_cp_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_bass_cp_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	switch (param->opcode) {
	case RTK_BT_LE_AUDIO_BASS_CP_ADD_SOURCE: {
		if (param->op_param.add_source_param.bis_info_size) {
			for (uint8_t i = 0; i < param->op_param.add_source_param.num_subgroups; i++) {
				//copy p_metadata
				if (param->op_param.add_source_param.p_cp_bis_info[i].metadata_len) {
					size = param->op_param.add_source_param.p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->op_param.add_source_param.p_cp_bis_info[i].p_metadata, size);
					param->op_param.add_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}
			//copy p_cp_bis_info
			size = param->op_param.add_source_param.bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->op_param.add_source_param.p_cp_bis_info, size);
			buf_ind->op_param.add_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}

		break;
	}
	case RTK_BT_LE_AUDIO_BASS_CP_MODIFY_SOURCE: {
		if (param->op_param.modify_source_param.bis_info_size) {
			for (uint8_t i = 0; i < param->op_param.modify_source_param.num_subgroups; i++) {
				//copy p_metadata
				if (param->op_param.modify_source_param.p_cp_bis_info[i].metadata_len) {
					size = param->op_param.modify_source_param.p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata, size);
					param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}
			//copy p_cp_bis_info
			size = param->op_param.modify_source_param.bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->op_param.modify_source_param.p_cp_bis_info, size);
			buf_ind->op_param.modify_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}
		break;
	}
	default:
		break;
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_bass_cp_ind_push size is over deep copy buf\r\n");
	return -1;
}

int le_audio_ipc_bass_brs_data_ind_push(uint8_t *buf, rtk_bt_le_audio_bass_brs_modify_ind_t *param,
										uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bass_brs_modify_ind_t *buf_ind = (rtk_bt_le_audio_bass_brs_modify_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_bass_brs_modify_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	if (param->p_brs_data) {
		if (param->p_brs_data->bis_info_size) {
			for (uint8_t i = 0; i < param->p_brs_data->num_subgroups; i++) {
				//copy p_metadata
				if (param->p_brs_data->p_cp_bis_info[i].metadata_len) {
					size = param->p_brs_data->p_cp_bis_info[i].metadata_len;
					if (end + size > buf + size_limit) {
						goto exit;
					}
					memcpy(end, param->p_brs_data->p_cp_bis_info[i].p_metadata, size);
					param->p_brs_data->p_cp_bis_info[i].p_metadata = (uint8_t *)(end - buf);
					end += ALIGN_UP(size, 4);
				}
			}
			//copy p_cp_bis_info
			size = param->p_brs_data->bis_info_size;
			if (end + size > buf + size_limit) {
				goto exit;
			}
			memcpy(end, param->p_brs_data->p_cp_bis_info, size);
			param->p_brs_data->p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)(end - buf);
			end += ALIGN_UP(size, 4);
		}

		//copy p_brs_data
		size = sizeof(rtk_bt_le_audio_bass_brs_data_t);
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->p_brs_data, size);
		buf_ind->p_brs_data = (rtk_bt_le_audio_bass_brs_data_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_bass_brs_data_ind_push size is over deep copy buf\r\n");
	return -1;
}
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
int le_audio_ipc_bap_start_metadata_cfg_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *buf_ind = (rtk_bt_le_audio_bap_start_metadata_cfg_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_bap_start_metadata_cfg_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = *param->p_metadata_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->p_metadata, size);
	buf_ind->p_metadata = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_bap_start_metadata_cfg_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
int le_audio_ipc_vocs_write_output_des_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_vocs_write_output_des_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_vocs_write_output_des_ind_t *buf_ind = (rtk_bt_le_audio_vocs_write_output_des_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_vocs_write_output_des_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->output_des.output_des_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->output_des.p_output_des, size);
	buf_ind->output_des.p_output_des = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_vocs_write_output_des_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
int le_audio_ipc_vocs_client_read_result_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_vocs_client_read_result_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_vocs_client_read_result_ind_t *buf_ind = (rtk_bt_le_audio_vocs_client_read_result_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_vocs_client_read_result_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	//only RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC need push
	if (param->type == RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC) {
		size = param->data.output_des.output_des_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.output_des.p_output_des, size);
		buf_ind->data.output_des.p_output_des = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_vocs_client_read_result_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
int le_audio_ipc_vocs_client_notify_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_vocs_client_notify_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_vocs_client_notify_ind_t *buf_ind = (rtk_bt_le_audio_vocs_client_notify_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_vocs_client_notify_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	//only RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC need push
	if (param->type == RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC) {
		size = param->data.output_des.output_des_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.output_des.p_output_des, size);
		buf_ind->data.output_des.p_output_des = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_vocs_client_notify_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
int le_audio_ipc_aics_write_input_des_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_aics_write_input_des_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_aics_write_input_des_ind_t *buf_ind = (rtk_bt_le_audio_aics_write_input_des_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_aics_write_input_des_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	size = param->input_des.input_des_len;
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param->input_des.p_input_des, size);
	buf_ind->input_des.p_input_des = (uint8_t *)(end - buf);
	end += ALIGN_UP(size, 4);

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_aics_write_input_des_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
int le_audio_ipc_aics_client_read_result_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_aics_client_read_result_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_aics_client_read_result_ind_t *buf_ind = (rtk_bt_le_audio_aics_client_read_result_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_aics_client_read_result_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	//only RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES need push
	if (param->type == RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES) {
		size = param->data.input_des.input_des_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.input_des.p_input_des, size);
		buf_ind->data.input_des.p_input_des = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_aics_client_read_result_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
int le_audio_ipc_aics_client_notify_ind_evt_push(uint8_t *buf, rtk_bt_le_audio_aics_client_notify_ind_t *param,
		uint32_t size_limit, uint32_t *actual_size)
{
	uint8_t *end = buf;
	uint32_t size = 0;
	rtk_bt_le_audio_aics_client_notify_ind_t *buf_ind = (rtk_bt_le_audio_aics_client_notify_ind_t *)buf;

	size = sizeof(rtk_bt_le_audio_aics_client_notify_ind_t);
	if (end + size > buf + size_limit) {
		goto exit;
	}
	memcpy(end, param, size);
	end += ALIGN_UP(size, 4);

	//only RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES need push
	if (param->type == RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES) {
		size = param->data.input_des.input_des_len;
		if (end + size > buf + size_limit) {
			goto exit;
		}
		memcpy(end, param->data.input_des.p_input_des, size);
		buf_ind->data.input_des.p_input_des = (uint8_t *)(end - buf);
		end += ALIGN_UP(size, 4);
	}

	*actual_size = (uint32_t)(end - buf);
	return 0;

exit:
	printf("le_audio_ipc_aics_client_notify_ind_evt_push size is over deep copy buf\r\n");
	return -1;
}
#endif
void *bt_le_audio_push_event_ipc_buf(uint16_t event, void *data, uint32_t buf_len, uint32_t *pactual_size)
{
	int ret = 0;
	bt_ipc_dev_request_message *dev_msg = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(*dev_msg));
	if (!dev_msg) {
		printf("%s allocate dev_msg fail \r\n", __func__);
		return NULL;
	}
	memset(dev_msg, 0, sizeof(*dev_msg));

	switch (event) {
	case RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND:
		ret = le_audio_ipc_iso_data_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_READ_RESULT_IND:
		ret = le_audio_ipc_mcp_client_read_result_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_NOTIFY_IND:
		ret = le_audio_ipc_mcs_client_notify_result_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_PA_ADV_REPORT_IND:
		ret = le_audio_ipc_pa_adv_report_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BASS_CLIENT_SYNC_INFO_REQ_IND:
		le_audio_ipc_bass_client_sync_info_req_ind_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_BASS_CLIENT_BRS_DATA_IND:
		ret = le_audio_ipc_bass_client_brs_data_ind_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
#if defined(RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT) && RTK_BLE_AUDIO_SCAN_DELEGATOR_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BASS_CP_IND:
		le_audio_ipc_bass_cp_ind_push((uint8_t *)dev_msg->param_buf, data,
									  IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_BASS_BRS_MODIFY_IND:
		ret = le_audio_ipc_bass_brs_data_ind_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND:
		ret = le_audio_ipc_bap_start_metadata_cfg_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_VOCS_WRITE_OUTPUT_DES_IND:
		ret = le_audio_ipc_vocs_write_output_des_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND:
		ret = le_audio_ipc_vocs_client_read_result_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_NOTIFY_IND:
		ret = le_audio_ipc_vocs_client_notify_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	case RTK_BT_LE_AUDIO_EVT_AICS_WRITE_INPUT_DES_IND:
		ret = le_audio_ipc_aics_write_input_des_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_READ_RESULT_IND:
		ret = le_audio_ipc_aics_client_read_result_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
	case RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_NOTIFY_IND:
		ret = le_audio_ipc_aics_client_notify_ind_evt_push((uint8_t *)dev_msg->param_buf, data,
				IPC_DEV_API_DATA_MAX, pactual_size);
		break;
#endif
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

static void le_audio_ipc_iso_data_send_pop(rtk_bt_le_audio_iso_data_send_info_t *param)
{
	param->p_data = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_data);
}

static void le_audio_ipc_pacs_get_pac_record_pop(rtk_bt_le_audio_pacs_get_pac_record_param_t *param)
{
	param->p_pac_num = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_pac_num);
}
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
static void le_audio_ipc_sync_setup_path_pop(rtk_bt_le_audio_sync_setup_data_path_param_t *param)
{
	param->setup_param.p_codec_config = (uint8_t *)((uint8_t *)param + (uint32_t)param->setup_param.p_codec_config);
}
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
static void le_audio_ipc_broadcast_source_create_pop(rtk_bt_le_audio_broadcast_source_create_param_t *param)
{
	uint8_t i = 0, j = 0, k = 0;

	for (i = 0; i < param->num_groups; i++) {
		for (j = 0; j < param->group[i].num_subgroups; j++) {
			//restore codec_cfg_l2
			param->group[i].subgroup[j].p_codec_cfg_l2 = (uint8_t *)((uint8_t *)param + (uint32_t)param->group[i].subgroup[j].p_codec_cfg_l2);

			//restore metadata
			param->group[i].subgroup[j].p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)param->group[i].subgroup[j].p_metadata);

			for (k = 0; k < param->group[i].subgroup[j].num_bis; k++) {
				//restore p_codec_cfg_l3
				param->group[i].subgroup[j].bis[k].p_codec_cfg_l3 = (uint8_t *)((uint8_t *)param + (uint32_t)param->group[i].subgroup[j].bis[k].p_codec_cfg_l3);
			}
		}
	}
}
static void le_audio_ipc_broadcast_source_setup_path_pop(rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *param)
{
	param->setup_param.p_codec_config = (uint8_t *)((uint8_t *)param + (uint32_t)param->setup_param.p_codec_config);
}
static void le_audio_ipc_broadcast_source_reconfig_pop(rtk_bt_le_audio_broadcast_source_reconfig_param_t *param)
{
	param->p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_metadata);
}
#endif

#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
static void le_audio_ipc_bass_client_write_cp_op_pop(rtk_bt_le_audio_bass_client_write_cp_op_param_t *param)
{
	switch (param->opcode) {
	case RTK_BT_LE_AUDIO_BASS_CP_ADD_SOURCE:
		param->op_param.add_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)param + (uint32_t)
				param->op_param.add_source_param.p_cp_bis_info);
		for (uint8_t i = 0; i < param->op_param.add_source_param.num_subgroups; i++) {
			param->op_param.add_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)
					param->op_param.add_source_param.p_cp_bis_info[i].p_metadata);
		}
		break;

	case RTK_BT_LE_AUDIO_BASS_CP_MODIFY_SOURCE:
		param->op_param.modify_source_param.p_cp_bis_info = (rtk_bt_le_audio_bass_cp_bis_info_t *)((uint8_t *)param + (uint32_t)
				param->op_param.modify_source_param.p_cp_bis_info);
		for (uint8_t i = 0; i < param->op_param.modify_source_param.num_subgroups; i++) {
			param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)
					param->op_param.modify_source_param.p_cp_bis_info[i].p_metadata);
		}
		break;

	default:
		break;
	}
}
#endif

#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
static void le_audio_ipc_unicast_config_pop(rtk_bt_le_audio_unicast_config_param_t *param)
{
	param->p_device_handle_tbl = (rtk_bt_le_audio_device_handle_t *)((uint8_t *)param + (uint32_t)param->p_device_handle_tbl);
}
static void le_audio_ipc_unicast_get_cfg_mask_pop(rtk_bt_le_audio_unicast_get_cfg_mask_param_t *param)
{
	param->p_device_handle_tbl = (rtk_bt_le_audio_device_handle_t *)((uint8_t *)param + (uint32_t)param->p_device_handle_tbl);
}
#if 0
static void le_audio_ipc_unicast_cfg_ase_metadata_pop(rtk_bt_le_audio_unicast_cfg_ase_metadata_param_t *param)
{
	param->p_metadata = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_metadata);
}
#endif
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
static void le_audio_ipc_csis_set_coordinator_check_adv_rsi_pop(rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *param)
{
	param->p_report_data = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_report_data);
}
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
static void le_audio_ipc_mcp_server_send_data_pop(rtk_bt_le_audio_mcp_server_send_data_param_t *param)
{
	if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME) {
		param->param.media_player_name.p_media_player_name = (uint8_t *)((uint8_t *)param + (uint32_t)param->param.media_player_name.p_media_player_name);
	} else if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE) {
		param->param.track_title.p_track_title = (uint8_t *)((uint8_t *)param + (uint32_t)param->param.track_title.p_track_title);
	}
}
static void le_audio_ipc_mcp_server_read_confirm_pop(rtk_bt_le_audio_mcp_server_read_confirm_param_t *param)
{
	if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME) {
		param->param.media_player_name.p_media_player_name = (uint8_t *)((uint8_t *)param + (uint32_t)param->param.media_player_name.p_media_player_name);
	} else if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE) {
		param->param.track_title.p_track_title = (uint8_t *)((uint8_t *)param + (uint32_t)param->param.track_title.p_track_title);
	} else if (param->char_uuid == RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_ICON_URL) {
		param->param.media_player_icon_url.p_media_player_icon_url = (uint8_t *)((uint8_t *)param + (uint32_t)
				param->param.media_player_icon_url.p_media_player_icon_url);
	}
}
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
static void le_audio_ipc_vocs_set_param_pop(rtk_bt_le_audio_vocs_set_param_t *param)
{
	param->vocs_param.output_des.p_output_des = (uint8_t *)((uint8_t *)param + (uint32_t)param->vocs_param.output_des.p_output_des);
}
static void le_audio_ipc_vocs_write_output_des_pop(rtk_bt_le_audio_vocs_write_output_des_param_t *param)
{
	param->p_output_des = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_output_des);
}
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
static void le_audio_ipc_aics_set_param_pop(rtk_bt_le_audio_aics_set_param_t *param)
{
	param->p_value = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_value);
}
static void le_audio_ipc_aics_write_input_des_pop(rtk_bt_le_audio_aics_write_input_des_param_t *param)
{
	param->p_input_des = (uint8_t *)((uint8_t *)param + (uint32_t)param->p_input_des);
}
#endif

void bt_le_audio_ipc_pop_cmd_param(uint8_t act, void *param)
{
	switch (act) {
	case RTK_BT_LE_AUDIO_ACT_ISO_DATA_SEND:
		le_audio_ipc_iso_data_send_pop((rtk_bt_le_audio_iso_data_send_info_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_PACS_GET_PAC_RECORD:
		le_audio_ipc_pacs_get_pac_record_pop((rtk_bt_le_audio_pacs_get_pac_record_param_t *)param);
		break;
#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_SYNC_SETUP_DATA_PATH:
		le_audio_ipc_sync_setup_path_pop((rtk_bt_le_audio_sync_setup_data_path_param_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_CREATE:
		le_audio_ipc_broadcast_source_create_pop((rtk_bt_le_audio_broadcast_source_create_param_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_SETUP_DATA_PATH:
		le_audio_ipc_broadcast_source_setup_path_pop((rtk_bt_le_audio_broadcast_source_setup_data_path_param_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_BROADCAST_SOURCE_RECONFIG:
		le_audio_ipc_broadcast_source_reconfig_pop((rtk_bt_le_audio_broadcast_source_reconfig_param_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_BASS_CLIENT_WRITE_CP_OP:
		le_audio_ipc_bass_client_write_cp_op_pop((rtk_bt_le_audio_bass_client_write_cp_op_param_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_UNICAST_CONFIG:
		le_audio_ipc_unicast_config_pop((rtk_bt_le_audio_unicast_config_param_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_UNICAST_GET_CFG_MASK:
		le_audio_ipc_unicast_get_cfg_mask_pop((rtk_bt_le_audio_unicast_get_cfg_mask_param_t *)param);
		break;
#if 0
	case RTK_BT_LE_AUDIO_ACT_UNICAST_CFG_ASE_METADATA:
		le_audio_ipc_unicast_cfg_ase_metadata_pop((rtk_bt_le_audio_unicast_cfg_ase_metadata_param_t *)param);
		break;
#endif
#endif
#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_CSIS_SET_COORDINATOR_CHECK_ADV_RSI:
		le_audio_ipc_csis_set_coordinator_check_adv_rsi_pop((rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_SEND_DATA:
		le_audio_ipc_mcp_server_send_data_pop((rtk_bt_le_audio_mcp_server_send_data_param_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_MCP_SERVER_READ_CFM:
		le_audio_ipc_mcp_server_read_confirm_pop((rtk_bt_le_audio_mcp_server_read_confirm_param_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_VOCS_SET_PARAM:
		le_audio_ipc_vocs_set_param_pop((rtk_bt_le_audio_vocs_set_param_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_VOCS_WRITE_OUTPUT_DES:
		le_audio_ipc_vocs_write_output_des_pop((rtk_bt_le_audio_vocs_write_output_des_param_t *)param);
		break;
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
	case RTK_BT_LE_AUDIO_ACT_AICS_SET_PARAM:
		le_audio_ipc_aics_set_param_pop((rtk_bt_le_audio_aics_set_param_t *)param);
		break;
	case RTK_BT_LE_AUDIO_ACT_AICS_WRITE_INPUT_DES:
		le_audio_ipc_aics_write_input_des_pop((rtk_bt_le_audio_aics_write_input_des_param_t *)param);
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
void bt_le_audio_pop_event_ipc_ret(uint16_t event, void *dst, void *src)
{
	(void)event;
	(void)dst;
	(void)src;
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