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

	if (gap_vendor_cmd_req(VENDOR_CMD_ONE_SHOT_ADV_OPCODE, len, param) == GAP_CAUSE_SUCCESS) {
		return GAP_CAUSE_SUCCESS;
	}
	return GAP_CAUSE_SEND_REQ_FAILED;
}
#endif
#endif

void bt_stack_vendor_callback(uint8_t cb_type, void *p_cb_data)
{
	T_GAP_VENDOR_CB_DATA cb_data;
	memcpy(&cb_data, p_cb_data, sizeof(T_GAP_VENDOR_CB_DATA));
	T_GAP_VENDOR_CMD_RSP *cmd_rsp = cb_data.p_gap_vendor_cmd_rsp;
	T_GAP_VENDOR_EVT_INFO *evt_info = cb_data.p_gap_vendor_evt_info;

	switch (cb_type) {
	case GAP_MSG_VENDOR_CMD_RSP:
		// printf("[bt_vendor_cb] cmd rsp, command: 0x%x, cause: 0x%x, "
		// 	   "is_cmpl_evt: %d, param_len: %d\r\n", cmd_rsp->command, cmd_rsp->cause,
		// 	   cmd_rsp->is_cmpl_evt, cmd_rsp->param_len);
		switch (cb_data.p_gap_vendor_cmd_rsp->command) {
#if defined(VENDOR_CMD_READ_BT_REGISTER_SUPPORT) && VENDOR_CMD_READ_BT_REGISTER_SUPPORT
		case VENDOR_CMD_READ_BT_REGISTER_OPCODE: {
				uint16_t bt_reg = 0;
				uint16_t buf = 0;
				uint8_t *pdata = &(cmd_rsp->param[0]);
				buf = *(pdata + 1);
				bt_reg = *pdata;
				bt_reg |= (buf << 8);
				printf("[bt_vendor_cb] bt register is 0x%04x \r\n", bt_reg);
			}
			break;
#endif
#if defined(VENDOR_CMD_ONE_SHOT_ADV_SUPPORT) && VENDOR_CMD_ONE_SHOT_ADV_SUPPORT
		case VENDOR_CMD_ONE_SHOT_ADV_OPCODE: {
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
				if (rtk_bt_mesh_is_enable()) {
					APP_PRINT_INFO1("One shot adv resp: cause 0x%x", cmd_rsp->cause);
					gap_sched_adv_done(GAP_SCHED_ADV_END_TYPE_SUCCESS);
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

	case GAP_MSG_VENDOR_EVT_INFO:
		printf("[bt_vendor_cb] evt info, param_len: %d\r\n", evt_info->param_len);
		break;

	default:
		break;
	}

	return;
}
