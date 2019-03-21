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

#ifndef __IMXRT_FLEXSPI_H_
#define __IMXRT_FLEXSPI_H_

#include <stddef.h>
#include <chip/imxrt105x_features.h>
#include <chip/imxrt105x_config.h>

#include "imxrt_config.h"

/*!
 * @addtogroup flexspi
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL))
#define FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL 0
#endif

/*! @name Driver version */
/*@{*/
/*! @brief FLEXSPI driver version 2.0.5. */
#define FSL_FLEXSPI_DRIVER_VERSION (MAKE_VERSION(2, 0, 5))
/*@}*/

#define FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNTn(0)

/*! @breif Formula to form FLEXSPI instructions in LUT table. */
#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                              \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
     FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))

/*! @brief Status structure of FLEXSPI.*/
enum _flexspi_status {
    kStatus_FLEXSPI_Busy = MAKE_STATUS(70, 0),                     /*!< FLEXSPI is busy */
    kStatus_FLEXSPI_SequenceExecutionTimeout = MAKE_STATUS(70, 1), /*!< Sequence execution timeout
                                                                            error occurred during FLEXSPI transfer. */
    kStatus_FLEXSPI_IpCommandSequenceError = MAKE_STATUS(70, 2),   /*!< IP command Sequence execution
                                                                     timeout error occurred during FLEXSPI transfer. */
    kStatus_FLEXSPI_IpCommandGrantTimeout = MAKE_STATUS(70, 3),    /*!< IP command grant timeout error
                                                                                    occurred during FLEXSPI transfer. */
};

/*! @brief CMD definition of FLEXSPI, use to form LUT instruction. */
enum _flexspi_command {
    kFLEXSPI_Command_STOP = 0x00U,           /*!< Stop execution, deassert CS. */
    kFLEXSPI_Command_SDR = 0x01U,            /*!< Transmit Command code to Flash, using SDR mode. */
    kFLEXSPI_Command_RADDR_SDR = 0x02U,      /*!< Transmit Row Address to Flash, using SDR mode. */
    kFLEXSPI_Command_CADDR_SDR = 0x03U,      /*!< Transmit Column Address to Flash, using SDR mode. */
    kFLEXSPI_Command_MODE1_SDR = 0x04U,      /*!< Transmit 1-bit Mode bits to Flash, using SDR mode. */
    kFLEXSPI_Command_MODE2_SDR = 0x05U,      /*!< Transmit 2-bit Mode bits to Flash, using SDR mode. */
    kFLEXSPI_Command_MODE4_SDR = 0x06U,      /*!< Transmit 4-bit Mode bits to Flash, using SDR mode. */
    kFLEXSPI_Command_MODE8_SDR = 0x07U,      /*!< Transmit 8-bit Mode bits to Flash, using SDR mode. */
    kFLEXSPI_Command_WRITE_SDR = 0x08U,      /*!< Transmit Programming Data to Flash, using SDR mode. */
    kFLEXSPI_Command_READ_SDR = 0x09U,       /*!< Receive Read Data from Flash, using SDR mode. */
    kFLEXSPI_Command_LEARN_SDR = 0x0AU,      /*!< Receive Read Data or Preamble bit from Flash, SDR mode. */
    kFLEXSPI_Command_DATSZ_SDR = 0x0BU,      /*!< Transmit Read/Program Data size (byte) to Flash, SDR mode. */
    kFLEXSPI_Command_DUMMY_SDR = 0x0CU,      /*!< Leave data lines undriven by FlexSPI controller.*/
    kFLEXSPI_Command_DUMMY_RWDS_SDR = 0x0DU, /*!< Leave data lines undriven by FlexSPI controller,
                                                  dummy cycles decided by RWDS. */
    kFLEXSPI_Command_DDR = 0x21U,            /*!< Transmit Command code to Flash, using DDR mode. */
    kFLEXSPI_Command_RADDR_DDR = 0x22U,      /*!< Transmit Row Address to Flash, using DDR mode. */
    kFLEXSPI_Command_CADDR_DDR = 0x23U,      /*!< Transmit Column Address to Flash, using DDR mode. */
    kFLEXSPI_Command_MODE1_DDR = 0x24U,      /*!< Transmit 1-bit Mode bits to Flash, using DDR mode. */
    kFLEXSPI_Command_MODE2_DDR = 0x25U,      /*!< Transmit 2-bit Mode bits to Flash, using DDR mode. */
    kFLEXSPI_Command_MODE4_DDR = 0x26U,      /*!< Transmit 4-bit Mode bits to Flash, using DDR mode. */
    kFLEXSPI_Command_MODE8_DDR = 0x27U,      /*!< Transmit 8-bit Mode bits to Flash, using DDR mode. */
    kFLEXSPI_Command_WRITE_DDR = 0x28U,      /*!< Transmit Programming Data to Flash, using DDR mode. */
    kFLEXSPI_Command_READ_DDR = 0x29U,       /*!< Receive Read Data from Flash, using DDR mode. */
    kFLEXSPI_Command_LEARN_DDR = 0x2AU,      /*!< Receive Read Data or Preamble bit from Flash, DDR mode. */
    kFLEXSPI_Command_DATSZ_DDR = 0x2BU,      /*!< Transmit Read/Program Data size (byte) to Flash, DDR mode. */
    kFLEXSPI_Command_DUMMY_DDR = 0x2CU,      /*!< Leave data lines undriven by FlexSPI controller.*/
    kFLEXSPI_Command_DUMMY_RWDS_DDR = 0x2DU, /*!< Leave data lines undriven by FlexSPI controller,
                                               dummy cycles decided by RWDS. */
    kFLEXSPI_Command_JUMP_ON_CS = 0x1FU,     /*!< Stop execution, deassert CS and save operand[7:0] as the
                                               instruction start pointer for next sequence */
};

/*! @brief pad definition of FLEXSPI, use to form LUT instruction. */
enum _flexspi_pad {
    kFLEXSPI_1PAD = 0x00U, /*!< Transmit command/address and transmit/receive data only through DATA0/DATA1. */
    kFLEXSPI_2PAD = 0x01U, /*!< Transmit command/address and transmit/receive data only through DATA[1:0]. */
    kFLEXSPI_4PAD = 0x02U, /*!< Transmit command/address and transmit/receive data only through DATA[3:0]. */
    kFLEXSPI_8PAD = 0x03U, /*!< Transmit command/address and transmit/receive data only through DATA[7:0]. */
};

/*! @brief FLEXSPI interrupt status flags.*/
typedef enum _flexspi_flags {
    kFLEXSPI_SequenceExecutionTimeoutFlag = FLEXSPI_INTEN_SEQTIMEOUTEN_MASK, /*!< Sequence execution timeout. */
    kFLEXSPI_AhbBusTimeoutFlag = FLEXSPI_INTEN_AHBBUSTIMEOUTEN_MASK,         /*!< AHB Bus timeout. */
    kFLEXSPI_SckStoppedBecauseTxEmptyFlag =
        FLEXSPI_INTEN_SCKSTOPBYWREN_MASK, /*!< SCK is stopped during command
                                               sequence because Async TX FIFO empty. */
    kFLEXSPI_SckStoppedBecauseRxFullFlag =
        FLEXSPI_INTEN_SCKSTOPBYRDEN_MASK, /*!< SCK is stopped during command
                                               sequence because Async RX FIFO full. */
#if !((defined(FSL_FEATURE_FLEXSPI_HAS_NO_DATA_LEARN)) && (FSL_FEATURE_FLEXSPI_HAS_NO_DATA_LEARN))
    kFLEXSPI_DataLearningFailedFlag = FLEXSPI_INTEN_DATALEARNFAILEN_MASK, /*!< Data learning failed. */
#endif
    kFLEXSPI_IpTxFifoWatermarkEmpltyFlag = FLEXSPI_INTEN_IPTXWEEN_MASK,    /*!< IP TX FIFO WaterMark empty. */
    kFLEXSPI_IpRxFifoWatermarkAvailableFlag = FLEXSPI_INTEN_IPRXWAEN_MASK, /*!< IP RX FIFO WaterMark available. */
    kFLEXSPI_AhbCommandSequenceErrorFlag =
        FLEXSPI_INTEN_AHBCMDERREN_MASK,                                  /*!< AHB triggered Command Sequences Error. */
    kFLEXSPI_IpCommandSequenceErrorFlag = FLEXSPI_INTEN_IPCMDERREN_MASK, /*!< IP triggered Command Sequences Error. */
    kFLEXSPI_AhbCommandGrantTimeoutFlag =
        FLEXSPI_INTEN_AHBCMDGEEN_MASK, /*!< AHB triggered Command Sequences Grant Timeout. */
    kFLEXSPI_IpCommandGrantTimeoutFlag =
        FLEXSPI_INTEN_IPCMDGEEN_MASK, /*!< IP triggered Command Sequences Grant Timeout. */
    kFLEXSPI_IpCommandExcutionDoneFlag =
        FLEXSPI_INTEN_IPCMDDONEEN_MASK,  /*!< IP triggered Command Sequences Execution finished. */
    kFLEXSPI_AllInterruptFlags = 0xFFFU, /*!< All flags. */
} flexspi_flags_t;

/*! @brief FLEXSPI sample clock source selection for Flash Reading.*/
typedef enum _flexspi_read_sample_clock {
    kFLEXSPI_ReadSampleClkLoopbackInternally = 0x0U,      /*!< Dummy Read strobe generated by FlexSPI Controller
                                                               and loopback internally. */
    kFLEXSPI_ReadSampleClkLoopbackFromDqsPad = 0x1U,      /*!< Dummy Read strobe generated by FlexSPI Controller
                                                               and loopback from DQS pad. */
    kFLEXSPI_ReadSampleClkLoopbackFromSckPad = 0x2U,      /*!< SCK output clock and loopback from SCK pad. */
    kFLEXSPI_ReadSampleClkExternalInputFromDqsPad = 0x3U, /*!< Flash provided Read strobe and input from DQS pad. */
} flexspi_read_sample_clock_t;

/*! @brief FLEXSPI interval unit for flash device select.*/
typedef enum _flexspi_cs_interval_cycle_unit {
    kFLEXSPI_CsIntervalUnit1SckCycle = 0x0U,   /*!< Chip selection interval: CSINTERVAL * 1 serial clock cycle. */
    kFLEXSPI_CsIntervalUnit256SckCycle = 0x1U, /*!< Chip selection interval: CSINTERVAL * 256 serial clock cycle. */
} flexspi_cs_interval_cycle_unit_t;

/*! @brief FLEXSPI AHB wait interval unit for writting.*/
typedef enum _flexspi_ahb_write_wait_unit {
    kFLEXSPI_AhbWriteWaitUnit2AhbCycle = 0x0U,     /*!< AWRWAIT unit is 2 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit8AhbCycle = 0x1U,     /*!< AWRWAIT unit is 8 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit32AhbCycle = 0x2U,    /*!< AWRWAIT unit is 32 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit128AhbCycle = 0x3U,   /*!< AWRWAIT unit is 128 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit512AhbCycle = 0x4U,   /*!< AWRWAIT unit is 512 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit2048AhbCycle = 0x5U,  /*!< AWRWAIT unit is 2048 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit8192AhbCycle = 0x6U,  /*!< AWRWAIT unit is 8192 ahb clock cycle. */
    kFLEXSPI_AhbWriteWaitUnit32768AhbCycle = 0x7U, /*!< AWRWAIT unit is 32768 ahb clock cycle. */
} flexspi_ahb_write_wait_unit_t;

/*! @brief Error Code when IP command Error detected.*/
typedef enum _flexspi_ip_error_code {
    kFLEXSPI_IpCmdErrorNoError = 0x0U,                  /*!< No error. */
    kFLEXSPI_IpCmdErrorJumpOnCsInIpCmd = 0x2U,          /*!< IP command with JMP_ON_CS instruction used. */
    kFLEXSPI_IpCmdErrorUnknownOpCode = 0x3U,            /*!< Unknown instruction opcode in the sequence. */
    kFLEXSPI_IpCmdErrorSdrDummyInDdrSequence = 0x4U,    /*!< Instruction DUMMY_SDR/DUMMY_RWDS_SDR
                                                             used in DDR sequence. */
    kFLEXSPI_IpCmdErrorDdrDummyInSdrSequence = 0x5U,    /*!< Instruction DUMMY_DDR/DUMMY_RWDS_DDR
                                                             used in SDR sequence. */
    kFLEXSPI_IpCmdErrorInvalidAddress = 0x6U,           /*!< Flash access start address exceed the whole
                                                            flash address range (A1/A2/B1/B2). */
    kFLEXSPI_IpCmdErrorSequenceExecutionTimeout = 0xEU, /*!< Sequence execution timeout. */
    kFLEXSPI_IpCmdErrorFlashBoundaryAcrosss = 0xFU,     /*!< Flash boundary crossed. */
} flexspi_ip_error_code_t;

/*! @brief Error Code when AHB command Error detected.*/
typedef enum _flexspi_ahb_error_code {
    kFLEXSPI_AhbCmdErrorNoError = 0x0U,               /*!< No error. */
    kFLEXSPI_AhbCmdErrorJumpOnCsInWriteCmd = 0x2U,    /*!< AHB Write command with JMP_ON_CS instruction
                                                           used in the sequence. */
    kFLEXSPI_AhbCmdErrorUnknownOpCode = 0x3U,         /*!< Unknown instruction opcode in the sequence. */
    kFLEXSPI_AhbCmdErrorSdrDummyInDdrSequence = 0x4U, /*!< Instruction DUMMY_SDR/DUMMY_RWDS_SDR used
                                                           in DDR sequence. */
    kFLEXSPI_AhbCmdErrorDdrDummyInSdrSequence = 0x5U, /*!< Instruction DUMMY_DDR/DUMMY_RWDS_DDR
                                                           used in SDR sequence. */
    kFLEXSPI_AhbCmdSequenceExecutionTimeout = 0x6U,   /*!< Sequence execution timeout. */
} flexspi_ahb_error_code_t;

/*! @brief FLEXSPI operation port select.*/
typedef enum _flexspi_port {
    kFLEXSPI_PortA1 = 0x0U, /*!< Access flash on A1 port. */
    kFLEXSPI_PortA2,        /*!< Access flash on A2 port. */
    kFLEXSPI_PortB1,        /*!< Access flash on B1 port. */
    kFLEXSPI_PortB2,        /*!< Access flash on B2 port. */
    kFLEXSPI_PortCount
} flexspi_port_t;

/*! @brief Trigger source of current command sequence granted by arbitrator.*/
typedef enum _flexspi_arb_command_source {
    kFLEXSPI_AhbReadCommand = 0x0U,
    kFLEXSPI_AhbWriteCommand = 0x1U,
    kFLEXSPI_IpCommand = 0x2U,
    kFLEXSPI_SuspendedCommand = 0x3U,
} flexspi_arb_command_source_t;

typedef enum _flexspi_command_type {
    kFLEXSPI_Command, /*!< FlexSPI operation: Only command, both TX and Rx buffer are ignored. */
    kFLEXSPI_Config,  /*!< FlexSPI operation: Configure device mode, the TX fifo size is fixed in LUT. */
    kFLEXSPI_Read,    /* /!< FlexSPI operation: Read, only Rx Buffer is effective. */
    kFLEXSPI_Write,   /* /!< FlexSPI operation: Read, only Tx Buffer is effective. */
} flexspi_command_type_t;

typedef struct _flexspi_ahbBuffer_config {
    uint8_t priority;    /*!< This priority for AHB Master Read which this AHB RX Buffer is assigned. */
    uint8_t masterIndex; /*!< AHB Master ID the AHB RX Buffer is assigned. */
    uint16_t bufferSize; /*!< AHB buffer size in byte. */
    bool enablePrefetch; /*!< AHB Read Prefetch Enable for current AHB RX Buffer corresponding Master, allows
                          prefetch disable/enable seperately for each master. */
} flexspi_ahbBuffer_config_t;

/*! @brief FLEXSPI configuration structure. */
typedef struct _flexspi_config {
    flexspi_read_sample_clock_t rxSampleClock; /*!< Sample Clock source selection for Flash Reading. */
    bool enableSckFreeRunning;                 /*!< Enable/disable SCK output free-running. */
    bool enableCombination;                    /*!< Enable/disable combining PORT A and B Data Pins
                                               (SIOA[3:0] and SIOB[3:0]) to support Flash Octal mode. */
    bool enableDoze;                           /*!< Enable/disable doze mode support. */
    bool enableHalfSpeedAccess;                /*!< Enable/disable divide by 2 of the clock for half
                                                speed commands. */
    bool enableSckBDiffOpt;                    /*!< Enable/disable SCKB pad use as SCKA differential clock
                                                output, when enable, Port B flash access is not available. */
    bool enableSameConfigForAll;               /*!< Enable/disable same configuration for all connected devices
                                                when enabled, same configuration in FLASHA1CRx is applied to all. */
    uint16_t seqTimeoutCycle;                  /*!< Timeout wait cycle for command sequence execution,
                                               timeout after ahbGrantTimeoutCyle*1024 serial root clock cycles. */
    uint8_t ipGrantTimeoutCycle;               /*!< Timeout wait cycle for IP command grant, timeout after
                                                ipGrantTimeoutCycle*1024 AHB clock cycles. */
    uint8_t txWatermark;                       /*!< FLEXSPI IP transmit watermark value. */
    uint8_t rxWatermark;                       /*!< FLEXSPI receive watermark value. */
    struct {
        bool enableAHBWriteIpTxFifo;  /*!< Enable AHB bus write access to IP TX FIFO. */
        bool enableAHBWriteIpRxFifo;  /*!< Enable AHB bus write access to IP RX FIFO. */
        uint8_t ahbGrantTimeoutCycle; /*!< Timeout wait cycle for AHB command grant,
                                       timeout after ahbGrantTimeoutCyle*1024 AHB clock cycles. */
        uint16_t ahbBusTimeoutCycle;  /*!< Timeout wait cycle for AHB read/write access,
                                      timeout after ahbBusTimeoutCycle*1024 AHB clock cycles. */
        uint8_t resumeWaitCycle;      /*!< Wait cycle for idle state before suspended command sequence
                                       resume, timeout after ahbBusTimeoutCycle AHB clock cycles. */
        flexspi_ahbBuffer_config_t buffer[FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT]; /*!< AHB buffer size. */
        bool enableClearAHBBufferOpt; /*!< Enable/disable automatically clean AHB RX Buffer and TX Buffer
                                       when FLEXSPI returns STOP mode ACK. */
        bool enableReadAddressOpt;    /*!< Enable/disable remove AHB read burst start address alignment limitation.
                                       when eanble, there is no AHB read burst start address alignment limitation. */
        bool enableAHBPrefetch;       /*!< Enable/disable AHB read prefetch feature, when enabled, FLEXSPI
                                       will fetch more data than current AHB burst. */
        bool enableAHBBufferable;     /*!< Enable/disable AHB bufferable write access support, when enabled,
                                       FLEXSPI return before waiting for command excution finished. */
        bool enableAHBCachable;       /*!< Enable AHB bus cachable read access support. */
    } ahbConfig;
} flexspi_config_t;

/*! @brief External device configuration items. */
typedef struct _flexspi_device_config {
    uint32_t flexspiRootClk;                         /*!< FLEXSPI serial root clock. */
    bool isSck2Enabled;                              /*!< FLEXSPI use SCK2. */
    uint32_t flashSize;                              /*!< Flash size in KByte. */
    flexspi_cs_interval_cycle_unit_t CSIntervalUnit; /*!< CS interval unit, 1 or 256 cycle. */
    uint16_t CSInterval;                             /*!< CS line assert interval, mutiply CS interval unit to
                                                      get the CS line assert interval cycles. */
    uint8_t CSHoldTime;                              /*!< CS line hold time. */
    uint8_t CSSetupTime;                             /*!< CS line setup time. */
    uint8_t dataValidTime;                           /*!< Data valid time for external device. */
    uint8_t columnspace;                             /*!< Column space size. */
    bool enableWordAddress;                          /*!< If enable word address.*/
    uint8_t AWRSeqIndex;                             /*!< Sequence ID for AHB write command. */
    uint8_t AWRSeqNumber;                            /*!< Sequence number for AHB write command. */
    uint8_t ARDSeqIndex;                             /*!< Sequence ID for AHB read command. */
    uint8_t ARDSeqNumber;                            /*!< Sequence number for AHB read command. */
    flexspi_ahb_write_wait_unit_t AHBWriteWaitUnit;  /*!< AHB write wait unit. */
    uint16_t AHBWriteWaitInterval;                   /*!< AHB write wait interval, mutiply AHB write interval
                                                      unit to get the AHB write wait cycles. */
    bool enableWriteMask;                            /*!< Enable/Disable FLEXSPI drive DQS pin as write mask
                                                      when writing to external device. */
} flexspi_device_config_t;

/*! @brief Transfer structure for FLEXSPI. */
typedef struct _flexspi_transfer {
    uint32_t deviceAddress;         /*!< Operation device address. */
    flexspi_port_t port;            /*!< Operation port. */
    flexspi_command_type_t cmdType; /*!< Execution command type. */
    uint8_t seqIndex;               /*!< Sequence ID for command. */
    uint8_t SeqNumber;              /*!< Sequence number for command. */
    uint32_t *data;                 /*!< Data buffer. */
    size_t dataSize;                /*!< Data size in bytes. */
} flexspi_transfer_t;

/* Forward declaration of the handle typedef. */
typedef struct _flexspi_handle flexspi_handle_t;

/*! @brief FLEXSPI transfer callback function. */
typedef void (*flexspi_transfer_callback_t)(FLEXSPI_Type *base,
                                            flexspi_handle_t *handle,
                                            status_t status,
                                            void *userData);

/*! @brief Transfer handle structure for FLEXSPI. */
struct _flexspi_handle {
    uint32_t state;                                 /*!< Internal state for FLEXSPI transfer */
    uint32_t *data;                                 /*!< Data buffer. */
    size_t dataSize;                                /*!< Remaining Data size in bytes. */
    size_t transferTotalSize;                       /*!< Total Data size in bytes. */
    flexspi_transfer_callback_t completionCallback; /*!< Callback for users while transfer finish or error occurred */
    void *userData;                                 /*!< FLEXSPI callback function parameter.*/
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus. */

/*!
 * @name Initialization and deinitialization
 * @{
 */
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
 *   base FLEXSPI peripheral base address.
 *   config FLEXSPI configure structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexspi_init(FLEXSPI_Type *base, const flexspi_config_t *config);

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
void imxrt_flexspi_getdefaultconfig(flexspi_config_t *config);

/****************************************************************************
 * Function: imxrt_flexspi_deinit
 *
 * Description:
 *   brief Gets default settings for FLEXSPI.
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
void imxrt_flexspi_deinit(FLEXSPI_Type *base);

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
void imxrt_flexspi_setflashconfig(FLEXSPI_Type *base, flexspi_device_config_t *config, flexspi_port_t port);

/****************************************************************************
 * Function: imxrt_flexspi_softwarereset
 *
 * Description:
 *   brief Software reset for the FLEXSPI logic.
 * 
 *   This function sets the software reset flags for both AHB and buffer domain and
 *   resets both AHB buffer and also IP FIFOs.
 *  
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_softwarereset(FLEXSPI_Type *base)
{
    base->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (base->MCR0 & FLEXSPI_MCR0_SWRESET_MASK) {
    }
}

/****************************************************************************
 * Function: imxrt_flexspi_softwarereset
 *
 * Description:
 *   brief Enables or disables the FLEXSPI module.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   enable True means enable FLEXSPI, false means disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_enable(FLEXSPI_Type *base, bool enable)
{
    if (enable) {
        base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    } else {
        base->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
    }
}

/* @} */

/*!
 * @name Interrupts
 * @{
 */
/****************************************************************************
 * Function: imxrt_flexspi_enableinterrupts
 *
 * Description:
 *   brief Enables the FLEXSPI interrupts.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   mask FLEXSPI interrupt source.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_enableinterrupts(FLEXSPI_Type *base, uint32_t mask)
{
    base->INTEN |= mask;
}

/****************************************************************************
 * Function: imxrt_flexspi_disableinterrupts
 *
 * Description:
 *   brief Disable the FLEXSPI interrupts.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   mask FLEXSPI interrupt source.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_disableinterrupts(FLEXSPI_Type *base, uint32_t mask)
{
    base->INTEN &= ~mask;
}

/* @} */

/*! @name DMA control */
/*@{*/

/****************************************************************************
 * Function: imxrt_flexspi_enabletxdma
 *
 * Description:
 *   brief Enables or disables FLEXSPI IP Tx FIFO DMA requests.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   enable Enable flag for transmit DMA request. Pass true for enable, false for disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_enabletxdma(FLEXSPI_Type *base, bool enable)
{
    if (enable) {
        base->IPTXFCR |= FLEXSPI_IPTXFCR_TXDMAEN_MASK;
    } else {
        base->IPTXFCR &= ~FLEXSPI_IPTXFCR_TXDMAEN_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_flexspi_enablerxdma
 *
 * Description:
 *   brief Enables or disables FLEXSPI IP Rx FIFO DMA requests.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   enable Enable flag for receive DMA request. Pass true for enable, false for disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_enablerxdma(FLEXSPI_Type *base, bool enable)
{
    if (enable) {
        base->IPRXFCR |= FLEXSPI_IPRXFCR_RXDMAEN_MASK;
    } else {
        base->IPRXFCR &= ~FLEXSPI_IPRXFCR_RXDMAEN_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_flexspi_gettxfifoaddress
 *
 * Description:
 *   brief Gets FLEXSPI IP tx fifo address for DMA transfer.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   Returns The tx fifo address.
 *
 ****************************************************************************/
static inline uint32_t imxrt_flexspi_gettxfifoaddress(FLEXSPI_Type *base)
{
    return (uint32_t)&base->TFDR[0];
}

/****************************************************************************
 * Function: imxrt_flexspi_getrxfifoaddress
 *
 * Description:
 *   brief Gets FLEXSPI IP rx fifo address for DMA transfer.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   Returns The rx fifo address.
 *
 ****************************************************************************/
static inline uint32_t imxrt_flexspi_getrxfifoaddress(FLEXSPI_Type *base)
{
    return (uint32_t)&base->RFDR[0];
}

/*@}*/

/*! @name FIFO control */
/*@{*/

/****************************************************************************
 * Function: imxrt_flexspi_getrxfifoaddress
 *
 * Description:
 *   brief Clears the FLEXSPI IP FIFO logic.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   txFifo Pass true to reset TX FIFO.
 *   rxFifo Pass true to reset RX FIFO.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_resetfifos(FLEXSPI_Type *base, bool txFifo, bool rxFifo)
{
    if (txFifo) {
        base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
    }
    if (rxFifo) {
        base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_flexspi_getfifocounts
 *
 * Description:
 *   brief Gets the valid data entries in the FLEXSPI FIFOs.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   txCount[out] Pointer through which the current number of bytes in the transmit FIFO is returned.
 *                Pass NULL if this value is not required.
 *   rxCount[out] Pointer through which the current number of bytes in the receive FIFO is returned.
 *                Pass NULL if this value is not required.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_getfifocounts(FLEXSPI_Type *base, size_t *txCount, size_t *rxCount)
{
    if (txCount) {
        *txCount = (((base->IPTXFSTS) & FLEXSPI_IPTXFSTS_FILL_MASK) >> FLEXSPI_IPTXFSTS_FILL_SHIFT) * 8U;
    }
    if (rxCount) {
        *rxCount = (((base->IPRXFSTS) & FLEXSPI_IPRXFSTS_FILL_MASK) >> FLEXSPI_IPRXFSTS_FILL_SHIFT) * 8U;
    }
}

/*@}*/

/*!
 * @name Status
 * @{
 */
/****************************************************************************
 * Function: imxrt_flexspi_getinterruptsstatusflags
 *
 * Description:
 *   brief Get the FLEXSPI interrupt status flags.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   Returns interrupt status flag, use status flag to AND #flexspi_flags_t could get the related status.
 *
 ****************************************************************************/
static inline uint32_t imxrt_flexspi_getinterruptsstatusflags(FLEXSPI_Type *base)
{
    return base->INTR;
}

/****************************************************************************
 * Function: imxrt_flexspi_clearinterruptstatusflags
 *
 * Description:
 *   brief Get the FLEXSPI interrupt status flags.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   interrupt status flag.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_clearinterruptstatusflags(FLEXSPI_Type *base, uint32_t mask)
{
    base->INTR |= mask;
}

#if !((defined(FSL_FEATURE_FLEXSPI_HAS_NO_DATA_LEARN)) && (FSL_FEATURE_FLEXSPI_HAS_NO_DATA_LEARN))
/****************************************************************************
 * Function: imxrt_flexspi_getdatalearningphase
 *
 * Description:
 *   brief Gets the sampling clock phase selection after Data Learning.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   portAPhase Pointer to a uint8_t type variable to receive the selected clock phase on PORTA.
 *   portBPhase Pointer to a uint8_t type variable to receive the selected clock phase on PORTB.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_getdatalearningphase(FLEXSPI_Type *base, uint8_t *portAPhase, uint8_t *portBPhase)
{
    if (portAPhase) {
        *portAPhase = (base->STS0 & FLEXSPI_STS0_DATALEARNPHASEA_MASK) >> FLEXSPI_STS0_DATALEARNPHASEA_SHIFT;
    }

    if (portBPhase) {
        *portBPhase = (base->STS0 & FLEXSPI_STS0_DATALEARNPHASEB_MASK) >> FLEXSPI_STS0_DATALEARNPHASEB_SHIFT;
    }
}
#endif

/****************************************************************************
 * Function: imxrt_flexspi_getarbitratorcommandsource
 *
 * Description:
 *   brief Gets the trigger source of current command sequence granted by arbitrator.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   Returns trigger source of current command sequence.
 *
 ****************************************************************************/
static inline flexspi_arb_command_source_t imxrt_flexspi_getarbitratorcommandsource(FLEXSPI_Type *base)
{
    return (flexspi_arb_command_source_t)((base->STS0 & FLEXSPI_STS0_ARBCMDSRC_MASK) >> FLEXSPI_STS0_ARBCMDSRC_SHIFT);
}

/****************************************************************************
 * Function: imxrt_flexspi_getipcommanderrorcode
 *
 * Description:
 *   brief Gets the error code when IP command error detected.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   index Pointer to a uint8_t type variable to receive the sequence index when error detected.
 *
 * Returned Value:
 *   Returns error code when IP command error detected.
 *
 ****************************************************************************/
static inline flexspi_ip_error_code_t imxrt_flexspi_getipcommanderrorcode(FLEXSPI_Type *base, uint8_t *index)
{
    *index = (base->STS1 & FLEXSPI_STS1_IPCMDERRID_MASK) >> FLEXSPI_STS1_IPCMDERRID_SHIFT;
    return (flexspi_ip_error_code_t)((base->STS1 & FLEXSPI_STS1_IPCMDERRCODE_MASK) >> FLEXSPI_STS1_IPCMDERRCODE_SHIFT);
}

/****************************************************************************
 * Function: imxrt_flexspi_getahbcommanderrorcode
 *
 * Description:
 *   brief Gets the error code when AHB command error detected.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   index Pointer to a uint8_t type variable to receive the sequence index when error detected.
 *
 * Returned Value:
 *   Returns error code when AHB command error detected.
 *
 ****************************************************************************/
static inline flexspi_ahb_error_code_t imxrt_flexspi_getahbcommanderrorcode(FLEXSPI_Type *base, uint8_t *index)
{
    *index = (base->STS1 & FLEXSPI_STS1_AHBCMDERRID_MASK) >> FLEXSPI_STS1_AHBCMDERRID_SHIFT;
    return (flexspi_ahb_error_code_t)((base->STS1 & FLEXSPI_STS1_AHBCMDERRCODE_MASK) >>
                                      FLEXSPI_STS1_AHBCMDERRCODE_SHIFT);
}

/****************************************************************************
 * Function: imxrt_flexspi_getbusidlestatus
 *
 * Description:
 *   brief Returns whether the bus is idle.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *
 * Returned Value:
 *   Returns true Bus is idle.
 *   Returns false Bus is busy.
 *
 ****************************************************************************/
static inline bool imxrt_flexspi_getbusidlestatus(FLEXSPI_Type *base)
{
    return (base->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (base->STS0 & FLEXSPI_STS0_SEQIDLE_MASK);
}
/*@}*/

/*!
 * @name Bus Operations
 * @{
 */

/****************************************************************************
 * Function: imxrt_flexspi_enableipparallelmode
 *
 * Description:
 *   brief Enables/disables the FLEXSPI IP command parallel mode.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   enable True means enable parallel mode, false means disable parallel mode.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_enableipparallelmode(FLEXSPI_Type *base, bool enable)
{
    if (enable) {
        base->IPCR1 |= FLEXSPI_IPCR1_IPAREN_MASK;
    } else {
        base->IPCR1 &= ~FLEXSPI_IPCR1_IPAREN_MASK;
    }
}

/****************************************************************************
 * Function: imxrt_flexspi_enableahbparallelmode
 *
 * Description:
 *   brief Enables/disables the FLEXSPI AHB command parallel mode.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   enable True means enable parallel mode, false means disable parallel mode.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_enableahbparallelmode(FLEXSPI_Type *base, bool enable)
{
    if (enable) {
        base->AHBCR |= FLEXSPI_AHBCR_APAREN_MASK;
    } else {
        base->AHBCR &= ~FLEXSPI_AHBCR_APAREN_MASK;
    }
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
 *   also allows user to update command content inside a command. Each command consists of up to
 *   8 instructions and occupy 4*32-bit memory.
 *   cmd Command sequence array.
 *   count Number of sequences.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexspi_updatelut(FLEXSPI_Type *base, uint32_t index, const uint32_t *cmd, uint32_t count);

/****************************************************************************
 * Function: imxrt_flexspi_writedata
 *
 * Description:
 *   brief Writes data into FIFO.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   data The data bytes to send
 *   fifoIndex Destination fifo index.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexspi_writedata(FLEXSPI_Type *base, uint32_t data, uint8_t fifoIndex)
{
    base->TFDR[fifoIndex] = data;
}

/****************************************************************************
 * Function: imxrt_flexspi_readdata
 *
 * Description:
 *   brief Receives data from data FIFO.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   fifoIndex Source fifo index.
 *
 * Returned Value:
 *   Returns The data in the FIFO.
 *
 ****************************************************************************/
static inline uint32_t imxrt_flexspi_readdata(FLEXSPI_Type *base, uint8_t fifoIndex)
{
    return base->RFDR[fifoIndex];
}

/****************************************************************************
 * Function: imxrt_flexspi_writeblocking
 *
 * Description:
 *   brief Sends a buffer of data bytes using blocking method.
 *   
 *   @note This function blocks via polling until all bytes have been sent.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
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
status_t imxrt_flexspi_writeblocking(FLEXSPI_Type *base, uint32_t *buffer, size_t size);

/****************************************************************************
 * Function: imxrt_flexspi_readblocking
 *
 * Description:
 *   brief Receives a buffer of data bytes using a blocking method.
 *   
 *   @note This function blocks via polling until all bytes have been received.
 * 
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   buffer The data bytes to receive
 *   size The number of data bytes to receive
 *
 * Returned Value:
 *   Returns kStatus_Success read success without error
 *   Returns kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 *   Returns kStatus_FLEXSPI_IpCommandSequenceError IP command sequencen error detected
 *   Returns kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 *
 ****************************************************************************/
status_t imxrt_flexspi_readblocking(FLEXSPI_Type *base, uint32_t *buffer, size_t size);

/****************************************************************************
 * Function: imxrt_flexspi_readblocking
 *
 * Description:
 *   brief Execute command to transfer a buffer data bytes using a blocking method.
 *   
 * Input Parameters:
 *   base FLEXSPI peripheral base address.
 *   xfer pointer to the transfer structure.
 *
 * Returned Value:
 *   Returns kStatus_Success command transfer success without error
 *   Returns kStatus_FLEXSPI_SequenceExecutionTimeout sequence execution timeout
 *   Returns kStatus_FLEXSPI_IpCommandSequenceError IP command sequencen error detected
 *   Returns kStatus_FLEXSPI_IpCommandGrantTimeout IP command grant timeout detected
 *
 ****************************************************************************/
status_t imxrt_flexspi_transferblocking(FLEXSPI_Type *base, flexspi_transfer_t *xfer);
/*! @} */

/*!
 * @name Transactional
 * @{
 */

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
void imxrt_flexspi_transfercreatehandle(FLEXSPI_Type *base, flexspi_handle_t *handle, flexspi_transfer_callback_t callback, void *userData);

/****************************************************************************
 * Function: imxrt_flexspi_transfernonblocking
 *
 * Description:
 *   brief Performs a interrupt non-blocking transfer on the FLEXSPI bus.
 * 
 *   @note Calling the API returns immediately after transfer initiates. The user needs
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
status_t imxrt_flexspi_transfernonblocking(FLEXSPI_Type *base, flexspi_handle_t *handle, flexspi_transfer_t *xfer);

/****************************************************************************
 * Function: imxrt_flexspi_transfernonblocking
 *
 * Description:
 *   brief Gets the master transfer status during a interrupt non-blocking transfer.
 * 
 *   @note Calling the API returns immediately after transfer initiates. The user needs
 *   to call FLEXSPI_GetTransferCount to poll the transfer status to check whether
 *   the transfer is finished. If the return status is not kStatus_FLEXSPI_Busy, the transfer
 *   is finished. For FLEXSPI_Read, the dataSize should be multiple of rx watermark levle, or
 *   FLEXSPI could not read data properly.
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
status_t imxrt_flexspi_tranfergetcount(FLEXSPI_Type *base, flexspi_handle_t *handle, size_t *count);

/****************************************************************************
 * Function: imxrt_flexspi_transferabort
 *
 * Description:
 *   brief Aborts an interrupt non-blocking transfer early.
 * 
 *   @note This API can be called at any time when an interrupt non-blocking transfer initiates
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
void imxrt_flexspi_transferabort(FLEXSPI_Type *base, flexspi_handle_t *handle);

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
void imxrt_flexspi_transferhandleirq(FLEXSPI_Type *base, flexspi_handle_t *handle);
/*! @} */

#if defined(__cplusplus)
}
#endif /*_cplusplus. */
/*@}*/

#endif /* __IMXRT_FLEXSPI_H_ */
