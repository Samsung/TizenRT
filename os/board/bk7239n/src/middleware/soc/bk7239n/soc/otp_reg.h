// Copyright 2022-2023 Beken
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
#if 0
#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#define OTP__ADDR (SOC_OTP_REG_BASE + (0x0 << 2))

#define OTP__SHFWEN_POS (0)
#define OTP__SHFWEN_MASK (0xf)

#define OTP__SHFREN_POS (4)
#define OTP__SHFREN_MASK (0xf)

#define OTP__PUFLCK_POS (8)
#define OTP__PUFLCK_MASK (0xf)

#define OTP__TMLCK_POS (12)
#define OTP__TMLCK_MASK (0xf)

#define OTP__ENROLL_POS (16)
#define OTP__ENROLL_MASK (0xf)

#define OTP__OTPLCK_POS (20)
#define OTP__OTPLCK_MASK (0xf)

#define OTP__RESERVED_24_27_POS (24)
#define OTP__RESERVED_24_27_MASK (0xf)

#define OTP__PGMPRT_POS (28)
#define OTP__PGMPRT_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x4 << 2))

#define OTP__ROLCK_CDE_000_POS (0)
#define OTP__ROLCK_CDE_000_MASK (0xf)

#define OTP__ROLCK_CDE_001_POS (4)
#define OTP__ROLCK_CDE_001_MASK (0xf)

#define OTP__ROLCK_CDE_002_POS (8)
#define OTP__ROLCK_CDE_002_MASK (0xf)

#define OTP__ROLCK_CDE_003_POS (12)
#define OTP__ROLCK_CDE_003_MASK (0xf)

#define OTP__ROLCK_CDE_004_POS (16)
#define OTP__ROLCK_CDE_004_MASK (0xf)

#define OTP__ROLCK_CDE_005_POS (20)
#define OTP__ROLCK_CDE_005_MASK (0xf)

#define OTP__ROLCK_CDE_006_POS (24)
#define OTP__ROLCK_CDE_006_MASK (0xf)

#define OTP__ROLCK_CDE_007_POS (28)
#define OTP__ROLCK_CDE_007_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x5 << 2))

#define OTP__ROLCK_CDE_008_POS (0)
#define OTP__ROLCK_CDE_008_MASK (0xf)

#define OTP__ROLCK_CDE_009_POS (4)
#define OTP__ROLCK_CDE_009_MASK (0xf)

#define OTP__ROLCK_CDE_010_POS (8)
#define OTP__ROLCK_CDE_010_MASK (0xf)

#define OTP__ROLCK_CDE_011_POS (12)
#define OTP__ROLCK_CDE_011_MASK (0xf)

#define OTP__ROLCK_CDE_012_POS (16)
#define OTP__ROLCK_CDE_012_MASK (0xf)

#define OTP__ROLCK_CDE_013_POS (20)
#define OTP__ROLCK_CDE_013_MASK (0xf)

#define OTP__ROLCK_CDE_014_POS (24)
#define OTP__ROLCK_CDE_014_MASK (0xf)

#define OTP__ROLCK_CDE_015_POS (28)
#define OTP__ROLCK_CDE_015_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x6 << 2))

#define OTP__ROLCK_CDE_016_POS (0)
#define OTP__ROLCK_CDE_016_MASK (0xf)

#define OTP__ROLCK_CDE_017_POS (4)
#define OTP__ROLCK_CDE_017_MASK (0xf)

#define OTP__ROLCK_CDE_018_POS (8)
#define OTP__ROLCK_CDE_018_MASK (0xf)

#define OTP__ROLCK_CDE_019_POS (12)
#define OTP__ROLCK_CDE_019_MASK (0xf)

#define OTP__ROLCK_CDE_020_POS (16)
#define OTP__ROLCK_CDE_020_MASK (0xf)

#define OTP__ROLCK_CDE_021_POS (20)
#define OTP__ROLCK_CDE_021_MASK (0xf)

#define OTP__ROLCK_CDE_022_POS (24)
#define OTP__ROLCK_CDE_022_MASK (0xf)

#define OTP__ROLCK_CDE_023_POS (28)
#define OTP__ROLCK_CDE_023_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8 << 2))

#define OTP__SECR_CTL_OTP_0_POS (0)
#define OTP__SECR_CTL_OTP_0_MASK (0x7)

#define OTP__SECR_CTL_OTP_1_POS (3)
#define OTP__SECR_CTL_OTP_1_MASK (0x7)

#define OTP__SECR_CTL_OTP_2_POS (6)
#define OTP__SECR_CTL_OTP_2_MASK (0x7)

#define OTP__SECR_CTL_OTP_3_POS (9)
#define OTP__SECR_CTL_OTP_3_MASK (0x7)

#define OTP__SECR_CTL_PUF_0_POS (12)
#define OTP__SECR_CTL_PUF_0_MASK (0x7)

#define OTP__SECR_CTL_PUF_1_POS (15)
#define OTP__SECR_CTL_PUF_1_MASK (0x7)

#define OTP__SECR_CTL_PUF_2_POS (18)
#define OTP__SECR_CTL_PUF_2_MASK (0x7)

#define OTP__SECR_CTL_PUF_3_POS (21)
#define OTP__SECR_CTL_PUF_3_MASK (0x7)

#define OTP__SECR_ZEROIZE_POS (24)
#define OTP__SECR_ZEROIZE_MASK (0x7)

#define OTP__RESERVED_27_27_POS (27)
#define OTP__RESERVED_27_27_MASK (0x1)

#define OTP__SECR_PROT_EN_POS (28)
#define OTP__SECR_PROT_EN_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xc << 2))

#define OTP__ZEROIZED_PUF_POS (0)
#define OTP__ZEROIZED_PUF_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xe << 2))

#define OTP__ZEROIZED_OTP0_POS (0)
#define OTP__ZEROIZED_OTP0_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xf << 2))

#define OTP__ZEROIZED_OTP1_POS (0)
#define OTP__ZEROIZED_OTP1_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x1c << 2))

#define OTP__PUF_LCKWD_000_POS (0)
#define OTP__PUF_LCKWD_000_MASK (0xf)

#define OTP__PUF_LCKWD_001_POS (4)
#define OTP__PUF_LCKWD_001_MASK (0xf)

#define OTP__PUF_LCKWD_002_POS (8)
#define OTP__PUF_LCKWD_002_MASK (0xf)

#define OTP__PUF_LCKWD_003_POS (12)
#define OTP__PUF_LCKWD_003_MASK (0xf)

#define OTP__PUF_LCKWD_004_POS (16)
#define OTP__PUF_LCKWD_004_MASK (0xf)

#define OTP__PUF_LCKWD_005_POS (20)
#define OTP__PUF_LCKWD_005_MASK (0xf)

#define OTP__PUF_LCKWD_006_POS (24)
#define OTP__PUF_LCKWD_006_MASK (0xf)

#define OTP__PUF_LCKWD_007_POS (28)
#define OTP__PUF_LCKWD_007_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x1d << 2))

#define OTP__PUF_LCKWD_008_POS (0)
#define OTP__PUF_LCKWD_008_MASK (0xf)

#define OTP__PUF_LCKWD_009_POS (4)
#define OTP__PUF_LCKWD_009_MASK (0xf)

#define OTP__PUF_LCKWD_010_POS (8)
#define OTP__PUF_LCKWD_010_MASK (0xf)

#define OTP__PUF_LCKWD_011_POS (12)
#define OTP__PUF_LCKWD_011_MASK (0xf)

#define OTP__PUF_LCKWD_012_POS (16)
#define OTP__PUF_LCKWD_012_MASK (0xf)

#define OTP__PUF_LCKWD_013_POS (20)
#define OTP__PUF_LCKWD_013_MASK (0xf)

#define OTP__PUF_LCKWD_014_POS (24)
#define OTP__PUF_LCKWD_014_MASK (0xf)

#define OTP__PUF_LCKWD_015_POS (28)
#define OTP__PUF_LCKWD_015_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x1e << 2))

#define OTP__PUF_LCKWD_016_POS (0)
#define OTP__PUF_LCKWD_016_MASK (0xf)

#define OTP__PUF_LCKWD_017_POS (4)
#define OTP__PUF_LCKWD_017_MASK (0xf)

#define OTP__PUF_LCKWD_018_POS (8)
#define OTP__PUF_LCKWD_018_MASK (0xf)

#define OTP__PUF_LCKWD_019_POS (12)
#define OTP__PUF_LCKWD_019_MASK (0xf)

#define OTP__PUF_LCKWD_020_POS (16)
#define OTP__PUF_LCKWD_020_MASK (0xf)

#define OTP__PUF_LCKWD_021_POS (20)
#define OTP__PUF_LCKWD_021_MASK (0xf)

#define OTP__PUF_LCKWD_022_POS (24)
#define OTP__PUF_LCKWD_022_MASK (0xf)

#define OTP__PUF_LCKWD_023_POS (28)
#define OTP__PUF_LCKWD_023_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x1f << 2))

#define OTP__PUF_LCKWD_024_POS (0)
#define OTP__PUF_LCKWD_024_MASK (0xf)

#define OTP__PUF_LCKWD_025_POS (4)
#define OTP__PUF_LCKWD_025_MASK (0xf)

#define OTP__PUF_LCKWD_026_POS (8)
#define OTP__PUF_LCKWD_026_MASK (0xf)

#define OTP__PUF_LCKWD_027_POS (12)
#define OTP__PUF_LCKWD_027_MASK (0xf)

#define OTP__PUF_LCKWD_028_POS (16)
#define OTP__PUF_LCKWD_028_MASK (0xf)

#define OTP__PUF_LCKWD_029_POS (20)
#define OTP__PUF_LCKWD_029_MASK (0xf)

#define OTP__PUF_LCKWD_030_POS (24)
#define OTP__PUF_LCKWD_030_MASK (0xf)

#define OTP__PUF_LCKWD_031_POS (28)
#define OTP__PUF_LCKWD_031_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x20 << 2))

#define OTP__PUF_LCKWD_000_POS (0)
#define OTP__PUF_LCKWD_000_MASK (0xf)

#define OTP__PUF_LCKWD_001_POS (4)
#define OTP__PUF_LCKWD_001_MASK (0xf)

#define OTP__PUF_LCKWD_002_POS (8)
#define OTP__PUF_LCKWD_002_MASK (0xf)

#define OTP__PUF_LCKWD_003_POS (12)
#define OTP__PUF_LCKWD_003_MASK (0xf)

#define OTP__PUF_LCKWD_004_POS (16)
#define OTP__PUF_LCKWD_004_MASK (0xf)

#define OTP__PUF_LCKWD_005_POS (20)
#define OTP__PUF_LCKWD_005_MASK (0xf)

#define OTP__PUF_LCKWD_006_POS (24)
#define OTP__PUF_LCKWD_006_MASK (0xf)

#define OTP__PUF_LCKWD_007_POS (28)
#define OTP__PUF_LCKWD_007_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x21 << 2))

#define OTP__PUF_LCKWD_008_POS (0)
#define OTP__PUF_LCKWD_008_MASK (0xf)

#define OTP__PUF_LCKWD_009_POS (4)
#define OTP__PUF_LCKWD_009_MASK (0xf)

#define OTP__PUF_LCKWD_010_POS (8)
#define OTP__PUF_LCKWD_010_MASK (0xf)

#define OTP__PUF_LCKWD_011_POS (12)
#define OTP__PUF_LCKWD_011_MASK (0xf)

#define OTP__PUF_LCKWD_012_POS (16)
#define OTP__PUF_LCKWD_012_MASK (0xf)

#define OTP__PUF_LCKWD_013_POS (20)
#define OTP__PUF_LCKWD_013_MASK (0xf)

#define OTP__PUF_LCKWD_014_POS (24)
#define OTP__PUF_LCKWD_014_MASK (0xf)

#define OTP__PUF_LCKWD_015_POS (28)
#define OTP__PUF_LCKWD_015_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x3e << 2))

#define OTP__PUF_LCKWD_240_POS (0)
#define OTP__PUF_LCKWD_240_MASK (0xf)

#define OTP__PUF_LCKWD_241_POS (4)
#define OTP__PUF_LCKWD_241_MASK (0xf)

#define OTP__PUF_LCKWD_242_POS (8)
#define OTP__PUF_LCKWD_242_MASK (0xf)

#define OTP__PUF_LCKWD_243_POS (12)
#define OTP__PUF_LCKWD_243_MASK (0xf)

#define OTP__PUF_LCKWD_244_POS (16)
#define OTP__PUF_LCKWD_244_MASK (0xf)

#define OTP__PUF_LCKWD_245_POS (20)
#define OTP__PUF_LCKWD_245_MASK (0xf)

#define OTP__PUF_LCKWD_246_POS (24)
#define OTP__PUF_LCKWD_246_MASK (0xf)

#define OTP__PUF_LCKWD_247_POS (28)
#define OTP__PUF_LCKWD_247_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x3f << 2))

#define OTP__PUF_LCKWD_248_POS (0)
#define OTP__PUF_LCKWD_248_MASK (0xf)

#define OTP__PUF_LCKWD_249_POS (4)
#define OTP__PUF_LCKWD_249_MASK (0xf)

#define OTP__PUF_LCKWD_250_POS (8)
#define OTP__PUF_LCKWD_250_MASK (0xf)

#define OTP__PUF_LCKWD_251_POS (12)
#define OTP__PUF_LCKWD_251_MASK (0xf)

#define OTP__PUF_LCKWD_252_POS (16)
#define OTP__PUF_LCKWD_252_MASK (0xf)

#define OTP__PUF_LCKWD_253_POS (20)
#define OTP__PUF_LCKWD_253_MASK (0xf)

#define OTP__PUF_LCKWD_254_POS (24)
#define OTP__PUF_LCKWD_254_MASK (0xf)

#define OTP__PUF_LCKWD_255_POS (28)
#define OTP__PUF_LCKWD_255_MASK (0xf)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x80 << 2))

#define OTP__PTR_000_POS (0)
#define OTP__PTR_000_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x81 << 2))

#define OTP__PTR_001_POS (0)
#define OTP__PTR_001_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x82 << 2))

#define OTP__PTR_002_POS (0)
#define OTP__PTR_002_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x83 << 2))

#define OTP__PTR_003_POS (0)
#define OTP__PTR_003_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x84 << 2))

#define OTP__PTR_004_POS (0)
#define OTP__PTR_004_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x85 << 2))

#define OTP__PTR_005_POS (0)
#define OTP__PTR_005_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x86 << 2))

#define OTP__PTR_006_POS (0)
#define OTP__PTR_006_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x87 << 2))

#define OTP__PTR_007_POS (0)
#define OTP__PTR_007_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x88 << 2))

#define OTP__PTR_008_POS (0)
#define OTP__PTR_008_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x89 << 2))

#define OTP__PTR_009_POS (0)
#define OTP__PTR_009_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8a << 2))

#define OTP__PTR_010_POS (0)
#define OTP__PTR_010_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8b << 2))

#define OTP__PTR_011_POS (0)
#define OTP__PTR_011_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8c << 2))

#define OTP__PTR_012_POS (0)
#define OTP__PTR_012_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8d << 2))

#define OTP__PTR_013_POS (0)
#define OTP__PTR_013_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8e << 2))

#define OTP__PTR_014_POS (0)
#define OTP__PTR_014_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x8f << 2))

#define OTP__PTR_015_POS (0)
#define OTP__PTR_015_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x90 << 2))

#define OTP__PTC_000_POS (0)
#define OTP__PTC_000_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x91 << 2))

#define OTP__PTC_001_POS (0)
#define OTP__PTC_001_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x92 << 2))

#define OTP__PTC_002_POS (0)
#define OTP__PTC_002_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x93 << 2))

#define OTP__PTC_003_POS (0)
#define OTP__PTC_003_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x94 << 2))

#define OTP__PTC_004_POS (0)
#define OTP__PTC_004_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x95 << 2))

#define OTP__PTC_005_POS (0)
#define OTP__PTC_005_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x96 << 2))

#define OTP__PTC_006_POS (0)
#define OTP__PTC_006_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x97 << 2))

#define OTP__PTC_007_POS (0)
#define OTP__PTC_007_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x98 << 2))

#define OTP__PTC_008_POS (0)
#define OTP__PTC_008_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x99 << 2))

#define OTP__PTC_009_POS (0)
#define OTP__PTC_009_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x9a << 2))

#define OTP__PTC_010_POS (0)
#define OTP__PTC_010_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x9b << 2))

#define OTP__PTC_011_POS (0)
#define OTP__PTC_011_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x9c << 2))

#define OTP__PTC_012_POS (0)
#define OTP__PTC_012_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x9d << 2))

#define OTP__PTC_013_POS (0)
#define OTP__PTC_013_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x9e << 2))

#define OTP__PTC_014_POS (0)
#define OTP__PTC_014_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x9f << 2))

#define OTP__PTC_015_POS (0)
#define OTP__PTC_015_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xa0 << 2))

#define OTP__CDE_PSMSK_0_POS (0)
#define OTP__CDE_PSMSK_0_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xa1 << 2))

#define OTP__CDE_PSMSK_1_POS (0)
#define OTP__CDE_PSMSK_1_MASK (0xffff)

#define OTP__RESERVED_16_31_POS (16)
#define OTP__RESERVED_16_31_MASK (0xffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xa8 << 2))

#define OTP__RANDOM_VALUE_POS (0)
#define OTP__RANDOM_VALUE_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xac << 2))

#define OTP__INTRPT_ST_POS (0)
#define OTP__INTRPT_ST_MASK (0x3ff)

#define OTP__RESERVED_10_15_POS (10)
#define OTP__RESERVED_10_15_MASK (0x3f)

#define OTP__INTRPT_EN_POS (16)
#define OTP__INTRPT_EN_MASK (0x3ff)

#define OTP__RESERVED_26_31_POS (26)
#define OTP__RESERVED_26_31_MASK (0x3f)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xad << 2))

#define OTP__OTP_PSMSK_0_POS (0)
#define OTP__OTP_PSMSK_0_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xae << 2))

#define OTP__OTP_PSMSK_1_POS (0)
#define OTP__OTP_PSMSK_1_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xaf << 2))

#define OTP__PUF_PSMSK_POS (0)
#define OTP__PUF_PSMSK_MASK (0xff)

#define OTP__RESERVED_8_23_POS (8)
#define OTP__RESERVED_8_23_MASK (0xffff)

#define OTP__LCK_PSMSK_POS (24)
#define OTP__LCK_PSMSK_MASK (0xff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb0 << 2))

#define OTP__VERSION_POS (0)
#define OTP__VERSION_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb1 << 2))

#define OTP__BUSY_POS (0)
#define OTP__BUSY_MASK (0x1)

#define OTP__ERROR_POS (1)
#define OTP__ERROR_MASK (0x1)

#define OTP__WARNING_POS (2)
#define OTP__WARNING_MASK (0x1)

#define OTP__WRONG_POS (3)
#define OTP__WRONG_MASK (0x1)

#define OTP__FORBID_POS (4)
#define OTP__FORBID_MASK (0x1)

#define OTP__RESERVED_5_7_POS (5)
#define OTP__RESERVED_5_7_MASK (0x7)

#define OTP__FAILCNT_POS (8)
#define OTP__FAILCNT_MASK (0xf)

#define OTP__RESERVED_12_31_POS (12)
#define OTP__RESERVED_12_31_MASK (0xfffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb2 << 2))

#define OTP__PDSTB_POS (0)
#define OTP__PDSTB_MASK (0x1)

#define OTP__CLKOSC_EN_POS (1)
#define OTP__CLKOSC_EN_MASK (0x1)

#define OTP__RESERVED_2_3_POS (2)
#define OTP__RESERVED_2_3_MASK (0x3)

#define OTP__RD_MODE_POS (4)
#define OTP__RD_MODE_MASK (0x3)

#define OTP__RD_TO_SEL_POS (6)
#define OTP__RD_TO_SEL_MASK (0x3)

#define OTP__FRE_CONT_POS (8)
#define OTP__FRE_CONT_MASK (0x1)

#define OTP__RESERVED_9_10_POS (9)
#define OTP__RESERVED_9_10_MASK (0x3)

#define OTP__HT_RD_POS (11)
#define OTP__HT_RD_MASK (0x1)

#define OTP__RESERVED_12_23_POS (12)
#define OTP__RESERVED_12_23_MASK (0xfff)

#define OTP__PTC_PAGE_POS (24)
#define OTP__PTC_PAGE_MASK (0xff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb4 << 2))

#define OTP__AUTO_REPAIR_POS (0)
#define OTP__AUTO_REPAIR_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb5 << 2))

#define OTP__INI_OFF_CHK_POS (0)
#define OTP__INI_OFF_CHK_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb8 << 2))

#define OTP__PUF_QTY_CHK_POS (0)
#define OTP__PUF_QTY_CHK_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xb9 << 2))

#define OTP__PUF_ENROLL_POS (0)
#define OTP__PUF_ENROLL_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xba << 2))

#define OTP__PUF_ZEROIZE_POS (0)
#define OTP__PUF_ZEROIZE_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xbb << 2))

#define OTP__SET_FLAG_POS (0)
#define OTP__SET_FLAG_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xbc << 2))

#define OTP__OTP_ZEROIZE_POS (0)
#define OTP__OTP_ZEROIZE_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xc0 << 2))

#define OTP__PUF_000_POS (0)
#define OTP__PUF_000_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xc1 << 2))

#define OTP__PUF_001_POS (0)
#define OTP__PUF_001_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xdf << 2))

#define OTP__PUF_031_POS (0)
#define OTP__PUF_031_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xe0 << 2))

#define OTP__PUF_032_POS (0)
#define OTP__PUF_032_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xe1 << 2))

#define OTP__PUF_033_POS (0)
#define OTP__PUF_033_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0xff << 2))

#define OTP__PUF_063_POS (0)
#define OTP__PUF_063_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x100 << 2))

#define OTP__OTP_000_POS (0)
#define OTP__OTP_000_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x101 << 2))

#define OTP__OTP_001_POS (0)
#define OTP__OTP_001_MASK (0xffffffff)

#define OTP__ADDR (SOC_OTP_REG_BASE + (0x1ff << 2))

#define OTP__OTP_255_POS (0)
#define OTP__OTP_255_MASK (0xffffffff)

#ifdef __cplusplus
}
#endif
#endif