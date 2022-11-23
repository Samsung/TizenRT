//#include "FreeRTOS.h"
//#include <task.h>

//#include "freertos_pmu.h"
#include "platform_autoconf.h"
#include "ameba_soc.h"

uint32_t missing_tick = 0;

static uint32_t wakelock     = DEFAULT_WAKELOCK;
static uint32_t sleepwakelock_timeout     = 0;
static u32 system_can_yield = 1; /* default is can */
static uint32_t sleep_type = SLEEP_PG; /* 0 is power gate, 1 is clock gate */
static uint32_t max_sleep_time = 0; /* if user want wakeup peridically, can set this timer*/
SLEEP_ParamDef sleep_param ALIGNMTO(32); /* cacheline aligned for lp & np */

static uint32_t deepwakelock     = DEFAULT_DEEP_WAKELOCK;
static uint32_t deepwakelock_timeout     = 0;
static uint32_t sysactive_timeout_temp = 0;
static uint32_t sysactive_timeout_flag = 0;

u32 tickless_debug = 0;


/* ++++++++ FreeRTOS macro implementation ++++++++ */

/* psm dd hook info */
PSM_DD_HOOK_INFO gPsmDdHookInfo[PMU_MAX];

u32 pmu_exec_sleep_hook_funs(void)
{
	PSM_DD_HOOK_INFO *pPsmDdHookInfo = NULL;
	u32 nDeviceIdOffset = 0;
	u32 ret = TRUE;

	for (nDeviceIdOffset = 0; nDeviceIdOffset < PMU_MAX; nDeviceIdOffset++) {
		pPsmDdHookInfo = &gPsmDdHookInfo[nDeviceIdOffset];

		/*if this device register and sleep_hook_fun not NULL*/
		if (pPsmDdHookInfo && pPsmDdHookInfo->sleep_hook_fun) {
			ret = pPsmDdHookInfo->sleep_hook_fun(0, pPsmDdHookInfo->sleep_param_ptr);

			if (ret == FALSE) {
				break;
			}
		}
	}

	return nDeviceIdOffset;
}

void pmu_exec_wakeup_hook_funs(u32 nDeviceIdMax)
{
	PSM_DD_HOOK_INFO *pPsmDdHookInfo = NULL;
	u32 nDeviceIdOffset = 0;

	for (nDeviceIdOffset = 0; nDeviceIdOffset < nDeviceIdMax; nDeviceIdOffset++) {
		pPsmDdHookInfo = &gPsmDdHookInfo[nDeviceIdOffset];

		/*if this device register and sleep_hook_fun not NULL*/
		if (pPsmDdHookInfo && pPsmDdHookInfo->wakeup_hook_fun) {
			pPsmDdHookInfo->wakeup_hook_fun(0, pPsmDdHookInfo->wakeup_param_ptr);
		}
	}
}

#define SYSTICK_THRES 0x7fffffff
/*
return: TRUE: time1 > time2
*/
static int freertos_systick_check(u32 time1, u32 time2)
{
	u32 delta = time1 > time2 ? time1 - time2 : time2 - time1;
	if (delta < SYSTICK_THRES) {
		return time1 > time2 ? TRUE : FALSE;
	} else {	//overflow
		return time1 < time2 ? TRUE : FALSE;
	}
}


uint32_t pmu_set_sysactive_time(uint32_t timeout)
{
	u32 TimeOut = 0;

	if (sysactive_timeout_flag) {
		if (timeout > sysactive_timeout_temp) {
			sysactive_timeout_temp = timeout;
		}
		return 1;
	}

	if (sysactive_timeout_temp > timeout) {
		timeout = sysactive_timeout_temp;
	}
	sysactive_timeout_temp = 0;

	TimeOut = xTaskGetTickCount() + timeout;

	if (freertos_systick_check(TimeOut, sleepwakelock_timeout)) {
		sleepwakelock_timeout = TimeOut;
	}

	return 0;
}

uint32_t pmu_set_sysactive_timeFromISR(uint32_t timeout)
{
	u32 TimeOut = 0;

	if (sysactive_timeout_flag) {
		if (timeout > sysactive_timeout_temp) {
			sysactive_timeout_temp = timeout;
		}
		return 1;
	}

	if (sysactive_timeout_temp > timeout) {
		timeout = sysactive_timeout_temp;
	}
	sysactive_timeout_temp = 0;

	TimeOut = xTaskGetTickCountFromISR() + timeout;

	if (freertos_systick_check(TimeOut, sleepwakelock_timeout)) {
		sleepwakelock_timeout = TimeOut;
	}

	return 0;
}


void pmu_register_sleep_callback(u32 nDeviceId, PSM_HOOK_FUN sleep_hook_fun, void *sleep_param_ptr,
								 PSM_HOOK_FUN wakeup_hook_fun, void *wakeup_param_ptr)
{
	PSM_DD_HOOK_INFO *pPsmDdHookInfo = NULL;

	assert_param(nDeviceId < PMU_MAX);
	assert_param((sleep_hook_fun != NULL) || (wakeup_hook_fun != NULL));

	pPsmDdHookInfo = &gPsmDdHookInfo[nDeviceId];
	pPsmDdHookInfo->nDeviceId			= nDeviceId;
	pPsmDdHookInfo->sleep_hook_fun		= sleep_hook_fun;
	pPsmDdHookInfo->sleep_param_ptr		= sleep_param_ptr;
	pPsmDdHookInfo->wakeup_hook_fun		= wakeup_hook_fun;
	pPsmDdHookInfo->wakeup_param_ptr	= wakeup_param_ptr;
}

void pmu_unregister_sleep_callback(u32 nDeviceId)
{
	PSM_DD_HOOK_INFO *pPsmDdHookInfo = NULL;

	assert_param(nDeviceId < PMU_MAX);

	pPsmDdHookInfo = &gPsmDdHookInfo[nDeviceId];

	_memset(pPsmDdHookInfo, 0x00, sizeof(PSM_DD_HOOK_INFO));
}


/* can not yield CPU under suspend/resume process */
uint32_t pmu_yield_os_check(void)
{
	return system_can_yield;
}

//for test
#if 0
VOID aontimer_int(u32 Data)
{
	(VOID)Data;

	DBG_8195A("aontimer handler 1: %x\n", SOCPS_AONWakeReason());
	SOCPS_AONTimerClearINT();
	DBG_8195A("aontimer handler 2: %x\n", SOCPS_AONWakeReason());
	RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, DISABLE);
}

VOID aontimer_test()
{
	u32 KR4_is_NP = LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1));
#if defined (ARM_CORE_CM4)
	if (KR4_is_NP) {
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		SOCPS_AONTimer(2000);
		SOCPS_AONTimerINT_EN(ENABLE);
		InterruptRegister(aontimer_int, AON_TIM_IRQ, NULL, 3);
		InterruptEn(AON_TIM_IRQ, 3);
		SOCPS_SetAPWakeEvent_MSK1(WAKE_SRC_AON_TIM, ENABLE);
	}
#else
	if (!KR4_is_NP) {
		BKUP_Set(BKUP_REG1, BIT(2));
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		SOCPS_AONTimer(2000);
		SOCPS_AONTimerINT_EN(ENABLE);
		InterruptRegister(aontimer_int, AON_TIM_IRQ, NULL, 3);
		InterruptEn(AON_TIM_IRQ, 3);
		SOCPS_SetAPWakeEvent_MSK1(WAKE_SRC_AON_TIM, ENABLE);
	}
#endif
}
#endif

/*
 *  It is called in idle task.
 *
 *  @return  true  : System is ready to check conditions that if it can enter sleep.
 *           false : System keep awake.
 **/
CONFIG_FW_CRITICAL_CODE_SECTION
int freertos_ready_to_sleep(void)
{
	u32 current_tick = xTaskGetTickCountFromISR();

	/* timeout */
	if (freertos_systick_check(current_tick, sleepwakelock_timeout) == FALSE) {
		return FALSE;
	}

	/* DSP shall PG firstly because DSP power is higher than AP */
	if ((sleep_type == SLEEP_PG) && dsp_status_on()) {
		return FALSE;
	}


	if (wakelock == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

#if defined (CONFIG_FW_DRIVER_COEXIST) && CONFIG_FW_DRIVER_COEXIST
int freertos_check_wakelock_timeout(void)
{
	u32 current_tick = xTaskGetTickCount();

	/* timeout */
	if (freertos_systick_check(current_tick, sleepwakelock_timeout) == FALSE) {
		return FALSE;
	} else {
		return TRUE;
	}

}
#endif


/*
 *  It is called in freertos pre_sleep_processing.
 *
 *  @return  true  : System is ready to check conditions that if it can enter dsleep.
 *           false : System can't enter deep sleep.
 **/
CONFIG_FW_CRITICAL_CODE_SECTION
int freertos_ready_to_dsleep(void)
{
	u32 current_tick = xTaskGetTickCountFromISR();

	/* timeout */
	if (current_tick < deepwakelock_timeout) {
		return FALSE;
	}

	if (deepwakelock == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
 *  It is called when freertos is going to sleep.
 *  At this moment, all sleep conditons are satisfied. All freertos' sleep pre-processing are done.
 *
 *  @param  expected_idle_time : The time that FreeRTOS expect to sleep.
 *                               If we set this value to 0 then FreeRTOS will do nothing in its sleep function.
 **/

void freertos_pre_sleep_processing(unsigned int *expected_idle_time)
{
	uint32_t tick_before_sleep;
	uint32_t tick_passed;
	volatile uint32_t ms_passed = 0;

	if (freertos_ready_to_dsleep()) {
		sleep_param.sleep_time = 0;// do not wake on system schedule tick
		sleep_param.dlps_enable = ENABLE;
	} else {
		sleep_param.sleep_time = max_sleep_time;//*expected_idle_time;
		max_sleep_time = 0;
		sleep_param.dlps_enable = DISABLE;
	}
	sleep_param.sleep_type = sleep_type;

	*expected_idle_time = 0;

	/*  Store gtimer timestamp before sleep */
	tick_before_sleep = SYSTIMER_TickGet();
	sysactive_timeout_flag = 1;

	/* for test */
#if 0
	aontimer_test();
#endif

	if (sleep_type == SLEEP_PG) {
		SOCPS_SleepPG();
	} else {
		SOCPS_SleepCG();
	}

	/*  update kernel tick by calculating passed tick from gtimer */
	/*  get current gtimer timestamp */
	tick_passed = SYSTIMER_GetPassTick(tick_before_sleep);

	tick_passed += missing_tick;
	missing_tick = tick_passed & 0x1F;

	/* ms =x*1000/32768 = (x *1000) >>15 */
	ms_passed = (u32)((((u64)tick_passed) * 1000) >> 15);

	vTaskStepTick(ms_passed); /*  update kernel tick */

	sysactive_timeout_flag = 0;
	pmu_set_sysactive_timeFromISR(2);

	if (tickless_debug) {
#if defined (ARM_CORE_CM4)
		DBG_8195A("km4 sleeped:[%d] ms\n", ms_passed);
#else
		DBG_8195A("kr4 sleeped:[%d] ms\n", ms_passed);
#endif
	}

}


CONFIG_FW_CRITICAL_CODE_SECTION
#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
void freertos_post_sleep_processing(unsigned int *expected_idle_time)
{
#if defined (ARM_CORE_CM4)

#ifndef configSYSTICK_CLOCK_HZ
	*expected_idle_time = 1 + (SysTick->VAL / (configCPU_CLOCK_HZ / configTICK_RATE_HZ));
#else
	*expected_idle_time = 1 + (SysTick->VAL / (configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ));
#endif
#elif defined (RSICV_CORE_KR4)
	(void)expected_idle_time;
#endif
}
#endif

/* NVIC will power off under sleep power gating mode, so we can */
/* not use systick like FreeRTOS default implementation */
CONFIG_FW_CRITICAL_CODE_SECTION
#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
	unsigned int xModifiableIdleTime;
	eSleepModeStatus eSleepStatus;

	system_can_yield = 0;

	/* disable systick count */
	Systick_Cmd(DISABLE);
	/* disable interrupts, including systick */
	__disable_irq();

	eSleepStatus = eTaskConfirmSleepModeStatus();

	/* If a context switch is pending or a task is waiting for the scheduler
	to be unsuspended then abandon the low power entry. */
	if (eSleepStatus == eAbortSleep) {
	} else if (freertos_ready_to_sleep()) {
		/* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
		set its parameter to 0 to indicate that its implementation contains
		its own wait for interrupt or wait for event instruction, and so wfi
		should not be executed again.  However, the original expected idle
		time variable must remain unmodified, so a copy is taken. */
		xModifiableIdleTime = (unsigned int)xExpectedIdleTime;
		configPRE_SLEEP_PROCESSING(xModifiableIdleTime);
		if (xModifiableIdleTime > 0) {
			__DSB();
			__WFI();
			__ISB();
		}
		configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
	} else {

		Systick_Cmd(ENABLE);
		__DSB();
		__WFI();
		__NOP();		//fix cpu fused instruction issue
		__ISB();
	}

	/* Re-enable interrupts */
	__enable_irq();
	/* Re-enable systick count */
	Systick_Cmd(ENABLE);

	system_can_yield = 1;
}
#endif

/* -------- FreeRTOS macro implementation -------- */

void pmu_acquire_wakelock(uint32_t nDeviceId)
{
	uint32_t PrevStatus = irq_disable_save();
	wakelock |= BIT(nDeviceId);
	irq_enable_restore(PrevStatus);
}

void pmu_release_wakelock(uint32_t nDeviceId)
{
	uint32_t PrevStatus = irq_disable_save();
	wakelock &= ~BIT(nDeviceId);
	irq_enable_restore(PrevStatus);
}

uint32_t pmu_get_wakelock_status(void)
{
	return wakelock;
}

uint32_t pmu_set_sleep_type(uint32_t type)
{
	sleep_type = type;

	return 0;
}

uint32_t pmu_get_sleep_type(void)
{
	return sleep_type;
}

void pmu_set_max_sleep_time(uint32_t timer_ms)
{
	max_sleep_time = timer_ms;
}

void pmu_set_dsleep_active_time(uint32_t TimeOutMs)
{
	u32 timeout = 0;


	timeout = xTaskGetTickCount() + TimeOutMs;
	//DBG_8195A("pmu_set_dsleep_active_time: %d %d\n", timeout, deepwakelock_timeout);

	if (timeout > deepwakelock_timeout) {
		deepwakelock_timeout = timeout;
	}
}

void pmu_acquire_deepwakelock(uint32_t nDeviceId)
{
	deepwakelock |= BIT(nDeviceId);
}

void pmu_release_deepwakelock(uint32_t nDeviceId)
{
	deepwakelock &= ~BIT(nDeviceId);
}

uint32_t pmu_get_deepwakelock_status(void)
{
	return deepwakelock;
}


void pmu_tickless_debug(u32 NewStatus)
{
	if (NewStatus == ENABLE) {
		tickless_debug = 1;
	} else {
		tickless_debug = 0;
	}
}
