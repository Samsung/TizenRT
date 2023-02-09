/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef _BT_IPC_PROFILE_CONFIG_H_
#define _BT_IPC_PROFILE_CONFIG_H_

/* ------------------------------- Data Types ------------------------------- */

enum BT_IPC_PROFILE {
	RTK_BT_IPC_DEVICE = 0,
	RTK_BT_IPC_COMMON,
	RTK_BT_IPC_GAP,
	RTK_BT_IPC_GAP_BLE,
	RTK_BT_IPC_GATTS,
	RTK_BT_IPC_GATTC,
	RTK_BT_IPC_GAP_BR,
	RTK_BT_IPC_AVRCP,
	RTK_BT_IPC_A2DP,
	RTK_BT_IPC_SPP,
	RTK_BT_IPC_HFP,
	RTK_BT_IPC_SDP,
	RTK_BT_IPC_POWER_STATE,
	RTK_BT_IPC_VENDOR,
	RTK_BT_IPC_MP_API,
	RTK_BT_IPC_CUSTOMER,
	RTK_BT_IPC_LE_ISO,
	RTK_BT_IPC_LE_AUDIO
};

enum BT_IPC_DEVICE_ACT {
	RTK_BT_DEVICE_IPC_ACT_BT_PRE_ENABLE = 1,
	RTK_BT_DEVICE_IPC_ACT_BT_ENABLE,
	RTK_BT_DEVICE_IPC_ACT_BT_DISABLE
};

enum BT_IPC_COMMON_ACT {
	RTK_BT_ACT_IPC_REGISTER_CB = 1,
	RTK_BT_ACT_IPC_UNREGISTER_CB,
	RTK_BT_ACT_IPC_ENABLE_DIRECT_CALL,
	RTK_BT_ACT_IPC_GET_HEAP_SIZE
};

enum BT_IPC_VENDOR_ACT {
	RTK_BT_ACT_IPC_SET_TX_POWER_GAIN = 1,
	RTK_BT_ACT_IPC_SET_BT_ANTENNA,
	RTK_BT_ACT_IPC_SET_HCI_DEBUG_ENABLE
};

enum BT_IPC_MP_ACT {
	RTK_BT_MP_POWER_ON = 1,
	RTK_BT_MP_POWER_OFF
};

#endif /* _BT_IPC_PROFILE_CONFIG_H_ */