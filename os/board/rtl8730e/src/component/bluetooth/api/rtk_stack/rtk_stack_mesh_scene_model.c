#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_scene_model.h>
#include <scene.h>
#if MODEL_ENABLE_DELAY_MSG_RSP
#include "delay_msg_rsp.h"
#endif

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
#if defined(BT_MESH_ENABLE_SCENE_CLIENT_MODEL) && BT_MESH_ENABLE_SCENE_CLIENT_MODEL
static mesh_model_info_t scene_client_model;

static int32_t scene_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case SCENE_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_CLIENT_MODEL, RTK_BT_MESH_SCENE_CLIENT_MODEL_STATUS,
									sizeof(scene_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(scene_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case SCENE_CLIENT_REGISTER_STATUS:
        {
            scene_client_register_status_t *pdata = (scene_client_register_status_t *)pargs;
            uint32_t len = 6 + 2 * pdata->scene_array_len;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_CLIENT_MODEL, RTK_BT_MESH_SCENE_CLIENT_MODEL_REGISTER_STATUS,
									len);
		    memcpy(p_evt->data, &(pdata->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pdata->status), 1);
            memcpy((uint8_t *)(p_evt->data) + 3, &(pdata->current_scene), 2);
            memcpy((uint8_t *)(p_evt->data) + 5, &(pdata->scene_array_len), 1);
            memcpy((uint8_t *)(p_evt->data) + 6, pdata->scene_array, 2 * pdata->scene_array_len);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool scene_client_model_init(void)
{
	scene_client_model.model_data_cb = scene_client_data;
	return scene_client_reg(0, &scene_client_model);
}

uint16_t bt_mesh_scene_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_SCENE_CLIENT_MODEL_STORE: {
		rtk_bt_mesh_scene_store_t *store_value = (rtk_bt_mesh_scene_store_t *)p_cmd->param;
		ret = scene_store(&scene_client_model, store_value->dst, store_value->app_key_index, store_value->scene_number, store_value->ack);
		break;
        }
        case RTK_BT_MESH_SCENE_CLIENT_MODEL_RECALL: {
        rtk_bt_mesh_scene_recall_t *recall_value = (rtk_bt_mesh_scene_recall_t *)p_cmd->param;
		generic_transition_time_t trans_time;
		memcpy(&trans_time, &(recall_value->trans_time), sizeof(generic_transition_time_t));
		ret = scene_recall(&scene_client_model, recall_value->dst, recall_value->app_key_index, recall_value->scene_number, recall_value->tid,
                                                recall_value->optional, trans_time, recall_value->delay, recall_value->ack);
		break;
        }
        case RTK_BT_MESH_SCENE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_scene_get_t *get_value = (rtk_bt_mesh_scene_get_t *)p_cmd->param;
		ret = scene_get(&scene_client_model, get_value->dst, get_value->app_key_index);
		break;
        }
        case RTK_BT_MESH_SCENE_CLIENT_MODEL_REGISTER_GET: {
		rtk_bt_mesh_scene_register_get_t *register_get = (rtk_bt_mesh_scene_register_get_t *)p_cmd->param;
		ret = scene_register_get(&scene_client_model, register_get->dst, register_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_SCENE_CLIENT_MODEL_DELETE: {
		rtk_bt_mesh_scene_delete_t *delete_value = (rtk_bt_mesh_scene_delete_t *)p_cmd->param;
		ret = scene_delete(&scene_client_model, delete_value->dst, delete_value->app_key_index, delete_value->scene_number, delete_value->ack);
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
#endif  // BT_MESH_ENABLE_SCENE_CLIENT_MODEL

#if ((defined(BT_MESH_ENABLE_SCENE_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SERVER_MODEL) || \
    (defined(BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL))
static mesh_model_info_t scene_server_model;
static uint16_t sample_datas[SCENE_DATA_MAX_LEN];

uint8_t store_value[16] = {0x1,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};
uint8_t store_value2[16] = {0x3,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};
static scene_storage_memory_t scenes[] = 
{
    {1,store_value},
    {2,store_value2},
};
#endif

#if defined(BT_MESH_ENABLE_SCENE_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SERVER_MODEL
static int32_t scene_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    /* avoid gcc compile warning */
    UNUSED(pmodel_info);

    switch (type)
    {
        case SCENE_SERVER_GET:
            {
                rtk_bt_mesh_scene_server_get_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL, RTK_BT_MESH_SCENE_SERVER_MODEL_GET, 
                                            sizeof(rtk_bt_mesh_scene_server_get_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_t *)p_evt->data;
                p_get_data->current_scene = &(((scene_server_get_t *)pargs)->current_scene);
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        case SCENE_SERVER_RECALL:
            {
                scene_server_recall_t *p_get_data = NULL;
				p_get_data = (scene_server_recall_t *)pargs;
				if (p_get_data) {
                    rtk_bt_evt_t *p_evt = NULL;
                    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL, RTK_BT_MESH_SCENE_SERVER_MODEL_RECALL, SCENE_DATA_MAX_LEN + 6);
                    memcpy((uint8_t *)p_evt->data, p_get_data, 2);
                    memcpy((uint8_t *)(p_evt->data) + 2,&p_get_data->total_time, 1);
                    memcpy((uint8_t *)(p_evt->data) + 3,&p_get_data->remaining_time, 1);
                    memcpy((uint8_t *)(p_evt->data) + 4, p_get_data->pmemory, SCENE_DATA_MAX_LEN);
                    rtk_bt_evt_indicate(p_evt, NULL);
				}
            }
            break;
        case SCENE_SERVER_GET_DEFAULT_TRANSITION_TIME:
            {
            }
            break;
        case SCENE_SERVER_GET_REGISTER_STATUS:
            {
                rtk_bt_mesh_scene_server_get_register_status_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL, RTK_BT_MESH_SCENE_SERVER_MODEL_GET_REGISTER_STATUS, 
                                            sizeof(rtk_bt_mesh_scene_server_get_register_status_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_register_status_t *)p_evt->data;
                p_get_data->status = &(((scene_server_get_register_status_t *)pargs)->status);
                rtk_bt_evt_indicate(p_evt, NULL);
            }
            break;
        case SCENE_SERVER_GET_SCENES_NUM:
            {
                rtk_bt_mesh_scene_server_get_scenes_num_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL, RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES_NUM, 
                                            sizeof(rtk_bt_mesh_scene_server_get_scenes_num_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_scenes_num_t *)p_evt->data;
                p_get_data->num_scenes = &(((scene_server_get_scenes_num_t *)pargs)->num_scenes);
                rtk_bt_evt_indicate(p_evt, NULL);
            }    
            break;
        case SCENE_SERVER_GET_SCENES:
            {
                scene_server_get_scenes_t *p_data = pargs;
                rtk_bt_mesh_scene_server_get_scenes_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL, RTK_BT_MESH_SCENE_SERVER_MODEL_GET_SCENES, 
                                            sizeof(rtk_bt_mesh_scene_server_get_scenes_t) + p_data->num_scenes * sizeof(uint16_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_scenes_t *)p_evt->data;
                p_get_data->num_scenes = p_data->num_scenes;
                p_get_data->scenes = sample_datas;
                rtk_bt_evt_indicate(p_evt, NULL);
                p_data->scenes = sample_datas;
            }    
            break;
        case SCENE_SERVER_GET_STATUS_RECALL:
            {
                scene_server_get_status_recall_t *p_data = (scene_server_get_status_recall_t *)pargs;
                rtk_bt_mesh_scene_server_get_status_recall_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL, RTK_BT_MESH_SCENE_SERVER_MODEL_GET_STATUS_RECALL, 
                                            sizeof(rtk_bt_mesh_scene_server_get_status_recall_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_status_recall_t *)p_evt->data;
                p_get_data->scene_number = p_data->scene_number;
                p_get_data->status_recall = &(((scene_server_get_status_recall_t *)pargs)->status_recall);
                rtk_bt_evt_indicate(p_evt, NULL);
            }    
            break;
        default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool scene_server_model_init(void){
    bool ret = 0;
    scene_server_model.model_data_cb = scene_server_data;
    ret = scene_server_reg(0,&scene_server_model);
    if (ret)
    {
        scene_server_set_storage_memory(&scene_server_model,scenes,sizeof(scenes)/sizeof(scene_storage_memory_t));
        return ret;
    }
    return ret;
}

uint16_t bt_mesh_scene_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_SCENE_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_scene_server_db_set_t *db_get = (rtk_bt_mesh_scene_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using scene_server_set_storage_memory *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        scene_server_set_storage_memory(&scene_server_model,scenes,sizeof(scenes)/sizeof(scene_storage_memory_t));
		ret = 0;
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
#endif // end of BT_MESH_ENABLE_SCENE_SERVER_MODEL

#if defined(BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
static mesh_model_info_t scene_setup_server_model;

uint16_t deleted_scene = 0;

scene_server_store_t scene_store_buffer = {0};

static uint8_t scene_store_data[SCENE_DATA_MAX_LEN];
static int32_t scene_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    /* avoid gcc compile warning */
    UNUSED(pmodel_info);

    switch (type)
    {
        case SCENE_SERVER_STORE:
        {
            scene_server_store_t *p_get_data = NULL;
            p_get_data = (scene_server_store_t *)pargs;
			if (p_get_data) {
                rtk_bt_mesh_scene_server_store_t *scene_store;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL, RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_STORE, 
                                            SCENE_DATA_MAX_LEN + 3);
                scene_store = (rtk_bt_mesh_scene_server_store_t*)p_evt->data;
                scene_store->status = p_get_data->status;
                scene_store->scene_number = p_get_data->scene_number;
                scene_store->pmemory = scene_store_data;
                rtk_bt_evt_indicate(p_evt, NULL);
                p_get_data->pmemory = scene_store_data; //reserve 2 bytes to store length
			}
        }
        break;
        case SCENE_SERVER_DELETE:
        {
            scene_server_delete_t *p_get_data = pargs;
			p_get_data = (scene_server_delete_t *)pargs;
			if (p_get_data) {
                deleted_scene = p_get_data->scene_number;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL, RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_DELETE, sizeof(scene_server_delete_t));
                memcpy(p_evt->data, p_get_data, sizeof(scene_server_delete_t));
                rtk_bt_evt_indicate(p_evt, NULL);
			}
        }
        break;
        case SCENE_SERVER_GET:
            {
                rtk_bt_mesh_scene_server_get_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL, RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET, 
                                            sizeof(rtk_bt_mesh_scene_server_get_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_t *)p_evt->data;
                p_get_data->current_scene = &(((scene_server_get_t *)pargs)->current_scene);
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        break;
        case SCENE_SERVER_GET_SCENES_NUM:
            {
                rtk_bt_mesh_scene_server_get_scenes_num_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL, RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES_NUM, 
                                            sizeof(rtk_bt_mesh_scene_server_get_scenes_num_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_scenes_num_t *)p_evt->data;
                p_get_data->num_scenes = &(((scene_server_get_scenes_num_t *)pargs)->num_scenes);
                rtk_bt_evt_indicate(p_evt, NULL);
            }    
        break;
        case SCENE_SERVER_GET_SCENES:
            {
                scene_server_get_scenes_t *p_data = pargs;
                rtk_bt_mesh_scene_server_get_scenes_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL, RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_SCENES, 
                                            sizeof(rtk_bt_mesh_scene_server_get_scenes_t) + p_data->num_scenes * sizeof(uint16_t));
                p_get_data = (rtk_bt_mesh_scene_server_get_scenes_t *)p_evt->data;
                p_get_data->num_scenes = p_data->num_scenes;
                p_get_data->scenes = sample_datas;
                rtk_bt_evt_indicate(p_evt, NULL);
                p_data->scenes = sample_datas;
            }    
        break;
        case SCENE_SERVER_GET_REGISTER_STATUS:
            {
                scene_setup_server_get_register_status_t *p_data = pargs;
                rtk_bt_mesh_scene_setup_server_get_register_status_t *p_get_data;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL, RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_GET_STATUS_REGISTER, 
                                            sizeof(rtk_bt_mesh_scene_server_get_register_status_t));
                p_get_data = (rtk_bt_mesh_scene_setup_server_get_register_status_t *)p_evt->data;
                p_get_data->scene_number = p_data->scene_number;
                p_get_data->status = &(((scene_setup_server_get_register_status_t *)pargs)->status);
                rtk_bt_evt_indicate(p_evt, NULL);
            }    
        break;
        default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool scene_setup_server_model_init(void){
    bool ret = 0;
    scene_setup_server_model.model_data_cb = scene_setup_server_data;
    ret = scene_setup_server_reg(0,&scene_setup_server_model);
    if (ret)
    {
        scene_setup_server_set_storage_memory(&scene_setup_server_model,scenes,sizeof(scenes)/sizeof(scene_storage_memory_t));
        return ret;
    }
    return ret;
}

uint16_t bt_mesh_scene_setup_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_SCENE_SETUP_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_scene_server_db_set_t *db_get = (rtk_bt_mesh_scene_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using scene_setup_server_set_storage_memory *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        scene_setup_server_set_storage_memory(&scene_setup_server_model,scenes,sizeof(scenes)/sizeof(scene_storage_memory_t));
		ret = 0;
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
#endif // end of BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT
