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

#ifndef _IMXRT_DMAMUX_H_
#define _IMXRT_DMAMUX_H_

#include "imxrt_config.h"
#include "chip/imxrt105x_features.h"

/*!
 * @addtogroup dmamux
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief DMAMUX driver version 2.0.2. */
#define FSL_DMAMUX_DRIVER_VERSION (MAKE_VERSION(2, 0, 2))
/*@}*/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name DMAMUX Initialization and de-initialization
 * @{
 */

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
void imxrt_dmamux_init(DMAMUX_Type *base);

/****************************************************************************
 * Function: imxrt_dmamux_deinit
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
void imxrt_dmamux_deinit(DMAMUX_Type *base);

/* @} */
/*!
 * @name DMAMUX Channel Operation
 * @{
 */

/****************************************************************************
 * Function: imxrt_dmamux_enablechannel
 *
 * Description:
 *   brief Enables the DMAMUX channel.
 * 
 *   This function enables the DMAMUX channel.
 *
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *   channel DMAMUX channel number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_dmamux_enablechannel(DMAMUX_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK;
}

/****************************************************************************
 * Function: imxrt_dmamux_disablechannel
 *
 * Description:
 *   brief Disables the DMAMUX channel.
 * 
 *   This function disables the DMAMUX channel.
 * 
 *   note The user must disable the DMAMUX channel before configuring it.
 *
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *   channel DMAMUX channel number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_dmamux_disablechannel(DMAMUX_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] &= ~DMAMUX_CHCFG_ENBL_MASK;
}

/****************************************************************************
 * Function: imxrt_dmamux_setsource
 *
 * Description:
 *   brief Configures the DMAMUX channel source.
 * 
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *   channel DMAMUX channel number.
 *   source Channel source, which is used to trigger the DMA transfer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_dmamux_setsource(DMAMUX_Type *base, uint32_t channel, uint32_t source)
{
    DEBUGASSERT(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] = ((base->CHCFG[channel] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE(source));
}

#if defined(FSL_FEATURE_DMAMUX_HAS_TRIG) && FSL_FEATURE_DMAMUX_HAS_TRIG > 0U
/****************************************************************************
 * Function: imxrt_dmamux_enableperiodtrigger
 *
 * Description:
 *   brief Enables the DMAMUX period trigger.
 * 
 *   This function enables the DMAMUX period trigger feature.
 * 
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *   channel DMAMUX channel number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_dmamux_enableperiodtrigger(DMAMUX_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] |= DMAMUX_CHCFG_TRIG_MASK;
}

/****************************************************************************
 * Function: imxrt_dmamux_disableperiodtrigger
 *
 * Description:
 *   brief Disables the DMAMUX period trigger.
 * 
 *   This function disables the DMAMUX period trigger.
 * 
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *   channel DMAMUX channel number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_dmamux_disableperiodtrigger(DMAMUX_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->CHCFG[channel] &= ~DMAMUX_CHCFG_TRIG_MASK;
}
#endif /* FSL_FEATURE_DMAMUX_HAS_TRIG */

#if (defined(FSL_FEATURE_DMAMUX_HAS_A_ON) && FSL_FEATURE_DMAMUX_HAS_A_ON)
/****************************************************************************
 * Function: imxrt_dmamux_enablealwayon
 *
 * Description:
 *   brief Enables the DMA channel to be always ON.
 * 
 *   This function enables the DMAMUX channel always ON feature.
 * 
 * Input Parameters:
 *   base DMAMUX peripheral base address.
 *   channel DMAMUX channel number.
 *   param enable Switcher of the always ON feature. "true" means enabled, "false" means disabled.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_dmamux_enablealwayon(DMAMUX_Type *base, uint32_t channel, bool enable)
{
    DEBUGASSERT(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    if (enable) {
        base->CHCFG[channel] |= DMAMUX_CHCFG_A_ON_MASK;
    } else {
        base->CHCFG[channel] &= ~DMAMUX_CHCFG_A_ON_MASK;
    }
}
#endif /* FSL_FEATURE_DMAMUX_HAS_A_ON */

/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* @} */

#endif /* _IMXRT_DMAMUX_H_ */
