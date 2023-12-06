/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <stdio.h>
#include <string.h>
#include <dlist.h>
#include <osif.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_avrcp.h>
#include <gap_br.h>
#include <trace_app.h>
#include <remote.h>
#include <btm.h>
#include <bt_avrcp.h>

static uint8_t volume_req = RTK_BT_DEFAULT_ABSOLUTE_VOLUME;
extern T_APP_BR_LINK *app_find_br_link(uint8_t *bd_addr);

static void app_avrcp_bt_cback(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len)
{
	(void)buf_len;
	T_BT_EVENT_PARAM *param = event_buf;
	T_APP_BR_LINK *p_link;
	bool handle = true;
	rtk_bt_evt_t *p_evt = NULL;

	switch (event_type) {
	case BT_EVENT_AVRCP_CONN_IND: {
		rtk_bt_avrcp_conn_ind_t *p_avrcp_conn_ind = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_CONN_IND");
		printf("app_avrcp_bt_cback: T_EVENT_AVRCP_CONN_IND \r\n");
		p_link = app_find_br_link(param->avrcp_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_avrcp_connect_cfm(p_link->bd_addr, true);
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_CONN_IND,sizeof(rtk_bt_avrcp_conn_ind_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_avrcp_conn_ind = (rtk_bt_avrcp_conn_ind_t *)p_evt->data;
			memcpy((void *)p_avrcp_conn_ind->bd_addr, (void *)p_link->bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_CONN_CMPL: {
		APP_PRINT_INFO0("BT_EVENT_AVRCP_CONN_CMPL");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_CONN_CMPL \r\n");
		p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_CONN_CMPL,0);
		if (!p_evt) {
			printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
			handle = false;
			break;
		}
		/* Send event */
		if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
			handle = false;
			break;
		}
	}
	break;

	case BT_EVENT_AVRCP_DISCONN_CMPL: {
		p_link = app_find_br_link(param->avrcp_disconn_cmpl.bd_addr);
		if (!p_link) {
			printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_DISCONN_CMPL no link found \r\n");
			break;
		}
		p_link->avrcp_play_status = BT_AVRCP_PLAY_STATUS_STOPPED;
		APP_PRINT_INFO0("BT_EVENT_AVRCP_DISCONN_CMPL");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_DISCONN_CMPL \r\n");
	}
	break;

	case BT_EVENT_AVRCP_PLAY_STATUS_CHANGED_REG_REQ: {
		APP_PRINT_INFO0("BT_EVENT_AVRCP_PLAY_STATUS_CHANGED_REG_REQ");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_PLAY_STATUS_CHANGED_REG_REQ \r\n");
		p_link = app_find_br_link(param->avrcp_reg_play_status_changed.bd_addr);
		if (p_link != NULL) {
			if (p_link->is_streaming) {
				bt_avrcp_play_status_change_register_rsp(p_link->bd_addr, BT_AVRCP_PLAY_STATUS_PLAYING);
			}
			else {
				bt_avrcp_play_status_change_register_rsp(p_link->bd_addr, BT_AVRCP_PLAY_STATUS_PAUSED);
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_PLAY_STATUS_RSP:
	case BT_EVENT_AVRCP_PLAY_STATUS_CHANGED: {
		rtk_bt_avrcp_sub_event_t *p_avrcp_sub_event_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_PLAY_STATUS_CHANGED");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_PLAY_STATUS_CHANGED \r\n");
		p_link = app_find_br_link(param->avrcp_play_status_changed.bd_addr);
		if (p_link != NULL) {
			if (p_link->avrcp_play_status != param->avrcp_play_status_changed.play_status) {
				p_link->avrcp_play_status = param->avrcp_play_status_changed.play_status;
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED_REG_REQ,sizeof(rtk_bt_avrcp_sub_event_t));
				if (!p_evt) {
					printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_avrcp_sub_event_t = (rtk_bt_avrcp_sub_event_t *)p_evt->data;
				p_avrcp_sub_event_t->avrcp_play_status =  param->avrcp_play_status_changed.play_status;
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_PLAY: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_PLAY");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_PLAY \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_PLAY_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_play.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_PAUSE: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_PAUSE");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_PAUSE \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_PAUSE_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_pause.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_STOP: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_STOP");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_STOP \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_STOP_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_stop.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_FORWARD: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_FORWARD");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_FORWARD \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_FORWARD_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_forward.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_BACKWARD: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_BACKWARD");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_BACKWARD \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_BACKWARD_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_backward.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_FAST_FORWARD_START: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_FAST_FORWARD_START");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_FAST_FORWARD_START \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_FAST_FORWARD_START_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_fast_forward_start.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_FAST_FORWARD_STOP: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_FAST_FORWARD_STOP");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_FAST_FORWARD_STOP \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_FAST_FORWARD_STOP_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_fast_forward_stop.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_REWIND_START: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_REWIND_START");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_REWIND_START \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_REWIND_START_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_rewind_start.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_REWIND_STOP: {
		rtk_bt_avrcp_digital_interface_command_t *p_passthrough_cmd_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_REWIND_STOP");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_REWIND_STOP \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_REWIND_STOP_REQ_EVENT,sizeof(rtk_bt_avrcp_digital_interface_command_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_passthrough_cmd_t = (rtk_bt_avrcp_digital_interface_command_t *)p_evt->data;
			memcpy((void *)p_passthrough_cmd_t->bd_addr, (void *)param->avrcp_rewind_stop.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_ABSOLUTE_VOLUME_SET: {
		rtk_bt_avrcp_absolute_volume_set_t *p_avrcp_absolute_volume_set_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_ABSOLUTE_VOLUME_SET");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_ABSOLUTE_VOLUME_SET \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_ABSOLUTE_VOLUME_SET,sizeof(rtk_bt_avrcp_absolute_volume_set_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_avrcp_absolute_volume_set_t = (rtk_bt_avrcp_absolute_volume_set_t *)p_evt->data;
			memcpy((void *)p_avrcp_absolute_volume_set_t->bd_addr, (void *)param->avrcp_absolute_volume_set.bd_addr, 6);
			p_avrcp_absolute_volume_set_t->volume = param->avrcp_absolute_volume_set.volume;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_VOLUME_CHANGED: {
		rtk_bt_avrcp_volume_t *p_avrcp_volume_t = NULL;

		APP_PRINT_INFO0("BT_EVENT_AVRCP_VOLUME_CHANGED");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_VOLUME_CHANGED \r\n");
		p_link = app_find_br_link(param->avrcp_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_VOLUME_CHANGED,sizeof(rtk_bt_avrcp_volume_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_avrcp_volume_t = (rtk_bt_avrcp_volume_t *)p_evt->data;
			p_avrcp_volume_t->volume = param->avrcp_volume_changed.volume;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_AVRCP_REG_VOLUME_CHANGED: {
		rtk_bt_avrcp_volume_change_req_t *p_volume_t = NULL;
		uint8_t cb_ret = 0;
	
		APP_PRINT_INFO0("BT_EVENT_AVRCP_REG_VOLUME_CHANGED");
		printf("app_avrcp_bt_cback: BT_EVENT_AVRCP_REG_VOLUME_CHANGED \r\n");
		p_link = app_find_br_link(param->avrcp_reg_volume_changed.bd_addr);
		if (p_link != NULL) {
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_AVRCP,RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED,sizeof(rtk_bt_avrcp_volume_change_req_t));
			if (!p_evt) {
				printf("app_avrcp_bt_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_volume_t = (rtk_bt_avrcp_volume_change_req_t *)p_evt->data;
			memcpy((void *)p_volume_t->bd_addr, (void *)param->avrcp_reg_volume_changed.bd_addr, 6);
			p_volume_t->volume = volume_req;
			/* Send event direct calling */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, &cb_ret)) {
				handle = false;
				break;
			}
			bt_avrcp_volume_change_register_rsp(p_link->bd_addr, volume_req);
		}
	}
	break;

	default: {
		handle = false;
	}
	break;

	}	
	if (handle == true) {
		APP_PRINT_INFO1("app_avrcp_bt_cback: event_type 0x%04x", event_type);
		printf("app_avrcp_bt_cback: event_type 0x%04x \r\n", event_type);
	}
}

static uint16_t bt_stack_avrcp_connect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_connect_req(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_disconnect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_disconnect_req(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_play(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_play(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_pause(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_pause(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_stop(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_stop(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_forward(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_forward(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_backword(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_backward(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_rewind_start(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_rewind_start(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_rewind_stop(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_rewind_stop(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_fast_forward_start(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_fast_forward_start(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_fast_forward_stop(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_avrcp_fast_forward_stop(bd_addr)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_absolute_volume_set(void *param)
{
	rtk_bt_avrcp_absolute_volume_set_t *p_volume_t = (rtk_bt_avrcp_absolute_volume_set_t *)param;

	if (bt_avrcp_absolute_volume_set(p_volume_t->bd_addr, p_volume_t->volume)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_avrcp_volume_change_req(void *param)
{
	rtk_bt_avrcp_volume_change_req_t *p_volume_t = (rtk_bt_avrcp_volume_change_req_t *)param;

	volume_req = p_volume_t->volume;
	if (bt_avrcp_volume_change_req(p_volume_t->bd_addr, p_volume_t->volume)) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

uint16_t bt_stack_avrcp_act_handle(rtk_bt_cmd_t *p_cmd)
{
	int16_t ret = 0;
	APP_PRINT_INFO1("bt_stack_avrcp_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act){

		case RTK_BT_AVRCP_ACT_CONNECT:
			ret = bt_stack_avrcp_connect(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_DISCONNECT:
			ret = bt_stack_avrcp_disconnect(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_PLAY:
			ret = bt_stack_avrcp_play(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_PAUSE:
			ret = bt_stack_avrcp_pause(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_STOP:
			ret = bt_stack_avrcp_stop(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_FORWARD:
			ret = bt_stack_avrcp_forward(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_BACKWARD:
			ret = bt_stack_avrcp_backword(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_REWIND_START:
			ret = bt_stack_avrcp_rewind_start(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_REWIND_STOP:
			ret = bt_stack_avrcp_rewind_stop(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_FAST_FORWARD_START:
			ret = bt_stack_avrcp_fast_forward_start(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_FAST_FORWARD_STOP:
			ret = bt_stack_avrcp_fast_forward_stop(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_ABSOLUTE_VOLUME_SET:
			ret = bt_stack_avrcp_absolute_volume_set(p_cmd->param);
			break;

		case RTK_BT_AVRCP_ACT_VOLUME_CHANGE_REQ:
			ret = bt_stack_avrcp_volume_change_req(p_cmd->param);
			break;

		default:
			printf("bt_stack_avrcp_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

uint16_t bt_stack_avrcp_init(void)
{
	printf("[AVRCP]app_avrcp_init\n");

	bt_avrcp_init(1);
	bt_mgr_cback_register(app_avrcp_bt_cback);

	return RTK_BT_OK;
}

extern void avrcp_deinit(void);

void bt_stack_avrcp_deinit(void)
{
	printf("[AVRCP]app_avrcp_deinit\n");

	avrcp_deinit();
}

#endif