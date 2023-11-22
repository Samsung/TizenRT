/** mbed Microcontroller Library
  ******************************************************************************
  * @file    sys_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides following mbed system API:
  *				-JTAG OFF
  *				-LOGUART ON/OFF
  *				-OTA image switch
  *				-System Reset
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "cmsis.h"
#include "sys_api.h"
#include "flash_api.h"
#include "device_lock.h"
#include "ameba_ota.h"

//#define printf					DiagPrintf

#define RSIP_REMAP_REGION_ADDR_SHIFT	12

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_SYSAPI
 *  @brief    MBED_SYSAPI driver modules.
 *  @{
 */


/** @defgroup MBED_SYSAPI_Exported_Functions MBED_SYSAPI Exported Functions
  * @{
  */

/**
  * @brief  Turn off the JTAG function.
  * @retval none
  */
void sys_jtag_off(void)
{
	Pinmux_Swdoff();
}

/**
  * @brief  Clear the signature of current firmware.
  * @retval none
  */
void sys_clear_ota_signature(void)
{
	u8 otaDstIdx;
	u8 otaCurIdx;
	u32 ota_sig[2];
	u32 Address[2];
	u8 empty_sig[8] = {0x0};
	int ImgID = 0;
	u32 app_ota1_start_addr;
	u32 app_ota2_start_addr;

	flash_get_layout_info(IMG_APP_OTA1, &app_ota1_start_addr, NULL);
	flash_get_layout_info(IMG_APP_OTA2, &app_ota2_start_addr, NULL);

	otaCurIdx = ota_get_cur_index(0);
	otaDstIdx = otaCurIdx ^ 1;

	for (ImgID = 1; ImgID < MAX_IMG_NUM; ImgID++) {

		Address[otaCurIdx] = (otaCurIdx == 0 ? app_ota1_start_addr : app_ota2_start_addr) - SPI_FLASH_BASE;
		Address[otaDstIdx] = (otaDstIdx == 0 ? app_ota1_start_addr : app_ota2_start_addr) - SPI_FLASH_BASE;

		if (ImgID == OTA_IMGID_APP) {
			Address[otaCurIdx] = Address[otaCurIdx] + 0x1000; /* skip certificate for image2 */
			Address[otaDstIdx] = Address[otaDstIdx] + 0x1000; /* skip certificate for image2 */
		}

		DBG_8195A("[%s] IMGID: %d, current OTA%d Address: 0x%08x, target OTA%d Address: 0x%08x\n", __func__, ImgID, otaCurIdx + 1, Address[otaCurIdx], otaDstIdx + 1,
				  Address[otaDstIdx]);

		ota_sig[0] = HAL_READ32(SPI_FLASH_BASE, Address[otaDstIdx]);
		ota_sig[1] = HAL_READ32(SPI_FLASH_BASE, Address[otaDstIdx] + 4);

		if (ota_sig[0] == 0x35393138 && ota_sig[1] == 0x31313738) {
			device_mutex_lock(RT_DEV_LOCK_FLASH);
			FLASH_WriteStream(Address[otaCurIdx], 8, (u8 *)empty_sig);
			device_mutex_unlock(RT_DEV_LOCK_FLASH);
		} else {
			DBG_8195A("[%s] IMGID: %d, current firmware is OTA%d, target firmware OTA%d is invalid\n", __func__, ImgID, (otaCurIdx + 1), (otaDstIdx + 1));
		}
	}
}

/**
  * @brief  Recover the signature of the other firmware.
  * @retval none
  */
void sys_recover_ota_signature(void)
{
	u8 otaDstIdx;
	u8 otaCurIdx;
	u8 *backup = NULL;
	u32 Address[2];
	u32 sig[2] = {0x35393138, 0x31313738};
	int ImgID = 0;
	u32 app_ota1_start_addr;
	u32 app_ota2_start_addr;

	backup = (u8 *)malloc(0x1000);
	if (backup == NULL) {
		DBG_8195A("[%s] backup malloc failded\n", __func__);
		return;
	}

	flash_get_layout_info(IMG_APP_OTA1, &app_ota1_start_addr, NULL);
	flash_get_layout_info(IMG_APP_OTA2, &app_ota2_start_addr, NULL);

	otaCurIdx = ota_get_cur_index(0);
	otaDstIdx = otaCurIdx ^ 1;

	for (ImgID = 1; ImgID < MAX_IMG_NUM; ImgID++) {

		Address[otaDstIdx] = (otaDstIdx == 0 ? app_ota1_start_addr : app_ota2_start_addr) - SPI_FLASH_BASE;
		Address[otaCurIdx] = (otaCurIdx == 0 ? app_ota1_start_addr : app_ota2_start_addr) - SPI_FLASH_BASE;

		if (ImgID == OTA_IMGID_APP) {
			Address[otaDstIdx] = Address[otaDstIdx] + 0x1000; /* skip certificate */
			Address[otaCurIdx] = Address[otaCurIdx] + 0x1000; /* skip certificate */
		}

		DBG_8195A("[%s] IMGID: %d, current OTA%d Address: 0x%08x, target OTA%d Address: 0x%08x\n", __func__, ImgID, otaCurIdx + 1, Address[otaCurIdx], otaDstIdx + 1,
				  Address[otaDstIdx]);
		device_mutex_lock(RT_DEV_LOCK_FLASH);
		/* backup this sector */
		FLASH_ReadStream(Address[otaDstIdx], 0x1000, backup);

		/* erase this sector */
		FLASH_EraseXIP(EraseSector, Address[otaDstIdx]);

		/* copy signature to backup */
		_memcpy((void *)backup, (const void *)sig, 8);

		/* write this sector with target data erased */
		for (int idx = 0; idx < 0x1000; idx += 256) {
			FLASH_WriteStream((Address[otaDstIdx] + idx), 256, (u8 *)backup);
		}
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
	}

	free(backup);
}

/**
  * @brief  Open log uart.
  * @retval none
  */
void sys_log_uart_on(void)
{
	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, ON);

	UART_INTConfig(UART2_DEV, RUART_BIT_ERBI, ENABLE);
	UART_RxCmd(UART2_DEV, ENABLE);
}

/**
  * @brief  Close log uart.
  * @retval none
  */
void sys_log_uart_off(void)
{
	UART_INTConfig(UART2_DEV, RUART_BIT_ERBI, DISABLE);
	UART_RxCmd(UART2_DEV, DISABLE);

	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, OFF);
}


/**
  * @brief  Store or load ADC calibration parameter.
  * @param  write: This parameter can be one of the following values:
  *		@arg 0: Load ADC calibration parameter a & b & c.
  *		@arg 1: Store ADC calibration parameter a & b & c.
  * @param  a: Pointer to ADC parameter a.
  * @param  b: Pointer to ADC parameter b.
  * @param  c: Pointer to ADC parameter c.
  * @retval none
  */
void sys_adc_calibration(u8 write, u16 *a, u16 *b, u16 *c)
{
	/* To avoid gcc warnings */
	(void) write;
	(void) a;
	(void) b;
	(void) c;

	printf("ADC calibration is finished in FT test. Calibration parameters can be found in EFUSE." \
		   "Please refer to Battery Measurement chapter in Application Note to get calibration parameters.\n");

	assert_param(0);
}

/**
  * @brief  System software reset.
  * @retval none
  */
void sys_reset(void)
{
	System_Reset();
}


/**
  * @brief  Vector reset.
  * @retval none
  */
void sys_cpu_reset(void)
{
	DBG_8195A("Ameba-D2 not support sys_cpu_reset function!\n");
	assert_param(0);
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
