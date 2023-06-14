/**
  ******************************************************************************
  * @file    ameba_nandflash.h
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

#ifndef _AMEBA_NANDFLASH_H
#define _AMEBA_NANDFLASH_H

/* Other definations --------------------------------------------------------*/
#define NAND_L2P_TABLE_MARK_BAD				0x2
#define NAND_L2P_TABLE_MARK_GOOD			0x1
/*each block id occupy two bits to decide block status: 0b00 is not check, 0b01 is good, 0b10 is bad.*/
#define NAND_L2P_TABLE_SET_BAD(block_id)	(nand_l2p_table[(block_id) >> 4] |= (NAND_L2P_TABLE_MARK_BAD << (((block_id) & 0xF) << 1)))
#define NAND_L2P_TABLE_SET_GOOD(block_id)	(nand_l2p_table[(block_id) >> 4] |= (NAND_L2P_TABLE_MARK_GOOD << (((block_id) & 0xF) << 1)))
#define NAND_L2P_TABLE_GET(block_id)		((nand_l2p_table[(block_id) >> 4] >> (((block_id) & 0xF) << 1)) & 0x3)

void NandImgCopy(u8 *pData, u8 *HostAddr, size_t DataLen);

#endif  //_AMEBA_FLASH_CLK_H
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
