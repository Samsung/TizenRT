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
#include <rtk_bt_gattc.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_cfg.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_profile_config.h>
#include <rtk_bt_gatts.h>
#else
#include <rtk_stack_internal.h>
#include <rtk_bt_def.h>
#endif
#include <rtk_bt_device.h>
#include <bt_ipc_parameter_conversion.h>

#define EVENT_NUM 64
/* NOTE: If user's event callback need more stack size, please enlarge it */
#define EVENT_TASK_STACK_SIZE 4096
#define EVENT_TASK_PRIORITY 3

#if !defined(CONFIG_BT_AP) || (!CONFIG_BT_AP)
/* Internal definition for mask of which evt callback support direct calling  */
#define LE_GAP_EVT_DIRECT_CALLING_SUPPORT_MASK  	\
		(1 << RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND)

#define LE_GATTS_EVT_DIRECT_CALLING_SUPPORT_MASK	0
#define LE_GATTC_EVT_DIRECT_CALLING_SUPPORT_MASK	0
#define LE_ISO_EVT_DIRECT_CALLING_SUPPORT_MASK		\
		(1 << RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND)
#define BR_A2DP_EVT_DIRECT_CALLING_SUPPORT_MASK		\
		(1 << RTK_BT_A2DP_EVT_STREAM_DATA_IND)
#define BR_HFP_EVT_DIRECT_CALLING_SUPPORT_MASK		\
		(1 << RTK_BT_HFP_EVT_SCO_DATA_IND)
#define LE_AUDIO_EVT_DIRECT_CALLING_SUPPORT_MASK		\
		((1 << RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND) | \
		(1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND) | \
		(1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND) | \
		(1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_BROADCAST_CODE_IND) | \
		(1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_PREFER_BIS_SYNC_IND))
#define GAP_EVT_DIRECT_CALLING_SUPPORT_MASK  	\
		(1 << RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND)

static uint32_t rtk_bt_le_gap_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_le_gatts_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_le_gattc_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_le_iso_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_br_a2dp_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_br_hfp_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_le_audio_evt_direct_calling_flag = {0};
static uint32_t rtk_bt_gap_evt_direct_calling_flag = {0};

extern uint16_t bt_stack_api_send(void *pcmd);
extern void bt_stack_gap_delete_pending_cmd(rtk_bt_cmd_t *p_cmd);
#endif

static void * g_evt_reg_cb_mutex = NULL;
static void * g_evt_task_sem = NULL;
static void * g_evt_queue = NULL;
static void * g_evt_task_hdl = NULL;
uint8_t g_evt_task_running = 0;

rtk_bt_evt_cb_t rtk_bt_le_evt_cb_tbl[RTK_BT_LE_GP_MAX-RTK_BT_API_LE_BASE] = {0};
rtk_bt_evt_cb_t rtk_bt_br_evt_cb_tbl[RTK_BT_BR_GP_MAX-RTK_BT_API_BR_BASE] = {0};
rtk_bt_evt_cb_t rtk_bt_evt_cb_tbl[RTK_BT_COMMON_GP_MAX-RTK_BT_API_COMMON_BASE] = {0};

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP

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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GAP, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GAP_BLE, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GATTS, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GATTC, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_GAP_BR, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_A2DP, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_HFP, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_AVRCP, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SDP, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_LE_ISO, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_LE_AUDIO, act, host_msg->param_buf, actual_size);
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
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_SPP, act, host_msg->param_buf, actual_size);
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

static uint16_t bt_send_ipc_cmd(uint8_t group,uint8_t act,void *param, uint32_t param_len)
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

static void rtk_bt_pre_excute_evt_hdl(uint8_t group, uint8_t act, void *param)
{
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
}

#endif

/* ===============================================Common Api ======================================= */
void rtk_bt_le_addr_to_str(void *paddr, char *str, uint32_t len)
{
	rtk_bt_le_addr_t *addr = (rtk_bt_le_addr_t*)paddr;
	char type[10];
	memset(str, 0, len);

	switch(addr->type) {
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

	switch(addr_type & 0xF) {
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

uint16_t rtk_bt_send_cmd(uint8_t group,uint8_t act,void *param, uint32_t param_len)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	return bt_send_ipc_cmd(group, act, param, param_len);
#else
	rtk_bt_cmd_t *pcmd = NULL;
	uint16_t ret = RTK_BT_FAIL;

	pcmd = (rtk_bt_cmd_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_cmd_t));
	if (!pcmd)
		goto end;

	memset(pcmd, 0, sizeof(rtk_bt_cmd_t));
	pcmd->group = group;
	pcmd->act = act;
	pcmd->param = param;
	pcmd->param_len = param_len;

	if (false == osif_sem_create(&pcmd->psem,0,1)){
		ret = RTK_BT_ERR_OS_OPERATION;
		goto end;
	}

	if (bt_stack_api_send(pcmd)){
		ret = RTK_BT_ERR_MSG_SEND;
		goto end;
	}

	if (false == osif_sem_take(pcmd->psem, BT_API_SYNC_TIMEOUT)) {
		/* if the pcmd has been added in pending list, and no stack_cb to pick it off
		within api_sync_timeout, it need to be deleted here */
		if (pcmd->list.next != NULL) {
			bt_stack_gap_delete_pending_cmd(pcmd);
		}
		ret = RTK_BT_ERR_SYNC_TIMEOUT;
		goto end;
	}

	ret = pcmd->ret;
end:
	if (pcmd){
		if (pcmd->psem)
			osif_sem_delete(pcmd->psem);
		osif_mem_free(pcmd);
	}
	return ret;
#endif
}

uint8_t rtk_bt_excute_evt_cb(uint8_t group, uint8_t evt_code, void *param)
{
	uint8_t ret = 0;
	rtk_bt_evt_cb_t cb_func = NULL;

	if (group < RTK_BT_API_BR_BASE) {
		cb_func = rtk_bt_le_evt_cb_tbl[group-RTK_BT_API_LE_BASE]; 
	} else if (group < RTK_BT_API_COMMON_BASE) {
		cb_func = rtk_bt_br_evt_cb_tbl[group-RTK_BT_API_BR_BASE];	
	} else /* if (group >= RTK_BT_API_COMMON_BASE) */{
		cb_func = rtk_bt_evt_cb_tbl[group-RTK_BT_API_COMMON_BASE];	
	}

	if(cb_func)
		ret = cb_func(evt_code, param);

	return ret;
}

void rtk_bt_event_free(rtk_bt_evt_t* pevt)
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
uint8_t ipc_msg_count = 0;
rtk_bt_evt_t bt_ipc_evt = {0};
#endif

static void rtk_bt_evt_taskentry(void* ctx)
{
	(void)ctx;
	rtk_bt_evt_t* pevt;
	
	g_evt_task_running = 1;
	osif_sem_give(g_evt_task_sem);

	while (g_evt_task_running)
	{
		if (osif_msg_recv(g_evt_queue, &pevt, BT_TIMEOUT_FOREVER))
		{
			/* Check msg */
			if (!pevt) 
				continue;
			/* check if need to exit event task */
			if (RTK_BT_EVENT_TASK_EXIT == pevt->group) {
				g_evt_task_running = 0;
				rtk_bt_event_free(pevt);
				continue;
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
				rtk_bt_pre_excute_evt_hdl((uint8_t)p_ipc_msg->PROFILE_ID, (uint8_t)p_ipc_msg->EVENT_ID, (void *)p_ipc_msg->param_buf);
				p_ipc_msg->ret[0] = rtk_bt_excute_evt_cb((uint8_t)p_ipc_msg->PROFILE_ID, (uint8_t)p_ipc_msg->EVENT_ID, (void *)p_ipc_msg->param_buf);
				/*set EVENT_ID to 0 to notify NP that event is finished*/
				p_ipc_msg->EVENT_ID = 0xFFFFFFFF;
				pevt->data = NULL;
				DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_dev_request_message));
				/* AP use static memory, no need to free */
				// rtk_bt_event_free(pevt);
				ipc_msg_count --;
			}
#else
			/* Call back user registered function */
			rtk_bt_excute_evt_cb(pevt->group, pevt->evt, pevt->data);
			rtk_bt_event_free(pevt);
#endif
		}
	}
	// printf("[Bt evt task]: exit bt evt task\r\n");
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	while (ipc_msg_count) {
		bt_ipc_dev_request_message *p_ipc_msg = NULL;
		/* not running return fail */
		printf("%s: task is not running! \r\n", __func__);
		if (pevt->data) {
			p_ipc_msg = (bt_ipc_dev_request_message *)pevt->data;
			p_ipc_msg->ret[0] = 1;
			p_ipc_msg->EVENT_ID = 0xFFFFFFFF;
			DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
			pevt->data = NULL;
		}
		ipc_msg_count --;
	}
#endif
	API_PRINT("[BT evt task] bt evt task exit\r\n");
	osif_sem_give(g_evt_task_sem);
	osif_task_delete(NULL);
}

uint16_t rtk_bt_evt_init(void)
{
	if (false == osif_mutex_create(&g_evt_reg_cb_mutex))
		goto failed;
	
	if (false == osif_sem_create(&g_evt_task_sem,0,1))
		goto failed;
	
	if (false == osif_msg_queue_create(&g_evt_queue, EVENT_NUM, sizeof(void*)))
		goto failed;
	
	g_evt_task_running = 0;
	if (false == osif_task_create(&g_evt_task_hdl, "bt_event_task",
								rtk_bt_evt_taskentry, NULL, 
								EVENT_TASK_STACK_SIZE, EVENT_TASK_PRIORITY))
		goto failed;
#if defined (CONFIG_BT_AP) && CONFIG_BT_AP
	ipc_msg_count = 0;
	memset ((void *)&bt_ipc_evt, 0, sizeof(rtk_bt_evt_t));
#endif
	if (false == osif_sem_take(g_evt_task_sem, BT_TIMEOUT_FOREVER))
		goto failed;
	
	return 0;

failed:
	if (g_evt_task_hdl)
		osif_task_delete(g_evt_task_hdl);
	if (g_evt_queue)
		osif_msg_queue_delete(g_evt_queue);
	if (g_evt_task_sem)
		osif_sem_delete(g_evt_task_sem);
	if (g_evt_reg_cb_mutex)
		osif_mutex_delete(g_evt_reg_cb_mutex);
	
	return RTK_BT_FAIL;
}

static uint16_t rtk_bt_evt_reset_callback(void)
{
	osif_mutex_take(g_evt_reg_cb_mutex, 0xFFFFFFFF);
	memset(rtk_bt_le_evt_cb_tbl, 0, sizeof(rtk_bt_le_evt_cb_tbl));
	memset(rtk_bt_br_evt_cb_tbl, 0, sizeof(rtk_bt_br_evt_cb_tbl));
	memset(rtk_bt_evt_cb_tbl, 0, sizeof(rtk_bt_evt_cb_tbl));
	osif_mutex_give(g_evt_reg_cb_mutex);

	return 0;
}

uint16_t rtk_bt_evt_deinit(void)
{
	rtk_bt_evt_t *p_evt = rtk_bt_event_create(RTK_BT_EVENT_TASK_EXIT, 0, 0);
	rtk_bt_evt_indicate(p_evt, NULL);

	if (false == osif_sem_take(g_evt_task_sem, BT_TIMEOUT_FOREVER))
		return RTK_BT_FAIL;

	osif_sem_delete(g_evt_task_sem);
	osif_msg_queue_delete(g_evt_queue);
	g_evt_task_hdl = NULL;
	g_evt_task_sem = NULL;
	g_evt_queue = NULL;
#if defined (CONFIG_BT_AP) && CONFIG_BT_AP
	ipc_msg_count = 0;
	memset ((void *)&bt_ipc_evt, 0, sizeof(rtk_bt_evt_t));
#endif
	rtk_bt_evt_reset_callback();
	osif_mutex_delete(g_evt_reg_cb_mutex);
	g_evt_reg_cb_mutex = NULL;

	return 0;
}

uint16_t rtk_bt_evt_register_callback(uint8_t group, rtk_bt_evt_cb_t cb)
{	
	bool b_is_le_mode = false;
	bool b_is_br_mode = false;
	bool b_is_common = false;

	API_PRINT("--------------> rtk_bt_evt_register_callback: group = 0x%x \r\n",group);
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
	
	osif_mutex_take(g_evt_reg_cb_mutex, 0xFFFFFFFF);
	if (b_is_le_mode)
		rtk_bt_le_evt_cb_tbl[group-RTK_BT_API_LE_BASE] = cb;
	if (b_is_br_mode)
		rtk_bt_br_evt_cb_tbl[group-RTK_BT_API_BR_BASE] = cb;
	if (b_is_common)
		rtk_bt_evt_cb_tbl[group-RTK_BT_API_COMMON_BASE] = cb;
		
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;

		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_REGISTER_CB,
										(uint8_t *)&group, sizeof(uint8_t));
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		osif_mem_free(ret);
	}
#endif
	osif_mutex_give(g_evt_reg_cb_mutex);

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
	
	osif_mutex_take(g_evt_reg_cb_mutex, 0xFFFFFFFF);
	if (b_is_le_mode)
		rtk_bt_le_evt_cb_tbl[group-RTK_BT_API_LE_BASE] = NULL;
	if (b_is_br_mode)
		rtk_bt_br_evt_cb_tbl[group-RTK_BT_API_BR_BASE] = NULL;
	if (b_is_common)
		rtk_bt_evt_cb_tbl[group-RTK_BT_API_COMMON_BASE] = NULL;

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;

		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_UNREGISTER_CB,
										(uint8_t *)&group, sizeof(uint8_t));
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		osif_mem_free(ret);
	}
#endif
	osif_mutex_give(g_evt_reg_cb_mutex);

	return 0;
}

uint16_t rtk_bt_set_evt_cb_direct_calling(uint8_t group, uint32_t evt_bit_mask)
{
	if (true != rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;
	uint16_t err;
	rtk_bt_direct_call_t direct_call_t = {0};

	direct_call_t.group = group;
	direct_call_t.evt_bit_mask = evt_bit_mask;
	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_COMMON, RTK_BT_ACT_IPC_ENABLE_DIRECT_CALL,
										(uint8_t *)&direct_call_t, sizeof(rtk_bt_direct_call_t));
	if (ret[0] != RTK_BT_OK) {
		printf("[core AP][IPC] %s fail ! \r\n", __func__);
	}
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

	return err;
#else
	uint16_t ret = 0;
	switch(group) {
	case RTK_BT_LE_GP_GAP:
		rtk_bt_le_gap_evt_direct_calling_flag = 
								evt_bit_mask & LE_GAP_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_LE_GP_GATTS:
		rtk_bt_le_gatts_evt_direct_calling_flag = 
								evt_bit_mask & LE_GATTS_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_LE_GP_GATTC:
		rtk_bt_le_gattc_evt_direct_calling_flag = 
								evt_bit_mask & LE_GATTC_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_LE_GP_ISO:
		rtk_bt_le_iso_evt_direct_calling_flag = 
								evt_bit_mask & LE_ISO_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_BR_GP_A2DP:
		rtk_bt_br_a2dp_evt_direct_calling_flag = 
								evt_bit_mask & BR_A2DP_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_BR_GP_HFP:
		rtk_bt_br_hfp_evt_direct_calling_flag = 
								evt_bit_mask & BR_HFP_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_LE_GP_AUDIO:
		rtk_bt_le_audio_evt_direct_calling_flag = 
								evt_bit_mask & LE_AUDIO_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	case RTK_BT_COMMON_GP_GAP:
		rtk_bt_gap_evt_direct_calling_flag = 
								evt_bit_mask & GAP_EVT_DIRECT_CALLING_SUPPORT_MASK;
		break;
	default:
		break;
	}

	return ret;
#endif
}

rtk_bt_evt_t *rtk_bt_event_create(uint8_t group, uint8_t evt, uint32_t param_len)
{
	rtk_bt_evt_t* pevt = NULL;
	pevt = (rtk_bt_evt_t* )osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_evt_t));
	if (NULL == pevt)
		goto end;

	memset(pevt,0,sizeof(rtk_bt_evt_t));
	pevt->group = group;
	pevt->evt = evt;

	if (param_len != 0){
		pevt->data_len = param_len;

		if (param_len <= RTK_EVT_POOL_SIZE){
			pevt->data = pevt->data_pool;
		}
		else{
			pevt->data = osif_mem_alloc(RAM_TYPE_DATA_ON, param_len);
			if (NULL == pevt->data){
				osif_mem_free(pevt);
				pevt = NULL;
				goto end;
			}
		}
		memset(pevt->data,0,param_len);
	}

end:
	return pevt;
}

#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE || \
	defined(CONFIG_BT_NP) && CONFIG_BT_NP

bool rtk_bt_check_evt_cb_direct_calling(uint8_t group, uint8_t evt_code)
{
	bool ret = false;

	switch(group) {
	case RTK_BT_LE_GP_GAP:
		if (rtk_bt_le_gap_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;
	case RTK_BT_LE_GP_GATTS:
		if (rtk_bt_le_gatts_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;
	case RTK_BT_LE_GP_GATTC:
		if (rtk_bt_le_gattc_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;		
	case RTK_BT_LE_GP_ISO:
		if (rtk_bt_le_iso_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;
	case RTK_BT_BR_GP_A2DP:
		if (rtk_bt_br_a2dp_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;
	case RTK_BT_BR_GP_HFP:
		if (rtk_bt_br_hfp_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;
	case RTK_BT_COMMON_GP_GAP:
		if (rtk_bt_gap_evt_direct_calling_flag & (1<<evt_code))
			ret = true;
		break;
	case RTK_BT_LE_GP_AUDIO:
		if (rtk_bt_le_audio_evt_direct_calling_flag & (1<<evt_code))
			ret = true;			
		break;
	default:
		break;
	}

	return ret;
}
#endif

uint16_t rtk_bt_evt_indicate(void *evt, uint8_t *cb_ret)
{
	rtk_bt_evt_t *p_evt = (rtk_bt_evt_t *)evt;
	(void)p_evt;
	(void)cb_ret;
	
	if(!evt)
		return RTK_BT_ERR_POINTER_INVALID;

#if defined(CONFIG_BT_SINGLE_CORE) && CONFIG_BT_SINGLE_CORE || \
	defined(CONFIG_BT_NP) && CONFIG_BT_NP
	if (true == rtk_bt_check_evt_cb_direct_calling(p_evt->group, p_evt->evt)) {
		if (cb_ret != NULL) {
			*cb_ret = rtk_bt_excute_evt_cb(p_evt->group, p_evt->evt, p_evt->data);
		}
		/* If the evt cb is direct calling, the evt msg isn't actually send to evt task, 
		Instead, it's excuted here, so need to free it here */
		rtk_bt_event_free(evt);
		return 0;
	}
#endif
	if (g_evt_task_running == 0)
		return RTK_BT_ERR_NOT_READY;

	/* If the evt cb is not direct calling, send evt msg to event task */
	if(false == osif_msg_send(g_evt_queue, &evt, BT_TIMEOUT_NONE))
		return RTK_BT_ERR_OS_OPERATION;
	
	return 0;
}

void BT_API_DUMPBUF(uint8_t level, const char * func, uint8_t * buf, uint16_t len)
{
    int i = 0;
    if(level <= BT_API_DEBUG_LEVEL){
        BT_API_PRINT(level,"%s:buf %p, buf len is %d\r\n",func,buf,len);
        for(i = 0; i < len; i++)
        {
            printf("%02x ",buf[i]);
            if((i+1)%16 == 0)
                printf("\r\n");
            }
        printf("\r\n");
    }
}
