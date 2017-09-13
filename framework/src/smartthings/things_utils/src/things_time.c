#include <time.h>

#include <things_time.h>

things_result things_time_set(unsigned int second, unsigned int microsecond)
{
	if ((int)second < 0 || (int)microsecond < 0) {
		return THINGS_INVALID_ARGS;
	}

	struct timespec current_time;

	current_time.tv_sec = second;
	current_time.tv_nsec = (long int)microsecond * 1000;

	if (clock_settime(CLOCK_REALTIME, &current_time) != 0) {
		return THINGS_FAIL;
	}

	return THINGS_SUCCESS;
}

things_result things_time_info_to_utc_time( things_time_info *time_info, unsigned int *utc_second )
{
	if(time_info == NULL || utc_second == NULL) {
		return THINGS_INVALID_ARGS;
	}

	struct tm tm;

	tm.tm_sec  = time_info->second;
	tm.tm_min  = time_info->minute;
	tm.tm_hour = time_info->hour;
	tm.tm_mday = time_info->day;
	tm.tm_mon  = time_info->month;
	tm.tm_year = time_info->year;
	tm.tm_wday = time_info->day_of_week;
	tm.tm_yday = time_info->day_of_year;

	*utc_second = (unsigned int) mktime(&tm);

	return THINGS_SUCCESS;
}
