/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_vfork.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/sched.h>
#include <tinyara/arch.h>
#include <arch/irq.h>

#include "up_vfork.h"
#include "up_internal.h"
#include "sched/sched.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_vfork_archprepare
 *
 * Description:
 *   Prepare the ARMv7-A full-descending child stack and relocate the
 *   register-save frame.  The common up_vfork() orchestration supplies the
 *   TCB and performs the shared register and task-start work.
 *
 ****************************************************************************/

int up_vfork_archprepare(struct tcb_s *parent, struct task_tcb_s *child, const struct vfork_s *context, struct up_vfork_stack_s *stack)
{
	size_t stackframe;
	uint32_t newsp;
	uint32_t newfp;
	uint32_t newtop;
	uint32_t stacktop;
	uint32_t stackutil;
	FAR void *child_frame;

	/* The parent argv[] frame is below stack_base_ptr.  Keep an equal frame
	 * in the child so the argv relocation remains inside the child stack.
	 */
	stackframe = (uintptr_t)parent->stack_base_ptr - (uintptr_t)parent->stack_alloc_ptr;

	if (stackframe > 0) {
		/* ARMv7-A's up_stack_frame() reserves space at the bottom of the
		 * full-descending stack without moving xcp.regs.  Copy the parent's
		 * argv[] frame before vfork_stackargsetup() adjusts its pointers.
		 */
		child_frame = up_stack_frame((FAR struct tcb_s *)child, stackframe);
		if (child_frame == NULL) {
			return -ENOMEM;
		}

		memcpy(child_frame, parent->stack_alloc_ptr, stackframe);
	}

	/* How much of the parent's stack was utilized?  The ARM uses
	 * a push-down stack so that the current stack pointer should
	 * be lower than the initial, adjusted stack pointer.  The
	 * stack usage should be the difference between those two.
	 */

	stacktop = (uint32_t)parent->stack_base_ptr + parent->adj_stack_size;
	DEBUGASSERT(stacktop > context->sp);
	stackutil = stacktop - context->sp;

	svdbg("Parent: stackutil:%" PRIu32 "\n", stackutil);

	/* Make some feeble effort to preserve the stack contents.  This is
	 * feeble because the stack surely contains invalid pointers and other
	 * content that will not work in the child context.  However, if the
	 * user follows all of the caveats of vfork() usage, even this feeble
	 * effort is overkill.
	 */

	newtop = (uint32_t)child->cmn.stack_base_ptr + child->cmn.adj_stack_size;

	newsp = newtop - stackutil;

	/* Move the register context to newtop. */

	memcpy((void *)(newsp - XCPTCONTEXT_SIZE), child->cmn.xcp.regs, XCPTCONTEXT_SIZE);

	child->cmn.xcp.regs = (void *)(newsp - XCPTCONTEXT_SIZE);

	memcpy((void *)newsp, (const void *)context->sp, stackutil);

	/* Was there a frame pointer in place before? */

	if (context->fp >= context->sp && context->fp < stacktop) {
		uint32_t frameutil = stacktop - context->fp;
		newfp = newtop - frameutil;
	} else {
		newfp = context->fp;
	}

	svdbg("Old stack top:%08" PRIx32 " SP:%08" PRIx32 " FP:%08" PRIx32 "\n", stacktop, context->sp, context->fp);
	svdbg("New stack top:%08" PRIx32 " SP:%08" PRIx32 " FP:%08" PRIx32 "\n", newtop, newsp, newfp);

	stack->newsp = newsp;
	stack->newfp = newfp;
	stack->parent_sp = context->sp;
	return OK;
}
