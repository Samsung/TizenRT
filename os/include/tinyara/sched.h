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

/********************************************************************************
 *
 *   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
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
 ********************************************************************************/
///@file tinyara/sched.h

#ifndef __INCLUDE_TINYARA_SCHED_H
#define __INCLUDE_TINYARA_SCHED_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <queue.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <mqueue.h>
#include <time.h>

#if defined(CONFIG_ENABLE_STACKMONITOR) && defined(CONFIG_DEBUG)
#include <tinyara/clock.h>
#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/mm/mm.h>
#endif
#endif

#include <tinyara/irq.h>
#include <tinyara/mm/shm.h>
#include <tinyara/fs/fs.h>
#include <tinyara/net/net.h>
#include <tinyara/mpu.h>
#include <arch/arch.h>
#ifdef CONFIG_ARMV8M_TRUSTZONE
#include <tinyara/tz_context.h>
#endif

/* Configuration ****************************************************************/
/* Task groups currently only supported for retention of child status */

#undef HAVE_TASK_GROUP
#undef HAVE_GROUP_MEMBERS

/* We need a group an group members if we are supporting the parent/child
 * relationship.
 */

#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS)
#define HAVE_TASK_GROUP     1
#define HAVE_GROUP_MEMBERS  1

/* We need a group (but not members) if any other resources are shared within
 * a task group.  NOTE: that we essentially always need a task group and that
 * managing this definition adds a lot of overhead just to handle a corner-
 * case very minimal system!
 */

#else
#if !defined(CONFIG_DISABLE_PTHREAD) && defined(CONFIG_SCHED_HAVE_PARENT)
#define HAVE_TASK_GROUP   1		/* pthreads with parent */
#elif !defined(CONFIG_DISABLE_ENVIRON)
#define HAVE_TASK_GROUP   1		/* Environment variables */
#elif !defined(CONFIG_DISABLE_SIGNALS)
#define HAVE_TASK_GROUP   1		/* Signals */
#elif defined(CONFIG_SCHED_ATEXIT)
#define HAVE_TASK_GROUP   1		/* Group atexit() function */
#elif defined(CONFIG_SCHED_ONEXIT)
#define HAVE_TASK_GROUP   1		/* Group on_exit() function */
#elif defined(CONFIG_SCHED_WAITPID)
#define HAVE_TASK_GROUP   1		/* Group waitpid() function */
#elif CONFIG_NFILE_DESCRIPTORS > 0
#define HAVE_TASK_GROUP   1		/* File descriptors */
#elif CONFIG_NFILE_STREAMS > 0
#define HAVE_TASK_GROUP   1		/* Standard C buffered I/O */
#elif CONFIG_NSOCKET_DESCRIPTORS > 0
#define HAVE_TASK_GROUP   1		/* Sockets */
#elif !defined(CONFIG_DISABLE_MQUEUE)
#define HAVE_TASK_GROUP   1		/* Message queues */
#elif defined(CONFIG_ARCH_ADDRENV)
#define HAVE_TASK_GROUP   1		/* Address environment */
#elif defined(CONFIG_MM_SHM)
#define HAVE_TASK_GROUP   1		/* Shared memory */
#elif defined(CONFIG_BINARY_MANAGER)
#define HAVE_TASK_GROUP   1		/* Binary management */
#endif
#endif

/* In any event, we don't need group members if support for pthreads is disabled */

#ifdef CONFIG_DISABLE_PTHREAD
#undef HAVE_GROUP_MEMBERS
#endif

/* Task Management Definitions **************************************************/
/* Special task IDS.  Any negative PID is invalid. */

#define NULL_TASK_PROCESS_ID      (pid_t)0
#define INVALID_PROCESS_ID        (pid_t)-1

/* This is the maximum number of times that a lock can be set */

#define MAX_LOCK_COUNT             127

/* Values for the struct tcb_s flags bits */

#define TCB_FLAG_TTYPE_SHIFT       (0)	/* Bits 0-1: thread type */
#define TCB_FLAG_TTYPE_MASK        (3 << TCB_FLAG_TTYPE_SHIFT)
#define TCB_FLAG_TTYPE_TASK      (0 << TCB_FLAG_TTYPE_SHIFT)	/* Normal user task */
#define TCB_FLAG_TTYPE_PTHREAD   (1 << TCB_FLAG_TTYPE_SHIFT)	/* User pthread */
#define TCB_FLAG_TTYPE_KERNEL    (2 << TCB_FLAG_TTYPE_SHIFT)	/* Kernel thread */
#define TCB_FLAG_NONCANCELABLE     (1 << 2)	/* Bit 2: Pthread is non-cancelable */
#define TCB_FLAG_CANCEL_DEFERRED   (1 << 3)	/* Bit 3: Deferred (vs asynch) cancellation type */
#define TCB_FLAG_CANCEL_PENDING    (1 << 4)	/* Bit 4: Pthread cancel is pending */
#define TCB_FLAG_POLICY_SHIFT      (5) /* Bit 5-6: Scheduling policy */
 #define TCB_FLAG_POLICY_MASK       (3 << TCB_FLAG_POLICY_SHIFT)
 #  define TCB_FLAG_SCHED_FIFO      (0 << TCB_FLAG_POLICY_SHIFT) /* FIFO scheding policy */
 #  define TCB_FLAG_ROUND_ROBIN     (1 << TCB_FLAG_POLICY_SHIFT) /* Round robin scheding policy */
 #  define TCB_FLAG_SCHED_SPORADIC  (2 << TCB_FLAG_POLICY_SHIFT) /* Sporadic scheding policy */
 #  define TCB_FLAG_SCHED_OTHER     (3 << TCB_FLAG_POLICY_SHIFT) /* Other scheding policy */
#define TCB_FLAG_CPU_LOCKED        (1 << 7) /* Bit 7: Locked to this CPU */
#define TCB_FLAG_EXIT_PROCESSING   (1 << 8) /* Bit 8: Exitting */
#define TCB_FLAG_SYSCALL           (1 << 10)                     /* Bit 9: In a system call */
/* Bits 11-15: Available */

/* Values for struct task_group tg_flags */

#define GROUP_FLAG_NOCLDWAIT       (1 << 0)	/* Bit 0: Do not retain child exit status */
#define GROUP_FLAG_ADDRENV         (1 << 1)	/* Bit 1: Group has an address environment */
#define GROUP_FLAG_PRIVILEGED      (1 << 2)	/* Bit 2: Group is privileged */

/* Values for struct child_status_s ch_flags */

#define CHILD_FLAG_TTYPE_SHIFT     (0)	/* Bits 0-1: child thread type */
#define CHILD_FLAG_TTYPE_MASK      (3 << CHILD_FLAG_TTYPE_SHIFT)
#define CHILD_FLAG_TTYPE_TASK    (0 << CHILD_FLAG_TTYPE_SHIFT)	/* Normal user task */
#define CHILD_FLAG_TTYPE_PTHREAD (1 << CHILD_FLAG_TTYPE_SHIFT)	/* User pthread */
#define CHILD_FLAG_TTYPE_KERNEL  (2 << CHILD_FLAG_TTYPE_SHIFT)	/* Kernel thread */
#define CHILD_FLAG_EXITED          (1 << 0)	/* Bit 2: The child thread has exit'ed */

/* Values for flag of pthread key allocated */

#define KEY_NOT_INUSE   (0)
#define KEY_INUSE       (1)

#define MAX_PID_MASK	(CONFIG_MAX_TASKS - 1)
#define PIDHASH(pid)	((pid) & MAX_PID_MASK)

/********************************************************************************
 * Public Type Definitions
 ********************************************************************************/

#ifndef __ASSEMBLY__

/* General Task Management Types ************************************************/

/**
 * @brief This is the type of the task_state field of the TCB. NOTE: the order and
 * content of this enumeration is critical since there are some OS tables indexed
 * by these values.  The range of values is assumed to fit into a uint8_t in
 * struct tcb_s.
 */
enum tstate_e {
	TSTATE_TASK_INVALID = 0,	/* INVALID      - The TCB is uninitialized */
	TSTATE_TASK_PENDING,		/* READY_TO_RUN - Pending preemption unlock */
	TSTATE_TASK_READYTORUN,		/* READY-TO-RUN - But not running */
	TSTATE_TASK_RUNNING,		/* READY_TO_RUN - And running */

	TSTATE_TASK_INACTIVE,		/* BLOCKED      - Initialized but not yet activated */
	TSTATE_WAIT_SEM,			/* BLOCKED      - Waiting for a semaphore */
	TSTATE_WAIT_FIN,		/* BLOCKED		- Waiting to be unblocked by fin */
#ifndef CONFIG_DISABLE_SIGNALS
	TSTATE_WAIT_SIG,			/* BLOCKED      - Waiting for a signal */
#endif
#ifndef CONFIG_DISABLE_MQUEUE
	TSTATE_WAIT_MQNOTEMPTY,		/* BLOCKED      - Waiting for a MQ to become not empty. */
	TSTATE_WAIT_MQNOTFULL,		/* BLOCKED      - Waiting for a MQ to become not full. */
#endif
#ifdef CONFIG_PAGING
	TSTATE_WAIT_PAGEFILL,		/* BLOCKED      - Waiting for page fill */
#endif
	NUM_TASK_STATES				/* Must be last */
};
typedef enum tstate_e tstate_t;

/* The following definitions are determined by tstate_t */

#define FIRST_READY_TO_RUN_STATE TSTATE_TASK_READYTORUN
#define LAST_READY_TO_RUN_STATE  TSTATE_TASK_RUNNING
#define FIRST_BLOCKED_STATE      TSTATE_TASK_INACTIVE
#define LAST_BLOCKED_STATE       (NUM_TASK_STATES-1)

/* The following is the form of a thread start-up function */

typedef CODE void (*start_t)(void);

/** @brief This is the entry point into the main thread of the task or into a created
 * pthread within the task.
 */
union entry_u {
	pthread_startroutine_t pthread;
	main_t main;
};
typedef union entry_u entry_t;

/* This is the type of the function called at task startup */

#ifdef CONFIG_SCHED_STARTHOOK
typedef CODE void (*starthook_t)(FAR void *arg);
#endif

/* These are the types of the functions that are executed with exit() is called
 * (if registered via atexit() on on_exit()).
 */

#ifdef CONFIG_SCHED_ATEXIT
typedef CODE void (*atexitfunc_t)(void);
#endif

#ifdef CONFIG_SCHED_ONEXIT
typedef CODE void (*onexitfunc_t)(int exitcode, FAR void *arg);
#endif

/* struct child_status_s *********************************************************/
/** @brief This structure is used to maintin information about child tasks.
 * pthreads work differently, they have join information.  This is
 * only for child tasks.
 */
#ifdef CONFIG_SCHED_CHILD_STATUS
struct child_status_s {
	FAR struct child_status_s *flink;

	uint8_t ch_flags;			/* Child status:  See CHILD_FLAG_* definitions */
	pid_t ch_pid;				/* Child task ID */
	int ch_status;				/* Child exit status */
};
#endif

/* struct pthread_cleanup_s ******************************************************/
/* This structure describes one element of the pthread cleanup stack */

#ifdef CONFIG_PTHREAD_CLEANUP
struct pthread_cleanup_s {
	pthread_cleanup_t pc_cleaner;    /* Cleanup callback address */
	FAR void *pc_arg;                /* Argument that accompanies the callback */
};
#endif

/* struct dspace_s ***************************************************************/

/** @brief This structure describes a reference counted D-Space region.  This must be a
 * separately allocated "break-away" structure that can be owned by a task and
 * any pthreads created by the task.
 */
#ifdef CONFIG_PIC
struct dspace_s {
	/* The life of the structure allocation is determined by this reference
	 * count.  This count is number of threads that shared the same D-Space.
	 * This includes the parent task as well as any pthreads created by the
	 * parent task or any of its child threads.
	 */

	uint16_t crefs;

	/* This is the allocated D-Space memory region.  This may be a physical
	 * address allocated with kmm_malloc(), or it may be virtual address associated
	 * with an address environment (if CONFIG_ARCH_ADDRENV=y).
	 */

	FAR uint8_t *region;
};
#endif

/* struct stackinfo_s *******************************************************/

/* Used to report stack information */

struct stackinfo_s {
	size_t    adj_stack_size;	/* Stack size after adjustment         */
					/* for hardware, processor, etc.       */
					/* (for debug purposes only)           */
	FAR void *stack_alloc_ptr;	/* Pointer to allocated stack          */
					/* Needed to deallocate stack          */
	FAR void *stack_base_ptr;	/* Adjusted initial stack pointer      */
					/* after the frame has been removed    */
					/* from the stack.                     */
};

/* struct task_group_s ***********************************************************/
/* All threads created by pthread_create belong in the same task group (along with
 * the thread of the original task).  struct task_group_s is a shared structure
 * referenced by the TCB of each thread that is a member of the task group.
 *
 * This structure should contain *all* resources shared by tasks and threads that
 * belong to the same task group:
 *
 *   Child exit status
 *   Environment variables
 *   PIC data space and address environments
 *   File descriptors
 *   FILE streams
 *   Sockets
 *   Address environments.
 *
 * Each instance of struct task_group_s is reference counted. Each instance is
 * created with a reference count of one.  The reference incremented when each
 * thread joins the group and decremented when each thread exits, leaving the
 * group.  When the reference count decrements to zero, the struct task_group_s
 * is free.
 */

#ifdef HAVE_TASK_GROUP

#ifndef CONFIG_DISABLE_PTHREAD
struct join_s;					/* Forward reference                        */
/* Defined in kernel/pthread/pthread.h       */
#endif

#ifdef CONFIG_BINFMT_LOADABLE
struct binary_s;				/* Forward reference                        */
#endif
/** @brief Structure for Task Group Information */
struct task_group_s {
#if defined(HAVE_GROUP_MEMBERS) || defined(CONFIG_ARCH_ADDRENV)
	struct task_group_s *flink;	/* Supports a singly linked list            */
	gid_t tg_gid;				/* The ID of this task group                */
#endif
#ifdef HAVE_GROUP_MEMBERS
	gid_t tg_pgid;				/* The ID of the parent task group          */
#endif
#if !defined(CONFIG_DISABLE_PTHREAD) && defined(CONFIG_SCHED_HAVE_PARENT)
	pid_t tg_task;				/* The ID of the task within the group      */
#endif
#if defined(CONFIG_BINARY_MANAGER)
	int tg_binidx;				/* The Index of binary in binary table      */
#endif

	uint8_t tg_flags;			/* See GROUP_FLAG_* definitions             */

	/* Group membership ********************************************************** */

	uint8_t tg_nmembers;		/* Number of members in the group           */
#ifdef HAVE_GROUP_MEMBERS
	uint8_t tg_mxmembers;		/* Number of members in allocation          */
	FAR pid_t *tg_members;		/* Members of the group                     */
#endif

#if defined(CONFIG_SCHED_ATEXIT) && !defined(CONFIG_SCHED_ONEXIT)
	/* atexit support *********************************************************** */

	sq_queue_t tg_atexitfunc;
#endif

#ifdef CONFIG_SCHED_ONEXIT
	/* on_exit support ********************************************************** */
	sq_queue_t tg_onexitfunc;
#endif

#ifdef CONFIG_SCHED_HAVE_PARENT
	/* Child exit status ********************************************************* */

#ifdef CONFIG_SCHED_CHILD_STATUS
	FAR struct child_status_s *tg_children; /* Head of a list of child status     */
#endif

#ifndef HAVE_GROUP_MEMBERS
	/* REVISIT: What if parent thread exits?  Should use tg_pgid. */

	pid_t    tg_ppid;                 /* This is the ID of the parent thread      */
#ifndef CONFIG_SCHED_CHILD_STATUS
	uint16_t tg_nchildren;            /* This is the number active children       */
#endif
#endif /* HAVE_GROUP_MEMBERS */
#endif /* CONFIG_SCHED_HAVE_PARENT */

#if defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_SCHED_HAVE_PARENT)
	/* waitpid support *********************************************************** */
	/* Simple mechanism used only when there is no support for SIGCHLD            */

	sem_t tg_exitsem;			/* Support for waitpid                      */
	int *tg_statloc;			/* Location to return exit status           */
#endif

#ifndef CONFIG_DISABLE_PTHREAD
	/* Pthreads ****************************************************************** */
	/* Pthread join Info:                       */
	sem_t tg_joinsem;			/*   Mutually exclusive access to join data */
	FAR struct join_s *tg_joinhead;	/*   Head of a list of join data            */
	FAR struct join_s *tg_jointail;	/*   Tail of a list of join data            */
#if CONFIG_NPTHREAD_KEYS > 0
	uint8_t tg_key[PTHREAD_KEYS_MAX];		/* flag of pthread keys allocated */
	pthread_destructor_t tg_destructor[PTHREAD_KEYS_MAX];	/* Address list of each destructor */
#endif
#endif

#ifndef CONFIG_DISABLE_SIGNALS
	/* POSIX Signal Control Fields *********************************************** */

	sq_queue_t sigpendingq;		/* List of pending signals                  */
#endif

#ifndef CONFIG_DISABLE_ENVIRON
	/* Environment variables ***************************************************** */

	size_t tg_envsize;			/* Size of environment string allocation    */
	FAR char *tg_envp;			/* Allocated environment strings            */
#endif

	/* PIC data space and address environments *********************************** */
	/* Logically the PIC data space belongs here (see struct dspace_s).  The
	 * current logic needs review:  There are differences in the away that the
	 * life of the PIC data is managed.
	 */

#if CONFIG_NFILE_DESCRIPTORS > 0
	/* File descriptors ********************************************************** */

	struct filelist tg_filelist;	/* Maps file descriptor to file             */
#endif

#if CONFIG_NFILE_STREAMS > 0
	/* FILE streams ************************************************************** */
	/* In a flat, single-heap build.  The stream list is allocated with this
	 * structure.  But kernel mode with a kernel allocator, it must be separately
	 * allocated using a user-space allocator.
	 */

#if (defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)) && \
	 defined(CONFIG_MM_KERNEL_HEAP)
	FAR struct streamlist *tg_streamlist;
#else
	struct streamlist tg_streamlist;	/* Holds C buffered I/O info                */
#endif
#endif

#if CONFIG_NSOCKET_DESCRIPTORS > 0
	/* Sockets ******************************************************************* */

	struct socketlist tg_socketlist;	/* Maps socket descriptor to socket         */
#endif

#ifndef CONFIG_DISABLE_MQUEUE
	/* POSIX Named Message Queue Fields ****************************************** */

	sq_queue_t tg_msgdesq;		/* List of opened message queues           */
#endif

#ifdef CONFIG_ARCH_ADDRENV
	/* Address Environment ******************************************************* */

	group_addrenv_t tg_addrenv;	/* Task group address environment           */
#endif

#ifdef CONFIG_MM_SHM
	/* Shared Memory ************************************************************* */

	struct group_shm_s tg_shm;	/* Task shared memory logic                 */
#endif

#if defined(CONFIG_PREFERENCE) && CONFIG_TASK_NAME_SIZE > 0
	/* Preference **************************************************************** */
	/* The values of private preference are managed by task group.
	 * Each 'value' is saved in a file named 'key' and they are located in a directory /mnt/private/<tg_name>.
	 */
	char tg_name[CONFIG_TASK_NAME_SIZE + 1]; /* Group name (with NULL terminator)  */
#endif
};
#endif

#ifdef CONFIG_BINFMT_LOADABLE
/* This macro verifies whether the tcb is for a main task of the binary.
 * It checks if the tcb is for a task and if it has non NULL loading data i.e. 'bininfo'
 */
#define IS_BINARY_MAINTASK(tcb)    (((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_TASK) \
					&& (((struct task_tcb_s *)tcb)->bininfo != NULL))
#endif

/* struct tcb_s ******************************************************************/

FAR struct wdog_s;				/* Forward reference                   */
/** @brief This is the common part of the task control block (TCB).  The TCB is the heart
 * of the TinyAra task-control logic.  Each task or thread is represented by a TCB
 * that includes these common definitions.
 */
struct tcb_s {
	/* Fields used to support list management ************************************ */

	FAR struct tcb_s *flink;	/* Doubly linked list                  */
	FAR struct tcb_s *blink;

#ifdef CONFIG_BINARY_MANAGER
	/* Fields used to support binary list management ***************************** */
	FAR struct tcb_s *bin_flink;	/* Doubly linked list				   */
	FAR struct tcb_s *bin_blink;
#endif

	/* Task Group **************************************************************** */

#ifdef HAVE_TASK_GROUP
	FAR struct task_group_s *group;	/* Pointer to shared task group data   */
#endif

	/* Task Management Fields **************************************************** */

	pid_t pid;					/* This is the ID of the thread        */

	start_t start;				/* Thread start function               */
	entry_t entry;				/* Entry Point into the thread         */
	uint8_t sched_priority;		/* Current priority of the thread      */

#ifdef CONFIG_PRIORITY_INHERITANCE
#if CONFIG_SEM_NNESTPRIO > 0
	uint8_t npend_reprio;		/* Number of nested reprioritizations  */
	uint8_t pend_reprios[CONFIG_SEM_NNESTPRIO];
#endif
	uint8_t base_priority;		/* "Normal" priority of the thread     */
#endif

	uint8_t task_state;			/* Current state of the thread         */
	uint16_t flags;				/* Misc. general status flags          */
	int16_t lockcount;			/* 0=preemptable (not-locked)          */
#ifdef CONFIG_CANCELLATION_POINTS
	int16_t cpcount;			/* Nested cancellation point count     */
#endif

#if CONFIG_RR_INTERVAL > 0
	int timeslice;				/* RR timeslice interval remaining     */
#endif
	FAR struct wdog_s *waitdog;	/* All timed waits used this wdog      */

	/* Stack-Related Fields ****************************************************** */

	size_t adj_stack_size;		/* Stack size after adjustment         */
	/* for hardware, processor, etc.       */
	/* (for debug purposes only)           */
	FAR void *stack_alloc_ptr;	/* Pointer to allocated stack          */
	/* Need to deallocate stack            */
	FAR void *adj_stack_ptr;	/* Adjusted stack_alloc_ptr for HW     */
	FAR void *stack_base_ptr;              /* Adjusted initial stack pointer  */

	/* The initial stack pointer value     */

#ifdef CONFIG_MPU_STACKGUARD
	FAR void *stack_guard;          /* address of the stack guard */
	size_t guard_size;              /* size of the guard region */
#endif
	/* External Module Support *************************************************** */

#ifdef CONFIG_PIC
	FAR struct dspace_s *dspace;	/* Allocated area for .bss and .data   */
#endif

	/* POSIX Semaphore Control Fields ******************************************** */

	sem_t *waitsem;				/* Semaphore ID waiting on             */

	/* POSIX Signal Control Fields *********************************************** */

#ifndef CONFIG_DISABLE_SIGNALS
	sigset_t sigprocmask;		/* Signals that are blocked            */
	sigset_t sigwaitmask;		/* Waiting for pending signals         */
	sq_queue_t sigactionq;		/* List of actions for signals         */
	sq_queue_t sigpendactionq;	/* List of pending signal actions      */
	sq_queue_t sigpostedq;		/* List of posted signals              */
	siginfo_t sigunbinfo;		/* Signal info when task unblocked     */
#ifdef CONFIG_SIGKILL_HANDLER
	_sa_sigaction_t sigkillusrhandler; /* User defined SIGKILL handler      */
#endif
#ifdef HAVE_GROUP_MEMBERS
	sigset_t sigrecvmask;		/* Signals that are blocked by receiving signals */
#endif
#endif

	/* POSIX Named Message Queue Fields ****************************************** */

#ifndef CONFIG_DISABLE_MQUEUE
	FAR struct mqueue_inode_s *msgwaitq;	/* Waiting for this message queue      */
#endif

	/* Library related fields **************************************************** */

	int pterrno;				/* Current per-thread errno            */

	/* State save areas ********************************************************** */
	/* The form and content of these fields are platform-specific.                */

	struct xcptcontext xcp;		/* Interrupt register save area        */

	uint32_t uheap;			/* User heap object pointer */
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint32_t uspace;		/* User space object for app binary */

#ifdef CONFIG_ARM_MPU						/* MPU register values for loadable apps only */
	uint32_t mpu_regs[MPU_REG_NUMBER * MPU_NUM_REGIONS];	/* MPU for apps is configured during loading and disabled in task_terminate */
#endif
#endif

#if defined(CONFIG_MPU_STACK_OVERFLOW_PROTECTION)		/* MPU register values for stack protection */
	uint32_t stack_mpu_regs[MPU_REG_NUMBER];		/* MPU for stack is configured during stack creation and disabled at stack release */
#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	uint32_t app_id;			/* Indicates app id of the task and used to index into umm_heap_table */
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
	volatile TZ_ModuleId_t tz_context;
#endif

#if CONFIG_TASK_NAME_SIZE > 0
	char name[CONFIG_TASK_NAME_SIZE + 1];	/* Task name (with NUL terminator)     */
#endif
#ifdef CONFIG_TASK_MONITOR
	bool is_active;
#endif

	int fin_data;			/* Irq notification Data to be handled */
	int pending_fin_data;		/* Pended irq notification data */
};

/* struct task_tcb_s *************************************************************/
/** @brief This is the particular form of the task control block (TCB) structure used by
 * tasks (and kernel threads).  There are two TCB forms:  one for pthreads and
 * one for tasks.  Both share the common TCB fields (which must appear at the
 * top of the structure) plus additional fields unique to tasks and threads.
 * Having separate structures for tasks and pthreads adds some complexity, but
 * saves memory in that it prevents pthreads from being burdened with the
 * overhead required for tasks (and vice versa).
 */
struct task_tcb_s {
	/* Common TCB fields ********************************************************* */

	struct tcb_s cmn;			/* Common TCB fields                   */

	/* Task Management Fields **************************************************** */

#ifdef CONFIG_SCHED_STARTHOOK
	starthook_t starthook;		/* Task startup function               */
	FAR void *starthookarg;		/* The argument passed to the function */
#endif
#ifdef CONFIG_BINFMT_LOADABLE
	/* Loadable module support *************************************************** */

	FAR struct binary_s *bininfo;	/* Describes resources used by program	*/
#endif

	/* Values needed to restart a task ******************************************* */

	uint8_t init_priority;		/* Initial priority of the task        */
	FAR char **argv;			/* Name+start-up parameters            */
};

/* struct pthread_tcb_s **********************************************************/

#ifndef CONFIG_DISABLE_PTHREAD
/** @brief This is the particular form of the task control block (TCB) structure used by
 * pthreads.  There are two TCB forms:  one for pthreads and one for tasks.  Both
 * share the common TCB fields (which must appear at the top of the structure)
 * plus additional fields unique to tasks and threads.  Having separate structures
 * for tasks and pthreads adds some complexity,  but saves memory in that it
 * prevents pthreads from being burdened with the overhead required for tasks
 * (and vice versa).
 */
struct pthread_tcb_s {
	/* Common TCB fields ********************************************************* */

	struct tcb_s cmn;			/* Common TCB fields                   */

	/* Task Management Fields **************************************************** */

	pthread_addr_t arg;			/* Startup argument                    */
	FAR void *joininfo;			/* Detach-able info to support join    */

	/* Robust mutex support *********************************************/

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
	FAR struct pthread_mutex_s *mhead;	/* List of mutexes held by thread      */
#endif

	/* Clean-up stack ***************************************************/

#ifdef CONFIG_PTHREAD_CLEANUP
	/* tos   - The index to the next avaiable entry at the top of the stack.
	 * stack - The pre-allocated clean-up stack memory.
	 */

	uint8_t tos;
	struct pthread_cleanup_s stack[CONFIG_PTHREAD_CLEANUP_STACKSIZE];
#endif

	/* POSIX Thread Specific Data ************************************************ */

#if CONFIG_NPTHREAD_KEYS > 0
	void *key_data[PTHREAD_KEYS_MAX];
#endif
#if defined(CONFIG_BUILD_PROTECTED)
	struct pthread_region_s *region;
#endif
};
#endif							/* !CONFIG_DISABLE_PTHREAD */

/* This is the callback type used by sched_foreach() */

typedef void (*sched_foreach_t)(FAR struct tcb_s *tcb, FAR void *arg);

#endif							/* __ASSEMBLY__ */

/* 
 * @cond
 * @internal
 * {
 */
#define STKMON_MAX_LOGS (CONFIG_MAX_TASKS * 2)

/* Structure for saving terminated task/pthread information with stack monitor. */
struct stkmon_save_s {
	clock_t timestamp;
	pid_t chk_pid;
	size_t chk_stksize;
	size_t chk_peaksize;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	int chk_peakheap;
#endif
#if (CONFIG_TASK_NAME_SIZE > 0)
	char chk_name[CONFIG_TASK_NAME_SIZE + 1];
#endif
};

void stkmon_copy_log(struct stkmon_save_s *dest_arr);
/* 
 * }
 * @endcond
 */
/********************************************************************************
 * Public Data
 ********************************************************************************/

#ifndef __ASSEMBLY__
#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

/* TCB helpers ******************************************************************/

/**
 * @ingroup SCHED_KERNEL
 * @brief returns the TCB of the currently running task (i.e., the
 * caller)
 * @details @b #include <tinyara/sched.h> \n
 *  Basically, this function just wraps the
 *   head of the ready-to-run list and manages access to the TCB from outside
 *   of the sched/ sub-directory.
 * @return TCB structure
 * @since TizenRT v1.0
 */
FAR struct tcb_s *sched_self(void);

/* sched_foreach
 */
/**
 * @ingroup SCHED_KERNEL
 * @brief enumerate over each task and provide the TCB of each task
 * or thread to a callback function.  Interrupts will be disabled throughout
 * this enumeration!
 *
 * @details @b #include <tinyara/sched.h>
 * @param[in] handler The function to be called with the TCB of each task
 * @param[in] arg param
 * @return none
 * @since TizenRT v1.0
 */
void sched_foreach(sched_foreach_t handler, FAR void *arg);

/**
 * @ingroup SCHED_KERNEL
 * @brief Give a task ID, look up the corresponding TCB
 * @details @b #include <tinyara/sched.h> \n
 *   Given a task ID, this function will return
 *   the a pointer to the corresponding TCB (or NULL if there
 *   is no such task ID).
 * @param[in] pid Pid for tcb
 * @return TCB structure about pid
 * @since TizenRT v1.0
 */
FAR struct tcb_s *sched_gettcb(pid_t pid);

/* File system helpers **********************************************************/
/* These functions all extract lists from the group structure assocated with the
 * currently executing task.
 */

#if CONFIG_NFILE_DESCRIPTORS > 0
/**
 * @cond
 * @internal
 */
FAR struct filelist *sched_getfiles(void);
/**
 * @endcond
 */
#if CONFIG_NFILE_STREAMS > 0
/**
 * @ingroup SCEHD_KERNEL
 * @brief Return a pointer to the streams list for this thread
 * @details @b #include <tinyara/sched.h>
 * @return A pointer to the errno
 * @since TizenRT v1.0
 */
FAR struct streamlist *sched_getstreams(void);
#endif							/* CONFIG_NFILE_STREAMS */
#endif							/* CONFIG_NFILE_DESCRIPTORS */

#if CONFIG_NSOCKET_DESCRIPTORS > 0
FAR struct socketlist *sched_getsockets(void);
#endif							/* CONFIG_NSOCKET_DESCRIPTORS */

#ifdef CONFIG_SCHED_CPULOAD
int sched_start_cpuload_snapshot(int ticks);
void sched_clear_cpuload_snapshot(void);
void sched_get_cpuload_snapshot(pid_t *result_addr);
#endif

/********************************************************************************
 * Name: task_starthook
 *
 * Description:
 *   Configure a start hook... a function that will be called on the thread
 *   of the new task before the new task's main entry point is called.
 *   The start hook is useful, for example, for setting up automatic
 *   configuration of C++ constructors.
 *
 * Inputs:
 *   tcb - The new, unstarted task task that needs the start hook
 *   starthook - The pointer to the start hook function
 *   arg - The argument to pass to the start hook function.
 *
 * Return:
 *   None
 *
 ********************************************************************************/

#ifdef CONFIG_SCHED_STARTHOOK
/**
 * @cond
 * @internal
 */
void task_starthook(FAR struct task_tcb_s *tcb, starthook_t starthook, FAR void *arg);
/**
 * @endcond
 */
#endif

/********************************************************************************
 * Internal vfork support.  The overall sequence is:
 *
 * 1) User code calls vfork().  vfork() is provided in architecture-specific
 *    code.
 * 2) vfork()and calls task_vforksetup().
 * 3) task_vforksetup() allocates and configures the child task's TCB.  This
 *    consists of:
 *    - Allocation of the child task's TCB.
 *    - Initialization of file descriptors and streams
 *    - Configuration of environment variables
 *    - Setup the intput parameters for the task.
 *    - Initialization of the TCB (including call to up_initial_state()
 * 4) vfork() provides any additional operating context. vfork must:
 *    - Allocate and initialize the stack
 *    - Initialize special values in any CPU registers that were not
 *      already configured by up_initial_state()
 * 5) vfork() then calls task_vforkstart()
 * 6) task_vforkstart() then executes the child thread.
 *
 * task_vforkabort() may be called if an error occurs between steps 3 and 6.
 *
 ********************************************************************************/
/**
 * @cond
 * @internal
 */
FAR struct task_tcb_s *task_vforksetup(start_t retaddr);
/**
 * @internal
 */
pid_t task_vforkstart(FAR struct task_tcb_s *child);
/**
 * @internal
 */
void task_vforkabort(FAR struct task_tcb_s *child, int errcode);

/**
 * @endcond
 */
#undef EXTERN
#if defined(__cplusplus)
}
#endif
#endif							/* __ASSEMBLY__ */

#endif							/* __INCLUDE_TINYARA_SCHED_H */
