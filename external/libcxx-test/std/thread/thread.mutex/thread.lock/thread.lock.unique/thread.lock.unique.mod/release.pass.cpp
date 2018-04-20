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

// mutex_type* release() noexcept;

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

struct mutex
{
    static int lock_count;
    static int unlock_count;
    void lock() {++lock_count;}
    void unlock() {++unlock_count;}
};

int mutex::lock_count = 0;
int mutex::unlock_count = 0;

static mutex m;

int tc_libcxx_thread_thread_lock_unique_mod_release(void)
{
    std::unique_lock<mutex> lk(m);
    TC_ASSERT_EXPR(lk.mutex() == &m);
    TC_ASSERT_EXPR(lk.owns_lock() == true);
    TC_ASSERT_EXPR(mutex::lock_count == 1);
    TC_ASSERT_EXPR(mutex::unlock_count == 0);
    TC_ASSERT_EXPR(lk.release() == &m);
    TC_ASSERT_EXPR(lk.mutex() == nullptr);
    TC_ASSERT_EXPR(lk.owns_lock() == false);
    TC_ASSERT_EXPR(mutex::lock_count == 1);
    TC_ASSERT_EXPR(mutex::unlock_count == 0);
    TC_SUCCESS_RESULT();
    return 0;
}
