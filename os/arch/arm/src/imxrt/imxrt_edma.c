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
/****************************************************************************
 * os/arch/arm/src/imxrt/imxrt_edma.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Portions of the eDMA logic derive from NXP sample code which has a compatible
 * BSD 3-clause license:
 *
 *   Copyright (c) 2015, Freescale Semiconductor, Inc.
 *   Copyright 2016-2017 NXP
 *   All rights reserved
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <queue.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>

#include "up_arch.h"
#include "cache.h"
#include "up_internal.h"
#include "sched/sched.h"

#include "chip.h"
#include "chip/imxrt_edma.h"
#include "chip/imxrt_dmamux.h"
#include "imxrt_periphclks.h"
#include "imxrt_edma.h"
#include "imxrt_clock.h"

#ifdef CONFIG_IMXRT_EDMA

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EDMA_TRANSFER_ENABLED_MASK 0x80U

#undef dmaerr
#undef dmainfo
#if defined(DEBUG_IMX_DMA_DRIVER)
#define dmainfo(format, ...)   printf(format, ##__VA_ARGS__)
#define dmaerr(format, ...)   printf(format, ##__VA_ARGS__)
#else
#define dmainfo(format, ...)
//#define dmaerr(format, ...)
#endif
#define dmaerr(format, ...)   printf(format, ##__VA_ARGS__)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get instance number for EDMA.
 *
 * @param base EDMA peripheral base address.
 */
static uint32_t imxrt_edma_getinstance(DMA_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map EDMA instance number to base pointer. */
static DMA_Type *const s_edmaBases[] = DMA_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map EDMA instance number to clock name. */
static const clock_ip_name_t s_edmaClockName[] = EDMA_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*! @brief Array to map EDMA instance number to IRQ number. */
static const IRQn_Type s_edmaIRQNumber[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;

/*! @brief Pointers to transfer handle for each EDMA channel. */
static edma_handle_t *s_EDMAHandle[FSL_FEATURE_EDMA_MODULE_CHANNEL * FSL_FEATURE_SOC_EDMA_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/
/****************************************************************************
 * Function: imxrt_edma_getinstance
 *
 * Description:
 *   brief Get instance number for EDMA.
 *
 * Input Parameters:
 *   base EDMA peripheral base address.
 *
 * Returned Value:
 *   Return the instance value
 *
 ****************************************************************************/
static uint32_t imxrt_edma_getinstance(DMA_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_edmaBases); instance++) {
        if (s_edmaBases[instance] == base) {
            break;
        }
    }

    DEBUGASSERT(instance < ARRAY_SIZE(s_edmaBases));

    return instance;
}

/****************************************************************************
 * Function: imxrt_edma_getinstance
 *
 * Description:
 *   brief Push content of TCD structure into hardware TCD register.
 *
 * Input Parameters:
 *   base EDMA peripheral base address.
 *   channel EDMA channel number.
 *   tcd Point to TCD structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_installtcd(DMA_Type *base, uint32_t channel, edma_tcd_t *tcd)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    DEBUGASSERT(tcd != NULL);
    DEBUGASSERT(((uint32_t)tcd & 0x1FU) == 0);

    /* Push tcd into hardware TCD register */
    base->TCD[channel].SADDR = tcd->SADDR;
    base->TCD[channel].SOFF = tcd->SOFF;
    base->TCD[channel].ATTR = tcd->ATTR;
    base->TCD[channel].NBYTES_MLNO = tcd->NBYTES;
    base->TCD[channel].SLAST = tcd->SLAST;
    base->TCD[channel].DADDR = tcd->DADDR;
    base->TCD[channel].DOFF = tcd->DOFF;
    base->TCD[channel].CITER_ELINKNO = tcd->CITER;
    base->TCD[channel].DLAST_SGA = tcd->DLAST_SGA;
    /* Clear DONE bit first, otherwise ESG cannot be set */
    base->TCD[channel].CSR = 0;
    base->TCD[channel].CSR = tcd->CSR;
    base->TCD[channel].BITER_ELINKNO = tcd->BITER;
}

/****************************************************************************
 * Function: imxrt_edma_init
 *
 * Description:
 *   brief Initializes the eDMA peripheral.
 *
 *   This function ungates the eDMA clock and configures the eDMA peripheral according
 *   to the configuration structure.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   config A pointer to the configuration structure, see "edma_config_t".
 *   note This function enables the minor loop map feature.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_init(DMA_Type *base, const edma_config_t *config)
{
    DEBUGASSERT(config != NULL);

    uint32_t tmpreg;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Ungate EDMA peripheral clock */
    imxrt_clock_enableclock(s_edmaClockName[imxrt_edma_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* clear all the enabled request, status to make sure EDMA status is in normal condition */
    base->ERQ = 0U;
    base->INT = 0xFFFFFFFFU;
    base->ERR = 0xFFFFFFFFU;
    /* Configure EDMA peripheral according to the configuration structure. */
    tmpreg = base->CR;
    tmpreg &= ~(DMA_CR_ERCA_MASK | DMA_CR_HOE_MASK | DMA_CR_CLM_MASK | DMA_CR_EDBG_MASK);
    tmpreg |= (DMA_CR_ERCA(config->enableRoundRobinArbitration) | DMA_CR_HOE(config->enableHaltOnError) |
               DMA_CR_CLM(config->enableContinuousLinkMode) | DMA_CR_EDBG(config->enableDebugMode) | DMA_CR_EMLM(true));
    base->CR = tmpreg;
}

/****************************************************************************
 * Function: imxrt_edma_deinit
 *
 * Description:
 *   brief Deinitializes the eDMA peripheral.
 *
 *   This function gates the eDMA clock.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_deinit(DMA_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Gate EDMA peripheral clock */
    imxrt_clock_disableclock(s_edmaClockName[imxrt_edma_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/****************************************************************************
 * Function: imxrt_edma_getdefaultconfig
 *
 * Description:
 *   brief Gets the eDMA default configuration structure.
 *
 *   This function sets the configuration structure to default values.
 *   The default configuration is set to the following values.
 *   code
 *     config.enableContinuousLinkMode = false;
 *     config.enableHaltOnError = true;
 *     config.enableRoundRobinArbitration = false;
 *     config.enableDebugMode = false;
 *   endcode
 *
 * Input Parameters:
 *   config A pointer to the eDMA configuration structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_getdefaultconfig(edma_config_t *config)
{
    DEBUGASSERT(config != NULL);

    /* Initializes the configure structure to zero. */
    memset(config, 0, sizeof(*config));

    config->enableRoundRobinArbitration = false;
    config->enableHaltOnError = true;
    config->enableContinuousLinkMode = false;
    config->enableDebugMode = false;
}

/****************************************************************************
 * Function: imxrt_edma_resetchannel
 *
 * Description:
 *   brief Sets all TCD registers to default values.
 *
 *   This function sets TCD registers for this channel to default values.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   note This function must not be called while the channel transfer is ongoing
 *     or it causes unpredictable results.
 *   note This function enables the auto stop request feature.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_resetchannel(DMA_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    imxrt_edma_tcdreset((edma_tcd_t *)&base->TCD[channel]);
}

/****************************************************************************
 * Function: imxrt_edma_settransferconfig
 *
 * Description:
 *   brief Configures the eDMA transfer attribute.
 *
 *   This function configures the transfer attribute, including source address, destination address,
 *   transfer size, address offset, and so on. It also configures the scatter gather feature if the
 *   user supplies the TCD address.
 *   Example:
 *   code
 *    edma_transfer_t config;
 *    edma_tcd_t tcd;
 *    config.srcAddr = ..;
 *    config.destAddr = ..;
 *    ...
 *    EDMA_SetTransferConfig(DMA0, channel, &config, &stcd);
 *   endcode
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   config Pointer to eDMA transfer configuration structure.
 *   nextTcd Point to TCD structure. It can be NULL if users
 *              do not want to enable scatter/gather feature.
 *   note If nextTcd is not NULL, it means scatter gather feature is enabled
 *       and DREQ bit is cleared in the previous transfer configuration, which
 *       is set in the eDMA_ResetChannel.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_settransferconfig(DMA_Type *base, uint32_t channel, const edma_transfer_config_t *config, edma_tcd_t *nextTcd)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    DEBUGASSERT(config != NULL);
    DEBUGASSERT(((uint32_t)nextTcd & 0x1FU) == 0);

    imxrt_edma_tcdsettransferconfig((edma_tcd_t *)&base->TCD[channel], config, nextTcd);
}

/****************************************************************************
 * Function: imxrt_edma_setminoroffsetconfig
 *
 * Description:
 *   brief Configures the eDMA minor offset feature.
 *
 *   The minor offset means that the signed-extended value is added to the source address or destination
 *   address after each minor loop.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   config A pointer to the minor offset configuration structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_setminoroffsetconfig(DMA_Type *base, uint32_t channel, const edma_minor_offset_config_t *config)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    DEBUGASSERT(config != NULL);

    uint32_t tmpreg;

    tmpreg = base->TCD[channel].NBYTES_MLOFFYES;
    tmpreg &= ~(DMA_NBYTES_MLOFFYES_SMLOE_MASK | DMA_NBYTES_MLOFFYES_DMLOE_MASK | DMA_NBYTES_MLOFFYES_MLOFF_MASK);
    tmpreg |=
        (DMA_NBYTES_MLOFFYES_SMLOE(config->enableSrcMinorOffset) |
         DMA_NBYTES_MLOFFYES_DMLOE(config->enableDestMinorOffset) | DMA_NBYTES_MLOFFYES_MLOFF(config->minorOffset));
    base->TCD[channel].NBYTES_MLOFFYES = tmpreg;
}

/****************************************************************************
 * Function: imxrt_edma_setchannellink
 *
 * Description:
 *   brief Sets the channel link for the eDMA transfer.
 *
 *   This function configures either the minor link or the major link mode. The minor link means that the channel link is
 *   triggered every time CITER decreases by 1. The major link means that the channel link is triggered when the CITER is
 *   exhausted.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   type A channel link type, which can be one of the following:
 *     arg kEDMA_LinkNone
 *     arg kEDMA_MinorLink
 *     arg kEDMA_MajorLink
 *   linkedChannel The linked channel number.
 *   note Users should ensure that DONE flag is cleared before calling this interface, or the configuration is invalid.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_setchannellink(DMA_Type *base, uint32_t channel, edma_channel_link_type_t type, uint32_t linkedChannel)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    DEBUGASSERT(linkedChannel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    imxrt_edma_tcdsetchannellink((edma_tcd_t *)&base->TCD[channel], type, linkedChannel);
}

/****************************************************************************
 * Function: imxrt_edma_setbandwidth
 *
 * Description:
 *   brief Sets the bandwidth for the eDMA transfer.
 *
 *   Because the eDMA processes the minor loop, it continuously generates read/write sequences
 *   until the minor count is exhausted. The bandwidth forces the eDMA to stall after the completion of
 *   each read/write access to control the bus request bandwidth seen by the crossbar switch.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   bandWidth A bandwidth setting, which can be one of the following:
 *     arg kEDMABandwidthStallNone
 *     arg kEDMABandwidthStall4Cycle
 *     arg kEDMABandwidthStall8Cycle
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_setbandwidth(DMA_Type *base, uint32_t channel, edma_bandwidth_t bandWidth)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    base->TCD[channel].CSR = (base->TCD[channel].CSR & (~DMA_CSR_BWC_MASK)) | DMA_CSR_BWC(bandWidth);
}

/****************************************************************************
 * Function: imxrt_edma_setmodulo
 *
 * Description:
 *   brief Sets the source modulo and the destination modulo for the eDMA transfer.
 *
 *   This function defines a specific address range specified to be the value after (SADDR + SOFF)/(DADDR + DOFF)
 *   calculation is performed or the original register value. It provides the ability to implement a circular data
 *   queue easily.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   srcModulo A source modulo value.
 *   destModulo A destination modulo value.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_setmodulo(DMA_Type *base, uint32_t channel, edma_modulo_t srcModulo, edma_modulo_t destModulo)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t tmpreg;

    tmpreg = base->TCD[channel].ATTR & (~(DMA_ATTR_SMOD_MASK | DMA_ATTR_DMOD_MASK));
    base->TCD[channel].ATTR = tmpreg | DMA_ATTR_DMOD(destModulo) | DMA_ATTR_SMOD(srcModulo);
}

/****************************************************************************
 * Function: imxrt_edma_enablechannelinterrupts
 *
 * Description:
 *   brief Enables the interrupt source for the eDMA transfer.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   mask The mask of interrupt source to be set. Users need to use
 *             the defined edma_interrupt_enable_t type.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_enablechannelinterrupts(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    /* Enable error interrupt */
    if (mask & kEDMA_ErrorInterruptEnable) {
        base->EEI |= (0x1U << channel);
    }

    /* Enable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable) {
        base->TCD[channel].CSR |= DMA_CSR_INTMAJOR_MASK;
    }

    /* Enable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable) {
        base->TCD[channel].CSR |= DMA_CSR_INTHALF_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_edma_disablechannelinterrupts
 *
 * Description:
 *   brief Disables the interrupt source for the eDMA transfer.
 *
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   mask The mask of the interrupt source to be set. Use
 *             the defined edma_interrupt_enable_t type.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_disablechannelinterrupts(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    /* Disable error interrupt */
    if (mask & kEDMA_ErrorInterruptEnable) {
        base->EEI &= ~(0x1U << channel);
    }

    /* Disable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable) {
        base->TCD[channel].CSR &= ~DMA_CSR_INTMAJOR_MASK;
    }

    /* Disable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable) {
        base->TCD[channel].CSR &= ~DMA_CSR_INTHALF_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_edma_tcdreset
 *
 * Description:
 *   brief Sets all fields to default values for the TCD structure.
 *
 *   This function sets all fields for this TCD structure to default value.
 * 
 * Input Parameters:
 *   tcd Pointer to the TCD structure.
 *   note This function enables the auto stop request feature.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcdreset(edma_tcd_t *tcd)
{
    DEBUGASSERT(tcd != NULL);
    DEBUGASSERT(((uint32_t)tcd & 0x1FU) == 0);

    /* Reset channel TCD */
    tcd->SADDR = 0U;
    tcd->SOFF = 0U;
    tcd->ATTR = 0U;
    tcd->NBYTES = 0U;
    tcd->SLAST = 0U;
    tcd->DADDR = 0U;
    tcd->DOFF = 0U;
    tcd->CITER = 0U;
    tcd->DLAST_SGA = 0U;
    /* Enable auto disable request feature */
    tcd->CSR = DMA_CSR_DREQ(true);
    tcd->BITER = 0U;
}

/****************************************************************************
 * Function: imxrt_edma_tcdsettransferconfig
 *
 * Description:
 *   brief Configures the eDMA TCD transfer attribute.
 *
 *   The TCD is a transfer control descriptor. The content of the TCD is the same as the hardware TCD registers.
 *   The STCD is used in the scatter-gather mode.
 *   This function configures the TCD transfer attribute, including source address, destination address,
 *   transfer size, address offset, and so on. It also configures the scatter gather feature if the
 *   user supplies the next TCD address.
 *   Example:
 *   code
 *     edma_transfer_t config = {
 *     ...
 *     }
 *     edma_tcd_t tcd __aligned(32);
 *     edma_tcd_t nextTcd __aligned(32);
 *     EDMA_TcdSetTransferConfig(&tcd, &config, &nextTcd);
 *   endcode
 * 
 * Input Parameters:
 *   tcd Pointer to the TCD structure.
 *   config Pointer to eDMA transfer configuration structure.
 *   nextTcd Pointer to the next TCD structure. It can be NULL if users
 *                do not want to enable scatter/gather feature.
 *   note TCD address should be 32 bytes aligned or it causes an eDMA error.
 *   note If the nextTcd is not NULL, the scatter gather feature is enabled
 *       and DREQ bit is cleared in the previous transfer configuration, which
 *       is set in the EDMA_TcdReset.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcdsettransferconfig(edma_tcd_t *tcd, const edma_transfer_config_t *config, edma_tcd_t *nextTcd)
{
    DEBUGASSERT(tcd != NULL);
    DEBUGASSERT(((uint32_t)tcd & 0x1FU) == 0);
    DEBUGASSERT(config != NULL);
    DEBUGASSERT(((uint32_t)nextTcd & 0x1FU) == 0);

    /* source address */
    tcd->SADDR = config->srcAddr;
    /* destination address */
    tcd->DADDR = config->destAddr;
    /* Source data and destination data transfer size */
    tcd->ATTR = DMA_ATTR_SSIZE(config->srcTransferSize) | DMA_ATTR_DSIZE(config->destTransferSize);
    /* Source address signed offset */
    tcd->SOFF = config->srcOffset;
    /* Destination address signed offset */
    tcd->DOFF = config->destOffset;
    /* Minor byte transfer count */
    tcd->NBYTES = config->minorLoopBytes;
    /* Current major iteration count */
    tcd->CITER = config->majorLoopCounts;
    /* Starting major iteration count */
    tcd->BITER = config->majorLoopCounts;
    /* Enable scatter/gather processing */
    if (nextTcd != NULL) {
        tcd->DLAST_SGA = (uint32_t)nextTcd;
        /*
            Before call EDMA_TcdSetTransferConfig or EDMA_SetTransferConfig,
            user must call EDMA_TcdReset or EDMA_ResetChannel which will set
            DREQ, so must use "|" or "&" rather than "=".

            Clear the DREQ bit because scatter gather has been enabled, so the
            previous transfer is not the last transfer, and channel request should
            be enabled at the next transfer(the next TCD).
        */
        tcd->CSR = (tcd->CSR | DMA_CSR_ESG_MASK) & ~DMA_CSR_DREQ_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_edma_tcdsetminoroffsetconfig
 *
 * Description:
 *   brief Configures the eDMA TCD minor offset feature.
 *
 *   A minor offset is a signed-extended value added to the source address or a destination
 *   address after each minor loop.
 * 
 * Input Parameters:
 *   tcd A point to the TCD structure.
 *   config A pointer to the minor offset configuration structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcdsetminoroffsetconfig(edma_tcd_t *tcd, const edma_minor_offset_config_t *config)
{
    DEBUGASSERT(tcd != NULL);
    DEBUGASSERT(((uint32_t)tcd & 0x1FU) == 0);

    uint32_t tmpreg;

    tmpreg = tcd->NBYTES &
             ~(DMA_NBYTES_MLOFFYES_SMLOE_MASK | DMA_NBYTES_MLOFFYES_DMLOE_MASK | DMA_NBYTES_MLOFFYES_MLOFF_MASK);
    tmpreg |=
        (DMA_NBYTES_MLOFFYES_SMLOE(config->enableSrcMinorOffset) |
         DMA_NBYTES_MLOFFYES_DMLOE(config->enableDestMinorOffset) | DMA_NBYTES_MLOFFYES_MLOFF(config->minorOffset));
    tcd->NBYTES = tmpreg;
}

/****************************************************************************
 * Function: imxrt_edma_tcdsetminoroffsetconfig
 *
 * Description:
 *   brief Sets the channel link for the eDMA TCD.
 *
 *   This function configures either a minor link or a major link. The minor link means the channel link is
 *   triggered every time CITER decreases by 1. The major link means that the channel link  is triggered when the CITER is
 *   exhausted.
 * 
 *   note Users should ensure that DONE flag is cleared before calling this interface, or the configuration is invalid.
 * 
 * Input Parameters:
 *   tcd Point to the TCD structure.
 *   type Channel link type, it can be one of:
 *     arg kEDMA_LinkNone
 *     arg kEDMA_MinorLink
 *     arg kEDMA_MajorLink
 *   linkedChannel The linked channel number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcdsetchannellink(edma_tcd_t *tcd, edma_channel_link_type_t type, uint32_t linkedChannel)
{
    DEBUGASSERT(tcd != NULL);
    DEBUGASSERT(((uint32_t)tcd & 0x1FU) == 0);
    DEBUGASSERT(linkedChannel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    if (type == kEDMA_MinorLink) { /* Minor link config */
        uint32_t tmpreg;

        /* Enable minor link */
        tcd->CITER |= DMA_CITER_ELINKYES_ELINK_MASK;
        tcd->BITER |= DMA_BITER_ELINKYES_ELINK_MASK;
        /* Set linked channel */
        tmpreg = tcd->CITER & (~DMA_CITER_ELINKYES_LINKCH_MASK);
        tmpreg |= DMA_CITER_ELINKYES_LINKCH(linkedChannel);
        tcd->CITER = tmpreg;
        tmpreg = tcd->BITER & (~DMA_BITER_ELINKYES_LINKCH_MASK);
        tmpreg |= DMA_BITER_ELINKYES_LINKCH(linkedChannel);
        tcd->BITER = tmpreg;
    } else if (type == kEDMA_MajorLink) { /* Major link config */
        uint32_t tmpreg;

        /* Enable major link */
        tcd->CSR |= DMA_CSR_MAJORELINK_MASK;
        /* Set major linked channel */
        tmpreg = tcd->CSR & (~DMA_CSR_MAJORLINKCH_MASK);
        tcd->CSR = tmpreg | DMA_CSR_MAJORLINKCH(linkedChannel);
    } else { /* Link none */
        tcd->CITER &= ~DMA_CITER_ELINKYES_ELINK_MASK;
        tcd->BITER &= ~DMA_BITER_ELINKYES_ELINK_MASK;
        tcd->CSR &= ~DMA_CSR_MAJORELINK_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_edma_tcdsetmodulo
 *
 * Description:
 *   brief Sets the source modulo and the destination modulo for the eDMA TCD.
 *
 *   This function defines a specific address range specified to be the value after (SADDR + SOFF)/(DADDR + DOFF)
 *   calculation is performed or the original register value. It provides the ability to implement a circular data
 *   queue easily.
 * 
 * Input Parameters:
 *   tcd A pointer to the TCD structure.
 *   srcModulo A source modulo value.
 *   destModulo A destination modulo value.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcdsetmodulo(edma_tcd_t *tcd, edma_modulo_t srcModulo, edma_modulo_t destModulo)
{
    DEBUGASSERT(tcd != NULL);
    DEBUGASSERT(((uint32_t)tcd & 0x1FU) == 0);

    uint32_t tmpreg;

    tmpreg = tcd->ATTR & (~(DMA_ATTR_SMOD_MASK | DMA_ATTR_DMOD_MASK));
    tcd->ATTR = tmpreg | DMA_ATTR_DMOD(destModulo) | DMA_ATTR_SMOD(srcModulo);
}

/****************************************************************************
 * Function: imxrt_edma_tcdenableinterrupts
 *
 * Description:
 *   brief Enables the interrupt source for the eDMA TCD.
 * 
 * Input Parameters:
 *   tcd Point to the TCD structure.
 *   mask The mask of interrupt source to be set. Users need to use
 *             the defined edma_interrupt_enable_t type.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcdenableinterrupts(edma_tcd_t *tcd, uint32_t mask)
{
    DEBUGASSERT(tcd != NULL);

    /* Enable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable) {
        tcd->CSR |= DMA_CSR_INTMAJOR_MASK;
    }

    /* Enable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable) {
        tcd->CSR |= DMA_CSR_INTHALF_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_edma_tcddisableinterrupts
 *
 * Description:
 *   brief Disables the interrupt source for the eDMA TCD.
 * 
 * Input Parameters:
 *   tcd Point to the TCD structure.
 *   mask The mask of interrupt source to be set. Users need to use
 *             the defined edma_interrupt_enable_t type.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_tcddisableinterrupts(edma_tcd_t *tcd, uint32_t mask)
{
    DEBUGASSERT(tcd != NULL);

    /* Disable Major interrupt */
    if (mask & kEDMA_MajorInterruptEnable) {
        tcd->CSR &= ~DMA_CSR_INTMAJOR_MASK;
    }

    /* Disable Half major interrupt */
    if (mask & kEDMA_HalfInterruptEnable) {
        tcd->CSR &= ~DMA_CSR_INTHALF_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_edma_getremainingmajorloopcount
 *
 * Description:
 *   brief Gets the remaining major loop count from the eDMA current channel TCD.
 *
 *   This function checks the TCD (Task Control Descriptor) status for a specified
 *   eDMA channel and returns the number of major loop count that has not finished.
 * 
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *
 * Returned Value:
 *   Returns Major loop count which has not been transferred yet for the current TCD.
 *   note 1. This function can only be used to get unfinished major loop count of transfer without
 *           the next TCD, or it might be inaccuracy.
 *        2. The unfinished/remaining transfer bytes cannot be obtained directly from registers while
 *           the channel is running.
 *           Because to calculate the remaining bytes, the initial NBYTES configured in DMA_TCDn_NBYTES_MLNO
 *           register is needed while the eDMA IP does not support getting it while a channel is active.
 *           In another word, the NBYTES value reading is always the actual (decrementing) NBYTES value the dma_engine
 *           is working with while a channel is running.
 *           Consequently, to get the remaining transfer bytes, a software-saved initial value of NBYTES (for example
 *           copied before enabling the channel) is needed. The formula to calculate it is shown below:
 *           RemainingBytes = RemainingMajorLoopCount * NBYTES(initially configured)
 *
 ****************************************************************************/
uint32_t imxrt_edma_getremainingmajorloopcount(DMA_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t remainingCount = 0;

    if (DMA_CSR_DONE_MASK & base->TCD[channel].CSR) {
        remainingCount = 0;
    } else {
        /* Calculate the unfinished bytes */
        if (base->TCD[channel].CITER_ELINKNO & DMA_CITER_ELINKNO_ELINK_MASK) {
            remainingCount =
                (base->TCD[channel].CITER_ELINKYES & DMA_CITER_ELINKYES_CITER_MASK) >> DMA_CITER_ELINKYES_CITER_SHIFT;
        } else {
            remainingCount =
                (base->TCD[channel].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK) >> DMA_CITER_ELINKNO_CITER_SHIFT;
        }
    }

    return remainingCount;
}

/****************************************************************************
 * Function: imxrt_imxrt_edma_getchannelstatusflags
 *
 * Description:
 *   brief Gets the eDMA channel status flags.
 * 
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *
 * Returned Value:
 *   Returns The mask of channel status flags. Users need to use the
 *         _edma_channel_status_flags type to decode the return variables.
 *
 ****************************************************************************/
uint32_t imxrt_imxrt_edma_getchannelstatusflags(DMA_Type *base, uint32_t channel)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t retval = 0;

    /* Get DONE bit flag */
    retval |= ((base->TCD[channel].CSR & DMA_CSR_DONE_MASK) >> DMA_CSR_DONE_SHIFT);
    /* Get ERROR bit flag */
    retval |= (((base->ERR >> channel) & 0x1U) << 1U);
    /* Get INT bit flag */
    retval |= (((base->INT >> channel) & 0x1U) << 2U);

    return retval;
}

/****************************************************************************
 * Function: imxrt_edma_clearchannelstatusflags
 *
 * Description:
 *   brief Clears the eDMA channel status flags.
 * 
 * Input Parameters:
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *   mask The mask of channel status to be cleared. Users need to use
 *             the defined _edma_channel_status_flags type.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_clearchannelstatusflags(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    /* Clear DONE bit flag */
    if (mask & kEDMA_DoneFlag) {
        base->CDNE = channel;
    }
    /* Clear ERROR bit flag */
    if (mask & kEDMA_ErrorFlag) {
        base->CERR = channel;
    }
    /* Clear INT bit flag */
    if (mask & kEDMA_InterruptFlag) {
        base->CINT = channel;
    }
}

/****************************************************************************
 * Function: imxrt_get_startinstance
 *
 * Description:
 *   brief Get start instance
 * 
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Returns the number of start instance
 *
 ****************************************************************************/
static uint8_t imxrt_get_startinstance(void)
{
    static uint8_t StartInstanceNum;

#if defined(DMA0)
    StartInstanceNum = imxrt_edma_getinstance(DMA0);
#elif defined(DMA1)
    StartInstanceNum = imxrt_edma_getinstance(DMA1);
#elif defined(DMA2)
    StartInstanceNum = imxrt_edma_getinstance(DMA2);
#elif defined(DMA3)
    StartInstanceNum = imxrt_edma_getinstance(DMA3);
#endif

    return StartInstanceNum;
}

/****************************************************************************
 * Function: imxrt_edma_createhandle
 *
 * Description:
 *   brief Creates the eDMA handle.
 *
 *   This function is called if using the transactional API for eDMA. This function
 *   initializes the internal state of the eDMA handle.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer. The eDMA handle stores callback function and
 *               parameters.
 *   base eDMA peripheral base address.
 *   channel eDMA channel number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_createhandle(edma_handle_t *handle, DMA_Type *base, uint32_t channel)
{
    DEBUGASSERT(handle != NULL);
    DEBUGASSERT(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);

    uint32_t edmaInstance;
    uint32_t channelIndex;
    uint8_t StartInstance;
    edma_tcd_t *tcdRegs;

    /* Zero the handle */
    memset(handle, 0, sizeof(*handle));

    handle->base = base;
    handle->channel = channel;
    /* Get the DMA instance number */
    edmaInstance = imxrt_edma_getinstance(base);
    StartInstance = imxrt_get_startinstance();
    channelIndex = ((edmaInstance - StartInstance) * FSL_FEATURE_EDMA_MODULE_CHANNEL) + channel;
    s_EDMAHandle[channelIndex] = handle;

    /* Enable NVIC interrupt */
    up_enable_irq(s_edmaIRQNumber[edmaInstance][channel]);

    /*
       Reset TCD registers to zero. Unlike the EDMA_TcdReset(DREQ will be set),
       CSR will be 0. Because in order to suit EDMA busy check mechanism in
       EDMA_SubmitTransfer, CSR must be set 0.
    */
    tcdRegs = (edma_tcd_t *)&handle->base->TCD[handle->channel];
    tcdRegs->SADDR = 0;
    tcdRegs->SOFF = 0;
    tcdRegs->ATTR = 0;
    tcdRegs->NBYTES = 0;
    tcdRegs->SLAST = 0;
    tcdRegs->DADDR = 0;
    tcdRegs->DOFF = 0;
    tcdRegs->CITER = 0;
    tcdRegs->DLAST_SGA = 0;
    tcdRegs->CSR = 0;
    tcdRegs->BITER = 0;
}

/****************************************************************************
 * Function: imxrt_edma_installtcdmemory
 *
 * Description:
 *   brief Installs the TCDs memory pool into the eDMA handle.
 *
 *   This function is called after the EDMA_CreateHandle to use scatter/gather feature. This function shall only be used
 *   while users need to use scatter gather mode. Scatter gather mode enables EDMA to load a new transfer control block
 *   (tcd) in hardware, and automatically reconfigure that DMA channel for a new transfer.
 *   Users need to prepare tcd memory and also configure tcds using interface EDMA_SubmitTransfer.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *   tcdPool A memory pool to store TCDs. It must be 32 bytes aligned.
 *   tcdSize The number of TCD slots.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_installtcdmemory(edma_handle_t *handle, edma_tcd_t *tcdPool, uint32_t tcdSize)
{
    DEBUGASSERT(handle != NULL);
    DEBUGASSERT(((uint32_t)tcdPool & 0x1FU) == 0);

    /* Initialize tcd queue attribute. */
    handle->header = 0;
    handle->tail = 0;
    handle->tcdUsed = 0;
    handle->tcdSize = tcdSize;
    handle->flags = 0;
    handle->tcdPool = tcdPool;
}

/****************************************************************************
 * Function: imxrt_edma_setcallback
 *
 * Description:
 *   brief Installs a callback function for the eDMA transfer.
 *
 *   This callback is called in the eDMA IRQ handler. Use the callback to do something after
 *   the current major loop transfer completes. This function will be called every time one tcd finished transfer.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *   callback eDMA callback function pointer.
 *   userData A parameter for the callback function.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_setcallback(edma_handle_t *handle, edma_callback callback, void *userData)
{
    DEBUGASSERT(handle != NULL);

    handle->callback = callback;
    handle->userData = userData;
}

/****************************************************************************
 * Function: imxrt_edma_preparetransfer
 *
 * Description:
 *   brief Prepares the eDMA transfer structure.
 *
 *   This function prepares the transfer configuration structure according to the user input.
 * 
 * Input Parameters:
 *   config The user configuration structure of type edma_transfer_t.
 *   srcAddr eDMA transfer source address.
 *   srcWidth eDMA transfer source address width(bytes).
 *   destAddr eDMA transfer destination address.
 *   destWidth eDMA transfer destination address width(bytes).
 *   bytesEachRequest eDMA transfer bytes per channel request.
 *   transferBytes eDMA transfer bytes to be transferred.
 *   type eDMA transfer type.
 *   note The data address and the data width must be consistent. For example, if the SRC
 *       is 4 bytes, the source address must be 4 bytes aligned, or it results in
 *       source address error (SAE).
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_preparetransfer(edma_transfer_config_t *config,
                          void *srcAddr,
                          uint32_t srcWidth,
                          void *destAddr,
                          uint32_t destWidth,
                          uint32_t bytesEachRequest,
                          uint32_t transferBytes,
                          edma_transfer_type_t type)
{
    DEBUGASSERT(config != NULL);
    DEBUGASSERT(srcAddr != NULL);
    DEBUGASSERT(destAddr != NULL);
    DEBUGASSERT((srcWidth == 1U) || (srcWidth == 2U) || (srcWidth == 4U) || (srcWidth == 16U) || (srcWidth == 32U));
    DEBUGASSERT((destWidth == 1U) || (destWidth == 2U) || (destWidth == 4U) || (destWidth == 16U) || (destWidth == 32U));
    DEBUGASSERT(transferBytes % bytesEachRequest == 0);

    /* Initializes the configure structure to zero. */
    memset(config, 0, sizeof(*config));

    config->destAddr = (uint32_t)destAddr;
    config->srcAddr = (uint32_t)srcAddr;
    config->minorLoopBytes = bytesEachRequest;
    config->majorLoopCounts = transferBytes / bytesEachRequest;
    switch (srcWidth) {
    case 1U:
        config->srcTransferSize = kEDMA_TransferSize1Bytes;
        break;
    case 2U:
        config->srcTransferSize = kEDMA_TransferSize2Bytes;
        break;
    case 4U:
        config->srcTransferSize = kEDMA_TransferSize4Bytes;
        break;
    case 16U:
        config->srcTransferSize = kEDMA_TransferSize16Bytes;
        break;
    case 32U:
        config->srcTransferSize = kEDMA_TransferSize32Bytes;
        break;
    default:
        break;
    }
    switch (destWidth) {
    case 1U:
        config->destTransferSize = kEDMA_TransferSize1Bytes;
        break;
    case 2U:
        config->destTransferSize = kEDMA_TransferSize2Bytes;
        break;
    case 4U:
        config->destTransferSize = kEDMA_TransferSize4Bytes;
        break;
    case 16U:
        config->destTransferSize = kEDMA_TransferSize16Bytes;
        break;
    case 32U:
        config->destTransferSize = kEDMA_TransferSize32Bytes;
        break;
    default:
        break;
    }
    switch (type) {
    case kEDMA_MemoryToMemory:
        config->destOffset = destWidth;
        config->srcOffset = srcWidth;
        break;
    case kEDMA_MemoryToPeripheral:
        config->destOffset = 0U;
        config->srcOffset = srcWidth;
        break;
    case kEDMA_PeripheralToMemory:
        config->destOffset = destWidth;
        config->srcOffset = 0U;
        break;
    default:
        break;
    }
}

/****************************************************************************
 * Function: imxrt_edma_submittransfer
 *
 * Description:
 *   brief Submits the eDMA transfer request.
 *
 *   This function submits the eDMA transfer request according to the transfer configuration structure.
 *   In scatter gather mode, call this function will add a configured tcd to the circular list of tcd pool.
 *   The tcd pools is setup by call function EDMA_InstallTCDMemory before.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *   config Pointer to eDMA transfer configuration structure.
 *
 * Returned Value:
 *   Returns kStatus_EDMA_Success It means submit transfer request succeed.
 *   Returns kStatus_EDMA_QueueFull It means TCD queue is full. Submit transfer request is not allowed.
 *   Returns kStatus_EDMA_Busy It means the given channel is busy, need to submit request later.
 *
 ****************************************************************************/
status_t imxrt_edma_submittransfer(edma_handle_t *handle, const edma_transfer_config_t *config)
{
    DEBUGASSERT(handle != NULL);
    DEBUGASSERT(config != NULL);

    edma_tcd_t *tcdRegs = (edma_tcd_t *)&handle->base->TCD[handle->channel];

    if (handle->tcdPool == NULL) {
        /*
            Check if EDMA is busy: if the given channel started transfer, CSR will be not zero. Because
            if it is the last transfer, DREQ will be set. If not, ESG will be set. So in order to suit
            this check mechanism, EDMA_CreatHandle will clear CSR register.
        */
        if ((tcdRegs->CSR != 0) && ((tcdRegs->CSR & DMA_CSR_DONE_MASK) == 0)) {
            return kStatus_EDMA_Busy;
        } else {
            imxrt_edma_settransferconfig(handle->base, handle->channel, config, NULL);
            /* Enable auto disable request feature */
            handle->base->TCD[handle->channel].CSR |= DMA_CSR_DREQ_MASK;
            /* Enable major interrupt */
            handle->base->TCD[handle->channel].CSR |= DMA_CSR_INTMAJOR_MASK;

            return kStatus_Success;
        }
    } else { /* Use the TCD queue. */
        uint32_t primask;
        uint32_t csr;
        int8_t currentTcd;
        int8_t previousTcd;
        int8_t nextTcd;

        /* Check if tcd pool is full. */
        primask = imxrt_disableglobalirq();
        if (handle->tcdUsed >= handle->tcdSize) {
            imxrt_enableglobalirq(primask);

            return kStatus_EDMA_QueueFull;
        }
        currentTcd = handle->tail;
        handle->tcdUsed++;
        /* Calculate index of next TCD */
        nextTcd = currentTcd + 1U;
        if (nextTcd == handle->tcdSize) {
            nextTcd = 0U;
        }
        /* Advance queue tail index */
        handle->tail = nextTcd;
        imxrt_enableglobalirq(primask);
        /* Calculate index of previous TCD */
        previousTcd = currentTcd ? currentTcd - 1U : handle->tcdSize - 1U;
        /* Configure current TCD block. */
        imxrt_edma_tcdreset(&handle->tcdPool[currentTcd]);
        imxrt_edma_tcdsettransferconfig(&handle->tcdPool[currentTcd], config, NULL);
        /* Enable major interrupt */
        handle->tcdPool[currentTcd].CSR |= DMA_CSR_INTMAJOR_MASK;
        /* Link current TCD with next TCD for identification of current TCD */
        handle->tcdPool[currentTcd].DLAST_SGA = (uint32_t)&handle->tcdPool[nextTcd];
        /* Chain from previous descriptor unless tcd pool size is 1(this descriptor is its own predecessor). */
        if (currentTcd != previousTcd) {
            /* Enable scatter/gather feature in the previous TCD block. */
            csr = (handle->tcdPool[previousTcd].CSR | DMA_CSR_ESG_MASK) & ~DMA_CSR_DREQ_MASK;
            handle->tcdPool[previousTcd].CSR = csr;
            /*
                Check if the TCD block in the registers is the previous one (points to current TCD block). It
                is used to check if the previous TCD linked has been loaded in TCD register. If so, it need to
                link the TCD register in case link the current TCD with the dead chain when TCD loading occurs
                before link the previous TCD block.
            */
            if (tcdRegs->DLAST_SGA == (uint32_t)&handle->tcdPool[currentTcd]) {
                /* Clear the DREQ bits for the dynamic scatter gather */
                tcdRegs->CSR |= DMA_CSR_DREQ_MASK;
                /* Enable scatter/gather also in the TCD registers. */
                csr = tcdRegs->CSR | DMA_CSR_ESG_MASK;
                /* Must write the CSR register one-time, because the transfer maybe finished anytime. */
                tcdRegs->CSR = csr;
                /*
                    It is very important to check the ESG bit!
                    Because this hardware design: if DONE bit is set, the ESG bit can not be set. So it can
                    be used to check if the dynamic TCD link operation is successful. If ESG bit is not set
                    and the DLAST_SGA is not the next TCD address(it means the dynamic TCD link succeed and
                    the current TCD block has been loaded into TCD registers), it means transfer finished
                    and TCD link operation fail, so must install TCD content into TCD registers and enable
                    transfer again. And if ESG is set, it means transfer has not finished, so TCD dynamic
                    link succeed.
                */
                if (tcdRegs->CSR & DMA_CSR_ESG_MASK) {
                    tcdRegs->CSR &= ~DMA_CSR_DREQ_MASK;
                    return kStatus_Success;
                }
                /*
                    Check whether the current TCD block is already loaded in the TCD registers. It is another
                    condition when ESG bit is not set: it means the dynamic TCD link succeed and the current
                    TCD block has been loaded into TCD registers.
                */
                if (tcdRegs->DLAST_SGA == (uint32_t)&handle->tcdPool[nextTcd]) {
                    return kStatus_Success;
                }
                /*
                    If go to this, means the previous transfer finished, and the DONE bit is set.
                    So shall configure TCD registers.
                */
            } else if (tcdRegs->DLAST_SGA != 0) {
                /* The current TCD block has been linked successfully. */
                return kStatus_Success;
            } else {
                /*
                    DLAST_SGA is 0 and it means the first submit transfer, so shall configure
                    TCD registers.
                */
            }
        }
        /* There is no live chain, TCD block need to be installed in TCD registers. */
        imxrt_edma_installtcd(handle->base, handle->channel, &handle->tcdPool[currentTcd]);
        /* Enable channel request again. */
        if (handle->flags & EDMA_TRANSFER_ENABLED_MASK) {
            handle->base->SERQ = DMA_SERQ_SERQ(handle->channel);
        }

        return kStatus_Success;
    }
}

/****************************************************************************
 * Function: imxrt_edma_starttransfer
 *
 * Description:
 *   brief eDMA starts transfer.
 *   
 *   This function enables the channel request. Users can call this function after submitting the transfer request
 *   or before submitting the transfer request.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_starttransfer(edma_handle_t *handle)
{
    DEBUGASSERT(handle != NULL);

    if (handle->tcdPool == NULL) {
        handle->base->SERQ = DMA_SERQ_SERQ(handle->channel);
    } else { /* Use the TCD queue. */
        uint32_t primask;
        edma_tcd_t *tcdRegs = (edma_tcd_t *)&handle->base->TCD[handle->channel];

        handle->flags |= EDMA_TRANSFER_ENABLED_MASK;

        /* Check if there was at least one descriptor submitted since reset (TCD in registers is valid) */
        if (tcdRegs->DLAST_SGA != 0U) {
            primask = imxrt_disableglobalirq();
            /* Check if channel request is actually disable. */
            if ((handle->base->ERQ & (1U << handle->channel)) == 0U) {
                /* Check if transfer is paused. */
                if ((!(tcdRegs->CSR & DMA_CSR_DONE_MASK)) || (tcdRegs->CSR & DMA_CSR_ESG_MASK)) {
                    /*
                        Re-enable channel request must be as soon as possible, so must put it into
                        critical section to avoid task switching or interrupt service routine.
                    */
                    handle->base->SERQ = DMA_SERQ_SERQ(handle->channel);
                }
            }
            imxrt_enableglobalirq(primask);
        }
    }
}

/****************************************************************************
 * Function: imxrt_edma_stoptransfer
 *
 * Description:
 *   brief eDMA stops transfer.
 * 
 *   This function disables the channel request to pause the transfer. Users can call EDMA_StartTransfer()
 *   again to resume the transfer.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_stoptransfer(edma_handle_t *handle)
{
    DEBUGASSERT(handle != NULL);

    handle->flags &= (~EDMA_TRANSFER_ENABLED_MASK);
    handle->base->CERQ = DMA_CERQ_CERQ(handle->channel);
}

/****************************************************************************
 * Function: imxrt_edma_aborttransfer
 *
 * Description:
 *   brief eDMA aborts transfer.
 *
 *   This function disables the channel request and clear transfer status bits.
 *   Users can submit another transfer after calling this API.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_aborttransfer(edma_handle_t *handle)
{
    handle->base->CERQ = DMA_CERQ_CERQ(handle->channel);
    /*
        Clear CSR to release channel. Because if the given channel started transfer,
        CSR will be not zero. Because if it is the last transfer, DREQ will be set.
        If not, ESG will be set.
    */
    handle->base->TCD[handle->channel].CSR = 0;
    /* Cancel all next TCD transfer. */
    handle->base->TCD[handle->channel].DLAST_SGA = 0;

    /* Handle the tcd */
    if (handle->tcdPool != NULL) {
        handle->header = 0;
        handle->tail = 0;
        handle->tcdUsed = 0;
    }
}

/****************************************************************************
 * Function: imxrt_edma_starttransfer
 *
 * Description:
 *   brief eDMA IRQ handler for the current major loop transfer completion.
 *
 *   This function clears the channel major interrupt flag and calls
 *   the callback function if it is not NULL.
 *
 *   Note:
 *   For the case using TCD queue, when the major iteration count is exhausted, additional operations are performed.
 *   These include the final address adjustments and reloading of the BITER field into the CITER.
 *   Assertion of an optional interrupt request also occurs at this time, as does a possible fetch of a new TCD from
 *   memory using the scatter/gather address pointer included in the descriptor (if scatter/gather is enabled).
 *
 *   For instance, when the time interrupt of TCD[0] happens, the TCD[1] has already been loaded into the eDMA engine.
 *   As sga and sga_index are calculated based on the DLAST_SGA bitfield lies in the TCD_CSR register, the sga_index
 *   in this case should be 2 (DLAST_SGA of TCD[1] stores the address of TCD[2]). Thus, the "tcdUsed" updated should be
 *   (tcdUsed - 2U) which indicates the number of TCDs can be loaded in the memory pool (because TCD[0] and TCD[1] have
 *   been loaded into the eDMA engine at this point already.).
 *
 *   For the last two continuous ISRs in a scatter/gather process, they  both load the last TCD (The last ISR does not
 *   load a new TCD) from the memory pool to the eDMA engine when major loop completes.
 *   Therefore, ensure that the header and tcdUsed updated are identical for them.
 *   tcdUsed are both 0 in this case as no TCD to be loaded.
 *
 *   See the "eDMA basic data flow" in the eDMA Functional description section of the Reference Manual for
 *   further details.
 * 
 * Input Parameters:
 *   handle eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_edma_handleirq(edma_handle_t *handle)
{
    DEBUGASSERT(handle != NULL);

    /* Clear EDMA interrupt flag */
    handle->base->CINT = handle->channel;
    if ((handle->tcdPool == NULL) && (handle->callback != NULL)) {
        (handle->callback)(handle, handle->userData, true, 0);
    } else { /* Use the TCD queue. Please refer to the API descriptions in the eDMA header file for detailed information. */
        uint32_t sga = handle->base->TCD[handle->channel].DLAST_SGA;
        uint32_t sga_index;
        int32_t tcds_done;
        uint8_t new_header;
        bool transfer_done;

        /* Check if transfer is already finished. */
        transfer_done = ((handle->base->TCD[handle->channel].CSR & DMA_CSR_DONE_MASK) != 0);
        /* Get the offset of the next transfer TCD blocks to be loaded into the eDMA engine. */
        sga -= (uint32_t)handle->tcdPool;
        /* Get the index of the next transfer TCD blocks to be loaded into the eDMA engine. */
        sga_index = sga / sizeof(edma_tcd_t);
        /* Adjust header positions. */
        if (transfer_done) {
            /* New header shall point to the next TCD to be loaded (current one is already finished) */
            new_header = sga_index;
        } else {
            /* New header shall point to this descriptor currently loaded (not finished yet) */
            new_header = sga_index ? sga_index - 1U : handle->tcdSize - 1U;
        }
        /* Calculate the number of finished TCDs */
        if (new_header == handle->header) {
            if (handle->tcdUsed == handle->tcdSize) {
                tcds_done = handle->tcdUsed;
            } else {
                /* No TCD in the memory are going to be loaded or internal error occurs. */
                tcds_done = 0;
            }
        } else {
            tcds_done = new_header - handle->header;
            if (tcds_done < 0) {
                tcds_done += handle->tcdSize;
            }
        }
        /* Advance header which points to the TCD to be loaded into the eDMA engine from memory. */
        handle->header = new_header;
        /* Release TCD blocks. tcdUsed is the TCD number which can be used/loaded in the memory pool. */
        handle->tcdUsed -= tcds_done;
        /* Invoke callback function. */
        if (handle->callback) {
            (handle->callback)(handle, handle->userData, transfer_done, tcds_done);
        }

        /* clear the DONE bit here is meaningful for below cases:
         *1.A new TCD has been loaded to EDMA already:
         * need to clear the DONE bit in the IRQ handler to avoid TCD in EDMA been overwritten
         * if peripheral request isn't coming before next transfer request.
         *2.A new TCD has not been loaded to EDMA:
         * for the case that transfer request occur in the privious edma callback, this is a case that doesn't
         * need scatter gather, so keep DONE bit during the next transfer request will re-install the TCD.
         */
        if (transfer_done) {
            handle->base->CDNE = handle->channel;
        }
    }
}

/* 8 channels (Shared): kl28 */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL == 8U

#if defined(DMA0)
void DMA0_04_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[0]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[4]);
    }
}

void DMA0_15_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[1]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[5]);
    }
}

void DMA0_26_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[2]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[6]);
    }
}

void DMA0_37_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[3]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[7]);
    }
}
#endif

#if defined(DMA1)

#if defined(DMA0)
void DMA1_04_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[8]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[12]);
    }
}

void DMA1_15_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[9]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[13]);
    }
}

void DMA1_26_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[10]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[14]);
    }
}

void DMA1_37_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[11]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[15]);
    }
}

#else
void DMA1_04_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[0]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[4]);
    }
}

void DMA1_15_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[1]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[5]);
    }
}

void DMA1_26_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[2]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[6]);
    }
}

void DMA1_37_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[3]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[7]);
    }
}
#endif
#endif
#endif /* 8 channels (Shared) */

/* 16 channels (Shared): K32H844P */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL == 16U

void DMA0_08_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[0]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 8U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[8]);
    }
}

void DMA0_19_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[1]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 9U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[9]);
    }
}

void DMA0_210_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[2]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 10U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[10]);
    }
}

void DMA0_311_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[3]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 11U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[11]);
    }
}

void DMA0_412_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[4]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 12U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[12]);
    }
}

void DMA0_513_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[5]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 13U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[13]);
    }
}

void DMA0_614_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[6]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 14U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[14]);
    }
}

void DMA0_715_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[7]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 15U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[15]);
    }
}

#if defined(DMA1)
void DMA1_08_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[16]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 8U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[24]);
    }
}

void DMA1_19_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[17]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 9U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[25]);
    }
}

void DMA1_210_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[18]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 10U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[26]);
    }
}

void DMA1_311_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[19]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 11U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[27]);
    }
}

void DMA1_412_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[20]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 12U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[28]);
    }
}

void DMA1_513_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[21]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 13U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[29]);
    }
}

void DMA1_614_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[22]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 14U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[30]);
    }
}

void DMA1_715_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA1, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[23]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA1, 15U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[31]);
    }
}
#endif
#endif /* 16 channels (Shared) */

/* 32 channels (Shared): k80 */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL == 32U

void DMA0_DMA16_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[0]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 16U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[16]);
    }
}

void DMA1_DMA17_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[1]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 17U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[17]);
    }
}

void DMA2_DMA18_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[2]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 18U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[18]);
    }
}

void DMA3_DMA19_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[3]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 19U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[19]);
    }
}

void DMA4_DMA20_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[4]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 20U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[20]);
    }
}

void DMA5_DMA21_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[5]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 21U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[21]);
    }
}

void DMA6_DMA22_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[6]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 22U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[22]);
    }
}

void DMA7_DMA23_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[7]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 23U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[23]);
    }
}

void DMA8_DMA24_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 8U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[8]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 24U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[24]);
    }
}

void DMA9_DMA25_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 9U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[9]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 25U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[25]);
    }
}

void DMA10_DMA26_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 10U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[10]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 26U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[26]);
    }
}

void DMA11_DMA27_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 11U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[11]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 27U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[27]);
    }
}

void DMA12_DMA28_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 12U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[12]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 28U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[28]);
    }
}

void DMA13_DMA29_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 13U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[13]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 29U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[29]);
    }
}

void DMA14_DMA30_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 14U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[14]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 30U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[30]);
    }
}

void DMA15_DMA31_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 15U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[15]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 31U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[31]);
    }
}
#endif /* 32 channels (Shared) */

/* 32 channels (Shared): MCIMX7U5_M4 */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL == 32U

void DMA0_0_4_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 0U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[0]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 4U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[4]);
    }
}

void DMA0_1_5_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 1U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[1]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 5U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[5]);
    }
}

void DMA0_2_6_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 2U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[2]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 6U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[6]);
    }
}

void DMA0_3_7_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 3U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[3]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 7U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[7]);
    }
}

void DMA0_8_12_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 8U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[8]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 12U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[12]);
    }
}

void DMA0_9_13_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 9U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[9]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 13U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[13]);
    }
}

void DMA0_10_14_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 10U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[10]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 14U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[14]);
    }
}

void DMA0_11_15_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 11U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[11]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 15U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[15]);
    }
}

void DMA0_16_20_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 16U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[16]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 20U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[20]);
    }
}

void DMA0_17_21_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 17U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[17]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 21U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[21]);
    }
}

void DMA0_18_22_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 18U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[18]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 22U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[22]);
    }
}

void DMA0_19_23_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 19U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[19]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 23U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[23]);
    }
}

void DMA0_24_28_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 24U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[24]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 28U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[28]);
    }
}

void DMA0_25_29_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 25U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[25]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 29U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[29]);
    }
}

void DMA0_26_30_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 26U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[26]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 30U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[30]);
    }
}

void DMA0_27_31_DriverIRQHandler(void)
{
    if ((imxrt_edma_getchannelstatusflags(DMA0, 27U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[27]);
    }
    if ((imxrt_edma_getchannelstatusflags(DMA0, 31U) & kEDMA_InterruptFlag) != 0U) {
        imxrt_edma_handleirq(s_EDMAHandle[31]);
    }
}
#endif /* 32 channels (Shared): MCIMX7U5 */

/* 4 channels (No Shared): kv10  */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL > 0

void DMA0_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[0]);
}

void DMA1_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[1]);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
}

void DMA2_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[2]);
}

void DMA3_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[3]);
}

/* 8 channels (No Shared) */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL > 4U

void DMA4_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[4]);
}

void DMA5_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[5]);
}

void DMA6_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[6]);
}

void DMA7_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[7]);
}
#endif /* FSL_FEATURE_EDMA_MODULE_CHANNEL == 8 */

/* 16 channels (No Shared) */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL > 8U

void DMA8_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[8]);
}

void DMA9_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[9]);
}

void DMA10_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[10]);
}

void DMA11_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[11]);
}

void DMA12_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[12]);
}

void DMA13_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[13]);
}

void DMA14_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[14]);
}

void DMA15_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[15]);
}
#endif /* FSL_FEATURE_EDMA_MODULE_CHANNEL == 16 */

/* 32 channels (No Shared) */
#if defined(FSL_FEATURE_EDMA_MODULE_CHANNEL) && FSL_FEATURE_EDMA_MODULE_CHANNEL > 16U

void DMA16_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[16]);
}

void DMA17_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[17]);
}

void DMA18_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[18]);
}

void DMA19_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[19]);
}

void DMA20_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[20]);
}

void DMA21_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[21]);
}

void DMA22_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[22]);
}

void DMA23_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[23]);
}

void DMA24_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[24]);
}

void DMA25_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[25]);
}

void DMA26_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[26]);
}

void DMA27_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[27]);
}

void DMA28_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[28]);
}

void DMA29_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[29]);
}

void DMA30_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[30]);
}

void DMA31_DriverIRQHandler(void)
{
    imxrt_edma_handleirq(s_EDMAHandle[31]);
}
#endif /* FSL_FEATURE_EDMA_MODULE_CHANNEL == 32 */

#endif /* 4/8/16/32 channels (No Shared)  */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_dmainitialize
 *
 * Description:
 *   Initialize the DMA subsystem
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void weak_function up_dmainitialize(void)
{
	edma_config_t edmaConfig = {0};

	dmainfo("Initialize eDMA\n");

	/* Enable peripheral clocking to eDMA and DMAMUX modules */

    imxrt_edma_getdefaultconfig(&edmaConfig);
	imxrt_edma_init(s_edmaBases[0], &edmaConfig);

	/* Attach DMA interrupt vectors.
	 *
	 * NOTE that there are only 16 vectors for 32 DMA channels.
	 */

	DMA0_DMA16_DriverIRQHandler();
	DMA1_DMA17_DriverIRQHandler();
	DMA2_DMA18_DriverIRQHandler();
	DMA3_DMA19_DriverIRQHandler();
	DMA4_DMA20_DriverIRQHandler();
	DMA5_DMA21_DriverIRQHandler();
	DMA6_DMA22_DriverIRQHandler();
	DMA7_DMA23_DriverIRQHandler();
	DMA8_DMA24_DriverIRQHandler();
	DMA9_DMA25_DriverIRQHandler();
	DMA10_DMA26_DriverIRQHandler();
	DMA11_DMA27_DriverIRQHandler();
	DMA12_DMA28_DriverIRQHandler();
	DMA13_DMA29_DriverIRQHandler();
	DMA14_DMA30_DriverIRQHandler();
	DMA15_DMA31_DriverIRQHandler();

	/* Disable and clear all error interrupts */

	putreg32(0, IMXRT_EDMA_EEI);
	putreg32(0xffffffff, IMXRT_EDMA_ERR);

	/* Clear all pending DMA channel interrupts */

	putreg32(0xffffffff, IMXRT_EDMA_INT);

	/* Enable the channel interrupts at the NVIC (still disabled at the eDMA
	 * controller).
	 */

	up_enable_irq(IMXRT_IRQ_EDMA0_16);
	up_enable_irq(IMXRT_IRQ_EDMA1_17);
	up_enable_irq(IMXRT_IRQ_EDMA2_18);
	up_enable_irq(IMXRT_IRQ_EDMA3_19);
	up_enable_irq(IMXRT_IRQ_EDMA4_20);
	up_enable_irq(IMXRT_IRQ_EDMA5_21);
	up_enable_irq(IMXRT_IRQ_EDMA6_22);
	up_enable_irq(IMXRT_IRQ_EDMA7_23);
	up_enable_irq(IMXRT_IRQ_EDMA8_24);
	up_enable_irq(IMXRT_IRQ_EDMA9_25);
	up_enable_irq(IMXRT_IRQ_EDMA10_26);
	up_enable_irq(IMXRT_IRQ_EDMA11_27);
	up_enable_irq(IMXRT_IRQ_EDMA12_28);
	up_enable_irq(IMXRT_IRQ_EDMA13_29);
	up_enable_irq(IMXRT_IRQ_EDMA14_30);
	up_enable_irq(IMXRT_IRQ_EDMA15_31);
}

/****************************************************************************
 * Name: imxrt_dmasample
 *
 * Description:
 *   Sample DMA register contents
 *
 * Assumptions:
 *   - DMA handle allocated by imxrt_dmach_alloc()
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_DMA
void imxrt_dmasample(DMACH_HANDLE handle, struct imxrt_dmaregs_s *regs)
{
	struct imxrt_dmach_s *dmach = (struct imxrt_dmach_s *)handle;
	uintptr_t regaddr;
	unsigned int chan;
	irqstate_t flags;

	DEBUGASSERT(dmach != NULL && regs != NULL);
	chan = dmach->chan;
	regs->chan = chan;

	/* eDMA Global Registers */

	flags = irqsave();

	regs->cr = getreg32(IMXRT_EDMA_CR);	/* Control */
	regs->es = getreg32(IMXRT_EDMA_ES);	/* Error Status */
	regs->erq = getreg32(IMXRT_EDMA_ERQ);	/* Enable Request */
	regs->req = getreg32(IMXRT_EDMA_INT);	/* Interrupt Request */
	regs->err = getreg32(IMXRT_EDMA_ERR);	/* Error */
	regs->hrs = getreg32(IMXRT_EDMA_HRS);	/* Hardware Request Status */
	regs->ears = getreg32(IMXRT_EDMA_EARS);	/* Enable Asynchronous Request in Stop */

	/* eDMA Channel registers */

	regaddr = IMXRT_EDMA_DCHPRI(chan);
	regs->dchpri = getreg8(regaddr);	/* Channel priority */

	/* eDMA TCD */

	base = IMXRT_EDMA_TCD_BASE(chan);
	regs->saddr = getreg32(base + IMXRT_EDMA_TCD_SADDR_OFFSET);
	regs->soff = getreg16(base + IMXRT_EDMA_TCD_SOFF_OFFSET);
	regs->attr = getreg16(base + IMXRT_EDMA_TCD_ATTR_OFFSET);
	regs->nbml = getreg32(base + IMXRT_EDMA_TCD_NBYTES_ML_OFFSET);
	regs->slast = getreg32(base + IMXRT_EDMA_TCD_SLAST_OFFSET);
	regs->daddr = getreg32(base + IMXRT_EDMA_TCD_DADDR_OFFSET);
	regs->doff = getreg16(base + IMXRT_EDMA_TCD_DOFF_OFFSET);
	regs->citer = getreg16(base + IMXRT_EDMA_TCD_CITER_ELINK_OFFSET);
	regs->dlastsga = getreg32(base + IMXRT_EDMA_TCD_DLASTSGA_OFFSET);
	regs->csr = getreg16(base + IMXRT_EDMA_TCD_CSR_OFFSET);
	regs->biter = getreg16(base + IMXRT_EDMA_TCD_BITER_ELINK_OFFSET);

	/* DMAMUX registers */

	regaddr = IMXRT_DMAMUX_CHCFG(chan);
	regs->dmamux = getreg32(regaddr);	/* Channel configuration */

	irqrestore(flags);
}
#endif							/* CONFIG_DEBUG_DMA */

/****************************************************************************
 * Name: imxrt_dmadump
 *
 * Description:
 *   Dump previously sampled DMA register contents
 *
 * Assumptions:
 *   - DMA handle allocated by imxrt_dmach_alloc()
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_DMA
void imxrt_dmadump(const struct imxrt_dmaregs_s *regs, const char *msg)
{
	unsigned int chan;

	DEBUGASSERT(regs != NULL && msg != NULL);

	chan = regs->chan;
	DEBUGASSERT(chan < IMXRT_EDMA_NCHANNELS);

	dmainfo("%s\n", msg);
	dmainfo("  eDMA Global Registers:\n");
	dmainfo("          CR: %08x\n", regs->cr);
	dmainfo("          ES: %08x\n", regs->es);
	dmainfo("         ERQ: %08x\n", regs->erq);
	dmainfo("         INT: %08x\n", regs->req);
	dmainfo("         ERR: %08x\n", regs->err);
	dmainfo("        EARS: %08x\n", regs->hrs);

	/* eDMA Channel registers */

	dmainfo("  eDMA Channel %u Registers:\n", chan);
	dmainfo("    DCHPRI: %02x\n", regs->dchpri);

	/* eDMA TCD */

	dmainfo("  eDMA Channel %u TCD Registers:\n", chan);
	dmainfo("       SADDR: %08x\n", regs->saddr);
	dmainfo("        SOFF: %04x\n", regs->soff);
	dmainfo("        ATTR: %04x\n", regs->attr);
	dmainfo("        NBML: %05x\n", regs->nbml);
	dmainfo("       SLAST: %05x\n", regs->slast);
	dmainfo("       DADDR: %05x\n", regs->daddr);
	dmainfo("        DOFF: %04x\n", regs->doff);
	dmainfo("       CITER: %04x\n", regs->citer);
	dmainfo("    DLASTSGA: %08x\n", regs->dlastsga);
	dmainfo("         CSR: %04x\n", regs->csr);
	dmainfo("       BITER: %04x\n", regs->biter);

	/* DMAMUX registers */

	dmainfo("  DMAMUX Channel %u Registers:\n", chan);
	dmainfo("      DMAMUX: %08x\n", regs->dmamux);
}
#endif							/* CONFIG_DEBUG_DMA */
#endif							/* CONFIG_IMXRT_EDMA */
