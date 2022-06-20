/**************************************************************************//**
 * @file     core_rv_base.h
 * @brief    CMSIS RISC-V common Core Peripheral Access Layer Header File
 * @version  V0.0.1
 * @date     04. October 2017
 ******************************************************************************/
/*
 * Modification copyright (c) 2020 Realtek Semiconductor Corp.
 * Copyright (c) 2017 GreenWaves Technologies SAS. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined ( __ICCARM__ )
#pragma system_include         /* treat file as system include file for MISRA check */
#elif defined (__clang__)
#pragma clang system_header   /* treat file as system include file */
#endif

#ifndef __CORE_RV_BASE_H_GENERIC
#define __CORE_RV_BASE_H_GENERIC

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                 CMSIS definitions
 ******************************************************************************/
/**
  \ingroup RISCV
  @{
 */

/*  CMSIS RISCV definitions */
#define __RISCV_CMSIS_VERSION_MAIN (0U) /*!< [31:16] CMSIS HAL main version */
#define __RISCV_CMSIS_VERSION_SUB (1U)  /*!< [15:0]  CMSIS HAL sub version */
#define __RISCV_CMSIS_VERSION \
    ((__RISCV_CMSIS_VERSION_MAIN << 16U) | __RISCV_CMSIS_VERSION_SUB) /*!< CMSIS HAL version number */

#define __RISCV_V (1U) /*!< RISCV Core Version */

#include "cmsis_compiler.h" /* CMSIS compiler specific defines */


#ifdef __cplusplus
}
#endif

#endif /* __CORE_RV_BASE_H_GENERIC */

#ifndef __CMSIS_GENERIC

#ifndef __CORE_RISCV_H_DEPENDANT
#define __CORE_RISCV_H_DEPENDANT

#ifdef __cplusplus
extern "C" {
#endif

/* check device defines and use defaults */
#if defined __CHECK_DEVICE_DEFINES
#ifndef __RISCV_REV
#define __RISCV_REV               0x0000U
#warning "__RISCV_REV not defined in device header file; using default!"
#endif

#ifndef __ICACHE_PRESENT
#define __ICACHE_PRESENT             0U
#warning "__ICACHE_PRESENT not defined in device header file; using default!"
#endif

#ifndef __DCACHE_PRESENT
#define __DCACHE_PRESENT             0U
#warning "__DCACHE_PRESENT not defined in device header file; using default!"
#endif

#ifndef __L2CACHE_PRESENT
#define __L2CACHE_PRESENT             0U
#warning "__L2CACHE_PRESENT not defined in device header file; using default!"
#endif

#ifndef __IMEM0_PRESENT
#define __IMEM0_PRESENT             0U
#warning "__IMEM0_PRESENT not defined in device header file; using default!"
#endif

#ifndef __DMEM0_PRESENT
#define __DMEM0_PRESENT             0U
#warning "__DMEM0_PRESENT not defined in device header file; using default!"
#endif

#ifndef __IMEM1_PRESENT
#define __IMEM1_PRESENT             0U
#warning "__IMEM1_PRESENT not defined in device header file; using default!"
#endif

#ifndef __DMEM1_PRESENT
#define __DMEM1_PRESENT             0U
#warning "__DMEM1_PRESENT not defined in device header file; using default!"
#endif

#ifndef __L2MEM_PRESENT
#define __L2MEM_PRESENT             0U
#warning "__L2MEM_PRESENT not defined in device header file; using default!"
#endif

#ifndef __FPU_PRESENT
#define __FPU_PRESENT             0U
#warning "__FPU_PRESENT not defined in device header file; using default!"
#endif

#ifndef __MPU_PRESENT
#define __MPU_PRESENT             0U
#warning "__MPU_PRESENT not defined in device header file; using default!"
#endif

#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS          0U
#warning "__NVIC_PRIO_BITS not defined in device header file; using default!"
#endif

#ifndef __Vendor_SysTickConfig
#define __Vendor_SysTickConfig    0U
#warning "__Vendor_SysTickConfig not defined in device header file; using default!"
#endif

#endif

/* IO definitions (access restrictions to peripheral registers) */
/**
    \defgroup CMSIS_glob_defs CMSIS Global Defines

    <strong>IO Type Qualifiers</strong> are used
    \li to specify the access to peripheral variables.
    \li for automatic generation of peripheral register debug information.
*/
#ifdef __cplusplus
#define   __I     volatile             /*!< Defines 'read only' permissions */
#else
#define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

#define REG64(reg)      (*(__IO uint64_t *)(reg))  /*!< Defines 64-bit peripheral accessing macro  */

#define REG32(reg)      (*(__IO uint32_t *)(reg))  /*!< Defines 32-bit peripheral accessing macro  */

#define REG16(reg)      (*(__IO uint16_t *)(reg))  /*!< Defines 16-bit peripheral accessing macro  */

#define REG8(reg)       (*(__IO uint8_t *)(reg))   /*!< Defines 8-bit peripheral accessing macro  */

/*@} end of group RISCV */

/*******************************************************************************
 *                 Register Abstraction
  Core Register contain:
  - Core Register
  - Core NVIC Register
  - Core SCB Register
  - Core SysTick Register
  - Core Debug Register
  - Core MPU Register
  - Core FPU Register
******************************************************************************/
/**
   \defgroup CMSIS_core_register Defines and Type Definitions
   \brief Type definitions and defines for RISCV processor based devices.
*/

/**
  \ingroup    CMSIS_core_register
  \defgroup   CMSIS_CORE  Status and Control Registers
  \brief      Core Register type definitions.
  @{
 */
/* Memory mapping of Core Hardware */
#define MSIP_ADDR       0x010             /*!< Mahcine Software Interrupt Pending Register */
#define MSTATUS_ADDR    0x300             /*!< Mahcine Status Register */
#define MISA_ADDR       0x301             /*!< ISA and Extensions Register */
#define MTVEC_ADDR      0x305             /*!< Machine Trap-handler Base Address Register */
#define MEPC_ADDR       0x341             /*!< Machine Exception Program Counter Register */
#define MCAUSE_ADDR     0x342             /*!< Machine Trap Cause Register */

#define MVENDORID_ADDR  0xF11             /*!< Vendor ID Register */
#define MARCHID_ADDR    0xF12             /*!< Architecture ID Register */
#define MIMPID_ADDR     0xF13             /*!< Implementation ID Register */
#define MHARTID_ADDR    0xF14             /*!< Hardware Thread ID Register */

#define PCER_ADDR       0x7A0             /*!< Performance Counter Mode Register */
#define PCMR_ADDR       0x7A1             /*!< Performance Counter Event Register */

#ifndef ASIC

#define PCCR0_ADDR      0x780             /*!< Performance Counter Counter CYCLE Register */
#define PCCR1_ADDR      0x781             /*!< Performance Counter Counter INSTR Register */
#define PCCR2_ADDR      0x782             /*!< Performance Counter Counter LD_STALL Register */
#define PCCR3_ADDR      0x783             /*!< Performance Counter Counter JR_STALL Register */
#define PCCR4_ADDR      0x784             /*!< Performance Counter Counter IMISS Register */
#define PCCR5_ADDR      0x785             /*!< Performance Counter Counter LD Register */
#define PCCR6_ADDR      0x786             /*!< Performance Counter Counter ST Register */
#define PCCR7_ADDR      0x787             /*!< Performance Counter Counter JUMP Register */
#define PCCR8_ADDR      0x788             /*!< Performance Counter Counter BRANCH Register */
#define PCCR9_ADDR      0x789             /*!< Performance Counter Counter BTAKEN Register */
#define PCCR10_ADDR     0x78A             /*!< Performance Counter Counter RVC Register */
#define PCCR11_ADDR     0x78B             /*!< Performance Counter Counter LD_EXT Register */
#define PCCR12_ADDR     0x78C             /*!< Performance Counter Counter ST_EXT Register */
#define PCCR13_ADDR     0x78D             /*!< Performance Counter Counter LD_EXT_CYC Register */
#define PCCR14_ADDR     0x78E             /*!< Performance Counter Counter ST_EXT_CYC Register */
#define PCCR15_ADDR     0x78F             /*!< Performance Counter Counter TCDM_CONT Register */

#endif

#define PCCR31_ADDR     0x79F             /*!< Performance Counter Counter ALL Register */

#define HWLP_S0_ADDR    0x7B0             /*!< Hardware Loop Start0 Register */
#define HWLP_E0_ADDR    0x7B1             /*!< Hardware Loop End0 Register */
#define HWLP_C0_ADDR    0x7B2             /*!< Hardware Loop Count0 Register */
#define HWLP_S1_ADDR    0x7B3             /*!< Hardware Loop Start1 Register */
#define HWLP_E1_ADDR    0x7B4             /*!< Hardware Loop End1 Register */
#define HWLP_C1_ADDR    0x7B5             /*!< Hardware Loop Count1 Register */

#define DMHARTID_ADDR   0x014             /*!< User Unique Hardware Thread ID Register */
#define UEPC_ADDR       0x041             /*!< User Exception Program Counter Register */

#define CPRIV_ADDR      0xC10             /*!< Current Privilege Level Register */
/* Only for FC */
#define USTATUS_FC_ADDR 0x000             /*!< FC User Status Register */
#define UTVEC_FC_ADDR   0x005             /*!< FC User Trap-handler Base Address Register */
#define UCAUSE_FC_ADDR  0x042             /*!< FC User Trap Cause Register */

/**
  \brief  Union type to access the Vendor ID Register (MVENDORID).
 */
typedef union {
	struct {
		uint32_t OFFSET : 7; /*!< bit:  0..6   */
		uint32_t BANK : 25;  /*!< bit:  7..31  */
	} b;                     /*!< Structure used for bit  access */
	uint32_t w;              /*!< Type      used for word access */
} MVENDORID_Type;

/* MVENDORID Register Definitions */
#define MVENDORID_BANK_Pos 7U             /*!< MVENDORID: BANK Position */
#define MVENDORID_BANK_Msk (0x1FFFFFFUL)  /*!< MVENDORID: BANK Mask */

#define MVENDORID_OFFSET_Pos 0U           /*!< MVENDORID: OFFSET Position */
#define MVENDORID_OFFSET_Msk (0x7FUL)     /*!< MVENDORID: OFFSET Mask */


/**
  \brief  Union type to access the Hart ID Register (MHARTID).
 */
typedef union {
	struct {
		uint32_t OFFSET : 5; /*!< bit:  0..4  Hardware thread id */
		uint32_t BANK : 27;  /*!< bit:  5..31 Reserved */
	} b;                     /*!< Structure used for bit  access */
	uint32_t w;              /*!< Type      used for word access */
} MHARTID_Type;
/* MHARTID Register Definitions */
#define MHARTID_BANK_Pos 0U               /*!< MHARTID: BANK Position */
#define MHARTID_BANK_Msk (0x1FUL)         /*!< MHARTID: BANK Mask */


/**
  \brief  Union type to access the ISA and Extensions Register (MISA).
 */
typedef union {
	struct {
		uint32_t EXTENSION : 26; /*!< bit:  0.. 25  ISA Extension */
		uint32_t WIRI : 4;       /*!< bit:  26..29  Reserved */
		uint32_t MXL : 2;        /*!< bit:  30..31  Machine XLEN */
	} b;                         /*!< Structure used for bit  access */
	uint32_t w;                  /*!< Type      used for word access */
} MISA_Type;

/* MISA Register Definitions */
#define MISA_MXL_Pos 30U                     /*!< MISA: MXL Position */
#define MISA_MXL_Msk (0x3UL << MISA_MXL_Pos) /*!< MISA: MXL Mask */

#define MISA_EXTENSION_Pos 0U                /*!< MISA: EXTENSION Position */
#define MISA_EXTENSION_Msk (0x3FFFFFFUL)     /*!< MISA: EXTENSION Mask */


/**
   \brief  Union type to access the Machine Mode Status Register (MSTATUS).
*/
typedef union {
	struct {
		uint32_t UIE : 1;   /*!< bit:  0       User Interrupt-enable bit */
		uint32_t SIE : 1;   /*!< bit:  1       Supervisor Interrupt-enable bit */
		uint32_t WPRI0 : 1; /*!< bit:  2       Reserved */
		uint32_t MIE : 1;   /*!< bit:  3       Machine Interrupt-enable bit */
		uint32_t UPIE : 1;  /*!< bit:  4       Prior User Interrupt-enable bit on trap */
		uint32_t SPIE : 1;  /*!< bit:  5       Prior Supervisor Interrupt-enable bit on trap */
		uint32_t WPRI1 : 1; /*!< bit:  6       Reserved */
		uint32_t MPIE : 1;  /*!< bit:  7       Prior Machine Interrupt-enable bit on trap */
		uint32_t SPP : 1;   /*!< bit:  8       Supervisor previous privilege value, up to supervisor mode*/
		uint32_t WPRI2 : 2; /*!< bit:  9, 10   Reserved */
		uint32_t MPP : 2;   /*!< bit:  11, 12  Supervise previous privilege value, up to machine mode */
		uint32_t FS : 2;    /*!< bit:  13, 14  Reserved */
		uint32_t XS : 2;    /*!< bit:  15, 16  Reserved */
		uint32_t MPRV : 1;  /*!< bit:  17      Reserved */
		uint32_t SUM : 1;   /*!< bit:  18      Reserved */
		uint32_t MXR : 1;   /*!< bit:  19      Reserved */
		uint32_t TVM : 1;   /*!< bit:  20      Reserved */
		uint32_t TW : 1;    /*!< bit:  21      Reserved */
		uint32_t TSR : 1;   /*!< bit:  22      Reserved */
		uint32_t WPRI3 : 8; /*!< bit:  23..30  Reserved */
		uint32_t SD : 1;    /*!< bit:  31      Reserved */
	} b;                    /*!< Structure used for bit  access */
	uint32_t w;             /*!< Type      used for word access */
} MSTATUS_Type;

/**
   \brief  Structure type to access the machine timer
*/
#if __riscv_xlen > 32
typedef struct {
	__IO uint64_t MTIME;         /* mtime register */
	__IO uint64_t MTIMECMP;      /* mtimecmp register */
} MTIME_Type;
#else
typedef struct {
	__IO uint32_t MTIME_LO;      /* Low part of mtime register */
	__IO uint32_t MTIME_HI;      /* High part of mtime register */
	__IO uint32_t MTIMECMP_LO;   /* Low part of mtimecmp register */
	__IO uint32_t MTIMECMP_HI;   /* High part of mtimecmp register */
} MTIME_Type;
#endif

/* MSTATUS Register Definitions */
#define MSTATUS_SD_Pos 31U                     /*!< MSTATUS: SD Position */
#define MSTATUS_SD_Msk (1UL << MSTATUS_SD_Pos) /*!< MSTATUS: SD Mask */

#define MSTATUS_TSR_Pos 22U                      /*!< MSTATUS: TSR Position */
#define MSTATUS_TSR_Msk (1UL << MSTATUS_TSR_Pos) /*!< MSTATUS: TSR Mask */

#define MSTATUS_TW_Pos 21U                     /*!< MSTATUS: TW Position */
#define MSTATUS_TW_Msk (1UL << MSTATUS_TW_Pos) /*!< MSTATUS: TW Mask */

#define MSTATUS_TVM_Pos 20U                      /*!< MSTATUS: TVM Position */
#define MSTATUS_TVM_Msk (1UL << MSTATUS_TVM_Pos) /*!< MSTATUS: TVM Mask */

#define MSTATUS_MXR_Pos 19U                      /*!< MSTATUS: MXR Position */
#define MSTATUS_MXR_Msk (1UL << MSTATUS_MXR_Pos) /*!< MSTATUS: MXR Mask */

#define MSTATUS_SUM_Pos 18U                      /*!< MSTATUS: SUM Position */
#define MSTATUS_SUM_Msk (1UL << MSTATUS_SUM_Pos) /*!< MSTATUS: SUM Mask */

#define MSTATUS_MPRV_Pos 17U                       /*!< MSTATUS: MPRV Position */
#define MSTATUS_MPRV_Msk (1UL << MSTATUS_MPRV_Pos) /*!< MSTATUS: MPRV Mask */

#define MSTATUS_XS_Pos 15U                     /*!< MSTATUS: XS Position */
#define MSTATUS_XS_Msk (3UL << MSTATUS_XS_Pos) /*!< MSTATUS: XS Mask */

#define MSTATUS_FS_Pos 13U                     /*!< MSTATUS: FS Position */
#define MSTATUS_FS_Msk (3UL << MSTATUS_FS_Pos) /*!< MSTATUS: FS Mask */

#define MSTATUS_MPP_Pos 11U                      /*!< MSTATUS: MPP Position */
#define MSTATUS_MPP_Msk (3UL << MSTATUS_MPP_Pos) /*!< MSTATUS: MPP Mask */

#define MSTATUS_SPP_Pos 8U                       /*!< MSTATUS: SPP Position */
#define MSTATUS_SPP_Msk (1UL << MSTATUS_SPP_Pos) /*!< MSTATUS: SPP Mask */

#define MSTATUS_MPIE_Pos 7U                        /*!< MSTATUS: MPIE Position */
#define MSTATUS_MPIE_Msk (1UL << MSTATUS_MPIE_Pos) /*!< MSTATUS: MPIE Mask */

#define MSTATUS_SPIE_Pos 5U                        /*!< MSTATUS: SPIE Position */
#define MSTATUS_SPIE_Msk (1UL << MSTATUS_SPIE_Pos) /*!< MSTATUS: SPIE Mask */

#define MSTATUS_UPIE_Pos 4U                        /*!< MSTATUS: UPIE Position */
#define MSTATUS_UPIE_Msk (1UL << MSTATUS_UPIE_Pos) /*!< MSTATUS: UPIE Mask */

#define MSTATUS_MIE_Pos 3U                       /*!< MSTATUS: MIE Position */
#define MSTATUS_MIE_Msk (1UL << MSTATUS_MIE_Pos) /*!< MSTATUS: MIE Mask */

#define MSTATUS_SIE_Pos 1U                       /*!< MSTATUS: SIE Position */
#define MSTATUS_SIE_Msk (1UL << MSTATUS_SIE_Pos) /*!< MSTATUS: SIE Mask */

#define MSTATUS_UIE_Pos 0U                       /*!< MSTATUS: UIE Position */
#define MSTATUS_UIE_Msk (1UL)                    /*!< MSTATUS: UIE Mask */

/**
  \brief  Union type to access the Machine Trap-Vector Baser-Address Register (MTVEC).
 */
typedef union {
	struct {
		uint32_t MODE : 2;  /*!< bit:  0..1  vector mode */
		uint32_t BASE : 30; /*!< bit:  2..31 vector base address */
	} b;                    /*!< Structure used for bit  access */
	uint32_t w;             /*!< Type      used for word access */
} MTVEC_Type;

/* MTVEC Register Definitions */
#define MTVEC_BASE_Pos 2U                               /*!< MTVEC: BASE Position */
#define MTVEC_BASE_Msk (0x3FFFFFFFUL << MTVEC_BASE_Pos) /*!< MTVEC: BASE Mask */

#define MTVEC_MODE_Pos 0U                            /*!< MTVEC: MODE Position */
#define MTVEC_MODE_Msk (0x3UL)                       /*!< MTVEC: MODE Mask */


/**
  \brief  Union type to access the Machine Trap Cause Register (MCAUSE).
 */
typedef union {
	struct {
		uint32_t EC : 31; /*!< bit: 0..30  Exception code */
		uint32_t IRQ : 1; /*!< bit:  31 Interrupt flag */
	} b;                  /*!< Structure used for bit  access */
	uint32_t w;           /*!< Type      used for word access */
} MCAUSE_Type;

/* MCAUSE Register Definitions */
#define MCAUSE_IRQ_Pos 31U                       /*!< MCAUSE: IRQ Position */
#define MCAUSE_IRQ_Msk (0x1UL << MCAUSE_IRQ_Pos) /*!< MCAUSE: IRQ Mask */

#define MCAUSE_EC_Pos 0U                         /*!< MCAUSE: EC Position */
#define MCAUSE_EC_Msk (0x1FFFFFFFUL)             /*!< MCAUSE: EC Mask */

/*@} */


/*******************************************************************************
 *                Hardware Abstraction Layer
  Core Function Interface contains:
  - Core NVIC Functions
  - Core SysTick Functions
  - Core Debug Functions
  - Core Register Access Functions
 ******************************************************************************/
/**
  \defgroup CMSIS_Core_FunctionInterface Functions and Instructions Reference
*/

/* ##########################   NVIC functions  #################################### */
/**
  \ingroup  CMSIS_Core_FunctionInterface
  \defgroup CMSIS_Core_NVICFunctions NVIC Functions
  \brief    Functions that manage interrupts and exceptions via the NVIC.
  @{
 */



#define NVIC_USER_IRQ_OFFSET          MAX_SYSTEM_EXECPTION_NUM

#ifdef CMSIS_NVIC_VIRTUAL
#ifndef CMSIS_NVIC_VIRTUAL_HEADER_FILE
#define CMSIS_NVIC_VIRTUAL_HEADER_FILE "cmsis_nvic_virtual.h"
#endif
#include CMSIS_NVIC_VIRTUAL_HEADER_FILE
#else
#define NVIC_EnableIRQ              __NVIC_EnableIRQ
#define NVIC_GetEnableIRQ           __NVIC_GetEnableIRQ
#define NVIC_DisableIRQ             __NVIC_DisableIRQ
#define NVIC_GetPendingIRQ          __NVIC_GetPendingIRQ
#define NVIC_SetPendingIRQ          __NVIC_SetPendingIRQ
#define NVIC_ClearPendingIRQ        __NVIC_ClearPendingIRQ
#define NVIC_GetActive              __NVIC_GetActive_IRQ
#define NVIC_SetPriority            __NVIC_SetPriority
#define NVIC_GetPriority            __NVIC_GetPriority
#define NVIC_SystemReset            __NVIC_SystemReset
#endif /* CMSIS_NVIC_VIRTUAL */

#ifdef CMSIS_VECTAB_VIRTUAL
#ifndef CMSIS_VECTAB_VIRTUAL_HEADER_FILE
#define CMSIS_VECTAB_VIRTUAL_HEADER_FILE "cmsis_vectab_virtual.h"
#endif
#include CMSIS_VECTAB_VIRTUAL_HEADER_FILE
#else
#define NVIC_SetVector              __NVIC_SetVector
#define NVIC_GetVector              __NVIC_GetVector
#define NVIC_SetVectorArg           __NVIC_SetVectorArg
#define NVIC_GetVectorArg           __NVIC_GetVectorArg
#endif  /* (CMSIS_VECTAB_VIRTUAL) */

/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
extern void __NVIC_EnableIRQ(IRQn_Type IRQn);

/**
  \brief   Get Interrupt Enable status
  \details Returns a device specific interrupt enable status from the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \return             0  Interrupt is not enabled.
  \return             1  Interrupt is enabled.
  \note    IRQn must not be negative.
 */
extern uint32_t __NVIC_GetEnableIRQ(IRQn_Type IRQn);

/**
  \brief   Disable Interrupt
  \details Disables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
extern void __NVIC_DisableIRQ(IRQn_Type IRQn);

/**
  \brief   Get Pending Interrupt
  \details Reads the NVIC pending register and returns the pending bit for the specified device specific interrupt.
  \param [in]      IRQn  Device specific interrupt number.
  \return             0  Interrupt status is not pending.
  \return             1  Interrupt status is pending.
  \note    IRQn must not be negative.
 */
extern uint32_t __NVIC_GetPendingIRQ(IRQn_Type IRQn);

/**
  \brief   Set Pending Interrupt
  \details Sets the pending bit of a device specific interrupt in the NVIC pending register.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
extern void __NVIC_SetPendingIRQ(IRQn_Type IRQn);

/**
  \brief   Clear Pending Interrupt
  \details Clears the pending bit of a device specific interrupt in the NVIC pending register.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
extern void __NVIC_ClearPendingIRQ(IRQn_Type IRQn);

/**
  \brief   Get Active Interrupt
  \details Reads the active register in the NVIC and returns the active bit for the device specific interrupt.
  \param [in]      IRQn  Device specific interrupt number.
  \return             0  Interrupt status is not active.
  \return             1  Interrupt status is active.
  \note    IRQn must not be negative.
 */
extern uint32_t __NVIC_GetActive(IRQn_Type IRQn);

/**
  \brief   Set Interrupt Priority
  \details Sets the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]      IRQn  Interrupt number.
  \param [in]  priority  Priority to set.
  \note    The priority cannot be set for every processor exception.
 */
extern void __NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

/**
  \brief   Get Interrupt Priority
  \details Reads the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]   IRQn  Interrupt number.
  \return             Interrupt Priority.
                      Value is aligned automatically to the implemented priority bits of the microcontroller.
 */
extern uint32_t __NVIC_GetPriority(IRQn_Type IRQn);

/**
  \brief   Set Interrupt Vector
  \details Sets an interrupt vector in SRAM based interrupt vector table.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
           VTOR must been relocated to SRAM before.
  \param [in]   IRQn      Interrupt number
  \param [in]   vector    Address of interrupt handler function
 */
extern void __NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);

/**
  \brief   Get Interrupt Vector
  \details Reads an interrupt vector from interrupt vector table.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]   IRQn      Interrupt number.
  \return                 Address of interrupt handler function
 */
extern uint32_t __NVIC_GetVector(IRQn_Type IRQn);

/**
  \brief   Set Interrupt Vector Argument
  \details Sets an interrupt vector argument in SRAM based interrupt vector table.
           The interrupt number can be positive to specify a device specific interrupt,
           VTOR must been relocated to SRAM before.
  \param [in]   IRQn      Interrupt number
  \param [in]   vector    Address of interrupt handler function
 */
extern void __NVIC_SetVectorArg(IRQn_Type IRQn, uint32_t arg);

/**
  \brief   Get Interrupt Vector Argument
  \details Reads an interrupt vector from interrupt vector table.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]   IRQn      Interrupt number.
  \return                 Address of interrupt handler function
 */
extern uint32_t __NVIC_GetVectorArg(IRQn_Type IRQn);


/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
__STATIC_INLINE void __NVIC_SystemReset(void)
{
	// TODO:
}

/*@} end of CMSIS_Core_NVICFunctions */

/* ##########################   CORE ID functions  #################################### */
/**
  \ingroup  CMSIS_Core_IDFunctionInterface
  \defgroup CMSIS_Core_IDFunctions ID Functions
  \brief    Functions that manage Cluster and Core ID.
  @{
 */

__attribute__((always_inline)) __STATIC_INLINE uint32_t __core_ID(void)
{
	int hart_id;

	__ASM volatile("csrr %0, mhartid" : "=r"(hart_id) :);
	return hart_id & 0x1f;
}

__attribute__((always_inline)) __STATIC_INLINE uint32_t __cluster_ID(void)
{
	int hart_id;

	__ASM volatile("csrr %0, mhartid" : "=r"(hart_id) :);
	return (hart_id >> 5) & 0x3f;
}

__attribute__((always_inline)) __STATIC_INLINE uint32_t __hart_ID(void)
{
	int hart_id;

	__ASM volatile("csrr %0, mhartid" : "=r"(hart_id) :);
	return hart_id;
}
/*@} end of CMSIS_Core_IDFunctions */

#ifdef __cplusplus
}
#endif

#endif /* __CORE_RISCV_H_DEPENDANT */

#endif /* __CMSIS_GENERIC */
