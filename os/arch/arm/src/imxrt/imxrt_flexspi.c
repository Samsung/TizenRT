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

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tinyara/irq.h>

#include "irq/irq.h"

#include "imxrt_flexspi.h"
#include "imxrt_periphclks.h"
#include "imxrt_clock.h"
#include "imxrt_log.h"

/*******************************************************************************
 * Definitations
 ******************************************************************************/

#define FREQ_1MHz (1000000UL)
#define FLEXSPI_DLLCR_DEFAULT (0x100UL)
#define FLEXSPI_LUT_KEY_VAL (0x5AF05AF0ul)

enum {
    kFLEXSPI_DelayCellUnitMin = 75,  /* 75ps. */
    kFLEXSPI_DelayCellUnitMax = 225, /* 225ps. */
};

/*! @brief Common sets of flags used by the driver. */
enum _flexspi_flag_constants {
    /*! IRQ sources enabled by the non-blocking transactional API. */
    kIrqFlags = kFLEXSPI_IpTxFifoWatermarkEmpltyFlag | kFLEXSPI_IpRxFifoWatermarkAvailableFlag |
                kFLEXSPI_SequenceExecutionTimeoutFlag | kFLEXSPI_IpCommandSequenceErrorFlag |
                kFLEXSPI_IpCommandGrantTimeoutFlag | kFLEXSPI_IpCommandExcutionDoneFlag,

    /*! Errors to check for. */
    kErrorFlags = kFLEXSPI_SequenceExecutionTimeoutFlag | kFLEXSPI_IpCommandSequenceErrorFlag |
                  kFLEXSPI_IpCommandGrantTimeoutFlag,
};

enum _flexspi_transfer_state {
    kFLEXSPI_Idle = 0x0U,      /*!< Transfer is done. */
    kFLEXSPI_BusyWrite = 0x1U, /*!< FLEXSPI is busy write transfer. */
    kFLEXSPI_BusyRead = 0x2U,  /*!< FLEXSPI is busy write transfer. */
};

/*! @brief Typedef for interrupt handler. */
typedef void (*flexspi_isr_t)(FLEXSPI_Type *base, void *flexspiHandle);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
* @brief Get the instance number for FLEXSPI.
*
* @param base FLEXSPI base pointer.
*/
uint32_t imxrt_flexspi_getinstance(FLEXSPI_Type *base);

/*!
* @brief Configure flash A/B sample clock DLL.
*
* @param base FLEXSPI base pointer.
* @param config Flash configuration parameters.
*/
static uint32_t imxrt_flexspi_configuredll(FLEXSPI_Type *base, flexspi_device_config_t *config);

/*!
* @brief Check and clear IP command execution errors.
*
* @param base FLEXSPI base pointer.
* @param status interrupt status.
*/
status_t imxrt_flexspi_checkandclearerror(FLEXSPI_Type *base, uint32_t status);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to flexspi handles for each instance. */
static void *s_flexspiHandle[FSL_FEATURE_SOC_FLEXSPI_COUNT];

/*! @brief Pointers to flexspi bases for each instance. */
static FLEXSPI_Type *const s_flexspiBases[] = FLEXSPI_BASE_PTRS;

/*! @brief Pointers to flexspi IRQ number for each instance. */
static const IRQn_Type s_flexspiIrqs[] = FLEXSPI_IRQS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name array */
static const clock_ip_name_t s_flexspiClock[] = FLEXSPI_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
/****************************************************************************
 * Function: imxrt_flexspi_getinstance
 *
 * Description:
 *   brief Get the instance number for FLEXSPI.
 *
 * Input Parameters:
 *   base FLEXSPI base pointer.
 *
 * Returned Value:
 *   Returns the value of instance
 * 
 ****************************************************************************/
uint32_t imxrt_flexspi_getinstance(FLEXSPI_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_FLEXSPI_COUNT; instance++) {
        if (s_flexspiBases[instance] == base) {
            break;
        }
    }

    DEBUGASSERT(instance < FSL_FEATURE_SOC_FLEXSPI_COUNT);

    return instance;
}

/****************************************************************************
 * Function: imxrt_flexspi_configuredll
 *
 * Description:
 *   brief Configure flash A/B sample clock DLL.
 *
 * Input Parameters:
 *   base FLEXSPI base pointer.
 *   config Flash configuration parameters.
 *
 * Returned Value:
 *   Returns the value of flexspi dll
 * 
 ****************************************************************************/
static uint32_t imxrt_flexspi_configuredll(FLEXSPI_Type *base, flexspi_device_config_t *config)
{
    bool isUnifiedConfig = true;
    uint32_t flexspiDllValue;
    uint32_t dllValue;
    uint32_t temp;

    uint8_t rxSampleClock = (base->MCR0 & FLEXSPI_MCR0_RXCLKSRC_MASK) >> FLEXSPI_MCR0_RXCLKSRC_SHIFT;
    switch (rxSampleClock)
    {
    case kFLEXSPI_ReadSampleClkLoopbackInternally:
    case kFLEXSPI_ReadSampleClkLoopbackFromDqsPad:
    case kFLEXSPI_ReadSampleClkLoopbackFromSckPad:
        isUnifiedConfig = true;
        break;
    case kFLEXSPI_ReadSampleClkExternalInputFromDqsPad:
        if (config->isSck2Enabled) {
            isUnifiedConfig = true;
        } else {
            isUnifiedConfig = false;
        }
        break;
    default:
        break;
    }

    if (isUnifiedConfig) {
        flexspiDllValue = FLEXSPI_DLLCR_DEFAULT; /* 1 fixed delay cells in DLL delay chain) */
    } else {
        if (config->flexspiRootClk >= 100 * FREQ_1MHz) {
            /* DLLEN = 1, SLVDLYTARGET = 0xF, */
            flexspiDllValue = FLEXSPI_DLLCR_DLLEN(1) | FLEXSPI_DLLCR_SLVDLYTARGET(0x0F);
        } else {
            temp = config->dataValidTime * 1000; /* Convert data valid time in ns to ps. */
            dllValue = temp / kFLEXSPI_DelayCellUnitMin;
            if (dllValue * kFLEXSPI_DelayCellUnitMin < temp) {
                dllValue++;
            }
            flexspiDllValue = FLEXSPI_DLLCR_OVRDEN(1) | FLEXSPI_DLLCR_OVRDVAL(dllValue);
        }
    }
    return flexspiDllValue;
}

/****************************************************************************
 * Function: imxrt_flexspi_configuredll
 *
 * Description:
 *   brief Check and clear IP command execution errors.
 *
 * Input Parameters:
 *   base FLEXSPI base pointer.
 *   status interrupt status.
 *
 * Returned Value:
 *   Returns the value of status
 * 
 ****************************************************************************/
status_t imxrt_flexspi_checkandclearerror(FLEXSPI_Type *base, uint32_t status)
{
    status_t result = kStatus_Success;

    /* Check for error. */
    status &= kErrorFlags;
    if (status) {
        /* Select the correct error code.. */
        if (status & kFLEXSPI_SequenceExecutionTimeoutFlag) {
            result = kStatus_FLEXSPI_SequenceExecutionTimeout;
        } else if (status & kFLEXSPI_IpCommandSequenceErrorFlag) {
            result = kStatus_FLEXSPI_IpCommandSequenceError;
        } else if (status & kFLEXSPI_IpCommandGrantTimeoutFlag) {
            result = kStatus_FLEXSPI_IpCommandGrantTimeout;
        } else {
            DEBUGASSERT(false);
        }

        /* Clear the flags. */
        imxrt_flexspi_clearinterruptstatusflags(base, status);

        /* Reset fifos. These flags clear automatically. */
        base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
        base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_flexspi_init
 *
 * Description:
 *   brief Initializes the FLEXSPI module and internal state.
 *
 *   This function enables the clock for FLEXSPI and also configures the FLEXSPI with the
 *   input configure parameters. Users should call this function before any FLEXSPI operations.
 *
 * Input Parameters:
 *   base FLEXSPI base pointer.
 *   config FLEXSPI configure structure.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_init(FLEXSPI_Type *base, const flexspi_config_t *config)
{
    uint32_t configValue = 0;
    uint8_t i = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the flexspi clock */
    imxrt_clock_enableclock(s_flexspiClock[imxrt_flexspi_getinstance(base)]);

#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Reset peripheral before configuring it. */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    imxrt_flexspi_softwarereset(base);

    /* Configure MCR0 configuration items. */
    configValue = FLEXSPI_MCR0_RXCLKSRC(config->rxSampleClock) | FLEXSPI_MCR0_DOZEEN(config->enableDoze) |
                  FLEXSPI_MCR0_IPGRANTWAIT(config->ipGrantTimeoutCycle) |
                  FLEXSPI_MCR0_AHBGRANTWAIT(config->ahbConfig.ahbGrantTimeoutCycle) |
                  FLEXSPI_MCR0_SCKFREERUNEN(config->enableSckFreeRunning) |
                  FLEXSPI_MCR0_HSEN(config->enableHalfSpeedAccess) |
                  FLEXSPI_MCR0_COMBINATIONEN(config->enableCombination) |
                  FLEXSPI_MCR0_ATDFEN(config->ahbConfig.enableAHBWriteIpTxFifo) |
                  FLEXSPI_MCR0_ARDFEN(config->ahbConfig.enableAHBWriteIpRxFifo) | FLEXSPI_MCR0_MDIS_MASK;
    base->MCR0 = configValue;

    /* Configure MCR1 configurations. */
    configValue =
        FLEXSPI_MCR1_SEQWAIT(config->seqTimeoutCycle) | FLEXSPI_MCR1_AHBBUSWAIT(config->ahbConfig.ahbBusTimeoutCycle);
    base->MCR1 = configValue;

    /* Configure MCR2 configurations. */
    configValue = base->MCR2;
    configValue &= ~(FLEXSPI_MCR2_RESUMEWAIT_MASK | FLEXSPI_MCR2_SCKBDIFFOPT_MASK | FLEXSPI_MCR2_SAMEDEVICEEN_MASK |
                     FLEXSPI_MCR2_CLRAHBBUFOPT_MASK);
    configValue |= FLEXSPI_MCR2_RESUMEWAIT(config->ahbConfig.resumeWaitCycle) |
                   FLEXSPI_MCR2_SCKBDIFFOPT(config->enableSckBDiffOpt) |
                   FLEXSPI_MCR2_SAMEDEVICEEN(config->enableSameConfigForAll) |
                   FLEXSPI_MCR2_CLRAHBBUFOPT(config->ahbConfig.enableClearAHBBufferOpt);

    base->MCR2 = configValue;

    /* Configure AHB control items. */
    configValue = base->AHBCR;
    configValue &= ~(FLEXSPI_AHBCR_READADDROPT_MASK | FLEXSPI_AHBCR_PREFETCHEN_MASK | FLEXSPI_AHBCR_BUFFERABLEEN_MASK |
                     FLEXSPI_AHBCR_CACHABLEEN_MASK);
    configValue |= FLEXSPI_AHBCR_READADDROPT(config->ahbConfig.enableReadAddressOpt) |
                   FLEXSPI_AHBCR_PREFETCHEN(config->ahbConfig.enableAHBPrefetch) |
                   FLEXSPI_AHBCR_BUFFERABLEEN(config->ahbConfig.enableAHBBufferable) |
                   FLEXSPI_AHBCR_CACHABLEEN(config->ahbConfig.enableAHBCachable);
    base->AHBCR = configValue;

    /* Configure AHB rx buffers. */
    for (i = 0; i < FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT; i++) {
        configValue = base->AHBRXBUFCR0[i];

        configValue &= ~(FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK |
                         FLEXSPI_AHBRXBUFCR0_MSTRID_MASK | FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK);
        configValue |= FLEXSPI_AHBRXBUFCR0_PREFETCHEN(config->ahbConfig.buffer[i].enablePrefetch) |
                       FLEXSPI_AHBRXBUFCR0_PRIORITY(config->ahbConfig.buffer[i].priority) |
                       FLEXSPI_AHBRXBUFCR0_MSTRID(config->ahbConfig.buffer[i].masterIndex) |
                       FLEXSPI_AHBRXBUFCR0_BUFSZ(config->ahbConfig.buffer[i].bufferSize / 8);
        base->AHBRXBUFCR0[i] = configValue;
    }

    /* Configure IP Fifo watermarks. */
    base->IPRXFCR &= ~FLEXSPI_IPRXFCR_RXWMRK_MASK;
    base->IPRXFCR |= FLEXSPI_IPRXFCR_RXWMRK(config->rxWatermark / 8 - 1);
    base->IPTXFCR &= ~FLEXSPI_IPTXFCR_TXWMRK_MASK;
    base->IPTXFCR |= FLEXSPI_IPTXFCR_TXWMRK(config->txWatermark / 8 - 1);

    /* Reset flash size on all ports */
    for (i = 0; i < kFLEXSPI_PortCount; i++) {
        base->FLSHCR0[i] = 0;
    }
}

/****************************************************************************
 * Function: imxrt_flexspi_getdefaultconfig
 *
 * Description:
 *   brief Gets default settings for FLEXSPI.
 *
 * Input Parameters:
 *   config FLEXSPI configuration structure.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_getdefaultconfig(flexspi_config_t *config)
{
    /* Initializes the configure structure to zero. */
    memset(config, 0, sizeof(*config));

    config->rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackInternally;
    config->enableSckFreeRunning = false;
    config->enableCombination = false;
    config->enableDoze = true;
    config->enableHalfSpeedAccess = false;
    config->enableSckBDiffOpt = false;
    config->enableSameConfigForAll = false;
    config->seqTimeoutCycle = 0xFFFFU;
    config->ipGrantTimeoutCycle = 0xFFU;
    config->txWatermark = 8;
    config->rxWatermark = 8;
    config->ahbConfig.enableAHBWriteIpTxFifo = false;
    config->ahbConfig.enableAHBWriteIpRxFifo = false;
    config->ahbConfig.ahbGrantTimeoutCycle = 0xFFU;
    config->ahbConfig.ahbBusTimeoutCycle = 0xFFFFU;
    config->ahbConfig.resumeWaitCycle = 0x20U;
    memset(config->ahbConfig.buffer, 0, sizeof(config->ahbConfig.buffer));
    for (uint8_t i = 0; i < FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT; i++) {
        config->ahbConfig.buffer[i].bufferSize = 256; /* Default buffer size 256 bytes*/
    }
    config->ahbConfig.enableClearAHBBufferOpt = false;
    config->ahbConfig.enableReadAddressOpt = false;
    config->ahbConfig.enableAHBPrefetch = false;
    config->ahbConfig.enableAHBBufferable = false;
    config->ahbConfig.enableAHBCachable = false;
}

/****************************************************************************
 * Function: imxrt_flexspi_deinit
 *
 * Description:
 *   brief Deinitializes the FLEXSPI module.
 *
 *   Clears the FLEXSPI state and  FLEXSPI module registers.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_deinit(FLEXSPI_Type *base)
{
    /* Reset peripheral. */
    imxrt_flexspi_softwarereset(base);
}

/****************************************************************************
 * Function: imxrt_flexspi_setflashconfig
 *
 * Description:
 *   brief Configures the connected device parameter.
 *
 *   This function configures the connected device relevant parameters, such as the size, command, and so on.
 *   The flash configuration value cannot have a default value. The user needs to configure it according to the
 *   connected device.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   config Flash configuration parameters.
 *   port FLEXSPI Operation port.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_setflashconfig(FLEXSPI_Type *base, flexspi_device_config_t *config, flexspi_port_t port)
{
    uint32_t configValue = 0;
    uint8_t index = port >> 1; /* PortA with index 0, PortB with index 1. */

    /* Wait for bus idle before change flash configuration. */
    while (!imxrt_flexspi_getbusidlestatus(base)) {
    }

    /* Configure flash size. */
    base->FLSHCR0[port] = config->flashSize;

    /* Configure flash parameters. */
    base->FLSHCR1[port] = FLEXSPI_FLSHCR1_CSINTERVAL(config->CSInterval) |
                          FLEXSPI_FLSHCR1_CSINTERVALUNIT(config->CSIntervalUnit) |
                          FLEXSPI_FLSHCR1_TCSH(config->CSHoldTime) | FLEXSPI_FLSHCR1_TCSS(config->CSSetupTime) |
                          FLEXSPI_FLSHCR1_CAS(config->columnspace) | FLEXSPI_FLSHCR1_WA(config->enableWordAddress);

    /* Configure AHB operation items. */
    configValue = base->FLSHCR2[port];

    configValue &= ~(FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK | FLEXSPI_FLSHCR2_AWRWAIT_MASK | FLEXSPI_FLSHCR2_AWRSEQNUM_MASK |
                     FLEXSPI_FLSHCR2_AWRSEQID_MASK | FLEXSPI_FLSHCR2_ARDSEQNUM_MASK | FLEXSPI_FLSHCR2_AWRSEQID_MASK);

    configValue |=
        FLEXSPI_FLSHCR2_AWRWAITUNIT(config->AHBWriteWaitUnit) | FLEXSPI_FLSHCR2_AWRWAIT(config->AHBWriteWaitInterval);

    if (config->AWRSeqNumber > 0U) {
        configValue |=
            FLEXSPI_FLSHCR2_AWRSEQID(config->AWRSeqIndex) | FLEXSPI_FLSHCR2_AWRSEQNUM(config->AWRSeqNumber - 1U);
    }

    if (config->ARDSeqNumber > 0U) {
        configValue |=
            FLEXSPI_FLSHCR2_ARDSEQID(config->ARDSeqIndex) | FLEXSPI_FLSHCR2_ARDSEQNUM(config->ARDSeqNumber - 1U);
    }

    base->FLSHCR2[port] = configValue;

    /* Configure DLL. */
    base->DLLCR[index] = imxrt_flexspi_configuredll(base, config);

    /* Configure write mask. */
    if (config->enableWriteMask) {
        base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMOPT1_MASK;
    } else {
        base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMOPT1_MASK;
    }

    if (index == 0) { /*PortA*/
        base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMENA_MASK;
        base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMENA(config->enableWriteMask);
    } else {
        base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMENB_MASK;
        base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMENB(config->enableWriteMask);
    }

    /* Exit stop mode. */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
}

/****************************************************************************
 * Function: imxrt_flexspi_updatelut
 *
 * Description:
 *   brief Updates the LUT table.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   index From which index start to update. It could be any index of the LUT table, which
 *     also allows user to update command content inside a command. Each command consists of up to
 *     8 instructions and occupy 4*32-bit memory.
 *   cmd Command sequence array.
 *   count Number of sequences.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_updatelut(FLEXSPI_Type *base, uint32_t index, const uint32_t *cmd, uint32_t count)
{
    DEBUGASSERT(index < 64U);

    uint8_t i = 0;
    volatile uint32_t *lutBase;

    /* Wait for bus idle before change flash configuration. */
    while (!imxrt_flexspi_getbusidlestatus(base)) {
    }

    /* Unlock LUT for update. */
    base->LUTKEY = FLEXSPI_LUT_KEY_VAL;
    base->LUTCR = 0x02;

    lutBase = &base->LUT[index];
    for (i = index; i < count; i++) {
        *lutBase++ = *cmd++;
    }

    /* Lock LUT. */
    base->LUTKEY = FLEXSPI_LUT_KEY_VAL;
    base->LUTCR = 0x01;
}

/****************************************************************************
 * Function: imxrt_flexspi_writeblocking
 *
 * Description:
 *   brief Sends a buffer of data bytes using blocking method.
 *   note This function blocks via polling until all bytes have been sent.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address
 *   buffer The data bytes to send
 *   size The number of data bytes to send
 *
 * Returned Value:
 *   Returns kStatus_Success write success without error
 *   Returns kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 *   Returns kStatus_FLEXSPI_IpCommandSequenceError IP command sequencen error detected
 *   Returns kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 * 
 ****************************************************************************/
status_t imxrt_flexspi_writeblocking(FLEXSPI_Type *base, uint32_t *buffer, size_t size)
{
    uint8_t txWatermark = ((base->IPTXFCR & FLEXSPI_IPTXFCR_TXWMRK_MASK) >> FLEXSPI_IPTXFCR_TXWMRK_SHIFT) + 1;
    uint32_t status;
    status_t result = kStatus_Success;
    uint32_t i = 0;

    /* Send data buffer */
    while (size) {
        /* Wait until there is room in the fifo. This also checks for errors. */
        while (!((status = base->INTR) & kFLEXSPI_IpTxFifoWatermarkEmpltyFlag)) {
        }

        result = imxrt_flexspi_checkandclearerror(base, status);

        if (result) {
            return result;
        }

        /* Write watermark level data into tx fifo . */
        if (size >= 8 * txWatermark) {
            for (i = 0; i < 2 * txWatermark; i++) {
                base->TFDR[i] = *buffer++;
            }

            size = size - 8 * txWatermark;
        } else {
            for (i = 0; i < (size / 4 + 1); i++) {
                base->TFDR[i] = *buffer++;
            }
            size = 0;
        }

        /* Push a watermark level datas into IP TX FIFO. */
        base->INTR |= kFLEXSPI_IpTxFifoWatermarkEmpltyFlag;
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_flexspi_readblocking
 *
 * Description:
 *   brief Receives a buffer of data bytes using a blocking method.
 *   note This function blocks via polling until all bytes have been sent.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address
 *   buffer The data bytes to send
 *   size The number of data bytes to receive
 *
 * Returned Value:
 *   Returns kStatus_Success read success without error
 *   Returns kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 *   Returns kStatus_FLEXSPI_IpCommandSequenceError IP command sequencen error detected
 *   Returns kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 * 
 ****************************************************************************/
status_t imxrt_flexspi_readblocking(FLEXSPI_Type *base, uint32_t *buffer, size_t size)
{
    uint8_t rxWatermark = ((base->IPRXFCR & FLEXSPI_IPRXFCR_RXWMRK_MASK) >> FLEXSPI_IPRXFCR_RXWMRK_SHIFT) + 1;
    uint32_t status;
    status_t result = kStatus_Success;
    uint32_t i = 0;

    /* Send data buffer */
    while (size) {
        if (size >= 8 * rxWatermark) {
            /* Wait until there is room in the fifo. This also checks for errors. */
            while (!((status = base->INTR) & kFLEXSPI_IpRxFifoWatermarkAvailableFlag)) {
                result = imxrt_flexspi_checkandclearerror(base, status);

                if (result) {
                    return result;
                }
            }
        } else {
            /* Wait fill level. This also checks for errors. */
            while (size > ((((base->IPRXFSTS) & FLEXSPI_IPRXFSTS_FILL_MASK) >> FLEXSPI_IPRXFSTS_FILL_SHIFT) * 8U)) {
                result = imxrt_flexspi_checkandclearerror(base, base->INTR);

                if (result) {
                    return result;
                }
            }
        }

        result = imxrt_flexspi_checkandclearerror(base, base->INTR);

        if (result) {
            return result;
        }

        /* Read watermark level data from rx fifo . */
        if (size >= 8 * rxWatermark) {
            for (i = 0; i < 2 * rxWatermark; i++) {
                *buffer++ = base->RFDR[i];
            }

            size = size - 8 * rxWatermark;
        } else {
            for (i = 0; i < (size / 4 + 1); i++) {
                *buffer++ = base->RFDR[i];
            }
            size = 0;
        }

        /* Pop out a watermark level datas from IP RX FIFO. */
        base->INTR |= kFLEXSPI_IpRxFifoWatermarkAvailableFlag;
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_flexspi_transferblocking
 *
 * Description:
 *   brief Execute command to transfer a buffer data bytes using a blocking method.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address
 *   xfer pointer to the transfer structure.
 *
 * Returned Value:
 *   Returns kStatus_Success command transfer success without error
 *   Returns kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 *   Returns kStatus_FLEXSPI_IpCommandSequenceError IP command sequencen error detected
 *   Returns kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 * 
 ****************************************************************************/
status_t imxrt_flexspi_transferblocking(FLEXSPI_Type *base, flexspi_transfer_t *xfer)
{
    uint32_t configValue = 0;
    status_t result = kStatus_Success;

    /* Clear sequence pointer before sending data to external devices. */
    base->FLSHCR2[xfer->port] |= FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK;

    /* Clear former pending status before start this tranfer. */
    base->INTR |= FLEXSPI_INTR_AHBCMDERR_MASK | FLEXSPI_INTR_IPCMDERR_MASK | FLEXSPI_INTR_AHBCMDGE_MASK |
                  FLEXSPI_INTR_IPCMDGE_MASK;

    /* Configure base addresss. */
    base->IPCR0 = xfer->deviceAddress;

    /* Reset fifos. */
    base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
    base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;

    /* Configure data size. */
    if ((xfer->cmdType == kFLEXSPI_Read) || (xfer->cmdType == kFLEXSPI_Write) || (xfer->cmdType == kFLEXSPI_Config)) {
        configValue = FLEXSPI_IPCR1_IDATSZ(xfer->dataSize);
    }

    /* Configure sequence ID. */
    configValue |= FLEXSPI_IPCR1_ISEQID(xfer->seqIndex) | FLEXSPI_IPCR1_ISEQNUM(xfer->SeqNumber - 1);
    base->IPCR1 = configValue;

    /* Start Transfer. */
    base->IPCMD |= FLEXSPI_IPCMD_TRG_MASK;

    if ((xfer->cmdType == kFLEXSPI_Write) || (xfer->cmdType == kFLEXSPI_Config)) {
        result = imxrt_flexspi_writeblocking(base, xfer->data, xfer->dataSize);
    } else if (xfer->cmdType == kFLEXSPI_Read) {
        result = imxrt_flexspi_readblocking(base, xfer->data, xfer->dataSize);
    } else {
    }

    /* Wait for bus idle. */
    while (!imxrt_flexspi_getbusidlestatus(base)) {
    }

    if (xfer->cmdType == kFLEXSPI_Command) {
        result = imxrt_flexspi_checkandclearerror(base, base->INTR);
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_flexspi_transfercreatehandle
 *
 * Description:
 *   brief Initializes the FLEXSPI handle which is used in transactional functions.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   handle pointer to flexspi_handle_t structure to store the transfer state.
 *   callback pointer to user callback function.
 *   userData user parameter passed to the callback function.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_transfercreatehandle(FLEXSPI_Type *base,
                                  flexspi_handle_t *handle,
                                  flexspi_transfer_callback_t callback,
                                  void *userData)
{
    DEBUGASSERT(handle);

    uint32_t instance = imxrt_flexspi_getinstance(base);

    /* Zero handle. */
    memset(handle, 0, sizeof(*handle));

    /* Set callback and userData. */
    handle->completionCallback = callback;
    handle->userData = userData;

    /* Save the context in global variables to support the double weak mechanism. */
    s_flexspiHandle[instance] = handle;

    /* Enable NVIC interrupt. */
    up_disable_irq(s_flexspiIrqs[instance]);
}

/****************************************************************************
 * Function: imxrt_flexspi_transfernonblocking
 *
 * Description:
 *   brief Performs a interrupt non-blocking transfer on the FLEXSPI bus.
 *
 *   note Calling the API returns immediately after transfer initiates. The user needs
 *   to call FLEXSPI_GetTransferCount to poll the transfer status to check whether
 *   the transfer is finished. If the return status is not kStatus_FLEXSPI_Busy, the transfer
 *   is finished. For FLEXSPI_Read, the dataSize should be multiple of rx watermark levle, or
 *   FLEXSPI could not read data properly.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   handle pointer to flexspi_handle_t structure which stores the transfer state.
 *   xfer pointer to flexspi_transfer_t structure.
 *
 * Returned Value:
 *   Returns kStatus_Success Successfully start the data transmission.
 *   Returns kStatus_FLEXSPI_Busy Previous transmission still not finished.
 * 
 ****************************************************************************/
status_t imxrt_flexspi_transfernonblocking(FLEXSPI_Type *base, flexspi_handle_t *handle, flexspi_transfer_t *xfer)
{
    uint32_t configValue = 0;
    status_t result = kStatus_Success;

    DEBUGASSERT(handle);
    DEBUGASSERT(xfer);

    /* Check if the I2C bus is idle - if not return busy status. */
    if (handle->state != kFLEXSPI_Idle) {
        result = kStatus_FLEXSPI_Busy;
    } else {
        handle->data = xfer->data;
        handle->dataSize = xfer->dataSize;
        handle->transferTotalSize = xfer->dataSize;
        handle->state = (xfer->cmdType == kFLEXSPI_Read) ? kFLEXSPI_BusyRead : kFLEXSPI_BusyWrite;

        /* Clear sequence pointer before sending data to external devices. */
        base->FLSHCR2[xfer->port] |= FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK;

        /* Clear former pending status before start this tranfer. */
        base->INTR |= FLEXSPI_INTR_AHBCMDERR_MASK | FLEXSPI_INTR_IPCMDERR_MASK | FLEXSPI_INTR_AHBCMDGE_MASK |
                      FLEXSPI_INTR_IPCMDGE_MASK;

        /* Configure base addresss. */
        base->IPCR0 = xfer->deviceAddress;

        /* Reset fifos. */
        base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
        base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;

        /* Configure data size. */
        if ((xfer->cmdType == kFLEXSPI_Read) || (xfer->cmdType == kFLEXSPI_Write)) {
            configValue = FLEXSPI_IPCR1_IDATSZ(xfer->dataSize);
        }

        /* Configure sequence ID. */
        configValue |= FLEXSPI_IPCR1_ISEQID(xfer->seqIndex) | FLEXSPI_IPCR1_ISEQNUM(xfer->SeqNumber - 1);
        base->IPCR1 = configValue;

        /* Start Transfer. */
        base->IPCMD |= FLEXSPI_IPCMD_TRG_MASK;

        if (handle->state == kFLEXSPI_BusyRead) {
            imxrt_flexspi_enableinterrupts(base, kFLEXSPI_IpRxFifoWatermarkAvailableFlag |
                                               kFLEXSPI_SequenceExecutionTimeoutFlag |
                                               kFLEXSPI_IpCommandSequenceErrorFlag |
                                               kFLEXSPI_IpCommandGrantTimeoutFlag | kFLEXSPI_IpCommandExcutionDoneFlag);
        } else {
            imxrt_flexspi_enableinterrupts(base, kFLEXSPI_IpTxFifoWatermarkEmpltyFlag |
                                               kFLEXSPI_SequenceExecutionTimeoutFlag |
                                               kFLEXSPI_IpCommandSequenceErrorFlag |
                                               kFLEXSPI_IpCommandGrantTimeoutFlag | kFLEXSPI_IpCommandExcutionDoneFlag);
        }
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_flexspi_tranfergetcount
 *
 * Description:
 *   brief Gets the master transfer status during a interrupt non-blocking transfer.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   handle pointer to flexspi_handle_t structure which stores the transfer state.
 *   count Number of bytes transferred so far by the non-blocking transaction.
 *
 * Returned Value:
 *   Returns kStatus_InvalidArgument count is Invalid.
 *   Returns kStatus_Success Successfully return the count.
 * 
 ****************************************************************************/
status_t imxrt_flexspi_tranfergetcount(FLEXSPI_Type *base, flexspi_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    status_t result = kStatus_Success;

    if (handle->state == kFLEXSPI_Idle) {
        result = kStatus_NoTransferInProgress;
    } else {
        *count = handle->transferTotalSize - handle->dataSize;
    }

    return result;
}

/****************************************************************************
 * Function: imxrt_flexspi_transferabort
 *
 * Description:
 *   brief Aborts an interrupt non-blocking transfer early.
 *
 *   note This API can be called at any time when an interrupt non-blocking transfer initiates
 *   to abort the transfer early.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   handle pointer to flexspi_handle_t structure which stores the transfer state
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_transferabort(FLEXSPI_Type *base, flexspi_handle_t *handle)
{
    DEBUGASSERT(handle);

    imxrt_flexspi_disableinterrupts(base, kIrqFlags);
    handle->state = kFLEXSPI_Idle;
}

/****************************************************************************
 * Function: imxrt_flexspi_transferhandleirq
 *
 * Description:
 *   brief Master interrupt handler.
 *
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   handle pointer to flexspi_handle_t structure.
 *
 * Returned Value:
 *   None
 * 
 ****************************************************************************/
void imxrt_flexspi_transferhandleirq(FLEXSPI_Type *base, flexspi_handle_t *handle)
{
    uint8_t status;
    status_t result;
    uint8_t txWatermark;
    uint8_t rxWatermark;
    uint8_t i = 0;

    status = base->INTR;

    result = imxrt_flexspi_checkandclearerror(base, status);

    if ((result != kStatus_Success) && (handle->completionCallback != NULL)) {
        imxrt_flexspi_transferabort(base, handle);
        if (handle->completionCallback) {
            handle->completionCallback(base, handle, result, handle->userData);
        }
        return;
    }

    if ((status & kFLEXSPI_IpRxFifoWatermarkAvailableFlag) && (handle->state == kFLEXSPI_BusyRead)) {
        rxWatermark = ((base->IPRXFCR & FLEXSPI_IPRXFCR_RXWMRK_MASK) >> FLEXSPI_IPRXFCR_RXWMRK_SHIFT) + 1;

        /* Read watermark level data from rx fifo . */
        if (handle->dataSize >= 8 * rxWatermark) {
            /* Read watermark level data from rx fifo . */
            for (i = 0; i < 2 * rxWatermark; i++) {
                *handle->data++ = base->RFDR[i];
            }

            handle->dataSize = handle->dataSize - 8 * rxWatermark;
        } else {
            for (i = 0; i < (handle->dataSize / 4 + 1); i++) {
                *handle->data++ = base->RFDR[i];
            }
            handle->dataSize = 0;
        }
        /* Pop out a watermark level datas from IP RX FIFO. */
        base->INTR |= kFLEXSPI_IpRxFifoWatermarkAvailableFlag;
    }

    if (status & kFLEXSPI_IpCommandExcutionDoneFlag) {
        base->INTR |= kFLEXSPI_IpCommandExcutionDoneFlag;

        imxrt_flexspi_transferabort(base, handle);

        if (handle->completionCallback) {
            handle->completionCallback(base, handle, kStatus_Success, handle->userData);
        }
    }

    /* TX FIFO empty interrupt, push watermark level data into tx FIFO. */
    if ((status & kFLEXSPI_IpTxFifoWatermarkEmpltyFlag) && (handle->state == kFLEXSPI_BusyWrite)) {
        if (handle->dataSize) {
            txWatermark = ((base->IPTXFCR & FLEXSPI_IPTXFCR_TXWMRK_MASK) >> FLEXSPI_IPTXFCR_TXWMRK_SHIFT) + 1;
            /* Write watermark level data into tx fifo . */
            if (handle->dataSize >= 8 * txWatermark) {
                for (i = 0; i < 2 * txWatermark; i++) {
                    base->TFDR[i] = *handle->data++;
                }

                handle->dataSize = handle->dataSize - 8 * txWatermark;
            } else {
                for (i = 0; i < (handle->dataSize / 4 + 1); i++)
                {
                    base->TFDR[i] = *handle->data++;
                }
                handle->dataSize = 0;
            }

            /* Push a watermark level datas into IP TX FIFO. */
            base->INTR |= kFLEXSPI_IpTxFifoWatermarkEmpltyFlag;
        }
    } else {
    }
}
