/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

SDIOHCFG_TypeDef sdioh_config = {
	.sdioh_bus_speed = SD_SPEED_HS, //SD_SPEED_DS or SD_SPEED_HS
	.sdioh_bus_width = SDIOH_BUS_WIDTH_4BIT, //SDIOH_BUS_WIDTH_1BIT or SDIOH_BUS_WIDTH_4BIT
	.sdioh_cd_pin = _PC_0, // _PNC or gpio pins, e.g. _PC_0
	.sdioh_wp_pin = _PNC, //_PB_31 or _PNC
};

/* Please do not modify this table!!! */
const u8 SDIO_PAD[2][6] = {
	/* CLK     CMD     D0      D1      D2      D3 */
	{_PB_28, _PB_27, _PB_29, _PB_30, _PB_25, _PB_26}, // Group0
	{_PB_17, _PB_16, _PB_18, _PB_19, _PB_14, _PB_15}  // Group1
};

/* sdio pinmux group: 0x0-0x1 */
u8 SDH_Pin_Grp = 0x0;
