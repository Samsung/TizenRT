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
 * os/arch/arm/src/imxrt/imxrt_lpi2c.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Ivan Ucherdzhiev <ivanucherdjiev@gmail.com>
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

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>

#include "irq/irq.h"

#include <arch/board/board.h>
#include "imxrt_clock.h"
#include "imxrt_lpi2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Common sets of flags used by the driver. */
enum _lpi2c_flag_constants {
    /*! All flags which are cleared by the driver upon starting a transfer. */
    kMasterClearFlags = kLPI2C_MasterEndOfPacketFlag | kLPI2C_MasterStopDetectFlag | kLPI2C_MasterNackDetectFlag |
                        kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterFifoErrFlag | kLPI2C_MasterPinLowTimeoutFlag |
                        kLPI2C_MasterDataMatchFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kMasterIrqFlags = kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterTxReadyFlag | kLPI2C_MasterRxReadyFlag |
                      kLPI2C_MasterStopDetectFlag | kLPI2C_MasterNackDetectFlag | kLPI2C_MasterPinLowTimeoutFlag |
                      kLPI2C_MasterFifoErrFlag,

    /*! Errors to check for. */
    kMasterErrorFlags = kLPI2C_MasterNackDetectFlag | kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterFifoErrFlag |
                        kLPI2C_MasterPinLowTimeoutFlag,

    /*! All flags which are cleared by the driver upon starting a transfer. */
    kSlaveClearFlags = kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveBitErrFlag |
                       kLPI2C_SlaveFifoErrFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kSlaveIrqFlags = kLPI2C_SlaveTxReadyFlag | kLPI2C_SlaveRxReadyFlag | kLPI2C_SlaveStopDetectFlag |
                     kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveFifoErrFlag | kLPI2C_SlaveBitErrFlag |
                     kLPI2C_SlaveTransmitAckFlag | kLPI2C_SlaveAddressValidFlag,

    /*! Errors to check for. */
    kSlaveErrorFlags = kLPI2C_SlaveFifoErrFlag | kLPI2C_SlaveBitErrFlag,
};

/* ! @brief LPI2C master fifo commands. */
enum _lpi2c_master_fifo_cmd {
    kTxDataCmd = LPI2C_MTDR_CMD(0x0U), /*!< Transmit DATA[7:0] */
    kRxDataCmd = LPI2C_MTDR_CMD(0X1U), /*!< Receive (DATA[7:0] + 1) bytes */
    kStopCmd = LPI2C_MTDR_CMD(0x2U),   /*!< Generate STOP condition */
    kStartCmd = LPI2C_MTDR_CMD(0x4U),  /*!< Generate(repeated) START and transmit address in DATA[[7:0] */
};

/*!
 * @brief Default watermark values.
 *
 * The default watermarks are set to zero.
 */
enum _lpi2c_default_watermarks {
    kDefaultTxWatermark = 0,
    kDefaultRxWatermark = 0,
};

/*! @brief States for the state machine used by transactional APIs. */
enum _lpi2c_transfer_states {
    kIdleState = 0,
    kSendCommandState,
    kIssueReadCommandState,
    kTransferDataState,
    kStopState,
    kWaitForCompletionState,
};

/*! @brief Typedef for master interrupt handler. */
typedef void (*lpi2c_master_isr_t)(LPI2C_Type *base, lpi2c_master_handle_t *handle);

/*! @brief Typedef for slave interrupt handler. */
typedef void (*lpi2c_slave_isr_t)(LPI2C_Type *base, lpi2c_slave_handle_t *handle);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Not static so it can be used from fsl_lpi2c_edma.c. */
uint32_t imxrt_lpi2c_getinstance(LPI2C_Type *base);

static uint32_t imxrt_lpi2c_getcyclesforwidth(uint32_t sourceClock_Hz,
                                        uint32_t width_ns,
                                        uint32_t maxCycles,
                                        uint32_t prescaler);

static status_t imxrt_lpi2c_masterwaitfortxready(LPI2C_Type *base);

static status_t imxrt_lpi2c_runtransferstatemachine(LPI2C_Type *base, lpi2c_master_handle_t *handle, bool *isDone);

static void imxrt_lpi2c_inittransferstatemachine(lpi2c_master_handle_t *handle);

static status_t imxrt_lpi2c_slavecheckandclearerror(LPI2C_Type *base, uint32_t flags);

static void imxrt_lpi2c_commonirqhandler(LPI2C_Type *base, uint32_t instance);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map LPI2C instance number to base pointer. */
static LPI2C_Type *const kLpi2cBases[] = LPI2C_BASE_PTRS;

/*! @brief Array to map LPI2C instance number to IRQ number. */
static IRQn_Type const kLpi2cIrqs[] = LPI2C_IRQS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map LPI2C instance number to clock gate enum. */
static clock_ip_name_t const kLpi2cClocks[] = LPI2C_CLOCKS;

#if defined(LPI2C_PERIPH_CLOCKS)
/*! @brief Array to map LPI2C instance number to pheripheral clock gate enum. */
static const clock_ip_name_t kLpi2cPeriphClocks[] = LPI2C_PERIPH_CLOCKS;
#endif

#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*! @brief Pointer to master IRQ handler for each instance. */
static lpi2c_master_isr_t s_lpi2cMasterIsr;

/*! @brief Pointers to master handles for each instance. */
static lpi2c_master_handle_t *s_lpi2cMasterHandle[ARRAY_SIZE(kLpi2cBases)];

/*! @brief Pointer to slave IRQ handler for each instance. */
static lpi2c_slave_isr_t s_lpi2cSlaveIsr;

/*! @brief Pointers to slave handles for each instance. */
static lpi2c_slave_handle_t *s_lpi2cSlaveHandle[ARRAY_SIZE(kLpi2cBases)];

/*******************************************************************************
 * Code
 ******************************************************************************/

/****************************************************************************
 * Function: imxrt_lpi2c_getinstance
 *
 * Description:
 *   brief Returns an instance number given a base address.
 *
 *   If an invalid base address is passed, debug builds will assert. Release builds will just return
 *   instance number 0.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *
 * Returned Value:
 *   Returns LPI2C instance number starting from 0.
 *
 ****************************************************************************/
uint32_t imxrt_lpi2c_getinstance(LPI2C_Type *base)
{
    uint32_t instance;
    for (instance = 0; instance < ARRAY_SIZE(kLpi2cBases); ++instance) {
        if (kLpi2cBases[instance] == base) {
            return instance;
        }
    }

    DEBUGASSERT(false);
    return 0;
}

/****************************************************************************
 * Function: imxrt_imxrt_lpi2c_getcyclesforwidth
 *
 * Description:
 *   brief Computes a cycle count for a given time in nanoseconds.
 *
 * Input Parameters:
 *   sourceClock_Hz LPI2C functional clock frequency in Hertz.
 *   width_ns Desired with in nanoseconds.
 *   maxCycles Maximum cycle count, determined by the number of bits wide the cycle count field is.
 *   prescaler LPI2C prescaler setting. Pass 1 if the prescaler should not be used, as for slave glitch widths.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static uint32_t imxrt_imxrt_lpi2c_getcyclesforwidth(uint32_t sourceClock_Hz,
                                        uint32_t width_ns,
                                        uint32_t maxCycles,
                                        uint32_t prescaler)
{
    DEBUGASSERT(sourceClock_Hz > 0);
    DEBUGASSERT(prescaler > 0);

    uint32_t busCycle_ns = 1000000 / (sourceClock_Hz / prescaler / 1000);
    uint32_t cycles = 0;

    /* Search for the cycle count just below the desired glitch width. */
    while ((((cycles + 1) * busCycle_ns) < width_ns) && (cycles + 1 < maxCycles)) {
        ++cycles;
    }

    /* If we end up with zero cycles, then set the filter to a single cycle unless the */
    /* bus clock is greater than 10x the desired glitch width. */
    if ((cycles == 0) && (busCycle_ns <= (width_ns * 10))) {
        cycles = 1;
    }

    return cycles;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastercheckandclearerror
 *
 * Description:
 *   brief Convert provided flags to status code, and clear any errors if present.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   status Current status flags value that will be checked.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_LPI2C_PinLowTimeout
 *   Returns #kStatus_LPI2C_ArbitrationLost
 *   Returns #kStatus_LPI2C_Nak
 *   Returns #kStatus_LPI2C_FifoError
 *
 ****************************************************************************/
/* Not static so it can be used from fsl_lpi2c_edma.c. */
status_t imxrt_lpi2c_mastercheckandclearerror(LPI2C_Type *base, uint32_t status)
{
    status_t result = kStatus_Success;

    /* Check for error. These errors cause a stop to automatically be sent. We must */
    /* clear the errors before a new transfer can start. */
    status &= kMasterErrorFlags;
    if (status) {
        /* Select the correct error code. Ordered by severity, with bus issues first. */
        if (status & kLPI2C_MasterPinLowTimeoutFlag) {
            result = kStatus_LPI2C_PinLowTimeout;
        } else if (status & kLPI2C_MasterArbitrationLostFlag) {
            result = kStatus_LPI2C_ArbitrationLost;
        } else if (status & kLPI2C_MasterNackDetectFlag) {
            result = kStatus_LPI2C_Nak;
        } else if (status & kLPI2C_MasterFifoErrFlag) {
            result = kStatus_LPI2C_FifoError;
        } else {
            DEBUGASSERT(false);
        }

        /* Clear the flags. */
        imxrt_lpi2c_masterclearstatusflags(base, status);

        /* Reset fifos. These flags clear automatically. */
        base->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterwaitfortxready
 *
 * Description:
 *   brief Wait until there is room in the tx fifo.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_LPI2C_PinLowTimeout
 *   Returns #kStatus_LPI2C_ArbitrationLost
 *   Returns #kStatus_LPI2C_Nak
 *   Returns #kStatus_LPI2C_FifoError
 *
 ****************************************************************************/
static status_t imxrt_lpi2c_masterwaitfortxready(LPI2C_Type *base)
{
    uint32_t status;
    size_t txCount;
    size_t txFifoSize = FSL_FEATURE_LPI2C_FIFO_SIZEn(base);

#if LPI2C_WAIT_TIMEOUT
    uint32_t waitTimes = LPI2C_WAIT_TIMEOUT;
#endif
    do {
        status_t result;

        /* Get the number of words in the tx fifo and compute empty slots. */
        imxrt_lpi2c_mastergetfifocounts(base, NULL, &txCount);
        txCount = txFifoSize - txCount;

        /* Check for error flags. */
        status = imxrt_lpi2c_mastergetstatusflags(base);
        result = imxrt_lpi2c_mastercheckandclearerror(base, status);
        if (result) {
            return result;
        }
#if LPI2C_WAIT_TIMEOUT
    } while ((!txCount) && (--waitTimes));

    if (waitTimes == 0) {
        return kStatus_LPI2C_Timeout;
    }
#else
    } while (!txCount);
#endif

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterwaitfortxready
 *
 * Description:
 *   brief Make sure the bus isn't already busy.
 *
 *   A busy bus is allowed if we are the one driving it.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_LPI2C_Busy
 *
 ****************************************************************************/
/* Not static so it can be used from fsl_lpi2c_edma.c. */
status_t imxrt_lpi2c_checkforbusybus(LPI2C_Type *base)
{
    uint32_t status = imxrt_lpi2c_mastergetstatusflags(base);
    if ((status & kLPI2C_MasterBusBusyFlag) && (!(status & kLPI2C_MasterBusyFlag))) {
        return kStatus_LPI2C_Busy;
    }

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastergetdefaultconfig
 *
 * Description:
 *   brief Provides a default configuration for the LPI2C master peripheral.
 *
 *   This function provides the following default configuration for the LPI2C master peripheral:
 *   code
 *    masterConfig->enableMaster            = true;
 *    masterConfig->debugEnable             = false;
 *    masterConfig->ignoreAck               = false;
 *    masterConfig->pinConfig               = kLPI2C_2PinOpenDrain;
 *    masterConfig->baudRate_Hz             = 100000U;
 *    masterConfig->busIdleTimeout_ns       = 0;
 *    masterConfig->pinLowTimeout_ns        = 0;
 *    masterConfig->sdaGlitchFilterWidth_ns = 0;
 *    masterConfig->sclGlitchFilterWidth_ns = 0;
 *    masterConfig->hostRequest.enable      = false;
 *    masterConfig->hostRequest.source      = kLPI2C_HostRequestExternalPin;
 *    masterConfig->hostRequest.polarity    = kLPI2C_HostRequestPinActiveHigh;
 *   endcode
 *
 *   After calling this function, you can override any settings in order to customize the configuration,
 *   prior to initializing the master driver with imxrt_lpi2c_masterinit().
 *
 * Input Parameters:
 *   masterConfig[out] User provided configuration structure for default values. Refer to #lpi2c_master_config_t.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_mastergetdefaultconfig(lpi2c_master_config_t *masterConfig)
{
    /* Initializes the configure structure to zero. */
    memset(masterConfig, 0, sizeof(*masterConfig));

    masterConfig->enableMaster = true;
    masterConfig->debugEnable = false;
    masterConfig->enableDoze = true;
    masterConfig->ignoreAck = false;
    masterConfig->pinConfig = kLPI2C_2PinOpenDrain;
    masterConfig->baudRate_Hz = 100000U;
    masterConfig->busIdleTimeout_ns = 0;
    masterConfig->pinLowTimeout_ns = 0;
    masterConfig->sdaGlitchFilterWidth_ns = 0;
    masterConfig->sclGlitchFilterWidth_ns = 0;
    masterConfig->hostRequest.enable = false;
    masterConfig->hostRequest.source = kLPI2C_HostRequestExternalPin;
    masterConfig->hostRequest.polarity = kLPI2C_HostRequestPinActiveHigh;
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterinit
 *
 * Description:
 *   brief Initializes the LPI2C master peripheral.
 *
 *   This function enables the peripheral clock and initializes the LPI2C master peripheral as described by the user
 *   provided configuration. A software reset is performed prior to configuration.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   masterConfig User provided peripheral configuration. Use imxrt_lpi2c_mastergetdefaultconfig() to get a set of
 *                defaults
 *                      that you can override.
 *   sourceClock_Hz Frequency in Hertz of the LPI2C functional clock. Used to calculate the baud rate divisors,
 *      filter widths, and timeout periods.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_masterinit(LPI2C_Type *base, const lpi2c_master_config_t *masterConfig, uint32_t sourceClock_Hz)
{
    uint32_t prescaler;
    uint32_t cycles;
    uint32_t cfgr2;
    uint32_t value;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)

    uint32_t instance = imxrt_lpi2c_getinstance(base);

    /* Ungate the clock. */
    imxrt_clock_enableclock(kLpi2cClocks[instance]);
#if defined(LPI2C_PERIPH_CLOCKS)
    /* Ungate the functional clock in initialize function. */
    imxrt_clock_enableclock(kLpi2cPeriphClocks[instance]);
#endif

#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Reset peripheral before configuring it. */
    imxrt_lpi2c_masterreset(base);

    /* Doze bit: 0 is enable, 1 is disable */
    base->MCR = LPI2C_MCR_DBGEN(masterConfig->debugEnable) | LPI2C_MCR_DOZEN(!(masterConfig->enableDoze));

    /* host request */
    value = base->MCFGR0;
    value &= (~(LPI2C_MCFGR0_HREN_MASK | LPI2C_MCFGR0_HRPOL_MASK | LPI2C_MCFGR0_HRSEL_MASK));
    value |= LPI2C_MCFGR0_HREN(masterConfig->hostRequest.enable) |
             LPI2C_MCFGR0_HRPOL(masterConfig->hostRequest.polarity) |
             LPI2C_MCFGR0_HRSEL(masterConfig->hostRequest.source);
    base->MCFGR0 = value;

    /* pin config and ignore ack */
    value = base->MCFGR1;
    value &= ~(LPI2C_MCFGR1_PINCFG_MASK | LPI2C_MCFGR1_IGNACK_MASK);
    value |= LPI2C_MCFGR1_PINCFG(masterConfig->pinConfig);
    value |= LPI2C_MCFGR1_IGNACK(masterConfig->ignoreAck);
    base->MCFGR1 = value;

    imxrt_lpi2c_mastersetwatermarks(base, kDefaultTxWatermark, kDefaultRxWatermark);

    imxrt_lpi2c_mastersetbaudrate(base, sourceClock_Hz, masterConfig->baudRate_Hz);

    /* Configure glitch filters and bus idle and pin low timeouts. */
    prescaler = (base->MCFGR1 & LPI2C_MCFGR1_PRESCALE_MASK) >> LPI2C_MCFGR1_PRESCALE_SHIFT;
    cfgr2 = base->MCFGR2;
    if (masterConfig->busIdleTimeout_ns) {
        cycles = imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, masterConfig->busIdleTimeout_ns,
                                         (LPI2C_MCFGR2_BUSIDLE_MASK >> LPI2C_MCFGR2_BUSIDLE_SHIFT), prescaler);
        cfgr2 &= ~LPI2C_MCFGR2_BUSIDLE_MASK;
        cfgr2 |= LPI2C_MCFGR2_BUSIDLE(cycles);
    }
    if (masterConfig->sdaGlitchFilterWidth_ns) {
        cycles = imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, masterConfig->sdaGlitchFilterWidth_ns,
                                         (LPI2C_MCFGR2_FILTSDA_MASK >> LPI2C_MCFGR2_FILTSDA_SHIFT), 1);
        cfgr2 &= ~LPI2C_MCFGR2_FILTSDA_MASK;
        cfgr2 |= LPI2C_MCFGR2_FILTSDA(cycles);
    }
    if (masterConfig->sclGlitchFilterWidth_ns) {
        cycles = imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, masterConfig->sclGlitchFilterWidth_ns,
                                         (LPI2C_MCFGR2_FILTSCL_MASK >> LPI2C_MCFGR2_FILTSCL_SHIFT), 1);
        cfgr2 &= ~LPI2C_MCFGR2_FILTSCL_MASK;
        cfgr2 |= LPI2C_MCFGR2_FILTSCL(cycles);
    }
    base->MCFGR2 = cfgr2;
    if (masterConfig->pinLowTimeout_ns) {
        cycles = imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, masterConfig->pinLowTimeout_ns / 256,
                                         (LPI2C_MCFGR2_BUSIDLE_MASK >> LPI2C_MCFGR2_BUSIDLE_SHIFT), prescaler);
        base->MCFGR3 = (base->MCFGR3 & ~LPI2C_MCFGR3_PINLOW_MASK) | LPI2C_MCFGR3_PINLOW(cycles);
    }

    imxrt_lpi2c_masterenable(base, masterConfig->enableMaster);
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterdeinit
 *
 * Description:
 *   brief Deinitializes the LPI2C master peripheral.
 *
 *   This function disables the LPI2C master peripheral and gates the clock. It also performs a software
 *   reset to restore the peripheral to reset conditions.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_masterdeinit(LPI2C_Type *base)
{
    /* Restore to reset state. */
    imxrt_lpi2c_masterreset(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)

    uint32_t instance = imxrt_lpi2c_getinstance(base);

    /* Gate clock. */
    imxrt_clock_disableclock(kLpi2cClocks[instance]);
#if defined(LPI2C_PERIPH_CLOCKS)
    /* Gate the functional clock. */
    imxrt_clock_disableclock(kLpi2cPeriphClocks[instance]);
#endif

#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterconfiguredatamatch
 *
 * Description:
 *   brief Configures LPI2C master data match feature.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   config Settings for the data match feature.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_masterconfiguredatamatch(LPI2C_Type *base, const lpi2c_data_match_config_t *config)
{
    /* Disable master mode. */
    bool wasEnabled = (base->MCR & LPI2C_MCR_MEN_MASK) >> LPI2C_MCR_MEN_SHIFT;
    imxrt_lpi2c_masterenable(base, false);

    base->MCFGR1 = (base->MCFGR1 & ~LPI2C_MCFGR1_MATCFG_MASK) | LPI2C_MCFGR1_MATCFG(config->matchMode);
    base->MCFGR0 = (base->MCFGR0 & ~LPI2C_MCFGR0_RDMO_MASK) | LPI2C_MCFGR0_RDMO(config->rxDataMatchOnly);
    base->MDMR = LPI2C_MDMR_MATCH0(config->match0) | LPI2C_MDMR_MATCH1(config->match1);

    /* Restore master mode. */
    if (wasEnabled) {
        imxrt_lpi2c_masterenable(base, true);
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastersetbaudrate
 *
 * Description:
 *   brief Sets the I2C bus frequency for master transactions.
 *
 *   The LPI2C master is automatically disabled and re-enabled as necessary to configure the baud
 *   rate. Do not call this function during a transfer, or the transfer is aborted.
 *
 *   note Please note that the second parameter is the clock frequency of LPI2C module, the third
 *   parameter means user configured bus baudrate, this implementation is different from other I2C drivers
 *   which use baudrate configuration as second parameter and source clock frequency as third parameter.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   sourceClock_Hz LPI2C functional clock frequency in Hertz.
 *   baudRate_Hz Requested bus frequency in Hertz.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_mastersetbaudrate(LPI2C_Type *base, uint32_t sourceClock_Hz, uint32_t baudRate_Hz)
{
    uint32_t prescale = 0;
    uint32_t bestPre = 0;
    uint32_t bestClkHi = 0;
    uint32_t absError = 0;
    uint32_t bestError = 0xffffffffu;
    uint32_t value;
    uint32_t clkHiCycle;
    uint32_t computedRate;
    int i;
    bool wasEnabled;

    /* Disable master mode. */
    wasEnabled = (base->MCR & LPI2C_MCR_MEN_MASK) >> LPI2C_MCR_MEN_SHIFT;
    imxrt_lpi2c_masterenable(base, false);

    /* Baud rate = (sourceClock_Hz/2^prescale)/(CLKLO+1+CLKHI+1 + ROUNDDOWN((2+FILTSCL)/2^prescale) */
    /* Assume CLKLO = 2*CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2. */
    for (prescale = 1; (prescale <= 128) && (bestError != 0); prescale = 2 * prescale) {
        for (clkHiCycle = 1; clkHiCycle < 32; clkHiCycle++) {
            if (clkHiCycle == 1) {
                computedRate = (sourceClock_Hz / prescale) / (1 + 3 + 2 + 2 / prescale);
            } else {
                computedRate = (sourceClock_Hz / prescale) / (3 * clkHiCycle + 2 + 2 / prescale);
            }

            absError = baudRate_Hz > computedRate ? baudRate_Hz - computedRate : computedRate - baudRate_Hz;

            if (absError < bestError) {
                bestPre = prescale;
                bestClkHi = clkHiCycle;
                bestError = absError;

                /* If the error is 0, then we can stop searching because we won't find a better match. */
                if (absError == 0) {
                    break;
                }
            }
        }
    }

    /* Standard, fast, fast mode plus and ultra-fast transfers. */
    value = LPI2C_MCCR0_CLKHI(bestClkHi);

    if (bestClkHi < 2) {
        value |= LPI2C_MCCR0_CLKLO(3) | LPI2C_MCCR0_SETHOLD(2) | LPI2C_MCCR0_DATAVD(1);
    } else {
        value |= LPI2C_MCCR0_CLKLO(2 * bestClkHi) | LPI2C_MCCR0_SETHOLD(bestClkHi) | LPI2C_MCCR0_DATAVD(bestClkHi / 2);
    }

    base->MCCR0 = value;

    for (i = 0; i < 8; i++) {
        if (bestPre == (1U << i)) {
            bestPre = i;
            break;
        }
    }
    base->MCFGR1 = (base->MCFGR1 & ~LPI2C_MCFGR1_PRESCALE_MASK) | LPI2C_MCFGR1_PRESCALE(bestPre);

    /* Restore master mode. */
    if (wasEnabled) {
        imxrt_lpi2c_masterenable(base, true);
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterstart
 *
 * Description:
 *   brief Sends a START signal and slave address on the I2C bus.
 *
 *   This function is used to initiate a new master mode transfer. First, the bus state is checked to ensure
 *   that another master is not occupying the bus. Then a START signal is transmitted, followed by the
 *   7-bit address specified in the a address parameter. Note that this function does not actually wait
 *   until the START and address are successfully sent on the bus before returning.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   address 7-bit slave device address, in bits [6:0].
 *   dir Master transfer direction, either #kLPI2C_Read or #kLPI2C_Write. This parameter is used to set
 *        the R/w bit (bit 0) in the transmitted slave address.
 *
 * Returned Value:
 *   Returns #kStatus_Success START signal and address were successfully enqueued in the transmit FIFO.
 *   Returns #kStatus_LPI2C_Busy Another master is currently utilizing the bus.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_masterstart(LPI2C_Type *base, uint8_t address, lpi2c_direction_t dir)
{
    /* Return an error if the bus is already in use not by us. */
    status_t result = imxrt_lpi2c_checkforbusybus(base);
    if (result) {
        return result;
    }

    /* Clear all flags. */
    imxrt_lpi2c_masterclearstatusflags(base, kMasterClearFlags);

    /* Turn off auto-stop option. */
    base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;

    /* Wait until there is room in the fifo. */
    result = imxrt_lpi2c_masterwaitfortxready(base);
    if (result) {
        return result;
    }

    /* Issue start command. */
    base->MTDR = kStartCmd | (((uint32_t)address << 1U) | (uint32_t)dir);

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterstop
 *
 * Description:
 *   brief Sends a STOP signal on the I2C bus.
 *
 *   This function does not return until the STOP signal is seen on the bus, or an error occurs.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *
 * Returned Value:
 *   Returns #kStatus_Success The STOP signal was successfully sent on the bus and the transaction terminated.
 *   Returns #kStatus_LPI2C_Busy Another master is currently utilizing the bus.
 *   Returns #kStatus_LPI2C_Nak The slave device sent a NAK in response to a byte.
 *   Returns #kStatus_LPI2C_FifoError FIFO under run or overrun.
 *   Returns #kStatus_LPI2C_ArbitrationLost Arbitration lost error.
 *   Returns #kStatus_LPI2C_PinLowTimeout SCL or SDA were held low longer than the timeout.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_masterstop(LPI2C_Type *base)
{
    /* Wait until there is room in the fifo. */
    status_t result = imxrt_lpi2c_masterwaitfortxready(base);
    if (result) {
        return result;
    }

    /* Send the STOP signal */
    base->MTDR = kStopCmd;

/* Wait for the stop detected flag to set, indicating the transfer has completed on the bus. */
/* Also check for errors while waiting. */
#if LPI2C_WAIT_TIMEOUT
    uint32_t waitTimes = LPI2C_WAIT_TIMEOUT;
#endif

#if LPI2C_WAIT_TIMEOUT
    while ((result == kStatus_Success) && (--waitTimes)) {
#else
    while (result == kStatus_Success) {
#endif
        uint32_t status = imxrt_lpi2c_mastergetstatusflags(base);

        /* Check for error flags. */
        result = imxrt_lpi2c_mastercheckandclearerror(base, status);

        /* Check if the stop was sent successfully. */
        if (status & kLPI2C_MasterStopDetectFlag) {
            imxrt_lpi2c_masterclearstatusflags(base, kLPI2C_MasterStopDetectFlag);
            break;
        }
    }

#if LPI2C_WAIT_TIMEOUT
    if (waitTimes == 0) {
        return kStatus_LPI2C_Timeout;
    }
#endif

    return result;
}

/****************************************************************************
 * Function: imxrt_lpi2c_masterreceive
 *
 * Description:
 *   brief Performs a polling receive transfer on the I2C bus.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   rxBuff The pointer to the data to be transferred.
 *   rxSize The length in bytes of the data to be transferred.
 *
 * Returned Value:
 *   Returns #kStatus_Success Data was received successfully.
 *   Returns #kStatus_LPI2C_Busy Another master is currently utilizing the bus.
 *   Returns #kStatus_LPI2C_Nak The slave device sent a NAK in response to a byte.
 *   Returns #kStatus_LPI2C_FifoError FIFO under run or overrun.
 *   Returns #kStatus_LPI2C_ArbitrationLost Arbitration lost error.
 *   Returns #kStatus_LPI2C_PinLowTimeout SCL or SDA were held low longer than the timeout.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_masterreceive(LPI2C_Type *base, void *rxBuff, size_t rxSize)
{
    status_t result;
    uint8_t *buf;

    DEBUGASSERT(rxBuff);

    /* Handle empty read. */
    if (!rxSize) {
        return kStatus_Success;
    }

    /* Wait until there is room in the command fifo. */
    result = imxrt_lpi2c_masterwaitfortxready(base);
    if (result) {
        return result;
    }

    /* Issue command to receive data. */
    base->MTDR = kRxDataCmd | LPI2C_MTDR_DATA(rxSize - 1);

#if LPI2C_WAIT_TIMEOUT
    uint32_t waitTimes = LPI2C_WAIT_TIMEOUT;
#endif

    /* Receive data */
    buf = (uint8_t *)rxBuff;
    while (rxSize--) {
        /* Read LPI2C receive fifo register. The register includes a flag to indicate whether */
        /* the FIFO is empty, so we can both get the data and check if we need to keep reading */
        /* using a single register read. */
        uint32_t value;
        do {
            /* Check for errors. */
            result = imxrt_lpi2c_mastercheckandclearerror(base, imxrt_lpi2c_mastergetstatusflags(base));
            if (result) {
                return result;
            }

            value = base->MRDR;
#if LPI2C_WAIT_TIMEOUT
        } while ((value & LPI2C_MRDR_RXEMPTY_MASK) && (--waitTimes));
        if (waitTimes == 0) {
            return kStatus_LPI2C_Timeout;
        }
#else
        } while (value & LPI2C_MRDR_RXEMPTY_MASK);
#endif

        *buf++ = value & LPI2C_MRDR_DATA_MASK;
    }

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastersend
 *
 * Description:
 *   brief Performs a polling send transfer on the I2C bus.
 *
 *   Sends up to a txSize number of bytes to the previously addressed slave device. The slave may
 *   reply with a NAK to any byte in order to terminate the transfer early. If this happens, this
 *   function returns #kStatus_LPI2C_Nak.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   txBuff The pointer to the data to be transferred.
 *   txSize The length in bytes of the data to be transferred.
 *
 * Returned Value:
 *   Returns #kStatus_Success Data was sent successfully.
 *   Returns #kStatus_LPI2C_Busy Another master is currently utilizing the bus.
 *   Returns #kStatus_LPI2C_Nak The slave device sent a NAK in response to a byte.
 *   Returns #kStatus_LPI2C_FifoError FIFO under run or overrun.
 *   Returns #kStatus_LPI2C_ArbitrationLost Arbitration lost error.
 *   Returns #kStatus_LPI2C_PinLowTimeout SCL or SDA were held low longer than the timeout.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_mastersend(LPI2C_Type *base, void *txBuff, size_t txSize)
{
    uint8_t *buf = (uint8_t *)txBuff;

    DEBUGASSERT(txBuff);

    /* Send data buffer */
    while (txSize--) {
        /* Wait until there is room in the fifo. This also checks for errors. */
        status_t result = imxrt_lpi2c_masterwaitfortxready(base);
        if (result)
        {
            return result;
        }

        /* Write byte into LPI2C master data register. */
        base->MTDR = *buf++;
    }

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastertransferblocking
 *
 * Description:
 *   brief Performs a master polling transfer on the I2C bus.
 *
 *   note The API does not return until the transfer succeeds or fails due
 *   to error happens during transfer.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   transfer Pointer to the transfer structure.
 *
 * Returned Value:
 *   Returns #kStatus_Success Data was received successfully.
 *   Returns #kStatus_LPI2C_Busy Another master is currently utilizing the bus.
 *   Returns #kStatus_LPI2C_Nak The slave device sent a NAK in response to a byte.
 *   Returns #kStatus_LPI2C_FifoError FIFO under run or overrun.
 *   Returns #kStatus_LPI2C_ArbitrationLost Arbitration lost error.
 *   Returns #kStatus_LPI2C_PinLowTimeout SCL or SDA were held low longer than the timeout.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_mastertransferblocking(LPI2C_Type *base, lpi2c_master_transfer_t *transfer)
{
    status_t result = kStatus_Success;
    uint16_t commandBuffer[7];
    uint32_t cmdCount = 0;

    DEBUGASSERT(transfer);
    DEBUGASSERT(transfer->subaddressSize <= sizeof(transfer->subaddress));

    /* Return an error if the bus is already in use not by us. */
    result = imxrt_lpi2c_checkforbusybus(base);
    if (result) {
        return result;
    }

    /* Clear all flags. */
    imxrt_lpi2c_masterclearstatusflags(base, kMasterClearFlags);

    /* Turn off auto-stop option. */
    base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;

    lpi2c_direction_t direction = transfer->subaddressSize ? kLPI2C_Write : transfer->direction;
    if (!(transfer->flags & kLPI2C_TransferNoStartFlag)) {
        commandBuffer[cmdCount++] =
            (uint16_t)kStartCmd | (uint16_t)((uint16_t)((uint16_t)transfer->slaveAddress << 1U) | (uint16_t)direction);
    }

    /* Subaddress, MSB first. */
    if (transfer->subaddressSize) {
        uint32_t subaddressRemaining = transfer->subaddressSize;
        while (subaddressRemaining--)
        {
            uint8_t subaddressByte = (transfer->subaddress >> (8 * subaddressRemaining)) & 0xff;
            commandBuffer[cmdCount++] = subaddressByte;
        }
    }

    /* Reads need special handling. */
    if ((transfer->dataSize) && (transfer->direction == kLPI2C_Read)) {
        /* Need to send repeated start if switching directions to read. */
        if (direction == kLPI2C_Write) {
            commandBuffer[cmdCount++] =
                (uint16_t)kStartCmd |
                (uint16_t)((uint16_t)((uint16_t)transfer->slaveAddress << 1U) | (uint16_t)kLPI2C_Read);
        }
    }

    /* Send command buffer */
    uint32_t index = 0;
    while (cmdCount--) {
        /* Wait until there is room in the fifo. This also checks for errors. */
        result = imxrt_lpi2c_masterwaitfortxready(base);
        if (result) {
            return result;
        }

        /* Write byte into LPI2C master data register. */
        base->MTDR = commandBuffer[index];
        index++;
    }

    /* Transmit data. */
    if ((transfer->direction == kLPI2C_Write) && (transfer->dataSize > 0)) {
        /* Send Data. */
        result = imxrt_lpi2c_mastersend(base, transfer->data, transfer->dataSize);
    }

    /* Receive Data. */
    if ((transfer->direction == kLPI2C_Read) && (transfer->dataSize > 0)) {
        result = imxrt_lpi2c_masterreceive(base, transfer->data, transfer->dataSize);
    }

    if (result) {
        return result;
    }

    if ((transfer->flags & kLPI2C_TransferNoStopFlag) == 0) {
        result = imxrt_lpi2c_masterstop(base);
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastertransfercreatehandle
 *
 * Description:
 *   brief Creates a new handle for the LPI2C master non-blocking APIs.
 *
 *   The creation of a handle is for use with the non-blocking APIs. Once a handle
 *   is created, there is not a corresponding destroy handle. If the user wants to
 *   terminate a transfer, the imxrt_lpi2c_mastertransferabort() API shall be called.
 *
 *
 *   note The function also enables the NVIC IRQ for the input LPI2C. Need to notice
 *   that on some SoCs the LPI2C IRQ is connected to INTMUX, in this case user needs to
 *   enable the associated INTMUX IRQ in application.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle[out] Pointer to the LPI2C master driver handle.
 *   callback User provided pointer to the asynchronous callback function.
 *   userData User provided pointer to the application callback data.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_mastertransfercreatehandle(LPI2C_Type *base,
                                      lpi2c_master_handle_t *handle,
                                      lpi2c_master_transfer_callback_t callback,
                                      void *userData)
{
    uint32_t instance;

    DEBUGASSERT(handle);

    /* Clear out the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = imxrt_lpi2c_getinstance(base);

    /* Save base and instance. */
    handle->completionCallback = callback;
    handle->userData = userData;

    /* Save this handle for IRQ use. */
    s_lpi2cMasterHandle[instance] = handle;

    /* Set irq handler. */
    s_lpi2cMasterIsr = imxrt_lpi2c_mastertransferhandleirq;

    /* Clear internal IRQ enables and enable NVIC IRQ. */
    imxrt_lpi2c_masterdisableinterrupts(base, kMasterIrqFlags);

    /* Enable NVIC IRQ, this only enables the IRQ directly connected to the NVIC.
     In some cases the LPI2C IRQ is configured through INTMUX, user needs to enable
     INTMUX IRQ in application code. */
    up_enable_irq(kLpi2cIrqs[instance]);
}

/****************************************************************************
 * Function: imxrt_lpi2c_runtransferstatemachine
 *
 * Description:
 *   brief Execute states until FIFOs are exhausted.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Master nonblocking driver handle.
 *   isDone Set to true if the transfer has completed.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_LPI2C_PinLowTimeout
 *   Returns #kStatus_LPI2C_ArbitrationLost
 *   Returns #kStatus_LPI2C_Nak
 *   Returns #kStatus_LPI2C_FifoError
 *
 ****************************************************************************/
static status_t imxrt_lpi2c_runtransferstatemachine(LPI2C_Type *base, lpi2c_master_handle_t *handle, bool *isDone)
{
    uint32_t status;
    status_t result = kStatus_Success;
    lpi2c_master_transfer_t *xfer;
    size_t txCount;
    size_t rxCount;
    size_t txFifoSize = FSL_FEATURE_LPI2C_FIFO_SIZEn(base);
    bool state_complete = false;

    /* Set default isDone return value. */
    *isDone = false;

    /* Check for errors. */
    status = imxrt_lpi2c_mastergetstatusflags(base);
    result = imxrt_lpi2c_mastercheckandclearerror(base, status);
    if (result) {
        return result;
    }

    /* Get pointer to private data. */
    xfer = &handle->transfer;

    /* Get fifo counts and compute room in tx fifo. */
    imxrt_lpi2c_mastergetfifocounts(base, &rxCount, &txCount);
    txCount = txFifoSize - txCount;

    while (!state_complete) {
        /* Execute the state. */
        switch (handle->state) {
        case kSendCommandState: {
            /* Make sure there is room in the tx fifo for the next command. */
            if (!txCount--) {
                state_complete = true;
                break;
            }

            /* Issue command. buf is a uint8_t* pointing at the uint16 command array. */
            base->MTDR = *(uint16_t *)handle->buf;
            handle->buf += sizeof(uint16_t);

            /* Count down until all commands are sent. */
            if (--handle->remainingBytes == 0) {
                /* Choose next state and set up buffer pointer and count. */
                if (xfer->dataSize) {
                    /* Either a send or receive transfer is next. */
                    handle->state = kTransferDataState;
                    handle->buf = (uint8_t *)xfer->data;
                    handle->remainingBytes = xfer->dataSize;
                    if (xfer->direction == kLPI2C_Read) {
                        /* Disable TX interrupt */
                        imxrt_lpi2c_masterdisableinterrupts(base, kLPI2C_MasterTxReadyFlag);
                    }
                } else {
                    /* No transfer, so move to stop state. */
                    handle->state = kStopState;
                }
            }
            break;
        }
        case kIssueReadCommandState:
            /* Make sure there is room in the tx fifo for the read command. */
            if (!txCount--) {
                state_complete = true;
                break;
            }

            base->MTDR = kRxDataCmd | LPI2C_MTDR_DATA(xfer->dataSize - 1);

            /* Move to transfer state. */
            handle->state = kTransferDataState;
            if (xfer->direction == kLPI2C_Read) {
                /* Disable TX interrupt */
                imxrt_lpi2c_masterdisableinterrupts(base, kLPI2C_MasterTxReadyFlag);
            }
            break;
        case kTransferDataState:
            if (xfer->direction == kLPI2C_Write) {
                /* Make sure there is room in the tx fifo. */
                if (!txCount--) {
                    state_complete = true;
                    break;
                }

                /* Put byte to send in fifo. */
                base->MTDR = *(handle->buf)++;
            } else {
                /* XXX handle receive sizes > 256, use kIssueReadCommandState */
                /* Make sure there is data in the rx fifo. */
                if (!rxCount--) {
                    state_complete = true;
                    break;
                }

                /* Read byte from fifo. */
                *(handle->buf)++ = base->MRDR & LPI2C_MRDR_DATA_MASK;
            }

            /* Move to stop when the transfer is done. */
            if (--handle->remainingBytes == 0) {
                handle->state = kStopState;
            }
            break;
        case kStopState:
            /* Only issue a stop transition if the caller requested it. */
            if ((xfer->flags & kLPI2C_TransferNoStopFlag) == 0) {
                /* Make sure there is room in the tx fifo for the stop command. */
                if (!txCount--) {
                    state_complete = true;
                    break;
                }

                base->MTDR = kStopCmd;
            } else {
                /* Caller doesn't want to send a stop, so we're done now. */
                *isDone = true;
                state_complete = true;
                break;
            }
            handle->state = kWaitForCompletionState;
            break;
        case kWaitForCompletionState:
            /* We stay in this state until the stop state is detected. */
            if (status & kLPI2C_MasterStopDetectFlag) {
                *isDone = true;
            }
            state_complete = true;
            break;
        default:
            DEBUGASSERT(false);
            break;
        }
    }
    return result;
}

/****************************************************************************
 * Function: imxrt_lpi2c_inittransferstatemachine
 *
 * Description:
 *   brief Prepares the transfer state machine and fills in the command buffer.
 *
 * Input Parameters:
 *   handle Master nonblocking driver handle.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void imxrt_lpi2c_inittransferstatemachine(lpi2c_master_handle_t *handle)
{
    lpi2c_master_transfer_t *xfer = &handle->transfer;

    /* Handle no start option. */
    if (xfer->flags & kLPI2C_TransferNoStartFlag) {
        if (xfer->direction == kLPI2C_Read) {
            /* Need to issue read command first. */
            handle->state = kIssueReadCommandState;
        } else {
            /* Start immediately in the data transfer state. */
            handle->state = kTransferDataState;
        }

        handle->buf = (uint8_t *)xfer->data;
        handle->remainingBytes = xfer->dataSize;
    } else {
        uint16_t *cmd = (uint16_t *)&handle->commandBuffer;
        uint32_t cmdCount = 0;

        /* Initial direction depends on whether a subaddress was provided, and of course the actual */
        /* data transfer direction. */
        lpi2c_direction_t direction = xfer->subaddressSize ? kLPI2C_Write : xfer->direction;

        /* Start command. */
        cmd[cmdCount++] =
            (uint16_t)kStartCmd | (uint16_t)((uint16_t)((uint16_t)xfer->slaveAddress << 1U) | (uint16_t)direction);

        /* Subaddress, MSB first. */
        if (xfer->subaddressSize) {
            uint32_t subaddressRemaining = xfer->subaddressSize;
            while (subaddressRemaining--) {
                uint8_t subaddressByte = (xfer->subaddress >> (8 * subaddressRemaining)) & 0xff;
                cmd[cmdCount++] = subaddressByte;
            }
        }

        /* Reads need special handling. */
        if ((xfer->dataSize) && (xfer->direction == kLPI2C_Read)) {
            /* Need to send repeated start if switching directions to read. */
            if (direction == kLPI2C_Write) {
                cmd[cmdCount++] = (uint16_t)kStartCmd |
                                  (uint16_t)((uint16_t)((uint16_t)xfer->slaveAddress << 1U) | (uint16_t)kLPI2C_Read);
            }

            /* Read command. */
            cmd[cmdCount++] = kRxDataCmd | LPI2C_MTDR_DATA(xfer->dataSize - 1);
        }

        /* Set up state machine for transferring the commands. */
        handle->state = kSendCommandState;
        handle->remainingBytes = cmdCount;
        handle->buf = (uint8_t *)&handle->commandBuffer;
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastertransfernonblocking
 *
 * Description:
 *   brief Performs a non-blocking transaction on the I2C bus.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to the LPI2C master driver handle.
 *   transfer The pointer to the transfer descriptor.
 *
 * Returned Value:
 *   Returns #kStatus_Success The transaction was started successfully.
 *   Returns #kStatus_LPI2C_Busy Either another master is currently utilizing the bus, or a non-blocking
 *           transaction is already in progress.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_mastertransfernonblocking(LPI2C_Type *base,
                                         lpi2c_master_handle_t *handle,
                                         lpi2c_master_transfer_t *transfer)
{
    status_t result;

    DEBUGASSERT(handle);
    DEBUGASSERT(transfer);
    DEBUGASSERT(transfer->subaddressSize <= sizeof(transfer->subaddress));

    /* Return busy if another transaction is in progress. */
    if (handle->state != kIdleState) {
        return kStatus_LPI2C_Busy;
    }

    /* Return an error if the bus is already in use not by us. */
    result = imxrt_lpi2c_checkforbusybus(base);
    if (result) {
        return result;
    }

    /* Disable LPI2C IRQ sources while we configure stuff. */
    imxrt_lpi2c_masterdisableinterrupts(base, kMasterIrqFlags);

    /* Save transfer into handle. */
    handle->transfer = *transfer;

    /* Generate commands to send. */
    imxrt_lpi2c_inittransferstatemachine(handle);

    /* Clear all flags. */
    imxrt_lpi2c_masterclearstatusflags(base, kMasterClearFlags);

    /* Turn off auto-stop option. */
    base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;

    /* Enable LPI2C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
    imxrt_lpi2c_masterenableInterrupts(base, kMasterIrqFlags);

    return result;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastertransfergetcount
 *
 * Description:
 *   brief Returns number of bytes transferred so far.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to the LPI2C master driver handle.
 *   count[out] Number of bytes transferred so far by the non-blocking transaction.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_mastertransfergetcount(LPI2C_Type *base, lpi2c_master_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    if (!count) {
        return kStatus_InvalidArgument;
    }

    /* Catch when there is not an active transfer. */
    if (handle->state == kIdleState) {
        *count = 0;
        return kStatus_NoTransferInProgress;
    }

    uint8_t state;
    uint16_t remainingBytes;
    uint32_t dataSize;

    /* Cache some fields with IRQs disabled. This ensures all field values */
    /* are synchronized with each other during an ongoing transfer. */
    uint32_t irqs = imxrt_lpi2c_mastergetenableinterrupts(base);
    imxrt_lpi2c_masterdisableinterrupts(base, irqs);
    state = handle->state;
    remainingBytes = handle->remainingBytes;
    dataSize = handle->transfer.dataSize;
    imxrt_lpi2c_masterenableInterrupts(base, irqs);

    /* Get transfer count based on current transfer state. */
    switch (state) {
    case kIdleState:
    case kSendCommandState:
    case kIssueReadCommandState: /* XXX return correct value for this state when >256 reads are supported */
        *count = 0;
        break;
    case kTransferDataState:
        *count = dataSize - remainingBytes;
        break;
    case kStopState:
    case kWaitForCompletionState:
    default:
        *count = dataSize;
        break;
    }

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastertransferabort
 *
 * Description:
 *   brief Terminates a non-blocking LPI2C master transmission early.
 *
 *   note It is not safe to call this function from an IRQ handler that has a higher priority than the
 *        LPI2C peripheral's IRQ priority.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to the LPI2C master driver handle.
 *
 * Returned Value:
 *   Returns #kStatus_Success A transaction was successfully aborted.
 *   Returns #kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 *
 ****************************************************************************/
void imxrt_lpi2c_mastertransferabort(LPI2C_Type *base, lpi2c_master_handle_t *handle)
{
    if (handle->state != kIdleState) {
        /* Disable internal IRQ enables. */
        imxrt_lpi2c_masterdisableinterrupts(base, kMasterIrqFlags);

        /* Reset fifos. */
        base->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;

        /* Send a stop command to finalize the transfer. */
        base->MTDR = kStopCmd;

        /* Reset handle. */
        handle->state = kIdleState;
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_mastertransferhandleirq
 *
 * Description:
 *   brief Reusable routine to handle master interrupts.
 *   
 *   note This function does not need to be called unless you are reimplementing the
 *    nonblocking API's interrupt handler routines to add special functionality.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to the LPI2C master driver handle.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_mastertransferhandleirq(LPI2C_Type *base, lpi2c_master_handle_t *handle)
{
    bool isDone;
    status_t result;

    /* Don't do anything if we don't have a valid handle. */
    if (!handle) {
        return;
    }

    if (handle->state == kIdleState) {
        return;
    }

    result = imxrt_lpi2c_runtransferstatemachine(base, handle, &isDone);

    if (isDone || (result != kStatus_Success)) {
        /* XXX need to handle data that may be in rx fifo below watermark level? */

        /* XXX handle error, terminate xfer */

        /* Disable internal IRQ enables. */
        imxrt_lpi2c_masterdisableinterrupts(base, kMasterIrqFlags);

        /* Set handle to idle state. */
        handle->state = kIdleState;

        /* Invoke callback. */
        if (handle->completionCallback) {
            handle->completionCallback(base, handle, result, handle->userData);
        }
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavegetdefaultconfig
 *
 * Description:
 *   brief Provides a default configuration for the LPI2C slave peripheral.
 *
 *   This function provides the following default configuration for the LPI2C slave peripheral:
 *   code
 *    slaveConfig->enableSlave               = true;
 *    slaveConfig->address0                  = 0U;
 *    slaveConfig->address1                  = 0U;
 *    slaveConfig->addressMatchMode          = kLPI2C_MatchAddress0;
 *    slaveConfig->filterDozeEnable          = true;
 *    slaveConfig->filterEnable              = true;
 *    slaveConfig->enableGeneralCall         = false;
 *    slaveConfig->sclStall.enableAck        = false;
 *    slaveConfig->sclStall.enableTx         = true;
 *    slaveConfig->sclStall.enableRx         = true;
 *    slaveConfig->sclStall.enableAddress    = true;
 *    slaveConfig->ignoreAck                 = false;
 *    slaveConfig->enableReceivedAddressRead = false;
 *    slaveConfig->sdaGlitchFilterWidth_ns   = 0;
 *    slaveConfig->sclGlitchFilterWidth_ns   = 0;
 *    slaveConfig->dataValidDelay_ns         = 0;
 *    slaveConfig->clockHoldTime_ns          = 0;
 *   endcode
 *
 *   After calling this function, override any settings  to customize the configuration,
 *   prior to initializing the master driver with imxrt_lpi2c_slaveinit(). Be sure to override at least the a
 *   address0 member of the configuration structure with the desired slave address.
 *
 * Input Parameters:
 *   slaveConfig[out] User provided configuration structure that is set to default values. Refer to
 *                    #lpi2c_slave_config_t.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_slavegetdefaultconfig(lpi2c_slave_config_t *slaveConfig)
{
    /* Initializes the configure structure to zero. */
    memset(slaveConfig, 0, sizeof(*slaveConfig));

    slaveConfig->enableSlave = true;
    slaveConfig->address0 = 0U;
    slaveConfig->address1 = 0U;
    slaveConfig->addressMatchMode = kLPI2C_MatchAddress0;
    slaveConfig->filterDozeEnable = true;
    slaveConfig->filterEnable = true;
    slaveConfig->enableGeneralCall = false;
    slaveConfig->sclStall.enableAck = false;
    slaveConfig->sclStall.enableTx = true;
    slaveConfig->sclStall.enableRx = true;
    slaveConfig->sclStall.enableAddress = false;
    slaveConfig->ignoreAck = false;
    slaveConfig->enableReceivedAddressRead = false;
    slaveConfig->sdaGlitchFilterWidth_ns = 0; /* TODO determine default width values */
    slaveConfig->sclGlitchFilterWidth_ns = 0;
    slaveConfig->dataValidDelay_ns = 0;
    slaveConfig->clockHoldTime_ns = 0;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slaveinit
 *
 * Description:
 *   brief Initializes the LPI2C slave peripheral.
 *
 *   This function enables the peripheral clock and initializes the LPI2C slave peripheral as described by the user
 *   provided configuration.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   slaveConfig User provided peripheral configuration. Use imxrt_lpi2c_slavegetdefaultconfig() to get a set of defaults
 *               that you can override.
 *   sourceClock_Hz Frequency in Hertz of the LPI2C functional clock. Used to calculate the filter widths,
 *                  data valid delay, and clock hold time.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_slaveinit(LPI2C_Type *base, const lpi2c_slave_config_t *slaveConfig, uint32_t sourceClock_Hz)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)

    uint32_t instance = imxrt_lpi2c_getinstance(base);

    /* Ungate the clock. */
    imxrt_clock_enableclock(kLpi2cClocks[instance]);
#if defined(LPI2C_PERIPH_CLOCKS)
    /* Ungate the functional clock in initialize function. */
    imxrt_clock_enableclock(kLpi2cPeriphClocks[instance]);
#endif

#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Restore to reset conditions. */
    imxrt_lpi2c_slavereset(base);

    /* Configure peripheral. */
    base->SAMR = LPI2C_SAMR_ADDR0(slaveConfig->address0) | LPI2C_SAMR_ADDR1(slaveConfig->address1);

    base->SCFGR1 =
        LPI2C_SCFGR1_ADDRCFG(slaveConfig->addressMatchMode) | LPI2C_SCFGR1_IGNACK(slaveConfig->ignoreAck) |
        LPI2C_SCFGR1_RXCFG(slaveConfig->enableReceivedAddressRead) | LPI2C_SCFGR1_GCEN(slaveConfig->enableGeneralCall) |
        LPI2C_SCFGR1_ACKSTALL(slaveConfig->sclStall.enableAck) | LPI2C_SCFGR1_TXDSTALL(slaveConfig->sclStall.enableTx) |
        LPI2C_SCFGR1_RXSTALL(slaveConfig->sclStall.enableRx) |
        LPI2C_SCFGR1_ADRSTALL(slaveConfig->sclStall.enableAddress);

    base->SCFGR2 =
        LPI2C_SCFGR2_FILTSDA(imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, slaveConfig->sdaGlitchFilterWidth_ns,
                                                     (LPI2C_SCFGR2_FILTSDA_MASK >> LPI2C_SCFGR2_FILTSDA_SHIFT), 1)) |
        LPI2C_SCFGR2_FILTSCL(imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, slaveConfig->sclGlitchFilterWidth_ns,
                                                     (LPI2C_SCFGR2_FILTSCL_MASK >> LPI2C_SCFGR2_FILTSCL_SHIFT), 1)) |
        LPI2C_SCFGR2_DATAVD(imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, slaveConfig->dataValidDelay_ns,
                                                    (LPI2C_SCFGR2_DATAVD_MASK >> LPI2C_SCFGR2_DATAVD_SHIFT), 1)) |
        LPI2C_SCFGR2_CLKHOLD(imxrt_lpi2c_getcyclesforwidth(sourceClock_Hz, slaveConfig->clockHoldTime_ns,
                                                     (LPI2C_SCFGR2_CLKHOLD_MASK >> LPI2C_SCFGR2_CLKHOLD_SHIFT), 1));

    /* Save SCR to last so we don't enable slave until it is configured */
    base->SCR = LPI2C_SCR_FILTDZ(slaveConfig->filterDozeEnable) | LPI2C_SCR_FILTEN(slaveConfig->filterEnable) |
                LPI2C_SCR_SEN(slaveConfig->enableSlave);
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavedeinit
 *
 * Description:
 *   brief Deinitializes the LPI2C slave peripheral.
 *
 *   This function disables the LPI2C slave peripheral and gates the clock. It also performs a software
 *   reset to restore the peripheral to reset conditions.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_slavedeinit(LPI2C_Type *base)
{
    imxrt_lpi2c_slavereset(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)

    uint32_t instance = imxrt_lpi2c_getinstance(base);

    /* Gate the clock. */
    imxrt_clock_disableclock(kLpi2cClocks[instance]);

#if defined(LPI2C_PERIPH_CLOCKS)
    /* Gate the functional clock. */
    imxrt_clock_disableclock(kLpi2cPeriphClocks[instance]);
#endif

#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavecheckandclearerror
 *
 * Description:
 *   brief Convert provided flags to status code, and clear any errors if present.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   status Current status flags value that will be checked.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_LPI2C_BitError
 *   Returns #kStatus_LPI2C_FifoError
 *
 ****************************************************************************/
static status_t imxrt_lpi2c_slavecheckandclearerror(LPI2C_Type *base, uint32_t flags)
{
    status_t result = kStatus_Success;

    flags &= kSlaveErrorFlags;
    if (flags) {
        if (flags & kLPI2C_SlaveBitErrFlag) {
            result = kStatus_LPI2C_BitError;
        } else if (flags & kLPI2C_SlaveFifoErrFlag) {
            result = kStatus_LPI2C_FifoError;
        } else {
            DEBUGASSERT(false);
        }

        /* Clear the errors. */
        imxrt_lpi2c_slaveclearstatusflags(base, flags);
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavesend
 *
 * Description:
 *   brief Performs a polling send transfer on the I2C bus.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   txBuff The pointer to the data to be transferred.
 *   txSize The length in bytes of the data to be transferred.
 *   actualTxSize[out]
 *
 * Returned Value:
 *   Returns Error or success status returned by API.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_slavesend(LPI2C_Type *base, void *txBuff, size_t txSize, size_t *actualTxSize)
{
    uint8_t *buf = (uint8_t *)txBuff;
    size_t remaining = txSize;

    DEBUGASSERT(txBuff);

#if LPI2C_WAIT_TIMEOUT
    uint32_t waitTimes = LPI2C_WAIT_TIMEOUT;
#endif

    while (remaining) {
        uint32_t flags;
        status_t result;

        /* Wait until we can transmit. */
        do {
            /* Check for errors */
            flags = imxrt_lpi2c_slavegetstatusflags(base);
            result = imxrt_lpi2c_slavecheckandclearerror(base, flags);
            if (result) {
                if (actualTxSize) {
                    *actualTxSize = txSize - remaining;
                }
                return result;
            }
#if LPI2C_WAIT_TIMEOUT
        } while (
            (!(flags & (kLPI2C_SlaveTxReadyFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag))) &&
            (--waitTimes));
        if (waitTimes == 0) {
            return kStatus_LPI2C_Timeout;
        }
#else
        } while (
            !(flags & (kLPI2C_SlaveTxReadyFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag)));
#endif

        /* Send a byte. */
        if (flags & kLPI2C_SlaveTxReadyFlag) {
            base->STDR = *buf++;
            --remaining;
        }

        /* Exit loop if we see a stop or restart */
        if (flags & (kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag)) {
            imxrt_lpi2c_slaveclearstatusflags(base, kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag);
            break;
        }
    }

    if (actualTxSize) {
        *actualTxSize = txSize - remaining;
    }

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavereceive
 *
 * Description:
 *   brief Performs a polling receive transfer on the I2C bus.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   rxBuff The pointer to the data to be transferred.
 *   rxSize The length in bytes of the data to be transferred.
 *   actualRxSize[out]
 *
 * Returned Value:
 *   Returns Error or success status returned by API.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_slavereceive(LPI2C_Type *base, void *rxBuff, size_t rxSize, size_t *actualRxSize)
{
    uint8_t *buf = (uint8_t *)rxBuff;
    size_t remaining = rxSize;

    DEBUGASSERT(rxBuff);

#if LPI2C_WAIT_TIMEOUT
    uint32_t waitTimes = LPI2C_WAIT_TIMEOUT;
#endif

    while (remaining) {
        uint32_t flags;
        status_t result;

        /* Wait until we can receive. */
        do {
            /* Check for errors */
            flags = imxrt_lpi2c_slavegetstatusflags(base);
            result = imxrt_lpi2c_slavecheckandclearerror(base, flags);
            if (result) {
                if (actualRxSize)
                {
                    *actualRxSize = rxSize - remaining;
                }
                return result;
            }
#if LPI2C_WAIT_TIMEOUT
        } while (
            (!(flags & (kLPI2C_SlaveRxReadyFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag))) &&
            (--waitTimes));
        if (waitTimes == 0) {
            return kStatus_LPI2C_Timeout;
        }
#else
        } while (
            !(flags & (kLPI2C_SlaveRxReadyFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag)));
#endif

        /* Receive a byte. */
        if (flags & kLPI2C_SlaveRxReadyFlag) {
            *buf++ = base->SRDR & LPI2C_SRDR_DATA_MASK;
            --remaining;
        }

        /* Exit loop if we see a stop or restart */
        if (flags & (kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag)) {
            imxrt_lpi2c_slaveclearstatusflags(base, kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag);
            break;
        }
    }

    if (actualRxSize) {
        *actualRxSize = rxSize - remaining;
    }

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavereceive
 *
 * Description:
 *   brief Creates a new handle for the LPI2C slave non-blocking APIs.
 *
 *   The creation of a handle is for use with the non-blocking APIs. Once a handle
 *   is created, there is not a corresponding destroy handle. If the user wants to
 *   terminate a transfer, the imxrt_lpi2c_slavetransferabort() API shall be called.
 *
 *   note The function also enables the NVIC IRQ for the input LPI2C. Need to notice
 *   that on some SoCs the LPI2C IRQ is connected to INTMUX, in this case user needs to
 *   enable the associated INTMUX IRQ in application.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle[out] Pointer to the LPI2C slave driver handle.
 *   callback User provided pointer to the asynchronous callback function.
 *   userData User provided pointer to the application callback data.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_slavetransfercreatehandle(LPI2C_Type *base,
                                     lpi2c_slave_handle_t *handle,
                                     lpi2c_slave_transfer_callback_t callback,
                                     void *userData)
{
    uint32_t instance;

    DEBUGASSERT(handle);

    /* Clear out the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = imxrt_lpi2c_getinstance(base);

    /* Save base and instance. */
    handle->callback = callback;
    handle->userData = userData;

    /* Save this handle for IRQ use. */
    s_lpi2cSlaveHandle[instance] = handle;

    /* Set irq handler. */
    s_lpi2cSlaveIsr = imxrt_lpi2c_slavetransferhandleirq;

    /* Clear internal IRQ enables and enable NVIC IRQ. */
    imxrt_lpi2c_slavedisableinterrupts(base, kSlaveIrqFlags);
    up_enable_irq(kLpi2cIrqs[instance]);

    /* Nack by default. */
    base->STAR = LPI2C_STAR_TXNACK_MASK;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavetransfernonblocking
 *
 * Description:
 *   brief Starts accepting slave transfers.
 *
 *   Call this API after calling I2C_SlaveInit() and imxrt_lpi2c_slavetransfercreatehandle() to start processing
 *   transactions driven by an I2C master. The slave monitors the I2C bus and pass events to the
 *   callback that was passed into the call to imxrt_lpi2c_slavetransfercreatehandle(). The callback is always invoked
 *   from the interrupt context.
 *
 *   The set of events received by the callback is customizable. To do so, set the a eventMask parameter to
 *   the OR'd combination of #lpi2c_slave_transfer_event_t enumerators for the events you wish to receive.
 *   The #kLPI2C_SlaveTransmitEvent and #kLPI2C_SlaveReceiveEvent events are always enabled and do not need
 *   to be included in the mask. Alternatively, you can pass 0 to get a default set of only the transmit and
 *   receive events that are always enabled. In addition, the #kLPI2C_SlaveAllEvents constant is provided as
 *   a convenient way to enable all events.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to #lpi2c_slave_handle_t structure which stores the transfer state.
 *   eventMask Bit mask formed by OR'ing together #lpi2c_slave_transfer_event_t enumerators to specify
 *             which events to send to the callback. Other accepted values are 0 to get a default set of
 *             only the transmit and receive events, and #kLPI2C_SlaveAllEvents to enable all events.
 *
 * Returned Value:
 *   Returns #kStatus_Success Slave transfers were successfully started.
 *   Returns #kStatus_LPI2C_Busy Slave transfers have already been started on this handle.
 *
 ****************************************************************************/
status_t imxrt_lpi2c_slavetransfernonblocking(LPI2C_Type *base, lpi2c_slave_handle_t *handle, uint32_t eventMask)
{
    uint32_t status;

    DEBUGASSERT(handle);

    /* Return busy if another transaction is in progress. */
    if (handle->isBusy) {
        return kStatus_LPI2C_Busy;
    }

    /* Return an error if the bus is already in use not by us. */
    status = imxrt_lpi2c_slavegetstatusflags(base);
    if ((status & kLPI2C_SlaveBusBusyFlag) && (!(status & kLPI2C_SlaveBusyFlag))) {
        return kStatus_LPI2C_Busy;
    }

    /* Disable LPI2C IRQ sources while we configure stuff. */
    imxrt_lpi2c_slavedisableinterrupts(base, kSlaveIrqFlags);

    /* Clear transfer in handle. */
    memset(&handle->transfer, 0, sizeof(handle->transfer));

    /* Record that we're busy. */
    handle->isBusy = true;

    /* Set up event mask. tx and rx are always enabled. */
    handle->eventMask = eventMask | kLPI2C_SlaveTransmitEvent | kLPI2C_SlaveReceiveEvent;

    /* Ack by default. */
    base->STAR = 0;

    /* Clear all flags. */
    imxrt_lpi2c_slaveclearstatusflags(base, kSlaveClearFlags);

    /* Enable LPI2C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
    imxrt_lpi2c_slaveenableinterrupts(base, kSlaveIrqFlags);

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavetransfergetcount
 *
 * Description:
 *   brief Gets the slave transfer status during a non-blocking transfer.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to i2c_slave_handle_t structure.
 *   count[out] Pointer to a value to hold the number of bytes transferred. May be NULL if the count is not
 *      required.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_NoTransferInProgress
 *
 ****************************************************************************/
status_t imxrt_lpi2c_slavetransfergetcount(LPI2C_Type *base, lpi2c_slave_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    if (!count) {
        return kStatus_InvalidArgument;
    }

    /* Catch when there is not an active transfer. */
    if (!handle->isBusy) {
        *count = 0;
        return kStatus_NoTransferInProgress;
    }

    /* For an active transfer, just return the count from the handle. */
    *count = handle->transferredCount;

    return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavetransferabort
 *
 * Description:
 *   brief Aborts the slave non-blocking transfers.
 *   note This API could be called at any time to stop slave for handling the bus events.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to #lpi2c_slave_handle_t structure which stores the transfer state.
 *
 * Returned Value:
 *   Returns #kStatus_Success
 *   Returns #kStatus_LPI2C_Idle
 *
 ****************************************************************************/
void imxrt_lpi2c_slavetransferabort(LPI2C_Type *base, lpi2c_slave_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Return idle if no transaction is in progress. */
    if (handle->isBusy) {
        /* Disable LPI2C IRQ sources. */
        imxrt_lpi2c_slavedisableinterrupts(base, kSlaveIrqFlags);

        /* Nack by default. */
        base->STAR = LPI2C_STAR_TXNACK_MASK;

        /* Reset transfer info. */
        memset(&handle->transfer, 0, sizeof(handle->transfer));

        /* We're no longer busy. */
        handle->isBusy = false;
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavetransferhandleirq
 *
 * Description:
 *   brief Reusable routine to handle slave interrupts.
 *   note This function does not need to be called unless you are reimplementing the
 *      non blocking API's interrupt handler routines to add special functionality.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   handle Pointer to #lpi2c_slave_handle_t structure which stores the transfer state.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_lpi2c_slavetransferhandleirq(LPI2C_Type *base, lpi2c_slave_handle_t *handle)
{
    uint32_t flags;
    lpi2c_slave_transfer_t *xfer;

    /* Check for a valid handle in case of a spurious interrupt. */
    if (!handle) {
        return;
    }

    xfer = &handle->transfer;

    /* Get status flags. */
    flags = imxrt_lpi2c_slavegetstatusflags(base);

    if (flags & (kLPI2C_SlaveBitErrFlag | kLPI2C_SlaveFifoErrFlag)) {
        xfer->event = kLPI2C_SlaveCompletionEvent;
        xfer->completionStatus = imxrt_lpi2c_slavecheckandclearerror(base, flags);

        if ((handle->eventMask & kLPI2C_SlaveCompletionEvent) && (handle->callback)) {
            handle->callback(base, xfer, handle->userData);
        }
        return;
    }
    if (flags & (kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag)) {
        xfer->event = (flags & kLPI2C_SlaveRepeatedStartDetectFlag) ? kLPI2C_SlaveRepeatedStartEvent :
                                                                      kLPI2C_SlaveCompletionEvent;
        xfer->receivedAddress = 0;
        xfer->completionStatus = kStatus_Success;
        xfer->transferredCount = handle->transferredCount;

        if (xfer->event == kLPI2C_SlaveCompletionEvent) {
            handle->isBusy = false;
        }

        if (handle->wasTransmit) {
            /* Subtract one from the transmit count to offset the fact that LPI2C asserts the */
            /* tx flag before it sees the nack from the master-receiver, thus causing one more */
            /* count that the master actually receives. */
            --xfer->transferredCount;
            handle->wasTransmit = false;
        }

        /* Clear the flag. */
        imxrt_lpi2c_slaveclearstatusflags(base, flags & (kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag));

        /* Revert to sending an Ack by default, in case we sent a Nack for receive. */
        base->STAR = 0;

        if ((handle->eventMask & xfer->event) && (handle->callback)) {
            handle->callback(base, xfer, handle->userData);
        }

        /* Clean up transfer info on completion, after the callback has been invoked. */
        memset(&handle->transfer, 0, sizeof(handle->transfer));
    }
    if (flags & kLPI2C_SlaveAddressValidFlag) {
        xfer->event = kLPI2C_SlaveAddressMatchEvent;
        xfer->receivedAddress = base->SASR & LPI2C_SASR_RADDR_MASK;

        if ((handle->eventMask & kLPI2C_SlaveAddressMatchEvent) && (handle->callback)) {
            handle->callback(base, xfer, handle->userData);
        }
    }
    if (flags & kLPI2C_SlaveTransmitAckFlag) {
        xfer->event = kLPI2C_SlaveTransmitAckEvent;

        if ((handle->eventMask & kLPI2C_SlaveTransmitAckEvent) && (handle->callback)) {
            handle->callback(base, xfer, handle->userData);
        }
    }

    /* Handle transmit and receive. */
    if (flags & kLPI2C_SlaveTxReadyFlag) {
        handle->wasTransmit = true;

        /* If we're out of data, invoke callback to get more. */
        if ((!xfer->data) || (!xfer->dataSize)) {
            xfer->event = kLPI2C_SlaveTransmitEvent;
            if (handle->callback) {
                handle->callback(base, xfer, handle->userData);
            }

            /* Clear the transferred count now that we have a new buffer. */
            handle->transferredCount = 0;
        }

        /* Transmit a byte. */
        if ((xfer->data) && (xfer->dataSize)) {
            base->STDR = *xfer->data++;
            --xfer->dataSize;
            ++handle->transferredCount;
        }
    }
    if (flags & kLPI2C_SlaveRxReadyFlag) {
        /* If we're out of room in the buffer, invoke callback to get another. */
        if ((!xfer->data) || (!xfer->dataSize)) {
            xfer->event = kLPI2C_SlaveReceiveEvent;
            if (handle->callback) {
                handle->callback(base, xfer, handle->userData);
            }

            /* Clear the transferred count now that we have a new buffer. */
            handle->transferredCount = 0;
        }

        /* Receive a byte. */
        if ((xfer->data) && (xfer->dataSize)) {
            *xfer->data++ = base->SRDR;
            --xfer->dataSize;
            ++handle->transferredCount;
        } else {
            /* We don't have any room to receive more data, so send a nack. */
            base->STAR = LPI2C_STAR_TXNACK_MASK;
        }
    }
}

/****************************************************************************
 * Function: imxrt_lpi2c_slavetransferhandleirq
 *
 * Description:
 *   brief Shared IRQ handler that can call both master and slave ISRs.
 *
 *   The master and slave ISRs are called through function pointers in order to decouple
 *   this code from the ISR functions. Without this, the linker would always pull in both
 *   ISRs and every function they call, even if only the functional API was used.
 *
 * Input Parameters:
 *   base The LPI2C peripheral base address.
 *   instance The LPI2C peripheral instance number.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void imxrt_lpi2c_commonirqhandler(LPI2C_Type *base, uint32_t instance)
{
    /* Check for master IRQ. */
    if ((base->MCR & LPI2C_MCR_MEN_MASK) && s_lpi2cMasterIsr) {
        /* Master mode. */
        s_lpi2cMasterIsr(base, s_lpi2cMasterHandle[instance]);
    }

    /* Check for slave IRQ. */
    if ((base->SCR & LPI2C_SCR_SEN_MASK) && s_lpi2cSlaveIsr) {
        /* Slave mode. */
        s_lpi2cSlaveIsr(base, s_lpi2cSlaveHandle[instance]);
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

#if defined(LPI2C0)
/* Implementation of LPI2C0 handler named in startup code. */
void LPI2C0_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(LPI2C0, 0);
}
#endif

#if defined(LPI2C1)
/* Implementation of LPI2C1 handler named in startup code. */
void LPI2C1_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(LPI2C1, 1);
}
#endif

#if defined(LPI2C2)
/* Implementation of LPI2C2 handler named in startup code. */
void LPI2C2_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(LPI2C2, 2);
}
#endif

#if defined(LPI2C3)
/* Implementation of LPI2C3 handler named in startup code. */
void LPI2C3_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(LPI2C3, 3);
}
#endif

#if defined(LPI2C4)
/* Implementation of LPI2C4 handler named in startup code. */
void LPI2C4_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(LPI2C4, 4);
}
#endif

#if defined(CM4_0__LPI2C)
/* Implementation of CM4_0__LPI2C handler named in startup code. */
void M4_0_LPI2C_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(CM4_0__LPI2C, imxrt_lpi2c_getinstance(CM4_0__LPI2C));
}
#endif

#if defined(CM4__LPI2C)
/* Implementation of CM4__LPI2C handler named in startup code. */
void M4_LPI2C_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(CM4__LPI2C, imxrt_lpi2c_getinstance(CM4__LPI2C));
}
#endif

#if defined(CM4_1__LPI2C)
/* Implementation of CM4_1__LPI2C handler named in startup code. */
void M4_1_LPI2C_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(CM4_1__LPI2C, imxrt_lpi2c_getinstance(CM4_1__LPI2C));
}
#endif

#if defined(DMA__LPI2C0)
/* Implementation of DMA__LPI2C0 handler named in startup code. */
void DMA_I2C0_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(DMA__LPI2C0, imxrt_lpi2c_getinstance(DMA__LPI2C0));
}
#endif

#if defined(DMA__LPI2C1)
/* Implementation of DMA__LPI2C1 handler named in startup code. */
void DMA_I2C1_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(DMA__LPI2C1, imxrt_lpi2c_getinstance(DMA__LPI2C1));
}
#endif

#if defined(DMA__LPI2C2)
/* Implementation of DMA__LPI2C2 handler named in startup code. */
void DMA_I2C2_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(DMA__LPI2C2, imxrt_lpi2c_getinstance(DMA__LPI2C2));
}
#endif

#if defined(DMA__LPI2C3)
/* Implementation of DMA__LPI2C3 handler named in startup code. */
void DMA_I2C3_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(DMA__LPI2C3, imxrt_lpi2c_getinstance(DMA__LPI2C3));
}
#endif

#if defined(DMA__LPI2C4)
/* Implementation of DMA__LPI2C3 handler named in startup code. */
void DMA_I2C4_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(DMA__LPI2C4, imxrt_lpi2c_getinstance(DMA__LPI2C4));
}
#endif

#if defined(ADMA__LPI2C0)
/* Implementation of DMA__LPI2C0 handler named in startup code. */
void ADMA_I2C0_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(ADMA__LPI2C0, imxrt_lpi2c_getinstance(ADMA__LPI2C0));
}
#endif

#if defined(ADMA__LPI2C1)
/* Implementation of DMA__LPI2C1 handler named in startup code. */
void ADMA_I2C1_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(ADMA__LPI2C1, imxrt_lpi2c_getinstance(ADMA__LPI2C1));
}
#endif

#if defined(ADMA__LPI2C2)
/* Implementation of DMA__LPI2C2 handler named in startup code. */
void ADMA_I2C2_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(ADMA__LPI2C2, imxrt_lpi2c_getinstance(ADMA__LPI2C2));
}
#endif

#if defined(ADMA__LPI2C3)
/* Implementation of DMA__LPI2C3 handler named in startup code. */
void ADMA_I2C3_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(ADMA__LPI2C3, imxrt_lpi2c_getinstance(ADMA__LPI2C3));
}
#endif

#if defined(ADMA__LPI2C4)
/* Implementation of DMA__LPI2C3 handler named in startup code. */
void ADMA_I2C4_INT_DriverIRQHandler(void)
{
    imxrt_lpi2c_commonirqhandler(ADMA__LPI2C4, imxrt_lpi2c_getinstance(ADMA__LPI2C4));
}
#endif
