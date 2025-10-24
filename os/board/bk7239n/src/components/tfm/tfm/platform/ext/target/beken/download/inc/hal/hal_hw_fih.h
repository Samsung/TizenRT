// Copyright 2020-2021 Beken
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

#pragma once

#define PRRO_BASE_ADDR                  0x41040000
#define PRRO_REG02                      (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x2*4)))
#define PRRO_REG02_BIT_SOFT_RESET       (1)
#define PRRO_SOFT_RESET                 PRRO_REG02 |= PRRO_REG02_BIT_SOFT_RESET

#define PRRO_REG17_CMP0_ADDR_START      (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x17*4)))
#define PRRO_REG18_CMP0_ADDR_END        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x18*4)))
#define PRRO_REG19_CMP0_DATA_SRC        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x19*4)))
#define PRRO_REG1A_CMP0_DATA_DST        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x1a*4)))

#define PRRO_REG1B_CMP1_ADDR_START      (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x1B*4)))
#define PRRO_REG1C_CMP1_ADDR_END        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x1C*4)))
#define PRRO_REG1D_CMP1_DATA_SRC        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x1D*4)))
#define PRRO_REG1E_CMP1_DATA_DST        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x1E*4)))

#define PRRO_REG1F_CMP2_ADDR_START      (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x1F*4)))
#define PRRO_REG20_CMP2_ADDR_END        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x20*4)))
#define PRRO_REG21_CMP2_DATA_SRC        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x21*4)))
#define PRRO_REG22_CMP2_DATA_DST        (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x22*4)))

#define PRRO_REG23_CMP_INT_STATUS       (*((volatile unsigned int *)(PRRO_BASE_ADDR + 0x23*4)))

enum {
	FIH_DATA_PUBLIC_KEY_HASH,
	FIH_DATA_BOOT_TYPE,
	FIH_DATA_BOOT_FLAG,
	FIH_DATA_LCS,
	FIH_DATA_EFUSE,
	FIH_DATA_INVALID,
};

#define FIH_DATA_PUBLIC_KEY_HASH_MASK (0xFFFF)
#define FIH_DATA_BOOT_TYPE_MASK       (0xF<<16)
#define FIH_DATA_BOOT_FLAG_MASK       (0xF<<20)
#define FIH_DATA_LCS_MASK             (0xF<<24)
#define FIH_DATA_EFUSE_MASK           (0xF<<28)

#if CONFIG_HW_FIH
void bk_fih_init(void);
void bk_fih_set_src(uint32_t id, uint32_t data);
void bk_fih_set_dst(uint32_t id, uint32_t data);
void bk_fih_set_addr_range(uint32_t start, uint32_t end);
void bk_fih_start(void);
void bk_fih_stop(void);
void dump_prro_regs(void);
#else
#define UNUSED_VAR(var) (void)(var)
#define bk_fih_init(...)
#define bk_fih_set_src(id, data) UNUSED_VAR(id); UNUSED_VAR(data)
#define bk_fih_set_dst(id, data) UNUSED_VAR(id); UNUSED_VAR(data)
#define bk_fih_set_addr_range(start, end) UNUSED_VAR(start); UNUSED_VAR(end)
#define bk_fih_start()
#define bk_fih_stop()
#define dump_prro_regs()
#endif
