/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DIAGNOSE_PROTOCOL_H_
#define _AMEBA_DIAGNOSE_PROTOCOL_H_

#include "ameba_diagnose_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int rtk_diag_proto_init(rtk_diag_queue_next_event_getter_t getter);
void rtk_diag_proto_deinit(void);

void rtk_diag_proto_set_buffer(RtkDiagDataFrame_t *sender_buffer, u16 sender_buffer_size);
void rtk_diag_proto_pack_event(const RtkDiagEvent_t *event, u16 global_offset, u16 local_offset);

void rtk_diag_proto_pack_error(RtkDiagDataFrame_t *sender_buffer, u8 cmd_type, u8 error);
void rtk_diag_proto_pack_custom(RtkDiagDataFrame_t *sender_buffer, u8 cmd_type, const u8 *payload, u16 payload_length);

#ifdef __cplusplus
}
#endif

#endif