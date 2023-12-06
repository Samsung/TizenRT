#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_scheduler_model.h>
#include <scheduler.h>

#if defined(BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL) && BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL
static mesh_model_info_t scheduler_client_model;

static int32_t scheduler_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case SCHEDULER_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCHEDULER_CLIENT_MODEL, RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_STATUS,
									sizeof(scheduler_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(scheduler_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case SCHEDULER_CLIENT_STATUS_ACTION:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCHEDULER_CLIENT_MODEL, RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_STATUS,
									sizeof(scheduler_client_status_action_t));
		    memcpy(p_evt->data, pargs, sizeof(scheduler_client_status_action_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    
    return 0;
}

bool scheduler_client_model_init(void)
{
	scheduler_client_model.model_data_cb = scheduler_client_data;
	return scheduler_client_reg(0, &scheduler_client_model);
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_scheduler_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
	case RTK_BT_MESH_SCHEDULER_CLIENT_MODEL_GET: {
		rtk_bt_mesh_scheduler_get_t *scheduler = (rtk_bt_mesh_scheduler_get_t *)p_cmd->param;
		ret = scheduler_get(&scheduler_client_model, scheduler->dst, scheduler->app_key_index);
		break;
	}
    case RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_GET: {
		rtk_bt_mesh_scheduler_action_get_t *scheduler_action = (rtk_bt_mesh_scheduler_action_get_t *)p_cmd->param;
		ret = scheduler_action_get(&scheduler_client_model, scheduler_action->dst, scheduler_action->app_key_index, 
                            scheduler_action->index);
		break;
	}
    case RTK_BT_MESH_SCHEDULER_ACTION_CLIENT_MODEL_SET: {
		rtk_bt_mesh_scheduler_action_set_t *action_set = (rtk_bt_mesh_scheduler_action_set_t *)p_cmd->param;
        scheduler_register_t scheduler;
        memcpy(&scheduler, &(action_set->scheduler), sizeof(scheduler_register_t));
		ret = scheduler_action_set(&scheduler_client_model, action_set->dst, action_set->app_key_index, 
                            scheduler, action_set->ack);
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


#endif  // BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL) && BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL
mesh_model_info_t scheduler_server_model;

static int32_t scheduler_server_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
        case SCHEDULER_SERVER_GET:
            {
            	rtk_bt_mesh_scheduler_server_get_t *p_get_data;
				uint8_t cb_ret = 0;
				rtk_bt_evt_t *p_evt = NULL;
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCHEDULER_SERVER_MODEL, RTK_BT_MESH_SCHEDULER_SERVER_MODEL_GET, 
											sizeof(rtk_bt_mesh_scheduler_server_get_t));
				p_get_data = (rtk_bt_mesh_scheduler_server_get_t *)p_evt->data;
				p_get_data->schedulers = &(((scheduler_server_get_t *)pargs)->schedulers);
				rtk_bt_evt_indicate(p_evt, &cb_ret);
            }
            break;
        case SCHEDULER_SERVER_GET_ACTION:
            {
			    rtk_bt_mesh_scheduler_server_get_action_t *p_get_data;
				uint8_t cb_ret = 0;
				rtk_bt_evt_t *p_evt = NULL;
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCHEDULER_SERVER_MODEL, RTK_BT_MESH_SCHEDULER_ACTION_SERVER_MODEL_GET, 
											sizeof(rtk_bt_mesh_scheduler_server_get_action_t));
				p_get_data = (rtk_bt_mesh_scheduler_server_get_action_t *)p_evt->data;
				p_get_data->index = ((scheduler_server_get_action_t *)pargs)->index;
				p_get_data->scheduler =(rtk_bt_mesh_scheduler_register_t *) &(((scheduler_server_get_action_t *)pargs)->scheduler);
				rtk_bt_evt_indicate(p_evt, &cb_ret);
            }
            break;
        default:
            printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
            break;
    }
    
    return 0;
}

bool scheduler_server_model_init(void)
{
    scheduler_server_model.model_data_cb = scheduler_server_data;
	return scheduler_server_reg(0, &scheduler_server_model);
}
#endif // end of BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL

#if defined(BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL
mesh_model_info_t scheduler_setup_server_model;

scheduler_server_set_action_t scheduler_action_set_buffer = {0};

static int32_t scheduler_setup_server_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
        case SCHEDULER_SERVER_SET_ACTION:
            {
                scheduler_server_set_action_t *p_get_data = pargs;
            	p_get_data = (scheduler_server_set_action_t *)pargs;

				if (p_get_data) {
                    rtk_bt_evt_t *p_evt = NULL;
                    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCHEDULER_SETUP_SERVER_MODEL, RTK_BT_MESH_SCHEDULER_ACTION_SERVER_MODEL_SET, sizeof(scheduler_server_set_action_t));
					memcpy(p_evt->data, p_get_data, sizeof(scheduler_server_set_action_t));
                    rtk_bt_evt_indicate(p_evt, NULL);
				}
            }
            break;
        case SCHEDULER_SERVER_GET_ACTION:
            {
                rtk_bt_mesh_scheduler_server_get_action_t *p_get_data;
				rtk_bt_evt_t *p_evt = NULL;
				p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCHEDULER_SETUP_SERVER_MODEL, RTK_BT_MESH_SCHEDULER_SETUP_SERVER_MODEL_GET_ACTION, 
											sizeof(rtk_bt_mesh_scheduler_server_get_action_t));
				p_get_data = (rtk_bt_mesh_scheduler_server_get_action_t *)p_evt->data;
				p_get_data->index = ((scheduler_server_get_action_t *)pargs)->index;
				p_get_data->scheduler =(rtk_bt_mesh_scheduler_register_t *) &(((scheduler_server_get_action_t *)pargs)->scheduler);
				rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        default:
            printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
            break;
    }
    
    return 0;
}

bool scheduler_setup_server_model_init(void)
{
    scheduler_setup_server_model.model_data_cb = scheduler_setup_server_data;
	return scheduler_setup_server_reg(0, &scheduler_setup_server_model);
}

#endif //end of BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT