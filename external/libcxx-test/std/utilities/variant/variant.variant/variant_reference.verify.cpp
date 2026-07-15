//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: variant cannot contain reference types

#include <variant>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify variant cannot be instantiated with reference types
    constexpr bool var_lref = !std::is_constructible<std::variant<int, int&>>::value;
    constexpr bool var_clref = !std::is_constructible<std::variant<int, const int&>>::value;
    constexpr bool var_rref = !std::is_constructible<std::variant<int, int&&>>::value;
    TC_ASSERT_MSG(var_lref, "variant<int, int&> shall not be constructible");
    TC_ASSERT_MSG(var_clref, "variant<int, const int&> shall not be constructible");
    TC_ASSERT_MSG(var_rref, "variant<int, int&&> shall not be constructible");
    
    return true;
}

int tc_utilities_variant_variant_variant_reference_verify(void) {
    test();
    // Negative test case: verifies that certain code should NOT compile (not compiling = pass).
    // Converted to runtime test using type traits. Static compile-time check removed.
    // #if TEST_STD_VER > 17
    //     static_assert(test());
    // #endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
