/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#ifndef __PLIC_H
#define __PLIC_H

#define PLIC_BASE                       RV_INTC_BASE
#define PLIC_INT_PRI_BITS               3               // Interrupt priority bits, max interrupt priority = 2^PLIC_INT_PRI_BITS-1. 2 ~ 4
#define PLIC_INT_PRIS                   ((1 << PLIC_INT_PRI_BITS) - 1)

#define PLIC_INT_PRI_REG_OFFSET         0x0800          // the address offset of Interrupt Priority Register
#define PLIC_INT_PEND_REG_OFFSET        0x0C00          // the address offset of Interrupt Pending Register
#define PLIC_INT_SIP_REG_OFFSET         MSIP_ADDR       // the address offset of Software Interrupt Pending Register
#define PLIC_INT_CLAIM_REG_OFFSET       0x0014          // the address offset of Interrupt Claim Register (Pending IRQ ID reading)
#define PLIC_INT_COMP_REG_OFFSET        0x0018          // the address offset of Interrupt Complete Register (IRQ Ack)
#define PLIC_INT_THRD_REG_OFFSET        0x001C          // the address offset of Interrupt Threshold Register
#define PLIC_INT_EN_REG_OFFSET          0x0020          // the address offset of Interrupt Enable Register

#define MAX_HART_NUM                    8               // Max HART number supported
#define PLIC_HART_SELF                  0xFFFFFFFFUL    // Get PLIC self id from mhartid


#define PLIC_HART_REG(hart, reg) REG32((PLIC_BASE) + ((hart) << 8) + (reg))

#define PLIC_HART_IRQ_READ(reg, hart, irq) \
    PLIC_HART_REG((hart), (reg) + (((irq) >> 5) << 2))

#define PLIC_HART_IRQ_SET(reg, hart, irq) \
do { \
    PLIC_HART_REG((hart), (reg) + (((irq) >> 5) << 2)) |= (1 << ((irq) & 0x1f)); \
} while(0)

#define PLIC_HART_IRQ_CLEAR(reg, hart, irq) \
do { \
    PLIC_HART_REG((hart), (reg) + (((irq) >> 5) << 2)) &= ~(1 << ((irq) & 0x1f)); \
} while(0)

#define PLIC_IRQ_REG(reg, irq) REG32((PLIC_BASE) + (reg) + ((irq) << 2))
#define PLIC_IRQ_PEND_REG(irq) \
  REG32((PLIC_BASE) + (PLIC_INT_PEND_REG_OFFSET) + ((irq >> 5) << 2))

#define PLIC_IRQ_SET(reg, irq) \
do { \
    REG32((reg) + ((irq) << 2)) |= (1 << ((irq) & 0x1f)); \
} while(0)

#define PLIC_IRQ_CLEAR(reg, irq) \
do { \
    REG32((reg) + ((irq) << 2)) &= ~(1 << ((irq) & 0x1f)); \
} while(0)

/**
  \brief   Enable an Interrupt.
  \details To enable an interrupt whith a given IRQ ID.
  \param   irq_id the interrupt ID.
 */
extern void plic_enable_irq(uint32_t irq_id);

/**
  \brief   Disable an Interrupt.
  \details To disable an interrupt whith a given IRQ ID.
  \param   irq_id the interrupt ID.
 */
extern void plic_disable_irq(uint32_t irq_id);

/**
  \brief   Get the enabling of an Interrupt.
  \details To enable an interrupt whith a given IRQ ID.
  \param   irq_id the interrupt ID.
  \return  The status of the interrupt enabling.
 */
extern uint32_t plic_get_irq_enable(uint32_t irq_id);

/**
  \brief   Set the enable status of 32 Interrupts in the same register.
  \details To set 32 interrupt enable bits with a given register id
  \param   irq_id the interrupt ID.
  \return  The status of the interrupt enabling.
 */
extern void plic_set_enable(uint32_t reg_id, uint32_t interrupt);

/**
  \brief   Get the enable status of 32 Interrupts in the same register.
  \details To get 32 interrupt enable bits with a given register id
  \param   irq_id the interrupt ID.
  \return  The status of the interrupt enabling.
 */
extern uint32_t plic_get_enable(uint32_t reg_id);



/**
  \brief   Write the interrupt enable bit maps.
  \details To write the interrupt enble bit maps
  \param   enable the buffer of the interrupt enable bits map.
 */
extern void plic_write_enables(uint32_t *enable);

/**
  \brief   Set Interrupt Priority.
  \details Set the Priority of a given Interrupt ID. IRQ ID 0 is reserved.
  \param   irq_id the interrupt ID.
  \param   priority the priority of the interrupt ID.
 */
extern void plic_set_irq_priority(uint32_t irq_id, uint32_t priority);

/**
  \brief   Get Interrupt Priority of given IRQ ID.
  \details Get the Priority of a given Interrupt ID. IRQ ID 0 is reserved.
  \param   irq_id the interrupt ID.
  \return  The priority of given IRQ ID.
 */
extern uint32_t plic_get_irq_priority(uint32_t irq_id);

/**
  \brief   Get Pending Interrupt.
  \details Get all the interrupt pending bits map. All value 0 for no pending interrupt.
  \param   irq_pending the buffer to store the read interrupt pending status. the buffser size
                       must be enough to store all the interrupr pending bits map.
  \param   priority the priority of the interrupt ID.
 */
extern void plic_read_pendings(uint32_t *irq_pending);

/**
  \brief   Get the pending status of an interrupt.
  \details Get the pending status of the given IRQ ID.
  \param   irq_id the IRQ ID.
  \return  The interrupt pending status of the given IRQ ID.
           0: interrupt is not pending
           1: interrupt is pending.
 */
extern uint32_t plic_get_irq_pending(uint32_t irq_id);

/**
  \brief   To claim the highest priority pending interrupt.
  \details Claims the highest priority pending interrupt and get its IRQ ID.
  \return  The IRQ ID of the highest priority pending interrupt.
 */
extern uint32_t plic_claim_irq(void);

/**
  \brief   To complete an interrupt.
  \details Completes an interrupt handleing and clean its pending status.
  \param   irq_id The given IRQ ID of the interrupt to be completed.
  \return  void.
 */
extern void plic_complete_irq(uint32_t irq_id);

/**
  \brief   Set interrupt priority threshold.
  \details Set the interrupt priority threshold. Only interrupts with its
           priority higher than this threshold can active.
  \param   priority The interrupt priority threshold.
  \return  void.
 */
extern void plic_set_threshold(uint32_t priority);

/**
  \brief   Get interrupt priority threshold.
  \details Get the interrupt priority threshold. Only interrupts with its
           priority higher than this threshold can active.
  \param   priority The interrupt priority threshold.
  \return  void.
 */
extern uint32_t plic_get_threshold(void);

/**
  \brief   Set software interrupt pending.
  \details To assert the software interrupt pending signal of the Hardware Architecture Target.
  \param   hart The hart target to set software interrupt.
  \return  void.
 */
extern void plic_set_sip(uint32_t hart);

/**
  \brief   Clear software interrupt pending.
  \details To clear the software interrupt pending signal of the Hardware Architecture Target.
  \param   hart The hart target to clear software interrupt.
  \return  void.
 */
extern void plic_clear_sip(uint32_t hart);

/**
  \brief   PLIC HAL initialization
  \details To initial the PLIC HAL code.
  \return  void.
 */
extern void plic_init(void);

/**
  \brief   PLIC get active IRQ Id.
  \details To get currently active IRQ Id.
  \return  The active IRQ Id.
 */
extern uint32_t plic_get_active_irq_id(void);
extern uint32_t plic_get_active_irq_id_ram(void);

#endif /* end of #define __PLIC_H */
