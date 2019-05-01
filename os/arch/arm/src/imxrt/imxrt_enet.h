/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
/************************************************************************************
 * os/arch/arm/src/imxrt/imxrt_enet.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_ENET_H
#define __ARCH_ARM_SRC_IMXRT_IMXRT_ENET_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include "chip/imxrt_enet.h"

#ifdef CONFIG_IMXRT_ENET

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#define ENET_MSCR_HOLDTIME_1CYCLE  (0 << ENET_MSCR_HOLDTIME_SHIFT)	/* 1 internal module clock cycle */
#define ENET_MSCR_HOLDTIME_2CYCLES (1 << ENET_MSCR_HOLDTIME_SHIFT)	/* 2 internal module clock cycles */
#define ENET_MSCR_HOLDTIME_3CYCLES (2 << ENET_MSCR_HOLDTIME_SHIFT)	/* 3 internal module clock cycles */
#define ENET_MSCR_HOLDTIME_8CYCLES (7 << ENET_MSCR_HOLDTIME_SHIFT)	/* 8 internal module clock cycles */

#define ENET_MMFR_OP_WRNOTMII      (0 << ENET_MMFR_OP_SHIFT)	/* Write frame, not MII compliant */
#define ENET_MMFR_OP_WRMII         (1 << ENET_MMFR_OP_SHIFT)	/* Write frame, MII management frame */
#define ENET_MMFR_OP_RDMII         (2 << ENET_MMFR_OP_SHIFT)	/* Read frame, MII management frame */
#define ENET_MMFR_OP_RdNOTMII      (3 << ENET_MMFR_OP_SHIFT)	/* Read frame, not MII compliant */

/* Definitions for use with imxrt_phy_boardinitialize */

#define EMAC_INTF 0

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Function: up_netinitialize
 *
 * Description:
 *   Initialize the first network interface.  If there are more than one
 *   interface in the chip, then board-specific logic will have to provide
 *   this function to determine which, if any, Ethernet controllers should
 *   be initialized.  Also prototyped in up_internal.h.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 * Assumptions:
 *   Called very early in the initialization sequence.
 *
 ************************************************************************************/

void up_netinitialize(void);

/************************************************************************************
 * Function: imxrt_phy_boardinitialize
 *
 * Description:
 *   Some boards require specialized initialization of the PHY before it can be
 *   used.  This may include such things as configuring GPIOs, resetting the PHY,
 *   etc.  If CONFIG_IMXRT_ENET_PHYINIT is defined in the configuration then the
 *   board specific logic must provide imxrt_phyinitialize();  The i.MX RT Ethernet
 *   driver will call this function one time before it first uses the PHY.
 *
 * Input Parameters:
 *   intf - Always zero for now.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 ************************************************************************************/

#ifdef CONFIG_IMXRT_ENET_PHYINIT
int imxrt_phy_boardinitialize(int intf);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* CONFIG_IMXRT_ENET */
#endif							/* __ARCH_ARM_SRC_IMXRT_IMXRT_ENET_H */
