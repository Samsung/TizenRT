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
		if (p_notify->notify) {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_NOTIFY_IND,
										sizeof(rtk_bt_gattc_cccd_value_ind_t) + p_notify->value_size);
		} else {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_INDICATE_IND,
										sizeof(rtk_bt_gattc_cccd_value_ind_t) + p_notify->value_size);
		}

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
		/* confirm need to call rtk_bt_gattc_confirm when receive indication in user app callback */
		ret = APP_RESULT_PENDING;
		break;
	}

	case GATT_CLIENT_EVENT_CCCD_CFG: {
		T_GATT_CLIENT_CCCD_CFG *p_cccd = (T_GATT_CLIENT_CCCD_CFG *)p_data;
		rtk_bt_gattc_cccd_update_ind_t *p_cccd_ind = NULL;
		if (p_cccd->cccd_data) {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_CCCD_ENABLE_IND,
										sizeof(rtk_bt_gattc_cccd_update_ind_t));
			p_cccd_ind = (rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;
			if (p_cccd->cccd_data & GATT_CLIENT_CONFIG_NOTIFICATION) {
				p_cccd_ind->bnotify = true;
			}
			if (p_cccd->cccd_data & GATT_CLIENT_CONFIG_INDICATION) {
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
			memcpy(&p_cccd_ind->uuid, &p_cccd->uuid, sizeof(rtk_bt_gattc_uuid_t));
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
		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			rtk_bt_event_free(p_evt);
		}
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

static uint16_t bt_stack_gattc_unregister_profile(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_gattc_register_t *reg_param = (rtk_bt_gattc_register_t *)param;
	int i;

	for (i = 0; i < MAX_SPEC_SERVICE_NUM; i++) {
		if (bt_statck_srv_cb[i].cb && bt_statck_srv_cb[i].id == reg_param->profile_id) {
			break;
		}
	}

	if (i == MAX_SPEC_SERVICE_NUM) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	cause = gatt_client_spec_register((T_ATTR_UUID *)&reg_param->srv_uuid, NULL);
	if (cause) {
		printf("gatt_client_spec_register() cause = %d\r\n", cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	bt_statck_srv_cb[i].cb = NULL;

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

				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					rtk_bt_event_free(p_evt);
				}
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
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				rtk_bt_event_free(p_evt);
			}
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

static uint16_t bt_stack_gattc_confirm(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_gattc_cfm_param_t *p_cfm_param = (rtk_bt_gattc_cfm_param_t *)param;

	cause = gatt_client_ind_confirm(p_cfm_param->conn_handle, p_cfm_param->cid ? p_cfm_param->cid : GATT_FIXED_L2CAP_CID); /* just in case APP forget set cid */

	if (cause) {
		printf("bt_stack_gattc_confirm() cause = %d\r\n", cause);
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
	case RTK_BT_GATTC_ACT_UNREGISTER_PROFILE:
		ret = bt_stack_gattc_unregister_profile(p_cmd->param);
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
	case RTK_BT_GATTC_ACT_CONFIRM:
		ret = bt_stack_gattc_confirm(p_cmd->param);
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
	(void)app_conf;
#if GATTC_TBL_STORAGE_SUPPORT
	gattc_tbl_storage_init(); //store peripheral service information to ftl
#endif

#if RTK_BT_5_2_EATT_SUPPORT && F_BT_5_2_EATT_SUPPORT
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
#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
#include "mesh_api.h"
#endif

static void bt_stack_gattc_discover_state_cb(uint8_t conn_id, T_DISCOVERY_STATE discovery_state);
static void bt_stack_gattc_discover_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE res_type, T_DISCOVERY_RESULT_DATA res_data);
static void bt_stack_gattc_read_result_cb(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value);
static void bt_stack_gattc_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);
static void bt_stack_gattc_disconnect_cb(uint8_t conn_id);

static T_APP_RESULT bt_stack_gattc_cccd_notify_indicate_cb(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value);
static void bt_stack_gattc_cccd_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);

#define RTK_BT_GATTC_ENABLE_CCCD 	BIT15
#define RTK_BT_GATTC_INDICATE 		BIT1
#define RTK_BT_GATTC_NOTIFY 		BIT0

static rtk_bt_gattc_app_priv_t *g_rtk_bt_gattc_priv = NULL;
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
static rtk_bt_gattc_req_t *bt_stack_gattc_create_req(
	rtk_bt_gattc_req_type_t req_type,
	void *param, uint16_t param_size)
{
	uint32_t handle_count = 0;
	uint16_t data_len = 0;
	rtk_bt_gattc_req_t *p_req = (rtk_bt_gattc_req_t *)osif_mem_alloc(
									RAM_TYPE_DATA_ON,
									sizeof(rtk_bt_gattc_req_t));
	if (!p_req) {
		return NULL;
	}

	memset(p_req, 0, sizeof(rtk_bt_gattc_req_t));
	p_req->req_type = req_type;
	memcpy(&p_req->common_param, param, param_size);

	/* some params have 2nd level ptr, need to malloc memory and deep copy */
	if (BT_STACK_GATTC_READ_REQ == p_req->req_type &&
		(RTK_BT_GATT_CHAR_READ_MULTIPLE == p_req->read_param.type ||
		 RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE == p_req->read_param.type)) {
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

	} else if (BT_STACK_GATTC_WRITE_REQ == p_req->req_type ||
			   BT_STACK_GATTC_WRITE_CMD == p_req->req_type) {
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

static uint16_t bt_stack_gattc_queue_init(rtk_bt_gatt_queue_t *p_queue)
{
	INIT_LIST_HEAD(&p_queue->pending_list);
	p_queue->pending_ele_num = 0;
	INIT_LIST_HEAD(&p_queue->sending_list);
	p_queue->sending_ele_num = 0;
	INIT_LIST_HEAD(&p_queue->prio_pending_list);
	p_queue->prio_pending_ele_num = 0;

	return 0;
}

static void bt_stack_gattc_queue_clear_profile(rtk_bt_gatt_queue_t *p_queue, uint16_t profile_id)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	struct list_head *plist = NULL;

	//delete node of pending list which is waiting to send
	if (!list_empty(&p_queue->pending_list)) {
		plist = p_queue->pending_list.next;
		while (plist != &p_queue->pending_list) {
			p_req = (rtk_bt_gattc_req_t *)plist;
			plist = plist->next;
			if (p_req->common_param.profile_id == profile_id) {
				list_del(&p_req->list);
				bt_stack_gattc_free_req(p_req);
				p_queue->pending_ele_num --;
			}
		}
	}

	//delete node of sending list which has been send but not trigger callback
	if (!list_empty(&p_queue->sending_list)) {
		plist = p_queue->sending_list.next;
		while (plist != &p_queue->sending_list) {
			p_req = (rtk_bt_gattc_req_t *)plist;
			plist = plist->next;
			if (p_req->common_param.profile_id == profile_id) {
				list_del(&p_req->list);
				bt_stack_gattc_free_req(p_req);
				p_queue->sending_ele_num --;
			}
		}
	}

	if (!list_empty(&p_queue->prio_pending_list)) {
		plist = p_queue->prio_pending_list.next;
		while (plist != &p_queue->prio_pending_list) {
			p_req = (rtk_bt_gattc_req_t *)plist;
			plist = plist->next;
			if (p_req->common_param.profile_id == profile_id) {
				list_del(&p_req->list);
				bt_stack_gattc_free_req(p_req);
				p_queue->prio_pending_ele_num--;
			}
		}
	}
}

static void bt_stack_gattc_queue_clear_all(rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	struct list_head *plist = NULL;

	//delete node of pending list which is waiting to send
	if (!list_empty(&p_queue->pending_list)) {
		plist = p_queue->pending_list.next;
		while (plist != &p_queue->pending_list) {
			p_req = (rtk_bt_gattc_req_t *)plist;
			plist = plist->next;
			list_del(&p_req->list);
			bt_stack_gattc_free_req(p_req);
		}
		p_queue->pending_ele_num = 0;
	}

	//delete node of sending list which has been send but not trigger callback
	if (!list_empty(&p_queue->sending_list)) {
		plist = p_queue->sending_list.next;
		while (plist != &p_queue->sending_list) {
			p_req = (rtk_bt_gattc_req_t *)plist;
			plist = plist->next;
			list_del(&p_req->list);
			bt_stack_gattc_free_req(p_req);
		}
		p_queue->sending_ele_num = 0;
	}

	if (!list_empty(&p_queue->prio_pending_list)) {
		plist = p_queue->prio_pending_list.next;
		while (plist != &p_queue->prio_pending_list) {
			p_req = (rtk_bt_gattc_req_t *)plist;
			plist = plist->next;
			list_del(&p_req->list);
			bt_stack_gattc_free_req(p_req);
		}
		p_queue->prio_pending_ele_num = 0;
	}
}

static bool bt_stack_gattc_check_sending_queue_empty(rtk_bt_gatt_queue_t *p_queue)
{
	bool bsending_queue_empty = false;

	if (list_empty(&p_queue->sending_list)) {
		bsending_queue_empty = true;
	}

	return bsending_queue_empty;
}

static uint16_t bt_stack_gattc_add_req_in_queue(rtk_bt_gatt_queue_t *p_queue,
		rtk_bt_gattc_req_t *p_req,
		gatt_queue_list_type_t list_type)
{
	if (BT_STACK_SENDING_LIST == list_type) {
		list_add_tail(&p_req->list, &p_queue->sending_list);
		p_queue->sending_ele_num++;
	} else if (BT_STACK_PENDING_LIST == list_type) {
		/* The number of element in pending queue should be limited, otherwise
		 the write and read request of high frequnce will use up memory */
		if (p_queue->pending_ele_num >= BT_QUEUE_PENDING_ELEMENT_MAX) {
			printf("Error: GATTC pending queue full, wait a moment to send data again !!!\r\n");
			return RTK_BT_ERR_QUEUE_FULL;
		}
		list_add_tail(&p_req->list, &p_queue->pending_list);
		p_queue->pending_ele_num++;
		API_PRINT("Pending ele num+++++, num: %d\r\n", p_queue->pending_ele_num);
	} else if (BT_STACK_PRIO_PENDING_LIST == list_type) {
		list_add_tail(&p_req->list, &p_queue->prio_pending_list);
		p_queue->prio_pending_ele_num++;
	}

	return RTK_BT_OK;
}

/**
 * @brief get first element of pending list and pick it off from list
 */
static rtk_bt_gattc_req_t *bt_stack_gattc_get_first_pending_element(
	rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	if (!list_empty(&p_queue->pending_list)) {
		p_req = (rtk_bt_gattc_req_t *)p_queue->pending_list.next;
		list_del(&p_req->list);
		p_queue->pending_ele_num--;
		API_PRINT("Pending ele num-----, num: %d\r\n", p_queue->pending_ele_num);
	}
	return p_req;
}

/**
 * @brief get first element of sending list and pick it off from list
 */
static rtk_bt_gattc_req_t *bt_stack_gattc_get_first_sending_element(
	rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gattc_req_t *p_req = NULL;

	if (!list_empty(&p_queue->sending_list)) {
		p_req = (rtk_bt_gattc_req_t *)p_queue->sending_list.next;
		list_del(&p_req->list);
		p_queue->sending_ele_num--;
	}

	return p_req;
}

static bool bt_stack_gattc_check_profile_conn_handle(
	rtk_bt_gattc_profile_t *p_profile,
	uint16_t conn_handle)
{
	bool b_conn_find = false;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return b_conn_find;
	}

	if ((p_profile->conn_handle_map & (1 << conn_id)) != 0) {
		b_conn_find = true;
	}

	return b_conn_find;
}

static rtk_bt_gattc_profile_t *bt_stack_gattc_find_profile(uint16_t profile_id)
{
	rtk_bt_gattc_profile_t *p_profile = NULL;
	struct list_head *plist = NULL;
	bool b_find = false;

	if (!list_empty(&g_rtk_bt_gattc_priv->profile_list)) {
		plist = g_rtk_bt_gattc_priv->profile_list.next;
		while (plist != &g_rtk_bt_gattc_priv->profile_list) {
			p_profile = (rtk_bt_gattc_profile_t *)plist;
			if (p_profile->profile_id == profile_id) {
				b_find = true;
				break;
			} else {
				plist = plist->next;
			}
		}
	}

	if (!b_find) {
		return NULL;
	} else {
		return p_profile;
	}
}

/**
 * @brief add an profile element to list
 */
static void bt_stack_gattc_add_profile(rtk_bt_gattc_profile_t *p_profile)
{
	list_add_tail(&p_profile->list, &g_rtk_bt_gattc_priv->profile_list);
}

/**
 * @brief delete the profile element from list but not free it
 */
static void bt_stack_gattc_delete_profile(rtk_bt_gattc_profile_t *p_profile)
{
	list_del(&p_profile->list);
	/*  free the profile outside, not here*/
}

static void bt_stack_gattc_free_profile(rtk_bt_gattc_profile_t *p_profile)
{
	osif_mem_free(p_profile);
}

static rtk_bt_gattc_cccd_record_t *bt_stack_gattc_find_cccd_recorder(uint16_t conn_handle, uint16_t profile_id, uint16_t cccd_handle)
{
	rtk_bt_gattc_cccd_record_t *p_cccd_record = NULL;
	struct list_head *plist = NULL;
	bool bfind = false;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return NULL;
	}

	if (!list_empty(&g_rtk_bt_gattc_priv->cccd_record_list[conn_id])) {
		plist = g_rtk_bt_gattc_priv->cccd_record_list[conn_id].next;
		while (plist != &g_rtk_bt_gattc_priv->cccd_record_list[conn_id]) {
			p_cccd_record = (rtk_bt_gattc_cccd_record_t *)plist;
			if ((p_cccd_record->profile_id == profile_id) && (p_cccd_record->cccd_handle == cccd_handle)) {
				bfind = true;
				break;
			}
			plist = plist->next;
		}
	}

	if (bfind) {
		return p_cccd_record;
	} else {
		return NULL;
	}
}

static bool bt_stack_gattc_cccd_record_get_profile_id(uint16_t conn_handle, uint16_t char_val_handle, bool bnotify, uint16_t *profile_id)
{
	rtk_bt_gattc_cccd_record_t *p_cccd_record = NULL;
	struct list_head *plist = NULL;
	bool bfind = false;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return bfind;
	}

	if (!list_empty(&g_rtk_bt_gattc_priv->cccd_record_list[conn_id])) {
		plist = g_rtk_bt_gattc_priv->cccd_record_list[conn_id].next;
		while (plist != &g_rtk_bt_gattc_priv->cccd_record_list[conn_id]) {
			p_cccd_record = (rtk_bt_gattc_cccd_record_t *)plist;
			// if((p_cccd_record->value_handle == value_handle) && ((p_cccd_record->bnotify == bnotify) || (p_cccd_record->bindicate == !bnotify)))
			if (p_cccd_record->char_val_handle == char_val_handle) {
				if ((p_cccd_record->cccd_value & RTK_BT_GATTC_NOTIFY) == bnotify ||
					(p_cccd_record->cccd_value & RTK_BT_GATTC_INDICATE) == !bnotify) {
					*profile_id = p_cccd_record->profile_id;
					bfind = true;
					break;
				}
			}
			plist = plist->next;
		}
	}

	if (!bfind) {
		*profile_id = 0xFFFF;
	}

	return bfind;
}

static void bt_stack_gattc_add_cccd_record(uint16_t conn_handle, rtk_bt_gattc_cccd_record_t *p_cccd_record)
{
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return;
	}

	list_add_tail(&p_cccd_record->list, &g_rtk_bt_gattc_priv->cccd_record_list[conn_id]);
}

static void bt_stack_gattc_clear_all_cccd_recorder(uint16_t conn_handle)
{
	rtk_bt_gattc_cccd_record_t *p_cccd_record = NULL;
	struct list_head *plist = NULL;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return;
	}

	if (!list_empty(&g_rtk_bt_gattc_priv->cccd_record_list[conn_id])) {
		plist = g_rtk_bt_gattc_priv->cccd_record_list[conn_id].next;
		while (plist != &g_rtk_bt_gattc_priv->cccd_record_list[conn_id]) {
			p_cccd_record = (rtk_bt_gattc_cccd_record_t *)plist;
			plist = plist->next;
			list_del(&p_cccd_record->list);
			osif_mem_free(p_cccd_record);
		}
	}
}

static void bt_stack_gattc_clear_cccd_recorder_for_profile(uint16_t conn_handle, uint16_t profile_id)
{
	rtk_bt_gattc_cccd_record_t *p_cccd_record = NULL;
	struct list_head *plist = NULL;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return;
	}

	if (!list_empty(&g_rtk_bt_gattc_priv->cccd_record_list[conn_id])) {
		plist = g_rtk_bt_gattc_priv->cccd_record_list[conn_id].next;
		while (plist != &g_rtk_bt_gattc_priv->cccd_record_list[conn_id]) {
			p_cccd_record = (rtk_bt_gattc_cccd_record_t *)plist;
			plist = plist->next;
			if (p_cccd_record->profile_id == profile_id) {
				list_del(&p_cccd_record->list);
				osif_mem_free(p_cccd_record);
			}
		}
	}

}

static uint16_t bt_stack_gattc_send_discover(rtk_bt_gattc_req_t *disc_req)
{
	rtk_bt_gattc_discover_param_t *p_disc_param = &disc_req->disc_param;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_disc_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	switch (p_disc_param->type) {
	case RTK_BT_GATT_DISCOVER_PRIMARY_ALL:
		if (GAP_CAUSE_SUCCESS != client_all_primary_srv_discovery(conn_id, gattc_common_id)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID: {
		rtk_bt_uuid_type_t uuid_type = p_disc_param->disc_primary_by_uuid.uuid_type;
		uint8_t *uuid = p_disc_param->disc_primary_by_uuid.uuid;
		switch (uuid_type) {
		case BT_UUID_TYPE_16:
			if (GAP_CAUSE_SUCCESS != client_by_uuid_srv_discovery(conn_id, gattc_common_id, *(uint16_t *)uuid)) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
			break;
		case BT_UUID_TYPE_32:
			/* TODO: */
			return RTK_BT_ERR_UNSUPPORTED;
			break;
		case BT_UUID_TYPE_128:
			if (GAP_CAUSE_SUCCESS != client_by_uuid128_srv_discovery(conn_id, gattc_common_id, uuid)) {
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
		if (GAP_CAUSE_SUCCESS != client_relationship_discovery(conn_id, gattc_common_id, start_handle, end_handle)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	}
	case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL: {
		uint16_t start_handle = p_disc_param->disc_char_all.start_handle;
		uint16_t end_handle = p_disc_param->disc_char_all.end_handle;
		if (GAP_CAUSE_SUCCESS != client_all_char_discovery(conn_id, gattc_common_id, start_handle, end_handle)) {
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
			if (GAP_CAUSE_SUCCESS != client_by_uuid_char_discovery(conn_id, gattc_common_id, start_handle, end_handle, *(uint16_t *)uuid)) {
				return RTK_BT_ERR_LOWER_STACK_API;
			}
			break;
		case BT_UUID_TYPE_32:
			/* TODO */
			return RTK_BT_ERR_UNSUPPORTED;
			break;
		case BT_UUID_TYPE_128:
			if (GAP_CAUSE_SUCCESS != client_by_uuid128_char_discovery(conn_id, gattc_common_id, start_handle, end_handle, uuid)) {
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
		if (GAP_CAUSE_SUCCESS != client_all_char_descriptor_discovery(conn_id, gattc_common_id, start_handle, end_handle)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		break;
	}
	default:
		return RTK_BT_ERR_NO_ENTRY;
		break;
	}

	if (0xFF == disc_req->intern_flag) {
		g_rtk_bt_gattc_priv->read_char_num[conn_id] = 0;
	}
	return RTK_BT_OK;

}

static uint16_t bt_stack_gattc_send_read(rtk_bt_gattc_req_t *read_req)
{
	rtk_bt_gattc_read_param_t *p_read_param = &read_req->read_param;
	uint16_t ret = 0;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_read_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	switch (p_read_param->type) {
	case RTK_BT_GATT_CHAR_READ_BY_HANDLE: {
		uint16_t handle = p_read_param->by_handle.handle;
		if (GAP_CAUSE_SUCCESS != client_attr_read(conn_id, gattc_common_id, handle)) {
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
		uint32_t handle_count = p_read_param->multiple_variable.handle_count;
		uint16_t *handles = p_read_param->multiple_variable.handles;

#if defined(F_BT_ATT_READ_MULTIPLE_VARIABLE) && F_BT_ATT_READ_MULTIPLE_VARIABLE
		if (GAP_CAUSE_SUCCESS != client_attr_read_multi_variable(conn_id, gattc_common_id, handle_count, handles)) {
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
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_write_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_REQ) {
		write_type = GATT_WRITE_TYPE_REQ;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP) {
		write_type = GATT_WRITE_TYPE_CMD;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		write_type = GATT_WRITE_TYPE_SIGNED_CMD;
	} else {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (GAP_CAUSE_SUCCESS != client_attr_write(conn_id,
			gattc_common_id,
			write_type,
			p_write_param->handle,
			p_write_param->length,
			(uint8_t *)p_write_param->data)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP ||
		p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		g_rtk_bt_stack_credits--;
	}

	return 0;
}

static uint16_t bt_stack_gattc_send_cccd(rtk_bt_gattc_req_t *write_cccd_req)
{
	uint8_t i = 0;
	rtk_bt_gattc_req_t *p_req = write_cccd_req;
	uint16_t value = p_req->writed_cccd_val & (~RTK_BT_GATTC_ENABLE_CCCD);
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_req->update_cccd_param.conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	for (i = 0; i < 3; i++) {
		if (GAP_CAUSE_SUCCESS == client_attr_write(
				conn_id,
				gattc_cccd_id, GATT_WRITE_TYPE_REQ,
				p_req->update_cccd_param.cccd_handle,
				sizeof(uint16_t), (uint8_t *)&value)) {
			return RTK_BT_OK;
		}

		osif_delay(20);
	}
	if (i >= 3) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return RTK_BT_OK;
}

typedef uint16_t (*gattc_send_func)(rtk_bt_gattc_req_t *);

static gattc_send_func bt_stack_gattc_get_send_func(rtk_bt_gattc_req_type_t req_type)
{
	gattc_send_func send_func = NULL;
	switch (req_type) {
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
	case BT_STACK_GATTC_CCCD_UPDATE_REQ:
		send_func = bt_stack_gattc_send_cccd;
		break;
	default:
		break;
	}

	return send_func;
}

static uint16_t bt_stack_gattc_cmd_send(uint16_t conn_handle,
										rtk_bt_gattc_req_t *p_gattc_req)
{
	uint16_t ret = 0;
	uint8_t conn_id;
	rtk_bt_gatt_queue_t *p_queue;
	gattc_send_func gattc_send = NULL;
	rtk_bt_gattc_req_type_t req_type = p_gattc_req->req_type;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_queue = &g_rtk_bt_gattc_priv->cmd_queue[conn_id];

	gattc_send = bt_stack_gattc_get_send_func(req_type);
	if (!gattc_send) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (g_rtk_bt_stack_credits) {
		ret = gattc_send(p_gattc_req);
		if (RTK_BT_OK == ret) {
			ret = bt_stack_gattc_add_req_in_queue(p_queue, p_gattc_req,
												  BT_STACK_SENDING_LIST);
		} else {
			return ret;
		}
	} else {
		ret = RTK_BT_ERR_NO_CREDITS;
	}

	return ret;
}

static uint16_t bt_stack_gattc_req_send_or_pend(uint16_t conn_handle,
		rtk_bt_gattc_req_t *p_gattc_req)
{
	uint16_t ret = 0;
	uint8_t conn_id;
	rtk_bt_gatt_queue_t *p_queue;
	gattc_send_func gattc_send = NULL;
	rtk_bt_gattc_req_type_t req_type = p_gattc_req->req_type;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];

	gattc_send = bt_stack_gattc_get_send_func(req_type);
	if (!gattc_send) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (bt_stack_gattc_check_sending_queue_empty(p_queue)) {
		ret = gattc_send(p_gattc_req);
		if (RTK_BT_OK == ret) {
			ret = bt_stack_gattc_add_req_in_queue(p_queue, p_gattc_req, BT_STACK_SENDING_LIST);
		} else {
			return ret;
		}
	} else {
		ret = bt_stack_gattc_add_req_in_queue(p_queue, p_gattc_req, BT_STACK_PENDING_LIST);
	}

	return ret;
}

/**
 * @brief send the gattc request or add the request to prio_pending_list，
 *        prio_pending_list has higher priority than pending_list, and when
 *        internal discover completed, prio_pending_list will be moved in
 * 		  front of pending_list, so that it can be handled first.
 */
static uint16_t bt_stack_gattc_req_send_or_pend_prio(uint16_t conn_handle,
		rtk_bt_gattc_req_t *p_gattc_req)
{
	uint16_t ret = 0;
	uint8_t conn_id;
	rtk_bt_gatt_queue_t *p_queue;
	gattc_send_func gattc_send = NULL;
	rtk_bt_gattc_req_type_t req_type = p_gattc_req->req_type;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];

	gattc_send = bt_stack_gattc_get_send_func(req_type);
	if (!gattc_send) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (bt_stack_gattc_check_sending_queue_empty(p_queue)) {
		ret = gattc_send(p_gattc_req);

		if (RTK_BT_OK == ret) {
			ret = bt_stack_gattc_add_req_in_queue(p_queue, p_gattc_req, BT_STACK_SENDING_LIST);
		} else {
			return ret;
		}
	} else {
		ret = bt_stack_gattc_add_req_in_queue(p_queue, p_gattc_req, BT_STACK_PRIO_PENDING_LIST);
	}

	return ret;
}

/**
 * @brief pick off the whole prio_pending_list and add to the head of pending_list
 */
static void bt_stack_gattc_move_prio_pending_to_pending_head(uint16_t conn_handle)
{
	uint8_t conn_id;
	rtk_bt_gatt_queue_t *p_queue;
	struct list_head *pending_list;
	struct list_head *prio_pending_list;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return;
	}

	p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
	pending_list = &p_queue->pending_list;
	prio_pending_list = &p_queue->prio_pending_list;

	if (list_empty(prio_pending_list)) {
		return;
	}

	prio_pending_list->prev->next = pending_list->next;
	pending_list->next->prev = prio_pending_list->prev;
	pending_list->next = prio_pending_list->next;
	prio_pending_list->next->prev = pending_list;
	p_queue->pending_ele_num += p_queue->prio_pending_ele_num;
	API_PRINT("Pending ele num++++prio_ele, num: %d\r\n", p_queue->pending_ele_num);

	INIT_LIST_HEAD(prio_pending_list);
	p_queue->prio_pending_ele_num = 0;
}

static void bt_stack_gattc_err_indicate(rtk_bt_gattc_req_t *p_gattc_req,
										uint16_t err_code)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_req_type_t req_type = p_gattc_req->req_type;

	if (0xFF == p_gattc_req->intern_flag) {
		req_type = BT_STACK_GATTC_READ_REQ;
	}

	switch (req_type) {
	case BT_STACK_GATTC_DISC_REQ: {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND,
									sizeof(rtk_bt_gattc_discover_ind_t));
		rtk_bt_gattc_discover_ind_t *p_disc_ind =
			(rtk_bt_gattc_discover_ind_t *)p_evt->data;
		p_disc_ind->conn_handle = p_gattc_req->disc_param.conn_handle;
		p_disc_ind->profile_id = p_gattc_req->disc_param.profile_id;
		p_disc_ind->type = p_gattc_req->disc_param.type;
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
		p_read_ind->conn_handle = p_gattc_req->read_param.conn_handle;
		p_read_ind->profile_id = p_gattc_req->read_param.profile_id;
		if (0xFF == p_gattc_req->intern_flag) {
			p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
		} else {
			p_read_ind->type = p_gattc_req->read_param.type;
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
		p_write_ind->conn_handle = p_gattc_req->write_param.conn_handle;
		p_write_ind->profile_id = p_gattc_req->write_param.profile_id;
		p_write_ind->type = p_gattc_req->write_param.type;
		p_write_ind->status = RTK_BT_STATUS_FAIL;
		p_write_ind->err_code = err_code;
		break;
	}
	case BT_STACK_GATTC_CCCD_UPDATE_REQ: {
		bool enable = p_gattc_req->writed_cccd_val & RTK_BT_GATTC_ENABLE_CCCD;
		uint8_t evt = (enable ? RTK_BT_GATTC_EVT_CCCD_ENABLE_IND :
					   RTK_BT_GATTC_EVT_CCCD_DISABLE_IND);
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									evt, sizeof(rtk_bt_gattc_write_ind_t));
		rtk_bt_gattc_cccd_update_ind_t *p_cccd_update =
			(rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;
		p_cccd_update->conn_handle = p_gattc_req->update_cccd_param.conn_handle;
		p_cccd_update->profile_id = p_gattc_req->update_cccd_param.profile_id;
		p_cccd_update->status = RTK_BT_STATUS_FAIL;
		p_cccd_update->err_code = err_code;
		break;
	}
	default:
		break;
	}
}

/**
 * @brief handle pending req in pending_list: take an req element and send it, if
 * 		  send success, add it to req sending_list, if failed, indicate to app
 */
static uint16_t bt_stack_gattc_handle_pending_req(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint8_t conn_id;
	rtk_bt_gatt_queue_t *p_req_queue;
	rtk_bt_gattc_req_t *p_pending_req = NULL;
	gattc_send_func gattc_send = NULL;
	rtk_bt_gattc_req_type_t req_type = 0;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_req_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];

	while (!list_empty(&p_req_queue->pending_list)) {
		p_pending_req = bt_stack_gattc_get_first_pending_element(p_req_queue);
		req_type = p_pending_req->req_type;
		gattc_send = bt_stack_gattc_get_send_func(req_type);
		if (!gattc_send) {
			return RTK_BT_ERR_NO_ENTRY;
		}

		ret = gattc_send(p_pending_req);
		/* if send req success, add req to sending queue and break */
		if (RTK_BT_OK == ret) {
			ret = bt_stack_gattc_add_req_in_queue(p_req_queue, p_pending_req, BT_STACK_SENDING_LIST);
			break;
		} else {
			/* if send req fail, inform app of the err, free the req,
				and go on the loop */
			bt_stack_gattc_err_indicate(p_pending_req, ret);
			bt_stack_gattc_free_req(p_pending_req);
		}
	}

	return ret;
}

static void bt_stack_gattc_discover_state_cb(uint8_t conn_id,
		T_DISCOVERY_STATE discovery_state)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_discover_ind_t *p_disc_ind = NULL;
	rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
	rtk_bt_gatt_queue_t *p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
	rtk_bt_gattc_req_t *p_req = NULL;
	uint8_t status = RTK_BT_STATUS_DONE;
	uint16_t err_code = 0;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	switch (discovery_state) {
	case DISC_STATE_SRV_DONE:
	case DISC_STATE_RELATION_DONE:
	case DISC_STATE_CHAR_DONE:
	case DISC_STATE_CHAR_UUID16_DONE:
	case DISC_STATE_CHAR_UUID128_DONE:
	case DISC_STATE_CHAR_DESCRIPTOR_DONE:
		status = RTK_BT_STATUS_DONE;
		break;
	case DISC_STATE_FAILED:
		status = RTK_BT_STATUS_FAIL;
		err_code = RTK_BT_FAIL;
		break;
	default:
		return;
	}

	p_req = bt_stack_gattc_get_first_sending_element(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_DISC_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		return;
	}

	if (0xFF != p_req->intern_flag) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND,
									sizeof(rtk_bt_gattc_discover_ind_t));
		if (!p_evt) {
			return;
		}

		p_disc_ind = (rtk_bt_gattc_discover_ind_t *)p_evt->data;
		p_disc_ind->conn_handle = conn_handle;
		p_disc_ind->profile_id = p_req->disc_param.profile_id;
		p_disc_ind->type = p_req->disc_param.type;
		p_disc_ind->status = status;
		p_disc_ind->err_code = err_code;
		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			rtk_bt_event_free(p_evt);
		}

	} else {
		/* if read_by_uuid(internal use discover and read_by_handle) failed or
			discover out nothing, indicate err here */
		if (DISC_STATE_FAILED == discovery_state ||
			0 == g_rtk_bt_gattc_priv->read_char_num[conn_id]) {
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
										RTK_BT_GATTC_EVT_READ_RESULT_IND,
										sizeof(rtk_bt_gattc_read_ind_t));
			if (!p_evt) {
				return;
			}
			p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
			p_read_ind->conn_handle = conn_handle;
			p_read_ind->profile_id = p_req->disc_param.profile_id;
			p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
			p_read_ind->status = RTK_BT_STATUS_FAIL;
			p_read_ind->err_code = RTK_BT_FAIL;
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				rtk_bt_event_free(p_evt);
			}
		} else {
			bt_stack_gattc_move_prio_pending_to_pending_head(conn_handle);
		}
	}

	bt_stack_gattc_free_req(p_req);
	bt_stack_gattc_handle_pending_req(conn_handle);
}

static void bt_stack_gattc_discover_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE res_type, T_DISCOVERY_RESULT_DATA res_data)
{
	uint16_t ret = 0;
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_discover_ind_t *p_disc_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	if (!list_empty(&p_queue->sending_list)) {
		p_req = (rtk_bt_gattc_req_t *)p_queue->sending_list.next;
	}

	if (!p_req || p_req->req_type != BT_STACK_GATTC_DISC_REQ) {
		return;
	}

	if (p_req->disc_param.type != discover_type_mapping_table[res_type]) {
		return;
	}

	/* 	if the discover internal flag is set, it is used for read_by_uuid, so,
		get the handles of the wanted uuid, and use read_by_handle to read */
	if (0xFF == p_req->intern_flag) {
		rtk_bt_gattc_read_param_t read_param = {0};
		read_param.profile_id = p_req->disc_param.profile_id;
		read_param.conn_handle = p_req->disc_param.conn_handle;
		read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
		if (DISC_RESULT_BY_UUID16_CHAR == res_type) {
			read_param.by_handle.handle = res_data.p_char_uuid16_disc_data->value_handle;
		} else if (DISC_RESULT_BY_UUID128_CHAR == res_type) {
			read_param.by_handle.handle = res_data.p_char_uuid128_disc_data->value_handle;
		}

		rtk_bt_gattc_req_t *p_gattc_req = bt_stack_gattc_create_req(
											  BT_STACK_GATTC_READ_REQ, &read_param,
											  sizeof(rtk_bt_gattc_read_param_t));
		if (!p_gattc_req) {
			return;
		}

		p_gattc_req->intern_flag = 0xFF;
		ret = bt_stack_gattc_req_send_or_pend_prio(p_req->read_param.conn_handle,
				p_gattc_req);
		if (RTK_BT_OK != ret) {
			bt_stack_gattc_free_req(p_gattc_req);
			return;
		}

		g_rtk_bt_gattc_priv->read_char_num[conn_id]++;
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

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		rtk_bt_event_free(p_evt);
	}
}

static void bt_stack_gattc_read_result_cb(uint8_t conn_id, uint16_t cause,
		uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_req = bt_stack_gattc_get_first_sending_element(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_READ_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_READ_RESULT_IND, sizeof(rtk_bt_gattc_read_ind_t));
	if (!p_evt) {
		return;
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
			return;
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

	if (0xFF == p_req->intern_flag) {
		p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
		g_rtk_bt_gattc_priv->read_char_num[conn_id]--;
	}

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		rtk_bt_event_free(p_evt);
	}

	bt_stack_gattc_free_req(p_req);

	/* if read_by_handle cb comes and read_by_uuid the latest cb comes,  send event
	   to indicate charac read is done */
	if ((0xFF == p_req->intern_flag && 0 == g_rtk_bt_gattc_priv->read_char_num[conn_id]) ||
		(0xFF != p_req->intern_flag && RTK_BT_GATT_CHAR_READ_BY_HANDLE == p_req->read_param.type && 0 == cause)) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t));
		if (!p_evt) {
			return;
		}

		p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
		p_read_ind->profile_id = p_req->read_param.profile_id;
		p_read_ind->conn_handle = conn_handle;
		if (0xFF == p_req->intern_flag) {
			p_read_ind->type = RTK_BT_GATT_CHAR_READ_BY_UUID;
		} else {
			p_read_ind->type = p_req->read_param.type;
		}
		p_read_ind->status = RTK_BT_STATUS_DONE;

		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			rtk_bt_event_free(p_evt);
		}
	}

	bt_stack_gattc_handle_pending_req(conn_handle);
}

static void bt_stack_gattc_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits)
{
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_write_ind_t *p_write_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	if (type == GATT_WRITE_TYPE_REQ) {
		p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
		p_req = bt_stack_gattc_get_first_sending_element(p_queue);
		if (!p_req || p_req->req_type != BT_STACK_GATTC_WRITE_REQ) {
			printf("Gattc cb mismatch with req type\r\n");
			return;
		}
	} else if (GATT_WRITE_TYPE_CMD == type || GATT_WRITE_TYPE_SIGNED_CMD == type) {
		p_queue = &g_rtk_bt_gattc_priv->cmd_queue[conn_id];
		g_rtk_bt_stack_credits = credits;
		p_req = bt_stack_gattc_get_first_sending_element(p_queue);
		if (!p_req || p_req->req_type != BT_STACK_GATTC_WRITE_CMD) {
			printf("Gattc cb mismatch with req type\r\n");
			return;
		}
	} else {
		printf("bt_stack_gattc_write_result_cb: wrong type \r\n");
		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_WRITE_RESULT_IND, sizeof(rtk_bt_gattc_write_ind_t));
	if (!p_evt) {
		return;
	}

	p_write_ind = (rtk_bt_gattc_write_ind_t *)p_evt->data;
	p_write_ind->profile_id = p_req->write_param.profile_id;
	p_write_ind->conn_handle = conn_handle;
	p_write_ind->type = p_req->write_param.type;
	p_write_ind->status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_DONE;
	p_write_ind->err_code = cause;
	p_write_ind->handle = handle;

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		rtk_bt_event_free(p_evt);
	}

	bt_stack_gattc_free_req(p_req);

	//handle pending list
	if (type == GATT_WRITE_TYPE_REQ) {
		bt_stack_gattc_handle_pending_req(conn_handle);
	}
}

static void bt_stack_gattc_disconnect_cb(uint8_t conn_id)
{
	rtk_bt_gattc_profile_t *p_profile = NULL;
	struct list_head *plist = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	if (!list_empty(&g_rtk_bt_gattc_priv->profile_list)) {
		plist = g_rtk_bt_gattc_priv->profile_list.next;
		while (plist != &g_rtk_bt_gattc_priv->profile_list) {
			p_profile = (rtk_bt_gattc_profile_t *)plist;
			p_profile->conn_handle_map &= (~(1U << conn_id));
			plist = plist->next;
		}
	}

	//clear conn index map
	bt_stack_gattc_clear_all_cccd_recorder(conn_handle);

	bt_stack_gattc_queue_clear_all(&g_rtk_bt_gattc_priv->cmd_queue[conn_id]);
	bt_stack_gattc_queue_clear_all(&g_rtk_bt_gattc_priv->request_queue[conn_id]);
}

static void bt_stack_gattc_cccd_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
		uint16_t handle, uint16_t cause, uint8_t credits)
{
	(void)type;
	(void)credits;
	uint16_t cccd_handle = handle;
	rtk_bt_gattc_req_t *p_req = NULL;
	rtk_bt_gatt_queue_t *p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
	bool benable_cccd = true;
	rtk_bt_gattc_cccd_record_t *p_cccd_record = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_cccd_update_ind_t *p_cccd_update_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_req = bt_stack_gattc_get_first_sending_element(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_CCCD_UPDATE_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		return;
	}

	if ((p_req->writed_cccd_val & RTK_BT_GATTC_ENABLE_CCCD) != 0) {
		benable_cccd = true;
	} else {
		benable_cccd = false;
	}

	if (benable_cccd) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_CCCD_ENABLE_IND,
									sizeof(rtk_bt_gattc_cccd_update_ind_t));
	} else {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_CCCD_DISABLE_IND,
									sizeof(rtk_bt_gattc_cccd_update_ind_t));
	}

	if (!p_evt) {
		return;
	}

	p_cccd_record = bt_stack_gattc_find_cccd_recorder(conn_handle,
					p_req->update_cccd_param.profile_id, cccd_handle);
	if (!p_cccd_record) {
		printf("Gattc cccd record can't found\r\n");
		return;
	}

	if (!cause) {
		p_cccd_record->cccd_value = p_req->writed_cccd_val & 0x3;
	}

	bt_stack_gattc_free_req(p_req);

	//indicate event
	p_cccd_update_ind = (rtk_bt_gattc_cccd_update_ind_t *)p_evt->data;

	p_cccd_update_ind->profile_id = p_req->update_cccd_param.profile_id;
	p_cccd_update_ind->conn_handle = conn_handle;
	p_cccd_update_ind->cccd_handle = cccd_handle;
	p_cccd_update_ind->bnotify = p_req->update_cccd_param.bnotify;
	p_cccd_update_ind->bindicate = p_req->update_cccd_param.bindicate;
	p_cccd_update_ind->status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_DONE;
	p_cccd_update_ind->err_code = cause;

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		rtk_bt_event_free(p_evt);
		return;
	}

	bt_stack_gattc_handle_pending_req(conn_handle);
}

static T_APP_RESULT bt_stack_gattc_cccd_notify_indicate_cb(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
	uint16_t profile_id = 0xFFFF;
	uint16_t char_val_handle = handle;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gattc_cccd_value_ind_t *p_value_ind = NULL;
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	if (false == bt_stack_gattc_cccd_record_get_profile_id(conn_handle,
			char_val_handle, notify, &profile_id)) {
		return APP_RESULT_REJECT;
	}

	if (notify) {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_NOTIFY_IND,
									sizeof(rtk_bt_gattc_cccd_value_ind_t));
	} else {
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC, RTK_BT_GATTC_EVT_INDICATE_IND,
									sizeof(rtk_bt_gattc_cccd_value_ind_t));
	}

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

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		rtk_bt_event_free(p_evt);
	}

	/* confirm need to call rtk_bt_gattc_confirm when receive indication in user app callback */
	return APP_RESULT_PENDING;
}

static uint16_t bt_stack_gattc_discover(void *param)
{
	uint16_t ret = 0;
	rtk_bt_gattc_discover_param_t *p_dis_param = (rtk_bt_gattc_discover_param_t *)param;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	rtk_bt_gattc_req_t *p_gattc_req = NULL;
	bool b_conn_find = false;

	p_profile = bt_stack_gattc_find_profile(p_dis_param->profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	b_conn_find = bt_stack_gattc_check_profile_conn_handle(p_profile,
				  p_dis_param->conn_handle);
	if (!b_conn_find) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	p_gattc_req = bt_stack_gattc_create_req(BT_STACK_GATTC_DISC_REQ,
											p_dis_param, sizeof(rtk_bt_gattc_discover_param_t));
	if (!p_gattc_req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	ret = bt_stack_gattc_req_send_or_pend(p_dis_param->conn_handle,
										  p_gattc_req);
	if (RTK_BT_OK != ret) {
		bt_stack_gattc_free_req(p_gattc_req);
		return ret;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_read(void *param)
{
	uint16_t ret = 0;
	rtk_bt_gattc_read_param_t *p_read_param = (rtk_bt_gattc_read_param_t *)param;
	bool b_conn_find = false;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	rtk_bt_gattc_req_t *p_gattc_req = NULL;

	p_profile = bt_stack_gattc_find_profile(p_read_param->profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	b_conn_find = bt_stack_gattc_check_profile_conn_handle(p_profile,
				  p_read_param->conn_handle);
	if (!b_conn_find) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

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

		p_gattc_req = bt_stack_gattc_create_req(BT_STACK_GATTC_DISC_REQ,
												&disc_param, sizeof(rtk_bt_gattc_discover_param_t));
		if (!p_gattc_req) {
			return RTK_BT_ERR_NO_MEMORY;
		}
		p_gattc_req->intern_flag = 0xFF;

		ret = bt_stack_gattc_req_send_or_pend(p_read_param->conn_handle,
											  p_gattc_req);
		if (RTK_BT_OK != ret) {
			bt_stack_gattc_free_req(p_gattc_req);
			return ret;
		}

		return RTK_BT_OK;
	}

	p_gattc_req = bt_stack_gattc_create_req(BT_STACK_GATTC_READ_REQ,
											p_read_param, sizeof(rtk_bt_gattc_read_param_t));
	if (!p_gattc_req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	ret = bt_stack_gattc_req_send_or_pend(p_read_param->conn_handle,
										  p_gattc_req);
	if (RTK_BT_OK != ret) {
		bt_stack_gattc_free_req(p_gattc_req);
		return ret;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_write(void *param)
{
	uint16_t ret = 0;
	rtk_bt_gattc_write_param_t *p_write_param = (rtk_bt_gattc_write_param_t *)param;
	bool b_conn_find = false;
	rtk_bt_gattc_req_type_t req_type = 0;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	rtk_bt_gattc_req_t *p_gattc_req = NULL;

	p_profile = bt_stack_gattc_find_profile(p_write_param->profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	b_conn_find = bt_stack_gattc_check_profile_conn_handle(p_profile,
				  p_write_param->conn_handle);
	if (!b_conn_find) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_REQ) {
		req_type = BT_STACK_GATTC_WRITE_REQ;
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP ||
			   p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		req_type = BT_STACK_GATTC_WRITE_CMD;
	}

	p_gattc_req = bt_stack_gattc_create_req(req_type, p_write_param,
											sizeof(rtk_bt_gattc_write_param_t));
	if (!p_gattc_req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_REQ) {
		ret = bt_stack_gattc_req_send_or_pend(p_write_param->conn_handle,
											  p_gattc_req);
	} else if (p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP ||
			   p_write_param->type == RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED) {
		ret = bt_stack_gattc_cmd_send(p_write_param->conn_handle,
									  p_gattc_req);
	}

	if (RTK_BT_OK != ret) {
		bt_stack_gattc_free_req(p_gattc_req);
		return ret;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_update_cccd(void *param, bool benable)
{
	uint16_t ret = 0;
	rtk_bt_gattc_update_cccd_param_t *p_cccd_param = (rtk_bt_gattc_update_cccd_param_t *)param;
	bool b_conn_find = false;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	rtk_bt_gattc_req_t *p_gattc_req = NULL;
	rtk_bt_gattc_cccd_record_t *p_cccd_record = NULL;

	p_profile = bt_stack_gattc_find_profile(p_cccd_param->profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	b_conn_find = bt_stack_gattc_check_profile_conn_handle(p_profile,
				  p_cccd_param->conn_handle);
	if (!b_conn_find) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	p_gattc_req = bt_stack_gattc_create_req(BT_STACK_GATTC_CCCD_UPDATE_REQ,
											p_cccd_param, sizeof(rtk_bt_gattc_update_cccd_param_t));
	if (!p_gattc_req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	p_cccd_record = bt_stack_gattc_find_cccd_recorder(p_cccd_param->conn_handle,
					p_cccd_param->profile_id,
					p_cccd_param->cccd_handle);
	if (p_cccd_record) {
		/* 	get last time writed cccd_value, and the real cccd_value to be writed
			is "cccd_value |= notify_bit", then the update of notify bit will not
			affect old indication bit */
		p_gattc_req->writed_cccd_val = p_cccd_record->cccd_value;
	} else {
		/* 	if p_cccd_record not exist, means it's first time set this cccd, initial
			value is 0 */
		p_gattc_req->writed_cccd_val = 0;
	}

	if (benable) {
		p_gattc_req->writed_cccd_val |= RTK_BT_GATTC_ENABLE_CCCD;
		if (p_cccd_param->bindicate) {
			p_gattc_req->writed_cccd_val |= RTK_BT_GATTC_INDICATE;
		}
		if (p_cccd_param->bnotify) {
			p_gattc_req->writed_cccd_val |= RTK_BT_GATTC_NOTIFY;
		}

	} else {
		p_gattc_req->writed_cccd_val &= (~RTK_BT_GATTC_ENABLE_CCCD);
		if (p_cccd_param->bindicate) {
			p_gattc_req->writed_cccd_val &= (~RTK_BT_GATTC_INDICATE);
		}
		if (p_cccd_param->bnotify) {
			p_gattc_req->writed_cccd_val &= (~RTK_BT_GATTC_NOTIFY);
		}
	}

	ret = bt_stack_gattc_req_send_or_pend(p_cccd_param->conn_handle, p_gattc_req);
	if (RTK_BT_OK != ret) {
		bt_stack_gattc_free_req(p_gattc_req);
		return ret;
	}
	/* if set cccd send success and it's first time set cccd, need create a cccd_record */
	if (!p_cccd_record) {
		p_cccd_record = (rtk_bt_gattc_cccd_record_t *)osif_mem_alloc(
							RAM_TYPE_DATA_ON,
							sizeof(rtk_bt_gattc_cccd_record_t));
		memset(p_cccd_record, 0, sizeof(rtk_bt_gattc_cccd_record_t));
		bt_stack_gattc_add_cccd_record(p_cccd_param->conn_handle, p_cccd_record);
		p_cccd_record->profile_id = p_cccd_param->profile_id;
		p_cccd_record->conn_handle = p_cccd_param->conn_handle;
		p_cccd_record->char_val_handle = p_cccd_param->char_val_handle;
		p_cccd_record->cccd_handle = p_cccd_param->cccd_handle;
		/* cccd_value area will be updated when cccd_write_result_cb comes */
		p_cccd_record->cccd_value = 0;
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_confirm(void *param)
{
	uint16_t ret = 0;
	uint8_t i = 0;
	rtk_bt_gattc_cfm_param_t *p_cfm_param = (rtk_bt_gattc_cfm_param_t *)param;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	bool b_conn_find = false;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_cfm_param->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_profile = bt_stack_gattc_find_profile(p_cfm_param->profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	b_conn_find = bt_stack_gattc_check_profile_conn_handle(p_profile,
				  p_cfm_param->conn_handle);
	if (!b_conn_find) {
		return RTK_BT_ERR_NO_CONNECTION;
	}

	for (i = 0; i < 3; i++) {
		if (GAP_CAUSE_SUCCESS == client_attr_ind_confirm(conn_id)) {
			ret = RTK_BT_OK;
			break;
		}
	}
	if (i >= 3) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return ret;
}

static uint16_t bt_stack_gattc_attach_conn(void *param)
{
	uint32_t attach_id = *(uint32_t *)param;
	uint16_t profile_id = RTK_BT_GET_PROFILE_ID(attach_id);
	uint16_t conn_handle = RTK_BT_GET_CONN_HANDLE(attach_id);
	rtk_bt_gattc_profile_t *p_profile = NULL;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_profile = bt_stack_gattc_find_profile(profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	p_profile->conn_handle_map |= (1 << conn_id);

	return RTK_BT_OK;
}

static uint16_t bt_stack_gattc_detach_conn(void *param)
{
	uint32_t attach_id = *(uint32_t *)param;
	uint16_t profile_id = RTK_BT_GET_PROFILE_ID(attach_id);
	uint16_t conn_handle = RTK_BT_GET_CONN_HANDLE(attach_id);
	rtk_bt_gattc_profile_t *p_profile = NULL;
	uint8_t conn_id;

	if (bt_stack_le_gap_get_conn_id(conn_handle, &conn_id) != 0) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	p_profile = bt_stack_gattc_find_profile(profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}
	p_profile->conn_handle_map &= (~(1 << conn_id));

	bt_stack_gattc_clear_all_cccd_recorder(conn_handle);

	bt_stack_gattc_queue_clear_profile(&g_rtk_bt_gattc_priv->request_queue[conn_id], profile_id);
	bt_stack_gattc_queue_clear_profile(&g_rtk_bt_gattc_priv->cmd_queue[conn_id], profile_id);

	return RTK_BT_OK;
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

static uint16_t bt_stack_gattc_register_profile(void *param)
{
	uint16_t profile_id = *(uint16_t *)param;
	rtk_bt_gattc_profile_t *p_profile = NULL;

	p_profile = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_gattc_profile_t));
	if (!p_profile) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	p_profile->profile_id = profile_id;
	p_profile->conn_handle_map = 0;
	bt_stack_gattc_add_profile(p_profile);

	return 0;
}

static uint16_t bt_stack_gattc_unregister_profile(void *param)
{
	uint16_t profile_id = *(uint16_t *)param;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	uint8_t i = 0;

	p_profile = bt_stack_gattc_find_profile(profile_id);
	if (!p_profile) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	for (i = 0; i < GAP_MAX_LINKS; i++) {
		if (((1 << i) & p_profile->conn_handle_map) != 0) {
			bt_stack_gattc_queue_clear_profile(&g_rtk_bt_gattc_priv->request_queue[i], profile_id);
			bt_stack_gattc_queue_clear_profile(&g_rtk_bt_gattc_priv->cmd_queue[i], profile_id);
			bt_stack_gattc_clear_cccd_recorder_for_profile(le_get_conn_handle(i), profile_id);
		}
	}

	bt_stack_gattc_delete_profile(p_profile);
	bt_stack_gattc_free_profile(p_profile);

	return 0;
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

	if (tx_pending_num == NULL || conn_id >= GAP_MAX_LINKS) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	//there is no pending list in cmd queue
	if (g_rtk_bt_gattc_priv) {
		*tx_pending_num = g_rtk_bt_gattc_priv->request_queue[conn_id].pending_ele_num +
						  g_rtk_bt_gattc_priv->request_queue[conn_id].prio_pending_ele_num +
						  g_rtk_bt_gattc_priv->request_queue[conn_id].sending_ele_num +
						  g_rtk_bt_gattc_priv->cmd_queue[conn_id].sending_ele_num;
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
		ret = bt_stack_gattc_register_profile(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_UNREGISTER_PROFILE:
		ret = bt_stack_gattc_unregister_profile(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_ATTACH_CONN:
		ret = bt_stack_gattc_attach_conn(p_cmd->param);
		break;
	case RTK_BT_GATTC_ACT_DETACH_CONN:
		ret = bt_stack_gattc_detach_conn(p_cmd->param);
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
	case RTK_BT_GATTC_ACT_CONFIRM:
		ret = bt_stack_gattc_confirm(p_cmd->param);
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
	rtk_bt_gatt_queue_t *p_queue = &g_rtk_bt_gattc_priv->request_queue[conn_id];
	rtk_bt_gattc_read_ind_t *p_read_ind = NULL;
	rtk_bt_gattc_read_ind_t read_ind_temp = {0};
	rtk_bt_evt_t *p_evt = NULL;
	// rtk_bt_gattc_evt_read_res_t evt_param_tmp;

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

	p_req = bt_stack_gattc_get_first_sending_element(p_queue);
	if (!p_req || p_req->req_type != BT_STACK_GATTC_READ_REQ) {
		printf("Gattc cb mismatch with req type\r\n");
		return APP_RESULT_REJECT;
	}

	if (p_req->read_param.type != RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE ||
		p_req->read_param.multiple_variable.handle_count != res_num_handle ||
		p_req->read_param.conn_handle != conn_handle) {
		return APP_RESULT_REJECT;
	}

	read_ind_temp.profile_id = p_req->read_param.profile_id;
	read_ind_temp.conn_handle = conn_handle;
	read_ind_temp.type = p_req->read_param.type;
	read_ind_temp.status = cause ? RTK_BT_STATUS_FAIL : RTK_BT_STATUS_CONTINUE;
	read_ind_temp.err_code = cause;

	for (i = 0; i < res_num_handle; i++) {
		if (p_req->read_param.multiple_variable.handles[i] != read_mul_data->p_read_tbl[i].handle) {
			return APP_RESULT_REJECT;
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

		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			rtk_bt_event_free(p_evt);
		}
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
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				rtk_bt_event_free(p_evt);
			}
		}
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTC,
									RTK_BT_GATTC_EVT_READ_RESULT_IND,
									sizeof(rtk_bt_gattc_read_ind_t));
		p_read_ind = (rtk_bt_gattc_read_ind_t *)p_evt->data;
		memcpy(p_read_ind, &read_ind_temp, sizeof(rtk_bt_gattc_read_ind_t));
		p_read_ind->status = RTK_BT_STATUS_DONE;
		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			rtk_bt_event_free(p_evt);
		}
	}

	bt_stack_gattc_handle_pending_req(conn_handle);
	return ret;
}
#endif  /* UPPER_STACK_VERSION == VERSION_2021 */

uint16_t bt_stack_gattc_init(rtk_bt_app_conf_t *app_conf)
{
	(void)app_conf;
	uint16_t ret = 0;
	uint32_t i = 0;
	g_rtk_bt_gattc_priv = (rtk_bt_gattc_app_priv_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_gattc_app_priv_t));

	if (!g_rtk_bt_gattc_priv) {
		ret = RTK_BT_ERR_NO_MEMORY;
		goto failed;
	}

	memset(g_rtk_bt_gattc_priv, 0, sizeof(rtk_bt_gattc_app_priv_t));

	INIT_LIST_HEAD(&g_rtk_bt_gattc_priv->profile_list);
	for (i = 0; i < GAP_MAX_LINKS; i++) {
		ret = bt_stack_gattc_queue_init(&g_rtk_bt_gattc_priv->request_queue[i]);
		if (RTK_BT_OK != ret) {
			goto failed;
		}

		ret = bt_stack_gattc_queue_init(&g_rtk_bt_gattc_priv->cmd_queue[i]);
		if (RTK_BT_OK != ret) {
			goto failed;
		}

		INIT_LIST_HEAD(&g_rtk_bt_gattc_priv->cccd_record_list[i]);
	}

#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
	if (app_conf->app_profile_support & RTK_BT_PROFILE_MESH) {
		client_init(2 + MESH_GATT_CLIENT_COUNT);
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
	if (g_rtk_bt_gattc_priv) {
		osif_mem_free(g_rtk_bt_gattc_priv);
	}
	g_rtk_bt_gattc_priv = NULL;
	return ret;
}

void  bt_stack_gattc_deinit(void)
{
	uint8_t i = 0;
	rtk_bt_gattc_profile_t *p_profile = NULL;
	struct list_head *plist = NULL;

	for (i = 0; i < GAP_MAX_LINKS; i++) {
		bt_stack_gattc_clear_all_cccd_recorder(i);
		bt_stack_gattc_queue_clear_all(&g_rtk_bt_gattc_priv->request_queue[i]);
		bt_stack_gattc_queue_clear_all(&g_rtk_bt_gattc_priv->cmd_queue[i]);
	}

	if (!list_empty(&g_rtk_bt_gattc_priv->profile_list)) {
		plist = g_rtk_bt_gattc_priv->profile_list.next;
		while (plist != &g_rtk_bt_gattc_priv->profile_list) {
			p_profile = (rtk_bt_gattc_profile_t *)plist;
			plist = plist->next;
			bt_stack_gattc_delete_profile(p_profile);
			bt_stack_gattc_free_profile(p_profile);
		}
	}
	osif_mem_free(g_rtk_bt_gattc_priv);
	g_rtk_bt_gattc_priv = NULL;

}
#endif /* #if RTK_BLE_MGR_LIB */