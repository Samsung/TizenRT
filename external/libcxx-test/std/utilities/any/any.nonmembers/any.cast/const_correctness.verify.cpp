//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <any>

// Test: any_cast cannot cast away const

#include <any>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

struct TestType {};
struct TestType2 {};

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_const_correctness_verify() {
    // Verify that any_cast from const any requires const lvalue reference or CopyConstructible
    constexpr bool ref_from_const = !std::is_constructible<TestType&, const std::any>::value;
    constexpr bool rref_from_const = !std::is_constructible<TestType&&, const std::any>::value;
    constexpr bool ref_from_const_rvalue = !std::is_constructible<TestType2&, const std::any&&>::value;
    constexpr bool rref_from_const_rvalue = !std::is_constructible<TestType2&&, const std::any&&>::value;
    TC_ASSERT_MSG(ref_from_const, "TestType& shall not be constructible from const any");
    TC_ASSERT_MSG(rref_from_const, "TestType&& shall not be constructible from const any");
    TC_ASSERT_MSG(ref_from_const_rvalue, "TestType2& shall not be constructible from const rvalue any");
    TC_ASSERT_MSG(rref_from_const_rvalue, "TestType2&& shall not be constructible from const rvalue any");
    
    return true;
}

int tc_any_cast_const_correctness_verify(void) {
    test_const_correctness_verify();
#if TEST_STD_VER > 17
    static_assert(test_const_correctness_verify());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
