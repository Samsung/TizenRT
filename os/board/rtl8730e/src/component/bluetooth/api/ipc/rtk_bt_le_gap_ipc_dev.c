/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

#if RTK_BLE_5_0_AE_ADV_SUPPORT
typedef struct ext_adv_info_s {
	uint8_t *adv_data;
	uint8_t *scan_rsp_data;
} ext_adv_info_t;

static ext_adv_info_t ipc_dev_ext_adv[16] = {0};
#endif

rtk_bt_evt_cb_ret_t bt_ipc_le_gap_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_le_gap_evt_mem_size(evt_code);
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
	ret = bt_ipc_api_dev_message_send(RTK_BT_LE_GP_GAP, evt_code, 
									dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

__WEAK int rtk_bt_verify(uint32_t param_num, uint32_t *param)
{
	(void)param_num;
	(void)param;

	printf("Error: rtk_bt_verify is not compiled\r\n");
	return 0;
}

__WEAK int rtk_bt_get_verify_cmd_index(char *cmd_str)
{
	(void)cmd_str;

	printf("Error: rtk_bt_get_verify_cmd_index is not compiled\r\n");
	return 0;
}

void bt_le_gap_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;
#if RTK_BLE_5_0_AE_ADV_SUPPORT || RTK_BLE_5_0_PA_ADV_SUPPORT
	uint8_t adv_handle = 0;
#endif

	memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));

	switch (p_ipc_msg->API_ID) {
	case RTK_BT_LE_GAP_ACT_GET_VERSION: {
		rtk_bt_le_version_info_t version = {0};
		ret = rtk_bt_le_gap_get_version(&version);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&version, sizeof(version));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_DEVICE_ADDR: {
		rtk_bt_le_addr_t bd_addr;
		memset(&bd_addr, 0, sizeof(rtk_bt_le_addr_t));
		ret = (uint16_t)rtk_bt_le_gap_get_address(&bd_addr);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&bd_addr, sizeof(bd_addr));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_DEVICE_NAME: {
		const uint8_t *name = (const uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_set_device_name(name);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_RAND_ADDR: {
		rtk_bt_le_set_rand_addr_t *param = (rtk_bt_le_set_rand_addr_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_set_rand_addr(param->auto_generate, param->type, param->addr_val);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_APPEARANCE: {
		uint16_t appearance;
		memcpy((void *)&appearance, (void *)p_ipc_msg->param_buf, sizeof(uint16_t));
		ret = (uint16_t)rtk_bt_le_gap_set_appearance(appearance);
		break;
	}
	case RTK_BT_LE_GAP_ACT_SET_PREFERRED_CONN_PARAM: {
		rtk_bt_le_preferred_conn_param_t *p_preferred_conn_param = (rtk_bt_le_preferred_conn_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_set_preferred_conn_param(p_preferred_conn_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_ADV_DATA: {
		ret = (uint16_t)rtk_bt_le_gap_set_adv_data((uint8_t *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_SCAN_RSP_DATA: {
		ret = (uint16_t)rtk_bt_le_gap_set_scan_rsp_data((uint8_t *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
		break;
	}

	case RTK_BT_LE_GAP_ACT_START_ADV: {
		rtk_bt_le_adv_param_t *adv_param = (rtk_bt_le_adv_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_start_adv(adv_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_STOP_ADV: {
		ret = (uint16_t)rtk_bt_le_gap_stop_adv();
		break;
	}

	case RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER: {
		rtk_bt_le_scan_info_filter_param_t *p_scan_info = (rtk_bt_le_scan_info_filter_param_t *)p_ipc_msg->param_buf;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_SCAN_INFO_FILTER, (void *)p_scan_info);
		ret = (uint16_t)rtk_bt_le_gap_scan_info_filter(p_scan_info);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_SCAN_PARAM: {
		rtk_bt_le_scan_param_t *scan_param = (rtk_bt_le_scan_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_set_scan_param(scan_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_START_SCAN: {
		ret = (uint16_t)rtk_bt_le_gap_start_scan();
		break;
	}

	case RTK_BT_LE_GAP_ACT_STOP_SCAN:
		ret = (uint16_t)rtk_bt_le_gap_stop_scan();
		break;

	case RTK_BT_LE_GAP_ACT_CONN: {
		rtk_bt_le_create_conn_param_t *creat_conn_param = (rtk_bt_le_create_conn_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_connect(creat_conn_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_DISCONN: {
		uint16_t conn_handle;
		memcpy((void *)&conn_handle, (void *)p_ipc_msg->param_buf, sizeof(uint16_t));
		ret = (uint16_t)rtk_bt_le_gap_disconnect(conn_handle);
		break;
	}

#if RTK_BLE_5_0_AE_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV: {
		rtk_bt_le_ext_adv_create_t *p_create = (rtk_bt_le_ext_adv_create_t *)p_ipc_msg->param_buf;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_CREATE_EXT_ADV, (void*)p_create);
		ret = (uint16_t)rtk_bt_le_gap_create_ext_adv(p_create->p_adv_param, &adv_handle);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&adv_handle, sizeof(uint8_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA: {
		rtk_bt_le_ext_adv_data_t *ext_adv_data = 
									(rtk_bt_le_ext_adv_data_t *)p_ipc_msg->param_buf;
		adv_handle = ext_adv_data->adv_handle;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_SET_EXT_ADV_DATA, (void*)ext_adv_data);
		/* if ext adv is not set first time, the previous adv data shall be free and 
		malloc a new memory for new adv_data */
		if (ipc_dev_ext_adv[adv_handle].adv_data) {
			osif_mem_free(ipc_dev_ext_adv[adv_handle].adv_data);
			ipc_dev_ext_adv[adv_handle].adv_data = NULL;
		}
		if (ext_adv_data->len) {
			ipc_dev_ext_adv[adv_handle].adv_data = 
						(uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, ext_adv_data->len);
			memcpy(ipc_dev_ext_adv[adv_handle].adv_data, ext_adv_data->pdata, 
															ext_adv_data->len);
		}
		ret = (uint16_t)rtk_bt_le_gap_set_ext_adv_data(ext_adv_data->adv_handle, 
						ipc_dev_ext_adv[adv_handle].adv_data, ext_adv_data->len);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA: {
		rtk_bt_le_ext_adv_data_t *scan_rsp_data = (rtk_bt_le_ext_adv_data_t *)p_ipc_msg->param_buf;
		adv_handle = scan_rsp_data->adv_handle;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_SET_EXT_SCAN_RSP_DATA, (void*)scan_rsp_data);
		if (ipc_dev_ext_adv[adv_handle].scan_rsp_data) {
			osif_mem_free(ipc_dev_ext_adv[adv_handle].scan_rsp_data);
			ipc_dev_ext_adv[adv_handle].scan_rsp_data = NULL;
		}
		if (scan_rsp_data->len) {
			ipc_dev_ext_adv[adv_handle].scan_rsp_data = 
							(uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, scan_rsp_data->len);
			memcpy(ipc_dev_ext_adv[adv_handle].scan_rsp_data, scan_rsp_data->pdata, 
																scan_rsp_data->len);
		}
		ret = (uint16_t)rtk_bt_le_gap_set_ext_scan_rsp_data(scan_rsp_data->adv_handle, 
						ipc_dev_ext_adv[adv_handle].scan_rsp_data, scan_rsp_data->len);
		break;
	}

	case RTK_BT_LE_GAP_ACT_START_EXT_ADV: {
		rtk_bt_le_ext_adv_start_t *param = (rtk_bt_le_ext_adv_start_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_start_ext_adv(param->adv_handle, param->duration);
		break;
	}

	case RTK_BT_LE_GAP_ACT_STOP_EXT_ADV: {
		adv_handle = p_ipc_msg->param_buf[0];
		ret = rtk_bt_le_gap_stop_ext_adv(adv_handle);
		break;
	}

	case RTK_BT_LE_GAP_ACT_REMOVE_EXT_ADV: {
		adv_handle = p_ipc_msg->param_buf[0];
		if (ipc_dev_ext_adv[adv_handle].adv_data) {
			osif_mem_free(ipc_dev_ext_adv[adv_handle].adv_data);
			ipc_dev_ext_adv[adv_handle].adv_data = NULL;
		}
		if (ipc_dev_ext_adv[adv_handle].scan_rsp_data) {
			osif_mem_free(ipc_dev_ext_adv[adv_handle].scan_rsp_data);
			ipc_dev_ext_adv[adv_handle].scan_rsp_data = NULL;
		}
		ret = rtk_bt_le_gap_remove_ext_adv(adv_handle);
		break;
	}
#endif

#if RTK_BLE_5_0_AE_ADV_SUPPORT || RTK_BLE_5_0_AE_SCAN_SUPPORT
	case RTK_BT_LE_GAP_ACT_EXT_CONN: {
		rtk_bt_le_ext_create_conn_param_t *ext_conn_param = 
								(rtk_bt_le_ext_create_conn_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_ext_connect(ext_conn_param);
		break;
	}
#endif /* RTK_BLE_5_0_AE_ADV_SUPPORT || RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if RTK_BLE_5_0_PA_ADV_SUPPORT
	case RTK_BT_LE_GAP_ACT_START_PA: {
		rtk_bt_le_pa_param_t *padv_param = (rtk_bt_le_pa_param_t *)p_ipc_msg->param_buf;
		adv_handle = padv_param->adv_handle;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_START_PA, (void*)padv_param);
		ret = (uint16_t)rtk_bt_le_gap_start_pa(padv_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_STOP_PA: {
		adv_handle = p_ipc_msg->param_buf[0];
		ret = rtk_bt_le_gap_stop_pa(adv_handle);
		break;
	}

	case RTK_BT_LE_GAP_ACT_UPDATE_PA: {
		rtk_bt_le_pa_update_t *param = (rtk_bt_le_pa_update_t *)p_ipc_msg->param_buf;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_UPDATE_PA, (void*)param);
		ret = rtk_bt_le_gap_update_pa(param->adv_handle, param->update_did_only, param->pa_data, param->pa_len);
		break;
	}
#endif /* RTK_BLE_5_0_PA_ADV_SUPPORT */

#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	case RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM: {
		rtk_bt_le_pa_sync_get_param_t *sync_get = 
								(rtk_bt_le_pa_sync_get_param_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_pa_sync_get_param(sync_get->type, sync_get->p_value,
																sync_get->sync_id);
		if (RTK_BT_OK == ret) {
			uint8_t size = 0;		
			if(sync_get->type == RTK_BT_LE_PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE 
						|| sync_get->type == RTK_BT_LE_PA_SYNC_PARAM_DEV_STATE) {
				size = sizeof(uint8_t); 
			} else if(sync_get->type == RTK_BT_LE_PA_SYNC_PARAM_SYNCHRONIZED_PARAM) {
				size = sizeof(rtk_bt_le_pa_sync_sync_param_t); 
			} else {
				printf("RTK_BT_LE_GAP_ACT_PA_SYNC_GET_PARAM type is error\r\n");
			}
			memcpy((void *)&p_ipc_msg->ret[1], (void *)sync_get->p_value, size);
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_PA_SYNC_MODIFY_ADV_LIST: {
		rtk_bt_le_pa_sync_modify_advlist_t *pa_sync_modify = 
								(rtk_bt_le_pa_sync_modify_advlist_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_pa_sync_modify_adv_list(pa_sync_modify->operation, 
													pa_sync_modify->adv_addr,
													pa_sync_modify->adv_sid);
		break;
	}

	case RTK_BT_LE_GAP_ACT_PA_SYNC_CREATE: {
		rtk_bt_le_pa_sync_create_t *pa_sync_create = 
								(rtk_bt_le_pa_sync_create_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_pa_sync_create(pa_sync_create);
		if (RTK_BT_OK == ret) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)pa_sync_create->p_sync_id, sizeof(uint8_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_PA_SYNC_TERMINATE: {
		uint8_t sync_id = p_ipc_msg->param_buf[0];
		ret = rtk_bt_le_gap_pa_sync_terminate(sync_id);
		break;
	}
#endif /* RTK_BLE_5_0_PA_SYNC_SUPPORT */

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
	case RTK_BT_LE_GAP_ACT_EXT_SCAN_SET_PARAM: {
		rtk_bt_le_ext_scan_param_t *ext_scan_param = 
								(rtk_bt_le_ext_scan_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_ext_scan_set_param(ext_scan_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_EXT_SCAN_START: {
		ret = (uint16_t)rtk_bt_le_gap_ext_scan_start();
		break;
	}

	case RTK_BT_LE_GAP_ACT_EXT_SCAN_STOP: {
		ret = (uint16_t)rtk_bt_le_gap_ext_scan_stop();
		break;
	}
#endif /* RTK_BLE_5_0_AE_SCAN_SUPPORT */

#if RTK_BLE_5_1_PAST_SENDER_SUPPORT
	case RTK_BT_LE_GAP_ACT_PAST_TRANSFER: {
		rtk_bt_le_past_send_t *past_send = (rtk_bt_le_past_send_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_past_send(past_send->conn_handle, past_send->service_data,
										past_send->use_sync_id, past_send->idx);
		break;
	}
#endif /* RTK_BLE_5_1_PAST_SENDER_SUPPORT */

#if RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
	case RTK_BT_LE_GAP_ACT_PAST_RECV_SET: {
		rtk_bt_le_past_recv_param_t *past_recv_set = 
									(rtk_bt_le_past_recv_param_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_past_recipient_set(past_recv_set);
		break;
	}

	case RTK_BT_LE_GAP_ACT_DEFAULT_PAST_RECV_SET: {
		rtk_bt_le_past_recv_param_t *past_recv_set = 
									(rtk_bt_le_past_recv_param_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_default_past_recipient_set(past_recv_set);
		break;
	}
#endif /* RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT */

	case RTK_BT_LE_GAP_ACT_UPDATE_CONN_PARAM: {
		rtk_bt_le_update_conn_param_t *update_conn_param = (rtk_bt_le_update_conn_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_update_conn_param(update_conn_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_MODIFY_WHITELIST: {
		rtk_bt_le_modify_wl_param_t *wl_op_param = (rtk_bt_le_modify_wl_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_modify_whitelist(wl_op_param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_READ_RSSI: {
		rtk_bt_le_read_rssi_param_t read_rssi_param = {0};
		int8_t rssi_val = 0;
		memcpy((void *)&read_rssi_param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_le_read_rssi_param_t));
		ret = (uint16_t)rtk_bt_le_gap_read_rssi(read_rssi_param.conn_handle, &rssi_val);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&rssi_val, sizeof(int8_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_DEV_STATE: {
		rtk_bt_le_gap_dev_state_t dev_state;
		memset(&dev_state, 0, sizeof(rtk_bt_le_gap_dev_state_t));
		ret = (uint16_t)rtk_bt_le_gap_get_dev_state(&dev_state);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&dev_state, sizeof(rtk_bt_le_gap_dev_state_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_ACTIVE_CONN: {
		rtk_bt_le_get_active_conn_t active_conn;
		memset(&active_conn, 0, sizeof(rtk_bt_le_get_active_conn_t));
		ret = (uint16_t)rtk_bt_le_gap_get_active_conn(&active_conn);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&active_conn, sizeof(rtk_bt_le_get_active_conn_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_CONN_HANDLE_BY_ADDR: {
		rtk_bt_le_get_conn_handle_by_addr_param_t conn_handle_param_t = {0};
		uint16_t conn_handle = 0;
		memcpy((void *)&conn_handle_param_t, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_le_get_conn_handle_by_addr_param_t));
		ret = (uint16_t)rtk_bt_le_gap_get_conn_handle_by_addr(&conn_handle_param_t.addr, &conn_handle);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&conn_handle, sizeof(uint16_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_CONN_INFO: {
		rtk_bt_le_get_conn_info_param_t info;
		rtk_bt_le_conn_info_t conn_info;
		memset(&info, 0, sizeof(rtk_bt_le_get_conn_info_param_t));
		memcpy((void *)&info, (void *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
		ret = (uint16_t)rtk_bt_le_gap_get_conn_info(info.conn_handle, &conn_info);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&conn_info, sizeof(conn_info));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_MTU_SIZE: {
		rtk_bt_le_get_mtu_size_param_t get_mtu_size_param_t = {0};
		uint16_t mtu_size = 0;
		memcpy((void *)&get_mtu_size_param_t, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_le_get_mtu_size_param_t));
		ret = (uint16_t)rtk_bt_le_gap_get_mtu_size(get_mtu_size_param_t.conn_handle, &mtu_size);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&mtu_size, sizeof(uint16_t));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_CHANNELS: {
		ret = (uint16_t)rtk_bt_le_gap_set_channels(p_ipc_msg->param_buf);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_DATA_LEN: {
		rtk_bt_le_set_datalen_param_t *conn_data_len = (rtk_bt_le_set_datalen_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_set_data_len(conn_data_len);
		break;
	}

	case RTK_BT_LE_GAP_ACT_SET_PHY: {
		rtk_bt_le_set_phy_param_t *phy_param = (rtk_bt_le_set_phy_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_set_phy(phy_param);
		break;
	}

#if RTK_BLE_PRIVACY_SUPPORT
	case RTK_BT_LE_GAP_ACT_PRIVACY_INIT: {
		ret = (uint16_t)rtk_bt_le_gap_privacy_init(*((bool *)p_ipc_msg->param_buf));
		break;
	}

	case RTK_BT_LE_GAP_ACT_PRIVACY_MODIFY_LIST: {
		rtk_bt_le_modify_resolv_list_t *param = (rtk_bt_le_modify_resolv_list_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_privacy_modify_resolving_list(param);
		break;
	}
#endif

	case RTK_BT_LE_GAP_ACT_SET_SEC_PARAM: {
		rtk_bt_le_security_param_t *param = (rtk_bt_le_security_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_sm_set_security_param(param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_START_SECURITY: {
		uint16_t conn_handle;
		memcpy((void *)&conn_handle, (void *)p_ipc_msg->param_buf, sizeof(uint16_t));
		ret = (uint16_t)rtk_bt_le_sm_start_security(conn_handle);
		break;
	}

	case RTK_BT_LE_GAP_ACT_PAIRING_CONFIRM: {
		rtk_bt_le_pair_cfm_t *param = (rtk_bt_le_pair_cfm_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_sm_pairing_confirm(param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_OOB_KEY_INPUT: {
		rtk_bt_le_set_oob_key_t *param = (rtk_bt_le_set_oob_key_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_sm_set_oob_tk(param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_PASSKEY_ENTRY: {
		rtk_bt_le_auth_key_input_t *param = (rtk_bt_le_auth_key_input_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_sm_passkey_entry(param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_PASSKEY_CONFIRM: {
		rtk_bt_le_auth_key_confirm_t *param = (rtk_bt_le_auth_key_confirm_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_sm_passkey_confirm(param);
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_BOND_NUM: {
		uint8_t bond_num;
		ret = (uint16_t)rtk_bt_le_sm_get_bond_num(&bond_num);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&bond_num, sizeof(bond_num));
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_BOND_INFO: {
		rtk_bt_le_get_bond_info_param_t get_bondinfo = {0};
		memcpy((void *)&get_bondinfo, (void *)p_ipc_msg->param_buf, p_ipc_msg->buf_size);
		uint8_t size = get_bondinfo.size;
		rtk_bt_le_bond_info_t *info = (rtk_bt_le_bond_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_le_bond_info_t)*size);
		memset(info, 0, sizeof(rtk_bt_le_bond_info_t)*size);
		ret = (uint16_t)rtk_bt_le_sm_get_bond_info(info, &size);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&size, sizeof(uint8_t));
			memcpy((void *)&p_ipc_msg->ret[2], (void *)info, sizeof(rtk_bt_le_bond_info_t)*size);
		}
		osif_mem_free(info);
		break;
	}

	case RTK_BT_LE_GAP_ACT_DELETE_BOND_DEVICE: {
		rtk_bt_le_addr_t *bd_addr = (rtk_bt_le_addr_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_sm_delete_bond_device(bd_addr);
		break;
	}

	case RTK_BT_LE_GAP_ACT_CLEAR_BOND_LIST: {
		ret = (uint16_t)rtk_bt_le_sm_clear_bond_list();
		break;
	}

	case RTK_BT_LE_GAP_ACT_GET_TX_PENDING_NUM: {
		uint16_t tx_pending_num = 0;
		rtk_bt_le_get_tx_pending_num_param_t *param = (rtk_bt_le_get_tx_pending_num_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_gap_get_tx_pending_num(param->conn_handle, &tx_pending_num);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&tx_pending_num, sizeof(uint16_t));
		}
		break;
	}
	case RTK_BT_LE_GAP_ACT_VENDOR_CMD_REQ: {
		rtk_bt_le_gap_vendor_cmd_param_t *vendor_param = 
						(rtk_bt_le_gap_vendor_cmd_param_t *)p_ipc_msg->param_buf;
		bt_le_ipc_pop_cmd_param(RTK_BT_LE_GAP_ACT_VENDOR_CMD_REQ, (void *)vendor_param);
		ret = (uint16_t)rtk_bt_le_gap_vendor_cmd_req(vendor_param);
		break;
	}

#if RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	case RTK_BT_LE_GAP_ACT_READ_LOCAL_TX_POWER: {
		rtk_bt_le_txpower_read_local_t *param = (rtk_bt_le_txpower_read_local_t *)p_ipc_msg->param_buf;
		int8_t cur_txpower, max_txpower;
		int8_t *power_ret = (int8_t *)&(p_ipc_msg->ret[1]);
		ret = rtk_bt_le_gap_read_local_tx_power(param->conn_handle, param->phy, &cur_txpower, &max_txpower);
		if (ret == 0) {
			power_ret[0] = cur_txpower;
			power_ret[1] = max_txpower;
		}
		break;
	}

	case RTK_BT_LE_GAP_ACT_READ_REMOTE_TX_POWER: {
		rtk_bt_le_txpower_read_remote_t *param = (rtk_bt_le_txpower_read_remote_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_read_remote_tx_power(param->conn_handle, param->phy);
		break;
	}
	
	case RTK_BT_LE_GAP_ACT_TX_POWER_REPORT_SET: {
		rtk_bt_le_txpower_report_set_t *param = (rtk_bt_le_txpower_report_set_t *)p_ipc_msg->param_buf;
		ret = rtk_bt_le_gap_tx_power_report_set(param->conn_handle, param->local_enable, param->remote_enable);
		break;
	}
#endif

	case RTK_BT_LE_GAP_ACT_GET_CONN_ID: {
		rtk_bt_le_get_conn_id_t *param = (rtk_bt_le_get_conn_id_t *)p_ipc_msg->param_buf;
		uint8_t conn_id;
		ret = rtk_bt_le_gap_get_conn_id(param->conn_handle, &conn_id);
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&conn_id, sizeof(uint8_t));
		}
		break;
	}

	default :
		printf("%s, Unknown GAP IPC RTK_BLE_ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	p_ipc_msg->ret[0] = ret;
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}