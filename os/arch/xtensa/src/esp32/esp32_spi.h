/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_XTENSA_SRC_ESP32_ESP32_SPIDRV_H
#define __ARCH_XTENSA_SRC_ESP32_ESP32_SPIDRV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <arch/chip/irq.h>
#include "chip/esp32_soc.h"
#include "esp32_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/** SPI master clock is divided by 80MHz apb clock. Below defines are example frequencies, and are accurate. Be free to specify a random frequency, it will be rounded to closest frequency (to macros below if above 8MHz).
* 8MHz
*/
#define SPI_MASTER_FREQ_1M      (APB_CLK_FREQ/80)	///< 1mhz
#define SPI_MASTER_FREQ_2M      (APB_CLK_FREQ/40)	///< 2mhz
#define SPI_MASTER_FREQ_3M      (APB_CLK_FREQ/27)	///< 2.96mhz
#define SPI_MASTER_FREQ_4M      (APB_CLK_FREQ/20)	///< 4mhz
#define SPI_MASTER_FREQ_5M      (APB_CLK_FREQ/15)	///< 5.33mhz
#define SPI_MASTER_FREQ_6M      (APB_CLK_FREQ/13)	///< 6.15mhz
#define SPI_MASTER_FREQ_7M      (APB_CLK_FREQ/11)	///< 7.27mhz
#define SPI_MASTER_FREQ_8M      (APB_CLK_FREQ/10)	///< 8mhz
#define SPI_MASTER_FREQ_9M      (APB_CLK_FREQ/9)	///< 8.89MHz
#define SPI_MASTER_FREQ_10M     (APB_CLK_FREQ/8)	///< 10MHz
#define SPI_MASTER_FREQ_11M     (APB_CLK_FREQ/7)	///< 11.43MHz
#define SPI_MASTER_FREQ_13M     (APB_CLK_FREQ/6)	///< 13.33MHz
#define SPI_MASTER_FREQ_16M     (APB_CLK_FREQ/5)	///< 16MHz
#define SPI_MASTER_FREQ_20M     (APB_CLK_FREQ/4)	///< 20MHz
#define SPI_MASTER_FREQ_26M     (APB_CLK_FREQ/3)	///< 26.67MHz
#define SPI_MASTER_FREQ_40M     (APB_CLK_FREQ/2)	///< 40MHz
#define SPI_MASTER_FREQ_80M     (APB_CLK_FREQ/1)	///< 80MHz

/*
 *
 */
typedef union {
	struct {
		uint32_t clkcnt_l: 6;	/*In the master mode it must be equal to spi_clkcnt_N. In the slave mode it must be 0. */
		uint32_t clkcnt_h: 6;	/*In the master mode it must be floor((spi_clkcnt_N+1)/2-1). In the slave mode it must be 0. */
		uint32_t clkcnt_n: 6;	/*In the master mode it is the divider of spi_clk. So spi_clk frequency is system/(spi_clkdiv_pre+1)/(spi_clkcnt_N+1) */
		uint32_t clkdiv_pre: 13;	/*In the master mode it is pre-divider of spi_clk. */
		uint32_t clk_equ_sysclk: 1;	/*In the master mode 1: spi_clk is eqaul to system 0: spi_clk is divided from system clock. */
	};
	uint32_t val;
} spi_clock_reg_t;

/*
 *
 *
 */
typedef enum {
	ESP32_SPI_PORT2 = 2,
	ESP32_SPI_PORT3 = 3,
	ESP32_SPI_PORT_MAX,
} spi_port_t;

///< SPI2, HSPI
#define HSPI_PORT       ESP32_SPI_PORT2
///< SPI3, VSPI
#define VSPI_PORT       ESP32_SPI_PORT3

///
#define MAX_CS_NUM      (3)

extern void Test_SPI(int port);

#ifdef __cplusplus
}
#endif
#endif							/* __ARCH_XTENSA_SRC_ESP32_ESP32_SPIDRV_H */
