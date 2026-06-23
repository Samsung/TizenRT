//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// XFAIL: availability-bad_optional_access-missing

// <optional>

// class bad_optional_access : public exception

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
namespace {
TEST_CONSTEXPR_CXX20 bool test() {
    using std::bad_optional_access;

    constexpr bool is_base = std::is_base_of<std::exception, bad_optional_access>::value;
    constexpr bool is_convert = std::is_convertible<bad_optional_access*, std::exception*>::value;
    TC_ASSERT_MSG(is_base, "bad_optional_access shall be derived from std::exception");
    TC_ASSERT_MSG(is_convert, "bad_optional_access* shall be convertible to std::exception*");
    
    return true;
}
} // namespace

int tc_utilities_optional_optional_bad_optional_access_derive_pass(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
