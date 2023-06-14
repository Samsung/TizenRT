/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <rtk_bt_def.h>
#include <rtk_bt_vendor.h>
#include <bt_ipc_host_api.h>

void bt_vendor_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	switch (p_ipc_msg->API_ID) {
	case RTK_BT_ACT_IPC_SET_TX_POWER_GAIN: {
			uint32_t index = 0;
			memcpy((void *)&index, (void *)&p_ipc_msg->param_buf[0], sizeof(uint32_t));
			rtk_bt_set_bt_tx_power_gain_index(index);
		}
		break;

	case RTK_BT_ACT_IPC_SET_BT_ANTENNA: {
			rtk_bt_set_bt_antenna(p_ipc_msg->param_buf[0]);
		}
		break;

	case RTK_BT_ACT_IPC_SET_HCI_DEBUG_ENABLE: {
			rtk_bt_hci_debug_enable();
		}
		break;

	default:
		break;
	}
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}


