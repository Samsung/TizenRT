/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * libs/libc/machine/risc-v/asm.h
 *
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 * SPDX-FileCopyrightText: 2017  SiFive Inc. All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the FreeBSD License.   This program is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
 * including the implied warranties of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  A copy of this license is available at
 *  http://www.opensource.org/licenses.
 ****************************************************************************/

#ifndef __LIBS_LIBC_MACHINE_RISCV_GNU_ASM_H
#define __LIBS_LIBC_MACHINE_RISCV_GNU_ASM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_ARCH_RV64
#  define SZREG  8
#  define REG_S sd
#  define REG_L ld
#else
#  define SZREG  4
#  define REG_S sw
#  define REG_L lw
#endif

#ifdef CONFIG_ARCH_QPFPU
#  define SZFREG  16
#  define FREG_S fsq
#  define FREG_L flq
#elif defined(CONFIG_ARCH_DPFPU)
#  define SZFREG   8
#  define FREG_S fsd
#  define FREG_L fld
#elif defined(CONFIG_ARCH_FPU)
#  define SZFREG   4
#  define FREG_S fsw
#  define FREG_L flw
#endif

#endif /* __LIBS_LIBC_MACHINE_RISCV_GNU_ASM_H */
