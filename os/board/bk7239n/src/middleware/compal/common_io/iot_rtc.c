// Copyright 2020-2021 Beken 
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include <os/mem.h>

#include "iot_rtc.h"

#include <driver/aon_rtc.h>
#include "aon_rtc_driver.h"
#include "aon_rtc_hal.h"

#include "sys_driver.h"
#include "portmacro.h"

#include "sys/time.h"
#include "time.h"


#define RTC_TAG "RTC"
#ifdef RTC_TAG
#define RTC_LOGI(...) BK_LOGI(RTC_TAG, ##__VA_ARGS__)
#define RTC_LOGW(...) BK_LOGW(RTC_TAG, ##__VA_ARGS__)
#define RTC_LOGE(...) BK_LOGE(RTC_TAG, ##__VA_ARGS__)
#define RTC_LOGD(...) BK_LOGD(RTC_TAG, ##__VA_ARGS__)
#else
#define RTC_LOGI(...) 
#define RTC_LOGW(...) 
#define RTC_LOGE(...) 
#define RTC_LOGD(...) 

#endif


#define USEC_TO_SEC (1000000)
#define INVALID_VAL_YEAR  (0xFFFF)
#define LIMIT_VAL_MONTH    (12)
#define LIMIT_VAL_DAY       (31)
#define LIMIT_VAL_HOUR      (23)
#define LIMIT_VAL_MINUTE    (59)
#define LIMIT_VAL_SECOND    (59)
#define LIMIT_VAL_WDAY      (7)


typedef struct IotRtcDescriptor
{
    IotRtcStatus_t status;//work status
    void (*func)(IotRtcStatus_t arg1,void *arg2);
    void *arg2;
    
    time_t uUTC_time;
    struct timeval seconds;
    
    uint8_t alarmName[ALARM_NAME_MAX_LEN];
    uint32_t uWakeUpTick;
}IotRtcDescriptor_t;

int64_t s_microseconds_offset=0;
/*************************User driver related***********************************/
int gettimeofday(struct timeval *tv, void *ptz)
{
    (void)ptz;
    
    if(tv!=NULL)
    {
        uint64_t uCurTimeUs=s_microseconds_offset+bk_aon_rtc_get_us();

        tv->tv_sec=uCurTimeUs/1000000;
        tv->tv_usec=uCurTimeUs%1000000;
        RTC_LOGI("%s sec:%d us:%d \n",__func__,tv->tv_sec,tv->tv_usec);
    }

    return IOT_RTC_SUCCESS;

}

int settimeofday(const struct timeval *tv,const struct timezone *tz)
{
    (void)tz;
    if(tv)
    {
        uint64_t setTimeUs=((uint64_t)tv->tv_sec)*1000000LL+tv->tv_usec ;
        uint64_t getCurTimeUs=bk_aon_rtc_get_us();
        s_microseconds_offset=setTimeUs-getCurTimeUs;
        RTC_LOGI("%s sec:%d us:%d \n",__func__,tv->tv_sec,tv->tv_usec);
        RTC_LOGI("%s set:%d cur:%d diff:%d \n",__func__,setTimeUs,getCurTimeUs,s_microseconds_offset);

    }
    return IOT_RTC_SUCCESS;
}

static bool is_rtc_opened=false;

IotRtcHandle_t iot_rtc_open( int32_t lRtcInstance )
{
    if(lRtcInstance!=0)
    {
        RTC_LOGE("%s Err idx\n",__func__);
        return NULL;
    }
    if(is_rtc_opened==true)
    {
        RTC_LOGE("%s ReOpen Err\n",__func__);
        return NULL;
    }
    
    /*bk_aon_rtc_tick_init started in pm.c*/

    IotRtcHandle_t time_rtc_instance=(IotRtcDescriptor_t*)os_zalloc(sizeof(IotRtcDescriptor_t));
    if(time_rtc_instance==NULL)
    {
        RTC_LOGE("There is not enough ememory for RTC\n");
        return NULL;
    }
    is_rtc_opened=true;
    time_rtc_instance->status=eRtcTimerStopped;
//    time_rtc_instance->rtc_status=IOT_RTC_NOT_STARTED;
    RTC_LOGI("%s Opened\n",__func__);
    return time_rtc_instance;
}

int32_t iot_rtc_set_datetime( IotRtcHandle_t const pxRtcHandle,
                              const IotRtcDatetime_t * pxDatetime )
{
    if((pxRtcHandle==NULL)||(pxDatetime==NULL))
    {
        RTC_LOGE("%s handle err\n",__func__);
        return IOT_RTC_INVALID_VALUE;
    }
    if(pxDatetime->usYear==INVALID_VAL_YEAR)
    {
        RTC_LOGE("%s param err\n",__func__);
        return IOT_RTC_INVALID_VALUE;
    }
    if((pxDatetime->ucMonth>LIMIT_VAL_MONTH)||(pxDatetime->ucDay>LIMIT_VAL_DAY)||(pxDatetime->ucHour>LIMIT_VAL_HOUR)
    ||(pxDatetime->ucMinute>LIMIT_VAL_MINUTE)||(pxDatetime->ucSecond>LIMIT_VAL_SECOND)||(pxDatetime->ucWday>LIMIT_VAL_WDAY))
    {
        RTC_LOGE("%s over limit \n",__func__);
        return IOT_RTC_SET_FAILED;
    }
    //do the action
    struct tm tmp;
    tmp.tm_sec=pxDatetime->ucSecond;
    tmp.tm_min=pxDatetime->ucMinute;
    tmp.tm_hour=pxDatetime->ucHour;
    tmp.tm_mday=pxDatetime->ucDay;
    tmp.tm_mon=pxDatetime->ucMonth;
    tmp.tm_year=pxDatetime->usYear;
    tmp.tm_wday=pxDatetime->ucWday;

    IotRtcHandle_t rtc_instance=pxRtcHandle;
    rtc_instance->seconds.tv_sec=mktime(&tmp);
    RTC_LOGI("%s  set Sec %d us:%d \n",__func__,rtc_instance->seconds.tv_sec,rtc_instance->seconds.tv_usec);

    if(settimeofday(&rtc_instance->seconds,NULL)!=IOT_RTC_SUCCESS)
    {
        return IOT_RTC_SET_FAILED;
    }

    rtc_instance->status=eRtcTimerRunning;
//    rtc_instance->rtc_status=eRtcTimerRunning;

    return IOT_RTC_SUCCESS;
}

int32_t iot_rtc_get_datetime( IotRtcHandle_t const pxRtcHandle,
                              IotRtcDatetime_t * pxDatetime )
{
    if((pxRtcHandle==NULL)||(pxDatetime==NULL))
    {
        RTC_LOGE("%s err value\n",__func__);
        return IOT_RTC_INVALID_VALUE;
    }

    IotRtcHandle_t rtc_instance=pxRtcHandle;
    if(rtc_instance->status==eRtcTimerStopped)
    {
        RTC_LOGE("%s rtc not started\n",__func__);
        return IOT_RTC_NOT_STARTED;
    }

    struct timeval s_get_time;
  
    if(gettimeofday(&s_get_time,NULL)!=IOT_RTC_SUCCESS)
    {
        RTC_LOGE("%s get cur time err\n",__func__);
        return IOT_RTC_GET_FAILED;
    }
    
    struct tm *tmp;
    tmp=gmtime(&s_get_time.tv_sec);
    pxDatetime->usYear=tmp->tm_year;
    pxDatetime->ucMonth=tmp->tm_mon;
    pxDatetime->ucDay=tmp->tm_mday;
    pxDatetime->ucHour=tmp->tm_hour;
    pxDatetime->ucMinute=tmp->tm_min;
    pxDatetime->ucSecond=tmp->tm_sec;
    pxDatetime->ucWday=tmp->tm_wday;
    RTC_LOGI("%s year:%d mon:%d day:%d hour:%d min:%d sec:%d wd:%d\n",__func__,pxDatetime->usYear,pxDatetime->ucMonth,
    pxDatetime->ucDay,pxDatetime->ucHour,pxDatetime->ucMinute,pxDatetime->ucSecond,pxDatetime->ucWday);
    return IOT_RTC_SUCCESS;
}  
static void rtc_cb(aon_rtc_id_t id,uint8_t *name_p,void *param)
{
    RTC_LOGI("id:%d name:%s ",id,name_p);
    IotRtcHandle_t rtc_instance=(IotRtcDescriptor_t*)param;
    if(rtc_instance->func)
    {
        rtc_instance->func(rtc_instance->status,rtc_instance->arg2);
        rtc_instance->func=NULL;
    }
    RTC_LOGI(" :%s\n",__func__);
}

static void rtc_wake_cb(aon_rtc_id_t id,uint8_t *name_p,void *param)
{
    RTC_LOGI("%s ",__func__);
    IotRtcHandle_t rtc_instance=(IotRtcDescriptor_t*)param;
    if(rtc_instance->func)
    {
        rtc_instance->func(rtc_instance->status,rtc_instance->arg2);
        rtc_instance->func=NULL;

    }
    RTC_LOGI(" id:%d name:%s \n",id,name_p);
}

void iot_rtc_set_callback( IotRtcHandle_t const pxRtcHandle,
                           IotRtcCallback_t xCallback,
                           void * pvUserContext )
{
    if(pxRtcHandle==NULL)
    {
        RTC_LOGE("Callback Invalid Handle\n");
        return;
    }

    IotRtcHandle_t rtc_instance=pxRtcHandle;
    rtc_instance->func=xCallback;
    rtc_instance->arg2=pvUserContext;
}                           

int32_t iot_rtc_ioctl( IotRtcHandle_t const pxRtcHandle,
                       IotRtcIoctlRequest_t xRequest,
                       void * const pvBuffer )
{
    if((pxRtcHandle==NULL)||(xRequest==-1))
    {
        return IOT_RTC_INVALID_VALUE;
    }
    IotRtcHandle_t rtc_instance=pxRtcHandle;

    struct timeval get_time;
    struct tm tmp;

    switch(xRequest)
    {
        case eSetRtcAlarm:
        {
            if(rtc_instance->status==eRtcTimerStopped)
            {
                RTC_LOGE("eSetRtcAlarm Rtc not start\n");
                return IOT_RTC_NOT_STARTED;
            }
            gettimeofday(&get_time,NULL);
            IotRtcDatetime_t *user_time=(IotRtcDatetime_t*)pvBuffer;
            if((user_time->ucMonth>LIMIT_VAL_MONTH)||(user_time->ucDay>LIMIT_VAL_DAY)||(user_time->ucHour>LIMIT_VAL_HOUR)
            ||(user_time->ucMinute>LIMIT_VAL_MINUTE)||(user_time->ucSecond>LIMIT_VAL_SECOND)||(user_time->ucWday>LIMIT_VAL_WDAY))
            {
                return IOT_RTC_SET_FAILED;
            }
            tmp.tm_sec=user_time->ucSecond;
            tmp.tm_min=user_time->ucMinute;
            tmp.tm_hour=user_time->ucHour;
            tmp.tm_mday=user_time->ucDay;
            tmp.tm_mon=user_time->ucMonth;
            tmp.tm_year=user_time->usYear;
            tmp.tm_wday=user_time->ucWday;

            rtc_instance->uUTC_time=mktime(&tmp)-get_time.tv_sec;
            if(rtc_instance->uUTC_time<=0)
            {
                RTC_LOGE("Set Invalid Alarm \n");
                return IOT_RTC_INVALID_VALUE;
            }

            alarm_info_t alarm_info;
            alarm_info.name[0]='S';
            alarm_info.name[1]='e';
            alarm_info.name[2]='t';
            alarm_info.name[3]='R';
            alarm_info.name[4]='t';
            alarm_info.name[5]='c';

            alarm_info.period_tick=(uint64_t)rtc_instance->uUTC_time*bk_rtc_get_clock_freq();//switch to tick cnt
            RTC_LOGI("eSetRtcAlarm Tgt:%d period:%d ",rtc_instance->uUTC_time,alarm_info.period_tick);
            alarm_info.period_cnt=1;
            alarm_info.param_p=(void*)rtc_instance;
            alarm_info.callback=rtc_cb;
            bk_alarm_register(AON_RTC_ID_1,&alarm_info);
            os_memcpy(rtc_instance->alarmName,alarm_info.name,ALARM_NAME_MAX_LEN);
            rtc_instance->uUTC_time=mktime(&tmp);
            rtc_instance->status=eRtcTimerAlarmTriggered;
            RTC_LOGI(" status:%d",rtc_instance->status);
        }
        break;
        case eGetRtcAlarm:
        {
            if(rtc_instance->status!=eRtcTimerAlarmTriggered)
            {
                return IOT_RTC_NOT_STARTED;
            }
            gettimeofday(&get_time,NULL);
            struct tm *get_alarm;
            get_alarm=gmtime(&rtc_instance->uUTC_time);
            IotRtcDatetime_t *user_time=(IotRtcDatetime_t*)pvBuffer;
            user_time->usYear=get_alarm->tm_year;
            user_time->ucMonth=get_alarm->tm_mon;
            user_time->ucDay=get_alarm->tm_mday;
            user_time->ucHour=get_alarm->tm_hour;
            user_time->ucMinute=get_alarm->tm_min;
            user_time->ucSecond=get_alarm->tm_sec;
            user_time->ucWday=get_alarm->tm_wday;
            RTC_LOGI("eGetRtcAlarm status:%d \n",rtc_instance->status);
        }
        break;
        case eCancelRtcAlarm:
        {
            if(rtc_instance->status!=eRtcTimerAlarmTriggered)
            {
                return IOT_RTC_SET_FAILED;
            }
            bk_alarm_unregister(AON_RTC_ID_1,rtc_instance->alarmName);
            rtc_instance->status=eRtcTimerStopped;
        }
        break;

        case eGetRtcStatus:
        {
            IotRtcStatus_t *rtc_status=(IotRtcStatus_t *)pvBuffer;
            *rtc_status=rtc_instance->status;
            RTC_LOGI("eGetRtcStatus:%d\n",*rtc_status);
        }
        break;
        case eSetRtcWakeupTime:
        {
            if(rtc_instance->status==eRtcTimerStopped)
            {
                RTC_LOGE("eSetRtcWakeupTime Rtc not start\n");
                return IOT_RTC_NOT_STARTED;
            }
//            return IOT_RTC_FUNCTION_NOT_SUPPORTED;
            uint32_t *uWakeUpTimeMs=(uint32_t*)pvBuffer;
            if((*uWakeUpTimeMs)<=0)
            {
                RTC_LOGE("eSetRtcWakeupTime param err:%d",(*uWakeUpTimeMs));
                return IOT_RTC_INVALID_VALUE;
            }
            alarm_info_t alarm_info;
            alarm_info.name[0]='S';
            alarm_info.name[1]='e';
            alarm_info.name[2]='t';
            alarm_info.name[3]='W';
            alarm_info.name[4]='A';
            alarm_info.name[5]='K';

            alarm_info.period_tick=(*uWakeUpTimeMs)*bk_rtc_get_clock_freq()/1000;//switch to tick cnt
            alarm_info.period_cnt=1;
            alarm_info.param_p=(void*)rtc_instance;
            alarm_info.callback=rtc_wake_cb;
            bk_alarm_register(AON_RTC_ID_1,&alarm_info);
            os_memcpy(rtc_instance->alarmName,alarm_info.name,ALARM_NAME_MAX_LEN);
            rtc_instance->status=eRtcTimerWakeupTriggered;
            rtc_instance->uWakeUpTick=alarm_info.period_tick;//(*uWakeUpTimeMs)*bk_rtc_get_clock_freq()/1000;
            RTC_LOGI("eSetRtcWakeupTime Tgt_ms:%d period:%d curTick:%d\n",(*uWakeUpTimeMs),rtc_instance->uWakeUpTick,alarm_info.period_tick);
        }        
        break;
        case eGetRtcWakeupTime:
        {
            if(rtc_instance->status!=eRtcTimerWakeupTriggered)
            {
                return IOT_RTC_NOT_STARTED;
            }
 //           return IOT_RTC_FUNCTION_NOT_SUPPORTED;
            uint32_t *uGetMs=(uint32_t *)pvBuffer;
            (*uGetMs)=rtc_instance->uWakeUpTick*1000/bk_rtc_get_clock_freq();

            RTC_LOGI("eGetRtcWakeupTime :%dms \n",(*uGetMs));
        }        
        break;
        case eCancelRtcWakeup:
        {
            if(rtc_instance->status!=eRtcTimerWakeupTriggered)
            {
                return IOT_RTC_SET_FAILED;
            }
  //          return IOT_RTC_FUNCTION_NOT_SUPPORTED;
            bk_alarm_unregister(AON_RTC_ID_1,rtc_instance->alarmName);
            rtc_instance->status=eRtcTimerStopped;
        }
        break;
        
        default:
        {
            return IOT_RTC_INVALID_VALUE;
        }
        break;
    }
    
    return IOT_RTC_SUCCESS;
}                       

int32_t iot_rtc_close( IotRtcHandle_t const pxRtcHandle )
{
    if(pxRtcHandle==NULL)
    {
        RTC_LOGE("%s Invalid Handler\n",__func__);
        return IOT_RTC_INVALID_VALUE;
    }

    if(is_rtc_opened==false)
    {
        RTC_LOGE("%s aleady closed\n",__func__);
        return IOT_RTC_INVALID_VALUE;
    }
    is_rtc_opened=false;
    os_free(pxRtcHandle);
    RTC_LOGI("%s sucess\n",__func__);
    return IOT_RTC_SUCCESS;
}

