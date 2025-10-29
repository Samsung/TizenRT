/**
 * @file iot_timer.c
 * @brief Beken Corp implementation of Common I/O timer API.
 * 
 * @copyright Copyright (c) 2022 Beken Corp Inc.
 * 
 */

/* FreeRTOS kernel layer */
#include "FreeRTOS.h"
#include "semphr.h"

/* timer driver layer */
#include "driver/hal/hal_timer_types.h"
#include "timer_hal.h"
#include "driver/timer.h"
#include "driver/timer_types.h"

/* timer iot layer */
#include "iot_timer.h"

#define IOT_TIMER_TAG "iot_timer"

/* attention (copied from timer_driver.c, be careful to check and sync)
 *  1. timer2 has been used for time cali
 *  2. timer3 has been used for clock, but bk7271 use timer1
 *  3. bk7271 has used timer0(10ms)
 */
#define USED_TIMER_ID TIMER_ID0

static void timer_callback(timer_id_t id);

typedef enum{
    IOT_TIMER_OPENED = 0,     /*!< timer opened flag */
    IOT_TIMER_CLOSED,         /*!< timer closed flag */
} timer_status_t;

typedef enum{
    IOT_COUNTER_STARTED = 0,  /*!< timer counter started flag */
    IOT_COUNTER_STOPED,       /*!< timer counter stoped flag */
} counter_status_t;

typedef struct IotTimerDescriptor
{
    timer_id_t timer_id;                /*!< timer id */
    uint64_t timeout_us;                /*!< timeout setting */      
    uint32_t last_count;                /*!< care about last count */
    timer_status_t timer_status;        /*!< timer status */
    counter_status_t counter_status;    /*!< counter status */
    timer_isr_t timer_isr;              /*!< timer interrupt service routine */
    IotTimerCallback_t xTimerCallback;  /*!< user defined callback function */
    void *pvUserContext;                /*!< user defined parameters */
} IotTimerDescriptor_t;

static IotTimerDescriptor_t xDefaultTimerDesc = 
{
    .timer_id = USED_TIMER_ID,
    .timeout_us = 0,
    .last_count = 0,
    .timer_status = IOT_TIMER_CLOSED,
    .counter_status = IOT_COUNTER_STOPED,
    .timer_isr = NULL,
    .xTimerCallback = NULL,
    .pvUserContext = NULL,
};

IotTimerHandle_t iot_timer_open(int32_t lTimerInstance)
{  
    if(USED_TIMER_ID != lTimerInstance){
        BK_LOGE(IOT_TIMER_TAG, "Invalid instance of timer %s\n", __func__);
        return NULL;
    }

    IotTimerHandle_t TimerDesc = &xDefaultTimerDesc;
    TimerDesc->timer_status = IOT_TIMER_OPENED;
    return TimerDesc;
}

int32_t iot_timer_close( IotTimerHandle_t const pxTimerHandle )
{
    if(NULL == pxTimerHandle){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    if(IOT_TIMER_CLOSED == pxTimerHandle->timer_status){
        BK_LOGE(IOT_TIMER_TAG, "Timer handler is not open %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    if(BK_OK != bk_timer_stop(pxTimerHandle->timer_id))
    {
        BK_LOGE(IOT_TIMER_TAG, "Timer stop failed %s\n", __func__);
        return IOT_TIMER_SET_FAILED;
    }
    /* Reset the timer handler to default */
    pxTimerHandle->timer_status = IOT_TIMER_CLOSED;
    pxTimerHandle->xTimerCallback = NULL;
    pxTimerHandle->counter_status = IOT_COUNTER_STOPED;
    pxTimerHandle->pvUserContext = NULL; 
    pxTimerHandle->timer_isr = NULL;
    pxTimerHandle->timeout_us = 0;
    pxTimerHandle->last_count = 0;
    return IOT_TIMER_SUCCESS;
}

static void timer_callback(timer_id_t id)
{
    IotTimerHandle_t TimerDesc = &xDefaultTimerDesc;
    BK_LOGD(IOT_TIMER_TAG, "Timer %d callback\n", id);
    if(NULL == TimerDesc->xTimerCallback){
        BK_LOGD(IOT_TIMER_TAG, "Null callback %s\n", __func__);
        return;        
    }
    TimerDesc->xTimerCallback(TimerDesc->pvUserContext);
    
    /* get the timer end count for time out setting*/
    TimerDesc->last_count = bk_timer_get_period(TimerDesc->timer_id);
    TimerDesc->timeout_us = 0;
    TimerDesc->xTimerCallback = NULL;
    TimerDesc->pvUserContext = NULL;
    bk_timer_cancel(id);
}

void iot_timer_set_callback( IotTimerHandle_t const pxTimerHandle,
                             IotTimerCallback_t xCallback,
                             void * pvUserContext )
{
    if(pxTimerHandle == NULL){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return ;
    }
    pxTimerHandle->timer_isr = timer_callback;
    pxTimerHandle->xTimerCallback = xCallback;
    pxTimerHandle->pvUserContext = pvUserContext;
}

int32_t iot_timer_start( IotTimerHandle_t const pxTimerHandle )
{
    if(NULL == pxTimerHandle){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    if(NULL == pxTimerHandle->timer_isr){
        if(BK_OK != bk_timer_enable(pxTimerHandle->timer_id)){
            BK_LOGE(IOT_TIMER_TAG, "Timer enable failed %s\n", __func__);            
        }
    } else if(BK_OK != bk_timer_delay_with_callback(pxTimerHandle->timer_id, pxTimerHandle->timeout_us, pxTimerHandle->timer_isr)){
        BK_LOGE(IOT_TIMER_TAG, "Timer start failed %s\n", __func__);
        return IOT_TIMER_SET_FAILED;
    }
    pxTimerHandle->counter_status = IOT_COUNTER_STARTED;
    return IOT_TIMER_SUCCESS;
}

int32_t iot_timer_stop( IotTimerHandle_t const pxTimerHandle )
{
    if(NULL == pxTimerHandle){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    if(BK_OK != bk_timer_stop(pxTimerHandle->timer_id)) {
        BK_LOGE(IOT_TIMER_TAG, "Timer stop failed %s\n", __func__);
        return IOT_TIMER_SET_FAILED;
    }
    pxTimerHandle->counter_status = IOT_COUNTER_STOPED;
    /* reset the last count to zero */
    pxTimerHandle->last_count = 0;
    return IOT_TIMER_SUCCESS;
}

int32_t iot_timer_get_value( IotTimerHandle_t const pxTimerHandle,
                             uint64_t * ullMicroSeconds )
{
    if(NULL == pxTimerHandle){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    if(NULL == ullMicroSeconds){
        BK_LOGE(IOT_TIMER_TAG, "Time buffer is NULL %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    if(IOT_COUNTER_STOPED == pxTimerHandle->counter_status){
        BK_LOGE(IOT_TIMER_TAG, "Timer counter not running %s\n", __func__);
        return IOT_TIMER_NOT_RUNNING;
    }
    *ullMicroSeconds = bk_timer_get_time(pxTimerHandle->timer_id, 1, pxTimerHandle->last_count, TIMER_UNIT_US);
    return IOT_TIMER_SUCCESS;
}

int32_t iot_timer_delay( IotTimerHandle_t const pxTimerHandle,
                         uint32_t ulDelayMicroSeconds )
{   
    if(NULL == pxTimerHandle){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    /* set time out value */
    pxTimerHandle->timeout_us = (uint64_t)ulDelayMicroSeconds;
    return  IOT_TIMER_SUCCESS;    
}

int32_t iot_timer_cancel( IotTimerHandle_t const pxTimerHandle )
{
    if(NULL == pxTimerHandle){
        BK_LOGE(IOT_TIMER_TAG, "Invalid handler %s\n", __func__);
        return IOT_TIMER_INVALID_VALUE;
    }
    /* this will cancel the timer delay and continue counting without resetting the count to zero */
    if(BK_OK != bk_timer_cancel(pxTimerHandle->timer_id)){
        BK_LOGE(IOT_TIMER_TAG, "Timer delay cancel failed %s\n", __func__);
        return IOT_TIMER_FUNCTION_NOT_SUPPORTED;
    }
    /* set time out to zero */
    pxTimerHandle->timeout_us = 0;
    /* set the timer isr to NULL */
    pxTimerHandle->timer_isr = NULL;
    return IOT_TIMER_SUCCESS;    
}

