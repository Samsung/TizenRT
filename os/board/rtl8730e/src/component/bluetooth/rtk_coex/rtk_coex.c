/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include "hci_platform.h"
#include "rtk_coex.h"

#if defined(HCI_BT_COEX_ENABLE) && HCI_BT_COEX_ENABLE

#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX

void bt_coex_init(void)
{

}

void bt_coex_process_rx_frame(u8 type,u8 *data, u16 len)
{

}

void bt_coex_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{

}

void bt_coex_deinit(void)
{

}

#else //defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX

rtk_bt_coex_priv_t* p_rtk_bt_coex_priv = NULL;

extern bool hci_if_write_internal(uint8_t *buf, uint32_t len);

static rtk_bt_coex_conn_t*  bt_coex_find_link_by_handle(uint16_t conn_handle)
{
	bool b_find = false;
	struct list_head *plist = NULL;
	rtk_bt_coex_conn_t *p_conn = NULL;

	if(!list_empty(&p_rtk_bt_coex_priv->conn_list)){
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while(plist != &p_rtk_bt_coex_priv->conn_list){
			p_conn = (rtk_bt_coex_conn_t *)plist;
			if((p_conn->conn_handle & 0xFFF) == conn_handle){
				b_find = true;
				break;
			}else{
				plist = plist->next;
			}
		}
	}

	if(b_find)
		return p_conn;
	else
		return NULL;
}

/*static void bt_coex_dbg_dump(uint8_t *pbuf, uint32_t len)
{
	uint32_t i = 0;

	printf("bt_coex_dbg_dump: length = 0x%lx  \r\n",len);
	for(i=0;i<len;i++){
		printf("0x%x   ",pbuf[i]);
		if((i+1)%16 == 0)
			printf("\r\n");
	}

	printf("\r\n");
}*/

static void bt_coex_send_vendor_cmd(uint8_t *pbuf, uint8_t len)
{
	DBG_BT_COEX("bt_coex_send_vendor_cmd -----> \r\n");

	pbuf[0] = HCI_CMD;
	pbuf[1] = HCI_VENDOR_SET_PROFILE_REPORT_COMMAND & 0xFF;
	pbuf[2] = (HCI_VENDOR_SET_PROFILE_REPORT_COMMAND >> 8) & 0xFF;
	pbuf[3] = len - 4;

	DBG_BT_COEX("bt_coex_send_vendor_cmd: len = %d\r\n", len);
	DBG_BT_COEX("bt_coex_send_vendor_cmd: pbuf = ");
	for (uint8_t i = 0; i < len; i++) {
		DBG_BT_COEX("0x%02x ", *(pbuf + i));
	}
	DBG_BT_COEX("\r\n");

	hci_if_write_internal(pbuf, len);

	DBG_BT_COEX("bt_coex_send_vendor_cmd <----- \r\n");
}

static void bt_coex_set_profile_info_to_fw(void)
{
	uint8_t handle_number = 0;
	uint8_t report_number = 0;
	struct list_head *plist = NULL;
	rtk_bt_coex_conn_t *p_conn = NULL;
	uint8_t *pbuf = NULL;
	uint8_t offset = 0;

	if(!list_empty(&p_rtk_bt_coex_priv->conn_list)){
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while(plist != &p_rtk_bt_coex_priv->conn_list){
			p_conn = (rtk_bt_coex_conn_t *)plist;
			if(p_conn->profile_bitmap != 0)
				handle_number ++;
			plist = plist->next;
			if(handle_number == 0xFF)
				break;
		}
	}

	if(handle_number == 0)
		handle_number ++; // profile 0x00 should be reported to bt fw

	pbuf = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, (1 + handle_number * 6) + 4);
	if(!pbuf)
		return;

	pbuf[offset + 4] = handle_number;
	offset ++;

	plist = NULL;
	if(!list_empty(&p_rtk_bt_coex_priv->conn_list)){
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while(plist != &p_rtk_bt_coex_priv->conn_list){
			p_conn = (rtk_bt_coex_conn_t *)plist;
			DBG_BT_COEX("conn_handle 0x%x, profile_bitmap 0x%x, profile_status_bitmap 0x%x\r\n",p_conn->conn_handle,p_conn->profile_bitmap,p_conn->profile_status_bitmap);
			if(p_conn->profile_bitmap != 0){
				report_number++;
				memcpy(pbuf + offset + 4, &p_conn->conn_handle, 2);
				offset += 2;
				memcpy(pbuf + offset + 4, &p_conn->profile_bitmap, 2);
				offset += 2;
				memcpy(pbuf + offset + 4, &p_conn->profile_status_bitmap, 2);
				offset += 2;
			}
			plist = plist->next;
			if(report_number == handle_number)
				break;
		}
	}

	//bt_coex_dbg_dump(pbuf, offset + 4);

	bt_coex_send_vendor_cmd(pbuf, offset + 4);

	osif_mem_free(pbuf);
}

static void bt_coex_setup_check_timer(rtk_bt_coex_conn_t *p_conn, uint16_t profile_idx)
{
	rtk_bt_coex_monitor_node_t *p_monitor_node = NULL;

	if(profile_idx != PROFILE_A2DP && profile_idx != PROFILE_PAN)
		return;

	p_monitor_node = (rtk_bt_coex_monitor_node_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_monitor_node_t));
	if(!p_monitor_node)
		return;


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
}

static void bt_coex_del_check_timer(rtk_bt_coex_conn_t *p_conn, uint16_t profile_idx)
{
	rtk_bt_coex_monitor_node_t *p_monitor_node = NULL;
	struct list_head *plist = NULL;

	if(profile_idx != PROFILE_A2DP && profile_idx != PROFILE_PAN)
		return;

	osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
	if(!list_empty(&p_rtk_bt_coex_priv->monitor_list)){
		plist = p_rtk_bt_coex_priv->monitor_list.next;
		while(plist != &p_rtk_bt_coex_priv->monitor_list){
			p_monitor_node = (rtk_bt_coex_monitor_node_t *)plist;
			if((p_monitor_node->p_conn == p_conn) && (p_monitor_node->profile_idx == profile_idx)){
				break;
			}else{
				plist = plist->next;
			}
		}
	}
	osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);

	if(!p_monitor_node)
		return;

	osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
	list_del(&p_monitor_node->list);
	osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);

	osif_mem_free(p_monitor_node);
}

static void bt_coex_update_profile_info(rtk_bt_coex_conn_t *p_conn, uint8_t profile_index, bool b_is_add)
{
	bool b_need_update = false;

	if(b_is_add == true){
		if(p_conn->profile_refcount[profile_index] == 0){
			b_need_update = true;
			p_conn->profile_bitmap |= BIT(profile_index);
			/* LEAUDIO and SCO is always busy */
			if (profile_index == PROFILE_SCO || profile_index == PROFILE_LE_AUDIO) {
				p_conn->profile_status_bitmap |= BIT(profile_index);
			}
			p_conn->profile_refcount[profile_index]++;
			bt_coex_setup_check_timer(p_conn,profile_index);
		}
	}else{
		if(p_conn->profile_refcount[profile_index] != 0){
			p_conn->profile_refcount[profile_index]--;
			if(p_conn->profile_refcount[profile_index] == 0){
				b_need_update = true;
				p_conn->profile_bitmap &= ~(BIT(profile_index));
				p_conn->profile_status_bitmap &= ~(BIT(profile_index));
				if((profile_index == PROFILE_HID) && (p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL))){
					p_conn->profile_bitmap &= ~(BIT(PROFILE_HID_INTERVAL));
				}

				bt_coex_del_check_timer(p_conn,profile_index);
			}
		}
	}

	if(b_need_update){
		bt_coex_set_profile_info_to_fw();
	}
}

static void bt_coex_handle_connection_complet_evt(uint8_t *p_evt_data)
{
	uint8_t conn_status = p_evt_data[0];
	uint16_t conn_handle = 0;
	rtk_bt_coex_conn_t* p_conn = NULL;
	uint8_t link_type = p_evt_data[9];

	if(conn_status){
		return;
	}

	conn_handle = (uint16_t)((p_evt_data[2] << 8) | p_evt_data[1]);
	DBG_BT_COEX("bt_coex_handle_connection_complet_evt: conn_handle = 0x%02X \r\n",conn_handle);

	p_conn = bt_coex_find_link_by_handle(conn_handle);

	if(!p_conn){
		DBG_BT_COEX("bt_coex_handle_connection_complet_evt: alloc new connection \r\n");
		p_conn = (rtk_bt_coex_conn_t*) osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_conn_t));
		if(!p_conn){
			return;
		}
		memset(p_conn,0,sizeof(rtk_bt_coex_conn_t));
		p_conn->conn_handle = conn_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount,0,sizeof(p_conn->profile_refcount));
	if((0 == link_type) || (2 == link_type)){
		bt_coex_update_profile_info(p_conn, PROFILE_SCO, true);
	}
}

static void bt_coex_handle_disconnection_complete_evt(uint8_t *pdata)
{
	uint16_t conn_handle = 0;
	uint8_t status = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;
	rtk_bt_coex_profile_info_t *p_profile = NULL;
	struct list_head *plist = NULL;

	status = pdata[0];
	if(status != 0)
		return;

	conn_handle = (uint16_t)((pdata[2]<<8) | pdata[1]);
	printf("[BT_COEX] bt_coex_handle_disconnection_complete_evt: conn_handle = %d \r\n", conn_handle);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if(!p_conn)
		return;

	if(!list_empty(&p_conn->profile_list)){
		plist = p_conn->profile_list.next;
		while(plist != &p_conn->profile_list){
			p_profile = (rtk_bt_coex_profile_info_t *)plist;
			bt_coex_update_profile_info(p_conn,p_profile->idx,false);
			plist = plist->next;
			list_del(&p_profile->list);
			osif_mem_free(p_profile);
		}
	}
	list_del(&p_conn->list);
	osif_mem_free(p_conn);

	printf("[BT_COEX] exit bt_coex_handle_disconnection_complete_evt \r\n");
}

static void bt_coex_le_connect_complete_evt(uint8_t enhance, uint8_t *pdata)
{
	uint16_t conn_handle = 0;
	uint16_t interval = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	if (pdata[0] != 0) /* status */
		return;

	conn_handle = (uint16_t)((pdata[2]<<8) | pdata[1]);

	if (enhance)
		interval = (uint16_t)((pdata[23]<<8) | pdata[22]);
	else
		interval = (uint16_t)((pdata[11]<<8) | pdata[10]);

	printf("[BT_COEX] bt_coex_le_connect_complete_evt: conn_handle = %d, interval = 0x%x \r\n", conn_handle, interval);

	p_conn = bt_coex_find_link_by_handle(conn_handle);

	if (!p_conn) {
		p_conn = (rtk_bt_coex_conn_t*) osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_conn_t));
		if(!p_conn){
			return;
		}
		memset(p_conn, 0, sizeof(rtk_bt_coex_conn_t));
		p_conn->conn_handle = conn_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount,0,PROFILE_MAX);

	bt_coex_update_profile_info(p_conn,PROFILE_HID,true);
	if (interval < 60) {
		p_conn->profile_status_bitmap |= BIT(PROFILE_HID);
		bt_coex_update_profile_info(p_conn,PROFILE_HID_INTERVAL,true);
	}
}

static void bt_coex_le_update_connection_evt(uint8_t *pdata)
{
	uint16_t interval = 0;
	uint16_t conn_handle = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	conn_handle = (uint16_t)((pdata[2]<<8) | pdata[1]);
	interval = (uint16_t)((pdata[4]<<8) | pdata[3]);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if(!p_conn)
		return;

	if((p_conn->profile_bitmap & BIT(PROFILE_HID)) == 0)
		return;

	if(interval < 60){
		if((p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL)) == 0){
			p_conn->profile_status_bitmap |= BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn,PROFILE_HID_INTERVAL,true);
		}
	}else{
		if((p_conn->profile_bitmap| BIT(PROFILE_HID_INTERVAL))){
			p_conn->profile_status_bitmap &= ~BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn,PROFILE_HID_INTERVAL,false);
		}
	}
}

static void rtk_handle_le_cis_established_evt(uint8_t* pdata)
{
    uint16_t conn_handle = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	if (pdata[0] != 0) /* status */
		return;

	conn_handle = (uint16_t)((pdata[2]<<8) | pdata[1]);

	printf("[BT_COEX] %s: conn_handle = 0x%x\r\n", __func__,conn_handle);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if (!p_conn) {
		p_conn = (rtk_bt_coex_conn_t*) osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_conn_t));
		if(!p_conn){
			return;
		}
		memset(p_conn, 0, sizeof(rtk_bt_coex_conn_t));
		p_conn->conn_handle = conn_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount,0,PROFILE_MAX);

	bt_coex_update_profile_info(p_conn,PROFILE_LE_AUDIO,true);
}

static void rtk_handle_le_big_complete_evt(uint8_t* pdata)
{
    uint16_t big_handle = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	if (pdata[0] != 0) /* status */
		return;

	big_handle = (uint16_t)pdata[1];

	printf("[BT_COEX] %s: big_handle = 0x%x\r\n", __func__,big_handle);

	p_conn = bt_coex_find_link_by_handle(big_handle);
	if (!p_conn) {
		p_conn = (rtk_bt_coex_conn_t*) osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_conn_t));
		if(!p_conn){
			return;
		}
		memset(p_conn, 0, sizeof(rtk_bt_coex_conn_t));
		p_conn->conn_handle = big_handle;
		INIT_LIST_HEAD(&p_conn->profile_list);
		list_add_tail(&p_conn->list, &p_rtk_bt_coex_priv->conn_list);
	}	

	p_conn->profile_bitmap = 0;
	p_conn->profile_status_bitmap = 0;
	memset(p_conn->profile_refcount,0,PROFILE_MAX);

	bt_coex_update_profile_info(p_conn,PROFILE_LE_AUDIO,true);
}

static void rtk_handle_le_terminate_big_complete_evt(uint8_t* pdata)
{
    uint16_t big_handle = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	if (pdata[0] != 0) /* status */
		return;

	big_handle = (uint16_t)pdata[1];

	printf("[BT_COEX] %s: big_handle = 0x%x\r\n", __func__,big_handle);

	p_conn = bt_coex_find_link_by_handle(big_handle);
    if(p_conn) {
        if(p_conn->profile_bitmap & BIT(PROFILE_LE_AUDIO))
            bt_coex_update_profile_info(p_conn, PROFILE_LE_AUDIO, false);
		list_del(&p_conn->list);
		osif_mem_free(p_conn);	
    }
}

static void bt_coex_handle_le_meta_evt(uint8_t *pdata)
{
	uint8_t sub_evt = pdata[0];
	DBG_BT_COEX("bt_coex_handle_le_meta_evt: sub evt = 0x%x \r\n",sub_evt);
	switch(sub_evt){
		case HCI_EV_LE_CONN_COMPLETE:
			bt_coex_le_connect_complete_evt(0,pdata+1);
			break;
		case HCI_EV_LE_ENHANCED_CONN_COMPLETE:
			bt_coex_le_connect_complete_evt(1,pdata+1);
			break;
		case HCI_EV_LE_CONN_UPDATE_COMPLETE:
			bt_coex_le_update_connection_evt(pdata+1);
			break;
		case HCI_EV_LE_CIS_EST:
			rtk_handle_le_cis_established_evt(pdata+1);
			break;
		case HCI_EV_LE_CREATE_BIG_CPL:
			rtk_handle_le_big_complete_evt(pdata+1);
			break;
		case HCI_EV_LE_TERM_BIG_CPL:
			rtk_handle_le_terminate_big_complete_evt(pdata+1);
			break;
		default:
			break;
	}
}

static void bt_coex_le_handle_mode_change_evt(uint8_t *pdata)
{
	uint16_t interval = 0;
	uint16_t conn_handle = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	conn_handle = (uint16_t)((pdata[2]<<8) | pdata[1]);
	interval = (uint16_t)((pdata[5]<<8) | pdata[4]);

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if(!p_conn)
		return;

	if((p_conn->profile_bitmap & BIT(PROFILE_HID)) == 0)
		return;

	if(interval < 60){
		if((p_conn->profile_bitmap & BIT(PROFILE_HID_INTERVAL)) == 0){
			p_conn->profile_status_bitmap |= BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn,PROFILE_HID_INTERVAL,true);
		}
	}else{
		if((p_conn->profile_bitmap| BIT(PROFILE_HID_INTERVAL))){
			p_conn->profile_status_bitmap &= ~BIT(PROFILE_HID);
			bt_coex_update_profile_info(p_conn,PROFILE_HID_INTERVAL,false);
		}
	}
}

static void bt_coex_process_evt(uint8_t *pdata)
{
	uint8_t evt = pdata[0];

	if(!pdata)
		return;

	evt = pdata[0];

	switch(evt){
		case HCI_EV_MODE_CHANGE:
			bt_coex_le_handle_mode_change_evt(pdata+2);
			break;
		case HCI_EV_DISCONN_COMPLETE:
			bt_coex_handle_disconnection_complete_evt(pdata+2);
			break;
		case HCI_EV_LE_META:
			bt_coex_handle_le_meta_evt(pdata+2);
			break;
		case HCI_EV_CONN_COMPLETE:
		case HCI_EV_SYNC_CONN_COMPLETE:
			bt_coex_handle_connection_complet_evt(pdata+2);
			break;
	}
}

static int psm_to_profile(u16 psm)
{
	switch (psm) {
	case PSM_AVCTP:
	case PSM_SDP:
		return 0xFF;	//ignore

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

static rtk_bt_coex_profile_info_t* bt_coex_find_profile(rtk_bt_coex_conn_t *p_conn, uint16_t cid, uint8_t dir)
{
	bool b_find = false;
	struct list_head *plist = NULL;
	rtk_bt_coex_profile_info_t *p_profile = NULL;

	if(!list_empty(&p_conn->profile_list)){
		plist = p_conn->profile_list.next;
		while(plist != &p_conn->profile_list){
			p_profile = (rtk_bt_coex_profile_info_t *)plist;
			if((dir == DIR_IN) && (cid == p_profile->scid)){
				b_find = true;
				break;
			}

			if((dir == DIR_OUT) && (cid == p_profile->dcid)){
				b_find = true;
				break;
			}

			plist = plist->next;

		}
	}

	if(b_find)
		return p_profile;
	else
		return NULL;
}

static void bt_coex_handle_l2cap_conn_req(rtk_bt_coex_conn_t *p_conn, uint16_t psm,uint16_t scid, uint8_t dir)
{
	int idx = psm_to_profile(psm);
	rtk_bt_coex_profile_info_t *p_profile = NULL;
	rtk_bt_coex_profile_info_t *p_profile_info = NULL;
	struct list_head *plist = NULL;

	DBG_BT_COEX("bt_coex_handle_l2cap_conn_req : scid = 0x%x \r\n",scid);

	if(idx == 0xFF)
		return;

	p_profile = bt_coex_find_profile(p_conn,scid,dir);
	if(p_profile)
		return;

	p_profile = osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_profile_info_t));
	if(!p_profile)
		return;

	memset(p_profile,0,sizeof(rtk_bt_coex_profile_info_t));

	p_profile->psm = psm;
	p_profile->idx = idx;

	if(dir == DIR_OUT)
		p_profile->scid = scid;
	else if(dir == DIR_IN)
		p_profile->dcid = scid;

	if(psm == PSM_AVDTP){
		bool b_find = false;
		if(!list_empty(&p_conn->profile_list)){
			plist = p_conn->profile_list.next;
			while(plist != &p_conn->profile_list){
				p_profile_info = (rtk_bt_coex_profile_info_t *)plist;
				if(p_profile_info->psm == psm){
					b_find = true;
					break;
				}else
					plist = plist->next;
			}
		}

		if(!b_find)
			p_profile->flags = A2DP_SIGNAL;
		else
			p_profile->flags = A2DP_MEDIA;
	}

	printf("[BT_COEX] bt_coex_handle_l2cap_conn_req: p_profile->flags = %d \r\n",p_profile->flags);

	list_add_tail(&p_profile->list, &p_conn->profile_list);
}

static void bt_coex_handle_l2cap_conn_rsp(rtk_bt_coex_conn_t *p_conn, uint16_t scid, uint16_t dcid, uint8_t res,uint8_t dir)
{
	rtk_bt_coex_profile_info_t *p_profile = NULL;

	DBG_BT_COEX("bt_coex_handle_l2cap_conn_rsp: dcid = 0x%x,scid = 0x%x \r\n", dcid, scid);

	p_profile = bt_coex_find_profile(p_conn,scid,dir);

	if(!p_profile)
		return;

	if(!res){
		if(dir == DIR_IN)
			p_profile->dcid = dcid;
		else if(dir == DIR_OUT)
			p_profile->scid = dcid;

		DBG_BT_COEX("bt_coex_handle_l2cap_conn_rsp: idx = 0x%x \r\n",p_profile->idx);

		bt_coex_update_profile_info(p_conn,p_profile->idx,true);
	}
}

static void bt_coex_handle_handle_l2cap_dis_conn_req(rtk_bt_coex_conn_t *p_conn, uint16_t scid, uint16_t dcid, uint8_t dir)
{
	bool b_find = false;
	struct list_head *plist = NULL;
	rtk_bt_coex_profile_info_t *p_profile = NULL;

	if(!list_empty(&p_conn->profile_list)){
		plist = p_conn->profile_list.next;
		while(plist != &p_conn->profile_list){
			p_profile = (rtk_bt_coex_profile_info_t *)plist;
			if(dir == DIR_IN){
				if((p_profile->dcid == scid) && (p_profile->scid == dcid)){
					b_find = true;
					break;
				}
			}

			if(dir == DIR_OUT){
				if((p_profile->dcid == dcid) && (p_profile->scid == scid)){
					b_find = true;
					break;
				}
			}

			plist = plist->next;

		}
	}

	if(b_find == false)
		return;


	DBG_BT_COEX("bt_coex_handle_handle_l2cap_dis_conn_req: disconnect profile \r\n");

	if ((p_profile->idx == PROFILE_A2DP) && (p_conn->profile_bitmap & BIT(PROFILE_SINK))){
		printf("[BT_COEX] delete PROFILE Sink profile \r\n");
		p_conn->profile_bitmap &= ~(BIT(PROFILE_SINK));
	}

	DBG_BT_COEX("bt_coex_handle_handle_l2cap_dis_conn_req: p_profile->idx = 0x%x \r\n",p_profile->idx);
	bt_coex_update_profile_info(p_conn,p_profile->idx,false);

	list_del(&p_profile->list);
	osif_mem_free(p_profile);
}

static void bt_coex_packet_counter_handle(rtk_bt_coex_conn_t *p_conn, uint16_t cid, uint8_t dir)
{
	rtk_bt_coex_profile_info_t *p_profile = NULL;

	p_profile = bt_coex_find_profile(p_conn,cid,dir);

	if(!p_profile)
		return;

	if((p_profile->idx == PROFILE_A2DP) && (p_profile->flags == A2DP_MEDIA)){
		if(!(p_conn->profile_status_bitmap & BIT(PROFILE_A2DP))){
			p_conn->profile_status_bitmap |= BIT(PROFILE_A2DP);
			if(dir == DIR_IN){
				if(!(p_conn->profile_bitmap & (BIT(PROFILE_SINK))))
					p_conn->profile_bitmap |= BIT(PROFILE_SINK);
			}
			bt_coex_set_profile_info_to_fw();
		}

		p_conn->a2dp_cnt ++;
	}

	if(p_profile->idx == PROFILE_PAN)
		p_conn->pan_cnt ++;
}

void bt_coex_dump_frame(uint8_t *pdata,uint32_t len)
{
	uint32_t i = 0;

	printf("dump frame: len = 0x%lx \r\n",len);
	for(i=0;i<len;i++)
	{
		printf("0x%x  ",pdata[i]);
		if((i+1)%16 == 0)
			printf("\r\n");
	}
	printf("\r\n");
}

static void bt_coex_process_acl_data(uint8_t* pdata,uint16_t len,uint8_t dir)
{
	uint16_t conn_handle = 0;
	uint16_t channel_id = 0;
	uint16_t flags = 0;
	uint8_t code = 0;
	uint16_t psm = 0;
	uint16_t scid = 0;
	uint16_t dcid = 0;
	uint16_t res = 0;
	rtk_bt_coex_conn_t *p_conn = NULL;

	conn_handle = (uint16_t)((pdata[1]<<8) | pdata[0]);

	flags = (conn_handle>>12);

	conn_handle = conn_handle & 0xFFF;

	p_conn = bt_coex_find_link_by_handle(conn_handle);
	if(!p_conn)
		return;

	if(flags == 0x01)
		return;

	channel_id = (uint16_t)((pdata[7]<<8) | pdata[6]);

	if((channel_id == 0x0001) && len < 11)
		return;

	code = pdata[8];
	if(channel_id == 0x0001){
		switch (code){
			case L2CAP_CONN_REQ:
				psm = (uint16_t) ((pdata[13]<<8) | pdata[12]);
				scid = (uint16_t) ((pdata[15]<<8) | pdata[14]);
				DBG_BT_COEX("bt_coex_process_acl_data: L2CAP_CONN_REQ: psm = 0x%x, scid = 0x%x \r\n",psm,scid);
				bt_coex_handle_l2cap_conn_req(p_conn,psm,scid,dir);
				break;
			case L2CAP_CONN_RSP:
				dcid = (uint16_t) ((pdata[13]<<8) | pdata[12]);
				scid = (uint16_t) ((pdata[15]<<8) | pdata[14]);
				res = (uint16_t) ((pdata[17]<<8) | pdata[16]);
				DBG_BT_COEX("bt_coex_process_acl_data: L2CAP_CONN_RSP \r\n");
				bt_coex_handle_l2cap_conn_rsp(p_conn,scid,dcid,res,dir);
				break;
			case L2CAP_DISCONN_REQ:
				dcid = (uint16_t) ((pdata[13]<<8) | pdata[12]);
				scid = (uint16_t) ((pdata[15]<<8) | pdata[14]);
				bt_coex_handle_handle_l2cap_dis_conn_req(p_conn,scid,dcid,dir);
				break;
			case L2CAP_DISCONN_RSP:
				break;
			default:
				break;
		}
	}else{
		if((((p_conn->profile_bitmap & BIT(PROFILE_A2DP)) > 0) || ((p_conn->profile_bitmap & BIT(PROFILE_PAN)) > 0))){
			bt_coex_packet_counter_handle(p_conn,channel_id,DIR_IN);
		}
	}
}

static void bt_coex_monitor_timer_handler(void *arg)
{
	UNUSED(arg);
	rtk_bt_coex_monitor_node_t *p_monitor = NULL;
	rtk_bt_coex_conn_t *p_conn = NULL;
	struct list_head *plist = NULL;

	if(list_empty(&p_rtk_bt_coex_priv->monitor_list))
		return;

	plist = p_rtk_bt_coex_priv->monitor_list.next;
	while(plist != &p_rtk_bt_coex_priv->monitor_list){
		p_monitor = (rtk_bt_coex_monitor_node_t *)plist;
		p_conn = p_monitor->p_conn;

		if(p_monitor->b_first_add){
			p_monitor->b_first_add = 0;
		}else{

			if(p_monitor->profile_idx == PROFILE_A2DP){

				DBG_BT_COEX("bt_coex_monitor_timer_handler: a2dp cnt = 0x%lx \r\n",p_conn->a2dp_cnt);

				if((p_conn->a2dp_cnt == 0) && (p_conn->a2dp_pre_cnt > 0)){
					p_conn->profile_status_bitmap &= (~BIT(PROFILE_A2DP));
					bt_coex_set_profile_info_to_fw();
				}

				p_conn->a2dp_pre_cnt = p_conn->a2dp_cnt;
				p_conn->a2dp_cnt = 0;
			}

			if(p_monitor->profile_idx == PROFILE_PAN){

				if((p_conn->pan_cnt == 0) && (p_conn->pan_pre_cnt > 0)){
					p_conn->profile_status_bitmap &= (~BIT(PROFILE_PAN));
					bt_coex_set_profile_info_to_fw();
				}
				p_conn->pan_pre_cnt = p_conn->pan_cnt;
				p_conn->pan_cnt = 0;
			}
		}

		plist = plist->next;
	}
}

void bt_coex_process_rx_frame(uint8_t type,uint8_t *pdata, uint16_t len)
{
	if(!pdata)
		return;

	if(type == HCI_EVT)
		bt_coex_process_evt(pdata);

	if(type == HCI_ACL)
		bt_coex_process_acl_data(pdata,len,DIR_IN);
}

void bt_coex_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{
	if(!pdata)
		return;

	if(type == HCI_ACL)
		bt_coex_process_acl_data(pdata,len,DIR_OUT);
}

void bt_coex_init(void)
{
	printf("[BT_COEX]: Init \r\n");
	p_rtk_bt_coex_priv = (rtk_bt_coex_priv_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_coex_priv_t));
	memset(p_rtk_bt_coex_priv,0,sizeof(rtk_bt_coex_priv_t));
	INIT_LIST_HEAD(&p_rtk_bt_coex_priv->conn_list);
	INIT_LIST_HEAD(&p_rtk_bt_coex_priv->monitor_list);
	if(false == osif_mutex_create(&p_rtk_bt_coex_priv->monitor_mutex))
		return;

	if(true == osif_timer_create(&p_rtk_bt_coex_priv->monitor_timer,"bt_coex_monitor_timer",(uint32_t)NULL,BT_COEX_MONITOR_INTERVAL,true,bt_coex_monitor_timer_handler))
		osif_timer_start(&p_rtk_bt_coex_priv->monitor_timer);
}

void bt_coex_deinit(void)
{
	struct list_head *plist = NULL;
	rtk_bt_coex_monitor_node_t *p_monitor = NULL;
	rtk_bt_coex_conn_t *p_conn = NULL;

	printf("[BT_COEX]: Deinit \r\n");
	osif_timer_stop(&p_rtk_bt_coex_priv->monitor_timer);

	osif_mutex_take(p_rtk_bt_coex_priv->monitor_mutex, 0xFFFFFFFFUL);
	if(!list_empty(&p_rtk_bt_coex_priv->monitor_list)){
		plist = p_rtk_bt_coex_priv->monitor_list.next;
		while(plist != &p_rtk_bt_coex_priv->monitor_list){
			p_monitor = (rtk_bt_coex_monitor_node_t *)plist;
			plist = plist->next;
			list_del(&p_monitor->list);
			osif_mem_free(p_monitor);
		}
	}
	osif_mutex_give(p_rtk_bt_coex_priv->monitor_mutex);

	plist = NULL;
	if(!list_empty(&p_rtk_bt_coex_priv->conn_list)){
		plist = p_rtk_bt_coex_priv->conn_list.next;
		while(plist != &p_rtk_bt_coex_priv->conn_list){
			p_conn = (rtk_bt_coex_conn_t *)plist;
			plist = plist->next;
			list_del(&p_conn->list);
			osif_mem_free(p_conn);
		}
	}

	osif_mutex_delete(p_rtk_bt_coex_priv->monitor_mutex);
	osif_timer_delete(&p_rtk_bt_coex_priv->monitor_timer);
	osif_mem_free(p_rtk_bt_coex_priv);
}

#endif //defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX

#else //defined(HCI_BT_COEX_ENABLE) && HCI_BT_COEX_ENABLE

void bt_coex_init(void)
{

}

void bt_coex_process_rx_frame(u8 type,u8 *data, u16 len)
{

}

void bt_coex_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{

}

void bt_coex_deinit(void)
{

}

#endif //defined(HCI_BT_COEX_ENABLE) && HCI_BT_COEX_ENABLE

