/**
  ******************************************************************************
  * @file    rtl8721d_rtc.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Real-Time Clock (RTC) peripheral:
  *           - Initialization
  *           - Calendar (Time and Date) configuration
  *           - Alarms configuration
  *           - WakeUp Timer configuration
  *           - Daylight Saving configuration
  *           - Output pin Configuration
  *           - Smooth digital Calibration configuration
  *           - TimeStamp configuration
  *           - Interrupts and flags management
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

/**
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value: Byte to be converted.
  * @retval Converted byte
  */
u8 RTC_ByteToBcd2(u8 Value)
{
	u8 bcdhigh = 0;

	while (Value >= 10) {
		bcdhigh++;
		Value -= 10;
	}

	return ((u8)(bcdhigh << 4) | Value);
}

/**
  * @brief  Convert from 2 digit BCD to Binary.
  * @param  Value: BCD value to be converted.
  * @retval Converted word
  */
u8 RTC_Bcd2ToByte(u8 Value)
{
	u8 tmp = 0;
	tmp = (u8)(((Value & 0xF0) >> 0x4) * 10);
	return ((u8)(tmp + (Value & 0x0F)));
}

/**
  * @brief  Enters the RTC Initialization mode.
  * @note  the time, date and prescaler registers can be updated in init mode.
  * @param  None
  * @retval status value:
  *          - 1: RTC is in Init mode
  *          - 0: RTC is not in Init mode
  */
u32 RTC_EnterInitMode(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0x00;
	u32 status = 0;
	u32 initstatus = 0x00;

	/* Check if the Initialization mode is set */
	if ((RTC->RTC_ISR & RTC_BIT_INITF) == 0) {
		/* Set the Initialization mode */
		RTC->RTC_ISR |= (u32)RTC_BIT_INIT;

		/* Wait till RTC is in INIT state and if Time out is reached exit */
		do {
			/* check Calendar registers update is allowed */
			initstatus = RTC->RTC_ISR & RTC_BIT_INITF;
			counter++;
		} while ((counter != INITMODE_TIMEOUT) && (initstatus == 0x00));

		if (RTC->RTC_ISR & RTC_BIT_INITF) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 1;
	}

	return (status);
}

/**
  * @brief  Exits the RTC Initialization mode.
  * @note   When the initialization sequence is complete, the calendar restarts
  *         counting after 4 RTCCLK cycles.
  * @param  None
  * @retval None
  * @about 3 RTC clock
  */
void RTC_ExitInitMode(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);

	/* Exit Initialization mode */
	RTC->RTC_ISR &= ~RTC_BIT_INIT;
}

/**
  * @brief  Waits until the RTC Time register (RTC_TR) is
  *         synchronized with RTC APB clock.
  * @note   To read the calendar through the shadow registers after Calendar
  *         initialization, calendar update or after wakeup from low power modes
  *         the software must first clear the RSF flag.
  *         The software must then wait until it is set again before reading
  *         the calendar, which means that the calendar registers have been
  *         correctly copied into the RTC_TR shadow registers.
  * @param  None
  * @retval status value:
  *          - 1: RTC registers are synchronised
  *          - 0: RTC registers are not synchronised
  * @about 2 RTC clock
  */
u32 RTC_WaitForSynchro(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0;
	u32 status = 0;
	u32 synchrostatus = 0x00;

	/* we can not poll RTC_ISR_RSF when RTC_CR_BYPSHAD */
	if (RTC->RTC_CR & RTC_BIT_BYPSHAD) {
		/* it will take four 32.768k clock cycles to wait the registers being synchronised */
		DelayUs(125);

		return 1;
	}

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* write 1 to Clear RSF flag */
	RTC->RTC_ISR |= RTC_BIT_RSF;

	/* Wait the registers to be synchronised */
	do {
		synchrostatus = RTC->RTC_ISR & RTC_BIT_RSF; /* 6bus cycle one time */
		counter++;
	} while ((counter != SYNCHRO_TIMEOUT) && (synchrostatus == 0x00));

	if (RTC->RTC_ISR & RTC_BIT_RSF) {
		status = 1;
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return (status);
}

/**
  * @brief  Enables or Disables the Bypass Shadow feature.
  * @note   When the Bypass Shadow is enabled the calendar value are taken
  *         directly from the Calendar counter.
  * @param  NewState: new state of the Bypass Shadow feature.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
*/
u32 RTC_BypassShadowCmd(u32 NewState)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 status;

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		if (NewState != DISABLE) {
			/* Set the BYPSHAD bit */
			RTC->RTC_CR |= RTC_BIT_BYPSHAD;
		} else {
			/* Reset the BYPSHAD bit */
			RTC->RTC_CR &= ~RTC_BIT_BYPSHAD;
		}

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Fills each RTC_InitStruct member with its default value.
  * @param  RTC_InitStruct: pointer to a RTC_InitTypeDef structure which will be
  *         initialized.
  * @retval None
  */
void RTC_StructInit(RTC_InitTypeDef *RTC_InitStruct)
{
	/* Initialize the RTC_HourFormat member */
	RTC_InitStruct->RTC_HourFormat = RTC_HourFormat_24;

	/* Initialize the RTC_AsynchPrediv member */
	RTC_InitStruct->RTC_AsynchPrediv = (u32)0x7F; /* 32768 /128 = 256Hz */

	/* Initialize the RTC_SynchPrediv member */
	RTC_InitStruct->RTC_SynchPrediv = (u32)0xFF; /* 256 /256 = 1Hz*/

	/*Initialize the RTC_Day_Threshold member*/
	RTC_InitStruct->RTC_DayThreshold = (u32) 0x1FF;
}

/**
  * @brief  Initializes the RTC registers according to the specified parameters
  *         in RTC_InitStruct.
  * @param  RTC_InitStruct: pointer to a RTC_InitTypeDef structure that contains
  *         the configuration information for the RTC peripheral.
  * @note   The RTC Prescaler register is write protected and can be written in
  *         initialization mode only.
  * @retval status value:
  *          - 1: RTC registers are initialized
  *          - 0: RTC registers are not initialized
  */
u32 RTC_Init(RTC_InitTypeDef *RTC_InitStruct)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 Temp = 0;
	u32 status = 0;

	/* Check the parameters */
	assert_param(IS_RTC_HOUR_FORMAT(RTC_InitStruct->RTC_HourFormat));
	assert_param(IS_RTC_ASYNCH_PREDIV(RTC_InitStruct->RTC_AsynchPrediv));
	assert_param(IS_RTC_SYNCH_PREDIV(RTC_InitStruct->RTC_SynchPrediv));
	assert_param(IS_RTC_DAY_THRES(RTC_InitStruct->RTC_DayThreshold));

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		/* Configure the RTC PRER to provide 1Hz to the Calendar */
		Temp = (RTC_InitStruct->RTC_SynchPrediv) | RTC_PREDIV_A(RTC_InitStruct->RTC_AsynchPrediv);
		RTC->RTC_PRER = Temp;

		/* Set RTC CR FMT Bit */
		Temp = RTC->RTC_CR;
		Temp &= ((u32)~(RTC_BIT_FMT | RTC_DAYTHRES_MSK | RTC_BIT_DOVTHIE));
		Temp |= ((u32)((RTC_InitStruct->RTC_HourFormat) | RTC_DAY_THRES(RTC_InitStruct->RTC_DayThreshold)));

		/* Reset the BYPSHAD bit, get counter from shadow registger */
		Temp &= ~RTC_BIT_BYPSHAD;
		RTC->RTC_CR = Temp;

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Fills each RTC_TimeStruct member with its default value
  *         (Time = 00d:00h:00min:00sec).
  * @param  RTC_TimeStruct: pointer to a RTC_TimeTypeDef structure which will be
  *         initialized.
  * @retval None
  */
void RTC_TimeStructInit(RTC_TimeTypeDef *RTC_TimeStruct)
{
	/* Time = 00h:00min:00sec */
	RTC_TimeStruct->RTC_H12_PMAM = RTC_H12_AM;
	RTC_TimeStruct->RTC_Year = RTC_BASE_YEAR;
	RTC_TimeStruct->RTC_Hours = 0;
	RTC_TimeStruct->RTC_Minutes = 0;
	RTC_TimeStruct->RTC_Seconds = 0;
	RTC_TimeStruct->RTC_Days = 0;
}

/**
  * @brief  Set the RTC current time.
  * @param  RTC_Format: specifies the format of the entered parameters.
  *          This parameter can be  one of the following values:
  *            @arg RTC_Format_BIN:  Binary data format
  *            @arg RTC_Format_BCD:  BCD data format
  * @param  RTC_TimeStruct: pointer to a RTC_TimeTypeDef structure that contains
  *                        the time configuration information for the RTC.
  * @retval status value:
  *          - 1: RTC Time register is configured
  *          - 0: RTC Time register is not configured
  */
u32 RTC_SetTime(u32 RTC_Format, RTC_TimeTypeDef *RTC_TimeStruct)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 tmpreg = 0;
	u32 status = 0;

	/* Check the parameters */
	assert_param(IS_RTC_FORMAT(RTC_Format));
	assert_param(IS_RTC_YEAR_THRES(RTC_TimeStruct->RTC_Year));

	if (RTC_Format == RTC_Format_BIN) {
		if ((RTC->RTC_CR & RTC_BIT_FMT) != RTC_HourFormat_24) {
			assert_param(IS_RTC_HOUR12(RTC_TimeStruct->RTC_Hours));
			assert_param(IS_RTC_H12_AMPM(RTC_TimeStruct->RTC_H12_PMAM));
		} else {
			RTC_TimeStruct->RTC_H12_PMAM = 0x00;
			assert_param(IS_RTC_HOUR24(RTC_TimeStruct->RTC_Hours));
		}
		assert_param(IS_RTC_MINUTES(RTC_TimeStruct->RTC_Minutes));
		assert_param(IS_RTC_SECONDS(RTC_TimeStruct->RTC_Seconds));
	} else {
		if ((RTC->RTC_CR & RTC_BIT_FMT) != RTC_HourFormat_24) {
			tmpreg = RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Hours);
			assert_param(IS_RTC_HOUR12(tmpreg));
			assert_param(IS_RTC_H12_AMPM(RTC_TimeStruct->RTC_H12_PMAM));
		} else {
			RTC_TimeStruct->RTC_H12_PMAM = 0x00;
			assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Hours)));
		}
		assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Minutes)));
		assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Seconds)));
	}

	/* Check the input parameters format */
	if (RTC_Format != RTC_Format_BIN) {
		tmpreg = (((u32)(RTC_TimeStruct->RTC_Hours) << RTC_SHIFT_HU) | \
				  ((u32)(RTC_TimeStruct->RTC_Minutes) << RTC_SHIFT_MNU) | \
				  ((u32)RTC_TimeStruct->RTC_Seconds) | \
				  (RTC_PM(RTC_TimeStruct->RTC_H12_PMAM)));
	} else {
		tmpreg = (u32)(((u32)RTC_ByteToBcd2(RTC_TimeStruct->RTC_Hours) << RTC_SHIFT_HU) | \
					   ((u32)RTC_ByteToBcd2(RTC_TimeStruct->RTC_Minutes) << RTC_SHIFT_MNU) | \
					   ((u32)RTC_ByteToBcd2(RTC_TimeStruct->RTC_Seconds)) | \
					   ((RTC_PM(RTC_TimeStruct->RTC_H12_PMAM))));
	}

	/* set Days */
	tmpreg |= RTC_DAY(RTC_TimeStruct->RTC_Days);

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		/* Set the RTC_TR register */
		RTC->RTC_TR = (u32)(tmpreg & RTC_TR_RESERVED_MASK);

		/* Set the RTC_YEARR register */
		RTC->RTC_YEAR = RTC_YEAR(RTC_TimeStruct->RTC_Year - RTC_BASE_YEAR); //starts from 1900

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Get the RTC current Time.
  * @param  RTC_Format: specifies the format of the returned parameters.
  *          This parameter can be  one of the following values:
  *            @arg RTC_Format_BIN:  Binary data format
  *            @arg RTC_Format_BCD:  BCD data format
  * @param  RTC_TimeStruct: pointer to a RTC_TimeTypeDef structure that will
  *                        contain the returned current time configuration.
  * @retval None
  */
void RTC_GetTime(u32 RTC_Format, RTC_TimeTypeDef *RTC_TimeStruct)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 tmpreg = 0;

	/* Check the parameters */
	assert_param(IS_RTC_FORMAT(RTC_Format));

	/* Get the RTC_TR register */
	tmpreg = (u32)(RTC->RTC_TR & RTC_TR_RESERVED_MASK);

	/* Fill the structure fields with the read parameters */
	RTC_TimeStruct->RTC_Hours = (u8)((tmpreg & (RTC_MASK_HT | RTC_MASK_HU)) >> RTC_SHIFT_HU);
	RTC_TimeStruct->RTC_Minutes = (u8)((tmpreg & (RTC_MASK_MNT | RTC_MASK_MNU)) >> RTC_SHIFT_MNU);
	RTC_TimeStruct->RTC_Seconds = (u8)(tmpreg & (RTC_MASK_ST | RTC_MASK_SU));
	RTC_TimeStruct->RTC_H12_PMAM = (u8)((tmpreg & RTC_BIT_PM) >> RTC_SHIFT_PM);
	RTC_TimeStruct->RTC_Days = (u16)RTC_GET_DAY(tmpreg);

	/* Check the input parameters format */
	if (RTC_Format == RTC_Format_BIN) {
		/* Convert the structure parameters to Binary format */
		RTC_TimeStruct->RTC_Hours = (u8)RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Hours);
		RTC_TimeStruct->RTC_Minutes = (u8)RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Minutes);
		RTC_TimeStruct->RTC_Seconds = (u8)RTC_Bcd2ToByte(RTC_TimeStruct->RTC_Seconds);
	}

	RTC_TimeStruct->RTC_Year = (u8)RTC_GET_YEAR(RTC->RTC_YEAR) + RTC_BASE_YEAR;
}

/**
  * @brief  clear day over threshold pending interrupt.
  * @retval None
  */
void RTC_DayIntClear(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 isr = RTC->RTC_ISR;
	u32 counter = 0;

	/*not clear alarm flag and wakeup flag*/
	isr &= ~(RTC_BIT_ALMF | RTC_BIT_WUTF);

	isr |= RTC_BIT_DOVTHF;

	RTC->RTC_ISR = isr;

	while (1) {
		/* check Alarm flag clear success*/
		if ((RTC->RTC_ISR & RTC_BIT_DOVTHF) == 0) {
			break;
		}
		if (counter >= ALARMDIS_TIMEOUT) {
			break;
		}

		counter++;
	}

	/*it will take two 32.768k clock cycle for actual interrupt signal clear*/
}

/**
  * @brief  Enables or disables the RTC day over threshold interrupt.
  * @param  NewState: new state of the RTC day over threshold interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  */
u32 RTC_DayIntCmd(u32 NewState)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 status = 0;

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		/* clear isr, or set will fail */
		RTC_DayIntClear();

		/* Configure the day over threshold interrupt state */
		if (NewState != DISABLE) {
			/*enable day over threshold interrupt*/
			RTC->RTC_CR |= (u32)(RTC_BIT_DOVTHIE);
		} else {
			RTC->RTC_CR &= (u32)~(RTC_BIT_DOVTHIE);
		}

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Set the RTC day threshold.
  * @param  DayThres: specifies the day threshold to be configured.
  *          This parameter can be a value of @ref RTC_Day_Threshold.
  * @retval status value:
  *          - 1: RTC day threshold is configured
  *          - 0: RTC day threshold is not configured
  */
u32 RTC_DayThresSet(u32 DayThres)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 temp = RTC->RTC_CR;
	u32 status;

	assert_param(IS_RTC_DAY_THRES(DayThres));

	temp &= ~RTC_DAYTHRES_MSK;
	temp |= RTC_DAY_THRES(DayThres);

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		/*Set the RTC_CR register*/
		RTC->RTC_CR = temp;

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Get the RTC day threshold value in RTC_CR register.
  * @retval RTC day threshold value
  */
u32 RTC_DayThresGet(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);

	return RTC_GET_DAY_THRES(RTC->RTC_CR);
}

/**
  * @brief  Set the specified RTC Alarm.
  * @note   The Alarm register can only be written when the corresponding Alarm
  *         is disabled (Use the RTC_AlarmCmd(DISABLE)).
  * @param  RTC_Format: specifies the format of the returned parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_Format_BIN: Binary data format
  *            @arg RTC_Format_BCD: BCD data format
  * @param  RTC_AlarmStruct: pointer to a RTC_AlarmTypeDef structure that
  *                          contains the alarm configuration parameters.
  * @retval status value:
  *          - 1: RTC Time Alarm is configured
  *          - 0: RTC Time Alarm is not configured
  */
u32 RTC_SetAlarm(u32 RTC_Format, RTC_AlarmTypeDef *RTC_AlarmStruct)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 tmpreg = 0;
	u32 counter = 0;
	u32 status = 0;

	/* Check the parameters */
	assert_param(IS_RTC_FORMAT(RTC_Format));
	assert_param(IS_ALARM_MASK(RTC_AlarmStruct->RTC_AlarmMask));
	assert_param(IS_ALARM2_MASK(RTC_AlarmStruct->RTC_Alarm2Mask));

	if (RTC_Format == RTC_Format_BIN) {
		if ((RTC->RTC_CR & RTC_BIT_FMT) != RTC_HourFormat_24) {
			assert_param(IS_RTC_HOUR12(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours));
			assert_param(IS_RTC_H12_AMPM(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM));
		} else {
			RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM = 0x00;
			assert_param(IS_RTC_HOUR24(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours));
		}
		assert_param(IS_RTC_MINUTES(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes));
		assert_param(IS_RTC_SECONDS(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds));
	} else {
		if ((RTC->RTC_CR & RTC_BIT_FMT) != RTC_HourFormat_24) {
			tmpreg = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours);
			assert_param(IS_RTC_HOUR12(tmpreg));
			assert_param(IS_RTC_H12_AMPM(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM));
		} else {
			RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM = 0x00;
			assert_param(IS_RTC_HOUR24(RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours)));
		}

		assert_param(IS_RTC_MINUTES(RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes)));
		assert_param(IS_RTC_SECONDS(RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds)));
	}

	/* Check the input parameters format */
	if (RTC_Format != RTC_Format_BIN) {
		tmpreg = (((u32)(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours) << RTC_SHIFT_ALR_HU) | \
				  ((u32)(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes) << RTC_SHIFT_ALR_MNU) | \
				  ((u32)RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds) | \
				  (RTC_ALR_PM(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM)) | \
				  ((u32)RTC_AlarmStruct->RTC_AlarmMask));
	} else {
		tmpreg = (((u32)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours) << RTC_SHIFT_ALR_HU) | \
				  ((u32)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes) << RTC_SHIFT_ALR_MNU) | \
				  ((u32)RTC_ByteToBcd2(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds)) | \
				  (RTC_ALR_PM(RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM)) | \
				  ((u32)RTC_AlarmStruct->RTC_AlarmMask));
	}

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Disable Alarm */
	RTC->RTC_CR &= ~RTC_Alarm;
	while (1) {
		/* check Alarm update allowed */
		if (RTC->RTC_ISR & RTC_BIT_ALMWF) {
			break;
		}
		if (counter >= ALARMDIS_TIMEOUT) {
			break;
		}

		counter++;
	}

	/* clear isr, or set wll fail */
	RTC->RTC_ISR |= RTC_BIT_ALMF;

	if (RTC->RTC_ISR & RTC_BIT_ALMWF) {
		/* Configure the Alarm1 register H:M:S */
		RTC->RTC_ALMR1L = (u32)tmpreg;

		/* Configure the Alarm2 register D */
		RTC->RTC_ALMR1H = ((u32)RTC_AlarmStruct->RTC_AlarmTime.RTC_Days) | ((u32)RTC_AlarmStruct->RTC_Alarm2Mask);

		status = 1;
	} else {
		status = 0;
	}

	/* Enable Alarm after this function */

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Fills each RTC_AlarmStruct member with its default value
  *         (Time = 00d:00h:00mn:20sec / Date = 1st day of the month/Mask =
  *         all fields are masked except Alarm Seconds field).
  * @param  RTC_AlarmStruct: pointer to a @ref RTC_AlarmTypeDef structure which
  *         will be initialized.
  * @retval None
  */
void RTC_AlarmStructInit(RTC_AlarmTypeDef *RTC_AlarmStruct)
{
	/* Alarm Time Settings : Time = 00h:00mn:00sec */
	RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM = RTC_H12_AM;
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours = 0;
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes = 0;
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds = 20;
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Days = 0;

	/* Alarm Masks Settings : Mask =  all fields are not masked */
	RTC_AlarmStruct->RTC_AlarmMask = RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;

	/* Alarm2 Mask: Day */
	RTC_AlarmStruct->RTC_Alarm2Mask = RTC_Alarm2Mask_Days;
}

/**
  * @brief  Get the RTC Alarm value and masks.
  * @param  RTC_Format: specifies the format of the output parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_Format_BIN: Binary data format
  *            @arg RTC_Format_BCD: BCD data format
  * @param  RTC_AlarmStruct: pointer to a RTC_AlarmTypeDef structure that will
  *                          contains the output alarm configuration values.
  * @retval None
  */
void RTC_GetAlarm(u32 RTC_Format, RTC_AlarmTypeDef *RTC_AlarmStruct)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 tmpreg = 0;

	/* Check the parameters */
	assert_param(IS_RTC_FORMAT(RTC_Format));

	/* Get the RTC_ALRMxR register */
	tmpreg = (u32)(RTC->RTC_ALMR1L);

	/* Fill the structure with the read parameters */
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours = (u8)((tmpreg & (RTC_MASK_ALR_HT | RTC_MASK_ALR_HU)) >> RTC_SHIFT_ALR_HU);
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes = (u8)((tmpreg & (RTC_MASK_ALR_MNT | RTC_MASK_ALR_MNU)) >> RTC_SHIFT_ALR_MNU);
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds = (u8)(tmpreg & (RTC_MASK_ALR_ST | RTC_MASK_ALR_SU));
	RTC_AlarmStruct->RTC_AlarmTime.RTC_H12_PMAM = (u8)((tmpreg & RTC_BIT_ALR_PM) >> RTC_SHIFT_ALR_PM);
	RTC_AlarmStruct->RTC_AlarmMask = (u32)(tmpreg & RTC_AlarmMask_All);

	if (RTC_Format == RTC_Format_BIN) {
		RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Hours);
		RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Minutes);
		RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds = RTC_Bcd2ToByte(RTC_AlarmStruct->RTC_AlarmTime.RTC_Seconds);
	}

	/* Get Days */
	RTC_AlarmStruct->RTC_AlarmTime.RTC_Days = (u16)((RTC->RTC_ALMR1H) & RTC_MASK_ALR_DAY);
	RTC_AlarmStruct->RTC_Alarm2Mask = (u32)((RTC->RTC_ALMR1H) & RTC_BIT_MSK3);
}

/**
  * @brief  Enables or disables the specified RTC Alarm.
  * @param  NewState: new state of the specified alarm.
  *          This parameter can be: ENABLE or DISABLE.
  */
void RTC_AlarmCmd(u32 NewState)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0;

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Configure the Alarm state */
	if (NewState != DISABLE) {
		RTC->RTC_ISR |= RTC_BIT_ALMF;

		RTC->RTC_CR |= (u32)(RTC_Alarm | RTC_Alarm_IntEn);

		/* we should wait shadow reigster sync ok */
		(void) RTC_WaitForSynchro();
	} else {
		/* clear isr, or set will fail */
		RTC->RTC_ISR |= RTC_BIT_ALMF;

		/* Disable the Alarm in RTC_CR register */
		RTC->RTC_CR &= (u32)~(RTC_Alarm | RTC_Alarm_IntEn);

		/* wait alarm disable */
		while (1) {
			/* check Alarm update allowed */
			if ((RTC->RTC_ISR & RTC_BIT_ALMWF) && (!(RTC->RTC_CR & RTC_BIT_ALME))) {
				break;
			}
			if (counter >= ALARMDIS_TIMEOUT) {
				break;
			}

			counter++;
		}
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);
}

/**
  * @brief  clear alarm pending interrupt.
  * @retval None
  */
void RTC_AlarmClear(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 isr = RTC->RTC_ISR;
	u32 counter = 0;

	/*not clear day over thres flag and wakeup flag*/
	isr &= ~(RTC_BIT_DOVTHF | RTC_BIT_WUTF);

	isr |= RTC_BIT_ALMF;

	RTC->RTC_ISR = isr;

	while (1) {
		/* check Alarm flag clear success*/
		if ((RTC->RTC_ISR & RTC_BIT_ALMF) == 0) {
			break;
		}
		if (counter >= ALARMDIS_TIMEOUT) {
			break;
		}

		counter++;
	}

	/*it will take two 32.768k clock cycle for actual interrupt signal clear*/
}

/**
  * @brief  Waits until the RTC Wakeup timer enable bit (WUTE bit) is
  *         synchronized with RTC APB clock.
  * @note   The software must then wait until it is set again before
  *         wakeup timer works, which means that the WUTE bit have
  *         been correctly copied into the shadow register.
  *         By the time WUTE bit is synchronized, other control bits of
  *         wakeup timer has already been synchronized, because these
  *          bits are configure before WUTE bit.
  * @param  None
  * @retval status value:
  *          - 1: RTC Wakeup Timer registers are synchronised
  *          - 0: RTC Wakeup Timer registers are not synchronised
  * @about 2 RTC clock
  */
u32 RTC_WaitForWUTSynchro(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0;
	u32 status = 0;

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* write 1 to Clear WUTRSF flag */
	RTC->RTC_ISR |= RTC_BIT_WUTRSF;

	/* Wait the WUT registers to be synchronised */
	while (counter <= SYNCHRO_TIMEOUT) {
		counter++;

		if (RTC->RTC_ISR & RTC_BIT_WUTRSF) {
			break;
		}
	}

	if (RTC->RTC_ISR & RTC_BIT_WUTRSF) {
		status = 1;
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return (status);
}

/**
  * @brief  Set the specified RTC Wakeup Timer.
  * @note   The Wakeup Timer register can only be written when the WakeupTimer
  *         is disabled (Use the RTC_WakeupTimerCmd(DISABLE)).
  * @param  RTC_WakeupRange: wakeup time to be configured.
  *	This parameter can be a value from 0x1 to 0x1FF.
  * @retval status value:
  *          - 1: RTC WakeupTimer is configured
  *          - 0: RTC WakeupTimer is not configured
  */
u32 RTC_SetWakeup(u32 RTC_WakeupRange)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0;
	u32 status = 0;

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Disable WakeupTimer */
	RTC->RTC_CR &= ~RTC_BIT_WUTE;

	while (counter <= WUTDIS_TIMEOUT) {
		counter++;

		/* check WakeupTimer update allowed */
		if (RTC->RTC_ISR & RTC_BIT_WUTWF) {
			break;
		}
	}

	/* clear isr, or set wll fail */
	RTC->RTC_ISR |= RTC_BIT_WUTF;

	if (RTC->RTC_ISR & RTC_BIT_WUTWF) {
		/* Configure the WakeupTimer cloak */
		RTC->RTC_WUTR = RTC_WUT(RTC_WakeupRange);

		status = 1;
	} else {
		status = 0;
	}

	/* Enable wakeup timer after this function */

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Get the RTC Wakeup Timer value and masks.
  * @param  none.
  * @retval the wakeup time value.
  */
u32 RTC_GetWakeup(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);

	/* Fill the structure with the read parameters */
	return RTC_GET_WUT(RTC->RTC_WUTR);
}

/**
  * @brief  Enables or disables the specified RTC Alarm.
  * @param  NewState: new state of the specified alarm.
  *          This parameter can be: ENABLE or DISABLE.
  * @note: wakeup timer starts down counting after 3 to 4 seconds
  */
void RTC_WakeupCmd(u32 NewState)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0;

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Configure the wakeup timer state */
	if (NewState != DISABLE) {
		RTC->RTC_ISR |= RTC_BIT_WUTF;

		RTC->RTC_CR |= (u32)(RTC_BIT_WUTE | RTC_BIT_WUTIE);

		/* we should wait shadow reigster sync ok, it will takes 3 to 4 seconds*/
		RTC_WaitForWUTSynchro();

	} else {
		/* clear isr, or set will fail */
		RTC->RTC_ISR |= RTC_BIT_WUTF;

		/* Disable the wakeup timer in RTC_CR register */
		RTC->RTC_CR &= (u32)~(RTC_BIT_WUTE | RTC_BIT_WUTIE);

		/* wait wakeup timer disable */
		while (counter <= WUTDIS_TIMEOUT) {
			counter++;

			/* check wakeup timer update allowed */
			if ((RTC->RTC_ISR & RTC_BIT_WUTWF) && (!(RTC->RTC_CR & RTC_BIT_WUTE))) {
				break;
			}
		}
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);
}

/**
  * @brief  clear wakeup pending interrupt.
  * @retval None
  */
void RTC_WakeupClear(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 isr = RTC->RTC_ISR;
	u32 counter = 0;

	/*not clear day over thres flag and alarm flag*/
	isr &= ~(RTC_BIT_DOVTHF | RTC_BIT_ALMF);

	isr |= RTC_BIT_WUTF;

	RTC->RTC_ISR = isr;

	while (counter <= WUTDIS_TIMEOUT) {
		counter++;

		/* check wakeup timer clear success */
		if ((RTC->RTC_ISR & RTC_BIT_WUTF) == 0) {
			break;
		}
	}

	/*it will take two 32.768k clock cycle for actual interrupt signal clear*/
}

/**
  * @brief  Adds or substract one hour from the current time.
  * @param  RTC_DayLightSaving: the value of hour adjustment.
  *          This parameter can be one of the following values:
  *            @arg RTC_DayLightSaving_SUB1H: Substract one hour (winter time)
  *            @arg RTC_DayLightSaving_ADD1H: Add one hour (summer time)
  * @param  RTC_StoreOperation: Specifies the value to be written in the BCK bit
  *                            in CR register to store the operation.
  *          This parameter can be one of the following values:
  *            @arg RTC_StoreOperation_Reset: BCK Bit Reset
  *            @arg RTC_StoreOperation_Set: BCK Bit Set
  * @retval None
  */
u32 RTC_DayLightSavingConfig(u32 RTC_DayLightSaving, u32 RTC_StoreOperation)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 CRTmp = RTC->RTC_CR;
	u32 status = 0;

	/* Check the parameters */
	assert_param(IS_RTC_DAYLIGHT_SAVING(RTC_DayLightSaving));
	assert_param(IS_RTC_STORE_OPERATION(RTC_StoreOperation));

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Clear the bits to be configured */
	CRTmp &= (u32)~(RTC_BIT_BKP);

	/* Configure the RTC_CR register */
	CRTmp |= (u32)(RTC_DayLightSaving | RTC_StoreOperation);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		/* write CR */
		RTC->RTC_CR = CRTmp;

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	//RTC_WaitForSynchro();

	return status;
}

/**
  * @brief  Returns the RTC Day Light Saving stored operation.
  * @retval RTC Day Light Saving stored operation.
  *          - RTC_StoreOperation_Reset
  *          - RTC_StoreOperation_Set
  */
u32 RTC_GetStoreOperation(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);

	return (RTC->RTC_CR & RTC_BIT_BKP);
}

/**
  * @brief  Configures the RTC output.
  * @param  RTC_Output: Specifies which signal will be routed to the RTC output.
  *          This parameter can be one of the following values:
  *            @arg RTC_Output_Disable: No output selected
  *            @arg RTC_Output_Alarm: signal of Alarm mapped to output
  *            @arg RTC_Output_clkspre: signal of clkspre mapped to output
  *            @arg RTC_Output_clkapre: signal of clkapre mapped to output
  * @retval None
  */
u32 RTC_OutputConfig(u32 RTC_Output)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 CRTmp = RTC->RTC_CR;
	u32 status;

	/* Check the parameters */
	assert_param(IS_RTC_OUTPUT(RTC_Output));

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* Clear the bits to be configured */
	CRTmp &= (u32)~(RTC_MASK_OSEL);

	/* Configure the output selection and polarity */
	CRTmp |= (u32)(RTC_Output);

	/* Set Initialization mode */
	if (RTC_EnterInitMode()) {
		/* write CR */
		RTC->RTC_CR = CRTmp;

		/* Exit Initialization mode */
		RTC_ExitInitMode();

		if (RTC_WaitForSynchro()) {
			status = 1;
		} else {
			status = 0;
		}
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/**
  * @brief  Configures the Coarse calibration parameters.
  * @param  CalibSign: CalibSign specifies the sign of the coarse calibration value.
  *          This parameter can be  one of the following values:
  *            @arg RTC_CalibSign_Positive: The value sign is positive
  *            @arg RTC_CalibSign_Negative: The value sign is negative
  * @param  Value: value of coarse calibration expressed in ppm (coded on 7 bits).
  * @param  CalibPeriod: Calibration period, Compensate DC clkapre cycles every (CALP+1) minutes (coded on 3 bits).
  *   This parameter can be one of the following values:
  *		 @arg RTC_CalibPeriod_1MIN
  *		 @arg RTC_CalibPeriod_2MIN
  *		 @arg RTC_CalibPeriod_3MIN
  *		 @arg RTC_CalibPeriod_4MIN
  *		 @arg RTC_CalibPeriod_5MIN
  *		 @arg RTC_CalibPeriod_6MIN
  *		 @arg RTC_CalibPeriod_7MIN
  *		 @arg RTC_CalibPeriod_8MIN
  * @param  Calib_Enable: RTC_Calib_Disable or RTC_Calib_Enable .
  * @retval status value:
  *          - 1: RTC Coarse calibration are initialized
  *          - 0: RTC Coarse calibration are not initialized
  */
u32 RTC_SmoothCalibConfig(u32 CalibSign, u32 Value, u32 CalibPeriod, u32 Calib_Enable)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);
	u32 counter = 0;
	u32 status = 0;
	u32 temp = 0;

	/* Check the parameters */
	assert_param(IS_RTC_CALIB_SIGN(CalibSign));
	assert_param(IS_RTC_CALIB_VALUE(Value));
	assert_param(IS_RTC_CALIB_PERIOD(CalibPeriod));
	assert_param(IS_RTC_CALIB_ENABLE(Calib_Enable));

	/* Disable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xCA);
	RTC->RTC_WPR = RTC_KEY(0x53);

	/* check if a calibration is pending*/
	/* wait until the Calibration is completed*/
	while (1) {
		temp = RTC->RTC_ISR;

		if ((temp & RTC_BIT_RECALPF) == 0) {
			break;
		}
		if (counter >= RECALPF_TIMEOUT) {
			break;
		}

		counter++;
	}

	/* check if the calibration pending is completed or if there is no calibration operation at all*/
	if ((RTC->RTC_ISR & RTC_BIT_RECALPF) == 0) {
		if (Calib_Enable == RTC_Calib_Disable) {
			RTC->RTC_CALIBR &= ~RTC_Calib_Enable;
		} else {
			RTC->RTC_CALIBR = (u32)(Value | CalibSign | Calib_Enable | CalibPeriod);
		}
		status = 1;
	} else {
		status = 0;
	}

	/* Enable the write protection for RTC registers */
	RTC->RTC_WPR = RTC_KEY(0xFF);

	return status;
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
