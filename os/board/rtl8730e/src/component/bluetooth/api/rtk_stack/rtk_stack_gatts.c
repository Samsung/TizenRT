/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <osif.h>
#include <dlist.h>
#include <platform_stdlib.h>

#include <rtk_bt_def.h>
#include <rtk_stack_config.h>
#include <rtk_bt_gatts.h>  
#include <rtk_bt_att_defs.h>
#include <rtk_bt_common.h>
#include <rtk_stack_gatt.h>

#include <gap.h>
#include <gap_conn_le.h>
#include <gap_config.h>
#include <gatt_builtin_services.h>

#if RTK_BLE_MGR_LIB
#include <bt_gatt_svc.h>
#else
#include <profile_server.h>
#endif

#if !RTK_BLE_MGR_LIB
uint16_t g_rtk_bt_stack_credits = 0;
#endif
static rtk_bt_gatts_app_priv_t *g_rtk_bt_gatts_priv = NULL;

static struct rtk_bt_gatt_service *bt_stack_gatts_find_service_node_by_server_id(T_SERVER_ID server_id);
static T_APP_RESULT bt_stack_gatts_attr_read_cb(
											#if RTK_BLE_MGR_LIB
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
#if !RTK_BLE_MGR_LIB
	uint16_t conn_handle = le_get_conn_handle(conn_id);
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(service_id);
	if(!p_srv_node)
		return APP_RESULT_REJECT;

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_READ_IND,sizeof(rtk_bt_gatts_read_ind_t));
	
	p_read_ind = (rtk_bt_gatts_read_ind_t*)p_evt->data;
	p_read_ind->app_id = p_srv_node->app_id;
	p_read_ind->conn_handle = conn_handle;
	p_read_ind->index = attrib_index;
	p_read_ind->offset = offset;
#if RTK_BLE_MGR_LIB
	p_read_ind->cid = cid;
#else
	p_read_ind->cid = GATT_FIXED_L2CAP_CID;
#endif

	if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
		rtk_bt_event_free(p_evt);
		return APP_RESULT_REJECT;
	}

	return APP_RESULT_PENDING;
}

static T_APP_RESULT bt_stack_gatts_attr_write_cb(
										#if RTK_BLE_MGR_LIB
											uint16_t conn_handle, uint16_t cid,
										#else
											uint8_t conn_id,
										#endif
										T_SERVER_ID service_id,
										uint16_t attrib_index, T_WRITE_TYPE write_type,
										uint16_t length, uint8_t *p_value,
										#if RTK_BLE_MGR_LIB
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
#if !RTK_BLE_MGR_LIB
	uint16_t conn_handle = le_get_conn_handle(conn_id);
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(service_id);
	if(!p_srv_node)
		return APP_RESULT_REJECT;

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_WRITE_IND,sizeof(rtk_bt_gatts_write_ind_t));
	if(!p_evt)
		return APP_RESULT_REJECT;

	p_write_ind = p_evt->data;

	p_write_ind->app_id = p_srv_node->app_id;
	p_write_ind->conn_handle = conn_handle;
	p_write_ind->index = attrib_index;
	p_write_ind->offset = 0;
	p_write_ind->len = length;
	if(length) {
		/* 2nd level ptr in write event msg need deep copy */
		p_write_ind->value = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, length);
		memcpy(p_write_ind->value, p_value, length);
		/* 	user_data point to the memory alloced for 2nd level ptr, so it's convenient
			to free it when free p_evt */
		p_evt->user_data = p_write_ind->value;
	}
#if RTK_BLE_MGR_LIB
	p_write_ind->cid = cid;
#else
	p_write_ind->cid = GATT_FIXED_L2CAP_CID;
#endif

	switch(write_type){
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
	
	if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
		rtk_bt_event_free(p_evt);
		return APP_RESULT_REJECT;
	}

	return APP_RESULT_PENDING;
}

static void bt_stack_gatts_cccd_update_cb(
										#if RTK_BLE_MGR_LIB
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
#if !RTK_BLE_MGR_LIB
	uint16_t conn_handle = le_get_conn_handle(conn_id);
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(service_id);
	if(!p_srv_node)
		return;

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_CCCD_IND,sizeof(rtk_bt_gatts_cccd_ind_t));
	if(!p_evt)
		return;

	p_cccd_ind = (rtk_bt_gatts_cccd_ind_t *)p_evt->data;

	p_cccd_ind->conn_handle = conn_handle;
	p_cccd_ind->app_id = p_srv_node->app_id;
	p_cccd_ind->index = index;
	p_cccd_ind->value = cccbits;
#if RTK_BLE_MGR_LIB
	p_cccd_ind->cid = cid;
#else
	p_cccd_ind->cid = GATT_FIXED_L2CAP_CID;
#endif

	if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
		rtk_bt_event_free(p_evt);
	}
	
}

#if RTK_BLE_MGR_LIB
static const T_FUN_GATT_EXT_SERVICE_CBS rtk_bt_ext_gatts_cb =
#else
static const T_FUN_GATT_SERVICE_CBS rtk_bt_gatts_cb =
#endif
{
	bt_stack_gatts_attr_read_cb,
	bt_stack_gatts_attr_write_cb,
	bt_stack_gatts_cccd_update_cb,
};

static void bt_stack_gatts_free_srv_tbl(T_ATTRIB_APPL* rtk_srv_table, uint16_t count)
{
	if (rtk_srv_table)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			if (rtk_srv_table[i].p_value_context && 
				*(uint16_t *)(rtk_srv_table[i].type_value) != GATT_UUID_INCLUDE) {
				osif_mem_free(rtk_srv_table[i].p_value_context);
			}
		}
		osif_mem_free(rtk_srv_table);
	}
}

static struct rtk_bt_gatt_service *bt_stack_gatts_find_service_node_by_app_id(uint8_t app_id)
{
	struct rtk_bt_gatt_service * p_service_node = NULL;
	bool b_find = false;
	struct list_head *plist = NULL;

	if(list_empty(&g_rtk_bt_gatts_priv->service_list)){
		// printf("service list is empty \r\n");
		goto end;
	}

	plist = g_rtk_bt_gatts_priv->service_list.next;
	while(&g_rtk_bt_gatts_priv->service_list != plist){
		p_service_node = (struct rtk_bt_gatt_service *)plist;
		if(p_service_node->app_id == app_id){
			// printf("found node: app_id = %d \r\n",p_service_node->app_id);
			b_find = true;
			break;
		}
		else{
			plist = plist->next;
		}
	}

end:
	if(b_find == true)
		return p_service_node;
	else 
		return NULL;
}

static struct rtk_bt_gatt_service *bt_stack_gatts_find_service_node_by_server_id(T_SERVER_ID server_id)
{
	struct rtk_bt_gatt_service * p_service_node = NULL;
	struct list_head *plist = NULL;
	bool b_find = false;

	if(list_empty(&g_rtk_bt_gatts_priv->service_list)){
		// printf("service list is empty \r\n");
		goto end;
	}

	plist = g_rtk_bt_gatts_priv->service_list.next;
	while(&g_rtk_bt_gatts_priv->service_list != plist){
		p_service_node = (struct rtk_bt_gatt_service *)plist;
		if((T_SERVER_ID)p_service_node->server_info == server_id){
			// printf("found node: server id = %d \r\n",p_service_node->server_info);
			b_find = true;
			break;
		}
		else{
			plist = plist->next;
		}
	}

end:
	if(b_find == true)
		return p_service_node;
	else 
		return NULL;
}

static void bt_stack_gatts_insert_service_node(struct rtk_bt_gatt_service *p_srv_node)
{
	if(!p_srv_node)
		return;

	list_add_tail(&p_srv_node->list, &g_rtk_bt_gatts_priv->service_list);

	return;
}

static void bt_stack_gatts_delete_service_node(struct rtk_bt_gatt_service *p_srv_node)
{
	if(!p_srv_node)
		return;

	if(!list_empty(&g_rtk_bt_gatts_priv->service_list))
		list_del(&p_srv_node->list);
	if(p_srv_node->alloc_ind)
		osif_mem_free(p_srv_node);

	return;
}


#if !RTK_BLE_MGR_LIB
static T_APP_RESULT bt_stack_gatts_evt_register_service_complete(T_SERVER_APP_CB_DATA *p_param)
{
	T_SERVER_RESULT result = p_param->event_data.server_reg_after_init_result.result;
	T_SERVER_ID srv_id = p_param->event_data.server_reg_after_init_result.service_id;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	struct list_head *plist = NULL;
	rtk_bt_evt_t *p_gatts_evt = NULL;
	rtk_bt_gatts_reg_ind_t *p_reg_ind = NULL;
	T_APP_RESULT ret = APP_RESULT_SUCCESS;
	bool add_service_ret = false;

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(srv_id);
	if(!p_srv_node){
		printf("Cannot get the app_id from srv_id in SRV_REGISTERING srv\r\n");
		return APP_RESULT_REJECT;
	}

	if (GATT_SERVER_FAIL == result)
	{
		printf("result is fail:cause = 0x%x \r\n",p_param->event_data.server_reg_after_init_result.cause);
		if(p_srv_node->user_data){
			bt_stack_gatts_free_srv_tbl(p_srv_node->user_data,p_srv_node->attr_count);
		}
		bt_stack_gatts_delete_service_node(p_srv_node);
		ret = APP_RESULT_REJECT;
	}
	else
	{
		/* Change register status */
		p_srv_node->register_status = SRV_REGISTERED;
		// printf("result is successfully \r\n");
		ret = APP_RESULT_SUCCESS;
	}

	p_gatts_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_REGISTER_SERVICE,sizeof(rtk_bt_gatts_reg_ind_t));
	if(p_gatts_evt){
		p_reg_ind = (rtk_bt_gatts_reg_ind_t *)p_gatts_evt->data;
		p_reg_ind->app_id = p_srv_node->app_id;
		p_reg_ind->reg_status = ((ret == APP_RESULT_SUCCESS) ? RTK_BT_OK : RTK_BT_FAIL);

		rtk_bt_evt_indicate(p_gatts_evt, NULL);
	}else{
		printf("p_gatts_evt alloc fail \r\n");
	}

	g_rtk_bt_gatts_priv->srv_registering = 0;

	plist = g_rtk_bt_gatts_priv->service_list.next;
	while(&g_rtk_bt_gatts_priv->service_list != plist){
		p_srv_node = (struct rtk_bt_gatt_service *)plist;
		if((p_srv_node->register_status == SRV_WAIT_FOR_REG) && 
			!g_rtk_bt_gatts_priv->srv_registering &&
			p_srv_node->user_data){
			printf("waiting for register: app_id = %d \r\n",p_srv_node->app_id);    
			g_rtk_bt_gatts_priv->srv_registering = 1;

			if (true == p_srv_node->assgin_handle_flag) {
				add_service_ret = server_add_service_by_start_handle((T_SERVER_ID *)&p_srv_node->server_info,
										(uint8_t *)p_srv_node->user_data,
										p_srv_node->attr_count * sizeof(T_ATTRIB_APPL),
										rtk_bt_gatts_cb, p_srv_node->start_handle);
			} else {
				add_service_ret = server_add_service((T_SERVER_ID *)&p_srv_node->server_info,
											(uint8_t *)p_srv_node->user_data,
											p_srv_node->attr_count * sizeof(T_ATTRIB_APPL),
											rtk_bt_gatts_cb);
			}
			if (false == add_service_ret) {
				bt_stack_gatts_free_srv_tbl(p_srv_node->user_data, p_srv_node->attr_count);
				bt_stack_gatts_delete_service_node(p_srv_node);
				continue;
			}
			p_srv_node->register_status = SRV_REGISTERING;
			break;
		}
		else{
			plist = plist->next;
		}
	}

	return ret;

}

static uint16_t bt_stack_gatts_send_notify(rtk_bt_gatts_req_t *p_req)
{
	uint16_t mtu_size = 0;
	uint16_t data_len = 0;
	uint8_t i = 0;
	struct rtk_bt_gatt_service * p_srv_node = NULL;
	uint8_t conn_id;
	
	if (!le_get_conn_id_by_handle(p_req->conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_req->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

	/* data len of handle_value_notification is 0 to MTU-3, if over it, server_send_data will fail */
	data_len = (mtu_size - 3) < p_req->len ? (mtu_size - 3) : p_req->len;
	for (i = 0; i < 3; i++)
	{
		if (true == server_send_data(conn_id, p_srv_node->server_info, 
				p_req->index, (uint8_t*)p_req->data, data_len, GATT_PDU_TYPE_NOTIFICATION))
		{
			g_rtk_bt_stack_credits--;
			list_add_tail(&p_req->list,&g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_list);
			g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_ele_num++;
			return RTK_BT_OK;
		}
		osif_delay(20);
	}

	return RTK_BT_ERR_LOWER_STACK_API;
}

static uint16_t bt_stack_gatts_send_indicate(rtk_bt_gatts_req_t *p_req)
{
	uint16_t mtu_size = 0;
	uint16_t data_len = 0;
	uint8_t i = 0;
	struct rtk_bt_gatt_service * p_srv_node = NULL;
	uint8_t conn_id;
	
	if (!le_get_conn_id_by_handle(p_req->conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_req->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

	/* data len of handle_value_indication is 0 to MTU-3, if over it, server_send_data will fail */
	data_len = (mtu_size - 3) < p_req->len ? (mtu_size - 3) : p_req->len;
	for (i = 0; i < 3; i++)
	{
		if (true == server_send_data(conn_id, p_srv_node->server_info, 
					p_req->index, (uint8_t*)p_req->data, data_len, GATT_PDU_TYPE_INDICATION))
		{
			list_add_tail(&p_req->list,&g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_list);
			g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_ele_num++;
			return RTK_BT_OK;
		}
		osif_delay(20);
	}

	return RTK_BT_ERR_LOWER_STACK_API;
}

static T_APP_RESULT bt_stack_gatts_evt_send_data_complete(T_SERVER_APP_CB_DATA *p_param)
{
	T_SERVER_ID server_id = p_param->event_data.send_data_result.service_id;
	uint16_t index = p_param->event_data.send_data_result.attrib_idx;
	uint8_t conn_id = p_param->event_data.send_data_result.conn_id;
	uint16_t cause = p_param->event_data.send_data_result.cause;
	uint16_t credits = p_param->event_data.send_data_result.credits;
	struct rtk_bt_gatt_service * p_srv_node = NULL;
	rtk_bt_gatts_req_t *p_req = NULL;
	struct list_head * plist = NULL;
	uint16_t ret = 0;
	bool bindicate = false;
	bool bnotify = false;
	rtk_bt_evt_t *p_evt = NULL, *p_evt_err = NULL;
	rtk_bt_gatts_ntf_and_ind_ind_t *p_ind = NULL, *p_ind_err = NULL;	
	uint16_t conn_handle = le_get_conn_handle(conn_id);

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(server_id);
	if(!p_srv_node)
		return APP_RESULT_APP_ERR;

	//search notify sending list
	if(!list_empty(&g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_list)){
		plist = g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_list.next;
		while(plist != &g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_list){
			p_req = (rtk_bt_gatts_req_t *)plist;
			if((p_req->app_id == p_srv_node->app_id) && (p_req->index == index)){
				bnotify = true;
				g_rtk_bt_stack_credits = credits;
				break;
			}else{
				plist = plist->next;
			}
		}
	}

	//search indicate queue
	if(!bnotify){
		if(!list_empty(&g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_list)){
			plist = g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_list.next;
			while(plist != &g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_list){
				p_req = (rtk_bt_gatts_req_t *)plist;
				if((p_req->app_id == p_srv_node->app_id) && (p_req->index == index)){
					bindicate = true;
					break;
				}else{
					plist = plist->next;
				}
			}
		}
	}

	if(bnotify){
		list_del(&p_req->list);
		g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_ele_num--;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND,sizeof(rtk_bt_gatts_ntf_and_ind_ind_t));
		if(!p_evt)
			return APP_RESULT_APP_ERR;
		p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t *)p_evt->data;

		p_ind->app_id = p_srv_node->app_id;
		p_ind->conn_handle = conn_handle;
		p_ind->seq = p_req->seq;
		p_ind->index = index;
		p_ind->err_code = cause;

		if(p_req->len != 0)
			osif_mem_free(p_req->data);
		osif_mem_free(p_req);
		p_req = NULL;

		if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
			rtk_bt_event_free(p_evt);
			return APP_RESULT_APP_ERR;
		}
	}

	if(bindicate){
		list_del(&p_req->list);
		g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_ele_num--;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND,sizeof(rtk_bt_gatts_ntf_and_ind_ind_t));
		if(!p_evt)
			return APP_RESULT_APP_ERR;
		p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t *)p_evt->data;

		p_ind->app_id = p_srv_node->app_id;
		p_ind->conn_handle = conn_handle;
		p_ind->seq = p_req->seq;
		p_ind->err_code = cause;

		if(p_req->len != 0)
			osif_mem_free(p_req->data);
		osif_mem_free(p_req);
		p_req = NULL;

		if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
			rtk_bt_event_free(p_evt);
			return APP_RESULT_APP_ERR;
		}
		
		if(!list_empty(&g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_list)){
			p_req = (rtk_bt_gatts_req_t *)g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_list.next;
			list_del(&p_req->list);
			g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_ele_num--;
		}else{
			p_req = NULL;
		}
		
		if(p_req){
			printf("sending indicating pending node \r\n");
			ret = bt_stack_gatts_send_indicate(p_req);
			if(RTK_BT_OK != ret){
				p_evt_err = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND,sizeof(rtk_bt_gatts_ntf_and_ind_ind_t));
				if(!p_evt_err){
					if(p_req->len != 0)
						osif_mem_free(p_req->data);
					osif_mem_free(p_req);
					return APP_RESULT_APP_ERR;;
				}
				p_ind_err = (rtk_bt_gatts_ntf_and_ind_ind_t *)p_evt_err->data;
				p_ind_err->app_id = p_req->app_id;
				p_ind_err->conn_handle = p_req->conn_handle;
				p_ind_err->index = p_req->index;
				p_ind_err->err_code = ret;

				if(p_req->len != 0)
					osif_mem_free(p_req->data);
				osif_mem_free(p_req);

				if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt_err, NULL)){
					rtk_bt_event_free(p_evt_err);
					return APP_RESULT_APP_ERR;;
				}
			}
		}

	}

	return APP_RESULT_SUCCESS;
}

static T_APP_RESULT bt_stack_gatts_app_service_callback(T_SERVER_ID service_id, void *data)
{
	T_APP_RESULT app_result = APP_RESULT_SUCCESS;

	if (!data)
		return APP_RESULT_APP_ERR;

	/* Only handle 'GENERAL ID' callback */
	if (SERVICE_PROFILE_GENERAL_ID == service_id)
	{
		T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)data;

		switch (p_param->eventId)
		{
		case PROFILE_EVT_SRV_REG_COMPLETE:
			// printf("PROFILE_EVT_SRV_REG_COMPLETE \r\n");
			break;
		case PROFILE_EVT_SRV_REG_AFTER_INIT_COMPLETE:
			// printf("PROFILE_EVT_SRV_REG_AFTER_INIT_COMPLETE \r\n");
			app_result = bt_stack_gatts_evt_register_service_complete(p_param);
			break;
		case PROFILE_EVT_SEND_DATA_COMPLETE:
			// printf("PROFILE_EVT_SEND_DATA_COMPLETE \r\n");
			app_result = bt_stack_gatts_evt_send_data_complete(p_param);
			break;
		default:
			printf("unknown gatts app service id: 0x%x \r\n",p_param->eventId);
			app_result = APP_RESULT_APP_ERR;
			break;
		}
	}

	return app_result;
}
#endif

#if RTK_BLE_MGR_LIB
static bool bt_stack_gatts_queue_del_item(bool notify, uint16_t conn_handle, uint16_t app_id, uint16_t index)
{
	struct list_head *plist = NULL;
	rtk_bt_gatts_req_t *p_req;
	rtk_bt_gatt_queue_t *queue;
	uint8_t conn_id;
	
	if (!le_get_conn_id_by_handle(conn_handle, &conn_id))
		return false;

	if (notify) {
		queue = &g_rtk_bt_gatts_priv->notify_queue[conn_id];
	} else {
		queue = &g_rtk_bt_gatts_priv->indicate_queue[conn_id];
	}

	if(!list_empty(&queue->sending_list)) {
		plist = queue->sending_list.next;
		while(plist != &queue->sending_list){
			p_req = (rtk_bt_gatts_req_t *)plist;
			if ((p_req->app_id == app_id) && (p_req->index == index)) {
				list_del(&p_req->list);
				queue->sending_ele_num--;
				osif_mem_free(p_req);
				return true;
			} else {
				plist = plist->next;
			}
		}
	}

	return false;
}

static void bt_stack_gatts_send_data_cb(T_EXT_SEND_DATA_RESULT result)
{
	struct rtk_bt_gatt_service * p_srv_node = NULL;
	uint16_t conn_handle = result.conn_handle;
	uint16_t index = result.attrib_idx;
	rtk_bt_evt_t *p_evt = NULL;
	rtk_bt_gatts_ntf_and_ind_ind_t *p_ind = NULL;	
	uint8_t evt;

	p_srv_node = bt_stack_gatts_find_service_node_by_server_id(result.service_id);
	if(!p_srv_node)
		return;

	if (bt_stack_gatts_queue_del_item(true, conn_handle, p_srv_node->app_id, index)) {
		evt = RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND;
	} else if (bt_stack_gatts_queue_del_item(false, conn_handle, p_srv_node->app_id, index)) {
		evt = RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND;
	} else {
		return;
	}

	p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, evt, sizeof(rtk_bt_gatts_ntf_and_ind_ind_t));
	if(!p_evt)
		return;

	p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t *)p_evt->data;
	p_ind->app_id = p_srv_node->app_id;
	p_ind->conn_handle = conn_handle;
	p_ind->cid = result.cid;
	p_ind->index = index;
	p_ind->err_code = result.cause;

	if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
		rtk_bt_event_free(p_evt);
	}
}
#endif

static T_APP_RESULT bt_stack_le_gap_service_callback(T_SERVER_ID service_id, void *param)
{
	(void)service_id;
	T_APP_RESULT result = APP_RESULT_SUCCESS;
	T_GAPS_CALLBACK_DATA *p_gap_data = (T_GAPS_CALLBACK_DATA *)param;

	if (SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE == p_gap_data->msg_type) {
		switch (p_gap_data->msg_data.opcode)
		{
		case GAPS_WRITE_DEVICE_NAME:
		{
			T_LOCAL_NAME device_name;
			memcpy(device_name.local_name, p_gap_data->msg_data.p_value, p_gap_data->msg_data.len);
			device_name.local_name[p_gap_data->msg_data.len] = 0;
			flash_save_local_name(&device_name);
			break;
		}
		case GAPS_WRITE_APPEARANCE:
		{
			T_LOCAL_APPEARANCE appearance = {.local_appearance = *(uint16_t *)p_gap_data->msg_data.p_value};
			flash_save_local_appearance(&appearance);
			break;
		}
		case GATT_SERVICE_WRITE_CLIENT_SUPPORTED_FEATURES:
		{
			rtk_bt_evt_t *p_evt = NULL;
			rtk_bt_gatts_client_supported_features_ind_t *p_ind = NULL;	
			p_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,
										RTK_BT_GATTS_EVT_CLIENT_SUPPORTED_FEATURES,
										sizeof(rtk_bt_gatts_client_supported_features_ind_t));
			if(!p_evt)
				break;

			p_ind = (rtk_bt_gatts_client_supported_features_ind_t *)p_evt->data;
			p_ind->conn_handle = le_get_conn_handle(p_gap_data->conn_id);
#if F_BT_GATT_SERVER_EXT_API
			p_ind->cid = p_gap_data->cid;
#else
			p_ind->cid = GATT_FIXED_L2CAP_CID;
#endif
			p_ind->features = *(uint8_t *)p_gap_data->msg_data.p_value;

			if(RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)){
				rtk_bt_event_free(p_evt);
			}

			// printf("GATT_SERVICE_WRITE_CLIENT_SUPPORTED_FEATURES: len=%d, value=0x%x\r\n",p_gap_data->msg_data.len,*(uint8_t *)p_gap_data->msg_data.p_value);
			break;
		}
		default:
			break;
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

	g_rtk_bt_gatts_priv = osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_gatts_app_priv_t));
	if(!g_rtk_bt_gatts_priv)
		return RTK_BT_ERR_NO_MEMORY;

	memset(g_rtk_bt_gatts_priv,0,sizeof(rtk_bt_gatts_app_priv_t));

	INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->service_list);

	for(i=0;i<GAP_MAX_LINKS;i++){
		INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->notify_queue[i].pending_list);
		g_rtk_bt_gatts_priv->notify_queue[i].pending_ele_num = 0;
		INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->notify_queue[i].sending_list);
		g_rtk_bt_gatts_priv->notify_queue[i].sending_ele_num = 0;
		INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->indicate_queue[i].pending_list);
		g_rtk_bt_gatts_priv->indicate_queue[i].pending_ele_num = 0;
		INIT_LIST_HEAD(&g_rtk_bt_gatts_priv->indicate_queue[i].sending_list);
		g_rtk_bt_gatts_priv->indicate_queue[i].sending_ele_num = 0;
	}

	if (app_conf && true == app_conf->cccd_not_check) {
		gap_config_cccd_not_check(CONFIG_GATT_CCCD_NOT_CHECK);
	}

	if (app_conf && false == app_conf->user_def_service) {
		if (flash_load_local_name(&local_device_name) == 0)
			gaps_set_parameter(GAPS_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, local_device_name.local_name);
		else
			gaps_set_parameter(GAPS_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, default_device_name);
		if (flash_load_local_appearance(&appearance_local) == 0)
			gaps_set_parameter(GAPS_PARAM_APPEARANCE, sizeof(uint16_t), &appearance_local.local_appearance);
		else
			gaps_set_parameter(GAPS_PARAM_APPEARANCE, sizeof(uint16_t), &default_appearance);

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
#if RTK_BLE_MGR_LIB
	if(!gatt_svc_init(GATT_SVC_USE_EXT_SERVER, svc_num))
		printf("gatt_svc_init fail \r\n");
#else
	/* GATT server init */
	server_init(svc_num);
	server_register_app_cb(bt_stack_gatts_app_service_callback);
#endif
	return RTK_BT_OK;
}

void bt_stack_gatts_deinit(void)
{
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_req_t *p_req = NULL;
	struct list_head *plist = NULL;
	uint32_t i = 0;

	//delete service
	if(!list_empty(&g_rtk_bt_gatts_priv->service_list)){
		plist = g_rtk_bt_gatts_priv->service_list.next;
		while(plist != &g_rtk_bt_gatts_priv->service_list){
			p_srv_node = (struct rtk_bt_gatt_service*)plist;
			plist = plist->next;
			list_del(&p_srv_node->list);
			if(p_srv_node->user_data)
				bt_stack_gatts_free_srv_tbl((T_ATTRIB_APPL *)p_srv_node->user_data,p_srv_node->attr_count);
			if(p_srv_node->alloc_ind)
				osif_mem_free((void *)p_srv_node);
		}
	}

	//delete notify queue and indicate queue
	for(i=0;i<GAP_MAX_LINKS;i++){
		if(!list_empty(&g_rtk_bt_gatts_priv->notify_queue[i].pending_list)){
			plist = g_rtk_bt_gatts_priv->notify_queue[i].pending_list.next;
			while(plist != &g_rtk_bt_gatts_priv->notify_queue[i].pending_list){
				p_req = (rtk_bt_gatts_req_t*)plist;
				plist = plist->next;
				list_del(&p_req->list);				
				if(p_req->len!=0)
					osif_mem_free(p_req->data);
				osif_mem_free(p_req);
			}
			g_rtk_bt_gatts_priv->notify_queue[i].pending_ele_num = 0;
		}
		if(!list_empty(&g_rtk_bt_gatts_priv->notify_queue[i].sending_list)){
			plist = g_rtk_bt_gatts_priv->notify_queue[i].sending_list.next;
			while(plist != &g_rtk_bt_gatts_priv->notify_queue[i].sending_list){
				p_req = (rtk_bt_gatts_req_t*)plist;
				plist = plist->next;
				list_del(&p_req->list);
				if(p_req->len!=0)
					osif_mem_free(p_req->data);
				osif_mem_free(p_req);
			}
			g_rtk_bt_gatts_priv->notify_queue[i].sending_ele_num = 0;
		}
	}
	
	for(i=0;i<GAP_MAX_LINKS;i++){
		if(!list_empty(&g_rtk_bt_gatts_priv->indicate_queue[i].pending_list)){
			plist = g_rtk_bt_gatts_priv->indicate_queue[i].pending_list.next;
			while(plist != &g_rtk_bt_gatts_priv->indicate_queue[i].pending_list){
				p_req = (rtk_bt_gatts_req_t*)plist;
				plist = plist->next;
				list_del(&p_req->list);
				if(p_req->len!=0)
					osif_mem_free(p_req->data);
				osif_mem_free(p_req);
			}
			g_rtk_bt_gatts_priv->indicate_queue[i].pending_ele_num = 0;
		}

		if(!list_empty(&g_rtk_bt_gatts_priv->indicate_queue[i].sending_list)){
			plist = g_rtk_bt_gatts_priv->indicate_queue[i].sending_list.next;
			while(plist != &g_rtk_bt_gatts_priv->indicate_queue[i].sending_list){
				p_req = (rtk_bt_gatts_req_t*)plist;
				plist = plist->next;
				list_del(&p_req->list);
				if(p_req->len!=0)
					osif_mem_free(p_req->data);
				osif_mem_free(p_req);
			}
			g_rtk_bt_gatts_priv->indicate_queue[i].sending_ele_num = 0;
		}
	}

	osif_mem_free(g_rtk_bt_gatts_priv);
	g_rtk_bt_gatts_priv = NULL;
}


static uint32_t bt_stack_gatts_permission_trans(uint8_t perm)
{
	uint32_t permission = 0x0000;

	/* Get all read permission */
	if (perm & RTK_BT_GATT_PERM_READ)
		permission |= GATT_PERM_READ;
	if (perm & RTK_BT_GATT_PERM_READ_ENCRYPT)
		permission |= GATT_PERM_READ_ENCRYPTED_REQ;
	if (perm & RTK_BT_GATT_PERM_READ_AUTHEN)
		permission |= GATT_PERM_READ_AUTHEN_MITM_REQ;

	/* Get all write permission */
	if (perm & RTK_BT_GATT_PERM_WRITE)
		permission |= GATT_PERM_WRITE;
	if (perm & RTK_BT_GATT_PERM_WRITE_ENCRYPT)
		permission |= GATT_PERM_WRITE_ENCRYPTED_REQ;
	if (perm & RTK_BT_GATT_PERM_WRITE_AUTHEN)
		permission |= GATT_PERM_WRITE_AUTHEN_MITM_REQ;

	return permission;
}


static uint16_t bt_stack_uuid16_attr_convert(uint16_t gatt_type, rtk_bt_gatt_attr_t * p_app_gatt_attr,T_ATTRIB_APPL* p_stack_gatt_attr)
{
	uint16_t uuid16 = BT_UUID_16(p_app_gatt_attr->uuid)->val;

	switch(uuid16){
		case GATT_UUID_PRIMARY_SERVICE:
		case GATT_UUID_SECONDARY_SERVICE:
		{
			uint8_t uuid_type = ((struct bt_uuid *)p_app_gatt_attr->user_data)->type;
			uint8_t *uuid = NULL;
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->permissions = GATT_PERM_READ;
			if (BT_UUID_TYPE_16 == uuid_type){
				uuid = (uint8_t *)&(BT_UUID_16((struct bt_uuid *)p_app_gatt_attr->user_data)->val);
				p_stack_gatt_attr->flags = (ATTRIB_FLAG_VALUE_INCL | gatt_type);
				// printf("uuid16 case p_stack_gatt_attr->flags = 0x%x \r\n",p_stack_gatt_attr->flags);
				p_stack_gatt_attr->type_value[2] = uuid[0];
				p_stack_gatt_attr->type_value[3] = uuid[1];
				p_stack_gatt_attr->p_value_context = NULL;
				p_stack_gatt_attr->value_len = 2;
			}
			else if (BT_UUID_TYPE_128 == uuid_type){
				uuid = BT_UUID_128((struct bt_uuid *)p_app_gatt_attr->user_data)->val;
				p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID | gatt_type;
				p_stack_gatt_attr->value_len = 16;
				p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
				if (!p_stack_gatt_attr->p_value_context)
					return RTK_BT_ERR_NO_MEMORY;
				memcpy(p_stack_gatt_attr->p_value_context, (void *)uuid, p_stack_gatt_attr->value_len);
			}
				break;
		}

		case GATT_UUID_INCLUDE:
		{			
			struct rtk_bt_gatt_service *p_incl_srv = (struct rtk_bt_gatt_service *)(p_app_gatt_attr->user_data);
			struct rtk_bt_gatt_service *p_gatts_srv_node = NULL;
			T_ATTRIB_APPL *p_bt_stack_attr_tbl = NULL;
			p_gatts_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_incl_srv->app_id);
			if(!p_gatts_srv_node){
				// printf("include srv is null case \r\n");
				p_gatts_srv_node = (struct rtk_bt_gatt_service *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(struct rtk_bt_gatt_service));
				if(!p_gatts_srv_node)
					return RTK_BT_ERR_NO_MEMORY;
				memset(p_gatts_srv_node,0,sizeof(struct rtk_bt_gatt_service));
				p_gatts_srv_node->alloc_ind = 1;
				p_bt_stack_attr_tbl = (T_ATTRIB_APPL *)osif_mem_alloc(RAM_TYPE_DATA_ON, p_incl_srv->attr_count*sizeof(T_ATTRIB_APPL));
				if(!p_bt_stack_attr_tbl){
					osif_mem_free(p_gatts_srv_node);
					return RTK_BT_ERR_NO_MEMORY;
				}

				// printf("p_incl_srv->attr_count: %d \r\n",p_incl_srv->attr_count);
				memset(p_bt_stack_attr_tbl,0,p_incl_srv->attr_count*sizeof(T_ATTRIB_APPL));
				p_gatts_srv_node->app_id = p_incl_srv->app_id;
				p_gatts_srv_node->register_status = SRV_ALLOC_FOR_INCLUDE;
				p_gatts_srv_node->user_data = p_bt_stack_attr_tbl;
				p_gatts_srv_node->attr_count = p_incl_srv->attr_count;
				bt_stack_gatts_insert_service_node(p_gatts_srv_node);
			}
			else{
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

		case GATT_UUID_CHARACTERISTIC:
		{
			struct rtk_bt_gatt_chrc *char_attr_val = (struct rtk_bt_gatt_chrc *)p_app_gatt_attr->user_data;
			if((char_attr_val->properties & 0x10) && (char_attr_val->properties & 0x20))
				return RTK_BT_ERR_UNSUPPORTED;
			p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL;
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->type_value[2] = char_attr_val->properties;
			p_stack_gatt_attr->value_len = 1;
			p_stack_gatt_attr->p_value_context = NULL;
			p_stack_gatt_attr->permissions = GATT_PERM_READ;

			break;
		}

		case GATT_UUID_CHAR_EXTENDED_PROP:
		{
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

		case GATT_UUID_CHAR_USER_DESCR:
		{
			p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z;
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->value_len = strlen((const char*)p_app_gatt_attr->user_data);
			if (!p_stack_gatt_attr->value_len){
				p_stack_gatt_attr->p_value_context = NULL;
			}else{
				p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
				if (!p_stack_gatt_attr->p_value_context)
					return RTK_BT_ERR_NO_MEMORY;
				memcpy(p_stack_gatt_attr->p_value_context, p_app_gatt_attr->user_data, p_stack_gatt_attr->value_len);
			}
			p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
			break;
		}
		case GATT_UUID_CHAR_CLIENT_CONFIG:
		{
			p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL;//ATTRIB_FLAG_CCCD_NO_FILTER
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->type_value[2] = 0;
			p_stack_gatt_attr->type_value[3] = 0;
			p_stack_gatt_attr->value_len = 2;
			p_stack_gatt_attr->p_value_context = NULL;
			p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
			/* Force readable and writable, because Core_spec_v5.2 p1556 Table 3.10 defines it. */
			if ((p_stack_gatt_attr->permissions & 0x0f) == 0)
				p_stack_gatt_attr->permissions |= GATT_PERM_READ;
			if ((p_stack_gatt_attr->permissions & 0xf0) == 0)
				p_stack_gatt_attr->permissions |= GATT_PERM_WRITE;
			break;
		}
		case GATT_UUID_CHAR_SERVER_CONFIG:
		{
			struct rtk_bt_gatt_scc* scc_attr_val = (struct rtk_bt_gatt_scc*)p_app_gatt_attr->user_data;
			p_stack_gatt_attr->flags = ATTRIB_FLAG_VALUE_INCL;
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->type_value[2] = LO_WORD(scc_attr_val->flags);
			p_stack_gatt_attr->type_value[3] = LO_WORD(scc_attr_val->flags);
			p_stack_gatt_attr->value_len = 2;
			p_stack_gatt_attr->p_value_context = NULL;
			p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
			/* Force readable and writable, because Core_spec_v5.2 p1557 Table 3.12 defines it. */
			if ((p_stack_gatt_attr->permissions & 0x0f) == 0)
				p_stack_gatt_attr->permissions |= GATT_PERM_READ;
			if ((p_stack_gatt_attr->permissions & 0xf0) == 0)
				p_stack_gatt_attr->permissions |= GATT_PERM_WRITE;
			
			break;
		}
		case GATT_UUID_CHAR_FORMAT:
		{
			struct rtk_bt_gatt_cpf* format = (struct rtk_bt_gatt_cpf*)p_app_gatt_attr->user_data;
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
		case GATT_UUID_CHAR_AGGR_FORMAT:
		{
			struct rtk_bt_gatt_caf* agg_format = (struct rtk_bt_gatt_caf*)p_app_gatt_attr->user_data;
			p_stack_gatt_attr->flags = ATTRIB_FLAG_VOID;
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->value_len = agg_format->count*2;
			if (!p_stack_gatt_attr->value_len){
				p_stack_gatt_attr->p_value_context = NULL;
			}
			else{
				p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
				if (!p_stack_gatt_attr->p_value_context)
					return RTK_BT_ERR_NO_MEMORY;
				memcpy(p_stack_gatt_attr->p_value_context, (void *)agg_format->handles, p_stack_gatt_attr->value_len);
			}
			p_stack_gatt_attr->permissions = GATT_PERM_READ;
			break;
		}
		default:
		{
			p_stack_gatt_attr->flags = (RTK_BT_GATT_APP == p_app_gatt_attr->flag) ? ATTRIB_FLAG_VALUE_APPL : ATTRIB_FLAG_VOID;
			p_stack_gatt_attr->type_value[0] = LO_WORD(uuid16);
			p_stack_gatt_attr->type_value[1] = HI_WORD(uuid16);
			p_stack_gatt_attr->value_len = p_app_gatt_attr->len;
			if (ATTRIB_FLAG_VALUE_APPL == p_stack_gatt_attr->flags){
				p_stack_gatt_attr->value_len = 0;
				p_stack_gatt_attr->p_value_context = NULL;
			}
			else{
				if (!p_stack_gatt_attr->value_len)
					return RTK_BT_ERR_PARAM_INVALID;

				p_stack_gatt_attr->p_value_context = osif_mem_alloc(RAM_TYPE_DATA_ON, p_stack_gatt_attr->value_len);
				if (!p_stack_gatt_attr->p_value_context)
					return RTK_BT_ERR_NO_MEMORY;
				memcpy(p_stack_gatt_attr->p_value_context, p_app_gatt_attr->user_data, p_stack_gatt_attr->value_len);
			}
			p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);
			break;
		} 
	} 

	return RTK_BT_OK;
}

static uint16_t bt_stack_uuid128_attr_convert(uint16_t gatt_type, rtk_bt_gatt_attr_t * p_app_gatt_attr,T_ATTRIB_APPL* p_stack_gatt_attr)
{
	(void)gatt_type;
	uint8_t* uuid128 = BT_UUID_128(p_app_gatt_attr->uuid)->val;
	
	p_stack_gatt_attr->flags = ATTRIB_FLAG_UUID_128BIT | ATTRIB_FLAG_VALUE_APPL;
	memcpy(p_stack_gatt_attr->type_value, (void*)uuid128, 16);
	p_stack_gatt_attr->value_len = 0;
	p_stack_gatt_attr->p_value_context = NULL;
	p_stack_gatt_attr->permissions = bt_stack_gatts_permission_trans(p_app_gatt_attr->perm);

	return 0;
}

static uint16_t bt_stack_gatts_register_service(void  *p_gatts_srv)
{
	struct rtk_bt_gatt_service * p_gatts_app_srv = (struct rtk_bt_gatt_service *)p_gatts_srv;
	struct rtk_bt_gatt_service * p_srv_node = NULL;
	T_ATTRIB_APPL* rtk_service_table = NULL;
	uint16_t gatt_type = 0;
	rtk_bt_gatt_attr_t * p_app_gatt_attr = NULL;
	T_ATTRIB_APPL* p_stack_gatt_attr = NULL;
	bool add_service_ret = false;
	uint16_t ret = 0;
	uint32_t i = 0;

	if (p_gatts_app_srv->type >= GATT_SERVICE_TYPE_RESERVED)
		return RTK_BT_ERR_PARAM_INVALID;

#if UPPER_STACK_VERSION == VERSION_2019
	if (GATT_SERVICE_OVER_BLE != p_gatts_app_srv->type)
		return RTK_BT_ERR_PARAM_INVALID;
	gatt_type = ATTRIB_FLAG_LE;
#elif UPPER_STACK_VERSION == VERSION_2021
	gatt_type = (GATT_SERVICE_OVER_BLE == p_gatts_app_srv->type)?ATTRIB_FLAG_LE:ATTRIB_FLAG_BREDR;
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_gatts_app_srv->app_id);

	if(p_srv_node){
		if(p_srv_node->register_status > SRV_ALLOC_FOR_INCLUDE)
			return RTK_BT_ERR_ALREADY_IN_PROGRESS;
	}
	
	if (!p_srv_node) {
		rtk_service_table = (T_ATTRIB_APPL*)osif_mem_alloc(RAM_TYPE_DATA_ON,p_gatts_app_srv->attr_count * sizeof(T_ATTRIB_APPL));
		if(!rtk_service_table){
			return RTK_BT_ERR_NO_MEMORY;
		}

		p_gatts_app_srv->user_data = (void *)rtk_service_table;           
		bt_stack_gatts_insert_service_node(p_gatts_app_srv);    
	
	} else {
		if(p_srv_node->attr_count < p_gatts_app_srv->attr_count)
			return RTK_BT_ERR_MISMATCH;
		
		rtk_service_table = (T_ATTRIB_APPL*)p_srv_node->user_data;
		p_gatts_app_srv->user_data = rtk_service_table;
		bt_stack_gatts_delete_service_node(p_srv_node);
		bt_stack_gatts_insert_service_node(p_gatts_app_srv);    
	}

	memset(rtk_service_table, 0, p_gatts_app_srv->attr_count * sizeof(T_ATTRIB_APPL));
	for(i=0;i<p_gatts_app_srv->attr_count;i++)
	{
		p_app_gatt_attr = &p_gatts_app_srv->attrs[i];
		p_stack_gatt_attr = &rtk_service_table[i];
		if (BT_UUID_TYPE_16 == p_app_gatt_attr->uuid->type){
			ret = bt_stack_uuid16_attr_convert(gatt_type,p_app_gatt_attr,p_stack_gatt_attr);
		}else if(BT_UUID_TYPE_128 == p_app_gatt_attr->uuid->type){
			ret = bt_stack_uuid128_attr_convert(gatt_type,p_app_gatt_attr,p_stack_gatt_attr);
		}else{
			printf("known uuid type: 0x%x \r\n",p_app_gatt_attr->uuid->type);
		}
		if (ret)
			return ret;
	}

#if RTK_BLE_MGR_LIB
	add_service_ret = gatt_svc_add((T_SERVER_ID *)&p_gatts_app_srv->server_info,
								   (uint8_t *)rtk_service_table,
								   p_gatts_app_srv->attr_count * sizeof(T_ATTRIB_APPL),
								   &rtk_bt_ext_gatts_cb,
								   bt_stack_gatts_send_data_cb);
	if (add_service_ret) {
		rtk_bt_evt_t *p_gatts_evt = NULL;
		rtk_bt_gatts_reg_ind_t *p_reg_ind = NULL;

		p_gatts_app_srv->register_status = SRV_REGISTERED;
		p_gatts_evt = rtk_bt_event_create(RTK_BT_LE_GP_GATTS, RTK_BT_GATTS_EVT_REGISTER_SERVICE, sizeof(rtk_bt_gatts_reg_ind_t));
		if (p_gatts_evt) {
			p_reg_ind = (rtk_bt_gatts_reg_ind_t *)p_gatts_evt->data;
			p_reg_ind->app_id = p_gatts_app_srv->app_id;
			p_reg_ind->reg_status = ((ret == APP_RESULT_SUCCESS) ? RTK_BT_OK : RTK_BT_FAIL);
			rtk_bt_evt_indicate(p_gatts_evt, NULL);
		} else {
			printf("p_gatts_evt alloc fail \r\n");
		}
	} else {
		bt_stack_gatts_free_srv_tbl(rtk_service_table, p_gatts_app_srv->attr_count);
		bt_stack_gatts_delete_service_node(p_gatts_app_srv);
		printf("gatt_svc_add: failed \r\n");
		return RTK_BT_ERR_LOWER_STACK_API;
	}

#else

	if (!g_rtk_bt_gatts_priv->srv_registering) {
		// printf("register service calling \r\n");
		g_rtk_bt_gatts_priv->srv_registering = 1;
		if (true == p_gatts_app_srv->assgin_handle_flag) {
			add_service_ret = server_add_service_by_start_handle((T_SERVER_ID *)&p_gatts_app_srv->server_info,
										(uint8_t *)rtk_service_table,
										p_gatts_app_srv->attr_count * sizeof(T_ATTRIB_APPL),
										rtk_bt_gatts_cb, p_gatts_app_srv->start_handle);
		} else {
			add_service_ret = server_add_service((T_SERVER_ID *)&p_gatts_app_srv->server_info,
										(uint8_t *)rtk_service_table,
										p_gatts_app_srv->attr_count * sizeof(T_ATTRIB_APPL),
										rtk_bt_gatts_cb);
		}
		if (false == add_service_ret) {
			bt_stack_gatts_free_srv_tbl(rtk_service_table, p_gatts_app_srv->attr_count);
			bt_stack_gatts_delete_service_node(p_gatts_app_srv);
			printf("server_add_service: failed \r\n");
			return RTK_BT_ERR_LOWER_STACK_API;
		}
		p_gatts_app_srv->register_status = SRV_REGISTERING;
	} else {
		p_gatts_app_srv->register_status = SRV_WAIT_FOR_REG;
	}
#endif

	return 0;
}

void bt_stack_gatts_evt_indicate_mtu_exchange(uint16_t conn_handle, uint16_t mtu)
{
	rtk_bt_gatt_mtu_exchange_ind_t *p_gatt_mtu_exchange_ind = NULL;
	rtk_bt_evt_t *p_evt_t = NULL;

	p_evt_t = rtk_bt_event_create(RTK_BT_LE_GP_GATTS,RTK_BT_GATTS_EVT_MTU_EXCHANGE,sizeof(rtk_bt_gatt_mtu_exchange_ind_t));
	if(!p_evt_t)
		return;

	p_gatt_mtu_exchange_ind = (rtk_bt_gatt_mtu_exchange_ind_t *)p_evt_t->data;
	p_gatt_mtu_exchange_ind->result = RTK_BT_OK;
	p_gatt_mtu_exchange_ind->conn_handle = conn_handle;
	p_gatt_mtu_exchange_ind->mtu_size = mtu;

	rtk_bt_evt_indicate(p_evt_t, NULL);
}

static uint16_t bt_stack_gatts_read_rsp(void *param)
{
	rtk_bt_gatts_read_resp_param_t *p_read_rsp = (rtk_bt_gatts_read_resp_param_t *)param;
	struct rtk_bt_gatt_service * p_srv_node = NULL;
	T_APP_RESULT cause = APP_RESULT_SUCCESS; 
#if !RTK_BLE_MGR_LIB
	uint8_t conn_id;
	uint16_t mtu_size = 0, data_len = 0;
#endif

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_read_rsp->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;
		
	cause = (0 == p_read_rsp->err_code) ? APP_RESULT_SUCCESS : APP_RESULT_REJECT;
#if RTK_BLE_MGR_LIB
	if (!gatt_svc_read_confirm(p_read_rsp->conn_handle,
							   p_read_rsp->cid ? p_read_rsp->cid : GATT_FIXED_L2CAP_CID, /* just in case APP forget set cid */
							   (T_SERVER_ID)p_srv_node->server_info,
                           	   p_read_rsp->index, (uint8_t*)p_read_rsp->data,
							   p_read_rsp->len, p_read_rsp->len, cause))
		return RTK_BT_ERR_LOWER_STACK_API;
#else
	
	if (!le_get_conn_id_by_handle(p_read_rsp->conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);
	/* data len of read response is 0 to MTU-1, if over it, server_attr_read_confirm will fail */
	data_len = (mtu_size - 1) < p_read_rsp->len ? (mtu_size - 1) : p_read_rsp->len;
	if (!server_attr_read_confirm(conn_id, 
								  (T_SERVER_ID)p_srv_node->server_info, 
								  p_read_rsp->index,
								  (uint8_t*)p_read_rsp->data, data_len, cause))
		return RTK_BT_ERR_LOWER_STACK_API;
#endif

	return RTK_BT_OK;
}

static uint16_t bt_stack_gatts_write_rsp(void *param)
{
	rtk_bt_gatts_write_resp_param_t *p_write_rsp_t = (rtk_bt_gatts_write_resp_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	T_APP_RESULT cause;
#if !RTK_BLE_MGR_LIB
	uint8_t conn_id;
#endif
	
	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_write_rsp_t->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	cause = (T_APP_RESULT)((0 == p_write_rsp_t->err_code) ? APP_SUCCESS : APP_RESULT_REJECT);

#if RTK_BLE_MGR_LIB
	if (!gatt_svc_write_confirm(p_write_rsp_t->conn_handle,
								p_write_rsp_t->cid ? p_write_rsp_t->cid : GATT_FIXED_L2CAP_CID, /* just in case APP forget set cid */
								(T_SERVER_ID)p_srv_node->server_info,
								p_write_rsp_t->index, cause))
		return RTK_BT_ERR_LOWER_STACK_API;
#else

	if (!le_get_conn_id_by_handle(p_write_rsp_t->conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	if(RTK_BT_GATTS_WRITE_LONG == p_write_rsp_t->type){
		if (!server_exec_write_confirm(conn_id, cause, 0))
			return RTK_BT_ERR_LOWER_STACK_API;

	}else if (RTK_BT_GATTS_WRITE_REQ == p_write_rsp_t->type){
		if (!server_attr_write_confirm(conn_id, 
										p_srv_node->server_info,
										p_write_rsp_t->index, cause))
			return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif
	return RTK_BT_OK;
}

static uint16_t bt_stack_gatts_notify(void *param)
{
#if RTK_BLE_MGR_LIB
	uint16_t mtu_size = 0;
	rtk_bt_gatts_ntf_and_ind_param_t *p_notify_param = (rtk_bt_gatts_ntf_and_ind_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_req_t *p_req = NULL;
	uint16_t data_len = p_notify_param->len;
	uint16_t conn_handle = p_notify_param->conn_handle;
	uint8_t conn_id;
	uint16_t cid = p_notify_param->cid;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	if(cid == 0)/* just in case APP forget set cid, auto select channel is not ready now. */
		cid = GATT_FIXED_L2CAP_CID;

	p_req = (rtk_bt_gatts_req_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_gatts_req_t));
	if(!p_req)
		return RTK_BT_ERR_NO_MEMORY;

	memset(p_req, 0, sizeof(rtk_bt_gatts_req_t));

	p_req->app_id = p_notify_param->app_id;
	p_req->conn_handle = conn_handle;
	p_req->seq = p_notify_param->seq;
	p_req->index = p_notify_param->index;
	
	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_notify_param->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	if(cid == GATT_FIXED_L2CAP_CID) {
		le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);
		/* data len of handle_value_indication is 0 to MTU-3, if over it, gatt_svc_send_data will fail */
		data_len = (mtu_size - 3) < p_notify_param->len ? (mtu_size - 3) : p_notify_param->len;
	}
	/* In EATT, if dynamic created L2CAP channel is used here, please make sure data_len <= L2CAP_MTU_of_this_channel - 3*/

	if(gatt_svc_send_data(conn_handle, cid,
						  (T_SERVER_ID)p_srv_node->server_info, 
						  p_notify_param->index,
						  (uint8_t*)p_notify_param->data,
						  data_len, GATT_PDU_TYPE_NOTIFICATION)) {
		list_add_tail(&p_req->list, &g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_list);
		g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_ele_num++;
		return RTK_BT_OK;
	}

	return RTK_BT_ERR_LOWER_STACK_API;

#else

	uint16_t ret = 0;
	rtk_bt_gatts_ntf_and_ind_param_t *p_notify_param = (rtk_bt_gatts_ntf_and_ind_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_req_t *p_gatts_req = NULL;

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_notify_param->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	p_gatts_req = (rtk_bt_gatts_req_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_gatts_req_t));
	if(!p_gatts_req)
		return RTK_BT_ERR_NO_MEMORY;

	memset(p_gatts_req, 0, sizeof(rtk_bt_gatts_req_t));

	if(p_notify_param->len != 0){
		p_gatts_req->len = p_notify_param->len;
		p_gatts_req->data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,p_gatts_req->len);
		if(!p_gatts_req->data){
			osif_mem_free(p_gatts_req);
			return RTK_BT_ERR_NO_MEMORY;
		}else{
			memcpy(p_gatts_req->data,p_notify_param->data,p_notify_param->len);
		}
	}

	p_gatts_req->app_id = p_notify_param->app_id;
	p_gatts_req->conn_handle = p_notify_param->conn_handle;
	p_gatts_req->seq = p_notify_param->seq;
	p_gatts_req->index = p_notify_param->index;

	if (!g_rtk_bt_stack_credits) {
		ret = RTK_BT_ERR_NO_CREDITS;
	} else {
		ret = bt_stack_gatts_send_notify(p_gatts_req);
	}

	if(RTK_BT_OK != ret){
		if(p_gatts_req->len != 0)
			osif_mem_free(p_gatts_req->data);
		osif_mem_free(p_gatts_req);
		return ret;
	}

	return RTK_BT_OK;
#endif
}

static uint16_t bt_stack_gatts_indicate(void *param)
{
#if RTK_BLE_MGR_LIB
	uint16_t mtu_size = 0;
	rtk_bt_gatts_ntf_and_ind_param_t *p_indicate_param = (rtk_bt_gatts_ntf_and_ind_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_req_t *p_req = NULL;
	uint16_t data_len = p_indicate_param->len;
	uint16_t conn_handle = p_indicate_param->conn_handle;
	uint8_t conn_id;
	uint16_t cid = p_indicate_param->cid;

	if (!le_get_conn_id_by_handle(conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	if(cid == 0)/* just in case APP forget set cid, auto select channel is not ready now. */
		cid = GATT_FIXED_L2CAP_CID;
	
	p_req = (rtk_bt_gatts_req_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_gatts_req_t));
	if(!p_req)
		return RTK_BT_ERR_NO_MEMORY;

	memset(p_req, 0, sizeof(rtk_bt_gatts_req_t));

	p_req->app_id = p_indicate_param->app_id;
	p_req->conn_handle = conn_handle;
	p_req->seq = p_indicate_param->seq;
	p_req->index = p_indicate_param->index;
	
	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_indicate_param->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	if(cid == GATT_FIXED_L2CAP_CID) {
		le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);
		/* data len of handle_value_indication is 0 to MTU-3, if over it, gatt_svc_send_data will fail */
		data_len = (mtu_size - 3) < p_indicate_param->len ? (mtu_size - 3) : p_indicate_param->len;
	}
	/* In EATT, if dynamic created L2CAP channel is used here, please make sure data_len <= L2CAP_MTU_of_this_channel - 3*/

	if(gatt_svc_send_data(conn_handle, cid,
						  (T_SERVER_ID)p_srv_node->server_info, 
						  p_indicate_param->index,
						  (uint8_t*)p_indicate_param->data,
						  data_len, GATT_PDU_TYPE_INDICATION)) {
		list_add_tail(&p_req->list, &g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_list);
		g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_ele_num++;
		return RTK_BT_OK;
	}
	
	return RTK_BT_ERR_LOWER_STACK_API;

#else

	uint16_t ret = 0;
	rtk_bt_gatts_ntf_and_ind_param_t *p_indicate_param = (rtk_bt_gatts_ntf_and_ind_param_t *)param;
	struct rtk_bt_gatt_service *p_srv_node = NULL;
	rtk_bt_gatts_req_t *p_gatts_req = NULL;
	uint8_t conn_id;

	if (!le_get_conn_id_by_handle(p_indicate_param->conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	p_srv_node = bt_stack_gatts_find_service_node_by_app_id(p_indicate_param->app_id);
	if(!p_srv_node)
		return RTK_BT_ERR_NO_ENTRY;

	p_gatts_req = (rtk_bt_gatts_req_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_gatts_req_t));
	if(!p_gatts_req)
		return RTK_BT_ERR_NO_MEMORY;

	memset(p_gatts_req, 0, sizeof(rtk_bt_gatts_req_t));

	if(p_indicate_param->len != 0){
		p_gatts_req->len = p_indicate_param->len;
		p_gatts_req->data = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,p_gatts_req->len);
		if(!p_gatts_req->data){
			osif_mem_free(p_gatts_req);
			return RTK_BT_ERR_NO_MEMORY;
		}else{
			memcpy(p_gatts_req->data,p_indicate_param->data,p_indicate_param->len);
		}
	}

	p_gatts_req->app_id = p_indicate_param->app_id;
	p_gatts_req->conn_handle = p_indicate_param->conn_handle;
	p_gatts_req->seq = p_indicate_param->seq;
	p_gatts_req->index = p_indicate_param->index;

	/* 	if sending_list is not empty, means an indicate is sending, cannot send another any more
		so add the current indicate to pending_list */
	if(!list_empty(&g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_list)){
		list_add_tail(&p_gatts_req->list,&g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_list);
		g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_ele_num++;
		return RTK_BT_OK;
	}

	ret = bt_stack_gatts_send_indicate(p_gatts_req);
	if(RTK_BT_OK != ret) {
		if(p_gatts_req->len != 0)
			osif_mem_free(p_gatts_req->data);
		osif_mem_free(p_gatts_req);
		return ret;
	}

	return RTK_BT_OK;

#endif
}

#if !RTK_BLE_MGR_LIB
uint16_t bt_stack_le_gatts_get_tx_pending_num(uint16_t conn_handle,uint16_t *tx_pending_num)
{
	uint8_t conn_id;
	
	if (!le_get_conn_id_by_handle(conn_handle, &conn_id))
		return RTK_BT_ERR_PARAM_INVALID;

	if(tx_pending_num == NULL || conn_id >= GAP_MAX_LINKS) {
		return RTK_BT_ERR_PARAM_INVALID;
	}
	
	if(g_rtk_bt_gatts_priv) {
		//there is no pending queue in notify queue and no pri_pending_queue in indicate_queue
		*tx_pending_num = g_rtk_bt_gatts_priv->indicate_queue[conn_id].pending_ele_num + 
							g_rtk_bt_gatts_priv->indicate_queue[conn_id].sending_ele_num + 
								g_rtk_bt_gatts_priv->notify_queue[conn_id].sending_ele_num;
	}
	
    return RTK_BT_OK;
}
#endif

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_stack_gatts_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_OK;
	
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_GATTS)) {
		printf("Error: Gatts profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	switch(p_cmd->act){
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
			ret = bt_stack_gatts_notify(p_cmd->param);
			break;
		case RTK_BT_GATTS_ACT_INDICATE:
			ret = bt_stack_gatts_indicate(p_cmd->param);
			break;
		default:
			printf("bt_stack_le_act_handle:unknown act: %d \r\n",p_cmd->act);
			ret = RTK_BT_ERR_NO_CASE_ELEMENT;
			break;
	}

end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}

