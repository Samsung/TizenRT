/**
*****************************************************************************************
*     Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bt_ipc_profile_config.h
  * @brief    bt ipc profile enum.
  * @details  
  * @author   sherman
  * @date     2021-10-20
  * @version  v1.0
  * *************************************************************************************
  */

#ifndef _BT_IPC_PROFILE_CONFIG_H_
#define _BT_IPC_PROFILE_CONFIG_H_

/* ------------------------------- Data Types ------------------------------- */

enum BT_IPC_PROFILE {
	RTK_BT_IPC_MAIN = 0,
	RTK_BT_IPC_A2DP_SINK,
	RTK_BT_IPC_GAP_BLE,
	RTK_BT_IPC_GAP_LEGACY,
	RTK_BT_IPC_SDP,
	RTK_BT_IPC_AVRCP,
	RTK_BT_IPC_A2DP,
	RTK_BT_IPC_GATT,
	RTK_BT_IPC_GATTC,
	RTK_BT_IPC_GATTS,
	RTK_BT_IPC_POWER_STATE,
	RTK_BT_IPC_VENDOR,
	RTK_BT_IPC_MP_API,
	RTK_BT_IPC_CUSTOMER,
};
#endif /* _BT_IPC_PROFILE_CONFIG_H_ */