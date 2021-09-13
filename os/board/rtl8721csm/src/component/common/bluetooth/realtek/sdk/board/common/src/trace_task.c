/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#include "os_msg.h"
#include "os_task.h"

#include "trace_app.h"
#include "cycle_queue.h"
#include "bt_board.h"
#include "trace_uart.h"

#define TRACE_QUEUE_LENGTH_EVENT      0x80

#define TRACE_EVENT_TX                0
#define TRACE_EVENT_TX_COMPLETED      1
#define TRACE_EVENT_DEINIT            0xFF

typedef struct _TTraceTaskBuffer
{
    uint8_t     *Pointer;
    uint16_t Length;
} TTraceTaskBuffer;

typedef struct
{
    void               *handle;               /* task handle */
    void               *QueueHandleEvent;     /* task queue */
    TTraceTaskBuffer    Buffer;               /* actual buffer */
} T_TRACE_TASK_INFO;

static T_TRACE_TASK_INFO trace;
static uint8_t flag_trace_deinit = 0;

bool traceuart_tx_cb(void)
{
    uint8_t Event = TRACE_EVENT_TX_COMPLETED;
    if (flag_trace_deinit == 0) {
        os_msg_send(trace.QueueHandleEvent, &Event, 0);
        return true;
    }
    return false;
}

static void traceStartTransmit(void)
{
    uint16_t queueSize = CycQueueSize();

    if (trace.Buffer.Pointer == (uint8_t *)0 && (queueSize > 0))
    {
        trace.Buffer.Pointer = (uint8_t *)(cyc_buffer + pRead);

        if (pRead + queueSize >= MAX_BUFFER_SIZE)
        {
            trace.Buffer.Length = MAX_BUFFER_SIZE - pRead;
        }
        else
        {
            trace.Buffer.Length = queueSize;
        }
        trace_uart_tx(trace.Buffer.Pointer, trace.Buffer.Length, traceuart_tx_cb);
    }
}

static void trace_task(void *pParameters)
{
    (void)pParameters;
    uint8_t flag_to_break = 0;
    while (1)
    {
        uint8_t Event;

        if (os_msg_recv(trace.QueueHandleEvent, &Event, 0xFFFFFFFF) == true)
        {
            switch (Event)
            {
            case TRACE_EVENT_TX:            /* new trace output */
                traceStartTransmit();
                break;

            case TRACE_EVENT_TX_COMPLETED:  /* transmit completed */
                if (trace.Buffer.Pointer != (uint8_t *)0)
                {
                    trace.Buffer.Pointer = (uint8_t *)0;
                    UpdateQueueRead(trace.Buffer.Length);
                    trace.Buffer.Length = 0;
                }
                traceStartTransmit();
                break;

            case TRACE_EVENT_DEINIT:
                flag_to_break = 1;
                break;

            default:
                break;
            }
        }
        if (flag_to_break)
            break;
    }
    FreeCycQueue();
    trace_uart_deinit();
    if (trace.QueueHandleEvent != NULL)
        os_msg_queue_delete(trace.QueueHandleEvent);
    trace.handle = NULL;
    trace.QueueHandleEvent = NULL;
    flag_trace_deinit = 2;
    os_task_delete(NULL);
}

bool bt_trace_init(void)
{
    trace_uart_init();

    if ((trace.handle != NULL) && (trace.QueueHandleEvent != NULL))
    {
        printf("reopen bt trace, do nothing\r\n");
        return false;
    }

    os_msg_queue_create(&trace.QueueHandleEvent, TRACE_QUEUE_LENGTH_EVENT, sizeof(uint8_t));
    os_task_create(&trace.handle, "trace_task", trace_task, NULL, 0x200, TRACE_TASK_PRIO);   /*need up priority */

    return true;
}

bool bt_trace_uninit(void)
{
    uint8_t event = TRACE_EVENT_DEINIT;
    flag_trace_deinit = 1;
    os_msg_send(trace.QueueHandleEvent, &event, 0xFFFFFFFF);
    while (flag_trace_deinit != 2) {
        os_delay(10);
    }
    flag_trace_deinit = 0;
    return true;
}

bool trace_print(void *pData, uint16_t Length)
{
    if (trace.handle == NULL)
    {
        printf("trace task is deleted, not work\r\n");
        return false;
    }

    if (flag_trace_deinit == 0 && CycQueueWrite(pData, Length))
    {
        uint32_t pending_msg_count = 0;
        os_msg_queue_peek(trace.QueueHandleEvent, &pending_msg_count);
        if (pending_msg_count >= TRACE_QUEUE_LENGTH_EVENT) {
            return false;
        }

        uint8_t Event = TRACE_EVENT_TX;
        os_msg_send(trace.QueueHandleEvent, &Event, 0);
        return true;
    }
    else
    {
        //printf("trace_print: buffer is full, need %d, left %d\r\n", Length, CycQueueRemainSize());
        return false;
    }
}
