/**
  ******************************************************************************
  * @file    rtl8721d_uart_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the UART firmware
  *             library, including the following functionalities of the Universal Asynchronous
  *             Receiver/Transmitter (UART) peripheral:
  *           - Initialization
  *           - Clear UART TX/RX FIFO
  *           - Uart Rx Path Control (disable/enable)
  *           - Baud Rate Setting
  *           - Receive/Send Data Interface
  *           - Uart RX Path Selection(high rate rx path/low power rx path)
  *           - Uart RX Path Clock Source Selection
  *           - Uart Low Power Rx Initialization
  *           - Uart Low Power Rx Monitor Function Initialization
  *           - Low Power Rx Baud Rate Setting
  *           - Uart IrDA Function Initialization
  *           - Uart Rx Byte Counter Interface
  *           - Output pin Configuration
  *           - Interrupts and flags management
  *
  *  @verbatim
  *
  *          ===================================================================
  *                                 How to use this driver
  *          ===================================================================
  *          1. Enable peripheral clock using the following functions(it is enable default)
  *              RCC_PeriphClockCmd(APBPeriph_UARTx, APBPeriph_UARTx_CLOCK, ENABLE); for UART0 and UART1
  *
  *          2. configure the UART pinmux.
  *		    Pinmux_Config(Pin_Num, PINMUX_FUNCTION_UART)
  *
  *          3. disable uart rx path(high rate Rx/low power rx)
  *
  *          4. select rx path(high rate rx/low power rx)
  *
  *          5. select rx path clock source(XTAL 40M Hz/XTAL 8M Hz/OSC 8M Hz)
  *
  *          6. clear rx fifo.
  *
  *          7. Program Word Length , Stop Bit, Parity, Hardware flow control and DMA Mode(ENABLE/DISABLE)
  *              using the UART_Init() function.
  *
  *          8. Program the Baud Rate, using function UART_SetBaud().
  *
  *          9. Enable the NVIC and the corresponding interrupt using the function
  *             UART_INTConfig() and register the uart irq handler if you need to use interrupt mode.
  *
  *          10. When using the DMA mode
  *                   - GDMA related configurations(source address/destination address/block size etc.)
  *                   - Configure the uart DMA burst size using UART_TXDMAConfig()/UART_RXDMAConfig() function
  *                   - Active the UART TX/RX DMA Request using UART_TXDMACmd()/UART_RXDMACmd() function
  *
  *          11. enable uart rx path(High Rate Rx/Low power rx)
  *
  *          12. Enable the DMA using the DMA_Cmd() function, when using DMA mode.
  *
  *          Refer to related specifications for more details about IrDA function and Low Power Rx Path(monitoring and clock
  *           swicth).
  *
  *          In order to reach higher communication baudrates, high rate rx path can be used.
  *          Low power rx can be used for saving power.
  *
  *  @endverbatim
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

/**
  * @brief  Deinitializes the UARTx peripheral registers to their default reset values.
  * @param  UARTx: where x can be 0/1/2.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_DeInit(
	UART_TypeDef *UARTx
)
{
	/*disable all interrupts*/
	UARTx->IER = 0x00;
}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to an UART_InitTypeDef structure which will be initialized.
  * @retval   None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_StructInit(UART_InitTypeDef *UART_InitStruct)
{
	/* UART_InitStruct members default value */
	UART_InitStruct->Parity         = RUART_PARITY_ENABLE;
	UART_InitStruct->ParityType     = RUART_ODD_PARITY;
	UART_InitStruct->StickParity    = RUART_STICK_PARITY_DISABLE;
	UART_InitStruct->StopBit        = RUART_STOP_BIT_1;
	UART_InitStruct->WordLen        = RUART_WLS_8BITS;
	UART_InitStruct->RxFifoTrigLevel = UART_RX_FIFOTRIG_LEVEL_1BYTES;
	UART_InitStruct->RxErReportCtrl  = UART_RX_EEROR_REPORT_ENABLE;
	UART_InitStruct->DmaModeCtrl    = ENABLE;
	UART_InitStruct->FlowControl    = DISABLE;
	UART_InitStruct->RxTimeOutCnt   = 64;
}

/**
  * @brief    Initializes the UARTx peripheral according to the specified
  *              parameters in the UART_InitStruct.
  * @param  UARTx: where x can be 0/1/2.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *              the configuration information for the specified UART peripheral.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
	u32 TempFcr;
	u32 TempRxPath;

	/*check the parameters*/
	assert_param(IS_UART_RXFIFO_LEVEL(UART_InitStruct->RxFifoTrigLevel));
	assert_param(IS_UART_RX_ERROR_REPORT(UART_InitStruct->RxErReportCtrl));

	/*disable rx path*/
	UARTx->RX_PATH_CTRL &= ~RUART_BIT_R_RST_NEWRX_N;

	/*clear rx fifo */
	UART_ClearRxFifo(UARTx);

	/* Disable all interrupts */
	UARTx->IER = 0x00;

	/* Clean Rx break signal interrupt status at initial stage.*/
	UARTx->SCR |= RUART_BIT_SCRATCH_7;

	/*get the FCR register value*/
	TempFcr = UARTx->FCR;

	/*clear the rx fifo level and rx error report feild ( FCR[7:6] & FCR[0])*/
	TempFcr &= ~(RUART_BIT_FIFO_EN | RUART_MASK_RECVTRG);

	/*set the rx fifo trigger level*/
	TempFcr |= UART_InitStruct->RxFifoTrigLevel;

	/*enable or disable rx error report*/
	TempFcr |= UART_InitStruct->RxErReportCtrl;

	UARTx->FCR = TempFcr;

	/* Configure FlowControl */
	if (UART_InitStruct->FlowControl == ENABLE) {
		UARTx->MCR |= RUART_MCL_FLOW_ENABLE;
	} else {
		UARTx->MCR &= ~ RUART_MCL_FLOW_ENABLE;
	}

	/*configure word length, stop bit, parity, parity type, stickparity*/
	UARTx->LCR = ((UART_InitStruct->WordLen) |
				  (UART_InitStruct->StopBit << 2) |
				  (UART_InitStruct->Parity << 3) |
				  (UART_InitStruct->ParityType << 4) |
				  (UART_InitStruct->StickParity << 5));

	/* disable or enable the UART DMA mode */
	if (UART_InitStruct->DmaModeCtrl != DISABLE) {
		UARTx->FCR |= RUART_BIT_DMA_MODE;
	} else {
		UARTx->FCR &= (~ RUART_BIT_DMA_MODE);
	}

	/*configure rx timeout counter*/
	TempRxPath = UARTx->RX_PATH_CTRL;
	TempRxPath &= ~(RUART_MASK_R_RXTO_THRS);
	TempRxPath |= (UART_InitStruct->RxTimeOutCnt << 16);
	UARTx->RX_PATH_CTRL = TempRxPath;
}

/**
  * @brief    get ovsr & ovsr_adj parameters for the given baudrate
  * @param  UARTx: where x can be 0/1/2.
  * @param  baudrate: the desired baudrate
  * @param  ovsr: the pointer to ovsr parameter
  * @param  ovsr_adj: the pointer to ovsr_adj parameter
  * @retval   searching status:
  *              - 1: found
  *              - 0: not found
  */
__weak HAL_ROM_TEXT_SECTION  u32
UART_BaudParaGet(u32 baudrate, u32 *ovsr, u32 *ovsr_adj)
{
	/* get baud rate parameter based on baudrate */
	UART_BaudParaGetFull(XTAL_ClkGet(), baudrate, ovsr, ovsr_adj);

	return 1;
}

/**
  * @brief    get ovsr & ovsr_adj parameters according to the given baudrate and UART IP clock.
  * @param  UARTx: where x can be 0/1/2.
  * @param  IPclk: the given UART IP clock, Unit: [ Hz ]
  * @param  baudrate: the desired baudrate, Unit: bps[ bit per second ]
  * @param  ovsr: parameter related to the desired baud rate( corresponding to STS[23:4] )
  * @param  ovsr_adj: parameter related to the desired baud rate( corresponding to SCR[26:16] )
  * @retval  None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_BaudParaGetFull(u32 IPclk, u32 baudrate, u32 *ovsr, u32 *ovsr_adj)
{
	u32 i;
	u32 Remainder;
	u32 TempAdj = 0;
	u32 TempMultly;

	/*obtain the ovsr parameter*/
	*ovsr = IPclk / baudrate;

	/*get the remainder related to the ovsr_adj parameter*/
	Remainder = IPclk % baudrate;

	/*calculate the ovsr_adj parameter*/
	for (i = 0; i < 11; i++) {
		TempAdj = TempAdj << 1;
		TempMultly = (Remainder * (12 - i));
		TempAdj |= ((TempMultly / baudrate - (TempMultly - Remainder) / baudrate) ? 1 : 0);
	}

	/*obtain the ovsr_adj parameter*/
	*ovsr_adj = TempAdj;
}

/**
  * @brief    Set Uart Baud Rate use baudrate val.
  * @param  UARTx: where x can be 0/1/2.
  * @param  BaudRate: Baud Rate Val, like 115200 (unit is HZ).
  * @retval  None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_SetBaud(UART_TypeDef *UARTx, u32 BaudRate)
{
	u32 RegValue;
	u32 Ovsr;
	u32 Ovsr_adj;

	/* get baud rate parameter based on baudrate */
	UART_BaudParaGetFull(XTAL_ClkGet(), BaudRate, &Ovsr, &Ovsr_adj);

	/* Set DLAB bit to 1 to access DLL/DLM */
	UARTx->LCR |= RUART_BIT_DLAB;

	/*Clean Rx break signal interrupt status at initial stage*/
	UARTx->SCR |= RUART_BIT_SCRATCH_7;

	/* Set OVSR(xfactor) */
	RegValue = UARTx->STS;
	RegValue &= ~(RUART_MASK_XFACTOR);
	RegValue |= ((Ovsr << 4) & RUART_MASK_XFACTOR);
	UARTx->STS = RegValue;

	/* Set OVSR_ADJ[10:0] (xfactor_adj[26:16]) */
	RegValue = UARTx->SCR;
	RegValue &= ~(RUART_MASK_XFACTOR_ADJ);
	RegValue |= ((Ovsr_adj << 16) & RUART_MASK_XFACTOR_ADJ);
	UARTx->SCR = RegValue;

	/* clear DLAB bit */
	UARTx->LCR &= ~(RUART_BIT_DLAB);

	/*rx baud rate configureation*/
	RegValue = UARTx->MON_BAUD_STS;
	RegValue &= (~RUART_MASK_R_CYCNUM_PERBIT_XTAL);
	RegValue |= Ovsr;
	UARTx->MON_BAUD_STS = RegValue;

	RegValue = UARTx->MON_BAUD_CTRL;
	RegValue &= (~RUART_MASK_R_CYCNUM_PERBIT_OSC);
	RegValue |= (Ovsr << 9);
	UARTx->MON_BAUD_CTRL = RegValue;

	RegValue = UARTx->RX_PATH_CTRL;
	RegValue &= (~RUART_MASK_RXBAUD_ADJ_10_0);
	RegValue |= (Ovsr_adj << 3);
	UARTx->RX_PATH_CTRL = RegValue;
}

/**
  * @brief    Set Uart Baud Rate use baudrate parameter Ovsr & Ovsr_adj.
  * @param  UARTx: where x can be 0/1/2.
  * @param  Ovsr: Specifies the ovsr parameter related to baud rate.
  *				This parameter is caculated by using uart IP clock and the desired baud rate
  * @param  Ovsr_adj: Specifies the ovsr_adj parameter related fractional part for the desired baud rate.
  *				This parameter is also caculated by using uart IP clock and the desired baud rate
  * @retval  None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_SetBaudExt(UART_TypeDef *UARTx, u32 Ovsr, u32 Ovsr_adj)
{
	u32 RegValue;

	/* Set DLAB bit to 1 to access DLL/DLM */
	UARTx->LCR |= RUART_BIT_DLAB;

	/*Clean Rx break signal interrupt status at initial stage*/
	UARTx->SCR |= RUART_BIT_SCRATCH_7;

	/* Set OVSR(xfactor) */
	RegValue = UARTx->STS;
	RegValue &= ~(RUART_MASK_XFACTOR);
	RegValue |= ((Ovsr << 4) & RUART_MASK_XFACTOR);
	UARTx->STS = RegValue;

	/* Set OVSR_ADJ[10:0] (xfactor_adj[26:16]) */
	RegValue = UARTx->SCR;
	RegValue &= ~(RUART_MASK_XFACTOR_ADJ);
	RegValue |= ((Ovsr_adj << 16) & RUART_MASK_XFACTOR_ADJ);
	UARTx->SCR = RegValue;

	/* clear DLAB bit */
	UARTx->LCR &= ~(RUART_BIT_DLAB);

	/*rx baud rate configureation*/
	RegValue = UARTx->MON_BAUD_STS;
	RegValue &= (~RUART_MASK_R_CYCNUM_PERBIT_XTAL);
	RegValue |= Ovsr;
	UARTx->MON_BAUD_STS = RegValue;

	RegValue = UARTx->MON_BAUD_CTRL;
	RegValue &= (~RUART_MASK_R_CYCNUM_PERBIT_OSC);
	RegValue |= (Ovsr << 9);
	UARTx->MON_BAUD_CTRL = RegValue;

	RegValue = UARTx->RX_PATH_CTRL;
	RegValue &= (~RUART_MASK_RXBAUD_ADJ_10_0);
	RegValue |= (Ovsr_adj << 3);
	UARTx->RX_PATH_CTRL = RegValue;
}

/**
  * @brief    to set the RX FIFO level to trigger RX interrupt/RTS de-assert
  * @param  UARTx: where x can be 0/1/2.
  * @param  FifoLv: This parameter can be one of the following values:
  *		 @arg UART_RX_FIFOTRIG_LEVEL_1BYTES
  *		 @arg UART_RX_FIFOTRIG_LEVEL_16BYTES
  *		 @arg UART_RX_FIFOTRIG_LEVEL_32BYTES
  *		 @arg UART_RX_FIFOTRIG_LEVEL_62BYTES
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION void
UART_SetRxLevel(UART_TypeDef *UARTx, u32 FifoLv)
{
	u32 RegValue = 0;

	assert_param(IS_UART_RXFIFO_LEVEL(FifoLv));

	RegValue = UARTx->FCR;

	RegValue &= ~RUART_MASK_RECVTRG;
	RegValue |= FifoLv;

	UARTx->FCR = RegValue;
}

/**
  * @brief    enable or disable Uart High Rate Rx Path
  * @param  UARTx: where x can be 0/1/2.
  * @param  NewState: the new state of the Rx Path.
  *              This parameter can be: ENABLE or DISABLE.
  * @note    Because the TX/RX of the IrDA transceiver are in the same module,
  *              RX fifo can also receive available data when in the process of TX.Therefore,
  *              the RX Path must be disabled when TX is going on.Namely, IrDA must work
  *              in the Half duplex mode, though normal UART IP supports Full duplex.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION void
UART_RxCmd(UART_TypeDef *UARTx, u32 NewState)
{
	if (NewState != DISABLE) {
		/*enable lp uart receiver(AmebaD merge hp rx to lp rx path)*/
		UARTx->RX_PATH_CTRL |= RUART_BIT_R_RST_NEWRX_N;
	} else {
		/*disable lp uart receiver(AmebaD merge hp rx to lp rx path)*/
		UARTx->RX_PATH_CTRL &= ~RUART_BIT_R_RST_NEWRX_N;
	}
}

/**
* @brief    check if write data to tx fifo.
* @param  UARTx: where x can be 0/1/2.
* @retval   status value:
*              - 1: tx fifo is not full and can write data to tx fifo
*              - 0: tx fifo is full
*/
__weak HAL_ROM_TEXT_SECTION  u32
UART_Writable(UART_TypeDef *UARTx)
{
	/*check if tx fifo is full or not: full return 0 and not full return 1.*/
	if (UARTx->LSR & RUART_BIT_TX_NOT_FULL) {
		return 1;
	} else {
		return 0;
	}
}

/**
* @brief    check if there is data in rx fifo.
* @param  UARTx: where x can be 0/1/2.
* @retval   status value:
*              - 1: rx fifo has valid data and can read data from rx fifo
*              - 0: rx fifo is empty
*/
__weak HAL_ROM_TEXT_SECTION  u32
UART_Readable(UART_TypeDef *UARTx)
{
	/*check if rx fifo has available data.
	available data return 1 and no available data return 0.*/
	if (UARTx->LSR & RUART_BIT_DRDY) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param  UARTx: where x can be 0/1/2.
  * @param  TxData: the data to transmit.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_CharPut(UART_TypeDef *UARTx, u8 TxData)
{
	/*write one byte to tx fifo*/
	UARTx->RBR_OR_UART_THR = TxData;
}

/**
  * @brief  Get the received data by the UARTx peripheral.
  * @param  UARTx: where x can be 0/1/2.
  * @param  pRxByte: the pointer to received data.
  * @retval   None.
  */
__weak HAL_ROM_TEXT_SECTION  void
UART_CharGet(
	UART_TypeDef *UARTx,
	u8  *pRxByte
)
{
	/*read one byte from rx fifo*/
	*pRxByte = (u8)UARTx->RBR_OR_UART_THR;
}

/**
  * @brief  Receive data from rx FIFO, poll UART_Readable.
  * @param  UARTx: selected UART peripheral, where x can be 0/1/2.
  * @param[out]  outBuf: buffer to save data read from UART FIFO.
  * @param  count: number of data to be read.
  * @retval none
  */
__weak HAL_ROM_TEXT_SECTION
void UART_ReceiveData(
	UART_TypeDef *UARTx,
	u8 *OutBuf,
	u32 Count
)
{
	u32 cnt = 0;

	for (cnt = 0; cnt < Count; cnt++) {
		while (UART_Readable(UARTx) == 0);

		*OutBuf++ = (u8)UARTx->RBR_OR_UART_THR;
	}
}

/**
  * @brief  Send data to tx FIFO, poll UART_Writable.
  * @param  UARTx: selected UART peripheral, where x can be 0/1/2.
  * @param  inBuf: buffer to be written to Tx FIFO.
  * @param  count: number of data to be written.
  * @retval none
  */
__weak HAL_ROM_TEXT_SECTION
void UART_SendData(
	UART_TypeDef *UARTx,
	u8 *InBuf,
	u32 Count
)
{
	u32 cnt = 0;

	for (cnt = 0; cnt < Count; cnt++) {
		while (UART_Writable(UARTx) == 0);

		UARTx->RBR_OR_UART_THR = *InBuf++;
	}
}

/**
  * @brief  Receive data from rx FIFO, with timeout.
  * @param  UARTx: selected UART peripheral, where x can be 0/1/2.
  * @param[out]  outBuf: buffer to save data read from UART FIFO.
  * @param  count: number of data to be read.
  * @param  Times: poll UART_Readable times before timeout.
  * @retval transfer len
  */
__weak HAL_ROM_TEXT_SECTION
u32 UART_ReceiveDataTO(
	UART_TypeDef *UARTx,
	u8 *OutBuf,
	u32 Count,
	u32 Times
)
{
	u32 cnt = 0;
	u32 polltimes = 0;

	while (1) {
		if (UART_Readable(UARTx)) {
			*(OutBuf + cnt) = (u8)UARTx->RBR_OR_UART_THR;
			cnt++;
			polltimes = 0;

			if (cnt >= Count) {
				break;
			}
		} else {
			polltimes++;

			/* time out */
			if (polltimes > Times) {
				break;
			}
		}
	}

	return (cnt);
}

/**
  * @brief  Send data to tx FIFO, with timeout.
  * @param  UARTx: selected UART peripheral, where x can be 0/1/2.
  * @param  InBuf: buffer to be written to Tx FIFO.
  * @param  Count: number of data to be written.
  * @param  Times: poll UART_Writable times before timeout.
  * @retval transfer len
  */
__weak HAL_ROM_TEXT_SECTION
u32 UART_SendDataTO(
	UART_TypeDef *UARTx,
	u8 *InBuf,
	u32 Count,
	u32 Times
)
{
	u32 cnt = 0;
	u32 polltimes = 0;

	while (1) {
		if (UART_Writable(UARTx)) {
			UARTx->RBR_OR_UART_THR = *(InBuf + cnt);
			cnt++;
			polltimes = 0;

			if (cnt >= Count) {
				break;
			}
		} else {
			polltimes++;

			/* time out */
			if (polltimes > Times) {
				break;
			}
		}
	}

	return (cnt);
}

/**
  * @brief    Clear Rx Byte Counter.
  * @param  UARTx: where x can be 0/1/2.
  * @note    clear the Rx Byte Counter(REG_RX_BYTE_CNT[15:0]) by writing 1 to
  *              clr_rx_byte_cnt(REG_RX_BYTE_CNT[16]).
  * @retval   None
  */
__weak HAL_ROM_TEXT_SECTION void
UART_RxByteCntClear(UART_TypeDef *UARTx)
{
	/*write 1 to clr_rx_byte_cnt(REG_RX_BYTE_CNT[16]) to clear Rx Byte Counter*/
	UARTx->RX_BYTE_CNT |= RUART_BIT_CLR_RX_BYTE_CNT;
}

/**
  * @brief    Get the Rx Byte Counter Value.
  * @param  UARTx: where x can be 0/1/2.
  * @note    Get the Rx Byte Counter Value By reading the REG_RX_BYTE_CNT[15:0]
  * @note    this counter will increase 1 for each reading the rx fifo, no matter rx fifo is empty or not.
  * @retval   the value of Rx Byte Counter
  */
__weak HAL_ROM_TEXT_SECTION u32
UART_RxByteCntGet(UART_TypeDef *UARTx)
{
	u32 RegValue;

	/*Read the REG_RX_BYTE_CNT register value*/
	RegValue = UARTx->RX_BYTE_CNT;

	/*Get the rx byte counter field (REG_RX_BYTE_CNT[15:0]) value*/
	RegValue &= RUART_MASK_RO_RX_BYTE_CNT;

	return RegValue;
}

/**
* @brief    enable or disable UART break contol function .
* @param  UARTx: where x can be 0/1/2.
* @param  NewState: the new state of UART Break Signal.
*              This parameter can be: ENABLE or DISABLE.
* @retval   None
*/
__weak HAL_ROM_TEXT_SECTION void
UART_BreakCtl(UART_TypeDef *UARTx, u32 NewState)
{
	u32 RegValue;

	/*get the Line Control Register(LCR) value*/
	RegValue = UARTx->LCR;

	if (NewState) {
		/*enable break control*/
		RegValue |= RUART_BIT_BRCTL;
	} else {
		/*disable break control*/
		RegValue &= (~ RUART_BIT_BRCTL);
	}

	UARTx->LCR = RegValue;
}

/**
  * @brief    Clear Rx fifo ( write 1 to FCR[1] ).
  * @param  UARTx: where x can be 0/1/2.
  * @retval   None
  */
__weak HAL_ROM_TEXT_SECTION u32
UART_ClearRxFifo(UART_TypeDef *UARTx)
{
	u32  Temp;
	u32  WaitTime = 5;

	/*write 1 to FCR[1] to clear rx fifo*/
	UARTx->FCR |= RUART_BIT_RECV_CLR;

	while (WaitTime > 0) {
		/*check Rx fifo if empty or not*/
		Temp = UARTx->LSR & RUART_BIT_DRDY;

		if (Temp == 0) {
			break;
		}

		WaitTime--;
	}

	if (Temp == 0) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}


/**
  * @brief    Clear Tx fifo ( write 1 to FCR[2] ).
  * @param  UARTx: where x can be 0/1/2.
  * @retval   None
  */
__weak HAL_ROM_TEXT_SECTION void
UART_ClearTxFifo(UART_TypeDef *UARTx)
{
	/*write 1 to FCR[2] to clear tx fifo*/
	UARTx->FCR |= RUART_BIT_XMIT_CLR;
}

/**
  * @brief    Enables or disables the specified UARTx interrupts.
  * @param  UARTx: where x can be 0/1/2.
  * @param  UART_IT: specifies the UARTx interrupt sources to be enabled or disabled.
  *          This parameter can be one or combinations of the following values:
  *            @arg RUART_BIT_ERBI:  Received Data Available interrupt
  *            @arg RUART_BIT_ETBEI: Transmitter FIFO Empty interrupt
  *            @arg RUART_BIT_ELSI:  Receiver Line Status interrupt
  *            @arg RUART_BIT_EDSSI: Modem Status interrupt
  *            @arg RUART_BIT_EMDI: Rx Path Monitor done interrupt
  *            @arg RUART_BIT_ETOI: Rx Time Out interrupt
  *            @arg RUART_BIT_ERXNDI: Rx Noise Detected interrupt
  * @param  newState: new state of the specified UARTx interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION void
UART_INTConfig(
	UART_TypeDef *UARTx,
	u32 UART_IT,
	u32 newState
)
{
	if (newState == ENABLE) {
		/* Enable the selected UARTx interrupts */
		UARTx->IER |= UART_IT;
	} else {
		/* Disable the selected UARTx interrupts */
		UARTx->IER &= (u32)~UART_IT;
	}
}

/**
  * @brief    clear the specified UARTx interrupts.
  * @param  UARTx: where x can be 0/1/2.
  * @param  UART_IT: specifies the UARTx interrupt sources to be clear.
  *          This parameter can be one or combinations of the following values:
  *            @arg RUART_BIT_RXNDICF:  Rx Noise Detected interrupt
  *            @arg RUART_BIT_MDICF:  Rx Monitor done interrupt
  *            @arg RUART_BIT_MICF:  Modem Status interrupt
  *            @arg RUART_BIT_TOICF:  Rx Time Out interrupt
  *            @arg RUART_BIT_RLSICF:   Receiver Line Status interrupt
  * @retval None
  */

__weak HAL_ROM_TEXT_SECTION void
UART_INT_Clear(
	UART_TypeDef *UARTx,
	u32 UART_IT
)
{
	/* Clear the selected UARTx interrupts */
	UARTx->ICR |= UART_IT;
}


/**
  * @brief    get the interrupt identification register value.
  * @param  UARTx: where x can be 0/1/2.
  * @note    the masks RUART_BIT_INT_PEND can be used to get the uart inetrrupt identification field value.
  * @retval   the interrupt identification register value.
  */
__weak HAL_ROM_TEXT_SECTION u32
UART_IntStatus(UART_TypeDef *UARTx)
{
	/*return the uart interrupt identification register value*/
	return UARTx->IIR;
}

/**
  * @brief    get the modem status register value.
  * @param  UARTx: where x can be 0/1/2.
  * @retval   the modem status register value.
  */
__weak HAL_ROM_TEXT_SECTION u32
UART_ModemStatusGet(UART_TypeDef *UARTx)
{
	/*return the modem status register value*/
	return UARTx->MSR;
}

/**
  * @brief    get the Line status register value.
  * @param  UARTx: where x can be 0/1/2.
  * @note    the meaning of the lower eight bits in LSR register is:
  *              - [bit 0]  RUART_BIT_DRDY:   data ready indicator.
  *              - [bit 1]  RUART_BIT_OVR_ERR:   over run.
  *              - [bit 2]  RUART_BIT_PAR_ERR:   parity error.
  *              - [bit 3]  RUART_BIT_FRM_ERR:   framing error.
  *              - [bit 4]  RUART_BIT_BREAK_INT:   break interrupt error.
  *              - [bit 5]  RUART_BIT_TX_EMPTY:   tx fifo empty indicator.
  *              - [bit 6]  RUART_BIT_TX_NOT_FULL:   tx fifo not full indicator.
  *              - [bit 7]  RUART_BIT_RXFIFO_ERR:  rx fifo error.
  *			 the meaning of the high eight bits in LSR register is:
  *              - [bit 8]  RUART_BIT_RXFIFO_INT:   rx fifo available interrupt indicator.
  *              - [bit 9]  RUART_BIT_TIMEOUT_INT:   rx timeout interrupt indicator.
  *              - [bit 10]  RUART_BIT_MONITOR_DONE_INT:   rx path monitor done interrupt indicator.
  *              - [bit 11]  RUART_BIT_MODEM_INT:   modem status pending interrupt indicator.
  *              - [bit 12]  RUART_BIT_RXND_INT:   rx noise detected interrupt indicator.
  * @retval   the Line status register value.
  */
__weak HAL_ROM_TEXT_SECTION u32
UART_LineStatusGet(UART_TypeDef *UARTx)
{
	return UARTx->LSR;
}

/**
  * @brief  wait uart tx complete.
  * @param  UARTx: where x can be 0/1/2.
  * @retval: none
  */
__weak HAL_ROM_TEXT_SECTION
void UART_WaitBusy(UART_TypeDef *UARTx, u32 PollTimes)
{
	u32 times = 0;

	/* Wait for Uart print out */
	while (1) {
		if (UARTx->LSR & RUART_BIT_TX_EMPTY) {
			break;
		}

		DelayUs(100);

		times++;

		if (times > PollTimes) {
			break;
		}
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
