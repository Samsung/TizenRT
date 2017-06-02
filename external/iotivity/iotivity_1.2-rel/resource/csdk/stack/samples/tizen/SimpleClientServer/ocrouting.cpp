/* ****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <array>
#include <iostream>
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

using namespace std;

#define TAG "ocrouting"

int gQuitFlag = 0;
static GMainLoop *g_mainloop = NULL;
pthread_t g_thread;

/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

void *GMainLoopThread(void *param)
{

    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            cout << "\nOCStack process error";
            return NULL;
        }
#ifndef ROUTING_GATEWAY
        sleep(1);
#endif
    }

    if (g_mainloop)
    {
        g_main_loop_quit(g_mainloop);
    }
    return NULL;
}

int main()
{
    cout << "\nOCRouting sample is starting...";
    g_mainloop = g_main_loop_new(NULL, FALSE);
    if(!g_mainloop)
    {
        printf("g_main_loop_new failed\n");
        return 0;
    }

    if (OCInit(NULL, 0, OC_GATEWAY) != OC_STACK_OK)
    {
        cout << "\nOCStack init error";
        return 0;
    }

    // Break from loop with Ctrl-C
    cout << "\nEntering ocrouting main loop...";

    signal(SIGINT, handleSigInt);


    int result = pthread_create(&g_thread, NULL, GMainLoopThread, (void *)NULL);
    if (result < 0)
    {
        printf("pthread_create failed in initialize\n");
        return 0;
    }

    g_main_loop_run(g_mainloop);

    cout << "\nExiting ocrouting main loop...";

    if (OCStop() != OC_STACK_OK)
    {
        cout << "\nOCStack process error";
    }

    return 0;
}
