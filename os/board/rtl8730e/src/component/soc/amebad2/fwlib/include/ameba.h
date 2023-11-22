/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _HAL_AMEBA_H_
#define _HAL_AMEBA_H_

#include "platform_autoconf.h"
#include "basic_types.h"
#include "section_config.h"
#include "memproc.h"
#include "strproc.h"
#include "rand.h"
#include "diag.h"
#include "hal_platform.h"
#include "ameba_vector.h"
#include "ameba_loguart.h"
#include "ameba_soc_ps.h"
#include "shell.h"
#include "monitor_rom.h"
#include "ameba_syscfg.h"
#include "ameba_pmc.h"
#include "ameba_clk.h"
#include "ameba_bor.h"
#include "ameba_ipsec.h"
#include "ameba_crypto_api.h"
#include "ameba_boot.h"
#include "ameba_wl_on.h"
#include "ameba_rsip.h"
#include "ameba_spic.h"
#include "ameba_backup_reg.h"
#include "ameba_pinmap.h"
#include "ameba_ipc.h"
#include "sysreg.h"
#include "ameba_pinmux.h"
#include "ameba_otpc.h"
#include "ameba_spinand.h"
#include "ameba_rsip.h"
#include "ameba_chipinfo.h"
#include "ameba_swr_calib.h"
#include "ameba_nandflash.h"
#include "ameba_system.h"
#if (defined(CONFIG_USB_NEW_STACK_EN) || defined(CONFIG_BUILD_USB_LIB))
#include "ameba_usb.h"
#endif
#ifndef CONFIG_BUILD_ROM
#include "ameba_ipc_api.h"
#endif



/* ===========================  Configuration of the ARM ARMV8MBL Processor and Core Peripherals  ============================ */

#if defined (ARM_CORE_CM4)
#ifdef AMEBAD_TODO
#define __ARMV8MML_REV                 0x0000U  /*!< ARMV8MML Core Revision                                                    */
#define __Vendor_SysTickConfig         0        /*!< Set to 1 if different SysTick Config is used                              */
#define __VTOR_PRESENT                 1        /*!< Set to 1 if CPU supports Vector Table Offset Register                     */
#define __FPU_DP                       0        /*!< Double Precision FPU                                                      */
#endif
#define __CM3_REV                      0x0200    /**< Core revision r0p0 */
#define __MPU_PRESENT                  1         /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS               3         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         1         /**< Vendor specific implementation of SysTickConfig is defined *///see vPortSetupTimerInterrupt
#define __SAUREGION_PRESENT            1        /*!< SAU present or not                                                        */
#define DCACHE_4WAY

#define __FPU_PRESENT             1       /*!< FPU present                                   */
#define __VFP_FP__	1
#define __DSP_PRESENT             1

#define __PMU_PRESENT             1
#define __PMU_NUM_EVENTCNT        4       /*!< __PMU_NUM_EVENTCNT range is [2, 31]           */

#ifndef __ARM_FEATURE_CMSE
#define __ARM_FEATURE_CMSE	3
#endif
#include <arm_cmse.h>   /* Use CMSE intrinsics */
#include "core_armv81mml.h"
#include "core_cache.h"
#elif defined (ARM_CORE_CM0)
#define __ARMV8MBL_REV                 0x0000U  /*!< ARMV8MBL Core Revision                                                    */
#define __NVIC_PRIO_BITS               2        /*!< Number of Bits used for Priority Levels                                   */
#define __Vendor_SysTickConfig         0        /*!< Set to 1 if different SysTick Config is used                              */
#define __VTOR_PRESENT                 1        /*!< Set to 1 if CPU supports Vector Table Offset Register                     */
#define __SAU_REGION_PRESENT           0        /*!< SAU present or not                                                        */

#define __MPU_PRESENT                  1         /**< Defines if an MPU is present or not */
#include "core_armv8mbl.h"
#include "core_cache.h"
#elif defined (ARM_CORE_CA32)
#include <string.h>
#include <stdlib.h>

#define __FPU_PRESENT			1
#define __CORTEX_A				7
#include "core_ca.h"
#include "cmsis_cp15.h"
#include "irq_ctrl.h"
#endif
#include "ameba_trustzone.h"
#include "mpu_config.h"

#include "ameba_gdma.h"
#include "ameba_pwmtimer.h"
#include "ameba_gpio.h"
#include "ameba_spi.h"
#include "ameba_uart.h"
#include "ameba_i2c.h"
#include "ameba_adc.h"
#include "ameba_comp.h"
#include "ameba_wdg.h"
#include "ameba_rtc.h"
#include "ameba_delay.h"
#include "ameba_ir.h"
#include "ameba_lcdc.h"
#include "ameba_mipi.h"
#include "ameba_ledc.h"
#include "ameba_audio.h"
#include "ameba_sport.h"
#include "ameba_aontimer.h"
#include "ameba_wakepin.h"

#ifndef ARM_CORE_CA32
#include "ameba_cache.h"
#else
#include "amebaap_cache.h"
#endif
#include "ameba_ddrphy.h"
#include "ameba_ddrc.h"
#include "ameba_ddrc_front.h"
#include "ameba_ddr_device.h"
#include "ameba_sdioh.h"
#include "ameba_sd.h"
#include "ameba_thermal.h"
#include "ameba_debugtimer.h"
#include "ameba_trng.h"
#include "ameba_eddsa.h"
#include "ameba_ecdsa.h"
#include "ameba_rsa.h"
#include "ameba_reset.h"
#include "ameba_psram.h"
#include "ameba_psphy.h"

#include "ameba_rcc.h"
#include "ameba_rom_patch.h"
#include "ameba_usrcfg.h"
#include "log.h"
#ifndef ARM_CORE_CA32
#include "ameba_v8m_crashdump.h"
#endif
// firmware information, located at the header of Image2
#define FW_VERSION          (0x0100)
#define FW_SUBVERSION       (0x0001)
#define FW_CHIP_ID          (0x8195)
#define FW_CHIP_VER         (0x01)
#define FW_BUS_TYPE         (0x01)          // the iNIC firmware type: USB/SDIO
#define FW_INFO_RSV1        (0x00)          // the firmware information reserved
#define FW_INFO_RSV2        (0x00)          // the firmware information reserved
#define FW_INFO_RSV3        (0x00)          // the firmware information reserved
#define FW_INFO_RSV4        (0x00)          // the firmware information reserved

#define FLASH_SECTOR_SIZE				0x1000

/* Spic_Mode */
#define ReadQuadIOMode			0
#define ReadQuadOMode			1
#define ReadDualIOMode			2
#define ReadDualOMode 			3
#define ReadOneMode				4

#define IMAGE_HEADER_LEN		0x20
typedef struct {
	u32 signature[2];
	u32 image_size;
	u32 image_addr;

	/* reserved for extention */
	u32 sb_header;
	u32 reserved[3];
} IMAGE_HEADER;

typedef enum  _HAL_Status {
	HAL_OK            = 0x00,
	HAL_BUSY          = 0x01,
	HAL_TIMEOUT       = 0x02,
	HAL_ERR_PARA      = 0x03,     // error with invaild parameters
	HAL_ERR_MEM       = 0x04,     // error with memory allocation failed
	HAL_ERR_HW        = 0x05,     // error with hardware error

	HAL_ERR_UNKNOWN   = 0xee      // unknown error

} HAL_Status;


#define USE_FULL_ASSERT

#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : io_assert_failed((uint8_t *)__FUNCTION__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void io_assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif //_HAL_AMEBA_H_
