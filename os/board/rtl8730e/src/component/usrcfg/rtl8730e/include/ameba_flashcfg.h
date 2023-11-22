/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _AMEBA_FLASHCFG_H_
#define _AMEBA_FLASHCFG_H_

extern const u16 Flash_Speed;
extern const u16 Flash_ReadMode;

extern u32 IMG_ADDR[][2];

extern const FlashInfo_TypeDef Flash_AVL[];
extern const FlashInfo_TypeDef NAND_AVL[];

extern FlashLayoutInfo_TypeDef Flash_Layout_Nor[];
extern FlashLayoutInfo_TypeDef Flash_Layout_Nand[];
extern FlashLayoutInfo_TypeDef Flash_Layout_Nor_Linux[];
extern FlashLayoutInfo_TypeDef Flash_Layout_Nand_Linux[];

//================functions=================
void flash_init_userdef(void);
void nand_init_userdef(void);
void flash_get_layout_info(u32 type, u32 *start, u32 *end);

#endif
