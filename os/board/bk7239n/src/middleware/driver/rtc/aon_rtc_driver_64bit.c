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
#include "sys/time.h"

#include <driver/aon_rtc.h>
#include "aon_rtc_hal_64bit.h"
#include "aon_rtc_driver_64bit.h"

#if defined(CONFIG_ROSC_COMPENSATION)
#include <driver/rosc_32k.h>
#endif

/* 
 * NOTES: System entery deepsleep or reboot, the aon rtc time should reserved.
 * 1.When enter deep sleep, the DTCM power is lost,so have to save time to flash.
 * 2.Write time to flash takes about 200us~3ms, When reboot system,the easy flash API call rtos_get_semaphore in ISR cause assert
 * 3.When reboot system,DTCM doesn't loss power,so can save time in DTCM.
 */
#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
#include <modules/pm.h>
#include "reset_reason.h"
#if defined(CONFIG_EASY_FLASH)
#include "bk_ef.h"
#endif
#endif
#if defined(CONFIG_AON_PMU)
#include "aon_pmu_driver.h"
#endif
#ifdef CONFIG_FREERTOS_SMP
#include "spinlock.h"
static volatile spinlock_t rtc_spin_lock = SPIN_LOCK_INIT;
#endif // CONFIG_FREERTOS_SMP

#define AON_RTC_UNIT_NUM (AON_RTC_HAL_UNIT_NUM)
#define AON_RTC_NAME ((uint8_t *)"aon_rtc")

typedef struct {
	//aon_rtc_id_t id;	//no needs it until now:the id is matched from APP,DRIVER,HAL,HW/SOC layer.
	bool inited;
	//uint8_t using_cnt;	//remove it as only one HW can't for many APPs

	aon_rtc_hal_t hal;

	//Record APP param's
	//bool period;
	//uint32_t tick;

	//Alarm list
	alarm_node_t *alarm_head_p;
	uint32_t alarm_node_cnt;
} aon_rtc_driver_t;

typedef struct {
	aon_rtc_isr_t callback;
	void *isr_param;
} aon_rtc_callback_t;

typedef struct {
	alarm_node_t nodes[AON_RTC_MAX_ALARM_CNT];
	uint64_t busy_bits;
} aon_rtc_nodes_memory_t;

static aon_rtc_driver_t s_aon_rtc[AON_RTC_UNIT_NUM] = {0};
static aon_rtc_callback_t s_aon_rtc_tick_isr[AON_RTC_UNIT_NUM] = {NULL};
static aon_rtc_callback_t s_aon_rtc_upper_isr[AON_RTC_UNIT_NUM] = {NULL};
static aon_rtc_nodes_memory_t *s_aon_rtc_nodes_p[AON_RTC_UNIT_NUM];
static void aon_rtc_interrupt_disable(aon_rtc_id_t id);

#define AONRTC_GET_SET_TIME_RTC_ID AON_RTC_ID_1
static int64_t s_boot_time_us = 0;	//timeofday value

#if defined(CONFIG_RTC_ANA_WAKEUP_SUPPORT)
#define RTC_ANA_TIME_PERIOD_MAX 16
static uint32_t s_wkup_time_period = 0;
#endif

#define AON_RTC_OPS_SAFE_DELAY_US (125)
/*
 * AON RTC uses 32k clock,and 3 cycles later can be clock sync.
 * CPU clock is more faster then AON RTC, so software operates AON RTC register
 * will be effect after 125 us(RTC 3+1 clock cycles).
 * So if operate the same register in 125 us, the second operation will be failed.
 */
static void aon_rtc_delay_to_grantee_ops_safe()
{
	extern void delay_us(UINT32 us);

	delay_us(AON_RTC_OPS_SAFE_DELAY_US);
}

#ifdef CONFIG_EXTERN_32K
static uint32_t s_aon_rtc_clock_freq = AON_RTC_EXTERN_32K_CLOCK_FREQ;
#else
static uint32_t s_aon_rtc_clock_freq = AON_RTC_DEFAULT_CLOCK_FREQ;
#endif
static uint64_t s_time_base_us = 0;
static uint64_t s_time_base_tick = 0;

static inline uint32_t rtc_enter_critical()
{
       uint32_t flags = rtos_disable_int();

#ifdef CONFIG_FREERTOS_SMP
       spin_lock(&rtc_spin_lock);
#endif // CONFIG_FREERTOS_SMP

       return flags;
}

static inline void rtc_exit_critical(uint32_t flags)
{
#ifdef CONFIG_FREERTOS_SMP
       spin_unlock(&rtc_spin_lock);
#endif // CONFIG_FREERTOS_SMP

       rtos_enable_int(flags);
}

__IRAM_SEC float bk_rtc_get_ms_tick_count(void) {
	return (float)s_aon_rtc_clock_freq/1000;
}

uint32_t bk_rtc_get_clock_freq(void) {
	return s_aon_rtc_clock_freq;
}

static uint64_t rtc_tick_to_us(uint64_t rtc_tick)
{
	if(s_aon_rtc_clock_freq == AON_RTC_EXTERN_32K_CLOCK_FREQ)
	{
		return ((rtc_tick * 125LL * 125LL) >> 9); // rtc_tick * 1000 * 1000 / 32768;
	}
	else if(s_aon_rtc_clock_freq == AON_RTC_DEFAULT_CLOCK_FREQ)
	{
		return ((rtc_tick * 125LL) >> 2);   // rtc_tick * 1000 * 1000 / 32000;
	}
	else if(s_aon_rtc_clock_freq != 0)
	{
		return (rtc_tick * 1000LL * 1000LL) / s_aon_rtc_clock_freq;
	}

	return -1;
}

static uint64_t rtc_tick_to_ms(uint64_t rtc_tick)
{
	if(s_aon_rtc_clock_freq == AON_RTC_EXTERN_32K_CLOCK_FREQ)
	{
		return ((rtc_tick * 125) >> 12); // rtc_tick * 1000 / 32768;
	}
	else if(s_aon_rtc_clock_freq == AON_RTC_DEFAULT_CLOCK_FREQ)
	{
		return (rtc_tick >> 5);   // rtc_tick * 1000 / 32000;
	}
	else if(s_aon_rtc_clock_freq != 0)
	{
		return (rtc_tick * 1000LL) / s_aon_rtc_clock_freq;
	}

	return -1;
}

static uint64_t rtc_tick_to_s(uint64_t rtc_tick)
{
	if(s_aon_rtc_clock_freq == AON_RTC_EXTERN_32K_CLOCK_FREQ)
	{
		return ((rtc_tick) >> 15); // rtc_tick / 32768;
	}
	else if(s_aon_rtc_clock_freq == AON_RTC_DEFAULT_CLOCK_FREQ)
	{
		return (rtc_tick >> 8) / 125LL;   // rtc_tick / 32000;
	}
	else if(s_aon_rtc_clock_freq != 0)
	{
		return rtc_tick / s_aon_rtc_clock_freq;
	}

	return -1;
}

static inline uint64_t get_diff_time_us(void) {
	uint64_t time_tick = bk_aon_rtc_get_current_tick(AONRTC_GET_SET_TIME_RTC_ID);
	uint64_t time_diff = rtc_tick_to_us(time_tick - s_time_base_tick); // *1000LL/bk_rtc_get_ms_tick_count();
	return time_diff;
}

void bk_rtc_update_base_time(void) {
	uint64_t time_tick = bk_aon_rtc_get_current_tick(AONRTC_GET_SET_TIME_RTC_ID);
	uint64_t time_diff = rtc_tick_to_us(time_tick - s_time_base_tick);  // *1000LL/bk_rtc_get_ms_tick_count();

	s_time_base_tick = time_tick;
	s_time_base_us += time_diff;
	// AON_RTC_LOGI("s_time_base_tick: 0x%x:0x%08x\r\n", (u32)(s_time_base_tick >> 32), (u32)(s_time_base_tick & 0xFFFFFFFF));
	// AON_RTC_LOGI("s_time_base_us: 0x%x:0x%08x\r\n", (u32)(s_time_base_us >> 32), (u32)(s_time_base_us & 0xFFFFFFFF));
}

void bk_rtc_set_clock_freq(uint32_t clock_freq){
#if defined(CONFIG_AON_RTC_DYNAMIC_CLOCK_SUPPORT)
	AON_RTC_LOGI("Set clock freq: %d\n", clock_freq);
	if (clock_freq == s_aon_rtc_clock_freq) {
		return;
	}
	uint32_t int_level = rtc_enter_critical();
	{
		bk_rtc_update_base_time();
		s_aon_rtc_clock_freq = clock_freq;
	}
	rtc_exit_critical(int_level);
#endif
}

 __IRAM_SEC uint64_t bk_aon_rtc_get_us(void) {
	uint64_t time_tick = bk_aon_rtc_get_current_tick(AONRTC_GET_SET_TIME_RTC_ID);
	uint64_t time_diff = rtc_tick_to_us(time_tick - s_time_base_tick);  // *1000LL/bk_rtc_get_ms_tick_count();
    uint64_t time_us = s_time_base_us + time_diff;
    return  time_us;
}

 __IRAM_SEC uint64_t bk_aon_rtc_get_ms(void) {
	uint64_t time_tick = bk_aon_rtc_get_current_tick(AONRTC_GET_SET_TIME_RTC_ID);
	uint64_t time_diff = rtc_tick_to_ms(time_tick - s_time_base_tick);
    uint64_t time_ms = s_time_base_us + time_diff;
    return  time_ms;
}

#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
static int64_t s_rtc_keep_tick_offset = 0x0;
#define AONRTC_DEEPSLEEP_KEEPTIME_EF_KEYNUM 2

static bool bk_rtc_get_aon_pmu_deepsleep_flag()
{
#if defined(CONFIG_AON_PMU)
	return (aon_pmu_drv_reg_get(PMU_REG2) & BIT(BIT_SLEEP_FLAG_DEEP_SLEEP));
#endif
	return false;
}

/*
 * WARNING:
 * This function is only called by AON RTC driver when set timeofday!!!
 * WORKAROUND:
 * save the time value to flash.
 * after system wakeup,restore the baked time.
 */
static bk_err_t aon_rtc_bake_timeofday()
{
#if defined(CONFIG_EASY_FLASH)
	struct timeval rtc_keep_time = {0, 0};
	rtc_keep_time.tv_sec = s_boot_time_us/1000000;
	rtc_keep_time.tv_usec = s_boot_time_us%1000000;
	//bake up current time to flash
	char *rtc_keep_time_key[2] = {"rtc_tv_sec", "rtc_tv_usec"};
	uint32_t ret = 0;

	for(int key_index = 0; key_index < AONRTC_DEEPSLEEP_KEEPTIME_EF_KEYNUM; key_index++)
	{
		if(key_index == 0)
			ret = bk_set_env_enhance(rtc_keep_time_key[key_index], (const void *)&rtc_keep_time.tv_sec, sizeof(rtc_keep_time.tv_sec));
		else if(key_index == 1)
			ret = bk_set_env_enhance(rtc_keep_time_key[key_index], (const void *)&rtc_keep_time.tv_usec, sizeof(rtc_keep_time.tv_usec));

		AON_RTC_LOGD("%s, sec=%d, usec=%d, ret=%d.\n",__func__, (uint32_t)rtc_keep_time.tv_sec, (uint32_t)rtc_keep_time.tv_usec, ret);
	}
#else
	AON_RTC_LOGI("TODO:Please save the time to none lost memory\r\n", __func__);
#endif
	return BK_OK;
}

static void aon_rtc_get_timeofday(struct timeval *time_p)
{
	AON_RTC_LOGD("%s[+]\r\n", __func__);

#if defined(CONFIG_EASY_FLASH)
	char *rtc_keep_time_key[2] = {"rtc_tv_sec", "rtc_tv_usec"};

	for(int key_index = 0; key_index < AONRTC_DEEPSLEEP_KEEPTIME_EF_KEYNUM; key_index++){
		if(key_index == 0)
			bk_get_env_enhance(rtc_keep_time_key[key_index], &(time_p->tv_sec), sizeof(time_p->tv_sec));
		else if(key_index == 1)
			bk_get_env_enhance(rtc_keep_time_key[key_index], &(time_p->tv_usec), sizeof(time_p->tv_usec));
	}
	AON_RTC_LOGD("%s tv_sec:%d tv_usec:%d\n",__func__, (uint32_t)time_p->tv_sec, (uint32_t)time_p->tv_usec);
#else
	AON_RTC_LOGI("TODO:%s\n", __func__);
#endif

}

#if 0
bk_err_t aon_rtc_enter_reboot(void)
{
	struct timeval rtc_keep_time = {0, 0};

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	//backup poweroff time
	bk_rtc_gettimeofday(&rtc_keep_time, 0);
	AON_RTC_LOGD("bake:tv_sec:%d,tv_usec:%d\n", (uint32_t)rtc_keep_time.tv_sec, rtc_keep_time.tv_usec);

	s_boot_time_us = ((uint64_t)rtc_keep_time.tv_sec)*1000000LL+rtc_keep_time.tv_usec;
	aon_rtc_bake_timeofday();

	aon_rtc_hal_reset_counter(&s_aon_rtc[AONRTC_GET_SET_TIME_RTC_ID].hal);
	aon_rtc_hal_clear_reset_counter(&s_aon_rtc[AONRTC_GET_SET_TIME_RTC_ID].hal);

	return BK_OK;
}
#endif

static void aon_rtc_compute_boot_timeofday(void)
{

	AON_RTC_LOGD("%s[+]\r\n", __func__);
	
#if (defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT))
	struct timeval time = {0, 0};
	aon_rtc_get_timeofday(&time);
	s_boot_time_us = ((uint64_t)time.tv_sec)*1000000LL+time.tv_usec;
#endif
	//deepsleep boot
	if(bk_rtc_get_aon_pmu_deepsleep_flag())
	{
		AON_RTC_LOGD("deepsleep s_boot_time_us:%lld\r\n",s_boot_time_us);
	}
	else if(bk_misc_get_reset_reason() != RESET_SOURCE_POWERON)	//hot reboot
	{
		AON_RTC_LOGD("hot reboot s_boot_time_us:%lld\r\n",s_boot_time_us);
	}
	else	//cold boot,power on
	{
		s_boot_time_us = 0;
		AON_RTC_LOGD("power on s_boot_time_us:%lld\r\n",s_boot_time_us);
	}
}

static void bk_rtc_keep_time_enter_deepsleep(void)
{
	AON_RTC_LOGD("[+]%s\r\n", __func__);
}

static void bk_rtc_keep_time_exit_deepsleep(void)
{
	AON_RTC_LOGD("%s\r\n", __func__);
}

static void aon_rtc_register_deepsleep_cb(void)
{
    pm_cb_conf_t aon_rtc_enter_config = {
            .cb = (pm_cb)bk_rtc_keep_time_enter_deepsleep,
            .args = NULL
            };
    pm_cb_conf_t aon_rtc_exit_config = {
            .cb = (pm_cb)bk_rtc_keep_time_exit_deepsleep,
            .args = NULL
            };
    bk_pm_sleep_register_cb(PM_MODE_DEEP_SLEEP, PM_DEV_ID_RTC, &aon_rtc_enter_config, &aon_rtc_exit_config);
}

#endif

bk_err_t bk_rtc_get_deepsleep_duration_seconds(uint32_t *deepsleep_seconds)
{
#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
    *deepsleep_seconds = rtc_tick_to_s(s_rtc_keep_tick_offset);  //    /(bk_rtc_get_ms_tick_count() * 1000);
    return BK_OK;
#endif
    return BK_FAIL;
}

static void alarm_dump_node(alarm_node_t *node_p)
{
#if defined(CONFIG_AON_RTC_DEBUG)
	AON_RTC_LOGD("%s[+]\r\n", __func__);

	AON_RTC_LOGD("node_p=0x%x\r\n", node_p);
	if(node_p)
	{		
		AON_RTC_LOGD("next=0x%x\r\n", node_p->next);
		AON_RTC_LOGD("name=%s\r\n", node_p->name);
		AON_RTC_LOGD("period_tick=0x%x\r\n", node_p->period_tick);
		AON_RTC_LOGD("period_cnt=%d\r\n", node_p->period_cnt);
		AON_RTC_LOGD("start_tick=0x%x\r\n", node_p->start_tick);
		AON_RTC_LOGD("expired_tick=0x%x\r\n", node_p->expired_tick);
	}

	AON_RTC_LOGD("%s[-]\r\n", __func__);
#endif
}

static void alarm_dump_list(alarm_node_t *head_p)
{
#if defined(CONFIG_AON_RTC_DEBUG)
	alarm_node_t *cur_p = head_p;
	uint32_t count = 0;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	int_level = rtc_enter_critical();
	while(cur_p)
	{
		alarm_dump_node(cur_p);
		count++;

		cur_p = cur_p->next;
	}
	rtc_exit_critical(int_level);

	AON_RTC_LOGD("node cnt=%d\r\n", count);

	AON_RTC_LOGD("%s[-]\r\n", __func__);
#endif
}

static alarm_node_t* aon_rtc_request_node(aon_rtc_id_t id)
{
	uint32_t i = 0; 

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	for(i = 0; i < AON_RTC_MAX_ALARM_CNT; i++)
	{
		if((s_aon_rtc_nodes_p[id]->busy_bits & (0x1<<i)) == 0)
		{
			AON_RTC_LOGD("%s[-]:node[%d]=0x%x\r\n", __func__, i, &s_aon_rtc_nodes_p[id]->nodes[i]);
			s_aon_rtc_nodes_p[id]->busy_bits |= (0x1<<i);
			return &s_aon_rtc_nodes_p[id]->nodes[i];
		}
	}

	return NULL;
}

static void aon_rtc_release_node(aon_rtc_id_t id, alarm_node_t *node_p)
{
	uint32_t i = 0; 

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	for(i = 0; i < AON_RTC_MAX_ALARM_CNT; i++)
	{
		if(&s_aon_rtc_nodes_p[id]->nodes[i] == node_p)
		{
			s_aon_rtc_nodes_p[id]->busy_bits &= ~(0x1<<i);
			os_memset(&s_aon_rtc_nodes_p[id]->nodes[i], 0, sizeof(alarm_info_t));
			AON_RTC_LOGD("%s[-]:node[%d]=0x%x\r\n", __func__, i, &s_aon_rtc_nodes_p[id]->nodes[i]);
			break;
		}
	}

	if(i >= AON_RTC_MAX_ALARM_CNT)
	{
		AON_RTC_LOGE("release node err\r\n");
	}
}

static int32_t alarm_insert_node(aon_rtc_id_t id, alarm_node_t *node_p)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *next_p = NULL;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	int_level = rtc_enter_critical();

	//check whether the same name
	cur_p = s_aon_rtc[id].alarm_head_p;
	while(cur_p)
	{
		if(strncmp((const char *)cur_p->name, (const char *)node_p->name, ALARM_NAME_MAX_LEN) == 0)
		{
			AON_RTC_LOGE("name=%s has registered\r\n", node_p->name);
			rtc_exit_critical(int_level);
			return -1;
		}

		cur_p = cur_p->next;
	}

	//search the node position
	cur_p = s_aon_rtc[id].alarm_head_p;

	//no node
	if(cur_p == NULL)
	{
		s_aon_rtc[id].alarm_head_p = node_p;
		s_aon_rtc[id].alarm_node_cnt++;
		AON_RTC_LOGD("insert first node 0x%x,name=%s\r\n", node_p, node_p->name);
		
		rtc_exit_critical(int_level);
		return 0;
	}

	//only one node
	next_p = cur_p->next;
	if(next_p == NULL)
	{
		if(cur_p->expired_tick <= node_p->expired_tick)
			cur_p->next = node_p;
		else
		{
			node_p->next = cur_p;
			s_aon_rtc[id].alarm_head_p = node_p;
		}
		s_aon_rtc[id].alarm_node_cnt++;
		rtc_exit_critical(int_level);

		//TODO:log debug
		AON_RTC_LOGD("list total has two nodes\r\n");

		return 0;
	}

	//more then 2 nodes
	while(next_p)
	{
		if(cur_p->expired_tick <= node_p->expired_tick)	//move after cur_p
		{
			if(next_p->expired_tick <= node_p->expired_tick)	//search next
			{
				cur_p = next_p;
				next_p = next_p->next;
				continue;
			}
			else	//insert
			{
				node_p->next = next_p;
				cur_p->next = node_p;
				s_aon_rtc[id].alarm_node_cnt++;
				rtc_exit_critical(int_level);
				return 0;
			}
		}
		else	//insert before cur_p, means the first node, head
		{
			node_p->next = cur_p;
			s_aon_rtc[id].alarm_head_p = node_p;
			s_aon_rtc[id].alarm_node_cnt++;
			rtc_exit_critical(int_level);
			return 0;
		}
	}

	//the last one
	cur_p->next = node_p;
	s_aon_rtc[id].alarm_node_cnt++;
	rtc_exit_critical(int_level);

	//dump list info
	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	AON_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	return 0;
}

static alarm_node_t *alarm_remove_node(aon_rtc_id_t id, uint8_t *name_p)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *previous_p = NULL;
	alarm_node_t *remove_node_p = NULL;
	uint32_t int_level = 0;
	uint32_t node_cnt = 0;

	AON_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	int_level = rtc_enter_critical();
	//
	previous_p = cur_p = s_aon_rtc[id].alarm_head_p;
	while(cur_p)
	{
		//double check pointer is valid
		node_cnt++;
		BK_ASSERT(node_cnt <= AON_RTC_MAX_ALARM_CNT); /* ASSERT VERIFIED */

		if(strncmp((const char *)cur_p->name, (const char *)name_p, ALARM_NAME_MAX_LEN) == 0)
		{
			//first one
			if(previous_p == cur_p)
			{
				remove_node_p = s_aon_rtc[id].alarm_head_p;
				s_aon_rtc[id].alarm_head_p = cur_p->next;
				s_aon_rtc[id].alarm_node_cnt--;

				AON_RTC_LOGD("free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				aon_rtc_release_node(id, cur_p);
				break;
			}
			else
			{
				remove_node_p = cur_p;
				previous_p->next = cur_p->next;
				s_aon_rtc[id].alarm_node_cnt--;
				AON_RTC_LOGD("free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				aon_rtc_release_node(id, cur_p);
				break;
			}
		}

		previous_p = cur_p;
		cur_p = cur_p->next;
	}

	rtc_exit_critical(int_level);

	if(remove_node_p == NULL)
	{
		AON_RTC_LOGD("%s:can't find %s alarm\r\n", __func__, name_p);
	}

	//dump list info
	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	AON_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	return remove_node_p;
}

static void alarm_update_expeired_nodes(aon_rtc_id_t id)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *next_p = NULL;
	uint32_t node_cnt = 0;
	uint64_t cur_tick = 0;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);
	
	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	int_level = rtc_enter_critical();

	//search the node position
	cur_p = s_aon_rtc[id].alarm_head_p;
	while(cur_p)
	{
		alarm_dump_node(cur_p);
		alarm_dump_node(cur_p->next);

		//double check pointer is valid
		node_cnt++;
		BK_ASSERT(node_cnt <= AON_RTC_MAX_ALARM_CNT); /* ASSERT VERIFIED */

		next_p = cur_p->next;

		cur_tick = bk_aon_rtc_get_current_tick(id);
		//maybe callback runs too much time,so assume the time in bk_rtc_get_ms_tick_count() means has expired
		if(cur_p->expired_tick <= cur_tick + AON_RTC_PRECISION_TICK)
		{
			if(cur_p->callback)
			{
				cur_p->callback(id, cur_p->name, cur_p->cb_param_p);
			}

			//last time alarm
			if(cur_p->period_cnt == 1)
			{
				cur_p->period_cnt = 0;
				s_aon_rtc[id].alarm_head_p = cur_p->next;	//head move to next
				s_aon_rtc[id].alarm_node_cnt--;
/* 
 * WARNING:As freertos doesn't support free memory in ISR context.
 * The chip no needs to use a task for AON RTC which wastes some memory.
 * so the APPLIACTION who calls bk_alarm_register should release the memory
 * returns by bk_alarm_register.
 */
#if 0
				AON_RTC_LOGD("last alarm:free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				os_free(cur_p);
#endif
				aon_rtc_release_node(id, cur_p);
			}
			//loop timer
			else 
			{
				if(cur_p->period_cnt != ALARM_LOOP_FOREVER)
				{
					cur_p->period_cnt--;
					AON_RTC_LOGD("%s left %d times \r\n", cur_p->name, cur_p->period_cnt);
				}

				//has next
				if(next_p)	//move to switable position
				{
					s_aon_rtc[id].alarm_head_p = cur_p->next;	//head move to next
					cur_p->expired_tick += cur_p->period_tick;
					cur_p->next = NULL;		//cur_p is removed
					s_aon_rtc[id].alarm_node_cnt--; //it will ++ in alarm_insert_node
					if(alarm_insert_node(id, cur_p) != 0)
					{
						AON_RTC_LOGE("alarm name=%s insert fail\r\n", cur_p->name);
						rtc_exit_critical(int_level);
						return;
					}
				}
				else	//only self
				{
					//just update self expired time
					cur_p->expired_tick += cur_p->period_tick;
					AON_RTC_LOGD("%s update next expired time %d \r\n", cur_p->name, cur_p->expired_tick);
				}
			}
		}
		else	//no expired
		{
			break;
		}

		cur_p = next_p;	//TODO:maybe cur_p offset is too small and calback excutes too much time, here can't switch to next NODE.

		alarm_dump_list(s_aon_rtc[id].alarm_head_p);
	}

	rtc_exit_critical(int_level);

	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	AON_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);
}

bk_err_t bk_aon_rtc_register_tick_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param)
{
	//AON_RTC_RETURN_ON_INVALID_ID(id);
	uint32_t int_level = rtc_enter_critical();
	s_aon_rtc_tick_isr[id].callback = isr;
	s_aon_rtc_tick_isr[id].isr_param = param;
	rtc_exit_critical(int_level);
	return BK_OK;
}

/*
 * aon rtc set tick uses 3 cycles of 32k in ASIC,
 * cpu should check whether set tick success.
 * If twice set tick in 3/32 ms, the second time set tick value will be failed.
 */
static void aon_rtc_set_tick(aon_rtc_hal_t *hal, uint64_t val)
{
	uint64_t start_tick = 0, cur_tick = 0;

	aon_rtc_hal_set_tick_val(hal, val);
	start_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	while(aon_rtc_hal_get_tick_val_lpo(hal) != val)
	{
		cur_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		if(cur_tick - start_tick > AON_RTC_OPS_SAFE_DELAY_US)
		{
			AON_RTC_LOGE("%s:set tick timeout,set_tick=0x%llx, lpo_tick=0x%llx\r\n", __func__, val, aon_rtc_hal_get_tick_val_lpo(hal));
			break;
		}
		aon_rtc_hal_set_tick_val(hal, val);
	}
}

bk_err_t bk_aon_rtc_register_upper_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param)
{
	//AON_RTC_RETURN_ON_INVALID_ID(id);
	uint32_t int_level = rtc_enter_critical();
	s_aon_rtc_upper_isr[id].callback = isr;
	s_aon_rtc_upper_isr[id].isr_param = param;
	rtc_exit_critical(int_level);
	return BK_OK;
}

#if 0
static bk_err_t aon_rtc_isr_handler(aon_rtc_id_t id)
{
	//uses tick as one time timer
	if(aon_rtc_hal_get_tick_int_status(&s_aon_rtc[id].hal))
	{
		if (s_aon_rtc_tick_isr[id].callback) {
			s_aon_rtc_tick_isr[id].callback(id, AON_RTC_NAME, s_aon_rtc_tick_isr[id].isr_param);
		}
		aon_rtc_hal_clear_tick_int_status(&s_aon_rtc[id].hal);

		bk_aon_rtc_destroy(id);
	}

	//uses upper timer as period timer
	if(aon_rtc_hal_get_upper_int_status(&s_aon_rtc[id].hal))
	{
		if (s_aon_rtc_upper_isr[id].callback) {
			s_aon_rtc_upper_isr[id].callback(id, AON_RTC_NAME, s_aon_rtc_upper_isr[id].isr_param);
		}

		aon_rtc_hal_clear_upper_int_status(&s_aon_rtc[id].hal);
	}

	//TODO: clear NVIC/INTC/PLIC int pending status

	return BK_OK;
}
#else

#if defined(CONFIG_AON_RTC_DEBUG)
#define AON_RTC_ISR_DEBUG_MAX_CNT (256)
static uint32_t s_isr_cnt = 0;
static uint64_t s_isr_debug_in_tick[AON_RTC_ISR_DEBUG_MAX_CNT];
static uint64_t s_isr_debug_out_tick[AON_RTC_ISR_DEBUG_MAX_CNT];
static uint64_t s_isr_debug_set_tick[AON_RTC_ISR_DEBUG_MAX_CNT];
#endif

static bk_err_t aon_rtc_isr_handler(aon_rtc_id_t id)
{
	uint32_t int_level = rtc_enter_critical();

#if defined(CONFIG_AON_RTC_DEBUG)
	s_isr_debug_in_tick[(s_isr_cnt)%AON_RTC_ISR_DEBUG_MAX_CNT] = bk_aon_rtc_get_current_tick(id);
#endif

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	//uses tick as one time timer
	if(aon_rtc_hal_get_tick_int_status(&s_aon_rtc[id].hal))
	{
		//maybe the isr callback runs too much time and set next tick value too small, caused next isr can't response.
		aon_rtc_hal_clear_tick_int_status(&s_aon_rtc[id].hal);

		alarm_update_expeired_nodes(id);

		//reset the timer tick
		if(s_aon_rtc[id].alarm_head_p)
		{
			//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
			//BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_aon_rtc[id].alarm_head_p->expired_tick);	//4:reserve enough time to set the tick
			if((bk_aon_rtc_get_current_tick(id) + 1 > s_aon_rtc[id].alarm_head_p->expired_tick))
			{
				AON_RTC_LOGE("next expired tick is invalid\r\n");
				rtc_exit_critical(int_level);
				return BK_FAIL;
			}
			aon_rtc_set_tick(&s_aon_rtc[id].hal, s_aon_rtc[id].alarm_head_p->expired_tick);
#if defined(CONFIG_AON_RTC_DEBUG)
			s_isr_debug_set_tick[(s_isr_cnt)%AON_RTC_ISR_DEBUG_MAX_CNT] = s_aon_rtc[id].alarm_head_p->expired_tick;
#endif
			AON_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick, (uint32_t)bk_aon_rtc_get_current_tick(id));
		}
		else
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, AON_RTC_ROUND_TICK);
			AON_RTC_LOGD("no alarm:cur_tick=0x%x\r\n", (uint32_t)bk_aon_rtc_get_current_tick(id));
		}
	}

	//TODO: clear NVIC/INTC/PLIC int pending status

	AON_RTC_LOGD("%s[-]\r\n", __func__);
#if defined(CONFIG_AON_RTC_DEBUG)
	s_isr_debug_out_tick[(s_isr_cnt)%AON_RTC_ISR_DEBUG_MAX_CNT] = bk_aon_rtc_get_current_tick(id);
	s_isr_cnt++;
#endif
	rtc_exit_critical(int_level);

	return BK_OK;
}

#endif

static void aon_rtc1_isr_handler(void)
{
	aon_rtc_isr_handler(AON_RTC_ID_1);
}

#if (SOC_AON_RTC_UNIT_NUM > 1)
static void aon_rtc2_isr_handler(void)
{
	aon_rtc_isr_handler(AON_RTC_ID_2);
}
#endif

#if (defined(CONFIG_SYSTEM_CTRL))
static void aon_rtc_interrupt_enable(aon_rtc_id_t id)
{
	switch(id)
	{
		case AON_RTC_ID_1:
			sys_drv_int_group2_enable(RTC_INTERRUPT_CTRL_BIT);
			break;
#if (SOC_AON_RTC_UNIT_NUM > 1)
		case AON_RTC_ID_2:
			sys_drv_int_group2_enable(RTC1_INTERRUPT_CTRL_BIT);
			break;
#endif
		default:
			break;
	}
}

static void aon_rtc_interrupt_disable(aon_rtc_id_t id)
{
	switch(id)
	{
		case AON_RTC_ID_1:
			sys_drv_int_group2_disable(RTC_INTERRUPT_CTRL_BIT);
			break;
#if (SOC_AON_RTC_UNIT_NUM > 1)
		case AON_RTC_ID_2:
			sys_drv_int_group2_disable(RTC1_INTERRUPT_CTRL_BIT);
			break;
#endif
		default:
			break;
	}
}
#endif

static bk_err_t aon_rtc_sw_init(aon_rtc_id_t id)
{
	os_memset(&s_aon_rtc[id], 0, sizeof(s_aon_rtc[id]));
	os_memset(&s_aon_rtc_tick_isr[id], 0, sizeof(s_aon_rtc_tick_isr[id]));
	os_memset(&s_aon_rtc_upper_isr[id], 0, sizeof(s_aon_rtc_upper_isr[id]));

	s_aon_rtc_nodes_p[id] = os_zalloc(sizeof(aon_rtc_nodes_memory_t));
	if(s_aon_rtc_nodes_p[id] == NULL)
	{
		return BK_ERR_NO_MEM;
	}

#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
	aon_rtc_register_deepsleep_cb();
#endif

	return BK_OK;
}

static void aon_rtc_hw_init(aon_rtc_id_t id)
{
	aon_rtc_int_config_t int_config_table[] = AON_RTC_INT_CONFIG_TABLE;
	aon_rtc_int_config_t *cur_int_cfg = &int_config_table[id];

	AON_RTC_LOGD("%s[+]cur_tick=%d\r\n", __func__, (uint32_t)bk_aon_rtc_get_current_tick(id));

	if(!aon_rtc_hal_is_enable(&s_aon_rtc[id].hal))
	{
		aon_rtc_hal_init(&s_aon_rtc[id].hal);
	}
	//set upper to max value
	aon_rtc_hal_set_upper_val(&s_aon_rtc[id].hal, AON_RTC_ROUND_TICK);
	aon_rtc_hal_enable_upper_int(&s_aon_rtc[id].hal);

	bk_int_isr_register(cur_int_cfg->int_src, cur_int_cfg->isr, NULL);
#if (defined(CONFIG_SYSTEM_CTRL))
	aon_rtc_interrupt_enable(id);
#endif
	aon_rtc_hal_start_counter(&s_aon_rtc[id].hal);

	AON_RTC_LOGD("%s[-]cur_tick=%d\r\n", __func__, (uint32_t)bk_aon_rtc_get_current_tick(id));
}

bk_err_t bk_aon_rtc_driver_init(void)
{
	AON_RTC_LOGD("%s[+]\r\n", __func__);

	//TOTO: Enter critical protect
	for (int id = AON_RTC_ID_1; id < AON_RTC_ID_MAX; id++) {
		if(!s_aon_rtc[id].inited)
		{
#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
			if(id == AON_RTC_ID_1)
			{
				//get baked time which saved entry sleep/reboot
				aon_rtc_compute_boot_timeofday();
			}
#endif

			aon_rtc_sw_init(id);
			aon_rtc_hw_init(id);
			s_aon_rtc[id].inited = true;
		}
	}

	//TOTO: exit critical protect
	AON_RTC_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

bk_err_t bk_aon_rtc_driver_deinit(void)
{
	aon_rtc_int_config_t int_cfg_table[] = AON_RTC_INT_CONFIG_TABLE;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	for (int id = AON_RTC_ID_1; id < AON_RTC_ID_MAX; id++) {
		if(s_aon_rtc[id].inited)
		{
			aon_rtc_hal_deinit(&s_aon_rtc[id].hal);
#if (defined(CONFIG_SYSTEM_CTRL))
			aon_rtc_interrupt_disable(id);
#endif
			bk_int_isr_unregister(int_cfg_table[id].int_src);

			if(s_aon_rtc_nodes_p[id] != NULL)
			{
				os_free(s_aon_rtc_nodes_p[id]);
				s_aon_rtc_nodes_p[id] = NULL;
			}

			s_aon_rtc[id].inited = false;
		}
	}

#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
	bk_pm_sleep_unregister_cb(PM_MODE_DEEP_SLEEP, PM_DEV_ID_RTC, true, true);
#endif

	AON_RTC_LOGD("%s[-]\r\n", __func__);
	return BK_OK;
}

#if 0	//remove it, only one HW can't be used for many APPs.
bk_err_t bk_aon_rtc_create(aon_rtc_id_t id, rtc_tick_t tick, bool period)
{
	//Avoid APP call this function before driver has done bk_aon_rtc_driver_init
	if(s_aon_rtc[id].inited == false)
	{
		//TODO: logs: call aon_rtc_init first.
		return BK_ERR_NOT_INIT;
	}

	if(s_aon_rtc[id].using_cnt)
	{
		//TODO: logs: call bk_aon_rtc_destroy first.
		return BK_ERR_BUSY;
	}

	//TOTO: Enter critical protect

	s_aon_rtc[id].using_cnt++;
	s_aon_rtc[id].tick = tick;
	s_aon_rtc[id].period = period;

	//init HW
	s_aon_rtc[id].hal.id = id;
	aon_rtc_hal_init(&s_aon_rtc[id].hal);

	if(period)	//use upper value int
	{
		aon_rtc_hal_set_upper_val(&s_aon_rtc[id].hal, tick);
		aon_rtc_hal_enable_upper_int(&s_aon_rtc[id].hal);
	}
	else
	{
		//confirm tick val <= upper value, or tick int never be entry.
		aon_rtc_hal_set_upper_val(&s_aon_rtc[id].hal, AON_RTC_UPPER_VAL_MAX);

		aon_rtc_set_tick(&s_aon_rtc[id].hal, tick);
		aon_rtc_hal_enable_tick_int(&s_aon_rtc[id].hal);
	}

	//start to run
	aon_rtc_start_run(id);

	//TOTO: Exit critical protect

	return BK_OK;
}

bk_err_t bk_aon_rtc_destroy(aon_rtc_id_t id)
{
	//TOTO: Enter critical protect

	if(s_aon_rtc[id].inited == false)
	{
		//TODO: logs: call aon_rtc_init first.
		//TOTO: Exit critical protect
		return BK_ERR_NOT_INIT;
	}

	if(s_aon_rtc[id].using_cnt == 0)
	{
		//TODO: logs: call bk_aon_rtc_create first.
		//TOTO: Exit critical protect
		return BK_ERR_NOT_INIT;
	}

	//stop HW before SW change value, avoid ISR status was update to INTC/NVIC/PLIC...
	//but doesn't response ISR, after HW deinit, the ISR comes caused error.
	aon_rtc_hal_deinit(&s_aon_rtc[id].hal);

	s_aon_rtc[id].using_cnt = 0;
	s_aon_rtc[id].tick = 0;
	s_aon_rtc[id].period = false;

	//TOTO: Exit critical protect

	return BK_OK;
}
#endif

bk_err_t bk_alarm_register(aon_rtc_id_t id, alarm_info_t *alarm_info_p)
{
	alarm_node_t *node_p = NULL;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	if(id >= AON_RTC_ID_MAX)
	{
		AON_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return BK_ERR_PARAM;
	}

	if(alarm_info_p == NULL)
	{
		return BK_ERR_PARAM;
	}

	if(alarm_info_p->period_tick < AON_RTC_PRECISION_TICK)	//in protect area to reduce consume time before set tick.
	{
		AON_RTC_LOGE("period_tick should not smaller then %d\r\n", AON_RTC_PRECISION_TICK);
		return BK_FAIL;
	}

	int_level = rtc_enter_critical();

	if(s_aon_rtc[id].alarm_node_cnt >= AON_RTC_MAX_ALARM_CNT)
	{
		rtc_exit_critical(int_level);
		AON_RTC_LOGE("alarm registered too much:%d\r\n", AON_RTC_MAX_ALARM_CNT);
		return BK_FAIL;
	}

	//request a node
	node_p = aon_rtc_request_node(id);
	if(node_p == NULL)
	{
		rtc_exit_critical(int_level);
		AON_RTC_LOGE("alarm registered:no memory\r\n");
		return BK_ERR_NO_MEM;
	}

	memset(node_p, 0, sizeof(alarm_node_t));
	node_p->callback = alarm_info_p->callback;
	node_p->cb_param_p = alarm_info_p->param_p;
	memcpy(&node_p->name[0], alarm_info_p->name, ALARM_NAME_MAX_LEN);
	node_p->name[ALARM_NAME_MAX_LEN] = 0;
	node_p->start_tick = bk_aon_rtc_get_current_tick(id);	//tick
	node_p->period_tick = alarm_info_p->period_tick;
	//BK_ASSERT(alarm_info_p->period_cnt);
	if(alarm_info_p->period_cnt == 0)
	{
		rtc_exit_critical(int_level);
		AON_RTC_LOGE("no set period cnt\r\n");
		return BK_ERR_PARAM;
	}

	node_p->period_cnt = alarm_info_p->period_cnt;
	node_p->expired_tick = node_p->start_tick + (alarm_info_p->period_tick);
	
	//push to alarm list
	if(alarm_insert_node(id, node_p) != 0)
	{
		AON_RTC_LOGE("alarm name=%s has registered\r\n", alarm_info_p->name);
		aon_rtc_release_node(id, node_p);
		rtc_exit_critical(int_level);
		return BK_FAIL;
	}

	//reset the timer tick
	if(node_p == s_aon_rtc[id].alarm_head_p)	//insert node is the first one, should reset tick val
	{
		//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
		//BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_aon_rtc[id].alarm_head_p->expired_tick);	//4:reserve enough time to set the tick
		if((bk_aon_rtc_get_current_tick(id) + 1 > s_aon_rtc[id].alarm_head_p->expired_tick))
		{
			rtc_exit_critical(int_level);
			AON_RTC_LOGE("next expired tick is invalid\r\n");
			return BK_FAIL;
		}

		aon_rtc_set_tick(&s_aon_rtc[id].hal, s_aon_rtc[id].alarm_head_p->expired_tick);
	}

	aon_rtc_hal_enable_tick_int(&s_aon_rtc[id].hal);
	AON_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick, (uint32_t)bk_aon_rtc_get_current_tick(id));

	rtc_exit_critical(int_level);

	AON_RTC_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}


//the timer isn't expired, but app un-register it.
bk_err_t bk_alarm_unregister(aon_rtc_id_t id, uint8_t *name_p)
{
	alarm_node_t *remove_node_p = NULL;
	alarm_node_t *previous_head_node_p = NULL;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	if(id >= AON_RTC_ID_MAX)
	{
		AON_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return BK_ERR_PARAM;
	}

	int_level = rtc_enter_critical();
	previous_head_node_p = s_aon_rtc[id].alarm_head_p;
	remove_node_p = alarm_remove_node(id, name_p);

	//reset the timer tick
	if(previous_head_node_p == remove_node_p)	//the previous head is removed
	{
		if(s_aon_rtc[id].alarm_head_p)	//new head exist
		{
			//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
			//BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_aon_rtc[id].alarm_head_p->expired_tick);	//reserve enough time to set the tick
			if((bk_aon_rtc_get_current_tick(id) + 1 > s_aon_rtc[id].alarm_head_p->expired_tick))
			{
				rtc_exit_critical(int_level);
				AON_RTC_LOGE("next expired tick is invalid\r\n");
				return BK_FAIL;
			}

			aon_rtc_set_tick(&s_aon_rtc[id].hal, s_aon_rtc[id].alarm_head_p->expired_tick);
			AON_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", s_aon_rtc[id].alarm_head_p->expired_tick, bk_aon_rtc_get_current_tick(id));
		}
		else	//has no nodes now
		{
			//If the ISR at enable status, and the previous set tick time come, it will produce an Interrupt and maybe wakeup system.
			aon_rtc_hal_disable_tick_int(&s_aon_rtc[id].hal);
			// aon_rtc_set_tick(&s_aon_rtc[id].hal, AON_RTC_ROUND_TICK);
			// AON_RTC_LOGD("no alarm:cur_tick=0x%x\r\n", bk_aon_rtc_get_current_tick(id));
		}
	}

	rtc_exit_critical(int_level);

	AON_RTC_LOGD("%s[-]\r\n", __func__);
	return BK_OK;
}

#if (defined(CONFIG_AON_RTC) && (!defined(CONFIG_AON_RTC_MANY_USERS)))
bk_err_t bk_aon_rtc_tick_init()
{
	aon_rtc_hal_init(&s_aon_rtc[AON_RTC_ID_1].hal);

	//set upper to max value
	aon_rtc_hal_set_upper_val(&s_aon_rtc[AON_RTC_ID_1].hal, AON_RTC_ROUND_TICK);
	aon_rtc_hal_set_tick_val(&s_aon_rtc[AON_RTC_ID_1].hal, 0);
	return BK_OK;
}

bk_err_t bk_aon_rtc_open_rtc_wakeup(rtc_tick_t period)
{
    uint64_t wakeup_period= 0;

    wakeup_period = aon_rtc_hal_get_upper_val(&s_aon_rtc[AON_RTC_ID_1].hal) + period;
    aon_rtc_hal_set_tick_val(&s_aon_rtc[AON_RTC_ID_1].hal, wakeup_period);

    aon_rtc_hal_enable_tick_int(&s_aon_rtc[AON_RTC_ID_1].hal);

    return BK_OK;
}
#endif

__IRAM_SEC uint64_t bk_aon_rtc_get_current_tick(aon_rtc_id_t id)
{
	if(id >= AON_RTC_ID_MAX)
	{
		AON_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return 0;
	}

	return (aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal));
}

#if defined(CONFIG_ROSC_COMPENSATION)
__IRAM_SEC uint64_t bk_aon_rtc_get_current_tick_with_compensation(aon_rtc_id_t id)
{
	uint64_t tick_val = bk_aon_rtc_get_current_tick(id);
	return (tick_val + bk_rosc_32k_get_tick_diff(tick_val));
}
#endif

#if defined(CONFIG_RTC_ANA_WAKEUP_SUPPORT)
static int ana_wakesource_rtc_enter_cb(uint64_t sleep_time, void *args)
{
	uint32_t period = s_wkup_time_period;

	if (period >= RTC_ANA_TIME_PERIOD_MAX) {
		AON_RTC_LOGE("rtc wakeup period range 0~15\r\n");
	}
	sys_drv_rtc_ana_wakeup_enable(period);

	return 0;
}

bk_err_t bk_rtc_ana_register_wakeup_source(rtc_tick_t period)
{
	pm_cb_conf_t enter_conf;

	s_wkup_time_period = period;
	AON_RTC_LOGI("regist wakeup source rtc period: %d\r\n", period);

	enter_conf.cb = ana_wakesource_rtc_enter_cb;
	enter_conf.args = NULL;

	return bk_pm_sleep_register_cb(PM_MODE_SUPER_DEEP_SLEEP, PM_DEV_ID_RTC, &enter_conf, NULL);
}
#endif

#if defined(CONFIG_AON_RTC_DEBUG)
void bk_aon_rtc_timing_test(aon_rtc_id_t id, uint32_t round, uint32_t cycles, rtc_tick_t set_tick)
{
	uint32_t int_level = 0;
	uint32_t i = 0, j = 0;
	uint64_t start_tick = 0, end_tick = 0;
	uint64_t u64_start_tick = 0, u64_end_tick = 0;
	uint32_t max_offset_tick = 0, min_offset_tick = 0xffffffff;
	uint32_t fail_cnt = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	int_level = rtc_enter_critical();
	
	//get uint32_t tick counter check
	for(i = 0; i < round; i++)
	{
		start_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		for(j = 0; j < cycles; j++)
		{
			aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		}
		end_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);

		if(min_offset_tick > end_tick - start_tick)
			min_offset_tick = end_tick - start_tick;
		if(max_offset_tick < end_tick - start_tick)
			max_offset_tick = end_tick - start_tick;
	}
	AON_RTC_LOGI("Gettick uint32:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);

	//get uint64_t tick counter check
	max_offset_tick = 0;
	min_offset_tick = 0xffffffff;
	for(i = 0; i < round; i++)
	{
		u64_start_tick = bk_aon_rtc_get_current_tick(id);
		for(j = 0; j < cycles; j++)
		{
			bk_aon_rtc_get_current_tick(id);
		}
		u64_end_tick = bk_aon_rtc_get_current_tick(id);

		if(min_offset_tick > (uint32_t)(u64_end_tick - u64_start_tick))
			min_offset_tick = (uint32_t)(u64_end_tick - u64_start_tick);
		if(max_offset_tick < u64_end_tick - u64_start_tick)
			max_offset_tick = u64_end_tick - u64_start_tick;
	}
	AON_RTC_LOGI("Gettick uint64:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);

	//set tick val check
	max_offset_tick = 0;
	min_offset_tick = 0xffffffff;
	for(i = 0; i < round; i++)
	{
		start_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		for(j = 0; j < cycles; j++)
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, set_tick);
		}
		end_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);

		if(min_offset_tick > end_tick - start_tick)
			min_offset_tick = end_tick - start_tick;
		if(max_offset_tick < end_tick - start_tick)
			max_offset_tick = end_tick - start_tick;
	}
	AON_RTC_LOGI("Settick:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);

	fail_cnt = 0;
	max_offset_tick = 0;
	min_offset_tick = 0xffffffff;
	for(i = 0; i < round; i++)
	{
		start_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		for(j = 0; j < cycles; j++)
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, set_tick);
			if(set_tick != aon_rtc_hal_get_tick_val(&s_aon_rtc[id].hal))
			{
				fail_cnt++;
			}
		}
		end_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);

		if(min_offset_tick > end_tick - start_tick)
			min_offset_tick = end_tick - start_tick;
		if(max_offset_tick < end_tick - start_tick)
			max_offset_tick = end_tick - start_tick;
	}
	AON_RTC_LOGI("Settick:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);
	AON_RTC_LOGI("Settick:%d rounds*%d times:check fail_cnt=%d\r\n", i, j, fail_cnt);

	rtc_exit_critical(int_level);
	AON_RTC_LOGD("%s[-]\r\n", __func__);
}
#endif

void bk_aon_rtc_dump(aon_rtc_id_t id)
{
#if defined(CONFIG_AON_RTC_DEBUG)
	uint32_t i = 0, index = 0;

	for(i = s_isr_cnt - AON_RTC_ISR_DEBUG_MAX_CNT; i < s_isr_cnt; i++)
	{
		index = i % AON_RTC_ISR_DEBUG_MAX_CNT;

		for(volatile uint32_t j = 0; j < 1800; j++);	//confirm log output normarlly
		
		AON_RTC_LOGI("isr_in[%d]=0x%llx,out=0x%llx,set=0x%llx\r\n", index, s_isr_debug_in_tick[index], s_isr_debug_out_tick[index], s_isr_debug_set_tick[index]);
	}
#endif
	aon_rtc_struct_dump();

	alarm_dump_list(s_aon_rtc[id].alarm_head_p);
}

bk_err_t bk_rtc_gettimeofday(struct timeval *tv, void *ptz)
{
    (void)ptz;

    if(tv!=NULL)
    {
        uint64_t uCurTimeUs = s_boot_time_us + bk_aon_rtc_get_us();

        tv->tv_sec=uCurTimeUs/1000000;
        tv->tv_usec=uCurTimeUs%1000000;
        AON_RTC_LOGD("s_boot_time_us:h=0x%x,l=0x%x \r\n", s_boot_time_us>>32, (uint32_t)s_boot_time_us);
        AON_RTC_LOGD("%s sec=%d,us:%d\n", __func__, tv->tv_sec, tv->tv_usec);
    } else
        AON_RTC_LOGW("%s tv is null \r\n",__func__);

    return BK_OK;
}

bk_err_t bk_rtc_settimeofday(const struct timeval *tv,const struct timezone *tz)
{
    (void)tz;
    if(tv)
    {
        uint64_t setTimeUs = ((uint64_t)tv->tv_sec)*1000000LL + tv->tv_usec ;
        uint64_t getCurTimeUs = bk_aon_rtc_get_us();

        s_boot_time_us = setTimeUs - getCurTimeUs;
        AON_RTC_LOGD("%s:sec=%d us=%d\n", __func__, tv->tv_sec, tv->tv_usec);
        AON_RTC_LOGD("get us:h=0x%x,l=0x%x\n", getCurTimeUs>>32, (uint32_t)getCurTimeUs);

#if defined(CONFIG_AON_RTC_KEEP_TIME_SUPPORT)
        aon_rtc_bake_timeofday();
#endif
    }

    return BK_OK;
}


#if defined(CONFIG_AON_RTC_DEBUG)
void bk_64bits_test(void)
{
	uint64_t val_64bits = 0xffffffffffff;
	uint64_t x1 = 0x111111111, x2 = 0x222222222, x3 = 0x333333333, t = 0;
	uint32_t xh1 = 0x1, xl1 = 0x11111111;
	//uint32_t xh2 = 0x2, xl2 = 0x22222222;
	//uint32_t xh3 = 0x3, xl3 = 0x33333333;
	//uint32_t th1 = 0, tl1 = 0;
	
	t = xh1;
	t = t<<32;
	t += xl1;
	if (t == 0x111111111)
	{
		AON_RTC_LOGD("left move 0x1<<32 is right\r\n");
	}

	if (t == x1)
	{
		AON_RTC_LOGD("uint64 compare is right\r\n");
	}

	if ((t & 0xffffffff) == xl1)
	{
		AON_RTC_LOGD("uint64 low 32bits is right\r\n");
	}

	if ((t >> 32) == xh1)
	{
		AON_RTC_LOGD("right move uint64 high 32bits is right\r\n");
	}

	if(x1 + x2 == x3)
	{
		AON_RTC_LOGD("uint64 add is right\r\n");
	}

	if(x3 - x2 == x1)
	{
		AON_RTC_LOGD("uint64 minus is right\r\n");
	}

	if((x1 * 2) == x2)
	{
		AON_RTC_LOGD("uint64 multi is right\r\n");
	}	

	if((x2 / 2) == x1)
	{
		AON_RTC_LOGD("uint64 divide2 is right\r\n");
	}	

	if((x2 / 32) == 0x11111111)
	{
		AON_RTC_LOGD("uint64 divide32 is right\r\n");
	}	

	//pass:only output low 32 bits valid data
	for(uint32_t i = 0; i < 64; i++)
		AON_RTC_LOGD("0xffffffffffff>>%d == 0x%llx\r\n", i, val_64bits>>i);		//64 bits printf is error

	//print:BIT64(i) low 32 bits
	for(uint32_t i = 0; i < 64; i++)
		AON_RTC_LOGD("Bit[%d] = 0x%llx, &=0x%llx\r\n", i, BIT64(i), (val_64bits & BIT64(i)));

	AON_RTC_LOGD("64bits move\r\n");
	for(uint64_t i = 0; i < 64; i++)
		AON_RTC_LOGD("Bit[%d] = 0x%llx, &=0x%llx\r\n", i, BIT64(i), (val_64bits & BIT64(i)));

	AON_RTC_LOGD("32bits move\r\n");
	val_64bits = 0xa0a0a0a0a0a0a0a0;
	for(uint32_t i = 0; i < 64; i++)
	{
		uint64_t ret = val_64bits & BIT64(i);
		AON_RTC_LOGD("ret[%d] = 0x%llx = 0x%llx \r\n", i, ret, ret);	//print ret twice as 64 bits
	}

	AON_RTC_LOGD("64bits move\r\n");
	val_64bits = 0xa0a0a0a0a0a0a0a0;
	for(uint64_t i = 0; i < 64; i++)
	{
		uint64_t ret = val_64bits & BIT64(i);
		AON_RTC_LOGD("ret[%d] = 0x%llx = 0x%llx \r\n", i, i, ret);		//print i twice as 64 bits
	}
}
#endif
