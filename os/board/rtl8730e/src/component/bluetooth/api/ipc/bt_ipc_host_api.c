/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- */
#include "bt_ipc_host_api.h"
#include "bt_ipc_dev_api.h"
#include "bt_ipc_debug.h"
#include "bt_ipc_profile_config.h"
#include "platform_opts.h"
#include "ameba_soc.h"
#include "osif.h"
#include "platform_opts_bt.h"
#include "rtk_bt_le_gap.h"
#include "rtk_bt_gatts.h"
#include "rtk_bt_gattc.h"
#include "bt_ipc_cfg.h"
#include "rtk_bt_common.h"

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_BT_IPC_HOST_API_PRIO 4
#define BT_IPC_HOST_API_STACK_SIZE 2048

/* ---------------------------- Global Variables ---------------------------- */
struct bt_ipc_host_api_task_struct	             btIpcHostApiTask = {0};
void                           *bt_host_api_message_send_sema = NULL;

bt_ipc_host_request_message bt_host_ipc_api_request_info __attribute__((aligned(64)));
IPC_MSG_STRUCT bt_host_ipc_api_msg __attribute__((aligned(64)));
extern rtk_bt_evt_t bt_ipc_evt;
extern uint8_t ipc_msg_count;
extern uint8_t g_evt_task_running;

/**
 * @brief  to haddle the ipc message interrupt, wakeup event task to process.
 * @param  Data[inout]: IPC data.
 * @param  IrqStatus[in]: interrupt status.
 * @param  ChanNum[in]: IPC channel number.
 * @return none.
 */
void bt_ipc_host_api_hdl(void *Data, uint32_t IrqStatus, uint32_t ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;
	bt_ipc_dev_request_message *p_ipc_msg = NULL;
	PIPC_MSG_STRUCT p_ipc_recv_msg = NULL;

	if (bt_ipc_evt.data) {
		printf("%s: bt_ipc_evt.data is not NULL ! \r\n", __func__);
		return;
	}
	p_ipc_recv_msg = ipc_get_message(BT_IPC_DIR_EVENT_RX, \
							BT_IPC_D2H_API_TRAN);
	p_ipc_msg = (bt_ipc_dev_request_message *)p_ipc_recv_msg->msg;
	DCache_Invalidate((uint32_t)p_ipc_msg, sizeof(bt_ipc_dev_request_message));
	if (p_ipc_msg == NULL) {
		printf("%s: Device IPC API message is NULL, invalid! \r\n", __func__);
		return;
	}
	/* if task is running, wakeup */
	if (g_evt_task_running) {
		memset((void *)&bt_ipc_evt,0,sizeof(rtk_bt_evt_t));
		bt_ipc_evt.group = RTK_BT_LE_GP_MAX;
		bt_ipc_evt.evt = RTK_BT_LE_GAP_EVT_MAX;
		bt_ipc_evt.data = (void *)p_ipc_msg;
		/* wakeup task */
		if (rtk_bt_evt_indicate(&bt_ipc_evt, NULL)) {
			printf("%s: indicate evt fail \r\n", __func__);
		} else {
			ipc_msg_count ++;
		}
	} else {
		/* not running return fail */
		printf("%s: task is not running! \r\n", __func__);
		p_ipc_msg->ret[0] = 1;
		p_ipc_msg->EVENT_ID = 0xFFFFFFFF;
		DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
		bt_ipc_evt.data = NULL;
	}
}

/**
 * @brief  to send a ipc message to device and wait result.
 * @param  profile_index[in]: mesh/peripheral/central ...
 * @param  api_id[in]: BT_API_ID.
 * @param  param_buf[in]: pointer to API parameter.
 * @return pointer to result of API.
 */
int *bt_ipc_api_host_message_send(uint32_t profile_id, uint32_t api_id, uint8_t *param_buf, uint32_t buf_len)
{
	int *ret = NULL;

	osif_sem_take(bt_host_api_message_send_sema, 0xFFFFFFFF);
	memset((void *)&bt_host_ipc_api_request_info, 0, sizeof(bt_ipc_host_request_message));

	bt_host_ipc_api_request_info.PROFILE_ID = profile_id;
	bt_host_ipc_api_request_info.API_ID = api_id;
	if (param_buf != NULL) {
		memcpy((void *)bt_host_ipc_api_request_info.param_buf, (void *)param_buf, buf_len);
	}
	bt_host_ipc_api_request_info.buf_size = buf_len;
	DCache_Clean((uint32_t)&bt_host_ipc_api_request_info, sizeof(bt_ipc_host_request_message));

	memset((void *)&bt_host_ipc_api_msg, 0, sizeof(IPC_MSG_STRUCT));
	bt_host_ipc_api_msg.msg = (uint32_t)&bt_host_ipc_api_request_info;
	bt_host_ipc_api_msg.msg_type = IPC_USER_POINT;
	bt_host_ipc_api_msg.msg_len = sizeof(bt_ipc_host_request_message);
	DCache_Clean((uint32_t)&bt_host_ipc_api_msg, sizeof(IPC_MSG_STRUCT));

	ipc_send_message(BT_IPC_DIR_API_TX, BT_IPC_H2D_API_TRAN, \
					&bt_host_ipc_api_msg);

	while (1) {
		/* waiting until dev have handled the ipc message */
		if (bt_host_ipc_api_request_info.API_ID != 0xFFFFFFFF) {
			osif_delay(1);
			DCache_Invalidate((uint32_t)&bt_host_ipc_api_request_info, sizeof(bt_ipc_host_request_message));
		} else {
			break;
		}
	}

	ret = (int *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_host_ipc_api_request_info.ret));
	if (ret == NULL) {
		DBG_BT_IPC("%s: allocate mem for ret fail \r\n", __func__);
		return NULL;
	}
	memset((void *)ret, 0, sizeof(bt_host_ipc_api_request_info.ret));
	memcpy((void *)ret, (void *)&bt_host_ipc_api_request_info.ret, sizeof(bt_host_ipc_api_request_info.ret));

	osif_sem_give(bt_host_api_message_send_sema);
	return ret;
}

/**
 * @brief  to initialize the ipc host for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_init_host(void)
{
	osif_sem_create(&bt_host_api_message_send_sema, 0, 0xffffffff);
	osif_sem_give(bt_host_api_message_send_sema);
}

/**
 * @brief  to deinit the ipc host for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_deinit_host(void)
{
	osif_sem_delete(bt_host_api_message_send_sema);
}

#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   bt_ipc_host_api_table[] = {
	{IPC_USER_POINT, 	bt_ipc_host_api_hdl,	(VOID *) NULL, BT_IPC_DIR_EVENT_RX, BT_IPC_D2H_API_TRAN, IPC_RX_FULL},
};
#endif
