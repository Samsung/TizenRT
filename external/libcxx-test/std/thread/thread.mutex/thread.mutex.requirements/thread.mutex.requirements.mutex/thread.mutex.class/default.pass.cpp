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

// class mutex;

// mutex();

#include <mutex>
#include <type_traits>
#include "libcxx_tc_common.h"

int tc_libcxx_thread_thread_mutex_class_default(void)
{
    static_assert(std::is_nothrow_default_constructible<std::mutex>::value, "");
    static std::mutex m;
    TC_SUCCESS_RESULT();
    return 0;
}
