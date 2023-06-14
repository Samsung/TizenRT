/**
  ******************************************************************************
  * @file    rtl8721d_gpio_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the General Purpose Input Output (GPIO) peripheral:
  *           - Initialization and Configuration
  *           - GPIO Read and Write
  *           - GPIO Interrupts management
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

HAL_ROM_BSS_SECTION GPIO_IRQ_FUN PortA_IrqHandler[32]; // The interrupt handler triggered by Port A[x]
HAL_ROM_BSS_SECTION GPIO_IRQ_FUN PortB_IrqHandler[32]; // The interrupt handler triggered by Port B[x]
HAL_ROM_BSS_SECTION GPIO_IRQ_FUN PortC_IrqHandler[32]; // The interrupt handler triggered by Port C[x]
HAL_ROM_BSS_SECTION void *PortA_IrqData[32];
HAL_ROM_BSS_SECTION void *PortB_IrqData[32];
HAL_ROM_BSS_SECTION void *PortC_IrqData[32];

HAL_ROM_DATA_SECTION GPIO_TypeDef *GPIO_PORTx[3] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE};

/**
  * @brief  Config GPIO interrupt mode
  * @param  GPIO_Pin: GPIO pin num from PinName.
  * @param  NewState: ENABLE/DISABLE
  * @param  GPIO_ITTrigger: Specifies interrupt mode is level or edge trigger
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_INT_Trigger_LEVEL: level trigger
  *		 		@arg GPIO_INT_Trigger_EDGE: edge trigger
  *		 		@arg GPIO_INT_Trigger_BOTHEDGE: both-edge trigger
  * @param  GPIO_ITPolarity: Specifies interrupt mode is high or low active trigger
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_INT_POLARITY_ACTIVE_LOW: falling edge or low level active
  *		 		@arg GPIO_INT_POLARITY_ACTIVE_HIGH: rising edge or high level active
  * @param  GPIO_ITDebounce: enable or disable de-bounce for interrupt
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_INT_DEBOUNCE_DISABLE: disable de-bounce
  *		 		@arg GPIO_INT_DEBOUNCE_ENABLE: enable de-bounce
  *
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_INTMode(u32 GPIO_Pin, u32 NewState,
				  u32 GPIO_ITTrigger,
				  u32 GPIO_ITPolarity,
				  u32 GPIO_ITDebounce)
{
	GPIO_TypeDef *GPIO = NULL;
	u8 port_num;
	u8 pin_num;
	u32 RegValue;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);
	GPIO = GPIO_PORTx[port_num];

	if (NewState == ENABLE) {

		/* Config Level or Edge trigger */
		if (GPIO_ITTrigger != GPIO_INT_Trigger_BOTHEDGE) {
			/* close both-edge, because both-edge is high priority */
			RegValue = GPIO->GPIO_INT_BOTHEDGE;
			RegValue &= ~(1 << pin_num);
			GPIO->GPIO_INT_BOTHEDGE = RegValue;

			/* open level or single-edge */
			RegValue = GPIO->GPIO_INT_TYPE;
			RegValue &= ~(1 << pin_num);
			RegValue |= (GPIO_ITTrigger << pin_num);
			GPIO->GPIO_INT_TYPE = RegValue;

			/* Config Low active or Gigh active */
			RegValue = GPIO->GPIO_INT_POLARITY;
			RegValue &= ~(1 << pin_num);
			RegValue |= (GPIO_ITPolarity << pin_num);
			GPIO->GPIO_INT_POLARITY = RegValue;
		} else {
			RegValue = GPIO->GPIO_INT_BOTHEDGE;
			RegValue |= (1 << pin_num);
			GPIO->GPIO_INT_BOTHEDGE = RegValue;
		}

		/* 0 is close interrupt debounce */
		RegValue = GPIO->GPIO_DEBOUNCE;
		RegValue &= ~(1 << pin_num);
		if (GPIO_ITDebounce == GPIO_INT_DEBOUNCE_ENABLE) {
			RegValue |= (0x01 << pin_num);
		}
		GPIO->GPIO_DEBOUNCE = RegValue;
	}

	/* set interrupt mode enable register */
	RegValue = GPIO->GPIO_INT_EN;
	if (NewState == ENABLE) {
		RegValue |= (0x01 << pin_num); //int mode
	} else {
		RegValue &= ~(0x01 << pin_num); //normal gpio mode
	}
	GPIO->GPIO_INT_EN = RegValue;
}

/**
  * @brief  Disable or Enable the interrupt of a specified pin
  * @param  GPIO_Pin: GPIO pin num from PinName.
  * @param  NewState: ENABLE/DISABLE
  *
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_INTConfig(u32 GPIO_Pin, u32 NewState)
{
	GPIO_TypeDef *GPIO = NULL;
	u8 port_num;
	u8 pin_num;
	u32 RegValue;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);
	GPIO = GPIO_PORTx[port_num];

	/* Clear pending interrupt */
	GPIO->GPIO_INT_EOI = (1 << pin_num);

	/* set interrupt mask register */
	RegValue =  GPIO->GPIO_INT_MASK;
	if (NewState == DISABLE) {
		RegValue |= (0x01 << pin_num); //mask irq
	} else {
		RegValue &= ~(0x01 << pin_num);
	}
	GPIO->GPIO_INT_MASK = RegValue;
}

/**
  * @brief  The common GPIO interrupt handler
  * @param  pData: the data pointer from the registered IRQ handler
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
u32 GPIO_INTHandler(IN VOID *pData)
{
	GPIO_TypeDef *GPIO = (GPIO_TypeDef *)pData;
	u32 IrqStatus;
	u32 i;
	u32 port_a_in;
	u32 event;

	port_a_in =  GPIO->GPIO_EXT_PORT[0];
	IrqStatus = GPIO->GPIO_INT_STATUS;

	/* Clear pending edge interrupt */
	GPIO->GPIO_INT_EOI = IrqStatus;

	if (GPIO == GPIOA_BASE) {
		for (i = 0; i < 32; i++) {
			if (IrqStatus & (0x1UL << i)) {
				if (PortA_IrqHandler[i] != NULL) {
					if (port_a_in & (0x1UL << i)) {
						event = HAL_IRQ_RISE | (i << 16); /* (event | port_pin << 16 */
					} else {
						event = HAL_IRQ_FALL | (i << 16); /* (event | port_pin << 16 */
					}
					PortA_IrqHandler[i](PortA_IrqData[i], event);
				}
			}
		}
	} else if (GPIO == GPIOB_BASE) {
		for (i = 0; i < 32; i++) {
			if (IrqStatus & (0x1UL << i)) {
				if (PortB_IrqHandler[i] != NULL) {
					if (port_a_in & (0x1UL << i)) {
						event = HAL_IRQ_RISE | (i << 16) | (0x1UL << 21); /* (event | port_pin << 16 */
					} else {
						event = HAL_IRQ_FALL | (i << 16) | (0x1UL << 21); /* (event | port_pin << 16 */
					}
					PortB_IrqHandler[i](PortB_IrqData[i], event);
				}
			}
		}
	} else {
		for (i = 0; i < 32; i++) {
			if (IrqStatus & (0x1UL << i)) {
				if (PortC_IrqHandler[i] != NULL) {
					if (port_a_in & (0x1UL << i)) {
						event = HAL_IRQ_RISE | (i << 16) | (0x2UL << 21); /* (event | port_pin << 16 */
					} else {
						event = HAL_IRQ_FALL | (i << 16) | (0x2UL << 21); /* (event | port_pin << 16 */
					}
					PortC_IrqHandler[i](PortC_IrqData[i], event);
				}
			}
		}
	}

	return 0;
}

/**
  * @brief  Set the specified GPIO pin data Direction.
  * @param  GPIO_Pin: GPIO pin num from PinName.
  * @param  data_direction: Specifies GPIO pin direction
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_Mode_IN: GPIO pin direction config input mode
  *		 		@arg GPIO_Mode_OUT: GPIO pin direction config output mode
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_Direction(u32 GPIO_Pin, u32 data_direction)
{
	GPIO_TypeDef *GPIO = NULL;
	u8 port_num;
	u8 pin_num;
	u32 RegValue;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);
	GPIO = GPIO_PORTx[port_num];

	/* data direction, 0 is IN */
	if (data_direction == GPIO_Mode_IN) {
		RegValue =  GPIO->PORT[0].GPIO_DDR;
		RegValue &= ~BIT(pin_num);
		GPIO->PORT[0].GPIO_DDR = RegValue;
	} else {
		RegValue =  GPIO->PORT[0].GPIO_DDR;
		RegValue |= BIT(pin_num);
		GPIO->PORT[0].GPIO_DDR = RegValue;
	}
}

/**
  * @brief  Initializes the GPIO registers according to the specified parameters
  *         in GPIO_InitStruct.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the GPIO peripheral.
  * @note   OpenDrain output: on drive + OUT + GPIO[gpio_bit] = 0, pin should have pull-up resistor
  * @note   Input HighZ: no drive + IN, user can input high or low use this pin
  *
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_Init(GPIO_InitTypeDef  *GPIO_InitStruct)
{
	/* open gpio function and clock */
	//RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

	assert_param(GPIO_InitStruct->GPIO_Mode <= GPIO_Mode_INT);

	/* GPIO Pad shouddown control: Turn on the corresponding GPIO Pad and pinmux to GPIO */
	Pinmux_Config(GPIO_InitStruct->GPIO_Pin, PINMUX_FUNCTION_GPIO);

	if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_INT) {
		GPIO_Direction(GPIO_InitStruct->GPIO_Pin, GPIO_Mode_IN);

		GPIO_INTMode(GPIO_InitStruct->GPIO_Pin, ENABLE, GPIO_InitStruct->GPIO_ITTrigger,
					 GPIO_InitStruct->GPIO_ITPolarity, GPIO_InitStruct->GPIO_ITDebounce);
	} else {
		GPIO_INTMode(GPIO_InitStruct->GPIO_Pin, DISABLE, 0, 0, 0);

		if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) {
			GPIO_Direction(GPIO_InitStruct->GPIO_Pin, GPIO_Mode_OUT);
		} else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IN) {
			GPIO_Direction(GPIO_InitStruct->GPIO_Pin, GPIO_Mode_IN);
			PAD_PullCtrl(GPIO_InitStruct->GPIO_Pin, GPIO_InitStruct->GPIO_PuPd);
		}
	}
}

/**
  * @brief  De-Initializes a GPIO Pin, reset it as default setting.
  * @param  GPIO_Pin: GPIO pin num from PinName.
  *
  *
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_DeInit(u32 GPIO_Pin)
{
	GPIO_TypeDef *GPIO = NULL;
	u8 port_num;
	u8 pin_num;
	u32 RegValue;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);
	GPIO = GPIO_PORTx[port_num];

	/* config as IN & HIGHZ*/
	RegValue =  GPIO->PORT[0].GPIO_DDR;
	RegValue &= ~(1 << pin_num);
	GPIO->PORT[0].GPIO_DDR = RegValue;
	PAD_PullCtrl(GPIO_Pin, GPIO_PuPd_NOPULL);

	/* Disable Interrupt */
	RegValue =  GPIO->GPIO_INT_EN;
	RegValue &= ~(1 << pin_num);
	GPIO->GPIO_INT_EN = RegValue;

	/* Clear pending interrupt */
	GPIO->GPIO_INT_EOI = (1 << pin_num);

	/* UnRegister IRQ */
	if (port_num == GPIO_PORT_A) {
		PortA_IrqHandler[pin_num] = (GPIO_IRQ_FUN)NULL;
		PortA_IrqData[pin_num] = NULL;
	} else if (port_num == GPIO_PORT_B) {
		PortB_IrqHandler[pin_num] = (GPIO_IRQ_FUN)NULL;
		PortB_IrqData[pin_num] = NULL;
	} else {
		PortC_IrqHandler[pin_num] = (GPIO_IRQ_FUN)NULL;
		PortC_IrqData[pin_num] = NULL;
	}
}

/**
  * @brief  Read a specified GPIO pin Value
  * @param  GPIO_Pin: GPIO pin num from PinName.
  * @retval The input port pin current status(High or Low).
  */
__weak HAL_ROM_TEXT_SECTION
u32 GPIO_ReadDataBit(u32 GPIO_Pin)
{
	GPIO_TypeDef *GPIO = NULL;
	u8 port_num;
	u8 pin_num;
	u32 RegValue;
	u32 pin_status;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);
	GPIO = GPIO_PORTx[port_num];

	RegValue =  GPIO->GPIO_EXT_PORT[0];
	if (RegValue & (1 << pin_num)) {
		pin_status = GPIO_PIN_HIGH;
	} else {
		pin_status = GPIO_PIN_LOW;
	}

	return pin_status;

}

/**
  * @brief  Write a specified output port pin.
  * @param  GPIO_Pin: GPIO pin num from PinName.
  * @param  Pin_State: The state going to be set to the assigned GPIO pin.
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_PIN_LOW: Pin state set to low
  *		 		@arg GPIO_PIN_HIGH: Pin state set to high
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_WriteBit(u32 GPIO_Pin, u32 Pin_State)
{
	GPIO_TypeDef *GPIO = NULL;
	u8 port_num;
	u8 pin_num;
	u32 RegValue;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);
	GPIO = GPIO_PORTx[port_num];

	RegValue =  GPIO->PORT[0].GPIO_DR;
	if (Pin_State == GPIO_PIN_LOW) {
		RegValue &= ~BIT(pin_num);
	} else {
		RegValue |= BIT(pin_num);
	}
	GPIO->PORT[0].GPIO_DR = RegValue;
}

/**
  * @brief  set the specified GPIO port pins data direction.
  * @param  GPIO_Port: Specifies port number
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_PORT_A: port number is A
  *		 		@arg GPIO_PORT_B: port number is B
  * @param  GPIO_Mask: One bit one GPIO pin, PortA/B:0x00000000 ~0xFFFFFFFF.
  * @param  data_direction: Specifies GPIO port pins direction
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_Mode_IN: GPIO port pins direction config input mode
  *		 		@arg GPIO_Mode_OUT: GPIO port pins direction config output mode
  * @note   It can config multiple pins of one port by parameter GPIO_Mask
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_PortDirection(u32 GPIO_Port, u32 GPIO_Mask, u32 data_direction)
{
	GPIO_TypeDef *GPIO = NULL;
	u32 RegValue;

	assert_param(IS_GPIO_PORT_NUM(GPIO_Port));

	GPIO = GPIO_PORTx[GPIO_Port];

	/* data direction, 0 is IN */
	RegValue =  GPIO->PORT[0].GPIO_DDR;

	if (data_direction == GPIO_Mode_IN) {
		RegValue &= ~GPIO_Mask;
	} else {
		RegValue |= GPIO_Mask;
	}

	GPIO->PORT[0].GPIO_DDR = RegValue;
}

/**
  * @brief  Reads the specified GPIO pins.
  * @param  GPIO_Port: Specifies port number
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_PORT_A: port number is A
  *		 		@arg GPIO_PORT_B: port number is B
  * @param  GPIO_Mask: One bit one GPIO pin, PortA/B:0x00000000 ~0xFFFFFFFF.
  * @note   It can config multiple pins of one port by parameter GPIO_Mask
  * @retval The value of the specified port pins
  */
__weak HAL_ROM_TEXT_SECTION
u32 GPIO_PortRead(u32 GPIO_Port, u32 GPIO_Mask)
{
	GPIO_TypeDef *GPIO = NULL;
	u32 RegValue;

	assert_param(IS_GPIO_PORT_NUM(GPIO_Port));

	GPIO = GPIO_PORTx[GPIO_Port];

	RegValue =  GPIO->GPIO_EXT_PORT[0];

	RegValue &= GPIO_Mask;

	return RegValue;
}

/**
  * @brief  Write the specified port pins.
  * @param  GPIO_Port: Specifies port number
  *          This parameter can be one of the following values:
  *		 		@arg GPIO_PORT_A: port number is A
  *		 		@arg GPIO_PORT_B: port number is B
  * @param	GPIO_Mask: One bit one GPIO pin, PortA/B:0x00000000 ~0xFFFFFFFF.
  * @param  Port_State: Specifies the state going to be set to the assigned GPIO pins.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_PortWrite(u32 GPIO_Port, u32 GPIO_Mask, u32 Port_State)
{
	GPIO_TypeDef *GPIO = NULL;
	u32 RegValue;

	assert_param(IS_GPIO_PORT_NUM(GPIO_Port));

	GPIO = GPIO_PORTx[GPIO_Port];

	RegValue =  GPIO->PORT[0].GPIO_DR;

	/* clear the mask bits */
	RegValue &= ~GPIO_Mask;

	/* set mask bits */
	RegValue |= (Port_State & GPIO_Mask);

	GPIO->PORT[0].GPIO_DR = RegValue;
}

/**
  * @brief  Set the specified port debounce clock.
  * @param  GPIO_Port: Specifies port number
  *          This parameter can be one of the following values:
  *				@arg GPIO_PORT_A: port number is A
  *				@arg GPIO_PORT_B: port number is B
  *				@arg GPIO_PORT_C: port number is C
  * @param	DivideCount: debounce clock division with 32KHz.range: 0x0 - 0xFF.
  *			de-bounce time = (DivideCount +1) * 2*32μs.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_DebounceClock(u32 GPIO_Port, u32 DivideCount)
{
	GPIO_TypeDef *GPIO = NULL;

	assert_param(IS_GPIO_PORT_NUM(GPIO_Port));

	GPIO = GPIO_PORTx[GPIO_Port];

	GPIO->GPIO_DB_DIV_CONFIG = GPIO_EXT_DB_DIV_CNT(DivideCount);
}

/**
  * @brief  Set the level sensitive interrupt synchronized to pclk_intr.
  * @param  GPIO_Port: Specifies port number
  *          This parameter can be one of the following values:
  *				@arg GPIO_PORT_A: port number is A
  *				@arg GPIO_PORT_B: port number is B
  *				@arg GPIO_PORT_C: port number is C
  * @param	NewState: all level-sensitive interrupts being synchronized to pclk_intr
  *          This parameter can be one of the folling values:
  *				@ENABLE: Synchronize to pclk_intr with 2 cycles
  *				@DISABLE: No synchronization to pclk_intr (default)
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_LevelSync(u32 GPIO_Port, u32 NewState)
{
	GPIO_TypeDef *GPIO = NULL;

	assert_param(IS_GPIO_PORT_NUM(GPIO_Port));

	GPIO = GPIO_PORTx[GPIO_Port];

	if (NewState == ENABLE) {
		GPIO->GPIO_ITN_LS_SYNC = GPIO_BIT_LEVEL_SYNC;
	} else {
		GPIO->GPIO_ITN_LS_SYNC = 0;
	}
}

/**
  * @brief  To register a user interrupt handler for a specified pin
  * @param  GPIO_Pin: GPIO pin num from PinName.
  * @param  IrqHandler: The IRQ handler to be assigned to the specified pin
  * @param  IrqData: The pointer will be pass the the IRQ handler
  *
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void GPIO_UserRegIrq(u32 GPIO_Pin, VOID *IrqHandler, VOID *IrqData)
{
	u8 port_num;
	u8 pin_num;

	port_num = PORT_NUM(GPIO_Pin);
	pin_num = PIN_NUM(GPIO_Pin);

	if (port_num == GPIO_PORT_A) {
		PortA_IrqHandler[pin_num] = (GPIO_IRQ_FUN)IrqHandler;
		PortA_IrqData[pin_num] = IrqData;
	} else if (port_num == GPIO_PORT_B) {
		PortB_IrqHandler[pin_num] = (GPIO_IRQ_FUN)IrqHandler;
		PortB_IrqData[pin_num] = IrqData;
	} else {
		PortC_IrqHandler[pin_num] = (GPIO_IRQ_FUN)IrqHandler;
		PortC_IrqData[pin_num] = IrqData;
	}
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
