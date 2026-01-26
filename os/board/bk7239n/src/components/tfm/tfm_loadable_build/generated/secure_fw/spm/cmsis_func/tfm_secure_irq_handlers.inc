/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#include "psa_manifest/pid.h"


/* Definitions of the signals of the IRQs */
const struct tfm_core_irq_signal_data_t tfm_core_irq_signals[] = {
   {0,0,0}                         /* add dummy element to avoid non-standard empty array */
};

const size_t tfm_core_irq_signals_count = (sizeof(tfm_core_irq_signals) /
                                           sizeof(*tfm_core_irq_signals)) - 1;  /* adjust for the dummy element */

extern void priv_irq_handler_main(uint32_t partition_id,
                                  uint32_t unpriv_handler,
                                  uint32_t irq_signal,
                                  uint32_t irq_line);

/* Forward declarations of unpriv IRQ handlers*/

/* Definitions of privileged IRQ handlers */
