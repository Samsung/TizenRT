/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "smartthings/ocf_things.h"
#include <iotbus/iotbus_gpio.h>

static pthread_mutex_t gGpiomutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gGpiocond;

#define GPIO_SIGNAL			\
do {										\
    pthread_mutex_lock(&gGpiomutex);			\
    pthread_cond_signal(&gGpiocond);			\
    pthread_mutex_unlock(&gGpiomutex);			\
} while(0)

#define GPIO_WAIT						\
do {										\
    pthread_mutex_lock(&gGpiomutex);			\
    pthread_cond_wait(&gGpiocond, &gGpiomutex);	\
    pthread_mutex_unlock(&gGpiomutex);			\
} while(0)

static int user_opinion = 0;

#define TAG                     "LIGHT_THINGS"

static const char* RES_SWITCH   = "/switch";

extern bool handle_get_request_on_switch(ocf_things_get_request_message_s* req_msg, ocf_things_representation_s* resp_rep);
extern bool handle_set_request_on_switch(ocf_things_set_request_message_s* req_msg, ocf_things_representation_s* resp_rep);


void handle_reset_result(bool result)
{
    printf("[%s]Reset %s.\n", result ? "succeeded" : "failed", TAG);
}

void handle_things_status_change(ocf_things_status_e things_status)
{
    printf("[%s]Things status is changed: %d\n", TAG, things_status);
}
int g_quit_flag = 0;

void handle_main_loop(void)
{
    // Run the loop to keep the thread 
//    signal(SIGINT, handle_sigint);
    while (!g_quit_flag)
    {
        sleep(1);
    }
}

void handle_sigint(int signum)
{
    #if 0
        if (signum == SIGINT)
        {
            printf("[%s]Interrupt received to terminate the application.\n", TAG);
            g_quit_flag = 1;
        }
    #endif
}

void gpio_callback_event(void *user_data)
{
    user_opinion = 1;
    GPIO_SIGNAL;
    
	return;
}

bool handle_reset_request(void)
{
    printf("[%s]Received a request for RESET. ( Btn : SW703 ) : \n", TAG);
    #if 0
    pthread_mutex_init(&gGpiomutex, NULL);
    iotbus_gpio_context_h m_gpio1 = iotbus_gpio_open(44); // right btn
    user_opinion = 0;

    iotbus_gpio_register_cb(m_gpio1, IOTBUS_GPIO_EDGE_RISING, gpio_callback_event, (void *)m_gpio1);
    GPIO_WAIT;
    iotbus_gpio_unregister_cb(m_gpio1);

    pthread_mutex_destroy(&gGpiomutex);

    return (1 == user_opinion) ? true : false;
    #endif
    return true;
}



bool handle_ownership_transfer_request(void)
{
    

    printf("[%s]Received a request for Ownership-transfer. ( Btn : SW703 ) : \n", TAG);
    #if 0
    pthread_mutex_init(&gGpiomutex, NULL);
    iotbus_gpio_context_h m_gpio1 = iotbus_gpio_open(44); // right btn
    user_opinion = 0;

    iotbus_gpio_register_cb(m_gpio1, IOTBUS_GPIO_EDGE_RISING, gpio_callback_event, (void *)m_gpio1);
    GPIO_WAIT;
    iotbus_gpio_unregister_cb(m_gpio1);

    pthread_mutex_destroy(&gGpiomutex);

    return (1 == user_opinion) ? true : false;
    #endif
    return true;
}


bool handle_get_request(ocf_things_get_request_message_s* req_msg, ocf_things_representation_s* resp_rep)
{
    printf("Received a GET request on %s\n", req_msg->resource_uri);
    
    bool ret = false;
    
    if(0 == strcmp(req_msg->resource_uri, RES_SWITCH))
    {
        ret = handle_get_request_on_switch(req_msg, resp_rep);
    }
    else
    {
        printf("Not supported uri.\n");
    }
    
    return ret;
}

bool handle_set_request(ocf_things_set_request_message_s* req_msg, ocf_things_representation_s* resp_rep)
{
    printf("Received a SET request on %s\n", req_msg->resource_uri);
    
    if(0 == strcmp(req_msg->resource_uri, RES_SWITCH))
    {
        handle_set_request_on_switch(req_msg, resp_rep);
    }
    else
    {
        return false;
    }
    
    return true;
}

int ess_process(int argc, char* argv[])
{
    bool easysetup_complete = false;
    ocf_things_initialize("/rom/sampleDevice.json", &easysetup_complete);
    iotapi_initialize();
       
    if (!easysetup_complete)
    {
        int cmd = 1;

        if (1 != cmd)
        {
            ocf_things_deinitialize();
            return 0;
        }
    }

    ocf_things_register_request_cb                  (handle_get_request, handle_set_request);
    ocf_things_register_reset_cb                    (handle_reset_request, handle_reset_result);
    ocf_things_register_user_confirm_cb             (handle_ownership_transfer_request);
    ocf_things_register_things_status_change_cb     (handle_things_status_change);

    ocf_things_start();

    printf("[%s]=====================================================\n", TAG);
    printf("[%s]                    Stack Started                    \n", TAG);
    printf("[%s]=====================================================\n", TAG);

    handle_main_loop();

    ocf_things_stop();
    ocf_things_deinitialize();

    printf("[%s]=====================================================\n", TAG);
    printf("[%s]                    Stack Stopped                    \n", TAG);
    printf("[%s]=====================================================\n", TAG);

    return 0;
}
