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
#include "imxrt_clock.h"
#include "imxrt_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get instance number for DMAMUX.
 *
 * @param base DMAMUX peripheral base address.
 */
static uint32_t imxrt_dmamux_getinstance(DMAMUX_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map DMAMUX instance number to base pointer. */
static DMAMUX_Type *const s_dmamuxBases[] = DMAMUX_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map DMAMUX instance number to clock name. */
static const clock_ip_name_t s_dmamuxClockName[] = DMAMUX_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
/****************************************************************************
 * Function: imxrt_dmamux_getinstance
 *
 * Description:
 *   brief Get instance number for DMAMUX.
 *
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *
 * Returned Value:
 *   Return the instance value
 *
 ****************************************************************************/
static uint32_t imxrt_dmamux_getinstance(DMAMUX_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_dmamuxBases); instance++) {
        if (s_dmamuxBases[instance] == base) {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_dmamuxBases));

    return instance;
}

/****************************************************************************
 * Function: imxrt_dmamux_init
 *
 * Description:
 *   brief Initializes the DMAMUX peripheral.
 *
 *   This function ungates the DMAMUX clock.
 *
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_dmamux_init(DMAMUX_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    imxrt_clock_enableclock(s_dmamuxClockName[imxrt_dmamux_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/****************************************************************************
 * Function: imxrt_dmamux_getinstance
 *
 * Description:
 *   brief Deinitializes the DMAMUX peripheral.
 *
 *   This function gates the DMAMUX clock.
 *
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_dmamux_deinit(DMAMUX_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    imxrt_clock_disableclock(s_dmamuxClockName[imxrt_dmamux_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}
