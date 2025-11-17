/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_SYSCFG_H_
#define _AMEBA_SYSCFG_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup SYSCFG
  * @brief SYSCFG driver modules
  * @{
  */

/** @addtogroup SYSCFG
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * Used for system, user can not used it if not needed.
  *
  *****************************************************************************************
  * REG_SYS_SYSTEM_CFG0 Introduction
  *****************************************************************************************
  *
  *    BIT[31]		Trapped PKG_ENG_SEL value 0: normal package; 1: engineering mode
  *    BIT[30]		CHIP_EN PIN input value
  *    BIT[27:24]	BD info
  *    BIT[16]		1: Test chip;  0:MP
  *    BIT[11:8]	Vendor ID
  *    BIT[7:4]		Chip version
  *    BIT[3:0]		Vendor ID defined in RF
  *
  *****************************************************************************************
  * REG_LP_SYSTEM_CFG1 Introduction
  *****************************************************************************************
  *
  *    BIT[31:28]	is BIT_SYSCFG_TRP_ICFG, value is load from following trap pin:
  *    			ICFG[0]/ICFG[1]/ICFG[2]/ICFG[3] when trap pin TSET_MODE_SEL = 1
  *    BIT[27]		is BIT_SYSCFG_TRP_BOOT_SEL
  *    			0: boot normal, 1: uart flash download
  *    			value load from trap pin UART_DOWNLOAD_IMAGE
  *    BIT[25]		is BIT_SYSCFG_TRP_SPSLDO_SEL, Trapped Selection for SPS
  *    			0: SWR mode; 1: LDO mode
  *    			load from trap pin SPS_LDO_SEL
  *    BIT[8]		BIT_SYS_XCLK_VLD	Xtal Clock Stable, 1: Clock Stable
  *    BIT[0]		BIT_SYSCFG_ALDN_STS 1: SYS CFG autoload done; 0; SYSCFG autoload not ready
  *
  *****************************************************************************************
  * REG_LP_SYSTEM_CFG2 Introduction
  *****************************************************************************************
  *
  *    BIT[7:0]		ROM Information
  *
  *
  *****************************************************************************************
    * trap pins
  *****************************************************************************************
  *
  *    GPIOA_0:
  *			TEST_MODE_SEL
  *			default PD
  *
  *    GPIOA_3:
  *			SPS_LDO_SEL
  *			default PU, internal PU
  *			0: SWR 1: LDO
  *			load to 0x1F4[25]
  *
  *    GPIOA_30:
  *			UART_DOWNLOAD_IMAGE
  *			default PU
  *			load to 0x1F4[27]
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup SYSCFG_Exported_Constants SYSCFG Exported Constants
  * @{
  */

/** @defgroup SYSCFG_Bounding_Option_definitions
  * @{
  */
#define SYSCFG_BD_QFN32						((u32)0x000000000)
#define SYSCFG_BD_QFN48_MCM_8MBFlash			((u32)0x000000001)
#define SYSCFG_BD_QFN48						((u32)0x000000002)
#define SYSCFG_BD_QFN48_NEW					((u32)0x000000000)
#define SYSCFG_BD_QFN68						((u32)0x000000007)
#define SYSCFG_BD_QFN68_NEW					((u32)0x000000005)
#define SYSCFG_BD_TFBGA_MCM_8761A			((u32)0x000000004)
/**
  * @}
  */

/** @defgroup SYSCFG_CUT_Version_definitions
  * @{
  */
#define SYSCFG_CUT_VERSION_A				0
#define SYSCFG_CUT_VERSION_B				1
#define SYSCFG_CUT_VERSION_C				2
#define SYSCFG_CUT_VERSION_D				3
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SYSCFG_Exported_Functions SYSCFG Exported Functions
  * @{
  */
_LONG_CALL_ u32 SYSCFG_GetChipInfo(void);
//_LONG_CALL_ u32 SYSCFG_TRP_UARTImage(void);
_LONG_CALL_ u32 SYSCFG_TRP_TestMode(void);
_LONG_CALL_ u32 SYSCFG_TRP_OTPBYP(void);
_LONG_CALL_ u32 SYSCFG_TRP_BootSel(void);
_LONG_CALL_ u32 SYSCFG_TRP_ICFG(void);
_LONG_CALL_ u32 SYSCFG_ROMINFO_Get(void);
_LONG_CALL_ void SYSCFG_ROMINFO_Set(void);
_LONG_CALL_ u32 SYSCFG_CHIPType_Get(void);
_LONG_CALL_ u32 SYSCFG_OTP_RDPEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_RSIPEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_SBootEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_LowBaudLog(void);
_LONG_CALL_ u32 SYSCFG_OTP_DisBootLog(void);
_LONG_CALL_ u32 SYSCFG_OTP_BigPageNand(void);
_LONG_CALL_ u32 SYSCFG_OTP_USBLoadFullSpeed(void);
_LONG_CALL_ u32 SYSCFG_OTP_RandDelayEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_RMAMode(void);
_LONG_CALL_ u32 SYSCFG_OTP_BootFlashBaud(void);
_LONG_CALL_ u32 SYSCFG_OTP_SPICBootSpeedUpDis(void);
_LONG_CALL_ u32 SYSCFG_OTP_SPICAddr4ByteEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_FlashDSleepEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_UartDownloadDis(void);
_LONG_CALL_ u32 SYSCFG_OTP_SBoot_HW_DIS(void);
_LONG_CALL_ u32 SYSCFG_OTP_AntiRollback(void);
_LONG_CALL_ u32 SYSCFG_OTP_FaultLogDis(void);
_LONG_CALL_ u32 SYSCFG_OTP_RSIPMode(void);
_LONG_CALL_ u32 SYSCFG_OTP_HUKDerivEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_RomPatchEn(void);
_LONG_CALL_ u32 SYSCFG_OTP_GetUsbPhyCalData(void);
_LONG_CALL_ u32 SYSCFG_OTP_GetUsbPhyCalData2(void);
_LONG_CALL_ u32 SYSCFG_OTP_UsbDownloadEnable(void);
_LONG_CALL_ u32 SYSCFG_BootFromNor(void);
_LONG_CALL_ u32 SYSCFG_OTP_BootSelect(void);
_LONG_CALL_ u32 SYSCFG_HWTrigDownload(void);
_LONG_CALL_ u32 SYS_M0CtrlSel(u32 id);
_LONG_CALL_ u32 SYSCFG_SWTrigDownload(void);
/**
  * @}
  */

/* Registers Definitions --------------------------------------------------------*/

/** @} */

/**
  * @}
  */

/**
  * @}
  */

/* Other definations --------------------------------------------------------*/
/*******************  Macro definition for TRP_ICFG ********************/
#define SYSCFG_TRP_ICFG_STOP_IN_ROMBOOT		2
#define SYSCFG_TRP_ICFG_FLASH_LOCATION		3 /* Ameba1 used, AmebaZ not use */
#define SYSCFG_OTP_BOOTNOR					2
#define SYSCFG_OTP_BOOTNAND					1

/*******************  Macro definition for RSIP MODE ********************/
#define RSIP_XTS_MODE                                             0
#define RSIP_CTR_MODE                                             1
#define RSIP_GCM_MODE                                            2

#endif //_AMEBA_SYSCFG_H_