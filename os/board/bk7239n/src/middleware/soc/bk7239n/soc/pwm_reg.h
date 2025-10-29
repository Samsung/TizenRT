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


#define PWM_REG0_ADDR (SOC_PWM_REG_BASE + (0x0 << 2))

#define PWM_REG0_DEVICEID_POS (0)
#define PWM_REG0_DEVICEID_MASK (0xffffffff)

#define PWM_REG1_ADDR (SOC_PWM_REG_BASE + (0x1 << 2))

#define PWM_REG1_VERSIONID_POS (0)
#define PWM_REG1_VERSIONID_MASK (0xffffffff)

#define PWM_REG2_ADDR (SOC_PWM_REG_BASE + (0x2 << 2))

#define PWM_REG2_SOFT_RESET_POS (0)
#define PWM_REG2_SOFT_RESET_MASK (0x1)

#define PWM_REG2_BPS_CLKGATE_POS (1)
#define PWM_REG2_BPS_CLKGATE_MASK (0x1)

#define PWM_REG2_RESERVED_BIT_2_31_POS (2)
#define PWM_REG2_RESERVED_BIT_2_31_MASK (0x3fffffff)

#define PWM_REG3_ADDR (SOC_PWM_REG_BASE + (0x3 << 2))

#define PWM_REG3_PWM_O_POS (0)
#define PWM_REG3_PWM_O_MASK (0xfff)

#define PWM_REG3_RESERVED_12_31_POS (12)
#define PWM_REG3_RESERVED_12_31_MASK (0xfffff)

#define PWM_REG4_ADDR (SOC_PWM_REG_BASE + (0x4 << 2))

#define PWM_REG4_CEN1_POS (0)
#define PWM_REG4_CEN1_MASK (0x1)

#define PWM_REG4_CEN2_POS (1)
#define PWM_REG4_CEN2_MASK (0x1)

#define PWM_REG4_CEN3_POS (2)
#define PWM_REG4_CEN3_MASK (0x1)

#define PWM_REG4_CEN4_POS (3)
#define PWM_REG4_CEN4_MASK (0x1)

#define PWM_REG4_CEN5_POS (4)
#define PWM_REG4_CEN5_MASK (0x1)

#define PWM_REG4_CEN6_POS (5)
#define PWM_REG4_CEN6_MASK (0x1)

#define PWM_REG4_CEN7_POS (6)
#define PWM_REG4_CEN7_MASK (0x1)

#define PWM_REG4_CEN8_POS (7)
#define PWM_REG4_CEN8_MASK (0x1)

#define PWM_REG4_CEN9_POS (8)
#define PWM_REG4_CEN9_MASK (0x1)

#define PWM_REG4_CEN10_POS (9)
#define PWM_REG4_CEN10_MASK (0x1)

#define PWM_REG4_CEN11_POS (10)
#define PWM_REG4_CEN11_MASK (0x1)

#define PWM_REG4_CEN12_POS (11)
#define PWM_REG4_CEN12_MASK (0x1)

#define PWM_REG4_SYNC_ALLPSC_POS (12)
#define PWM_REG4_SYNC_ALLPSC_MASK (0x1)

#define PWM_REG4_SYNC_ALLPSC_EN_POS (13)
#define PWM_REG4_SYNC_ALLPSC_EN_MASK (0x1)

#define PWM_REG4_RESERVED_14_31_POS (14)
#define PWM_REG4_RESERVED_14_31_MASK (0x3ffff)

#define PWM_REG10_ADDR (SOC_PWM_REG_BASE + (0x10 << 2))

#define PWM_REG10_PWM_O_0_ALLOCATE_POS (0)
#define PWM_REG10_PWM_O_0_ALLOCATE_MASK (0xffffffff)

#define PWM_REG11_ADDR (SOC_PWM_REG_BASE + (0x11 << 2))

#define PWM_REG11_PWM_O_1_ALLOCATE_POS (0)
#define PWM_REG11_PWM_O_1_ALLOCATE_MASK (0xffffffff)

#define PWM_REG12_ADDR (SOC_PWM_REG_BASE + (0x12 << 2))

#define PWM_REG12_PWM_O_2_ALLOCATE_POS (0)
#define PWM_REG12_PWM_O_2_ALLOCATE_MASK (0xffffffff)

#define PWM_REG13_ADDR (SOC_PWM_REG_BASE + (0x13 << 2))

#define PWM_REG13_PWM_O_3_ALLOCATE_POS (0)
#define PWM_REG13_PWM_O_3_ALLOCATE_MASK (0xffffffff)

#define PWM_REG14_ADDR (SOC_PWM_REG_BASE + (0x14 << 2))

#define PWM_REG14_PWM_O_4_ALLOCATE_POS (0)
#define PWM_REG14_PWM_O_4_ALLOCATE_MASK (0xffffffff)

#define PWM_REG15_ADDR (SOC_PWM_REG_BASE + (0x15 << 2))

#define PWM_REG15_PWM_O_5_ALLOCATE_POS (0)
#define PWM_REG15_PWM_O_5_ALLOCATE_MASK (0xffffffff)

#define PWM_REG16_ADDR (SOC_PWM_REG_BASE + (0x16 << 2))

#define PWM_REG16_PWM_O_6_ALLOCATE_POS (0)
#define PWM_REG16_PWM_O_6_ALLOCATE_MASK (0xffffffff)

#define PWM_REG17_ADDR (SOC_PWM_REG_BASE + (0x17 << 2))

#define PWM_REG17_PWM_O_7_ALLOCATE_POS (0)
#define PWM_REG17_PWM_O_7_ALLOCATE_MASK (0xffffffff)

#define PWM_REG18_ADDR (SOC_PWM_REG_BASE + (0x18 << 2))

#define PWM_REG18_PWM_O_8_ALLOCATE_POS (0)
#define PWM_REG18_PWM_O_8_ALLOCATE_MASK (0xffffffff)

#define PWM_REG19_ADDR (SOC_PWM_REG_BASE + (0x19 << 2))

#define PWM_REG19_PWM_O_9_ALLOCATE_POS (0)
#define PWM_REG19_PWM_O_9_ALLOCATE_MASK (0xffffffff)

#define PWM_REG1A_ADDR (SOC_PWM_REG_BASE + (0x1a << 2))

#define PWM_REG1A_PWM_O_10_ALLOCATE_POS (0)
#define PWM_REG1A_PWM_O_10_ALLOCATE_MASK (0xffffffff)

#define PWM_REG1B_ADDR (SOC_PWM_REG_BASE + (0x1b << 2))

#define PWM_REG1B_PWM_O_11_ALLOCATE_POS (0)
#define PWM_REG1B_PWM_O_11_ALLOCATE_MASK (0xffffffff)

#define PWM_REG20_ADDR (SOC_PWM_REG_BASE + (0x20 << 2))

#define PWM_REG20_PWM_I_0_ALLOCATE_POS (0)
#define PWM_REG20_PWM_I_0_ALLOCATE_MASK (0xffffffff)

#define PWM_REG21_ADDR (SOC_PWM_REG_BASE + (0x21 << 2))

#define PWM_REG21_PWM_I_1_ALLOCATE_POS (0)
#define PWM_REG21_PWM_I_1_ALLOCATE_MASK (0xffffffff)

#define PWM_REG22_ADDR (SOC_PWM_REG_BASE + (0x22 << 2))

#define PWM_REG22_PWM_I_2_ALLOCATE_POS (0)
#define PWM_REG22_PWM_I_2_ALLOCATE_MASK (0xffffffff)

#define PWM_REG23_ADDR (SOC_PWM_REG_BASE + (0x23 << 2))

#define PWM_REG23_PWM_I_3_ALLOCATE_POS (0)
#define PWM_REG23_PWM_I_3_ALLOCATE_MASK (0xffffffff)

#define PWM_REG24_ADDR (SOC_PWM_REG_BASE + (0x24 << 2))

#define PWM_REG24_PWM_I_4_ALLOCATE_POS (0)
#define PWM_REG24_PWM_I_4_ALLOCATE_MASK (0xffffffff)

#define PWM_REG25_ADDR (SOC_PWM_REG_BASE + (0x25 << 2))

#define PWM_REG25_PWM_I_5_ALLOCATE_POS (0)
#define PWM_REG25_PWM_I_5_ALLOCATE_MASK (0xffffffff)

#define PWM_REG26_ADDR (SOC_PWM_REG_BASE + (0x26 << 2))

#define PWM_REG26_PWM_I_6_ALLOCATE_POS (0)
#define PWM_REG26_PWM_I_6_ALLOCATE_MASK (0xffffffff)

#define PWM_REG27_ADDR (SOC_PWM_REG_BASE + (0x27 << 2))

#define PWM_REG27_PWM_I_7_ALLOCATE_POS (0)
#define PWM_REG27_PWM_I_7_ALLOCATE_MASK (0xffffffff)

#define PWM_REG28_ADDR (SOC_PWM_REG_BASE + (0x28 << 2))

#define PWM_REG28_PWM_I_8_ALLOCATE_POS (0)
#define PWM_REG28_PWM_I_8_ALLOCATE_MASK (0xffffffff)

#define PWM_REG29_ADDR (SOC_PWM_REG_BASE + (0x29 << 2))

#define PWM_REG29_PWM_I_9_ALLOCATE_POS (0)
#define PWM_REG29_PWM_I_9_ALLOCATE_MASK (0xffffffff)

#define PWM_REG2A_ADDR (SOC_PWM_REG_BASE + (0x2a << 2))

#define PWM_REG2A_PWM_I_10_ALLOCATE_POS (0)
#define PWM_REG2A_PWM_I_10_ALLOCATE_MASK (0xffffffff)

#define PWM_REG2B_ADDR (SOC_PWM_REG_BASE + (0x2b << 2))

#define PWM_REG2B_PWM_I_11_ALLOCATE_POS (0)
#define PWM_REG2B_PWM_I_11_ALLOCATE_MASK (0xffffffff)

#define PWM_REG2C_ADDR (SOC_PWM_REG_BASE + (0x2c << 2))

#define PWM_REG2C_PWM_I_12_ALLOCATE_POS (0)
#define PWM_REG2C_PWM_I_12_ALLOCATE_MASK (0xffffffff)

#define PWM_REG2D_ADDR (SOC_PWM_REG_BASE + (0x2d << 2))

#define PWM_REG2D_PWM_I_13_ALLOCATE_POS (0)
#define PWM_REG2D_PWM_I_13_ALLOCATE_MASK (0xffffffff)

#define PWM_REG2E_ADDR (SOC_PWM_REG_BASE + (0x2e << 2))

#define PWM_REG2E_PWM_I_14_ALLOCATE_POS (0)
#define PWM_REG2E_PWM_I_14_ALLOCATE_MASK (0xffffffff)

#define PWM_REG2F_ADDR (SOC_PWM_REG_BASE + (0x2f << 2))

#define PWM_REG2F_PWM_I_15_ALLOCATE_POS (0)
#define PWM_REG2F_PWM_I_15_ALLOCATE_MASK (0xffffffff)

#define PWM_REG30_ADDR (SOC_PWM_REG_BASE + (0x30 << 2))

#define PWM_REG30_PWM_I_16_ALLOCATE_POS (0)
#define PWM_REG30_PWM_I_16_ALLOCATE_MASK (0xffffffff)

#define PWM_REG31_ADDR (SOC_PWM_REG_BASE + (0x31 << 2))

#define PWM_REG31_PWM_I_17_ALLOCATE_POS (0)
#define PWM_REG31_PWM_I_17_ALLOCATE_MASK (0xffffffff)

#define PWM_REG32_ADDR (SOC_PWM_REG_BASE + (0x32 << 2))

#define PWM_REG32_PWM_I_18_ALLOCATE_POS (0)
#define PWM_REG32_PWM_I_18_ALLOCATE_MASK (0xffffffff)

#define PWM_REG33_ADDR (SOC_PWM_REG_BASE + (0x33 << 2))

#define PWM_REG33_PWM_I_19_ALLOCATE_POS (0)
#define PWM_REG33_PWM_I_19_ALLOCATE_MASK (0xffffffff)

#define PWM_REG34_ADDR (SOC_PWM_REG_BASE + (0x34 << 2))

#define PWM_REG34_PWM_I_20_ALLOCATE_POS (0)
#define PWM_REG34_PWM_I_20_ALLOCATE_MASK (0xffffffff)

#define PWM_REG35_ADDR (SOC_PWM_REG_BASE + (0x35 << 2))

#define PWM_REG35_PWM_I_21_ALLOCATE_POS (0)
#define PWM_REG35_PWM_I_21_ALLOCATE_MASK (0xffffffff)

#define PWM_REG36_ADDR (SOC_PWM_REG_BASE + (0x36 << 2))

#define PWM_REG36_PWM_I_22_ALLOCATE_POS (0)
#define PWM_REG36_PWM_I_22_ALLOCATE_MASK (0xffffffff)

#define PWM_REG37_ADDR (SOC_PWM_REG_BASE + (0x37 << 2))

#define PWM_REG37_PWM_I_23_ALLOCATE_POS (0)
#define PWM_REG37_PWM_I_23_ALLOCATE_MASK (0xffffffff)

#define PWM_REG40_ADDR (SOC_PWM_REG_BASE + (0x40 << 2))

#define PWM_REG40_OCM3PE_POS (0)
#define PWM_REG40_OCM3PE_MASK (0x1)

#define PWM_REG40_OCM2PE_POS (1)
#define PWM_REG40_OCM2PE_MASK (0x1)

#define PWM_REG40_OCM1PE_POS (2)
#define PWM_REG40_OCM1PE_MASK (0x1)

#define PWM_REG40_ARPE3_POS (3)
#define PWM_REG40_ARPE3_MASK (0x1)

#define PWM_REG40_ARPE2_POS (4)
#define PWM_REG40_ARPE2_MASK (0x1)

#define PWM_REG40_ARPE1_POS (5)
#define PWM_REG40_ARPE1_MASK (0x1)

#define PWM_REG40_OC3PE_POS (6)
#define PWM_REG40_OC3PE_MASK (0x1)

#define PWM_REG40_OC2PE_POS (7)
#define PWM_REG40_OC2PE_MASK (0x1)

#define PWM_REG40_OC1PE_POS (8)
#define PWM_REG40_OC1PE_MASK (0x1)

#define PWM_REG40_RESERVED_9_17_POS (9)
#define PWM_REG40_RESERVED_9_17_MASK (0x1ff)

#define PWM_REG40_CMS_TIM3_POS (18)
#define PWM_REG40_CMS_TIM3_MASK (0x3)

#define PWM_REG40_CMS_TIM2_POS (20)
#define PWM_REG40_CMS_TIM2_MASK (0x3)

#define PWM_REG40_CMS_TIM1_POS (22)
#define PWM_REG40_CMS_TIM1_MASK (0x3)

#define PWM_REG40_RESERVED_24_25_POS (24)
#define PWM_REG40_RESERVED_24_25_MASK (0x3)

#define PWM_REG40_URS3_POS (26)
#define PWM_REG40_URS3_MASK (0x1)

#define PWM_REG40_URS2_POS (27)
#define PWM_REG40_URS2_MASK (0x1)

#define PWM_REG40_URS1_POS (28)
#define PWM_REG40_URS1_MASK (0x1)

#define PWM_REG40_DIR_TIM3_POS (29)
#define PWM_REG40_DIR_TIM3_MASK (0x1)

#define PWM_REG40_DIR_TIM2_POS (30)
#define PWM_REG40_DIR_TIM2_MASK (0x1)

#define PWM_REG40_DIR_TIM1_POS (31)
#define PWM_REG40_DIR_TIM1_MASK (0x1)

#define PWM_REG41_ADDR (SOC_PWM_REG_BASE + (0x41 << 2))

#define PWM_REG41_RESERVED_0_7_POS (0)
#define PWM_REG41_RESERVED_0_7_MASK (0xff)

#define PWM_REG41_COM_MODE_POS (8)
#define PWM_REG41_COM_MODE_MASK (0x1)

#define PWM_REG41_CCPC_POS (9)
#define PWM_REG41_CCPC_MASK (0x1)

#define PWM_REG41_TRIOS3_POS (10)
#define PWM_REG41_TRIOS3_MASK (0x7)

#define PWM_REG41_ADCS3_POS (13)
#define PWM_REG41_ADCS3_MASK (0x7)

#define PWM_REG41_RESERVED_16_16_POS (16)
#define PWM_REG41_RESERVED_16_16_MASK (0x1)

#define PWM_REG41_CCUS_POS (17)
#define PWM_REG41_CCUS_MASK (0x1)

#define PWM_REG41_TRIOS2_POS (18)
#define PWM_REG41_TRIOS2_MASK (0x7)

#define PWM_REG41_ADCS2_POS (21)
#define PWM_REG41_ADCS2_MASK (0x7)

#define PWM_REG41_RESERVED_24_25_POS (24)
#define PWM_REG41_RESERVED_24_25_MASK (0x3)

#define PWM_REG41_TRIOS1_POS (26)
#define PWM_REG41_TRIOS1_MASK (0x7)

#define PWM_REG41_ADCS1_POS (29)
#define PWM_REG41_ADCS1_MASK (0x7)

#define PWM_REG42_ADDR (SOC_PWM_REG_BASE + (0x42 << 2))

#define PWM_REG42_SMS1_POS (0)
#define PWM_REG42_SMS1_MASK (0xf)

#define PWM_REG42_SMS2_POS (4)
#define PWM_REG42_SMS2_MASK (0xf)

#define PWM_REG42_SMS3_POS (8)
#define PWM_REG42_SMS3_MASK (0xf)

#define PWM_REG42_TS1_POS (12)
#define PWM_REG42_TS1_MASK (0x7)

#define PWM_REG42_RESERVED_15_15_POS (15)
#define PWM_REG42_RESERVED_15_15_MASK (0x1)

#define PWM_REG42_TS2_POS (16)
#define PWM_REG42_TS2_MASK (0x7)

#define PWM_REG42_TS3_POS (19)
#define PWM_REG42_TS3_MASK (0x7)

#define PWM_REG42_RESERVED_22_31_POS (22)
#define PWM_REG42_RESERVED_22_31_MASK (0x3ff)

#define PWM_REG43_ADDR (SOC_PWM_REG_BASE + (0x43 << 2))

#define PWM_REG43_CC1IE_POS (0)
#define PWM_REG43_CC1IE_MASK (0x1)

#define PWM_REG43_CC2IE_POS (1)
#define PWM_REG43_CC2IE_MASK (0x1)

#define PWM_REG43_CC3IE_POS (2)
#define PWM_REG43_CC3IE_MASK (0x1)

#define PWM_REG43_CC4IE_POS (3)
#define PWM_REG43_CC4IE_MASK (0x1)

#define PWM_REG43_CC5IE_POS (4)
#define PWM_REG43_CC5IE_MASK (0x1)

#define PWM_REG43_CC6IE_POS (5)
#define PWM_REG43_CC6IE_MASK (0x1)

#define PWM_REG43_CC7IE_POS (6)
#define PWM_REG43_CC7IE_MASK (0x1)

#define PWM_REG43_CC8IE_POS (7)
#define PWM_REG43_CC8IE_MASK (0x1)

#define PWM_REG43_CC9IE_POS (8)
#define PWM_REG43_CC9IE_MASK (0x1)

#define PWM_REG43_UIE1_POS (9)
#define PWM_REG43_UIE1_MASK (0x1)

#define PWM_REG43_UIE2_POS (10)
#define PWM_REG43_UIE2_MASK (0x1)

#define PWM_REG43_UIE3_POS (11)
#define PWM_REG43_UIE3_MASK (0x1)

#define PWM_REG43_TIE1_POS (12)
#define PWM_REG43_TIE1_MASK (0x1)

#define PWM_REG43_TIE2_POS (13)
#define PWM_REG43_TIE2_MASK (0x1)

#define PWM_REG43_TIE3_POS (14)
#define PWM_REG43_TIE3_MASK (0x1)

#define PWM_REG43_COMIE_POS (15)
#define PWM_REG43_COMIE_MASK (0x1)

#define PWM_REG43_RESERVED_16_19_POS (16)
#define PWM_REG43_RESERVED_16_19_MASK (0xf)

#define PWM_REG43_DF_TIM1_POS (20)
#define PWM_REG43_DF_TIM1_MASK (0xf)

#define PWM_REG43_DF_TIM2_POS (24)
#define PWM_REG43_DF_TIM2_MASK (0xf)

#define PWM_REG43_DF_TIM3_POS (28)
#define PWM_REG43_DF_TIM3_MASK (0xf)

#define PWM_REG44_ADDR (SOC_PWM_REG_BASE + (0x44 << 2))

#define PWM_REG44_CC1IF_POS (0)
#define PWM_REG44_CC1IF_MASK (0x1)

#define PWM_REG44_CC2IF_POS (1)
#define PWM_REG44_CC2IF_MASK (0x1)

#define PWM_REG44_CC3IF_POS (2)
#define PWM_REG44_CC3IF_MASK (0x1)

#define PWM_REG44_CC4IF_POS (3)
#define PWM_REG44_CC4IF_MASK (0x1)

#define PWM_REG44_CC5IF_POS (4)
#define PWM_REG44_CC5IF_MASK (0x1)

#define PWM_REG44_CC6IF_POS (5)
#define PWM_REG44_CC6IF_MASK (0x1)

#define PWM_REG44_CC7IF_POS (6)
#define PWM_REG44_CC7IF_MASK (0x1)

#define PWM_REG44_CC8IF_POS (7)
#define PWM_REG44_CC8IF_MASK (0x1)

#define PWM_REG44_CC9IF_POS (8)
#define PWM_REG44_CC9IF_MASK (0x1)

#define PWM_REG44_UIF1_POS (9)
#define PWM_REG44_UIF1_MASK (0x1)

#define PWM_REG44_UIF2_POS (10)
#define PWM_REG44_UIF2_MASK (0x1)

#define PWM_REG44_UIF3_POS (11)
#define PWM_REG44_UIF3_MASK (0x1)

#define PWM_REG44_TIF1_POS (12)
#define PWM_REG44_TIF1_MASK (0x1)

#define PWM_REG44_TIF2_POS (13)
#define PWM_REG44_TIF2_MASK (0x1)

#define PWM_REG44_TIF3_POS (14)
#define PWM_REG44_TIF3_MASK (0x1)

#define PWM_REG44_COMIF_POS (15)
#define PWM_REG44_COMIF_MASK (0x1)

#define PWM_REG44_RESERVED_16_31_POS (16)
#define PWM_REG44_RESERVED_16_31_MASK (0xffff)

#define PWM_REG45_ADDR (SOC_PWM_REG_BASE + (0x45 << 2))

#define PWM_REG45_RESERVED_0_8_POS (0)
#define PWM_REG45_RESERVED_0_8_MASK (0x1ff)

#define PWM_REG45_UG1_POS (9)
#define PWM_REG45_UG1_MASK (0x1)

#define PWM_REG45_UG2_POS (10)
#define PWM_REG45_UG2_MASK (0x1)

#define PWM_REG45_UG3_POS (11)
#define PWM_REG45_UG3_MASK (0x1)

#define PWM_REG45_TG1_POS (12)
#define PWM_REG45_TG1_MASK (0x1)

#define PWM_REG45_TG2_POS (13)
#define PWM_REG45_TG2_MASK (0x1)

#define PWM_REG45_TG3_POS (14)
#define PWM_REG45_TG3_MASK (0x1)

#define PWM_REG45_COMG_POS (15)
#define PWM_REG45_COMG_MASK (0x1)

#define PWM_REG45_DTM1_POS (16)
#define PWM_REG45_DTM1_MASK (0x3)

#define PWM_REG45_DTM2_POS (18)
#define PWM_REG45_DTM2_MASK (0x3)

#define PWM_REG45_DTM3_POS (20)
#define PWM_REG45_DTM3_MASK (0x3)

#define PWM_REG45_INIT_LEVEL1_POS (22)
#define PWM_REG45_INIT_LEVEL1_MASK (0x1)

#define PWM_REG45_INIT_LEVEL2_POS (23)
#define PWM_REG45_INIT_LEVEL2_MASK (0x1)

#define PWM_REG45_INIT_LEVEL3_POS (24)
#define PWM_REG45_INIT_LEVEL3_MASK (0x1)

#define PWM_REG45_RESERVED_25_31_POS (25)
#define PWM_REG45_RESERVED_25_31_MASK (0x7f)

#define PWM_REG46_ADDR (SOC_PWM_REG_BASE + (0x46 << 2))

#define PWM_REG46_CH1P_POS (0)
#define PWM_REG46_CH1P_MASK (0x3)

#define PWM_REG46_CH2P_POS (2)
#define PWM_REG46_CH2P_MASK (0x3)

#define PWM_REG46_CH3P_POS (4)
#define PWM_REG46_CH3P_MASK (0x3)

#define PWM_REG46_CH4P_POS (6)
#define PWM_REG46_CH4P_MASK (0x3)

#define PWM_REG46_CH5P_POS (8)
#define PWM_REG46_CH5P_MASK (0x3)

#define PWM_REG46_CH6P_POS (10)
#define PWM_REG46_CH6P_MASK (0x3)

#define PWM_REG46_CH1E_POS (12)
#define PWM_REG46_CH1E_MASK (0x1)

#define PWM_REG46_CH2E_POS (13)
#define PWM_REG46_CH2E_MASK (0x1)

#define PWM_REG46_CH3E_POS (14)
#define PWM_REG46_CH3E_MASK (0x1)

#define PWM_REG46_CH4E_POS (15)
#define PWM_REG46_CH4E_MASK (0x1)

#define PWM_REG46_CH5E_POS (16)
#define PWM_REG46_CH5E_MASK (0x1)

#define PWM_REG46_CH6E_POS (17)
#define PWM_REG46_CH6E_MASK (0x1)

#define PWM_REG46_TIM1CCM_POS (18)
#define PWM_REG46_TIM1CCM_MASK (0x1)

#define PWM_REG46_TIM2CCM_POS (19)
#define PWM_REG46_TIM2CCM_MASK (0x1)

#define PWM_REG46_TIM3CCM_POS (20)
#define PWM_REG46_TIM3CCM_MASK (0x1)

#define PWM_REG46_OC1M_POS (21)
#define PWM_REG46_OC1M_MASK (0x7)

#define PWM_REG46_OC2M_POS (24)
#define PWM_REG46_OC2M_MASK (0x7)

#define PWM_REG46_OC3M_POS (27)
#define PWM_REG46_OC3M_MASK (0x7)

#define PWM_REG46_RESERVED_30_31_POS (30)
#define PWM_REG46_RESERVED_30_31_MASK (0x3)

#define PWM_REG47_ADDR (SOC_PWM_REG_BASE + (0x47 << 2))

#define PWM_REG47_TIM1_CNT_POS (0)
#define PWM_REG47_TIM1_CNT_MASK (0xffffffff)

#define PWM_REG48_ADDR (SOC_PWM_REG_BASE + (0x48 << 2))

#define PWM_REG48_TIM2_CNT_POS (0)
#define PWM_REG48_TIM2_CNT_MASK (0xffffffff)

#define PWM_REG49_ADDR (SOC_PWM_REG_BASE + (0x49 << 2))

#define PWM_REG49_TIM3_CNT_POS (0)
#define PWM_REG49_TIM3_CNT_MASK (0xffffffff)

#define PWM_REG4A_ADDR (SOC_PWM_REG_BASE + (0x4a << 2))

#define PWM_REG4A_PSC1_POS (0)
#define PWM_REG4A_PSC1_MASK (0xff)

#define PWM_REG4A_PSC2_POS (8)
#define PWM_REG4A_PSC2_MASK (0xff)

#define PWM_REG4A_PSC3_POS (16)
#define PWM_REG4A_PSC3_MASK (0xff)

#define PWM_REG4A_RESERVED_24_31_POS (24)
#define PWM_REG4A_RESERVED_24_31_MASK (0xff)

#define PWM_REG4B_ADDR (SOC_PWM_REG_BASE + (0x4b << 2))

#define PWM_REG4B_TIM1_ARR_POS (0)
#define PWM_REG4B_TIM1_ARR_MASK (0xffffffff)

#define PWM_REG4C_ADDR (SOC_PWM_REG_BASE + (0x4c << 2))

#define PWM_REG4C_TIM2_ARR_POS (0)
#define PWM_REG4C_TIM2_ARR_MASK (0xffffffff)

#define PWM_REG4D_ADDR (SOC_PWM_REG_BASE + (0x4d << 2))

#define PWM_REG4D_TIM3_ARR_POS (0)
#define PWM_REG4D_TIM3_ARR_MASK (0xffffffff)

#define PWM_REG4E_ADDR (SOC_PWM_REG_BASE + (0x4e << 2))

#define PWM_REG4E_TIM1_RCR_POS (0)
#define PWM_REG4E_TIM1_RCR_MASK (0xffffffff)

#define PWM_REG4F_ADDR (SOC_PWM_REG_BASE + (0x4f << 2))

#define PWM_REG4F_TIM2_RCR_POS (0)
#define PWM_REG4F_TIM2_RCR_MASK (0xffffffff)

#define PWM_REG50_ADDR (SOC_PWM_REG_BASE + (0x50 << 2))

#define PWM_REG50_TIM3_RCR_POS (0)
#define PWM_REG50_TIM3_RCR_MASK (0xffffffff)

#define PWM_REG51_ADDR (SOC_PWM_REG_BASE + (0x51 << 2))

#define PWM_REG51_CCR1_POS (0)
#define PWM_REG51_CCR1_MASK (0xffffffff)

#define PWM_REG52_ADDR (SOC_PWM_REG_BASE + (0x52 << 2))

#define PWM_REG52_CCR2_POS (0)
#define PWM_REG52_CCR2_MASK (0xffffffff)

#define PWM_REG53_ADDR (SOC_PWM_REG_BASE + (0x53 << 2))

#define PWM_REG53_CCR3_POS (0)
#define PWM_REG53_CCR3_MASK (0xffffffff)

#define PWM_REG54_ADDR (SOC_PWM_REG_BASE + (0x54 << 2))

#define PWM_REG54_CCR4_POS (0)
#define PWM_REG54_CCR4_MASK (0xffffffff)

#define PWM_REG55_ADDR (SOC_PWM_REG_BASE + (0x55 << 2))

#define PWM_REG55_CCR5_POS (0)
#define PWM_REG55_CCR5_MASK (0xffffffff)

#define PWM_REG56_ADDR (SOC_PWM_REG_BASE + (0x56 << 2))

#define PWM_REG56_CCR6_POS (0)
#define PWM_REG56_CCR6_MASK (0xffffffff)

#define PWM_REG57_ADDR (SOC_PWM_REG_BASE + (0x57 << 2))

#define PWM_REG57_CCR7_POS (0)
#define PWM_REG57_CCR7_MASK (0xffffffff)

#define PWM_REG58_ADDR (SOC_PWM_REG_BASE + (0x58 << 2))

#define PWM_REG58_CCR8_POS (0)
#define PWM_REG58_CCR8_MASK (0xffffffff)

#define PWM_REG59_ADDR (SOC_PWM_REG_BASE + (0x59 << 2))

#define PWM_REG59_CCR9_POS (0)
#define PWM_REG59_CCR9_MASK (0xffffffff)

#define PWM_REG5A_ADDR (SOC_PWM_REG_BASE + (0x5a << 2))

#define PWM_REG5A_DT1_POS (0)
#define PWM_REG5A_DT1_MASK (0x3ff)

#define PWM_REG5A_DT2_POS (10)
#define PWM_REG5A_DT2_MASK (0x3ff)

#define PWM_REG5A_DT3_POS (20)
#define PWM_REG5A_DT3_MASK (0x3ff)

#define PWM_REG5A_RESERVED_30_31_POS (30)
#define PWM_REG5A_RESERVED_30_31_MASK (0x3)

#define PWM_REG5B_ADDR (SOC_PWM_REG_BASE + (0x5b << 2))

#define PWM_REG5B_TIM1_ARR_SHAD_POS (0)
#define PWM_REG5B_TIM1_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG5C_ADDR (SOC_PWM_REG_BASE + (0x5c << 2))

#define PWM_REG5C_TIM2_ARR_SHAD_POS (0)
#define PWM_REG5C_TIM2_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG5D_ADDR (SOC_PWM_REG_BASE + (0x5d << 2))

#define PWM_REG5D_TIM3_ARR_SHAD_POS (0)
#define PWM_REG5D_TIM3_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG5E_ADDR (SOC_PWM_REG_BASE + (0x5e << 2))

#define PWM_REG5E_TIM1_RCR_SHAD_POS (0)
#define PWM_REG5E_TIM1_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG5F_ADDR (SOC_PWM_REG_BASE + (0x5f << 2))

#define PWM_REG5F_TIM2_RCR_SHAD_POS (0)
#define PWM_REG5F_TIM2_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG60_ADDR (SOC_PWM_REG_BASE + (0x60 << 2))

#define PWM_REG60_TIM3_RCR_SHAD_POS (0)
#define PWM_REG60_TIM3_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG61_ADDR (SOC_PWM_REG_BASE + (0x61 << 2))

#define PWM_REG61_CCR1_SHAD_POS (0)
#define PWM_REG61_CCR1_SHAD_MASK (0xffffffff)

#define PWM_REG62_ADDR (SOC_PWM_REG_BASE + (0x62 << 2))

#define PWM_REG62_CCR2_SHAD_POS (0)
#define PWM_REG62_CCR2_SHAD_MASK (0xffffffff)

#define PWM_REG63_ADDR (SOC_PWM_REG_BASE + (0x63 << 2))

#define PWM_REG63_CCR3_SHAD_POS (0)
#define PWM_REG63_CCR3_SHAD_MASK (0xffffffff)

#define PWM_REG64_ADDR (SOC_PWM_REG_BASE + (0x64 << 2))

#define PWM_REG64_CCR4_SHAD_POS (0)
#define PWM_REG64_CCR4_SHAD_MASK (0xffffffff)

#define PWM_REG65_ADDR (SOC_PWM_REG_BASE + (0x65 << 2))

#define PWM_REG65_CCR5_SHAD_POS (0)
#define PWM_REG65_CCR5_SHAD_MASK (0xffffffff)

#define PWM_REG66_ADDR (SOC_PWM_REG_BASE + (0x66 << 2))

#define PWM_REG66_CCR6_SHAD_POS (0)
#define PWM_REG66_CCR6_SHAD_MASK (0xffffffff)

#define PWM_REG67_ADDR (SOC_PWM_REG_BASE + (0x67 << 2))

#define PWM_REG67_CCR7_SHAD_POS (0)
#define PWM_REG67_CCR7_SHAD_MASK (0xffffffff)

#define PWM_REG68_ADDR (SOC_PWM_REG_BASE + (0x68 << 2))

#define PWM_REG68_CCR8_SHAD_POS (0)
#define PWM_REG68_CCR8_SHAD_MASK (0xffffffff)

#define PWM_REG69_ADDR (SOC_PWM_REG_BASE + (0x69 << 2))

#define PWM_REG69_CCR9_SHAD_POS (0)
#define PWM_REG69_CCR9_SHAD_MASK (0xffffffff)

#define PWM_REG6A_ADDR (SOC_PWM_REG_BASE + (0x6a << 2))

#define PWM_REG6A_CH6E_SHAD_POS (0)
#define PWM_REG6A_CH6E_SHAD_MASK (0x1)

#define PWM_REG6A_CH5E_SHAD_POS (1)
#define PWM_REG6A_CH5E_SHAD_MASK (0x1)

#define PWM_REG6A_CH4E_SHAD_POS (2)
#define PWM_REG6A_CH4E_SHAD_MASK (0x1)

#define PWM_REG6A_CH3E_SHAD_POS (3)
#define PWM_REG6A_CH3E_SHAD_MASK (0x1)

#define PWM_REG6A_CH2E_SHAD_POS (4)
#define PWM_REG6A_CH2E_SHAD_MASK (0x1)

#define PWM_REG6A_CH1E_SHAD_POS (5)
#define PWM_REG6A_CH1E_SHAD_MASK (0x1)

#define PWM_REG6A_OC1M_SHAD_POS (6)
#define PWM_REG6A_OC1M_SHAD_MASK (0x7)

#define PWM_REG6A_OC2M_SHAD_POS (9)
#define PWM_REG6A_OC2M_SHAD_MASK (0x7)

#define PWM_REG6A_OC3M_SHAD_POS (12)
#define PWM_REG6A_OC3M_SHAD_MASK (0x7)

#define PWM_REG6A_RESERVED_15_31_POS (15)
#define PWM_REG6A_RESERVED_15_31_MASK (0x1ffff)

#define PWM_REG80_ADDR (SOC_PWM_REG_BASE + (0x80 << 2))

#define PWM_REG80_OCM3PE_POS (0)
#define PWM_REG80_OCM3PE_MASK (0x1)

#define PWM_REG80_OCM2PE_POS (1)
#define PWM_REG80_OCM2PE_MASK (0x1)

#define PWM_REG80_OCM1PE_POS (2)
#define PWM_REG80_OCM1PE_MASK (0x1)

#define PWM_REG80_ARPE3_POS (3)
#define PWM_REG80_ARPE3_MASK (0x1)

#define PWM_REG80_ARPE2_POS (4)
#define PWM_REG80_ARPE2_MASK (0x1)

#define PWM_REG80_ARPE1_POS (5)
#define PWM_REG80_ARPE1_MASK (0x1)

#define PWM_REG80_OC3PE_POS (6)
#define PWM_REG80_OC3PE_MASK (0x1)

#define PWM_REG80_OC2PE_POS (7)
#define PWM_REG80_OC2PE_MASK (0x1)

#define PWM_REG80_OC1PE_POS (8)
#define PWM_REG80_OC1PE_MASK (0x1)

#define PWM_REG80_RESERVED_9_17_POS (9)
#define PWM_REG80_RESERVED_9_17_MASK (0x1ff)

#define PWM_REG80_CMS_TIM3_POS (18)
#define PWM_REG80_CMS_TIM3_MASK (0x3)

#define PWM_REG80_CMS_TIM2_POS (20)
#define PWM_REG80_CMS_TIM2_MASK (0x3)

#define PWM_REG80_CMS_TIM1_POS (22)
#define PWM_REG80_CMS_TIM1_MASK (0x3)

#define PWM_REG80_RESERVED_24_25_POS (24)
#define PWM_REG80_RESERVED_24_25_MASK (0x3)

#define PWM_REG80_URS3_POS (26)
#define PWM_REG80_URS3_MASK (0x1)

#define PWM_REG80_URS2_POS (27)
#define PWM_REG80_URS2_MASK (0x1)

#define PWM_REG80_URS1_POS (28)
#define PWM_REG80_URS1_MASK (0x1)

#define PWM_REG80_DIR_TIM3_POS (29)
#define PWM_REG80_DIR_TIM3_MASK (0x1)

#define PWM_REG80_DIR_TIM2_POS (30)
#define PWM_REG80_DIR_TIM2_MASK (0x1)

#define PWM_REG80_DIR_TIM1_POS (31)
#define PWM_REG80_DIR_TIM1_MASK (0x1)

#define PWM_REG81_ADDR (SOC_PWM_REG_BASE + (0x81 << 2))

#define PWM_REG81_RESERVED_0_7_POS (0)
#define PWM_REG81_RESERVED_0_7_MASK (0xff)

#define PWM_REG81_COM_MODE_POS (8)
#define PWM_REG81_COM_MODE_MASK (0x1)

#define PWM_REG81_CCPC_POS (9)
#define PWM_REG81_CCPC_MASK (0x1)

#define PWM_REG81_TRIOS3_POS (10)
#define PWM_REG81_TRIOS3_MASK (0x7)

#define PWM_REG81_ADCS3_POS (13)
#define PWM_REG81_ADCS3_MASK (0x7)

#define PWM_REG81_RESERVED_16_16_POS (16)
#define PWM_REG81_RESERVED_16_16_MASK (0x1)

#define PWM_REG81_CCUS_POS (17)
#define PWM_REG81_CCUS_MASK (0x1)

#define PWM_REG81_TRIOS2_POS (18)
#define PWM_REG81_TRIOS2_MASK (0x7)

#define PWM_REG81_ADCS2_POS (21)
#define PWM_REG81_ADCS2_MASK (0x7)

#define PWM_REG81_RESERVED_24_25_POS (24)
#define PWM_REG81_RESERVED_24_25_MASK (0x3)

#define PWM_REG81_TRIOS1_POS (26)
#define PWM_REG81_TRIOS1_MASK (0x7)

#define PWM_REG81_ADCS1_POS (29)
#define PWM_REG81_ADCS1_MASK (0x7)

#define PWM_REG82_ADDR (SOC_PWM_REG_BASE + (0x82 << 2))

#define PWM_REG82_SMS1_POS (0)
#define PWM_REG82_SMS1_MASK (0xf)

#define PWM_REG82_SMS2_POS (4)
#define PWM_REG82_SMS2_MASK (0xf)

#define PWM_REG82_SMS3_POS (8)
#define PWM_REG82_SMS3_MASK (0xf)

#define PWM_REG82_TS1_POS (12)
#define PWM_REG82_TS1_MASK (0x7)

#define PWM_REG82_RESERVED_15_15_POS (15)
#define PWM_REG82_RESERVED_15_15_MASK (0x1)

#define PWM_REG82_TS2_POS (16)
#define PWM_REG82_TS2_MASK (0x7)

#define PWM_REG82_TS3_POS (19)
#define PWM_REG82_TS3_MASK (0x7)

#define PWM_REG82_RESERVED_22_31_POS (22)
#define PWM_REG82_RESERVED_22_31_MASK (0x3ff)

#define PWM_REG83_ADDR (SOC_PWM_REG_BASE + (0x83 << 2))

#define PWM_REG83_CC1IE_POS (0)
#define PWM_REG83_CC1IE_MASK (0x1)

#define PWM_REG83_CC2IE_POS (1)
#define PWM_REG83_CC2IE_MASK (0x1)

#define PWM_REG83_CC3IE_POS (2)
#define PWM_REG83_CC3IE_MASK (0x1)

#define PWM_REG83_CC4IE_POS (3)
#define PWM_REG83_CC4IE_MASK (0x1)

#define PWM_REG83_CC5IE_POS (4)
#define PWM_REG83_CC5IE_MASK (0x1)

#define PWM_REG83_CC6IE_POS (5)
#define PWM_REG83_CC6IE_MASK (0x1)

#define PWM_REG83_CC7IE_POS (6)
#define PWM_REG83_CC7IE_MASK (0x1)

#define PWM_REG83_CC8IE_POS (7)
#define PWM_REG83_CC8IE_MASK (0x1)

#define PWM_REG83_CC9IE_POS (8)
#define PWM_REG83_CC9IE_MASK (0x1)

#define PWM_REG83_UIE1_POS (9)
#define PWM_REG83_UIE1_MASK (0x1)

#define PWM_REG83_UIE2_POS (10)
#define PWM_REG83_UIE2_MASK (0x1)

#define PWM_REG83_UIE3_POS (11)
#define PWM_REG83_UIE3_MASK (0x1)

#define PWM_REG83_TIE1_POS (12)
#define PWM_REG83_TIE1_MASK (0x1)

#define PWM_REG83_TIE2_POS (13)
#define PWM_REG83_TIE2_MASK (0x1)

#define PWM_REG83_TIE3_POS (14)
#define PWM_REG83_TIE3_MASK (0x1)

#define PWM_REG83_COMIE_POS (15)
#define PWM_REG83_COMIE_MASK (0x1)

#define PWM_REG83_RESERVED_16_19_POS (16)
#define PWM_REG83_RESERVED_16_19_MASK (0xf)

#define PWM_REG83_DF_TIM1_POS (20)
#define PWM_REG83_DF_TIM1_MASK (0xf)

#define PWM_REG83_DF_TIM2_POS (24)
#define PWM_REG83_DF_TIM2_MASK (0xf)

#define PWM_REG83_DF_TIM3_POS (28)
#define PWM_REG83_DF_TIM3_MASK (0xf)

#define PWM_REG84_ADDR (SOC_PWM_REG_BASE + (0x84 << 2))

#define PWM_REG84_CC1IF_POS (0)
#define PWM_REG84_CC1IF_MASK (0x1)

#define PWM_REG84_CC2IF_POS (1)
#define PWM_REG84_CC2IF_MASK (0x1)

#define PWM_REG84_CC3IF_POS (2)
#define PWM_REG84_CC3IF_MASK (0x1)

#define PWM_REG84_CC4IF_POS (3)
#define PWM_REG84_CC4IF_MASK (0x1)

#define PWM_REG84_CC5IF_POS (4)
#define PWM_REG84_CC5IF_MASK (0x1)

#define PWM_REG84_CC6IF_POS (5)
#define PWM_REG84_CC6IF_MASK (0x1)

#define PWM_REG84_CC7IF_POS (6)
#define PWM_REG84_CC7IF_MASK (0x1)

#define PWM_REG84_CC8IF_POS (7)
#define PWM_REG84_CC8IF_MASK (0x1)

#define PWM_REG84_CC9IF_POS (8)
#define PWM_REG84_CC9IF_MASK (0x1)

#define PWM_REG84_UIF1_POS (9)
#define PWM_REG84_UIF1_MASK (0x1)

#define PWM_REG84_UIF2_POS (10)
#define PWM_REG84_UIF2_MASK (0x1)

#define PWM_REG84_UIF3_POS (11)
#define PWM_REG84_UIF3_MASK (0x1)

#define PWM_REG84_TIF1_POS (12)
#define PWM_REG84_TIF1_MASK (0x1)

#define PWM_REG84_TIF2_POS (13)
#define PWM_REG84_TIF2_MASK (0x1)

#define PWM_REG84_TIF3_POS (14)
#define PWM_REG84_TIF3_MASK (0x1)

#define PWM_REG84_COMIF_POS (15)
#define PWM_REG84_COMIF_MASK (0x1)

#define PWM_REG84_RESERVED_16_31_POS (16)
#define PWM_REG84_RESERVED_16_31_MASK (0xffff)

#define PWM_REG85_ADDR (SOC_PWM_REG_BASE + (0x85 << 2))

#define PWM_REG85_RESERVED_0_8_POS (0)
#define PWM_REG85_RESERVED_0_8_MASK (0x1ff)

#define PWM_REG85_UG1_POS (9)
#define PWM_REG85_UG1_MASK (0x1)

#define PWM_REG85_UG2_POS (10)
#define PWM_REG85_UG2_MASK (0x1)

#define PWM_REG85_UG3_POS (11)
#define PWM_REG85_UG3_MASK (0x1)

#define PWM_REG85_TG1_POS (12)
#define PWM_REG85_TG1_MASK (0x1)

#define PWM_REG85_TG2_POS (13)
#define PWM_REG85_TG2_MASK (0x1)

#define PWM_REG85_TG3_POS (14)
#define PWM_REG85_TG3_MASK (0x1)

#define PWM_REG85_COMG_POS (15)
#define PWM_REG85_COMG_MASK (0x1)

#define PWM_REG85_DTM1_POS (16)
#define PWM_REG85_DTM1_MASK (0x3)

#define PWM_REG85_DTM2_POS (18)
#define PWM_REG85_DTM2_MASK (0x3)

#define PWM_REG85_DTM3_POS (20)
#define PWM_REG85_DTM3_MASK (0x3)

#define PWM_REG85_RESERVED_22_31_POS (22)
#define PWM_REG85_RESERVED_22_31_MASK (0x3ff)

#define PWM_REG86_ADDR (SOC_PWM_REG_BASE + (0x86 << 2))

#define PWM_REG86_CH1P_POS (0)
#define PWM_REG86_CH1P_MASK (0x3)

#define PWM_REG86_CH2P_POS (2)
#define PWM_REG86_CH2P_MASK (0x3)

#define PWM_REG86_CH3P_POS (4)
#define PWM_REG86_CH3P_MASK (0x3)

#define PWM_REG86_CH4P_POS (6)
#define PWM_REG86_CH4P_MASK (0x3)

#define PWM_REG86_CH5P_POS (8)
#define PWM_REG86_CH5P_MASK (0x3)

#define PWM_REG86_CH6P_POS (10)
#define PWM_REG86_CH6P_MASK (0x3)

#define PWM_REG86_CH1E_POS (12)
#define PWM_REG86_CH1E_MASK (0x1)

#define PWM_REG86_CH2E_POS (13)
#define PWM_REG86_CH2E_MASK (0x1)

#define PWM_REG86_CH3E_POS (14)
#define PWM_REG86_CH3E_MASK (0x1)

#define PWM_REG86_CH4E_POS (15)
#define PWM_REG86_CH4E_MASK (0x1)

#define PWM_REG86_CH5E_POS (16)
#define PWM_REG86_CH5E_MASK (0x1)

#define PWM_REG86_CH6E_POS (17)
#define PWM_REG86_CH6E_MASK (0x1)

#define PWM_REG86_TIM1CCM_POS (18)
#define PWM_REG86_TIM1CCM_MASK (0x1)

#define PWM_REG86_TIM2CCM_POS (19)
#define PWM_REG86_TIM2CCM_MASK (0x1)

#define PWM_REG86_TIM3CCM_POS (20)
#define PWM_REG86_TIM3CCM_MASK (0x1)

#define PWM_REG86_OC1M_POS (21)
#define PWM_REG86_OC1M_MASK (0x7)

#define PWM_REG86_OC2M_POS (24)
#define PWM_REG86_OC2M_MASK (0x7)

#define PWM_REG86_OC3M_POS (27)
#define PWM_REG86_OC3M_MASK (0x7)

#define PWM_REG86_RESERVED_30_31_POS (30)
#define PWM_REG86_RESERVED_30_31_MASK (0x3)

#define PWM_REG87_ADDR (SOC_PWM_REG_BASE + (0x87 << 2))

#define PWM_REG87_TIM1_CNT_POS (0)
#define PWM_REG87_TIM1_CNT_MASK (0xffffffff)

#define PWM_REG88_ADDR (SOC_PWM_REG_BASE + (0x88 << 2))

#define PWM_REG88_TIM2_CNT_POS (0)
#define PWM_REG88_TIM2_CNT_MASK (0xffffffff)

#define PWM_REG89_ADDR (SOC_PWM_REG_BASE + (0x89 << 2))

#define PWM_REG89_TIM3_CNT_POS (0)
#define PWM_REG89_TIM3_CNT_MASK (0xffffffff)

#define PWM_REG8A_ADDR (SOC_PWM_REG_BASE + (0x8a << 2))

#define PWM_REG8A_PSC1_POS (0)
#define PWM_REG8A_PSC1_MASK (0xff)

#define PWM_REG8A_PSC2_POS (8)
#define PWM_REG8A_PSC2_MASK (0xff)

#define PWM_REG8A_PSC3_POS (16)
#define PWM_REG8A_PSC3_MASK (0xff)

#define PWM_REG8A_RESERVED_24_31_POS (24)
#define PWM_REG8A_RESERVED_24_31_MASK (0xff)

#define PWM_REG8B_ADDR (SOC_PWM_REG_BASE + (0x8b << 2))

#define PWM_REG8B_TIM1_ARR_POS (0)
#define PWM_REG8B_TIM1_ARR_MASK (0xffffffff)

#define PWM_REG8C_ADDR (SOC_PWM_REG_BASE + (0x8c << 2))

#define PWM_REG8C_TIM2_ARR_POS (0)
#define PWM_REG8C_TIM2_ARR_MASK (0xffffffff)

#define PWM_REG8D_ADDR (SOC_PWM_REG_BASE + (0x8d << 2))

#define PWM_REG8D_TIM3_ARR_POS (0)
#define PWM_REG8D_TIM3_ARR_MASK (0xffffffff)

#define PWM_REG8E_ADDR (SOC_PWM_REG_BASE + (0x8e << 2))

#define PWM_REG8E_TIM1_RCR_POS (0)
#define PWM_REG8E_TIM1_RCR_MASK (0xffffffff)

#define PWM_REG8F_ADDR (SOC_PWM_REG_BASE + (0x8f << 2))

#define PWM_REG8F_TIM2_RCR_POS (0)
#define PWM_REG8F_TIM2_RCR_MASK (0xffffffff)

#define PWM_REG90_ADDR (SOC_PWM_REG_BASE + (0x90 << 2))

#define PWM_REG90_TIM3_RCR_POS (0)
#define PWM_REG90_TIM3_RCR_MASK (0xffffffff)

#define PWM_REG91_ADDR (SOC_PWM_REG_BASE + (0x91 << 2))

#define PWM_REG91_CCR1_POS (0)
#define PWM_REG91_CCR1_MASK (0xffffffff)

#define PWM_REG92_ADDR (SOC_PWM_REG_BASE + (0x92 << 2))

#define PWM_REG92_CCR2_POS (0)
#define PWM_REG92_CCR2_MASK (0xffffffff)

#define PWM_REG93_ADDR (SOC_PWM_REG_BASE + (0x93 << 2))

#define PWM_REG93_CCR3_POS (0)
#define PWM_REG93_CCR3_MASK (0xffffffff)

#define PWM_REG94_ADDR (SOC_PWM_REG_BASE + (0x94 << 2))

#define PWM_REG94_CCR4_POS (0)
#define PWM_REG94_CCR4_MASK (0xffffffff)

#define PWM_REG95_ADDR (SOC_PWM_REG_BASE + (0x95 << 2))

#define PWM_REG95_CCR5_POS (0)
#define PWM_REG95_CCR5_MASK (0xffffffff)

#define PWM_REG96_ADDR (SOC_PWM_REG_BASE + (0x96 << 2))

#define PWM_REG96_CCR6_POS (0)
#define PWM_REG96_CCR6_MASK (0xffffffff)

#define PWM_REG97_ADDR (SOC_PWM_REG_BASE + (0x97 << 2))

#define PWM_REG97_CCR7_POS (0)
#define PWM_REG97_CCR7_MASK (0xffffffff)

#define PWM_REG98_ADDR (SOC_PWM_REG_BASE + (0x98 << 2))

#define PWM_REG98_CCR8_POS (0)
#define PWM_REG98_CCR8_MASK (0xffffffff)

#define PWM_REG99_ADDR (SOC_PWM_REG_BASE + (0x99 << 2))

#define PWM_REG99_CCR9_POS (0)
#define PWM_REG99_CCR9_MASK (0xffffffff)

#define PWM_REG9A_ADDR (SOC_PWM_REG_BASE + (0x9a << 2))

#define PWM_REG9A_DT1_POS (0)
#define PWM_REG9A_DT1_MASK (0x3ff)

#define PWM_REG9A_DT2_POS (10)
#define PWM_REG9A_DT2_MASK (0x3ff)

#define PWM_REG9A_DT3_POS (20)
#define PWM_REG9A_DT3_MASK (0x3ff)

#define PWM_REG9A_RESERVED_30_31_POS (30)
#define PWM_REG9A_RESERVED_30_31_MASK (0x3)

#define PWM_REG9B_ADDR (SOC_PWM_REG_BASE + (0x9b << 2))

#define PWM_REG9B_TIM1_ARR_SHAD_POS (0)
#define PWM_REG9B_TIM1_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG9C_ADDR (SOC_PWM_REG_BASE + (0x9c << 2))

#define PWM_REG9C_TIM2_ARR_SHAD_POS (0)
#define PWM_REG9C_TIM2_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG9D_ADDR (SOC_PWM_REG_BASE + (0x9d << 2))

#define PWM_REG9D_TIM3_ARR_SHAD_POS (0)
#define PWM_REG9D_TIM3_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG9E_ADDR (SOC_PWM_REG_BASE + (0x9e << 2))

#define PWM_REG9E_TIM1_RCR_SHAD_POS (0)
#define PWM_REG9E_TIM1_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG9F_ADDR (SOC_PWM_REG_BASE + (0x9f << 2))

#define PWM_REG9F_TIM2_RCR_SHAD_POS (0)
#define PWM_REG9F_TIM2_RCR_SHAD_MASK (0xffffffff)

#define PWM_REGA0_ADDR (SOC_PWM_REG_BASE + (0xa0 << 2))

#define PWM_REGA0_TIM3_RCR_SHAD_POS (0)
#define PWM_REGA0_TIM3_RCR_SHAD_MASK (0xffffffff)

#define PWM_REGA1_ADDR (SOC_PWM_REG_BASE + (0xa1 << 2))

#define PWM_REGA1_CCR1_SHAD_POS (0)
#define PWM_REGA1_CCR1_SHAD_MASK (0xffffffff)

#define PWM_REGA2_ADDR (SOC_PWM_REG_BASE + (0xa2 << 2))

#define PWM_REGA2_CCR2_SHAD_POS (0)
#define PWM_REGA2_CCR2_SHAD_MASK (0xffffffff)

#define PWM_REGA3_ADDR (SOC_PWM_REG_BASE + (0xa3 << 2))

#define PWM_REGA3_CCR3_SHAD_POS (0)
#define PWM_REGA3_CCR3_SHAD_MASK (0xffffffff)

#define PWM_REGA4_ADDR (SOC_PWM_REG_BASE + (0xa4 << 2))

#define PWM_REGA4_CCR4_SHAD_POS (0)
#define PWM_REGA4_CCR4_SHAD_MASK (0xffffffff)

#define PWM_REGA5_ADDR (SOC_PWM_REG_BASE + (0xa5 << 2))

#define PWM_REGA5_CCR5_SHAD_POS (0)
#define PWM_REGA5_CCR5_SHAD_MASK (0xffffffff)

#define PWM_REGA6_ADDR (SOC_PWM_REG_BASE + (0xa6 << 2))

#define PWM_REGA6_CCR6_SHAD_POS (0)
#define PWM_REGA6_CCR6_SHAD_MASK (0xffffffff)

#define PWM_REGA7_ADDR (SOC_PWM_REG_BASE + (0xa7 << 2))

#define PWM_REGA7_CCR7_SHAD_POS (0)
#define PWM_REGA7_CCR7_SHAD_MASK (0xffffffff)

#define PWM_REGA8_ADDR (SOC_PWM_REG_BASE + (0xa8 << 2))

#define PWM_REGA8_CCR8_SHAD_POS (0)
#define PWM_REGA8_CCR8_SHAD_MASK (0xffffffff)

#define PWM_REGA9_ADDR (SOC_PWM_REG_BASE + (0xa9 << 2))

#define PWM_REGA9_CCR9_SHAD_POS (0)
#define PWM_REGA9_CCR9_SHAD_MASK (0xffffffff)

#define PWM_REGAA_ADDR (SOC_PWM_REG_BASE + (0xaa << 2))

#define PWM_REGAA_CH6E_SHAD_POS (0)
#define PWM_REGAA_CH6E_SHAD_MASK (0x1)

#define PWM_REGAA_CH5E_SHAD_POS (1)
#define PWM_REGAA_CH5E_SHAD_MASK (0x1)

#define PWM_REGAA_CH4E_SHAD_POS (2)
#define PWM_REGAA_CH4E_SHAD_MASK (0x1)

#define PWM_REGAA_CH3E_SHAD_POS (3)
#define PWM_REGAA_CH3E_SHAD_MASK (0x1)

#define PWM_REGAA_CH2E_SHAD_POS (4)
#define PWM_REGAA_CH2E_SHAD_MASK (0x1)

#define PWM_REGAA_CH1E_SHAD_POS (5)
#define PWM_REGAA_CH1E_SHAD_MASK (0x1)

#define PWM_REGAA_OC1M_SHAD_POS (6)
#define PWM_REGAA_OC1M_SHAD_MASK (0x7)

#define PWM_REGAA_OC2M_SHAD_POS (9)
#define PWM_REGAA_OC2M_SHAD_MASK (0x7)

#define PWM_REGAA_OC3M_SHAD_POS (12)
#define PWM_REGAA_OC3M_SHAD_MASK (0x7)

#define PWM_REGAA_RESERVED_15_31_POS (15)
#define PWM_REGAA_RESERVED_15_31_MASK (0x1ffff)

#define PWM_REGC0_ADDR (SOC_PWM_REG_BASE + (0xc0 << 2))

#define PWM_REGC0_OCM3PE_POS (0)
#define PWM_REGC0_OCM3PE_MASK (0x1)

#define PWM_REGC0_OCM2PE_POS (1)
#define PWM_REGC0_OCM2PE_MASK (0x1)

#define PWM_REGC0_OCM1PE_POS (2)
#define PWM_REGC0_OCM1PE_MASK (0x1)

#define PWM_REGC0_ARPE3_POS (3)
#define PWM_REGC0_ARPE3_MASK (0x1)

#define PWM_REGC0_ARPE2_POS (4)
#define PWM_REGC0_ARPE2_MASK (0x1)

#define PWM_REGC0_ARPE1_POS (5)
#define PWM_REGC0_ARPE1_MASK (0x1)

#define PWM_REGC0_OC3PE_POS (6)
#define PWM_REGC0_OC3PE_MASK (0x1)

#define PWM_REGC0_OC2PE_POS (7)
#define PWM_REGC0_OC2PE_MASK (0x1)

#define PWM_REGC0_OC1PE_POS (8)
#define PWM_REGC0_OC1PE_MASK (0x1)

#define PWM_REGC0_RESERVED_9_17_POS (9)
#define PWM_REGC0_RESERVED_9_17_MASK (0x1ff)

#define PWM_REGC0_CMS_TIM3_POS (18)
#define PWM_REGC0_CMS_TIM3_MASK (0x3)

#define PWM_REGC0_CMS_TIM2_POS (20)
#define PWM_REGC0_CMS_TIM2_MASK (0x3)

#define PWM_REGC0_CMS_TIM1_POS (22)
#define PWM_REGC0_CMS_TIM1_MASK (0x3)

#define PWM_REGC0_RESERVED_24_25_POS (24)
#define PWM_REGC0_RESERVED_24_25_MASK (0x3)

#define PWM_REGC0_URS3_POS (26)
#define PWM_REGC0_URS3_MASK (0x1)

#define PWM_REGC0_URS2_POS (27)
#define PWM_REGC0_URS2_MASK (0x1)

#define PWM_REGC0_URS1_POS (28)
#define PWM_REGC0_URS1_MASK (0x1)

#define PWM_REGC0_DIR_TIM3_POS (29)
#define PWM_REGC0_DIR_TIM3_MASK (0x1)

#define PWM_REGC0_DIR_TIM2_POS (30)
#define PWM_REGC0_DIR_TIM2_MASK (0x1)

#define PWM_REGC0_DIR_TIM1_POS (31)
#define PWM_REGC0_DIR_TIM1_MASK (0x1)

#define PWM_REGC1_ADDR (SOC_PWM_REG_BASE + (0xc1 << 2))

#define PWM_REGC1_RESERVED_0_7_POS (0)
#define PWM_REGC1_RESERVED_0_7_MASK (0xff)

#define PWM_REGC1_COM_MODE_POS (8)
#define PWM_REGC1_COM_MODE_MASK (0x1)

#define PWM_REGC1_CCPC_POS (9)
#define PWM_REGC1_CCPC_MASK (0x1)

#define PWM_REGC1_TRIOS3_POS (10)
#define PWM_REGC1_TRIOS3_MASK (0x7)

#define PWM_REGC1_ADCS3_POS (13)
#define PWM_REGC1_ADCS3_MASK (0x7)

#define PWM_REGC1_RESERVED_16_16_POS (16)
#define PWM_REGC1_RESERVED_16_16_MASK (0x1)

#define PWM_REGC1_CCUS_POS (17)
#define PWM_REGC1_CCUS_MASK (0x1)

#define PWM_REGC1_TRIOS2_POS (18)
#define PWM_REGC1_TRIOS2_MASK (0x7)

#define PWM_REGC1_ADCS2_POS (21)
#define PWM_REGC1_ADCS2_MASK (0x7)

#define PWM_REGC1_RESERVED_24_25_POS (24)
#define PWM_REGC1_RESERVED_24_25_MASK (0x3)

#define PWM_REGC1_TRIOS1_POS (26)
#define PWM_REGC1_TRIOS1_MASK (0x7)

#define PWM_REGC1_ADCS1_POS (29)
#define PWM_REGC1_ADCS1_MASK (0x7)

#define PWM_REGC2_ADDR (SOC_PWM_REG_BASE + (0xc2 << 2))

#define PWM_REGC2_SMS1_POS (0)
#define PWM_REGC2_SMS1_MASK (0xf)

#define PWM_REGC2_SMS2_POS (4)
#define PWM_REGC2_SMS2_MASK (0xf)

#define PWM_REGC2_SMS3_POS (8)
#define PWM_REGC2_SMS3_MASK (0xf)

#define PWM_REGC2_TS1_POS (12)
#define PWM_REGC2_TS1_MASK (0x7)

#define PWM_REGC2_RESERVED_15_15_POS (15)
#define PWM_REGC2_RESERVED_15_15_MASK (0x1)

#define PWM_REGC2_TS2_POS (16)
#define PWM_REGC2_TS2_MASK (0x7)

#define PWM_REGC2_TS3_POS (19)
#define PWM_REGC2_TS3_MASK (0x7)

#define PWM_REGC2_RESERVED_22_31_POS (22)
#define PWM_REGC2_RESERVED_22_31_MASK (0x3ff)

#define PWM_REGC3_ADDR (SOC_PWM_REG_BASE + (0xc3 << 2))

#define PWM_REGC3_CC1IE_POS (0)
#define PWM_REGC3_CC1IE_MASK (0x1)

#define PWM_REGC3_CC2IE_POS (1)
#define PWM_REGC3_CC2IE_MASK (0x1)

#define PWM_REGC3_CC3IE_POS (2)
#define PWM_REGC3_CC3IE_MASK (0x1)

#define PWM_REGC3_CC4IE_POS (3)
#define PWM_REGC3_CC4IE_MASK (0x1)

#define PWM_REGC3_CC5IE_POS (4)
#define PWM_REGC3_CC5IE_MASK (0x1)

#define PWM_REGC3_CC6IE_POS (5)
#define PWM_REGC3_CC6IE_MASK (0x1)

#define PWM_REGC3_CC7IE_POS (6)
#define PWM_REGC3_CC7IE_MASK (0x1)

#define PWM_REGC3_CC8IE_POS (7)
#define PWM_REGC3_CC8IE_MASK (0x1)

#define PWM_REGC3_CC9IE_POS (8)
#define PWM_REGC3_CC9IE_MASK (0x1)

#define PWM_REGC3_UIE1_POS (9)
#define PWM_REGC3_UIE1_MASK (0x1)

#define PWM_REGC3_UIE2_POS (10)
#define PWM_REGC3_UIE2_MASK (0x1)

#define PWM_REGC3_UIE3_POS (11)
#define PWM_REGC3_UIE3_MASK (0x1)

#define PWM_REGC3_TIE1_POS (12)
#define PWM_REGC3_TIE1_MASK (0x1)

#define PWM_REGC3_TIE2_POS (13)
#define PWM_REGC3_TIE2_MASK (0x1)

#define PWM_REGC3_TIE3_POS (14)
#define PWM_REGC3_TIE3_MASK (0x1)

#define PWM_REGC3_COMIE_POS (15)
#define PWM_REGC3_COMIE_MASK (0x1)

#define PWM_REGC3_RESERVED_16_19_POS (16)
#define PWM_REGC3_RESERVED_16_19_MASK (0xf)

#define PWM_REGC3_DF_TIM1_POS (20)
#define PWM_REGC3_DF_TIM1_MASK (0xf)

#define PWM_REGC3_DF_TIM2_POS (24)
#define PWM_REGC3_DF_TIM2_MASK (0xf)

#define PWM_REGC3_DF_TIM3_POS (28)
#define PWM_REGC3_DF_TIM3_MASK (0xf)

#define PWM_REGC4_ADDR (SOC_PWM_REG_BASE + (0xc4 << 2))

#define PWM_REGC4_CC1IF_POS (0)
#define PWM_REGC4_CC1IF_MASK (0x1)

#define PWM_REGC4_CC2IF_POS (1)
#define PWM_REGC4_CC2IF_MASK (0x1)

#define PWM_REGC4_CC3IF_POS (2)
#define PWM_REGC4_CC3IF_MASK (0x1)

#define PWM_REGC4_CC4IF_POS (3)
#define PWM_REGC4_CC4IF_MASK (0x1)

#define PWM_REGC4_CC5IF_POS (4)
#define PWM_REGC4_CC5IF_MASK (0x1)

#define PWM_REGC4_CC6IF_POS (5)
#define PWM_REGC4_CC6IF_MASK (0x1)

#define PWM_REGC4_CC7IF_POS (6)
#define PWM_REGC4_CC7IF_MASK (0x1)

#define PWM_REGC4_CC8IF_POS (7)
#define PWM_REGC4_CC8IF_MASK (0x1)

#define PWM_REGC4_CC9IF_POS (8)
#define PWM_REGC4_CC9IF_MASK (0x1)

#define PWM_REGC4_UIF1_POS (9)
#define PWM_REGC4_UIF1_MASK (0x1)

#define PWM_REGC4_UIF2_POS (10)
#define PWM_REGC4_UIF2_MASK (0x1)

#define PWM_REGC4_UIF3_POS (11)
#define PWM_REGC4_UIF3_MASK (0x1)

#define PWM_REGC4_TIF1_POS (12)
#define PWM_REGC4_TIF1_MASK (0x1)

#define PWM_REGC4_TIF2_POS (13)
#define PWM_REGC4_TIF2_MASK (0x1)

#define PWM_REGC4_TIF3_POS (14)
#define PWM_REGC4_TIF3_MASK (0x1)

#define PWM_REGC4_COMIF_POS (15)
#define PWM_REGC4_COMIF_MASK (0x1)

#define PWM_REGC4_RESERVED_16_31_POS (16)
#define PWM_REGC4_RESERVED_16_31_MASK (0xffff)

#define PWM_REGC5_ADDR (SOC_PWM_REG_BASE + (0xc5 << 2))

#define PWM_REGC5_RESERVED_0_8_POS (0)
#define PWM_REGC5_RESERVED_0_8_MASK (0x1ff)

#define PWM_REGC5_UG1_POS (9)
#define PWM_REGC5_UG1_MASK (0x1)

#define PWM_REGC5_UG2_POS (10)
#define PWM_REGC5_UG2_MASK (0x1)

#define PWM_REGC5_UG3_POS (11)
#define PWM_REGC5_UG3_MASK (0x1)

#define PWM_REGC5_TG1_POS (12)
#define PWM_REGC5_TG1_MASK (0x1)

#define PWM_REGC5_TG2_POS (13)
#define PWM_REGC5_TG2_MASK (0x1)

#define PWM_REGC5_TG3_POS (14)
#define PWM_REGC5_TG3_MASK (0x1)

#define PWM_REGC5_COMG_POS (15)
#define PWM_REGC5_COMG_MASK (0x1)

#define PWM_REGC5_DTM1_POS (16)
#define PWM_REGC5_DTM1_MASK (0x3)

#define PWM_REGC5_DTM2_POS (18)
#define PWM_REGC5_DTM2_MASK (0x3)

#define PWM_REGC5_DTM3_POS (20)
#define PWM_REGC5_DTM3_MASK (0x3)

#define PWM_REGC5_RESERVED_22_31_POS (22)
#define PWM_REGC5_RESERVED_22_31_MASK (0x3ff)

#define PWM_REGC6_ADDR (SOC_PWM_REG_BASE + (0xc6 << 2))

#define PWM_REGC6_CH1P_POS (0)
#define PWM_REGC6_CH1P_MASK (0x3)

#define PWM_REGC6_CH2P_POS (2)
#define PWM_REGC6_CH2P_MASK (0x3)

#define PWM_REGC6_CH3P_POS (4)
#define PWM_REGC6_CH3P_MASK (0x3)

#define PWM_REGC6_CH4P_POS (6)
#define PWM_REGC6_CH4P_MASK (0x3)

#define PWM_REGC6_CH5P_POS (8)
#define PWM_REGC6_CH5P_MASK (0x3)

#define PWM_REGC6_CH6P_POS (10)
#define PWM_REGC6_CH6P_MASK (0x3)

#define PWM_REGC6_CH1E_POS (12)
#define PWM_REGC6_CH1E_MASK (0x1)

#define PWM_REGC6_CH2E_POS (13)
#define PWM_REGC6_CH2E_MASK (0x1)

#define PWM_REGC6_CH3E_POS (14)
#define PWM_REGC6_CH3E_MASK (0x1)

#define PWM_REGC6_CH4E_POS (15)
#define PWM_REGC6_CH4E_MASK (0x1)

#define PWM_REGC6_CH5E_POS (16)
#define PWM_REGC6_CH5E_MASK (0x1)

#define PWM_REGC6_CH6E_POS (17)
#define PWM_REGC6_CH6E_MASK (0x1)

#define PWM_REGC6_TIM1CCM_POS (18)
#define PWM_REGC6_TIM1CCM_MASK (0x1)

#define PWM_REGC6_TIM2CCM_POS (19)
#define PWM_REGC6_TIM2CCM_MASK (0x1)

#define PWM_REGC6_TIM3CCM_POS (20)
#define PWM_REGC6_TIM3CCM_MASK (0x1)

#define PWM_REGC6_OC1M_POS (21)
#define PWM_REGC6_OC1M_MASK (0x7)

#define PWM_REGC6_OC2M_POS (24)
#define PWM_REGC6_OC2M_MASK (0x7)

#define PWM_REGC6_OC3M_POS (27)
#define PWM_REGC6_OC3M_MASK (0x7)

#define PWM_REGC6_RESERVED_30_31_POS (30)
#define PWM_REGC6_RESERVED_30_31_MASK (0x3)

#define PWM_REGC7_ADDR (SOC_PWM_REG_BASE + (0xc7 << 2))

#define PWM_REGC7_TIM1_CNT_POS (0)
#define PWM_REGC7_TIM1_CNT_MASK (0xffffffff)

#define PWM_REGC8_ADDR (SOC_PWM_REG_BASE + (0xc8 << 2))

#define PWM_REGC8_TIM2_CNT_POS (0)
#define PWM_REGC8_TIM2_CNT_MASK (0xffffffff)

#define PWM_REGC9_ADDR (SOC_PWM_REG_BASE + (0xc9 << 2))

#define PWM_REGC9_TIM3_CNT_POS (0)
#define PWM_REGC9_TIM3_CNT_MASK (0xffffffff)

#define PWM_REGCA_ADDR (SOC_PWM_REG_BASE + (0xca << 2))

#define PWM_REGCA_PSC1_POS (0)
#define PWM_REGCA_PSC1_MASK (0xff)

#define PWM_REGCA_PSC2_POS (8)
#define PWM_REGCA_PSC2_MASK (0xff)

#define PWM_REGCA_PSC3_POS (16)
#define PWM_REGCA_PSC3_MASK (0xff)

#define PWM_REGCA_RESERVED_24_31_POS (24)
#define PWM_REGCA_RESERVED_24_31_MASK (0xff)

#define PWM_REGCB_ADDR (SOC_PWM_REG_BASE + (0xcb << 2))

#define PWM_REGCB_TIM1_ARR_POS (0)
#define PWM_REGCB_TIM1_ARR_MASK (0xffffffff)

#define PWM_REGCC_ADDR (SOC_PWM_REG_BASE + (0xcc << 2))

#define PWM_REGCC_TIM2_ARR_POS (0)
#define PWM_REGCC_TIM2_ARR_MASK (0xffffffff)

#define PWM_REGCD_ADDR (SOC_PWM_REG_BASE + (0xcd << 2))

#define PWM_REGCD_TIM3_ARR_POS (0)
#define PWM_REGCD_TIM3_ARR_MASK (0xffffffff)

#define PWM_REGCE_ADDR (SOC_PWM_REG_BASE + (0xce << 2))

#define PWM_REGCE_TIM1_RCR_POS (0)
#define PWM_REGCE_TIM1_RCR_MASK (0xffffffff)

#define PWM_REGCF_ADDR (SOC_PWM_REG_BASE + (0xcf << 2))

#define PWM_REGCF_TIM2_RCR_POS (0)
#define PWM_REGCF_TIM2_RCR_MASK (0xffffffff)

#define PWM_REGD0_ADDR (SOC_PWM_REG_BASE + (0xd0 << 2))

#define PWM_REGD0_TIM3_RCR_POS (0)
#define PWM_REGD0_TIM3_RCR_MASK (0xffffffff)

#define PWM_REGD1_ADDR (SOC_PWM_REG_BASE + (0xd1 << 2))

#define PWM_REGD1_CCR1_POS (0)
#define PWM_REGD1_CCR1_MASK (0xffffffff)

#define PWM_REGD2_ADDR (SOC_PWM_REG_BASE + (0xd2 << 2))

#define PWM_REGD2_CCR2_POS (0)
#define PWM_REGD2_CCR2_MASK (0xffffffff)

#define PWM_REGD3_ADDR (SOC_PWM_REG_BASE + (0xd3 << 2))

#define PWM_REGD3_CCR3_POS (0)
#define PWM_REGD3_CCR3_MASK (0xffffffff)

#define PWM_REGD4_ADDR (SOC_PWM_REG_BASE + (0xd4 << 2))

#define PWM_REGD4_CCR4_POS (0)
#define PWM_REGD4_CCR4_MASK (0xffffffff)

#define PWM_REGD5_ADDR (SOC_PWM_REG_BASE + (0xd5 << 2))

#define PWM_REGD5_CCR5_POS (0)
#define PWM_REGD5_CCR5_MASK (0xffffffff)

#define PWM_REGD6_ADDR (SOC_PWM_REG_BASE + (0xd6 << 2))

#define PWM_REGD6_CCR6_POS (0)
#define PWM_REGD6_CCR6_MASK (0xffffffff)

#define PWM_REGD7_ADDR (SOC_PWM_REG_BASE + (0xd7 << 2))

#define PWM_REGD7_CCR7_POS (0)
#define PWM_REGD7_CCR7_MASK (0xffffffff)

#define PWM_REGD8_ADDR (SOC_PWM_REG_BASE + (0xd8 << 2))

#define PWM_REGD8_CCR8_POS (0)
#define PWM_REGD8_CCR8_MASK (0xffffffff)

#define PWM_REGD9_ADDR (SOC_PWM_REG_BASE + (0xd9 << 2))

#define PWM_REGD9_CCR9_POS (0)
#define PWM_REGD9_CCR9_MASK (0xffffffff)

#define PWM_REGDA_ADDR (SOC_PWM_REG_BASE + (0xda << 2))

#define PWM_REGDA_DT1_POS (0)
#define PWM_REGDA_DT1_MASK (0x3ff)

#define PWM_REGDA_DT2_POS (10)
#define PWM_REGDA_DT2_MASK (0x3ff)

#define PWM_REGDA_DT3_POS (20)
#define PWM_REGDA_DT3_MASK (0x3ff)

#define PWM_REGDA_RESERVED_30_31_POS (30)
#define PWM_REGDA_RESERVED_30_31_MASK (0x3)

#define PWM_REGDB_ADDR (SOC_PWM_REG_BASE + (0xdb << 2))

#define PWM_REGDB_TIM1_ARR_SHAD_POS (0)
#define PWM_REGDB_TIM1_ARR_SHAD_MASK (0xffffffff)

#define PWM_REGDC_ADDR (SOC_PWM_REG_BASE + (0xdc << 2))

#define PWM_REGDC_TIM2_ARR_SHAD_POS (0)
#define PWM_REGDC_TIM2_ARR_SHAD_MASK (0xffffffff)

#define PWM_REGDD_ADDR (SOC_PWM_REG_BASE + (0xdd << 2))

#define PWM_REGDD_TIM3_ARR_SHAD_POS (0)
#define PWM_REGDD_TIM3_ARR_SHAD_MASK (0xffffffff)

#define PWM_REGDE_ADDR (SOC_PWM_REG_BASE + (0xde << 2))

#define PWM_REGDE_TIM1_RCR_SHAD_POS (0)
#define PWM_REGDE_TIM1_RCR_SHAD_MASK (0xffffffff)

#define PWM_REGDF_ADDR (SOC_PWM_REG_BASE + (0xdf << 2))

#define PWM_REGDF_TIM2_RCR_SHAD_POS (0)
#define PWM_REGDF_TIM2_RCR_SHAD_MASK (0xffffffff)

#define PWM_REGE0_ADDR (SOC_PWM_REG_BASE + (0xe0 << 2))

#define PWM_REGE0_TIM3_RCR_SHAD_POS (0)
#define PWM_REGE0_TIM3_RCR_SHAD_MASK (0xffffffff)

#define PWM_REGE1_ADDR (SOC_PWM_REG_BASE + (0xe1 << 2))

#define PWM_REGE1_CCR1_SHAD_POS (0)
#define PWM_REGE1_CCR1_SHAD_MASK (0xffffffff)

#define PWM_REGE2_ADDR (SOC_PWM_REG_BASE + (0xe2 << 2))

#define PWM_REGE2_CCR2_SHAD_POS (0)
#define PWM_REGE2_CCR2_SHAD_MASK (0xffffffff)

#define PWM_REGE3_ADDR (SOC_PWM_REG_BASE + (0xe3 << 2))

#define PWM_REGE3_CCR3_SHAD_POS (0)
#define PWM_REGE3_CCR3_SHAD_MASK (0xffffffff)

#define PWM_REGE4_ADDR (SOC_PWM_REG_BASE + (0xe4 << 2))

#define PWM_REGE4_CCR4_SHAD_POS (0)
#define PWM_REGE4_CCR4_SHAD_MASK (0xffffffff)

#define PWM_REGE5_ADDR (SOC_PWM_REG_BASE + (0xe5 << 2))

#define PWM_REGE5_CCR5_SHAD_POS (0)
#define PWM_REGE5_CCR5_SHAD_MASK (0xffffffff)

#define PWM_REGE6_ADDR (SOC_PWM_REG_BASE + (0xe6 << 2))

#define PWM_REGE6_CCR6_SHAD_POS (0)
#define PWM_REGE6_CCR6_SHAD_MASK (0xffffffff)

#define PWM_REGE7_ADDR (SOC_PWM_REG_BASE + (0xe7 << 2))

#define PWM_REGE7_CCR7_SHAD_POS (0)
#define PWM_REGE7_CCR7_SHAD_MASK (0xffffffff)

#define PWM_REGE8_ADDR (SOC_PWM_REG_BASE + (0xe8 << 2))

#define PWM_REGE8_CCR8_SHAD_POS (0)
#define PWM_REGE8_CCR8_SHAD_MASK (0xffffffff)

#define PWM_REGE9_ADDR (SOC_PWM_REG_BASE + (0xe9 << 2))

#define PWM_REGE9_CCR9_SHAD_POS (0)
#define PWM_REGE9_CCR9_SHAD_MASK (0xffffffff)

#define PWM_REGEA_ADDR (SOC_PWM_REG_BASE + (0xea << 2))

#define PWM_REGEA_CH6E_SHAD_POS (0)
#define PWM_REGEA_CH6E_SHAD_MASK (0x1)

#define PWM_REGEA_CH5E_SHAD_POS (1)
#define PWM_REGEA_CH5E_SHAD_MASK (0x1)

#define PWM_REGEA_CH4E_SHAD_POS (2)
#define PWM_REGEA_CH4E_SHAD_MASK (0x1)

#define PWM_REGEA_CH3E_SHAD_POS (3)
#define PWM_REGEA_CH3E_SHAD_MASK (0x1)

#define PWM_REGEA_CH2E_SHAD_POS (4)
#define PWM_REGEA_CH2E_SHAD_MASK (0x1)

#define PWM_REGEA_CH1E_SHAD_POS (5)
#define PWM_REGEA_CH1E_SHAD_MASK (0x1)

#define PWM_REGEA_OC1M_SHAD_POS (6)
#define PWM_REGEA_OC1M_SHAD_MASK (0x7)

#define PWM_REGEA_OC2M_SHAD_POS (9)
#define PWM_REGEA_OC2M_SHAD_MASK (0x7)

#define PWM_REGEA_OC3M_SHAD_POS (12)
#define PWM_REGEA_OC3M_SHAD_MASK (0x7)

#define PWM_REGEA_RESERVED_15_31_POS (15)
#define PWM_REGEA_RESERVED_15_31_MASK (0x1ffff)

#define PWM_REG100_ADDR (SOC_PWM_REG_BASE + (0x100 << 2))

#define PWM_REG100_OCM3PE_POS (0)
#define PWM_REG100_OCM3PE_MASK (0x1)

#define PWM_REG100_OCM2PE_POS (1)
#define PWM_REG100_OCM2PE_MASK (0x1)

#define PWM_REG100_OCM1PE_POS (2)
#define PWM_REG100_OCM1PE_MASK (0x1)

#define PWM_REG100_ARPE3_POS (3)
#define PWM_REG100_ARPE3_MASK (0x1)

#define PWM_REG100_ARPE2_POS (4)
#define PWM_REG100_ARPE2_MASK (0x1)

#define PWM_REG100_ARPE1_POS (5)
#define PWM_REG100_ARPE1_MASK (0x1)

#define PWM_REG100_OC3PE_POS (6)
#define PWM_REG100_OC3PE_MASK (0x1)

#define PWM_REG100_OC2PE_POS (7)
#define PWM_REG100_OC2PE_MASK (0x1)

#define PWM_REG100_OC1PE_POS (8)
#define PWM_REG100_OC1PE_MASK (0x1)

#define PWM_REG100_RESERVED_9_17_POS (9)
#define PWM_REG100_RESERVED_9_17_MASK (0x1ff)

#define PWM_REG100_CMS_TIM3_POS (18)
#define PWM_REG100_CMS_TIM3_MASK (0x3)

#define PWM_REG100_CMS_TIM2_POS (20)
#define PWM_REG100_CMS_TIM2_MASK (0x3)

#define PWM_REG100_CMS_TIM1_POS (22)
#define PWM_REG100_CMS_TIM1_MASK (0x3)

#define PWM_REG100_RESERVED_24_25_POS (24)
#define PWM_REG100_RESERVED_24_25_MASK (0x3)

#define PWM_REG100_URS3_POS (26)
#define PWM_REG100_URS3_MASK (0x1)

#define PWM_REG100_URS2_POS (27)
#define PWM_REG100_URS2_MASK (0x1)

#define PWM_REG100_URS1_POS (28)
#define PWM_REG100_URS1_MASK (0x1)

#define PWM_REG100_DIR_TIM3_POS (29)
#define PWM_REG100_DIR_TIM3_MASK (0x1)

#define PWM_REG100_DIR_TIM2_POS (30)
#define PWM_REG100_DIR_TIM2_MASK (0x1)

#define PWM_REG100_DIR_TIM1_POS (31)
#define PWM_REG100_DIR_TIM1_MASK (0x1)

#define PWM_REG101_ADDR (SOC_PWM_REG_BASE + (0x101 << 2))

#define PWM_REG101_RESERVED_0_7_POS (0)
#define PWM_REG101_RESERVED_0_7_MASK (0xff)

#define PWM_REG101_COM_MODE_POS (8)
#define PWM_REG101_COM_MODE_MASK (0x1)

#define PWM_REG101_CCPC_POS (9)
#define PWM_REG101_CCPC_MASK (0x1)

#define PWM_REG101_TRIOS3_POS (10)
#define PWM_REG101_TRIOS3_MASK (0x7)

#define PWM_REG101_ADCS3_POS (13)
#define PWM_REG101_ADCS3_MASK (0x7)

#define PWM_REG101_RESERVED_16_16_POS (16)
#define PWM_REG101_RESERVED_16_16_MASK (0x1)

#define PWM_REG101_CCUS_POS (17)
#define PWM_REG101_CCUS_MASK (0x1)

#define PWM_REG101_TRIOS2_POS (18)
#define PWM_REG101_TRIOS2_MASK (0x7)

#define PWM_REG101_ADCS2_POS (21)
#define PWM_REG101_ADCS2_MASK (0x7)

#define PWM_REG101_RESERVED_24_25_POS (24)
#define PWM_REG101_RESERVED_24_25_MASK (0x3)

#define PWM_REG101_TRIOS1_POS (26)
#define PWM_REG101_TRIOS1_MASK (0x7)

#define PWM_REG101_ADCS1_POS (29)
#define PWM_REG101_ADCS1_MASK (0x7)

#define PWM_REG102_ADDR (SOC_PWM_REG_BASE + (0x102 << 2))

#define PWM_REG102_SMS1_POS (0)
#define PWM_REG102_SMS1_MASK (0xf)

#define PWM_REG102_SMS2_POS (4)
#define PWM_REG102_SMS2_MASK (0xf)

#define PWM_REG102_SMS3_POS (8)
#define PWM_REG102_SMS3_MASK (0xf)

#define PWM_REG102_TS1_POS (12)
#define PWM_REG102_TS1_MASK (0x7)

#define PWM_REG102_RESERVED_15_15_POS (15)
#define PWM_REG102_RESERVED_15_15_MASK (0x1)

#define PWM_REG102_TS2_POS (16)
#define PWM_REG102_TS2_MASK (0x7)

#define PWM_REG102_TS3_POS (19)
#define PWM_REG102_TS3_MASK (0x7)

#define PWM_REG102_RESERVED_22_31_POS (22)
#define PWM_REG102_RESERVED_22_31_MASK (0x3ff)

#define PWM_REG103_ADDR (SOC_PWM_REG_BASE + (0x103 << 2))

#define PWM_REG103_CC1IE_POS (0)
#define PWM_REG103_CC1IE_MASK (0x1)

#define PWM_REG103_CC2IE_POS (1)
#define PWM_REG103_CC2IE_MASK (0x1)

#define PWM_REG103_CC3IE_POS (2)
#define PWM_REG103_CC3IE_MASK (0x1)

#define PWM_REG103_CC4IE_POS (3)
#define PWM_REG103_CC4IE_MASK (0x1)

#define PWM_REG103_CC5IE_POS (4)
#define PWM_REG103_CC5IE_MASK (0x1)

#define PWM_REG103_CC6IE_POS (5)
#define PWM_REG103_CC6IE_MASK (0x1)

#define PWM_REG103_CC7IE_POS (6)
#define PWM_REG103_CC7IE_MASK (0x1)

#define PWM_REG103_CC8IE_POS (7)
#define PWM_REG103_CC8IE_MASK (0x1)

#define PWM_REG103_CC9IE_POS (8)
#define PWM_REG103_CC9IE_MASK (0x1)

#define PWM_REG103_UIE1_POS (9)
#define PWM_REG103_UIE1_MASK (0x1)

#define PWM_REG103_UIE2_POS (10)
#define PWM_REG103_UIE2_MASK (0x1)

#define PWM_REG103_UIE3_POS (11)
#define PWM_REG103_UIE3_MASK (0x1)

#define PWM_REG103_TIE1_POS (12)
#define PWM_REG103_TIE1_MASK (0x1)

#define PWM_REG103_TIE2_POS (13)
#define PWM_REG103_TIE2_MASK (0x1)

#define PWM_REG103_TIE3_POS (14)
#define PWM_REG103_TIE3_MASK (0x1)

#define PWM_REG103_COMIE_POS (15)
#define PWM_REG103_COMIE_MASK (0x1)

#define PWM_REG103_RESERVED_16_19_POS (16)
#define PWM_REG103_RESERVED_16_19_MASK (0xf)

#define PWM_REG103_DF_TIM1_POS (20)
#define PWM_REG103_DF_TIM1_MASK (0xf)

#define PWM_REG103_DF_TIM2_POS (24)
#define PWM_REG103_DF_TIM2_MASK (0xf)

#define PWM_REG103_DF_TIM3_POS (28)
#define PWM_REG103_DF_TIM3_MASK (0xf)

#define PWM_REG104_ADDR (SOC_PWM_REG_BASE + (0x104 << 2))

#define PWM_REG104_CC1IF_POS (0)
#define PWM_REG104_CC1IF_MASK (0x1)

#define PWM_REG104_CC2IF_POS (1)
#define PWM_REG104_CC2IF_MASK (0x1)

#define PWM_REG104_CC3IF_POS (2)
#define PWM_REG104_CC3IF_MASK (0x1)

#define PWM_REG104_CC4IF_POS (3)
#define PWM_REG104_CC4IF_MASK (0x1)

#define PWM_REG104_CC5IF_POS (4)
#define PWM_REG104_CC5IF_MASK (0x1)

#define PWM_REG104_CC6IF_POS (5)
#define PWM_REG104_CC6IF_MASK (0x1)

#define PWM_REG104_CC7IF_POS (6)
#define PWM_REG104_CC7IF_MASK (0x1)

#define PWM_REG104_CC8IF_POS (7)
#define PWM_REG104_CC8IF_MASK (0x1)

#define PWM_REG104_CC9IF_POS (8)
#define PWM_REG104_CC9IF_MASK (0x1)

#define PWM_REG104_UIF1_POS (9)
#define PWM_REG104_UIF1_MASK (0x1)

#define PWM_REG104_UIF2_POS (10)
#define PWM_REG104_UIF2_MASK (0x1)

#define PWM_REG104_UIF3_POS (11)
#define PWM_REG104_UIF3_MASK (0x1)

#define PWM_REG104_TIF1_POS (12)
#define PWM_REG104_TIF1_MASK (0x1)

#define PWM_REG104_TIF2_POS (13)
#define PWM_REG104_TIF2_MASK (0x1)

#define PWM_REG104_TIF3_POS (14)
#define PWM_REG104_TIF3_MASK (0x1)

#define PWM_REG104_COMIF_POS (15)
#define PWM_REG104_COMIF_MASK (0x1)

#define PWM_REG104_RESERVED_16_31_POS (16)
#define PWM_REG104_RESERVED_16_31_MASK (0xffff)

#define PWM_REG105_ADDR (SOC_PWM_REG_BASE + (0x105 << 2))

#define PWM_REG105_RESERVED_0_8_POS (0)
#define PWM_REG105_RESERVED_0_8_MASK (0x1ff)

#define PWM_REG105_UG1_POS (9)
#define PWM_REG105_UG1_MASK (0x1)

#define PWM_REG105_UG2_POS (10)
#define PWM_REG105_UG2_MASK (0x1)

#define PWM_REG105_UG3_POS (11)
#define PWM_REG105_UG3_MASK (0x1)

#define PWM_REG105_TG1_POS (12)
#define PWM_REG105_TG1_MASK (0x1)

#define PWM_REG105_TG2_POS (13)
#define PWM_REG105_TG2_MASK (0x1)

#define PWM_REG105_TG3_POS (14)
#define PWM_REG105_TG3_MASK (0x1)

#define PWM_REG105_COMG_POS (15)
#define PWM_REG105_COMG_MASK (0x1)

#define PWM_REG105_DTM1_POS (16)
#define PWM_REG105_DTM1_MASK (0x3)

#define PWM_REG105_DTM2_POS (18)
#define PWM_REG105_DTM2_MASK (0x3)

#define PWM_REG105_DTM3_POS (20)
#define PWM_REG105_DTM3_MASK (0x3)

#define PWM_REG105_RESERVED_22_31_POS (22)
#define PWM_REG105_RESERVED_22_31_MASK (0x3ff)

#define PWM_REG106_ADDR (SOC_PWM_REG_BASE + (0x106 << 2))

#define PWM_REG106_CH1P_POS (0)
#define PWM_REG106_CH1P_MASK (0x3)

#define PWM_REG106_CH2P_POS (2)
#define PWM_REG106_CH2P_MASK (0x3)

#define PWM_REG106_CH3P_POS (4)
#define PWM_REG106_CH3P_MASK (0x3)

#define PWM_REG106_CH4P_POS (6)
#define PWM_REG106_CH4P_MASK (0x3)

#define PWM_REG106_CH5P_POS (8)
#define PWM_REG106_CH5P_MASK (0x3)

#define PWM_REG106_CH6P_POS (10)
#define PWM_REG106_CH6P_MASK (0x3)

#define PWM_REG106_CH1E_POS (12)
#define PWM_REG106_CH1E_MASK (0x1)

#define PWM_REG106_CH2E_POS (13)
#define PWM_REG106_CH2E_MASK (0x1)

#define PWM_REG106_CH3E_POS (14)
#define PWM_REG106_CH3E_MASK (0x1)

#define PWM_REG106_CH4E_POS (15)
#define PWM_REG106_CH4E_MASK (0x1)

#define PWM_REG106_CH5E_POS (16)
#define PWM_REG106_CH5E_MASK (0x1)

#define PWM_REG106_CH6E_POS (17)
#define PWM_REG106_CH6E_MASK (0x1)

#define PWM_REG106_TIM1CCM_POS (18)
#define PWM_REG106_TIM1CCM_MASK (0x1)

#define PWM_REG106_TIM2CCM_POS (19)
#define PWM_REG106_TIM2CCM_MASK (0x1)

#define PWM_REG106_TIM3CCM_POS (20)
#define PWM_REG106_TIM3CCM_MASK (0x1)

#define PWM_REG106_OC1M_POS (21)
#define PWM_REG106_OC1M_MASK (0x7)

#define PWM_REG106_OC2M_POS (24)
#define PWM_REG106_OC2M_MASK (0x7)

#define PWM_REG106_OC3M_POS (27)
#define PWM_REG106_OC3M_MASK (0x7)

#define PWM_REG106_RESERVED_30_31_POS (30)
#define PWM_REG106_RESERVED_30_31_MASK (0x3)

#define PWM_REG107_ADDR (SOC_PWM_REG_BASE + (0x107 << 2))

#define PWM_REG107_TIM1_CNT_POS (0)
#define PWM_REG107_TIM1_CNT_MASK (0xffffffff)

#define PWM_REG108_ADDR (SOC_PWM_REG_BASE + (0x108 << 2))

#define PWM_REG108_TIM2_CNT_POS (0)
#define PWM_REG108_TIM2_CNT_MASK (0xffffffff)

#define PWM_REG109_ADDR (SOC_PWM_REG_BASE + (0x109 << 2))

#define PWM_REG109_TIM3_CNT_POS (0)
#define PWM_REG109_TIM3_CNT_MASK (0xffffffff)

#define PWM_REG10A_ADDR (SOC_PWM_REG_BASE + (0x10a << 2))

#define PWM_REG10A_PSC1_POS (0)
#define PWM_REG10A_PSC1_MASK (0xff)

#define PWM_REG10A_PSC2_POS (8)
#define PWM_REG10A_PSC2_MASK (0xff)

#define PWM_REG10A_PSC3_POS (16)
#define PWM_REG10A_PSC3_MASK (0xff)

#define PWM_REG10A_RESERVED_24_31_POS (24)
#define PWM_REG10A_RESERVED_24_31_MASK (0xff)

#define PWM_REG10B_ADDR (SOC_PWM_REG_BASE + (0x10b << 2))

#define PWM_REG10B_TIM1_ARR_POS (0)
#define PWM_REG10B_TIM1_ARR_MASK (0xffffffff)

#define PWM_REG10C_ADDR (SOC_PWM_REG_BASE + (0x10c << 2))

#define PWM_REG10C_TIM2_ARR_POS (0)
#define PWM_REG10C_TIM2_ARR_MASK (0xffffffff)

#define PWM_REG10D_ADDR (SOC_PWM_REG_BASE + (0x10d << 2))

#define PWM_REG10D_TIM3_ARR_POS (0)
#define PWM_REG10D_TIM3_ARR_MASK (0xffffffff)

#define PWM_REG10E_ADDR (SOC_PWM_REG_BASE + (0x10e << 2))

#define PWM_REG10E_TIM1_RCR_POS (0)
#define PWM_REG10E_TIM1_RCR_MASK (0xffffffff)

#define PWM_REG10F_ADDR (SOC_PWM_REG_BASE + (0x10f << 2))

#define PWM_REG10F_TIM2_RCR_POS (0)
#define PWM_REG10F_TIM2_RCR_MASK (0xffffffff)

#define PWM_REG110_ADDR (SOC_PWM_REG_BASE + (0x110 << 2))

#define PWM_REG110_TIM3_RCR_POS (0)
#define PWM_REG110_TIM3_RCR_MASK (0xffffffff)

#define PWM_REG111_ADDR (SOC_PWM_REG_BASE + (0x111 << 2))

#define PWM_REG111_CCR1_POS (0)
#define PWM_REG111_CCR1_MASK (0xffffffff)

#define PWM_REG112_ADDR (SOC_PWM_REG_BASE + (0x112 << 2))

#define PWM_REG112_CCR2_POS (0)
#define PWM_REG112_CCR2_MASK (0xffffffff)

#define PWM_REG113_ADDR (SOC_PWM_REG_BASE + (0x113 << 2))

#define PWM_REG113_CCR3_POS (0)
#define PWM_REG113_CCR3_MASK (0xffffffff)

#define PWM_REG114_ADDR (SOC_PWM_REG_BASE + (0x114 << 2))

#define PWM_REG114_CCR4_POS (0)
#define PWM_REG114_CCR4_MASK (0xffffffff)

#define PWM_REG115_ADDR (SOC_PWM_REG_BASE + (0x115 << 2))

#define PWM_REG115_CCR5_POS (0)
#define PWM_REG115_CCR5_MASK (0xffffffff)

#define PWM_REG116_ADDR (SOC_PWM_REG_BASE + (0x116 << 2))

#define PWM_REG116_CCR6_POS (0)
#define PWM_REG116_CCR6_MASK (0xffffffff)

#define PWM_REG117_ADDR (SOC_PWM_REG_BASE + (0x117 << 2))

#define PWM_REG117_CCR7_POS (0)
#define PWM_REG117_CCR7_MASK (0xffffffff)

#define PWM_REG118_ADDR (SOC_PWM_REG_BASE + (0x118 << 2))

#define PWM_REG118_CCR8_POS (0)
#define PWM_REG118_CCR8_MASK (0xffffffff)

#define PWM_REG119_ADDR (SOC_PWM_REG_BASE + (0x119 << 2))

#define PWM_REG119_CCR9_POS (0)
#define PWM_REG119_CCR9_MASK (0xffffffff)

#define PWM_REG11A_ADDR (SOC_PWM_REG_BASE + (0x11a << 2))

#define PWM_REG11A_DT1_POS (0)
#define PWM_REG11A_DT1_MASK (0x3ff)

#define PWM_REG11A_DT2_POS (10)
#define PWM_REG11A_DT2_MASK (0x3ff)

#define PWM_REG11A_DT3_POS (20)
#define PWM_REG11A_DT3_MASK (0x3ff)

#define PWM_REG11A_RESERVED_30_31_POS (30)
#define PWM_REG11A_RESERVED_30_31_MASK (0x3)

#define PWM_REG11B_ADDR (SOC_PWM_REG_BASE + (0x11b << 2))

#define PWM_REG11B_TIM1_ARR_SHAD_POS (0)
#define PWM_REG11B_TIM1_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG11C_ADDR (SOC_PWM_REG_BASE + (0x11c << 2))

#define PWM_REG11C_TIM2_ARR_SHAD_POS (0)
#define PWM_REG11C_TIM2_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG11D_ADDR (SOC_PWM_REG_BASE + (0x11d << 2))

#define PWM_REG11D_TIM3_ARR_SHAD_POS (0)
#define PWM_REG11D_TIM3_ARR_SHAD_MASK (0xffffffff)

#define PWM_REG11E_ADDR (SOC_PWM_REG_BASE + (0x11e << 2))

#define PWM_REG11E_TIM1_RCR_SHAD_POS (0)
#define PWM_REG11E_TIM1_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG11F_ADDR (SOC_PWM_REG_BASE + (0x11f << 2))

#define PWM_REG11F_TIM2_RCR_SHAD_POS (0)
#define PWM_REG11F_TIM2_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG120_ADDR (SOC_PWM_REG_BASE + (0x120 << 2))

#define PWM_REG120_TIM3_RCR_SHAD_POS (0)
#define PWM_REG120_TIM3_RCR_SHAD_MASK (0xffffffff)

#define PWM_REG121_ADDR (SOC_PWM_REG_BASE + (0x121 << 2))

#define PWM_REG121_CCR1_SHAD_POS (0)
#define PWM_REG121_CCR1_SHAD_MASK (0xffffffff)

#define PWM_REG122_ADDR (SOC_PWM_REG_BASE + (0x122 << 2))

#define PWM_REG122_CCR2_SHAD_POS (0)
#define PWM_REG122_CCR2_SHAD_MASK (0xffffffff)

#define PWM_REG123_ADDR (SOC_PWM_REG_BASE + (0x123 << 2))

#define PWM_REG123_CCR3_SHAD_POS (0)
#define PWM_REG123_CCR3_SHAD_MASK (0xffffffff)

#define PWM_REG124_ADDR (SOC_PWM_REG_BASE + (0x124 << 2))

#define PWM_REG124_CCR4_SHAD_POS (0)
#define PWM_REG124_CCR4_SHAD_MASK (0xffffffff)

#define PWM_REG125_ADDR (SOC_PWM_REG_BASE + (0x125 << 2))

#define PWM_REG125_CCR5_SHAD_POS (0)
#define PWM_REG125_CCR5_SHAD_MASK (0xffffffff)

#define PWM_REG126_ADDR (SOC_PWM_REG_BASE + (0x126 << 2))

#define PWM_REG126_CCR6_SHAD_POS (0)
#define PWM_REG126_CCR6_SHAD_MASK (0xffffffff)

#define PWM_REG127_ADDR (SOC_PWM_REG_BASE + (0x127 << 2))

#define PWM_REG127_CCR7_SHAD_POS (0)
#define PWM_REG127_CCR7_SHAD_MASK (0xffffffff)

#define PWM_REG128_ADDR (SOC_PWM_REG_BASE + (0x128 << 2))

#define PWM_REG128_CCR8_SHAD_POS (0)
#define PWM_REG128_CCR8_SHAD_MASK (0xffffffff)

#define PWM_REG129_ADDR (SOC_PWM_REG_BASE + (0x129 << 2))

#define PWM_REG129_CCR9_SHAD_POS (0)
#define PWM_REG129_CCR9_SHAD_MASK (0xffffffff)

#define PWM_REG12A_ADDR (SOC_PWM_REG_BASE + (0x12a << 2))

#define PWM_REG12A_CH6E_SHAD_POS (0)
#define PWM_REG12A_CH6E_SHAD_MASK (0x1)

#define PWM_REG12A_CH5E_SHAD_POS (1)
#define PWM_REG12A_CH5E_SHAD_MASK (0x1)

#define PWM_REG12A_CH4E_SHAD_POS (2)
#define PWM_REG12A_CH4E_SHAD_MASK (0x1)

#define PWM_REG12A_CH3E_SHAD_POS (3)
#define PWM_REG12A_CH3E_SHAD_MASK (0x1)

#define PWM_REG12A_CH2E_SHAD_POS (4)
#define PWM_REG12A_CH2E_SHAD_MASK (0x1)

#define PWM_REG12A_CH1E_SHAD_POS (5)
#define PWM_REG12A_CH1E_SHAD_MASK (0x1)

#define PWM_REG12A_OC1M_SHAD_POS (6)
#define PWM_REG12A_OC1M_SHAD_MASK (0x7)

#define PWM_REG12A_OC2M_SHAD_POS (9)
#define PWM_REG12A_OC2M_SHAD_MASK (0x7)

#define PWM_REG12A_OC3M_SHAD_POS (12)
#define PWM_REG12A_OC3M_SHAD_MASK (0x7)

#define PWM_REG12A_RESERVED_15_31_POS (15)
#define PWM_REG12A_RESERVED_15_31_MASK (0x1ffff)

#ifdef __cplusplus
}
#endif
