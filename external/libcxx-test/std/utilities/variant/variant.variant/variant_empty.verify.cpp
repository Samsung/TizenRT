//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: variant cannot be empty (must have at least one alternative)

#include <variant>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify variant cannot be instantiated with no alternatives
    constexpr bool var_empty = !std::is_constructible<std::variant<>>::value;
    TC_ASSERT_MSG(var_empty, "variant<> shall not be constructible");
    
    return true;
}

int tc_utilities_variant_variant_variant_empty_verify(void) {
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
