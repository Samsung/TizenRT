/** mbed Microcontroller Library
  ******************************************************************************
  * @file    gpio_irq_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for GPIO IRQ.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "objects.h"
#include "gpio_irq_api.h"
#include "gpio_irq_ex_api.h"

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @addtogroup MBED_GPIO
 *  @brief    MBED_GPIO driver modules.
 *  @{
 */

/** @addtogroup MBED_GPIO_Exported_Functions MBED_GPIO Exported Functions
  * @{
  */

/**
  * @brief  Initialize the GPIO device's interrupt mode, including mode, trigger and polarity registers.
  * @param  obj: GPIO irq object defined in application software.
  * @param  pin: PinName according to pinmux spec.
  * @param  handler: Interrupt handler to be assigned to the specified pin.
  * @param  id: Handler parameter.
  * @return 0 if initialization is ok or -1 if error happens.
  */
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	u8 port_num;

	if (pin == NC) {
		return -1;
	}

	obj->pin = pin;

	port_num = PORT_NUM(pin);

	GPIO_INTConfig(pin, DISABLE);

	GPIO_InitStruct.GPIO_Pin = obj->pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_INT;
	GPIO_InitStruct.GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
	GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
	GPIO_InitStruct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_DISABLE;

	GPIO_Init(&GPIO_InitStruct);

	if (port_num == GPIO_PORT_A) {
		InterruptRegister(GPIO_INTHandler, GPIOA_IRQ, (u32)GPIOA_BASE, 5);
		InterruptEn(GPIOA_IRQ, 5);
	} else if (port_num == GPIO_PORT_B) {
		InterruptRegister(GPIO_INTHandler, GPIOB_IRQ, (u32)GPIOB_BASE, 5);
		InterruptEn(GPIOB_IRQ, 5);
	} else if (port_num == GPIO_PORT_C) {
		InterruptRegister(GPIO_INTHandler, GPIOC_IRQ, (u32)GPIOC_BASE, 5);
		InterruptEn(GPIOC_IRQ, 5);
	}

	GPIO_UserRegIrq(GPIO_InitStruct.GPIO_Pin, (VOID *) handler, (VOID *) id);

	return 0;
}

/**
  * @brief  Deinitialize the GPIO device interrupt mode, including mode, trigger and polarity registers.
  * @param  obj: GPIO irq object defined in application software.
  * @retval none
  */
void gpio_irq_free(gpio_irq_t *obj)
{
	GPIO_DeInit(obj->pin);
}

/**
  * @brief  Configure GPIO interrupt as enabled or disabled.
  * @param  obj: GPIO irq object defined in application software.
  * @param  event: GPIO interrupt event, this parameter can be one of the following values:
  *		@arg IRQ_RISE: Rising edge interrupt event.
  *		@arg IRQ_FALL: Falling edge interrupt event.
  *		@arg IRQ_FALL_RISE: Both rising and falling edge interrupt event.
  *		@arg IRQ_LOW: Low level interrupt event.
  *		@arg IRQ_HIGH: High level interrupt event.
  *		@arg IRQ_NONE: No interrupt event.
  * @param  enable: This parameter can be one of the following values:
  *		@arg 0: Disable GPIO interrupt.
  *		@arg 1: Enable GPIO interrupt.
  * @retval none
  */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
	u32 GPIO_ITTrigger = 0;
	u32 GPIO_ITPolarity = 0;

	switch ((uint8_t)event) {
	case IRQ_RISE:
		GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
		GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_HIGH;
		break;

	case IRQ_FALL:
		GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
		GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
		break;

	case IRQ_FALL_RISE:
		GPIO_ITTrigger = GPIO_INT_Trigger_BOTHEDGE;
		GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
		break;

	case IRQ_LOW:
		GPIO_ITTrigger = GPIO_INT_Trigger_LEVEL;
		GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
		break;

	case IRQ_HIGH:
		GPIO_ITTrigger = GPIO_INT_Trigger_LEVEL;
		GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_HIGH;
		break;

	case IRQ_NONE:
		// ?
		break;

	default:
		break;
	}

	if (enable) {
		GPIO_INTMode(obj->pin, ENABLE, GPIO_ITTrigger, GPIO_ITPolarity, GPIO_INT_DEBOUNCE_ENABLE);
	} else {
		GPIO_INTMode(obj->pin, DISABLE, 0, 0, 0);
	}
}

/**
  * @brief  Enable GPIO interrupt.
  * @param  obj: GPIO irq object defined in application software.
  * @retval none
  */
void gpio_irq_enable(gpio_irq_t *obj)
{
	GPIO_INTConfig(obj->pin, ENABLE);
}

/**
  * @brief  Disable GPIO interrupt.
  * @param  obj: GPIO irq object defined in application software.
  * @retval none
  */
void gpio_irq_disable(gpio_irq_t *obj)
{
	GPIO_INTConfig(obj->pin, DISABLE);
}

/**
  * @brief  Deinitialize the GPIO device's interrupt mode, including mode, trigger and polarity registers.
  * @param  obj: GPIO irq object defined in application software.
  * @retval none
  * @note It is the same with function GPIO_DeInit();
  */
void gpio_irq_deinit(gpio_irq_t *obj)
{
	GPIO_DeInit(obj->pin);
}

/**
  * @brief  Configure pull type of the selected interrupt pin.
  * @param  obj: GPIO irq object defined in application software.
  * @param  pull_type: This parameter can be one of the following values:
  *		@arg PullNone: HighZ, user can input high or low with this pin.
  *		@arg PullDown: Pull down.
  *		@arg PullUp: Pull up.
  * @retval none
  * @note It is the same with function gpio_mode();
  */
void gpio_irq_pull_ctrl(gpio_irq_t *obj, PinMode pull_type)
{
	u32 GPIO_PuPd;

	switch (pull_type) {
	case PullNone:/* No driver -> Input & High Impendance */
		GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;

	case PullDown:
		GPIO_PuPd = GPIO_PuPd_DOWN;
		break;

	case PullUp:
		GPIO_PuPd = GPIO_PuPd_UP;
		break;

	default:
		GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	}

	PAD_PullCtrl(obj->pin, GPIO_PuPd);
}

/**
  * @brief  Configure GPIO interrupt as enabled or disabled.
  * @param  obj: GPIO irq object defined in application software.
  * @param  event: GPIO interrupt event, this parameter can be one of the following values:
  *		@arg IRQ_RISE: Rising edge interrupt event.
  *		@arg IRQ_FALL: Falling edge interrupt event.
  *		@arg IRQ_FALL_RISE: Both rising and falling edge interrupt event.
  *		@arg IRQ_LOW: Low level interrupt event.
  *		@arg IRQ_HIGH: High level interrupt event.
  *		@arg IRQ_NONE: No interrupt event.
  * @param  enable: This parameter can be one of the following values:
  *		@arg 0: Disable GPIO interrupt.
  *		@arg 1: Enable GPIO interrupt.
  * @retval none
  * @note It is the same with function gpio_irq_set();
  */
void gpio_irq_set_event(gpio_irq_t *obj, gpio_irq_event event)
{
	gpio_irq_set(obj, event, ENABLE);
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
