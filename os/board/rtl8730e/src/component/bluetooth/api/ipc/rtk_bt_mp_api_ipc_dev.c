/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <rtk_bt_def.h>
#include <rtk_bt_mp_api.h>
#include <bt_ipc_host_api.h>

void bt_mp_api_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	switch (p_ipc_msg->API_ID) {
	case RTK_BT_MP_POWER_ON: {
			rtk_bt_mp_power_on();
		}
		break;

	case RTK_BT_MP_POWER_OFF: {
			rtk_bt_mp_power_off();
		}
		break;

	default:
		break;
	}
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}


