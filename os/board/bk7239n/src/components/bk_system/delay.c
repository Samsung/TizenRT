#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_misc.h"
#include "bk_sys_ctrl.h"
#include "bk_fake_clock.h"
#include "bk_drv_model.h"
#include <components/system.h>
#include "sys_driver.h"
#include "arch_delay.h"
#include "timer_driver.h"

#if 1
void bk_delay(INT32 num)
{
	volatile INT32 i, j;

	for (i = 0; i < num; i ++) {
		for (j = 0; j < 100; j ++)
			;
	}
}
#endif

__IRAM_SEC void delay_us(UINT32 us)
{
#if defined(CONFIG_TIMER_US)
	bk_timer_delay_us(us);
#else
	arch_delay_us(us);
#endif
}

void delay_ms(UINT32 ms)
{
#if defined(CONFIG_TIMER_US)
	bk_timer_delay_us(1000 * ms);
#else
	arch_delay_us(1000 * ms);
#endif
}

void delay_sec(UINT32 ms_count)
{
	UINT32 t0;
	UINT32 t1;

	t0 = fclk_get_second();
	while (1) {
		t1 = fclk_get_second();
		if (t1 - t0 >= 1)
			break;
	}
}

void delay_tick(UINT32 tick_count)
{
	UINT32 t0;
	UINT32 t1;

	t0 = bk_get_tick();
	while (1) {
		t1 = bk_get_tick();
		if (t1 - t0 >= 1)
			break;
	}
}
