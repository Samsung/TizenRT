/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_gattc.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

/* ===============================================rtk_bt_gatts ipc action======================================= */
rtk_bt_evt_cb_ret_t bt_ipc_gatts_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;

	// data_len = get_bt_gatts_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_gatts_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s gatts get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_LE_GP_GATTS, evt_code, 
										dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_gatts_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	switch (p_ipc_msg->API_ID) {
	case RTK_BT_GATTS_ACT_REGISTER_SERVICE: {
		uint8_t app_id = ((struct rtk_bt_gatt_service *)p_ipc_msg->param_buf)->app_id;
		uint32_t srv_size = p_ipc_msg->buf_size;
		struct rtk_bt_gatt_service *srv_reg = osif_mem_alloc(RAM_TYPE_DATA_ON, srv_size);
		memset(srv_reg, 0, srv_size);
		memcpy(srv_reg, p_ipc_msg->param_buf, srv_size);
		srv_reg->alloc_ind = 1;
		bt_gatts_ipc_pop_cmd_param(RTK_BT_GATTS_ACT_REGISTER_SERVICE, (void *)srv_reg);
		ret = (uint16_t)rtk_bt_gatts_register_service(srv_reg);
		if (ret == RTK_BT_ERR_ALREADY_IN_PROGRESS) {
			printf("Service %d already exist \r\n", app_id);
			osif_mem_free(srv_reg);
		}
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_GATTS_ACT_NOTIFY: {
		rtk_bt_gatts_ntf_and_ind_param_t *ntf_param = (rtk_bt_gatts_ntf_and_ind_param_t *)p_ipc_msg->param_buf;
		bt_gatts_ipc_pop_cmd_param(RTK_BT_GATTS_ACT_NOTIFY, (void *)ntf_param);
		ret = (uint16_t)rtk_bt_gatts_notify(ntf_param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_GATTS_ACT_INDICATE: {
		rtk_bt_gatts_ntf_and_ind_param_t *ind_param = (rtk_bt_gatts_ntf_and_ind_param_t *)p_ipc_msg->param_buf;
		bt_gatts_ipc_pop_cmd_param(RTK_BT_GATTS_ACT_INDICATE, (void *)ind_param);
		ret = (uint16_t)rtk_bt_gatts_indicate(ind_param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_GATTS_ACT_READ_RSP: {
		rtk_bt_gatts_read_resp_param_t *resp_read = (rtk_bt_gatts_read_resp_param_t *)p_ipc_msg->param_buf;
		bt_gatts_ipc_pop_cmd_param(RTK_BT_GATTS_ACT_READ_RSP, (void *)resp_read);
		ret = (uint16_t)rtk_bt_gatts_read_resp(resp_read);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	case RTK_BT_GATTS_ACT_WRITE_RSP: {
		rtk_bt_gatts_write_resp_param_t *resp_write = (rtk_bt_gatts_write_resp_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_gatts_write_resp(resp_write);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;
	}

	default:
		printf("%s, Unknown GATTS IPC RTK_BT_ACT: %d\r\n ", __func__, (int)p_ipc_msg->API_ID);
		break;
	}
	
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}

/* ===============================================rtk_bt_gattc ipc action======================================= */
rtk_bt_evt_cb_ret_t bt_ipc_gattc_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;

	// data_len = get_bt_gattc_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_gattc_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s gatts get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_LE_GP_GATTC, evt_code, 
										dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_gattc_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));

	switch (p_ipc_msg->API_ID) {
	case RTK_BT_GATTC_ACT_REGISTER_PROFILE: {
#if RTK_BLE_MGR_LIB
		rtk_bt_gattc_register_t param = {0};
		memcpy((void *)&param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_register_t));
		ret = (uint16_t)rtk_bt_gattc_register_profile(param.profile_id, param.srv_uuid);
#else
		uint16_t profile_id;
		memcpy((void *)&profile_id, (void *)p_ipc_msg->param_buf, 2);
		ret = (uint16_t)rtk_bt_gattc_register_profile(profile_id);
#endif
		break;
	}

	case RTK_BT_GATTC_ACT_UNREGISTER_PROFILE: {
#if RTK_BLE_MGR_LIB
		rtk_bt_gattc_register_t param = {0};
		memcpy((void *)&param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_register_t));
		ret = (uint16_t)rtk_bt_gattc_register_profile(param.profile_id, param.srv_uuid);
#else
		uint16_t profile_id;
		memcpy((void *)&profile_id, (void *)p_ipc_msg->param_buf, 2);
		ret = (uint16_t)rtk_bt_gattc_unregister_profile(profile_id);
#endif
		break;
	}

#if !RTK_BLE_MGR_LIB
	case RTK_BT_GATTC_ACT_ATTACH_CONN: {
		uint32_t attch_id;
		uint16_t profile_id, conn_handle;
		memcpy((void *)&attch_id, (void *)p_ipc_msg->param_buf, 4);
		profile_id = RTK_BT_GET_PROFILE_ID(attch_id);
		conn_handle = RTK_BT_GET_CONN_HANDLE(attch_id);
		ret = (uint16_t)rtk_bt_gattc_attach_connect(profile_id, conn_handle);
		break;
	}

	case RTK_BT_GATTC_ACT_DETACH_CONN: {
		uint32_t attch_id;
		uint16_t profile_id, conn_handle;
		memcpy((void *)&attch_id, (void *)p_ipc_msg->param_buf, 4);
		profile_id = RTK_BT_GET_PROFILE_ID(attch_id);
		conn_handle = RTK_BT_GET_CONN_HANDLE(attch_id);
		ret = (uint16_t)rtk_bt_gattc_detach_connect(profile_id, conn_handle);
		break;
	}
#endif

	case RTK_BT_GATTC_ACT_DISCOVER: {
#if RTK_BLE_MGR_LIB
		ret = rtk_bt_gattc_discover_all(*((uint16_t *)p_ipc_msg->param_buf));
#else
		rtk_bt_gattc_discover_param_t dis_param = {0};
		memcpy((void *)&dis_param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_discover_param_t));
		ret = rtk_bt_gattc_discover(&dis_param);
#endif
		break;
	}

#if RTK_BLE_MGR_LIB
	case RTK_BT_GATTC_ACT_FIND: {
		rtk_bt_gattc_find_param_t find_param = {0};
		memcpy((void *)&find_param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_find_param_t));
		ret = rtk_bt_gattc_find(&find_param);
		if(ret == 0) {
			uint8_t *dst = (uint8_t *)&p_ipc_msg->ret[1];
			if (find_param.type == RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE) {
				memcpy((void *)dst, (void *)find_param.find_char.p_handle, sizeof(uint16_t));
			} else if (find_param.type == RTK_BT_GATT_FIND_CHARACTERISTIC_DESC_RANGE) {
				memcpy((void *)dst, (void *)find_param.find_char_range.p_handle_start, sizeof(uint16_t));
				dst += sizeof(uint16_t);
				memcpy((void *)dst, (void *)find_param.find_char_range.p_handle_end, sizeof(uint16_t));
			} else if (find_param.type == RTK_BT_GATT_FIND_CHARACTERISTIC_CCCD_HANDLE) {
				memcpy((void *)dst, (void *)find_param.find_char_cccd.p_handle, sizeof(uint16_t));
				dst += sizeof(uint16_t);
				memcpy((void *)dst, (void *)find_param.find_char_cccd.p_notify, sizeof(bool));
				dst += sizeof(uint8_t);
				memcpy((void *)dst, (void *)find_param.find_char_cccd.p_indicate, sizeof(bool));
			} else if (find_param.type == RTK_BT_GATT_FIND_INCLUDE_SERVICE) {
				memcpy((void *)dst, (void *)find_param.find_include_srv.p_instance_num, sizeof(uint8_t));
				dst += sizeof(uint8_t);
				memcpy((void *)dst, (void *)find_param.find_include_srv.instance_id, sizeof(find_param.find_include_srv.instance_id));
			}
		}
		break;
	}
#endif

	case RTK_BT_GATTC_ACT_READ: {
		rtk_bt_gattc_read_param_t *p_read_param = (rtk_bt_gattc_read_param_t *)p_ipc_msg->param_buf;
		bt_gattc_ipc_pop_cmd_param(RTK_BT_GATTC_ACT_READ, (void *)p_read_param);
		ret = (uint16_t)rtk_bt_gattc_read(p_read_param);
		break;
	}

	case RTK_BT_GATTC_ACT_WRITE: {
		rtk_bt_gattc_write_param_t *p_write_param = (rtk_bt_gattc_write_param_t *)p_ipc_msg->param_buf;
		bt_gattc_ipc_pop_cmd_param(RTK_BT_GATTC_ACT_WRITE, (void *)p_write_param);
		ret = (uint16_t)rtk_bt_gattc_write(p_write_param);
		break;
	}

	case RTK_BT_GATTC_ACT_CCCD_ENABLE: {
		rtk_bt_gattc_update_cccd_param_t update_cccd_param = {0};
		memcpy((void *)&update_cccd_param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_update_cccd_param_t));
		ret = (uint16_t)rtk_bt_gattc_enable_notify_or_indicate(&update_cccd_param);
		break;
	}

	case RTK_BT_GATTC_ACT_CCCD_DISABLE: {
		rtk_bt_gattc_update_cccd_param_t update_cccd_param = {0};
		memcpy((void *)&update_cccd_param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_update_cccd_param_t));
		ret = (uint16_t)rtk_bt_gattc_disable_notify_or_indicate(&update_cccd_param);


		break;
	}

	case RTK_BT_GATTC_ACT_CONFIRM: {
		rtk_bt_gattc_cfm_param_t cfm_param = {0};
		memcpy((void *)&cfm_param, (void *)p_ipc_msg->param_buf, sizeof(rtk_bt_gattc_cfm_param_t));
		ret = (uint16_t)rtk_bt_gattc_confirm(&cfm_param);
		break;
	}

	default:
		printf("%s, Unknown GATTS IPC RTK_BT_ACT: %d\r\n ", __func__, (int)p_ipc_msg->API_ID);
		break;
	}
	
	p_ipc_msg->ret[0] = ret;
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}