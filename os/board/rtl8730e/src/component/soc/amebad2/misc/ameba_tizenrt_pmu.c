#include "platform_autoconf.h"
#include "ameba_soc.h"

uint32_t missing_tick = 0;

#ifndef CONFIG_PLATFORM_TIZENRT_OS
static uint32_t wakelock     = DEFAULT_WAKELOCK;
#endif
static uint32_t sleepwakelock_timeout     = 0;
static u32 system_can_yield = 1; /* default is can */
static uint32_t sleep_type = SLEEP_PG; /* 0 is power gate, 1 is clock gate */
static uint32_t max_sleep_time = 0; /* if user want wakeup peridically, can set this timer*/
SLEEP_ParamDef sleep_param ALIGNMTO(32); /* cacheline aligned for lp & np */

static uint32_t deepwakelock     = DEFAULT_DEEP_WAKELOCK;
static uint32_t deepwakelock_timeout     = 0;
static uint32_t sysactive_timeout_temp = 0;
static uint32_t sysactive_timeout_flag = 0;

#ifdef CONFIG_SMP
/* cpu hotplug flag for each core */
volatile u32 cpuhp_flag[CONFIG_SMP_NCPUS];
#endif

/* ++++++++ TizenRT macro implementation ++++++++ */

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
static int tizenrt_systick_check(u32 time1, u32 time2)
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

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	TimeOut = xTaskGetTickCount() + timeout;
#else
	TimeOut = TICK2MSEC(clock_systimer()) + timeout;
#endif

	if (tizenrt_systick_check(TimeOut, sleepwakelock_timeout)) {
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
int tizenrt_ready_to_sleep(void)
{
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	u32 current_tick = xTaskGetTickCount();
#else
	u32 current_tick = TICK2MSEC(clock_systimer());
#endif

	/* timeout */
	if (tizenrt_systick_check(current_tick, sleepwakelock_timeout) == FALSE) {
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

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	if (wakelock == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
#endif
	return TRUE;
}

/*
 *  It is called in tizenrt pre_sleep_processing.
 *
 *  @return  true  : System is ready to check conditions that if it can enter dsleep.
 *           false : System can't enter deep sleep.
 **/
CONFIG_FW_CRITICAL_CODE_SECTION
int tizenrt_ready_to_dsleep(void)
{
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	u32 current_tick = xTaskGetTickCount();
#else
	u32 current_tick = TICK2MSEC(clock_systimer());
#endif

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
 *  It is called when tizenrt is going to sleep.
 *  At this moment, all sleep conditons are satisfied. All tizenrt' sleep pre-processing are done.
 *
 *  @param  expected_idle_time : The time that TizenRT expect to sleep.
 *                               If we set this value to 0 then TizenRT will do nothing in its sleep function.
 **/
#if 0 //for longrun test
VOID pg_aontimer_int(u32 Data)
{
	DBG_8195A("pg Hp aontimer handler 1\n", SOCPS_AONWakeReason());
	SOCPS_AONTimerClearINT();
	DBG_8195A("pg Hp aontimer handler 2\n", SOCPS_AONWakeReason());
	RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, DISABLE);
}
#endif


#ifdef CONFIG_PM_TICKSUPPRESS
static void (*tizenrt_sleep_handler)(clock_t);
void up_register_wakehandler(void (*handler)(clock_t))
{
	tizenrt_sleep_handler = handler;
}
#endif

void tizenrt_pre_sleep_processing(uint32_t *expected_idle_time)
{
	uint32_t tick_before_sleep;
	uint32_t tick_passed;
	volatile uint32_t ms_passed = 0;

	if (tizenrt_ready_to_dsleep()) {
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

	/* When we reach back here, LP core might be still printing information 
	Thus, we should add wait LOGUART TX complete, before AP core start printing logs */
	LOGUART_WaitTxComplete();

	/*  update kernel tick by calculating passed tick from gtimer */
	/*  get current gtimer timestamp */
	tick_passed = SYSTIMER_GetPassTick(tick_before_sleep);

	tick_passed += missing_tick;
	missing_tick = tick_passed & 0x1F;

	/* ms =x*1000/32768 = (x *1000) >>15 */
	ms_passed = (u32)((((u64)tick_passed) * 1000) >> 15);

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	vTaskStepTick(ms_passed); /*  update kernel tick */
#else
#ifdef CONFIG_PM_TICKSUPPRESS
	if (tizenrt_sleep_handler) {
		tizenrt_sleep_handler((u64)ms_passed); /*  update kernel tick */
	}
#endif
#endif

	sysactive_timeout_flag = 0;

#ifndef CONFIG_CLINTWOOD
	pmu_set_sysactive_time(2);
#endif

	pmvdbg("ap sleeped:[%d] ms\n", ms_passed);
}

CONFIG_FW_CRITICAL_CODE_SECTION
void tizenrt_post_sleep_processing(uint32_t *expected_idle_time)
{
#ifndef CONFIG_ARCH_CORTEXA32
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

/* -------- TizenRT macro implementation -------- */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
void pmu_acquire_wakelock(uint32_t nDeviceId)
{
	u32 PrevStatus;
#ifndef CONFIG_ARCH_CORTEXA32
	PrevStatus = ulSetInterruptMaskFromISR();
#else
	PrevStatus = portDISABLE_INTERRUPTS();
#endif

	wakelock |= BIT(nDeviceId);

#ifndef CONFIG_ARCH_CORTEXA32
	vClearInterruptMaskFromISR(PrevStatus);
#else
	portRESTORE_INTERRUPTS(PrevStatus);
#endif
}

void pmu_release_wakelock(uint32_t nDeviceId)
{
	u32 PrevStatus;
#ifndef CONFIG_ARCH_CORTEXA32
	PrevStatus = ulSetInterruptMaskFromISR();
#else
	PrevStatus = portDISABLE_INTERRUPTS();
#endif

	wakelock &= ~BIT(nDeviceId);

#ifndef CONFIG_ARCH_CORTEXA32
	vClearInterruptMaskFromISR(PrevStatus);
#else
	portRESTORE_INTERRUPTS(PrevStatus);
#endif
}

uint32_t pmu_get_wakelock_status(void)
{
	return wakelock;
}
#endif

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

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	timeout = xTaskGetTickCount() + TimeOutMs;
#else
	timeout = TICK2MSEC(clock_systimer()) + TimeOutMs;
#endif
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

// TODO: For hotplug mode
#ifdef CONFIG_SMP
void pmu_set_secondary_cpu_state(uint32_t CoreID, uint32_t NewStatus)
{
	cpuhp_flag[CoreID] = NewStatus;
}

uint32_t pmu_get_secondary_cpu_state(uint32_t CoreID)
{
	return cpuhp_flag[CoreID];
}

bool pmu_secondary_cpu_state_is_running(uint32_t CoreID)
{
	if (cpuhp_flag[CoreID] == CPU1_RUNNING) 
		return 1;
	else 
		return 0;
}

bool pmu_secondary_cpu_state_is_hotplug(uint32_t CoreID)
{
	if (cpuhp_flag[CoreID] == CPU1_HOTPLUG) 
		return 1;
	else 
		return 0;
}

bool pmu_secondary_cpu_state_is_wake(uint32_t CoreID)
{
	if (cpuhp_flag[CoreID] == CPU1_WAKE_FROM_PG) 
		return 1;
	else 
		return 0;
}
#endif
