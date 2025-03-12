/**
  ******************************************************************************
  * @file    ameba_loguart.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for UART LOG firmware
  *          library.
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

static LOG_UART_PORT LOG_UART_IDX_FLAG[] = {
	{1, LOGUART_BIT_TP2F_NOT_FULL, LOGUART_BIT_TP2F_EMPTY, 52125, UART_LOG_IRQ},	/* CM0 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{0, LOGUART_BIT_TP1F_NOT_FULL, LOGUART_BIT_TP1F_EMPTY, 781875, UART_LOG_IRQ},		/* CM4 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{3, LOGUART_BIT_TP4F_NOT_FULL, LOGUART_BIT_TP4F_EMPTY, 3127500, UART_LOG_IRQ},	/* CA32 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
};

/**
  * @brief  LOGUART send char
  * @param  c: send char value
  */
void LOGUART_PutChar(u8 c)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 CPUID = SYS_CPUID();

	UARTLOG->LOGUART_UART_THRx[LOG_UART_IDX_FLAG[CPUID].idx] = c;

}

/**
  * @brief  LOGUART get char
  * @param  PullMode This parameter can be one of the following values:
  *            @arg TRUE: poll line status for RX
  *            @arg FALSE: read RX register directly
  * @retval receive data
  */
u8 LOGUART_GetChar(BOOL PullMode)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

	if (PullMode) {
		while (1) {
			if (UARTLOG->LOGUART_UART_LSR & LOGUART_BIT_DRDY) {
				return UARTLOG->LOGUART_UART_RBR;
			}
		}
	} else {
		return UARTLOG->LOGUART_UART_RBR;
	}
}

/**
  * @brief    check if there is data in rx fifo.
  * @retval   status value:
  *              - 1: rx fifo has valid data and can read data from rx fifo
  *              - 0: rx fifo is empty
  */
u8 LOGUART_Readable(void)
{
	/*check if rx fifo has available data.
	available data return 1 and no available data return 0.*/
	if (LOGUART_DEV->LOGUART_UART_LSR & LOGUART_BIT_DRDY) {
		return 1;
	} else {
		return 0;
	}
}
/**
  * @brief    check if there is space in tx fifo.
  * @retval   status value:
  *              - 1: tx fifo has space and can put data into tx fifo
  *              - 0: tx fifo is empty
  */
u8 LOGUART_Ready(void)
{
	/*check if tx fifo has available space to send*/
	if (LOGUART_DEV->LOGUART_UART_LSR & LOG_UART_IDX_FLAG[2].not_full) {
		return 1;
	} else {
		return 0;
	}
}
/**
  * @brief    Set Loguart Baud Rate use baudrate val.
  * @param  UARTLOG: LOGUART device.
  * @param  BaudRate: Baud Rate Val, like 115200 (unit is HZ).
  * @retval  None
  * @note   from uart driver
  */
void LOGUART_SetBaud(LOGUART_TypeDef *UARTLOG, u32 BaudRate)
{
	u32 RegValue;
	u32 Ovsr;
	u32 Ovsr_adj;

	/* get baud rate parameter based on baudrate */
	UART_BaudParaGetFull(XTAL_ClkGet(), BaudRate, &Ovsr, &Ovsr_adj);

	/* Set DLAB bit to 1 to access DLL/DLM */
	UARTLOG->LOGUART_UART_LCR |= LOGUART_BIT_DLAB;

	/*Clean Rx break signal interrupt status at initial stage*/
	UARTLOG->LOGUART_UART_SCR |= LOGUART_BIT_SCRATCH_7;

	/* Set OVSR(xfactor) */
	RegValue = UARTLOG->LOGUART_UART_STSR;
	RegValue &= ~(LOGUART_MASK_XFACTOR);
	RegValue |= LOGUART_XFACTOR(Ovsr);
	UARTLOG->LOGUART_UART_STSR = RegValue;

	/* Set OVSR_ADJ[10:0] (xfactor_adj[26:16]) */
	RegValue = UARTLOG->LOGUART_UART_SCR;
	RegValue &= ~(LOGUART_MASK_XFACTOR_ADJ);
	RegValue |= LOGUART_XFACTOR_ADJ(Ovsr_adj);
	UARTLOG->LOGUART_UART_SCR = RegValue;

	/* clear DLAB bit */
	UARTLOG->LOGUART_UART_LCR &= ~(LOGUART_BIT_DLAB);

	/*rx baud rate configureation*/
	RegValue = UARTLOG->LOGUART_UART_REG_MON_BAUD_STS;
	RegValue &= (~LOGUART_MASK_R_CYCNUM_PERBIT_XTAL);
	RegValue |= Ovsr;
	UARTLOG->LOGUART_UART_REG_MON_BAUD_STS = RegValue;

	RegValue = UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL;
	RegValue &= (~LOGUART_MASK_R_CYCNUM_PERBIT_OSC);
	RegValue |= LOGUART_R_CYCNUM_PERBIT_OSC(Ovsr);
	UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL = RegValue;

	RegValue = UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL;
	RegValue &= (~LOGUART_MASK_RXBAUD_ADJ_10_0);
	RegValue |= LOGUART_RXBAUD_ADJ_10_0(Ovsr_adj);
	UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL = RegValue;
}

/**
  * @brief  wait log uart tx is complete.
  * @retval: none
  * @note   this function is to wait until tx data is completely push from tx fifo and all the tx fifo is empty.
  */
void LOGUART_WaitTxComplete(void)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 lsr;

	/* Wait for LogUart print out */
	while (1) {
		lsr = UARTLOG->LOGUART_UART_LSR;

		if ((lsr & LOGUART_BIT_TP1F_EMPTY) && (lsr & LOGUART_BIT_TP2F_EMPTY) && (lsr & LOGUART_BIT_TP3F_EMPTY)  && \
			(lsr & LOGUART_BIT_TP4F_EMPTY) && (!(lsr & LOGUART_BIT_RPF_DRDY)) && (lsr & LOGUART_BIT_TX_EMPTY)) {
			break;
		}

		DelayUs(100);
	}

	/* delay at least 12 cycles of one bit time to make sure the last data is completely out of tx fifo, 4800bps is 2.5ms, 115200bps is 104us, 1.5Mbps is 8us */
	if (SYSCFG_OTP_LowBaudLog()) {
		DelayUs(105);	
	} else {
		DelayUs(8);
	}
}

/**
  * @brief  Enables or Disables the AGG function.
  * @param  UARTLOG: LOGUART device.
  * @param  NewState: new state of AGG enable bit
  *          This parameter can be:ENABLE or DISABLE
  * @retval None
  */
void LOGUART_AGGCmd(LOGUART_TypeDef *UARTLOG, u32 NewState)
{
	if (NewState != DISABLE) {
		/* set the AGG enable bit */
		UARTLOG->LOGUART_UART_AGGC |= LOGUART_BIT_AGG_EN;
	} else {
		/* reset the AGG enable bit */
		UARTLOG->LOGUART_UART_AGGC &= ~LOGUART_BIT_AGG_EN;
	}
}


/**
  * @brief  Configure the path open bits.
  * @param  UARTLOG: LOGUART device.
  * @param  PathIndex: specifies the path number to be configured.
  *              - LOGUART_PATH_INDEX_1: path1 open bit, NP log
  *              - LOGUART_PATH_INDEX_2: path2 open bit, LP log
  *              - LOGUART_PATH_INDEX_3: path3 open bit, BT log
  *              - LOGUART_PATH_INDEX_4: path4 open bit, AP log
  *              - LOGUART_PATH_INDEX_5: path5 open bit, BT FW log
  * @param  NewState: new state of path enable bit
  *          This parameter can be:ENABLE or DISABLE
  * @retval None
  */
void LOGUART_AGGPathCmd(LOGUART_TypeDef *UARTLOG, u8 PathIndex, u32 NewState)
{
	u32 TempAGGC;

	/* Check the parameters */
	assert_param(IS_LOGUART_PATH_INDEX(PathIndex));

	/*Get the AGGC register value*/
	TempAGGC = UARTLOG->LOGUART_UART_AGGC;

	switch (PathIndex) {
	case LOGUART_PATH_INDEX_1:
		/*configure the path1 enable bit for NP log*/
		if (NewState != DISABLE) {
			TempAGGC |= LOGUART_BIT_TP1_EN;
		} else {
			TempAGGC &= (~ LOGUART_BIT_TP1_EN);
		}
		break;
	case LOGUART_PATH_INDEX_2:
		/*configure the path2 enable bit for LP log*/
		if (NewState != DISABLE) {
			TempAGGC |= LOGUART_BIT_TP2_EN;
		} else {
			TempAGGC &= (~ LOGUART_BIT_TP2_EN);
		}
		break;
	case LOGUART_PATH_INDEX_3:
		/*configure the path3 enable bit for BT log*/
		if (NewState != DISABLE) {
			TempAGGC |= LOGUART_BIT_TP3_EN;
		} else {
			TempAGGC &= (~ LOGUART_BIT_TP3_EN);
		}
		break;
	case LOGUART_PATH_INDEX_4:
		/*configure the path4 enable bit for AP log*/
		if (NewState != DISABLE) {
			TempAGGC |= LOGUART_BIT_TP4_EN;
		} else {
			TempAGGC &= (~ LOGUART_BIT_TP4_EN);
		}
		break;
	case LOGUART_PATH_INDEX_5:
		/*configure the path5 enable bit for BT FW log*/
		if (NewState != DISABLE) {
			TempAGGC |= LOGUART_BIT_RP_EN;
		} else {
			TempAGGC &= (~ LOGUART_BIT_RP_EN);
		}
		break;
	default:
		break;
	}

	UARTLOG->LOGUART_UART_AGGC = TempAGGC;

	/*delay 1us for register sync before configuration takes effect*/
	DelayUs(1);
}

/**
  * @brief  Set LOGUART IMR
  * @param  SetValue: specifies the LOGUART interrupt sources to be enabled or disabled.
  *          This parameter can be one or combinations of the following values:
  *            @arg LOGUART_BIT_ERBI:  Received Data Available interrupt
  *            @arg LOGUART_BIT_ELSI:  Receiver Line Status interrupt
  *            @arg LOGUART_BIT_ETOI: Rx Time Out interrupt
  *            @arg LOGUART_BIT_EDSSI: Modem Status Interrupt
  *            @arg LOGUART_TX_EMPTY_PATH_1_INTR: Tx Path 1 Transmitter FIFO Empty interrupt
  *            @arg LOGUART_TX_EMPTY_PATH_2_INTR: Tx Path 2 Transmitter FIFO Empty interrupt
  *            @arg LOGUART_TX_EMPTY_PATH_3_INTR: Tx Path 3 Transmitter FIFO Empty interrupt
  *            @arg LOGUART_TX_EMPTY_PATH_4_INTR: Tx Path 4 Transmitter FIFO Empty interrupt
  * @retval none
  */
void LOGUART_INTConfig(LOGUART_TypeDef *UARTLOG, u32 UART_IT, u32 newState)
{
	if (newState == ENABLE) {
		/* Enable the selected UARTx interrupts */
		UARTLOG->LOGUART_UART_IER |= UART_IT;
	} else {
		/* Disable the selected UARTx interrupts */
		UARTLOG->LOGUART_UART_IER &= (u32)~UART_IT;
	}
}

/**
  * @brief  Clear the specified LOGUART pending flag
  *          This parameter can be one or combinations of the following values:
  *            @arg LOGUART_BIT_RPRLSICF: Relay path line status clear flag
  *            @arg LOGUART_BIT_TOICF Timeout interrupt clear flag
  *            @arg LOGUART_BIT_RLSICF:  Receiver line statue interrupt clear flag
  *            @arg LOGUART_BIT_MDICF:  Monitor done interrupt clear flag
  * @retval none
  */
void LOGUART_INTClear(LOGUART_TypeDef *UARTLOG, u32 UART_IT)
{
	UARTLOG->LOGUART_UART_ICR |= UART_IT;
}

/**
  * @brief    get the line status register value.
  * @param  UARTLOG: LOGUART device.
  * @retval   the line status register value.
  */
u32 LOGUART_GetStatus(LOGUART_TypeDef *UARTLOG)
{
	return UARTLOG->LOGUART_UART_LSR;
}

/**
  * @brief    Clear Rx fifo ( write 1 to FCR[1] ).
  * @param  LOGUART: LOGUART device.
  * @retval
  *              - 1: clear rx fifo success
  *              - 0: clear rx fifo fail
  * @note   from uart driver
  */
u32 LOGUART_ClearRxFifo(LOGUART_TypeDef *UARTLOG)
{
	u32  Temp;
	u32  WaitTime = 5;

	/*write 1 to FCR[1] to clear rx fifo*/
	UARTLOG->LOGUART_UART_FCR |= LOGUART_BIT_RECV_CLR;

	while (WaitTime > 0) {
		/*check Rx fifo if empty or not*/
		Temp = UARTLOG->LOGUART_UART_LSR & LOGUART_BIT_DRDY;

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

void LOGUART_BT_SendData(u8 *InBuf, u32 Count)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 cnt = 0;
	u32 CounterIndex = 0;

	for (cnt = 0; cnt < Count; cnt++) {
		while (1) {
			CounterIndex++;

			/* timeout value is the same with path1(km4) timeout*/
			if (CounterIndex >= 781875) {
				break;
			}

			if (UARTLOG->LOGUART_UART_LSR & LOGUART_BIT_TP3F_NOT_FULL) {
				break;
			}
		}

		UARTLOG->LOGUART_UART_THRx[2] = *InBuf++;
	}
}

void LOGUART_Relay_StructInit(LOGUART_Relay_InitTypeDef *LOGUART_Relay_InitStruct)
{
	LOGUART_Relay_InitStruct->WordLen = LOGUART_WLS_8BITS;     // word length select
	LOGUART_Relay_InitStruct->StopBit = LOGUART_STOP_BIT_1;     // 0 -> 1 stop bit, 1 -> 2 stop bit
	LOGUART_Relay_InitStruct->Parity = DISABLE;      // parity check enable
	LOGUART_Relay_InitStruct->ParityType = LOGUART_ODD_PARITY;
	LOGUART_Relay_InitStruct->StickParity = DISABLE;
	LOGUART_Relay_InitStruct->RxErReportCtrl = LOGUART_RX_EEROR_REPORT_ENABLE;
}

void LOGUART_Relay_SetFormat(LOGUART_TypeDef *UARTLOG, LOGUART_Relay_InitTypeDef *LOGUART_Relay_InitStruct)
{
	u32 TempFcr;

	/*configure relay path: word length, stop bit, parity, parity type, stickparity*/
	UARTLOG->LOGUART_UART_RP_LCR = ((LOGUART_Relay_InitStruct->WordLen) |
									(LOGUART_RP_STB(LOGUART_Relay_InitStruct->StopBit)) |
									(LOGUART_RP_PEN(LOGUART_Relay_InitStruct->Parity)) |
									(LOGUART_RP_EPS(LOGUART_Relay_InitStruct->ParityType)) |
									(LOGUART_RP_STKP(LOGUART_Relay_InitStruct->StickParity)));

	/*enable or disable relay path error report*/
	TempFcr = UARTLOG->LOGUART_UART_FCR;
	TempFcr &= ~LOGUART_BIT_RP_FIFO_EN;
	TempFcr |= LOGUART_RP_FIFO_EN(LOGUART_Relay_InitStruct->RxErReportCtrl);
	TempFcr &= ~LOGUART_BIT_RECV_CLR; //to avoid clearing rx fifo

	UARTLOG->LOGUART_UART_FCR = TempFcr;
}

void LOGUART_Relay_SetBaud(LOGUART_TypeDef *UARTLOG, u32 RPBaudRate)
{
	u32 RegValue;
	u32 Ovsr;
	u32 Ovsr_adj;

	/* get baud rate parameter based on baudrate */
	UART_BaudParaGetFull(XTAL_ClkGet(), RPBaudRate, &Ovsr, &Ovsr_adj);
	/* Set DLAB bit to 1 to access DLL/DLM */
	UARTLOG->LOGUART_UART_RP_LCR |= LOGUART_BIT_RP_DLAB;

	/*rx baud rate configureation*/
	RegValue = UARTLOG->LOGUART_UART_RP_LCR;
	RegValue &= (~LOGUART_MASK_RP_RXBAUD_19_0);
	RegValue |= LOGUART_RP_RXBAUD_19_0(Ovsr);
	UARTLOG->LOGUART_UART_RP_LCR = RegValue;

	RegValue = UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL;
	RegValue &= (~LOGUART_MASK_RP_RXBAUD_ADJ_10_0);
	RegValue |= LOGUART_RP_RXBAUD_ADJ_10_0(Ovsr);
	UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL = RegValue;

	/* clear DLAB bit */
	UARTLOG->LOGUART_UART_RP_LCR &= ~(LOGUART_BIT_RP_DLAB);
}

u32 LOGUART_Relay_ClearRxFifo(LOGUART_TypeDef *UARTLOG)
{
	u32  Temp;
	u32  WaitTime = 5;

	/*write 1 to FCR[13] to clear rx fifo*/
	u32 TempFcr;
	TempFcr = UARTLOG->LOGUART_UART_FCR;
	TempFcr |= LOGUART_BIT_RP_RECV_CLR;
	TempFcr &= ~LOGUART_BIT_RECV_CLR; //to avoid clearing rx fifo

	UARTLOG->LOGUART_UART_FCR = TempFcr;


	while (WaitTime > 0) {
		/*check Rx fifo if empty or not*/
		Temp = UARTLOG->LOGUART_UART_LSR & LOGUART_BIT_RPF_DRDY;

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

void LOGUART_Relay_RxCmd(LOGUART_TypeDef *UARTLOG, u32 NewState)
{
	if (NewState != DISABLE) {
		UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL |= LOGUART_BIT_R_RST_RP_RX_N;
	} else {
		UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL &= ~LOGUART_BIT_R_RST_RP_RX_N;
	}
}

u32 LOGUART_GetIMR(void)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

	return UARTLOG->LOGUART_UART_IER;
}

void LOGUART_SetIMR(u32 SetValue)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

	UARTLOG->LOGUART_UART_IER = SetValue;
}

#ifdef CONFIG_PM
void LOGUART_Suspend(void)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

	LOGUART_WaitTxComplete();
	LOGUART_ClearRxFifo(UARTLOG);
}
#endif

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
