/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#if (defined RTK_BLE_ISO_SUPPORT) && (RTK_BLE_ISO_SUPPORT)
#include <rtk_bt_common.h>
#include <bt_ipc_host_api.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_le_iso.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_parameter_conversion.h>

rtk_bt_evt_cb_ret_t bt_ipc_le_iso_cb_dev_to_host(uint8_t evt_code, void *data, uint32_t data_len)
{
	int *ret = NULL;
	uint16_t err = RTK_BT_EVT_CB_OK;
	// uint32_t data_len = 0;
	uint32_t actual_size = 0;
	bt_ipc_dev_request_message *dev_msg = NULL;
	
	// data_len = get_bt_le_iso_evt_mem_size(evt_code);
	// if (data_len == 0xFFFFFFFF) {
	// 	printf("%s: The param of %d is not in the evt table\r\n", __func__, evt_code);
	// 	return err;
	// }
	if (data_len > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		return err;
	}
	dev_msg = (bt_ipc_dev_request_message *)bt_le_iso_push_event_ipc_buf(evt_code, data, data_len, &actual_size);
	if (!dev_msg) {
		printf("%s: le iso get ipc buf fail \r\n", __func__);
		return err;
	}
	if (actual_size > IPC_DEV_API_DATA_MAX) {
		printf("%s: The param of %d is over ipc message memory\r\n", __func__, evt_code);
		goto exit;
	}
	ret = bt_ipc_api_dev_message_send(RTK_BT_LE_GP_ISO, evt_code, 
									dev_msg->param_buf, actual_size);
	err = (uint16_t)ret[0];
	osif_mem_free(ret);

exit:
	osif_mem_free(dev_msg);

	return err;
}

void bt_le_iso_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;

	switch (p_ipc_msg->API_ID) {
#if (defined(RTK_BLE_ISO_CIS_SUPPORT) && RTK_BLE_ISO_CIS_SUPPORT)		
	case RTK_BT_LE_ISO_ACT_CIG_SETUP_PATH: {
		rtk_bt_le_iso_setup_path_param_t * param = (rtk_bt_le_iso_setup_path_param_t *)p_ipc_msg->param_buf;
		bt_le_iso_ipc_pop_cmd_param(RTK_BT_LE_ISO_ACT_CIG_SETUP_PATH, (void *)param);
		ret = (uint16_t)rtk_bt_le_iso_cig_setup_path(param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_CIG_REMOVE_PATH: {
		rtk_bt_le_iso_remove_path_param_t * param = (rtk_bt_le_iso_remove_path_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_remove_path(param->iso_conn_handle,param->data_path_direction);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_DISCONNECT: {
		uint16_t cis_conn_handle = *(uint16_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_disconnect(cis_conn_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_READ_ISO_TX_SYNC: {
		rtk_bt_le_iso_cig_read_iso_tx_sync_info_t tx_sync_info = {0};
		rtk_bt_le_iso_cig_read_iso_tx_sync_t *param = (rtk_bt_le_iso_cig_read_iso_tx_sync_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_read_iso_tx_sync(param->cis_conn_handle,&tx_sync_info);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&tx_sync_info, sizeof(rtk_bt_le_iso_cig_read_iso_tx_sync_info_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_CIG_READ_LINK_QUALITY: {
		rtk_bt_le_iso_cig_read_link_quality_info_t link_quality_info = {0};
		rtk_bt_le_iso_cig_read_link_quality_t *param = (rtk_bt_le_iso_cig_read_link_quality_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_read_link_quality(param->cis_conn_handle,&link_quality_info);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&link_quality_info, sizeof(rtk_bt_le_iso_cig_read_link_quality_info_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;	
	}	

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_ADD_CIS: {
		rtk_bt_le_iso_cig_cis_info_t *param = (rtk_bt_le_iso_cig_cis_info_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_add_cis(param->cig_id,param->cis_id);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_SET_CIG_PARAM: {
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_set_cig_param((rtk_bt_le_iso_cig_initiator_set_cig_param_t *)p_ipc_msg->param_buf);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_SET_CIS_PARAM: {
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_set_cis_param((rtk_bt_le_iso_cig_initiator_set_cis_param_t *)p_ipc_msg->param_buf);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_START_SETTING: {
		uint8_t cig_id = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_start_setting(cig_id);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_SET_CIS_ACL_LINK: {
		rtk_bt_le_iso_cig_initiator_set_cis_acl_link_param_t *param = (rtk_bt_le_iso_cig_initiator_set_cis_acl_link_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_set_cis_acl_link(param->cis_id, param->conn_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_ID: {
		uint8_t cig_id = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_create_cis_by_cig_id(cig_id);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_CREATE_CIS_BY_CIG_CONN_HANDLE: {
		uint16_t cis_conn_handle = 0;
		rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle_param_t *param = (rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_create_cis_by_cis_conn_handle(param->cig_id, param->cis_count, &cis_conn_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&cis_conn_handle, sizeof(uint16_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_REMOVE_CIG: {
		uint8_t cig_id = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_remove_cig(cig_id);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_ACCEPT_CIS: {
		uint16_t cis_conn_handle = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_acceptor_accept_cis(cis_conn_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_REJECT_CIS: {
		rtk_bt_le_iso_cig_acceptor_reject_cis_param_t * param = (rtk_bt_le_iso_cig_acceptor_reject_cis_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_acceptor_reject_cis(param->cis_conn_handle, param->reason);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_CONFIG_SDU_PARAM: {
		rtk_bt_le_iso_cig_acceptor_config_sdu_param_t * param = (rtk_bt_le_iso_cig_acceptor_config_sdu_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_acceptor_config_sdu_param( param->cis_conn_handle, 
																   param->acceptor_config_sdu_flag,
																   param->sdu_interval_m_s,
																   param->sdu_interval_s_m,
																   param->max_sdu_m_s,
																   param->max_sdu_s_m);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_CONFIG_CIS_REQ_ACTION: {
		rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t action = *(rtk_bt_le_iso_cig_acceptor_config_cis_req_action_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_acceptor_config_cis_req_action(action);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_ACCEPTOR_REGISTER_CALLBACK: {
		ret = (uint16_t)rtk_bt_le_iso_cig_acceptor_register_callback();
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_REGISTER_CALLBACK: {
		uint8_t cig_id = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_register_callback(cig_id);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;		
	}

	case RTK_BT_LE_ISO_ACT_CIG_GET_CONN_HANDLE: {
		uint16_t conn_handle = 0;
		rtk_bt_le_iso_cig_get_conn_handle_param_t *param = (rtk_bt_le_iso_cig_get_conn_handle_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_get_conn_handle(param->cis_conn_handle, &conn_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&conn_handle, sizeof(uint16_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_GET_CIS_INFO: {
		rtk_bt_le_iso_cig_cis_info_t cis_info = {0};
		rtk_bt_le_iso_cig_get_cis_info_param_t *param = (rtk_bt_le_iso_cig_get_cis_info_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_get_cis_info(param->cis_conn_handle, &cis_info);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&cis_info, sizeof(rtk_bt_le_iso_cig_cis_info_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_GET_ISOCH_INFO: {
		rtk_bt_le_iso_cis_channel_info_t isoch_info = {0};
		rtk_bt_le_iso_cig_get_isoch_info_param_t *param = (rtk_bt_le_iso_cig_get_isoch_info_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_get_isoch_info(param->cis_conn_handle, &isoch_info);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&isoch_info, sizeof(rtk_bt_le_iso_cis_channel_info_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;		
	}	

	case RTK_BT_LE_ISO_ACT_CIG_INITIATOR_GET_CIS_CONN_HANDLE: {
		uint16_t cis_conn_handle = 0;
		rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t *param = (rtk_bt_le_iso_cig_initiator_get_cis_handle_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_cig_initiator_get_cis_conn_handle(param->cis_id, &cis_conn_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&cis_conn_handle, sizeof(uint16_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;		
	}	
#endif

#if (defined(RTK_BLE_ISO_BIS_SUPPORT) && RTK_BLE_ISO_BIS_SUPPORT)
	case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_INIT: {
		rtk_bt_le_iso_big_init_param_t * param = (rtk_bt_le_iso_big_init_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_broadcaster_init(param->big_num,param->bis_num);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE: {
		uint8_t big_handle = 0;
		rtk_bt_le_iso_big_broadcaster_create_param_t * param = (rtk_bt_le_iso_big_broadcaster_create_param_t *)p_ipc_msg->param_buf;
		bt_le_iso_ipc_pop_cmd_param(RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_CREATE, (void *)param);
		ret = (uint16_t)rtk_bt_le_iso_big_broadcaster_create(param->adv_handle,param->p_big_param, &big_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&big_handle, sizeof(uint8_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;	
	}	

	case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_TERMINATE: {
		uint8_t big_handle = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_broadcaster_terminate(big_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_BIG_BROADCASTER_READ_TX_SYNC: {
	    rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t tx_sync_info = {0};
		rtk_bt_le_iso_big_broadcaster_read_tx_sync_t *param = (rtk_bt_le_iso_big_broadcaster_read_tx_sync_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_broadcaster_read_tx_sync(param->bis_conn_handle, &tx_sync_info);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&tx_sync_info, sizeof(rtk_bt_le_iso_big_broadcaster_read_tx_sync_info_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;		
	}	
	
	case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_INIT: {
		rtk_bt_le_iso_big_init_param_t * param = (rtk_bt_le_iso_big_init_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_receiver_init(param->big_num,param->bis_num);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC: {
		uint8_t big_handle = 0;
		rtk_bt_le_iso_big_receiver_create_sync_t * param = (rtk_bt_le_iso_big_receiver_create_sync_t *)p_ipc_msg->param_buf;
		bt_le_iso_ipc_pop_cmd_param(RTK_BT_LE_ISO_ACT_BIG_RECEIVER_CREATE_SYNC, (void *)param);
		ret = (uint16_t)rtk_bt_le_iso_big_receiver_create_sync(param->sync_handle,param->p_sync_param, &big_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&big_handle, sizeof(uint8_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_TERMINATE_SYNC: {
		uint8_t big_handle = *(uint8_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_receiver_terminate_sync(big_handle);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}	

	case RTK_BT_LE_ISO_ACT_BIG_RECEIVER_READ_LINK_QUALITY: {
		rtk_bt_le_iso_big_receiver_read_link_quality_info_t link_quality_info = {0};
		rtk_bt_le_iso_big_receiver_read_link_quality_t * param = (rtk_bt_le_iso_big_receiver_read_link_quality_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_receiver_read_iso_link_quality(param->bis_conn_handle,&link_quality_info);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		if (ret == 0) {
			memcpy((void *)&p_ipc_msg->ret[1], (void *)&link_quality_info, sizeof(rtk_bt_le_iso_big_receiver_read_link_quality_info_t));
		}
		p_ipc_msg->ret[0] = ret;
		break;	
	}

	case RTK_BT_LE_ISO_ACT_BIG_SETUP_PATH: {
		rtk_bt_le_iso_setup_path_param_t * param = (rtk_bt_le_iso_setup_path_param_t *)p_ipc_msg->param_buf;
		bt_le_iso_ipc_pop_cmd_param(RTK_BT_LE_ISO_ACT_BIG_SETUP_PATH, (void *)param);
		ret = (uint16_t)rtk_bt_le_iso_big_setup_path(param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}		

	case RTK_BT_LE_ISO_ACT_BIG_REMOVE_PATH: {
		rtk_bt_le_iso_remove_path_param_t * param = (rtk_bt_le_iso_remove_path_param_t *)p_ipc_msg->param_buf;
		ret = (uint16_t)rtk_bt_le_iso_big_remove_path(param->iso_conn_handle, param->data_path_direction);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}	
#endif

	case RTK_BT_LE_ISO_ACT_ISO_DATA_SEND: {
		rtk_bt_le_iso_data_send_info_t * param = (rtk_bt_le_iso_data_send_info_t *)p_ipc_msg->param_buf;
		bt_le_iso_ipc_pop_cmd_param(RTK_BT_LE_ISO_ACT_ISO_DATA_SEND, (void *)param);
		ret = (uint16_t)rtk_bt_le_iso_data_send(param);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));
		p_ipc_msg->ret[0] = ret;
		break;	
	}	

	default :
		printf("%s, Unknown LE ISO ACT: %d \r\n", __func__, (int)p_ipc_msg->API_ID);
		break;
	}

	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}
#endif