/** mbed Microcontroller Library
 ******************************************************************************
 * @file	rtc_api.c
 * @author
 * @version V1.0.0
 * @date	2016-08-01
 * @brief	This file provides mbed API for RTC.
 ******************************************************************************
 * @attention
 *
 * This module is a confidential and proprietary property of RealTek and
 * possession or use of this module requires written permission of RealTek.
 *
 * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
 ******************************************************************************
 */

#include "rtc_api.h"
#include <time.h>

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_RTC
 *  @brief    MBED_RTC driver modules.
 *  @{
 */

/* group: MBED_RTC Exported Types */
/* Since all the members in the group are static which will not be extracted to the doxygen doc,
  original comment has been deleted to avoid there is nothing displayed under the group. */

static struct tm rtc_timeinfo;
static int rtc_en = 0;
static alarm_irq_handler rtc_alarm_handler;

static const u8 dim[12] = {
	31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
/* end of the group */

/** @defgroup MBED_RTC_Exported_Functions MBED_RTC Exported Functions
  * @{
  */

/**
  * @brief  Judge whether a year is a leap year or not.
  * @param  year: Actual year - 1900.
  * @return Result.
  * @retval 1: This year is a leap year.
  * @retval 0: This year is not a leap year.
  */
static inline bool is_leap_year(unsigned int year)
{
	u32 full_year = year + 1900;
	return (!(full_year % 4) && (full_year % 100)) || !(full_year % 400);
}

/**
  * @brief  Calculate total days in a specified month of a specified year.
  * @param  year: Actual year - 1900.
  * @param  month: Specified month, which can be 0~11.
  * @note 0 represents January.
  * @return Number of days in the month of the year.
  */
static u8 days_in_month(u8 month, u8 year)
{
	u8 ret = dim[month];
	if (ret == 0) {
		ret = is_leap_year(year) ? 29 : 28;
	}
	return ret;
}

/**
  * @brief  Calculate month and day of the month according to year and day of the year.
  * @param  year: Actual year - 1900.
  * @param  yday: Day of the year.
  * @param  mon: Pointer to the variable that stores month, which can be 0~11.
  * @note 0 represents January.
  * @param  mday: Pointer to the variable that stores day of month, which can be 1~31.
  * @retval none
  */
static void rtc_calculate_mday(int year, int yday, int *mon, int *mday)
{
	int t_mon = -1, t_yday = yday + 1;

	while (t_yday > 0) {
		t_mon ++;
		t_yday -= days_in_month(t_mon, year);
	}

	*mon = t_mon;
	*mday = t_yday + days_in_month(t_mon, year);
}

/**
  * @brief  Calculate the day of a week according to date.
  * @param  year: Actual year - 1900.
  * @param  mon: Month of the year, which can be 0~11.
  * @note 0 represents January.
  * @param  mday: Day of the month.
  * @param  wday: Pointer to the variable that stores day of a week, which can be 0~6.
  * @note 0 represents Sunday.
  * @retval none
  */
static void rtc_calculate_wday(int year, int mon, int mday, int *wday)
{
	int t_year = year + 1900, t_mon = mon + 1;

	if (t_mon == 1 || t_mon == 2) {
		t_year --;
		t_mon += 12;
	}

	int c = t_year / 100;
	int y = t_year % 100;
	int week = (c / 4) - 2 * c + (y + y / 4) + (26 * (t_mon + 1) / 10) + mday - 1;

	while (week < 0) {
		week += 7;
	}
	week %= 7;

	*wday = week;
}

/**
  * @brief  Restore global rtc_timeinfo variable whose value is lost after system reset.
  * @param  none
  * @retval none
  * @attention Make sure RESTORE bit in RTC_YEAR register has been assertted.
  */
static void rtc_restore_timeinfo(void)
{
	int days_in_year;

	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	rtc_timeinfo.tm_sec = RTC_TimeStruct.RTC_Seconds;
	rtc_timeinfo.tm_min = RTC_TimeStruct.RTC_Minutes;
	rtc_timeinfo.tm_hour = RTC_TimeStruct.RTC_Hours;
	rtc_timeinfo.tm_yday = RTC_TimeStruct.RTC_Days;
	rtc_timeinfo.tm_year = RTC_TimeStruct.RTC_Year - 1900;

	days_in_year = (is_leap_year(rtc_timeinfo.tm_year) ? 366 : 365);
	if (rtc_timeinfo.tm_yday > (days_in_year - 1)) {
		rtc_timeinfo.tm_year ++;
		rtc_timeinfo.tm_yday -= days_in_year;

		/* over one year, update year in RTC_YEAR and days in RTC_TR */
		RTC_TimeStruct.RTC_Days = rtc_timeinfo.tm_yday;
		RTC_TimeStruct.RTC_Year = rtc_timeinfo.tm_year + 1900;
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
	}

	rtc_calculate_mday(rtc_timeinfo.tm_year, rtc_timeinfo.tm_yday, &rtc_timeinfo.tm_mon, &rtc_timeinfo.tm_mday);
	rtc_calculate_wday(rtc_timeinfo.tm_year, rtc_timeinfo.tm_mon, rtc_timeinfo.tm_mday, &rtc_timeinfo.tm_wday);
}

/**
  * @brief  Backup tm_year parameter in global rtc_timeinfo variable before system reset.
  * @param  none
  * @retval none
  */
void rtc_backup_timeinfo(void)
{
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);

	/* set RTC_BIT_RESTORE bit to '1' before system reset */
	RTC->RTC_YEAR |= 0x80000000;
}

/**
  * @brief  Initialize the RTC device, including clock, function and RTC registers.
  * @param  none
  * @retval  none
  */
void rtc_init(void)
{
	/* Enable RTC function and clock */
	RCC_PeriphClockCmd(APBPeriph_RTC, APBPeriph_RTC_CLOCK, ENABLE);

	RTC_InitTypeDef RTC_InitStruct;

	RTC_StructInit(&RTC_InitStruct);
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;

	RTC_Init(&RTC_InitStruct);

	RTC_BypassShadowCmd(ENABLE);

	/* 32760 need add need add 15 cycles (256Hz) every 4 min*/
	//RTC_SmoothCalibConfig(RTC_CalibSign_Positive, 15,
	//	RTC_CalibPeriod_4MIN, RTC_Calib_Enable);

	rtc_en = 1;
}

/**
  * @brief  Deinitialize the RTC device.
  * @param  none
  * @retval  none
  */
void rtc_free(void)
{
	rtc_en = 0;
	rtc_alarm_handler = NULL;
}

/**
  * @brief  Judge whether RTC is enabled or not.
  * @param  none
  * @return RTC status. It can be one of the following values:
  * @retval 1: RTC has been enabled.
  * @retval 0: RTC has not been enabled.
  */
int rtc_isenabled(void)
{
	return rtc_en;
}

/**
  * @brief  Set the specified timestamp in seconds to RTC.
  * @param  t: Seconds from 1970.1.1 00:00:00 to specified data and time which is to be set.
  * @retval  none
  */
void rtc_write(time_t t)
{
	/* Convert the time in to a tm*/
	struct tm *timeinfo = localtime(&t);

	RTC_TimeTypeDef RTC_TimeStruct;

	/*set time in RTC */
	RTC_TimeStruct.RTC_H12_PMAM = RTC_H12_AM;
	RTC_TimeStruct.RTC_Year = timeinfo->tm_year + 1900;
	RTC_TimeStruct.RTC_Days = timeinfo->tm_yday;
	RTC_TimeStruct.RTC_Hours = timeinfo->tm_hour;
	RTC_TimeStruct.RTC_Minutes = timeinfo->tm_min;
	RTC_TimeStruct.RTC_Seconds = timeinfo->tm_sec;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

	/* Set rtc_timeinfo*/
	_memcpy((void *)&rtc_timeinfo, (void *)timeinfo, sizeof(struct tm));
}

/**
  * @brief  Get current timestamp in seconds from RTC.
  * @param  none
  * @return Current timestamp in seconds which is calculated from 1970.1.1 00:00:00.
  */
time_t rtc_read(void)
{
	time_t t;
	struct tm tm_temp;
	RTC_TimeTypeDef RTC_TimeStruct;
	u32 delta_days = 0;
	RTC_TypeDef *RTC = ((RTC_TypeDef *) RTC_BASE);

	if ((0x80000000 & (RTC->RTC_YEAR)) != 0) {
		rtc_restore_timeinfo();
		RTC->RTC_YEAR &= (~0x80000000);
	}

	_memcpy((void *)&tm_temp, (void *)&rtc_timeinfo, sizeof(struct tm));

	/*hour, min, sec get from RTC*/
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	tm_temp.tm_sec = RTC_TimeStruct.RTC_Seconds;
	tm_temp.tm_min = RTC_TimeStruct.RTC_Minutes;
	tm_temp.tm_hour = RTC_TimeStruct.RTC_Hours;

	/* calculate how many days later from last time update rtc_timeinfo */
	delta_days = RTC_TimeStruct.RTC_Days - tm_temp.tm_yday;

	/* calculate  wday, mday, yday, mon, year*/
	tm_temp.tm_wday += delta_days;
	if (tm_temp.tm_wday >= 7) {
		tm_temp.tm_wday = tm_temp.tm_wday % 7;
	}

	tm_temp.tm_yday += delta_days;
	tm_temp.tm_mday += delta_days;

	while (tm_temp.tm_mday > days_in_month(tm_temp.tm_mon, tm_temp.tm_year)) {
		tm_temp.tm_mday -= days_in_month(tm_temp.tm_mon, tm_temp.tm_year);
		tm_temp.tm_mon++;

		if (tm_temp.tm_mon >= 12) {
			tm_temp.tm_mon -= 12;
			tm_temp.tm_yday -= is_leap_year(tm_temp.tm_year) ? 366 : 365;
			tm_temp.tm_year ++;

			/* over one year, update days in RTC_TR */
			RTC_TimeStruct.RTC_Days = tm_temp.tm_yday;
			RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

			/* update rtc_timeinfo */
			_memcpy((void *)&rtc_timeinfo, (void *)&tm_temp, sizeof(struct tm));
		}
	}

	/* Convert to timestamp(seconds from 1970.1.1 00:00:00)*/
	t = mktime(&tm_temp);

	return t;
}


/**
  * @brief  Disable RTC Alarm.
  * @param  none
  * @retval   none
  */
void rtc_disable_alarm(void)
{
	InterruptDis(RTC_IRQ);
	InterruptUnRegister(RTC_IRQ);
	RTC_AlarmCmd(DISABLE);

	rtc_alarm_handler = NULL;
}

/**
  * @brief  Register RTC alarm interrupt handler.
  * @param  data: RTC IRQ callback data.
  * @retval  0
  */
u32 rtc_alarm_intr_handler(void *data)
{
	/* To avoid gcc warnings */
	(void) data;

	alarm_irq_handler hdl;

	/*clear alarm flag*/
	RTC_AlarmClear();

	/* execute user handler*/
	if (rtc_alarm_handler != NULL) {
		hdl = (alarm_irq_handler)rtc_alarm_handler;
		hdl();
	}

	/*disable alarm*/
	rtc_disable_alarm();

	return 0;
}

/**
  * @brief  Set the specified RTC Alarm and interrupt.
  * @param  alarm: Alarm object defined in application software.
  * @param  alarmHandler:  Alarm interrupt callback function.
  * @return Status. It can be one of the following values:
  * @retval 1: Success.
  * @retval Others: Error.
  */
u32 rtc_set_alarm(alarm_t *alrm, alarm_irq_handler alarmHandler)
{
	RTC_AlarmTypeDef RTC_AlarmStruct_temp;

	rtc_alarm_handler = alarmHandler;

	/* set alarm */
	RTC_AlarmStructInit(&RTC_AlarmStruct_temp);
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_H12_PMAM = RTC_H12_AM;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Days = alrm->yday;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Hours = alrm->hour;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Minutes = alrm->min;
	RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Seconds = alrm->sec;

	RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_None;
	RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_None;

	RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStruct_temp);

	RTC_AlarmCmd(ENABLE);
	InterruptRegister((IRQ_FUN)rtc_alarm_intr_handler, RTC_IRQ, (u32)alrm, 5);
	InterruptEn(RTC_IRQ, 5);

	return _TRUE;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
