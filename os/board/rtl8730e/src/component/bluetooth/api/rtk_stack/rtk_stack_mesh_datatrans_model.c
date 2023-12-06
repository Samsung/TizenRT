#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_common.h>
#include <rtk_bt_def.h>
#include <rtk_bt_mesh_datatrans_model.h>
#include <rtk_bt_mesh_def.h>
#include <datatrans_model.h>

#if defined(BT_MESH_ENABLE_DATATRANS_MODEL) && BT_MESH_ENABLE_DATATRANS_MODEL
mesh_model_info_t datatrans;
static uint8_t sample_data[DATA_TRANS_DATA_MAX_LEN];

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_datatrans_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	if (!p_cmd) {
		printf("[%s] param is NULL!\r\n", __func__);
		return RTK_BT_FAIL;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_DATATRANS_MODEL_ACT_WRITE: {
		rtk_bt_mesh_datatrans_client_write_api_t *data_write = (rtk_bt_mesh_datatrans_client_write_api_t *)p_cmd->param;
		ret = datatrans_write(&datatrans, data_write->dst, data_write->app_key_index, data_write->data_len, data_write->data, data_write->ack);
	}
	break;
	case RTK_BT_MESH_DATATRANS_MODEL_ACT_READ: {
		rtk_bt_mesh_datatrans_client_read_api_t *data_read = (rtk_bt_mesh_datatrans_client_read_api_t *)p_cmd->param;
		ret = datatrans_read(&datatrans, data_read->dst, data_read->app_key_index, data_read->read_len);
	}
	break;
	default:
		printf("[%s] Unknown p_cmd->act:%d\r\n", __func__, p_cmd->act);
		break;
	}
	ret = ret | RTK_BT_STACK_MESH_ERROR_FLAG;
end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}

static int32_t bt_mesh_datatrans_model_data_callback(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	(void)pmodel_info;
	switch (type) {
	case DATATRANS_SERVER_WRITE: {
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_datatrans_server_write_event_t *p_data_write;
		datatrans_server_write_t *pdata = pargs;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_DATATRANS_MODEL, RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_WRITE,
									sizeof(rtk_bt_mesh_datatrans_server_write_event_t));
		p_data_write = (rtk_bt_mesh_datatrans_server_write_event_t *)p_evt->data;
		p_data_write->status = pdata->status;
		if (pdata->data_len > DATA_TRANS_DATA_MAX_LEN) {
			printf("[%s] The len of datatrans receive data is %d, extend max DATA_TRANS_DATA_MAX_LEN:%d\r\n", __func__, pdata->data_len, DATA_TRANS_DATA_MAX_LEN);
			p_data_write->data_len = DATA_TRANS_DATA_MAX_LEN;
		} else {
			p_data_write->data_len = pdata->data_len;
		}
		memcpy(p_data_write->data, pdata->data, p_data_write->data_len);

		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case DATATRANS_SERVER_READ: {
		datatrans_server_read_t *pdata = pargs;
		uint8_t cb_ret = 0;
		if (pdata->data_len > DATA_TRANS_DATA_MAX_LEN) {
			pdata->data_len = DATA_TRANS_DATA_MAX_LEN;
		}

		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_datatrans_server_read_event_t *server_read;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_DATATRANS_MODEL, RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ,
									sizeof(rtk_bt_mesh_datatrans_server_read_event_t) + pdata->data_len);
		server_read = (rtk_bt_mesh_datatrans_server_read_event_t *)p_evt->data;
		server_read->data_len = pdata->data_len;
		server_read->data = sample_data;
		rtk_bt_evt_indicate(p_evt, &cb_ret);
		/* here we shall not use update_ind->data to get data, because update_ind is freed
		in rtk_bt_evt_indicate */

		pdata->data = sample_data;
		break;
	}
	case DATATRANS_CLIENT_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_DATATRANS_MODEL, RTK_BT_MESH_DATATRANS_MODEL_EVT_CLIENT_WRITE_STATUS,
									sizeof(rtk_bt_mesh_datatrans_client_write_event_t));
		memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_datatrans_client_write_event_t));

		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case DATATRANS_CLIENT_DATA: {
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_datatrans_client_read_event_t *p_data_read;
		datatrans_client_data_t *pdata = pargs;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_DATATRANS_MODEL, RTK_BT_MESH_DATATRANS_MODEL_EVT_CLIENT_READ_RESULT,
									sizeof(rtk_bt_mesh_datatrans_client_read_event_t));
		p_data_read = (rtk_bt_mesh_datatrans_client_read_event_t *)p_evt->data;
		p_data_read->data_len = pdata->data_len;
		memcpy(p_data_read->data, pdata->data, p_data_read->data_len);

		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return 0;
}

bool datatrans_model_init(void)
{
	/* register data transmission model */
	datatrans.model_data_cb = bt_mesh_datatrans_model_data_callback;
	return datatrans_reg(0, &datatrans);
}

#endif  // BT_MESH_ENABLE_DATATRANS_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT
