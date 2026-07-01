/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_diagnose_types.h"
#include "ameba_diagnose_protocol.h"

#include "os_wrapper.h"


extern rtk_diag_sender_t g_diag_sender;
extern RtkDiagDataFrame_t *g_using_frame_buffer;
extern u16 g_using_frame_buffer_capacity;

void rtk_diag_proto_pack_crc(RtkDiagDataFrame_t *frame)
{
	u8 check_sum = frame->cmd;
	for (int i = 0; i < frame->size - 1; i++) {
		check_sum ^= frame->payload[i];
	}
	frame->payload[frame->size - 1] = check_sum;
}

void rtk_diag_proto_pack_custom(RtkDiagDataFrame_t *sender_buffer, u8 cmd_type, const u8 *payload, u16 payload_length)
{
	sender_buffer->header = RTK_DIAG_FRAME_HEADER;
	sender_buffer->size = payload_length + 1;
	sender_buffer->cmd = cmd_type;
	_memcpy(sender_buffer->payload, payload, payload_length);
	rtk_diag_proto_pack_crc(sender_buffer);
}

void rtk_diag_proto_pack_error(RtkDiagDataFrame_t *sender_buffer, u8 cmd_type, u8 error)
{
	if (error) {
		cmd_type = BIT7 | cmd_type;
	} else {
		cmd_type = (~BIT7) & cmd_type;
	}
	rtk_diag_proto_pack_custom(sender_buffer, cmd_type, &error, 1);
}


int rtk_diag_req_low(u8 cmd, const u8 *payload, u16 payload_length, u8 error)
{
	if (payload) {
		rtk_diag_proto_pack_custom(g_using_frame_buffer, cmd, payload, payload_length);
	} else {
		rtk_diag_proto_pack_error(g_using_frame_buffer, cmd, error);
	}
#ifdef DIAG_CORE_AS_AP
	g_diag_sender((u8 *)g_using_frame_buffer, RTK_DIAG_FRAME_STRUCTURE_REAL_SIZE(g_using_frame_buffer));
#ifndef DIAG_SINGLE_CORE
	DCache_Invalidate((u32)g_using_frame_buffer, g_using_frame_buffer_capacity);
#endif
#endif
	return RTK_SUCCESS;
}