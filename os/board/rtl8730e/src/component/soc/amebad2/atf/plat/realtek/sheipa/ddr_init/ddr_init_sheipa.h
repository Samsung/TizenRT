/*
 * Copyright (c) 2018, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DDR_SHEIPA_H
#define DDR_SHEIPA_H

/* DRAM Config */
#define DRAM_CHIP_NUM			2
#define DDR3

#define REG_BASE			0x00000000
/* System Control and Monitor */
#define BSP_PS_I_SMU_BASE		(REG_BASE + 0x1FB07000)
/* PLL/CRT Control Unit */
#define BSP_PS_I_PMU_BASE		(REG_BASE + 0x1FB08000)
/* Memory Subsystem DRAMC */
#define BSP_MS_I_DRAMC_0_BASE		(REG_BASE + 0x1B100000)
/* Memory Subsystem DRAMC */
#define BSP_MS_I_DRAMC_1_BASE		(REG_BASE + 0x1B101000)
/* Memory Subsystem DDRPHY */
#define BSP_MS_I_PHY_1_BASE		(REG_BASE+0x1B103500)

#define PS_SMU_BASE			BSP_PS_I_SMU_BASE
#define PS_SMU_DEVICE_ID_OFF		0x00
#define PS_SMU_SHEIPA_VER_OFF		0x04
#define PS_SMU_VERSION_OFF		0x08
#define PS_SMU_DEVELOPER_OFF		0x0c
#define PS_SMU_BITFILE_YEAR_OFF		0x10
#define PS_SMU_BFILE_DATE_OFF		0x14

#define PS_SMU_TOP_CONFIG00_OFF		0x40
#define PS_SMU_TOP_CONFIG01_OFF		0x44
#define PS_SMU_TOP_CONFIG02_OFF		0x48
#define PS_SMU_TOP_CONFIG03_OFF		0x4c
#define PS_SMU_TOP_CONFIG04_OFF		0x50
#define PS_SMU_TOP_CONFIG05_OFF		0x54
#define PS_SMU_TOP_CONFIG06_OFF		0x58
#define PS_SMU_TOP_CONFIG07_OFF		0x5c

#define PS_SMU_SYS_MODE_OFF		0x80
#define PS_SMU_CS1_JUMP_ADDR_OFF	0xf0
#define PS_SMU_CS2_JUMP_ADDR_OFF	0xf4
#define PS_SMU_CS3_JUMP_ADDR_OFF	0xf8
#define PS_SMU_SS_JUMP_ADDR_OFF		0xfc
/* BIT operation */
#define DW_BITS_MASK(__bfws, __bits) ((uint32_t)((__bfws) == 32) ?	\
	0x0 : (((0xffffffff) >> (32 - __bits)) << (__bfws)))

#define DW_BIT_MASK(__bfws) ((uint32_t)((__bfws) == 32) ?	\
	0x0 : (0x1 << (__bfws)))

#define DW_BIT_MASK_WIDTH(__bfws, __bits) ((uint32_t)((__bfws) == 32) ?	\
	0xFFFFFFFF : ((uint32_t)((1 << (__bits)) - 1) << (__bfws)))

#define DW_BITS_CLEAR(__datum, __bfws, __bits)				\
	((__datum) = ((uint32_t)(__datum) & ~DW_BITS_MASK(__bfws, __bits)))

#define DW_BIT_CLEAR(__datum, __bfws)					\
	((__datum) = ((uint32_t)(__datum) & ~DW_BIT_MASK(__bfws)))

#define DW_BITS_SET(__datum, __bfws, __bits)				\
	((__datum) = ((uint32_t)(__datum) & ~DW_BITS_MASK(__bfws, __bits)) |	\
	(DW_BITS_MASK(__bfws, __bits)))

#define DW_BIT_SET(__datum, __bfws)				\
	((__datum) = ((uint32_t)(__datum) & ~DW_BIT_MASK(__bfws)) |	\
	(DW_BIT_MASK(__bfws)))

#define DW_BITS_SET_VAL(__datum, __bfws, __val, bit_num)	\
	((__datum) = ((uint32_t)(__datum) & ~DW_BIT_MASK_WIDTH(__bfws, bit_num)) |	\
	((__val << (__bfws)) & DW_BIT_MASK_WIDTH(__bfws, bit_num)))

struct ps_smu_portmap {
	volatile unsigned int device_id;	/* Device ID Register(0x000) */
	volatile unsigned int sheipa_ver;	/* Sheipa Version Reg(0x004) */
	volatile unsigned int version;		/* Version Reg (0x008) */
	volatile unsigned int developer;	/* Developer Reg (0x00c) */
	volatile unsigned int bitfile_year;	/* Bitfile Year Reg (0x010) */
	volatile unsigned int bitfile_date;	/* Biffile Date Reg (0x014) */
	volatile unsigned int reserved1[10];
	volatile unsigned int top_config00;	/* Top Config Reg 0 (0x040) */
	volatile unsigned int top_config01;	/* Top Config Reg 1 (0x044) */
	volatile unsigned int top_config02;	/* Top Config Reg 2 (0x048) */
	volatile unsigned int top_config03;	/* Top Config Reg 3 (0x04c) */
	volatile unsigned int top_config04;	/* Top Config Reg 4 (0x050) */
	volatile unsigned int top_config05;	/* Top Config Reg 5 (0x054) */
	volatile unsigned int top_config06;	/* Top Config Reg 6 (0x058) */
	volatile unsigned int top_config07;	/* Top Config Reg 7 (0x05c) */
	volatile unsigned int reserved2[8];
	volatile unsigned int sys_mode;		/* System Mode Reg (0x080) */
	volatile unsigned int reserved3[27];
	volatile unsigned int cpu_jump_addr[4];	/* CS1 Jump Addr Reg (0x0f0) */
};

/* PMU */
#define PS_PMU_BASE			BSP_PS_I_PMU_BASE
#define PS_PMU_PLL0_CFG_OFF		0x00
#define PS_PMU_PLL0_OEB_OFF		0x04
#define PS_PMU_PLL0_PWDN_OFF		0x08
#define PS_PMU_PLL0_RSTB_OFF		0x0c
#define PS_PMU_PLL0_BP_OFF		0x10
#define PS_PMU_PLL0_TST_OFF		0x14

#define PS_PMU_PLL1_CFG_OFF		0x20
#define PS_PMU_PLL1_OEB_OFF		0x24
#define PS_PMU_PLL1_PWDN_OFF		0x28
#define PS_PMU_PLL1_RSTB_OFF		0x2c
#define PS_PMU_PLL1_BP_OFF		0x30
#define PS_PMU_PLL1_TST_OFF		0x34

#define PS_PMU_PLL2_CFG_OFF		0x40
#define PS_PMU_PLL2_OEB_OFF		0x44
#define PS_PMU_PLL2_PWDN_OFF		0x48
#define PS_PMU_PLL2_RSTB_OFF		0x4c
#define PS_PMU_PLL2_BP_OFF		0x50
#define PS_PMU_PLL2_TST_OFF		0x54

#define PS_PMU_PLL3_CFG_OFF		0x60
#define PS_PMU_PLL3_OEB_OFF		0x64
#define PS_PMU_PLL3_PWDN_OFF		0x68
#define PS_PMU_PLL3_RSTB_OFF		0x6c
#define PS_PMU_PLL3_BP_OFF		0x70
#define PS_PMU_PLL3_TST_OFF		0x74

#define PS_PMU_CKO_SEL_OFF		0x80
#define PS_PMU_DIV8_RSTB_OFF		0x84
#define PS_PMU_CKBUF_PWDN_OFF		0x88

#define PS_PMU_DPI_REG0_OFF		0x100
#define PS_PMU_PAD_REG0_OFF		0x104
#define PS_PMU_PAD_REG1_OFF		0x108

#define PS_PMU_PLL_DPI_REG0_OFF		0x200
#define PS_PMU_PLL_DPI_REG1_OFF		0x204
#define PS_PMU_PLL_DPI_REG2_OFF		0x208
#define PS_PMU_PLL_DPI_REG3_OFF		0x20c
#define PS_PMU_PLL_DPI_REG4_OFF		0x210
#define PS_PMU_DLL_DPI_REG0_OFF		0x220
#define PS_PMU_DLL_DPI_REG1_OFF		0x224
#define PS_PMU_DLL_DPI_REG2_OFF		0x228
#define PS_PMU_DLL_DPI_REG3_OFF		0x22c
#define PS_PMU_DLL_DPI_REG4_OFF		0x230
#define PS_PMU_DLL_DPI_REG5_OFF		0x234

#define PS_PMU_DPI_REG1_OFF		0x240

#define PS_PMU_CRT_REG0_OFF		0x300
#define PS_PMU_DBG_REG0_OFF		0x304
#define PS_PMU_CPUSEL_REG0_OFF		0x308

#define PS_PMU_IDCODE_OFF		0xffc

#define PS_PMU_IDCODE_VALUE		0x05912060

#define PLL_DPI_SW_RESET_N_BFO		0
#define PLL_DPI_SW_RESET_N_BFW		1
#define DLL_DPI_SW_RESET_N_BFO		1
#define DLL_DPI_SW_RESET_N_BFW		1

/* PLL_DPI_REG0_WR */
#define PLL_DPI_EN_POST_PI_BFO		0
#define PLL_DPI_EN_POST_PI_BFW		10
#define PLL_DPI_POST_PI_SEL0_BFO	10
#define PLL_DPI_POST_PI_SEL0_BFW	5
#define PLL_DPI_POST_PI_SEL1_BFO	15
#define PLL_DPI_POST_PI_SEL1_BFW	5
#define PLL_DPI_POST_PI_SEL2_BFO	20
#define PLL_DPI_POST_PI_SEL2_BFW	5
#define PLL_DPI_POST_PI_SEL3_BFO	25
#define PLL_DPI_POST_PI_SEL3_BFW	5

/* PLL_DPI_REG1_WR */
#define PLL_DPI_POST_PI_SEL4_BFO	0
#define PLL_DPI_POST_PI_SEL4_BFW	5
#define PLL_DPI_POST_PI_SEL5_BFO	5
#define PLL_DPI_POST_PI_SEL5_BFW	5
#define PLL_DPI_POST_PI_SEL6_BFO	10
#define PLL_DPI_POST_PI_SEL6_BFW	5
#define PLL_DPI_POST_PI_SEL7_BFO	15
#define PLL_DPI_POST_PI_SEL7_BFW	5
#define PLL_DPI_POST_PI_SEL8_BFO	20
#define PLL_DPI_POST_PI_SEL8_BFW	5
#define PLL_DPI_POST_PI_SEL9_BFO	25
#define PLL_DPI_POST_PI_SEL9_BFW	5

/* PLL_DPI_REG2_WR */
#define PLL_DPI_CCO_BAND_BFO		0
#define PLL_DPI_CCO_BAND_BFW		2
#define PLL_DPI_CCO_KVCO_BFO		2
#define PLL_DPI_CCO_KVCO_BFW		1
#define PLL_DPI_ICP_BFO			3
#define PLL_DPI_ICP_BFW			4
#define PLL_DPI_LOOP_PI_ISEL_BFO	7
#define PLL_DPI_LOOP_PI_ISEL_BFW	2
#define PLL_DPI_LPF_CP_BFO		9
#define PLL_DPI_LPF_CP_BFW		1
#define PLL_DPI_LPF_SR_BFO		10
#define PLL_DPI_LPF_SR_BFW		3
#define PLL_DPI_PDIV_BFO		13
#define PLL_DPI_PDIV_BFW		2
#define PLL_DPI_PLL_DBUG_EN_BFO		15
#define PLL_DPI_PLL_DBUG_EN_BFW		1
#define PLL_DPI_PLL_LDO_VSEL_BFO	16
#define PLL_DPI_PLL_LDO_VSEL_BFW	2
#define PLL_DPI_PLL_SEL_CPMODE_BFO	18
#define PLL_DPI_PLL_SEL_CPMODE_BFW	1
#define PLL_DPI_POST_PI_BIAS_BFO	19
#define PLL_DPI_POST_PI_BIAS_BFW	2
#define PLL_DPI_POST_PI_RL_BFO		21
#define PLL_DPI_POST_PI_RL_BFW		2
#define PLL_DPI_POST_PI_RS_BFO		23
#define PLL_DPI_POST_PI_RS_BFW		1
#define DPI_PLL_BYPASS_BFO		24
#define DPI_PLL_BYPASS_BFW		1
#define PLL_DPI_MCK_CMD_EN_BFO		25
#define PLL_DPI_MCK_CMD_EN_BFW		1
#define PLL_DPI_FINE_TUNE_RANGE_BFO	26
#define PLL_DPI_FINE_TUNE_RANGE_BFW	1

/* PLL_DPI_REG3_WR */
#define PLL_DPI_F_CODE_BFO		0
#define PLL_DPI_F_CODE_BFW		3
#define PLL_DPI_STEP_IN_BFO		13
#define PLL_DPI_STEP_IN_BFW		3
#define PLL_DPI_BYPASS_PI_BFO		26
#define PLL_DPI_BYPASS_PI_BFW		1
#define PLL_DPI_EN_CENTER_IN_BFO	27
#define PLL_DPI_EN_CENTER_IN_BFW	1
#define PLL_DPI_EN_SSC_BFO		28
#define PLL_DPI_EN_SSC_BFW		1
#define PLL_DPI_WEIGHTING_SEL_BFO	29
#define PLL_DPI_WEIGHTING_SEL_BFW	3

/* PLL_DPI_REG4_WR */
#define PLL_DPI_N_CODE_BFO		0
#define PLL_DPI_N_CODE_BFW		8
#define PLL_DPI_TBASE_BFO		8
#define PLL_DPI_TBASE_BFW		12
#define PLL_DPI_ORDER_BFO		20
#define PLL_DPI_ORDER_BFW		1
#define PLL_DPI_SEL_MODE_BFO		21
#define PLL_DPI_SEL_MODE_BFW		1
#define PLL_DPI_WD_ENABLE_BFO		22
#define PLL_DPI_WD_ENABLE_BFW		1
#define PLL_DPI_F390K_BFO		23
#define PLL_DPI_F390K_BFW		2
#define PLL_DPI_TIME0_CK_BFO		25
#define PLL_DPI_TIME0_CK_BFW		3
#define PLL_DPI_TIME_RDY_CKOUT_BFO	28
#define PLL_DPI_TIME_RDY_CKOUT_BFW	3
#define PLL_DPI_TIME2_RST_WIDTH_BFO	30
#define PLL_DPI_TIME2_RST_WIDTH_BFW	2

/* PLL_DPI_REG5_WR */
#define PLL_DPI_PLL_DUM_BFO		0
#define PLL_DPI_PLL_DUM_BFW		8

/* PLL_DPI_REG6_WR */
#define PLL_DPI_DUM_IN_BFO		0
#define PLL_DPI_DUM_IN_BFW		32

/* DLL_DPI_REG0_WR */
#define DLL_DPI_EN_POST_PI_BFO		0
#define DLL_DPI_EN_POST_PI_BFW		10
#define DLL_DPI_POST_PI_SEL0_BFO	10
#define DLL_DPI_POST_PI_SEL0_BFW	6
#define DLL_DPI_POST_PI_SEL1_BFO	16
#define DLL_DPI_POST_PI_SEL1_BFW	6
#define DLL_DPI_POST_PI_SEL2_BFO	22
#define DLL_DPI_POST_PI_SEL2_BFW	6

/* DLL_DPI_REG1_WR */
#define DLL_DPI_POST_PI_SEL3_BFO	0
#define DLL_DPI_POST_PI_SEL3_BFW	6
#define DLL_DPI_POST_PI_SEL4_BFO	6
#define DLL_DPI_POST_PI_SEL4_BFW	6
#define DLL_DPI_POST_PI_SEL5_BFO	12
#define DLL_DPI_POST_PI_SEL5_BFW	6
#define DLL_DPI_POST_PI_SEL6_BFO	18
#define DLL_DPI_POST_PI_SEL6_BFW	6
#define DLL_DPI_POST_PI_SEL7_BFO	24
#define DLL_DPI_POST_PI_SEL7_BFW	6

/* DLL_DPI_REG2_WR */
#define DLL_DPI_CCO_BAND_BFO		0
#define DLL_DPI_CCO_BAND_BFW		2
#define DLL_DPI_CCO_KVCO_BFO		2
#define DLL_DPI_CCO_KVCO_BFW		1
#define DLL_DPI_ICP_BFO			3
#define DLL_DPI_ICP_BFW			4
#define DLL_DPI_LOOP_PI_ISEL_BFO	7
#define DLL_DPI_LOOP_PI_ISEL_BFW	2
#define DLL_DPI_LPF_CP_BFO		9
#define DLL_DPI_LPF_CP_BFW		1
#define DLL_DPI_LPF_SR_BFO		10
#define DLL_DPI_LPF_SR_BFW		3
#define DLL_DPI_PDIV_BFO		13
#define DLL_DPI_PDIV_BFW		2
#define DLL_DPI_PLL_DBUG_EN_BFO		15
#define DLL_DPI_PLL_DBUG_EN_BFW		1
#define DLL_DPI_PLL_LDO_VSEL_BFO	16
#define DLL_DPI_PLL_LDO_VSEL_BFW	2
#define DLL_DPI_PLL_SEL_CPMODE_BFO	18
#define DLL_DPI_PLL_SEL_CPMODE_BFW	1
#define DLL_DPI_POST_PI_BIAS_BFO	19
#define DLL_DPI_POST_PI_BIAS_BFW	2
#define DLL_DPI_POST_PI_RL_BFO		21
#define DLL_DPI_POST_PI_RL_BFW		2
#define DLL_DPI_POST_PI_RS_BFO		23
#define DLL_DPI_POST_PI_RS_BFW		1
#define DPI_DLL_BYPASS_BFO		24
#define DPI_DLL_BYPASS_BFW		1
#define DLL_DPI_MCK_CLK_EN_BFO		25
#define DLL_DPI_MCK_CLK_EN_BFW		7

/* DLL_DPI_REG3_WR */
#define DLL_DPI_F_CODE_BFO		0
#define DLL_DPI_F_CODE_BFW		13
#define DLL_DPI_STEP_IN_BFO		13
#define DLL_DPI_STEP_IN_BFW		13
#define DLL_DPI_BYPASS_PI_BFO		26
#define DLL_DPI_BYPASS_PI_BFW		1
#define DLL_DPI_EN_CENTER_IN_BFO	27
#define DLL_DPI_EN_CENTER_IN_BFW	1
#define DLL_DPI_EN_SSC_BFO		28
#define DLL_DPI_EN_SSC_BFW		1
#define DLL_DPI_WEIGHTING_SEL_BFO	29
#define DLL_DPI_WEIGHTING_SEL_BFW	3

/* DLL_DPI_REG4_WR */
#define DLL_DPI_N_CODE_BFO		0
#define DLL_DPI_N_CODE_BFW		8
#define DLL_DPI_TBASE_BFO		8
#define DLL_DPI_TBASE_BFW		12
#define DLL_DPI_ORDER_BFO		1
#define DLL_DPI_ORDER_BFW		20
#define DLL_DPI_SEL_MODE_BFO		1
#define DLL_DPI_SEL_MODE_BFW		21
#define DLL_DPI_WD_ENABLE_BFO		1
#define DLL_DPI_WD_ENABLE_BFW		22
#define DLL_DPI_F390K_BFO		23
#define DLL_DPI_F390K_BFW		2
#define DLL_DPI_TIME0_CK_BFO		25
#define DLL_DPI_TIME0_CK_BFW		3
#define DLL_DPI_TIME_RDY_CKOUT_BFO	28
#define DLL_DPI_TIME_RDY_CKOUT_BFW	2
#define DLL_DPI_TIME2_RST_WIDTH_BFO	30
#define DLL_DPI_TIME2_RST_WIDTH_BFW	2

/* DLL_DPI_REG5_WR */
#define DPI_ANA_PWDN_BFO		0
#define DPI_ANA_PWDN_BFW		1
#define DPI_PAD_PWDN_BFO		1
#define DPI_PAD_PWDN_BFW		1

/* DLL_DPI_REG6_WR */
#define DLL_DPI_PLL_DUM_BFO		0
#define DLL_DPI_PLL_DUM_BFW		8
#define DLL_DPI_CLK_OE_BFO		8
#define DLL_DPI_CLK_OE_BFW		10
#define DLL_DPI_OESYNC_OP_SEL_BFO	18
#define DLL_DPI_OESYNC_OP_SEL_BFW	10

/* DLL_DPI_REG7_WR */
#define DLL_DPI_POST_PI_SEL8_BFO	0
#define DLL_DPI_POST_PI_SEL8_BFW	6
#define DLL_DPI_POST_PI_SEL9_BFO	6
#define DLL_DPI_POST_PI_SEL9_BFW	6

/* DLL_DPI_REG8_WR */
#define DLL_DPI_DUM_IN_BFO		0
#define DLL_DPI_DUM_IN_BFW		32

/*  DPI_REG1_RDATA */
#define DPI_DLL_STABLE_BFO		0
#define DPI_DLL_STABLE_BFW		1
#define DPI_PLL_STABLE_BFO		1
#define DPI_PLL_STABLE_BFW		1

struct ps_pmu_portmap {
	volatile unsigned int pll0_cfg;		/* PLL 0 Control Reg (0x000) */
	volatile unsigned int pll0_oeb;		/* PLL 0 Control Reg (0x004) */
	volatile unsigned int pll0_pwdn;	/* PLL 0 Control Reg (0x008) */
	volatile unsigned int pll0_rstb;	/* PLL 0 Control Reg (0x00c) */
	volatile unsigned int reserved0[1];
	volatile unsigned int pll0_tst;		/* PLL 0 Control Reg (0x014) */
	volatile unsigned int pll0_codeready;	/* PLL 0 Control Reg (0x018) */
	volatile unsigned int pll0_wdout;	/* PLL 0 Control Reg (0x01c) */

	volatile unsigned int pll1_cfg;		/* PLL 1 Control Reg (0x020) */
	volatile unsigned int pll1_oeb;		/* PLL 1 Control Reg (0x024) */
	volatile unsigned int pll1_pwdn;	/* PLL 1 Control Reg (0x028) */
	volatile unsigned int pll1_rstb;	/* PLL 1 Control Reg (0x02c) */
	volatile unsigned int reserved1[1];
	volatile unsigned int pll1_tst;		/* PLL 1 Control Reg (0x034) */
	volatile unsigned int pll1_codeready;	/* PLL 0 Control Reg (0x038) */
	volatile unsigned int pll1_wdout;	/* PLL 0 Control Reg (0x03c) */

	volatile unsigned int pll2_cfg;		/* PLL 2 Control Reg (0x040) */
	volatile unsigned int pll2_oeb;		/* PLL 2 Control Reg (0x044) */
	volatile unsigned int pll2_pwdn;	/* PLL 2 Control Reg (0x048) */
	volatile unsigned int pll2_rstb;	/* PLL 2 Control Reg (0x04c) */
	volatile unsigned int pll2_dummy0;
	volatile unsigned int pll2_tst;		/* PLL 2 Control Reg (0x054) */
	volatile unsigned int pll2_dummy1;
	volatile unsigned int pll2_wdout;	/* PLL 2 Control Reg (0x05c) */

	volatile unsigned int pll3_cfg;		/* PLL 3 Control Reg (0x060) */
	volatile unsigned int pll3_oeb;		/* PLL 3 Control Reg (0x064) */
	volatile unsigned int pll3_pwdn;	/* PLL 3 Control Reg (0x068) */
	volatile unsigned int pll3_rstb; 	/* PLL 3 Control Reg (0x06c) */
	volatile unsigned int pll3_dummy0;
	volatile unsigned int pll3_tst;		/* PLL 3 Control Reg (0x074) */
	volatile unsigned int pll3_dummy1;
	volatile unsigned int pll3_wdout;	/* PLL 3 Control Reg (0x07c) */

	volatile unsigned int cko_sel;		/* PLL Control Reg (0x080) */
	volatile unsigned int div8_rstb;	/* PLL Control Reg (0x084) */
	volatile unsigned int ckbuf_pwdn;	/* PLL Control Reg (0x088) */
	volatile unsigned int reserved4[29];


	volatile unsigned int dpi_reg0;		/* DDR Control Reg (0x100) */
	volatile unsigned int pad_reg0;		/* Pad Control Reg (0x104) */
	volatile unsigned int pad_reg1;		/* Pad Control Reg (0x108) */
	volatile unsigned int reserved5[61];

	volatile unsigned int pll_dpi_reg0;	/* DDR Control Reg (0x200) */
	volatile unsigned int pll_dpi_reg1;	/* DDR Control Reg (0x204) */
	volatile unsigned int pll_dpi_reg2;	/* DDR Control Reg (0x208) */
	volatile unsigned int pll_dpi_reg3;	/* DDR Control Reg (0x20c) */
	volatile unsigned int pll_dpi_reg4;	/* DDR Control Reg (0x210) */
	volatile unsigned int pll_dpi_reg5;	/* DDR Control Reg (0x214) */
	volatile unsigned int pll_dpi_reg6;	/* DDR Control Reg (0x218) */
	volatile unsigned int pll_dpi_reg7;	/* DDR Control Reg (0x21c) */
	volatile unsigned int dll_dpi_reg0;	/* DDR Control Reg (0x220) */
	volatile unsigned int dll_dpi_reg1;	/* DDR Control Reg (0x224) */
	volatile unsigned int dll_dpi_reg2;	/* DDR Control Reg (0x228) */
	volatile unsigned int dll_dpi_reg3;	/* DDR Control Reg (0x22c) */
	volatile unsigned int dll_dpi_reg4;	/* DDR Control Reg (0x230) */
	volatile unsigned int dll_dpi_reg5;	/* DDR Control Reg (0x234) */
	volatile unsigned int dll_dpi_reg6;	/* DDR Control Reg (0x238) */
	volatile unsigned int dll_dpi_reg7;	/* DDR Control Reg (0x23c) */
	volatile unsigned int dpi_reg1;		/* DDR Control Reg (0x240) */
	volatile unsigned int dll_dpi_reg8;	/* DDR Control Reg (0x244) */
	volatile unsigned int reserved8[46];
	volatile unsigned int crt_reg0;		/* CRT Control Reg (0x300) */
	volatile unsigned int dbg_reg0;		/* DBG Control Reg (0x304) */
	volatile unsigned int cpusel_reg0;	/* CPU Select Reg (0x308) */
	volatile unsigned int reserved9[828];
	volatile unsigned int id_code;		/* CPU Select Reg(0xffc) */
};

#define MS_0_CTRL_BASE		BSP_MS_I_DRAMC_0_BASE
#define MS_0_CTRL_PHY_BASE	(BSP_MS_I_DRAMC_0_BASE)
#define MS_0_WRAP_BASE		(MS_0_CTRL_BASE + 0x200)

#define MS_1_CTRL_BASE		BSP_MS_I_DRAMC_1_BASE
#define MS_1_CTRL_PHY_BASE	(BSP_MS_I_DRAMC_1_BASE)
#define MS_1_WRAP_BASE		(MS_1_CTRL_BASE + 0x200)

#define MS_PCTL_CCR_OFFSET		0x000
#define MS_PCTL_DCR_OFFSET		0x004
#define MS_PCTL_IOCR_OFFSET		0x008
#define MS_PCTL_CSR_OFFSET		0x00c
#define MS_PCTL_DRR_OFFSET		0x010
#define MS_PCTL_TPR0_OFFSET		0x014
#define MS_PCTL_TPR1_OFFSET		0x018
#define MS_PCTL_TPR2_OFFSET		0x01c
#define MS_PCTL_TPR3_OFFSET		0x020
#define MS_PCTL_CDPIN_OFFSET		0x028
#define MS_PCTL_TDPIN_OFFSET		0x02c

#define MS_PCTL_MRINFO_OFFSET		0x030
#define MS_PCTL_MR0_OFFSET		0x034
#define MS_PCTL_MR1_OFFSET		0x038
#define MS_PCTL_MR2_OFFSET		0x03c
#define MS_PCTL_MR3_OFFSET		0x040
#define MS_PCTL_MR4_OFFSET		0x044
#define MS_PCTL_MR5_OFFSET		0x048
#define MS_PCTL_MR6_OFFSET		0x04c
#define MS_PCTL_IOCR1_OFFSET		0x050

#define MS_PCTL_DLLCR4_OFFSET		0x054
#define MS_PCTL_DLLCR5_OFFSET		0x058
#define MS_PCTL_DLLCR6_OFFSET		0x05c
#define MS_PCTL_DLLCR7_OFFSET		0x060
#define MS_PCTL_DLLCR8_OFFSET		0x064
#define MS_PCTL_DQTR0_OFFSET		0x068
#define MS_PCTL_DQTR1_OFFSET		0x06c
#define MS_PCTL_DQTR2_OFFSET		0x070
#define MS_PCTL_DQTR3_OFFSET		0x074
#define MS_PCTL_DQTR4_OFFSET		0x078
#define MS_PCTL_DQTR5_OFFSET		0x07c
#define MS_PCTL_DQTR6_OFFSET		0x080
#define MS_PCTL_DQTR7_OFFSET		0x084
#define MS_PCTL_DQSTR_OFFSET		0x088
#define MS_PCTL_DQSBTR_OFFSET		0x08c
#define MS_PCTL_ODTCR_OFFSET		0x090

#define MS_PCTL_DTR0_OFFSET		0x094
#define MS_PCTL_DTR1_OFFSET		0x098
#define MS_PCTL_DTAR_OFFSET		0x09c

#define MS_PCTL_ZQCR0_OFFSET		0x0a0
#define MS_PCTL_ZQCR1_OFFSET		0x0a4
#define MS_PCTL_ZQSR_OFFSET		0x0a8
#define MS_PCTL_RSLR0_OFFSET		0x0ac
#define MS_PCTL_RSLR1_OFFSET		0x0b0
#define MS_PCTL_RSLR2_OFFSET		0x0b4
#define MS_PCTL_RSLR3_OFFSET		0x0b8
#define MS_PCTL_RDGR0_OFFSET		0x0bc
#define MS_PCTL_RDGR1_OFFSET		0x0c0
#define MS_PCTL_RDGR2_OFFSET		0x0c4
#define MS_PCTL_RDGR3_OFFSET		0x0c8
#define MS_PCTL_MXSL_OFFSET		0x0cc

#define MS_PCTL_BCR_OFFSET		0x0d0
#define MS_PCTL_BCT_OFFSET		0x0d4
#define MS_PCTL_BCM_OFFSET		0x0d8
#define MS_PCTL_BST_OFFSET		0x0dc
#define MS_PCTL_BSRAM0_OFFSET		0x0e0
#define MS_PCTL_BSRAM1_OFFSET		0x0e4
#define MS_PCTL_BER_OFFSET		0x0e8

#define MS_PCTL_BYR_OFFSET		0x0ec
#define MS_PCTL_BFA_OFFSET		0x0f0
#define MS_PCTL_IDR_OFFSET		0x0f8
#define MS_PCTL_ERR_OFFSET		0x0fc

#define MS_WRAP_MISC_OFFSET		0x224

#define MS_PHY_DLY0			0x100
#define MS_PHY_DLY1_RST			0x104
#define MS_PHY_DLY_CLK			0x108
#define MS_PHY_DLY_ST			0x10c
#define MS_PHY_DLY_NUM			0x110

#define PCTL_CCR_INIT_BFO		0
#define PCTL_CCR_INIT_BFW		1
#define PCTL_CCR_DTT_BFO		1
#define PCTL_CCR_DTT_BFW		1
#define PCTL_CCR_BTT_BFO		2
#define PCTL_CCR_BTT_BFW		1
#define PCTL_CCR_DPIT_BFO		3
#define PCTL_CCR_DPIT_BFW		1
#define PCTL_CCR_SRXT_BFO		4
#define PCTL_CCR_SRXT_BFW		1
#define PCTL_CCR_FLUSH_FIFO_BFO		8
#define PCTL_CCR_FLUSH_FIFO_BFW		1
#define PCTL_CCR_CR_UPDATE_BFO		31
#define PCTL_CCR_CR_UPDATE_BFW		1

#define PCTL_DCR_DDR3_BFO		0
#define PCTL_DCR_DDR3_BFW		1
#define PCTL_DCR_SDR_BFO		1
#define PCTL_DCR_SDR_BFW		1

#define PCTL_DCR_DDR_BFO		0
#define PCTL_DCR_DDR_BFW		4
#define PCTL_DCR_DQ32_BFO		4
#define PCTL_DCR_DQ32_BFW		2
#define PCTL_DCR_DFI_RATE_BFO		8
#define PCTL_DCR_DFI_RATE_BFW		3
#define PCTL_DCR_RANK2_BFO		12
#define PCTL_DCR_RANK2_BFW		1
#define PCTL_DCR_DBI_BFO		17
#define PCTL_DCR_DBI_BFW		1
#define PCTL_DCR_PAR_BFO		18
#define PCTL_DCR_PAR_BFW		1
#define PCTL_DCR_GEAR_DOWN_BFO		19
#define PCTL_DCR_GEAR_DOWN_BFW		1
#define PCTL_DCR_BG2_BFO		21
#define PCTL_DCR_BG2_BFW		1
#define PCTL_DCR_BG_REMAP_BFO		22
#define PCTL_DCR_BG_REMAP_BFW		1
#define PCTL_DCR_ZQC_BFO		30
#define PCTL_DCR_ZQC_BFW		1

#define PCTL_IOCR_ODT_2PINS_BFO		0
#define PCTL_IOCR_ODT_2PINS_BFW		1
#define PCTL_IOCR_STC_CKE_BFO		2
#define PCTL_IOCR_STC_CKE_BFW		1
#define PCTL_IOCR_STC_ODT_BFO		3
#define PCTL_IOCR_STC_ODT_BFW		1
#define PCTL_IOCR_RD_PIPE_BFO		8
#define PCTL_IOCR_RD_PIPE_BFW		4
#define PCTL_IOCR_TPHY_WD_BFO		12
#define PCTL_IOCR_TPHY_WD_BFW		5
#define PCTL_IOCR_TPHY_WL_BFO		17
#define PCTL_IOCR_TPHY_WL_BFW		3
#define PCTL_IOCR_TPHY_RD_EN_BFO	20
#define PCTL_IOCR_TPHY_RD_EN_BFW	5

#define PCTL_CSR_MEM_IDLE_BFO		8
#define PCTL_CSR_MEM_IDLE_BFW		1
#define PCTL_CSR_DT_IDLE_BFO		9
#define PCTL_CSR_DT_IDLE_BFW		1
#define PCTL_CSR_BSTC_IDLE_BFO		10
#define PCTL_CSR_BSTC_IDLE_BFW		1
#define PCTL_CSR_DT_FAIL_BFO		11
#define PCTL_CSR_DT_FAIL_BFW		1
#define PCTL_CSR_BT_FAIL_BFO		12
#define PCTL_CSR_BT_FAIL_BFW		1

#define PCTL_DRR_TRFC_BFO		0
#define PCTL_DRR_TRFC_BFW		7
#define PCTL_DRR_TREF_BFO		8
#define PCTL_DRR_TREF_BFW		16
#define PCTL_DRR_REF_NUM_BFO		24
#define PCTL_DRR_REF_NUM_BFW		4
#define PCTL_DRR_REF_DIS_BFO		28
#define PCTL_DRR_REF_DIS_BFW		1
#define PCTL_DRR_ZQCL_INV_BFO		29
#define PCTL_DRR_ZQCL_INV_BFW		3

#define PCTL_TPR0_TRP_BFO		0
#define PCTL_TPR0_TRP_BFW		4
#define PCTL_TPR0_TRAS_BFO		4
#define PCTL_TPR0_TRAS_BFW		5
#define PCTL_TPR0_TWR_BFO		9
#define PCTL_TPR0_TWR_BFW		4
#define PCTL_TPR0_TRTP_BFO		13
#define PCTL_TPR0_TRTP_BFW		4
#define PCTL_TPR0_TCKE_BFO		17
#define PCTL_TPR0_TCKE_BFW		6
#define PCTL_TPR0_TZQCS_BFO		24
#define PCTL_TPR0_TZQCS_BFW		8

#define PCTL_TPR1_TRRD_BFO		0
#define PCTL_TPR1_TRRD_BFW		4
#define PCTL_TPR1_TRC_BFO		4
#define PCTL_TPR1_TRC_BFW		6
#define PCTL_TPR1_TRCD_BFO		10
#define PCTL_TPR1_TRCD_BFW		4
#define PCTL_TPR1_TCCD_BFO		14
#define PCTL_TPR1_TCCD_BFW		3
#define PCTL_TPR1_TWTR_BFO		17
#define PCTL_TPR1_TWTR_BFW		3
#define PCTL_TPR1_TRTW_BFO		20
#define PCTL_TPR1_TRTW_BFW		4
#define PCTL_TPR1_TFAW_BFO		24
#define PCTL_TPR1_TFAW_BFW		5

#define PCTL_TPR2_INIT_REF_NUM_BFO	0
#define PCTL_TPR2_INIT_REF_NUM_BFW	4
#define PCTL_TPR2_TMRD_BFO		4
#define PCTL_TPR2_TMRD_BFW		4
#define PCTL_TPR2_TNS_BFO		8
#define PCTL_TPR2_TNS_BFW		4
#define PCTL_TPR2_TUS_BFO		12
#define PCTL_TPR2_TUS_BFW		10
#define PCTL_TPR2_RST_TUS_BFO		22
#define PCTL_TPR2_RST_TUS_BFW		10

#define PCTL_TPR3_TRPAB_PB_BFO		0
#define PCTL_TPR3_TRPAB_PB_BFW		2
#define PCTL_TPR3_TRAS_MSB_BFO		2
#define PCTL_TPR3_TRAS_MSB_BFW		1
#define PCTL_TPR3_TFAW_MSB_BFO		3
#define PCTL_TPR3_TFAW_MSB_BFW		1
#define PCTL_TPR3_DPIN_LAT_BFO		4
#define PCTL_TPR3_DPIN_LAT_BFW		3
#define PCTL_TPR3_TPPD_BFO		12
#define PCTL_TPR3_TPPD_BFW		2
#define PCTL_TPR3_TCCD_S_BFO		14
#define PCTL_TPR3_TCCD_S_BFW		3
#define PCTL_TPR3_TWTR_S_BFO		17
#define PCTL_TPR3_TWTR_S_BFW		3
#define PCTL_TPR3_TCCD_R_BFO		29
#define PCTL_TPR3_TCCD_R_BFW		3

#define PCTL_MRINFO_WR_LAT_BFO		0
#define PCTL_MRINFO_WR_LAT_BFW		5
#define PCTL_MRINFO_RD_LAT_BFO		5
#define PCTL_MRINFO_RD_LAT_BFW		5
#define PCTL_MRINFO_ADD_LAT_BFO		10
#define PCTL_MRINFO_ADD_LAT_BFW		5
#define PCTL_MRINFO_TDQSCK_MAX_BFO	15
#define PCTL_MRINFO_TDQSCK_MAX_BFW	3

#define PCTL_MR_BL_BFO			0
#define PCTL_MR_BL_BFW			3
#define PCTL_MR_BT_BFO			3
#define PCTL_MR_BT_BFW			1
#define PCTL_MR_CAS_BFO			4
#define PCTL_MR_CAS_BFW			3
#define PCTL_MR_OP_BFO			8
#define PCTL_MR_OP_BFW			12

#define PCTL_EMR_ADDLAT_BFO		3
#define PCTL_EMR_ADDLAT_BFW		3

#define PCTL_TIE_DPIN_RSTN_BFO		0
#define PCTL_TIE_DPIN_RSTN_BFW		1
#define PCTL_TIE_DPIN_CKE_BFO		1
#define PCTL_TIE_DPIN_CKE_BFW		1
#define PCTL_TIE_DPIN_ODT_BFO		2
#define PCTL_TIE_DPIN_ODT_BFW		1

#define PCTL_BCR_STOP_BFO		0
#define PCTL_BCR_STOP_BFW		1
#define PCTL_BCR_CMP_BFO		1
#define PCTL_BCR_CMP_BFW		1
#define PCTL_BCR_LOOP_BFO		2
#define PCTL_BCR_LOOP_BFW		1
#define PCTL_BCR_DIS_MASK_BFO		3
#define PCTL_BCR_DIS_MASK_BFW		1
#define PCTL_BCR_AT_STOP_BFO		4
#define PCTL_BCR_AT_STOP_BFW		1
#define PCTL_BCR_FLUSH_CMD_BFO		8
#define PCTL_BCR_FLUSH_CMD_BFW		1
#define PCTL_BCR_FLUSH_WD_BFO		9
#define PCTL_BCR_FLUSH_WD_BFW		1
#define PCTL_BCR_FLUSH_RGD_BFO		10
#define PCTL_BCR_FLUSH_RGD_BFW		1
#define PCTL_BCR_FLUSH_RD_BFO		11
#define PCTL_BCR_FLUSH_RD_BFW		1
#define PCTL_BCR_FLUSH_RD_EXPC_BFO	16
#define PCTL_BCR_FLUSH_RD_EXPC_BFW	14

#define PCTL_BST_ERR_FST_TH_BFO		0
#define PCTL_BST_ERR_FST_TH_BFW		12
#define PCTL_BST_ERR_CNT_BFO		16
#define PCTL_BST_ERR_CNT_BFW		14

#define PCTL_BSRAM0_CMD_LEVEL_BFO	0
#define PCTL_BSRAM0_CMD_LEVEL_BFW	12
#define PCTL_BSRAM0_WD_LEVEL_BFO	16
#define PCTL_BSRAM0_WD_LEVEL_BFW	14

#define PCTL_BSRAM1_RG_LEVEL_BFO	0
#define PCTL_BSRAM1_RG_LEVEL_BFW	14
#define PCTL_BSRAM1_RD_LEVEL_BFO	16
#define PCTL_BSRAM1_RD_LEVEL_BFW	14

#define WRAP_MISC_PAGE_SIZE_BFO		0
#define WRAP_MISC_PAGE_SIZE_BFW		4
#define WRAP_MISC_BANK_SIZE_BFO		4
#define WRAP_MISC_BANK_SIZE_BFW		2
#define WRAP_MISC_BST_SIZE_BFO		6
#define WRAP_MISC_BST_SIZE_BFW		3
#define WRAP_MISC_HYBR_SIZE_BFO		10
#define WRAP_MISC_HYBR_SIZE_BFW		2

struct ms_rxi310_portmap {
	volatile unsigned int ccr;		/* 0x000 */
	volatile unsigned int dcr;		/* 0x004 */
	volatile unsigned int iocr;		/* 0x008 */
	volatile unsigned int csr;		/* 0x00c */
	volatile unsigned int drr;		/* 0x010 */
	volatile unsigned int tpr0;		/* 0x014 */
	volatile unsigned int tpr1;		/* 0x018 */
	volatile unsigned int tpr2;		/* 0x01c */
	volatile unsigned int tpr3;		/* 0x020 */
	volatile unsigned int cdpin_ndge;	/* 0x024 */
	volatile unsigned int cdpin;		/* 0x028 */
	volatile unsigned int tdpin;		/* 0x02c */
	volatile unsigned int mrinfo;		/* 0x030 */
	volatile unsigned int mr0;		/* 0x034 */
	volatile unsigned int mr1;		/* 0x038 */
	volatile unsigned int mr2;		/* 0x03c */
	volatile unsigned int mr3;		/* 0x040 */
	volatile unsigned int mr4;		/* 0x044 */
	volatile unsigned int mr5;		/* 0x048 */
	volatile unsigned int mr6;		/* 0x04c */
	volatile unsigned int iocr1;		/* 0x050 */
	volatile unsigned int dllcr4;		/* 0x054 */
	volatile unsigned int dllcr5;		/* 0x058 */
	volatile unsigned int dllcr6;		/* 0x05c */
	volatile unsigned int dpdri;		/* 0x060 */
	volatile unsigned int dpdr;		/* 0x064 */
	volatile unsigned int dqtr0;		/* 0x068 */
	volatile unsigned int dqtr1;		/* 0x06c */
	volatile unsigned int dqtr2;		/* 0x070 */
	volatile unsigned int dqtr3;		/* 0x074 */
	volatile unsigned int dqtr4;		/* 0x078 */
	volatile unsigned int dqtr5;		/* 0x07c */
	volatile unsigned int dqtr6;		/* 0x080 */
	volatile unsigned int dqtr7;		/* 0x084 */
	volatile unsigned int dqstr;		/* 0x088 */
	volatile unsigned int dqsbtr;		/* 0x08c */
	volatile unsigned int odtcr;		/* 0x090 */
	volatile unsigned int dtr0;		/* 0x094 */
	volatile unsigned int dtr1;		/* 0x098 */
	volatile unsigned int dtar;		/* 0x09c */
	volatile unsigned int zqcr0;		/* 0x0a0 */
	volatile unsigned int zqcr1;		/* 0x0a4 */
	volatile unsigned int zqsr;		/* 0x0a8 */
	volatile unsigned int rslr0;		/* 0x0ac */
	volatile unsigned int rslr1;		/* 0x0b0 */
	volatile unsigned int rslr2;		/* 0x0b4 */
	volatile unsigned int rslr3;		/* 0x0b8 */
	volatile unsigned int rdgr0;		/* 0x0bc */
	volatile unsigned int rdgr1;		/* 0x0c0 */
	volatile unsigned int rdgr2;		/* 0x0c4 */
	volatile unsigned int rdgr3;		/* 0x0c8 */
	volatile unsigned int mxsl;		/* 0x0cc */
	volatile unsigned int bcr;		/* 0x0d0 */
	volatile unsigned int bct;		/* 0x0d4 */
	volatile unsigned int bcm;		/* 0x0d8 */
	volatile unsigned int bst;		/* 0x0dc */
	volatile unsigned int bsram0;		/* 0x0e0 */
	volatile unsigned int bsram1;		/* 0x0e4 */
	volatile unsigned int ber;		/* 0x0e8 */
	volatile unsigned int byr;		/* 0x0ec */
	volatile unsigned int pctl_svn;		/* 0x0f0 */
	volatile unsigned int reserved4[1];
	volatile unsigned int pctl_idr;		/* 0x0f8 */
	volatile unsigned int err;		/* 0x0fc */
	volatile unsigned int phy_dly0;		/* 0x100 */
	volatile unsigned int phy_dly1_rst;	/* 0x104 */
	volatile unsigned int phy_dly_clk;	/* 0x108 */
	volatile unsigned int phy_dly_st;	/* 0x10c */
	volatile unsigned int phy_dly_num;	/* 0x110 */
	volatile unsigned int reserved2[68];
	volatile unsigned int misc;		/* 0x224 */
	volatile unsigned int cq_ver;		/* 0x228 */
	volatile unsigned int cq_mon;		/* 0x22c */
	volatile unsigned int wq_ver;		/* 0x230 */
	volatile unsigned int wq_mon;		/* 0x234 */
	volatile unsigned int rq_ver;		/* 0x240 */
	volatile unsigned int rq_mon;		/* 0x244 */
	volatile unsigned int reserved3[22];
	volatile unsigned int wwrap_idr;	/* 0x2a0 */
	volatile unsigned int wrap_svn;		/* 0x2a4 */
	volatile unsigned int reserved5[22];
	volatile unsigned int sch_index;	/* 0x300 */
	volatile unsigned int sch_data;		/* 0x304 */
	volatile unsigned int prof_index;	/* 0x308 */
	volatile unsigned int prof_data;	/* 0x30c */
};

#define QFIFO_CMD_BANK_BFO		34
#define QFIFO_CMD_BANK_BFW		3
#define QFIFO_CMD_PAGE_BFO		18
#define QFIFO_CMD_PAGE_BFW		16
#define QFIFO_CMD_COLU_BFO		5
#define QFIFO_CMD_COLU_BFW		12
#define QFIFO_BST_LEN_BFO		1
#define QFIFO_BST_LEN_BFW		4
#define QFIFO_CMD_WRRD_BFO		0
#define QFIFO_CMD_WRRD_BFW		1

enum qfifo_wrrd {
	RD_CMD = 0,
	WR_CMD = 1
};

struct bstc_cmd_data_b37 {
	uint32_t CMD_BANK;
	uint32_t CMD_PAGE;
	uint32_t CMD_COLU;
	uint32_t BST_LEN;
	enum qfifo_wrrd WRRD;
};

struct bstc_data_b128 {
	uint32_t data_3;
	uint32_t data_2;
	uint32_t data_1;
	uint32_t data_0;
};

struct bstc_data_b64 {
	uint32_t data_1;
	uint32_t data_0;
};

struct dpi_pll_portmap {
	volatile unsigned int ADR_DLY_0;		/* 0x000 */
	volatile unsigned int ADR_DLY_1;		/* 0x004 */
	volatile unsigned int ADR_DLY_2;		/* 0x008 */
	volatile unsigned int CMD_DLY_0;		/* 0x00C */
	volatile unsigned int PAD_OE_CTL;		/* 0x010 */
	volatile unsigned int PAD_IDDQ;			/* 0x014 */
	volatile unsigned int PAD_ZCTRL_OVRD;		/* 0x018 */
	volatile unsigned int PAD_CTRL_PROG;		/* 0x01C */
	volatile unsigned int PAD_ZCTRL_STATUS;		/* 0x020 */
	volatile unsigned int WRLVL_CTRL;		/* 0x024 */
	volatile unsigned int WRLVL_RD_DATA;		/* 0x028 */
	volatile unsigned int TEST_CTRL0;		/* 0x02C */
	volatile unsigned int DQ_DLY_0_0;		/* 0x030 */
	volatile unsigned int DQ_DLY_0_1;		/* 0x034 */
	volatile unsigned int DQ_DLY_0_2;		/* 0x038 */
	volatile unsigned int DQ_DLY_0_3;		/* 0x03C */
	volatile unsigned int DQ_DLY_1;			/* 0x040 */
	volatile unsigned int READ_CTRL_0_0;		/* 0x044 */
	volatile unsigned int READ_CTRL_0_1;		/* 0x048 */
	volatile unsigned int READ_CTRL_0_2;		/* 0x04C */
	volatile unsigned int READ_CTRL_0_3;		/* 0x050 */
	volatile unsigned int READ_CTRL_1;		/* 0x054 */
	volatile unsigned int READ_CTRL_2;		/* 0x058 */
	volatile unsigned int ODT_TTC_SET2;		/* 0x05C */
	volatile unsigned int ODT_TTC_SET3;		/* 0x060 */
	volatile unsigned int ODT_TTF_SET6;		/* 0x064 */
	volatile unsigned int RX_OFFSET_0_0;		/* 0x068 */
	volatile unsigned int RX_OFFSET_0_1;		/* 0x06C */
	volatile unsigned int RX_OFFSET_0_2;		/* 0x070 */
	volatile unsigned int RX_OFFSET_0_3;		/* 0x074 */
	volatile unsigned int RX_OFFSET_1_0;		/* 0x078 */
	volatile unsigned int RX_OFFSET_1_1;		/* 0x07C */
	volatile unsigned int RX_OFFSET_1_2;		/* 0x080 */
	volatile unsigned int RX_OFFSET_1_3;		/* 0x084 */
	volatile unsigned int RX_OFFSET_2;		/* 0x088 */
	volatile unsigned int PAD_RZCTRL_STATUS;	/* 0x08C */
	volatile unsigned int DQ_DLY_2;			/* 0x090 */
	volatile unsigned int DCC_0_0;			/* 0x094 */
	volatile unsigned int DCC_0_1;			/* 0x098 */
	volatile unsigned int DCC_0_2;			/* 0x09C */
	volatile unsigned int DCC_0_3;			/* 0x0A0 */
	volatile unsigned int DCC_0_4;			/* 0x0A4 */
	volatile unsigned int DCC_1_0;			/* 0x0A8 */
	volatile unsigned int DCC_1_1;			/* 0x0AC */
	volatile unsigned int DCC_1_2;			/* 0x0B0 */
	volatile unsigned int DCC_1_3;			/* 0x0B4 */
	volatile unsigned int DCC_1_4;			/* 0x0B8 */
	volatile unsigned int DCC_2_0;			/* 0x0BC */
	volatile unsigned int DCC_2_1;			/* 0x0C0 */
	volatile unsigned int DCC_2_2;			/* 0x0C4 */
	volatile unsigned int DCC_2_3;			/* 0x0C8 */
	volatile unsigned int DCC_2_4;			/* 0x0CC */
	volatile unsigned int DT_SET0;			/* 0x0D0 */
	volatile unsigned int ODT_TTF_SET2;		/* 0x0D4 */
	volatile unsigned int ODT_TTF_SET3;		/* 0x0D8 */
	volatile unsigned int ODT_TTF_SET5;		/* 0x0DC */
	volatile unsigned int ODT_TTC_SET0;		/* 0x0E0 */
	volatile unsigned int ODT_TTC_SET1;		/* 0x0E4 */
	volatile unsigned int ODT_TTC_SET4;		/* 0x0E8 */
	volatile unsigned int ODT_TTF_SET0;		/* 0x0EC */
	volatile unsigned int ODT_TTF_SET1;		/* 0x0F0 */
	volatile unsigned int ODT_TTF_SET4;		/* 0x0F4 */
	volatile unsigned int OCD_SET0;			/* 0x0F8 */
	volatile unsigned int OCD_SET1;			/* 0x0FC */
	volatile unsigned int OCD_SET2;			/* 0x100 */
	volatile unsigned int OCD_SET3;			/* 0x104 */
	volatile unsigned int OCD_SET4;			/* 0x108 */
	volatile unsigned int PAD_BUS_0;		/* 0x10C */
	volatile unsigned int PAD_BUS_1;		/* 0x110 */
	volatile unsigned int ZQCMP;			/* 0x114 */
	volatile unsigned int DPI_CTRL_0;		/* 0x118 */
	volatile unsigned int DPI_CTRL_1;		/* 0x11C */
	volatile unsigned int TEST_CTRL1;		/* 0x120 */
	volatile unsigned int FIFO_ADR0_0;		/* 0x124 */
	volatile unsigned int FIFO_ADR0_1;		/* 0x128 */
	volatile unsigned int FIFO_ADR0_2;		/* 0x12C */
	volatile unsigned int FIFO_ADR0_3;		/* 0x130 */
	volatile unsigned int FIFO_ADR1_0;		/* 0x134 */
	volatile unsigned int FIFO_ADR1_1;		/* 0x138 */
	volatile unsigned int FIFO_ADR1_2;		/* 0x13C */
	volatile unsigned int FIFO_ADR1_3;		/* 0x140 */
	volatile unsigned int DQ_SHIFTER_0_0;		/* 0x144 */
	volatile unsigned int DQ_SHIFTER_0_1;		/* 0x148 */
	volatile unsigned int DQ_SHIFTER_0_2;		/* 0x14C */
	volatile unsigned int DQ_SHIFTER_0_3;		/* 0x150 */
	volatile unsigned int DQ_SHIFTER_1_0;		/* 0x154 */
	volatile unsigned int DQ_SHIFTER_1_1;		/* 0x158 */
	volatile unsigned int DQ_SHIFTER_1_2;		/* 0x15C */
	volatile unsigned int DQ_SHIFTER_1_3;		/* 0x160 */
	volatile unsigned int DUMMY_0;			/* 0x164 */
	volatile unsigned int DUMMY_1;			/* 0x168 */
	volatile unsigned int DUMMY_2;			/* 0x16C */
	volatile unsigned int DUMMY_3;			/* 0x170 */
	volatile unsigned int ODT_TTC_SET5;		/* 0x174 */
	volatile unsigned int ODT_TTC_SET6;		/* 0x178 */
	volatile unsigned int RX_WATCHDOG;		/* 0x17C */
	volatile unsigned int RX_PLL;			/* 0x180 */
	volatile unsigned int RX_FILT_0;		/* 0x184 */
	volatile unsigned int RX_FILT_1;		/* 0x188 */
	volatile unsigned int RX_FILT_2;		/* 0x18c */
	volatile unsigned int RX_FILT_3;		/* 0x190 */
	volatile unsigned int CRT_ALL;			/* 0x194 */
	volatile unsigned int reserved_2[2];
	volatile unsigned int BIST_2TO1_0;		/* 0x1A0 */
	volatile unsigned int BIST_2TO1_1;		/* 0x1A4 */
	volatile unsigned int BIST_PT;			/* 0x1A8 */
	volatile unsigned int INT_CTRL;			/* 0x1AC */
	volatile unsigned int INT_STATUS_0;		/* 0x1B0 */
	volatile unsigned int BIST_DEBUG;		/* 0x1B4 */
	volatile unsigned int reserved_3[3];
	volatile unsigned int INT_STATUS_2;		/* 0x1C4 */
	volatile unsigned int PLL_EN;			/* 0x1C8 */
	volatile unsigned int DCC_3_0;			/* 0x1CC */
	volatile unsigned int DCC_3_1;			/* 0x1D0 */
	volatile unsigned int DCC_3_2;			/* 0x1D4 */
	volatile unsigned int DCC_3_3;			/* 0x1D8 */
	volatile unsigned int DCC_3_4;			/* 0x1DC */
};

struct dpi_dll_portmap {
	volatile unsigned int ADR_DLY_0;		/* 0x000 */
	volatile unsigned int ADR_DLY_1;		/* 0x004 */
	volatile unsigned int ADR_DLY_2;		/* 0x008 */
	volatile unsigned int CMD_DLY_0;		/* 0x00C */
	volatile unsigned int PAD_OE_CTL;		/* 0x010 */
	volatile unsigned int PAD_IDDQ;			/* 0x014 */
	volatile unsigned int PAD_ZCTRL_OVRD;		/* 0x018 */
	volatile unsigned int PAD_CTRL_PROG;		/* 0x01C */
	volatile unsigned int PAD_ZCTRL_STATUS;		/* 0x020 */
	volatile unsigned int WRLVL_CTRL;		/* 0x024 */
	volatile unsigned int WRLVL_RD_DATA;		/* 0x028 */
	volatile unsigned int TEST_CTRL0;		/* 0x02C */
	volatile unsigned int DQ_DLY_0_0;		/* 0x030 */
	volatile unsigned int DQ_DLY_0_1;		/* 0x034 */
	volatile unsigned int DQ_DLY_0_2;		/* 0x038 */
	volatile unsigned int DQ_DLY_0_3;		/* 0x03C */
	volatile unsigned int DQ_DLY_1;			/* 0x040 */
	volatile unsigned int READ_CTRL_0_0;		/* 0x044 */
	volatile unsigned int READ_CTRL_0_1;		/* 0x048 */
	volatile unsigned int READ_CTRL_0_2;		/* 0x04C */
	volatile unsigned int READ_CTRL_0_3;		/* 0x050 */
	volatile unsigned int READ_CTRL_1;		/* 0x054 */
	volatile unsigned int READ_CTRL_2;		/* 0x058 */
	volatile unsigned int ODT_TTC_SET2;		/* 0x05C */
	volatile unsigned int ODT_TTC_SET3;		/* 0x060 */
	volatile unsigned int ODT_TTF_SET6;		/* 0x064 */
	volatile unsigned int CAL_SHIFT_CTRL;		/* 0x068 */
	volatile unsigned int CAL_LS_SEL_0;		/* 0x06C */
	volatile unsigned int CAL_LS_SEL_1;		/* 0x070 */
	volatile unsigned int CAL_LS_SEL_2;		/* 0x074 */
	volatile unsigned int CAL_LS_SEL_3;		/* 0x078 */
	volatile unsigned int CAL_RS_SEL_0;		/* 0x07C */
	volatile unsigned int CAL_RS_SEL_1;		/* 0x080 */
	volatile unsigned int CAL_RS_SEL_2;		/* 0x084 */
	volatile unsigned int CAL_RS_SEL_3;		/* 0x088 */
	volatile unsigned int DQS_IN_DLY_0_0;		/* 0x08C */
	volatile unsigned int DQS_IN_DLY_0_1;		/* 0x090 */
	volatile unsigned int DQS_IN_DLY_0_2;		/* 0x094 */
	volatile unsigned int DQS_IN_DLY_0_3;		/* 0x098 */
	volatile unsigned int DQS_IN_DLY_1_0;		/* 0x09C */
	volatile unsigned int DQS_IN_DLY_1_1;		/* 0x0A0 */
	volatile unsigned int DQS_IN_DLY_1_2;		/* 0x0A4 */
	volatile unsigned int DQS_IN_DLY_1_3;		/* 0x0A8 */
	volatile unsigned int DQS_IN_DLY_2_0;		/* 0x0AC */
	volatile unsigned int DQS_IN_DLY_2_1;		/* 0x0B0 */
	volatile unsigned int DQS_IN_DLY_2_2;		/* 0x0B4 */
	volatile unsigned int DQS_IN_DLY_2_3;		/* 0x0B8 */
	volatile unsigned int DQS_IN_DLY_3_0;		/* 0x0BC */
	volatile unsigned int DQS_IN_DLY_3_1;		/* 0x0C0 */
	volatile unsigned int DQS_IN_DLY_3_2;		/* 0x0C4 */
	volatile unsigned int DQS_IN_DLY_3_3;		/* 0x0C8 */
	volatile unsigned int ANA_DLL_0;		/* 0x0CC */
	volatile unsigned int DT_SET0;			/* 0x0D0 */
	volatile unsigned int ODT_TTF_SET2;		/* 0x0D4 */
	volatile unsigned int ODT_TTF_SET3;		/* 0x0D8 */
	volatile unsigned int ODT_TTF_SET5;		/* 0x0DC */
	volatile unsigned int ODT_TTC_SET0;		/* 0x0E0 */
	volatile unsigned int ODT_TTC_SET1;		/* 0x0E4 */
	volatile unsigned int ODT_TTC_SET4;		/* 0x0E8 */
	volatile unsigned int ODT_TTF_SET0;		/* 0x0EC */
	volatile unsigned int ODT_TTF_SET1;		/* 0x0F0 */
	volatile unsigned int ODT_TTF_SET4;		/* 0x0F4 */
	volatile unsigned int OCD_SET0;			/* 0x0F8 */
	volatile unsigned int OCD_SET1;			/* 0x0FC */
	volatile unsigned int OCD_SET2;			/* 0x100 */
	volatile unsigned int OCD_SET3;			/* 0x104 */
	volatile unsigned int OCD_SET4;			/* 0x108 */
	volatile unsigned int PAD_BUS_0;		/* 0x10C */
	volatile unsigned int PAD_BUS_1;		/* 0x110 */
	volatile unsigned int ZQCMP;			/* 0x114 */
	volatile unsigned int DPI_CTRL_0;		/* 0x118 */
	volatile unsigned int DPI_CTRL_1;		/* 0x11C */
	volatile unsigned int CAL_OUT_SEL;		/* 0x120 */
	volatile unsigned int CAL_OUT_0_0;		/* 0x124 */
	volatile unsigned int CAL_OUT_0_1;		/* 0x128 */
	volatile unsigned int CAL_OUT_0_2;		/* 0x12C */
	volatile unsigned int CAL_OUT_0_3;		/* 0x130 */
	volatile unsigned int WR_FIFO_0_0;		/* 0x134 */
	volatile unsigned int WR_FIFO_0_1;		/* 0x138 */
	volatile unsigned int WR_FIFO_0_2;		/* 0x13C */
	volatile unsigned int WR_FIFO_0_3;		/* 0x140 */
	volatile unsigned int WR_FIFO_1_0;		/* 0x144 */
	volatile unsigned int WR_FIFO_1_1;		/* 0x148 */
	volatile unsigned int WR_FIFO_1_2;		/* 0x14C */
	volatile unsigned int WR_FIFO_1_3;		/* 0x150 */
	volatile unsigned int RD_FIFO_0_0;		/* 0x154 */
	volatile unsigned int RD_FIFO_0_1;		/* 0x158 */
	volatile unsigned int RD_FIFO_0_2;		/* 0x15C */
	volatile unsigned int RD_FIFO_0_3;		/* 0x160 */
	volatile unsigned int RD_FIFO_1_0;		/* 0x164 */
	volatile unsigned int RD_FIFO_1_1;		/* 0x168 */
	volatile unsigned int RD_FIFO_1_2;		/* 0x16C */
	volatile unsigned int RD_FIFO_1_3;		/* 0x170 */
	volatile unsigned int ODT_TTC_SET5;		/* 0x174 */
	volatile unsigned int ODT_TTC_SET6;		/* 0x178 */
	volatile unsigned int TEST_CTRL1;		/* 0x17C */
	volatile unsigned int DQ_DLY_2;			/* 0x180 */
	volatile unsigned int reserved_0[1];
	volatile unsigned int PAD_RZCTRL_STATUS;	/* 0x188 */
	volatile unsigned int ANA_DLL_1;		/* 0x18C */
	volatile unsigned int reserved_1[1];
	volatile unsigned int CRT_ALL;			/* 0x194 */
	volatile unsigned int BIST_DEBUG;		/* 0x198 */
	volatile unsigned int reserved_2[1];
	volatile unsigned int BIST_2TO1_0;		/* 0x1A0 */
	volatile unsigned int BIST_2TO1_1;		/* 0x1A4 */
	volatile unsigned int BIST_PT;			/* 0x1A8 */
	volatile unsigned int INT_CTRL;			/* 0x1AC */
	volatile unsigned int INT_STATUS_0;		/* 0x1B0 */
	volatile unsigned int INT_STATUS_1_0;		/* 0x1B4 */
	volatile unsigned int INT_STATUS_1_1;		/* 0x1B8 */
	volatile unsigned int INT_STATUS_1_2;		/* 0x1BC */
	volatile unsigned int INT_STATUS_1_3;		/* 0x1C0 */
	volatile unsigned int INT_STATUS_2;		/* 0x1C4 */
	volatile unsigned int PLL_DLL_EN;		/* 0x1C8 */
	volatile unsigned int DUMMY_0;			/* 0x1CC */
	volatile unsigned int DUMMY_1;			/* 0x1D0 */
	volatile unsigned int DUMMY_2;			/* 0x1D4 */
	volatile unsigned int DUMMY_3;			/* 0x1D8 */
};

enum dram_type {
	NON_DDR = 0,
	DDR_1 = 1,
	DDR_2 = 2,
	DDR_3 = 3,
	DDR_4 = 4,
	SDR = 8,
	LPDDR_2 = 10,
	LPDDR_3 = 11,
	LPDDR_4 = 12
};

enum dram_page_size {
	PAGE_256K = 0,
	PAGE_512K = 1,
	PAGE_1K = 2,
	PAGE_2K = 3,
	PAGE_4K = 4,
	PAGE_8K = 5,
	PAGE_16K = 6,
	PAGE_32K = 7,
	PAGE_64K = 8
};

enum dram_bank_size {
	BANK_2 = 0,
	BANK_4 = 1,
	BANK_8 = 2,
	BANK_16 = 3
};

enum dram_dq_width {
	DQ_16 = 0,
	DQ_32 = 1,
	HALF_DQ32 = 2
};

enum mode0_bst_len {
	BST_LEN_4 = 0,
	BST_LEN_FLY = 1,
	BST_LEN_8 = 2,
	BST_LEN_16 = 3
};

enum mode0_bst_type {
	SENQUENTIAL = 0,
	INTERLEAVE = 1
};

enum dfi_ratio_type {
	DFI_RATIO_1 = 0,	/* DFI= 1:1, or SDR */
	DFI_RATIO_2 = 1,
	DFI_RATIO_4 = 2
};

/* write tck */
enum dram_wr_tck {
	WR_TCK_5 = 5,
	WR_TCK_6 = 6,
	WR_TCK_7 = 7,
	WR_TCK_8 = 8,
	WR_TCK_9 = 9,
	WR_TCK_10 = 10,
	WR_TCK_11 = 11,
	WR_TCK_12 = 12,
	WR_TCK_13 = 13,
	WR_TCK_14 = 14,
	WR_TCK_15 = 15,
	WR_TCK_16 = 16,
	WR_TCK_17 = 17,
	WR_TCK_18 = 18,
	WR_TCK_19 = 19,
	WR_TCK_20 = 20
};

/* timing tccd tck */
enum timing_tccd_tck {
	TCCD_TCK_4 = 4,
	TCCD_TCK_5 = 5,
	TCCD_TCK_6 = 6,
	TCCD_TCK_7 = 7,
	TCCD_TCK_8 = 8
};

struct dram_device_info {
	struct dram_info *dev;
	struct dram_mode_reg_info *mode_reg;
	struct dram_timing_info *timing;
	uint32_t ddr_period_ps;
	enum dfi_ratio_type dfi_rate;
};

struct dram_info {
	enum dram_type device_type;
	enum dram_page_size page;
	enum dram_bank_size bank;
	enum dram_dq_width dq_width;
};

struct dram_mode_reg_info {
	enum mode0_bst_len bst_len;
	enum mode0_bst_type bst_type;
	uint32_t mode0_cas;
	uint32_t mode0_wr;
	uint32_t mode1_dll_en_n;
	uint32_t mode1_all_lat;
	uint32_t mode2_cwl;
	uint32_t dram_par_lat;
	uint32_t dram_wr_lat;
	uint32_t dram_rd_lat;
	uint32_t dram_add_lat;
};

struct dram_timing_info {
	uint32_t  trfc_ps;
	uint32_t  trefi_ps;
	uint32_t  wr_max_tck;
	uint32_t  trcd_ps;
	uint32_t  trp_ps;
	uint32_t  tras_ps;
	uint32_t  trrd_tck;
	uint32_t  twr_ps;
	uint32_t  twtr_tck;
	uint32_t  tmrd_tck;
	uint32_t  trtp_tck;
	uint32_t  tccd_tck;
	uint32_t  trc_ps;
	uint32_t  tccd_s_tck;
	uint32_t  twtr_s_tck;
	uint32_t  tfaw_ps;
	uint32_t  tcke_tck;
};

#define SDR_IDR			0x03100000
#define DDR2_IDR		0x0002dd20
#define DDR3_IDR		0x0002dd30
#define DDR2_IDR_EXTRA		0x0002dd21
#define DDR3_IDR_EXTRA		0x0002dd31
#define WRAP_CR_SDR_MISC_DQ32	0x13
#define WRAP_CR_SDR_MISC_DQ16	0x12
#define PCTL_CR_SDR_DCR_DQ32	0x18		/* #SDR, DQ32 */
#define PCTL_CR_SDR_DCR_DQ16	0x28		/* #SDR, DQ16 */
#define PCTL_CR_SDR_IOCR	0x100		/* #rd_pipe=2 */
#define PCTL_CR_SDR_MRINFO	0x60		/* {add_lat, rd_lat=3, wr_lat} */
#define PCTL_CR_SDR_MR0		0x32
#define PCTL_CR_SDR_DRR		0x11010004	/* #tref=0x8af0, trfc=0x6 */
#define PCTL_CR_SDR_TPR0	0x2621
#define PCTL_CR_SDR_TPR1	0x6883
#define PCTL_CR_SDR_TPR2	0x2044
#define PCTL_CR_SDR_DLY0	0x80

#define WRAP_CR_MISC_DQ32	0x64
#define WRAP_CR_MISC_DQ16	0x63

#define PCTL_CR_DDR2_DCR_DQ32	0x212		/* #DFI=2:1, DDR2, dq32 */
#define PCTL_CR_DDR2_DCR_DQ16	0x202		/* #DFI=2:1, DDR2 */
#define PCTL_CR_DDR2_IOCR	0x1000		/* #tphy_wrdata= 0x1 */
#define PCTL_CR_DDR2_MRINFO	0x42		/* #WL=3, RL=4, AL=0 */
#define PCTL_CR_DDR2_MR0	0x1063
#define PCTL_CR_DDR2_MR1	0x6
#define PCTL_CR_DDR2_MR2	0x00		/* #CWL_Latency */
#define PCTL_CR_DDR2_DRR	0x09050005
#define PCTL_CR_DDR2_TPR0	0x2221
#define PCTL_CR_DDR2_TPR1	0x2524421
#define PCTL_CR_DDR2_TPR2	0x40102		/* #tmrd=2 */

#define PCTL_CR_DDR3_DCR_DQ32	0x213		/* #DFI=2:1, DDR3, dq32 */
#define PCTL_CR_DDR3_DCR_DQ16	0x203		/* #DFI=2:1, DDR3 */
#define PCTL_CR_DDR3_IOCR	0x1000		/* #tphy_wrdata= 0x1 */
#define PCTL_CR_DDR3_MRINFO	0x63		/* #WL=3, RL=4, AL=0 */
#define PCTL_CR_DDR3_MR0	0x220
#define PCTL_CR_DDR3_MR1	0x6
#define PCTL_CR_DDR3_MR2	0x0		/* #CWL_Latency =5 */
#define PCTL_CR_DDR3_DRR	0x09050005
#define PCTL_CR_DDR3_TPR0	0x6221
#define PCTL_CR_DDR3_TPR1	0x3568432
#define PCTL_CR_DDR3_TPR2	0x1040182
#define PCTL_CR_CSR		0x700

enum dfi_ratio_type  dfi_ratio_1 = DFI_RATIO_1;
enum dfi_ratio_type  dfi_ratio_2 = DFI_RATIO_2;
enum dfi_ratio_type  dfi_ratio_4 = DFI_RATIO_4;

/* DRAM DIE INFO */
struct dram_info sdr_dev = {
	SDR,
	PAGE_512K,
	BANK_4,
	DQ_32
};

struct dram_info sdr_dev_half_dq = {
	SDR,
	PAGE_512K,
	BANK_4,
	DQ_16
};

struct dram_info ddr1_dev = {
	DDR_1,
	PAGE_1K,
	BANK_4,
	DQ_32
};

struct dram_info ddr2_dev = {
	DDR_2,
	PAGE_1K,
	BANK_8,
	DQ_32
};

struct dram_info ddr2_dev_half_dq = {
	DDR_2,
	PAGE_1K,
	BANK_8,
	HALF_DQ32
};

struct dram_info ddr3_dev = {
	DDR_3,
	PAGE_1K,
	BANK_8,
	DQ_32
};

struct dram_info ddr3_dev_half_dq = {
	DDR_3,
	PAGE_1K,
	BANK_8,
	HALF_DQ32
};

struct dram_info ddr4_dev = {
	DDR_4,
	PAGE_1K,
	BANK_8,
	DQ_32
};

struct dram_info ddr4_dev_half_dq = {
	DDR_4,
	PAGE_1K,
	BANK_8,
	HALF_DQ32
};
/* ddr3 device */
struct dram_info rle0629_dev = {
	DDR_3,
	PAGE_1K,
	BANK_8,
	HALF_DQ32
};

/* DRAM DEVICE MODE REG INFO */
struct dram_mode_reg_info sdr_mode_reg_160mhz = {
	BST_LEN_4,
	SENQUENTIAL,
	0x3,	/* mode0_cas: 6 */
	0x1,	/* mode0_wr:5 */
	0,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	0,	/* mode2_cwl:5 */
	0,	/* parity_lat */
	0,	/* cr_mrinfo_wr_lat */
	3,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_latdram_info-> mode_reg-> dram_add_lat */
};

struct dram_mode_reg_info ddr1_mode_reg_400mhz = {
	BST_LEN_4,
	SENQUENTIAL,
	0x3,	/* mode0_cas: 3 */
	0x0,	/* mode0_wr */
	0,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	0,	/* mode2_cwl:5 */
	0,	/* parity_lat */
	1,	/* cr_mrinfo_wr_lat */
	2,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_lat */
};

struct dram_mode_reg_info ddr2_mode_reg_160mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x6,	/* mode0_cas: 6 */
	0x1,	/* mode0_wr:5 */
	0,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	0,	/* mode2_cwl:5 */
	0,	/* parity_lat */
	5,	/* cr_mrinfo_wr_lat */
	6,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr3_mode_reg_160mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x4,	/* mode0_cas: 6 */
	0x1, 	/* mode0_wr:5 */
	0,   	/* mode1_dll_en_n */
	0,   	/* mode1_all_lat=0 */
	0,   	/* mode2_cwl:5 */
	0,   	/* parity_lat */
	6,   	/* cr_mrinfo_wr_lat */
	5,   	/* cr_mrinfo_rd_lat */
	0    	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr3_mode_reg_1600mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0xc,	/* mode0_cas: 10 */
	0x6,	/* mode0_wr:12 */
	0,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	3,	/* mode2_cwl:8 */
	0,	/* parity_lat */
	8,	/* cr_mrinfo_wr_lat */
	10,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr3_mode_reg_1866mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x3,	/* mode0_cas: 13 */
	0x7,	/* mode0_wr:14 */
	0,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	4,	/* mode2_cwl:9 */
	0,	/* parity_lat */
	9,	/* cr_mrinfo_wr_lat */
	13,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr3_mode_reg_2133mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x5,	/* mode0_cas: 14 */
	0x0,	/* mode0_wr:16 */
	0,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	5,	/* mode2_cwl:10 */
	0,	/* parity_lat */
	10,	/* cr_mrinfo_wr_lat */
	14,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr4_mode_reg_160mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x0,	/* mode0_cas: 9 */
	0x0,	/* mode0_wr:10 */
	1,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	0,	/* mode2_cwl:9 */
	0,	/* parity_lat */
	9,	/* cr_mrinfo_wr_lat */
	9,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr4_mode_reg_1600mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x2, /* mode0_cas: 11 */
	0x1, /* mode0_wr:12 */
	1,   /* mode1_dll_en_n */
	0,   /* mode1_all_lat=0 */
	0,   /* mode2_cwl:9 */
	0,   /* parity_lat */
	9,   /* cr_mrinfo_wr_lat */
	11,  /* cr_mrinfo_rd_lat */
	0    /* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr4_mode_reg_1866mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x3,	/* mode0_cas: 12 */
	0x1,	/* mode0_wr:12 */
	1,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	1,	/* mode2_cwl:10 */
	0,	/* parity_lat */
	10,	/* cr_mrinfo_wr_lat */
	12,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr4_mode_reg_2133mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x6,	/* mode0_cas: 15 */
	0x3,	/* mode0_wr:16 */
	1,	/* mode1_dll_en */
	0,	/* mode1_all_lat=0 */
	0x2,	/* mode2_cwl:11 */
	0,	/* parity_lat */
	11,	/* cr_mrinfo_wr_lat */
	15,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

struct dram_mode_reg_info ddr4_mode_reg_2400mhz = {
	BST_LEN_8,
	SENQUENTIAL,
	0x7,	/* mode0_cas: 16 */
	0x5,	/* mode0_wr:18.007 */
	1,	/* mode1_dll_en_n */
	0,	/* mode1_all_lat=0 */
	0x3,	/* mode2_cwl:12 */
	0,	/* parity_lat */
	12,	/* cr_mrinfo_wr_lat */
	16,	/* cr_mrinfo_rd_lat */
	0	/* cr_mrinfo_add_lat */
};

/* DRAM DEVICE TIMEING INFO */
struct dram_timing_info sdr_timing_160mhz = {
	60000,		/* trfc_ps;    */
	7812500,	/* trefi_ps;   */
	2,		/* wr_max_tck; */
	18000,		/* trcd_ps;    */
	18000,		/* trp_ps;     */
	42000,		/* tras_ps;    */
	4,		/* trrd_tck;   */
	2,		/* twr_tck;    */
	0,		/* twtr_tck;   */
	2,		/* tmrd_tck;   */
	4,		/* trtp_tck;   */
	2,		/* tccd_tck;   */
	60000,		/* trc_ps;     */
	0,		/* tccd_s_tck; */
	0,		/* twtr_s_tck; */
	50000,		/* tfaw_ps;    */
	1,		/* tcke_tck;   */
};

struct dram_timing_info ddr1_timing_400mhz = {
	66000,		/* trfc_ps;    */
	69999800,	/* trefi_ps;   */
	2,		/* wr_max_tck; */
	15000,		/* trcd_ps;    */
	15000,		/* trp_ps;     */
	40000,		/* tras_ps;    */
	2,		/* trrd_tck;   */
	1500,		/* twr_ps;     */
	2,		/* twtr_tck;   */
	2,		/* tmrd_tck;   */
	0,		/* trtp_tck;   */
	1,		/* tccd_tck;   */
	55000,		/* trc_ps;     */
	0,		/* tccd_s_tck; */
	0,		/* twtr_s_tck; */
	40000,		/* tfaw_ps;    */
	1,		/* tcke_tck;   */
};

struct dram_timing_info ddr3_timing_160mhz = {
	110000,		/* trfc_ps;    */
	70312500,	/* trefi_ps;   */
	16,		/* wr_max_tck; */
	14000,		/* trcd_ps;    */
	14000,		/* trp_ps;     */
	36000,		/* tras_ps;    */
	4,		/* trrd_tck;   */
	15000,		/* twr_ps;     */
	4,		/* twtr_tck;   */
	4,		/* tmrd_tck;   */
	4,		/* trtp_tck;   */
	4,		/* tccd_tck;   */
	50090,		/* trc_ps;     */
	0,		/* tccd_s_tck; */
	0,		/* twtr_s_tck; */
	40000,		/* tfaw_ps;    */
	3,		/* tcke_tck;   */
};

struct dram_timing_info ddr3_timing_1600mhz = {
	110000,		/* trfc_ps;    */
	70312500,	/* trefi_ps;   */
	12,		/* wr_max_tck; */
	13750,		/* trcd_ps;    */
	13750,		/* trp_ps;     */
	35000,		/* tras_ps;    */
	5,		/* trrd_tck;   */
	15000,		/* twr_ps;     */
	6,		/* twtr_tck;   */
	7,
	6,		/* trtp_tck;   */
	4,		/* tccd_tck;   */
	48750,		/* trc_ps;     */
	0,		/* tccd_s_tck; */
	0,		/* twtr_s_tck; */
	40000,		/* tfaw_ps; */
	4,		/* tcke_tck; */
};

struct dram_timing_info ddr3_timing_1866mhz = {
	110000,		/* trfc_ps; */
	70312500,	/* trefi_ps; */
	16,		/* wr_max_tck; */
	13910,		/* trcd_ps; */
	13910,		/* trp_ps; */
	34000,		/* tras_ps; */
	6,		/* trrd_tck; */
	15000,		/* twr_ps; */
	7,		/* twtr_tck;*/
	7,
	7,		/* trtp_tck; */
	4,		/* tccd_tck; */
	47910,		/* trc_ps; */
	0,		/* tccd_s_tck; */
	0,		/* twtr_s_tck; */
	40000,		/* tfaw_ps; */
	5,		/* tcke_tck; */
};

struct dram_timing_info ddr3_timing_2133mhz = {
	110000,		/* trfc_ps;    */
	70312500,	/* trefi_ps;   */
	16,		/* wr_max_tck; */
	13900,		/* trcd_ps;    */
	13900,		/* trp_ps;     */
	33000,		/* tras_ps;    */
	6,		/* trrd_tck;   */
	15000,		/* twr_ps;     */
	8,		/* twtr_tck;   */
	7,
	8,		/* trtp_tck;   */
	4,		/* tccd_tck;   */
	46090,		/* trc_ps;     */
	0,		/* tccd_s_tck; */
	0,		/* twtr_s_tck; */
	35000,		/* tfaw_ps;    */
	6,		/* tcke_tck;   */
};

struct dram_timing_info ddr4_timing_160mhz = {
	208000,		/* trfc_ps;    */
	70312500,	/* trefi_ps;   */
	10,		/* wr_max_tck; */
	13750,		/* trcd_ps;    */
	13750,		/* trp_ps;     */
	35000,		/* tras_ps;    */
	5,		/* trrd_tck;   */
	15000,		/* twr_ps;     */
	6,		/* twtr_tck;   */
	7,
	6,		/* trtp_tck;   */
	5,		/* tccd_tck;   */
	48750,		/* trc_ps;     */
	4,		/* tccd_s_tck; */
	6,		/* twtr_s_tck; */
	35000,		/* tfaw_ps;    */
	4,		/* tcke_tck;   */
};

struct dram_timing_info ddr4_timing_1600mhz = {
	208000,		/* trfc_ps;     */
	70312500,	/* trefi_ps;    */
	12,		/* wr_max_tck;  */
	13750,		/* trcd_ps;     */
	13750,		/* trp_ps;      */
	35000,		/* tras_ps;     */
	5,		/* trrd_tck;    */
	15000,		/* twr_ps;      */
	6,		/* twtr_tck;    */
	7,
	6,		/* trtp_tck;    */
	5,		/* tccd_tck;    */
	48750,		/* trc_ps;      */
	4,		/* tccd_s_tck;  */
	6,		/* twtr_s_tck;  */
	35000,		/* tfaw_ps;     */
	4,		/* tcke_tck;    */
};

struct dram_timing_info ddr4_timing_1866mhz = {
	208000,		/* trfc_ps;    */
	70312500,	/* trefi_ps;   */
	14,		/* wr_max_tck; */
	15000,		/* trcd_ps;    */
	15000,		/* trp_ps;     */
	34000,		/* tras_ps;    */
	6,		/* trrd_tck;   */
	15000,		/* twr_ps;     */
	7,		/* twtr_tck;   */
	7,
	7,		/* trtp_tck;   */
	5,		/* tccd_tck;   */
	48750,		/* trc_ps;     */
	4,		/* tccd_s_tck; */
	7,		/* twtr_s_tck; */
	30000,		/* tfaw_ps;    */
	5,		/* tcke_tck;   */
};

struct dram_timing_info ddr4_timing_2133mhz = {
	250000,		/* trfc_ps;    */
	70312500,	/* trefi_ps;   */
	16,		/* wr_max_tck; */
	14060,		/* trcd_ps;    */
	14060,		/* trp_ps;     */
	33000,		/* tras_ps;    */
	6,		/* trrd_tck;   */
	15000,		/* twr_ps;     */
	8,		/* twtr_tck;   */
	7,
	8,		/* trtp_tck;   */
	6,		/* tccd_tck;   */
	46090,		/* trc_ps;     */
	4,		/* tccd_s_tck; */
	8,		/* twtr_s_tck; */
	30000,		/* tfaw_ps;    */
	6,		/* tcke_tck;   */
};

struct dram_timing_info ddr4_timing_2400mhz = {
	250000,     /* trfc_ps;    */
	70312500,   /* trefi_ps;   */
	20,         /* wr_max_tck; */
	12500,      /* trcd_ps;    */
	12500,      /* trp_ps;     */
	39000,      /* tras_ps;    */
	6,          /* trrd_tck;   */
	15000,      /* twr_ps;     */
	8,          /* twtr_tck;   */
	8,
	10,         /* trtp_tck;   */
	6,          /* tccd_tck;   */
	45320,      /* trc_ps;     */
	4,          /* tccd_s_tck; */
	9,          /* twtr_s_tck; */
	30000,      /* tfaw_ps;    */
	7,          /* tcke_tck;   */
};

/* DRAM DEVICEINFO */
struct dram_device_info dll_dram_info_DDR2_25MHZ = {
	&ddr2_dev,
	&ddr2_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	40000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR2_25MHZ_HALF_DQ = {
	&ddr2_dev_half_dq,
	&ddr2_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	40000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR2_66MHZ = {
	&ddr2_dev,
	&ddr2_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	15000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR2_66MHZ_HALF_DQ = {
	&ddr2_dev_half_dq,
	&ddr2_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	15000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_25MHZ = {
	&ddr3_dev,
	&ddr3_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	40000,				/* 25MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_25MHZ_HALF_DQ = {
	&ddr3_dev_half_dq,
	&ddr3_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	40000,				/* 25MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_66MHZ = {
	&ddr3_dev,
	&ddr3_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	15000,				/* 66MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_66MHZ_HALF_DQ = {
	&ddr3_dev_half_dq,
	&ddr3_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	15000,				/* 66MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_160MHZ = {
	&ddr3_dev,
	&ddr3_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	125000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_160MHZ_HALF_DQ = {
	&ddr3_dev_half_dq,
	&ddr3_mode_reg_160mhz,
	&ddr3_timing_160mhz,
	125000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_1600MHZ = {
	&ddr3_dev,
	&ddr3_mode_reg_1600mhz,
	&ddr3_timing_1600mhz,
	1250,				/* 800MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_1600MHZ_HALF_DQ = {
	&ddr3_dev_half_dq,
	&ddr3_mode_reg_1600mhz,
	&ddr3_timing_1600mhz,
	1250,				/* 800MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_1866MHZ = {
	&ddr3_dev,
	&ddr3_mode_reg_1866mhz,
	&ddr3_timing_1866mhz,
	1072,				/* 933MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_1866MHZ_HALF_DQ = {
	&ddr3_dev_half_dq,
	&ddr3_mode_reg_1866mhz,
	&ddr3_timing_1866mhz,
	1072,				/* 933MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_2133MHZ = {
	&ddr3_dev,
	&ddr3_mode_reg_2133mhz,
	&ddr3_timing_2133mhz,
	938,				/* 1066MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR3_2133MHZ_HALF_DQ = {
	&ddr3_dev_half_dq,
	&ddr3_mode_reg_2133mhz,
	&ddr3_timing_2133mhz,
	938,				/* 1066MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_25MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_160mhz,
	&ddr4_timing_160mhz,
	40000,				/* 25MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_25MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_160mhz,
	&ddr4_timing_160mhz,
	40000,				/* 25MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_66MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_160mhz,
	&ddr4_timing_160mhz,
	15000,				/* 66MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_66MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_160mhz,
	&ddr4_timing_160mhz,
	15000,				/* 66MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_160MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_160mhz,
	&ddr4_timing_160mhz,
	125000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_160MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_160mhz,
	&ddr4_timing_160mhz,
	125000,				/* ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_1600MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_1600mhz,
	&ddr4_timing_1600mhz,
	1250,				/* 800MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_1600MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_1600mhz,
	&ddr4_timing_1600mhz,
	1250,				/* 800MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_1866MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_1866mhz,
	&ddr4_timing_1866mhz,
	1072,				/* 933MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_1866MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_1866mhz,
	&ddr4_timing_1866mhz,
	1072,				/* 933MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_2133MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_2133mhz,
	&ddr4_timing_2133mhz,
	938,				/* 1066MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_2133MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_2133mhz,
	&ddr4_timing_2133mhz,
	938,				/* 1066MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_2400MHZ = {
	&ddr4_dev,
	&ddr4_mode_reg_2400mhz,
	&ddr4_timing_2400mhz,
	833,				/* 1200MHZ,ddr_period_ps */
	DFI_RATIO_2
};

struct dram_device_info dll_dram_info_DDR4_2400MHZ_HALF_DQ = {
	&ddr4_dev_half_dq,
	&ddr4_mode_reg_2400mhz,
	&ddr4_timing_2400mhz,
	833,				/* 1400MHZ,ddr_period_ps */
	DFI_RATIO_2
};
#endif
