/****************************************************************************
 * external/sysview/os_trace_events_tizenrt.h
 *
 *   Copyright (C) 2018 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: ByoungTae Cho <bt.cho@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *
 ****************************************************************************/

/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2015 - 2017  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the RTT protocol and J-Link.                       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* conditions are met:                                                *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this list of conditions and the following disclaimer.    *
*                                                                    *
* o Redistributions in binary form must reproduce the above          *
*   copyright notice, this list of conditions and the following      *
*   disclaimer in the documentation and/or other materials provided  *
*   with the distribution.                                           *
*                                                                    *
* o Neither the name of SEGGER Microcontroller GmbH & Co. KG         *
*   nor the names of its contributors may be used to endorse or      *
*   promote products derived from this software without specific     *
*   prior written permission.                                        *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: V2.42                                    *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : os_trace.h
Purpose : Interface header for Micrium uC/OS-III and SystemView.
Revision: $Rev: 3809 $
*/
#include "SEGGER_SYSVIEW.h"

/*
********************************************************************************
* TizenRT Trace Macros
********************************************************************************
*/
#ifdef CONFIG_SYSVIEW
#  define  OS_TRACE_INIT()                             SEGGER_SYSVIEW_Conf()
#  define  OS_TRACE_START()                            SEGGER_SYSVIEW_Start()
#  define  OS_TRACE_STOP()                             SEGGER_SYSVIEW_Stop()


/*
********************************************************************************
* TizenRT Trace fixed defines for SystemView
********************************************************************************
*/

#  define OS_TRACE_ID_OFFSET                          (32u)

#  define OS_TRACE_ID_TICK_INCREMENT                  ( 1u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_ISR_REGISTER                    ( 2u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_MSG_Q_CREATE               ( 3u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_MSG_Q_POST                 ( 4u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_MSG_Q_PEND                 ( 5u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_SEM_CREATE                 ( 6u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_SEM_POST                   ( 7u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_SEM_WAIT                   ( 8u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MUTEX_CREATE                    ( 9u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MUTEX_DEL                       (10u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MUTEX_POST                      (11u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MUTEX_PEND                      (12u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_TASK_PRIORITY                   (13u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_SEM_CREATE                      (15u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_SEM_DEL                         (16u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_SEM_POST                        (17u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_SEM_WAIT                        (18u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_SEM_TIMEWAIT                    (19u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MSGQ_OPEN                       (20u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MSGQ_CLOSE                      (21u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MSGQ_SEND                       (22u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MSGQ_RECV                       (23u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_FLAG_CREATE                     (24u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_FLAG_DEL                        (25u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_FLAG_POST                       (26u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_FLAG_PEND                       (27u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MEM_CREATE                      (28u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MEM_ALLOC                       (29u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_MEM_FREE                        (30u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_ANNOTATION_MSG                  (50u + OS_TRACE_ID_OFFSET)
#  define OS_TRACE_ID_ANNOTATION_VALUE                (51u + OS_TRACE_ID_OFFSET)
#else  /* CONFIG_SYSVIEW */
#  define  OS_TRACE_INIT()
#  define  OS_TRACE_START()
#  define  OS_TRACE_STOP()
#endif /* CONFIG_SYSVIEW */

#ifdef CONFIG_SYSVIEW_MONITOR_TASK
#  define OS_TRACE_TASK_CREATE(p_tcb)                 if (p_tcb != 0) {                               \
                                                        SEGGER_SYSVIEW_OnTaskCreate((U32)p_tcb->pid); \
                                                        SYSVIEW_AddTask((U32)p_tcb->pid,              \
                                                                        &(p_tcb->name[0]),            \
                                                                        p_tcb->sched_priority,        \
                                                                        p_tcb->stack_alloc_ptr,       \
                                                                        p_tcb->adj_stack_size         \
                                                                        );                            \
                                                      }

#  define OS_TRACE_TASK_UPDATE(p_tcb)                 if (p_tcb != 0) {                               \
                                                        SEGGER_SYSVIEW_OnTaskCreate((U32)p_tcb->pid); \
                                                        SYSVIEW_UpdateTask((U32)p_tcb->pid,           \
                                                        &(p_tcb->name[0]),                            \
                                                        p_tcb->sched_priority,                        \
                                                        p_tcb->stack_alloc_ptr,                       \
                                                        p_tcb->adj_stack_size                         \
                                                        );                                            \
                                                      }

#  define OS_TRACE_TASK_READY(p_tcb)                            SYSVIEW_TaskReady((U32)p_tcb->pid)
#  define OS_TRACE_TASK_SWITCHED_IN(p_tcb)                      SYSVIEW_TaskSwitchedIn((U32)p_tcb->pid)
#  define OS_TRACE_TASK_DLY(dly_ticks)
#  define OS_TRACE_TASK_SUSPEND(p_tcb)
#  define OS_TRACE_TASK_SUSPENDED(p_tcb)                        SYSVIEW_TaskSuspend((U32)p_tcb->pid)
#  define OS_TRACE_TASK_RESUME(p_tcb)                           SYSVIEW_TaskReady((U32)p_tcb->pid)
#  define OS_TRACE_TASK_PRIORITY(p_tcb, _old, _new)             SEGGER_SYSVIEW_RecordU32x3(OS_TRACE_ID_TASK_PRIORITY,              SEGGER_SYSVIEW_ShrinkId((U32)p_tcb->pid),   (U32)_old, (U32)_new                             )
#else
#  define OS_TRACE_TASK_CREATE(p_tcb)
#  define OS_TRACE_TASK_UPDATE(p_tcb)
#  define OS_TRACE_TASK_READY(p_tcb)
#  define OS_TRACE_TASK_SWITCHED_IN(p_tcb)
#  define OS_TRACE_TASK_DLY(dly_ticks)
#  define OS_TRACE_TASK_SUSPEND(p_tcb)
#  define OS_TRACE_TASK_SUSPENDED(p_tcb)
#  define OS_TRACE_TASK_RESUME(p_tcb)
#  define OS_TRACE_TASK_DEL(p_tcb)
#  define OS_TRACE_TASK_PRIORITY(p_tcb, _old, _new)
#endif

#ifdef CONFIG_SYSVIEW_MONITOR_INTERRUPT
#  define OS_TRACE_ISR_REGISTER(isr_id, isr_name, isr_prio)     SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_ISR_REGISTER,               (U32)isr_id,                         (U32)isr_prio                                           )
#  define OS_TRACE_ISR_ENTER()                                  SEGGER_SYSVIEW_RecordEnterISR()
#  define OS_TRACE_ISR_EXIT()                                   SEGGER_SYSVIEW_RecordExitISR()
#else
#  define OS_TRACE_ISR_REGISTER(isr_id, isr_name, isr_prio)
#  define OS_TRACE_ISR_ENTER()
#  define OS_TRACE_ISR_EXIT()
#endif

#ifdef CONFIG_SYSVIEW_MONITOR_OSTICK
#  define OS_TRACE_TICK_INCREMENT(OSTickCtr)                    SEGGER_SYSVIEW_RecordU32  (OS_TRACE_ID_TICK_INCREMENT,             (U32)OSTickCtr                                                                               )
#else
#  define OS_TRACE_TICK_INCREMENT(OSTickCtr)
#endif

#ifdef CONFIG_SYSVIEW_MONITOR_SEMAPHORE
#  define OS_TRACE_SEM_CREATE(p_sem, p_name)                    SYSVIEW_RecordU32Register(OS_TRACE_ID_SEM_CREATE,                  ((U32)p_sem),      p_name                                                                    )
#  define OS_TRACE_SEM_DEL(p_sem, p_calleraddr)                 if (p_sem->trace) SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_SEM_DEL,                    (U32)p_sem,        (U32)p_calleraddr                                       )
#  define OS_TRACE_SEM_POST_ENTER(p_sem, p_calleraddr)          if (p_sem->trace) SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_SEM_POST,                   (U32)p_sem,        (U32)p_calleraddr                                       )
#  define OS_TRACE_SEM_WAIT_ENTER(p_sem, p_calleraddr)          if (p_sem->trace) SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_SEM_WAIT,                   (U32)p_sem,        (U32)p_calleraddr                                       )
#  define OS_TRACE_SEM_TIMEWAIT(p_sem, timeout, p_calleraddr)   if (p_sem->trace) SEGGER_SYSVIEW_RecordU32x3(OS_TRACE_ID_SEM_TIMEWAIT,               (U32)p_sem,        (U32)timeout,       (U32)p_calleraddr                   )
#  define OS_TRACE_SEM_POST_EXIT(p_sem, RetVal)                 if (p_sem->trace) SEGGER_SYSVIEW_RecordEndCallU32(OS_TRACE_ID_SEM_POST,              RetVal)
#  define OS_TRACE_SEM_WAIT_EXIT(p_sem, RetVal)                 if (p_sem->trace) SEGGER_SYSVIEW_RecordEndCallU32(OS_TRACE_ID_SEM_WAIT,              RetVal)

#else
#  define OS_TRACE_SEM_CREATE(p_sem, p_name)
#  define OS_TRACE_SEM_DEL(p_sem, p_calleraddr)
#  define OS_TRACE_SEM_POST_ENTER(p_sem, p_calleraddr)
#  define OS_TRACE_SEM_WAIT_ENTER(p_sem, p_calleraddr)
#  define OS_TRACE_SEM_POST_EXIT(p_sem, RetVal)
#  define OS_TRACE_SEM_WAIT_EXIT(p_sem, RetVal)
#  define OS_TRACE_SEM_TIMEWAIT(p_sem, timeout, p_calleraddr)
#endif

#ifdef CONFIG_SYSVIEW_MONITOR_MSGQ
#  define OS_TRACE_MSGQ_OPEN(p_q, p_name)                       SYSVIEW_RecordU32Register(OS_TRACE_ID_MSGQ_OPEN,                   ((U32)p_q),                          p_name                                                  )
#  define OS_TRACE_MSGQ_CLOSE(p_q, p_calleraddr)                SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_MSGQ_CLOSE,                 SEGGER_SYSVIEW_ShrinkId((U32)p_q),   (U32)p_calleraddr                                       )
#  define OS_TRACE_MSGQ_SEND(p_q, p_calleraddr)                 SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_MSGQ_SEND,                  SEGGER_SYSVIEW_ShrinkId((U32)p_q),   (U32)p_calleraddr                                       )
#  define OS_TRACE_MSGQ_RECV(p_q, p_calleraddr)                 SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_MSGQ_RECV,                  SEGGER_SYSVIEW_ShrinkId((U32)p_q),   (U32)p_calleraddr                                       )
#else
#  define OS_TRACE_MSGQ_OPEN(p_q, p_name)
#  define OS_TRACE_MSGQ_CLOSE(p_q, p_calleraddr)
#  define OS_TRACE_MSGQ_SEND(p_q, p_calleraddr)
#  define OS_TRACE_MSGQ_RECV(p_q, p_calleraddr)
#endif

#ifdef CONFIG_SYSVIEW_MONITOR_MEMINFO
#  define OS_TRACE_MEM_ALLOC(p_mem, p_blk, p_calleraddr)        SEGGER_SYSVIEW_RecordU32x3(OS_TRACE_ID_MEM_ALLOC,                  (U32)p_mem,        (U32)p_blk,         (U32)p_calleraddr                                     )
#  define OS_TRACE_MEM_FREE(p_mem, p_calleraddr)                SEGGER_SYSVIEW_RecordU32x2(OS_TRACE_ID_MEM_FREE,                   (U32)p_mem,        (U32)p_calleraddr                                                         )
#else
#  define OS_TRACE_MEM_ALLOC(p_mem, p_blk, p_calleraddr)
#  define OS_TRACE_MEM_FREE(p_mem, p_calleraddr)
#endif

#ifdef CONFIG_SYSVIEW
#  ifndef SYSVIEW_GET_RETADDR
#  define SYSVIEW_GET_RETADDR \
	uint32_t retaddr = 0; \
	do { \
		asm volatile ("mov %0,lr\n" : "=r" (retaddr));\
	} while (0);
#  endif
#else
#  define SYSVIEW_GET_RETADDR
#endif

#ifdef CONFIG_SYSVIEW
#  define OS_TRACE_MSG(msg)                                       SEGGER_SYSVIEW_RecordString(OS_TRACE_ID_ANNOTATION_MSG, msg)
#  define OS_TRACE_VALUE(msg)                                     SEGGER_SYSVIEW_RecordU32(OS_TRACE_ID_ANNOTATION_VALUE, value)
#else
#  define OS_TRACE_MSG(msg)
#  define OS_TRACE_VALUE(msg)
#endif

/*
************************************************************************************************************************
*                                                   API Functions
************************************************************************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif
void  SYSVIEW_TaskReady      (U32 TaskID);
void  SYSVIEW_TaskSwitchedIn (U32 TaskID);
void  SYSVIEW_TaskSuspend    (U32 TaskID);
void  SYSVIEW_AddTask        (U32 TaskID, const char* NamePtr, U8 Prio, void *StkBasePtr, U32 StkSize);
void  SYSVIEW_UpdateTask     (U32 TaskID, const char* NamePtr, U8 Prio, void *StkBasePtr, U32 StkSize);
void  SYSVIEW_SendTaskInfo   (U32 TaskID, const char* sName, unsigned Prio, U32 StackBase, unsigned StackSize);
void  SYSVIEW_RecordU32x4    (unsigned Id, U32 Para0, U32 Para1, U32 Para2, U32 Para3);
void  SYSVIEW_RecordU32x5    (unsigned Id, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4);

void  SYSVIEW_RecordU32Register(unsigned EventId, U32 ResourceId, const char* sResource);
void  SYSVIEW_SendResourceList (void);

#ifdef __cplusplus
}
#endif
