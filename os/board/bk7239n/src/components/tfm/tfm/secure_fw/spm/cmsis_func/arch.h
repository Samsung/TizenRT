/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include "tfm_api.h"

#ifndef __ARCH_H__
#define __ARCH_H__

/*
 * This function pointer is meant to only hold non secure function pointers.
 * It will be turned into a non-secure one (LSB cleared) before being called
 * whatever happens anyway (unless cast to another function pointer type).
 * Registers will be cleared before branching so that no information leaks
 * from secure to non-secure world.
 */
typedef void (*nsfptr_t) (void) __attribute__((cmse_nonsecure_call));

extern nsfptr_t ns_entry;

/**
 * \brief  Jump to non-secure code.
 */
void jump_to_ns_code(void);

/**
 * \brief Trigger EXC_RETURN
 *
 * \param[in] exc_return The EXC_RETURN value to initiate the return
 *                       procedure
 *
 * \returns              Does not return
 */
void tfm_arch_trigger_exc_return(uint32_t exc_return);

/**
 * \brief Trigger an sfn request/return completion from Thread mode
 *
 * \param[in] res        The tfm_status_e return value to report back to SPM
 * \param[in] exc_return The EXC_RETURN value to use to return from the original
 *                       request/return
 * \param[in] msp        The value of the top of the MSP to reset the MSP before
 *                       trigger the TFM_SVC_SFN_COMPLETION
 *
 * \return               Does not return
 */
void tfm_sfn_completion(enum tfm_status_e res, uint32_t exc_return, uintptr_t msp);

#endif /* __ARCH_H__ */
