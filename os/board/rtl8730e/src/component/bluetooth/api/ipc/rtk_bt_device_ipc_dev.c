/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <bt_ipc_host_api.h>
#include <bt_api_config.h>
#include <rtk_bt_ipc_dev_internal.h>

extern bool rtk_bt_pre_enable(void);

void bt_device_ipc_hdl(bt_ipc_host_request_message *p_ipc_msg)
{
	uint16_t ret = 0;
	switch (p_ipc_msg->API_ID) {
	case RTK_BT_DEVICE_IPC_ACT_BT_PRE_ENABLE:
		p_ipc_msg->ret[0] = rtk_bt_pre_enable();
		break;

	case RTK_BT_DEVICE_IPC_ACT_BT_ENABLE:
		ret = rtk_bt_enable((void *)&p_ipc_msg->param_buf[0]);
		memset(p_ipc_msg->ret, 0, sizeof(p_ipc_msg->ret));		
#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
		rtk_bt_device_le_audio_return_value(p_ipc_msg);
#endif
		p_ipc_msg->ret[0] = ret;
		break;

	case RTK_BT_DEVICE_IPC_ACT_BT_DISABLE:
		p_ipc_msg->ret[0] = rtk_bt_disable();
		break;

	default:
		break;
	}
	DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
}


