#if defined(CONFIG_AON_RTC)
#include <driver/aon_rtc.h>
#include <driver/aon_rtc_types.h>
#endif
#include "bk_pm_internal_api.h"
#include <driver/pwr_clk.h>

#define TAG "os"

#if defined(CONFIG_AON_RTC)

static uint64_t base_aon_time = 0;
static uint32_t base_os_time = 0;
#endif

void rtos_init_base_time(void) {
#if defined(CONFIG_AON_RTC)
	base_aon_time = bk_aon_rtc_get_us()/1000;
	base_os_time = rtos_get_time();
	BK_LOGI(TAG, "os time(%dms).\r\n", base_os_time);
	BK_LOGI(TAG, "base aon rtc time: %d:%d\r\n", (uint32_t)(base_aon_time >> 32),
		(uint32_t)(base_aon_time & 0xFFFFFFFF));
#endif
}

uint32_t rtos_get_time_diff(void) {
#if defined(CONFIG_AON_RTC)
	uint64_t cur_aon_time = bk_aon_rtc_get_us()/1000;
	uint64_t cur_os_time = (uint64_t)rtos_get_time(); //ms
	uint64_t diff_time = (cur_aon_time - base_aon_time); //ms
	uint32_t diff_ms = 0;

	if((uint32_t)(diff_time >> 32) & 0x7FF0000) {
		BK_LOGI(TAG, "aon_rtc overfollow....\r\n");
		BK_DUMP_OUT("diff time: 0x%x:0x%08x\r\n", (u32)(diff_time >> 32), (u32)(diff_time & 0xFFFFFFFF));
		return 0;
	}

	if (diff_time + base_os_time < cur_os_time) {
		return 0;
	}

	diff_ms = (uint32_t)(diff_time + base_os_time - cur_os_time);

	if (diff_ms > 20000) {
		BK_LOGI(TAG, "aon_rtc diff_ms: %dms.\r\n", diff_ms);
		BK_LOGI(TAG, "cur aon time: 0x%x:0x%08x\r\n", (u32)(cur_aon_time >> 32), (u32)(cur_aon_time & 0xFFFFFFFF));
	}

	return  diff_ms / OS_MS_PER_TICK; // tick
#else
	return 0;
#endif
}


// Workaroud for BK7236 systick rate.
// TODO remove it once BK7236 supports systick of fixed 32K
static uint32_t s_tick_clock_rate = configSYSTICK_CLOCK_HZ;
void os_update_tick_clock_rate(uint32_t rate)
{
	if (rate == s_tick_clock_rate) {
		return;
	}

	/* Calculate the constants required to configure the tick interrupt. */
#if ( configUSE_TICKLESS_IDLE >= 1 )
	ulTimerCountsForOneTick = ( rate / configTICK_RATE_HZ );
	xMaximumPossibleSuppressedTicks = portMAX_24_BIT_NUMBER / ulTimerCountsForOneTick;
	ulStoppedTimerCompensation = portMISSED_COUNTS_FACTOR / ( configCPU_CLOCK_HZ / rate );
	BK_LOGI(TAG, "ulTimerCountsForOneTick=%u, xMaximumPossibleSuppressedTicks=%u, ulStoppedTimerCompensation=%u\r\n",
		ulTimerCountsForOneTick, xMaximumPossibleSuppressedTicks, ulStoppedTimerCompensation);
#endif

	uint32_t cur_val = portNVIC_SYSTICK_CURRENT_VALUE_REG;
	uint32_t cur_load = portNVIC_SYSTICK_LOAD_REG;
	uint32_t new_load = ( rate / configTICK_RATE_HZ ) - 1UL;
	uint32_t new_val = 0;

	if (cur_load && cur_val) {
		new_val = (uint32_t)((cur_val * new_load) / cur_load);
	}

	/* Stop and reset the SysTick. */
	portNVIC_SYSTICK_CTRL_REG = 0UL;
	portNVIC_SYSTICK_CURRENT_VALUE_REG = new_val;

	/* Configure SysTick to interrupt at the requested rate. */
	portNVIC_SYSTICK_LOAD_REG = new_load;
	portNVIC_SYSTICK_CTRL_REG = portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT;

	BK_LOGI(TAG, "cur_val=%u, cur_load=%u, new_val=%u, new_load=%u\r\n",
		cur_val, cur_load, new_val, new_load);
}

void dump_os_tick_info(void)
{
	BK_LOGI(TAG, "configSYSTICK_CLOCK_HZ=%x\r\n", configSYSTICK_CLOCK_HZ);
	BK_LOGI(TAG, "configCPU_CLOCK_HZ=%x\r\n", configCPU_CLOCK_HZ);
	BK_LOGI(TAG, "configTICK_RATE_HZ=%x\r\n", configTICK_RATE_HZ);
	BK_LOGI(TAG, "portMISSED_COUNTS_FACTOR=%x\r\n", portMISSED_COUNTS_FACTOR);

#if ( configUSE_TICKLESS_IDLE >= 1 )
	BK_LOGI(TAG, "ulTimerCountsForOneTick=%x\r\n", ulTimerCountsForOneTick);
	BK_LOGI(TAG, "xMaximumPossibleSuppressedTicks=%x\r\n", xMaximumPossibleSuppressedTicks);
	BK_LOGI(TAG, "ulStoppedTimerCompensation=%x\r\n", ulStoppedTimerCompensation);
#endif

	BK_LOGI(TAG, "portNVIC_SYSTICK_CURRENT_VALUE_REG3=%u\r\n", portNVIC_SYSTICK_CURRENT_VALUE_REG);
}


#if ( configUSE_TICKLESS_IDLE == 2 )
#define CONFIG_UPDATE_TICK_THEN_ENABLE_INT 1

static inline void systick_gated_update(TickType_t xExpectedIdleTime, uint32_t ulReloadValue)
{
#if defined(CONFIG_AON_RTC)
	TickType_t slept_ticks = rtos_get_time_diff();

	/* Restart SysTick so it runs from portNVIC_SYSTICK_LOAD_REG
	* again, then set portNVIC_SYSTICK_LOAD_REG back to its standard
	* value. */
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;
	portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1;
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
	if(slept_ticks > 1) {
		vTaskStepTick(slept_ticks - 1);
#if defined(CONFIG_TASK_WDT)
		bk_task_wdt_feed();
#endif
	}
#endif
}

#if !defined(CONFIG_UPDATE_TICK_THEN_ENABLE_INT)
static inline void systick_update(TickType_t xExpectedIdleTime, uint32_t ulReloadValue)
{
	uint32_t ulCompleteTickPeriods = 0, ulCompletedSysTickDecrements = 0;

	/* Disable the SysTick clock without reading the
	* portNVIC_SYSTICK_CTRL_REG register to ensure the
	* portNVIC_SYSTICK_COUNT_FLAG_BIT is not cleared if it is set.
	* Again, the time the SysTick is stopped for is accounted for as
	* best it can be, but using the tickless mode will inevitably
	* result in some tiny drift of the time maintained by the kernel
	* with respect to calendar time*/
	portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT );

	/* Determine if the SysTick clock has already counted to zero and
	* been set back to the current reload value (the reload back being
	* correct for the entire expected idle time) or if the SysTick is
	* yet to count to zero (in which case an interrupt other than the
	* SysTick must have brought the system out of sleep mode). */
	if( ( portNVIC_SYSTICK_CTRL_REG & portNVIC_SYSTICK_COUNT_FLAG_BIT ) != 0 ) {
		uint32_t ulCalculatedLoadValue;
		/* The tick interrupt is already pending, and the SysTick count
		* reloaded with ulReloadValue.  Reset the
		* portNVIC_SYSTICK_LOAD_REG with whatever remains of this tick
		* period. */
		ulCalculatedLoadValue = ( ulTimerCountsForOneTick - 1UL ) - ( ulReloadValue - portNVIC_SYSTICK_CURRENT_VALUE_REG );

		/* Don't allow a tiny value, or values that have somehow
		* underflowed because the post sleep hook did something
		* that took too long. */
		if( ( ulCalculatedLoadValue < ulStoppedTimerCompensation ) || ( ulCalculatedLoadValue > ulTimerCountsForOneTick ) ) {
			ulCalculatedLoadValue = ( ulTimerCountsForOneTick - 1UL );
		}

		portNVIC_SYSTICK_LOAD_REG = ulCalculatedLoadValue;

		/* As the pending tick will be processed as soon as this
		* function exits, the tick value maintained by the tick is
		* stepped forward by one less than the time spent waiting. */
		ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
	} else {

		/* Something other than the tick interrupt ended the sleep.
		* Work out how long the sleep lasted rounded to complete tick
		* periods (not the ulReload value which accounted for part
		* ticks). */
		ulCompletedSysTickDecrements = ( xExpectedIdleTime * ulTimerCountsForOneTick ) - portNVIC_SYSTICK_CURRENT_VALUE_REG;

		/* How many complete tick periods passed while the processor
		* was waiting? */
		ulCompleteTickPeriods = ulCompletedSysTickDecrements / ulTimerCountsForOneTick;

		/* The reload value is set to whatever fraction of a single tick
		* period remains. */
		portNVIC_SYSTICK_LOAD_REG = ( ( ulCompleteTickPeriods + 1UL ) * ulTimerCountsForOneTick ) - ulCompletedSysTickDecrements;
	}

	/* Restart SysTick so it runs from portNVIC_SYSTICK_LOAD_REG
	* again, then set portNVIC_SYSTICK_LOAD_REG back to its standard
	* value. */
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;

	if (ulCompleteTickPeriods > 0) {
		vTaskStepTick( ulCompleteTickPeriods );
	}

	portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1UL;
}
#endif

void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
	uint32_t ulReloadValue;
	TickType_t xModifiableIdleTime;
#if defined(CONFIG_SYS_CPU0)
  #if (CONFIG_CPU_CNT > 1)
	uint32_t cp1_psram_malloc_count_state       = 0;
	if(bk_pm_low_vol_vote_state_get())
	{
		cp1_psram_malloc_count_state = bk_pm_get_cp1_psram_malloc_count();
		pm_cp1_psram_malloc_count_state_set(cp1_psram_malloc_count_state);
		//pm_debug_module_state();
	}
	if(bk_pm_cp1_recovery_all_state_get())
	{
		bk_pm_module_check_cp1_shutdown();
	}
  #endif
#endif
	/* Make sure the SysTick reload value does not overflow the counter. */
	if( xExpectedIdleTime > xMaximumPossibleSuppressedTicks ) {
		xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
	}

	/* Stop the SysTick momentarily. The time the SysTick is stopped for is
	 * accounted for as best it can be, but using the tickless mode will
	 * inevitably result in some tiny drift of the time maintained by the
	 * kernel with respect to calendar time. */
	portNVIC_SYSTICK_CTRL_REG &= ~portNVIC_SYSTICK_ENABLE_BIT;
	/* Calculate the reload value required to wait xExpectedIdleTime
	 * tick periods. -1 is used because this code will execute part way
	 * through one of the tick periods. */
	ulReloadValue = portNVIC_SYSTICK_CURRENT_VALUE_REG + ( ulTimerCountsForOneTick * ( xExpectedIdleTime - 1UL ) );

	if( ulReloadValue > ulStoppedTimerCompensation ) {
		ulReloadValue -= ulStoppedTimerCompensation;
	}

	/* Enter a critical section but don't use the taskENTER_CRITICAL()
	 * method as that will mask interrupts that should exit sleep mode. */
	__asm volatile ( "cpsid i" ::: "memory" );
	__asm volatile ( "dsb" );
	__asm volatile ( "isb" );

	/* If a context switch is pending or a task is waiting for the scheduler
	 * to be un-suspended then abandon the low power entry. */
	if( eTaskConfirmSleepModeStatus() == eAbortSleep ) {
		/* Restart from whatever is left in the count register to complete
		* this tick period. */
		portNVIC_SYSTICK_LOAD_REG = portNVIC_SYSTICK_CURRENT_VALUE_REG;

		/* Reset the reload register to the value required for normal tick
		* periods. */
		portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1UL;

		/* Restart SysTick. */
		portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
		//portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_INT_BIT;
		/* Re-enable interrupts - see comments above the cpsid instruction()
		* above. */
		__asm volatile ( "cpsie i" ::: "memory" );
	} else {
		/* Set the new reload value. */
		portNVIC_SYSTICK_LOAD_REG = ulReloadValue;

		/* Clear the SysTick count flag and set the count value back to
		* zero. */
		portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

		/* Restart SysTick. */
		portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;

		/* Sleep until something happens. configPRE_SLEEP_PROCESSING() can
		* set its parameter to 0 to indicate that its implementation
		* contains its own wait for interrupt or wait for event
		* instruction, and so wfi should not be executed again. However,
		* the original expected idle time variable must remain unmodified,
		* so a copy is taken. */
		xModifiableIdleTime = xExpectedIdleTime;
		configPRE_SLEEP_PROCESSING( xModifiableIdleTime );

		if( xModifiableIdleTime > 0 ) {
#if defined(CONFIG_PM)
			pm_suspend(xModifiableIdleTime);
#else
			bk_pm_suppress_ticks_and_sleep(xModifiableIdleTime);
#endif
		}

		configPOST_SLEEP_PROCESSING( xExpectedIdleTime );
		#if defined(CONFIG_UPDATE_TICK_THEN_ENABLE_INT)
		systick_gated_update(xExpectedIdleTime, ulReloadValue);//it improve the systick update when adding here,otherwize the systick update fail and enter the vPortSuppressTicksAndSleep() fail
		#endif
		/* Re-enable interrupts to allow the interrupt that brought the MCU
		* out of sleep mode to execute immediately. See comments above
		* the cpsid instruction above. */
		__asm volatile ( "cpsie i" ::: "memory" );
		__asm volatile ( "dsb" );
		__asm volatile ( "isb" );

		/* Disable interrupts again because the clock is about to be stopped
		* and interrupts that execute while the clock is stopped will
		* increase any slippage between the time maintained by the RTOS and
		* calendar time. */
		__asm volatile ( "cpsid i" ::: "memory" );
		__asm volatile ( "dsb" );
		__asm volatile ( "isb" );
#if defined(CONFIG_UPDATE_TICK_THEN_ENABLE_INT)
		systick_gated_update(xExpectedIdleTime, ulReloadValue);//it improve the systick update when adding here,otherwize the systick update fail and enter the vPortSuppressTicksAndSleep() fai
#else
		systick_update(xExpectedIdleTime, ulReloadValue);
#endif
/* Restart SysTick. */
		portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
		/* Exit with interrupts enabled. */
		__asm volatile ( "cpsie i" ::: "memory" );
	}
}
#endif

