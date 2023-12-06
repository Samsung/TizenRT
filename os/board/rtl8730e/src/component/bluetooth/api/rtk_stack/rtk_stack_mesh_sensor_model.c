#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_sensor_model.h>
#include <sensor.h>

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
#if defined(BT_MESH_ENABLE_SENSOR_CLIENT_MODEL) && BT_MESH_ENABLE_SENSOR_CLIENT_MODEL
#define TRIGGER_DELTA_UNITLESS_LEN 2
static mesh_model_info_t sensor_client_model;

static int32_t sensor_client_data(const mesh_model_info_p pmodel_info,
                                           uint32_t type, void *pargs)
{
    UNUSED(pmodel_info);
    switch (type)
    {
    case SENSOR_CLIENT_STATUS_DESCRIPTOR:
        {
            rtk_bt_evt_t *p_evt = NULL;
            sensor_client_status_descriptor_t *pdata = (sensor_client_status_descriptor_t *) pargs;
            uint32_t len = 6 + pdata->num_descriptors * sizeof(sensor_descriptor_t);
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_DESCRIPTOR_CLIENT_MODEL_STATUS,
									len);
		    memcpy(p_evt->data, pargs, 6);
            memcpy((uint8_t *)p_evt->data + 6, pdata->descriptors, pdata->num_descriptors * sizeof(sensor_descriptor_t));
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case SENSOR_CLIENT_STATUS_CADENCE:
        {
            rtk_bt_evt_t *p_evt = NULL;
            bool flag = 0;
            sensor_client_status_cadence_t *pdata = (sensor_client_status_cadence_t *) pargs;
            if (pdata->cadence != NULL)
            {
                uint8_t status_trigger_type = pdata->cadence->status_trigger_type;
                uint8_t trigger_len = 0;
                uint8_t raw_value_len = pdata->cadence->raw_value_len;
                flag = 1;
                if (SENSOR_TRIGGER_TYPE_CHARACTERISTIC == status_trigger_type)
                {
                    trigger_len = pdata->cadence->raw_value_len;
                }
                else
                {
                    trigger_len = TRIGGER_DELTA_UNITLESS_LEN;
                }
                uint32_t len = 9 + 4 * pdata->cadence->raw_value_len;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_STATUS,
                                        len);
                memcpy(p_evt->data, & pdata->src, 2);
                memcpy((uint8_t *)p_evt->data + 2, & pdata->property_id, 2);
                memcpy((bool *)p_evt->data + 4, &flag, 1);
                memcpy((uint8_t *)p_evt->data + 5, & pdata->cadence->raw_value_len, 2);
                memcpy((uint8_t *)p_evt->data + 7, pdata->cadence->status_trigger_delta_down, trigger_len);
                memcpy((uint8_t *)p_evt->data + 7 + trigger_len, pdata->cadence->status_trigger_delta_up, trigger_len);
                memcpy((uint8_t *)p_evt->data + 7 + 2 * trigger_len, &pdata->cadence->status_min_interval, 1);
                memcpy((uint8_t *)p_evt->data + 8 + 2 * trigger_len, pdata->cadence->fast_cadence_low, raw_value_len);
                memcpy((uint8_t *)p_evt->data + 8 + 2 * trigger_len + raw_value_len, pdata->cadence->fast_cadence_high, raw_value_len);
                rtk_bt_evt_indicate(p_evt, NULL);
            } else {
                uint32_t len = 5;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_STATUS,
                                        len);
                memcpy(p_evt->data, & pdata->src, 2);
                memcpy((uint8_t *)p_evt->data + 2, & pdata->property_id, 2);
                memcpy((bool *)p_evt->data + 4, &flag, 1);
                rtk_bt_evt_indicate(p_evt, NULL);
            } 
        }
        break;
    case SENSOR_CLIENT_STATUS_SETTINGS:
        {
            rtk_bt_evt_t *p_evt = NULL;
            sensor_client_status_settings_t *pdata = (sensor_client_status_settings_t *) pargs;
            uint32_t len = pdata->num_ids * 2 + 6;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_SETTINGS_CLIENT_MODEL_STATUS,
									len);
		    memcpy(p_evt->data, pargs, 6);
            memcpy((uint8_t *)p_evt->data + 6, pdata->setting_ids, len - 6);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case SENSOR_CLIENT_STATUS_SETTING:
        {
            rtk_bt_evt_t *p_evt = NULL;
            bool flag = 0;
            uint32_t len = 0;
            sensor_client_status_setting_t *pdata = (sensor_client_status_setting_t *) pargs;
            if (pdata->setting != NULL)
            {
                len = 11 + pdata->setting->setting_raw_len;
                flag = 1;
            } else {
                len = 7;
            }
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_STATUS,
									len);
		    memcpy(p_evt->data, pdata, 6);
            memcpy((bool *)p_evt->data + 6, &flag, 1);
            if (pdata->setting != NULL)
            {
                memcpy((uint8_t *)p_evt->data + 7, &pdata->setting->setting_property_id, 4);
		        memcpy((uint8_t *)p_evt->data + 11, pdata->setting->setting_raw, pdata->setting->setting_raw_len);
            }
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case SENSOR_CLIENT_STATUS:
        {
            rtk_bt_evt_t *p_evt = NULL;
            sensor_client_status_t *pdata = (sensor_client_status_t *) pargs;
            uint32_t len = pdata->marshalled_sensor_data_len + 4;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_CLIENT_MODEL_STATUS,
									len);
		    memcpy(p_evt->data, pargs, 4);
            memcpy((uint8_t *)p_evt->data + 4, pdata->marshalled_sensor_data, len - 4);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;    
    case SENSOR_CLIENT_STATUS_COLUMN:
        {
            rtk_bt_evt_t *p_evt = NULL;
            sensor_client_status_column_t *pdata = (sensor_client_status_column_t *) pargs;
            uint32_t len = pdata->column_raw_value_len + 6;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_COLUMN_CLIENT_MODEL_STATUS,
									len);
		    memcpy(p_evt->data, pargs, 6);
            memcpy((uint8_t *)p_evt->data + 6, pdata->column_raw_value, len - 6);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case SENSOR_CLIENT_STATUS_SERIES:
        {
            rtk_bt_evt_t *p_evt = NULL;
            sensor_client_status_series_t *pdata = (sensor_client_status_series_t *) pargs;
            uint32_t len = pdata->series_raw_value_len + 6;
		    p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_CLIENT_MODEL, RTK_BT_MESH_SENSOR_SERIES_CLIENT_MODEL_STATUS,
									len);
		    memcpy(p_evt->data, pargs, 6);
            memcpy((uint8_t *)p_evt->data + 6, pdata->series_raw_value, len - 6);
		    rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    
    return 0;    
}   

bool sensor_client_model_init(void)
{
	sensor_client_model.model_data_cb = sensor_client_data;
	return sensor_client_reg(0, &sensor_client_model);
}

uint16_t bt_mesh_sensor_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
    case RTK_BT_MESH_SENSOR_DESCRIPTOR_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_descriptor_get_t *sensor_des_get = (rtk_bt_mesh_sensor_descriptor_get_t *)p_cmd->param;
		ret = sensor_descriptor_get(&sensor_client_model, sensor_des_get->dst, sensor_des_get->app_key_index,
                                    sensor_des_get->property_id);
		break;
	}
    case RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_cadence_get_t *sensor_cad_get = (rtk_bt_mesh_sensor_cadence_get_t *)p_cmd->param;
		ret = sensor_cadence_get(&sensor_client_model, sensor_cad_get->dst, sensor_cad_get->app_key_index,
                                sensor_cad_get->property_id);
		break;
	}
    case RTK_BT_MESH_SENSOR_CADENCE_CLIENT_MODEL_SET: {
		rtk_bt_mesh_sensor_cadence_set_t *sensor_cad_set = (rtk_bt_mesh_sensor_cadence_set_t *)p_cmd->param;
        sensor_cadence_t cadence;
        cadence.fast_cadence_period_divisor = sensor_cad_set->fast_cadence_period_divisor;
        cadence.status_trigger_type = sensor_cad_set->status_trigger_type;
        cadence.raw_value_len = sensor_cad_set->raw_value_len;
        cadence.status_trigger_delta_down = sensor_cad_set->trigger_down;
        cadence.status_trigger_delta_up = sensor_cad_set->trigger_up;
        cadence.status_min_interval = sensor_cad_set->status_min_interval;
        cadence.fast_cadence_low = sensor_cad_set->cadence_low;
        cadence.fast_cadence_high = sensor_cad_set->cadence_high;
		ret = sensor_cadence_set(&sensor_client_model, sensor_cad_set->dst, sensor_cad_set->app_key_index,
                                sensor_cad_set->property_id, &cadence, sensor_cad_set->ack);
		break;
	}
    case RTK_BT_MESH_SENSOR_SETTINGS_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_settings_get_t *sensor_sets_get = (rtk_bt_mesh_sensor_settings_get_t *)p_cmd->param;
		ret = sensor_settings_get(&sensor_client_model, sensor_sets_get->dst, sensor_sets_get->app_key_index,
                                sensor_sets_get->property_id);
		break;
	}
    case RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_setting_get_t *sensor_set_get = (rtk_bt_mesh_sensor_setting_get_t *)p_cmd->param;
		ret = sensor_setting_get(&sensor_client_model, sensor_set_get->dst, sensor_set_get->app_key_index,
                                sensor_set_get->property_id, sensor_set_get->setting_property_id);
		break;
	}
    case RTK_BT_MESH_SENSOR_SETTING_CLIENT_MODEL_SET: {
		rtk_bt_mesh_sensor_setting_set_t *sensor_set_set = (rtk_bt_mesh_sensor_setting_set_t *)p_cmd->param;
		ret = sensor_setting_set(&sensor_client_model, sensor_set_set->dst, sensor_set_set->app_key_index,
                                sensor_set_set->property_id, sensor_set_set->setting_property_id,
                                sensor_set_set->setting_raw_len, (void *)&sensor_set_set->setting_raw, 
                                sensor_set_set->ack);
		break;
	}
    case RTK_BT_MESH_SENSOR_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_get_t *sensor_g = (rtk_bt_mesh_sensor_get_t *)p_cmd->param;
		ret = sensor_get(&sensor_client_model, sensor_g->dst, sensor_g->app_key_index, sensor_g->property_id);
		break;
	}
    case RTK_BT_MESH_SENSOR_COLUMN_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_column_get_t *sensor_col_get = (rtk_bt_mesh_sensor_column_get_t *)p_cmd->param;
		ret = sensor_column_get(&sensor_client_model, sensor_col_get->dst, sensor_col_get->app_key_index,
                                sensor_col_get->property_id, sensor_col_get->raw_value_x_len,
                                (void *)&sensor_col_get->raw_value_x);
		break;
	}
    case RTK_BT_MESH_SENSOR_SERIES_CLIENT_MODEL_GET: {
		rtk_bt_mesh_sensor_series_get_t *sensor_ser_get = (rtk_bt_mesh_sensor_series_get_t *)p_cmd->param;
		ret = sensor_series_get(&sensor_client_model, sensor_ser_get->dst, sensor_ser_get->app_key_index,
                         sensor_ser_get->property_id, sensor_ser_get->raw_value_x_len,
                         (void *)&sensor_ser_get->raw_value_x1, (void *)&sensor_ser_get->raw_value_x2);
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


#endif  // BT_MESH_ENABLE_SENSOR_CLIENT_MODEL


#if ((defined(BT_MESH_ENABLE_SENSOR_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SERVER_MODEL) || \
    (defined(BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL))
#define TRIGGER_DELTA_UNITLESS_LEN 2

mesh_model_info_t sensor_server_model;
/***********Setup database for test***********/
sensor_descriptor_t descriptor1 = {1,10,10,1,1,1};
sensor_descriptor_t descriptor2 = {2,5,5,1,1,1};
uint8_t sensor_raw_data_len = 5;//the array length in mesh_device.c is 5
uint8_t setting_raw1[] = {1, 2, 3, 4, 5};
uint8_t setting_raw2[] = {5, 4, 3, 2, 1};
sensor_setting_t settings[]={
    {1,3,5,setting_raw1},
    {2,3,5,setting_raw2}
};

uint8_t setting_raw3[] = {1, 1, 1, 1, 1};
uint8_t setting_raw4[] = {2, 2, 2, 2, 2};
sensor_setting_t settings2[]={
    {1,3,5,setting_raw3},
    {2,3,5,setting_raw4}
};
uint8_t delta_down_db[] = {3,2,1,2,3};
uint8_t delta_up_db[] =  {1,2,3,2,1};
uint8_t cadence_low_db[] = {4,3,2,3,4};
uint8_t cadence_high_db[] = {2,3,4,3,2};
uint8_t delta_down_db2[] = {3,3,3,3,3};
uint8_t delta_up_db2[] =  {4,4,4,4,4};
uint8_t cadence_low_db2[] = {3,3,3,3,3};
uint8_t cadence_high_db2[] = {4,4,4,4,4};
sensor_cadence_t cadence_db[]={{5,1,0,delta_down_db,delta_up_db,1,cadence_low_db,cadence_high_db}};
sensor_cadence_t cadence_db2[]={{5,1,0,delta_down_db2,delta_up_db2,1,cadence_low_db2,cadence_high_db2}};
int32_t pub_count_db = 0;

static sensor_db_t sensor_db[] = {
    {{1,10,10,1,1,1}, 5, 2, settings, cadence_db, 0},
    {{2,5,5,1,1,1}, 5, 2, settings2, cadence_db2, 0}
};
#endif
/********database setup finished********/

#if defined(BT_MESH_ENABLE_SENSOR_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SERVER_MODEL
uint8_t sensor_get_value[SENSOR_GET_DATA_MAX_LEN * 2];
uint8_t sensor_column_get_value[SENSOR_GET_COLUMN_MAX_LEN * 2];
uint8_t sensor_series_get_value[SENSOR_GET_SERIES_MAX_LEN * 2];
sensor_descriptor_t sensor_descriptors[SENSOR_GET_DATA_MAX_LEN];
static int32_t sensor_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
    case SENSOR_SERVER_GET:{
        rtk_bt_mesh_sensor_server_get_t *p_get_data = pargs;
        uint8_t cb_ret = 0;
        if (p_get_data)
            {
                /**
                 * if needed,customer can indicate setting information by creating event
                 * customer should know the length of information when indicationg app
                 * if have not only one pointer, use one pointer to put all data in it
                 * then analyze in stack layer
                */
                rtk_bt_mesh_sensor_server_get_t *sensor_value;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_GET,
									sizeof(rtk_bt_mesh_sensor_server_get_t) + SENSOR_GET_DATA_MAX_LEN + 2);
                sensor_value = (rtk_bt_mesh_sensor_server_get_t *)p_evt->data;
                sensor_value->property_id = p_get_data->property_id;
                sensor_value->raw_data = sensor_get_value;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                p_get_data->raw_data = &sensor_get_value[2];//reserve uint16_t to store data len
            }
        break;
    }
    case SENSOR_SERVER_GET_COLUMN: {
        rtk_bt_mesh_sensor_server_get_column_t *p_get_data = pargs;
        uint8_t cb_ret = 0;
        if (p_get_data)
            {
                rtk_bt_mesh_sensor_server_direct_get_column_t *column_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_COLUMN_GET,
									sizeof(rtk_bt_mesh_sensor_server_direct_get_column_t) + p_get_data->raw_value_x_len + 4 + SENSOR_GET_COLUMN_MAX_LEN);
                column_get = (rtk_bt_mesh_sensor_server_direct_get_column_t *)p_evt->data;
                column_get->property_id = p_get_data->property_id;
                column_get->raw_value_x_len = p_get_data->raw_value_x_len;
                memcpy(column_get->raw_value_x, p_get_data->raw_value_x, p_get_data->raw_value_x_len);
                column_get->value = sensor_column_get_value;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                p_get_data->column_len = LE_EXTRN2WORD(sensor_column_get_value);
                p_get_data->column = (uint8_t *)sensor_column_get_value + 2;
            }
        break;
    }
    case SENSOR_SERVER_GET_SERIES: {
        rtk_bt_mesh_sensor_server_get_series_t *p_get_data = pargs;
        uint8_t cb_ret = 0;
        if (p_get_data)
            {
                rtk_bt_mesh_sensor_server_direct_get_series_t *series_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_SERIES_GET,
									sizeof(rtk_bt_mesh_sensor_server_direct_get_series_t) + 2 * SENSOR_GET_SERIES_MAX_LEN + 2);
                series_get = (rtk_bt_mesh_sensor_server_direct_get_series_t *)p_evt->data;
                series_get->property_id = p_get_data->property_id;
                series_get->raw_value_x_len = p_get_data->raw_value_x_len;
                memcpy(series_get->raw_value_x1, p_get_data->raw_value_x1, p_get_data->raw_value_x_len);
                memcpy(series_get->raw_value_x2, p_get_data->raw_value_x2, p_get_data->raw_value_x_len);
                series_get->value = sensor_series_get_value;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                p_get_data->series_len = LE_EXTRN2WORD(sensor_series_get_value);
                p_get_data->series = (uint8_t *)sensor_series_get_value + 2;
            }
        break;
    }
    case SENSOR_SERVER_COMPARE_CADENCE: {
        rtk_bt_mesh_sensor_server_compare_cadence_t *p_get_data;
        uint8_t cb_ret = 0;
        rtk_bt_evt_t *p_evt = NULL;
        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_COMPARE_CADENCE,
                            sizeof(rtk_bt_mesh_sensor_server_compare_cadence_t));
        p_get_data = (rtk_bt_mesh_sensor_server_compare_cadence_t *)p_evt->data;
        p_get_data->property_id = ((sensor_server_compare_cadence_t *)pargs)->property_id;
        p_get_data->need_fast_divisor = &(((sensor_server_compare_cadence_t *)pargs)->need_fast_divisor);
        rtk_bt_evt_indicate(p_evt, &cb_ret);
        break;
    }
    case SENSOR_SERVER_GET_DESCRIPTOR_NUM: {
        rtk_bt_mesh_sensor_server_get_descriptor_num_t *p_get_data;
        sensor_server_get_descriptor_num_t *descriptor_num_get = pargs;
        uint8_t cb_ret = 0;
        rtk_bt_evt_t *p_evt = NULL;
        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_NUM_GET,
                            sizeof(rtk_bt_mesh_sensor_server_get_descriptor_num_t));
        p_get_data = (rtk_bt_mesh_sensor_server_get_descriptor_num_t *)p_evt->data;
        p_get_data->property_id = descriptor_num_get->property_id;
        p_get_data->descriptor_num = &descriptor_num_get->descriptor_num;
        rtk_bt_evt_indicate(p_evt, &cb_ret);
        break;
    }
    case SENSOR_SERVER_GET_DESCRIPTOR: {
        rtk_bt_mesh_sensor_server_get_descriptor_t *p_get_data;
        sensor_server_get_descriptor_t *descriptor_get = pargs;
        uint8_t cb_ret = 0;
        rtk_bt_evt_t *p_evt = NULL;
        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_DESCRIPTOR_GET,
                            sizeof(rtk_bt_mesh_sensor_server_get_descriptor_t) + descriptor_get->descriptor_num * sizeof(rtk_bt_mesh_sensor_descriptor_t));
        p_get_data = (rtk_bt_mesh_sensor_server_get_descriptor_t *)p_evt->data;
        p_get_data->property_id = descriptor_get->property_id;
        p_get_data->descriptor_num = descriptor_get->descriptor_num;
        p_get_data->descriptor =(rtk_bt_mesh_sensor_descriptor_t *) &sensor_descriptors[0];
        rtk_bt_evt_indicate(p_evt, &cb_ret);
        descriptor_get->descriptor = sensor_descriptors;
        break;
    }
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool sensor_server_model_init(void)
{
    bool ret = 0;
    sensor_server_model.model_data_cb = sensor_server_data;
    ret = sensor_server_reg(0,&sensor_server_model);
    if (ret)
    {
        sensor_server_set_db(&sensor_server_model, sensor_db, sizeof(sensor_db)/sizeof(sensor_db_t));
        return ret;
    }
    return ret;
}

uint16_t bt_mesh_sensor_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_SENSOR_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_sensor_server_db_set_t *db_get = (rtk_bt_mesh_sensor_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using sensor_server_set_db *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        sensor_server_set_db(&sensor_server_model, sensor_db, sizeof(sensor_db)/sizeof(sensor_db_t));
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
#endif // end of BT_MESH_ENABLE_SENSOR_SERVER_MODEL

#if defined(BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL) && BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL
mesh_model_info_t sensor_setup_server_model;

sensor_server_set_cadence_t             sensor_cadence_buffer = {0};
sensor_server_set_setting_t             sensor_setting_buffer = {0};
uint8_t cadence_value[SENSOR_CADENCE_DATA_MAX_LEN * 4] = {0};
uint16_t settings_store[SENSOR_SETTINGS_DATA_MAX_LEN];
uint8_t setting_store[SENSOR_SETTING_DATA_MAX_LEN];
sensor_cadence_t cadence_transfer;
/*Customer can use link list to store data in database and search data using property_id*/
static int32_t sensor_setup_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    (void) pmodel_info;

    switch (type)
    {
    case SENSOR_SERVER_SET_CADENCE:
        {
            sensor_server_set_cadence_t *p_set_data = pargs;
            if (p_set_data)
            {
                /**
                 * if needed,customer can indicate setting information by creating event
                 * customer should know the length of information when indicationg app
                */
                uint8_t trigger_len;
                uint8_t raw_value_len = (p_set_data->cadence)->raw_value_len;
                if (SENSOR_TRIGGER_TYPE_CHARACTERISTIC == (p_set_data->cadence)->status_trigger_type)
                {
                    trigger_len = (p_set_data->cadence)->raw_value_len;
                }
                else
                {
                    trigger_len = TRIGGER_DELTA_UNITLESS_LEN;
                }
                uint32_t len = (p_set_data->cadence)->raw_value_len * 2 + trigger_len * 2 + 5;
                rtk_bt_evt_t *p_evt = NULL;
		        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_CADENCE_SET,
									len);
		        memcpy(p_evt->data, pargs, 2);
                memcpy((uint8_t *)(p_evt->data) + 2, p_set_data->cadence, 2);
                memcpy((uint8_t *)(p_evt->data) + 4, (p_set_data->cadence)->status_trigger_delta_down, trigger_len);
                memcpy((uint8_t *)(p_evt->data) + 4 + trigger_len, (p_set_data->cadence)->status_trigger_delta_up, trigger_len);
                memcpy((uint8_t *)(p_evt->data) + 4 + 2 * trigger_len, &(p_set_data->cadence)->status_min_interval, 1);
                memcpy((uint8_t *)(p_evt->data) + 5 + 2 * trigger_len, (p_set_data->cadence)->fast_cadence_low, raw_value_len);
                memcpy((uint8_t *)(p_evt->data) + 5 + 2 * trigger_len + raw_value_len, (p_set_data->cadence)->fast_cadence_high, raw_value_len);
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case SENSOR_SERVER_SET_SETTING:
        {
            sensor_server_set_setting_t *p_set_data = pargs;
            if (p_set_data)
            {
                /**
                 * if needed,customer can indicate setting information by creating event
                 * customer should know the length of information when indicationg app
                */
                uint32_t len = (p_set_data->setting)->setting_raw_len + 6;
                rtk_bt_evt_t *p_evt = NULL;
		        p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTING_SET,
									len);
                memcpy(p_evt->data, &p_set_data->property_id, 2);
                memcpy((uint8_t *)(p_evt->data) + 2, &(p_set_data->setting->setting_property_id), 2);
                memcpy((uint8_t *)(p_evt->data) + 4, &(p_set_data->setting->setting_access), 1);
                memcpy((uint8_t *)(p_evt->data) + 5, &(p_set_data->setting->setting_raw_len), 1);
                memcpy((uint8_t *)(p_evt->data) + 6, p_set_data->setting->setting_raw, (p_set_data->setting)->setting_raw_len);
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case SENSOR_SERVER_GET_CADENCE:
        {
            sensor_server_get_cadence_t *p_data = pargs;
            rtk_bt_mesh_sensor_server_get_cadence_t *p_get_data;
            uint8_t cb_ret = 0;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_CADENCE_GET,
                                sizeof(rtk_bt_mesh_sensor_server_get_cadence_t) + 4*SENSOR_CADENCE_DATA_MAX_LEN);
            p_get_data = (rtk_bt_mesh_sensor_server_get_cadence_t *)p_evt->data;
            p_get_data->property_id = p_data->property_id;
            p_get_data->cadence = cadence_value;
            rtk_bt_evt_indicate(p_evt, &cb_ret);
            // uint16_t len = MESH_LE_EXTRN2WORD(&cadence_value[0]);
            /****dump cadence data to p_data*****/
            /*reserve 2 bytes to store msg length for ipc and direct calling*/
            if (MESH_LE_EXTRN2WORD(&cadence_value[0]) != 0) {
                cadence_transfer.raw_value_len = cadence_value[2];
                cadence_transfer.fast_cadence_period_divisor = cadence_value[3] & 0x7F;
                cadence_transfer.status_trigger_type = (cadence_value[3] & 0x80) >> 7;
                cadence_transfer.status_trigger_delta_down = &cadence_value[4];
                uint16_t trigger_len = 0;
                if (cadence_transfer.status_trigger_type)
                {
                    trigger_len = 2;
                } else {
                    trigger_len = cadence_transfer.raw_value_len;
                }
                cadence_transfer.status_trigger_delta_up = &cadence_value[4 + trigger_len];
                cadence_transfer.status_min_interval = cadence_value[4 + 2* trigger_len];
                cadence_transfer.fast_cadence_low = &cadence_value[5 + 2* trigger_len];
                cadence_transfer.fast_cadence_high = &cadence_value[5 + 2 * trigger_len + cadence_value[2]];
                p_data->cadence = &cadence_transfer;
            } else{
                p_data->cadence = NULL;
            }
            
        }
        break;
    case SENSOR_SERVER_GET_SETTINGS:
        {
            sensor_server_get_settings_t *p_data = pargs;
            rtk_bt_mesh_sensor_server_get_settings_t *p_get_data;
            uint8_t cb_ret = 0;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTINGS_GET,
                                sizeof(rtk_bt_mesh_sensor_server_get_settings_t) + SENSOR_SETTINGS_DATA_MAX_LEN);
            p_get_data = (rtk_bt_mesh_sensor_server_get_settings_t *)p_evt->data;
            p_get_data->property_id = p_data->property_id;
            p_get_data->settings_data = settings_store;
            rtk_bt_evt_indicate(p_evt, &cb_ret);
            p_data->settings_data = settings_store;
        }
        break;
    case SENSOR_SERVER_GET_SETTING:
        {
            sensor_server_get_setting_t *p_data = pargs;
            rtk_bt_mesh_sensor_server_get_setting_t *p_get_data;
            uint8_t cb_ret = 0;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL, RTK_BT_MESH_SENSOR_SERVER_MODEL_SETTING_GET,
                                sizeof(rtk_bt_mesh_sensor_server_get_setting_t) + SENSOR_SETTING_DATA_MAX_LEN);
            p_get_data = (rtk_bt_mesh_sensor_server_get_setting_t *)p_evt->data;
            p_get_data->property_id = p_data->property_id;
            p_get_data->setting_property_id = p_data->setting->setting_property_id;
            p_get_data->setting_data = setting_store;
            rtk_bt_evt_indicate(p_evt, &cb_ret);
            p_data->setting->setting_access = setting_store[0];
            p_data->setting->setting_raw_len = setting_store[1];
            if (p_data->setting->setting_raw_len == 0)
            {
                p_data->setting->setting_raw = NULL;
            } else {
                p_data->setting->setting_raw = &setting_store[2];
            }
        }
        break;
    default:
        printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
        break;
    }
    return 0;
}

bool sensor_setup_server_model_init(void)
{
    bool ret = 0;
    sensor_setup_server_model.model_data_cb = sensor_setup_server_data;
    ret = sensor_setup_server_reg(0,&sensor_setup_server_model);
    if (ret)
    {
        sensor_setup_server_set_db(&sensor_setup_server_model, sensor_db, sizeof(sensor_db)/sizeof(sensor_db_t));
        return ret;
    }
    return ret;
}

uint16_t bt_mesh_sensor_setup_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
    uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
    switch (p_cmd->act) {
        case RTK_BT_MESH_SENSOR_SETUP_SERVER_MODEL_ACT_DB_SET: {
		// rtk_bt_mesh_sensor_server_db_set_t *db_get = (rtk_bt_mesh_sensor_server_db_set_t *)p_cmd->param;
        /*****  customer need to parse data here , fill the database and then set the database using sensor_setup_server_set_db *****/
        // printf("0x");
        // for (int i = 0; i < db_get->value_len; i++)
        // {
        //     printf("%x",db_get->p_data[i]);
        // }
        // printf("\r\n");
        sensor_setup_server_set_db(&sensor_setup_server_model, sensor_db, sizeof(sensor_db)/sizeof(sensor_db_t));
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

#endif // end of BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT