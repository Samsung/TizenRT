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

// template <class Mutex> class unique_lock;

// template <class Clock, class Duration>
//   bool try_lock_until(const chrono::time_point<Clock, Duration>& abs_time);

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

static bool try_lock_until_called = false;

struct mutex
{
    template <class Clock, class Duration>
        bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time)
    {
        typedef std::chrono::milliseconds ms;
        TC_ASSERT_EXPR(Clock::now() - abs_time < ms(5));
        try_lock_until_called = !try_lock_until_called;
        return try_lock_until_called;
    }
    void unlock() {}
};

static mutex m;

int tc_libcxx_thread_thread_lock_unique_locking_try_lock_until(void)
{
    typedef std::chrono::steady_clock Clock;
    std::unique_lock<mutex> lk(m, std::defer_lock);
    TC_ASSERT_EXPR(lk.try_lock_until(Clock::now()) == true);
    TC_ASSERT_EXPR(try_lock_until_called == true);
    TC_ASSERT_EXPR(lk.owns_lock() == true);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        TEST_IGNORE_NODISCARD lk.try_lock_until(Clock::now());
        TC_ASSERT_EXPR(false);
    }
    catch (std::system_error& e)
    {
        TC_ASSERT_EXPR(e.code().value() == EDEADLK);
    }
#endif
    lk.unlock();
    TC_ASSERT_EXPR(lk.try_lock_until(Clock::now()) == false);
    TC_ASSERT_EXPR(try_lock_until_called == false);
    TC_ASSERT_EXPR(lk.owns_lock() == false);
    lk.release();
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        TEST_IGNORE_NODISCARD lk.try_lock_until(Clock::now());
        TC_ASSERT_EXPR(false);
    }
    catch (std::system_error& e)
    {
        TC_ASSERT_EXPR(e.code().value() == EPERM);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
