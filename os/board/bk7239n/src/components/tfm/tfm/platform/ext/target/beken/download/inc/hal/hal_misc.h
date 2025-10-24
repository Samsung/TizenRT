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

#include "system.h"
#include "bl_bk_reg.h"

/* Important notice:
 *
 * Currently the digital team has no way to verify the PLL,
 * so we keep all the code same as BK7256, and make the PLL enable an optional feature, it's disabled by default.
 * */

#define addSYSTEM_Reg0x8                                        *((volatile unsigned long *) (0x44010000+0x8*4))

#define addSYSTEM_Reg0x9                                        *((volatile unsigned long *) (0x44010000+0x9*4))

#define set_SYSTEM_Reg0x9_ckdiv_flash(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0xC000000)) | ((val) << 26))

#define set_SYSTEM_Reg0x9_cksel_flash(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3000000)) | ((val) << 24))

#define addSYSTEM_Reg0x10                                       *((volatile unsigned long *) (0x44010000+0x10*4))
#define addSYSTEM_Reg0x40                                       *((volatile unsigned long *) (0x44010000+0x40*4))
#define addSYSTEM_Reg0x45                                       *((volatile unsigned long *) (0x44010000+0x45*4))
#define addSYSTEM_Reg0x46                                       *((volatile unsigned long *) (0x44010000+0x46*4))

//addFLASH_Reg0x4
#define addFLASH_Reg0x4                                         *((volatile unsigned long *) (0x44030000+0x4*4))
#define posFLASH_Reg0x4_addr_sw_reg                             0
#define bitFLASH_Reg0x4_addr_sw_reg                             0xFFFFFF
#define set_FLASH_Reg0x4_addr_sw_reg(val)                       addFLASH_Reg0x4 = ((addFLASH_Reg0x4 & (~0xFFFFFF)) | ((val) << 0))
#define get_FLASH_Reg0x4_addr_sw_reg                            (addFLASH_Reg0x4 & 0xFFFFFF)

#define posFLASH_Reg0x4_op_type_sw                              24
#define bitFLASH_Reg0x4_op_type_sw                              0x1F000000
#define set_FLASH_Reg0x4_op_type_sw(val)                        addFLASH_Reg0x4 = ((addFLASH_Reg0x4 & (~0x1F000000)) | ((val) << 24))
#define get_FLASH_Reg0x4_op_type_sw                             ((addFLASH_Reg0x4 & 0x1F000000) >> 24)

#define posFLASH_Reg0x4_op_sw                                   29
#define bitFLASH_Reg0x4_op_sw                                   0x20000000
#define set_FLASH_Reg0x4_op_sw(val)                             addFLASH_Reg0x4 = ((addFLASH_Reg0x4 & (~0x20000000)) | ((val) << 29))
#define setf_FLASH_Reg0x4_op_sw                                 addFLASH_Reg0x4 |= 0x20000000
#define clrf_FLASH_Reg0x4_op_sw                                 addFLASH_Reg0x4 &= ~0x20000000
#define get_FLASH_Reg0x4_op_sw                                  ((addFLASH_Reg0x4 & 0x20000000) >> 29)

#define posFLASH_Reg0x4_wp_value                                30
#define bitFLASH_Reg0x4_wp_value                                0x40000000
#define set_FLASH_Reg0x4_wp_value(val)                          addFLASH_Reg0x4 = ((addFLASH_Reg0x4 & (~0x40000000)) | ((val) << 30))
#define setf_FLASH_Reg0x4_wp_value                              addFLASH_Reg0x4 |= 0x40000000
#define clrf_FLASH_Reg0x4_wp_value                              addFLASH_Reg0x4 &= ~0x40000000
#define get_FLASH_Reg0x4_wp_value                               ((addFLASH_Reg0x4 & 0x40000000) >> 30)

#define posFLASH_Reg0x4_busy_sw                                 31
#define bitFLASH_Reg0x4_busy_sw                                 0x80000000
#define get_FLASH_Reg0x4_busy_sw                                ((addFLASH_Reg0x4 & 0x80000000) >> 31)


//addFLASH_Reg0xa
#define addFLASH_Reg0xa                                         *((volatile unsigned long *) (0x44030000+0xa*4))
#define posFLASH_Reg0xa_flash_clk_conf                          0
#define bitFLASH_Reg0xa_flash_clk_conf                          0xF
#define set_FLASH_Reg0xa_flash_clk_conf(val)                    addFLASH_Reg0xa = ((addFLASH_Reg0xa & (~0xF)) | ((val) << 0))
#define get_FLASH_Reg0xa_flash_clk_conf                         (addFLASH_Reg0xa & 0xF)

#define posFLASH_Reg0xa_mode_sel                                4
#define bitFLASH_Reg0xa_mode_sel                                0x1F0
#define set_FLASH_Reg0xa_mode_sel(val)                          addFLASH_Reg0xa = ((addFLASH_Reg0xa & (~0x1F0)) | ((val) << 4))
#define get_FLASH_Reg0xa_mode_sel                               ((addFLASH_Reg0xa & 0x1F0) >> 4)

#define posFLASH_Reg0xa_fwren_flash_cpu                         9
#define bitFLASH_Reg0xa_fwren_flash_cpu                         0x200
#define set_FLASH_Reg0xa_fwren_flash_cpu(val)                   addFLASH_Reg0xa = ((addFLASH_Reg0xa & (~0x200)) | ((val) << 9))
#define setf_FLASH_Reg0xa_fwren_flash_cpu                       addFLASH_Reg0xa |= 0x200
#define clrf_FLASH_Reg0xa_fwren_flash_cpu                       addFLASH_Reg0xa &= ~0x200
#define get_FLASH_Reg0xa_fwren_flash_cpu                        ((addFLASH_Reg0xa & 0x200) >> 9)

#define posFLASH_Reg0xa_wrsr_data                               10
#define bitFLASH_Reg0xa_wrsr_data                               0x3FFFC00
#define set_FLASH_Reg0xa_wrsr_data(val)                         addFLASH_Reg0xa = ((addFLASH_Reg0xa & (~0x3FFFC00)) | ((val) << 10))
#define get_FLASH_Reg0xa_wrsr_data                              ((addFLASH_Reg0xa & 0x3FFFC00) >> 10)

#define posFLASH_Reg0xa_crc_en                                  26
#define bitFLASH_Reg0xa_crc_en                                  0x4000000
#define set_FLASH_Reg0xa_crc_en(val)                            addFLASH_Reg0xa = ((addFLASH_Reg0xa & (~0x4000000)) | ((val) << 26))
#define setf_FLASH_Reg0xa_crc_en                                addFLASH_Reg0xa |= 0x4000000
#define clrf_FLASH_Reg0xa_crc_en                                addFLASH_Reg0xa &= ~0x4000000
#define get_FLASH_Reg0xa_crc_en                                 ((addFLASH_Reg0xa & 0x4000000) >> 26)


#define addFLASH_Reg0x15                                         *((volatile unsigned long *) (0x44030000+0x15*4))
#define posFLASH_Reg0x15_op_type_sw                              24

typedef enum {
	CLOCK_XTAL_26M = 1,
	CLOCK_PLL_60M,
} flash_clock_t;

void hal_enable_pll(void);
void hal_disable_pll(void);
void hal_enable_high_freq(void);
void hal_set_flash_clock(flash_clock_t clock);
void dump_sys_regs(void);

