/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#include "ameba_soc.h"
#include "ameba_plic.h"

typedef void (*IRQHandler_t)(uint32_t);
IMAGE2_RAM_DATA_SECTION
static uint32_t plic_irq_id = 0;

/**
  \brief   PLIC Interrupt handler function
  \details The interrupt handler function of PLIC, which should be called by root interrup handler
            with cause ID = MIE(11)
  \return  void.
 */
IMAGE2_RAM_TEXT_SECTION
void plic_interrupt_handler_ram(__UNUSED uint32_t irq)
{
	IRQHandler_t irq_handler;
	uint32_t nvic_irq;
	uint32_t old_pri;
	uint32_t cur_pri;

	/* mask KR4 irqs */
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, HAL_READ32(PMC_BASE, SYSPMC_CTRL) | PMC_BIT_KR4_IRQ_MASK);
	plic_irq_id = plic_claim_irq();
	/* unmask kr4 irqs */
	HAL_WRITE32(PMC_BASE, SYSPMC_CTRL, HAL_READ32(PMC_BASE, SYSPMC_CTRL) & ~PMC_BIT_KR4_IRQ_MASK);

	if (plic_irq_id == 0) {
		return;
	}

	nvic_irq = plic_irq_id + NVIC_PLIC_IRQ_OFFSET;
	irq_handler = (IRQHandler_t)NVIC_GetVector((IRQn_Type) nvic_irq);
	if (irq_handler != (IRQHandler_t) 0) {
		old_pri = plic_get_threshold();
		cur_pri = plic_get_irq_priority(plic_irq_id);
		plic_set_threshold(cur_pri);
#ifdef CONFIG_SYS_NESTED_IRQ
		__enable_irq();
#endif
		irq_handler(__NVIC_GetVectorArg((IRQn_Type) nvic_irq));
#ifdef CONFIG_SYS_NESTED_IRQ
		__disable_irq();
#endif
		plic_set_threshold(old_pri);
	} else {
		DBG_8195A("nvic_irq %d Fun Not Assign!!!!!\n", nvic_irq);
	}
	plic_complete_irq(plic_irq_id);
	plic_irq_id = 0;
}

/**
  \brief   PLIC get active IRQ Id.
  \details To get currently active IRQ Id.
  \return  The active IRQ Id.
 */
IMAGE2_RAM_TEXT_SECTION
uint32_t plic_get_active_irq_id_ram(void)
{
	return plic_irq_id;
}

/**
  \brief   Get Active Interrupt
  \details Reads the active register in the NVIC and returns the active bit for the device specific interrupt.
  \param [in]      IRQn  Device specific interrupt number.
  \return             0  Interrupt status is not active.
  \return             1  Interrupt status is active.
  \note    IRQn must not be negative.
 */
IMAGE2_RAM_TEXT_SECTION
uint32_t __NVIC_GetActive_IRQ(IRQn_Type IRQn)
{
	/* U mode does not has the right */
	uint32_t reg_val;
	uint32_t is_irq;
	int32_t irq_n;

	if (IRQn >= PLIC_MAX_IRQn) {
		return 0;
	}

	reg_val = __csr_read(mcause);
	is_irq = ((reg_val >> 31) != 0);
	irq_n = (int32_t)(reg_val & 0x7FFFFFFF);

	if ((int32_t) IRQn < 0) {
		if (!is_irq) {
			return (((int32_t) IRQn + MAX_SYSTEM_EXECPTION_NUM) == irq_n);
		} else {
			return 0;
		}
	}

	if (is_irq) {
#ifdef CONFIG_CPU_RV_RTK_PLIC
		if ((int32_t) IRQn >= NVIC_PLIC_IRQ_OFFSET) {
			irq_n = (int32_t)(plic_get_active_irq_id_ram() + NVIC_PLIC_IRQ_OFFSET);
			return (irq_n == (int32_t) IRQn);
		}
#endif

		return (irq_n == (int32_t) IRQn);
	}
	return 0;
}

