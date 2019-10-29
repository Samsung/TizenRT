/*
 * stm32l4_ospi.h
 *
 *  Created on: Sep 25, 2019
 *      Author: calebkang
 */

#ifndef OS_ARCH_ARM_SRC_STM32L4_STM32L4_OSPI_H_
#define OS_ARCH_ARM_SRC_STM32L4_STM32L4_OSPI_H_

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

struct ospi_dev_s;
FAR struct ospi_dev_s *stm32l4_ospi_initialize(int intf);

int stm32l4_ospi_enter_memorymapped(struct ospi_dev_s *dev, const struct ospi_meminfo_s *meminfo, uint32_t lpto);
int stm32l4_ospi_exit_memorymapped(struct ospi_dev_s *dev);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* OS_ARCH_ARM_SRC_STM32L4_STM32L4_OSPI_H_ */
