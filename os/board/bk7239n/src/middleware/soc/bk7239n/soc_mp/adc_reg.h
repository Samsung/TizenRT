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


#define ADC_REG0_ADDR (SOC_SADC_REG_BASE + (0x0 << 2))

#define ADC_REG0_DEVICEID_POS (0)
#define ADC_REG0_DEVICEID_MASK (0xffffffff)

#define ADC_REG1_ADDR (SOC_SADC_REG_BASE + (0x1 << 2))

#define ADC_REG1_VERSIONID_POS (0)
#define ADC_REG1_VERSIONID_MASK (0xffffffff)

#define ADC_REG2_ADDR (SOC_SADC_REG_BASE + (0x2 << 2))

#define ADC_REG2_SOFT_RST_POS (0)
#define ADC_REG2_SOFT_RST_MASK (0x1)

#define ADC_REG2_BYPASS_CKG_POS (1)
#define ADC_REG2_BYPASS_CKG_MASK (0x1)

#define ADC_REG2_RESERVED_BIT_2_31_POS (2)
#define ADC_REG2_RESERVED_BIT_2_31_MASK (0x3fffffff)

#define ADC_REG3_ADDR (SOC_SADC_REG_BASE + (0x3 << 2))

#define ADC_REG3_GBSTATUS_POS (0)
#define ADC_REG3_GBSTATUS_MASK (0xffffffff)

#define ADC_REG4_ADDR (SOC_SADC_REG_BASE + (0x4 << 2))

#define ADC_REG4_ENABLE_POS (0)
#define ADC_REG4_ENABLE_MASK (0x1)

#define ADC_REG4_CALIB_MODE_POS (1)
#define ADC_REG4_CALIB_MODE_MASK (0x1)

#define ADC_REG4_CALIB_DONE_POS (2)
#define ADC_REG4_CALIB_DONE_MASK (0x1)

#define ADC_REG4_SAMP_SEL_POS (3)
#define ADC_REG4_SAMP_SEL_MASK (0x1)

#define ADC_REG4_ADC_CHAN_POS (4)
#define ADC_REG4_ADC_CHAN_MASK (0xf)

#define ADC_REG4_ADC_MODE_POS (8)
#define ADC_REG4_ADC_MODE_MASK (0x1)

#define ADC_REG4_ADC_DUMP_NUM_POS (9)
#define ADC_REG4_ADC_DUMP_NUM_MASK (0x7)

#define ADC_REG4_ADC_SCLK_DIV_POS (12)
#define ADC_REG4_ADC_SCLK_DIV_MASK (0xf)

#define ADC_REG4_ADC_SRAW_POS (16)
#define ADC_REG4_ADC_SRAW_MASK (0x1)

#define ADC_REG4_RESERVED_BIT_17_31_POS (17)
#define ADC_REG4_RESERVED_BIT_17_31_MASK (0x7fff)

#define ADC_REG5_ADDR (SOC_SADC_REG_BASE + (0x5 << 2))

#define ADC_REG5_CO_GAIN_POS (0)
#define ADC_REG5_CO_GAIN_MASK (0xff)

#define ADC_REG5_RESERVED_BIT_8_31_POS (8)
#define ADC_REG5_RESERVED_BIT_8_31_MASK (0xffffff)

#define ADC_REG6_ADDR (SOC_SADC_REG_BASE + (0x6 << 2))

#define ADC_REG6_CO_OFFSET_POS (0)
#define ADC_REG6_CO_OFFSET_MASK (0xffff)

#define ADC_REG6_RESERVED_BIT_16_31_POS (16)
#define ADC_REG6_RESERVED_BIT_16_31_MASK (0xffff)

#define ADC_REG7_ADDR (SOC_SADC_REG_BASE + (0x7 << 2))

#define ADC_REG7_CWT_CAL00_POS (0)
#define ADC_REG7_CWT_CAL00_MASK (0xffffff)

#define ADC_REG7_RESERVED_BIT_24_31_POS (24)
#define ADC_REG7_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG8_ADDR (SOC_SADC_REG_BASE + (0x8 << 2))

#define ADC_REG8_CWT_CAL01_POS (0)
#define ADC_REG8_CWT_CAL01_MASK (0xffffff)

#define ADC_REG8_RESERVED_BIT_24_31_POS (24)
#define ADC_REG8_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG9_ADDR (SOC_SADC_REG_BASE + (0x9 << 2))

#define ADC_REG9_CWT_CAL02_POS (0)
#define ADC_REG9_CWT_CAL02_MASK (0xffffff)

#define ADC_REG9_RESERVED_BIT_24_31_POS (24)
#define ADC_REG9_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REGA_ADDR (SOC_SADC_REG_BASE + (0xa << 2))

#define ADC_REGA_CWT_CAL03_POS (0)
#define ADC_REGA_CWT_CAL03_MASK (0xffffff)

#define ADC_REGA_RESERVED_BIT_24_31_POS (24)
#define ADC_REGA_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REGB_ADDR (SOC_SADC_REG_BASE + (0xb << 2))

#define ADC_REGB_CWT_CAL04_POS (0)
#define ADC_REGB_CWT_CAL04_MASK (0xffffff)

#define ADC_REGB_RESERVED_BIT_24_31_POS (24)
#define ADC_REGB_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REGC_ADDR (SOC_SADC_REG_BASE + (0xc << 2))

#define ADC_REGC_CWT_CAL05_POS (0)
#define ADC_REGC_CWT_CAL05_MASK (0xffffff)

#define ADC_REGC_RESERVED_BIT_24_31_POS (24)
#define ADC_REGC_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REGD_ADDR (SOC_SADC_REG_BASE + (0xd << 2))

#define ADC_REGD_CWT_CAL06_POS (0)
#define ADC_REGD_CWT_CAL06_MASK (0xffffff)

#define ADC_REGD_RESERVED_BIT_24_31_POS (24)
#define ADC_REGD_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REGE_ADDR (SOC_SADC_REG_BASE + (0xe << 2))

#define ADC_REGE_CWT_CAL07_POS (0)
#define ADC_REGE_CWT_CAL07_MASK (0xffffff)

#define ADC_REGE_RESERVED_BIT_24_31_POS (24)
#define ADC_REGE_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REGF_ADDR (SOC_SADC_REG_BASE + (0xf << 2))

#define ADC_REGF_CWT_CAL08_POS (0)
#define ADC_REGF_CWT_CAL08_MASK (0xffffff)

#define ADC_REGF_RESERVED_BIT_24_31_POS (24)
#define ADC_REGF_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG10_ADDR (SOC_SADC_REG_BASE + (0x10 << 2))

#define ADC_REG10_CWT_CAL09_POS (0)
#define ADC_REG10_CWT_CAL09_MASK (0xffffff)

#define ADC_REG10_RESERVED_BIT_24_31_POS (24)
#define ADC_REG10_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG11_ADDR (SOC_SADC_REG_BASE + (0x11 << 2))

#define ADC_REG11_CWT_CAL0A_POS (0)
#define ADC_REG11_CWT_CAL0A_MASK (0xffffff)

#define ADC_REG11_RESERVED_BIT_24_31_POS (24)
#define ADC_REG11_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG12_ADDR (SOC_SADC_REG_BASE + (0x12 << 2))

#define ADC_REG12_CWT_CAL0B_POS (0)
#define ADC_REG12_CWT_CAL0B_MASK (0xffffff)

#define ADC_REG12_RESERVED_BIT_24_31_POS (24)
#define ADC_REG12_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG13_ADDR (SOC_SADC_REG_BASE + (0x13 << 2))

#define ADC_REG13_CWT_CAL0C_POS (0)
#define ADC_REG13_CWT_CAL0C_MASK (0xffffff)

#define ADC_REG13_RESERVED_BIT_24_31_POS (24)
#define ADC_REG13_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG14_ADDR (SOC_SADC_REG_BASE + (0x14 << 2))

#define ADC_REG14_CWT_CAL0D_POS (0)
#define ADC_REG14_CWT_CAL0D_MASK (0xffffff)

#define ADC_REG14_RESERVED_BIT_24_31_POS (24)
#define ADC_REG14_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG15_ADDR (SOC_SADC_REG_BASE + (0x15 << 2))

#define ADC_REG15_CWT_CAL0E_POS (0)
#define ADC_REG15_CWT_CAL0E_MASK (0xffffff)

#define ADC_REG15_RESERVED_BIT_24_31_POS (24)
#define ADC_REG15_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG16_ADDR (SOC_SADC_REG_BASE + (0x16 << 2))

#define ADC_REG16_CWT_CAL0F_POS (0)
#define ADC_REG16_CWT_CAL0F_MASK (0xffffff)

#define ADC_REG16_RESERVED_BIT_24_31_POS (24)
#define ADC_REG16_RESERVED_BIT_24_31_MASK (0xff)

#define ADC_REG17_ADDR (SOC_SADC_REG_BASE + (0x17 << 2))

#define ADC_REG17_ADC_INT_EN_POS (0)
#define ADC_REG17_ADC_INT_EN_MASK (0x1)

#define ADC_REG17_ADC_INT_LEVEL_POS (1)
#define ADC_REG17_ADC_INT_LEVEL_MASK (0x3f)

#define ADC_REG17_RESERVED_BIT_7_31_POS (7)
#define ADC_REG17_RESERVED_BIT_7_31_MASK (0x1ffffff)

#define ADC_REG18_ADDR (SOC_SADC_REG_BASE + (0x18 << 2))

#define ADC_REG18_FIFO_RDATA_POS (0)
#define ADC_REG18_FIFO_RDATA_MASK (0xffff)

#define ADC_REG18_RESERVED_BIT_16_31_POS (16)
#define ADC_REG18_RESERVED_BIT_16_31_MASK (0xffff)

#define ADC_REG19_ADDR (SOC_SADC_REG_BASE + (0x19 << 2))

#define ADC_REG19_FIFO_FULL_POS (0)
#define ADC_REG19_FIFO_FULL_MASK (0x1)

#define ADC_REG19_FIFO_EMPTY_POS (1)
#define ADC_REG19_FIFO_EMPTY_MASK (0x1)

#define ADC_REG19_INT_STATE_POS (2)
#define ADC_REG19_INT_STATE_MASK (0x1)

#define ADC_REG19_RESERVED_BIT_3_31_POS (3)
#define ADC_REG19_RESERVED_BIT_3_31_MASK (0x1fffffff)

#ifdef __cplusplus
}
#endif
