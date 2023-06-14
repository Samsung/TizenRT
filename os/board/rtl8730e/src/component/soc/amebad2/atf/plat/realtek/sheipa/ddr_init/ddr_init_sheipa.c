/*
 * Copyright (c) 2018, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <string.h>
#include <common/debug.h>
#include "ddr_init_sheipa.h"

void dpi_pll_enable (struct ps_pmu_portmap *dev_map)
{
	/* reset PLL */
	DW_BIT_CLEAR(dev_map->pll_dpi_reg2, PLL_DPI_MCK_CMD_EN_BFO);

	dev_map->pll_dpi_reg0 = ((0x8 << PLL_DPI_POST_PI_SEL3_BFO) |
				(0x0 << PLL_DPI_POST_PI_SEL2_BFO) |
				(0x0 << PLL_DPI_POST_PI_SEL1_BFO) |
				(0x0 << PLL_DPI_POST_PI_SEL0_BFO) |
				(0x3ff));

	dev_map->pll_dpi_reg1 = ((0x8 << PLL_DPI_POST_PI_SEL9_BFO) |
				(0x0 << PLL_DPI_POST_PI_SEL8_BFO) |
				(0x8 << PLL_DPI_POST_PI_SEL7_BFO) |
				(0x0 << PLL_DPI_POST_PI_SEL6_BFO) |
				(0x8 << PLL_DPI_POST_PI_SEL5_BFO) |
				(0x0 << PLL_DPI_POST_PI_SEL4_BFO));

	dev_map->pll_dpi_reg2 = ((0x0 << PLL_DPI_CCO_BAND_BFO) |
				(0x1 << PLL_DPI_CCO_KVCO_BFO) |
				(0x3 << PLL_DPI_ICP_BFO) |
				(0x0 << PLL_DPI_LOOP_PI_ISEL_BFO) |
				(0x0 << PLL_DPI_LPF_CP_BFO) |
				(0x3 << PLL_DPI_LPF_SR_BFO) |
				(0x0 << PLL_DPI_PDIV_BFO) |
				(0x0 << PLL_DPI_PLL_DBUG_EN_BFO) |
				(0x0 << PLL_DPI_PLL_LDO_VSEL_BFO) |
				(0x0 << PLL_DPI_PLL_SEL_CPMODE_BFO) |
				(0x0 << PLL_DPI_POST_PI_BIAS_BFO) |
				(0x0 << PLL_DPI_POST_PI_RL_BFO) |
				(0x0 << PLL_DPI_POST_PI_RS_BFO) |
				(0x0 << DPI_PLL_BYPASS_BFO) |
				(0x0 << PLL_DPI_MCK_CMD_EN_BFO) |
				(0x0 << PLL_DPI_FINE_TUNE_RANGE_BFO));

	dev_map->pll_dpi_reg3 = ((0x0 << PLL_DPI_F_CODE_BFO) |
				(0x0 << PLL_DPI_STEP_IN_BFO) |
				(0x0 << PLL_DPI_BYPASS_PI_BFO) |
				(0x0 << PLL_DPI_EN_CENTER_IN_BFO) |
				(0x0 << PLL_DPI_EN_SSC_BFO) |
				(0x0 << PLL_DPI_WEIGHTING_SEL_BFO));

	dev_map->pll_dpi_reg4 = ((0x1e << PLL_DPI_N_CODE_BFO) |
				(0x0 << PLL_DPI_TBASE_BFO) |
				(0x0 << PLL_DPI_ORDER_BFO) |
				(0x0 << PLL_DPI_SEL_MODE_BFO) |
				(0x0 << PLL_DPI_WD_ENABLE_BFO) |
				(0x0 << PLL_DPI_F390K_BFO) |
				(0x1 << PLL_DPI_TIME0_CK_BFO) |
				(0x3 << PLL_DPI_TIME_RDY_CKOUT_BFO) |
				(0x0 << PLL_DPI_TIME2_RST_WIDTH_BFO));

	dev_map->pll_dpi_reg5 = ((0x0 << PLL_DPI_PLL_DUM_BFO));
	dev_map->pll_dpi_reg6 = ((0xc << PLL_DPI_DUM_IN_BFO));
	/* reset PLL to set value */
	dev_map->dpi_reg0 = dev_map->dpi_reg0 | (0x1 << PLL_DPI_SW_RESET_N_BFO);
	DW_BIT_SET(dev_map->pll_dpi_reg2, PLL_DPI_MCK_CMD_EN_BFO);
}

void dpi_pll_init (struct dpi_pll_portmap *pll_map,
			struct dram_device_info *dram_info)
{
	uint32_t cr_bist_2to1_0;
	uint32_t cas_wr;
	uint32_t cas_rd;
	uint32_t cas_rd_t;
	uint32_t crl_srt;
	uint32_t add_lat;

	if ((dram_info->dev->device_type) == DDR_2) {
		cas_rd = dram_info->mode_reg->mode0_cas;
		add_lat = dram_info->mode_reg->mode1_all_lat;
		cas_wr = cas_rd + add_lat - 1;
	} else if ((dram_info->dev->device_type) == DDR_3) {
		crl_srt = (dram_info->mode_reg->mode0_cas >> 1);
		if (((dram_info->mode_reg->mode0_cas) & 0x1))
			cas_rd_t = crl_srt + 12;
		else
			cas_rd_t = crl_srt + 4;

		if (dram_info->mode_reg->mode1_all_lat == 1)
			add_lat = cas_rd - 1;
		else if (dram_info->mode_reg->mode1_all_lat == 2)
			add_lat = cas_rd - 2;
		else
			add_lat = 0;

		cas_rd = cas_rd_t + add_lat;

		cas_wr = dram_info->mode_reg->mode2_cwl + 5 + add_lat;
	}

	DW_BITS_SET_VAL(pll_map->RX_PLL, 7, 0x1e, 8);
	DW_BITS_SET_VAL(pll_map->RX_WATCHDOG, 22, 0x2c, 10);
	pll_map->PLL_EN = 0x0010001e;

	while ((((pll_map->INT_STATUS_2 >> 2) & 0xf)) != 0xf)
	;

	pll_map->CRT_ALL = (pll_map->CRT_ALL & 0x05ffffff) | 0x7a000000;

	cr_bist_2to1_0 = pll_map->BIST_2TO1_0;
	cr_bist_2to1_0 = (cr_bist_2to1_0 & 0x1fffffff) | 0x80000000;

	if (cas_rd & 0x1)
		cr_bist_2to1_0 = (cr_bist_2to1_0 & 0xbfffffff) | 0x40000000;

	if (cas_wr & 0x1)
		cr_bist_2to1_0 = (cr_bist_2to1_0 & 0xdfffffff) | 0x20000000;

	pll_map->BIST_2TO1_0 = 0xb0000000;

	while (((pll_map->INT_STATUS_2 >> 2) & 0xf) != 0xf)
	;
}

void dpi_dll_enable (struct ps_pmu_portmap *dev_map)
{

	DW_BIT_SET(dev_map->dll_dpi_reg2, 0);

	dev_map->dll_dpi_reg0 = ((0x0 << DLL_DPI_POST_PI_SEL2_BFO) |
				(0x0 << DLL_DPI_POST_PI_SEL1_BFO) |
				(0x0 << DLL_DPI_POST_PI_SEL0_BFO) |
				(0x3ff << PLL_DPI_EN_POST_PI_BFO));

	dev_map->dll_dpi_reg1 = ((0x0 << DLL_DPI_POST_PI_SEL6_BFO) |
				(0x0 << DLL_DPI_POST_PI_SEL4_BFO) |
				(0x10 << DLL_DPI_POST_PI_SEL7_BFO) |
				(0x10 << DLL_DPI_POST_PI_SEL5_BFO) |
				(0x10 << DLL_DPI_POST_PI_SEL3_BFO));

	dev_map->dll_dpi_reg7 = ((0x0 << DLL_DPI_POST_PI_SEL8_BFO) |
				(0x10 << DLL_DPI_POST_PI_SEL9_BFO));

	dev_map->dll_dpi_reg2 = ((0x0 << DLL_DPI_CCO_BAND_BFO) |
				(0x1 << DLL_DPI_CCO_KVCO_BFO) |
				(0x3 << DLL_DPI_ICP_BFO) |
				(0x0 << DLL_DPI_LOOP_PI_ISEL_BFO) |
				(0x0 << DLL_DPI_LPF_CP_BFO) |
				(0x3 << DLL_DPI_LPF_SR_BFO) |
				(0x0 << DLL_DPI_PDIV_BFO) |
				(0x0 << DLL_DPI_PLL_DBUG_EN_BFO) |
				(0x0 << DLL_DPI_PLL_LDO_VSEL_BFO) |
				(0x0 << DLL_DPI_PLL_SEL_CPMODE_BFO) |
				(0x0 << DLL_DPI_POST_PI_BIAS_BFO) |
				(0x0 << DLL_DPI_POST_PI_RL_BFO) |
				(0x0 << DLL_DPI_POST_PI_RS_BFO) |
				(0x0 << DPI_DLL_BYPASS_BFO) |
				(0x1 << DLL_DPI_MCK_CLK_EN_BFO));

	dev_map->dll_dpi_reg3 = ((0x0 << DLL_DPI_F_CODE_BFO) |
				(0x3 << DLL_DPI_STEP_IN_BFO) |
				(0x0 << DLL_DPI_BYPASS_PI_BFO) |
				(0x0 << DLL_DPI_EN_CENTER_IN_BFO) |
				(0x0 << DLL_DPI_EN_SSC_BFO) |
				(0x0 << DLL_DPI_WEIGHTING_SEL_BFO));


	dev_map->dll_dpi_reg4 = ((0x3e << DLL_DPI_N_CODE_BFO) |
				(0x0 << DLL_DPI_TBASE_BFO) |
				(0x0 << DLL_DPI_ORDER_BFO) |
				(0x0 << DLL_DPI_SEL_MODE_BFO) |
				(0x0 << DLL_DPI_WD_ENABLE_BFO) |
				(0x0 << DLL_DPI_F390K_BFO) |
				(0x1 << DLL_DPI_TIME0_CK_BFO) |
				(0x3 << DLL_DPI_TIME_RDY_CKOUT_BFO) |
				(0x0 << DLL_DPI_TIME2_RST_WIDTH_BFO));


	dev_map->dll_dpi_reg6 = ((0x0 << DLL_DPI_PLL_DUM_BFO) |
				(0x1 << DLL_DPI_CLK_OE_BFO) |
				(0x0 << DLL_DPI_OESYNC_OP_SEL_BFO));

	dev_map->dpi_reg0 = dev_map->dpi_reg0 | (0x1 << DLL_DPI_SW_RESET_N_BFO);

	DW_BIT_SET(dev_map->dll_dpi_reg2, DLL_DPI_MCK_CLK_EN_BFO + 1);
	DW_BIT_SET(dev_map->dll_dpi_reg6, DLL_DPI_CLK_OE_BFO + 1);

	DW_BITS_SET_VAL(dev_map->dll_dpi_reg2, DLL_DPI_MCK_CLK_EN_BFO + 2,
			0xf, 4);
	DW_BITS_SET_VAL(dev_map->dll_dpi_reg6, DLL_DPI_CLK_OE_BFO + 2,
			0xff, 8);
}

void dpi_dll_init (struct dpi_dll_portmap *dll_map,
			struct dram_device_info *dram_info)
{
	uint32_t cas_rd;
	uint32_t cas_rd_t;
	uint32_t crl_srt;
	uint32_t add_lat;

	if ((dram_info->dev->device_type) == DDR_2) {
		cas_rd = dram_info->mode_reg->mode0_cas;
		add_lat = dram_info->mode_reg->mode1_all_lat;
	} else if ((dram_info->dev->device_type) == DDR_3) {
		crl_srt = (dram_info->mode_reg->mode0_cas >> 1);
		if (((dram_info->mode_reg->mode0_cas) & 0x1))
			cas_rd_t = crl_srt + 12;
		else
			cas_rd_t = crl_srt + 4;

		if (dram_info->mode_reg->mode1_all_lat == 1)
			add_lat = cas_rd - 1;
		else if (dram_info->mode_reg->mode1_all_lat == 2)
			add_lat = cas_rd - 2;
		else
			add_lat = 0;
	    cas_rd = cas_rd_t + add_lat;
	}

	dll_map->BIST_2TO1_0 = 0xb0000000;
	dll_map->READ_CTRL_0_0 = 0x400a;
	dll_map->READ_CTRL_0_1 = 0x400a;
	dll_map->READ_CTRL_0_2 = 0x400a;
	dll_map->READ_CTRL_0_3 = 0x400a;
	dll_map->DPI_CTRL_0 = 0x03EF0412;
}

void dram_init (struct ms_rxi310_portmap *dev_map, struct dram_device_info *dram_info)
{
	uint32_t cr_bst_len = 0;
	uint32_t cas_wr;
	uint32_t mrinfo_wr_lat;
	uint32_t cas_rd;
	uint32_t mrinfo_rd_lat;
	uint32_t cas_add;
	uint32_t mrinfo_add_lat;
	uint32_t dram_mr2 = 0;
	uint32_t dram_mr0 = 0;
	uint32_t cr_twr;
	uint32_t dram_wr_tck;
	uint32_t dram_wr = 0;
	uint32_t tfaw_tck;
	uint32_t cr_trtw;
	uint32_t cr_trtw_t = 0;
	uint32_t trtp_tck;
	uint32_t twtr_tck;
	uint32_t tmrd_tck;
	uint32_t tras_tck;
	uint32_t trp_tck;
	uint32_t tccd_tck;
	uint32_t trrd_tck;
	uint32_t trc_tck;
	uint32_t trcd_tck;
	uint32_t dram_period;
	uint32_t cr_rst_tus;
	uint32_t cr_tus;
	uint32_t cr_tns = 0;
	uint32_t dfi_rate;
	enum dram_type ddr_type = 0;
	enum dram_dq_width dq_width;
	enum dram_page_size page;

	dfi_rate = 1 << (uint32_t) (dram_info->dfi_rate);

	dram_period = (dram_info->ddr_period_ps) * (dfi_rate);

	if (dram_info->timing->tras_ps % dram_period)
		tras_tck = (dram_info->timing->tras_ps / dram_period) + 1;
	else
		tras_tck = (dram_info->timing->tras_ps / dram_period);

	if (dram_info->timing->trcd_ps % dram_period)
		trcd_tck = (dram_info->timing->trcd_ps / dram_period) + 1;
	else
		trcd_tck = (dram_info->timing->trcd_ps / dram_period);

	if (dram_info->timing->trc_ps % dram_period)
		trc_tck = (dram_info->timing->trc_ps / dram_period) + 1;
	else
		trc_tck = (dram_info->timing->trc_ps / dram_period);

	if (trc_tck == 1)
		trc_tck = 2;

	if (dram_info->timing->trp_ps % dram_period)
		trp_tck = (dram_info->timing->trp_ps / dram_period) + 1;
	else
		trp_tck = (dram_info->timing->trp_ps / dram_period);

	if (dram_info->timing->tfaw_ps % dram_period)
		tfaw_tck = (dram_info->timing->tfaw_ps / dram_period) + 1;
	else
		tfaw_tck = (dram_info->timing->tfaw_ps / dram_period);

	if (dram_info->timing->trtp_tck % dfi_rate)
		trtp_tck = (dram_info->timing->trtp_tck / dfi_rate) + 1;
	else
		trtp_tck = (dram_info->timing->trtp_tck / dfi_rate);

	if (dram_info->timing->twtr_tck % dfi_rate)
		twtr_tck = (dram_info->timing->twtr_tck / dfi_rate) + 1;
	else
		twtr_tck = (dram_info->timing->twtr_tck / dfi_rate);

	if (dram_info->timing->tccd_tck % dfi_rate)
		tccd_tck = (dram_info->timing->tccd_tck / dfi_rate) + 1;
	else
		tccd_tck = (dram_info->timing->tccd_tck / dfi_rate);

	if (dram_info->timing->trrd_tck % dfi_rate)
		trrd_tck = (dram_info->timing->trrd_tck / dfi_rate) + 1;
	else
		trrd_tck = (dram_info->timing->trrd_tck / dfi_rate);

	if (dram_info->timing->tmrd_tck % dfi_rate)
		tmrd_tck = (dram_info->timing->tmrd_tck / dfi_rate) + 1;
	else
		tmrd_tck = (dram_info->timing->tmrd_tck / dfi_rate);

	if ((dram_info->dev->device_type) == SDR) {
		cas_wr = 0;
		cas_rd = dram_info->mode_reg->dram_rd_lat;
	} else {
		cas_wr = (dram_info->mode_reg->dram_wr_lat) +
		       (dram_info->mode_reg->dram_par_lat) +
		       (dram_info->mode_reg->dram_add_lat);

		cas_rd = (dram_info->mode_reg->dram_rd_lat) +
		       (dram_info->mode_reg->dram_par_lat) +
		       (dram_info->mode_reg->dram_add_lat);
	}

	cas_add = (dram_info->mode_reg->dram_add_lat);

	if (((dram_info->timing->twr_ps) * dfi_rate) % dram_period)
		dram_wr_tck = (((dram_info->timing->twr_ps) * dfi_rate)
							/ dram_period) + 1;
	else
		dram_wr_tck = (((dram_info->timing->twr_ps) * dfi_rate)
							/ dram_period);

	if (dram_wr_tck > (dram_info->timing->wr_max_tck))
		dram_wr_tck = (dram_info->timing->wr_max_tck);

	if (dram_wr_tck % dfi_rate)
		cr_twr = (dram_wr_tck / dfi_rate) + 1;
	else
		cr_twr = (dram_wr_tck / dfi_rate);

	if ((dram_info->dev->device_type) == DDR_3) {
		ddr_type = DDR_3;
		if (dram_info->mode_reg->bst_len == BST_LEN_4) {
			cr_bst_len = 0;		/* bst_4 */
			dram_mr0 = 0x220;
		} else {
			cr_bst_len = 1;		/* bst_8 */
			dram_mr0 = 0x220;
		}

		dram_mr2 = dram_info->mode_reg->mode2_cwl << 3;

		switch (dram_wr_tck) {
		case WR_TCK_5:
			dram_wr = 1;
			break;
		case WR_TCK_6:
			dram_wr = 2;
			break;
		case WR_TCK_7:
			dram_wr = 3;
			break;
		case WR_TCK_8:
			dram_wr = 4;
			break;
		case WR_TCK_10:
			dram_wr = 5;
			break;
		case WR_TCK_12:
			dram_wr = 6;
			break;
		case WR_TCK_14:
			dram_wr = 7;
			break;
		default:
			dram_wr = 0;
			break;
		}
		dram_mr0 = (dram_wr << (PCTL_MR_OP_BFO + 1)) |
			(0 << PCTL_MR_OP_BFO) |
			((dram_info->mode_reg->mode0_cas >> 1) << PCTL_MR_CAS_BFO) |
			(dram_info->mode_reg->bst_type << PCTL_MR_BT_BFO) |
			((dram_info->mode_reg->mode0_cas & 0x1) << 2) |
			dram_mr0;
		cr_trtw_t = 8;
		cr_rst_tus = 7;
		cr_tus = 9;
		cr_tns = 8;
	} else if ((dram_info->dev->device_type) == DDR_4) {
		ddr_type = DDR_4;
		if (dram_info->mode_reg->bst_len == BST_LEN_4) {
			cr_bst_len = 0;			/* bst_4 */
			dram_mr0 = 0x2;
		} else {
			cr_bst_len = 1;
			dram_mr0 = 0x0;
		}

		dram_mr2 = (dram_info->mode_reg->mode2_cwl << 3) | (1 << 9);

		switch (dram_wr_tck) {
		case WR_TCK_10:
			dram_wr = 0;
			break;
		case WR_TCK_12:
			dram_wr = 1;
			break;
		case WR_TCK_14:
			dram_wr = 2;
			break;
		case WR_TCK_16:
			dram_wr = 3;
			break;
		case WR_TCK_18:
			dram_wr = 4;
			break;
		case WR_TCK_20:
			dram_wr = 5;
			break;
		default:
			dram_wr = 6;
			break;
		}
		dram_mr0 = (dram_wr << (PCTL_MR_OP_BFO + 1)) |
			(0 << PCTL_MR_OP_BFO) |
			((dram_info->mode_reg->mode0_cas >> 1) << PCTL_MR_CAS_BFO) |
			(dram_info->mode_reg->bst_type << PCTL_MR_BT_BFO) |
			((dram_info->mode_reg->mode0_cas & 0x1) << 2) |
			dram_mr0;

		cr_trtw_t = (dram_info->mode_reg->dram_rd_lat + 6) -
				dram_info->mode_reg->dram_wr_lat;
		cr_rst_tus = 7;
		cr_tus     = 9;
		cr_tns     = 9;
	} else if ((dram_info->dev->device_type) == DDR_2) {
		ddr_type = DDR_2;
		if (dram_info->mode_reg->bst_len == BST_LEN_4) {
			cr_bst_len = 0;
			cr_trtw_t = 4;
			dram_mr0 = 0x2;
		} else {
			cr_bst_len = 1;
			cr_trtw_t = 6;
			dram_mr0 = 0x3;
		}

		dram_mr2 = dram_info->mode_reg->mode2_cwl;
		dram_mr0 = (dram_wr << (PCTL_MR_OP_BFO + 1)) |
			(0 << PCTL_MR_OP_BFO) |
			(dram_info->mode_reg->mode0_cas << PCTL_MR_CAS_BFO) |
			(dram_info->mode_reg->bst_type << PCTL_MR_BT_BFO) |
			dram_mr0;
	} else if ((dram_info->dev->device_type) == DDR_1) {
		ddr_type = DDR_1;
		if (dram_info->mode_reg->bst_len == BST_LEN_4) {
			dram_mr0 = 2;
			cr_bst_len = 0;
		} else {
			dram_mr0 = 3;		/* bst_8 */
			cr_bst_len = 1;
		}
		dram_mr0 = (cas_rd << PCTL_MR_CAS_BFO) |
			(dram_info->mode_reg->bst_type << PCTL_MR_BT_BFO) |
			dram_mr0;
		cr_trtw_t = 0;
	} else if ((dram_info->dev->device_type) == SDR) {
		ddr_type = SDR;
		if (dram_info->mode_reg->bst_len == BST_LEN_4) {
			dram_mr0 = 2;
			cr_bst_len = 0;
		} else {
			dram_mr0 = 3;		/* bst_8 */
			cr_bst_len = 1;
		}

		dram_mr0 = (dram_info->mode_reg->mode0_cas << PCTL_MR_CAS_BFO) |
			(dram_info->mode_reg->bst_type << PCTL_MR_BT_BFO) |
			dram_mr0;
		cr_trtw_t = 0;
	}

	if (dfi_rate == 1) {
		mrinfo_wr_lat = cas_wr;
		mrinfo_rd_lat = cas_rd;
		mrinfo_add_lat = dram_info->mode_reg->dram_add_lat;
	} else {
		if (cas_wr % dfi_rate)
			mrinfo_wr_lat = (cas_wr + 1) / dfi_rate;
		else
			mrinfo_wr_lat = cas_wr / dfi_rate;

		if (cas_rd % dfi_rate)
			mrinfo_rd_lat = (cas_rd + 1) / dfi_rate;
		else
			mrinfo_rd_lat = cas_rd / dfi_rate;

		if (cas_add % dfi_rate)
			mrinfo_add_lat = (cas_add+1) / dfi_rate;
		else
			mrinfo_add_lat = cas_add / dfi_rate;
	}

	if ((cr_trtw_t & 0x1))
		cr_trtw = (cr_trtw_t + 1) / dfi_rate;
	else
		cr_trtw = cr_trtw_t / dfi_rate;

	dq_width = dram_info->dev->dq_width;

	page = dram_info->dev->page + 1;
	if (dq_width == DQ_32)
		page = page + 1;

	if (dq_width == HALF_DQ32)
		dq_width = 2;

	if ((dram_info->dev->device_type) == DDR_4) {
		dev_map->misc = 0x64;
		dev_map->dcr = 0x214;
		dev_map->iocr = 0x407210;

		switch (dram_info->timing->tccd_tck) {
		case TCCD_TCK_4:
			dev_map->mr6 = 0 << 10;
			break;
		case TCCD_TCK_5:
			dev_map->mr6 = 1 << 10;
			break;
		case TCCD_TCK_6:
			dev_map->mr6 = 2 << 10;
			break;
		case TCCD_TCK_7:
			dev_map->mr6 = 3 << 10;
			break;
		case TCCD_TCK_8:
			dev_map->mr6 = 4 << 10;
			break;
		default:
			dev_map->mr6 = 4 << 10;
			break;
		}
		dev_map->mr6 = 0x800;
		dev_map->mr3 = 0;
		dev_map->mr5 = 0x400;
		dev_map->mr2 = 0x28;
		dev_map->mr1 = 0x1;
		dev_map->mr0 = 0x34;
		dev_map->mrinfo = 0x108;
		dev_map->drr = 0x11904d8b;
		dev_map->tpr0 = 0x0008b757;
		dev_map->tpr1 = 0x7058dc74;
		dev_map->tpr2 = 0x01040182;
		dev_map->tpr3 = 0x00068000;
	}

	if (((dram_info->dev->device_type) == DDR_2) ||
		((dram_info->dev->device_type) == DDR_3)) {
		dev_map->misc = ((page << WRAP_MISC_PAGE_SIZE_BFO) |
				(dram_info->dev->bank << WRAP_MISC_BANK_SIZE_BFO) |
				(cr_bst_len << WRAP_MISC_BST_SIZE_BFO));
		/* PCTL setting */
		dev_map->dcr = ((0 << PCTL_DCR_BG_REMAP_BFO) |
				(0 << PCTL_DCR_RANK2_BFO) |
				(dfi_rate << PCTL_DCR_DFI_RATE_BFO) |
				(dq_width << PCTL_DCR_DQ32_BFO) |
				(ddr_type << PCTL_DCR_DDR_BFO));

		dev_map->iocr = ((0x101) << 12);
		dev_map->mr2 = dram_mr2;

		dev_map->mr1 = 0x6;
		dev_map->mr0 = dram_mr0;
		dev_map->mrinfo = ((0 << PCTL_MRINFO_TDQSCK_MAX_BFO) |
				((mrinfo_add_lat & 0x1f) << PCTL_MRINFO_ADD_LAT_BFO) |
				((mrinfo_rd_lat  & 0x1f) << PCTL_MRINFO_RD_LAT_BFO) |
				((mrinfo_wr_lat  & 0x1f) << PCTL_MRINFO_WR_LAT_BFO));

		dev_map->mrinfo = 0x63;
		dev_map->drr = ((0 << PCTL_DRR_REF_DIS_BFO) |
				(9 << PCTL_DRR_REF_NUM_BFO) |
				((((dram_info->timing->trefi_ps) / dram_period) - 256)
					<< PCTL_DRR_TREF_BFO) |
				((((dram_info->timing->trfc_ps) / dram_period) + 1)
					<< PCTL_DRR_TRFC_BFO));
		dev_map->tpr0 = ((6 << PCTL_TPR0_TCKE_BFO) |
				(trtp_tck << PCTL_TPR0_TRTP_BFO) |
				(cr_twr << PCTL_TPR0_TWR_BFO) |
				(tras_tck << PCTL_TPR0_TRAS_BFO) |
				(trp_tck << PCTL_TPR0_TRP_BFO));
		dev_map->tpr1 = ((tfaw_tck << PCTL_TPR1_TFAW_BFO) |
				(cr_trtw << PCTL_TPR1_TRTW_BFO) |
				(twtr_tck << PCTL_TPR1_TWTR_BFO) |
				(tccd_tck << PCTL_TPR1_TCCD_BFO) |
				(trcd_tck << PCTL_TPR1_TRCD_BFO) |
				(trc_tck << PCTL_TPR1_TRC_BFO) |
				(trrd_tck << PCTL_TPR1_TRRD_BFO));
		dev_map->tpr2 = ((cr_rst_tus << PCTL_TPR2_RST_TUS_BFO) |
				(cr_tus << PCTL_TPR2_TUS_BFO) |
				(cr_tns << PCTL_TPR2_TNS_BFO) |
				(tmrd_tck << PCTL_TPR2_TMRD_BFO) |
				(3 << PCTL_TPR2_INIT_REF_NUM_BFO));
	}
	dev_map->sch_index = 0x3;
	dev_map->sch_data = 0x60080;
	dev_map->sch_index = 0x6;
	dev_map->sch_data = 0x60080;
	dev_map->sch_index = 0x9;
	dev_map->sch_data = 0x60080;
	dev_map->sch_index = 0xc;
	dev_map->sch_data = 0x60080;
	dev_map->sch_index = 0x12;
	dev_map->sch_data = 0x60080;
	dev_map->csr = 0x700;

	/* start to init */
	dev_map->ccr = 0x1;

	while (((dev_map->ccr) & 0x1) == 0x0)
	;
	/* enter mem_mode */
	dev_map->csr = 0x600;
}

int sheipa_dram_init(void)
{
	unsigned int sys_mode;
	unsigned int enabled_cpu;
	unsigned int active_cpu;
	struct ps_pmu_portmap *ps_pmu_map;
	struct ms_rxi310_portmap *ms_ctrl_0_map;
	struct ps_smu_portmap *ps_smu_map;
	ps_smu_map = (struct ps_smu_portmap *) PS_SMU_BASE;
	struct dpi_dll_portmap *dpi_dll_map;
	ps_pmu_map = (struct ps_pmu_portmap *) PS_PMU_BASE;
	ms_ctrl_0_map = (struct ms_rxi310_portmap *) BSP_MS_I_DRAMC_0_BASE;
	dpi_dll_map = (struct dpi_dll_portmap *) BSP_MS_I_PHY_1_BASE;

	/* PLL1 1200M Freq=1201.50; M=87, N=2, K=1, VCO=1201.50; code=0x4457 */
	ps_pmu_map->pll1_pwdn = 0x3;
	ps_pmu_map->pll1_rstb = 0x0;
	ps_pmu_map->pll1_rstb = 0x10001;
	ps_pmu_map->pll1_cfg = 0x01b48c57;
	ps_pmu_map->pll1_oeb = 0x0;
	ps_pmu_map->pll1_codeready = 0x1;

	/* PLL2 300M Freq=300.38; M=87, N=2, K=4, VCO=1201.50; code=0x7457 */
	ps_pmu_map->pll2_cfg = 0x01b4ec57;
	ps_pmu_map->pll2_pwdn = 0x3;
	ps_pmu_map->pll2_rstb = 0x0;
	ps_pmu_map->pll2_rstb = 0x10001;
	ps_pmu_map->pll2_oeb = 0x0;

	/* PLL3 74.25M Freq=148.50; M=42, N=2, K=4, VCO=594.00; code=0x742a */
	ps_pmu_map->pll3_cfg = 0x01b4ec2a;
	ps_pmu_map->pll3_pwdn = 0x3;
	ps_pmu_map->pll3_rstb = 0x0;
	ps_pmu_map->pll3_rstb = 0x10001;
	ps_pmu_map->pll3_oeb = 0x0;

	/* Enable RTK_PHY PLL/DLL */
	ps_pmu_map->dpi_reg0 = ((0x0  << DLL_DPI_SW_RESET_N_BFO) |
				(0x0  << PLL_DPI_SW_RESET_N_BFO));
	ps_pmu_map->dll_dpi_reg5 = ((0x1 << DPI_ANA_PWDN_BFO) |
				(0x1 << DPI_PAD_PWDN_BFO));
	dpi_dll_enable (ps_pmu_map);
#ifdef DDR3
	dpi_dll_init (dpi_dll_map, &dll_dram_info_DDR3_25MHZ);
#elif defined(DDR4)
	dpi_dll_init (dpi_dll_map, &dll_dram_info_DDR4_25MHZ);
#else
	dpi_dll_init (dpi_dll_map, &dll_dram_info_DDR2_25MHZ);
#endif
	/* Enable Test CPU */
	/* Get current test subsystem information from sys_mode reg */
	sys_mode = ps_smu_map->sys_mode;
	enabled_cpu = (sys_mode >> 2) & 0x3;
	active_cpu = (sys_mode >> 4) & 0x3;
	if (enabled_cpu == 0) {
		if (active_cpu == 0x1)
			ps_pmu_map->cpusel_reg0 = 0x1;
		else if (active_cpu == 0x2)
			ps_pmu_map->cpusel_reg0 = 0x2;
		else if (active_cpu == 0x3)
			ps_pmu_map->cpusel_reg0 = 0x4;
	}

#ifdef DDR3
	if (DRAM_CHIP_NUM == 2)
		dram_init(ms_ctrl_0_map, &dll_dram_info_DDR3_25MHZ);
	else if (DRAM_CHIP_NUM == 1)
		dram_init(ms_ctrl_0_map, &dll_dram_info_DDR3_25MHZ_HALF_DQ);
#elif defined(DDR4)
	if (DRAM_CHIP_NUM == 2)
		dram_init(ms_ctrl_0_map, &dll_dram_info_DDR4_25MHZ);
	else if (DRAM_CHIP_NUM == 1)
		dram_init(ms_ctrl_0_map, &dll_dram_info_DDR4_25MHZ_HALF_DQ);
#else
	if (DRAM_CHIP_NUM == 2)
		dram_init(ms_ctrl_0_map, &dll_dram_info_DDR2_25MHZ);
	else if (DRAM_CHIP_NUM == 1)
		dram_init(ms_ctrl_0_map, &dll_dram_info_DDR2_25MHZ_HALF_DQ);
#endif

    return 0;
}
