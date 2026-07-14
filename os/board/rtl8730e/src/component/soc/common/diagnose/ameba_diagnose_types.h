/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DIAGNOSE_TYPES_H_
#define _AMEBA_DIAGNOSE_TYPES_H_

#include "ameba_soc.h"
#include "ameba_diagnose_format.h"
#include "ameba_diagnose_customer.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************************
 *                                   Internal
 ****************************************************************************************************/
#define RTK_DIAG_HEAP_SIZE              2048
#define RTK_DIAG_SYS_HEAP_UPPER_LIMIT   4096
#define RTK_DIAG_LITTLE_EVENT_THRESHOLD 128
#define RTK_DIAG_BIT_EVENT_THRESHOLD    RTK_DIAG_HEAP_SIZE
#define RTK_DIAG_SEND_BUFFER_SIZE       512

#define RTK_DIAG_SEND_BUFFER_SIZE_MIN (sizeof(RtkDiagDataFrame_t) + 2 + 1 + 1) //at least transform one byte event
#define RTK_DIAG_HEAP_SIZE_MIN (sizeof(RtkDiagEvent_t) + RTK_DIAG_LITTLE_EVENT_THRESHOLD) //at least store one largest little event

enum rtk_diag_cmd_type {
	RTK_DIAG_CMD_TYPE_OSTIME  = 0x01,
	RTK_DIAG_CMD_TYPE_BUFFER  = 0x02,
	RTK_DIAG_CMD_TYPE_READ    = 0x03,
	RTK_DIAG_CMD_TYPE_HEAP    = 0x04,
	RTK_DIAG_CMD_TYPE_VER     = 0x05,
	RTK_DIAG_CMD_TYPE_GET_DEL = 0x06,
	RTK_DIAG_CMD_TYPE_CLR_DEL = 0x07,
	RTK_DIAG_CMD_TYPE_DEL_BF  = 0x08,
	RTK_DIAG_CMD_TYPE_DEL_AF  = 0x09,
	RTK_DIAG_CMD_TYPE_SET_LOG = 0x0A,

	RTK_DIAG_CMD_TYPE_SET_DBG_LOG = 0x20,
	RTK_DIAG_CMD_TYPE_NONE,
};

enum rtk_diag_ipc_msg_type {
	RTK_DIAG_IPC_MSG_TYPE_CONFIG    = 0x00,
	RTK_DIAG_IPC_MSG_TYPE_ATCMD     = 0x01,
	RTK_DIAG_IPC_MSG_TYPE_EVT_ADD   = 0x02,
};

#define RTK_DIAG_IPC_WAIT_RESPONSE  (-255)
#define RTK_DIAG_IPC_DATA_READY     (-254)
#define RTK_DIAG_IPC_FINISH         (-253)
#define RTK_DIAG_IPC_WAIT_TIMEOUT (1000000)

typedef struct {
	u8 type; //enum from rtk_diag_cmd_type
	u32 timestamp;
	u16 offset;
	u32 capacity;
	u8 log;
} RtkDiagAtCmd_t;

typedef struct {
	u32 addr; //addr of event(RtkDiagEvent_t) or atcmd(RtkDiagAtCmd_t)
	u32 size; //size of data at addr
	u8 type;  //rtk_diag_ipc_msg_type
	int flag; //RTK_DIAG_IPC_WAIT_RESPONSE: before response, other: return code
} RtkDiagIpcMsg_t;

typedef struct {
	u32 evt_time;
	u16 evt_type;
	u16 evt_len;
} RtkDiagDeletedEvent_t;

#define RTK_DIAG_FRAME_STRUCTURE_REAL_SIZE(frame) (sizeof(RtkDiagDataFrame_t) + frame->size)
#define RTK_DIAG_EVENT_STRUCTURE_REAL_SIZE(event) (sizeof(RtkDiagEvent_t) + event->evt_len)

typedef u32(*rtk_diag_heap_length_getter_t)(void *data);
typedef void (*rtk_diag_heap_release_notice_t)(void *data);
typedef const RtkDiagEvent_t *(*rtk_diag_queue_next_event_getter_t)(void);
typedef void (*rtk_diag_ring_array_emplace_t)(void *address, const void *origin_data);
typedef int (*rtk_diag_sender_t)(const u8 *data, u16 len);


#ifdef DIAG_DEBUG_TEST
#define DIAG_DBG_ASSERT(v) assert_param(v)
#else
#define DIAG_DBG_ASSERT(v)
#endif

#ifdef __cplusplus
}
#endif

#endif