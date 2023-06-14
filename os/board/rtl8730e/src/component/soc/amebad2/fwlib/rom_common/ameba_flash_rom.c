/**
  ******************************************************************************
  * @file    rtl8721d_flash_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the SPIC peripheral:
  *           - Initialization with selectable BitMode of 1/2/4 bitmode
  *           - User Mode/Auto mode configuration
  *           - Read/Program/Erase configuration
  *           - Calibration configuration
  *           - Deep Power Down
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
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
HAL_ROM_DATA_SECTION
u32 SPIC_CALIB_PATTERN[2] = {
	0x96969999,
	0xFC66CC3F,
};

HAL_ROM_BSS_SECTION
FLASH_InitTypeDef flash_init_para;

/**
* @brief  This function is used to switch between User mode and Auto mode.
* @param  enable: This parameter can be: ENABLE or DISABLE.
*			@arg ENABLE: Set SPIC to user mode
*			@arg DISABLE: Set SPIC to auto mode
* @retval none
*/
HAL_ROM_TEXT_SECTION
static void FLASH_UserModeEn(u8 enable)
{
	SPIC_TypeDef *spi_flash = SPIC;

	/* Wait spic busy done before switch mode */
	FLASH_WaitBusy(WAIT_SPIC_BUSY);

	if (enable) {
		spi_flash->CTRLR0 |= BIT_USER_MODE;
	} else {
		spi_flash->CTRLR0 &= ~BIT_USER_MODE;
	}
}

/**
* @brief  This function is used to send Rx command to flash to get status register or flash id.
* @param  cmd: command that need to be sent.
* @param  read_len: the number of bytes that will be read by SPIC after sending cmd.
* @param  read_data: pointer to a byte array which is used to save datas received.
* @note This function is only used for rx status/flash id ,not used for read flash data.
*		Only work in OneBitMode.
* @retval none
*/
HAL_ROM_TEXT_SECTION
__WEAK void FLASH_RxCmd(u8 cmd, u32 read_len, u8 *read_data)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 rx_num = 0;
	u32 value;

	/* Enter user mode */
	FLASH_UserModeEn(ENABLE);

	/* Set OneBit Mode */
	FLASH_SetSpiMode(&flash_init_para, SpicOneBitMode);

	/* Set CMD phase: 1 byte, ADDR phase: 0 byte */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(0);

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	spi_flash->RX_NDF = RX_NDF(read_len);
	spi_flash->TX_NDF = 0;

	/* set CTRLR0: RX_mode and one bit mode for data & addr*/
	spi_flash->CTRLR0 = ((spi_flash->CTRLR0 & 0xFFF0FFFF) | TMOD(3));

	/* set flash_cmd: write cmd to fifo */
	spi_flash->DR[0].BYTE = cmd;

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* read data */
	rx_num = 0;
	while (rx_num < read_len) {
		if (spi_flash->SR & BIT_RFNE) {
			read_data[rx_num] = spi_flash->DR[0].BYTE;
			rx_num += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Recover */
	FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);

	/* Exit user mode */
	FLASH_UserModeEn(DISABLE);

}

/**
* @brief  This function is used to read data from flash in User Mode.
* @param  cmd: Read data command.
* @param  StartAddr: Start address in flash from which SPIC wants to read.
* @param  read_len: the number of bytes that SPIC wants to read.
* @param  read_data: pointer to a byte array which is used to save received data.
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_RxData(u8 cmd, u32 StartAddr, u32 read_len, u8 *read_data)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u8 rd_cmd = cmd;
	u32 rx_num = 0;
	u32 value;

	if (rd_cmd == 0) {
		rd_cmd = flash_init_para.FLASH_cur_cmd;
	}

	/* Enter user mode */
	FLASH_UserModeEn(ENABLE);

	/* set CTRLR0: RX_mode */
	spi_flash->CTRLR0 |= TMOD(3);

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	spi_flash->RX_NDF = RX_NDF(read_len);
	spi_flash->TX_NDF = 0;

	/* Set ADDR length */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	if (flash_init_para.FLASH_dum_en) {
		value |= USER_ADDR_LENGTH(flash_init_para.FLASH_addr_phase_len + 1);
	} else {
		value |= USER_ADDR_LENGTH(flash_init_para.FLASH_addr_phase_len);
	}

	spi_flash->USER_LENGTH = value;

	/* Set target slave */
	spi_flash->SER = 1;

	/* set flash_cmd: write cmd & address to fifo & addr is MSB */
	spi_flash->DR[0].BYTE = rd_cmd;
	if (flash_init_para.FLASH_addr_phase_len == ADDR_4_BYTE) {
		spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF000000) >> 24);
	}

	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF0000) >> 16);
	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF00) >> 8);
	spi_flash->DR[0].BYTE = (u8)(StartAddr & 0xFF);

	/* Push dummy byte*/
	if (flash_init_para.FLASH_dum_en) {
		spi_flash->DR[0].BYTE = 0x0;
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	rx_num = 0;
	while (rx_num < read_len) {
		if (spi_flash->SR & BIT_RFNE) {
			read_data[rx_num] = spi_flash->DR[0].BYTE;
			rx_num += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy(WAIT_TRANS_COMPLETE);

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
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_WaitBusy(u32 WaitType)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 BusyCheck = 0;
	u8 status = 0;

	do {
		if (WaitType == WAIT_SPIC_BUSY) {
			BusyCheck = (spi_flash->SR & BIT_BUSY);

		} else if (WaitType == WAIT_TRANS_COMPLETE) {
			/* When transfer completes, SSIENR.SPIC_EN are cleared by HW automatically. */
			BusyCheck = (spi_flash->SSIENR & BIT_SPIC_EN);

		} else if (WaitType == WAIT_FLASH_BUSY) {
			FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_status, 1, &status);

			BusyCheck = (status & flash_init_para.FLASH_Busy_bit);
		} else if (WaitType == WAIT_WRITE_EN) {
			FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_status, 1, &status);

			/* write enable ok & flash not busy */
			BusyCheck = (!(status & flash_init_para.FLASH_WLE_bit)) || (status & flash_init_para.FLASH_Busy_bit);
		}

		if (!BusyCheck) {
			break;
		}
	} while (1);
}

/**
* @brief  This function is used to send WREN(Write Enable) cmd to set WLE bit
*		in flash status register.
* @param  none
* @note This function must be used prior to every Page Program,
	Sector/Block/Chip Erase, Write Status Register instruction.
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_WriteEn(void)
{
	/* Wait for flash busy done */
	FLASH_WaitBusy(WAIT_FLASH_BUSY);

	/* Set flash_cmd: WREN to FIFO */
	FLASH_TxCmd(flash_init_para.FLASH_cmd_wr_en, 0, 0);

	/* Wait for write enable done*/
	FLASH_WaitBusy(WAIT_WRITE_EN);
}

/**
* @brief  This function is used to send Flash Instruction with Data Phase.
* @param  cmd: The command need to be sent.
* @param  DataPhaseLen: The length of Data phase.
* @param  pData: pointer to byte array which are to be sent in Data phase.
* @note
*		- This function works in OneBitMode and User mode.
*		- This function can only be used to tx cmd(WREN,WRSR,DP,RDP,ERASE etc.),not used to tx data.
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_TxCmd(u8 cmd, u8 DataPhaseLen, u8 *pData)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 tx_num = 0;
	u32 addr_length;
	u32 ctrl0;
	u32 value;

	/* Enter user mode */
	FLASH_UserModeEn(ENABLE);

	/* backup bitmode & addr_len */
	addr_length = spi_flash->USER_LENGTH;
	ctrl0 = spi_flash->CTRLR0;

	/* set CTRLR0: TX mode and one bit mode */
	spi_flash->CTRLR0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* Set ADDR length: 0 byte, skip ADDR phase */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(0);

	/* Set TX_NDF: frame number of Tx data. */
	spi_flash->RX_NDF = 0;
	spi_flash->TX_NDF = TX_NDF(DataPhaseLen);

	/* set flash_cmd: write cmd to fifo */
	spi_flash->DR[0].BYTE = cmd;

	/* fill data */
	for (tx_num = 0; tx_num < DataPhaseLen; tx_num++) {
		spi_flash->DR[0].BYTE = (u8) * (pData + tx_num);
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Restore bitmode & addr_len */
	spi_flash->USER_LENGTH = addr_length;
	spi_flash->CTRLR0 = ctrl0;

	/* Call FLASH_WaitBusy(WAIT_FLASH_BUSY) after this function:
		1) wait flash busy done (wip=0)
		2) restore SPIC to auto mode */

}

/**
  * @brief  This function is used to write data to flash in OneBitMode and User Mode.
  * @param  StartAddr: Start address in flash from which SPIC writes.
  * @param  DataPhaseLen: the number of bytes that SPIC sends in Data Phase.
  * @param  pData: pointer to a byte array that is to be sent.
  * @note
  *		- RXI312_SPIC Tx data frames number can be 0~2^24-1 bytes.
  *			TX_NDF can prevent TX FIFO underflow, when TX FIFO empty and Tx num<TX_NDF, clock
  *			would be gated and CS keep low.
  *			TX_NDF can't prevent TX FIFO overflow, so driver should check SR.TFNF before push data to FIFO.
  * @retval none
  */
HAL_ROM_TEXT_SECTION
__WEAK void FLASH_TxData(u32 StartAddr, u32 DataPhaseLen, u8 *pData)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 tx_num = 0;
	u32 ctrl0;
	u32 value;

	assert_param(((StartAddr & 0xFF) + DataPhaseLen) <= 0x100); /* addr should page(256) align */

	/* write enable cmd */
	FLASH_WriteEn();

	/* Enter user mode */
	FLASH_UserModeEn(ENABLE);

	/* set CTRLR0: TX mode and one bit mode */
	ctrl0 = spi_flash->CTRLR0;
	spi_flash->CTRLR0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* Set ADDR length */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(flash_init_para.FLASH_addr_phase_len);

	/* Set TX_NDF: frame number of Tx data. */
	spi_flash->RX_NDF = 0;
	spi_flash->TX_NDF = TX_NDF(DataPhaseLen);

	/* set flash_cmd: write cmd & address to fifo & addr is MSB */
	spi_flash->DR[0].BYTE = FLASH_CMD_PP;
	if (flash_init_para.FLASH_addr_phase_len == ADDR_4_BYTE) {
		spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF000000) >> 24);
	}

	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF0000) >> 16);
	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF00) >> 8);
	spi_flash->DR[0].BYTE = (u8)(StartAddr & 0xFF);

	/* write some data to fill the fifo before set SSIENR. */
	tx_num = 0;
	while (tx_num < DataPhaseLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = (u8) * (pData + tx_num);
			tx_num += 1;
		} else {
			break;
		}
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* write the remaining data into fifo */
	while (tx_num < DataPhaseLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = (u8) * (pData + tx_num);
			tx_num += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Restore bitmode */
	spi_flash->CTRLR0 = ctrl0;

	/* The result of calling this function:
		1) wait flash busy done (wip=0)
		2) restore SPIC to auto mode */
	FLASH_WaitBusy(WAIT_FLASH_BUSY);

}

/**
  * @brief  FLASH_SetStatus used to set register status. FLASH_WriteEn & FLASH_WaitBusy
  *  		are included in this function to avoid hardfault when TxCmd in XIP
  * @param    Cmd: command to be sent
  * @param    Len: the number of bytes to be sent after Cmd
  * @param    Status: pointer to byte array to be sent
  * @retval     none
  */
HAL_ROM_TEXT_SECTION
__WEAK void FLASH_SetStatus(u8 Cmd, u32 Len, u8 *Status)
{
	FLASH_WriteEn();
	FLASH_TxCmd(Cmd, Len, Status);
	FLASH_WaitBusy(WAIT_FLASH_BUSY);
}

/**
  * @brief  FLASH_SetStatusBits set or clear status bits., used to set protect bit or quad enable bit
  * @param    SetBits: 16bits valid, SetBits[7:0] is status1 & SetBits[15:8] is status2
  * @param    NewState: ENABLE/DISABLE
  * @retval none
  */
HAL_ROM_TEXT_SECTION
__WEAK void FLASH_SetStatusBits(u32 SetBits, u32 NewState)
{
	u8 status[2];
	u8 status1_mask = (u8)(SetBits & 0xFF);
	u8 status2_mask = (u8)((SetBits >> 8) & 0xFF);
	u8 StatusLen = 1;

	/* status1 */
	FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_status, 1, &status[0]);
	if (NewState == ENABLE) {
		status[0] |=  status1_mask;
	} else {
		status[0] &=  ~status1_mask;
	}

	/* check if status2 */
	if (flash_init_para.FLASH_Status2_exist) {
		StatusLen = 2;
		FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_status2, 1, &status[1]);

		if (NewState == ENABLE) {
			status[1] |=  status2_mask;
		} else {
			status[1] &=  ~status2_mask;
		}
	}

	if (!flash_init_para.FLASH_cmd_wr_status2) {
		FLASH_WriteEn();
		FLASH_TxCmd(flash_init_para.FLASH_cmd_wr_status, StatusLen, status);
		FLASH_WaitBusy(WAIT_FLASH_BUSY);
	} else {
		FLASH_WriteEn();
		FLASH_TxCmd(flash_init_para.FLASH_cmd_wr_status, 1, &status[0]);
		FLASH_WaitBusy(WAIT_FLASH_BUSY);

		FLASH_WriteEn();
		FLASH_TxCmd(flash_init_para.FLASH_cmd_wr_status2, 1, &status[1]);
		FLASH_WaitBusy(WAIT_FLASH_BUSY);
	}
}

/**
  * @brief  This function is used to erase flash.
  * @param EraseType: can be one of the following  parameters:
  		@arg EraseChip: Erase the whole chip.
  		@arg EraseBlock: Erase specified block(64KB)
  		@arg EraseSector: Erase specified sector(4KB)
  * @param    Address should 4 byte align.The block/sector which
  * 		the address in will be erased.
  * @retval none
  */
HAL_ROM_TEXT_SECTION
__WEAK void FLASH_Erase(u32 EraseType, u32 Address)
{
	u8 Addr[4];
	u8 len = 0;

	assert_param(EraseType <= EraseSector); /* max len is 2 */

	if (flash_init_para.FLASH_addr_phase_len == ADDR_4_BYTE) {
		len = 4;
		Addr[0] = (Address >> 24) & 0xFF;
		Addr[1] = (Address >> 16) & 0xFF;
		Addr[2] = (Address >> 8) & 0xFF;
		Addr[3] = Address & 0xFF;
	} else {
		len = 3;
		Addr[0] = (Address >> 16) & 0xFF;
		Addr[1] = (Address >> 8) & 0xFF;
		Addr[2] = Address & 0xFF;
	}

	FLASH_WriteEn();

	if (EraseType == EraseChip) {
		FLASH_TxCmd(flash_init_para.FLASH_cmd_chip_e, 0, 0);
	} else if (EraseType == EraseBlock) {
		FLASH_TxCmd(flash_init_para.FLASH_cmd_block_e, len, Addr);
	} else if (EraseType == EraseSector) {
		FLASH_TxCmd(flash_init_para.FLASH_cmd_sector_e, len, Addr);
	}

	FLASH_WaitBusy(WAIT_FLASH_BUSY);
}

/**
  * @brief  This function is used to make flash enter/release from deep power down mode.
  * @param NewState: ENABLE or DISABLE
  * @retval none
  * @note Release from power-down will take the time duration of tRES1 before the device will resume normal
  *    operation and other instructions are accepted. The value of tRES1 is as following:
  *    Winbond: 3us, MXIC: 8.8/20/30/100 us, Micron: 30us, GD: 20/30 us
  */
__weak HAL_ROM_TEXT_SECTION
void FLASH_DeepPowerDown(u32 NewState)
{
	if (NewState == ENABLE) {
		if (SYSCFG_OTP_FlashDSleepEn()) {
			FLASH_WriteEn();
			FLASH_TxCmd(flash_init_para.FLASH_cmd_pwdn, 0, 0);
			FLASH_UserModeEn(DISABLE);
		}
	} else {
		FLASH_TxCmd(flash_init_para.FLASH_cmd_pwdn_release, 0, 0);
		DelayUs(100);
		FLASH_WaitBusy(WAIT_FLASH_BUSY);
	}
}

/**
  * @brief  This function is used to configure SPIC according to parameters in FLASH_InitStruct.
  * @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
  *         structure that contains the configuration information for SPIC.
  * @retval none
  */
HAL_ROM_TEXT_SECTION
static void FLASH_Config(FLASH_InitTypeDef *FLASH_InitStruct)
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
	spi_flash->USER_LENGTH = Value32 | USER_ADDR_LENGTH(FLASH_InitStruct->FLASH_addr_phase_len) | USER_CMD_LENGHT(
								 1);

	/* set dummy byte */
	spi_flash->DUM_BYTE = FLASH_InitStruct->FLASH_dum_byte;

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
HAL_ROM_TEXT_SECTION
void FLASH_SetSpiMode(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode)
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

	/* set user read dummy cycle.
		If DUM_EN, push one dummy byte in addr phase manually and reduce one byte in dummy phase */
	if (FLASH_InitStruct->FLASH_dum_en) {
		if (SpicBitMode == SpicQuadIOBitMode) {
			dclen -= QUAD_DUM_CYCLE_NUM * 2 * FLASH_InitStruct->FLASH_baud_rate;
		} else if (SpicBitMode == SpicDualIOBitMode) {
			dclen -= DUAL_DUM_CYCLE_NUM * 2 * FLASH_InitStruct->FLASH_baud_rate;
		}
	}

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
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_DREAD;
		break;
	case SpicDualIOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_dual_io_valid_cmd;
		ctrl0 |= (ADDR_CH(1) | DATA_CH(1));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_2READ;
		break;
	case SpicQuadOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_quad_o_valid_cmd;
		ctrl0 |= (ADDR_CH(0) | DATA_CH(2));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_QREAD;
		break;
	case SpicQuadIOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_quad_io_valid_cmd;
		ctrl0 |= (ADDR_CH(2) | DATA_CH(2));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_4READ;
		break;
	default:
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;
	}

	if (FLASH_InitStruct->FLASH_dum_en) {
		Value32 |= BIT_DUM_EN;
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
HAL_ROM_TEXT_SECTION
void FLASH_StructInit_Micron(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_MICRON;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	FLASH_InitStruct->FLASH_QuadEn_bit = 0;
	FLASH_InitStruct->FLASH_Busy_bit = BIT(0);
	FLASH_InitStruct->FLASH_WLE_bit = BIT(1);
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = 0;

	/* set Micron rd_dummy_cyle based on baudrate, default 100MHz */
	/* FLASH_BAUD_100M */
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x05;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x05;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = 0x09;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0x09;
	/* FLASH_BAUD_50M */
	//FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x01;
	//FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x03;
	/* FLASH_BAUD_25M */
	//FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x01;
	//FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x01;
	/* < 25MHz */
	//FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x01;
	//FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x01;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_wr_dual_i = 0xA2;
	FLASH_InitStruct->FLASH_wr_dual_ii = 0xD2;
	FLASH_InitStruct->FLASH_rd_dual_io = 0xBB;
	FLASH_InitStruct->FLASH_rd_dual_o = 0x3B;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_wr_quad_i = 0x32;
	FLASH_InitStruct->FLASH_wr_quad_ii = 0x12;
	FLASH_InitStruct->FLASH_rd_quad_io = 0xEB;
	FLASH_InitStruct->FLASH_rd_quad_o = 0x6B;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = 0x06;
	FLASH_InitStruct->FLASH_cmd_rd_id = 0x9F;
	FLASH_InitStruct->FLASH_cmd_rd_status = 0x05;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = 0x01;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0xC7;
	FLASH_InitStruct->FLASH_cmd_block_e = 0xD8;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0x20;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0xAB;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0xB9;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = 0xBB;
	FLASH_InitStruct->FLASH_addr_phase_len = ADDR_3_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE;
	FLASH_InitStruct->FLASH_dum_byte = 0x0;
}

/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to MXIC Spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_StructInit_MXIC(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_MXIC;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* mxic  QSPI En: is bit6 */
	FLASH_InitStruct->FLASH_QuadEn_bit = BIT(6);
	FLASH_InitStruct->FLASH_Busy_bit = BIT(0);	//WIP
	FLASH_InitStruct->FLASH_WLE_bit = BIT(1);	//WLE
	FLASH_InitStruct->FLASH_Status2_exist = 0;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x08;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x04;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = 0x08;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0x06;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_wr_dual_i = 0;
	FLASH_InitStruct->FLASH_wr_dual_ii = 0;
	FLASH_InitStruct->FLASH_rd_dual_io = 0xBB;
	FLASH_InitStruct->FLASH_rd_dual_o = 0x3B;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_wr_quad_i = 0;
	FLASH_InitStruct->FLASH_wr_quad_ii = 0x38;
	FLASH_InitStruct->FLASH_rd_quad_io = 0xEB;
	FLASH_InitStruct->FLASH_rd_quad_o = 0x6B;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = 0x06;
	FLASH_InitStruct->FLASH_cmd_rd_id = 0x9F;
	FLASH_InitStruct->FLASH_cmd_rd_status = 0x05;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_wr_status = 0x01;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0x60;
	FLASH_InitStruct->FLASH_cmd_block_e = 0xD8;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0x20;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0xAB;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0xB9;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = 0xBB;
	FLASH_InitStruct->FLASH_addr_phase_len = ADDR_3_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE;
	FLASH_InitStruct->FLASH_dum_byte = 0x0;
}

/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to Gigadevice spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_StructInit_GD(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_GD;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* winbond QSPI En: is status2 bit1, so is status bit9*/
	FLASH_InitStruct->FLASH_QuadEn_bit = BIT(9);
	FLASH_InitStruct->FLASH_Busy_bit = BIT(0);//WIP
	FLASH_InitStruct->FLASH_WLE_bit = BIT(1);//WLE
	FLASH_InitStruct->FLASH_Status2_exist = 1;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = 0x08;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = 0x04;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = 0x08;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = 0x06;


	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_wr_dual_i = 0;
	FLASH_InitStruct->FLASH_wr_dual_ii = 0;
	FLASH_InitStruct->FLASH_rd_dual_io = 0xBB;
	FLASH_InitStruct->FLASH_rd_dual_o = 0x3B;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_wr_quad_i = 0;
	FLASH_InitStruct->FLASH_wr_quad_ii = 0x32;
	FLASH_InitStruct->FLASH_rd_quad_io = 0xEB;
	FLASH_InitStruct->FLASH_rd_quad_o = 0x6B;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = 0x06;
	FLASH_InitStruct->FLASH_cmd_rd_id = 0x9F;
	FLASH_InitStruct->FLASH_cmd_rd_status = 0x05;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0x35;
	FLASH_InitStruct->FLASH_cmd_wr_status = 0x01;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = 0x60;
	FLASH_InitStruct->FLASH_cmd_block_e = 0xD8;
	FLASH_InitStruct->FLASH_cmd_sector_e = 0x20;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = 0xAB;
	FLASH_InitStruct->FLASH_cmd_pwdn = 0xB9;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = 0xBB;
	FLASH_InitStruct->FLASH_addr_phase_len = ADDR_3_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE;
	FLASH_InitStruct->FLASH_dum_byte = 0x0;
}

/**
* @brief  Initializes the parameters in the FLASH_InitTypeDef with
*		its default values according to WINBOND_W25Q256FV_Spec.
* @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
*		structure which will be initialized.
* @retval none
*/
HAL_ROM_TEXT_SECTION
void FLASH_StructInit(FLASH_InitTypeDef *FLASH_InitStruct)
{
	FLASH_InitStruct->FLASH_Id = FLASH_ID_WINBOND;
	FLASH_InitStruct->FLASH_baud_rate = 1;

	/* status bit define */
	/* winbond QSPI En: is status2 bit1, so is status bit9*/
	FLASH_InitStruct->FLASH_QuadEn_bit = BIT(9);
	FLASH_InitStruct->FLASH_Busy_bit = FLASH_STATUS_BUSY;
	FLASH_InitStruct->FLASH_WLE_bit = FLASH_STATUS_WLE;
	FLASH_InitStruct->FLASH_Status2_exist = 1;

	/* calibration data */
	FLASH_InitStruct->FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[0] = 0;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[1] = FLASH_DM_CYCLE_2O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[2] = FLASH_DM_CYCLE_2IO;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[3] = FLASH_DM_CYCLE_4O;
	FLASH_InitStruct->FLASH_rd_dummy_cyle[4] = FLASH_DM_CYCLE_4IO;

	/* set 2bit mode cmd */
	FLASH_InitStruct->FLASH_wr_dual_i = 0;
	FLASH_InitStruct->FLASH_wr_dual_ii = 0;
	FLASH_InitStruct->FLASH_rd_dual_io = FLASH_CMD_2READ;
	FLASH_InitStruct->FLASH_rd_dual_o = FLASH_CMD_DREAD;
	FLASH_InitStruct->FALSH_dual_o_valid_cmd = BIT_RD_DUAL_I;
	FLASH_InitStruct->FALSH_dual_io_valid_cmd = BIT_RD_DUAL_IO;

	/* set 4bit mode cmd */
	FLASH_InitStruct->FLASH_wr_quad_i = 0;
	FLASH_InitStruct->FLASH_wr_quad_ii = FLASH_CMD_4PP;
	FLASH_InitStruct->FLASH_rd_quad_io = FLASH_CMD_4READ;
	FLASH_InitStruct->FLASH_rd_quad_o = FLASH_CMD_QREAD;
	FLASH_InitStruct->FALSH_quad_o_valid_cmd = BIT_RD_QUAD_O;
	FLASH_InitStruct->FALSH_quad_io_valid_cmd = BIT_RD_QUAD_IO;

	/* other flash commnad set */
	FLASH_InitStruct->FLASH_cmd_wr_en = FLASH_CMD_WREN;
	FLASH_InitStruct->FLASH_cmd_rd_id = FLASH_CMD_RDID;
	FLASH_InitStruct->FLASH_cmd_rd_status = FLASH_CMD_RDSR;
	FLASH_InitStruct->FLASH_cmd_rd_status2 = 0x35;//FLASH_CMD_RDSR2;
	FLASH_InitStruct->FLASH_cmd_wr_status = FLASH_CMD_WRSR;
	FLASH_InitStruct->FLASH_cmd_wr_status2 = 0;
	FLASH_InitStruct->FLASH_cmd_chip_e = FLASH_CMD_CE;
	FLASH_InitStruct->FLASH_cmd_block_e = FLASH_CMD_BE;
	FLASH_InitStruct->FLASH_cmd_sector_e = FLASH_CMD_SE;
	FLASH_InitStruct->FLASH_cmd_pwdn_release = FLASH_CMD_RDP;
	FLASH_InitStruct->FLASH_cmd_pwdn = FLASH_CMD_DP;

	/* new calibration */
	FLASH_InitStruct->phase_shift_idx = 0;

	FLASH_InitStruct->FLASH_rd_fast_single = 0xBB;
	FLASH_InitStruct->FLASH_addr_phase_len = ADDR_3_BYTE;
	FLASH_InitStruct->FLASH_dum_en = DISABLE;
	FLASH_InitStruct->FLASH_dum_byte = 0x0;
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
HAL_ROM_TEXT_SECTION
u8 FLASH_Init(u8 SpicBitMode)
{
	/* config fixed parameters */
	FLASH_Config(&flash_init_para);

	/* Set SPI Mode */
	flash_init_para.FLASH_cur_bitmode = SpicBitMode;
	FLASH_SetSpiMode(&flash_init_para, SpicBitMode);

	return _TRUE;
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
