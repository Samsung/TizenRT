/**************************************************************************//**
 * @file     cmsis_gcc.h
 * @brief    CMSIS compiler GCC header file
 * @version  V5.0.4
 * @date     09. April 2018
 ******************************************************************************/
/*
 * Modification copyright (c) 2020 Realtek Semiconductor Corp.
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
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

#ifndef __CMSIS_GCC_H
#define __CMSIS_GCC_H

#define __RISCV_ARCH__ 1
/* ignore some GCC warnings */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* Fallback for __has_builtin */
#ifndef __has_builtin
#define __has_builtin(x) (0)
#endif

/* CMSIS compiler specific defines */
#ifndef   __ASM
#define __ASM                                  __asm
#endif
#ifndef   __INLINE
#define __INLINE                               inline
#endif
#ifndef   __STATIC_INLINE
#define __STATIC_INLINE                        static inline
#endif
#ifndef   __STATIC_FORCEINLINE
#define __STATIC_FORCEINLINE                   __STATIC_INLINE
#endif
#ifndef   __NO_RETURN
#define __NO_RETURN                            __attribute__((__noreturn__))
#endif
#ifndef   __USED
#define __USED                                 __attribute__((used))
#endif
#ifndef   __UNUSED
#define __UNUSED                               __attribute__((unused))
#endif
#ifndef   __WEAK
#define __WEAK                                 __attribute__((weak))
#endif
#ifndef   __PACKED
#define __PACKED                               __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_STRUCT
#define __PACKED_STRUCT                        struct __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_UNION
#define __PACKED_UNION                         union __attribute__((packed, aligned(1)))
#endif
#ifndef   __UNALIGNED_UINT32        /* deprecated */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
struct __attribute__((packed)) T_UINT32 {
	uint32_t v;
};
#pragma GCC diagnostic pop
#define __UNALIGNED_UINT32(x)                  (((struct T_UINT32 *)(x))->v)
#endif
#ifndef   __UNALIGNED_UINT16_WRITE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
__PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };
#pragma GCC diagnostic pop
#define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT16_READ
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
__PACKED_STRUCT T_UINT16_READ { uint16_t v; };
#pragma GCC diagnostic pop
#define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)
#endif
#ifndef   __UNALIGNED_UINT32_WRITE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
__PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };
#pragma GCC diagnostic pop
#define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT32_READ
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
__PACKED_STRUCT T_UINT32_READ { uint32_t v; };
#pragma GCC diagnostic pop
#define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)
#endif
#ifndef   __ALIGNED
#define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif
#ifndef   __RESTRICT
#define __RESTRICT                             __restrict
#endif

/** define the RISC-V Privileg Level support
    1: M(achine)-mode
    2: M-mode + U(ser)-mode
    3: M-mode + S(upervisor)-mode + U-mode
*/
#define PRIVILEG_LEVEL      1
#define N_EXTENSION         0   //  Is “N” Standard Extension for User-Level Interrupts support?

/* ###########################  Core Function Access  ########################### */
/** \ingroup  CMSIS_Core_FunctionInterface
    \defgroup CMSIS_Core_RegAccFunctions CMSIS Core Register Access Functions
  @{
 */

/**
  \brief  Defines the address of CSR (Control and Status Register).
*/
enum riscv_machine_mode_csr_e {
	/* Name          |Address     |Description */
	/* Machine Information Registers */
	mvendorid   	= 0xF11,    // RO, Vendor ID.
	marchid     	= 0xF12,    // RO, Architecture ID.
	mimpid      	= 0xF13,    // RO, Implementation ID.
	mhartid     	= 0xF14,    // RO, Hardware thread ID.

	/* Machine Trap Setup */
	mstatus     	= 0x300,    // RW, Machine status register.
	misa        	= 0x301,    // RW, ISA and extensions
	medeleg     	= 0x302,    // RW, Machine exception delegation register.
	mideleg     	= 0x303,    // RW, Machine interrupt delegation register.
	mie         	= 0x304,    // RW, Machine interrupt-enable register.
	mtvec       	= 0x305,    // RW, Machine trap-handler base address.
	mcounteren  	= 0x306,    // RW, Machine counter enable.

	/* Machine Trap Handling */
	mscratch    	= 0x340,    // RW, Scratch register for machine trap handlers.
	mepc        	= 0x341,    // RW, Machine exception program counter.
	mcause      	= 0x342,    // RW, Machine trap cause.
	mtval       	= 0x343,    // RW, Machine bad address or instruction.
	mip         	= 0x344,    // RW, Machine interrupt pending.

	/* Machine Memory Protection */
	pmpcfg0     	= 0x3A0,    // RW, Physical memory protection configuration.
	pmpcfg1     	= 0x3A1,    // RW, Physical memory protection configuration, RV32 only.
	pmpcfg2     	= 0x3A2,    // RW, Physical memory protection configuration.
	pmpcfg3     	= 0x3A3,    // RW, Physical memory protection configuration, RV32 only.
	pmpaddr0    	= 0x3B0,    // RW, Physical memory protection address register.
	pmpaddr1    	= 0x3B1,    // RW, Physical memory protection address register.
	pmpaddr2    	= 0x3B3,    // RW, Physical memory protection address register.
	pmpaddr3    	= 0x3B4,    // RW, Physical memory protection address register.
	pmpaddr4    	= 0x3B5,    // RW, Physical memory protection address register.
	pmpaddr5    	= 0x3B6,    // RW, Physical memory protection address register.
	pmpaddr6    	= 0x3B7,    // RW, Physical memory protection address register.
	pmpaddr7    	= 0x3B8,    // RW, Physical memory protection address register.
	pmpaddr8    	= 0x3B9,    // RW, Physical memory protection address register.
	pmpaddr9    	= 0x3BA,    // RW, Physical memory protection address register.
	pmpaddr10   	= 0x3BB,    // RW, Physical memory protection address register.
	pmpaddr11   	= 0x3BC,    // RW, Physical memory protection address register.
	pmpaddr12   	= 0x3BD,    // RW, Physical memory protection address register.
	pmpaddr13   	= 0x3BE,    // RW, Physical memory protection address register.
	pmpaddr14   	= 0x3BF,    // RW, Physical memory protection address register.

	/* Machine Counter/Timers */
	mcycle 			= 0xB00, 	// RW, Machine cycle counter.
	minstret 		= 0xB02, 	// RW, Machine instructions-retired counter.
	mhpmcounter3	= 0xB03, 	// RW, Machine performance-monitoring counter3.
	mhpmcounter4	= 0xB04, 	// RW, Machine performance-monitoring counter4.
	mhpmcounter5	= 0xB05, 	// RW, Machine performance-monitoring counter5.
	mhpmcounter6	= 0xB06, 	// RW, Machine performance-monitoring counter6.
	// ...
	mhpmcounter31 	= 0xB1F, 	// RW, Machine performance-monitoring counter.
	mcycleh 		= 0xB80, 	// RW, Upper 32 bits of mcycle, RV32I only.
	minstreth 		= 0xB82, 	// RW, Upper 32 bits of minstret, RV32I only.
	mhpmcounter3h 	= 0xB83, 	// RW, Upper 32 bits of mhpmcounter3, RV32I only.
	mhpmcounter4h 	= 0xB84, 	// RW, Upper 32 bits of mhpmcounter4, RV32I only.
	mhpmcounter5h 	= 0xB85, 	// RW, Upper 32 bits of mhpmcounter5, RV32I only.
	mhpmcounter6h 	= 0xB86, 	// RW, Upper 32 bits of mhpmcounter6, RV32I only.
	// ...
	mhpmcounter31h  = 0xB9F,    // RW, Upper 32 bits of mhpmcounter31, RV32I only.

	/* Machine Counter Setup */
	mcountinhibit 	= 0x320, 	// RW, Machine counter-inhibit register.
	mhpmevent3      = 0x323, 	// RW, Machine performance-monitoring3 event selector.
	mhpmevent4      = 0x324, 	// RW, Machine performance-monitoring4 event selector.
	mhpmevent5      = 0x325, 	// RW, Machine performance-monitoring5 event selector.
	mhpmevent6      = 0x326, 	// RW, Machine performance-monitoring6 event selector.
	// ...
	mhpmevent31 	= 0x33F, 	// RW, 	Machine performance-monitoring event selector.

	/* Debug/Trace Registers (shared with Debug Mode) */
	tselect 		= 0x7A0, 	// RW, Debug/Trace trigger register select.
	tdata1 			= 0x7A1, 	// RW, First Debug/Trace trigger data register.
	tdata2 			= 0x7A2, 	// RW, Second Debug/Trace trigger data register.
	tdata3 			= 0x7A3, 	// RW, Third Debug/Trace trigger data register.

	/* Debug Mode Registers */
	dcsr 			= 0x7B0, 	// RW, Debug control and status register.
	dpc 			= 0x7B1, 	// RW, Debug PC.
	dscratch0 		= 0x7B2, 	// RW, Debug scratch register 0.
	dscratch1 		= 0x7B3  	// RW, Debug scratch register 1.
};

/**
  \brief  Bit offset definition for M-mode Status register.
*/
enum mstatus_bits_e {
	mstatus_bit_uie     = 0,    // U-mode interrupt enable
	mstatus_bit_sie     = 1,    // S-mode interrupt enable
	mstatus_bit_mie     = 3,    // M-mode interrupt enable
	mstatus_bit_upie    = 4,
	mstatus_bit_spie    = 5,
	mstatus_bit_mpie    = 7,    // MPIE = MIE when entering ISR, MIE = MPIE when exiting ISR by "mret" instruction
	mstatus_bit_mpp     = 11,   // 2 bits, MPP = current previlieg mode when entering ISR
};

/**
  \brief  Bit offset definition for M-mode Interrupt Pending register.
*/
enum mip_bits_e {
	mip_bit_usip        = 0,    // U-mode software interrupt pending
	mip_bit_ssip        = 1,    // S-mode software interrupt pending
	mip_bit_msip        = 3,    // M-mode software interrupt pending
	mip_bit_utip        = 4,    // U-mode timer interrupt pending
	mip_bit_stip        = 5,    // S-mode timer interrupt pending
	mip_bit_mtip        = 7,    // M-mode timer interrupt pending
	mip_bit_ueip        = 8,    // U-mode external interrupt pending
	mip_bit_seip        = 9,    // S-mode external interrupt pending
	mip_bit_meip        = 11    // M-mode external interrupt pending
};

/**
  \brief  Bit offset definition for M-mode Interrupt Enable register.
*/
enum mie_bits_e {
	mie_bit_usie        = 0,    // U-mode software interrupt enable
	mie_bit_ssie        = 1,    // S-mode software interrupt enable
	mie_bit_msie        = 3,    // M-mode software interrupt enable
	mie_bit_utie        = 4,    // U-mode timer interrupt enable
	mie_bit_stie        = 5,    // S-mode timer interrupt enable
	mie_bit_mtie        = 7,    // M-mode timer interrupt enable
	mie_bit_ueie        = 8,    // U-mode external interrupt enable
	mie_bit_seie        = 9,    // S-mode external interrupt enable
	mie_bit_meie        = 11    // M-mode external interrupt enable
};

/**
  \brief   Get PRIVLVL Register
  \details Returns the content of the PRIVLVL Register.
  \return               PRIVLVL Register value
 */
__STATIC_FORCEINLINE uint32_t __get_CPRIV(void)
{
#if defined(PRIVILEG_LEVEL) && (PRIVILEG_LEVEL > 1)
	// TODO: HW implementation to store current privleg mode is needed
	uint32_t ret = 1;

	//__ASM volatile ("csrr %0, priv_reg??" : "=r" (ret) );
	return (ret);
#else
	// M-mode only, always return true
	return 1;
#endif
}

/**
 *  @brief      Set one or more bits of a CSR register.
 *  @details    Set a given CSR register value with a given bits value.
 *  @param[in]  csr The given CSR address.
 *  @param[in]  bits The bits value to set the given CSR.
 *
 */
// csrrs rd, csr, rs1   ==> t = CSRs[csr]; CSRs[csr] = t | x[rs1]; x[rd] = t
#define __csr_set(csr, bits)                                                       \
    ({                                                                             \
        uint32_t reg_val = (uint32_t) bits;                                        \
        __ASM volatile ("csrrs %0, " #csr ", %1" : "=r"(reg_val) : "rK"(reg_val)); \
        reg_val;                                                                   \
    })

/**
 *  @brief      Clear one or more bits of a CSR register.
 *  @param[in]  csr The given CSR address.
 *  @param[in]  bits The mask of bits value to be cleared.
 *
 */
// csrrc rd, csr, rs1 ==> t = CSRs[csr]; CSRs[csr] = t &~x[rs1]; x[rd] = t
#define __csr_clear(csr, bits)                                                    \
    ({                                                                            \
        uint32_t reg_val = (uint32_t) bits;                                       \
        __ASM volatile("csrrc %0, " #csr ", %1" : "=r"(reg_val) : "rK"(reg_val)); \
        reg_val;                                                                  \
    })

/**
 *  @brief      Read a CSR register.
 *  @param[in]  csr The given CSR address.
 *
 *  @returns    The CSR register value.
 */
// csrr rd, csr   ==> x[rd] = CSRs[csr]
#define __csr_read(csr)                                                \
    ({                                                                 \
        uint32_t reg_val;                                              \
        __ASM volatile("csrr %0, " #csr : "=r"(reg_val) : : "memory"); \
        reg_val;                                                       \
    })

/**
 *  @brief      Write a CSR register with a given value.
 *  @param[in]  csr The address of the CSR to be written.
 *  @param[in]  value The value for the CSR writting.
 *
 */
// csrw csr, rs1    ==> CSRs[csr] = x[rs1]
#define __csr_write(csr, value)                                           \
    ({                                                                    \
        uint32_t reg_val = (uint32_t)(value);                             \
        __ASM volatile("csrw " #csr ", %0" : : "rK"(reg_val) : "memory"); \
    })

/**
  \brief   Restore the MIE bit
  \details This function enables interrupt by restoring the saved interrupt
  status register.

  __enable_irq_restore and __disable_irq_save normally appear in pairs
 */
__STATIC_FORCEINLINE void __enable_irq_restore(uint32_t mie)
{
	uint32_t flags;
	__ASM volatile("csrrs %0, mstatus, %1" : "=r"(flags) : "r"(mie));
}

/**
  \brief  Disable IRQ Interrupts and Save Status

  \details This function disables interrupts by clearing the MIE-bit in the
  mstatus register, and returns the previous value of that register.
  __disable_irq_save and __enable_irq_restore normally appear in pairs
*/
__STATIC_FORCEINLINE uint32_t __disable_irq_save(void)
{
	uint32_t flags;
	uint32_t mie = 0x8;

	__ASM volatile("csrrc %0, mstatus, %1" : "=r"(flags): "rK"(mie));
	flags = flags & mie;

	return flags;
}

/**
  \brief   Enable IRQ Interrupts
  \details Enables IRQ interrupts by setting the MPIE-bit in the MSTATUS.
           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_irq(void)
{
	__csr_set(mstatus, (1 << mstatus_bit_mie));
}

/**
  \brief   Disable IRQ Interrupts
  \details Disables IRQ interrupts by clearing the MPIE-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_irq(void)
{
	__csr_clear(mstatus, (1 << mstatus_bit_mie));
}

/**
  \brief   Set mstatus Register
  \details Writes the given value to the mstatus Register.
  \param [in]    control  mstatus Register value to set
 */
__STATIC_FORCEINLINE void __set_MSTATUS(uint32_t control)
{

	__ASM volatile("csrw mstatus, %0" ::"r"(control));
}

/**
  \brief   Get MCAUSE Register
  \details Returns the content of the MCAUSE Register.
  \return               MCAUSE Register value
 */
__STATIC_FORCEINLINE uint32_t __get_MCAUSE(void)
{
	uint32_t result;

	__ASM volatile("csrr %0, mcause" : "=r"(result));
	return (result);
}

/**
  \brief   Get MSTATUS Register
  \details Returns the content of the MSTATUS Register.
  \return               MSTATUS Register value
 */
__STATIC_FORCEINLINE uint32_t __get_MSTATUS(void)
{
	uint32_t result;

	__ASM volatile("csrr %0, mstatus" : "=r"(result));
	return (result);
}

#if defined(N_EXTENSION) && (N_EXTENSION == 1)

/**
  \brief   Set ustatus Register
  \details Writes the given value to the ustatus Register.
  \param [in]    control  ustatus Register value to set
 */
__STATIC_FORCEINLINE void __set_USTATUS(uint32_t control)
{

	__ASM volatile("csrw 0x0, %0" :: "r"(control));
}

/**
  \brief   Get UCAUSE Register
  \details Returns the content of the UCAUSE Register.
  \return               UCAUSE Register value
 */
__STATIC_FORCEINLINE uint32_t __get_UCAUSE(void)
{
	uint32_t result;

	__ASM volatile("csrr %0, 0x042" : "=r"(result));
	return (result);
}

/**
  \brief   Get USTATUS Register
  \details Returns the content of the USTATUS Register.
  \return               USTATUS Register value
 */
__STATIC_FORCEINLINE uint32_t __get_USTATUS(void)
{
	uint32_t result;

	__ASM volatile("csrr %0, 0x0" : "=r"(result));
	return (result);
}

#endif  // end of "#if defined(N_EXTENSION) && (N_EXTENSION == 1)"

/**
  \brief   Get Process Stack Pointer
  \details Returns the current value of the Process Stack Pointer (PSP).
  \return               PSP Register value
 */
__STATIC_FORCEINLINE uint32_t __get_SP(void)
{
	register uint32_t result;

	__ASM volatile("mv %0, sp" : "=r"(result));
	return (result);
}

/**
  \brief   Set Process Stack Pointer
  \details Assigns the given value to the Process Stack Pointer (PSP).
  \param [in]    topOfProcStack  Process Stack Pointer value to set
 */
__STATIC_FORCEINLINE void __set_SP(uint32_t topOfMainStack)
{
	__ASM volatile("mv sp, %0" ::"r"(topOfMainStack));
}


/**
  \brief   Get Process Stack Pointer
  \details Returns the current value of the Process Stack Pointer (PSP).
  \return               PSP Register value
 */
__STATIC_FORCEINLINE uint32_t __get_PSP(void)
{
	register uint32_t result;

	__ASM volatile("lw %0, (userStack)" : "=r"(result));
	return (result);
}


/**
  \brief   Set Process Stack Pointer
  \details Assigns the given value to the Process Stack Pointer (PSP).
  \param [in]    topOfProcStack  Process Stack Pointer value to set
 */
__STATIC_FORCEINLINE void __set_PSP(uint32_t topOfProcStack)
{
	__ASM volatile("sw %0, (userStack)(x0)" ::"r"(topOfProcStack));
}

/**
  \brief   Get Main Stack Pointer
  \details Returns the current value of the Main Stack Pointer (MSP).
  \return               MSP Register value
 */
__STATIC_FORCEINLINE uint32_t __get_MSP(void)
{
	register uint32_t result;

	__ASM volatile("lw %0, (kernelStack)" : "=r"(result));
	return (result);
}

/**
  \brief   Set Main Stack Pointer
  \details Assigns the given value to the Main Stack Pointer (MSP).
  \param [in]    topOfMainStack  Main Stack Pointer value to set
 */
__STATIC_FORCEINLINE void __set_MSP(uint32_t topOfMainStack)
{
	__ASM volatile("sw %0, (kernelStack)(x0)" ::"r"(topOfMainStack));
}

#if defined(PRIVILEG_LEVEL) && (PRIVILEG_LEVEL > 1)

/**
  \brief   Get the running mode is User Mode
  \details Read 0xC10 privilege register
  \return               Is User mode
 */
__STATIC_FORCEINLINE uint32_t __is_U_Mode()
{
	// TODO: no HW implementation currently
}

/**
  \brief   Get the running mode is Machine Mode
  \details Read 0xC10 privilege register
  \return               Is Machine mode
 */
__STATIC_FORCEINLINE uint32_t __is_M_Mode()
{
	// TODO: no HW implementation currently
}

#endif  // end of "#if defined(PRIVILEG_LEVEL) && (PRIVILEG_LEVEL > 1)"

/**
  \brief   Get current IRQ mode
  \details Returns the IRQ mode of current exception.
  \return               The IRQ mode.
  \                     0: System fault exception or ECall exception.
  \                     1: Standard interrupt.
 */
__STATIC_FORCEINLINE uint32_t __is_IRQ_Mode(void)
{
	return ((__get_MCAUSE() >> 31) & 0x01);
}

/**
  \brief   Get IPSR Register
  \details Returns the content of the mip CSR.
  \return               mip CSR value
 */
__STATIC_FORCEINLINE uint32_t __get_IPSR(void)
{
	if (__get_MCAUSE() & (1 << 31)) {
		return __csr_read(mip);
	} else {
		return 0;
	}
}

/**
  \brief   Get Priority Mask
  \details Returns the current state of the priority mask bit from the Priority Mask Register.
  \return               Priority Mask value
 */
__STATIC_FORCEINLINE uint32_t __get_PRIMASK(void)
{
	// RISC-V has no such "Priority Mask Register" design as ARM's, use MIE to indicates this status
	if (__get_MSTATUS() & (1 << 3)) {
		// M-mode Interrupt is enabled: Priority Mask bit = 0
		return 0;
	} else {
		// M-mode Interrupt is disabled: Priority Mask bit = 1
		return 1;
	}
}

/**
  \brief   Set Control Register
  \details Writes the given value to the Control Register.
  \param [in]    control  Control Register value to set
 */
__STATIC_FORCEINLINE  void __set_CONTROL(uint32_t control)
{
	// TODO:
}

/**
  \brief   Set Machine Trap-Vector Base-Address Register
  \details Writes the base address and mode to the Machine Trap-Vector Base-Address Register.
  \param [in]    vect_base  The vector table base address.
  \param [in]    mode  The vector table mode.
  \                    0: All exceptions set pc to BASE
  \                    1: Asynchronous interrupts set pc to BASE+4×cause
 */
__STATIC_FORCEINLINE  void __set_MTVEC(uint32_t vect_base, uint32_t mode)
{
	uint32_t reg_val;

	// base address must be 4-bytes aligned
	reg_val = (vect_base & 0xFFFFFFFC) | (mode & 0x03);
	__ASM volatile("csrw 0x305, %0" :: "r"(reg_val));
}

__STATIC_FORCEINLINE  uint32_t __get_MTVEC(void)
{
	return __csr_read(0x305);
}

/**
  \brief   Set Trigger Select Register
  \details Writes value to the Trigger Select Register.
  \param [in]    value Register value to write.
 */
__STATIC_FORCEINLINE  void __set_TSELECT(uint32_t value)
{
	__csr_write(tselect, value);
}

/**
  \brief   Get Trigger Select Register
  \details Returns the content of the tselect CSR.
  \return               tselect CSR value
 */
__STATIC_FORCEINLINE  uint32_t __get_TSELECT(void)
{
	return __csr_read(tselect);
}

/**
  \brief   Set Trigger Data 1
  \details Writes value to the Trigger Data 1 Register.
  \param [in]    value Register value to write.
 */
__STATIC_FORCEINLINE  void __set_TDATA1(uint32_t value)
{
	__csr_write(tdata1, value);
}

/**
  \brief   Get Trigger Data 1
  \details Returns the content of the tdata1 CSR.
  \return               tselect CSR value
 */
__STATIC_FORCEINLINE  uint32_t __get_TDATA1(void)
{
	return __csr_read(tdata1);
}

/**
  \brief   Set Trigger Data 2
  \details Writes value to the Trigger Data 2 Register.
  \param [in]    value Register value to write.
 */
__STATIC_FORCEINLINE  void __set_TDATA2(uint32_t value)
{
	__csr_write(tdata2, value);
}

/**
  \brief   Get Trigger Data 2
  \details Returns the content of the tdata2 CSR.
  \return               tselect CSR value
 */
__STATIC_FORCEINLINE  uint32_t __get_TDATA2(void)
{
	return __csr_read(tdata2);
}

#define __get_BASEPRI()      (0)
/*@} end of CMSIS_Core_RegAccFunctions */


/* ##########################  Core Instruction Access  ######################### */
/** \defgroup CMSIS_Core_InstructionInterface CMSIS Core Instruction Interface
  Access to dedicated instructions
  @{
*/

/* Define macros for porting to both thumb1 and thumb2.
 * For thumb1, use low register (r0-r7), specified by constraint "l"
 * Otherwise, use general registers, specified by constraint "r"
 */
#if defined (__thumb__) && !defined (__thumb2__)
#define __CMSIS_GCC_OUT_REG(r) "=l" (r)
#define __CMSIS_GCC_RW_REG(r) "+l" (r)
#define __CMSIS_GCC_USE_REG(r) "l" (r)
#else
#define __CMSIS_GCC_OUT_REG(r) "=r" (r)
#define __CMSIS_GCC_RW_REG(r) "+r" (r)
#define __CMSIS_GCC_USE_REG(r) "r" (r)
#endif

/**
  \brief   No Operation
  \details No Operation does nothing. This instruction can be used for code alignment purposes.
 */
#define __NOP() __ASM volatile ("nop")

/**
  \brief   Wait For Interrupt
  \details Wait For Interrupt is a hint instruction that suspends execution until one of a number of events occurs.
 */
#define __WFI() __ASM volatile ("wfi")


/**
  \brief   Wait For Event
  \details Wait For Event is a hint instruction that permits the processor to enter
           a low-power state until one of a number of events occurs.
 */
// RISC-V has no "wfe" instruction
#define __WFE() __ASM volatile ("nop")


/**
  \brief   Send Event
  \details Send Event is a hint instruction. It causes an event to be signaled to the CPU.
 */
// RISC-V has no "sev" instruction
#define __SEV() __ASM volatile ("nop")


/**
  \brief   Instruction Synchronization Barrier
  \details Instruction Synchronization Barrier flushes the pipeline in the processor,
           so that all instructions following the ISB are fetched from cache or memory,
           after the instruction has been completed.
 */
__STATIC_FORCEINLINE void __ISB(void)
{
	__ASM volatile("fence");
	/* 11 nop instructions to clean up pipeline */
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
	__ASM volatile("nop");
}


/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
__STATIC_FORCEINLINE void __DSB(void)
{
	__ASM volatile("fence");
}


/**
  \brief   Data Memory Barrier
  \details Ensures the apparent order of the explicit memory operations before
           and after the instruction, without ensuring their completion.
 */
__STATIC_FORCEINLINE void __DMB(void)
{
	__ASM volatile("fence");
}


/**
  \brief   Count leading zeros
  \details Counts the number of leading zeros of a data value.
  \param [in]  value  Value to count the leading zeros
  \return             number of leading zeros in value
 */
#define __CLZ __builtin_clz


/**
  \ingroup PULP_BUILTIN_MARCO
  @{
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t __ExtInsMaskFast(uint32_t size, uint32_t offset)
{
	return ((((size - 1)) << 5) | (offset));
}
__attribute__((always_inline)) __STATIC_INLINE uint32_t __ExtInsMaskSafe(uint32_t size, uint32_t offset)
{
	return ((((size - 1) & 0x1F) << 5) | (offset & 0x1F));
}

/*! @brief Packing of scalars into vectors */
#define __VEC_PACK2(x, y)                     __builtin_pulp_pack2((int16_t)  x, (int16_t)  y)
#define __VEC_PACKU2(x, y)                    __builtin_pulp_pack2((uint16_t) x, (uint16_t) y)
#define __VEC_PACK4(x, y, z, t)               __builtin_pulp_pack4((int8_t)   x, (int8_t)   y, (int8_t)  z, (int8_t)  t)
#define __VEC_PACKU4(x, y, z, t)              __builtin_pulp_pack4((uint8_t)  x, (uint8_t)  y, (uint8_t) z, (uint8_t) t)

/*! @brief Max */
#define __MAX(x, y)              __builtin_pulp_maxsi((x), (y))
#define __MAX2(x, y)             __builtin_pulp_max2(x, y)
#define __MAX4(x, y)             __builtin_pulp_max4(x, y)

#define __MAXU2(x, y)            __builtin_pulp_maxu2(x, y)
#define __MAXU4(x, y)            __builtin_pulp_maxu4(x, y)

/*! @brief Min */
#define __MIN2(x, y)             __builtin_pulp_min2(x, y)
#define __MIN4(x, y)             __builtin_pulp_min4(x, y)

#define __MINU2(x, y)            __builtin_pulp_minu2(x, y)
#define __MINU4(x, y)            __builtin_pulp_minu4(x, y)

/*! @brief Clip */
#define __CLIP(x, precision)     __builtin_pulp_clip(x, -(1<<(precision)), (1<<precision)-1)
#define __CLIPU(x, precision)    __builtin_pulp_clipu(x, 0, (1<<precision)-1)

/*! @brief Abs */
#define __ABS2(x)                __builtin_pulp_abs2(x)
#define __ABS4(x)                __builtin_pulp_abs4(x)

/* Unary minus */
#define __NEG2(x)                __builtin_pulp_neg2(x)
#define __NEG4(x)                __builtin_pulp_neg4(x)

/* Addition */
#define __ADD2(x, y)             __builtin_pulp_add2(x, y)
#define __ADD4(x, y)             __builtin_pulp_add4(x, y)

/* Substraction */
#define __SUB2(x, y)             __builtin_pulp_sub2(x, y)
#define __SUB4(x, y)             __builtin_pulp_sub4(x, y)

/* Average */
#define __AVG2(x, y)             __builtin_pulp_avg2(x, y)
#define __AVG4(x, y)             __builtin_pulp_avg4(x, y)

/* Average unsigned */
#define __AVGU2(x, y)            __builtin_pulp_avgu2(x, y)
#define __AVGU4(x, y)            __builtin_pulp_avgu4(x, y)

/* Bitwise and */
#define __AND2(x, y)             __builtin_pulp_and2(x, y)
#define __AND4(x, y)             __builtin_pulp_and4(x, y)

/* Bitwise or */
#define __OR2(x, y)              __builtin_pulp_or2(x, y)
#define __OR4(x, y)              __builtin_pulp_or4(x, y)

/* Bitwise exor */
#define __EXOR2(x, y)            __builtin_pulp_exor2(x, y)
#define __EXOR4(x, y)            __builtin_pulp_exor4(x, y)

/* Logical shift right */
#define __SRL2(x, y)             __builtin_pulp_srl2(x, y)
#define __SRL4(x, y)             __builtin_pulp_srl4(x, y)

/* Arithmetic shift right */
#define __SRA2(x, y)             __builtin_pulp_sra2(x, y)
#define __SRA4(x, y)             __builtin_pulp_sra4(x, y)

/* Logical shift left */
#define __SLL2(x, y)             __builtin_pulp_sll2(x, y)
#define __SLL4(x, y)             __builtin_pulp_sll4(x, y)

/*! @brief Mac */
#define __MAC(acc, x, y)         __builtin_pulp_mac(x, y, acc)
#define __MSU(acc, x, y)         __builtin_pulp_msu(x, y, acc)
#define __MACS(acc, x, y)        __builtin_pulp_macs(x, y, acc)
#define __MACHHS(acc, x, y)      __builtin_pulp_machhs(x, y, acc)
#define __MACU(acc, x, y)        __builtin_pulp_macu(x, y, acc)
#define __MACHHU(acc, x, y)      __builtin_pulp_machhu(x, y, acc)

#define __MACSN(acc, x, y, n)      __builtin_pulp_macsN(x, y, acc, n)
#define __MACUN(acc, x, y, n)      __builtin_pulp_macuN(x, y, acc, n)
#define __MACSRN(acc, x, y, n)     __builtin_pulp_macsRN(x, y, acc, n, (1<<(n-1)))
#define __MACURN(acc, x, y, n)     __builtin_pulp_macuRN(x, y, acc, n, (1<<(n-1)))

#define __MACHHSN(acc, x, y, n)    __builtin_pulp_machhsN(x, y, acc, n)
#define __MACHHUN(acc, x, y, n)    __builtin_pulp_machhuN(x, y, acc, n)
#define __MACHHSRN(acc, x, y, n)   __builtin_pulp_machhsN(x, y, acc, n, (1<<(n-1)))
#define __MACHHURN(acc, x, y, n)   __builtin_pulp_machhuN(x, y, acc, n, (1<<(n-1)))

/*! @brief Multiplications */
#define __MULS(x, y)            __builtin_pulp_muls(x, y)
#define __MULU(x, y)            __builtin_pulp_mulu(x, y)
#define __MULHHS(x, y)          __builtin_pulp_mulhhs(x, y)
#define __MULHHU(x, y)          __builtin_pulp_mulhhu(x, y)

#define __MULSN(x, y, n)        __builtin_pulp_mulsN(x, y, n)
#define __MULSRN(x, y, n)       __builtin_pulp_mulsRN(x, y, n, (1<<(n-1)))
#define __MULUN(x, y, n)        __builtin_pulp_muluN(x, y, n)
#define __MULURN(x, y, n)       __builtin_pulp_muluRN(x, y, n, (1<<(n-1)))

#define __MULHHSN(x, y, n)      __builtin_pulp_mulhhsN(x, y, n)
#define __MULHHSRN(x, y, n)     __builtin_pulp_mulhhsRN(x, y, n, (1<<(n-1)))
#define __MULHHUN(x, y, n)      __builtin_pulp_mulhhuN(x, y, n)
#define __MULHHURN(x, y, n)     __builtin_pulp_mulhhuRN(x, y, n, (1<<(n-1)))

/*! @brief Vectorial product and sum of products */
#define __DOTP2(x, y)           __builtin_pulp_dotsp2(x, y)
#define __DOTPU2(x, y)          __builtin_pulp_dotup2(x, y)
#define __DOTPUS2(x, y)         __builtin_pulp_dotusp2(x, y)

#define __SUM_DOTP2(x, y, z)    __builtin_pulp_sdotsp2(x, y, z)
#define __SUM_DOTPU2(x, y, z)   __builtin_pulp_sdotup2(x, y, z)
#define __SUM_DOTPUS2(x, y, z)  __builtin_pulp_sdotusp2(x, y, z)

#define __DOTP4(x, y)           __builtin_pulp_dotsp4(x, y)
#define __DOTPU4(x, y)          __builtin_pulp_dotup4(x, y)
#define __DOTPUS4(x, y)         __builtin_pulp_dotusp4(x, y)

#define __SUM_DOTP4(x, y, z)    __builtin_pulp_sdotsp4(x, y, z)
#define __SUM_DOTPU4(x, y, z)   __builtin_pulp_sdotup4(x, y, z)
#define __SUM_DOTPUS4(x, y, z)  __builtin_pulp_sdotusp4(x, y, z)


/*! @brief Complex Multiplication, Q15x15 into Q15, with optional post scaling by 1 or 2 */
#define __CPLX_MULS(x, y)        __builtin_pulp_cplxmuls(x, y)
#define __CPLX_MULS_DIV2(x, y)   __builtin_pulp_cplxmulsdiv2(x, y)
#define __CPLX_MULS_DIV4(x, y)   __builtin_pulp_cplxmulsdiv4(x, y)

/*! @brief Complex conjugate */
#define __CPLX_CONJ(x)           __builtin_pulp_cplx_conj(x)

/*! @brief Complex rotation by -pi/2 */
#define __CPLX_MJ(x)             __builtin_pulp_cplxmjrot2(x)

/*! @brief Complex substration, result rotated by -pi/2 */
#define __SUB2_ROTMJ(x, y)       __builtin_pulp_sub2rotmj(x, y)

/*! @brief Complex addition with post scaling by 1 or 2 */
#define __ADD2_DIV2(x, y)        __builtin_pulp_add2div2(x, y)
#define __ADD2_DIV4(x, y)        __builtin_pulp_add2div4(x, y)

/*! @brief Complex subtraction with post scaling by 1 or 2 */
#define __SUB2_DIV2(x, y)        __builtin_pulp_sub2div2(x, y)
#define __SUB2_DIV4(x, y)        __builtin_pulp_sub2div4(x, y)

/*! @brief Viterbi Max and Viterbi Select, pair of Q15 */
#define __VIT_MAX(x, y)          __builtin_pulp_vitmax2(x, y)
#define __VIT_SEL(x, y)          __builtin_pulp_vitsel2(x, y)

/*! @brief Position of the most significant bit of x */
#define __FL1(x)                 __builtin_pulp_fl1(x)

/*! @brief Number of sign bits */
#define __CLB(x)                 __builtin_pulp_clb(x)

/*! @brief Bit Extraction */
#define __BIT_EXTRACT(src, size, off)           __builtin_pulp_bextract(src, size, off)
#define __BIT_EXTRACTU(src, size, off)          __builtin_pulp_bextractu(src, size, off)

#define __BIT_EXTRACT_R(src, size, off)         __builtin_pulp_bextract_r(src,  __ExtInsMaskFast(size, off))
#define __BIT_EXTRACTU_R(src, size, off)        __builtin_pulp_bextractu_r(src, __ExtInsMaskFast(size, off))

#define __BIT_EXTRACT_R_SAFE(src, size, off)    __builtin_pulp_bextract_r(src,  __ExtInsMaskSafe(size, off))
#define __BIT_EXTRACTU_R_SAFE(src, size, off)   __builtin_pulp_bextractu_r(src, __ExtInsMaskSafe(size, off))

/*! @brief Bit insertion */
#define __BIT_INSERT(dst, src, size, off)        __builtin_pulp_binsert(dst, ~(((1<<size)-1)<<off), src, (((1<<size)-1)<<off), off)
#define __BIT_INSERT_R(dst, src, size, off)      __builtin_pulp_binsert_r(dst, src, __ExtInsMaskFast(size, off))
#define __BIT_INSERT_R_SAFE(dst, src, size, off) __builtin_pulp_binsert_r(dst, src, __ExtInsMaskSafe(size, off))

/*! @brief 1 bit rotation to the right, 32 bits input */
#define __ROTR(x)                                __builtin_pulp_rotr(x)

/*! @brief Add with normalization and rounding */
#define __ADD_ROUND_NORM(x, y, scale)            __builtin_pulp_addRN(x, y, scale, (1<<(scale-1)))
#define __ADD_ROUND_NORM_REG(x, y, scale)        __builtin_pulp_addRN_r(x, y, scale)
#define __ADD_ROUND_NORMU(x, y, scale)           __builtin_pulp_adduRN(x, y, scale, (1<<(scale-1)))
#define __ADD_ROUND_NORMU_REG(x, y, scale)       __builtin_pulp_adduRN_r(x, y, scale)

/*! @brief Normalization and rounding */
#define __ROUND_NORM(x, scale)                   __builtin_pulp_addRN(x, 0, scale, (1<<(scale-1)))
#define __ROUND_NORM_REG(x, scale)               __builtin_pulp_addRN_r(x, 0, scale)
#define __ROUND_NORMU(x, scale)                  __builtin_pulp_adduRN(x, 0, scale, (1<<(scale-1)))
#define __ROUND_NORMU_REG(x, scale)              __builtin_pulp_adduRN_r(x, 0, scale)
/*@} end of group PULP_BUILTIN_MARCO */

/**
  \ingroup PULP_VECTOR_TYPE
  @{
 */
typedef   signed short v2s   __attribute__((vector_size(4)));
typedef unsigned short v2u   __attribute__((vector_size(4)));

typedef   signed char  v4s   __attribute__((vector_size(4)));
typedef unsigned char  v4u   __attribute__((vector_size(4)));

typedef signed char    v4qi   __attribute__((vector_size(4)));
typedef unsigned char  v4qu   __attribute__((vector_size(4)));

typedef signed short   v2hi   __attribute__((vector_size(4)));
typedef unsigned short v2hu   __attribute__((vector_size(4)));
/*@} end of group PULP_VECTOR_TYPE */

#pragma GCC diagnostic pop

#endif /* __CMSIS_GCC_H */
