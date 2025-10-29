/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cmsis.h"
#include "tfm_hal_interrupt.h"

/*
 * This file contains the interrupt HAL API implementations base on NVIC.
 * Platforms that use NVIC as interrupt controller can use it directly, or have
 * their own implementations.
 */

enum tfm_hal_status_t tfm_hal_irq_enable(uint32_t irq_num)
{
    NVIC_EnableIRQ((IRQn_Type)irq_num);

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_irq_disable(uint32_t irq_num)
{
    NVIC_DisableIRQ((IRQn_Type)irq_num);

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_irq_clear_pending(uint32_t irq_num)
{
    NVIC_ClearPendingIRQ((IRQn_Type)irq_num);

    return TFM_HAL_SUCCESS;
}
