//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Not supported on Arduino due lack of absolute time need to implement iCalendar
#if !defined(WITH_ARDUINO)

#define _XOPEN_SOURCE  //Needed by strptime
#include "iotivity_config.h"
#include <string.h>
#include "iotvticalendar.h"
#include "oic_string.h"

#ifndef HAVE_STRPTIME
char *strptime(const char *buf, const char *fmt, struct tm *tm);
#endif

static char dtFormat[] =  "%Y%m%dT%H%M%S"; //date-time format
static char dFormat[] =  "%Y%m%d";         // date format

static const char FREQ[]  = "FREQ";
static const char UNTIL[] = "UNTIL";
static const char BYDAY[] = "BYDAY";
static const char DAILY[] = "DAILY";

IotvtICalResult_t ParsePeriod(const char *periodStr, IotvtICalPeriod_t *period)
{
    if ((NULL == periodStr) || (NULL == period))
    {
        return IOTVTICAL_INVALID_PARAMETER;
    }

    char *endDTPos;
    char *fmt = "";
    int   startDTLen;
    int   endDTLen;

    //Finding length of startDateTime and endDateTime in period
    //startDateTime and endDateTime can have form YYYYmmdd or YYYYmmddTHHMMSS
    //startDateTime and endDateTime must be same form
    //Eg: periodStr = "20150629T153050/20150630T203055"
    //    periodStr = "20150629/20150630"
    if (NULL == (endDTPos = strchr(periodStr, '/')))
    {
        return IOTVTICAL_INVALID_PERIOD;
    }
    endDTPos += 1;
    startDTLen = endDTPos - periodStr - 1;
    endDTLen   = strlen(endDTPos);

    //Checking if both startDateTime and endDateTime are of same form
    if (startDTLen == endDTLen)
    {
        if (8 == startDTLen) //YYYYmmdd
        {
            fmt = dFormat;
        }
        else if (15 == startDTLen) //YYYYmmddTHHMMSS
        {
            fmt = dtFormat;
        }
        else
        {
            return IOTVTICAL_INVALID_PERIOD;
        }
    }
    else
    {
        return IOTVTICAL_INVALID_PERIOD;
    }

    //Checking if startDateTime has right format
    if (NULL != strptime(periodStr, fmt, &period->startDateTime))
    {
        //Checking if endDateTime has right format
        if (NULL != strptime(endDTPos, fmt, &period->endDateTime))
        {
            //Checking if endDateTime is after startDateTime
            if ((period->startDateTime.tm_year > period->endDateTime.tm_year)
                || ((period->startDateTime.tm_year == period->endDateTime.tm_year)
                    && (period->startDateTime.tm_mon > period->endDateTime.tm_mon))
                || ((period->startDateTime.tm_year == period->endDateTime.tm_year)
                    && (period->startDateTime.tm_mon == period->endDateTime.tm_mon)
                    && (period->startDateTime.tm_mday > period->endDateTime.tm_mday))
                || (( fmt == dtFormat) && (period->startDateTime.tm_year == period->endDateTime.tm_year)
                    && (period->startDateTime.tm_mon == period->endDateTime.tm_mon)
                    && (period->startDateTime.tm_mday == period->endDateTime.tm_mday)
                    && (period->startDateTime.tm_hour > period->endDateTime.tm_hour))
                || (( fmt == dtFormat) && (period->startDateTime.tm_year == period->endDateTime.tm_year)
                    && (period->startDateTime.tm_mon == period->endDateTime.tm_mon)
                    && (period->startDateTime.tm_mday == period->endDateTime.tm_mday)
                    && (period->startDateTime.tm_hour == period->endDateTime.tm_hour)
                    && (period->startDateTime.tm_min > period->endDateTime.tm_min))
                || (( fmt == dtFormat) && (period->startDateTime.tm_year == period->endDateTime.tm_year)
                    && (period->startDateTime.tm_mon == period->endDateTime.tm_mon)
                    && (period->startDateTime.tm_mday == period->endDateTime.tm_mday)
                    && (period->startDateTime.tm_hour == period->endDateTime.tm_hour)
                    && (period->startDateTime.tm_min == period->endDateTime.tm_min)
                    && (period->startDateTime.tm_sec > period->endDateTime.tm_sec)))
            {
                return IOTVTICAL_INVALID_PERIOD;
            }
            else
            {
                return IOTVTICAL_SUCCESS;
            }
        }
    }
    return IOTVTICAL_INVALID_PERIOD;
}

/**
 * Parses untilRule and populate "until" field of struct IotvtICalRecur_t.
 *
 * @param untilRule is a string to be parsed.
 * @param recur is the reference to the @ref IotvtICalRecur_t to be populated.
 *
 * @return ::IOTVTICAL_SUCCESS is succesful, else in case of error
 * ::IOTVTICAL_ERROR, if untilRule has invalid format or ::IOTVTICAL_INVALID_SUCCESS,
 * if no error while parsing.
 */
static IotvtICalResult_t ParseDate(char *untilRule, IotvtICalRecur_t *recur)
{
    char *date = strchr(untilRule, '=');

    if (NULL == date)
    {
        return IOTVTICAL_ERROR;
    }
    date += 1;

    if (strlen(date) == 8) //YYYYmmdd
    {
        if (NULL != strptime(date, dFormat, &recur->until))
        {
            return IOTVTICAL_SUCCESS;
        }
    }
    return IOTVTICAL_ERROR;
}

/**
 * Parses bydayRule and populate "byDay" field of struct @ref IotvtICalRecur_t.
 *
 * @param bydayRule is a string to be parsed.
 * @param recur is a reference to @ref IotvtICalRecur_t struct to be populated.
 *
 * @return ::IOTVTICAL_SUCCESS is succesful, else in case of error ::IOTVTICAL_ERROR,
 * if bydayRule has empty weekday list or invalid weekdays.
 */
static IotvtICalResult_t  ParseByday(char *bydayRule, IotvtICalRecur_t *recur)
{
    if (strstr(bydayRule, "SU"))
    {
        recur->byDay = recur->byDay | SUNDAY;
    }
    if (strstr(bydayRule, "MO"))
    {
        recur->byDay = recur->byDay | MONDAY;
    }
    if (strstr(bydayRule, "TU"))
    {
        recur->byDay = recur->byDay | TUESDAY;
    }
    if (strstr(bydayRule, "WE"))
    {
        recur->byDay = recur->byDay | WEDNESDAY;
    }
    if (strstr(bydayRule, "TH"))
    {
        recur->byDay = recur->byDay | THURSDAY;
    }
    if (strstr(bydayRule, "FR"))
    {
        recur->byDay = recur->byDay | FRIDAY;
    }
    if (strstr(bydayRule, "SA"))
    {
        recur->byDay = recur->byDay | SATURDAY;
    }

    //Checking if byDay list is empty or has inValid weekdays
    if (recur->byDay == NO_WEEKDAY)
    {
        return IOTVTICAL_ERROR;
    }

    return IOTVTICAL_SUCCESS;
}

IotvtICalResult_t ParseRecur(const char *recurStr, IotvtICalRecur_t *recur)
{
    if ((NULL == recurStr) || (NULL == recur))
    {
        return IOTVTICAL_INVALID_PARAMETER;
    }

    const char *startPos="";
    const char *endPos="";
    char        buf[50];
    int         freqFlag = 0; //valid RRULE must have "FREQ" parameter.
                              //flag to track if RRULE has "FREQ" or not

    startPos = recurStr;
    //Iterates though recurrence rule
    //Eg, RRULE: FREQ=DAILY; UNTIL=20150703; BYDAY=MO, WE, FR
    while ('\0' != startPos)
    {
        endPos = strchr(startPos, ';');
        if (endPos)
        {
            endPos += 1;
        }
        OICStrcpy(buf, (endPos - startPos), startPos);
        if (NULL != strstr(buf, FREQ))
        {
            if (NULL != strstr(buf, DAILY))
            {
                recur->freq = FREQ_DAILY;
                freqFlag = 1;
            }
            else
            {
                return IOTVTICAL_INVALID_RRULE;
            }
        }
        else if (NULL != strstr(buf, UNTIL))
        {
            if (IOTVTICAL_SUCCESS != ParseDate(buf, recur))
            {
                return IOTVTICAL_INVALID_RRULE;
            }
        }
        else if (NULL != strstr(buf, BYDAY))
        {
            if (IOTVTICAL_SUCCESS != ParseByday(buf, recur))
            {
                return IOTVTICAL_INVALID_RRULE;
            };
        }
        startPos = endPos;
    }

    if (1 != freqFlag)
    {
        return IOTVTICAL_INVALID_RRULE;
    }

    return IOTVTICAL_SUCCESS;
}

/**
 * Computes number of days between two dates.
 *
 * @param date1 earlier date.
 * @param date2 later date.
 *
 * @return  number of days between date1 & date2.
 */
static int DiffDays(IotvtICalDateTime_t *date1, IotvtICalDateTime_t *date2)
{
    int days;
    int leapDays=0;

    if (date2->tm_year > date1->tm_year)
    {
        for (int y = date1->tm_year; y < date2->tm_year; y++)
        {
            y += TM_YEAR_OFFSET;
            if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0))
            {
               leapDays += 1;
            }
        }
    }

    days = (365 * date2->tm_year + date2->tm_yday + leapDays) -
           (365 * date1->tm_year + date1->tm_yday);

    return days;
}

/**
 * Computes number of seconds between two time.
 *
 * @param time1 earlier time.
 * @param date2 later time.
 *
 * @return  number of seconds between time1 and time2.
 */
static int DiffSecs(IotvtICalDateTime_t *time1, IotvtICalDateTime_t *time2)
{
    return (3600 * time2->tm_hour + 60 * time2->tm_min + time2->tm_sec) -
           (3600 * time1->tm_hour + 60 * time1->tm_min + time1->tm_sec);
}

/**
 * Validates if the @param currentTime is with in allowable period.
 *
 * @param period allowable period.
 * @param currentTime the time that need to be validated against allowable time.
 *
 * @return ::IOTVTICAL_VALID_ACCESS, if the request is within valid time period.
 * ::IOTVTICAL_INVALID_ACCESS, if the request is not within valid time period.
 * ::IOTVTICAL_INVALID_PARAMETER, if parameter are invalid.
 */
static IotvtICalResult_t ValidatePeriod(IotvtICalPeriod_t *period, IotvtICalDateTime_t *currentTime)
{
    if (NULL == period || NULL == currentTime)
    {
        return IOTVTICAL_INVALID_PARAMETER;
    }

    bool validStartTime = true;
    bool validEndTime = true;
    bool validDay = false;
    bool todayIsStartDay = (0 == DiffDays(&period->startDateTime, currentTime)) ? true : false;
    bool todayIsEndDay = (0 == DiffDays(currentTime, &period->endDateTime)) ? true : false;

    //If today is the start day of the allowable period then check
    //currentTime > allowable period startTime
    if (todayIsStartDay)
    {
        validStartTime = (0 <= DiffSecs(&period->startDateTime, currentTime)) ? true : false;
    }

    //If today is the end day of allowable period then check
    //currentTime < allowable period endTime
    if (todayIsEndDay)
    {
        validEndTime = (0 <= DiffSecs(currentTime, &period->endDateTime)) ? true :false;
    }

    //Check if today is valid day between startDate and EndDate inclusive
    if ((0 <= DiffDays(&period->startDateTime, currentTime)) &&
       (0 <= DiffDays(currentTime, &period->endDateTime)))
    {
        validDay = true;
    }

    if (validDay && validStartTime && validEndTime)
    {
        return IOTVTICAL_VALID_ACCESS;
    }
    else
    {
        return IOTVTICAL_INVALID_ACCESS;
    }
}

IotvtICalResult_t IsRequestWithinValidTime(const char *periodStr, const char *recurStr)
{
    //NULL recur rule means no recurring patter exist.
    //Period can't be null. Period is used with or without
    //recur rule to compute allowable access time.
    if (NULL == periodStr)
    {
        return IOTVTICAL_INVALID_PARAMETER;
    }

    IotvtICalPeriod_t period = {.startDateTime={.tm_sec=0}};
    IotvtICalRecur_t recur = {.freq=0};
    IotvtICalResult_t ret = IOTVTICAL_INVALID_ACCESS;

    time_t rawTime = time(0);
    IotvtICalDateTime_t *currentTime = localtime(&rawTime);

    ret  = ParsePeriod(periodStr, &period);
    if (ret != IOTVTICAL_SUCCESS)
    {
        return ret;
    }

    //If recur is NULL then the access time is between period's startDateTime and endDateTime
    if (NULL == recurStr)
    {
        ret = ValidatePeriod(&period, currentTime);
    }

    //If recur is not NULL then the access time is between period's startTime and
    //endTime on days specified in "BYDAY" list. The first instance of recurrence
    //is computed from period's startDate and the last instance is computed from
    //"UNTIL". If "UNTIL" is not specified then the recurrence goes for forever.
    //Eg, RRULE: FREQ=DAILY; UNTIL=20150703; BYDAY=MO, WE, FR
    if (NULL != recurStr)
    {
        ret = ParseRecur(recurStr, &recur);
        if (ret != IOTVTICAL_SUCCESS)
        {
            return ret;
        }

        if ((0 <= DiffSecs(&period.startDateTime, currentTime))&&
           (0 <= DiffSecs(currentTime, &period.endDateTime)) &&
           (0 <= DiffDays(&period.startDateTime, currentTime)))
        {
            IotvtICalDateTime_t emptyDT = {.tm_sec=0};
            ret = IOTVTICAL_VALID_ACCESS;

            //"UNTIL" is an optional parameter of RRULE, checking if until present in recur
            if (0 != memcmp(&recur.until, &emptyDT, sizeof(IotvtICalDateTime_t)))
            {
                if(0 > DiffDays(currentTime, &recur.until))
                {
                    ret = IOTVTICAL_INVALID_ACCESS;
                }
            }

            //"BYDAY" is an optional parameter of RRULE, checking if byday present in recur
            if (NO_WEEKDAY != recur.byDay)
            {

                int isValidWD = (0x1 << currentTime->tm_wday) & recur.byDay; //Valid weekdays
                if (!isValidWD)
                {
                    ret = IOTVTICAL_INVALID_ACCESS;
                }
             }
        }
        else
        {
            ret = IOTVTICAL_INVALID_ACCESS;
        }
    }
    return ret;
}
#endif
