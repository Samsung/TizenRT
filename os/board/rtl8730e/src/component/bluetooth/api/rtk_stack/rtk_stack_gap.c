/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <dlist.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_common.h>
#include <rtk_stack_config.h>
#include <rtk_stack_internal.h>
#include <bt_api_config.h>

#include <gap.h>
#include <gap_ecfc.h>
#include <gap_vendor.h>

uint16_t bt_stack_gap_init(void)
{
#if (defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT) && (defined(F_BT_5_2_L2C_ECFC_SUPPORT) && F_BT_5_2_L2C_ECFC_SUPPORT)
	if (!gap_ecfc_init(GAP_MAX_ECFC_PROTOCAL_NUM)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}
#endif

	gap_register_vendor_cb(bt_stack_vendor_callback);

	return 0;
}

#if (defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT) && (defined(F_BT_5_2_L2C_ECFC_SUPPORT) && F_BT_5_2_L2C_ECFC_SUPPORT)
uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
{
	rtk_bt_cmd_t *p_cmd = NULL;
	rtk_bt_evt_t *p_evt = NULL;

	switch (msg) {
	case GAP_ECFC_PROTO_REG_RSP: {
		T_GAP_ECFC_PROTO_REG_RSP *p_rsp = (T_GAP_ECFC_PROTO_REG_RSP *)p_buf;
		BT_LOGD("GAP_ECFC_PROTO_REG_RSP: proto_id %d, psm 0x%x, cause 0x%x\r\n",
				p_rsp->proto_id, p_rsp->psm, p_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg);
		if (p_cmd) {
			rtk_bt_gap_ecfc_register_t *param = (rtk_bt_gap_ecfc_register_t *)p_cmd->param;
			if (param->psm == p_rsp->psm) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = p_rsp->cause;
				*param->p_proto_id = p_rsp->proto_id;
				osif_sem_give(p_cmd->psem);
			}
		}
		break;
	}
	case GAP_ECFC_SEC_REG_RSP: {
		T_GAP_ECFC_SEC_REG_RSP *p_rsp = (T_GAP_ECFC_SEC_REG_RSP *)p_buf;
		BT_LOGD("GAP_ECFC_SEC_REG_RSP: is_le %d, psm 0x%x, active %d, uuid 0x%x, cause 0x%x\r\n",
				p_rsp->is_le, p_rsp->psm, p_rsp->active, p_rsp->uuid, p_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg);
		if (p_cmd) {
			rtk_bt_gap_ecfc_sec_reg_req_t *param = (rtk_bt_gap_ecfc_sec_reg_req_t *)p_cmd->param;
			if (param->psm == p_rsp->psm) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = p_rsp->cause;
				osif_sem_give(p_cmd->psem);
			}
		}
		break;
	}
	case GAP_ECFC_CONN_IND: {
		rtk_bt_ecfc_conn_req_ind_t *p_ind = NULL;
		T_GAP_ECFC_CONN_IND *p_info = (T_GAP_ECFC_CONN_IND *)p_buf;
		BT_LOGD("GAP_ECFC_CONN_IND: proto_id %d, conn_handle %d, remote_mtu %d, cid_num %d, cid %d %d %d %d %d, bd_addr %02x:%02x:%02x:%02x:%02x:%02x, bd_type 0x%x, identity_id %d\r\n",
				p_info->proto_id, p_info->conn_handle, p_info->remote_mtu, p_info->cid_num,
				p_info->cid[0], p_info->cid[1], p_info->cid[2], p_info->cid[3], p_info->cid[4],
				p_info->bd_addr[5], p_info->bd_addr[4], p_info->bd_addr[3], p_info->bd_addr[2], p_info->bd_addr[1], p_info->bd_addr[0],
				p_info->bd_type, p_info->identity_id);
		p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_ECFC_CONN_REQ_IND,
									sizeof(rtk_bt_ecfc_conn_req_ind_t));
		p_ind = (rtk_bt_ecfc_conn_req_ind_t *)p_evt->data;
		memcpy(p_ind, p_info, sizeof(rtk_bt_ecfc_conn_req_ind_t));
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GAP_ECFC_CONN_RSP: {
		T_GAP_ECFC_CONN_RSP *p_rsp = (T_GAP_ECFC_CONN_RSP *)p_buf;
		BT_LOGD("GAP_ECFC_CONN_RSP: proto_id %d, cid_num %d, cause 0x%x, bd_addr %02x:%02x:%02x:%02x:%02x:%02x, bd_type 0x%x\r\n",
				p_rsp->proto_id, p_rsp->cid_num, p_rsp->cause,
				p_rsp->bd_addr[5], p_rsp->bd_addr[4], p_rsp->bd_addr[3], p_rsp->bd_addr[2], p_rsp->bd_addr[1], p_rsp->bd_addr[0],
				p_rsp->bd_type);
		p_cmd = bt_stack_pending_cmd_search(msg);
		if (p_cmd) {
			rtk_bt_gap_ecfc_conn_req_t *param = (rtk_bt_gap_ecfc_conn_req_t *)p_cmd->param;
			if (param->proto_id == p_rsp->proto_id) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = p_rsp->cause;
				osif_sem_give(p_cmd->psem);
			}
		}
		break;
	}
	case GAP_ECFC_CONN_CMPL: {
		rtk_bt_ecfc_conn_cmpl_ind_t *p_ind = NULL;
		T_GAP_ECFC_CONN_CMPL_INFO *p_info = (T_GAP_ECFC_CONN_CMPL_INFO *)p_buf;
		BT_LOGD("GAP_ECFC_CONN_CMPL: proto_id %d, cause 0x%x, conn_handle %d, ds_data_offset %d, bd_addr %02x:%02x:%02x:%02x:%02x:%02x, bd_type 0x%x\r\n",
				p_info->proto_id, p_info->cause, p_info->conn_handle, p_info->ds_data_offset,
				p_info->bd_addr[5], p_info->bd_addr[4], p_info->bd_addr[3], p_info->bd_addr[2], p_info->bd_addr[1], p_info->bd_addr[0],
				p_info->bd_type);
		BT_LOGD("GAP_ECFC_CONN_CMPL: remote_mtu %d, local_mtu %d, local_mps %d\r\n",
				p_info->remote_mtu, p_info->local_mtu, p_info->local_mps);
		p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_ECFC_CONN_CMPL_IND,
									sizeof(rtk_bt_ecfc_conn_cmpl_ind_t));
		p_ind = (rtk_bt_ecfc_conn_cmpl_ind_t *)p_evt->data;
		memcpy(p_ind, p_info, sizeof(rtk_bt_ecfc_conn_cmpl_ind_t));
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GAP_ECFC_DATA_IND: {
		rtk_bt_ecfc_data_ind_t *p_ind = NULL;
		T_GAP_ECFC_DATA_IND *p_info = (T_GAP_ECFC_DATA_IND *)p_buf;
		BT_LOGD("GAP_ECFC_DATA_IND: proto_id %d, conn_handle %d, cid %d, length %d\r\n",
				p_info->proto_id, p_info->conn_handle, p_info->cid, p_info->length);
		p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_ECFC_DATA_IND,
									sizeof(rtk_bt_ecfc_data_ind_t) + p_info->length);
		p_ind = (rtk_bt_ecfc_data_ind_t *)p_evt->data;
		p_ind->proto_id = p_info->proto_id;
		p_ind->conn_handle = p_info->conn_handle;
		p_ind->cid = p_info->cid;
		p_ind->length = p_info->length;
		p_ind->data = (uint8_t *)p_ind + sizeof(rtk_bt_ecfc_data_ind_t);
		memcpy(p_ind->data, p_info->data + p_info->gap, p_info->length);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GAP_ECFC_DATA_RSP: {
		T_GAP_ECFC_DATA_RSP *p_rsp = (T_GAP_ECFC_DATA_RSP *)p_buf;
		BT_LOGD("GAP_ECFC_DATA_RSP: proto_id %d, conn_handle %d, cid %d, cause 0x%x\r\n",
				p_rsp->proto_id, p_rsp->conn_handle, p_rsp->cid, p_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg);
		if (p_cmd) {
			rtk_bt_gap_ecfc_send_data_t *param = (rtk_bt_gap_ecfc_send_data_t *)p_cmd->param;
			if (param->conn_handle == p_rsp->conn_handle && param->cid == p_rsp->cid) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = p_rsp->cause;
				osif_sem_give(p_cmd->psem);
			}
		}
		break;
	}
	case GAP_ECFC_DISCONN_IND: {
		rtk_bt_ecfc_disconn_ind_t *p_ind = NULL;
		T_GAP_ECFC_DISCONN_IND *p_info = (T_GAP_ECFC_DISCONN_IND *)p_buf;
		BT_LOGD("GAP_ECFC_DISCONN_IND: proto_id %d, conn_handle 0x%d, cid 0x%x, cause 0x%x\r\n",
				p_info->proto_id, p_info->conn_handle, p_info->cid, p_info->cause);
		p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_ECFC_DISCONN_IND,
									sizeof(rtk_bt_ecfc_disconn_ind_t));
		p_ind = (rtk_bt_ecfc_disconn_ind_t *)p_evt->data;
		memcpy(p_ind, p_info, sizeof(rtk_bt_ecfc_disconn_ind_t));
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case GAP_ECFC_DISCONN_RSP: {
		T_GAP_ECFC_DISCONN_RSP *p_rsp = (T_GAP_ECFC_DISCONN_RSP *)p_buf;
		BT_LOGD("GAP_ECFC_DISCONN_RSP: proto_id %d, conn_handle 0x%x, cid 0x%x, cause 0x%x\r\n",
				p_rsp->proto_id, p_rsp->conn_handle, p_rsp->cid, p_rsp->cause);
		p_cmd = bt_stack_pending_cmd_search(msg);
		if (p_cmd) {
			rtk_bt_gap_ecfc_disconn_req_t *param = (rtk_bt_gap_ecfc_disconn_req_t *)p_cmd->param;
			if (param->conn_handle == p_rsp->conn_handle && param->cid == p_rsp->cid) {
				bt_stack_pending_cmd_delete(p_cmd);
				p_cmd->ret = p_rsp->cause;
				osif_sem_give(p_cmd->psem);
			}
		}
		break;
	}
	case GAP_ECFC_RECONFIGURE_IND: {
		rtk_bt_ecfc_reconf_req_ind_t *p_ind = NULL;
		uint8_t reconf_ret = 0;
		T_GAP_ECFC_RECONFIGURE_IND *p_info = (T_GAP_ECFC_RECONFIGURE_IND *)p_buf;
		BT_LOGD("GAP_ECFC_RECONFIGURE_IND: proto_id %d, conn_handle %d, remote_mtu %d, remote_mps %d, cid_num %d, cid %d %d %d %d %d\r\n",
				p_info->proto_id, p_info->conn_handle, p_info->remote_mtu, p_info->remote_mps, p_info->cid_num,
				p_info->cid[0], p_info->cid[1], p_info->cid[2], p_info->cid[3], p_info->cid[4]);
		p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND,
									sizeof(rtk_bt_ecfc_reconf_req_ind_t));
		p_ind = (rtk_bt_ecfc_reconf_req_ind_t *)p_evt->data;
		memcpy(p_ind, p_info, sizeof(rtk_bt_ecfc_reconf_req_ind_t));
		rtk_bt_evt_indicate(p_evt, &reconf_ret);
		if (reconf_ret == RTK_BT_EVT_CB_REJECT) {
			return GAP_ECFC_RCFG_UNACCEPTABLE_PARAMS;
		} else {
			return GAP_ECFC_RCFG_ACCEPT;
		}
		break;
	}
	case GAP_ECFC_RECONFIGURE_RSP: {
		rtk_bt_ecfc_reconf_rsp_ind_t *p_ind = NULL;
		T_GAP_ECFC_RECONFIGURE_RSP *p_rsp = (T_GAP_ECFC_RECONFIGURE_RSP *)p_buf;
		BT_LOGD("GAP_ECFC_RECONFIGURE_RSP: proto_id %d, cause 0x%x, conn_handle 0x%x, local_mtu %d, local_mps %d,cid_num %d, cid %d %d %d %d %d\r\n",
				p_rsp->proto_id, p_rsp->cause, p_rsp->conn_handle, p_rsp->local_mtu, p_rsp->local_mps, p_rsp->cid_num,
				p_rsp->cid[0], p_rsp->cid[1], p_rsp->cid[2], p_rsp->cid[3], p_rsp->cid[4]);
		p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_ECFC_RECONF_RSP_IND,
									sizeof(rtk_bt_ecfc_reconf_rsp_ind_t));
		p_ind = (rtk_bt_ecfc_reconf_rsp_ind_t *)p_evt->data;
		memcpy(p_ind, p_rsp, sizeof(rtk_bt_ecfc_reconf_rsp_ind_t));
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	default:
		break;
	}
	return 0;
}

static uint16_t bt_stack_gap_ecfc_set_param(void *arg)
{
	rtk_bt_gap_ecfc_param_set_t *param = (rtk_bt_gap_ecfc_param_set_t *)arg;

	if (!gap_ecfc_set_param((T_GAP_ECFC_PARAM_TYPE)param->type, 2, &param->value)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_register(void *arg)
{
	rtk_bt_gap_ecfc_register_t *param = (rtk_bt_gap_ecfc_register_t *)arg;

	if (!gap_ecfc_reg_proto(param->psm, app_ecfc_callback, param->is_le, (uint8_t *)param->p_proto_id, (uint8_t)param->data_path)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_send_sec_reg_req(void *arg)
{
	rtk_bt_gap_ecfc_sec_reg_req_t *param = (rtk_bt_gap_ecfc_sec_reg_req_t *)arg;

	if (!gap_ecfc_send_sec_reg_req(param->is_le, (uint8_t)param->active, param->psm,
								   param->uuid, param->requirement, param->key_size)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_send_conn_req(void *arg)
{
	rtk_bt_gap_ecfc_conn_req_t *param = (rtk_bt_gap_ecfc_conn_req_t *)arg;

	if (!gap_ecfc_send_conn_req(param->psm, param->proto_id, param->local_mtu, param->bd_addr, param->bd_type, param->cid_num)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_send_conn_cfm(void *arg)
{
	rtk_bt_gap_ecfc_conn_cfm_t *param = (rtk_bt_gap_ecfc_conn_cfm_t *)arg;

	if (!gap_ecfc_send_conn_cfm(param->conn_handle, param->identity_id, (T_GAP_ECFC_CONN_CFM_CAUSE)param->cause,
								param->p_cid, param->cid_num, param->local_mtu)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_send_disconn_req(void *arg)
{
	rtk_bt_gap_ecfc_disconn_req_t *param = (rtk_bt_gap_ecfc_disconn_req_t *)arg;

	if (!gap_ecfc_send_disconn_req(param->conn_handle, param->cid)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_send_data(void *arg)
{
	rtk_bt_gap_ecfc_send_data_t *param = (rtk_bt_gap_ecfc_send_data_t *)arg;

	if (!gap_ecfc_send_data_req(param->conn_handle, param->cid, param->p_data, param->length)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

static uint16_t bt_stack_gap_ecfc_send_reconf_req(void *arg)
{
	rtk_bt_gap_ecfc_reconf_req_t *param = (rtk_bt_gap_ecfc_reconf_req_t *)arg;

	if (!gap_ecfc_send_reconfigure_req(param->conn_handle, param->p_cid, param->cid_num,
									   param->local_mtu, param->local_mps)) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}
#endif

static uint16_t bt_stack_gap_vendor_cmd_req(void *param)
{
	T_GAP_CAUSE cause;
	rtk_bt_gap_vendor_cmd_param_t *vendor_param = (rtk_bt_gap_vendor_cmd_param_t *)param;

	cause = gap_vendor_cmd_req(vendor_param->op, vendor_param->len, vendor_param->cmd_param);
	if (cause) {
		return RTK_BT_ERR_LOWER_STACK_API;
	}

	return 0;
}

uint16_t bt_stack_gap_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_FAIL;
	bool async_handle = false;
	BT_LOGD("bt_stack_gap_act_handle: act = %d \r\n", p_cmd->act);
	switch (p_cmd->act) {

#if (defined(RTK_BT_5_2_L2C_ECFC_SUPPORT) && RTK_BT_5_2_L2C_ECFC_SUPPORT) && (defined(F_BT_5_2_L2C_ECFC_SUPPORT) && F_BT_5_2_L2C_ECFC_SUPPORT)
	case RTK_BT_GAP_ACT_ECFC_SET_PARAM:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_SET_PARAM \r\n");
		ret = bt_stack_gap_ecfc_set_param(p_cmd->param);
		break;
	case RTK_BT_GAP_ACT_ECFC_SEC_REG_REQ:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_SEC_REG_REQ \r\n");
		p_cmd->user_data = GAP_ECFC_SEC_REG_RSP;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_gap_ecfc_send_sec_reg_req(p_cmd->param);
		async_handle = true;
		break;
	case RTK_BT_GAP_ACT_ECFC_REGISTER:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_REGISTER \r\n");
		p_cmd->user_data = GAP_ECFC_PROTO_REG_RSP;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_gap_ecfc_register(p_cmd->param);
		async_handle = true;
		break;
	case RTK_BT_GAP_ACT_ECFC_CONN_REQ:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_CONN_REQ \r\n");
		p_cmd->user_data = GAP_ECFC_CONN_RSP;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_gap_ecfc_send_conn_req(p_cmd->param);
		async_handle = true;
		break;
	case RTK_BT_GAP_ACT_ECFC_CONN_CFM:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_CONN_CFM \r\n");
		ret = bt_stack_gap_ecfc_send_conn_cfm(p_cmd->param);
		break;
	case RTK_BT_GAP_ACT_ECFC_DISCONN_REQ:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_DISCONN_REQ \r\n");
		p_cmd->user_data = GAP_ECFC_DISCONN_RSP;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_gap_ecfc_send_disconn_req(p_cmd->param);
		async_handle = true;
		break;
	case RTK_BT_GAP_ACT_ECFC_SEND_DATA:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_SEND_DATA \r\n");
		p_cmd->user_data = GAP_ECFC_DATA_RSP;
		bt_stack_pending_cmd_insert(p_cmd);
		ret = bt_stack_gap_ecfc_send_data(p_cmd->param);
		async_handle = true;
		break;
	case RTK_BT_GAP_ACT_ECFC_RECONF_REQ:
		BT_LOGD("RTK_BT_GAP_ACT_ECFC_RECONF_REQ \r\n");
		ret = bt_stack_gap_ecfc_send_reconf_req(p_cmd->param);
		break;
#endif
	case RTK_BT_GAP_ACT_VENDOR_CMD_REQ:
		ret = bt_stack_gap_vendor_cmd_req(p_cmd->param);
		break;

	default:
		BT_LOGE("bt_stack_gap_act_handle: unknown act: %d \r\n", p_cmd->act);
		ret = RTK_BT_ERR_NO_CASE_ELEMENT;
		break;
	}

	if (async_handle) {
		if (ret) {
			/*  if bt_stack_xxx failed, shall not wait for callback, and the pending p_cmd
			    should be deleted here */
			bt_stack_pending_cmd_delete(p_cmd);
			p_cmd->ret = ret;
			osif_sem_give(p_cmd->psem);
		}
	} else {
		p_cmd->ret = ret;
		osif_sem_give(p_cmd->psem);
	}

	return ret;
}
