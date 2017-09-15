#pragma once

#include <things_common.h>

/**
 * @file things_time.h
 * @brief time API
 */

/**
 * @defgroup time time
 * @brief <b> time API </b>
 *
 * 1. dawit time module
 *  - dawit use microsecond resolution system time
 *	- dawit use utc time
 *
 * @brief <b> time module example </b>
 * @includelineno  ex_time.c
 *
 * @{
 */

/**
* @brief time_info structure
*/
typedef struct {
	int second;		/**<  Seconds  										*/
	int minute;		/**<  Minutes 										*/
	int hour;		/**<  Hour (0--23) 									*/
	int day;		/**<  Day of month (1--31) 							*/
	int month;		/**<  Month (0--11) 								*/
	int year;		/**<  Year (calendar year minus 1900) 				*/
	int day_of_week;/**<  Weekday (0--6; Sunday = 0) 					*/
	int day_of_year;/**<  Day of year (0--365) 							*/
} things_time_info;

/**
 * @brief set UTC time (microsecond resolution)
 *
 * @param[in] second       :  set time, second
 * @param[in] microsecond  :  set time, microsecond
 *
 * @return things_SUCCESS       :  success
 * @return things_FAIL          :  fail
 * @return things_INVALID_ARGS  :  input parameter invalid
 */
things_result things_time_set(unsigned int second, unsigned int microsecond);

/**
 * @brief time_info to UTC (second) time
 *
 * @param[in]   time_info   :  things_time_info
 * @param[out]  utc_second  :  utc second
 *
 * @return things_SUCCESS       :  success
 * @return things_FAIL          :  fail
 * @return things_INVALID_ARGS  :  input parameter invalid
 */
things_result things_time_info_to_utc_time(things_time_info *time_info, unsigned int *utc_second);

/**
 @}
	 */// end time group
