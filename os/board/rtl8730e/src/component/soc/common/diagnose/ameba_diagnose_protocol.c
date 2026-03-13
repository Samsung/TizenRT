/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_diagnose_types.h"
#include "ameba_diagnose_protocol.h"
#include "ameba_diagnose.h"

#include "os_wrapper.h"

enum {
	RTK_DIAG_TRANSFORM_COMPLETE = 0,
	RTK_DIAG_TRANSFORM_ONGOING,
};

typedef struct {
	RtkDiagDataFrame_t *data_frame;
	rtk_diag_queue_next_event_getter_t getter;
	u16 payload_capacity;
	u16 event_capacity; //==payload_capacity - sizeof(offset):2 - sizeof(crc):1

	const RtkDiagEvent_t *last_event;
	u8 last_event_state;
	u16 last_event_offset; /** Offset of fragment transmission for the previous event */
} RtkDiagTransformHandler_t;

static RtkDiagTransformHandler_t *g_handler = NULL;

static int rtk_diag_proto_load_data_from_start(const RtkDiagEvent_t *event, u16 local_offset)
{
	assert_param(local_offset == 0);
#if 0
	if (g_handler->last_event_state == RTK_DIAG_TRANSFORM_ONGOING) {
		/** No pending data to send is recorded on the device side at this time */
		if (g_handler->last_event == event) {
			/** Still transmit the previous event, possibly because the first data fragment was lost */
		} else {
			/** Different from the last event, possibly because the previous event fragment was deleted */
		}
	}
#endif
	u16 event_size = RTK_DIAG_EVENT_STRUCTURE_REAL_SIZE(event);
	u16 total_event_size = 0;
	if (event_size > g_handler->event_capacity) {
		/** Fragment transmission for events larger than the buffer capacity */
		g_handler->data_frame->cmd = 0x03;
		total_event_size = g_handler->event_capacity; //loads using whole buffer
		g_handler->last_event = event;
		g_handler->last_event_offset = g_handler->event_capacity;
		g_handler->last_event_state = RTK_DIAG_TRANSFORM_ONGOING;
		_memcpy(g_handler->data_frame->payload + 2, (void *)event, g_handler->event_capacity);
	} else {
		/** Attempt to transmit multiple events at once if their size is less than the buffer capacity */
		g_handler->last_event_offset = 0;
		g_handler->last_event_state = RTK_DIAG_TRANSFORM_COMPLETE;
		g_handler->data_frame->cmd = 0x03;
		while (event) {
			event_size = RTK_DIAG_EVENT_STRUCTURE_REAL_SIZE(event);
			if (event_size + total_event_size > g_handler->event_capacity) {
				break;
			}
			g_handler->last_event = event;
			_memcpy(g_handler->data_frame->payload + 2 + total_event_size, (void *)event, event_size);
			total_event_size += event_size;
			event = g_handler->getter();
		}
	}
	g_handler->data_frame->size = total_event_size + 2 + 1; //add offset and cmd
	return RTK_SUCCESS;
}

static int rtk_diag_proto_load_data_from_middle(const RtkDiagEvent_t *event, u16 local_offset)
{
	assert_param(local_offset != 0);
#if 0
	if (g_handler->last_event_state == RTK_DIAG_TRANSFORM_COMPLETE) {
		if (g_handler->last_event == event) {
			/** Retransmit a specific data fragment */
		} else {
			/** Is the retransmitted fragment different from the previously recorded event? Should not enter here */
			assert_param(0);
		}
	}
#endif
	u16 total_event_size = 0;
	g_handler->last_event = event;
	u16 event_size = RTK_DIAG_EVENT_STRUCTURE_REAL_SIZE(event);
	if (event_size - local_offset > g_handler->event_capacity) {
		/** Need to continue fragmenting */
		g_handler->data_frame->cmd = 0x13;
		g_handler->last_event_state = RTK_DIAG_TRANSFORM_ONGOING;
		g_handler->last_event_offset = local_offset;
		total_event_size = g_handler->event_capacity;
	} else {
		/** Send all at once */
		g_handler->data_frame->cmd = 0x23;
		g_handler->last_event_state = RTK_DIAG_TRANSFORM_COMPLETE;
		g_handler->last_event_offset = 0;
		total_event_size = event_size - local_offset;
	}
	_memcpy(g_handler->data_frame->payload + 2, (u8 *)event + local_offset, total_event_size);
	g_handler->data_frame->size = total_event_size + 2 + 1; //add offset and cmd
	return RTK_SUCCESS;
}

int rtk_diag_proto_init(rtk_diag_queue_next_event_getter_t getter)
{
	g_handler = (RtkDiagTransformHandler_t *)rtos_mem_malloc(sizeof(RtkDiagTransformHandler_t));
	if (NULL == g_handler) {
		RTK_LOGA("DIAG", "Malloc failed\n");
		return RTK_ERR_DIAG_MALLOC;
	}
	// g_handler->data_frame->header = RTK_DIAG_FRAME_HEADER;

	g_handler->getter = getter;
	g_handler->payload_capacity = 0;
	g_handler->event_capacity = 0;//==payload_capacity - sizeof(offset):2 - sizeof(crc):1

	g_handler->last_event = NULL;
	g_handler->last_event_state = RTK_DIAG_TRANSFORM_COMPLETE;
	return RTK_SUCCESS;
}

void rtk_diag_proto_deinit(void)
{
	if (g_handler) {
		rtos_mem_free(g_handler);
		g_handler = NULL;
	}
}

void rtk_diag_proto_set_buffer(RtkDiagDataFrame_t *sender_buffer, u16 sender_buffer_size)
{
	g_handler->data_frame = sender_buffer;
	g_handler->data_frame->header = RTK_DIAG_FRAME_HEADER;
	g_handler->payload_capacity = sender_buffer_size - sizeof(RtkDiagDataFrame_t);
	g_handler->event_capacity = g_handler->payload_capacity - 2 - 1;//==payload_capacity - sizeof(offset):2 - sizeof(crc):1
}

void rtk_diag_proto_pack_event(const RtkDiagEvent_t *event, u16 global_offset, u16 local_offset)
{
	if (event == NULL) {
		//pack an empty frame indicate all event are transferred
		g_handler->data_frame->cmd = 0x03;
		g_handler->data_frame->size = 1;
	} else {
		if (local_offset == 0) {
			rtk_diag_proto_load_data_from_start(event, local_offset);
		} else {
			rtk_diag_proto_load_data_from_middle(event, local_offset);
		}
		_memcpy(g_handler->data_frame->payload, &global_offset, sizeof(u16));
		//WARNING: DONOT USE: *((u16*)g_handler->data_frame->payload) = global_offset;
	}
	rtk_diag_proto_pack_crc(g_handler->data_frame);
}