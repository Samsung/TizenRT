/**
  ******************************************************************************
  * @file    rtl8721d_loguart_rom.c
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

HAL_ROM_DATA_SECTION
static LOG_UART_PORT LOG_UART_IDX_FLAG[] = {
	{1, LOGUART_BIT_TP2F_NOT_FULL, LOGUART_BIT_TP2F_EMPTY, 52125, UART_LOG_IRQ},	/* CM0 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{0, LOGUART_BIT_TP1F_NOT_FULL, LOGUART_BIT_TP1F_EMPTY, 781875, UART_LOG_IRQ},		/* CM4 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{3, LOGUART_BIT_TP4F_NOT_FULL, LOGUART_BIT_TP4F_EMPTY, 3127500, UART_LOG_IRQ},	/* CA7 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
};
/**
  * @brief  Fills each LOGUART_InitTypeDef member with its default value.
  * @param  LOGUART_InitStruct: pointer to a LOGUART_InitTypeDef structure which will be
  *         initialized.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void LOGUART_StructInit(LOGUART_InitTypeDef *LOGUART_InitStruct)
{
	LOGUART_InitStruct->DmaModeCtrl = DISABLE;
	LOGUART_InitStruct->WordLen = LOGUART_WLS_8BITS;     // word length select
	LOGUART_InitStruct->StopBit = LOGUART_STOP_BIT_1;     // 0 -> 1 stop bit, 1 -> 2 stop bit
	LOGUART_InitStruct->Parity = DISABLE;      // parity check enable
	LOGUART_InitStruct->ParityType = LOGUART_ODD_PARITY;
	LOGUART_InitStruct->StickParity = DISABLE;

	LOGUART_InitStruct->FlowControl = DISABLE;
	LOGUART_InitStruct->RxTimeOutCnt = 64;
	LOGUART_InitStruct->RxFifoTrigLevel = LOGUART_RX_FIFOTRIG_LEVEL_1BYTES;
	LOGUART_InitStruct->RxErReportCtrl = LOGUART_RX_EEROR_REPORT_ENABLE;

	LOGUART_InitStruct->AGGEn = DISABLE;
	LOGUART_InitStruct->P1En = ENABLE;	//open path1 for HP log
	LOGUART_InitStruct->P2En = DISABLE;	//close path2 for LP log
	LOGUART_InitStruct->P3En = DISABLE;	//close path3 for BT log
	LOGUART_InitStruct->P4En = DISABLE;	//close path4 for AP log
	LOGUART_InitStruct->RPEn = DISABLE;	//close path5 for BT FW log
	LOGUART_InitStruct->AGGTO = 0x1;	// set timeout is 1 cycle to aviod rx disorder log, one cycle is one period of loguart Tx
	LOGUART_InitStruct->AGGTH = LOGUART_AGG_THRESHOLD_LEVEL_8BYTES;
	LOGUART_InitStruct->AGG_SYNC_PAT = 0xFF;	// AGG sync pattern is 0xFF
}

/**
  * @brief    Initializes the UARTLOG peripheral according to the specified
  *              parameters in the LOGUART_InitStruct.
  * @param  UARTLOG: LOGUART device.
  * @param  LOGUART_InitStruct: pointer to a LOGUART_InitTypeDef structure that contains
  *              the configuration information for LOGUART peripheral.
  * @retval None
  * @note   from uart driver
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void _LOGUART_Init(LOGUART_TypeDef *UARTLOG, LOGUART_InitTypeDef *LOGUART_InitStruct)
{
	u32 TempFcr;
	u32 TempRxPath;
	u32 TempAGGC;

	/*check the parameters*/
	assert_param(IS_LOGUART_RXFIFO_LEVEL(LOGUART_InitStruct->RxFifoTrigLevel));
	assert_param(IS_LOGUART_RX_ERROR_REPORT(LOGUART_InitStruct->RxErReportCtrl));
	assert_param(IS_LOGUART_AGG_THRESHOLD_LEVEL(LOGUART_InitStruct->AGGTH));

	/*disable rx path*/
	UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL &= ~LOGUART_BIT_R_RST_NEWRX_N;

	/*clear rx fifo */
	LOGUART_ClearRxFifo(UARTLOG);

	/* Disable all interrupts */
	UARTLOG->LOGUART_UART_IER = 0x00;

	/* Clean Rx break signal interrupt status at initial stage.*/
	UARTLOG->LOGUART_UART_SCR |= LOGUART_BIT_SCRATCH_7;

	/*get the FCR register value*/
	TempFcr = UARTLOG->LOGUART_UART_FCR;

	/*clear the rx fifo level and rx error report feild ( FCR[7:6] & FCR[0])*/
	TempFcr &= ~(LOGUART_BIT_FIFO_EN | LOGUART_MASK_RECVTRG);

	/*set the rx fifo trigger level*/
	TempFcr |= LOGUART_InitStruct->RxFifoTrigLevel;

	/*enable or disable rx error report*/
	TempFcr |= LOGUART_InitStruct->RxErReportCtrl;

	UARTLOG->LOGUART_UART_FCR = TempFcr;

	/* Configure FlowControl */
	if (LOGUART_InitStruct->FlowControl == ENABLE) {
		UARTLOG->LOGUART_UART_MCR |= (LOGUART_BIT_DTR | LOGUART_BIT_AFE);
	} else {
		UARTLOG->LOGUART_UART_MCR &= ~(LOGUART_BIT_DTR | LOGUART_BIT_AFE);
	}

	/*configure word length, stop bit, parity, parity type, stickparity*/
	UARTLOG->LOGUART_UART_LCR = ((LOGUART_InitStruct->WordLen) |
								 LOGUART_STB(LOGUART_InitStruct->StopBit) |
								 LOGUART_PEN(LOGUART_InitStruct->Parity) |
								 LOGUART_EPS(LOGUART_InitStruct->ParityType) |
								 LOGUART_STKP(LOGUART_InitStruct->StickParity));

	/* disable or enable the UART DMA mode */
	if (LOGUART_InitStruct->DmaModeCtrl != DISABLE) {
		UARTLOG->LOGUART_UART_FCR |= LOGUART_BIT_DMA_MODE;
	} else {
		UARTLOG->LOGUART_UART_FCR &= (~ LOGUART_BIT_DMA_MODE);
	}

	/*configure rx timeout counter*/
	TempRxPath = UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL;
	TempRxPath &= ~(LOGUART_MASK_R_RXTO_THRS);
	TempRxPath |= LOGUART_R_RXTO_THRS(LOGUART_InitStruct->RxTimeOutCnt);
	UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL = TempRxPath;

	/*Get the AGGC register value*/
	TempAGGC = UARTLOG->LOGUART_UART_AGGC;

	/*configure the AGG enable bit*/
	if (LOGUART_InitStruct->AGGEn != DISABLE) {
		TempAGGC |= LOGUART_BIT_AGG_EN;
	} else {
		TempAGGC &= (~ LOGUART_BIT_AGG_EN);
	}

	/*configure the open path1 bit*/
	if (LOGUART_InitStruct->P1En != DISABLE) {
		TempAGGC |= LOGUART_BIT_TP1_EN;
	} else {
		TempAGGC &= (~ LOGUART_BIT_TP1_EN);
	}

	/*configure the open path2 bit*/
	if (LOGUART_InitStruct->P2En != DISABLE) {
		TempAGGC |= LOGUART_BIT_TP2_EN;
	} else {
		TempAGGC &= (~ LOGUART_BIT_TP2_EN);
	}

	/*configure the open path3 bit*/
	if (LOGUART_InitStruct->P3En != DISABLE) {
		TempAGGC |= LOGUART_BIT_TP3_EN;
	} else {
		TempAGGC &= (~ LOGUART_BIT_TP3_EN);
	}

	/*configure the open path4 bit*/
	if (LOGUART_InitStruct->P4En != DISABLE) {
		TempAGGC |= LOGUART_BIT_TP4_EN;
	} else {
		TempAGGC &= (~ LOGUART_BIT_TP4_EN);
	}

	/*configure the open relay path bit*/
	if (LOGUART_InitStruct->RPEn != DISABLE) {
		TempAGGC |= LOGUART_BIT_RP_EN;
	} else {
		TempAGGC &= (~ LOGUART_BIT_RP_EN);
	}

	/*configure AGG timeout*/
	TempAGGC &= (~ LOGUART_MASK_AGG_TO);
	TempAGGC |= LOGUART_AGG_TO(LOGUART_InitStruct->AGGTO);

	/*configure AGG threshold*/
	TempAGGC &= (~ LOGUART_MASK_AGG_TH);
	TempAGGC |= LOGUART_AGG_TH(LOGUART_InitStruct->AGGTH);

	/*configure sync pattern*/
	TempAGGC &= (~ LOGUART_MASK_AGG_SYNC_PAT);
	TempAGGC |= (LOGUART_InitStruct->AGG_SYNC_PAT);

	UARTLOG->LOGUART_UART_AGGC = TempAGGC;
}

/**
  * @brief    Set Loguart Baud Rate use baudrate val.
  * @param  UARTLOG: LOGUART device.
  * @param  BaudRate: Baud Rate Val, like 115200 (unit is HZ).
  * @retval  None
  * @note   from uart driver
  */
__weak HAL_ROM_TEXT_SECTION
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
  * @brief    enable or disable Loguart Rx Path
  * @param  UARTLOG: LOGUART device.
  * @param  NewState: the new state of the Rx Path.
  *              This parameter can be: ENABLE or DISABLE.
  * @retval None
  * @note    from uart driver
  */
__weak HAL_ROM_TEXT_SECTION
void LOGUART_RxCmd(LOGUART_TypeDef *UARTLOG, u32 NewState)
{
	if (NewState != DISABLE) {
		UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL |= LOGUART_BIT_R_RST_NEWRX_N;
	} else {
		UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL &= ~LOGUART_BIT_R_RST_NEWRX_N;
	}
}

/**
  * @brief  Initializes the LOGUART registers according to the specified parameters
  *         in LOGUART_InitStruct.
  * @param  LOGUART_InitStruct: pointer to a LOGUART_InitTypeDef structure that contains
  *         the configuration information for the LOGUART peripheral.
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_Init(LOGUART_InitTypeDef *LOGUART_InitStruct)
{
	_LOGUART_Init(LOGUART_DEV, LOGUART_InitStruct);

	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_PALADIUM) {
		LOGUART_SetBaud(LOGUART_DEV, 2000000);
	} else if (SYSCFG_OTP_LowBaudLog() == TRUE) {
		LOGUART_SetBaud(LOGUART_DEV, 115200);
	} else {
		LOGUART_SetBaud(LOGUART_DEV, 1500000);
	}

	LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, ENABLE);
	LOGUART_RxCmd(LOGUART_DEV, ENABLE);
}

/**
  * @brief  Enables or Disables the AGG function.
  * @param  UARTLOG: LOGUART device.
  * @param  NewState: new state of AGG enable bit
  *          This parameter can be:ENABLE or DISABLE
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
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
HAL_ROM_TEXT_SECTION _LONG_CALL_
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
  * @brief  Set AGG timeout val.
  * @param  UARTLOG: LOGUART device.
  * @param  TimeOut: Timeout Val (unit is one baudrate period of loguart Tx).
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_AGGSetTimeOut(LOGUART_TypeDef *UARTLOG, u32 TimeOut)
{
	u32 TempAGGC;

	/*configure AGG timeout value*/
	TempAGGC = UARTLOG->LOGUART_UART_AGGC;
	TempAGGC &= ~(LOGUART_MASK_AGG_TO);
	TempAGGC |= LOGUART_AGG_TO(TimeOut);
	UARTLOG->LOGUART_UART_AGGC = TempAGGC;
}

/**
  * @brief  Set AGG threshold val.
  * @param  UARTLOG: LOGUART device.
  * @param  Threshold: Threshold Val.
  *	This parameter can be a value of @LOGUART_AGG_THRESHOLD_LEVEL_define.
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_AGGSetThreshold(LOGUART_TypeDef *UARTLOG, u32 Threshold)
{
	u32 TempAGGC;

	assert_param(IS_LOGUART_AGG_THRESHOLD_LEVEL(Threshold));

	/*configure AGG threshold value*/
	TempAGGC = UARTLOG->LOGUART_UART_AGGC;
	TempAGGC &= ~(LOGUART_MASK_AGG_TH);
	TempAGGC |= LOGUART_AGG_TH(Threshold);
	UARTLOG->LOGUART_UART_AGGC = TempAGGC;
}

/**
  * @brief  Set AGG sync pattern.
  * @param  UARTLOG: LOGUART device.
  * @param  SyncPattern: sync pattern.
  * @retval None
  * @note sync pattern is 0xFF default, it won't change normally
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_AGGSetSyncPattern(LOGUART_TypeDef *UARTLOG, u32 SyncPattern)
{
	u32 TempAGGC;

	/*configure AGG sync pattern*/
	TempAGGC = UARTLOG->LOGUART_UART_AGGC;
	TempAGGC &= ~(LOGUART_MASK_AGG_SYNC_PAT);
	TempAGGC |= LOGUART_AGG_SYNC_PAT(SyncPattern);
	UARTLOG->LOGUART_UART_AGGC = TempAGGC;
}

/**
  * @brief  Open or close all the path.
  * @param  UARTLOG: LOGUART device.
  * @param  NewState: new state of path enable bit
  *          This parameter can be:ENABLE or DISABLE
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_AGGPathAllCmd(LOGUART_TypeDef *UARTLOG, u32 NewState)
{
	u32 TempAGGC;

	/*Get the AGGC register value*/
	TempAGGC = UARTLOG->LOGUART_UART_AGGC;

	if (NewState != DISABLE) {
		TempAGGC |= (LOGUART_BIT_TP1_EN | LOGUART_BIT_TP2_EN | LOGUART_BIT_TP3_EN | LOGUART_BIT_TP4_EN | LOGUART_BIT_RP_EN);
	} else {
		TempAGGC &= ~(LOGUART_BIT_TP1_EN | LOGUART_BIT_TP2_EN | LOGUART_BIT_TP3_EN | LOGUART_BIT_TP4_EN | LOGUART_BIT_RP_EN);
	}

	UARTLOG->LOGUART_UART_AGGC = TempAGGC;

	/*delay 1us for register sync before configuration takes effect*/
	DelayUs(1);
}

/**
  * @brief  LOGUART send char
  * @param  c: send char value
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_PutChar(u8 c)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 CounterIndex = 0;
	u32 CPUID = SYS_CPUID();

	if (ConfigDebugClose == 1) {
		return;
	}

	while (1) {
		CounterIndex++;
		if (CounterIndex >= LOG_UART_IDX_FLAG[CPUID].tx_timeout) {
			break;
		}

		if (UARTLOG->LOGUART_UART_LSR & LOG_UART_IDX_FLAG[CPUID].not_full) {
			break;
		}
	}
	UARTLOG->LOGUART_UART_THRx[LOG_UART_IDX_FLAG[CPUID].idx] = c;

	if (c == KB_ASCII_LF) {
		UARTLOG->LOGUART_UART_THRx[LOG_UART_IDX_FLAG[CPUID].idx] = KB_ASCII_CR;
	}
}

/**
  * @brief  LOGUART get char
  * @param  PullMode This parameter can be one of the following values:
  *            @arg TRUE: poll line status for RX
  *            @arg FALSE: read RX register directly
  * @retval receive data
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
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
HAL_ROM_TEXT_SECTION _LONG_CALL_
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
  * @brief Get the number of valid entries in receive FIFO.
  * @param  none.
  * @retval  The number of valid entries in receive FIFO.Value range:0-16.
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
u32 LOGUART_GetRxCount(void)
{
	return LOGUART_GET_RX_FIFO_PTR(LOGUART_DEV->LOGUART_UART_LSR);
}

/**
  * @brief  Get LOGUART IMR
  * @retval LOGUART IMR
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
u32 LOGUART_GetIMR(void)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

	return UARTLOG->LOGUART_UART_IER;
}

/**
  * @brief  Set LOGUART IMR
  * @param  SetValue: specifies the LOGUART interrupt sources to be enabled or disabled.
  * @retval none
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_SetIMR(u32 SetValue)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

	UARTLOG->LOGUART_UART_IER = SetValue;
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
__weak HAL_ROM_TEXT_SECTION
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
__weak HAL_ROM_TEXT_SECTION
void LOGUART_INTClear(LOGUART_TypeDef *UARTLOG, u32 UART_IT)
{
	UARTLOG->LOGUART_UART_ICR |= UART_IT;
}

/**
  * @brief    get the line status register value.
  * @param  UARTLOG: LOGUART device.
  * @retval   the line status register value.
  */
__weak HAL_ROM_TEXT_SECTION
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
__weak HAL_ROM_TEXT_SECTION
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

/**
  * @brief  wait log uart path fifo empty.
  * @retval: none
  * @note   this function is to check fifo of path. To check if tx process is finish, please use LOGUART_WaitTxComplete.
  */
__weak HAL_ROM_TEXT_SECTION
void LOGUART_WaitBusy(void)
{
	u32 CPUID = SYS_CPUID();

	/* Wait for LogUart print out */
	while (1) {
		LOGUART_TypeDef *UARTLOG = LOGUART_DEV;

		if (UARTLOG->LOGUART_UART_LSR & LOG_UART_IDX_FLAG[CPUID].empty) {
			break;
		}

		DelayUs(100);
	}
}

/**
  * @brief  wait log uart tx is complete.
  * @retval: none
  * @note   this function is to wait until tx data is completely push from tx fifo and all the tx fifo is empty.
  */
__weak HAL_ROM_TEXT_SECTION
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

	/* delay at least 12 cycles of one bit time to make sure the last data is completely out of tx fifo, 4800bps is 2.5ms */
	DelayMs(3);
}

/**
  * @brief  LOGUART send char to path 3(BT) fifo
  * @param  InBuf: buffer to be written to Tx FIFO.
  * @param  Count: number of data to be written.
  * @retval none
  * @note   this function is for path3(BT) to tx.
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
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

/**
  * @brief  Fills each LOGUART_Relay_InitTypeDef member with its default value.
  * @param  LOGUART_Relay_InitTypeDef: pointer to a LOGUART_RP_InitTypeDef structure which will be
  *         initialized.
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_Relay_StructInit(LOGUART_Relay_InitTypeDef *LOGUART_Relay_InitStruct)
{
	LOGUART_Relay_InitStruct->WordLen = LOGUART_WLS_8BITS;     // word length select
	LOGUART_Relay_InitStruct->StopBit = LOGUART_STOP_BIT_1;     // 0 -> 1 stop bit, 1 -> 2 stop bit
	LOGUART_Relay_InitStruct->Parity = DISABLE;      // parity check enable
	LOGUART_Relay_InitStruct->ParityType = LOGUART_ODD_PARITY;
	LOGUART_Relay_InitStruct->StickParity = DISABLE;
	LOGUART_Relay_InitStruct->RxErReportCtrl = LOGUART_RX_EEROR_REPORT_ENABLE;
}

/**
  * @brief    Initializes the UARTLOGRelay Path according to the specified
  *              parameters in the LOGUART_Relay_InitTypeDef.
  * @param  UARTLOG: LOGUART device.
  * @param  LOGUART_Relay_InitTypeDef: pointer to a LOGUART_Relay_InitTypeDef structure that contains
  *              the configuration information for Relay Path.
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
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

/**
  * @brief    Set Loguart Relay Path Baud Rate use baudrate val.
  * @param  UARTLOG: LOGUART device.
  * @param  RPBaudRate: Baud Rate Val of Relay Path, like 115200 (unit is HZ).
  * @retval  None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
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

/**
  * @brief    Clear Relay Path Rx fifo ( write 1 to FCR[13] ).
  * @param  UARTLOG: LOGUART device.
  * @retval
  *              - 1: clear relay path fifo success
  *              - 0: clear relay path fifo fail
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
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

/**
  * @brief    enable or disable Loguart Relay Path
  * @param  UARTLOG: LOGUART device.
  * @param  NewState: the new state of the Relay Path.
  *              This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_Relay_RxCmd(LOGUART_TypeDef *UARTLOG, u32 NewState)
{
	if (NewState != DISABLE) {
		UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL |= LOGUART_BIT_R_RST_RP_RX_N;
	} else {
		UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL &= ~LOGUART_BIT_R_RST_RP_RX_N;
	}
}

/**
  * @brief  configure Loguart monitor parameters.
  * @param  UARTLOG: LOGUART device.
  * @param  BitNumThres: Configure bit number threshold of one monitor period.
  * @param  OscPerbitUpdCtrl: the OSC cycnum_perbit update bit(can be ENABLE or DISABLE).
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_MonitorParaConfig(LOGUART_TypeDef *UARTLOG, u32 BitNumThres, u32 OscPerbitUpdCtrl)
{
	u32 RegValue = 0;

	/*check the parameters*/
	assert_param((BitNumThres > 0) && (BitNumThres < 128));

	/* Step 1: Reset Low Power Rx Path */
	UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL &= ~LOGUART_BIT_R_RST_NEWRX_N;

	RegValue = UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL;

	/*step 2: Configure bit number threshold of one monitor period.*/
	RegValue &= (~ LOGUART_MASK_R_BIT_NUM_THRES);
	RegValue |= LOGUART_R_BIT_NUM_THRES(BitNumThres);

	/*step 3: Configure the OSC cycnum_perbit update bit (LOGUART_UART_REG_MON_BAUD_CTRL[29])*/
	if (OscPerbitUpdCtrl != DISABLE) {
		/*enable OSC cycnum_perbit update*/
		RegValue |= LOGUART_BIT_R_UPD_OSC_IN_XTAL;
	} else {
		/*disable OSC cycnum_perbit update*/
		RegValue &= ~ LOGUART_BIT_R_UPD_OSC_IN_XTAL;
	}
	UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL = RegValue;
}

/**
  * @brief    set uart baud rate of low power tx and rx path
  * @param  UARTLOG: LOGUART device.
  * @param  BaudRate: the desired baud rate
  * @param  IPClockHz: the uart clock. unit: [Hz]
  * @note    according to the baud rate calculation formlula in low power rx path, method
  *              implemented is as follows:
  *                                 - CycPerbit = round( fpclock/BaudRate)
  * @retval  None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_LPBaudSet(LOGUART_TypeDef *UARTLOG, u32 BaudRate, u32 IPClockHz)
{
	u32 CycPerbit = 0;
	u32 RegValue = 0;
	u32 RegOscBaud = 0;

	/*check the parameters*/
	assert_param((BaudRate > 0 && BaudRate <= 500000));

	/*Calculate the r_cycnum_perbit field of REG_MON_BAUD_STS,
	   according to clock and the desired baud rate*/
	if ((IPClockHz % BaudRate) >= (BaudRate + 1) / 2) {
		CycPerbit = IPClockHz / BaudRate + 1;
	} else {
		CycPerbit = IPClockHz / BaudRate;
	}

	/* Set DLAB bit to 1 to access DLL/DLM */
	UARTLOG->LOGUART_UART_LCR |= LOGUART_BIT_DLAB;

	/*Clean Rx break signal interrupt status at initial stage*/
	UARTLOG->LOGUART_UART_SCR |= LOGUART_BIT_SCRATCH_7;

	/* Set OVSR(xfactor) */
	RegValue = UARTLOG->LOGUART_UART_STSR;
	RegValue &= ~(LOGUART_MASK_XFACTOR);
	RegValue |= LOGUART_XFACTOR(CycPerbit);
	UARTLOG->LOGUART_UART_STSR = RegValue;

	/* Clr OVSR_ADJ[10:0] (xfactor_adj[26:16]) */
	RegValue = UARTLOG->LOGUART_UART_SCR;
	RegValue &= ~(LOGUART_MASK_XFACTOR_ADJ);
	UARTLOG->LOGUART_UART_SCR = RegValue;

	/* clear DLAB bit */
	UARTLOG->LOGUART_UART_LCR &= ~(LOGUART_BIT_DLAB);

	/* Average clock cycle number of one bit. MON_BAUD_STS[19:0] */
	RegValue = UARTLOG->LOGUART_UART_REG_MON_BAUD_STS;
	RegValue &= (~LOGUART_MASK_R_CYCNUM_PERBIT_XTAL);
	RegValue |= LOGUART_R_CYCNUM_PERBIT_XTAL(CycPerbit);

	/* set CycPerbit */
	UARTLOG->LOGUART_UART_REG_MON_BAUD_STS = RegValue;

	/* Average clock cycle number of one bit OSC. REG_MON_BAUD_CTRL[28:9] */
	RegOscBaud = UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL;
	RegOscBaud &= (~ LOGUART_MASK_R_CYCNUM_PERBIT_OSC);
	RegOscBaud |= LOGUART_R_CYCNUM_PERBIT_OSC(CycPerbit);

	/*set the OSC CycPerbit*/
	UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL = RegOscBaud;

	UARTLOG->LOGUART_UART_REG_RX_PATH_CTRL &= (~LOGUART_MASK_RXBAUD_ADJ_10_0);
}

/**
  * @brief    Set Loguart Relay Path Baud Rate of Low Power.
  * @param  UARTLOG: LOGUART device.
  * @param  RPBaudRate: Baud Rate Val of Relay Path, like 4800 (unit is HZ).
  * @param  IPClockHz: the uart clock. unit: [Hz]
  * @retval  None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_Relay_LPSetBaud(LOGUART_TypeDef *UARTLOG, u32 RPBaudRate, u32 IPClockHz)
{
	u32 CycPerbit = 0;
	u32 RegValue = 0;

	/*check the parameters*/
	assert_param((RPBaudRate > 0 && RPBaudRate <= 500000));

	/*Calculate the r_cycnum_perbit field of REG_MON_BAUD_STS,
	   according to clock and the desired baud rate*/
	if ((IPClockHz % RPBaudRate) >= (RPBaudRate + 1) / 2) {
		CycPerbit = IPClockHz / RPBaudRate + 1;
	} else {
		CycPerbit = IPClockHz / RPBaudRate;
	}

	/* Set DLAB bit to 1 to access DLL/DLM */
	UARTLOG->LOGUART_UART_RP_LCR |= LOGUART_BIT_RP_DLAB;

	/*rx baud rate configureation*/
	RegValue = UARTLOG->LOGUART_UART_RP_LCR;
	RegValue &= (~LOGUART_MASK_RP_RXBAUD_19_0);
	RegValue |= LOGUART_RP_RXBAUD_19_0(CycPerbit);
	UARTLOG->LOGUART_UART_RP_LCR = RegValue;

	RegValue = UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL;
	RegValue &= (~LOGUART_MASK_RP_RXBAUD_ADJ_10_0);
	UARTLOG->LOGUART_UART_RP_RX_PATH_CTRL = RegValue;

	/* clear DLAB bit */
	UARTLOG->LOGUART_UART_RP_LCR &= ~(LOGUART_BIT_RP_DLAB);
}

/**
  * @brief    enable or disable the monitor function in Low Power Rx Path( REG_MON_BAUD_CTRL[0] ).
  * @param  UARTLOG: LOGUART device.
  * @param  NewState: the new state of monitoring.
  *              This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_RxMonitorCmd(LOGUART_TypeDef *UARTLOG, u32 NewState)
{
	/* configure Low Power rx monitoring function*/
	if (NewState != DISABLE) {
		/* Function enable of monitoring rx baud */
		UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL |= LOGUART_BIT_R_MON_BAUD_EN;
	} else {
		/* Function disable of monitoring rx baud */
		UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL &= (~ LOGUART_BIT_R_MON_BAUD_EN);
	}
}

/**
  * @brief    Get the Monitor Baud Rate control register value of the Low Power Rx Path.
  * @param  UARTLOG: LOGUART device.
  * @note    MON_BAUD_CTRL[28:9] field value is the monitor result under OSC 2M Hz Clock.
  * @retval   the value of the MON_BAUD_CTRL register
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
u32 LOGUART_RxMonBaudCtrlRegGet(LOGUART_TypeDef *UARTLOG)
{
	/*return the monitor baud rate control register value(MON_BAUD_CTRL)*/
	return UARTLOG->LOGUART_UART_REG_MON_BAUD_CTRL;
}

/**
  * @brief    Get the status of the Low Power Rx Monitoring.
  * @param  UARTLOG: LOGUART device.
  * @note    Read this register can clear the monitor interrupt status.
  *              Besides,REG_MON_BAUD_STS[19:0] field value is the monitor
  *              result under XTAL 2M Hz Clock.
  * @retval   the value of the REG_MON_BAUD_STS register
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
u32 LOGUART_RxMonitorSatusGet(LOGUART_TypeDef *UARTLOG)
{
	/*return the monitor status register value*/
	return UARTLOG->LOGUART_UART_REG_MON_BAUD_STS;
}

/**
  * @brief  DIAG use LOGUART init
  * @param  InitConsol
  *            @arg TRUE: shell_init_rom & InterruptRegister will be excute
  *            @arg FALSE: without shell_init_rom & InterruptRegister, used for just reinit HW like resume from PG
  * @retval none
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
void LOGUART_DiagInit(BOOL InitConsol)
{
	LOGUART_InitTypeDef LOGUART_InitStruct;
	u32 CPUID = SYS_CPUID();

	/* Release log uart reset and clock */
	RCC_PeriphClockCmd(APBPeriph_LOGUART, APBPeriph_LOGUART_CLOCK, ENABLE);

	/* Just Support S0 */
	Pinmux_UartLogCtrl(PINMUX_S0, ON);

	LOGUART_StructInit(&LOGUART_InitStruct);
	LOGUART_Init(&LOGUART_InitStruct);

	//4 initial uart log parameters before any uartlog operation
	if (InitConsol == _TRUE) {
		u32 TblSize = 0;
		void *PTable = NULL;

		/* Register Log Uart Callback function */
		InterruptRegister((IRQ_FUN) shell_uart_irq_rom, LOG_UART_IDX_FLAG[CPUID].irq_num, (u32)NULL, 3);
		InterruptEn(LOG_UART_IDX_FLAG[CPUID].irq_num, 3);

		TblSize = cmd_rom_table(&PTable);
		shell_init_rom(TblSize, PTable);// executing boot seq.,
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
