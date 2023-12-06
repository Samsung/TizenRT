#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_generic_onoff_model.h>
#include <rtk_bt_mesh_def.h>

#include <generic_on_off.h>

#if defined(BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL
static mesh_model_info_t generic_on_off_client_model;

static int32_t generic_on_off_client_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case GENERIC_ON_OFF_CLIENT_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ONOFF_CLIENT_MODEL, RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_EVT_STATUS,
									sizeof(generic_on_off_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(generic_on_off_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return 0;
}

bool generic_on_off_client_model_init(void)
{
	generic_on_off_client_model.model_data_cb = generic_on_off_client_data;
	return generic_on_off_client_reg(0, &generic_on_off_client_model);
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_generic_onoff_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_SET: {
		rtk_bt_mesh_generic_onoff_client_act_set_t *onoff_set = (rtk_bt_mesh_generic_onoff_client_act_set_t *)p_cmd->param;
		generic_transition_time_t trans_time;
		memcpy(&trans_time, &(onoff_set->trans_time), sizeof(generic_transition_time_t));
		ret = generic_on_off_set(&generic_on_off_client_model, onoff_set->dst, onoff_set->app_key_index, onoff_set->on_off, onoff_set->tid,
								 onoff_set->optional, trans_time, onoff_set->delay, onoff_set->ack);
		break;
	}
	case RTK_BT_MESH_GENERIC_ONOFF_CLIENT_MODEL_ACT_GET: {
		rtk_bt_mesh_generic_onoff_client_act_get_t *onoff_get = (rtk_bt_mesh_generic_onoff_client_act_get_t *)p_cmd->param;
		ret = generic_on_off_get(&generic_on_off_client_model, onoff_get->dst, onoff_get->app_key_index);
		break;
	}
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
#endif // end of BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL
static mesh_model_info_t generic_on_off_server_model;

static int32_t generic_on_off_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	(void)pmodel_info;

	switch (type) {
	case GENERIC_ON_OFF_SERVER_SET: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL, RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_SET,
									sizeof(rtk_bt_mesh_generic_onoff_server_evt_set_t));
		memcpy(p_evt->data, pargs, sizeof(generic_on_off_server_set_t));
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GENERIC_ON_OFF_SERVER_GET: {
		rtk_bt_mesh_generic_onoff_server_evt_get_t *get_onoff;
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL, RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_GET,
									sizeof(rtk_bt_mesh_generic_onoff_server_evt_get_t));
		get_onoff = (rtk_bt_mesh_generic_onoff_server_evt_get_t *)p_evt->data;
		get_onoff->on_off = &(((generic_on_off_server_get_t *)pargs)->on_off);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GENERIC_ON_OFF_SERVER_GET_DEFAULT_TRANSITION_TIME: {
		// Receive a generic on off set message without Transition Time field,so get Transition Time from app
		break;
	}
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool generic_on_off_server_model_init(void)
{
	generic_on_off_server_model.model_data_cb = generic_on_off_server_data;
	return generic_on_off_server_reg(0, &generic_on_off_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT
