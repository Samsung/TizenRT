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

/// @file stc_sched_monitor.h

/// @brief Background starvation invariant monitor
///
/// The monitor samples the scheduler's task lists at a fixed interval and
/// looks for a single condition:
///
///   no task in the priority band under test stays continuously ready to
///   run, without ever being given a CPU, for longer than a grace period.
///
/// Unlike the individual scenarios, which each target one known mechanism,
/// the monitor is generic.  It does not care *why* a task was passed over,
/// only that it was, so it detects starvation defects that nobody thought
/// to write a scenario for.  It is intended to run concurrently with every
/// other scenario in the suite, which makes the whole suite a starvation
/// detector at close to no extra cost.
///
/// Scheduler state is read through the OS API test driver, which calls
/// sched_foreach() on the caller's behalf.  That walk runs with a critical
/// section held, so the sampling callback only copies fields out; every
/// decision is made afterwards.
///
/// Two limitations, both learned from hardware and both load bearing:
///
/// 1. The monitor perturbs what it observes.  It wakes every sampling
///    period and blocks again, and each time it blocks
///    sched_removereadytorun() runs on its CPU and pulls a waiting task
///    in.  That is rotation, supplied by the observer, at a rate close
///    to CONFIG_RR_INTERVAL.  The monitor must therefore never run
///    alongside a scenario whose oracle is a share of CPU time.
///
/// 2. For the same reason it is blind on its own CPU.  Starvation there
///    is prevented by its own cycling, so on a two CPU target it is in
///    practice a detector for the CPU it is not pinned to.
///
/// It remains the right instrument where it is the thing being read
/// rather than a bystander, which is what SCN-SMP-01 does.

#ifndef __EXAMPLES_TESTCASE_KERNEL_STC_SCHED_MONITOR_H
#define __EXAMPLES_TESTCASE_KERNEL_STC_SCHED_MONITOR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Sampling interval.  Fine enough to see several samples inside one
 * round-robin slice, coarse enough that the monitor itself costs almost no
 * CPU.
 */

#define STC_MON_PERIOD_MS       10

/* Largest number of tasks the monitor tracks in one sample. */

#define STC_MON_MAX_TASKS       32

/* Grace period, expressed in round-robin slices.
 *
 * A task that is waiting its turn legitimately stays ready to run for up to
 * roughly one slice per task ahead of it, so the grace period has to exceed
 * that.  It is set deliberately generously, because of what this monitor is
 * for: it detects tasks that are never scheduled, not tasks that are
 * scheduled late.  A starved task exceeds any grace period immediately and
 * stays over it forever, while a merely unlucky one never gets close.
 * Widening the window therefore removes false positives without weakening
 * detection at all.
 */

#define STC_MON_GRACE_SLICES    10

/* Stack for the monitor task.  It keeps its working set in static storage,
 * so this only has to cover the ioctl path.
 */

#define STC_MON_STACKSIZE       2048

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Start sampling.
 *
 * min_priority bounds what the monitor watches: only tasks at or above this
 * priority are considered.  Pass the priority of the workload under test.
 * Without that bound the monitor would flag ordinary low priority tasks
 * that are correctly being passed over by a busy higher priority workload.
 *
 * Returns OK, or ERROR if the monitor is already running or could not be
 * created.
 */

int stc_monitor_start(int min_priority);

/* Stop sampling and wait for the monitor to exit.
 *
 * Always call this before evaluating any assertion, so that a failing
 * assertion cannot leave the monitor task behind.  Returns OK, or ERROR if
 * the monitor did not exit within its timeout.
 */

int stc_monitor_stop(void);

/* Results from the most recent start and stop cycle. */

uint32_t stc_monitor_violations(void);
uint32_t stc_monitor_samples(void);
uint32_t stc_monitor_max_dwell_ms(void);
pid_t stc_monitor_worst_pid(void);

/* Print a one line summary.  Call outside any measurement window. */

void stc_monitor_report(const char *tag);

#endif							/* __EXAMPLES_TESTCASE_KERNEL_STC_SCHED_MONITOR_H */
