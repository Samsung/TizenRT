/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2020 STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BOOT_HAL_H__
#define __BOOT_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_compiler.h"

/* Include header section */

#ifdef __cplusplus
extern "C" {
#endif

struct boot_arm_vector_table {
    uint32_t msp;
    uint32_t reset;
};

/*
 * \brief It clears that part of the RAM which was used by MCUBoot, expect the
 *        TFM_SHARED_DATA area, which is used to pass data to the TF-M runtime.
 *
 * \note  This function must be implemented per target platform by system
 *        integrator. If the bootloader has not loaded any secret to the shared
 *        RAM then this function can immediately return to shorten the boot-up
 *        time. Clearing RAM area can be done several way, it is platform
 *        dependent:
 *        - Overwritten with a pre-defined constant value (i.e.: 0).
 *        - Overwritten with a random value.
 *        - Change the secret if its location is known.
 *        - Set a register which can hide some part of the flash/RAM against
 *          next stage software components.
 *        - Etc.
 */
void boot_clear_ram_area(void);

/*
 * \brief Chain-loading the next image in the boot sequence.
 *        Can be overridden for platform specific initialization.
 * \param[in] reset_handler_addr Address of next image's Reset_Handler() in
                                 the boot chain (TF-M SPE, etc.)
 */
void boot_jump_to_next_image(uint32_t reset_handler_addr) __NO_RETURN;

/**
 * \brief Platform peripherals and devices initialization.
 *        Can be overridden for platform specific initialization.
 *
 * \return Returns 0 on success, non-zero otherwise
 */
int32_t boot_platform_init(void);

/**
 * \brief Perform later platform specific initialization. Runs at the end of the
 *        boot initialization phase, for platform-specific code that depends on
 *        other initialization code being run beforehand.
 *
 * \return Returns 0 on success, non-zero otherwise
 */
int32_t boot_platform_post_init(void);

/**
 * \brief Platform operation to start secure image.
 *        Can be overridden for platform specific initialization.
 *
 * \param[in] vt  pointer to secure application vector table descriptor
 */
void boot_platform_quit(struct boot_arm_vector_table *vt) __NO_RETURN;

bool is_secureboot(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_HAL_H__ */
