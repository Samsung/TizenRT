//+FHDR-------------------------------------------------------------------------
// Copyright (c) 2006, Realtek Semiconductor Corporation
// Realtek's Proprietary/Confidential
//
// All rights reserved. No part of this design may be reproduced or stored
// in a retrieval system, or transmitted, in any form or by any means,
// electronic, mechanical, photocopying, recording, or otherwise,
// without prior written permission of the Realtek Semiconductor Corporation.
// Unauthorized reproduction, duplication, use, or disclosure of this
// design will be deemed as infringement.
//------------------------------------------------------------------------------
// Description:
//   DRAM_CTRL initial flow:
//    >> Info and Timing: CR_MISC -> CR_PCTL
//    >> init start -> check init done
//    >> set tns for txsdll(optional, for exit-self-refresh timing control)
//-FHDR-------------------------------------------------------------------------
#include "ameba_soc.h"
#include "ameba_ddr_param_defs.h"

static const char *TAG = "DDR";
#define FRQC_TEST             0

//////////////////////////////////////////////////
// ROUND_UP function
//////////////////////////////////////////////////
#define ROUND_UP(divider, divisor) (divider%divisor) ? ((divider/divisor)+1) : (divider/divisor)

u8 ddr_init_index(void)
{
	u32 DDRType = DDR_PHY_ChipInfo_ddrtype();
	u32 MemorySize = ChipInfo_MemorySize();

	if (DDRType == DDR_Type_DDR3L) {
		return 3;//rxi316_dram_info_m_ddr3_dev
	} else if (DDRType == DDR_Type_DDR2) {
		if (MemorySize == DDR_Size_512M) {
			return 1; //rxi316_dram_info_m_ddr2_dev_512m
		} else if (MemorySize == DDR_Size_1G) {
			return 0; //rxi316_dram_info_m_ddr2_dev
		}
	}

	while (1) {
		// let the program stuck here
		RTK_LOGE(TAG, "DDR Size is not configured\n");
		DelayMs(10000);
	}
}

//////////////////////////////////////////////////
// dram_ctrl performance tune
//////////////////////////////////////////////////
void rxi316_perf_tune(struct rxi316_dram_device_info *rxi316_dram_info)
{
	DDRC_TypeDef *ddrc = DDRC_DEV;

	uint32_t colu_bfo = 2;
	uint32_t bank_bfo = 2;
	uint32_t row_bfo  = 5;
	uint32_t rank_bfo = 2;
	uint32_t bg_bfo   = 2;
	uint32_t ch_bfo   = 2;
	uint32_t dq_bfo;

	uint32_t bus_addr[32];
	int      i;

	int      colu_remap_bits;
	int      bank_remap_bits; // bank+rank+bg
	int      dram_colu_bits;
	int      dram_bank_bits;  // bank+rank+bg

	uint32_t axi_row0_remap, axi_row1_remap, axi_row2_remap, axi_row3_remap, axi_row4_remap, axi_row5_remap, axi_row6_remap,
			 axi_row7_remap;
	uint32_t axi_row8_remap, axi_row9_remap, axi_row10_remap, axi_row11_remap, axi_row12_remap, axi_row13_remap,
			 axi_row14_remap, axi_row15_remap;
	uint32_t axi_row16_remap;
	uint32_t axi_bank0_remap, axi_bank1_remap, axi_bank2_remap;
	uint32_t axi_colu2_remap, axi_colu3_remap, axi_colu4_remap, axi_colu5_remap, axi_colu6_remap, axi_colu7_remap,
			 axi_colu8_remap, axi_colu9_remap;
	uint32_t axi_colu10_remap, axi_colu11_remap;
	uint32_t axi_rank_remap;
	uint32_t axi_ch_remap;
	uint32_t axi_bg0_remap, axi_bg1_remap;

	uint32_t cr_row0_remap, cr_row1_remap, cr_row2_remap, cr_row3_remap, cr_row4_remap, cr_row5_remap, cr_row6_remap,
			 cr_row7_remap;
	uint32_t cr_row8_remap, cr_row9_remap, cr_row10_remap, cr_row11_remap, cr_row12_remap, cr_row13_remap, cr_row14_remap,
			 cr_row15_remap;
	uint32_t cr_row16_remap;
	uint32_t cr_bank0_remap, cr_bank1_remap, cr_bank2_remap;
	uint32_t cr_colu2_remap, cr_colu3_remap, cr_colu4_remap, cr_colu5_remap, cr_colu6_remap, cr_colu7_remap, cr_colu8_remap,
			 cr_colu9_remap;
	uint32_t cr_colu10_remap, cr_colu11_remap;
	uint32_t cr_rank_remap;
	uint32_t cr_ch_remap;
	uint32_t cr_bg0_remap, cr_bg1_remap;

	if (rxi316_dram_info-> dev-> dq_width == DQ_32) {
		dq_bfo = 2;
	} else if (rxi316_dram_info-> dev-> dq_width == DQ_16) {
		dq_bfo = 1;
	} else if (rxi316_dram_info-> dev-> dq_width == DQ_8) {
		dq_bfo = 0;
	} else { // DQ_48
		dq_bfo = 2;
	}

	bus_addr[0 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_0 ;
	bus_addr[1 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_1 ;
	bus_addr[2 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_2 ;
	bus_addr[3 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_3 ;
	bus_addr[4 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_4 ;
	bus_addr[5 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_5 ;
	bus_addr[6 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_6 ;
	bus_addr[7 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_7 ;
	bus_addr[8 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_8 ;
	bus_addr[9 ]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_9 ;
	bus_addr[10]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_10;
	bus_addr[11]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_11;
	bus_addr[12]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_12;
	bus_addr[13]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_13;
	bus_addr[14]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_14;
	bus_addr[15]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_15;
	bus_addr[16]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_16;
	bus_addr[17]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_17;
	bus_addr[18]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_18;
	bus_addr[19]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_19;
	bus_addr[20]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_20;
	bus_addr[21]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_21;
	bus_addr[22]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_22;
	bus_addr[23]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_23;
	bus_addr[24]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_24;
	bus_addr[25]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_25;
	bus_addr[26]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_26;
	bus_addr[27]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_27;
	bus_addr[28]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_28;
	bus_addr[29]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_29;
	bus_addr[30]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_30;
	bus_addr[31]  = rxi316_dram_info-> dramc_info-> bus_addr_bit -> bus_addr_31;

	// initial valu 32: unremap to bus_addr[31:0]
	axi_row0_remap   = axi_row1_remap   = axi_row2_remap  = axi_row3_remap  =
			axi_row4_remap   = axi_row5_remap   = axi_row6_remap  = axi_row7_remap  =
								   axi_row8_remap   = axi_row9_remap   = axi_row10_remap = axi_row11_remap =
										   axi_row12_remap  = axi_row13_remap  = axi_row14_remap = axi_row15_remap =
												   axi_row16_remap  = 32;
	axi_bank0_remap  = axi_bank1_remap  = axi_bank2_remap = 32;
	axi_colu2_remap  = axi_colu3_remap  = axi_colu4_remap = axi_colu5_remap =
			axi_colu6_remap  = axi_colu7_remap  = axi_colu8_remap = axi_colu9_remap =
								   axi_colu10_remap = axi_colu11_remap = 32;
	axi_rank_remap   = 32;
	axi_ch_remap     = 32;
	axi_bg0_remap    = 32;
	axi_bg1_remap    = 32;

	colu_remap_bits = 0;
	bank_remap_bits = 0; // bank+rank+bg
	dram_colu_bits  = 0;
	dram_bank_bits  = 0; // bank+rank+bg

	// decode row/bank/colu/rank/bg/ch remap to bus_addr[n]
	for (i = 0; i < 32; i++) {
		if (bus_addr[i] == 0) {
			axi_colu2_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 1) {
			axi_colu3_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 2) {
			axi_colu4_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 3) {
			axi_colu5_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 4) {
			axi_colu6_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 5) {
			axi_colu7_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 6) {
			axi_colu8_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 7) {
			axi_colu9_remap = i;
			colu_remap_bits = colu_remap_bits + 1;
		} else if (bus_addr[i] == 8) {
			axi_colu10_remap = i;
			colu_remap_bits  = colu_remap_bits + 1;
		} else if (bus_addr[i] == 9) {
			axi_colu11_remap = i;
			colu_remap_bits  = colu_remap_bits + 1;
		} else if (bus_addr[i] == 10) {
			axi_bank0_remap = i;
			bank_remap_bits = bank_remap_bits + 1;
		} else if (bus_addr[i] == 11) {
			axi_bank1_remap = i;
			bank_remap_bits = bank_remap_bits + 1;
		} else if (bus_addr[i] == 12) {
			axi_bank2_remap = i;
			bank_remap_bits = bank_remap_bits + 1;
		} else if (bus_addr[i] == 13) {
			axi_bg0_remap = i;
			bank_remap_bits = bank_remap_bits + 1;
		} else if (bus_addr[i] == 14) {
			axi_bg1_remap = i;
			bank_remap_bits = bank_remap_bits + 1;
		} else if (bus_addr[i] == 15) {
			axi_rank_remap = i;
			bank_remap_bits = bank_remap_bits + 1;
		} else if (bus_addr[i] == 16) {
			axi_row0_remap = i;
		} else if (bus_addr[i] == 17) {
			axi_row1_remap = i;
		} else if (bus_addr[i] == 18) {
			axi_row2_remap = i;
		} else if (bus_addr[i] == 19) {
			axi_row3_remap = i;
		} else if (bus_addr[i] == 20) {
			axi_row4_remap = i;
		} else if (bus_addr[i] == 21) {
			axi_row5_remap = i;
		} else if (bus_addr[i] == 22) {
			axi_row6_remap = i;
		} else if (bus_addr[i] == 23) {
			axi_row7_remap = i;
		} else if (bus_addr[i] == 24) {
			axi_row8_remap = i;
		} else if (bus_addr[i] == 25) {
			axi_row9_remap = i;
		} else if (bus_addr[i] == 26) {
			axi_row10_remap = i;
		} else if (bus_addr[i] == 27) {
			axi_row11_remap = i;
		} else if (bus_addr[i] == 28) {
			axi_row12_remap = i;
		} else if (bus_addr[i] == 29) {
			axi_row13_remap = i;
		} else if (bus_addr[i] == 30) {
			axi_row14_remap = i;
		} else if (bus_addr[i] == 31) {
			axi_row15_remap = i;
		} else if (bus_addr[i] == 32) {
			axi_row16_remap = i;
		} else if (bus_addr[i] == 33) {
			axi_ch_remap = i;
		}
	}

	dram_colu_bits = rxi316_dram_info-> dev-> dram_colu_bits;
	if (rxi316_dram_info-> dev-> bank == BANK_2) {
		dram_bank_bits = 1;
	} else if (rxi316_dram_info-> dev-> bank == BANK_4) {
		dram_bank_bits = 2;
	} else if (rxi316_dram_info-> dev-> bank == BANK_8) {
		dram_bank_bits = 3;
	} else if (rxi316_dram_info-> dev-> bank == BANK_16) {
		dram_bank_bits = 4;
	} else {
		dram_bank_bits = 3;
	}
	if (colu_remap_bits != (dram_colu_bits - 2)) {
		RTK_LOGE(TAG, "> [ADDR REMAP ERROR]: colu_remap_bits != dram_colu_bits!!!\n");
	}
	if (bank_remap_bits != dram_bank_bits) {
		RTK_LOGE(TAG, "> [ADDR REMAP ERROR]: bank_remap_bits != dram_bank_bits!!!\n");
	}
	//printf("> colu_remap_bits     = %d\n",   colu_remap_bits);
	//printf("> dram_colu_bits      = %d\n",   dram_colu_bits);
	//printf("> bank_remap_bits     = %d\n",   bank_remap_bits);
	//printf("> dram_bank_bits      = %d\n",   dram_bank_bits);

	// decode cr_reg value
	cr_row0_remap   = (axi_row0_remap   != 32) ? (axi_row0_remap   - row_bfo  - dq_bfo - 0) : 15;
	cr_row1_remap   = (axi_row1_remap   != 32) ? (axi_row1_remap   - row_bfo  - dq_bfo - 1) : 15;
	cr_row2_remap   = (axi_row2_remap   != 32) ? (axi_row2_remap   - row_bfo  - dq_bfo - 2) : 15;
	cr_row3_remap   = (axi_row3_remap   != 32) ? (axi_row3_remap   - row_bfo  - dq_bfo - 3) : 15;
	cr_row4_remap   = (axi_row4_remap   != 32) ? (axi_row4_remap   - row_bfo  - dq_bfo - 4) : 15;
	cr_row5_remap   = (axi_row5_remap   != 32) ? (axi_row5_remap   - row_bfo  - dq_bfo - 5) : 15;
	cr_row6_remap   = (axi_row6_remap   != 32) ? (axi_row6_remap   - row_bfo  - dq_bfo - 6) : 15;
	cr_row7_remap   = (axi_row7_remap   != 32) ? (axi_row7_remap   - row_bfo  - dq_bfo - 7) : 15;
	cr_row8_remap   = (axi_row8_remap   != 32) ? (axi_row8_remap   - row_bfo  - dq_bfo - 8) : 15;
	cr_row9_remap   = (axi_row9_remap   != 32) ? (axi_row9_remap   - row_bfo  - dq_bfo - 9) : 15;
	cr_row10_remap  = (axi_row10_remap  != 32) ? (axi_row10_remap  - row_bfo  - dq_bfo - 10) : 15;
	cr_row11_remap  = (axi_row11_remap  != 32) ? (axi_row11_remap  - row_bfo  - dq_bfo - 11) : 15;
	cr_row12_remap  = (axi_row12_remap  != 32) ? (axi_row12_remap  - row_bfo  - dq_bfo - 12) : 15;
	cr_row13_remap  = (axi_row13_remap  != 32) ? (axi_row13_remap  - row_bfo  - dq_bfo - 13) : 15;
	cr_row14_remap  = (axi_row14_remap  != 32) ? (axi_row14_remap  - row_bfo  - dq_bfo - 14) : 15;
	cr_row15_remap  = (axi_row15_remap  != 32) ? (axi_row15_remap  - row_bfo  - dq_bfo - 15) : 15;
	cr_row16_remap  = (axi_row16_remap  != 32) ? (axi_row16_remap  - row_bfo  - dq_bfo - 16) : 15;

	cr_bank0_remap  = (axi_bank0_remap  != 32) ? (axi_bank0_remap  - bank_bfo - dq_bfo) : 31;
	cr_bank1_remap  = (axi_bank1_remap  != 32) ? (axi_bank1_remap  - bank_bfo - dq_bfo) : 31;
	cr_bank2_remap  = (axi_bank2_remap  != 32) ? (axi_bank2_remap  - bank_bfo - dq_bfo) : 31;

	cr_colu2_remap  = (axi_colu2_remap  != 32) ? (axi_colu2_remap  - colu_bfo - dq_bfo - 0) : 15;
	cr_colu3_remap  = (axi_colu3_remap  != 32) ? (axi_colu3_remap  - colu_bfo - dq_bfo - 1) : 15;
	cr_colu4_remap  = (axi_colu4_remap  != 32) ? (axi_colu4_remap  - colu_bfo - dq_bfo - 2) : 15;
	cr_colu5_remap  = (axi_colu5_remap  != 32) ? (axi_colu5_remap  - colu_bfo - dq_bfo - 3) : 15;
	cr_colu6_remap  = (axi_colu6_remap  != 32) ? (axi_colu6_remap  - colu_bfo - dq_bfo - 4) : 15;
	cr_colu7_remap  = (axi_colu7_remap  != 32) ? (axi_colu7_remap  - colu_bfo - dq_bfo - 5) : 15;
	cr_colu8_remap  = (axi_colu8_remap  != 32) ? (axi_colu8_remap  - colu_bfo - dq_bfo - 6) : 15;
	cr_colu9_remap  = (axi_colu9_remap  != 32) ? (axi_colu9_remap  - colu_bfo - dq_bfo - 7) : 15;
	cr_colu10_remap = (axi_colu10_remap != 32) ? (axi_colu10_remap - colu_bfo - dq_bfo - 8) : 15;
	cr_colu11_remap = (axi_colu11_remap != 32) ? (axi_colu11_remap - colu_bfo - dq_bfo - 9) : 15;

	cr_rank_remap   = (axi_rank_remap   != 32) ? (axi_rank_remap   - rank_bfo - dq_bfo) : 31;
	cr_ch_remap     = (axi_ch_remap     != 32) ? (axi_ch_remap     - ch_bfo   - dq_bfo) : 31;
	cr_bg0_remap    = (axi_bg0_remap    != 32) ? (axi_bg0_remap    - bg_bfo   - dq_bfo) : 31;
	cr_bg1_remap    = (axi_bg1_remap    != 32) ? (axi_bg1_remap    - bg_bfo   - dq_bfo) : 31;

	// ----> cr_row_remap0 setting
	ddrc->DDRC_ROW_REMAP0 = (DDRC_ROW0_REMAP(cr_row0_remap) | DDRC_ROW1_REMAP(cr_row1_remap) | \
							 DDRC_ROW2_REMAP(cr_row2_remap) | DDRC_ROW3_REMAP(cr_row3_remap) | \
							 DDRC_ROW4_REMAP(cr_row4_remap) | DDRC_ROW5_REMAP(cr_row5_remap) | \
							 DDRC_ROW6_REMAP(cr_row6_remap) | DDRC_ROW7_REMAP(cr_row7_remap));

	// ----> cr_row_remap1 setting
	ddrc->DDRC_ROW_REMAP1 = (DDRC_ROW8_REMAP(cr_row8_remap) | DDRC_ROW9_REMAP(cr_row9_remap) | \
							 DDRC_ROW10_REMAP(cr_row10_remap) | DDRC_ROW11_REMAP(cr_row11_remap) | \
							 DDRC_ROW12_REMAP(cr_row12_remap) | DDRC_ROW13_REMAP(cr_row13_remap) | \
							 DDRC_ROW14_REMAP(cr_row14_remap) | DDRC_ROW15_REMAP(cr_row15_remap));

	// ----> cr_row_remap2 setting
	ddrc->DDRC_ROW_REMAP2 = DDRC_ROW16_REMAP(cr_row16_remap);

	// ----> cr_bank_remap0 setting
	ddrc->DDRC_BANK_REMAP0 = (DDRC_BA0_REMAP(cr_bank0_remap) | DDRC_BA1_REMAP(cr_bank1_remap) | DDRC_BA2_REMAP(cr_bank2_remap));

	// ----> cr_colu_remap0 setting
	ddrc->DDRC_COLU_REMAP0 = (DDRC_COL2_REMAP(cr_colu2_remap) | DDRC_COL3_REMAP(cr_colu3_remap) | \
							  DDRC_COL4_REMAP(cr_colu4_remap) | DDRC_COL5_REMAP(cr_colu5_remap) | \
							  DDRC_COL6_REMAP(cr_colu6_remap) | DDRC_COL7_REMAP(cr_colu7_remap) | \
							  DDRC_COL8_REMAP(cr_colu8_remap) | DDRC_COL9_REMAP(cr_colu9_remap));

	// ----> cr_colu_remap1 setting
	ddrc->DDRC_COLU_REMAP1 = (DDRC_COL10_REMAP(cr_colu10_remap) | DDRC_COL11_REMAP(cr_colu11_remap));

	// ----> cr_rank_remap0 setting
	ddrc->DDRC_RANK_REMAP0 = DDRC_RA_REMAP(cr_rank_remap);

	// ----> cr_ch_remap0 setting
	ddrc->DDRC_CH_REMAP0   = DDRC_CH_REMAP(cr_ch_remap)  ;

	// ----> cr_bg_remap0 setting
	ddrc->DDRC_BG_REMAP0 = (DDRC_BG0_REMAP(cr_bg0_remap) | DDRC_BG1_REMAP(cr_bg1_remap));

	// ----> cr_slot_park setting
	ddrc->DDRC_SLOT_PARK = (DDRC_IDLE_WINOW(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_idle_window) | \
							DDRC_PERF_PARK(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_wslot_def));

	// ----> cr_slot_starve setting
	ddrc->DDRC_SLOT_STARVE = (DDRC_LPW_STARVE_CYC(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpw_starve) | \
							  DDRC_HPR_STARVE_CYC(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_hpr_starve) | \
							  DDRC_LPR_STARVE_CYC(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_starve) | \
							  DDRC_BASE_STARVE_CYC(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_base_starve));

	// ----> cr_slot_cntr setting
	ddrc->DDRC_SLOT_CNTR = (DDRC_LPW_ISU_LMT(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpw_isu_limit) | \
							DDRC_HPR_ISU_LMT(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_hpr_isu_limit) | \
							DDRC_LPR_ISU_LMT(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_isu_limit));

	// ----> cr_cbctl0 setting
	ddrc->DDRC_CBCTL0 = (DDRC_LPR_FREESLOT_NUM(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_freeslot_num) | \
						 DDRC_LPR_RMW_RUN_LEN(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_rmw_run_len));

	// ----> cr_cbctl1 setting
	ddrc->DDRC_CBCTL1 = (DDRC_DIS_WMG(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_dis_wmg)           |
						 DDRC_PAGEOPEN(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_pageopen)          |
						 DDRC_W_HAZ_EXP_CTRL(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_wmg_num_threshold) |
						 DDRC_LPW_RMW_RUN_LEN(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpw_rmw_run_len) |
						 DDRC_PAGE_HIT_THR_WR(4) | DDRC_PAGE_HIT_THR_WR_EN(1));

	// ----> cr_cbstarve0 setting
	ddrc->DDRC_CBSTARVE0 = (rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_max_starve);

	// ----> cr_cbstarve1 setting
	ddrc->DDRC_CBSTARVE1 = (rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_hpr_max_starve);

	// ----> cr_cbstarve2 setting
	ddrc->DDRC_CBSTARVE2 = (rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpw_max_starve);

	// ----> cr_cbstarve3 setting
	ddrc->DDRC_CBSTARVE3 = (rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_rmw_max_starve);

	// ----> cr_cbstarve4 setting
	ddrc->DDRC_CBSTARVE4 = (rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpw_rmw_max_starve);

	// ----> cr_bank_sch_ctrl setting
	ddrc->DDRC_BSCH = DDRC_BA_SEL_LMT(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_ba_sel_times);

	ddrc->DDRC_HZD_CTRL0 = (ddrc->DDRC_HZD_CTRL0 & ~DDRC_MASK_WHZD_THD) | DDRC_WHZD_THD(4);
	ddrc->DDRC_HZD_CTRL1 = (ddrc->DDRC_HZD_CTRL1 & ~DDRC_MASK_WHZD_CRDT_NUM) | DDRC_WHZD_CRDT_NUM(4);
} // rxi316_perf_tune

//////////////////////////////////////////////////
// dram_ctrl_init
//////////////////////////////////////////////////
void rxi316_dram_init(struct rxi316_dram_device_info *rxi316_dram_info)
{
	// DRAM info
	uint32_t     pctl_period, dfi_rate;
	// CR_DRAM_SIZE
	uint32_t     dram_size;
	// CR_MISC
	uint32_t     page_size;
	enum         rxi316_dram_bank_size    bank_size;
	enum         rxi316_dram_bst_len      bst_size;
	enum         rxi316_hybr_dq_size      hybr_size;
	// CR_WRAP_CHOP_BD
	enum         cr_chop_size      chop_size;
	// CR_DCR
	uint32_t     cr_rank2, cr_wr_dbi, cr_rd_dbi, cr_par, cr_gear_down, cr_bg2, cr_bg_remap, cr_zqc;
	uint32_t     cr_full_wr, cr_rmw_gra;
	// CR_MRx
	uint32_t     dram_bstlen, dram_rl, dram_wl, dram_type;
	// CR_MRINFO
	uint32_t     cas_wr, cas_rd;
	uint32_t     cas_add, mrinfo_add_lat;
	uint32_t     mrinfo_dqsck_max, mrinfo_wr_lat, mrinfo_rd_lat;
	uint32_t     sdram_wr, mrinfo_nwr, mrinfo_rtp;
	// CR_IOCR
	uint32_t     tphy_rd, tphy_wd, tphy_wl, dual_ch_en, addr_share_en, ac_share_en;
	uint32_t     odt_2pins, cke_nop, stc_cke, stc_odt, cmd_2t_en, rtw_2t_dis, half_csn, rd_pipe;
	// CR_DRR
	uint32_t     trfc_tck, trefi_tck, ref_num, ref_dis, zqcl_inv;
	// CR_TPR0~CR_TPR2
	uint32_t     trp_tck, trp_tck_tmp, tras_tck, tras_tck_tmp, twr_tck, trtp_tck, tcke_tck, tpd_en, tzqcs_tck;
	uint32_t     trrd_tck, trc_tck, trcd_tck, tccd_tck, twtr_tck, trtw_tck, tfaw_tck;
	uint32_t     tmrd_tck, zqc_inv, rst_tus_tck, tus_tck, tns_tck;
	// CR_TPR3
	uint32_t     trpab_pb_ps, trpab_pb_tck, tras_msb, tfaw_msb;
	uint32_t     dpin_cmd_lat, trcd_msb, twtr_msb, trtw_msb, trfc_msb, twr_msb, tppd_tck;
	uint32_t     tccd_s_tck, twtr_s_tck, trrd_s_tck, tccd_kw_tck, tccd_r_tck;
	uint32_t     tCCD_K, tCCD_KW;
	// CR_TPR4~CR_TPR5
	uint32_t     trp_msb;
	uint32_t     odt_hold_default, odt_hold, odt_dly;
	// CR_MR0
	uint32_t     mr0_bl;
	enum         rxi316_dram_bst_type     bst_type;
	uint32_t     mr0_cas, mr0_wr, mr0_bit2, mr0_bit12, mr0_bit13;
	// CR_MR1
	uint32_t     dll_en, mr1_bit2, mr1_bl, dram_al, mr1_wr_lpddr23, mr1_wr_lpddr4, rtt_norm;
	// CR_MR2
	uint32_t     mr2_cas, mr2_cwl, mr2_bit4, dyc_odt_en;
	// CR_MR0~CR_MR 22
	uint32_t     mr0_or_val, mr1_or_val, mr2_or_val, mr3_or_val;

	DDRC_TypeDef *ddrc = DDRC_DEV;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	uint32_t ddr_type = DDR_PHY_ChipInfo_ddrtype();

	dfi_rate = 1 << (uint32_t)(rxi316_dram_info->dfi_rate);
	pctl_period = (rxi316_dram_info-> ddr_period_ps) * (dfi_rate); // according DFI_RATE to setting
	dram_type = rxi316_dram_info-> dev-> device_type;
	if (rxi316_dram_info-> dev -> bst_len == BST_LEN_2) {
		dram_bstlen = 2;
		bst_size = BST_LEN_16;
	} else if (rxi316_dram_info-> dev -> bst_len == BST_LEN_4) {
		dram_bstlen = 4;
		bst_size  = BST_LEN_2;
	} else if (rxi316_dram_info-> dev -> bst_len == BST_LEN_8) {
		dram_bstlen = 8;
		bst_size  = BST_LEN_4;
	} else { // BST_16
		dram_bstlen = 16;
		bst_size  = BST_LEN_8;
	}
	dram_wl = rxi316_dram_info-> mode_reg-> dram_wr_lat;
	dram_rl = rxi316_dram_info-> mode_reg-> dram_rd_lat;
	dram_al = rxi316_dram_info-> mode_reg-> dram_add_lat;

	// ----> cr_dram_size setting
	dram_size = 0xffffffff;
	ddrc->DDRC_DRAM_SIZE = DDRC_DRAM_SIZE(dram_size);

	// ----> cr_misc setting
	//page_size = (rxi316_dram_info-> dev-> page);
	if (rxi316_dram_info-> dev-> dq_width == DQ_32) {
		page_size = (rxi316_dram_info-> dev-> dram_colu_bits) + 2 - 8;
	} else if (rxi316_dram_info-> dev-> dq_width == DQ_16) {
		page_size = (rxi316_dram_info-> dev-> dram_colu_bits) + 1 - 8;
	} else if (rxi316_dram_info-> dev-> dq_width == DQ_8) {
		page_size = (rxi316_dram_info-> dev-> dram_colu_bits) + 0 - 8;
	} else {
		page_size = (rxi316_dram_info-> dev-> dram_colu_bits) + 2 - 8;
	}
	bank_size = (rxi316_dram_info-> dev-> bank);
	hybr_size = (rxi316_dram_info-> dev-> hybr_size);

	ddrc->DDRC_MISC = (DDRC_PAGE_SIZE(page_size) | DDRC_BANK_SIZE(bank_size) | DDRC_BST_SIZE(bst_size) | DDRC_HYBR_SIZE(hybr_size));

	// ----> cr_wrap_chop_bd setting
	chop_size = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->chop_size;
	ddrc->DDRC_CHOP_SIZE = DDRC_CR_CHOP_SIZE(chop_size);

	// ----> cr_dcr setting
	dram_type    = (rxi316_dram_info-> dev-> device_type);
	cr_rank2     = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->rank2;
	cr_wr_dbi    = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->wr_dbi;
	cr_rd_dbi    = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->rd_dbi;
	cr_par       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->par;
	cr_gear_down = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->gear_down;
	cr_bg2       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->bg2;
	cr_bg_remap  = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->bg_remap;
	cr_zqc       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->zqc;
	cr_full_wr   = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->full_wr;
	cr_rmw_gra   = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->rmw_gra;
	ddrc->DDRC_DCR = (DDRC_DRAM_TYPE(dram_type) | DDRC_DFI_RATE(dfi_rate) | DDRC_RANK2(cr_rank2) | DDRC_WR_DBI(cr_wr_dbi) | \
					  DDRC_RD_DBI(cr_rd_dbi) | DDRC_PAR(cr_par) | DDRC_GEAR_DOWN(cr_gear_down) | DDRC_BG2(cr_bg2) | \
					  DDRC_BG_UNMAP(cr_bg_remap) | DDRC_RMW_GRA(cr_rmw_gra) | DDRC_FULLWR(cr_full_wr) | DDRC_ZQC(cr_zqc));

	if (DQ_32 == rxi316_dram_info-> dev-> dq_width) {
		ddrc->DDRC_DCR |= DDRC_BIT_DQ_32;
	} else if (DQ_16 == rxi316_dram_info-> dev-> dq_width) {
		ddrc->DDRC_DCR |= DDRC_BIT_HALF_DQ;
	} else if (HYBR_DQ == rxi316_dram_info-> dev-> dq_width) {
		ddrc->DDRC_DCR |= DDRC_BIT_HYBR_DQ;
	} else if (DQ_8 == rxi316_dram_info-> dev-> dq_width) {
		ddrc->DDRC_DCR |= DDRC_BIT_QUAR_DQ;
	}

#if FRQC_TEST
	uint32_t cr_gp_num;
	cr_gp_num = 1;
gp_reg_config:
#endif
	// ----> cr_iocr setting
	odt_2pins     = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->odt_2pins;
	cke_nop       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->cke_nop;
	stc_cke       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->stc_cke;
	stc_odt       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->stc_odt;
	cmd_2t_en     = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->cmd_2t_en;
	rtw_2t_dis    = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->rtw_2t_dis;
	half_csn      = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->half_csn;
	rd_pipe       = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->rd_pipe;
	tphy_wl       = 0; // not support in rtk_phy
	tphy_wd       = ROUND_UP(rxi316_dram_info->mode_reg-> tphy_wd, dfi_rate);
	tphy_rd       = ROUND_UP(rxi316_dram_info->mode_reg-> tphy_rd, dfi_rate);
	dual_ch_en    = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->dual_ch_en;
	addr_share_en = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->addr_share_en;
	ac_share_en   = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->ac_share_en;

	ddrc->DDRC_IOCR = (DDRC_ODT_2PIN(odt_2pins) | DDRC_CKE_NOP(cke_nop) | DDRC_STC_CKE(stc_cke) | DDRC_STC_ODT(stc_odt) | \
					   DDRC_2N_PRE_CMD(cmd_2t_en) | DDRC_TRTW_2T_DIS(rtw_2t_dis) | DDRC_HALF_CS_N(half_csn) | DDRC_RD_PIPE(rd_pipe) | \
					   DDRC_TPHY_WDATA(tphy_wd) | DDRC_TPHY_WLAT(tphy_wl) | DDRC_TPHY_RDATA_EN(tphy_rd) | DDRC_DC_CH1(dual_ch_en) | \
					   DDRC_ADDR_SHARE(addr_share_en) | DDRC_AC_SHARE(ac_share_en));

	// ----> cr_mr0 setting
	mr0_bl       = 0;
	mr0_bit2     = 0;
	mr0_bit12    = 0;
	mr0_bit13    = 0;
	mr0_cas      = 0;
	mr0_wr       = 0;
	mr0_or_val   = rxi316_dram_info-> mode_reg-> mr0_or_val;
	if (dram_bstlen == 16) {
		if ((rxi316_dram_info-> dev-> device_type) == LPDDR_1) {
			mr0_bl  = 4;
		}
	} else if (dram_bstlen == 8) {
		if ((rxi316_dram_info-> dev-> device_type) == DDR_2 ||
			(rxi316_dram_info-> dev-> device_type) == LPDDR_1) {
			mr0_bl  = 3;
		} else {
			mr0_bl  = 0;
		}
	} else if (dram_bstlen == 4) {
		if ((rxi316_dram_info-> dev-> device_type) == DDR_3 ||
			(rxi316_dram_info-> dev-> device_type) == DDR_2 ||
			(rxi316_dram_info-> dev-> device_type) == LPDDR_1) {
			mr0_bl  = 2;
		}
	} else if (dram_bstlen == 2) {
		if ((rxi316_dram_info-> dev-> device_type) == DDR_2 ||
			(rxi316_dram_info-> dev-> device_type) == LPDDR_1) {
			mr0_bl  = 1;
		}
	}

	bst_type = (rxi316_dram_info-> dev-> bst_type);

	if ((rxi316_dram_info-> dev-> device_type) == DDR_2 ||
		(rxi316_dram_info-> dev-> device_type) == LPDDR_1) {
		mr0_cas  = dram_rl;
	} else if ((rxi316_dram_info-> dev-> device_type) == DDR_3) {
		if (dram_rl < 5) {
			mr0_bit2 = 0;
			mr0_cas  = 1;
		} else {
			if (dram_rl < 12) {
				mr0_bit2 = 0;
				mr0_cas = dram_rl - 4;
			} else {
				mr0_bit2 = 1;
				mr0_cas = dram_rl - 12;
			}
		}
	}

	sdram_wr = rxi316_dram_info-> timing-> nWR;
	if ((rxi316_dram_info-> dev-> device_type) == DDR_2) { // nWR setting
		if ((sdram_wr  >= 2) && (sdram_wr  <= 8)) { // 2~8
			mr0_wr = sdram_wr - 1 ;
		}
	} else if ((rxi316_dram_info-> dev-> device_type) == DDR_3) {
		if (sdram_wr  < 5) {  // 5
			mr0_wr = 1;
		} else if ((sdram_wr  >= 5) && (sdram_wr  <= 8)) { // 5~8
			mr0_wr = sdram_wr  - 4;
		} else if ((sdram_wr  >= 9) && (sdram_wr <= 14)) { // 9~14
			mr0_wr = sdram_wr / 2 ;
		} else { // 15, 16
			mr0_wr = 0;
		}
	}
	// LPDDR1 without nWR

	if ((rxi316_dram_info-> dev-> device_type) != DDR_2) {
		mr0_bit12 = 1;
	}

	ddrc->DDRC_MR0 = (
						 (mr0_bl      << PCTL_MR0_BL_BFO)        |
						 (mr0_bit2    << 2)                      |
						 (bst_type    << PCTL_MR0_BST_TYPE_BFO)  |
						 (mr0_cas     << PCTL_MR0_CAS_BFO)       |
						 (mr0_wr      << PCTL_MR0_WR_BFO)        |
						 (mr0_bit12   << 12)                     |
						 (mr0_bit13   << 13)                     |
						 (mr0_or_val)
					 ); //DDRC_MR0(x)

	dll_en   = 0; // 1:enable DLL, 0:disable
	mr1_bit2 = 0;
	mr1_bl   = 0;
	mr1_wr_lpddr23 = 0;
	mr1_wr_lpddr4  = 0;
	rtt_norm   = 0; // Enable DDR4 RTT_NORM for RZQ/4
	mr1_or_val = rxi316_dram_info-> mode_reg-> mr1_or_val;
	dll_en   = 0; // 1:enable DLL, 0:disable

	// nWR setting
	sdram_wr = rxi316_dram_info-> timing-> nWR;

	ddrc->DDRC_MR1 = (
						 (dll_en          << PCTL_MR1_DLL_EN_BFO)   |    // mr1[0]
						 (mr1_bit2        << 2)   |                      // mr1[2]
						 (mr1_bl          << PCTL_MR1_BL_BFO)   |        // mr1[2:0]
						 (dram_al         << PCTL_MR1_ADD_LAT_BFO)   |   // mr1[4:3]
						 (mr1_wr_lpddr23  << PCTL_MR1_WR_LP23_BFO)   |   // mr1[7:5]
						 (mr1_wr_lpddr4   << PCTL_MR1_WR_LP4_BFO)   |    // mr1[6:4]
						 (rtt_norm        << PCTL_MR1_RTT_NORM_EN_BFO) | // mr1[10:8]
						 (mr1_or_val)
					 );

	// ----> cr_mr2 setting
	mr2_cas    = 0;
	mr2_cwl    = 0;
	mr2_bit4   = 0;
	dyc_odt_en = 0;
	mr2_or_val = rxi316_dram_info-> mode_reg-> mr2_or_val;
	if ((rxi316_dram_info-> dev-> device_type) == DDR_3) {
		mr2_cwl = dram_wl - 5;
	}

	if ((rxi316_dram_info-> dev-> device_type) == LPDDR_1 || (rxi316_dram_info-> dev-> device_type) == DDR_2 ||
		(rxi316_dram_info-> dev-> device_type) == DDR_3) {
		if ((rxi316_dram_info-> dev-> device_type) == DDR_2) {
			dyc_odt_en = 0; // Disable dynamic odt
		} else {
			if (rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->stc_odt) {
				dyc_odt_en = 0; // Disable dynamic odt
			} else {
				dyc_odt_en = 1; // Enable dynamic odt
			}
		}
	}
	ddrc->DDRC_MR2 = (
						 (mr2_cas         << PCTL_MR2_CAS_BFO) |          // mr2[3:0]
						 (mr2_cwl         << PCTL_MR2_CWL_BFO) |          // mr2[5:3]
						 (mr2_bit4        << 4) |                         // mr2[4]
						 (dyc_odt_en      << PCTL_MR2_DYC_ODT_EN_BFO) |   // mr2[10:9]
						 (mr2_or_val)
					 );

	// ----> cr_mr3 setting
	mr3_or_val = rxi316_dram_info-> mode_reg-> mr3_or_val;

	ddrc->DDRC_MR3 = mr3_or_val;

	// ----> cr_mrinfo setting
	mrinfo_dqsck_max = 0;

	cas_wr = (rxi316_dram_info-> mode_reg-> dram_wr_lat) +
			 (rxi316_dram_info-> mode_reg-> dram_par_lat);

	cas_rd = (rxi316_dram_info-> mode_reg-> dram_rd_lat) +
			 (rxi316_dram_info-> mode_reg-> dram_par_lat);

	cas_add = (rxi316_dram_info-> mode_reg-> dram_add_lat);

	mrinfo_wr_lat  = ROUND_UP(cas_wr, dfi_rate);
	mrinfo_rd_lat  = ROUND_UP(cas_rd, dfi_rate);
	mrinfo_add_lat = ROUND_UP(cas_add, dfi_rate);

	// nWR setting
	sdram_wr = rxi316_dram_info-> timing-> nWR;
	if ((rxi316_dram_info-> dev-> device_type) == DDR_2   || // sdram_wr= 2 ~ 8
		(rxi316_dram_info-> dev-> device_type) == DDR_3) { // sdram_wr= 5 ~ 8,10,12,14,16
		// nWR setting
		mrinfo_nwr = ROUND_UP(sdram_wr, dfi_rate);
	} else { // LPDDR_1
		mrinfo_nwr = ROUND_UP(rxi316_dram_info-> timing-> twr_ps, pctl_period);
	}

	// RTP setting
	if ((rxi316_dram_info-> dev-> device_type) == LPDDR_1) { // RTP settig
		mrinfo_rtp = (dram_bstlen / 2) / dfi_rate;
	} else if ((rxi316_dram_info-> dev-> device_type) == DDR_2) { // RTP settig
		mrinfo_rtp = ROUND_UP(((((dram_bstlen / 2) - 2) / dfi_rate) + (rxi316_dram_info->timing->trtp_ps)), pctl_period);
	} else { // DDR3
		mrinfo_rtp = ROUND_UP(rxi316_dram_info->timing->trtp_ps, pctl_period);
	}

	ddrc->DDRC_MR_INFO = (DDRC_WR_LAT(mrinfo_wr_lat) | DDRC_RD_LAT(mrinfo_rd_lat) | DDRC_ADD_LAT(mrinfo_add_lat) | \
						  DDRC_T_DQSCK_MAX(mrinfo_dqsck_max) | DDRC_NWR(mrinfo_nwr) | DDRC_RTP(mrinfo_rtp));

	// ----> cr_drr setting
	trfc_tck = ROUND_UP(rxi316_dram_info->timing->trfc_ps, pctl_period);
	trefi_tck = (ROUND_UP(rxi316_dram_info->timing->trefi_ps, pctl_period)) - 200;

	ref_num = 1;
	ref_dis  = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->ref_dis;
	zqcl_inv = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->zqcl_inv;

	ddrc->DDRC_DRR = (DDRC_TRFC(trfc_tck) | DDRC_TREF(trefi_tck) | DDRC_REF_NUM(ref_num) | DDRC_REF_DIS(ref_dis) | \
					  DDRC_ZQCL_INV(zqcl_inv));

	// ----> cr_tpr0 setting
	tras_tck = ROUND_UP(rxi316_dram_info->timing->tras_ps, pctl_period);
	twr_tck  = ROUND_UP(rxi316_dram_info->timing->twr_ps, pctl_period);
	trp_tck  = ROUND_UP(rxi316_dram_info->timing->trp_ps, pctl_period);
	trtp_tck = ROUND_UP(rxi316_dram_info->timing->trtp_ps, pctl_period);

#if DDR_AUTOGATING
	// for enable cts fifo, which will increase latency between phy and controller
	tcke_tck = 0xA;
#else
	if (rxi316_dram_info-> timing-> tcke_ps > rxi316_dram_info-> timing-> txp_ps) {
		tcke_tck = ROUND_UP(rxi316_dram_info->timing->tcke_ps, pctl_period);
	} else {
		tcke_tck = ROUND_UP(rxi316_dram_info->timing->txp_ps, pctl_period);
	}
#endif

	tpd_en    = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->tpd_en;
	tzqcs_tck = ROUND_UP(rxi316_dram_info->timing->tzqcs_tck, dfi_rate);

	ddrc->DDRC_TPR0 = (DDRC_TRP(trp_tck) | DDRC_TRAS(tras_tck) | DDRC_TWR(twr_tck) | DDRC_TRTP(trtp_tck) | \
					   DDRC_TCKE(tcke_tck) | DDRC_TPD_EN(tpd_en) | DDRC_TZQCS(tzqcs_tck));

	// ----> cr_tpr1 setting
	// trrd,trc,trcd,tccd,twtr,trtw,tfaw_lsb
	trrd_tck     = ROUND_UP(rxi316_dram_info->timing->trrd_ps, pctl_period);
	tras_tck_tmp = ROUND_UP(rxi316_dram_info->timing->tras_ps, pctl_period);
	trp_tck_tmp  = ROUND_UP(rxi316_dram_info->timing->trp_ps, pctl_period);
	trc_tck      = ROUND_UP(rxi316_dram_info->timing->trc_ps, pctl_period);
	if ((tras_tck_tmp + trp_tck_tmp) > trc_tck) {
		trc_tck = tras_tck_tmp + trp_tck_tmp;
	}

	trcd_tck = (ROUND_UP(rxi316_dram_info->timing->trcd_ps, pctl_period)) - 1; // cmd_sch: bank_sch more 1t
	tccd_tck = ROUND_UP(rxi316_dram_info->timing->tccd_tck, dfi_rate);
	twtr_tck = ROUND_UP(rxi316_dram_info->timing->twtr_tck, dfi_rate);

	// trtw expression
	// ==== trtw clock cycles in dram domain
	if (dram_type == LPDDR_1) {
		trtw_tck = dram_bstlen / 2 + dram_rl;
	} else if (dram_type == DDR_2) {
		trtw_tck = dram_bstlen / 2 + 2;
	} else if (dram_type == DDR_3) {
		trtw_tck = dram_rl + dram_bstlen / 2 + 2 - dram_wl;
	} else { // SDR or Others
		trtw_tck = dram_rl + dram_bstlen;
	}
	trtw_tck = ROUND_UP(trtw_tck, dfi_rate);

	tfaw_tck = ROUND_UP(rxi316_dram_info->timing->tfaw_ps, pctl_period);

	ddrc->DDRC_TPR1 = (DDRC_TRRD(trrd_tck) | DDRC_TRC(trc_tck) | DDRC_TRCD(trcd_tck) | DDRC_TCCD(tccd_tck) | \
					   DDRC_TWTR(twtr_tck) | DDRC_TRTW(trtw_tck) | DDRC_TFAW(tfaw_tck));

	// ----> cr_tpr2 setting
	// trrd,trc,trcd,tccd,twtr,trtw,tfaw_lsb
	if (((rxi316_dram_info-> dev-> device_type) == DDR_2) || // DDR2/LPDDR1
		((rxi316_dram_info-> dev-> device_type) == LPDDR_1)) {
		zqc_inv = 2; // init refresh num or zq short cmd interval times(by enable zqc:dcr[30])
	} else {
		zqc_inv = rxi316_dram_info->dramc_info->rxi316_dram_ctrl_reg_common->zqc_inv; // init refresh num or zq short cmd interval times(by enable zqc:dcr[30])
	}
	if ((rxi316_dram_info-> dev-> device_type) == DDR_3) {
		tmrd_tck = rxi316_dram_info->timing->tmrd_tck;
	} else if ((rxi316_dram_info-> dev-> device_type) == DDR_2) {
		tmrd_tck = ROUND_UP(rxi316_dram_info->timing->tmrd_tck, pctl_period);
	} else {
		tmrd_tck = ROUND_UP(rxi316_dram_info->timing->tmrd_tck, dfi_rate);
	}

	rst_tus_tck = 0;
	tus_tck 	= 0;
	tns_tck 	= 0;
	// rst_tus, tus, tns
	if (dram_type == DDR_2) { // DDR2
		rst_tus_tck = 0;
		tus_tck     = ((200000000 / pctl_period) >> 10) + 1;      // 200us
		tns_tck     = ((400000   / pctl_period) >> 7) + 1;        // 400ns
	} else if (dram_type == DDR_3) {
		rst_tus_tck = ((200000000 / pctl_period) >> 10) + 1;      // 200us
		tus_tck     = ((500000000 / pctl_period) >> 10) + 1;      // 500us
		tns_tck     = ((trfc_tck + (10000 / pctl_period)) >> 7) + 1; // max (5tck, tRFC+10ns)
	} else if (dram_type == LPDDR_1) {
		rst_tus_tck = 0;
		tus_tck     = ((200000000 / pctl_period) >> 10) + 1; // 200us
		tns_tck     = 0;
	}

	ddrc->DDRC_TPR2 = (DDRC_INIT_REF_NUM_ZQC_INV(zqc_inv) | DDRC_TMRD_INIT_MRR(tmrd_tck) | DDRC_TNS(tns_tck) | \
					   DDRC_TUS(tus_tck) | DDRC_RST_TUS(rst_tus_tck));
	// ----> cr_tpr3 setting
	// trpab_pb,tras,tfaw,dpin_cmd_lat,tppd,tccd_s,twtr_s,tccd_r
	trpab_pb_ps = 0;

	trpab_pb_tck = ROUND_UP(trpab_pb_ps, pctl_period);

	tras_msb = (tras_tck & 0x20) >> 5; // tras_msb, mask tras lsb[4:0]

	tfaw_tck = ROUND_UP(rxi316_dram_info->timing->tfaw_ps, pctl_period);
	tfaw_msb = (tfaw_tck & 0x20) >> 5; // tfaw_msb, mask tfaw_lsb[4:0]

	dpin_cmd_lat = 0;

	trcd_msb = (trcd_tck & 0x10)  >> 4; // trcd_msb, mask trcd_lsb[3:0]
	twtr_msb = (twtr_tck & 0x8)   >> 3; // twtr_msb, mask twtr_lsb[2:0]
	trtw_msb = (trtw_tck & 0x10)  >> 4; // trtw_msb, mask trtw_lsb[3:0]
	trfc_msb = (trfc_tck & 0x100) >> 8; // trfc_msb, mask trfc_lsb[7:0]
	twr_msb  = (twr_tck  & 0x10)  >> 4; // twr msb

	tppd_tck = 0;

	tccd_s_tck = ROUND_UP(rxi316_dram_info->timing->tccd_s_tck, dfi_rate);
	twtr_s_tck = ROUND_UP(rxi316_dram_info->timing->twtr_s_tck, dfi_rate);
	trrd_s_tck = ROUND_UP(rxi316_dram_info->timing->trrd_s_tck, dfi_rate);
	tCCD_K = 0;
	// tccd_r: control rank tccd timing(for dqs preamable confict), using tccd_tck + 1
	if ((dram_bstlen / dfi_rate) > rxi316_dram_info->timing->tccd_tck) {
		trrd_s_tck = ROUND_UP(((dram_bstlen / dfi_rate) - rxi316_dram_info->timing->tccd_tck),  dfi_rate);
	} else {
		tCCD_K = 1;
	}
	tCCD_KW = 0;
	if (cr_rank2) {
		tccd_r_tck = tCCD_K;
		tccd_kw_tck = tCCD_KW;
	} else {
		tccd_r_tck = 0;
		tccd_kw_tck = 0;
	}

	ddrc->DDRC_TPR3 = (DDRC_TRPAB_PB(trpab_pb_tck) | DDRC_TRAS_MSB(tras_msb) | DDRC_TFAW_MSB(tfaw_msb) | DDRC_DPIN_CMD_LAT(dpin_cmd_lat) | \
					   DDRC_TRCD_MSB(trcd_msb) | DDRC_TWTR_MSB(twtr_msb) | DDRC_TRTW_MSB(trtw_msb) | DDRC_TRFC_MSB(trfc_msb) | \
					   DDRC_TWR_MSB(twr_msb) | DDRC_TPPD(tppd_tck) | DDRC_TCCD_S(tccd_s_tck) | DDRC_TWTR_S(twtr_s_tck) | \
					   DDRC_TRRD_S(trrd_s_tck) | DDRC_TCCD_KW(tccd_kw_tck) | DDRC_TCCD_K(tccd_r_tck));

	// ----> cr_tpr4 setting
	trp_msb = (trp_tck & 0x10)  >> 4; // trp_msb, mask trp_lsb[3:0]
	ddrc->DDRC_TPR4 = DDRC_TRP_MSB(trp_msb);

	// ----> cr_tpr5 setting
	odt_hold_default  = dram_bstlen / 2 + 2;
	odt_dly           = 0;

	if (dfi_rate == 1) {
		odt_hold = odt_hold_default;
	} else { // dfi2 use 2tpreabmle
		odt_hold = odt_hold_default + 1;
	}

	ddrc->DDRC_TPR5 = (DDRC_ODT_HOLD(odt_hold) | DDRC_ODT_DLY(odt_dly));

#if FRQC_TEST
	if (cr_gp_num == 1) {
		printf("/////////  Group 1 control reg start setting  /////////\n");
		cr_gp_num = 0;
		goto gp_reg_config;
	}
#endif

	// ----> cr_imr setting
	ddrc->DDRC_IMR = DDRC_INTR_MASK(0xFFFF);

	// set all_mode idle
	ddrc->DDRC_CSR = (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE | DDRC_BIT_MEM_IDLE);

	//dynamic SREX
	ddrc->DDRC_DPERF0 |= DDRC_DIW(0xFFFF);
	//ddrc->DDRC_IOCR |= DDRC_DYN_SRE(1);

	if (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG) & LSYS_BIT_BOOT_WAKE_FROM_PS_HS) {
		ddrc->DDRC_DRR |= DDRC_REF_DIS(ENABLE);
		// start to init
		/* ddrc drive cke here --> low */
		ddrc->DDRC_CCR = DDRC_BIT_CR_UPDATE;

		// enable PWDPAN15N when phy and controller ready
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  | LSYS_BIT_PWDPAD15N_DQ | LSYS_BIT_PWDPAD15N_CA));

		/* release rst and tcke for controller */
		if (ddr_type == DDR_Type_DDR2) {
			ddr_phy->DDRPHY_CRT_CTL = 0x44000103; //according to ddr3 type

		} else {
			ddr_phy->DDRPHY_CRT_CTL = 0x44000021; //according to ddr3 type
		}

		uint32_t     temp;
		//ddrc->DDRC_DRR |= DDRC_REF_DIS(ENABLE);
		temp = ddrc->DDRC_CSR;
		temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
		temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_RSVD));
		ddrc->DDRC_CSR = temp;

		ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SHIGH) |
							   DDRC_CAS_N_NDGE(DDR_SHIGH) |	DDRC_WE_N_NDGE(DDR_SHIGH) | \
							   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));
		ddrc->DDRC_TIE_DPIN = (DDRC_ODT(DDR_SLOW) | DDRC_CKE(DDR_SHIGH) | DDRC_RST_N(DDR_SHIGH));

		ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);

		while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);
		//ddrc->DDRC_DRR &= (~DDRC_BIT_REF_DIS);
		DelayUs(1);

		/* refresh */
		temp = ddrc->DDRC_CSR;
		temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
		temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_RSVD));
		ddrc->DDRC_CSR = temp;

		ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SLOW) |
							   DDRC_CAS_N_NDGE(DDR_SLOW) |  DDRC_WE_N_NDGE(DDR_SHIGH) | \
							   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));
		ddrc->DDRC_TIE_DPIN = (DDRC_ODT(DDR_SLOW) | DDRC_CKE(DDR_SHIGH) | DDRC_RST_N(DDR_SHIGH));

		ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
		while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);

		/* deselect better */
		temp = ddrc->DDRC_CSR;
		temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
		temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_RSVD));
		ddrc->DDRC_CSR = temp;

		ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SHIGH) | DDRC_RAS_N_NDGE(DDR_SLOW) |
							   DDRC_CAS_N_NDGE(DDR_SLOW) |  DDRC_WE_N_NDGE(DDR_SHIGH) | \
							   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));

		ddrc->DDRC_TIE_DPIN &= (~DDRC_BIT_CKE);
		ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
		while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);
	} else {
		// start to init
		ddrc->DDRC_CCR = DDRC_BIT_INIT;
		// wait init_done: read cr_ccr[0] == 1
		while (((ddrc->DDRC_CCR) & DDRC_BIT_INIT) != DDRC_BIT_INIT);
	}

	// enter mem_mode
	ddrc->DDRC_CSR = (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE);

	ddrc->DDRC_DRR |= DDRC_TREF(trefi_tck);
	ddrc->DDRC_DRR &= (~DDRC_BIT_REF_DIS);

	// start to init
	ddrc->DDRC_CCR = DDRC_BIT_CR_UPDATE;

	// check mode state
	while (((ddrc->DDRC_CSR) & 0x7ff) != (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE));

#if FRQC_TEST
	// set all_mode idle
	ddrc->DDRC_CSR = (DDRC_BIT_CR_GP | DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE | DDRC_BIT_MEM_IDLE); // set gp1
	// enter mem_mode
	ddrc->DDRC_CSR = (DDRC_BIT_CR_GP | DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE); // set gp1
	// set dpin cmd issue self_refresh
	ddrc->DDRC_CMD_DPIN = (DDRC_BIT_ACT_N | DDRC_BIT_WE_N); // cr_reg 0x28
	ddrc->DDRC_CMD_DPIN_NDGE = (DDRC_BIT_ACT_N_NDGE | DDRC_BIT_WE_N_NDGE); // cr_reg 0x24
	// set frqc start
	ddrc->DDRC_CCR = DDRC_BIT_FRQC;
	// check frqc done: CCR[5]==1
	while (((ddrc->DDRC_CCR) & DDRC_BIT_FRQC) != DDRC_BIT_FRQC);
#endif // FRQC_TEST

} // rxi316_dram_init

/**
* @brief  This function is used to turn on ddr frontend ar & aw channel.
* @param  none
* @retval none
*/
void rxi316_ftend_init(void)
{
	DDRCF_TypeDef *ddrc_ftend = DDRCF_DEV;
	struct rxi316_dram_device_info *rxi316_dram_info;

	ddrc_ftend->Port[0].DDRC_CFG_Px_PORTCTRL = 0;
	ddrc_ftend->Port[1].DDRC_CFG_Px_PORTCTRL = 0;
	ddrc_ftend->Port[2].DDRC_CFG_Px_PORTCTRL = 0;

	rxi316_dram_info = rxi316_dram_info_m[ddr_init_index()];
	/*CBCTL0[LPR_FREESLOT_NUM] < CFG_CM_FE_RRB[max_num_lpr_entries_ch]*/
	ddrc_ftend->DDRC_CFG_CM_FE_RRB = (ddrc_ftend->DDRC_CFG_CM_FE_RRB & ~DDRC_MASK_MAX_NUM_LPR_ENTRIES_CH0) |
									 DDRC_MAX_NUM_LPR_ENTRIES_CH0(rxi316_dram_info-> dramc_info-> slot_starve_reg -> cr_lpr_freeslot_num + 1);
}

/**
  * @brief  Enable/Disable DDRC dynamic self-refresh for power saving.
  * @param  Enable: Enable or Disable dynamic self-refresh function.
  * @param  IDleCnt: idle counter setting, unit DDRC CLK.
  * @retval None
  */
void rxi316_DynSre_init(u32 IdleCnt, u32 state)
{
	DDRC_TypeDef *ddrc = DDRC_DEV;

	if (state == ENABLE) {
		ddrc->DDRC_DPERF0 &= (~DDRC_MASK_DIW);

		ddrc->DDRC_DPERF0 |= DDRC_DIW(IdleCnt);
		ddrc->DDRC_IOCR |= DDRC_BIT_DYN_SRE;

	} else {
		ddrc->DDRC_IOCR &= (~DDRC_BIT_DYN_SRE);

	}
	ddrc->DDRC_CMD_DPIN_NDGE = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SLOW) |
								DDRC_CAS_N_NDGE(DDR_SLOW) |  DDRC_WE_N_NDGE(DDR_SHIGH) | \
								DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));
	ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SLOW) |
						   DDRC_CAS_N_NDGE(DDR_SLOW) |  DDRC_WE_N_NDGE(DDR_SHIGH) | \
						   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));

	ddrc->DDRC_TPR2 = (((ddrc->DDRC_TPR2) & (~ DDRC_MASK_TNS)) | DDRC_TNS(0x2));

	/* update IOCR reg */
	ddrc->DDRC_CCR = DDRC_BIT_CR_UPDATE;

}

/**
* @brief  This function is used to detect dram size.
* @param  none
* @retval detect result
* @        1: mapping error
* @        0: mapping ok
* @note: to detect the ddr density, should not open DDR2_512M_DRAM/DDR2_256M_DRAM macro
*/
u32 ddr_size_detect(void)
{
	/*the actual application size is no more than 2GB, according to spec, the PAGE size is no more than 2MB*/
	/*the DQ is restrict to 16, so Column bit is no more than 10 bits*/
	/*firstly we use ddr2_dq16_bus_addr_remap_info info to detect*/
	/* now DDR2 have three density: 256Mb��? 512Mb, and 1Gb*/

	HAL_WRITE32(DDR_BASE, 0, 0);
	HAL_WRITE32(DDR_BASE, 0x400, 0x400);

	DCache_CleanInvalidate(DDR_BASE, (0x400 + 32));

	if (HAL_READ32(DDR_BASE, 0) == 0x400) {
		RTK_LOGW(TAG, "DDR Density is 256Mbit\n");
		RTK_LOGW(TAG, "Please open DDR2_256M_DRAM macro\n");
		return 1;
	}

	if (HAL_READ32(DDR_BASE, 0) == 0x0) {
		HAL_WRITE32(DDR_BASE, 0x2000, 0x2000);
		DCache_CleanInvalidate(DDR_BASE, (0x2000 + 32));

		if (HAL_READ32(DDR_BASE, 0) == 0x2000) {
			RTK_LOGW(TAG, "DDR Density is 512Mbit\n");
			RTK_LOGW(TAG, "Please open DDR2_512M_DRAM macro\n");
			return 1;
		}
		if (HAL_READ32(DDR_BASE, 0) == 0x0) {
			HAL_WRITE32(DDR_BASE, 0x8000000, 0x8000000);
			DCache_CleanInvalidate(DDR_BASE, (0x8000000 + 32));
			if (HAL_READ32(DDR_BASE, 0) == 0x8000000) {
				RTK_LOGI(TAG, "DDR Density is 1Gbit\n");
				return 0;
			}
		}
	}
	RTK_LOGW(TAG, "DDR Density is unknown\n");
	return 1;
}

/**
* @brief  This function is used to init ddr controller.
* @param  none
* @retval none
*/
void ddr_init(void)
{
	u8 ddr_init_idx = ddr_init_index();

	struct   rxi316_dram_device_info *rxi316_dram_device_info_sel;
	rxi316_dram_device_info_sel = rxi316_dram_info_m[ddr_init_idx];

	// **** DRAMC 0 Init **** //
	rxi316_perf_tune(rxi316_dram_device_info_sel);
	rxi316_dram_init(rxi316_dram_device_info_sel);

	/* Frontend init */
	rxi316_ftend_init();
}
