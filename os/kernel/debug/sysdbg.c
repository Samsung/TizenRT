/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/***************************************************************************
 * Included Files
 ****************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/debug/sysdbg.h>
#include <tinyara/clock.h>
#include <sys/types.h>
#include <stdint.h>
#include <tinyara/fs/fs.h>
#include <stdbool.h>
#include <assert.h>
#include <semaphore.h>
#include "sched/sched.h"

/* Kindly update the debug_version as and when changes done in struct
 * sysdbg_struct as there will be dependency on T32 scripts and
 * python script to handle the structure */
volatile int g_debug_version = 1;

/***************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int sysdbg_open(FAR struct file *filep);
static int sysdbg_close(FAR struct file *filep);
static ssize_t sysdbg_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static void sysdbg_monitor_enable(void);
static void sysdbg_monitor_disable(void);
static void sysdbg_print(void);

#ifdef CONFIG_TASK_SCHED_HISTORY
static void update_maxtask_count(int count);
static uint32_t max_task_count = CONFIG_DEBUG_TASK_MAX_COUNT;
#endif
#ifdef CONFIG_IRQ_SCHED_HISTORY
static void update_maxirq_count(int count);
static uint32_t max_irq_count = CONFIG_DEBUG_IRQ_MAX_COUNT;
#endif
#ifdef CONFIG_SEMAPHORE_HISTORY
static void update_maxsem_count(int count);
static uint32_t max_sem_count = CONFIG_DEBUG_SEM_MAX_COUNT;
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

static const struct file_operations g_sysdbgops = {
	sysdbg_open,					/* open  */
	sysdbg_close,					/* close */
	0,						/* read  */
	sysdbg_write,					/* write */
	0,						/* seek  */
	0,						/* ioctl */
	0						/* poll  */
};


/****************************************************************************
 * Private data
 ****************************************************************************/

FAR sysdbg_t *sysdbg_struct = NULL;
static bool sysdbg_monitor = false;
static int32_t sysdbg_dev_opened;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
* Name: simulate_data_abort
*
* Description:
*   This function simulates the data abort with null pointer access
*
* Inputs:
*   None
*
* Return Value:
*   None
*
* Assumptions:
*   None
****************************************************************************/

static void simulate_data_abort(void)
{
	int *p = NULL;
	dbg("\nSimulating Data abort exception by Null pointer access\n");
	*p = 0x0;
}

/****************************************************************************
* Name: simulate_user_assert
*
* Description:
*   This function simulates the user assert by calling up_assert through PANIC macro
*
* Inputs:
*   None
*
* Return Value:
*   None
*
* Assumptions:
*   None
****************************************************************************/

static void simulate_user_assert(void)
{
	dbg("\nSimulating user assert\n");
	PANIC();
}

/****************************************************************************
* Name: simulate_prefetch_abort
*
* Description:
*   This function simulates the prefetch abort by accessing the unreachable addr
*
* Inputs:
*   None
*
* Return Value:
*   None
*
* Assumptions:
*   None
*
****************************************************************************/

static void simulate_prefetch_abort(void)
{
	dbg("\nSimulating Prefetch abort by accessing unreachable address \n");

	((void (*)(void))0xDEADBEEF)();
}

/****************************************************************************
* Name: simulate_undef_abort
*
* Description:
*   This function simulates the undefined Instruction exception by executing invalid instruction
*
* Inputs:
*   None
*
* Return Value:
*   None
*
* Assumptions:
*   None
*
****************************************************************************/

static void simulate_undef_abort(void)
{
	dbg("\nSimulating Undefined instruction exception\n");
	/*  0xe7f0def0 is an undefined instruction for both Arm and Thumb mode */
	__asm__ __volatile__(".word 0xe7f0def0\n");
}

/****************************************************************************
 * Name:  show_usage
 *
 * Description:
 *   This function prints the usage of sysdbg tash command
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void show_usage(void)
{
	dbg("USAGE:\n");
	dbg("sysdbg enable_monitor  /* Allocates memory and enables monitoring */\n");
	dbg("sysdbg disable_monitor /* Frees the memory and disables monitoring */\n");
	dbg("sysdbg read   /* Reads /dev/sysdbg to view debug information*/\n");
	dbg("sysdbg max_task_count <count> /* Enter max_task_count */\n");
	dbg("sysdbg max_irq_count <count> /* Enter max_irq_count */\n");
	dbg("sysdbg max_sem_count <count> /* Enter max_sem_count */\n");
	dbg("sysdbg data_abort /* Simulate data abort */\n");
	dbg("sysdbg prefetch_abort /* Simulate Prefetch abort */\n");
	dbg("sysdbg undef_abort  /* Simulate Undefined abort */\n");
	dbg("sysdbg usr_assert /* Simulate User Assert */\n");
	dbg("sysdbg dump_stack /* Dumps the current task stack */\n");
	dbg("sysdbg dump_allstack /* Dumps the stack of all tasks */\n");
	dbg("sysdbg help /* Display the help */\n");
}

/****************************************************************************
 * Name: sysdbg_open
 *
 * Description:
 *   fops to open the single instance of debug device
 *
 * Inputs:
 *   file pointer
 *
 * Return Value:
 *   If success OK else EBUSY
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
static int sysdbg_open(FAR struct file *filep)
{
	if (sysdbg_dev_opened) {
		dbg("sysdbg device busy\n");
		return -EBUSY;
	}

	sysdbg_dev_opened = 1;

	return OK;
}

/****************************************************************************
 * Name: sysdbg_close
 *
 * Description:
 *   fops to close the debug device
 *
 * Inputs:
 *   file pointer
 *
 * Return Value:
 *   If success OK else ERROR (-1)
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static int sysdbg_close(FAR struct file *filep)
{
	if (!sysdbg_dev_opened) {
		dbg("sysdbg device close failed\n");
		return ERROR;
	}

	sysdbg_dev_opened = 0;

	return OK;
}

/****************************************************************************
 * Name: sysdbg_print
 *
 * Description:
 *   Read the saved debug data and display on console
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void sysdbg_print(void)
{
#if defined(CONFIG_TASK_SCHED_HISTORY) || defined(CONFIG_IRQ_SCHED_HISTORY) || defined(CONFIG_SEMAPHORE_HISTORY)
	uint32_t idx = 0;
#endif
	irqstate_t saved_state;
	if (!sysdbg_monitor) {
		dbg("sysdbg monitoring is not enabled, kindly enable\n");
		return;
	}

	if (!sysdbg_struct) {
		dbg("sysdbg_struct is NULL\n");
		return;
	}

	saved_state = enter_critical_section();
#ifdef CONFIG_TASK_SCHED_HISTORY
	lldbg("Displaying the TASK SCHEDULING HISTORY for %d count\n", max_task_count);
	lldbg("*****************************************************************************\n");
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("* TASK_SCHEDTIME\t\tTASK_NAME\t PID\t PRIORITY\t TCB\n");
#else
	lldbg("* TASK_SCHEDTIME\t PID\t PRIORITY\t TCB\n");
#endif
	lldbg("*****************************************************************************\n");
	idx = sysdbg_struct->task_lastindex;
	do {
#if CONFIG_TASK_NAME_SIZE > 0
		lldbg("%8lld%31s%10d%10d%16X\n",
#else
		lldbg("%8lld%14d%10d%16X\n",
#endif
			  (uint64_t)sysdbg_struct->sched[idx].time,
#if CONFIG_TASK_NAME_SIZE > 0
			  sysdbg_struct->sched[idx].task,
#endif
			  sysdbg_struct->sched[idx].pid, sysdbg_struct->sched[idx].sched_priority, sysdbg_struct->sched[idx].ptcb);
		idx--;
		/* Keeping it circular buffer */
		idx = idx & (max_task_count - 1);
	} while (idx != sysdbg_struct->task_lastindex);
#else
	lldbg("CONFIG_TASK_SCHED_HISTORY is not enabled to view task history");
#endif							/* End of CONFIG_TASK_SCHED_HISTORY */

#ifdef CONFIG_IRQ_SCHED_HISTORY
	lldbg("Displaying the IRQ SCHEDULING HISTORY for %d count\n", max_irq_count);
	lldbg("*****************************************************************************\n");
	lldbg("* IRQ_TIME\t IRQ_NUMBER\t ISR_ADDRESS\n");
	lldbg("*****************************************************************************\n");

	idx = sysdbg_struct->irq_lastindex;
	do {
		lldbg("%8lld\t%8d\t%8X\n", (uint64_t)sysdbg_struct->irq[idx].time, sysdbg_struct->irq[idx].irq, sysdbg_struct->irq[idx].fn);
		idx--;
		/* Keeping it circular buffer */
		idx = idx & (max_irq_count - 1);
	} while (idx != sysdbg_struct->irq_lastindex);
#else
	lldbg("CONFIG_IRQ_SCHED_HISTORY is not enabled to view irq history");
#endif							/* End of CONFIG_IRQ_SCHED_HISTORY */

#ifdef CONFIG_SEMAPHORE_HISTORY
	lldbg("Displaying the SEMAPHORE HISTORY for %d count\n", max_sem_count);
	lldbg("IN:INIT AQ:AQUIRED RL:RELEASED WT:WAITING DR: DESTROY UN:UNKNOWN\n");
	lldbg("PID : -1 indicates that, it's a ISR, for ISR, TCB field gives reference of ISR\n");
	lldbg("*********************************************************************************\n");
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("SEMAPHORE_TIME\t STATUS\t\t  TASK_NAME\t SEMAPHORE     TCB\t PID\n");
#else
	lldbg("SEMAPHORE_TIME\t STATUS\t   SEMAPHORE\t TCB\t   PID\n");
#endif
	lldbg("*********************************************************************************\n");

	idx = sysdbg_struct->sem_lastindex;
	do {
#if CONFIG_TASK_NAME_SIZE > 0
		lldbg("%8lld%18s%20s%16X%12X%6d\n",
#else
		lldbg("%8lld%18s%12X%12X%6d\n",
#endif
			  (uint64_t)sysdbg_struct->sem_log[idx].time, sysdbg_struct->sem_log[idx].status,
#if CONFIG_TASK_NAME_SIZE > 0
			  sysdbg_struct->sem_log[idx].task_name,
#endif
			  sysdbg_struct->sem_log[idx].sem, sysdbg_struct->sem_log[idx].ptcb, sysdbg_struct->sem_log[idx].pid);
		idx--;
		/* Keeping it circular buffer */
		idx = idx & (max_sem_count - 1);
	} while (idx != sysdbg_struct->sem_lastindex);
#else
	lldbg("CONFIG_SEMAPHORE_HISTORY is not enabled to view semaphore history");
#endif
	leave_critical_section(saved_state);
}

/****************************************************************************
 * Name: sysdbg_write
 *
 * Description:
 *   fops to read data buffer passed from user space
 *
 * Inputs:
 *   filp:    file pointer
 *   buffer : User space buffer
 *   buflen : User space buffer length
 *
 * Return Value:
 *   ret - 0 indicates success and -1 indicates failure
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static ssize_t sysdbg_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	char arg1[32] = { '\0' };
	char arg2[32] = { '\0' };

	sscanf(buffer, "%s %s", arg1, arg2);

	if (strncmp("enable_monitor", arg1, 14) == 0) {
		sysdbg_monitor_enable();
	} else if (strncmp("disable_monitor", arg1, 15) == 0) {
		sysdbg_monitor_disable();
	} else if (strncmp("read", arg1, 4) == 0) {
		sysdbg_print();
	} else if (strncmp(arg1, "max_task_count", 14) == 0) {
#ifdef CONFIG_TASK_SCHED_HISTORY
		update_maxtask_count(atoi(arg2));
#else
		dbg("To use this feature, kindly enable CONFIG_TASK_SCHED_HISTORY\n");
#endif
	} else if (strncmp(arg1, "max_irq_count", 13) == 0) {
#ifdef CONFIG_IRQ_SCHED_HISTORY
		update_maxirq_count(atoi(arg2));
#else
		dbg("To use this feature, kindly enable CONFIG_IRQ_SCHED_HISTORY\n");
#endif
	} else if (strncmp(arg1, "max_sem_count", 13) == 0) {
#ifdef CONFIG_SEMAPHORE_HISTORY
		update_maxsem_count(atoi(arg2));
#else
		dbg("To use this feature, kindly enable CONFIG_SEMAPHORE_HISTORY\n");
#endif
	} else if (strncmp("data_abort", arg1, 10) == 0) {
		simulate_data_abort();
	} else if (strncmp("prefetch_abort", arg1, 14) == 0) {
		simulate_prefetch_abort();
	} else if (strncmp("undef_abort", arg1, 11) == 0) {
		simulate_undef_abort();
	} else if (strncmp("usr_assert", arg1, 10) == 0) {
		simulate_user_assert();
	} else if (strncmp("dump_stack", arg1, 10) == 0) {
#ifndef CONFIG_FRAME_POINTER
		dbg("To use this feature, kindly enable CONFIG_FRAME_POINTER\n");
#else
		dump_stack();
#endif
	} else if (strncmp("dump_allstack", arg1, 13) == 0) {
#ifndef CONFIG_FRAME_POINTER
		dbg("To use this feature, kindly enable CONFIG_FRAME_POINTER\n");
#else
		dump_all_stack();
#endif
	} else if (strncmp("help", arg1, 4) == 0) {
		show_usage();
	} else {
		dbg("Invalid argument %s\n", arg1);
		show_usage();
	}

	return 0;
}

#ifdef CONFIG_SEMAPHORE_HISTORY
/****************************************************************************
 * Name: save_semaphore_history
 *
 * Description:
 *   This function logs the semaphore history
 *
 * Inputs:
 *   irq: Interrupt number
 *   fn:  Interrupt Service Routine (ISR)
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

void save_semaphore_history(FAR sem_t *sem, void *addr, sem_status_t status)
{
	irqstate_t saved_state;
	static uint32_t index = 0;

	if (!sysdbg_monitor) {
		/* Monitoring is still not enabled */
		return;
	}
	if (!sysdbg_struct) {
		lldbg("sysdbg_struct is NULL\n");
		return;
	}
	saved_state = enter_critical_section();
	/* Keeping it circular buffer */
	index = index & (max_sem_count - 1);

	sysdbg_struct->sem_log[index].time = clock_systimer();
	switch (status) {
	case SEM_INIT:
		strncpy(sysdbg_struct->sem_log[index].status, "IN", 2);
		break;
	case SEM_ACQUIRE:
		strncpy(sysdbg_struct->sem_log[index].status, "AQ", 2);
		break;
	case SEM_RELEASE:
		strncpy(sysdbg_struct->sem_log[index].status, "RL", 2);
		break;
	case SEM_WAITING:
		strncpy(sysdbg_struct->sem_log[index].status, "WT", 2);
		break;
	case SEM_DESTROY:
		strncpy(sysdbg_struct->sem_log[index].status, "DR", 2);
		break;
	default:
		lldbg("Unknown status\n");
		strncpy(sysdbg_struct->sem_log[index].status, "UN", 2);
		break;
	}
	sysdbg_struct->sem_log[index].sem = sem;
	sysdbg_struct->sem_log[index].ptcb = (void *)addr;
	if (addr == NULL) {
		addr = (void *)this_task();
		sysdbg_struct->sem_log[index].ptcb = (void *)addr;
	}
#if CONFIG_TASK_NAME_SIZE > 0
	strncpy(sysdbg_struct->sem_log[index].task_name, ((struct tcb_s *)addr)->name, TASK_NAME_SIZE);
#endif
	sysdbg_struct->sem_log[index].pid = ((struct tcb_s *)addr)->pid;
	sysdbg_struct->sem_lastindex = index;
	index++;
	leave_critical_section(saved_state);
}

/****************************************************************************
 * Name: update_maxsem_count
 *
 * Description:
 *   This function updates the maximum count value for semaphore monitoring
 *   by Disabling the monitoring first, update the count and enable monitoring
 *
 * Inputs:
 *   count: count value, and value will be in the power of 2
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   count value passed is in the power of 2
 *
 ****************************************************************************/

static void update_maxsem_count(int count)
{
	if (count <= 0 || (count & (count - 1)) != 0) {
		lldbg("Enter the count in decimal, which is >0 & power of 2 (ex: 32/64)\n");
		return;
	}
	if (sysdbg_monitor) {
		/* Disable the monitoring as it would effect the indexing */
		sysdbg_monitor_disable();
	}
	max_sem_count = count;

	lldbg("Updated new count\n");
	/* Enable the monitoring */
	sysdbg_monitor_enable();
}
#endif							/* End of CONFIG_SEMAPHORE_HISTORY */

#ifdef CONFIG_TASK_SCHED_HISTORY

/****************************************************************************
 * Name: save_task_scheduling_status
 *
 * Description:
 *   This function logs the task scheduling history
 *
 * Inputs:
 *   tcb : task control block structure pointer
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

void save_task_scheduling_status(struct tcb_s *tcb)
{
	irqstate_t saved_state;
	static uint32_t index = 0;

	if (!sysdbg_monitor) {
		/* Monitoring is still not enabled */
		return;
	}
	if (!sysdbg_struct) {
		lldbg("dbg_log is NULL\n");
		return;
	}

	saved_state = enter_critical_section();
	/* Keeping it circular buffer */
	index = index & (max_task_count - 1);

	sysdbg_struct->sched[index].time = clock_systimer();
	if (tcb) {
#if CONFIG_TASK_NAME_SIZE > 0
		strncpy(sysdbg_struct->sched[index].task, tcb->name, TASK_NAME_SIZE);
#endif
		sysdbg_struct->sched[index].pid = tcb->pid;
		sysdbg_struct->sched[index].sched_priority = tcb->sched_priority;
		sysdbg_struct->sched[index].ptcb = tcb;
	}
	sysdbg_struct->task_lastindex = index;
	index++;
	leave_critical_section(saved_state);

}

/****************************************************************************
 * Name: update_maxtask_count
 *
 * Description:
 *   This function updates the maximum count value for task monitoring
 *   by Disabling the monitoring first, update the count and enable monitoring
 *
 * Inputs:
 *   count: count value, and value will be in the power of 2
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   count value passed is in the power of 2
 *
 ****************************************************************************/

static void update_maxtask_count(int count)
{
	if (count <= 0 || (count & (count - 1)) != 0) {
		lldbg("Enter the count in decimal, which is >0 & power of 2 (ex: 1/32/64)\n");
		return;
	}
	if (sysdbg_monitor) {
		/* Disable the monitoring as it would effect the indexing */
		sysdbg_monitor_disable();
	}
	max_task_count = count;

	lldbg("Updated new count\n");
	/* Enable the monitoring */
	sysdbg_monitor_enable();
}

#endif							/* End of CONFIG_TASK_SCHED_HISTORY */

#ifdef CONFIG_IRQ_SCHED_HISTORY

/****************************************************************************
 * Name: save_irq_scheduling_status
 *
 * Description:
 *   This function logs the irq scheduling history
 *
 * Inputs:
 *   irq: Interrupt number
 *   fn:  Interrupt Service Routine (ISR)
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

void save_irq_scheduling_status(uint32_t irq, void *fn)
{
	static uint32_t index = 0;
	if (!sysdbg_monitor) {
		/* Monitoring is still not enabled */
		return;
	}
	if (!sysdbg_struct) {
		lldbg("sysdbg_struct is NULL\n");
		return;
	}

	/* Keeping it circular buffer */
	index = index & (max_irq_count - 1);

	sysdbg_struct->irq[index].time = clock_systimer();
	sysdbg_struct->irq[index].irq = irq;
	sysdbg_struct->irq[index].fn = (void *)fn;
	sysdbg_struct->irq_lastindex = index;
	index++;
}

/****************************************************************************
 * Name: update_maxirq_count
 *
 * Description:
 *   This function updates the maximum count value for irq monitoring
 *   by Disabling the monitoring first, update the count and enable monitoring
 *
 * Inputs:
 *   count: count value, and value will be in the power of 2
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   count value passed is in the power of 2
 *
 ****************************************************************************/

static void update_maxirq_count(int count)
{
	if (count <= 0 || (count & (count - 1)) != 0) {
		lldbg("Enter the count in decimal, which is >0 & power of 2 (ex: 32/64)\n");
		return;
	}
	if (sysdbg_monitor) {
		/* Disable the monitoring as it would effect the indexing */
		sysdbg_monitor_disable();
	}
	max_irq_count = count;

	lldbg("Updated new count\n");
	/* Enable the monitoring */
	sysdbg_monitor_enable();
}

#endif							/* End of CONFIG_IRQ_SCHED_HISTORY */

/****************************************************************************
 * Name: sysdbg_monitor_disable
 *
 * Description:
 *   Disables the sysdbg monitoring by Freeing the memory for debug struct
 *   Sets the global flag sysdbg_monitor to false to indicate monitoring
 *   is disabled
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void sysdbg_monitor_disable(void)
{
	irqstate_t saved_state;
	if (!sysdbg_monitor) {
		lldbg("sysdbg monitoring is not yet enabled\n");
		return;
	}
	if (sysdbg_struct) {
		saved_state = enter_critical_section();
		/* Set the flag first to avoid any race condition */
		sysdbg_monitor = false;
#ifdef CONFIG_TASK_SCHED_HISTORY
		/* Free the memory allocated for sched struct */
		if (sysdbg_struct->sched) {
			kmm_free(sysdbg_struct->sched);
			sysdbg_struct->sched = NULL;
		}
		sysdbg_struct->task_lastindex = 0;
#endif
#ifdef CONFIG_IRQ_SCHED_HISTORY
		/* Free the memory allocated for irq struct */
		if (sysdbg_struct->irq) {
			kmm_free(sysdbg_struct->irq);
			sysdbg_struct->irq = NULL;
		}
		sysdbg_struct->irq_lastindex = 0;
#endif
#ifdef CONFIG_SEMAPHORE_HISTORY
		/* Free the memory allocated for sem_log struct */
		if (sysdbg_struct->sem_log) {
			kmm_free(sysdbg_struct->sem_log);
			sysdbg_struct->sem_log = NULL;
		}
		sysdbg_struct->sem_lastindex = 0;
#endif
		/* Free the memory allocated for main sysdbg_struct */
		kmm_free(sysdbg_struct);
		sysdbg_struct = NULL;

		leave_critical_section(saved_state);
	}
	lldbg("Disabled sysdbg monitoring feature\n");
	return;
}

/****************************************************************************
 * Name: sysdbg_monitor_enable
 *
 * Description:
 *   Enables the sysdbg monitoring by allocating the memory for debug struct
 *   Atlease one of the below flags must be enabled during compile time
 *   CONFIG_TASK_SCHED_HISTORY : Logs the task scheduling
 *   CONFIG_IRQ_SCHED_HISTORY  : Logs the IRQ scheduling
 *   CONFIG_SEMAPHORE_HISTORY  : Logs the semaphore history
 *   Sets the global flag sysdbg_monitor to true to indicate monitoring is enabled
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void sysdbg_monitor_enable(void)
{
	if (sysdbg_monitor) {
		lldbg("sysdbg monitoring is alredy enabled\n");
		return;
	}
#if defined(CONFIG_TASK_SCHED_HISTORY) || defined(CONFIG_IRQ_SCHED_HISTORY) \
	|| defined(CONFIG_SEMAPHORE_HISTORY)
	int size = 0;
	irqstate_t saved_state;
	saved_state = enter_critical_section();
	if (sysdbg_struct == NULL) {
		size = sizeof(sysdbg_t);
		sysdbg_struct = (FAR struct sysdbg_s*)kmm_zalloc(size);

		if (sysdbg_struct == NULL) {
			lldbg("Failed to allocate memory(%d) for sysdbg_struct\n", size);
			goto fail;
		}
#ifdef CONFIG_TASK_SCHED_HISTORY
		size = sizeof(sched_history_t) * max_task_count;
		sysdbg_struct->sched = (FAR sched_history_t *)kmm_zalloc(size);
		if (sysdbg_struct->sched == NULL) {
			lldbg("Failed to allocate memory(%d) (max_task_count * sizeof(sched_history_t)\n", size);
			goto fail1;
		}
#endif
#ifdef CONFIG_IRQ_SCHED_HISTORY
		size = sizeof(irq_history_t) * max_irq_count;
		sysdbg_struct->irq = (FAR irq_history_t *)kmm_zalloc(size);
		if (sysdbg_struct->irq == NULL) {
			lldbg("Failed to allocate memory(%d) (max_irq_count * sizeof(struct irq_history_t)\n", size);
			goto fail2;
		}
#endif
#ifdef CONFIG_SEMAPHORE_HISTORY
		size = sizeof(sem_history_t) * max_sem_count;
		sysdbg_struct->sem_log = (FAR sem_history_t *)kmm_zalloc(size);
		if (sysdbg_struct->sem_log == NULL) {
			lldbg("Failed to allocate memory(%d) (max_sem_count * sizeof(struct sem_history_t)\n", size);
			goto fail3;
		}
#endif
		sysdbg_monitor = true;
		lldbg("Enabled sysdbg monitoring feature\n");

		leave_critical_section(saved_state);
		return;
	}

#ifdef CONFIG_SEMAPHORE_HISTORY
fail3:
#endif
#ifdef CONFIG_IRQ_SCHED_HISTORY
	kmm_free(sysdbg_struct->irq);
	sysdbg_struct->irq = NULL;
#endif

#ifdef CONFIG_IRQ_SCHED_HISTORY
fail2:
#endif
#ifdef CONFIG_TASK_SCHED_HISTORY
	kmm_free(sysdbg_struct->sched);
	sysdbg_struct->sched = NULL;
#endif

#ifdef CONFIG_TASK_SCHED_HISTORY
fail1:
#endif
	kmm_free(sysdbg_struct);
	sysdbg_struct = NULL;

fail:
	lldbg("Disabling sysdbg monitoring feature, kindly use less count\n");
	sysdbg_monitor = false;
	leave_critical_section(saved_state);
#else
	lldbg("Kindly enable atleast one of the below config flags \n\r \
		CONFIG_TASK_SCHED_HISTORY : To log Task scheduling history \n\r \
		CONFIG_IRQ_SCHED_HISTORY  : To log IRQ scheduling history \n\r \
		CONFIG_SEMAPHORE_HISTORY  : To log semaphore status\n");
#endif
	return;
}

/****************************************************************************
 * Name: sysdbg_init
 *
 * Description:
 *   Registers the system debug device driver
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

int sysdbg_init(void)
{
	(void)register_driver(SYSDBG_PATH, &g_sysdbgops, 0666, NULL);
#if defined(CONFIG_TASK_SCHED_HISTORY) || defined(CONFIG_IRQ_SCHED_HISTORY) \
	|| defined(CONFIG_SEMAPHORE_HISTORY)
	sysdbg_monitor_enable();
#endif
	return 0;
}
