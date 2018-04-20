//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <mutex>

// struct once_flag;

// constexpr once_flag() noexcept;

#include <mutex>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_thread_thread_once_onceflag_default(void)
{
    {
    std::once_flag f;
    (void)f;
    }
#if TEST_STD_VER >= 11
    {
    constexpr std::once_flag f;
    (void)f;
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
