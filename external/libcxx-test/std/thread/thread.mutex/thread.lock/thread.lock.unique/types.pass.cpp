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

// template <class Mutex>
// class unique_lock
// {
// public:
//     typedef Mutex mutex_type;
//     ...
// };

#include <mutex>
#include <type_traits>
#include "libcxx_tc_common.h"

int tc_libcxx_thread_thread_lock_unique_types(void)
{
    static_assert((std::is_same<std::unique_lock<std::mutex>::mutex_type,
                   std::mutex>::value), "");
    TC_SUCCESS_RESULT();
    return 0;
}
