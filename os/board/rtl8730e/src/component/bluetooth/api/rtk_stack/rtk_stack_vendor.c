/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <rtk_stack_internal.h>
#include <bt_api_config.h>
#include <bt_vendor_config.h>
#include <gap_vendor.h>
#include <rtk_stack_vendor.h>
#include <rtk_bt_vendor.h>
#include <rtk_bt_common.h>

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <trace_app.h>
#include <mesh_api.h>
#include <rtk_bt_device.h>

#if defined(VENDOR_CMD_ONE_SHOT_ADV_SUPPORT) && VENDOR_CMD_ONE_SHOT_ADV_SUPPORT
//For RTK mesh stack internal use
T_GAP_CAUSE le_vendor_one_shot_adv(void)
{
	uint8_t len = 1;
	uint8_t param[1] = {1};

	return gap_vendor_cmd_req(VENDOR_CMD_ONE_SHOT_ADV_OPCODE, len, param);
}
#endif
#endif

#if defined(VENDOR_CMD_LE_EXTENSION_FEATURE_SUPPORT) && VENDOR_CMD_LE_EXTENSION_FEATURE_SUPPORT
T_GAP_CAUSE le_ae_coding_scheme(T_GAP_AE_CODING_SCHEME coding_scheme)
{
	uint8_t param[2];
	param[0] = HCI_EXT_SUB_SET_CONTROLLER_PREFERRED_TX_CI;
	param[1] = coding_scheme;

	return gap_vendor_cmd_req(VENDOR_CMD_LE_EXTENSION_FEATURE_OPCODE, 2, param);
}
#endif

#if defined(VENDOR_CMD_PLATFORM_EXTENSION_FEATURE_SUPPORT) && VENDOR_CMD_PLATFORM_EXTENSION_FEATURE_SUPPORT
T_GAP_CAUSE le_vendor_ae_scheme(void)
{
	uint8_t param[3];
	param[0] = HCI_VENDOR_EXT_SUB_SET_FEATURE;
	param[1] = HCI_VENDOR_BT_FW_FEATURE_LEGACY_ADV_ON_CODED_PHY;
	param[2] = 0x1;

	return gap_vendor_cmd_req(VENDOR_CMD_PLATFORM_EXTENSION_FEATURE_OPCODE, 3, param);
}
#endif

#if defined(VENDOR_CMD_SET_MESH_INFO_SUPPORT) && VENDOR_CMD_SET_MESH_INFO_SUPPORT
T_GAP_CAUSE le_vendor_set_mesh_packet_priority(T_GAP_LE_MESH_PACKET_PRIORITY prio)
{
	uint8_t param[1] = {0};

	if (GAP_LE_MESH_PACKET_PRIORITY_HIGH == prio) {
		param[0] = 0x01;
	}

	return gap_vendor_cmd_req(VENDOR_CMD_SET_MESH_INFO_OPCODE, 1, param);
}
#endif

#if defined(VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT) && VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT
T_GAP_CAUSE le_vendor_free_run_clock_latch(void)
{
	uint8_t param[1];
	param[0] = SUB_CMD_FREE_RUN_CLOCK_LATCH;

	return gap_vendor_cmd_req(VENDOR_CMD_FREE_RUN_CLOCK_LATCH_OPCODE, 1, param);
}

T_GAP_CAUSE le_vendor_free_run_clock_latch_enable(uint8_t op)
{
	uint8_t param[2];
	param[0] = SUB_CMD_FREE_RUN_CLOCK_LATCH_ENABLE;
	param[1] = op;

	return gap_vendor_cmd_req(VENDOR_CMD_FREE_RUN_CLOCK_LATCH_OPCODE, 2, param);
}
#endif

void bt_stack_vendor_callback(uint8_t cb_type, void *p_cb_data)
{
	T_GAP_VENDOR_CB_DATA cb_data;
	memcpy(&cb_data, p_cb_data, sizeof(T_GAP_VENDOR_CB_DATA));
	T_GAP_VENDOR_CMD_RSP *cmd_rsp = cb_data.p_gap_vendor_cmd_rsp;
	T_GAP_VENDOR_EVT_INFO *evt_info = cb_data.p_gap_vendor_evt_info;

	switch (cb_type) {
	case GAP_MSG_VENDOR_CMD_RSP:
		// BT_LOGA("[bt_vendor_cb] cmd rsp, command: 0x%x, cause: 0x%x, "
		//     "is_cmpl_evt: %d, param_len: %d\r\n", cmd_rsp->command, cmd_rsp->cause,
		//     cmd_rsp->is_cmpl_evt, cmd_rsp->param_len);
		switch (cb_data.p_gap_vendor_cmd_rsp->command) {
#if defined(VENDOR_CMD_READ_BT_REGISTER_SUPPORT) && VENDOR_CMD_READ_BT_REGISTER_SUPPORT
		case VENDOR_CMD_READ_BT_REGISTER_OPCODE:
		case VENDOR_CMD_READ_PI_BT_RF_REGISTER_OPCODE:
		case VENDOR_CMD_READ_BT_RF_REGISTER_OPCODE: {
			uint16_t bt_reg = 0;
			uint16_t buf = 0;
			uint8_t *pdata = &(cmd_rsp->param[0]);
			buf = *(pdata + 1);
			bt_reg = *pdata;
			bt_reg |= (buf << 8);
			BT_LOGA("[bt_vendor_cb] bt register is 0x%04x \r\n", bt_reg);
		}
		break;
#endif
#if defined(VENDOR_CMD_ONE_SHOT_ADV_SUPPORT) && VENDOR_CMD_ONE_SHOT_ADV_SUPPORT
		case VENDOR_CMD_ONE_SHOT_ADV_OPCODE: {
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
			if (rtk_bt_mesh_is_enable()) {
				if (cmd_rsp->cause) {
					BT_LOGE("[%s] One shot adv resp fail: cause 0x%x", __func__, cmd_rsp->cause);
				} else {
					gap_sched_adv_done(GAP_SCHED_ADV_END_TYPE_SUCCESS);
				}
			}
#endif
		}
		break;
#endif
#if defined(VENDOR_CMD_SET_MESH_INFO_SUPPORT) && VENDOR_CMD_SET_MESH_INFO_SUPPORT
		case VENDOR_CMD_SET_MESH_INFO_OPCODE: {
			if (cmd_rsp->cause) {
				BT_LOGE("[%s] error cmd rsp, command: 0x%x, cause: 0x%x, is_cmpl_evt: %d, param_len: %d.\r\n", __func__, cmd_rsp->command, cmd_rsp->cause, cmd_rsp->is_cmpl_evt,
						cmd_rsp->param_len);
			}
		}
		break;
#endif
#if (defined(VENDOR_CMD_SET_TX_POWER_SUPPORT) && VENDOR_CMD_SET_TX_POWER_SUPPORT) || \
(defined(VENDOR_CMD_LE_EXTENSION_FEATURE_SUPPORT) && VENDOR_CMD_LE_EXTENSION_FEATURE_SUPPORT) || \
(defined(VENDOR_CMD_READ_ACC_HIT_COUNTER_SUPPORT) && VENDOR_CMD_READ_ACC_HIT_COUNTER_SUPPORT)
		case VENDOR_CMD_LE_EXTENSION_FEATURE_OPCODE: {
#if defined(VENDOR_CMD_SET_TX_POWER_SUPPORT) && VENDOR_CMD_SET_TX_POWER_SUPPORT
			if (cmd_rsp->param_len >= 1 && (SUB_CMD_SET_ADV_TX_POWER == cmd_rsp->param[0] || SUB_CMD_SET_CONN_TX_POWER == cmd_rsp->param[0])) {
				if (cmd_rsp->cause) {
					BT_LOGE("[%s] Set tx power fail, cause:0x%x.\r\n", __func__, cmd_rsp->cause);
				} else {
					BT_LOGA("[%s] Set tx power success.\r\n", __func__);
				}
			}
#endif
#if defined(VENDOR_CMD_LE_EXTENSION_FEATURE_SUPPORT) && VENDOR_CMD_LE_EXTENSION_FEATURE_SUPPORT
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT && defined(RTK_BLE_MESH_BASED_ON_CODED_PHY) && RTK_BLE_MESH_BASED_ON_CODED_PHY
			if (rtk_bt_mesh_is_enable()) {
				if (cmd_rsp->param_len >= 1 && HCI_EXT_SUB_SET_CONTROLLER_PREFERRED_TX_CI == cmd_rsp->param[0]) {
					if (cmd_rsp->cause) {
						BT_LOGE("[%s] Vendor cmd extension feature of opcode:0x%x subcmd:0x%x fail, cause:0x%x.\r\n", __func__, VENDOR_CMD_LE_EXTENSION_FEATURE_OPCODE,
								cmd_rsp->param[0], cmd_rsp->cause);
					} else {
						gap_sched_handle_ae_coding_scheme_set_done();
					}
				}
			}
#endif
#endif
#if defined(VENDOR_CMD_READ_ACC_HIT_COUNTER_SUPPORT) && VENDOR_CMD_READ_ACC_HIT_COUNTER_SUPPORT
#if defined(RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT) && RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT
			if (cmd_rsp->param_len >= 1 && SUB_CMD_READ_ACC_HIT_COUNTER == cmd_rsp->param[0]) {
				rtk_bt_vendor_conn_clock_info_t *p_slave_clock_info = rtk_bt_get_slave_clock_info();
				if (p_slave_clock_info) {
					memset(&(p_slave_clock_info->info), 0, sizeof(p_slave_clock_info->info));
					if (cmd_rsp->cause) {
						BT_LOGE("[%s] read acc hit counter fail, cause:0x%x.\r\n", __func__, cmd_rsp->cause);
						p_slave_clock_info->info.status = RTK_BT_LE_CLOCK_STATUS_CONN_HANDLE_ERROR;
					} else if (cmd_rsp->param_len >= 4) {
						uint8_t acc_hit = cmd_rsp->param[3];
						p_slave_clock_info->info.conn_handle = LE_TO_U16(&(cmd_rsp->param[1]));

						if (acc_hit) {
							BT_LOGA("[%s] read acc hit counter success.\r\n", __func__);
							p_slave_clock_info->info.status = RTK_BT_LE_CLOCK_STATUS_SUCCESS;
							p_slave_clock_info->info.clock_counter =
								((uint64_t)cmd_rsp->param[4])        |
								((uint64_t)cmd_rsp->param[5] << 8)   |
								((uint64_t)cmd_rsp->param[6] << 16)  |
								((uint64_t)cmd_rsp->param[7] << 24)  |
								((uint64_t)cmd_rsp->param[8] << 32);
							p_slave_clock_info->info.event_counter = LE_TO_U16(&(cmd_rsp->param[9]));
							p_slave_clock_info->info.ce_interval   = LE_TO_U16(&(cmd_rsp->param[11]));
						} else {
							BT_LOGA("[%s] read acc hit counter acc_hit=%d.\r\n", __func__, acc_hit);
							p_slave_clock_info->info.status = RTK_BT_LE_CLOCK_STATUS_GET_FAIL;
						}
					}
					if (p_slave_clock_info->sem) {
						osif_sem_give(p_slave_clock_info->sem);
					}
				}
			}
#endif
#endif
		}
		break;
#endif
#if defined(VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT) && VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT
		case VENDOR_CMD_FREE_RUN_CLOCK_LATCH_OPCODE: {
#if defined(RTK_BT_HC_CLOCK_OFFSET_SUPPORT) && RTK_BT_HC_CLOCK_OFFSET_SUPPORT
			rtk_bt_vendor_free_run_clock_t *p_hc_free_run_clock = rtk_bt_get_hc_free_run_clock();
			if (p_hc_free_run_clock && p_hc_free_run_clock->enable) {
				if (cmd_rsp->param_len >= 1 && SUB_CMD_FREE_RUN_CLOCK_LATCH_ENABLE == cmd_rsp->param[0]) {
					if (cmd_rsp->cause) {
						BT_LOGE("[%s] Free run clock latch enable fail, cause:0x%x.\r\n", __func__, cmd_rsp->cause);
						osif_sem_give(p_hc_free_run_clock->sem);
					} else {
						if (p_hc_free_run_clock->index == 0) {        //Enable free run clock FW latch function
							BT_LOGD("Free run clock latch %d\r\n", p_hc_free_run_clock->index);
							if (le_vendor_free_run_clock_latch() != GAP_CAUSE_SUCCESS) {
								BT_LOGE("[%s] le_vendor_free_run_clock_latch fail\r\n", __func__);
								osif_sem_give(p_hc_free_run_clock->sem);
							}
						} else if (p_hc_free_run_clock->index == 2) { //Disable free run clock FW latch function
							osif_sem_give(p_hc_free_run_clock->sem);
						} else {
							BT_LOGE("[%s] Free run clock latch flow error\r\n", __func__);
							osif_sem_give(p_hc_free_run_clock->sem);
						}
					}
				} else if (cmd_rsp->param_len >= 5 && SUB_CMD_FREE_RUN_CLOCK_LATCH == cmd_rsp->param[0]) {
					if (cmd_rsp->cause) {
						BT_LOGE("[%s] Free run clock latch fail, cause:0x%x.\r\n", __func__, cmd_rsp->cause);
						osif_sem_give(p_hc_free_run_clock->sem);
					} else {
						BT_LOGD("Get controller free run clock\r\n");
						memcpy(&p_hc_free_run_clock->controller_free_run_clock[p_hc_free_run_clock->index], &cmd_rsp->param[1], sizeof(uint32_t));
						if (p_hc_free_run_clock->index < 2) {
							p_hc_free_run_clock->index++;
							BT_LOGD("Free run clock latch %d\r\n", p_hc_free_run_clock->index);
							if (le_vendor_free_run_clock_latch() != GAP_CAUSE_SUCCESS) {
								BT_LOGE("[%s] le_vendor_free_run_clock_latch fail\r\n", __func__);
								osif_sem_give(p_hc_free_run_clock->sem);
							}
						} else if (p_hc_free_run_clock->index == 2) {
							BT_LOGD("Free run clock latch disable\r\n");
							if (le_vendor_free_run_clock_latch_enable(0) != GAP_CAUSE_SUCCESS) {
								BT_LOGE("[%s] le_vendor_free_run_clock_latch_enable fail\r\n", __func__);
								osif_sem_give(p_hc_free_run_clock->sem);
							}
						} else {
							BT_LOGE("[%s] Free run clock latch flow error\r\n", __func__);
							osif_sem_give(p_hc_free_run_clock->sem);
						}
					}
				} else {
					BT_LOGE("[%s] Free run clock latch response error\r\n", __func__);
					osif_sem_give(p_hc_free_run_clock->sem);
				}
			}
#endif
		}
		break;
#endif
#if defined(VENDOR_CMD_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT) && VENDOR_CMD_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT
		case VENDOR_CMD_GET_LE_ISO_SYNC_REF_AP_INFO_OPCODE: {
#if defined(RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT) && RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT
			rtk_bt_vendor_sync_ref_ap_info_t *p_ap_info = rtk_bt_get_ref_ap_info();
			BT_LOGD("Got AP Info Event \r\n");
			if (p_ap_info && p_ap_info->enable && (cmd_rsp->param_len == sizeof(rtk_bt_le_iso_sync_ref_ap_info_t))) {
				memcpy((void *)&p_ap_info->info, (void *)&cmd_rsp->param[0], sizeof(rtk_bt_le_iso_sync_ref_ap_info_t));
				if (p_ap_info->info.Status) {
					BT_LOGE("[%s] Get le iso sync ref ap info fail, cause:0x%x.\r\n", __func__, p_ap_info->info.Status);
				} else {
					BT_LOGD("[%s] Get le iso sync ref ap info \r\n", __func__);
				}
				osif_sem_give(p_ap_info->sem);
			} else {
				BT_LOGE("[%s] p_ap_info is Null of info enable is false or param len %d is not matched.\r\n", __func__, cmd_rsp->param_len);
				osif_sem_give(p_ap_info->sem);
			}
#endif
		}
		break;
#endif
		default:
			(void)cmd_rsp;
			break;
		}
		break;

	case GAP_MSG_VENDOR_EVT_INFO: {
		uint8_t vendor_evt_code;
		rtk_bt_evt_t *p_evt = NULL;

		if (!evt_info->param_len) {
			break;
		}
		vendor_evt_code = evt_info->param[0];
		BT_LOGA("[bt_vendor_cb] evt info, param_len: %d, evt_code: 0x%x\r\n", evt_info->param_len, vendor_evt_code);

		if (47 == evt_info->param_len && VENDOR_EVT_FW_EXCEPTION == vendor_evt_code) {
			p_evt = rtk_bt_event_create(RTK_BT_COMMON_GP_GAP, RTK_BT_GAP_EVT_FW_EXCEPTION_IND, 0);
			rtk_bt_evt_indicate(p_evt, NULL);
		}

		break;
	}

	default:
		break;
	}

	return;
}
