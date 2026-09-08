//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions

// bitset<N>& reset(size_t pos); // constexpr since C++23

// Make sure we throw std::out_of_range when calling reset() on an OOB index.

#include <bitset>
#include <cassert>
#include <stdexcept>
#include "libcxx_tc_common.h"

int tc_utilities_template_bitset_bitset_members_reset_one_out_of_range(void) {
    {
        std::bitset<0> v;
#ifndef _LIBCPP_NO_EXCEPTIONS
        try { v.reset(0); TC_ASSERT_EXPR(false); } catch (std::out_of_range const&) { }
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    {
        std::bitset<1> v("0");
#ifndef _LIBCPP_NO_EXCEPTIONS
        try { v.reset(2); TC_ASSERT_EXPR(false); } catch (std::out_of_range const&) { }
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    {
        std::bitset<10> v("0000000000");
#ifndef _LIBCPP_NO_EXCEPTIONS
        try { v.reset(10); TC_ASSERT_EXPR(false); } catch (std::out_of_range const&) { }
    }
#endif // _LIBCPP_NO_EXCEPTIONS

    return 0;
}
