/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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
#include <stdint.h>
#include <errno.h>
#include <tinyara/kmalloc.h>
#include <tinyara/watchdog.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/wqueue.h>
#include <tinyara/clock.h>
#include <tinyara/reboot_reason.h>
#include <arch/reboot_reason.h>
#include "wdt.h"
#include "wdt_hal.h"
#include "hal_aon_rtc_types.h"
#include "aon_rtc.h"

#ifdef CONFIG_WATCHDOG

extern uint32_t sys_hal_nmi_wdt_get_clk_div(void);
extern void bk_wdt_force_feed(void);

#if !defined(CONFIG_ARMINO_WDG_SLICE_MAX_MS) || (CONFIG_ARMINO_WDG_SLICE_MAX_MS < 1)
#define ARMINO_WDG_SLICE_CAP_MS 30000u
#else
#define ARMINO_WDG_SLICE_CAP_MS ((uint32_t)CONFIG_ARMINO_WDG_SLICE_MAX_MS)
#endif

#if !defined(CONFIG_ARMINO_WDG_MAX_SOFT_TIMEOUT_MS) || (CONFIG_ARMINO_WDG_MAX_SOFT_TIMEOUT_MS < 1)
#define ARMINO_WDG_MAX_SOFT_MS 300000u
#else
#define ARMINO_WDG_MAX_SOFT_MS ((uint32_t)CONFIG_ARMINO_WDG_MAX_SOFT_TIMEOUT_MS)
#endif

#define WDT_STOP	0
#define WDT_START	1
#define WDT_PAUSE	2
#define ARMINO_WDG_THREAD_FEED_LONG_MS 15000u
#define ARMINO_WDG_THREAD_FEED_MS 5000u
#define ARMINO_WDG_THREAD_FEED_SWITCH_MS 60000u
#define ARMINO_WDG_PANIC_RESET_FALLBACK_MS 2000u

/* Design boundary:
 * - NMI reset mode does direct register feed then returns immediately.
 * - Minute-level timeout strategy and PANIC are thread-side (hpwork).
 * - Before PANIC, a short HW watchdog window is armed to guarantee reset
 *   even if reboot flow stalls after dump.
 */

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * well-known watchdog_lowerhalf_s structure.
 */
struct armino_wdg_lowerhalf_s {
	FAR const struct watchdog_ops_s *ops;
	xcpt_t handler;
	uint32_t timeout_ms;
	uint32_t timeout_left;		/* Record remaining timeout for PAUSE and RESUME */
	uint32_t wdt_status;		/* 0: WDT_STOP; 1: WDT_START; 2: WDT_PAUSE */
	uint64_t start_tick;		/* 64-bit RTC baseline for elapsed-time calculation */
	uint32_t hw_feed_period_ms;	/* HW period used for NMI keepalive feed */
	uint32_t hw_feed_slots;		/* Logical timeout windows split by HW period */
	struct work_s feed_work;	/* Thread-side keepalive/panic scheduler */
	volatile bool panic_firing;	/* True once timeout panic path is entered */
	bool int_mode;				/* True for timeout interrupt mode on */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int armino_wdg_start(FAR struct watchdog_lowerhalf_s *lower);
static int armino_wdg_stop(FAR struct watchdog_lowerhalf_s *lower);
static int armino_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower);
static int armino_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status);
static int armino_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout);
static xcpt_t armino_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler);
static int armino_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg);
static uint32_t armino_wdt_hw_max_ms(void);
static uint32_t armino_wdg_ticks_to_ms(uint64_t ticks);
static uint32_t armino_wdg_elapsed_ms(FAR struct armino_wdg_lowerhalf_s *priv);
static uint32_t armino_wdg_worker_delay_ms(FAR struct armino_wdg_lowerhalf_s *priv);
static void armino_wdg_feed_worker(FAR void *arg);
static void armino_wdg_schedule_feed_worker(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t delay_ms);
static void armino_wdg_cancel_feed_worker(FAR struct armino_wdg_lowerhalf_s *priv);
static void armino_wdg_start_hw(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t ms);
static void armino_wdg_update_hw_feed_plan(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t logical_ms);
static void armino_wdg_arm_hw_feed_window(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t logical_ms);

/****************************************************************************
 * Private Data
 ****************************************************************************/

const struct watchdog_ops_s g_wdgops = {
	.start = armino_wdg_start,
	.stop = armino_wdg_stop,
	.keepalive = armino_wdg_keepalive,
	.getstatus = armino_wdg_getstatus,
	.settimeout = armino_wdg_settimeout,
	.capture = armino_wdg_capture,
	.ioctl = armino_wdg_ioctl,
};

/* Global gate for NMI direct-feed path.
 * - true : NMI handler may feed hardware watchdog.
 * - false: NMI handler must not feed (used by force-reboot paths).
 */
static volatile bool g_wdt_nmi_feed_enabled = true;

/* NMI deadman: bound how long NMI alone can keep the HW watchdog alive
 * if the HPWORK feeder thread is stalled.
 *
 * Mechanism:
 *   - For each logical timeout arm, derive a dynamic credit reload:
 *       reload = hw_feed_slots - 1
 *     where hw_feed_slots = ceil(logical_timeout / hw_feed_period).
 *   - HPWORK replenishes g_wdt_nmi_feed_credit to the reload value after
 *     every successful direct HW feed (proof that the thread side is alive).
 *   - NMI consumes one credit per fire; when credit hits zero, NMI stops
 *     feeding so the hardware watchdog can expire and reset the system.
 *
 * Result:
 *   In HPWORK-hung scenarios, NMI-only keepalive window scales with
 *   user timeout instead of a fixed constant.
 *
 * Concurrency note:
 *   - HPWORK only does a single 32-bit store (replenish), which is atomic
 *     on ARMv8-M.
 *   - NMI's read-decrement-write is uninterruptible because NMI is the
 *     highest priority exception on this core.
 *   - No locking is required.
 */
static volatile int32_t g_wdt_nmi_feed_credit;
static volatile int32_t g_wdt_nmi_feed_credit_reload;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: armino_wdt_hw_max_ms
 *
 * Description:
 *   Max milliseconds valid for bk_wdt_start such that hardware period scaling
 *   internal multiply-by-multi does not exceed 16-bit period.
 *
 ****************************************************************************/

static uint32_t armino_wdt_hw_max_ms(void)
{
	switch (sys_hal_nmi_wdt_get_clk_div()) {
	case 0:
		return 0xFFFFu / 16u;
	case 1:
		return 0xFFFFu / 8u;
	case 2:
		return 0xFFFFu / 4u;
	case 3:
		return 0xFFFFu / 2u;
	default:
		return 0xFFFFu;
	}
}

static uint32_t armino_wdg_ticks_to_ms(uint64_t ticks)
{
	uint32_t freq = bk_rtc_get_clock_freq();

	if (freq == 0u) {
		return 0u;
	}

	return (uint32_t)((ticks * 1000ULL) / (uint64_t)freq);
}

static uint32_t armino_wdg_elapsed_ms(FAR struct armino_wdg_lowerhalf_s *priv)
{
	uint64_t now_tick = (uint64_t)bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	uint64_t elapsed_ticks = now_tick - priv->start_tick;
	return armino_wdg_ticks_to_ms(elapsed_ticks);
}

static uint32_t armino_wdg_worker_delay_ms(FAR struct armino_wdg_lowerhalf_s *priv)
{
	uint32_t elapsed_ms;
	uint32_t remaining_ms;
	uint32_t delay_ms = ARMINO_WDG_THREAD_FEED_MS;

	elapsed_ms = armino_wdg_elapsed_ms(priv);
	if (elapsed_ms >= priv->timeout_left) {
		remaining_ms = 0u;
	} else {
		remaining_ms = priv->timeout_left - elapsed_ms;
	}

	/* Two-stage hpwork cadence:
	 * - Far from timeout: 15 s to reduce scheduling overhead.
	 * - Within last minute: 5 s to keep timeout reaction responsive.
	 * Use real-time remaining window instead of static timeout_left.
	 */
	if (remaining_ms > ARMINO_WDG_THREAD_FEED_SWITCH_MS) {
		delay_ms = ARMINO_WDG_THREAD_FEED_LONG_MS;
	}

	if (remaining_ms > 0u && remaining_ms < delay_ms) {
		delay_ms = remaining_ms;
	}

	return delay_ms;
}

static void armino_wdg_feed_worker(FAR void *arg)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)arg;
	uint32_t elapsed_ms;

	if (!priv || priv->wdt_status != WDT_START || priv->int_mode) {
		return;
	}

	elapsed_ms = armino_wdg_elapsed_ms(priv);
	if (elapsed_ms >= priv->timeout_left) {
		priv->panic_firing = true;
		/* Re-arm a short HW watchdog window before PANIC.
		 * If board reboot flow stalls after dump, HW reset is still guaranteed.
		 */
		(void)bk_wdt_start(ARMINO_WDG_PANIC_RESET_FALLBACK_MS);
#ifdef CONFIG_SYSTEM_REBOOT_REASON
		/* Mark this panic as watchdog timeout before asserting.
		 * up_assert() writes ASSERT reason only if no reason exists yet.
		 */
		up_reboot_reason_write(REBOOT_SYSTEM_WATCHDOG);
#endif
		PANIC();
		return;
	}

	bk_wdt_force_feed();
	/* Proof of life for NMI deadman: HPWORK is making progress. */
	g_wdt_nmi_feed_credit = g_wdt_nmi_feed_credit_reload;
	armino_wdg_schedule_feed_worker(priv, armino_wdg_worker_delay_ms(priv));
}

static void armino_wdg_schedule_feed_worker(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t delay_ms)
{
	uint32_t ticks = MSEC2TICK(delay_ms);

	if (ticks == 0u) {
		ticks = 1u;
	}

	(void)work_queue(HPWORK, &priv->feed_work, armino_wdg_feed_worker, priv, ticks);
}

static void armino_wdg_cancel_feed_worker(FAR struct armino_wdg_lowerhalf_s *priv)
{
	(void)work_cancel(HPWORK, &priv->feed_work);
}

static void armino_wdg_start_hw(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t ms)
{
	int ret;

	g_wdt_nmi_feed_enabled = true;
	ret = bk_wdt_start(ms);
	if (ret != BK_OK) {
		/* Defensive fallback: if HW arm fails, close NMI feed gate to avoid
		 * false "watchdog is armed" assumptions in subsequent flow.
		 */
		g_wdt_nmi_feed_enabled = false;
		llvdbg("bk_wdt_start(%u) failed: %d\n", ms, ret);
		DEBUGASSERT(ret == BK_OK);
	}
	(void)priv;
}

/****************************************************************************
 * Name: armino_wdg_update_hw_feed_plan
 *
 * Description:
 *   Derive hw feed period/slots from logical timeout (milliseconds).
 *
 ****************************************************************************/

static void armino_wdg_update_hw_feed_plan(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t logical_ms)
{
	uint32_t hw_max;
	uint32_t slice_cap;
	uint32_t n;

	if (logical_ms == 0) {
		priv->hw_feed_period_ms = 0;
		priv->hw_feed_slots = 1;
		return;
	}

	hw_max = armino_wdt_hw_max_ms();
	slice_cap = ARMINO_WDG_SLICE_CAP_MS;
	if (slice_cap > hw_max) {
		slice_cap = hw_max;
	}

	n = (logical_ms + slice_cap - 1u) / slice_cap;
	if (n == 0) {
		n = 1;
	}

	priv->hw_feed_period_ms = slice_cap;
	priv->hw_feed_slots = n;
}

/****************************************************************************
 * Name: armino_wdg_arm_hw_feed_window
 *
 * Description:
 *   Program watchdog hardware window from current plan.
 *
 ****************************************************************************/

static void armino_wdg_arm_hw_feed_window(FAR struct armino_wdg_lowerhalf_s *priv, uint32_t logical_ms)
{
	uint32_t hw_window_ms;

	if (priv->hw_feed_slots == 1u) {
		hw_window_ms = logical_ms;
	} else {
		hw_window_ms = priv->hw_feed_period_ms;
	}

	/* Dynamic deadman credit: make NMI-only hang window follow timeout.
	 * credits = number of intermediate HW slices before the final one.
	 * slots=1 means no intermediate slice => no NMI feed credit.
	 */
	if (priv->hw_feed_slots > 0u) {
		g_wdt_nmi_feed_credit_reload = (int32_t)(priv->hw_feed_slots - 1u);
	} else {
		g_wdt_nmi_feed_credit_reload = 0;
	}
	g_wdt_nmi_feed_credit = g_wdt_nmi_feed_credit_reload;

	armino_wdg_start_hw(priv, hw_window_ms);
}

/****************************************************************************
 * Name: armino_nmi_interrupt
 *
 * Description:
 *   WDT timeout interrupt
 *
 *   NMI context restrictions (must keep this handler minimal):
 *   - DO NOT call any blocking APIs (sleep/usleep/wait/sem/mutex/queue, etc.)
 *   - DO NOT call complex OS services (workqueue, memory alloc/free, filesystem,
 *     or other paths that may depend on scheduler state)
 *   - DO NOT call normal logging interfaces (printf/lldbg/syslog/BK_LOG*, etc.)
 *     because logging paths may use locks/buffers/queues and are not NMI-safe
 *   - Keep operations deterministic and short; prefer direct register access only
 *
 *   Deadman in RESET mode:
 *     The handler will refuse to feed once the HPWORK-supplied credit
 *     (g_wdt_nmi_feed_credit) is exhausted. This guarantees that a stalled
 *     HPWORK cannot be hidden by NMI keepalive: after a bounded number of
 *     NMI cycles without HPWORK progress, NMI stops feeding and the HW
 *     watchdog is allowed to reset the system.
 *
 * Input Parameters:
 *   Usual interrupt handler arguments.
 *
 * Returned Values:
 *   Always returns OK.
 *
 ****************************************************************************/
static int __attribute__((section(".iram"), noinline))
armino_nmi_interrupt(int irq, void *context, FAR void *arg)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)arg;

	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	(void)irq;
	(void)context;

	if (!g_wdt_nmi_feed_enabled) {
		return OK;
	}

	if (!priv->int_mode) {
		/* RESET mode: keepalive by direct register writes only.
		 * During panic/reboot flow, stop feeding so HW fallback reset can fire.
		 */
		if (priv->panic_firing) {
			return OK;
		}

		/* NMI deadman: HPWORK must replenish credit to keep us feeding.
		 * If credit is exhausted, the HPWORK feeder thread has not made
		 * progress for several NMI cycles (likely stalled). Stop feeding
		 * so the hardware watchdog can expire and reset the system,
		 * preventing indefinite NMI-only keepalive of a hung system.
		 */
		if (g_wdt_nmi_feed_credit <= 0) {
			return OK;
		}
		g_wdt_nmi_feed_credit--;

		wdt_hal_nmi_direct_feed();
		return OK;
	}

	/* Capture mode compatibility path: feed once then invoke user handler. */
	if (priv->handler) {
		wdt_hal_nmi_direct_feed();
		priv->handler(irq, context, NULL);
	}

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_start
 *
 * Description:
 *   Start the watchdog timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int armino_wdg_start(FAR struct watchdog_lowerhalf_s *lower)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	if (priv->wdt_status == WDT_STOP) {
		/* Start watchdog timer */
		priv->panic_firing = false;
		armino_wdg_update_hw_feed_plan(priv, priv->timeout_ms);
		armino_wdg_arm_hw_feed_window(priv, priv->timeout_ms);
		priv->wdt_status = WDT_START;
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record time right after wdt start
		priv->timeout_left = priv->timeout_ms;	/* epoch for GETSTATUS timeleft */
		armino_wdg_cancel_feed_worker(priv);
		armino_wdg_schedule_feed_worker(priv, armino_wdg_worker_delay_ms(priv));
		return OK;
	}

	return -EBUSY;
}

/****************************************************************************
 * Name: armino_wdg_stop
 *
 * Description:
 *   Stop the watchdog timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int armino_wdg_stop(FAR struct watchdog_lowerhalf_s *lower)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/* Stop watchdog timer */
	armino_wdg_cancel_feed_worker(priv);
	close_wdt();
	g_wdt_nmi_feed_enabled = false;
	g_wdt_nmi_feed_credit = 0;
	g_wdt_nmi_feed_credit_reload = 0;
	priv->hw_feed_slots = 0;
	priv->panic_firing = false;
	priv->wdt_status = WDT_STOP;

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_keepalive
 *
 * Description:
 *   Reset the watchdog timer to the current timeout value, prevent any
 *   imminent watchdog timeouts.  This is sometimes referred as "pinging"
 *   the watchdog timer or "petting the dog".
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int armino_wdg_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}
	if (priv->wdt_status == WDT_START) {
		/* Petting the dog: restart logical window from full timeout */
		priv->panic_firing = false;
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		priv->timeout_left = priv->timeout_ms;
		armino_wdg_update_hw_feed_plan(priv, priv->timeout_ms);
		armino_wdg_arm_hw_feed_window(priv, priv->timeout_ms);
		armino_wdg_cancel_feed_worker(priv);
		armino_wdg_schedule_feed_worker(priv, armino_wdg_worker_delay_ms(priv));
		return OK;
	}

	return -EPERM;
}

/****************************************************************************
 * Name: armino_wdg_getstatus
 *
 * Description:
 *   Get the current watchdog timer status
 *
 * Input Parameters:
 *   lower  - A pointer the publicly visible representation of the "lower-half"
 *            driver state structure.
 *   status - The location to return the watchdog status information.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int armino_wdg_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	/* Reture watchdog flag */
	status->flags = WDFLAGS_RESET;

	/* Return CAPTURE if interrupt mode is enabled */
	if (priv->int_mode) {
		status->flags |= WDFLAGS_CAPTURE;
	}

	/* Reture timeout in ms */
	status->timeout = priv->timeout_ms;

	/* Return WAIT if watchdog timer is on pause */
	if (priv->wdt_status == WDT_PAUSE) {
		status->flags |= WDFLAGS_WAIT;
		status->timeleft = priv->timeout_left;
	} else {
		uint32_t time_elapse_ms;
		uint64_t now_tick;
		uint64_t elapsed_ticks;

		/* Remaining in the current arm: timeout_left at last start/resume/keepalive
		 * minus wall time since start_tick.
		 */
		now_tick = (uint64_t)bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		elapsed_ticks = now_tick - priv->start_tick;
		time_elapse_ms = armino_wdg_ticks_to_ms(elapsed_ticks);
		if (time_elapse_ms > priv->timeout_left) {
			time_elapse_ms = priv->timeout_left;
		}

		status->timeleft = priv->timeout_left - time_elapse_ms;

		/* Return ACTIVE if watchdog timer is started */
		if (priv->wdt_status == WDT_START) {
			status->flags |= WDFLAGS_ACTIVE;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the watchdog timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   timeout - The new timeout value in milliseconds.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int armino_wdg_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	if (timeout > ARMINO_WDG_MAX_SOFT_MS) {
		return -EINVAL;
	}

	/*  Reset the watchdog timer with new timeout value */
	priv->timeout_ms = timeout;

	/* Re-start watchdog only if the status is WDT_START */
	if (priv->wdt_status == WDT_START) {
		priv->panic_firing = false;
		armino_wdg_update_hw_feed_plan(priv, priv->timeout_ms);
		armino_wdg_arm_hw_feed_window(priv, priv->timeout_ms);
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record time right after wdt start
		priv->timeout_left = priv->timeout_ms;
		armino_wdg_cancel_feed_worker(priv);
		armino_wdg_schedule_feed_worker(priv, armino_wdg_worker_delay_ms(priv));
	}

	return OK;
}

/****************************************************************************
 * Name: armino_wdg_capture
 *
 * Description:
 *   Don't reset on watchdog timer timeout; instead, call this user provider
 *   timeout handler.  NOTE:  Providing handler==NULL will restore the reset
 *   behavior.
 *
 * Input Parameters:
 *   lower      - A pointer the publicly visible representation of the "lower-half"
 *                driver state structure.
 *   newhandler - The new watchdog expiration function pointer.  If this
 *                function pointer is NULL, then the reset-on-expiration
 *                behavior is restored,
 *
 * Returned Values:
 *   The previous watchdog expiration function pointer or NULL is there was
 *   no previous function pointer, i.e., if the previous behavior was
 *   reset-on-expiration (NULL is also returned if an error occurs).
 *
 ****************************************************************************/
static xcpt_t armino_wdg_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	xcpt_t oldhandler;
	DEBUGASSERT(priv);
	if (!priv) {
		return NULL;
	}

	/* Record previous handler as the oldhandler */
	oldhandler = priv->handler;

	/* Set the new handler */
	priv->handler = handler;

	/* Set to default RESET_MODE first */
	if (priv->wdt_status == WDT_START) {
		uint64_t now_tick = (uint64_t)bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		uint64_t elapsed_ticks = now_tick - priv->start_tick;
		uint32_t time_elapse_ms = armino_wdg_ticks_to_ms(elapsed_ticks);

		if (time_elapse_ms > priv->timeout_left) {
			time_elapse_ms = priv->timeout_left;
		}

		priv->timeout_left -= time_elapse_ms;
	}
	priv->timeout_ms = priv->timeout_left;
	if (priv->timeout_ms > ARMINO_WDG_MAX_SOFT_MS) {
		priv->timeout_ms = ARMINO_WDG_MAX_SOFT_MS;
	}

	priv->int_mode = false;

	/* Change to INT_MODE if handler exists */
	if (handler) {
		priv->int_mode = true;
	}

	/* Re-start watchdog only if the status is WDT_START */
	if (priv->wdt_status == WDT_START) {
		priv->panic_firing = false;
		armino_wdg_update_hw_feed_plan(priv, priv->timeout_ms);
		armino_wdg_arm_hw_feed_window(priv, priv->timeout_ms);
		priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);	// Record time right after wdt start
		priv->timeout_left = priv->timeout_ms;
		armino_wdg_cancel_feed_worker(priv);
		armino_wdg_schedule_feed_worker(priv, armino_wdg_worker_delay_ms(priv));
	} else {
		armino_wdg_cancel_feed_worker(priv);
	}

	return oldhandler;
}

/****************************************************************************
 * Name: armino_wdg_ioctl
 *
 * Description:
 *   Any ioctl commands that are not recognized by the "upper-half" driver
 *   are forwarded to the lower half driver through this method.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *   cmd   - The ioctl command value
 *   arg   - The optional argument that accompanies the 'cmd'.  The
 *           interpretation of this argument depends on the particular
 *           command.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int armino_wdg_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg)
{
	struct armino_wdg_lowerhalf_s *priv = (struct armino_wdg_lowerhalf_s *)lower;
	DEBUGASSERT(priv);
	if (!priv) {
		return -ENODEV;
	}

	switch (cmd) {
	case WDIOC_PAUSE: {
		uint64_t now_tick;
		uint64_t elapsed_ticks;
		uint32_t time_elapse_ms;

		/* Pause is valid only while watchdog is running. */
		if (priv->wdt_status != WDT_START) {
			return -EPERM;
		}

		/* Freeze remaining = epoch timeout_left minus time since start_tick */
		now_tick = (uint64_t)bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		elapsed_ticks = now_tick - priv->start_tick;
		time_elapse_ms = armino_wdg_ticks_to_ms(elapsed_ticks);
		if (time_elapse_ms > priv->timeout_left) {
			time_elapse_ms = priv->timeout_left;
		}

		priv->timeout_left -= time_elapse_ms;

		armino_wdg_stop((struct watchdog_lowerhalf_s *)priv);
		priv->wdt_status = WDT_PAUSE;
	}
	break;

	case WDIOC_RESUME: {
		/* Pre-assumption: the WDIOC_PAUSE can only be resumed by WDIOC_RESUME */
		if (priv->wdt_status == WDT_PAUSE) {
			priv->wdt_status = WDT_START;
			armino_wdg_update_hw_feed_plan(priv, priv->timeout_left);
			armino_wdg_arm_hw_feed_window(priv, priv->timeout_left);
			priv->start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
			armino_wdg_cancel_feed_worker(priv);
			armino_wdg_schedule_feed_worker(priv, armino_wdg_worker_delay_ms(priv));
			/* timeout_left unchanged: remaining ms for this arm (GETSTATUS epoch) */
		}
	}
	break;

	default: {
		llvdbg("invalid cmd for wdg ioctl.\n");
		return -EINVAL;
	}
	}
	return OK;
}

/****************************************************************************
 * Name: armino_wdg_initialize
 *
 * Description:
 *   Initialize and register the armino watchdog. Watchdog timer is registered
 *   as 'devpath'.
 *
 ****************************************************************************/
int armino_wdg_initialize(const char *devpath)
{
	struct armino_wdg_lowerhalf_s *priv = NULL;
	priv = (struct armino_wdg_lowerhalf_s *)kmm_zalloc(sizeof(struct armino_wdg_lowerhalf_s));

	if (!priv) {
		llvdbg("wdg_lowerhalf_s allocation error.\n");
		return -ENOMEM;
	}

	/* Set the priv to default */
	priv->ops = &g_wdgops;
	priv->handler = NULL;

	priv->wdt_status = WDT_STOP;
	priv->int_mode = false;
	priv->panic_firing = false;

	irq_attach(ARMINO_IRQ_NMI, armino_nmi_interrupt, (void *)priv);
	up_enable_irq(ARMINO_IRQ_NMI);

	/* Register the watchdog driver as /dev/wdgX. */
	if (!watchdog_register(devpath, (FAR struct watchdog_lowerhalf_s *)priv)) {
		/*
		 * The actual cause of the failure may have been a failure to allocate
		 * perhaps a failure to register the timer driver (such as if the
		 * 'depath' were not unique).  We know here but we return EEXIST to
		 * indicate the failure (implying the non-unique devpath).
		 */
		kmm_free(priv);
		return -EEXIST;
	}

	return OK;
}

#ifdef CONFIG_WATCHDOG_FOR_IRQ
void up_wdog_init(uint16_t timeout)
{
	bk_wdt_start(timeout);
}

#endif

void up_wdog_keepalive(void)
{
	bk_wdt_feed();
}

void up_watchdog_disable(void)
{
	g_wdt_nmi_feed_enabled = false;
	g_wdt_nmi_feed_credit = 0;
	g_wdt_nmi_feed_credit_reload = 0;
	bk_wdt_stop();
}

#endif							//CONFIG_WATCHDOG
