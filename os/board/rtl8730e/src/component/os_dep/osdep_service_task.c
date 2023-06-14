/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <task.h>
#include <osdep_service.h>
#include <stdio.h>

static int wrapper_thread(int argc, char *argv[])
{
	int func_addr, ctx_addr;
	thread_func_t func;
	void *thctx;
	if (argc != 3) {
		DBG_ERR("%s error\n", argv[0]);
		return -1;
	}
	/* Arguments : [0] task name [1] func addr, [2] ctx addr */
	func_addr = strtoul(argv[1], NULL, 16);
	ctx_addr = strtoul(argv[2], NULL, 16);
	func = (thread_func_t)func_addr;
	thctx = (void *)ctx_addr;
	func(thctx);
	return OK;
}

int rtw_create_task(struct task_struct *ptask, const char *name,
					u32 stack_size, u32 priority, thread_func_t func, void *thctx)
{
	int func_addr, ctx_addr;
	pid_t pid;
	char str_func_addr[9];
	char str_ctx_addr[9];
	char *task_info[3];
	priority = SCHED_PRIORITY_DEFAULT + priority;
	priority = (priority > SCHED_PRIORITY_MAX || priority < SCHED_PRIORITY_MIN)?SCHED_PRIORITY_DEFAULT:priority;
	if(strncmp(name, "rtw_interrupt_thread", strlen("rtw_interrupt_thread") + 1) == 0) priority = 106;
	if(strncmp(name, "rtw_recv_tasklet", strlen("rtw_recv_tasklet") + 1) == 0) priority = 105;
	if(strncmp(name, "rtw_xmit_tasklet", strlen("rtw_xmit_tasklet") + 1) == 0) priority = 105;
	if(strncmp(name, "cmd_thread", strlen("cmd_thread") + 1) == 0) priority = 105;
	if(strncmp(name, "tcp_server_handler", strlen("tcp_server_handler") + 1) == 0) priority = 105;

	stack_size *= sizeof(uint32_t);
	func_addr = (int)func;
	ctx_addr = (int)thctx;
	task_info[0] = itoa(func_addr, str_func_addr, 16);
	task_info[1] = itoa(ctx_addr, str_ctx_addr, 16);
	task_info[2] = NULL;
	pid = kernel_thread(name, priority, stack_size, wrapper_thread, (char * const *)task_info);
	if (pid == ERROR) {
		DBG_ERR("%s fail\n", name);
		return _FAIL;
	}
	ptask->task = (pid_t)pid;
	ptask->task_name = name;
	return _SUCCESS;
}

void rtw_delete_task(struct task_struct *ptask)
{
	pid_t pid;
	int status;
	pid = (pid_t) ptask->task;
	status = task_delete(pid);
	if (status != OK) {
		DBG_ERR("Failed, status=%d!\n", status);
	}
	ptask->task = -1;
	return;
}

void rtw_set_priority_task(void *task, u32 NewPriority)
{
	FAR struct tcb_s *rtcb = sched_gettcb(*(pid_t *)task);
	DBG_INFO("\n");

	if (rtcb == NULL) {
		DBG_ERR("Failed to get main task %d!\n", *(pid_t *)task);
		return;
	}

	sched_setpriority(rtcb, NewPriority + SCHED_PRIORITY_DEFAULT);
	return;
}

int rtw_get_priority_task(void *task)
{
	FAR struct tcb_s *rtcb = sched_gettcb(*(pid_t *)task);
	DBG_INFO("\n");

	if (rtcb == NULL) {
		DBG_ERR("Failed to get main task %d!\n", *(pid_t *)task);
		return _FAIL;
	}

	return rtcb->sched_priority - SCHED_PRIORITY_DEFAULT;
}

void rtw_suspend_task(void *task)
{
	FAR struct tcb_s *rtcb = sched_gettcb(*(pid_t *)task);
	DBG_INFO("\n");

	if (rtcb == NULL) {
		DBG_ERR("Failed to get main task %d!\n", *(pid_t *)task);
		return;
	}

	return;
}

void rtw_resume_task(void *task)
{
	FAR struct tcb_s *rtcb = sched_gettcb(*(pid_t *)task);
	DBG_INFO("\n");

	if (rtcb == NULL) {
		DBG_ERR("Failed to get main task %d!\n", *(pid_t *)task);
		return;
	}

	return;
}

void rtw_suspend_task_all(void)
{
	DBG_INFO("\n");
	return;
}

void rtw_resume_task_all(void)
{
	DBG_INFO("\n");
	return;
}

void rtw_thread_enter(char *name)
{
	/* To avoid gcc warnings */
	DBG_INFO("RTKTHREAD %s\n", name);
}

void rtw_thread_exit()
{
	DBG_INFO("RTKTHREAD exit\n");
	exit(EXIT_SUCCESS);
}

u8 rtw_get_scheduler_state()
{
	DBG_INFO("\n");
	return 0;
}

void rtw_create_secure_context(u32 secure_stack_size)
{
	OSDEP_DBG("Not implement osdep service: rtw_create_secure_context");
}

void *rtw_get_current_TaskHandle(void)
{
	OSDEP_DBG("Not implement osdep service: rtw_get_current_TaskHandle");
	return NULL;
}

