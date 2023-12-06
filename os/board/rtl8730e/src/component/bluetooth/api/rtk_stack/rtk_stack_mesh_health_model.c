#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>

#include <rtk_bt_mesh_health_model.h>
#include <rtk_bt_mesh_def.h>
#include <platform_os.h>


#include <health.h>

#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
static mesh_model_info_t health_client_model;

static int32_t health_client_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	UNUSED(pmodel_info);
	switch (type) {
	case HEALTH_CLIENT_STATUS_ATTENTION: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_CLIENT_MODEL, RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_STATUS,
									sizeof(rtk_bt_mesh_health_client_status_attention_t));
		memcpy(p_evt->data, pargs, sizeof(rtk_bt_mesh_health_client_status_attention_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
    case HEALTH_CLIENT_STATUS_CURRENT: {
		rtk_bt_evt_t *p_evt = NULL;
        health_client_status_t *pdata = (health_client_status_t *)pargs;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_CLIENT_MODEL, RTK_BT_MESH_HEALTH_CLIENT_MODEL_CURT_STATUS,
									sizeof(health_client_status_t) + pdata->fault_array_len);
		memcpy(p_evt->data, pargs, 5);
        memcpy((uint8_t *)p_evt->data + 5, & pdata->fault_array_len, 1);
        memcpy((uint8_t *)p_evt->data + 6, pdata->fault_array, pdata->fault_array_len);
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
    case HEALTH_CLIENT_STATUS_REGISTERED: {
		rtk_bt_evt_t *p_evt = NULL;
        health_client_status_t *pdata = (health_client_status_t *)pargs;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_CLIENT_MODEL, RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_STATUS,
									sizeof(health_client_status_t) + pdata->fault_array_len);
		memcpy(p_evt->data, pargs, 5);
        memcpy((uint8_t *)p_evt->data + 5, & pdata->fault_array_len, 1);
        memcpy((uint8_t *)p_evt->data + 6, pdata->fault_array, pdata->fault_array_len);
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
    case HEALTH_CLIENT_STATUS_PERIOD: {
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_CLIENT_MODEL, RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_STATUS,
									sizeof(health_client_status_period_t));
		memcpy(p_evt->data, pargs, sizeof(health_client_status_period_t));
		rtk_bt_evt_indicate(p_evt, NULL);
	}
	break;
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return 0;
}

bool health_client_model_init(void)
{
	health_client_model.model_data_cb = health_client_data;
	return health_client_reg(0, &health_client_model);
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_health_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_GET: {
		rtk_bt_mesh_health_fault_get_t *fault_get = (rtk_bt_mesh_health_fault_get_t *)p_cmd->param;
		ret = health_fault_get(&health_client_model, fault_get->dst, fault_get->app_key_index, fault_get->company_id);
		break;
	}
	case RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_CLEAR: {
		rtk_bt_mesh_health_fault_clear_t *fault_clear = (rtk_bt_mesh_health_fault_clear_t *)p_cmd->param;
		ret = health_fault_clear(&health_client_model, fault_clear->dst, fault_clear->app_key_index, fault_clear->company_id, fault_clear->ack);
		break;
	}
    case RTK_BT_MESH_HEALTH_CLIENT_MODEL_FAULT_TEST: {
		rtk_bt_mesh_health_fault_test_t *fault_test = (rtk_bt_mesh_health_fault_test_t *)p_cmd->param;
		ret = health_fault_test(&health_client_model, fault_test->dst, fault_test->app_key_index, fault_test->test_id, fault_test->company_id, fault_test->ack);
		break;
	}
    case RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_GET: {
		rtk_bt_mesh_health_period_get_t *period_get = (rtk_bt_mesh_health_period_get_t *)p_cmd->param;
		ret = health_period_get(&health_client_model, period_get->dst, period_get->app_key_index);
		break;
	}
    case RTK_BT_MESH_HEALTH_CLIENT_MODEL_PERIOD_SET: {
		rtk_bt_mesh_health_period_set_t *period_set = (rtk_bt_mesh_health_period_set_t *)p_cmd->param;
		ret = health_period_set(&health_client_model, period_set->dst, period_set->app_key_index, period_set->fast_period_divisor, period_set->ack);
		break;
	}
    case RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_GET: {
		rtk_bt_mesh_health_attn_get_t *attn_get = (rtk_bt_mesh_health_attn_get_t *)p_cmd->param;
		ret = health_attn_get(&health_client_model, attn_get->dst, attn_get->app_key_index);
		break;
	}
    case RTK_BT_MESH_HEALTH_CLIENT_MODEL_ATTN_SET: {
		rtk_bt_mesh_health_attn_set_t *attn_set = (rtk_bt_mesh_health_attn_set_t *)p_cmd->param;
		ret = health_attn_set(&health_client_model, attn_set->dst, attn_set->app_key_index, attn_set->attn, attn_set->ack);
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
#endif // end of RTK_BLE_MESH_PROVISIONER_SUPPORT

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT

static mesh_model_info_t health_server_model;

uint8_t fault_store[HEALTH_FAULT_ARRAY_LEN];

static int32_t health_server_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
    /* avoid gcc compile warning */
    UNUSED(pmodel_info);

    switch (type)
    {
    case HEALTH_SERVER_GET_FAULT:
        {
            rtk_bt_mesh_health_server_fault_get_t *p_get_data = NULL;
            p_get_data = (rtk_bt_mesh_health_server_fault_get_t *)pargs;
            uint8_t cb_ret = 0;
            if (p_get_data) {
                rtk_bt_mesh_health_server_fault_get_t *fault_get;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_FAULT_GET, 
                                            sizeof(rtk_bt_mesh_health_server_fault_get_t) + HEALTH_FAULT_ARRAY_LEN);
                fault_get = (rtk_bt_mesh_health_server_fault_get_t *)p_evt->data;
                fault_get->company_id = p_get_data->company_id;
                fault_get->fault_array = fault_store;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                p_get_data->fault_array = fault_store;//reserve one byte to store length
            }
        }
        break;
    case HEALTH_SERVER_CLEAR_FAULT:
        {
            rtk_bt_mesh_health_server_fault_clear_t *p_get_data = NULL;
            p_get_data = (rtk_bt_mesh_health_server_fault_clear_t *)pargs;
            uint8_t cb_ret = 0;
            if (p_get_data) {
                rtk_bt_mesh_health_server_fault_clear_t *fault_clear;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_FAULT_CLEAR, 
                                            sizeof(rtk_bt_mesh_health_server_fault_clear_t) + HEALTH_FAULT_ARRAY_LEN);
                fault_clear = (rtk_bt_mesh_health_server_fault_clear_t *)p_evt->data;
                fault_clear->company_id = p_get_data->company_id;
                fault_clear->fault_array = fault_store;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                p_get_data->fault_array = fault_store;//reserve one byte to store length
            }
        }
        break;
    case HEALTH_SERVER_TEST_FAULT:
        {
            rtk_bt_mesh_health_server_fault_test_t *p_get_data = NULL;
            p_get_data = (rtk_bt_mesh_health_server_fault_test_t *)pargs;
            uint8_t cb_ret = 0;
            if (p_get_data) {
                rtk_bt_mesh_health_server_fault_test_t *fault_test;
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_FAULT_TEST, 
                                            sizeof(rtk_bt_mesh_health_server_fault_test_t) + HEALTH_FAULT_ARRAY_LEN);
                fault_test = (rtk_bt_mesh_health_server_fault_test_t *)p_evt->data;
                fault_test->company_id = p_get_data->company_id;
                fault_test->test_id = p_get_data->test_id;
                fault_test->fault_array = fault_store;
                rtk_bt_evt_indicate(p_evt, &cb_ret);
                p_get_data->fault_array = fault_store;//reserve one byte to store length
            }
        }
        break;
    case HEALTH_SERVER_GET_PERIOD:
        {
            rtk_bt_mesh_health_server_period_get_t *period_get;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_PERIOD_GET,
                                        sizeof(rtk_bt_mesh_health_server_period_get_t));
            period_get = (rtk_bt_mesh_health_server_period_get_t *)p_evt->data;
            period_get->fast_period_divisor = &(((health_server_period_t *)pargs)->fast_period_divisor);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case HEALTH_SERVER_SET_PERIOD:
        {
            health_server_period_t *p_set_data = NULL;
            p_set_data = (health_server_period_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_PERIOD_SET, sizeof(health_server_period_t));
                memcpy(p_evt->data, p_set_data, sizeof(health_server_period_t));
                rtk_bt_evt_indicate(p_evt, NULL);
            }
        }
        break;
    case HEALTH_SERVER_GET_ATTENTION:
        {
            rtk_bt_mesh_health_server_attn_get_t *attn_get;
            rtk_bt_evt_t *p_evt = NULL;
            p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_ATTN_GET,
                                        sizeof(rtk_bt_mesh_health_server_attn_get_t));
            attn_get = (rtk_bt_mesh_health_server_attn_get_t *)p_evt->data;
            attn_get->attn = &(((health_server_attn_t *)pargs)->attn);
            rtk_bt_evt_indicate(p_evt, NULL);
        }
        break;
    case HEALTH_SERVER_SET_ATTENTION:
        {
            health_server_attn_t *p_set_data = NULL;
            p_set_data = (health_server_attn_t *)pargs;
            if (p_set_data) {
                rtk_bt_evt_t *p_evt = NULL;
                p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL, RTK_BT_MESH_HEALTH_SERVER_MODEL_ATTN_SET, sizeof(health_server_attn_t));
                memcpy(p_evt->data, p_set_data, sizeof(health_server_attn_t));
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

bool health_server_model_init(void){
    bool ret = 0;
    health_server_model.model_data_cb = health_server_data;
    ret = health_server_reg(0,&health_server_model);
    if(!ret){
        return ret;
    }
    health_server_set_company_id(&health_server_model, COMPANY_ID);
    //set fault here
    return ret;
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_health_server_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_FAULT_REGISTER: {
		rtk_bt_mesh_health_server_fault_register_act_t *fault_register = (rtk_bt_mesh_health_server_fault_register_act_t *)p_cmd->param;
		health_server_fault_register(&health_server_model, fault_register->fault);
        ret = RTK_BT_OK;
		break;
	}
    case RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CURRENT_FAULT_CLEAR: {
		rtk_bt_mesh_health_server_fault_clear_act_t *fault_clear = (rtk_bt_mesh_health_server_fault_clear_act_t *)p_cmd->param;
		health_server_fault_clear(&health_server_model, fault_clear->fault);
        ret = RTK_BT_OK;
		break;
	}
    case RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CURRENT_FAULT_CLEAR_ALL: {
		health_server_fault_clear_all(&health_server_model);
        ret = RTK_BT_OK;
		break;
	}
    case RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_FAULT_COUNT: {
        rtk_bt_mesh_health_server_count_fault_act_t *fault_count = (rtk_bt_mesh_health_server_count_fault_act_t *)p_cmd->param;
		fault_count->fault_count = health_server_fault_count(&health_server_model);
        ret = RTK_BT_OK;
		break;
	}
    case RTK_BT_MESH_HEALTH_SERVER_MODEL_ACT_CHECK_FAULT_IS_SET: {
		rtk_bt_mesh_health_server_check_fault_act_t *fault_check = (rtk_bt_mesh_health_server_check_fault_act_t *)p_cmd->param;
		fault_check->set = health_server_fault_is_set(&health_server_model, fault_check->fault);
        ret = RTK_BT_OK;
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
#endif // end of RTK_BLE_MESH_DEVICE_SUPPORT
#endif // end of RTK_BLE_MESH_SUPPORT