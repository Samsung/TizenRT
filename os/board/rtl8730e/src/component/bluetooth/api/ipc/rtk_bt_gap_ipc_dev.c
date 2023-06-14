/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_gap.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

rtk_bt_evt_cb_ret_t bt_ipc_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_gap_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_le_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s: le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_COMMON_GP_GAP, evt_code, 
									  dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = RTK_BT_FAIL;

	memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));

	switch (p_ipc_msg->API_ID) {

#if RTK_BT_5_2_L2C_ECFC_SUPPORT
	case RTK_BT_GAP_ACT_ECFC_SET_PARAM: {
		rtk_bt_gap_ecfc_param_set_t *param = (rtk_bt_gap_ecfc_param_set_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_gap_ecfc_set_param(param->type, param->value);
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_SEC_REG_REQ: {
		rtk_bt_gap_ecfc_sec_reg_req_t *param = (rtk_bt_gap_ecfc_sec_reg_req_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_gap_ecfc_send_sec_reg_req(param);
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_REGISTER: {
		rtk_bt_gap_ecfc_register_t *param = (rtk_bt_gap_ecfc_register_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_gap_ecfc_register(param);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)param->p_proto_id, sizeof(uint16_t));
		}
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_CONN_REQ: {
		rtk_bt_gap_ecfc_conn_req_t *param = (rtk_bt_gap_ecfc_conn_req_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_gap_ecfc_send_conn_req(param);
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_CONN_CFM: {
		rtk_bt_gap_ecfc_conn_cfm_t *param = (rtk_bt_gap_ecfc_conn_cfm_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_gap_ecfc_send_conn_cfm(param);
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_DISCONN_REQ: {
		rtk_bt_gap_ecfc_disconn_req_t *param = (rtk_bt_gap_ecfc_disconn_req_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_gap_ecfc_send_disconn_req(param->conn_handle, param->cid);
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_SEND_DATA: {
		rtk_bt_gap_ecfc_send_data_t *param = (rtk_bt_gap_ecfc_send_data_t *)p_ipc_msg->param_buf;
		bt_gap_ipc_pop_cmd_param(RTK_BT_GAP_ACT_ECFC_SEND_DATA, (void *)param);
		ret = (uint16_t)rtk_bt_gap_ecfc_send_data(param->conn_handle, param->cid, param->p_data, param->length);
		break;
	}

	case RTK_BT_GAP_ACT_ECFC_RECONF_REQ: {
		rtk_bt_gap_ecfc_reconf_req_t *param = (rtk_bt_gap_ecfc_reconf_req_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_gap_ecfc_send_reconf_req(param);
		break;
	}
#endif

	default :
		printf("%s, Unknown GAP IPC RTK_BLE_ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	p_ipc_msg->ret[0] = ret;
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}