/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- */
#include <platform_opts_bt.h>
#include <rtk_stack_config.h>
#include <bt_ipc_dev_api.h>
#include <bt_ipc_host_api.h>
#include <bt_ipc_debug.h>
#include <bt_ipc_profile_config.h>
#include <platform_opts.h>
#include <ameba_soc.h>
#include <osif.h>
#include <bt_ipc_cfg.h>
#include <rtk_bt_ipc_dev_internal.h>

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_BT_IPC_DEV_API_PRIO 4
#define BT_IPC_DEV_API_STACK_SIZE 2048

/* ---------------------------- Global Variables ---------------------------- */
struct bt_ipc_dev_api_task_struct btIpcDevApiTask = {0};
void *bt_dev_api_message_send_sema = NULL;

bt_ipc_dev_request_message bt_dev_ipc_api_request_info __attribute__((aligned(64)));
IPC_MSG_STRUCT bt_dev_ipc_api_msg __attribute__((aligned(64)));

static bt_ipc_host_request_message *p_ipc_msg = NULL;
static uint8_t ipc_msg_count = 0;
/**
 * @brief  process the ipc message.
 * @param  none.
 * @return none.
 */
void bt_ipc_dev_api_task(void *context)
{
	(void)context;
	// uint8_t ret;
	struct bt_ipc_dev_api_task_struct *pcmdtask = &(btIpcDevApiTask);
	bt_ipc_host_request_message *p_ipc_msg_temp = NULL;

	DBG_BT_IPC("%s: Enter BT_IPC_DEV_API_TASK\r\n");
	pcmdtask->task_is_running = 1;
	while (1) {
		if (osif_sem_take(pcmdtask->wakeup_sema, 0xFFFFFFFF) == _FAIL) {
			DBG_BT_IPC("%s: download sema fail \r\n", __func__);
			break;
		}
		if (pcmdtask->blocked) {
			pcmdtask->task_is_running = 0;
			DBG_BT_IPC("%s: blocked(%d) break at line %d\n",
					   __func__, (int)pcmdtask->blocked, __LINE__);
			break;
		}
		if (!p_ipc_msg) {
			printf("%s: p_ipc_msg is NULL ! \r\n", __func__);
			continue;
		}
		switch (p_ipc_msg->PROFILE_ID) {
		case RTK_BT_IPC_DEVICE:
			bt_device_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_COMMON:
			bt_common_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_POWER_STATE:
			bt_power_control_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_VENDOR:
			bt_vendor_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_MP_API:
			bt_mp_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_GAP:
			bt_gap_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_GAP_BLE:
			bt_le_gap_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_GATTS:
			bt_gatts_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_GATTC:
			bt_gattc_api_ipc_hdl(p_ipc_msg);
			break;

#if defined(RKT_BLE_MESH_SUPPORT) && RKT_BLE_MESH_SUPPORT
		case RTK_BT_IPC_MESH_STACK:
			bt_le_mesh_stack_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_CONFIG_CLIENT_MODEL:
			bt_le_mesh_config_client_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_GENERIC_ONOFF_CLIENT_MODEL:
			bt_le_mesh_generic_onoff_client_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_DATATRANS_MODEL:
			bt_le_mesh_datatrans_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_LIGHT_LIGHTNESS_CLIENT_MODEL:
			bt_le_mesh_light_lightness_client_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_LIGHT_CTL_CLIENT_MODEL:
			bt_le_mesh_light_ctl_client_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_LIGHT_HSL_CLIENT_MODEL:
			bt_le_mesh_light_hsl_client_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_LIGHT_XYL_CLIENT_MODEL:
			bt_le_mesh_light_xyl_client_model_ipc_hdl(p_ipc_msg);
			break;
		case RTK_BT_IPC_MESH_LIGHT_LC_CLIENT_MODEL:
			bt_le_mesh_light_lc_client_model_ipc_hdl(p_ipc_msg);
			break;	
#endif

		case RTK_BT_IPC_GAP_BR:
			bt_br_gap_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_AVRCP:
			bt_avrcp_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_A2DP:
			bt_a2dp_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_SPP:
			bt_spp_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_HFP:
			bt_hfp_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_SDP:
			bt_sdp_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_LE_ISO:
			bt_le_iso_api_ipc_hdl(p_ipc_msg);
			break;

		case RTK_BT_IPC_LE_AUDIO:
			bt_le_audio_api_ipc_hdl(p_ipc_msg);
			break;

		default: {
			DBG_BT_IPC("%s(): user cmd %d not found !\r\n", __func__, (int)p_ipc_msg->PROFILE_ID);
		}
		break;
		}
		p_ipc_msg_temp = p_ipc_msg;
		p_ipc_msg = NULL;
		/*set API_ID to 0 to notify NP that event is finished*/
		p_ipc_msg_temp->API_ID = 0xFFFFFFFF;
		DCache_Clean((uint32_t)p_ipc_msg_temp, sizeof(bt_ipc_host_request_message));
		ipc_msg_count --;
	}
	while (ipc_msg_count) {
		/* not running return fail */
		printf("%s: task is not running! \r\n", __func__);
		if (p_ipc_msg) {
			p_ipc_msg->ret[0] = 1;
			p_ipc_msg->API_ID = 0xFFFFFFFF;
			DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
			p_ipc_msg = NULL;
		}
		ipc_msg_count --;
	}
	osif_sem_give(pcmdtask->terminate_sema);

	DBG_BT_IPC("%s: bt ipc dev api thread exit !\r\n", __func__);
	osif_task_delete(NULL);
}

/**
 * @brief  to haddle the ipc message interrupt, wakeup event task to process.
 * @param  Data[inout]: IPC data.
 * @param  IrqStatus[in]: interrupt status.
 * @param  ChanNum[in]: IPC channel number.
 * @return none.
 */
void bt_ipc_dev_api_hdl(void *Data, uint32_t IrqStatus, uint32_t ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;
	PIPC_MSG_STRUCT p_ipc_recv_msg = NULL;

	if (p_ipc_msg) {
		printf("%s: p_ipc_msg is not NULL ! \r\n", __func__);
		return;
	}
	p_ipc_recv_msg = ipc_get_message(BT_IPC_DIR_EVENT_RX, \
									 BT_IPC_H2D_API_TRAN);
	p_ipc_msg = (bt_ipc_host_request_message *)p_ipc_recv_msg->msg;
	DCache_Invalidate((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
	if (p_ipc_msg == NULL) {
		printf("%s: Device IPC API message is NULL, invalid! \r\n", __func__);
		return;
	}
	/* if task is running, wakeup */
	if (btIpcDevApiTask.task_is_running) {
		/* wakeup task */
		osif_sem_give(btIpcDevApiTask.wakeup_sema);
		ipc_msg_count ++;
	} else {
		/* not running return fail */
		printf("%s: task is not running! \r\n", __func__);
		p_ipc_msg->ret[0] = 1;
		p_ipc_msg->API_ID = 0xFFFFFFFF;
		DCache_Clean((uint32_t)p_ipc_msg, sizeof(bt_ipc_host_request_message));
		p_ipc_msg = NULL;
	}
}

/**
 * @brief  to send a ipc message to device and wait result.
 * @param  profile_id[in]: mesh/peripheral/central ...
 * @param  event_id[in]: BT_EVENT_ID.
 * @param  param_buf[in]: pointer to API parameter.
 * @param  buf_len[in]: buffer size.
 * @return pointer to result of API.
 */
int *bt_ipc_api_dev_message_send(uint32_t profile_id, uint32_t event_id, uint8_t *param_buf, uint32_t buf_len)
{
	int *ret = NULL;

	osif_sem_take(bt_dev_api_message_send_sema, 0xFFFFFFFF);

	memset((void *)&bt_dev_ipc_api_request_info, 0, sizeof(bt_ipc_dev_request_message));

	bt_dev_ipc_api_request_info.PROFILE_ID = profile_id;
	bt_dev_ipc_api_request_info.EVENT_ID = event_id;
	if (param_buf != NULL) {
		memcpy((void *)bt_dev_ipc_api_request_info.param_buf, (void *)param_buf, buf_len);
	}
	bt_dev_ipc_api_request_info.buf_size = buf_len;
	DCache_Clean((uint32_t)&bt_dev_ipc_api_request_info, sizeof(bt_ipc_dev_request_message));

	memset((void *)&bt_dev_ipc_api_msg, 0, sizeof(IPC_MSG_STRUCT));
	bt_dev_ipc_api_msg.msg = (uint32_t)&bt_dev_ipc_api_request_info;
	bt_dev_ipc_api_msg.msg_type = IPC_USER_POINT;
	bt_dev_ipc_api_msg.msg_len = sizeof(bt_ipc_dev_request_message);
	DCache_Clean((uint32_t)&bt_dev_ipc_api_msg, sizeof(IPC_MSG_STRUCT));
	ipc_send_message(BT_IPC_DIR_API_TX, BT_IPC_D2H_API_TRAN, \
					 &bt_dev_ipc_api_msg);

	while (1) {
		/* waiting until dev have handled the ipc message */
		if (bt_dev_ipc_api_request_info.EVENT_ID != 0xFFFFFFFF) {
			osif_delay(1);
			DCache_Invalidate((uint32_t)&bt_dev_ipc_api_request_info, sizeof(bt_ipc_dev_request_message));
		} else {
			break;
		}
	}

	ret = (int *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bt_dev_ipc_api_request_info.ret));
	if (ret == NULL) {
		DBG_BT_IPC("%s: allocate mem for ret fail \r\n", __func__);
		return NULL;
	}
	memset((void *)ret, 0, sizeof(bt_dev_ipc_api_request_info.ret));
	memcpy((void *)ret, (void *)&bt_dev_ipc_api_request_info.ret, sizeof(bt_dev_ipc_api_request_info.ret));

	osif_sem_give(bt_dev_api_message_send_sema);

	return ret;
}

/**
 * @brief  to initialize the ipc device for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_init_dev(void)
{
	/* initialize task structure */
	memset((void *)&btIpcDevApiTask, 0, sizeof(struct bt_ipc_dev_api_task_struct));
	btIpcDevApiTask.blocked = 0;

	/* initialize the semaphores */
	osif_sem_create(&btIpcDevApiTask.wakeup_sema, 0, 0xffffffff);
	osif_sem_create(&btIpcDevApiTask.terminate_sema, 0, 0xffffffff);
	osif_sem_create(&bt_dev_api_message_send_sema, 0, 0xffffffff);
	osif_sem_give(bt_dev_api_message_send_sema);
	ipc_msg_count = 0;
	p_ipc_msg = NULL;
	/* Initialize the event task */
	if (osif_task_create(&btIpcDevApiTask.task, (const char *const)"bt_ipc_dev_api_task", bt_ipc_dev_api_task,
						 NULL, BT_IPC_DEV_API_STACK_SIZE, CONFIG_BT_IPC_DEV_API_PRIO) != true) {
		btIpcDevApiTask.blocked = 1;
		osif_sem_delete(btIpcDevApiTask.wakeup_sema);
		osif_sem_delete(btIpcDevApiTask.terminate_sema);
		osif_sem_delete(bt_dev_api_message_send_sema);
		DBG_BT_IPC("%s: create bt_ipc_dev_api_task fail !\r\n", __func__);
	}
}

/**
 * @brief  to deinit the ipc device for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_deinit_dev(void)
{
	if (!btIpcDevApiTask.task) {
		DBG_BT_IPC("%s: bt ipc dev hasn't been initialized \r\n", __func__);
		return;
	}
	/* delete the task and free the semaphores */
	btIpcDevApiTask.blocked = 1;
	osif_sem_give(btIpcDevApiTask.wakeup_sema);
	osif_sem_take(btIpcDevApiTask.terminate_sema, 0xFFFFFFFF);
	osif_sem_delete(btIpcDevApiTask.wakeup_sema);
	osif_sem_delete(btIpcDevApiTask.terminate_sema);
	osif_sem_delete(bt_dev_api_message_send_sema);
	ipc_msg_count = 0;
	p_ipc_msg = NULL;
	memset((void *)&btIpcDevApiTask, 0, sizeof(struct bt_ipc_dev_api_task_struct));
}

#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   bt_ipc_dev_api_table[] = {
	{IPC_USER_POINT, 	bt_ipc_dev_api_hdl,	(VOID *) NULL, BT_IPC_DIR_EVENT_RX, BT_IPC_H2D_API_TRAN, IPC_RX_FULL},
};
#endif
