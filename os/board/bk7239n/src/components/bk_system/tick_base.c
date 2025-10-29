#include "sys_rtos.h"
#include "bk_fake_clock.h"
#include "bk_timer.h"
#include "bk_icu.h"
#include "bk_drv_model.h"
#include "bk_uart.h"
#include <os/os.h>
#include <components/system.h>

#include <components/log.h>
#include "bk_ps_time.h"

#include "bk_wdt.h"
#include <driver/timer.h>
#include "tick_timer_id.h"

#define TAG "tick"

static int s_tick_timer_id = TICK_TIMER_ID;

int bk_get_tick_timer_id(void)
{
	return s_tick_timer_id;
}

int bk_tick_init(void)
{
	s_tick_timer_id = TICK_TIMER_ID;
	bk_timer_start(s_tick_timer_id, bk_get_ms_per_tick(), (timer_isr_t)bk_tick_handle);

#if defined(CONFIG_TICK_CALI)
	extern uint32_t bk_cal_init(uint32_t setting); //TODO fix it
	bk_cal_init(0);
#endif
	return BK_OK;
}

int bk_tick_reload(uint32_t time_ms)
{
	bk_timer_start(s_tick_timer_id, time_ms, (timer_isr_t)bk_tick_handle);
	return BK_OK;
}