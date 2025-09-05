/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * os/board/stm32f407-disc1/src/stm32f407-disc1.h
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_STM32F407_DISC1_SRC_STM32F407DISC1_H
#define __ARCH_ARM_SRC_STM32F407_DISC1_SRC_STM32F407DISC1_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>
#include <stdint.h>
#include <arch/board/board.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration *************************************************************/
/* How many SPI modules does this chip support? */

#if STM32_NSPI < 1
#undef CONFIG_STM32_SPI1
#undef CONFIG_STM32_SPI2
#undef CONFIG_STM32_SPI3
#elif STM32_NSPI < 2
#undef CONFIG_STM32_SPI2
#undef CONFIG_STM32_SPI3
#elif STM32_NSPI < 3
#undef CONFIG_STM32_SPI3
#endif

/* Assume that we have everything */

#define HAVE_USBDEV     1
#define HAVE_USBHOST    1
#define HAVE_USBMONITOR 1
#define HAVE_SDIO       1

/* Can't support USB host or device features if USB OTG FS is not enabled */

#ifndef CONFIG_STM32_OTGFS
#undef HAVE_USBDEV
#undef HAVE_USBHOST
#undef HAVE_USBMONITOR
#endif

/* Can't support USB device monitor if USB device is not enabled */

#ifndef CONFIG_USBDEV
#undef HAVE_USBDEV
#undef HAVE_USBMONITOR
#endif

/* Can't support USB host is USB host is not enabled */

#ifndef CONFIG_USBHOST
#undef HAVE_USBHOST
#endif

/* Check if we should enable the USB monitor before starting TASH */

#if !defined(CONFIG_USBDEV_TRACE) || !defined(CONFIG_SYSTEM_USBMONITOR)
#undef HAVE_USBMONITOR
#endif

/* Can't support MMC/SD features if mountpoints are disabled or if SDIO support
 * is not enabled.
 */

#if defined(CONFIG_DISABLE_MOUNTPOINT) || !defined(CONFIG_STM32_SDIO)
#undef HAVE_SDIO
#endif

#undef SDIO_MINOR				/* Any minor number, default 0 */
#define SDIO_SLOTNO 0			/* Only one slot */

#ifdef HAVE_SDIO

#if defined(CONFIG_TASH_MMCSDSLOTNO) && CONFIG_TASH_MMCSDSLOTNO != 0
#warning "Only one MMC/SD slot, slot 0"
#undef CONFIG_TASH_MMCSDSLOTNO
#define CONFIG_TASH_MMCSDSLOTNO SDIO_SLOTNO
#endif

#if defined(CONFIG_TASH_MMCSDMINOR)
#define SDIO_MINOR CONFIG_TASH_MMCSDMINOR
#else
#define SDIO_MINOR 0
#endif

/* SD card bringup does not work if performed on the IDLE thread because it
 * will cause waiting.  Use either:
 *
 *  CONFIG_TASH_ARCHINIT=y, OR
 *  CONFIG_BOARD_INITIALIZE=y && CONFIG_BOARD_INITTHREAD=y
 */

#if defined(CONFIG_BOARD_INITIALIZE) && !defined(CONFIG_BOARD_INITTHREAD)
#warning "SDIO initialization cannot be perfomed on the IDLE thread"
#undef HAVE_SDIO
#endif
#endif

/* STM32F4 Discovery GPIOs **************************************************/
/* LEDs */

#define GPIO_LED1       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                         GPIO_OUTPUT_CLEAR | GPIO_PORTD | GPIO_PIN12)
#define GPIO_LED2       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                         GPIO_OUTPUT_CLEAR | GPIO_PORTD | GPIO_PIN13)
#define GPIO_LED3       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                         GPIO_OUTPUT_CLEAR | GPIO_PORTD | GPIO_PIN14)
#define GPIO_LED4       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                         GPIO_OUTPUT_CLEAR | GPIO_PORTD | GPIO_PIN15)

/* BUTTONS -- NOTE that all have EXTI interrupts configured */

#define MIN_IRQBUTTON   BUTTON_USER
#define MAX_IRQBUTTON   BUTTON_USER
#define NUM_IRQBUTTONS  1

#define GPIO_BTN_USER   (GPIO_INPUT | GPIO_FLOAT | GPIO_EXTI | GPIO_PORTA | GPIO_PIN0)

/* PWM
 *
 * The STM32F4 Discovery has no real on-board PWM devices, but the board can be
 * configured to output a pulse train using TIM4 CH2 on PD13.
 */

#define STM32F4DISCOVERY_PWMTIMER   4
#define STM32F4DISCOVERY_PWMCHANNEL 2

/* SPI chip selects */

#define GPIO_CS_MEMS      (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN3)

/* USB OTG FS
 *
 * PA9  OTG_FS_VBUS VBUS sensing (also connected to the green LED)
 * PC0  OTG_FS_PowerSwitchOn
 * PD5  OTG_FS_Overcurrent
 */

#define GPIO_OTGFS_VBUS   (GPIO_INPUT | GPIO_FLOAT | GPIO_SPEED_100MHz | \
                           GPIO_OPENDRAIN | GPIO_PORTA | GPIO_PIN9)
#define GPIO_OTGFS_PWRON  (GPIO_OUTPUT | GPIO_FLOAT | GPIO_SPEED_100MHz | \
                           GPIO_PUSHPULL | GPIO_PORTC | GPIO_PIN0)

#ifdef CONFIG_USBHOST
#define GPIO_OTGFS_OVER (GPIO_INPUT | GPIO_EXTI | GPIO_FLOAT | \
                           GPIO_SPEED_100MHz | GPIO_PUSHPULL | \
                           GPIO_PORTD | GPIO_PIN5)

#else
#define GPIO_OTGFS_OVER (GPIO_INPUT | GPIO_FLOAT | GPIO_SPEED_100MHz | \
                           GPIO_PUSHPULL | GPIO_PORTD | GPIO_PIN5)
#endif

/* UG-2864AMBAG01 or UG-2864HSWEG01 OLED Display (SPI 4-wire):
 *
 * --------------------------+----------------------------------------------
 * Connector CON10 J1:      | STM32F4Discovery
 * --------------+-----------+----------------------------------------------
 * CON10 J1:     | CON20 J2: | P1/P2:
 * --------------+-----------+----------------------------------------------
 * 1  3v3        | 3,4 3v3   | P2 3V
 * 3  /RESET     | 8 /RESET  | P2 PB6 (Arbitrary selection)
 * 5  /CS        | 7 /CS     | P2 PB7 (Arbitrary selection)
 * 7  A0|D/C     | 9 A0|D/C  | P2 PB8 (Arbitrary selection)
 * 9  LED+ (N/C) | -----     | -----
 * 2  5V Vcc     | 1,2 Vcc   | P2 5V
 * 4  DI         | 18 D1/SI  | P1 PA7 (GPIO_SPI1_MOSI == GPIO_SPI1_MOSI_1 (1))
 * 6  SCLK       | 19 D0/SCL | P1 PA5 (GPIO_SPI1_SCK == GPIO_SPI1_SCK_1 (1))
 * 8  LED- (N/C) | -----     | ------
 * 10 GND        | 20 GND    | P2 GND
 * --------------+-----------+----------------------------------------------
 * (1) Required because of on-board MEMS
 * -------------------------------------------------------------------------
 */

#if defined(CONFIG_LCD_UG2864AMBAG01) || defined(CONFIG_LCD_UG2864HSWEG01)
#define GPIO_OLED_RESET (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_CLEAR | GPIO_PORTB | GPIO_PIN6)
#define GPIO_OLED_CS    (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_SET | GPIO_PORTB | GPIO_PIN7)
#define GPIO_OLED_A0    (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_CLEAR | GPIO_PORTB | GPIO_PIN8)
#define GPIO_OLED_DC    GPIO_OLED_A0
#endif

/* STM32F4DIS-BB MicroSD
 *
 * ---------- ------------- ------------------------------
 * PIO        SIGNAL        Comments
 * ---------- ------------- ------------------------------
 * PB15       NCD           Pulled up externally
 * PC9        DAT1          Configured by driver
 * PC8        DAT0          "        " "" "    "
 * PC12       CLK           "        " "" "    "
 * PD2        CMD           "        " "" "    "
 * PC11       CD/DAT3       "        " "" "    "
 * PC10       DAT2          "        " "" "    "
 * ---------- ------------- ------------------------------
 */

#if defined(CONFIG_STM32F4DISBB) && defined(CONFIG_STM32_SDIO)
#define GPIO_SDIO_NCD   (GPIO_INPUT | GPIO_FLOAT | GPIO_EXTI | \
                           GPIO_PORTB | GPIO_PIN15)
#endif

/* ENC28J60
 *
 * --- ------ -------------- -------------------------------------------------------------------
 * PIN NAME   SIGNAL         NOTES
 * --- ------ -------------- -------------------------------------------------------------------
 *
 * 29 PA4    (no name)   10Mbit ENC28J60 Chip Selcet, SPI 2M FLASH
 * 30 PA5    PA5-SPI1-SCK1   2.4" TFT + Touchscreen, 10Mbit ENC28J60, SPI 2M FLASH
 * 31 PA6    PA6-SPI1-MISO1  2.4" TFT + Touchscreen, 10Mbit ENC28J60, SPI 2M FLASH
 * 32 PA7    PA7-SPI1-MOSI1  2.4" TFT + Touchscreen, 10Mbit ENC28J60, SPI 2M FLASH
 * 98 PE1    PE1-FSMC_NBL1  2.4" TFT + Touchscreen, 10Mbit EN28J60 Reset - not configured
 *    PA15    (no name)     10Mbps ENC28J60 Interrupt
 */

/* CS and Reset are active low.  Initial states are not selected and in
 * reset.  The ENC28J60 is taken out of reset when the driver is
 * initialized (thedriver does a soft reset too).
 */

#ifdef CONFIG_ENC28J60
#define GPIO_ENC28J60_CS    GPIO_SPI1_CS
/*#define GPIO_ENC28J60_RESET (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                               GPIO_OUTPUT_CLEAR|GPIO_PORTE|GPIO_PIN1)*/
#define GPIO_ENC28J60_INTR  GPIO_SPI1_INT
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public data
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_spiinitialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the stm32f4discovery
 *   board.
 *
 ****************************************************************************/

void weak_function stm32_spiinitialize(void);

/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_INITIALIZE=y :
 *     Called from board_initialize().
 *
 *   CONFIG_BOARD_INITIALIZE=y && CONFIG_TASH_ARCHINIT=y :
 *     Called from the TASH library
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_usbinitialize very early in initialization to setup
 *   USB-related GPIO pins for the STM32F4Discovery board.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_OTGFS
void weak_function stm32_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host
 *   functionality. This function will start a thread that will monitor for
 *   device connection/disconnection events.
 *
 ****************************************************************************/

#if defined(CONFIG_STM32_OTGFS) && defined(CONFIG_USBHOST)
int stm32_usbhost_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_extmemgpios
 *
 * Description:
 *   Initialize GPIOs for external memory usage
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_FSMC
void stm32_extmemgpios(const uint32_t *gpios, int ngpios);
#endif

/****************************************************************************
 * Name: stm32_extmemaddr
 *
 * Description:
 *   Initialize adress line GPIOs for external memory access
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_FSMC
void stm32_extmemaddr(int naddrs);
#endif

/****************************************************************************
 * Name: stm32_extmemdata
 *
 * Description:
 *   Initialize data line GPIOs for external memory access
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_FSMC
void stm32_extmemdata(int ndata);
#endif

/****************************************************************************
 * Name: stm32_enablefsmc
 *
 * Description:
 *  enable clocking to the FSMC module
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_FSMC
void stm32_enablefsmc(void);
#endif

/****************************************************************************
 * Name: stm32_disablefsmc
 *
 * Description:
 *  enable clocking to the FSMC module
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_FSMC
void stm32_disablefsmc(void);
#endif


/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_INITIALIZE=y :
 *     Called from board_initialize().
 *
 *   CONFIG_BOARD_INITIALIZE=n && CONFIG_TASH_ARCHINIT=y :
 *     Called from the TASH library
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_sdio_initialize
 *
 * Description:
 *   Initialize SDIO-based MMC/SD card support
 *
 ****************************************************************************/

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_STM32_SDIO)
int stm32_sdio_initialize(void);
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __ARCH_ARM_SRC_STM32F407_DISC1_SRC_STM32F407DISC1_H */
