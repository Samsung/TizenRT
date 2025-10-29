#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "icu_hal.h"
#include "icu_ll.h"

//TODO complete the ICU hal init, currently only set/get some ICU registers
bk_err_t icu_hal_init(icu_hal_t *hal)
{
	hal->hw = (icu_hw_t *)ICU_LL_REG_BASE();
	return BK_OK;
}
