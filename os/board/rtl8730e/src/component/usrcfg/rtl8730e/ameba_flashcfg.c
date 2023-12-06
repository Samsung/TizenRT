/**
  ******************************************************************************
  * @file    rtl8721dlp_flashcfg.c
  * @author
  * @version V1.0.0
  * @date    2018-09-12
  * @brief   This file provides firmware functions to manage the following
  *          functionalities:
  *           - flash function config
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

void flash_init_userdef(void);
void nand_init_userdef(void);
void flash_layout_init(void);

/**
* @brif Indicate the flash baudrate. It can be one of the following value:
*	0xFFFF: division ratio is 10 => SPIC clock = 1/10 nppll
*	0x7FFF: division ratio is 9 => SPIC clock = 1/9 nppll
*	0x3FFF: division ratio is 8 => SPIC clock = 1/8 nppll
*	0x1FFF: division ratio is 7 => SPIC clock = 1/7 nppll
*	0xFFF: division ratio is 6 => SPIC clock = 1/6 nppll
*	0x7FF: division ratio is 5 => SPIC clock = 1/5 nppll
*	0x3FF: division ratio is 4 => SPIC clock = 1/4 nppll
*/
const u16 Flash_Speed = 0x7FF;

/**
* @brif Indicate the flash read I/O mode. It can be one of the following value:
*	0xFFFF: Read quad IO, Address & Data 4 bits mode
*	0x7FFF: Read quad O, Just data 4 bits mode
*	0x3FFF: Read dual IO, Address & Data 2 bits mode
*	0x1FFF: Read dual O, Just data 2 bits mode
*	0x0FFF: 1 bit mode
* @note If the configured read mode is not supported, other modes would be searched until find the appropriate mode.
*/
const u16 Flash_ReadMode = 0xFFFF;

/* ftl */

u8 ftl_phy_page_num = 3;			/* The number of physical map pages, default is 3*/
u32 ftl_phy_page_start_addr;		/* The start offset of flash pages which is allocated to FTL physical map.
																	Users should modify it according to their own memory layout!! */
/**
* @brif Flash_AVL maintains the flash IC supported by SDK.
*	If users want to adpot new flash, add item in the following AVL.
*	Note that, if new flash can be classfied as one of the Realtek-defined categories according to Classification SPEC,
*	filling in the defined class index is necessary. Otherwise, FlashClassUser can be used to indicate new class.
*/
const FlashInfo_TypeDef Flash_AVL[] = {
	/*flash_id,		flash_id_mask,	flash_class,		status_mask,	FlashInitHandler */

	/* case1: Realtek defined class, any modification is not allowed */
	{0xEF,			0x000000FF,		FlashClass1,		0x000043FC,		NULL},	/* Winbond: MANUFACTURER_ID_WINBOND */
	{0xA1,			0x000000FF,		FlashClass1,		0x0000FFFC,		NULL}, 	/* Fudan Micro: MANUFACTURER_ID_FM */
	{0x0B,			0x000000FF,		FlashClass1,		0x000043FC,		NULL},	/* XTX */
	{0x0E,			0x000000FF,		FlashClass1,		0x000043FC,		NULL},	/* XTX(FT) */
	{0xC8,			0x000000FF,		FlashClass2,		0x000043FC,		NULL},	/* GD normal: MANUFACTURER_ID_GD */
	{0x28C2,		0x0000FFFF,		FlashClass6,		0x000200FC,		NULL},	/* MXIC wide-range VCC: MANUFACTURER_ID_MXIC */
	{0xC2,			0x000000FF,		FlashClass3,		0x000000FC,		NULL},	/* MXIC normal: MANUFACTURER_ID_BOHONG */
	{0x68,			0x000000FF,		FlashClass3,		0x000000FC,		NULL},	/* Hua Hong */
	{0x51,			0x000000FF,		FlashClass3,		0x000000FC,		NULL},	/* GD MD serial */
	{0x1C,			0x000000FF,		FlashClass4,		0x000000FC,		NULL},	/* ESMT: MANUFACTURER_ID_EON */
	{0x20,			0x000000FF,		FlashClass5,		0x000000FC,		NULL},	/* Microm: MANUFACTURER_ID_MICRON */

	/* case2: new flash, ID is not included in case1 list, but specification is compatible with FlashClass1~FlashClass6 */
	//{0xXX,			0x0000XXXX,		FlashClassX,		0x0000XXXX,		NULL},

	/* case3: new flash, ID is not included in case1 list, and specification is not compatible with FlashClass1~FlashClass6 */
	{0x00,			0x000000FF,		FlashClassUser,		0xFFFFFFFF,		&flash_init_userdef},

	/* End */
	{0xFF,			0xFFFFFFFF,		FlashClassNone,		0xFFFFFFFF,		NULL},
};

/**
* @brif NAND_AVL maintains the flash IC supported by SDK.
*	If users want to adpot new flash, add item in the following AVL.
*	Note that, if new flash can be classfied as one of the Realtek-defined categories according to Classification SPEC,
*	filling in the defined class index is necessary. Otherwise, FlashClassUser can be used to indicate new class.
*/
const FlashInfo_TypeDef NAND_AVL[] = {
	/*flash_id,		flash_id_mask,	flash_class,		status_mask,	FlashInitHandler */

	/* case1: Realtek defined class, any modification is not allowed */
	{0xEF,			0x000000FF,		FlashClass1,		0xFFFFFFFF,		NULL},	/* Winbond: MANUFACTURER_ID_WINBOND */
	{0xC8,			0x000000FF,		FlashClass2,		0xFFFFFFFF,		NULL},	/* GD recommend: MANUFACTURER_ID_GD */
	{0xE5,			0x000000FF, 	FlashClass2,		0xFFFFFFFF, 	NULL},	/* Dosilicon */
	{0xC2,			0x000000FF,		FlashClass3,		0xFFFFFFFF,		NULL},	/* MXIC normal: MANUFACTURER_ID_BOHONG */
	{0x2C,			0x000000FF,		FlashClass5,		0xFFFFFFFF,		NULL},	/* Microm: MANUFACTURER_ID_MICRON */

	/* case2: new flash, ID is not included in case1 list, but specification is compatible with FlashClass1~FlashClass6 */
	//{0xXX,			0x0000XXXX,		FlashClassX,		0x0000XXXX,		NULL},

	/* case3: new flash, ID is not included in case1 list, and specification is not compatible with FlashClass1~FlashClass6 */
	{0x00,			0x000000FF,		FlashClassUser,		0xFFFFFFFF,		&nand_init_userdef},

	/* End */
	{0xFF,			0xFFFFFFFF,		FlashClassNone,		0xFFFFFFFF,		NULL},
};

/*
* @brif	Nor Flash layout is set according to Nor Flash Layout in User Manual
*  In each entry, the first item is flash regoin type, the second item is start address, the second item is end address */
FlashLayoutInfo_TypeDef Flash_Layout_Nor[] = {
	/*Region_Type,		[StartAddr,	EndAddr]		*/
	{IMG_BOOT, 			0x08000000, 0x0801FFFF}, //Boot Manifest(4K) + KM4 Bootloader(124K)
	//Users should modify below according to their own memory
	{IMG_APP_OTA1, 	0x08020000, 0x081FFFFF}, //Certificate(4K) + Manifest(4K) + KR4 & KM4 Application OTA1 + Manifest(4K) + RDP IMG OTA1
	{IMG_APIMG_OTA1,	0x08200000, 0x082FFFFF}, //Manifest(4K) + AP IMG OTA1

	{IMG_BOOT_OTA2, 	0x08300000, 0x0833FFFF}, //Boot Manifest(4K) + KM4 Bootloader(252K) OTA
	{IMG_APP_OTA2, 	0x08340000, 0x0851FFFF}, //Certificate(4K) + Manifest(4K) + KR4 & KM4 Application OTA2 + Manifest(4K) + RDP IMG OTA2
	{IMG_APIMG_OTA2,	0x08520000, 0x0861FFFF}, //Manifest(4K) + AP IMG OTA2
	{FTL,				0x08019000, 0x0801c000}, 
//	{FTL,				0x08620000, 0x08622FFF}, //FTL for BT(>=12K), The start offset of flash pages which is allocated to FTL physical map.
	{VFS1, 				0x08623000, 0x08642FFF}, //VFS region 1 (128K)
	{USER, 				0xFFFFFFFF, 0xFFFFFFFF}, //reserve for user
	/* End */
	{0xFF, 				0xFFFFFFFF, 0xFFFFFFFF},
};

/*
* @brif	Nand Flash layout is set according to Nand Flash Layout in User Manual
*  In each entry, the first item is flash regoin type, the second item is start address, the second item is end address */
FlashLayoutInfo_TypeDef Flash_Layout_Nand[] = {
	/*Region_Type,		[StartAddr,	EndAddr]		*/
	{IMG_BOOT, 			0x08000000, 0x0801FFFF}, //Boot Manifest(4K) + KM4 Bootloader(124K)
	//Users should modify below according to their own memory
	{IMG_APP_OTA1, 	0x08020000, 0x081FFFFF}, //Certificate(4K) + Manifest(4K) + KR4 & KM4 Application OTA1 + Manifest(4K) + RDP IMG OTA1
	{IMG_APIMG_OTA1,	0x08200000, 0x082FFFFF}, //Manifest(4K) + AP IMG OTA1

	{IMG_BOOT_OTA2, 	0x08300000, 0x0833FFFF}, //Boot Manifest(4K) + KM4 Bootloader(252K) OTA
	{IMG_APP_OTA2, 	0x08340000, 0x0851FFFF}, //Certificate(4K) + Manifest(4K) + KR4 & KM4 Application OTA2 + Manifest(4K) + RDP IMG OTA2
	{IMG_APIMG_OTA2,	0x08520000, 0x0861FFFF}, //Manifest(4K) + AP IMG OTA2

	{FTL,				0x0C400000, 0x0C45FFFF}, //FTL for BT(>=384K), The start offset of flash pages which is allocated to FTL physical map.
	{VFS1, 				0x0C460000, 0x0C55FFFF}, //VFS region 1(1024K)
	{USER, 				0xFFFFFFFF, 0xFFFFFFFF}, //reserve for user
	/* End */
	{0xFF, 				0xFFFFFFFF, 0xFFFFFFFF},
};


/**
 * @brief  To adopt user defined class, need to initialize the parameters in the function below
 *       with its default values according to flash Spec, and add additional config code if need.
 * @param none.
 * @retval none
 * @note	The following parameter setting is an example of MXIC flash. For users reference.
 */
void flash_init_userdef(void)
{
	FLASH_InitTypeDef *FLASH_InitStruct = &flash_init_para;

	/*1. Initialize CMD parameters */

	FLASH_InitStruct->FLASH_Id = FLASH_ID_MXIC;

	/*1.1 status bit define */
	FLASH_InitStruct->FLASH_QuadEn_bit = BIT(6);
	FLASH_InitStruct->FLASH_Busy_bit = BIT(0);
	FLASH_InitStruct->FLASH_WLE_bit = BIT(1);
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/*1.2 dummy cycle */
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = FLASH_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = FLASH_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = FLASH_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = FLASH_DM_CYCLE_4IO;

	/*1.3 set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = 0xBB;
	FLASH_InitStruct->FLASH_rd_dual_o = 0x3B;

	/*1.4 set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = 0xEB;
	FLASH_InitStruct->FLASH_rd_quad_o = 0x6B;

	/*1.5 other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = 0x06;
	FLASH_InitStruct->FLASH_cmd_rd_id = 0x9F;
	FLASH_InitStruct->FLASH_cmd_rd_status = 0x05;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = 0x01;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0x60;
	FLASH_InitStruct->FLASH_cmd_block_e = 0xD8;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0x20;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0xAB;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0xB9;

	/*2. Add other configuration code here if needed for flash */
	//TODO

}

/**
 * @brief  To adopt user defined class, need to initialize the parameters in the function below
 *       with its default values according to flash Spec, and add additional config code if need.
 * @param none.
 * @retval none
 * @note	The following parameter setting is an example of MXIC flash. For users reference.
 */
void nand_init_userdef(void)
{
	FLASH_InitTypeDef *FLASH_InitStruct = &flash_init_para;

	/*1. Initialize CMD parameters */

	FLASH_InitStruct->FLASH_Id = FLASH_ID_MXIC;

	/*1.1 status bit define */
	/* mxic  QE bit is bit0 of feature 0xB0, QSPI is disabled by default */
	/* WP#, HOLD# are enabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = NAND_CFG_QUAD_ENABLE;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;


	/*1.2 dummy cycle */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0;


	/*1.3 set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = 0;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = 0;


	/*1.4 set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = 0;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = 0;

	/*1.5 other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/*2. Add other configuration code here if needed for flash */
	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}

void flash_get_layout_info(u32 type, u32 *start, u32 *end)
{
	u32 i = 0;
	u32 tmp_type;
	FlashLayoutInfo_TypeDef *pLayout;

#ifdef CONFIG_LINUX_FW_EN
	if (SYSCFG_BootFromNor()) {
		pLayout = Flash_Layout_Nor_Linux;
	} else {
		pLayout = Flash_Layout_Nand_Linux;
	}
#else
	if (SYSCFG_BootFromNor()) {
		pLayout = Flash_Layout_Nor;
	} else {
		pLayout = Flash_Layout_Nand;
	}
#endif

	while (pLayout[i].region_type != 0xFF) {
		tmp_type = pLayout[i].region_type;
		if (tmp_type == type) {
			if (start != NULL) {
				*start = pLayout[i].start_addr;
			}
			if (end != NULL) {
				*end = pLayout[i].end_addr;
			}
			return;
		}
		i++;
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
