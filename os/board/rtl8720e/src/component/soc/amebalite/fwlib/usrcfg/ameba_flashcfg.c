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
*	0x1FF: division ratio is 3 => SPIC clock = 1/3 nppll
*	0xFF: division ratio is 2 => SPIC clock = 1/2 nppll
*	other value is not support.
*/
const u16 Flash_Speed = 0xFF;

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
	{0x20,			0x000000FF,		FlashClass1,		0x000043FC,		NULL},	/* XMC: MANUFACTURER_ID_WINBOND */
	//{0x20,			0x000000FF,		FlashClass5,		0x000000FC,		NULL},	/* Micron: MANUFACTURER_ID_MICRON */

	/* case2: new flash, ID is not included in case1 list, but specification is compatible with FlashClass1~FlashClass6 */
	//{0xXX,			0x0000XXXX,		FlashClassX,		0x0000XXXX,		NULL},

	/* case3: new flash, ID is not included in case1 list, and specification is not compatible with FlashClass1~FlashClass6 */
	{0x00,			0x000000FF,		FlashClassUser,		0xFFFFFFFF,		&flash_init_userdef},

	/* End */
	{0xFF,			0xFFFFFFFF,		FlashClassNone,		0xFFFFFFFF,		NULL},
};

/*
* @brif	Flash layout is set according to Flash Layout in User Manual
*  In each entry, the first item is flash regoin type, the second item is start address, the second item is end address */
const FlashLayoutInfo_TypeDef Flash_Layout[] = {
	{IMG_BOOT, 		0x08000000, 0x08006FFF}, //Boot Manifest(4K) + KM4 Bootloader(24K)
	{RSVD, 			0x08007000, 0x0800EFFF}, //Reserved(32K)
	{FTL,			0x0800F000, 0x08012FFF}, //FTL for BT(16K), The start offset of flash pages which is allocated to FTL physical map.
	//Users should modify it according to their own memor
	{SYS_DATA,		0x08013000, 0x08013FFF}, //System data(4K)
	{IMG_IMG2_OTA1, 0x08014000, 0x081DFFFF}, //Certificate(4K) + Manifest(4K) + KR4 & KM4 IMG2 OTA1
	{IMG_IMG3_OTA1, 0x081E0000, 0x081FFFFF}, //Manifest(4K) + RDP IMG OTA1
	{IMG_BOOT_OTA, 	0x08200000, 0x08206FFF}, //Boot Manifest(4K) + KM4 Bootloader(24K) OTA
	{IMG_IMG2_OTA2, 0x08214000, 0x083DFFFF}, //Certificate(4K) + Manifest(4K) + KR4 & KM4 IMG2 OTA2
	{IMG_IMG3_OTA2, 0x083E0000, 0x083FFFFF}, //Manifest(4K) + RDP IMG OTA2
	{IMG_DSP,		0x08400000, 0x087DFFFF}, //Manifest(4K) + DSP IMG, only one DSP region in layout
	{USER, 			0x087E0000, 0x087FFFFF}, //Littlefs for user(32K)

	/* End */
	{0xFF, 			0xFFFFFFFF, 0xFFFFFFFF},
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

u32 IMG_ADDR[4][2] = {0}; /* IMG Flash Physical Address use for OTA */

u32 FLASH_RESERVED_DATA_BASE; /* Flash reserved data base address */
u32 FLASH_SYSTEM_DATA_ADDR;	/* Flash system data address */

u32 LFS_FLASH_BASE_ADDR; /* Littlefs Flash start address */
u32 LFS_DEVICE_SIZE;  /* Littlefs Flash region size */

u32 ftl_phy_page_start_addr;	/* The start offset of flash pages which is allocated to FTL physical map.
																	Users should modify it according to their own memory layout!! */
u8 ftl_phy_page_num = 3;		/* The number of physical map pages, default is 3*/

/**
 * @brief  Initialize ota image flash physical address, littlefs base, ftl base, reserved base and system data address.
 * @param none.
 * @retval none
 */
void flash_layout_init(void)
{
	u32 i = 0;
	u32 temp;

	while (Flash_Layout[i].region_type != 0xFF) {
		temp = Flash_Layout[i].region_type;
		if (temp == IMG_BOOT) {
			IMG_ADDR[OTA_IMGID_BOOT][OTA_INDEX_1] = Flash_Layout[i].start_addr;
		} else if (temp == IMG_BOOT_OTA) {
			IMG_ADDR[OTA_IMGID_BOOT][OTA_INDEX_2] = Flash_Layout[i].start_addr;
		} else if (temp == IMG_IMG2_OTA1) {
			IMG_ADDR[OTA_IMGID_IMG2][OTA_INDEX_1] = Flash_Layout[i].start_addr;
		} else if (temp == IMG_IMG2_OTA2) {
			IMG_ADDR[OTA_IMGID_IMG2][OTA_INDEX_2] = Flash_Layout[i].start_addr;
		} else if (temp == IMG_IMG3_OTA1) {
			IMG_ADDR[OTA_IMGID_IMG3][OTA_INDEX_1] = Flash_Layout[i].start_addr;
		} else if (temp == IMG_IMG3_OTA2) {
			IMG_ADDR[OTA_IMGID_IMG3][OTA_INDEX_2] = Flash_Layout[i].start_addr;
		} else if (temp == IMG_DSP) {
			IMG_ADDR[OTA_IMGID_DSP][OTA_INDEX_1] = Flash_Layout[i].start_addr;
			IMG_ADDR[OTA_IMGID_DSP][OTA_INDEX_2] = Flash_Layout[i].start_addr;
		} else if (temp == USER) {
			LFS_FLASH_BASE_ADDR = Flash_Layout[i].start_addr - SPI_FLASH_BASE;
			LFS_DEVICE_SIZE = Flash_Layout[i].end_addr - Flash_Layout[i].start_addr + 1;
		} else if (temp == FTL) {
			ftl_phy_page_start_addr = Flash_Layout[i].start_addr - SPI_FLASH_BASE;
			ftl_phy_page_num = (Flash_Layout[i].end_addr - Flash_Layout[i].start_addr + 1) / 0x1000 - 1;
		} else if (temp == RSVD) {
			FLASH_RESERVED_DATA_BASE = Flash_Layout[i].start_addr - SPI_FLASH_BASE;
		} else if (temp == SYS_DATA) {
			FLASH_SYSTEM_DATA_ADDR = Flash_Layout[i].start_addr - SPI_FLASH_BASE;
		}

		i++;
	}

}


/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
