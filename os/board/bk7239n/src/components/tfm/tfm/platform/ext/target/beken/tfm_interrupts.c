/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#include <stdint.h>

#include "cmsis.h"
#include "spm_ipc.h"
#include "tfm_hal_interrupt.h"
#include "tfm_peripherals_def.h"
#include "ffm/interrupt.h"
#include "load/interrupt_defs.h"
#include <components/log.h>
#include "exception_info.h"

#define TAG "I"

#define SOC_APB_WDT_REG_BASE	(0x54800000)
#define REBOOT_TAG_ADDR		(0x30003FF8)
#define REBOOT_TAG_REQ		(0xAA55AA55)

static struct irq_t timer0_irq = {0};

typedef void (*nsfptr_t) (void *arg) __attribute__((cmse_nonsecure_call));

nsfptr_t g_ns_entry;

uint32_t *dump_cb_reg = NULL;
__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_register_dump_callback(uint32_t cb_address, uint32_t cb_arg)
{

    /* Get the address of non-secure code entry point to jump there */
    //uint32_t entry_ptr = tfm_spm_hal_get_ns_entry_point();

    /* Clears LSB of the function address to indicate the function-call
     * will perform the switch from secure to non-secure
     */
	if((void *)cb_address == NULL)
		return PSA_ERROR_INVALID_ARGUMENT;

	g_ns_entry =  (nsfptr_t)cmse_nsfptr_create(cb_address);
	dump_cb_reg = (uint32_t*)cmse_nsfptr_create(cb_arg);

	return PSA_SUCCESS;
}

static void ns_dump_callback()
{
	if(dump_cb_reg) {
		store_and_dump_copy_context(dump_cb_reg);
	}
	g_ns_entry(dump_cb_reg);
}

void  NS_update_wdt(uint32_t val)
{
	REG_WRITE(SOC_APB_WDT_REG_BASE + 4 * 4, 0x5A0000 | val);
	REG_WRITE(SOC_APB_WDT_REG_BASE + 4 * 4, 0xA50000 | val);
}

void bfhfnmi_handler(void)
{
    if(__TZ_get_CONTROL_NS() & (1UL << 1)) //CONTROL_NS.SPSEL
    {
        uint32_t *psp_ns_pointer = (uint32_t  *)__TZ_get_PSP_NS();
        BK_LOGF(TAG, "\r\npsp_ns_p:0x%p\r\n", psp_ns_pointer);
        printf_word_buf_hex(psp_ns_pointer, 128);
    }
    else
    {
	    uint32_t *msp_ns_pointer = (uint32_t  *)__TZ_get_MSP_NS();  //Tizen always use msp_ns
		BK_LOGF(TAG, "\r\nmsp_ns_p:0x%p\r\n", msp_ns_pointer);
		printf_word_buf_hex(msp_ns_pointer, 128);
    }
	ns_dump_callback();
	while(1);
}

__attribute__((naked)) void NMI_Handler(void)
{
	EXCEPTION_INFO(EXCEPTION_TYPE_SECUREFAULT);
	if((*(volatile uint32_t *)(REBOOT_TAG_ADDR)) == REBOOT_TAG_REQ) /*'reset' do not triger dump*/
	{
		while(1);
	}

	NS_update_wdt(0xFFF0);
	BK_LOGF(TAG, "NMI\r\n");
	bfhfnmi_handler();
}

__attribute__((naked)) void HardFault_Handler(void)
{
	EXCEPTION_INFO(EXCEPTION_TYPE_HARDFAULT);
	BK_LOGF(TAG, "HF\r\n");
	bfhfnmi_handler();
}

__attribute__((naked)) void MemManage_Handler(void)
{
	EXCEPTION_INFO(EXCEPTION_TYPE_MEMFAULT);
	BK_LOGF(TAG, "MM\r\n");
	bfhfnmi_handler();
}

__attribute__((naked)) void BusFault_Handler(void)
{
	EXCEPTION_INFO(EXCEPTION_TYPE_BUSFAULT);
	BK_LOGF(TAG, "BF\r\n");
	bfhfnmi_handler();
}

__attribute__((naked)) void UsageFault_Handler(void)
{
	EXCEPTION_INFO(EXCEPTION_TYPE_USAGEFAULT);
	BK_LOGF(TAG, "UF\r\n");
	bfhfnmi_handler();
}

__attribute__((naked)) void SecureFault_Handler(void)
{
	EXCEPTION_INFO(EXCEPTION_TYPE_SECUREFAULT);
	BK_LOGF(TAG, "SF\r\n");
	bfhfnmi_handler();
}

void UART1_Handler(void)
{
	BK_LOGF(TAG, "UA\r\n");
	//while(1);
}

void GPIO_Handler(void)
{
	BK_LOGF(TAG, "GP\r\n");
	//while(1);
}

void TFM_TIMER0_Handler(void)
{
    spm_handle_interrupt(timer0_irq.p_pt, timer0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_timer0_irq_init(void *p_pt,
                                          struct irq_load_info_t *p_ildi)
{
    timer0_irq.p_ildi = p_ildi;
    timer0_irq.p_pt = p_pt;

    NVIC_SetPriority(SVCall_IRQn, 0); //SVCall_IRQn Prority default 0
    NVIC_SetPriority(TFM_TIMER0_IRQ, DEFAULT_IRQ_PRIORITY);
    NVIC_ClearTargetState(TFM_TIMER0_IRQ);
    NVIC_DisableIRQ(TFM_TIMER0_IRQ);

    return TFM_HAL_SUCCESS;
}

#ifdef PSA_API_TEST_IPC
static struct irq_t ff_test_uart_irq;

void FF_TEST_UART_IRQ_Handler(void)
{
    spm_handle_interrupt(ff_test_uart_irq.p_pt, ff_test_uart_irq.p_ildi);
}

enum tfm_hal_status_t ff_test_uart_irq_init(void *p_pt,
                                            struct irq_load_info_t *p_ildi)
{
    ff_test_uart_irq.p_ildi = p_ildi;
    ff_test_uart_irq.p_pt = p_pt;

    NVIC_SetPriority(FF_TEST_UART_IRQ, DEFAULT_IRQ_PRIORITY);
    NVIC_ClearTargetState(FF_TEST_UART_IRQ);
    NVIC_DisableIRQ(FF_TEST_UART_IRQ);

    return TFM_HAL_SUCCESS;
}

#endif
