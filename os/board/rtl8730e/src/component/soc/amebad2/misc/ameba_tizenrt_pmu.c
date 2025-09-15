#include "platform_autoconf.h"
#include "ameba_soc.h"
#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>

static uint32_t missing_tick = 0;

#ifndef CONFIG_PLATFORM_TIZENRT_OS
static uint32_t wakelock     = DEFAULT_WAKELOCK;
#endif
static uint32_t sleepwakelock_timeout     = 0;
static uint32_t sleep_type = SLEEP_PG; /* 0 is power gate, 1 is clock gate */
SLEEP_ParamDef sleep_param ALIGNMTO(32); /* cacheline aligned for lp & np */

static uint32_t sysactive_timeout_temp = 0;
static uint32_t sysactive_timeout_flag = 0;

/* ++++++++ TizenRT macro implementation ++++++++ */

/* psm dd hook info */
static PSM_DD_HOOK_INFO gPsmDdHookInfo[PMU_MAX];

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
	if ((HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_FEN) & (HSYS_BIT_FEN_AP))) {
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

static clock_t g_missing_tick_ms = 0;

void tizenrt_sleep_processing(void)
{
	uint32_t tick_before_sleep;
	uint32_t tick_passed;
	volatile uint32_t ms_passed = 0;

	sleep_param.sleep_type = sleep_type;

	/*  Store gtimer timestamp before sleep */
	tick_before_sleep = SYSTIMER_TickGet();

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

	g_missing_tick_ms = (clock_t)ms_passed;

#ifndef CONFIG_PLATFORM_TIZENRT_OS
	vTaskStepTick(ms_passed); /*  update kernel tick */
#endif
}

pm_wakeup_reason_code_t tizenrt_get_wakeupreason(void)
{
	/* Store wakeup reason and missing ticks for TizenRT PM */
	u16 raw_wakeup_reason = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_089);
	return (pm_wakeup_reason_code_t)raw_wakeup_reason;
}

clock_t tizenrt_get_missingtick(void)
{
	return g_missing_tick_ms;
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
#endif	/* CONFIG_PM */
