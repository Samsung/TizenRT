/**
  ******************************************************************************
  * @file    ameba_ddrphy_scanpara.c
  * @author
  * @version V1.0.0
  * @date    2020-08-20
  * @brief   This file contains all the functions prototypes for the ddrphy firmware
  *             library, including the following functionalities of the DDR PHY:
  *           -Initialization
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"
#include "ameba_ddrphy_scanpara.h"

static const char *TAG = "DDRPHY";
#if 0
#define SCAN_PRINT DiagPrintf
#define BSTC_DEBUG_PRINT TRUE
#else
#define BSTC_DEBUG_PRINT FALSE
#define SCAN_PRINT(...)
#endif

#define TEQUALTOPI		32

//--------------------------------------//
// SCAN STEPS
//--------------------------------------//
#define SCAN_PROC_STEP	TX_MR_WRLVL
#define TX_MR_WRLVL		1
#define TX_CK_PI		2
#define TX_RW_WRLVL		3
#define TX_CS_PI		4
#define TX_DQS_PI		5
#define RX_SCAN			6
#define TX_SCAN_SETUP_MAP	1

//--------------------------------------//

//--------------------------------------//
// Function Task
//--------------------------------------//
#define BSTC_TEST      0
#define FRQC_EN        0
//--------------------------------------//

//bstc
//=================================================//
//=========== User Define Part Start ==============//
//=================================================//

// 1:BUS_128; 0:BUS_64
#define BUS_128         1
#define HWPAT_EN        0
#define TEST_MSK        0 //TEST_MSK =1 => BSTC_DIS_MASK_EN =0, 是指開啟wr_mask
#define CR_RELOAD       1

//=================================================//
//=========== BSTC Define Part Start ==============//
//=================================================//
// DDRC BCR Setting For BSTC Function
//#define BSTC_BST_SIZE            0x2 // It refers the bus data number(bus bstlen)

// BSTC status setting
#if HWPAT_EN
#define BSTC_HW_PAT				0x1
#else
#define BSTC_HW_PAT				0x0
#endif // CR_HW_PAT

// Clean CR_BER and CR_BST.{ERR_CNT, ERR_FST_TH} registers
#define BSTC_CRR				0x0

#if CR_RELOAD
#define BSTC_AT_STOP_EN			0x0
#define BSTC_RELOAD				0x7
#else //!CR_RELOAD
#define BSTC_AT_STOP_EN			0x0
#define BSTC_RELOAD				0x0
#endif //CR_RELOAD

#if TEST_MSK
#define BSTC_DIS_MASK_EN		0x0
#else
#define BSTC_DIS_MASK_EN		0x1
#endif // TEST_MSK

#define BSTC_CMP_EN				0x1 //Enable compare mode.
#define BSTC_STOP_EN			0x0 //Disable to run BSTC hardware function.
#define BSTC_LOOP_EN			0x1 //Enable loop mode.

// BSTC BCT for loop counter setting
#define LOOP_CNT                 0x0

// Disable refresh function
#define CR_DIS_REF               0x0

//=================================================//
//=========== User Define Part end  ===============//
//=================================================//
#if DDR_SCAN_PARA

uint32_t pattern[640] = {
	0x8a18207f, 0xc50c103f, 0xe286081f, 0xf143040f, //random
	0x78a18207, 0x3c50c103, 0x9e286081, 0x4f143040,
	0x278a1820, 0x13c50c10, 0x89e28608, 0x44f14304,
	0xa278a182, 0xd13c50c1, 0x689e2860, 0x344f1430,
	0x9a278a18, 0xcd13c50c, 0xe689e286, 0x7344f143,
	0xb9a278a1, 0x5cd13c50, 0xae689e28, 0x57344f14,
	0x2b9a278a, 0x95cd13c5, 0xcae689e2, 0xe57344f1,
	0xf2b9a278, 0xf95cd13c, 0x7cae689e, 0xbe57344f,
	0x5f2b9a27, 0x2f95cd13, 0x17cae689, 0x0be57344,
	0x85f2b9a2, 0xc2f95cd1, 0xe17cae68, 0x70be5734,
	0x385f2b9a, 0x1c2f95cd, 0x8e17cae6, 0x470be573,
	0x2385f2b9, 0x91c2f95c, 0x48e17cae, 0x2470be57,
	0x92385f2b, 0xc91c2f95, 0x648e17ca, 0xb2470be5,
	0xd92385f2, 0x6c91c2f9, 0xb648e17c, 0x5b2470be,
	0xad92385f, 0xd6c91c2f, 0x6b648e17, 0xb5b2470b,
	0xdad92385, 0xed6c91c2, 0xf6b648e1, 0x7b5b2470,
	0xbdad9238, 0xded6c91c, 0x6f6b648e, 0x37b5b247,
	0x1bdad923, 0x8ded6c91, 0xc6f6b648, 0x637b5b24,
	0xb1bdad92, 0x58ded6c9, 0x2c6f6b64, 0x9637b5b2,
	0x4b1bdad9, 0xa58ded6c, 0xd2c6f6b6, 0xe9637b5b,
	0x74b1bdad, 0xba58ded6, 0xdd2c6f6b, 0xee9637b5,
	0x774b1bda, 0x3ba58ded, 0x9dd2c6f6, 0xcee9637b,
	0x6774b1bd, 0x33ba58de, 0x99dd2c6f, 0x4cee9637,
	0xa6774b1b, 0x533ba58d, 0xa99dd2c6, 0x54cee963,
	0xaa6774b1, 0xd533ba58, 0xea99dd2c, 0xf54cee96,
	0xfaa6774b, 0xfd533ba5, 0xfea99dd2, 0xff54cee9,
	0x7faa6774, 0x3fd533ba, 0x1fea99dd, 0x0ff54cee,
	0x07faa677, 0x03fd533b, 0x81fea99d, 0x40ff54ce,
	0x207faa67, 0x103fd533, 0x081fea99, 0x040ff54c,
	0x8207faa6, 0xc103fd53, 0x6081fea9, 0x3040ff54,
	0x18207faa, 0x0c103fd5, 0x86081fea, 0x43040ff5,
	0xa18207fa, 0x50c103fd, 0x286081fe, 0x143040ff, //128

	0x8a18207f, 0xc50c103f, 0xe286081f, 0xf143040f, //random
	0x78a18207, 0x3c50c103, 0x9e286081, 0x4f143040,
	0x278a1820, 0x13c50c10, 0x89e28608, 0x44f14304,
	0xa278a182, 0xd13c50c1, 0x689e2860, 0x344f1430,
	0x9a278a18, 0xcd13c50c, 0xe689e286, 0x7344f143,
	0xb9a278a1, 0x5cd13c50, 0xae689e28, 0x57344f14,
	0x2b9a278a, 0x95cd13c5, 0xcae689e2, 0xe57344f1,
	0xf2b9a278, 0xf95cd13c, 0x7cae689e, 0xbe57344f,
	0x5f2b9a27, 0x2f95cd13, 0x17cae689, 0x0be57344,
	0x85f2b9a2, 0xc2f95cd1, 0xe17cae68, 0x70be5734,
	0x385f2b9a, 0x1c2f95cd, 0x8e17cae6, 0x470be573,
	0x2385f2b9, 0x91c2f95c, 0x48e17cae, 0x2470be57,
	0x92385f2b, 0xc91c2f95, 0x648e17ca, 0xb2470be5,
	0xd92385f2, 0x6c91c2f9, 0xb648e17c, 0x5b2470be,
	0xad92385f, 0xd6c91c2f, 0x6b648e17, 0xb5b2470b,
	0xdad92385, 0xed6c91c2, 0xf6b648e1, 0x7b5b2470,
	0xbdad9238, 0xded6c91c, 0x6f6b648e, 0x37b5b247,
	0x1bdad923, 0x8ded6c91, 0xc6f6b648, 0x637b5b24,
	0xb1bdad92, 0x58ded6c9, 0x2c6f6b64, 0x9637b5b2,
	0x4b1bdad9, 0xa58ded6c, 0xd2c6f6b6, 0xe9637b5b,
	0x74b1bdad, 0xba58ded6, 0xdd2c6f6b, 0xee9637b5,
	0x774b1bda, 0x3ba58ded, 0x9dd2c6f6, 0xcee9637b,
	0x6774b1bd, 0x33ba58de, 0x99dd2c6f, 0x4cee9637,
	0xa6774b1b, 0x533ba58d, 0xa99dd2c6, 0x54cee963,
	0xaa6774b1, 0xd533ba58, 0xea99dd2c, 0xf54cee96,
	0xfaa6774b, 0xfd533ba5, 0xfea99dd2, 0xff54cee9,
	0x7faa6774, 0x3fd533ba, 0x1fea99dd, 0x0ff54cee,
	0x07faa677, 0x03fd533b, 0x81fea99d, 0x40ff54ce,
	0x207faa67, 0x103fd533, 0x081fea99, 0x040ff54c,
	0x8207faa6, 0xc103fd53, 0x6081fea9, 0x3040ff54,
	0x18207faa, 0x0c103fd5, 0x86081fea, 0x43040ff5,
	0xa18207fa, 0x50c103fd, 0x286081fe, 0x143040ff, //128
};

void DDR_PHY_ZQ_SET_CAL(u32 set_num, u32 NOCD_ZPROG_val)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ddr_phy->DDRPHY_PAD_CTRL_PROG = (ddr_phy->DDRPHY_PAD_CTRL_PROG & (~DDRPHY_MASK_DZQ_UP_SEL)) | DDRPHY_DZQ_UP_SEL(set_num);
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_DZQ_AUTO_UP;
	ddr_phy->DDRPHY_PAD_CTRL_ZPROG_SETx[set_num] = NOCD_ZPROG_val;

	/* HW auto do 3 times ZQ K and calculate average value */
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_ZCTRL_FILTER_EN;

	/*enable ZQ Calibration*/
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_ZCTRL_START;

	//wait ZQ calibration done
	while (!(ddr_phy->DDRPHY_PAD_ZCTRL_STATUS & BIT31)) {}
	RTK_LOGI(TAG,  "!!!DDRPHY_PAD_ZCTRL_STATUS = 0x%08x\n", ddr_phy->DDRPHY_PAD_ZCTRL_STATUS);
	/*disable ZQ Calibration*/
	ddr_phy->DDRPHY_PAD_CTRL_PROG &= (~DDRPHY_BIT_ZCTRL_START);
}

/**
  * @brief  dump R480 & ZQ cal result.
  * @param  None
  * @retval None
  */
void DDR_PHY_ZQ_Dump(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	RTK_LOGI(TAG,  "DDRPHY_PAD_RZCTRL_STATUS = 0x%08x\n", ddr_phy->DDRPHY_PAD_RZCTRL_STATUS);
	RTK_LOGI(TAG,  "DDRPHY_PAD_ZCTRL_STATUS = 0x%08x\n", ddr_phy->DDRPHY_PAD_ZCTRL_STATUS);
	RTK_LOGI(TAG,  "DDRPHY_PAD_ZCTRL_RESULT = 0x%08x\n", ddr_phy->DDRPHY_PAD_ZCTRL_RESULT);

	assert_param(ddr_phy->DDRPHY_ODT_TTCP0_SET0 == ddr_phy->DDRPHY_ODT_TTCP0_SET1);
	assert_param(ddr_phy->DDRPHY_ODT_TTCP1_SET0 == ddr_phy->DDRPHY_ODT_TTCP1_SET1);
	assert_param(ddr_phy->DDRPHY_ODT_TTCN0_SET0 == ddr_phy->DDRPHY_ODT_TTCN0_SET1);
	assert_param(ddr_phy->DDRPHY_ODT_TTCN1_SET0 == ddr_phy->DDRPHY_ODT_TTCN1_SET1);
	assert_param(ddr_phy->DDRPHY_OCDP0_SET0 == ddr_phy->DDRPHY_OCDP0_SET1);
	assert_param(ddr_phy->DDRPHY_OCDP1_SET0 == ddr_phy->DDRPHY_OCDP1_SET1);
	assert_param(ddr_phy->DDRPHY_OCDN0_SET0 == ddr_phy->DDRPHY_OCDN0_SET1);
	assert_param(ddr_phy->DDRPHY_OCDN1_SET0 == ddr_phy->DDRPHY_OCDN1_SET1);

	/*ODT_SEL set3 is the inverse of set1, OCD set7 keep closed by set to 00*/
#define ODT_SEL_EXCHANGE(TTCP, TTCN)	(((TTCP) & 0x00FFFFFF) | (((TTCN) & 0x0000FF00) << 16))
	RTK_LOGI(TAG,  "DDRPHY_ODT_TTCP0_SET0 = 0x%08x\n", ODT_SEL_EXCHANGE(ddr_phy->DDRPHY_ODT_TTCP0_SET0, ddr_phy->DDRPHY_ODT_TTCN0_SET0));
	RTK_LOGI(TAG,  "DDRPHY_ODT_TTCP1_SET0 = 0x%08x\n", ddr_phy->DDRPHY_ODT_TTCP1_SET0);
	RTK_LOGI(TAG,  "DDRPHY_ODT_TTCN0_SET0 = 0x%08x\n", ODT_SEL_EXCHANGE(ddr_phy->DDRPHY_ODT_TTCN0_SET0, ddr_phy->DDRPHY_ODT_TTCP0_SET0));
	RTK_LOGI(TAG,  "DDRPHY_ODT_TTCN1_SET0 = 0x%08x\n", ddr_phy->DDRPHY_ODT_TTCN1_SET0);
	RTK_LOGI(TAG,  "DDRPHY_OCDP0_SET0 = 0x%08x\n", ddr_phy->DDRPHY_OCDP0_SET0);
	RTK_LOGI(TAG,  "DDRPHY_OCDP1_SET0 = 0x%08x\n", ddr_phy->DDRPHY_OCDP1_SET0 & 0x00FFFFFF);
	RTK_LOGI(TAG,  "DDRPHY_OCDN0_SET0 = 0x%08x\n", ddr_phy->DDRPHY_OCDN0_SET0);
	RTK_LOGI(TAG,  "DDRPHY_OCDN1_SET0 = 0x%08x\n", ddr_phy->DDRPHY_OCDN1_SET0 & 0x00FFFFFF);

	RTK_LOGI(TAG,  "Please replace ZQ K para...\n");
}

/**
  * @brief  DDRPHY ZQ calibration.
  * @param  None
  * @retval None
  */
void DDR_PHY_ZQ_CAL(VOID)
{

	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	u32 temp;
#if 0
	ddr_phy->DDRPHY_PAD_BUS_0 = 0x9e580000; //for DDR3
	ddr_phy->DDRPHY_PAD_CTRL_PROG = 0x0a000c99;
	ddr_phy->DDRPHY_DPI_CTRL_1 = 0x00000003;
#endif
	RTK_LOGI(TAG,  "DDR_PHY_ZQ_CAL\n");
	ddr_phy->DDRPHY_PAD_CTRL_PROG = ((ddr_phy->DDRPHY_PAD_CTRL_PROG & (~DDRPHY_MASK_ZCTRL_CLK_SEL)) | DDRPHY_ZCTRL_CLK_SEL(0x2));
	if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR3L) {
		ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_RZQ_EXT_R240; //external register
	} else if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR2) {
		ddr_phy->DDRPHY_PAD_CTRL_PROG &= (~DDRPHY_BIT_RZQ_EXT_R240); //internal register
	} else {
		RTK_LOGI(TAG,  "Confirm R240 location to RDC...\n");
	}
	ddr_phy->DDRPHY_PAD_BUS_1 &= (~DDRPHY_BIT_ZQ_POWER_DOWN);
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_ZQ_CAL_CMD;
	ddr_phy->DDRPHY_ZQ_NOCD2 &= (~DDRPHY_BIT_ZQ_ENA_NOCD2);

//	temp = ddr_phy->DDRPHY_PAD_BUS_0;
//	temp &= ~DDRPHY_BIT_EN_DMYI;
//	//temp |= DDRPHY_BIT_VREF_SEL; //No need in DDR2/DDR3L
//	ddr_phy->DDRPHY_PAD_BUS_0 = temp;

	temp = ddr_phy->DDRPHY_PAD_BUS_2;
	temp &= (~(DDRPHY_MASK_VREF_S_0 | DDRPHY_BIT_VREF_RANGE_0 | DDRPHY_BIT_VREF_PD_0));
	temp |= (DDRPHY_VREF_S_0(0x6) | DDRPHY_BIT_VREF_RANGE_0); //for DDR2/3L VREF_DQ %50
	ddr_phy->DDRPHY_PAD_BUS_2 = temp;

	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	ddr_phy->DDRPHY_DPI_CTRL_1 |= (DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0);
	DelayMs(0xC8); //wait 200ms to let setting valid

	DDR_PHY_ZQ_Dump();
	/*OCD = 34, ODT = 60
	search OCD 34 in (LP)DDR34-0p5vdd item of T22ULP_25UD18_DDR34SEMPH0-zprogtable.xls
	find in line 99, enter D99~I99 to PAD_CTRL_ZPROG[5:0], A99~C99 to PAD_CTRL_ZPROG[10:8]
	search ODT = 60，find in line 25, enter D25~I25 to PAD_CTRL_ZPROG[21:16]，A25~C25 to PAD_CTRL_ZPROG[26:24]*/

	RTK_LOGI(TAG,  "Set 0 cal\n");
	// zq cal. SET0 for CMD, OCD = 40, ODT = 60K
	DDR_PHY_ZQ_SET_CAL(0, 0x0014003C);
	DDR_PHY_ZQ_Dump();

	RTK_LOGI(TAG,  "Set 1 cal\n");
	// zq cal. SET1 for DQ, OCD = 34, ODT = 60K
	DDR_PHY_ZQ_SET_CAL(1, 0x0014011E);
	DDR_PHY_ZQ_Dump();

	RTK_LOGI(TAG,  "Set 2 cal\n");
	// zq cal. SET2 for xx, OCD = 60, ODT = 60K
	DDR_PHY_ZQ_SET_CAL(2, 0x00140028);
	DDR_PHY_ZQ_Dump();

	//Disable ZQ Pad for power save(a little) After ZQ calibration
	ddr_phy->DDRPHY_DPI_CTRL_0 &= (~DDRPHY_MASK_FW_SET_MODE);
	ddr_phy->DDRPHY_PAD_BUS_1 |= DDRPHY_BIT_ZQ_POWER_DOWN;
}

/**
  * @brief  DDRPHY R480 calibration.
  * @param  None
  * @retval None
  */
void DDR_PHY_R480_CAL(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	u32 temp;

	if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR3L) {
		return; //DDR3L use external 240ohm, no need to K R480
	}

	RTK_LOGI(TAG,  "DDR_PHY_R480_CAL\n");
	ddr_phy->DDRPHY_PAD_CTRL_PROG = ((ddr_phy->DDRPHY_PAD_CTRL_PROG & (~DDRPHY_MASK_ZCTRL_CLK_SEL)) | DDRPHY_ZCTRL_CLK_SEL(0x2));
	ddr_phy->DDRPHY_PAD_CTRL_PROG &= (~DDRPHY_BIT_RZQ_EXT_R240); //internal register
	ddr_phy->DDRPHY_PAD_BUS_1 &= (~DDRPHY_BIT_ZQ_POWER_DOWN);
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_ZQ_CAL_CMD;
	ddr_phy->DDRPHY_ZQ_NOCD2 &= (~DDRPHY_BIT_ZQ_ENA_NOCD2);

//	temp = ddr_phy->DDRPHY_PAD_BUS_0;
//	temp &= ~DDRPHY_BIT_EN_DMYI;//EN_DMYI default is 0
//	//temp |= DDRPHY_BIT_VREF_SEL; //No need in DDR2/DDR3L
//	ddr_phy->DDRPHY_PAD_BUS_0 = temp;

	temp = ddr_phy->DDRPHY_PAD_BUS_2;
	temp &= (~(DDRPHY_MASK_VREF_S_0 | DDRPHY_BIT_VREF_RANGE_0 | DDRPHY_BIT_VREF_PD_0));
	if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR2) {
		temp |= DDRPHY_VREF_S_0(0x4); //for DDR2 VREF_CA
	}
	ddr_phy->DDRPHY_PAD_BUS_2 = temp;

	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	ddr_phy->DDRPHY_DPI_CTRL_1 |= (DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0);
	RTK_LOGI(TAG,  "DDRPHY_PAD_RZCTRL_STATUS 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_PAD_RZCTRL_STATUS), ddr_phy->DDRPHY_PAD_RZCTRL_STATUS);
	RTK_LOGI(TAG,  "DDRPHY_PAD_CTRL_PROG 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_PAD_CTRL_PROG), ddr_phy->DDRPHY_PAD_CTRL_PROG);
	RTK_LOGI(TAG,  "DDRPHY_PAD_BUS_0 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_PAD_BUS_0), ddr_phy->DDRPHY_PAD_BUS_0);
	RTK_LOGI(TAG,  "DDRPHY_PAD_BUS_1 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_PAD_BUS_1), ddr_phy->DDRPHY_PAD_BUS_1);
	RTK_LOGI(TAG,  "DDRPHY_PAD_BUS_2 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_PAD_BUS_2), ddr_phy->DDRPHY_PAD_BUS_2);
	RTK_LOGI(TAG,  "DDRPHY_DPI_CTRL_0 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_DPI_CTRL_0), ddr_phy->DDRPHY_DPI_CTRL_0);
	RTK_LOGI(TAG,  "DDRPHY_DPI_CTRL_1 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_DPI_CTRL_1), ddr_phy->DDRPHY_DPI_CTRL_1);
	RTK_LOGI(TAG,  "DDRPHY_ZQ_NOCD2 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_ZQ_NOCD2), ddr_phy->DDRPHY_ZQ_NOCD2);
	DelayMs(0xC8); //wait 200ms to let setting valid

	/*enable R480 Calibration*/
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_RZQ_CAL_EN;

	//wait R480 calibration done
	while (!(ddr_phy->DDRPHY_PAD_RZCTRL_STATUS & DDRPHY_BIT_RZQ_CAL_DONE)) {}
	RTK_LOGI(TAG,  "DDRPHY_PAD_RZCTRL_STATUS 0x%08x = 0x%08x\n", &(ddr_phy->DDRPHY_PAD_RZCTRL_STATUS), ddr_phy->DDRPHY_PAD_RZCTRL_STATUS);
	RTK_LOGI(TAG,  "DDRPHY_GET_RZQ_480CODE: 0x%08x\n", DDRPHY_GET_RZQ_480CODE(ddr_phy->DDRPHY_PAD_RZCTRL_STATUS));

	/*disable R480 Calibration*/
	ddr_phy->DDRPHY_PAD_CTRL_PROG &= (~DDRPHY_BIT_RZQ_CAL_EN);

	//internal R240 Calibration First
	ddr_phy->DDRPHY_DPI_CTRL_0 &= (~DDRPHY_MASK_FW_SET_MODE);
	ddr_phy->DDRPHY_PAD_BUS_1 |= DDRPHY_BIT_ZQ_POWER_DOWN;
}

void bstc_cmd_wr(uint32_t addr, struct bstc_cmd_data_b37 *cmd)
{
	uint32_t bstc_cmd;

	// qfifo_cmd [31:0]
	bstc_cmd = (((cmd->CMD_PAGE << QFIFO_CMD_PAGE_BFO) | (cmd->CMD_COLU << QFIFO_CMD_COLU_BFO) | (cmd->BST_LEN << QFIFO_BST_LEN_BFO) | (cmd->WRRD)));
	SCAN_PRINT("BSTC_CMD[31:00] 0x%08x = 0x%08x\n", addr, bstc_cmd);
	HAL_WRITE32(addr, 0, bstc_cmd);

	// qfifo_cmd [63:32]
	bstc_cmd = ((cmd->CMD_LP4MWR << (QFIFO_CMD_LP4MWR_BFO - 32)) |	(cmd->CMD_BANK << (QFIFO_CMD_BANK_BFO - 32)));
	SCAN_PRINT("BSTC_CMD[63:32] 0x%08x = 0x%08x\n", (addr + 4), bstc_cmd);
	HAL_WRITE32(addr, 4, bstc_cmd);
}

void bstc_wr_128(uint32_t addr, struct bstc_data_b128 *data)
{
	// wfifo_data [127:0]
	HAL_WRITE32(addr, (0 * 4), data->data_0);
	HAL_WRITE32(addr, (1 * 4), data->data_1);
	HAL_WRITE32(addr, (2 * 4), data->data_2);
	HAL_WRITE32(addr, (3 * 4), data->data_3);
}

void bstc_rd_128(uint32_t addr, struct bstc_data_b128 *data)
{
	// wfifo_data [127:0]
	data->data_0 = HAL_READ32(addr, 0 * 4);
	data->data_1 = HAL_READ32(addr, 1 * 4);
	data->data_2 = HAL_READ32(addr, 2 * 4);
	data->data_3 = HAL_READ32(addr, 3 * 4);
}

/**
  * @brief  DDRPHY BSTC test.
  * @param  None
  * @retval None
  */
void bstc_cmd_sram_fill(void)
{
	u32 bstc_addr;
	u32 loop_cnt = 0;
	u32 cmd_buf[8];
	struct bstc_cmd_data_b37 *bstc_cmd = (struct bstc_cmd_data_b37 *)cmd_buf;

	bstc_addr = CMD_SRAM_BASE;
	for (loop_cnt = 0; loop_cnt < CMD_WR_NUM; loop_cnt++) {
		// cmd: wr
		bstc_cmd->CMD_LP4MWR = LP4_MWR_INFO;						//cmd_bank_num
		if (loop_cnt % 2 == 0) {
			bstc_cmd->CMD_BANK = 0x0;								//cmd_bank_num_r
		} else {
			bstc_cmd->CMD_BANK = 0x4;
		}
		bstc_cmd->CMD_PAGE = DRAM_PAGE_ADR; 						//cmd_page_num
		bstc_cmd->CMD_COLU = DRAM_COLU_ADR + (DRAM_BST * loop_cnt); //cmd_colu_num
		bstc_cmd->BST_LEN = CMD_STMA_LEN;							//cmd_stma
		bstc_cmd->WRRD = DRAM_WR_CMD;
		bstc_cmd_wr(bstc_addr, bstc_cmd);

		bstc_addr += 0x4 * BSTC_CMD_WIDTH_WORD;
	}

	for (loop_cnt = 0; loop_cnt < CMD_RD_NUM; loop_cnt++) {
		// cmd: rd
		bstc_cmd->CMD_LP4MWR = LP4_MWR_INFO;						//cmd_bank
		if (loop_cnt % 2 == 0) {
			bstc_cmd->CMD_BANK = 0x0;								//cmd_bank_num_r
		} else {
			bstc_cmd->CMD_BANK = 0x4;
		}
		bstc_cmd->CMD_PAGE = DRAM_PAGE_ADR; 						//cmd_page
		bstc_cmd->CMD_COLU = DRAM_COLU_ADR + (DRAM_BST * loop_cnt); //cmd_colu
		bstc_cmd->BST_LEN = CMD_STMA_LEN;							//cmd_stma
		bstc_cmd->WRRD = DRAM_RD_CMD;
		bstc_cmd_wr(bstc_addr, bstc_cmd);

		bstc_addr += 0x4 * BSTC_CMD_WIDTH_WORD;
	}
}

void bstc_other_sram_fill(u32 sram_base_addr)
{
	u32 bstc_addr = sram_base_addr;
	u32 loop_cnt = 0;
	u32 data_entry = (BSTC_WD_EX_LVL / 4);
	u32 data_buf[16];
	struct bstc_data_b128 *bstc_data = (struct bstc_data_b128 *)data_buf;

	for (loop_cnt = 0; loop_cnt < data_entry; loop_cnt++) {
		// cmd: wr, bst4
		bstc_data->data_3 = pattern[loop_cnt * 4 + 3];
		bstc_data->data_2 = pattern[loop_cnt * 4 + 2];
		bstc_data->data_1 = pattern[loop_cnt * 4 + 1];
		bstc_data->data_0 = pattern[loop_cnt * 4 + 0];
		SCAN_PRINT("> BSTC_FILL_ADDR = 0x%08x, BSTC_WDSRAM_DATA = 0x%x%x%x%x\n",
				   bstc_addr, bstc_data->data_3, bstc_data->data_2, bstc_data->data_1, bstc_data->data_0);
		bstc_wr_128(bstc_addr, bstc_data);
		bstc_addr += 0x10; // push 128-bit
	}
}

void DDR_PHY_BSTC_STARK(void)
{
	u32 bstc_addr;
	u32 data_entry;
	u32 loop_cnt = 0;
	u32 bsram0_cmd_lvl, bsram0_wd_lvl, bsram1_rg_lvl, bsram1_rd_lvl;
	//int bstc_done;
	int err_cnt;
	DDRC_TypeDef *ddrc = DDRC_DEV;

	u32 data_buf[16];
	struct bstc_data_b128 *bstc_data = (struct bstc_data_b128 *)data_buf;

	RTK_LOGI(TAG,  "rxi316_bstc start.\n");

	//********************************************************
	//*******	   initialize bstc related ctrl 	 *********
	//********************************************************
	// Set Bstc Mode
	ddrc->DDRC_CSR = (DDRC_BIT_MEM_IDLE | DDRC_BIT_TM_IDLE | DDRC_BSTC_IDLE(0));
	RTK_LOGI(TAG,  "CR_CSR 0x%08x = 0x%08x\n", &(ddrc->DDRC_CSR), ddrc->DDRC_CSR);

	// Flush cmd/wd/rg/rd data sram
	ddrc->DDRC_BCR = (DDRC_BIT_FUS_CMD | DDRC_BIT_FUS_WD | DDRC_BIT_FUS_RG | DDRC_BIT_FUS_RD);
	RTK_LOGI(TAG,  "CR_BCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_BCR), ddrc->DDRC_BCR);

	// Set BCR control mode
	ddrc->DDRC_BCR = (DDRC_LOOP_CNT(LOOP_CNT) | DDRC_HW_PAT(BSTC_HW_PAT) | DDRC_CRR(BSTC_CRR) |	DDRC_AT_ERR_STOP(BSTC_AT_STOP_EN) | \
					  DDRC_DIS_MSK(BSTC_DIS_MASK_EN) | DDRC_LOOP(BSTC_LOOP_EN) | DDRC_CMP(BSTC_CMP_EN) | DDRC_STOP(BSTC_STOP_EN));
	RTK_LOGI(TAG,  "CR_BCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_BCR), ddrc->DDRC_BCR);

	// Set BEXPC when single mode or reload mode
	// (CR_LOOP=0) or (CR_LOOP=1 & CR_RELOAD=1)
	ddrc->DDRC_BEXPC = (DDRC_RD_EXPC_CNT(BSTC_RD_EX_LVL) |	DDRC_WD_EXPC_CNT(BSTC_WD_EX_LVL) | DDRC_CMD_EXPC_CNT(BSTC_CMD_EX_LVL));
	RTK_LOGI(TAG,  "CR_BEXPC 0x%08x = 0x%08x\n", &(ddrc->DDRC_BEXPC), ddrc->DDRC_BEXPC);

	//********************************************************
	//*******	   initialize bstc cmd				 *********
	//********************************************************
	data_entry = (BSTC_WD_EX_LVL / 4);
	RTK_LOGI(TAG,  "===== Write BSTC CMD_SRAM =====\n");
	bstc_cmd_sram_fill();

#if BSTC_DEBUG_PRINT
	//Check CMD SRAM
	RTK_LOGI(TAG,  "===== Read BSTC CMD_SRAM =====\n");
	bstc_addr = CMD_SRAM_BASE;
	for (loop_cnt = 0; loop_cnt < BSTC_CMD_NUM; loop_cnt++) {
		DiagPrintf("Read CMD 0x%08x = 0x%08x\n", bstc_addr, HAL_READ32(bstc_addr, 0));
		if (BSTC_CMD_WIDTH_WORD > 1) {
			DiagPrintf("Read CMD 0x%08x = 0x%08x\n", (bstc_addr + 4), HAL_READ32((bstc_addr + 4), 0));
		}
		bstc_addr += 0x4 * BSTC_CMD_WIDTH_WORD;
	}
#endif
	//========================================================
	//======   iniitialize bstc mask/wdata/rd_golden   =======
	//========================================================

	// Write wdata sram
	RTK_LOGI(TAG,  "===== Write BSTC WD_SRAM =====\n");
	bstc_other_sram_fill(WD_SRAM_BASE);

#if BSTC_DEBUG_PRINT
	// Read wdata sram
	RTK_LOGI(TAG,  "===== Read BSTC WD_SRAM =====\n");
	bstc_addr = WD_SRAM_BASE;
	for (loop_cnt = 0; loop_cnt < data_entry; loop_cnt++) {
		bstc_rd_128(bstc_addr, bstc_data);
		DiagPrintf("> BSTC_WDSRAM_ADDR = 0x%08x, BSTC_WDSRAM_DATA = 0x%x%x%x%x\n",
				   bstc_addr, bstc_data->data_3, bstc_data->data_2, bstc_data->data_1, bstc_data->data_0);
		bstc_addr += 0x10; // push 128-bit
	}
#endif

#if BSTC_DIS_MASK_EN
#else
	RTK_LOGI(TAG,  "===== Write BSTC MSK_SRAM =====\n");
	bstc_addr = MSK_SRAM_BASE;
	// not mask
	for (loop_cnt = 0; loop_cnt < data_entry; loop_cnt = (loop_cnt + 1)) {
		bstc_data->data_3 = 0x0000;
		bstc_data->data_2 = 0x0000;
		bstc_data->data_1 = 0x0000;
		bstc_data->data_0 = 0x0000;
		bstc_wr_128(bstc_addr, bstc_data);
		bstc_addr += 0x10; // push 128-bit
	}

	// Read wmsk sram
	RTK_LOGI(TAG,  "===== Read BSTC MSK_SRAM =====\n");
	bstc_addr = MSK_SRAM_BASE;
	for (loop_cnt = 0; loop_cnt < data_entry; loop_cnt = loop_cnt + 1) {

		bstc_rd_128(bstc_addr, bstc_data);
		DiagPrintf("> BSTC_WMSKSRAM_ADDR = 0x%08x, BSTC_WMSKSRAM_DATA = 0x%x%x%x%x\n",
				   bstc_addr, bstc_data->data_3, bstc_data->data_2, bstc_data->data_1, bstc_data->data_0);
		bstc_addr += 0x10; // push 128-bit
	}
#endif

	RTK_LOGI(TAG,  "===== Write BSTC RG_SRAM =====\n");
	bstc_other_sram_fill(RG_SRAM_BASE);
#if BSTC_DEBUG_PRINT
	// Read rg sram
	RTK_LOGI(TAG,  "===== Read BSTC RG_SRAM =====\n");
	bstc_addr = RG_SRAM_BASE;
	for (loop_cnt = 0; loop_cnt < data_entry; loop_cnt++) {
		bstc_rd_128(bstc_addr, bstc_data);

		DiagPrintf("> BSTC_RGSRAM_ADDR = 0x%08x, BSTC_RGSRAM_DATA = 0x%x%x%x%x\n",
				   bstc_addr, bstc_data->data_3, bstc_data->data_2, bstc_data->data_1, bstc_data->data_0);
		bstc_addr += 0x10; // push 128-bit
	}
#endif

	//********************************************************
	//*******	 check wdata_level/rgdata_level 	 *********
	//********************************************************
	bsram0_cmd_lvl = DDRC_GET_CMD_CNT(ddrc->DDRC_BSRAM0);
	bsram0_wd_lvl = DDRC_GET_WD_CNT(ddrc->DDRC_BSRAM0);
	bsram1_rg_lvl = DDRC_GET_RG_CNT(ddrc->DDRC_BSRAM1);
	bsram1_rd_lvl = DDRC_GET_RD_CNT(ddrc->DDRC_BSRAM1);

	if (bsram0_cmd_lvl != BSTC_CMD_EX_LVL) {
		RTK_LOGE(TAG, "[ERR] BSTC CMD sram filled error! Expect cmd lvl = 0x%08x, Actual cmd lvl= 0x%08x\n", BSTC_CMD_EX_LVL, bsram0_cmd_lvl);
	} else {
		RTK_LOGI(TAG,  "[INFO] BSTC CMD sram valid data filled correct! cmd lvl = 0x%08x\n", bsram0_cmd_lvl);
	}

	if (bsram0_wd_lvl != BSTC_WD_EX_LVL) {
		RTK_LOGE(TAG, "[ERR] BSTC WD sram filled error! Expect wd lvl = 0x%08x, Actual wd lvl= 0x%08x\n", BSTC_WD_EX_LVL, bsram0_wd_lvl);
	} else {
		RTK_LOGI(TAG,  "[INFO] BSTC WR sram valid data filled correct! WD lvl = 0x%08x\n", bsram0_wd_lvl);
	}

	if (bsram1_rg_lvl != BSTC_WD_EX_LVL) {
		RTK_LOGE(TAG, "[ERR] BSTC RG sram filled error! Expect rg lvl= 0x%08x, Actual rg lvl= 0x%08x\n", BSTC_WD_EX_LVL, bsram1_rg_lvl);
	} else {
		RTK_LOGI(TAG,  "[INFO] BSTC RG sram valid data filled correct! RG lvl = 0x%08x\n", bsram1_rg_lvl);
	}

	if (bsram1_rd_lvl != 0) {
		RTK_LOGE(TAG, "[ERR] BSTC RD sram has error!\n");
	} else {
		RTK_LOGI(TAG,  "[INFO] BSTC RD sram correct!\n");
	}

	//********************************************************
	//*******		   BSTC_MODE start				 *********
	//********************************************************
	RTK_LOGI(TAG,  "//===== BSTC Start =====\n");

	ddrc->DDRC_CCR = DDRC_BIT_BSTC;
	RTK_LOGI(TAG,  "CR_CCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_CCR), ddrc->DDRC_CCR);

	//********************************************************
	//*******		 Issue BSTC_MODE done			 *********
	//********************************************************
	// Wait BSTC_DONE
	while ((ddrc->DDRC_CCR & DDRC_BIT_BSTC) >> 2 == 0) {
		DelayMs(1000 * 5);
		//RTK_LOGI(TAG,  "//===== Force BSTC OFF =====\n");
		ddrc->DDRC_BCR |= DDRC_BIT_STOP;
		RTK_LOGI(TAG,  "CR_BCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_BCR), ddrc->DDRC_BCR);
	};	// wait BISC done

	//********************************************************
	//******		  iniitialize bstc rd			   *******
	//********************************************************
	// Read rdata sram
#if BSTC_DEBUG_PRINT
	RTK_LOGI(TAG,  "===== Read BSTC RD_SRAM =====\n");
	bstc_addr = RD_SRAM_BASE;
	for (loop_cnt = 0; loop_cnt < data_entry; loop_cnt++) {
		bstc_rd_128(bstc_addr, bstc_data);
		RTK_LOGI(TAG,  "> BSTC_RDSRAM_ADDR = 0x%08x, BSTC_RDSRAM_DATA = 0x%08x%x%x%x\n", bstc_addr, bstc_data->data_3, bstc_data->data_2,
				 bstc_data->data_1, bstc_data->data_0);
		bstc_addr += 0x10; // push 128-bit
	}
	RTK_LOGI(TAG,  "===== End of Read BSTC RD_SRAM =====\n");
#endif

	////********************************************************
	////*******    Check BSTC ERR Cnt and Read data    *********
	////********************************************************
	err_cnt = DDRC_GET_ERR_CNT(ddrc->DDRC_BST);

	if (err_cnt != 0) {
		RTK_LOGE(TAG, "[ERR] BSTC error, err_cnt = %d\n", err_cnt);
	} else {
		RTK_LOGI(TAG,  "[BSTC_OK] BSTC error cnt is 0!\n");
	}
	RTK_LOGI(TAG,  "BSTC ERROR BIT: 0x%08x\n", DDRC_GET_ERR_BIT(ddrc->DDRC_BER));

	RTK_LOGI(TAG,  "rxi316_bstc done\n");
	RTK_LOGI(TAG,  "\n");
}

void sram_sg(void)
{
	u32 bsram0_wd_lvl, bsram1_rd_lvl;
	//u32 bsram0_cmd_lvl, bsram1_rg_lvl;
	u32 err_cnt;
	DDRC_TypeDef *ddrc = DDRC_DEV;

	SCAN_PRINT(" //========== sram_sg ==========// \n");
	// Set Bstc Mode
	ddrc->DDRC_CSR = (DDRC_BIT_MEM_IDLE | DDRC_BIT_TM_IDLE | DDRC_BSTC_IDLE(0));
	SCAN_PRINT("CR_CSR 0x%08x = 0x%08x\n", &(ddrc->DDRC_CSR), ddrc->DDRC_CSR);

	//********************************************************
	//*******		   BSTC_MODE Reload 			 *********
	//********************************************************
	// Flush DRAMC all FIFO
	ddrc->DDRC_CCR = DDRC_BIT_FLUSH_FIFO;
	SCAN_PRINT("CR_CCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_CCR), ddrc->DDRC_CCR);

	// Clean err_bit
	ddrc->DDRC_BCR = DDRC_BIT_CRR;
	SCAN_PRINT("CR_BCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_BCR), ddrc->DDRC_BCR);

	// Set BEXPC when single mode or reload mode
	// (CR_LOOP=0) or (CR_LOOP=1 & CR_RELOAD=1)
	ddrc->DDRC_BEXPC = (DDRC_RD_EXPC_CNT(BSTC_RD_EX_LVL) |	DDRC_WD_EXPC_CNT(BSTC_WD_EX_LVL) | DDRC_CMD_EXPC_CNT(BSTC_CMD_EX_LVL));
	SCAN_PRINT("CR_BRXPC 0x%08x = 0x%08x\n", &(ddrc->DDRC_BEXPC), ddrc->DDRC_BEXPC);

	// Reload BSTC setting
	ddrc->DDRC_BCR = (DDRC_LOOP_CNT(LOOP_CNT) | DDRC_RELOAD_SRAM(BSTC_RELOAD) | DDRC_HW_PAT(BSTC_HW_PAT) | DDRC_CRR(BSTC_CRR) |	DDRC_AT_ERR_STOP(
						  BSTC_AT_STOP_EN) | DDRC_DIS_MSK(BSTC_DIS_MASK_EN) | DDRC_LOOP(BSTC_LOOP_EN) | DDRC_CMP(BSTC_CMP_EN) | DDRC_STOP(BSTC_STOP_EN));
	SCAN_PRINT("CR_BCR 0x0x%08x = 0x%08x\n", &(ddrc->DDRC_BCR), ddrc->DDRC_BCR);

	// Reload BSTC setting
	ddrc->DDRC_BCR = (DDRC_LOOP_CNT(LOOP_CNT) | DDRC_RELOAD_SRAM(0) | DDRC_HW_PAT(BSTC_HW_PAT) | DDRC_CRR(BSTC_CRR) | DDRC_AT_ERR_STOP(BSTC_AT_STOP_EN) | \
					  DDRC_DIS_MSK(BSTC_DIS_MASK_EN) | DDRC_LOOP(BSTC_LOOP_EN) | DDRC_CMP(BSTC_CMP_EN) | DDRC_STOP(BSTC_STOP_EN));
	SCAN_PRINT("CR_BCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_BCR), ddrc->DDRC_BCR);

	//********************************************************
	//*******	 check wdata_level/rgdata_level 	 *********
	//********************************************************

	//refill pattern to golden sram, 因為不知道為甚麼reload後 RG level會跑掉
	//bsram0_cmd_lvl = DDRC_GET_CMD_CNT(ddrc->DDRC_BSRAM0);
	bsram0_wd_lvl = DDRC_GET_WD_CNT(ddrc->DDRC_BSRAM0);
	//bsram1_rg_lvl = DDRC_GET_RG_CNT(ddrc->DDRC_BSRAM1);
	bsram1_rd_lvl = DDRC_GET_RD_CNT(ddrc->DDRC_BSRAM1);

	if (1/*bsram0_cmd_lvl != BSTC_CMD_EX_LVL*/) { //BSTC BUG Workaround
		ddrc->DDRC_BCR |= DDRC_BIT_FUS_CMD;
		ddrc->DDRC_BCR &= ~DDRC_BIT_FUS_CMD;
		bstc_cmd_sram_fill();
	} else {
		//RTK_LOGI(TAG,  "[INFO] BSTC CMD sram valid data filled correct!");
	}

	if (bsram0_wd_lvl != BSTC_WD_EX_LVL) {
		RTK_LOGE(TAG, "[ERR] BSTC WR sram filled error!\n");
	} else {
		//RTK_LOGI(TAG,  "[INFO] BSTC WR sram valid data filled correct!");
	}

	if (1/*bsram1_rg_lvl != BSTC_WD_EX_LVL*/) {//BSTC BUG Workaround
		//RTK_LOGI(TAG,  "=================== flush rg sram ====================");
		ddrc->DDRC_BCR |= DDRC_BIT_FUS_RG;
		ddrc->DDRC_BCR &= ~DDRC_BIT_FUS_RG;
		//RTK_LOGI(TAG,  "=================== flush rd sram ====================");
		ddrc->DDRC_BCR |= DDRC_BIT_FUS_RD;
		ddrc->DDRC_BCR &= ~DDRC_BIT_FUS_RD;
		//RTK_LOGI(TAG,  "==== ReWrite BSTC RG_SRAM ====");
		bstc_other_sram_fill(RG_SRAM_BASE);
	} else {
		//RTK_LOGI(TAG,  "[INFO] BSTC RG sram valid data filled correct!");
	}

	if (bsram1_rd_lvl != 0) {
		//RTK_LOGE(TAG, "[ERR] BSTC RD sram has error!\n");
	} else {
		//RTK_LOGI(TAG,  "[INFO] BSTC RD sram correct!\n");
	}

	//********************************************************
	//*******          BSTC_MODE start               *********
	//********************************************************
#if CR_DIS_REF
	ddrc->DDRC_DRR = (ddrc->DDRC_DRR & ~(DDRC_MASK_ZQCL_INV | DDRC_BIT_REF_DIS)) | DDRC_BIT_REF_DIS;
#endif

	//RTK_LOGI(TAG,  "//================ RXI316_BSTC Reload Start ================");
	//BSTC Start
	ddrc->DDRC_CCR = DDRC_BIT_BSTC;
	SCAN_PRINT("CR_CCR 0x%08x = 0x%08x\n", &(ddrc->DDRC_CCR), ddrc->DDRC_CCR);

	//********************************************************
	//*******        Issue BSTC_MODE done            *********
	//********************************************************
	// Wait BSTC_DONE
	while ((ddrc->DDRC_CCR & DDRC_BIT_BSTC) >> 2 == 0) {
		DelayMs(1000 * 2);//rw 秒數 ( sram_sg )
		//RTK_LOGI(TAG,  "//===== Force BSTC OFF =====\n");
		ddrc->DDRC_BCR |= DDRC_BIT_STOP;
	};	// wait BISC done

	//********************************************************
	//*******    Check BSTC ERR Cnt and Read data    *********
	//********************************************************
	err_cnt = DDRC_GET_ERR_CNT(ddrc->DDRC_BST);
	if (err_cnt != 0 || DDRC_GET_ERR_BIT(ddrc->DDRC_BER)) {
		RTK_LOGE(TAG, "[ERR] BSTC error, err_cnt = %d\n", err_cnt);
		RTK_LOGE(TAG, "BSTC ERROR BIT: 0x%08x\n", DDRC_GET_ERR_BIT(ddrc->DDRC_BER));
	} else {
		SCAN_PRINT("[BSTC_OK] BSTC error cnt is 0!\n");
	}

	SCAN_PRINT("//========== sram_sg end ==========// \n");
	SCAN_PRINT("\n");
}

u32 sram_sg_chk(void)
{
	DDRC_TypeDef *ddrc = DDRC_DEV;
	return ddrc->DDRC_BER;//r/w result
}

void DDR_PHY_SRAM_SG(void)
{
	sram_sg();
}

static u8 DDR_PHY_Scan_param_setup_map(void)
{
	u8 scan_stage;

	RTK_LOGI(TAG,  "\nPlease select TX_SCAN_MAP: 0 - hold, 1 - setup\n");

	LOGUART_ClearRxFifo(LOGUART_DEV);
	LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, DISABLE);

	while (!(LOGUART_Readable()));
	scan_stage = LOGUART_GetChar(_FALSE) - '0';

	LOGUART_ClearRxFifo(LOGUART_DEV);
	LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, ENABLE);
	RTK_LOGI(TAG,  "Input Num is %d\n", scan_stage);

	return scan_stage;
}

void tx_MR_wrlvl(void)
{
	DDRC_TypeDef *ddrc = DDRC_DEV;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	int MR1_addr, dqs_pi;
	RTK_LOGI(TAG,  "======== MR write leveling ========\n");
	RTK_LOGI(TAG,  "Current PI\n");
	RTK_LOGI(TAG,  "CK0 PI = %d\n", DDRPHY_GET_DPI_POST_PI_SEL0(ddr_phy->DDRPHY_PLL_PI0));
	RTK_LOGI(TAG,  "DQS0 PI = %d\n", DDRPHY_GET_DPI_POST_PI_SEL2(ddr_phy->DDRPHY_PLL_PI0));
	RTK_LOGI(TAG,  "DQS1 PI = %d\n", DDRPHY_GET_DPI_POST_PI_SEL3(ddr_phy->DDRPHY_PLL_PI0));
	RTK_LOGI(TAG,  "DQ0 PI = %d\n", DDRPHY_GET_DPI_POST_PI_SEL6(ddr_phy->DDRPHY_PLL_PI1));
	RTK_LOGI(TAG,  "DQ1 PI = %d\n", DDRPHY_GET_DPI_POST_PI_SEL7(ddr_phy->DDRPHY_PLL_PI2));

	// fw_set_protocal_index[1:0] = 0x2, immediately
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	ddr_phy->DDRPHY_DPI_CTRL_0 &= ~DDRPHY_BIT_2T_WR_PRE; // close 2T write preamble

	MR1_addr = DDRC_GET_MR1(ddrc->DDRC_MR1);

	/*disable refresh function*/
	ddrc->DDRC_CSR = (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE | DDRC_BIT_MEM_IDLE);//Set RXI316 all idle mode
	ddrc->DDRC_DRR |= DDRC_BIT_REF_DIS; //Disable auto refresh
	DelayMs(10);
	ddrc->DDRC_TIE_DPIN |= DDRC_BIT_CKE; //TIE CKE=1
	ddrc->DDRC_TIE_DPIN |= DDRC_BIT_CKE_1;
	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	DelayMs(10); //delay for tMOD

	/*precharge all*/
	ddrc->DDRC_CMD_DPIN_NDGE = DDRC_BIT_ACT_N_NDGE | DDRC_BIT_CAS_N_NDGE | DDRC_ADDR_NDGE(BIT10); //A10/CAS_N/ACT_N to High
	ddrc->DDRC_CMD_DPIN = DDRC_BIT_ACT_N_NDGE | DDRC_BIT_CAS_N_NDGE | DDRC_ADDR_NDGE(BIT10);
	ddrc->DDRC_CSR = (ddrc->DDRC_CSR & ~DDRC_MASK_DPIN_CMD_INFO) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_PRECHARGE_ALL);
	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	DelayMs(10);

	/*Write leveling enable*/
	ddrc->DDRC_CMD_DPIN_NDGE = DDRC_BIT_ACT_N_NDGE | DDRC_BA_NDGE(0x1) | DDRC_GET_ADDR_NDGE(MR1_addr) | BIT7;//Set bit7 of MR1
	ddrc->DDRC_CMD_DPIN = DDRC_BIT_ACT_N_NDGE | DDRC_BA_NDGE(0x1) | DDRC_GET_ADDR_NDGE(MR1_addr) | BIT7;
	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	DelayMs(10);
	ddrc->DDRC_TIE_DPIN &= ~DDRC_BIT_CKE; //TIE CKE=0
	ddrc->DDRC_TIE_DPIN &= ~DDRC_BIT_CKE_1;
	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	DelayMs(10);

	ddrc->DDRC_DRR &= ~DDRC_BIT_REF_DIS;//Enable auto refresh
	ddrc->DDRC_CSR = (DDRC_BIT_TM_IDLE | DDRC_BIT_BSTC_IDLE);
	//Enable DQ input
	ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[0] = (ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[0] & ~(DDRPHY_MASK_DQ_IE_FRC_EN | DDRPHY_MASK_DQ_IE_SIG)) |
										  DDRPHY_DQ_IE_FRC_EN(0xFF) | DDRPHY_DQ_IE_SIG(0xFF);
	ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[1] = (ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[1] & ~(DDRPHY_MASK_DQ_IE_FRC_EN | DDRPHY_MASK_DQ_IE_SIG)) |
										  DDRPHY_DQ_IE_FRC_EN(0xFF) | DDRPHY_DQ_IE_SIG(0xFF);

	ddr_phy->DDRPHY_DQ_DQS_CTRL |= DDRPHY_BIT_DQS_OE_FRC_EN; //Enable DQS output depend on the vaule of dqs_oe_sig
	ddr_phy->DDRPHY_DQ_DQS_CTRL |= DDRPHY_BIT_DQ_LATCH_EN; //Capture DQ bus enable

	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0; // fw_set_wr_dly[1:0] = 3

	for (dqs_pi = 0; dqs_pi < 32; dqs_pi++) {
		// DQS PI  32bit 1 PI
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL2)) | DDRPHY_DPI_POST_PI_SEL2(dqs_pi);
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL3)) | DDRPHY_DPI_POST_PI_SEL3(dqs_pi);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL4)) | DDRPHY_DPI_POST_PI_SEL4(dqs_pi);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL5)) | DDRPHY_DPI_POST_PI_SEL5(dqs_pi);

		// set DQS DLY
		/*ddr_phy->DDRPHY_DQ_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQ_DLY_1_SETx[0] & ~DDRPHY_MASK_DQS_DLY_SEL) | DDRPHY_DQS_DLY_SEL(dly);
		ddr_phy->DDRPHY_DQ_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQ_DLY_1_SETx[1] & ~DDRPHY_MASK_DQS_DLY_SEL) | DDRPHY_DQS_DLY_SEL(dly);*/
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0; // fw_set_wr_dly[1:0] = 3
		ddr_phy->DDRPHY_DQ_DQS_CTRL |= DDRPHY_BIT_TRIG_DQS; // trig_dqs
		DelayMs(500);

		//RTK_LOGI(TAG,  "CK0 PI = 0x%08x\n", DDRPHY_GET_DPI_POST_PI_SEL0(ddr_phy->DDRPHY_PLL_PI0));
		//RTK_LOGI(TAG,  "DQS0 PI = 0x%08x\n", DDRPHY_GET_DPI_POST_PI_SEL2(ddr_phy->DDRPHY_PLL_PI0));
		//RTK_LOGI(TAG,  "DQS1 PI = 0x%08x\n", DDRPHY_GET_DPI_POST_PI_SEL3(ddr_phy->DDRPHY_PLL_PI0));
		//RTK_LOGI(TAG,  "DQS2 PI = 0x%08x\n", DDRPHY_GET_DPI_POST_PI_SEL4(ddr_phy->DDRPHY_PLL_PI1));
		//RTK_LOGI(TAG,  "DQS3 PI = 0x%08x\n", DDRPHY_GET_DPI_POST_PI_SEL5(ddr_phy->DDRPHY_PLL_PI1));
		RTK_LOGI(TAG,  "DQSPI=%02d, DQ0~31 = 0x%08x\n", dqs_pi, ddr_phy->DDRPHY_DQ_PAT_IN0); // dq_pat_in

		//DelayMs(1000*1);
	}
}

void tx_rw_wrlvl(void)
{
	//CK-DQS
	int ddr_error_bit = 0, hold_cnt = 0, setup_cnt = 0;
	int pi_tmp, i;
	int dqspi_setup[4], dqspi_hold[4];//儲存檔位
	int dqspi_setup_map[4], dqspi_hold_map[4];//儲存map大小
	int dqspi_init[4], dqpi_init[4];
	int d_dqs0_dqs1, d_dqs0_dqs2, d_dqs0_dqs3, d_dqs0_dq0, d_dqs0_dq1, d_dqs0_dq2, d_dqs0_dq3;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	//fw_set_mode: during refresh
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	dqspi_init[0] = DDRPHY_GET_DPI_POST_PI_SEL2(ddr_phy->DDRPHY_PLL_PI0);
	dqspi_init[1] = DDRPHY_GET_DPI_POST_PI_SEL3(ddr_phy->DDRPHY_PLL_PI0);
	dqspi_init[2] = DDRPHY_GET_DPI_POST_PI_SEL4(ddr_phy->DDRPHY_PLL_PI1);
	dqspi_init[3] = DDRPHY_GET_DPI_POST_PI_SEL5(ddr_phy->DDRPHY_PLL_PI1);
	dqpi_init[0] = DDRPHY_GET_DPI_POST_PI_SEL6(ddr_phy->DDRPHY_PLL_PI1);
	dqpi_init[1] = DDRPHY_GET_DPI_POST_PI_SEL7(ddr_phy->DDRPHY_PLL_PI2);
	dqpi_init[2] = DDRPHY_GET_DPI_POST_PI_SEL8(ddr_phy->DDRPHY_PLL_PI2);
	dqpi_init[3] = DDRPHY_GET_DPI_POST_PI_SEL9(ddr_phy->DDRPHY_PLL_PI2);

	d_dqs0_dqs1 = dqspi_init[0] - dqspi_init[1];
	d_dqs0_dqs2 = dqspi_init[0] - dqspi_init[2];
	d_dqs0_dqs3 = dqspi_init[0] - dqspi_init[3];
	d_dqs0_dq0 = dqspi_init[0] - dqpi_init[0];
	d_dqs0_dq1 = dqspi_init[0] - dqpi_init[1];
	d_dqs0_dq2 = dqspi_init[0] - dqpi_init[2];
	d_dqs0_dq3 = dqspi_init[0] - dqpi_init[3];

	for (i = 0; i < DQ_slice; i++) { //initialize result array
		dqspi_setup[i] = 0xffff;
		dqspi_hold[i] = 0xffff;
		dqspi_setup_map[i] = -1;
		dqspi_hold_map[i] = -1;
	}

	//hold time
	for (pi_tmp = dqspi_init[0]; pi_tmp < dqspi_init[0] + 48; pi_tmp++) {
		RTK_LOGI(TAG,  " dqspi0(hold) = %d\n", pi_tmp % 32);
		RTK_LOGI(TAG,  " dqspi1(hold) = %d\n", (pi_tmp - d_dqs0_dqs1) % 32);
		RTK_LOGI(TAG,  " dqspi2(hold) = %d\n", (pi_tmp - d_dqs0_dqs2) % 32);
		RTK_LOGI(TAG,  " dqspi3(hold) = %d\n", (pi_tmp - d_dqs0_dqs3) % 32);

		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp);
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3);

		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(pi_tmp - d_dqs0_dq0);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(pi_tmp - d_dqs0_dq1);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(pi_tmp - d_dqs0_dq2);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(pi_tmp - d_dqs0_dq3);

		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//wr_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;

		//r/w
		sram_sg();
		ddr_error_bit = sram_sg_chk();//r/w result
		//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

		for (i = 0; i < DQ_slice; i++) { //check r/w result per slice
			if ((((ddr_error_bit & 0xffffffff) & (0xff << (8 * i))) != 0) && (dqspi_hold[i] == 0xffff)) {
				if (i == 0) {
					dqspi_hold[i] = (pi_tmp - 1) % 32;
				}
				if (i == 1) {
					dqspi_hold[i] = (pi_tmp - d_dqs0_dqs1 - 1) % 32;
				}
			}
		}
		//calculate hold time map
		for (i = 0; i < DQ_slice; i++) {
			if (dqspi_hold[i] == 0xffff) {
				dqspi_hold_map[i]++;
			}
		}

		RTK_LOGI(TAG,  " dqspi_hold 0/1 = %d, %d\n", dqspi_hold[0], dqspi_hold[1]);
		RTK_LOGI(TAG,  " dqspi_hold_map 0/1 = %d, %d\n", dqspi_hold_map[0], dqspi_hold_map[1]);

		if ((dqspi_hold[0] != 0xffff) && (dqspi_hold[1] != 0xffff)) {
			break;
		}

		//pi totally shift counter
		hold_cnt++;
	}

	RTK_LOGI(TAG,  "!!!hold shift to init!!!\n");
	for (i = 1; i <= hold_cnt; i++) { //hold time shift to initial
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp - i);
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1 - i);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2 - i);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3 - i);

		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(pi_tmp - d_dqs0_dq0 - i);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(pi_tmp - d_dqs0_dq1 - i);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(pi_tmp - d_dqs0_dq2 - i);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(pi_tmp - d_dqs0_dq3 - i);

		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//wr_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
	}

	//setup time
	for (pi_tmp = dqspi_init[0] + 64; pi_tmp > dqspi_init[0] + 16; pi_tmp--) {
		DiagPrintf(" dqspi0(setup) = %d\n", pi_tmp % 32);
		DiagPrintf(" dqspi1(setup) = %d\n", (pi_tmp - d_dqs0_dqs1) % 32);
		DiagPrintf(" dqspi2(setup) = %d\n", (pi_tmp - d_dqs0_dqs2) % 32);
		DiagPrintf(" dqspi3(setup) = %d\n", (pi_tmp - d_dqs0_dqs3) % 32);

		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp);
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3);

		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(pi_tmp - d_dqs0_dq0);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(pi_tmp - d_dqs0_dq1);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(pi_tmp - d_dqs0_dq2);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(pi_tmp - d_dqs0_dq3);

		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//wr_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;

		//r/w
		sram_sg();
		ddr_error_bit = sram_sg_chk();//r/w result
		//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

		for (i = 0; i < DQ_slice; i++) { //check r/w result per slice
			if ((((ddr_error_bit & 0xffffffff) & (0xff << (8 * i))) != 0) && (dqspi_setup[i] == 0xffff)) {
				if (i == 0) {
					dqspi_setup[i] = (pi_tmp + 1) % 32;
				}
				if (i == 1) {
					dqspi_setup[i] = (pi_tmp - d_dqs0_dqs1 + 1) % 32;
				}
			}
		}
		//calculate setup time map
		for (i = 0; i < DQ_slice; i++) {
			if (dqspi_setup[i] == 0xffff) {
				dqspi_setup_map[i]++;
			}
		}

		DiagPrintf(" dqspi_setup 0/1 = %d, %d\n", dqspi_setup[0], dqspi_setup[1]);
		DiagPrintf(" dqspi_setup_map 0/1 = %d, %d\n", dqspi_setup_map[0], dqspi_setup_map[1]);

		if ((dqspi_setup[0] != 0xffff) && (dqspi_setup[1] != 0xffff)) {
			break;
		}

		//pi totally shift counter
		setup_cnt++;
	}

	RTK_LOGI(TAG,  "!!!setup shift to init!!!\n");
	for (i = 1; i <= setup_cnt; i++) { //setup time shift to initial
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp + i);
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1 + i);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2 + i);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3 + i);

		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(pi_tmp - d_dqs0_dq0 + i);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(pi_tmp - d_dqs0_dq1 + i);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(pi_tmp - d_dqs0_dq2 + i);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(pi_tmp - d_dqs0_dq3 + i);

		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//wr_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
	}

	//normalize to 0~31
	for (i = 0; i < DQ_slice; i++) {
		if (dqspi_setup[i] != 0xffff) {
			dqspi_setup[i] = dqspi_setup[i] % 32;
		}
		if (dqspi_hold[i] != 0xffff) {
			dqspi_hold[i] = dqspi_hold[i] % 32;
		}
	}

	RTK_LOGI(TAG,  " dqspi_setup 0/1 = %d, %d\n", dqspi_setup[0], dqspi_setup[1]);
	RTK_LOGI(TAG,  " dqspi_setup_map 0/1 = %d, %d\n", dqspi_setup_map[0], dqspi_setup_map[1]);

	RTK_LOGI(TAG,  " dqspi_hold 0/1 = %d, %d\n", dqspi_hold[0], dqspi_hold[1]);
	RTK_LOGI(TAG,  " dqspi_hold_map 0/1 = %d, %d\n", dqspi_hold_map[0], dqspi_hold_map[1]);
	//shift = hold map - (hold map + setup map) / 2

	/*
	int center_shift;
	//shift DQS to center
	if (dqspi_hold_map[i] >= dqspi_setup_map[i])	center_shift = (dqspi_hold_map[i] - dqspi_setup_map[i]) >> 1;
	if (dqspi_hold_map[i] < dqspi_setup_map[i])		center_shift = (dqspi_setup_map[i] - dqspi_hold_map[i]) >> 1;
	for (i = 0; i <= center_shift; i++)
	{
		if (dqspi_hold_map[i] < dqspi_setup_map[i])	i *= -1;//
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2((dqspi_init[0] + i));
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3((dqspi_init[0] + i) - d_dqs0_dqs1);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4((dqspi_init[0] + i) - d_dqs0_dqs2);
		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5((dqspi_init[0] + i) - d_dqs0_dqs3);

		ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6((dqspi_init[0] + i) - d_dqs0_dq0);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7((dqspi_init[0] + i) - d_dqs0_dq1);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8((dqspi_init[0] + i) - d_dqs0_dq2);
		ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9((dqspi_init[0] + i) - d_dqs0_dq3);
		if (dqspi_hold_map[i] < dqspi_setup_map[i])	i *= -1;//
	}
	*/
}//wrlvl

void tx_ckpi(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	//固定cmd/addr, 然後移動CK, CS, DQ, DQS PI
	int ckpi, ckpi_init;
	int d_ck_cs0, d_ck_cs1, d_ck_dqs0, d_ck_dqs1, d_ck_dqs2, d_ck_dqs3, d_ck_dq0, d_ck_dq1, d_ck_dq2, d_ck_dq3;
	int cnt_setup = 0, cnt_hold = 0, cnt = 0;// count pi pass map setup/hold/total
	int ddr_error_bit;
	//int center_shift, i;

	//fw_set_mode: immediately
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//fw_set_mode: during refresh
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	//===============================================================================//
	//  ( CK / CS / DQS / DQ ) should change according to each project
	//===============================================================================//
	ckpi_init = DDRPHY_GET_DPI_POST_PI_SEL0(ddr_phy->DDRPHY_PLL_PI0);//CK_0
	//ckpi_init = DDRPHY_GET_DPI_POST_PI_SEL12(ddr_phy->DDRPHY_PLL_PI3);//CK_1
	ckpi = ckpi_init;

	ddr_phy->DDRPHY_CRT_CTL |= DDRPHY_BIT_MCK_CKE_SEL; //CKE follow cs0

	d_ck_cs0 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL10(ddr_phy->DDRPHY_PLL_PI2);//CS0
	d_ck_cs1 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL11(ddr_phy->DDRPHY_PLL_PI3);//CS1

	d_ck_dqs0 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL2(ddr_phy->DDRPHY_PLL_PI0);
	d_ck_dqs1 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL3(ddr_phy->DDRPHY_PLL_PI0);
	d_ck_dqs2 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL4(ddr_phy->DDRPHY_PLL_PI1);
	d_ck_dqs3 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL5(ddr_phy->DDRPHY_PLL_PI1);

	d_ck_dq0 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL6(ddr_phy->DDRPHY_PLL_PI1);
	d_ck_dq1 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL7(ddr_phy->DDRPHY_PLL_PI2);
	d_ck_dq2 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL8(ddr_phy->DDRPHY_PLL_PI2);
	d_ck_dq3 = ckpi - DDRPHY_GET_DPI_POST_PI_SEL9(ddr_phy->DDRPHY_PLL_PI2);

	//Rx ODT always on
	ddr_phy->DDRPHY_READ_CTRL_2_SETx[0] |= DDRPHY_BIT_DQ_ODT_FORCE_SIG | DDRPHY_BIT_DQ_ODT_FORCE_SEL | DDRPHY_BIT_DQS_ODT_FORCE_SIG | DDRPHY_BIT_DQS_ODT_FORCE_SEL;
	ddr_phy->DDRPHY_READ_CTRL_2_SETx[1] |= DDRPHY_BIT_DQ_ODT_FORCE_SIG | DDRPHY_BIT_DQ_ODT_FORCE_SEL | DDRPHY_BIT_DQS_ODT_FORCE_SIG | DDRPHY_BIT_DQS_ODT_FORCE_SEL;

	//fw_set_mode: immediately
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//wr_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
	//rd_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

	ckpi += 64;//初始值+64是為了不要往左掃時扣到負的(cmd是2T所以有可能>32pi??)

	if (DDR_PHY_Scan_param_setup_map() != TX_SCAN_SETUP_MAP) {
		while (cnt_hold < 64) { // scan ckpi hold time(1D)
			DiagPrintf("===========================================================\n");
			DiagPrintf("ckpi(hold) = %d\n", ckpi % 32);
			DiagPrintf("CSPI0(hold) = %d\n", (ckpi - d_ck_cs0) % 32);
			DiagPrintf("CSPI1(hold) = %d\n", (ckpi - d_ck_cs1) % 32);
			DiagPrintf("DQSPI0(hold) = %d\n", (ckpi - d_ck_dqs0) % 32);
			DiagPrintf("DQSPI1(hold) = %d\n", (ckpi - d_ck_dqs1) % 32);
			DiagPrintf("DQSPI2(hold) = %d\n", (ckpi - d_ck_dqs2) % 32);
			DiagPrintf("DQSPI3(hold) = %d\n", (ckpi - d_ck_dqs3) % 32);
			DiagPrintf("DQPI0(hold) = %d\n", (ckpi - d_ck_dq0) % 32);
			DiagPrintf("DQPI1(hold) = %d\n", (ckpi - d_ck_dq1) % 32);
			DiagPrintf("DQPI2(hold) = %d\n", (ckpi - d_ck_dq2) % 32);
			DiagPrintf("DQPI3(hold) = %d\n", (ckpi - d_ck_dq3) % 32);

			ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL0)) | DDRPHY_DPI_POST_PI_SEL0(ckpi);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL10) | DDRPHY_DPI_POST_PI_SEL10(ckpi - d_ck_cs0);
			ddr_phy->DDRPHY_PLL_PI3 = (ddr_phy->DDRPHY_PLL_PI3 & ~DDRPHY_MASK_DPI_POST_PI_SEL11) | DDRPHY_DPI_POST_PI_SEL11(ckpi - d_ck_cs1);

			ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(ckpi - d_ck_dqs0);
			ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(ckpi - d_ck_dqs1);
			ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(ckpi - d_ck_dqs2);
			ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(ckpi - d_ck_dqs3);

			ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(ckpi - d_ck_dq0);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(ckpi - d_ck_dq1);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(ckpi - d_ck_dq2);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(ckpi - d_ck_dq3);

			//fw_set_mode: immediately
			//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
			//wr_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			//rd_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

			//r/w
			sram_sg();
			ddr_error_bit = sram_sg_chk();//r/w result
			//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

			if (ddr_error_bit != 0) {
				//reset之後ckpi應該會回到初始點所以不用移回去
				ckpi -= cnt_hold;//"variable" ckpi shift to initial
				break;
			}

			if (cnt_hold >= 48) {
				break;
			}

			ckpi++;
			cnt_hold++;
		}//cnt_hold
	} else {
		while (cnt_setup < 64) { // scan ckpi setup time
			DiagPrintf("===========================================================\n");
			DiagPrintf("ckpi(setup) = %d\n", ckpi % 32);
			DiagPrintf("CSPI0(setup) = %d\n", (ckpi - d_ck_cs0) % 32);
			DiagPrintf("CSPI1(setup) = %d\n", (ckpi - d_ck_cs1) % 32);
			DiagPrintf("DQSPI0(setup) = %d\n", (ckpi - d_ck_dqs0) % 32);
			DiagPrintf("DQSPI1(setup) = %d\n", (ckpi - d_ck_dqs1) % 32);
			DiagPrintf("DQSPI2(setup) = %d\n", (ckpi - d_ck_dqs2) % 32);
			DiagPrintf("DQSPI3(setup) = %d\n", (ckpi - d_ck_dqs3) % 32);
			DiagPrintf("DQPI0(setup) = %d\n", (ckpi - d_ck_dq0) % 32);
			DiagPrintf("DQPI1(setup) = %d\n", (ckpi - d_ck_dq1) % 32);
			DiagPrintf("DQPI2(setup) = %d\n", (ckpi - d_ck_dq2) % 32);
			DiagPrintf("DQPI3(setup) = %d\n", (ckpi - d_ck_dq3) % 32);

			ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL0)) | DDRPHY_DPI_POST_PI_SEL0(ckpi);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL10) | DDRPHY_DPI_POST_PI_SEL10(ckpi - d_ck_cs0);
			ddr_phy->DDRPHY_PLL_PI3 = (ddr_phy->DDRPHY_PLL_PI3 & ~DDRPHY_MASK_DPI_POST_PI_SEL11) | DDRPHY_DPI_POST_PI_SEL11(ckpi - d_ck_cs1);

			ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(ckpi - d_ck_dqs0);
			ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(ckpi - d_ck_dqs1);
			ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(ckpi - d_ck_dqs2);
			ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(ckpi - d_ck_dqs3);

			ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(ckpi - d_ck_dq0);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(ckpi - d_ck_dq1);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(ckpi - d_ck_dq2);
			ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(ckpi - d_ck_dq3);

			//fw_set_mode: immediately
			//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
			//wr_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			//rd_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

			//r/w
			sram_sg();
			ddr_error_bit = sram_sg_chk();//r/w result
			//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

			if (ddr_error_bit != 0) {
				//reset之後ckpi應該會回到初始點所以不用移回去
				ckpi += cnt_setup;//"variable" ckpi shift to initial
				break;
			}

			if (cnt_setup >= 48) {
				break;
			}

			ckpi--;
			cnt_setup++;
		}//cnt_setup
	}
	//fw_set_mode: no read
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	cnt = (cnt_setup - 1) + (cnt_hold - 1) + 1;//計算pass的map有多大，初始點也要算所以+1

	RTK_LOGI(TAG,  " hold pi = %d\n", (ckpi + cnt_hold - 1) % 32);
	RTK_LOGI(TAG,  " setup pi = %d\n", (ckpi - cnt_setup + 1) % 32);

	RTK_LOGI(TAG,  " hold map = %d\n", cnt_hold - 1);
	RTK_LOGI(TAG,  " setup map = %d\n", cnt_setup - 1);
	RTK_LOGI(TAG,  " map size = %d\n", cnt);
	//shift = hold map - (hold map + setup map) / 2
}

void tx_cspi(void)// cspi
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	int cnt_setup = 0, cnt_hold = 0, cnt;// count pi pass map setup/hold/total
	//int , center_shift = 0, i
	int cspi, cspi0_init, ddr_error_bit;
//	int cspi1_init;

	//fw_set_mode: during refresh
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	//===============================================================================//
	//  CS  should change according to each project
	//===============================================================================//
	ddr_phy->DDRPHY_CRT_CTL |= DDRPHY_BIT_MCK_CKE_SEL; //CKE follow cs0

	int cs_sel = 0;
	if (cs_sel == 0) {
		cspi0_init = DDRPHY_GET_DPI_POST_PI_SEL10(ddr_phy->DDRPHY_PLL_PI2);    //mck_cs_0
	}
//	if (cs_sel == 1) {
//		cspi1_init = DDRPHY_GET_DPI_POST_PI_SEL11(ddr_phy->DDRPHY_PLL_PI3);    //mck_cs_1
//	}

	if (cs_sel == 0) {
		cspi = cspi0_init;
	}
//	if (cs_sel == 1) {
//		cspi = cspi1_init;
//	}

	cspi += 32;//初始值+32是為了不要往左掃時扣到負的(cs是1T所以+32)

	if (DDR_PHY_Scan_param_setup_map() != TX_SCAN_SETUP_MAP) {
		while (cnt_hold < 32) { // scan cspi hold time(1D)
			DiagPrintf("CSPI %d (hold) = %d\n", cs_sel, cspi % 32);
			if (cs_sel == 0) {
				ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL10) | DDRPHY_DPI_POST_PI_SEL10(cspi);
			}
//		if (cs_sel == 1) {
//			ddr_phy->DDRPHY_PLL_PI3 = (ddr_phy->DDRPHY_PLL_PI3 & ~DDRPHY_MASK_DPI_POST_PI_SEL11) | DDRPHY_DPI_POST_PI_SEL11(cspi);
//		}

			//fw_set_mode: immediately
			//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
			//wr_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			//rd_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

			//r/w
			sram_sg();
			ddr_error_bit = sram_sg_chk();//r/w result
			//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

			if (ddr_error_bit != 0) {
				RTK_LOGE(TAG, "===========================================================Read Error\n");
				//reset之後ckpi如果會回到初始點就不用移回去
				cspi -= cnt_hold;//"variable" cspi shift to initial
				break;
			}

			cspi++;
			cnt_hold++;
		}//cnt_hold
	} else {
		while (cnt_setup < 32) {
			DiagPrintf("CSPI %d (setup) = %d\n", cs_sel, cspi % 32);
			if (cs_sel == 0) {
				ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL10) | DDRPHY_DPI_POST_PI_SEL10(cspi);
			}
//		if (cs_sel == 1) {
//			ddr_phy->DDRPHY_PLL_PI3 = (ddr_phy->DDRPHY_PLL_PI3 & ~DDRPHY_MASK_DPI_POST_PI_SEL11) | DDRPHY_DPI_POST_PI_SEL11(cspi);
//		}

			//fw_set_mode: immediately
			//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
			//wr_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			//rd_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

			//r/w
			sram_sg();
			ddr_error_bit = sram_sg_chk();//r/w result
			//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

			if (ddr_error_bit != 0) {
				RTK_LOGE(TAG, "===========================================================Read Error\n");
				//reset之後ckpi如果會回到初始點就不用移回去
				cspi += cnt_setup;//"variable" cspi shift to initial
				break;
			}

			cspi--;
			cnt_setup++;

		}//cnt_setup
	}
//calculate r/w pass map, +1 for the initial pi
	cnt = (cnt_setup - 1) + (cnt_hold - 1) + 1;
	(void) cnt;/* To avoid gcc warnings */

	RTK_LOGI(TAG,  "============== setup pi = %d\n", (cspi - cnt_setup + 1) % 32);
	RTK_LOGI(TAG,  "============== hold pi = %d\n", (cspi + cnt_hold - 1) % 32);
	RTK_LOGI(TAG,  "~~~~~~~ cspi setup map = %d\n", cnt_setup - 1);
	RTK_LOGI(TAG,  "~~~~~~~ cspi hold map = %d\n", cnt_hold - 1);
	//shift =cspi hold map - (cspi hold map + cspi setup map) / 2
}

void tx_dqs(void)
{
	//scan every vref with the same initial pi
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	DDRC_TypeDef *ddrc = DDRC_DEV;

#define SCAN_PER_SLICE 0
#define SCAN_VREF 0
	int dqspi_init[4], hold_count, setup_count, pi_tmp, ddr_error_bit, i;
	int dqspi_setup[4], dqspi_hold[4];
	int MR6_VR, MR6_VREF, MR6_tccd_L, vref_step_init, vref_step;

	int scan_vref_tune = 4, vref_up_down = 0, scan_vref_cnt_loop;
	int scan_vref_cnt_up = 1, scan_vref_cnt_down = 1;// scan upward and downward except the middle(initial) vref step

	float vref[100];

	int dqspi2D_hold[4][100] = {0};
	int dqspi2D_setup[4][100] = {0};

	//fw_set_mode: no read
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	dqspi_init[0] = DDRPHY_GET_DPI_POST_PI_SEL2(ddr_phy->DDRPHY_PLL_PI0); //mck_dqs_0
	dqspi_init[1] = DDRPHY_GET_DPI_POST_PI_SEL3(ddr_phy->DDRPHY_PLL_PI0); //mck_dqs_1
	dqspi_init[2] = DDRPHY_GET_DPI_POST_PI_SEL4(ddr_phy->DDRPHY_PLL_PI1); //mck_dqs_2
	dqspi_init[3] = DDRPHY_GET_DPI_POST_PI_SEL5(ddr_phy->DDRPHY_PLL_PI1); //mck_dqs_3

	//MR6_VR = (ddrc->DDRC_CDPIN_NDGE >> 6) & 1;   //!!!!! not MR6 !!!!! already change vref with DPIN
	//MR6_VREF = ddrc->DDRC_CDPIN_NDGE & 0x3F;
	//MR6_VR = MR6_RANGE_init;
	//MR6_VREF = MR6_VREF_init;
	MR6_VR = (ddrc->DDRC_MR6 >> 6) & 0x1;
	MR6_VREF = ddrc->DDRC_MR6 & 0x3F;
	MR6_tccd_L = (ddrc->DDRC_MR6 >> 10) & 0x7;

	(void) MR6_tccd_L;/* To avoid gcc warnings */
	(void) vref;/* To avoid gcc warnings */

	if (MR6_VR == 1) { //range2
		vref_step_init = MR6_VREF;
	} else {
		vref_step_init = MR6_VREF + 23;
	}
	//RTK_LOGI(TAG,  "MR6_VR = %d\n", MR6_VR);
	//RTK_LOGI(TAG,  "MR6_VREF = %d\n", MR6_VREF);
	//RTK_LOGI(TAG,  " Vref init = %f\n", vref_step_init);
	for (vref_up_down = 0; vref_up_down < 2; vref_up_down++) { //scan vref upward or downward
		if (vref_up_down == 0) {
			scan_vref_tune *= 1;
			scan_vref_cnt_loop = scan_vref_cnt_up;
		}
		if (vref_up_down == 1) {
			scan_vref_tune *= -1;
			scan_vref_cnt_loop = scan_vref_cnt_down;
		}

		//scan vref
		for (vref_step = vref_step_init; vref_step < 74; vref_step = vref_step + scan_vref_tune) {
			if ((vref_up_down == 1) && (vref_step == vref_step_init)) { // do not scan initial vref value in downward direction
				scan_vref_cnt_loop--;
				continue;
			}

			for (i = 0; i < 4; i++) {
				dqspi_setup[i] = 0xff;
				dqspi_hold[i] = 0xff;
			}

			if (vref_step < 23) {
				vref[vref_step] = 45 + vref_step * 0.65;
			} else {
				vref[vref_step] = 60 + (vref_step - 23) * 0.65;
			}
			//RTK_LOGI(TAG,  " Vref = %f\n", vref[vref_step]);

#if SCAN_PER_SLICE == 0
			int d_dqs0_dqs1, d_dqs0_dqs2, d_dqs0_dqs3;
			d_dqs0_dqs1 = dqspi_init[0] - dqspi_init[1];
			d_dqs0_dqs2 = dqspi_init[0] - dqspi_init[2];
			d_dqs0_dqs3 = dqspi_init[0] - dqspi_init[3];

			hold_count = 0;
			//hold time
			for (pi_tmp = dqspi_init[0] + 32; pi_tmp < dqspi_init[0] + 48; pi_tmp++) {
				//DiagPrintf(" dqspi0(hold) = %d\n", pi_tmp % 32);
				//DiagPrintf(" dqspi1(hold) = %d\n", (pi_tmp - d_dqs0_dqs1) % 32);
				//DiagPrintf(" dqspi2(hold) = %d\n", (pi_tmp - d_dqs0_dqs2) % 32);
				//DiagPrintf(" dqspi3(hold) = %d\n", (pi_tmp - d_dqs0_dqs3) % 32);

				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL2)) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp);
				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL3)) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL4)) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL5)) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3);

				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;

				//r/w
				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result
				//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

				//DDR_tool output format
				//printf("%f, %d, %d, %d, %d, %d, 0x%08x\n", vref[vref_step], DQ_slice, (pi_tmp % 32), ((pi_tmp - d_dqs0_dqs1) % 32), ((pi_tmp - d_dqs0_dqs2) % 32),
				//	   ((pi_tmp - d_dqs0_dqs3) % 32), ddr_error_bit);
				DiagPrintf("%d, %d, %d, %d, 0x%08x\n", (pi_tmp % 32), ((pi_tmp - d_dqs0_dqs1) % 32), ((pi_tmp - d_dqs0_dqs2) % 32), ((pi_tmp - d_dqs0_dqs3) % 32),
						   ddr_error_bit);

				//check r/w result per slice
				if (((ddr_error_bit & (0xff << (8 * 0))) != 0) && (dqspi_hold[0] == 0xff)) {
					dqspi_hold[0] = pi_tmp - 1;
				}
				if (((ddr_error_bit & (0xff << (8 * 1))) != 0) && (dqspi_hold[1] == 0xff)) {
					dqspi_hold[1] = pi_tmp - d_dqs0_dqs1 - 1;
				}

				if ((dqspi_hold[0] != 0xff) && (dqspi_hold[1] != 0xff)) {
					break;
				}

				hold_count++;
			}//hold time

			for (i = 1; i <= hold_count; i++) { //hold time shift to initial
				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL2)) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp - i);
				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL3)) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1 - i);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL4)) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2 - i);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL5)) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3 - i);

				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			}

			setup_count = 0;
			//setup time
			for (pi_tmp = dqspi_init[0] + 32; pi_tmp > dqspi_init[0] + 16; pi_tmp--) {
				//DiagPrintf(" dqspi0(setup) = %d\n", pi_tmp % 32);
				//DiagPrintf(" dqspi1(setup) = %d\n", (pi_tmp - d_dqs0_dqs1) % 32);
				//DiagPrintf(" dqspi2(setup) = %d\n", (pi_tmp - d_dqs0_dqs2) % 32);
				//DiagPrintf(" dqspi3(setup) = %d\n", (pi_tmp - d_dqs0_dqs3) % 32);

				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL2)) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp);
				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL3)) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL4)) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL5)) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3);

				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;

				//r/w
				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result
				//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

				//DDR_tool output format
				//printf("%f, %d, %d, %d, %d, %d, 0x%08x\n", vref[vref_step], DQ_slice, (pi_tmp % 32), ((pi_tmp - d_dqs0_dqs1) % 32), ((pi_tmp - d_dqs0_dqs2) % 32),
				//	   ((pi_tmp - d_dqs0_dqs3) % 32), ddr_error_bit);
				DiagPrintf("%d, %d, %d, %d, 0x%08x\n", (pi_tmp % 32), ((pi_tmp - d_dqs0_dqs1) % 32), ((pi_tmp - d_dqs0_dqs2) % 32), ((pi_tmp - d_dqs0_dqs3) % 32),
						   ddr_error_bit);

				//check r/w result per slice
				if (((ddr_error_bit & (0xff << (8 * 0))) != 0) && (dqspi_setup[0] == 0xff)) {
					dqspi_setup[0] = pi_tmp + 1;
				}
				if (((ddr_error_bit & (0xff << (8 * 1))) != 0) && (dqspi_setup[1] == 0xff)) {
					dqspi_setup[1] = pi_tmp - d_dqs0_dqs1 + 1;
				}

				if ((dqspi_setup[0] != 0xff) && (dqspi_setup[1] != 0xff)) {
					break;
				}

				setup_count++;
			}//setup time

			for (i = 1; i <= setup_count; i++) { //setup time shift to initial pi
				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL2)) | DDRPHY_DPI_POST_PI_SEL2(pi_tmp + i);
				ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL3)) | DDRPHY_DPI_POST_PI_SEL3(pi_tmp - d_dqs0_dqs1 + i);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL4)) | DDRPHY_DPI_POST_PI_SEL4(pi_tmp - d_dqs0_dqs2 + i);
				ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~(DDRPHY_MASK_DPI_POST_PI_SEL5)) | DDRPHY_DPI_POST_PI_SEL5(pi_tmp - d_dqs0_dqs3 + i);

				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			}
#endif

			RTK_LOGI(TAG,  "================================\n");
			dqspi2D_hold[0][vref_step] = dqspi_hold[0] % 32;
			dqspi2D_hold[1][vref_step] = dqspi_hold[1] % 32;
			dqspi2D_hold[2][vref_step] = dqspi_hold[2] % 32;
			dqspi2D_hold[3][vref_step] = dqspi_hold[3] % 32;
			dqspi2D_setup[0][vref_step] = dqspi_setup[0] % 32;
			dqspi2D_setup[1][vref_step] = dqspi_setup[1] % 32;
			dqspi2D_setup[2][vref_step] = dqspi_setup[2] % 32;
			dqspi2D_setup[3][vref_step] = dqspi_setup[3] % 32;
			RTK_LOGI(TAG,  "dqspi_hold 0 / 1 / 2 / 3 = %d, %d, %d, %d\n", dqspi2D_hold[0][vref_step], dqspi2D_hold[1][vref_step],
					 dqspi2D_hold[2][vref_step], dqspi2D_hold[3][vref_step]);
			RTK_LOGI(TAG,  "dqspi_setup 0 / 1 / 2 / 3 = %d, %d, %d, %d\n", dqspi2D_setup[0][vref_step], dqspi2D_setup[1][vref_step],
					 dqspi2D_setup[2][vref_step], dqspi2D_setup[3][vref_step]);
#if !SCAN_VREF
			RTK_LOGI(TAG,  "Do not scan tx vref\n");
			break;
#endif
			//RTK_LOGI(TAG,  "=======================================\n");
			//RTK_LOGI(TAG,  "===========END OF ONE VREF STEP==========\n");
			//RTK_LOGI(TAG,  "=======================================\n");

			scan_vref_cnt_loop--;
			if (scan_vref_cnt_loop < 0) {
				break;
			}
		}//scan vref
#if !SCAN_VREF
		break;
#endif
	}//scan vref upward or downward

	//shift = dqspi_init - (dqspi_hold + dqspi_setup) / 2
}//tx dqs

void rx_dqsen(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

#define SCAN_DELAY 0
#define SCAN_PER_SLICE 0
	int dqsen, dqsen_init[4];//dqsen_dly;//要掃的dqsen、dqsen的初始值、DQS安全前的最後一檔
	int ddr_error_bit;
	int d_dqsen0_dqsen1;
	//int dqs_int_flag=1;//判斷dqs_en是否跑到preamble的0的部分
	int dqs_int[DQ_slice], dqsen_hold[DQ_slice], dqsen_delay_hold[DQ_slice];//dqsen scan all slice at the same time

	//fw_set_mode: during no read
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	//store the initial value of dqs_en
	dqsen_init[0] = ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] & 0x7F;
	dqsen_init[1] = ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & 0x7F;

	d_dqsen0_dqsen1 = dqsen_init[0] - dqsen_init[1];

	for (int i = 0; i < DQ_slice; i++) {
		dqsen_hold[i] = 0xFF;
		dqsen_delay_hold[i] = 0xFF;
	}

	(void) dqsen_delay_hold;/* To avoid gcc warnings */

	//fw_dqsen_ftun_upd = 1
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] |= DDRPHY_BIT_FW_DQSEN_FTUN_UPD;
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] |= DDRPHY_BIT_FW_DQSEN_FTUN_UPD;

	//set dqsen delay = 0
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] &= ~DDRPHY_MASK_DQS_EN_DLY_SEL;
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] &= ~DDRPHY_MASK_DQS_EN_DLY_SEL;

	//fw_set_mode: immediately
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//rd_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

	//rst_dqsen_mode: 0x210, dqs_int of first point???
	ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_DQSEN_MODE)) | DDRPHY_RST_DQSEN_MODE(0x1);
	ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_DQSEN_MODE)) | DDRPHY_RST_DQSEN_MODE(0x0);

	// scan dqs_en
	for (dqsen = dqsen_init[0] ; dqsen < 12; dqsen++) {
		DiagPrintf(" dqsen0(->) = %d\n", dqsen);
		DiagPrintf(" dqsen1(->) = %d\n", dqsen - d_dqsen0_dqsen1);

		//set dqs_en
		ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] & ~0x7F) | ((dqsen) & 0x7F);
		ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & ~0x7F) | ((dqsen - d_dqsen0_dqsen1) & 0x7F);

		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//rd_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

		//r/w
		sram_sg();
		ddr_error_bit = sram_sg_chk();//r/w result
		//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

		//dqs_int
		dqs_int[0] = (ddr_phy->DDRPHY_INT_STATUS_0 & 0x1);
		dqs_int[1] = (ddr_phy->DDRPHY_INT_STATUS_0 & 0x2) >> 1;

		//rst_fifo_mode: 0x208
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_RST_FIFO_MODE)) | DDRPHY_RST_FIFO_MODE(0x3);
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_RST_FIFO_MODE)) | DDRPHY_RST_FIFO_MODE(0x2);
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_RST_FIFO_MODE)) | DDRPHY_RST_FIFO_MODE(0x1);
		//rst_dqsen_mode: 0x210
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_DQSEN_MODE)) | DDRPHY_RST_DQSEN_MODE(0x1);
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_DQSEN_MODE)) | DDRPHY_RST_DQSEN_MODE(0x0);
		//rst_3point_mode: 0x210
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_3POINT_MODE)) | DDRPHY_RST_3POINT_MODE(0x1);
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_3POINT_MODE)) | DDRPHY_RST_3POINT_MODE(0x0);

		//printf("~~~~~ dqs_int ~~~~~~~\n");
		DiagPrintf("dqs_int: 0x%08x, ddr_error_bit: 0x%08x\n", ddr_phy->DDRPHY_INT_STATUS_0, ddr_error_bit);

		ddr_phy->DDRPHY_INT_CTRL |= DDRPHY_BIT_INT_WRITE_EN_2;
		ddr_phy->DDRPHY_INT_CTRL |= DDRPHY_BIT_FW_CLR_DQS_INT; // fw clear dqs_int
		ddr_phy->DDRPHY_INT_STATUS_0 &= ~(DDRPHY_BIT_DQS_INT_0 | DDRPHY_BIT_DQS_INT_1 | DDRPHY_BIT_DQS_INT_2 | DDRPHY_BIT_DQS_INT_3); //dqs_int
		ddr_phy->DDRPHY_INT_STATUS_0 &= ~(DDRPHY_BIT_TM_DQSEN_LATE_INT_0 | DDRPHY_BIT_TM_DQSEN_LATE_INT_1 | DDRPHY_BIT_TM_DQSEN_LATE_INT_2 |
										  DDRPHY_BIT_TM_DQSEN_LATE_INT_3); //dqs_int late
		ddr_phy->DDRPHY_INT_STATUS_0 &= ~(DDRPHY_BIT_TM_DQSEN_EARLY_INT_0 | DDRPHY_BIT_TM_DQSEN_EARLY_INT_1 | DDRPHY_BIT_TM_DQSEN_EARLY_INT_2 |
										  DDRPHY_BIT_TM_DQSEN_EARLY_INT_3); //dqs_int early
		ddr_phy->DDRPHY_INT_CTRL &= ~DDRPHY_BIT_FW_CLR_DQS_INT;

		//check r/w result per slice
		/*if (((ddr_error_bit & (0xff << (8 * 0))) != 0) && (dqsen_hold[0] == 0xff))
				dqsen_hold[0] = dqsen - 1;
		if (((ddr_error_bit & (0xff << (8 * 1))) != 0) && (dqsen_hold[1] == 0xff))
				dqsen_hold[1] = dqsen - d_dqsen0_dqsen1 - 1;
		if (((ddr_error_bit & (0xff << (8 * 2))) != 0) && (dqsen_hold[2] == 0xff))
				dqsen_hold[2] = dqsen - d_dqsen0_dqsen2 - 1;
		if (((ddr_error_bit & (0xff << (8 * 3))) != 0) && (dqsen_hold[3] == 0xff))
				dqsen_hold[3] = dqsen - d_dqsen0_dqsen3 - 1;
		if ((dqsen_hold[0] != 0xff) && (dqsen_hold[1] != 0xff) && (dqsen_hold[2] != 0xff) && (dqsen_hold[3] != 0xff))
				break;
		*/

		//check dqs_int per slice
		if ((dqs_int[0] == 1) && (dqsen_hold[0] == 0xff)) {
			dqsen_hold[0] = dqsen - 1;
		}
		if ((dqs_int[1] == 1) && (dqsen_hold[1] == 0xff)) {
			dqsen_hold[1] = dqsen - 1;
		}

		//if ((dqsen_hold[0] != 0xff) && (dqsen_hold[1] != 0xff) && (dqsen_hold[2] != 0xff) && (dqsen_hold[3] != 0xff))
		//		  break;

	}//scan dqsen

	//set dqsen to dqs_int boundary
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] & ~0x7F) | ((dqsen_hold[0]) & 0x7F);
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & ~0x7F) | ((dqsen_hold[1]) & 0x7F);

	//fw_set_mode: immediately
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//rd_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

#if SCAN_DELAY
	//may reset first??
	// scan dqs_en delay
	for (dqsen_dly = 1; dqsen_dly < 64; dqsen_dly += 1) {
		DiagPrintf(" dqsen dly(->) = %d\n", dqsen_dly);
		ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] & ~DDRPHY_MASK_DQS_EN_DLY_SEL) | DDRPHY_DQS_EN_DLY_SEL(dqsen_dly);
		ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & ~DDRPHY_MASK_DQS_EN_DLY_SEL) | DDRPHY_DQS_EN_DLY_SEL(dqsen_dly);

		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//rd_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

		//r/w
		sram_sg();
		ddr_error_bit = sram_sg_chk();//r/w result
		//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

		//dqs_int
		dqs_int[0] = (ddr_phy->DDRPHY_INT_STATUS_0 & 0x1);
		dqs_int[1] = (ddr_phy->DDRPHY_INT_STATUS_0 & 0x2) >> 1;

		//if too much delay tap??
		//rst_fifo_mode: 0x208
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_RST_FIFO_MODE)) | DDRPHY_RST_FIFO_MODE(0x3);
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_RST_FIFO_MODE)) | DDRPHY_RST_FIFO_MODE(0x2);
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_RST_FIFO_MODE)) | DDRPHY_RST_FIFO_MODE(0x1);
		//rst_dqsen_mode: 0x210
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_DQSEN_MODE)) | DDRPHY_RST_DQSEN_MODE(0x1);
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_DQSEN_MODE)) | DDRPHY_RST_DQSEN_MODE(0x0);
		//rst_3point_mode: 0x210
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_3POINT_MODE)) | DDRPHY_RST_3POINT_MODE(0x1);
		ddr_phy->DDRPHY_DPI_CTRL_2 = (ddr_phy->DDRPHY_DPI_CTRL_2 & ~(DDRPHY_MASK_RST_3POINT_MODE)) | DDRPHY_RST_3POINT_MODE(0x0);


		DiagPrintf("dqs_int: 0x%08x, ddr_error_bit: 0x%08x\n", ddr_phy->DDRPHY_INT_STATUS_0, ddr_error_bit);

		ddr_phy->DDRPHY_INT_CTRL |= DDRPHY_BIT_INT_WRITE_EN_2;
		ddr_phy->DDRPHY_INT_CTRL |= DDRPHY_BIT_FW_CLR_DQS_INT; // fw clear dqs_int
		ddr_phy->DDRPHY_INT_STATUS_0 &= ~(DDRPHY_BIT_DQS_INT_0 | DDRPHY_BIT_DQS_INT_1 | DDRPHY_BIT_DQS_INT_2 | DDRPHY_BIT_DQS_INT_3); //dqs_int
		ddr_phy->DDRPHY_INT_STATUS_0 &= ~(DDRPHY_BIT_TM_DQSEN_LATE_INT_0 | DDRPHY_BIT_TM_DQSEN_LATE_INT_1 | DDRPHY_BIT_TM_DQSEN_LATE_INT_2 |
										  DDRPHY_BIT_TM_DQSEN_LATE_INT_3); //dqs_int late
		ddr_phy->DDRPHY_INT_STATUS_0 &= ~(DDRPHY_BIT_TM_DQSEN_EARLY_INT_0 | DDRPHY_BIT_TM_DQSEN_EARLY_INT_1 | DDRPHY_BIT_TM_DQSEN_EARLY_INT_2 |
										  DDRPHY_BIT_TM_DQSEN_EARLY_INT_3); //dqs_int early
		ddr_phy->DDRPHY_INT_CTRL &= ~DDRPHY_BIT_FW_CLR_DQS_INT;

		//check r/w result per slice
		/*if (((ddr_error_bit & (0xff << (8 * 0))) != 0) && (dqsen_delay_hold[0] == 0xff))
				dqsen_delay_hold[0] = dqsen_dly - 1;
		if (((ddr_error_bit & (0xff << (8 * 1))) != 0) && (dqsen_delay_hold[1] == 0xff))
				dqsen_delay_hold[1] = dqsen_dly - 1;
		if (((ddr_error_bit & (0xff << (8 * 2))) != 0) && (dqsen_delay_hold[2] == 0xff))
				dqsen_delay_hold[2] = dqsen_dly - 1;
		if (((ddr_error_bit & (0xff << (8 * 3))) != 0) && (dqsen_delay_hold[3] == 0xff))
				dqsen_delay_hold[3] = dqsen_dly - 1;
		if ((dqsen_delay_hold[0] != 0xff) && (dqsen_delay_hold[1] != 0xff) && (dqsen_delay_hold[2] != 0xff) && (dqsen_delay_hold[3] != 0xff))
				break;
		*/

		//check dqs_int per slice
		if ((dqs_int[0] == 1) && (dqsen_delay_hold[0] == 0xff)) {
			dqsen_delay_hold[0] = dqsen_dly - 1;
		}
		if ((dqs_int[1] == 1) && (dqsen_delay_hold[1] == 0xff)) {
			dqsen_delay_hold[1] = dqsen_dly - 1;
		}
		if ((dqsen_delay_hold[0] != 0xff) && (dqsen_delay_hold[1] != 0xff)) {
			break;
		}
	}//scan dqsen delay

	//把dqs_en delay設定到要框的邊界
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] & ~DDRPHY_MASK_DQS_EN_DLY_SEL) | DDRPHY_DQS_EN_DLY_SEL(dqsen_delay_hold[0]);
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & ~DDRPHY_MASK_DQS_EN_DLY_SEL) | DDRPHY_DQS_EN_DLY_SEL(dqsen_delay_hold[1]);
#endif
#if SCAN_DELAY
	//把dqs_en設定到掃出來的邊界前 1T (DDR2)
	dqsen_hold[0] = dqsen_hold[0] - 2;
	dqsen_hold[1] = dqsen_hold[1] - 2;
#else
	dqsen_hold[0] = dqsen_hold[0] - 1;
	dqsen_hold[1] = dqsen_hold[1] - 1;
#endif
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] & ~0x7F) | (dqsen_hold[0] & 0x7F);
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & ~0x7F) | (dqsen_hold[1] & 0x7F);

	//fw_set_mode: immediately
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//rd_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

	RTK_LOGI(TAG,  "dqsen_hold 0, 1 = %d, %d\n", dqsen_hold[0], dqsen_hold[1]);
#if SCAN_DELAY
	RTK_LOGI(TAG,  "dqsen delay 0, 1 = %d, %d\n", dqsen_delay_hold[0], dqsen_delay_hold[1]);
#endif

	//This is result
	RTK_LOGI(TAG,  "DDRPHY_READ_CTRL_0_SETx[0]: 0x%08x\n", ddr_phy->DDRPHY_READ_CTRL_0_SETx[0]);
	RTK_LOGI(TAG,  "DDRPHY_READ_CTRL_0_SETx[1]: 0x%08x\n", ddr_phy->DDRPHY_READ_CTRL_0_SETx[1]);

	//fw_dqsen_ftun_upd = 0
	//ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] &= ~DDRPHY_BIT_FW_DQSEN_FTUN_UPD;
	//ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] &= ~DDRPHY_BIT_FW_DQSEN_FTUN_UPD;
	//ddr_phy->DDRPHY_READ_CTRL_4_SETx[2] &= ~DDRPHY_BIT_FW_DQSEN_FTUN_UPD;
	//ddr_phy->DDRPHY_READ_CTRL_4_SETx[3] &= ~DDRPHY_BIT_FW_DQSEN_FTUN_UPD;
}//rx_dqsen

void SOC_DQ_VREF(int step)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//REG_WRITE64_Mask2(ddr_phy->DDRPHY_CAL_MODE_CTRL, CAL_MODE_CTRL_FW_VR_UPD_0, 4, 0xf); // 1: vref_s can be applied to Vref pad by fw_set_rd_dly
	//REG_READ64(ddr_phy->DDRPHY_0x374);
	//REG_READ64(ddr_phy->DDRPHY_0x378);

	//set vref
	ddr_phy->DDRPHY_PAD_BUS_2 = (ddr_phy->DDRPHY_PAD_BUS_2 & ~DDRPHY_MASK_VREF_S_0) | DDRPHY_VREF_S_0(step);
	ddr_phy->DDRPHY_PAD_BUS_2 = (ddr_phy->DDRPHY_PAD_BUS_2 & ~DDRPHY_MASK_VREF_S_1) | DDRPHY_VREF_S_1(step);
	//REG_WRITE64_Mask2(protocal_index, (ddr_phy->DDRPHY_PAD_BUS_3), PAD_BUS_3_S2_VREF, PAD_BUS_3_S2_VREF, step);
	//REG_WRITE64_Mask2(protocal_index, (ddr_phy->DDRPHY_PAD_BUS_3), PAD_BUS_3_S3_VREF, PAD_BUS_3_S3_VREF, step);

	//rd_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

	//REG_READ64(ddr_phy->DDRPHY_0x374);
	//REG_READ64(ddr_phy->DDRPHY_0x378);

	DelayMs(1000);
	//REG_WRITE64_Mask2(ddr_phy->DDRPHY_CAL_MODE_CTRL, CAL_MODE_CTRL_FW_VR_UPD_0, 4, 0); // 0: vref_s canNOT be applied to Vref pad by fw_set_rd_dly
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);
}//SOC_DQ_VREF

void rx_delaytap_write(int Ref_Te[])
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2); // fw_set_mode[1:0] = 0x2

	ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] |= DDRPHY_BIT_FW_RD_TE_UPD; //te

	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(Ref_Te[0]);
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(Ref_Te[1]);
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(Ref_Te[2]);
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(Ref_Te[3]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(Ref_Te[4]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(Ref_Te[5]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(Ref_Te[6]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(Ref_Te[7]);

	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(Ref_Te[8]);
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(Ref_Te[9]);
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(Ref_Te[10]);
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(Ref_Te[11]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(Ref_Te[12]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(Ref_Te[13]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(Ref_Te[14]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(Ref_Te[15]);

	// dqsn 0~7
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(Ref_Te[32]);
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1) | DDRPHY_FW_RD_DLY_NEG_SEL_1(Ref_Te[33]);
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2) | DDRPHY_FW_RD_DLY_NEG_SEL_2(Ref_Te[34]);
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3) | DDRPHY_FW_RD_DLY_NEG_SEL_3(Ref_Te[35]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4) | DDRPHY_FW_RD_DLY_NEG_SEL_4(Ref_Te[36]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5) | DDRPHY_FW_RD_DLY_NEG_SEL_5(Ref_Te[37]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6) | DDRPHY_FW_RD_DLY_NEG_SEL_6(Ref_Te[38]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7) | DDRPHY_FW_RD_DLY_NEG_SEL_7(Ref_Te[39]);

	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(Ref_Te[40]);
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1) | DDRPHY_FW_RD_DLY_NEG_SEL_1(Ref_Te[41]);
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2) | DDRPHY_FW_RD_DLY_NEG_SEL_2(Ref_Te[42]);
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3) | DDRPHY_FW_RD_DLY_NEG_SEL_3(Ref_Te[43]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4) | DDRPHY_FW_RD_DLY_NEG_SEL_4(Ref_Te[44]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5) | DDRPHY_FW_RD_DLY_NEG_SEL_5(Ref_Te[45]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6) | DDRPHY_FW_RD_DLY_NEG_SEL_6(Ref_Te[46]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7) | DDRPHY_FW_RD_DLY_NEG_SEL_7(Ref_Te[47]);

	//DBI
	ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
				Ref_Te[64]);
	ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
				Ref_Te[65]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
				Ref_Te[66]);
	ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
				Ref_Te[67]);

	//This is result
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1]);

	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1]);

	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0]);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = 0x%08x;\n", ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1]);

	// fw_rd_dly
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
}//rx_delaytap_write

void rx_dq(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

#define OFFLINE_CAL_EN 0//1: off line delta cal, 0: scan Rx dq per DQ(3 point K off)
#define SCAN_VREF 0

	//int left[72], right[72];
	int TE[72], TE_init[72];
	int i, vref_range, vref_init, vref_step, vref_tune = 3, vref_step_count_up = 2, vref_step_count_down = 2;//
	int vref_step_count_loop;
	int ddr_error_bit;
	float vref[100];

	//fw_set_mode: during no read
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	vref_init = DDRPHY_GET_VREF_S_0(ddr_phy->DDRPHY_PAD_BUS_2);
	vref_range = (ddr_phy->DDRPHY_PAD_BUS_2 & DDRPHY_BIT_VREF_RANGE_0) >> 7;
	if (vref_range == 0) { //range0:38% – 53%, range1:44% - 59%
		vref[vref_init] = vref_init + 38;
	}
	if (vref_range == 1) {
		vref[vref_init] = vref_init + 44;
	}
	RTK_LOGI(TAG,  "vref_init = %d\n", vref_init);
	RTK_LOGI(TAG,  "vref[] = %d\n", (int)vref[vref_init]);
	RTK_LOGI(TAG,  "vref_range = %d\n", vref_range);

	//store default initial TE from hwsetting
	// dqsp
	TE_init[0] = DDRPHY_GET_FW_RD_DLY_POS_SEL_0(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0]);
	TE_init[1] = DDRPHY_GET_FW_RD_DLY_POS_SEL_1(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0]);
	TE_init[2] = DDRPHY_GET_FW_RD_DLY_POS_SEL_2(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0]);
	TE_init[3] = DDRPHY_GET_FW_RD_DLY_POS_SEL_3(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0]);
	TE_init[4] = DDRPHY_GET_FW_RD_DLY_POS_SEL_4(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0]);
	TE_init[5] = DDRPHY_GET_FW_RD_DLY_POS_SEL_5(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0]);
	TE_init[6] = DDRPHY_GET_FW_RD_DLY_POS_SEL_6(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0]);
	TE_init[7] = DDRPHY_GET_FW_RD_DLY_POS_SEL_7(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0]);

	TE_init[8] = DDRPHY_GET_FW_RD_DLY_POS_SEL_0(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1]);
	TE_init[9] = DDRPHY_GET_FW_RD_DLY_POS_SEL_1(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1]);
	TE_init[10] = DDRPHY_GET_FW_RD_DLY_POS_SEL_2(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1]);
	TE_init[11] = DDRPHY_GET_FW_RD_DLY_POS_SEL_3(ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1]);
	TE_init[12] = DDRPHY_GET_FW_RD_DLY_POS_SEL_4(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1]);
	TE_init[13] = DDRPHY_GET_FW_RD_DLY_POS_SEL_5(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1]);
	TE_init[14] = DDRPHY_GET_FW_RD_DLY_POS_SEL_6(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1]);
	TE_init[15] = DDRPHY_GET_FW_RD_DLY_POS_SEL_7(ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1]);

	// dqsn
	TE_init[32] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_0(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0]);
	TE_init[33] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_1(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0]);
	TE_init[34] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_2(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0]);
	TE_init[35] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_3(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0]);
	TE_init[36] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_4(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0]);
	TE_init[37] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_5(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0]);
	TE_init[38] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_6(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0]);
	TE_init[39] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_7(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0]);

	TE_init[40] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_0(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1]);
	TE_init[41] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_1(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1]);
	TE_init[42] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_2(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1]);
	TE_init[43] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_3(ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1]);
	TE_init[44] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_4(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1]);
	TE_init[45] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_5(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1]);
	TE_init[46] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_6(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1]);
	TE_init[47] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_7(ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1]);

	//DBI
	TE_init[64] = DDRPHY_GET_FW_RD_DLY_POS_SEL_8(ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0]);
	TE_init[65] = DDRPHY_GET_FW_RD_DLY_POS_SEL_8(ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1]);
	TE_init[66] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_8(ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0]);
	TE_init[67] = DDRPHY_GET_FW_RD_DLY_NEG_SEL_8(ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1]);


	/*PHY_REG_0x368 = REG_READ64(ddr_phy->DDRPHY_CAL_VREF_CTRL);
	PHY_REG_0x208 = REG_READ64(ddr_phy->DDRPHY_DPI_CTRL_0);//fw_set_mode
	PHY_REG_0x6c8 = REG_READ64(ddr_phy->DDRPHY_READ_CTRL_4_0); //delta
	PHY_REG_0x6cc = REG_READ64(ddr_phy->DDRPHY_READ_CTRL_4_1); //delta
	PHY_REG_0x9c8 = REG_READ64(ddr_phy->DDRPHY_READ_CTRL_4_2); //delta
	PHY_REG_0x9cc = REG_READ64(ddr_phy->DDRPHY_READ_CTRL_4_3); //delta
	PHY_REG_0x620 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_PSEL_0); // rw_blk_01 DQ rising	delta width //P: s0
	PHY_REG_0x624 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_PSEL_1); // rw_blk_01 DQ rising	delta width //P: s1
	PHY_REG_0x628 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_NSEL_0); // rw_blk_01 DQ falling delta width //N: s0
	PHY_REG_0x62c = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_NSEL_1); // rw_blk_01 DQ falling delta width //N: s1
	PHY_REG_0x920 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_PSEL_2); // rw_blk_23 DQ rising	delta width
	PHY_REG_0x924 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_PSEL_3); // rw_blk_23 DQ rising	delta width
	PHY_REG_0x928 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_NSEL_2); // rw_blk_23 DQ falling delta width
	PHY_REG_0x92c = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DQ_DEL_NSEL_3); // rw_blk_23 DQ falling delta width
	PHY_REG_0x640 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DM_DEL_PNSEL_0); // dm0_del_neg_sel_ls/dm0_del_pos_sel_ls/dm0_del_neg_sel/dm0_del_pos_sel
	PHY_REG_0x644 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DM_DEL_PNSEL_1); // rw_blk_1 DM rising/falling delta width
	PHY_REG_0x940 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DM_DEL_PNSEL_2); // rw_blk_2 DM rising/falling delta width
	PHY_REG_0x944 = REG_READ64(protocal_index, (ddr_phy->DDRPHY_DM_DEL_PNSEL_3); // rw_blk_3 DM rising/falling delta width
	PHY_REG_0x36c = REG_READ64(protocal_index, (ddr_phy->DDRPHY_CAL_MODE_CTRL); //fw_delta_force & enable cal_offline
	*/
#if OFFLINE_CAL_EN==0
	RTK_LOGI(TAG,  "~~~~~~~~ Scan Rx DQ ~~~~~~~~\n");
	RTK_LOGI(TAG,  "!!!!! Please add random pattern if 'dbi' function is enable !!!!!\n");

	int TE_iterate[72], TEp_setup[32], TEn_setup[32], TEp_hold[32], TEn_hold[32], TEp_dbi_setup[4], TEn_dbi_setup[4], TEp_dbi_hold[4], TEn_dbi_hold[4];
	//int TE_read[72];//debug

	//int array_pinmux[32]={0,3,2,1,6,5,4,7,
	//						15,8,9,12,11,14,13,10,
	//						17,18,19,16,23,22,21,20,
	//						30,27,26,29,28,25,24,31};//fill the array element with controller name(e.g. PHY DQ1<=>MC DQ3, arr[1]=3 )
	int array_pinmux[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; //fill the array element with controller name(e.g. PHY DQ1<=>MC DQ1, arr[1]=1)

	for (i = 0; i < 72; i++) {
		TE_iterate[i] = TE_init[i];
	}

	//disable 3 point K
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_CAL_SET_MODE)) | DDRPHY_CAL_SET_MODE(0x3);
	//REG_WRITE64_Mask2(ddr_phy->DDRPHY_CAL_MODE_CTRL, CAL_MODE_CTRL_FW_DELTA_FORCE, CAL_MODE_CTRL_FW_DELTA_FORCE, 1); //fw_delta_force=1
	//REG_WRITE64_Mask2(ddr_phy->DDRPHY_CAL_VREF_CTRL, CAL_VREF_CTRL_VR_CNT_TARGET, CAL_VREF_CTRL_VR_CNT_TARGET, 0); // disable vref auto k

	// fw_rd_te_upd = 1
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
	ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] |= DDRPHY_BIT_FW_RD_TE_UPD; //te

	for (int vref_direction = 0; vref_direction <= 1; vref_direction++) { // vref_direction = 0: scan vref upward, vref_firection = 1: scan vref downward
		if (vref_direction == 0) { //scan lower rx vref
			vref_tune *= 1;
			vref_step_count_loop = vref_step_count_up;
		}
		if (vref_direction == 1) {
			vref_tune *= -1;
			vref_step_count_loop = vref_step_count_down;
		}

		for (vref_step = vref_init; vref_step < 64; vref_step += vref_tune) {
			if ((vref_direction == 1) && (vref_step == vref_init)) {
				vref_step_count_loop--;
				continue;
			}

			if (vref_range == 0) { //range0:38% – 53%, range1:44% - 59%
				vref[vref_step] = vref_step + 38;
			}
			if (vref_range == 1) {
				vref[vref_step] = vref_step + 44;
			}
			//DiagPrintf(" vref(loop) = " + FloatToStr(vref[vref_step]) );

			SOC_DQ_VREF(vref_step); //do not support change vref range

			for (i = 0; i < 32; i++) { //initialize the result array
				TEp_setup[i] = 0xff;
				TEn_setup[i] = 0xff;
				TEp_hold[i] = -1;
				TEn_hold[i] = -1;
			}
			for (i = 0; i < 4; i++) {
				TEp_dbi_setup[i] = 0xff;
				TEn_dbi_setup[i] = 0xff;
				TEp_dbi_hold[i] = 0xff;
				TEn_dbi_hold[i] = 0xff;
			}

			for (i = 0; i < 72; i++) { //initialize TE
				//TE_iterate[i]=TE_init[i];
				TE[i] = TE_iterate[i];
				//TE[i]=TE_init[i];
			}

			RTK_LOGI(TAG,  "DQ P setup\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //setup rising
				//fw_set_mode: immediately
				//REG_WRITE64_Mask2(protocal_index, (ddr_phy->DDRPHY_DPI_CTRL_0), DPI_CTRL_0_FW_SET_MODE, DPI_CTRL_0_FW_SET_MODE, 0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(TE[0]); //DQ0p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(TE[1]); //DQ1p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(TE[2]); //DQ2p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(TE[3]); //DQ3p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(TE[4]); //DQ4p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(TE[5]); //DQ5p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(TE[6]); //DQ6p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(TE[7]); //DQ7p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(TE[8]); //DQ8p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(TE[9]); //DQ9p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(TE[10]); //DQ10p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(TE[11]); //DQ11p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(TE[12]); //DQ12p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(TE[13]); //DQ13p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(TE[14]); //DQ14p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(TE[15]); //DQ15p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				for (i = 0; i < 32; i++) {
					if (((ddr_error_bit >> array_pinmux[i]) & 0x1) == 0) {
						TEp_setup[i] = TE[i];
					}
					if (TE[i] == 0)	{
						TE[i] = 0;
					} else	{
						TE[i] = TE[i] - 1;
					}
				}

				if (ddr_error_bit == BIT_LEN_MASK_32(DQ_BIT_NUM)) {
					break;
				}
				if (cnt >= 32) {
					DiagPrintf("!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//scan setup rising

			for (i = 0; i < 32; i++) {
				TE[i] = TE_iterate[i];
			}

			RTK_LOGI(TAG,  "DQ P hold\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //hold rising
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(TE[0]); //DQ0p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(TE[1]); //DQ1p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(TE[2]); //DQ2p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(TE[3]); //DQ3p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(TE[4]); //DQ4p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(TE[5]); //DQ5p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(TE[6]); //DQ6p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(TE[7]); //DQ7p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(TE[8]); //DQ8p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(TE[9]); //DQ9p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(TE[10]); //DQ10p
				ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(TE[11]); //DQ11p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(TE[12]); //DQ12p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(TE[13]); //DQ13p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(TE[14]); //DQ14p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(TE[15]); //DQ15p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				for (i = 0; i < 32; i++) {
					if (((ddr_error_bit >> array_pinmux[i]) & 0x1) == 0) {
						TEp_hold[i] = TE[i];
					}
					if (TE[i] == 31) {
						TE[i] = 31;
					} else	{
						TE[i] = TE[i] + 1;
					}
				}

				if (ddr_error_bit == BIT_LEN_MASK_32(DQ_BIT_NUM)) {
					break;
				}
				if (cnt >= 32) {
					DiagPrintf("!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//scan hold rising

			// initialize dq p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(
					TE_iterate[0]); //DQ0p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(
					TE_iterate[1]); //DQ1p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(
					TE_iterate[2]); //DQ2p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(
					TE_iterate[3]); //DQ3p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(
					TE_iterate[4]); //DQ4p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(
					TE_iterate[5]); //DQ5p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(
					TE_iterate[6]); //DQ6p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(
					TE_iterate[7]); //DQ7p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(
					TE_iterate[8]); //DQ8p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(
					TE_iterate[9]); //DQ9p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(
					TE_iterate[10]); //DQ10p
			ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(
					TE_iterate[11]); //DQ11p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(
					TE_iterate[12]); //DQ12p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(
					TE_iterate[13]); //DQ13p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(
					TE_iterate[14]); //DQ14p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(
					TE_iterate[15]); //DQ15p

			RTK_LOGI(TAG,  "DQ N setup\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //setup falling
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(TE[32]); //DQ0p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1) | DDRPHY_FW_RD_DLY_NEG_SEL_1(TE[33]); //DQ1p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2) | DDRPHY_FW_RD_DLY_NEG_SEL_2(TE[34]); //DQ2p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3) | DDRPHY_FW_RD_DLY_NEG_SEL_3(TE[35]); //DQ3p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4) | DDRPHY_FW_RD_DLY_NEG_SEL_4(TE[36]); //DQ4p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5) | DDRPHY_FW_RD_DLY_NEG_SEL_5(TE[37]); //DQ5p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6) | DDRPHY_FW_RD_DLY_NEG_SEL_6(TE[38]); //DQ6p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7) | DDRPHY_FW_RD_DLY_NEG_SEL_7(TE[39]); //DQ7p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(TE[40]); //DQ8p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1) | DDRPHY_FW_RD_DLY_NEG_SEL_1(TE[41]); //DQ9p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2) | DDRPHY_FW_RD_DLY_NEG_SEL_2(TE[42]); //DQ10p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3) | DDRPHY_FW_RD_DLY_NEG_SEL_3(TE[43]); //DQ11p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4) | DDRPHY_FW_RD_DLY_NEG_SEL_4(TE[44]); //DQ12p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5) | DDRPHY_FW_RD_DLY_NEG_SEL_5(TE[45]); //DQ13p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6) | DDRPHY_FW_RD_DLY_NEG_SEL_6(TE[46]); //DQ14p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7) | DDRPHY_FW_RD_DLY_NEG_SEL_7(TE[47]); //DQ15p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				for (i = 0; i < 32; i++) {
					if (((ddr_error_bit >> array_pinmux[i]) & 0x1) == 0) {
						TEn_setup[i] = TE[i + 32];
					}
					if (TE[i + 32] == 0) {
						TE[i + 32] = 0;
					} else	{
						TE[i + 32] = TE[i + 32] - 1;
					}
				}

				if (ddr_error_bit == BIT_LEN_MASK_32(DQ_BIT_NUM)) {
					break;
				}
				if (cnt >= 32) {
					DiagPrintf("!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//scan setup falling

			for (i = 32; i < 64; i++) {
				TE[i] = TE_iterate[i];
			}

			RTK_LOGI(TAG,  "DQ N hold\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //hold falling
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(TE[32]); //DQ0p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1) | DDRPHY_FW_RD_DLY_NEG_SEL_1(TE[33]); //DQ1p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2) | DDRPHY_FW_RD_DLY_NEG_SEL_2(TE[34]); //DQ2p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3) | DDRPHY_FW_RD_DLY_NEG_SEL_3(TE[35]); //DQ3p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4) | DDRPHY_FW_RD_DLY_NEG_SEL_4(TE[36]); //DQ4p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5) | DDRPHY_FW_RD_DLY_NEG_SEL_5(TE[37]); //DQ5p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6) | DDRPHY_FW_RD_DLY_NEG_SEL_6(TE[38]); //DQ6p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7) | DDRPHY_FW_RD_DLY_NEG_SEL_7(TE[39]); //DQ7p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(TE[40]); //DQ8p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_1) | DDRPHY_FW_RD_DLY_NEG_SEL_1(TE[41]); //DQ9p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_2) | DDRPHY_FW_RD_DLY_NEG_SEL_2(TE[42]); //DQ10p
				ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_3) | DDRPHY_FW_RD_DLY_NEG_SEL_3(TE[43]); //DQ11p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_4) | DDRPHY_FW_RD_DLY_NEG_SEL_4(TE[44]); //DQ12p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_5) | DDRPHY_FW_RD_DLY_NEG_SEL_5(TE[45]); //DQ13p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_6) | DDRPHY_FW_RD_DLY_NEG_SEL_6(TE[46]); //DQ14p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_7) | DDRPHY_FW_RD_DLY_NEG_SEL_7(TE[47]); //DQ15p
				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				for (i = 0; i < 32; i++) {
					if (((ddr_error_bit >> array_pinmux[i]) & 0x1) == 0) {
						TEn_hold[i] = TE[i + 32];
					}
					if (TE[i + 32] == 31)	{
						TE[i + 32] = 31;
					} else	{
						TE[i + 32] = TE[i + 32] + 1;
					}
				}

				if (ddr_error_bit == BIT_LEN_MASK_32(DQ_BIT_NUM)) {
					break;
				}
				if (cnt >= 32) {
					DiagPrintf("!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//scan hold falling
			// initialize dq n
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(
					TE_iterate[32]); //DQ0p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(
					TE_iterate[33]); //DQ1p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(
					TE_iterate[34]); //DQ2p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(
					TE_iterate[35]); //DQ3p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(
					TE_iterate[36]); //DQ4p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(
					TE_iterate[37]); //DQ5p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(
					TE_iterate[38]); //DQ6p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(
					TE_iterate[39]); //DQ7p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(
					TE_iterate[40]); //DQ8p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_1) | DDRPHY_FW_RD_DLY_POS_SEL_1(
					TE_iterate[41]); //DQ9p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_2) | DDRPHY_FW_RD_DLY_POS_SEL_2(
					TE_iterate[42]); //DQ10p
			ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_3) | DDRPHY_FW_RD_DLY_POS_SEL_3(
					TE_iterate[43]); //DQ11p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_4) | DDRPHY_FW_RD_DLY_POS_SEL_4(
					TE_iterate[44]); //DQ12p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_5) | DDRPHY_FW_RD_DLY_POS_SEL_5(
					TE_iterate[45]); //DQ13p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_6) | DDRPHY_FW_RD_DLY_POS_SEL_6(
					TE_iterate[46]); //DQ14p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_7) | DDRPHY_FW_RD_DLY_POS_SEL_7(
					TE_iterate[47]); //DQ15p

			RTK_LOGI(TAG,  "dbi P setup\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //setup rising dbi
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
							TE[64]); //dbi0p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
							TE[65]); //dbi1p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				if (((ddr_error_bit >> 0) & 0xff) == 0) { // slice0 error bit vs dbi0
					TEp_dbi_setup[0] = TE[64];
				}
				if (TE[64] == 0) {
					TE[64] = 0;
				} else	{
					TE[64] = TE[64] - 1;
				}

				if (((ddr_error_bit >> 8) & 0xff) == 0) { // slice1 error bit vs dbi1
					TEp_dbi_setup[1] = TE[65];
				}
				if (TE[65] == 0) {
					TE[65] = 0;
				} else	{
					TE[65] = TE[65] - 1;
				}

				if (((ddr_error_bit & 0xff) != 0) && ((ddr_error_bit & 0xff00) != 0)) {
					break;
				}
				if (cnt >= 32) {
					DiagPrintf("!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//setup rising dbi

			TE[64] = TE_iterate[64];
			TE[65] = TE_iterate[65];

			RTK_LOGI(TAG,  "dbi P hold\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //hold rising dbi
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
							TE[64]); //dbi0p
				ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
							TE[65]); //dbi1p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				if (((ddr_error_bit >> 0) & 0xff) == 0) { // slice0 error bit vs dbi0
					TEp_dbi_hold[0] = TE[64];
				}
				if (TE[64] == 31) {
					TE[64] = 31;
				} else	{
					TE[64] = TE[64] + 1;
				}

				if (((ddr_error_bit >> 8) & 0xff) == 0) { // slice1 error bit vs dbi1
					TEp_dbi_hold[1] = TE[65];
				}
				if (TE[65] == 31) {
					TE[65] = 31;
				} else	{
					TE[65] = TE[65] + 1;
				}

				if (((ddr_error_bit & 0xff) != 0) && ((ddr_error_bit & 0xff00) != 0)) {
					break;
				}
				if (cnt >= 32) {
					RTK_LOGI(TAG,  "!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//hold rising dbi

			ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
						TE_iterate[64]); //dbi0p
			ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_8) | DDRPHY_FW_RD_DLY_POS_SEL_8(
						TE_iterate[65]); //dbi1p

			RTK_LOGI(TAG,  "dbi N setup\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //setup falling dbi
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
				ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
							TE[66]); //dbi0p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
							TE[67]); //dbi1p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				if (((ddr_error_bit >> 0) & 0xff) == 0) { // slice0 error bit vs dbi0
					TEn_dbi_setup[0] = TE[66];
				}
				if (TE[66] == 0) {
					TE[66] = 0;
				} else	{
					TE[66] = TE[66] - 1;
				}

				if (((ddr_error_bit >> 8) & 0xff) == 0) { // slice1 error bit vs dbi1
					TEn_dbi_setup[1] = TE[67];
				}
				if (TE[67] == 0) {
					TE[67] = 0;
				} else	{
					TE[67] = TE[67] - 1;
				}

				if (((ddr_error_bit & 0xff) != 0) && ((ddr_error_bit & 0xff00) != 0)) {
					break;
				}
				if (cnt >= 32) {
					DiagPrintf("!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//setup falling dbi

			TE[66] = TE_iterate[66];
			TE[67] = TE_iterate[67];

			RTK_LOGI(TAG,  "dbi n hold\n");
			for (int cnt = 1; cnt <= 32; cnt++) { //hold falling dbi
				//fw_set_mode: immediately
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);

				ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
							TE[66]); //dbi0p
				ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
							TE[67]); //dbi1p

				//wr_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
				//rd_delay
				ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;
				//fw_set_mode: no read
				//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

				sram_sg();
				ddr_error_bit = sram_sg_chk();//r/w result

				if (((ddr_error_bit >> 0) & 0xff) == 0) { // slice0 error bit vs dbi0
					TEn_dbi_hold[0] = TE[66];
				}
				if (TE[66] == 31) {
					TE[66] = 31;
				} else	{
					TE[66] = TE[66] + 1;
				}

				if (((ddr_error_bit >> 8) & 0xff) == 0) { // slice1 error bit vs dbi1
					TEn_dbi_hold[1] = TE[67];
				}
				if (TE[67] == 31) {
					TE[67] = 31;
				} else	{
					TE[67] = TE[67] + 1;
				}

				if (((ddr_error_bit & 0xff) != 0) && ((ddr_error_bit & 0xff00) != 0)) {
					break;
				}
				if (cnt >= 32) {
					RTK_LOGI(TAG,  "!!!!!!!! TE shift FAIL !!!!!!!!\n");
					break;
				}
			}//hold falling dbi

			ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
						TE_iterate[66]); //dbi0p
			ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = (ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_8) | DDRPHY_FW_RD_DLY_NEG_SEL_8(
						TE_iterate[67]); //dbi1p
			//wr_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0;
			//rd_delay
			ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

			//print result
			/*for (int i = 0; i < DQ_BIT_NUM; i++)
					RTK_LOGI(TAG,  "%d, DQ%dp: setup = %d, hold = %d, map = %d, center = %d\n", ((int)vref[vref_step]), (i), (TEp_setup[i]), (TEp_hold[i]), (TEp_hold[i]-TEp_setup[i]+1), ((int)((TEp_setup[i]+TEp_hold[i])/2)));
			for (int i = 0; i < DQ_BIT_NUM; i++)
					RTK_LOGI(TAG,  "%d, DQ%dn: setup = %d, hold = %d, map = %d, center = %d\n", ((int)vref[vref_step]), (i), (TEn_setup[i]), (TEn_hold[i]), (TEn_hold[i]-TEn_setup[i]+1), ((int)((TEn_setup[i]+TEn_hold[i])/2)));
			for (int i = 0; i < 2; i++)
					RTK_LOGI(TAG,  "%d, dbi%dp: setup = %d, hold = %d, map = %d, center = %d\n", ((int)vref[vref_step]), (i), (TEp_dbi_setup[i]), (TEp_dbi_hold[i]), (TEp_dbi_hold[i]-TEp_dbi_setup[i]+1), ((int)((TEp_dbi_setup[i]+TEp_dbi_hold[i])/2)));
			for (int i = 0; i < 2; i++)
					RTK_LOGI(TAG,  "%d, dbi%dn: setup = %d, hold = %d, map = %d, center = %d\n", ((int)vref[vref_step]), (i), (TEn_dbi_setup[i]), (TEn_dbi_hold[i]), (TEn_dbi_hold[i]-TEn_dbi_setup[i]+1), ((int)((TEn_dbi_setup[i]+TEn_dbi_hold[i])/2)));
			*/

			// DDR tool input format
			//if delta(L,R) = -1/0 map=2tap, delta(L,R) = -1/-1 map=1tap
			//ddr_error_bit = REG_READ64(protocal_index, BSP_MS_I_DRAMC_0_BASE + 0x0e8);
			ddr_error_bit = 0;

			RTK_LOGI(TAG,  "%d, DM1, %d, %d, %d, %d, %d, %d, 0x%08x\n", ((int)vref[vref_step]), ((int)((TEp_dbi_setup[0] + TEp_dbi_hold[0]) / 2)),
					 ((int)((TEp_dbi_setup[0] + TEp_dbi_hold[0]) / 2) - TEp_dbi_setup[0] - 1), (TEp_dbi_hold[0] - (int)((TEp_dbi_setup[0] + TEp_dbi_hold[0]) / 2) - 1),
					 ((int)((TEn_dbi_setup[0] + TEn_dbi_hold[0]) / 2)), ((int)((TEn_dbi_setup[0] + TEn_dbi_hold[0]) / 2) - TEn_dbi_setup[0] - 1),
					 (TEn_dbi_hold[0] - (int)((TEn_dbi_setup[0] + TEn_dbi_hold[0]) / 2) - 1), ddr_error_bit);


			RTK_LOGI(TAG,  "%d, DM1, %d, %d, %d, %d, %d, %d, 0x%08x\n", ((int)vref[vref_step]), ((int)((TEp_dbi_setup[1] + TEp_dbi_hold[1]) / 2)),
					 ((int)((TEp_dbi_setup[1] + TEp_dbi_hold[1]) / 2) - TEp_dbi_setup[1] - 1), (TEp_dbi_hold[1] - (int)((TEp_dbi_setup[1] + TEp_dbi_hold[1]) / 2) - 1),
					 ((int)((TEn_dbi_setup[1] + TEn_dbi_hold[1]) / 2)), ((int)((TEn_dbi_setup[1] + TEn_dbi_hold[1]) / 2) - TEn_dbi_setup[1] - 1),
					 (TEn_dbi_hold[1] - (int)((TEn_dbi_setup[1] + TEn_dbi_hold[1]) / 2) - 1), ddr_error_bit);

			RTK_LOGI(TAG,  "%d, DM2, %d, %d, %d, %d, %d, %d, 0x%08x\n", ((int)vref[vref_step]), ((int)((TEp_dbi_setup[2] + TEp_dbi_hold[2]) / 2)),
					 ((int)((TEp_dbi_setup[2] + TEp_dbi_hold[2]) / 2) - TEp_dbi_setup[2] - 1), (TEp_dbi_hold[2] - (int)((TEp_dbi_setup[2] + TEp_dbi_hold[2]) / 2) - 1),
					 ((int)((TEn_dbi_setup[2] + TEn_dbi_hold[2]) / 2)), ((int)((TEn_dbi_setup[2] + TEn_dbi_hold[2]) / 2) - TEn_dbi_setup[2] - 1),
					 (TEn_dbi_hold[2] - (int)((TEn_dbi_setup[2] + TEn_dbi_hold[2]) / 2) - 1), ddr_error_bit);

			RTK_LOGI(TAG,  "%d, DM3, %d, %d, %d, %d, %d, %d, 0x%08x\n", ((int)vref[vref_step]), ((int)((TEp_dbi_setup[3] + TEp_dbi_hold[3]) / 2)),
					 ((int)((TEp_dbi_setup[3] + TEp_dbi_hold[3]) / 2) - TEp_dbi_setup[3] - 1), (TEp_dbi_hold[3] - (int)((TEp_dbi_setup[3] + TEp_dbi_hold[3]) / 2) - 1),
					 ((int)((TEn_dbi_setup[3] + TEn_dbi_hold[3]) / 2)), ((int)((TEn_dbi_setup[3] + TEn_dbi_hold[3]) / 2) - TEn_dbi_setup[3] - 1),
					 (TEn_dbi_hold[3] - (int)((TEn_dbi_setup[3] + TEn_dbi_hold[3]) / 2) - 1), ddr_error_bit);


			for (int i = 0; i < DQ_BIT_NUM; i++)
				DiagPrintf("%d, DQ, %d, %d, %d, %d, %d, %d, %d, 0x%08x\n", ((int)vref[vref_step]), (i), ((int)((TEp_setup[i] + TEp_hold[i]) / 2)),
						   ((int)((TEp_setup[i] + TEp_hold[i]) / 2) - TEp_setup[i] - 1), (TEp_hold[i] - (int)((TEp_setup[i] + TEp_hold[i]) / 2) - 1),
						   ((int)((TEn_setup[i] + TEn_hold[i]) / 2)), ((int)((TEn_setup[i] + TEn_hold[i]) / 2) - TEn_setup[i] - 1),
						   (TEn_hold[i] - (int)((TEn_setup[i] + TEn_hold[i]) / 2) - 1), ddr_error_bit);

			//set TE_iterate with new TE
			for (int i = 0; i < DQ_BIT_NUM; i++) {
				TE_iterate[i] = (int)((TEp_setup[i] + TEp_hold[i]) / 2);
				TE_iterate[i + 32] = (int)((TEn_setup[i] + TEn_hold[i]) / 2);

			}

			for (int i = 0; i < DQ_BIT_NUM; i++) {
				DiagPrintf("%d: [%d, %d]=>%d\n", i, TEp_setup[i], TEp_hold[i], TE_iterate[i]);
			}
			for (int i = 0; i < DQ_BIT_NUM; i++) {
				DiagPrintf("%d: [%d, %d]=>%d\n", i + 32, TEn_setup[i], TEn_hold[i], TE_iterate[i + 32]);
			}

			TE_iterate[64] = (int)((TEp_dbi_setup[0] + TEp_dbi_hold[0]) / 2); //dbi0 p
			TE_iterate[65] = (int)((TEp_dbi_setup[1] + TEp_dbi_hold[1]) / 2); //dbi1 p
			TE_iterate[68] = (int)((TEp_dbi_setup[2] + TEp_dbi_hold[2]) / 2); //dbi2 p
			TE_iterate[69] = (int)((TEp_dbi_setup[3] + TEp_dbi_hold[3]) / 2); //dbi3 p
			TE_iterate[66] = (int)((TEn_dbi_setup[0] + TEn_dbi_hold[0]) / 2); //dbi0 n
			TE_iterate[67] = (int)((TEn_dbi_setup[1] + TEn_dbi_hold[1]) / 2); //dbi1 n
			TE_iterate[70] = (int)((TEn_dbi_setup[2] + TEn_dbi_hold[2]) / 2); //dbi2 n
			TE_iterate[71] = (int)((TEn_dbi_setup[3] + TEn_dbi_hold[3]) / 2); //dbi3 n
			rx_delaytap_write(TE_iterate);
			ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
			ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] |= DDRPHY_BIT_FW_RD_TE_UPD; //te

			//debug
			/*cal_rd_dly_out(TE_read);
			cal_delta_out_left(left);
			cal_delta_out_right(right);
			//dbi
			RTK_LOGI(TAG,  "dbi0 : Te_p = "+IntToStr(TE_read[64])+", left_p =  "+IntToStr(left[64])+", right_p = "+IntToStr(right[64])+" / Te_n = "+IntToStr(TE_read[66])+", left_n =  "+IntToStr(left[66])+", right_n = "+IntToStr(right[66]) );
			RTK_LOGI(TAG,  "dbi1 : Te_p = "+IntToStr(TE_read[65])+", left_p =  "+IntToStr(left[65])+", right_p = "+IntToStr(right[65])+" / Te_n = "+IntToStr(TE_read[67])+", left_n =  "+IntToStr(left[67])+", right_n = "+IntToStr(right[67]) );
			RTK_LOGI(TAG,  "dbi2 : Te_p = "+IntToStr(TE_read[68])+", left_p =  "+IntToStr(left[68])+", right_p = "+IntToStr(right[68])+" / Te_n = "+IntToStr(TE_read[70])+", left_n =  "+IntToStr(left[70])+", right_n = "+IntToStr(right[70]) );
			RTK_LOGI(TAG,  "dbi3 : Te_p = "+IntToStr(TE_read[69])+", left_p =  "+IntToStr(left[69])+", right_p = "+IntToStr(right[69])+" / Te_n = "+IntToStr(TE_read[71])+", left_n =  "+IntToStr(left[71])+", right_n = "+IntToStr(right[71]) );
			for (int i = 0; i < 32; i++)//dq
			{
					DiagPrintf("dq"+IntToStr(i)+" : Te_p = "+IntToStr(TE_read[i])+", left_p =  "+IntToStr(left[i])+", right_p = "+IntToStr(right[i])+" / Te_n = "+IntToStr(TE_read[i+32])+", left_n =  "+IntToStr(left[i+32])+", right_n = "+IntToStr(right[i+32]) );
					//DiagPrintf("dq%d : Te = %d, left =  %d, right = %d\n", i, rx_dq[i], left[i], right[i]);
			}*/

#if !SCAN_VREF
			RTK_LOGI(TAG,  "Do not scan tx vref\n");
			break;
#endif

			vref_step_count_loop--;
			if (vref_step_count_loop < 0) {
				break;
			}
		}//vref step

#if !SCAN_VREF
		break;
#endif
		if (vref_direction == 0) {
			//set TE init
			for (int i = 0; i < 72; i++) {
				TE_iterate[i] = TE_init[i];
			}
			rx_delaytap_write(TE_init);
			ddr_phy->DDRPHY_READ_CTRL_4_SETx[0] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
			ddr_phy->DDRPHY_READ_CTRL_4_SETx[1] |= DDRPHY_BIT_FW_RD_TE_UPD; //te
		}
	}//scan vref direction

	// 3 point K
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_CAL_SET_MODE) | DDRPHY_CAL_SET_MODE(0x0);
#endif
}//rx dq

void scan_rxfifo(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	int fifo, fifo_init, fifo_min, fifo_max = 0;
	int ddr_error_bit;

	//fw_set_mode: during no read
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x0);

	//store the initial value of tm_rd_fifo
	fifo_init =	DDRPHY_GET_TM_RD_FIFO(ddr_phy->DDRPHY_READ_CTRL_1);
	fifo_min = fifo_init;

	//disable 3 point K
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_CAL_SET_MODE) | DDRPHY_CAL_SET_MODE(0x3);

	for (fifo = fifo_init; fifo < 64; fifo++) { //scan tm_rd_fifo (->)
		DiagPrintf(" tm_rd_fifo = %d\n", fifo);
		//set tm_td_fifo
		ddr_phy->DDRPHY_READ_CTRL_1 = (ddr_phy->DDRPHY_READ_CTRL_1 & ~DDRPHY_MASK_TM_RD_FIFO) | DDRPHY_TM_RD_FIFO(fifo);
		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//rd_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

		//rw
		sram_sg();
		ddr_error_bit = sram_sg_chk();//r/w result
		//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

		//reset fifo pointer
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(0x3);
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(0x2);
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(0x1);

		if (ddr_error_bit != 0) {
			break;
		}

		fifo_max = fifo;
	}

	//disable 3 point K after reset
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_CAL_SET_MODE) | DDRPHY_CAL_SET_MODE(0x3);

	for (fifo = fifo_init - 1; fifo > 0; fifo--) { //scan tm_rd_fifo (<-)
		DiagPrintf(" tm_rd_fifo = %d\n", fifo);
		//set tm_td_fifo
		ddr_phy->DDRPHY_READ_CTRL_1 = (ddr_phy->DDRPHY_READ_CTRL_1 & ~DDRPHY_MASK_TM_RD_FIFO) | DDRPHY_TM_RD_FIFO(fifo);
		//fw_set_mode: immediately
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
		//rd_delay
		ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

		//rw
		sram_sg();
		ddr_error_bit = sram_sg_chk();//r/w result
		//ddr_phy->DDRPHY_CRT_RST_CTL &= ~(DDRPHY_BIT_RST_N | DDRPHY_BIT_PTR_RST_N); //PHY RESET

		//reset fifo pointer
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(0x3);
		//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(0x2);
		ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(0x1);

		if (ddr_error_bit != 0) {
			break;
		}

		if ((fifo_min > fifo) && (ddr_error_bit == 0)) {
			fifo_min = fifo;
		}
	}

	//calculate r/w pass map，set fifo to "min+1"
	RTK_LOGI(TAG,  "  tm_rd_fifo = %d ~ %d\n", fifo_min, fifo_max);

	//set at fifo_min+1
	ddr_phy->DDRPHY_READ_CTRL_1 = (ddr_phy->DDRPHY_READ_CTRL_1 & ~DDRPHY_MASK_TM_RD_FIFO) | DDRPHY_TM_RD_FIFO(fifo_min + 1);
	//fw_set_mode: immediately
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//rd_delay
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1;

	//print final setting of fifo
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_READ_CTRL_1 = 0x%08x;\n", ddr_phy->DDRPHY_READ_CTRL_1);
	//fifo = DDRPHY_GET_TM_RD_FIFO(ddr_phy->DDRPHY_READ_CTRL_1);
	//printf("fifo=%d\n", fifo);
}//scan_rxfifo

u32 _tx_ck_pi_cs_shift(int d_ck_cs, int idx)
{
	int temp, flag, cmd_ex_rd_str_num, oesync_op_sel;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	if (d_ck_cs < 0) {
		flag = -1;
		temp = d_ck_cs + TEQUALTOPI;
		ddr_phy->DDRPHY_AFIFO_STR_SEL |= BIT(28 + idx);
	} else if (d_ck_cs >= TEQUALTOPI) {
		flag = 1;
		temp = d_ck_cs - TEQUALTOPI;
		ddr_phy->DDRPHY_AFIFO_STR_SEL |= BIT(28 + idx);
	} else {
		flag = 0;
		temp = d_ck_cs;
	}
	/*cmd_ex_rd_str_num can only be modified once*/
	if (idx == 0 && flag != 0) {
		cmd_ex_rd_str_num = DDRPHY_GET_CMD_EX_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1);
		cmd_ex_rd_str_num += flag;
		if (cmd_ex_rd_str_num < 1) {
			RTK_LOGI(TAG,  "let all rd_str_num + 1 in AFIFO_STR_0/1/2\n");
		}
		ddr_phy->DDRPHY_AFIFO_STR_1 = (ddr_phy->DDRPHY_AFIFO_STR_1 & ~DDRPHY_MASK_CMD_EX_RD_STR_NUM) | DDRPHY_CMD_EX_RD_STR_NUM(cmd_ex_rd_str_num);
		/*clk shift-1 is equal to right shift+31, but cs shift-1 just shift-1, because mck_cke_sel is set to let mck_cke = mck_cs, so we shall set bit26*/
		ddr_phy->DDRPHY_AFIFO_STR_SEL |= BIT(26);
	}

	oesync_op_sel = 10 + idx; //op_sel[10]:mck_cs
	if (temp >= TEQUALTOPI / 2) {
		ddr_phy->DDRPHY_PLL_CTL1 |= DDRPHY_DPI_OESYNC_OP_SEL(BIT(oesync_op_sel));
	} else {
		ddr_phy->DDRPHY_PLL_CTL1 &= ~DDRPHY_DPI_OESYNC_OP_SEL(BIT(oesync_op_sel));
	}
	return temp;
}

u32 _tx_ck_pi_dq_shift(int d_ck_dq, int idx)
{
	int temp, flag, rd_str_num, oesync_op_sel;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	if (d_ck_dq < 0) {
		flag = -1;
		temp = d_ck_dq + TEQUALTOPI;
	} else if (d_ck_dq >= TEQUALTOPI) {
		flag = +1;
		temp = d_ck_dq - TEQUALTOPI;
	} else {
		flag = 0;
		temp = d_ck_dq;
	}

	if (flag != 0) {
		rd_str_num = ((ddr_phy->DDRPHY_AFIFO_STR_0 >> idx) & 0x7);
		rd_str_num += flag;
		if (rd_str_num < 2) {
			//According to post-sim, TX FIFO rd_str_num should >=2 #by project
			RTK_LOGI(TAG,  "=====================Confirm to RDC PHY\n");
		}
		ddr_phy->DDRPHY_AFIFO_STR_0 = (ddr_phy->DDRPHY_AFIFO_STR_0 & ~(0x7 << idx)) | ((rd_str_num & 0x7) << idx);
	}

	if (idx % 8 == 4) {
		oesync_op_sel = 2 + (idx - 4) / 8; //op_sel[2]:mck_dqs_0
	} else {
		oesync_op_sel = 6 + (idx) / 8; //op_sel[6]:mck_dq_0
	}

	if (temp >= TEQUALTOPI / 2) {
		ddr_phy->DDRPHY_PLL_CTL1 |= DDRPHY_DPI_OESYNC_OP_SEL(BIT(oesync_op_sel));
	} else {
		ddr_phy->DDRPHY_PLL_CTL1 &= ~DDRPHY_DPI_OESYNC_OP_SEL(BIT(oesync_op_sel));
	}

	return temp;
}

static void _tx_cs_pi_middle_value(int cspi_shift)
{
	int cspi, temp;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	cspi = DDRPHY_GET_DPI_POST_PI_SEL10(ddr_phy->DDRPHY_PLL_PI2);
	temp = _tx_ck_pi_cs_shift(cspi + cspi_shift, 0);
	ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL10) | DDRPHY_DPI_POST_PI_SEL10(temp);

	/*acturally no need because AmebaD2 only has mck_cs_0*/
	cspi = DDRPHY_GET_DPI_POST_PI_SEL11(ddr_phy->DDRPHY_PLL_PI3);
	temp = _tx_ck_pi_cs_shift(cspi + cspi_shift, 1);
	ddr_phy->DDRPHY_PLL_PI3 = (ddr_phy->DDRPHY_PLL_PI3 & ~DDRPHY_MASK_DPI_POST_PI_SEL11) | DDRPHY_DPI_POST_PI_SEL11(temp);
}

static void _tx_dqs_pi_middle_value(int dqs0pi_shift, int dqs1pi_shift)
{
	int dqspi, temp;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	dqspi = DDRPHY_GET_DPI_POST_PI_SEL2(ddr_phy->DDRPHY_PLL_PI0);
	temp = _tx_ck_pi_dq_shift(dqspi + dqs0pi_shift, 4);
	ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL2) | DDRPHY_DPI_POST_PI_SEL2(temp);

	dqspi = DDRPHY_GET_DPI_POST_PI_SEL3(ddr_phy->DDRPHY_PLL_PI0);
	temp = _tx_ck_pi_dq_shift(dqspi + dqs1pi_shift, 12);
	ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~DDRPHY_MASK_DPI_POST_PI_SEL3) | DDRPHY_DPI_POST_PI_SEL3(temp);

	dqspi = DDRPHY_GET_DPI_POST_PI_SEL4(ddr_phy->DDRPHY_PLL_PI1);
	temp = _tx_ck_pi_dq_shift(dqspi + dqs1pi_shift, 20);
	ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL4) | DDRPHY_DPI_POST_PI_SEL4(temp);

	dqspi = DDRPHY_GET_DPI_POST_PI_SEL5(ddr_phy->DDRPHY_PLL_PI1);
	temp = _tx_ck_pi_dq_shift(dqspi + dqs1pi_shift, 28);
	ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL5) | DDRPHY_DPI_POST_PI_SEL5(temp);
}

static void _tx_dq_pi_middle_value(int dq0pi_shift, int dq1pi_shift)
{
	int dqpi, temp;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	dqpi = DDRPHY_GET_DPI_POST_PI_SEL6(ddr_phy->DDRPHY_PLL_PI1);
	temp = _tx_ck_pi_dq_shift(dqpi + dq0pi_shift, 0);
	ddr_phy->DDRPHY_PLL_PI1 = (ddr_phy->DDRPHY_PLL_PI1 & ~DDRPHY_MASK_DPI_POST_PI_SEL6) | DDRPHY_DPI_POST_PI_SEL6(temp);

	dqpi = DDRPHY_GET_DPI_POST_PI_SEL7(ddr_phy->DDRPHY_PLL_PI2);
	temp = _tx_ck_pi_dq_shift(dqpi + dq1pi_shift, 8);
	ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL7) | DDRPHY_DPI_POST_PI_SEL7(temp);
	dqpi = DDRPHY_GET_DPI_POST_PI_SEL8(ddr_phy->DDRPHY_PLL_PI2);

	temp = _tx_ck_pi_dq_shift(dqpi + dq1pi_shift, 16);
	ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL8) | DDRPHY_DPI_POST_PI_SEL8(temp);

	dqpi = DDRPHY_GET_DPI_POST_PI_SEL9(ddr_phy->DDRPHY_PLL_PI2);
	temp = _tx_ck_pi_dq_shift(dqpi + dq1pi_shift, 24);
	ddr_phy->DDRPHY_PLL_PI2 = (ddr_phy->DDRPHY_PLL_PI2 & ~DDRPHY_MASK_DPI_POST_PI_SEL9) | DDRPHY_DPI_POST_PI_SEL9(temp);
}

static void _tx_ck_pi_middle_value(int ckpi_shift)
{
	int ckpi, temp;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ckpi = DDRPHY_GET_DPI_POST_PI_SEL0(ddr_phy->DDRPHY_PLL_PI0);//CK_0

	if (ckpi + ckpi_shift < 0) {
		temp = ckpi + ckpi_shift + TEQUALTOPI;
	} else if (ckpi + ckpi_shift >= TEQUALTOPI) {
		temp =  ckpi + ckpi_shift - TEQUALTOPI;
	} else {
		temp =  ckpi + ckpi_shift;
	}

	if (temp >= TEQUALTOPI / 2) {
		ddr_phy->DDRPHY_PLL_CTL1 |= DDRPHY_DPI_OESYNC_OP_SEL(BIT(0));
	} else {
		ddr_phy->DDRPHY_PLL_CTL1 &= ~DDRPHY_DPI_OESYNC_OP_SEL(BIT(0));
	}

	ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & ~(DDRPHY_MASK_DPI_POST_PI_SEL0)) | DDRPHY_DPI_POST_PI_SEL0(temp);

	_tx_cs_pi_middle_value(ckpi_shift);
	_tx_dqs_pi_middle_value(ckpi_shift, ckpi_shift);
	_tx_dq_pi_middle_value(ckpi_shift, ckpi_shift);
}

static void _rw_wrlvl_pi_middle_value(int rwlvl0pi_shift, int rwlvl1pi_shift)
{
	_tx_dqs_pi_middle_value(rwlvl0pi_shift, rwlvl1pi_shift);
	_tx_dq_pi_middle_value(rwlvl0pi_shift, rwlvl1pi_shift);
}

void DDR_PHY_Scan_Result_Set(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	_rw_wrlvl_pi_middle_value(0, 0);//Tx MR write leveling result
	_tx_ck_pi_middle_value(0);		//Tx ck pi result
	_rw_wrlvl_pi_middle_value(0, 0);//Tx rw write leveling result

	_tx_cs_pi_middle_value(0);		//Tx cs pi result
	_tx_dqs_pi_middle_value(0, 0);	//Tx dqs pi result

	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_PLL_PI0 = 0x%08x;\n", ddr_phy->DDRPHY_PLL_PI0);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_PLL_PI1 = 0x%08x;\n", ddr_phy->DDRPHY_PLL_PI1);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_PLL_PI2 = 0x%08x;\n", ddr_phy->DDRPHY_PLL_PI2);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_PLL_PI3 = 0x%08x;\n", ddr_phy->DDRPHY_PLL_PI3);

	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_PLL_CTL1 = 0x%08x;\n", ddr_phy->DDRPHY_PLL_CTL1);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_AFIFO_STR_0 = 0x%08x;\n", ddr_phy->DDRPHY_AFIFO_STR_0);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_AFIFO_STR_1 = 0x%08x;\n", ddr_phy->DDRPHY_AFIFO_STR_1);
	RTK_LOGI(TAG,  "ddr_phy->DDRPHY_AFIFO_STR_SEL = 0x%08x;\n", ddr_phy->DDRPHY_AFIFO_STR_SEL);
}

static u8 DDR_PHY_Scan_param_Stage(void)
{
	u8 scan_stage;

	RTK_LOGI(TAG,  "\nFor ddr Calibration. Please select scan stage:\n");
	RTK_LOGI(TAG,  "1 - TX_MR_WRLVL(DDR3 Only)\n");
	RTK_LOGI(TAG,  "2 - TX_CK_PI\n");
	RTK_LOGI(TAG,  "3 - TX_RW_WRLVL\n");
	RTK_LOGI(TAG,  "4 - TX_CS_PI\n");
	RTK_LOGI(TAG,  "5 - TX_DQS_PI\n");
	RTK_LOGI(TAG,  "6 - RX_SCAN\n");

	LOGUART_ClearRxFifo(LOGUART_DEV);
	LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, DISABLE);

	while (!(LOGUART_Readable()));
	scan_stage = LOGUART_GetChar(_FALSE) - '0';

	LOGUART_ClearRxFifo(LOGUART_DEV);
	LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, ENABLE);
	RTK_LOGI(TAG,  "Input Num is %d\n", scan_stage);

	return scan_stage;
}

void DDR_PHY_Scan_param(void)
{
	u8 scan_stage = DDR_PHY_Scan_param_Stage();

	//when ddrphy init, do R480/ZQ K first
	DDR_PHY_BSTC_STARK();
	sram_sg();
	RTK_LOGI(TAG,  "sram_sg_chk: %d\n", sram_sg_chk());

	if (scan_stage == TX_MR_WRLVL) {
		/*1. MR_wrlvl, DDR2 Not Support, DDR3 shall do this*/
		if (DDR_PHY_ChipInfo_ddrtype() != DDR_Type_DDR2) {
			tx_MR_wrlvl();
		} else {
			RTK_LOGE(TAG, "DDR2 Not Support MR_wrlvl\r\n");
		}
	} else if (scan_stage == TX_CK_PI) {
		/*2. ckpi, move ck/cs/dqs/dq to scan cmd signal map*/
		//remember modify TX_SCAN_SETUP_MAP
		tx_ckpi();
	} else if (scan_stage == TX_RW_WRLVL) {
		/*3. rw wrlvl, move dqs/dq to scan dqs map*/
		tx_rw_wrlvl();
	} else if (scan_stage == TX_CS_PI) {
		/*4. cspi, move cs to scan cs map*/
		//remember modify TX_SCAN_SETUP_MAP
		tx_cspi();
	} else if (scan_stage == TX_DQS_PI) {
		/*5. dqpi, move dq to scan dq slice map*/
		tx_dqs();
	} else if (scan_stage == RX_SCAN) {
		/*6. dqsen, DQS Gating Training, find dqs preamble*/
		rx_dqsen();
		/*7. dq delay tap, 3 point Calibration*/
		rx_dq();
		/*8. rx fifo scan*/
		scan_rxfifo();
	}
}
#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/
