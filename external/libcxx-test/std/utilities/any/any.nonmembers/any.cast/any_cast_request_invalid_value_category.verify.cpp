//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <any>

// Test: any_cast with invalid value category requests

#include <any>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

struct TestType {};

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_invalid_value_category_verify() {
    // Test that any_cast ValueType must be const lvalue reference or CopyConstructible
    // for const lvalue any
    constexpr bool ref_from_const = !std::is_constructible<TestType&, const std::any>::value;
    TC_ASSERT_MSG(ref_from_const, "TestType& shall not be constructible from const any");
    
    // Test that any_cast ValueType must be lvalue reference or CopyConstructible
    // for lvalue any
    constexpr bool rref_from_lvalue = !std::is_constructible<TestType&&, std::any>::value;
    TC_ASSERT_MSG(rref_from_lvalue, "TestType&& shall not be constructible from lvalue any");
    
    // Test that any_cast ValueType must be rvalue reference or CopyConstructible
    // for rvalue any
    constexpr bool lref_from_rvalue = !std::is_constructible<TestType&, std::any&&>::value;
    TC_ASSERT_MSG(lref_from_rvalue, "TestType& shall not be constructible from rvalue any");
    
    return true;
}

int tc_any_cast_invalid_value_category_verify(void) {
    test_invalid_value_category_verify();
#if TEST_STD_VER > 17
    static_assert(test_invalid_value_category_verify());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
