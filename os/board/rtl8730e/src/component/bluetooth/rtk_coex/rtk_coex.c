/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <osif.h>
#include "hci_platform.h"
#include "rtk_bt_gap.h"
#include "rtk_coex.h"
#if defined(CONFIG_BT_COEXIST)
#include "rtw_coex_host_api.h"
#else
//#error "Please Enable Coexist!!!"
#endif
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "bt_debug.h"
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#if defined(HCI_BT_COEX_ENABLE) && HCI_BT_COEX_ENABLE

struct rtk_bt_coex_priv_t *p_rtk_bt_coex_priv = NULL;
bool bt_coex_initialized = false;

static struct rtk_bt_coex_conn_t  *bt_coex_find_link_by_handle(uint16_t conn_handle)
{
	bool b_find = false;
	struct list_head *plist = NULL;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while (plist != &p_rtk_bt_coex_priv->conn_list) {
			p_conn = (struct rtk_bt_coex_conn_t *)plist;
			if ((p_conn->conn_handle & 0xFFF) == conn_handle) {
				b_find = true;
				break;
			} else {
				plist = plist->next;
			}
		}
	}

	if (b_find) {
		return p_conn;
	} else {
		return NULL;
	}
}

static void bt_coex_send_vendor_cmd(uint16_t cmd_id, uint8_t *pbuf, uint8_t len)
{
	DBG_BT_COEX("bt_coex_send_vendor_cmd -----> cmd_id 0x%x\r\n", cmd_id);
	rtk_bt_gap_vendor_cmd_param_t param = {
		.op = cmd_id,
		.len = len,
		.cmd_param = pbuf
	};

	DBG_BT_COEX("bt_coex_send_vendor_cmd: len = %d\r\n", len);
	DBG_BT_COEX_DUMP("bt_coex_send_vendor_cmd: pbuf = ", pbuf, len);

	rtk_bt_gap_vendor_cmd_req(&param);

	DBG_BT_COEX("bt_coex_send_vendor_cmd <----- \r\n");
}

static void bt_coex_set_profile_info_to_fw(void)
{
	uint8_t handle_number = 0;
	uint8_t report_number = 0;
	struct list_head *plist = NULL;
	struct rtk_bt_coex_conn_t *p_conn = NULL;
	uint8_t *pbuf = NULL;
	uint8_t offset = 0;

	if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while (plist != &p_rtk_bt_coex_priv->conn_list) {
			p_conn = (struct rtk_bt_coex_conn_t *)plist;
			if (p_conn->profile_bitmap != 0) {
				handle_number ++;
			}
			plist = plist->next;
			if (handle_number == 0xFF) {
				break;
			}
		}
	}

	if (handle_number == 0) {
		handle_number ++;    /* profile 0x00 should be reported to bt fw */
	}

	pbuf = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, (1 + handle_number * 6));
	if (!pbuf) {
		return;
	}

	pbuf[offset] = handle_number;
	offset ++;

	plist = NULL;
	if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while (plist != &p_rtk_bt_coex_priv->conn_list) {
			p_conn = (struct rtk_bt_coex_conn_t *)plist;
			DBG_BT_COEX("conn_handle 0x%x, profile_bitmap 0x%x, profile_status_bitmap 0x%x\r\n", p_conn->conn_handle, p_conn->profile_bitmap,
						p_conn->profile_status_bitmap);
			if (p_conn->profile_bitmap != 0) {
				report_number++;
				memcpy(pbuf + offset, &p_conn->conn_handle, 2);
				offset += 2;
				memcpy(pbuf + offset, &p_conn->profile_bitmap, 2);
				offset += 2;
				memcpy(pbuf + offset, &p_conn->profile_status_bitmap, 2);
				offset += 2;
			}
			plist = plist->next;
			if (report_number == handle_number) {
				break;
			}
		}
	}

	/* DBG_BT_COEX_DUMP("", pbuf, offset); */

	bt_coex_send_vendor_cmd(HCI_VENDOR_SET_PROFILE_REPORT_COMMAND, pbuf, offset);

	osif_mem_free(pbuf);
}

static void bt_coex_setup_check_timer(struct rtk_bt_coex_conn_t *p_conn, uint16_t profile_idx)
{
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	struct rtk_bt_coex_monitor_node_t *p_monitor_node = NULL;

	if (profile_idx != PROFILE_A2DP && profile_idx != PROFILE_PAN) {
		return;
	}

	p_monitor_node = (struct rtk_bt_coex_monitor_node_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_monitor_node_t));
	if (!p_monitor_node) {
		return;
	}


	p_monitor_node->b_first_add = 1;
	p_monitor_node->p_conn = p_conn;
	p_monitor_node->profile_idx = profile_idx;

	p_conn->a2dp_cnt = 0;
	p_conn->a2dp_pre_cnt = 0;
	p_conn->pan_cnt = 0;
	p_conn->pan_pre_cnt = 0;

	osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
	list_add_tail(&p_monitor_node->list, &p_rtk_bt_coex_priv->monitor_list);
	osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
}

static void bt_coex_del_check_timer(struct rtk_bt_coex_conn_t *p_conn, uint16_t profile_idx)
{
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	struct rtk_bt_coex_monitor_node_t *p_monitor_node = NULL;
	struct list_head *plist = NULL;

	if (profile_idx != PROFILE_A2DP && profile_idx != PROFILE_PAN) {
		return;
	}

	osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
	if (!list_empty(&p_rtk_bt_coex_priv->monitor_list)) {
		plist = p_rtk_bt_coex_priv->monitor_list.next;
		while (plist != &p_rtk_bt_coex_priv->monitor_list) {
			p_monitor_node = (struct rtk_bt_coex_monitor_node_t *)plist;
			if ((p_monitor_node->p_conn == p_conn) && (p_monitor_node->profile_idx == profile_idx)) {
				break;
			} else {
				plist = plist->next;
			}
		}
	}
	osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);

	if (!p_monitor_node) {
		return;
	}

	osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
	list_del(&p_monitor_node->list);
	osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);

	osif_mem_free(p_monitor_node);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
}

static void bt_coex_update_profile_info(struct rtk_bt_coex_conn_t *p_conn, uint8_t profile_index, bool b_is_add)
{
	bool b_need_update = false;

	if (b_is_add == true) {
		if (p_conn->profile_refcount[profile_index] == 0) {
			b_need_update = true;
			p_conn->profile_bitmap |= BIT(profile_index);
			/* LEAUDIO and SCO is always busy */
			if (profile_index == PROFILE_SCO || profile_index == PROFILE_LE_AUDIO) {
				p_conn->profile_status_bitmap |= BIT(profile_index);
			}
			p_conn->profile_refcount[profile_index]++;
			bt_coex_setup_check_timer(p_conn, profile_index);
		}
	} else {
		if (p_conn->profile_refcount[profile_index] != 0) {
			p_conn->profile_refcount[profile_index]--;
			if (p_conn->profile_refcount[profile_index] == 0) {
				b_need_update = true;
				p_conn->profile_bitmap &= ~(BIT(profile_index));
				p_conn->profile_status_bitmap &= ~(BIT(profile_index));
				if ((profile_index == PROFILE_HID) && (p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL))) {
					p_conn->profile_bitmap &= ~(BIT(PROFILE_HID_INTERVAL));
				}

				bt_coex_del_check_timer(p_conn, profile_index);
			}
		}
	}

	if (b_need_update) {
		bt_coex_set_profile_info_to_fw();
	}
}

static void bt_coex_handle_connection_complet_evt(uint8_t *p_evt_data)
{
	uint8_t conn_status = p_evt_data[0];
	uint16_t conn_handle = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;
	uint8_t link_type = p_evt_data[9];

	if (conn_status) {
		return;
	}

	conn_handle = (uint16_t)((p_evt_data[2] << 8) | p_evt_data[1]);
	DBG_BT_COEX("bt_coex_handle_connection_complet_evt: conn_handle = 0x%02x \r\n", conn_handle);

	p_conn = bt_coex_find_link_by_handle(conn_handle);

	if (!p_conn) {
		DBG_BT_COEX("bt_coex_handle_connection_complet_evt: alloc new connection \r\n");
		p_conn = (struct rtk_bt_coex_conn_t *) osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_conn_t));
		if (!p_conn) {
			return;
		}
		memset(p_conn, 0, sizeof(struct rtk_bt_coex_conn_t));
		p_conn->conn_handle = conn_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount, 0, sizeof(p_conn->profile_refcount));
	if ((0 == link_type) || (2 == link_type)) {
		p_conn->type = HCI_CONN_TYPE_SCO_ESCO;
		bt_coex_update_profile_info(p_conn, PROFILE_SCO, true);
	} else {
		p_conn->type = HCI_CONN_TYPE_L2CAP;
	}
}

static void bt_coex_handle_disconnection_complete_evt(uint8_t *pdata)
{
	uint16_t conn_handle = 0;
	uint8_t status = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;
	struct rtk_bt_coex_profile_info_t *p_profile = NULL;
	struct list_head *plist = NULL;

	status = pdata[0];
	if (status != 0) {
		DBG_BT_COEX("bt_coex_handle_disconnection_complete_evt return, status 0x%x\r\n", status);
		return;
	}

	conn_handle = (uint16_t)((pdata[2] << 8) | pdata[1]);
	DBG_BT_COEX("bt_coex_handle_disconnection_complete_evt: conn_handle = 0x%x \r\n", conn_handle);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if (!p_conn) {
		return;
	}
	switch (p_conn->type) {
	case HCI_CONN_TYPE_L2CAP:
		if (!list_empty(&p_conn->profile_list)) {
			plist = p_conn->profile_list.next;
			while (plist != &p_conn->profile_list) {
				p_profile = (struct rtk_bt_coex_profile_info_t *)plist;
				bt_coex_update_profile_info(p_conn, p_profile->idx, false);
				plist = plist->next;
				list_del(&p_profile->list);
				osif_mem_free(p_profile);
			}
		}
		break;
	case HCI_CONN_TYPE_SCO_ESCO:
		bt_coex_update_profile_info(p_conn, PROFILE_SCO, false);
		break;
	case HCI_CONN_TYPE_LE:
		bt_coex_update_profile_info(p_conn, PROFILE_HID, false);
		break;
	case HCI_CONN_TYPE_CIS:
		bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, false);
		break;
	default:
		break;
	}

	list_del(&p_conn->list);
	osif_mem_free(p_conn);

	DBG_BT_COEX("exit bt_coex_handle_disconnection_complete_evt \r\n");
}

static void bt_coex_le_connect_complete_evt(uint8_t enhance, uint8_t *pdata)
{
	uint16_t conn_handle = 0;
	uint16_t interval = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	if (pdata[0] != 0) { /* status */
		return;
	}

	conn_handle = (uint16_t)((pdata[2] << 8) | pdata[1]);

	if (enhance) {
		interval = (uint16_t)((pdata[24] << 8) | pdata[23]);
	} else {
		interval = (uint16_t)((pdata[12] << 8) | pdata[11]);
	}

#ifdef CONFIG_PLATFORM_TIZENRT_OS
	BT_LOGA("[BT_COEX] bt_coex_le_connect_complete_evt: conn_handle = %d, interval = 0x%x \r\n", conn_handle, interval);
#else
	DBG_BT_COEX("bt_coex_le_connect_complete_evt: conn_handle = %d, interval = 0x%x \r\n", conn_handle, interval);
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

	p_conn = bt_coex_find_link_by_handle(conn_handle);

	if (!p_conn) {
		p_conn = (struct rtk_bt_coex_conn_t *) osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_conn_t));
		if (!p_conn) {
			return;
		}
		memset(p_conn, 0, sizeof(struct rtk_bt_coex_conn_t));
		p_conn->conn_handle = conn_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount, 0, PROFILE_MAX);
	p_conn->type = HCI_CONN_TYPE_LE;

	p_conn->connect_interval = interval;

	bt_coex_update_profile_info(p_conn, PROFILE_HID, true);
	if (interval < BT_LE_BUSY_CONN_INTERVAL) {
		p_conn->profile_status_bitmap |= BIT(PROFILE_HID);
		bt_coex_update_profile_info(p_conn, PROFILE_HID_INTERVAL, true);
	}
}

static void bt_coex_le_update_connection_evt(uint8_t *pdata)
{
	uint16_t interval = 0;
	uint16_t conn_handle = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	conn_handle = (uint16_t)((pdata[2] << 8) | pdata[1]);
	interval = (uint16_t)((pdata[4] << 8) | pdata[3]);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if (!p_conn) {
		return;
	}

	if ((p_conn->profile_bitmap & BIT(PROFILE_HID)) == 0) {
		return;
	}

	p_conn->connect_interval = interval;

	if (interval < BT_LE_BUSY_CONN_INTERVAL) {
		if ((p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL)) == 0) {
			p_conn->profile_status_bitmap |= BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn, PROFILE_HID_INTERVAL, true);
		}
	} else {
		if ((p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL))) {
			p_conn->profile_status_bitmap &= ~BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn, PROFILE_HID_INTERVAL, false);
		}
	}
}

static void rtk_handle_le_cis_established_evt(uint8_t *pdata)
{
	uint16_t conn_handle = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	if (pdata[0] != 0) { /* status */
		return;
	}

	conn_handle = (uint16_t)((pdata[2] << 8) | pdata[1]);

	DBG_BT_COEX("%s: conn_handle = 0x%x\r\n", __func__, conn_handle);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if (!p_conn) {
		p_conn = (struct rtk_bt_coex_conn_t *) osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_conn_t));
		if (!p_conn) {
			return;
		}
		memset(p_conn, 0, sizeof(struct rtk_bt_coex_conn_t));
		p_conn->conn_handle = conn_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount, 0, PROFILE_MAX);
	p_conn->type = HCI_CONN_TYPE_CIS;

	bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, true);
}

static void rtk_handle_le_big_complete_evt(uint8_t *pdata)
{
	uint8_t big_handle = 0, status = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	status = pdata[0];
	big_handle = pdata[1];

	if (status != 0) {/* status */
		DBG_BT_COEX("%s: return by status = 0x%x\r\n", __func__, status);
		return;
	}

	DBG_BT_COEX("%s: big_handle = 0x%x\r\n", __func__, big_handle);

	p_conn = bt_coex_find_link_by_handle((uint16_t)big_handle);
	if (!p_conn) {
		p_conn = (struct rtk_bt_coex_conn_t *) osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_conn_t));
		if (!p_conn) {
			return;
		}
		memset(p_conn, 0, sizeof(struct rtk_bt_coex_conn_t));
		p_conn->conn_handle = big_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount, 0, PROFILE_MAX);
	p_conn->type = HCI_CONN_TYPE_BIS;

	bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, true);
}

static void rtk_handle_le_terminate_big_complete_evt(uint8_t *pdata)
{
	uint8_t big_handle = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	big_handle = pdata[0];

	DBG_BT_COEX("%s: big_handle = 0x%x\r\n", __func__, big_handle);

	p_conn = bt_coex_find_link_by_handle((uint16_t)big_handle);
	if (p_conn) {
		DBG_BT_COEX("%s: profile_bitmap = 0x%x\r\n", __func__, p_conn->profile_bitmap);
		if (p_conn->profile_bitmap & BIT(PROFILE_LE_AUDIO)) {
			bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, false);
		}
		list_del(&p_conn->list);
		osif_mem_free(p_conn);
	}
}

static void rtk_handle_le_big_sync_establish_evt(uint8_t *pdata)
{
	uint8_t big_handle = 0, status = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	status = pdata[0];
	big_handle = pdata[1];

	if (status != 0) {/* status */
		DBG_BT_COEX("%s: return by status = 0x%x\r\n", __func__, status);
		return;
	}

	DBG_BT_COEX("%s: big_handle = 0x%x\r\n", __func__, big_handle);

	p_conn = bt_coex_find_link_by_handle((uint16_t)big_handle);
	if (!p_conn) {
		p_conn = (struct rtk_bt_coex_conn_t *) osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_conn_t));
		if (!p_conn) {
			return;
		}
		memset(p_conn, 0, sizeof(struct rtk_bt_coex_conn_t));
		p_conn->conn_handle = big_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount, 0, PROFILE_MAX);
	p_conn->type = HCI_CONN_TYPE_BIS;

	bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, true);
}

static void rtk_handle_le_big_sync_lost_evt(uint8_t *pdata)
{
	uint8_t big_handle = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	big_handle = pdata[0];

	DBG_BT_COEX("%s: big_handle = 0x%x\r\n", __func__, big_handle);

	p_conn = bt_coex_find_link_by_handle((uint16_t)big_handle);
	if (p_conn) {
		DBG_BT_COEX("%s: profile_bitmap = 0x%x\r\n", __func__, p_conn->profile_bitmap);
		if (p_conn->profile_bitmap & BIT(PROFILE_LE_AUDIO)) {
			bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, false);
		}
		list_del(&p_conn->list);
		osif_mem_free(p_conn);
	}
}

static void bt_coex_handle_le_meta_evt(uint8_t *pdata)
{
	uint8_t sub_evt = pdata[0];
	/* DBG_BT_COEX("bt_coex_handle_le_meta_evt: sub evt = 0x%x \r\n", sub_evt); */
	switch (sub_evt) {
	case HCI_EV_LE_CONN_COMPLETE:
		bt_coex_le_connect_complete_evt(0, pdata + 1);
		break;
	case HCI_EV_LE_ENHANCED_CONN_COMPLETE:
		bt_coex_le_connect_complete_evt(1, pdata + 1);
		break;
	case HCI_EV_LE_CONN_UPDATE_COMPLETE:
		bt_coex_le_update_connection_evt(pdata + 1);
		break;
	case HCI_EV_LE_CIS_EST:
		rtk_handle_le_cis_established_evt(pdata + 1);
		break;
	case HCI_EV_LE_CREATE_BIG_CPL:
		rtk_handle_le_big_complete_evt(pdata + 1);
		break;
	case HCI_EV_LE_TERM_BIG_CPL:
		rtk_handle_le_terminate_big_complete_evt(pdata + 1);
		break;
	case HCI_EV_LE_BIG_SYNC_EST:
		rtk_handle_le_big_sync_establish_evt(pdata + 1);
		break;
	case HCI_EV_LE_BIG_SYNC_LST:
		rtk_handle_le_big_sync_lost_evt(pdata + 1);
		break;
	default:
		break;
	}
}

static void bt_coex_handle_mode_change_evt(uint8_t *pdata)
{
	uint16_t interval = 0;
	uint16_t conn_handle = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	conn_handle = (uint16_t)((pdata[2] << 8) | pdata[1]);
	interval = (uint16_t)((pdata[5] << 8) | pdata[4]);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if (!p_conn) {
		return;
	}

	if ((p_conn->profile_bitmap & BIT(PROFILE_HID)) == 0) {
		return;
	}

	p_conn->connect_interval = interval;

	if (interval < BT_HID_BUSY_INTERVAL) {
		if ((p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL)) == 0) {
			p_conn->profile_status_bitmap |= BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn, PROFILE_HID_INTERVAL, true);
		}
	} else {
		if ((p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL))) {
			p_conn->profile_status_bitmap &= ~BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn, PROFILE_HID_INTERVAL, false);
		}
	}
}

static void bt_coex_process_evt(uint8_t *pdata)
{
	uint8_t evt = 0;

	if (!pdata) {
		return;
	}

	evt = pdata[0];

	switch (evt) {
	case HCI_EV_MODE_CHANGE:
		bt_coex_handle_mode_change_evt(pdata + 2);
		break;
	case HCI_EV_DISCONN_COMPLETE:
		bt_coex_handle_disconnection_complete_evt(pdata + 2);
		break;
	case HCI_EV_LE_META:
		bt_coex_handle_le_meta_evt(pdata + 2);
		break;
	case HCI_EV_CONN_COMPLETE:
	case HCI_EV_SYNC_CONN_COMPLETE:
		bt_coex_handle_connection_complet_evt(pdata + 2);
		break;
	}
}

static int psm_to_profile(u16 psm)
{
	switch (psm) {
	case PSM_AVCTP:
	case PSM_SDP:
		return 0xFF;    /* ignore */

	case PSM_HID:
	case PSM_HID_INT:
		return PROFILE_HID;

	case PSM_AVDTP:
		return PROFILE_A2DP;

	case PSM_PAN:
	case PSM_OPP:
	case PSM_FTP:
	case PSM_BIP:
	case PSM_RFCOMM:
		return PROFILE_PAN;

	default:
		return PROFILE_PAN;
	}
}

static struct rtk_bt_coex_profile_info_t *bt_coex_find_profile(struct rtk_bt_coex_conn_t *p_conn, uint16_t scid, uint16_t dcid, uint8_t dir)
{
	bool b_find = false;
	struct list_head *plist = NULL;
	struct rtk_bt_coex_profile_info_t *p_profile = NULL;

	if (!list_empty(&p_conn->profile_list)) {
		plist = p_conn->profile_list.next;
		while (plist != &p_conn->profile_list) {
			p_profile = (struct rtk_bt_coex_profile_info_t *)plist;

			if (dcid == 0) { /* for l2cap connect req */
				if ((dir == DIR_IN) && (scid == p_profile->dcid)) {
					DBG_BT_COEX("bt_coex_find_profile for rx l2cap connect req: dir %d, p_profile->dcid = 0x%x\r\n", dir, p_profile->dcid);
					b_find = true;
					break;
				}
				if ((dir == DIR_OUT) && (scid == p_profile->scid)) {
					b_find = true;
					DBG_BT_COEX("bt_coex_find_profile for tx l2cap connect req: dir %d, p_profile->scid = 0x%x\r\n", dir, p_profile->scid);
					break;
				}
			} else if (scid == 0) { /* for packet_counter_handle */
				if ((dir == DIR_IN) && (dcid == p_profile->scid)) {
					b_find = true;
					break;
				}
				if ((dir == DIR_OUT) && (dcid == p_profile->dcid)) {
					b_find = true;
					break;
				}
			} else { /* for l2cap connect rsp */
				if ((dir == DIR_IN) && (scid == p_profile->scid)) {
					b_find = true;
					DBG_BT_COEX("bt_coex_find_profile for rx l2cap connect rsp: dir %d, p_profile->scid = 0x%x\r\n", dir, p_profile->scid);
					break;
				}
				if ((dir == DIR_OUT) && (scid == p_profile->dcid)) {
					DBG_BT_COEX("bt_coex_find_profile for tx l2cap connect rsp: dir %d, p_profile->dcid = 0x%x\r\n", dir, p_profile->dcid);
					b_find = true;
					break;
				}
			}

			plist = plist->next;

		}
	}

	if (b_find) {
		return p_profile;
	} else {
		return NULL;
	}
}

static void bt_coex_handle_l2cap_conn_req(struct rtk_bt_coex_conn_t *p_conn, uint16_t psm, uint16_t scid, uint8_t dir)
{
	int idx = psm_to_profile(psm);
	struct rtk_bt_coex_profile_info_t *p_profile = NULL;
	struct rtk_bt_coex_profile_info_t *p_profile_info = NULL;
	struct list_head *plist = NULL;

	DBG_BT_COEX("bt_coex_handle_l2cap_conn_req : psm = 0x%x scid = 0x%x dir = %d\r\n", psm, scid, dir);

	if (idx == 0xFF) {
		return;
	}

	p_profile = bt_coex_find_profile(p_conn, scid, 0, dir);
	if (p_profile) {
		return;
	}

	p_profile = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_profile_info_t));
	if (!p_profile) {
		return;
	}

	memset(p_profile, 0, sizeof(struct rtk_bt_coex_profile_info_t));

	p_profile->psm = psm;
	p_profile->idx = idx;

	if (dir == DIR_OUT) {
		p_profile->scid = scid;
	} else if (dir == DIR_IN) {
		p_profile->dcid = scid;
	}

	if (psm == PSM_AVDTP) {
		bool b_find = false;
		if (!list_empty(&p_conn->profile_list)) {
			plist = p_conn->profile_list.next;
			while (plist != &p_conn->profile_list) {
				p_profile_info = (struct rtk_bt_coex_profile_info_t *)plist;
				if (p_profile_info->psm == psm) {
					b_find = true;
					break;
				} else {
					plist = plist->next;
				}
			}
		}

		if (!b_find) {
			p_profile->flags = A2DP_SIGNAL;
		} else {
			p_profile->flags = A2DP_MEDIA;
		}
	}

	DBG_BT_COEX("bt_coex_handle_l2cap_conn_req: list_add_tail scid = 0x%x, dcid = 0x%x, flags = %d\r\n", p_profile->scid, p_profile->dcid, p_profile->flags);

	list_add_tail(&p_profile->list, &p_conn->profile_list);
}

static void bt_coex_handle_l2cap_conn_rsp(struct rtk_bt_coex_conn_t *p_conn, uint16_t scid, uint16_t dcid, uint8_t res, uint8_t dir)
{
	struct rtk_bt_coex_profile_info_t *p_profile = NULL;

	DBG_BT_COEX("bt_coex_handle_l2cap_conn_rsp: dcid = 0x%x, scid = 0x%x, res = 0x%x, dir = %d\r\n", dcid, scid, res, dir);

	p_profile = bt_coex_find_profile(p_conn, scid, dcid, dir);

	if (!p_profile) {
		return;
	}

	if (!res) {
		if (dir == DIR_IN) {
			p_profile->dcid = dcid;
		} else if (dir == DIR_OUT) {
			p_profile->scid = dcid;
		}

		DBG_BT_COEX("bt_coex_handle_l2cap_conn_rsp: idx = 0x%x, scid = 0x%x, dcid = 0x%x\r\n", p_profile->idx, p_profile->scid, p_profile->dcid);

		bt_coex_update_profile_info(p_conn, p_profile->idx, true);
	}
}

static void bt_coex_handle_handle_l2cap_dis_conn_req(struct rtk_bt_coex_conn_t *p_conn, uint16_t scid, uint16_t dcid, uint8_t dir)
{
	bool b_find = false;
	struct list_head *plist = NULL;
	struct rtk_bt_coex_profile_info_t *p_profile = NULL;

	if (!list_empty(&p_conn->profile_list)) {
		plist = p_conn->profile_list.next;
		while (plist != &p_conn->profile_list) {
			p_profile = (struct rtk_bt_coex_profile_info_t *)plist;
			if (dir == DIR_IN) {
				if ((p_profile->dcid == scid) && (p_profile->scid == dcid)) {
					b_find = true;
					break;
				}
			}

			if (dir == DIR_OUT) {
				if ((p_profile->dcid == dcid) && (p_profile->scid == scid)) {
					b_find = true;
					break;
				}
			}

			plist = plist->next;

		}
	}

	if (b_find == false) {
		return;
	}


	DBG_BT_COEX("bt_coex_handle_handle_l2cap_dis_conn_req: disconnect profile \r\n");

	if (p_profile->idx == PROFILE_A2DP) {
		if (p_conn->profile_bitmap & BIT(PROFILE_SINK)) {
			DBG_BT_COEX("delete PROFILE_SINK profile \r\n");
			p_conn->profile_bitmap &= ~(BIT(PROFILE_SINK));
		}
		if (p_conn->profile_status_bitmap & BIT(PROFILE_SINK)) {
			DBG_BT_COEX("delete PROFILE_SINK status \r\n");
			p_conn->profile_status_bitmap &= ~(BIT(PROFILE_SINK));
		}
	}

	DBG_BT_COEX("bt_coex_handle_handle_l2cap_dis_conn_req: p_profile->idx = 0x%x \r\n", p_profile->idx);
	bt_coex_update_profile_info(p_conn, p_profile->idx, false);

	list_del(&p_profile->list);
	osif_mem_free(p_profile);
}


static void bt_coex_set_bitpool_to_fw(uint8_t *user_data, uint16_t length)
{
	struct sbc_frame_hdr *sbc_header = NULL;
	struct rtp_header rtph;
	uint8_t hci_buf[1] = {0};

	/* We assume it is SBC if the packet length
	    * is bigger than 100 bytes
	    */
	if (length > 100) {
		memcpy(&rtph, user_data, sizeof(struct rtp_header));

		DBG_BT_COEX("bt_coex_set_bitpool_to_fw rtp: v 0x%x, cc 0x%x, pt 0x%x \r\n", rtph.v, rtph.cc, rtph.pt);

		/* move forward */
		user_data += sizeof(struct rtp_header) + rtph.cc * 4 + 1;

		/* point to the sbc frame header */
		sbc_header = (struct sbc_frame_hdr *)user_data;
		hci_buf[0] = (uint8_t)sbc_header->bitpool;

		DBG_BT_COEX("bt_coex_set_bitpool_to_fw bitpool %d channel_mode %d sampling_frequency %d\r\n", sbc_header->bitpool, sbc_header->channel_mode,
					sbc_header->sampling_frequency);

		/* the first 4 byte reserved for header */
		bt_coex_send_vendor_cmd(HCI_VENDOR_SET_BITPOOL, hci_buf, 1);
	}
}

static void bt_coex_packet_counter_handle(struct rtk_bt_coex_conn_t *p_conn, uint16_t cid, uint8_t dir, uint8_t *pdata, uint16_t length)
{
	struct rtk_bt_coex_profile_info_t *p_profile = NULL;

	p_profile = bt_coex_find_profile(p_conn, 0, cid, dir);

	if (!p_profile) {
		return;
	}

	if ((p_profile->idx == PROFILE_A2DP) && (p_profile->flags == A2DP_MEDIA)) {
		if (!(p_conn->profile_status_bitmap & BIT(PROFILE_A2DP))) {
			p_conn->profile_status_bitmap |= BIT(PROFILE_A2DP);
			if (dir == DIR_IN) {
				if (!(p_conn->profile_bitmap & (BIT(PROFILE_SINK)))) {
					p_conn->profile_bitmap |= BIT(PROFILE_SINK);
				}
				if (!(p_conn->profile_status_bitmap & (BIT(PROFILE_SINK)))) {
					p_conn->profile_status_bitmap |= BIT(PROFILE_SINK);
				}
			}
			bt_coex_set_profile_info_to_fw();
			bt_coex_set_bitpool_to_fw(pdata, length);
		}

		p_conn->a2dp_cnt ++;
	}

	if (p_profile->idx == PROFILE_PAN) {
		p_conn->pan_cnt ++;
	}
}

void bt_coex_dump_frame(uint8_t *pdata, uint32_t len)
{
	(void)pdata;
	(void)len;
	DBG_BT_COEX("dump frame: len = 0x%x \r\n", len);
	DBG_BT_COEX_DUMP("", pdata, len);
}

static void bt_coex_process_acl_data(uint8_t *pdata, uint16_t len, uint8_t dir)
{
	uint16_t conn_handle = 0;
	uint16_t channel_id = 0;
	uint16_t flags = 0;
	uint8_t code = 0;
	uint16_t psm = 0;
	uint16_t scid = 0;
	uint16_t dcid = 0;
	uint16_t res = 0;
	uint16_t pdu_len = 0;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	conn_handle = (uint16_t)((pdata[1] << 8) | pdata[0]);

	flags = (conn_handle >> 12);

	conn_handle = conn_handle & 0xFFF;

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if (!p_conn) {
		return;
	}

	if (flags == 0x01) {
		return;
	}

	pdu_len = (uint16_t)((pdata[5] << 8) | pdata[4]);
	channel_id = (uint16_t)((pdata[7] << 8) | pdata[6]);

	if ((channel_id == 0x0001) && len < 11) {
		return;
	}

	code = pdata[8];
	if (channel_id == 0x0001) {
		switch (code) {
		case L2CAP_CONN_REQ:
			psm = (uint16_t)((pdata[13] << 8) | pdata[12]);
			scid = (uint16_t)((pdata[15] << 8) | pdata[14]);
			DBG_BT_COEX("bt_coex_process_acl_data: L2CAP_CONN_REQ: psm = 0x%x, scid = 0x%x \r\n", psm, scid);
			bt_coex_handle_l2cap_conn_req(p_conn, psm, scid, dir);
			break;
		case L2CAP_CONN_RSP:
			dcid = (uint16_t)((pdata[13] << 8) | pdata[12]);
			scid = (uint16_t)((pdata[15] << 8) | pdata[14]);
			res = (uint16_t)((pdata[17] << 8) | pdata[16]);
			DBG_BT_COEX("bt_coex_process_acl_data: L2CAP_CONN_RSP \r\n");
			bt_coex_handle_l2cap_conn_rsp(p_conn, scid, dcid, res, dir);
			break;
		case L2CAP_DISCONN_REQ:
			dcid = (uint16_t)((pdata[13] << 8) | pdata[12]);
			scid = (uint16_t)((pdata[15] << 8) | pdata[14]);
			bt_coex_handle_handle_l2cap_dis_conn_req(p_conn, scid, dcid, dir);
			break;
		case L2CAP_DISCONN_RSP:
			break;
		default:
			break;
		}
	} else {
		if ((((p_conn->profile_bitmap & BIT(PROFILE_A2DP)) > 0) || ((p_conn->profile_bitmap & BIT(PROFILE_PAN)) > 0))) {
			bt_coex_packet_counter_handle(p_conn, channel_id, dir, pdata + 9, pdu_len);
		}
	}
}

static void bt_coex_monitor_timer_handler(void *arg)
{
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	UNUSED(arg);
	struct rtk_bt_coex_monitor_node_t *p_monitor = NULL;
	struct rtk_bt_coex_conn_t *p_conn = NULL;
	struct list_head *plist = NULL;

	if (list_empty(&p_rtk_bt_coex_priv->monitor_list)) {
		return;
	}

	plist = p_rtk_bt_coex_priv->monitor_list.next;
	while (plist != &p_rtk_bt_coex_priv->monitor_list) {
		p_monitor = (struct rtk_bt_coex_monitor_node_t *)plist;
		p_conn = p_monitor->p_conn;

		if (p_monitor->b_first_add) {
			p_monitor->b_first_add = 0;
		} else {

			if (p_monitor->profile_idx == PROFILE_A2DP) {

				DBG_BT_COEX("bt_coex_monitor_timer_handler: a2dp cnt = %d \r\n", p_conn->a2dp_cnt);

				if ((p_conn->a2dp_cnt == 0) && (p_conn->a2dp_pre_cnt > 0)) {
					p_conn->profile_status_bitmap &= (~BIT(PROFILE_A2DP));
					if (p_conn->profile_status_bitmap & BIT(PROFILE_SINK)) {
						p_conn->profile_status_bitmap &= ~(BIT(PROFILE_SINK));
					}
					bt_coex_set_profile_info_to_fw();
				}

				p_conn->a2dp_pre_cnt = p_conn->a2dp_cnt;
				p_conn->a2dp_cnt = 0;
			}

			if (p_monitor->profile_idx == PROFILE_PAN) {

				if ((p_conn->pan_cnt == 0) && (p_conn->pan_pre_cnt > 0)) {
					p_conn->profile_status_bitmap &= (~BIT(PROFILE_PAN));
					bt_coex_set_profile_info_to_fw();
				}
				p_conn->pan_pre_cnt = p_conn->pan_cnt;
				p_conn->pan_cnt = 0;
			}
		}

		plist = plist->next;
	}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
}

#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
static void bt_coex_send_to_coex_driver(void);
static void bt_coex_setup_link_timer_handler(void *arg)
{
	(void)arg;

	if (!bt_coex_initialized) {
		return;
	}

	p_rtk_bt_coex_priv->bt_info_cur.le_setup_link = 0;
	DBG_BT_COEX("setup_link_timer done\r\n");
	bt_coex_send_to_coex_driver();
}

static uint16_t bt_coex_count_setup_link_timeout(uint16_t conn_intvl)
{
	//CONFIG_COEX_BT_CONNING_TIMER_THR 1000 // unit:1ms
	//transmitWindowDelay_max: 3.75ms, 3*1.25ms
	//transmitWindowoffset_max: conn_interval
	//transmitWindowSize_max: 10ms, 8*1.25ms
	//connecting_timer min: transmitWindowDelay_max+transmitWindowoffset_max+transmitWindowSize_max+(connInterval)*6 = 3+conn_interval+8+conn_interval*6
	//connecting_timer max: CONFIG_COEX_BT_CONNING_TIMER_THR
	// conn_intvl ----- timeout
	//   6(7.5ms) ----- 66.25ms
	//  24(30ms) ------ 223.75ms
	// 120(150ms) ----- 1063.75ms
	uint16_t timeout = (((conn_intvl * 7) + 11) * 5) >> 2; // unit: ms

	if (conn_intvl == 0) {
		return 0;
	}

	if (timeout > 1000) {
		timeout = 1000;
	}

	return timeout;

}

static uint16_t bt_coex_get_max_connect_intvl(void)
{
	uint16_t connect_interval = 0;
	struct list_head *plist = NULL;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while (plist != &p_rtk_bt_coex_priv->conn_list) {
			p_conn = (struct rtk_bt_coex_conn_t *)plist;
			if (p_conn->connect_interval > connect_interval) {
				connect_interval = p_conn->connect_interval;
			}
			plist = plist->next;
		}
	}

	return connect_interval;
}

static uint8_t bt_coex_count_link(void)
{
	uint8_t link_cnt = 0;
	struct list_head *plist = NULL;

	if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while (plist != &p_rtk_bt_coex_priv->conn_list) {
			link_cnt++;
			plist = plist->next;
		}
	}

	return link_cnt;
}

static uint8_t bt_coex_link_status(void)
{
	enum LE_CONNECT_STATUS_e link_status = LE_CONNECT_NONE;
	struct list_head *plist = NULL;
	struct rtk_bt_coex_conn_t *p_conn = NULL;

	if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while (plist != &p_rtk_bt_coex_priv->conn_list) {
			p_conn = (struct rtk_bt_coex_conn_t *)plist;
			if (p_conn->connect_interval < BT_LE_BUSY_CONN_INTERVAL) {
				link_status = LE_CONNECT_BUSY;
				break;
			} else {
				link_status = LE_CONNECT_IDLE;
			}
			plist = plist->next;
		}
	}

	return link_status;
}

static void bt_coex_send_b2w_sw_mailbox(uint8_t *user_data, uint16_t length)
{
#if defined(CONFIG_BT_COEXIST)
	DBG_BT_COEX_DUMP("bt_coex_send_b2w_sw_mailbox: pdata = ", user_data, length);
	rtk_coex_btc_bt_hci_notify(user_data, length, COEX_H2C_BT_HCI_NOTIFY_SW_MAILBOX);
#else
	(void) user_data;
	(void) length;
#endif
}

static void bt_coex_send_construct_mp_report(uint8_t mailbox_id, uint8_t mp_op_code, uint8_t scan_type)
{
	/* B0: mailbox_id; B1: seq; B2: opcode; ...*/
	union rtk_coex_b2w_sw_mailbox_bt_mp_report mailbox_para = {0};
	if (RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_TYPE == mp_op_code) {
		/* B3: [b7:b4],len;[b3:b0],status; B4: data1 */
		osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.id = mailbox_id;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.subid = mp_op_code;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.len = 1;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.status = 0;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.le_bg_scan = 0;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.le_init_scan = (p_rtk_bt_coex_priv->bt_info_cur.le_scan_type == LE_SCAN_INIT);
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_type.le_scan = (p_rtk_bt_coex_priv->bt_info_cur.le_scan_type == LE_SCAN_NORMAL);
		osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);

		bt_coex_send_b2w_sw_mailbox((uint8_t *)&mailbox_para, sizeof(union rtk_coex_b2w_sw_mailbox_bt_mp_report));
	} else if (RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_PARA == mp_op_code) {
		/* B3: [b7:b4],len;[b3:b0],status; B4: data0; B5: data1; B6: data2; B7: data3 */
		osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.id = mailbox_id;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.subid = mp_op_code;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.len = 4;
		mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.status = 0;
		if (scan_type == LE_SCAN_INIT) {
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_window_low = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_window & 0xFF;
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_window_high = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_window >> 8;
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_intvl_low = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_intvl & 0xFF;
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_intvl_high = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_intvl >> 8;
		} else if (scan_type == LE_SCAN_NORMAL) {
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_window_low = p_rtk_bt_coex_priv->bt_info_cur.le_scan_window & 0xFF;
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_window_high = p_rtk_bt_coex_priv->bt_info_cur.le_scan_window >> 8;
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_intvl_low = p_rtk_bt_coex_priv->bt_info_cur.le_scan_intvl & 0xFF;
			mailbox_para.rtk_coex_b2w_sw_mailbox_le_scan_para.scan_intvl_high = p_rtk_bt_coex_priv->bt_info_cur.le_scan_intvl >> 8;
		}
		osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);

		bt_coex_send_b2w_sw_mailbox((uint8_t *)&mailbox_para, sizeof(union rtk_coex_b2w_sw_mailbox_bt_mp_report));
	}
}
static void bt_coex_send_construct_bt_info(uint8_t mailbox_id)
{
	/* B0: mailbox_id; B1: mailbox_len; B2: LB2; B3: LB3; B4: hb0; B5: hb1; B6: hb2; B7: hb3*/
	struct rtk_coex_b2w_sw_mailbox_bt_info_report_by_itself mailbox_para = {0};
	osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
	mailbox_para.id = mailbox_id;
	mailbox_para.len = 6;
	mailbox_para.hid = (p_rtk_bt_coex_priv->bt_info_cur.le_connect == LE_CONNECT_BUSY);
	mailbox_para.le_link = (p_rtk_bt_coex_priv->bt_info_cur.le_connect != LE_CONNECT_NONE);
	mailbox_para.multi_link = (p_rtk_bt_coex_priv->bt_info_cur.le_link_cnt > 1);
	mailbox_para.ble_scan_en = (p_rtk_bt_coex_priv->bt_info_cur.le_scan_type != LE_SCAN_NONE);
	osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);

	bt_coex_send_b2w_sw_mailbox((uint8_t *)&mailbox_para, sizeof(struct rtk_coex_b2w_sw_mailbox_bt_info_report_by_itself));
}
static void bt_coex_send_construct_extra_bt_info(uint8_t mailbox_id, uint8_t setup_link)
{
	/* B0: mailbox_id; B1: mailbox_len; B2: LB2; B3: LB3; B4: hb0; B5: hb1; B6: hb2; B7: hb3*/
	struct rtk_coex_b2w_sw_mailbox_bt_extra_info_report_by_itself mailbox_para = {0};
	mailbox_para.id = mailbox_id;
	mailbox_para.len = 6;
	mailbox_para.le_setup_link = setup_link;

	bt_coex_send_b2w_sw_mailbox((uint8_t *)&mailbox_para, sizeof(struct rtk_coex_b2w_sw_mailbox_bt_extra_info_report_by_itself));
}
static void bt_coex_send_construct_scan_start_end(uint8_t mailbox_id)
{
	/* B0: mailbox_id; B1: mailbox_len; B2: LB2; B3: LB3; B4: hb0; B5: hb1; B6: hb2*/
	struct rtk_coex_b2w_sw_mailbox_bt_le_init_scan_start_end mailbox_para = {0};
	osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
	mailbox_para.id = mailbox_id;
	mailbox_para.len = 6;
	mailbox_para.scan_window_low = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_window & 0xFF;
	mailbox_para.scan_window_high = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_window >> 8;
	mailbox_para.scan_intvl_low = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_intvl & 0xFF;
	mailbox_para.scan_intvl_high = p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_intvl >> 8;
	osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);

	bt_coex_send_b2w_sw_mailbox((uint8_t *)&mailbox_para, sizeof(struct rtk_coex_b2w_sw_mailbox_bt_le_init_scan_start_end));
}

static void bt_coex_send_to_coex_driver(void)
{
	if (!bt_coex_initialized) {
		return;
	}

	if (0 == memcmp(&p_rtk_bt_coex_priv->bt_info_prev, &p_rtk_bt_coex_priv->bt_info_cur, sizeof(struct rtk_coex_bt_info_t))) {
		return;
	}

	// mailbox_id = 0x30, subid = 0x2d
	if (p_rtk_bt_coex_priv->bt_info_cur.le_scan_type != p_rtk_bt_coex_priv->bt_info_prev.le_scan_type) {
		// mailbox_id = 0x49, send scan intvl/window
		if (p_rtk_bt_coex_priv->bt_info_cur.le_scan_type == LE_SCAN_INIT) {
			//DBG_BT_COEX("send mailbox_id=0x49\r\n");
			bt_coex_send_construct_scan_start_end(RTK_COEX_MAILBOX_BT_SCAN_START_END);
		}
		// mailbox_id = 0x30, subid = 0x2d, send scan type
		// mailbox_id = 0x30, subid = 0x2e, send scan intvl/window
		//DBG_BT_COEX("send mailbox_id=0x30,subid=0x2d\r\n");
		bt_coex_send_construct_mp_report(RTK_COEX_MAILBOX_BT_MP_REPORT, RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_TYPE, 0);
		if (p_rtk_bt_coex_priv->bt_info_cur.le_scan_type == LE_SCAN_NORMAL) {
			//DBG_BT_COEX("send mailbox_id=0x30,subid=0x2e\r\n");
			bt_coex_send_construct_mp_report(RTK_COEX_MAILBOX_BT_MP_REPORT, RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_PARA, LE_SCAN_NORMAL);
		}
	}

	// mailbox_id = 0x47
	if ((p_rtk_bt_coex_priv->bt_info_cur.le_setup_link != p_rtk_bt_coex_priv->bt_info_prev.le_setup_link)) {
		//DBG_BT_COEX("send mailbox_id=0x47\r\n");
		bt_coex_send_construct_extra_bt_info(RTK_COEX_MAILBOX_BT_LE_EXTRA_INFO_BY_ITSELF, p_rtk_bt_coex_priv->bt_info_cur.le_setup_link);
	}

	// mailbox_id = 0x27
	if ((p_rtk_bt_coex_priv->bt_info_cur.le_scan_type != p_rtk_bt_coex_priv->bt_info_prev.le_scan_type)
		|| (p_rtk_bt_coex_priv->bt_info_cur.le_connect != p_rtk_bt_coex_priv->bt_info_prev.le_connect)
		|| (p_rtk_bt_coex_priv->bt_info_cur.le_link_cnt != p_rtk_bt_coex_priv->bt_info_prev.le_link_cnt)) {
		//DBG_BT_COEX("send mailbox_id=0x27\r\n");
		bt_coex_send_construct_bt_info(RTK_COEX_MAILBOX_BT_INFO_REPORT_BY_ITSELF);
	}

	// backup
	memcpy(&p_rtk_bt_coex_priv->bt_info_prev, &p_rtk_bt_coex_priv->bt_info_cur, sizeof(struct rtk_coex_bt_info_t));

	// debug print
	DBG_BT_COEX("bt_info:iscan_i/w/scan_i/w/scan_t/con/setup/link=%d/%d/%d/%d/%d/%d/%d/%d\r\n", p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_intvl,
				p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_window,
				p_rtk_bt_coex_priv->bt_info_cur.le_scan_intvl,
				p_rtk_bt_coex_priv->bt_info_cur.le_scan_window,
				p_rtk_bt_coex_priv->bt_info_cur.le_scan_type,
				p_rtk_bt_coex_priv->bt_info_cur.le_connect,
				p_rtk_bt_coex_priv->bt_info_cur.le_setup_link,
				p_rtk_bt_coex_priv->bt_info_cur.le_link_cnt);
}

void bt_coex_send_w2b_sw_mailbox(uint8_t *user_data, uint16_t length)
{
	// AMEBAD: use hci driver auto report, ignore coex driver trigger action
#if 0
	uint8_t mailbox_id = 0, subid = 0, scan_type = 0;

	if (length == 0 || user_data == NULL) {
		return;
	}

	if (!bt_coex_initialized) {
		return;
	}

	DBG_BT_COEX_DUMP("bt_coex_send_w2b_sw_mailbox: pdata = ", user_data, length);

	mailbox_id = user_data[0];
	subid = user_data[2];
	scan_type = user_data[3];
	switch (mailbox_id) {
	case RTK_COEX_MAILBOX_BT_IGNORE_WLAN_ACT:
		/* B0: mailbox_id; B1: mailbox_len; B2: enable; */
		break;
	case RTK_COEX_MAILBOX_WL_OP_MODE:
		/* B0: mailbox_id; B1: mailbox_len; B2: opcode; B3:channel_idx; B4:bw*/
		break;
	case RTK_COEX_MAILBOX_BT_INFO_REPORT:
		/* B0: mailbox_id; B1: mailbox_len; B2: trigger*/
		bt_coex_send_construct_bt_info(RTK_COEX_MAILBOX_BT_INFO_REPORT);
		break;
	case RTK_COEX_MAILBOX_BT_MP_REPORT:
		/* B0: mailbox_id; B1: mailbox_len; B2: opcode; ...*/
		if (RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_TYPE == subid) {
			bt_coex_send_construct_mp_report(RTK_COEX_MAILBOX_BT_MP_REPORT, RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_TYPE, 0);
		} else if (RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_PARA == subid) {
			// user_data[3]: 1: le bg scan; 2: init scan; 4: le scan
			if (scan_type == 2) {
				bt_coex_send_construct_mp_report(RTK_COEX_MAILBOX_BT_MP_REPORT, RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_PARA, LE_SCAN_INIT);
			}
			if (scan_type == 4) {
				bt_coex_send_construct_mp_report(RTK_COEX_MAILBOX_BT_MP_REPORT, RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_PARA, LE_SCAN_NORMAL);
			}
		}
		break;
	case RTK_COEX_MAILBOX_BT_LE_EXTRA_INFO:
		/* B0: mailbox_id; B1: mailbox_len; B2: trigger*/
		bt_coex_send_construct_extra_bt_info(RTK_COEX_MAILBOX_BT_LE_EXTRA_INFO, p_rtk_bt_coex_priv->bt_info_cur.le_setup_link);
		break;
	default:
		break;
#else
	(void) user_data;
	(void) length;
#endif
	}

	static void bt_coex_process_cmd(uint8_t *pdata, uint16_t len)
	{
		/* B0&B1:opcode; B2: len; B3~:data*/
		uint16_t hci_op = 0;
		uint8_t scan_en = 0;
		uint16_t scan_win = 0, scan_intvl = 0;

		(void)len;

		if (!bt_coex_initialized) {
			return;
		}

		hci_op = (pdata[1] << 8) + pdata[0];

		switch (hci_op) {
		case BT_HCI_OP_LE_SET_SCAN_PARAM:
			scan_intvl = (pdata[5] << 8) + pdata[4];
			scan_win = (pdata[7] << 8) + pdata[6];
			osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
			p_rtk_bt_coex_priv->bt_info_cur.le_scan_window = scan_win;
			p_rtk_bt_coex_priv->bt_info_cur.le_scan_intvl = scan_intvl;
			osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
			break;
		case BT_HCI_OP_LE_SET_SCAN_ENABLE:
			scan_en = pdata[3];
			osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
			if (scan_en == 1) {
				p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_NORMAL;
			} else if (scan_en == 0) {
				p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_NONE;
			}
			osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
			break;
		case BT_HCI_OP_LE_CREATE_CONN:
			scan_en = 1;
			scan_intvl = (pdata[4] << 8) + pdata[3];
			scan_win = (pdata[6] << 8) + pdata[5];
			osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
			p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_window = scan_win;
			p_rtk_bt_coex_priv->bt_info_cur.le_scan_init_intvl = scan_intvl;
			p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_INIT;
			p_rtk_bt_coex_priv->bt_info_cur.le_setup_link = 1;
			osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
			break;
		case BT_HCI_OP_LE_CREATE_CONN_CANCEL:
			osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
			p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_NONE;
			p_rtk_bt_coex_priv->bt_info_cur.le_setup_link = 0;
			osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
			break;
		default:
			break;
		}
	}
#endif

	void bt_coex_evt_notify(uint8_t *pdata, uint16_t len)
	{
		uint8_t evt = pdata[0];

		if (!bt_coex_initialized) {
			return;
		}

		switch (evt) {
#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
		case HCI_EV_LE_META: {
			/* B0: event code, B1: length, B2: subevent_code, ... */
			uint8_t sub_evt = pdata[2];
			uint8_t status = pdata[3];
			uint16_t connect_timeout = 0;
			switch (sub_evt) {
			// case init scan done event
			// p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_NONE;
			case HCI_EV_LE_CONN_COMPLETE:
			/* B3: status; B4&B5: connect_handle; ... B14&B15:connection_interval; ...*/
			case HCI_EV_LE_ENHANCED_CONN_COMPLETE:
				/* B3: status; B4&B5: connect_handle; ... B26&B27:connection_interval; ...*/
				osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
				p_rtk_bt_coex_priv->bt_info_cur.le_link_cnt = bt_coex_count_link();
				p_rtk_bt_coex_priv->bt_info_cur.le_connect = bt_coex_link_status();
				osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
				if (status == 0) {
					osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
					p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_NONE;
					p_rtk_bt_coex_priv->bt_info_cur.le_setup_link = 1;
					osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
					if (sub_evt != HCI_EV_LE_CONN_UPDATE_COMPLETE) {
						// delay to set setup_link = 0
						connect_timeout = bt_coex_count_setup_link_timeout(bt_coex_get_max_connect_intvl());
						DBG_BT_COEX("setup_link_timer start(conn_to=%d)\r\n", connect_timeout);
						if ((p_rtk_bt_coex_priv->setup_link_timer == NULL) && (connect_timeout > 0)) {
							if (true == osif_timer_create(&p_rtk_bt_coex_priv->setup_link_timer, "bt_coex_setup_link_timer", NULL, connect_timeout, false,
														  bt_coex_setup_link_timer_handler)) {
								osif_timer_start(&p_rtk_bt_coex_priv->setup_link_timer);
							} else {
								DBG_BT_COEX("bt_coex_evt_notify: setup_link_timer create fail!!!\r\n");
							}
						} else if (connect_timeout > 0) {
							osif_timer_restart(&p_rtk_bt_coex_priv->setup_link_timer, connect_timeout);
						}
					}
				} else {
					osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
					p_rtk_bt_coex_priv->bt_info_cur.le_scan_type = LE_SCAN_NONE;
					p_rtk_bt_coex_priv->bt_info_cur.le_setup_link = 0;
					osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
				}
				break;

			case HCI_EV_LE_CONN_UPDATE_COMPLETE:
				/* B3: status; B4&B5: connect_handle; B6&B7:connection_interval; ...*/
				p_rtk_bt_coex_priv->bt_info_cur.le_connect = bt_coex_link_status();
			default:
				break;
			}
			break;
		}
		case HCI_EV_DISCONN_COMPLETE: {
			/* B0: event code, B1: length, B2: status, B3&B4: connect_handle, B5: Reason */
			uint8_t status = pdata[2];
			if (status == 0) {
				osif_mutex_take(p_rtk_bt_coex_priv->info_paras_mutex, 0xFFFFFFFFUL);
				p_rtk_bt_coex_priv->bt_info_cur.le_link_cnt = bt_coex_count_link();
				p_rtk_bt_coex_priv->bt_info_cur.le_connect = bt_coex_link_status();
				osif_mutex_give(p_rtk_bt_coex_priv->info_paras_mutex);
			}
			break;
		}
#endif

		default:
			(void) len;
			break;
		}
	}

	void bt_coex_process_rx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
	{
		if (!pdata) {
			return;
		}

		if (type == HCI_EVT) {
			bt_coex_process_evt(pdata);
			bt_coex_evt_notify(pdata, len);
		}

		if (type == HCI_ACL) {
			bt_coex_process_acl_data(pdata, len, DIR_IN);
		}

#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
		// auto report
		bt_coex_send_to_coex_driver();
#endif
	}

	void bt_coex_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
	{
		if (!pdata) {
			return;
		}

		if (type == HCI_ACL) {
			bt_coex_process_acl_data(pdata, len, DIR_OUT);
		}

#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
		if (type == HCI_CMD) {
			bt_coex_process_cmd(pdata, len);
		}
#endif
	}

	void bt_coex_init(void)
	{
		DBG_BT_COEX("Init \r\n");
		p_rtk_bt_coex_priv = (struct rtk_bt_coex_priv_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct rtk_bt_coex_priv_t));
		if (!p_rtk_bt_coex_priv) {
			return;
		}
		memset(p_rtk_bt_coex_priv, 0, sizeof(struct rtk_bt_coex_priv_t));
		INIT_LIST_HEAD(&p_rtk_bt_coex_priv->conn_list);

#ifndef CONFIG_PLATFORM_TIZENRT_OS
		INIT_LIST_HEAD(&p_rtk_bt_coex_priv->monitor_list);
		if (false == osif_mutex_create(&p_rtk_bt_coex_priv->monitor_mutex)) {
			return;
		}

		if (true == osif_timer_create(&p_rtk_bt_coex_priv->monitor_timer, "bt_coex_monitor_timer", NULL, BT_COEX_MONITOR_INTERVAL, true,
									  bt_coex_monitor_timer_handler)) {
			osif_timer_start(&p_rtk_bt_coex_priv->monitor_timer);
		}
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
		if (false == osif_mutex_create(&p_rtk_bt_coex_priv->info_paras_mutex)) {
			DBG_BT_COEX("pinfo_paras_mutex create fail.\r\n");
		}
#endif

		bt_coex_initialized = true;
	}

	void bt_coex_deinit(void)
	{
		struct list_head *plist = NULL;
#ifndef CONFIG_PLATFORM_TIZENRT_OS
		struct rtk_bt_coex_monitor_node_t *p_monitor = NULL;
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
		struct rtk_bt_coex_conn_t *p_conn = NULL;

		DBG_BT_COEX("Deinit \r\n");
		bt_coex_initialized = false;

#ifndef CONFIG_PLATFORM_TIZENRT_OS
		osif_timer_stop(&p_rtk_bt_coex_priv->monitor_timer);

		osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
		if (!list_empty(&p_rtk_bt_coex_priv->monitor_list)) {
			plist = p_rtk_bt_coex_priv->monitor_list.next;
			while (plist != &p_rtk_bt_coex_priv->monitor_list) {
				p_monitor = (struct rtk_bt_coex_monitor_node_t *)plist;
				plist = plist->next;
				list_del(&p_monitor->list);
				osif_mem_free(p_monitor);
			}
		}
		osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

		plist = NULL;
		if (!list_empty(&p_rtk_bt_coex_priv->conn_list)) {
			plist = p_rtk_bt_coex_priv->conn_list.next;
			while (plist != &p_rtk_bt_coex_priv->conn_list) {
				p_conn = (struct rtk_bt_coex_conn_t *)plist;
				plist = plist->next;
				list_del(&p_conn->list);
				{
					struct list_head *p_profile_list = NULL;
					struct rtk_bt_coex_profile_info_t *p_profile = NULL;
					p_profile_list = p_conn->profile_list.next;
					while (p_profile_list != &p_conn->profile_list) {
						p_profile = (struct rtk_bt_coex_profile_info_t *)p_profile_list;
						p_profile_list = p_profile_list->next;
						list_del(&p_profile->list);
						osif_mem_free(p_profile);
					}
				}
				osif_mem_free(p_conn);
			}
		}

#ifndef CONFIG_PLATFORM_TIZENRT_OS
		osif_mutex_delete(p_rtk_bt_coex_priv->monitor_mutex);
		osif_timer_delete(&p_rtk_bt_coex_priv->monitor_timer);
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
		osif_mutex_delete(p_rtk_bt_coex_priv->info_paras_mutex);
		osif_timer_delete(&p_rtk_bt_coex_priv->setup_link_timer);
#endif
		osif_mem_free(p_rtk_bt_coex_priv);
	}

#else /* defined(HCI_BT_COEX_ENABLE) && HCI_BT_COEX_ENABLE */

void bt_coex_init(void)
{

}

void bt_coex_process_rx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{

}

void bt_coex_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{

}

void bt_coex_deinit(void)
{

}

#endif /* defined(HCI_BT_COEX_ENABLE) && HCI_BT_COEX_ENABLE */

