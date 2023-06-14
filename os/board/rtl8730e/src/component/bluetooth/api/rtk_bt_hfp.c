/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <platform_opts_bt.h>
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <rtk_bt_hfp.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_profile_config.h>
#endif

#if !defined(CONFIG_BT_AP) || (!CONFIG_BT_AP)
extern rtk_bt_hfp_ag_conf_t ag_conf;
extern rtk_bt_hfp_hf_conf_t hf_conf;
#endif

uint16_t rtk_bt_hfp_cfg(uint8_t role, void *p_conf_t)
{
	/* Check bt enable status (rtk_bt_hfp_cfg should only be invoked before RTK_BT_IS_ENABLED()) */
	if (rtk_bt_is_enable()) {
		printf("[HFP]: rtk_bt_hfp_cfg should not be invoked after bt enable \r\n");
		return RTK_BT_FAIL;
	}
	if (!p_conf_t) {
		printf("[HFP]: p_conf_t is NULL \r\n");
		return RTK_BT_FAIL;
	}
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;
		uint16_t err;

		if (RTK_BT_AUDIO_HFP_ROLE_AG == role) {
			ret = bt_ipc_api_host_message_send(RTK_BT_IPC_HFP, RTK_BT_HFP_ACT_CONFIG_AG,
									(uint8_t *)p_conf_t, sizeof(rtk_bt_hfp_ag_conf_t));
		} else if (RTK_BT_AUDIO_HFP_ROLE_HF == role) {
			ret = bt_ipc_api_host_message_send(RTK_BT_IPC_HFP, RTK_BT_HFP_ACT_CONFIG_HF,
									(uint8_t *)p_conf_t, sizeof(rtk_bt_hfp_hf_conf_t));
		} else {
			printf("[HFP]: unknow role %d \r\n", role);
			return RTK_BT_FAIL;
		}
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		err = ret[0];
		osif_mem_free(ret);

		return err;
	}
#else
	switch (role) {
	case RTK_BT_AUDIO_HFP_ROLE_AG: {
		memcpy((void *)&ag_conf, p_conf_t, sizeof(rtk_bt_hfp_ag_conf_t));
	}
	break;

	case RTK_BT_AUDIO_HFP_ROLE_HF: {
		memcpy((void *)&hf_conf, p_conf_t, sizeof(rtk_bt_hfp_hf_conf_t));
	}
	break;

	default : {
		printf("[HFP]: Unknown role %d \r\n", role);
	}
	break;
	}

	return RTK_BT_OK;
#endif
}

uint16_t rtk_bt_hfp_connect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_CONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_hfp_disconnect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_DISCONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_hfp_sco_connect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_SCO_CONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_hfp_sco_disconnect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_SCO_DISCONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_hfp_call_incoming(uint8_t *bd_addr, const char *call_num, uint8_t call_num_len, uint8_t call_num_type)
{
	uint16_t ret = 0;
	rtk_bt_hfp_call_incoming_t call_incoming_t = {0};

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr || !call_num)
		return RTK_BT_ERR_POINTER_INVALID;

	if (call_num_len > 20)
		return RTK_BT_ERR_PARAM_INVALID;

	memcpy((void *)&call_incoming_t.bd_addr, bd_addr, 6);
	memcpy((void *)call_incoming_t.call_num, call_num, call_num_len);
	call_incoming_t.call_num_len = call_num_len;
	call_incoming_t.call_num_type = call_num_type;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_CALL_INCOMING, 
							(uint8_t *)&call_incoming_t, sizeof(rtk_bt_hfp_call_incoming_t));

	return ret;
}

uint16_t rtk_bt_hfp_call_answer(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_CALL_ANSWER, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_hfp_call_ternimate(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_CALL_TERMINATE, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_hfp_data_send(rtk_bt_hfp_sco_data_send_t *p_data_send_t)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!p_data_send_t)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_SEND_SCO_DATA, 
							p_data_send_t, sizeof(rtk_bt_hfp_sco_data_send_t));

	return ret;
}

uint16_t rtk_bt_hfp_batt_level_report(uint8_t *bd_addr, uint8_t power_level)
{
	uint16_t ret = 0;
	rtk_bt_hfp_batt_level_t batt_level_t = {0};

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	memcpy((void *)batt_level_t.bd_addr, (void *)bd_addr, 6);
	batt_level_t.level = power_level;
	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_HFP, RTK_BT_HFP_ACT_REPORT_BATT_LEVEL, 
							(uint8_t *)&batt_level_t, sizeof(rtk_bt_hfp_batt_level_t));

	return ret;
}