#include <bt_api_config.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT

#include <stdio.h>
#include <stdint.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_remote_prov_model.h>
#include <remote_provisioning.h>
#include <generic_types.h>

#if defined(BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL) && BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL
static uint16_t rtk_bt_mesh_remote_prov_client_scan(rtk_bt_mesh_remote_prov_client_scan_start_t *rmt_scan)
{
	uint8_t uuid_flag = 0;
	for (int i = 0; i < 16; i++) {
		if (rmt_scan->uuid[i] != 0) {
			uuid_flag = 1;
			break;
		}
	}
	if (uuid_flag) {
		return rmt_prov_client_scan_start(rmt_scan->dst, rmt_scan->net_key_index, rmt_scan->scanned_items_limit, \
										  rmt_scan->scan_timeout, rmt_scan->uuid);
	} else {
		return rmt_prov_client_scan_start(rmt_scan->dst, rmt_scan->net_key_index, rmt_scan->scanned_items_limit, \
										  rmt_scan->scan_timeout, NULL);
	}
}

extern bool bt_stack_profile_check(rtk_bt_profile_t profile);
uint16_t bt_mesh_remote_prov_client_model_act_handle(rtk_bt_cmd_t *p_cmd)
{
	uint16_t ret = RTK_BT_MESH_MSG_SEND_CAUSE_FAIL;
	if (true != bt_stack_profile_check(RTK_BT_PROFILE_MESH)) {
		printf("Error: BLE MESH profile is not initiated\r\n");
		ret = RTK_BT_ERR_UNSUPPORTED;
		goto end;
	}
	if (!p_cmd) {
		printf("[%s] param is NULL!\r\n", __func__);
		return RTK_BT_FAIL;
	}
	switch (p_cmd->act) {
	case RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_SCAN_START:
		ret = rtk_bt_mesh_remote_prov_client_scan(p_cmd->param);
		break;
	case RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_SCAN_CAPA_GET: {
		rtk_bt_mesh_remote_prov_client_scan_capa_get_t *scan_capa_get;
		scan_capa_get = (rtk_bt_mesh_remote_prov_client_scan_capa_get_t *)p_cmd->param;
		ret = rmt_prov_client_scan_caps_get(scan_capa_get->dst, scan_capa_get->net_key_index);
		break;
	}
	case RTK_BT_MESH_REMOTE_PROV_CLIENT_ACT_LINK_OPEN: {
		rtk_bt_mesh_remote_prov_client_link_open_t *link_open;
		link_open = (rtk_bt_mesh_remote_prov_client_link_open_t *)p_cmd->param;
		ret = rmt_prov_client_link_open_prov(link_open->dst, link_open->net_key_index, link_open->uuid, link_open->link_open_timeout);
		break;
	}
	default:
		printf("[%s] Unknown act:%d\r\n", __func__, p_cmd->act);
		break;
	}
	ret = ret | RTK_BT_STACK_MESH_ERROR_FLAG;
end:
	p_cmd->ret = ret;
	osif_sem_give(p_cmd->psem);
	return ret;
}

static int32_t remote_prov_client_data(const mesh_model_info_p pmodel_info, uint32_t type, void *pargs)
{
	(void)pmodel_info;
	int32_t ret = MODEL_SUCCESS;
	switch (type) {
	case RMT_PROV_CLIENT_SCAN_CAPS_STATUS: {
		rmt_prov_client_scan_caps_status_t *pdata = (rmt_prov_client_scan_caps_status_t *)pargs;
		rtk_bt_mesh_rmt_prov_client_scan_capa_status_t *scan_capa_status;
		rtk_bt_evt_t *p_evt = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_REMOTE_PROV_CLIENT_MODEL, RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_SCAN_CAPA_STATUS,
									sizeof(rtk_bt_mesh_rmt_prov_client_scan_capa_status_t));
		scan_capa_status = (rtk_bt_mesh_rmt_prov_client_scan_capa_status_t *)p_evt->data;
		scan_capa_status->src = pdata->src;
		scan_capa_status->max_scan_items = pdata->max_scanned_items;
		scan_capa_status->support_active_scan = pdata->support_active_scan;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case RMT_PROV_CLIENT_SCAN_STATUS: {
		rmt_prov_client_scan_status_t *pdata = (rmt_prov_client_scan_status_t *)pargs;
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_rmt_prov_client_scan_status_t *scan_status = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_REMOTE_PROV_CLIENT_MODEL, RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_SCAN_STATUS,
									sizeof(rtk_bt_mesh_rmt_prov_client_scan_status_t));
		scan_status = (rtk_bt_mesh_rmt_prov_client_scan_status_t *)p_evt->data;
		scan_status->src = pdata->src;
		scan_status->prov_status = pdata->status;
		scan_status->scan_status = pdata->scan_state;
		scan_status->scanned_items_limit = pdata->scanned_items_limit;
		scan_status->timeout = pdata->timeout;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case RMT_PROV_CLIENT_SCAN_REPORT: {
		rmt_prov_client_scan_report_t *pdata = (rmt_prov_client_scan_report_t *)pargs;
		uint16_t oob = 0xffff;
		if (pdata->poob) {
			oob = (pdata->poob[1] << 8) + pdata->poob[0];
		}
		uint32_t uri_hash = 0;
		if (pdata->puri_hash) {
			uri_hash = (pdata->puri_hash[3] << 24) + (pdata->puri_hash[2] << 16) +
					   (pdata->puri_hash[1] << 8) + pdata->puri_hash[0];
		}
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_rmt_prov_client_scan_report_t *scan_report = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_REMOTE_PROV_CLIENT_MODEL, RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_SCAN_REPORT,
									sizeof(rtk_bt_mesh_rmt_prov_client_scan_report_t));
		scan_report = (rtk_bt_mesh_rmt_prov_client_scan_report_t *)p_evt->data;
		scan_report->src = pdata->src;
		scan_report->rssi = pdata->rssi;
		scan_report->oob = oob;
		scan_report->uri_hash = uri_hash;
		memcpy(scan_report->uuid, pdata->uuid, 16);
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case RMT_PROV_CLIENT_EXTENED_SCAN_REPORT: {
		rmt_prov_client_extened_scan_report_t *pdata = (rmt_prov_client_extened_scan_report_t *)pargs;
		uint16_t oob = 0xffff;
		if (pdata->poob) {
			oob = (pdata->poob[1] << 8) + pdata->poob[0];
		}
		printf("rmt_prov_extened_scan_report: oob %d, uuid ", oob);
		data_uart_dump(pdata->uuid, 16);
		if (pdata->adv_structs_len > 0) {
			printf("rmt_prov_extened_scan_report: adv structs ");
			data_uart_dump(pdata->padv_structs, pdata->adv_structs_len);
		}
		break;
	}
	case RMT_PROV_CLIENT_LINK_STATUS: {
		rmt_prov_client_link_status_t *pdata = (rmt_prov_client_link_status_t *)pargs;
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_rmt_prov_client_link_status_t *link_status = NULL;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_REMOTE_PROV_CLIENT_MODEL, RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_LINK_STATUS,
									sizeof(rtk_bt_mesh_rmt_prov_client_link_status_t));
		link_status = (rtk_bt_mesh_rmt_prov_client_link_status_t *)p_evt->data;
		link_status->src = pdata->src;
		link_status->prov_status = pdata->status;
		link_status->link_status = pdata->link_state;
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	case RMT_PROV_CLIENT_LINK_REPORT: {
		rmt_prov_client_link_report_t *pdata = (rmt_prov_client_link_report_t *)pargs;
		rtk_bt_evt_t *p_evt = NULL;
		rtk_bt_mesh_rmt_prov_client_link_report_t *link_report;
		p_evt = rtk_bt_event_create(RTK_BT_LE_GP_MESH_REMOTE_PROV_CLIENT_MODEL, RTK_BT_MESH_REMOTE_PROV_CLIENT_EVT_LINK_REPORT,
									sizeof(rtk_bt_mesh_rmt_prov_client_link_report_t));
		link_report = (rtk_bt_mesh_rmt_prov_client_link_report_t *)p_evt->data;
		link_report->src = pdata->src;
		link_report->prov_status = pdata->status;
		link_report->link_status = pdata->link_state;
		if (pdata->preason) {
			link_report->close_reason = *pdata->preason;
		} else {
			link_report->close_reason = RTK_BT_MESH_RMT_PROV_LINK_NOT_CLOSE;
		}
		rtk_bt_evt_indicate(p_evt, NULL);
		break;
	}
	default:
		printf("[%s] Unknown type:%d\r\n", __func__, (int)type);
		break;
	}

	return ret;
}

bool remote_prov_client_init(void)
{
	/* register remote provisioning client model */
	return rmt_prov_client_reg(0, remote_prov_client_data);
}

#endif // end of BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL
#endif // end of RTK_BLE_MESH_SUPPORT
