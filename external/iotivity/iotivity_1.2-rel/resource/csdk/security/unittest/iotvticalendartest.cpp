// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Not supported on Arduino due lack of absolute time need to implement iCalendar
#ifndef WITH_ARDUINO

#include "gtest/gtest.h"
#include "iotvticalendar.h"
#include "logger.h"

#define TAG  "CALENDAR-UT"

static void printPeriod(IotvtICalPeriod_t *period)
{
    if(NULL == period)
    {
        return;
    }

    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_year = %d",period->startDateTime.tm_year);
    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_mon = %d",period->startDateTime.tm_mon);
    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_mday = %d",period->startDateTime.tm_mday);
    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_hour = %d",period->startDateTime.tm_hour);
    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_min = %d",period->startDateTime.tm_min);
    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_sec = %d",period->startDateTime.tm_sec);

    OIC_LOG_V(INFO, TAG, "period->endDateTime.tm_year = %d",period->endDateTime.tm_year);
    OIC_LOG_V(INFO, TAG, "period->endDateTime.tm_mon = %d",period->endDateTime.tm_mon);
    OIC_LOG_V(INFO, TAG, "period->endDateTime.tm_mday = %d",period->endDateTime.tm_mday);
    OIC_LOG_V(INFO, TAG, "period->endDateTime.tm_hour = %d",period->endDateTime.tm_hour);
    OIC_LOG_V(INFO, TAG, "period->endDateTime.tm_min = %d",period->endDateTime.tm_min);
    OIC_LOG_V(INFO, TAG, "period->startDateTime.tm_sec = %d",period->endDateTime.tm_sec);
}


static void printRecur(IotvtICalRecur_t *recur)
{
    OIC_LOG_V(INFO, TAG, "recur->freq = %d", recur->freq);
    OIC_LOG_V(INFO, TAG, "recur->until.tm_year = %d", recur->until.tm_year);
    OIC_LOG_V(INFO, TAG, "recur->until.tm_mon = %d", recur->until.tm_mon);
    OIC_LOG_V(INFO, TAG, "recur->until.tm_mday = %d", recur->until.tm_mday);

    if(0 != (recur->byDay & SUNDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Sunday");
    }
    if(0 != (recur->byDay & MONDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Monday");
    }
    if(0 != (recur->byDay & TUESDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Tuesday");
    }
    if(0 != (recur->byDay & WEDNESDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Wednesday");
    }
    if(0 != (recur->byDay & THURSDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Thursday");
    }
    if(0 != (recur->byDay & FRIDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Friday");
    }
    if(0 != (recur->byDay & SATURDAY))
    {
        OIC_LOG_V(INFO, TAG, "recur->byDay = %s", "Saturday");
    }
}

#ifndef HAVE_LOCALTIME_R

#define localtime_r localtime_r_compat

/**
 * @description An implementation of `localtime_r` for systems which only
 * have a `localtime` implementation.
 */
static tm* localtime_r_compat(const time_t* timer, tm* result)
{
    if (NULL == result)
    {
        OIC_LOG(ERROR, TAG, "localtime_r received null results parameter");
    }

    if (NULL == timer)
    {
        OIC_LOG(WARNING, TAG, "localtime_r received null timer parameter");
    }

    tm* tempPtr = localtime(timer);
    memcpy(result, tempPtr, sizeof(tm));
    return result;
}
#endif

static void checkValidityOfRequest(char *recurStr, char *periodStr,int startTime, int endTime,
                                    int byDay)
{
    time_t rt;
    IotvtICalDateTime_t t1;
    IotvtICalDateTime_t t2;
    int isValidWD;
    IotvtICalResult_t result;

    memset(&t1, 0, sizeof(t1));
    memset(&t2, 0, sizeof(t2));

    do
    {
        rt = time(0);
        tzset();
        localtime_r(&rt, &t1);

        result = IsRequestWithinValidTime(periodStr, recurStr);

        rt = time(0);
        tzset();
        localtime_r(&rt, &t2);

        OIC_LOG_V(INFO, TAG, "t1 = %02d:%02d:%02d", t1.tm_hour, t1.tm_min, t1.tm_sec );
        OIC_LOG_V(INFO, TAG, "t2 = %02d:%02d:%02d", t2.tm_hour, t2.tm_min, t2.tm_sec );
    }while(t1.tm_hour != t2.tm_hour);

    if(byDay > 0)
    {
        isValidWD = (0x1 << t2.tm_wday) & byDay;
        if(isValidWD && startTime <= t2.tm_hour && endTime > t2.tm_hour)
        {
            EXPECT_EQ(IOTVTICAL_VALID_ACCESS, result);
        }
        else
        {
            EXPECT_EQ(IOTVTICAL_INVALID_ACCESS, result);
        }
    }
    else
    {
        if(startTime <= t2.tm_hour && endTime > t2.tm_hour)
        {
            EXPECT_EQ(IOTVTICAL_VALID_ACCESS, result);
        }
        else
        {
            EXPECT_EQ(IOTVTICAL_INVALID_ACCESS, result);
        }
    }
}

//ParsePeriod Tests
TEST(ParsePeriodTest, ParsePeriodValidDateTime)
{
    char periodStr[] = "20150629T153050/20150630T233055";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_SUCCESS, ParsePeriod(periodStr,&period));
    printPeriod(&period);
}

TEST(ParsePeriodTest, ParsePeriodValidDate)
{
    char periodStr[] = "20150629/20150630";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_SUCCESS, ParsePeriod(periodStr,&period));
    printPeriod(&period);
}

TEST(ParsePeriodTest, ParsePeriodMismatchStartDTEndDT1)
{
    char periodStr[] = "20150629T153050/20150630";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParsePeriodTest, ParsePeriodMismatchStartDTEndDT2)
{
    char periodStr[] = "20150629/20150630T203055";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParsePeriodTest, ParsePeriodInvalidStartDT1)
{
    char periodStr[] = "20150629T1530/20150630T203055";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParsePeriodTest, ParsePeriodInvalidEndtDT2)
{
    char periodStr[] = "20150629T153050/20150630203055";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParsePeriodTest, ParsePeriodInvalidStartD3)
{
    char periodStr[] = "201506/20150630";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParsePeriodTest, ParsePeriodInvalidEndD4)
{
    char periodStr[] = "20150629/201530";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParsePeriodTest, ParsePeriodEndDTBeforeStartDT)
{
    char periodStr[] = "20150630T203055/20150629T153050";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}
TEST(ParsePeriodTest, ParsePeriodEndDBeforeStartD)
{
    char periodStr[] = "20150630/20150629";
    IotvtICalPeriod_t period = IotvtICalPeriod_t();
    EXPECT_EQ(IOTVTICAL_INVALID_PERIOD, ParsePeriod(periodStr,&period));
}

TEST(ParseRecurTest, ParseRecurValid1)
{
    char recurStr[] = "FREQ=DAILY; UNTIL=20150703; BYDAY=MO, WE, FR";
    IotvtICalRecur_t recur = IotvtICalRecur_t();
    EXPECT_EQ(IOTVTICAL_SUCCESS, ParseRecur(recurStr, &recur));
    printRecur(&recur);
}

TEST(ParseRecurTest, ParseRecurValid2)
{
    char recurStr[] = "FREQ=DAILY";
    IotvtICalRecur_t recur =  IotvtICalRecur_t();
    EXPECT_EQ(IOTVTICAL_SUCCESS, ParseRecur(recurStr, &recur));
    printRecur(&recur);
}

TEST(ParseRecurTest, ParseRecurInValidFreq1)
{
    char recurStr[] = "FREQ=WEEKLY; UNTIL=20150703; BYDAY=TU";
    IotvtICalRecur_t recur = IotvtICalRecur_t();
    EXPECT_EQ(IOTVTICAL_INVALID_RRULE, ParseRecur(recurStr, &recur));
}

TEST(ParseRecurTest, ParseRecurInValidFreq2)
{
    char recurStr[] = "UNTIL=20150703; BYDAY=TU";
    IotvtICalRecur_t recur =  IotvtICalRecur_t();
    EXPECT_EQ(IOTVTICAL_INVALID_RRULE, ParseRecur(recurStr, &recur));
}

TEST(ParseRecurTest, ParseRecurInValidUntil)
{
    char recurStr[] = "FREQ=DAILY; UNTIL=20150703T095055; BYDAY=MO, WE, FR";
    IotvtICalRecur_t recur = IotvtICalRecur_t();
    EXPECT_EQ(IOTVTICAL_INVALID_RRULE, ParseRecur(recurStr, &recur));
}

TEST(ParseRecurTest, ParseRecurInValidByday)
{
    char recurStr[] = "FREQ=DAILY; UNTIL=20150703; BYDAY=";
    IotvtICalRecur_t recur = IotvtICalRecur_t();
    EXPECT_EQ(IOTVTICAL_INVALID_RRULE, ParseRecur(recurStr, &recur));
}

TEST(IsRequestWithinValidTimeTest, IsRequestWithinValidTimeValidPeriod1)
{
    char periodStr[] = "20150630/20551230";
    EXPECT_EQ(IOTVTICAL_VALID_ACCESS, IsRequestWithinValidTime(periodStr, NULL));
}

TEST(IsRequestWithinValidTimeTest, IsRequestWithinValidTimeValidPeriodAndRecur1)
{
    //Daily on days MO, WE & FR from 6:00:00am to 8:00:00pm until 20551230
    char recurStr[] = "FREQ=DAILY; UNTIL=20551230; BYDAY=MO, WE, FR";
    char periodStr[] = "20150630T060000/20150630T200000";
    int startTime = 6;
    int endTime = 20;
    int byDay = MONDAY | WEDNESDAY | FRIDAY;

    checkValidityOfRequest(recurStr, periodStr, startTime, endTime, byDay);
}

TEST(IsRequestWithinValidTimeTest, IsRequestWithinValidTimeValidPeriodAndRecur2)
{
    //Daily forever from 6:00:00am to 8:00:00pm
    char recurStr[] = "FREQ=DAILY";
    char periodStr[] = "20150630T060000/20150630T200000";
    int startTime = 6;
    int endTime = 20;
    int byDay = -1;

    checkValidityOfRequest(recurStr, periodStr, startTime, endTime, byDay);
}

TEST(IsRequestWithinValidTimeTest, IsRequestWithinValidTimeValidPeriodAndRecur3)
{
    //Daily until 20551230 from 6:00:00am to 8:00:00pm
    char recurStr[] = "FREQ=DAILY; UNTIL=20551230";
    char periodStr[] = "20150630T060000/20150630T200000";
    int startTime = 6;
    int endTime = 20;
    int byDay = -1;

    checkValidityOfRequest(recurStr, periodStr, startTime, endTime, byDay);

}

TEST(IsRequestWithinValidTimeTest, IsRequestWithinValidTimeValidPeriodAndRecur4)
{
    //Daily forever on days MO, WE & Fr from 6:00:00am to 8:00:00pm
    char recurStr[] = "FREQ=DAILY; BYDAY=MO, WE, FR";
    char periodStr[] = "20150630T060000/20150630T200000";
    int startTime = 6;
    int endTime = 20;
    int byDay = MONDAY | WEDNESDAY | FRIDAY;

    checkValidityOfRequest(recurStr, periodStr, startTime, endTime, byDay);

 }

TEST(IsRequestWithinValidTimeTest, IsRequestWithinValidTimeInValidPeriodAndValidRecur)
{
    //Daily forever on days MO, WE & Fr
    char recurStr[] = "FREQ=DAILY; BYDAY=MO, WE, FR";
    char periodStr[] = "20150630/20150730";

    EXPECT_EQ(IOTVTICAL_INVALID_ACCESS, IsRequestWithinValidTime(periodStr, recurStr));
}

#endif
