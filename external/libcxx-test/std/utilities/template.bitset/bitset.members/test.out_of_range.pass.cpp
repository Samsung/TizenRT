//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions

// constexpr bool test(size_t pos) const;

// Make sure we throw std::out_of_range when calling test() on an OOB index.

#include <bitset>
#include <cassert>
#include <stdexcept>
#include "libcxx_tc_common.h"

int tc_utilities_template_bitset_bitset_members_test_out_of_range(void) {
    {
        std::bitset<0> v;
#ifndef _LIBCPP_NO_EXCEPTIONS
        try { (void) v.test(0); TC_ASSERT_EXPR(false); }
        catch (std::out_of_range const&) { }
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    {
        std::bitset<1> v("0");
#ifndef _LIBCPP_NO_EXCEPTIONS
        try { (void) v.test(2); TC_ASSERT_EXPR(false); }
        catch (std::out_of_range const&) { }
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    {
        std::bitset<10> v("0000000000");
#ifndef _LIBCPP_NO_EXCEPTIONS
        try { (void) v.test(10); TC_ASSERT_EXPR(false); }
        catch (std::out_of_range const&) { }
    }
#endif // _LIBCPP_NO_EXCEPTIONS

    return 0;
}
