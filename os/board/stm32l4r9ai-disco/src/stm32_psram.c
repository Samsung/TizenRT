/*
 * stm32_psram.c
 *
 *  Created on: Sep 25, 2019
 *      Author: calebkang
 */
#include <tinyara/config.h>

#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>

#include <arch/board/board.h>
#include <arch/board/stm32l4r9i_discovery_io.h>

#include "up_arch.h"
#include "stm32l4_fmc.h"

extern uint32_t bsp_lcd_initialized;
extern uint32_t bsp_psram_initialized;
/**
  * @brief  PSRAM power on
  *         Power on PSRAM.
  */
static void PSRAM_PowerOn(void)
{
  /* Configure DSI_RESET and DSI_POWER_ON only if lcd is not currently used */
  if(bsp_lcd_initialized == 0)
  {
    BSP_IO_Init();

#if defined(USE_STM32L4R9I_DISCO_REVA) || defined(USE_STM32L4R9I_DISCO_REVB)
    /* Set DSI_POWER_ON to input floating to avoid I2C issue during input PD configuration */
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_INPUT);

    /* Configure the GPIO connected to DSI_RESET signal */
    BSP_IO_ConfigPin(IO_PIN_10, IO_MODE_OUTPUT);

    /* Activate DSI_RESET (active low) */
    BSP_IO_WritePin(IO_PIN_10, GPIO_PIN_RESET);

    /* Configure the GPIO connected to DSI_POWER_ON signal as input pull down */
    /* to activate 3V3_LCD. VDD_LCD is also activated if VDD = 3,3V */
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_INPUT_PD);

    /* Wait at least 1ms before enabling 1V8_LCD */
    HAL_Delay(1);

    /* Configure the GPIO connected to DSI_POWER_ON signal as output low */
    /* to activate 1V8_LCD. VDD_LCD is also activated if VDD = 1,8V */
    BSP_IO_WritePin(IO_PIN_8, GPIO_PIN_RESET);
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_OUTPUT);
#else /* USE_STM32L4R9I_DISCO_REVA || USE_STM32L4R9I_DISCO_REVB */
    /* Configure the GPIO connected to DSI_3V3_POWERON signal as output low */
    /* to activate 3V3_LCD. VDD_LCD is also activated if VDD = 3,3V */
    BSP_IO_WritePin(IO_PIN_8, GPIO_PIN_RESET);
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_OUTPUT);

    /* Wait at least 1ms before enabling 1V8_LCD */
    HAL_Delay(1);

    /* Configure the GPIO connected to DSI_1V8_POWERON signal as output low */
    /* to activate 1V8_LCD. VDD_LCD is also activated if VDD = 1,8V */
    BSP_IO_WritePin(AGPIO_PIN_2, GPIO_PIN_RESET);
    BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_OUTPUT);
#endif /* USE_STM32L4R9I_DISCO_REVA || USE_STM32L4R9I_DISCO_REVB */

    /* Wait at least 15 ms (minimum reset low width is 10ms and add margin for 1V8_LCD ramp-up) */
    HAL_Delay(15);
  }
}

/**
  * @brief  PSRAM power off
  *         Power off PSRAM.
  */
static void PSRAM_PowerOff(void)
{
  /* Set DSI_POWER_ON to analog mode only if lcd is not currently used */
  if(bsp_lcd_initialized == 0)
  {
#if defined(USE_STM32L4R9I_DISCO_REVA) || defined(USE_STM32L4R9I_DISCO_REVB)
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_ANALOG);
#else /* USE_STM32L4R9I_DISCO_REVA || USE_STM32L4R9I_DISCO_REVB */
    /* Disable first DSI_1V8_PWRON then DSI_3V3_PWRON */
    BSP_IO_ConfigPin(AGPIO_PIN_2, IO_MODE_ANALOG);
    BSP_IO_ConfigPin(IO_PIN_8, IO_MODE_ANALOG);
#endif /* USE_STM32L4R9I_DISCO_REVA || USE_STM32L4R9I_DISCO_REVB */
  }
}



void stm32_psram_initialize(void)
{
  PSRAM_PowerOn();
  stm32l4_fmc_psram_initialize();
}





