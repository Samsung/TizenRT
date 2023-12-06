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


#ifdef ARM_CORE_CA32
/* Add declaration here for TizenRT to compile */
#ifndef configNUM_CORES
#define configNUM_CORES			1
#endif
/* cpu hotplug flag for each core */
volatile u32 cpuhp_flag[configNUM_CORES];
#endif

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
		return time1 >= time2 ? TRUE : FALSE;
	} else {	//overflow
		return time1 <= time2 ? TRUE : FALSE;
	}
}


uint32_t pmu_set_sysactive_time(uint32_t timeout)
{
	uint32_t TimeOut = 0;

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

#if defined (ARM_CORE_CM4)
u32 ap_clk_status_on(void)
{

	if (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & HSYS_BIT_CKE_AP) {
		return 1;
	} else {
		return 0;
	}
}

u32 ap_status_on(void)
{
	if ((HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (HSYS_BIT_PSW_HP_AP_L2))) {
		return 1;
	} else {
		return 0;
	}
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
	u32 current_tick = xTaskGetTickCount();

	/* timeout */
	if (freertos_systick_check(current_tick, sleepwakelock_timeout) == FALSE) {
		return FALSE;
	}

#if defined (ARM_CORE_CM4)
	if ((HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW) & LSYS_BIT_AP_ENABLE)) {
		if (!(HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW) & LSYS_BIT_AP_RUNNING)) {
			if (! ap_status_on()) {
				pmu_set_sleep_type(SLEEP_PG);
			} else if (! ap_clk_status_on()) {
				pmu_set_sleep_type(SLEEP_CG);
			} else {
				return FALSE;
			}
			pmu_release_wakelock(PMU_OS);
		} else {
			return FALSE;
		}
	}
#endif

	if (wakelock == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
 *  It is called in freertos pre_sleep_processing.
 *
 *  @return  true  : System is ready to check conditions that if it can enter dsleep.
 *           false : System can't enter deep sleep.
 **/
CONFIG_FW_CRITICAL_CODE_SECTION
int freertos_ready_to_dsleep(void)
{
	u32 current_tick = xTaskGetTickCount();

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
#if defined (ARM_CORE_CM4)
#if 0 //for longrun test
VOID pg_aontimer_int(u32 Data)
{
	DBG_8195A("pg Hp aontimer handler 1\n", SOCPS_AONWakeReason());
	SOCPS_AONTimerClearINT();
	DBG_8195A("pg Hp aontimer handler 2\n", SOCPS_AONWakeReason());
	RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, DISABLE);
}
#endif

void freertos_pre_sleep_processing(uint32_t *expected_idle_time)
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
#ifdef AMEBAD2_TODO
	IPCM0_DEV->IPCx_USR[IPC_INT_CHAN_SHELL_SWITCH] = 0x00000000;
#endif

	//BKUP_Set(BKUP_REG0, BIT_KM4_WAKE_DELAY);
	/* for test */
#if 0
	RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
	u32 tmp = rand();
	SOCPS_AONTimer(tmp % 800 + 50);
	SOCPS_AONTimerINT_EN(ENABLE);
	InterruptRegister(pg_aontimer_int, AON_TIM_IRQ, NULL, 7);
	InterruptEn(AON_TIM_IRQ, 7);
	SOCPS_SetNPWakeEvent_MSK0_HP(WAKE_SRC_AON_TIM, ENABLE);
#endif

	if (sleep_type == SLEEP_PG) {
		SOCPS_SleepPG();
	} else {
		SOCPS_SleepCG();
	}

	//pmu_set_sysactive_time(5);
	//BKUP_Clear(BKUP_REG0, BIT_KM4_WAKE_DELAY);

	/*  update kernel tick by calculating passed tick from gtimer */
	/*  get current gtimer timestamp */
	tick_passed = SYSTIMER_GetPassTick(tick_before_sleep);

	tick_passed += missing_tick;
	missing_tick = tick_passed & 0x1F;

	/* ms =x*1000/32768 = (x *1000) >>15 */
	ms_passed = (u32)((((u64)tick_passed) * 1000) >> 15);

	vTaskStepTick(ms_passed); /*  update kernel tick */

	sysactive_timeout_flag = 0;
	pmu_set_sysactive_time(2);

	if (tickless_debug) {
		DBG_8195A("np sleeped:[%d] ms\n", ms_passed);
	}
}

#elif defined (ARM_CORE_CA32)

void freertos_pre_sleep_processing(uint32_t *expected_idle_time)
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
#ifdef AMEBAD2_TODO
	IPCM0_DEV->IPCx_USR[IPC_INT_CHAN_SHELL_SWITCH] = 0x00000000;
	InterruptDis(UART_LOG_IRQ);
#endif

	if (sleep_type == SLEEP_CG) {
		SOCPS_SleepCG();
	} else {
		SOCPS_SleepPG();
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

#ifndef CONFIG_CLINTWOOD
	pmu_set_sysactive_time(2);
#endif

	if (tickless_debug) {
		DBG_8195A("ap sleeped:[%d] ms\n", ms_passed);
	}
}
#else

CONFIG_FW_CRITICAL_CODE_SECTION
void freertos_pre_sleep_processing(uint32_t *expected_idle_time)
{

#ifdef CONFIG_SOC_PS_MODULE
	uint32_t tick_before_sleep;
	uint32_t tick_passed;
	volatile uint32_t ms_passed = 0;
	*expected_idle_time = 0;

	/*  Store gtimer timestamp before sleep */
	tick_before_sleep = SYSTIMER_TickGet();
	sysactive_timeout_flag = 1;

	/* some function call like xTaskGetTickCount may cause IRQ ON, */
	/* so we close IRQ again here to avoid sys irq when enter or exit sleep */
	//__asm volatile( "cpsid i" );

	SOCPS_SWRLDO_Suspend(ENABLE);

	/* make sure the following function dont call system API that may open IRQ */

	SOCPS_SleepInit();

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

	/* timer clock is 32768, 0x20 is 1ms */
	ms_passed = (((tick_passed & 0xFFFF8000) >> 15) * 1000) + (((tick_passed & 0x7FFF) * 1000) >> 15);

	vTaskStepTick(ms_passed); /*  update kernel tick */

	if (ps_config.km0_tickles_debug) {
		DBG_8195A("km0 sleeped:[%d] ms\n", ms_passed);
	}

	sysactive_timeout_flag = 0;
	sleepwakelock_timeout = xTaskGetTickCount() + 1;

	SOCPS_SWRLDO_Suspend(DISABLE);
	//__asm volatile( "cpsie i" );
#else
	/*  If PS is not enabled, then use freertos sleep function */
#endif
}
#endif

CONFIG_FW_CRITICAL_CODE_SECTION
void freertos_post_sleep_processing(uint32_t *expected_idle_time)
{
#ifndef ARM_CORE_CA32
#ifndef configSYSTICK_CLOCK_HZ
	*expected_idle_time = 1 + (SysTick->VAL / (configCPU_CLOCK_HZ / configTICK_RATE_HZ));
#else
	*expected_idle_time = 1 + (SysTick->VAL / (configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ));
#endif
#else
	UNUSED(expected_idle_time);
#ifdef AMEBAD2_TODO
#endif
#endif

}

#ifndef ARM_CORE_CA32
/* NVIC will power off under sleep power gating mode, so we can */
/* not use systick like FreeRTOS default implementation */
CONFIG_FW_CRITICAL_CODE_SECTION
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
	uint32_t xModifiableIdleTime;
	eSleepModeStatus eSleepStatus;

	system_can_yield = 0;

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	/* Enter a critical section but don't use the taskENTER_CRITICAL()
	method as that will mask interrupts that should exit sleep mode. */
#if defined (ARM_CORE_CM0)
	taskENTER_CRITICAL();
#else
	__asm volatile("cpsid i");
#endif

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
		xModifiableIdleTime = (uint32_t)xExpectedIdleTime;
		configPRE_SLEEP_PROCESSING(xModifiableIdleTime);
		if (xModifiableIdleTime > 0) {
			__asm volatile("dsb");
			__asm volatile("wfi");
			__asm volatile("isb");
		}
		configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
	}
#if defined (ARM_CORE_CM4)
	else {
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		__asm volatile("dsb");
		__asm volatile("wfi");
		__asm volatile("nop");   //fix cpu fused instruction issue
		__asm volatile("isb");
	}
#endif
	/* Re-enable interrupts - see comments above the cpsid instruction()
	above. */
#if defined (ARM_CORE_CM0)
	taskEXIT_CRITICAL();
#else
	__asm volatile("cpsie i");
#endif

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	system_can_yield = 1;
}
#else
u32 check_wfi_state(u8 core_id)
{
	CA32_TypeDef *ca32 = CA32_BASE;
	u32 core_state = (ca32->CA32_C0_CPU_STATUS);

	if ((core_id == 0) && (core_state & BIT8)) {
		/* core0 already in WFE state*/
		return 1;
	} else if ((core_id == 1) && (core_state & BIT9)) {
		/* core1 already in WFE state*/
		return 1;
	} else {
		return 0;
	}
}

#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
	uint32_t xModifiableIdleTime;
	eSleepModeStatus eSleepStatus;

	/* need further check*/
	system_can_yield = 0;

	if (portGET_CORE_ID() == 0) {
		/* mask sys tick interrupt*/
		arm_arch_timer_int_mask(pdTRUE);

		/* Enter a critical section but don't use the taskENTER_CRITICAL()
		method as that will mask interrupts that should exit sleep mode. */
		portDISABLE_INTERRUPTS();

		portGET_TASK_LOCK();
		eSleepStatus = eTaskConfirmSleepModeStatus();
		portRELEASE_TASK_LOCK();

		/* If a context switch is pending or a task is waiting for the scheduler
		to be unsuspended then abandon the low power entry. */
		if (eSleepStatus == eAbortSleep) {
			arm_arch_timer_int_mask(pdFALSE);
		} else if (freertos_ready_to_sleep()) {

			//HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_AON_BACKUP2, 0);
			/* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
			set its parameter to 0 to indicate that its implementation contains
			its own wait for interrupt or wait for event instruction, and so wfi
			should not be executed again.  However, the original expected idle
			time variable must remain unmodified, so a copy is taken. */
			xModifiableIdleTime = (uint32_t)xExpectedIdleTime;

#if ( configNUM_CORES > 1 )
			/*PG flow */
			if (pmu_get_sleep_type() == SLEEP_PG) {
				/* CPU1 just come back from pg, so can't sleep here */
				if (pmu_get_secondary_cpu_state(1) == CPU1_WAKE_FROM_PG) {
					goto EXIT;
				}

				/* CPU1 is in task schedular, tell CPU1 to enter hotplug */
				if (pmu_get_secondary_cpu_state(1) == CPU1_RUNNING) {
					/* CPU1 may in WFI idle state. Wake it up to enter hotplug itself */
					portENABLE_INTERRUPTS();
					arm_gic_raise_softirq(1, 0);
					arm_arch_timer_int_mask(pdFALSE);
					DelayUs(100);
					goto EXIT;
				}

				/* CG flow */
			} else {
				if (!check_wfi_state(1)) {
					goto EXIT;
				}
			}
#endif
			configPRE_SLEEP_PROCESSING(xModifiableIdleTime);

			/* When wake from pg, arm timer has been reset, so a new compare value is necessary to
			   trigger an timer interrupt */
			if (pmu_get_sleep_type() == SLEEP_PG) {
				arm_arch_timer_set_compare(arm_arch_timer_count() + 50000);
			}
			arm_arch_timer_int_mask(pdFALSE);
			//if (xModifiableIdleTime > 0) {
			//	__asm volatile("dsb");
			//	__asm volatile("wfi");
			//	__asm volatile("isb");
			//}
			configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
		} else {
			/* power saving when idle*/
			arm_arch_timer_int_mask(pdFALSE);
			__asm(" DSB");
			__asm(" WFI");
			__asm(" ISB");
			/* power saving when idle*/
		}
#if ( configNUM_CORES > 1 )
EXIT:
#endif
		/* Re-enable interrupts and sys tick*/
		portENABLE_INTERRUPTS();
	} else if (portGET_CORE_ID() == 1) {
		if (pmu_get_sleep_type() == SLEEP_PG) {
			if (freertos_ready_to_sleep()) {
				/* CPU1 will enter hotplug state. Raise a task yield to migrate its task */
				pmu_set_secondary_cpu_state(1, CPU1_HOTPLUG);
				portYIELD();
			}
		}

		portDISABLE_INTERRUPTS();
		__asm("	DSB");
		__asm("	WFI");
		__asm("	ISB");
		portENABLE_INTERRUPTS();
	}

	/* need further check*/
	system_can_yield = 1;
}
#endif
#endif
/* -------- FreeRTOS macro implementation -------- */

void pmu_acquire_wakelock(uint32_t nDeviceId)
{
//	u32 PrevStatus;
//#ifndef ARM_CORE_CA32
//	PrevStatus = ulSetInterruptMaskFromISR();
//#else
//	PrevStatus = portDISABLE_INTERRUPTS();
//#endif
//
//	wakelock |= BIT(nDeviceId);
//
//#ifndef ARM_CORE_CA32
//	vClearInterruptMaskFromISR(PrevStatus);
//#else
//	portRESTORE_INTERRUPTS(PrevStatus);
//#endif
}

void pmu_release_wakelock(uint32_t nDeviceId)
{
//	u32 PrevStatus;
//#ifndef ARM_CORE_CA32
//	PrevStatus = ulSetInterruptMaskFromISR();
//#else
//	PrevStatus = portDISABLE_INTERRUPTS();
//#endif
//
//	wakelock &= ~BIT(nDeviceId);
//
//#ifndef ARM_CORE_CA32
//	vClearInterruptMaskFromISR(PrevStatus);
//#else
//	portRESTORE_INTERRUPTS(PrevStatus);
//#endif
}

uint32_t pmu_get_wakelock_status(void)
{
	return wakelock;
}

uint32_t pmu_set_sleep_type(uint32_t type)
{
	sleep_type = type;
#if defined (ARM_CORE_CM0)
	if (ps_config.km0_pg_enable) {
		sleep_type = SLEEP_PG;
	}
#endif
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

#ifdef ARM_CORE_CA32
void pmu_set_secondary_cpu_state(uint32_t CoreID, uint32_t NewStatus)
{
	cpuhp_flag[CoreID] = NewStatus;
}

uint32_t pmu_get_secondary_cpu_state(uint32_t CoreID)
{
	return cpuhp_flag[CoreID];
}

int pmu_secondary_cpu_state_is_running(uint32_t CoreID)
{
	return (cpuhp_flag[CoreID] == CPU1_RUNNING);
}

int pmu_secondary_cpu_state_is_hotplug(uint32_t CoreID)
{
	return (cpuhp_flag[CoreID] == CPU1_HOTPLUG);
}

int pmu_secondary_cpu_state_is_wake(uint32_t CoreID)
{
	return (cpuhp_flag[CoreID] == CPU1_WAKE_FROM_PG);
}
#endif

