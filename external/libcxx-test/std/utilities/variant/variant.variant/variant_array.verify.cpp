//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: variant cannot contain array types

#include <variant>
#include <type_traits>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
    // Verify variant cannot be instantiated with array types
    constexpr bool var_incomplete = !std::is_constructible<std::variant<int, int[]>>::value;
    constexpr bool var_bound = !std::is_constructible<std::variant<int, int[42]>>::value;
    constexpr bool var_multi_dim = !std::is_constructible<std::variant<int, int[][42]>>::value;
    TC_ASSERT_MSG(var_incomplete, "variant<int, int[]> shall not be constructible");
    TC_ASSERT_MSG(var_bound, "variant<int, int[42]> shall not be constructible");
    TC_ASSERT_MSG(var_multi_dim, "variant<int, int[][42]> shall not be constructible");
    
    return true;
}

int tc_utilities_variant_variant_variant_array_verify(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
