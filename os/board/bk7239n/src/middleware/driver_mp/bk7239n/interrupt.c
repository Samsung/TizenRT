// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "icu_driver.h"
#include "interrupt_base.h"
#include <driver/int.h>
#include "sdkconfig.h"
#include "armstar.h"
#include "arch_interrupt.h"
#include "components/log.h"

#if defined(CONFIG_FREERTOS_TRACE)
#include "trcRecorder.h"
#endif

#if defined(CONFIG_ARCH_INT_STATIS)
static uint32_t s_int_statis[InterruptMAX_IRQn] = {0};
#define INT_INC_STATIS(irq) s_int_statis[(irq)] ++
#else
#define INT_INC_STATIS(irq)
#endif

#if (defined(CONFIG_FREERTOS_TRACE))
#define IRQ_TRACE_BEGIN(irq)	xTraceISRBegin(xGetTraceISRHandle(irq))
#define IRQ_TRACE_END()			xTraceISREnd(0)
#else
#define IRQ_TRACE_BEGIN(irq)
#define IRQ_TRACE_END()
#endif

#define ARCH_ISR_HANDLER(irq)  \
{\
	INT_INC_STATIS(irq);\
	IRQ_TRACE_BEGIN(irq);\
	int_group_isr_t isr_cb;\
	isr_cb = arch_interrupt_get_handler(irq);\
	if (isr_cb != NULL) {\
		(*(isr_cb))();\
	}\
	IRQ_TRACE_END();\
}

void soc_isr_init(void)
{
	arch_isr_entry_init();
}

void soc_isr_deinit(void)
{
    /*to do something*/
}

void __attribute__ ((interrupt)) DMA0_NSEC_Handler(void)
{
	ARCH_ISR_HANDLER(0);
}

void __attribute__ ((interrupt)) ENCP_SEC_Handler(void)
{
	ARCH_ISR_HANDLER(1);
}

void __attribute__ ((interrupt)) ENCP_NSEC_Handler(void)
{
	ARCH_ISR_HANDLER(2);
}

extern uint32_t     g_wdt_handler_lr;
void __attribute__ ((interrupt)) TIMER0_Handler(void)
{
//	g_wdt_handler_lr = __get_LR();   TODO for tizen
	ARCH_ISR_HANDLER(3);
}

void __attribute__ ((interrupt)) UART0_Handler(void)
{
	ARCH_ISR_HANDLER(4);
}

void __attribute__ ((interrupt)) PWM0_Handler(void)
{
	ARCH_ISR_HANDLER(5);
}

void __attribute__ ((interrupt)) I2C0_Handler(void)
{
	ARCH_ISR_HANDLER(6);
}

void __attribute__ ((interrupt)) SPI0_Handler(void)
{
	ARCH_ISR_HANDLER(7);
}

void __attribute__ ((interrupt)) SARADC_Handler(void)
{
	ARCH_ISR_HANDLER(8);
}

void __attribute__ ((interrupt)) UART3_Handler(void)
{
	ARCH_ISR_HANDLER(9);
}

void __attribute__ ((interrupt)) SDIO_Handler(void)
{
	ARCH_ISR_HANDLER(10);
}

void __attribute__ ((interrupt)) GDMA_Handler(void)
{
	ARCH_ISR_HANDLER(11);
}

void __attribute__ ((interrupt)) LA_Handler(void)
{
	ARCH_ISR_HANDLER(12);
}

void __attribute__ ((interrupt)) TIMER1_Handler(void)
{
	ARCH_ISR_HANDLER(13);
}

void __attribute__ ((interrupt)) I2C1_Handler(void)
{
	ARCH_ISR_HANDLER(14);
}

void __attribute__ ((interrupt)) UART1_Handler(void)
{
	ARCH_ISR_HANDLER(15);
}

void __attribute__ ((interrupt)) UART2_Handler(void)
{
	ARCH_ISR_HANDLER(16);
}

void __attribute__ ((interrupt)) SPI1_Handler(void)
{
	ARCH_ISR_HANDLER(17);
}

void __attribute__ ((interrupt)) LED_Handler(void)
{
	ARCH_ISR_HANDLER(18);
}

void __attribute__ ((interrupt)) Anti_Tamper_Handler(void)
{
	ARCH_ISR_HANDLER(19);
}

void __attribute__ ((interrupt)) CKMN_Handler(void)
{
	ARCH_ISR_HANDLER(21);
}

void __attribute__ ((interrupt)) I2S0_Handler(void)
{
	ARCH_ISR_HANDLER(24);
}

void __attribute__ ((interrupt)) PHY_MBP_Handler(void)
{
	ARCH_ISR_HANDLER(29);
}

void __attribute__ ((interrupt)) PHY_RIU_Handler(void)
{
	ARCH_ISR_HANDLER(30);
}

void __attribute__ ((interrupt)) MAC_INT_TX_RX_TIMER_Handler(void)
{
	ARCH_ISR_HANDLER(31);
}

void __attribute__ ((interrupt)) MAC_INT_TX_RX_MISC_Handler(void)
{
	ARCH_ISR_HANDLER(32);
}

void __attribute__ ((interrupt)) MAC_INT_RX_TRIGGER_Handler(void)
{
	ARCH_ISR_HANDLER(33);
}

void __attribute__ ((interrupt)) MAC_INT_TX_TRIGGER_Handler(void)
{
	ARCH_ISR_HANDLER(34);
}

void __attribute__ ((interrupt)) MAC_INT_PORT_TRIGGER_Handler(void)
{
	ARCH_ISR_HANDLER(35);
}

void __attribute__ ((interrupt)) MAC_INT_GEN_Handler(void)
{
	ARCH_ISR_HANDLER(36);
}

void __attribute__ ((interrupt)) HSU_Handler(void)
{
	ARCH_ISR_HANDLER(37);
}

void __attribute__ ((interrupt)) INT_MAC_WAKEUP_Handler(void)
{
	ARCH_ISR_HANDLER(38);
}

void __attribute__ ((interrupt)) BTDM_Handler(void)
{
	ARCH_ISR_HANDLER(39);
}

void __attribute__ ((interrupt)) BLE_Handler(void)
{
	ARCH_ISR_HANDLER(40);
}

void __attribute__ ((interrupt)) BT_Handler(void)
{
	ARCH_ISR_HANDLER(41);
}

void __attribute__ ((interrupt)) QSPI0_Handler(void)
{
	ARCH_ISR_HANDLER(42);
}

void __attribute__ ((interrupt)) THREAD_Handler(void)
{
	ARCH_ISR_HANDLER(48);
}

void __attribute__ ((interrupt)) OTP_Handler(void)
{
	ARCH_ISR_HANDLER(50);
}

void __attribute__ ((interrupt)) DPLL_UNLOCK_Handler(void)
{
	ARCH_ISR_HANDLER(51);
}

void __attribute__ ((interrupt)) GPIO_Handler(void)
{
	ARCH_ISR_HANDLER(55);
}

void __attribute__ ((interrupt)) GPIO_NS_Handler(void)
{
	ARCH_ISR_HANDLER(56);
}

void __attribute__ ((interrupt)) ANA_GPIO_Handler(void)
{
	ARCH_ISR_HANDLER(58);
}

void __attribute__ ((interrupt)) ANA_RTC_Handler(void)
{
	ARCH_ISR_HANDLER(59);
}

void __attribute__ ((interrupt)) ABNORMAL_GPIO_Handler(void)
{
	ARCH_ISR_HANDLER(60);
}

void __attribute__ ((interrupt)) ABNORMAL_RTC_Handler(void)
{
	ARCH_ISR_HANDLER(61);
}