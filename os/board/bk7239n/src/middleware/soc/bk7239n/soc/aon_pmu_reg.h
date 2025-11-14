// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#define AON_PMU_R0_ADDR (SOC_AON_PMU_REG_BASE + (0x0 << 2))

#define AON_PMU_R0_MEMCHK_BPS_POS (0)
#define AON_PMU_R0_MEMCHK_BPS_MASK (0x1)

#define AON_PMU_R0_FAST_BOOT_POS (1)
#define AON_PMU_R0_FAST_BOOT_MASK (0x1)

#define AON_PMU_R0_DIG_WAKE_EN_POS (1)
#define AON_PMU_R0_DIG_WAKE_EN_MASK (0x1)

#define AON_PMU_R0_RESERVED_BIT_3_30_POS (3)
#define AON_PMU_R0_RESERVED_BIT_3_30_MASK (0xfffffff)

#define AON_PMU_R0_GPIO_SLEEP_POS (31)
#define AON_PMU_R0_GPIO_SLEEP_MASK (0x1)

#define AON_PMU_R2_ADDR (SOC_AON_PMU_REG_BASE + (0x2 << 2))

#define AON_PMU_R2_WDT_RST_ANA_POS (0)
#define AON_PMU_R2_WDT_RST_ANA_MASK (0x1)

#define AON_PMU_R2_WDT_RST_TOP_POS (1)
#define AON_PMU_R2_WDT_RST_TOP_MASK (0x1)

#define AON_PMU_R2_WDT_RST_AON_POS (2)
#define AON_PMU_R2_WDT_RST_AON_MASK (0x1)

#define AON_PMU_R2_RESERVED_BIT_3_7_POS (3)
#define AON_PMU_R2_RESERVED_BIT_3_7_MASK (0x1f)

#define AON_PMU_R2_RTC_VALUE_SAMP_SEL_POS (8)
#define AON_PMU_R2_RTC_VALUE_SAMP_SEL_MASK (0x1)

#define AON_PMU_R2_RESERVED_BIT_9_30_POS (9)
#define AON_PMU_R2_RESERVED_BIT_9_30_MASK (0x3fffff)

#define AON_PMU_R2_OTP_VDD_EN_POS (31)
#define AON_PMU_R2_OTP_VDD_EN_MASK (0x1)

#define AON_PMU_R25_ADDR (SOC_AON_PMU_REG_BASE + (0x25 << 2))

#define AON_PMU_R25_RSV_POS (0)
#define AON_PMU_R25_RSV_MASK (0xffffffff)

#define AON_PMU_R40_ADDR (SOC_AON_PMU_REG_BASE + (0x40 << 2))

#define AON_PMU_R40_WAKE1_DELAY_POS (0)
#define AON_PMU_R40_WAKE1_DELAY_MASK (0xf)

#define AON_PMU_R40_WAKE2_DELAY_POS (4)
#define AON_PMU_R40_WAKE2_DELAY_MASK (0xf)

#define AON_PMU_R40_WAKE3_DELAY_POS (8)
#define AON_PMU_R40_WAKE3_DELAY_MASK (0xf)

#define AON_PMU_R40_HALT1_DELAY_POS (12)
#define AON_PMU_R40_HALT1_DELAY_MASK (0xf)

#define AON_PMU_R40_HALT2_DELAY_POS (16)
#define AON_PMU_R40_HALT2_DELAY_MASK (0xf)

#define AON_PMU_R40_HALT3_DELAY_POS (20)
#define AON_PMU_R40_HALT3_DELAY_MASK (0xf)

#define AON_PMU_R40_HALT_VOLT_POS (24)
#define AON_PMU_R40_HALT_VOLT_MASK (0x1)

#define AON_PMU_R40_HALT_XTAL_POS (25)
#define AON_PMU_R40_HALT_XTAL_MASK (0x1)

#define AON_PMU_R40_HALT_CORE_POS (26)
#define AON_PMU_R40_HALT_CORE_MASK (0x1)

#define AON_PMU_R40_HALT_FLASH_POS (27)
#define AON_PMU_R40_HALT_FLASH_MASK (0x1)

#define AON_PMU_R40_HALT_ROSC_POS (28)
#define AON_PMU_R40_HALT_ROSC_MASK (0x1)

#define AON_PMU_R40_HALT_RESTEN_POS (29)
#define AON_PMU_R40_HALT_RESTEN_MASK (0x1)

#define AON_PMU_R40_HALT_ISOLAT_POS (30)
#define AON_PMU_R40_HALT_ISOLAT_MASK (0x1)

#define AON_PMU_R40_HALT_CLKENA_POS (31)
#define AON_PMU_R40_HALT_CLKENA_MASK (0x1)

#define AON_PMU_R41_ADDR (SOC_AON_PMU_REG_BASE + (0x41 << 2))

#define AON_PMU_R41_LPO_CONFIG_POS (0)
#define AON_PMU_R41_LPO_CONFIG_MASK (0x3)

#define AON_PMU_R41_FLSHSCK_IOCAP_POS (2)
#define AON_PMU_R41_FLSHSCK_IOCAP_MASK (0x3)

#define AON_PMU_R41_WAKEUP_ENA_POS (4)
#define AON_PMU_R41_WAKEUP_ENA_MASK (0x3f)

#define AON_PMU_R41_GPIO_CLKSEL_POS (10)
#define AON_PMU_R41_GPIO_CLKSEL_MASK (0x1)

#define AON_PMU_R41_RESERVED_BIT_11_23_POS (11)
#define AON_PMU_R41_RESERVED_BIT_11_23_MASK (0x1fff)

#define AON_PMU_R41_HALT_LPO_POS (24)
#define AON_PMU_R41_HALT_LPO_MASK (0x1)

#define AON_PMU_R41_HALT_CPU_POS (25)
#define AON_PMU_R41_HALT_CPU_MASK (0x1)

#define AON_PMU_R41_HALT_ANAREG_POS (26)
#define AON_PMU_R41_HALT_ANAREG_MASK (0x1)

#define AON_PMU_R41_HALT_LPEN_POS (27)
#define AON_PMU_R41_HALT_LPEN_MASK (0x1)

#define AON_PMU_R41_RESERVED_BIT_28_31_POS (28)
#define AON_PMU_R41_RESERVED_BIT_28_31_MASK (0xf)

#define AON_PMU_R43_ADDR (SOC_AON_PMU_REG_BASE + (0x43 << 2))

#define AON_PMU_R71_ADDR (SOC_AON_PMU_REG_BASE + (0x71 << 2))

#define AON_PMU_R71_RESERVED_BIT_0_19_POS (0)
#define AON_PMU_R71_RESERVED_BIT_0_19_MASK (0xfffff)

#define AON_PMU_R71_WAKEUP_SOURCE_POS (20)
#define AON_PMU_R71_WAKEUP_SOURCE_MASK (0x3)

#define AON_PMU_R71_ABNORMAL_WAKEUP_SOURCE_POS (22)
#define AON_PMU_R71_ABNORMAL_WAKEUP_SOURCE_MASK (0x3)

#define AON_PMU_R71_RESERVED_BIT_24_31_POS (24)
#define AON_PMU_R71_RESERVED_BIT_24_31_MASK (0xff)

#define AON_PMU_R72_ADDR (SOC_AON_PMU_REG_BASE + (0x72 << 2))

#define AON_PMU_R72_FINECNT_32K_SAMP_POS (0)
#define AON_PMU_R72_FINECNT_32K_SAMP_MASK (0x3ff)

#define AON_PMU_R72_BLE_CNT_RVLD_POS (10)
#define AON_PMU_R72_BLE_CNT_RVLD_MASK (0x1)

#define AON_PMU_R72_RESERVED_BIT_11_31_POS (11)
#define AON_PMU_R72_RESERVED_BIT_11_31_MASK (0x1fffff)

#define AON_PMU_R73_ADDR (SOC_AON_PMU_REG_BASE + (0x73 << 2))

#define AON_PMU_R73_CLKCNT_32K_SAMP_POS (0)
#define AON_PMU_R73_CLKCNT_32K_SAMP_MASK (0xfffffff)

#define AON_PMU_R73_RESERVED_BIT_28_31_POS (28)
#define AON_PMU_R73_RESERVED_BIT_28_31_MASK (0xf)

#define AON_PMU_R77_ADDR (SOC_AON_PMU_REG_BASE + (0x77 << 2))

#define AON_PMU_R77_ANA_GPIO_STA_POS (0)
#define AON_PMU_R77_ANA_GPIO_STA_MASK (0xffffff)

#define AON_PMU_R77_L1_POS (24)
#define AON_PMU_R77_L1_MASK (0xff)

#define AON_PMU_R78_ADDR (SOC_AON_PMU_REG_BASE + (0x78 << 2))

#define AON_PMU_R78_RTC_TICK_H_POS (0)
#define AON_PMU_R78_RTC_TICK_H_MASK (0xf)

#define AON_PMU_R78_RESERVED_BIT_4_31_POS (4)
#define AON_PMU_R78_RESERVED_BIT_4_31_MASK (0xfffffff)

#define AON_PMU_R79_ADDR (SOC_AON_PMU_REG_BASE + (0x79 << 2))

#define AON_PMU_R79_RTC_TICK_L_POS (0)
#define AON_PMU_R79_RTC_TICK_L_MASK (0xffffffff)

#define AON_PMU_R7C_ADDR (SOC_AON_PMU_REG_BASE + (0x7c << 2))

#define AON_PMU_R7C_ID_POS (0)
#define AON_PMU_R7C_ID_MASK (0xffffffff)

#define AON_PMU_R7D_ADDR (SOC_AON_PMU_REG_BASE + (0x7d << 2))

#define AON_PMU_R7D_ADC_CAL_POS (0)
#define AON_PMU_R7D_ADC_CAL_MASK (0x3f)

#define AON_PMU_R7D_BGCAL_POS (6)
#define AON_PMU_R7D_BGCAL_MASK (0x3f)

#define AON_PMU_R7D_26MPLL_UNLOCK_POS (12)
#define AON_PMU_R7D_26MPLL_UNLOCK_MASK (0x1)

#define AON_PMU_R7D_DPLL_UNLOCK_POS (13)
#define AON_PMU_R7D_DPLL_UNLOCK_MASK (0x1)

#define AON_PMU_R7D_BAND_POS (14)
#define AON_PMU_R7D_BAND_MASK (0x7f)

#define AON_PMU_R7D_L2_POS (21)
#define AON_PMU_R7D_L2_MASK (0x7ff)

#define AON_PMU_R7E_ADDR (SOC_AON_PMU_REG_BASE + (0x7e << 2))

#define AON_PMU_R7E_CBCAL_POS (0)
#define AON_PMU_R7E_CBCAL_MASK (0x1f)

#define AON_PMU_R7E_BAND_CAL_POS (5)
#define AON_PMU_R7E_BAND_CAL_MASK (0x3ff)

#define AON_PMU_R7E_ZCD_CAL_POS (16)
#define AON_PMU_R7E_ZCD_CAL_MASK (0x3f)

#define AON_PMU_R7E_L3_POS (22)
#define AON_PMU_R7E_L3_MASK (0x3ff)

#define AON_PMU_R7F_ADDR (SOC_AON_PMU_REG_BASE + (0x7f << 2))

#define AON_PMU_R7F_TD_INT_POS (0)
#define AON_PMU_R7F_TD_INT_MASK (0x1)

#define AON_PMU_R7F_TD_CHEN_POS (1)
#define AON_PMU_R7F_TD_CHEN_MASK (0xffff)

#define AON_PMU_R7F_TD_STATE_POS (17)
#define AON_PMU_R7F_TD_STATE_MASK (0x3)

#define AON_PMU_R7F_TD_DO_POS (19)
#define AON_PMU_R7F_TD_DO_MASK (0x1fff)

#ifdef __cplusplus
}
#endif
