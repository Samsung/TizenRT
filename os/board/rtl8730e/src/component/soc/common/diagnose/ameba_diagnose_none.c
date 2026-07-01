/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_diagnose.h"

int rtk_diag_init(u16 heap_capacity, u16 sender_buffer_size)
{
	UNUSED(heap_capacity);
	UNUSED(sender_buffer_size);
	return RTK_SUCCESS;
}

void rtk_diag_deinit(void)
{
}
int rtk_diag_event_add(u8 evt_level, u16 evt_type, const u8 *evt_info, u16 evt_len)
{
	UNUSED(evt_level);
	UNUSED(evt_type);
	UNUSED(evt_info);
	UNUSED(evt_len);
	return RTK_SUCCESS;
}

//handle atcmd request and response
int rtk_diag_req_timestamp(void)
{
	return RTK_SUCCESS;
}
int rtk_diag_req_version(void)
{
	return RTK_SUCCESS;
}
int rtk_diag_req_set_buf_com_capacity(u8 *sender_buffer, u16 capacity)
{
	UNUSED(sender_buffer);
	UNUSED(capacity);
	return RTK_SUCCESS;
}
int rtk_diag_req_set_buf_evt_capacity(u16 capacity)
{
	UNUSED(capacity);
	return RTK_SUCCESS;
}
int rtk_diag_req_get_event(u32 timestamp, u16 offset)
{
	UNUSED(timestamp);
	UNUSED(offset);
	return RTK_SUCCESS;
}
int rtk_diag_req_del_event_before(u32 timestamp)
{
	UNUSED(timestamp);
	return RTK_SUCCESS;
}
int rtk_diag_req_del_event_after(u32 timestamp)
{
	UNUSED(timestamp);
	return RTK_SUCCESS;
}
int rtk_diag_req_get_del_events(void)
{
	return RTK_SUCCESS;
}
int rtk_diag_req_clr_del_events(void)
{
	return RTK_SUCCESS;
}
int rtk_diag_req_log_enable(u8 state)
{
	UNUSED(state);
	return RTK_SUCCESS;
}
