/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static const char *TAG = "TRUSTZONE";
/**
  * @brief  Configure slave port as secure or non-secure attribute.
  * @param  PPC_PeripheralId: it can be one of the followings:
			SECFG_SPORT2_I2S
			SECFG_SPORT1_I2S
			SECFG_SPORT0_I2S
			SECFG_VAD
			SECFG_AUDIO_CDEDC
			SECFG_TIMER
			SECFG_TRNG
			SECFG_LEDC
			SECFG_UART3_BT
			SECFG_UART2
			SECFG_UART1
			SECFG_UART0
			SECFG_DDRC
			SECFG_PSRAM_CTRL
			SECFG_RXI300_HS
			SECFG_HS_SYSON
			SECFG_I2C2
			SECFG_I2C1
			SECFG_IR
			SECFG_EDCSA
			SECFG_ED25519
			SECFG_RSA
			SECFG_MIPI_DSI
			SECFG_SPI1
			SECFG_SPI0
			SECFG_GDMA0_CFG
			SECFG_LCDC_CFG
			SECFG_SDEMMC
			SECFG_SHA
			SECFG_AES
			SECFG_USBOTG_CFG
			SECFG_WIFI_CFG
			SECFG_ZIGBEE
			SECFG_DDR_BSTC
			SECFG_DDR_PHY
			SECFG_BT_CFG
			SECFG_SPORT3_I2S
  * @param  Status: it can be SECURE or NON_SECURE
  * @retval None
  */
void TZ_ConfigSlaveSecurity(PPC_PeripheralId Perip, u32 Status)
{
	PPC_TypeDef *PPC = KM4_PPC;
	u32 Value, IsReg1, BitIndex;

	if (IS_PPC_REG1(Perip)) {
		Value = PPC->PPC_REG1;
		BitIndex = Perip - 32;
		IsReg1 = TRUE;
	} else if (IS_PPC_REG0(Perip)) {
		Value = PPC->PPC_REG0;
		BitIndex = Perip;
		IsReg1 = FALSE;
	} else {
		RTK_LOGW(TAG, "PPC PeripID[%d] illegal !!!\n", Perip);
		assert_param(0);
	}

	if (Status == SECURE) {
		Value &= ~(1 << BitIndex);
	} else if (Status == NON_SECURE) {
		Value |= (1 << BitIndex);
	}

	if (IsReg1) {
		PPC->PPC_REG1 = Value;
	} else {
		PPC->PPC_REG0 = Value;
	}

}

