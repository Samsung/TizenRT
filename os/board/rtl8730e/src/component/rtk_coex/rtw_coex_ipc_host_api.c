/**
  ******************************************************************************
  * @file    rtw_coex_ipc_host_api.c
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "rtw_coex_ipc_def.h"
#include "rtw_coex_ipc_cfg.h"
#include "rtw_coex_ipc.h"
#include "rtw_coex_host_api.h"
#include "wifi_intf_drv_to_upper.h"

/* -------------------------------- Defines --------------------------------- */
/*host api task*/
#define CONFIG_COEX_IPC_HOST_API_PRIO 3
#define COEX_STACK_SIZE_IPC_HST_API (COEX_IPC_HST_API_BASIC_SIZE + 128 + CONTEXT_SAVE_SIZE)

/* ---------------------------- Global Variables ---------------------------- */
rtos_sema_t  g_host_coex_ipc_api_task_wake_sema = NULL;
rtos_sema_t  g_host_coex_ipc_api_message_send_sema = NULL;

//todo:move to non-cache data section
struct coex_ipc_host_req_msg g_host_coex_ipc_api_request_info __attribute__((aligned(64)));
u32	last_api_id = 0;  /*for debug*/
#ifdef COEX_IPC_DIR_MSG_TX
IPC_MSG_STRUCT g_host_coex_ipc_api_msg __attribute__((aligned(64)));
#endif

/* -------------------------- Function declaration -------------------------- */
extern int rtk_coex_ipc_c2h_info_indicate(u16 type, u8 *pdata, u16 data_len);

/* ---------------------------- Public Functions ---------------------------- */

__weak int rtk_coex_ipc_h2c_info_handler(u16 type, u8 *pdata, u16 data_len)
{
	int ret = 0;
	u32 param_buf[3];
	u8 *data_temp = NULL;

	if (pdata == NULL || data_len == 0) {
		param_buf[0] = (u32)type;
		param_buf[1] = 0;
		param_buf[2] = 0;
		ret = coex_ipc_api_host_message_send(IPC_API_ID_COEX_H2C_INFO, param_buf, 3);
		return ret;
	}

	data_temp = (u8 *)rtos_mem_zmalloc(data_len);
	if (data_temp == NULL) {
		return -1;
	}
	memcpy(data_temp, pdata, data_len);

	param_buf[0] = (u32)type;
	param_buf[1] = (u32)data_len;
	param_buf[2] = (u32)data_temp;

	DCache_Clean((u32)data_temp, data_len);
	ret = coex_ipc_api_host_message_send(IPC_API_ID_COEX_H2C_INFO, param_buf, 3);
	DCache_Invalidate((u32)data_temp, data_len);
	memcpy(pdata, data_temp, data_len);

	rtos_mem_free(data_temp);

	return ret;
}


/**
 * @brief  process the ipc message.
 * @param  none.
 * @return none.
 */
void coex_ipc_api_host_task(void)
{
	struct coex_ipc_dev_req_msg *p_ipc_msg = NULL;
	int ret = 0;

	do {
		rtos_sema_take(g_host_coex_ipc_api_task_wake_sema, 0xFFFFFFFF);

#ifdef COEX_IPC_DIR_MSG_RX
		PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(COEX_IPC_DIR_MSG_RX, \
										 IPC_D2H_COEX_API_TRAN);
		p_ipc_msg = (struct coex_ipc_dev_req_msg *)p_ipc_recv_msg->msg;
#else
		p_ipc_msg = (struct coex_ipc_dev_req_msg *)ipc_get_message(IPC_D2H_COEX_API_TRAN);
#endif /* COEX_IPC_DIR_MSG_RX */

		DCache_Invalidate((u32)p_ipc_msg, sizeof(struct coex_ipc_dev_req_msg));

		if (p_ipc_msg == NULL) {
			RTK_LOGS(TAG_WLAN_COEX, RTK_LOG_ERROR, "DEV IPC API msg NULL\n");
			continue;
		}

		switch (p_ipc_msg->enevt_id) {
		case IPC_API_ID_COEX_C2H_INFO: {
			u16 type = (u16)p_ipc_msg->param_buf[0];
			u16 data_len = (u16)p_ipc_msg->param_buf[1];
			u8 *pdata = (u8 *)p_ipc_msg->param_buf[2];

			DCache_Invalidate((u32)pdata, data_len);
			ret = rtk_coex_ipc_c2h_info_indicate(type, pdata, data_len);
			DCache_Clean((u32)pdata, data_len);
			p_ipc_msg->ret = ret;
			break;
		}
		default:
			RTK_LOGS(TAG_WLAN_COEX, RTK_LOG_ERROR, "Host API Unknown evt(%x)\n", p_ipc_msg->enevt_id);
			break;
		}
		/*set EVENT_ID to 0 to notify NP that event is finished*/
		p_ipc_msg->enevt_id = 0;
		DCache_Clean((u32)p_ipc_msg, sizeof(struct coex_ipc_dev_req_msg));
	} while (1);
	rtos_task_delete(NULL);
}

/**
 * @brief  to haddle the ipc message interrupt, wakeup event task to process.
 * @param  Data[inout]: IPC data.
 * @param  IrqStatus[in]: interrupt status.
 * @param  ChanNum[in]: IPC channel number.
 * @return none.
 */
void coex_ipc_api_host_int_hdl(void *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	/* wakeup task */
	rtos_sema_give(g_host_coex_ipc_api_task_wake_sema);
}

/**
 * @brief  to send a ipc message to device and wait result.
 * @param  ID[in]: api_id.
 * @param  param_buf[in]: pointer to API parameter.
 * @return result of API.
 */
int coex_ipc_api_host_message_send(u32 id, u32 *param_buf, u32 buf_len)
{
	int ret = 0;
	last_api_id = id;

	rtos_sema_take(g_host_coex_ipc_api_message_send_sema, 0xFFFFFFFF);
	int cnt = 0;

	while (1) {
		DCache_Invalidate((u32)&g_host_coex_ipc_api_request_info, sizeof(struct coex_ipc_host_req_msg));
		if (g_host_coex_ipc_api_request_info.api_id != IPC_API_PROCESS_DONE) {
			rtos_time_delay_ms(1);
			/*When blocking scan is invoked in BT COEXIST, the scan time may increases due to TDMA scan, up to 8.96s (5G) +2.17s (2.4G)*/
			cnt = (cnt + 1) % 12000;
		} else {
			break;
		}
		if (cnt == 0) {
			RTK_LOGS(TAG_WLAN_COEX, RTK_LOG_ERROR, "last coex ipc not hdl \n");
		}
	}

	memset(&g_host_coex_ipc_api_request_info, 0, sizeof(struct coex_ipc_host_req_msg));

	g_host_coex_ipc_api_request_info.api_id = id;
	if (param_buf != NULL) {
		memcpy(g_host_coex_ipc_api_request_info.param_buf, param_buf, buf_len * sizeof(u32));
	}
	DCache_Clean((u32)&g_host_coex_ipc_api_request_info, sizeof(struct coex_ipc_host_req_msg));

#ifdef COEX_IPC_DIR_MSG_TX
	memset(&g_host_coex_ipc_api_msg, 0, sizeof(IPC_MSG_STRUCT));
	g_host_coex_ipc_api_msg.msg = (u32)&g_host_coex_ipc_api_request_info;
	g_host_coex_ipc_api_msg.msg_type = IPC_USER_POINT;
	g_host_coex_ipc_api_msg.msg_len = sizeof(struct coex_ipc_host_req_msg);
	DCache_Clean((u32)&g_host_coex_ipc_api_msg, sizeof(IPC_MSG_STRUCT));

	ipc_send_message(COEX_IPC_DIR_MSG_TX, IPC_H2D_COEX_API_TRAN, \
					 &g_host_coex_ipc_api_msg);
#else
	ipc_send_message(IPC_H2D_COEX_API_TRAN, &g_host_coex_ipc_api_request_info);
#endif /* COEX_IPC_DIR_MSG_TX */

	while (1) {
		if (g_host_coex_ipc_api_request_info.api_id != IPC_API_PROCESS_DONE) {
			rtos_time_delay_ms(1);
			DCache_Invalidate((u32)&g_host_coex_ipc_api_request_info, sizeof(struct coex_ipc_host_req_msg));
			/*When blocking scan is invoked in BT COEXIST, the scan time may increases due to TDMA scan, up to 8.96s (5G) +2.17s (2.4G)*/
			cnt = (cnt + 1) % 12000;
		} else {
			break;
		}
		if (cnt == 0) {
			RTK_LOGS(TAG_WLAN_COEX, RTK_LOG_ALWAYS, "HstMsgSend wait coex ipc done 0x%x, 0x%x\n", g_host_coex_ipc_api_request_info.api_id, last_api_id);
		}
	}
	ret = g_host_coex_ipc_api_request_info.ret;
	rtos_sema_give(g_host_coex_ipc_api_message_send_sema);
	return ret;
}

/**
 * @brief  to initialize the ipc host for coex api.
 * @param  none.
 * @return none.
 */
void coex_ipc_api_init_host(void)
{
	/* initialize the semaphores */
	rtos_sema_create_static(&g_host_coex_ipc_api_task_wake_sema, 0, 0xFFFFFFFF);
	rtos_sema_create_static(&g_host_coex_ipc_api_message_send_sema, 0, 0xFFFFFFFF);
	rtos_sema_give(g_host_coex_ipc_api_message_send_sema);

	/* Initialize the event task */
	if (RTK_SUCCESS != rtos_task_create(NULL, (const char *const)"coex_ipc_api_host_task", (rtos_task_function_t)coex_ipc_api_host_task, NULL,
										COEX_STACK_SIZE_IPC_HST_API, CONFIG_COEX_IPC_HOST_API_PRIO)) {
		RTK_LOGS(TAG_WLAN_COEX, RTK_LOG_ERROR, "Create coex_ipc_api_host_task Err\n");
	}

}

/**
 * @brief  to initialize the ipc host for the coex.
 * @param  none.
 * @return none.
 */
void coex_ipc_host_init(void)
{
	coex_ipc_api_init_host();
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_coex_api_host_table = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = coex_ipc_api_host_int_hdl,
	.RxIrqData = (void *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (void *) NULL,
	.IPC_Direction = COEX_IPC_DIR_MSG_RX,
	.IPC_Channel = IPC_D2H_COEX_API_TRAN
};

/* ---------------------------- Global Variables ---------------------------- */
