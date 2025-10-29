/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "spm_ipc.h"

#include "load/interrupt_defs.h"
#include "load/partition_defs.h"
#include "psa/service.h"

/**
 * \brief Return the IRQ load info context pointer associated with a signal
 *
 * \param[in]      p_ldinf      The load info of the partition in which we look
 *                              for the signal.
 * \param[in]      signal       The signal to query for.
 *
 * \retval NULL                 if one of more the following are true:
 *                              - the \ref signal indicates more than one signal
 *                              - the \ref signal does not belong to the
 *                                partition.
 * \retval Any other value      The load info pointer associated with the signal
 */
struct irq_load_info_t *get_irq_info_for_signal(
                                    const struct partition_load_info_t *p_ldinf,
                                    psa_signal_t signal);

/**
 * \brief Entry of Secure interrupt handler. Platforms can call this function to
 *        handle individual interrupts.
 *
 * \param[in] p_pt         The owner Partition of the interrupt to handle
 * \param[in] p_ildi       The irq_load_info_t struct of the interrupt to handle
 *
 * Note:
 *  The input parameters are maintained by platforms and they must be init-ed
 *  in the interrupt init functions.
 */
void spm_handle_interrupt(void *p_pt, struct irq_load_info_t *p_ildi);

/*
 * Prepare execution context for deprivileged FLIH functions
 * Parameters:
 *      p_owner_sp - IRQ owner partition_t pointer
 *      flih_func  - The FLIH Function
 */
uint32_t tfm_flih_prepare_depriv_flih(struct partition_t *p_owner_sp,
                                      uintptr_t flih_func);

/*
 * Go back to ISR from FLIH functions
 * Parameters:
 *      result - The return value of the FLIH Function, indicating to the SPM
 *               how to complete the FLIH processing, for example setting signal
 *               to the Secure Partition.
 *      p_ctx_flih_ret - The contents on the Main Stack when this function is
 *                       called. It is used to restore context.
 */
uint32_t tfm_flih_return_to_isr(psa_flih_result_t result,
                                struct context_flih_ret_t *p_ctx_flih_ret);
