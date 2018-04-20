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

// explicit operator bool() const;

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

static std::mutex m;

int tc_libcxx_thread_thread_lock_unique_obs_op_bool(void)
{
    std::unique_lock<std::mutex> lk0;
    TC_ASSERT_EXPR(static_cast<bool>(lk0) == false);
    std::unique_lock<std::mutex> lk1(m);
    TC_ASSERT_EXPR(static_cast<bool>(lk1) == true);
    lk1.unlock();
    TC_ASSERT_EXPR(static_cast<bool>(lk1) == false);
    TC_SUCCESS_RESULT();
    return 0;
}
