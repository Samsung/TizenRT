/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba.h"
#include "os_wrapper.h"

int rtos_sched_start(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_sched_stop(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_sched_suspend(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_sched_resume(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_sched_get_state(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

static int wrapper_thread(int argc, char *argv[])
{
	void (*p_routine)(void *);
	void *p_param;

	if (argc != 3) {
		dbg("%s wrapper_thread error\n", argv[0]);
		return FAIL;
	}

	p_routine = (void *)strtoul(argv[1], NULL, 16);
	p_param = (void *)strtoul(argv[2], NULL, 16);

	p_routine(p_param);

	return SUCCESS;
}

int rtos_task_create(rtos_task_t *pp_handle, const char *p_name, void (*p_routine)(void *),
					 void *p_param, uint16_t stack_size_in_byte, uint16_t priority)
{
	pid_t pid;
	/* The following character array of size 9 is used to store pointer address in hex represented as ascii.
	   As address has 32-bit integer value, the maximum string length in hex is 8 with 1 null character.*/
	char routine_addr[9], param_addr[9];
	char *task_info[3];

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return FAIL;
	}

	if (*pp_handle) {
		dbg("%s %p\n", p_name, *pp_handle);
		dbg("task already init\n");
		return FAIL;
	}

	task_info[0] = itoa((int)p_routine, routine_addr, 16);
	task_info[1] = itoa((int)p_param, param_addr, 16);
	task_info[2] = NULL;
	stack_size_in_byte *= sizeof(uint32_t);
	priority = priority + SCHED_PRIORITY_DEFAULT;
	priority = (priority > SCHED_PRIORITY_MAX || priority < SCHED_PRIORITY_MIN)?SCHED_PRIORITY_DEFAULT:priority;

	pid = kernel_thread(p_name, priority, (int)stack_size_in_byte, wrapper_thread, (char *const *)task_info);
	if (pid == ERROR) {
		dbg("%s create fail\n", p_name);
		return FAIL;
	}

	*pp_handle = (rtos_task_t)((uint32_t)pid);

	return SUCCESS;
}

int rtos_task_delete(rtos_task_t p_handle)
{
	pid_t pid = (pid_t)((uint32_t)p_handle);

	if (task_delete(pid) != OK) {
		dbg("delete task 0x%x fail\n", p_handle);
		return FAIL;
	}

	return SUCCESS;
}

int rtos_task_suspend(rtos_task_t p_handle)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_task_resume(rtos_task_t p_handle)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_task_yield(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

rtos_task_t rtos_task_handle_get(void)
{
	return (rtos_task_t)((uint32_t)getpid());
}

uint32_t rtos_task_priority_get(rtos_task_t p_handle)
{
	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return 0;
	}

	pid_t pid = (*(pid_t *)p_handle);
	struct tcb_s *p_tcb = sched_gettcb(pid);

	if (!p_tcb) {
		dbg("p_tcb is NULL\n");
		return 0;
	}

	uint32_t priority = p_tcb->sched_priority - SCHED_PRIORITY_DEFAULT;

	return priority;
}

int rtos_task_priority_set(rtos_task_t p_handle, uint16_t priority)
{
	struct tcb_s *p_tcb;
	pid_t pid = (*(pid_t *)p_handle);

	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	p_tcb = sched_gettcb(pid);
	if (!p_tcb) {
		dbg("pid %d tcb is NULL\n", pid);
		return FAIL;
	}

	priority = priority + SCHED_PRIORITY_DEFAULT;
	priority = (priority > SCHED_PRIORITY_MAX || priority < SCHED_PRIORITY_MIN)?SCHED_PRIORITY_DEFAULT:priority;

	if (sched_setpriority(p_tcb, priority) != OK) {
		dbg("sched setpriority fail\n");
		return FAIL;
	}

	return SUCCESS;
}

void rtos_create_secure_context(uint32_t size)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
}
