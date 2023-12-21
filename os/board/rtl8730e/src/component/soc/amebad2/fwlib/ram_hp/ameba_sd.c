#include "ameba_soc.h"

static const char *TAG = "SDIO";
SRAM_NOCACHE_DATA_SECTION
SD_CardInfo card_info;

static void SDIOH_Pinmux(void)
{
	Pinmux_Config(_PB_27, PINMUX_FUNCTION_SDIOH);	/* CMD */
	Pinmux_Config(_PB_28, PINMUX_FUNCTION_SDIOH);	/* CLK */
	Pinmux_Config(_PB_29, PINMUX_FUNCTION_SDIOH); 	/* D0 */

	PAD_PullCtrl(_PB_27, GPIO_PuPd_UP);
	PAD_PullCtrl(_PB_28, GPIO_PuPd_UP);
	PAD_PullCtrl(_PB_29, GPIO_PuPd_UP);

	if (sdioh_config.sdioh_bus_width == SDIOH_BUS_WIDTH_4BIT) {
		Pinmux_Config(_PB_25, PINMUX_FUNCTION_SDIOH);	/* D2 */
		Pinmux_Config(_PB_26, PINMUX_FUNCTION_SDIOH);	/* D3 */
		Pinmux_Config(_PB_30, PINMUX_FUNCTION_SDIOH);	/* D1 */

		PAD_PullCtrl(_PB_25, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_26, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_30, GPIO_PuPd_UP);
	}

	if (sdioh_config.sdioh_cd_pin != _PNC) {			/* CD */
		Pinmux_Config((u8)sdioh_config.sdioh_cd_pin, PINMUX_FUNCTION_SDIOH);
		PAD_PullCtrl((u8)sdioh_config.sdioh_cd_pin, GPIO_PuPd_UP);
	}

	if (sdioh_config.sdioh_wp_pin != _PNC) {			/* WP */
		Pinmux_Config((u8)sdioh_config.sdioh_wp_pin, PINMUX_FUNCTION_SDIOH);
		PAD_PullCtrl((u8)sdioh_config.sdioh_wp_pin, GPIO_PuPd_UP);
	}
}

/**
  * @brief  Check SDIOH response is valid or not.
  * @param  resp_type: which can be a value of @ref SDIOH_Card_Response_Byte_Index.
  * @param  cmd: indicate the command of which the response to be checked
  * @retval  0: response is valid
  *		   others: response is invalid
  */
static u32 CmdRespError(u8 resp_type, u8 cmd)
{
	u32 ret = HAL_OK;
	u16 err_status;
	u8 resp_byte0, resp_byte1, resp_byte3, resp_byte4, t_cmd = cmd;

	ret = SDIOH_CheckTxError(&err_status);
	if (ret != HAL_OK) {
		if (err_status & SDIOH_SD_CMD_RSP_TO_ERR) {
			return HAL_TIMEOUT;
		} else if (err_status & SDIOH_SD_TUNNING_PAT_COMP_ERR) {
			return HAL_ERR_HW;
		} else {
			return ret;
		}
	}

	/* For R1, R6, R7 */
	if ((resp_type == SDIOH_RESP_R1) || (resp_type == SDIOH_RESP_R6) || (resp_type == SDIOH_RESP_R7)) {
		if ((cmd == SD_CMD_WrMulBlk) || (cmd == SD_CMD_RdMulBlk)) {
			t_cmd = SD_CMD_StopXsmission;
		}

		resp_byte0 = SDIOH_GetResponse(SDIO_RESP0);
		resp_byte1 = SDIOH_GetResponse(SDIO_RESP1);
		resp_byte3 = SDIOH_GetResponse(SDIO_RESP3);
		resp_byte4 = SDIOH_GetResponse(SDIO_RESP4);

		if ((resp_byte0 & SDIOH_CMD_IDX_MASK) != t_cmd) {
			RTK_LOGE(TAG, "Command index error !!\r\n");
			return HAL_ERR_UNKNOWN;
		}

		if (cmd == SD_CMD_AppCmd) {
			if (!(resp_byte4 & SD_APP_CMD)) {
				RTK_LOGE(TAG, "ACMD isn't expected !!\r\n");
				return HAL_ERR_UNKNOWN;
			}
		} else if ((cmd == SD_CMD_RdSingleBlk) || (cmd == SD_CMD_RdMulBlk)) {
			if ((resp_byte1 & SD_ADDRESS_ERROR) || (resp_byte1 & SD_BLOCK_LEN_ERROR)) {
				return HAL_ERR_UNKNOWN;
			}
		} else if ((cmd == SD_CMD_WrBlk) || (cmd == SD_CMD_WrMulBlk)) {
			if ((resp_byte1 & SD_ADDRESS_ERROR) || (resp_byte1 & SD_BLOCK_LEN_ERROR) || (resp_byte1 & SD_WP_VIOLATION)) {
				return HAL_ERR_UNKNOWN;
			}
		}
	}

	/* For R7 */
	if (resp_type == SDIOH_RESP_R7) {
		// check the echo-back of check pattern
		if (resp_byte4 != SDIOH_CMD8_CHK_PATN) {
			RTK_LOGE(TAG, "Check pattern error !!\r\n");
			return HAL_ERR_UNKNOWN;
		}
		// check the VHS
		if ((resp_byte3 & 0xF) != SDIOH_CMD8_VHS) {
			RTK_LOGE(TAG, "Voltage accepted error !!\r\n");
			return HAL_ERR_UNKNOWN;
		}
	}

	return HAL_OK;
}

/**
  * @brief  Reset SD card.
  * @param  None.
  * @retval  0: reset SD card successfully
  *		   others: fail to reset SD card
  */
static u32 SD_ResetCard(void)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;

	/***** CMD0 *****/
	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_GoIdleSte;
	cmd_attr.rsp_type = SDIOH_NO_RESP;
	cmd_attr.rsp_crc_chk = DISABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	// check if any errors
	ret = CmdRespError(SDIOH_NO_RESP, SD_CMD_GoIdleSte);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

/**
  * @brief  Sends SD Memory Card interface condition, which includes host supply voltage information
  *  		and asks the card whether card supports voltage.
  * @param  voltage_mismatch: pointer to a variable to indicate SD card responds to CMD8 or not.
  * @retval  HAL_OK: SD card responds to CMD8 successfully
  *		   HAL_TIMEOUT: SD card does not respond to CMD8
  */
static u32 SD_VoltageCheck(u8 *voltage_mismatch)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;

	/***** CMD8 *****/
	cmd_attr.arg = (SDIOH_CMD8_VHS << 8) | SDIOH_CMD8_CHK_PATN;
	cmd_attr.idx = SD_CMD_SendIfCond;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R7, SD_CMD_SendIfCond);
	if (ret == HAL_TIMEOUT) {
		*voltage_mismatch = 1;  /* for Ver1.x SD card*/
		RTK_LOGI(TAG, "voltage mismatch\n");
		ret = HAL_OK;

	} else if (ret == HAL_OK) {
		*voltage_mismatch = 0;
		RTK_LOGI(TAG, "voltage match\n");
	}

	return ret;
}

/**
  * @brief  Forces the card to stop transmission.
  * @param  None
  * @retval  HAL_OK: Transmission is stopped successfully
  *		   Others: Fail to stop transmisstion
  */
u32 SD_StopTransfer(void)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;
	SDIOH_TypeDef *psdioh = SDIOH_BASE;

	/***** CMD12 *****/
	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_StopXsmission;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_StopXsmission);
	if (ret != HAL_OK) {
		return ret;
	}

	// stop the transfer & the transfer state machine returns to idle state
	psdioh->CARD_STOP = SDIOH_TARGET_MODULE_SD;

	return HAL_OK;
}

/**
  * @brief  Get operating condition register(OCR) value in response.
  * @param  voltage_mismatch: indicate voltage mismatch or not when check voltage.
  * @retval  HAL_OK: Get OCR successfully
  *		   Others: Fail to get OCR
  */
#if defined(SDIO) && (SDIO == SD)

static u32 SD_GetOCR(u8 voltage_mismatch)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;
	u32 retry = 100;

	while (retry--) {
		// Normal ACMD41 (CMD55)
		cmd_attr.arg = 0;
		cmd_attr.idx = SD_CMD_AppCmd;
		cmd_attr.rsp_type = SDIOH_RSP_6B;
		cmd_attr.rsp_crc_chk = ENABLE;
		cmd_attr.data_present = SDIOH_NO_DATA;
		ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
		if (ret != HAL_OK) {
			return ret;
		}

		ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_AppCmd);
		if (ret != HAL_OK) {
			break;
		}

		// Normal ACMD41 (CMD41)
#ifdef SDIOH_SUPPORT_SD30
		if (voltage_mismatch)
			cmd_attr.arg = (SD_SUPPORT_SDSC_ONLY << 30) | (SD_MAX_PERFORM << 28) | (SD_USE_CUR_VOL << 24) |
						   SDIOH_OCR_VDD_WIN;  // 3.3V
		else
			cmd_attr.arg = (SD_SUPPORT_SDHC_SDXC << 30) | (SD_MAX_PERFORM << 28) | (SD_SWITCH_18V << 24) |
						   SDIOH_OCR_VDD_WIN;  // 1.8V
#else
		if (voltage_mismatch) {
			cmd_attr.arg = (SD_SUPPORT_SDSC_ONLY << 30) | SDIOH_OCR_VDD_WIN;
		} else {
			cmd_attr.arg = (SD_SUPPORT_SDHC_SDXC << 30) | SDIOH_OCR_VDD_WIN;
		}
#endif

		cmd_attr.idx = SD_CMD_SdSendOpCond;
		cmd_attr.rsp_type = SDIOH_RSP_6B;
		cmd_attr.rsp_crc_chk = DISABLE;
		cmd_attr.data_present = SDIOH_NO_DATA;
		ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
		if (ret != HAL_OK) {
			return ret;
		}

		ret = CmdRespError(SDIOH_RESP_R3, SD_CMD_SdSendOpCond);
		if (ret != HAL_OK) {
			return ret;
		}

		// check if initialization is complete, Card power up status bit, OCR bit31
		if (SDIOH_GetResponse(SDIO_RESP1) & BIT7) {
			break;
		}

		DelayUs(10000);
	}

	if (ret != HAL_OK) {
		return ret;
	}
	if (!retry) {
		return HAL_TIMEOUT;
	}

	// check CCS(Card Capacity Status) bit, OCR bit30
	if (SDIOH_GetResponse(SDIO_RESP1) & BIT6) {
		card_info.is_sdhc_sdxc = 1;
		RTK_LOGI(TAG, "This is a SDHC/SDXC card...\r\n");

#ifdef SDIOH_SUPPORT_SD30
		// check S18A(Switching to 1.8V Accepted) bit, OCR bit24
		if (SDIOH_GetResponse(SDIO_RESP1) & BIT0) {
			ret = SD_VoltageSwitch();
			if (ret != HAL_OK) {
				return ret;
			}
		} else {
			card_info.sig_level = SDIOH_SIG_VOL_33;
			card_info.bus_spd = SD_SPEED_DS;
			RTK_LOGI(TAG, "Keep 3.3V...\r\n");
		}
#else
		card_info.sig_level = SDIOH_SIG_VOL_33;
		card_info.bus_spd = SD_SPEED_DS;
		RTK_LOGI(TAG, "Keep 3.3V...\r\n");
#endif
	} else {
		card_info.is_sdhc_sdxc = 0;
		card_info.sig_level = SDIOH_SIG_VOL_33;
		card_info.bus_spd = SD_SPEED_DS;
		RTK_LOGI(TAG, "This is a SDSC card...\r\n");
	}

	return HAL_OK;
}

#else

static SD_RESULT SD_GetOCR(u8 voltage_mismatch)
{

	u32 ret, cnt = 1000;
	SDIOH_CmdTypeDef cmd_attr;
	u8 resp_byte1;

	card_info.is_sdhc_sdxc = 1;

	resp_byte1 = SDIOH_GetResponse(SDIO_RESP1);

	do {
		cmd_attr.arg = 0x40FF8080;
		cmd_attr.idx = EMMC_CMD_SendOpCond;
		cmd_attr.rsp_type = SDIOH_RSP_6B;
		cmd_attr.rsp_crc_chk = DISABLE;
		cmd_attr.data_present = SDIOH_DATA_EXIST;
		ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);

		if (ret != HAL_OK) {
			return ret;
		}

		ret = CmdRespError(SDIOH_RESP_R3, EMMC_CMD_SendOpCond);
		if (ret != HAL_OK) {
			return ret;
		}

		resp_byte1 = SDIOH_GetResponse(SDIO_RESP1);

		DelayMs(1);

		if (resp_byte1 & BIT7) {
			return HAL_OK;
		}
	} while (cnt--);

	return HAL_ERR_UNKNOWN;

}

#endif
/**
  * @brief  Asks any card to send the CID numbers on the CMD line (any card that is connected to the host will respond)
  * @param  None.
  * @retval  HAL_OK: Get CID successfully
  *		   Others: Fail to get CID
  */
static u32 SD_GetCID(SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;
	u8 *pbuf = card_info.dma_buf;

	/***** CMD2 *****/
	_memset((void *)(pbuf), 0, SDIOH_C6R2_BUF_LEN);
	DCache_CleanInvalidate((u32)pbuf, SDIOH_C6R2_BUF_LEN);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)(pbuf)) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_R2;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_AllSendCid;
	cmd_attr.rsp_type = SDIOH_RSP_17B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_XFER_CPLT_TIMEOUT, SD_SemStruct);
	if (ret != HAL_OK) {
		return ret;
	}

	DCache_Invalidate((u32)pbuf, SDIOH_C6R2_BUF_LEN);

	ret = CmdRespError(SDIOH_RESP_R2, SD_CMD_AllSendCid);
	if (ret != HAL_OK) {
		return ret;
	}
#if defined(SDIO) && (SDIO == EMMC)
	RTK_LOGI(TAG, "Manufacturer ID:%d\r\n", pbuf[1]);
	RTK_LOGI(TAG, "OEM/Application ID:%x\r\n", pbuf[3]);
	RTK_LOGI(TAG, "Product name:%x%x%x%x%x%x\r\n", pbuf[4], pbuf[5], pbuf[6], pbuf[7], pbuf[8],
			 pbuf[9]);
	RTK_LOGI(TAG, "Product serial number:%02X%02X%02X%02X\r\n", pbuf[11], pbuf[12], pbuf[13],
			 pbuf[14]);
	RTK_LOGI(TAG, "Manufacturing date:%d/%d\r\n", 2013 + (pbuf[15] & 0xf), pbuf[15] >> 4);
#else
	RTK_LOGI(TAG, "Manufacturer ID:%d\r\n", pbuf[1]);
	RTK_LOGI(TAG, "OEM/Application ID:%c%c\r\n", pbuf[2], pbuf[3]);
	//RTK_LOGI(TAG, "Product name:%c%c%c%c%c\r\n", pbuf[4], pbuf[5], pbuf[6], pbuf[7], pbuf[8]);
	//RTK_LOGI(TAG, "Product serial number:%02X%02X%02X%02X\r\n", pbuf[10], pbuf[11], pbuf[12], pbuf[13]);
	RTK_LOGI(TAG, "Manufacturing date:%d/%d\r\n", 2000 + (((pbuf[14] & 0xF) << 4) | (pbuf[15] >> 4)),
			 pbuf[15] & 0xF);
#endif
	return HAL_OK;
}

/**
  * @brief  Ask the card to publish a new relative address (RCA)
  * @param  None.
  * @retval  HAL_OK: Get RCA successfully
  *		   Others: Fail to get RCA
  */
static u32 SD_GetRCA(void)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;

	/***** CMD3 *****/
	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_SendRelAddr;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R6, SD_CMD_SendRelAddr);
	if (ret != HAL_OK) {
		return ret;
	}

	// get RCA
	card_info.rca = (SDIOH_GetResponse(SDIO_RESP1) << 8) | (SDIOH_GetResponse(SDIO_RESP2));
	RTK_LOGI(TAG, "RCA = %04X\r\n", card_info.rca);

	return HAL_OK;
}

/**
  * @brief  Ask the Addressed card sends its card-specific data (CSD) on the CMD line.
  * @param  None.
  * @retval  HAL_OK: Get CSD successfully
  *		   Others: Fail to get CSD
  */
static u32 SD_GetCSD(SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;
	u8 *pbuf = card_info.dma_buf;
	u32 c_size, n;

	/***** CMD9 *****/
	_memset((void *)(pbuf), 0, SDIOH_C6R2_BUF_LEN);
	DCache_CleanInvalidate((u32)pbuf, SDIOH_C6R2_BUF_LEN);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)(pbuf)) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_R2;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = (card_info.rca) << 16;
	cmd_attr.idx = SD_CMD_SendCsd;
	cmd_attr.rsp_type = SDIOH_RSP_17B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_XFER_CPLT_TIMEOUT, SD_SemStruct);
	if (ret != HAL_OK) {
		return ret;
	}

	DCache_Invalidate((u32)pbuf, SDIOH_C6R2_BUF_LEN);

	ret = CmdRespError(SDIOH_RESP_R2, SD_CMD_SendCsd);
	if (ret != HAL_OK) {
		return ret;
	}

	_memcpy(card_info.csd, pbuf + 1, SDIOH_CSD_LEN);

#if defined(SDIO) && (SDIO == SD)
	if (card_info.csd[0] >> 6) {
		c_size = (((card_info.csd[7] & 0x3F) << 16) | (card_info.csd[8] << 8) | (card_info.csd[9])) + 1;
		card_info.capaticy = c_size << 9;  //KB

		RTK_LOGI(TAG, "CSD Version:2.0\r\n");
		RTK_LOGI(TAG, "User data area capacity: %d GB\r\n", card_info.capaticy / 1024 / 1024);

	} else {
		c_size = (((card_info.csd[6] & 0x3) << 10) | (card_info.csd[7] << 2) | (card_info.csd[8] >> 6)) + 1;
		n = ((((card_info.csd[9] & 0x3) << 1) | (card_info.csd[10] >> 7)) + 2) + (card_info.csd[5] & 0xF);

		card_info.capaticy = (u32)(c_size << (n - 10));  //KB

		RTK_LOGI(TAG, "CSD Version:1.0\r\n");
		RTK_LOGI(TAG, "User data area capacity: %d MB\r\n", card_info.capaticy / 1024);
	}
#endif

	card_info.read_bl_len = 1 << (card_info.csd[5] & 0xF);
	card_info.write_bl_len = 1 << (((card_info.csd[12] & 0x3) << 2) | (card_info.csd[13] >> 6));

	RTK_LOGI(TAG, "Max. read data block length: %d Bytes\r\n", card_info.read_bl_len);
	RTK_LOGI(TAG, "Max. write data block length: %d Bytes\r\n", card_info.write_bl_len);

	return HAL_OK;
}

/**
  * @brief  Select/Deselect the SD card.
  * @param  select: can be _TRUE or _FALSE.
  * @retval  HAL_OK: Select/Deselect card successfully
  *		   Others: Fail to Select/Deselect card
  */
static u32 SD_SelectDeselect(u8 select)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;

	/***** CMD7 *****/
	if (select == _TRUE) {
		cmd_attr.arg = (card_info.rca) << 16;
		cmd_attr.idx = SD_CMD_SelDeselCard;
		cmd_attr.rsp_type = SDIOH_RSP_6B;
		cmd_attr.rsp_crc_chk = ENABLE;
		cmd_attr.data_present = SDIOH_NO_DATA;
		ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
		if (ret != HAL_OK) {
			return ret;
		}

		ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SelDeselCard);
		if (ret != HAL_OK) {
			return ret;
		}
	} else {
		// address 0 deselects all
		cmd_attr.arg = 0;
		cmd_attr.idx = SD_CMD_SelDeselCard;
		cmd_attr.rsp_type = SDIOH_NO_RESP;
		cmd_attr.rsp_crc_chk = DISABLE;
		cmd_attr.data_present = SDIOH_NO_DATA;
		ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
		if (ret != HAL_OK) {
			return ret;
		}

		// check if any errors
		ret = CmdRespError(SDIOH_NO_RESP, SD_CMD_SelDeselCard);
		if (ret != HAL_OK) {
			return ret;
		}
	}

	return HAL_OK;
}

/**
  * @brief  Set the bus width of the SD card.
  * @param  bus_width: can be one of the @ref SDIOH_Bus_Width
  * @retval  HAL_OK: Set bus width successfully
  *		   Others: Fail to set bus width
  */
static u32 SD_SetBusWidth(u8 bus_width)
{
	u32 ret, wid_arg_4bit, wid_arg_1bit;
	SDIOH_CmdTypeDef cmd_attr;

	if (bus_width > SDIOH_BUS_WIDTH_4BIT) {
		return HAL_ERR_PARA;
	}

	if (SDIOH_GetBusWidth() == bus_width) {
		RTK_LOGI(TAG, "Current SD bus width is already the specified setting...\r\n");
		return HAL_OK;
	}

#if defined(SDIO) && (SDIO == SD)
	/***** ACMD6 (CMD55) *****/
	cmd_attr.arg = (card_info.rca) << 16;
	cmd_attr.idx = SD_CMD_AppCmd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_AppCmd);
	if (ret != HAL_OK) {
		return ret;
	}
	wid_arg_4bit = 0x2;
	wid_arg_1bit = 0x0;
#else
	wid_arg_4bit = 0x03B70100;   //EXT_CSD register B7 byte: 01, 4bit mode; 00, 1bit mode
	wid_arg_1bit = 0x03B70000;
#endif
	/***** ACMD6 (CMD6) *****/
	if (bus_width == SDIOH_BUS_WIDTH_4BIT) {
		cmd_attr.arg = wid_arg_4bit;  // 4-bit bus
	} else {
		cmd_attr.arg = wid_arg_1bit;  // 1-bit bus
	}
	cmd_attr.idx = SD_CMD_SetBusWidth;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SetBusWidth);
	if (ret != HAL_OK) {
		return ret;
	}

	// Host also selects the specified mode
	SDIOH_SetBusWidth(bus_width);

	return HAL_OK;
}

/**
  * @brief  Get the SD Configuration Register (SCR).
  * @param  None
  * @retval  HAL_OK: Get SCR successfully
  *		   Others: Fail to get SCR
  */
static u32 SD_GetSCR(SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;
	u8 *pbuf = card_info.dma_buf;

	/***** ACMD51 (CMD55) *****/
	cmd_attr.arg = (card_info.rca) << 16;
	cmd_attr.idx = SD_CMD_AppCmd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_AppCmd);
	if (ret != HAL_OK) {
		return ret;
	}

	/***** ACMD51 (CMD51) *****/
	_memset((void *)(pbuf), 0, SDIOH_C6R2_BUF_LEN);
	DCache_CleanInvalidate((u32)pbuf, SDIOH_C6R2_BUF_LEN);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)(pbuf)) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_64B;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_SendScr;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_XFER_CPLT_TIMEOUT, SD_SemStruct);
	if (ret != HAL_OK) {
		ret = SD_StopTransfer();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG, "Stop transmission error !!\r\n");
		}

		return HAL_ERR_UNKNOWN;
	}

	DCache_Invalidate((u32)pbuf, SDIOH_C6R2_BUF_LEN);

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SendScr);
	if (ret != HAL_OK) {
		return ret;
	}

	switch (pbuf[0] & 0xF) {
	case 2:
		if (pbuf[2] >> 7) {
			RTK_LOGI(TAG, "SD specification version: 3.0X\r\n");
			card_info.sd_spec_ver = SD_SPEC_V300;
		} else {
			RTK_LOGI(TAG, "SD specification version: 2.00\r\n");
			card_info.sd_spec_ver = SD_SPEC_V200;
		}
		break;
	case 1:
		RTK_LOGI(TAG, "SD specification version: 1.10\r\n");
		card_info.sd_spec_ver = SD_SPEC_V110;
		break;
	case 0:
		RTK_LOGI(TAG, "SD specification version: 1.01\r\n");
		card_info.sd_spec_ver = SD_SPEC_V101;
		break;
	default:
		RTK_LOGW(TAG, "SD specification version: Unknown\r\n");
		card_info.sd_spec_ver = 0xFF;
	}

	RTK_LOGI(TAG, "Data status after erase: %d\r\n", pbuf[1] >> 7);

	return HAL_OK;
}

/**
  * @brief  Checks switchable function (mode 0) and switch card function (mode 1).
  * @param  mode: can be a value of @ref SD_CMD6_operation_mode
  * @param  speed: can be a value of @SD_CMD6_Function_Group1
  * @param  buf_32align: pointer to a buffer to save the switch function status
  * @retval  HAL_OK: Check/Set function successfully
  *		   Others: Fail to get check/Set function
  */
static u32 SD_SwitchFunction(u8 mode, u8 speed, u8 *buf_32align, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;

	if ((buf_32align == NULL) || (((u32)buf_32align) & 0x1F)) {
		return HAL_ERR_PARA;
	}

	/***** CMD6 *****/
	_memset((void *)buf_32align, 0, SDIOH_C6R2_BUF_LEN);
	DCache_CleanInvalidate((u32)buf_32align, SDIOH_C6R2_BUF_LEN);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)buf_32align) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_64B;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = (mode << 31) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | speed;
	cmd_attr.idx = SD_CMD_SwitchFunc;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_XFER_CPLT_TIMEOUT, SD_SemStruct);
	if (ret != HAL_OK) {
		ret = SD_StopTransfer();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG,  "Stop transmission error !!\r\n");
		}

		return HAL_ERR_UNKNOWN;
	}

	DCache_Invalidate((u32)buf_32align, SDIOH_C6R2_BUF_LEN);

	// check if any errors
	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SwitchFunc);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

/**
  * @brief  SD card interrupt handler.
  * @param  None
  * @retval  None
  */
static u32 SD_IRQHandler(SD_DMASemaStruct *SD_SemStruct)
{
	SDIOH_TypeDef *psdioh = SDIOH_BASE;
	volatile u32 tmp1;
	volatile u8 tmp2 = 0;

	tmp1 = SDIOH_GetISR();

	if (tmp1 & SDIOH_DMA_CTL_INT_EN) {
		if (SD_SemStruct->Psd_sema != NULL) {
			SD_SemStruct->SDSendSem(SD_SEMA_MAX_DELAY);
		}
	}

	if (tmp1) {
		SDIOH_INTClearPendingBit(tmp1);
	}

	if (psdioh->CARD_INT_PEND & SDIOH_SDMMC_INT_PEND) {
		tmp2 = psdioh->CARD_EXIST;

		if (tmp2 & SDIOH_SD_EXIST) {
			if (tmp2 & SDIOH_SD_WP) {
				card_info.sd_status = SD_PROTECTED;
			} else {
				card_info.sd_status = SD_INSERT;
			}

			RTK_LOGI(TAG, "Card Detect\n");
		} else {
			card_info.sd_status = SD_NODISK;

			RTK_LOGI(TAG, "Card Remove\n");
		}

		psdioh->CARD_INT_PEND |= SDIOH_SDMMC_INT_PEND;
	}

	__DSB();

	return 0;
}

SD_RESULT SD_GetEXTCSD(u8 *pbuf, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;

	/***** CMD8 *****/
	DCache_CleanInvalidate((u32)pbuf, SD_BLOCK_SIZE);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)(pbuf)) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_NORMAL;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = 0;
	cmd_attr.idx = EMMC_CMD_SendExtCsd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 100);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Send CMD8 error !!\r\n");
		return (SD_RESULT)ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_READ_TIMEOUT, SD_SemStruct);

	if (ret != HAL_OK) {
		ret = SD_StopTransfer();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG, "Stop transmission error !!\r\n");
		}

		return (SD_RESULT)HAL_ERR_UNKNOWN;
	}

	DCache_Invalidate((u32)pbuf, SD_BLOCK_SIZE);

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SendIfCond);

	return (SD_RESULT)ret;
}

/**
 *  @brief To read one block from the SD card.
 *  @param  BlockIdx: The start block to begin to read from the card.
 *  @param  readbuff: The buffer to read data blocks (must be 32-Byte alignment).
 *  @retval  HAL_OK: Read data successfully
 *  		   Others: Fail to read data
 */
u32 SD_ReadBlock(uint8_t *readbuff, uint32_t BlockIdx, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret, start;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;

	assert_param((readbuff != NULL) && ((((u32)readbuff) & 0x1F) == 0));

	if (card_info.is_sdhc_sdxc) {
		start = (u32)BlockIdx;
	} else {
		start = (u32)(BlockIdx * SD_BLOCK_SIZE);
	}

	/***** CMD17 *****/
	DCache_CleanInvalidate((u32)readbuff, SD_BLOCK_SIZE);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)readbuff) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_NORMAL;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = start;
	cmd_attr.idx = SD_CMD_RdSingleBlk;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Send CMD17 error !!\r\n");
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_READ_TIMEOUT, SD_SemStruct);

	if (ret != HAL_OK) {
		ret = SD_StopTransfer();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG, "Stop transmission error !!\r\n");
		}

		return HAL_ERR_UNKNOWN;
	}

	DCache_Invalidate((u32)readbuff, SD_BLOCK_SIZE);

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_RdSingleBlk);

	return ret;
}

/**
 *  @brief To read multi-block from the SD card.
 *  @param  BlockIdx: The start block to begin to read from the card.
 *  @param  readbuff: The buffer to read data blocks (must be 32-Byte alignment).
 *  @param  NumberOfBlocks: the number of blocks to be read
 *  @retval  HAL_OK: Read data successfully
 *  		   Others: Fail to read data
 */
u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t BlockIdx, uint32_t NumberOfBlocks, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret, start;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;

	assert_param(NumberOfBlocks > 1);
	assert_param((readbuff != NULL) && ((((u32)readbuff) & 0x1F) == 0));

	if (card_info.is_sdhc_sdxc) {
		start = (u32)BlockIdx;
	} else {
		start = (u32)(BlockIdx * SD_BLOCK_SIZE);
	}

	/***** CMD18 *****/
	DCache_CleanInvalidate((u32)readbuff, NumberOfBlocks * SD_BLOCK_SIZE);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)readbuff) / 8;
	dma_cfg.blk_cnt = NumberOfBlocks;
	dma_cfg.type = SDIOH_DMA_NORMAL;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = start;
	cmd_attr.idx = SD_CMD_RdMulBlk;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_READ_TIMEOUT * NumberOfBlocks, SD_SemStruct);
	if (ret != HAL_OK) {
		return ret;
	}

	DCache_Invalidate((u32)readbuff, NumberOfBlocks * SD_BLOCK_SIZE);

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_RdMulBlk);

	return ret;

}

/**
 *  @brief To write one block to the SD card.
 *  @param  BlockIdx: The start block to begin writing to the card.
 *  @param  writebuff: The buffer to write data blocks (must be 32-Byte alignment).
 *  @retval  HAL_OK: Write data successfully
 *  		   Others: Fail to write data
 */
u32 SD_WriteBlock(uint8_t *writebuff, uint32_t BlockIdx, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret, start;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;
	assert_param((writebuff != NULL) && ((((u32)writebuff) & 0x1F) == 0));

	if (card_info.is_sdhc_sdxc) {
		start = (u32)BlockIdx;
	} else {
		start = (u32)(BlockIdx * SD_BLOCK_SIZE);
	}

	/***** CMD24 *****/
	DCache_CleanInvalidate((u32)writebuff, SD_BLOCK_SIZE);

	dma_cfg.op = SDIOH_DMA_WRITE;
	dma_cfg.start_addr = ((u32)writebuff) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_NORMAL;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = start;
	cmd_attr.idx = SD_CMD_WrBlk;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_WRITE_TIMEOUT, SD_SemStruct);
	if (ret != HAL_OK) {
		ret = SD_StopTransfer();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG, "Stop transmission error !!\r\n");
		}

		return HAL_ERR_UNKNOWN;
	}

	// check if any errors
	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_WrBlk);

	return ret;

}

/**
 *  @brief To write multi-block to the SD card.
 *  @param  BlockIdx: The start block to begin writing to the card.
 *  @param  NumberOfBlocks: The block count.
 *  @param  writebuff: The buffer to write data blocks (must be 32-Byte alignment).
 *  @retval  HAL_OK: Write data successfully
 *  		   Others: Fail to write data
 */
u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t BlockIdx, uint32_t NumberOfBlocks, SD_DMASemaStruct *SD_SemStruct)
{
	u32 start, ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;

	assert_param(NumberOfBlocks > 1);
	assert_param((writebuff != NULL) && ((((u32)writebuff) & 0x1F) == 0));

	if (card_info.is_sdhc_sdxc) {
		start = (u32)BlockIdx;
	} else {
		start = (u32)(BlockIdx * SD_BLOCK_SIZE);
	}

#if defined(SDIO) && (SDIO == SD)
	/***** ACMD23 (CMD55) *****/
	cmd_attr.arg = (card_info.rca) << 16;
	cmd_attr.idx = SD_CMD_AppCmd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_AppCmd);
	if (ret != HAL_OK) {
		return ret;
	}

	/***** ACMD23 (CMD23) *****/
	cmd_attr.arg = NumberOfBlocks;
	cmd_attr.idx = SD_CMD_SetWrBlkEraseCnt;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SetWrBlkEraseCnt);
	if (ret != HAL_OK) {
		return ret;
	}
#endif
	DCache_CleanInvalidate((u32)writebuff, NumberOfBlocks * SD_BLOCK_SIZE);

	/***** CMD25 *****/
	dma_cfg.op = SDIOH_DMA_WRITE;
	dma_cfg.start_addr = ((u32)writebuff) / 8;
	dma_cfg.blk_cnt = NumberOfBlocks;
	dma_cfg.type = SDIOH_DMA_NORMAL;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = start;
	cmd_attr.idx = SD_CMD_WrMulBlk;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_WRITE_TIMEOUT * NumberOfBlocks, SD_SemStruct);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_WrMulBlk);

	return ret;

}

/**
 *  @brief To erase data in the SD card.
 *  @param  StartBlock: The start block to begin erasing.
 *  @param  EndBlock: The end block to begin erasing.
 *  @retval  HAL_OK: Erase data successfully
 *  		   Others: Fail to Erase data.
 */
u32 SD_Erase(uint32_t StartBlock, uint32_t EndBlock)
{
	SDIOH_CmdTypeDef cmd_attr;
	u32 start, end, ret, blk_cnt, start_cmd, end_cmd;

#if defined(SDIO) && (SDIO == EMMC)
	start_cmd = EMMC_CMD_EraseAddrSt;
	end_cmd = EMMC_CMD_EraseAddrEd;
#else
	start_cmd = SD_CMD_EraseBlkSt;
	end_cmd = SD_CMD_EraseBlkEd;
#endif

	assert_param(StartBlock <= EndBlock);

	if (card_info.is_sdhc_sdxc) {
		start = (u32)StartBlock;
		end = (u32)EndBlock;
	} else {
		start = (u32)(StartBlock * SD_BLOCK_SIZE);
		end = (u32)(EndBlock * SD_BLOCK_SIZE);
	}

	blk_cnt = EndBlock - StartBlock + 1;

	/***** CMD32 *****/
	cmd_attr.arg = start;
	cmd_attr.idx = start_cmd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Send CMD32 error !!\r\n");
	}

	ret = CmdRespError(SDIOH_RESP_R1, start_cmd);
	if (ret != HAL_OK) {
		return ret;
	}

	/***** CMD33 *****/
	cmd_attr.arg = end;
	cmd_attr.idx = end_cmd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Send CMD33 error !!\r\n");
	}

	ret = CmdRespError(SDIOH_RESP_R1, end_cmd);
	if (ret != HAL_OK) {
		return ret;
	}

	/***** CMD38 *****/
	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_Erase;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_ERASE_TIMEOUT * blk_cnt);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Send CMD38 error !!\r\n");
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_Erase);
	return ret;
}

/**
 *  @brief To get the current state of the SD card.
 *  @param  None.
 *  @retval  Current state of SD card, which is one of the @ref SD_Card_States values.
 */
u8 SD_GetCardStatus(void)
{
	u32 ret;
	u8 state;
	SDIOH_CmdTypeDef cmd_attr;

	/***** CMD13 *****/
	cmd_attr.arg = (card_info.rca) << 16;
	cmd_attr.idx = SD_CMD_SendSts;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SendSts);
	if (ret != HAL_OK) {
		return SD_CARD_ERROR;
	}

	// get card's current state
	state = (SDIOH_GetResponse(SDIO_RESP3) >> 1) & 0xF;
	RTK_LOGI(TAG, "card_curr_ste = %d\r\n", state);

	return state;
}

/**
 *  @brief To get the SD status from the SD card.
 *  @param  buf_32align: The buffer to store the SD status (must be 32-Byte alignment).
 *  @retval  HAL_OK: Get SD status successfully
 *  		   Others: Fail to get SD status.
 */
u32 SD_GetSDStatus(u8 *buf_32align, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_DmaCtl dma_cfg;
	SDIOH_CmdTypeDef cmd_attr;

	assert_param((buf_32align != NULL) && ((((u32)buf_32align) & 0x1F) == 0));

	/***** ACMD13 (CMD55) *****/
	cmd_attr.arg = (card_info.rca) << 16;
	cmd_attr.idx = SD_CMD_AppCmd;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_AppCmd);
	if (ret != HAL_OK) {
		return ret;
	}

	/***** ACMD13 (CMD13) *****/
	_memset((void *)buf_32align, 0, SDIOH_C6R2_BUF_LEN);
	DCache_CleanInvalidate((u32)buf_32align, SDIOH_C6R2_BUF_LEN);

	dma_cfg.op = SDIOH_DMA_READ;
	dma_cfg.start_addr = ((u32)buf_32align) / 8;
	dma_cfg.blk_cnt = 1;
	dma_cfg.type = SDIOH_DMA_64B;
	SDIOH_DMAConfig(&dma_cfg);

	cmd_attr.arg = 0;
	cmd_attr.idx = SD_CMD_SendSts;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_DATA_EXIST;
	ret = SDIOH_SendCommand(&cmd_attr, 0);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = SDIOH_WaitDMADone(SDIOH_XFER_CPLT_TIMEOUT, SD_SemStruct);
	if (ret != HAL_OK) {
		ret = SD_StopTransfer();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG, "Stop transmission error !!\r\n");
		}

		return HAL_ERR_UNKNOWN;
	}

	DCache_Invalidate((u32)buf_32align, SDIOH_C6R2_BUF_LEN);

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SendSts);

	return ret;
}

/**
  *  @brief To switch the SD bus speed.
  *  @param  speed: can be SD_SPEED_DS or SD_SPEED_HS of @ref SD_access_mode
  *  @retval  HAL_OK: switch speed successfully
  *  		   Others: Fail to switch speed.
  */
#if defined(SDIO) && (SDIO == SD)

u32 SD_SwitchBusSpeed(u8 speed, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	u8 sw_spd, support_spd;
	u8 *pbuf = card_info.dma_buf;

	assert_param(speed <= SD_SPEED_HS);

	if ((card_info.bus_spd) == speed) {
		RTK_LOGI(TAG, "Current SD bus speed is already the specified setting.\r\n");
		return HAL_OK;
	}

	/* Get spec version to check if card supports CMD6 which was added in version 1.10 */
	ret = SD_GetSCR(SD_SemStruct);
	if (ret != HAL_OK) {
		return ret;
	}

	if ((card_info.sd_spec_ver) >= SD_SPEC_V110) {
		/* get the supported speed modes */
		ret = SD_SwitchFunction(SD_CMD6_CHECK_MODE, SD_KEEP_CUR_SPEED, pbuf, SD_SemStruct);
		if (ret != HAL_OK) {
			return ret;
		}

		sw_spd = speed;

		support_spd = pbuf[13];
		if (support_spd & (1 << sw_spd)) {
			if ((pbuf[16] & 0xF) == sw_spd) {
				RTK_LOGI(TAG, "SD card's current speed mode is already the specified setting !!\r\n");
			} else {
				/* check if the specified speed can be switched */
				ret = SD_SwitchFunction(SD_CMD6_CHECK_MODE, sw_spd, pbuf, SD_SemStruct);

				if (ret != HAL_OK) {
					return ret;
				}

				if ((pbuf[16] & 0xF) == sw_spd) {
					/* Switch to the specified speed */
					ret = SD_SwitchFunction(SD_CMD6_SWITCH_MODE, sw_spd, pbuf, SD_SemStruct);

					if (ret != HAL_OK) {
						return ret;
					}

					if ((pbuf[16] & 0xF) == sw_spd) {
						RTK_LOGI(TAG, "SD card changes to the specified speed mode successfully\r\n");
						if (speed == SD_SPEED_DS) {
							SDIOH_SwitchSpeed(SDIOH_CLK_DIV4, SDIOH_SD20_MODE); // 25 MHz
							card_info.bus_spd = SD_SPEED_DS;
						} else if (speed == SD_SPEED_HS) {
							SDIOH_SwitchSpeed(SDIOH_CLK_DIV2, SDIOH_SD20_MODE); // 50 MHz
							card_info.bus_spd = SD_SPEED_HS;
						}
					} else {
						RTK_LOGE(TAG, "The switch request is canceled !!\r\n");
						return HAL_ERR_UNKNOWN;
					}
				} else {
					RTK_LOGW(TAG, "The specified speed mode can't be switched !!\r\n");
					return HAL_ERR_UNKNOWN;
				}
			}
		} else {
			RTK_LOGW(TAG, "This card doesn't support the specified speed mode !!\r\n");
			return HAL_ERR_HW;
		}
	} else {
		RTK_LOGW(TAG, "This card doesn't support CMD6 and can't switch the bus speed !!\r\n");
		return HAL_ERR_HW;
	}
	return HAL_OK;
}

#else
SD_RESULT SD_SwitchBusSpeed(u8 speed, SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	SDIOH_CmdTypeDef cmd_attr;

	if ((card_info.bus_spd) == speed) {
		RTK_LOGI(TAG, "Current SD bus speed is already the specified setting.\r\n");
		return HAL_OK;
	}

	/***** CMD6 (CMD6) *****/
	if (speed == SD_SPEED_HS) {
		cmd_attr.arg = 0x03B90100;    //EXT_CSD register B9 byte: 01, high speed mode;
	} else {
		cmd_attr.arg = 0x03B90000;
	}
	cmd_attr.idx = SD_CMD_SetBusWidth;
	cmd_attr.rsp_type = SDIOH_RSP_6B;
	cmd_attr.rsp_crc_chk = ENABLE;
	cmd_attr.data_present = SDIOH_NO_DATA;
	ret = SDIOH_SendCommand(&cmd_attr, SDIOH_CMD_CPLT_TIMEOUT);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = CmdRespError(SDIOH_RESP_R1, SD_CMD_SetBusWidth);
	if (ret != HAL_OK) {
		return ret;
	}

	// Host also selects the specified mode
	if (speed == SD_SPEED_DS) {
		SDIOH_SwitchSpeed(SDIOH_CLK_DIV4, SDIOH_SD20_MODE); // 25 MHz
		card_info.bus_spd = SD_SPEED_DS;
	} else if (speed == SD_SPEED_HS) {
		SDIOH_SwitchSpeed(SDIOH_CLK_DIV2, SDIOH_SD20_MODE); // 50 MHz
		card_info.bus_spd = SD_SPEED_HS;
	}
	return HAL_OK;
}

#endif

/****************************** The following functions are for FATFS call **************************************/
/**
  *  @brief To get the current state of the SDIOH and card.
  *  @param  None.
  *  @retval  SD_OK: SD card is initialized.
  * 		SD_NODISK: SD card is removed.
  *			SD_INSERT: SD card is inserted.
  *			SD_INITERR: SD card is init fail.
  *			SD_PROTECTED: SD card is write-protected.
  *			SD_ERROR: Some errors occur.
  */
SD_RESULT SD_Status(void)
{
	return card_info.sd_status;
}

/**
  *  @brief To write blocks of data to the SD card.
  *  @param  sector: the start index of blocks to write to.
  *  @param  data: pointer to data buffer. If the address of data buffer is 32-byte alinged,
  *		the write performance would be higher.
  *  @param  count: specify how many blocks to be written.
  *  @retval  SD_OK: Success to write blocks.
  *		SD_ERROR: Fail to write blocks.
  */
SD_RESULT SD_WriteBlocks(u32 sector, const u8 *data, u32 count, SD_DMASemaStruct *SD_SemStruct)
{
	u8 res, i = 0;
	u8 *ptr;

	if ((u32)data & 0x1F) { /* Not 32-byte aligned */
		ptr = pvPortMalloc(SD_BLOCK_SIZE + 0x1F);
		if (ptr == NULL) {
			RTK_LOGE(TAG, "Allocate buffer error !!\r\n");
			return SD_ERROR;
		}
		ptr = (u8 *)(((((u32)ptr - 1) >> 5) + 1) << 5); /*next 32-byte aligned*/

		do {
			_memcpy(ptr, data + i * SD_BLOCK_SIZE, SD_BLOCK_SIZE);

			res = SD_WriteBlock(ptr, sector + i, SD_SemStruct);
			if (res != HAL_OK) {
				break;
			}

		} while (++i < count);

		vPortFree(ptr);

	} else { /*32-byte aligned */

		if (count == 1) {
			res = SD_WriteBlock((uint8_t *)data, sector, SD_SemStruct);
		} else {
			res = SD_WriteMultiBlocks((uint8_t *)data, sector, count, SD_SemStruct);
		}
	}

	if (res == HAL_OK) {
		return SD_OK;
	}

	return SD_ERROR;
}

/**
  *  @brief To read blocks of data from the SD card.
  *  @param  sector: the start index of blocks to read from.
  *  @param  data: pointer to data buffer. If the address of data buffer is 32-byte alinged,
  *		the read performance would be higher.
  *  @param  count: specify how many blocks to be read.
  *  @retval  SD_OK: Success to read blocks.
  *		SD_ERROR: Fail to read blocks.
  */
SD_RESULT SD_ReadBlocks(u32 sector, u8 *data, u32 count, SD_DMASemaStruct *SD_SemStruct)
{
	u8 res, i = 0;
	u8 *ptr;

	if ((u32)data & 0x1F) { /* Not 32-byte aligned */
		ptr = pvPortMalloc(SD_BLOCK_SIZE + 0x1F);
		if (ptr == NULL) {
			RTK_LOGE(TAG, "Allocate buffer error !!\r\n");
			return SD_ERROR;
		}
		ptr = (u8 *)(((((u32)ptr - 1) >> 5) + 1) << 5); /*next 32-byte aligned*/

		do {
			res = SD_ReadBlock(ptr, sector + i, SD_SemStruct);
			if (res != HAL_OK) {
				break;
			}

			_memcpy(data + i * SD_BLOCK_SIZE, ptr, SD_BLOCK_SIZE);

		} while (++i < count);

		vPortFree(ptr);

	} else { /*32-byte aligned */

		if (count == 1) {
			res = SD_ReadBlock(data, sector, SD_SemStruct);
		} else {
			res = SD_ReadMultiBlocks(data, sector, count, SD_SemStruct);
		}
	}

	if (res == HAL_OK) {
		return SD_OK;
	}

	return SD_ERROR;
}

/**
  *  @brief To get the capacity of the SD card.
  *  @param  sector_count: the capacity in blocks.
  *  @retval  SD_OK: Success to get capacity.
  *			SD_ERROR: Fail to get capacity.
  */
SD_RESULT SD_GetCapacity(u32 *sector_count, SD_DMASemaStruct *SD_SemStruct)
{
	if (card_info.capaticy == 0) {
#if defined(SDIO) && (SDIO == EMMC)
		u8 *EXT_CSD = pvPortMalloc(512 + 31);
		EXT_CSD = (u8 *)(((((u32)EXT_CSD - 1) >> 5) + 1) << 5); /*next 32-byte aligned*/
		SD_GetEXTCSD(EXT_CSD, SD_SemStruct);
		card_info.capaticy = (EXT_CSD[215] << 24 | EXT_CSD[214] << 16 | EXT_CSD[213] << 8 | EXT_CSD[212]) / 2;

		vPortFree(EXT_CSD);
#else
		SD_GetCSD(SD_SemStruct);
#endif
	}

	*sector_count = card_info.capaticy * (1024 / SD_BLOCK_SIZE); //capacity in block counts

	if (*sector_count) {
		return SD_OK;
	}
	return SD_ERROR;
}

/**
  *  @brief To initialize the SD memory card.
  *  @param  None.
  *  @retval  SD_OK: Initialize SD card successfully
  *			Others: Fail to initialize SD card
  */
SD_RESULT SD_Init(SD_DMASemaStruct *SD_SemStruct)
{
	u32 ret;
	u8 voltage_mismatch = 0;
	IRQn_Type IrqNum;

#if defined (ARM_CORE_CM4)
	IrqNum = SDIO_HOST_IRQ;
#elif defined (ARM_CORE_CA32)
	IrqNum = SDIO_HOST_IRQ;
#endif

	_memset(&card_info, 0, sizeof(SD_CardInfo));
	card_info.sd_status = SD_NODISK;

	/* Configure pinmux */
	SDIOH_Pinmux();

	/* Initialize SDIOH */
	SDIOH_Init(sdioh_config.sdioh_bus_width);
#if defined(SDIO) &&(SDIO == EMMC)
	card_info.sd_status = SD_INSERT;
#else
	InterruptRegister((IRQ_FUN)SD_IRQHandler, SDIO_HOST_IRQ, (u32)SD_SemStruct, 5);
	InterruptEn(IrqNum, 5);

	__DSB();
	__ISB();
	DelayUs(10); /* for ap dual core system*/
#endif
	DelayMs(100);
	card_info.sd_status = SD_INSERT;
	/* Initialize SD card */
	if (card_info.sd_status == SD_INSERT) {
		do {
			/* Card Identification */
			ret = SD_ResetCard();
			if (ret != HAL_OK) {
				break;
			}

#if defined(SDIO) && (SDIO == SD)
			ret = SD_VoltageCheck(&voltage_mismatch);
			if (ret != HAL_OK) {
				break;
			}
#endif
			ret = SD_GetOCR(voltage_mismatch);
			if (ret != HAL_OK) {
				break;
			}

			ret = SD_GetCID(SD_SemStruct);
			if (ret != HAL_OK) {
				break;
			}

			ret = SD_GetRCA();
			if (ret != HAL_OK) {
				break;
			}

			/* switch to non-initial mode */
			ret = SDIOH_InitialModeCmd(DISABLE, card_info.sig_level);
			if (ret != HAL_OK) {
				break;
			} else {
				if (card_info.sig_level == SDIOH_SIG_VOL_18) {
					card_info.bus_spd = SD_SPEED_SDR12;
				} else {
					card_info.bus_spd = SD_SPEED_DS;
				}
			}

			ret = SD_GetCSD(SD_SemStruct);
			if (ret != HAL_OK) {
				break;
			}

			ret = SD_SelectDeselect(_TRUE);
			if (ret != HAL_OK) {
				break;
			}

			if (sdioh_config.sdioh_bus_width == SDIOH_BUS_WIDTH_4BIT) {
				ret = SD_SetBusWidth(SDIOH_BUS_WIDTH_4BIT);
				if (ret != HAL_OK) {
					break;
				}
			}

			if (sdioh_config.sdioh_bus_speed == SD_SPEED_HS) {
				ret = SD_SwitchBusSpeed(SD_SPEED_HS, SD_SemStruct);
				if (ret != HAL_OK) {
					break;
				}

			}
		} while (0);

		if (ret == HAL_OK) {
			card_info.sd_status = SD_OK;
			RTK_LOGI(TAG, "SD card is initialized\r\n");
		} else {
			card_info.sd_status = SD_INITERR;
			RTK_LOGE(TAG, "Init FAIL, ret: %d\n", ret);
		}
	} else if (card_info.sd_status == SD_PROTECTED) {
		RTK_LOGE(TAG, "Card is write protected !!\r\n");

	} else if (card_info.sd_status == SD_NODISK) {
		RTK_LOGE(TAG, "Card is removed\r\n");
		return SD_INITERR;
	}

	return card_info.sd_status;
}

/**
  *  @brief To de-initialize the SDIO host controller.
  *  @param None.
  *  @retval  SD_OK: SDIO host controller is de-initialize successfully.
  */
SD_RESULT SD_DeInit(void)
{
	IRQn_Type IrqNum;

#if defined (ARM_CORE_CM4)
	IrqNum = SDIO_HOST_IRQ;
#elif defined (ARM_CORE_CA32)
	IrqNum = SDIO_HOST_IRQ;
#endif

	SDIOH_DeInit();
	RCC_PeriphClockCmd(APBPeriph_SDH, APBPeriph_SDH_CLOCK, DISABLE);
	InterruptUnRegister(IrqNum);
	InterruptDis(IrqNum);

	return SD_OK;
}

