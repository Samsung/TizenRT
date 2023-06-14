/**
  ******************************************************************************
  * @file    floader_nand_ftl_mfg.c
  * @author  Realsil WLAN5 Team
  * @date    2021-01-27
  * @brief   This file provides the manufacture specific FTL functionalities of NAND Flash
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "lfs_nand_ftl.h"

/* Private defines -----------------------------------------------------------*/

#define NF_STS_ECC_MASK				0x30U
#define NF_STS_ECC_NO_BITFLIPS		(0 << 4)
#define NF_STS_ECC_HAS_BITFLIPS		(1 << 4)
#define NF_STS_ECC_ERROR			(2 << 4)
#define NF_STS_DOSILICON_ECCS2		(1 << 6)
#define NF_STS_MICRON_ECCS2			(1 << 6)

#define NF_ECCSR_MACRONIX_CMD		0x7CU
#define NF_ECCSR_MACRONIX_ADDR		0x00U  /* Dummy */
#define NF_ECCSR_MACRONIX_MASK		0x0FU

#define NF_CFG_MICRON_CFG0			(1 << 1)
#define NF_CFG_MICRON_CFG1			(1 << 6)
#define NF_CFG_MICRON_CFG2			(1 << 7)
#define NF_CFG_WINBOND_BUF_READ		(1 << 3)

#define NF_WINBOND_DIE_SEL_CMD		0xC2U

#define NF_PARAMETER_PAGE_ADDR						0x01U
#define NF_PARAMETER_PAGE_GIGADEVICE_ADDR			0x04U
#define NF_PARAMETER_PAGE_SIZE						256U
#define NF_PARAMETER_PAGE_CNT						3U
#define NF_PARAMETER_PAGE_TOTAL_SIZE				(NF_PARAMETER_PAGE_SIZE * NF_PARAMETER_PAGE_CNT)
#define NF_PARAMETER_PAGE_OFFSET_MFG				32U
#define NF_PARAMETER_PAGE_OFFSET_MODEL				44U
#define NF_PARAMETER_PAGE_OFFSET_BYTES_PER_PAGE		80U
#define NF_PARAMETER_PAGE_OFFSET_OOB_BYTES_PER_PAGE	84U
#define NF_PARAMETER_PAGE_OFFSET_PAGES_PER_BLOCK	92U
#define NF_PARAMETER_PAGE_OFFSET_BLOCKS_PER_LUN		96U
#define NF_PARAMETER_PAGE_OFFSET_LUNS				100U
#define NF_PARAMETER_PAGE_OFFSET_MAX_BB_PER_LUN		103U
#define NF_PARAMETER_PAGE_OFFSET_REQ_HOST_ECC_LEVEL	112U
#define NF_PARAMETER_PAGE_OFFSET_CRC				254U

#define NF_PARAMETER_PAGE_ONFI_CRC_POLYNOM			0x8005U
#define NF_PARAMETER_PAGE_ONFI_CRC_BASE				0x4F4EU

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 NAND_FTL_GetMfgStatus(u8 cmd, u8 addr);
static u16 NAND_FTL_CalculateOnfiCrc16(const u8 *buf);
static s32 NAND_FTL_GetValidParameterOffset(u8 *buf);
static u8 NAND_FTL_Winbond_Init(NAND_FTL_DeviceTypeDef *nand);
static u8 NAND_FTL_Winbond_SelectTarget(NAND_FTL_DeviceTypeDef *nand, u8 target);
static u8 NAND_FTL_Common_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status);
static u8 NAND_FTL_Dosilicon_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status);
static u8 NAND_FTL_GigaDevice_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status);
static u8 NAND_FTL_Macronix_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status);
static u8 NAND_FTL_Micron_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status);
static u8 NAND_FTL_Winbond_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status);
static u8 NAND_FTL_Common_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data);
static u8 NAND_FTL_Dosilicon_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data);
static u8 NAND_FTL_GigaDevice_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data);
static u8 NAND_FTL_Micron_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data);
static u8 NAND_FTL_Macronix_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data);
static u8 NAND_FTL_Winbond_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data);

/* Private variables ---------------------------------------------------------*/


static NAND_FTL_MfgOpsTypeDef NandDefaultOps = {
	.GetEccStatus = NAND_FTL_Common_GetEccStatus,
	.ReadParameterPage = NAND_FTL_Common_ReadParameterPage
};


static NAND_FTL_MfgOpsTypeDef DosiliconOps = {
	.GetEccStatus = NAND_FTL_Dosilicon_GetEccStatus,
	.ReadParameterPage = NAND_FTL_Dosilicon_ReadParameterPage
};


static NAND_FTL_MfgOpsTypeDef GigaDeviceOps = {
	.GetEccStatus = NAND_FTL_GigaDevice_GetEccStatus,
	.ReadParameterPage = NAND_FTL_GigaDevice_ReadParameterPage
};


static NAND_FTL_MfgOpsTypeDef MacronixOps = {
	.GetEccStatus = NAND_FTL_Macronix_GetEccStatus,
	.ReadParameterPage = NAND_FTL_Macronix_ReadParameterPage
};


static NAND_FTL_MfgOpsTypeDef MicronOps = {
	.GetEccStatus = NAND_FTL_Micron_GetEccStatus,
	.ReadParameterPage = NAND_FTL_Micron_ReadParameterPage
};


static NAND_FTL_MfgOpsTypeDef WinbondOps = {
	.Init = NAND_FTL_Winbond_Init,
	.SelectTarget = NAND_FTL_Winbond_SelectTarget,
	.GetEccStatus = NAND_FTL_Winbond_GetEccStatus,
	.ReadParameterPage = NAND_FTL_Winbond_ReadParameterPage
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Read manufacture specific NAND Flash status register
  * @param  cmd : command
  * @param  addr : address
  * @retval Value of NAND Flash status register
  */

static u8 NAND_FTL_GetMfgStatus(u8 cmd, u8 addr)
{
	u8 data;

	NAND_RxCmd(cmd, 1, &addr, 1, &data);

	return data;
}

/**
  * @brief  Calculate ONFI CRC16 for parameter page
  * @param  buf : buffer for parameter page data
  * @retval ONFI CRC16 calculated result
  */

static u16 NAND_FTL_CalculateOnfiCrc16(const u8 *buf)
{
	u16 i;
	u16 crc = NF_PARAMETER_PAGE_ONFI_CRC_BASE;
	u16 len = NF_PARAMETER_PAGE_SIZE - 2U;
	while (len--) {
		crc ^= (*buf++) << 8;
		for (i = 0; i < 8; i++) {
			crc = (crc << 1) ^ ((crc & 0x8000) ? NF_PARAMETER_PAGE_ONFI_CRC_POLYNOM : 0U);
		}
	}

	return crc;
}

/**
  * @brief  Get the offset of valid parameter
  * @param  buf : buffer for parameter page data with multiple copies
  * @retval -1 : not found; >= 0 : offset of valid parameter
  */

static s32 NAND_FTL_GetValidParameterOffset(u8 *buf)
{
	s32 result = -1;
	u32 offset;
	u16 crc;
	u16 calCrc;

	for (offset = 0; offset < NF_PARAMETER_PAGE_TOTAL_SIZE; offset += NF_PARAMETER_PAGE_SIZE) {
		/* Check signature "ONFI" */
		if ((buf[offset] == 0x4FU) && (buf[offset + 1] == 0x4EU) && (buf[offset + 2] == 0x46U) && (buf[offset + 3] == 0x49U)) {
			/* Check CRC */
			crc = buf[offset + NF_PARAMETER_PAGE_OFFSET_CRC] + (buf[offset + NF_PARAMETER_PAGE_OFFSET_CRC + 1] << 8);
			calCrc = NAND_FTL_CalculateOnfiCrc16((const u8 *)(buf + offset));
			if (crc == calCrc) {
				result = (s32)offset;
				break;
			}
		}
	}

	return result;
}

/**
  * @brief  Init Winbond specific NAND FTL functionalities
  * @param  nand : NAND device info
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Winbond_Init(NAND_FTL_DeviceTypeDef *nand)
{
	u8 ret = HAL_OK;
	u8 i;
	Flash_InfoTypeDef *info = &nand->MemInfo;

	/* Make sure all dies are in buffer read mode and not continuous read mode */
	if (info->Targets > 1) {
		/* Default select target 0 */
		for (i = info->Targets; i > 0; i--) {
			ret = NAND_FTL_Winbond_SelectTarget(nand, i - 1);
			if (ret == HAL_OK) {
				NAND_SetStatusBits(NAND_REG_CFG, NF_CFG_WINBOND_BUF_READ, ENABLE);
			}
		}
	} else {
		NAND_SetStatusBits(NAND_REG_CFG, NF_CFG_WINBOND_BUF_READ, ENABLE);
	}

	return ret;
}

/**
  * @brief  Select target for Winbond
  * @param  nand : NAND device info
  * @param  target : target index
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Winbond_SelectTarget(NAND_FTL_DeviceTypeDef *nand, u8 target)
{
	Flash_InfoTypeDef *info = &nand->MemInfo;
	if (info->Targets > 1) {
		NAND_TxCmd(NF_WINBOND_DIE_SEL_CMD, 0, NULL, 1, &target);
	}
	return HAL_OK;
}

/**
  * @brief  Get ECC status for common NAND devices
  * @param  nand : NAND device info
  * @param  status : raw ECC status
  * @retval Parsed ECC status:
  *			HAL_OK : OK
  *			UERR_NAND_BITFLIP_ERROR : bitflip(s) corrected
  *			UERR_NAND_BITFLIP_FATAL : bitflips not corrected
  *			HAL_ERR_UNKNOWN: other error
  */

static u8 NAND_FTL_Common_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status)
{
	u8 ret = HAL_ERR_UNKNOWN;

	UNUSED(nand);

	switch (status & NF_STS_ECC_MASK) {
	case NF_STS_ECC_NO_BITFLIPS:
		ret = HAL_OK;
		break;
	case NF_STS_ECC_HAS_BITFLIPS:
		/* No way to know exactly how many bitflips have been fixed, warn as the maximum possible value */
		ret = UERR_NAND_BITFLIP_ERROR;
		break;
	case NF_STS_ECC_ERROR:
		ret = UERR_NAND_BITFLIP_FATAL;
		break;
	default:
		break;
	}

	return ret;
}

/**
  * @brief  Get ECC status for Dosilicon NAND devices
  * @param  nand : NAND device info
  * @param  status : raw ECC status
  * @retval Parsed ECC status:
  *			HAL_OK : OK
  *			UERR_NAND_BITFLIP_ERROR : bitflip(s) corrected
  *			UERR_NAND_BITFLIP_FATAL : bitflips not corrected
  *			HAL_ERR_UNKNOWN: other error
  */

static u8 NAND_FTL_Dosilicon_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status)
{
	u8 ret = HAL_ERR_UNKNOWN;

	UNUSED(nand);

	switch (status & NF_STS_ECC_MASK) {
	case NF_STS_ECC_NO_BITFLIPS:
		ret = HAL_OK;
		break;
	case NF_STS_ECC_HAS_BITFLIPS:
		if (status & NF_STS_DOSILICON_ECCS2) {
			ret = UERR_NAND_BITFLIP_ERROR; /* 7~8 bitflips corrected, data refreshment must be taken to guarantee data retention */
		} else {
			ret = UERR_NAND_BITFLIP_WARN;  /* 1-3 bitflips corrected */
		}
		break;
	case NF_STS_ECC_ERROR:
		ret = UERR_NAND_BITFLIP_FATAL;
	default:
		break;
	}

	return ret;
}

/**
  * @brief  Get ECC status for GigaDevice NAND devices
  * @param  nand : NAND device info
  * @param  status : raw ECC status
  * @retval Parsed ECC status:
  *			HAL_OK : OK
  *			UERR_NAND_BITFLIP_WARN : bitflip(s) corrected and bitflip(s) less than ECC bits
  *			UERR_NAND_BITFLIP_ERROR : bitflip(s) corrected and bitflip(s) equal to ECC bits
  *			UERR_NAND_BITFLIP_FATAL : bitflips not corrected
  *			HAL_ERR_UNKNOWN: other error
  */

static u8 NAND_FTL_GigaDevice_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status)
{
	u8 ret = HAL_ERR_UNKNOWN;

	UNUSED(nand);

	switch (status & NF_STS_ECC_MASK) {
	case NF_STS_ECC_NO_BITFLIPS:
		ret = HAL_OK;
		break;
	case NF_STS_ECC_HAS_BITFLIPS:
		ret = UERR_NAND_BITFLIP_WARN; /* 4-7 bitflips corrected */
		break;
	case NF_STS_ECC_ERROR:
		ret = UERR_NAND_BITFLIP_FATAL;
		break;
	default:
		ret = UERR_NAND_BITFLIP_ERROR; /* 8 bitflips corrected */
		break;
	}

	return ret;
}

/**
  * @brief  Get ECC status for Macronix NAND devices
  * @param  nand : NAND device info
  * @param  status : raw ECC status
  * @retval Parsed ECC status:
  *			HAL_OK : OK
  *			UERR_NAND_BITFLIP_WARN : bitflip(s) corrected and bitflip(s) less than ECC bits
  *			UERR_NAND_BITFLIP_ERROR : bitflip(s) corrected and bitflip(s) equal to ECC bits
  *			UERR_NAND_BITFLIP_FATAL : bitflips not corrected
  *			HAL_ERR_UNKNOWN: other error
  */

static u8 NAND_FTL_Macronix_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status)
{
	u8 ret = HAL_ERR_UNKNOWN;
	u8 eccsr;

	UNUSED(nand);

	switch (status & NF_STS_ECC_MASK) {
	case NF_STS_ECC_NO_BITFLIPS:
		ret = HAL_OK;
		break;
	case NF_STS_ECC_HAS_BITFLIPS:
		eccsr = NAND_FTL_GetMfgStatus(NF_ECCSR_MACRONIX_CMD, NF_ECCSR_MACRONIX_ADDR);
		if ((eccsr & NF_ECCSR_MACRONIX_MASK) == 4U) {
			ret = UERR_NAND_BITFLIP_ERROR; /* 4 bitflips corrected, data refreshment must be taken to guarantee data retention*/
		} else {
			ret = UERR_NAND_BITFLIP_WARN; /* 1-3 bitflips corrected */
		}
		break;
	case NF_STS_ECC_ERROR:
		ret = UERR_NAND_BITFLIP_FATAL;
		break;
	default:
		break;
	}

	return ret;
}

/**
  * @brief  Get ECC status for Micron NAND devices
  * @param  nand : NAND device info
  * @param  status : raw ECC status
  * @retval Parsed ECC status:
  *			HAL_OK : OK
  *			UERR_NAND_BITFLIP_WARN : bitflip(s) corrected and bitflip(s) less than ECC bits
  *			UERR_NAND_BITFLIP_ERROR : bitflip(s) corrected and bitflip(s) equal to ECC bits
  *			UERR_NAND_BITFLIP_FATAL : bitflips not corrected
  *			HAL_ERR_UNKNOWN: other error
  */

static u8 NAND_FTL_Micron_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status)
{
	u8 ret = HAL_ERR_UNKNOWN;

	UNUSED(nand);

	switch (status & NF_STS_ECC_MASK) {
	case NF_STS_ECC_NO_BITFLIPS:
		ret = HAL_OK;
		break;
	case NF_STS_ECC_HAS_BITFLIPS:
		if (status & NF_STS_MICRON_ECCS2) {
			ret = UERR_NAND_BITFLIP_ERROR; // 7-8 bitflips corrected, data refreshment must be taken to guarantee data retention
		} else {
			ret = UERR_NAND_BITFLIP_WARN;  // 1-3 bitflips corrected
		}
		break;
	case NF_STS_ECC_ERROR:
		ret = UERR_NAND_BITFLIP_FATAL;
		break;
	default:
		ret = UERR_NAND_BITFLIP_WARN;  // 4-6 bitflips corrected
		break;
	}

	return ret;
}

/**
  * @brief  Get ECC status for Winbond NAND devices
  * @param  nand : NAND device info
  * @param  status : raw ECC status
  * @retval Parsed ECC status:
  *			HAL_OK : OK
  *			UERR_NAND_BITFLIP_ERROR : bitflip(s) corrected
  *			UERR_NAND_BITFLIP_FATAL : bitflips not corrected
  *			HAL_ERR_UNKNOWN: other error
  */

static u8 NAND_FTL_Winbond_GetEccStatus(NAND_FTL_DeviceTypeDef *nand, u8 status)
{
	return NAND_FTL_Common_GetEccStatus(nand, status);
}

/**
  * @brief  Read parameter page for common NAND devices
  * @param  nand : NAND device info
  * @param  data : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Common_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data)
{
	u8 ret = HAL_ERR_UNKNOWN;
	u8 reg;
	u8 bk_reg;
	u8 retry;

	UNUSED(nand);

	reg = NAND_GetStatus(NAND_REG_CFG);
	bk_reg = reg;

	reg |= NAND_CFG_OTP_ENABLE;
	reg &= ~NAND_CFG_ECC_ENABLE;

	for (retry = 0; retry < 3; retry++) {
		NAND_SetStatus(NAND_REG_CFG, reg);
		reg = NAND_GetStatus(NAND_REG_CFG);
		if (((reg & NAND_CFG_OTP_ENABLE) == NAND_CFG_OTP_ENABLE) && ((reg & NAND_CFG_ECC_ENABLE) == 0)) {
			ret = HAL_OK;
			break;
		}
	}

	if (ret != HAL_OK) {
		NAND_SetStatus(NAND_REG_CFG, bk_reg);
		return HAL_TIMEOUT;
	}

	ret = NAND_Page_Read(NF_PARAMETER_PAGE_ADDR, 0, NF_PARAMETER_PAGE_TOTAL_SIZE, data);
	if (ret == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		/* ECC status will be ignored */
		ret = HAL_OK;
	}

	NAND_SetStatus(NAND_REG_CFG, 0x10);

	NAND_SetStatus(NAND_REG_CFG, bk_reg);

	return ret;
}

/**
  * @brief  Read parameter page for Dosilicon NAND devices
  * @param  nand : NAND device info
  * @param  data : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Dosilicon_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data)
{
	u8 ret = HAL_ERR_UNKNOWN;
	u8 reg;
	u8 retry;

	UNUSED(nand);

	reg = NAND_GetStatus(NAND_REG_CFG);

	for (retry = 0; retry < 3; retry++) {
		NAND_SetStatus(NAND_REG_CFG, 0x40); /* As per datasheet */
		reg = NAND_GetStatus(NAND_REG_CFG);
		if (reg == 0x40) {
			ret = HAL_OK;
			break;
		}
	}

	if (ret != HAL_OK) {
		NAND_SetStatus(NAND_REG_CFG, reg);
		return HAL_TIMEOUT;
	}

	ret = NAND_Page_Read(NF_PARAMETER_PAGE_ADDR, 0, NF_PARAMETER_PAGE_TOTAL_SIZE, data);
	if (ret == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		/* ECC status will be ignored */
		ret = HAL_OK;
	}

	NAND_SetStatus(NAND_REG_CFG, 0x10); /* As per datasheet, 0x10 */

	NAND_SetStatus(NAND_REG_CFG, reg);

	return ret;
}

/**
  * @brief  Read parameter page for GigaDevice NAND devices
  * @param  nand : NAND device info
  * @param  data : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_GigaDevice_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data)
{
	u8 ret = HAL_ERR_UNKNOWN;
	u8 reg;
	u8 bk_reg;
	u8 retry;
	u32 addr;

	UNUSED(nand);

	reg = NAND_GetStatus(NAND_REG_CFG);
	bk_reg = reg;

	reg |= NAND_CFG_OTP_ENABLE;
	reg &= ~NAND_CFG_ECC_ENABLE;

	for (retry = 0; retry < 3; retry++) {
		NAND_SetStatus(NAND_REG_CFG, reg);
		reg = NAND_GetStatus(NAND_REG_CFG);
		if (((reg & NAND_CFG_OTP_ENABLE) == NAND_CFG_OTP_ENABLE) && ((reg & NAND_CFG_ECC_ENABLE) == 0)) {
			ret = HAL_OK;
			break;
		}
	}

	if (ret != HAL_OK) {
		NAND_SetStatus(NAND_REG_CFG, bk_reg);
		return HAL_TIMEOUT;
	}

	if ((nand->MemInfo.DID == 0x81) || (nand->MemInfo.DID == 0x91)  // GD5F1GM7
		|| (nand->MemInfo.DID == 0x82) || (nand->MemInfo.DID == 0x92)) {  // GD5F2GM7
		addr = NF_PARAMETER_PAGE_ADDR;
	} else {
		addr = NF_PARAMETER_PAGE_GIGADEVICE_ADDR;
	}
	ret = NAND_Page_Read(addr, 0, NF_PARAMETER_PAGE_TOTAL_SIZE, data);
	if (ret == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		/* ECC status will be ignored */
		ret = HAL_OK;
	}

	NAND_SetStatus(NAND_REG_CFG, 0x10);

	NAND_SetStatus(NAND_REG_CFG, bk_reg);

	return ret;
}

/**
  * @brief  Read parameter page for Micron NAND devices
  * @param  nand : NAND device info
  * @param  data : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Micron_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data)
{
	u8 ret = HAL_ERR_UNKNOWN;
	u8 reg;
	u8 bk_reg;
	u8 retry;

	UNUSED(nand);

	reg = NAND_GetStatus(NAND_REG_CFG);
	bk_reg = reg;

	reg &= ~NF_CFG_MICRON_CFG0;
	reg |= NF_CFG_MICRON_CFG1;
	reg &= ~NF_CFG_MICRON_CFG2;

	for (retry = 0; retry < 3; retry++) {
		NAND_SetStatus(NAND_REG_CFG, reg);
		reg = NAND_GetStatus(NAND_REG_CFG);
		if (((reg & NF_CFG_MICRON_CFG0) == 0)
			&& ((reg & NF_CFG_MICRON_CFG1) == NF_CFG_MICRON_CFG1)
			&& ((reg & NF_CFG_MICRON_CFG2) == 0)) {
			ret = HAL_OK;
			break;
		}
	}

	if (ret != HAL_OK) {
		NAND_SetStatus(NAND_REG_CFG, bk_reg);
		return HAL_TIMEOUT;
	}

	ret = NAND_Page_Read(NF_PARAMETER_PAGE_ADDR, 0, NF_PARAMETER_PAGE_TOTAL_SIZE, data);
	if (ret == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		/* ECC status will be ignored */
		ret = HAL_OK;
	}

	NAND_SetStatus(NAND_REG_CFG, 0x00); /* As per datasheet */

	NAND_SetStatus(NAND_REG_CFG, bk_reg);

	return ret;
}

/**
  * @brief  Read parameter page for Macronix NAND devices
  * @param  nand : NAND device info
  * @param  data : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Macronix_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data)
{
	u8 ret = HAL_ERR_UNKNOWN;
	u8 reg;
	u8 retry;

	UNUSED(nand);

	reg = NAND_GetStatus(NAND_REG_CFG);

	for (retry = 0; retry < 3; retry++) {
		NAND_SetStatus(NAND_REG_CFG, 0x40); /* As per datasheet */
		reg = NAND_GetStatus(NAND_REG_CFG);
		if (reg == 0x40) {
			ret = HAL_OK;
			break;
		}
	}

	if (ret != HAL_OK) {
		NAND_SetStatus(NAND_REG_CFG, reg);
		return HAL_TIMEOUT;
	}

	ret = NAND_Page_Read(NF_PARAMETER_PAGE_ADDR, 0, NF_PARAMETER_PAGE_TOTAL_SIZE, data);
	if (ret == 0xFFU) {
		ret = HAL_TIMEOUT;
	} else {
		/* ECC status will be ignored */
		ret = HAL_OK;
	}

	NAND_SetStatus(NAND_REG_CFG, 0x10); /* As per datasheet, 0x10 or 0x00 */

	NAND_SetStatus(NAND_REG_CFG, reg);

	return ret;
}

/**
  * @brief  Read parameter page for Winbond NAND devices
  * @param  nand : NAND device info
  * @param  data : data buffer
  * @retval HAL_OK : OK; others : FAIL
  */

static u8 NAND_FTL_Winbond_ReadParameterPage(NAND_FTL_DeviceTypeDef *nand, u8 *data)
{
	return NAND_FTL_Common_ReadParameterPage(nand, data);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Init manufacture specific NAND FTL functionalities
  * @param  nand : NAND device info
  * @param  ops : callback of manufacture specific FTL operations
  * @retval HAL_OK : OK; others : FAIL
  */

u8 NAND_FTL_MfgInit(NAND_FTL_DeviceTypeDef *nand)
{
	u8 ret;
	u8 retry;
	s32 paramBufOffset;
	u8 *paramBuf;
	u8 buf[NF_PARAMETER_PAGE_TOTAL_SIZE];
	Flash_InfoTypeDef *info = &nand->MemInfo;
	NAND_FTL_MfgOpsTypeDef *ops;

	info->Targets = 1;

	switch (info->MID) {
	case NAND_MFG_DOSILICON:
		nand->MfgOps = &DosiliconOps;
		break;
	case NAND_MFG_GIGADEVICE:
		nand->MfgOps = &GigaDeviceOps;
		break;
	case NAND_MFG_MACRONIX:
		nand->MfgOps = &MacronixOps;
		break;
	case NAND_MFG_MICRON:
		nand->MfgOps = &MicronOps;
		break;
	case NAND_MFG_WINBOND:
		nand->MfgOps = &WinbondOps;
		if (info->DID == 0xAB) { /* As per Linux SPI NAND Flash driver */
			info->Targets = 2;
		}
		break;
	default:
		nand->MfgOps = &NandDefaultOps;
		break;
	}

	ops = (NAND_FTL_MfgOpsTypeDef *)nand->MfgOps;

	if (ops->Init) {
		ret = ops->Init(nand);
		if (ret != HAL_OK) {
			return ret;
		}
	}

	if (ops->ReadParameterPage) {
		for (retry = 0; retry < 3; retry++) {
			ret = ops->ReadParameterPage(nand, buf);
			if (ret == HAL_OK) {
				break;
			}
		}
		if (ret == HAL_OK) {
			paramBufOffset = NAND_FTL_GetValidParameterOffset(buf);
			if (paramBufOffset >= 0) {
				paramBuf = buf + paramBufOffset;

				FTL_MEMCPY((void *)info->MFG, (const void *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_MFG), 12);
				FTL_MEMCPY((void *)info->Model, (const void *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_MODEL), 32);

				info->PageSize = *(u32 *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_BYTES_PER_PAGE);
				info->OobSize = *(u16 *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_OOB_BYTES_PER_PAGE);
				info->PagesPerBlock = *(u32 *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_PAGES_PER_BLOCK);
				info->BlocksPerLun = *(u32 *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_BLOCKS_PER_LUN);
				info->LunsPerTarget = paramBuf[NF_PARAMETER_PAGE_OFFSET_LUNS];
				info->MaxBadBlocksPerLun = *(u16 *)(paramBuf + NF_PARAMETER_PAGE_OFFSET_MAX_BB_PER_LUN);
				info->ReqHostEccLevel = paramBuf[NF_PARAMETER_PAGE_OFFSET_REQ_HOST_ECC_LEVEL];
				info->Capacity = info->PageSize * info->PagesPerBlock * info->BlocksPerLun * info->LunsPerTarget * info->Targets;
			} else {
				ret = UERR_CHK;
				DBG_PRINTF(MODULE_FLASH, LEVEL_ERROR, "NAND parameter page checksum error\n");
			}
		} else {
			DBG_PRINTF(MODULE_FLASH, LEVEL_ERROR, "Read NAND parameter page fail: 0x%02X\n", ret);
		}
	} else {
		ret = HAL_ERR_PARA;
	}

	return ret;
}

