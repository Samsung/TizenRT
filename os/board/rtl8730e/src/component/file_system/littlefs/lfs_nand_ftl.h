/**
  ******************************************************************************
  * @file    floader_nand_ftl.h
  * @author  Realsil WLAN5 Team
  * @date    2021-01-27
  * @brief   This file is the header file for floader_nand_ftl.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _LFS_NAND_FTL_H
#define _LFS_NAND_FTL_H

#include "ameba.h"

/* Exported defines ----------------------------------------------------------*/

#define NF_BAD_BLOCK				0x00U
#define NF_GOOD_BLOCK				0xFFU

#define NF_SMALL_PAGE_SIZE			2048U
#define NF_BIG_PAGE_SIZE			4096U

#define NAND_MFG_DOSILICON	0xE5U
#define NAND_MFG_GIGADEVICE	0xC8U
#define NAND_MFG_MACRONIX	0xC2U
#define NAND_MFG_MICRON		0x2CU
#define NAND_MFG_WINBOND	0xEFU

#define UERR_INIT					0x20U /* Init error */
#define UERR_PERM       			0x21U /* Operation not permitted */
#define UERR_PROTO					0x22U /* Protocol error */
#define UERR_CHK					0x23U /* Check error */
#define UERR_OVERRANGE				0x24U /* Operation overrange */
#define UERR_NAND_BAD_BLOCK			0x25U /* NAND bad block */
#define UERR_NAND_WORN_BLOCK		0x26U /* NAND wore block */
#define UERR_NAND_BITFLIP_WARN		0x27U /* NAND bitflips < ECC level, corrected */
#define UERR_NAND_BITFLIP_ERROR		0x28U /* NAND bitflips equals to ECC level, corrected */
#define UERR_NAND_BITFLIP_FATAL		0x29U /* NAND bitflips higher than ECC level, cannot be corrected */

#define FTL_MEMSET	_memset
#define FTL_MEMCPY	_memcpy

#define __IO	 	volatile

#if defined (__ICCARM__)
#define _PACKED_       __packed
#else
#define _PACKED_       __attribute__ ((packed))
#endif

#ifndef NULL
#define NULL 0
#endif

/* Exported types ------------------------------------------------------------*/

typedef struct {
	/* For both NOR and NAND */
	u8  MID;
	u8  DID;
	u8  ExtDID;

	/* For NAND only */
	u8	MFG[12];			/* Manufacture, ONFI parameter page 32-43 */
	u8	Model[20];			/* Device model, ONFI parameter page 44-63 */
	u32 PageSize;			/* Bytes per page, ONFI parameter page 80-83 */
	u16 OobSize;			/* OOB bytes per page, ONFI parameter page 84-85 */
	u32 PagesPerBlock;		/* Pages per erase block, ONFI parameter page 92-95 */
	u32 BlocksPerLun;		/* Blocks per logical unit, ONFI parameter page 96-99 */
	u8  LunsPerTarget;		/* Logical units per target, ONFI parameter page 100 */
	u16 MaxBadBlocksPerLun;	/* Max bad blocks per logical unit, ONFI parameter page 103-104 */
	u8  ReqHostEccLevel;	/* Required host ECC bits, ONFI parameter page 112 */

	u8  Targets;			/* Targets/dies */
	u32 Capacity;			/* Capacity */

	u8	Reserved[6];		/* Padding to 64 bytes */
} _PACKED_ Flash_InfoTypeDef;

typedef struct {
	Flash_InfoTypeDef MemInfo;
	void *MfgOps;
	__IO u8 CurTarget;
	__IO u8 Initialized;
	__IO u32 LastErasedBlockAddr;
} NAND_FTL_DeviceTypeDef;

typedef struct {
	u8(* Init)(NAND_FTL_DeviceTypeDef *nand);
	u8(* SelectTarget)(NAND_FTL_DeviceTypeDef *nand, u8 target);
	u8(* GetEccStatus)(NAND_FTL_DeviceTypeDef *nand, u8 status);
	u8(* ReadParameterPage)(NAND_FTL_DeviceTypeDef *nand, u8 *data);
} NAND_FTL_MfgOpsTypeDef;


u8 NAND_FTL_Init(void);
u8 NAND_FTL_ReadPage(u32 addr, u8 *buf);
u8 NAND_FTL_EraseBlock(unsigned int addr, u8 force);
u8 NAND_FTL_WritePage(u32 addr, const u8 *buf, u8 do_erase);
u8 NAND_FTL_MfgInit(NAND_FTL_DeviceTypeDef *nand);

#endif // _LFS_NAND_FTL_H

