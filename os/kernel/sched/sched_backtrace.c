/****************************************************************************
 * sched/sched/sched_backtrace.c
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/sched.h>
#include <tinyara/arch.h>
#include <tinyara/spinlock.h>

#include "sched/sched.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

#ifdef CONFIG_SMP
struct backtrace_arg_s {
  FAR void **buffer;  /* Buffer to store backtrace */
  int size;           /* Size of buffer */
  int skip;           /* Number of frames to skip */
  int stacksize;      /* Number of frames captured */
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_SMP
/****************************************************************************
 * Name: sched_backtrace_handler
 *
 * Description:
 *   This function is called from the IPI handler to get the backtrace
 *   of the running task on the target CPU.
 *
 ****************************************************************************/

static int sched_backtrace_handler(FAR void *cookie)
{
  FAR struct backtrace_arg_s *arg = cookie;
  FAR struct tcb_s *tcb;

  /* Get the running task */

  tcb = this_task();

  /* Get the backtrace - asserted_location=0 for normal backtrace */

  arg->stacksize = up_backtrace(tcb, arg->buffer, arg->size, arg->skip, 0);

  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_backtrace
 *
 * Description:
 *   Get thread backtrace from specified tid.
 *   Store up to SIZE return address of the current program state in
 *   the array pointed to by BUFFER.
 *
 * Input Parameters:
 *   tid   - Task ID to get backtrace
 *   buffer - Buffer to store backtrace
 *   size   - Size of buffer
 *   skip   - Number of frames to skip
 *
 * Returned Value:
 *   Number of frames captured, or negative error code on failure
 *
 ****************************************************************************/

int sched_backtrace(pid_t tid, FAR void **buffer, int size, int skip)
{
  FAR struct tcb_s *tcb;
  int ret = 0;

  if (size <= 0 || !buffer)
    {
      return -EINVAL;
    }

  /* Get the TCB */

  tcb = sched_gettcb(tid);
  if (!tcb)
    {
      return -ESRCH;
    }

#ifdef CONFIG_SMP
  /* If the task is running on another CPU, we need to use IPI */

  if (tcb->cpu != this_cpu() && tcb->task_state == TSTATE_TASK_RUNNING)
    {
      /* IPI-based backtrace not supported in TizenRT */
      lldbg("Backtrace: Task %d running on CPU %d, IPI not supported\n", tid, tcb->cpu);
      return 0;
    }
#endif
/*TODO*/
#if 0
  /* If the task is running on another CPU, we need to use IPI */

  if (tcb->cpu != this_cpu() && tcb->task_state == TSTATE_TASK_RUNNING)
    {
      struct backtrace_arg_s arg;

      arg.buffer  = buffer;
      arg.size    = size;
      arg.skip    = skip;
      arg.stacksize = 0;

      /* Send IPI to the target CPU */

      up_send_sipi(tcb->cpu, sched_backtrace_handler, &arg);

      return arg.stacksize;
    }
#endif

  /* Task is not running or on the same CPU, we can safely get the backtrace */
  /* asserted_location=0 for normal backtrace (not from ASSERT) */

  ret = up_backtrace(tcb, buffer, size, skip, 0);

  return ret;
}

#ifdef CONFIG_ARCH_STACKDUMP
/****************************************************************************
 * Name: sched_dumpstack
 *
 * Description:
 *   Dump the stack of the specified task.
 *
 ****************************************************************************/

void sched_dumpstack(pid_t tid)
{
  FAR struct tcb_s *tcb;
#ifdef CONFIG_ARCH_STACKDUMP_DEPTH
  #ifdef CONFIG_ARCH_STACKDUMP_DEPTH
  void *buffer[CONFIG_ARCH_STACKDUMP_DEPTH];
  int size;
  int i;

  tcb = sched_gettcb(tid);
  if (!tcb)
    {
      _lldbg("Task %d not found\n", tid);
      return;
    }

  size = sched_backtrace(tid, buffer, CONFIG_ARCH_STACKDUMP_DEPTH, 0);
  if (size < 0)
    {
      _lldbg("Failed to get backtrace for task %d: %d\n", tid, size);
      return;
    }

  _lldbg("Backtrace for task %d (%s):\n", tid, tcb->name);
  #endif
  for (i = 0; i < size; i++)
    {
      _lldbg("  [%d]: %p\n", i, buffer[i]);
    }
#else
  /* Fallback when CONFIG_ARCH_STACKDUMP_DEPTH is not defined */
  tcb = sched_gettcb(tid);
  if (!tcb)
    {
      _lldbg("Task %d not found\n", tid);
      return;
    }

  _lldbg("Backtrace for task %d (%s) not available (CONFIG_ARCH_STACKDUMP_DEPTH undefined)\n", tid, tcb->name);
#endif
}
#endif
