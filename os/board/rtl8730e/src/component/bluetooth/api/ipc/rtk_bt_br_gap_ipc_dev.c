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
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

rtk_bt_evt_cb_ret_t bt_ipc_br_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_br_gap_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_br_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s: le get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_BR_GP_GAP, evt_code, 
									dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_br_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	switch (p_ipc_msg->API_ID) {

	case RTK_BT_BR_GAP_ACT_SET_DEFAULT_PARAM: {
		ret = (uint16_t)rtk_bt_br_gap_set_default_param((rtk_bt_br_gap_default_param_t *)p_ipc_msg->param_buf);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR: {
		rtk_bt_br_bd_addr_t bd_addr;
		memset(&bd_addr, 0, sizeof(rtk_bt_br_bd_addr_t));
		ret = (uint16_t)rtk_bt_br_gap_get_address(&bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&bd_addr, sizeof(rtk_bt_br_bd_addr_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_DEVICE_NAME: {
		const uint8_t *name = (const uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_set_device_name(name);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_PAGE_PARAM: {
		rtk_bt_br_page_param_t *page_param = (rtk_bt_br_page_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_set_page_param(page_param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_CFG_PAGE_PARAM: {
		rtk_bt_br_page_param_t *page_param = (rtk_bt_br_page_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_cfg_page_param(page_param->pagescan_type,
													page_param->pagescan_interval,
													page_param->pagescan_window);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_INQUIRY_PARAM: {
		rtk_bt_br_inquiry_param_t *inquiry_param = (rtk_bt_br_inquiry_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_set_inquiry_param(inquiry_param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_CFG_INQUIRY_PARAM: {
		rtk_bt_br_inquiry_param_t *inquiry_param = (rtk_bt_br_inquiry_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_cfg_inquiry_param(inquiry_param->inquiryscan_type,
														inquiry_param->inquiryscan_interval,
														inquiry_param->inquiryscan_window);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_DISCONNECT: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_br_gap_disconnect(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_START_INQUIRY: {
		rtk_bt_br_inquiry_start_t *start_inquiry_param = (rtk_bt_br_inquiry_start_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_start_inquiry(start_inquiry_param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_GET_REMOTE_NAME: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_get_remote_name(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_MAX_BOND_NUM_GET: {
		uint8_t max_bond_num = 0;
		ret = (uint16_t)rtk_bt_br_gap_max_bond_num_get(&max_bond_num);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&max_bond_num, sizeof(uint8_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_NUM_GET: {
		uint8_t bond_num = 0;
		ret = (uint16_t)rtk_bt_br_gap_bond_num_get(&bond_num);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&bond_num, sizeof(uint8_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_ADDR_GET: {
		rtk_bt_br_bond_addr_t *pbond_addr_t = (rtk_bt_br_bond_addr_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_bond_addr_get(pbond_addr_t->priority, pbond_addr_t->bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)pbond_addr_t->bd_addr, 6);
		}
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_INDEX_GET: {
		rtk_bt_br_bond_index_t *pbond_index_t = (rtk_bt_br_bond_index_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_bond_index_get(&pbond_index_t->index, pbond_index_t->bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&pbond_index_t->index, sizeof(uint8_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_KEY_GET: {
		rtk_bt_br_bond_key_t *pbond_key_t = (rtk_bt_br_bond_key_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_bond_key_get(pbond_key_t->bd_addr, &pbond_key_t->key_type, pbond_key_t->link_key);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)pbond_key_t, sizeof(rtk_bt_br_bond_key_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_KEY_SET: {
		rtk_bt_br_bond_key_t *pbond_key_t = (rtk_bt_br_bond_key_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_bond_key_set(pbond_key_t);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_PRIORITY_SET: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_bond_priority_set(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_DELETE: {
		uint8_t *bd_addr = (uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_br_gap_bond_delete(bd_addr);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_BOND_CLEAR: {
		ret = (uint16_t)rtk_bt_br_gap_bond_clear();
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_PINCODE: {
		ret = (uint16_t)rtk_bt_br_gap_set_pincode((uint8_t *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_RADIO_MODE: {
		ret = (uint16_t)rtk_bt_br_gap_set_radio_mode((uint8_t)p_ipc_msg->param_buf[0]);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_COD: {
		ret = (uint16_t)rtk_bt_br_gap_set_cod((uint32_t *)p_ipc_msg->param_buf);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_BR_GAP_ACT_SET_SUPV_TIMEOUT: {
		ret = (uint16_t)rtk_bt_br_gap_set_supvisiontimeout((uint16_t *)p_ipc_msg->param_buf);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	default :
		printf("%s, Unknown GAP IPC RTK_BR_ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}

#endif