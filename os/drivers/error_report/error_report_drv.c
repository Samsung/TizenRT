/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <errno.h>
#include <debug.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/clock.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/error_report_internal.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int error_report_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t err_report_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t err_report_write(FAR struct file *filep, FAR const char *buffer, size_t len);
/****************************************************************************
 * Private Data
 ****************************************************************************/
#define INFINITE_CHECK_HIGH_THRESHOLD	100
static const struct file_operations err_report_fops = {
	0,                          /* open */
	0,                          /* close */
	err_report_read,            /* read */
	err_report_write,           /* write */
	0,                          /* seek */
	error_report_ioctl          /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};

static create_infwait_err_rec g_create_err_rec;
static infinity_waitq_t g_thd_waitq[CONFIG_MAX_TASKS];
#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
static const char *g_exceptional_waits[] = {"tash", "lpwork", "hpwork", "LWIP_TCP/IP", "Network Error Reporting", "WPA Supplicant", "WLAN Driver mxmgmt", "WPA Ctrl Iface FIFO", "Wi-Fi API monitor"};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
#define INSERT_INFINITE_WAITQ(pos, tcb, cpu)        \
		do {                                        \
			g_thd_waitq[pos].pid = tcb->pid;        \
			g_thd_waitq[pos].task_state = tcb->task_state;\
			g_thd_waitq[pos].count = 1;             \
			g_thd_waitq[pos].active_cpu = cpu.active;\
			nwerr_vdbg("Inserted pid %d at pos %d\n", tcb->pid, pos);\
		} while (0)

#define DELETE_INFINITE_WAITQ(pos)                  \
		do {                                        \
			g_thd_waitq[pos].pid = 0;               \
			g_thd_waitq[pos].count = 0;             \
			g_thd_waitq[pos].task_state = 0;        \
			g_thd_waitq[pos].active_cpu = 0;        \
			nwerr_vdbg("Reset pos %d\n", pos);      \
		} while (0)
#endif

#define RETURN_ADDR_FROM_TCB(retval, tcbptr)        \
	do {                                            \
		if (tcbptr != NULL) {                       \
			entry_t e = tcbptr->entry;              \
			if ((tcbptr->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD) {\
				retval = (unsigned long)e.pthread;  \
			} else {                                \
				retval = (unsigned long)e.main;     \
			}                                       \
		} else {                                    \
			retval = 0;                             \
		}                                           \
	} while (0)

static int prv_is_reserved(const char *taskname)
{
	int i;
	if (taskname == NULL) {
		return -1;
	}
	for (i = 0; i < sizeof(g_exceptional_waits) / sizeof(g_exceptional_waits[0]); i++) {
		if (!strncmp(taskname, g_exceptional_waits[i], strlen(taskname))) {
			return 1;
		}
	}
	return 0;
}

static ssize_t err_report_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t err_report_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

static unsigned long prv_fetch_taskaddr(int pid)
{
	unsigned long retval;
	struct tcb_s *tcbptr = sched_gettcb(pid);
	RETURN_ADDR_FROM_TCB(retval, tcbptr);
	return retval;
}

static void prv_tcb_handler(FAR struct tcb_s *tcb, void *args)
{
	int pos = 0;
	if ((tcb->task_state >= TSTATE_WAIT_SEM) && (tcb->task_state < NUM_TASK_STATES)) {
		int ret;
		struct cpuload_s cpu;
		ret = clock_cpuload(tcb->pid, 0, &cpu);
		if (ret != OK) {
			return;
		}       
		/* Hash pid over the waitq range */
		pos = tcb->pid % CONFIG_MAX_TASKS;
		if ((g_thd_waitq[pos].pid == tcb->pid)) {
			if (g_thd_waitq[pos].task_state == tcb->task_state) {
				if (g_thd_waitq[pos].active_cpu == cpu.active) {
					int threshold;
					g_thd_waitq[pos].count++;
					if (prv_is_reserved(tcb->name) == 1) {
						threshold = INFINITE_CHECK_HIGH_THRESHOLD;
					} else {
						threshold = CONFIG_ERROR_REPORT_INFINITE_CHECK_THRESHOLD;
					}
					
					nwerr_vdbg("Inc wait count for pid %d\n", tcb->pid);
					if (g_thd_waitq[pos].count >= threshold) {
						// Add code for error report addition
						uint32_t task_addr;
						RETURN_ADDR_FROM_TCB(task_addr, tcb);
						nwerr_vdbg("pid %d blocked\n", tcb->pid);
						g_create_err_rec(tcb->pid, tcb->task_state, task_addr);
						/* Reset the count, indicating that is free for insert */
						DELETE_INFINITE_WAITQ(pos);
					}
				} else {
					/* The thread was running in between, and blocked again */
					INSERT_INFINITE_WAITQ(pos, tcb, cpu);
				}
			} else {
				/* Ideally does not happen, but we account for it anyway */
				INSERT_INFINITE_WAITQ(pos, tcb, cpu);
			}
		} else if (!g_thd_waitq[pos].pid) {
			/* Add new entry */
			INSERT_INFINITE_WAITQ(pos, tcb, cpu);
		} else {
			/* Collision resolution, find the next empty slot */
			int i;
			for (i = pos+1; i != pos; i = (i + 1) % CONFIG_MAX_TASKS) {
				if (g_thd_waitq[i].pid == 0) {
					/* Found an empty slot, insert values here */
					INSERT_INFINITE_WAITQ(i, tcb, cpu);
					break;
				}
			}
			if (i == pos) {
				nwerrdbg("Cannot find a slot to insert values\n");
			}
		}
	} else {
		/* Maybe the pid was blocked during the 
		 * last sampling instance, and 
		 * is active again. Reset the data 
		 * in this case. 
		 */
		/* Hash pid over the waitq range */
		pos = tcb->pid % CONFIG_MAX_TASKS;
		if ((g_thd_waitq[pos].pid == tcb->pid)) {
			/* Reset the count, indicating that is free for insert */
			DELETE_INFINITE_WAITQ(pos);
		} else if (g_thd_waitq[pos].pid) {
			/* Collision resolution, find the slot using linear search */
			int i;
			for (i = pos+1; i != pos; i = (i + 1) % CONFIG_MAX_TASKS) {
				if (g_thd_waitq[i].pid == tcb->pid) {
					/* Found an empty slot, insert values here */
					DELETE_INFINITE_WAITQ(i);
					break;
				}
			}
		}
	}
}



/************************************************************************************
 * Name: error_report_ioctl
 *
 * Description:  The ioctl method for error reporting.
 *
 ************************************************************************************/
static int error_report_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	nwerr_vdbg("cmd: %d arg: %ld\n", cmd, arg);
	/* Handle built-in ioctl commands */
	switch (cmd) {
	case ERIOC_GET_ENTRY:
	{
		thread_entry_t *thd = (thread_entry_t *) ((uintptr_t)arg);
		thd->entry = prv_fetch_taskaddr(thd->pid);
		ret = OK;
	}
	break;
	case ERIOC_CHECK_INFWAIT:
		sched_foreach(prv_tcb_handler, NULL);
		ret = OK;
	break;
	case ERIOC_GET_BACKTRACE:
	{
		infinty_waitdata_t *waitdata = (infinty_waitdata_t *) ((uintptr_t)arg);
		struct tcb_s *tcb = sched_gettcb(waitdata->pid);
		if (tcb == NULL) {
			break;
		}
		waitdata->ncalls = 0;
		/* Read the functions from the callstack into waitdata structure */
		uint32_t *ptr;
		ptr = (uint32_t *)tcb->xcp.regs[13];
		while ((void *)ptr < tcb->adj_stack_ptr && (waitdata->ncalls < CONFIG_ERROR_REPORT_BACKTRACE_MAX_DEPTH)) {
			if (*ptr >= 0x04000000 && *ptr <= 0x04800000) {
				waitdata->backtrace[waitdata->ncalls] = *ptr - 4;
				nwerr_vdbg("Stack: 0x%08x\n", *ptr - 4);
				waitdata->ncalls++;
			}
			ptr++;
		}
		/* Fill out the remaining slots in waitdata with 0 */
		while (waitdata->ncalls < CONFIG_ERROR_REPORT_BACKTRACE_MAX_DEPTH) {
			waitdata->backtrace[waitdata->ncalls] = 0;
			waitdata->ncalls++;
		}
		waitdata->task_state = tcb->task_state;
		RETURN_ADDR_FROM_TCB(waitdata->entry, tcb);
		ret = OK;
	}
	break;
	case ERIOC_SET_CALLBACK:
	{
		create_infwait_err_rec cb = (create_infwait_err_rec) ((uintptr_t)arg);
		g_create_err_rec = cb;
		ret = OK;
	}
	break;
	default:
		nwerrdbg("Unrecognized cmd: %d arg: %ld\n", cmd, arg);
	break;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: error_report_drv_register
 *
 * Description:
 *   Register task management path, ERROR_REPORT_DRVPATH. This function is
 *   called within the error reporting module, during initialization.
 *   The callback is implemented in the error report module, and passed as
 *   an argument to this function. when the error report driver infers the
 *   presence of long waiting threads, it signals the error reporting module
 *   using the registered callback.
 * @param[in] callback function with signature type create_infwait_err_rec.
 *   Used for reporting infinitely waiting threads.
 ****************************************************************************/

void error_report_drv_register(void)
{
	(void)register_driver(ERROR_REPORT_DRVPATH, &err_report_fops, 0666, NULL);
}
