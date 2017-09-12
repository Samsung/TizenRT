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
//
//
//*********************************************************************

// Defining _POSIX_C_SOURCE macro with 200809L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1-2008 base
// specification (excluding the XSI extension).
// For POSIX.1-2008 base specification,
// Refer http://pubs.opengroup.org/stage7tc1/
//
// For this specific file, see use of usleep
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif // _POSIX_C_SOURCE

#include "iotivity_config.h"
#include <gtest/gtest.h>

#include "octhread.h"
#include <cathreadpool.h>

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#define HNS_PER_US 10

//#define DEBUG_VERBOSE 1

// The debug print lines are left in for now since the output can be
// helpful for developers trying to debug or extend the tests.
// However, by default they are #defined out so as not to get in
// the way of normal test runs.
#ifdef DEBUG_VERBOSE
#define DBG_printf(...) printf(__VA_ARGS__)
#else
#define DBG_printf(...)
#endif

static const uint64_t USECS_PER_SEC = 1000000;

static const uint64_t USECS_PER_MSEC = 1000;

static const int MINIMAL_LOOP_SLEEP = 20;
static const int MINIMAL_EXTRA_SLEEP = 25;

uint64_t getAbsTime()
{
    uint64_t currentTime=0;
#if _POSIX_TIMERS > 0
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    currentTime = ts.tv_sec * USECS_PER_SEC + ts.tv_nsec / 1000;
#elif defined(_WIN32)
    FILETIME time;
    ULARGE_INTEGER microseconds;

    GetSystemTimeAsFileTime(&time);

    // Time is in hundreds of nanoseconds, so we must convert to uS
    microseconds.LowPart = time.dwLowDateTime;
    microseconds.HighPart = time.dwHighDateTime;
    microseconds.QuadPart /= HNS_PER_US;

    currentTime = microseconds.QuadPart;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    currentTime = tv.tv_sec * USECS_PER_SEC + tv.tv_usec;
#endif
    return currentTime;
}

TEST(MutexTests, TC_01_CREATE)
{
    oc_mutex mymutex = oc_mutex_new();

    EXPECT_TRUE(mymutex != NULL);
    if (mymutex != NULL)
    {
        oc_mutex_free(mymutex);
    }
}

typedef struct _tagFunc1
{
    oc_mutex mutex;
    volatile bool thread_up;
    volatile bool finished;
} _func1_struct;

void mutexFunc(void *context)
{
    _func1_struct* pData = (_func1_struct*) context;

    DBG_printf("Thread: trying to lock\n");

    // setting the flag must be done before lock attempt, as the test
    // thread starts off with the mutex locked
    pData->thread_up = true;
    oc_mutex_lock(pData->mutex);

    DBG_printf("Thread: got lock\n");
    usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
    DBG_printf("Thread: releasing\n");

    pData->finished = true; // assignment guarded by lock

    oc_mutex_unlock(pData->mutex);
}

TEST(MutexTests, TC_03_THREAD_LOCKING)
{
    ca_thread_pool_t mythreadpool;

    EXPECT_EQ(CA_STATUS_OK, ca_thread_pool_init(3, &mythreadpool));

    _func1_struct pData = {0, false, false};

    pData.mutex = oc_mutex_new();

    EXPECT_TRUE(pData.mutex != NULL);
    if (pData.mutex != NULL)
    {
        DBG_printf("test: Holding mutex in test\n");
        oc_mutex_lock(pData.mutex);

        DBG_printf("test: starting thread\n");
        //start thread
        EXPECT_EQ(CA_STATUS_OK,
                  ca_thread_pool_add_task(mythreadpool, mutexFunc, &pData));

        DBG_printf("test: waiting for thread to be up.\n");

        while (!pData.thread_up)
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
        }
        // At this point the thread is running and close to trying to lock.
        // For test purposes only, use of condition variables is being avoided,
        // so a minor sleep is used.
        usleep(MINIMAL_EXTRA_SLEEP * USECS_PER_MSEC);

        DBG_printf("test: unlocking\n");

        oc_mutex_unlock(pData.mutex);

        DBG_printf("test: waiting for thread to release\n");
        while (!pData.finished)
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
        }

        oc_mutex_lock(pData.mutex);

        // Cleanup Everything

        oc_mutex_unlock(pData.mutex);
        oc_mutex_free(pData.mutex);
    }

    ca_thread_pool_free(mythreadpool);
}

TEST(ConditionTests, TC_01_CREATE)
{
    oc_cond mycond = oc_cond_new();

    EXPECT_TRUE(mycond != NULL);
    if (mycond != NULL)
    {
        oc_cond_free(mycond);
    }
}

// Normally we would use one pair of mutex/cond-var communicating to the
// worker threads and one pair back to the main thread. However since
// testing the oc_cond itself is the point, only one pair is used here.
typedef struct _tagFunc2
{
    int id;
    oc_mutex mutex;
    oc_cond condition;
    volatile bool thread_up;
    volatile bool finished;
} _func2_struct;

void condFunc(void *context)
{
    _func2_struct* pData = (_func2_struct*) context;

    DBG_printf("Thread_%d: waiting on condition\n", pData->id);

    oc_mutex_lock(pData->mutex);

    pData->thread_up = true;

    oc_cond_wait(pData->condition, pData->mutex);

    pData->finished = true; // assignment guarded by lock

    oc_mutex_unlock(pData->mutex);

    DBG_printf("Thread_%d: completed.\n", pData->id);
}

#ifdef _WIN32
/** @todo: Enable.  Need to solve nanosleep issue */
TEST(ConditionTests, DISABLED_TC_02_SIGNAL)
#else
TEST(ConditionTests, TC_02_SIGNAL)
#endif
{
    const int MAX_WAIT_MS = 2000;
    ca_thread_pool_t mythreadpool;

    EXPECT_EQ(CA_STATUS_OK, ca_thread_pool_init(3, &mythreadpool));

    oc_mutex sharedMutex = oc_mutex_new();
    oc_cond sharedCond = oc_cond_new();

    _func2_struct pData1 =
    { 1, sharedMutex, sharedCond, false, false };
    _func2_struct pData2 =
    { 2, sharedMutex, sharedCond, false, false };

    EXPECT_TRUE(pData1.mutex != NULL);
    if (pData1.mutex != NULL)
    {
        DBG_printf("starting thread\n");
        // start threads
        EXPECT_EQ(CA_STATUS_OK,
                  ca_thread_pool_add_task(mythreadpool, condFunc, &pData1));
        EXPECT_EQ(CA_STATUS_OK,
                  ca_thread_pool_add_task(mythreadpool, condFunc, &pData2));

        DBG_printf("test    : sleeping\n");

        while (!pData1.thread_up || !pData2.thread_up)
        {
            // For test purposes only, use of condition variables is being
            // avoided, so a minor sleep is used.
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
        }
        // At this point the threads are running and both have locked. One
        // has already started waiting on the condition and the other is at
        // least close.

        oc_mutex_lock(sharedMutex);
        // once the lock is acquired it means both threads were waiting.
        DBG_printf("test    : signaling first thread\n");
        oc_cond_signal(sharedCond);
        oc_mutex_unlock(sharedMutex);

        // At this point either of the child threads might lock the mutex in
        // their cond_wait call, or this test thread might lock it again if
        // mutex_lock gets executed before the child threads can react to
        // the signaling. Thus we wait on their flag variables
        int waitCount = 1; // start with 1 for minumum targetWait value.
        while (!pData1.finished && !pData2.finished)
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
            waitCount++;
        }

        // As a rough hueristic wait twice as long for the second to possibly
        // finish:
        int targetWait = waitCount * 2;
        for (int i = 0;
             (i < targetWait) && (!pData1.finished && !pData2.finished); i++)
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
        }
        usleep(MINIMAL_EXTRA_SLEEP);

        // only one should be finished
        oc_mutex_lock(sharedMutex);
        EXPECT_NE(pData1.finished, pData2.finished);
        oc_mutex_unlock(sharedMutex);

        DBG_printf("test    : signaling another thread\n");

        oc_mutex_lock(sharedMutex);
        oc_cond_signal(sharedCond);
        oc_mutex_unlock(sharedMutex);

        waitCount = 0;
        while ((!pData1.finished || !pData2.finished)
               && ((waitCount * MINIMAL_EXTRA_SLEEP) < MAX_WAIT_MS))
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
            waitCount++;
        }

        // both should finally be finished
        EXPECT_TRUE(pData1.finished);
        EXPECT_TRUE(pData2.finished);

        // Cleanup Everything

        oc_mutex_free(pData1.mutex);
    }

    oc_cond_free(pData1.condition);

    ca_thread_pool_free(mythreadpool);
}

TEST(ConditionTests, TC_03_BROADCAST)
{
    const int MAX_WAIT_MS = 2000;
    ca_thread_pool_t mythreadpool;

    EXPECT_EQ(CA_STATUS_OK, ca_thread_pool_init(3, &mythreadpool));

    oc_mutex sharedMutex = oc_mutex_new();
    oc_cond sharedCond = oc_cond_new();

    _func2_struct pData1 =
    { 1, sharedMutex, sharedCond, false, false };
    _func2_struct pData2 =
    { 2, sharedMutex, sharedCond, false, false };

    EXPECT_TRUE(pData1.mutex != NULL);
    if (pData1.mutex != NULL)
    {
        DBG_printf("starting thread\n");
        // start threads
        EXPECT_EQ(CA_STATUS_OK,
                  ca_thread_pool_add_task(mythreadpool, condFunc, &pData1));
        EXPECT_EQ(CA_STATUS_OK,
                  ca_thread_pool_add_task(mythreadpool, condFunc, &pData2));

        DBG_printf("test    : sleeping\n");

        while (!pData1.thread_up || !pData2.thread_up)
        {
            // For test purposes only, use of condition variables is being
            // avoided, so a minor sleep is used.
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
        }
        // At this point the threads are running and both have locked. One
        // has already started waiting on the condition and the other is at
        // least close.

        DBG_printf("test    : signaling all threads\n");

        oc_mutex_lock(sharedMutex);
        // once the lock is acquired it means both threads were waiting.
        oc_cond_broadcast(sharedCond);
        oc_mutex_unlock(sharedMutex);

        int waitCount = 0;
        while ((!pData1.finished || !pData2.finished)
               && ((waitCount * MINIMAL_EXTRA_SLEEP) < MAX_WAIT_MS))
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
            waitCount++;
        }

        // both should finally be finished
        EXPECT_TRUE(pData1.finished);
        EXPECT_TRUE(pData2.finished);

        // Cleanup Everything

        oc_mutex_free(sharedMutex);
    }

    oc_cond_free(sharedCond);

    ca_thread_pool_free(mythreadpool);
}

#ifdef _WIN32
/** @todo: Enable.  Need to solve nanosleep issue */
TEST(ConditionTests, DISABLED_TC_04_TIMECHECK)
#else
TEST(ConditionTests, TC_04_TIMECHECK)
#endif
{
    uint64_t begin = getAbsTime();

    usleep(1);

    uint64_t end = getAbsTime();

    EXPECT_LT(begin, end); // should never be the same value
}

void timedFunc(void *context)
{
    _func2_struct* pData = (_func2_struct*) context;

    DBG_printf("Thread_%d: waiting for timeout \n", pData->id);

    oc_mutex_lock(pData->mutex);

    uint64_t abs = USECS_PER_SEC / 2; // 1/2 seconds

    // test UTIMEDOUT
    OCWaitResult_t ret = oc_cond_wait_for(pData->condition,
                                          pData->mutex, abs);
    EXPECT_EQ(OC_WAIT_TIMEDOUT, ret);

    pData->thread_up = true;

    DBG_printf("Thread_%d: waiting for signal \n", pData->id);

    abs = 5 * USECS_PER_SEC; // 5 seconds

    // test signal
    ret = oc_cond_wait_for(pData->condition, pData->mutex, abs);
    EXPECT_EQ(OC_WAIT_SUCCESS, ret);

    pData->finished = true; // assignment guarded by lock

    oc_mutex_unlock(pData->mutex);

    DBG_printf("Thread_%d: stopping\n", pData->id);
}

TEST(ConditionTests, TC_05_WAIT)
{
    const int MAX_WAIT_MS = 5000;
    ca_thread_pool_t mythreadpool;

    EXPECT_EQ(CA_STATUS_OK, ca_thread_pool_init(3, &mythreadpool));

    oc_mutex sharedMutex = oc_mutex_new();
    oc_cond sharedCond = oc_cond_new();

    _func2_struct pData1 =
    { 1, sharedMutex, sharedCond, false, false };

    EXPECT_TRUE(sharedMutex != NULL);
    if (sharedMutex != NULL)
    {
        DBG_printf("test    : starting thread\n");
        //start thread
        EXPECT_EQ(CA_STATUS_OK,
                  ca_thread_pool_add_task(mythreadpool, timedFunc, &pData1));

        DBG_printf("test    : waiting for thread to timeout once.\n");

        while (!pData1.thread_up)
        {
            // For test purposes only, use of condition variables is being
            // avoided, so a minor sleep is used.
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
        }


        DBG_printf("test    : signaling first thread\n");

        oc_mutex_lock(sharedMutex);
        oc_cond_signal(sharedCond);
        oc_mutex_unlock(sharedMutex);

        int waitCount = 0;
        while (!pData1.finished
               && ((waitCount * MINIMAL_EXTRA_SLEEP) < MAX_WAIT_MS))
        {
            usleep(MINIMAL_LOOP_SLEEP * USECS_PER_MSEC);
            waitCount++;
        }

        EXPECT_TRUE(pData1.finished); // thread should finally be finished

        // Cleanup Everything

        oc_mutex_free(sharedMutex);
    }

    oc_cond_free(sharedCond);

    ca_thread_pool_free(mythreadpool);
}

TEST(ConditionTests, TC_07_WAITDURATION)
{
    const double TARGET_WAIT = 1.125;

    oc_mutex sharedMutex = oc_mutex_new();
    oc_cond sharedCond = oc_cond_new();

    oc_mutex_lock(sharedMutex);

    uint64_t beg = getAbsTime();

    OCWaitResult_t ret = oc_cond_wait_for(sharedCond, sharedMutex,
                                          (uint64_t)(TARGET_WAIT * USECS_PER_SEC));
    EXPECT_EQ(OC_WAIT_TIMEDOUT,ret);

    uint64_t end = getAbsTime();

    double secondsDiff = (end - beg) / (double) USECS_PER_SEC;

#ifdef _WIN32
    // Windows does not guarantee that the thread will resume execution from a
    // yield within any given time frame. We will assume that the threads
    // should have resumed within one second of the requested timeout value.
    EXPECT_NEAR(TARGET_WAIT, secondsDiff, 1.00);
#else
    EXPECT_NEAR(TARGET_WAIT, secondsDiff, 0.05);
#endif

    oc_mutex_unlock(sharedMutex);

    // Cleanup Everything

    oc_mutex_free(sharedMutex);

    oc_cond_free(sharedCond);
}
