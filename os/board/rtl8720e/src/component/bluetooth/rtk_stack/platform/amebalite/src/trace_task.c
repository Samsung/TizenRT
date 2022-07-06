/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include "trace_app.h"
#include "cycle_queue.h"
#include "trace_uart.h"
#include "osif.h"

#define TRACE_TASK_PRIO               3
#define TRACE_TASK_STACK_SIZE         0x200
#define TRACE_QUEUE_LENGTH_EVENT      0x80

#define TRACE_EVENT_TX                0
#define TRACE_EVENT_DEINIT            0xFF

typedef struct _TTraceTaskBuffer {
	uint8_t     *Pointer;
	uint16_t    Length;
} TTraceTaskBuffer;

typedef struct {
	void               *handle;               /* task handle */
	void               *QueueHandleEvent;     /* task queue */
	TTraceTaskBuffer    Buffer;               /* actual buffer */
} T_TRACE_TASK_INFO;

static T_TRACE_TASK_INFO trace;
static uint8_t flag_trace_deinit = 0;

bool traceuart_tx_cb(void)
{
	trace.Buffer.Pointer = NULL;
	UpdateQueueRead(trace.Buffer.Length);
	trace.Buffer.Length = 0;

	return true;
}

static void traceStartTransmit(void)
{
	uint16_t queueSize = CycQueueSize();

	if (queueSize > 0) {
		trace.Buffer.Pointer = (uint8_t *)(cyc_buffer + pRead);

		if (pRead + queueSize >= MAX_BUFFER_SIZE) {
			trace.Buffer.Length = MAX_BUFFER_SIZE - pRead;
		} else {
			trace.Buffer.Length = queueSize;
		}
		trace_uart_tx(trace.Buffer.Pointer, trace.Buffer.Length, traceuart_tx_cb);
	}
}

static void trace_task(void *pParameters)
{
	(void)pParameters;
	uint8_t Event;
	uint8_t flag_to_break = 0;

	while (1) {
		if (osif_msg_recv(trace.QueueHandleEvent, &Event, 0xFFFFFFFF) == true) {
			switch (Event) {
			case TRACE_EVENT_TX:
				traceStartTransmit();
				break;

			case TRACE_EVENT_DEINIT:
				traceStartTransmit();       //Send the remaining trace log
				flag_to_break = 1;
				break;

			default:
				printf("trace_task recv msg %d\r\n", Event);
				break;
			}
		} else {
			printf("trace_task recv msg fail\r\n");
		}
		if (flag_to_break) {
			break;
		}
	}
	FreeCycQueue();
	trace_uart_deinit();
	if (trace.QueueHandleEvent != NULL) {
		osif_msg_queue_delete(trace.QueueHandleEvent);
	}
	memset(&trace, 0, sizeof(trace));
	flag_trace_deinit = 2;
	osif_task_delete(NULL);
}

bool bt_trace_init(void)
{
	trace_uart_init();

	if ((trace.handle != NULL) && (trace.QueueHandleEvent != NULL)) {
		printf("reopen bt trace, do nothing\r\n");
		return false;
	}

	osif_msg_queue_create(&trace.QueueHandleEvent, TRACE_QUEUE_LENGTH_EVENT, sizeof(uint8_t));
	osif_task_create(&trace.handle, "trace_task", trace_task, NULL, TRACE_TASK_STACK_SIZE, TRACE_TASK_PRIO);

	return true;
}

bool bt_trace_deinit(void)
{
	uint8_t event = TRACE_EVENT_DEINIT;
	flag_trace_deinit = 1;
	osif_msg_send(trace.QueueHandleEvent, &event, 0xFFFFFFFF);
	while (flag_trace_deinit != 2) {
		osif_delay(1);
	}
	flag_trace_deinit = 0;
	return true;
}

bool trace_print(void *pData, uint16_t Length)
{
	if (trace.handle == NULL) {
		printf("trace task is needed to print trace log\r\n");
		return false;
	}
	
	if (flag_trace_deinit == 0 && CycQueueWrite(pData, Length)) {
		uint32_t pending_msg_count = 0;
		osif_msg_queue_peek(trace.QueueHandleEvent, &pending_msg_count);
		if (pending_msg_count >= TRACE_QUEUE_LENGTH_EVENT) {
			return false;
		}

		uint8_t Event = TRACE_EVENT_TX;
		osif_msg_send(trace.QueueHandleEvent, &Event, 0);
		return true;
	} else {
		//printf("trace_print: buffer is full, need %d, left %d\r\n", Length, CycQueueRemainSize());
		return false;
	}
}
