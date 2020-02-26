/**
  ******************************************************************************
  * @file    rtl8721dhp_ota_ram.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the OTA functions.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

#include "ameba_soc.h"
#include "strproc.h"

/**
  * @brief  Set OTA image index.
  * @param  OTAIdx: This parameter can be  one of the following values:
  *            @arg OTA_INDEX_1: select OTA1(low flash address) as image2
  *            @arg OTA_INDEX_2: select OTA2(high flash address) as image2   
  * @retval status value:
  *          - 1: set ok
  *          - 0: set fail
  */

#define OTA1_ADDR 0x6000
#define OTA2_ADDR 0X106000

IMAGE2_RAM_TEXT_SECTION
u32 OTA_Change(u32 OTAIdx)
{
	u32 BitIdx = 0;
	u32 ota2_sig[2];

	ota2_sig[0] = HAL_READ32(SPI_FLASH_BASE, OTA2_ADDR);
	ota2_sig[1] = HAL_READ32(SPI_FLASH_BASE, OTA2_ADDR+4);

	for(int i = 0; i < 2; i++)
		DBG_8195A("%x",ota2_sig[i]);

	if(0x35393138 == ota2_sig[0] && 0x31313738 == ota2_sig[1])
		BitIdx = 2;
	else
		BitIdx = 1;

	ota2_sig[0] = 0x35393138;
	ota2_sig[1] = 0x31313738;
		
	/* even bits 0 currrent is OTA1 */
	if (BitIdx == 1) {
		if (OTAIdx == OTA_INDEX_1) {
			DBG_8195A("currrent is OTA1, select OTA1 \n");
		} else {
			DBG_8195A("currrent is OTA1, select OTA2 \n");
			FLASH_EreaseDwordsXIP(OTA1_ADDR, 2);
			FLASH_EreaseDwordsXIP(OTA2_ADDR, 2);
			FLASH_TxData12BXIP(OTA2_ADDR, 8,(u8*)ota2_sig);
		}
	} else { /* odd bits 0 currrent is OTA2 */
		if (OTAIdx == OTA_INDEX_1) {
			DBG_8195A("currrent is OTA2, select OTA1 \n");
			FLASH_EreaseDwordsXIP(OTA1_ADDR, 2);
			FLASH_EreaseDwordsXIP(OTA2_ADDR, 2);
			FLASH_TxData12BXIP(OTA1_ADDR, 8, (u8*)ota2_sig);
		} else {
			DBG_8195A("currrent is OTA2, select OTA2 \n");
		}	
	}
	
	return _TRUE;
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
