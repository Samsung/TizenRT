/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/sched.h>
#include <tinyara/config.h>
#include <tinyara/mm/mm.h>

#ifdef CONFIG_DEBUG_MM_HEAPINFO
static void kdbg_heapinfo_init(FAR struct tcb_s *tcb, FAR void *arg)
{

	tcb->peak_alloc_size = 0;

	printf("PID %d, peak allocated heap information is cleared\n", tcb->pid);
}

static void kdbg_heapinfo_task(FAR struct tcb_s *tcb, FAR void *arg)
{
	if (tcb->pid == 0) {
		tcb->adj_stack_size = CONFIG_IDLETHREAD_STACKSIZE;
	}
	printf("%3d | %5d |     %5d  %5d | ", tcb->pid, tcb->adj_stack_size, tcb->curr_alloc_size, tcb->peak_alloc_size);

	/* Show task name and arguments */
#if CONFIG_TASK_NAME_SIZE > 0
	printf("%s(", tcb->name);
#else
	printf("<noname>(");
#endif
	printf(")\n");
}
#endif
int kdbg_heapinfo(int argc, char **args)
{
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	struct mm_heap_s *user_heap = mm_get_heap_info();
	if (argc > 1 && strcmp(args[1], "init") == 0) {
		sched_foreach(kdbg_heapinfo_init, NULL);

		printf("Peak allocated memory size is cleared\n");
		return OK;
	}

	heapinfo_parse(user_heap);

	printf("PID | STACK | HEAP Curr | Peak | NAME\n");

	sched_foreach(kdbg_heapinfo_task, NULL);
#endif
	return OK;
}

