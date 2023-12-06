/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#if defined(RTK_BREDR_SUPPORT) && RTK_BREDR_SUPPORT
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <string.h>
#include <stdio.h>
#include <dlist.h>
#include <osif.h>
#include <rtk_bt_br_gap.h>
#include <gap.h>
#include <gap_br.h>
#include <trace_app.h>
#include <remote.h>
#include <btm.h>
#include <sysm.h>
#include <bt_bond.h>

#define RTK_BT_STACK_DEFAULT_COD (0x18 | (0x04 << 8) | (0x24 << 16))
#define RTK_BT_STACK_DEFAULT_SUPVISIONTIMEOUT (0x1f40)

T_APP_DB app_db;

uint8_t bt_stack_br_gap_ready = false;
uint32_t class_of_device = RTK_BT_STACK_DEFAULT_COD;
uint16_t supervision_timeout = RTK_BT_STACK_DEFAULT_SUPVISIONTIMEOUT;
rtk_bt_br_inquiry_param_t inquiry_t = {0};
rtk_bt_br_page_param_t page_t = {0};
static uint8_t pin_code[4] = {1, 2, 3, 4};

extern void sys_mgr_event_handle(uint8_t event);

T_APP_BR_LINK *app_find_br_link(uint8_t *bd_addr)
{
	T_APP_BR_LINK *p_link = NULL;
	uint8_t	i;

	if (bd_addr != NULL) {
		for (i = 0; i < MAX_BR_LINK_NUM; i++) {
			if (app_db.br_link[i].used == true &&
				!memcmp(app_db.br_link[i].bd_addr, bd_addr, 6)) {
				p_link = &app_db.br_link[i];
				break;
			}
		}
	}

	return p_link;
}

static T_APP_BR_LINK *app_alloc_br_link(uint8_t *bd_addr)
{
	T_APP_BR_LINK *p_link = NULL;
	uint8_t	i;

	if (bd_addr != NULL) {
		for (i = 0; i < MAX_BR_LINK_NUM; i++) {
			if (app_db.br_link[i].used == false) {
				p_link = &app_db.br_link[i];

				p_link->used = true;
				p_link->id   = i;
				memcpy(p_link->bd_addr, bd_addr, 6);
				break;
			}
		}
	}

	return p_link;
}

static bool app_free_br_link(T_APP_BR_LINK *p_link)
{
	if (p_link != NULL) {
		if (p_link->used == true) {
			memset(p_link, 0, sizeof(T_APP_BR_LINK));
			return true;
		}
	}

	return false;
}

static void bt_stack_mgr_cback(T_BT_EVENT event_type, void *event_buf, uint16_t buf_len)
{
	(void)buf_len;
	T_BT_EVENT_PARAM *param = event_buf;
	bool handle = true;
	rtk_bt_evt_t *p_evt = NULL;

	switch (event_type) {
	case BT_EVENT_READY: {
		memcpy(app_db.factory_addr.addr, param->ready.bd_addr, 6);
		APP_PRINT_INFO1("bt_stack_mgr_cback: bt_ready, bd_addr %b",
						TRACE_BDADDR(param->ready.bd_addr));
		printf("bt_stack_mgr_cback: bt_ready, bd_addr %02x:%02x:%02x:%02x:%02x:%02x \r\n",
						param->ready.bd_addr[5], param->ready.bd_addr[4], param->ready.bd_addr[3], param->ready.bd_addr[2], 
						param->ready.bd_addr[1], param->ready.bd_addr[0]);
		bt_stack_br_gap_ready = true;
	}
	break;

	case BT_EVENT_INQUIRY_RESULT: {
		rtk_bt_br_inquiry_result_t *p_result = NULL;
		{
			APP_PRINT_INFO1("bt_stack_mgr_cback: inquiry result, bd_addr %b",
						TRACE_BDADDR(param->inquiry_result.bd_addr));
			{
				p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_INQUIRY_RESULT, sizeof(rtk_bt_br_inquiry_result_t));
				if (!p_evt) {
					printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
					handle = false;
					break;
				}
				p_result = (rtk_bt_br_inquiry_result_t *)p_evt->data;
				memcpy((void *)p_result, (void *)&param->inquiry_result, sizeof(rtk_bt_br_inquiry_result_t));
				/* Send event */
				if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
					handle = false;
					break;
				}
			}
		}
	}
	break;

	case BT_EVENT_REMOTE_NAME_RSP: {
		rtk_bt_br_remote_name_rsp_t *p_name_rsp = NULL;
		T_APP_BR_LINK *p_link = NULL;
		printf("bt_stack_mgr_cback: BT_EVENT_REMOTE_NAME_RSP \r\n");
		p_link = app_find_br_link(param->acl_conn_ind.bd_addr);
		if (!p_link) {
			printf("bt_stack_mgr_cback: no link found \r\n");
			break;
		}
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_REMOTE_NAME_RSP, sizeof(rtk_bt_br_remote_name_rsp_t));
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_name_rsp = (rtk_bt_br_remote_name_rsp_t *)p_evt->data;
			memcpy((void *)p_name_rsp, (void *)&param->remote_name_rsp, sizeof(rtk_bt_br_remote_name_rsp_t));
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_ACL_CONN_IND: {
		T_APP_BR_LINK *p_link = NULL;
		p_link = app_find_br_link(param->acl_conn_ind.bd_addr);
		if (p_link != NULL) {
			bt_acl_conn_reject(param->acl_conn_ind.bd_addr, BT_ACL_REJECT_UNACCEPTABLE_ADDR);
			printf("bt_stack_mgr_cback: already allocated link \r\n");
			break;
		} else {
			bt_acl_conn_accept(param->acl_conn_ind.bd_addr, BT_LINK_ROLE_SLAVE);
		}
		gap_br_set_radio_mode(GAP_RADIO_MODE_NONE_DISCOVERABLE, false, 0);
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_ACL_CONN_IND, 6);
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			memcpy((void *)p_evt->data, (void *)param->acl_conn_ind.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_LINK_KEY_REQ: {
		printf("bt_stack_mgr_cback: BT_EVENT_LINK_KEY_REQ \r\n");
		uint8_t found = 0;
		uint8_t link_key[16];
		T_BT_LINK_KEY_TYPE type;

		if (bt_bond_key_get(param->link_key_req.bd_addr, link_key, (uint8_t *)&type)) {
			bt_link_key_cfm(param->link_key_req.bd_addr, true, type, link_key);
			found = 1;
		} else {
			bt_link_key_cfm(param->link_key_req.bd_addr, false, type, link_key);
			found = 0;
		}
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_LINK_KEY_REQ, sizeof(uint8_t));
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			memcpy((void *)p_evt->data, (void *)&found, sizeof(uint8_t));
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_LINK_PIN_CODE_REQ: {
		bt_link_pin_code_cfm(param->link_pin_code_req.bd_addr, pin_code, 4, true);
	}
	break;

	case BT_EVENT_LINK_USER_CONFIRMATION_REQ: {
		gap_br_user_cfm_req_cfm(param->link_user_confirmation_req.bd_addr, GAP_CFM_CAUSE_ACCEPT);
	}
	break;

	case BT_EVENT_ACL_CONN_SUCCESS: {
		T_APP_BR_LINK *p_link = NULL;
		p_link = app_alloc_br_link(param->acl_conn_success.bd_addr);
		if (!p_link) {
			printf("bt_stack_mgr_cback: link alloc fail \r\n");
			break;
		}
		APP_PRINT_INFO0("link alloc success");
		printf("bt_stack_mgr_cback: link alloc success, hadnle 0x%x \r\n", param->acl_conn_success.handle);
		gap_br_set_radio_mode(GAP_RADIO_MODE_NONE_DISCOVERABLE, false, 0);
		memcpy((void *)&p_link->handle, (void *)&param->acl_conn_success.handle, sizeof(uint16_t));
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_ACL_CONN_SUCCESS, 6);
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			memcpy((void *)p_evt->data, (void *)param->acl_conn_success.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_ACL_CONN_SNIFF: {
		T_APP_BR_LINK *p_link = NULL;
		p_link = app_find_br_link(param->acl_conn_sniff.bd_addr);
		if (!p_link) {
			printf("bt_stack_mgr_cback: no link found \r\n");
			break;
		}
		APP_PRINT_INFO0("BT_EVENT_ACL_CONN_SNIFF");
		printf("bt_stack_mgr_cback: BT_EVENT_ACL_CONN_SNIFF \r\n");
		{
			rtk_bt_br_acl_sniff_t *p_sniff = NULL;
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_ACL_SNIFF, sizeof(rtk_bt_br_acl_sniff_t));
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_sniff = (rtk_bt_br_acl_sniff_t *)p_evt->data;
			memcpy((void *)p_sniff->bd_addr, (void *)param->acl_conn_sniff.bd_addr, 6);
			p_sniff->interval = param->acl_conn_sniff.interval;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_ACL_CONN_ACTIVE: {
		T_APP_BR_LINK *p_link = NULL;
		p_link = app_find_br_link(param->acl_conn_active.bd_addr);
		if (!p_link) {
			printf("bt_stack_mgr_cback: no link found \r\n");
			break;
		}
		APP_PRINT_INFO0("BT_EVENT_ACL_CONN_ACTIVE");
		printf("bt_stack_mgr_cback: BT_EVENT_ACL_CONN_ACTIVE \r\n");
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_ACL_ACTIVE, 6);
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			memcpy((void *)p_evt->data, (void *)param->acl_conn_active.bd_addr, 6);
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_ACL_CONN_DISCONN: {
		T_APP_BR_LINK *p_link = NULL;
		p_link = app_find_br_link(param->acl_conn_disconn.bd_addr);
		if (!p_link) {
			printf("bt_stack_mgr_cback: no link found \r\n");
			break;
		}
		app_free_br_link(p_link);
		gap_br_set_radio_mode(GAP_RADIO_MODE_VISIBLE_CONNECTABLE, false, 0);
		{
			rtk_bt_br_acl_disc_t *p_acl_disc_t = NULL;

			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_ACL_DISCONN, sizeof(rtk_bt_br_acl_disc_t));
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			p_acl_disc_t = (rtk_bt_br_acl_disc_t *)p_evt->data;
			memcpy((void *)p_acl_disc_t->bd_addr, (void *)param->acl_conn_disconn.bd_addr, 6);
			p_acl_disc_t->cause = param->acl_conn_disconn.cause;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	case BT_EVENT_LINK_KEY_INFO: {
		rtk_bt_br_bond_key_t *pbond_key_t = NULL;
		APP_PRINT_INFO0("BT_EVENT_LINK_KEY_INFO");
		printf("bt_stack_mgr_cback: BT_EVENT_LINK_KEY_INFO \r\n");
		bt_bond_key_set(param->link_key_info.bd_addr, param->link_key_info.link_key,
						param->link_key_info.key_type);
		{
			p_evt = rtk_bt_event_create(RTK_BT_BR_GP_GAP,RTK_BT_BR_GAP_LINK_KEY_INFO, sizeof(rtk_bt_br_bond_key_t));
			if (!p_evt) {
				printf("bt_stack_mgr_cback: evt_t allocate fail \r\n");
				handle = false;
				break;
			}
			pbond_key_t = (rtk_bt_br_bond_key_t *)p_evt->data;
			memcpy((void *)pbond_key_t->bd_addr, (void *)param->link_key_info.bd_addr, 6);
			memcpy((void *)pbond_key_t->link_key, (void *)param->link_key_info.link_key, 16);
			pbond_key_t->key_type = param->link_key_info.key_type;
			/* Send event */
			if (RTK_BT_OK != rtk_bt_evt_indicate(p_evt, NULL)) {
				handle = false;
				break;
			}
		}
	}
	break;

	default:
		handle = false;
		break;
	}

	if (handle == true) {
		// APP_PRINT_INFO1("bt_stack_mgr_cback: event_type 0x%04x", event_type);
		// printf("bt_stack_mgr_cback: event_type 0x%04x \r\n", event_type);
	}
}

static void bt_stack_br_gap_config(void *evt_msg_q)
{
	(void)evt_msg_q;
	uint16_t link_policy = GAP_LINK_POLICY_ROLE_SWITCH | GAP_LINK_POLICY_SNIFF_MODE;

	uint8_t radio_mode = GAP_RADIO_MODE_NONE_DISCOVERABLE;
	bool limited_discoverable = false;
	bool auto_accept_acl = false;

	uint8_t pair_mode = GAP_PAIRING_MODE_PAIRABLE;
	uint16_t auth_flags = GAP_AUTHEN_BIT_GENERAL_BONDING_FLAG | GAP_AUTHEN_BIT_SC_FLAG;
	uint8_t io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
	uint8_t oob_enable = false;
	uint8_t bt_mode = GAP_BT_MODE_21ENABLED;

	memset(&app_db, 0, sizeof(T_APP_DB));

	// printf("[LEGACY GAP]bt_gap_init\n");

	// legacy_set_gap_param(GAP_PARAM_LEGACY_NAME, GAP_DEVICE_NAME_LEN, device_name_legacy);
	gap_set_param(GAP_PARAM_BOND_BR_PAIRING_MODE, sizeof(uint8_t), &pair_mode);
	gap_set_param(GAP_PARAM_BOND_BR_AUTHEN_REQUIREMENTS_FLAGS, sizeof(uint16_t), &auth_flags);
	gap_set_param(GAP_PARAM_BOND_BR_IO_CAPABILITIES, sizeof(uint8_t), &io_cap);
	gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(uint8_t), &oob_enable);

	gap_br_set_param(GAP_BR_PARAM_BT_MODE, sizeof(uint8_t), &bt_mode);
	gap_br_set_param(GAP_BR_PARAM_COD, sizeof(uint32_t), &class_of_device);
	gap_br_set_param(GAP_BR_PARAM_LINK_POLICY, sizeof(uint16_t), &link_policy);
	gap_br_set_param(GAP_BR_PARAM_SUPV_TOUT, sizeof(uint16_t), &supervision_timeout);
	gap_br_set_param(GAP_BR_PARAM_AUTO_ACCEPT_ACL, sizeof(bool), &auto_accept_acl);

	gap_br_set_param(GAP_BR_PARAM_RADIO_MODE, sizeof(uint8_t), &radio_mode);
	gap_br_set_param(GAP_BR_PARAM_LIMIT_DISCOV, sizeof(bool), &limited_discoverable);

	gap_br_set_param(GAP_BR_PARAM_PAGE_SCAN_TYPE, sizeof(uint8_t), &page_t.pagescan_type);
	gap_br_set_param(GAP_BR_PARAM_PAGE_SCAN_INTERVAL, sizeof(uint16_t), &page_t.pagescan_interval);
	gap_br_set_param(GAP_BR_PARAM_PAGE_SCAN_WINDOW, sizeof(uint16_t), &page_t.pagescan_window);
	gap_br_set_param(GAP_BR_PARAM_PAGE_TIMEOUT, sizeof(uint16_t), &page_t.page_timeout);

	gap_br_set_param(GAP_BR_PARAM_INQUIRY_SCAN_TYPE, sizeof(uint8_t), &inquiry_t.inquiryscan_type);
	gap_br_set_param(GAP_BR_PARAM_INQUIRY_SCAN_INTERVAL, sizeof(uint16_t), &inquiry_t.inquiryscan_interval);
	gap_br_set_param(GAP_BR_PARAM_INQUIRY_SCAN_WINDOW, sizeof(uint16_t), &inquiry_t.inquiryscan_window);
	gap_br_set_param(GAP_BR_PARAM_INQUIRY_MODE, sizeof(uint8_t), &inquiry_t.inquiry_mode);
}

static void bt_stack_br_framework_init(void *evt_msg_q)
{
	// printf("[LEGACY GAP]framework_init\n");
	/* System Manager */
	sys_mgr_init(evt_msg_q);
	/* Initialize remote control manager*/
	remote_mgr_init(REMOTE_SESSION_ROLE_SINGLE);
	/* Bluetooth Manager */
	bt_mgr_init();
}

extern void gap_br_register_cb(P_GAP_BR_CBACK cback);

static void bt_stack_br_app_gap_init(void)
{
	// printf("[LEGACY GAP]app_gap_init\n");
	bt_mgr_cback_register(bt_stack_mgr_cback);
}

uint16_t bt_stack_br_gap_set_default_param(rtk_bt_br_gap_default_param_t *param)
{
	(void)param;
	uint16_t err = RTK_BT_OK;
	/* TODO */

	return err;
}

uint16_t bt_stack_br_gap_init(void *p_evt_msg_q)
{
	bt_stack_br_framework_init(p_evt_msg_q);
	bt_stack_br_gap_config(p_evt_msg_q);
	bt_stack_br_app_gap_init();

	return RTK_BT_OK;
}

uint16_t bt_stack_br_gap_wait_ready(void)
{
	do {
		osif_delay(100);
	} while (!bt_stack_br_gap_ready);

	return RTK_BT_OK;
}

uint16_t rtk_stack_framework_event_handler(uint8_t event)
{
	sys_mgr_event_handle(event);

	return RTK_BT_OK;
}

uint16_t bt_stack_br_gap_deinit(void)
{
	bt_stack_br_gap_ready = false;
	/* bt mgr deinit */
	bt_mgr_deinit();
	/* remote mgr deinit */
	remote_mgr_deinit();
	/* deinit bt framework */
	sys_mgr_deinit();

	return 0;
}

static uint16_t bt_stack_br_gap_get_device_addr(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_br_bd_addr_t *paddr = (rtk_bt_br_bd_addr_t *)param;

	paddr->type = RTK_BT_BR_ADDR_TYPE_PUBLIC;
	cause = gap_get_param(GAP_PARAM_BD_ADDR, (void *)paddr->bd_addr.addr);
	if (cause) {
		API_PRINT("bt_stack_br_gap_get_device_addr: cause = %x \r\n",cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_br_gap_set_device_name(void *param)
{
	T_GAP_CAUSE cause;
	char *pname = (char *)param;

	cause = gap_br_set_dev_name((uint8_t *)pname, strlen((const char*)pname));
	if (cause) {
		API_PRINT("bt_stack_br_gap_set_device_name: cause = %x \r\n",cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_br_gap_cfg_page_param(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_br_page_param_t *page_param = (rtk_bt_br_page_param_t *)param;
	uint8_t pagescan_type = page_param->pagescan_type;
	uint16_t pagescan_interval = page_param->pagescan_interval;
	uint16_t pagescan_window = page_param->pagescan_window;

	cause = gap_br_cfg_page_scan_param(pagescan_type, pagescan_interval, pagescan_window);
	if (cause) {
		API_PRINT("bt_stack_br_gap_cfg_page_param: cause = %x \r\n",cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_br_gap_cfg_inquiry_param(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_br_inquiry_param_t *inquiry_param = (rtk_bt_br_inquiry_param_t *)param;
	uint8_t inquiryscan_type = inquiry_param->inquiryscan_type;
	uint16_t inquiryscan_window = inquiry_param->inquiryscan_window;
	uint16_t inquiryscan_interval = inquiry_param->inquiryscan_interval;

	cause = gap_br_cfg_inquiry_scan_param(inquiryscan_type, inquiryscan_interval, inquiryscan_window);
	if (cause) {
		API_PRINT("bt_stack_br_gap_cfg_inquiry_param: cause = %x \r\n",cause);
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_br_gap_disconnect(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (gap_br_send_acl_disconn_req(bd_addr) == GAP_CAUSE_SUCCESS) {
		return RTK_BT_OK;
	}

	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_start_inquiry(void *param)
{
	rtk_bt_br_inquiry_start_t *start_inquiry_param = (rtk_bt_br_inquiry_start_t *)param;

	gap_br_start_inquiry(start_inquiry_param->limited_inquiry, start_inquiry_param->timeout);
	
	return 0;
}

static uint16_t bt_stack_br_sm_set_security_param(void *param)
{
	uint16_t ret = RTK_BT_FAIL;
	T_GAP_CAUSE cause;
	rtk_bt_br_security_param_t *p_sec_param = (rtk_bt_br_security_param_t *)param;
	uint16_t auth_flags = 0;
	uint8_t pair_mode = GAP_PAIRING_MODE_PAIRABLE;

	/* radio mode should be set none discoverable before config br secure connection */
	gap_br_set_radio_mode(GAP_RADIO_MODE_NONE_DISCOVERABLE, false, 0);
	cause = gap_set_param(GAP_PARAM_BOND_BR_PAIRING_MODE, sizeof(uint8_t),
							&pair_mode);
	if (cause) {
		goto exit;
	}

	cause = gap_set_param(GAP_PARAM_BOND_BR_IO_CAPABILITIES, sizeof(uint8_t),
							&p_sec_param->io_cap);
	if (cause) {
		goto exit;
	}

	cause = gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(uint8_t),
							&p_sec_param->oob_data_flag);
	if (cause) {
		goto exit;
	}
	auth_flags = p_sec_param->bond_flag | p_sec_param->br_bond_flag << 1 |
				 p_sec_param->mitm_flag << 2 | p_sec_param->sec_pair_flag << 3 |
				 p_sec_param->br_sec_pair_flag << 10;
	cause = gap_set_param(GAP_PARAM_BOND_BR_AUTHEN_REQUIREMENTS_FLAGS, sizeof(uint16_t),
							&auth_flags);
	if (cause) {
		goto exit;
	}
	cause = gap_set_pairable_mode();
	if (!cause) {
		ret = RTK_BT_OK;
	}

exit:
	gap_br_set_radio_mode(GAP_RADIO_MODE_VISIBLE_CONNECTABLE, false, 0);
	return ret;
}

static uint16_t bt_stack_br_gap_get_remote_name(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	gap_br_get_remote_name(bd_addr);
	
	return 0;
}

/* get supported max bond num */
static uint16_t bt_stack_br_gap_max_bond_num_get(void *param)
{
	uint8_t *pmax_bond_num = (uint8_t *)param;

	*pmax_bond_num = bt_max_bond_num_get();
	
	return RTK_BT_OK;
}

/* get bond device num */
static uint16_t bt_stack_br_gap_bond_num_get(void *param)
{
	uint8_t *pbond_num = (uint8_t *)param;

	*pbond_num = bt_bond_num_get();
	
	return RTK_BT_OK;
}

static uint16_t bt_stack_br_gap_bond_addr_get(void *param)
{
	rtk_bt_br_bond_addr_t *pbond_addr_t = (rtk_bt_br_bond_addr_t *)param;

	if (bt_bond_addr_get(pbond_addr_t->priority, pbond_addr_t->bd_addr)) {
		return RTK_BT_OK;
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_bond_index_get(void *param)
{
	rtk_bt_br_bond_index_t *pbond_index_t = (rtk_bt_br_bond_index_t *)param;

	if (bt_bond_index_get(pbond_index_t->bd_addr, &pbond_index_t->index)) {
		return RTK_BT_OK;
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_bond_key_get(void *param)
{
	rtk_bt_br_bond_key_t *pbond_key_t = (rtk_bt_br_bond_key_t *)param;

	if (bt_bond_key_get(pbond_key_t->bd_addr, pbond_key_t->link_key, &pbond_key_t->key_type)) {
		return RTK_BT_OK;
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_bond_key_set(void *param)
{
	rtk_bt_br_bond_key_t *pbond_key_t = (rtk_bt_br_bond_key_t *)param;

	if (bt_bond_key_set(pbond_key_t->bd_addr, pbond_key_t->link_key, pbond_key_t->key_type)) {
		return RTK_BT_OK;
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_bond_priority_set(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_bond_priority_set(bd_addr)) {
		return RTK_BT_OK;
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_bond_delete(void *param)
{
	uint8_t *bd_addr = (uint8_t *)param;

	if (bt_bond_delete(bd_addr)) {
		return RTK_BT_OK;
	}
	
	return RTK_BT_FAIL;
}

static uint16_t bt_stack_br_gap_bond_clear(void *param)
{
	(void)param;

	bt_bond_clear();
	
	return 0;
}

static uint16_t bt_stack_br_gap_set_pincode(void *param, uint32_t length)
{
	if (length > 4) {
		printf("bt_stack_br_gap_set_pincode: length is invalid %d \r\n", (int)length);
		return 1;
	}
	memcpy((void *)pin_code, param, length);
	
	return 0;
}

static uint16_t bt_stack_br_gap_set_radio_mode(void *param)
{
	T_GAP_CAUSE cause;
	uint8_t radio_mode = *((uint8_t*)param);

	if ((radio_mode == 0) || (radio_mode > 0x04)) {
		printf("bt_stack_br_gap_set_radio_mode: radio mode error 0x%x \r\n", radio_mode);
		return 1;
	}
	cause = gap_br_set_radio_mode(radio_mode, false, 0);
	if (GAP_CAUSE_SUCCESS == cause) {
		return 0;
	} else {
		printf("bt_stack_br_gap_set_radio_mode: error 0x%x \r\n", cause);
		return 1;
	}
}

static uint16_t bt_stack_br_gap_set_sniff_mode(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_br_sniff_mode_t *p_sniff_mode_t = (rtk_bt_br_sniff_mode_t *)param;

	if (p_sniff_mode_t->enable) {
		printf("bt_stack_br_gap_set_sniff_mode: enable \r\n");
		cause = gap_br_enter_sniff_mode(p_sniff_mode_t->bd_addr,
									p_sniff_mode_t->min_interval,
									p_sniff_mode_t->max_interval,
									p_sniff_mode_t->sniff_attempt,
									p_sniff_mode_t->sniff_timeout);
	} else {
		printf("bt_stack_br_gap_set_sniff_mode: disable \r\n");
		cause = gap_br_exit_sniff_mode(p_sniff_mode_t->bd_addr);
	}

	if (GAP_CAUSE_SUCCESS == cause) {
		return 0;
	} else {
		printf("bt_stack_br_gap_set_sniff_mode: error 0x%x \r\n", cause);
		return 1;
	}
}

uint16_t bt_stack_br_gap_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = 0;
	API_PRINT("bt_stack_br_gap_act_handle: act = %d \r\n", p_cmd->act);
	switch(p_cmd->act){

		case RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR:
			API_PRINT("RTK_BT_BR_GAP_ACT_GET_DEVICE_ADDR \r\n");
			ret = bt_stack_br_gap_get_device_addr(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_SET_DEVICE_NAME:
			API_PRINT("RTK_BT_BR_GAP_ACT_SET_DEVICE_NAME \r\n");
			ret = bt_stack_br_gap_set_device_name(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_CFG_PAGE_PARAM:
			API_PRINT("RTK_BT_BR_GAP_ACT_CFG_PAGE_PARAM \r\n");
			ret = bt_stack_br_gap_cfg_page_param(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_CFG_INQUIRY_PARAM:
			API_PRINT("RTK_BT_BR_GAP_ACT_CFG_INQUIRY_PARAM \r\n");
			ret = bt_stack_br_gap_cfg_inquiry_param(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_DISCONNECT:
			API_PRINT("RTK_BT_BR_GAP_ACT_DISCONNECT \r\n");
			ret = bt_stack_br_gap_disconnect(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_START_INQUIRY:
			API_PRINT("RTK_BT_BR_GAP_ACT_START_INQUIRY \r\n");
			ret = bt_stack_br_gap_start_inquiry(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_SET_SEC_PARAM:
			API_PRINT("RTK_BT_BR_GAP_ACT_SET_SEC_PARAM \r\n");
			ret = bt_stack_br_sm_set_security_param(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_GET_REMOTE_NAME:
			API_PRINT("RTK_BT_BR_GAP_ACT_GET_REMOTE_NAME \r\n");
			ret = bt_stack_br_gap_get_remote_name(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_MAX_BOND_NUM_GET:
			API_PRINT("RTK_BT_BR_GAP_ACT_MAX_BOND_NUM_GET \r\n");
			ret = bt_stack_br_gap_max_bond_num_get(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_NUM_GET:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_NUM_GET \r\n");
			ret = bt_stack_br_gap_bond_num_get(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_ADDR_GET:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_ADDR_GET \r\n");
			ret = bt_stack_br_gap_bond_addr_get(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_INDEX_GET:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_INDEX_GET \r\n");
			ret = bt_stack_br_gap_bond_index_get(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_KEY_GET:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_KEY_GET \r\n");
			ret = bt_stack_br_gap_bond_key_get(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_KEY_SET:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_KEY_SET \r\n");
			ret = bt_stack_br_gap_bond_key_set(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_PRIORITY_SET:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_PRIORITY_SET \r\n");
			ret = bt_stack_br_gap_bond_priority_set(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_DELETE:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_DELETE \r\n");
			ret = bt_stack_br_gap_bond_delete(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_BOND_CLEAR:
			API_PRINT("RTK_BT_BR_GAP_ACT_BOND_CLEAR \r\n");
			ret = bt_stack_br_gap_bond_clear(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_SET_PINCODE:
			API_PRINT("RTK_BT_BR_GAP_ACT_SET_PINCODE \r\n");
			ret = bt_stack_br_gap_set_pincode(p_cmd->param, p_cmd->param_len);
			break;

		case RTK_BT_BR_GAP_ACT_SET_RADIO_MODE:
			API_PRINT("RTK_BT_BR_GAP_ACT_SET_RADIO_MODE \r\n");
			ret = bt_stack_br_gap_set_radio_mode(p_cmd->param);
			break;

		case RTK_BT_BR_GAP_ACT_SET_SNIFF_MODE:
			API_PRINT("RTK_BT_BR_GAP_ACT_SET_SNIFF_MODE \r\n");
			ret = bt_stack_br_gap_set_sniff_mode(p_cmd->param);
			break;

		default:
			printf("bt_stack_le_act_handle: unknown act: %d \r\n", p_cmd->act);
			ret = 0;
			break;
	}

	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);

	return ret;
}

#endif