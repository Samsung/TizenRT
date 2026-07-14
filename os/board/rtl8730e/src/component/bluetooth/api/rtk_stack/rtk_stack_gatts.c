/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <osif.h>
#include <dlist.h>
#include "platform_stdlib.h"
#include "basic_types.h"

#include <rtk_bt_def.h>
#include <rtk_stack_config.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_common.h>
#include <rtk_stack_gatt.h>

#include <gap.h>
#include <gap_conn_le.h>
#include <gap_config.h>
#include <gatt_builtin_services.h>

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
#include <bt_gatt_svc.h>
#else
#include <profile_server.h>
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
rtk_bt_gatts_app_priv_t *g_rtk_bt_gatts_priv = NULL;
#else
static rtk_bt_gatts_app_priv_t *g_rtk_bt_gatts_priv = NULL;
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

static struct rtk_bt_gatt_service *bt_stack_gatts_find_service_node_by_server_id(T_SERVER_ID server_id);
static T_APP_RESULT bt_stack_gatts_attr_read_cb(
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	uint16_t conn_handle, uint16_t cid,
#else
	uint8_t conn_id,
#endif
	T_SERVER_ID service_id,
	uint16_t attrib_index, uint16_t offset,
	uint16_t *p_length, uint8_t **pp_value)
{
	(void)p_length;
	(void)pp_value;
	rtk_bt_evt_t *p_evt = NULL;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_read_ind_t *p_read_ind = NULL;
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	uint16_t conn_handle = le_get_conn_handle(conn_id);
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(service_id);
	if (!p_srv_node) {
		return APP_RESULT_REJECT;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_EVT_READ_IND, sizeof(rtk_bt_gatts_read_ind_t));

	p_read_ind = (rtk_bt_gatts_read_ind_t *)p_evt->data;
	p_read_ind->app_id = p_srv_node->app_id;
	p_read_ind->conn_handle = conn_handle;
	p_read_ind->index = attrib_index;
	p_read_ind->offset = offset;
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	p_read_ind->cid = cid;
#else
	p_read_ind->cid = L2C_FIXED_CID_ATT;
#endif

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		return APP_RESULT_REJECT;
	}

	return APP_RESULT_PENDING;
}

static T_APP_RESULT bt_stack_gatts_attr_write_cb(
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	uint16_t conn_handle, uint16_t cid,
#else
	uint8_t conn_id,
#endif
	T_SERVER_ID service_id,
	uint16_t attrib_index, T_WRITE_TYPE write_type,
	uint16_t length, uint8_t *p_value,
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	P_FUN_EXT_WRITE_IND_POST_PROC *p_write_ind_post_proc
#else
	P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc
#endif
)
{
	(void)p_write_ind_post_proc;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_write_ind_t *p_write_ind = NULL;
	rtk_bt_evt_t *p_evt = NULL;
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	uint16_t conn_handle = le_get_conn_handle(conn_id);
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(service_id);
	if (!p_srv_node) {
		return APP_RESULT_REJECT;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_EVT_WRITE_IND, sizeof(rtk_bt_gatts_write_ind_t) + length);
	if (!p_evt) {
		return APP_RESULT_REJECT;
	}

	p_write_ind = p_evt->data;

	p_write_ind->app_id = p_srv_node->app_id;
	p_write_ind->conn_handle = conn_handle;
	p_write_ind->index = attrib_index;
	p_write_ind->offset = 0;
	p_write_ind->len = length;
	if (length) {
		p_write_ind->value = (uint8_t *)p_evt->data + sizeof(rtk_bt_gatts_write_ind_t);
		memcpy(p_write_ind->value, p_value, length);
	}
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	p_write_ind->cid = cid;
#else
	p_write_ind->cid = L2C_FIXED_CID_ATT;
#endif

	switch (write_type) {
	case WRITE_REQUEST:
		p_write_ind->type = RTK_BT_GATTS_WRITE_REQ;
		break;
	case WRITE_WITHOUT_RESPONSE:
		p_write_ind->type = RTK_BT_GATTS_WRITE_NO_RESP;
		break;
	case WRITE_SIGNED_WITHOUT_RESPONSE:
		p_write_ind->type = RTK_BT_GATTS_WRITE_NO_RESP_SIGNED;
		break;
	case WRITE_LONG:
		p_write_ind->type = RTK_BT_GATTS_WRITE_LONG;
		break;
	default:
		p_write_ind->type = RTK_BT_GATTS_WRITE_REQ;
		break;
	}

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		return APP_RESULT_REJECT;
	}

	return APP_RESULT_PENDING;
}

static void bt_stack_gatts_cccd_update_cb(
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	uint16_t conn_handle, uint16_t cid,
#else
	uint8_t conn_id,
#endif
	T_SERVER_ID service_id,
	uint16_t index, uint16_t cccbits)
{
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gatts_cccd_ind_t *p_cccd_ind = NULL;
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	uint16_t conn_handle = le_get_conn_handle(conn_id);
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(service_id);
	if (!p_srv_node) {
		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_EVT_CCCD_IND, sizeof(rtk_bt_gatts_cccd_ind_t));
	if (!p_evt) {
		return;
	}

	p_cccd_ind = (rtk_bt_gatts_cccd_ind_t *)p_evt->data;

	p_cccd_ind->conn_handle = conn_handle;
	p_cccd_ind->app_id = p_srv_node->app_id;
	p_cccd_ind->index = index;
	p_cccd_ind->value = cccbits;
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	p_cccd_ind->cid = cid;
#else
	p_cccd_ind->cid = L2C_FIXED_CID_ATT;
#endif

	rtk_bt_evt_indicate(p_evt, NULL);
}

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
static const T_FUN_GATT_EXT_SERVICE_CBS rtk_bt_ext_gatts_cb =
#else
static const T_FUN_GATT_SERVICE_CBS rtk_bt_gatts_cb =
#endif
{
	bt_stack_gatts_attr_read_cb,
	bt_stack_gatts_attr_write_cb,
	bt_stack_gatts_cccd_update_cb,
};

static void bt_stack_gatts_free_srv_tbl(T_ATTRIB_APPL *rtk_srv_table, uint16_t count)
{
	if (rtk_srv_table) {
		for (uint32_t i = 0; i < count; i++) {
			if (rtk_srv_table[i].p_value_context &&
				*(uint16_t *)(rtk_srv_table[i].type_value) != GATT_UUID_INCLUDE) {
				osif_mem_free(rtk_srv_table[i].p_value_context);
			}
		}
		osif_mem_free(rtk_srv_table);
	}
}

static struct rtk_bt_gatt_service *bt_stack_gatts_find_service_node_by_app_id(uint16_t app_id)
{
	struct rtk_bt_gatt_service *p_service_node = NULL;

	list_for_each_entry(p_service_node, &g_rtk_bt_gatts_priv->service_list, list, struct rtk_bt_gatt_service) {
		if (p_service_node->app_id == app_id) {
			return p_service_node;
		}
	}

	return NULL;
}

static struct rtk_bt_gatt_service *bt_stack_gatts_find_service_node_by_server_id(T_SERVER_ID server_id)
{
	struct rtk_bt_gatt_service *p_service_node = NULL;

	list_for_each_entry(p_service_node, &g_rtk_bt_gatts_priv->service_list, list, struct rtk_bt_gatt_service) {
		if ((T_SERVER_ID)p_service_node->server_info == server_id) {
			return p_service_node;
		}
	}

	return NULL;
}

static void bt_stack_gatts_insert_service_node(struct rtk_bt_gatt_service *p_srv_node)
{
	if (!p_srv_node) {
		return;
	}

	list_add_tail(&p_srv_node->list, &g_rtk_bt_gatts_priv->service_list);
}

static void bt_stack_gatts_delete_service_node(struct rtk_bt_gatt_service *p_srv_node)
{
	if (!p_srv_node) {
		return;
	}

	list_del(&p_srv_node->list);
	if (p_srv_node->alloc_ind) {
		osif_mem_free(p_srv_node);
	}
}

static bool _indicate_data_send_compelete(bool notify, uint16_t cause, rtk_bt_gatts_req_t *req)
{
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gatts_ntf_and_ind_ind_t *p_ind = NULL;
	bool ret = true;

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,
								notify ? RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND : RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND,
								sizeof(rtk_bt_gatts_ntf_and_ind_ind_t));
	if (!p_evt) {
		ret = false;
		goto end;
	}

	p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t *)p_evt->data;

	p_ind->app_id = req->app_id;
	p_ind->conn_handle = req->conn_handle;
	p_ind->seq = req->seq;
	p_ind->index = req->index;
	p_ind->cid = req->cid;
	p_ind->err_code = cause;

	if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
		ret = false;
	}

end:
	osif_mem_free(req);
	return ret;
}

static uint16_t _send_data(bool notify, rtk_bt_gatts_req_t *req)
{
	T_GATT_PDU_TYPE type = notify ? GATT_PDU_TYPE_NOTIFICATION : GATT_PDU_TYPE_INDICATION;
	uint16_t len = req->len;
	struct rtk_bt_gatt_service *node = NULL;
	uint8_t conn_id;
	bool sent;
	uint16_t credits = 0;

	le_get_gap_param(GAP_PARAM_LE_REMAIN_CREDITS, &credits);
	if (!credits) {
		return RTK_BT_ERR_NO_CREDITS;
	}

	if (!le_get_conn_id_by_handle(req->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	node = bt_stack_gatts_find_service_node_by_app_id(req->app_id);
	if (!node) {
		return RTK_BT_ERR_NO_ENTRY;
	}

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	/* In EATT, if dynamic created L2CAP channel is used here, please make sure data_len <= L2CAP_MTU_of_this_channel - 3*/
	sent = gatt_svc_send_data(req->conn_handle, req->cid, (T_SERVER_ID)node->server_info,
							  req->index, (uint8_t *)req->data, len, type);
#else
	sent = server_send_data(conn_id, node->server_info, req->index, (uint8_t *)req->data, len, type);
#endif

	if (sent) { /* send to stack OK */
		req->flag = REQ_FLAG_ALREADY_SENT;
		return RTK_BT_OK;
	}

	return RTK_BT_ERR_LOWER_STACK_API;
}

static void _handle_indicate_pending_queue(void)
{
	/* btgap.a NOT queue indicate packets. Add packet to list then send it when no indicate is pending in stack */
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	rtk_bt_gatt_queue_t *queue, *send_q = NULL;
	rtk_bt_gatts_req_t *req;
	uint8_t i;
	uint32_t min_seq = 0xFFFFFFFF;

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		queue =  &g_rtk_bt_gatts_priv->indicate_queue[i];
		if (list_empty(&queue->pending_list)) {
			continue;
		}
		req = (rtk_bt_gatts_req_t *)queue->pending_list.next;
		if (req->flag & REQ_FLAG_ALREADY_SENT) {
			continue;
		}
		if (req->seq <= min_seq) {
			min_seq = req->seq;
			send_q = queue;
		}
	}

	if (send_q) {
		req = (rtk_bt_gatts_req_t *)send_q->pending_list.next;
		if (RTK_BT_OK != _send_data(false, req)) {
			list_del(&req->list);
			send_q->pending_ele_num--;
			_indicate_data_send_compelete(false, RTK_BT_ERR_LOWER_STACK_API, req);
		}
	}
#endif
}

static rtk_bt_gatts_req_t *bt_stack_gatts_remove_sent_req(bool notify, uint16_t conn_id, uint16_t app_id, uint16_t index)
{
	rtk_bt_gatts_req_t *req, *next;
	rtk_bt_gatt_queue_t *queue = notify ? &g_rtk_bt_gatts_priv->notify_queue[conn_id] : &g_rtk_bt_gatts_priv->indicate_queue[conn_id];

	list_for_each_entry_safe(req, next, &queue->pending_list, list, rtk_bt_gatts_req_t) {
		if ((req->app_id == app_id) && (req->index == index)) {
			list_del(&req->list);
			queue->pending_ele_num--;
			return req;
		}
	}

	return NULL;
}
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
static void bt_stack_gatts_send_data_cb(T_EXT_SEND_DATA_RESULT result);
#endif
static bool _register_service(struct rtk_bt_gatt_service *node)
{
	uint8_t *database = (uint8_t *)node->user_data;
	T_SERVER_ID *p_id = (T_SERVER_ID *)&node->server_info;
	uint16_t length = node->attr_count * sizeof(T_ATTRIB_APPL);
	bool ret;

	if (g_rtk_bt_gatts_priv->srv_registering) {
		return false;
	}

	g_rtk_bt_gatts_priv->srv_registering = 1;

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	ret = gatt_svc_add(p_id, database, length, &rtk_bt_ext_gatts_cb, bt_stack_gatts_send_data_cb);
#else
	if (true == node->assgin_handle_flag) {
		ret = server_add_service_by_start_handle(p_id, database, length,
												 rtk_bt_gatts_cb, node->start_handle);
	} else {
		ret = server_add_service(p_id, database, length, rtk_bt_gatts_cb);
	}
#endif

	if (ret == true) {
		node->register_status = SRV_REGISTERING;
	} else {
		bt_stack_gatts_free_srv_tbl((T_ATTRIB_APPL *)database, node->attr_count);
		bt_stack_gatts_delete_service_node(node);
		BT_LOGE("register service to stack failed \r\n");
		g_rtk_bt_gatts_priv->srv_registering = 0;
	}

	return ret;
}

static T_APP_RESULT _register_service_complete_cb(T_SERVER_ID srv_id, uint16_t cause)
{
	struct rtk_bt_gatt_service *node = NULL, *next;
	rtk_bt_evt_t *p_gatts_evt = NULL;
	rtk_bt_gatts_reg_ind_t *p_reg_ind = NULL;
	T_APP_RESULT ret = APP_RESULT_SUCCESS;

	node = bt_stack_gatts_find_service_node_by_server_id(srv_id);
	if (!node) {
		BT_LOGE("Cannot get the app_id from srv_id in SRV_REGISTERING srv\r\n");
		return APP_RESULT_REJECT;
	}

	if (cause) {
		BT_LOGE("result is fail: cause = 0x%x \r\n", cause);
		if (node->user_data) {
			bt_stack_gatts_free_srv_tbl(node->user_data, node->attr_count);
		}
		bt_stack_gatts_delete_service_node(node);
		ret = APP_RESULT_REJECT;
	} else {
		/* Change register status */
		node->register_status = SRV_REGISTERED;
		// BT_LOGA("result is successfully \r\n");
		ret = APP_RESULT_SUCCESS;
	}

	p_gatts_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_EVT_REGISTER_SERVICE, sizeof(rtk_bt_gatts_reg_ind_t));
	if (p_gatts_evt) {
		p_reg_ind = (rtk_bt_gatts_reg_ind_t *)p_gatts_evt->data;
		p_reg_ind->app_id = node->app_id;
		p_reg_ind->reg_status = ((ret == APP_RESULT_SUCCESS) ? RTK_BT_OK : RTK_BT_FAIL);

		rtk_bt_evt_indicate(p_gatts_evt, NULL);
	} else {
		BT_LOGE("p_gatts_evt alloc fail \r\n");
	}

	g_rtk_bt_gatts_priv->srv_registering = 0;

	list_for_each_entry_safe(node, next, &g_rtk_bt_gatts_priv->service_list, list, struct rtk_bt_gatt_service) {
		if (node->register_status == SRV_WAIT_FOR_REG) {
			if (_register_service(node)) {
				break;
			}
		}
	}
	return ret;
}

static bool _send_data_complete_cb(T_SERVER_ID server_id, uint16_t index, uint8_t conn_id, uint16_t cause, T_GATT_PDU_TYPE data_type)
{
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_req_t *req = NULL;
	bool notify = false;

	if (GATT_PDU_TYPE_NOTIFICATION == data_type) {
		notify = true;
	} else if (GATT_PDU_TYPE_INDICATION != data_type) {
		return false;
	}

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(server_id);
	if (!p_srv_node) {
		return false;
	}

	req = bt_stack_gatts_remove_sent_req(notify, conn_id, p_srv_node->app_id, index);

	if (!req || !_indicate_data_send_compelete(notify, cause, req)) {
		return false;
	}

	if (!notify) {
		_handle_indicate_pending_queue();
	}

	return true;
}

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB

static void bt_stack_gatts_general_callback(uint8_t type, void *p_data)
{
	if (type == GATT_SVC_EVENT_REG_AFTER_INIT_RESULT) {
		T_GATT_SVC_REG_AFTER_INIT_RESULT *result = (T_GATT_SVC_REG_AFTER_INIT_RESULT *)p_data;

		_register_service_complete_cb(result->service_id, result->cause);
	}
}

static void bt_stack_gatts_send_data_cb(T_EXT_SEND_DATA_RESULT result)
{
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(result.conn_handle, &conn_id)) {
		return;
	}

	_send_data_complete_cb(result.service_id, result.attrib_idx, conn_id, result.cause, result.data_type);
}

#else /* RTK_BLE_MGR_LIB */

static T_APP_RESULT bt_stack_gatts_app_service_callback(T_SERVER_ID service_id, void *data)
{
	T_APP_RESULT app_result = APP_RESULT_APP_ERR;
	T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)data;

	/* Only handle 'GENERAL ID' callback */
	if (!p_param || SERVICE_PROFILE_GENERAL_ID != service_id) {
		return APP_RESULT_APP_ERR;
	}

	if (p_param->eventId == PROFILE_EVT_SRV_REG_AFTER_INIT_COMPLETE) {
		T_SERVER_ID srv_id = p_param->event_data.server_reg_after_init_result.service_id;
		uint16_t cause = p_param->event_data.server_reg_after_init_result.cause;

		app_result = _register_service_complete_cb(srv_id, cause);

	} else if (p_param->eventId == PROFILE_EVT_SEND_DATA_COMPLETE) {
		T_SERVER_ID server_id = p_param->event_data.send_data_result.service_id;
		uint16_t index = p_param->event_data.send_data_result.attrib_idx;
		uint8_t conn_id = p_param->event_data.send_data_result.conn_id;
		uint16_t cause = p_param->event_data.send_data_result.cause;
		T_GATT_PDU_TYPE data_type = p_param->event_data.send_data_result.data_type;

		app_result = (T_APP_RESULT)_send_data_complete_cb(server_id, index, conn_id, cause, data_type);
	}

	return app_result;
}

#endif /* !RTK_BLE_MGR_LIB */

static T_APP_RESULT bt_stack_le_gap_service_callback(T_SERVER_ID service_id, void *param)
{
	(void)service_id;
	T_APP_RESULT result = APP_RESULT_SUCCESS;
	rtk_bt_evt_t *p_evt = NULL;
	T_GAPS_CALLBACK_DATA *p_gap_data = (T_GAPS_CALLBACK_DATA *)param;

	if (SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE == p_gap_data->msg_type) {
		switch (p_gap_data->msg_data.opcode) {
		case GAPS_WRITE_DEVICE_NAME: {
			T_LOCAL_NAME device_name;
			memcpy(device_name.local_name, p_gap_data->msg_data.p_value, p_gap_data->msg_data.len);
			device_name.local_name[p_gap_data->msg_data.len] = 0;
			flash_save_local_name(&device_name);
			break;
		}
		case GAPS_WRITE_APPEARANCE: {
			T_LOCAL_APPEARANCE appearance = {.local_appearance = *(uint16_t *)p_gap_data->msg_data.p_value};
			flash_save_local_appearance(&appearance);
			break;
		}
		case GATT_SERVICE_WRITE_CLIENT_SUPPORTED_FEATURES: {
			rtk_bt_gatts_client_supported_features_ind_t *p_ind = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,
										RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES,
										sizeof(rtk_bt_gatts_client_supported_features_ind_t));
			if (!p_evt) {
				break;
			}

			p_ind = (rtk_bt_gatts_client_supported_features_ind_t *)p_evt->data;
			p_ind->conn_handle = le_get_conn_handle(p_gap_data->conn_id);
#if defined(F_BT_GATT_SERVER_EXT_API) && F_BT_GATT_SERVER_EXT_API
			p_ind->cid = p_gap_data->cid;
#else
			p_ind->cid = L2C_FIXED_CID_ATT;
#endif
			p_ind->features = *(uint8_t *)p_gap_data->msg_data.p_value;

			rtk_bt_evt_indicate(p_evt, NULL);

			break;
		}
		default:
			break;
		}
	} else if (SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION == p_gap_data->msg_type) {
		if (GATT_SERVICE_CHANGE_CCCD_ENABLE == p_gap_data->msg_data.opcode ||
			GATT_SERVICE_CHANGE_CCCD_DISABLE == p_gap_data->msg_data.opcode) {
			rtk_bt_gatts_service_changed_cccd_ind_t *srv_change = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,
										RTK_BT_GATTS_EVT_SERVICE_CHANGED_CCCD_IND,
										sizeof(rtk_bt_gatts_service_changed_cccd_ind_t));
			if (!p_evt) {
				return result;
			}

			srv_change = (rtk_bt_gatts_service_changed_cccd_ind_t *)p_evt->data;
#if (defined(F_BT_GATT_SERVER_EXT_API) && F_BT_GATT_SERVER_EXT_API) && (defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB)
			srv_change->conn_handle = p_gap_data->conn_handle;
			srv_change->cid = p_gap_data->cid;
#else
			srv_change->conn_handle = le_get_conn_handle(p_gap_data->conn_id);
			srv_change->cid = L2C_FIXED_CID_ATT;
#endif
			srv_change->cccd_enable = (GATT_SERVICE_CHANGE_CCCD_ENABLE == p_gap_data->msg_data.opcode) ? true : false;

			rtk_bt_evt_indicate(p_evt, NULL);
		}
	}

	return result;
}

uint16_t  bt_stack_gatts_init(rtk_bt_app_conf_t *app_conf)
{
	uint8_t appearance_prop = GAPS_PROPERTY_WRITE_ENABLE;
	uint8_t device_name_prop = GAPS_PROPERTY_WRITE_ENABLE;
	uint8_t default_device_name[GAP_DEVICE_NAME_LEN] = "RTK_BT_DEVICE";
	uint16_t default_appearance = GAP_GATT_APPEARANCE_UNKNOWN;
	T_LOCAL_APPEARANCE appearance_local;
	T_LOCAL_NAME local_device_name;
	uint8_t svc_num;
	uint8_t i = 0;

	g_rtk_bt_gatts_priv = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_gatts_app_priv_t));
	if (!g_rtk_bt_gatts_priv) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	memset(g_rtk_bt_gatts_priv, 0, sizeof(rtk_bt_gatts_app_priv_t));

	INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->service_list);

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->notify_queue[i].pending_list);
		g_rtk_bt_gatts_priv->notify_queue[i].pending_ele_num = 0;
		INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->indicate_queue[i].pending_list);
		g_rtk_bt_gatts_priv->indicate_queue[i].pending_ele_num = 0;
	}

	if (app_conf && true == app_conf->cccd_not_check) {
		gap_config_cccd_not_check(CONFIG_GATT_CCCD_NOT_CHECK);
	}

	if (app_conf && false == app_conf->user_def_service) {
		if (flash_load_local_name(&local_device_name) == 0) {
			gaps_set_parameter(GAPS_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, local_device_name.local_name);
		} else {
			gaps_set_parameter(GAPS_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, default_device_name);
		}
		if (flash_load_local_appearance(&appearance_local) == 0) {
			gaps_set_parameter(GAPS_PARAM_APPEARANCE, sizeof(uint16_t), &appearance_local.local_appearance);
		} else {
			gaps_set_parameter(GAPS_PARAM_APPEARANCE, sizeof(uint16_t), &default_appearance);
		}

		/* Make device name and appearance writable, because these can be configured by API */
		gaps_set_parameter(GAPS_PARAM_APPEARANCE_PROPERTY, sizeof(appearance_prop), &appearance_prop);
		gaps_set_parameter(GAPS_PARAM_DEVICE_NAME_PROPERTY, sizeof(device_name_prop), &device_name_prop);

		/* Built-in service callback, including GAPS and GATTS */
		gatt_register_callback((void *)bt_stack_le_gap_service_callback);
		svc_num = RTK_BT_GATTS_SERVICE_NUM - 2;
	} else {
		server_builtin_service_reg(false);
		svc_num = RTK_BT_GATTS_SERVICE_NUM;
	}
#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	if (!gatt_svc_init(GATT_SVC_USE_EXT_SERVER, svc_num)) {
		BT_LOGE("gatt_svc_init fail \r\n");
		return RTK_BT_ERR_LOWER_STACK_API;
	}
	gatt_svc_register_general_cb(bt_stack_gatts_general_callback);
#else
	/* GATT server init */
	server_init(svc_num);
	server_register_app_cb(bt_stack_gatts_app_service_callback);
#endif
	return RTK_BT_OK;
}

void bt_stack_gatts_queue_clear_all(rtk_bt_gatt_queue_t *p_queue)
{
	rtk_bt_gatts_req_t *req, *next;

	list_for_each_entry_safe(req, next, &p_queue->pending_list, list, rtk_bt_gatts_req_t) {
		list_del(&req->list);
		osif_mem_free(req);
	}
	p_queue->pending_ele_num = 0;
}

void bt_stack_gatts_disconnect_queue_clear(uint8_t conn_id)
{
	bt_stack_gatts_queue_clear_all(&g_rtk_bt_gatts_priv->notify_queue[conn_id]);
	bt_stack_gatts_queue_clear_all(&g_rtk_bt_gatts_priv->indicate_queue[conn_id]);
}

void bt_stack_gatts_deinit(void)
{
	struct rtk_bt_gatt_service *node, *next;
	int i = 0;

	//delete service
	list_for_each_entry_safe(node, next, &g_rtk_bt_gatts_priv->service_list, list, struct rtk_bt_gatt_service) {
		bt_stack_gatts_free_srv_tbl((T_ATTRIB_APPL *)node->user_data, node->attr_count);
		bt_stack_gatts_delete_service_node(node);
	}

	//delete notify queue and indicate queue
	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		bt_stack_gatts_disconnect_queue_clear(i);
	}

	osif_mem_free(g_rtk_bt_gatts_priv);
	g_rtk_bt_gatts_priv = NULL;
}

static uint32_t bt_stack_gatts_permission_trans(uint8_t perm)
{
	uint32_t permission = 0x0000;

	/* Get all read permission */
	if (perm & RTK_BT_GATT_PERM_READ) {
		permission |= GATT_PERM_READ;
	}
	if (perm & RTK_BT_GATT_PERM_READ_ENCRYPT) {
		permission |= GATT_PERM_READ_ENCRYPTED_REQ;
	}
	if (perm & RTK_BT_GATT_PERM_READ_AUTHEN) {
		permission |= GATT_PERM_READ_AUTHEN_MITM_REQ;
	}

	/* Get all write permission */
	if (perm & RTK_BT_GATT_PERM_WRITE) {
		permission |= GATT_PERM_WRITE;
	}
	if (perm & RTK_BT_GATT_PERM_WRITE_ENCRYPT) {
		permission |= GATT_PERM_WRITE_ENCRYPTED_REQ;
	}
	if (perm & RTK_BT_GATT_PERM_WRITE_AUTHEN) {
		permission |= GATT_PERM_WRITE_AUTHEN_MITM_REQ;
	}

	return permission;
}

static uint16_t bt_stack_uuid16_common_attr_convert(rtk_bt_gatt_attr_t *app_attr, T_ATTRIB_APPL *stack_attr)
{
	uint16_t uuid16 = BT_UUID_16(app_attr->uuid)->val;

	stack_attr->flags = (RTK_BT_GATT_APP == app_attr->flag) ? ATTRIB_FLAG_VALUE_APPL : ATTRIB_FLAG_VOID;
	stack_attr->type_value[0] = LO_WORD(uuid16);
	stack_attr->type_value[1] = HI_WORD(uuid16);
	if (ATTRIB_FLAG_VALUE_APPL == stack_attr->flags) {
		stack_attr->value_len = 0;
		stack_attr->p_value_context = NULL;
		if (GATT_UUID_CHAR_CLIENT_CONFIG == uuid16) {
			stack_attr->flags |= ATTRIB_FLAG_CCCD_APPL;
		}
	} else {
		stack_attr->value_len = app_attr->len;
		stack_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, stack_attr->value_len);
		if (!stack_attr->p_value_context) {
			return RTK_BT_ERR_NO_MEMORY;
		}
		memcpy(stack_attr->p_value_context, app_attr->user_data, stack_attr->value_len);
	}
	stack_attr->permissions = bt_stack_gatts_permission_trans(app_attr->perm);

	return RTK_BT_OK;
}

static uint16_t bt_stack_uuid16_attr_convert(uint16_t gatt_type, rtk_bt_gatt_attr_t *p_app_gatt_attr, T_ATTRIB_APPL *p_stack_gatt_attr)
{
	uint16_t uuid16 = BT_UUID_16(p_app_gatt_attr->uuid)->val;

	switch (uuid16) {
	case GATT_UUID_PRIMARY_SERVICE:
	case GATT_UUID_SECONDARY_SERVICE: {
		uint8_t uuid_type = ((struct bt_uuid *)p_app_gatt_attr->user_data)->type;
		uint8_t *uuid = NULL;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->permissions = GATT_PERM_READ;
		if (BT_UUID_TYPE_16 == uuid_type) {
			uuid = (uint8_t *) & (BT_UUID_16((struct bt_uuid *)p_app_gatt_attr->user_data)->val);
			p_stack_gatt_attr->flags = (ATTRIB_FLAG_VALUE_INCL | gatt_type);
			// BT_LOGA("uuid16 case p_stack_gatt_attr->flags = 0x%x \r\n",p_stack_gatt_attr->flags);
			p_stack_gatt_attr->type_value[2] = uuid[0];
			p_stack_gatt_attr->type_value[3] = uuid[1];
			p_stack_gatt_attr->p_value_context = NULL;
			p_stack_gatt_attr->value_len = 2;
		} else if (BT_UUID_TYPE_128 == uuid_type) {
			uuid = BT_UUID_128((struct bt_uuid *)p_app_gatt_attr->user_data)->val;
			p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID | gatt_type;
			p_stack_gatt_attr->value_len = 16;
			p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
			if (!p_stack_gatt_attr->p_value_context) {
				return RTK_BT_ERR_NO_MEMORY;
			}
			memcpy(p_stack_gatt_attr->p_value_context, (void *)uuid, p_stack_gatt_attr->value_len);
		}
		break;
	}

	case GATT_UUID_INCLUDE: {
		struct rtk_bt_gatt_service *p_incl_srv = (struct rtk_bt_gatt_service *)(p_app_gatt_attr->user_data);
		struct rtk_bt_gatt_service *p_gatts_srv_node = NULL;
		T_ATTRIB_APPL *p_bt_stack_attr_tbl = NULL;
		p_gatts_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_incl_srv->app_id);
		if (!p_gatts_srv_node) {
			// BT_LOGE("include srv is null case \r\n");
			p_gatts_srv_node = (struct rtk_bt_gatt_service *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_gatt_service));
			if (!p_gatts_srv_node) {
				return RTK_BT_ERR_NO_MEMORY;
			}
			memset(p_gatts_srv_node, 0, sizeof(struct rtk_bt_gatt_service));
			p_gatts_srv_node->alloc_ind = 1;
			p_bt_stack_attr_tbl = (T_ATTRIB_APPL *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_incl_srv->attr_count * sizeof(T_ATTRIB_APPL));
			if (!p_bt_stack_attr_tbl) {
				osif_mem_free(p_gatts_srv_node);
				return RTK_BT_ERR_NO_MEMORY;
			}

			// BT_LOGA("p_incl_srv->attr_count: %d \r\n",p_incl_srv->attr_count);
			memset(p_bt_stack_attr_tbl, 0, p_incl_srv->attr_count * sizeof(T_ATTRIB_APPL));
			p_gatts_srv_node->app_id = p_incl_srv->app_id;
			p_gatts_srv_node->register_status = SRV_ALLOC_FOR_INCLUDE;
			p_gatts_srv_node->user_data = p_bt_stack_attr_tbl;
			p_gatts_srv_node->attr_count = p_incl_srv->attr_count;
			bt_stack_gatts_insert_service_node(p_gatts_srv_node);
		} else {
			p_bt_stack_attr_tbl = p_gatts_srv_node->user_data;
		}
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->value_len = 0;
		p_stack_gatt_attr->p_value_context = p_bt_stack_attr_tbl;
		p_stack_gatt_attr->permissions = GATT_PERM_READ;
		break;
	}

	case GATT_UUID_CHARACTERISTIC: {
		struct rtk_bt_gatt_chrc *char_attr_val = (struct rtk_bt_gatt_chrc *)p_app_gatt_attr->user_data;
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->type_value[2] = char_attr_val->properties;
		p_stack_gatt_attr->value_len = 1;
		p_stack_gatt_attr->p_value_context = NULL;
		p_stack_gatt_attr->permissions = GATT_PERM_READ;
		break;
	}

	case GATT_UUID_CHAR_EXTENDED_PROP: {
		if (RTK_BT_GATT_APP == p_app_gatt_attr->flag || RTK_BT_GATT_VOID == p_app_gatt_attr->flag) {
			return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		}
		struct rtk_bt_gatt_cep *cep_attr_val = (struct rtk_bt_gatt_cep *)p_app_gatt_attr->user_data;
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->type_value[2] = LO_WORD(cep_attr_val->properties);
		p_stack_gatt_attr->type_value[3] = HI_WORD(cep_attr_val->properties);
		p_stack_gatt_attr->value_len = 2;
		p_stack_gatt_attr->p_value_context = NULL;
		p_stack_gatt_attr->permissions = GATT_PERM_READ;
		break;
	}

	case GATT_UUID_CHAR_USER_DESCR: {
		if (RTK_BT_GATT_APP == p_app_gatt_attr->flag || RTK_BT_GATT_VOID == p_app_gatt_attr->flag) {
			return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		}
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->value_len = strlen((const char *)p_app_gatt_attr->user_data);
		if (!p_stack_gatt_attr->value_len) {
			p_stack_gatt_attr->p_value_context = NULL;
		} else {
			p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
			if (!p_stack_gatt_attr->p_value_context) {
				return RTK_BT_ERR_NO_MEMORY;
			}
			memcpy(p_stack_gatt_attr->p_value_context, p_app_gatt_attr->user_data, p_stack_gatt_attr->value_len);
		}
		p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
		break;
	}

	case GATT_UUID_CHAR_CLIENT_CONFIG: {
		if (RTK_BT_GATT_APP == p_app_gatt_attr->flag || RTK_BT_GATT_VOID == p_app_gatt_attr->flag) {
			return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		}
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;//ATTRIB_FLAG_CCCD_NO_FILTER
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->type_value[2] = 0;
		p_stack_gatt_attr->type_value[3] = 0;
		p_stack_gatt_attr->value_len = 2;
		p_stack_gatt_attr->p_value_context = NULL;
		p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
		/* Force readable and writable, because Core_spec_v5.2 p1556 Table 3.10 defines it. */
		if ((p_stack_gatt_attr->permissions & 0x0f) == 0) {
			p_stack_gatt_attr->permissions |= GATT_PERM_READ;
		}
		if ((p_stack_gatt_attr->permissions & 0xf0) == 0) {
			p_stack_gatt_attr->permissions |= GATT_PERM_WRITE;
		}
		break;
	}

	case GATT_UUID_CHAR_SERVER_CONFIG: {
		if (RTK_BT_GATT_APP == p_app_gatt_attr->flag || RTK_BT_GATT_VOID == p_app_gatt_attr->flag) {
			return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		}
		struct rtk_bt_gatt_scc *scc_attr_val = (struct rtk_bt_gatt_scc *)p_app_gatt_attr->user_data;
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->type_value[2] = LO_WORD(scc_attr_val->flags);
		p_stack_gatt_attr->type_value[3] = LO_WORD(scc_attr_val->flags);
		p_stack_gatt_attr->value_len = 2;
		p_stack_gatt_attr->p_value_context = NULL;
		p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
		/* Force readable and writable, because Core_spec_v5.2 p1557 Table 3.12 defines it. */
		if ((p_stack_gatt_attr->permissions & 0x0f) == 0) {
			p_stack_gatt_attr->permissions |= GATT_PERM_READ;
		}
		if ((p_stack_gatt_attr->permissions & 0xf0) == 0) {
			p_stack_gatt_attr->permissions |= GATT_PERM_WRITE;
		}
		break;
	}

	case GATT_UUID_CHAR_FORMAT: {
		if (RTK_BT_GATT_APP == p_app_gatt_attr->flag || RTK_BT_GATT_VOID == p_app_gatt_attr->flag) {
			return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		}
		struct rtk_bt_gatt_cpf *format = (struct rtk_bt_gatt_cpf *)p_app_gatt_attr->user_data;
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->type_value[2] = format->format;
		p_stack_gatt_attr->type_value[3] = format->exponent;
		p_stack_gatt_attr->type_value[4] = ((format->unit >> 0) & 0xff);
		p_stack_gatt_attr->type_value[5] = ((format->unit >> 8) & 0xff);
		p_stack_gatt_attr->type_value[6] = format->name_space;
		p_stack_gatt_attr->type_value[7] = ((format->description >> 0) & 0xff);
		p_stack_gatt_attr->type_value[8] = ((format->description >> 8) & 0xff);
		p_stack_gatt_attr->p_value_context = NULL;
		p_stack_gatt_attr->value_len = 7;
		p_stack_gatt_attr->permissions = GATT_PERM_READ;
		break;
	}

	case GATT_UUID_CHAR_AGGR_FORMAT: {
		if (RTK_BT_GATT_APP == p_app_gatt_attr->flag || RTK_BT_GATT_VOID == p_app_gatt_attr->flag) {
			return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		}
		struct rtk_bt_gatt_caf *agg_format = (struct rtk_bt_gatt_caf *)p_app_gatt_attr->user_data;
		p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID;
		p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
		p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
		p_stack_gatt_attr->value_len = agg_format->count * 2;
		if (!p_stack_gatt_attr->value_len) {
			p_stack_gatt_attr->p_value_context = NULL;
		} else {
			p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
			if (!p_stack_gatt_attr->p_value_context) {
				return RTK_BT_ERR_NO_MEMORY;
			}
			memcpy(p_stack_gatt_attr->p_value_context, (void *)agg_format->handles, p_stack_gatt_attr->value_len);
		}
		p_stack_gatt_attr->permissions = GATT_PERM_READ;
		break;
	}

	default: {
		return bt_stack_uuid16_common_attr_convert(p_app_gatt_attr, p_stack_gatt_attr);
		break;
	}
	}

	return RTK_BT_OK;
}

static uint16_t bt_stack_uuid128_attr_convert(uint16_t gatt_type, rtk_bt_gatt_attr_t *app_attr, T_ATTRIB_APPL *attr)
{
	(void)gatt_type;
	uint8_t *uuid128 = BT_UUID_128(app_attr->uuid)->val;

	attr->flags = ATTRIB_FLAG_UUID_128BIT | ATTRIB_FLAG_VALUE_APPL;
	memcpy(attr->type_value, (void *)uuid128, 16);
	attr->value_len = 0;
	attr->p_value_context = NULL;
	attr->permissions = bt_stack_gatts_permission_trans(app_attr->perm);

	return 0;
}

static uint16_t bt_stack_gatts_register_service(void  *p_gatts_srv)
{
	struct rtk_bt_gatt_service *app_srv = (struct rtk_bt_gatt_service *)p_gatts_srv;
	struct rtk_bt_gatt_service *node = NULL;
	T_ATTRIB_APPL *attrs = NULL;
	uint16_t gatt_type = 0;
	rtk_bt_gatt_attr_t *app_attr = NULL;
	T_ATTRIB_APPL *attr = NULL;
	uint16_t ret = 0;
	uint32_t i = 0;

	app_srv->alloc_ind = 0;
	gatt_type = (GATT_SERVICE_OVER_BLE == app_srv->type) ? ATTRIB_FLAG_LE : ATTRIB_FLAG_BREDR;

	node = bt_stack_gatts_find_service_node_by_app_id(app_srv->app_id);

	if (node) {
		if (node->register_status > SRV_ALLOC_FOR_INCLUDE) {
			return RTK_BT_ERR_ALREADY_IN_PROGRESS;
		}
	}

	if (!node) {
		attrs = (T_ATTRIB_APPL *)osif_mem_alloc(RAM_TYPE_DATA_ON, app_srv->attr_count * sizeof(T_ATTRIB_APPL));
		if (!attrs) {
			return RTK_BT_ERR_NO_MEMORY;
		}

		app_srv->user_data = (void *)attrs;
		bt_stack_gatts_insert_service_node(app_srv);

	} else {
		if (node->attr_count < app_srv->attr_count) {
			return RTK_BT_ERR_MISMATCH;
		}

		attrs = (T_ATTRIB_APPL *)node->user_data;
		app_srv->user_data = attrs;
		bt_stack_gatts_delete_service_node(node);
		bt_stack_gatts_insert_service_node(app_srv);
	}

	memset(attrs, 0, app_srv->attr_count * sizeof(T_ATTRIB_APPL));
	for (i = 0; i < app_srv->attr_count; i++) {
		app_attr = &app_srv->attrs[i];
		attr = &attrs[i];
		if (BT_UUID_TYPE_16 == app_attr->uuid->type) {
			ret = bt_stack_uuid16_attr_convert(gatt_type, app_attr, attr);
		} else if (BT_UUID_TYPE_128 == app_attr->uuid->type) {
			ret = bt_stack_uuid128_attr_convert(gatt_type, app_attr, attr);
		} else {
			BT_LOGE("unknown uuid type: 0x%x \r\n", app_attr->uuid->type);
		}
		if (ret) {
			return ret;
		}
	}

	if (g_rtk_bt_gatts_priv->srv_registering) {
		app_srv->register_status = SRV_WAIT_FOR_REG;
	} else {
		if (!_register_service(app_srv)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}

	return 0;
}

void bt_stack_gatts_evt_indicate_mtu_exchange(uint16_t conn_handle, uint16_t mtu)
{
	rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_exchange_ind = NULL;
	rtk_bt_evt_t *p_evt_t = NULL;

	p_evt_t = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_EVT_MTU_EXCHANGE, sizeof(rtk_bt_gatt_mtu_exchange_ind_t));
	if (!p_evt_t) {
		return;
	}

	p_gatt_mtu_exchange_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)p_evt_t->data;
	p_gatt_mtu_exchange_ind->result = RTK_BT_OK;
	p_gatt_mtu_exchange_ind->conn_handle = conn_handle;
	p_gatt_mtu_exchange_ind->mtu_size = mtu;

	rtk_bt_evt_indicate(p_evt_t, NULL);
}

/* convert ATT error to T_APP_RESULT which is used in rtk ble stack */
static T_APP_RESULT att_err_to_app_result(uint8_t att_err)
{
	return att_err ? (ATT_ERR | att_err) : APP_RESULT_SUCCESS;
}

static uint16_t bt_stack_gatts_read_rsp(void *param)
{
	rtk_bt_gatts_read_resp_param_t *p_read_rsp = (rtk_bt_gatts_read_resp_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	T_APP_RESULT cause = APP_RESULT_SUCCESS;
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	uint8_t conn_id;
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_read_rsp->app_id);
	if (!p_srv_node) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	cause = att_err_to_app_result(p_read_rsp->err_code);

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	if (!gatt_svc_read_confirm(p_read_rsp->conn_handle,
							   p_read_rsp->cid ? p_read_rsp->cid : L2C_FIXED_CID_ATT, /* just in case APP forget set cid */
							   (T_SERVER_ID)p_srv_node->server_info,
							   p_read_rsp->index, (uint8_t *)p_read_rsp->data,
							   p_read_rsp->len, p_read_rsp->len, cause)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#else

	if (!le_get_conn_id_by_handle(p_read_rsp->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!server_attr_read_confirm(conn_id,
								  (T_SERVER_ID)p_srv_node->server_info,
								  p_read_rsp->index,
								  (uint8_t *)p_read_rsp->data, p_read_rsp->len, cause)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	return RTK_BT_OK;
}

static uint16_t bt_stack_gatts_write_rsp(void *param)
{
	rtk_bt_gatts_write_resp_param_t *p_write_rsp_t = (rtk_bt_gatts_write_resp_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	T_APP_RESULT cause;
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	uint8_t conn_id;
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_write_rsp_t->app_id);
	if (!p_srv_node) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	cause = att_err_to_app_result(p_write_rsp_t->err_code);

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	if (!gatt_svc_write_confirm(p_write_rsp_t->conn_handle,
								p_write_rsp_t->cid ? p_write_rsp_t->cid : L2C_FIXED_CID_ATT, /* just in case APP forget set cid */
								(T_SERVER_ID)p_srv_node->server_info,
								p_write_rsp_t->index, cause)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#else

	if (!le_get_conn_id_by_handle(p_write_rsp_t->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (RTK_BT_GATTS_WRITE_LONG == p_write_rsp_t->type) {
		if (!server_exec_write_confirm(conn_id, cause, 0)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}

	} else if (RTK_BT_GATTS_WRITE_REQ == p_write_rsp_t->type) {
		if (!server_attr_write_confirm(conn_id,
									   p_srv_node->server_info,
									   p_write_rsp_t->index, cause)) {
			return RTK_BT_ERR_LOWER_STACK_API;
		}
	}
#endif
	return RTK_BT_OK;
}

static uint16_t bt_stack_gatts_send_data(void *param, bool notify)
{
	rtk_bt_gatts_ntf_and_ind_param_t *p_notify_param = (rtk_bt_gatts_ntf_and_ind_param_t *)param;
	uint16_t conn_handle = p_notify_param->conn_handle;
	uint16_t len = p_notify_param->len;
	uint8_t conn_id;
	uint16_t ret = 0;
	rtk_bt_gatt_queue_t *queue;
	rtk_bt_gatts_req_t *req = NULL;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	queue = notify ? &g_rtk_bt_gatts_priv->notify_queue[conn_id] : &g_rtk_bt_gatts_priv->indicate_queue[conn_id];

	/* Save memory when high throughput */
	if (queue->pending_ele_num >= BT_QUEUE_PENDING_ELEMENT_MAX) {
		// BT_LOGE("Error: GATTS pending queue full, wait a moment to send data again !!!\r\n");
		return RTK_BT_ERR_QUEUE_FULL;
	}

	req = (rtk_bt_gatts_req_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_gatts_req_t) + len);
	if (!req) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	memset(req, 0, sizeof(rtk_bt_gatts_req_t));

	req->app_id = p_notify_param->app_id;
	req->conn_handle = conn_handle;
	req->seq = p_notify_param->seq;
	req->index = p_notify_param->index;
	req->cid = p_notify_param->cid ? p_notify_param->cid : L2C_FIXED_CID_ATT; /* just in case APP forget set cid */

	if (len != 0) {
		req->len = len;
		req->data = (uint8_t *)req + sizeof(rtk_bt_gatts_req_t);
		memcpy(req->data, p_notify_param->data, p_notify_param->len);
	}

#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	/* ble_mgr.a will queue packets, no matter notify or indicate. So send directly before add to list */
	if (notify)
#endif
	{
		ret = _send_data(notify, req);
		if (ret != RTK_BT_OK) {
			osif_mem_free(req);
			return ret;
		}
	}

	list_add_tail(&req->list, &queue->pending_list);
	queue->pending_ele_num++;
	req->seq = g_rtk_bt_gatts_priv->last_seq++;

	if (!notify) {
		_handle_indicate_pending_queue();
	}

	return RTK_BT_OK;
}

uint16_t bt_stack_le_gatts_get_tx_pending_num(uint16_t conn_handle, uint16_t *tx_pending_num)
{
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (tx_pending_num == NULL || conn_id >= RTK_BLE_GAP_MAX_LINKS) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (g_rtk_bt_gatts_priv) {
		//there is no pending queue in notify queue and no pri_pending_queue in indicate_queue
		*tx_pending_num = g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_ele_num +
						  g_rtk_bt_gatts_priv->notify_queue[conn_id].pending_ele_num;
	}

	return RTK_BT_OK;
}

uint16_t bt_stack_gatts_service_changed_indicate(void *param)
{
	rtk_bt_gatts_service_changed_indicate_param_t *srv_change = (rtk_bt_gatts_service_changed_indicate_param_t *)param;

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB
	if (!gatt_svc_service_changed_indicate(srv_change->conn_handle,
										   srv_change->cid ? srv_change->cid : L2C_FIXED_CID_ATT,
										   srv_change->start_handle,
										   srv_change->end_handle)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#else
	uint8_t conn_id;
	if (!le_get_conn_id_by_handle(srv_change->conn_handle, &conn_id)) {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (!gatts_service_changed_indicate(conn_id,
										srv_change->start_handle,
										srv_change->end_handle)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	return RTK_BT_OK;
}

uint16_t bt_stack_gatts_get_attribute_handle(void *param)
{
	rtk_bt_gatts_get_attr_handle_param_t *get_handle = (rtk_bt_gatts_get_attr_handle_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(get_handle->app_id);
	if (!p_srv_node) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	*get_handle->attr_handle = server_get_start_handle((T_SERVER_ID)p_srv_node->server_info)
							   + get_handle->attr_index;

	return RTK_BT_OK;
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_stack_gatts_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_OK;

	if (true != bt_stack_profile_check(RTK_BT_PROFILE_GATTS)) {
		BT_LOGE("Error: Gatts profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch (p_cmd->act) {
	case RTK_BT_GATTS_ACT_REGISTER_SERVICE:
		ret = bt_stack_gatts_register_service(p_cmd->param);
		break;
	case RTK_BT_GATTS_ACT_READ_RSP:
		ret = bt_stack_gatts_read_rsp(p_cmd->param);
		break;
	case RTK_BT_GATTS_ACT_WRITE_RSP:
		ret = bt_stack_gatts_write_rsp(p_cmd->param);
		break;
	case RTK_BT_GATTS_ACT_NOTIFY:
		ret = bt_stack_gatts_send_data(p_cmd->param, true);
		break;
	case RTK_BT_GATTS_ACT_INDICATE:
		ret = bt_stack_gatts_send_data(p_cmd->param, false);
		break;
	case RTK_BT_GATTS_ACT_SERVICE_CHANGED_INDICATE:
		ret = bt_stack_gatts_service_changed_indicate(p_cmd->param);
		break;
	case RTK_BT_GATTS_ACT_GET_ATTR_HANDLE:
		ret = bt_stack_gatts_get_attribute_handle(p_cmd->param);
		break;
	default:
		BT_LOGE("bt_stack_le_act_handle:unknown act: %d \r\n", p_cmd->act);
		ret = RTK_BT_ERR_NO_CASE_ELEMENT;
		break;
	}

end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}