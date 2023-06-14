/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_common.h>
#include <bt_ipc_host_api.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_hfp.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

rtk_bt_evt_cb_ret_t bt_ipc_hfp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_hfp_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_hfp_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s: hfp get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_BR_GP_HFP, evt_code, 
									dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_hfp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	switch (p_ipc_msg->API_ID) {

	case RTK_BT_HFP_ACT_CONFIG_AG: {
		rtk_bt_hfp_ag_conf_t *p_ag_conf_t = (rtk_bt_hfp_ag_conf_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_cfg((uint8_t)RTK_BT_AUDIO_HFP_ROLE_AG, (void *)p_ag_conf_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_CONFIG_HF: {
		rtk_bt_hfp_hf_conf_t *p_hf_conf_t = (rtk_bt_hfp_hf_conf_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_cfg((uint8_t)RTK_BT_AUDIO_HFP_ROLE_HF, (void *)p_hf_conf_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_CONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_connect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_DISCONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_disconnect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_SCO_CONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_sco_connect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_SCO_DISCONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_sco_disconnect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_CALL_INCOMING: {
		rtk_bt_hfp_call_incoming_t *p_call_incoming_t = (rtk_bt_hfp_call_incoming_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_call_incoming(p_call_incoming_t->bd_addr,
										p_call_incoming_t->call_num,
										p_call_incoming_t->call_num_len,
										p_call_incoming_t->call_num_type);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_CALL_ANSWER: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_call_answer(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_CALL_TERMINATE: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_call_ternimate(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_HFP_ACT_SEND_SCO_DATA: {
		rtk_bt_hfp_sco_data_send_t *p_data_send_t = (rtk_bt_hfp_sco_data_send_t *)p_ipc_msg->param_buf;
		bt_hfp_ipc_pop_cmd_param(RTK_BT_HFP_ACT_SEND_SCO_DATA, (void *)p_data_send_t);
		ret = rtk_bt_hfp_data_send(p_data_send_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
		break;
	}

	case RTK_BT_HFP_ACT_REPORT_BATT_LEVEL: {
		rtk_bt_hfp_batt_level_t *pbatt_level_t = (rtk_bt_hfp_batt_level_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_hfp_batt_level_report(pbatt_level_t->bd_addr, pbatt_level_t->level);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	default :
		printf("%s, Unknown HFP ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}

#endif