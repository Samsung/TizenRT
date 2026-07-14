/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include "platform_autoconf.h"
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_gap.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gattc.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
#include <rtk_bt_mesh_common.h>
#include <rtk_bt_mesh_datatrans_model.h>
#include <rtk_bt_mesh_light_model.h>
#include <rtk_bt_mesh_def.h>
#include <rtk_bt_mesh_generic_onoff_model.h>
#include <rtk_bt_mesh_time_model.h>
#include <rtk_bt_mesh_scheduler_model.h>
#include <rtk_bt_mesh_scene_model.h>
#include <rtk_bt_mesh_generic_default_transition_time.h>
#include <rtk_bt_mesh_generic_model.h>
#include <rtk_bt_mesh_sensor_model.h>
#include <rtk_bt_mesh_health_model.h>
#endif

#define EVENT_NUM 64
/* NOTE: If user's event callback need more stack size, please enlarge it */
#define EVENT_TASK_STACK_SIZE 4096
#define EVENT_TASK_PRIORITY 4

/* Internal definition for which evt callback shall direct calling. */
static uint32_t rtk_bt_le_gap_evt_direct_calling_flag =
	(1 << RTK_BT_LE_GAP_EVT_REMOTE_CONN_UPDATE_REQ_IND);
static uint32_t rtk_bt_le_gatts_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_le_gattc_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_le_iso_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_br_avrcp_evt_direct_calling_flag =
	(1 << RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED);
static uint32_t rtk_bt_br_a2dp_evt_direct_calling_flag =
	(1 << RTK_BT_A2DP_EVT_STREAM_DATA_IND);
static uint32_t rtk_bt_br_hfp_evt_direct_calling_flag =
	((1 << RTK_BT_HFP_EVT_SCO_DATA_IND) |
	 (1 << RTK_BT_HFP_EVT_HF_BATTERY_IND) |
	 (1 << RTK_BT_HFP_EVT_AG_INDICATORS_STATUS_REQ));
static uint32_t rtk_bt_br_pbap_evt_direct_calling_flag = 0;
static uint32_t rtk_bt_le_audio_evt_bap_direct_calling_flag =
	((1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_PA_SYNC_PARAM_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_BASS_GET_BIG_SYNC_PARAM_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_ASCS_ASE_STATE_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_ASCS_GET_PREFER_QOS_IND) |
	 (1 << RTK_BT_LE_AUDIO_EVT_ISO_DATA_RECEIVE_IND));
static uint32_t rtk_bt_le_audio_evt_cap_direct_calling_flag =
	(1 << RTK_BT_LE_AUDIO_EVT_MCP_SERVER_READ_IND);
static uint32_t rtk_bt_gap_evt_direct_calling_flag =
	(1 << RTK_BT_GAP_EVT_ECFC_RECONF_REQ_IND);

#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
static uint32_t rtk_bt_mesh_generic_onoff_server_model_evt_direct_calling_flag =
	(1 << RTK_BT_MESH_GENERIC_ONOFF_SERVER_MODEL_EVT_GET);
static uint32_t rtk_bt_mesh_datatrans_model_evt_direct_calling_flag =
	(1 << RTK_BT_MESH_DATATRANS_MODEL_EVT_SERVER_READ);
#endif

extern uint16_t bt_stack_api_send(void *pcmd);
#if !defined(CONFIG_BT_ZEPHYR) || !CONFIG_BT_ZEPHYR
extern void bt_stack_pending_cmd_delete(rtk_bt_cmd_t *p_cmd);
#endif

static uint32_t api_task_msg_num = 0;
static void *g_evt_task_sem = NULL;
static void *g_evt_queue = NULL;
static void *g_evt_task_hdl = NULL;
bool event_task_running = false;

rtk_bt_evt_cb_t rtk_bt_le_evt_cb_tbl[RTK_BT_LE_GP_MAX - RTK_BT_API_LE_BASE] = {0};
rtk_bt_evt_cb_t rtk_bt_br_evt_cb_tbl[RTK_BT_BR_GP_MAX - RTK_BT_API_BR_BASE] = {0};
rtk_bt_evt_cb_t rtk_bt_evt_cb_tbl[RTK_BT_COMMON_GP_MAX - RTK_BT_API_COMMON_BASE] = {0};

/* ===============================================Common Api ======================================= */
void rtk_bt_le_addr_to_str(void *paddr, char *str, uint32_t len)
{
	rtk_bt_le_addr_t *addr = (rtk_bt_le_addr_t *)paddr;
	char type[10];
	memset(str, 0, len);

	switch (addr->type) {
	case RTK_BT_LE_ADDR_TYPE_PUBLIC:
		strncpy(type, "public", 7);
		break;
	case RTK_BT_LE_ADDR_TYPE_RANDOM:
		strncpy(type, "random", 7);
		break;
	case RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC:
		strncpy(type, "public-id", 10);
		break;
	case RTK_BT_LE_ADDR_TYPE_RPA_RANDOM:
		strncpy(type, "random-id", 10);
		break;
	default:
		snprintf(type, sizeof(type), "0x%02x", addr->type);
		break;
	}
	snprintf(str, len, "%02X:%02X:%02X:%02X:%02X:%02X(%s)",
			 addr->addr_val[5], addr->addr_val[4], addr->addr_val[3],
			 addr->addr_val[2], addr->addr_val[1], addr->addr_val[0], type);
}

void rtk_bt_addr_val_to_str(uint8_t *paddr, char *str, uint32_t len)
{
	memset(str, 0, len);
	snprintf(str, len, "%02X:%02X:%02X:%02X:%02X:%02X",
			 paddr[5], paddr[4], paddr[3],
			 paddr[2], paddr[1], paddr[0]);
}

void rtk_bt_br_addr_to_str(uint8_t *paddr, char *str, uint32_t len)
{
	rtk_bt_addr_val_to_str(paddr, str, len);
}

void rtk_bt_addr_to_str(uint8_t addr_type, uint8_t *paddr, char *str, uint32_t len)
{
	char str_type[20] = {0};
	memset(str, 0, len);

	switch (addr_type) {
	case RTK_BT_ADDR_LE_PUBLIC:
		strncpy(str_type + strlen(str_type), "le public", 10);
		break;
	case RTK_BT_ADDR_LE_RANDOM:
		strncpy(str_type + strlen(str_type), "le random", 10);
		break;
	case RTK_BT_ADDR_LE_RPA_PUBLIC:
		strncpy(str_type + strlen(str_type), "le public-id", 13);
		break;
	case RTK_BT_ADDR_LE_RPA_RANDOM:
		strncpy(str_type + strlen(str_type), "le random-id", 13);
		break;
	case RTK_BT_ADDR_CLASSIC:
		strncpy(str_type + strlen(str_type), "classic", 8);
		break;
	default:
		snprintf(str_type, sizeof(str_type), "0x%02x", addr_type);
		break;
	}
	snprintf(str, len, "%02X:%02X:%02X:%02X:%02X:%02X(%s)",
			 paddr[5], paddr[4], paddr[3],
			 paddr[2], paddr[1], paddr[0], str_type);
}

#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
static void *api_sem_pool_q = NULL;

static struct bt_evt_pool {
	void *pool_q_small;
	uint8_t buf_small[BT_EVT_SMALL_POOL_SIZE][BT_EVT_SMALL_BUF_SIZE];

	void *pool_q_medium;
	uint8_t buf_medium[BT_EVT_MEDIUM_POOL_SIZE][BT_EVT_MEDIUM_BUF_SIZE];

	void *pool_q_large;
	uint8_t buf_large[BT_EVT_LARGE_POOL_SIZE][BT_EVT_LARGE_BUF_SIZE];
} *evt_pool = NULL;

bool bt_api_sem_pool_init(void)
{
	int i;
	void *sem = NULL;

	if (!osif_msg_queue_create(&api_sem_pool_q, BT_API_SEM_POOL_SIZE, sizeof(void *))) {
		return false;
	}

	for (i = 0; i < BT_API_SEM_POOL_SIZE; i++) {
		if (osif_sem_create(&sem, 0, 1)) {
			if (osif_msg_send(api_sem_pool_q, &sem, 0)) {
				continue;
			} else {
				osif_sem_delete(sem);
			}
		}
		goto fail;
	}
	BT_LOGA("BT API memory pre alloc\r\n");
	return true;

fail:
	while (osif_msg_recv(api_sem_pool_q, &sem, 0)) {
		osif_sem_delete(sem);
	}
	osif_msg_queue_delete(api_sem_pool_q);
	api_sem_pool_q = NULL;

	return false;
}

void bt_api_sem_pool_deinit(void)
{
	int i = 0;
	uint32_t msg_num = 0;
	void *sem = NULL;

	if (!api_sem_pool_q) {
		return;
	}

	/* wait all sem being freed(give back to pool) by app */
	while (1) {
		osif_msg_queue_peek(api_sem_pool_q, &msg_num);
		if (BT_API_SEM_POOL_SIZE == msg_num) {
			break;
		}
		osif_delay(5);
		i++;
		if (200 == i) {
			BT_LOGA("!!wait api sem full freed over 1s\r\n");
		}
	}

	while (osif_msg_recv(api_sem_pool_q, &sem, 0)) {
		osif_sem_delete(sem);
	};

	osif_msg_queue_delete(api_sem_pool_q);
	api_sem_pool_q = NULL;
}

bool bt_evt_mem_pool_init(void)
{
	int i;
	void *pbuf;

	if (!evt_pool) {
		evt_pool = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct bt_evt_pool));
		if (!evt_pool) {
			return false;
		}
		memset(evt_pool, 0, sizeof(struct bt_evt_pool));
	}

	if (false == osif_msg_queue_create(&evt_pool->pool_q_small, BT_EVT_SMALL_POOL_SIZE, sizeof(void *))) {
		goto fail;
	}
	if (false == osif_msg_queue_create(&evt_pool->pool_q_medium, BT_EVT_MEDIUM_POOL_SIZE, sizeof(void *))) {
		goto fail;
	}
	if (false == osif_msg_queue_create(&evt_pool->pool_q_large, BT_EVT_LARGE_POOL_SIZE, sizeof(void *))) {
		goto fail;
	}
	for (i = 0; i < BT_EVT_SMALL_POOL_SIZE; i++) {
		pbuf = &evt_pool->buf_small[i][0];
		if (false == osif_msg_send(evt_pool->pool_q_small, &pbuf, 0)) {
			goto fail;
		}
	}
	for (i = 0; i < BT_EVT_MEDIUM_POOL_SIZE; i++) {
		pbuf = &evt_pool->buf_medium[i][0];
		if (false == osif_msg_send(evt_pool->pool_q_medium, &pbuf, 0)) {
			goto fail;
		}
	}
	for (i = 0; i < BT_EVT_LARGE_POOL_SIZE; i++) {
		pbuf = &evt_pool->buf_large[i][0];
		if (false == osif_msg_send(evt_pool->pool_q_large, &pbuf, 0)) {
			goto fail;
		}
	}
	return true;

fail:
	if (evt_pool->pool_q_small) {
		while (osif_msg_recv(evt_pool->pool_q_small, &pbuf, 0));
		osif_msg_queue_delete(evt_pool->pool_q_small);
	}
	if (evt_pool->pool_q_medium) {
		while (osif_msg_recv(evt_pool->pool_q_medium, &pbuf, 0));
		osif_msg_queue_delete(evt_pool->pool_q_medium);
	}
	if (evt_pool->pool_q_large) {
		while (osif_msg_recv(evt_pool->pool_q_large, &pbuf, 0));
		osif_msg_queue_delete(evt_pool->pool_q_large);
	}
	if (evt_pool) {
		osif_mem_free(evt_pool);
		evt_pool = NULL;
	}
	return false;
}

void bt_evt_mem_pool_deinit(void)
{
	void *pbuf;

	if (!evt_pool) {
		return;
	}

	while (osif_msg_recv(evt_pool->pool_q_small, &pbuf, 0));
	while (osif_msg_recv(evt_pool->pool_q_medium, &pbuf, 0));
	while (osif_msg_recv(evt_pool->pool_q_large, &pbuf, 0));
	osif_msg_queue_delete(evt_pool->pool_q_small);
	osif_msg_queue_delete(evt_pool->pool_q_medium);
	osif_msg_queue_delete(evt_pool->pool_q_large);

	osif_mem_free(evt_pool);
	evt_pool = NULL;
}

static void *choose_evt_mem_pool_q(uint32_t size)
{
	if (size <= BT_EVT_SMALL_BUF_SIZE) {
		return evt_pool->pool_q_small;
	} else if (size <= BT_EVT_MEDIUM_BUF_SIZE) {
		return evt_pool->pool_q_medium;
	} else if (size <= BT_EVT_LARGE_BUF_SIZE) {
		return evt_pool->pool_q_large;
	} else {
		return NULL;
	}
}

static void *find_evt_pool_q_by_buf(uint8_t *pbuf)
{
	if (pbuf >= &evt_pool->buf_small[0][0] &&
		pbuf < (uint8_t *)&evt_pool->buf_small[0][0] + sizeof(evt_pool->buf_small)) {
		return evt_pool->pool_q_small;
	} else if (pbuf >= &evt_pool->buf_medium[0][0] &&
			   pbuf < (uint8_t *)&evt_pool->buf_medium[0][0] + sizeof(evt_pool->buf_medium)) {
		return evt_pool->pool_q_medium;
	} else if (pbuf >= &evt_pool->buf_large[0][0] &&
			   pbuf < (uint8_t *)&evt_pool->buf_large[0][0] + sizeof(evt_pool->buf_large)) {
		return evt_pool->pool_q_large;
	} else {
		return NULL;
	}
}
#endif  /* RTK_BT_API_MEM_PRE_ALLOC */

static bool bt_api_sem_alloc(void **pp_handle)
{
#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	return osif_msg_recv(api_sem_pool_q, pp_handle, 0);
#else
	return osif_sem_create(pp_handle, 0, 1);
#endif
}

static bool bt_api_sem_free(void *p_handle)
{
#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	return osif_msg_send(api_sem_pool_q, &p_handle, 0);
#else
	return osif_sem_delete(p_handle);
#endif
}

uint16_t rtk_bt_send_cmd(uint8_t group, uint8_t act, void *param, uint32_t param_len)
{
	uint16_t ret = RTK_BT_OK;
	uint32_t flags = 0;
	rtk_bt_cmd_t cmd = {0};
	rtk_bt_cmd_t *pcmd = &cmd;

	pcmd->group = group;
	pcmd->act = act;
	pcmd->param = param;
	pcmd->param_len = param_len;

	flags = osif_lock();
	api_task_msg_num++;
	osif_unlock(flags);

	/* check if bt deinit started */
	if (!rtk_bt_is_enable()) {
		ret = RTK_BT_ERR_NOT_READY;
		goto msg_num_hdl;
	}

	if (false == bt_api_sem_alloc(&pcmd->psem)) {
		ret = RTK_BT_ERR_OS_OPERATION;
		goto msg_num_hdl;
	}

	if (bt_stack_api_send(pcmd)) {
		ret = RTK_BT_ERR_MSG_SEND;
		goto msg_num_hdl;
	}

msg_num_hdl:
	flags = osif_lock();
	api_task_msg_num--;
	osif_unlock(flags);

	if (ret) {
		goto end;
	}

	if (false == osif_sem_take(pcmd->psem, BT_TIMEOUT_FOREVER)) {
#if !defined(CONFIG_BT_ZEPHYR) || !CONFIG_BT_ZEPHYR
		/* if the pcmd has been added in pending list, and no stack_cb to pick it off
		within api_sync_timeout, it need to be deleted here */
		bt_stack_pending_cmd_delete(pcmd);
#endif
		ret = RTK_BT_ERR_SYNC_TIMEOUT;
		goto end;
	}

	ret = pcmd->ret;

end:
	if (pcmd->psem) {
		bt_api_sem_free(pcmd->psem);
	}

	return ret;
}

void bt_wait_cmd_send_complete(void)
{
	int i = 0;

	while (api_task_msg_num) {
		osif_delay(5);
		i++;
		if (200 == i) {
			BT_LOGA("!!wait cmd send complete over 1s\r\n");
		}
	}
}

static uint8_t rtk_bt_excute_evt_cb(uint8_t group, uint8_t evt_code, void *param, uint32_t len)
{
	uint8_t ret = 0;
	rtk_bt_evt_cb_t cb_func = NULL;

	if (/*group >= RTK_BT_API_LE_BASE && */group < RTK_BT_LE_GP_MAX) {
		cb_func = rtk_bt_le_evt_cb_tbl[group - RTK_BT_API_LE_BASE];
	} else if (group >= RTK_BT_API_BR_BASE && group < RTK_BT_BR_GP_MAX) {
		cb_func = rtk_bt_br_evt_cb_tbl[group - RTK_BT_API_BR_BASE];
	} else if (group >= RTK_BT_API_COMMON_BASE && group < RTK_BT_COMMON_GP_MAX) {
		cb_func = rtk_bt_evt_cb_tbl[group - RTK_BT_API_COMMON_BASE];
	} else {
		return RTK_BT_EVT_CB_FAIL;
	}

	if (cb_func) {
		ret = cb_func(evt_code, param, len);
	}

	return ret;
}

void rtk_bt_event_free(rtk_bt_evt_t *pevt)
{
#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	void *evt_q = find_evt_pool_q_by_buf((uint8_t *)pevt);

	if (evt_q) {
		if (!osif_msg_send(evt_q, &pevt, 0)) {
			BT_LOGE("rtk_bt_event_free failed!\r\n");
			return;
		}
	} else
#endif
	{
		osif_mem_free(pevt);
	}
}

static void rtk_bt_evt_taskentry(void *ctx)
{
	(void)ctx;
	rtk_bt_evt_t *pevt;

	osif_sem_give(g_evt_task_sem);

#if !defined(CONFIG_PLATFORM_TIZENRT_OS) || defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	osif_create_secure_context(BT_SECURE_STACK_SIZE);
#endif //#if !defined(CONFIG_PLATFORM_TIZENRT_OS) || defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)

	while (true) {
		if (osif_msg_recv(g_evt_queue, &pevt, BT_TIMEOUT_FOREVER)) {
			/* Check msg */
			if (!pevt) {
				continue;
			}
			/* check if need to exit event task */
			if (RTK_BT_EVENT_TASK_EXIT == pevt->group) {
				rtk_bt_event_free(pevt);
				break;
			}

			/* Call back user registered function */
			rtk_bt_excute_evt_cb(pevt->group, pevt->evt, pevt->data, pevt->data_len);
			rtk_bt_event_free(pevt);
		}
	}
	BT_LOGD("[BT evt task] bt evt task exit\r\n");
	osif_sem_give(g_evt_task_sem);
	osif_task_delete(NULL);
}

uint16_t rtk_bt_evt_init(void)
{
	if (false == osif_sem_create(&g_evt_task_sem, 0, 1)) {
		goto failed;
	}

	if (false == osif_msg_queue_create(&g_evt_queue, EVENT_NUM, sizeof(void *))) {
		goto failed;
	}

	if (false == osif_task_create(&g_evt_task_hdl, "bt_event_task",
								  rtk_bt_evt_taskentry, NULL,
								  EVENT_TASK_STACK_SIZE, EVENT_TASK_PRIORITY)) {
		goto failed;
	}

	if (false == osif_sem_take(g_evt_task_sem, BT_TIMEOUT_FOREVER)) {
		goto failed;
	}

	event_task_running = true;
	return 0;

failed:
	if (g_evt_task_hdl) {
		osif_task_delete(g_evt_task_hdl);
		g_evt_task_hdl = NULL;
	}
	if (g_evt_queue) {
		osif_msg_queue_delete(g_evt_queue);
		g_evt_queue = NULL;
	}
	if (g_evt_task_sem) {
		osif_sem_delete(g_evt_task_sem);
		g_evt_task_sem = NULL;
	}

	return RTK_BT_FAIL;
}

static uint16_t rtk_bt_evt_reset_callback(void)
{
	memset(rtk_bt_le_evt_cb_tbl, 0, sizeof(rtk_bt_le_evt_cb_tbl));
	memset(rtk_bt_br_evt_cb_tbl, 0, sizeof(rtk_bt_br_evt_cb_tbl));
	memset(rtk_bt_evt_cb_tbl, 0, sizeof(rtk_bt_evt_cb_tbl));

	return 0;
}

uint16_t rtk_bt_evt_deinit(void)
{
	rtk_bt_evt_t *p_evt;

	event_task_running = false;

	/* This is the last event indicated. */
	p_evt = rtk_bt_event_create(RTK_BT_EVENT_TASK_EXIT, 0, 0);
	if (!p_evt) {
		return RTK_BT_ERR_NO_MEMORY;
	}

	rtk_bt_evt_indicate(p_evt, NULL);

	if (false == osif_sem_take(g_evt_task_sem, BT_TIMEOUT_FOREVER)) {
		return RTK_BT_FAIL;
	}

	osif_sem_delete(g_evt_task_sem);
	osif_msg_queue_delete(g_evt_queue);
	g_evt_task_hdl = NULL;
	g_evt_task_sem = NULL;
	g_evt_queue = NULL;
	rtk_bt_evt_reset_callback();

	return 0;
}

uint16_t rtk_bt_evt_register_callback(uint8_t group, rtk_bt_evt_cb_t cb)
{
	bool b_is_le_mode = false;
	bool b_is_br_mode = false;
	bool b_is_common = false;

	BT_LOGD("--------------> rtk_bt_evt_register_callback: group = 0x%x \r\n", group);

	if (group < RTK_BT_LE_GP_MAX) {
		b_is_le_mode = true;
	} else if ((group >= RTK_BT_API_BR_BASE) && (group < RTK_BT_BR_GP_MAX)) {
		b_is_br_mode = true;
	} else if ((group >= RTK_BT_API_COMMON_BASE) && (group < RTK_BT_COMMON_GP_MAX)) {
		b_is_common = true;
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (b_is_le_mode) {
		rtk_bt_le_evt_cb_tbl[group - RTK_BT_API_LE_BASE] = cb;
	}
	if (b_is_br_mode) {
		rtk_bt_br_evt_cb_tbl[group - RTK_BT_API_BR_BASE] = cb;
	}
	if (b_is_common) {
		rtk_bt_evt_cb_tbl[group - RTK_BT_API_COMMON_BASE] = cb;
	}

	return 0;
}

uint16_t rtk_bt_evt_unregister_callback(uint8_t group)
{
	bool b_is_le_mode = false;
	bool b_is_br_mode = false;
	bool b_is_common = false;

	BT_LOGD("--------------> rtk_bt_evt_unregister_callback \r\n");
	if (true != rtk_bt_is_enable()) {
		return RTK_BT_ERR_NOT_READY;
	}

	if (group < RTK_BT_LE_GP_MAX) {
		b_is_le_mode = true;
	} else if ((group >= RTK_BT_API_BR_BASE) && (group < RTK_BT_BR_GP_MAX)) {
		b_is_br_mode = true;
	} else if ((group >= RTK_BT_API_COMMON_BASE) && (group < RTK_BT_COMMON_GP_MAX)) {
		b_is_common = true;
	} else {
		return RTK_BT_ERR_PARAM_INVALID;
	}

	if (b_is_le_mode) {
		rtk_bt_le_evt_cb_tbl[group - RTK_BT_API_LE_BASE] = NULL;
	}
	if (b_is_br_mode) {
		rtk_bt_br_evt_cb_tbl[group - RTK_BT_API_BR_BASE] = NULL;
	}
	if (b_is_common) {
		rtk_bt_evt_cb_tbl[group - RTK_BT_API_COMMON_BASE] = NULL;
	}

	return 0;
}

rtk_bt_evt_t *rtk_bt_event_create(uint8_t group, uint8_t evt, uint32_t param_len)
{
	rtk_bt_evt_t *pevt = NULL;
#if defined(RTK_BT_API_MEM_PRE_ALLOC) && RTK_BT_API_MEM_PRE_ALLOC
	void *pbuf = NULL;
	void *evt_q = choose_evt_mem_pool_q(sizeof(rtk_bt_evt_t) + param_len);

	if (evt_q && osif_msg_recv(evt_q, &pbuf, 0)) {
		/* Double check if the evt buf len match the queue. If a small buf is send to medium queue,
		it will cause memory overflow when a medium buf is needed but just get a small buf from medium queue. */
		if (evt_q != find_evt_pool_q_by_buf((uint8_t *)pbuf)) {
			BT_LOGE("!Error, evt buf len mismatch the queue\r\n");
			return NULL;
		}
		pevt = (rtk_bt_evt_t *)pbuf;
	} else /* event size too large(evt_q is NULL) or no enough queue element */
#endif
	{
		pevt = (rtk_bt_evt_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(rtk_bt_evt_t) + param_len);
	}

	if (!pevt) {
		return NULL;
	}

	memset(pevt, 0, sizeof(rtk_bt_evt_t) + param_len);
	pevt->group = group;
	pevt->evt = evt;
	pevt->data_len = param_len;
	pevt->data = BT_STRUCT_TAIL(pevt, rtk_bt_evt_t);

	return pevt;
}

static bool rtk_bt_check_evt_cb_direct_calling(uint8_t group, uint8_t evt_code)
{
	bool ret = false;

	switch (group) {
	case RTK_BT_LE_GP_GAP:
		if (rtk_bt_le_gap_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_GATTS:
		if (rtk_bt_le_gatts_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_GATTC:
		if (rtk_bt_le_gattc_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_ISO:
		if (rtk_bt_le_iso_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_AVRCP:
		if (rtk_bt_br_avrcp_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_A2DP:
		if (rtk_bt_br_a2dp_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_HFP:
		if (rtk_bt_br_hfp_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_BR_GP_PBAP:
		if (rtk_bt_br_pbap_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_COMMON_GP_GAP:
		if (rtk_bt_gap_evt_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_BAP:
		if (rtk_bt_le_audio_evt_bap_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_CAP:
		if (rtk_bt_le_audio_evt_cap_direct_calling_flag & (1 << evt_code)) {
			ret = true;
		}
		break;
#if defined(RTK_BLE_MESH_SUPPORT) && RTK_BLE_MESH_SUPPORT
	case RTK_BT_LE_GP_MESH_GENERIC_ONOFF_SERVER_MODEL:
		if (rtk_bt_mesh_generic_onoff_server_model_evt_direct_calling_flag) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_MESH_DATATRANS_MODEL:
		if (rtk_bt_mesh_datatrans_model_evt_direct_calling_flag) {
			ret = true;
		}
		break;
	case RTK_BT_LE_GP_MESH_LIGHT_LIGHTNESS_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_CTL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_CTL_TEMPERATURE_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_HUE_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_HSL_SATURATION_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_XYL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_LC_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_LIGHT_LC_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_TIME_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_TIME_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCHEDULER_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCHEDULER_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCENE_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SCENE_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_LEVEL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_POWER_LEVEL_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_BATTERY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_LOCATION_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_USER_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_CLIENT_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_ADMIN_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_GENERIC_MANU_PROPERTY_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SENSOR_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_SENSOR_SETUP_SERVER_MODEL:
	case RTK_BT_LE_GP_MESH_HEALTH_SERVER_MODEL:
		ret = true;
		break;
#endif
	default:
		break;
	}

	return ret;
}

uint16_t rtk_bt_evt_indicate(void *evt, uint8_t *cb_ret)
{
	uint16_t ret = 0;
	uint32_t msg_num = 0;
	rtk_bt_evt_t *p_evt = (rtk_bt_evt_t *)evt;

	if (!evt) {
		return RTK_BT_ERR_POINTER_INVALID;
	}

	if (true == rtk_bt_check_evt_cb_direct_calling(p_evt->group, p_evt->evt)) {
		uint8_t cb_ret_tmp = rtk_bt_excute_evt_cb(p_evt->group, p_evt->evt, p_evt->data, p_evt->data_len);
		if (cb_ret != NULL) {
			*cb_ret = cb_ret_tmp;
		}
		/* If the evt cb is direct calling, the evt msg isn't actually send to evt task,
		Instead, it's excuted here, so need to free it here */
		rtk_bt_event_free(evt);
		return 0;
	}

	/* send EXIT as last msg to kill task */
	if (!event_task_running && p_evt->group != RTK_BT_EVENT_TASK_EXIT) {
		ret = RTK_BT_ERR_NOT_READY;
		goto end;
	}

	/* When too many scan/escan/padv reported, the event msg may make the msg queue full.
	Then the event such as scan_stop may report fail, so let scan/escan/padv event don't
	take all queue space, leave 1 to let other event have chance to be reported */
	if (RTK_BT_LE_GP_GAP == p_evt->group &&
		(RTK_BT_LE_GAP_EVT_SCAN_RES_IND == p_evt->evt ||
		 RTK_BT_LE_GAP_EVT_EXT_SCAN_RES_IND == p_evt->evt ||
		 RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND == p_evt->evt)) {
		if (osif_msg_queue_peek(g_evt_queue, &msg_num) && msg_num >= (EVENT_NUM - 1)) {
			ret = RTK_BT_ERR_QUEUE_FULL;
			goto end;
		}
	}
	if (false == osif_msg_send(g_evt_queue, &evt, BT_TIMEOUT_NONE)) {
		ret = RTK_BT_ERR_OS_OPERATION;
	}

end:
	if (ret) {
		rtk_bt_event_free(evt);
	}

	return ret;
}
