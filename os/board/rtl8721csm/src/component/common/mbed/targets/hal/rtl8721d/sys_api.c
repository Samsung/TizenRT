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
#include "rtl8721d_ota.h"

void rtc_backup_timeinfo(void);

//#define printf					rtw_printf

/** @addtogroup AmebaD_Mbed_API 
  * @{
  */

/** @defgroup MBED_SYSAPI
 *  @brief      MBED_SYSAPI driver modules.
 *  @{
 */
 
/** @defgroup MBED_SYSAPI_Exported_Functions MBED_SYSAPI Exported Functions
  * @{
  */

/**
  * @brief  Turn off the JTAG function
  * @retval none  
  */
void sys_jtag_off(void)
{
	Pinmux_Swdoff();
}

/**
  * @brief  clear the signature of current firmware
  * @retval none
  */
void sys_clear_ota_signature(void)
{
	u32 AddrStart, Offset, IsMinus, PhyAddr;
	u32 FwAddr = 0, DstAddr = 0;
	u8 EmpSig = 0;

	RSIP_REG_TypeDef* RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);
	u32 CtrlTemp = RSIP->FLASH_MMU[0].MMU_ENTRYx_CTRL;

	if (CtrlTemp & MMU_BIT_ENTRY_VALID) {
		AddrStart = RSIP->FLASH_MMU[0].MMU_ENTRYx_STRADDR;
		Offset = RSIP->FLASH_MMU[0].MMU_ENTRYx_OFFSET;
		IsMinus = CtrlTemp & MMU_BIT_ENTRY_OFFSET_MINUS;

		if(IsMinus)
			PhyAddr = AddrStart - Offset;
		else
			PhyAddr = AddrStart + Offset;

		if(PhyAddr == LS_IMG2_OTA1_ADDR){
			FwAddr = LS_IMG2_OTA1_ADDR;
			DstAddr = LS_IMG2_OTA2_ADDR;
		}else{
			FwAddr = LS_IMG2_OTA2_ADDR;
			DstAddr = LS_IMG2_OTA1_ADDR;
		}
	}
	if(HAL_READ32(DstAddr, 0)==0x35393138&&HAL_READ32(DstAddr, 4)==0x31313738){
		FLASH_WriteStream(FwAddr - SPI_FLASH_BASE, 8, &EmpSig);
	}else{
		printf("ERROR: ANOTHER IMAGE2 SIGNATURE IS NOT VALIDED, THIS CMD WILL NOT BE EXCUTED!!!\n");
	}
}

/**
  * @brief  recover the signature of the other firmware
  * @retval none
  */
void sys_recover_ota_signature(void)
{
	u32 AddrStart, Offset, IsMinus, PhyAddr;
	u8 Ota2Use = _FALSE;
	u32 DstAddr, CurAddr;
	u32 sig[2]={0x35393138,0x31313738};

	RSIP_REG_TypeDef* RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);
	u32 CtrlTemp = RSIP->FLASH_MMU[0].MMU_ENTRYx_CTRL;

	/* Get which firmware used */
	if (CtrlTemp & MMU_BIT_ENTRY_VALID) {
		AddrStart = RSIP->FLASH_MMU[0].MMU_ENTRYx_STRADDR;
		Offset = RSIP->FLASH_MMU[0].MMU_ENTRYx_OFFSET;
		IsMinus = CtrlTemp & MMU_BIT_ENTRY_OFFSET_MINUS;

		if(IsMinus)
			PhyAddr = AddrStart - Offset;
		else
			PhyAddr = AddrStart + Offset;

		if(PhyAddr == LS_IMG2_OTA1_ADDR)
			Ota2Use = _FALSE;
		else
			Ota2Use = _TRUE;
	}

	if(Ota2Use) {
		CurAddr = LS_IMG2_OTA2_ADDR;
		DstAddr = LS_IMG2_OTA1_ADDR;
	} else {
		CurAddr = LS_IMG2_OTA1_ADDR;
		DstAddr = LS_IMG2_OTA2_ADDR;
	}
	FLASH_EreaseDwordsXIP(DstAddr-SPI_FLASH_BASE, 2);
	FLASH_EreaseDwordsXIP(DstAddr-SPI_FLASH_BASE, 2);
	FLASH_TxData12BXIP(DstAddr-SPI_FLASH_BASE, 8, (u8*)sig);
}

/**
  * @brief  open log uart
  * @retval none
  */
void sys_log_uart_on(void)
{
	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, ON);

	UART_INTConfig(UART2_DEV, RUART_IER_ERBI, ENABLE);
	UART_RxCmd(UART2_DEV, ENABLE);
}

/**
  * @brief  close log uart
  * @retval none
  */
void sys_log_uart_off(void)
{
	UART_INTConfig(UART2_DEV, RUART_IER_ERBI, DISABLE);
	UART_RxCmd(UART2_DEV, DISABLE);

	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, OFF);
}


/**
  * @brief  store or load adc calibration parameter 
  * @param  write:  this parameter can be one of the following values:
  *		@arg 0: load adc calibration parameter offset & gain
  *		@arg 1: store adc calibration parameter offset & gain
  * @param  offset: pointer to adc parameter offset
  * @param  gain: pointer to adc parameter gain
  * @retval none
  */
void sys_adc_calibration(u8 write, u16 *offset, u16 *gain)
{
	/* To avoid gcc warnings */
	( void ) write;
	( void ) offset;
	( void ) gain;
	
	printf("ADC calibration is finished in FT test. Calibration parameters can be found in EFUSE." \
		"Please refer to Battery Measurement chapter in Application Note to get calibration parameters.\n");

	assert_param(0);
}

/**
  * @brief  system software reset
  * @retval none
  */
void sys_reset(void)
{
	WDG_InitTypeDef WDG_InitStruct;
	u32 CountProcess;
	u32 DivFacProcess;

	rtc_backup_timeinfo();
	
	BKUP_Set(BKUP_REG0, BIT_KM4SYS_RESET_HAPPEN);

	WDG_Scalar(50, &CountProcess, &DivFacProcess);
	WDG_InitStruct.CountProcess = CountProcess;
	WDG_InitStruct.DivFacProcess = DivFacProcess;
	WDG_Init(&WDG_InitStruct);

	WDG_Cmd(ENABLE);
}


/**
  * @brief vector reset 
  * @retval none
  */
void sys_cpu_reset(void)
{
#ifdef AMEBAD_TODO
	u32 reg_value;

	rtc_backup_timeinfo();

	reg_value = HAL_READ32(PERI_ON_BASE, REG_SOC_FUNC_EN);
	reg_value |= BIT_SOC_PATCH_FUNC1;
	HAL_WRITE32(PERI_ON_BASE, REG_SOC_FUNC_EN, reg_value);

	/* Ensure all outstanding memory accesses included */
	/* buffered write are completed before reset */
	__DSB();

	/* CPU reset */
	/* Keep priority group unchanged */
	SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) |
		(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
		SCB_AIRCR_VECTRESET_Pos | SCB_AIRCR_VECTCLRACTIVE_Pos); 

	/* Ensure completion of memory access */
	__DSB();

	while(1);
#endif
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
