#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_light_model.h>
#include <light_lightness.h>
#include <light_ctl.h>
#include <light_hsl.h>
#include <light_xyl.h>
#include <light_lc.h>
#include <light_lc_fsm.h>

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL
static mesh_model_info_t light_lightness_client_model;

/*****************light lightness client model************************/
static int32_t light_lightness_client_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_LIGHTNESS_CLIENT_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_STATUS,
									sizeof(light_lightness_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_lightness_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LIGHTNESS_CLIENT_STATUS_LINEAR: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_STATUS,
									sizeof(light_lightness_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_lightness_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LIGHTNESS_CLIENT_STATUS_LAST: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_CLIENT_MODEL_STATUS,
									sizeof(light_lightness_client_status_last_t));
		memcpy(p_evt->data, pargs, sizeof(light_lightness_client_status_last_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LIGHTNESS_CLIENT_STATUS_DEFAULT: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_STATUS,
									sizeof(light_lightness_client_status_default_t));
		memcpy(p_evt->data, pargs, sizeof(light_lightness_client_status_default_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LIGHTNESS_CLIENT_STATUS_RANGE: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_STATUS,
									sizeof(light_lightness_client_status_range_t));
		memcpy(p_evt->data, pargs, sizeof(light_lightness_client_status_range_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return 0;
}

bool light_lightness_client_model_init(void)
{
	light_lightness_client_model.model_data_cb = light_lightness_client_data;
	return light_lightness_client_reg(0, &light_lightness_client_model);
}

uint16_t bt_mesh_light_lightness_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lightness_get_t *lightness_get = (rtk_bt_mesh_light_lightness_get_t *)p_cmd->param;
		ret = light_lightness_get(&light_lightness_client_model, lightness_get->dst, lightness_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lightness_set_t *lightness_set = (rtk_bt_mesh_light_lightness_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (lightness_set->trans_time).num_steps;
		time.step_resolution = (lightness_set->trans_time).step_resolution;
		ret = light_lightness_set(&light_lightness_client_model, lightness_set->dst, lightness_set->app_key_index,
								  lightness_set->lightness, lightness_set->tid, lightness_set->optional,
								  time, lightness_set->delay, lightness_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lightness_linear_get_t *lightness_linear_get = (rtk_bt_mesh_light_lightness_linear_get_t *)p_cmd->param;
		ret = light_lightness_linear_get(&light_lightness_client_model, lightness_linear_get->dst, lightness_linear_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lightness_linear_set_t *lightness_linear_set = (rtk_bt_mesh_light_lightness_linear_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (lightness_linear_set->trans_time).num_steps;
		time.step_resolution = (lightness_linear_set->trans_time).step_resolution;
		ret = light_lightness_linear_set(&light_lightness_client_model, lightness_linear_set->dst, lightness_linear_set->app_key_index,
										 lightness_linear_set->lightness, lightness_linear_set->tid, lightness_linear_set->optional,
										 time, lightness_linear_set->delay, lightness_linear_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lightness_last_get_t *lightness_last_get = (rtk_bt_mesh_light_lightness_last_get_t *)p_cmd->param;
		ret = light_lightness_last_get(&light_lightness_client_model, lightness_last_get->dst, lightness_last_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lightness_default_get_t *lightness_default_get = (rtk_bt_mesh_light_lightness_default_get_t *)p_cmd->param;
		ret = light_lightness_default_get(&light_lightness_client_model, lightness_default_get->dst, lightness_default_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lightness_default_set_t *lightness_default_set = (rtk_bt_mesh_light_lightness_default_set_t *)p_cmd->param;
		ret = light_lightness_default_set(&light_lightness_client_model, lightness_default_set->dst, lightness_default_set->app_key_index,
										  lightness_default_set->lightness, lightness_default_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lightness_range_get_t *lightness_range_get = (rtk_bt_mesh_light_lightness_range_get_t *)p_cmd->param;
		ret = light_lightness_range_get(&light_lightness_client_model, lightness_range_get->dst, lightness_range_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lightness_range_set_t *lightness_range_set = (rtk_bt_mesh_light_lightness_range_set_t *)p_cmd->param;
		ret = light_lightness_range_set(&light_lightness_client_model, lightness_range_set->dst, lightness_range_set->app_key_index,
										lightness_range_set->range_min, lightness_range_set->range_max, lightness_range_set->ack);
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
#endif  // BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL
/*******************light_ctl_client_model****************************/
static mesh_model_info_t light_ctl_client_model;

static int32_t light_ctl_client_data(const mesh_model_info_p pmodel_info,
									 uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_CTL_CLIENT_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_STATUS,
									sizeof(light_ctl_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_ctl_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_CTL_CLIENT_STATUS_TEMPERATURE: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_STATUS,
									sizeof(light_ctl_client_status_temperature_t));
		memcpy(p_evt->data, pargs, sizeof(light_ctl_client_status_temperature_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_CTL_CLIENT_STATUS_TEMPERATURE_RANGE: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_STATUS,
									sizeof(light_ctl_client_status_temperature_range_t));
		memcpy(p_evt->data, pargs, sizeof(light_ctl_client_status_temperature_range_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_CTL_CLIENT_STATUS_DEFAULT: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_STATUS,
									sizeof(light_ctl_client_status_default_t));
		memcpy(p_evt->data, pargs, sizeof(light_ctl_client_status_default_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool light_ctl_client_model_init(void)
{
	light_ctl_client_model.model_data_cb = light_ctl_client_data;
	return light_ctl_client_reg(0, &light_ctl_client_model);
}

uint16_t bt_mesh_light_ctl_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_ctl_get_t *ctl_get = (rtk_bt_mesh_light_ctl_get_t *)p_cmd->param;
		ret = light_ctl_get(&light_ctl_client_model, ctl_get->dst, ctl_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_ctl_set_t *ctl_set = (rtk_bt_mesh_light_ctl_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (ctl_set->trans_time).num_steps;
		time.step_resolution = (ctl_set->trans_time).step_resolution;
		ret = light_ctl_set(&light_ctl_client_model, ctl_set->dst, ctl_set->app_key_index, ctl_set->lightness,
							ctl_set->temperature, ctl_set->delta_uv, ctl_set->tid, ctl_set->optional,
							time, ctl_set->delay, ctl_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_GET: {
		rtk_bt_mesh_light_ctl_temperature_get_t *ctl_temperature_get = (rtk_bt_mesh_light_ctl_temperature_get_t *)p_cmd->param;
		ret = light_ctl_temperature_get(&light_ctl_client_model, ctl_temperature_get->dst, ctl_temperature_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_SET: {
		rtk_bt_mesh_light_ctl_temperature_set_t *ctl_temperature_set = (rtk_bt_mesh_light_ctl_temperature_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (ctl_temperature_set->trans_time).num_steps;
		time.step_resolution = (ctl_temperature_set->trans_time).step_resolution;
		ret = light_ctl_temperature_set(&light_ctl_client_model, ctl_temperature_set->dst, ctl_temperature_set->app_key_index,
										ctl_temperature_set->temperature, ctl_temperature_set->delta_uv, ctl_temperature_set->tid,
										ctl_temperature_set->optional, time, ctl_temperature_set->delay, ctl_temperature_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_GET: {
		rtk_bt_mesh_light_ctl_temperature_range_get_t *ctl_temperature_range_get = (rtk_bt_mesh_light_ctl_temperature_range_get_t *)p_cmd->param;
		ret = light_ctl_temperature_range_get(&light_ctl_client_model, ctl_temperature_range_get->dst, ctl_temperature_range_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_TEMPERATURE_RANGE_SET: {
		rtk_bt_mesh_light_ctl_temperature_range_set_t *ctl_temperature_range_set = (rtk_bt_mesh_light_ctl_temperature_range_set_t *)p_cmd->param;
		ret = light_ctl_temperature_range_set(&light_ctl_client_model, ctl_temperature_range_set->dst, ctl_temperature_range_set->app_key_index,
											  ctl_temperature_range_set->range_min, ctl_temperature_range_set->range_max, ctl_temperature_range_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_GET: {
		rtk_bt_mesh_light_ctl_default_get_t *ctl_default_get = (rtk_bt_mesh_light_ctl_default_get_t *)p_cmd->param;
		ret = light_ctl_default_get(&light_ctl_client_model, ctl_default_get->dst, ctl_default_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_CTL_CLIENT_MODEL_DEFAULT_SET: {
		rtk_bt_mesh_light_ctl_default_set_t *ctl_default_set = (rtk_bt_mesh_light_ctl_default_set_t *)p_cmd->param;
		ret = light_ctl_default_set(&light_ctl_client_model, ctl_default_set->dst, ctl_default_set->app_key_index, ctl_default_set->lightness,
									ctl_default_set->temperature, ctl_default_set->delta_uv, ctl_default_set->ack);
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
#endif  // BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL
/****************light hsl client model*************************/
static mesh_model_info_t light_hsl_client_model;

static int32_t light_hsl_client_data(const mesh_model_info_p pmodel_info,
									 uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_HSL_CLIENT_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_STATUS,
									sizeof(light_hsl_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_hsl_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_HSL_CLIENT_STATUS_TARGET: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_HSL_TARGET_CLIENT_MODEL_STATUS,
									sizeof(light_hsl_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_hsl_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_HSL_CLIENT_STATUS_HUE: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_STATUS,
									sizeof(light_hsl_client_status_hue_t));
		memcpy(p_evt->data, pargs, sizeof(light_hsl_client_status_hue_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_HSL_CLIENT_STATUS_SATURATION: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_STATUS,
									sizeof(light_hsl_client_status_saturation_t));
		memcpy(p_evt->data, pargs, sizeof(light_hsl_client_status_saturation_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_HSL_CLIENT_STATUS_DEFAULT: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_STATUS,
									sizeof(light_hsl_client_status_default_t));
		memcpy(p_evt->data, pargs, sizeof(light_hsl_client_status_default_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_HSL_CLIENT_STATUS_RANGE: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_STATUS,
									sizeof(light_hsl_client_status_range_t));
		memcpy(p_evt->data, pargs, sizeof(light_hsl_client_status_range_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool light_hsl_client_model_init(void)
{
	light_hsl_client_model.model_data_cb = light_hsl_client_data;
	return light_hsl_client_reg(0, &light_hsl_client_model);
}

uint16_t bt_mesh_light_hsl_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_hsl_get_t *hsl_get = (rtk_bt_mesh_light_hsl_get_t *)p_cmd->param;
		ret = light_hsl_get(&light_hsl_client_model, hsl_get->dst, hsl_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_hsl_set_t *hsl_set = (rtk_bt_mesh_light_hsl_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (hsl_set->trans_time).num_steps;
		time.step_resolution = (hsl_set->trans_time).step_resolution;
		ret = light_hsl_set(&light_hsl_client_model, hsl_set->dst, hsl_set->app_key_index, hsl_set->lightness,
							hsl_set->hue, hsl_set->saturation, hsl_set->tid, hsl_set->optional,
							time, hsl_set->delay, hsl_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_TARGET_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_hsl_target_get_t *hsl_target_get = (rtk_bt_mesh_light_hsl_target_get_t *)p_cmd->param;
		ret = light_hsl_target_get(&light_hsl_client_model, hsl_target_get->dst, hsl_target_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_hsl_hue_get_t *hsl_hue_get = (rtk_bt_mesh_light_hsl_hue_get_t *)p_cmd->param;
		ret = light_hsl_hue_get(&light_hsl_client_model, hsl_hue_get->dst, hsl_hue_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_HUE_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_hsl_hue_set_t *hsl_hue_set = (rtk_bt_mesh_light_hsl_hue_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (hsl_hue_set->trans_time).num_steps;
		time.step_resolution = (hsl_hue_set->trans_time).step_resolution;
		ret = light_hsl_hue_set(&light_hsl_client_model, hsl_hue_set->dst, hsl_hue_set->app_key_index,
								hsl_hue_set->hue, hsl_hue_set->tid, hsl_hue_set->optional,
								time, hsl_hue_set->delay, hsl_hue_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_hsl_saturation_get_t *hsl_saturation_get = (rtk_bt_mesh_light_hsl_saturation_get_t *)p_cmd->param;
		ret = light_hsl_saturation_get(&light_hsl_client_model, hsl_saturation_get->dst, hsl_saturation_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_SATURATION_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_hsl_saturation_set_t *hsl_saturation_set = (rtk_bt_mesh_light_hsl_saturation_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (hsl_saturation_set->trans_time).num_steps;
		time.step_resolution = (hsl_saturation_set->trans_time).step_resolution;
		ret = light_hsl_saturation_set(&light_hsl_client_model, hsl_saturation_set->dst, hsl_saturation_set->app_key_index,
									   hsl_saturation_set->saturation, hsl_saturation_set->tid, hsl_saturation_set->optional,
									   time, hsl_saturation_set->delay, hsl_saturation_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_hsl_default_get_t *hsl_default_get = (rtk_bt_mesh_light_hsl_default_get_t *)p_cmd->param;
		ret = light_hsl_default_get(&light_hsl_client_model, hsl_default_get->dst, hsl_default_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_DEFAULT_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_hsl_default_set_t *hsl_default_set = (rtk_bt_mesh_light_hsl_default_set_t *)p_cmd->param;
		ret = light_hsl_default_set(&light_hsl_client_model, hsl_default_set->dst, hsl_default_set->app_key_index,
									hsl_default_set->lightness, hsl_default_set->hue, hsl_default_set->saturation, hsl_default_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_hsl_range_get_t *hsl_range_get = (rtk_bt_mesh_light_hsl_range_get_t *)p_cmd->param;
		ret = light_hsl_range_get(&light_hsl_client_model, hsl_range_get->dst, hsl_range_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_HSL_RANGE_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_hsl_range_set_t *hsl_range_set = (rtk_bt_mesh_light_hsl_range_set_t *)p_cmd->param;
		ret = light_hsl_range_set(&light_hsl_client_model, hsl_range_set->dst, hsl_range_set->app_key_index, hsl_range_set->hue_range_min,
								  hsl_range_set->hue_range_max, hsl_range_set->saturation_range_min, hsl_range_set->saturation_range_max,
								  hsl_range_set->ack);
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
#endif  // BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL
/***************light xyl client model*********************/
static mesh_model_info_t light_xyl_client_model;

static int32_t light_xyl_client_data(const mesh_model_info_p pmodel_info,
									 uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_XYL_CLIENT_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_STATUS,
									sizeof(light_xyl_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_xyl_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_XYL_CLIENT_STATUS_TARGET: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_XYL_TARGET_CLIENT_MODEL_STATUS,
									sizeof(light_xyl_client_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_xyl_client_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_XYL_CLIENT_STATUS_DEFAULT: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_STATUS,
									sizeof(light_xyl_client_status_default_t));
		memcpy(p_evt->data, pargs, sizeof(light_xyl_client_status_default_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_XYL_CLIENT_STATUS_RANGE: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_CLIENT_MODEL, RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_STATUS,
									sizeof(light_xyl_client_status_range_t));
		memcpy(p_evt->data, pargs, sizeof(light_xyl_client_status_range_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool light_xyl_client_model_init(void)
{
	light_xyl_client_model.model_data_cb = light_xyl_client_data;
	return light_xyl_client_reg(0, &light_xyl_client_model);
}

uint16_t bt_mesh_light_xyl_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_xyl_get_t *xyl_get = (rtk_bt_mesh_light_xyl_get_t *)p_cmd->param;
		ret = light_xyl_get(&light_xyl_client_model, xyl_get->dst, xyl_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_xyl_set_t *xyl_set = (rtk_bt_mesh_light_xyl_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (xyl_set->trans_time).num_steps;
		time.step_resolution = (xyl_set->trans_time).step_resolution;
		light_xyl_t xyl_value = {(xyl_set->xyl).xyl_lightness, (xyl_set->xyl).xyl_x, (xyl_set->xyl).xyl_y};
		ret = light_xyl_set(&light_xyl_client_model, xyl_set->dst, xyl_set->app_key_index, xyl_value,
							xyl_set->tid, xyl_set->optional, time,
							xyl_set->delay, xyl_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_TARGET_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_xyl_target_get_t *xyl_target_set = (rtk_bt_mesh_light_xyl_target_get_t *)p_cmd->param;
		ret = light_xyl_target_get(&light_xyl_client_model, xyl_target_set->dst, xyl_target_set->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_xyl_default_get_t *xyl_default_get = (rtk_bt_mesh_light_xyl_default_get_t *)p_cmd->param;
		ret = light_xyl_default_get(&light_xyl_client_model, xyl_default_get->dst, xyl_default_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_DEFAULT_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_xyl_default_set_t *xyl_default_set = (rtk_bt_mesh_light_xyl_default_set_t *)p_cmd->param;
		light_xyl_t xyl_value = {(xyl_default_set->xyl).xyl_lightness, (xyl_default_set->xyl).xyl_x, (xyl_default_set->xyl).xyl_y};
		ret = light_xyl_default_set(&light_xyl_client_model, xyl_default_set->dst, xyl_default_set->app_key_index,
									xyl_value, xyl_default_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_xyl_range_get_t *xyl_range_get = (rtk_bt_mesh_light_xyl_range_get_t *)p_cmd->param;
		ret = light_xyl_range_get(&light_xyl_client_model, xyl_range_get->dst, xyl_range_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_XYL_RANGE_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_xyl_range_set_t *xyl_range_set = (rtk_bt_mesh_light_xyl_range_set_t *)p_cmd->param;
		ret = light_xyl_range_set(&light_xyl_client_model, xyl_range_set->dst, xyl_range_set->app_key_index,
								  xyl_range_set->xyl_x_range_min, xyl_range_set->xyl_x_range_max,
								  xyl_range_set->xyl_y_range_min, xyl_range_set->xyl_y_range_max, xyl_range_set->ack);
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
#endif  // BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL) && BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL
/*******************light lc client model**********************************/
static mesh_model_info_t light_lc_client_model;

static int32_t light_lc_client_data(const mesh_model_info_p pmodel_info,
									uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_LC_CLIENT_MODE_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_STATUS,
									sizeof(light_lc_client_mode_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_lc_client_mode_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LC_CLIENT_OM_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_STATUS,
									sizeof(light_lc_client_om_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_lc_client_om_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LC_CLIENT_LIGHT_ON_OFF_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_STATUS,
									sizeof(light_lc_client_light_on_off_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_lc_client_light_on_off_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	case LIGHT_LC_CLIENT_PROPERTY_STATUS: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_CLIENT_MODEL, RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_STATUS,
									sizeof(light_lc_client_property_status_t));
		memcpy(p_evt->data, pargs, sizeof(light_lc_client_property_status_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool light_lc_client_model_init(void)
{
	light_lc_client_model.model_data_cb = light_lc_client_data;
	return light_lc_client_reg(0, &light_lc_client_model);
}

uint16_t bt_mesh_light_lc_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lc_mode_get_t *lc_mode_get = (rtk_bt_mesh_light_lc_mode_get_t *)p_cmd->param;
		ret = light_lc_mode_get(&light_lc_client_model, lc_mode_get->dst, lc_mode_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_MODE_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lc_mode_set_t *lc_mode_set = (rtk_bt_mesh_light_lc_mode_set_t *)p_cmd->param;
		ret = light_lc_mode_set(&light_lc_client_model, lc_mode_set->dst, lc_mode_set->app_key_index, lc_mode_set->mode, lc_mode_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lc_om_get_t *lc_om_get = (rtk_bt_mesh_light_lc_om_get_t *)p_cmd->param;
		ret = light_lc_om_get(&light_lc_client_model, lc_om_get->dst, lc_om_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_OM_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lc_om_set_t *lc_om_set = (rtk_bt_mesh_light_lc_om_set_t *)p_cmd->param;
		ret = light_lc_om_set(&light_lc_client_model, lc_om_set->dst, lc_om_set->app_key_index, lc_om_set->mode, lc_om_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lc_lightonoff_get_t *lc_lightonoff_get = (rtk_bt_mesh_light_lc_lightonoff_get_t *)p_cmd->param;
		ret = light_lc_light_on_off_get(&light_lc_client_model, lc_lightonoff_get->dst, lc_lightonoff_get->app_key_index);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_LIGHTONOFF_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lc_lightonoff_set_t *lc_lightonoff_set = (rtk_bt_mesh_light_lc_lightonoff_set_t *)p_cmd->param;
		generic_transition_time_t time;
		time.num_steps = (lc_lightonoff_set->trans_time).num_steps;
		time.step_resolution = (lc_lightonoff_set->trans_time).step_resolution;
		ret = light_lc_light_on_off_set(&light_lc_client_model, lc_lightonoff_set->dst, lc_lightonoff_set->app_key_index,
										lc_lightonoff_set->light_on_off, lc_lightonoff_set->tid, lc_lightonoff_set->optional,
										time, lc_lightonoff_set->delay, lc_lightonoff_set->ack);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_GET: {
		rtk_bt_mesh_light_lc_property_get_t *lc_property_get = (rtk_bt_mesh_light_lc_property_get_t *)p_cmd->param;
		ret = light_lc_property_get(&light_lc_client_model, lc_property_get->dst, lc_property_get->app_key_index, lc_property_get->property_id);
		break;
	}
	case RTK_BT_MESH_LIGHT_LC_PROPERTY_CLIENT_MODEL_SET: {
		rtk_bt_mesh_light_lc_property_set_t *lc_property_set = (rtk_bt_mesh_light_lc_property_set_t *)p_cmd->param;
		ret = light_lc_property_set(&light_lc_client_model, lc_property_set->dst, lc_property_set->app_key_index, lc_property_set->property_id,
									(uint8_t *) & (lc_property_set->pvalue), lc_property_set->value_len, lc_property_set->ack);
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

#endif  // BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL
static mesh_model_info_t light_lightness_server_model;

static rtk_bt_mesh_light_lightness_server_get_t lightness_store_get;
static rtk_bt_mesh_light_lightness_server_get_t linear_store_get;
static rtk_bt_mesh_light_lightness_server_get_t lightness_default_store_get;
static rtk_bt_mesh_light_lightness_server_get_t last_store_get;
static rtk_bt_mesh_light_lightness_server_get_range_t lightness_range_store_get;

static int32_t light_lightness_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_LIGHTNESS_SERVER_GET: {
		rtk_bt_mesh_light_lightness_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lightness_server_direct_get_t *lightness_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_GET,
										sizeof(rtk_bt_mesh_light_lightness_server_direct_get_t));
			lightness_get = (rtk_bt_mesh_light_lightness_server_direct_get_t *)p_evt->data;
			lightness_get->value = &lightness_store_get;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = lightness_store_get.lightness;
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_GET_LINEAR: {
		rtk_bt_mesh_light_lightness_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lightness_server_direct_get_t *linear_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_SERVER_MODEL_GET,
										sizeof(rtk_bt_mesh_light_lightness_server_direct_get_t));
			linear_get = (rtk_bt_mesh_light_lightness_server_direct_get_t *)p_evt->data;
			linear_get->value = &linear_store_get;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = linear_store_get.lightness;
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_GET_DEFAULT: {
		rtk_bt_mesh_light_lightness_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lightness_server_direct_get_t *default_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_SERVER_MODEL_GET,
										sizeof(rtk_bt_mesh_light_lightness_server_direct_get_t));
			default_get = (rtk_bt_mesh_light_lightness_server_direct_get_t *)p_evt->data;
			default_get->value = &lightness_default_store_get;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = lightness_default_store_get.lightness;
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_GET_LAST: {
		rtk_bt_mesh_light_lightness_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lightness_server_direct_get_t *last_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_SERVER_MODEL_GET,
										sizeof(rtk_bt_mesh_light_lightness_server_direct_get_t));
			last_get = (rtk_bt_mesh_light_lightness_server_direct_get_t *)p_evt->data;
			last_get->value = &last_store_get;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = last_store_get.lightness;
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_GET_RANGE: {
		rtk_bt_mesh_light_lightness_server_get_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lightness_server_direct_get_range_t *range_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_SERVER_MODEL_GET,
										sizeof(rtk_bt_mesh_light_lightness_server_direct_get_range_t));
			range_get = (rtk_bt_mesh_light_lightness_server_direct_get_range_t *)p_evt->data;
			range_get->value = &lightness_range_store_get;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->range_min = lightness_range_store_get.range_min;
			p_get_data->range_max = lightness_range_store_get.range_max;
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_SET: {
		light_lightness_server_set_t *p_set_data = pargs;
		uint8_t cb_ret = 0;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_SERVER_MODEL_SET, 
										sizeof(light_lightness_server_set_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lightness_server_set_t));
			rtk_bt_evt_indicate(p_evt, &cb_ret);
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_SET_LINEAR: {
		light_lightness_server_set_t *p_set_data = pargs;
		uint8_t cb_ret = 0;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_LINEAR_SERVER_MODEL_SET,
										sizeof(light_lightness_server_set_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lightness_server_set_t));
			rtk_bt_evt_indicate(p_evt, &cb_ret);
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_SET_LAST: {
		light_lightness_server_get_t *p_set_data = pargs;
		uint8_t cb_ret = 0;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_LAST_SERVER_MODEL_SET,
										sizeof(light_lightness_server_get_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lightness_server_get_t));
			rtk_bt_evt_indicate(p_evt, &cb_ret);
		}
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return 0;
}

bool light_lightness_server_model_init(void)
{
	light_lightness_server_model.model_data_cb = light_lightness_server_data;
	return light_lightness_server_reg(0, &light_lightness_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL
static mesh_model_info_t light_lightness_setup_server_model;

static int32_t light_lightness_setup_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case LIGHT_LIGHTNESS_SERVER_SET_DEFAULT: {
		light_lightness_server_set_default_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_DEFAULT_SERVER_MODEL_SET,
										sizeof(light_lightness_server_set_default_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lightness_server_set_default_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_LIGHTNESS_SERVER_SET_RANGE: {
		light_lightness_server_set_range_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_LIGHTNESS_RANGE_SERVER_MODEL_SET,
										sizeof(light_lightness_server_set_range_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lightness_server_set_range_t));
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

bool light_lightness_setup_server_model_init(void)
{
	light_lightness_setup_server_model.model_data_cb = light_lightness_setup_server_data;
	return light_lightness_setup_server_reg(0, &light_lightness_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL
static mesh_model_info_t light_ctl_server_model;

static rtk_bt_mesh_light_ctl_server_get_t ctl_store_get;
static rtk_bt_mesh_light_ctl_server_get_default_t ctl_store_default;
static rtk_bt_mesh_light_ctl_server_get_temperature_range_t ctl_store_range;

static int32_t light_ctl_server_data(const mesh_model_info_p pmodel_info,
									 uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_CTL_SERVER_GET: {
		rtk_bt_mesh_light_ctl_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_ctl_server_direct_get_t *lightness_ctl_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_ctl_server_direct_get_t));
			lightness_ctl_get = (rtk_bt_mesh_light_ctl_server_direct_get_t *)p_evt->data;
			lightness_ctl_get->value = &ctl_store_get;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = ctl_store_get.lightness;
			p_get_data->temperature = ctl_store_get.temperature;
		}
	}
	break;
	case LIGHT_CTL_SERVER_GET_DEFAULT: {
		rtk_bt_mesh_light_ctl_server_get_default_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_ctl_server_direct_get_default_t *default_ctl_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_DEFAULT_GET,
										 sizeof(rtk_bt_mesh_light_ctl_server_direct_get_default_t));
			default_ctl_get = (rtk_bt_mesh_light_ctl_server_direct_get_default_t *)p_evt->data;
			default_ctl_get->value = &ctl_store_default;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = ctl_store_default.lightness;
			p_get_data->temperature = ctl_store_default.temperature;
			p_get_data->delta_uv = ctl_store_default.delta_uv;
		}
	}
	break;
	case LIGHT_CTL_SERVER_GET_TEMPERATURE_RANGE: {
		rtk_bt_mesh_light_ctl_server_get_temperature_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *range_ctl_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET,
										sizeof(rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t));
			range_ctl_get = (rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *)p_evt->data;
			range_ctl_get->value = &ctl_store_range;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->range_max = ctl_store_range.range_max;
			p_get_data->range_min = ctl_store_range.range_min;
		}
	}
	break;
	case LIGHT_CTL_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	case LIGHT_CTL_SERVER_SET: {
		light_ctl_server_set_t *p_get_data = pargs;
		if (p_get_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_SET, 
										sizeof(light_ctl_server_set_t));
			memcpy(p_evt->data, p_get_data, sizeof(light_ctl_server_set_t));
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

bool light_ctl_server_model_init(void)
{
	light_ctl_server_model.model_data_cb = light_ctl_server_data;
	return light_ctl_server_reg(0, &light_ctl_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_CTL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_SETUP_SERVER_MODEL
static mesh_model_info_t light_ctl_setup_server_model;

static int32_t light_ctl_setup_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_CTL_SERVER_SET_TEMPERATURE_RANGE: {
		light_ctl_server_set_temperature_range_t *p_set_data = (light_ctl_server_set_temperature_range_t *)pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_SET,
										sizeof(light_ctl_server_set_temperature_range_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_ctl_server_set_temperature_range_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_CTL_SERVER_SET_DEFAULT: {
		light_ctl_server_set_default_t *p_set_data = (light_ctl_server_set_default_t *)pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_DEFAULT_SET,
										sizeof(light_ctl_server_set_default_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_ctl_server_set_default_t));
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

bool light_ctl_setup_server_model_init(void)
{
	light_ctl_setup_server_model.model_data_cb = light_ctl_setup_server_data;
	return light_ctl_setup_server_reg(0, &light_ctl_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_CTL_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL
static mesh_model_info_t light_ctl_temperature_server_model;

static rtk_bt_mesh_light_ctl_server_get_temperature_t tempearture_store;
static rtk_bt_mesh_light_ctl_server_get_temperature_range_t tempearture_range_store;

static int32_t light_ctl_temperature_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_CTL_SERVER_GET_TEMPERATURE: {
		rtk_bt_mesh_light_ctl_server_get_temperature_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_ctl_server_direct_get_temperature_t *ctl_temperature_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_GET,
										sizeof(rtk_bt_mesh_light_ctl_server_direct_get_temperature_t));
			ctl_temperature_get = (rtk_bt_mesh_light_ctl_server_direct_get_temperature_t *)p_evt->data;
			ctl_temperature_get->value = &tempearture_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->temperature = tempearture_store.temperature;
			p_get_data->delta_uv = tempearture_store.delta_uv;
		}
	}
	break;
	case LIGHT_CTL_SERVER_SET_TEMPERATURE: {
		light_ctl_server_set_temperature_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_SET,
										sizeof(light_ctl_server_set_temperature_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_ctl_server_set_temperature_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_CTL_SERVER_GET_TEMPERATURE_RANGE: { //get from light ctl server model,but set in ctl setup server model
		rtk_bt_mesh_light_ctl_server_get_temperature_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *range_temperature_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL, RTK_BT_MESH_LIGHT_CTL_SERVER_MODEL_TEMPERATURE_RANGE_GET_T,
										sizeof(rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t));
			range_temperature_get = (rtk_bt_mesh_light_ctl_server_direct_get_temperature_range_t *)p_evt->data;
			range_temperature_get->value = &tempearture_range_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->range_max = tempearture_range_store.range_max;
			p_get_data->range_min = tempearture_range_store.range_min;
		}
	}
	break;
	case LIGHT_CTL_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool light_ctl_temperature_server_model_init(void)
{
	light_ctl_temperature_server_model.model_data_cb = light_ctl_temperature_server_data;
	return light_ctl_temperature_server_reg(0, &light_ctl_temperature_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL
static mesh_model_info_t light_hsl_server_model;

static rtk_bt_mesh_light_hsl_server_get_t hsl_store;
static rtk_bt_mesh_light_hsl_server_get_default_t hsl_default_store;
static rtk_bt_mesh_light_hsl_server_get_range_t hsl_range_store;

static int32_t light_hsl_server_data(const mesh_model_info_p pmodel_info,
									 uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_HSL_SERVER_GET: {
		rtk_bt_mesh_light_hsl_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_hsl_server_direct_get_t *hsl_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_t));
			hsl_get = (rtk_bt_mesh_light_hsl_server_direct_get_t *)p_evt->data;
			hsl_get->value = &hsl_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->lightness = hsl_store.lightness;
			p_get_data->hue = hsl_store.hue;
			p_get_data->saturation = hsl_store.saturation;
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_DEFAULT: {
		rtk_bt_mesh_light_hsl_server_get_default_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_evt_t *p_evt = NULL;
			rtk_bt_mesh_light_hsl_server_direct_get_default_t *hsl_default;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_DEFAULT_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_default_t));
			hsl_default = (rtk_bt_mesh_light_hsl_server_direct_get_default_t *)p_evt->data;
			hsl_default->value = &hsl_default_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->hue = hsl_default_store.hue;
			p_get_data->lightness = hsl_default_store.lightness;
			p_get_data->saturation = hsl_default_store.saturation;
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_RANGE: {
		rtk_bt_mesh_light_hsl_server_get_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_hsl_server_direct_get_range_t *hsl_range;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_range_t));
			hsl_range = (rtk_bt_mesh_light_hsl_server_direct_get_range_t *)p_evt->data;
			hsl_range->value = &hsl_range_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->hue_range_min = hsl_range_store.hue_range_min;
			p_get_data->hue_range_max = hsl_range_store.hue_range_max;
			p_get_data->saturation_range_min = hsl_range_store.saturation_range_min;
			p_get_data->saturation_range_max = hsl_range_store.saturation_range_max;
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	case LIGHT_HSL_SERVER_SET: {
		light_hsl_server_set_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_SERVER_MODEL_SET, 
										sizeof(light_hsl_server_set_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_hsl_server_set_t));
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

bool light_hsl_server_model_init(void)
{
	light_hsl_server_model.model_data_cb = light_hsl_server_data;
	return light_hsl_server_reg(0, &light_hsl_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL
static mesh_model_info_t light_hsl_hue_server_model;

static rtk_bt_mesh_light_hsl_server_get_hue_t hue_store;
static rtk_bt_mesh_light_hsl_server_get_range_t hue_range_store;

static int32_t light_hsl_hue_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_HSL_SERVER_GET_HUE: {
		rtk_bt_mesh_light_hsl_server_get_hue_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_hsl_server_direct_get_hue_t *hue_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_hue_t));
			hue_get = (rtk_bt_mesh_light_hsl_server_direct_get_hue_t *)p_evt->data;
			hue_get->value = &hue_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->hue = hue_store.hue;
		}
	}
	break;
	case LIGHT_HSL_SERVER_SET_HUE: {
		light_hsl_server_set_hue_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_HUE_SERVER_MODEL_SET, 
			sizeof(light_hsl_server_set_hue_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_hsl_server_set_hue_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_RANGE: {
		rtk_bt_mesh_light_hsl_server_get_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_hsl_server_direct_get_range_t *hue_range;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_H, 
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_range_t));
			hue_range = (rtk_bt_mesh_light_hsl_server_direct_get_range_t *)p_evt->data;
			hue_range->value = &hue_range_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->hue_range_min = hue_range_store.hue_range_min;
			p_get_data->hue_range_max = hue_range_store.hue_range_max;
			p_get_data->saturation_range_min = hue_range_store.saturation_range_min;
			p_get_data->saturation_range_max = hue_range_store.saturation_range_max;
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}
	return 0;
}

bool light_hsl_hue_server_model_init(void)
{
	light_hsl_hue_server_model.model_data_cb = light_hsl_hue_server_data;
	return light_hsl_hue_server_reg(0, &light_hsl_hue_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL
static mesh_model_info_t light_hsl_saturation_server_model;

static rtk_bt_mesh_light_hsl_server_get_saturation_t saturation_store;
static rtk_bt_mesh_light_hsl_server_get_range_t saturation_range_store;

static int32_t light_hsl_saturation_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_HSL_SERVER_GET_SATURATION: {
		rtk_bt_mesh_light_hsl_server_get_saturation_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_hsl_server_direct_get_saturation_t *saturation_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_GET,
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_saturation_t));
			saturation_get = (rtk_bt_mesh_light_hsl_server_direct_get_saturation_t *)p_evt->data;
			saturation_get->value = &saturation_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->saturation = saturation_store.saturation;
		}
	}
	break;
	case LIGHT_HSL_SERVER_SET_SATURATION: {
		light_hsl_server_set_saturation_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL_SET,
										sizeof(light_hsl_server_set_saturation_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_hsl_server_set_saturation_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_RANGE: {
		rtk_bt_mesh_light_hsl_server_get_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_hsl_server_direct_get_range_t *saturation_range;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_GET_S,
										sizeof(rtk_bt_mesh_light_hsl_server_direct_get_range_t));
			saturation_range = (rtk_bt_mesh_light_hsl_server_direct_get_range_t *)p_evt->data;
			saturation_range->value = &saturation_range_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->hue_range_min = saturation_range_store.hue_range_min;
			p_get_data->hue_range_max = saturation_range_store.hue_range_max;
			p_get_data->saturation_range_min = saturation_range_store.saturation_range_min;
			p_get_data->saturation_range_max = saturation_range_store.saturation_range_max;
		}
	}
	break;
	case LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return 0;
}

bool light_hsl_saturation_server_model_init(void)
{
	light_hsl_saturation_server_model.model_data_cb = light_hsl_saturation_server_data;
	return light_hsl_saturation_server_reg(0, &light_hsl_saturation_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_HSL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_HSL_SETUP_SERVER_MODEL
static mesh_model_info_t light_hsl_setup_server_model;

static int32_t light_hsl_setup_server_data(const mesh_model_info_p pmodel_info,
		uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_HSL_SERVER_SET_DEFAULT: {
		light_hsl_server_set_default_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_DEFAULT_SERVER_MODEL_SET,
										sizeof(light_hsl_server_set_default_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_hsl_server_set_default_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_HSL_SERVER_SET_RANGE: {
		light_hsl_server_set_range_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_HSL_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_HSL_RANGE_SERVER_MODEL_SET, 
										sizeof(light_hsl_server_set_range_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_hsl_server_set_range_t));
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

bool light_hsl_setup_server_model_init(void)
{
	light_hsl_setup_server_model.model_data_cb = light_hsl_setup_server_data;
	return light_hsl_setup_server_reg(0, &light_hsl_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_HSL_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL
static mesh_model_info_t light_xyl_server_model;

static rtk_bt_mesh_light_xyl_server_get_t xyl_store;
static rtk_bt_mesh_light_xyl_server_get_default_t xyl_default_store;
static rtk_bt_mesh_light_xyl_server_get_range_t xyl_range_store;

static int32_t light_xyl_server_data(const mesh_model_info_p pmodel_info,
									 uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_XYL_SERVER_GET: {
		rtk_bt_mesh_light_xyl_server_get_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_xyl_server_direct_get_t *xyl_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL, RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_xyl_server_direct_get_t));
			xyl_get = (rtk_bt_mesh_light_xyl_server_direct_get_t *)p_evt->data;
			xyl_get->value = &xyl_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->xyl_lightness = xyl_store.xyl_lightness;
			p_get_data->xyl_x = xyl_store.xyl_x;
			p_get_data->xyl_y = xyl_store.xyl_y;
		}
	}
	break;
	case LIGHT_XYL_SERVER_GET_DEFAULT: {
		rtk_bt_mesh_light_xyl_server_get_default_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_xyl_server_direct_get_default_t *xyl_default;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL, RTK_BT_MESH_LIGHT_XYL_DEFAULT_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_xyl_server_direct_get_default_t));
			xyl_default = (rtk_bt_mesh_light_xyl_server_direct_get_default_t *)p_evt->data;
			xyl_default->value = &xyl_default_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->xyl_lightness = xyl_default_store.xyl_lightness;
			p_get_data->xyl_x = xyl_default_store.xyl_x;
			p_get_data->xyl_y = xyl_default_store.xyl_y;
		}
	}
	break;
	case LIGHT_XYL_SERVER_GET_RANGE: {
		rtk_bt_mesh_light_xyl_server_get_range_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_xyl_server_direct_get_range_t *xyl_range;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL, RTK_BT_MESH_LIGHT_XYL_RANGE_SERVER_MODEL_GET, 
										sizeof(rtk_bt_mesh_light_xyl_server_direct_get_range_t));
			xyl_range = (rtk_bt_mesh_light_xyl_server_direct_get_range_t *)p_evt->data;
			xyl_range->value = &xyl_range_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->xyl_x_range_max = xyl_range_store.xyl_x_range_max;
			p_get_data->xyl_x_range_min = xyl_range_store.xyl_x_range_min;
			p_get_data->xyl_y_range_max = xyl_range_store.xyl_y_range_max;
			p_get_data->xyl_y_range_min = xyl_range_store.xyl_y_range_min;
		}
	}
	break;
	case LIGHT_XYL_SERVER_GET_DEFAULT_TRANSITION_TIME: {
	}
	break;
	case LIGHT_XYL_SERVER_SET: {
		light_xyl_server_set_t *p_get_data = pargs;
		if (p_get_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL, RTK_BT_MESH_LIGHT_XYL_SERVER_MODEL_SET, 
										sizeof(light_xyl_server_set_t));
			memcpy(p_evt->data, p_get_data, sizeof(light_xyl_server_set_t));
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

bool light_xyl_server_model_init(void)
{
	light_xyl_server_model.model_data_cb = light_xyl_server_data;
	return light_xyl_server_reg(0, &light_xyl_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_XYL_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_XYL_SETUP_SERVER_MODEL
static mesh_model_info_t light_xyl_setup_server_model;

static int32_t light_xyl_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_XYL_SERVER_SET_DEFAULT: {
		light_xyl_server_set_default_t *p_set_data = NULL;
		p_set_data = (light_xyl_server_set_default_t *)pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_XYL_DEFAULT_SERVER_MODEL_SET,
										sizeof(light_xyl_server_set_default_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_xyl_server_set_default_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_XYL_SERVER_SET_RANGE: {
		light_xyl_server_set_range_t *p_set_data = NULL;
		p_set_data = (light_xyl_server_set_range_t *)pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_XYL_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_XYL_RANGE_SERVER_MODEL_SET, 
										sizeof(light_xyl_server_set_range_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_xyl_server_set_range_t));
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

bool light_xyl_setup_server_model_init(void)
{
	light_xyl_setup_server_model.model_data_cb = light_xyl_setup_server_data;
	return light_xyl_setup_server_reg(0, &light_xyl_setup_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_XYL_SETUP_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL
static mesh_model_info_t light_lc_server_model;

static rtk_bt_mesh_light_lc_server_get_mode_t mode_store;
static rtk_bt_mesh_light_lc_server_get_om_t om_store;
static rtk_bt_mesh_light_lc_server_get_light_on_off_t onoff_store;
static rtk_bt_mesh_light_lc_server_get_sm_transition_time_t sm_store;

static int32_t light_lc_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_LC_SERVER_GET_MODE: {
		rtk_bt_mesh_light_lc_server_get_mode_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lc_server_direct_get_mode_t *mode_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_MODE_GET, 
										sizeof(rtk_bt_mesh_light_lc_server_direct_get_mode_t));
			mode_get = (rtk_bt_mesh_light_lc_server_direct_get_mode_t *)p_evt->data;
			mode_get->value = &mode_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->mode = mode_store.mode;
		}
	}
	break;
	case LIGHT_LC_SERVER_SET_MODE: {
		light_lc_server_set_mode_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_MODE_SET, 
										sizeof(light_lc_server_set_mode_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lc_server_set_mode_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_LC_SERVER_GET_OM: {
		rtk_bt_mesh_light_lc_server_get_om_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lc_server_direct_get_om_t *om_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_OM_GET, 
										sizeof(rtk_bt_mesh_light_lc_server_direct_get_om_t));
			om_get = (rtk_bt_mesh_light_lc_server_direct_get_om_t *)p_evt->data;
			om_get->value = &om_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->mode = om_store.mode;
		}
	}
	break;
	case LIGHT_LC_SERVER_SET_OM: {
		light_lc_server_set_om_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_OM_SET, 
										sizeof(light_lc_server_set_om_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lc_server_set_om_t));
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}
	break;
	case LIGHT_LC_SERVER_GET_LIGHT_ON_OFF: {
		rtk_bt_mesh_light_lc_server_get_light_on_off_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t *onoff_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_LIGHT_ON_OFF_GET,
										sizeof(rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t));
			onoff_get = (rtk_bt_mesh_light_lc_server_direct_get_light_on_off_t *)p_evt->data;
			onoff_get->value = &onoff_store;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->on_off = onoff_store.on_off;
		}
	}
	break;
	case LIGHT_LC_SERVER_GET_DEFAULT_TRANSITION_TIME: {

	}
	break;
	case LIGHT_LC_SERVER_GET_SM_TRANSITION_TIME: {
		rtk_bt_mesh_light_lc_server_get_sm_transition_time_t *p_get_data = pargs;
		if (p_get_data) {
			rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t *sm_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_SM_TRANSITION_TIME_GET,
										sizeof(rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t));
			sm_get = (rtk_bt_mesh_light_lc_server_direct_get_sm_transition_time_t *)p_evt->data;
			sm_get->value = &sm_store;
			rtk_bt_evt_indicate(p_evt, NULL);
			p_get_data->light_on_off = sm_store.light_on_off;
			p_get_data->sm_trans_time = sm_store.sm_trans_time;
		}
	}
	break;
	case LIGHT_LC_SERVER_SET_LIGHT_ON_OFF: {
		light_lc_server_set_light_on_off_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_LIGHT_ON_OFF_SET,
										sizeof(light_lc_server_set_light_on_off_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lc_server_set_light_on_off_t));
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

bool light_lc_server_model_init(void)
{
	light_lc_server_model.model_data_cb = light_lc_server_data;
	return light_lc_server_reg(0, &light_lc_server_model);
}
#endif // end of BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL

#if defined(BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL
static mesh_model_info_t light_lc_setup_server_model;

static rtk_bt_mesh_light_lc_server_direct_get_property_para_t property_store;

static int32_t light_lc_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	/* avoid gcc compile warning */
	UNUSED(pmodel_info);

	switch (type) {
	case LIGHT_LC_SERVER_GET_PROPERTY: {
		rtk_bt_mesh_light_lc_server_get_property_t *p_get_data = pargs;
		uint8_t cb_ret = 0;
		if (p_get_data) {
			rtk_bt_mesh_light_lc_server_direct_get_property_t *property_get;
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_PROPERTY_GET,
										sizeof(rtk_bt_mesh_light_lc_server_direct_get_property_t));
			property_get = (rtk_bt_mesh_light_lc_server_direct_get_property_t *)p_evt->data;
			property_get->value = &property_store;
			property_get->property_id = p_get_data->property_id;
			rtk_bt_evt_indicate(p_evt, &cb_ret);
			p_get_data->property_value = property_store.property_value;
			p_get_data->value_len = property_store.value_len;
		}
	}
	break;
	case LIGHT_LC_SERVER_SET_PROPERTY: {
		light_lc_server_set_property_t *p_set_data = pargs;
		if (p_set_data) {
			rtk_bt_evt_t *p_evt = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL, RTK_BT_MESH_LIGHT_LC_SERVER_MODEL_PROPERTY_SET,
										sizeof(light_lc_server_set_property_t));
			memcpy(p_evt->data, p_set_data, sizeof(light_lc_server_set_property_t));
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

bool light_lc_setup_server_model_init(void)
{
	light_lc_setup_server_model.model_data_cb = light_lc_setup_server_data;
	return light_lc_setup_server_reg(0, &light_lc_setup_server_model);
}
#endif // end if BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT
