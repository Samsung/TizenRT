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
 * os/drivers/crashrpt.c
 *
 * Crash diagnostic capture - FIN_WAIT + timed wait method.
 * See os/include/tinyara/crashrpt.h for the design summary.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>
#include <semaphore.h>
#include <sched.h>

#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>
#include <tinyara/clock.h>
#include <tinyara/semaphore.h>
#include <tinyara/fs/fs.h>
#include <tinyara/crashrpt.h>

#include "sched/sched.h"
#include "semaphore/semaphore.h"

#ifdef CONFIG_CRASH_REPORT

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* struct crashrpt_file_s.opaque must be large enough to hold a struct file.
 * The application passes a crashrpt_file_s by value through ioctl; the driver
 * casts the opaque array to a struct file pointer and uses it directly.
 */

static_assert(sizeof(struct crashrpt_file_s) >= sizeof(struct file), "crashrpt_file_s too small for struct file");

#ifndef CONFIG_CRASH_REPORT_DEADLINE_MS
#define CONFIG_CRASH_REPORT_DEADLINE_MS   3000
#endif

/* The reporter is boosted to this priority so that it is guaranteed to be
 * the next thread to run.  Without the boost sched_addreadytorun() may not
 * select it, the deadline is consumed doing nothing and no report is
 * produced.
 */

#define CRASHRPT_BOOST_PRIORITY           (SCHED_PRIORITY_MAX)

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* The registered reporter.  NULL when no reporter is available. */

static FAR struct tcb_s *g_reporter_tcb;
static pid_t g_reporter_pid;

/* Posted by the reporter through CRASHRPTIOC_DONE. */

static sem_t g_done_sem;

/* Re-entry guard.  Set before anything else happens in crashrpt_notify().
 *
 * up_assert() has not yet set its own abort state when our hook runs, so a
 * fault raised by the reporter would be treated as a fresh assert and would
 * re-enter this path.  Without this flag that recurses until the stack or
 * the watchdog ends it.
 */

static volatile bool g_report_in_progress;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int crashrpt_open(FAR struct file *filep);
static int crashrpt_close(FAR struct file *filep);
static ssize_t crashrpt_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t crashrpt_write(FAR struct file *filep, FAR const char *buffer, size_t len);
static int crashrpt_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations crashrpt_fops = {
	crashrpt_open,				/* open   */
	crashrpt_close,				/* close  */
	crashrpt_read,				/* read   */
	crashrpt_write,				/* write  */
	NULL,						/* seek   */
	crashrpt_ioctl				/* ioctl  */
#ifndef CONFIG_DISABLE_POLL
	, NULL						/* poll   */
#endif
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	, NULL						/* unlink */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int crashrpt_open(FAR struct file *filep)
{
	return OK;
}

static int crashrpt_close(FAR struct file *filep)
{
	return OK;
}

static ssize_t crashrpt_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t crashrpt_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return len;
}

/****************************************************************************
 * Name: crashrpt_ioctl
 ****************************************************************************/

static int crashrpt_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct tcb_s *rtcb;
	irqstate_t flags;
	int ret = OK;

	switch (cmd) {
	case CRASHRPTIOC_REGISTER: {
		flags = enter_critical_section();
		if (g_reporter_tcb != NULL) {
			leave_critical_section(flags);
			dbg("crash reporter already registered, pid %d\n", g_reporter_pid);
			return -EBUSY;
		}

		rtcb = this_task();
		g_reporter_tcb = rtcb;
		g_reporter_pid = rtcb->pid;
		leave_critical_section(flags);

		lldbg("crash reporter registered, pid %d prio %d\n", g_reporter_pid, rtcb->sched_priority);
	}
	break;

	case CRASHRPTIOC_UNREGISTER: {
		flags = enter_critical_section();
		if (g_reporter_tcb != this_task()) {
			leave_critical_section(flags);
			return -EPERM;
		}
		g_reporter_tcb = NULL;
		g_reporter_pid = 0;
		leave_critical_section(flags);
	}
	break;

	case CRASHRPTIOC_DONE: {
		/* Release the asserting thread.  Only the registered reporter may
		 * do this, otherwise any task could let the assert proceed early.
		 */

		if (g_reporter_tcb != this_task()) {
			return -EPERM;
		}

		if (!g_report_in_progress) {
			/* Nothing is waiting.  Not an error - the deadline may already
			 * have expired and the asserting thread moved on.
			 */
			return OK;
		}

		sem_post(&g_done_sem);
	}
	break;

	case CRASHRPTIOC_FILE_OPEN: {
		FAR struct crashrpt_file_open_s *oarg =
			(FAR struct crashrpt_file_open_s *)arg;
		FAR struct file *fp;

		lldbg("CRASHRPTIOC_FILE_OPEN: path=%s flags=0x%x\n", oarg ? oarg->path : "(null)", oarg ? oarg->oflags : 0);

		if (oarg == NULL) {
			return -EINVAL;
		}

		fp = (FAR struct file *)oarg->file.opaque;

		ret = file_open(fp, oarg->path, oarg->oflags, oarg->mode);
		lldbg("CRASHRPTIOC_FILE_OPEN: file_open returned %d\n", ret);
		if (ret < 0) {
			return ret;
		}
	}
	break;

	case CRASHRPTIOC_FILE_WRITE: {
		FAR struct crashrpt_file_write_s *warg =
			(FAR struct crashrpt_file_write_s *)arg;
		FAR struct file *fp;

		if (warg == NULL || warg->file == NULL) {
			return -EINVAL;
		}

		fp = (FAR struct file *)warg->file->opaque;

		ret = file_write(fp, warg->buf, warg->len);
		if (ret < 0) {
			return ret;
		}
	}
	break;

	case CRASHRPTIOC_FILE_FSYNC: {
		FAR struct crashrpt_file_s *farg = (FAR struct crashrpt_file_s *)arg;
		FAR struct file *fp;

		if (farg == NULL) {
			return -EINVAL;
		}

		fp = (FAR struct file *)farg->opaque;

		ret = file_fsync(fp);
		if (ret < 0) {
			return ret;
		}
	}
	break;

	case CRASHRPTIOC_FILE_CLOSE: {
		FAR struct crashrpt_file_s *carg = (FAR struct crashrpt_file_s *)arg;
		FAR struct file *fp;

		if (carg == NULL) {
			return -EINVAL;
		}

		fp = (FAR struct file *)carg->opaque;

		ret = file_close(fp);
		if (ret < 0) {
			return ret;
		}
	}
	break;

	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: crashrpt_arm_watchdog
 *
 * Description:
 *   Weak default.  Boards should override this with a real hardware
 *   watchdog kick.  See the header for why this matters.
 *
 ****************************************************************************/

void weak_function crashrpt_arm_watchdog(uint32_t timeout_ms)
{
	UNUSED(timeout_ms);
}

/****************************************************************************
 * Name: crashrpt_notify
 ****************************************************************************/

bool crashrpt_notify(int reason)
{
	irqstate_t flags;
	int saved_priority;
	int ret;

	/* Take the re-entry guard first, before any other work.  See the comment
	 * on g_report_in_progress above - this ordering is the single most
	 * important detail in this file.
	 */

	if (g_report_in_progress) {
		return false;
	}
	g_report_in_progress = true;

	if (g_reporter_tcb == NULL) {
		return false;
	}

	/* We must be in ordinary thread context.  If CURRENT_REGS is set we are
	 * inside an exception handler - a hardware fault, or an assert raised
	 * from an interrupt - and the scheduler cannot be used to run the
	 * reporter.  Those cases fall back to whatever the caller can persist
	 * on its own.
	 */

	if (up_interrupt_context()) {
		return false;
	}

	flags = enter_critical_section();

	/* The reporter must actually be parked.  If it is blocked on something
	 * else, or is itself the thread that crashed, waking it does nothing.
	 */

	if (g_reporter_tcb->task_state != TSTATE_WAIT_FIN) {
		leave_critical_section(flags);
		lldbg("crash reporter not parked (state %d), skipping\n", g_reporter_tcb->task_state);
		return false;
	}

	/* Boost so the reporter is certain to be scheduled ahead of us.  This is
	 * a correctness requirement, not a latency optimisation.
	 */

	saved_priority = g_reporter_tcb->sched_priority;
	if (saved_priority < CRASHRPT_BOOST_PRIORITY) {
		(void)sched_setpriority(g_reporter_tcb, CRASHRPT_BOOST_PRIORITY);
	}

	leave_critical_section(flags);

	/* Arm the hardware backstop before handing over control. */

	crashrpt_arm_watchdog(CONFIG_CRASH_REPORT_DEADLINE_MS);

	lldbg("waking crash reporter pid %d, reason %d\n", g_reporter_pid, reason);

	/* Wake it.
	 *
	 * fin_notify() uses the ordinary up_unblock_task(), which saves our
	 * context so that we resume here once the reporter is done.  Do NOT be
	 * tempted to use up_unblock_task_without_savereg() - that variant exists
	 * to discard a broken context on the real crash path and would destroy
	 * the very thread that has to carry on and reboot the board.
	 */

	if (fin_notify(g_reporter_pid, reason) != OK) {
		lldbg("failed to wake crash reporter\n");
		return false;
	}

	/* Wait, bounded.  sem_tickwait() is used rather than sem_timedwait()
	 * because it takes a tick delay directly and does not depend on the
	 * clock_gettime() path.
	 */

	ret = sem_tickwait(&g_done_sem, clock_systimer(), MSEC2TICK(CONFIG_CRASH_REPORT_DEADLINE_MS));
	if (ret != OK) {
		lldbg("crash reporter did not complete within %d ms\n", CONFIG_CRASH_REPORT_DEADLINE_MS);
		return false;
	}

	lldbg("crash report complete\n");
	return true;
}

/****************************************************************************
 * Name: crashrpt_register
 ****************************************************************************/

void crashrpt_register(void)
{
	sem_init(&g_done_sem, 0, 0);

	/* The completion semaphore is a simple signal, so priority inheritance
	 * would only add work on a path that must stay short.
	 */

	sem_setprotocol(&g_done_sem, SEM_PRIO_NONE);

	(void)register_driver(CRASHRPT_DEVPATH, &crashrpt_fops, 0666, NULL);
}

#endif							/* CONFIG_CRASH_REPORT */
