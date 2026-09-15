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
 * os/include/tinyara/crashrpt.h
 *
 * Crash diagnostic capture - FIN_WAIT + timed wait method.
 *
 * An application registers a high priority "reporter" thread which parks in
 * fin_wait().  When the kernel is about to begin assert handling it wakes
 * that thread and blocks, bounded by a deadline, until the reporter signals
 * that it has finished writing its diagnostics.  Assert handling then
 * proceeds normally.
 *
 * The hook runs at the very first statement of up_assert(), before the
 * critical section is taken, before the other CPUs are paused and before the
 * abort state is recorded.  For an application generated assert the system
 * is therefore still completely healthy at that point.
 *
 * Application side usage:
 *
 *   static int crash_reporter(int argc, char *argv[])
 *   {
 *       int fd = open(CRASHRPT_DEVPATH, O_RDWR);
 *       if (fd < 0) {
 *           return ERROR;
 *       }
 *
 *       if (ioctl(fd, CRASHRPTIOC_REGISTER, 0) < 0) {
 *           close(fd);
 *           return ERROR;
 *       }
 *
 *       for (;;) {
 *           int reason = fin_wait();       // parks here until a crash
 *
 *           collect_product_diagnostics(reason);
 *           write_report_file(reason);
 *
 *           ioctl(fd, CRASHRPTIOC_DONE, 0);
 *       }
 *   }
 *
 *   // create it at a high priority - it must outrank everything runnable
 *   task_create("crashrpt", SCHED_PRIORITY_MAX - 1, 4096, crash_reporter, NULL);
 *
 * Notes for the reporter implementation:
 *
 *   - Keep the work short.  The reboot is held up for its whole duration.
 *   - Prefer a raw partition or an append only log.  Writing to a mounted
 *     filesystem straight after a crash risks corrupting it.
 *   - Do not call ASSERT() from the reporter.  The re-entry guard will stop
 *     the recursion, but the report is then lost.
 *
 ****************************************************************************/

#ifndef __INCLUDE_TINYARA_CRASHRPT_H
#define __INCLUDE_TINYARA_CRASHRPT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <sys/types.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_CRASH_REPORT

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CRASHRPT_DEVPATH        "/dev/crashrpt"

/* IOCTL commands ***********************************************************/

/* CRASHRPTIOC_REGISTER
 *   Register the calling thread as the crash reporter.  The caller must then
 *   park itself with fin_wait().  Only one reporter may be registered.
 *   Argument: none.
 *
 * CRASHRPTIOC_UNREGISTER
 *   Remove the calling thread as the crash reporter.
 *   Argument: none.
 *
 * CRASHRPTIOC_DONE
 *   Signal that diagnostic capture has completed.  Releases the asserting
 *   thread so that assert handling can continue.
 *   Argument: none.
 */

#define CRASHRPTIOC_REGISTER    _CRASHRPTIOC(0x0001)
#define CRASHRPTIOC_UNREGISTER  _CRASHRPTIOC(0x0002)
#define CRASHRPTIOC_DONE        _CRASHRPTIOC(0x0003)

/* CRASHRPTIOC_FILE_OPEN
 *   Open a file using the kernel-internal file_open() which detaches the
 *   struct file from the thread's file descriptor table.  This is needed
 *   because the standard open()/write() syscalls go through fs_getfilep()
 *   -> sched_getfiles() which may fail in the crash path.
 *
 *   Argument: pointer to struct crashrpt_file_open_s.
 *   Returns: 0 on success, negated errno on failure.  The caller must pass
 *   the crashrpt_file_s back to CRASHRPTIOC_FILE_WRITE / FILE_CLOSE.
 *
 * CRASHRPTIOC_FILE_WRITE
 *   Write to a file opened with CRASHRPTIOC_FILE_OPEN.
 *   Argument: pointer to struct crashrpt_file_write_s.
 *   Returns: number of bytes written, or negated errno on failure.
 *
 * CRASHRPTIOC_FILE_FSYNC
 *   Flush a file to the medium.
 *   Argument: pointer to struct crashrpt_file_s.
 *   Returns: 0 on success, negated errno on failure.
 *
 * CRASHRPTIOC_FILE_CLOSE
 *   Close a file opened with CRASHRPTIOC_FILE_OPEN.
 *   Argument: pointer to struct crashrpt_file_s.
 *   Returns: 0 on success, negated errno on failure.
 */

#define CRASHRPTIOC_FILE_OPEN   _CRASHRPTIOC(0x0004)
#define CRASHRPTIOC_FILE_WRITE  _CRASHRPTIOC(0x0005)
#define CRASHRPTIOC_FILE_FSYNC  _CRASHRPTIOC(0x0006)
#define CRASHRPTIOC_FILE_CLOSE  _CRASHRPTIOC(0x0007)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Reason codes.  The value is returned to the reporter thread as the return
 * value of its fin_wait() call.  Values must be distinct from NO_FIN_DATA.
 */

enum crashrpt_reason_e {
	CRASHRPT_REASON_UNKNOWN        = 1,	/* Reason could not be determined  */
	CRASHRPT_REASON_APP_ASSERT     = 2,	/* Application called ASSERT()     */
	CRASHRPT_REASON_KERNEL_ASSERT  = 3,	/* Kernel called ASSERT()          */
	CRASHRPT_REASON_HW_FAULT       = 4,	/* Hardware fault or CPU abort     */
};

/* Opaque handle for a file opened through the crash report driver.  The
 * internals are only meaningful to the driver; the application treats this
 * as an opaque token and passes a pointer to it back to FILE_WRITE /
 * FILE_FSYNC / FILE_CLOSE.
 */

struct crashrpt_file_s {
	void *opaque[4];		/* struct file, reserved */
};

struct crashrpt_file_open_s {
	const char *path;		/* Path to open */
	int oflags;				/* Open flags (O_WRONLY | O_CREAT | ...) */
	mode_t mode;			/* File mode */
	struct crashrpt_file_s file;	/* Filled in by the driver */
};

struct crashrpt_file_write_s {
	struct crashrpt_file_s *file;	/* From CRASHRPTIOC_FILE_OPEN */
	const void *buf;		/* Data to write */
	size_t len;				/* Number of bytes */
};


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: crashrpt_register
 *
 * Description:
 *   Create the /dev/crashrpt device.  Called from up_initialize().
 *
 ****************************************************************************/

void crashrpt_register(void);

/****************************************************************************
 * Name: crashrpt_notify
 *
 * Description:
 *   Wake the registered reporter thread and wait, bounded by
 *   CONFIG_CRASH_REPORT_DEADLINE_MS, for it to finish.
 *
 *   MUST be called as the very first statement of up_assert(), before the
 *   critical section is entered.  Calling it later is unsafe - by then
 *   interrupts are disabled, the other CPUs are paused and the scheduler
 *   cannot run the reporter.
 *
 *   The guard against re-entry is taken inside this function before anything
 *   else happens.  This matters because up_assert() has not yet recorded the
 *   abort state, so a fault raised by the reporter would otherwise look like
 *   a brand new assert and re-enter here indefinitely.
 *
 * Input Parameters:
 *   reason - One of enum crashrpt_reason_e.
 *
 * Returned Value:
 *   true  - The reporter ran and signalled completion.
 *   false - No reporter, wrong context, or the deadline expired.  The caller
 *           should fall back to whatever it can persist without help.
 *
 ****************************************************************************/

bool crashrpt_notify(int reason);

/****************************************************************************
 * Name: crashrpt_arm_watchdog
 *
 * Description:
 *   Arm the hardware watchdog before control is handed to the reporter.
 *
 *   The default implementation is a no-op and is declared weak.  Boards
 *   SHOULD override it.  The watchdog is the only bound that genuinely holds
 *   once something has already gone wrong - the deadline enforced by this
 *   driver relies on a working scheduler, which cannot be assumed.
 *
 * Input Parameters:
 *   timeout_ms - Suggested timeout, derived from the configured deadline.
 *
 ****************************************************************************/

void crashrpt_arm_watchdog(uint32_t timeout_ms);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* CONFIG_CRASH_REPORT */
#endif							/* __INCLUDE_TINYARA_CRASHRPT_H */
