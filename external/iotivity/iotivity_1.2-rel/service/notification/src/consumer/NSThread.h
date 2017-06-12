//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef _NS_THREAD_H_
#define _NS_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>

typedef enum
{
    pthread
} NS_THREAD_MODEL;

#define NS_THREAD pthread

#if (NS_THREAD == pthread)
#include <pthread.h>

typedef struct
{
    bool isStarted;
    pthread_t thread_id;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;
} NSConsumerThread;

#endif

typedef void *(*NSThreadFunc)(void *);

#ifndef __TIZENRT__
NSConsumerThread * NSThreadInit(NSThreadFunc, void *);
#else
NSConsumerThread * NSThreadInit(NSThreadFunc, void *, const char *, int );
#endif

void NSThreadLock(NSConsumerThread *);

void NSThreadUnlock(NSConsumerThread *);

void NSThreadJoin(NSConsumerThread *);

void NSThreadStop(NSConsumerThread *);

void NSDestroyThreadHandle(NSConsumerThread *);

void NSThreadDetach();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _NS_THREAD_H_
