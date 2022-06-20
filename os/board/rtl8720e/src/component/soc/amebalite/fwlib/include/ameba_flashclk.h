/**
  ******************************************************************************
  * @file    ameba_flashclk.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the Flash Clock firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_FLASH_CLK_H
#define _AMEBA_FLASH_CLK_H

/* Other definations --------------------------------------------------------*/
typedef enum {
	FlashClass1 = 0,
	FlashClass2,
	FlashClass3,
	FlashClass4,
	FlashClass5,
	FlashClass6,
	FlashClassUser = 0xFE,
	FlashClassNone = 0xFF,
} FlashClass;

typedef struct {
	u32 flash_id;
	u32 id_mask;
	u8 flash_class;
	u32 sta_mask;
	VOID (*FlashInitHandler)(VOID);
} FlashInfo_TypeDef;

typedef enum {
	FLASH_CLK_DIV2 = 1,
	FLASH_CLK_DIV3,
	FLASH_CLK_DIV4,
	FLASH_CLK_DIV5,
	FLASH_CLK_DIV6,
	FLASH_CLK_DIV7,
	FLASH_CLK_DIV8,
	FLASH_CLK_DIV9,
	FLASH_CLK_DIV10,
} FlashDivInt_E;

typedef struct {
	u8 phase_int;
	u8 phase_frac;
} FLASH_CLK_Phase;

typedef struct {
	u8 div_int;
	u8 div_frac;
} FLASH_CLK_Div;

#define FLASH_SPICCLK_TO_PLLCLK(Div)			((Div) + 1)		/*Calculate 1 SPIC CLK to N PLL CLK*/
#define FLASH_PLLCLK_EQUAL_PAHSE				8				/*Divide 1 PLL CLK to 8 Phase, So 1 SPIC CLK equal to FLASHDIVINT_TO_PLL_CLK(Div) * 8*/
#define FLASH_SHIFT_IDX_TO_PLLCLK(shift_idx)	((shift_idx) / FLASH_PLLCLK_EQUAL_PAHSE)	/*phase_int means how many PLL CLK is shift*/
#define FLASH_SHIFT_IDX_TO_PAHSE(shift_idx)		((shift_idx) % FLASH_PLLCLK_EQUAL_PAHSE)	/*phase_frac means how many phase shift in one PLL CLK*/

void flash_highspeed_setup(void);
#endif  //_AMEBA_FLASH_CLK_H
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
