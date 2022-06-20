/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"


/**
  * @brief  Configure slave port as secure or non-secure attribute.
  * @param  PPC_PeripheralId: it can be one of the followings:
		SECFG_PSRAM_USERMODE = 0,
		SECFG_SPIC_USERMODE,
		SECFG_BT_CFG,
		SECFG_OTPC,
		SECFG_PSRAM_PHY,
		SECFG_SYSON,
		SECFG_RXI300,
		SECFG_UART0,
		SECFG_UART1,
		SECFG_UART2,
		SECFG_UART3_BT,
		SECFG_UART4_LOG,
		SECFG_LEDC,
		SECFG_TRNG_PORT1_2,
		SECFG_AUDIO_CODEC,
		SECFG_TIMER0_7_BASIC,
		SECFG_TIMER8_11,
		SECFG_GPIOA_B,
		SECFG_RTC,
		SECFG_ADC_ADC_COMP,
		SECFG_THERMAL,
		SECFG_CAP_TOUCH,
		SECFG_WDG,
		SECFG_IPC,
		SECFG_SDM32K,
		SECFG_AUDIO_CFG,
		SECFG_WIFI_CFG,
		SECFG_RSVD,
		SECFG_SPORT0_I2S,
		SECFG_SPORT1_I2S,
		SECFG_AES,
		SECFG_SHA,
		SECFG_GDMA0_CFG,
		SECFG_SPI0,
		SECFG_SPI1,
		SECFG_I2C0,
		SECFG_I2C1,
		SECFG_DBG_TIMER,
		SECFG_EDCSA,
  * @param  Status: it can be SECURE or NON_SECURE
  * @retval None
  */
void TZ_ConfigSlaveSecurity(PPC_PeripheralId Perip, u32 Status)
{
	PPC_TypeDef *PPC = KM4_PPC; //km0, modify later
	PPC_TypeDef *PPC1 = KM4_PPC2;

	u32 Value, IsReg0, BitIndex;

	if (IS_PPC0(Perip)) {
		Value = PPC->PPC_REG0;
		BitIndex = Perip - 26;
		IsReg0 = TRUE;
	} else {
		Value = PPC1->PPC_REG0;
		BitIndex = Perip;
		IsReg0 = FALSE;
	}

	if (Status == SECURE) {
		Value &= ~(1 << BitIndex);
	} else if (Status == NON_SECURE) {
		Value |= (1 << BitIndex);
	}

	if (IsReg0) {
		PPC->PPC_REG0 = Value;
	} else {
		PPC1->PPC_REG0 = Value;
	}

}
