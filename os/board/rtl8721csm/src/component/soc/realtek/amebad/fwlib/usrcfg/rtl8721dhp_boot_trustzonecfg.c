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
const TZ_CFG_TypeDef tz_config[]=
{
//  Start					End						NSC
	{0x40000000,			0x50000000-1,			0},	/* entry0: Peripherals NS */
	{0x1010A000,			0x101D4000-1,			0},	/* entry1: IROM & DROM NS */
	{0x100E0000,			0x10100000-1,			0},	/* entry2: BT/WIFI Extention SRAM */

#if defined (configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1U)
	/* Actually Image 3(RDP) enable*/
	{0x00000000,					(u32)__psram_image3_start__- 1,		0},	/* entry3: KM0 SRAM, Retention SRAM, PSRAM_NS */
	{(u32)__psram_image3_end__,		(u32)__ram_image3_start__ -1,		0},	/* entry4: FLASH & SRAM NS */
	{(u32)__ram_image3_end__,		0x1007C000-1,						1},	/* entry5: NSC 4K */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
#else
	{0x00000000,			0x1007C000-1,			0},	/* entry3: KM0 SRAM, Retention SRAM, PSRAM & FLASH & SRAM NS */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry4: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry5: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry6: TODO */
	{0xFFFFFFFF,			0xFFFFFFFF,				0},	/* entry7: TODO */
#endif
};
