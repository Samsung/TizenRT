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
	u32 AddrStart, Offset, PhyAddr;
	u32 FwAddr = 0;
	u8 EmpSig = 0;

	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	u32 CtrlTemp = RSIP->FLASH_MMU[0].RSIP_REMAPxBAR;
	u32 OffsetTemp = RSIP->FLASH_MMU[0].RSIP_REMAPxOR;

	if (OffsetTemp & RSIP_BIT_REMAP_x_ENABLE) {
		AddrStart = RSIP_GET_REMAP_BASE_x_ADDR(CtrlTemp);
		Offset = RSIP_GET_REMAP_x_OFFSET(OffsetTemp);

		PhyAddr = (AddrStart + Offset) << RSIP_REMAP_REGION_ADDR_SHIFT;

		if (PhyAddr == LS_IMG2_OTA1_ADDR) {
			FwAddr = LS_IMG2_OTA1_ADDR;
		} else {
			FwAddr = LS_IMG2_OTA2_ADDR;
		}
	}

	if(FwAddr == 0){
		DBG_8195A("Wrong FwAddr in sys_clear_ota_signature \r\n");
	}
	FLASH_WriteStream(FwAddr - SPI_FLASH_BASE, 4, &EmpSig);
}

/**
  * @brief  Recover the signature of the other firmware.
  * @retval none
  */
void sys_recover_ota_signature(void)
{
	u32 AddrStart, Offset, PhyAddr;
	u8 Ota2Use = _FALSE;
	u32 DstAddr, CurAddr;
	u8 OldSig, NewSig;
	u8 *buf;

	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	u32 CtrlTemp = RSIP->FLASH_MMU[0].RSIP_REMAPxBAR;
	u32 OffsetTemp = RSIP->FLASH_MMU[0].RSIP_REMAPxOR;

	if (OffsetTemp & RSIP_BIT_REMAP_x_ENABLE) {
		AddrStart = RSIP_GET_REMAP_BASE_x_ADDR(CtrlTemp);
		Offset = RSIP_GET_REMAP_x_OFFSET(OffsetTemp);

		PhyAddr = (AddrStart + Offset) << RSIP_REMAP_REGION_ADDR_SHIFT;

		if (PhyAddr == LS_IMG2_OTA1_ADDR) {
			Ota2Use = _FALSE;
		} else {
			Ota2Use = _TRUE;
		}
	}

	if (Ota2Use) {
		CurAddr = LS_IMG2_OTA2_ADDR;
		DstAddr = LS_IMG2_OTA1_ADDR;
	} else {
		CurAddr = LS_IMG2_OTA1_ADDR;
		DstAddr = LS_IMG2_OTA2_ADDR;
	}

	/* Get signature of the two firmware */
	ota_readstream_user(DstAddr, 4, &OldSig);
	ota_readstream_user(CurAddr, 4, &NewSig);
	DBG_8195A("old sig: %08x, new sig:%08x\n", OldSig, NewSig);

	if (OldSig == NewSig) {
		return;
	}

	/* Backup first 4KB of the old firmware */
	buf = (u8 *)pvPortMalloc(4096);
	ota_readstream_user(DstAddr, 4096, buf);

	/* Modify signature */
	_memcpy(buf, &NewSig, 4);

	/* Erase first sector */
	flash_erase_sector(NULL, DstAddr);

	/* Write back 4KB to first sector */
	flash_stream_write(NULL, DstAddr, 4096, buf);

	vPortFree(buf);
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
