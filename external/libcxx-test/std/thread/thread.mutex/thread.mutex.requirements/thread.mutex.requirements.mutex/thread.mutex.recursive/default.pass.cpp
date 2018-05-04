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

// class recursive_mutex;

// recursive_mutex();

#include <mutex>
#include "libcxx_tc_common.h"

int tc_libcxx_thread_thread_mutex_recursive_default(void)
{
    static std::recursive_mutex m;
    TC_SUCCESS_RESULT();
    return 0;
}
