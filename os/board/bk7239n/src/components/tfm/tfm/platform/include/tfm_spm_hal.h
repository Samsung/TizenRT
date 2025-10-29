/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPM_HAL_H__
#define __TFM_SPM_HAL_H__

#include <stdint.h>
#include "cmsis.h"
#include "fih.h"
#include "tfm_secure_api.h"
#ifdef TFM_MULTI_CORE_TOPOLOGY
#include "tfm_multi_core.h"
#endif
#include "tfm_plat_defs.h"

/*
 * Quantized default IRQ priority, the value is:
 * (Number of configurable priority) / 4: (1UL << __NVIC_PRIO_BITS) / 4
 */
 
 /*Comment: The DEFAULT_IRQ_PRIORITY define at tfm_peripherals_def.h*/
//#define DEFAULT_IRQ_PRIORITY    (1UL << (__NVIC_PRIO_BITS - 2))

/**
 * \brief Holds peripheral specific data fields required to manage the
 *        peripherals isolation
 *
 * This structure has to be defined in the platform directory, and may have
 * different definition for each platform. The structure should contain fields
 * that describe the peripheral for the functions that are prototyped in this
 * file and are responsible for configuring the isolation of the peripherals.
 *
 * Pointers to structures of this type are managed by the SPM, and passed to the
 * necessary function on isolation request. The pointers are also defined by the
 * platform in the header file tfm_peripherals_def.h. For details on this, see
 * the documentation of that file.
 */
struct platform_data_t;

enum irq_target_state_t {
    TFM_IRQ_TARGET_STATE_SECURE,
    TFM_IRQ_TARGET_STATE_NON_SECURE,
};

#ifdef TFM_FIH_PROFILE_ON
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
/**
 * \brief This function initialises the HW used for isolation, and sets the
 *        default configuration for them.
 * This function is called during TF-M core early startup, after DB init
 *
 * \return Returns values as specified by FIH specific platform error code.
 */
fih_int tfm_spm_hal_setup_isolation_hw(void);
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

/**
 * \brief Configure peripherals for a partition based on the platform data and
 *        partition privilege
 *
 * This function is called during partition initialisation (before calling the
 * init function for the partition)
 *
 * \param[in] privileged       Whether the partition is privileged.
 * \param[in] platform_data    The platform fields of the partition DB record to
 *                             be used for configuration.
 *
 * \return Returns values as specified by FIH specific platform error code
 */
fih_int tfm_spm_hal_configure_default_isolation(
                 bool privileged,
                 const struct platform_data_t *platform_data);
#else /* TFM_FIH_PROFILE_ON */
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
/**
 * \brief This function initialises the HW used for isolation, and sets the
 *        default configuration for them.
 * This function is called during TF-M core early startup, after DB init
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t tfm_spm_hal_setup_isolation_hw(void);
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

/**
 * \brief Configure peripherals for a partition based on the platform data and
 *        partition privilege
 *
 * This function is called during partition initialisation (before calling the
 * init function for the partition)
 *
 * \param[in] privileged       Whether the partition is privileged.
 * \param[in] platform_data    The platform fields of the partition DB record to
 *                             be used for configuration.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t tfm_spm_hal_configure_default_isolation(
                 bool priviledged,
                 const struct platform_data_t *platform_data);
#endif /* TFM_FIH_PROFILE_ON */

/**
 * \brief Get the VTOR value of non-secure image
 *
 * \return Returns the address where the vector table of the non-secure image
 *         is located
 */
uint32_t tfm_spm_hal_get_ns_VTOR(void);

/**
 * \brief Get the initial address of non-secure image main stack
 *
 * \return Returns the initial non-secure MSP
 */
uint32_t tfm_spm_hal_get_ns_MSP(void);

/**
 * \brief Get the entry point of the non-secure image
 *
 * \return Returns the address of the non-secure image entry point
 */
uint32_t tfm_spm_hal_get_ns_entry_point(void);

/**
 * \brief Set the priority of a secure IRQ
 *
 * \param[in] irq_line    The IRQ to set the priority for. Might be less than 0
 *
 * \details This function sets the priority for the IRQ passed in the parameter.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 *
 * \note The priority value must be less than the value of PendSV (0x80) and
 *       greater than SVC (0x0).
 *       Platforms are responsible for the priority values assignment to each
 *       IRQ based on their platforms and use cases.
 */
enum tfm_plat_err_t tfm_spm_hal_set_secure_irq_priority(IRQn_Type irq_line);

/**
 * \brief Clears a pending IRQ
 *
 * \param[in] irq_line    The IRQ to clear pending for.
 */
void tfm_spm_hal_clear_pending_irq(IRQn_Type irq_line);

/**
 * \brief Enables an IRQ
 *
 * \param[in] irq_line    The IRQ to be enabled.
 */
void tfm_spm_hal_enable_irq(IRQn_Type irq_line);

/**
 * \brief Disables an IRQ
 *
 * \param[in] irq_line    The IRQ to be disabled
 */
void tfm_spm_hal_disable_irq(IRQn_Type irq_line);

/**
 * \brief Set the target state of an IRQ
 *
 * \param[in] irq_line      The IRQ to set the priority for.
 * \param[in] target_state  Target state to ret for the IRQ.
 *
 * \return                TFM_IRQ_TARGET_STATE_SECURE if interrupt is assigned
 *                        to Secure
 *                        TFM_IRQ_TARGET_STATE_NON_SECURE if interrupt is
 *                        assigned to Non-Secure
 */
enum irq_target_state_t tfm_spm_hal_set_irq_target_state(
                                          IRQn_Type irq_line,
                                          enum irq_target_state_t target_state);

#if !defined(__SAUREGION_PRESENT) || (__SAUREGION_PRESENT == 0)
/**
 * \brief Platform-specific check whether the current partition has access to a memory range
 *
 * The function checks whether the current partition has access to a memory range,
 * taking into consideration the implementation-defined attribution unit that is
 * present on a particular platform.
 *
 * \param[in] p      The start address of the range to check
 * \param[in] s      The size of the range to check
 * \param[in] flags  The flags to pass to the cmse_check_address_range func
 *
 * \return True if the access is granted, false otherwise.
 */
bool tfm_spm_hal_has_access_to_region(const void *p, size_t s, int flags);
#endif /* !defined(__SAUREGION_PRESENT) || (__SAUREGION_PRESENT == 0) */

#endif /* __TFM_SPM_HAL_H__ */
