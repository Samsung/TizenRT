#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_default_transition_time.h>
#include <generic_default_transition_time.h>

#if defined(BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL
static mesh_model_info_t generic_default_transition_time_client_model;

static int32_t generic_default_transition_time_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL, RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_STATUS,
									sizeof(generic_default_transition_time_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_default_transition_time_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_default_transition_time_client_model_init(void)
{
	generic_default_transition_time_client_model.model_data_cb = generic_default_transition_time_client_data;
	return generic_default_transition_time_client_reg(0, &generic_default_transition_time_client_model);
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_generic_default_transition_time_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_GET: {
		rtk_bt_mesh_generic_default_transition_time_get_t *time_client_get = (rtk_bt_mesh_generic_default_transition_time_get_t *)p_cmd->param;
		ret = generic_default_transition_time_get(&generic_default_transition_time_client_model, time_client_get->dst, time_client_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_SET: {
		rtk_bt_mesh_generic_default_transition_time_set_t *time_client_set = (rtk_bt_mesh_generic_default_transition_time_set_t *)p_cmd->param;
		generic_transition_time_t trans_time;
        memcpy(&trans_time,&(time_client_set->trans_time),sizeof(generic_transition_time_t));
        ret = generic_default_transition_time_set(&generic_default_transition_time_client_model, time_client_set->dst, time_client_set->app_key_index,
                                                    trans_time, time_client_set->ack);
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

#endif  // BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL

mesh_model_info_t generic_default_transition_time_server_model;

static int32_t generic_default_transition_time_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
    case GENERIC_DEFAULT_TRANSITION_TIME_SERVER_GET:
        {
            rtk_bt_mesh_generic_default_transition_time_server_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL, RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_GET, 
                                        sizeof(rtk_bt_mesh_generic_default_transition_time_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_default_transition_time_server_get_t *)p_evt->data;
            p_get_data->trans_time =(rtk_bt_mesh_generic_transition_time_t *) &(((generic_default_transition_time_server_get_t *)pargs)->trans_time);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_DEFAULT_TRANSITION_TIME_SERVER_SET:
        {
            generic_default_transition_time_server_set_t *p_get_data = NULL;
            p_get_data = (generic_default_transition_time_server_set_t *) pargs;
            if (p_get_data)
            {
               rtk_bt_evt_t *p_evt = NULL;
               p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL, RTK_BT_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_SET, sizeof(generic_default_transition_time_server_get_t));
               memcpy(p_evt->data, p_get_data, sizeof(generic_default_transition_time_server_set_t));
               rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_default_transition_time_server_model_init(void){
    generic_default_transition_time_server_model.model_data_cb = generic_default_transition_time_server_data;
    return generic_default_transition_time_server_reg(0,&generic_default_transition_time_server_model);
}

#endif // end of BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT