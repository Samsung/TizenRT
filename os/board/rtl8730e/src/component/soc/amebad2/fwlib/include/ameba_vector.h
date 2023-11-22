/**
  ******************************************************************************
  * @file    ameba_vector.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the IRQ firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_VECTOR_TABLE_H_
#define _AMEBA_VECTOR_TABLE_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup IRQ
  * @brief IRQ modules
  * @{
  */

/** @addtogroup IRQ
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * IRQ table, please refer to IRQ Exported Constants->IRQn_enum->IRQn
  *
  *****************************************************************************************
  * how to use
  *****************************************************************************************
  *		1. register/unregister IRQ use: InterruptRegister/InterruptUnRegister
  *		2. enable/disable IRQ use: InterruptEn/InterruptDis
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup IRQ_Exported_Types IRQ Exported Types
  * @{
  */
typedef s32 IRQn_Type;
typedef void (*HAL_VECTOR_FUN)(void);
typedef u32(*IRQ_FUN)(void *Data);
typedef void (*Fault_Patch)(uint32_t *MSP, uint32_t *PSP, uint32_t lr, uint32_t fault_id);
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup IRQ_Exported_Constants IRQ Exported Constants
  * @{
  */
#if defined (ARM_CORE_CM4)
/** @defgroup IRQn_enum
  * @{
  */
enum IRQn {
	/******  Processor Exceptions Numbers ********/
	NonMaskableInt_IRQn		= -14,	/*!< 2 Non Maskable Interrupt */
	HardFault_IRQn			= -13,	/*!< 3 Hard Fault, all classes of Fault */
	MemoryManagement_IRQn	= -12,	/*!< 4 Cortex-M3 Memory Management Interrupt */
	BusFault_IRQn			= -11,	/*!< 5 Cortex-M3 Bus Fault Interrupt */
	UsageFault_IRQn			= -10,	/*!< 6 Cortex-M3 Usage Fault Interrupt */
	SVCall_IRQn				= -5,	/*!< 11 Cortex-M3 SV Call Interrupt  */
	DebugMonitor_IRQn		= -4,	/*!< 12 Cortex-M3 Debug Monitor Interrupt */
	PendSV_IRQn				= -2,	/*!< 14 Cortex-M3 Pend SV Interrupt */
	SysTick_IRQn			= -1,	/*!< 15 Cortex-M3 System Tick Interrupt */

	/******  Specific Interrupt Numbers ************/
	TIMER0_IRQ			= 0,
	TIMER1_IRQ			= 1,
	TIMER2_IRQ			= 2,
	TIMER3_IRQ			= 3,
	TIMER4_IRQ			= 4,
	TIMER5_IRQ			= 5,
	TIMER6_IRQ			= 6,
	TIMER7_IRQ			= 7,
	UART_LOG_IRQ		= 8,
	GPIOA_IRQ			= 9,
	GPIOB_IRQ			= 10,
	GPIOC_IRQ			= 11,
	RTC_IRQ				= 12,
	CTOUCH_IRQ			= 13,
	ADC_IRQ				= 14,
	ADC_COMP_IRQ		= 15,
	BOR_IRQ				= 16,
	PWR_DOWN_IRQ		= 17,
	VADBT_OR_VADPC_IRQ	= 18,
	SPI_FLASH_IRQ		= 19,
	AP_WAKE_IRQ_LP_IRQ	= 20,
	THERMAL_IRQ			= 21,
	I2C0_IRQ			= 22,
	RSIP_IRQ			= 23,
	IPC_NP_IRQ			= 24,
	GDMA0_CHANNEL0_IRQ	= 25,
	GDMA0_CHANNEL1_IRQ	= 26,
	GDMA0_CHANNEL2_IRQ	= 27,
	GDMA0_CHANNEL3_IRQ	= 28,
	GDMA0_CHANNEL4_IRQ	= 29,
	GDMA0_CHANNEL5_IRQ	= 30,
	GDMA0_CHANNEL6_IRQ	= 31,
	GDMA0_CHANNEL7_IRQ	= 32,
	LCDC_IRQ			= 33,
	WL_DMA_IRQ			= 34,
	WL_PROTOCOL_IRQ		= 35,
	AES_IRQ				= 36,
	SHA_IRQ				= 37,
	SDIO_HOST_IRQ		= 38,
	USB_OTG_IRQ			= 39,
	SPI0_IRQ			= 40,
	SPI1_IRQ			= 41,
	RSVD_IRQ			= 42,
	RSA_IRQ				= 43,
	ECDSA_IRQ			= 44,
	ED25519_IRQ			= 45,
	PSRAMC_IRQ			= 46,
	DDR_IRQ				= 47,
	RXI300_HP_IRQ		= 48,
	IR_IRQ				= 49,
	UART0_IRQ			= 50,
	UART1_IRQ			= 51,
	UART2_IRQ			= 52,
	UART3_BT_IRQ		= 53,
	TRNG_IRQ			= 54,
	I2C1_IRQ			= 55,
	I2C2_IRQ			= 56,
	TIMER8_IRQ			= 57,
	TIMER9_IRQ			= 58,
	WDG1_IRQ			= 59,
	WDG2_IRQ			= 60,
	WDG3_IRQ			= 61,
	WDG4_IRQ			= 62,
	SPORT0_IRQ			= 63,
	SPORT1_IRQ			= 64,
	SPORT2_IRQ			= 65,
	SPORT3_IRQ			= 66,
	BT_SCB_IRQ			= 67,
	LEDC_IRQ			= 68,
	MIPI_DSI_IRQ		= 69,
	AES_S_IRQ			= 70,
	SHA_S_IRQ			= 71,
	AON_WAKEPIN_IRQ		= 72,
	ZIGBEE_IRQ			= 73,
	BT_WAKE_HOST_IRQ	= 74,
	TIMER10_IRQ			= 75,
	TIMER11_IRQ			= 76,
	TIMER12_IRQ			= 77,
	TIMER13_IRQ			= 78,
	AON_TIM_IRQ			= 79,
};
/**
  * @}
  */
#endif
#if defined (ARM_CORE_CM0)

/** @defgroup LPIRQn_enum
  * @{
  */
enum LPIRQn {
	/******  Processor Exceptions Numbers ********/
	NonMaskableInt_IRQn		= -14,	/*!< 2 Non Maskable Interrupt */
	HardFault_IRQn			= -13,	/*!< 3 Hard Fault, all classes of Fault */
	MemoryManagement_IRQn	= -12,	/*!< 4 Cortex-M3 Memory Management Interrupt */
	BusFault_IRQn			= -11,	/*!< 5 Cortex-M3 Bus Fault Interrupt */
	UsageFault_IRQn			= -10,	/*!< 6 Cortex-M3 Usage Fault Interrupt */
	SVCall_IRQn				= -5,	/*!< 11 Cortex-M3 SV Call Interrupt  */
	DebugMonitor_IRQn		= -4,	/*!< 12 Cortex-M3 Debug Monitor Interrupt */
	PendSV_IRQn				= -2,	/*!< 14 Cortex-M3 Pend SV Interrupt */
	SysTick_IRQn			= -1,	/*!< 15 Cortex-M3 System Tick Interrupt */

	/******  Specific Interrupt Numbers ************/
	IPC_IRQ					= 0,
	RXI300_IRQ				= 1,
	WIFI_FISR_FESR_IRQ		= 2,
	WIFI_FTSR_MAILBOX_IRQ	= 3,
	AON_TIM_IRQ				= 4,
	NP_WAKE_IRQ				= 5,
	AP_WAKE_IRQ				= 6,
	WDG0_IRQ				= 7,
	GDMA0_CHANNEL2_IRQ		= 8,
	GDMA0_CHANNEL3_IRQ		= 9,
	TIMER2_IRQ				= 10,
	TIMER3_IRQ				= 11,
	TIMER4_IRQ				= 12,
	TIMER5_IRQ				= 13,
	TIMER6_IRQ				= 14,
	TIMER7_IRQ				= 15,
	UART_LOG_IRQ			= 16,
	GPIOA_IRQ				= 17,
	GPIOB_IRQ				= 18,
	GPIOC_IRQ				= 19,
	RTC_IRQ					= 20,
	CTOUCH_IRQ				= 21,
	ADC_IRQ					= 22,
	ADC_COMP_IRQ			= 23,
	BOR_IRQ					= 24,
	PWR_DOWN_IRQ			= 25,
	VADBT_OR_VADPC_IRQ		= 26,
	SPI_FLASH_IRQ			= 27,
	DEBUG_TIMER_IRQ			= 28,
	THERMAL_IRQ				= 29,
	I2C0_IRQ				= 30,
	RSIP_IRQ				= 31,
	AON_WAKEPIN_IRQ			= 32,
	BT_WAKE_HOST_IRQ		= 33,
	NFIQOUT0_OR_NIRQOUT0_IRQ	= 34,
	NFIQOUT1_OR_NIRQOUT1_IRQ	= 35,
	WL_DMA_IRQ				= 36,
	WL_PROTOCOL_IRQ			= 37,
	TIMER10_IRQ				= 38,
	TIMER11_IRQ				= 39,
	TIMER12_IRQ				= 40,
	TIMER13_IRQ				= 41,
	I2C1_IRQ				= 42,
	I2C2_IRQ				= 43,

};
/**
  * @}
  */
#endif
#if defined (ARM_CORE_CA32)
/** @defgroup APIRQn_enum
  * @{
  */
enum APIRQn {
	/******  Processor Exceptions Numbers ********/
	SGI0_Edge	=	-32,	/* software generated interrupt 0 (recommended non-secure) */
	SGI1_Edge	=	-31,	/* software generated interrupt 1 (recommended non-secure) */
	SGI2_Edge	=	-30,	/* software generated interrupt 2 (recommended non-secure) */
	SGI3_Edge	=	-29,	/* software generated interrupt 3 (recommended non-secure) */
	SGI4_Edge	=	-28,	/* software generated interrupt 4 (recommended non-secure) */
	SGI5_Edge	=	-27,	/* software generated interrupt 5 (recommended non-secure) */
	SGI6_Edge	=	-26,	/* software generated interrupt 6 (recommended non-secure) */
	SGI7_Edge	=	-25,	/* software generated interrupt 7 (recommended non-secure) */
	SGI8_Edge	=	-24,	/* software generated interrupt 8 (recommended secure) */
	SGI9_Edge	=	-23,	/* software generated interrupt 9 (recommended secure) */
	SGI10_Edge	=	-22,	/* software generated interrupt 10 (recommended secure) */
	SGI11_Edge	=	-21,	/* software generated interrupt 11 (recommended secure) */
	SGI12_Edge	=	-20,	/* software generated interrupt 12 (recommended secure) */
	SGI13_Edge	=	-19,	/* software generated interrupt 13 (recommended secure) */
	SGI14_Edge	=	-18,	/* software generated interrupt 14 (recommended secure) */
	SGI15_Edge	=	-17,	/* software generated interrupt 15 (recommended secure) */

	PPI6_IRQ	=	-7	,	/* Virtual maintenance interrupt. */
	PPI5_IRQ	=	-6	,	/* Hypervisor timer event. */
	PPI4_IRQ	=	-5	,	/* Virtual timer event. */
	PPI0_IRQ	=	-4	,	/* Legacy nFIQ signal. Not used. */
	PPI1_IRQ	=	-3	,	/* Secure physical timer event. */
	PPI2_IRQ	=	-2	,	/* Non-secure physical timer event. */
	PPI3_IRQ	=	-1	,	/* Legacy nIRQ signal. Not used. */

	/******  Specific Interrupt Numbers ************/
	AON_TIM_IRQ			= 95,
	TIMER0_IRQ			= 0,
	TIMER1_IRQ			= 1,
	TIMER2_IRQ			= 2,
	TIMER3_IRQ			= 3,
	TIMER4_IRQ			= 4,
	TIMER5_IRQ			= 5,
	TIMER6_IRQ			= 6,
	TIMER7_IRQ			= 7,
	UART_LOG_IRQ		= 8,
	GPIOA_IRQ			= 9,
	GPIOB_IRQ			= 10,
	GPIOC_IRQ			= 11,
	RTC_IRQ				= 12,
	CTOUCH_IRQ			= 13,
	ADC_IRQ				= 14,
	ADC_COMP_IRQ		= 15,
	BOR_IRQ				= 16,
	PWR_DOWN_IRQ		= 17,
	VADBT_OR_VADPC_IRQ	= 18,
	SPI_FLASH_IRQ		= 19,
	DEBUG_TIMER_IRQ		= 20,
	THERMAL_IRQ			= 21,
	I2C0_IRQ			= 22,
	RSIP_IRQ			= 23,
	IPC_AP_IRQ			= 24,
	GDMA0_CHANNEL0_IRQ	= 25,
	GDMA0_CHANNEL1_IRQ	= 26,
	GDMA0_CHANNEL2_IRQ	= 27,
	GDMA0_CHANNEL3_IRQ	= 28,
	GDMA0_CHANNEL4_IRQ	= 29,
	GDMA0_CHANNEL5_IRQ	= 30,
	GDMA0_CHANNEL6_IRQ	= 31,
	GDMA0_CHANNEL7_IRQ	= 32,
	LCDC_IRQ			= 33,
	WL_DMA_IRQ			= 34,
	WL_PROTOCOL_IRQ		= 35,
	AES_IRQ				= 36,
	SHA_IRQ				= 37,
	SDIO_HOST_IRQ		= 38,
	USB_OTG_IRQ			= 39,
	SPI0_IRQ			= 40,
	SPI1_IRQ			= 41,
	RSVD_IRQ			= 42,
	RSA_IRQ				= 43,
	ECDSA_IRQ			= 44,
	ED25519_IRQ			= 45,
	PSRAMC_IRQ			= 46,
	DDR_IRQ				= 47,
	RXI300_HP_IRQ		= 48,
	IR_IRQ				= 49,
	UART0_IRQ			= 50,
	UART1_IRQ			= 51,
	UART2_IRQ			= 52,
	UART3_BT_IRQ		= 53,
	TRNG_IRQ			= 54,
	I2C1_IRQ			= 55,
	I2C2_IRQ			= 56,
	TIMER8_IRQ			= 57,
	TIMER9_IRQ			= 58,
	WDG1_IRQ			= 59,
	WDG2_IRQ			= 60,
	WDG3_IRQ			= 61,
	WDG4_IRQ			= 62,
	SPORT0_IRQ			= 63,
	SPORT1_IRQ			= 64,
	SPORT2_IRQ			= 65,
	SPORT3_IRQ			= 66,
	BT_SCB_IRQ			= 67,
	LEDC_IRQ			= 68,
	PMUIRQ0_IRQ			= 69,
	MIPI_DSI_IRQ		= 70,
	AXIERIRQ_IRQ		= 71,
	AES_S_IRQ			= 72,
	SHA_S_IRQ			= 73,
	AON_WAKEPIN_IRQ		= 74,
	ZIGBEE_IRQ			= 75,
	BT_WAKE_HOST_IRQ	= 76,
	CTIIRQ_IRQ			= 77,
	TIMER10_IRQ			= 78,
	TIMER11_IRQ			= 79,
	TIMER12_IRQ			= 80,
	TIMER13_IRQ			= 81,
};
/**
  * @}
  */
#endif

/** @defgroup IRQn_Priority_def
  * @{
  */
#ifdef ARM_CORE_CM4
enum IRQnPriority {
	INT_PRI0 = 0,				//KM4 Priority value 0 is the highest active priority
	INT_PRI_HIGHEST	= INT_PRI0,
	INT_PRI1,					//lower numbers have higher priority in KM4
	INT_PRI2,
	INT_PRI_HIGH 	= INT_PRI2,
	INT_PRI3,
	INT_PRI4,
	INT_PRI_MIDDLE	= INT_PRI4,
	INT_PRI5,
	INT_PRI6,
	INT_PRI7,
	INT_PRI_LOWEST	= INT_PRI7,
};
#elif defined (ARM_CORE_CM0)
enum IRQnPriority {
	INT_PRI0 = 0,
	INT_PRI_HIGHEST = INT_PRI0,		//KM0 Priority
	INT_PRI1,
	INT_PRI_HIGH	= INT_PRI1,
	INT_PRI2,
	INT_PRI_MIDDLE	= INT_PRI2,
	INT_PRI3,
	INT_PRI_LOWEST	= INT_PRI3,		//KM0 only have 4 priority level
};
#elif defined (ARM_CORE_CA32)
enum NS_IRQnPriority {
	INT_PRI0 = 0,
	INT_PRI_HIGHEST = INT_PRI0,		//CA32 Priority in NS World, lower numbers have higher priority
	INT_PRI1 = 0x10,				//valid bit in GICD_IPRIORITYRn is [7:3], But NS world can only use [7:4] for each field
	INT_PRI2 = 0x20,
	INT_PRI3 = 0x30,
	INT_PRI4 = 0x40,
	INT_PRI5 = 0x50,
	INT_PRI_HIGH	= INT_PRI5,
	INT_PRI6 = 0x60,
	INT_PRI7 = 0x70,
	INT_PRI8 = 0x80,
	INT_PRI9 = 0x90,
	INT_PRI10 = 0xA0,
	INT_PRI_MIDDLE	= INT_PRI10,
	INT_PRI11 = 0xB0,
	INT_PRI12 = 0xC0,
	INT_PRI13 = 0xD0,
	INT_PRI14 = 0xE0,
	INT_PRI15 = 0xF0,
	//INT_PRI15 shall not be used for The GIC always masks an interrupt that has the largest supported priority field value.
	INT_PRI_LOWEST	= INT_PRI14,
};
#endif
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup IRQ_Exported_Functions IRQ Exported Functions
  * @{
  */
extern _LONG_CALL_ void irq_table_init(u32 StackP);
extern _LONG_CALL_ void irq_enable(IRQn_Type   IrqNum);
extern _LONG_CALL_ void irq_disable(IRQn_Type   IrqNum);
extern _LONG_CALL_ void irq_set_priority(IRQn_Type irqn, uint32_t priority);
extern _LONG_CALL_ uint32_t irq_get_priority(IRQn_Type irqn);
extern _LONG_CALL_ void irq_set_pending(IRQn_Type irqn);
extern _LONG_CALL_ uint32_t irq_get_pending(IRQn_Type irqn);
extern _LONG_CALL_ void irq_clear_pending(IRQn_Type irqn);
extern _LONG_CALL_ BOOL irq_register(IRQ_FUN IrqFun, IRQn_Type IrqNum, u32 Data, u32 Priority);
extern _LONG_CALL_ BOOL irq_unregister(IRQn_Type IrqNum);
extern _LONG_CALL_ BOOL FaultPatch_register(Fault_Patch PatchFun);

#ifdef ARM_CORE_CA32
#define InterruptRegister_edge		irq_register_edge
#endif
#define InterruptRegister			irq_register
#define InterruptUnRegister		irq_unregister

#define InterruptEn(a,b)			irq_enable(a)
#define InterruptDis(a)			irq_disable(a)
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Other Definitions --------------------------------------------------------*/
extern IRQ_FUN UserIrqFunTable[];
extern u32 UserIrqDataTable[];
extern HAL_VECTOR_FUN  NewVectorTable[];

#if defined (ARM_CORE_CM4)
#define MAX_VECTOR_TABLE_NUM			(80+16)
#define MAX_PERIPHERAL_IRQ_NUM			80
#define MAX_IRQ_PRIORITY_VALUE			7
#else
#define MAX_VECTOR_TABLE_NUM			(16+44)
#define MAX_PERIPHERAL_IRQ_NUM			44
#define MAX_NVIC_IPR_NUM				((MAX_PERIPHERAL_IRQ_NUM + 3) >> 2)
#define MAX_IRQ_PRIORITY_VALUE			3
#endif

#define MSP_RAM_LP			0x23001FFC
#define MSPLIM_RAM_HP		0x30001000
#define MSP_RAM_HP			0x30002FFC
#define MSP_RAM_HP_NS		0x2001BFFC

#define BIT_EXCRETURN_S			BIT(6)	/* Indicates whether a Secure or Non-secure stack is used to restore stack frame on exception return. 0: Non-secure, 1: Secure*/
#define BIT_EXCRETURN_MODE		BIT(3)	/* Indicates the Mode that was stacked from. 0: Handler mode, 1: Thread mode */
#define BIT_EXCRETURN_SPSEL		BIT(2)	/* Stack pointer selection. The value of this bit indicates the transitory value of the CONTROL.SPSEL bit
											associated with the Security state of the exception as indicated by EXC_RETURN.ES. 0: MSP, 1: PSP*/
#define BIT_EXCRETURN_ES		BIT(0)	/* The security domain the exception was taken to. 0: Non-secure, 1: Secure */

#define BIT_CONTROL_SPSEL		BIT(1)	/* Stack-pointer select. Defines the stack pointer to be used. 0: use MSP, 1: in thread mode, use PSP */

#endif //_AMEBA_VECTOR_TABLE_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
