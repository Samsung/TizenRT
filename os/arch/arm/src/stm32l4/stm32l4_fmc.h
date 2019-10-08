/*
 * stm32l4_fmc.h
 *
 *  Created on: Sep 25, 2019
 *      Author: calebkang
 */

#ifndef OS_ARCH_ARM_SRC_STM32L4_STM32L4_FMC_H_
#define OS_ARCH_ARM_SRC_STM32L4_STM32L4_FMC_H_

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include "chip.h"


/************************************************************************************
 * Public Functions
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

uint8_t stm32l4_fmc_psram_initialize(void);
uint8_t stm32l4_fmc_psram_deinitialize(void);
uint8_t stm32l4_fmc_psram_read(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t stm32l4_fmc_psram_read_dma(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t stm32l4_fmc_psram_write(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t stm32l4_fmc_psram_write_dma(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);


/****************************************************************************
 * Name: stm32_fmc_enable
 *
 * Description:
 *   Enable clocking to the FMC.
 *
 ****************************************************************************/

void stm32l4_fmc_enable(void);

/****************************************************************************
 * Name: stm32_fmc_disable
 *
 * Description:
 *   Disable clocking to the FMC.
 *
 ****************************************************************************/

void stm32l4_fmc_disable(void);


#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* OS_ARCH_ARM_SRC_STM32L4_STM32L4_FMC_H_ */
