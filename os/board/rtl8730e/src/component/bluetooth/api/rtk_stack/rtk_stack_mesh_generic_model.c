#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_model.h>
#include <generic_level.h>
#include <generic_power_on_off.h>
#include <generic_power_level.h>
#include <generic_battery.h>
#include <generic_location.h>
#include <generic_property.h>

extern bool bt_stack_profile_check(rtk_bt_profile_t profile); 
#if defined(BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL
static mesh_model_info_t generic_level_client_model;

static int32_t generic_level_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_LEVEL_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LEVEL_CLIENT_MODEL, RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_STATUS,
									sizeof(generic_level_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_level_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_level_client_model_init(void)
{
	generic_level_client_model.model_data_cb = generic_level_client_data;
	return generic_level_client_reg(0, &generic_level_client_model);
}

uint16_t bt_mesh_generic_level_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_GET: {
		rtk_bt_mesh_generic_level_get_t *level_get = (rtk_bt_mesh_generic_level_get_t *)p_cmd->param;
		ret = generic_level_get(&generic_level_client_model, level_get->dst, level_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_SET: {
		rtk_bt_mesh_generic_level_set_t *level_set = (rtk_bt_mesh_generic_level_set_t *)p_cmd->param;
		generic_transition_time_t trans_time;
        memcpy(&trans_time,&(level_set->trans_time),sizeof(generic_transition_time_t));
        ret = generic_level_set(&generic_level_client_model, level_set->dst, level_set->app_key_index,
                                level_set->level, level_set->tid, level_set->optional, trans_time, level_set->delay, level_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_DELTA_SET: {
		rtk_bt_mesh_generic_level_delta_set_t *delta_set = (rtk_bt_mesh_generic_level_delta_set_t *)p_cmd->param;
		generic_transition_time_t trans_time;
        memcpy(&trans_time,&(delta_set->trans_time),sizeof(generic_transition_time_t));
        ret = generic_delta_set(&generic_level_client_model, delta_set->dst, delta_set->app_key_index,
                                                  delta_set->delta_level, delta_set->tid, delta_set->optional,
                                                  trans_time, delta_set->delay, delta_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_LEVEL_CLIENT_MODEL_MOVE_SET: {
		rtk_bt_mesh_generic_level_move_set_t *move_set = (rtk_bt_mesh_generic_level_move_set_t *)p_cmd->param;
		generic_transition_time_t trans_time;
        memcpy(&trans_time,&(move_set->trans_time),sizeof(generic_transition_time_t));
        ret = generic_move_set(&generic_level_client_model, move_set->dst, move_set->app_key_index,
                                                  move_set->delta_level, move_set->tid, move_set->optional,
                                                  trans_time, move_set->delay, move_set->ack);
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
#endif  // BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL
static mesh_model_info_t generic_power_on_off_client_model;

static int32_t generic_power_on_off_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_POWER_ON_OFF_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_CLIENT_MODEL, RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_STATUS,
									sizeof(generic_power_on_off_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_power_on_off_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_power_on_off_client_model_init(void)
{
	generic_power_on_off_client_model.model_data_cb = generic_power_on_off_client_data;
	return generic_power_on_off_client_reg(0, &generic_power_on_off_client_model);
}

uint16_t bt_mesh_generic_power_on_off_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_GET: {
		rtk_bt_mesh_generic_on_power_up_get_t *power_get = (rtk_bt_mesh_generic_on_power_up_get_t *)p_cmd->param;
		ret = generic_on_power_up_get(&generic_power_on_off_client_model, power_get->dst, power_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_ON_POWER_UP_CLIENT_MODEL_SET: {
		rtk_bt_mesh_generic_on_power_up_set_t *power_set = (rtk_bt_mesh_generic_on_power_up_set_t *)p_cmd->param;
		generic_on_power_up_t power_stat;
        memcpy(&power_stat,&(power_set->on_power_up),sizeof(generic_on_power_up_t));
        ret = generic_on_power_up_set(&generic_power_on_off_client_model, power_set->dst, power_set->app_key_index,
                                power_stat, power_set->ack);
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
#endif  // BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL
static mesh_model_info_t generic_power_level_client_model;

static int32_t generic_power_level_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_POWER_LEVEL_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_STATUS,
									sizeof(generic_power_level_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_power_level_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_POWER_LEVEL_CLIENT_STATUS_LAST:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_LAST_STATUS,
									sizeof(generic_power_level_client_status_simple_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_power_level_client_status_simple_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_POWER_LEVEL_CLIENT_STATUS_DEFAULT:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_STATUS,
									sizeof(generic_power_level_client_status_simple_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_power_level_client_status_simple_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_POWER_LEVEL_CLIENT_STATUS_RANGE:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_STATUS,
									sizeof(generic_power_level_client_status_range_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_power_level_client_status_range_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_power_level_client_model_init(void)
{
	generic_power_level_client_model.model_data_cb = generic_power_level_client_data;
	return generic_power_level_client_reg(0, &generic_power_level_client_model);
}

uint16_t bt_mesh_generic_power_level_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_GET: {
		rtk_bt_mesh_generic_power_level_get_t *level_get = (rtk_bt_mesh_generic_power_level_get_t *)p_cmd->param;
		ret = generic_power_level_get(&generic_power_level_client_model, level_get->dst, level_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_SET: {
		rtk_bt_mesh_generic_power_level_set_t *level_set = (rtk_bt_mesh_generic_power_level_set_t *)p_cmd->param;
		generic_transition_time_t trans_time;
        memcpy(&trans_time,&(level_set->trans_time),sizeof(generic_transition_time_t));
        ret = generic_power_level_set(&generic_power_level_client_model, level_set->dst, level_set->app_key_index,
                                level_set->power, level_set->tid, level_set->optional, trans_time, level_set->delay, level_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_LAST_GET: {
		rtk_bt_mesh_generic_power_level_last_get_t *last_get = (rtk_bt_mesh_generic_power_level_last_get_t *)p_cmd->param;
		ret = generic_power_last_get(&generic_power_level_client_model, last_get->dst, last_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_GET: {
		rtk_bt_mesh_generic_power_level_default_get_t *default_get = (rtk_bt_mesh_generic_power_level_default_get_t *)p_cmd->param;
		ret = generic_power_default_get(&generic_power_level_client_model, default_get->dst, default_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_DEFAULT_SET: {
		rtk_bt_mesh_generic_power_level_default_set_t *default_set = (rtk_bt_mesh_generic_power_level_default_set_t *)p_cmd->param;
		ret = generic_power_default_set(&generic_power_level_client_model, default_set->dst, default_set->app_key_index, default_set->power, default_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_GET: {
		rtk_bt_mesh_generic_power_level_range_get_t *range_get = (rtk_bt_mesh_generic_power_level_range_get_t *)p_cmd->param;
		ret = generic_power_range_get(&generic_power_level_client_model, range_get->dst, range_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_POWER_LEVEL_CLIENT_MODEL_RANGE_SET: {
		rtk_bt_mesh_generic_power_level_range_set_t *range_set = (rtk_bt_mesh_generic_power_level_range_set_t *)p_cmd->param;
		ret = generic_power_range_set(&generic_power_level_client_model, range_set->dst, range_set->app_key_index,
                                        range_set->range_min, range_set->range_max, range_set->ack);
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
#endif  // BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL
static mesh_model_info_t generic_battery_client_model;

static int32_t generic_battery_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_BATTERY_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_BATTERY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_STATUS,
									sizeof(generic_battery_client_status_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_battery_client_status_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_battery_client_model_init(void)
{
	generic_battery_client_model.model_data_cb = generic_battery_client_data;
	return generic_battery_client_reg(0, &generic_battery_client_model);
}

uint16_t bt_mesh_generic_battery_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_BATTERY_CLIENT_MODEL_GET: {
		rtk_bt_mesh_generic_battery_get_t *battery_get = (rtk_bt_mesh_generic_battery_get_t *)p_cmd->param;
		ret = generic_battery_get(&generic_battery_client_model, battery_get->dst, battery_get->app_key_index);
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
#endif  // BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL
static mesh_model_info_t generic_location_client_model;

static int32_t generic_location_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_LOCATION_CLIENT_STATUS_GLOBAL:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_CLIENT_MODEL, RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_STATUS,
									sizeof(generic_location_client_status_global_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_location_client_status_global_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_LOCATION_CLIENT_STATUS_LOCAL:
        {
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_CLIENT_MODEL, RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_STATUS,
									sizeof(generic_location_client_status_local_t));
		    memcpy(p_evt->data, pargs, sizeof(generic_location_client_status_local_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_location_client_model_init(void)
{
	generic_location_client_model.model_data_cb = generic_location_client_data;
	return generic_location_client_reg(0, &generic_location_client_model);
}

uint16_t bt_mesh_generic_location_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_GET: {
		rtk_bt_mesh_generic_location_global_get_t *global_get = (rtk_bt_mesh_generic_location_global_get_t *)p_cmd->param;
		ret = generic_location_global_get(&generic_location_client_model, global_get->dst, global_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_GLOBAL_SET: {
		rtk_bt_mesh_generic_location_global_set_t *global_set = (rtk_bt_mesh_generic_location_global_set_t *)p_cmd->param;
		generic_location_global_t global_value;
        memcpy(&global_value, &global_set->global, sizeof(generic_location_global_t));
        ret = generic_location_global_set(&generic_location_client_model, global_set->dst, global_set->app_key_index, global_value, global_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_GET: {
		rtk_bt_mesh_generic_location_local_get_t *local_get = (rtk_bt_mesh_generic_location_local_get_t *)p_cmd->param;
		ret = generic_location_local_get(&generic_location_client_model, local_get->dst, local_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_LOCATION_CLIENT_MODEL_LOCAL_SET: {
		rtk_bt_mesh_generic_location_local_set_t *local_set = (rtk_bt_mesh_generic_location_local_set_t *)p_cmd->param;
        generic_location_local_t local_value;
        memcpy(&local_value, &local_set->local, sizeof(generic_location_local_t));
        ret = generic_location_local_set(&generic_location_client_model, local_set->dst, local_set->app_key_index, local_value, local_set->ack);
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
#endif  // BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL) && BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL
static mesh_model_info_t generic_property_client_model;

static int32_t generic_property_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case GENERIC_USER_PROPERIES_CLIENT_STATUS:
        {
            generic_properties_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTIES_STATUS,
									sizeof(generic_properties_client_status_t) + pvalue->num_ids * sizeof(uint16_t));
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->num_ids), 1);
            memcpy((uint8_t *)(p_evt->data) + 3, pvalue->pproperty_ids, 2 * pvalue->num_ids);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_USER_PROPERTY_CLIENT_STATUS:
        {
            generic_property_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_STATUS,
									sizeof(generic_property_client_status_t) + pvalue->value_len);
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->property_id), 2);
            memcpy((uint8_t *)(p_evt->data) + 4, &(pvalue->property_access), 1);
            memcpy((uint8_t *)(p_evt->data) + 5, &(pvalue->value_len), 2);
            memcpy((uint8_t *)(p_evt->data) + 7, pvalue->pproperty_value, pvalue->value_len);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_ADMIN_PROPERTIES_CLIENT_STATUS:
        {
            generic_properties_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTIES_STATUS,
									sizeof(generic_properties_client_status_t) + pvalue->num_ids * sizeof(uint16_t));
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->num_ids), 1);
            memcpy((uint8_t *)(p_evt->data) + 3, pvalue->pproperty_ids, 2 * pvalue->num_ids);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_ADMIN_PROPERTY_CLIENT_STATUS:
        {
            generic_property_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_STATUS,
									sizeof(generic_property_client_status_t) + pvalue->value_len);
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->property_id), 2);
            memcpy((uint8_t *)(p_evt->data) + 4, &(pvalue->property_access), 1);
            memcpy((uint8_t *)(p_evt->data) + 5, &(pvalue->value_len), 2);
            memcpy((uint8_t *)(p_evt->data) + 7, pvalue->pproperty_value, pvalue->value_len);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_MANUFACTURER_PROPERTIES_CLIENT_STATUS:
        {
            generic_properties_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTIES_STATUS,
									sizeof(generic_properties_client_status_t) + pvalue->num_ids * sizeof(uint16_t));
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->num_ids), 1);
            memcpy((uint8_t *)(p_evt->data) + 3, pvalue->pproperty_ids, 2 * pvalue->num_ids);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_MANUFACTURER_PROPERTY_CLIENT_STATUS:
        {
            generic_property_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_STATUS,
									sizeof(generic_property_client_status_t) + pvalue->value_len);
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->property_id), 2);
            memcpy((uint8_t *)(p_evt->data) + 4, &(pvalue->property_access), 1);
            memcpy((uint8_t *)(p_evt->data) + 5, &(pvalue->value_len), 2);
            memcpy((uint8_t *)(p_evt->data) + 7, pvalue->pproperty_value, pvalue->value_len);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_CLIENT_PROPERTIES_CLIENT_STATUS:
        {
            generic_properties_client_status_t *pvalue = pargs;
            rtk_bt_evt_t *p_evt = NULL;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_PROPERTY_CLIENT_MODEL, RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_CLIENT_PROPERTY_STATUS,
									sizeof(generic_properties_client_status_t) + pvalue->num_ids * sizeof(uint16_t));
            memcpy(p_evt->data, &(pvalue->src), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pvalue->num_ids), 1);
            memcpy((uint8_t *)(p_evt->data) + 3, pvalue->pproperty_ids, 2 * pvalue->num_ids);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }

    return 0;
}

bool generic_property_client_model_init(void)
{
	generic_property_client_model.model_data_cb = generic_property_client_data;
	return generic_property_client_reg(0, &generic_property_client_model);
}

uint16_t bt_mesh_generic_property_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTIES_GET: {
		rtk_bt_mesh_generic_user_properties_get_t *user_get = (rtk_bt_mesh_generic_user_properties_get_t *)p_cmd->param;
		ret = generic_user_properties_get(&generic_property_client_model, user_get->dst, user_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_GET: {
		rtk_bt_mesh_generic_user_property_get_t *user_property_get = (rtk_bt_mesh_generic_user_property_get_t *)p_cmd->param;
		ret = generic_user_property_get(&generic_property_client_model, user_property_get->dst, user_property_get->app_key_index, user_property_get->property_id);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_USER_PROPERTY_SET: {
		rtk_bt_mesh_generic_user_property_set_t *user_property_set = (rtk_bt_mesh_generic_user_property_set_t *)p_cmd->param;
		ret = generic_user_property_set(&generic_property_client_model, user_property_set->dst, user_property_set->app_key_index, user_property_set->property_id,
                                        user_property_set->pvalue, user_property_set->value_len, user_property_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTIES_GET: {
		rtk_bt_mesh_generic_admin_properties_get_t *admin_get = (rtk_bt_mesh_generic_admin_properties_get_t *)p_cmd->param;
		ret = generic_admin_properties_get(&generic_property_client_model, admin_get->dst, admin_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_GET: {
		rtk_bt_mesh_generic_admin_property_get_t *admin_property_get = (rtk_bt_mesh_generic_admin_property_get_t *)p_cmd->param;
		ret = generic_admin_property_get(&generic_property_client_model, admin_property_get->dst, admin_property_get->app_key_index, admin_property_get->property_id);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_ADMIN_PROPERTY_SET: {
		rtk_bt_mesh_generic_admin_property_set_t *admin_property_set = (rtk_bt_mesh_generic_admin_property_set_t *)p_cmd->param;
		ret = generic_admin_property_set(&generic_property_client_model, admin_property_set->dst, admin_property_set->app_key_index, admin_property_set->property_id,
                                        admin_property_set->property_access, admin_property_set->pvalue, admin_property_set->value_len, admin_property_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTIES_GET: {
		rtk_bt_mesh_generic_manu_properties_get_t *manu_get = (rtk_bt_mesh_generic_manu_properties_get_t *)p_cmd->param;
		ret = generic_manufacturer_properties_get(&generic_property_client_model, manu_get->dst, manu_get->app_key_index);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_GET: {
		rtk_bt_mesh_generic_manu_property_get_t *manu_property_get = (rtk_bt_mesh_generic_manu_property_get_t *)p_cmd->param;
		ret = generic_manufacturer_property_get(&generic_property_client_model, manu_property_get->dst, manu_property_get->app_key_index, manu_property_get->property_id);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_MANU_PROPERTY_SET: {
		rtk_bt_mesh_generic_manu_property_set_t *manu_property_set = (rtk_bt_mesh_generic_manu_property_set_t *)p_cmd->param;
		ret = generic_manufacturer_property_set(&generic_property_client_model, manu_property_set->dst, manu_property_set->app_key_index, manu_property_set->property_id,
                                                manu_property_set->property_access, manu_property_set->ack);
		break;
        }
        case RTK_BT_MESH_GENERIC_PROPERTY_CLIENT_MODEL_CLIENT_PROPERTY_GET: {
		rtk_bt_mesh_generic_client_properties_get_t *client_get = (rtk_bt_mesh_generic_client_properties_get_t *)p_cmd->param;
		ret = generic_client_properties_get(&generic_property_client_model, client_get->dst, client_get->app_key_index, client_get->property_id);
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


#endif  // BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL

mesh_model_info_t generic_level_server_model;

generic_level_server_get_t                               cur_generic_level = {0};
generic_level_server_get_default_transition_time_t       cur_default_transition_time = {0};
generic_level_server_set_t                               generic_level_buffer = {0};
generic_level_server_set_move_t                          generic_level_move_buffer = {0};

static int32_t generic_level_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_LEVEL_SERVER_GET:
        {
            rtk_bt_mesh_generic_level_server_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_GET, sizeof(rtk_bt_mesh_generic_level_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_level_server_get_t *)p_evt->data;
            p_get_data->level = &(((generic_level_server_get_t *)pargs)->level);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_LEVEL_SERVER_GET_DEFAULT_TRANSITION_TIME:
        {
        }
        break;
    case GENERIC_LEVEL_SERVER_SET:
        {
            generic_level_server_set_t *p_get_data = NULL;
            p_get_data = (generic_level_server_set_t *)pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_SET, sizeof(generic_level_server_set_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_level_server_set_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case GENERIC_LEVEL_SERVER_SET_MOVE:
        {
            generic_level_server_set_move_t *p_get_data = NULL;
            p_get_data = (generic_level_server_set_move_t *)pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_LEVEL_SERVER_MODEL_SET_MOVE, sizeof(generic_level_server_set_move_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_level_server_set_move_t));
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

bool generic_level_server_model_init(void)
{
	generic_level_server_model.model_data_cb = generic_level_server_data;
	return generic_level_server_reg(0, &generic_level_server_model);
}

#endif // end of BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL
mesh_model_info_t generic_power_on_off_server_model;

static int32_t generic_power_on_off_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_POWER_ON_OFF_SERVER_GET:
        {
            rtk_bt_mesh_generic_power_on_off_server_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_GET, 
                                        sizeof(rtk_bt_mesh_generic_power_on_off_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_power_on_off_server_get_t *)p_evt->data;
            p_get_data->on_power_up = &(((generic_power_on_off_server_get_t *)pargs)->on_power_up);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_power_on_off_server_model_init(void)
{
	generic_power_on_off_server_model.model_data_cb = generic_power_on_off_server_data;
	return generic_power_on_off_server_reg(0, &generic_power_on_off_server_model);
}

#endif // end of BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL
mesh_model_info_t generic_power_on_off_setup_server_model;

static int32_t generic_power_on_off_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_POWER_ON_OFF_SERVER_SET:
        {
            generic_power_on_off_server_set_t *p_get_data = NULL;
            p_get_data = (generic_power_on_off_server_set_t *)pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL_SET, 
                                            sizeof(generic_power_on_off_server_set_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_power_on_off_server_set_t));
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

bool generic_power_on_off_setup_server_model_init(void)
{
	generic_power_on_off_setup_server_model.model_data_cb = generic_power_on_off_setup_server_data;
	return generic_power_on_off_setup_server_reg(0, &generic_power_on_off_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL
mesh_model_info_t generic_power_level_server_model;
static int32_t generic_power_level_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
    case GENERIC_POWER_LEVEL_SERVER_GET:
        {
            rtk_bt_mesh_generic_power_level_server_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_GET, 
                                        sizeof(rtk_bt_mesh_generic_power_level_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_power_level_server_get_t *)p_evt->data;
            p_get_data->power = &(((generic_power_level_server_get_t *)pargs)->power);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_GET_DEFAULT_TRANSITION_TIME:
        {
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_GET_RANGE:
        {
            rtk_bt_mesh_generic_power_level_server_direct_get_range_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_RANGE_GET, 
                                        sizeof(rtk_bt_mesh_generic_power_level_server_direct_get_range_t));
            p_get_data = (rtk_bt_mesh_generic_power_level_server_direct_get_range_t *)p_evt->data;
            p_get_data->value = pargs; //for test
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_SET:
        {
            generic_power_level_server_set_t *p_get_data = NULL;
            p_get_data = (generic_power_level_server_set_t *)pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_SET, 
                                            sizeof(generic_power_level_server_set_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_power_level_server_set_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_GET_LAST:
        {
            rtk_bt_mesh_generic_power_level_server_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_LAST_GET, 
                                        sizeof(rtk_bt_mesh_generic_power_level_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_power_level_server_get_t *)p_evt->data;
            p_get_data->power = &(((generic_power_level_server_get_t *)pargs)->power);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_SET_LAST:
        {
            generic_power_level_server_set_last_t *p_get_data = NULL;
            p_get_data = (generic_power_level_server_set_last_t *)pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_LAST_SET, 
                                            sizeof(generic_power_level_server_set_last_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_power_level_server_set_last_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_GET_DEFAULT:
        {
            rtk_bt_mesh_generic_power_level_server_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL_DEFAULT_GET, 
                                        sizeof(rtk_bt_mesh_generic_power_level_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_power_level_server_get_t *)p_evt->data;
            p_get_data->power = &(((generic_power_level_server_get_t *)pargs)->power);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_power_level_server_model_init(void)
{
	generic_power_level_server_model.model_data_cb = generic_power_level_server_data;
	return generic_power_level_server_reg(0, &generic_power_level_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL
mesh_model_info_t generic_power_level_setup_server_model;
static int32_t generic_power_level_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_POWER_LEVEL_SERVER_SET_DEFAULT:
        {
            generic_power_level_server_set_default_t *p_get_data = NULL;
            p_get_data = (generic_power_level_server_set_default_t *)pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL_DEFAULT_SET, 
                                            sizeof(generic_power_level_server_set_default_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_power_level_server_set_default_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case GENERIC_POWER_LEVEL_SERVER_SET_RANGE:
        {
            generic_power_level_server_set_range_t *p_get_data = NULL;
            p_get_data = (generic_power_level_server_set_range_t*) pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL_RANGE_SET, 
                                            sizeof(generic_power_level_server_set_range_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_power_level_server_set_range_t));
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

bool generic_power_level_setup_server_model_init(void)
{
	generic_power_level_setup_server_model.model_data_cb = generic_power_level_setup_server_data;
	return generic_power_level_setup_server_reg(0, &generic_power_level_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL
mesh_model_info_t generic_battery_server_model;

static int32_t generic_battery_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_BATTERY_SERVER_GET:
        {
            rtk_bt_mesh_generic_battery_server_direct_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_BATTERY_SERVER_MODEL, RTK_BT_MESH_GENERIC_BATTERY_SERVER_MODEL_GET, 
                                        sizeof(rtk_bt_mesh_generic_battery_server_direct_get_t));
            p_get_data = (rtk_bt_mesh_generic_battery_server_direct_get_t *)p_evt->data;
            p_get_data->value = pargs;
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_battery_server_model_init(void)
{
	generic_battery_server_model.model_data_cb = generic_battery_server_data;
	return generic_battery_server_reg(0, &generic_battery_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL
mesh_model_info_t generic_location_server_model;

static int32_t generic_location_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_LOCATION_SERVER_GET_GLOBAL:
        {
            rtk_bt_mesh_generic_location_server_direct_get_global_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SERVER_MODEL, RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_GLOBAL_GET, 
                                        sizeof(rtk_bt_mesh_generic_location_server_direct_get_global_t));
            p_get_data = (rtk_bt_mesh_generic_location_server_direct_get_global_t *)p_evt->data;
            p_get_data->value = pargs;
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_LOCATION_SERVER_GET_LOCAL:
        {
            rtk_bt_mesh_generic_location_server_direct_get_local_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SERVER_MODEL, RTK_BT_MESH_GENERIC_LOCATION_SERVER_MODEL_LOCAL_GET, 
                                        sizeof(rtk_bt_mesh_generic_location_server_direct_get_local_t));
            p_get_data = (rtk_bt_mesh_generic_location_server_direct_get_local_t *)p_evt->data;
            p_get_data->value = pargs;
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_location_server_model_init(void)
{
	generic_location_server_model.model_data_cb = generic_location_server_data;
	return generic_location_server_reg(0, &generic_location_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL
mesh_model_info_t generic_location_setup_server_model;

static int32_t generic_location_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_LOCATION_SERVER_GET_GLOBAL:
        {
            rtk_bt_mesh_generic_location_server_direct_get_global_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_GLOBAL_GET, 
                                        sizeof(rtk_bt_mesh_generic_location_server_direct_get_global_t));
            p_get_data = (rtk_bt_mesh_generic_location_server_direct_get_global_t *)p_evt->data;
            p_get_data->value = pargs;
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_LOCATION_SERVER_GET_LOCAL:
        {
            rtk_bt_mesh_generic_location_server_direct_get_local_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_LOCAL_GET, 
                                        sizeof(rtk_bt_mesh_generic_location_server_direct_get_local_t));
            p_get_data = (rtk_bt_mesh_generic_location_server_direct_get_local_t *)p_evt->data;
            p_get_data->value = pargs;
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_LOCATION_SERVER_SET_GLOBAL:
        {
            generic_location_server_set_global_t *p_get_data = NULL;
            p_get_data = (generic_location_server_set_global_t*) pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_GLOBAL_SET, 
                                            sizeof(generic_location_server_set_global_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_location_server_set_global_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case GENERIC_LOCATION_SERVER_SET_LOCAL:
        {
            generic_location_server_set_local_t *p_get_data = NULL;
            p_get_data = (generic_location_server_set_local_t*) pargs;
            if (p_get_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL, RTK_BT_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL_LOCAL_SET, 
                                            sizeof(generic_location_server_set_local_t));
                memcpy(p_evt->data, p_get_data, sizeof(generic_location_server_set_local_t));
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

bool generic_location_setup_server_model_init(void)
{
	generic_location_setup_server_model.model_data_cb = generic_location_setup_server_data;
	return generic_location_setup_server_reg(0, &generic_location_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL

mesh_model_info_t generic_user_property_server_model;

uint8_t user_pvalue_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};
uint16_t user_ids_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};

uint8_t user_store_value[3] = {0x1,0x1,0x2};
uint8_t user_store_value2[2] = {0x3,0x5};

static generic_property_db_t user_db[] =
{
    {1,3,user_store_value,3},
    {2,3,user_store_value2,2}
};
static int32_t generic_user_property_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_USER_PROPERTY_SERVER_SET:
        {
            generic_user_property_server_set_t *pdata = pargs;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_PROPERTY_SET, 
                                        sizeof(rtk_bt_mesh_generic_user_property_server_set_t) + pdata->value_len);
            memcpy(p_evt->data, &(pdata->property_id), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pdata->value_len), 2);
            memcpy((uint8_t *)(p_evt->data) + 4, pdata->pproperty_value, pdata->value_len);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_PARAMETER_GET:
        {
            generic_property_server_para_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_para_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_PARAMETER_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_para_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_para_get_t *)p_evt->data;
            p_get_data->property_id = pdata->property_id;
            p_get_data->pvalue = (rtk_bt_mesh_generic_property_server_para_t *) &(((generic_property_server_para_get_t *)pargs)->pvalue);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_VALUE_GET:
        {
            generic_property_server_value_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_value_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_VALUE_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_value_get_t) + pdata->value_len);
            p_get_data = (rtk_bt_mesh_generic_property_server_value_get_t *)p_evt->data;
            p_get_data->property_id = pdata->property_id;
            p_get_data->value_len = pdata->value_len;
            p_get_data->pproperty_value = user_pvalue_store;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->pproperty_value = user_pvalue_store;
        }
        break;
    case GENERIC_PROPERTY_SERVER_NUM_IDS_GET:
        {
            rtk_bt_mesh_generic_property_server_num_ids_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_NUM_IDS_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_num_ids_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_num_ids_get_t *)p_evt->data;
            p_get_data->pvalue = (rtk_bt_mesh_generic_property_server_num_ids_t *) &(((generic_property_server_num_ids_get_t *)pargs)->pvalue);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_IDS_VALUE_GET:
        {
            generic_property_server_num_ids_value_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_num_ids_value_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_IDS_VALUE_GET, 
                                        pdata->num_ids * 2 + sizeof(rtk_bt_mesh_generic_property_server_num_ids_value_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)p_evt->data;
            p_get_data->num_ids = pdata->num_ids;
            p_get_data->property_ids = user_ids_store;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->property_ids = user_ids_store;
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_user_property_server_model_init(void)
{
    bool ret = 0;
	generic_user_property_server_model.model_data_cb = generic_user_property_server_data;
	ret = generic_user_property_server_reg(0, &generic_user_property_server_model);
    if (ret)
    {
        generic_user_property_server_set_db(&generic_user_property_server_model,user_db,sizeof(user_db)/sizeof(generic_property_db_t));
    }
    return ret;
}

uint16_t bt_mesh_generic_user_property_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_generic_property_server_db_set_t *db_get = (rtk_bt_mesh_generic_property_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using generic_user_property_server_set_db *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        generic_user_property_server_set_db(&generic_user_property_server_model,user_db,sizeof(user_db)/sizeof(generic_property_db_t));
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

#endif // end of BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL

mesh_model_info_t generic_admin_property_server_model;

uint8_t admin_store_value[3] = {0x1,0x1,0x2};
uint8_t admin_store_value2[2] = {0x3,0x5};
static generic_property_db_t admin_db[] =
{
    {1,3,admin_store_value,3},
    {2,3,admin_store_value2,2}
};

uint8_t admin_pvalue_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};
uint16_t admin_ids_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};

static int32_t generic_admin_property_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_ADMIN_PROPERTY_SERVER_SET:
        {
            generic_admin_property_server_set_t *pdata = pargs;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_PROPERTY_SET, 
                                        sizeof(generic_admin_property_server_set_t) + pdata->value_len);
            memcpy(p_evt->data, &(pdata->property_id), 2);
            memcpy((uint8_t *)(p_evt->data) + 2, &(pdata->property_access), 1);
            memcpy((uint8_t *)(p_evt->data) + 3, &(pdata->value_len), 2);
            memcpy((uint8_t *)(p_evt->data) + 5, pdata->pproperty_value, pdata->value_len);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_PARAMETER_GET:
        {
            generic_property_server_para_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_para_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_PARAMETER_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_para_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_para_get_t *)p_evt->data;
            p_get_data->property_id = pdata->property_id;
            p_get_data->pvalue = (rtk_bt_mesh_generic_property_server_para_t *) &(((generic_property_server_para_get_t *)pargs)->pvalue);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_VALUE_GET:
        {
            generic_property_server_value_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_value_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_VALUE_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_value_get_t) + pdata->value_len);
            p_get_data = (rtk_bt_mesh_generic_property_server_value_get_t *)p_evt->data;
            p_get_data->property_id = pdata->property_id;
            p_get_data->value_len = pdata->value_len;
            p_get_data->pproperty_value = admin_pvalue_store;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->pproperty_value = admin_pvalue_store;
        }
        break;
    case GENERIC_PROPERTY_SERVER_NUM_IDS_GET:
        {
            rtk_bt_mesh_generic_property_server_num_ids_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_NUM_IDS_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_num_ids_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_num_ids_get_t *)p_evt->data;
            p_get_data->pvalue = (rtk_bt_mesh_generic_property_server_num_ids_t *) &(((generic_property_server_num_ids_get_t *)pargs)->pvalue);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_IDS_VALUE_GET:
        {
            generic_property_server_num_ids_value_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_num_ids_value_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_IDS_VALUE_GET, 
                                        pdata->num_ids * 2 + sizeof(rtk_bt_mesh_generic_property_server_num_ids_value_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)p_evt->data;
            p_get_data->num_ids = pdata->num_ids;
            p_get_data->property_ids = admin_ids_store;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->property_ids = admin_ids_store;
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_admin_property_server_model_init(void)
{
    bool ret = 0;
	generic_admin_property_server_model.model_data_cb = generic_admin_property_server_data;
    ret = generic_admin_property_server_reg(0, &generic_admin_property_server_model);
    if (ret)
    {
        generic_admin_property_server_set_db(&generic_admin_property_server_model,admin_db,sizeof(admin_db)/sizeof(generic_property_db_t));
    }
    return ret;
}

uint16_t bt_mesh_generic_admin_property_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_generic_property_server_db_set_t *db_get = (rtk_bt_mesh_generic_property_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using generic_admin_property_server_set_db *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        generic_admin_property_server_set_db(&generic_admin_property_server_model,admin_db,sizeof(admin_db)/sizeof(generic_property_db_t));
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

#endif // end of BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL
mesh_model_info_t generic_manu_property_server_model;

uint8_t manu_store_value[3] = {0x1,0x1,0x2};
uint8_t manu_store_value2[2] = {0x3,0x5};
static generic_property_db_t manu_db[] =
{
    {1,3,manu_store_value,3},
    {2,3,manu_store_value2,2}
};

uint8_t manu_pvalue_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};
uint16_t manu_ids_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};

static int32_t generic_manu_property_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_MANUFACTURER_PROPERTY_SERVER_SET:
        {
            generic_manufacturer_property_server_set_t *pdata = pargs;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_PROPERTY_SET, 
                                        sizeof(generic_manufacturer_property_server_set_t));
            memcpy(p_evt->data, pdata, sizeof(generic_manufacturer_property_server_set_t));
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_PARAMETER_GET:
        {
            generic_property_server_para_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_para_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_PARAMETER_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_para_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_para_get_t *)p_evt->data;
            p_get_data->property_id = pdata->property_id;
            p_get_data->pvalue = (rtk_bt_mesh_generic_property_server_para_t *) &(((generic_property_server_para_get_t *)pargs)->pvalue);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_VALUE_GET:
        {
            generic_property_server_value_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_value_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_VALUE_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_value_get_t) + pdata->value_len);
            p_get_data = (rtk_bt_mesh_generic_property_server_value_get_t *)p_evt->data;
            p_get_data->property_id = pdata->property_id;
            p_get_data->value_len = pdata->value_len;
            p_get_data->pproperty_value = manu_pvalue_store;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->pproperty_value = manu_pvalue_store;
        }
        break;
    case GENERIC_PROPERTY_SERVER_NUM_IDS_GET:
        {
            rtk_bt_mesh_generic_property_server_num_ids_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_NUM_IDS_GET, 
                                        sizeof(rtk_bt_mesh_generic_property_server_num_ids_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_num_ids_get_t *)p_evt->data;
            p_get_data->pvalue = (rtk_bt_mesh_generic_property_server_num_ids_t *) &(((generic_property_server_num_ids_get_t *)pargs)->pvalue);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case GENERIC_PROPERTY_SERVER_IDS_VALUE_GET:
        {
            generic_property_server_num_ids_value_get_t *pdata = pargs;
            rtk_bt_mesh_generic_property_server_num_ids_value_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_IDS_VALUE_GET, 
                                        pdata->num_ids * 2 + sizeof(rtk_bt_mesh_generic_property_server_num_ids_value_get_t));
            p_get_data = (rtk_bt_mesh_generic_property_server_num_ids_value_get_t *)p_evt->data;
            p_get_data->num_ids = pdata->num_ids;
            p_get_data->property_ids = manu_ids_store;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->property_ids = manu_ids_store;
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_manu_property_server_model_init(void)
{
    bool ret = 0;
	generic_manu_property_server_model.model_data_cb = generic_manu_property_server_data;
    ret = generic_manufacturer_property_server_reg(0, &generic_manu_property_server_model);
    if (ret)
    {
        generic_manufacturer_property_server_set_db(&generic_manu_property_server_model,manu_db,sizeof(manu_db)/sizeof(generic_property_db_t));
    }
    return ret;
}

uint16_t bt_mesh_generic_manu_property_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_generic_property_server_db_set_t *db_get = (rtk_bt_mesh_generic_property_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using generic_manufacturer_property_server_set_db *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        generic_manufacturer_property_server_set_db(&generic_manu_property_server_model,manu_db,sizeof(manu_db)/sizeof(generic_property_db_t));
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

#endif // end of BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL

#if defined(BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL) && BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL
mesh_model_info_t generic_client_property_server_model;

uint8_t client_ids_stores[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};
uint16_t client_ids_value_store[GENERIC_PROPERTY_DATA_MAX_LEN] = {0};
static int32_t generic_client_property_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
        
    case GENERIC_CLIENT_PROPERTY_SERVER_GET:
        {
            generic_client_property_server_get_t *pdata = pargs;
            rtk_bt_mesh_generic_client_property_server_direct_get_t *p_get_data;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL, RTK_BT_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL_PROPERTY_GET, 
                                        2 * GENERIC_PROPERTY_DATA_MAX_LEN + sizeof(generic_client_property_server_get_t));
            p_get_data = (rtk_bt_mesh_generic_client_property_server_direct_get_t *)p_evt->data;
            p_get_data->property_id_start = pdata->property_id_start;
            p_get_data->value = client_ids_stores;
            rtk_bt_evt_indicate(p_evt, NULL);
            pdata->num_ids = client_ids_stores[0];
            memcpy(client_ids_value_store,&client_ids_stores[1], 2 * client_ids_stores[0]);// for avoid address misaligned
            pdata->pproperty_ids = client_ids_value_store;
        }
        break;
    
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool generic_client_property_server_model_init(void)
{
	generic_client_property_server_model.model_data_cb = generic_client_property_server_data;
	return generic_client_property_server_reg(0, &generic_client_property_server_model);
}

#endif // end of BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT