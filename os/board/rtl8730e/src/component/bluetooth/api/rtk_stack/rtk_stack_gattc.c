/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <dlist.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gattc.h>
#include <rtk_bt_gatts.h>
#include <rtk_stack_config.h>
#include <rtk_stack_gatt.h>
#include <rtk_stack_internal.h>
#include <gap_conn_le.h>
extern struct amebad2_uart_t *amebad2_uart;

#if RTK_BLE_MGR_LIB
#include <bt_gatt_client.h>

#define SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(index)														\
static T_APP_RESULT _spec_srv_callback##index(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)	\
{																										\
	return bt_stack_gattc_common_callback(conn_handle, type, p_data, index);							\
}

#define SPECIFIC_SERVICE_CALLBACK_POINTER(index)			_spec_srv_callback##index

#define MAX_SPEC_SERVICE_NUM	16

typedef struct {
	uint16_t id;
	P_FUN_GATT_CLIENT_CB cb;
} _srv_cb_t;

_srv_cb_t bt_statck_srv_cb[MAX_SPEC_SERVICE_NUM] = {0};

T_APP_RESULT bt_stack_gattc_common_callback(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data, uint8_t index)
{
	rtk_bt_evt_t *p_evt = NULL;
	uint16_t profile_id = bt_statck_srv_cb[index].id;
	T_APP_RESULT ret = APP_RESULT_SUCCESS;

	switch (type) {
	case GATT_CLIENT_EVENT_DIS_DONE: {
		rtk_bt_gattc_discover_ind_t *p_disc_ind = NULL;
		T_GATT_CLIENT_DIS_DONE *p_disc = (T_GATT_CLIENT_DIS_DONE *)p_data;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND,
									sizeof(rtk_bt_gattc_discover_ind_t));
		if (p_evt) {
			p_disc_ind = (rtk_bt_gattc_discover_ind_t *)p_evt->data;
			p_disc_ind->profile_id = profile_id;
			p_disc_ind->conn_handle = conn_handle;
			p_disc_ind->is_found = p_disc->is_found;
			p_disc_ind->load_from_storage = p_disc->load_from_ftl;
			p_disc_ind->srv_instance_num = p_disc->srv_instance_num;
		}
		break;
	}

	case GATT_CLIENT_EVENT_READ_RESULT: {
		rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
		T_GATT_CLIENT_READ_RESULT *p_result = (T_GATT_CLIENT_READ_RESULT *)p_data;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t) + p_result->value_size);
		if (p_evt) {
			p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
			p_read_ind->profile_id = profile_id;
			p_read_ind->conn_handle = conn_handle;
			p_read_ind->char_type = (rtk_bt_gattc_char_type_t)p_result->char_type;
			p_read_ind->srv_instance_id = p_result->srv_instance_id;
			memcpy(&p_read_ind->char_uuid, &p_result->char_uuid, sizeof(rtk_bt_gattc_uuid_t));
			p_read_ind->handle = p_result->handle;
			p_read_ind->len = p_result->value_size;
			p_read_ind->value = (uint8_t *)p_evt->data + sizeof(rtk_bt_gattc_read_ind_t);
			memcpy(p_read_ind->value, p_result->p_value, p_result->value_size);
			p_read_ind->err_code = p_result->cause;
			if (p_read_ind->err_code) {
				p_read_ind->status = RTK_BT_STATUS_FAIL;
			} else {
				p_read_ind->status = RTK_BT_STATUS_DONE;
			}
		}
		break;
	}

	case GATT_CLIENT_EVENT_WRITE_RESULT: {
		rtk_bt_gattc_write_ind_t *p_write_ind = NULL;
		T_GATT_CLIENT_WRITE_RESULT *p_result = (T_GATT_CLIENT_WRITE_RESULT *)p_data;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_WRITE_RESULT_IND,
									sizeof(rtk_bt_gattc_write_ind_t));
		if (p_evt) {
			p_write_ind = (rtk_bt_gattc_write_ind_t *)p_evt->data;
			p_write_ind->profile_id = profile_id;
			p_write_ind->conn_handle = conn_handle;
			p_write_ind->char_type = (rtk_bt_gattc_char_type_t)p_result->char_type;
			p_write_ind->srv_instance_id = p_result->srv_instance_id;
			memcpy(&p_write_ind->char_uuid, &p_result->char_uuid, sizeof(rtk_bt_gattc_uuid_t));
			p_write_ind->type = (rtk_bt_gattc_write_type_t)p_result->type;
			p_write_ind->handle = p_result->handle;
			p_write_ind->err_code = p_result->cause;
			if (p_write_ind->err_code) {
				p_write_ind->status = RTK_BT_STATUS_FAIL;
			} else {
				p_write_ind->status = RTK_BT_STATUS_DONE;
			}
		}
		break;
	}

	case GATT_CLIENT_EVENT_NOTIFY_IND: {
		T_GATT_CLIENT_NOTIFY_IND *p_notify = (T_GATT_CLIENT_NOTIFY_IND *)p_data;
		rtk_bt_gattc_cccd_value_ind_t *p_ind = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									p_notify->notify ? RTK_BT_GATTC_EVT_NOTIFY_IND : RTK_BT_GATTC_EVT_INDICATE_IND,
									sizeof(rtk_bt_gattc_cccd_value_ind_t) + p_notify->value_size);

		if (p_evt) {
			p_ind = (rtk_bt_gattc_cccd_value_ind_t *)p_evt->data;
			p_ind->profile_id = profile_id;
			p_ind->conn_handle = conn_handle;
			p_ind->cid = p_notify->cid;
			p_ind->srv_instance_id = p_notify->srv_instance_id;
			memcpy(&p_ind->char_uuid, &p_notify->char_uuid, sizeof(rtk_bt_gattc_uuid_t));
			p_ind->value_handle = p_notify->handle;
			p_ind->len = p_notify->value_size;
			p_ind->value = (uint8_t *)p_evt->data + sizeof(rtk_bt_gattc_cccd_value_ind_t);
			memcpy(p_ind->value, p_notify->p_value, p_notify->value_size);
		}
		break;
	}

	case GATT_CLIENT_EVENT_CCCD_CFG: {
		T_GATT_CLIENT_CCCD_CFG *p_cccd = (T_GATT_CLIENT_CCCD_CFG *)p_data;
		rtk_bt_gattc_cccd_update_ind_t *p_cccd_ind = NULL;
		if (p_cccd->cccd_cfg) {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_CCCD_ENABLE_IND,
										sizeof(rtk_bt_gattc_cccd_update_ind_t));
			p_cccd_ind = (rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;
			if (p_cccd->cccd_cfg & GATT_CLIENT_CONFIG_NOTIFICATION) {
				p_cccd_ind->bnotify = true;
			}
			if (p_cccd->cccd_cfg & GATT_CLIENT_CONFIG_INDICATION) {
				p_cccd_ind->bindicate = true;
			}
		} else {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_CCCD_DISABLE_IND,
										sizeof(rtk_bt_gattc_cccd_update_ind_t));
			p_cccd_ind = (rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;
			p_cccd_ind->bnotify = true;
			p_cccd_ind->bindicate = true;
		}
		if (p_cccd_ind) {
			p_cccd_ind->profile_id = profile_id;
			p_cccd_ind->conn_handle = conn_handle;
			p_cccd_ind->srv_instance_id = p_cccd->srv_instance_id;
			memcpy(&p_cccd_ind->uuid, &p_cccd->char_uuid, sizeof(rtk_bt_gattc_uuid_t));
			p_cccd_ind->err_code = p_cccd->cause;
			if (p_cccd_ind->err_code) {
				p_cccd_ind->status = RTK_BT_STATUS_FAIL;
			} else {
				p_cccd_ind->status = RTK_BT_STATUS_DONE;
			}
		}
		break;
	}

	default:
		break;
	}

	if (p_evt) {
		rtk_bt_evt_indicate(p_evt, NULL);
	}

	return ret;
}

SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(0);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(1);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(2);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(3);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(4);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(5);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(6);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(7);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(8);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(9);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(10);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(11);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(12);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(13);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(14);
SPECIFIC_SERVICE_CALLBACK_IMPLEMENT(15);

static P_FUN_GATT_CLIENT_CB _spec_srv_cb_array[16] = {
	SPECIFIC_SERVICE_CALLBACK_POINTER(0),
	SPECIFIC_SERVICE_CALLBACK_POINTER(1),
	SPECIFIC_SERVICE_CALLBACK_POINTER(2),
	SPECIFIC_SERVICE_CALLBACK_POINTER(3),
	SPECIFIC_SERVICE_CALLBACK_POINTER(4),
	SPECIFIC_SERVICE_CALLBACK_POINTER(5),
	SPECIFIC_SERVICE_CALLBACK_POINTER(6),
	SPECIFIC_SERVICE_CALLBACK_POINTER(7),
	SPECIFIC_SERVICE_CALLBACK_POINTER(8),
	SPECIFIC_SERVICE_CALLBACK_POINTER(9),
	SPECIFIC_SERVICE_CALLBACK_POINTER(10),
	SPECIFIC_SERVICE_CALLBACK_POINTER(11),
	SPECIFIC_SERVICE_CALLBACK_POINTER(12),
	SPECIFIC_SERVICE_CALLBACK_POINTER(13),
	SPECIFIC_SERVICE_CALLBACK_POINTER(14),
	SPECIFIC_SERVICE_CALLBACK_POINTER(15)
};

static uint16_t bt_stack_gattc_register_profile(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_gattc_register_t *reg_param = (rtk_bt_gattc_register_t *)param;
	P_FUN_GATT_CLIENT_CB cb;
	int i;

	for (i = 0; i < MAX_SPEC_SERVICE_NUM; i++) {
		if (bt_statck_srv_cb[i].cb == NULL) {
			break;
		}
	}

	if (i == MAX_SPEC_SERVICE_NUM) {
		return RTK_BT_ERR_NO_RESOURCE;
	}

	cb = _spec_srv_cb_array[i];

	cause = gatt_client_spec_register((T_ATTR_UUID *)&reg_param->srv_uuid, cb);
	if (cause) {
		printf("gatt_client_spec_register() cause = %d\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	bt_statck_srv_cb[i].cb = cb;
	bt_statck_srv_cb[i].id = reg_param->profile_id;

	return RTK_BT_OK;
}

T_APP_RESULT bt_stack_gattc_discover_all_callback(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
{
	switch (type) {
	case GATT_CLIENT_EVENT_DIS_ALL_STATE: {
		rtk_bt_gattc_discover_all_state_ind_t *p_disc_ind = NULL;
		T_GATT_CLIENT_DIS_ALL_DONE *p_disc = (T_GATT_CLIENT_DIS_ALL_DONE *)p_data;
		API_PRINT("bt_stack_gattc_discover_all_callback: conn_handle %d, is_success %d, load_from_ftl %d",
				  conn_handle, p_disc->state, p_disc->load_from_ftl);

		if (p_disc->state == GATT_CLIENT_STATE_DONE || p_disc->state == GATT_CLIENT_STATE_FAILED) {
			rtk_bt_evt_t *p_evt;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_DISCOVER_ALL_STATE_IND,
										sizeof(rtk_bt_gattc_discover_all_state_ind_t));
			if (p_evt) {
				p_disc_ind = (rtk_bt_gattc_discover_all_state_ind_t *)p_evt->data;
				p_disc_ind->conn_handle = conn_handle;
				p_disc_ind->is_success = (p_disc->state == GATT_CLIENT_STATE_DONE) ? true : false;
				p_disc_ind->load_from_storage = p_disc->load_from_ftl;

				rtk_bt_evt_indicate(p_evt, NULL);
			}
		}
		break;
	}

	case GATT_CLIENT_EVENT_GATT_SERVICE_INFO: {
		rtk_bt_gattc_gatt_service_info_ind_t *p_ind = NULL;
		T_GATT_CLIENT_GATT_SERVICE_INFO *p_info = (T_GATT_CLIENT_GATT_SERVICE_INFO *)p_data;
		T_GATT_SERVICE_DATA *p_gatt_data = p_info->p_gatt_data;
		rtk_bt_evt_t *p_evt;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_GATT_SERVICE_INFO_IND,
									sizeof(rtk_bt_gattc_gatt_service_info_ind_t));
		if (p_evt) {
			p_ind = (rtk_bt_gattc_gatt_service_info_ind_t *)p_evt->data;
			p_ind->conn_handle = conn_handle;
			p_ind->load_from_storage = p_info->load_from_ftl;
			p_ind->is_found = p_info->is_found;
			if (p_ind->is_found) {
				p_ind->char_flag = p_gatt_data->char_exist;
				memcpy(p_ind->database_hash, p_gatt_data->database_hash, RTK_BT_GATT_SVC_DATABASE_HASH_LEN);
				p_ind->client_features_len = p_gatt_data->client_supported_features_len;
				memcpy(p_ind->client_features, p_gatt_data->client_supported_features, RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_MAX_LEN);
				p_ind->server_features_len = p_gatt_data->server_supported_features_len;
				memcpy(p_ind->server_features, p_gatt_data->server_supported_features, RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_MAX_LEN);
			}
			rtk_bt_evt_indicate(p_evt, NULL);
		}
		break;
	}

	default:
		break;
	}

	return APP_RESULT_SUCCESS;
}

static uint16_t bt_stack_gattc_discover_all(void *param)
{
	T_GAP_CAUSE cause;
	uint16_t conn_handle = (uint16_t)(*((uint16_t *)param));

	cause = gatt_client_start_discovery_all(conn_handle, bt_stack_gattc_discover_all_callback);
	if (cause) {
		printf("gatt_client_start_discovery_all() conn_handle = %d, cause = %d\r\n", conn_handle, cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_find(void *param)
{
	bool ret;
	rtk_bt_gattc_find_param_t *p_find_param = (rtk_bt_gattc_find_param_t *)param;

	if (p_find_param->type == RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE) {
		ret = gatt_client_find_char_handle(p_find_param->conn_handle,
										   (T_ATTR_UUID *)&p_find_param->find_char.srv_uuid,
										   (T_ATTR_UUID *)&p_find_param->find_char.char_uuid,
										   p_find_param->find_char.p_handle);
	} else if (p_find_param->type == RTK_BT_GATT_FIND_CHARACTERISTIC_DESC_RANGE) {
		ret = gatt_client_find_char_descriptor_range(p_find_param->conn_handle,
				(T_ATTR_UUID *)&p_find_param->find_char_range.srv_uuid,
				(T_ATTR_UUID *)&p_find_param->find_char_range.char_uuid,
				p_find_param->find_char_range.p_handle_start,
				p_find_param->find_char_range.p_handle_end);
	} else if (p_find_param->type == RTK_BT_GATT_FIND_CHARACTERISTIC_CCCD_HANDLE) {
		uint8_t cccd_prop;
		ret = gatt_client_find_char_cccd_handle(p_find_param->conn_handle,
												(T_ATTR_UUID *)&p_find_param->find_char_cccd.srv_uuid,
												(T_ATTR_UUID *)&p_find_param->find_char_cccd.char_uuid,
												p_find_param->find_char_cccd.p_handle, &cccd_prop);
		if (ret) {
			*p_find_param->find_char_cccd.p_notify = (cccd_prop & GATT_CLIENT_CONFIG_NOTIFICATION) ? true : false;
			*p_find_param->find_char_cccd.p_indicate = (cccd_prop & GATT_CLIENT_CONFIG_INDICATION) ? true : false;
		}
	} else if (p_find_param->type == RTK_BT_GATT_FIND_INCLUDE_SERVICE) {
		T_ATTR_INSTANCE attr_instance;
		ret = gatt_client_find_include_srv_by_primary(p_find_param->conn_handle,
				(T_ATTR_UUID *)&p_find_param->find_include_srv.primary_uuid,
				(T_ATTR_UUID *)&p_find_param->find_include_srv.include_uuid,
				&attr_instance);
		if (ret) {
			*p_find_param->find_include_srv.p_instance_num = attr_instance.instance_num;
			memcpy(p_find_param->find_include_srv.instance_id, attr_instance.instance_id, sizeof(uint8_t) * ATTR_INSTANCE_NUM_MAX);
		}
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (ret == false) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_read(void *param)
{
	T_GAP_CAUSE cause = 0;
	rtk_bt_gattc_read_param_t *p_read_param = (rtk_bt_gattc_read_param_t *)param;

	if (p_read_param->type == RTK_BT_GATT_CHAR_READ_BY_HANDLE) {
		cause = gatt_client_read(p_read_param->conn_handle, p_read_param->by_handle.handle, NULL);
	} else if (p_read_param->type == RTK_BT_GATT_CHAR_READ_BY_UUID)
		cause = gatt_client_read_uuid(p_read_param->conn_handle, p_read_param->by_uuid.start_handle,
									  p_read_param->by_uuid.end_handle, p_read_param->by_uuid.char_uuid.p.uuid16, NULL);

	if (cause) {
		printf("bt_stack_gattc_read() type = %d, cause = %d\r\n", p_read_param->type, cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_write(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_gattc_write_param_t *p_write_param = (rtk_bt_gattc_write_param_t *)param;
	T_GATT_WRITE_TYPE write_type = 0;

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_REQ) {
		write_type = GATT_WRITE_TYPE_REQ;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP) {
		write_type = GATT_WRITE_TYPE_CMD;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		write_type = GATT_WRITE_TYPE_SIGNED_CMD;
	} else {
		return RTK_BT_ERR_NO_ENTRY;
	}

	cause = gatt_client_write(p_write_param->conn_handle, write_type,
							  p_write_param->handle, p_write_param->length, p_write_param->data, NULL);
	if (cause) {
		printf("bt_stack_gattc_write() cause = %d\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_update_cccd(void *param, bool benable)
{
	T_GAP_CAUSE cause;
	rtk_bt_gattc_update_cccd_param_t *p_cccd_param = (rtk_bt_gattc_update_cccd_param_t *)param;
	uint8_t cccd_cfg = GATT_CLIENT_CONFIG_DISABLE;

	if (benable) {
		if (p_cccd_param->bindicate && !p_cccd_param->bnotify) {
			cccd_cfg = GATT_CLIENT_CONFIG_INDICATION;
		} else if (!p_cccd_param->bindicate && p_cccd_param->bnotify) {
			cccd_cfg = GATT_CLIENT_CONFIG_NOTIFICATION;
		} else {
			cccd_cfg = GATT_CLIENT_CONFIG_ALL;
		}
	}

	if (p_cccd_param->srv_cfg) {
		cause = gatt_client_enable_srv_cccd(p_cccd_param->conn_handle, (T_ATTR_UUID *)&p_cccd_param->srv_uuid, cccd_cfg);
	} else
		cause = gatt_client_enable_char_cccd(p_cccd_param->conn_handle, (T_ATTR_UUID *)&p_cccd_param->srv_uuid,
											 (T_ATTR_UUID *)&p_cccd_param->char_uuid, cccd_cfg);

	if (cause) {
		printf("gatt_client_start_discovery_all() cause = %d\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_stack_gattc_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;

	if (true != bt_stack_profile_check(RTK_BT_PROFILE_GATTC)) {
		printf("Error: Gattc profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}

	switch (p_cmd->act) {
	case RTK_BT_GATTC_ACT_REGISTER_PROFILE:
		ret = bt_stack_gattc_register_profile(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_DISCOVER:
		ret = bt_stack_gattc_discover_all(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_FIND:
		ret = bt_stack_gattc_find(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_READ:
		ret = bt_stack_gattc_read(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_WRITE:
		ret = bt_stack_gattc_write(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_CCCD_ENABLE:
		ret = bt_stack_gattc_update_cccd(p_cmd->param, true);
		break;
	case RTK_BT_GATTC_ACT_CCCD_DISABLE:
		ret = bt_stack_gattc_update_cccd(p_cmd->param, false);
		break;
	default:
		printf("bt_stack_le_act_handle:unknown act: %d \r\n", p_cmd->act);
		ret = RTK_BT_ERR_NO_CASE_ELEMENT;
		break;
	}

end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}

uint16_t bt_stack_gattc_init(rtk_bt_app_conf_t *app_conf)
{
	int i;
	(void)app_conf;
#if GATTC_TBL_STORAGE_SUPPORT
	gattc_tbl_storage_init(); //store peripheral service information to ftl
#endif

#if (defined(RTK_BT_5_2_EATT_SUPPORT) && RTK_BT_5_2_EATT_SUPPORT) && F_BT_5_2_EATT_SUPPORT
	if (!gatt_client_init(GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT |
						  GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT |
						  GATT_CLIENT_DISCOV_MODE_USE_EXT_CLIENT |
						  GATT_CLIENT_DISCOV_MODE_GATT_SVC)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (!gatt_client_cfg_client_supported_feature(GATT_SVC_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#else

	if (!gatt_client_init(GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT |
						  GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	for (i = 0; i < MAX_SPEC_SERVICE_NUM; i++) {
		bt_statck_srv_cb[i].cb = NULL;
	}

	return RTK_BT_OK;
}

void bt_stack_gattc_deinit(void)
{
}

void bt_stack_gattc_evt_indicate_mtu_exchange(uint16_t conn_handle, uint16_t mtu)
{
	rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_exchange_ind = NULL;
	rtk_bt_evt_t *p_evt_t = NULL;

	p_evt_t = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
								  RTK_BT_GATTC_EVT_MTU_EXCHANGE,
								  sizeof(rtk_bt_gatt_mtu_exchange_ind_t));
	if (!p_evt_t) {
		return;
	}

	p_gatt_mtu_exchange_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)p_evt_t->data;
	p_gatt_mtu_exchange_ind->result = RTK_BT_OK;
	p_gatt_mtu_exchange_ind->conn_handle = conn_handle;
	p_gatt_mtu_exchange_ind->mtu_size = mtu;

	rtk_bt_evt_indicate(p_evt_t, NULL);
}
#else /* #if RTK_BLE_MGR_LIB */

#include <profile_client.h>
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <mesh_api.h>
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
#include <provision_client.h>
#include <proxy_client.h>
extern T_APP_RESULT bt_stack_mesh_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
#endif
#endif

static void bt_stack_gattc_discover_state_cb(uint8_t conn_id, T_DISCOVERY_STATE discovery_state);
static void bt_stack_gattc_discover_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE res_type, T_DISCOVERY_RESULT_DATA res_data);
static void bt_stack_gattc_read_result_cb(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value);
static void bt_stack_gattc_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);
static void bt_stack_gattc_disconnect_cb(uint8_t conn_id);

static T_APP_RESULT bt_stack_gattc_cccd_notify_indicate_cb(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value);
static void bt_stack_gattc_cccd_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);

#define RTK_BT_GATTC_INDICATE 		BIT1
#define RTK_BT_GATTC_NOTIFY 		BIT0

static rtk_bt_gattc_app_priv_t *gattc_priv = NULL;
static T_CLIENT_ID gattc_common_id = CLIENT_PROFILE_GENERAL_ID;
static T_CLIENT_ID gattc_cccd_id = CLIENT_PROFILE_GENERAL_ID;

static const T_FUN_CLIENT_CBS gattc_common_cb = {
	/* Discovery State callback */
	bt_stack_gattc_discover_state_cb,
	/* Discovery result callback */
	bt_stack_gattc_discover_result_cb,
	/* Read response callback */
	bt_stack_gattc_read_result_cb,
	/* Write result callback */
	bt_stack_gattc_write_result_cb,
	/* Notify Indicate callback (NULL) */
	NULL,
	/* Link disconnection callback */
	bt_stack_gattc_disconnect_cb,
};


static const T_FUN_CLIENT_CBS gattc_cccd_cb = {
	/* Discovery State callback (NULL) */
	NULL,
	/* Discovery result callback (NULL) */
	NULL,
	/* Read response callback (NULL) */
	NULL,
	/* CCCD write result callback */
	bt_stack_gattc_cccd_write_result_cb,
	/* Notify Indicate callback */
	bt_stack_gattc_cccd_notify_indicate_cb,
	/* Link disconnection callback (NULL) */
	NULL,
};

static uint8_t discover_type_mapping_table[] = {
	[DISC_RESULT_ALL_SRV_UUID16]    = RTK_BT_GATT_DISCOVER_PRIMARY_ALL,
	[DISC_RESULT_ALL_SRV_UUID128]   = RTK_BT_GATT_DISCOVER_PRIMARY_ALL,
	[DISC_RESULT_SRV_DATA]          = RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID,
	[DISC_RESULT_CHAR_UUID16]       = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL,
	[DISC_RESULT_CHAR_UUID128]      = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL,
	[DISC_RESULT_CHAR_DESC_UUID16]  = RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL,
	[DISC_RESULT_CHAR_DESC_UUID128] = RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL,
	[DISC_RESULT_RELATION_UUID16]   = RTK_BT_GATT_DISCOVER_INCLUDE,
	[DISC_RESULT_RELATION_UUID128]  = RTK_BT_GATT_DISCOVER_INCLUDE,
	[DISC_RESULT_BY_UUID16_CHAR]    = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID,
	[DISC_RESULT_BY_UUID128_CHAR]   = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID,
};

/**
 * @brief create a gattc request and deep copy the api param into it
 */
static rtk_bt_gattc_req_t *bt_stack_gattc_create_req(rtk_bt_gattc_req_type_t type, void *param)
{
	uint16_t data_len = 0;
	rtk_bt_gattc_req_t *p_req = (rtk_bt_gattc_req_t *)osif_mem_alloc(
									RAM_TYPE_DATA_ON,
									sizeof(rtk_bt_gattc_req_t));
	if (!p_req) {
		return NULL;
	}

	memset(p_req, 0, sizeof(rtk_bt_gattc_req_t));
	p_req->req_type = type;

	if (type == BT_STACK_GATTC_READ_REQ)
		memcpy(&p_req->read_param, param, sizeof(rtk_bt_gattc_read_param_t));
	else if (type == BT_STACK_GATTC_WRITE_REQ || type == BT_STACK_GATTC_WRITE_CMD)
		memcpy(&p_req->write_param, param, sizeof(rtk_bt_gattc_write_param_t));
	else if (type == BT_STACK_GATTC_DISC_REQ)
		memcpy(&p_req->disc_param, param, sizeof(rtk_bt_gattc_discover_param_t));
	else if (type == BT_STACK_GATTC_CCCD_ENABLE_REQ || type == BT_STACK_GATTC_CCCD_DISABLE_REQ)
		memcpy(&p_req->update_cccd_param, param, sizeof(rtk_bt_gattc_update_cccd_param_t));


	/* some params have 2nd level ptr, need to malloc memory and deep copy */
	if (BT_STACK_GATTC_READ_REQ == type &&
		(RTK_BT_GATT_CHAR_READ_MULTIPLE == p_req->read_param.type ||
		 RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE == p_req->read_param.type)) {
		uint32_t handle_count = 0;
		handle_count = p_req->read_param.multiple_variable.handle_count;
		data_len = handle_count * sizeof(uint16_t);
		p_req->read_param.multiple_variable.handles =
			(uint16_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, data_len);
		if (!p_req->read_param.multiple_variable.handles) {
			osif_mem_free(p_req);
			return NULL;
		}
		memcpy(p_req->read_param.multiple_variable.handles,
			   ((rtk_bt_gattc_read_param_t *)param)->multiple_variable.handles,
			   data_len);
		p_req->user_data = p_req->read_param.multiple_variable.handles;

	} else if (BT_STACK_GATTC_WRITE_REQ == type ||
			   BT_STACK_GATTC_WRITE_CMD == type) {
		data_len = p_req->write_param.length;
		p_req->write_param.data = (void *)osif_mem_alloc(RAM_TYPE_DATA_ON, data_len);
		if (!p_req->write_param.data) {
			osif_mem_free(p_req);
			return NULL;
		}

		memcpy(p_req->write_param.data, ((rtk_bt_gattc_write_param_t *)param)->data,
			   data_len);
		p_req->user_data = p_req->write_param.data;
	}

	return p_req;
}

static void bt_stack_gattc_free_req(rtk_bt_gattc_req_t *p_req)
{
	if (p_req && p_req->user_data) {
		osif_mem_free(p_req->user_data);
	}
	osif_mem_free(p_req);
}

static void bt_stack_gattc_queue_init(rtk_bt_gatt_queue_t *p_queue)
{
	INIT_LIST_HEAD(&p_queue->pending_list);
	p_queue->pending_ele_num = 0;
}

static void bt_stack_gattc_queue_clear_all(rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gattc_req_t *req, *next;

	list_for_each_entry_safe(req, next, &p_queue->pending_list, list, rtk_bt_gattc_req_t) {
		list_del(&req->list);
		bt_stack_gattc_free_req(req);
	}
	p_queue->pending_ele_num = 0;
}

/**
 * @brief get first element of pending list
 */
static rtk_bt_gattc_req_t *get_pending_req(rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	if (!list_empty(&p_queue->pending_list)) {
		p_req = (rtk_bt_gattc_req_t *)p_queue->pending_list.next;
	}
	return p_req;
}

/**
 * @brief return first sent element of pending list and pick it off from list
 */
static rtk_bt_gattc_req_t *remove_sent_req(rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gattc_req_t *p_req = NULL;

	if (!list_empty(&p_queue->pending_list)) {
		p_req = (rtk_bt_gattc_req_t *)p_queue->pending_list.next;
		if (!(p_req->flag & REQ_FLAG_ALREADY_SENT))
			return NULL;
		list_del(&p_req->list);
		p_queue->pending_ele_num--;
	}

	return p_req;
}

static rtk_bt_gattc_cccd_record_t *bt_stack_gattc_find_cccd_recorder(uint8_t conn_id, uint16_t profile_id, uint16_t cccd_handle)
{
	rtk_bt_gattc_cccd_record_t *record = NULL;

	list_for_each_entry(record, &gattc_priv->cccd_record_list[conn_id], list, rtk_bt_gattc_cccd_record_t) {
		if ((record->profile_id == profile_id) && (record->cccd_handle == cccd_handle)) {
			return record;
		}
	}

	return NULL;
}

static bool bt_stack_gattc_cccd_record_get_profile_id(uint8_t conn_id, uint16_t val_handle, uint16_t *profile_id)
{
	rtk_bt_gattc_cccd_record_t *record = NULL;

	list_for_each_entry(record, &gattc_priv->cccd_record_list[conn_id], list, rtk_bt_gattc_cccd_record_t) {
		if (record->val_handle == val_handle) {
			*profile_id = record->profile_id;
			return true;
		}
	}

	return false;
}

static void bt_stack_gattc_add_cccd_record(uint8_t conn_id, rtk_bt_gattc_cccd_record_t *record)
{
	list_add_tail(&record->list, &gattc_priv->cccd_record_list[conn_id]);
}

static void bt_stack_gattc_clear_all_cccd_recorder(uint8_t conn_id)
{
	rtk_bt_gattc_cccd_record_t *record, *next;

	list_for_each_entry_safe(record, next, &gattc_priv->cccd_record_list[conn_id], list, rtk_bt_gattc_cccd_record_t) {
		list_del(&record->list);
		osif_mem_free(record);
	}
}

static uint16_t bt_stack_gattc_send_discover(rtk_bt_gattc_req_t *disc_req)
{
	rtk_bt_gattc_discover_param_t *p_disc_param = &disc_req->disc_param;

	switch (p_disc_param->type) {
	case RTK_BT_GATT_DISCOVER_PRIMARY_ALL:
		if (GAP_CAUSE_SUCCESS != client_all_primary_srv_discovery(disc_req->conn_id, gattc_common_id)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID: {
		rtk_bt_uuid_type_t uuid_type = p_disc_param->disc_primary_by_uuid.uuid_type;
		uint8_t *uuid = p_disc_param->disc_primary_by_uuid.uuid;
		switch (uuid_type) {
		case BT_UUID_TYPE_16:
			if (GAP_CAUSE_SUCCESS != client_by_uuid_srv_discovery(disc_req->conn_id, gattc_common_id, *(uint16_t *)uuid)) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
			break;
		case BT_UUID_TYPE_32:
			/* TODO: */
			return RTK_BT_ERR_UNSUPPORTED;
			break;
		case BT_UUID_TYPE_128:
			if (GAP_CAUSE_SUCCESS != client_by_uuid128_srv_discovery(disc_req->conn_id, gattc_common_id, uuid)) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
			break;
		default:
			return RTK_BT_ERR_UNSUPPORTED;
			break;
		}
		break;
	}
	case RTK_BT_GATT_DISCOVER_INCLUDE: {
		uint16_t start_handle = p_disc_param->disc_inc.start_handle;
		uint16_t end_handle = p_disc_param->disc_inc.end_handle;
		if (GAP_CAUSE_SUCCESS != client_relationship_discovery(disc_req->conn_id, gattc_common_id, start_handle, end_handle)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	}
	case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL: {
		uint16_t start_handle = p_disc_param->disc_char_all.start_handle;
		uint16_t end_handle = p_disc_param->disc_char_all.end_handle;
		if (GAP_CAUSE_SUCCESS != client_all_char_discovery(disc_req->conn_id, gattc_common_id, start_handle, end_handle)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	}
	case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID: {
		uint16_t start_handle = p_disc_param->disc_char_by_uuid.start_handle;
		uint16_t end_handle = p_disc_param->disc_char_by_uuid.end_handle;
		rtk_bt_uuid_type_t uuid_type = p_disc_param->disc_char_by_uuid.uuid_type;
		uint8_t *uuid = p_disc_param->disc_char_by_uuid.uuid;
		switch (uuid_type) {
		case BT_UUID_TYPE_16:
			if (GAP_CAUSE_SUCCESS != client_by_uuid_char_discovery(disc_req->conn_id, gattc_common_id, start_handle, end_handle, *(uint16_t *)uuid)) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
			break;
		case BT_UUID_TYPE_32:
			/* TODO */
			return RTK_BT_ERR_UNSUPPORTED;
			break;
		case BT_UUID_TYPE_128:
			if (GAP_CAUSE_SUCCESS != client_by_uuid128_char_discovery(disc_req->conn_id, gattc_common_id, start_handle, end_handle, uuid)) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
			break;
		default:
			return RTK_BT_ERR_NO_ENTRY;
			break;
		}
		break;
	}
	case RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL: {
		uint16_t start_handle = p_disc_param->disc_descriptor.start_handle;
		uint16_t end_handle = p_disc_param->disc_descriptor.end_handle;
		if (GAP_CAUSE_SUCCESS != client_all_char_descriptor_discovery(disc_req->conn_id, gattc_common_id, start_handle, end_handle)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	}
	default:
		return RTK_BT_ERR_NO_ENTRY;
		break;
	}

	return RTK_BT_OK;

}

static uint16_t bt_stack_gattc_send_read(rtk_bt_gattc_req_t *read_req)
{
	rtk_bt_gattc_read_param_t *p_read_param = &read_req->read_param;
	uint16_t ret = 0;

	switch (p_read_param->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE: {
		uint16_t handle = p_read_param->by_handle.handle;
		if (GAP_CAUSE_SUCCESS != client_attr_read(read_req->conn_id, gattc_common_id, handle)) {
			ret = RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	}
	case RTK_BT_GATT_CHAR_READ_BY_UUID:
	/* need use discover to realize read by uuid */
	case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		ret = RTK_BT_ERR_UNSUPPORTED;
		break;
	case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE: {
#if defined(F_BT_ATT_READ_MULTIPLE_VARIABLE) && F_BT_ATT_READ_MULTIPLE_VARIABLE
		uint32_t handle_count = p_read_param->multiple_variable.handle_count;
		uint16_t *handles = p_read_param->multiple_variable.handles;

		if (GAP_CAUSE_SUCCESS != client_attr_read_multi_variable(read_req->conn_id, gattc_common_id, handle_count, handles)) {
			ret = RTK_BT_ERR_LOWER_STACK_API;
		}
#else
		ret = RTK_BT_ERR_UNSUPPORTED;
#endif
		break;
	}
	default:
		ret = RTK_BT_ERR_NO_ENTRY;
		break;
	}

	return ret;
}

static uint16_t bt_stack_gattc_send_write(rtk_bt_gattc_req_t *write_req)
{
	rtk_bt_gattc_write_param_t *p_write_param = &write_req->write_param;
	T_GATT_WRITE_TYPE write_type = GATT_WRITE_TYPE_REQ;

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_REQ) {
		write_type = GATT_WRITE_TYPE_REQ;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP) {
		write_type = GATT_WRITE_TYPE_CMD;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		write_type = GATT_WRITE_TYPE_SIGNED_CMD;
	} else {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (GAP_CAUSE_SUCCESS != client_attr_write(write_req->conn_id,
			gattc_common_id,
			write_type,
			p_write_param->handle,
			p_write_param->length,
			(uint8_t *)p_write_param->data)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gattc_send_cccd(rtk_bt_gattc_req_t *p_req)
{
	rtk_bt_gattc_cccd_record_t *record = NULL;

	record = bt_stack_gattc_find_cccd_recorder(p_req->conn_id,
					p_req->update_cccd_param.profile_id, p_req->update_cccd_param.cccd_handle);
	if (!record) {
		record = (rtk_bt_gattc_cccd_record_t *)osif_mem_alloc(
							RAM_TYPE_DATA_ON,
							sizeof(rtk_bt_gattc_cccd_record_t));
		memset(record, 0, sizeof(rtk_bt_gattc_cccd_record_t));
		bt_stack_gattc_add_cccd_record(p_req->conn_id, record);
		record->profile_id = p_req->update_cccd_param.profile_id;
		record->conn_handle = p_req->update_cccd_param.conn_handle;
		record->val_handle = p_req->update_cccd_param.char_val_handle;
		record->cccd_handle = p_req->update_cccd_param.cccd_handle;
	}

	record->cccd_value = 0;
	if (p_req->req_type == BT_STACK_GATTC_CCCD_ENABLE_REQ) {
		if (p_req->update_cccd_param.bindicate) {
			record->cccd_value |= RTK_BT_GATTC_INDICATE;
		}
		if (p_req->update_cccd_param.bnotify) {
			record->cccd_value |= RTK_BT_GATTC_NOTIFY;
		}
	}

	if (GAP_CAUSE_SUCCESS == client_attr_write(
			p_req->conn_id,
			gattc_cccd_id, GATT_WRITE_TYPE_REQ,
			p_req->update_cccd_param.cccd_handle,
			sizeof(uint16_t), (uint8_t *)&(record->cccd_value))) {
		return RTK_BT_OK;
	}

	return RTK_BT_ERR_LOWER_STACK_API;
}

typedef uint16_t (*gattc_send_func)(rtk_bt_gattc_req_t *);

static gattc_send_func bt_stack_gattc_get_send_func(rtk_bt_gattc_req_type_t type)
{
	gattc_send_func send_func = NULL;
	switch (type) {
	case BT_STACK_GATTC_DISC_REQ:
		send_func = bt_stack_gattc_send_discover;
		break;
	case BT_STACK_GATTC_READ_REQ:
		send_func = bt_stack_gattc_send_read;
		break;
	case BT_STACK_GATTC_WRITE_REQ:
	case BT_STACK_GATTC_WRITE_CMD:
		send_func = bt_stack_gattc_send_write;
		break;
	case BT_STACK_GATTC_CCCD_ENABLE_REQ:
	case BT_STACK_GATTC_CCCD_DISABLE_REQ:
		send_func = bt_stack_gattc_send_cccd;
		break;
	default:
		break;
	}

	return send_func;
}

static void bt_stack_gattc_err_indicate(rtk_bt_gattc_req_t *req,
										uint16_t err_code)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_req_type_t type = req->req_type;

	if (req->flag & REQ_FLAG_READ_BY_UUID) {
		type = BT_STACK_GATTC_READ_REQ;
	}

	switch (type) {
	case BT_STACK_GATTC_DISC_REQ: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND,
									sizeof(rtk_bt_gattc_discover_ind_t));
		rtk_bt_gattc_discover_ind_t *p_disc_ind =
			(rtk_bt_gattc_discover_ind_t *)p_evt->data;
		p_disc_ind->conn_handle = req->disc_param.conn_handle;
		p_disc_ind->profile_id = req->disc_param.profile_id;
		p_disc_ind->type = req->disc_param.type;
		p_disc_ind->status = RTK_BT_STATUS_FAIL;
		p_disc_ind->err_code = err_code;
		break;
	}
	case BT_STACK_GATTC_READ_REQ: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t));
		rtk_bt_gattc_read_ind_t *p_read_ind =
			(rtk_bt_gattc_read_ind_t *)p_evt->data;
		p_read_ind->conn_handle = req->read_param.conn_handle;
		p_read_ind->profile_id = req->read_param.profile_id;
		if (req->flag & REQ_FLAG_READ_BY_UUID) {
			p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
		} else {
			p_read_ind->type = req->read_param.type;
		}
		p_read_ind->status = RTK_BT_STATUS_FAIL;
		p_read_ind->err_code = err_code;
		break;
	}
	case BT_STACK_GATTC_WRITE_REQ:
	case BT_STACK_GATTC_WRITE_CMD: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_WRITE_RESULT_IND,
									sizeof(rtk_bt_gattc_write_ind_t));
		rtk_bt_gattc_write_ind_t *p_write_ind =
			(rtk_bt_gattc_write_ind_t *)p_evt->data;
		p_write_ind->conn_handle = req->write_param.conn_handle;
		p_write_ind->profile_id = req->write_param.profile_id;
		p_write_ind->type = req->write_param.type;
		p_write_ind->status = RTK_BT_STATUS_FAIL;
		p_write_ind->err_code = err_code;
		break;
	}
	case BT_STACK_GATTC_CCCD_ENABLE_REQ:
	case BT_STACK_GATTC_CCCD_DISABLE_REQ: {
		uint8_t evt = ((type == BT_STACK_GATTC_CCCD_ENABLE_REQ) ? RTK_BT_GATTC_EVT_CCCD_ENABLE_IND :
					   RTK_BT_GATTC_EVT_CCCD_DISABLE_IND);
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									evt, sizeof(rtk_bt_gattc_write_ind_t));
		rtk_bt_gattc_cccd_update_ind_t *p_cccd_update =
			(rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;
		p_cccd_update->conn_handle = req->update_cccd_param.conn_handle;
		p_cccd_update->profile_id = req->update_cccd_param.profile_id;
		p_cccd_update->status = RTK_BT_STATUS_FAIL;
		p_cccd_update->err_code = err_code;
		break;
	}
	default:
		break;
	}

	if (p_evt)
		rtk_bt_evt_indicate(p_evt, NULL);
}

/* send cmd to stack, if no credits, it will fail and free the req */
static uint16_t send_cmd_to_stack(rtk_bt_gattc_req_t *req)
{
	gattc_send_func func = NULL;
	rtk_bt_gattc_req_type_t type = req->req_type;
	uint16_t ret;
	uint16_t credits = 0;

	le_get_gap_param(GAP_PARAM_LE_REMAIN_CREDITS, &credits);
	if (!credits) { 
		ret = RTK_BT_ERR_NO_CREDITS;
		goto fail;
	}

	func = bt_stack_gattc_get_send_func(type);
	if (!func) {
		ret = RTK_BT_ERR_NO_ENTRY;
		goto fail;
	}

	ret = func(req);
	if (RTK_BT_OK == ret) {
		req->flag |= REQ_FLAG_ALREADY_SENT;
		return ret;
	}

fail:
	bt_stack_gattc_free_req(req);
	return ret;
}

static bool send_req_to_stack(rtk_bt_gattc_req_t *req, rtk_bt_gatt_queue_t *queue)
{
	gattc_send_func func = NULL;
	rtk_bt_gattc_req_type_t type = req->req_type;
	uint16_t ret;

	func = bt_stack_gattc_get_send_func(type);
	if (!func) {
		return false;
	}

	ret = func(req);
	if (RTK_BT_OK == ret) {
		req->flag |= REQ_FLAG_ALREADY_SENT;
		return true;
	}

	list_del(&req->list);
	bt_stack_gattc_err_indicate(req, ret);
	bt_stack_gattc_free_req(req);
	queue->pending_ele_num--;
	return false;
}

/**
 * @brief handle pending req in pending_list
 */
static void bt_stack_gattc_handle_pending_req(uint8_t conn_id)
{
	(void)conn_id;  /* leave the param conn_id for future debug need */
	rtk_bt_gatt_queue_t *queue;
	rtk_bt_gattc_req_t *req = NULL;
	uint8_t i, choosen_connid = 0xFF;
	uint32_t min_seq = 0xFFFFFFFF;

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		queue = &gattc_priv->request_queue[i];
		if (list_empty(&queue->pending_list))
			continue;
		req = (rtk_bt_gattc_req_t *)queue->pending_list.next;
		if (req->flag & REQ_FLAG_ALREADY_SENT)
			continue;
		if (req->seq <= min_seq) {
			min_seq = req->seq;
			choosen_connid = i;
		}
	}
	if (choosen_connid != 0xFF) {
		queue = &gattc_priv->request_queue[choosen_connid];
		req = (rtk_bt_gattc_req_t *)queue->pending_list.next;
		send_req_to_stack(req, queue);
	}
}

static uint16_t bt_stack_gattc_add_req_to_list(uint8_t conn_id,
		rtk_bt_gattc_req_t *req)
{
	rtk_bt_gatt_queue_t *p_queue;

	if (req->req_type == BT_STACK_GATTC_WRITE_CMD)
		p_queue = &gattc_priv->cmd_queue[conn_id];
	else
		p_queue = &gattc_priv->request_queue[conn_id];

	/* The number of element in pending queue should be limited, otherwise
		the write and read request of high frequnce will use up memory */
	if (p_queue->pending_ele_num >= BT_QUEUE_PENDING_ELEMENT_MAX) {
		printf("Error: GATTC pending queue full, wait a moment to send data again !!!\r\n");
		return RTK_BT_ERR_QUEUE_FULL;
	}
	list_add_tail(&req->list, &p_queue->pending_list);
	p_queue->pending_ele_num++;
	gattc_priv->last_seq++;
	req->seq = gattc_priv->last_seq;
	API_PRINT("Pending ele num+++++, num: %d\r\n", p_queue->pending_ele_num);

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_send(uint16_t conn_handle,
									rtk_bt_gattc_req_t *req)
{
	uint8_t conn_id;
	uint16_t ret = 0;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		bt_stack_gattc_free_req(req);
		return RTK_BT_ERR_PARAM_INVALID;
	}
	req->conn_id = conn_id;

	if (req->req_type == BT_STACK_GATTC_WRITE_CMD) {
		ret = send_cmd_to_stack(req);
		if (ret != RTK_BT_OK)
			return ret;
	}

	if (bt_stack_gattc_add_req_to_list(conn_id, req)) {
		bt_stack_gattc_free_req(req);
		return RTK_BT_ERR_QUEUE_FULL;
	}

	if (req->req_type != BT_STACK_GATTC_WRITE_CMD) {
		/* request not affected by credits, so, just trigger to send it */
		bt_stack_gattc_handle_pending_req(conn_id);
	}

	return RTK_BT_OK;
}

static void internal_read_req_add(uint8_t conn_id, rtk_bt_gattc_req_t *read_req)
{
	rtk_bt_gatt_queue_t *p_queue;
	rtk_bt_gattc_req_t *req;

	p_queue = &gattc_priv->request_queue[conn_id];

	list_for_each_entry(req, &p_queue->pending_list, list, rtk_bt_gattc_req_t) {
	/* 	the internal used read_by_handle req shall be put before
		1. all internal used disc_char_by_uuid req which haven't been sent
		2. all other normal pending node */
		if ((req->flag == REQ_FLAG_READ_BY_UUID && req->req_type == BT_STACK_GATTC_DISC_REQ && 
			req->disc_param.type == RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID)  
			|| !req->flag) 
			break;
	}

	read_req->conn_id = conn_id;
	read_req->flag = REQ_FLAG_READ_BY_UUID;
	list_add_tail(&read_req->list, &req->list);
	p_queue->pending_ele_num++;
	gattc_priv->last_seq++;
	read_req->seq = gattc_priv->last_seq;
	API_PRINT("Pending ele num+++++, num: %d\r\n", p_queue->pending_ele_num);
}

static bool internel_read_req_is_done(uint8_t conn_id)
{
	rtk_bt_gattc_req_t *req;

	list_for_each_entry(req, &gattc_priv->request_queue[conn_id].pending_list, list, rtk_bt_gattc_req_t) {
		if ((req->flag & REQ_FLAG_READ_BY_UUID) && req->req_type == BT_STACK_GATTC_READ_REQ)
			return false;
	}

	return true;
}

static void bt_stack_gattc_discover_state_cb(uint8_t conn_id,
		T_DISCOVERY_STATE discovery_state)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_discover_ind_t *p_disc_ind = NULL;
	rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
	rtk_bt_gatt_queue_t *p_queue = &gattc_priv->request_queue[conn_id];
	rtk_bt_gattc_req_t *p_req = NULL;
	uint8_t status = RTK_BT_STATUS_DONE;
	uint16_t err_code = 0;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_req = remove_sent_req(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_DISC_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		goto end;
	}

	if (!(p_req->flag & REQ_FLAG_READ_BY_UUID)) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND,
									sizeof(rtk_bt_gattc_discover_ind_t));
		if (!p_evt) {
			goto end;
		}

		if (discovery_state == DISC_STATE_FAILED) {
			status = RTK_BT_STATUS_FAIL;
			err_code = RTK_BT_FAIL;
		}

		p_disc_ind = (rtk_bt_gattc_discover_ind_t *)p_evt->data;
		p_disc_ind->conn_handle = conn_handle;
		p_disc_ind->profile_id = p_req->disc_param.profile_id;
		p_disc_ind->type = p_req->disc_param.type;
		p_disc_ind->status = status;
		p_disc_ind->err_code = err_code;
		rtk_bt_evt_indicate(p_evt, NULL);

	} else {
		/* if read_by_uuid(internal use discover and read_by_handle) failed or
			discover out nothing, indicate err here */
		if (DISC_STATE_FAILED == discovery_state ||
			internel_read_req_is_done(conn_id)) {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_READ_RESULT_IND,
										sizeof(rtk_bt_gattc_read_ind_t));
			if (!p_evt) {
				goto end;
			}
			p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
			p_read_ind->conn_handle = conn_handle;
			p_read_ind->profile_id = p_req->disc_param.profile_id;
			p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
			p_read_ind->status = RTK_BT_STATUS_FAIL;
			p_read_ind->err_code = RTK_BT_FAIL;
			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}

end:
	bt_stack_gattc_free_req(p_req);
	bt_stack_gattc_handle_pending_req(conn_id);
}

static void bt_stack_gattc_discover_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE res_type, T_DISCOVERY_RESULT_DATA res_data)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = &gattc_priv->request_queue[conn_id];
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_discover_ind_t *p_disc_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_req = get_pending_req(p_queue);

	if (!p_req || (!(p_req->flag & REQ_FLAG_ALREADY_SENT)) || p_req->req_type != BT_STACK_GATTC_DISC_REQ) {
		return;
	}

	if (p_req->disc_param.type != discover_type_mapping_table[res_type]) {
		return;
	}

	/* 	if the discover is used for read_by_uuid, record the handles, read value by handle */
	if (p_req->flag & REQ_FLAG_READ_BY_UUID) {
		rtk_bt_gattc_read_param_t read_param = {0};
		read_param.profile_id = p_req->disc_param.profile_id;
		read_param.conn_handle = conn_handle;
		read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
		if (DISC_RESULT_BY_UUID16_CHAR == res_type) {
			read_param.by_handle.handle = res_data.p_char_uuid16_disc_data->value_handle;
		} else if (DISC_RESULT_BY_UUID128_CHAR == res_type) {
			read_param.by_handle.handle = res_data.p_char_uuid128_disc_data->value_handle;
		}

		rtk_bt_gattc_req_t *p_read_req = bt_stack_gattc_create_req(BT_STACK_GATTC_READ_REQ, &read_param);
		if (!p_read_req) {
			return;
		}

		internal_read_req_add(conn_id, p_read_req);

		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND, sizeof(rtk_bt_gattc_discover_ind_t));
	p_disc_ind = (rtk_bt_gattc_discover_ind_t *)p_evt->data;

	p_disc_ind->conn_handle = conn_handle;
	p_disc_ind->profile_id = p_req->disc_param.profile_id;
	p_disc_ind->type = p_req->disc_param.type;
	p_disc_ind->status = RTK_BT_STATUS_CONTINUE;
	// p_disc_ind->has_data = 1;

	switch (res_type) {
	case DISC_RESULT_ALL_SRV_UUID16:
		p_disc_ind->disc_primary_all_per.start_handle = res_data.p_srv_uuid16_disc_data->att_handle;
		p_disc_ind->disc_primary_all_per.end_handle = res_data.p_srv_uuid16_disc_data->end_group_handle;
		p_disc_ind->disc_primary_all_per.uuid_type = BT_UUID_TYPE_16;
		memcpy((void *)p_disc_ind->disc_primary_all_per.uuid, (void *)&res_data.p_srv_uuid16_disc_data->uuid16, BT_UUID_SIZE_16);
		break;
	case DISC_RESULT_ALL_SRV_UUID128:
		p_disc_ind->disc_primary_all_per.start_handle = res_data.p_srv_uuid128_disc_data->att_handle;
		p_disc_ind->disc_primary_all_per.end_handle = res_data.p_srv_uuid128_disc_data->end_group_handle;
		p_disc_ind->disc_primary_all_per.uuid_type = BT_UUID_TYPE_128;
		memcpy((void *)p_disc_ind->disc_primary_all_per.uuid, (void *)res_data.p_srv_uuid128_disc_data->uuid128, BT_UUID_SIZE_128);
		break;
	case DISC_RESULT_SRV_DATA:
		p_disc_ind->disc_primary_by_uuid_per.start_handle = res_data.p_srv_disc_data->att_handle;
		p_disc_ind->disc_primary_by_uuid_per.end_handle = res_data.p_srv_disc_data->end_group_handle;
		break;
	case DISC_RESULT_RELATION_UUID16:
		p_disc_ind->disc_inc_per.handle = res_data.p_relation_uuid16_disc_data->decl_handle;
		p_disc_ind->disc_inc_per.start_handle = res_data.p_relation_uuid16_disc_data->att_handle;
		p_disc_ind->disc_inc_per.end_handle = res_data.p_relation_uuid16_disc_data->end_group_handle;
		p_disc_ind->disc_inc_per.uuid_type = BT_UUID_TYPE_16;
		memcpy((void *)p_disc_ind->disc_inc_per.uuid, (void *)&res_data.p_relation_uuid16_disc_data->uuid16, BT_UUID_SIZE_16);
		break;
	case DISC_RESULT_RELATION_UUID128:
		p_disc_ind->disc_inc_per.handle = res_data.p_relation_uuid128_disc_data->decl_handle;
		p_disc_ind->disc_inc_per.start_handle = res_data.p_relation_uuid128_disc_data->att_handle;
		p_disc_ind->disc_inc_per.end_handle = res_data.p_relation_uuid128_disc_data->end_group_handle;
		p_disc_ind->disc_inc_per.uuid_type = BT_UUID_TYPE_128;
		memcpy((void *)p_disc_ind->disc_inc_per.uuid, (void *)res_data.p_relation_uuid128_disc_data->uuid128, BT_UUID_SIZE_128);
		break;
	case DISC_RESULT_CHAR_UUID16:
		p_disc_ind->disc_char_all_per.handle = res_data.p_char_uuid16_disc_data->decl_handle;
		p_disc_ind->disc_char_all_per.properties = res_data.p_char_uuid16_disc_data->properties;
		p_disc_ind->disc_char_all_per.value_handle = res_data.p_char_uuid16_disc_data->value_handle;
		p_disc_ind->disc_char_all_per.uuid_type = BT_UUID_TYPE_16;
		memcpy((void *)p_disc_ind->disc_char_all_per.uuid, (void *)&res_data.p_char_uuid16_disc_data->uuid16, BT_UUID_SIZE_16);
		break;
	case DISC_RESULT_CHAR_UUID128:
		p_disc_ind->disc_char_all_per.handle = res_data.p_char_uuid128_disc_data->decl_handle;
		p_disc_ind->disc_char_all_per.properties = res_data.p_char_uuid128_disc_data->properties;
		p_disc_ind->disc_char_all_per.value_handle = res_data.p_char_uuid128_disc_data->value_handle;
		p_disc_ind->disc_char_all_per.uuid_type = BT_UUID_TYPE_128;
		memcpy((void *)p_disc_ind->disc_char_all_per.uuid, (void *)res_data.p_char_uuid128_disc_data->uuid128, BT_UUID_SIZE_128);
		break;
	case DISC_RESULT_BY_UUID16_CHAR:
		p_disc_ind->disc_char_by_uuid_per.handle = res_data.p_char_uuid16_disc_data->decl_handle;
		p_disc_ind->disc_char_by_uuid_per.properties = res_data.p_char_uuid16_disc_data->properties;
		p_disc_ind->disc_char_by_uuid_per.value_handle = res_data.p_char_uuid16_disc_data->value_handle;
		p_disc_ind->disc_char_by_uuid_per.uuid_type = BT_UUID_TYPE_16;
		memcpy((void *)p_disc_ind->disc_char_by_uuid_per.uuid, (void *)&res_data.p_char_uuid16_disc_data->uuid16, BT_UUID_SIZE_16);
		break;
	case DISC_RESULT_BY_UUID128_CHAR:
		p_disc_ind->disc_char_by_uuid_per.handle = res_data.p_char_uuid128_disc_data->decl_handle;
		p_disc_ind->disc_char_by_uuid_per.properties = res_data.p_char_uuid128_disc_data->properties;
		p_disc_ind->disc_char_by_uuid_per.value_handle = res_data.p_char_uuid128_disc_data->value_handle;
		p_disc_ind->disc_char_by_uuid_per.uuid_type = BT_UUID_TYPE_128;
		memcpy((void *)p_disc_ind->disc_char_by_uuid_per.uuid, (void *)res_data.p_char_uuid128_disc_data->uuid128, BT_UUID_SIZE_128);
		break;
	case DISC_RESULT_CHAR_DESC_UUID16:
		p_disc_ind->disc_descriptor_per.handle = res_data.p_char_desc_uuid16_disc_data->handle;
		p_disc_ind->disc_descriptor_per.uuid_type = BT_UUID_TYPE_16;
		memcpy((void *)p_disc_ind->disc_descriptor_per.uuid, (void *)&res_data.p_char_desc_uuid16_disc_data->uuid16, BT_UUID_SIZE_16);
		break;
	case DISC_RESULT_CHAR_DESC_UUID128:
		p_disc_ind->disc_descriptor_per.handle = res_data.p_char_desc_uuid128_disc_data->handle;
		p_disc_ind->disc_descriptor_per.uuid_type = BT_UUID_TYPE_128;
		memcpy((void *)p_disc_ind->disc_descriptor_per.uuid, (void *)res_data.p_char_desc_uuid128_disc_data->uuid128, BT_UUID_SIZE_128);
		break;
	default:
		p_disc_ind->status = RTK_BT_STATUS_FAIL;
		break;
	}

	rtk_bt_evt_indicate(p_evt, NULL);
}

static void bt_stack_gattc_read_result_cb(uint8_t conn_id, uint16_t cause,
		uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = &gattc_priv->request_queue[conn_id];
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_req = remove_sent_req(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_READ_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		goto end;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_READ_RESULT_IND, sizeof(rtk_bt_gattc_read_ind_t));
	if (!p_evt) {
		goto end;
	}

	p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
	p_read_ind->profile_id = p_req->read_param.profile_id;
	p_read_ind->conn_handle = conn_handle;
	p_read_ind->type = p_req->read_param.type;
	p_read_ind->status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_CONTINUE;
	p_read_ind->err_code = cause;
	/* before malloc memory according to value_size, need to check if cause has err,
		if cause has err, value_size may be invalid and very huge */
	if (0 == cause) {
		p_evt->user_data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, value_size);
		if (!p_evt->user_data) {
			goto end;
		}

		switch (p_req->read_param.type) {
		case RTK_BT_GATT_CHAR_READ_BY_HANDLE:
			p_read_ind->by_handle.handle = handle;
			p_read_ind->by_handle.len = value_size;
			p_read_ind->by_handle.value = p_evt->user_data;
			// p_read_ind->by_handle.value = p_value;
			memcpy(p_read_ind->by_handle.value, p_value, value_size);
			break;
		case RTK_BT_GATT_CHAR_READ_BY_UUID:
			p_read_ind->by_uuid_per.handle = handle;
			p_read_ind->by_uuid_per.len = value_size;
			p_read_ind->by_uuid_per.value = p_evt->user_data;
			// p_read_ind->by_uuid_per.value = p_value;
			memcpy(p_read_ind->by_uuid_per.value, p_value, value_size);
			break;
		case RTK_BT_GATT_CHAR_READ_MULTIPLE:
		case RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE:
			break;
		default:
			break;
		}
	}

	if (p_req->flag & REQ_FLAG_READ_BY_UUID) {
		p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
	}

	rtk_bt_evt_indicate(p_evt, NULL);

	/* if read_by_handle cb comes and read_by_uuid the latest cb comes,  send event
	   to indicate charac read is done */
	if (((p_req->flag & REQ_FLAG_READ_BY_UUID) && internel_read_req_is_done(conn_id)) ||
		(!(p_req->flag & REQ_FLAG_READ_BY_UUID) && RTK_BT_GATT_CHAR_READ_BY_HANDLE == p_req->read_param.type && 0 == cause)) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t));
		if (!p_evt) {
			goto end;
		}

		p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
		p_read_ind->profile_id = p_req->read_param.profile_id;
		p_read_ind->conn_handle = conn_handle;
		if (p_req->flag & REQ_FLAG_READ_BY_UUID) {
			p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
		} else {
			p_read_ind->type = p_req->read_param.type;
		}
		p_read_ind->status = RTK_BT_STATUS_DONE;

		rtk_bt_evt_indicate(p_evt, NULL);
	}

end:
	bt_stack_gattc_free_req(p_req);
	bt_stack_gattc_handle_pending_req(conn_id);
}

static void bt_stack_gattc_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits)
{
	(void)credits;
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_write_ind_t *p_write_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	if (type == GATT_WRITE_TYPE_REQ) {
		p_queue = &gattc_priv->request_queue[conn_id];
		p_req = remove_sent_req(p_queue);
		if (!p_req || p_req->req_type != BT_STACK_GATTC_WRITE_REQ) {
			printf("Gattc cb mismatch with req type\r\n");
			goto end;
		}
	} else if (GATT_WRITE_TYPE_CMD == type || GATT_WRITE_TYPE_SIGNED_CMD == type) {
		p_queue = &gattc_priv->cmd_queue[conn_id];
		p_req = remove_sent_req(p_queue);
		if (!p_req || p_req->req_type != BT_STACK_GATTC_WRITE_CMD) {
			printf("Gattc cb mismatch with req type\r\n");
			goto end;
		}
	} else {
		printf("bt_stack_gattc_write_result_cb: wrong type \r\n");
		goto end;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_WRITE_RESULT_IND, sizeof(rtk_bt_gattc_write_ind_t));
	if (!p_evt) {
		goto end;
	}

	p_write_ind = (rtk_bt_gattc_write_ind_t *)p_evt->data;
	p_write_ind->profile_id = p_req->write_param.profile_id;
	p_write_ind->conn_handle = conn_handle;
	p_write_ind->type = p_req->write_param.type;
	p_write_ind->status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_DONE;
	p_write_ind->err_code = cause;
	p_write_ind->handle = handle;

	rtk_bt_evt_indicate(p_evt, NULL);

end:
	bt_stack_gattc_free_req(p_req);
	if (p_req->req_type != BT_STACK_GATTC_WRITE_CMD)
		bt_stack_gattc_handle_pending_req(conn_id);
}

static void bt_stack_gattc_disconnect_cb(uint8_t conn_id)
{
	bt_stack_gattc_clear_all_cccd_recorder(conn_id);

	bt_stack_gattc_queue_clear_all(&gattc_priv->cmd_queue[conn_id]);
	bt_stack_gattc_queue_clear_all(&gattc_priv->request_queue[conn_id]);
}

static void bt_stack_gattc_cccd_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
		uint16_t cccd_handle, uint16_t cause, uint8_t credits)
{
	(void)type;
	(void)credits;
	rtk_bt_gattc_req_t *p_req = NULL;
	bool enable;
	rtk_bt_gatt_queue_t *p_queue = &gattc_priv->request_queue[conn_id];
	rtk_bt_gattc_cccd_record_t *record = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_cccd_update_ind_t *p_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_req = remove_sent_req(p_queue);
	if (!p_req || (p_req->req_type != BT_STACK_GATTC_CCCD_ENABLE_REQ && p_req->req_type != BT_STACK_GATTC_CCCD_DISABLE_REQ)) {
		printf("Gattc cb mismatch with req type\r\n");
		goto end;
	}

	enable = (p_req->req_type == BT_STACK_GATTC_CCCD_ENABLE_REQ);
	if (!enable) {
		record = bt_stack_gattc_find_cccd_recorder(conn_id, p_req->update_cccd_param.profile_id, p_req->update_cccd_param.cccd_handle);
		if (record) {
			list_del(&record->list);
			osif_mem_free(record);
		}
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
								enable ? RTK_BT_GATTC_EVT_CCCD_ENABLE_IND : RTK_BT_GATTC_EVT_CCCD_DISABLE_IND,
								sizeof(rtk_bt_gattc_cccd_update_ind_t));

	if (!p_evt) {
		goto end;
	}

	//indicate event
	p_ind = (rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;

	p_ind->profile_id = p_req->update_cccd_param.profile_id;
	p_ind->conn_handle = conn_handle;
	p_ind->cccd_handle = cccd_handle;
	p_ind->bnotify = p_req->update_cccd_param.bnotify;
	p_ind->bindicate = p_req->update_cccd_param.bindicate;
	p_ind->status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_DONE;
	p_ind->err_code = cause;

	rtk_bt_evt_indicate(p_evt, NULL);

end:
	bt_stack_gattc_free_req(p_req);
	bt_stack_gattc_handle_pending_req(conn_id);
}

static T_APP_RESULT bt_stack_gattc_cccd_notify_indicate_cb(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
	uint16_t profile_id = 0xFFFF;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_cccd_value_ind_t *p_value_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	if (false == bt_stack_gattc_cccd_record_get_profile_id(conn_id, handle, &profile_id)) {
		return APP_RESULT_REJECT;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
								notify ? RTK_BT_GATTC_EVT_NOTIFY_IND : RTK_BT_GATTC_EVT_INDICATE_IND,
								sizeof(rtk_bt_gattc_cccd_value_ind_t));

	if (!p_evt) {
		return APP_RESULT_REJECT;
	}

	p_evt->user_data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, value_size);
	if (!p_evt->user_data) {
		return APP_RESULT_REJECT;
	}

	p_value_ind = (rtk_bt_gattc_cccd_value_ind_t *)p_evt->data;
	p_value_ind->conn_handle = conn_handle;
	p_value_ind->profile_id = profile_id;
	p_value_ind->value_handle = handle;
	p_value_ind->len = value_size;
	p_value_ind->value = p_evt->user_data;
	memcpy(p_value_ind->value, p_value, value_size);

	rtk_bt_evt_indicate(p_evt, NULL);

	return APP_RESULT_SUCCESS;
}

static uint16_t bt_stack_gattc_discover(void *param)
{
	rtk_bt_gattc_discover_param_t *p_dis_param = (rtk_bt_gattc_discover_param_t *)param;
	rtk_bt_gattc_req_t *req = NULL;

	req = bt_stack_gattc_create_req(BT_STACK_GATTC_DISC_REQ, p_dis_param);
	if (!req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	return bt_stack_gattc_send(p_dis_param->conn_handle, req);
}

static uint16_t bt_stack_gattc_read(void *param)
{
	rtk_bt_gattc_read_param_t *p_read_param = (rtk_bt_gattc_read_param_t *)param;
	rtk_bt_gattc_req_t *req = NULL;

	/* read by uuid is special, use discover by uuid to realize it */
	if (RTK_BT_GATT_CHAR_READ_BY_UUID == p_read_param->type) {
		rtk_bt_gattc_discover_param_t disc_param = {0};
		disc_param.conn_handle = p_read_param->conn_handle;
		disc_param.profile_id = p_read_param->profile_id;
		disc_param.type = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID;
		disc_param.disc_char_by_uuid.start_handle = p_read_param->by_uuid.start_handle;
		disc_param.disc_char_by_uuid.end_handle = p_read_param->by_uuid.end_handle;
		disc_param.disc_char_by_uuid.uuid_type = p_read_param->by_uuid.uuid_type;
		memcpy(disc_param.disc_char_by_uuid.uuid,
			   p_read_param->by_uuid.uuid, BT_UUID_SIZE_MAX);

		req = bt_stack_gattc_create_req(BT_STACK_GATTC_DISC_REQ, &disc_param);
		if (!req) {
			return RTK_BT_ERR_NO_MEMORY;
		}
		req->flag = REQ_FLAG_READ_BY_UUID;
		
	} else {
		req = bt_stack_gattc_create_req(BT_STACK_GATTC_READ_REQ, p_read_param);
		if (!req) {
			return RTK_BT_ERR_NO_MEMORY;
		}
	}

	return bt_stack_gattc_send(p_read_param->conn_handle, req);
}

static uint16_t bt_stack_gattc_write(void *param)
{
	rtk_bt_gattc_write_param_t *p_write_param = (rtk_bt_gattc_write_param_t *)param;
	rtk_bt_gattc_req_type_t type = 0;
	rtk_bt_gattc_req_t *req = NULL;

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_REQ) {
		type = BT_STACK_GATTC_WRITE_REQ;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP ||
			   p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		type = BT_STACK_GATTC_WRITE_CMD;
	}

	req = bt_stack_gattc_create_req(type, p_write_param);
	if (!req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	return bt_stack_gattc_send(p_write_param->conn_handle, req);
}

static uint16_t bt_stack_gattc_update_cccd(void *param, bool benable)
{
	rtk_bt_gattc_update_cccd_param_t *p_cccd_param = (rtk_bt_gattc_update_cccd_param_t *)param;
	rtk_bt_gattc_req_t *req = NULL;

	req = bt_stack_gattc_create_req(benable ? BT_STACK_GATTC_CCCD_ENABLE_REQ : BT_STACK_GATTC_CCCD_DISABLE_REQ, p_cccd_param);
	if (!req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	return bt_stack_gattc_send(p_cccd_param->conn_handle, req);
}

static uint16_t bt_stack_gattc_exchange_mtu(void *param)
{
	T_GAP_CAUSE cause;
	uint16_t conn_handle = *(uint16_t *)param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	cause = client_send_exchange_mtu_req(conn_id);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

void bt_stack_gattc_evt_indicate_mtu_exchange(uint16_t conn_handle, uint16_t mtu)
{
	rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_exchange_ind = NULL;
	rtk_bt_evt_t *p_evt_t = NULL;

	p_evt_t = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
								  RTK_BT_GATTC_EVT_MTU_EXCHANGE,
								  sizeof(rtk_bt_gatt_mtu_exchange_ind_t));
	if (!p_evt_t) {
		return;
	}

	p_gatt_mtu_exchange_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)p_evt_t->data;
	p_gatt_mtu_exchange_ind->result = RTK_BT_OK;
	p_gatt_mtu_exchange_ind->conn_handle = conn_handle;
	p_gatt_mtu_exchange_ind->mtu_size = mtu;

	rtk_bt_evt_indicate(p_evt_t, NULL);
}

uint16_t bt_stack_le_gattc_get_tx_pending_num(uint16_t conn_handle, uint16_t *tx_pending_num)
{
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (tx_pending_num == NULL || conn_id >= RTK_BLE_GAP_MAX_LINKS) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	//there is no pending list in cmd queue
	if (gattc_priv) {
		*tx_pending_num = gattc_priv->request_queue[conn_id].pending_ele_num +
						  gattc_priv->cmd_queue[conn_id].pending_ele_num;
	}

	return 0;
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_stack_gattc_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;

	if (true != bt_stack_profile_check(RTK_BT_PROFILE_GATTC)) {
		printf("Error: Gattc profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	// printf("bt_stack_gattc_act_handle: act = %d \r\n", p_cmd->act);
	switch (p_cmd->act) {
	case RTK_BT_GATTC_ACT_REGISTER_PROFILE:
		ret = 0;
		break;
	case RTK_BT_GATTC_ACT_EXCHANGE_MTU:
		ret = bt_stack_gattc_exchange_mtu(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_DISCOVER:
		ret = bt_stack_gattc_discover(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_READ:
		ret = bt_stack_gattc_read(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_WRITE:
		ret = bt_stack_gattc_write(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_CCCD_ENABLE:
		ret = bt_stack_gattc_update_cccd(p_cmd->param, true);
		break;
	case RTK_BT_GATTC_ACT_CCCD_DISABLE:
		ret = bt_stack_gattc_update_cccd(p_cmd->param, false);
		break;
	default:
		printf("bt_stack_le_act_handle:unknown act: %d \r\n", p_cmd->act);
		ret = RTK_BT_ERR_NO_CASE_ELEMENT;
		break;
	}

end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}

#if (UPPER_STACK_VERSION == VERSION_2021) && F_BT_ATT_READ_MULTIPLE_VARIABLE
T_APP_RESULT bt_stack_gattc_multiple_read_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
	int i = 0;
	uint16_t conn_handle = le_get_conn_handle(conn_id);
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = &gattc_priv->request_queue[conn_id];
	rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
	rtk_bt_gattc_read_ind_t read_ind_temp = {0};
	rtk_bt_evt_t *p_evt = NULL;
	T_APP_RESULT ret = APP_RESULT_SUCCESS;
	T_CLIENT_APP_CB_DATA *cb_data = (T_CLIENT_APP_CB_DATA *)p_data;

	if (client_id != CLIENT_PROFILE_GENERAL_ID ||
		cb_data->cb_type != CLIENT_APP_CB_TYPE_READ_MULTI_RESULT) {
		return APP_RESULT_REJECT;
	}

	T_READ_MULTI_RESULT_CB_DATA *read_mul_data =
		&cb_data->cb_content.read_multi_result_data;
	uint16_t cause = read_mul_data->cause;
	uint16_t res_num_handle = read_mul_data->num_handle;

	p_req = remove_sent_req(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_READ_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		ret = APP_RESULT_REJECT;
		goto end;
	}

	if (p_req->read_param.type != RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE ||
		p_req->read_param.multiple_variable.handle_count != res_num_handle ||
		p_req->read_param.conn_handle != conn_handle) {
		ret = APP_RESULT_REJECT;
		goto end;
	}

	read_ind_temp.profile_id = p_req->read_param.profile_id;
	read_ind_temp.conn_handle = conn_handle;
	read_ind_temp.type = p_req->read_param.type;
	read_ind_temp.status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_CONTINUE;
	read_ind_temp.err_code = cause;

	for (i = 0; i < res_num_handle; i++) {
		if (p_req->read_param.multiple_variable.handles[i] != read_mul_data->p_read_tbl[i].handle) {
			ret = APP_RESULT_REJECT;
			goto end;
		}
	}

	if (cause) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t));
		p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
		memcpy(p_read_ind, &read_ind_temp, sizeof(rtk_bt_gattc_read_ind_t));
		p_read_ind->multiple_variable_per.len = 0;
		p_read_ind->multiple_variable_per.value = NULL;

		rtk_bt_evt_indicate(p_evt, NULL);
	} else {
		for (i = 0; i < res_num_handle; i++) {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_READ_RESULT_IND,
										sizeof(rtk_bt_gattc_read_ind_t));
			p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
			memcpy(p_read_ind, &read_ind_temp, sizeof(rtk_bt_gattc_read_ind_t));
			p_read_ind->multiple_variable_per.len = read_mul_data->p_read_tbl[i].read_len;
			// p_read_ind->multiple_variable_per.value = read_mul_data->p_read_tbl[i].p_data;
			p_read_ind->multiple_variable_per.value = (uint8_t *)osif_mem_alloc(
						RAM_TYPE_DATA_ON,
						p_read_ind->multiple_variable_per.len);
			memcpy(p_read_ind->multiple_variable_per.value,
				   read_mul_data->p_read_tbl[i].p_data, p_read_ind->multiple_variable_per.len);
			p_evt->user_data = p_read_ind->multiple_variable_per.value;
			rtk_bt_evt_indicate(p_evt, NULL);
		}
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t));
		p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
		memcpy(p_read_ind, &read_ind_temp, sizeof(rtk_bt_gattc_read_ind_t));
		p_read_ind->status = RTK_BT_STATUS_DONE;
		rtk_bt_evt_indicate(p_evt, NULL);
	}

end:
	bt_stack_gattc_free_req(p_req);
	bt_stack_gattc_handle_pending_req(conn_id);
	return ret;
}
#endif  /* UPPER_STACK_VERSION == VERSION_2021 */

uint16_t bt_stack_gattc_init(rtk_bt_app_conf_t *app_conf)
{
	(void)app_conf;
	uint16_t ret = 0;
	uint32_t i = 0;
	gattc_priv = (rtk_bt_gattc_app_priv_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_gattc_app_priv_t));

	if (!gattc_priv) {
		ret = RTK_BT_ERR_NO_MEMORY;
		goto failed;
	}

	memset(gattc_priv, 0, sizeof(rtk_bt_gattc_app_priv_t));

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		bt_stack_gattc_queue_init(&gattc_priv->request_queue[i]);
		bt_stack_gattc_queue_init(&gattc_priv->cmd_queue[i]);
		
		INIT_LIST_HEAD(&gattc_priv->cccd_record_list[i]);
	}

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	if (app_conf->app_profile_support & RTK_BT_PROFILE_MESH) {
		client_init(2 + MESH_GATT_CLIENT_COUNT);  // regist prov client for Remote Provisioning Server when execute remote provisioning process as mesh device role
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
		prov_client_add(bt_stack_mesh_client_callback);
		proxy_client_add(bt_stack_mesh_client_callback);
#endif
	} else {
		client_init(2);  //set the default value
	}
#else
	client_init(2);
#endif

	/* Register common GATT client */
	if (false == client_register_spec_client_cb(&gattc_common_id, &gattc_common_cb)) {
		gattc_common_id = CLIENT_PROFILE_GENERAL_ID;
		ret = RTK_BT_ERR_LOWER_STACK_API;
		goto failed;
	}

	/* Register GATT client for CCCD ops, use another client because
	 * we can avoid Repeated judgment for SUBS req and others.
	 */
	if (false == client_register_spec_client_cb(&gattc_cccd_id, &gattc_cccd_cb)) {
		gattc_cccd_id = CLIENT_PROFILE_GENERAL_ID;
		ret = RTK_BT_ERR_LOWER_STACK_API;
		goto failed;
	}

#if (UPPER_STACK_VERSION == VERSION_2021) && F_BT_ATT_READ_MULTIPLE_VARIABLE
	client_register_general_client_cb(bt_stack_gattc_multiple_read_callback);
#endif

	return 0;

failed:
	if (gattc_priv) {
		osif_mem_free(gattc_priv);
	}
	gattc_priv = NULL;
	return ret;
}

void  bt_stack_gattc_deinit(void)
{
	uint8_t i = 0;

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		bt_stack_gattc_clear_all_cccd_recorder(i);
		bt_stack_gattc_queue_clear_all(&gattc_priv->request_queue[i]);
		bt_stack_gattc_queue_clear_all(&gattc_priv->cmd_queue[i]);
	}

	osif_mem_free(gattc_priv);
	gattc_priv = NULL;
}
#endif /* #if RTK_BLE_MGR_LIB */