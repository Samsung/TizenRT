/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CORE_TRUSTZONE_H__
#define __TFM_CORE_TRUSTZONE_H__

#include "security_defs.h"

/* This file holds specification or design defined TrustZone settings. */

/*
 * To avoid faking jump with FNC_RETURN or EXC_RETURN in NSPE, seal the secure
 * stack by putting two words (8 bytes) at the start of stack (higher address)
 * of it. The Armv8-M Architecture Reference Manual recommends to use seal value
 * 0xFEF5EDA5.
 */
#define TFM_STACK_SEALED_SIZE           8
#define TFM_STACK_SEAL_VALUE            STACK_SEAL_PATTERN

/*
 * The numbers in 32bit words while basic FP involved in preempted context:
 * S0 - S15, FPSCR, Reserved
 */
#define TFM_BASIC_FP_CONTEXT_WORDS      18

/*
 * The numbers in 32bit words while addtional FP involved in preempted context:
 * S16 - S31
 */
#define TFM_ADDTIONAL_FP_CONTEXT_WORDS  16

/*
 * SG sets LR[0] to ZERO indicates a non-secure to secure transition.
 * Make sure to apply this mask on a 'LR' just after 'SG' (in veneer).
 */
#define TFM_VENEER_LR_BIT0_MASK         1

#endif /* __TFM_CORE_TRUSTZONE_H__ */
