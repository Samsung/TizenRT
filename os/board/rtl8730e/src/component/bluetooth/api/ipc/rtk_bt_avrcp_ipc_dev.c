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
#include <rtk_bt_avrcp.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

rtk_bt_evt_cb_ret_t bt_ipc_avrcp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_avrcp_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_avrcp_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s: avrcp get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_BR_GP_AVRCP, evt_code, 
									dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_avrcp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	switch (p_ipc_msg->API_ID) {

	case RTK_BT_AVRCP_ACT_CONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_connect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_DISCONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_disconnect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_PLAY: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_play(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_PAUSE: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_pause(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_STOP: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_stop(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_FORWARD: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_forward(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_BACKWARD: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_backward(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_REWIND_START: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_rewind_start(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_REWIND_STOP: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_rewind_stop(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_FAST_FORWARD_START: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_fast_forward_start(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_FAST_FORWARD_STOP: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_fast_forward_stop(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_ABSOLUTE_VOLUME_SET: {
		rtk_bt_avrcp_absolute_volume_set_t *p_volume_t = (rtk_bt_avrcp_absolute_volume_set_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_absolute_volume_set(p_volume_t->bd_addr, p_volume_t->volume);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_AVRCP_ACT_VOLUME_CHANGE_REQ: {
		rtk_bt_avrcp_volume_change_req_t *p_volume_t = (rtk_bt_avrcp_volume_change_req_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_avrcp_volume_change_req(p_volume_t->bd_addr, p_volume_t->volume);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	default :
		printf("%s, Unknown AVRCP ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}

#endif