/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <platform_opts_bt.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gattc.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_cfg.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_host_drc.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_dev_drc.h>
#include <bt_ipc_profile_config.h>
#include <bt_ipc_parameter_conversion.h>
#include <rtk_bt_gatts.h>
#else
#include <rtk_bt_def.h>
#endif
#include <rtk_bt_device.h>
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <rtk_bt_mesh_common.h>
#include <rtk_bt_mesh_datatrans_model.h>
#include <rtk_bt_mesh_light_model.h>
#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_onoff_model.h>
#include <rtk_bt_mesh_time_model.h>
#include <rtk_bt_mesh_scheduler_model.h>
#include <rtk_bt_mesh_scene_model.h>
#include <rtk_bt_mesh_generic_default_transition_time.h>
#include <rtk_bt_mesh_generic_model.h>
#include <rtk_bt_mesh_sensor_model.h>
#include <rtk_bt_mesh_health_model.h>
#endif


extern struct amebad2_uart_t *amebad2_uart;



#define EVENT_NUM 64
/* NOTE: If user's event callback need more stack size, please enlarge it */
#define EVENT_TASK_STACK_SIZE 4096
#define EVENT_TASK_PRIORITY 4

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
/* Internal definition for which act shall direct calling. */
static uint32_t rtk_bt_common_act_direct_calling_flag = 0;
static uint32_t rtk_bt_device_act_direct_calling_flag =
	(1 << RTK_BT_DEVICE_IPC_ACT_BT_DISABLE);
static uint32_t rtk_bt_mp_act_direct_calling_flag = 0;
static uint32_t rtk_bt_power_act_direct_calling_flag = 0;
static uint32_t rtk_bt_vendor_act_direct_calling_flag = 0;
static uint32_t rtk_bt_gap_act_direct_calling_flag = 0;
static uint32_t rtk_bt_le_gap_act_direct_calling_flag = 0;
static uint32_t rtk_bt_le_gatts_act_direct_calling_flag = 0;
static uint32_t rtk_bt_le_gattc_act_direct_calling_flag = 0;
static uint32_t rtk_bt_le_iso_act_direct_calling_flag = 0;
static uint32_t rtk_bt_le_audio_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_gap_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_a2dp_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_hfp_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_avrcp_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_sdp_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_spp_act_direct_calling_flag = 0;
static uint32_t rtk_bt_br_hid_act_direct_calling_flag = 0;
#endif

/* Internal definition for which evt callback shall direct calling. */
static uint32_t rtk_bt_le_gap_evt_direct_calling_flag =
	(1 << RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND);
static uint32_t rtk_bt_le_gatts_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_le_gattc_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_le_iso_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_br_avrcp_evt_direct_calling_flag =
	(1 << RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED);
static uint32_t rtk_bt_br_a2dp_evt_direct_calling_flag =
	(1 << RTK_BT_A2DP_EVT_STREAM_DATA_IND);
static uint32_t rtk_bt_br_hfp_evt_direct_calling_flag =
	((1 << RTK_BT_HFP_EVT_SCO_DATA_IND) |
	 (1 << RTK_BT_HFP_EVT_HF_BATTERY_IND));
static uint32_t rtk_bt_le_audio_evt_direct_calling_flag =
	((1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_BAP_START_QOS_CFG_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_BAP_START_METADATA_CFG_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND));
static uint32_t rtk_bt_gap_evt_direct_calling_flag =
	(1 << RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND);

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
static uint32_t rtk_bt_mesh_generic_onoff_server_model_evt_direct_calling_flag =
	(1 << RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_GET);
static uint32_t rtk_bt_mesh_datatrans_model_evt_direct_calling_flag =
	(1 << RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ);
#endif

#if !defined(CONFIG_BT_AP) || (!CONFIG_BT_AP)
extern uint16_t bt_stack_api_send(void *pcmd);
#if !defined(CONFIG_BT_ZEPHYR) || (!CONFIG_BT_ZEPHYR)
extern void bt_stack_pending_cmd_delete(rtk_bt_cmd_t *p_cmd);
#endif
#endif

static void *g_evt_task_sem = NULL;
static void *g_evt_queue = NULL;
static void *g_evt_task_hdl = NULL;
#if defined (CONFIG_BT_AP) && CONFIG_BT_AP
/* for directly calling using */
static void *g_drc_evt_task_sem = NULL;
static void *g_drc_evt_queue = NULL;
static void *g_drc_evt_task_hdl = NULL;
#endif
bool event_task_running = false;
uint32_t event_task_msg_num = 0;

rtk_bt_evt_cb_t rtk_bt_le_evt_cb_tbl[RTK_BT_LE_GP_MAX - RTK_BT_API_LE_BASE] = {0};
rtk_bt_evt_cb_t rtk_bt_br_evt_cb_tbl[RTK_BT_BR_GP_MAX - RTK_BT_API_BR_BASE] = {0};
rtk_bt_evt_cb_t rtk_bt_evt_cb_tbl[RTK_BT_COMMON_GP_MAX - RTK_BT_API_COMMON_BASE] = {0};

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP

bool rtk_bt_check_act_direct_calling(uint8_t group, uint8_t act_code)
{
	bool ret = false;

	switch (group) {
	case RTK_BT_IPC_COMMON:
		if (rtk_bt_common_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_DEVICE:
		if (rtk_bt_device_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_MP_API:
		if (rtk_bt_mp_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_POWER_STATE:
		if (rtk_bt_power_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_VENDOR:
		if (rtk_bt_vendor_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_GAP:
		if (rtk_bt_gap_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_GAP_BLE:
		if (rtk_bt_le_gap_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_GATTS:
		if (rtk_bt_le_gatts_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_GATTC:
		if (rtk_bt_le_gattc_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_LE_ISO:
		if (rtk_bt_le_iso_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_LE_AUDIO:
		if (rtk_bt_le_audio_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_GAP_BR:
		if (rtk_bt_br_gap_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_A2DP:
		if (rtk_bt_br_a2dp_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_HFP:
		if (rtk_bt_br_hfp_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_AVRCP:
		if (rtk_bt_br_avrcp_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_SDP:
		if (rtk_bt_br_sdp_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_SPP:
		if (rtk_bt_br_spp_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	case RTK_BT_IPC_HID:
		if (rtk_bt_br_hid_act_direct_calling_flag & (1 << act_code)) {
			ret = true;
		}
		break;
	default:
		break;
	}

	return ret;
}

static uint16_t rtk_bt_gap_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_gap_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_GAP, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_GAP, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GAP, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_gap_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_le_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	if (act == RTK_BT_LE_GAP_ACT_GET_BOND_INFO) {
		max_return_size = len;
	} else {
		max_return_size = rtk_le_get_act_ret_param_size(act);
	}
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_le_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_GAP_BLE, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_GAP_BLE, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GAP_BLE, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_le_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_gatts_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_gatts_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	host_msg = (bt_ipc_host_request_message *)bt_gatts_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s gatts get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_GATTS, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_GATTS, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GATTS, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_gatts_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);

	return err;
}

static uint16_t rtk_bt_gattc_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_gattc_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	host_msg = (bt_ipc_host_request_message *)bt_gattc_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s gatts get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_GATTC, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_GATTC, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GATTC, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_gattc_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);

	return err;
}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
static uint16_t rtk_bt_mesh_set_act(uint32_t group, uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;

	/* check return size whether enough */
	max_return_size = rtk_mesh_get_act_ret_param_size(group, act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (true == rtk_bt_check_act_direct_calling(group, act)) {
		ret = bt_ipc_drc_host_message_send(group, act, data, len);
	} else {
		ret = bt_ipc_api_host_message_send(group, act, data, len);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! ret:0x%x\r\n", __func__, ret[0]);
	} else {
		/* check whether need memcpy return value */
		rtk_mesh_copy_act_ret_param(group, act, data, &ret[1], len);
	}
	err = (uint16_t)ret[0] | RTK_BT_STACK_MESH_ERROR_FLAG;
	osif_mem_free(ret);

	return err;
}
#endif

static uint16_t rtk_bt_br_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_br_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_br_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_GAP_BR, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_GAP_BR, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GAP_BR, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_br_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_a2dp_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_a2dp_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_a2dp_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_A2DP, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_A2DP, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_A2DP, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_a2dp_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_hfp_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_hfp_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_hfp_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_HFP, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_HFP, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_HFP, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_hfp_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_avrcp_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_avrcp_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_avrcp_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_AVRCP, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_AVRCP, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_AVRCP, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_avrcp_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_sdp_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_sdp_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_sdp_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_SDP, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_SDP, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SDP, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_sdp_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_le_iso_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_le_iso_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_le_iso_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_LE_ISO, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_LE_ISO, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_LE_ISO, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_le_iso_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_le_audio_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_le_audio_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_le_audio_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_LE_AUDIO, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_LE_AUDIO, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_LE_AUDIO, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_le_audio_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_spp_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_spp_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_spp_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_SPP, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_SPP, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SPP, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_spp_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t rtk_bt_hid_set_act(uint16_t act, void *data, uint32_t len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_FAIL;
	uint32_t max_return_size = 0;
	uint32_t actual_size = 0;
	bt_ipc_host_request_message *host_msg = NULL;

	/* check return size whether enough */
	max_return_size = rtk_hid_get_act_ret_param_size(act);
	if (max_return_size > IPC_HOST_API_RET_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	if (len > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		return err;
	}
	/* depy copy secondary memory pointed by pointer */
	host_msg = (bt_ipc_host_request_message *)bt_hid_push_cmd_ipc_buf(act, data, len, &actual_size);
	if (!host_msg) {
		printf("%s le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_HOST_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, act);
		goto exit;
	}
	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_HID, act)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_HID, act, host_msg->param_buf, actual_size);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_HID, act, host_msg->param_buf, actual_size);
	}
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	} else {
		/* check whether need memcpy return value */
		rtk_hid_copy_act_ret_param(act, data, &ret[1]);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(host_msg);
	return err;
}

static uint16_t bt_send_ipc_cmd(uint8_t group, uint8_t act, void *param, uint32_t param_len)
{
	uint16_t err = RTK_BT_FAIL;

	switch (group) {
	case RTK_BT_LE_GP_GAP:
		err = rtk_bt_le_set_act(act, param, param_len);
		break;

	case RTK_BT_LE_GP_GATTC:
		err = rtk_bt_gattc_set_act(act, param, param_len);
		break;

	case RTK_BT_LE_GP_GATTS:
		err = rtk_bt_gatts_set_act(act, param, param_len);
		break;

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	case RTK_BT_LE_GP_MESH_STACK:
		group = RTK_BT_IPC_MESH_STACK;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#if defined(BT_MESH_ENABLE_DATATRANS_MODEL) && BT_MESH_ENABLE_DATATRANS_MODEL
	case RTK_BT_LE_GP_MESH_DATATRANS_MODEL:
		group = RTK_BT_IPC_MESH_DATATRANS_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	case RTK_BT_LE_GP_MESH_CONFIG_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_CONFIG_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#if defined(BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_ONOFF_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_ONOFF_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL) && BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_REMOTE_PROV_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_REMOTE_PROV_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_LIGHT_CTL_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_LIGHT_HSL_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_LIGHT_XYL_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_LIGHT_LC_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_TIME_CLIENT_MODEL) && BT_MESH_ENABLE_TIME_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_TIME_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_TIME_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL) && BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_SCHEDULER_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_SCHEDULER_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_SCENE_CLIENT_MODEL) && BT_MESH_ENABLE_SCENE_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_SCENE_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_SCENE_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_LEVEL_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_LEVEL_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_POWER_ON_OFF_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_BATTERY_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_BATTERY_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_LOCATION_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_LOCATION_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_PROPERTY_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
#if defined(BT_MESH_ENABLE_SENSOR_CLIENT_MODEL) && BT_MESH_ENABLE_SENSOR_CLIENT_MODEL
	case RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_SENSOR_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif
	case RTK_BT_LE_GP_MESH_HEALTH_CLIENT_MODEL:
		group = RTK_BT_IPC_MESH_HEALTH_CLIENT_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of RTK_BLE_MESH_PROVISIONER_SUPPORT

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
#if defined(BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SCENE_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_SCENE_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_SCENE_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_SCENE_SETUP_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SENSOR_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_SENSOR_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_SENSOR_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_SENSOR_SETUP_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL
	case RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL:
		group = RTK_BT_IPC_MESH_HEALTH_SERVER_MODEL;
		err = rtk_bt_mesh_set_act(group, act, param, param_len);
		break;
#endif // end of RTK_BLE_MESH_DEVICE_SUPPORT
#endif

	case RTK_BT_LE_GP_ISO:
		err = rtk_bt_le_iso_set_act(act, param, param_len);
		break;

	case RTK_BT_LE_GP_AUDIO:
		err = rtk_bt_le_audio_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_GAP:
		err = rtk_bt_br_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_A2DP:
		err = rtk_bt_a2dp_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_HFP:
		err = rtk_bt_hfp_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_AVRCP:
		err = rtk_bt_avrcp_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_SPP:
		err = rtk_bt_spp_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_HID:
		err = rtk_bt_hid_set_act(act, param, param_len);
		break;

	case RTK_BT_BR_GP_SDP:
		err = rtk_bt_sdp_set_act(act, param, param_len);
		break;

	case RTK_BT_COMMON_GP_GAP:
		err = rtk_bt_gap_set_act(act, param, param_len);
		break;

	default:
		printf("%s: unknown rtk bt group index %d \r\n", __func__, group);
		break;
	}

	return err;
}

static void rtk_bt_pre_excute_evt_hdl(uint8_t group, uint8_t act, void *ipc_msg)
{
	void *param = ((bt_ipc_dev_request_message *)ipc_msg)->param_buf;

	if (RTK_BT_LE_GP_GAP == group) {
		bt_le_ipc_pop_event_param(act, param);
	} else if (RTK_BT_LE_GP_GATTC == group) {
		bt_gattc_ipc_pop_event_param(act, param);
	} else if (RTK_BT_LE_GP_GATTS == group) {
		bt_gatts_ipc_pop_event_param(act, param);
	} else if (RTK_BT_BR_GP_GAP == group) {
		bt_br_ipc_pop_event_param(act, param);
	} else if (RTK_BT_BR_GP_A2DP == group) {
		bt_a2dp_ipc_pop_event_param(act, param);
	} else if (RTK_BT_BR_GP_HFP == group) {
		bt_hfp_ipc_pop_event_param(act, param);
	} else if (RTK_BT_BR_GP_AVRCP == group) {
		bt_avrcp_ipc_pop_event_param(act, param);
	} else if (RTK_BT_BR_GP_SPP == group) {
		bt_spp_ipc_pop_event_param(act, param);
	} else if (RTK_BT_BR_GP_SDP == group) {
		bt_sdp_ipc_pop_event_param(act, param);
	} else if (RTK_BT_LE_GP_ISO == group) {
		bt_le_iso_ipc_pop_event_param(act, param);
	} else if (RTK_BT_LE_GP_AUDIO == group) {
		bt_le_audio_ipc_pop_event_param(act, param);
	}
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#if defined(BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL == group) {
		bt_mesh_generic_onoff_server_model_alloc_mem_for_event_param(act, ipc_msg);
	}
#endif // end of BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL
#if defined(BT_MESH_ENABLE_DATATRANS_MODEL) && BT_MESH_ENABLE_DATATRANS_MODEL
	else if (RTK_BT_LE_GP_MESH_DATATRANS_MODEL == group) {
		bt_mesh_datatrans_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif
#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL == group) {
		bt_mesh_light_lightness_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL == group) {
		bt_mesh_light_ctl_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
	
#endif // end of BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL
else if (RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL == group) {
		bt_mesh_light_ctl_temperature_model_alloc_mem_for_event_param(act, ipc_msg);
	}	
#endif
#if defined(BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL == group) {
		bt_mesh_light_hsl_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL == group) {
		bt_mesh_light_hsl_hue_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL == group) {
		bt_mesh_light_hsl_saturation_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL == group) {
		bt_mesh_light_xyl_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL == group) {
		bt_mesh_light_lc_model_alloc_mem_for_event_param(act, ipc_msg);
	}
#endif // end of BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL
#if defined(BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL == group) {
		bt_mesh_light_lc_setup_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL
#if defined(BT_MESH_ENABLE_TIME_SERVER_MODEL) && BT_MESH_ENABLE_TIME_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL == group) {
		bt_mesh_time_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_TIME_SERVER_MODEL
#if defined(BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL == group) {
		bt_mesh_time_setup_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL) && BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_SCHEDULER_SERVER_MODEL == group) {
		bt_mesh_scheduler_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_SCHEDULER_SETUP_SERVER_MODEL == group) {
		bt_mesh_scheduler_setup_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SCENE_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL == group) {
		bt_mesh_scene_model_alloc_mem_for_event_param(act, ipc_msg);
	}
#endif // end of BT_MESH_ENABLE_SCENE_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL == group) {
		bt_mesh_scene_setup_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL == group) {
		bt_mesh_generic_default_transition_time_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL == group) {
		bt_mesh_generic_level_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL == group) {
		bt_mesh_generic_power_on_off_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL == group) {
		bt_mesh_generic_power_level_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_BATTERY_SERVER_MODEL == group) {
		bt_mesh_generic_battery_model_alloc_mem_for_event_param(act, ipc_msg);
	} 	
#endif // end of BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SERVER_MODEL == group) {
		bt_mesh_generic_location_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL == group) {
		bt_mesh_generic_location_setup_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL == group) {
		bt_mesh_generic_user_property_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL == group) {
		bt_mesh_generic_client_property_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL == group) {
		bt_mesh_generic_admin_property_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL == group) {
		bt_mesh_generic_manu_property_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SENSOR_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL == group) {
		bt_mesh_sensor_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif // end of BT_MESH_ENABLE_SENSOR_SERVER_MODEL
#if defined(BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL
	else if (RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL == group) {
		bt_mesh_sensor_setup_model_alloc_mem_for_event_param(act, ipc_msg);
	} 
#endif
#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
	else if (RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL == group) {
		bt_mesh_health_model_alloc_mem_for_event_param(act, ipc_msg);
	}
#endif // end of RTK_BLE_MESH_DEVICE_SUPPORT
#endif
}

static void rtk_bt_post_excute_evt_hdl(bt_ipc_dev_request_message *p_ipc_msg)
{
	uint8_t group = (uint8_t)p_ipc_msg->PROFILE_ID;
	uint8_t event = (uint8_t)p_ipc_msg->EVENT_ID;

	if (RTK_BT_LE_GP_AUDIO == group) {
		bt_le_audio_push_event_ipc_ret(event, &p_ipc_msg->ret[1], p_ipc_msg->param_buf);
	}
}
#endif

/* ===============================================Common Api ======================================= */
void rtk_bt_le_addr_to_str(void *paddr, char *str, uint32_t len)
{
	rtk_bt_le_addr_t *addr = (rtk_bt_le_addr_t *)paddr;
	char type[10];
	memset(str, 0, len);

	switch (addr->type) {
	case RTK_BT_LE_ADDR_TYPE_PUBLIC:
		strcpy(type, "public");
		break;
	case RTK_BT_LE_ADDR_TYPE_RANDOM:
		strcpy(type, "random");
		break;
	case RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC:
		strcpy(type, "public-id");
		break;
	case RTK_BT_LE_ADDR_TYPE_RPA_RANDOM:
		strcpy(type, "random-id");
		break;
	default:
		snprintf(type, sizeof(type), "0x%02x", addr->type);
		break;
	}
	snprintf(str, len, "%02X:%02X:%02X:%02X:%02X:%02X(%s)",
			 addr->addr_val[5], addr->addr_val[4], addr->addr_val[3],
			 addr->addr_val[2], addr->addr_val[1], addr->addr_val[0], type);
}

void rtk_bt_br_addr_to_str(uint8_t *paddr, char *str, uint32_t len)
{
	memset(str, 0, len);
	snprintf(str, len, "%02X:%02X:%02X:%02X:%02X:%02X",
			 paddr[5], paddr[4], paddr[3],
			 paddr[2], paddr[1], paddr[0]);
}

void rtk_bt_addr_to_str(uint8_t addr_type, uint8_t *paddr, char *str, uint32_t len)
{
	char str_type[20] = {0};
	memset(str, 0, len);

	switch (addr_type & 0xF) {
	case RTK_BT_ADDR_LE_PUBLIC:
		strcpy(str_type + strlen(str_type), "le public");
		break;
	case RTK_BT_ADDR_LE_RANDOM:
		strcpy(str_type + strlen(str_type), "le random");
		break;
	case RTK_BT_ADDR_LE_RPA_PUBLIC:
		strcpy(str_type + strlen(str_type), "le public-id");
		break;
	case RTK_BT_ADDR_LE_RPA_RANDOM:
		strcpy(str_type + strlen(str_type), "le random-id");
		break;
	case RTK_BT_ADDR_CLASSIC:
		strcpy(str_type + strlen(str_type), "classic");
		break;
	default:
		snprintf(str_type, sizeof(str_type), "0x%02x", addr_type);
		break;
	}
	snprintf(str, len, "%02X:%02X:%02X:%02X:%02X:%02X(%s)",
			 paddr[5], paddr[4], paddr[3],
			 paddr[2], paddr[1], paddr[0], str_type);
}

uint16_t rtk_bt_send_cmd(uint8_t group, uint8_t act, void *param, uint32_t param_len)
{
	uint16_t ret = RTK_BT_FAIL;
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	ret = bt_send_ipc_cmd(group, act, param, param_len);
#else

	rtk_bt_cmd_t *pcmd = NULL;

	pcmd = (rtk_bt_cmd_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_cmd_t));
	if (!pcmd) {
		goto end;
	}

	memset(pcmd, 0, sizeof(rtk_bt_cmd_t));
	pcmd->group = group;
	pcmd->act = act;
	pcmd->param = param;
	pcmd->param_len = param_len;

	if (false == osif_sem_create(&pcmd->psem, 0, 1)) {
		ret = RTK_BT_ERR_OS_OPERATION;
		goto end;
	}

	if (bt_stack_api_send(pcmd)) {
		ret = RTK_BT_ERR_MSG_SEND;
		goto end;
	}

	if (false == osif_sem_take(pcmd->psem, 0xffffffff)) {
#if !defined(CONFIG_BT_ZEPHYR) || (!CONFIG_BT_ZEPHYR)
		/* if the pcmd has been added in pending list, and no stack_cb to pick it off
		within api_sync_timeout, it need to be deleted here */
		bt_stack_pending_cmd_delete(pcmd);
#endif
		ret = RTK_BT_ERR_SYNC_TIMEOUT;
		goto end;
	}

	ret = pcmd->ret;
end:
	if (pcmd) {
		if (pcmd->psem) {
			osif_sem_delete(pcmd->psem);
		}
		osif_mem_free(pcmd);
	}
#endif
	return ret;
}

uint8_t rtk_bt_excute_evt_cb(uint8_t group, uint8_t evt_code, void *param, uint32_t len)
{
	uint8_t ret = 0;
	rtk_bt_evt_cb_t cb_func = NULL;

	if (group < RTK_BT_API_BR_BASE) {
		cb_func = rtk_bt_le_evt_cb_tbl[group - RTK_BT_API_LE_BASE];
	} else if (group < RTK_BT_API_COMMON_BASE) {
		cb_func = rtk_bt_br_evt_cb_tbl[group - RTK_BT_API_BR_BASE];
	} else { /* if (group >= RTK_BT_API_COMMON_BASE) */
		cb_func = rtk_bt_evt_cb_tbl[group - RTK_BT_API_COMMON_BASE];
	}

	if (cb_func) {
		ret = cb_func(evt_code, param, len);
	}

	return ret;
}

void rtk_bt_event_free(rtk_bt_evt_t *pevt)
{
	if (pevt->data_len > RTK_EVT_POOL_SIZE) {
		osif_mem_free(pevt->data);
	}
	if (pevt->user_data != NULL) {
		osif_mem_free(pevt->user_data);
	}

	osif_mem_free(pevt);
}

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
rtk_bt_evt_t bt_ipc_drc_evt = {0};
rtk_bt_evt_t bt_ipc_common_evt = {0};

static void rtk_bt_drc_evt_taskentry(void *ctx)
{
	(void)ctx;
	rtk_bt_evt_t *pevt;
	printf("[Bt evt task]: enter bt directly calling evt task\r\n");
	osif_sem_give(g_drc_evt_task_sem);

	while (true) {
		if (osif_msg_recv(g_drc_evt_queue, &pevt, BT_TIMEOUT_FOREVER)) {
			/* Check msg */
			if (!pevt) {
				continue;
			}
			/* check if need to exit event task */
			if (RTK_BT_DRC_EVENT_TASK_EXIT == pevt->group) {
				rtk_bt_event_free(pevt);
				break;
			}
			/* event from ipc */
			{
				bt_ipc_dev_request_message *p_ipc_msg = NULL;

				if (pevt->data == NULL) {
					printf("%s: Device IPC API message is NULL, invalid! \r\n", __func__);
					continue;
				} else {
					p_ipc_msg = (bt_ipc_dev_request_message *)pevt->data;
				}
				/* for pop param of some special acts */
				rtk_bt_pre_excute_evt_hdl((uint8_t)p_ipc_msg->PROFILE_ID, (uint8_t)p_ipc_msg->EVENT_ID, (void *)p_ipc_msg);
				p_ipc_msg->ret[0] = rtk_bt_excute_evt_cb((uint8_t)p_ipc_msg->PROFILE_ID, (uint8_t)p_ipc_msg->EVENT_ID,
									(void *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
				rtk_bt_post_excute_evt_hdl(p_ipc_msg);
				/*set EVENT_ID to 0 to notify NP that event is finished*/
				pevt->data = NULL;
				p_ipc_msg->EVENT_ID = 0xFFFFFFFF;
				DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_dev_request_message));
				/* AP use static memory, no need to free */
				// rtk_bt_event_free(pevt);
			}
		}
	}
	printf("[Bt evt task]: exit bt directly calling evt task\r\n");
	API_PRINT("[BT evt task] bt directly calling evt task exit\r\n");
	osif_sem_give(g_drc_evt_task_sem);
	osif_task_delete(NULL);
}
#endif

static void rtk_bt_evt_taskentry(void *ctx)
{
	(void)ctx;
	rtk_bt_evt_t *pevt;

	osif_sem_give(g_evt_task_sem);

	while (true) {
		if (osif_msg_recv(g_evt_queue, &pevt, 0xffffffff)) {
			/* Check msg */
			if (!pevt) {
				continue;
			}
			/* check if need to exit event task */
			if (RTK_BT_EVENT_TASK_EXIT == pevt->group) {
				rtk_bt_event_free(pevt);
				break;
			}
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
			/* event from ipc */
			{
				bt_ipc_dev_request_message *p_ipc_msg = NULL;

				if (pevt->data == NULL) {
					printf("%s: Device IPC API message is NULL, invalid! \r\n", __func__);
					continue;
				} else {
					p_ipc_msg = (bt_ipc_dev_request_message *)pevt->data;
				}
				/* for pop param of some special acts */
				rtk_bt_pre_excute_evt_hdl((uint8_t)p_ipc_msg->PROFILE_ID, (uint8_t)p_ipc_msg->EVENT_ID, (void *)p_ipc_msg);
				p_ipc_msg->ret[0] = rtk_bt_excute_evt_cb((uint8_t)p_ipc_msg->PROFILE_ID, (uint8_t)p_ipc_msg->EVENT_ID,
									(void *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
				rtk_bt_post_excute_evt_hdl(p_ipc_msg);
				/*set EVENT_ID to 0 to notify NP that event is finished*/
				pevt->data = NULL;
				p_ipc_msg->EVENT_ID = 0xFFFFFFFF;
				DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_dev_request_message));
				/* AP use static memory, no need to free */
				// rtk_bt_event_free(pevt);
			}
#else
			/* Call back user registered function */
			rtk_bt_excute_evt_cb(pevt->group, pevt->evt, pevt->data, pevt->data_len);
			rtk_bt_event_free(pevt);
#endif
		}
	}
	// printf("[Bt evt task]: exit bt evt task\r\n");
	API_PRINT("[BT evt task] bt evt task exit\r\n");
	osif_sem_give(g_evt_task_sem);
	osif_task_delete(NULL);
}

uint16_t rtk_bt_evt_init(void)
{
#if defined (CONFIG_BT_AP) && CONFIG_BT_AP
	/* directly calling component */
	if (false == osif_sem_create(&g_drc_evt_task_sem, 0, 1)) {
		goto failed;
	}

	if (false == osif_msg_queue_create(&g_drc_evt_queue, EVENT_NUM, sizeof(void *))) {
		goto failed;
	}

	if (false == osif_task_create(&g_drc_evt_task_hdl, "bt_drc_event_task",
								  rtk_bt_drc_evt_taskentry, NULL,
								  EVENT_TASK_STACK_SIZE, EVENT_TASK_PRIORITY)) {
		goto failed;
	}

	if (false == osif_sem_take(g_drc_evt_task_sem, 0xffffffff)) {
		goto failed;
	}

	memset((void *)&bt_ipc_drc_evt, 0, sizeof(rtk_bt_evt_t));
	memset((void *)&bt_ipc_common_evt, 0, sizeof(rtk_bt_evt_t));
#endif
	if (false == osif_sem_create(&g_evt_task_sem, 0, 1)) {
		goto failed;
	}

	if (false == osif_msg_queue_create(&g_evt_queue, EVENT_NUM, sizeof(void *))) {
		goto failed;
	}

	if (false == osif_task_create(&g_evt_task_hdl, "bt_event_task",
								  rtk_bt_evt_taskentry, NULL,
								  EVENT_TASK_STACK_SIZE, EVENT_TASK_PRIORITY)) {
		goto failed;
	}

	if (false == osif_sem_take(g_evt_task_sem, 0xffffffff)) {
		goto failed;
	}

	event_task_running = true;
	return 0;

failed:
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	if (g_drc_evt_task_hdl) {
		osif_task_delete(g_drc_evt_task_hdl);
	}
	if (g_drc_evt_queue) {
		osif_msg_queue_delete(g_drc_evt_queue);
	}
	if (g_drc_evt_task_sem) {
		osif_sem_delete(g_drc_evt_task_sem);
	}
#endif
	if (g_evt_task_hdl) {
		osif_task_delete(g_evt_task_hdl);
	}
	if (g_evt_queue) {
		osif_msg_queue_delete(g_evt_queue);
	}
	if (g_evt_task_sem) {
		osif_sem_delete(g_evt_task_sem);
	}

	return RTK_BT_FAIL;
}

static uint16_t rtk_bt_evt_reset_callback(void)
{
	memset(rtk_bt_le_evt_cb_tbl, 0, sizeof(rtk_bt_le_evt_cb_tbl));
	memset(rtk_bt_br_evt_cb_tbl, 0, sizeof(rtk_bt_br_evt_cb_tbl));
	memset(rtk_bt_evt_cb_tbl, 0, sizeof(rtk_bt_evt_cb_tbl));

	return 0;
}

uint16_t rtk_bt_evt_deinit(void)
{
	rtk_bt_evt_t *p_evt;

	event_task_running = false;

	/* Waiting rtk_bt_evt_indicate() on other tasks interrupted by deinit task to complete */
	while (event_task_msg_num) {
		osif_delay(5);
	}

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	/* directly calling component */
	p_evt = rtk_bt_event_create(RTK_BT_DRC_EVENT_TASK_EXIT, 0, 0);
	if (!p_evt)
		return RTK_BT_ERR_NO_MEMORY;

	rtk_bt_evt_indicate(p_evt, NULL);

	if (false == osif_sem_take(g_drc_evt_task_sem, 0xffffffff)) {
		return RTK_BT_FAIL;
	}

	osif_sem_delete(g_drc_evt_task_sem);
	osif_msg_queue_delete(g_drc_evt_queue);
	g_drc_evt_task_hdl = NULL;
	g_drc_evt_task_sem = NULL;
	g_drc_evt_queue = NULL;
	memset((void *)&bt_ipc_drc_evt, 0, sizeof(rtk_bt_evt_t));
	memset((void *)&bt_ipc_common_evt, 0, sizeof(rtk_bt_evt_t));
#endif

	p_evt = rtk_bt_event_create(RTK_BT_EVENT_TASK_EXIT, 0, 0);
	if (!p_evt)
		return RTK_BT_ERR_NO_MEMORY;

	rtk_bt_evt_indicate(p_evt, NULL);

	if (false == osif_sem_take(g_evt_task_sem, 0xffffffff)) {
		return RTK_BT_FAIL;
	}

	osif_sem_delete(g_evt_task_sem);
	osif_msg_queue_delete(g_evt_queue);
	g_evt_task_hdl = NULL;
	g_evt_task_sem = NULL;
	g_evt_queue = NULL;
	rtk_bt_evt_reset_callback();

	return 0;
}

uint16_t rtk_bt_evt_register_callback(uint8_t group, rtk_bt_evt_cb_t cb)
{
	bool b_is_le_mode = false;
	bool b_is_br_mode = false;
	bool b_is_common = false;

	API_PRINT("--------------> rtk_bt_evt_register_callback: group = 0x%x \r\n", group);

	if (group < RTK_BT_LE_GP_MAX) {
		b_is_le_mode = true;
	} else if ((group >= RTK_BT_API_BR_BASE) && (group < RTK_BT_BR_GP_MAX)) {
		b_is_br_mode = true;
	} else if ((group >= RTK_BT_API_COMMON_BASE) && (group < RTK_BT_COMMON_GP_MAX)) {
		b_is_common = true;
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (b_is_le_mode) {
		rtk_bt_le_evt_cb_tbl[group - RTK_BT_API_LE_BASE] = cb;
	}
	if (b_is_br_mode) {
		rtk_bt_br_evt_cb_tbl[group - RTK_BT_API_BR_BASE] = cb;
	}
	if (b_is_common) {
		rtk_bt_evt_cb_tbl[group - RTK_BT_API_COMMON_BASE] = cb;
	}

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;

		if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_REGISTER_CB)) {
			ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_REGISTER_CB,
										   (uint8_t *)&group, sizeof(uint8_t));
		} else {
			ret = bt_ipc_api_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_REGISTER_CB,
										   (uint8_t *)&group, sizeof(uint8_t));
		}
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		osif_mem_free(ret);
	}
#endif

	return 0;
}

uint16_t rtk_bt_evt_unregister_callback(uint8_t group)
{
	bool b_is_le_mode = false;
	bool b_is_br_mode = false;
	bool b_is_common = false;

	API_PRINT("--------------> rtk_bt_evt_unregister_callback \r\n");
	if (true != rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (group < RTK_BT_LE_GP_MAX) {
		b_is_le_mode = true;
	} else if ((group >= RTK_BT_API_BR_BASE) && (group < RTK_BT_BR_GP_MAX)) {
		b_is_br_mode = true;
	} else if ((group >= RTK_BT_API_COMMON_BASE) && (group < RTK_BT_COMMON_GP_MAX)) {
		b_is_common = true;
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (b_is_le_mode) {
		rtk_bt_le_evt_cb_tbl[group - RTK_BT_API_LE_BASE] = NULL;
	}
	if (b_is_br_mode) {
		rtk_bt_br_evt_cb_tbl[group - RTK_BT_API_BR_BASE] = NULL;
	}
	if (b_is_common) {
		rtk_bt_evt_cb_tbl[group - RTK_BT_API_COMMON_BASE] = NULL;
	}

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;

		if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_UNREGISTER_CB)) {
			ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_UNREGISTER_CB,
										   (uint8_t *)&group, sizeof(uint8_t));
		} else {
			ret = bt_ipc_api_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_UNREGISTER_CB,
										   (uint8_t *)&group, sizeof(uint8_t));
		}
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		osif_mem_free(ret);
	}
#endif

	return 0;
}

rtk_bt_evt_t *rtk_bt_event_create(uint8_t group, uint8_t evt, uint32_t param_len)
{
	rtk_bt_evt_t *pevt = NULL;
	pevt = (rtk_bt_evt_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_evt_t));
	if (NULL == pevt) {
		goto end;
	}

	memset(pevt, 0, sizeof(rtk_bt_evt_t));
	pevt->group = group;
	pevt->evt = evt;

	if (param_len != 0) {
		pevt->data_len = param_len;

		if (param_len <= RTK_EVT_POOL_SIZE) {
			pevt->data = pevt->data_pool;
		} else {
			pevt->data = osif_mem_alloc(RAM_TYPE_DATA_ON, param_len);
			if (NULL == pevt->data) {
				osif_mem_free(pevt);
				pevt = NULL;
				goto end;
			}
		}
		memset(pevt->data, 0, param_len);
	}

end:
	return pevt;
}

bool rtk_bt_check_evt_cb_direct_calling(uint8_t group, uint8_t evt_code)
{
	bool ret = false;

	switch (group) {
	case RTK_BT_LE_GP_GAP:
		if (rtk_bt_le_gap_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_GATTS:
		if (rtk_bt_le_gatts_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_GATTC:
		if (rtk_bt_le_gattc_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_ISO:
		if (rtk_bt_le_iso_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_AVRCP:
		if (rtk_bt_br_avrcp_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_A2DP:
		if (rtk_bt_br_a2dp_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_HFP:
		if (rtk_bt_br_hfp_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_COMMON_GP_GAP:
		if (rtk_bt_gap_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_AUDIO:
		if (rtk_bt_le_audio_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	case RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL:
		if (rtk_bt_mesh_generic_onoff_server_model_evt_direct_calling_flag) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_MESH_DATATRANS_MODEL:
		if (rtk_bt_mesh_datatrans_model_evt_direct_calling_flag) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCHEDULER_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCHEDULER_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_BATTERY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL:
		ret = true;
		break;
#endif
	default:
		break;
	}

	return ret;
}

uint16_t rtk_bt_evt_indicate(void *evt, uint8_t *cb_ret)
{
	uint16_t ret = 0;
	uint32_t flags = 0;
	rtk_bt_evt_t *p_evt = (rtk_bt_evt_t *)evt;
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	uint8_t directly_calling_flag = 0;
#endif
#if !(defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE || \
	defined(CONFIG_BT_NP) && CONFIG_BT_NP)
	(void)cb_ret;
#endif

	if (!evt) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	if (true == rtk_bt_check_evt_cb_direct_calling(p_evt->group, p_evt->evt) || \
		RTK_BT_DRC_EVENT_TASK_EXIT == p_evt->group) {
		directly_calling_flag = 1;
	}
#else
	uint8_t cb_ret_tmp = 0;
	if (true == rtk_bt_check_evt_cb_direct_calling(p_evt->group, p_evt->evt)) {
		cb_ret_tmp = rtk_bt_excute_evt_cb(p_evt->group, p_evt->evt, p_evt->data, p_evt->data_len);
		if (cb_ret != NULL) {
			*cb_ret = cb_ret_tmp;
		}
		/* If the evt cb is direct calling, the evt msg isn't actually send to evt task,
		Instead, it's excuted here, so need to free it here */
		rtk_bt_event_free(evt);
		return 0;
	}
#endif

	flags = osif_lock();
	event_task_msg_num++;
	osif_unlock(flags);

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	if (!event_task_running && \
		p_evt->group != RTK_BT_EVENT_TASK_EXIT && \
		p_evt->group != RTK_BT_DRC_EVENT_TASK_EXIT) {
		ret = RTK_BT_ERR_NOT_READY;
		goto end;
	}
	/* If the evt cb is not direct calling, send evt msg to event task */
	if (directly_calling_flag) {
		if (false == osif_msg_send(g_drc_evt_queue, &evt, 0) {
			ret = RTK_BT_ERR_OS_OPERATION;
		}
	} else {
		if (false == osif_msg_send(g_evt_queue, &evt, 0)) {
			ret = RTK_BT_ERR_OS_OPERATION;
		}
	}
#else
	/* send EXIT as last msg to kill task */
	if (!event_task_running && p_evt->group != RTK_BT_EVENT_TASK_EXIT) {
		ret = RTK_BT_ERR_NOT_READY;
		goto end;
	}
	if (false == osif_msg_send(g_evt_queue, &evt, 0)) {
		ret = RTK_BT_ERR_OS_OPERATION;
	}
#endif

end:
	flags = osif_lock();
	event_task_msg_num--;
	osif_unlock(flags);

#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE || \
	defined(CONFIG_BT_NP) && CONFIG_BT_NP
	/* AP use static memory, no need to free */
	if (ret)
		rtk_bt_event_free(evt);
#endif
	return ret;
}
