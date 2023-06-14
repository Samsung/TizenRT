/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "ameba_soc.h"

__weak HAL_ROM_TEXT_SECTION
void io_assert_failed(uint8_t *file, uint32_t line)
{
	DBG_8195A("io driver parameters error! file_name: %s, line: %d", file, line);

	for (;;);
}
