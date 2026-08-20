/****************************************************************************
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
 * lib/libc/sched/sched_backtrace.c
 *
 * Ported from NuttX (libs/libc/sched/sched_backtrace.c).
 *
 * Returns the call-stack (return addresses) of the CURRENT thread using the
 * compiler-generated EHABI unwind tables (.ARM.exidx) via libgcc's
 * _Unwind_Backtrace.  This is precise (no frame pointer, no stack-scan
 * heuristic) and runs entirely in the caller's context - no syscall trap.
 *
 * Requires the unwound code to be compiled with unwind tables
 * (-funwind-tables); CONFIG_SCHED_BACKTRACE turns that on.
 *
 * Note: only the current thread is supported (tid must be the caller).
 * Backtracing another (sleeping) thread needs its saved context and is not
 * provided by this user-space implementation.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_SCHED_BACKTRACE

#include <sys/types.h>
#include <unistd.h>
#include <unwind.h>

/****************************************************************************
 * Private Data Types
 ****************************************************************************/

struct trace_arg {
	FAR void **array;	/* Caller-supplied buffer for return addresses */
	_Unwind_Word cfa;	/* Canonical Frame Address of the previous frame */
	int cnt;		/* Number of entries stored so far (starts negative
				 * to honour 'skip') */
	int size;		/* Capacity of 'array' */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Allow arch-specific adjustment of a return address (weak: identity). */

__attribute__((weak))
FAR void *unwind_arch_adjustment(FAR void *prev, FAR void *addr)
{
	(void)prev;
	return addr;
}

/****************************************************************************
 * Name: backtrace_helper
 *
 * Description:
 *   Called by _Unwind_Backtrace once per stack frame.  Records the return
 *   address (instruction pointer) of each frame into the caller's buffer.
 ****************************************************************************/

static _Unwind_Reason_Code backtrace_helper(FAR struct _Unwind_Context *ctx, FAR void *a)
{
	FAR struct trace_arg *arg = a;

	/* We are first called with the address inside this backtrace machinery;
	 * negative 'cnt' skips those (and any user-requested 'skip') frames.
	 */

	if (arg->cnt >= 0) {
		arg->array[arg->cnt] = (FAR void *)_Unwind_GetIP(ctx);
		if (arg->cnt > 0) {
			arg->array[arg->cnt] = unwind_arch_adjustment(arg->array[arg->cnt - 1], arg->array[arg->cnt]);
		}

		/* Detect lack of progress (same IP + CFA) -> end of stack. */

		_Unwind_Word cfa = _Unwind_GetCFA(ctx);

		if (arg->cnt > 0 && arg->array[arg->cnt - 1] == arg->array[arg->cnt] && cfa == arg->cfa) {
			return _URC_END_OF_STACK;
		}

		arg->cfa = cfa;
	}

	if (++arg->cnt >= arg->size) {
		return _URC_END_OF_STACK;
	}

	return _URC_NO_REASON;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_backtrace
 *
 * Description:
 *   Get the backtrace (return addresses) of the specified thread.  Only the
 *   calling thread (tid == getpid()) is supported.
 *
 * Input Parameters:
 *   tid    - Thread ID (must be the current thread)
 *   buffer - Where the return addresses are stored
 *   size   - Maximum number of addresses to store
 *   skip   - Number of innermost frames to omit
 *
 * Returned Value:
 *   The number of addresses stored in 'buffer', or 0 on failure.
 *
 ****************************************************************************/

int sched_backtrace(pid_t tid, FAR void **buffer, int size, int skip)
{
	struct trace_arg arg;

	if (buffer == NULL || size <= 0) {
		return 0;
	}

	/* This user-space implementation can only unwind its own stack. */

	if (tid != getpid()) {
		return 0;
	}

	arg.array = buffer;
	arg.cfa = 0;
	arg.size = size;
	arg.cnt = -skip - 1;

	_Unwind_Backtrace(backtrace_helper, &arg);

	/* _Unwind_Backtrace tends to leave a trailing NULL above _start. */

	if (arg.cnt > 1 && arg.array[arg.cnt - 1] == NULL) {
		--arg.cnt;
	}

	return arg.cnt > 0 ? arg.cnt : 0;
}

#endif /* CONFIG_SCHED_BACKTRACE */
