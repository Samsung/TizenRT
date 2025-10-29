#include <stdint.h>
#include <stdbool.h>
#include "hal_jtag.h"
#include "dubhe_otp.h"


int dubhe_driver_init( unsigned long);
void arm_ce_dbg_set(bool debug_en);
void close_wdt(void);
bool efuse_is_spe_debug_enabled(void);
void hal_jtag_enable(void);
void hal_jtag_disable(void);

static bool allowed_debug(void)
{
	uint8_t lcs[4] = {0};
	int ret;

	ret = arm_ce_get_otp_info(6/*OTP_INFO_LIFE_CYCLE*/, (uint8_t *)lcs, 4, 0);
	if (0 != ret) {
		return false;
	}

	if ((lcs[0] == 0x0) || (lcs[0] == 0x1)) {
		return true;
	}

	if (efuse_is_spe_debug_enabled()) {
		return true;
	}

	return false;
}

void hal_secure_debug(void)
{
	dubhe_driver_init(0x4b110000);

	if (allowed_debug()) {
		hal_jtag_enable();
	} else {
		hal_jtag_disable();
	}
}
void hal_jtag_enable(void)
{
#if !CONFIG_BL2_WDT
	close_wdt();
#endif
	arm_ce_dbg_set(true);
	if (efuse_is_spe_debug_enabled()) {
		*((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) = CORTEX_M33_CPU_DEBUG_ENABLE_VALUE;
	} else {
		*((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) = CORTEX_M33_CPU_DEBUG_ENABLE_VALUE;
	}
}

void hal_jtag_disable(void)
{
	*((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) = CORTEX_M33_CPU_DEBUG_DISABLE_VALUE;
	arm_ce_dbg_set(false);
}
// eof

