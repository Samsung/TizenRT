/**
  ******************************************************************************
  * @file    floader_nand_ftl.c
  * @author  Realsil WLAN5 Team
  * @date    2021-01-27
  * @brief   This file provides the general FTL functionalities of NAND Flash
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */

#include "lfs_nand_ftl.h"
#include "littlefs_adapter.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

typedef enum {
	NAND_PAGE_WORN_OK = 0,
	NAND_PAGE_WORN_WARN = 1,
	NAND_PAGE_WORN_ERROR = 2,
	NAND_PAGE_WORN_FATAL = 3,
} NAND_FTL_PageWornStatusDef;

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


static NAND_FTL_DeviceTypeDef NF_Device;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Get block aligned page address
  * @param  nand : NAND device info
  * @param  addr : page address
  * @retval Block aligned page address
  */

static u32 NF_GetBlockAddr(NAND_FTL_DeviceTypeDef *nand, u32 addr)
{
	Flash_InfoTypeDef *info = &nand->MemInfo;
	u32 result = addr - (addr % info->PagesPerBlock);
	return result;
}

/**
  * @brief  Select target
  * @param  nand : NAND device info
  * @param  addr : page address
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NF_SelectTarget(NAND_FTL_DeviceTypeDef *nand, u32 addr)
{
	u8 ret = HAL_OK;
	u32 target;
	Flash_InfoTypeDef *info = &nand->MemInfo;
	NAND_FTL_MfgOpsTypeDef *ops = (NAND_FTL_MfgOpsTypeDef *)nand->MfgOps;

	if (ops->SelectTarget != NULL) {

		target = addr / (info->PagesPerBlock * info->BlocksPerLun * info->LunsPerTarget);

		if (target >= info->Targets) {
			return HAL_ERR_PARA;
		}

		if (target == nand->CurTarget) {
			return HAL_OK;
		}

		if (info->Targets == 1) {
			nand->CurTarget = target;
			return HAL_OK;
		}

		ret = ops->SelectTarget(nand, target);
	}

	return ret;
}

/**
  * @brief  Check bad block
  * @param  nand : NAND device info
  * @param  addr : page address
  * @param  value : 0 - Good; 1 - Bad
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NF_IsBad(NAND_FTL_DeviceTypeDef *nand, u32 addr, u8 *value)
{
	Flash_InfoTypeDef *info = &nand->MemInfo;
	u32 block_addr;
	u8 ret;
	u8 data[2];

	block_addr = NF_GetBlockAddr(nand, addr);
	ret = NAND_Page_Read(block_addr, info->PageSize, 2, data);
	if (ret != 0xFFU) {
		ret = HAL_OK;
		if ((data[0] != NF_GOOD_BLOCK) || (data[1] != NF_GOOD_BLOCK)) {
			*value = 1;
		} else {
			*value = 0;
		}
	} else {
		ret = HAL_TIMEOUT;
	}

	return ret;
}

/**
  * @brief  Mark bad block
  * @param  nand : NAND device info
  * @param  addr : page address
  * @param  value : 0 - Good; 1 - Bad
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NF_MarkBad(NAND_FTL_DeviceTypeDef *nand, unsigned int addr)
{
	Flash_InfoTypeDef *info = &nand->MemInfo;
	u32 block_addr;
	u8 ret;
	u8 data[2] = {0, 0};

	data[0] = NF_BAD_BLOCK;
	data[1] = NF_BAD_BLOCK;

	block_addr = NF_GetBlockAddr(nand, addr);
	ret = NAND_Page_Write(block_addr, info->PageSize, 2, data);
	if (ret == 0U) {
		FS_DBG(FS_WARNING, "Mark block 0x%08X to bad", addr);
		ret = HAL_OK;
	} else if (ret == 0xFFU) {
		FS_DBG(FS_ERROR, "Mark block 0x%08X to bad: TIMEOUT", addr);
		ret = HAL_TIMEOUT;
	} else {
		FS_DBG(FS_ERROR, "Fail to mark bad block 0x%08X: 0x%02X", addr, ret);
		ret = HAL_ERR_HW;
	}

	return ret;
}

/**
  * @brief  Erase block
  * @param  nand : NAND device info
  * @param  addr : page address
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NF_EraseBlock(NAND_FTL_DeviceTypeDef *nand, unsigned int addr)
{
	u8 ret;

	ret = NAND_Erase(addr);
	if (ret == 0U) {
		ret = HAL_OK;
	} else if (ret == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		FS_DBG(FS_ERROR, "Fail to erase block 0x%08X: 0x%02X", addr, ret);
		ret = NF_MarkBad(nand, addr);
		if (ret == HAL_OK) {
			ret = UERR_NAND_WORN_BLOCK;
		}
	}

	return ret;
}

/**
  * @brief  Get NAND page worn status
  * @param  ecc_status : ECC status of the NAND page
  * @retval NAND_FTL_PageWornStatusDef
  */

static NAND_FTL_PageWornStatusDef NF_GetPageWornStatus(u8 ecc_status)
{
	NAND_FTL_PageWornStatusDef page_status;

	switch (ecc_status) {
	case HAL_OK:
		page_status = NAND_PAGE_WORN_OK;
		break;
	case UERR_NAND_BITFLIP_WARN:
		page_status = NAND_PAGE_WORN_WARN;
		break;
	case UERR_NAND_BITFLIP_ERROR:
		page_status = NAND_PAGE_WORN_ERROR;
		break;
	default: // UERR_NAND_BITFLIP_FATAL
		page_status = NAND_PAGE_WORN_FATAL;
		break;
	}

	return page_status;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Init NAND FTL functionalities
  * @param  None
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_Init(void)
{
	u8 flash_ID[3];
	u8 reg;
	u8 ret;

	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	Flash_InfoTypeDef *info = &nand->MemInfo;

	FS_DBG(FS_INFO, "NAND init started");

	nand->LastErasedBlockAddr = 0xFFFFFFFF;

	flash_ID[0] = 0;
	flash_ID[1] = 0;
	flash_ID[2] = 0;

	/* NAND Flash has been initialized when boot */
	//NAND_StructInit(&flash_init_para);
	//NAND_Init(SpicOneBitMode);

	/* Reset to clean status */
	NAND_TxCmd(0xFF, 0, NULL, 0, NULL);

	NAND_RxCmd(flash_init_para.FLASH_cmd_rd_id, 0, NULL, 2, flash_ID);
	if (flash_ID[0] == NAND_MFG_MICRON) {
		/* Micron's DID are two bytes */
		NAND_RxCmd(flash_init_para.FLASH_cmd_rd_id, 0, NULL, 3, flash_ID);
	}

	info->MID = flash_ID[0];
	info->DID = flash_ID[1];
	info->ExtDID = flash_ID[2];

	reg = NAND_GetStatus(NAND_REG_CFG);
	/* Enable internal ECC */
	reg |= NAND_CFG_ECC_ENABLE;
	/* Enable/disable QUAD as per init parameters */
	if (flash_init_para.FLASH_QuadEn_bit != 0) {
		reg |= NAND_CFG_QUAD_ENABLE;
	} else {
		reg &= ~NAND_CFG_QUAD_ENABLE;
	}
	NAND_SetStatus(NAND_REG_CFG, reg);

	/* Unlock all blocks */
	/* Move the unlock operation to ImageTool */
	//NAND_SetStatus(NAND_REG_BLOCK_LOCK, NAND_BL_ALL_UNLOCKED);

	ret = NAND_FTL_MfgInit(nand);
	if (ret == HAL_OK) {
		nand->Initialized = 1;
		FS_DBG(FS_INFO, "NAND init done");
	}

	return ret;
}

/**
  * @brief  Query NAND Flash info
  * @param  nand : NAND device info
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_Query(Flash_InfoTypeDef *info)
{
	u8 ret = UERR_INIT;
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;

	if (nand->Initialized) {
		FTL_MEMCPY((void *)info, (const void *)&nand->MemInfo, sizeof(Flash_InfoTypeDef));
		ret = HAL_OK;
	}

	return ret;
}

/**
  * @brief  Read page
  * @param  addr : page address
  * @param  buf : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_ReadPage(u32 addr, u8 *buf)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	Flash_InfoTypeDef *info = &nand->MemInfo;
	//printf("info->PageSize is 0x%x ========\r\n",info->PageSize);
	u8 status;
	u8 ret;
	u8 is_bad_block;
	NAND_FTL_MfgOpsTypeDef *ops = (NAND_FTL_MfgOpsTypeDef *)nand->MfgOps;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_IsBad(nand, addr, &is_bad_block);
	if (ret != HAL_OK) {
		return ret;
	}

	if (is_bad_block) {
		ret = UERR_NAND_BAD_BLOCK;
	} else {
		status = NAND_Page_Read(addr, 0, info->PageSize, buf);
		if (status == 0U) {
			ret = HAL_OK;
		} else if (status == 0xFFU) {
			ret = HAL_TIMEOUT;
		} else {
			ret = ops->GetEccStatus(nand, status);
		}
	}

	return ret;
}

/**
  * @brief  Read page status
  * @param  addr : page address
  * @param  buf : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_ReadPageStatus(u32 addr, u8 *buf)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	Flash_InfoTypeDef *info = &nand->MemInfo;
	u8 status;
	u8 ret;
	u8 is_bad_block;
	NAND_FTL_MfgOpsTypeDef *ops = (NAND_FTL_MfgOpsTypeDef *)nand->MfgOps;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_IsBad(nand, addr, &is_bad_block);
	if (ret != HAL_OK) {
		return ret;
	}

	if (is_bad_block) {
		return UERR_NAND_BAD_BLOCK;
	}

	status = NAND_Page_Read(addr, 0, info->PageSize, buf);
	if (status == 0U) {
		ret = HAL_OK;
	} else if (status == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		ret = ops->GetEccStatus(nand, status);
	}

	return ret;
}

/**
  * @brief  Check block status
  * @param  addr : block aligned page address
  * @param  buf : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_ReadBlockStatus(u32 addr, u8 *buf, u8 *block_status, u32 *page_status)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	Flash_InfoTypeDef *info = &nand->MemInfo;
	u32 status_15_0 = 0;
	u32 status_31_16 = 0;
	u32 status_47_32 = 0;
	u32 status_63_48 = 0;
	u32 i;
	u8 ret;
	u8 is_bad_block;
	u8 has_page_warn = 0;
	u8 has_page_error = 0;
	u8 has_page_fatal = 0;
	u8 ecc_status;
	NAND_FTL_PageWornStatusDef worn_status;
	u32 block_addr = NF_GetBlockAddr(nand, addr);
	NAND_FTL_MfgOpsTypeDef *ops = (NAND_FTL_MfgOpsTypeDef *)nand->MfgOps;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, block_addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_IsBad(nand, block_addr, &is_bad_block);
	if (ret != HAL_OK) {
		return ret;
	}

	if (is_bad_block) {
		*block_status = UERR_NAND_BAD_BLOCK;
		return HAL_OK;
	}

	for (i = 0; i < info->PagesPerBlock; ++i) {
		ret = NAND_Page_Read(block_addr + i, 0, info->PageSize, buf);
		if (ret == 0U) {
			ret = HAL_OK;
		} else if (ret == 0xFFU) {
			ret = HAL_TIMEOUT;
		} else {
			ecc_status = ops->GetEccStatus(nand, ret);
			if (ecc_status != HAL_OK) {
				worn_status = NF_GetPageWornStatus(ecc_status);
				if (i < 16) {
					status_15_0 |= worn_status << (i * 2);
				} else if (i < 32) {
					status_31_16 |= worn_status << ((i - 16) * 2);
				} else if (i < 48) {
					status_47_32 |= worn_status << ((i - 32) * 2);
				} else {
					status_63_48 |= worn_status << ((i - 48) * 2);
				}
				if (worn_status == NAND_PAGE_WORN_WARN) {
					has_page_warn = 1;
				} else if (worn_status == NAND_PAGE_WORN_ERROR) {
					has_page_error = 1;
				} else if (worn_status == NAND_PAGE_WORN_FATAL) {
					has_page_fatal = 1;
				}
			}
			ret = HAL_OK;
		}
	}

	if (ret == HAL_OK) {
		*page_status = status_15_0;
		*(page_status + 1) = status_31_16;
		*(page_status + 2) = status_47_32;
		*(page_status + 3) = status_63_48;
		if (has_page_fatal != 0) {
			*block_status = UERR_NAND_BITFLIP_FATAL;
		} else if (has_page_error != 0) {
			*block_status = UERR_NAND_BITFLIP_ERROR;
		} else if (has_page_warn != 0) {
			*block_status = UERR_NAND_BITFLIP_WARN;
		} else {
			*block_status = HAL_OK;
		}
	}

	return ret;
}

/**
  * @brief  Erase block
  * @param  addr : page address
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_EraseBlock(unsigned int addr, u8 force)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	u8 ret;
	u8 is_bad_block;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_IsBad(nand, addr, &is_bad_block);
	if (ret != HAL_OK) {
		return ret;
	}

	if (is_bad_block && (force == 0)) {
		ret = UERR_NAND_BAD_BLOCK;
	} else {
		if (is_bad_block) {
			FS_DBG(FS_WARNING, "Force erase bad block 0x%08X", addr);
		}
		ret = NF_EraseBlock(nand, addr);
	}

	return ret;
}

/**
  * @brief  Write page
  * @param  addr : page address
  * @param  buf : page address
  * @param  do_erase : do block erase during write
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_WritePage(u32 addr, const u8 *buf, u8 do_erase)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	Flash_InfoTypeDef *info = &nand->MemInfo;
	u8 ret = HAL_OK;
	u8 is_bad_block;
	u32 block_addr;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_IsBad(nand, addr, &is_bad_block);
	if (ret != HAL_OK) {
		return ret;
	}

	if (is_bad_block) {
		ret = UERR_NAND_BAD_BLOCK;
	} else {
		if (do_erase) {
			block_addr = NF_GetBlockAddr(nand, addr);
			if (block_addr != nand->LastErasedBlockAddr) {
				ret = NF_EraseBlock(nand, block_addr);
				if ((ret == HAL_OK) || (ret == UERR_NAND_WORN_BLOCK)) {
					nand->LastErasedBlockAddr = block_addr;
				} else {
					return ret;
				}
			}
		}
		ret = NAND_Page_Write(addr, 0, info->PageSize, (u8 *)buf);
		if (ret == 0U) {
			ret = HAL_OK;
		} else if (ret == 0xFFU) {
			ret = HAL_TIMEOUT;
		} else {
			ret = NF_MarkBad(nand, addr);
			if (ret == HAL_OK) {
				ret = UERR_NAND_WORN_BLOCK;
			}
		}
	}

	return ret;
}

/**
  * @brief  Check bad block
  * @param  addr : page address
  * @param  value : 0 - Good; 1 - Bad
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_IsBad(u32 addr, u8 *value)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	u8 ret;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_IsBad(nand, addr, value);

	return ret;
}

/**
  * @brief  Mark bad block
  * @param  addr : page address
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_MarkBad(u32 addr)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;
	u8 ret;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	ret = NF_SelectTarget(nand, addr);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = NF_MarkBad(nand, addr);

	return ret;
}

/**
  * @brief  Read NAND status register
  * @param  cmd : command
  * @param  addr : register address
  * @param  value : value of NAND status register
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_GetStatus(u8 cmd, u8 addr, u8 *value)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	NAND_RxCmd(cmd, 1, &addr, 1, value);

	return HAL_OK;
}

/**
  * @brief  Write NAND status register
  * @param  cmd : command
  * @param  addr : register address
  * @param  value : value of NAND status register
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_SetStatus(u8 cmd, u8 addr, u8 value)
{
	NAND_FTL_DeviceTypeDef *nand = &NF_Device;

	if (!nand->Initialized) {
		return UERR_INIT;
	}

	NAND_TxCmd(cmd, 1, &addr, 1, &value);

	return NAND_WaitBusy(WAIT_FLASH_BUSY);
}

