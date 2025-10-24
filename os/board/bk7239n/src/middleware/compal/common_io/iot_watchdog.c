// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdint.h>
#include <string.h>
#include "driver/wdt.h"
#include "iot_watchdog.h"
#include "os/mem.h"
#include "components/system.h"
#include "os/os.h"

typedef struct {
    void (*func)(void *arg);
    void *arg;
    IotWatchdogStatus_t status;
} wdt_ctx_t;

wdt_ctx_t wdt_ctx = {0};

static bool s_wdt_driver_init_flag;
static uint32_t s_wdt_bite_time = 5000;

IotWatchdogHandle_t iot_watchdog_open( int32_t lWatchdogInstance )
{
    if(lWatchdogInstance < 0){
        return NULL;
    }

    if (true == s_wdt_driver_init_flag){
        return NULL;;
    }

    if(false == bk_wdt_is_driver_inited()){
        bk_wdt_driver_init();
    }

    s_wdt_driver_init_flag = true;

    return (IotWatchdogHandle_t) 1;
}

int32_t iot_watchdog_start( IotWatchdogHandle_t const pxWatchdogHandle )
{
    if(pxWatchdogHandle == NULL) {
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    if(0 == s_wdt_bite_time){
        return IOT_WATCHDOG_TIME_NOT_SET;
    }

    //if the bite time less than the bark time
    if((s_wdt_bite_time <= bk_wdt_get_feed_time()) || (bk_wdt_start(s_wdt_bite_time) != BK_OK)) {
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    wdt_ctx.status = eWatchdogTimerRunning;

    return IOT_WATCHDOG_SUCCESS;
}

int32_t iot_watchdog_stop( IotWatchdogHandle_t const pxWatchdogHandle )
{
    if((pxWatchdogHandle == NULL) || (bk_wdt_stop() != BK_OK)){
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    s_wdt_bite_time = 0;
    wdt_ctx.status = eWatchdogTimerStopped;

    return IOT_WATCHDOG_SUCCESS;
}

int32_t iot_watchdog_restart( IotWatchdogHandle_t const pxWatchdogHandle )
{
    if(pxWatchdogHandle == NULL) {
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    if(0 == s_wdt_bite_time){
        return IOT_WATCHDOG_TIME_NOT_SET;
    }

    if(BK_OK != bk_wdt_feed()){
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    return IOT_WATCHDOG_SUCCESS;
}

void bk_wdt_cb()
{
    if (wdt_ctx.func) {
        wdt_ctx.status = eWatchdogTimerBarkExpired;
        (wdt_ctx.func)(wdt_ctx.arg);
        wdt_ctx.func = NULL;
    }
}

void iot_watchdog_set_callback( IotWatchdogHandle_t const pxWatchdogHandle,
                                IotWatchdogCallback_t xCallback,
                                void * pvUserContext )
{
    if((pxWatchdogHandle == NULL) || (xCallback == NULL)){
        return ;
    }
    wdt_ctx.func = xCallback;
    wdt_ctx.arg = pvUserContext;
}

int32_t iot_watchdog_ioctl( IotWatchdogHandle_t const pxWatchdogHandle,
                            IotWatchdogIoctlRequest_t xRequest,
                            void * const pvBuffer )
{
    uint32_t feed_wdt = 0;

    if((pxWatchdogHandle == NULL) || (pvBuffer == NULL)){
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    switch(xRequest){
        case eSetWatchdogBiteTime:

             if ((*(uint32_t *) pvBuffer) <= bk_wdt_get_feed_time()){
                 return IOT_WATCHDOG_INVALID_VALUE;
             }

            s_wdt_bite_time = *(uint32_t *) pvBuffer;

            break;
        case eGetWatchdogBiteTime:

            os_memcpy(pvBuffer,&s_wdt_bite_time,sizeof(s_wdt_bite_time));

            break;
        case eSetWatchdogBarkTime:

             if ((s_wdt_bite_time != 0) && (*(uint32_t *) pvBuffer) >= s_wdt_bite_time){
                 return IOT_WATCHDOG_INVALID_VALUE;
             }

            feed_wdt = (*(uint32_t *) pvBuffer);
            bk_wdt_set_feed_time(feed_wdt);

            break;
        case eGetWatchdogBarkTime:

            feed_wdt = bk_wdt_get_feed_time();
            os_memcpy(pvBuffer,&feed_wdt,sizeof(feed_wdt));

            break;
        case eGetWatchdogStatus:

            os_memcpy(pvBuffer,&(wdt_ctx.status) ,sizeof(wdt_ctx.status));

            break;
        case eSetWatchdogBiteBehaviour:
            return IOT_WATCHDOG_FUNCTION_NOT_SUPPORTED;
        default:
            return IOT_WATCHDOG_INVALID_VALUE;
    }

    return IOT_WATCHDOG_SUCCESS;
}

int32_t iot_watchdog_close( IotWatchdogHandle_t const pxWatchdogHandle )
{
    int32_t rtn = IOT_WATCHDOG_SUCCESS;

    if(pxWatchdogHandle == NULL){
        return IOT_WATCHDOG_INVALID_VALUE;
    }

    if((false == bk_wdt_is_driver_inited()) && (true == s_wdt_driver_init_flag)){
        s_wdt_driver_init_flag = false;
        s_wdt_bite_time = 0;
    }
    else if (false == s_wdt_driver_init_flag){
        rtn = IOT_WATCHDOG_INVALID_VALUE;
    }
    else{
        if(iot_watchdog_stop( pxWatchdogHandle) != IOT_WATCHDOG_SUCCESS){
            return IOT_WATCHDOG_INVALID_VALUE;
        }
        bk_wdt_driver_deinit();
        s_wdt_driver_init_flag = false;
    }

    return rtn;
}
