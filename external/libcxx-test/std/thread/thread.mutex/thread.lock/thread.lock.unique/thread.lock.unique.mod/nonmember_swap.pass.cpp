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

// template <class Mutex>
//   static int swap(unique_lock<Mutex>& x, unique_lock<Mutex>& y);

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

struct mutex
{
    void lock() {}
    void unlock() {}
};

static mutex m;

int tc_libcxx_thread_thread_lock_unique_mod_nonmember_swap(void)
{
    std::unique_lock<mutex> lk1(m);
    std::unique_lock<mutex> lk2;
    swap(lk1, lk2);
    TC_ASSERT_EXPR(lk1.mutex() == nullptr);
    TC_ASSERT_EXPR(lk1.owns_lock() == false);
    TC_ASSERT_EXPR(lk2.mutex() == &m);
    TC_ASSERT_EXPR(lk2.owns_lock() == true);
    TC_SUCCESS_RESULT();
    return 0;
}
