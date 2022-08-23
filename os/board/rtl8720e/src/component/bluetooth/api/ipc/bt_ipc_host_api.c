/**
*****************************************************************************************
*     Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bt_ipc_host_api.c
  * @brief    Source file for bt ipc host api.
  * @details  bt ipc host api.
  * @author   sherman
  * @date     2021-10-20
  * @version  v1.0
  * *************************************************************************************
  */

/* -------------------------------- Includes -------------------------------- */
#include "bt_ipc_host_api.h"
#include "bt_ipc_dev_api.h"
#include "bt_ipc_debug.h"
#include "bt_ipc_profile_config.h"
#include "platform_opts.h"
#include "ameba_soc.h"
#include "platform_opts_bt.h"
#include "bt_ipc_cfg.h"

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_BT_IPC_HOST_API_PRIO 4
#define BT_IPC_HOST_API_STACK_SIZE 1024

/* ---------------------------- Global Variables ---------------------------- */
struct bt_ipc_host_api_task_struct	             btIpcHostApiTask = {0};
void                           *bt_host_api_message_send_sema = NULL;

bt_ipc_host_request_message bt_host_ipc_api_request_info __attribute__((aligned(64)));
IPC_MSG_STRUCT bt_host_ipc_api_msg __attribute__((aligned(64)));
extern void bt_customer_evt_hdl(uint8_t evt_code, uint8_t *param);

/**
 * @brief  process the ipc message.
 * @param  none.
 * @return none.
 */
void bt_ipc_host_api_task(void *context)
{
	(void)context;
	// uint8_t ret;
	struct bt_ipc_host_api_task_struct *pcmdtask = &(btIpcHostApiTask);
	bt_ipc_dev_request_message *p_ipc_msg = NULL;

	DBG_BT_IPC("%s: Enter BT_IPC_HOST_API_TASK\r\n", __func__);
	while (1) {
		if (osif_sem_take(pcmdtask->wakeup_sema, 0xFFFFFFFF) == _FAIL) {
			DBG_BT_IPC("%s: download sema fail \r\n", __func__);
			break;
		}
		if (pcmdtask->blocked) {
			DBG_BT_IPC("%s: blocked(%d) break at line %d\n",
					__func__, (int)pcmdtask->blocked, __LINE__);
			break;
		}
		PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(BT_IPC_DIR_EVENT_RX, \
										BT_IPC_D2H_API_TRAN);
		DCache_Invalidate((uint32_t)p_ipc_recv_msg, sizeof(IPC_MSG_STRUCT));
		p_ipc_msg = (bt_ipc_dev_request_message *)p_ipc_recv_msg->msg;
		DCache_Invalidate((uint32_t)p_ipc_msg, sizeof(bt_ipc_dev_request_message));
		if (p_ipc_msg == NULL) {
			DBG_BT_IPC("%s: Device IPC API message is NULL, invalid! \r\n", __func__);
			continue;
		}

		switch (p_ipc_msg->PROFILE_ID) {
		case RTK_BT_IPC_CUSTOMER: {
			bt_customer_evt_hdl(p_ipc_msg->EVENT_ID, p_ipc_msg->param_buf);
			break;
		}
		
		default: {
			DBG_BT_IPC("%s(): user cmd %d not found !\r\n", __func__, (int)p_ipc_msg->PROFILE_ID);
		}
		break;
		}
		/*set EVENT_ID to 0 to notify NP that event is finished*/
		p_ipc_msg->EVENT_ID = 0;
		DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_dev_request_message));
	}
	PIPC_MSG_STRUCT p_ipc_recv_msg = NULL;
	do {
		p_ipc_recv_msg = ipc_get_message(BT_IPC_DIR_EVENT_RX, \
										BT_IPC_D2H_API_TRAN);
		DCache_Invalidate((uint32_t)p_ipc_recv_msg, sizeof(IPC_MSG_STRUCT));
	} while (!p_ipc_recv_msg);
	osif_sem_give(pcmdtask->terminate_sema);

	DBG_BT_IPC("%s: bt ipc host api thread exit !\r\n", __func__);
	osif_task_delete(NULL);
}

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
	/* wakeup task */
	osif_sem_give(btIpcHostApiTask.wakeup_sema);
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
		if (bt_host_ipc_api_request_info.API_ID != 0) {
			osif_delay(1);
			DCache_Invalidate((uint32_t)&bt_host_ipc_api_request_info, sizeof(bt_ipc_host_request_message));
		} else {
			break;
		}
	}

	ret = (int *)osif_mem_alloc(0, sizeof(bt_host_ipc_api_request_info.ret));
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
	/* initialize task structure */
	memset((void *)&btIpcHostApiTask, 0, sizeof(struct bt_ipc_host_api_task_struct));
	btIpcHostApiTask.blocked = 0;
	
	/* initialize the semaphores */
	osif_sem_create(&btIpcHostApiTask.wakeup_sema, 0, 0xffffffff);
	osif_sem_create(&btIpcHostApiTask.terminate_sema, 0, 0xffffffff);
	osif_sem_create(&bt_host_api_message_send_sema, 0, 0xffffffff);
	osif_sem_give(bt_host_api_message_send_sema);

	/* Initialize the event task */
	if (osif_task_create(&btIpcHostApiTask.task, (const char *const)"bt_ipc_host_api_task", bt_ipc_host_api_task,
					NULL, BT_IPC_HOST_API_STACK_SIZE, CONFIG_BT_IPC_HOST_API_PRIO) != true) {
		btIpcHostApiTask.blocked = 1;
		osif_sem_delete(btIpcHostApiTask.wakeup_sema);
		osif_sem_delete(btIpcHostApiTask.terminate_sema);
		osif_sem_delete(bt_host_api_message_send_sema);
		DBG_BT_IPC("%s: create bt_ipc_host_api_task fail !\r\n", __func__);
	}
}

/**
 * @brief  to deinit the ipc host for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_deinit_host(void)
{
	if (!btIpcHostApiTask.task) {
		DBG_BT_IPC("%s: bt ipc host hasn't been initialized \r\n", __func__);
		return;
	}
	/* delete the task and free the semaphores */
	btIpcHostApiTask.blocked = 1;
	osif_sem_give(btIpcHostApiTask.wakeup_sema);
	osif_sem_take(btIpcHostApiTask.terminate_sema, 0xFFFFFFFF);
	osif_sem_delete(btIpcHostApiTask.wakeup_sema);
	osif_sem_delete(btIpcHostApiTask.terminate_sema);
	osif_sem_delete(bt_host_api_message_send_sema);

	memset((void *)&btIpcHostApiTask, 0, sizeof(struct bt_ipc_host_api_task_struct));
}

#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   bt_ipc_host_api_table[] = {
	{IPC_USER_POINT, 	bt_ipc_host_api_hdl,	(VOID *) NULL, BT_IPC_DIR_EVENT_RX, BT_IPC_D2H_API_TRAN, IPC_RX_FULL},
};
#endif
