
#include <lib/mmio.h>
#include "systimer.h"

void sys_timer_enable(unsigned char en)
{
	unsigned int value = mmio_read_32(CNT_CONTROL_BASE + SYSTIM_CNTCR);

	if (en)
		value |= SYSTIM_CNT_EN_BIT;
	else
		value &= ~SYSTIM_CNT_EN_BIT;

	mmio_write_32(CNT_CONTROL_BASE + SYSTIM_CNTCR, value);
}


