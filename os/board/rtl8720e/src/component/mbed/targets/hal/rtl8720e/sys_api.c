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
	u32 PhyAddr;
	u8 Ota2Use = _FALSE;
	u8 otaDstIdx;
	u8 otaCurIdx;
	u32 ota_sig[2];
	u32 Address[2];
	u32 cur_ver = 0;
	u8 empty_sig[4] = {0x0, 0x0, 0x0, 0x0};
	u32 AddrStart;
	int ImgID = 0;

	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	for (ImgID = 1; ImgID < 2; ImgID++) {
		AddrStart = RSIP->FLASH_MMU[ImgID].RSIP_REMAPxOR;

		if (AddrStart & RSIP_BIT_REMAP_x_ENABLE) {
			PhyAddr = (AddrStart >> 9) << 5;

			if (ImgID == OTA_IMGID_IMG2) {
				/* append certificate and manifest */
				PhyAddr = PhyAddr - 0x2000;

				if (PhyAddr == IMG_ADDR[ImgID][OTA_INDEX_1]) {
					Ota2Use = _FALSE;
				} else {
					Ota2Use = _TRUE;
				}

			} else {
				/* append manifest */
				PhyAddr = PhyAddr - 0x1000;
				if (PhyAddr == IMG_ADDR[ImgID][OTA_INDEX_1]) {
					Ota2Use = _FALSE;
				} else {
					Ota2Use = _TRUE;
				}
			}

		}

		if (Ota2Use) {
			otaCurIdx = OTA_INDEX_2;
		} else {
			otaCurIdx = OTA_INDEX_1;
		}

		otaDstIdx = otaCurIdx ^ 1;

		Address[otaCurIdx] = IMG_ADDR[ImgID][otaCurIdx] - SPI_FLASH_BASE;
		Address[otaDstIdx] = IMG_ADDR[ImgID][otaDstIdx] - SPI_FLASH_BASE;

		if (ImgID == OTA_IMGID_IMG2) {
			Address[otaCurIdx] = Address[otaCurIdx] + 0x1000; /* skip certificate for image2 */
			Address[otaDstIdx] = Address[otaDstIdx] + 0x1000; /* skip certificate for image2 */
		}

		/* get current firmware version */
		ota_readstream_user(Address[otaCurIdx] + 20, 4, (u8 *)&cur_ver);

		if (cur_ver == 0x0) {
			DBG_8195A("[%s] IMGID: %d, current firmware is OTA%d, current version: 0x%08x is already cleared\n", __func__, ImgID, (otaCurIdx + 1), cur_ver);
			return;
		}

		/* check if another firmware is valid */
		ota_readstream_user(Address[otaCurIdx], 4, (u8 *)&ota_sig[0]);
		ota_readstream_user(Address[otaCurIdx] + 4, 4, (u8 *)&ota_sig[1]);


		if (0x35393138 != ota_sig[0] || 0x31313738 != ota_sig[1]) {
			DBG_8195A("[%s] IMGID: %d, current firmware is OTA%d, target firmware OTA%d is invalid\n", __func__, ImgID, (otaCurIdx + 1), (otaDstIdx + 1));
			return;
		}

		DBG_8195A("[%s] IMGID: %d, current OTA%d, current version: 0x%08x\n", __func__, ImgID, (otaCurIdx + 1), cur_ver);
		DBG_8195A("[%s] IMGID: %d, current OTA%d Address: 0x%08x, target OTA%d Address: 0x%08x\n", __func__, ImgID, otaCurIdx + 1, Address[otaCurIdx], otaDstIdx + 1,
				  Address[otaDstIdx]);
		FLASH_EreaseDwordsXIP(Address[otaCurIdx] + 20, 1);
		FLASH_EreaseDwordsXIP(Address[otaDstIdx] + 20, 1);
		FLASH_TxData12BXIP(Address[otaDstIdx] + 20, 4, (u8 *)&cur_ver);
		FLASH_TxData12BXIP(Address[otaCurIdx] + 20, 4, empty_sig);
	}
}

/**
  * @brief  Recover the signature of the other firmware.
  * @retval none
  */
void sys_recover_ota_signature(void)
{
	u32 PhyAddr;
	u8 Ota2Use = _FALSE;
	u8 otaDstIdx;
	u8 otaCurIdx;
	u32 ota_sig[2];
	u32 Address[2];
	u32 cur_ver = 0;
	u8 empty_sig[4] = {0x0, 0x0, 0x0, 0x0};
	u32 AddrStart;
	int ImgID = 0;

	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	for (ImgID = 1; ImgID < 2; ImgID++) {
		AddrStart = RSIP->FLASH_MMU[ImgID].RSIP_REMAPxOR;

		if (AddrStart & RSIP_BIT_REMAP_x_ENABLE) {
			PhyAddr = (AddrStart >> 9) << 5;

			if (ImgID == OTA_IMGID_IMG2) {
				/* append certificate and manifest */
				PhyAddr = PhyAddr - 0x2000;

				if (PhyAddr == IMG_ADDR[ImgID][OTA_INDEX_1]) {
					Ota2Use = _FALSE;
				} else {
					Ota2Use = _TRUE;
				}

			} else {
				/* append manifest */
				PhyAddr = PhyAddr - 0x1000;
				if (PhyAddr == IMG_ADDR[ImgID][OTA_INDEX_1]) {
					Ota2Use = _FALSE;
				} else {
					Ota2Use = _TRUE;
				}
			}

		}

		if (Ota2Use) {
			otaDstIdx = OTA_INDEX_1;
		} else {
			otaDstIdx = OTA_INDEX_2;
		}

		otaCurIdx = otaDstIdx ^ 1;

		Address[otaDstIdx] = IMG_ADDR[ImgID][otaDstIdx] - SPI_FLASH_BASE;
		Address[otaCurIdx] = IMG_ADDR[ImgID][otaCurIdx] - SPI_FLASH_BASE;

		if (ImgID == OTA_IMGID_IMG2) {
			Address[otaDstIdx] = Address[otaDstIdx] + 0x1000; /* skip certificate */
			Address[otaCurIdx] = Address[otaCurIdx] + 0x1000; /* skip certificate */
		}

		/* get current firmware version */
		ota_readstream_user(Address[otaCurIdx] + 20, 4, (u8 *)&cur_ver);

		if (cur_ver == 0x0) {
			cur_ver = 1 << 16 | 1; /* set version as default value */
		}

		/* check if recover target firmware is valid */
		ota_readstream_user(Address[otaCurIdx], 4, (u8 *)&ota_sig[0]);
		ota_readstream_user(Address[otaCurIdx] + 4, 4, (u8 *)&ota_sig[1]);

		if (0x35393138 != ota_sig[0] || 0x31313738 != ota_sig[1]) {
			DBG_8195A("[%s] IMGID: %d, current firmware is OTA%d, target firmware OTA%d is invalid\n", __func__, ImgID, (otaCurIdx + 1), (otaDstIdx + 1));
			return;
		}

		DBG_8195A("[%s] IMGID: %d, current OTA%d, current version: 0x%08x\n", __func__, ImgID, (otaCurIdx + 1), cur_ver);
		DBG_8195A("[%s] IMGID: %d, current OTA%d Address: 0x%08x, target OTA%d Address: 0x%08x\n", __func__, ImgID, otaCurIdx + 1, Address[otaCurIdx], otaDstIdx + 1,
				  Address[otaDstIdx]);
		FLASH_EreaseDwordsXIP(Address[otaDstIdx] + 20, 1);
		FLASH_EreaseDwordsXIP(Address[otaCurIdx] + 20, 1);
		FLASH_TxData12BXIP(Address[otaDstIdx] + 20, 4, (u8 *)&cur_ver);
		FLASH_TxData12BXIP(Address[otaCurIdx] + 20, 4, empty_sig);
	}
}

/**
  * @brief  Open log uart.
  * @retval none
  */
void sys_log_uart_on(void)
{
	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, ON);

	LOGUART_INTConfig(LOGUART_DEV, RUART_BIT_ERBI, ENABLE);
	LOGUART_RxCmd(LOGUART_DEV, ENABLE);
}

/**
  * @brief  Close log uart.
  * @retval none
  */
void sys_log_uart_off(void)
{
	LOGUART_INTConfig(LOGUART_DEV, RUART_BIT_ERBI, DISABLE);
	LOGUART_RxCmd(LOGUART_DEV, DISABLE);

	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, OFF);
}


/**
  * @brief  Store or load ADC calibration parameter.
  * @param  write: This parameter can be one of the following values:
  *	   @arg 0: Load ADC calibration parameter a & b & c.
  *    @arg 1: Store ADC calibration parameter a & b & c.
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
  * @brief Vector reset.
  * @retval none
  */
void sys_cpu_reset(void)
{
	DBG_8195A("AmebaLite not support sys_cpu_reset function!\n");
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
