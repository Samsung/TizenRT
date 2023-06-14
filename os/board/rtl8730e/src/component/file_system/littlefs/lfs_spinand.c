/**
  ******************************************************************************
  * @file    rtl8721d_spi_nand_rom.c
  * @author
  * @version V1.0.0
  * @date    2020-10-22
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the SPIC peripheral:
  *           - Initialization with selectable BitMode of 1/2/4 bitmode
  *           - User Mode/Auto mode configuration
  *           - Read/Program/Erase configuration
  *           - Calibration configuration
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"

/*
  * @If the system designs for signal channel not support multi-channel connection,
  *    @SI connects to spi_sin[0] and SO connects to spi_sout[0].
  * @Else if supporting multi-channel communication,
  *    @bidirectional pins of {HOLDn, WPn, SO, SI} connect to input ports of spi_sin[3:0]
  *    @and output ports of spi_sout[3:0]
  */

/**
* @brief  This function is used to switch between User mode and Auto mode.
* @param  enable: This parameter can be: ENABLE or DISABLE.
*			@arg ENABLE: Set SPIC to user mode
*			@arg DISABLE: Set SPIC to auto mode
* @retval none
*/

static void FLASH_UserModeEn(u8 enable)
{
	SPIC_TypeDef *spi_flash = SPIC;

	/* Wait spic busy done before switch mode */
	NAND_WaitBusy(WAIT_SPIC_BUSY);

	if (enable) {
		spi_flash->CTRLR0 |= BIT_USER_MODE;
	} else {
		spi_flash->CTRLR0 &= ~BIT_USER_MODE;
	}
}

/**
* @brief  This function is used to send Rx command to flash to get status register or flash id.
* @param  cmd: command that need to be sent.
* @param  addr_len: The length of Addr phase.
* @param  paddr: pointer to a byte array that is to be sent.
* @param  read_len: the number of bytes that will be read by SPIC after sending addr.
* @param  read_data: pointer to a byte array which is used to save datas received.
* @note This function is only used for rx status/flash id ,not used for read flash data.
*		Only work in OneBitMode.
* @retval none
*/

void NAND_RxCmd(u8 cmd, u8 addr_len, u8 *paddr, u32 read_len, u8 *read_data)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 idx;
	u32 value;

	/* Enter User Mode */
	FLASH_UserModeEn(ENABLE);

	/* Set OneBit Mode */
	NAND_SetSpiMode(&flash_init_para, SpicOneBitMode);

	/*Get Feature CMD do not have dummy cycle after address phase*/
	if (cmd == flash_init_para.FLASH_cmd_rd_status) {
		spi_flash->USER_LENGTH &= ~MASK_USER_RD_DUMMY_LENGTH;
	}

	/* Set CMD phase: 1 byte and ADDR phase */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(addr_len);

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	spi_flash->RX_NDF = RX_NDF(read_len);
	spi_flash->TX_NDF = 0;

	/* set CTRLR0: RX_mode and one bit mode for data & addr*/
	spi_flash->CTRLR0 = ((spi_flash->CTRLR0 & 0xFFF0FFFF) | TMOD(3));

	/* set flash_cmd: write cmd to fifo */
	spi_flash->DR[0].BYTE = cmd;

	/* fill addr */
	for (idx = 0; idx < addr_len; idx++) {
		spi_flash->DR[0].BYTE = paddr[idx];
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* read data */
	idx = 0;
	while (idx < read_len) {
		if (spi_flash->SR & BIT_RFNE) {
			read_data[idx] = spi_flash->DR[0].BYTE;
			idx += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	NAND_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Recover */
	NAND_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);

	/* Exit user mode */
	FLASH_UserModeEn(DISABLE);
}


/**
* @brief  This function is used to wait until SPIC/Flash Busy done or WLE bit set.
* @param  WaitType:  wait type, which can be one of the following parameters:
*			@arg WAIT_SPIC_BUSY: wait until SPIC transmission done.
*			@arg WAIT_TRANS_COMPLETE: wait until user transfer done.
*			@arg WAIT_FLASH_BUSY: wait until Flash busy done.
*			@arg WAIT_WRITE_EN: wait until flash status WLE(Write Enbale Latch) bit is set.
* @retval  zero when waiting status of SPIC, or status of Flash if not timeout, otherwise the retval is 0xFF
* @note
*		-if nand is still busy when polling count is 0x200000, timeout error occured.
*			the max frequency of SPI is 100MHz, Get Status use 3Byte * 10ns = 240ns, and 0x200000 *240ns is 500ms
*			500ms is enough to wait Nand return status(Linux driver TO time is 400ms, and CPU running time is not count)
*			Contact with FAE of Dosilicon, beyond the max read/program/erase time, they will return error info to SPIC
*/

u8 NAND_WaitBusy(u32 WaitType)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 BusyCheck = 0;
	u8 status = 0;
	u32 i = 0;

	do {
		if (WaitType == WAIT_SPIC_BUSY) {
			BusyCheck = (spi_flash->SR & BIT_BUSY);

		} else if (WaitType == WAIT_TRANS_COMPLETE) {
			/* When transfer completes, SSIENR.SPIC_EN are cleared by HW automatically. */
			BusyCheck = (spi_flash->SSIENR & BIT_SPIC_EN);

		} else if (WaitType == WAIT_FLASH_BUSY) {
			i++;
			status = NAND_GetStatus(NAND_REG_STATUS);

			BusyCheck = (status & flash_init_para.FLASH_Busy_bit);
		} else if (WaitType == WAIT_WRITE_EN) {
			i++;
			status = NAND_GetStatus(NAND_REG_STATUS);

			/* write enable ok & flash not busy */
			BusyCheck = (!(status & flash_init_para.FLASH_WLE_bit)) || (status & flash_init_para.FLASH_Busy_bit);
		}

		if (!BusyCheck) {
			break;
		}

		if (i > 0x200000) {
			status = 0xFF;
			break;
		}
	} while (1);
	return status;
}

/**
* @brief  This function is used to send WREN(Write Enable) cmd to set WLE bit in flash status register.
* @param  none
* @note   This function must be used prior to every Program execute,Block Erase, OTP program/OTP protection.
* @retval none
*/

void NAND_WriteEn(void)
{
	/* Wait for flash busy done */
	NAND_WaitBusy(WAIT_FLASH_BUSY);

	/* Set flash_cmd: WREN to FIFO */
	NAND_TxCmd(flash_init_para.FLASH_cmd_wr_en, 0, NULL, 0, NULL);

	/* Wait for write enable done*/
	NAND_WaitBusy(WAIT_WRITE_EN);
}

/**
  * @brief  This function is used to transfer data to flash in OneBitMode and User Mode.
  * @param	cmd: The command need to be sent.
  * @param  AddrLen: The length of Addr phase.
  * @param  pAddr: pointer to a byte array that is to be sent.
  * @param  DataLen: the number of bytes that SPIC sends in Data Phase.
  * @param  pData: pointer to a byte array that is to be sent.
  * @note
  *		- RXI312_SPIC Tx data frames number can be 0~2^24-1 bytes.
  *			TX_NDF can prevent TX FIFO underflow, when TX FIFO empty and Tx num<TX_NDF, clock
  *			would be gated and CS keep low.
  *			TX_NDF can't prevent TX FIFO overflow, so driver should check SR.TFNF before push data to FIFO.
  * @retval none
  */

void NAND_TxCmd(u8 cmd, u8 AddrLen, u8 *pAddr, u32 DataLen, u8 *pData)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 tx_num = 0;
	u32 addr_length = spi_flash->USER_LENGTH;
	u32 ctrl0 = spi_flash->CTRLR0;
	u32 value;

	/* Enter user mode */
	FLASH_UserModeEn(ENABLE);

	/* set CTRLR0: TX mode and one bit mode */
	spi_flash->CTRLR0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* Set ADDR length */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(AddrLen);

	/* Set TX_NDF: frame number of Tx data. */
	spi_flash->RX_NDF = 0;
	spi_flash->TX_NDF = TX_NDF(DataLen);

	/* set flash_cmd: write cmd & address to fifo & addr is MSB */
	spi_flash->DR[0].BYTE = cmd;

	for (tx_num = 0; tx_num < AddrLen; tx_num++) {
		spi_flash->DR[0].BYTE = pAddr[tx_num];
	}

	/* we can not set ssienr=1 when FIFO empty */
	/* Pre-load data before enabling, but there are just 16 - 4 = 12 bytes fifo afer cmd+addr */
	tx_num = 0;
	while (tx_num < DataLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = pData[tx_num];
			tx_num += 1;
		} else {
			/* Here mean Transmit FIFO is Full. */
			break;
		}
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* Put the remain data into transmit FIFO */
	while (tx_num < DataLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = pData[tx_num];
			tx_num += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	NAND_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Restore bitmode & addr_len */
	spi_flash->USER_LENGTH = addr_length;
	spi_flash->CTRLR0 = ctrl0;
}


/**
  * @brief  FLASH_GetStatus used to get register status.
  * @param    FeatureID: the address of status register
  * @retval     the value of the status register
  */

u8 NAND_GetStatus(u8 FeatureID)
{
	u8 status;
	NAND_RxCmd(flash_init_para.FLASH_cmd_rd_status, 1, &FeatureID, 1, &status);
	return status;
}

/**
  * @brief  FLASH_SetStatus used to set register status. NAND_WaitBusy
  *  		are included in this function to avoid hardfault when TxCmd in XIP
  * @param    FeatureID: the address of status register
  * @param    Status: pointer to byte array to be sent
  * @retval     none
  */

void NAND_SetStatus(u8 FeatureID, u8 Status)
{
	NAND_TxCmd(flash_init_para.FLASH_cmd_wr_status, 1, &FeatureID, 1, &Status);
	NAND_WaitBusy(WAIT_FLASH_BUSY);
}

/**
  * @brief  FLASH_SetStatusBits set or clear status bits., used to set protect bit or quad enable bit
  * @param    FeatureID: the address of status register
  * @param    SetBits: bit mask in the status register
  * @param    NewState: ENABLE/DISABLE
  * @retval none
  */

void NAND_SetStatusBits(u8 FeatureID, u8 SetBits, u8 NewState)
{
	u8 status = NAND_GetStatus(FeatureID);
	u8 status_mask = (u8)(SetBits & 0xFF);

	if (NewState == ENABLE) {
		status |=  status_mask;
	} else {
		status &=  ~status_mask;
	}

	NAND_SetStatus(FeatureID, status);
}

/**
  * @brief  This function is used to erase flash. EraseBlock: Erase specified block(128KB)
  * @param    PageAddr: should 4 byte align. The block which the address in will be erased.
  * @retval status: Success:0 or erase Fail: Others.(timeout is 0xFF)
  */

u8 NAND_Erase(u32 PageAddr)
{
	u8 Addr[3];
	u8 status;
	Addr[0] = (u8)((PageAddr >> 16) & 0xFF);
	Addr[1] = (u8)((PageAddr >> 8) & 0xFF);
	Addr[2] = (u8)(PageAddr & 0xFF);

	NAND_WriteEn();

	/* Block Erase Sequence is CMD + PA23-16 + PA15-8 + PA7-0. */
	NAND_TxCmd(flash_init_para.FLASH_cmd_block_e, 3, Addr, 0, NULL);

	/* polling WEL and busy done */
	status = NAND_WaitBusy(WAIT_FLASH_BUSY);

	if (status & flash_init_para.FLASH_EFail_bit) {
		return status;
	} else {
		return 0;
	}
}

/**
  * @brief  This function is used to read data from nand array to Nand Cache.
  * @param    PageAddr: which page will be read
  * @retval status: NAND_STATUS_ECC_NO_BITFLIPS:0 or Need to check: Others.(timeout is 0xFF)
  * @note
  * 	 if status is 0 means NAND_STATUS_ECC_NO_BITFLIPS, ortherwise need confirm spi nand datasheet.
  */

u8 NAND_Page_Read_ArrayToCache(u32 PageAddr)
{
	u8 Addr[3];
	u8 status;
	Addr[0] = (u8)((PageAddr >> 16) & 0xFF);
	Addr[1] = (u8)((PageAddr >> 8) & 0xFF);
	Addr[2] = (u8)(PageAddr & 0xFF);

	/* Page read Sequence is CMD + PA23-16 + PA15-8 + PA7-0. */
	NAND_TxCmd(flash_init_para.FLASH_cmd_page_read, 3, Addr, 0, NULL);

	status = NAND_WaitBusy(WAIT_FLASH_BUSY);
	/*if status is 0 means NAND_STATUS_ECC_NO_BITFLIPS, ortherwise need confirm spi nand datasheet.*/
	return status;
}

/**
  * @brief  This function is used to read data from cache
  * @param    ByteAddr: the offset of the page
  * @param    ByteLen: the number of bytes to be read
  * @param	 pData: pointer to a byte array which is used to save received data.
  * @retval none
  */

void NAND_Page_Read_FromCache(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData)
{
	NAND_RxData(Cmd, ByteAddr, ByteLen, pData);
}

/**
  * @brief  This function is used to Load data to Nand Cache
  * @param    cmd: The command need to be sent, Can be NAND_CMD_QPP_RANDOM/NAND_CMD_QPP/NAND_CMD_PP_RANDOM/NAND_CMD_PP
  * @param    ByteAddr: the offset of the page
  * @param    ByteLen: the number of bytes to be transfered
  * @param	 pData: pointer to a byte array that is to be sent.
  * @retval status: Success:1 or Program Fail: Others.
  * @note
  * 	  When cur bitmode is 4IO/4O, cmd can be NAND_CMD_QPP_RANDOM/NAND_CMD_QPP/NAND_CMD_PP_RANDOM/NAND_CMD_PP,
  *				or cmd shall be NAND_CMD_PP_RANDOM/NAND_CMD_PP
  */

void NAND_Page_Write_Data_Xfer(u8 Cmd, u32 ByteAddr, u32 ByteLen, u8 *pData)
{
	/*WEL bit shall be set, ortherwise Nand will ignore this cmd*/
	NAND_WriteEn();
	NAND_TxData(Cmd, ByteAddr, ByteLen, pData);
}

/**
  * @brief  This function is used to program Page
  * @param    PageAddr: which page will be program
  * @retval status: Success:0 or Program Fail: Others.(timeout is 0xFF)
  */

u8 NAND_Page_Write_Program_Execute(u32 PageAddr)
{
	u8 Addr[3];
	u8 status;
	Addr[0] = (u8)((PageAddr >> 16) & 0xFF);
	Addr[1] = (u8)((PageAddr >> 8) & 0xFF);
	Addr[2] = (u8)(PageAddr & 0xFF);

	/* Program Execution Sequence is CMD + PA23-16 + PA15-8 + PA7-0. */
	NAND_TxCmd(flash_init_para.FLASH_cmd_page_write, 3, Addr, 0, NULL);

	/* polling WEL and busy done */
	status = NAND_WaitBusy(WAIT_FLASH_BUSY);

	if (status & flash_init_para.FLASH_PFail_bit) {
		return status;
	} else {
		return 0;
	}
}


/**
  * @brief  This function is used to configure SPIC according to parameters in FLASH_InitStruct.
  * @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
  *         structure that contains the configuration information for SPIC.
  * @retval none
  */

static void SPIC_Config(FLASH_InitTypeDef *FLASH_InitStruct)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 Value32;

	/* Disable SPI_FLASH User Mode */
	spi_flash->SSIENR = 0;

	/* set baud rate */
	spi_flash->BAUDR = SCKDV(flash_init_para.FLASH_baud_rate);

	/* spi slave select */
	spi_flash->SER = BIT_SER;

	/* set 2bit mode cmd */
	spi_flash->READ_FAST_SINGLE = FLASH_InitStruct->FLASH_rd_fast_single;
	spi_flash->READ_DUAL_ADDR_DATA = FLASH_InitStruct->FLASH_rd_dual_io;
	spi_flash->READ_DUAL_DATA = FLASH_InitStruct->FLASH_rd_dual_o;

	/* set 4bit mode cmd */
	spi_flash->READ_QUAD_ADDR_DATA = FLASH_InitStruct->FLASH_rd_quad_io;
	spi_flash->READ_QUAD_DATA = FLASH_InitStruct->FLASH_rd_quad_o;

	/* disable auto write */
	spi_flash->WRITE_SIGNLE = 0;
	spi_flash->WRITE_ENABLE = 0;
	spi_flash->WRITE_DUAL_DATA = 0;
	spi_flash->WRITE_DUAL_ADDR_DATA = 0;
	spi_flash->WRITE_QUAD_DATA = 0;
	spi_flash->WRITE_QUAD_ADDR_DATA = 0;

	/* ctrl2 */
	spi_flash->CTRLR2 |= BIT_SO_DNUM; /*if we are use multi channel we should set it to 1 */
	spi_flash->CTRLR2 &= ~(BIT_WPN_SET | BIT_WPN_DNUM);/* bit1 output control */

	/* set CMD/ADDR phase length */
	Value32 = spi_flash->AUTO_LENGTH & ~MASK_AUTO_ADDR_LENGTH;
	spi_flash->AUTO_LENGTH = Value32 | AUTO_ADDR_LENGTH(FLASH_InitStruct->FLASH_addr_phase_len);

	Value32 = spi_flash->USER_LENGTH & ~(MASK_USER_ADDR_LENGTH | MASK_USER_CMD_LENGHT);
	spi_flash->USER_LENGTH = Value32 | USER_ADDR_LENGTH(FLASH_InitStruct->FLASH_addr_phase_len) | USER_CMD_LENGHT(1);

	/*SPI Nand Flash read random value from floating pin as dummy byte */
	/* set dummy byte but don't ues it*/
	spi_flash->DUM_BYTE = FLASH_InitStruct->FLASH_dum_byte;

	spi_flash->DEVICE_INFO = PAGE_SIZE(FLASH_InitStruct->FLASH_pagesize_main_bit_exp) | BIT_NAND_FLASH;

	Value32 = spi_flash->READ_STATUS & ~(MASK_RD_ST_CMD_1ST_BYTE | MASK_RD_ST_CMD_2ND_BYTE | MASK_ST_CMD_LEN);
	Value32 |= RD_ST_CMD_1ST_BYTE(NAND_CMD_RDSR) | RD_ST_CMD_2ND_BYTE(NAND_REG_STATUS) | ST_CMD_LEN(2);
	spi_flash->READ_STATUS = Value32;

	Value32 = spi_flash->PAGE_READ & ~(MASK_PAGE_RD_CMD | MASK_PAGE_RD_CH | MASK_PAGE_RD_ADDR_LEN);
	Value32 |= PAGE_RD_CMD(NAND_CMD_PAGERD) | PAGE_RD_CH(0) | PAGE_RD_ADDR_LEN(3);
	spi_flash->PAGE_READ = Value32;
}

/**
* @brief  This function is used to set SPI mode.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*         structure that contains the configuration information for SPIC.
* @param SpicBitMode: SPI mode to be configured.
*		This parameter can be one of the following parameters:
*		@arg SpicOneBitMode
*		@arg SpicDualOBitMode
*		@arg SpicDualIOBitMode
*		@arg SpicQuadOBitMode
*		@arg SpicQuadIOBitMode
* @retval none
*/

void NAND_SetSpiMode(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 Value32;
	u32 ctrl0 = 0;
	u16 dclen = FLASH_InitStruct->FLASH_rd_dummy_cyle[SpicBitMode] * 2 * FLASH_InitStruct->FLASH_baud_rate;

	/* Disable SPI_FLASH User Mode */
	spi_flash->SSIENR = 0;

	/* set auto read dummy cycle and line delay */
	Value32 = spi_flash->AUTO_LENGTH & ~(MASK_RD_DUMMY_LENGTH | MASK_IN_PHYSICAL_CYC);
	spi_flash->AUTO_LENGTH = Value32 | RD_DUMMY_LENGTH(dclen) | IN_PHYSICAL_CYC(FLASH_InitStruct->FLASH_rd_sample_phase);

	/* set user read dummy cycle, USER_LENGTH will reuse IN_PHYSICAL_CYC in AUTO_LENGTH */
	Value32 = spi_flash->USER_LENGTH & ~MASK_USER_RD_DUMMY_LENGTH;
	spi_flash->USER_LENGTH = Value32 | USER_RD_DUMMY_LENGTH(dclen);

	/* clear multi channel first */
	ctrl0 = spi_flash->CTRLR0 & ~(CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* clear dual & quad bit mode */
	Value32 = spi_flash->VALID_CMD & ~(SPIC_VALID_CMD_MASK);

	switch (SpicBitMode) {
	case SpicDualOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_dual_o_valid_cmd;
		ctrl0 |= (ADDR_CH(0) | DATA_CH(1));
		FLASH_InitStruct->FLASH_cur_cmd = NAND_CMD_DREAD;
		break;
	case SpicDualIOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_dual_io_valid_cmd;
		ctrl0 |= (ADDR_CH(1) | DATA_CH(1));
		FLASH_InitStruct->FLASH_cur_cmd = NAND_CMD_2READ;
		break;
	case SpicQuadOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_quad_o_valid_cmd;
		ctrl0 |= (ADDR_CH(0) | DATA_CH(2));
		FLASH_InitStruct->FLASH_cur_cmd = NAND_CMD_QREAD;
		break;
	case SpicQuadIOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_quad_io_valid_cmd;
		ctrl0 |= (ADDR_CH(2) | DATA_CH(2));
		FLASH_InitStruct->FLASH_cur_cmd = NAND_CMD_4READ;
		break;
	default:
		FLASH_InitStruct->FLASH_cur_cmd = NAND_CMD_READ;
	}

	spi_flash->VALID_CMD = Value32;
	spi_flash->CTRLR0 = ctrl0;
}

/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to Micron Spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NAND_StructInit_Micron(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_MICRON;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* micron QSPI is enable by default, and cannot be disabled!! */
	/* WP#, HOLD# are enabled by default, and can be disabled by set bit1 of feature 0xA0*/
	FLASH_InitStruct->FLASH_QuadEn_bit = 0;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = NAND_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = NAND_DM_CYCLE_4IO;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = NAND_CMD_2READ;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = NAND_CMD_4READ;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 3);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to MXIC Spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NAND_StructInit_MXIC(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_MXIC;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* mxic  QE bit is bit0 of feature 0xB0, QSPI is disabled by default */
	/* WP#, HOLD# are enabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = NAND_CFG_QUAD_ENABLE;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = 0;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = 0;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = 0;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = 0;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to Gigadevice spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NAND_StructInit_GD(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_GD;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* gigadevice QE bit is bit0 of feature 0xB0, QSPI is disabled by default */
	/* WP#, HOLD# are enabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = NAND_CFG_QUAD_ENABLE;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = 0;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = 0;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = 0;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = 0;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;

	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to Winbond spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/

void NAND_StructInit(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_WINBOND;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* winbond QSPI is enable by default, set Bit1 of feature 0xA0 to disable QSPI*/
	/* WP#, HOLD# are disabled by default*/
	FLASH_InitStruct->FLASH_QuadEn_bit = 0;
	FLASH_InitStruct->FLASH_Busy_bit = NAND_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = NAND_STATUS_WEL;
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = NAND_DM_CYCLE_READ;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = NAND_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = NAND_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = NAND_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = NAND_DM_CYCLE_4IO;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_rd_dual_io = NAND_CMD_2READ;
	FLASH_InitStruct->FLASH_rd_dual_o = NAND_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_rd_quad_io = NAND_CMD_4READ;
	FLASH_InitStruct->FLASH_rd_quad_o = NAND_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = NAND_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = NAND_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = NAND_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = NAND_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0;
	FLASH_InitStruct->FLASH_cmd_block_e = NAND_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = NAND_CMD_FREAD;
	FLASH_InitStruct->FLASH_addr_phase_len = NAND_COL_ADDR_2_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE; /*FLASH_dum_en is used to control PRM function in Nor Flash.*/
	FLASH_InitStruct->FLASH_dum_byte = 0;

	/* basic page rd/wr operation */
	FLASH_InitStruct->FLASH_cmd_page_read = NAND_CMD_PAGERD;
	FLASH_InitStruct->FLASH_cmd_page_write = NAND_CMD_PROMEXEC;
	FLASH_InitStruct->FLASH_EFail_bit = NAND_STATUS_ERASE_FAILED;
	FLASH_InitStruct->FLASH_PFail_bit = NAND_STATUS_PROG_FAILED;
	FLASH_InitStruct->FLASH_ECC_bit = BIT_OFFSET_LEN_MASK_32(4, 2);

	/*The default state of the internal ECC is enabled. */
	FLASH_InitStruct->FLASH_ECC_EN_bit = NAND_CFG_ECC_ENABLE;
}


/**
* @brief  Init SPIC in specified SPI mode
* @param SpicBitMode: SPI mode to be configured.
*		This parameter can be one of the following parameters:
*		@arg SpicOneBitMode
*		@arg SpicDualOBitMode
*		@arg SpicDualIOBitMode
*		@arg SpicQuadOBitMode
*		@arg SpicQuadIOBitMode
* @retval 1
*/

u8 NAND_Init(u8 SpicBitMode)
{
	/* config fixed parameters */
	SPIC_Config(&flash_init_para);

	/* Set SPI Mode */
	flash_init_para.FLASH_cur_bitmode = SpicBitMode;
	NAND_SetSpiMode(&flash_init_para, SpicBitMode);

	return _TRUE;
}
/******************* (C) COPYRIGHT 2020 Realtek Semiconductor *****END OF FILE****/
