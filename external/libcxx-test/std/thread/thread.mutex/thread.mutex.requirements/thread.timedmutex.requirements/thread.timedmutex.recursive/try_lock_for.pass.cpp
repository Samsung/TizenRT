//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads

// <mutex>

// class recursive_timed_mutex;

// template <class Rep, class Period>
//     bool try_lock_for(const chrono::duration<Rep, Period>& rel_time);

#include <mutex>
#include <thread>
#include <cstdlib>
#include <cassert>
#include "libcxx_tc_common.h"

static std::recursive_timed_mutex m;

typedef std::chrono::steady_clock Clock;
typedef Clock::time_point time_point;
typedef Clock::duration duration;
typedef std::chrono::milliseconds ms;
typedef std::chrono::nanoseconds ns;

static int f1()
{
    time_point t0 = Clock::now();
    TC_ASSERT_EXPR(m.try_lock_for(ms(300)) == true);
    time_point t1 = Clock::now();
    TC_ASSERT_EXPR(m.try_lock());
    m.unlock();
    m.unlock();
    ns d = t1 - t0 - ms(250);
    TC_ASSERT_EXPR(d < ns(50000000));  // within 50ms
    return 0;
}

static int f2()
{
    time_point t0 = Clock::now();
    TC_ASSERT_EXPR(m.try_lock_for(ms(250)) == false);
    time_point t1 = Clock::now();
    ns d = t1 - t0 - ms(250);
    TC_ASSERT_EXPR(d < ns(50000000));  // within 50ms
    return 0;
}

int tc_libcxx_thread_thread_timedmutex_recursive_try_lock_for(void)
{
    {
        m.lock();
        std::thread t(f1);
        std::this_thread::sleep_for(ms(250));
        m.unlock();
        t.join();
    }
    {
        m.lock();
        std::thread t(f2);
        std::this_thread::sleep_for(ms(300));
        m.unlock();
        t.join();
    }
    TC_SUCCESS_RESULT();
    return 0;
}
