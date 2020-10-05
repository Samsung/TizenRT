/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * mm/mm_heap/mm_heapinfo_group.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>
#include <stdint.h>
#include <string.h>
#include <debug.h>
#include <tinyara/mm/heapinfo_internal.h>

int heapinfo_max_group;
struct heapinfo_group_s heapinfo_group[HEAPINFO_USER_GROUP_NUM];
struct heapinfo_group_info_s group_info[HEAPINFO_THREAD_NUM];

/****************************************************************************
 * Name: heapinfo_update_group
 *
 * Description:
 * Update Peak heap size for Group
 ****************************************************************************/
void heapinfo_update_group(mmsize_t size, pid_t pid)
{
	int check_idx;
	int group_num;
	int stack_pid;

	if (pid == HEAPINFO_INIT_INFO) {
		/* Invalid PID */
		mdbg("Invalid PID : %d\n", pid);
		return;
	}
	for (check_idx = 0; check_idx < HEAPINFO_THREAD_NUM; check_idx++) {
		if (pid == group_info[check_idx].pid) {
			group_num = group_info[check_idx].group;
			heapinfo_group[group_num].curr_size += size;
			/* Update peak size */
			if (heapinfo_group[group_num].peak_size < heapinfo_group[group_num].curr_size) {
				heapinfo_group[group_num].peak_size = heapinfo_group[group_num].curr_size;
				/* calculate the summation of stacks */
				heapinfo_group[group_num].stack_size = 0;
				for (stack_pid = 0; stack_pid < HEAPINFO_THREAD_NUM; stack_pid++) {
					if (group_info[stack_pid].pid != HEAPINFO_INIT_INFO && group_info[stack_pid].group == group_num) {
						heapinfo_group[group_num].stack_size += group_info[stack_pid].stack_size;
					}
				}
				heapinfo_group[group_num].heap_size = heapinfo_group[group_num].peak_size - heapinfo_group[group_num].stack_size;
			}
			break;
		}
	}
}

/****************************************************************************
 * Name: heapinfo_update_group_info
 *
 * Description:
 * when create or release task/thread, check that the task/thread is 
 * in group list
 ****************************************************************************/
void heapinfo_update_group_info(pid_t pid, int group, int type)
{
	int info_idx;
	struct tcb_s *tcb;
	switch (type) {
	case HEAPINFO_INIT_INFO:
		for (info_idx = 0; info_idx < HEAPINFO_THREAD_NUM; info_idx++) {
			group_info[info_idx].pid = pid;
			group_info[info_idx].group = group;
			group_info[info_idx].stack_size = 0;
		}
		break;
	case HEAPINFO_ADD_INFO:
		if (group > heapinfo_max_group) {
			heapinfo_max_group = group;
		}
		for (info_idx = 0; info_idx < HEAPINFO_THREAD_NUM; info_idx++) {
			if (group_info[info_idx].pid <= 0) {
				group_info[info_idx].pid = pid;
				group_info[info_idx].group = group;
				tcb = sched_gettcb(pid);
				if (tcb) {
					group_info[info_idx].stack_size = tcb->adj_stack_size;
					heapinfo_update_group(tcb->adj_stack_size, pid);
				}
				break;
			}
		}
		break;
	case HEAPINFO_DEL_INFO:
		for (info_idx = 0; info_idx < HEAPINFO_THREAD_NUM; info_idx++) {
			if (pid == group_info[info_idx].pid) {
				heapinfo_update_group((-1) * group_info[info_idx].stack_size, pid);

				group_info[info_idx].pid = -1;
				group_info[info_idx].group = -1;
				group_info[info_idx].stack_size = 0;
				break;
			}
		}
		break;
	default:
		break;
	}
}

/****************************************************************************
 * Name: heapinfo_check_group_list
 *
 * Description:
 * check that task/thread is in group list
 ****************************************************************************/
void heapinfo_check_group_list(pid_t pid, char *name)
{
	char *thread_list = CONFIG_HEAPINFO_USER_GROUP_LIST;

	int group_num = 0;

	char *name_start;
	char *name_end;
	int name_length;
	name_start = name_end = thread_list;

	while (*name_start != '\0') {
		if (*name_end == '/' || *name_end == ',' || *name_end == '\0') {
			name_length = name_end - name_start;
			if (name_length == strlen(name)) {
				if (strncmp(name_start, name, name_end - name_start) == 0) {
					heapinfo_update_group_info(pid, group_num, HEAPINFO_ADD_INFO);
					break;
				}
			}
			if (*name_end == '/') {
				group_num++;
			} else if (*name_end == '\0') {
				name_start = name_end;
				continue;
			}
			name_end++;
			name_start = name_end;
		} else {
			name_end++;
		}
	}
}
