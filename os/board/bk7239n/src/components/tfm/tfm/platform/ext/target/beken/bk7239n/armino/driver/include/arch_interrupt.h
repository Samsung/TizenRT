// Copyright 2020-2021 Beken
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

#pragma once

#include <common/bk_include.h>
#include <common/bk_err.h>
#include <driver/hal/hal_int_types.h>

/*Define INT ID */
typedef enum {
	INT_ID_DMA0_NSEC          = 0,
	INT_ID_ENC_SEC               ,
	INT_ID_ENC_NSEC              ,
	INT_ID_TIMER0                ,
	INT_ID_UART0                 ,
	INT_ID_PWM0                  ,
	INT_ID_I2C0                  ,
	INT_ID_SPI0                  ,
	INT_ID_SARADC                ,
	INT_ID_IRDA                  ,
	INT_ID_SDIO                  ,
	INT_ID_GDMA                  ,
	INT_ID_LA                    ,
	INT_ID_TIMER1                ,
	INT_ID_I2C1                  ,
	INT_ID_UART1                 ,
	INT_ID_UART2                 ,
	INT_ID_SPI1                  ,
	INT_ID_CAN                   ,
	INT_ID_USB                   ,
	INT_ID_QSPI0                 ,
	INT_ID_FFT                   ,
	INT_ID_SBC                   ,
	INT_ID_AUD                   ,
	INT_ID_I2S0                  ,
	INT_ID_JPEGENC               ,
	INT_ID_JPEGDEC               ,
	INT_ID_LCD                   ,
	INT_ID_DMA2D                 ,
	INT_ID_PHY_MBP_INT           ,
	INT_ID_PHY_RIU_INT           ,
	INT_ID_MAC_TX_RX_TIMER_N     ,
	INT_ID_MAC_TX_RX_MISC_N      ,
	INT_ID_MAC_RX_TRIGGER_N      ,
	INT_ID_MAC_TX_TRIGGER_N      ,
	INT_ID_MAC_PORT_TRIGGER_N    ,
	INT_ID_MAC_GEN_N             ,
	INT_ID_HSU_IRQ               ,
	INT_ID_INT_MAC_WAKEUP        ,
	INT_ID_DM                    ,
	INT_ID_BLE                   ,
	INT_ID_BT                    ,
	INT_ID_QSPI1                 ,
	INT_ID_PWM1                  ,
	INT_ID_I2S1                  ,
	INT_ID_I2S2                  ,
	INT_ID_H264                  ,
	INT_ID_SDMADC                ,
	INT_ID_MBOX0                 ,
	INT_ID_MBOX1                 ,
	INT_ID_BMC64                 ,
	INT_ID_DPLL_UNLOCK           ,
	INT_ID_TOUCHED               ,
	INT_ID_USBPLUG               ,
	INT_ID_RTC                   ,
	INT_ID_GPIO                  ,
	INT_ID_DMA1_SEC              ,
	INT_ID_DMA1_NSEC             ,
	INT_ID_YUVB                  ,
	INT_ID_ROTT                  ,
	INT_ID_MAX
} arch_int_src_t;

typedef enum {
	PRI_GROUP_BITS_7_1 = 0,
	PRI_GOURP_BITS_7_2,
	PRI_GOURP_BITS_7_3,
	PRI_GOURP_BITS_7_4,
	PRI_GOURP_BITS_7_5,
	PRI_GOURP_BITS_7_6,
	PRI_GOURP_BITS_7_7,
	PRI_GOURP_BITS_NONE,
} pri_group_t;

#define IRQ_DEFAULT_PRIORITY    (6)

bk_err_t arch_isr_entry_init(void);
void arch_interrupt_unregister_int(arch_int_src_t int_number);
void arch_interrupt_register_int(arch_int_src_t int_number, int_group_isr_t isr_callback);
void arch_interrupt_set_priority(arch_int_src_t int_number, uint32_t int_priority);
void arch_int_enable_irq(arch_int_src_t irq);
void arch_int_disable_irq(arch_int_src_t irq);
uint32_t arch_int_get_enable_irq(arch_int_src_t irq);
void arch_int_set_target_state(arch_int_src_t irq);
void arch_int_clear_target_state(arch_int_src_t irq);
uint32_t arch_int_get_target_state(arch_int_src_t irq);
void arch_int_dump_statis(void);
void arch_int_set_target_state_all(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SecureFault_Handler(void);
void DebugMon_Handler(void);

