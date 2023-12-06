#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_time_model.h>
#include <time_model.h>

#if defined(BT_MESH_ENABLE_TIME_CLIENT_MODEL) && BT_MESH_ENABLE_TIME_CLIENT_MODEL
static mesh_model_info_t time_client_model;

/*****************time client model************************/
static int32_t time_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case TIME_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_CLIENT_MODEL, RTK_BT_MESH_TIME_CLIENT_MODEL_STATUS,
									sizeof(time_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(time_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case TIME_CLIENT_STATUS_ROLE:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_CLIENT_MODEL, RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_STATUS,
									sizeof(time_client_status_role_t));
		    memcpy(p_evt->data, pargs, sizeof(time_client_status_role_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case TIME_CLIENT_STATUS_ZONE:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_CLIENT_MODEL, RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_STATUS,
									sizeof(time_client_status_zone_t));
		    memcpy(p_evt->data, pargs, sizeof(time_client_status_zone_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case TIME_CLIENT_STATUS_TAI_UTC_DELTA:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_CLIENT_MODEL, RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_STATUS,
									sizeof(time_client_status_tai_utc_delta_t));
		    memcpy(p_evt->data, pargs, sizeof(time_client_status_tai_utc_delta_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool time_client_model_init(void)
{
	time_client_model.model_data_cb = time_client_data;
	return time_client_reg(0, &time_client_model);
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_time_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
        case RTK_BT_MESH_TIME_CLIENT_MODEL_GET: {
		rtk_bt_mesh_time_get_t *time_client_get = (rtk_bt_mesh_time_get_t *)p_cmd->param;
		ret = time_get(&time_client_model, time_client_get->dst, time_client_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_GET: {
		rtk_bt_mesh_time_zone_get_t *time_client_zone_get = (rtk_bt_mesh_time_zone_get_t *)p_cmd->param;
		ret = time_get_zone(&time_client_model, time_client_zone_get->dst, time_client_zone_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_GET: {
		rtk_bt_mesh_time_tai_utc_delta_get_t *time_client_delta_get = (rtk_bt_mesh_time_tai_utc_delta_get_t *)p_cmd->param;
		ret = time_get_tai_utc_delta(&time_client_model, time_client_delta_get->dst, time_client_delta_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_GET: {
		rtk_bt_mesh_time_role_get_t *time_client_role_get = (rtk_bt_mesh_time_role_get_t *)p_cmd->param;
		ret = time_get_role(&time_client_model, time_client_role_get->dst, time_client_role_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_SET: {
		rtk_bt_mesh_time_set_t *time_client_set = (rtk_bt_mesh_time_set_t *)p_cmd->param;
        tai_time_t time;
        memcpy(&time,&(time_client_set->time),sizeof(tai_time_t));
		ret = time_set(&time_client_model, time_client_set->dst, time_client_set->app_key_index, time);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_ZONE_SET: {
		rtk_bt_mesh_time_zone_set_t *time_client_zone_set = (rtk_bt_mesh_time_zone_set_t *)p_cmd->param;
		ret = time_set_zone(&time_client_model, time_client_zone_set->dst, time_client_zone_set->app_key_index,
                            time_client_zone_set->time_zone_offset_new, time_client_zone_set->tai_of_zone_change);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_TAI_UTC_DELTA_SET: {
		rtk_bt_mesh_time_tai_utc_delta_set_t *time_client_delta_set = (rtk_bt_mesh_time_tai_utc_delta_set_t *)p_cmd->param;
		ret = time_set_tai_utc_delta(&time_client_model, time_client_delta_set->dst, time_client_delta_set->app_key_index,
                                    time_client_delta_set->tai_utc_delta_new, time_client_delta_set->tai_of_delta_change);
		break;
        }
        case RTK_BT_MESH_TIME_CLIENT_MODEL_ROLE_SET: {
		rtk_bt_mesh_time_role_set_t *time_client_role_set = (rtk_bt_mesh_time_role_set_t *)p_cmd->param;
        time_role_t role;
        memcpy(&role,&(time_client_role_set->role),sizeof(time_role_t));
		ret = time_set_role(&time_client_model, time_client_role_set->dst, time_client_role_set->app_key_index, role);
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


#endif  // BT_MESH_ENABLE_TIME_CLIENT_MODEL


#if defined(BT_MESH_ENABLE_TIME_SERVER_MODEL) && BT_MESH_ENABLE_TIME_SERVER_MODEL
static mesh_model_info_t time_server_model;

static int32_t time_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    /* avoid gcc compile warning */
    UNUSED(pmodel_info);

    switch (type)
    {
    case TIME_SERVER_GET:
        {
            rtk_bt_mesh_time_server_get_t *p_get_data = NULL;
            p_get_data = (rtk_bt_mesh_time_server_get_t *)pargs;
            uint8_t cb_ret = 0;
            if (p_get_data) {
                rtk_bt_mesh_time_server_direct_get_t *time_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL, RTK_BT_MESH_TIME_SERVER_MODEL_GET, 
                                            sizeof(rtk_bt_mesh_time_server_direct_get_t));
                time_get = (rtk_bt_mesh_time_server_direct_get_t *)p_evt->data;
                time_get->value = p_get_data;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
            }
        }
        break;
    case TIME_SERVER_GET_ZONE:
        {
            rtk_bt_mesh_time_server_get_zone_t *p_get_data = NULL;
            p_get_data = (rtk_bt_mesh_time_server_get_zone_t *)pargs;
            uint8_t cb_ret = 0;
            if (p_get_data) {
                rtk_bt_mesh_time_server_direct_get_zone_t *zone_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL, RTK_BT_MESH_TIME_SERVER_MODEL_ZONE_GET, 
                                            sizeof(rtk_bt_mesh_time_server_direct_get_zone_t));
                zone_get = (rtk_bt_mesh_time_server_direct_get_zone_t *)p_evt->data;
                zone_get->value = p_get_data;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
            }
        }
        break;
    case TIME_SERVER_GET_TAI_UTC_DELTA:
        {
            rtk_bt_mesh_time_server_get_tai_utc_delta_t *p_get_data = NULL;
		    p_get_data = (rtk_bt_mesh_time_server_get_tai_utc_delta_t *)pargs;
            if (p_get_data) {
                rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *delta_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL, RTK_BT_MESH_TIME_SERVER_MODEL_TAI_UTC_DELTA_GET, 
                                            sizeof(rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t));
                delta_get = (rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *)p_evt->data;
                delta_get->value = p_get_data;
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case TIME_SERVER_STATUS_SET:
        {
            time_server_status_set_t *p_set_data = NULL;
            p_set_data = (time_server_status_set_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL, RTK_BT_MESH_TIME_SERVER_MODEL_STATUS_SET, sizeof(time_server_get_tai_utc_delta_t));
                memcpy(p_evt->data, p_set_data, sizeof(time_server_status_set_t));
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

bool time_server_model_init(void){
    time_server_model.model_data_cb = time_server_data;
    return time_server_reg(0,&time_server_model);
}
#endif // end of BT_MESH_ENABLE_TIME_SERVER_MODEL

#if defined(BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL
static mesh_model_info_t time_setup_server_model;

static int32_t time_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    /* avoid gcc compile warning */
    UNUSED(pmodel_info);
    switch (type)
    {
    case TIME_SERVER_SET:
        {
            time_server_set_t *p_set_data = NULL;
            p_set_data = (time_server_set_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_SET, sizeof(time_server_set_t));
                memcpy(p_evt->data, p_set_data, sizeof(time_server_set_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case TIME_SERVER_SET_ZONE:
        {
            time_server_set_zone_t *p_set_data = NULL;
            p_set_data = (time_server_set_zone_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ZONE_SET, sizeof(time_server_set_zone_t));
                memcpy(p_evt->data, p_set_data, sizeof(time_server_set_zone_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case TIME_SERVER_GET_ROLE:
        {
            rtk_bt_mesh_time_server_get_role_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ROLE_GET, 
                                        sizeof(rtk_bt_mesh_time_server_get_role_t));
            p_get_data = (rtk_bt_mesh_time_server_get_role_t *)p_evt->data;
            p_get_data->role = &(((time_server_get_role_t *)pargs)->role);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case TIME_SERVER_SET_ROLE:
        {
            time_server_set_role_t *p_set_data = NULL;
		    p_set_data = (time_server_set_role_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ROLE_SET, sizeof(time_server_set_role_t));
                memcpy(p_evt->data, p_set_data, sizeof(time_server_set_role_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case TIME_SERVER_SET_TAI_UTC_DELTA:
        {
            time_server_set_tai_utc_delta_t *p_set_data = NULL;
		    p_set_data = (time_server_set_tai_utc_delta_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_TAI_UTC_DELTA_SET, sizeof(time_server_set_tai_utc_delta_t));
                memcpy(p_evt->data, p_set_data, sizeof(time_server_set_tai_utc_delta_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case TIME_SERVER_GET_ZONE:
        {
            rtk_bt_mesh_time_server_get_zone_t *p_get_data = NULL;
            p_get_data = (rtk_bt_mesh_time_server_get_zone_t *)pargs;
            uint8_t cb_ret = 0;
            if (p_get_data) {
                rtk_bt_mesh_time_server_direct_get_zone_t *zone_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_ZONE_GET, 
                                            sizeof(rtk_bt_mesh_time_server_direct_get_zone_t));
                zone_get = (rtk_bt_mesh_time_server_direct_get_zone_t *)p_evt->data;
                zone_get->value = p_get_data;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
            }
        }
        break;
    case TIME_SERVER_GET_TAI_UTC_DELTA:
        {
            rtk_bt_mesh_time_server_get_tai_utc_delta_t *p_get_data = NULL;
		    p_get_data = (rtk_bt_mesh_time_server_get_tai_utc_delta_t *)pargs;
            if (p_get_data) {
                rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *delta_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL, RTK_BT_MESH_TIME_SETUP_SERVER_MODEL_TAI_UTC_DELTA_GET, 
                                            sizeof(rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t));
                delta_get = (rtk_bt_mesh_time_server_direct_get_tai_utc_delta_t *)p_evt->data;
                delta_get->value = p_get_data;
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

bool time_setup_server_model_init(void){
    time_setup_server_model.model_data_cb = time_setup_server_data;
    return time_setup_server_reg(0,&time_setup_server_model);
}


#endif // end of BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT