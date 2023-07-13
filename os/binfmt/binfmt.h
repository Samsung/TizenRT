/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * os/binfmt/binfmt.h
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __BINFMT_BINFMT_H
#define __BINFMT_BINFMT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <tinyara/binfmt/binfmt.h>
#ifdef CONFIG_ARM_MPU
#include <tinyara/mpu.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* This is a list of registered handlers for different binary formats.  This
 * list should only be accessed by normal user programs.  It should be sufficient
 * protection to simply disable pre-emption when accessing this list.
 */

EXTERN FAR struct binfmt_s *g_binfmts;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: dump_module
 *
 * Description:
 *   Dump the contents of struct binary_s.
 *
 * Input Parameter:
 *   bin      - Load structure
 *
 * Returned Value:
 *   Zero (OK) on success; a negater errno value on failure
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_BINFMT
int dump_module(FAR const struct binary_s *bin);
#else
#define dump_module(bin)
#endif

/****************************************************************************
 * Name: binfmt_copyargv
 *
 * Description:
 *   In the kernel build, the argv list will likely lie in the caller's
 *   address environment and, hence, by inaccessible when we swith to the
 *   address environment of the new process address environment.  So we
 *   do not have any real option other than to copy the callers argv[] list.
 *
 * Input Parameter:
 *   bin      - Load structure
 *   argv     - Argument list
 *
 * Returned Value:
 *   Zero (OK) on sucess; a negater erro value on failure.
 *
 ****************************************************************************/

int binfmt_copyargv(FAR struct binary_s *bin, FAR char *const *argv);

/****************************************************************************
 * Name: binfmt_freeargv
 *
 * Description:
 *   Release the copied argv[] list.
 *
 * Input Parameters:
 *   bin      - Load structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
void binfmt_freeargv(FAR struct binary_s *bin);
#else
#define binfmt_freeargv(bin)
#endif


#ifdef CONFIG_ARM_MPU
static inline void binfmt_set_mpu(struct binary_s *binp)
{
	uint32_t *regs = NULL;
	uint8_t nregion = 0;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (binp->islibrary) {
		regs = binp->cmn_mpu_regs;
		nregion = mpu_get_nregion_info(MPU_REGION_COMMON_BIN);
	} else
#endif
	{
		regs = sched_self()->mpu_regs;
		nregion = mpu_get_nregion_info(MPU_REGION_APP_BIN);
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	/* Configure text section as RO and executable region */
	mpu_get_register_config_value(&regs[0], nregion - 3, (uintptr_t)binp->sections[BIN_TEXT], binp->sizes[BIN_TEXT], true, true);
	/* Configure ro section as RO and non-executable region */
	mpu_get_register_config_value(&regs[3], nregion - 2, (uintptr_t)binp->sections[BIN_RO], binp->sizes[BIN_RO], true, false);
	/* Complete RAM partition will be configured as RW region */
	mpu_get_register_config_value(&regs[6], nregion - 1, (uintptr_t)binp->sections[BIN_DATA], binp->ramsize, false, false);
#else
	/* Complete RAM partition will be configured as RW region */
	mpu_get_register_config_value(&regs[0], nregion - 1, (uintptr_t)binp->ramstart, binp->ramsize, false, true);
#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (binp->islibrary) {
		/* Set MPU register values to real MPU h/w */
		for (int i = 0; i < MPU_REG_NUMBER * NUM_APP_REGIONS; i += MPU_REG_NUMBER) {
			up_mpu_set_register(&regs[i]);
		}
	}
#endif
}
#else
#define binfmt_mpuinit(bin)
#endif


#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __BINFMT_BINFMT_H */
