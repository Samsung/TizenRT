/**
  ******************************************************************************
  * @file    rtl8721d_syscfg_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for SYSCFG firmware
  *          library.
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

/**
  * @brief  Get SYSCFG0 register, used for CUT version.
  * @retval Value: Bits defination
  *		 @arg BIT_MASK_SYSCFG_CUT_VER 0x000000F0
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_GetChipInfo(void)
{
	return HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);
}

/**
  * @brief  Get CUT version.
  * @retval Value: CUT version
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_CUTVersion(void)
{
	u32 value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0) & LSYS_MASK_HW_CHIP_ID;

	return LSYS_GET_HW_CHIP_ID(value32);
}

/**
  * @brief  check use UART download
  * @retval Value:
  * TRUE : UART_DOWNLOAD MODE
  * FALSE : Normal Mode
  * @note none.
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_TRP_UARTImage(void)
{
	u32 	value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);

	if (value32 & LSYS_BIT_PTRP_UARTDL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  check use ICFG used for test mode
  * @retval Value: ICFG value
  * @note you should pull low TSET_MODE_SEL trap pin if you want to use ICFG
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_TRP_ICFG(void)
{
	u32 	value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG1);

	return LSYS_GET_PTRP_ICFG(value32);
}

/**
  * @brief  check  test mode
  * @retval Value:
  * 1 :Test mode
  * 0 : Normal mode
  * @note you should pull High TSET_MODE_SEL trap pin if you want to use ICFG
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_TRP_TestMode(void)
{
	u32 value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);

	/* For MP chip The Trap pin High means normal mode, and low means Test mode.
	    However the load value is invert of pin status
	*/
	return LSYS_GET_PTRP_TMODE(value32);
}

/**
  * @brief  OTP BYPass
  * @retval Value:
  * 1 :OTP Bypass
  * 0 :Do not do Bypass
  * @note you should pull High TSET_MODE_SEL trap pin if you Bypass OTP
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_TRP_OTPBYP(void)
{
	u32 value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);

	/* For MP chip The Trap pin High means do not bypass, and low means bypass.
	    However the load value is invert of pin status
	*/
	return LSYS_GET_PTRP_OTPBYP(value32);

}

/**
  * @brief  Boot select pin status
  * @retval Value:
  * 1 : NorFlash
  * 0 : Nandflash
  * @note OnlyUsed when otp bootsel= 2'11 or 2'00
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_TRP_BootSel(void)
{
	u32 value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);

	return LSYS_GET_PTRP_BOOTSEL(value32);
}

/**
  * @brief  rom info get
  * @retval Value: rom info
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_ROMINFO_Get(void)
{
	u32 	value32 = LSYS_GET_ROM_VERSION_SW(HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG));

	return value32;
}

/**
  * @brief  rom info set
  */
HAL_ROM_TEXT_SECTION
void SYSCFG_ROMINFO_Set(void)
{
	u32 RomInfo = (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG) & ~LSYS_MASK_ROM_VERSION_SW);

	RomInfo |= ROMINFORMATION | (ROMVERSION_SUB << 8);

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, RomInfo);
}

/**
  * @brief  get chip type
  * @retval Value: ASIC FPGA PALADIUM RTL_SIM
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_CHIPType_Get(void)
{

	u32 ChipType = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);

	ChipType = LSYS_GET_HW_CHIP_TYPE(ChipType);

	return ChipType;
}

/**
  * @brief  Check whether RDP is enable in OTP
  * @retval
  * TRUE: Enabled
  * FALSE: Disabled
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_RDPEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);
	u32 Temp1 = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if ((Temp & SEC_BIT_LOGIC_RDP_EN) || (!(Temp1 & SEC_BIT_RDP_EN))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Check whether RSIP is enable in OTP
  * @retval
  * TRUE: Enabled
  * FALSE: Disabled
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_RSIPEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);
	u32 Temp1 = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if ((Temp & SEC_BIT_LOGIC_RSIP_EN) || (!(Temp1 & SEC_BIT_RSIP_EN))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Check whether Secure BOOT is enable in OTP
  * @retval
  * TRUE: Enabled
  * FALSE: Disabled
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_SBootEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);
	u32 Temp1 = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if ((Temp & SEC_BIT_LOGIC_SECURE_BOOT_EN) || (!(Temp1 & SEC_BIT_SECURE_BOOT_EN))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Check whether need lower the log
  * baudrate from 1500000 to 115200
  * @retval
  * TRUE: lower the baudrate
  * FALSE: keep at 1500000
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_LowBaudLog(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_LOW_BAUD_LOG_EN) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Check whether need Disable all the Boot log
  * @retval
  * TRUE: Disable all the boot log
  * FALSE: keep boot log
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_DisBootLog(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_DIS_BOOT_LOG_EN) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Check whether The nand is use big Page
  * @retval
  * TRUE: The nand page size is 4K
  * FALSE: The nand page size is 2K
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_BigPageNand(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_BIG_PAGE_NAND) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Check whether the USB download speed in ROM code is full speed
  * @retval
  * TRUE: Full speed
  * FALSE: High speed, default
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_USBLoadFullSpeed(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_USB_LOAD_SPEED) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Boot from nor or Nand
  * @retval
  * 2'11: Decide by Power on latch pin
  * 2'10: Boot from Nor
  * 2'01: Boot from Nand
  * 2'00: Decide by Power on latch pin
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_BootSelect(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	return SEC_GET_BOOT_SELECT(Temp);
}

/**
  * @brief Boot Flash clock selection
  * Baudrate = 40/(2*(1+x)) 0: 20M 1: 10M
  * @retval
  * TRUE:10M
  * FALSE: 20M
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_BootFlashBaud(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_BOOT_FLASH_BAUD_SEL) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief Whether speedup SPIC Bootflow or not
  * @retval
  * TRUE: do not speed up
  * FALSE: Speed up
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_SPICBootSpeedUpDis(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_SPIC_BOOT_SPEEDUP_DIS) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Whether use 4Byte address for nor flash
  * @retval
  * TRUE: use 4 byte address
  * FALSE: use 3 byte address
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_SPICAddr4ByteEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_SPIC_ADDR_4BYTE_EN) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Whether enable flash deepsleep mode
  * @retval
  * TRUE: Nor flash deep enabled
  * FALSE: Nor flash doesn't enabled
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_FlashDSleepEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if (Temp & SEC_BIT_FLASH_DEEP_SLEEP_EN) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Disable Power on Latch UartDownload or not
  * TRUE: Disable Power on Latch uartDownload
  * FALSE: Do not Disable Power on Latch uartDownload
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_UartDownloadDis(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if (!(Temp & SEC_BIT_UART_DOWNLOAD_DISABLE)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Check whether HW engine need Disable in Secure BOOT
  * @retval
  * TRUE: Enabled
  * FALSE: Disabled
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_SBoot_HW_DIS(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if (!(Temp & SEC_BIT_SECURE_BOOT_HW_DIS)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Check Anti-rollback is enable or not
  * TRUE: Anti-rollback function is enabled
  * FALSE: Anti-rollback function is disabled
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_AntiRollback(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if (!(Temp & SEC_BIT_ANTI_ROLLBACK_EN)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Disable Rom Hardfault Log or not
  * TRUE: Disable Rom Hardfault Log
  * FALSE: Do not Rom Hardfault Log
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_FaultLogDis(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if (!(Temp & SEC_BIT_FAULT_LOG_PRINT_DIS)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief RSIP alg select
  * OTP content
  * 2'11: XTS Mode
  * 2'10: AES-256-GCM
  * 2'01: AES-256-CTR
  * 2'00: XTS Mode
  * @retval
  * RSIP_GCM_MODE: AES-256-GCM
  * RSIP_CTR_MODE: AES-256-CTR
  * RSIP_XTS_MODE: XTS Mode
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_RSIPMode(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if (SEC_GET_RSIP_MODE(Temp) == RSIP_CTR_MODE) {
		return RSIP_CTR_MODE;
	} else if (SEC_GET_RSIP_MODE(Temp) == RSIP_GCM_MODE) {
		return RSIP_CTR_MODE;
	} else {
		return RSIP_XTS_MODE;
	}
}

/**
  * @brief  HUK derivation enable or not
  * TRUE: HUK derivation enable
  * FALSE: HUK derivation disable
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_HUKDerivEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if (!(Temp & SEC_BIT_HUK_DERIV_EN)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Disable Rom Patch or not. Disable Rom patch
  * if the ROM patch ctrl is empty
  * TRUE: Disable Rom Patch
  * FALSE: Do not Disable Rom Patch
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_RomPatchEn(void)
{
	u8 Temp = HAL_READ8(OTPC_REG_BASE, SEC_ROM_PATCH);

	/* By default the Rom Patch should Disabled(OTP empty with the value 3).
		Also if Realtek Do not need Rom patch, it can be disabled it forever in
		Realtek factory(all bits programed with the value 0)
	 */
	if ((SEC_GET_ROM_PATCH_PG(Temp) == SEC_ROM_PATCH_EMPTY) ||
		(SEC_GET_ROM_PATCH_PG(Temp) == SEC_ROM_PATCH_DIS)) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/**
  * @brief  Disablae random delay or not
  *  need modify to physical bits in MP chip
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_RandDelayEn(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);

	if ((Temp & SEC_BIT_BOOT_RAND_DELAY_EN)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Check whether the chip is in RMA mode
  * TRUE: the chip is in RMA mode
  * FALSE: The chip is in normal mode
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_RMAMode(void)
{
	u8 Temp = HAL_READ8(OTPC_REG_BASE, SEC_RMA);
	int cnt = 0;
	int i;

	for (i = 0; i < 8; i++) {
		if (Temp & BIT0) {
			cnt ++;
		}

		Temp >>= 1;
	}

	if ((cnt % 2) == 0) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/**
  * @brief  Get USB PHY calibration data
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_GetUsbPhyCalData(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG2);
	return Temp;
}

/**
  * @brief  Get USB PHY calibration data 2
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_GetUsbPhyCalData2(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG3);
	return Temp;
}

/**
  * @brief  Enable USB download or not
  * TRUE:  Enable USB download
  * FALSE: Disable USB download
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_OTP_UsbDownloadEnable(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_OTP_SYSCFG0);
	if (Temp & SEC_BIT_USB_DOWNLOAD_EN) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Check Boot from nor or nand
  * TRUE: Boot from Nor
  * FALSE: Boot from Nand
  */
__weak HAL_ROM_TEXT_SECTION
u32 SYSCFG_BootFromNor(void)
{
	if (SYSCFG_OTP_BootSelect() == SYSCFG_OTP_BOOTNOR) {
		return TRUE;
	}
	if (SYSCFG_OTP_BootSelect() == SYSCFG_OTP_BOOTNAND) {
		return FALSE;
	}

	/*Boot Select decided by Trap pin*/
	if (SYSCFG_TRP_BootSel()) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Check enter UART download or not
  * TRUE: enter UART download mode
  * FALSE: Boot from Nand
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_HWTrigDownload(void)
{
	if (SYSCFG_OTP_UartDownloadDis()) {
		return FALSE;
	}

	/*Decided by Trap pin*/
	return SYSCFG_TRP_UARTImage();
}

/**
  * @brief  select master port for M0 in lp platform.
  * @param  id
  * 0: SIC
  * 1: OTPC
  * 2/3 : KM0
  * @retval none
  */
__weak HAL_ROM_TEXT_SECTION
u32 SYS_M0CtrlSel(u32 id)
{
	u32 TmpIdx = 0;
	u32 Temp = 0;

	/* Polling Until LS platform IDLE*/
	while ((!Temp)  && (TmpIdx < 20000)) {
		Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_LPLAT_STATUS) & LSYS_BIT_LS_IDLE;
		DelayUs(1);
		TmpIdx++;
	}

	if (TmpIdx >= OTP_POLL_TIMES) {
		return _FAIL;
	}

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_LPLAT_CTRL);
	Temp &= ~LSYS_MASK_PLFM_MST0_SEL;
	Temp |= LSYS_PLFM_MST0_SEL(id);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_LPLAT_CTRL, Temp);

	return _SUCCESS;
}

/**
  * @brief Check if enable sotfware trigger UART download or not
  * TRUE: enable Software trigger UART download
  * FALSE: disable Software trigger UART download
  */
HAL_ROM_TEXT_SECTION
u32 SYSCFG_SWTrigDownload(void)
{
	u32 Temp = HAL_READ32(OTPC_REG_BASE, SEC_CFG2);

	if ((Temp & SEC_BIT_SWTRIG_UART_DOWNLOAD_DISABLE) && (BKUP_Read(0) & BKUP_BIT_UARTBURN_BOOT)) {
		return TRUE;
	} else {
		return FALSE;
	}
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

