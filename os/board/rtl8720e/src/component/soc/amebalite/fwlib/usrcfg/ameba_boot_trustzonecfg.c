/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

BOOT_RAM_DATA_SECTION
const TZ_CFG_TypeDef sau_config[] = {
//  Start					End						NSC
	{0x0001E000,			0x00045000 - 1,			0},	/* entry0: IROM & DROM NS */
#if defined (CONFIG_TRUSTZONE_EN) && (CONFIG_TRUSTZONE_EN == 1U)
	{0x08000000,			0x6015B000 - 1,			0},	/* entry1: Others */
	{0x6015B000,			0x6015C000 - 1,			1},	/* entry2: NSC */
	{0x60160000,			0xFFFFFFFF,				0},	/* entry3: TODO */
#else
	{0x08000000,			0xFFFFFFFF,				0},	/* entry1: Others */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
#endif
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};

BOOT_RAM_DATA_SECTION
const TZ_CFG_TypeDef mpc3_config[] =					/* Security configuration for PSRAM */
{
//  Start					End						NSC
#if defined (CONFIG_TRUSTZONE_EN) && (CONFIG_TRUSTZONE_EN == 1U)
	{0x60000000,			0x6015B000 - 1,			0},	/* entry0: TODO */
	{0x602e0000,			0xFFFFFFFF,				0},	/* entry1: TODO */
#else
	{0x60000000,			0x60180000 - 1,			0},	/* entry0: TODO */
	{0x60180000,			0xFFFFFFFF,				0},	/* entry1: TODO */
#endif
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};


BOOT_RAM_DATA_SECTION
const TZ_CFG_TypeDef mpc2_config[] =					/* Security configuration for sram1 S3 */
{
//  Start					End						NSC
	{0x20040000,			0x22000000 - 1,			0},	/* entry0: sram1 all ns£¬ including audio buf */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};

BOOT_RAM_DATA_SECTION
const TZ_CFG_TypeDef mpc1_config[] =					/* Security configuration for sram0 S2 */
{
//  Start					End						NSC
	{0x20000000,			0x20007000 - 1,			0},	/* entry0: MSP_NS, ROM_BSS_COMMON, ROM_BSS_NS, STDLIB_HEAP_RAM_NS */
	{0x20010000,			0x20040000 - 1,			0},	/* entry1: BD_RAM_NS*/
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};

BOOT_RAM_DATA_SECTION
const AF_CFG_TypeDef af0_config[] =					/* IDMA */
{
//  Start					End						AccessC
	{0x00000000,			0xFFFFFFFF - 1,			AF_WR},	/* entry0: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};

BOOT_RAM_DATA_SECTION
const AF_CFG_TypeDef af1_config[] =					/* HIFI5 */
{
//  Start					End						AccessC
	{0x00000000,			0xFFFFFFFF - 1,			AF_WR},	/* entry0: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
};

BOOT_RAM_DATA_SECTION
const AF_CFG_TypeDef af2_config[] =					/* LX */
{
//  Start					End						AccessC
	{0x00000000,			0xFFFFFFFF - 1,			AF_WR},	/* entry0: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: AUDIO_BUF */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};

BOOT_RAM_DATA_SECTION
const AF_CFG_TypeDef af3_config[] =					/* WIFI */
{
//  Start					End						AccessC
	{0x00000000,			0xFFFFFFFF - 1,			AF_WR},	/* entry0: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};


BOOT_RAM_DATA_SECTION
const AF_CFG_TypeDef af4_config[] =					/* GDMA0 */
{
//  Start					End						AccessC
	{0x00000000,			0xFFFFFFFF - 1,			AF_WR},	/* entry0: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};

BOOT_RAM_DATA_SECTION
const AF_CFG_TypeDef af5_config[] =					/* KR4 OTPC */
{
//  Start					End						AccessC
	{0x00000000,			0xFFFFFFFF - 1,			AF_WR},	/* entry0: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry1: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry2: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry3: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
};
