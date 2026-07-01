/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_diagnose.h"
#include "ameba_diagnose_transform.h"

#include "os_wrapper.h"

static u8 g_initialized = 0;
static rtos_mutex_t g_ipc_mutex = NULL;
static RtkDiagAtCmd_t g_at_cmd;
u8 g_diag_debug_log_state;
rtk_diag_sender_t g_diag_sender = NULL;
RtkDiagDataFrame_t *g_using_frame_buffer = NULL;
u16 g_using_frame_buffer_capacity = 0;

SRAM_NOCACHE_DATA_SECTION
static volatile RtkDiagIpcMsg_t g_diag_ipc_msg = {0};

int rtk_diag_init(u16 heap_capacity, u16 sender_buffer_size)
{
	//NOTE: These two parameter work in NP side, here just for using uniform API
	UNUSED(heap_capacity);
	UNUSED(sender_buffer_size);
	if (g_initialized == 1) {
		return RTK_SUCCESS;
	}
#ifdef DIAG_DEBUG_TASK
	void rtk_diag_debug_create_task(void);
	rtk_diag_debug_create_task();
#endif

	g_diag_sender = rtk_diag_uart_send;

	if (rtos_mutex_create(&g_ipc_mutex) != RTK_SUCCESS) {
		RTK_LOGA("DIAG", "Failed to create mutex in AP\n");
		return RTK_FAIL;
	}
	g_diag_debug_log_state = DISABLE;
	return RTK_SUCCESS;
}

void rtk_diag_deinit(void)
{
	if (g_initialized == 0) {
		return;
	}
	rtos_mutex_delete(g_ipc_mutex);
	g_ipc_mutex = NULL;
	g_initialized = 0;
	return;
}

static int rtk_diag_ipc_status_get(void)
{
	return g_diag_ipc_msg.flag;
}

static void rtk_diag_ipc_status_set(int flag)
{
	g_diag_ipc_msg.flag = flag;
}

static int rtk_diag_ipc_send_raw(u32 addr, u32 size, u8 type)
{
	IPC_MSG_STRUCT ipc_event_msg = {0};
	u32 timeout = RTK_DIAG_IPC_WAIT_TIMEOUT;

	DCache_Clean(addr, size);
	g_diag_ipc_msg.addr = addr;
	g_diag_ipc_msg.size = size;
	g_diag_ipc_msg.type = type;
	ipc_event_msg.msg = (u32)&g_diag_ipc_msg;
	rtk_diag_ipc_status_set(RTK_DIAG_IPC_WAIT_RESPONSE);
	DCache_Clean((u32)&g_diag_ipc_msg, sizeof(RtkDiagIpcMsg_t));
	ipc_send_message(DIAG_IPC_DIR, DIAG_IPC_CHANNEL, &ipc_event_msg);
	while (timeout--) {
		DCache_Invalidate((u32)&g_diag_ipc_msg, sizeof(RtkDiagIpcMsg_t));
		if (rtk_diag_ipc_status_get() != RTK_DIAG_IPC_WAIT_RESPONSE) {
			return RTK_SUCCESS;
		}
	}
	return RTK_ERR_TIMEOUT;
}

static int rtk_diag_ipc_send(u32 addr, u32 size, u8 type)
{
	if (rtos_mutex_take(g_ipc_mutex, MUTEX_WAIT_TIMEOUT) != RTK_SUCCESS) {
		return RTK_ERR_BUSY;
	}

	int ret = rtk_diag_ipc_send_raw(addr, size, type);
	if (ret == RTK_SUCCESS && type == RTK_DIAG_IPC_MSG_TYPE_ATCMD) {
		/* Only atcmd msg need to response with protocal */
		DCache_Invalidate((u32)g_using_frame_buffer, g_using_frame_buffer_capacity);
		ret = g_diag_sender((const u8 *)g_using_frame_buffer, RTK_DIAG_FRAME_STRUCTURE_REAL_SIZE(g_using_frame_buffer));
	}
	rtos_mutex_give(g_ipc_mutex);
	return ret;
}

int rtk_diag_config_transform(rtk_diag_sender_t sender, u8 *sender_buffer, u16 sender_buffer_size)
{
	int ret = RTK_SUCCESS;
	if (rtos_mutex_take(g_ipc_mutex, MUTEX_WAIT_TIMEOUT) != RTK_SUCCESS) {
		return RTK_ERR_BUSY;
	}
	g_diag_sender = sender == NULL ? rtk_diag_uart_send : sender;
	if (sender_buffer) {
		if (sender_buffer_size < RTK_DIAG_SEND_BUFFER_SIZE_MIN) {
			RTK_LOGA("DIAG", "buf cap(%u) must > %u\n", sender_buffer_size, RTK_DIAG_SEND_BUFFER_SIZE_MIN);
			ret = RTK_ERR_DIAG_TOO_SMALL_BUFF;
			goto end;
		}
		if ((RtkDiagDataFrame_t *)sender_buffer == g_using_frame_buffer && g_using_frame_buffer_capacity == sender_buffer_size) {
			ret =  RTK_SUCCESS;
			goto end;
		}
		g_using_frame_buffer = (RtkDiagDataFrame_t *)sender_buffer;
		g_using_frame_buffer_capacity = sender_buffer_size;
	}
	ret = rtk_diag_ipc_send_raw((u32)g_using_frame_buffer, g_using_frame_buffer_capacity, RTK_DIAG_IPC_MSG_TYPE_CONFIG);
end:
	rtos_mutex_give(g_ipc_mutex);
	return ret;
}

bool rtk_diag_transform_has_configed(void)
{
	return g_diag_sender && g_using_frame_buffer;
}

int rtk_diag_event_add(u8 evt_level, u16 evt_type, const u8 *evt_info, u16 evt_len)
{
	u32 ts = rtos_time_get_current_system_time_ms();//get ts as quick as possible

	RtkDiagEvent_t *event = (RtkDiagEvent_t *)rtos_mem_malloc(sizeof(RtkDiagEvent_t) + evt_len);
	if (event == NULL) {
		return RTK_ERR_DIAG_MALLOC;
	}
	event->evt_type = evt_type;
	event->evt_len = evt_len;
	event->evt_time = ts;
	event->evt_level = evt_level;
	_memcpy(event->evt_info, evt_info, evt_len);

	int ret = rtk_diag_ipc_send((u32)event, sizeof(RtkDiagEvent_t) + evt_len, RTK_DIAG_IPC_MSG_TYPE_EVT_ADD);
	rtos_mem_free(event);
	return ret;
}

int rtk_diag_req_timestamp(void)
{
	u32 tm = rtos_time_get_current_system_time_ms();
	return rtk_diag_req_low(RTK_DIAG_CMD_TYPE_OSTIME, (u8 *)&tm, sizeof(tm), 0);
}

int rtk_diag_req_version(void)
{
	extern const char *g_rtk_diag_format_hash;
	return rtk_diag_req_low(RTK_DIAG_CMD_TYPE_VER, (u8 *)g_rtk_diag_format_hash, strlen(g_rtk_diag_format_hash), 0);
}

int rtk_diag_req_set_buf_com_capacity(u8 *sender_buffer, u16 capacity)
{
	int ret = RTK_SUCCESS;
	if (sender_buffer) {
		if (g_using_frame_buffer != (RtkDiagDataFrame_t *)sender_buffer || g_using_frame_buffer_capacity != capacity) {
			g_using_frame_buffer = (RtkDiagDataFrame_t *)sender_buffer;
			g_using_frame_buffer_capacity = capacity;
			rtk_diag_ipc_send((u32)g_using_frame_buffer, g_using_frame_buffer_capacity, RTK_DIAG_IPC_MSG_TYPE_CONFIG);
		}
		rtk_diag_req_low(RTK_DIAG_CMD_TYPE_BUFFER, (u8 *)&capacity, sizeof(capacity), 0);
	} else {
		ret = capacity < RTK_DIAG_SEND_BUFFER_SIZE_MIN ? RTK_ERR_DIAG_TOO_SMALL_BUFF : RTK_ERR_DIAG_MALLOC;
		rtk_diag_req_low(RTK_DIAG_CMD_TYPE_BUFFER, NULL, 0, ret);
	}
	return ret;
}

int rtk_diag_req_set_buf_evt_capacity(u16 capacity)
{
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_HEAP;
	g_at_cmd.capacity = capacity;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

int rtk_diag_req_get_event(u32 timestamp, u16 offset)
{
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_READ;
	g_at_cmd.timestamp = timestamp;
	g_at_cmd.offset = offset;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

int rtk_diag_req_del_event_before(u32 timestamp)
{
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_DEL_BF;
	g_at_cmd.timestamp = timestamp;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

int rtk_diag_req_del_event_after(u32 timestamp)
{
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_DEL_AF;
	g_at_cmd.timestamp = timestamp;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

int rtk_diag_req_get_del_events(void)
{
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_GET_DEL;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

int rtk_diag_req_clr_del_events(void)
{
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_CLR_DEL;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

int rtk_diag_req_log_enable(u8 state)
{
	if (state) {
		rtk_log_level_set("*", RTK_LOG_INFO);
	} else {
		rtk_log_level_set("*", RTK_LOG_NONE);
	}
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_SET_LOG;
	g_at_cmd.log = state;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}

#ifdef DIAG_DEBUG_TEST
int rtk_diag_req_add_event_demo1(u8 evt_level, const char *data)
{
	struct diag_evt_demo_1 demo = {
		.u8_data = 1,
		.u16_data = 2,
		.f_data = 1.1,
		.ch_data = 'r',
	};
	strncpy(demo.str_data, data, 31);
	return rtk_diag_event_add(evt_level, RTK_EVENT_TYPE_DBG_DEMO_1, (const u8 *)&demo, sizeof(struct diag_evt_demo_1) - (32 - (strlen(data) + 1)));
}
int rtk_diag_req_dbg_log_enable(u8 state)
{
	g_diag_debug_log_state = state;
	g_at_cmd.type = RTK_DIAG_CMD_TYPE_SET_DBG_LOG;
	g_at_cmd.log = state;
	return rtk_diag_ipc_send((u32)&g_at_cmd, sizeof(RtkDiagAtCmd_t), RTK_DIAG_IPC_MSG_TYPE_ATCMD);
}
#endif