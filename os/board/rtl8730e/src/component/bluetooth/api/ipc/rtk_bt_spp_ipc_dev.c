/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_common.h>
#include <bt_ipc_host_api.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_spp.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

rtk_bt_evt_cb_ret_t bt_ipc_spp_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_spp_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_spp_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s: spp get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_BR_GP_SPP, evt_code, 
									dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_spp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	switch (p_ipc_msg->API_ID) {

	case RTK_BT_SPP_ACT_INIT_CFG: {
		rtk_bt_spp_init_cfg_t *pinit_param = (rtk_bt_spp_init_cfg_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_spp_init_cfg(pinit_param->spp_max_link_num, pinit_param->spp_rfc_max_service_num, 
											pinit_param->default_mtu_size, pinit_param->default_credits);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}
	
	case RTK_BT_SPP_ACT_SRV_CFG: {
		rtk_bt_spp_srv_cfg_t *psrv_param = (rtk_bt_spp_srv_cfg_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_service_register_cfg((void *)psrv_param->srv_class_uuid128, psrv_param->length, psrv_param->local_server_chann);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_SPP_ACT_CONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_spp_connect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_SPP_ACT_DISCONNECT: {
		rtk_bt_spp_disconn_req_t *p_disconn_req_t = (rtk_bt_spp_disconn_req_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_spp_disconnect(p_disconn_req_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_SPP_ACT_DISCONNECT_ALL: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_spp_disconnect_all(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_SPP_ACT_GIVE_CREDITS: {
		rtk_bt_spp_credits_give_t *p_credits_give_t = (rtk_bt_spp_credits_give_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_spp_credits_give(p_credits_give_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_SPP_ACT_SEND_DATA: {
		rtk_bt_spp_send_data_t *p_data_send_t = (rtk_bt_spp_send_data_t *)p_ipc_msg->param_buf;
		bt_spp_ipc_pop_cmd_param(RTK_BT_SPP_ACT_SEND_DATA, (void *)p_data_send_t);
		ret = rtk_bt_spp_send_data(p_data_send_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	default :
		printf("%s, Unknown SPP ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}

#endif