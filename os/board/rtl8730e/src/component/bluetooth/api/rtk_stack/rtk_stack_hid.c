/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- */
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <stdio.h>
#include <string.h>
#include <dlist.h>
#include <osif.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_hid.h>
#include <gap_br.h>
#include <btm.h>
#include <trace_app.h>
#include <remote.h>
#include <bt_hid.h>

static uint8_t hid_role;
static uint8_t remote_addr[6] = {0};
static uint8_t *hid_data = NULL;
static uint8_t data[100] = {0};
static uint16_t data_len = 0;
static rtk_bt_hid_des_t *pdescriptor = NULL;
static T_BT_HID_REPORT_TYPE report_type = BT_HID_REPORT_TYPE_RESERVED;
extern T_APP_BR_LINK *app_find_br_link(uint8_t *bd_addr);

static void app_hid_bt_cback(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len)
{
	(void)buf_len;
	T_BT_EVENT_PARAM *param = event_buf;
	T_APP_BR_LINK *p_link;
	bool handle = true;
	rtk_bt_evt_t *p_evt = NULL;

	switch (event_type) {
	
	case BT_EVENT_HID_CONN_IND: {
		rtk_bt_hid_conn_ind_t *p_hid_conn_ind = NULL;

		p_link = app_find_br_link(param->hid_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_hid_connect_cfm(p_link->bd_addr, true);
			APP_PRINT_INFO0("HID p_link confirmed");
			printf("app_hid_bt_cback: HID p_link confirmed \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HID,RTK_BT_HID_EVT_CONN_CMPL,sizeof(rtk_bt_hid_conn_ind_t));
				if (!p_evt) {
					printf("app_hid_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hid_conn_ind = (rtk_bt_hid_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hid_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HID p_link is NULL");
			printf("app_hid_bt_cback: HID p_link is NULL \r\n");
		}
	}
	break;

	case BT_EVENT_HID_CONN_CMPL: {
		rtk_bt_hid_conn_ind_t *p_hid_conn_ind = NULL;

		printf("app_hid_bt_cback: HID Connected \r\n");
		p_link = app_find_br_link(param->hid_conn_cmpl.bd_addr);
		if (p_link != NULL) {
			memcpy((void *)remote_addr, (void *)param->hid_conn_cmpl.bd_addr, 6);
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HID,RTK_BT_A2DP_EVT_CONN_CMPL,sizeof(rtk_bt_hid_conn_ind_t));
				if (!p_evt) {
					printf("app_hid_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hid_conn_ind = (rtk_bt_hid_conn_ind_t *)p_evt->data;
				memcpy((void *)p_hid_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		} else {
			APP_PRINT_INFO0("HID p_link is NULL");
			printf("app_hid_bt_cback: no acl link found \r\n");
		}
	}
	break;

	case BT_EVENT_HID_DISCONN_CMPL: {
		rtk_bt_hid_disconn_ind_t *p_hid_disconn_ind = NULL;
		{
			printf("app_hid_bt_cback: HID Disconnected \r\n");
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_HID,RTK_BT_HID_EVT_DISCONN_CMPL,sizeof(rtk_bt_hid_disconn_ind_t));
				if (!p_evt) {
					printf("app_hid_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_hid_disconn_ind = (rtk_bt_hid_disconn_ind_t *)p_evt->data;
				memcpy((void *)p_hid_disconn_ind->bd_addr, (void *)param->hid_disconn_cmpl.bd_addr, 6);
				p_hid_disconn_ind->cause = param->hid_disconn_cmpl.cause;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		}
	}
	break;

	case BT_EVENT_HID_CONTROL_DATA_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_CONTROL_DATA_IND \r\n");
	}
	break;

	case BT_EVENT_HID_GET_REPORT_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_GET_REPORT_IND \r\n");
		if (hid_data) {
			osif_mem_free(hid_data);
		}
		data_len = 1 + param->hid_get_report_ind.report_size;
		hid_data = osif_mem_alloc(RAM_TYPE_DATA_ON, data_len);
		hid_data[0] = param->hid_get_report_ind.report_id;
		report_type = (T_BT_HID_REPORT_TYPE)param->hid_get_report_ind.report_type;
		memcpy(&hid_data[1], data, param->hid_get_report_ind.report_size);
	}
	break;

	case BT_EVENT_HID_SET_REPORT_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_SET_REPORT_IND \r\n");
	}
	break;

	case BT_EVENT_HID_GET_PROTOCOL_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_GET_PROTOCOL_IND \r\n");
	}
	break;

	case BT_EVENT_HID_SET_PROTOCOL_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_SET_PROTOCOL_IND \r\n");
	}
	break;

	case BT_EVENT_HID_SET_IDLE_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_SET_IDLE_IND \r\n");
	}
	break;

	case BT_EVENT_HID_INTERRUPT_DATA_IND: {
		printf("app_hid_bt_cback: BT_EVENT_HID_INTERRUPT_DATA_IND \r\n");
	}
	break;

	default: {
		APP_PRINT_INFO1("app_hid_bt_cback: default event_type 0x%04x", event_type);
		// printf("app_hid_bt_cback: default event_type 0x%04x \r\n", event_type);
		handle = false;
	}
	break;
	}

	if (handle == true) {
		// APP_PRINT_INFO1("app_hid_bt_cback: event_type 0x%04x", event_type);
		// printf("app_hid_bt_cback: event_type 0x%04x \r\n", event_type);
	}
}

static uint16_t bt_stack_hid_disconnect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (bt_hid_disconnect_req(bd_addr)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hid_get_report_rsp(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(bd_addr);
	if (p_link != NULL) {
		if (bt_hid_control_get_report_rsp(bd_addr,
											report_type,
											hid_data,
											data_len)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_hid_input_data_send(void *param)
{
	rtk_bt_hid_input_data_t *pdata_t = (rtk_bt_hid_input_data_t *)param;
	T_APP_BR_LINK *p_link;

	p_link = app_find_br_link(pdata_t->bd_addr);
	if (p_link != NULL) {
		if (bt_hid_interrupt_data_send(pdata_t->bd_addr, BT_HID_REPORT_TYPE_INPUT,
										pdata_t->data, pdata_t->len)) {
			return RTK_BT_OK;
		}
	}

	return RTK_BT_FAIL;
}

uint16_t bt_stack_hid_descriptor_add(void *des, uint32_t length)
{
	if (pdescriptor) {
		printf("%s(): pdescriptor is not NULL !\r\n", __func__);
		return RTK_BT_FAIL;
	}
	/* allocate descriptor mem */
	pdescriptor = (rtk_bt_hid_des_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_hid_des_t));
	if (!pdescriptor) {
		printf("%s(): pdescriptor allocate fail !\r\n", __func__);
		return RTK_BT_FAIL;
	}
	/* memcpy record data */
	memcpy((void *)pdescriptor->des, des, length);
	pdescriptor->len = length;

	return RTK_BT_OK;
}

uint16_t bt_stack_hid_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	API_PRINT("bt_stack_hid_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act){

		case RTK_BT_HID_ACT_DISCONNECT:
			ret = bt_stack_hid_disconnect(p_cmd->param);
			break;

		case RTK_BT_HID_ACT_GET_REPORT_RSP:
			ret = bt_stack_hid_get_report_rsp(p_cmd->param);
			break;

		case RTK_BT_HID_ACT_INPUT_DATA_SEND:
			ret = bt_stack_hid_input_data_send(p_cmd->param);
			break;

		default:
			printf("bt_stack_hid_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

uint16_t bt_stack_hid_init(uint8_t role)
{
	printf("[HID]app_hid_init\n");

	if (!pdescriptor) {
		printf("%s(): pdescriptor is NULL !\r\n", __func__);
		return RTK_BT_FAIL;
	}
	if (!bt_hid_init(1, true)) {
		printf("[HID]bt_hid_init FAIL\n");
		return RTK_BT_FAIL;
	}
	bt_hid_descriptor_set(pdescriptor->des, pdescriptor->len);
	bt_mgr_cback_register(app_hid_bt_cback);
	hid_role = role;

	return RTK_BT_OK;
}

void bt_stack_hid_deinit(void)
{
	printf("[HID]app_hid_init need to do\n");
	if (pdescriptor) {
		osif_mem_free((void *)pdescriptor);
	}
}

#endif