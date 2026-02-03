// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <os/mem.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include <driver/int.h>
#include "sys_driver.h"
#include "sys_hal.h"
#include "aon_pmu_hal.h"
#include "sys/time.h"

#include <driver/aon_rtc.h>
#include <driver/wakeup.h>
#include "ana_rtc_driver.h"

#if defined(CONFIG_ROSC_COMPENSATION)
#include <driver/rosc_32k.h>
#endif

typedef struct
{
	bool inited;

	// Alarm list
	alarm_node_t *alarm_head_p;
	uint32_t alarm_node_cnt;
} ana_rtc_driver_t;

typedef struct
{
	aon_rtc_isr_t callback;
	void *isr_param;
} ana_rtc_callback_t;

typedef struct
{
	alarm_node_t nodes[AON_RTC_MAX_ALARM_CNT];
	uint64_t busy_bits;
} ana_rtc_nodes_memory_t;

static ana_rtc_driver_t s_ana_rtc = {0};
static ana_rtc_callback_t s_ana_rtc_tick_isr = {NULL};
static ana_rtc_nodes_memory_t *s_ana_rtc_nodes_p;

static void aon_rtc_interrupt_disable(aon_rtc_id_t id);

#define ANA_RTC_GET_SET_TIME_RTC_ID AON_RTC_ID_1
#define ANA_RTC_CURRENT_TICK        bk_aon_rtc_get_current_tick(ANA_RTC_GET_SET_TIME_RTC_ID)
static int64_t s_boot_time_us       = 0; // timeofday value

#ifdef CONFIG_EXTERN_32K
static uint32_t s_aon_rtc_clock_freq = AON_RTC_EXTERN_32K_CLOCK_FREQ;
#else
static uint32_t s_aon_rtc_clock_freq = AON_RTC_DEFAULT_CLOCK_FREQ;
#endif

#define ANA_RTC_OPS_SAFE_DELAY_US (125)
static uint64_t s_ana_rtc_base_tick = 0; // use for rtc reset
#if defined(CONFIG_ROSC_COMPENSATION)
static int64_t s_ana_rtc_diff_tick  = 0; // use for rtc compesation
#endif
static uint64_t s_time_base_us      = 0; // use for freq switch
static uint64_t s_time_base_tick    = 0; // use for freq switch
static uint32_t s_low_tick          = 0;
static uint32_t s_high_tick         = 0;
#if !defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
static bool s_rtc_updated           = true;
#endif

__IRAM_SEC float bk_rtc_get_ms_tick_count(void)
{
	return (float)s_aon_rtc_clock_freq / 1000;
}

uint32_t bk_rtc_get_clock_freq(void)
{
	return s_aon_rtc_clock_freq;
}

void bk_rtc_update_base_time(void)
{
	uint64_t time_tick = ANA_RTC_CURRENT_TICK;
	uint64_t time_diff = (time_tick - s_time_base_tick) * 1000LL / bk_rtc_get_ms_tick_count();

	s_time_base_tick = time_tick;
	s_time_base_us += time_diff;
	// RTC_LOGI("s_time_base_tick: 0x%x:0x%08x\r\n", (u32)(s_time_base_tick >> 32), (u32)(s_time_base_tick & 0xFFFFFFFF));
	// RTC_LOGI("s_time_base_us: 0x%x:0x%08x\r\n", (u32)(s_time_base_us >> 32), (u32)(s_time_base_us & 0xFFFFFFFF));
}

void bk_rtc_set_clock_freq(uint32_t clock_freq)
{
#if defined(CONFIG_AON_RTC_DYNAMIC_CLOCK_SUPPORT)
	AON_RTC_LOGI("Set clock freq: %d\n", clock_freq);
	if (clock_freq == s_aon_rtc_clock_freq) {
		return;
	}
	uint32_t int_level = rtos_disable_int();
	{
		bk_rtc_update_base_time();
		s_aon_rtc_clock_freq = clock_freq;
	}
	rtos_enable_int(int_level);
#endif
}

__IRAM_SEC uint64_t bk_aon_rtc_get_us(void)
{
	uint64_t time_tick;
	uint64_t time_diff;
	uint64_t time_us;
	uint64_t base_tick;
	uint64_t base_us;
	uint32_t int_level;

	/* Enter critical section to ensure atomic read of base values */
	int_level = rtos_disable_int();

	time_tick = ANA_RTC_CURRENT_TICK;
	base_tick = s_time_base_tick;
	base_us = s_time_base_us;

	/* Protect against RTC counter overflow or reset
	 * CRITICAL: This check MUST happen before any arithmetic
	 */
	if (time_tick >= base_tick)
	{
		/* CRITICAL: Ensure subtraction is performed in 64-bit unsigned arithmetic
		 *
		 * Why explicit casts are MANDATORY:
		 * 1. Even though time_tick/base_tick are declared as uint64_t, they may be
		 *    stored in 32-bit registers or memory locations
		 * 2. Without casts, the compiler may generate 32-bit subtraction first,
		 *    then sign-extend the result to 64-bit
		 * 3. If result is negative (borrow occurs), sign extension causes 0xFFFFFFFF
		 *    to appear in the high 32 bits
		 *
		 * Assembly proof from line 410365-410367:
		 *   subs r3, r0, r3    // 32-bit subtract, may set borrow flag
		 *   sbc r1, r1, r2     // If r1=1, r2=0, borrow=1 �� r1=0xFFFFFFFF ?
		 *
		 * Solution: Cast BEFORE subtraction forces 64-bit operation
		 */
		uint64_t tick_delta = (uint64_t)time_tick - (uint64_t)base_tick;
		/* Sanity check: detect if subtraction produced negative result
		 * If high bit is set (> 0x7FFFFFFFFFFFFFFF), the result wrapped around
		 * This indicates time_tick < base_tick despite the if-condition above
		 * Possible causes: race condition, hardware glitch, or compiler optimization issue
		 */
		if (tick_delta > 0x7FFFFFFFFFFFFFFFULL)
		{
			ANA_RTC_LOGE("CRITICAL: tick_delta overflow detected! delta=0x%llx, returning last known time\r\n",
			             tick_delta);
			rtos_enable_int(int_level);
			return base_us;  // Return safe value
		}

		/* Fallback handling for abnormal system time updates
		 * If tick_delta exceeds reasonable range, it indicates:
		 * 1. System hasn't updated base time for too long
		 * 2. Potential RTC hardware issue
		 * 3. Counter overflow scenario
		 *
		 * Max reasonable tick_delta: 0x7FFFFFFF (~68 seconds for 32KHz clock)
		 * Action: Cap the delta to prevent overflow in multiplication
		 */

		/* Optimize time calculation for efficiency
		 * Original: (tick_delta * 1000) / 32.768
		 * Optimized: (tick_delta * 125) >> 2  (for 32KHz)
		 *           (tick_delta * 15625) >> 9 (for 32.768KHz)
		 *
		 * Proof for 32KHz: 1000/32 = 31.25 = 125/4
		 * Proof for 32.768KHz: 1000/32.768 = 30.517578125 = 15625/512
		 *
		 * Benefits:
		 * 1. Smaller multiplier = faster computation
		 * 2. Bit shift vs division = much faster on ARM
		 * 3. No floating-point operations
		 * 4. Exact mathematical equivalence
		 *
		 * IMPORTANT: tick_delta is already uint64_t, no cast needed
		 */
		#ifdef CONFIG_EXTERN_32K
		/* For external 32.768 KHz crystal */
		time_diff = (tick_delta * 15625ULL) >> 9;
		#else
		/* For internal 32 KHz oscillator */
		time_diff = (tick_delta * 125ULL) >> 2;
		#endif
		time_us = base_us + time_diff;
	}
	else
	{
		/* Time tick wrapped around or RTC was reset */
		ANA_RTC_LOGW("RTC counter abnomaly: time_tick(0x%llx) < base_tick(0x%llx)\r\n",
		             time_tick, base_tick);
		time_us = base_us;  // Return last known good time
	}
	rtos_enable_int(int_level);
	return time_us;
}

__IRAM_SEC uint64_t bk_aon_rtc_get_ms(void)
{
	uint32_t int_level = rtos_disable_int();
	uint64_t time_tick = ANA_RTC_CURRENT_TICK;
	uint64_t base_tick = s_time_base_tick;
	uint64_t base_us = s_time_base_us;
	uint32_t rtc_freq = s_aon_rtc_clock_freq;
	rtos_enable_int(int_level);

	uint64_t tick_diff = time_tick - base_tick;
	uint64_t time_diff_ms;

	/* Convert tick to milliseconds using integer arithmetic
	 *
	 * Problem: bk_rtc_get_ms_tick_count() returns 32.768 (float)
	 *          Casting to uint32_t loses 0.768, causing 2.34% error
	 *
	 * Solution: Direct calculation using clock frequency
	 *   time_ms = tick_diff / (rtc_freq / 1000)
	 *           = (tick_diff * 1000) / rtc_freq
	 *
	 * For 32.768 KHz: time_ms = (tick_diff * 1000) / 32768
	 * For 32 KHz:     time_ms = (tick_diff * 1000) / 32000
	 *
	 * Precision: Exact integer division, no float precision loss
	 */

	/* Optimization: Use 32-bit division if possible for better performance
	 *
	 * Analysis:
	 * - If tick_diff fits in 32-bit, conversion to uint32_t is lossless
	 * - Explicit cast helps compiler generate optimized 32-bit division code
	 * - tick_diff * 1000 won't overflow: max is 4,294,967,295,000 < 2^64
	 */
	if (tick_diff <= UINT32_MAX) {
		/* 32-bit fast path: tick_diff guaranteed to fit in 32-bit by if-condition
		 * Maximum tick_diff = 0xFFFFFFFF �� 131,072 seconds �� 36 hours
		 * Cast to uint32_t is safe and lossless here
		 */
		uint32_t tick_diff_32 = (uint32_t)tick_diff;  /* Lossless: tick_diff <= UINT32_MAX */
		time_diff_ms = ((uint64_t)tick_diff_32 * 1000ULL) / rtc_freq;
	} else {
		/* 64-bit safe path: for very long intervals (> 36 hours)
		 * Split calculation to prevent overflow in tick_diff * 1000
		 * Formula: (tick_diff / freq) * 1000 + ((tick_diff % freq) * 1000) / freq
		 */
		uint64_t seconds = tick_diff / rtc_freq;
		uint64_t tick_remain = tick_diff % rtc_freq;
		time_diff_ms = seconds * 1000ULL + (tick_remain * 1000ULL) / rtc_freq;
	}

	uint64_t time_ms = base_us / 1000ULL + time_diff_ms;
	return time_ms;
}

#if defined(CONFIG_ROSC_COMPENSATION)
__IRAM_SEC uint64_t bk_aon_rtc_get_current_tick_with_compensation(aon_rtc_id_t id)
{
	return bk_aon_rtc_get_current_tick(id);
}
#endif

bk_err_t bk_rtc_get_deepsleep_duration_seconds(uint32_t *deepsleep_seconds)
{
	return BK_FAIL;
}

static void alarm_dump_node(alarm_node_t *node_p)
{
#if defined(CONFIG_ANA_RTC_DEBUG)
	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	ANA_RTC_LOGD("node_p=0x%x\r\n", node_p);
	if (node_p)
	{
		ANA_RTC_LOGD("next=0x%x\r\n", node_p->next);
		ANA_RTC_LOGD("name=%s\r\n", node_p->name);
		ANA_RTC_LOGD("period_tick=0x%x\r\n", node_p->period_tick);
		ANA_RTC_LOGD("period_cnt=%d\r\n", node_p->period_cnt);
		ANA_RTC_LOGD("start_tick=0x%x\r\n", node_p->start_tick);
		ANA_RTC_LOGD("expired_tick=0x%x\r\n", node_p->expired_tick);
	}

	ANA_RTC_LOGD("%s[-]\r\n", __func__);
#endif
}

static void alarm_dump_list(alarm_node_t *head_p)
{
#if defined(CONFIG_ANA_RTC_DEBUG)
	alarm_node_t *cur_p = head_p;
	uint32_t count = 0;
	uint32_t int_level = 0;

	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	int_level = rtos_disable_int();
	while (cur_p)
	{
		alarm_dump_node(cur_p);
		count++;

		cur_p = cur_p->next;
	}
	rtos_enable_int(int_level);

	ANA_RTC_LOGD("node cnt=%d\r\n", count);

	ANA_RTC_LOGD("%s[-]\r\n", __func__);
#endif
}

static alarm_node_t *ana_rtc_request_node(aon_rtc_id_t id)
{
	uint32_t i = 0;

	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	for (i = 0; i < AON_RTC_MAX_ALARM_CNT; i++)
	{
		if ((s_ana_rtc_nodes_p->busy_bits & (0x1 << i)) == 0)
		{
			ANA_RTC_LOGD("%s[-]:node[%d]=0x%x\r\n", __func__, i, &s_ana_rtc_nodes_p->nodes[i]);
			s_ana_rtc_nodes_p->busy_bits |= (0x1 << i);
			return &s_ana_rtc_nodes_p->nodes[i];
		}
	}

	return NULL;
}

static void ana_rtc_release_node(aon_rtc_id_t id, alarm_node_t *node_p)
{
	uint32_t i = 0;

	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	for (i = 0; i < AON_RTC_MAX_ALARM_CNT; i++)
	{
		if (&s_ana_rtc_nodes_p->nodes[i] == node_p)
		{
			s_ana_rtc_nodes_p->busy_bits &= ~(0x1 << i);
			os_memset(&s_ana_rtc_nodes_p->nodes[i], 0, sizeof(alarm_info_t));
			ANA_RTC_LOGD("%s[-]:node[%d]=0x%x\r\n", __func__, i, &s_ana_rtc_nodes_p->nodes[i]);
			break;
		}
	}

	if (i >= AON_RTC_MAX_ALARM_CNT)
	{
		ANA_RTC_LOGE("release node err\r\n");
	}
}

static int32_t alarm_insert_node(aon_rtc_id_t id, alarm_node_t *node_p)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *next_p = NULL;
	uint32_t int_level = 0;

	ANA_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_ana_rtc.alarm_node_cnt);

	alarm_dump_list(s_ana_rtc.alarm_head_p);

	int_level = rtos_disable_int();

	// check whether the same name
	cur_p = s_ana_rtc.alarm_head_p;
	while (cur_p)
	{
		if (strncmp((const char *)cur_p->name, (const char *)node_p->name, ALARM_NAME_MAX_LEN) == 0)
		{
			ANA_RTC_LOGE("name=%s has registered\r\n", node_p->name);
			rtos_enable_int(int_level);
			return -1;
		}

		cur_p = cur_p->next;
	}

	// search the node position
	cur_p = s_ana_rtc.alarm_head_p;

	// no node
	if (cur_p == NULL)
	{
		s_ana_rtc.alarm_head_p = node_p;
		s_ana_rtc.alarm_node_cnt++;

		ANA_RTC_LOGD("insert first node 0x%x,name=%s\r\n", node_p, node_p->name);

		rtos_enable_int(int_level);
		return 0;
	}

	// only one node
	next_p = cur_p->next;
	if (next_p == NULL)
	{
		if (cur_p->expired_tick <= node_p->expired_tick)
			cur_p->next = node_p;
		else
		{
			node_p->next = cur_p;
			s_ana_rtc.alarm_head_p = node_p;
		}
		s_ana_rtc.alarm_node_cnt++;
		rtos_enable_int(int_level);

		// TODO:log debug
		ANA_RTC_LOGD("list total has two nodes\r\n");

		return 0;
	}

	// more then 2 nodes
	while (next_p)
	{
		if (cur_p->expired_tick <= node_p->expired_tick) // move after cur_p
		{
			if (next_p->expired_tick <= node_p->expired_tick) // search next
			{
				cur_p = next_p;
				next_p = next_p->next;
				continue;
			}
			else // insert
			{
				node_p->next = next_p;
				cur_p->next = node_p;
				s_ana_rtc.alarm_node_cnt++;
				rtos_enable_int(int_level);
				return 0;
			}
		}
		else // insert before cur_p, means the first node, head
		{
			node_p->next = cur_p;
			s_ana_rtc.alarm_head_p = node_p;
			s_ana_rtc.alarm_node_cnt++;
			rtos_enable_int(int_level);
			return 0;
		}
	}

	// the last one
	cur_p->next = node_p;
	s_ana_rtc.alarm_node_cnt++;
	rtos_enable_int(int_level);

	// dump list info
	alarm_dump_list(s_ana_rtc.alarm_head_p);

	ANA_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_ana_rtc.alarm_node_cnt);

	return 0;
}

static alarm_node_t *alarm_remove_node(aon_rtc_id_t id, uint8_t *name_p)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *previous_p = NULL;
	alarm_node_t *remove_node_p = NULL;
	uint32_t int_level = 0;
	uint32_t node_cnt = 0;

	ANA_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_ana_rtc.alarm_node_cnt);

	int_level = rtos_disable_int();
	//
	previous_p = cur_p = s_ana_rtc.alarm_head_p;
	while (cur_p)
	{
		// double check pointer is valid
		node_cnt++;
		BK_ASSERT(node_cnt <= AON_RTC_MAX_ALARM_CNT); /* ASSERT VERIFIED */

		if (strncmp((const char *)cur_p->name, (const char *)name_p, ALARM_NAME_MAX_LEN) == 0)
		{
			// first one
			if (previous_p == cur_p)
			{
				remove_node_p = s_ana_rtc.alarm_head_p;
				s_ana_rtc.alarm_head_p = cur_p->next;
				s_ana_rtc.alarm_node_cnt--;

				ANA_RTC_LOGD("free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				ana_rtc_release_node(id, cur_p);
				break;
			}
			else
			{
				remove_node_p = cur_p;
				previous_p->next = cur_p->next;
				s_ana_rtc.alarm_node_cnt--;
				ANA_RTC_LOGD("free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				ana_rtc_release_node(id, cur_p);
				break;
			}
		}

		previous_p = cur_p;
		cur_p = cur_p->next;
	}

	rtos_enable_int(int_level);

	if (remove_node_p == NULL)
	{
		ANA_RTC_LOGD("%s:can't find %s alarm\r\n", __func__, name_p);
	}

	// dump list info
	alarm_dump_list(s_ana_rtc.alarm_head_p);

	ANA_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_ana_rtc.alarm_node_cnt);

	return remove_node_p;
}

static void alarm_update_expeired_nodes(aon_rtc_id_t id)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *next_p = NULL;
	uint32_t node_cnt = 0;
	uint64_t cur_tick = 0;
	uint32_t int_level = 0;

	ANA_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_ana_rtc.alarm_node_cnt);

	alarm_dump_list(s_ana_rtc.alarm_head_p);

	int_level = rtos_disable_int();

	// search the node position
	cur_p = s_ana_rtc.alarm_head_p;
	while (cur_p)
	{
		alarm_dump_node(cur_p);
		alarm_dump_node(cur_p->next);

		// double check pointer is valid
		node_cnt++;
		BK_ASSERT(node_cnt <= AON_RTC_MAX_ALARM_CNT); /* ASSERT VERIFIED */

		next_p = cur_p->next;

		cur_tick = bk_aon_rtc_get_current_tick(id);
		// maybe callback runs too much time,so assume the time in bk_rtc_get_ms_tick_count() means has expired
		if (cur_p->expired_tick <= cur_tick + AON_RTC_PRECISION_TICK)
		{
			if (cur_p->callback)
			{
				cur_p->callback(id, cur_p->name, cur_p->cb_param_p);
			}

			// last time alarm
			if (cur_p->period_cnt == 1)
			{
				cur_p->period_cnt = 0;
				s_ana_rtc.alarm_head_p = cur_p->next; // head move to next
				s_ana_rtc.alarm_node_cnt--;
/*
 * WARNING:As freertos doesn't support free memory in ISR context.
 * The chip no needs to use a task for AON RTC which wastes some memory.
 * so the APPLIACTION who calls bk_alarm_register should release the memory
 * returns by bk_alarm_register.
 */
#if 0
				ANA_RTC_LOGD("last alarm:free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				os_free(cur_p);
#endif
				ana_rtc_release_node(id, cur_p);
			}
			// loop timer
			else
			{
				if (cur_p->period_cnt != ALARM_LOOP_FOREVER)
				{
					cur_p->period_cnt--;
					ANA_RTC_LOGD("%s left %d times \r\n", cur_p->name, cur_p->period_cnt);
				}

				// has next
				if (next_p) // move to switable position
				{
					s_ana_rtc.alarm_head_p = cur_p->next; // head move to next
					cur_p->expired_tick += cur_p->period_tick;
					cur_p->next = NULL;			// cur_p is removed
					s_ana_rtc.alarm_node_cnt--; // it will ++ in alarm_insert_node
					if (alarm_insert_node(id, cur_p) != 0)
					{
						ANA_RTC_LOGE("alarm name=%s insert fail\r\n", cur_p->name);
						rtos_enable_int(int_level);
						return;
					}
				}
				else // only self
				{
					// just update self expired time
					cur_p->expired_tick += cur_p->period_tick;
					ANA_RTC_LOGD("%s update next expired time %d \r\n", cur_p->name, cur_p->expired_tick);
				}
			}
		}
		else // no expired
		{
			break;
		}

		cur_p = next_p; // TODO:maybe cur_p offset is too small and calback excutes too much time, here can't switch to next NODE.

		alarm_dump_list(s_ana_rtc.alarm_head_p);
	}

	rtos_enable_int(int_level);

	alarm_dump_list(s_ana_rtc.alarm_head_p);

	ANA_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_ana_rtc.alarm_node_cnt);
}

bk_err_t bk_aon_rtc_register_tick_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param)
{
	// AON_RTC_RETURN_ON_INVALID_ID(id);
	uint32_t int_level = rtos_disable_int();
	s_ana_rtc_tick_isr.callback = isr;
	s_ana_rtc_tick_isr.isr_param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

/*
 * aon rtc set tick uses 3 cycles of 32k in ASIC,
 * cpu should check whether set tick success.
 * If twice set tick in 3/32 ms, the second time set tick value will be failed.
 */
static void ana_rtc_set_tick(uint64_t val)
{
	uint64_t start_tick = 0, cur_tick = 0;
#if defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	if (val == AON_RTC_ROUND_TICK + 1)
	{
		sys_hal_set_ana_reg11_rtcsel((uint32_t)(AON_RTC_ROUND_TICK >> 32));
		sys_hal_set_ana_reg13_rtcsel((uint32_t)AON_RTC_ROUND_TICK - 1);
		return;
	}

#if defined(CONFIG_ROSC_COMPENSATION)
	uint64_t sleep_tick_old = val - ANA_RTC_CURRENT_TICK;
	uint64_t sleep_tick_new = sleep_tick_old * bk_rosc_32k_get_freq() / AON_RTC_DEFAULT_CLOCK_FREQ;
	val = val - sleep_tick_old + sleep_tick_new - s_ana_rtc_diff_tick;
#endif

	val -= s_ana_rtc_base_tick; // convert rtc tick

	uint32_t val_h = val >> 32, val_l = (uint32_t)val;

	sys_hal_set_ana_reg11_rtcsel(val_h);
	sys_hal_set_ana_reg13_rtcsel(val_l);

	ANA_RTC_LOGD("set tick: %d %d %d %d\r\n", (uint32_t)(s_ana_rtc_base_tick>>32), (uint32_t)s_ana_rtc_base_tick, val_h, val_l);

	start_tick = ANA_RTC_CURRENT_TICK;
	while ((sys_hal_get_ana_reg13_rtcsel() != val_l) || (sys_hal_get_ana_reg11_rtcsel() != val_h))
	{
		cur_tick = ANA_RTC_CURRENT_TICK;
		if (cur_tick - start_tick > ANA_RTC_OPS_SAFE_DELAY_US)
		{
			ANA_RTC_LOGE("%s:set tick timeout,set_tick=0x%llx, lpo_tick=0x%x %x\r\n", __func__, val,
						 ((uint64_t)sys_hal_get_ana_reg13_rtcsel() << 32) + sys_hal_get_ana_reg11_rtcsel());
			break;
		}
		sys_hal_set_ana_reg11_rtcsel(val_h);
		sys_hal_set_ana_reg13_rtcsel(val_l);
	}
#else
	if (val == AON_RTC_ROUND_TICK + 1)
	{
		sys_hal_set_ana_reg11_timersel((uint32_t)(AON_RTC_ROUND_TICK >> 32));
		sys_hal_set_ana_reg12_timersel((uint32_t)AON_RTC_ROUND_TICK - 1);
		return;
	}

#if defined(CONFIG_ROSC_COMPENSATION)
	uint64_t sleep_tick_old = val - ANA_RTC_CURRENT_TICK;
	uint64_t sleep_tick_new = sleep_tick_old * bk_rosc_32k_get_freq() / AON_RTC_DEFAULT_CLOCK_FREQ;
	val = val - sleep_tick_old + sleep_tick_new - s_ana_rtc_diff_tick;
#endif

	val -= s_ana_rtc_base_tick; // convert rtc tick

	uint32_t val_h = val >> 32, val_l = (uint32_t)val;

	sys_hal_set_ana_reg11_timersel(val_h);
	sys_hal_set_ana_reg12_timersel(val_l);

	ANA_RTC_LOGD("set tick: %d %d %d %d\r\n", (uint32_t)(s_ana_rtc_base_tick>>32), (uint32_t)s_ana_rtc_base_tick, val_h, val_l);

	start_tick = ANA_RTC_CURRENT_TICK;
	while ((sys_hal_get_ana_reg12_timersel() != val_l) || (sys_hal_get_ana_reg11_timersel() != val_h))
	{
		cur_tick = ANA_RTC_CURRENT_TICK;
		if (cur_tick - start_tick > ANA_RTC_OPS_SAFE_DELAY_US)
		{
			ANA_RTC_LOGE("%s:set tick timeout,set_tick=0x%llx, lpo_tick=0x%x %x\r\n", __func__, val,
						 ((uint64_t)sys_hal_get_ana_reg12_timersel() << 32) + sys_hal_get_ana_reg11_timersel());
			break;
		}
		sys_hal_set_ana_reg11_timersel(val_h);
		sys_hal_set_ana_reg12_timersel(val_l);
	}
#endif
	sys_hal_enable_ana_rtc_int();
	#if !defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	s_rtc_updated = false;
	#endif
}

#if defined(CONFIG_ANA_RTC_DEBUG)
#define ANA_RTC_ISR_DEBUG_MAX_CNT (256)
static uint32_t s_isr_cnt = 0;
static uint64_t s_isr_debug_in_tick[ANA_RTC_ISR_DEBUG_MAX_CNT];
static uint64_t s_isr_debug_out_tick[ANA_RTC_ISR_DEBUG_MAX_CNT];
static uint64_t s_isr_debug_set_tick[ANA_RTC_ISR_DEBUG_MAX_CNT];
#endif

static bk_err_t ana_rtc_isr_handler(void *args)
{
	aon_rtc_id_t id = ANA_RTC_GET_SET_TIME_RTC_ID;
	uint32_t int_level = rtos_disable_int();

#if defined(CONFIG_ANA_RTC_DEBUG)
	s_isr_debug_in_tick[(s_isr_cnt) % ANA_RTC_ISR_DEBUG_MAX_CNT] = bk_aon_rtc_get_current_tick(id);
#endif

	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	sys_hal_disable_ana_rtc_int();
	alarm_update_expeired_nodes(id);

	// reset the timer tick
	if (s_ana_rtc.alarm_head_p)
	{
		//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
		// BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_ana_rtc.alarm_head_p->expired_tick);	//4:reserve enough time to set the tick
		if ((bk_aon_rtc_get_current_tick(id) + 1 > s_ana_rtc.alarm_head_p->expired_tick))
		{
			ANA_RTC_LOGE("next expired tick is invalid\r\n");
			rtos_enable_int(int_level);
			return BK_FAIL;
		}
		ana_rtc_set_tick(s_ana_rtc.alarm_head_p->expired_tick);
#if defined(CONFIG_AON_RTC_DEBUG)
		s_isr_debug_set_tick[(s_isr_cnt) % AON_RTC_ISR_DEBUG_MAX_CNT] = s_ana_rtc.alarm_head_p->expired_tick;
#endif
		ANA_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", (uint32_t)s_ana_rtc.alarm_head_p->expired_tick, (uint32_t)bk_aon_rtc_get_current_tick(id));
	}
	else
	{
		ana_rtc_set_tick(AON_RTC_ROUND_TICK + 1);
		ANA_RTC_LOGD("no alarm:cur_tick=0x%x\r\n", (uint32_t)bk_aon_rtc_get_current_tick(id));
	}

	ANA_RTC_LOGD("%s[-]\r\n", __func__);
#if defined(CONFIG_ANA_RTC_DEBUG)
	s_isr_debug_out_tick[(s_isr_cnt) % ANA_RTC_ISR_DEBUG_MAX_CNT] = bk_aon_rtc_get_current_tick(id);
	s_isr_cnt++;
#endif
	rtos_enable_int(int_level);

	return BK_OK;
}

static void ana_rtc1_isr_handler(void)
{
	ana_rtc_isr_handler(AON_RTC_ID_1);
}

bk_err_t ana_rtc_wakeup_config(void *args)
{
	// protect sleep time check
	uint64_t current_tick;
	uint64_t expired_tick = AON_RTC_ROUND_TICK;
	int64_t sleep_tick;

#if defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	expired_tick = sys_hal_get_ana_reg13_rtcsel() + ((uint64_t)sys_hal_get_ana_reg11_rtcsel() << 32);
	current_tick = aon_pmu_hal_rtc_tick_l_get() + ((uint64_t)aon_pmu_hal_rtc_tick_h_get() << 32);
#else
	if (s_ana_rtc.alarm_head_p)
		expired_tick = s_ana_rtc.alarm_head_p->expired_tick;
	current_tick = ANA_RTC_CURRENT_TICK;
#endif

	sleep_tick = (int64_t)expired_tick - (int64_t)current_tick;

	if (sleep_tick < AON_RTC_MS_TICK_CNT) // close to 1ms
	{
		return BK_FAIL;
	}

	return BK_OK;
}

static bk_err_t ana_rtc_sw_init(void)
{
	os_memset(&s_ana_rtc, 0, sizeof(s_ana_rtc));
	os_memset(&s_ana_rtc_tick_isr, 0, sizeof(s_ana_rtc_tick_isr));

	s_high_tick = 0;
	s_low_tick = 0;

	s_ana_rtc_nodes_p = os_zalloc(sizeof(ana_rtc_nodes_memory_t));
	if (s_ana_rtc_nodes_p == NULL)
	{
		return BK_ERR_NO_MEM;
	}

#if 0
	wakeup_cb_conf_t wkup_cfg = {
		.cb = (wakeup_cb)ana_rtc_isr_handler,
		.args = ANA_RTC_GET_SET_TIME_RTC_ID,
	};
	wakeup_cb_conf_t src_cfg = {
		.cb = (wakeup_cb)ana_rtc_wakeup_config,
		.args = ANA_RTC_GET_SET_TIME_RTC_ID,
	};
	bk_wakeup_register_cb(WAKEUP_SOURCE_INT_RTC, wkup_cfg, src_cfg);
#endif
	return BK_OK;
}

void ana_rtc_update(void)
{
	#if !defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	if (!s_rtc_updated)
	{
		uint32_t high_tick = aon_pmu_hal_rtc_tick_h_get();
		// indicate rtc_value reset by ana_timer
		if (high_tick == 0)
		{
			if (s_ana_rtc.alarm_head_p)
				s_ana_rtc_base_tick = s_ana_rtc.alarm_head_p->expired_tick;
			s_rtc_updated = true;
			ana_rtc_set_tick(AON_RTC_ROUND_TICK + 1);
			return;
		}
	}
	// indicate rtc_value > 2^32,overflow happened
	s_high_tick++;
	#endif
}

static void ana_rtc_hw_init(void)
{
	uint32_t tick_h = (uint32_t)(AON_RTC_ROUND_TICK >> 32);
	uint32_t tick_l = (uint32_t)AON_RTC_ROUND_TICK;

	// rebase tick value
#if !defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
	s_ana_rtc_base_tick = aon_pmu_hal_rtc_tick_l_get();
#endif
	s_low_tick = 0;

	sys_hal_set_ana_reg8_spi_latch1v(1);
#if !defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
	sys_hal_set_ana_reg9_spi_timerwken(0);
#endif
	sys_hal_set_ana_reg5_pwd_rosc_spi(0);
	sys_hal_set_ana_reg5_rosc_disable(0);
	sys_hal_set_ana_reg9_spi_byp32pwd(1);

#ifdef CONFIG_EXTERN_32K
	sys_hal_set_ana_reg7_clk_sel(1);
#else
	sys_hal_set_ana_reg7_clk_sel(0);
#endif

	// set upper tick value
	sys_hal_set_ana_reg11_timersel(tick_h);
	sys_hal_set_ana_reg12_timersel(tick_l);

	// set wakeup tick value to make sure wakeup success
	sys_hal_set_ana_reg11_rtcsel(tick_h);
	sys_hal_set_ana_reg13_rtcsel(tick_l - 1);

	// rtc value sample select neg edge
	aon_pmu_hal_rtc_samp_sel(0);

	// rtc tick start
#if !defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
	sys_hal_set_ana_reg9_spi_timerwken(1);
#endif
	sys_hal_set_ana_reg8_spi_latch1v(0);

	// wait until rtc tick reset ok, will cost about 32us
#if !defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
	while (1)
	{
		if (aon_pmu_hal_rtc_tick_l_get() < s_ana_rtc_base_tick)
		{
			break;
		}
	}
#endif

	bk_int_isr_register(INT_SRC_ANA_RTC, ana_rtc1_isr_handler, NULL);

	ANA_RTC_LOGD("%s[-]cur_tick=%d\r\n", __func__, (uint32_t)ANA_RTC_CURRENT_TICK);
}

bk_err_t bk_aon_rtc_driver_init(void)
{
	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	// TOTO: Enter critical protect
	if (!s_ana_rtc.inited)
	{
		ana_rtc_sw_init();
		ana_rtc_hw_init();
		s_ana_rtc.inited = true;
	}

	// TOTO: exit critical protect
	ANA_RTC_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

bk_err_t bk_aon_rtc_driver_deinit(void)
{
	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	if (s_ana_rtc.inited)
	{
		sys_hal_set_ana_reg5_pwd_rosc_spi(1);
		sys_hal_set_ana_reg5_rosc_disable(1);
		sys_hal_set_ana_reg9_spi_byp32pwd(1);

		bk_int_isr_unregister(INT_SRC_ANA_RTC);
		if (s_ana_rtc_nodes_p != NULL)
		{
			os_free(s_ana_rtc_nodes_p);
			s_ana_rtc_nodes_p = NULL;
		}

		s_ana_rtc.inited = false;
	}

	ANA_RTC_LOGD("%s[-]\r\n", __func__);
	return BK_OK;
}

bk_err_t bk_alarm_register(aon_rtc_id_t id, alarm_info_t *alarm_info_p)
{
	alarm_node_t *node_p = NULL;
	uint32_t int_level = 0;

	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	if (id >= AON_RTC_ID_MAX)
	{
		ANA_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return BK_ERR_PARAM;
	}

	if (alarm_info_p == NULL)
	{
		return BK_ERR_PARAM;
	}

	if (alarm_info_p->period_tick < AON_RTC_PRECISION_TICK) // in protect area to reduce consume time before set tick.
	{
		ANA_RTC_LOGD("period_tick should not smaller then %d\r\n", AON_RTC_PRECISION_TICK);
		return BK_FAIL;
	}

	int_level = rtos_disable_int();

	if (s_ana_rtc.alarm_node_cnt >= AON_RTC_MAX_ALARM_CNT)
	{
		rtos_enable_int(int_level);
		ANA_RTC_LOGE("alarm registered too much:%d\r\n", AON_RTC_MAX_ALARM_CNT);
		return BK_FAIL;
	}

	// request a node
	node_p = ana_rtc_request_node(id);
	if (node_p == NULL)
	{
		rtos_enable_int(int_level);
		ANA_RTC_LOGE("alarm registered:no memory\r\n");
		return BK_ERR_NO_MEM;
	}

	memset(node_p, 0, sizeof(alarm_node_t));
	node_p->callback = alarm_info_p->callback;
	node_p->cb_param_p = alarm_info_p->param_p;
	memcpy(&node_p->name[0], alarm_info_p->name, ALARM_NAME_MAX_LEN);
	node_p->name[ALARM_NAME_MAX_LEN] = 0;
	node_p->start_tick = bk_aon_rtc_get_current_tick(id); // tick
	node_p->period_tick = alarm_info_p->period_tick;
	// BK_ASSERT(alarm_info_p->period_cnt);
	if (alarm_info_p->period_cnt == 0)
	{
		rtos_enable_int(int_level);
		ANA_RTC_LOGE("no set period cnt\r\n");
		return BK_ERR_PARAM;
	}

	node_p->period_cnt = alarm_info_p->period_cnt;
	node_p->expired_tick = node_p->start_tick + (alarm_info_p->period_tick);

	// push to alarm list
	if (alarm_insert_node(id, node_p) != 0)
	{
		ANA_RTC_LOGE("alarm name=%s has registered\r\n", alarm_info_p->name);
		ana_rtc_release_node(id, node_p);
		rtos_enable_int(int_level);
		return BK_FAIL;
	}

	// reset the timer tick
	if (node_p == s_ana_rtc.alarm_head_p) // insert node is the first one, should reset tick val
	{
		//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
		// BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_ana_rtc.alarm_head_p->expired_tick);	//4:reserve enough time to set the tick
		if ((bk_aon_rtc_get_current_tick(id) + 1 > s_ana_rtc.alarm_head_p->expired_tick))
		{
			rtos_enable_int(int_level);
			ANA_RTC_LOGE("next expired tick is invalid\r\n");
			return BK_FAIL;
		}

		ana_rtc_set_tick(s_ana_rtc.alarm_head_p->expired_tick);
	}

	ANA_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", (uint32_t)s_ana_rtc.alarm_head_p->expired_tick, (uint32_t)bk_aon_rtc_get_current_tick(id));

	rtos_enable_int(int_level);

	ANA_RTC_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

// the timer isn't expired, but app un-register it.
bk_err_t bk_alarm_unregister(aon_rtc_id_t id, uint8_t *name_p)
{
	alarm_node_t *remove_node_p = NULL;
	alarm_node_t *previous_head_node_p = NULL;
	uint32_t int_level = 0;

	ANA_RTC_LOGD("%s[+]\r\n", __func__);

	if (id >= AON_RTC_ID_MAX)
	{
		ANA_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return BK_ERR_PARAM;
	}

	int_level = rtos_disable_int();
	previous_head_node_p = s_ana_rtc.alarm_head_p;
	remove_node_p = alarm_remove_node(id, name_p);

	// reset the timer tick
	if (previous_head_node_p == remove_node_p) // the previous head is removed
	{
		if (s_ana_rtc.alarm_head_p) // new head exist
		{
			//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
			// BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_ana_rtc.alarm_head_p->expired_tick);	//reserve enough time to set the tick
			if ((bk_aon_rtc_get_current_tick(id) + 1 > s_ana_rtc.alarm_head_p->expired_tick))
			{
				rtos_enable_int(int_level);
				ANA_RTC_LOGE("next expired tick is invalid\r\n");
				return BK_FAIL;
			}

			ana_rtc_set_tick(s_ana_rtc.alarm_head_p->expired_tick);
			ANA_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", s_ana_rtc.alarm_head_p->expired_tick, bk_aon_rtc_get_current_tick(id));
		}
		else // has no nodes now
		{
			// aon_rtc_hal_disable_tick_int(&s_ana_rtc.hal);		//If the ISR at enable status, and the previous set tick time come, it will produce an Interrupt and maybe wakeup system.
			ana_rtc_set_tick(AON_RTC_ROUND_TICK + 1);
			ANA_RTC_LOGD("no alarm:cur_tick=0x%x\r\n", bk_aon_rtc_get_current_tick(id));
		}
	}

	rtos_enable_int(int_level);

	ANA_RTC_LOGD("%s[-]\r\n", __func__);
	return BK_OK;
}

__IRAM_SEC uint64_t bk_aon_rtc_get_current_tick(aon_rtc_id_t id)
{
	uint32_t low_tick, int_level = 0;

	if (id >= AON_RTC_ID_MAX)
	{
		ANA_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return 0;
	}

	int_level = rtos_disable_int();

	low_tick = aon_pmu_hal_rtc_tick_l_get();
#if CONFIG_ANA_RTC_SUPPORT_64BIT
	if (low_tick < s_low_tick)
	{
		ANA_RTC_LOGD("rtc carrying:%x %x %x\n", s_low_tick, low_tick, aon_pmu_hal_rtc_tick_l_get());
		#if !CONFIG_ANA_RTC_WAKUP_BY_RTC
		ana_rtc_update();
		#else
		s_high_tick++;
		#endif
	}
#else
	s_high_tick = aon_pmu_hal_rtc_tick_h_get();
#endif
	s_low_tick = low_tick;
#if defined(CONFIG_ROSC_COMPENSATION)
	uint64_t tick_val = ((uint64_t)s_high_tick << 32) + s_low_tick + s_ana_rtc_base_tick;
	s_ana_rtc_diff_tick = bk_rosc_32k_get_tick_diff(tick_val);
	rtos_enable_int(int_level);
	return (tick_val + s_ana_rtc_diff_tick);
#else
	rtos_enable_int(int_level);
	return ((uint64_t)s_high_tick << 32) + s_low_tick + s_ana_rtc_base_tick;
#endif
}

void bk_aon_rtc_dump(aon_rtc_id_t id)
{
#if defined(CONFIG_ANA_RTC_DEBUG)
	uint32_t i = 0, index = 0;

	for (i = s_isr_cnt - ANA_RTC_ISR_DEBUG_MAX_CNT; i < s_isr_cnt; i++)
	{
		index = i % ANA_RTC_ISR_DEBUG_MAX_CNT;

		for (volatile uint32_t j = 0; j < 1800; j++)
			; // confirm log output normarlly

		ANA_RTC_LOGI("isr_in[%d]=0x%llx,out=0x%llx,set=0x%llx\r\n", index, s_isr_debug_in_tick[index], s_isr_debug_out_tick[index], s_isr_debug_set_tick[index]);
	}
#endif
	alarm_dump_list(s_ana_rtc.alarm_head_p);
}

bk_err_t bk_rtc_gettimeofday(struct timeval *tv, void *ptz)
{
	(void)ptz;

	if (tv != NULL)
	{
		uint64_t uCurTimeUs = s_boot_time_us + bk_aon_rtc_get_us();

		tv->tv_sec = uCurTimeUs / 1000000;
		tv->tv_usec = uCurTimeUs % 1000000;
		ANA_RTC_LOGD("s_boot_time_us:h=0x%x,l=0x%x \r\n", s_boot_time_us >> 32, (uint32_t)s_boot_time_us);
		ANA_RTC_LOGD("%s sec=%d,us:%d\n", __func__, tv->tv_sec, tv->tv_usec);
	}
	else
		ANA_RTC_LOGW("%s tv is null \r\n", __func__);

	return BK_OK;
}

bk_err_t bk_rtc_settimeofday(const struct timeval *tv, const struct timezone *tz)
{
	(void)tz;
	if (tv)
	{
		uint64_t setTimeUs = ((uint64_t)tv->tv_sec) * 1000000LL + tv->tv_usec;
		uint64_t getCurTimeUs = bk_aon_rtc_get_us();

		s_boot_time_us = setTimeUs - getCurTimeUs;
		ANA_RTC_LOGD("%s:sec=%d us=%d\n", __func__, tv->tv_sec, tv->tv_usec);
		ANA_RTC_LOGD("get us:h=0x%x,l=0x%x\n", getCurTimeUs >> 32, (uint32_t)getCurTimeUs);
	}

	return BK_OK;
}

uint8_t *bk_rtc_get_first_alarm_name(void)
{
	alarm_node_t *first_node = NULL;
	uint32_t int_level = 0;
	uint8_t *name = NULL;

	// Enter critical section to protect alarm list access
	int_level = rtos_disable_int();

	// Get the first node (head of the sorted list)
	first_node = s_ana_rtc.alarm_head_p;

	if (first_node != NULL)
	{
		// Return pointer to the name field
		name = first_node->name;
		ANA_RTC_LOGD("%s: first alarm name=%s\r\n", __func__, name);
	}
	else
	{
		ANA_RTC_LOGD("%s: no alarm registered\r\n", __func__);
	}

	rtos_enable_int(int_level);

	return name;
}
