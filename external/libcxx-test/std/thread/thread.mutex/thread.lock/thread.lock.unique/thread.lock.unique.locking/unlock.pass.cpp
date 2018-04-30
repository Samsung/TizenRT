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

// static int unlock();

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

static bool unlock_called = false;

struct mutex
{
    void lock() {}
    void unlock() {unlock_called = true;}
};

static mutex m;

int tc_libcxx_thread_thread_lock_unique_locking_unlock(void)
{
    std::unique_lock<mutex> lk(m);
    lk.unlock();
    TC_ASSERT_EXPR(unlock_called == true);
    TC_ASSERT_EXPR(lk.owns_lock() == false);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        lk.unlock();
        TC_ASSERT_EXPR(false);
    }
    catch (std::system_error& e)
    {
        TC_ASSERT_EXPR(e.code().value() == EPERM);
    }
#endif
    lk.release();
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        lk.unlock();
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
