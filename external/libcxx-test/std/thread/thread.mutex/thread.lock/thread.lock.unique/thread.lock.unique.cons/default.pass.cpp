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

// unique_lock();

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_thread_thread_lock_unique_cons_default(void)
{
    std::unique_lock<std::mutex> ul;
    TC_ASSERT_EXPR(!ul.owns_lock());
    TC_ASSERT_EXPR(ul.mutex() == nullptr);
    TC_SUCCESS_RESULT();
    return 0;
}
