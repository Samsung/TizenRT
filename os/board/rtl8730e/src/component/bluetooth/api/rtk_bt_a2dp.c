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
#include <rtk_bt_a2dp.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_host_drc.h>
#include <bt_ipc_profile_config.h>
#endif

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
extern bool rtk_bt_check_act_direct_calling(uint8_t group, uint8_t act_code);
#else
extern rtk_bt_a2dp_media_codec_sbc_t rtk_codec_sbc;
extern rtk_bt_a2dp_media_codec_aac_t rtk_codec_aac;
#endif

uint16_t rtk_bt_a2dp_codec_cfg(uint8_t codec_type, void *codec_t, uint32_t length)
{
	/* Check bt enable status (rtk_bt_a2dp_codec_cfg should only be invoked before RTK_BT_IS_ENABLED()) */
	if (rtk_bt_is_enable()) {
		printf("[A2DP]: rtk_bt_a2dp_codec_cfg should not be invoked after bt enable \r\n");
		return RTK_BT_FAIL;
	}

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	{
		int *ret = NULL;
		uint16_t err;
		rtk_bt_a2dp_codec_param_t *pcodec_param = NULL;

		if (length > RTK_BT_A2DP_MAX_CODEC_PARAM_LENGTH) {
			printf("The param of %s is over flow\r\n", __func__);
			return RTK_BT_FAIL;
		}
		pcodec_param = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_a2dp_codec_param_t));
		if (!pcodec_param) {
			printf("%s allocate codec struct fail \r\n", __func__);
			return RTK_BT_FAIL;
		}
		memcpy((void *)pcodec_param->data, codec_t, length);
		pcodec_param->length = length;
		pcodec_param->codec_type = codec_type;
		if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_A2DP, RTK_BT_A2DP_ACT_CODEC_CFG)) {
			ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_A2DP, RTK_BT_A2DP_ACT_CODEC_CFG,
									(uint8_t *)pcodec_param, sizeof(rtk_bt_a2dp_codec_param_t));
		} else {
			ret = bt_ipc_api_host_message_send(RTK_BT_IPC_A2DP, RTK_BT_A2DP_ACT_CODEC_CFG,
									(uint8_t *)pcodec_param, sizeof(rtk_bt_a2dp_codec_param_t));
		}
		if (ret[0] != RTK_BT_OK) {
			printf("[core AP][IPC] %s fail ! \r\n", __func__);
		}
		err = ret[0];
		osif_mem_free(ret);
		osif_mem_free(pcodec_param);

		return err;
	}
#else
	switch (codec_type) {
	case RTK_BT_AUDIO_CODEC_SBC: {
		memcpy((void *)&rtk_codec_sbc, codec_t, length);
	}
	break;

	case RTK_BT_AUDIO_CODEC_AAC: {
		memcpy((void *)&rtk_codec_aac, codec_t, length);
	}
	break;

	default : {
		printf("[A2DP]: Unknown codec type %d \r\n", codec_type);
	}
	break;
	}

	return RTK_BT_OK;
#endif
}

uint16_t rtk_bt_a2dp_connect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_A2DP, RTK_BT_A2DP_ACT_CONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_a2dp_disconnect(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_A2DP, RTK_BT_A2DP_ACT_DISCONNECT, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_a2dp_start(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_A2DP, RTK_BT_A2DP_ACT_START, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_a2dp_suspend(uint8_t *bd_addr)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!bd_addr)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_A2DP, RTK_BT_A2DP_ACT_SUSPEND, 
							bd_addr, 6);

	return ret;
}

uint16_t rtk_bt_a2dp_data_send(rtk_bt_a2dp_stream_data_send_t *p_data_send_t)
{
	uint16_t ret = 0;

	if (!rtk_bt_is_enable())
		return RTK_BT_ERR_NOT_READY;

	if (!p_data_send_t)
		return RTK_BT_ERR_POINTER_INVALID;

	ret = rtk_bt_send_cmd(RTK_BT_BR_GP_A2DP, RTK_BT_A2DP_ACT_SEND_DATA, 
							p_data_send_t, sizeof(rtk_bt_a2dp_stream_data_send_t));

	return ret;
}
